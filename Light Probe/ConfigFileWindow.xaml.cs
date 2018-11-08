using System.Windows;

namespace Light_Probe
{
    /// <summary>
    /// Interaction logic for ConfigFileWindow.xaml
    /// </summary>
    public partial class ConfigFileWindow : Window
    {
        public ConfigFileWindow()
        {
            InitializeComponent();
        }

        private void BT_CloseConfigFileWindow_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
