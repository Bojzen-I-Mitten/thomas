using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
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
    /// Interaction logic for GraphControl.xaml
    /// </summary>
    public partial class GraphControl : UserControl
    {

        public int gridSpacing = 50;
        public delegate void PointsChanged();
        public event PointsChanged OnPointsChanged;

        public ObservableCollection<CustomPoint> points = new ObservableCollection<CustomPoint>();
        PointCollection rawPoints = new PointCollection();
        CustomPoint selectedLinePoint = null;
        CustomPoint selectedPoint = null;

        ScaleTransform scaleTransform = new ScaleTransform(1, 1);
        TranslateTransform translateTransform;

        double prevMouseX = 0;
        double prevMouseY = 0;

        public GraphControl()
        {
            InitializeComponent();
            

            this.Background = Brushes.Transparent;

            if(points.Count > 0)
                start.StartPoint = points[0].p;

            this.MouseMove += MainWindow_MouseMove;
            this.MouseDoubleClick += MainWindow_MouseDoubleClick;
            curve.Points = rawPoints;
            this.MouseDown += MainWindow_MouseDown;
            this.MouseUp += GraphControl_MouseUp;

            this.MouseWheel += GraphControl_MouseWheel;
            translateTransform = new TranslateTransform(0, 0);
            TransformGroup group = new TransformGroup();
            group.Children.Add(new ScaleTransform(1, -1)); //flip
            group.Children.Add(translateTransform);
            group.Children.Add(scaleTransform);
            this.RenderTransform = group;

            this.Loaded += GraphControl_Loaded;

        }
        public void OnLostFocus() 
        {
            selectedLinePoint = null;
            selectedPoint = null;
            InvalidateVisual();
        }

        public void ClearList()
        {
            points.Clear();
            rawPoints.Clear();
        }

        public void OnGotFocus()
        {
            InvalidateVisual();
            if (points.Count > 0)
                start.StartPoint = points[0].p;
        }


        private void GraphControl_Loaded(object sender, RoutedEventArgs e)
        {
            translateTransform.Y += (RenderSize.Height != 0 ? RenderSize.Height : 350) - 100;
            translateTransform.X += 50;
            this.Loaded -= GraphControl_Loaded;
        }

        private void GraphControl_MouseUp(object sender, MouseButtonEventArgs e)
        {
            selectedPoint = null;
        }

        private void GraphControl_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            Point pos = e.GetPosition(this);
            scaleTransform.ScaleX += e.Delta * 0.0001;
            scaleTransform.ScaleY += e.Delta * 0.0001;

            Point pos2 = e.GetPosition(this);

            translateTransform.X += pos2.X - pos.X;
            translateTransform.Y -= pos2.Y - pos.Y;
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            if(points.Count > 0)
                start.StartPoint = points[0].p;
            
            int distance = 10000;

           
            drawingContext.PushTransform(new ScaleTransform(1, -1));
        
            drawingContext.DrawRectangle(Brushes.Gray, null, new Rect(-distance, -distance, distance*2, distance*2));
            //grid
            
            for (int x = -distance; x < distance; x+= gridSpacing) // parallel to y-axis grid
            {
                drawingContext.DrawLine(new Pen(Brushes.DimGray, 1), new Point(x, -distance), new Point(x, distance));

                if(x != 0)
                {

                    FormattedText ft = new FormattedText((x / gridSpacing).ToString(),
                        CultureInfo.GetCultureInfo("en-us"),
                        FlowDirection.LeftToRight,
                        new Typeface("Verdana"),
                        10, System.Windows.Media.Brushes.Black);
                    ft.TextAlignment = TextAlignment.Center;
                    drawingContext.DrawText(ft, new System.Windows.Point(x, 5));
                }
               
            }
           
            for (int y = -distance; y < distance; y+= gridSpacing) //parllel to x-axis grid
            {
                drawingContext.DrawLine(new Pen(Brushes.DimGray, 1), new Point(-distance, y), new Point(distance, y));

                if (y != 0)
                {
                    FormattedText ft = new FormattedText((-y / gridSpacing).ToString(),
                         CultureInfo.GetCultureInfo("en-us"),
                         FlowDirection.LeftToRight,
                         new Typeface("Verdana"),
                         10, System.Windows.Media.Brushes.Black);
                    ft.TextAlignment = TextAlignment.Right;
                    drawingContext.DrawText(ft, new System.Windows.Point(-10, y-7.5));
                }
                else
                {
                    FormattedText ft = new FormattedText("0",
                         CultureInfo.GetCultureInfo("en-us"),
                         FlowDirection.LeftToRight,
                         new Typeface("Verdana"),
                         10, System.Windows.Media.Brushes.Black);
                    ft.TextAlignment = TextAlignment.Right;
                    drawingContext.DrawText(ft, new System.Windows.Point(-10, 5));
                }

            }
            
            
            //axis
            drawingContext.DrawLine(new Pen(Brushes.Black, 1), new Point(-distance, 0), new Point(distance, 0));
            drawingContext.DrawLine(new Pen(Brushes.Black, 1), new Point(0, -distance), new Point(0, distance));

            //unselected points

            drawingContext.PushTransform(new ScaleTransform(1, -1));
            foreach (CustomPoint point in points)
            {
                if (point.isLinePoint && point != selectedLinePoint)
                {

                    drawingContext.DrawRectangle(Brushes.Red, null, point.GetOffsetRect());
                }
            }


            //selected point
            if (selectedLinePoint != null)
            {
                CustomPoint point = selectedLinePoint;
                drawingContext.DrawRectangle(Brushes.Green, null, point.GetOffsetRect());
                foreach (CustomPoint cpPoint in GetControlPoints(selectedLinePoint))
                {
                    point = cpPoint;
                    drawingContext.DrawRectangle(Brushes.Green, null, point.GetOffsetRect());
                    drawingContext.DrawLine(new Pen(Brushes.Teal, 2), selectedLinePoint.p, cpPoint.p);
                }



            }
            base.OnRender(drawingContext);


        }

        private void MainWindow_MouseDown(object sender, MouseButtonEventArgs e)
        {

            CustomPoint point = GetPointAt(e.GetPosition(this));

            foreach (CustomPoint p in points)
            {
                if (!p.isLinePoint)
                    p.visible = false;
            }
            selectedPoint = null;
            selectedLinePoint = null;

            if (point != null)
            {
                int i = points.IndexOf(point);
                if (point.isLinePoint)
                {


                    if (points.Count > i + 1)
                        points[i + 1].visible = true;

                    if (i > 1)
                        points[i - 1].visible = true;

                    selectedLinePoint = point;
                }
                else
                {
                    point.visible = true;
                    CustomPoint mirror = GetMirrorControlPoint(point);
                    if (mirror != null)
                        mirror.visible = true;

                    selectedLinePoint = GetClosestLinePoint(point);
                }
                selectedPoint = point;


            }
            InvalidateVisual();


        }

        List<CustomPoint> GetControlPoints(CustomPoint linePoint)
        {
            int i = points.IndexOf(linePoint);
            List<CustomPoint> cpPoints = new List<CustomPoint>();

            if (i > 0)
                cpPoints.Add(points[i - 1]);
            if (i < points.Count - 1)
                cpPoints.Add(points[i + 1]);

            return cpPoints;
        }

        CustomPoint GetClosestLinePoint(CustomPoint controlPoint)
        {
            int i = points.IndexOf(controlPoint);
            if (points[i - 1].isLinePoint)
                return points[i - 1];
            else
                return points[i + 1];
        }


        CustomPoint GetMirrorControlPoint(CustomPoint controlPoint)
        {
            int i = points.IndexOf(controlPoint);
            if (points[i + 1].isLinePoint && points.Count > i + 2)
                return points[i + 2];
            else if (i > 1)
                return points[i - 2];
            else
                return null;
        }

        CustomPoint GetLinePointToTheLeft(CustomPoint controlPoint)
        {
            int i = points.IndexOf(controlPoint);
            i--;
            while (i >= 0)
            {
                if (points[i].isLinePoint)
                    return points[i];
                else
                    i--;
            }
            return null;
        }

        CustomPoint GetLinePointToTheRight(CustomPoint controlPoint)
        {
            int i = points.IndexOf(controlPoint);
            i++;
            while (i < points.Count)
            {
                if (points[i].isLinePoint)
                    return points[i];
                else
                    i++;
            }
            return null;
        }


        CustomPoint GetPointAt(Point mousePoint)
        {
            foreach (CustomPoint point in points)
            {
                if (point.visible)
                {
                    if (point.Contains(mousePoint))
                        return point;
                }
            }
            return null;
        }

        public void UpdateRawPoints()
        {
            rawPoints.Clear();
            for (int i = 1; i < points.Count; ++i)
            {
                rawPoints.Add(points[i].p);
            }
        }

        public void AddPoints(List<Point> newPoints)
        {
            ClearList();
            for(int i=0; i < newPoints.Count; ++i)
            {
                Point point = newPoints[i];
                CustomPoint cp = new CustomPoint(point.X*gridSpacing, point.Y*gridSpacing);
                if (points.Count == 0)
                {
                    points.Add(cp);
                }
                else
                {
                    points.Add(cp);
                    rawPoints.Add(cp.p);
                }
                if(i % 3 == 0)
                {
                    cp.visible = true;
                    cp.isLinePoint = true;
                }
            }
        }

        private void AddPoint(Point pos)
        {
            CustomPoint linePoint = new CustomPoint(pos.X, pos.Y);

            if (points.Count == 0)
            {
                points.Add(linePoint);
            }
            else
            {
                CustomPoint prevPoint = points[points.Count - 1];
                Vector dir = new Vector(pos.X - prevPoint.X, pos.Y - prevPoint.Y);

                CustomPoint cp1 = new CustomPoint(prevPoint.X + dir.X * 0.45, prevPoint.Y + dir.Y * 0.45);
                points.Add(cp1);
                CustomPoint cp2 = new CustomPoint(pos.X - dir.X * 0.45, pos.Y - dir.Y * 0.45);
                points.Add(cp2);
                points.Add(linePoint);

                rawPoints.Add(cp1.p);
                rawPoints.Add(cp2.p);
                rawPoints.Add(linePoint.p);
            }
            linePoint.visible = true;
            linePoint.isLinePoint = true;
        }


        bool isValidMovement(CustomPoint target, Point mousePoint)
        {
            int i = points.IndexOf(target);
            float mouseX = (float)mousePoint.X;
            bool movingLeft = target.p.X > mouseX;
            if (!target.isLinePoint) //control point
            {

                if (movingLeft)
                {
                    CustomPoint leftPoint = GetLinePointToTheLeft(target);
                    if (leftPoint.p.X < mouseX)
                        return true;
                }
                else
                {
                    CustomPoint rightPoint = GetLinePointToTheRight(target);
                    if (rightPoint.p.X > mouseX)
                        return true;
                }
                return false;
            }
            else
            {
                if (movingLeft)
                {
                    if (i == 0)
                        return true;

                    CustomPoint leftCP = points[i - 1];
                    if (leftCP.p.X < mouseX)
                        return true;
                }
                else
                {


                    if (i == points.Count - 1)
                        return true;

                    CustomPoint rightCP = points[i + 1];
                    if (rightCP.p.X > mouseX)
                        return true;
                }
                return false;
            }
        }



        private void MainWindow_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            CustomPoint point = GetPointAt(e.GetPosition(this));
            if (point == null)
                AddPoint(e.GetPosition(this));
        }

        private void MainWindow_MouseMove(object sender, MouseEventArgs e)
        {

            Cursor = Cursors.Arrow;
            CustomPoint over = GetPointAt(e.GetPosition(this));
            if (over != null || selectedPoint != null)
            {
                Cursor = Cursors.SizeAll;
            }
            if (selectedPoint != null)
            {
                if (isValidMovement(selectedPoint, e.GetPosition(this)))
                {
                    selectedPoint.MoveTo(e.GetPosition(this));
                    InvalidateVisual();
                    OnPointsChanged?.Invoke();
                }
                int i = points.IndexOf(selectedPoint);
                if(i != 0)
                {
                    rawPoints[i - 1] = selectedPoint.p;
                }
            }
            else if (e.LeftButton == MouseButtonState.Pressed)
            {
                double deltaX = e.GetPosition(this).X - prevMouseX;
                double deltaY = e.GetPosition(this).Y - prevMouseY;
                Cursor = Cursors.Hand;
                translateTransform.X += deltaX;
                translateTransform.Y -= deltaY;
            }
            prevMouseX = e.GetPosition(this).X;
            prevMouseY = e.GetPosition(this).Y;
        }
    }


    public class CustomPoint
    {

        private Rect rectangle;
        public bool isLinePoint = false;
        public bool visible = false;

        public double X { get { return rectangle.X; } }
        public double Y { get { return rectangle.Y; } }

        public Rect rect { get { return rectangle; } }
        public Point p { get { return rectangle.Location; } }

        public CustomPoint(double x, double y)
        {
            rectangle = new Rect(x, y, 10, 10);
        }

        public bool Contains(Point point)
        {
            return GetOffsetRect().Contains(point);
        }

        public Rect GetOffsetRect()
        {
            Rect offsetRect = new Rect(rect.Location, rect.Size);
            offsetRect.Offset(-5, -5);
            return offsetRect;
        }
        public void MoveTo(Point newPoint)
        {
            rectangle.X = newPoint.X;
            rectangle.Y = newPoint.Y;
        }
    }
}