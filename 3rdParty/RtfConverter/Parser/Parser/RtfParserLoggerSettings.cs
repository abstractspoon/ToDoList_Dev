// -- FILE ------------------------------------------------------------------
// name       : RtfParserLoggerSettings.cs
// project    : RTF Framelet
// created    : Jani Giannoudis - 2008.05.30
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------

namespace Itenso.Rtf.Parser
{

	// ------------------------------------------------------------------------
	public class RtfParserLoggerSettings
	{

		// ----------------------------------------------------------------------
		public RtfParserLoggerSettings()
			: this( true )
		{
		} // RtfParserLoggerSettings

		// ----------------------------------------------------------------------
		public RtfParserLoggerSettings( bool enabled )
		{
			this.enabled = enabled;
		} // RtfParserLoggerSettings

		// ----------------------------------------------------------------------
		public bool Enabled
		{
			get { return this.enabled; }
			set { this.enabled = value; }
		} // Enabled

		// ----------------------------------------------------------------------
		public string ParseBeginText
		{
			get { return this.parseBeginText; }
			set { this.parseBeginText = value; }
		} // ParseBeginText

		// ----------------------------------------------------------------------
		public string ParseEndText
		{
			get { return this.parseEndText; }
			set { this.parseEndText = value; }
		} // ParseEndText

		// ----------------------------------------------------------------------
		public string ParseGroupBeginText
		{
			get { return this.parseGroupBeginText; }
			set { this.parseGroupBeginText = value; }
		} // ParseGroupBeginText

		// ----------------------------------------------------------------------
		public string ParseGroupEndText
		{
			get { return this.parseGroupEndText; }
			set { this.parseGroupEndText = value; }
		} // ParseGroupEndText

		// ----------------------------------------------------------------------
		public string ParseTagText
		{
			get { return this.parseTagText; }
			set { this.parseTagText = value; }
		} // ParseTagText

		// ----------------------------------------------------------------------
		public string TextOverflowText
		{
			get { return this.textOverflowText; }
			set { this.textOverflowText = value; }
		} // TextOverflowText

		// ----------------------------------------------------------------------
		public string ParseTextText
		{
			get { return this.parseTextText; }
			set { this.parseTextText = value; }
		} // ParseTextText

		// ----------------------------------------------------------------------
		public string ParseSuccessText
		{
			get { return this.parseSuccessText; }
			set { this.parseSuccessText = value; }
		} // ParseSuccessText

		// ----------------------------------------------------------------------
		public string ParseFailKnownReasonText
		{
			get { return this.parseFailKnownReasonText; }
			set { this.parseFailKnownReasonText = value; }
		} // ParseFailKnownReasonText

		// ----------------------------------------------------------------------
		public string ParseFailUnknownReasonText
		{
			get { return this.parseFailUnknownReasonText; }
			set { this.parseFailUnknownReasonText = value; }
		} // ParseFailUnknownReasonText

		// ----------------------------------------------------------------------
		public int TextMaxLength
		{
			get { return this.textMaxLength; }
			set { this.textMaxLength = value; }
		} // TextMaxLength

		// ----------------------------------------------------------------------
		// members
		private bool enabled;
		private string parseBeginText = Strings.LogParseBegin;
		private string parseEndText = Strings.LogParseEnd;
		private string parseGroupBeginText = Strings.LogGroupBegin;
		private string parseGroupEndText = Strings.LogGroupEnd;
		private string parseTagText = Strings.LogTag;
		private string parseTextText = Strings.LogText;
		private string textOverflowText = Strings.LogOverflowText;
		private string parseSuccessText = Strings.LogParseSuccess;
		private string parseFailKnownReasonText = Strings.LogParseFail;
		private string parseFailUnknownReasonText = Strings.LogParseFailUnknown;

		private int textMaxLength = 80;

	} // class RtfParserLoggerSettings

} // namespace Itenso.Rtf.Parser
// -- EOF -------------------------------------------------------------------
