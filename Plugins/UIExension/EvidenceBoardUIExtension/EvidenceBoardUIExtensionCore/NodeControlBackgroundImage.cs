using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace EvidenceBoardUIExtension
{
	public class NodeControlBackgroundImage
	{
		public Image Image { get; private set; }
		public string FilePath { get; private set; } = string.Empty;
		public Rectangle Extents { get; private set; } = Rectangle.Empty;

		public bool IsValid { get { return (Image != null); } }

		public bool Set(string filePath, Rectangle rect)
		{
			if (string.IsNullOrWhiteSpace(filePath))
				return Clear();

			if ((filePath == FilePath) || !File.Exists(filePath))
				return false;

			var image = Image.FromFile(filePath);

			if (image == null)
				return false;

			Image = image;

			FilePath = filePath;
			Extents = rect;

			return true;
		}

		public bool Clear()
		{
			if (Image == null)
				return false;

			Image = null;
			FilePath = string.Empty;
			Extents = Rectangle.Empty;

			return true;
		}

		private float AspectRatio { get { return Geometry2D.GetAspectRatio(Image.Size); } }

		public Point RelativeToAbsolute(PointF ptRelative)
		{
			if (!UserLinkTarget.IsValid(ptRelative))
			{
				Debug.Assert(false);
				return Point.Empty;
			}

			var ptAbsolute = ptRelative;

			ptAbsolute.X *= Extents.Width;
			ptAbsolute.Y *= Extents.Height;

			ptAbsolute.X += Extents.X;
			ptAbsolute.Y += Extents.Y;

			return Point.Round(ptAbsolute);
		}

		public PointF AbsoluteToRelative(Point ptAbsolute)
		{
			if (!Extents.Contains(ptAbsolute))
			{
				Debug.Assert(false);
				return PointF.Empty;
			}

			PointF ptRelative = ptAbsolute;

			ptRelative.X -= Extents.X;
			ptRelative.Y -= Extents.Y;

			ptRelative.X /= Extents.Width;
			ptRelative.Y /= Extents.Height;

			return ptRelative;
		}

		public Size CalcSizeToFit(Size size)
		{
			if (!IsValid)
				return Size.Empty;

			float imageAspect = AspectRatio;
			float extentAspect = Geometry2D.GetAspectRatio(size);

			int newWidth = size.Width;

			if (imageAspect < extentAspect)
			{
				// Extents height is the limiting factor
				newWidth = (int)(size.Height * imageAspect);
			}

			return new Size(newWidth, (int)(newWidth / imageAspect));
		}

		public void ResizeToFit(Rectangle extents)
		{
			if (IsValid)
			{
				var size = CalcSizeToFit(extents.Size);
				Extents = Geometry2D.GetCentredRect(Geometry2D.Centroid(extents), size.Width, size.Height);
			}
			else
			{
				Extents = Rectangle.Empty;
			}
		}
		
		public DragMode HitTest(Point point, int edgeWidth)
		{
			if (IsValid)
			{
				var outerRect = Rectangle.Inflate(Extents, (edgeWidth / 2), (edgeWidth / 2));

				if (!outerRect.Contains(point))
					return DragMode.None;

				var innerRect = Rectangle.Inflate(Extents, -(edgeWidth / 2), -edgeWidth / 2);

				if (innerRect.Contains(point))
					return DragMode.BackgroundImage;

				if ((point.Y > innerRect.Top) && (point.Y < innerRect.Bottom))
				{
					if (point.X <= innerRect.Left)
						return DragMode.BackgroundImageLeft;

					// else
					return DragMode.BackgroundImageRight;
				}
				else if ((point.X > innerRect.Left) && (point.X < innerRect.Right))
				{
					if (point.Y <= innerRect.Top)
						return DragMode.BackgroundImageTop;

					// else
					return DragMode.BackgroundImageBottom;
				}
			}

			// else
			return DragMode.None;
		}

		public bool SetReposition(Point newCentre)
		{
			if (!IsValid)
				return false;

			var centre = Geometry2D.Centroid(Extents);

			if (newCentre == centre)
				return false;

			Extents = Geometry2D.GetCentredRect(newCentre, Extents.Width, Extents.Height);
			return true;
		}

		public bool SetBounds(Rectangle bounds)
		{
			if (!IsValid)
				return false;

			if (bounds.Width <= 0 && bounds.Height <= 0)
				return false;

			if (bounds == Extents)
				return false;

			Extents = bounds;
			return true;
		}

		public bool InflateWidth(int amount, Size minSize)
		{
			if (!IsValid || (minSize.Width <= 0))
				return false;

			if ((Extents.Width + amount) < minSize.Width)
				amount = (minSize.Width - Extents.Width);

			if (amount == 0)
				return false;

			// Calc new width and height preserving aspect ratio
			int newWidth = (Extents.Width + amount);
			int newHeight = (int)(newWidth / AspectRatio);

			Extents = Geometry2D.GetCentredRect(Geometry2D.Centroid(Extents), newWidth, newHeight);
			return true;
		}

		public bool InflateHeight(int amount, Size minSize)
		{
			if (!IsValid || (minSize.Height <= 0))
				return false;

			if (amount < 0)
				amount = Math.Max(minSize.Height, (Extents.Height + amount)) - Extents.Height;

			if (amount == 0)
				return false;

			// Calc new width and height preserving aspect ratio
			int newHeight = (Extents.Height + amount);
			int newWidth = (int)(newHeight * AspectRatio);

			Extents = Geometry2D.GetCentredRect(Geometry2D.Centroid(Extents), newWidth, newHeight);
			return true;
		}

		public Cursor GetCursor(DragMode edge)
		{
			switch (edge)
			{
			case DragMode.BackgroundImage:
				return Cursors.Arrow;

			case DragMode.BackgroundImageLeft:
			case DragMode.BackgroundImageRight:
				return Cursors.SizeWE;

			case DragMode.BackgroundImageTop:
			case DragMode.BackgroundImageBottom:
				return Cursors.SizeNS;
			}

			return null;
		}

		public string Encode()
		{
			if (!IsValid)
				return string.Empty;

			return string.Format("{0}|{1},{2},{3},{4}",
								FilePath,
								Extents.Left,
								Extents.Top,
								Extents.Right,
								Extents.Bottom);
		}

		static public bool TryParse(string metaData, out string filePath, out Rectangle rect)
		{
			filePath = string.Empty;
			rect = Rectangle.Empty;

			var parts = metaData.Split('|');

			if (parts.Count() >= 2)
			{
				var dims = parts[1].Split(',');

				if (dims.Count() != 4)
					return false;

				rect = Rectangle.FromLTRB(int.Parse(dims[0]),
										int.Parse(dims[1]),
										int.Parse(dims[2]),
										int.Parse(dims[3]));
				filePath = parts[0];
			}

			/*
			if (parts.Count() >= 3)
			{
			}  
			*/

			return true;
		}
	}
}
