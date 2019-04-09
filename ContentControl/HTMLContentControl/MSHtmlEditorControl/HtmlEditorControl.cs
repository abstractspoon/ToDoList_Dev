#region Using Directives

using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.Threading;
using System.Text.RegularExpressions;
using System.Web;
using System.IO;
using System.Net;
using System.Text;
using System.Globalization;
using System.Runtime.InteropServices;

using mshtmlDocument = mshtml.HTMLDocument;
using mshtmlBody = mshtml.HTMLBody;
using mshtmlStyleSheet = mshtml.IHTMLStyleSheet;
using mshtmlStyleElement = mshtml.IHTMLStyleElement;
using mshtmlStyle = mshtml.IHTMLStyle;
using mshtmlDomNode = mshtml.IHTMLDOMNode;
using mshtmlDomTextNode = mshtml.IHTMLDOMTextNode;
using mshtmlTextRange = mshtml.IHTMLTxtRange;
using mshtmlSelection = mshtml.IHTMLSelectionObject;
using mshtmlControlRange = mshtml.IHTMLControlRange;

using mshtmlElement = mshtml.IHTMLElement;
using mshtmlElementCollection = mshtml.IHTMLElementCollection;
using mshtmlControlElement = mshtml.IHTMLControlElement;
using mshtmlAnchorElement = mshtml.IHTMLAnchorElement;
using mshtmlImageElement = mshtml.IHTMLImgElement;
using mshtmlFontElement= mshtml.IHTMLFontElement;
using mshtmlLineElement = mshtml.IHTMLHRElement;
using mshtmlSpanElement = mshtml.IHTMLSpanFlow;
using mshtmlScriptElement = mshtml.IHTMLScriptElement;

using mshtmlTable = mshtml.IHTMLTable;
using mshtmlTableCaption = mshtml.IHTMLTableCaption;
using mshtmlTableRow = mshtml.IHTMLTableRow;
using mshtmlTableCell = mshtml.IHTMLTableCell;
using mshtmlTableRowMetrics = mshtml.IHTMLTableRowMetrics;
using mshtmlTableColumn = mshtml.IHTMLTableCol;

using mshtmlEventObject = mshtml.IHTMLEventObj;

using Pavonis.COM;
using Pavonis.COM.IOleCommandTarget;

#endregion

namespace MSDN.Html.Editor
{

    /// <summary>
    /// This is the main UserControl class that defines the Html Editor
    /// BodyHtml sets the complet Body including the body tag
    /// InnerText and InnerHtml sets the contents of the Body
    /// Property ReadOnly defines whether the content is editable
    /// </summary>
    [DefaultProperty("InnerText")]
    [ToolboxBitmap(typeof(HtmlEditorControl), "Resources.HtmlEditor.bmp")]
    public partial class HtmlEditorControl : UserControl
    {
		public new event EventHandler LostFocus;

		#region Public Events

		/// <summary>
		/// Public event that is raised if an internal processing exception is found
		/// </summary>
		[Category("Exception"), Description("An Internal Processing Exception was encountered")]
        public event HtmlExceptionEventHandler HtmlException;

        /// <summary>
        /// Public event that is raised if navigation event is captured
        /// </summary>
        [Category("Navigation"), Description("A Navigation Event was encountered")]
        public event HtmlNavigationEventHandler HtmlNavigation;

        #endregion

        #region Constant Defintions

        // general constants
        private const int HTML_BUFFER_SIZE = 256;
        private const byte DEFAULT_BORDER_SIZE = 2;

        // define the tags being used by the application
        private const string BODY_TAG = "BODY";
        private const string SCRIPT_TAG = "SCRIPT";
        private const string STYLE_TAG = "STYLE";
        private const string ANCHOR_TAG = "A";
        private const string FONT_TAG = "FONT";
        private const string BOLD_TAG = "STRONG";
        private const string UNDERLINE_TAG = "U";
        private const string ITALIC_TAG = "EM";
        private const string STRIKE_TAG = "STRIKE";
        private const string SUBSCRIPT_TAG = "SUB";
        private const string SUPERSCRIPT_TAG = "SUP";
        private const string HEAD_TAG = "HEAD";
        private const string IMAGE_TAG = "IMG";
        private const string TABLE_TAG = "TABLE";
        private const string TABLE_ROW_TAG = "TR";
        private const string TABLE_CELL_TAG = "TD";
        private const string TABLE_HEAD_TAG = "TH";
        private const string SPAN_TAG = "SPAN";
        private const string OPEN_TAG = "<";
        private const string CLOSE_TAG = ">";
        private const string SELECT_TYPE_TEXT = "text";
        private const string SELECT_TYPE_CONTROL = "control";
        private const string SELECT_TYPE_NONE = "none";

        // define commands for mshtml execution execution
        private const string HTML_COMMAND_OVERWRITE = "OverWrite";
        private const string HTML_COMMAND_BOLD = "Bold";
        private const string HTML_COMMAND_UNDERLINE = "Underline";
        private const string HTML_COMMAND_ITALIC = "Italic";
        private const string HTML_COMMAND_SUBSCRIPT = "Subscript";
        private const string HTML_COMMAND_SUPERSCRIPT = "Superscript";
        private const string HTML_COMMAND_STRIKE_THROUGH = "StrikeThrough";
        private const string HTML_COMMAND_FONT_NAME = "FontName";
        private const string HTML_COMMAND_FONT_SIZE = "FontSize";
        private const string HTML_COMMAND_FORE_COLOR = "ForeColor";
        private const string HTML_COMMAND_INSERT_FORMAT_BLOCK = "FormatBlock";
        private const string HTML_COMMAND_REMOVE_FORMAT = "RemoveFormat";
        private const string HTML_COMMAND_JUSTIFY_LEFT = "JustifyLeft";
        private const string HTML_COMMAND_JUSTIFY_CENTER = "JustifyCenter";
        private const string HTML_COMMAND_JUSTIFY_RIGHT = "JustifyRight";
        private const string HTML_COMMAND_INDENT = "Indent";
        private const string HTML_COMMAND_OUTDENT = "Outdent";
        private const string HTML_COMMAND_INSERT_LINE = "InsertHorizontalRule";
        private const string HTML_COMMAND_INSERT_LISTUNORDERED = "InsertUnorderedList";
        private const string HTML_COMMAND_INSERT_LISTORDERED = "InsertOrderedList";
        private const string HTML_COMMAND_INSERT_IMAGE = "InsertImage";
        private const string HTML_COMMAND_INSERT_LINK = "CreateLink";
        private const string HTML_COMMAND_REMOVE_LINK = "Unlink";
        private const string HTML_COMMAND_TEXT_CUT = "Cut";
        private const string HTML_COMMAND_TEXT_COPY = "Copy";
        private const string HTML_COMMAND_TEXT_PASTE = "Paste";
        private const string HTML_COMMAND_TEXT_DELETE = "Delete";
        private const string HTML_COMMAND_TEXT_UNDO = "Undo";
        private const string HTML_COMMAND_TEXT_REDO= "Redo";
        private const string HTML_COMMAND_TEXT_SELECT_ALL = "SelectAll";
        private const string HTML_COMMAND_TEXT_UNSELECT = "Unselect";
        private const string HTML_COMMAND_TEXT_PRINT = "Print";
        private const string HTML_FORMATTED_PRE = "Formatted";
        private const string HTML_FORMATTED_NORMAL = "Normal";
        private const string HTML_FORMATTED_HEADING = "Heading";
        private const string HTML_FORMATTED_HEADING1 = "Heading 1";
        private const string HTML_FORMATTED_HEADING2 = "Heading 2";
        private const string HTML_FORMATTED_HEADING3 = "Heading 3";
        private const string HTML_FORMATTED_HEADING4 = "Heading 4";
        private const string HTML_FORMATTED_HEADING5 = "Heading 5";

        // internal command constants
        private const string INTERNAL_COMMAND_TEXTCUT = "TextCut";
        private const string INTERNAL_COMMAND_TEXTCOPY = "TextCopy";
        private const string INTERNAL_COMMAND_TEXTPASTE = "TextPaste";
        private const string INTERNAL_COMMAND_TEXTDELETE = "TextDelete";
        private const string INTERNAL_COMMAND_CLEARSELECT = "ClearSelect";
        private const string INTERNAL_COMMAND_SELECTALL = "SelectAll";
        private const string INTERNAL_COMMAND_EDITUNDO = "EditUndo";
        private const string INTERNAL_COMMAND_EDITREDO = "EditRedo";
        private const string INTERNAL_COMMAND_FORMATBOLD = "FormatBold";
        private const string INTERNAL_COMMAND_FORMATUNDERLINE = "FormatUnderline";
        private const string INTERNAL_COMMAND_FORMATITALIC = "FormatItalic";
        private const string INTERNAL_COMMAND_FORMATSUPERSCRIPT = "FormatSuperscript";
        private const string INTERNAL_COMMAND_FORMATSUBSCRIPT = "FormatSubscript";
        private const string INTERNAL_COMMAND_FORMATSTRIKEOUT = "FormatStrikeout";
        private const string INTERNAL_COMMAND_FONTDIALOG = "FontDialog";
        private const string INTERNAL_COMMAND_FONTNORMAL = "FontNormal";
        private const string INTERNAL_COMMAND_COLORDIALOG = "ColorDialog";
        private const string INTERNAL_COMMAND_FONTINCREASE = "FontIncrease";
        private const string INTERNAL_COMMAND_FONTDECREASE = "FontDecrease";
        private const string INTERNAL_COMMAND_JUSTIFYLEFT = "JustifyLeft";
        private const string INTERNAL_COMMAND_JUSTIFYCENTER = "JustifyCenter";
        private const string INTERNAL_COMMAND_JUSTIFYRIGHT = "JustifyRight";
        private const string INTERNAL_COMMAND_FONTINDENT = "FontIndent";
        private const string INTERNAL_COMMAND_FONTOUTDENT = "FontOutdent";
        private const string INTERNAL_COMMAND_LISTORDERED = "ListOrdered";
        private const string INTERNAL_COMMAND_LISTUNORDERED = "ListUnordered";
        private const string INTERNAL_COMMAND_INSERTLINE = "InsertLine";
        private const string INTERNAL_COMMAND_INSERTTABLE = "InsertTable";
        private const string INTERNAL_COMMAND_TABLEPROPERTIES = "TableModify";
        private const string INTERNAL_COMMAND_TABLEINSERTROW = "TableInsertRow";
        private const string INTERNAL_COMMAND_TABLEDELETEROW = "TableDeleteRow";
        private const string INTERNAL_COMMAND_INSERTIMAGE = "InsertImage";
        private const string INTERNAL_COMMAND_INSERTLINK = "InsertLink";
        private const string INTERNAL_COMMAND_INSERTTEXT = "InsertText";
        private const string INTERNAL_COMMAND_INSERTHTML = "InsertHtml";
        private const string INTERNAL_COMMAND_FINDREPLACE = "FindReplace";
        private const string INTERNAL_COMMAND_DOCUMENTPRINT = "DocumentPrint";
        private const string INTERNAL_COMMAND_OPENFILE = "OpenFile";
        private const string INTERNAL_COMMAND_SAVEFILE = "SaveFile";
        private const string INTERNAL_TOGGLE_OVERWRITE = "ToggleOverwrite";
        private const string INTERNAL_TOGGLE_TOOLBAR = "ToggleToolbar";
        private const string INTERNAL_TOGGLE_SCROLLBAR = "ToggleScrollbar";
        private const string INTERNAL_TOGGLE_WORDWRAP = "ToggleWordwrap";
        private const string INTERNAL_FORMATTED_PRE = "FormattedPre";
        private const string INTERNAL_FORMATTED_NORMAL = "FormattedNormal";
        private const string INTERNAL_FORMATTED_HEADING1 = "FormattedHeading1";
        private const string INTERNAL_FORMATTED_HEADING2 = "FormattedHeading2";
        private const string INTERNAL_FORMATTED_HEADING3 = "FormattedHeading3";
        private const string INTERNAL_FORMATTED_HEADING4 = "FormattedHeading4";
        private const string INTERNAL_FORMATTED_HEADING5 = "FormattedHeading5";

        // browser html constan expressions
        private const string EMPTY_SPACE = @"&nbsp;";
        private const string EMPTY_URL = @"";
        private const string BLANK_HTML_PAGE = "about:blank";
        private const string TARGET_WINDOW_NEW = "_BLANK";
        private const string TARGET_WINDOW_SAME = "_SELF";

        // constants for displaying the HTML dialog
        private const string HTML_TITLE_EDIT = "Edit Html";
        private const string HTML_TITLE_VIEW = "View Html";
        private const string PASTE_TITLE_HTML = "Enter Html";
        private const string PASTE_TITLE_TEXT = "Enter Text";
        private const string HTML_TITLE_OPENFILE = "Open Html File";
        private const string HTML_TITLE_SAVEFILE = "Save Html File";
        private const string HTML_FILTER = "Html files (*.html,*.htm)|*.html;*htm|All files (*.*)|*.*";
        private const string HTML_EXTENSION = "html";
        private const string CONTENT_EDITABLE_INHERIT = "inherit";
        private const string DEFAULT_HTML_TEXT = "";

        // constants for regular expression work
        // BODY_INNER_TEXT_PARSE = @"(<)/*\w*/*(>)";
        // HREF_TEST_EXPRESSION = @"(http|ftp|https):\/\/[\w]+(.[\w]+)([\w\-\.,@?^=%&:/~\+#]*[\w\-\@?^=%&/~\+#])?";
        // BODY_PARSE_EXPRESSION = @"(?<preBody>.*)(?<bodyOpen><body.*?>)(?<innerBody>.*)(?<bodyClose></body>)(?<afterBody>.*)";
        private const string HREF_TEST_EXPRESSION =  @"mailto\:|(news|(ht|f)tp(s?))\:\/\/[\w]+(.[\w]+)([\w\-\.,@?^=%&:/~\+#]*[\w\-\@?^=%&/~\+#])?";
        private const string BODY_PARSE_PRE_EXPRESSION = @"(<body).*?(</body)";
        private const string BODY_PARSE_EXPRESSION = @"(?<bodyOpen>(<body).*?>)(?<innerBody>.*?)(?<bodyClose>(</body\s*>))";
        private const string BODY_DEFAULT_TAG = @"<Body></Body>";
        private const string BODY_TAG_PARSE_MATCH = @"${bodyOpen}${bodyClose}";
        private const string BODY_INNER_PARSE_MATCH = @"${innerBody}";
        private const string CONTENTTYPE_PARSE_EXPRESSION = @"^(?<mainType>\w+)(\/?)(?<subType>\w*)((\s*;\s*charset=)?)(?<charSet>.*)";
        private const string CONTENTTYPE_PARSE_MAINTYPE = @"${mainType}";
        private const string CONTENTTYPE_PARSE_SUBTYPE = @"${subType}";
        private const string CONTENTTYPE_PARSE_CHARSET = @"${charSet}";

        #endregion

        # region Initialization Code

        // browser constants and commands
        private object EMPTY_PARAMETER;

        // acceptable formatting commands
        // in case order to enable binary search
        private readonly string[] formatCommands = new String[] {"Formatted", "Heading 1", "Heading 2", "Heading 3", "Heading 4", "Heading 5", "Normal"};

        // document and body elements
        private mshtmlDocument document;
        private mshtmlBody body;
        private mshtmlStyleSheet stylesheet;
        private mshtmlScriptElement script;
        private volatile bool loading = false;
        private volatile bool codeNavigate = false;
        private volatile bool rebaseUrlsNeeded = false;

        // default values used to reset values
        private Color _defaultBodyBackColor;
        private Color _defaultBodyForeColor;
        private Color _defaultBackColor;
        private HtmlFontProperty _defaultFont;

        // internal property values
        private bool _readOnly;
        private bool _editEnabled; // distinct from readonly
        private bool _toolbarVisible;
        private DockStyle _toolbarDock;
        private string _bodyText;
        private string _bodyHtml;
        private string _bodyUrl;

        // internal body property values
        private Color _bodyBackColor;
        private Color _bodyForeColor;
        private HtmlFontProperty _bodyFont;
        private int[] _customColors;
        private string _imageDirectory;
        private string _htmlDirectory;
        private NavigateActionOption _navigateWindow;
        private DisplayScrollBarOption _scrollBars;
        private string _baseHref;
        private bool _autoWordWrap;
        private byte _borderSize;

        // find and replace internal text range
        private mshtmlTextRange _findRange;

        protected bool IsLoading { get { return loading; } }
        protected bool IsNavigating { get { return codeNavigate; } }
        protected ContextMenuStrip ContextMenu { get { return contextEditor; } }

