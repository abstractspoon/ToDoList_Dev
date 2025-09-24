// -- FILE ------------------------------------------------------------------
// name       : Program.cs
// project    : RTF Framelet
// created    : Jani Giannoudis - 2008.05.30
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.IO;
using System.Diagnostics;
using Itenso.Sys.Application;
using Itenso.Rtf;
using Itenso.Rtf.Support;
using Itenso.Rtf.Parser;
using Itenso.Rtf.Interpreter;
using Itenso.Rtf.Converter.Image;
using Itenso.Rtf.Converter.Html;

namespace Itenso.Solutions.Community.Rtf2Html
{

	// ------------------------------------------------------------------------
	enum ProgramExitCode
	{
		Successfully = 0,
		InvalidSettings = -1,
		ParseRtf = -2,
		DestinationDirectory = -3,
		InterpretRtf = -4,
		ConvertHtml = -5,
		SaveHtml = -6,
	} // enum ProgramExitCode

	// ------------------------------------------------------------------------
	public class Program
	{

		// ----------------------------------------------------------------------
		public Program(string rtf, string html, 
                       string a1, string a2, string a3,
                       string a4, string a5, string a6,
                       string a7, string a8, string a9)
		{
            ExitCode = ProgramExitCode.Successfully;
            string[] args = { rtf, html, a1, a2, a3, a4, a5, a6, a7, a8, a9 };

			this.settings = new ProgramSettings(args);
		} // Program

		// ----------------------------------------------------------------------
		private static ProgramExitCode ExitCode
		{
			get { return (ProgramExitCode)Environment.ExitCode; }
			set { Environment.ExitCode = (int)value; }
		} // ExitCode

		// ----------------------------------------------------------------------
		public bool Execute()
		{
			// program settings
			if ( ValidateProgramSettings() == false )
			{
				return false;
			}

			// parse rtf
			IRtfGroup rtfStructure = ParseRtf();
			if ( ExitCode != ProgramExitCode.Successfully )
			{
				return false;
			}

			// destination directory
			EnsureDestinationDirectory();
			if ( ExitCode != ProgramExitCode.Successfully )
			{
				return false;
			}

			// image handling
			RtfVisualImageAdapter imageAdapter = new RtfVisualImageAdapter(
				this.settings.ImageFileNamePattern,
				this.settings.ImageFormat );

			// interpret rtf
			IRtfDocument rtfDocument = InterpretRtf( rtfStructure, imageAdapter );
			if ( ExitCode != ProgramExitCode.Successfully )
			{
				return false;
			}

			// convert to hmtl
			string html = ConvertHmtl( rtfDocument, imageAdapter );
			if ( ExitCode != ProgramExitCode.Successfully )
			{
				return false;
			}

			// save html
			string fileName = SaveHmtl( html );
			if ( ExitCode != ProgramExitCode.Successfully )
			{
				return false;
			}

			// open html file
			OpenHtmlFile( fileName );
			if ( ExitCode != ProgramExitCode.Successfully )
			{
				return false;
			}

			// display html text
			DisplayHtmlText( html );
			if ( ExitCode != ProgramExitCode.Successfully )
			{
				return false;
			}

            return true;
		} // Execute

		// ----------------------------------------------------------------------
		private bool ValidateProgramSettings()
		{
			if ( !this.settings.IsValid )
			{
				ExitCode = ProgramExitCode.InvalidSettings;
				return false;
			}

			return true;
		} // ValidateProgramSettings

		// ----------------------------------------------------------------------
		private IRtfGroup ParseRtf()
		{
			IRtfGroup rtfStructure;
			RtfParserListenerFileLogger parserLogger = null;
			try
			{
				// logger
				if ( this.settings.LogParser )
				{
					string logFileName = this.settings.BuildDestinationFileName( 
						this.settings.LogDirectory,
						RtfParserListenerFileLogger.DefaultLogFileExtension );
					parserLogger = new RtfParserListenerFileLogger( logFileName );
				}

				// rtf parser
				// open readonly - in case of dominant locks...
				using ( FileStream stream = File.Open( this.settings.SourceFile, FileMode.Open, FileAccess.Read, FileShare.ReadWrite ) )
				{
					// parse the rtf structure
					RtfParserListenerStructureBuilder structureBuilder = new RtfParserListenerStructureBuilder();
					RtfParser parser = new RtfParser( structureBuilder );
					parser.IgnoreContentAfterRootGroup = true; // support WordPad documents
					if ( parserLogger != null )
					{
						parser.AddParserListener( parserLogger );
					}
					parser.Parse( new RtfSource( stream ) );
					rtfStructure = structureBuilder.StructureRoot;
				}
			}
			catch ( Exception /*e*/ )
			{
				if ( parserLogger != null )
				{
					parserLogger.Dispose();
				}

				//Console.WriteLine( "error while parsing rtf: " + e.Message );
				ExitCode = ProgramExitCode.ParseRtf;
				return null;
			}

			return rtfStructure;
		} // ParseRtf

		// ----------------------------------------------------------------------
		private void EnsureDestinationDirectory()
		{
			if ( !this.settings.HasDestinationOutput )
			{
				return;
			}

			try
			{
				if ( !Directory.Exists( this.settings.DestinationDirectory ) )
				{
					Directory.CreateDirectory( this.settings.DestinationDirectory ); 
				}
			}
			catch ( Exception /*e*/ )
			{
				//Console.WriteLine( "error while creating destination directory: " + e.Message );
				ExitCode = ProgramExitCode.DestinationDirectory;
				return;
			}
		} // EnsureDestinationDirectory

