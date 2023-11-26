using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

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
	}
}
