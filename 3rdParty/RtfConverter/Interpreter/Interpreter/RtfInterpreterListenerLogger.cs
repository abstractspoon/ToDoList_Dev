// -- FILE ------------------------------------------------------------------
// name       : RtfInterpreterListenerLogger.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.21
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Globalization;
using Itenso.Sys.Logging;

namespace Itenso.Rtf.Interpreter
{

	// ------------------------------------------------------------------------
	public class RtfInterpreterListenerLogger : RtfInterpreterListenerBase
	{

		// ----------------------------------------------------------------------
		public RtfInterpreterListenerLogger()
			: this( new RtfInterpreterLoggerSettings(), systemLogger )
		{
		} // RtfInterpreterListenerLogger

		// ----------------------------------------------------------------------
		public RtfInterpreterListenerLogger( RtfInterpreterLoggerSettings settings )
			: this( settings, systemLogger )
		{
		} // RtfInterpreterListenerLogger

		// ----------------------------------------------------------------------
		public RtfInterpreterListenerLogger( ILogger logger )
			: this( new RtfInterpreterLoggerSettings(), logger )
		{
		} // RtfInterpreterListenerLogger

		// ----------------------------------------------------------------------
		public RtfInterpreterListenerLogger( RtfInterpreterLoggerSettings settings, ILogger logger )
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
		} // RtfInterpreterListenerLogger

		// ----------------------------------------------------------------------
		public RtfInterpreterLoggerSettings Settings
		{
			get { return this.settings; }
		} // Settings

		// ----------------------------------------------------------------------
		public ILogger Logger
		{
			get { return this.logger; }
		} // Logger

		// ----------------------------------------------------------------------
		protected override void DoBeginDocument( IRtfInterpreterContext context )
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.BeginDocumentText ) )
			{
				Log( this.settings.BeginDocumentText );
			}
		} // DoBeginDocument

		// ----------------------------------------------------------------------
		protected override void DoInsertText( IRtfInterpreterContext context, string text )
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.TextFormatText ) )
			{
				string msg = text;
				if ( msg.Length > this.settings.TextMaxLength && !string.IsNullOrEmpty( this.settings.TextOverflowText ) )
				{
					msg = msg.Substring( 0, msg.Length - this.settings.TextOverflowText.Length ) + this.settings.TextOverflowText;
				}
				Log( string.Format(
					CultureInfo.InvariantCulture,
					this.settings.TextFormatText,
					msg,
					context.GetSafeCurrentTextFormat() ) );
			}
		} // DoInsertText

		// ----------------------------------------------------------------------
		protected override void DoInsertSpecialChar( IRtfInterpreterContext context, RtfVisualSpecialCharKind kind )
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.SpecialCharFormatText ) )
			{
				Log( string.Format(
					CultureInfo.InvariantCulture,
					this.settings.SpecialCharFormatText,
					kind ) );
			}
		} // DoInsertSpecialChar

		// ----------------------------------------------------------------------
		protected override void DoInsertBreak( IRtfInterpreterContext context, RtfVisualBreakKind kind )
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.BreakFormatText ) )
			{
				Log( string.Format(
					CultureInfo.InvariantCulture,
					this.settings.BreakFormatText,
					kind ) );
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
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.ImageFormatText ) )
			{
				Log( string.Format(
					CultureInfo.InvariantCulture,
					this.settings.ImageFormatText,
					format,
					width,
					height,
					desiredWidth,
					desiredHeight,
					scaleWidthPercent,
					scaleHeightPercent,
					imageDataHex,
					(imageDataHex.Length / 2) ) );
			}
		} // DoInsertImage

		// ----------------------------------------------------------------------
		protected override void DoEndDocument( IRtfInterpreterContext context )
		{
			if ( this.settings.Enabled && logger.IsInfoEnabled && !string.IsNullOrEmpty( this.settings.EndDocumentText ) )
			{
				Log( this.settings.EndDocumentText );
			}
		} // DoEndDocument

		// ----------------------------------------------------------------------
		private void Log( string message )
		{
			systemLogger.Info( message );
			if ( this.logger != null )
			{
				this.logger.Info( message );
			}
		} // Log

		// ----------------------------------------------------------------------
		// members
		private readonly RtfInterpreterLoggerSettings settings;
		private readonly ILogger logger;

		private static readonly ILogger systemLogger = Sys.Logging.Logger.GetLogger( typeof( RtfInterpreterListenerLogger ) );

	} // class RtfInterpreterListenerLogger

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
