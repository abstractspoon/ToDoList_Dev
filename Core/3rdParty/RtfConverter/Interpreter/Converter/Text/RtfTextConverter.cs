// -- FILE ------------------------------------------------------------------
// name       : RtfTextConverter.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.21
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Text;
using System.Globalization;
using Itenso.Rtf.Interpreter;

namespace Itenso.Rtf.Converter.Text
{

	// ------------------------------------------------------------------------
	public class RtfTextConverter : RtfInterpreterListenerBase
	{

		// ----------------------------------------------------------------------
		public const string DefaultTextFileExtension = ".txt";

		// ----------------------------------------------------------------------
		public RtfTextConverter()
			: this( new RtfTextConvertSettings() )
		{
		} // RtfTextConverter

		// ----------------------------------------------------------------------
		public RtfTextConverter( RtfTextConvertSettings settings )
		{
			if ( settings == null )
			{
				throw new ArgumentNullException( "settings" );
			}

			this.settings = settings;
		} // RtfTextConverter

		// ----------------------------------------------------------------------
		public string PlainText
		{
			get { return this.plainText.ToString(); }
		} // PlainText

		// ----------------------------------------------------------------------
		public RtfTextConvertSettings Settings
		{
			get { return this.settings; }
		} // Settings

		// ----------------------------------------------------------------------
		public void Clear()
		{
			plainText.Remove( 0, this.plainText.Length );
		} // Clear

		// ----------------------------------------------------------------------
		protected override void DoBeginDocument( IRtfInterpreterContext context )
		{
			Clear();
		} // DoBeginDocument

		// ----------------------------------------------------------------------
		protected override void DoInsertText( IRtfInterpreterContext context, string text )
		{
			if ( !context.CurrentTextFormat.IsHidden || this.settings.IsShowHiddenText )
			{
				this.plainText.Append( text );
			}
		} // DoInsertText

		// ----------------------------------------------------------------------
		protected override void DoInsertSpecialChar( IRtfInterpreterContext context, RtfVisualSpecialCharKind kind )
		{
			switch ( kind )
			{
				case RtfVisualSpecialCharKind.Tabulator:
					this.plainText.Append( this.settings.TabulatorText );
					break;
				case RtfVisualSpecialCharKind.NonBreakingSpace:
					this.plainText.Append( this.settings.NonBreakingSpaceText );
					break;
				case RtfVisualSpecialCharKind.EmSpace:
					this.plainText.Append( this.settings.EmSpaceText );
					break;
				case RtfVisualSpecialCharKind.EnSpace:
					this.plainText.Append( this.settings.EnSpaceText );
					break;
				case RtfVisualSpecialCharKind.QmSpace:
					this.plainText.Append( this.settings.QmSpaceText );
					break;
				case RtfVisualSpecialCharKind.EmDash:
					this.plainText.Append( this.settings.EmDashText );
					break;
				case RtfVisualSpecialCharKind.EnDash:
					this.plainText.Append( this.settings.EnDashText );
					break;
				case RtfVisualSpecialCharKind.OptionalHyphen:
					this.plainText.Append( this.settings.OptionalHyphenText );
					break;
				case RtfVisualSpecialCharKind.NonBreakingHyphen:
					this.plainText.Append( this.settings.NonBreakingHyphenText );
					break;
				case RtfVisualSpecialCharKind.Bullet:
					this.plainText.Append( this.settings.BulletText );
					break;
				case RtfVisualSpecialCharKind.LeftSingleQuote:
					this.plainText.Append( this.settings.LeftSingleQuoteText );
					break;
				case RtfVisualSpecialCharKind.RightSingleQuote:
					this.plainText.Append( this.settings.RightSingleQuoteText );
					break;
				case RtfVisualSpecialCharKind.LeftDoubleQuote:
					this.plainText.Append( this.settings.LeftDoubleQuoteText );
					break;
				case RtfVisualSpecialCharKind.RightDoubleQuote:
					this.plainText.Append( this.settings.RightDoubleQuoteText );
					break;
				default:
					this.plainText.Append( this.settings.UnknownSpecialCharText );
					break;
			}
		} // DoInsertSpecialChar

		// ----------------------------------------------------------------------
		protected override void DoInsertBreak( IRtfInterpreterContext context, RtfVisualBreakKind kind )
		{
			switch ( kind )
			{
				case RtfVisualBreakKind.Line:
					this.plainText.Append( this.settings.LineBreakText );
					break;
				case RtfVisualBreakKind.Page:
					this.plainText.Append( this.settings.PageBreakText );
					break;
				case RtfVisualBreakKind.Paragraph:
					this.plainText.Append( this.settings.ParagraphBreakText );
					break;
				case RtfVisualBreakKind.Section:
					this.plainText.Append( this.settings.SectionBreakText );
					break;
				default:
					this.plainText.Append( this.settings.UnknownBreakText );
					break;
			}
		} // DoInsertBreak

		// ----------------------------------------------------------------------
		protected override void DoInsertImage( IRtfInterpreterContext context,
			RtfVisualImageFormat format,
			int width, int height, int desiredWidth, int desiredHeight,
			int scaleWidthPercent, int scaleHeightPercent,
			string imageDataHex
		)
		{
			string imageFormatText = this.settings.ImageFormatText;
			if ( string.IsNullOrEmpty( imageFormatText ) )
			{
				return;
			}

			string imageText = string.Format(
				CultureInfo.InvariantCulture,
				imageFormatText,
				format,
				width,
				height,
				desiredWidth,
				desiredHeight,
				scaleWidthPercent,
				scaleHeightPercent,
				imageDataHex );

			this.plainText.Append( imageText );
		} // DoInsertImage

		// ----------------------------------------------------------------------
		// members
		private readonly StringBuilder plainText = new StringBuilder();
		private readonly RtfTextConvertSettings settings;

	} // class RtfTextConverter

} // namespace Itenso.Rtf.Converter.Text
// -- EOF -------------------------------------------------------------------
