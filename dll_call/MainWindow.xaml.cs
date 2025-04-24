using System.Runtime.InteropServices;
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

namespace dll_call
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        [DllImport("sample_nativecpp_dll_window.dll", CallingConvention = CallingConvention.StdCall)]
        private static extern void TestWindowOpen();

        [DllImport("sample_nativecpp_dll_window.dll", CallingConvention = CallingConvention.StdCall)]
        private static extern void TestWindowClose();


        private bool _IsOpened;

        private void ButtonBase_OnClick(object sender, RoutedEventArgs e)
        {
            if (!_IsOpened)
            {
                TestWindowOpen();
                _IsOpened = true;
            }
            else
            {
                TestWindowClose();
                _IsOpened = false;
            }
        }
    }
}