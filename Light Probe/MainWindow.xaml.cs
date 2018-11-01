using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
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
    }
}
public static class ExtensionMethods
{
    public static decimal Map(this decimal value, decimal fromSource, decimal toSource, decimal fromTarget, decimal toTarget)
    {
        return (value - fromSource) / (toSource - fromSource) * (toTarget - fromTarget) + fromTarget;
    }
}