using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace LicenseGenerator
{
    internal class LicenseModel
    {
        public string Type { get; set; }
        public string Model { get; set; }
        public string SN { get; set; }
        public string FileName { get => $"license_{Type}_{Model}"; }
        public DateTime ExpDate { get; set; }
        public DateTime LastDate { get; set; }
    }
    internal class ViewModel : INotifyPropertyChanged
    {
        private string mainDir = "";
        public string MainDir
        {
            get => mainDir;
            set
            {
                mainDir = value;
                OnPropertyChanged();
            }
        }

        private string machineSN = "";
        public string MachineSN
        {
            get => machineSN;
            set
            {
                machineSN = value;
                OnPropertyChanged();
            }
        }

        private List<string> machineSNs = new List<string>();
        public List<string> MachineSNs
        {
            get => machineSNs;
            set { machineSNs = value; OnPropertyChanged(); }
        }

        private List<string> typeNames = new List<string>()
        {
            "Robot", "Scanner", "Module"
        };
        public List<string> TypeNames
        {
            get => typeNames;
            set { typeNames = value; OnPropertyChanged(); }
        }


        private string typeName;
        public string TypeName
        {
            get => typeName;
            set
            {
                typeName = value;
                OnPropertyChanged();
            }
        }
        private string modelName = "";
        public string ModelName
        {
            get => modelName;
            set
            {
                modelName = value;
                OnPropertyChanged();
            }
        }

        private string sn = "";
        public string SN
        {
            get => sn;
            set
            {
                sn = value;
                OnPropertyChanged();
            }
        }

        private DateTime expirationDate = DateTime.Now.AddYears(100);
        public DateTime ExpirationDate
        {
            get => expirationDate;
            set
            {
                expirationDate = value;
                OnPropertyChanged();
            }
        }

        private int fileIndex;
        public int FileIndex
        {
            get => fileIndex;
            set
            {
                fileIndex = value;
                OnPropertyChanged();
            }
        }

        private ObservableCollection<LicenseModel> licenseFiles = new ObservableCollection<LicenseModel>();
        public ObservableCollection<LicenseModel> LicenseFiles
        {
            get => licenseFiles;
            set
            {
                licenseFiles = value;
                OnPropertyChanged();
            }
        }

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        /// <summary>
        /// Raises this object's PropertyChanged event.
        /// </summary>
        /// <param name="propertyName">The property that has a new value.</param>
        protected void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChangedEventHandler handler = this.PropertyChanged;
            if (handler != null)
            {
                var e = new PropertyChangedEventArgs(propertyName);
                handler(this, e);
            }
        }
        #endregion
    }
}
