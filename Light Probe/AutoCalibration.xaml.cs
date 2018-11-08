using System.Windows;

namespace Light_Probe
{
    /// <summary>
    /// Interaction logic for AutoCalibration.xaml
    /// </summary>
    public partial class AutoCalibration : Window
    {
        public AutoCalibration()
        {
            InitializeComponent();
        }

        private void BT_CloseAutoCalibration_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
