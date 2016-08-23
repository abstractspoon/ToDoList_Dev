// -- FILE ------------------------------------------------------------------
// name       : ProgramSettings.cs
// project    : RTF Framelet
// created    : Jani Giannoudis - 2008.05.30
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System.Drawing;
using System.IO;
using System.Text;
using System.Drawing.Imaging;
using Itenso.Rtf.Converter.Html;
using Itenso.Sys.Application;

namespace Itenso.Solutions.Community.Rtf2Html
{

	// ------------------------------------------------------------------------
	class ProgramSettings
	{

		// ----------------------------------------------------------------------
    public ProgramSettings(string[] args)
		{
			LoadApplicationArguments(args);
		} // ProgramSettings

		// ----------------------------------------------------------------------
		public bool IsHelpMode
		{
			get { return this.applicationArguments.IsHelpMode; }
		} // IsHelpMode

		// ----------------------------------------------------------------------
		public bool IsValid
		{
			get { return this.applicationArguments.IsValid; }
		} // IsValid

		// ----------------------------------------------------------------------
		public bool IsValidSourceFile
		{
			get
			{
				string sourceFile = SourceFile;
				return !string.IsNullOrEmpty( sourceFile ) && File.Exists( sourceFile );
			}
		} // IsValidSourceFile

		// ----------------------------------------------------------------------
		public string SourceFile
		{
			get { return this.sourceFileArgument.Value; }
		} // SourceFile

		// ----------------------------------------------------------------------
		public string SourceFileNameWithoutExtension
		{
			get
			{
				string sourceFile = SourceFile;
				if ( sourceFile == null )
				{
					return null;
				}
				FileInfo fi = new FileInfo( sourceFile );
				return fi.Name.Replace( fi.Extension, string.Empty );
			}
		} // SourceFileNameWithoutExtension

		// ----------------------------------------------------------------------
		public string DestinationDirectory
		{
			get
			{
				string destinationDirectory = destinationDirectoryArgument.Value;
				if ( string.IsNullOrEmpty( destinationDirectory ) && IsValidSourceFile )
				{
					FileInfo fi = new FileInfo( SourceFile );
					return fi.DirectoryName;
				}
				return destinationDirectory;
			}
		} // DestinationDirectory

		// ----------------------------------------------------------------------
		public string StyleSheets
		{
			get { return this.styleSheetsArgument.Value; }
		} // StyleSheets	

		// ----------------------------------------------------------------------
		public string ImagesDirectory
		{
			get { return this.imagesDirectoryArgument.Value; }
		} // ImagesDirectory	

		// ----------------------------------------------------------------------
		public string ImagesPath
		{
			get 
			{
				string imagesPath = DestinationDirectory;
				if ( !string.IsNullOrEmpty( ImagesDirectory ) )
				{
					imagesPath = Path.Combine( imagesPath, ImagesDirectory );
				}
				return imagesPath; 
			}
		} // ImagesPath	

		// ----------------------------------------------------------------------
		public string ImageType
		{
			get { return this.imageTypeArgument.Value; }
		} // ImageType	

		// ----------------------------------------------------------------------
		public string ImageFileNamePattern
		{
			get 
			{
				string imageFileNamePattern = SourceFileNameWithoutExtension + "{0}{1}";
				if ( !string.IsNullOrEmpty( ImagesDirectory ) )
				{
					imageFileNamePattern = Path.Combine( ImagesDirectory, imageFileNamePattern );
				}
				return imageFileNamePattern;
			}
		} // ImageFileNamePattern	

		// ----------------------------------------------------------------------
		public ImageFormat ImageFormat
		{
			get
			{
				ImageFormat imageFormat = ImageFormat.Jpeg;
				if ( !string.IsNullOrEmpty( ImageType ) )
				{
					switch ( ImageType.ToLower() )
					{
						case "jpg":
							imageFormat = ImageFormat.Jpeg;
							break;
						case "gif":
							imageFormat = ImageFormat.Gif;
							break;
						case "png":
							imageFormat = ImageFormat.Png;
							break;
					}
				}
				return imageFormat; 
			}
		} // ImageFormat	

		// ----------------------------------------------------------------------
		public Color? ImageBackgroundColor
		{
			get
			{
				string backgroundColorName = this.imageBackgroundColorNameArgument.Value;
				if ( string.IsNullOrEmpty( backgroundColorName ) )
				{
					return null;
				}

				Color backgroundColor = Color.FromName( backgroundColorName );
				if ( !backgroundColor.IsKnownColor )
				{
					return null;
				}
				return backgroundColor;
			}
		} // ImageBackgroundColor

