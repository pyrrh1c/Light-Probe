using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.IO.Ports;

namespace Light_Probe
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        string[] ports = SerialPort.GetPortNames();
        SerialPort port = new SerialPort();
        //string controlState = "AUTO";

        public MainWindow()
        {
            InitializeComponent();

            foreach (string port in ports)
            {
                try
                {
                    SerialPort testPort = new SerialPort(port, 9600);
                    testPort.Open();
                    CB_Ports.Items.Add(port);
                    testPort.Close();
                    testPort = null;
                }
                catch
                {
                }
            }
        }

        private void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string message = "";
            this.Dispatcher.Invoke(() =>
            {
                try
                {
                    message = port.ReadLine();
                    TB_LastMessage.Text = message;
                    PB_ULSensor.Value = int.Parse(message.Substring(3, 3));
                    PB_URSensor.Value = int.Parse(message.Substring(10, 3));
                    PB_LLSensor.Value = int.Parse(message.Substring(17, 3));
                    PB_LRSensor.Value = int.Parse(message.Substring(24, 3));
                    PB_hServoAngle.Value = int.Parse(message.Substring(33, 3));
                    PB_vServoAngle.Value = int.Parse(message.Substring(42, 3));
                    //Set the overall average
                    PB_SensorAverage.Value = (PB_ULSensor.Value +
                        PB_URSensor.Value +
                        PB_LLSensor.Value +
                        PB_LRSensor.Value) / 4;

                    //Set the horizontal balance
                    PB_HorizontalBalance.Value = 50 + (PB_URSensor.Value - PB_ULSensor.Value);
                    //Set the Vertical balance
                    PB_VerticalABalance.Value = 50 + (PB_LRSensor.Value - PB_LLSensor.Value);
                }
                catch
                {
                    TB_LastMessage.Text = "Message Error";
                }
            });
        }

        private void UpdateValue(ProgressBar targetBar, double targetValue)
        {
            targetBar.Value = targetValue;

            if (targetValue < 25)
            {
                targetBar.Foreground = Brushes.Red;
            }
            else if ((targetValue >= 25) && (targetValue < 75))
            {
                targetBar.Foreground = Brushes.Yellow;
            }
            else
            {
                targetBar.Foreground = Brushes.Green;
            }

        }

        //Methods from the Startup Tab

        private void CB_Ports_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {
            port.Close();
            port = null;
            port = new SerialPort(CB_Ports.SelectedValue.ToString(), 9600);
            try
            {
                port.Open();
                port.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
            }
            catch
            {
                TB_LastMessage.Text = "Port Error";
            }
        }

        private void BT_AutoCalibration_Click(object sender, RoutedEventArgs e)
        {
            AutoCalibration auto = new AutoCalibration();
            auto.ShowDialog();
        }

        private void BT_ConfigLoad_Click(object sender, RoutedEventArgs e)
        {
            ConfigFileWindow configFileWindow = new ConfigFileWindow();
            configFileWindow.ShowDialog();
        }

        private void BT_ConfigSave_Click(object sender, RoutedEventArgs e)
        {
            ConfigFileWindow configFileWindow = new ConfigFileWindow();
            configFileWindow.ShowDialog();
        }

        private void BT_RescanComPorts_Click(object sender, RoutedEventArgs e)
        {

        }

        //Methods from the Sensors Tab

        private void BT_ULSensorDecrease_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("ULOFFSET:DECREASE;");
        }

        private void BT_ULSensorIncrease_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("ULOFFSET:INCREASE;");
        }

        private void BT_URSensorDecrease_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("UROFFSET:DECREASE;");
        }

        private void BT_URSensorIncrease_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("UROFFSET:INCREASE;");
        }

        private void BT_LLSensorDecrease_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("LLOFFSET:DECREASE;");
        }

        private void BT_LLSensorIncrease_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("LLOFFSET:INCREASE;");
        }

        private void BT_LRSensorDecrease_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("LROFFSET:DECREASE;");
        }

        private void BT_LRSensorIncrease_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("ROFFSET:INCREASE;");
        }

        //Methods from the Servos Tab

        //Event Handler for the Servo Control Radio Group
        private void RB_ServoControlAutomatic_Checked(object sender, RoutedEventArgs e)
        {
            /*
            port.WriteLine("MODE=AUTO;");
            controlState = "AUTO";
            BT_HServoIncrease.IsEnabled = false;
            BT_HServoDecrease.IsEnabled = false;
            BT_VServoIncrease.IsEnabled = false;
            BT_VServoDecrease.IsEnabled = false;
            */
        }

        //Event Handler for the Servo Control Radio Group
        private void RB_ServoControlManual_Checked(object sender, RoutedEventArgs e)
        {
            /*
            port.WriteLine("MODE=MANUAL;");
            controlState = "MANUAL";
            BT_HServoIncrease.IsEnabled = true;
            BT_HServoDecrease.IsEnabled = true;
            BT_VServoIncrease.IsEnabled = true;
            BT_VServoDecrease.IsEnabled = true;
            */
        }

        private void BT_HServoDecrease_Click(object sender, RoutedEventArgs e)
        {

        }

        private void BT_HServoIncrease_Click(object sender, RoutedEventArgs e)
        {

        }

        private void BT_VServoDecrease_Click(object sender, RoutedEventArgs e)
        {

        }

        private void BT_VServoIncrease_Click(object sender, RoutedEventArgs e)
        {

        }

        private void BT_HServoDecreaseCalibration_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("HOFFSET:DECREASE;");
        }

        private void BT_HServoIncreaseCalibration_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("HOFFSET:INCREASE;");
        }

        private void BT_VServoDecreaseCalibration_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("VOFFSET:DECREASE;");
        }

        private void BT_VServoIncreaseCalibration_Click(object sender, RoutedEventArgs e)
        {
            port.WriteLine("VOFFSET:INCREASE;");
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
public static class ExtensionMethods
{
    public static decimal Map(this decimal value, decimal fromSource, decimal toSource, decimal fromTarget, decimal toTarget)
    {
        return (value - fromSource) / (toSource - fromSource) * (toTarget - fromTarget) + fromTarget;
    }
}