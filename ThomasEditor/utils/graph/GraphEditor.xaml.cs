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

namespace ThomasEditor
{
    /// <summary>
    /// Interaction logic for GraphEditor.xaml
    /// </summary>
    public partial class GraphEditor : UserControl
    {

        bool isExpanded = false;
        public static readonly DependencyProperty ValueProperty =
        DependencyProperty.Register(
        "Value", typeof(Curve),
        typeof(GraphEditor),
        new PropertyMetadata(new Curve())
        );

        public Curve Value
        {
            get { return (Curve)GetValue(ValueProperty); }
            set { SetValue(ValueProperty, value); }
        }

        public static readonly DependencyProperty PropNameProperty =
       DependencyProperty.Register(
       "PropName", typeof(String),
       typeof(GraphEditor)
       );

        public String PropName
        {
            get { return (String)GetValue(PropNameProperty); }
            set { SetValue(PropNameProperty, value); }
        }

        Thomas_Graph.ExpandedGraphControl expandedGraph;
        public GraphEditor()
        {
            InitializeComponent();
            Loaded += GraphEditor_Loaded;
            Unloaded += GraphEditor_Unloaded;
           
        }

        public void UpdatePoints()
        {
            graph.points.CollectionChanged -= Points_CollectionChanged;
            graph.AddPoints(Value.points);
            graph.points.CollectionChanged += Points_CollectionChanged;
        }

        private void GraphEditor_Unloaded(object sender, RoutedEventArgs e)
        {
            if(expandedGraph != null)
            {
                expandedGraph.Close();
                expandedGraph = null;
            }
            graph.OnPointsChanged -= GraphControl_OnPointsChanged;
            graph.points.CollectionChanged -= Points_CollectionChanged;
        }

        private void GraphEditor_Loaded(object sender, RoutedEventArgs e)
        {
            if (Value == null)
                Value = new Curve();
            graph.OnPointsChanged += GraphControl_OnPointsChanged;
            graph.points.CollectionChanged += Points_CollectionChanged;
            UpdatePoints();

            if (Value.expandedInPropertyGrid)
                Expand(null, new RoutedEventArgs());
        }

        private void UpdateValue()
        {
            List<Point> points = new List<Point>();
            foreach (Thomas_Graph.CustomPoint cp in graph.points)
                points.Add(new Point(cp.X / graph.gridSpacing, cp.Y / graph.gridSpacing));
            Value.points = points;
        }

        private void GraphControl_OnPointsChanged()
        {
            UpdateValue();
        }

        private void Points_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            UpdateValue();
        }
        private void Expand(object sender, RoutedEventArgs e)
        {
            if(expandedGraph == null)
            {
                isExpanded = true;
                Value.expandedInPropertyGrid = true;
                expandedGraph = new Thomas_Graph.ExpandedGraphControl(graph);
                expandedGraph.Title = PropName;
                expandedGraph.graph.OnPointsChanged += GraphControl_OnPointsChanged;
                expandedGraph.onPopIn += ExpandedGraph_onPopIn;
                graph.OnLostFocus();
                Keyboard.ClearFocus();

                expandedGraph.Closed += W_Closed;
                expandedGraph.Show();
                popup.IsOpen = false;
            }

        }

        private void ExpandedGraph_onPopIn()
        {
            isExpanded = false;
            Value.expandedInPropertyGrid = false;
            expandedGraph.Close();
            popup.IsOpen = true;
            popup.Height = popup.Width / 16 * 9;
            graph.UpdateRawPoints();
            graph.OnGotFocus();
            
        }

        private void W_Closed(object sender, EventArgs e)
        {
            expandedGraph = null;
        }

        private void Show_Graph(object sender, RoutedEventArgs e)
        {
            if(isExpanded)
            {
                Expand(null, new RoutedEventArgs());
            }
            else
            {
                popup.IsOpen = true;
                popup.Height = popup.Width / 16 * 9;
            }
            
        }
    }


}