        /// <summary>
        /// Private control constructor defining all default values
        /// </summary>
        public HtmlEditorControl()
        {
            // This call is required by the Windows.Forms Form Designer.
            InitializeComponent();

            // define the default values
            // browser constants and commands
            EMPTY_PARAMETER = System.Reflection.Missing.Value;

            // default values used to reset values
            _defaultBodyBackColor = Color.White;
            _defaultBodyForeColor = Color.Black;
            _defaultBackColor = SystemColors.ControlLight;
            _defaultFont = new HtmlFontProperty(this.Font);

            // define the default values of the properties
            _readOnly = false;
			_editEnabled = true;
			_toolbarVisible = true;
            _toolbarDock = DockStyle.Bottom;
            _bodyText = DEFAULT_HTML_TEXT;
            _bodyHtml = DEFAULT_HTML_TEXT;
            _bodyBackColor = _defaultBodyBackColor;
            _bodyForeColor = _defaultBodyForeColor;
            _bodyFont = _defaultFont;
            _scrollBars = DisplayScrollBarOption.Auto;
            _imageDirectory = string.Empty;
            _htmlDirectory = string.Empty;
            _navigateWindow = NavigateActionOption.Default;
            _baseHref = string.Empty;
            _autoWordWrap = true;
            _borderSize = DEFAULT_BORDER_SIZE;
            stylesheet = null;
            script = null;

            // define context menu state and size
            this.BackColor = _defaultBackColor;
            this.contextDocumentToolbar.Checked = true;
            this.contextDocumentScrollbar.Checked = true;
            this.contextDocumentWordwrap.Checked = true;
			this.toolstripEnableEditing.Checked = !ReadOnly;

			SetBrowserPanelSize();

            // load the blank Html page to load the MsHtml object model
            BrowserCodeNavigate(BLANK_HTML_PAGE);

            // after load ensure document marked as editable
            this.ReadOnly = _readOnly;
            this.ScrollBars = _scrollBars;

        } //HtmlEditorControl
		
		/// <summary>
		/// Defines all the body attributes once a document has been loaded
		/// </summary>
		private void DefineBodyAttributes()
        {
            // define the body colors based on the new body html
            if (body.bgColor == null)
            {
                _bodyBackColor = _defaultBodyBackColor;
            }
            else
            {
                _bodyBackColor = ColorTranslator.FromHtml((string)body.bgColor);
            }
            if (body.text == null)
            {
                _bodyForeColor = _defaultBodyForeColor;
            }
            else
            {
                _bodyForeColor = ColorTranslator.FromHtml((string)body.text);
            }

            // define the font object based on current font of new document
            // deafult used unless a style on the body modifies the value
            mshtmlStyle bodyStyle = body.style;
            if (bodyStyle != null)
            {
                string fontName = _bodyFont.Name;
                HtmlFontSize fontSize = _bodyFont.Size;
                bool fontBold = _bodyFont.Bold;
                bool fontItalic = _bodyFont.Italic;
                bool fontUnderline = _bodyFont.Underline;
                // define the font name if defined in the style
                if (bodyStyle.fontFamily != null) fontName = bodyStyle.fontFamily;
                if (bodyStyle.fontSize != null) fontSize = HtmlFontConversion.StyleSizeToHtml(bodyStyle.fontSize.ToString());
                if (bodyStyle.fontWeight != null) fontBold = HtmlFontConversion.IsStyleBold(bodyStyle.fontWeight);
                if (bodyStyle.fontStyle != null) fontItalic = HtmlFontConversion.IsStyleItalic(bodyStyle.fontStyle);
                fontUnderline = bodyStyle.textDecorationUnderline;
                // define the new font object and set the property
                _bodyFont = new HtmlFontProperty(fontName, fontSize, fontBold, fontItalic, fontUnderline);
                this.BodyFont = _bodyFont;
            }

            // define the content based on the current value
            this.ReadOnly = _readOnly;
            this.ScrollBars = _scrollBars;
            this.AutoWordWrap = _autoWordWrap;

        } //DefineBodyAttributes

        #endregion
        
        #region Control Methods and Events
    
        /// <summary>
        /// Method to perform the process of showing the context menus
        /// </summary>
        private void DocumentContextMenu(object sender, HtmlElementEventArgs e)
        {
            // if in readonly mode display the standard context menu
            // otherwise display the editing context menu
            if (!editorWebBrowser.IsWebBrowserContextMenuEnabled)
            {
                // should disable inappropriate commands
                if (IsParentTable())
                {
                    this.contextTable.Visible = true;
                }
                else
                {
                    this.contextTable.Visible = false;
                }

                // display the text processing context menu
                this.contextEditor.Show(this.editorWebBrowser, e.MousePosition);

                // cancel the standard menu and event bubbling
                e.BubbleEvent = false;
                e.ReturnValue = false;
            }

        } //DocumentContextMenu

		public int ContentMargin
		{
			set
			{
				if (value >= 0)
				{
					mshtml.IHTMLDocument2 doc = (document as mshtml.IHTMLDocument2);

					// The first parameter is the url, the second is the index of the added style sheet.
					mshtmlStyleSheet ss = doc.createStyleSheet("", 0);

					var rule = String.Format("{{ margin: {0}; }}", value);
					ss.addRule("body", rule);
				}
			}
		}


        /// <summary>
        /// Method to perform the process of selection change
        /// </summary>
        private void DocumentSelectionChange(object sender, EventArgs e)
        {
            // if not in readonly mode process the selection change
            if (!_readOnly && _editEnabled)
            {
                FormatSelectionChange();
            }

        } //DocumentSelectionChange

        private void DocumentLoseFocus(object sender, EventArgs e)
        {
			if (LostFocus != null)
				LostFocus(this, e);
		} //DocumentLoseFocus

        private void DocumentDoubleClick(object sender, EventArgs e)
        {
			SelectWordAtCaret();
		} //DocumentDoubleClick

		private void SelectWordAtCaret()
		{
			mshtmlSelection sel = document.selection as mshtmlSelection;

			if (sel != null)
			{
				mshtmlTextRange rng = sel.createRange() as mshtmlTextRange;

				if ((rng != null) && String.IsNullOrEmpty(rng.text))
				{
					rng.expand("word");

					// Omit trailing whitespace
					if ((rng != null) && !String.IsNullOrEmpty(rng.text))
					{
						int len = rng.text.Length;
						int wordLen = rng.text.TrimEnd(null).Length;

						rng.moveEnd("character", (wordLen - len));
						rng.select();
					}
				}
			}
		}

		/// <summary>
		/// Method to perform the process of key being pressed
		/// </summary>
		private void DocumentKeyPress(object sender, EventArgs e)
        {
            // define the event object being processes and review the key being pressed
            mshtmlEventObject eventObject = document.parentWindow.@event;
            if (eventObject.keyCode == (int)Keys.F5) //Refresh (116)
            {
                // Test the keypress and cancel Refresh
                eventObject.returnValue = false;
                eventObject.keyCode = 0;
            }
            else if (eventObject.keyCode == (int)Keys.F && eventObject.ctrlKey) //Ctrl+F (70)
            {
                // Test the keypress and cancel Refresh
                eventObject.returnValue = false;
                eventObject.keyCode = 0;
                // Show custom Find and Replace dialog
                FindReplacePrompt();
            }

        } //DocumentKeyPress


        /// <summary>
        /// Method used to navigate to the required page
        /// Call made sync using a loading variable
        /// </summary>
        private void BrowserCodeNavigate(string url)
        {
            // once navigated to the href page wait until successful
            // need to do this to ensure properties are all correctly set
            codeNavigate = true;
            loading = true;

            // perform the navigation
            this.editorWebBrowser.Navigate(url);

            // wait for the navigate to complete using the loading variable
            // DoEvents needs to be called to enable the DocumentComplete to execute
            while (loading)
            {
                Application.DoEvents();
                Thread.Sleep(0);
            }

        } //BrowserCodeNavigate


        /// <summary>
        /// Event can be used to canel the navigation and open a new window
        /// If window set to same then nothing happens
        /// </summary>
        private void BrowserBeforeNavigate(object sender, WebBrowserNavigatingEventArgs e)
        {
            string url = e.Url.ToString();
            if (!codeNavigate)
            {
                // call the appropriate event processing
                HtmlNavigationEventArgs navigateArgs = new HtmlNavigationEventArgs(url);
                OnHtmlNavigation(navigateArgs);

                // process the event based on the navigation option
                if (navigateArgs.Cancel)
                {
                    // cancel the navigation
                    e.Cancel = true;
                }
                else if (_navigateWindow == NavigateActionOption.NewWindow)
                {
                    // cancel the current navigation and load url into a new window
                    e.Cancel = true;
                    this.NavigateToUrl(url, true);
                }
                else
                {
                    // continue with current navigation
                    e.Cancel = false;
                }
            }
            else
            {
                // TODO Should ensure the following are no executed for the editor navigation
                //   Scripts
                //   Java
                //   ActiveX Controls
                //   Behaviors
                //   Dialogs

                // continue with current navigation
                e.Cancel = false;
            }
        
        } //BrowserBeforeNavigate


        /// <summary>
        /// Processing for the HtmlNavigation event
        /// </summary>
        private void OnHtmlNavigation(HtmlNavigationEventArgs args)
        {
            if (HtmlNavigation != null)
            {
                HtmlNavigation(this, args);
            }

        } //OnHtmlNavigation

        
        /// <summary>
        /// Document complete method for the web browser
        /// Initiated by navigating to the about:blank page (EMPTY_PARAMETER HTML document)
        /// </summary>
        private void BrowserDocumentComplete(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            // get access to the HTMLDocument
            document = (mshtmlDocument)this.editorWebBrowser.Document.DomDocument;
            body = (mshtmlBody)document.body;

            // COM Interop Start
            // once browsing has completed there is the need to setup some options
            // need to ensure URLs are not modified when html is pasted
            IOleCommandTarget target = null;
            int hResult = HRESULT.S_OK;
            // try to obtain the command target for the web browser document
            try
            {
                // cast the document to a command target
                target = (IOleCommandTarget)document;
                // set the appropriate no url fixups on paste
                hResult = target.Exec(ref CommandGroup.CGID_MSHTML, (int)CommandId.IDM_NOFIXUPURLSONPASTE, (int)CommandOption.OLECMDEXECOPT_DONTPROMPTUSER, ref EMPTY_PARAMETER, ref EMPTY_PARAMETER);
            }
            // catch any exception and map back to the HRESULT
            catch (Exception ex)
            {
                hResult = Marshal.GetHRForException(ex);
            }
            // test the HRESULT for a valid operation
            if (hResult == HRESULT.S_OK)
            {
                // urls will not automatically be rebased
                rebaseUrlsNeeded = false;
            }
            else
            {
                //throw new HtmlEditorException(string.Format("Error executing NOFIXUPURLSONPASTE: Result {0}", hResult));
                rebaseUrlsNeeded = true;
            }
            // COM Interop End

            // at this point the document and body has been loaded
            // so define the event handler for the context menu
            this.editorWebBrowser.Document.ContextMenuShowing += new HtmlElementEventHandler(DocumentContextMenu);
            this.editorWebBrowser.Document.AttachEventHandler("onselectionchange", DocumentSelectionChange);
            this.editorWebBrowser.Document.AttachEventHandler("onkeydown", DocumentKeyPress);
			this.editorWebBrowser.Document.AttachEventHandler("onfocusout", DocumentLoseFocus);
			this.editorWebBrowser.Document.AttachEventHandler("ondblclick", DocumentDoubleClick);

			// signalled complete
			codeNavigate = false;
            loading = false;

            // after navigation define the document Url
            string url = e.Url.ToString();
            _bodyUrl = IsStringEqual(url, BLANK_HTML_PAGE) ? string.Empty : url;

        } //BrowserDocumentComplete

		/// <summary>
		/// Create a new focus method that ensure the body gets the focus
		/// Should be called when text processing command are called
		/// </summary>
		public new bool Focus()
        {
            // have the return value be the focus return from the user control
            bool focus = base.Focus();
            // try to set the focus to the web browser
            try
            {
                this.editorWebBrowser.Focus();
                if (body != null) body.focus();
            }
            catch (Exception)
            {
                // ignore errors
            }
            return focus;

        } //Focus

        #endregion

        #region Runtime Display Properties

        /// <summary>
        /// Defines the base back color of the control
        /// Will override the base back color to alter the default
        /// </summary>
        public override Color BackColor
        {
            get
            {
                return base.BackColor;
            }
            set
            {
                base.BackColor = value;
                this.toolstripEditor.BackColor = value;
                //this.contextEditor.BackColor = value;
            }

        } //BackColor

        /// <summary>
        /// Determines if the property should be serialized
        /// </summary>
        public bool ShouldSerializeBackColor()
        {
            return (base.BackColor != _defaultBackColor);

        } //ShouldSerializeBackColor

        /// <summary>
        /// Resets the property
        /// </summary>
        public override void ResetBackColor()
        {
            base.BackColor = _defaultBackColor;
            this.toolstripEditor.BackColor = _defaultBackColor;

        } //ResetBackColor


        /// <summary>
        /// Property defining the width of the controls border
        /// </summary>
        [Category("RuntimeDisplay"), Description("Controls the Width of the Display Border")]
        [DefaultValue(DEFAULT_BORDER_SIZE)]
        public byte BorderSize
        {
            get
            {
                return _borderSize;
            }
            set
            {
                _borderSize = value;
                SetBrowserPanelSize();
            }

        } //BorderSize


        /// <summary>
        /// Property defining whether scroll bars should be displayed
        /// </summary>
        [Category("RuntimeDisplay"), Description("Controls the Display of Scrolls Bars")]
        [DefaultValue(DisplayScrollBarOption.Auto)]
        public DisplayScrollBarOption ScrollBars
        {
            get
            {
                return _scrollBars;
            }
            set
            {
                _scrollBars = value;
                // define the document scroll bar visibility
                body.scroll = _scrollBars.ToString();
                // define the menu bar state
                this.contextDocumentScrollbar.Checked = (value == DisplayScrollBarOption.No ? false : true);
            }

        } //ScrollBars


        /// <summary>
        /// Property defining whether words will be auto wrapped
        /// </summary>
        [Category("RuntimeDisplay"), Description("Controls the auto wrapping of content")]
        [DefaultValue(true)]
        public bool AutoWordWrap
        {
            get
            {
                return _autoWordWrap;
            }
            set
            {
                _autoWordWrap = value;
                // define the document word wrap property
                body.noWrap = !_autoWordWrap;
                // define the menu bar state
                this.contextDocumentWordwrap.Checked = value;
            }

        } //AutoWordWrap
        
        
        /// <summary>
        /// Property defining the default action when a user click on a link
        /// </summary>
        [Category("RuntimeDisplay"), Description("Window to use when clicking a Href")]
        [DefaultValue(NavigateActionOption.Default)]
        public NavigateActionOption NavigateAction
        {
            get
            {
                return _navigateWindow;
            }
            set
            {
                _navigateWindow = value;
            }

        } //NavigateAction


        /// <summary>
        /// Property defining the editable status of the text
        /// </summary>
        [Category("RuntimeDisplay"), Description("Marks the content as ReadOnly")]
        [DefaultValue(false)]
        public bool ReadOnly
        {
            get
            {
                return _readOnly;
            }
            set
            {
                _readOnly = value;

                // define the document editable property
                body.contentEditable = (!_readOnly && _editEnabled).ToString();

				// define the menu bar state
				UpdateToolbarEnabledState();
				
				// define whether the IE cntext menu should be shown
				this.editorWebBrowser.IsWebBrowserContextMenuEnabled = (_readOnly || !_editEnabled);
			}

        } //ReadOnly

        public bool EditEnabled
        {
            get
            {
                return _editEnabled;
            }
            set
            {
				_editEnabled = value;

                // define the document editable property
                body.contentEditable = (!_readOnly && _editEnabled).ToString();

				// define the menu bar state
				UpdateToolbarEnabledState();
				
				// define whether the IE cntext menu should be shown
				this.editorWebBrowser.IsWebBrowserContextMenuEnabled = (_readOnly || !_editEnabled);
            }

        } //ReadOnly


        /// <summary>
        /// Property defining the visibility of the defined toolbar
        /// </summary>
        [Category("RuntimeDisplay"), Description("Marks the toolbar as Visible")]
        [DefaultValue(true)]
        public bool ToolbarVisible
        {
            get
            {
                return _toolbarVisible;
            }
            set
            {
                _toolbarVisible = value;
                this.toolstripEditor.Visible = _toolbarVisible;
                this.contextDocumentToolbar.Checked = value;
                SetBrowserPanelSize();
            }

        } //ToolbarVisible


        /// <summary>
        /// Property defining the visibility of the defined toolbar
        /// </summary>
        [Category("RuntimeDisplay"), Description("Defines the docking location of the toolbar")]
        [DefaultValue(DockStyle.Bottom)]
        public DockStyle ToolbarDock
        {
            get
            {
                return _toolbarDock;
            }
            set
            {
                if (value != DockStyle.Fill && value != DockStyle.None)
                {
                    _toolbarDock = value;
                    this.toolstripEditor.Dock = _toolbarDock;
                    // also set the panel dock options as not set to fill
                    if (_toolbarDock == DockStyle.Bottom) this.browserPanel.Dock = DockStyle.Top;
                    else if (_toolbarDock == DockStyle.Top) this.browserPanel.Dock = DockStyle.Bottom;
                    else if (_toolbarDock == DockStyle.Left) this.browserPanel.Dock = DockStyle.Right;
                    else if (_toolbarDock == DockStyle.Right) this.browserPanel.Dock = DockStyle.Left;
                    // ensure control is repainted as docking has been modified
                    this.Invalidate();
                    SetBrowserPanelSize();
                }
            }

        } //ToolbarDock

        #endregion

        #region Body Properties (Text and HTML)

