using System;
using System.Collections.ObjectModel;
using System.Windows;


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
            SetPoints(smallGraph.points);
        }

        public void SetPoints(ObservableCollection<CustomPoint> points)
        {
            graph.points = points;
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
