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
using System.Windows.Shapes;

namespace Thomas_Graph
{
    /// <summary>
    /// Interaction logic for ExpandedGraphControl.xaml
    /// </summary>
    public partial class ExpandedGraphControl : Window
    {
        static double left = -1;
        static double top;
        static double width;
        static double height;
        public delegate void OnPopin();
        public event OnPopin onPopIn;

        public ExpandedGraphControl(GraphControl smallGraph)
        {
            InitializeComponent();
            Closed += ExpandedGraphControl_Closed;
            if(left == -1)
            {
                left = this.Left;
                top = this.Top;
                width = this.Width;
                height = this.Height;
            }
            else
            {
                this.Left = left;
                this.Top = top;
                this.Width = width;
                this.Height = height;
            }
            graph.points = smallGraph.points;
            graph.UpdateRawPoints();
        }
        private void ExpandedGraphControl_Closed(object sender, EventArgs e)
        {
            left = this.Left;
            top = this.Top;
            width = this.Width;
            height = this.Height;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            onPopIn?.Invoke();

        }
    }
}
