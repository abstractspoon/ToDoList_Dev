// -- FILE ------------------------------------------------------------------
// name       : RtfConvertedImageInfo.cs
// project    : RTF Framelet
// created    : Jani Giannoudis - 2008.10.13
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace Itenso.Rtf.Converter.Image
{

	// ------------------------------------------------------------------------
	public class RtfConvertedImageInfo : IRtfConvertedImageInfo
	{

		// ----------------------------------------------------------------------
		public RtfConvertedImageInfo( string fileName, ImageFormat format, Size size )
		{
			if ( fileName == null )
			{
				throw new ArgumentNullException( "fileName" );
			}

			this.fileName = fileName;
			this.format = format;
			this.size = size;
		} // RtfConvertedImageInfo

		// ----------------------------------------------------------------------
		public string FileName
		{
			get { return this.fileName; }
		} // FileName

		// ----------------------------------------------------------------------
		public ImageFormat Format
		{
			get { return this.format; }
		} // Format

		// ----------------------------------------------------------------------
		public Size Size
		{
			get { return this.size; }
		} // Size

		// ----------------------------------------------------------------------
		public override string ToString()
		{
			return fileName + " " + format + " " + size.Width + "x" + size.Height;
		} // ToString

		// ----------------------------------------------------------------------
		// members
		private readonly string fileName;
		private readonly ImageFormat format;
		private readonly Size size;

	} // class RtfConvertedImageInfo

} // namespace Itenso.Rtf.Converter.Image
// -- EOF -------------------------------------------------------------------