        /// <summary>
        /// Property defining the base text for the body
        /// The HTML value can be used at runtime
        /// </summary>
        [Category("Textual"), Description("Set the initial Body Text")]
        [DefaultValue(DEFAULT_HTML_TEXT)]
        public string InnerText
        {
            get
            {
                _bodyText = body.innerText;
                _bodyHtml = body.innerHTML;
                return _bodyText;
            }
            set
            {
                try
                {
                    // clear the defined body url
                    _bodyUrl = string.Empty;
                    if (value == null) value = string.Empty;
                    // set the body property
                    body.innerText = value;
                    // set the body text and html
                    _bodyText = body.innerText;
                    _bodyHtml = body.innerHTML;
                }
                catch (Exception ex)
                {
                    throw new HtmlEditorException("Inner Text for the body cannot be set.", "SetInnerText", ex);
                }

            }

        } //InnerText


        /// <summary>
        /// Property defining the HTML value for the body contents
        /// It is this value that gets serialized by the designer
        /// </summary>
        [Category("Textual"), Description("The Inner HTML of the contents")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), Browsable(false)]
        public string InnerHtml
        {
            get
            {
                _bodyText = body.innerText;
                _bodyHtml = body.innerHTML;
                return _bodyHtml;
            }
            set
            {
                try
                {
                    // clear the defined body url
                    _bodyUrl = string.Empty;
                    if (value == null) value = string.Empty;
                    // set the body property
                    body.innerHTML = value;
                    // set the body text and html
                    _bodyText = body.innerText;
                    _bodyHtml = body.innerHTML;
                    // if needed rebase urls
                    RebaseAnchorUrl();
                }
                catch (Exception ex)
                {
                    throw new HtmlEditorException("Inner Html for the body cannot be set.", "SetInnerHtml", ex);
                }
            }

        } //InnerHtml


