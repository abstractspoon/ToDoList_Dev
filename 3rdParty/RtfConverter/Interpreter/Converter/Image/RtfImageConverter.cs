// -- FILE ------------------------------------------------------------------
// name       : RtfImageConverter.cs
// project    : RTF Framelet
// created    : Jani Giannoudis - 2008.05.31
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.IO;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using Itenso.Rtf.Model;
using Itenso.Rtf.Interpreter;

namespace Itenso.Rtf.Converter.Image
{

	// ------------------------------------------------------------------------
	public class RtfImageConverter : RtfInterpreterListenerBase
	{

		// ----------------------------------------------------------------------
		public RtfImageConverter()
			: this( new RtfImageConvertSettings() )
		{
		} // RtfImageConverter

		// ----------------------------------------------------------------------
		public RtfImageConverter( RtfImageConvertSettings settings )
		{
			if ( settings == null )
			{
				throw new ArgumentNullException( "settings" );
			}

			this.settings = settings;
		} // RtfImageConverter

		// ----------------------------------------------------------------------
		public RtfImageConvertSettings Settings
		{
			get { return this.settings; }
		} // Settings

		// ----------------------------------------------------------------------
		public RtfConvertedImageInfoCollection ConvertedImages
		{
			get { return this.convertedImages; }
		} // ConvertedImages

		// ----------------------------------------------------------------------
		protected override void DoBeginDocument( IRtfInterpreterContext context )
		{
			base.DoBeginDocument( context );

			this.convertedImages.Clear();
		} // DoBeginDocument

		// ----------------------------------------------------------------------
		protected override void DoInsertImage( IRtfInterpreterContext context,
			RtfVisualImageFormat format,
			int width, int height, 
			int desiredWidth, int desiredHeight,
			int scaleWidthPercent, int scaleHeightPercent,
			string imageDataHex
		)
		{
			int imageIndex = this.convertedImages.Count + 1;
			string fileName = this.settings.GetImageFileName( imageIndex, format );
			EnsureImagesPath( fileName );

			byte[] imageBuffer = RtfVisualImage.ToBinary( imageDataHex );
			Size imageSize;
			ImageFormat imageFormat;
			if ( this.settings.ImageAdapter.TargetFormat == null )
			{
				using ( System.Drawing.Image image = System.Drawing.Image.FromStream( new MemoryStream( imageBuffer ) ) )
				{
					imageFormat = image.RawFormat;
					imageSize = image.Size;
				}
				using ( BinaryWriter binaryWriter = new BinaryWriter( File.Open( fileName, FileMode.Create ) ) )
				{
					binaryWriter.Write( imageBuffer );
				}
			}
			else
			{
				imageFormat = this.settings.ImageAdapter.TargetFormat;
				if ( this.settings.ScaleImage )
				{
					imageSize = new Size(
					 this.settings.ImageAdapter.CalcImageWidth( format, width, desiredWidth, scaleWidthPercent ),
					 this.settings.ImageAdapter.CalcImageHeight( format, height, desiredHeight, scaleHeightPercent ) );
				}
				else
				{
					imageSize = new Size( width, height );
				}

				SaveImage( imageBuffer, format, fileName, imageSize );
			}

			this.convertedImages.Add( new RtfConvertedImageInfo( fileName, imageFormat, imageSize ) );
		} // DoInsertImage

		// ----------------------------------------------------------------------
		protected virtual void SaveImage( byte[] imageBuffer, RtfVisualImageFormat format, string fileName, Size size )
		{
			ImageFormat targetFormat = this.settings.ImageAdapter.TargetFormat;

			float scaleOffset = this.settings.ScaleOffset;
			float scaleExtension = this.settings.ScaleExtension;
			using ( System.Drawing.Image image = System.Drawing.Image.FromStream(
				new MemoryStream( imageBuffer, 0, imageBuffer.Length ) ) )
			{
				Bitmap convertedImage = new Bitmap( new Bitmap( size.Width, size.Height, image.PixelFormat ) );
				Graphics graphic = Graphics.FromImage( convertedImage );
				graphic.CompositingQuality = CompositingQuality.HighQuality;
				graphic.SmoothingMode = SmoothingMode.HighQuality;
				graphic.InterpolationMode = InterpolationMode.HighQualityBicubic;
				RectangleF rectangle = new RectangleF( 
					scaleOffset, 
					scaleOffset, 
					size.Width + scaleExtension, 
					size.Height + scaleExtension );

				if ( this.settings.BackgroundColor.HasValue )
				{
					graphic.Clear( this.settings.BackgroundColor.Value );
				}

				graphic.DrawImage( image, rectangle );
				convertedImage.Save( fileName, targetFormat );
			}
		} // SaveImage

		// ----------------------------------------------------------------------
		protected virtual void EnsureImagesPath( string imageFileName )
		{
			FileInfo fi = new FileInfo( imageFileName );
			if ( !string.IsNullOrEmpty( fi.DirectoryName ) && !Directory.Exists( fi.DirectoryName ) )
			{
				Directory.CreateDirectory( fi.DirectoryName );
			}
		} // EnsureImagesPath

		// ----------------------------------------------------------------------
		// members
		private readonly RtfConvertedImageInfoCollection convertedImages = new RtfConvertedImageInfoCollection();
		private readonly RtfImageConvertSettings settings;

	} // class RtfImageConverter

} // namespace Itenso.Rtf.Converter.Image
// -- EOF -------------------------------------------------------------------
