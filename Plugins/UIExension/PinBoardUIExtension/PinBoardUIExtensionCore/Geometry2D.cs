using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace PinBoardUIExtension
{
	public class Geometry2D
	{
		public static Point Difference(Point ptStart, Point ptEnd)
		{
			return new Point(ptEnd.X - ptStart.X, ptEnd.Y - ptStart.Y);
		}

		public static Point MidPoint(Point ptStart, Point ptEnd)
		{
			return new Point((ptStart.X + ptEnd.X) / 2, (ptStart.Y + ptEnd.Y) / 2);
		}

		public static double Distance(Point ptStart, Point ptEnd)
		{
			return Math.Sqrt(DistanceSquared(ptStart, ptEnd));
		}

		public static double DistanceSquared(Point ptStart, Point ptEnd)
		{
			var ptDiff = Difference(ptEnd, ptStart);

			return ((ptDiff.X * ptDiff.X) + (ptDiff.Y * ptDiff.Y));
		}

		public static bool DistanceFromPointToSegment(Point pt, Point segStart, Point segEnd, ref double distance, ref Point ptIntersection)
		{
			double distSqrd = DistanceSquared(segStart, segEnd);

			if (distSqrd == 0.0)
			{
				return false;
			}

			double U = ((pt.X - segStart.X) * (segEnd.X - segStart.X)) +
					   ((pt.Y - segStart.Y) * (segEnd.Y - segStart.Y));

			U /= distSqrd;

			if ((U < 0.0) || (U > 1.0))
			{
				// closest point does not fall within the line segment
				return false;
			}

			ptIntersection.X = (int)(segStart.X + (U * (segEnd.X - segStart.X)));
			ptIntersection.Y = (int)(segStart.Y + (U * (segEnd.Y - segStart.Y)));

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

		public static int HitTest(Point[] points, 
									Point point, 
									double tolerance)
		{
			int segment = -1;
			Point ptUnused = Point.Empty;

			if (HitTest(points, point, tolerance, ref segment, ref ptUnused))
				return segment;

			return -1;
		}

		public static Point SegmentMidPoint(Point[] points, int segment)
		{
			if ((segment < 0) || (segment >= points.Count()))
				return Point.Empty;

			return MidPoint(points[segment], points[segment + 1]);
		}

		public static Point Centroid(Rectangle rect)
		{
			return new Point(rect.Left + (rect.Width / 2), rect.Top + (rect.Height / 2));
		}
	}

}
