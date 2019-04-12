// -- FILE ------------------------------------------------------------------
// name       : RtfVisualImage.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.23
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Globalization;
using System.IO;
using System.Text;
using Itenso.Sys;

namespace Itenso.Rtf.Model
{

	// ------------------------------------------------------------------------
	public sealed class RtfVisualImage : RtfVisual, IRtfVisualImage
	{

		// ----------------------------------------------------------------------
		public RtfVisualImage(
			RtfVisualImageFormat format,
			RtfTextAlignment alignment,
			int width,
			int height,
			int desiredWidth,
			int desiredHeight,
			int scaleWidthPercent,
			int scaleHeightPercent,
			string imageDataHex
		) :
			base( RtfVisualKind.Image )
		{
			if ( width <= 0 )
			{
				throw new ArgumentException( Strings.InvalidImageWidth( width ) );
			}
			if ( height <= 0 )
			{
				throw new ArgumentException( Strings.InvalidImageHeight( height ) );
			}
			if ( desiredWidth <= 0 )
			{
				throw new ArgumentException( Strings.InvalidImageDesiredWidth( desiredWidth ) );
			}
			if ( desiredHeight <= 0 )
			{
				throw new ArgumentException( Strings.InvalidImageDesiredHeight( desiredHeight ) );
			}
			if ( scaleWidthPercent <= 0 )
			{
				throw new ArgumentException( Strings.InvalidImageScaleWidth( scaleWidthPercent ) );
			}
			if ( scaleHeightPercent <= 0 )
			{
				throw new ArgumentException( Strings.InvalidImageScaleHeight( scaleHeightPercent ) );
			}
			if ( imageDataHex == null )
			{
				throw new ArgumentNullException( "imageDataHex" );
			}
			this.format = format;
			this.alignment = alignment;
			this.width = width;
			this.height = height;
			this.desiredWidth = desiredWidth;
			this.desiredHeight = desiredHeight;
			this.scaleWidthPercent = scaleWidthPercent;
			this.scaleHeightPercent = scaleHeightPercent;
			this.imageDataHex = imageDataHex;
		} // RtfVisualImage

		// ----------------------------------------------------------------------
		protected override void DoVisit( IRtfVisualVisitor visitor )
		{
			visitor.VisitImage( this );
		} // DoVisit

		// ----------------------------------------------------------------------
		public RtfVisualImageFormat Format
		{
			get { return this.format; }
		} // Format

		// ----------------------------------------------------------------------
		public RtfTextAlignment Alignment
		{
			get { return this.alignment; }
		} // Alignment

		// ----------------------------------------------------------------------
		public int Width
		{
			get { return this.width; }
		} // Width

		// ----------------------------------------------------------------------
		public int Height
		{
			get { return this.height; }
		} // Height

		// ----------------------------------------------------------------------
		public int DesiredWidth
		{
			get { return this.desiredWidth; }
		} // DesiredWidth

		// ----------------------------------------------------------------------
		public int DesiredHeight
		{
			get { return this.desiredHeight; }
		} // DesiredHeight

		// ----------------------------------------------------------------------
		public int ScaleWidthPercent
		{
			get { return this.scaleWidthPercent; }
		} // ScaleWidthPercent

		// ----------------------------------------------------------------------
		public int ScaleHeightPercent
		{
			get { return this.scaleHeightPercent; }
		} // ScaleHeightPercent

		// ----------------------------------------------------------------------
		public string ImageDataHex
		{
			get { return this.imageDataHex; }
		} // ImageDataHex

		// ----------------------------------------------------------------------
		public byte[] ImageDataBinary
		{
			get
			{
				if ( this.imageDataBinary == null )
				{
					this.imageDataBinary = ToBinary( this.imageDataHex );
				}
				return imageDataBinary;
			}
		} // ImageDataBinary