        /// <summary>
        /// Property defining the body tag of the html
        /// On set operation the body attributes are redefined
        /// </summary>
        [Category("Textual"), Description("Complete Document including Body Tag")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), Browsable(false)]
        public string BodyHtml
        {
            get
            {
                // set the read only property before return
                body.contentEditable = CONTENT_EDITABLE_INHERIT;
                string html = body.outerHTML.Trim();
                this.ReadOnly = _readOnly;
                return html;

            }
            set
            {
                // clear the defined body url
                _bodyUrl = string.Empty;

                // define some local working variables
                string bodyElement = string.Empty;
                string innerHtml = string.Empty;

                try
                {
                    // ensure have body open and close tags
                    if (Regex.IsMatch(value, BODY_PARSE_PRE_EXPRESSION, RegexOptions.IgnoreCase | RegexOptions.Multiline | RegexOptions.Singleline))
                    {
                        // define a regular expression for the Html Body parsing and obtain the match expression
                        Regex expression = new Regex(BODY_PARSE_EXPRESSION,  RegexOptions.IgnoreCase | RegexOptions.Multiline | RegexOptions.Singleline | RegexOptions.Compiled | RegexOptions.ExplicitCapture);
                        Match match = expression.Match(value);
                        // see if a match was found
                        if (match.Success)
                        {
                            // extract the body tag and the inner html
                            bodyElement = match.Result(BODY_TAG_PARSE_MATCH);
                            innerHtml = match.Result(BODY_INNER_PARSE_MATCH);
                            // remove whitespaces from the body and inner html tags
                            bodyElement = bodyElement.Trim();
                            innerHtml = innerHtml.Trim();
                        }
                    }
                    // ensure body was set
                    if (bodyElement == string.Empty)
                    {				
                        // assume the Html given is an inner html with no body
                        bodyElement = BODY_DEFAULT_TAG;
                        innerHtml = value.Trim();
                    }

                    // first navigate to a blank page to reset the html header
                    BrowserCodeNavigate(BLANK_HTML_PAGE);

                    // replace the body tag with the one passed in
                    mshtmlDomNode oldBodyNode = (mshtmlDomNode)document.body;
                    mshtmlDomNode newBodyNode = (mshtmlDomNode)document.createElement(bodyElement);
                    oldBodyNode.replaceNode(newBodyNode);

                    // define the new inner html and body objects
                    body = (mshtmlBody)document.body;
                    body.innerHTML = innerHtml;

                    // now all successfully loaded need to review the body attributes
                    _bodyText = body.innerText;
                    _bodyHtml = body.innerHTML;

                    // set and define the appropriate properties
                    // this will set the appropriate read only property
                    DefineBodyAttributes();

                    // if needed rebase urls
                    RebaseAnchorUrl();
                }
                catch (Exception ex)
                {
                    throw new HtmlEditorException("Outer Html for the body cannot be set.", "SetBodyHtml", ex);
                }
            }

        } //BodyHtml


        /// <summary>
        /// Property defining the Html tag of the document
        /// Should never be set as contains the HEAD tag
        /// </summary>
        [Category("Textual"), Description("Complete Document including Head and Body Tag")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), Browsable(false)]
        public string DocumentHtml
        {
            get
            {
                return document.documentElement.outerHTML.Trim();
            }

        } //DocumentHtml


        /// <summary>
        /// Property defining the Text selected by the user
        /// </summary>
        [Category("Textual"), Description("The Text selected by the User")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), Browsable(false)]
        public string SelectedText
        {
            get
            {
                // obtain the selected range object
                mshtmlTextRange range = GetTextRange();
                // return the text of the range
                if (range.text != null) return range.text;
                else return string.Empty;
            }
            set
            {
                try
                {
                    // obtain the selected range object
                    mshtmlTextRange range = GetTextRange();
                    // set the text range
                    if (range != null)
                    {
                        // encode the text to encode any html markup
                        string html = HttpUtility.HtmlEncode(value);
                        // once have a range execute the pasteHtml command
                        // this will overwrite the current selection
                        range.pasteHTML(html);
                    }
                }
                catch (Exception ex)
                {
                    throw new HtmlEditorException("Inner Text for the selection cannot be set.", "SetSelectedText", ex);
                }
            }

        } //SelectedText


        /// <summary>
        /// Property defining the Html selected by the user
        /// </summary>
        [Category("Textual"), Description("The Text selected by the User")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), Browsable(false)]
        public string SelectedHtml
        {
            get
            {
                // obtain the selected range object
                mshtmlTextRange range = GetTextRange();
                // return the text of the range
                if (range.text != null) return range.htmlText;
                else return string.Empty;
            }
            set
            {
                try
                {
                    // obtain the selected range object
                    mshtmlTextRange range = GetTextRange();
                    // set the text range
                    if (range != null)
                    {
                        // once have a range execute the pasteHtml command
                        // this will overwrite the current selection
                        range.pasteHTML(value);
                        // if needed rebase urls
                        RebaseAnchorUrl();
                    }
                }
                catch (Exception ex)
                {
                    throw new HtmlEditorException("Inner Html for the selection cannot be set.", "SetSelectedHtml", ex);
                }
            }

        } //SelectedHtml


        /// <summary>
        /// Property returning any Url that was used to load the current document
        /// </summary>
        [Category("Textual"), Description("Url used to load the Document")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), Browsable(false)]
        public string DocumentUrl
        {
            get
            {
                //return document.baseUrl;
                return _bodyUrl;
            }
        }

        #endregion

        #region Body Properties (Font and Color)

        /// <summary>
        /// Property for the body background color
        /// Reset and Serialize values defined
        /// </summary>
        [Category("Textual"), Description("Defines the Background Color of the Body")]
        public Color BodyBackColor
        {
            get
            {
                return _bodyBackColor;
            }
            set
            {
                // set the new value using a default if Empty passed in
                if (value != Color.Empty) _bodyBackColor = value;
                else _bodyBackColor = _defaultBodyBackColor;
                // set the bgcolor attribute of the body
                if (body != null)
                {
                    if (_bodyBackColor.ToArgb() == _defaultBodyBackColor.ToArgb())
                    {
                        document.bgColor = string.Empty;
                    }
                    else
                    {
                        document.bgColor = ColorTranslator.ToHtml(_bodyBackColor);
                    }
                }
            }

        } //BodyBackColor

        public bool ShouldSerializeBodyBackColor()
        {
            return (_bodyBackColor != _defaultBodyBackColor);

        } //ShouldSerializeBodyBackColor

        public void ResetBodyBackColor()
        {
            this.BodyBackColor = _defaultBodyBackColor;

        } //ResetBodyBackColor


        /// <summary>
        /// Property for the body fore color
        /// Reset and Serialize values defined
        /// </summary>
        [Category("Textual"), Description("Defines the Foreground Color of the Body")]
        public Color BodyForeColor
        {
            get
            {
                return _bodyForeColor;
            }
            set
            {
                // set the new value using a default if Empty passed in
                if (value != Color.Empty) _bodyForeColor = value;
                else _bodyForeColor = _defaultBodyForeColor;
                // set the text attribute of the body
                if (body != null)
                {
                    if (_bodyForeColor.ToArgb() == _defaultBodyForeColor.ToArgb())
                    {
                        document.fgColor = string.Empty;
                    }
                    else
                    {
                        document.fgColor = ColorTranslator.ToHtml(_bodyForeColor);
                    }
                }
            }

        } //BodyForeColor

        public bool ShouldSerializeBodyForeColor()
        {
            return (_bodyForeColor != _defaultBodyForeColor);

        } //ShouldSerializeBodyForeColor

        public void ResetBodyForeColor()
        {
            this.BodyForeColor = _defaultBodyForeColor;

        } //ResetBodyForeColor


        /// <summary>
        /// Property for the base font to use for text editing
        /// Reset and Serialize values defined
        /// </summary>
        [Category("Textual"), Description("Defines the base Font object for the Body")]
        [RefreshProperties(RefreshProperties.All)]
        public HtmlFontProperty BodyFont
        {
            get
            {
                return _bodyFont;
            }
            set
            {
                // set the new value using the default if set to null
                if (HtmlFontProperty.IsNotNull(value)) _bodyFont = value;
                else _bodyFont = _defaultFont;

                // set the font attributes based on any body styles
                mshtmlStyle bodyStyle = body.style;

                if (bodyStyle != null)
                {
                    if (HtmlFontProperty.IsEqual(_bodyFont, _defaultFont))
                    {
                        // ensure no values are set in the Body style
                        if (bodyStyle.fontFamily != null) bodyStyle.fontFamily = string.Empty;
                        if (bodyStyle.fontSize != null) bodyStyle.fontSize = string.Empty;
                        if (bodyStyle.fontWeight != null) bodyStyle.fontWeight = string.Empty;
					}
					else
                    {
                        // set the body styles based on the defined value
                        bodyStyle.fontFamily = _bodyFont.Name;
                        bodyStyle.fontWeight = HtmlFontConversion.HtmlFontBoldString(_bodyFont.Bold);

						if (_bodyFont.SizeInEms > 0)
							bodyStyle.fontSize = String.Format("{0}em", _bodyFont.SizeInEms);
						else
							bodyStyle.fontSize = HtmlFontConversion.HtmlFontSizeString(_bodyFont.Size);
                    }
                }
            }

        } //BodyFont

        public bool ShouldSerializeBodyFont()
        {
            return (HtmlFontProperty.IsNotEqual(_bodyFont, _defaultFont));

        } //ShouldSerializeBodyFont

        public void ResetBodyFont()
        {
            this.BodyFont = _defaultFont;

        } //ResetBodyFont


        /// <summary>
        /// Property defining the style sheet to be used for editing
        /// Can be used for standard templates
        /// </summary>
        [Category("Textual"), Description("Stylesheet Url used to display the Html")]
        [DefaultValue(EMPTY_URL)]
        public string StylesheetUrl
        {
            // [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), Browsable(false)]
            get
            {
                if (stylesheet != null && stylesheet.href != null) return stylesheet.href;
                else return string.Empty;
            }
            set
            {
                if (stylesheet != null)
                {
                    // if (document.styleSheets.length > 0)
                    // Create an IEnumerator for the cells of the row
                    // IEnumerator enumerator = document.styleSheets.GetEnumerator();
                    // enumerator.MoveNext();
                    // stylesheet = (mshtmlStyleSheet)enumerator.Current;
                    stylesheet.href = value;
                }
                else
                {
                    // do not create if the value is null or empty
                    if (value != null && value != string.Empty)
                    {
                        stylesheet = (mshtmlStyleSheet)document.createStyleSheet(value, 0);
                    }
                }
            }

        } //StylesheetUrl


        /// <summary>
        /// Property defining a script element that is to be used by all documents
        /// Can be used for document processing
        /// </summary>
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), Browsable(false)]
        [Category("Textual"), Description("Script used to display the Html")]
        [DefaultValue(EMPTY_URL)]
        public string ScriptSource
        {
            get
            {
                if (script != null) return script.src;
                else return string.Empty;
            }
            set
            {
                if (script != null)
                {
                    script.src = value;
                }
                else
                {
                    // create the script element
                    script = (mshtmlScriptElement)document.createElement(SCRIPT_TAG);
                    script.src = value;
                    script.defer = true;
                    // insert into the document
                    mshtmlDomNode node = (mshtmlDomNode)document.documentElement;
                    node.appendChild((mshtmlDomNode)script);
                }
            }

        } //ScriptSource

        #endregion

        #region Document Processing Operations

        /// <summary>
        /// Method to allow the user to load a document by navigation
        /// </summary>
        public void NavigateToUrl(string url)
        {
            // load the requested use Url
            BrowserCodeNavigate(url);

            // now all successfully laoded need to review the body attributes
            _bodyText = body.innerText;
            _bodyHtml = body.innerHTML;

            // set and define the appropriate properties
            DefineBodyAttributes();

        } //NavigateToUrl

        /// <summary>
        /// Method to allow the user to load a document by navigation
        /// A new window can optionally be specified
        /// </summary>
        public void NavigateToUrl(string url, bool newWindow)
        {
            if (newWindow)
            {
                // open the Url in a new window
                this.editorWebBrowser.Navigate(url, true);
            }
            else
            {
                // if no new window required call the normal navigate method
                NavigateToUrl(url);
            }

        } //NavigateToUrl


        /// <summary>
        /// Method to allow the user to load a document from a Url
        /// The body tag is used and loaded
        /// </summary>
        public void LoadFromUrl(string url)
        {

            HttpWebRequest webRqst = null;
            HttpWebResponse webResp = null;
            Stream stream = null;
            // load the body at the given url into the html editor
            try
            {
                // Creates an HttpWebRequest with the specified URL. 
                webRqst = (HttpWebRequest)WebRequest.Create(url); 
                // setup default credentials since may be in an intranet
                webRqst.Credentials = CredentialCache.DefaultCredentials;
                // send the HttpWebRequest and waits for the response.            
                webResp = (HttpWebResponse)webRqst.GetResponse();

                // parse the content type to determine the maintype and subtype
                string contenttype = webResp.ContentType;
                string maintype = string.Empty;
                string subtype = string.Empty;
                string charset = string.Empty;
                Regex expression = new Regex(CONTENTTYPE_PARSE_EXPRESSION,  RegexOptions.IgnoreCase | RegexOptions.Compiled | RegexOptions.ExplicitCapture);
                Match match = expression.Match(contenttype);
                // see if a match was found
                if (match.Success)
                {
                    // extract the content type elements
                    maintype = match.Result(CONTENTTYPE_PARSE_MAINTYPE);
                    subtype = match.Result(CONTENTTYPE_PARSE_SUBTYPE);
                    charset = match.Result(CONTENTTYPE_PARSE_CHARSET);
                }

                // retrieves the text if the content type is of text/html.
                if (IsStringEqual(maintype, "text") && IsStringEqual(subtype, "html")) 
                {
                    // define the encoder to use
                    Encoding encoder;
                    if (charset == string.Empty) charset = @"utf-8";
                    try
                    {
                        // base the encoder from the charset calculated
                        encoder = Encoding.GetEncoding(charset);
                    }
                    catch (Exception)
                    {
                        // use a default UTF8 encoder
                        encoder = Encoding.UTF8;
                    }
                    // read the response stream
                    StringBuilder html = new StringBuilder();
                    // read the response buffer and return the string representation
                    stream = webResp.GetResponseStream();
                    byte[] outputBuffer = new byte[HTML_BUFFER_SIZE];
                    // read the stream in buffer size blocks
                    int bytesRead = 0;
                    do
                    {
                        bytesRead = stream.Read(outputBuffer, 0, HTML_BUFFER_SIZE);
                        if (bytesRead > 0)
                        {
                            html.Append(encoder.GetString(outputBuffer, 0, bytesRead));
                        }
                    } while (bytesRead > 0);

                    // populate the string value from the text
                    this.BodyHtml = html.ToString();
                }
                else
                {
                    // navigated to a none Html document so throw exception
                    throw new HtmlEditorException(string.Format("Not a Html Document: {0}", url), "LoadFromUrl");
                }    
            }
            catch (HtmlEditorException ex)
            {
                // cannot load so throw an exception
                throw ex;
            }
            catch (WebException ex)
            {
                // cannot load so throw an exception
                throw new HtmlEditorException(string.Format("Cannot load Url: {0}", url), "LoadFromUrl", ex);
            }
            catch (Exception ex)
            {
                // cannot load so throw an exception
                throw new HtmlEditorException(string.Format("Cannot load Document: {0}", url), "LoadFromUrl", ex);
            }
            finally
            {
                // close the stream reader
                if (stream != null) stream.Close();
                // close the web response
                if (webResp != null) webResp.Close();
            }

        } //LoadFromUrl


        /// <summary>
        /// Method to allow a user to load a file given a file name
        /// </summary>
        public void LoadFromFile(string filename)
        {
            // init the container for the Html
            string contents = string.Empty;

            // check to see if the file exists
            if (!File.Exists(filename)) 
            {
                throw new HtmlEditorException("Not a valid file name.", "LoadFromFile");
            }

            // read the file contents
            //using (StreamReader reader = File.OpenText(filename))
            using (StreamReader reader = new StreamReader(filename, Encoding.UTF8))
            {
                contents = reader.ReadToEnd();
            }
            // if the contents where successfully read write to document
            if (contents != String.Empty)
            {
                this.BodyHtml = contents;
            }

        } //LoadFromFile

        
        /// <summary>
        /// Method to allow the user to select a file and read the contents into the Html stream
        /// </summary>
        public void OpenFilePrompt()
        {
            // init the container for the Html
            string contents = string.Empty;

            // create an open file dialog
            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                // init the input stream
                Stream stream = null;
                // define the dialog structure
                dialog.DefaultExt = HTML_EXTENSION;
                dialog.Title = HTML_TITLE_OPENFILE;
                dialog.AddExtension = true;
                dialog.Filter = HTML_FILTER;
                dialog.FilterIndex = 1;
                dialog.RestoreDirectory = true;
                if (_htmlDirectory != String.Empty) dialog.InitialDirectory = _htmlDirectory; 
                // show the dialog and see if the users enters OK
                if(dialog.ShowDialog() == DialogResult.OK)
                {
                    // look to see if the file contains any contents and stream
                    if((stream = dialog.OpenFile()) != null)
                    {
                        using (StreamReader reader = new StreamReader(stream, Encoding.UTF8))
                        {
                            // get the contents as text
                            contents = reader.ReadToEnd();
                            // persist the path navigation
                            _htmlDirectory = Path.GetDirectoryName(dialog.FileName);
                        }
                    }
                    // close the input stream
                    if (stream != null) stream.Close();
                }
            }
            // if the contents where successfully read write to document
            if (contents != String.Empty)
            {
                this.BodyHtml = contents;
            }

        } //OpenFilePrompt


        /// <summary>
        /// Method to allow the user to persist the Html stream to a file
        /// </summary>
        public void SaveFilePrompt()
        {
            // obtain the html contents
            string contents = this.BodyHtml;
            // create a file save dialog
            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                // init the outpu stream
                Stream stream = null;
                // define the dialog structure
                dialog.DefaultExt = HTML_EXTENSION;
                dialog.Title = HTML_TITLE_SAVEFILE;
                dialog.AddExtension = true;
                dialog.Filter = HTML_FILTER;
                dialog.FilterIndex = 1;
                dialog.RestoreDirectory = true;
                if (_htmlDirectory != String.Empty) dialog.InitialDirectory = _htmlDirectory; 
                // show the dialog and see if the users enters OK
                if(dialog.ShowDialog() == DialogResult.OK)
                {
                    // look to see if the stream can be open for output
                    if((stream = dialog.OpenFile()) != null)
                    {
                        // create the stream writer for the html
                        using (StreamWriter writer = new StreamWriter(stream))
                        {
                            // write out the body contents to the stream
                            writer.Write(contents);
                            writer.Flush();
                            // persist the path navigation
                            _htmlDirectory = Path.GetDirectoryName(dialog.FileName);
                        }
                    }
                    // close the input stream
                    if (stream != null) stream.Close();
                }
            }

        } //SaveFilePrompt

        
        /// <summary>
        /// Method to allow the user to edit the raw HTML
        /// Dialog presented and the body contents set
        /// </summary>
        public void HtmlContentsEdit()
        {
            using (EditHtmlForm dialog = new EditHtmlForm())
            {
                dialog.HTML = this.InnerHtml;
                dialog.ReadOnly = false;
                dialog.SetCaption(HTML_TITLE_EDIT);
                PreShowDialog(dialog);
                if (dialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
                {
					PostShowDialog(dialog);
                    this.InnerHtml = dialog.HTML;
                }
            }

        } //HtmlContentsEdit

        /// <summary>
        /// Method to allow the user to view the html contents
        /// The complete Html markup is presented
        /// </summary>
        public void HtmlContentsView()
        {
            using (EditHtmlForm dialog = new EditHtmlForm())
            {
                dialog.HTML = this.DocumentHtml;
                dialog.ReadOnly = true;
                dialog.SetCaption(HTML_TITLE_VIEW);
                PreShowDialog(dialog);
                dialog.ShowDialog(/*this.ParentForm*/);
				PostShowDialog(dialog);
			}

		} //HtmlContentsView


        /// <summary>
        /// Method to print the html text using the document print command
        /// Print preview is not supported
        /// </summary>
        public void DocumentPrint()
        {
            ExecuteCommandDocument(HTML_COMMAND_TEXT_PRINT);

        } //TextPrint


        /// <summary>
        /// Method to toggle the overwrite mode
        /// </summary>
        public void ToggleOverWrite()
        {
            ExecuteCommandDocument(HTML_COMMAND_OVERWRITE);

        } //ToggleOverWrite

        #endregion

        #region Document Text Operations

        /// <summary>
        /// Method to cut the currently selected text to the clipboard
        /// </summary>
        public void TextCut()
        {
            //this.editorWebBrowser.ExecWB(SHDocVw.OLECMDID.OLECMDID_CUT, PROMPT_USER_NO, ref EMPTY_PARAMETER, ref EMPTY_PARAMETER);
            ExecuteCommandDocument(HTML_COMMAND_TEXT_CUT);

        } //TextCut

        /// <summary>
        /// Method to copy the currently selected text to the clipboard
        /// </summary>
        public void TextCopy()
        {
            //this.editorWebBrowser.ExecWB(SHDocVw.OLECMDID.OLECMDID_COPY, PROMPT_USER_NO, ref EMPTY_PARAMETER, ref EMPTY_PARAMETER);
            ExecuteCommandDocument(HTML_COMMAND_TEXT_COPY);

        } //TextCopy

        /// <summary>
        /// Method to paste the currently selected text from the clipboard
        /// </summary>
        public void TextPaste()
        {
            //this.editorWebBrowser.ExecWB(SHDocVw.OLECMDID.OLECMDID_PASTE, PROMPT_USER_NO, ref EMPTY_PARAMETER, ref EMPTY_PARAMETER);
            ExecuteCommandDocument(HTML_COMMAND_TEXT_PASTE);

        } //TextPaste

        /// <summary>
        /// Method to delete the currently selected text from the screen
        /// </summary>
        public void TextDelete()
        {
            //this.editorWebBrowser.ExecWB(SHDocVw.OLECMDID.OLECMDID_DELETE, PROMPT_USER_NO, ref EMPTY_PARAMETER, ref EMPTY_PARAMETER);
            ExecuteCommandDocument(HTML_COMMAND_TEXT_DELETE);

        } //TextDelete

        /// <summary>
        /// Method to select the entire document contents
        /// </summary>
        public void TextSelectAll()
        {
            //this.editorWebBrowser.ExecWB(SHDocVw.OLECMDID.OLECMDID_SELECTALL, PROMPT_USER_NO, ref EMPTY_PARAMETER, ref EMPTY_PARAMETER);
            ExecuteCommandDocument(HTML_COMMAND_TEXT_SELECT_ALL);

        } //TextSelectAll

        /// <summary>
        /// Method to clear the document selection
        /// </summary>
        public void TextClearSelect()
        {
            //this.editorWebBrowser.ExecWB(SHDocVw.OLECMDID.OLECMDID_CLEARSELECTION, PROMPT_USER_NO, ref EMPTY_PARAMETER, ref EMPTY_PARAMETER);
            ExecuteCommandDocument(HTML_COMMAND_TEXT_UNSELECT);

        } //TextClearSelect


        /// <summary>
        /// Method to undo former commands
        /// </summary>
        public bool EditUndo()
        {
            //this.editorWebBrowser.ExecWB(SHDocVw.OLECMDID.OLECMDID_UNDO, PROMPT_USER_NO, ref EMPTY_PARAMETER, ref EMPTY_PARAMETER);
            return ExecuteCommandDocument(HTML_COMMAND_TEXT_UNDO);

        } //EditUndo

        /// <summary>
        /// Method to undo former commands
        /// </summary>
        public bool CanEditUndo()
        {
            return ExecuteQueryDocument(HTML_COMMAND_TEXT_UNDO);

        } //EditUndo

        /// <summary>
        /// Method to undo former commands
        /// </summary>
        public bool EditRedo()
        {
            //this.editorWebBrowser.ExecWB(SHDocVw.OLECMDID.OLECMDID_UNDO, PROMPT_USER_NO, ref EMPTY_PARAMETER, ref EMPTY_PARAMETER);
            return ExecuteCommandDocument(HTML_COMMAND_TEXT_REDO);

        } //EditUndo

        /// <summary>
        /// Method to redo former undo
        /// </summary>
        public bool CanEditRedo()
        {
            return ExecuteQueryDocument(HTML_COMMAND_TEXT_REDO);

        } //EditRedo

        #endregion

        #region Selected Text Formatting Operations

        /// <summary>
        /// Ensures the toolbar is correctly displaying state
        /// </summary>
        private void FormatSelectionChange()
        {
            this.toolstripEditUndo.Enabled = CanEditUndo();
            this.contextEditUndo.Enabled = CanEditUndo();
			
            // review the bold state of the selected text
            if (ExecuteCommandQuery(HTML_COMMAND_BOLD))
            {
                this.toolstripFormatBold.Checked = true;
                this.contextFormatBold.Checked = true;
            }
            else
            {
                this.toolstripFormatBold.Checked = false;
                this.contextFormatBold.Checked = false;
            }
            // review the italic state of the selected text
            if (ExecuteCommandQuery(HTML_COMMAND_ITALIC))
            {
                this.toolstripFormatItalic.Checked = true;
                this.contextFormatItalic.Checked = true;
            }
            else
            {
                this.toolstripFormatItalic.Checked = false;
                this.contextFormatItalic.Checked = false;
            }
            // review the underline state of the selected text
            if (ExecuteCommandQuery(HTML_COMMAND_UNDERLINE))
            {
                this.toolstripFormatUnderline.Checked = true;
                this.contextFormatUnderline.Checked = true;
            }
            else
            {
                this.toolstripFormatUnderline.Checked = false;
                this.contextFormatUnderline.Checked = false;
            }
            // review the subscript state of the selected text
            if (ExecuteCommandQuery(HTML_COMMAND_SUBSCRIPT))
            {
                this.contextFormatSubscript.Checked = true;
            }
            else
            {
                this.contextFormatSubscript.Checked = false;
            }
            // review the superscript state of the selected text
            if (ExecuteCommandQuery(HTML_COMMAND_SUPERSCRIPT))
            {
                this.contextFormatSuperscript.Checked = true;
            }
            else
            {
                this.contextFormatSuperscript.Checked = false;
            }
            // review the subscript state of the selected text
            if (ExecuteCommandQuery(HTML_COMMAND_STRIKE_THROUGH))
            {
                this.contextFormatStrikeout.Checked = true;
            }
            else
            {
                this.contextFormatStrikeout.Checked = false;
            }

        } //FormatSelectionChange

		protected void UpdateToolbarEnabledState()
		{
			this.toolstripTextCut.Enabled = (!_readOnly && _editEnabled);
            this.toolstripTextCopy.Enabled = (!_readOnly && _editEnabled);
            this.toolstripTextPaste.Enabled = (!_readOnly && _editEnabled);
            this.toolstripEditUndo.Enabled = (!_readOnly && _editEnabled);
            this.toolstripEditRedo.Enabled = (!_readOnly && _editEnabled);
            this.toolstripFormatBold.Enabled = (!_readOnly && _editEnabled);
            this.toolstripFormatUnderline.Enabled = (!_readOnly && _editEnabled);
            this.toolstripFormatItalic.Enabled = (!_readOnly && _editEnabled);
            this.toolstripFontDialog.Enabled = (!_readOnly && _editEnabled);
            this.toolstripFontNormal.Enabled = (!_readOnly && _editEnabled);
            this.toolstripColorDialog.Enabled = (!_readOnly && _editEnabled);
            this.toolstripFontIncrease.Enabled = (!_readOnly && _editEnabled);
            this.toolstripFontDecrease.Enabled = (!_readOnly && _editEnabled);
            this.toolstripJustifyLeft.Enabled = (!_readOnly && _editEnabled);
            this.toolstripJustifyCenter.Enabled = (!_readOnly && _editEnabled);
            this.toolstripJustifyRight.Enabled = (!_readOnly && _editEnabled);
            this.toolstripFontIndent.Enabled = (!_readOnly && _editEnabled);
            this.toolstripFontOutdent.Enabled = (!_readOnly && _editEnabled);
            this.toolstripListOrdered.Enabled = (!_readOnly && _editEnabled);
            this.toolstripListUnordered.Enabled = (!_readOnly && _editEnabled);
            this.toolstripInsertLine.Enabled = (!_readOnly && _editEnabled);
            this.toolstripInsertTable.Enabled = (!_readOnly && _editEnabled);
            this.toolstripInsertImage.Enabled = (!_readOnly && _editEnabled);
            this.toolstripInsertLink.Enabled = (!_readOnly && _editEnabled);
            this.toolstripFindReplace.Enabled = (!_readOnly && _editEnabled);

            this.toolstripEnableEditing.Enabled = !_readOnly;
			this.toolstripEnableEditing.Checked = (!_readOnly && _editEnabled);

			this.toolstripDocumentPrint.Enabled = true; // always
		}

	/// <summary>
	/// Method using the document to set the font name
	/// </summary>
	public void FormatFontName(string name)
        {
            ExecuteCommandRange(HTML_COMMAND_FONT_NAME, name);

        } //FormatFontName

        /// <summary>
        /// Method using the document to set the Html font size
        /// </summary>
        public void FormatFontSize(HtmlFontSize size)
        {
            ExecuteCommandRange(HTML_COMMAND_FONT_SIZE, (int)size);

        } //FormatFontSize


        /// <summary>
        /// Method using the document to toggle the selection with a bold tag
        /// </summary>
        public void FormatBold()
        {
            ExecuteCommandRange(HTML_COMMAND_BOLD, null);
            FormatSelectionChange();

        } //FormatBold

        /// <summary>
        /// Method using the document to toggle the selection with a underline tag
        /// </summary>
        public void FormatUnderline()
        {
            ExecuteCommandRange(HTML_COMMAND_UNDERLINE, null);
            FormatSelectionChange();

        } //FormatUnderline

        /// <summary>
        /// Method using the document to toggle the selection with a italic tag
        /// </summary>
        public void FormatItalic()
        {
            ExecuteCommandRange(HTML_COMMAND_ITALIC, null);
            FormatSelectionChange();

        } //FormatItalic

        /// <summary>
        /// Method using the document to toggle the selection with a Subscript tag
        /// </summary>
        public void FormatSubscript()
        {
            ExecuteCommandRange(HTML_COMMAND_SUBSCRIPT, null);
            FormatSelectionChange();

        } //FormatSubscript

        /// <summary>
        /// Method using the document to toggle the selection with a Superscript tag
        /// </summary>
        public void FormatSuperscript()
        {
            ExecuteCommandRange(HTML_COMMAND_SUPERSCRIPT, null);
            FormatSelectionChange();

        } //FormatSuperscript

        /// <summary>
        /// Method using the document to toggle the selection with a Strikeout tag
        /// </summary>
        public void FormatStrikeout()
        {
            ExecuteCommandRange(HTML_COMMAND_STRIKE_THROUGH, null);
            FormatSelectionChange();

        } //FormatStrikeout


        /// <summary>
        /// Method to increase the size of the font by 1 point
        /// </summary>
        public void FormatFontIncrease()
        {
            FormatFontChange(1);

        } //FormatFontIncrease

        /// <summary>
        /// Method to decrease the size of the font by 1 point
        /// </summary>
        public void FormatFontDecrease()
        {
            FormatFontChange(-1);

        } //FormatFontDecrease

        /// <summary>
        /// Method given a int value increase the font size by that amount
        /// </summary>
        private void FormatFontChange(int change)
        {
            // ensure the command is acceptable
            if (Math.Abs(change) > 6)
            {
                throw new HtmlEditorException("Value can only be in the range [1,6].", "FontIncreaseDecrease");
            }
            else
            {
                // obtain the selected range object
                mshtmlTextRange range = GetTextRange();

                // obtain the original font value
                object fontSize = QueryCommandRange(range, HTML_COMMAND_FONT_SIZE);
                int oldFontSize = (fontSize is System.Int32) ? (int)fontSize : (int)_bodyFont.Size;
                // calc the new font size and modify
                int newFontSize = oldFontSize + change;
                ExecuteCommandRange(range, HTML_COMMAND_FONT_SIZE, newFontSize);
            }

        } //FormatFontChange


        /// <summary>
        /// Method to remove any formatting tags
        /// </summary>
        public void FormatRemove()
        {
            ExecuteCommandRange(HTML_COMMAND_REMOVE_FORMAT, null);

        } //FormatRemove


        /// <summary>
        /// Method to Tab the current line to the right
        /// </summary>
        public void FormatTabRight()
        {
            ExecuteCommandRange(HTML_COMMAND_INDENT, null);

        } //FormatTabRight

        /// <summary>
        /// Method to Tab the current line to the left
        /// </summary>
        public void FormatTabLeft()
        {
            ExecuteCommandRange(HTML_COMMAND_OUTDENT, null);

        } //FormatTabLeft


        /// <summary>
        /// Method to insert an ordered or unordered list
        /// </summary>
        /// <param name="listtype"></param>
        public void FormatList(HtmlListType listtype)
        {
            switch (listtype)
            {
                case HtmlListType.Ordered:
                    ExecuteCommandRange(HTML_COMMAND_INSERT_LISTORDERED, null);
                    break;

                case HtmlListType.Unordered:
                    ExecuteCommandRange(HTML_COMMAND_INSERT_LISTUNORDERED, null);
                    break;
            }
        } //FormatList


        /// <summary>
        /// Method to define the font justification as LEFT
        /// </summary>
        public void JustifyLeft()
        {
            ExecuteCommandRange(HTML_COMMAND_JUSTIFY_LEFT, null);

        } //JustifyLeft

        /// <summary>
        /// Method to define the font justification as CENTER
        /// </summary>
        public void JustifyCenter()
        {
            ExecuteCommandRange(HTML_COMMAND_JUSTIFY_CENTER, null);

        } //JustifyCenter

        /// <summary>
        /// Method to define the font justification as Right
        /// </summary>
        public void JustifyRight()
        {
            ExecuteCommandRange(HTML_COMMAND_JUSTIFY_RIGHT, null);

        } //JustifyRight

        #endregion

        #region Object Insert Operations

        /// <summary>
        /// Method to insert a horizontal line in the body
        /// If have a control range rather than text range one could overwrite the controls with the line
        /// </summary>
        public void InsertLine()
        {
            mshtmlTextRange range = GetTextRange();
            if (range != null)
            {
                ExecuteCommandRange(range, HTML_COMMAND_INSERT_LINE, null);
            }
            else
            {
                throw new HtmlEditorException("Invalid Selection for Line insertion.", "InsertLine");
            }

        } //InsertLine


        /// <summary>
        /// Method to insert an image tag at the selected location
        /// </summary>
        public void InsertImage(string imageLocation)
        {
            ExecuteCommandRange(HTML_COMMAND_INSERT_IMAGE, imageLocation);

        } //InsertImage

        /// <summary>
        /// Method to insert a image and prompt a user for the link
        /// Calls the public InsertImage method
        /// </summary>
        public void InsertImagePrompt()
        {
            // set default image and text tags
            string imageText = string.Empty;
            string imageHref = string.Empty;
            ImageAlignOption imageAlign = ImageAlignOption.Left;
            mshtmlElement control = null;

            // look to see if an image has been selected
            control = GetFirstControl();
            if (control != null)
            {
                if (IsStringEqual(control.tagName, IMAGE_TAG))
                {
                    mshtmlImageElement image = (mshtmlImageElement)control;
                    imageHref = image.href;
                    imageText = image.alt;
                    if (image.align != null) imageAlign = (ImageAlignOption)TryParseEnum(typeof(ImageAlignOption), image.align, ImageAlignOption.Left);
                }
                else
                {
                    throw new HtmlEditorException("Can only Insert an Image over a previous Image.", "InsertImage");
                }
            }

            // Obtain the image file name
            // prompt the user for the new href
            using (EnterImageForm dialog = new EnterImageForm())
            {
                // set the dialog properties
                dialog.ImageLink = imageHref;
                dialog.ImageText = imageText;
                dialog.ImageAlign = imageAlign;
                PreShowDialog(dialog);
                // based on the user interaction perform the neccessary action
                // after one has a valid image href
                if (dialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
                {
					PostShowDialog(dialog);

					imageHref = dialog.ImageLink;
                    imageText = dialog.ImageText;
                    imageAlign = dialog.ImageAlign;
                    if (imageHref != string.Empty)
                    {
                        ExecuteCommandRange(HTML_COMMAND_INSERT_IMAGE, imageHref);
                        control = GetFirstControl();
                        if (control != null)
                        {
                            if (imageText == string.Empty) imageText = imageHref;
                            if (IsStringEqual(control.tagName, IMAGE_TAG))
                            {
                                mshtmlImageElement image = (mshtmlImageElement)control;
                                image.alt = imageText;
                                if (imageAlign != ImageAlignOption.Left) image.align = imageAlign.ToString().ToLower();
                            }
                        }
                    }
				}
			}

        } //InsertImagePrompt


        /// <summary>
        /// Method to create a web link from the users selected text
        /// </summary>
        public void InsertLink(string href)
        {
            ExecuteCommandRange(HTML_COMMAND_INSERT_LINK, href);

        } //InsertLink

        /// <summary>
        /// Method to insert a link and prompt a user for the href
        /// Calls the public InsertLink method
        /// </summary>
        public void InsertLinkPrompt()
        {
            // get the text range working with
            mshtmlTextRange range = GetTextRange();
            string hrefText = ((range == null) || (range.text == null)) ? string.Empty : range.text;

			string hrefLink = string.Empty;
            NavigateActionOption target;

			// calculate the items working with
			mshtmlAnchorElement anchor = null;
			mshtmlElement element = (mshtmlElement)range.parentElement();

			// parse up the tree until the anchor element is found
			while (element != null && !(element is mshtmlAnchorElement))
			{
				element = (mshtmlElement)element.parentElement;
			}

			// extract the HREF properties
			if (element is mshtmlAnchorElement)
			{
				anchor = (mshtmlAnchorElement)element;

				if (anchor.href != null)
				{
					hrefLink = anchor.href;
					hrefText = element.innerText;
				}
			}
			
			if (String.IsNullOrEmpty(hrefText))
			{
				SelectWordAtCaret();

				range = GetTextRange();
				hrefText = ((range == null) || (range.text == null)) ? string.Empty : range.text;
			}
			
			// if text is a valid href then set the link
			if (hrefLink == string.Empty)
			{
				if (IsValidHref(hrefText))
					hrefLink = hrefText;
				else
					hrefLink = "https://";
			}

			// prompt the user for the new href
			using (EnterHrefForm dialog = new EnterHrefForm())
			{
				dialog.HrefText = hrefText;
				dialog.HrefLink = hrefLink;

				PreShowDialog(dialog);

				DialogResult result = dialog.ShowDialog(/*this.ParentForm*/);

				// based on the user interaction perform the necessary action
				// after one has a valid href
				if ((result != DialogResult.Cancel))
				{
					PostShowDialog(dialog);

					if (result == DialogResult.Yes)
					{
						string newHrefText = dialog.HrefText.Trim();

						hrefLink = dialog.HrefLink;
						target = dialog.HrefTarget;

						if (!String.IsNullOrEmpty(newHrefText) && IsValidHref(hrefLink))
						{
							// If the user has changed the Href text, we paste
							// it over the current selection and then reselect it
							if (!hrefText.Equals(newHrefText))
							{
								ExecuteCommandRange(range, HTML_COMMAND_TEXT_PASTE, newHrefText);

								range = GetTextRange();
								range.moveStart("character", -newHrefText.Length);
							}

							// insert or update the current link
							if (anchor == null)
							{
								ExecuteCommandRange(range, HTML_COMMAND_INSERT_LINK, hrefLink);
								element = (mshtmlElement)range.parentElement();
								// parse up the tree until the anchor element is found
								while (element != null && !(element is mshtmlAnchorElement))
								{
									element = (mshtmlElement)element.parentElement;
								}
								if (element != null) anchor = (mshtmlAnchorElement)element;
							}
							else
							{
								anchor.href = hrefLink;
							}

							if (target != NavigateActionOption.Default)
							{
								anchor.target = (target == NavigateActionOption.NewWindow) ? TARGET_WINDOW_NEW : TARGET_WINDOW_SAME;
							}
							else
							{
								anchor.target = String.Empty;
							}

							range.collapse(false);
							range.select();
						}
					}
					else if (result == DialogResult.No)
					{
						// remove the current link assuming present
						if (anchor != null)
							ExecuteCommandRange(range, HTML_COMMAND_REMOVE_LINK, null); ;
					}

				}
			}
		} //InsertLinkPrompt

	/// <summary>
	/// Method to remove a web link from the users selected text
	/// </summary>
	public void RemoveLink()
        {
            ExecuteCommandRange(HTML_COMMAND_REMOVE_LINK, null);

        } //InsertLink


        /// <summary>
        /// Method to insert the style source into the document
        /// </summary>
        public void InsertStyleSource(string styleSource)
        {
            if (styleSource != null)
            {
                mshtmlDomNode bodyNode = (mshtmlDomNode)body;
                mshtmlDomNode styleNode = (mshtmlDomNode)document.createElement(STYLE_TAG);
                styleNode = bodyNode.insertBefore(styleNode, bodyNode.firstChild);
                mshtmlStyleElement style = (mshtmlStyleElement)styleNode;
                style.styleSheet.cssText = styleSource;
            }

        } //InsertStyleSource

        /// <summary>
        /// Method to insert the script source into the document
        /// </summary>
        public void InsertScriptSource(string scriptSource)
        {
            if (scriptSource != null)
            {
                mshtmlDomNode bodyNode = (mshtmlDomNode)body;
                mshtmlDomNode scriptNode = (mshtmlDomNode)document.createElement(SCRIPT_TAG);
                scriptNode = bodyNode.insertBefore(scriptNode, bodyNode.firstChild);
                mshtmlScriptElement script = (mshtmlScriptElement)scriptNode;
                script.text = scriptSource;
            }

        } //InsertScriptSource

        #endregion

        #region Text Insert Operations

        /// <summary>
        /// Method to insert the given HTML into the selected range
        /// </summary>
        public void InsertHtmlPrompt()
        {
            // display the dialog to obtain the Html to enter
            using (EditHtmlForm dialog = new EditHtmlForm())
            {
                dialog.HTML = "";
                dialog.ReadOnly = false;
                dialog.SetCaption(PASTE_TITLE_HTML);
                PreShowDialog(dialog);
                if (dialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
                {
					PostShowDialog(dialog);
					this.SelectedHtml = dialog.HTML;
				}
			}
        
        } //InsertHtmlPrompt


        /// <summary>
        /// Method to insert the given Text into the selected range
        /// </summary>
        public void InsertTextPrompt()
        {
            // display the dialog to obtain the Html to enter
            using (EditHtmlForm dialog = new EditHtmlForm())
            {
                dialog.HTML = "";
                dialog.ReadOnly = false;
                dialog.SetCaption(PASTE_TITLE_TEXT);
                PreShowDialog(dialog);
                if (dialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
                {
					PostShowDialog(dialog);
                    this.SelectedText = dialog.HTML;
				}
			}
        
        } //InsertTextPrompt


        /// <summary>
        /// Method to return the acceptable values for the format block
        /// </summary>
        public string[] GetFormatBlockCommands()
        {
            return formatCommands;

        } //GetFormatBlockCommands

        /// <summary>
        /// Method to format the selected text by wrapping in the given format tag
        /// </summary>
        public void InsertFormatBlock(string command)
        {
            // ensure the command is acceptable
            if (Array.BinarySearch(formatCommands, command) < 0)
            {
                throw new HtmlEditorException("Invalid Format Block selection.", "InsertFormatBlock");
            }
            else
            {
                ExecuteCommandRange(HTML_COMMAND_INSERT_FORMAT_BLOCK, command);
            }

        } //InsertFormatBlock


        /// <summary>
        /// Method to format the selected text by wrapping in a PRE tag for direct editing
        /// </summary>
        public void InsertFormattedBlock()
        {
            ExecuteCommandRange(HTML_COMMAND_INSERT_FORMAT_BLOCK, HTML_FORMATTED_PRE);

        } //InsertFormattedBlock

        /// <summary>
        /// Method to unformat the selected text by removing heading and pre tags
        /// </summary>
        public void InsertNormalBlock()
        {
            ExecuteCommandRange(HTML_COMMAND_INSERT_FORMAT_BLOCK, HTML_FORMATTED_NORMAL);

        } //InsertNormalBlock

        /// <summary>
        /// Method to insert a heading tag values being Heading 1,2,3,4,5
        /// </summary>
        public void InsertHeading(HtmlHeadingType headingType)
        {
            // obtain the selected range object
            mshtmlTextRange range = GetTextRange();

            // determine the appropriate heading tag and insert the heading
            string command = string.Format("{0} {1}", HTML_FORMATTED_HEADING, (int)headingType);
            InsertFormatBlock(command);

        } //InsertHeading

        #endregion

        #region System Prompt Dialog Functions
        
        /// <summary>
        /// Method to allow the insertion of an image using the system dialogs
        /// </summary>
        public void SystemInsertImage()
        {
            ExecuteCommandDocumentPrompt(HTML_COMMAND_INSERT_IMAGE);

        } //InsertImageSystem


        /// <summary>
        /// Method to allow the insertion of an href using the system dialogs
        /// </summary>
        public void SystemInsertLink()
        {
            ExecuteCommandDocumentPrompt(HTML_COMMAND_INSERT_LINK);

        } //InsertImageSystem

        #endregion

        #region Font and Color Processing Operations

        /// <summary>
        /// Method using the exec command that defines the font properties for the selected text
        /// </summary>
        public void FormatFontAttributes(HtmlFontProperty font)
        {
            // obtain the selected range object
            mshtmlTextRange range = GetTextRange();

            if (range != null && HtmlFontProperty.IsNotNull(font))
            {
                // Use the FONT object to set the properties
                // FontName, FontSize, Bold, Italic, Underline, Strikeout
                ExecuteCommandRange(range, HTML_COMMAND_FONT_NAME, font.Name);
                // set the font size provide set to a value
                if (font.Size != HtmlFontSize.Default) ExecuteCommandRange(range, HTML_COMMAND_FONT_SIZE, (int)font.Size);
                // determine the BOLD property and correct as neccessary
                object currentBold = QueryCommandRange(range, HTML_COMMAND_BOLD);
                bool fontBold = (currentBold is System.Boolean)? fontBold = (bool)currentBold : false;
                if (font.Bold != fontBold) ExecuteCommandRange(HTML_COMMAND_BOLD, null);
                // determine the UNDERLINE property and correct as neccessary
                object currentUnderline = QueryCommandRange(range, HTML_COMMAND_UNDERLINE);
                bool fontUnderline = (currentUnderline is System.Boolean) ? fontUnderline = (bool)currentUnderline : false;
                if (font.Underline != fontUnderline) ExecuteCommandRange(HTML_COMMAND_UNDERLINE, null);
                // determine the ITALIC property and correct as neccessary
                object currentItalic = QueryCommandRange(range, HTML_COMMAND_ITALIC);
                bool fontItalic = (currentItalic is System.Boolean) ? fontItalic = (bool)currentItalic : false;
                if (font.Italic != fontItalic) ExecuteCommandRange(HTML_COMMAND_ITALIC, null);
                // determine the STRIKEOUT property and correct as neccessary
                object currentStrikeout = QueryCommandRange(range, HTML_COMMAND_STRIKE_THROUGH);
                bool fontStrikeout = (currentStrikeout is System.Boolean) ? fontStrikeout = (bool)currentStrikeout : false;
                if (font.Strikeout != fontStrikeout) ExecuteCommandRange(HTML_COMMAND_STRIKE_THROUGH, null);
                // determine the SUPERSCRIPT property and correct as neccessary
                object currentSuperscript = QueryCommandRange(range, HTML_COMMAND_SUPERSCRIPT);
                bool fontSuperscript = (currentSuperscript is System.Boolean) ? fontSuperscript = (bool)currentSuperscript : false;
                if (font.Superscript != fontSuperscript) ExecuteCommandRange(HTML_COMMAND_SUPERSCRIPT, null);
                // determine the SUBSCRIPT property and correct as neccessary
                object currentSubscript = QueryCommandRange(range, HTML_COMMAND_SUBSCRIPT);
                bool fontSubscript = (currentSubscript is System.Boolean) ? fontSubscript = (bool)currentSubscript : false;
                if (font.Subscript != fontSubscript) ExecuteCommandRange(HTML_COMMAND_SUBSCRIPT, null);
            }
            else
            {
                // do not have text selected or a valid font class
                throw new HtmlEditorException("Invalid Text selection made or Invalid Font selection.", "FormatFont");
            }

        } //FormatFontAttributes


        /// <summary>
        /// Method using the exec command to define the color properties for the selected tag
        /// </summary>
        public void FormatFontColor(Color color)
        {
            // Use the COLOR object to set the property ForeColor
            string colorHtml;
            if (color != Color.Empty) colorHtml = ColorTranslator.ToHtml(color);
            else colorHtml = null;
            ExecuteCommandRange(HTML_COMMAND_FORE_COLOR, colorHtml);

        } //FormatFontColor


        /// <summary>
        /// Method to display the defined font dialog
        /// Use to set the selected text Font
        /// </summary>
        public void FormatFontAttributesPrompt()
        {
            using (FontAttributeForm dialog = new FontAttributeForm())
            {
                PreShowDialog(dialog);
                dialog.HtmlFont = GetFontAttributes();
                if (dialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
                {
					PostShowDialog(dialog);
                    HtmlFontProperty font = dialog.HtmlFont;
                    FormatFontAttributes(new HtmlFontProperty(font.Name, font.Size, font.Bold, font.Italic, font.Underline, font.Strikeout, font.Subscript, font.Superscript));
				}
			}

        } //FormatFontAttributesPrompt


        /// <summary>
        /// Method to display the system color dialog
        /// Use use to set the selected text Color
        /// </summary>
        public void FormatFontColorPrompt()
        {
            // display the Color dialog and use the selected color to modify text
            using (ColorDialog colorDialog = new ColorDialog())
            {
                colorDialog.AnyColor = true;
                colorDialog.SolidColorOnly = true;
                colorDialog.AllowFullOpen = true;
                colorDialog.Color = GetFontColor();
                colorDialog.CustomColors = _customColors;
                if (colorDialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
                {
                    _customColors = colorDialog.CustomColors;
                    FormatFontColor(colorDialog.Color);
                }
            }

        } //FormatFontColorPrompt

    
        /// <summary>
        /// Method to determine the Font of the selected text range
        /// Returns the default value of not defined if not Text selected
        /// </summary>
        public HtmlFontProperty GetFontAttributes()
        {
            // get the text range working with
            mshtmlTextRange range = GetTextRange();

            if (range != null)
            {
                // get font name to show
                object currentName = QueryCommandRange(range, HTML_COMMAND_FONT_NAME);
                string fontName = (currentName is System.String) ? (string)currentName : _bodyFont.Name;
                // determine font size to show
                object currentSize = QueryCommandRange(range, HTML_COMMAND_FONT_SIZE);
                HtmlFontSize fontSize = (currentSize is System.Int32) ? (HtmlFontSize)currentSize : _bodyFont.Size;
                // determine the BOLD property
                bool fontBold = IsFontBold(range);
                // determine the UNDERLINE property
                bool fontUnderline = IsFontUnderline(range);
                // determine the ITALIC property
                bool fontItalic = IsFontItalic(range);
                // determine the STRIKEOUT property
                bool fontStrikeout = IsFontStrikeout(range);
                // determine the SUPERSCRIPT property
                bool fontSuperscript = IsFontSuperscript(range);
                // determine the SUBSCRIPT property
                bool fontSubscript = IsFontSubscript(range);
                // calculate the Font and return
                return new HtmlFontProperty(fontName, fontSize, fontBold, fontUnderline, fontItalic, fontStrikeout, fontSubscript, fontSuperscript);
            }
            else
            {
                // no rnage selected so return null
                return _defaultFont;
            }

        } //GetFontAttributes


        /// <summary>
        /// DetermineS if the current font selected is bold when given a range
        /// </summary>
        private bool IsFontBold(mshtmlTextRange range)
        {
            // determine the BOLD property
            object currentBold = QueryCommandRange(range, HTML_COMMAND_BOLD);
            return (currentBold is System.Boolean)? (bool)currentBold : _bodyFont.Bold;

        } //IsFontBold

        /// <summary>
        /// Determines if the current font selected is bold when given a range
        /// </summary>
        public bool IsFontBold()
        {
            // get the text range working with
            mshtmlTextRange range = GetTextRange();
            // return the font property
            return IsFontBold(range);

        } //IsFontBold


        /// <summary>
        /// Determines if the current font selected is Underline when given a range
        /// </summary>
        /// <param name="range"></param>
        private bool IsFontUnderline(mshtmlTextRange range)
        {
            // determine the UNDERLINE property
            object currentUnderline = QueryCommandRange(range, HTML_COMMAND_UNDERLINE);
            return (currentUnderline is System.Boolean) ? (bool)currentUnderline : _bodyFont.Underline;

        } //IsFontUnderline

        /// <summary>
        /// Determines if the current font selected is Underline
        /// </summary>
        public bool IsFontUnderline()
        {
            // get the text range working with
            mshtmlTextRange range = GetTextRange();
            // return the font property
            return IsFontUnderline(range);

        } //IsFontUnderline


        /// <summary>
        /// Determines if the current font selected is Italic when given a range
        /// </summary>
        private bool IsFontItalic(mshtmlTextRange range)
        {
            // determine the ITALIC property
            object currentItalic = QueryCommandRange(range, HTML_COMMAND_ITALIC);
            return (currentItalic is System.Boolean) ? (bool)currentItalic : _bodyFont.Italic;

        } //IsFontItalic

        /// <summary>
        /// Determines if the current font selected is Italic
        /// </summary>
        /// <returns></returns>
        public bool IsFontItalic()
        {
            // get the text range working with
            mshtmlTextRange range = GetTextRange();
            // return the font property
            return IsFontItalic(range);

        } //IsFontItalic


        /// <summary>
        /// Determines if the current font selected is Strikeout when given a range
        /// </summary>
        private bool IsFontStrikeout(mshtmlTextRange range)
        {
            // determine the STRIKEOUT property
            object currentStrikeout = QueryCommandRange(range, HTML_COMMAND_STRIKE_THROUGH);
            return (currentStrikeout is System.Boolean) ? (bool)currentStrikeout : _bodyFont.Strikeout;

        } //IsFontStrikeout

        /// <summary>
        /// Determines if the current font selected is Strikeout
        /// </summary>
        public bool IsFontStrikeout()
        {
            // get the text range working with
            mshtmlTextRange range = GetTextRange();
            // return the font property
            return IsFontStrikeout(range);

        } //IsFontStrikeout

        /// <summary>
        /// Determines if the current font selected is Superscript when given a range
        /// </summary>
        private bool IsFontSuperscript(mshtmlTextRange range)
        {
            // determine the SUPERSCRIPT property
            object currentSuperscript = QueryCommandRange(range, HTML_COMMAND_SUPERSCRIPT);
            return (currentSuperscript is System.Boolean) ? (bool)currentSuperscript : false;

        } //IsFontSuperscript

        /// <summary>
        /// Determines if the current font selected is Superscript
        /// </summary>
        public bool IsFontSuperscript()
        {
            // get the text range working with
            mshtmlTextRange range = GetTextRange();
            // return the font property
            return IsFontSuperscript(range);

        } //IsFontSuperscript

        /// <summary>
        /// Determines if the current font selected is Subscript when given a range
        /// </summary>
        private bool IsFontSubscript(mshtmlTextRange range)
        {
            // determine the SUBSCRIPT property
            object currentSubscript = QueryCommandRange(range, HTML_COMMAND_SUBSCRIPT);
            return (currentSubscript is System.Boolean) ? (bool)currentSubscript : false;

        } //IsFontSubscript

        /// <summary>
        /// Determines if the current font selected is Subscript
        /// </summary>
        public bool IsFontSubscript()
        {
            // get the text range working with
            mshtmlTextRange range = GetTextRange();
            // return the font property
            return IsFontSubscript(range);

        } //IsFontSubscript


        /// <summary>
        /// Determines the color of the selected text range
        /// Returns default value if not text selected
        /// </summary>
        private Color GetFontColor()
        {
            object fontColor = QueryCommandRange(HTML_COMMAND_FORE_COLOR);
            return (fontColor is System.Int32) ? ColorTranslator.FromWin32((int)fontColor) : _bodyForeColor;

        } //GetFontColor

        #endregion

        #region Find and Replace Operations

        /// <summary>
        /// Dialog to allow the user to perform a find and replace
        /// </summary>
        public void FindReplacePrompt()
        {

            // define a default value for the text to find
            mshtmlTextRange range = GetTextRange();
            string initText = string.Empty;
            if (range != null)
            {
                string findText = range.text;
                if (findText != null) initText = findText.Trim();
            }

            // prompt the user for the new href
            using (FindReplaceForm dialog = new FindReplaceForm(initText,
                       new FindReplaceResetDelegate(this.FindReplaceReset),
                       new FindFirstDelegate(this.FindFirst),
                       new FindNextDelegate(this.FindNext),
                       new FindReplaceOneDelegate(this.FindReplaceOne),
                       new FindReplaceAllDelegate(this.FindReplaceAll) ))
            {
                PreShowDialog(dialog);
                DialogResult result = dialog.ShowDialog(/*this.ParentForm*/);
				PostShowDialog(dialog);
			}

		} //FindReplacePrompt


        /// <summary>
        /// Method to reset the find and replace options to initialize a new search
        /// </summary>
        public void FindReplaceReset()
        {
            // reset the range being worked with
            _findRange = (mshtmlTextRange)body.createTextRange();
            ((mshtmlSelection)document.selection).empty();

        } //FindReplaceReset


        /// <summary>
        /// Method to find the first occurrence of the given text string
        /// Uses false case for the search options
        /// </summary>
        public bool FindFirst(string findText)
        {
            return FindFirst(findText, false, false);

        } //FindFirst

        /// <summary>
        /// Method to find the first occurrence of the given text string
        /// </summary>
        public bool FindFirst(string findText, bool matchWhole, bool matchCase)
        {
            // reset the text search range prior to making any calls
            FindReplaceReset();

            // calls the Find Next once search has been initialized
            return FindNext(findText, matchWhole, matchCase);

        } //FindFirst


        /// <summary>
        /// Method to find the next occurrence of a given text string
        /// Assumes a previous search was made
        /// Uses false case for the search options
        /// </summary>
        public bool FindNext(string findText)
        {
            return FindNext(findText, false, false);

        } //FindNext

        /// <summary>
        /// Method to find the next occurrence of a given text string
        /// Assumes a previous search was made
        /// </summary>
        public bool FindNext(string findText, bool matchWhole, bool matchCase)
        {
            return (FindText(findText, matchWhole, matchCase) != null ? true : false);

        } //FindNext


        /// <summary>
        /// Replace the first occurrence of the given string with the other
        /// Uses false case for the search options
        /// </summary>
        public bool FindReplaceOne(string findText, string replaceText)
        {
            return FindReplaceOne(findText, replaceText);

        } //FindReplaceOne

        /// <summary>
        /// Method to replace the first occurrence of the given string with the other
        /// </summary>
        public bool FindReplaceOne(string findText, string replaceText, bool matchWhole, bool matchCase)
        {
            // find the given text within the find range
            mshtmlTextRange replaceRange = FindText(findText, matchWhole, matchCase);
            if (replaceRange != null) 
            {
                // if text found perform a replace
                replaceRange.text = replaceText;
                replaceRange.select();
                // replace made to return success
                return true;
            }
            else
            {
                // no replace was made so return false
                return false;
            }

        } //FindReplaceOne


        /// <summary>
        /// Method to replace all the occurrence of the given string with the other
        /// Uses false case for the reset range
        /// </summary>
        protected int FindReplaceAll(string findText, string replaceText, bool matchWhole, bool matchCase)
        {
            return FindReplaceAll(findText, replaceText, matchWhole, matchCase, false);

        } //FindReplaceAll

        /// <summary>
        /// Method to replace all the occurrences of the given string with the other
        /// </summary>
        public int FindReplaceAll(string findText, string replaceText, bool matchWhole, bool matchCase, bool resetRange)
        {
            if (resetRange)
            {
                _findRange = (mshtmlTextRange)body.createTextRange();
            }
            else if (_findRange == null)
            {
                return 0;
            }
            
            int found = 0;
            mshtmlTextRange replaceRange = null;

            do
            {
                // find the given text within the find range
                replaceRange = FindText(findText, matchWhole, matchCase);
                // if found perform a replace
                if (replaceRange != null) 
                {
                    replaceRange.text = replaceText;
                    found++;
                }
            } while (replaceRange != null);

            // return count of items repalced
            return found;

        } //FindReplaceAll


        /// <summary>
        /// Method to perform the actual find of the given text
        /// </summary>
        private mshtmlTextRange FindText(string findText, bool matchWhole, bool matchCase)
        {
            // define the search options
            int searchOptions = 0;
            if (matchWhole) searchOptions = searchOptions + 2;
            if (matchCase) searchOptions = searchOptions + 4;

            // perform the search operation
            if (_findRange.findText(findText, _findRange.text.Length, searchOptions))
            {
                // select the found text within the document
                _findRange.select();
                // limit the new find range to be from the newly found text
                mshtmlTextRange foundRange = (mshtmlTextRange)document.selection.createRange();
                _findRange = (mshtmlTextRange)body.createTextRange();
                _findRange.setEndPoint("StartToEnd", foundRange);
                // text found so return this selection
                return foundRange;
            }
            else
            {
                // reset the find ranges
                FindReplaceReset();
                // no text found so return null range
                return null;
            }

        } //FindText 

        #endregion

        #region Table Processing Operations

        /// <summary>
        /// Method to create a table class
        /// Insert method then works on this table
        /// </summary>
        public void TableInsert(HtmlTableProperty tableProperties)
        {
            // call the private insert table method with a null table entry
            ProcessTable(null, tableProperties);

        } //TableInsert

        /// <summary>
        /// Method to modify a tables properties
        /// Ensure a table is currently selected or insertion point is within a table
        /// </summary>
        public bool TableModify(HtmlTableProperty tableProperties)
        {
            // define the Html Table element
            mshtmlTable table = GetTableElement();

            // if a table has been selected then process
            if (table != null)
            {
                ProcessTable(table, tableProperties);
                return true;
            }
            else
            {
                return false;
            }

        } //TableModify

        /// <summary>
        /// Method to present to the user the table properties dialog
        /// Uses all the default properties for the table based on an insert operation
        /// </summary>
        public void TableInsertPrompt()
        {
            // if user has selected a table create a reference
            mshtmlTable table = GetFirstControl() as mshtmlTable;
            ProcessTablePrompt(table);

        } //TableInsertPrompt


        /// <summary>
        /// Method to present to the user the table properties dialog
        /// Ensure a table is currently selected or insertion point is within a table
        /// </summary>
        public bool TableModifyPrompt()
        {
            // define the Html Table element
            mshtmlTable table = GetTableElement();

            // if a table has been selected then process
            if (table != null)
            {
                ProcessTablePrompt(table);
                return true;
            }
            else
            {
                return false;
            }

        } //TableModifyPrompt


        /// <summary>
        /// Method to insert a new row into the table
        /// Based on the current user row and insertion after
        /// </summary>
        public void TableInsertRow()
        {
            // see if a table selected or insertion point inside a table
            mshtmlTable table = null;
            mshtmlTableRow row = null;
            GetTableElement(out table, out row);

            // process according to table being defined
            if (table != null && row != null)
            {
                try
                {
                    // find the existing row the user is on and perform the insertion
                    int index = row.rowIndex + 1;
                    mshtmlTableRow insertedRow = (mshtmlTableRow)table.insertRow(index);
                    // add the new columns to the end of each row
                    int numberCols = row.cells.length;
                    for (int idxCol = 0; idxCol < numberCols; idxCol++)
                    {
                        insertedRow.insertCell(-1);
                    }
                }
                catch (Exception ex)
                {
                    throw new HtmlEditorException("Unable to insert a new Row", "TableinsertRow", ex);
                }
            }
            else
            {
                throw new HtmlEditorException("Row not currently selected within the table", "TableInsertRow");
            }

        } //TableInsertRow


        /// <summary>
        /// Method to delete the currently selected row
        /// Operation based on the current user row location
        /// </summary>
        public void TableDeleteRow()
        {
            // see if a table selected or insertion point inside a table
            mshtmlTable table = null;
            mshtmlTableRow row = null;
            GetTableElement(out table, out row);

            // process according to table being defined
            if (table != null && row != null)
            {
                try
                {
                    // find the existing row the user is on and perform the deletion
                    int index = row.rowIndex;
                    table.deleteRow(index);
                }
                catch (Exception ex)
                {
                    throw new HtmlEditorException("Unable to delete the selected Row", "TableDeleteRow", ex);
                }
            }
            else
            {
                throw new HtmlEditorException("Row not currently selected within the table", "TableDeleteRow");
            }

        } //TableDeleteRow


        /// <summary>
        /// Method to present to the user the table properties dialog
        /// Uses all the default properties for the table based on an insert operation
        /// </summary>
        private void ProcessTablePrompt(mshtmlTable table)
        {
            using (TablePropertyForm dialog = new TablePropertyForm())
            {
                // define the base set of table properties
                HtmlTableProperty tableProperties = GetTableProperties(table);

                // set the dialog properties
                dialog.TableProperties = tableProperties;
                PreShowDialog(dialog);
                // based on the user interaction perform the neccessary action
                if (dialog.ShowDialog(/*this.ParentForm*/) == DialogResult.OK)
                {
					PostShowDialog(dialog);

					tableProperties = dialog.TableProperties;

                    if (table == null)
						TableInsert(tableProperties);
                    else
						ProcessTable(table, tableProperties);
				}
			}

        } // ProcessTablePrompt


        /// <summary>
        /// Method to insert a basic table
        /// Will honour the existing table if passed in
        /// </summary>
        private void ProcessTable(mshtmlTable table, HtmlTableProperty tableProperties)
        {
            try
            {
                // obtain a reference to the body node and indicate table present
                mshtmlDomNode bodyNode = (mshtmlDomNode)document.body;
                bool tableCreated = false;

                // ensure a table node has been defined to work with
                if (table == null)
                {
                    // create the table and indicate it was created
                    table = (mshtmlTable)document.createElement(TABLE_TAG);
                    tableCreated = true;
                }

                // define the table border, width, cell padding and spacing
                table.border = tableProperties.BorderSize;
                if (tableProperties.TableWidth > 0) table.width = (tableProperties.TableWidthMeasurement == MeasurementOption.Pixel) ? string.Format("{0}", tableProperties.TableWidth) : string.Format("{0}%", tableProperties.TableWidth); 
                else table.width = string.Empty;
                if (tableProperties.TableAlignment != HorizontalAlignOption.Default) table.align = tableProperties.TableAlignment.ToString().ToLower();
                else table.align = string.Empty;
                table.cellPadding = tableProperties.CellPadding.ToString();
                table.cellSpacing = tableProperties.CellSpacing.ToString();

                // define the given table caption and alignment
                string caption = tableProperties.CaptionText;
                mshtmlTableCaption tableCaption = table.caption;
                if (caption != null && caption != string.Empty)
                {
                    // ensure table caption correctly defined
                    if (tableCaption == null) tableCaption = table.createCaption();
                    ((mshtmlElement)tableCaption).innerText = caption;
                    if (tableProperties.CaptionAlignment != HorizontalAlignOption.Default) tableCaption.align = tableProperties.CaptionAlignment.ToString().ToLower();
                    if (tableProperties.CaptionLocation != VerticalAlignOption.Default) tableCaption.vAlign = tableProperties.CaptionLocation.ToString().ToLower();
                }
                else
                {
                    // if no caption specified remove the existing one
                    if (tableCaption != null)
                    {
                        // prior to deleting the caption the contents must be cleared
                        ((mshtmlElement)tableCaption).innerText = null;
                        table.deleteCaption();
                    }
                }

                // determine the number of rows one has to insert
                int numberRows, numberCols;
                if (tableCreated)
                {
                    numberRows = Math.Max((int)tableProperties.TableRows, 1);
                }
                else
                {
                    numberRows = Math.Max((int)tableProperties.TableRows, 1) - (int)table.rows.length;
                }

                // layout the table structure in terms of rows and columns
                table.cols = (int)tableProperties.TableColumns;
                if (tableCreated)
                {
                    // this section is an optimization based on creating a new table
                    // the section below works but not as efficiently
                    numberCols = Math.Max((int)tableProperties.TableColumns, 1);
                    // insert the appropriate number of rows
                    mshtmlTableRow tableRow;
                    for (int idxRow = 0; idxRow < numberRows; idxRow++)
                    {
                        tableRow = (mshtmlTableRow)table.insertRow(-1);
                        // add the new columns to the end of each row
                        for (int idxCol = 0; idxCol < numberCols; idxCol++)
                        {
                            tableRow.insertCell(-1);
                        }
                    }
                }
                else
                {
                    // if the number of rows is increasing insert the decrepency
                    if (numberRows > 0)
                    {
                        // insert the appropriate number of rows
                        for (int idxRow = 0; idxRow < numberRows; idxRow++)
                        {
                            table.insertRow(-1);
                        }
                    }
                    else
                    {
                        // remove the extra rows from the table
                        for (int idxRow = numberRows; idxRow < 0; idxRow++)
                        {
                            table.deleteRow(table.rows.length - 1);
                        }
                    }
                    // have the rows constructed
                    // now ensure the columns are correctly defined for each row
                    mshtmlElementCollection rows = table.rows;
                    foreach (mshtmlTableRow tableRow in rows)
                    {
                        numberCols = Math.Max((int)tableProperties.TableColumns, 1) - (int)tableRow.cells.length;
                        if (numberCols > 0)
                        {
                            // add the new column to the end of each row
                            for (int idxCol = 0; idxCol < numberCols; idxCol++)
                            {
                                tableRow.insertCell(-1);
                            }
                        }
                        else
                        {
                            // reduce the number of cells in the given row
                            // remove the extra rows from the table
                            for (int idxCol = numberCols; idxCol < 0; idxCol++)
                            {
                                tableRow.deleteCell(tableRow.cells.length - 1);
                            }				
                        }
                    }
                }

                // if the table was created then it requires insertion into the DOM
                // otherwise property changes are sufficient
                if (tableCreated)
                {
                    // table processing all complete so insert into the DOM
                    mshtmlDomNode tableNode = (mshtmlDomNode)table;
                    mshtmlElement tableElement = (mshtmlElement)table;
                    mshtmlSelection selection = document.selection;
                    mshtmlTextRange textRange = GetTextRange();
                    // final insert dependant on what user has selected
                    if (textRange != null)
                    {
                        // text range selected so overwrite with a table
                        try
                        {
                            string selectedText = textRange.text;
                            if (selectedText != null)
                            {
                                // place selected text into first cell
                                mshtmlTableRow tableRow = (mshtmlTableRow)table.rows.item(0, null);
                                ((mshtmlElement)tableRow.cells.item(0, null)).innerText = selectedText;
                            }
                            textRange.pasteHTML(tableElement.outerHTML);
                        }
                        catch (Exception ex)
                        {
                            throw new HtmlEditorException("Invalid Text selection for the Insertion of a Table.", "ProcessTable", ex);
                        }
                    }
                    else
                    {
                        mshtmlControlRange controlRange = GetAllControls();
                        if (controlRange != null)
                        {
                            // overwrite any controls the user has selected
                            try
                            {
                                // clear the selection and insert the table
                                // only valid if multiple selection is enabled
                                for (int idx = 1; idx < controlRange.length; idx++)
                                {
                                    controlRange.remove(idx);
                                }
                                controlRange.item(0).outerHTML = tableElement.outerHTML;
                                // this should work with initial count set to zero
                                // controlRange.add((mshtmlControlElement)table);
                            }
                            catch (Exception ex)
                            {
                                throw new HtmlEditorException("Cannot Delete all previously Controls selected.", "ProcessTable", ex);
                            }
                        }
                        else
                        {
                            // insert the table at the end of the HTML
                            bodyNode.appendChild(tableNode);
                        }
                    }
                }
                else
                {
                    // table has been correctly defined as being the first selected item
                    // need to remove other selected items
                    mshtmlControlRange controlRange = GetAllControls();
                    if (controlRange != null)
                    {
                        // clear the controls selected other than than the first table
                        // only valid if multiple selection is enabled
                        for (int idx = 1; idx < controlRange.length; idx++)
                        {
                            controlRange.remove(idx);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                // throw an exception indicating table structure change error
                throw new HtmlEditorException("Unable to modify Html Table properties.", "ProcessTable", ex);
            }

        } //ProcessTable


        /// <summary>
        /// Method to determine if the current selection is a table
        /// If found will return the table element
        /// </summary>
        private void GetTableElement(out mshtmlTable table, out mshtmlTableRow row)
        {
            table = null;
            row = null;
            mshtmlTextRange range = GetTextRange();

            try
            {
                // first see if the table element is selected
                table = GetFirstControl() as mshtmlTable;
                // if table not selected then parse up the selection tree
                if (table == null && range != null)
                {
                    mshtmlElement element = (mshtmlElement)range.parentElement();
                    // parse up the tree until the table element is found
                    while (element != null && table == null)
                    {
                        element = (mshtmlElement)element.parentElement;
                        // extract the Table properties
                        if (element is mshtmlTable)
                        {
                            table = (mshtmlTable)element;
                        }
                        // extract the Row  properties
                        if (element is mshtmlTableRow)
                        {
                            row = (mshtmlTableRow)element;
                        }
                    }
                }
            }
            catch (Exception)
            {
                // have unknown error so set return to null
                table = null;
                row = null;
            }

        } //GetTableElement

        /// <summary>
        /// Method to return the currently selected Html Table Element
        /// </summary>
        private mshtmlTable GetTableElement()
        {
            // define the table and row elements and obtain there values
            mshtmlTable table = null;
            mshtmlTableRow row = null;
            GetTableElement(out table, out row);

            // return the defined table element
            return table;
        
        }


        /// <summary>
        /// Given an Html Table Element determines the table properties
        /// Returns the properties as an HtmlTableProperty class
        /// </summary>
        private HtmlTableProperty GetTableProperties(mshtmlTable table)
        {
            // define a set of base table properties
            HtmlTableProperty tableProperties = new HtmlTableProperty(true);

            // if user has selected a table extract those properties
            if (table != null)
            {
                try
                {
                    // have a table so extract the properties
                    mshtmlTableCaption caption = table.caption;
                    // if have a caption persist the values
                    if (caption != null)
                    {
                        tableProperties.CaptionText = ((mshtmlElement)table.caption).innerText;
                        if (caption.align != null) tableProperties.CaptionAlignment = (HorizontalAlignOption)TryParseEnum(typeof(HorizontalAlignOption), caption.align, HorizontalAlignOption.Default);
                        if (caption.vAlign != null) tableProperties.CaptionLocation = (VerticalAlignOption)TryParseEnum(typeof(VerticalAlignOption), caption.vAlign, VerticalAlignOption.Default);
                    }
                    // look at the table properties
                    if (table.border != null) tableProperties.BorderSize = TryParseByte(table.border.ToString(), tableProperties.BorderSize);
                    if (table.align != null) tableProperties.TableAlignment = (HorizontalAlignOption)TryParseEnum(typeof(HorizontalAlignOption), table.align, HorizontalAlignOption.Default);
                    // define the table rows and columns
                    int rows = Math.Min(table.rows.length, Byte.MaxValue);
                    int cols = Math.Min(table.cols, Byte.MaxValue);
                    if (cols == 0 && rows > 0)
                    {
                        // cols value not set to get the maxiumn number of cells in the rows
                        foreach (mshtmlTableRow tableRow in table.rows)
                        {
                            cols = Math.Max(cols, (int)tableRow.cells.length);
                        }
                    }
                    tableProperties.TableRows = (byte)Math.Min(rows, byte.MaxValue);
                    tableProperties.TableColumns = (byte)Math.Min(cols, byte.MaxValue);
                    // define the remaining table properties
                    if (table.cellPadding != null) tableProperties.CellPadding = TryParseByte(table.cellPadding.ToString(), tableProperties.CellPadding);
                    if (table.cellSpacing != null) tableProperties.CellSpacing = TryParseByte(table.cellSpacing.ToString(), tableProperties.CellSpacing);
                    if (table.width != null)
                    {
                        string tableWidth = table.width.ToString();
                        if (tableWidth.TrimEnd(null).EndsWith("%"))
                        {
                            tableProperties.TableWidth = TryParseUshort(tableWidth.Remove(tableWidth.LastIndexOf("%"), 1), tableProperties.TableWidth);
                            tableProperties.TableWidthMeasurement = MeasurementOption.Percent;
                        }
                        else
                        {
                            tableProperties.TableWidth = TryParseUshort(tableWidth, tableProperties.TableWidth);
                            tableProperties.TableWidthMeasurement = MeasurementOption.Pixel;
                        }
                    }
                    else
                    {
                        tableProperties.TableWidth = 0;
                        tableProperties.TableWidthMeasurement = MeasurementOption.Pixel;
                    }
                }
                catch (Exception ex)
                {
                    // throw an exception indicating table structure change be determined
                    throw new HtmlEditorException("Unable to determine Html Table properties.", "GetTableProperties", ex);
                }
            }

            // return the table properties
            return tableProperties;

        } //GetTableProperties

        
        /// <summary>
        /// Method to return  a table defintion based on the user selection
        /// If table selected (or insertion point within table) returns these values
        /// </summary>
        public void GetTableDefinition(out HtmlTableProperty table, out bool tableFound)
        {
            // see if a table selected or insertion point inside a table
            mshtmlTable htmlTable = GetTableElement();

            // process according to table being defined
            if (htmlTable == null)
            {
                table = new HtmlTableProperty(true);
                tableFound = false;
            }
            else
            {
                table = GetTableProperties(htmlTable);
                tableFound = true;
            }

        } //GetTableDefinition


        /// <summary>
        /// Method to determine if the insertion point or selection is a table
        /// </summary>
        private bool IsParentTable()
        {
            // see if a table selected or insertion point inside a table
            mshtmlTable htmlTable = GetTableElement();

            // process according to table being defined
            if (htmlTable == null)
            {
                return false;
            }
            else
            {
                return true;
            }

        } //IsParentTable


        #endregion

        #region MsHtml Command Processing

        /// <summary>
        /// Performs a query of the command state
        /// </summary>
        private bool ExecuteCommandQuery(string command)
        {
            // obtain the selected range object and query command
            mshtmlTextRange range = GetTextRange();
            return ExecuteCommandQuery(range, command);

        } //ExecuteCommandQuery

        /// <summary>
        /// Executes the queryCommandState on the selected range (given the range)
        /// </summary>
        private bool ExecuteCommandQuery(mshtmlTextRange range, string command)
        {
            try
            {
                if (range != null)
                {
                    // ensure command is a valid command and then enabled for the selection
                    if (range.queryCommandSupported(command))
                    {
                        if (range.queryCommandEnabled(command))
                        {
                            // mark the selection with the appropriate tag
                            return range.queryCommandState(command);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                // Unknown error so inform user
                throw new HtmlEditorException("Unknown MSHTML Error.", command, ex);
            }

            // all elsee
            return false;

        } // ExecuteCommandQuery


        /// <summary>
        /// Executes the execCommand on the selected range
        /// </summary>
        private bool ExecuteCommandRange(string command, object data)
        {
            // obtain the selected range object and execute command
            mshtmlTextRange range = GetTextRange();
            return ExecuteCommandRange(range, command, data);

        } // ExecuteCommandRange

        /// <summary>
        /// Executes the execCommand on the selected range (given the range)
        /// </summary>
        private bool ExecuteCommandRange(mshtmlTextRange range, string command, object data)
        {
            try
            {
                if (range != null)
                {
                    // ensure command is a valid command and then enabled for the selection
                    if (range.queryCommandSupported(command))
                    {
                        if (range.queryCommandEnabled(command))
                        {
                            // mark the selection with the appropriate tag
                            return range.execCommand(command, false, data);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                // Unknown error so inform user
                throw new HtmlEditorException("Unknown MSHTML Error.", command, ex);
            }

            // all else
            return false;
        } // ExecuteCommandRange


        /// <summary>
        /// Executes the execCommand on the document
        /// </summary>
        private bool ExecuteCommandDocument(string command)
        {
            return ExecuteCommandDocument(command, false);

        } // ExecuteCommandDocument

        protected bool ExecuteQueryDocument(string command)
        {
            try
            {
                return document.queryCommandEnabled(command);
            }
            catch (Exception ex)
            {
                // Unknown error so inform user
                throw new HtmlEditorException("Unknown MSHTML Error.", command, ex);
            }
        }

        /// <summary>
        /// Executes the execCommand on the document with a system prompt
        /// </summary>
        protected bool ExecuteCommandDocumentPrompt(string command)
        {
            return ExecuteCommandDocument(command, true);

        } // ExecuteCommandDocumentPrompt

        /// <summary>
        /// Executes the execCommand on the document with a system prompt
        /// </summary>
        private bool ExecuteCommandDocument(string command, bool prompt)
        {
            try
            {
                // ensure command is a valid command and then enabled for the selection
                if (document.queryCommandSupported(command))
                {
                    // if (document.queryCommandEnabled(command)) {}
                    // Test fails with a COM exception if command is Print

                    // execute the given command
                    return document.execCommand(command, prompt, null);
                }
            }
            catch (Exception ex)
            {
                // Unknown error so inform user
                throw new HtmlEditorException("Unknown MSHTML Error.", command, ex);
            }

            // all else
            return false;
        } // ExecuteCommandDocumentPrompt


        /// <summary>
        /// Determines the value of the command
        /// </summary>
        protected object QueryCommandRange(string command)
        {
            // obtain the selected range object and execute command
            mshtmlTextRange range = GetTextRange();
            return QueryCommandRange(range, command);

        } // QueryCommandRange

        /// <summary>
        /// Determines the value of the command
        /// </summary>
        private object QueryCommandRange(mshtmlTextRange range, string command)
        {
            object retValue = null;
            if (range != null)
            {
                try
                {
                    // ensure command is a valid command and then enabled for the selection
                    if (range.queryCommandSupported(command))
                    {
                        if (range.queryCommandEnabled(command))
                        {
                            retValue = range.queryCommandValue(command);
                        }
                    }
                }
                catch (Exception)
                {
                    // have unknown error so set return to null
                    retValue = null;
                }
            }

            // return the obtained value
            return retValue;

        } //QueryCommandRange


        /// <summary>
        /// Gets the selected Html Range Element
        /// </summary>
        private mshtmlTextRange GetTextRange()
        {
            // define the selected range object
            mshtmlSelection selection;
            mshtmlTextRange range = null;

            try
            {
                // calculate the text range based on user selection
                selection = document.selection;
                if (IsStringEqual(selection.type, SELECT_TYPE_TEXT) || IsStringEqual(selection.type, SELECT_TYPE_NONE))
                {
                    range = (mshtmlTextRange)selection.createRange();
                }
            }
            catch (Exception)
            {
                // have unknown error so set return to null
                range = null;
            }

            return range;

        } // GetTextRange


        /// <summary>
        /// Gets the first selected Html Control as an Element
        /// </summary>
        private mshtmlElement GetFirstControl()
        {
            // define the selected range object
            mshtmlSelection selection;
            mshtmlControlRange range;
            mshtmlElement control = null;

            try
            {
                // calculate the first control based on the user selection
                selection = document.selection;
                if (IsStringEqual(selection.type, SELECT_TYPE_CONTROL))
                {
                    range = (mshtmlControlRange)selection.createRange();
                    if (range.length > 0) control = range.item(0);
                }
            }
            catch (Exception)
            {
                // have unknown error so set return to null
                control = null;
            }

            return control;

        } // GetFirstControl

        /// <summary>
        /// Gets all the selected Html Controls as a Control Range
        /// </summary>
        /// <returns></returns>
        private mshtmlControlRange GetAllControls()
        {
            // define the selected range object
            mshtmlSelection selection;
            mshtmlControlRange range = null;

            try
            {
                // calculate the first control based on the user selection
                selection = document.selection;
                if (IsStringEqual(selection.type, SELECT_TYPE_CONTROL))
                {
                    range = (mshtmlControlRange)selection.createRange();
                }
            }
            catch (Exception)
            {
                // have unknow error so set return to null
                range = null;
            }

            return range;

        } //GetAllControls

        #endregion

        #region Utility Functions

        /// <summary>
        /// Method to perform a parse of a string into a byte number
        /// </summary>
        private byte TryParseByte(string stringValue, byte defaultValue)
        {
            // define the return type
            byte result;

            // try the conversion to a double number
            if (!byte.TryParse(stringValue, NumberStyles.Any, NumberFormatInfo.InvariantInfo, out result))
            {
                // default value will be returned
                result = defaultValue;
            }

            // return the byte value
            return result;

        } //TryParseNumber


        /// <summary>
        /// Method to perform a parse of a string into a ushort number
        /// </summary>
        private ushort TryParseUshort(string stringValue, ushort defaultValue)
        {
            // define the return type
            ushort result;

            // try the conversion to a double number
            if (!ushort.TryParse(stringValue, NumberStyles.Any, NumberFormatInfo.InvariantInfo, out result))
            {
                // default value will be returned
                result = defaultValue;
            }

            // return the ushort value
            return result;

        } //TryParseUshort

        
        /// <summary>
        /// Method to perform a parse of the string into an enum
        /// </summary>
        private object TryParseEnum(Type enumType, string stringValue, object defaultValue)
        {
            // try the enum parse and return the default 
            object result = defaultValue;
            try
            {
                // try the enum parse operation
                result = Enum.Parse(enumType, stringValue, true);
            }
            catch (Exception)
            {
                // default value will be returned
                result = defaultValue;
            }

            // return the enum value
            return result;

        } //TryParseEnum


        /// <summary>
        /// Method to ensure dialog resembles the user form characteristics
        /// </summary>
        protected virtual void PreShowDialog(Form dialog)
        {
            // set ambient control properties
//             dialog.Font = this.ParentForm.Font;
//             dialog.ForeColor = this.ParentForm.ForeColor;
//             dialog.BackColor = this.ParentForm.BackColor;
//             dialog.Cursor = this.ParentForm.Cursor;
//             dialog.RightToLeft = this.ParentForm.RightToLeft;

            // define location and control style as system
            dialog.StartPosition = FormStartPosition.CenterParent;

        } // PreShowDialog

        protected virtual void PostShowDialog(Form dialog)
        {
        } // PostShowDialog

		/// <summary>
		/// Method to determine if a string url is valid
		/// </summary>
		virtual protected bool IsValidHref(string href)
		{
			return Regex.IsMatch(href, HREF_TEST_EXPRESSION, RegexOptions.IgnoreCase);

		} //IsValidHref

        /// <summary>
        /// Method to determine if the tag name is of the correct type
        /// A string comparision is made whilst ignoring case
        /// </summary>
        private bool IsStringEqual(string tagText, string tagType)
        {
            return (string.Compare(tagText, tagType, true) == 0)? true : false;

        } //IsStringEqual


        /// <summary>
        /// Method to remove references to about:blank from the anchors
        /// </summary>
        private void RebaseAnchorUrl()
        {
            if (rebaseUrlsNeeded)
            {
                // review the anchors and remove any references to about:blank
                mshtmlElementCollection anchors = body.getElementsByTagName(ANCHOR_TAG);
                foreach (mshtmlElement element in anchors)
                {
                    try
                    {
                        mshtmlAnchorElement anchor = (mshtmlAnchorElement)element;
                        string href = anchor.href;
                        // see if href need updating
                        if (href != null && Regex.IsMatch(href, BLANK_HTML_PAGE, RegexOptions.IgnoreCase))
                        {
                            anchor.href = href.Replace(BLANK_HTML_PAGE, string.Empty);
                        }
                    }
                    catch (Exception)
                    {
                        // ignore any errors
                    }
                }
            }
            
        } //RebaseAnchorUrl

        #endregion

        #region Internal Event Processing

        /// <summary>
        /// Control the sizing of the browser control
        /// </summary>
        private void browserPanelResize(object sender, EventArgs e)
        {
            SetBrowserPanelSize();
        }

        /// <summary>
        /// Method to perform the sizing of the control panel
        /// </summary>
        private void SetBrowserPanelSize()
        {
            // define the browser panel to fill the remaining space
            if (_toolbarDock == DockStyle.Top || _toolbarDock == DockStyle.Bottom)
            {
                if (_toolbarVisible)
                {
                    if (_toolbarDock == DockStyle.Top)
                    {
                        this.browserPanel.Top = this.toolstripEditor.Bottom;
                    }
                    else
                    {
                        this.browserPanel.Top = 0;
                    }
                }
                else
                {
                    this.browserPanel.Top = 0;
                }

                this.browserPanel.Width = this.Width;
                this.browserPanel.Height = (_toolbarVisible) ? Math.Max(0, this.Height - this.toolstripEditor.Height) : this.Height;
            }
            else if (_toolbarDock == DockStyle.Left || _toolbarDock == DockStyle.Right)
            {
                //this.browserPanel.Left = 0;
                this.browserPanel.Width = (_toolbarVisible) ? Math.Max(0, this.Width - this.toolstripEditor.Width) : this.Width;
                this.browserPanel.Height = this.Height;
            }

            // define the web browser within the browser panel
            this.editorWebBrowser.Left = (_toolbarDock == DockStyle.Left) ? 0 : (int)_borderSize;
            this.editorWebBrowser.Top = (_toolbarDock == DockStyle.Top) ? 0 : (int)_borderSize;
            this.editorWebBrowser.Height = Math.Max(0, this.browserPanel.Height);// - (_borderSize * ((_toolbarDock == DockStyle.Left || _toolbarDock == DockStyle.Right) ? 2 : 1)));
            this.editorWebBrowser.Width = Math.Max(0, this.browserPanel.Width);// - (_borderSize * ((_toolbarDock == DockStyle.Top || _toolbarDock == DockStyle.Bottom) ? 2 : 1)));

            this.Update();
        } //SetBrowserPanelSize

        #endregion

        #region Toolbar Processing Operations

        protected System.Windows.Forms.ToolStrip ToolBar 
        {
            get { return toolstripEditor; }
        }

        protected System.Windows.Forms.Panel BrowserPanel
        {
            get { return browserPanel; }
        }

		protected System.Windows.Forms.WebBrowser WebBrowser
		{
			get { return editorWebBrowser; }
		}

		/// <summary>
		/// General Tool Strip processing method
		/// Calls the ProcessCommand with the selected command Tag Text
		/// </summary>
		private void toolstripEditorClick(object sender, EventArgs e)
        {
            ToolStripButton button = (ToolStripButton)sender;
            string command = (string)button.Tag;
            ProcessCommand(command);

        }

		private void toolstripEnableEditClick(object sender, EventArgs e)
		{
			EditEnabled = !EditEnabled;
		}

		/// <summary>
		/// General Context Meun processing method
		/// Calls the ProcessCommand with the selected command Tag Text
		/// </summary>
		private void contextEditorClick(object sender, EventArgs e)
        {
            ToolStripMenuItem menuItem = (ToolStripMenuItem)sender;
            string command = (string)menuItem.Tag;
            ProcessCommand(command);

        } //contextEditorClick


        /// <summary>
        /// Method to process the toolbar command and handle error exception
        /// </summary>
        private void ProcessCommand(string command)
        {
            try
            {
                // Evaluate the Button property to determine which button was clicked.
                switch(command)
                {
                    case INTERNAL_COMMAND_TEXTCUT:
                        // Browser CUT command
                        TextCut();
                        break; 
                    case INTERNAL_COMMAND_TEXTCOPY:
                        // Browser COPY command
                        TextCopy();
                        break;
                    case INTERNAL_COMMAND_TEXTPASTE:
                        // Browser PASTE command
                        TextPaste();
                        break; 
                    case INTERNAL_COMMAND_TEXTDELETE:
                        // Browser DELETE command
                        TextDelete();
                        break; 
                    case INTERNAL_COMMAND_CLEARSELECT:
                        // Clears user selection
                        TextClearSelect();
                        break;
                    case INTERNAL_COMMAND_SELECTALL:
                        // Selects all document content
                        TextSelectAll();
                        break; 
                    case INTERNAL_COMMAND_EDITUNDO:
                        // Undo the previous editing
                        EditUndo();
                        break;
                    case INTERNAL_COMMAND_EDITREDO:
                        // Redo the previous undo
                        EditRedo();
                        break;
                    case INTERNAL_COMMAND_FORMATBOLD:
                        // Selection BOLD command
                        FormatBold();
                        break;
                    case INTERNAL_COMMAND_FORMATUNDERLINE:
                        // Selection UNDERLINE command
                        FormatUnderline();
                        break;
                    case INTERNAL_COMMAND_FORMATITALIC:
                        // Selection ITALIC command
                        FormatItalic();
                        break;
                    case INTERNAL_COMMAND_FORMATSUPERSCRIPT:
                        // Selection SUPERSCRIPT command
                        FormatSuperscript();
                        break;
                    case INTERNAL_COMMAND_FORMATSUBSCRIPT:
                        // Selection SUBSCRIPT command
                        FormatSubscript();
                        break;
                    case INTERNAL_COMMAND_FORMATSTRIKEOUT:
                        // Selection STRIKEOUT command
                        FormatStrikeout();
                        break;
                    case INTERNAL_COMMAND_FONTDIALOG:
                        // FONT style creation
                        FormatFontAttributesPrompt();
                        break;
                    case INTERNAL_COMMAND_FONTNORMAL:
                        // FONT style remove
                        FormatRemove();
                        break;
                    case INTERNAL_COMMAND_COLORDIALOG:
                        // COLOR style creation
                        FormatFontColorPrompt();
                        break;
                    case INTERNAL_COMMAND_FONTINCREASE:
                        // FONTSIZE increase
                        FormatFontIncrease();
                        break;
                    case INTERNAL_COMMAND_FONTDECREASE:
                        // FONTSIZE decrease
                        FormatFontDecrease();
                        break;
                    case INTERNAL_COMMAND_JUSTIFYLEFT:
                        // Justify Left
                        JustifyLeft();
                        break;
                    case INTERNAL_COMMAND_JUSTIFYCENTER:
                        // Justify Center
                        JustifyCenter();
                        break;
                    case INTERNAL_COMMAND_JUSTIFYRIGHT:
                        // Justify Right
                        JustifyRight();
                        break;
                    case INTERNAL_COMMAND_FONTINDENT:
                        // Tab Right
                        FormatTabRight();
                        break;
                    case INTERNAL_COMMAND_FONTOUTDENT:
                        // Tab Left
                        FormatTabLeft();
                        break;
                    case INTERNAL_COMMAND_LISTORDERED:
                        // Ordered List
                        FormatList(HtmlListType.Ordered);
                        break;
                    case INTERNAL_COMMAND_LISTUNORDERED:
                        // Unordered List
                        FormatList(HtmlListType.Unordered);
                        break;
                    case INTERNAL_COMMAND_INSERTLINE:
                        // Horizontal Line
                        InsertLine();
                        break;
                    case INTERNAL_COMMAND_INSERTTABLE:
                        // Display a dialog to enable the user to insert a table
                        TableInsertPrompt();
                        break;
                    case INTERNAL_COMMAND_TABLEPROPERTIES:
                        // Display a dialog to enable the user to modify a table
                        TableModifyPrompt();
                        break;
                    case INTERNAL_COMMAND_TABLEINSERTROW:
                        // Display a dialog to enable the user to modify a table
                        TableInsertRow();
                        break;
                    case INTERNAL_COMMAND_TABLEDELETEROW:
                        // Display a dialog to enable the user to modify a table
                        TableDeleteRow();
                        break;
                    case INTERNAL_COMMAND_INSERTIMAGE:
                        // Display a dialog to enable the user to insert a image
                        InsertImagePrompt();
                        break;
                    case INTERNAL_COMMAND_INSERTLINK:
                        // Display a dialog to enable user to insert the href
                        InsertLinkPrompt();
                        break;
                    case INTERNAL_COMMAND_INSERTTEXT:
                        // Display a dialog to enable user to insert the text
                        InsertTextPrompt();
                        break;
                    case INTERNAL_COMMAND_INSERTHTML:
                        // Display a dialog to enable user to insert the html
                        InsertHtmlPrompt();
                        break;
                    case INTERNAL_COMMAND_FINDREPLACE:
                        // Display a dialog to enable user to perform find and replace operations
                        FindReplacePrompt();
                        break;
                    case INTERNAL_COMMAND_DOCUMENTPRINT:
                        // Print the current document
                        DocumentPrint();
                        break;
                    case INTERNAL_COMMAND_OPENFILE:
                        // Open a selected file
                        OpenFilePrompt();
                        break;
                    case INTERNAL_COMMAND_SAVEFILE:
                        // Saves the current document
                        SaveFilePrompt();
                        break;
                    case INTERNAL_TOGGLE_OVERWRITE:
                        // toggles the document overwrite method
                        ToggleOverWrite();
                        break;
                    case INTERNAL_TOGGLE_TOOLBAR:
                        // toggles the toolbar visibility
                        this.ToolbarVisible = !_toolbarVisible;
                        break;
                    case INTERNAL_TOGGLE_SCROLLBAR:
                        // toggles the scrollbar visibility
                        this.ScrollBars = (_scrollBars == DisplayScrollBarOption.No ? DisplayScrollBarOption.Auto : DisplayScrollBarOption.No);
                        break;
                    case INTERNAL_TOGGLE_WORDWRAP:
                        // toggles the document word wrapping
                        this.AutoWordWrap = !_autoWordWrap;
                        break;
                    case INTERNAL_FORMATTED_NORMAL:
                        // set the selected text to normal formatting
                        ProcessFormatBlock(HTML_FORMATTED_NORMAL);
                        break;
                    case INTERNAL_FORMATTED_HEADING1:
                        // set the selected text to heading1 formatting
                        ProcessFormatBlock(HTML_FORMATTED_HEADING1);
                        break;
                    case INTERNAL_FORMATTED_HEADING2:
                        // set the selected text to heading2 formatting
                        ProcessFormatBlock(HTML_FORMATTED_HEADING2);
                        break;
                    case INTERNAL_FORMATTED_HEADING3:
                        // set the selected text to heading3 formatting
                        ProcessFormatBlock(HTML_FORMATTED_HEADING3);
                        break;
                    case INTERNAL_FORMATTED_HEADING4:
                        // set the selected text to heading4 formatting
                        ProcessFormatBlock(HTML_FORMATTED_HEADING4);
                        break;
                    case INTERNAL_FORMATTED_HEADING5:
                        // set the selected text to heading5 formatting
                        ProcessFormatBlock(HTML_FORMATTED_HEADING5);
                        break;
                    case INTERNAL_FORMATTED_PRE:
                        // set the selected text to pre formatting
                        ProcessFormatBlock(HTML_FORMATTED_PRE);
                        break;
                    default:
                        throw new HtmlEditorException("Unknown Operation Being Performed.", command);
                }
            }
            catch (HtmlEditorException ex)
            {
                // process the html exception
                OnHtmlException(new HtmlExceptionEventArgs(ex.Operation, ex));
            }
            catch (Exception ex)
            {
                // process the exception
                OnHtmlException(new HtmlExceptionEventArgs(null, ex));
            }

            // ensure web browser has the focus after command execution
            Focus();
        
        } //ProcessCommand


        /// <summary>
        /// Method to process the format block command and handle error exception
        /// </summary>
        private void ProcessFormatBlock(string command)
        {
            try
            {
                // execute the insertion command
                InsertFormatBlock(command);
            }
            catch (HtmlEditorException ex)
            {
                // process the html exception
                OnHtmlException(new HtmlExceptionEventArgs(ex.Operation, ex));
            }
            catch (Exception ex)
            {
                // process the standard exception
                OnHtmlException(new HtmlExceptionEventArgs(null, ex));
            }

            // ensure web browser has the focus after command execution
            Focus();
        }


        /// <summary>
        /// Method to raise an event if a delegeate is assigned for handling exceptions
        /// </summary>
        private void OnHtmlException(HtmlExceptionEventArgs args)
        {
            if (HtmlException == null)
            {
                // obtain the message and operation
                // concatenate the message with any inner message
                string operation = args.Operation;
                Exception ex = args.ExceptionObject;
                string message = ex.Message;
                if (ex.InnerException != null)
                {
                    if (ex.InnerException.Message != null)
                    {
                        message = string.Format("{0}\n{1}", message, ex.InnerException.Message);
                    }
                }
                // define the title for the internal message box
                string title;
                if (operation == null || operation == string.Empty)
                {
                    title = "Unknown Error";
                }
                else
                {
                    title = operation + " Error";
                }
                // display the error message box
                MessageBox.Show(this, message, title, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                HtmlException(this, args);
            }
        }

        #endregion

    } //HtmlEditorControl
}
