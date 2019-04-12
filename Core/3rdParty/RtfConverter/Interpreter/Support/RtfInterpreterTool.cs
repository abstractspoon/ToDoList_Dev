// -- FILE ------------------------------------------------------------------
// name       : RtfInterpreterTool.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.21
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System.IO;
using Itenso.Rtf.Interpreter;

namespace Itenso.Rtf.Support
{

	// ------------------------------------------------------------------------
	public static class RtfInterpreterTool
	{

		// ----------------------------------------------------------------------
		public static IRtfDocument BuildDoc( string rtfText, params IRtfInterpreterListener[] listeners )
		{
			return BuildDoc( RtfParserTool.Parse( rtfText ), listeners );
		} // BuildDoc

		// ----------------------------------------------------------------------
		public static IRtfDocument BuildDoc( TextReader rtfTextSource, params IRtfInterpreterListener[] listeners )
		{
			return BuildDoc( RtfParserTool.Parse( rtfTextSource ), listeners );
		} // BuildDoc

		// ----------------------------------------------------------------------
		public static IRtfDocument BuildDoc( Stream rtfTextSource, params IRtfInterpreterListener[] listeners )
		{
			return BuildDoc( RtfParserTool.Parse( rtfTextSource ), listeners );
		} // BuildDoc

		// ----------------------------------------------------------------------
		public static IRtfDocument BuildDoc( IRtfSource rtfTextSource, params IRtfInterpreterListener[] listeners )
		{
			return BuildDoc( RtfParserTool.Parse( rtfTextSource ), listeners );
		} // BuildDoc

		// ----------------------------------------------------------------------
		public static IRtfDocument BuildDoc( IRtfGroup rtfDocument, params IRtfInterpreterListener[] listeners )
		{
			RtfInterpreterListenerDocumentBuilder docBuilder = new RtfInterpreterListenerDocumentBuilder();
			IRtfInterpreterListener[] allListeners;
			if ( listeners == null )
			{
				allListeners = new IRtfInterpreterListener[] { docBuilder };
			}
			else
			{
				allListeners = new IRtfInterpreterListener[ listeners.Length + 1 ];
				allListeners[ 0 ] = docBuilder;
				listeners.CopyTo( allListeners, 1 );
			}
			Interpret( rtfDocument, allListeners );
			return docBuilder.Document;
		} // BuildDoc

		// ----------------------------------------------------------------------
		public static void Interpret( string rtfText, params IRtfInterpreterListener[] listeners )
		{
			Interpret( RtfParserTool.Parse( rtfText ), listeners );
		} // Interpret

		// ----------------------------------------------------------------------
		public static void Interpret( TextReader rtfTextSource, params IRtfInterpreterListener[] listeners )
		{
			Interpret( RtfParserTool.Parse( rtfTextSource ), listeners );
		} // Interpret

		// ----------------------------------------------------------------------
		public static void Interpret( Stream rtfTextSource, params IRtfInterpreterListener[] listeners )
		{
			Interpret( RtfParserTool.Parse( rtfTextSource ), listeners );
		} // Interpret

		// ----------------------------------------------------------------------
		public static void Interpret( IRtfSource rtfTextSource, params IRtfInterpreterListener[] listeners )
		{
			Interpret( RtfParserTool.Parse( rtfTextSource ), listeners );
		} // Interpret

		// ----------------------------------------------------------------------
		public static void Interpret( IRtfGroup rtfDocument, params IRtfInterpreterListener[] listeners )
		{
			RtfInterpreter parser = new RtfInterpreter();
			if ( listeners != null )
			{
				foreach ( IRtfInterpreterListener listener in listeners )
				{
					if ( listener != null )
					{
						parser.AddInterpreterListener( listener );
					}
				}
			}
			parser.Interpret( rtfDocument );
		} // Interpret

	} // class RtfInterpreterTool

} // namespace Itenso.Rtf.Support
// -- EOF -------------------------------------------------------------------