		// ----------------------------------------------------------------------
		public System.Drawing.Image ImageForDrawing
		{
			get
			{
				switch ( format )
				{
					case RtfVisualImageFormat.Bmp:
					case RtfVisualImageFormat.Jpg:
					case RtfVisualImageFormat.Png:
					case RtfVisualImageFormat.Emf:
					case RtfVisualImageFormat.Wmf:
						byte[] data = ImageDataBinary;
						return System.Drawing.Image.FromStream( new MemoryStream( data, 0, data.Length ) );
				}
				return null;
			}
		} // ImageForDrawing

		// ----------------------------------------------------------------------
		public static byte[] ToBinary( string imageDataHex )
		{
			if ( imageDataHex == null )
			{
				throw new ArgumentNullException ("imageDataHex" );
			}

			int hexDigits = imageDataHex.Length;
			int dataSize = hexDigits / 2;
			byte[] imageDataBinary = new byte[ dataSize ];

			StringBuilder hex = new StringBuilder( 2 );

			int dataPos = 0;
			for ( int i = 0; i < hexDigits; i++ )
			{
				hex.Append( imageDataHex[ i ] );
				if ( hex.Length == 2 )
				{
					imageDataBinary[ dataPos ] = byte.Parse( hex.ToString(), NumberStyles.HexNumber );
					dataPos++;
					hex.Remove( 0, 2 );
				}
			}

			return imageDataBinary;
		} // ToBinary

		// ----------------------------------------------------------------------
		protected override bool IsEqual( object obj )
		{
			RtfVisualImage compare = obj as RtfVisualImage; // guaranteed to be non-null
			return
				compare != null &&
				base.IsEqual( compare ) &&
				this.format == compare.format &&
				this.alignment == compare.alignment &&
				this.width == compare.width &&
				this.height == compare.height &&
				this.desiredWidth == compare.desiredWidth &&
				this.desiredHeight == compare.desiredHeight &&
				this.scaleWidthPercent == compare.scaleWidthPercent &&
				this.scaleHeightPercent == compare.scaleHeightPercent &&
				this.imageDataHex.Equals( compare.imageDataHex );
			//imageDataBinary.Equals( compare.imageDataBinary ); // cached info only
		} // IsEqual

		// ----------------------------------------------------------------------
		protected override int ComputeHashCode()
		{
			int hash = base.ComputeHashCode();
			hash = HashTool.AddHashCode( hash, this.format );
			hash = HashTool.AddHashCode( hash, this.alignment );
			hash = HashTool.AddHashCode( hash, this.width );
			hash = HashTool.AddHashCode( hash, this.height );
			hash = HashTool.AddHashCode( hash, this.desiredWidth );
			hash = HashTool.AddHashCode( hash, this.desiredHeight );
			hash = HashTool.AddHashCode( hash, this.scaleWidthPercent );
			hash = HashTool.AddHashCode( hash, this.scaleHeightPercent );
			hash = HashTool.AddHashCode( hash, this.imageDataHex );
			//hash = HashTool.AddHashCode( hash, this.imageDataBinary ); // cached info only
			return hash;
		} // ComputeHashCode

		// ----------------------------------------------------------------------
		public override string ToString()
		{
			return "[" + this.format + ": " + this.alignment + ", " +
				this.width + " x " + this.height + " " +
				"(" + this.desiredWidth + " x " + this.desiredHeight + ") " +
				"{" + this.scaleWidthPercent + "% x " + this.scaleHeightPercent + "%} " +
				":" + (this.imageDataHex.Length / 2) + " bytes]";
		} // ToString

		// ----------------------------------------------------------------------
		// members
		private readonly RtfVisualImageFormat format;
		private readonly RtfTextAlignment alignment;
		private readonly int width;
		private readonly int height;
		private readonly int desiredWidth;
		private readonly int desiredHeight;
		private readonly int scaleWidthPercent;
		private readonly int scaleHeightPercent;
		private readonly string imageDataHex;
		private byte[] imageDataBinary; // cached info only

	} // class RtfVisualImage

} // namespace Itenso.Rtf.Model
// -- EOF -------------------------------------------------------------------
