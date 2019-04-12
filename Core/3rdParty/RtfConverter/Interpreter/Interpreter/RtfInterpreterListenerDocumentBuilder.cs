// -- FILE ------------------------------------------------------------------
// name       : RtfInterpreterListenerDocumentBuilder.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.21
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System.Text;
using Itenso.Rtf.Model;

namespace Itenso.Rtf.Interpreter
{

	// ------------------------------------------------------------------------
	public sealed class RtfInterpreterListenerDocumentBuilder : RtfInterpreterListenerBase
	{

		// ----------------------------------------------------------------------
		public RtfInterpreterListenerDocumentBuilder()
		{
		} // RtfInterpreterListenerDocumentBuilder

		// ----------------------------------------------------------------------
		public bool CombineTextWithSameFormat
		{
			get { return this.combineTextWithSameFormat; }
			set { this.combineTextWithSameFormat = value; }
		} // CombineTextWithSameFormat

		// ----------------------------------------------------------------------
		public IRtfDocument Document
		{
			get { return this.document; }
		} // Document

		// ----------------------------------------------------------------------
		protected override void DoBeginDocument( IRtfInterpreterContext context )
		{
			this.document = null;
			this.visualContent = new RtfVisualCollection();
		} // DoBeginDocument

		// ----------------------------------------------------------------------
		protected override void DoInsertText( IRtfInterpreterContext context, string text )
		{
			if ( this.combineTextWithSameFormat )
			{
				IRtfTextFormat newFormat = context.GetSafeCurrentTextFormat();
				if ( !newFormat.Equals( this.pendingTextFormat ) )
				{
					FlushPendingText();
				}
				this.pendingTextFormat = newFormat;
				this.pendingText.Append( text );
			}
			else
			{
				this.visualContent.Add( new RtfVisualText( text, context.GetSafeCurrentTextFormat() ) );
			}
		} // DoInsertText

		// ----------------------------------------------------------------------
		protected override void DoInsertSpecialChar( IRtfInterpreterContext context, RtfVisualSpecialCharKind kind )
		{
			FlushPendingText();
			this.visualContent.Add( new RtfVisualSpecialChar( kind ) );
		} // DoInsertSpecialChar

		// ----------------------------------------------------------------------
		protected override void DoInsertBreak( IRtfInterpreterContext context, RtfVisualBreakKind kind )
		{
			FlushPendingText();
			this.visualContent.Add( new RtfVisualBreak( kind ) );
		} // DoInsertBreak

		// ----------------------------------------------------------------------
		protected override void DoInsertImage( IRtfInterpreterContext context,
			RtfVisualImageFormat format,
			int width, int height, int desiredWidth, int desiredHeight,
			int scaleWidthPercent, int scaleHeightPercent,
			string imageDataHex
		)
		{
			FlushPendingText();
			this.visualContent.Add( new RtfVisualImage( format,
				context.GetSafeCurrentTextFormat().Alignment,
				width, height, desiredWidth, desiredHeight,
				scaleWidthPercent, scaleHeightPercent, imageDataHex ) );
		} // DoInsertImage

		// ----------------------------------------------------------------------
		protected override void DoEndDocument( IRtfInterpreterContext context )
		{
			FlushPendingText();
			this.document = new RtfDocument( context, this.visualContent );
			this.visualContent = null;
		} // DoEndDocument

		// ----------------------------------------------------------------------
		private void FlushPendingText()
		{
			if ( this.pendingTextFormat != null )
			{
				this.visualContent.Add( new RtfVisualText( this.pendingText.ToString(), this.pendingTextFormat ) );
				this.pendingTextFormat = null;
				this.pendingText.Remove( 0, this.pendingText.Length );
			}
		} // FlushPendingText

		// ----------------------------------------------------------------------
		// members
		private bool combineTextWithSameFormat = true;

		private RtfDocument document;
		private RtfVisualCollection visualContent;

		private IRtfTextFormat pendingTextFormat;
		private readonly StringBuilder pendingText = new StringBuilder();

	} // class RtfInterpreterListenerDocumentBuilder

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
