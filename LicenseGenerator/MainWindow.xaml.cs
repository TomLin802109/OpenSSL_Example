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
            _vm.CpuSN = getCPU();
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
            var enc = new Encryptor();
            var cpu_b = Encoding.UTF8.GetBytes(cpu_id);
            _key = enc.Encrypt(new ByteData(cpu_b));
            return cpu_id;
        }

        private void loadLicenseFiles()
        {
            var license = Directory.EnumerateFiles(_vm.MainDir)
                .Select(v => v.Split("\\").Last())
                .Where(v => v.StartsWith("license_"));
            _vm.LicenseFiles = new ObservableCollection<LicenseModel>();
            foreach (var i in license)
            {
                var str = i.Split('_');
                if (str.Length != 3)
                    continue;
                var cipher_b = File.ReadAllBytes(_vm.MainDir + i);
                var enc = new Encryptor();
                if (str[1] != "pc")
                {
                    enc.SetKey(_key);
                }

                var sn_b = enc.Decrypt(new ByteData(cipher_b));
                var sn = Encoding.UTF8.GetString(sn_b.ToArray());
                _vm.LicenseFiles.Add(new LicenseModel(str[1], str[2], sn));
            }
        }

        private bool GenerateLicense(string fileName, string sn, bool defKey=true)
        {
            var in_b = Encoding.UTF8.GetBytes(sn);
            var enc = new Encryption.Encryptor();
            if (!defKey)
            {
                var bs = Encoding.UTF8.GetBytes(_vm.CpuSN);
                var key = enc.Encrypt(new ByteData(bs));
                enc.SetKey(key);
            }
                
            var res = enc.Encrypt(new Encryption.ByteData(in_b));
            File.WriteAllBytes(_vm.MainDir + fileName, res.ToArray());
            var cipher = File.ReadAllBytes(_vm.MainDir + fileName);
            var org_b = enc.Decrypt(new Encryption.ByteData(cipher)).ToArray();
            var org_str = Encoding.UTF8.GetString(org_b);
            return sn == org_str;
            
        }

        private void BtnGenerate_Click(object sender, RoutedEventArgs e)
        {
            var lm = new LicenseModel("pc", "cpu", _vm.CpuSN);
            if (GenerateLicense(lm.FileName, _vm.CpuSN, true))
            {
                _vm.LicenseFiles.Add(lm);
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

        private void BtnGenDevice_Click(object sender, RoutedEventArgs e)
        {
            if(_vm.DeviceType == null || _vm.DeviceModel==string.Empty || _vm.DeviceSN == string.Empty)
            {
                MessageBox.Show("Invalidate data", "Warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
            else
            {
                var str = (_vm.DeviceType as ComboBoxItem).Content as string;
                var lm = new LicenseModel(str.ToLower(), _vm.DeviceModel.ToLower(), _vm.DeviceSN.ToUpper());
                if (GenerateLicense(lm.FileName, _vm.DeviceSN, false))
                {
                    _vm.LicenseFiles.Add(lm);
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

        private void BtnGenFeature_Click(object sender, RoutedEventArgs e)
        {
            if (_vm.FeatureModel == null || _vm.FeatureSN == string.Empty)
            {
                MessageBox.Show("Invalidate data", "Warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
            else
            {
                var str = (_vm.FeatureModel as ComboBoxItem).Content as string;
                var lm = new LicenseModel("app", str.ToLower(), _vm.FeatureSN.ToUpper());
                if (GenerateLicense(lm.FileName, lm.SN, false))
                {
                    _vm.LicenseFiles.Add(lm);
                    MessageBox.Show($"{_vm.MainDir + lm.FileName} generated", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
                }
                else
                    MessageBox.Show("Generation failure", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}