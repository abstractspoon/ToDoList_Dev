using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace EvidenceBoardUIExtension
{
	public class NodeControlBackgroundImage
	{


		// -------------------------------------------------

		public Image Image { get; private set; }
		public string FilePath { get; private set; } = string.Empty;
		public Rectangle Bounds { get; private set; } = Rectangle.Empty;

		public bool HasImage { get { return (Image != null); } }

		public bool Set(string filePath, Rectangle rect)
		{
			if (string.IsNullOrWhiteSpace(filePath))
				return Clear();

			if (filePath == FilePath)
				return false;

			var image = Image.FromFile(filePath);

			if (image == null)
				return false;

			Image = image;

			FilePath = filePath;
			Bounds = rect;

			return true;
		}

		public bool Clear()
		{
			if (Image == null)
				return false;

			Image = null;
			FilePath = string.Empty;
			Bounds = Rectangle.Empty;

			return true;
		}

		public void ResizeWidth(int height)
		{

		}

		public void ResizeHeight(int width)
		{

		}

		public void ResizeToFit(Rectangle extents)
		{
			Rectangle rect = Rectangle.Empty;

			if (HasImage)
			{
				float imageAspect = Geometry2D.GetAspectRatio(Image.Size);
				float extentAspect = Geometry2D.GetAspectRatio(extents.Size);

				int newWidth = extents.Width;

				if (imageAspect < extentAspect)
				{
					// Extents height is the limiting factor
					newWidth = (int)(extents.Height * imageAspect);
				}

				rect = Geometry2D.GetCentredRect(Geometry2D.Centroid(extents), newWidth, imageAspect);
			}

			Bounds = rect;
		}



		public DragMode HitTestEdges(Point point, int hitWidth)
		{
			var outerRect = Rectangle.Inflate(Bounds, hitWidth / 2, hitWidth / 2);

			if (!outerRect.Contains(point))
				return DragMode.None;

			var innerRect = Rectangle.Inflate(Bounds, -hitWidth / 2, -hitWidth / 2);

			if (innerRect.Contains(point))
				return DragMode.None;

			if ((point.Y > innerRect.Top) && (point.Y < innerRect.Bottom))
			{
				if (point.X <= innerRect.Left)
					return DragMode.BackgroundLeft;

				// else
				return DragMode.BackgroundRight;
			}
			else if ((point.X > innerRect.Left) && (point.X < innerRect.Right))
			{
				if (point.Y <= innerRect.Top)
					return DragMode.BackgroundTop;

				// else
				return DragMode.BackgroundBottom;
			}

			// else
			return DragMode.None;
		}
	}
}
