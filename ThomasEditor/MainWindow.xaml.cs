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
using System.Windows.Interop;


namespace ThomasEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            
            InitializeComponent();

            var window = Window.GetWindow(this);
            IntPtr hWnd = new WindowInteropHelper(window).EnsureHandle();

            var success = ThomasManaged.ThomasWrapper.Init(hWnd);
            Console.WriteLine("window: " + success);
            ThomasManaged.ThomasWrapper.Start();

            HwndSource source = HwndSource.FromHwnd(hWnd);
            source.AddHook(new HwndSourceHook(WndProc));

            CompositionTarget.Rendering += testRender;
        }

        private static IntPtr WndProc(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            ThomasManaged.ThomasWrapper.eventHandler(hWnd, msg, wParam, lParam);

            return IntPtr.Zero;
        }

        private void testRender(object sender, EventArgs e)
        {
            ThomasManaged.ThomasWrapper.Update();
        }

    }
}
