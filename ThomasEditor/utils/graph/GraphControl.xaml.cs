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
using ThomasEditor;

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

        enum ManipulateMode { All, Vertical, Horizontal};
        ManipulateMode currentMode = ManipulateMode.All;

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
            this.KeyUp += GraphControl_KeyUp;
            
            translateTransform = new TranslateTransform(0, 0);
            TransformGroup group = new TransformGroup();
            group.Children.Add(new ScaleTransform(1, -1)); //flip
            group.Children.Add(translateTransform);
            group.Children.Add(scaleTransform);
            this.RenderTransform = group;

            this.Loaded += GraphControl_Loaded;
            Focusable = true;
            InvalidateVisual();
            

            Curve asdf = new Curve();
            String testString = "((x+2)*2 - 4) ^(2 + 1)";
            double testF = asdf.ParseFunction(ref testString, 2);
            

            double asdfasdf = 0.0;
            asdfasdf++;
        }

        private void GraphControl_KeyUp(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.LeftShift:
                case Key.LeftCtrl:
                    currentMode = ManipulateMode.All;
                    break;
            }
        }

        private void GraphControl_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.LeftShift:
                    currentMode = ManipulateMode.Vertical;
                    Cursor = Cursors.SizeNS;
                    break;
                case Key.LeftCtrl:
                    currentMode = ManipulateMode.Horizontal;
                    Cursor = Cursors.SizeWE;
                    break;
                case Key.Delete:
                    {
                        if(selectedLinePoint != null)
                        {
                            RemovePoint(selectedLinePoint);
                            selectedLinePoint = null;
                        }
                    }
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
            translateTransform.Y += ((RenderSize.Height != 0 ? RenderSize.Height : 350) - 100)/10;
            translateTransform.X += 50;
            this.Loaded -= GraphControl_Loaded;
            path.StrokeThickness = 1 / ((scaleTransform.ScaleX + scaleTransform.ScaleY) / 2.0);
            InvalidateVisual();
        }

        private void GraphControl_MouseUp(object sender, MouseButtonEventArgs e)
        {
            selectedPoint = null;
        }

        private void GraphControl_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            Point pos = e.GetPosition(this);
            double delta = e.Delta * 0.01;
            if(scaleTransform.ScaleX + delta > 1 && scaleTransform.ScaleX + delta < 15 && (currentMode == ManipulateMode.Horizontal || currentMode == ManipulateMode.All))
                scaleTransform.ScaleX += delta;
            if (scaleTransform.ScaleY + delta > 1 && scaleTransform.ScaleY + delta < 15 && (currentMode == ManipulateMode.Vertical || currentMode == ManipulateMode.All))
                scaleTransform.ScaleY += delta;


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
            
           

            int dynamicGridSpacingX = (int)(gridSpacing / scaleTransform.ScaleX);
            int dynamicGridSpacingY = (int)(gridSpacing / scaleTransform.ScaleY);

            double width = RenderSize.Width != 0 ? RenderSize.Width : 1000;
            double height = RenderSize.Height != 0 ? RenderSize.Height : 1000;

            int startX = (int)(Math.Round((-translateTransform.X) / dynamicGridSpacingX) * dynamicGridSpacingX) - dynamicGridSpacingX;
            int endX = (int)(Math.Round(((-translateTransform.X + width)) / dynamicGridSpacingX) * dynamicGridSpacingX) + dynamicGridSpacingX;

            int startY = (int)(Math.Round((-translateTransform.Y) / dynamicGridSpacingY) * dynamicGridSpacingY) - dynamicGridSpacingY;
            int endY = (int)(Math.Round(((-translateTransform.Y + height)) / dynamicGridSpacingY) * dynamicGridSpacingY) + dynamicGridSpacingY;
            drawingContext.PushTransform(new ScaleTransform(1, -1));
           
        
            drawingContext.DrawRectangle(Brushes.Gray, null,
                new Rect(startX - gridSpacing, startY - gridSpacing, 
                Math.Abs(endX-startX) + gridSpacing, Math.Abs(endY-startY) + gridSpacing));
            //grid

            double largestScale = (scaleTransform.ScaleX + scaleTransform.ScaleY)/2;

            
            for (int x = startX; x < endX; x+= dynamicGridSpacingX) // x-axis
            {
                drawingContext.DrawLine(new Pen(Brushes.DimGray, 1.0/scaleTransform.ScaleX), new Point(x, startY), new Point(x, endY));
                drawingContext.PushTransform(new ScaleTransform(scaleTransform.ScaleY, scaleTransform.ScaleX));
                if (x != 0)
                {

                    FormattedText ft = new FormattedText((x / (float)gridSpacing).ToString(),
                        CultureInfo.GetCultureInfo("en-us"),
                        FlowDirection.LeftToRight,
                        new Typeface("Verdana"),
                       12/ Math.Pow(largestScale, 2), System.Windows.Media.Brushes.Black);
                    ft.TextAlignment = TextAlignment.Center;
                    drawingContext.DrawText(ft, new System.Windows.Point(x/scaleTransform.ScaleY, 1.0/ scaleTransform.ScaleX));
                }
                drawingContext.PushTransform(new ScaleTransform(1 / scaleTransform.ScaleY, 1 / scaleTransform.ScaleX));

            }
           
            for (int y = startY; y < endY; y+= (int)(gridSpacing/ scaleTransform.ScaleY)) //y-axis
            {
                drawingContext.DrawLine(new Pen(Brushes.DimGray, 1.0 / scaleTransform.ScaleY), new Point(startX, y), new Point(endX, y));
                drawingContext.PushTransform(new ScaleTransform(scaleTransform.ScaleY, scaleTransform.ScaleX));
                if (y != 0)
                {
                    FormattedText ft = new FormattedText((-y / (float)gridSpacing).ToString(),
                         CultureInfo.GetCultureInfo("en-us"),
                         FlowDirection.LeftToRight,
                         new Typeface("Verdana"),
                         12/Math.Pow(largestScale, 2), System.Windows.Media.Brushes.Black);
                    ft.TextAlignment = TextAlignment.Right;
                    drawingContext.DrawText(ft, new System.Windows.Point(-1.0/ scaleTransform.ScaleY, (y/ scaleTransform.ScaleX)));
                }
                //else
                //{
                //    FormattedText ft = new FormattedText("0",
                //         CultureInfo.GetCultureInfo("en-us"),
                //         FlowDirection.LeftToRight,
                //         new Typeface("Verdana"),
                //         1.0/ avgScale, System.Windows.Media.Brushes.Black);
                //    ft.TextAlignment = TextAlignment.Right;
                //    drawingContext.DrawText(ft, new System.Windows.Point(-1.0/ scaleTransform.ScaleY, 2.0/ scaleTransform.ScaleX));
                //}
                  drawingContext.PushTransform(new ScaleTransform(1 / scaleTransform.ScaleY, 1 / scaleTransform.ScaleX));

            }
          //  drawingContext.PushTransform(new ScaleTransform(1/scaleTransform.ScaleY, 1/scaleTransform.ScaleX));

            //axis
            drawingContext.DrawLine(new Pen(Brushes.Black, 1.0 / largestScale), new Point(startX, 0), new Point(endX, 0));
            drawingContext.DrawLine(new Pen(Brushes.Black, 1.0 / largestScale), new Point(0, startY), new Point(0, endY));

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
                    Rect offsetRect = point.GetOffsetRect(scaleTransform);
                    drawingContext.DrawEllipse(Brushes.Green, null, point.p, offsetRect.Width/2, offsetRect.Height/2);
                    drawingContext.DrawLine(new Pen(Brushes.Teal, 2/scaleTransform.ScaleX), selectedLinePoint.p, cpPoint.p);
                }



            }
            base.OnRender(drawingContext);


        }

        private void MainWindow_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Focus();
            Keyboard.Focus(this);

            SelectPointAt(e.GetPosition(this));
            if(e.RightButton == MouseButtonState.Pressed)
            {
                menu.DataContext = selectedLinePoint;
                menu.IsOpen = true;
                addPointMenu.IsEnabled = selectedLinePoint == null;
            }
            InvalidateVisual();


        }

        void SelectPointAt(Point position)
        {
            CustomPoint point = GetPointAt(position);


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
            else if (i > 1 && !points[i-2].isLinePoint)
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
            InvalidateVisual();
        }

        void RemovePoint(CustomPoint point)
        {
            int pointIndex = points.IndexOf(point);
            bool lastPoint = pointIndex == points.Count - 1;
            foreach(CustomPoint cp in GetControlPoints(point))
            {
                points.Remove(cp);
            }
            points.Remove(point);

            if (pointIndex == 0 && points.Count > 0)
            {
                points.RemoveAt(0);
            }
            else if (lastPoint && points.Count > 0)
                points.RemoveAt(points.Count - 1);
            
            UpdateRawPoints();
            InvalidateVisual();
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
                    CustomPoint cp1 = new CustomPoint(pos.X + dir.X * 0.45, pos.Y);
                    CustomPoint cp2 = new CustomPoint(prevPoint.X - dir.X * 0.45, prevPoint.Y);
                    points.Insert(0, cp2);
                    points.Insert(0, cp1);
                    points.Insert(0, linePoint);
                }
                else if (prevPoint != points[points.Count - 1])
                {
                    CustomPoint nextPoint = points[i];
                    Vector dir1 = new Vector(prevPoint.X - pos.X, prevPoint.Y - pos.Y);
                    Vector dir2 = new Vector(nextPoint.X - pos.X, nextPoint.Y - pos.Y);


                    Vector dir = dir1.Length > dir2.Length ? dir2 : dir1;

                    CustomPoint cp1 = new CustomPoint(pos.X - Math.Abs(dir.X) * 0.45, pos.Y);
                    CustomPoint cp2 = new CustomPoint(pos.X + Math.Abs(dir.X) * 0.45, pos.Y);

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
                    CustomPoint cp1 = new CustomPoint(prevPoint.X + dir.X * 0.45, prevPoint.Y);
                    CustomPoint cp2 = new CustomPoint(pos.X - dir.X * 0.45, pos.Y);

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


        Point GetValidPosition(CustomPoint targetPoint, Point TargetPos)
        {
            int i = points.IndexOf(targetPoint);
            float mouseX = (float)TargetPos.X;
            bool movingLeft = targetPoint.p.X > mouseX;
            if (!targetPoint.isLinePoint) //control point
            {

                if (movingLeft)
                {
                    CustomPoint leftPoint = GetLinePointToTheLeft(targetPoint);
                    if (leftPoint.p.X > mouseX)
                        TargetPos.X = leftPoint.X;
                }
                else
                {
                    CustomPoint rightPoint = GetLinePointToTheRight(targetPoint);
                    if (rightPoint.p.X < mouseX)
                        TargetPos.X = rightPoint.X;
                }
            }
            else
            {
                if (movingLeft && i != 0)
                {
                    CustomPoint leftCP = points[i - 1];
                    if (leftCP.p.X > mouseX)
                        TargetPos.X = leftCP.X;

                    if(i - 2 > 0)
                    {
                        CustomPoint leftLineRCP = points[i - 2];
                        if (leftLineRCP.p.X > mouseX)
                            TargetPos.X = leftLineRCP.X;
                    }
                }
                else if(i < points.Count - 1)
                {

                    CustomPoint rightCP = points[i + 1];
                    if (rightCP.p.X < mouseX)
                        TargetPos.X = rightCP.X;

                    if (i + 2 < points.Count)
                    {
                        CustomPoint RightLineLCP = points[i + 2];
                        if (RightLineLCP.p.X < mouseX)
                            TargetPos.X = RightLineLCP.X;
                    }
                }
            }
            return TargetPos;
        }



        private void MainWindow_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            CustomPoint point = GetPointAt(e.GetPosition(this));
            if (point == null)
                AddPoint(e.GetPosition(this));
        }

        private void HandleMovement(Point targetPoint)
        {
            Point validPos = GetValidPosition(selectedPoint, targetPoint);
            Point deltaMovement = new Point(selectedPoint.p.X - validPos.X, selectedPoint.p.Y - validPos.Y);

            if (selectedPoint == selectedLinePoint)
            {
                foreach (CustomPoint cp in GetControlPoints(selectedLinePoint))
                {
                    Point newPos = new Point(cp.p.X - deltaMovement.X, cp.p.Y - deltaMovement.Y);
                    cp.MoveTo(GetValidPosition(cp, newPos));
                }
                validPos = GetValidPosition(selectedPoint, targetPoint);
                selectedPoint.MoveTo(validPos);
            }
            else //is a control point
            {
                selectedPoint.MoveTo(validPos);
                CustomPoint mirror = GetMirrorControlPoint(selectedPoint);
                if (mirror != null && !selectedLinePoint.unlockedControlPoints)
                {
                    Point cpDelta = new Point(selectedPoint.p.X - selectedLinePoint.p.X, selectedPoint.p.Y - selectedLinePoint.p.Y);
                    Point newPos = new Point(selectedLinePoint.p.X - cpDelta.X, selectedLinePoint.p.Y - cpDelta.Y);
                    mirror.MoveTo(GetValidPosition(mirror, newPos));

                    int i = points.IndexOf(selectedPoint) % 3;

                    if ((i == 1 && selectedPoint.X <= mirror.X) || (i == 2 && selectedPoint.X >= mirror.X))
                    {
                        int a = points.IndexOf(selectedPoint);
                        int b = points.IndexOf(mirror);
                        points[a] = mirror;
                        points[b] = selectedPoint;
                    }

                }

            }
        }


        private void MainWindow_MouseMove(object sender, MouseEventArgs e)
        {
            if(currentMode == ManipulateMode.All)
                Cursor = Cursors.Arrow;
            CustomPoint over = GetPointAt(e.GetPosition(this));
            if (over != null)
            {
                Cursor = Cursors.SizeAll;
            }
           
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                if (selectedPoint != null)
                {
                    Cursor = Cursors.SizeAll;
                    HandleMovement(e.GetPosition(this));
                    InvalidateVisual();
                    OnPointsChanged?.Invoke();

                    UpdateRawPoints();
                }
                else
                {
                    double deltaX = e.GetPosition(this).X - prevMouseX;
                    double deltaY = e.GetPosition(this).Y - prevMouseY;
                    Cursor = Cursors.Hand;
                    translateTransform.X += deltaX;
                    translateTransform.Y -= deltaY;
                    InvalidateVisual();
                }

                
            }
            prevMouseX = e.GetPosition(this).X;
            prevMouseY = e.GetPosition(this).Y;

            
        }

        private void MenuAddPoint(object sender, RoutedEventArgs e)
        {
            AddPoint(Mouse.GetPosition(this));
            SelectPointAt(Mouse.GetPosition(this));
        }

        private void MenuRemovePoint(object sender, RoutedEventArgs e)
        {
            RemovePoint(selectedLinePoint);
            selectedLinePoint = null;
            selectedPoint = null;
            UpdateRawPoints();

        }

        private void MenuUnlockPoint(object sender, RoutedEventArgs e)
        {
            selectedLinePoint.unlockedControlPoints = true;
        }

        private void MenuLockPoint(object sender, RoutedEventArgs e)
        {
            selectedLinePoint.unlockedControlPoints = false;
        }
    }


    public class CustomPoint
    {

        private Rect rectangle;
        public bool isLinePoint = false;
        public bool visible = false;
        public bool unlockedControlPoints = false;

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