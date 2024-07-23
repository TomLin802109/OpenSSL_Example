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

        public LicenseModel(string type, string model, string sn)
        {
            Type = type; Model = model; SN = sn;
        }
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

        private string cpuSN = "";
        public string CpuSN
        {
            get => cpuSN;
            set
            {
                cpuSN = value;
                OnPropertyChanged();
            }
        }

        private string deviceType;
        public string DeviceType
        {
            get => deviceType;
            set
            {
                deviceType = value;
                OnPropertyChanged();
            }
        }
        private string deviceModel = "";
        public string DeviceModel
        {
            get => deviceModel;
            set
            {
                deviceModel = value;
                OnPropertyChanged();
            }
        }
        private string deviceSN = "";
        public string DeviceSN
        {
            get => deviceSN;
            set
            {
                deviceSN = value;
                OnPropertyChanged();
            }
        }

        private string featureModel = "";
        public string FeatureModel
        {
            get => featureModel;
            set
            {
                featureModel = value;
                OnPropertyChanged();
            }
        }

        private string featureSN = "";
        public string FeatureSN
        {
            get => featureSN;
            set
            {
                featureSN = value;
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
