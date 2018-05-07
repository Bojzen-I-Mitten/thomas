﻿using System;
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

        ScaleTransform scaleTransform = new ScaleTransform(10, 10);
        TranslateTransform translateTransform;

        double prevMouseX = 0;
        double prevMouseY = 0;

        public GraphControl()
        {
            InitializeComponent();
            

            this.Background = Brushes.Transparent;

            this.MouseMove += MainWindow_MouseMove;
            this.MouseDoubleClick += MainWindow_MouseDoubleClick;
            curve.Points = rawPoints;
            this.MouseDown += MainWindow_MouseDown;
            this.MouseUp += GraphControl_MouseUp;

            this.MouseWheel += GraphControl_MouseWheel;
            this.KeyDown += GraphControl_KeyDown;
            
            translateTransform = new TranslateTransform(0, 0);
            TransformGroup group = new TransformGroup();
            group.Children.Add(new ScaleTransform(1, -1)); //flip
            group.Children.Add(translateTransform);
            group.Children.Add(scaleTransform);
            this.RenderTransform = group;

            this.Loaded += GraphControl_Loaded;
            Focusable = true;
            InvalidateVisual();
            
        }

        private void GraphControl_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.Delete:
                    gridSpacing -= 1;
                    break;
            }
            
            
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

            path.StrokeThickness = 1 / ((scaleTransform.ScaleX + scaleTransform.ScaleY) / 2.0);

            InvalidateVisual();
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            if(points.Count > 0)
                start.StartPoint = points[0].p;
            
            
            int zoomX = (int)(Math.Round(scaleTransform.ScaleX * 0.5) * 2);
            int zoomY = (int)(Math.Round(scaleTransform.ScaleX * 0.5) * 2);
            if (zoomX == 0) zoomX = 1;
            if (zoomY == 0) zoomY = 1;

            int dynamicGridSpacingX = gridSpacing / zoomX;
            int dynamicGridSpacingY = gridSpacing / zoomY;

            double width = RenderSize.Width != 0 ? RenderSize.Width : 1000;
            double height = RenderSize.Height != 0 ? RenderSize.Height : 1000;

            int startX = (int)(Math.Round((-translateTransform.X) / dynamicGridSpacingX) * dynamicGridSpacingX);
            int endX = (int)(Math.Round(((-translateTransform.X + width)) / dynamicGridSpacingX) * dynamicGridSpacingX);

            int startY = (int)(Math.Round((-translateTransform.Y) / dynamicGridSpacingY) * dynamicGridSpacingY);
            int endY = (int)(Math.Round(((-translateTransform.Y + height)) / dynamicGridSpacingY) * dynamicGridSpacingY);

            drawingContext.PushTransform(new ScaleTransform(1, -1));
        
            drawingContext.DrawRectangle(Brushes.Gray, null,
                new Rect(startX - gridSpacing, startY - gridSpacing, 
                Math.Abs(endX-startX) + gridSpacing, Math.Abs(endY-startY) + gridSpacing));
            //grid
            
            for (int x = startX; x < endX; x+= gridSpacing/zoomX) // parallel to y-axis grid
            {
                drawingContext.DrawLine(new Pen(Brushes.DimGray, 1.0/zoomX), new Point(x, startY), new Point(x, endY));

                if(x != 0)
                {

                    FormattedText ft = new FormattedText((x / (float)gridSpacing).ToString(),
                        CultureInfo.GetCultureInfo("en-us"),
                        FlowDirection.LeftToRight,
                        new Typeface("Verdana"),
                        10/zoomX, System.Windows.Media.Brushes.Black);
                    ft.TextAlignment = TextAlignment.Center;
                    drawingContext.DrawText(ft, new System.Windows.Point(x, 5));
                }
               
            }
           
            for (int y = startY; y < endY; y+= gridSpacing/zoomY) //parllel to x-axis grid
            {
                drawingContext.DrawLine(new Pen(Brushes.DimGray, 1.0 / zoomY), new Point(startX, y), new Point(endX, y));

                if (y != 0)
                {
                    FormattedText ft = new FormattedText((-y / (float)gridSpacing).ToString(),
                         CultureInfo.GetCultureInfo("en-us"),
                         FlowDirection.LeftToRight,
                         new Typeface("Verdana"),
                         10/zoomY, System.Windows.Media.Brushes.Black);
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
            drawingContext.DrawLine(new Pen(Brushes.Black, 1), new Point(startX, 0), new Point(endX, 0));
            drawingContext.DrawLine(new Pen(Brushes.Black, 1), new Point(0, startY), new Point(0, endY));

            //unselected points

            drawingContext.PushTransform(new ScaleTransform(1, -1));
            foreach (CustomPoint point in points)
            {
                if (point.isLinePoint && point != selectedLinePoint)
                {

                    drawingContext.DrawRectangle(Brushes.Red, null, point.GetOffsetRect(scaleTransform));
                }
            }


            //selected point
            if (selectedLinePoint != null)
            {
                CustomPoint point = selectedLinePoint;
                drawingContext.DrawRectangle(Brushes.Green, null, point.GetOffsetRect(scaleTransform));
                foreach (CustomPoint cpPoint in GetControlPoints(selectedLinePoint))
                {
                    point = cpPoint;
                    drawingContext.DrawRectangle(Brushes.Green, null, point.GetOffsetRect(scaleTransform));
                    drawingContext.DrawLine(new Pen(Brushes.Teal, 2/scaleTransform.ScaleX), selectedLinePoint.p, cpPoint.p);
                }



            }
            base.OnRender(drawingContext);


        }

        private void MainWindow_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Focus();
            Keyboard.Focus(this);
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
                    if (point.GetOffsetRect(scaleTransform).Contains(mousePoint))
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
                CustomPoint prevPoint = null;
                int i;
                for (i = 0; i < points.Count; i += 3)
                {
                    if (linePoint.X < points[i].X)
                    {
                        break;
                    }
                    prevPoint = points[i];
                }

                if (prevPoint == null)
                {
                    prevPoint = points[0];
                    Vector dir = new Vector(prevPoint.X - pos.X, prevPoint.Y - pos.Y);
                    CustomPoint cp1 = new CustomPoint(pos.X + dir.X * 0.45, pos.Y + dir.Y * 0.45);
                    CustomPoint cp2 = new CustomPoint(prevPoint.X - dir.X * 0.45, prevPoint.Y - dir.Y * 0.45);
                    points.Insert(0, cp2);
                    points.Insert(0, cp1);
                    points.Insert(0, linePoint);
                }
                else if (prevPoint != points[points.Count - 1])
                {
                    CustomPoint nextPoint = points[i];
                    Vector dir1 = new Vector(prevPoint.X - pos.X, prevPoint.Y - pos.Y);
                    Vector dir2 = new Vector(nextPoint.X - pos.X, nextPoint.Y - pos.Y);
                    CustomPoint cp1 = new CustomPoint(pos.X + dir1.X * 0.45, pos.Y + dir1.Y * 0.45);
                    CustomPoint cp2 = new CustomPoint(pos.X + dir2.X * 0.45, pos.Y + dir2.Y * 0.45);

                    //Update previousPoint's right CP
                    Vector prevCPvec = new Vector(points[i - 2].X - prevPoint.X, points[i - 2].Y - prevPoint.Y);
                    Point tempCP = new Point(points[i - 2].X, points[i - 2].Y);
                    Point prevAsPoint = new Point(prevPoint.X, prevPoint.Y);
                    Vector tempUnit = (prevCPvec / prevCPvec.Length);
                    double scalar = (-dir1 * tempUnit);
                    tempCP = prevAsPoint + tempUnit * scalar * 0.45;
                    points[i - 2] = new CustomPoint(tempCP.X, tempCP.Y);

                    //Update nextPoint's left CP
                    Vector nextCPvec = new Vector(points[i - 1].X - nextPoint.X, points[i - 1].Y - nextPoint.Y);
                    tempCP = new Point(points[i - 1].X, points[i - 1].Y);
                    Point nextAsPoint = new Point(nextPoint.X, nextPoint.Y);
                    tempUnit = (prevCPvec / prevCPvec.Length);
                    scalar = (-dir2 * tempUnit);
                    tempCP = nextAsPoint + tempUnit * scalar * 0.45;
                    points[i - 1] = new CustomPoint(tempCP.X, tempCP.Y);

                    points.Insert(i - 1, cp2);
                    points.Insert(i - 1, linePoint);
                    points.Insert(i - 1, cp1);
                }
                else
                {
                    Vector dir = new Vector(pos.X - prevPoint.X, pos.Y - prevPoint.Y);
                    CustomPoint cp1 = new CustomPoint(prevPoint.X + dir.X * 0.45, prevPoint.Y + dir.Y * 0.45);
                    CustomPoint cp2 = new CustomPoint(pos.X - dir.X * 0.45, pos.Y - dir.Y * 0.45);

                    points.Add(cp1);
                    points.Add(cp2);
                    points.Add(linePoint);
                }
            }

            UpdateRawPoints();
            linePoint.visible = true;
            linePoint.isLinePoint = true;

            InvalidateVisual();
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
                InvalidateVisual();
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

        public Rect GetOffsetRect(ScaleTransform scale)
        {
            Rect offsetRect = new Rect(rect.Location, new Size(rect.Width/scale.ScaleX, rect.Height/scale.ScaleY));
            offsetRect.Offset(-offsetRect.Width/2, -offsetRect.Height/2);
            return offsetRect;
        }
        public void MoveTo(Point newPoint)
        {
            rectangle.X = newPoint.X;
            rectangle.Y = newPoint.Y;
        }
    }
}