		// ----------------------------------------------------------------------
		public string CharacterEncoding
		{
			get { return this.characterEncodingArgument.Value; }
		} // CharacterEncoding	

		// ----------------------------------------------------------------------
		public Encoding Encoding
		{
			get
			{
				Encoding encoding = Encoding.UTF8;

				if ( !string.IsNullOrEmpty( CharacterEncoding ) )
				{
					switch ( CharacterEncoding.ToLower() )
					{
						case "ascii":
							encoding = Encoding.ASCII;
							break;
						case "utf7":
							encoding = Encoding.UTF7;
							break;
						case "utf8":
							encoding = Encoding.UTF8;
							break;
						case "unicode":
							encoding = Encoding.Unicode;
							break;
						case "bigendianunicode":
							encoding = Encoding.BigEndianUnicode;
							break;
						case "utf32":
							encoding = Encoding.UTF32;
							break;
						case "operatingsystem":
							encoding = Encoding.Default;
							break;
					}
				}

				return encoding;
			}
		} // Encoding

		// ----------------------------------------------------------------------
		public string CharacterSet
		{
			get { return this.characterSetArgument.Value; }
		} // CharacterSet	

		// ----------------------------------------------------------------------
		public bool HasDestinationOutput
		{
			get { return SaveHtml || SaveImage; }
		} // HasDestinationOutput	

		// ----------------------------------------------------------------------
		public bool SaveHtml
		{
			get { return this.saveHtmlArgument.Value; }
		} // SaveHtml	

		// ----------------------------------------------------------------------
		public bool SaveImage
		{
			get { return this.saveImageArgument.Value; }
		} // SaveImage	

		// ----------------------------------------------------------------------
		public bool DisplayHtml
		{
			get { return this.displayHtmlArgument.Value; }
		} // DisplayHtml

		// ----------------------------------------------------------------------
		public string LogDirectory
		{
			get { return this.logDirectoryArgument.Value; }
		} // LogDirectory	

		// ----------------------------------------------------------------------
		public bool LogParser
		{
			get { return this.logParserArgument.Value; }
		} // LogParser

		// ----------------------------------------------------------------------
		public bool LogInterpreter
		{
			get { return this.logInterpreterArgument.Value; }
		} // LogInterpreter

		// ----------------------------------------------------------------------
		public bool OpenHtml
		{
			get { return this.openHtmlArgument.Value; }
		} // OpenHtml

		// ----------------------------------------------------------------------
		public bool ShowHiddenText
		{
			get { return this.showHiddenTextArgument.Value; }
		} // ShowHiddenText

		// ----------------------------------------------------------------------
		public bool UseNonBreakingSpaces
		{
			get { return this.useNonBreakingSpacesArgument.Value; }
		} // UseNonBreakingSpaces

		// ----------------------------------------------------------------------
		public bool ConvertVisualHyperlinks
		{
			get { return this.convertVisualHyperlinksArgument.Value; }
		} // ConvertVisualHyperlinks

		// ----------------------------------------------------------------------
		public string VisualHyperlinkPattern
		{
			get { return this.visualHyperlinkPatternArgument.Value; }
		} // VisualHyperlinkPattern	

		// ----------------------------------------------------------------------
		public bool ExtendedImageScale
		{
			get { return this.extendedImageScaleArgument.Value; }
		} // ExtendedImageScale


		// ----------------------------------------------------------------------
		public string DocumentScope
		{
			get { return this.documentScopeArgument.Value; }
		} // DocumentScope	

		// ----------------------------------------------------------------------
		public RtfHtmlConvertScope ConvertScope
		{
			get
			{
				RtfHtmlConvertScope convertScope = RtfHtmlConvertScope.None;

				if ( !string.IsNullOrEmpty( DocumentScope ) )
				{
					string[] tokens = DocumentScope.Split( ',' );
					foreach ( string token in tokens )
					{
						switch ( token.ToLower() )
						{
							case "doc":
							case "document":
								convertScope |= RtfHtmlConvertScope.Document;
								break;
							case "html":
								convertScope |= RtfHtmlConvertScope.Html;
								break;
							case "head":
								convertScope |= RtfHtmlConvertScope.Head;
								break;
							case "body":
								convertScope |= RtfHtmlConvertScope.Body;
								break;
							case "content":
								convertScope |= RtfHtmlConvertScope.Content;
								break;
							case "*":
							case "all":
								convertScope |= RtfHtmlConvertScope.All;
								break;
						}
					}
				}
				return convertScope;
			}
		} // ConvertScope

