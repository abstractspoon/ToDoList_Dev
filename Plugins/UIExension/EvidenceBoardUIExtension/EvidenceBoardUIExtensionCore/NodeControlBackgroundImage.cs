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

		private float AspectRatio { get { return Geometry2D.GetAspectRatio(Image.Size); } }

		public void ResizeToFit(Rectangle extents)
		{
			Rectangle rect = Rectangle.Empty;

			if (HasImage)
			{
				float imageAspect = AspectRatio;
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
		
		public DragMode HitTest(Point point, int edgeWidth)
		{
			var outerRect = Rectangle.Inflate(Bounds, (edgeWidth / 2), (edgeWidth / 2));

			if (!outerRect.Contains(point))
				return DragMode.None;

			var innerRect = Rectangle.Inflate(Bounds, -(edgeWidth / 2), -edgeWidth / 2);

			if (innerRect.Contains(point))
				return DragMode.Background;

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

		public bool Resize(DragMode edge, int newValue)
		{
			var bounds = Bounds;

			switch (edge)
			{
			case DragMode.BackgroundLeft:
				bounds.Width += (bounds.Left - newValue);
				bounds.X = newValue;
				break;

			case DragMode.BackgroundRight:
				bounds.Width -= (bounds.Right - newValue);
				break;

			case DragMode.BackgroundTop:
				bounds.Height += (bounds.Top - newValue);
				bounds.Y = newValue;
				break;

			case DragMode.BackgroundBottom:
				bounds.Height -= (bounds.Bottom - newValue);
				break;

			default:
				return false;
			}

			if (bounds.Equals(Bounds))
				return false;

			// Preserve aspect ratio
			float imageAspect = AspectRatio;

			if (bounds.Width != Bounds.Width)
			{
				// Recalculate height
				int newHeight = (int)(bounds.Width / imageAspect);
				int diff = (bounds.Height - newHeight);

				bounds.Y += (diff / 2);
				bounds.Height -= diff;
			}
			else // height changed
			{
				// Recalculate width
				int newWidth = (int)(bounds.Height * imageAspect);
				int diff = (bounds.Width - newWidth);

				bounds.X += (diff / 2);
				bounds.Width -= diff;
			}

			Bounds = bounds;
			return true;
		}

		public Cursor GetCursor(DragMode edge)
		{
			switch (edge)
			{
			case DragMode.Background:
				return Cursors.Arrow;

			case DragMode.BackgroundLeft:
			case DragMode.BackgroundRight:
				return Cursors.SizeWE;

			case DragMode.BackgroundTop:
			case DragMode.BackgroundBottom:
				return Cursors.SizeNS;
			}

			return null;
		}
	}
}
