// -- FILE ------------------------------------------------------------------
// name       : RtfImageBuilder.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.23
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using Itenso.Rtf.Support;

namespace Itenso.Rtf.Interpreter
{

	// ------------------------------------------------------------------------
	public sealed class RtfImageBuilder : RtfElementVisitorBase
	{

		// ----------------------------------------------------------------------
		public RtfImageBuilder()
			: base( RtfElementVisitorOrder.DepthFirst )
		{
			Reset();
		} // RtfImageBuilder

		// ----------------------------------------------------------------------
		public void Reset()
		{
			this.format = RtfVisualImageFormat.Bmp;
			this.width = 0;
			this.height = 0;
			this.desiredWidth = 0;
			this.desiredHeight = 0;
			this.scaleWidthPercent = 100;
			this.scaleHeightPercent = 100;
			this.imageDataHex = null;
		} // Reset

		// ----------------------------------------------------------------------
		public RtfVisualImageFormat Format
		{
			get { return this.format; }
		} // Format

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
		protected override void DoVisitGroup( IRtfGroup group )
		{
			switch ( group.Destination )
			{
				case RtfSpec.TagPicture:
					Reset();
					VisitGroupChildren( group );
					break;
			}
		} // DoVisitGroup

		// ----------------------------------------------------------------------
		protected override void DoVisitTag( IRtfTag tag )
		{
			switch ( tag.Name )
			{
				case RtfSpec.TagPictureFormatWinDib:
				case RtfSpec.TagPictureFormatWinBmp:
					this.format = RtfVisualImageFormat.Bmp;
					break;
				case RtfSpec.TagPictureFormatEmf:
					this.format = RtfVisualImageFormat.Emf;
					break;
				case RtfSpec.TagPictureFormatJpg:
					this.format = RtfVisualImageFormat.Jpg;
					break;
				case RtfSpec.TagPictureFormatPng:
					this.format = RtfVisualImageFormat.Png;
					break;
				case RtfSpec.TagPictureFormatWmf:
					this.format = RtfVisualImageFormat.Wmf;
					break;
				case RtfSpec.TagPictureWidth:
					this.width = tag.ValueAsNumber;
					this.desiredWidth = width;
					break;
				case RtfSpec.TagPictureHeight:
					this.height = tag.ValueAsNumber;
					this.desiredHeight = height;
					break;
				case RtfSpec.TagPictureWidthGoal:
					this.desiredWidth = tag.ValueAsNumber;
					if ( this.width == 0 )
					{
						// hack to prevent WordPad documents which lack the \picw and \pich tags
						// from resulting in an exception due to undefined width/height
						this.width = this.desiredWidth;
					}
					break;
				case RtfSpec.TagPictureHeightGoal:
					this.desiredHeight = tag.ValueAsNumber;
					if ( this.height == 0 )
					{
						// hack to prevent WordPad documents which lack the \picw and \pich tags
						// from resulting in an exception due to undefined width/height
						this.height = this.desiredHeight;
					}
					break;
				case RtfSpec.TagPictureWidthScale:
					this.scaleWidthPercent = tag.ValueAsNumber;
					break;
				case RtfSpec.TagPictureHeightScale:
					this.scaleHeightPercent = tag.ValueAsNumber;
					break;
			}
		} // DoVisitTag

		// ----------------------------------------------------------------------
		protected override void DoVisitText( IRtfText text )
		{
			this.imageDataHex = text.Text;
		} // DoVisitText

		// ----------------------------------------------------------------------
		// members
		private RtfVisualImageFormat format;
		private int width;
		private int height;
		private int desiredWidth;
		private int desiredHeight;
		private int scaleWidthPercent;
		private int scaleHeightPercent;
		private string imageDataHex;

	} // class RtfImageBuilder

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
