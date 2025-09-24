// -- FILE ------------------------------------------------------------------
// name       : RtfTextConvertSettings.cs
// project    : RTF Framelet
// created    : Jani Giannoudis - 2008.05.29
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;

namespace Itenso.Rtf.Converter.Text
{

	// ------------------------------------------------------------------------
	public class RtfTextConvertSettings
	{

		// ----------------------------------------------------------------------
		public const string SeparatorCr = "\r";
		public const string SeparatorLf = "\n";
		public const string SeparatorCrLf = "\r\n";
		public const string SeparatorLfCr = "\n\r";

		// ----------------------------------------------------------------------
		public RtfTextConvertSettings()
			: this( SeparatorCrLf )
		{
		} // RtfTextConvertSettings

		// ----------------------------------------------------------------------
		public RtfTextConvertSettings( string breakText )
		{
			SetBreakText( breakText );
		} // RtfTextConvertSettings

		// ----------------------------------------------------------------------
		public bool IsShowHiddenText
		{
			get { return this.showHiddenText; }
			set { this.showHiddenText = value; }
		} // IsShowHiddenText

		// ----------------------------------------------------------------------
		public string TabulatorText
		{
			get { return this.tabulatorText; }
			set { this.tabulatorText = value; }
		} // TabulatorText

		// ----------------------------------------------------------------------
		public string NonBreakingSpaceText
		{
			get { return this.nonBreakingSpaceText; }
			set { this.nonBreakingSpaceText = value; }
		} // NonBreakingSpaceText

		// ----------------------------------------------------------------------
		public string EmSpaceText
		{
			get { return this.emSpaceText; }
			set { this.emSpaceText = value; }
		} // EmSpaceText

		// ----------------------------------------------------------------------
		public string EnSpaceText
		{
			get { return this.enSpaceText; }
			set { this.enSpaceText = value; }
		} // EnSpaceText

		// ----------------------------------------------------------------------
		public string QmSpaceText
		{
			get { return this.qmSpaceText; }
			set { this.qmSpaceText = value; }
		} // QmSpaceText

		// ----------------------------------------------------------------------
		public string EmDashText
		{
			get { return this.emDashText; }
			set { this.emDashText = value; }
		} // EmDashText

		// ----------------------------------------------------------------------
		public string EnDashText
		{
			get { return this.enDashText; }
			set { this.enDashText = value; }
		} // EnDashText

		// ----------------------------------------------------------------------
		public string OptionalHyphenText
		{
			get { return this.optionalHyphenText; }
			set { this.optionalHyphenText = value; }
		} // OptionalHyphenText

		// ----------------------------------------------------------------------
		public string NonBreakingHyphenText
		{
			get { return this.nonBreakingHyphenText; }
			set { this.nonBreakingHyphenText = value; }
		} // NonBreakingHyphenText

		// ----------------------------------------------------------------------
		public string BulletText
		{
			get { return this.bulletText; }
			set { this.bulletText = value; }
		} // BulletText

		// ----------------------------------------------------------------------
		public string LeftSingleQuoteText
		{
			get { return this.leftSingleQuoteText; }
			set { this.leftSingleQuoteText = value; }
		} // LeftSingleQuoteText

		// ----------------------------------------------------------------------
		public string RightSingleQuoteText
		{
			get { return this.rightSingleQuoteText; }
			set { this.rightSingleQuoteText = value; }
		} // RightSingleQuoteText

		// ----------------------------------------------------------------------
		public string LeftDoubleQuoteText
		{
			get { return this.leftDoubleQuoteText; }
			set { this.leftDoubleQuoteText = value; }
		} // LeftDoubleQuoteText

		// ----------------------------------------------------------------------
		public string RightDoubleQuoteText
		{
			get { return this.rightDoubleQuoteText; }
			set { this.rightDoubleQuoteText = value; }
		} // RightDoubleQuoteText

		// ----------------------------------------------------------------------
		public string UnknownSpecialCharText
		{
			get { return this.unknownSpecialCharText; }
			set { this.unknownSpecialCharText = value; }
		} // UnknownSpecialCharText

		// ----------------------------------------------------------------------
		public string LineBreakText
		{
			get { return this.lineBreakText; }
			set { this.lineBreakText = value; }
		} // LineBreakText

		// ----------------------------------------------------------------------
		public string PageBreakText
		{
			get { return this.pageBreakText; }
			set { this.pageBreakText = value; }
		} // PageBreakText

		// ----------------------------------------------------------------------
		public string ParagraphBreakText
		{
			get { return this.paragraphBreakText; }
			set { this.paragraphBreakText = value; }
		} // ParagraphBreakText

		// ----------------------------------------------------------------------
		public string SectionBreakText
		{
			get { return this.sectionBreakText; }
			set { this.sectionBreakText = value; }
		} // SectionBreakText

		// ----------------------------------------------------------------------
		public string UnknownBreakText
		{
			get { return this.unknownBreakText; }
			set { this.unknownBreakText = value; }
		} // UnknownBreakText

		// ----------------------------------------------------------------------
		public string ImageFormatText
		{
			get { return this.imageFormatText; }
			set { this.imageFormatText = value; }
		} // ImageFormatText

		// ----------------------------------------------------------------------
		public void SetBreakText( string breakText )
		{
			if ( breakText == null )
			{
				throw new ArgumentNullException( "breakText" );
			}

			this.lineBreakText = breakText;
			this.pageBreakText = breakText + breakText;
			this.paragraphBreakText = breakText;
			this.sectionBreakText = breakText + breakText;
			this.unknownBreakText = breakText;
		} // SetBreakText

		// ----------------------------------------------------------------------
		// members: hidden text
		private bool showHiddenText;

		// members: special chars
		private string tabulatorText = "\t";
		private string nonBreakingSpaceText = " ";
		private string emSpaceText = " ";
		private string enSpaceText = " ";
		private string qmSpaceText = " ";
		private string emDashText = "-";
		private string enDashText = "-";
		private string optionalHyphenText = "-";
		private string nonBreakingHyphenText = "-";
		private string bulletText = "°";
		private string leftSingleQuoteText = "`";
		private string rightSingleQuoteText = "´";
		private string leftDoubleQuoteText = "``";
		private string rightDoubleQuoteText = "´´";
		private string unknownSpecialCharText;

		// members: breaks
		private string lineBreakText;
		private string pageBreakText;
		private string paragraphBreakText;
		private string sectionBreakText;
		private string unknownBreakText;

		// members: image
		private string imageFormatText = Strings.ImageFormatText;

	} // class RtfTextConvertSettings

} // namespace Itenso.Rtf.Converter.Text
// -- EOF -------------------------------------------------------------------
