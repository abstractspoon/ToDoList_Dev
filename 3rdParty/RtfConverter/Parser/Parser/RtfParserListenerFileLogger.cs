// -- FILE ------------------------------------------------------------------
// name       : RtfParserListenerFileLogger.cs
// project    : RTF Framelet
// created    : Jani Giannoudis - 2008.06.03
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.IO;
using System.Text;
using System.Globalization;

namespace Itenso.Rtf.Parser
{

	// ------------------------------------------------------------------------
	public class RtfParserListenerFileLogger : RtfParserListenerBase, IDisposable
	{

		// ----------------------------------------------------------------------
		public const string DefaultLogFileExtension = ".parser.log";

		// ----------------------------------------------------------------------
		public RtfParserListenerFileLogger( string fileName )
			: this( fileName, new RtfParserLoggerSettings() )
		{
		} // RtfParserListenerFileLogger

		// ----------------------------------------------------------------------
		public RtfParserListenerFileLogger( string fileName, RtfParserLoggerSettings settings )
		{
			if ( fileName == null )
			{
				throw new ArgumentNullException( "fileName" );
			}
			if ( settings == null )
			{
				throw new ArgumentNullException( "settings" );
			}

			this.fileName = fileName;
			this.settings = settings;
		} // RtfParserListenerFileLogger

		// ----------------------------------------------------------------------
		public string FileName
		{
			get { return this.fileName; }
		} // FileName

		// ----------------------------------------------------------------------
		public RtfParserLoggerSettings Settings
		{
			get { return this.settings; }
		} // Settings

		// ----------------------------------------------------------------------
		public virtual void Dispose()
		{
			CloseStream();
		} // Dispose

		// ----------------------------------------------------------------------
		protected override void DoParseBegin()
		{
			EnsureDirectory();
			OpenStream();

			if ( this.settings.Enabled && !string.IsNullOrEmpty( this.settings.ParseBeginText ) )
			{
				WriteLine( this.settings.ParseBeginText );
			}
		} // DoParseBegin

		// ----------------------------------------------------------------------
		protected override void DoGroupBegin()
		{
			if ( this.settings.Enabled && !string.IsNullOrEmpty( this.settings.ParseGroupBeginText ) )
			{
				WriteLine( this.settings.ParseGroupBeginText );
			}
		} // DoGroupBegin

		// ----------------------------------------------------------------------
		protected override void DoTagFound( IRtfTag tag )
		{
			if ( this.settings.Enabled && !string.IsNullOrEmpty( this.settings.ParseTagText ) )
			{
				WriteLine( string.Format(
					CultureInfo.InvariantCulture,
					this.settings.ParseTagText,
					tag ) );
			}
		} // DoTagFound

		// ----------------------------------------------------------------------
		protected override void DoTextFound( IRtfText text )
		{
			if ( this.settings.Enabled && !string.IsNullOrEmpty( this.settings.ParseTextText ) )
			{
				string msg = text.Text;
				if ( msg.Length > this.settings.TextMaxLength && !string.IsNullOrEmpty( this.settings.TextOverflowText ) )
				{
					msg = msg.Substring( 0, msg.Length - this.settings.TextOverflowText.Length ) + this.settings.TextOverflowText;
				}
				WriteLine( string.Format(
					CultureInfo.InvariantCulture,
					this.settings.ParseTextText,
					msg ) );
			}
		} // DoTextFound

		// ----------------------------------------------------------------------
		protected override void DoGroupEnd()
		{
			if ( this.settings.Enabled && !string.IsNullOrEmpty( this.settings.ParseGroupEndText ) )
			{
				WriteLine( this.settings.ParseGroupEndText );
			}
		} // DoGroupEnd

		// ----------------------------------------------------------------------
		protected override void DoParseSuccess()
		{
			if ( this.settings.Enabled && !string.IsNullOrEmpty( this.settings.ParseSuccessText ) )
			{
				WriteLine( this.settings.ParseSuccessText );
			}
		} // DoParseSuccess

		// ----------------------------------------------------------------------
		protected override void DoParseFail( RtfException reason )
		{
			if ( this.settings.Enabled )
			{
				if ( reason != null )
				{
					if ( !string.IsNullOrEmpty( this.settings.ParseFailKnownReasonText ) )
					{
						WriteLine( string.Format(
							CultureInfo.InvariantCulture,
							this.settings.ParseFailKnownReasonText,
							reason.Message ) );
					}
				}
				else
				{
					if ( !string.IsNullOrEmpty( this.settings.ParseFailUnknownReasonText ) )
					{
						WriteLine( this.settings.ParseFailUnknownReasonText );
					}
				}
			}
		} // DoParseFail

		// ----------------------------------------------------------------------
		protected override void DoParseEnd()
		{
			if ( this.settings.Enabled && !string.IsNullOrEmpty( this.settings.ParseEndText ) )
			{
				WriteLine( this.settings.ParseEndText );
			}

			CloseStream();
		} // DoParseEnd

		// ----------------------------------------------------------------------
		private void WriteLine( params string[] msg )
		{
			if ( this.streamWriter == null )
			{
				return;
			}
			string logText = Indent( msg );
			this.streamWriter.WriteLine( logText );
			this.streamWriter.Flush();
		} // WriteLine

		// ----------------------------------------------------------------------
		private string Indent( params string[] msg )
		{
			StringBuilder buf = new StringBuilder();
			if ( msg != null )
			{
				for ( int i = 0; i < Level; i++ )
				{
					buf.Append( " " );
				}
				foreach ( string m in msg )
				{
					buf.Append( m );
				}
			}
			return buf.ToString();
		} // Indent

		// ----------------------------------------------------------------------
		private void EnsureDirectory()
		{
			FileInfo fi = new FileInfo( this.fileName );
			if ( !string.IsNullOrEmpty( fi.DirectoryName ) && !Directory.Exists( fi.DirectoryName ) )
			{
				Directory.CreateDirectory( fi.DirectoryName );
			}
		} // EnsureDirectory

		// ----------------------------------------------------------------------
		private void OpenStream()
		{
			if ( this.streamWriter != null )
			{
				return;
			}
			this.streamWriter = new StreamWriter( this.fileName );
		} // OpenStream

		// ----------------------------------------------------------------------
		private void CloseStream()
		{
			if ( this.streamWriter == null )
			{
				return;
			}
			this.streamWriter.Close();
			this.streamWriter.Dispose();
			this.streamWriter = null;
		} // OpenStream

		// ----------------------------------------------------------------------
		// members
		private readonly string fileName;
		private readonly RtfParserLoggerSettings settings;
		private StreamWriter streamWriter;

	} // class RtfParserListenerFileLogger

} // namespace Itenso.Rtf.Parser
// -- EOF -------------------------------------------------------------------
