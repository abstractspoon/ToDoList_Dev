// -- FILE ------------------------------------------------------------------
// name       : RtfHtmlConvertSettings.cs
// project    : RTF Framelet
// created    : Jani Giannoudis - 2008.06.02
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Collections.Specialized;
using Itenso.Rtf.Converter.Image;

namespace Itenso.Rtf.Converter.Html
{

	// ------------------------------------------------------------------------
	public class RtfHtmlConvertSettings
	{

		// ----------------------------------------------------------------------
		public const string DefaultDocumentHeader =
			"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//DE\" \"http://www.w3.org/TR/html4/loose.dtd\">";
		public const string DefaultDocumentCharacterSet = "UTF-8";

		// regex souce: http://msdn.microsoft.com/en-us/library/aa159903.aspx
		public const string DefaultVisualHyperlinkPattern =
			@"[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,3}(:[a-zA-Z0-9]*)?/?([a-zA-Z0-9\-\._\?\,\'/\\\+&%\$#\=~])*";

		// ----------------------------------------------------------------------
		public RtfHtmlConvertSettings()
			: this( new RtfVisualImageAdapter(), RtfHtmlConvertScope.All )
		{
		} // RtfHtmlConvertSettings

		// ----------------------------------------------------------------------
		public RtfHtmlConvertSettings( RtfHtmlConvertScope convertScope )
			: this( new RtfVisualImageAdapter(), convertScope )
		{
		} // RtfHtmlConvertSettings

		// ----------------------------------------------------------------------
		public RtfHtmlConvertSettings( IRtfVisualImageAdapter imageAdapter )
			: this( imageAdapter, RtfHtmlConvertScope.All )
		{
		} // RtfHtmlConvertSettings

		// ----------------------------------------------------------------------
		public RtfHtmlConvertSettings( IRtfVisualImageAdapter imageAdapter, RtfHtmlConvertScope convertScope )
		{
			if ( imageAdapter == null )
			{
				throw new ArgumentNullException( "imageAdapter" );
			}

			this.imageAdapter = imageAdapter;
			this.convertScope = convertScope;
			this.visualHyperlinkPattern = DefaultVisualHyperlinkPattern;
		} // RtfHtmlConvertSettings

		// ----------------------------------------------------------------------
		public IRtfVisualImageAdapter ImageAdapter
		{
			get { return this.imageAdapter; }
		} // ImageAdapter

		// ----------------------------------------------------------------------
		public RtfHtmlConvertScope ConvertScope
		{
			get { return this.convertScope; }
			set { this.convertScope = value; }
		} // ConvertScope

		// ----------------------------------------------------------------------
		public bool HasStyles
		{
			get { return this.styles != null && this.styles.Count > 0; }
		} // HasStyles
		
		// ----------------------------------------------------------------------
		public RtfHtmlCssStyleCollection Styles
		{
			get 
			{
				if ( this.styles == null )
				{
					this.styles = new RtfHtmlCssStyleCollection();
				}
				return this.styles; 
			}
		} // Styles

		// ----------------------------------------------------------------------
		public bool HasStyleSheetLinks
		{
			get { return this.styleSheetLinks != null && this.styleSheetLinks.Count > 0; }
		} // HasStyleSheetLinks

		// ----------------------------------------------------------------------
		public StringCollection StyleSheetLinks
		{
			get
			{
				if ( this.styleSheetLinks == null )
				{
					this.styleSheetLinks = new StringCollection();
				}
				return this.styleSheetLinks;
			}
		} // StyleSheetLinks

		// ----------------------------------------------------------------------
		public string DocumentHeader
		{
			get { return this.documentHeader; }
			set { this.documentHeader = value; }
		} // DocumentHeader

		// ----------------------------------------------------------------------
		public string Title
		{
			get { return this.title; }
			set { this.title = value; }
		} // Title

		// ----------------------------------------------------------------------
		public string CharacterSet
		{
			get { return this.characterSet; }
			set { this.characterSet = value; }
		} // CharacterSet

		// ----------------------------------------------------------------------
		public string VisualHyperlinkPattern
		{
			get { return this.visualHyperlinkPattern; }
			set { this.visualHyperlinkPattern = value; }
		} // VisualHyperlinkPattern

		// ----------------------------------------------------------------------
		public bool IsShowHiddenText
		{
			get { return this.showHiddenText; }
			set { this.showHiddenText = value; }
		} // IsShowHiddenText

		// ----------------------------------------------------------------------
		public bool ConvertVisualHyperlinks
		{
			get { return this.convertVisualHyperlinks; }
			set { this.convertVisualHyperlinks = value; }
		} // ConvertVisualHyperlinks

		// ----------------------------------------------------------------------
		public bool UseNonBreakingSpaces
		{
			get { return this.useNonBreakingSpaces; }
			set { this.useNonBreakingSpaces = value; }
		} // UseNonBreakingSpaces

		// ----------------------------------------------------------------------
		public string ImagesPath
		{
			get { return this.imagesPath; }
			set { this.imagesPath = value; }
		} // ImagesPath

		// ----------------------------------------------------------------------
		public string GetImageUrl( int index, RtfVisualImageFormat rtfVisualImageFormat )
		{
			string imageFileName = this.imageAdapter.ResolveFileName( index, rtfVisualImageFormat );
			return imageFileName.Replace( '\\', '/' );
		} // GetImageUrl

		// ----------------------------------------------------------------------
		// members
		private readonly IRtfVisualImageAdapter imageAdapter;
		private RtfHtmlConvertScope convertScope;
		private RtfHtmlCssStyleCollection styles;
		private StringCollection styleSheetLinks;
		private string imagesPath;
		private string documentHeader = DefaultDocumentHeader;
		private string title;
		private string visualHyperlinkPattern;
		private string characterSet = DefaultDocumentCharacterSet;
		private bool showHiddenText;
		private bool convertVisualHyperlinks;
		private bool useNonBreakingSpaces;

	} // class RtfHtmlConvertSettings

} // namespace Itenso.Rtf.Converter.Html
// -- EOF -------------------------------------------------------------------
