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
        public bool expandedInPropertyGrid = false;

        public Bezier()
        {
            points = new List<Point>();
        }

        double Interpolate(int indexLP, double x)
        {
            double p0x = points[indexLP].X;
            return (x - p0x) / (points[indexLP + 3].X - p0x);
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
            int currentMaxPos = 0;
            for (int i = 0; i < points.Count; i += 3)
            {
                int newMaxPos = i;
                
                if (points[newMaxPos].X >= x)
                {
                    return currentMaxPos;
                }
                else
                    currentMaxPos = newMaxPos;
                
            }
            return 0;
        }

        public double GetYFromX(double x)
        {
            if (points.Count == 0)
                return 1;
            if (x < points[0].X)
                return points[0].Y;
            else if (x > points[points.Count - 1].X)
                return points[points.Count-1].Y;
            else
            {
                int lp = GetLinePointIndexToLeft(x);
                return BezierFunction(lp, Interpolate(lp, x));
            }

            
        }
    }
}
