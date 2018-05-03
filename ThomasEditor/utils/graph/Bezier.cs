using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Thomas_Graph
{
    public class Bezier
    {
        public List<Point> points;
       

        public Bezier()
        {
            points = new List<Point>();
        }

        double Interpolate(double x)
        {
            double p0x = points[0].X;
            return (x - p0x) / (points[points.Count - 1].X - p0x);
        }

        double BezierFunction(int indexLP, double t)
        {
            double omt = (1 - t);
            double tt = t * t;
            double l0 = omt * omt * omt;
            double l1 = 3 * t * omt * omt;
            double l2 = 3 * tt * omt;
            double l3 = tt * t;
            return points[indexLP].Y * l0 + points[indexLP + 1].Y * l1 + points[indexLP + 2].Y * l2 + points[indexLP + 3].Y * l3;
        }

        int GetLinePointIndexToLeft(double x)
        {
            if (x < points[0].X || x > points[points.Count - 1].X)
                return -1;
            int currentMaxPos = 0;
            for (int i = 0; i < points.Count; i += 4)
            {
                int newMaxPos = i;
                if (points[newMaxPos].X > x)
                    break;
                currentMaxPos = newMaxPos;
            }

            return currentMaxPos;
        }

        public double GetYFromX(double x)
        {
            int lp = GetLinePointIndexToLeft(x);
            if (lp == -1)
                return points[0].Y; //Error
            return BezierFunction(lp, Interpolate(x));
        }
    }
}
