using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;

namespace ImageHelper
{
	public static class ExtensionMethods
	{
		public static bool HasArea(this Rectangle rect)
		{
			return ((rect.Width != 0) && (rect.Height != 0));
		}
	}

	///////////////////////////////////////////////////////////////////////////////////

	public class ImageUtils
	{
		public static string ImageFilter
		{
			get { return "Image Files (*.png, *.bmp, *.ico, *.jpg, *.jpeg, *.tiff, *.gif)|*.png;*.bmp;*.ico;*.jpg;*.jpeg;*.tiff;*.gif"; }
		}

		public static bool DrawZoomedImage(Image image, Graphics graphics, Rectangle destRect, Rectangle clipRect)
		{
			if (!destRect.HasArea() || !clipRect.HasArea())
				return false;

			var gSave = graphics.Save();

			var attrib = new ImageAttributes();
			attrib.SetWrapMode(WrapMode.TileFlipXY);

			graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
			graphics.SmoothingMode = SmoothingMode.HighQuality;

			graphics.IntersectClip(clipRect);
			graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, attrib);
			graphics.Restore(gSave);

			return true;
		}

		public static bool SaveImageToFile(Bitmap image, string filepath)
		{
			if (image == null)
				return false;

			string ext = Path.GetExtension(filepath).ToLowerInvariant();

			try
			{
				if (ext == ".png")
				{
					image.Save(filepath, ImageFormat.Png);
				}
				else if (ext == ".bmp")
				{
					image.Save(filepath, ImageFormat.Bmp);
				}
				else if (ext == ".tif")
				{
					image.Save(filepath, ImageFormat.Tiff);
				}
				else if ((ext == ".jpg") || (ext == ".jpeg"))
				{
					image.Save(filepath, ImageFormat.Jpeg);
				}
				else if (ext == ".gif")
				{
					image.Save(filepath, ImageFormat.Gif);
				}
				else
				{
					return false;
				}
			}
			catch (Exception)
			{
				return false;
			}

			return true;
		}
	}
}
