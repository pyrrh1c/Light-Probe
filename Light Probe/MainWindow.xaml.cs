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
                    TB_ULSensor.Text = PB_ULSensor.Value.ToString();
                    PB_URSensor.Value = int.Parse(message.Substring(10, 3));
                    TB_URSensor.Text = PB_URSensor.Value.ToString();
                    PB_LLSensor.Value = int.Parse(message.Substring(17, 3));
                    TB_LLSensor.Text = PB_LLSensor.Value.ToString();
                    PB_LRSensor.Value = int.Parse(message.Substring(24, 3));
                    TB_LRSensor.Text = PB_LRSensor.Value.ToString();
                    PB_hServoAngle.Value = int.Parse(message.Substring(33, 3));
                    TB_hServoAngle.Text = message.Substring(33, 3);
                    PB_vServoAngle.Value = int.Parse(message.Substring(42, 3));
                    TB_vServoAngle.Text = message.Substring(42, 3);
                    TB_ulOffset.Text = message.Substring(50, 3);
                    TB_urOffset.Text = message.Substring(58, 3);
                    TB_llOffset.Text = message.Substring(66, 3);
                    TB_lrOffset.Text = message.Substring(74, 3);
                    TB_hOffset.Text = message.Substring(83, 3);
                    TB_vOffset.Text = message.Substring(92, 3);
                    //Set the overall average
                    PB_SensorAverage.Value = (PB_ULSensor.Value +
                        PB_URSensor.Value +
                        PB_LLSensor.Value +
                        PB_LRSensor.Value) / 4;
                    TB_SensorAverage.Text = PB_SensorAverage.Value.ToString();

                    //Set the horizontal balance
                    PB_HorizontalBalance.Value = 50 + (PB_URSensor.Value - PB_ULSensor.Value);
                    TB_HorizontalBalance.Text = PB_HorizontalBalance.Value.ToString();
                    //Set the Vertical balance
                    PB_VerticalABalance.Value = 50 + (PB_LRSensor.Value - PB_LLSensor.Value);
                    TB_VerticalBalance.Text = PB_VerticalABalance.Value.ToString();
                }
                catch
                {
                    TB_LastMessage.Text = message;
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
            //Write a handler for these to address a situatio where the port is closed!
            TransmitCommand("DECREASE-ULOFFSET;");
        }

        private void BT_ULSensorIncrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("INCREASE-ULOFFSET;");
        }

        private void BT_URSensorDecrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("DECREASE-UROFFSET;");
        }

        private void BT_URSensorIncrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("INCREASE-UROFFSET;");
        }

        private void BT_LLSensorDecrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("DECREASE-LLOFFSET;");
        }

        private void BT_LLSensorIncrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("INCREASE-LLOFFSET;");
        }

        private void BT_LRSensorDecrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("DECREASE-LROFFSET;");
        }

        private void BT_LRSensorIncrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("INCREASE-LROFFSET;");
        }

        //Methods from the Servos Tab

        //Event Handler for the Servo Control Radio Group
        private void RB_ServoControlAutomatic_Checked(object sender, RoutedEventArgs e)
        {
            /*
            TransmitCommand("MODE=AUTO;");
            controlState = "AUTO";
            BT_HServoIncrease.IsEnabled = false;
            BT_HServoDecrease.IsEnabled = false;
            BT_VServoIncrease.IsEnabled = false;
            BT_VServoDecrease.IsEnabled = false;
            */
        }

        //Event Handler for the Servo Control Radio Group
        private void RB_ServoControlManualUI_Checked(object sender, RoutedEventArgs e)
        {
            /*
            TransmitCommand("MODE=MANUAL;");
            controlState = "MANUAL";
            BT_HServoIncrease.IsEnabled = true;
            BT_HServoDecrease.IsEnabled = true;
            BT_VServoIncrease.IsEnabled = true;
            BT_VServoDecrease.IsEnabled = true;
            */
        }

        //Event Handler for the Servo Control Radio Group
        private void RB_ServoControlManualWiiChuck_Checked(object sender, RoutedEventArgs e)
        {
            /*
            TransmitCommand("MODE=MANUAL;");
            controlState = "MANUAL";
            BT_HServoIncrease.IsEnabled = true;
            BT_HServoDecrease.IsEnabled = true;
            BT_VServoIncrease.IsEnabled = true;
            BT_VServoDecrease.IsEnabled = true;
            */
        }
        private void BT_HServoDecrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("DECREASE-HSERVO;");
        }

        private void BT_HServoIncrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("INCREASE-HSERVO;");
        }

        private void BT_VServoDecrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("DECREASE-VSERVO;");
        }

        private void BT_VServoIncrease_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("INCREASE-VSERVO;");
        }

        private void BT_HServoDecreaseCalibration_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("DECREASE-HOFFSET;");
        }

        private void BT_HServoIncreaseCalibration_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("INCREASE-HOFFSET;");
        }

        private void BT_VServoDecreaseCalibration_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("DECREASE-VOFFSET;");
        }

        private void BT_VServoIncreaseCalibration_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("INCREASE-VOFFSET;");
        }

        private void BT_EnableValTrans_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("ENABLE-TRANSMITVALUES;");
        }

        private void BT_DisableValTrans_Click(object sender, RoutedEventArgs e)
        {
            TransmitCommand("DISABLE-TRANSMITVALUES;");
        }

        private void TransmitCommand(string command)
        {
            try
            {
                port.Write(command);
            }
            catch
            {
                
            }
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