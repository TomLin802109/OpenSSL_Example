﻿using Microsoft.Win32;
using System.ComponentModel;
using System.IO;
using System.Management;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Encryption;
using System.Collections.ObjectModel;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace LicenseGenerator
{
    
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ViewModel _vm = new ViewModel();
        private ByteData _key = null;
        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = _vm;
            //_vm.MachineSN = getBaseBoradSN(); //getCPU();
            _vm.MachineSNs.Add(getCPU());
            _vm.MachineSNs.Add(getBaseBoradSN());
            _vm.MachineSNs.AddRange(getMacAddress());
            _vm.MachineSN = _vm.MachineSNs.First();
            _vm.MainDir = AppDomain.CurrentDomain.BaseDirectory;
            loadLicenseFiles();
        }
        private string getCPU()
        {
            var cpu_id = string.Empty;
            var _moc = new ManagementClass("win32_processor").GetInstances();
            foreach (ManagementObject i in _moc)
            {
                cpu_id = i["ProcessorID"].ToString();
            }
            return cpu_id;
        }
        private string getBaseBoradSN()
        {
            var sn = string.Empty;
            var _moc = new ManagementClass("win32_baseboard").GetInstances();
            foreach (ManagementObject i in _moc)
            {
                sn = i["SerialNumber"].ToString();
            }
            return sn;
        }
        private string[] getMacAddress()
        {
            try
            {
                ManagementObjectSearcher objMOS = new ManagementObjectSearcher("Select * FROM Win32_NetworkAdapterConfiguration");
                var macLs = new List<string>();
                foreach (ManagementObject objMO in objMOS.Get())
                {
                    object tempMacAddrObj = objMO["MacAddress"];
                    if (tempMacAddrObj == null) //Skip objects without a MACAddress
                        continue;
                    macLs.Add(tempMacAddrObj.ToString());
                    //foreach (PropertyData prop in objMO.Properties)
                    //{
                    //    if (prop.Value != null)
                    //        Console.WriteLine("{0}: {1}", prop.Name, prop.Value);
                    //}
                    objMO.Dispose();
                }
                return macLs.ToArray();
            }
            catch (Exception e)
            {
                return Array.Empty<string>();
            }
        }
        private ByteData generateKey(string cpu_id)
        {
            var enc = new Encryptor();
            var cpu_b = Encoding.UTF8.GetBytes(cpu_id);
            return _key = enc.Encrypt(new ByteData(cpu_b));
        }

        private void loadLicenseFiles()
        {
            var license = Directory.EnumerateFiles(_vm.MainDir)
                .Select(v => v.Split("\\").Last())
                .Where(v => v.StartsWith("license_"));
            _vm.LicenseFiles = new ObservableCollection<LicenseModel>();
            _key = generateKey(_vm.MachineSN);
            foreach (var i in license)
            {
                var str = i.Split('_');
                if (str.Length != 3)
                    continue;
                var model = loadLicense(i);
                _vm.LicenseFiles.Add(model);
            }
        }

        private LicenseModel loadLicense(string fileName)
        {
            var str = fileName.Split('_');
            var cipher_b = File.ReadAllBytes(_vm.MainDir + fileName);
            var enc = new Encryptor();
            if (str[1] != "pc")
            {
                enc.SetKey(_key);
            }
            var sn_b = enc.Decrypt(new ByteData(cipher_b));
            var res = new LicenseModel()
            {
                Type = str[1],
                Model = str[2],
                SN = "Incorrect format"
            };
            if (sn_b.Count() <= 12)
                return res;
            var sn = Encoding.UTF8.GetString(sn_b.Skip(12).ToArray());
            var date = sn_b.Take(12).ToArray();
            try
            {
                res.SN = sn.Length > 0 ? sn : "No decryption result";
                res.LastDate = new DateTime(BitConverter.ToUInt16(date, 0), BitConverter.ToUInt16(date, 2), BitConverter.ToUInt16(date, 4));
                res.ExpDate = new DateTime(BitConverter.ToUInt16(date, 6), BitConverter.ToUInt16(date, 8), BitConverter.ToUInt16(date, 10));
            }
            catch { }
            return res;
        }

        private IEnumerable<byte> getBytes(DateTime date)
        {
            var b1 = BitConverter.GetBytes((ushort)date.Year);
            var b2 = BitConverter.GetBytes((ushort)date.Month);
            var b3 = BitConverter.GetBytes((ushort)date.Day);
            return b1.Concat(b2).Concat(b3);
        }

        private bool GenerateLicense(string fileName, string sn, bool defKey=true)
        {
            var in_b = Encoding.UTF8.GetBytes(sn);
            var enc = new Encryption.Encryptor();
            if (!defKey)
            {
                _key = generateKey(_vm.MachineSN);
                enc.SetKey(_key);
            }
            
            var res = enc.Encrypt(new Encryption.ByteData(getBytes(DateTime.Now).Concat(getBytes(_vm.ExpirationDate)).Concat(in_b)));
            
            File.WriteAllBytes(_vm.MainDir + fileName, res.ToArray());
            var cipher = File.ReadAllBytes(_vm.MainDir + fileName);
            var org_b = enc.Decrypt(new Encryption.ByteData(cipher)).ToArray();
            var org_str = Encoding.UTF8.GetString(org_b.Skip(12).ToArray());
            return sn == org_str;
        }

        private void BtnGenerate_Click(object sender, RoutedEventArgs e)
        {
            var lm = new LicenseModel() { Type = "pc", Model = "", SN = _vm.MachineSN, ExpDate = _vm.ExpirationDate };
            if (GenerateLicense(lm.FileName, _vm.MachineSN, true))
            {
                loadLicenseFiles();
                MessageBox.Show($"{_vm.MainDir + lm.FileName} generated", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            else
                MessageBox.Show("Generation failure", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
        }

        private void BtnSelect_Click(object sender, RoutedEventArgs e)
        {
            OpenFolderDialog ofd = new OpenFolderDialog();
            ofd.Multiselect = false;
            ofd.InitialDirectory = AppDomain.CurrentDomain.BaseDirectory;
            ofd.ShowDialog();
            _vm.MainDir = ofd.FolderName + "\\";
            loadLicenseFiles();
        }

        private void BtnGenLicense_Click(object sender, RoutedEventArgs e)
        {
            if(_vm.TypeName == string.Empty || _vm.ModelName==string.Empty || _vm.SN == string.Empty)
            {
                MessageBox.Show("Invalidate data", "Warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
            else
            {
                var lm = new LicenseModel() { Type = _vm.TypeName.ToLower(), Model = _vm.ModelName.ToLower(), SN = _vm.SN.ToUpper(), ExpDate = _vm.ExpirationDate };
                if (GenerateLicense(lm.FileName, _vm.SN, false))
                {
                    loadLicenseFiles();
                    MessageBox.Show($"{_vm.MainDir + lm.FileName} generated", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
                }
                else
                    MessageBox.Show("Generation failure", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void LvSelectionChange(object sender, SelectionChangedEventArgs e)
        {

        }

        private void MiDelete_Click(object sender, RoutedEventArgs e)
        {
            if (_vm.FileIndex < 0)
                return;
            var res = MessageBox.Show($"{_vm.LicenseFiles[_vm.FileIndex].FileName} will be delete", "Info", MessageBoxButton.OKCancel, MessageBoxImage.Information);
            if(res == MessageBoxResult.OK)
            {
                File.Delete(_vm.MainDir + _vm.LicenseFiles[_vm.FileIndex].FileName);
                _vm.LicenseFiles.RemoveAt(_vm.FileIndex);
                CollectionViewSource.GetDefaultView(fileView.ItemsSource).Refresh();
            }
        }

        private void MiRefresh_Click(object sender, RoutedEventArgs e)
        {
            loadLicenseFiles();
        }
    }
}