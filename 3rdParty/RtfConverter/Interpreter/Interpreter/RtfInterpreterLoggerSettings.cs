// -- FILE ------------------------------------------------------------------
// name       : RtfInterpreterLoggerSettings.cs
// project    : RTF Framelet
// created    : Jani Giannoudis - 2008.05.29
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------

namespace Itenso.Rtf.Interpreter
{

	// ------------------------------------------------------------------------
	public class RtfInterpreterLoggerSettings
	{

		// ----------------------------------------------------------------------
		public RtfInterpreterLoggerSettings()
			: this( true )
		{
		} // RtfInterpreterLoggerSettings

		// ----------------------------------------------------------------------
		public RtfInterpreterLoggerSettings( bool enabled )
		{
			this.enabled = enabled;
		} // RtfInterpreterLoggerSettings

		// ----------------------------------------------------------------------
		public bool Enabled
		{
			get { return this.enabled; }
			set { this.enabled = value; }
		} // Enabled

		// ----------------------------------------------------------------------
		public string BeginDocumentText
		{
			get { return this.beginDocumentText; }
			set { this.beginDocumentText = value; }
		} // BeginDocumentText

		// ----------------------------------------------------------------------
		public string EndDocumentText
		{
			get { return this.endDocumentText; }
			set { this.endDocumentText = value; }
		} // EndDocumentText

		// ----------------------------------------------------------------------
		public string TextFormatText
		{
			get { return this.textFormatText; }
			set { this.textFormatText = value; }
		} // TextFormatText

		// ----------------------------------------------------------------------
		public string TextOverflowText
		{
			get { return this.textOverflowText; }
			set { this.textOverflowText = value; }
		} // TextOverflowText

		// ----------------------------------------------------------------------
		public string SpecialCharFormatText
		{
			get { return this.specialCharFormatText; }
			set { this.specialCharFormatText = value; }
		} // SpecialCharFormatText

		// ----------------------------------------------------------------------
		public string BreakFormatText
		{
			get { return this.breakFormatText; }
			set { this.breakFormatText = value; }
		} // BreakFormatText

		// ----------------------------------------------------------------------
		public string ImageFormatText
		{
			get { return this.imageFormatText; }
			set { this.imageFormatText = value; }
		} // ImageFormatText

		// ----------------------------------------------------------------------
		public int TextMaxLength
		{
			get { return this.textMaxLength; }
			set { this.textMaxLength = value; }
		} // TextMaxLength

		// ----------------------------------------------------------------------
		// members
		private bool enabled;
		private string beginDocumentText = Strings.LogBeginDocument;
		private string endDocumentText = Strings.LogEndDocument;
		private string textFormatText = Strings.LogInsertText;
		private string textOverflowText = Strings.LogOverflowText;
		private string specialCharFormatText = Strings.LogInsertChar;
		private string breakFormatText = Strings.LogInsertBreak;
		private string imageFormatText = Strings.LogInsertImage;

		private int textMaxLength = 80;

	} // class RtfInterpreterLoggerSettings

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