		// ----------------------------------------------------------------------
		private IRtfDocument InterpretRtf( IRtfGroup rtfStructure, IRtfVisualImageAdapter imageAdapter )
		{
			IRtfDocument rtfDocument;
			RtfInterpreterListenerFileLogger interpreterLogger = null;
			try
			{
				// logger
				if ( this.settings.LogInterpreter )
				{
					string logFileName = this.settings.BuildDestinationFileName(
						this.settings.LogDirectory,
						RtfInterpreterListenerFileLogger.DefaultLogFileExtension );
					interpreterLogger = new RtfInterpreterListenerFileLogger( logFileName );
				}

				// image converter
				RtfImageConverter imageConverter = null;
				if ( this.settings.SaveImage )
				{
					RtfImageConvertSettings imageConvertSettings = new RtfImageConvertSettings( imageAdapter );
					imageConvertSettings.ImagesPath = this.settings.DestinationDirectory;
					imageConvertSettings.BackgroundColor = this.settings.ImageBackgroundColor;
					if ( this.settings.ExtendedImageScale )
					{
						imageConvertSettings.ScaleExtension = 0.5f;
					}
					imageConverter = new RtfImageConverter( imageConvertSettings );
				}

				// rtf parser
				// interpret the rtf structure using the extractors
				rtfDocument = RtfInterpreterTool.BuildDoc( rtfStructure, interpreterLogger, imageConverter );

			}
			catch ( Exception /*e*/ )
			{
				if ( interpreterLogger != null )
				{
					interpreterLogger.Dispose();
				}

				//Console.WriteLine( "error while interpreting rtf: " + e.Message );
				ExitCode = ProgramExitCode.InterpretRtf;
				return null;
			}

			return rtfDocument;
		} // InterpretRtf

		// ----------------------------------------------------------------------
		private string ConvertHmtl( IRtfDocument rtfDocument, IRtfVisualImageAdapter imageAdapter )
		{
			string html;

			try
			{
				RtfHtmlConvertSettings htmlConvertSettings = new RtfHtmlConvertSettings( imageAdapter );
				if ( this.settings.CharacterSet != null )
				{
					htmlConvertSettings.CharacterSet = this.settings.CharacterSet;
				}
				htmlConvertSettings.Title = this.settings.SourceFileNameWithoutExtension;
				htmlConvertSettings.ImagesPath = this.settings.ImagesPath;
				htmlConvertSettings.IsShowHiddenText = this.settings.ShowHiddenText;
				htmlConvertSettings.UseNonBreakingSpaces = this.settings.UseNonBreakingSpaces;
				if ( this.settings.ConvertScope != RtfHtmlConvertScope.None )
				{
					htmlConvertSettings.ConvertScope = this.settings.ConvertScope;
				}
				if ( !string.IsNullOrEmpty( this.settings.StyleSheets ) )
				{
					string[] styleSheets = this.settings.StyleSheets.Split( ',' );
					htmlConvertSettings.StyleSheetLinks.AddRange( styleSheets );
				}
				htmlConvertSettings.ConvertVisualHyperlinks = this.settings.ConvertVisualHyperlinks;
				if ( !string.IsNullOrEmpty( this.settings.VisualHyperlinkPattern ) )
				{
					htmlConvertSettings.VisualHyperlinkPattern = this.settings.VisualHyperlinkPattern;
				}

				RtfHtmlConverter htmlConverter = new RtfHtmlConverter( rtfDocument, htmlConvertSettings );
				html = htmlConverter.Convert();
			}
			catch ( Exception /*e*/ )
			{
				//Console.WriteLine( "error while converting to html: " + e.Message );
				ExitCode = ProgramExitCode.ConvertHtml;
				return null;
			}

			return html;
		} // ConvertHmtl

		// ----------------------------------------------------------------------
		private string SaveHmtl( string text )
		{
			if ( !this.settings.SaveHtml )
			{
				return null;
			}

			string fileName = this.settings.BuildDestinationFileName( null, RtfHtmlConverter.DefaultHtmlFileExtension );
			try
			{
				using ( TextWriter writer = new StreamWriter( fileName, false, this.settings.Encoding ) )
				{
					writer.Write( text );
				}
			}
			catch ( Exception /*e*/ )
			{
				//Console.WriteLine( "error while saving html: " + e.Message );
				ExitCode = ProgramExitCode.SaveHtml;
				return null;
			}

			return fileName;
		} // SaveHmtl

		// ----------------------------------------------------------------------
		private void OpenHtmlFile( string fileName )
		{
			if ( !this.settings.SaveHtml || !this.settings.OpenHtml )
			{
				return;
			}
			Process.Start( fileName );
		} // OpenHtmlFile

		// ----------------------------------------------------------------------
		private void DisplayHtmlText( string htmlText )
		{
// 			if ( !this.settings.DisplayHtml )
// 			{
// 				return;
// 			}
// 			Console.WriteLine( htmlText );
		} // DisplayHtmlText

		// ----------------------------------------------------------------------
		// members
		private readonly ProgramSettings settings;

	} // class Program

} // namespace Itenso.Solutions.Community.Rtf2Html
// -- EOF -------------------------------------------------------------------