		// ----------------------------------------------------------------------
		public string BuildDestinationFileName( string path, string extension )
		{
			string sourceFileNameWithoutExtension = SourceFileNameWithoutExtension;
			if ( sourceFileNameWithoutExtension == null )
			{
				return null;
			}

			return Path.Combine( 
				string.IsNullOrEmpty( path ) ? DestinationDirectory : path,
				sourceFileNameWithoutExtension + extension );
		} // BuildDestinationFileName

		// ----------------------------------------------------------------------
    private void LoadApplicationArguments(string[] args) 
		{
			this.applicationArguments.Arguments.Add( new HelpModeArgument() );
			this.applicationArguments.Arguments.Add( this.sourceFileArgument );
			this.applicationArguments.Arguments.Add( this.destinationDirectoryArgument );
			this.applicationArguments.Arguments.Add( this.styleSheetsArgument );
			this.applicationArguments.Arguments.Add( this.imagesDirectoryArgument );
			this.applicationArguments.Arguments.Add( this.imageTypeArgument );
			this.applicationArguments.Arguments.Add( this.imageBackgroundColorNameArgument );
			this.applicationArguments.Arguments.Add( this.characterEncodingArgument );
			this.applicationArguments.Arguments.Add( this.characterSetArgument );
			this.applicationArguments.Arguments.Add( this.saveHtmlArgument );
			this.applicationArguments.Arguments.Add( this.saveImageArgument );
			this.applicationArguments.Arguments.Add( this.logDirectoryArgument );
			this.applicationArguments.Arguments.Add( this.logParserArgument );
			this.applicationArguments.Arguments.Add( this.logInterpreterArgument );
			this.applicationArguments.Arguments.Add( this.displayHtmlArgument );
			this.applicationArguments.Arguments.Add( this.openHtmlArgument );
			this.applicationArguments.Arguments.Add( this.showHiddenTextArgument );
			this.applicationArguments.Arguments.Add( this.useNonBreakingSpacesArgument );
			this.applicationArguments.Arguments.Add( this.convertVisualHyperlinksArgument );
			this.applicationArguments.Arguments.Add( this.visualHyperlinkPatternArgument );
			this.applicationArguments.Arguments.Add( this.extendedImageScaleArgument );
			this.applicationArguments.Arguments.Add( this.documentScopeArgument );

      this.applicationArguments.Load(args);
		} // LoadApplicationArguments

		// ----------------------------------------------------------------------
		// members
		private readonly ApplicationArguments applicationArguments = new ApplicationArguments();
		private readonly ValueArgument sourceFileArgument = new ValueArgument( ArgumentType.Mandatory );
		private readonly ValueArgument destinationDirectoryArgument = new ValueArgument();
		private readonly NamedValueArgument styleSheetsArgument = new NamedValueArgument( "CSS" );
		private readonly NamedValueArgument imagesDirectoryArgument = new NamedValueArgument( "ID" );
		private readonly NamedValueArgument imageTypeArgument = new NamedValueArgument( "IT" );
		private readonly NamedValueArgument imageBackgroundColorNameArgument = new NamedValueArgument( "BC" );
		private readonly NamedValueArgument characterEncodingArgument = new NamedValueArgument( "CE" );
		private readonly NamedValueArgument characterSetArgument = new NamedValueArgument( "CS" );
		private readonly ToggleArgument saveHtmlArgument = new ToggleArgument( "SH", true );
		private readonly ToggleArgument saveImageArgument = new ToggleArgument( "SI", true );
		private readonly NamedValueArgument logDirectoryArgument = new NamedValueArgument( "LD" );
		private readonly ToggleArgument logParserArgument = new ToggleArgument( "LP", false );
		private readonly ToggleArgument logInterpreterArgument = new ToggleArgument( "LI", false );
		private readonly ToggleArgument displayHtmlArgument = new ToggleArgument( "D", false );
		private readonly ToggleArgument openHtmlArgument = new ToggleArgument( "O", false );
		private readonly ToggleArgument showHiddenTextArgument = new ToggleArgument( "HT", false );
		private readonly ToggleArgument useNonBreakingSpacesArgument = new ToggleArgument( "NBS", false );
		private readonly ToggleArgument convertVisualHyperlinksArgument = new ToggleArgument( "CH", false );
		private readonly NamedValueArgument visualHyperlinkPatternArgument = new NamedValueArgument( "HP" );
		private readonly ToggleArgument extendedImageScaleArgument = new ToggleArgument( "XS", false );
		private readonly NamedValueArgument documentScopeArgument = new NamedValueArgument( "DS" );

	} // class ProgramSettings

} // namespace Itenso.Solutions.Community.Rtf2Html
// -- EOF -------------------------------------------------------------------
