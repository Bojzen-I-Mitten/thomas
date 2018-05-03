﻿using System;
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

namespace Thomas_Graph
{
    /// <summary>
    /// Interaction logic for GraphEditor.xaml
    /// </summary>
    public partial class GraphEditor : UserControl
    {

        bool isExpanded = false;
        public static readonly DependencyProperty ValueProperty =
        DependencyProperty.Register(
        "Value", typeof(Bezier),
        typeof(GraphEditor),
        new PropertyMetadata(new Bezier())
        );

        public Bezier Value
        {
            get { return (Bezier)GetValue(ValueProperty); }
            set { SetValue(ValueProperty, value); }
        }

        ExpandedGraphControl expandedGraph;
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
            graph.OnPointsChanged -= GraphControl_OnPointsChanged;
            graph.points.CollectionChanged -= Points_CollectionChanged;
        }

        private void GraphEditor_Loaded(object sender, RoutedEventArgs e)
        {
            if (Value == null)
                Value = new Bezier();
            graph.OnPointsChanged += GraphControl_OnPointsChanged;
            graph.points.CollectionChanged += Points_CollectionChanged;
            UpdatePoints();
        }

        private void UpdateValue()
        {
            List<Point> points = new List<Point>();
            foreach (CustomPoint cp in graph.points)
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
                expandedGraph = new ExpandedGraphControl(graph);
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