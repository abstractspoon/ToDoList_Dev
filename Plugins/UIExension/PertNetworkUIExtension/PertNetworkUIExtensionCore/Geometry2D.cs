using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace PertNetworkUIExtension
{
	public class Geometry2D
	{
		public static double Distance(Point ptStart, Point ptEnd)
		{
			return Math.Sqrt(DistanceSquared(ptStart, ptEnd));
		}

		public static double DistanceSquared(Point ptStart, Point ptEnd)
		{
			return (((ptStart.X - ptEnd.X) * (ptStart.X - ptEnd.X)) +
					((ptStart.Y - ptEnd.Y) * (ptStart.Y - ptEnd.Y)));
		}

		public static bool DistanceFromPointToSegment(Point pt, Point ptStart, Point ptEnd, ref double distance, ref Point ptIntersection)
		{
			double distSqrd = DistanceSquared(ptStart, ptEnd);

			if (distSqrd == 0.0)
			{
				return false;
			}

			double U = ((pt.X - ptStart.X) * (ptEnd.X - ptStart.X)) +
					   ((pt.Y - ptStart.Y) * (ptEnd.Y - ptStart.Y));

			U /= distSqrd;

			if ((U < 0.0) || (U > 1.0))
			{
				// closest point does not fall within the line segment
				return false;
			}

			ptIntersection.X = (int)(ptStart.X + (U * (ptEnd.X - ptStart.X)));
			ptIntersection.Y = (int)(ptStart.Y + (U * (ptEnd.Y - ptStart.Y)));

			distance = Distance(pt, ptIntersection);

			return true;
		}

		public static bool HitTest(Point[] points, 
									Point point, 
									double tolerance,
									ref int segment, 
									ref Point ptIntersection)
		{
			for (int i = 1; i < points.Count(); i++)
			{
				Point ptStart = points[i - 1];
				Point ptEnd = points[i];

				double distance = double.MaxValue;
				Point ptOnSeg = Point.Empty;

				if (Geometry2D.DistanceFromPointToSegment(point,
											   ptStart,
											   ptEnd,
											   ref distance,
											   ref ptOnSeg))
				{
					if (distance <= tolerance)
					{
						ptIntersection = ptOnSeg;
						segment = (i - 1);

						// We can stop as soon as we hit tolerance
						return true;
					}
				}
			}

			return false;
		}
	}

}
