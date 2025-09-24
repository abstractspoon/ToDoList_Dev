// -- FILE ------------------------------------------------------------------
// name       : RtfParserListenerLogger.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.19
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Text;
using System.Globalization;
using Itenso.Sys.Logging;

namespace Itenso.Rtf.Parser
{

	// ------------------------------------------------------------------------
	public class RtfParserListenerLogger : RtfParserListenerBase
	{

		// ----------------------------------------------------------------------
		public RtfParserListenerLogger()
			: this( new RtfParserLoggerSettings(), systemLogger )
		{
		} // RtfParserListenerLogger

		// ----------------------------------------------------------------------
		public RtfParserListenerLogger( RtfParserLoggerSettings settings )
			: this( settings, systemLogger )
		{
		} // RtfParserListenerLogger

		// ----------------------------------------------------------------------
		public RtfParserListenerLogger( ILogger logger )
			: this( new RtfParserLoggerSettings(), logger )
		{
		} // RtfParserListenerLogger

		// ----------------------------------------------------------------------
		public RtfParserListenerLogger( RtfParserLoggerSettings settings, ILogger logger )
		{
			if ( settings == null )
			{
				throw new ArgumentNullException( "settings" );
			}
			if ( logger == null )
			{
				throw new ArgumentNullException( "logger" );
			}

			this.settings = settings;
			this.logger = logger;
		} // RtfParserListenerLogger

		// ----------------------------------------------------------------------
		public RtfParserLoggerSettings Settings
		{
			get { return this.settings; }
		} // Settings

		// ----------------------------------------------------------------------
		public ILogger Logger
		{
			get { return this.logger; }
		} // Logger

		// ----------------------------------------------------------------------
		protected override void DoParseBegin()
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.ParseBeginText ) )
			{
				Log( this.settings.ParseBeginText );
			}
		} // DoParseBegin

		// ----------------------------------------------------------------------
		protected override void DoGroupBegin()
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.ParseGroupBeginText ) )
			{
				Log( this.settings.ParseGroupBeginText );
			}
		} // DoGroupBegin

		// ----------------------------------------------------------------------
		protected override void DoTagFound( IRtfTag tag )
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.ParseTagText ) )
			{
				Log( string.Format(
					CultureInfo.InvariantCulture,
					this.settings.ParseTagText,
					tag ) );
			}
		} // DoTagFound

		// ----------------------------------------------------------------------
		protected override void DoTextFound( IRtfText text )
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.ParseTextText ) )
			{
				string msg = text.Text;
				if ( msg.Length > this.settings.TextMaxLength && !string.IsNullOrEmpty( this.settings.TextOverflowText ) )
				{
					msg = msg.Substring( 0, msg.Length - this.settings.TextOverflowText.Length ) + this.settings.TextOverflowText;
				}
				Log( string.Format(
					CultureInfo.InvariantCulture,
					this.settings.ParseTextText,
					msg ) );
			}
		} // DoTextFound

		// ----------------------------------------------------------------------
		protected override void DoGroupEnd()
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.ParseGroupEndText ) )
			{
				Log( this.settings.ParseGroupEndText );
			}
		} // DoGroupEnd

		// ----------------------------------------------------------------------
		protected override void DoParseSuccess()
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.ParseSuccessText ) )
			{
				Log( this.settings.ParseSuccessText );
			}
		} // DoParseSuccess

		// ----------------------------------------------------------------------
		protected override void DoParseFail( RtfException reason )
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled )
			{
				if ( reason != null )
				{
					if ( !string.IsNullOrEmpty( this.settings.ParseFailKnownReasonText ) )
					{
						Log( string.Format(
							CultureInfo.InvariantCulture,
							this.settings.ParseFailKnownReasonText,
							reason.Message ) );
					}
				}
				else
				{
					if ( !string.IsNullOrEmpty( this.settings.ParseFailUnknownReasonText ) )
					{
						Log( this.settings.ParseFailUnknownReasonText );
					}
				}
			}
		} // DoParseFail

		// ----------------------------------------------------------------------
		protected override void DoParseEnd()
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.ParseEndText ) )
			{
				Log( this.settings.ParseEndText );
			}
		} // DoParseEnd

		// ----------------------------------------------------------------------
		private void Log( params string[] msg )
		{
			string logText = Indent( msg );

			systemLogger.Info( logText );

			if ( this.logger != null )
			{
				this.logger.Info( logText );
			}
		} // Log

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
		// members
		private readonly RtfParserLoggerSettings settings;
		private readonly ILogger logger;

		private static readonly ILogger systemLogger = Sys.Logging.Logger.GetLogger( typeof( RtfParserListenerLogger ) );

	} // class RtfParserListenerLogger

} // namespace Itenso.Rtf.Parser
// -- EOF -------------------------------------------------------------------
