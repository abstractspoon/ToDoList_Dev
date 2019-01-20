namespace ZetaHtmlEditControl
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Drawing;
    using System.Globalization;
    using System.IO;
    using System.Runtime.InteropServices;
    using System.Runtime.InteropServices.ComTypes;
    using System.Text;
    using System.Text.RegularExpressions;
    using System.Windows.Forms;
    using Helper;
    using PInvoke;
    using mshtml;
    using Properties;

    /// <summary>
    /// Edit control, primarily designed to work in conjunction
    /// with the ZetaHelpdesk application.
    /// </summary>
    public partial class HtmlEditControl :
        ExtendedWebBrowser,
        UnsafeNativeMethods.IDocHostUIHandler,
        UnsafeNativeMethods.IServiceProvider,
        UnsafeNativeMethods.IOleClientSite,
        IDisposable
    {
        private const int WmKeydown = 0x100;
        private const int WmSyskeydown = 0x104;
        private const int WmDocumentComplete = 0x400;

        private void setMenuShortcutKeys()
        {
            deleteToolStripMenuItem.ShortcutKeys = Keys.Delete;
            copyToolStripMenuItem.ShortcutKeys = Keys.Control | Keys.C;
            pasteToolStripMenuItem.ShortcutKeys = Keys.Control | Keys.V;
            pasteAsTextToolStripMenuItem.ShortcutKeys = Keys.Control | Keys.Shift | Keys.V;
            justifyLeftToolStripMenuItem.ShortcutKeys = Keys.Control | Keys.L;
            justifyCenterToolStripMenuItem.ShortcutKeys = Keys.Control | Keys.E;
            justifyRightToolStripMenuItem.ShortcutKeys = Keys.Control | Keys.R;
            hyperLinkToolStripMenuItem.ShortcutKeys = Keys.Control | Keys.K;
            boldToolStripMenuItem.ShortcutKeys = Keys.Control | Keys.Shift | Keys.F;
            italicToolStripMenuItem.ShortcutKeys = Keys.Control | Keys.Shift | Keys.I;
            cutToolStripMenuItem.ShortcutKeys = Keys.Control | Keys.X;
        }

        /// <summary>
        /// <c>Uwe Keim</c>, 2006-03-17.
        /// </summary>
        public override bool PreProcessMessage(
            ref Message msg)
        {
            if (msg.Msg == WmKeydown || msg.Msg == WmSyskeydown)
            {
                var isShift = (ModifierKeys & Keys.Shift) != 0;

                var key = ((Keys)((int)msg.WParam));

                var e = new PreviewKeyDownEventArgs(key | ModifierKeys);

                // Check all shortcuts that I handle by myself.
                if (doHandleShortcutKey(e, false))
                {
                    return true;
                }
                else
                {
                    if (key == Keys.Enter)
                    {
                        // 2010-11-02, Uwe Keim:
                        // Just as in TortoiseSVN dialogs, use Ctrl+Enter as default button.
                        if (e.Control && !e.Alt && !e.Shift)
                        {
                            closeDialogWithOK();
                            return true;
                        }

                        if (!e.Alt && !e.Shift && !e.Control)
                        {
                            return handleEnterKey();
                        }
                        else
                        {
                            return false;
                        }
                    }
                    if (key == Keys.Tab)
                    {
                        // TAB key.
                        if (!e.Control && !e.Alt)
                        {
                            if (handleTabKeyInsideTable(isShift))
                            {
                                return true;
                            }
                            else
                            {
                                // Forward or backward?
                                var forward = !isShift;

                                var form = FindForm();
                                if (form != null)
                                {
                                    var c = form.GetNextControl(this, forward);

                                    while (c != null &&
                                        c != this &&
                                        !c.TabStop)
                                    {
                                        c = form.GetNextControl(c, forward);
                                    }

                                    if (c != null)
                                    {
                                        c.Focus();
                                    }
                                }
                                return false;
                            }
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
            }

            return base.PreProcessMessage(ref msg);
        }

        public event EventHandler WantCloseDialogWithOK;

        private void closeDialogWithOK()
        {
            var h = WantCloseDialogWithOK;
            if (h != null)
            {
                h(this, EventArgs.Empty);
            }
        }

        private static bool handleEnterKey()
        {
            return false;

            /*
			// 2010-10-28, Uwe Keim.
			// Parts taken from DevExpress's ASPxHtmlEditControl.

			var editMode = MainHost.Current.WebProject.SettingsGeneral.WebBrowserEditEnterMode;

			switch (editMode)
			{
				case WebBrowserEditEnterMode.BR:
					{
						var uniqueID = DateTime.Now.Ticks;
						InsertHtmlAtCurrentSelection(string.Format(@"<br id=""{0}"" />", uniqueID));

						if (Document == null) throw new NullReferenceException(@"Document is null.");
						var lineFeedNode = Document.GetElementById(uniqueID.ToString(CultureInfo.InvariantCulture));
						if (lineFeedNode == null) throw new NullReferenceException();
						var lfn = (IHTMLElement)lineFeedNode.DomElement;

						lfn.removeAttribute(@"id");

						var rng = CurrentSelectionText;
						rng.collapse();
						rng.select();

						return true;
					}

				case WebBrowserEditEnterMode.P:
					return false;
				default:
					throw new ArgumentOutOfRangeException();
			}
            */
        }

        /// <summary>
        /// Give derived classes the chance to handle the TAB key
        /// when inside a table.
        /// Return TRUE if handled, FALSE if not handled.
        /// </summary>
        private bool handleTabKeyInsideTable(
             bool isShift)
        {
            if (CanTableCellProperties || CanAddTableRow)
            {
                if (IsControlSelection)
                {
                    // The whole table is selected, add row at the end.

                    if (!isShift && CanAddTableRow)
                    {
                        ExecuteTableAddTableRow();
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    // A cell is selected. Move to next/previous cell
                    // or add row if on last.

                    var rowIndex = CurrentSelectionTableRowIndex;
                    var columnIndex = CurrentSelectionTableColumnIndex;

                    var rowCount = CurrentSelectionTableRowCount;
                    var columnCount = CurrentSelectionTableColumnCount;

                    if (isShift)
                    {
                        // Previous cell.

                        if (columnIndex == 0 && rowIndex == 0)
                        {
                            return false;
                        }
                        else if (columnIndex > 0)
                        {
                            // Previous cell.
                            var row = CurrentSelectionTableRow;

                            var element = (IHTMLElement)
                                row.cells.item(
                                columnIndex - 1,
                                columnIndex - 1);
                            MoveCaretToElement(element);
                            return true;
                        }
                        else
                        {
                            // Previous line, last cell.
                            var table = (IHTMLTable)CurrentSelectionTable;
                            var previousRow =
                                (IHTMLTableRow)
                                table.rows.item(
                                rowIndex - 1,
                                rowIndex - 1);

                            var element =
                                (IHTMLElement)
                                previousRow.cells.item(
                                    previousRow.cells.length - 1,
                                    previousRow.cells.length - 1);
                            MoveCaretToElement(element);
                            return true;
                        }
                    }
                    else
                    {
                        // Next cell.

                        if (columnIndex < columnCount - 1)
                        {
                            // Next cell.
                            var row =
                                CurrentSelectionTableRow;

                            var element =
                                row.cells.item(
                                columnIndex + 1,
                                columnIndex + 1) as IHTMLElement;
                            MoveCaretToElement(element);
                            return true;
                        }
                        else if (columnIndex == columnCount - 1 &&
                            rowIndex < rowCount - 1)
                        {
                            // Next row, first cell.
                            var table = (IHTMLTable)CurrentSelectionTable;
                            var nextRow = (IHTMLTableRow)
                                table.rows.item(
                                rowIndex + 1,
                                rowIndex + 1);

                            var element =
                                nextRow.cells.item(0, 0) as IHTMLElement;
                            MoveCaretToElement(element);
                            return true;
                        }
                        else
                        {
                            // Add new row.
                            ExecuteTableAddTableRow();
                            return true;
                        }
                    }
                }
            }
            else
            {
                return false;
            }
        }

        #region private class attributes
        // ------------------------------------------------------------------

        Timer _timerTextChange = new Timer();

        private HtmlConversionHelper _htmlConversionHelper;

        private string _tmpFolderPath = string.Empty;
        private int _objectID = 1;
        private const string _cssDefFontStyle = @"font-family: Segoe UI, Tahoma, Verdana, Arial; font-size: {font-size}; ";
        private const string _cssFontStyle = @"font-family: {font-name}, Segoe UI, Tahoma, Verdana, Arial; font-size: {font-size}; ";

        private string _cssText = _defaultCssText;
        private string _htmlTemplate = _defaultHtmlTemplate;

        /// <summary>
        /// Gets or sets the default CSS text.
        /// </summary>
        /// <value>The default CSS text.</value>
        public static string DefaultCssText
        {
            get
            {
                return _defaultCssText;
            }
            set
            {
                _defaultCssText = value;
            }
        }

        /// <summary>
        /// Gets or sets the default HTML template.
        /// </summary>
        /// <value>The default HTML template.</value>
        public static string DefaultHtmlTemplate
        {
            get
            {
                return _defaultHtmlTemplate;
            }
            set
            {
                _defaultHtmlTemplate = value;
            }
        }

        private static string _defaultCssText = 
@"body { {font-style}; margin: 4px; line-height: 110%; } 
li { margin-bottom: 5pt; } 
a { color: blue; } 
table { {font-style}; } 
tr { {font-style}; } 
th { padding:1px; border-top: 2px inset #777; border-right: 2px inset #fff; border-bottom: 2px inset #aaa; border-left: 2px inset #fff; font-weight: bold; {font-style}; } 
td { padding: 1px; border: 2px inset #fff; {font-style}; }";

        private static string _defaultHtmlTemplate =
@"<!DOCTYPE html PUBLIC ""-//W3C//DTD XHTML 1.0 Transitional//EN"" ""http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"">
<html xmlns=""http://www.w3.org/1999/xhtml"">
	<head>
        <meta http-equiv=""X-UA-Compatible"" content=""IE=9; charset=utf-8"" /> 
		<!--meta http-equiv=""Content-Type"" content=""text/html; charset=utf-8"" /-->
		<style type=""text/css"">##CSS##</style>
	</head>
	<body spellcheck=""true"">##BODY##</body>
</html>";

        // ------------------------------------------------------------------
        #endregion

        [Browsable(false)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public IExternalInformationProvider ExternalInformationProvider
        {
            get { return _externalInformationProvider; }
            set
            {
                _externalInformationProvider = value;
                _everLoadedTextModules = false; // Reset to force reload.
                updateUI();
            }
        }

        /// <summary>
        /// Makes the full HTML from body.
        /// </summary>
        /// <param name="body">The body.</param>
        /// <returns></returns>
        public string MakeFullHtmlFromBody(
            string body)
        {
            return doBuildCompleteHtml(
                body,
                _defaultHtmlTemplate,
                _defaultCssText);
        }

        public HtmlEditControl()
        {
            InitializeComponent();

            //AllowWebBrowserDrop = false;

            Navigate(@"about:blank");

            _htmlConversionHelper = new HtmlConversionHelper();

            //tmp dir
            _tmpFolderPath = Path.Combine(Path.GetTempPath(),
                Guid.NewGuid().ToString());

            if (!Directory.Exists(_tmpFolderPath))
            {
                Directory.CreateDirectory(_tmpFolderPath);
            }

            //timer:
            _timerTextChange.Tick += timerTextChange_Tick;

            _timerTextChange.Interval = 200;
            _timerTextChange.Start();

            // --

            setMenuShortcutKeys();
        }

        #region Public Members

        [Browsable(false)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public int TextChangeCheckInterval
        {
            get
            {
                return _timerTextChange.Interval;
            }
            set
            {
                if (value < 1000) //1 min
                {
                    _timerTextChange.Interval = value;
                }
            }
        }

        /// <summary>
        /// Assigns a style sheet to the HTML editor.
        /// Set<see cref="DocumentText"/>t to activate.
        /// </summary>
        [Browsable(false)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public string CssText
        {
            set
            {
                _cssText = value;
            }
        }

        /// <summary>
        /// Set own HTML Code.
        /// This '##BODY##' Tag will be replaced with the Body.
        /// Optional: '##CSS##'
        /// Set <see cref="DocumentText"/> to activate.
        /// </summary>
        [Browsable(false)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public string HtmlTemplate
        {
            set
            {
                if (string.IsNullOrEmpty(value))
                {
                    throw new ArgumentNullException(
                        @"value",
                        Resources.SR_HtmlEditControl_HtmlTemplate_AvaluefortheHtmlTemplatemustbeprovided);
                }
                else if (!value.Contains(@"##BODY##"))
                {
                    throw new ArgumentException(
                        Resources.SR_HtmlEditControl_HtmlTemplate_MissingBODYinsidetheHtmlTemplatepropertyvalue,
                        @"value");
                }
                else
                {
                    _htmlTemplate = value;
                }
            }
        }

        [Browsable(false)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public string CompleteDocumentText
        {
            get { return base.DocumentText; }
            set { base.DocumentText = value; }
        }

        /// <summary>
        /// Gets or sets the HTML content.
        /// </summary>
        [Browsable(false)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public new string DocumentText
        {
            get
            {
                return prepareDocumentTextGet(base.DocumentText);
            }
            set
            {
                base.DocumentText = prepareDocumentTextSet(value);
            }
        }

        /// <summary>
        /// Gets the document text and stores images to the given folder.
        /// </summary>
        /// <param name="externalImagesFolderPath">Folder path to store the images.</param>
        /// <returns>Returns the HTML code of the body.</returns>
        public string GetDocumentText(
            string externalImagesFolderPath)
        {
            return GetDocumentText(externalImagesFolderPath, false);
        }

        public string GetDocumentText(
            string externalImagesFolderPath,
            bool useImagesFolderPathPlaceHolder)
        {
            return
                _htmlConversionHelper.ConvertGetHtml(
                    DocumentText,
                    Document == null ? null : Document.Url,
                    externalImagesFolderPath,
                    useImagesFolderPathPlaceHolder ? ImagesFolderPathPlaceHolder : null);
        }

        public static string[] GetContainedImageFileNames(
            string html)
        {
            if (string.IsNullOrEmpty(html) || !html.Contains(ImagesFolderPathPlaceHolder))
            {
                return new string[] { };
            }
            else
            {
                using (var ch = new HtmlConversionHelper())
                {
                    return ch.GetContainedImageFileNames(html, ImagesFolderPathPlaceHolder);
                }
            }
        }

        /// <summary>
        /// Stand-alone function to expand any placeholders inside
        /// a given HTML fragment.
        /// </summary>
        public static string ExpandImageFolderPathPlaceHolder(
            string html,
            string externalImagesFolderPath)
        {
            if (string.IsNullOrEmpty(html) || !html.Contains(ImagesFolderPathPlaceHolder))
            {
                return html;
            }
            else
            {
                using (var ch = new HtmlConversionHelper())
                {
                    return ch.ConvertSetHtml(
                        html,
                        externalImagesFolderPath,
                        ImagesFolderPathPlaceHolder);
                }
            }
        }

        public static string ImagesFolderPathPlaceHolder
        {
            get
            {
                return @"http://pseudo-image-folder-path";
            }
        }

        public void SetDocumentText(
            string text)
        {
            SetDocumentText(text, null, false);
        }

        public void SetDocumentText(
            string text,
            string externalImagesFolderPath,
            bool useImagesFolderPathPlaceHolder)
        {
            DocumentText =
                _htmlConversionHelper.ConvertSetHtml(
                    text,
                    externalImagesFolderPath,
                    useImagesFolderPathPlaceHolder ? ImagesFolderPathPlaceHolder : null);
        }

        /// <summary>
        /// Gets the text-only content of the body. 
        /// </summary>
        public string TextOnlyFromDocumentBody
        {
            get
            {
                var tmp = getBodyFromHtmlCode(base.DocumentText);
                tmp = getOnlyTextFromHtmlCode(tmp);
                return tmp;
            }
        }
        #endregion

        #region PrepareDocumentTextSet + BuildCompleteHtml + ReplaceCss
        private string prepareDocumentTextSet(string html)
        {
            return buildCompleteHtml(getBodyFromHtmlCode(html));
        }

        private string buildCompleteHtml(string htmlBody)
        {
            return doBuildCompleteHtml(htmlBody, _htmlTemplate, _cssText);
        }

        private string doBuildCompleteHtml(
            string htmlBody,
            string htmlTemplate,
            string cssText)
        {
            string tmpHtml;
            if (string.IsNullOrEmpty(htmlTemplate))
            {
                tmpHtml = htmlBody;
            }
            else
            {
                tmpHtml = htmlTemplate;
                tmpHtml = tmpHtml.Replace(@"##BODY##", htmlBody);
            }

            tmpHtml = tmpHtml.Replace(@"##CSS##", replaceCss(cssText));

            return tmpHtml;
        }

        private string replaceCss(string cssText)
        {
            if (!string.IsNullOrEmpty(cssText) && cssText.Contains(@"{font-style}"))
            {
                var fontStyle = CssFontStyle;
                cssText = cssText.Replace(@"{font-style}", fontStyle);
            }

            return cssText;
        }

        public string CssFontSize
        {
            get { return string.IsNullOrEmpty(_cssFontSize) ? getCssFontSizeWithUnit() : _cssFontSize; }
            set { _cssFontSize = value; }
        }

        public string CssFontName
        {
            get { return _cssFontName; }
            set { _cssFontName = value; }
        }

        public string CssFontStyle
        {
            get 
            {
                string fontStyle = (string.IsNullOrEmpty(_cssFontName) ? 
                                    _cssDefFontStyle : 
                                    _cssFontStyle.Replace(@"{font-name}", _cssFontName));

                return fontStyle.Replace(@"{font-size}", CssFontSize);
            }
        }

        private string getCssFontSizeWithUnit()
        {
            // http://stackoverflow.com/questions/139655/convert-pixels-to-points
            var font = Font;

            switch (font.Unit)
            {
                case GraphicsUnit.World:
                case GraphicsUnit.Display:
                case GraphicsUnit.Inch:
                case GraphicsUnit.Document:
                case GraphicsUnit.Millimeter:
                    return string.Format(@"{0}pt", font.SizeInPoints);
                case GraphicsUnit.Pixel:
                    return string.Format(@"{0}px", font.Size);
                case GraphicsUnit.Point:
                    return string.Format(@"{0}pt", font.Size);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        #endregion

        #region PrepareDocumentTextGet
        private static string prepareDocumentTextGet(string html)
        {
            var s = getBodyFromHtmlCode(html);
            s = Regex.Replace(s, @"<![^>]*>", string.Empty, RegexOptions.Singleline);
            return s;
        }
        #endregion

        #region GetOnlyTextFromHtmlCode + RemoveHtmlChars + RemoveTagFromHtmlCode
        private static string getOnlyTextFromHtmlCode(string htmlCode)
        {
            //<br>
            htmlCode = htmlCode.Replace("\r\n", @" ");
            htmlCode = htmlCode.Replace("\r", @" ");
            htmlCode = htmlCode.Replace("\n", @" ");

            htmlCode = htmlCode.Replace(@"</p>", Environment.NewLine + Environment.NewLine);
            htmlCode = htmlCode.Replace(@"</P>", Environment.NewLine + Environment.NewLine);

            //html comment 
            htmlCode = Regex.Replace(
                htmlCode,
                @"<!--.*?-->",
                string.Empty,
                RegexOptions.Singleline | RegexOptions.IgnoreCase);

            //<p>
            htmlCode = Regex.Replace(htmlCode,
                @"<br[^>]*>",
                Environment.NewLine,
                RegexOptions.Singleline | RegexOptions.IgnoreCase);

            //tags
            htmlCode = removeTagFromHtmlCode(@"style", htmlCode);
            htmlCode = removeTagFromHtmlCode(@"script", htmlCode);

            //html
            htmlCode = Regex.Replace(
                htmlCode,
                "<(.|\n)+?>",
                string.Empty,
                RegexOptions.Singleline | RegexOptions.IgnoreCase);

            //umlaute
            htmlCode = unescapeHtmlEntities(htmlCode);

            //whitespaces
            htmlCode = Regex.Replace(
                htmlCode,
                @" +",
                @" ",
                RegexOptions.Singleline | RegexOptions.IgnoreCase);

            return htmlCode;
        }

        private static string unescapeHtmlEntities(
            string htmlCode)
        {
            htmlCode = htmlCode.Replace(@"&nbsp;", @" ");

            htmlCode = htmlCode.Replace(@"&Auml;", @"Ä");
            htmlCode = htmlCode.Replace(@"&absp;", @"ä");
            htmlCode = htmlCode.Replace(@"&obsp;", @"ö");
            htmlCode = htmlCode.Replace(@"&Obsp;", @"Ö");
            htmlCode = htmlCode.Replace(@"&ubsp;", @"ü");
            htmlCode = htmlCode.Replace(@"&Ubsp;", @"Ü");
            htmlCode = htmlCode.Replace(@"&szlig;", @"ß");

            htmlCode = htmlCode.Replace(@"&pound;", @"£");
            htmlCode = htmlCode.Replace(@"&sect;", @"§");
            htmlCode = htmlCode.Replace(@"&copy;", @"©");
            htmlCode = htmlCode.Replace(@"&reg;", @"®");
            htmlCode = htmlCode.Replace(@"&micro;", @"µ");
            htmlCode = htmlCode.Replace(@"&para;", @"¶");
            htmlCode = htmlCode.Replace(@"&Oslash;", @"Ø");
            htmlCode = htmlCode.Replace(@"&oslash;", @"ø");
            htmlCode = htmlCode.Replace(@"&divide;", @"÷");
            htmlCode = htmlCode.Replace(@"&times;", @"×");
            return htmlCode;
        }

        private static string removeTagFromHtmlCode(
            string tag,
            string htmlCode)
        {
            return Regex.Replace(
                htmlCode,
                string.Format(@"<{0}.*?</{1}>", tag, tag),
                string.Empty,
                RegexOptions.Singleline | RegexOptions.IgnoreCase);
        }
        #endregion

        private static string getBodyFromHtmlCode(
            string htmlCode)
        {
            if (string.IsNullOrEmpty(htmlCode))
            {
                return htmlCode;
            }
            else if (htmlCode.IndexOf(@"<body",
                StringComparison.InvariantCultureIgnoreCase) >= 0)
            {
                var regex = new Regex(
                    @".*?<body[^>]*>(.*?)</body>",
                    RegexOptions.Singleline | RegexOptions.IgnoreCase);

                var m = regex.Match(htmlCode);

                return m.Success ? m.Groups[1].Value : htmlCode;
            }
            else
            {
                return htmlCode;
            }
        }

        #region HandlePaste [CheckImages-GetSourceURLFromClipboardH...-GetHtmlFragmentFromC...]
        // ------------------------------------------------------------------

        private void insertHtmlFragmentAtCurrentPosition(string html)
        {
            if (Document != null)
            {
                var doc = (HTMLDocument)Document.DomDocument;

                if (isControlSelection)
                {
                    doc.execCommand(@"Delete", false, null);
                }

                var selection = doc.selection;
                var range = (IHTMLTxtRange)selection.createRange();
                range.pasteHTML(html);
            }
        }

        private void handlePaste(
            bool asTextOnly)
        {
            if (Document != null)
            {
                var doc = (HTMLDocument)Document.DomDocument;

                if (isControlSelection)
                {
                    doc.execCommand(@"Delete", false, null);
                }
                string html;

                if (Clipboard.ContainsImage())
                {
                    var image = Clipboard.GetImage();
                    var file = Path.Combine(_tmpFolderPath, _objectID.ToString(CultureInfo.InvariantCulture));
                    if (image != null)
                    {
                        image.Save(file, image.RawFormat);
                    }
                    _objectID++;
                    html = string.Format(@"<img src=""{0}"" id=""Img{1}"" />", file, DateTime.Now.Ticks);
                }
                else
                {
                    if (!asTextOnly && Clipboard.ContainsText(TextDataFormat.Html))
                    {
                        // Get HTML from Clipboard.
                        // Modified 2006-06-12, Uwe Keim.
                        string clipText;
                        byte[] originalBuffer;
                        getHtmlFromClipboard(out clipText, out originalBuffer);

                        //selected fragment
                        html = getHtmlFragmentFromClipboardCode(clipText, originalBuffer);

                        //only body from fragment
                        html = getBodyFromHtmlCode(html);

                        //images save or load from web
                        html = checkImages(HtmlConversionHelper.FindImgs(html), html,
                            getSourceUrlFromClipboardHtmlCode(clipText));
                    }
                    else if (Clipboard.ContainsText(TextDataFormat.UnicodeText))
                    {
                        html = Clipboard.GetText(TextDataFormat.UnicodeText);

                        if (asTextOnly)
                        {
                            html = getOnlyTextFromHtmlCode(html);
                        }

                        html = PathHelper.HtmlEncode(html);
                        html = addNewLineToText(html);
                    }
                    else if (Clipboard.ContainsText(TextDataFormat.Text))
                    {
                        html = Clipboard.GetText(TextDataFormat.Text);

                        if (asTextOnly)
                        {
                            html = getOnlyTextFromHtmlCode(html);
                        }

                        html = PathHelper.HtmlEncode(html);
                        html = addNewLineToText(html);
                    }
                    else
                    {
                        html = string.Empty;
                    }
                }

                var selection = doc.selection;
                var range = (IHTMLTxtRange)selection.createRange();
                range.pasteHTML(html);
            }
        }

        /*
                private static string getExtension(ImageFormat rawFormat)
                {
                    if (rawFormat==null)
                    {
                        return string.Empty;
                    }
                    else if (rawFormat.Guid == ImageFormat.Bmp.Guid)
                    {
                        return @".bmp";
                    }
                    else if (rawFormat.Guid == ImageFormat.MemoryBmp.Guid)
                    {
                        // Save memory BMP as PNG.
                        return @".png";
                    }
                    else if (rawFormat.Guid == ImageFormat.Gif.Guid)
                    {
                        return @".gif";
                    }
                    else if (rawFormat.Guid == ImageFormat.Jpeg.Guid)
                    {
                        return @".jpg";
                    }
                    else if (rawFormat.Guid == ImageFormat.Png.Guid)
                    {
                        return @".png";
                    }
                    else if (rawFormat.Guid == ImageFormat.Tiff.Guid)
                    {
                        return @".tif";
                    }
                    else
                    {
                        return string.Empty;
                    }
                }
        */

        /// <summary>
        /// See http://66.249.93.104/search?q=cache:yfQWT9XlYogJ:www.eggheadcafe.com/aspnet_answers/NETFrameworkNETWindowsForms/Apr2006/post26606306.asp+IDataObject+html+utf-8&amp;hl=de&amp;gl=de&amp;ct=clnk&amp;cd=1&amp;client=firefox-a
        /// See http://bakamachine.blogspot.com/2006/05/workarond-for-dataobject-html.html
        /// </summary>
        /// <remarks>Added 2006-06-12, <c>Uwe Keim</c>.</remarks>
        /// <returns></returns>
        private static void getHtmlFromClipboard(
            out string clipText,
            out byte[] originalBuffer)
        {
            originalBuffer = getHtml(Clipboard.GetDataObject());
            clipText = Encoding.UTF8.GetString(originalBuffer);
        }

        private static string addNewLineToText(string text)
        {
            if (string.IsNullOrEmpty(text))
            {
                return text;
            }
            else
            {
                text = text.Replace("\r\n", "\n");
                text = text.Replace("\r", "\n");
                text = text.Replace("\n", @"<br />");

                return text;
            }
        }

        private static string checkImages(
            ICollection<HtmlConversionHelper.ImageInfo> originalNames,
            string html,
            string url)
        {
            if (originalNames != null && originalNames.Count > 0)
            {
                foreach (var s in originalNames)
                {
                    if (!s.Source.StartsWith(@"http") && !s.Source.StartsWith(@"https"))
                    {
                        html = html.Replace(
                            s.Source,
                            HtmlConversionHelper.GetPathFromFile(s.Source,new Uri(url)));
                    }
                }

            }
            return html;
        }

        private static string getSourceUrlFromClipboardHtmlCode(
            string htmlCode)
        {
            var htmlInfo = htmlCode.Substring(0, htmlCode.IndexOf('<') - 1);

            var i = htmlInfo.IndexOf(@"SourceURL:", StringComparison.Ordinal);
            var url = htmlInfo.Substring(i + 10);
            url = url.Substring(0, url.IndexOf('\r'));
            return url;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <remarks>Modified 2006-06-12, <c>Uwe Keim</c>.</remarks>
        private static string getHtmlFragmentFromClipboardCode(
            string htmlCode,
            byte[] originalBuffer)
        {
            //split Html to htmlInfo (and htmlSource)
            var htmlInfo = htmlCode.Substring(0, htmlCode.IndexOf('<') - 1);

            //get Fragment positions
            var tmp = htmlInfo.Substring(htmlInfo.IndexOf(@"StartFragment:", StringComparison.Ordinal) + 14);
            tmp = tmp.Substring(0, tmp.IndexOf('\r'));
            var posStartSelection = Convert.ToInt32(tmp);

            tmp = htmlInfo.Substring(htmlInfo.IndexOf(@"EndFragment:", StringComparison.Ordinal) + 12);
            tmp = tmp.Substring(0, tmp.IndexOf('\r'));
            var posEndSelection = Convert.ToInt32(tmp);

            // get Fragment. Always UTF-8 as of spec.
            var s = Encoding.UTF8.GetString(
                originalBuffer,
                posStartSelection,
                posEndSelection - posStartSelection);

            return s;
        }

        // ------------------------------------------------------------------
        #endregion

        #region SelectionType (Control, Text, None)

        private string selectionType
        {
            get
            {
                if (string.IsNullOrEmpty(DocumentText))
                {
                    return string.Empty;
                }
                else
                {
                    if (Document == null)
                    {
                        return string.Empty;
                    }
                    else
                    {
                        var selection =
                            ((HTMLDocument)Document.DomDocument).selection;
                        return selection.type.ToLower();
                    }
                }
            }
        }

        private bool isControlSelection
        {
            get
            {
                return selectionType == @"control";
            }
        }

        private bool isTextSelection
        {
            get
            {
                return selectionType == @"text";
            }
        }

        /*
                private bool IsNoneSelection
                {
                    get
                    {
                        string seleType = SelectionType;
                        return seleType == @"none" || string.IsNullOrEmpty( seleType );
                    }
                }
        */
        #endregion

        #region EndCleanMembers
        // ------------------------------------------------------------------

        void IDisposable.Dispose()
        {
            endClean();
        }

        ~HtmlEditControl()
        {
            endClean();
        }

        private void endClean()
        {
            if (_timerTextChange != null)
            {
                _timerTextChange.Stop();
                _timerTextChange.Dispose();
                _timerTextChange = null;
            }

            if (!string.IsNullOrEmpty(_tmpFolderPath))
            {
                if (Directory.Exists(_tmpFolderPath))
                {
                    Directory.Delete(_tmpFolderPath, true);
                }
                _tmpFolderPath = null;
            }

            if (_htmlConversionHelper != null)
            {
                ((IDisposable)_htmlConversionHelper).Dispose();
                _htmlConversionHelper = null;
            }
        }

        // ------------------------------------------------------------------
        #endregion

        //protected override void OnPreviewKeyDown(
        //    PreviewKeyDownEventArgs e)
        //{
        //    //if (!doHandleShortcutKey(e, false))
        //    //{
        //    //    base.OnPreviewKeyDown(e);
        //    //}
        //}

        private bool doHandleShortcutKey(
            PreviewKeyDownEventArgs e,
            bool onlyCheck)
        {
            if (e.KeyCode == Keys.V && e.Control) //v + ctrl
            {
                if (!onlyCheck)
                {
                    handlePaste(false);
                }
                return true;
            }
            else if (e.KeyCode == Keys.I && e.Control && e.Shift && e.Alt) //ctrl + alt + shift + i.
            {
                if (!onlyCheck)
                {
                    ExecuteSystemInfo();
                }
                return true;
            }
            else if (e.KeyCode == Keys.Delete) //del
            {
                if (!onlyCheck)
                {
                    ExecuteDelete();
                }
                return true;
            }
            else if (e.KeyCode == Keys.X && e.Control) //x + ctrl
            {
                if (!onlyCheck)
                {
                    ExecuteCut();
                }
                return true;
            }
            else if (e.KeyCode == Keys.C && e.Control) //c + ctrl
            {
                if (!onlyCheck)
                {
                    ExecuteCopy();
                }
                return true;
            }
            else if (e.KeyCode == Keys.Z && e.Control) //z + ctrl
            {
                if (!onlyCheck)
                {
                    ExecuteUndo();
                }
                return true;
            }
            else if (e.KeyCode == Keys.Y && e.Control) //y + ctrl
            {
                if (!onlyCheck)
                {
                    ExecuteRedo();
                }
                return true;
            }
            else if (
                e.KeyCode == Keys.U && e.Control ||
                    e.KeyCode == Keys.U && e.Control && e.Shift) //u+ ctrl+ shift
            {
                if (!onlyCheck)
                {
                    ExecuteUnderline();
                }
                return true;
            }
            else if (
                e.KeyCode == Keys.I && e.Control ||
                    e.KeyCode == Keys.K && e.Control && e.Shift) //k+ ctrl+ shift
            {
                if (!onlyCheck)
                {
                    ExecuteItalic();
                }
                return true;
            }
            else if (
                e.KeyCode == Keys.B && e.Control ||
                    e.KeyCode == Keys.F && e.Control && e.Shift) //f+ ctrl+ shift
            {
                if (!onlyCheck)
                {
                    ExecuteBold();
                }
                return true;
            }
            else if (e.KeyCode == Keys.K && e.Control) //k+ ctrl
            {
                if (!onlyCheck)
                {
                    ExecuteInsertHyperlink();
                }
                return true;
            }
            else if (e.KeyCode == Keys.A && e.Control) //a+ ctrl
            {
                if (!onlyCheck)
                {
                    ExecuteSelectAll();
                }
                return true;
            }
            else if (e.KeyCode == Keys.E && e.Control) //E+ ctrl
            {
                if (!onlyCheck)
                {
                    ExecuteJustifyCenter();
                }
                return true;
            }
            else if (e.KeyCode == Keys.L && e.Control) //l+ ctrl
            {
                if (!onlyCheck)
                {
                    ExecuteJustifyLeft();
                }
                return true;
            }
            else if (e.KeyCode == Keys.R && e.Control) //r+ ctrl
            {
                if (!onlyCheck)
                {
                    ExecuteJustifyRight();
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        private void ExecuteSystemInfo()
        {
            var msg =
                string.Format(
                @"URL: {0}.",
                Url);

            MessageBox.Show(
                FindForm(),
                msg,
                Resources.HtmlEditControl_ExecuteSystemInfo_Information,
                MessageBoxButtons.OK,
                MessageBoxIcon.Information);
        }

        public bool IsBold
        {
            get
            {
                return (bool)DomDocument.queryCommandValue(@"Bold");
            }
        }

        public bool IsItalic
        {
            get
            {
                return (bool)DomDocument.queryCommandValue(@"Italic");
            }
        }

        public bool IsUnderline
        {
            get
            {
                return (bool)DomDocument.queryCommandValue(@"Underline");
            }
        }

        public bool IsOrderedList
        {
            get
            {
                return (bool)DomDocument.queryCommandValue(@"InsertOrderedList");
            }
        }

        public bool IsBullettedList
        {
            get
            {
                return (bool)DomDocument.queryCommandValue(@"InsertUnorderedList");
            }
        }

        public bool IsJustifyLeft
        {
            get
            {
                return (bool)DomDocument.queryCommandValue(@"JustifyLeft");
            }
        }

        public bool IsJustifyCenter
        {
            get
            {
                return (bool)DomDocument.queryCommandValue(@"JustifyCenter");
            }
        }

        public bool IsJustifyRight
        {
            get
            {
                return (bool)DomDocument.queryCommandValue(@"JustifyRight");
            }
        }

        private void ExecuteSelectAll()
        {
            if (Document != null)
            {
                var doc = (HTMLDocument)Document.DomDocument;
                doc.execCommand(@"SelectAll", false, null);
            }
        }

        internal void ExecuteUnderline()
        {
            if (Document != null)
            {
                var doc = (HTMLDocument)Document.DomDocument;
                doc.execCommand(@"Underline", false, null);
            }
        }

        public bool Undo()
        {
            if (!CanUndo)
                return false;

            ExecuteUndo();
            return true;
        }

        public bool Redo()
        {
            if (!CanRedo)
                return false;

            ExecuteRedo();
            return true;
        }

        private void ExecuteRedo()
        {
            if (Document != null) Document.ExecCommand(@"Redo", false, null);
        }

        internal void ExecuteUndo()
        {
            if (Document != null) Document.ExecCommand(@"Undo", false, null);
        }

        protected override void OnGotFocus(EventArgs e)
        {
            base.OnGotFocus(e);

            if (Document != null)
            {
                if (Document.Body != null)
                {
                    Document.Body.Focus();
                }
            }
        }

        private bool _firstCreate = true;

        protected override void OnNavigated(WebBrowserNavigatedEventArgs e)
        {
            base.OnNavigated(e);

            _documentLoaded = true;

            if (_firstCreate)
            {
                _firstCreate = false;

                // 2012-08-28, Uwe Keim: Enable gray shortcut texts.
                contextMenuStrip.Renderer = new MyToolStripRender();
            }

            // 2005-09-02: Can be null if showing a full-window PDF-viewer.
            if (DomDocument != null)
            {
                if (!_customDocUIHandlerSet)
                {
                    // Do this here, too.
                    // Enables this control to be called from the contained
                    // JavaScript on the loaded HTML document.
                    // See GetValueFromScript() and SetValueFromScript().
                    //ObjectForScripting = this;

                    _customDocUIHandlerSet = true;

                    var doc = DomDocument;
                    var cd = (UnsafeNativeMethods.ICustomDoc)doc;

                    // Set the IDocHostUIHandler.
                    cd.SetUIHandler(this);
                }

                // --

                var oe = (UnsafeNativeMethods.IOleObject)ActiveXInstance;

                // 2013-05-19, Uwe Keim:
                // Hier wird konfiguriert, dass diese Klasse IServiceProvider-Anfragen
                // erhalten kann.
                oe.SetClientSite(this);
            }
        }

        private string _tmpCacheTextChange = string.Empty;

        private void timerTextChange_Tick(
            object sender,
            EventArgs e)
        {
            if (!IsDisposed) // Uwe Keim 2006-03-17.
            {
                var s = DocumentText ?? string.Empty;

                if (_tmpCacheTextChange != s)
                {
                    var prevText = _tmpCacheTextChange;
                    _tmpCacheTextChange = s;

                    if (TextChanged != null)
                    {
                        if (!_ignoreNextChange)
                            TextChanged(this, new EventArgs());

                        _ignoreNextChange = false;
                    }
                }
            }
        }

        // Hack to ignore a spurious change that occurs
        // after content has been set on the control
        private bool _ignoreNextChange = false;

        protected override void OnDocumentCompleted(WebBrowserDocumentCompletedEventArgs e)
        {
            base.OnDocumentCompleted(e);

            _ignoreNextChange = true;
        }

        public new event EventHandler TextChanged;

        //#region MyClipboard

        //private int _clipboardPosition;
        //private const int ClipboardCapacity = 10;
        //private readonly List<string> _clipboard = new List<string>();
        private bool _documentLoaded;
        private bool _customDocUIHandlerSet;
        private bool _textModulesFilled;
        private bool _everLoadedTextModules;
        private TextModuleInfo[] _textModules;
        private IExternalInformationProvider _externalInformationProvider;
        private string _cssFontSize = @"8.25pt", _cssFontName = @"Tahoma";

        //private string myClipboard
        //{
        //    get
        //    {
        //        return _clipboard.Count > 0 ? _clipboard[_clipboardPosition] : null;
        //    }
        //    set
        //    {
        //        if (_clipboardPosition < _clipboard.Count - 1)
        //        {
        //            var i = _clipboardPosition;
        //            int c = _clipboard.Count - 2;
        //            while (c >= i)
        //            {
        //                _clipboard.Add(_clipboard[c]);
        //                c--;
        //            }
        //            _clipboard.Add(value);
        //        }
        //        else
        //        {
        //            _clipboard.Add(value);
        //        }
        //        while (_clipboard.Count > ClipboardCapacity)
        //        {
        //            _clipboard.RemoveAt(0);
        //        }
        //        _clipboardPosition = _clipboard.Count - 1;
        //    }
        //}
        //#endregion

        #region Helpers for HTML extraction.
        // ------------------------------------------------------------------

        /// <summary>
        /// Extracts data of type <c>Dataformat.Html</c> from an <c>IDataObject</c> data container
        /// This method shouldn't throw any exception but writes relevant exception informations in the debug window
        /// </summary>
        /// <param name="data">data container</param>
        /// <returns>A byte[] array with the decoded string or null if the method fails</returns>
        /// <remarks>Added 2006-06-12, <c>Uwe Keim</c>.</remarks>
        private static byte[] getHtml(
            System.Windows.Forms.IDataObject data)
        {
            var interopData = (System.Runtime.InteropServices.ComTypes.IDataObject)data;

            var format =
                new FORMATETC
                {
                    cfFormat = ((short)DataFormats.GetFormat(DataFormats.Html).Id),
                    dwAspect = DVASPECT.DVASPECT_CONTENT,
                    lindex = (-1),
                    tymed = TYMED.TYMED_HGLOBAL
                };

            STGMEDIUM stgmedium;
            stgmedium.tymed = TYMED.TYMED_HGLOBAL;
            stgmedium.pUnkForRelease = null;

            //try
            //{
            var queryResult = interopData.QueryGetData(ref format);
            //}
            //catch ( Exception exp )
            //{
            //	Debug.WriteLine( "HtmlFromIDataObject.GetHtml -> QueryGetData(ref format) threw an exception: "
            //		+ Environment.NewLine + exp.ToString() );
            //	return null;
            //}

            if (queryResult != 0)
            {
                //Debug.WriteLine(
                //    string.Format(
                //        @"HtmlFromIDataObject.GetHtml -> QueryGetData(ref format) returned a code != 0 code: {0}",
                //        queryResult));
                return null;
            }

            //try
            //{
            interopData.GetData(ref format, out stgmedium);
            //}
            //catch ( Exception exp )
            //{
            //	System.Diagnostics.Debug.WriteLine( "HtmlFromIDataObject.GetHtml -> GetData(ref format, out stgmedium) threw this exception: "
            //		+ Environment.NewLine + exp.ToString() );
            //	return null;
            //}

            if (stgmedium.unionmember == IntPtr.Zero)
            {
                //Debug.WriteLine(
                //    @"HtmlFromIDataObject.GetHtml -> stgmedium.unionmember returned an IntPtr pointing to zero");
                return null;
            }

            var pointer = stgmedium.unionmember;

            var handleRef = new HandleRef(null, pointer);

            byte[] rawArray;

            try
            {
                var ptr1 = GlobalLock(handleRef);

                var length = GlobalSize(handleRef);

                rawArray = new byte[length];

                Marshal.Copy(ptr1, rawArray, 0, length);
            }
            //catch ( Exception exp )
            //{
            //	Debug.WriteLine( "HtmlFromIDataObject.GetHtml -> Html Import threw an exception: " + Environment.NewLine + exp.ToString() );
            //}
            finally
            {
                GlobalUnlock(handleRef);

            }

            return rawArray;
        }

        [DllImport(@"kernel32.dll", CharSet = CharSet.Auto, ExactSpelling = true, SetLastError = true)]
        private static extern IntPtr GlobalLock(HandleRef handle);

        [DllImport(@"kernel32.dll", CharSet = CharSet.Auto, ExactSpelling = true, SetLastError = true)]
        private static extern bool GlobalUnlock(HandleRef handle);

        [DllImport(@"kernel32.dll", CharSet = CharSet.Auto, ExactSpelling = true, SetLastError = true)]
        private static extern int GlobalSize(HandleRef handle);

        [DllImport(@"user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        private static extern bool PostMessage(IntPtr hWnd, uint Msg, int wParam, int lParam);

        // ------------------------------------------------------------------
        #endregion

        private void contextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            updateUI();
        }

        private void updateUI()
        {
            if (Document != null && Document.DomDocument != null)
            {
                boldToolStripMenuItem.Enabled =
                    CanBold;
                italicToolStripMenuItem.Enabled =
                    CanItalic;

                cutToolStripMenuItem.Enabled =
                    CanCut;

                copyToolStripMenuItem.Enabled =
                    CanCopy;

                pasteAsTextToolStripMenuItem.Enabled =
                    pasteToolStripMenuItem.Enabled =
                    CanPaste;
                deleteToolStripMenuItem.Enabled =
                    CanDelete;

                indentToolStripMenuItem.Enabled =
                    CanIndent;
                justifyCenterToolStripMenuItem.Enabled =
                    CanJustifyCenter;
                justifyLeftToolStripMenuItem.Enabled =
                    CanJustifyLeft;
                justifyRightToolStripMenuItem.Enabled =
                    CanJustifyRight;
                numberedListToolStripMenuItem.Enabled =
                    CanOrderedList;
                outdentToolStripMenuItem.Enabled =
                    CanOutdent;
                bullettedListToolStripMenuItem.Enabled =
                    CanBullettedList;

                foreColorToolStripMenuItem.Enabled =
                    CanForeColor;
                backColorToolStripMenuItem.Enabled =
                    CanBackColor;

                hyperLinkToolStripMenuItem.Enabled =
                    CanInsertHyperlink;

                htmlToolStripMenuItem.Enabled = CanShowSource;

                // --

                UpdateUIContextMenuTable();

                textModulesToolStripMenuItem.Visible =
                    textModulesSeparator.Visible =
                    HasTextModules;
            }
        }

        internal bool CanOutdent
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"Outdent"));
            }
        }

        internal bool CanOrderedList
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"InsertOrderedList"));
            }
        }

        public bool CanUndo
        {
            get
            {
                return DomDocument.queryCommandEnabled(@"Undo");
            }
        }

        public bool CanRedo
        {
            get
            {
                return DomDocument.queryCommandEnabled(@"Redo");
            }
        }

        internal bool CanJustifyRight
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"JustifyRight"));
            }
        }

        internal bool CanJustifyLeft
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"JustifyLeft"));
            }
        }

        internal bool CanJustifyCenter
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"JustifyCenter"));
            }
        }

        internal bool CanIndent
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"Indent"));
            }
        }

        internal bool CanDelete
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"Delete"));
            }
        }

        internal bool CanPaste
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"Paste"));
            }
        }

        internal bool CanCopy
        {
            get { return Document != null && ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"Copy"); }
        }

        private bool CanCut
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"Cut"));
            }
        }

        internal bool CanItalic
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"Italic"));
            }
        }

        internal bool CanUnderline
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"Underline"));
            }
        }

        internal bool CanBold
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"Bold"));
            }
        }

        internal bool CanBullettedList
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"InsertUnorderedList"));
            }
        }

        internal bool CanForeColor
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"ForeColor"));
            }
        }

        internal bool CanBackColor
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"BackColor"));
            }
        }

        internal bool CanInsertHyperlink
        {
            get
            {
                return Document != null && (Enabled &&
                                            ((HTMLDocument)Document.DomDocument).queryCommandEnabled(@"CreateLink"));
            }
        }

        internal bool CanShowSource
        {
            get { return true; }
        }

        private void boldToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteBold();
        }

        internal void ExecuteBold()
        {
            if (Document != null)
            {
                var doc = (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"Bold", false, null);
            }
        }

        private void italicToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteItalic();
        }

        internal void ExecuteItalic()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"Italic", false, null);
            }
        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecutePaste();
        }

        internal void ExecutePaste()
        {
            handlePaste(false);
        }

        private void pasteAsTextToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecutePasteAsText();
        }

        internal void ExecutePasteAsText()
        {
            handlePaste(true);
        }

        private void htmlToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteShowSource();
        }

        internal void ExecuteShowSource()
        {
            using (var form = new HtmlSourceTextEditForm(DocumentText))
            {
                form.ExternalInformationProvider = ExternalInformationProvider;

                if (form.ShowDialog(this) == DialogResult.OK)
                {
                    DocumentText = form.HtmlText;
                    updateUI();
                }
            }
        }

        private void hyperLinkToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteInsertHyperlink();
        }

        internal void ExecuteInsertHyperlink()
        {
            if (Document != null)
            {
                var doc = (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"CreateLink", true, null);
            }
        }

        private void indentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteIndent();
        }

        internal void ExecuteIndent()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"Indent", false, null);
            }
        }

        private void justifyCenterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteJustifyCenter();
        }

        internal void ExecuteJustifyCenter()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"JustifyCenter", false, null);
            }
        }

        private void justifyLeftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteJustifyLeft();
        }

        internal void ExecuteJustifyLeft()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"JustifyLeft", false, null);
            }
        }

        private void justifyRightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteJustifyRight();
        }

        internal void ExecuteJustifyRight()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"JustifyRight", false, null);
            }
        }

        private void numberedListToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteNumberedList();
        }

        internal void ExecuteNumberedList()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"InsertOrderedList", false, null);
            }
        }

        private void outdentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteOutdent();
        }

        internal void ExecuteOutdent()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"Outdent", false, null);
            }
        }

        private void bullettedListToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteBullettedList();
        }

        internal void ExecuteBullettedList()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"InsertUnorderedList", false, null);
            }
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteCopy();
        }

        internal void ExecuteCopy()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"Copy", false, null);
            }
        }

        private void cutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteCut();
        }

        internal void ExecuteCut()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                if (isTextSelection)
                {
                    var range =
                        (IHTMLTxtRange)doc.selection.createRange();

                    Clipboard.SetText(range.htmlText);

                    // 2011-10-20, added.
                    ExecuteDelete();
                }
            }
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteDelete();
        }

        internal void ExecuteDelete()
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"Delete", false, null);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void setForeColor(
            string color)
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"ForeColor", false, color);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void setBackColor(
            string color)
        {
            if (Document != null)
            {
                var doc =
                    (HTMLDocument)Document.DomDocument;

                doc.execCommand(@"BackColor", false, color);
            }
        }

        private void foreColorNoneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColorNone();
        }

        internal void ExecuteSetForeColorNone()
        {
            setForeColor(@"windowtext");
        }

        private void foreColor01ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColor01();
        }

        internal void ExecuteSetForeColor01()
        {
            setForeColor(@"c00000");
        }

        private void foreColor02ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColor02();
        }

        internal void ExecuteSetForeColor02()
        {
            setForeColor(@"ff0000");
        }

        private void foreColor03ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColor03();
        }

        internal void ExecuteSetForeColor03()
        {
            setForeColor(@"ffc000");
        }

        private void foreColor04ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColor04();
        }

        internal void ExecuteSetForeColor04()
        {
            setForeColor(@"ffff00");
        }

        private void foreColor05ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColor05();
        }

        internal void ExecuteSetForeColor05()
        {
            setForeColor(@"92d050");
        }

        private void foreColor06ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColor06();
        }

        internal void ExecuteSetForeColor06()
        {
            setForeColor(@"00b050");
        }

        private void foreColor07ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColor07();
        }

        internal void ExecuteSetForeColor07()
        {
            setForeColor(@"00b0f0");
        }

        private void foreColor08ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColor08();
        }

        internal void ExecuteSetForeColor08()
        {
            setForeColor(@"0070c0");
        }

        private void foreColor09ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColor09();
        }

        internal void ExecuteSetForeColor09()
        {
            setForeColor(@"002060");
        }

        private void foreColor10ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetForeColor10();
        }

        internal void ExecuteSetForeColor10()
        {
            setForeColor(@"7030a0");
        }

        private void backColorNoneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetBackColorNone();
        }

        internal void ExecuteSetBackColorNone()
        {
            setBackColor(string.Empty);
            setBackColor(@"transparent");
            //setBackColor(@"window");
        }

        private void backColor01ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetBackColor01();
        }

        internal void ExecuteSetBackColor01()
        {
            setBackColor(@"ffff00");
        }

        private void backColor02ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetBackColor02();
        }

        internal void ExecuteSetBackColor02()
        {
            setBackColor(@"00ff00");
        }

        private void backColor03ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetBackColor03();
        }

        internal void ExecuteSetBackColor03()
        {
            setBackColor(@"00ffff");
        }

        private void backColor04ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetBackColor04();
        }

        internal void ExecuteSetBackColor04()
        {
            setBackColor(@"ff0000");
        }

        private void backColor05ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteSetBackColor05();
        }

        internal void ExecuteSetBackColor05()
        {
            setBackColor(@"ff00ff");
        }

        public void ExecuteInsertTable()
        {
            using (var form = new HtmlEditorTableNewForm())
            {
                form.ExternalInformationProvider = ExternalInformationProvider;

                if (form.ShowDialog(FindForm()) == DialogResult.OK)
                {
                    InsertHtmlAtCurrentSelection(form.Html);
                }
            }
        }

        public void InsertHtmlAtCurrentSelection(
            string html)
        {
            if (IsControlSelection)
            {
                // if its a control range, it must be deleted before.
                var sel = CurrentSelectionControl;
                sel.execCommand(@"Delete", false, null);
            }

            var sel2 = CurrentSelectionText;
            sel2.pasteHTML(html);
        }

        /// <summary>
        /// Gets the current selection.
        /// </summary>
        /// <param name="selectionMPStart">The selection MP start.</param>
        /// <param name="selectionMPEnd">The selection MP end.</param>
        public void GetCurrentSelection(
            out IMarkupPointer selectionMPStart,
            out IMarkupPointer selectionMPEnd)
        {
            // get markup container of the whole document.
            var mc = (IMarkupContainer)DomDocument;

            // get the markup services.
            var ms = (IMarkupServices)DomDocument;

            // create two markup pointers.
            ms.CreateMarkupPointer(out selectionMPStart);
            ms.CreateMarkupPointer(out selectionMPEnd);

            selectionMPStart.MoveToContainer(mc, NativeMethods.BOOL_TRUE);
            selectionMPEnd.MoveToContainer(mc, NativeMethods.BOOL_TRUE);

            // --
            // position start and end pointers around the current selection.

            IHTMLSelectionObject selection = DomDocument.selection;

            string st = selection.type.ToLowerInvariant();

            if (st == @"none")
            {
                var ds = (IDisplayServices)DomDocument;

                IHTMLCaret caret;
                ds.GetCaret(out caret);

                caret.MoveMarkupPointerToCaret(selectionMPStart);
                caret.MoveMarkupPointerToCaret(selectionMPEnd);

                // set gravity, as in "Introduction to Markup Services" in MSDN.
                selectionMPStart.SetGravity(_POINTER_GRAVITY.POINTER_GRAVITY_Right);
            }
            else if (st == @"text")
            {
                // MoveToSelectionAnchor does only work with "text" selections.
                var selectionText = DomDocument.selection;

                var range = (IHTMLTxtRange)selectionText.createRange();

                ms.MovePointersToRange(range, selectionMPStart, selectionMPEnd);

                // swap if wrong direction.
                if (compareGt(selectionMPStart, selectionMPEnd))
                {
                    var tmp = selectionMPStart;
                    selectionMPStart = selectionMPEnd;
                    selectionMPEnd = tmp;
                }

                // set gravity, as in "Introduction to Markup Services" in MSDN.
                selectionMPStart.SetGravity(_POINTER_GRAVITY.POINTER_GRAVITY_Right);
            }
            else if (st == @"control")
            {
                // MoveToSelectionAnchor does only work with "text" selections.
                var selectionControl = DomDocument.selection;

                var range = selectionControl.createRange()
                    as IHTMLControlRange;

                // Strangly, range was null sometimes.
                // E.g. when I resized a table (=control selection)
                // and then did an undo.
                if (range != null)
                {
                    if (range.length > 0)
                    {
                        var start = range.item(0);
                        var end = range.item(range.length - 1);

                        selectionMPStart.MoveAdjacentToElement(
                            start,
                            _ELEMENT_ADJACENCY.ELEM_ADJ_BeforeBegin);
                        selectionMPEnd.MoveAdjacentToElement(
                            end,
                            _ELEMENT_ADJACENCY.ELEM_ADJ_AfterEnd);
                    }
                }
            }
            else
            {
                // is there yet another selection type?
                //Debug.Assert(false);
            }
        }

        /// <summary>
        /// Gets the current selected element.
        /// </summary>
        /// <value>The current selected element.</value>
        public IHTMLElement CurrentSelectedElement
        {
            get
            {

                if (DomDocument.all.length == 0)
                {
                    return null;
                }
                else
                {
                    var txt = CurrentSelectionText;

                    if (txt != null)
                    {
                        return txt.parentElement();
                    }
                    else
                    {
                        var ctrl = CurrentSelectionControl;
                        return ctrl.commonParentElement();
                    }
                }
            }
        }

        public void ExecuteInsertTableRow()
        {
            var table = CurrentSelectionTable as IHTMLTable;

            if (table != null)
            {
                int rowIndex = CurrentSelectionTableRowIndex;

                IHTMLTableRow row =
                    HtmlEditorTableNewForm.AddTableRowsAfterRow(
                    table,
                    rowIndex,
                    1);

                // Set focus to first cell in the new line.
                if (row != null)
                {
                    var cell = row.cells.item(0, 0)
                        as IHTMLTableCell;
                    MoveCaretToElement(cell as IHTMLElement);
                }
            }
        }

        private void MoveCaretToElement(
            IHTMLElement element)
        {
            if (element != null)
            {
                var ms = (IMarkupServices)DomDocument;
                IMarkupPointer mp;
                ms.CreateMarkupPointer(out mp);

                var mp2 = (IMarkupPointer2)mp;
                mp2.MoveToContent(element, NativeMethods.BOOL_TRUE);

                var ds = (IDisplayServices)DomDocument;
                IDisplayPointer dp;
                ds.CreateDisplayPointer(out dp);

                dp.MoveToMarkupPointer(mp, null);

                // --

                IHTMLCaret caret;
                ds.GetCaret(out caret);

                caret.MoveCaretToPointer(
                    dp,
                    NativeMethods.BOOL_TRUE,
                    _CARET_DIRECTION.CARET_DIRECTION_SAME);
                caret.Show(NativeMethods.BOOL_TRUE);
            }
        }

        /// <summary>
        /// Executes the insert table column.
        /// </summary>
        public void ExecuteInsertTableColumn()
        {
            var table = CurrentSelectionTable as IHTMLTable;

            if (table != null)
            {
                int columnIndex = CurrentSelectionTableColumnIndex;

                HtmlEditorTableNewForm.AddTableColumnsAfterColumn(
                    table,
                    columnIndex,
                    1);
            }
        }

        /// <summary>
        /// Executes the table add table row.
        /// </summary>
        public void ExecuteTableAddTableRow()
        {
            var table = CurrentSelectionTable as IHTMLTable;

            if (table != null)
            {
                IHTMLTableRow row =
                    HtmlEditorTableNewForm.AddTableRowsAtBottom(
                    table,
                    1);

                MoveCaretToElement(row.cells.item(0, 0) as IHTMLElement);
            }
        }

        /// <summary>
        /// Executes the table add table column.
        /// </summary>
        public void ExecuteTableAddTableColumn()
        {
            var table = CurrentSelectionTable as IHTMLTable;

            if (table != null)
            {
                HtmlEditorTableNewForm.AddTableColumnsAtRight(
                    table,
                    1);
            }
        }

        /// <summary>
        /// Executes the table properties.
        /// </summary>
        public void ExecuteTableProperties()
        {
            var table = CurrentSelectionTable as IHTMLTable;

            if (table != null)
            {
                using (var form = new HtmlEditorTableNewForm())
                {
                    form.ExternalInformationProvider = ExternalInformationProvider;

                    form.Table = table;
                    form.ShowDialog(FindForm());
                }
            }
        }

        /// <summary>
        /// Executes the table delete row.
        /// </summary>
        public void ExecuteTableDeleteRow()
        {
            var table = CurrentSelectionTable as IHTMLTable;
            var rowIndex = CurrentSelectionTableRowIndex;

            if (table != null && rowIndex != -1)
            {
                table.deleteRow(rowIndex);
            }
        }

        /// <summary>
        /// Executes the table delete column.
        /// </summary>
        public void ExecuteTableDeleteColumn()
        {
            var table = CurrentSelectionTable as IHTMLTable;
            int columnIndex = CurrentSelectionTableColumnIndex;

            if (table != null && columnIndex != -1)
            {
                IHTMLElementCollection rows = table.rows;

                if (rows != null)
                {
                    for (int i = 0; i < rows.length; ++i)
                    {
                        var row = rows.item(i, i) as IHTMLTableRow;

                        if (row != null)
                        {
                            row.deleteCell(columnIndex);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Executes the table delete table.
        /// </summary>
        public void ExecuteTableDeleteTable()
        {
            var table = CurrentSelectionTable as IHTMLTable;

            if (table != null)
            {
                var tableNode = table as IHTMLDOMNode;

                if (tableNode != null)
                {
                    tableNode.removeNode(true);
                }
            }
        }

        /// <summary>
        /// Executes the table row properties.
        /// </summary>
        public void ExecuteTableRowProperties()
        {
            IHTMLTableRow row = CurrentSelectionTableRow;

            if (row != null)
            {
                using (var form = new HtmlEditorCellPropertiesForm())
                {
                    form.ExternalInformationProvider = ExternalInformationProvider;

                    form.Initialize(row);
                    form.ShowDialog(FindForm());
                }
            }
        }

        /// <summary>
        /// Executes the table column properties.
        /// </summary>
        public void ExecuteTableColumnProperties()
        {
            var table = CurrentSelectionTable as IHTMLTable;
            var columnIndex = CurrentSelectionTableColumnIndex;

            if (table != null && columnIndex >= 0)
            {
                using (var form = new HtmlEditorCellPropertiesForm())
                {
                    form.ExternalInformationProvider = ExternalInformationProvider;

                    form.Initialize(table, columnIndex);
                    form.ShowDialog(FindForm());
                }
            }
        }

        /// <summary>
        /// Executes the table cell properties.
        /// </summary>
        public void ExecuteTableCellProperties()
        {
            var cells = CurrentSelectionTableCells;

            if (cells != null && cells.Length > 0)
            {
                using (var form = new HtmlEditorCellPropertiesForm())
                {
                    form.ExternalInformationProvider = ExternalInformationProvider;

                    form.Initialize(cells);
                    form.ShowDialog(FindForm());
                }
            }
        }

        /// <summary>
        /// Gets a value indicating whether this instance can table properties.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can table properties; otherwise, <c>false</c>.
        /// </value>
        public bool CanTableProperties
        {
            get
            {
                return
                    IsTableCurrentSelectionInsideTable;
            }
        }

        /// <summary>
        /// Gets a value indicating whether this instance can add table row.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can add table row; otherwise, <c>false</c>.
        /// </value>
        public bool CanAddTableRow
        {
            get
            {
                return
                    IsTableCurrentSelectionInsideTable;
            }
        }

        /// <summary>
        /// Gets a value indicating whether this instance can add table column.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can add table column; otherwise, <c>false</c>.
        /// </value>
        public bool CanAddTableColumn
        {
            get
            {
                return
                    IsTableCurrentSelectionInsideTable;
            }
        }

        public bool CanInsertTable
        {
            get
            {
                return true;
            }
        }

        private void UpdateUIContextMenuTable()
        {
            insertNewTableToolStripMenuItem.Enabled = CanInsertTable;
            insertRowBeforeCurrentRowToolStripMenuItem.Enabled = CanInsertTableRow;
            insertColumnBeforeCurrentColumnToolStripMenuItem.Enabled = CanInsertTableColumn;
            addRowAfterTheLastTableRowToolStripMenuItem.Enabled = CanAddTableRow;
            addColumnAfterTheLastTableColumnToolStripMenuItem.Enabled = CanAddTableColumn;
            tablePropertiesToolStripMenuItem.Enabled = CanTableProperties;
            rowPropertiesToolStripMenuItem.Enabled = CanTableRowProperties;
            columnPropertiesToolStripMenuItem.Enabled = CanTableColumnProperties;
            cellPropertiesToolStripMenuItem.Enabled = CanTableCellProperties;
            deleteRowToolStripMenuItem.Enabled = CanTableDeleteRow;
            deleteColumnToolStripMenuItem.Enabled = CanTableDeleteColumn;
            deleteTableToolStripMenuItem.Enabled = CanTableDeleteTable;
        }

        /// <summary>
        /// Gets a value indicating whether this instance can insert table row.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can insert table row; otherwise, <c>false</c>.
        /// </value>
        public bool CanInsertTableRow
        {
            get
            {
                return
                    IsTableCurrentSelectionInsideTable;
            }
        }

        /// <summary>
        /// Gets a value indicating whether this instance can insert table column.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can insert table column; otherwise, <c>false</c>.
        /// </value>
        public bool CanInsertTableColumn
        {
            get
            {
                return
                    IsTableCurrentSelectionInsideTable;
            }
        }

        /// <summary>
        /// Gets a value indicating whether this instance can table delete row.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can table delete row; otherwise, <c>false</c>.
        /// </value>
        public bool CanTableDeleteRow
        {
            get
            {
                return
                    CurrentSelectionTableCell != null;
            }
        }

        /// <summary>
        /// Gets a value indicating whether this instance can table delete column.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can table delete column; otherwise, <c>false</c>.
        /// </value>
        public bool CanTableDeleteColumn
        {
            get
            {
                return
                    CurrentSelectionTableCell != null;
            }
        }

        /// <summary>
        /// Gets a value indicating whether this instance can table delete table.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can table delete table; otherwise, <c>false</c>.
        /// </value>
        public bool CanTableDeleteTable
        {
            get
            {
                return
                    CurrentSelectionTable != null;
            }
        }

        /// <summary>
        /// Gets a value indicating whether this instance can table row properties.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can table row properties; otherwise, <c>false</c>.
        /// </value>
        public bool CanTableRowProperties
        {
            get
            {
                return
                    CurrentSelectionTableRow != null;
            }
        }

        /// <summary>
        /// Gets a value indicating whether this instance can table column properties.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can table column properties; otherwise, <c>false</c>.
        /// </value>
        public bool CanTableColumnProperties
        {
            get
            {
                return
                    CurrentSelectionTableCell != null;
            }
        }

        /// <summary>
        /// Gets a value indicating whether this instance can table cell properties.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance can table cell properties; otherwise, <c>false</c>.
        /// </value>
        public bool CanTableCellProperties
        {
            get
            {
                IHTMLTableCell[] cells =
                    CurrentSelectionTableCells;

                return cells != null && cells.Length > 0;
            }
        }

        public bool IsTableSelection
        {
            get
            {
                if (!IsControlSelection)
                {
                    return false;
                }
                else
                {
                    var rng = CurrentSelectionControl;

                    if (rng.length <= 0)
                    {
                        return false;
                    }
                    else
                    {
                        var element = rng.item(0);

                        var tagName = element.tagName.ToLowerInvariant();
                        return tagName == @"table";
                    }
                }
            }
        }

        private IHTMLTable2 CurrentSelectionTable
        {
            get
            {
                // A complete table.
                if (IsTableSelection)
                {
                    var rng = CurrentSelectionControl;
                    var element = rng.item(0);

                    return element as IHTMLTable2;
                }
                // Inside a table (nested)?
                else
                {
                    IHTMLElement element;

                    if (IsControlSelection)
                    {
                        IHTMLControlRange rng = CurrentSelectionControl;
                        element = rng.item(0);
                    }
                    else
                    {
                        IHTMLTxtRange rng = CurrentSelectionText;
                        element = rng == null ? null : rng.parentElement();
                    }

                    while (element != null)
                    {
                        string tagName = element.tagName.ToLowerInvariant();

                        if (tagName == @"table")
                        {
                            return element as IHTMLTable2;
                        }
                        else
                        {
                            // Go up.
                            element = element.parentElement;
                        }
                    }

                    // Not found.
                    return null;
                }
            }
        }

        private bool IsTableCurrentSelectionInsideTable
        {
            get
            {
                return CurrentSelectionTable != null;
            }
        }

        private IHTMLTableRow CurrentSelectionTableRow
        {
            get
            {
                if (CurrentSelectionTable == null)
                {
                    return null;
                }
                // A complete table.
                else if (IsTableSelection || IsControlSelection)
                {
                    return null;
                }
                else
                {
                    // --
                    // Go up until the TR is found.

                    IHTMLElement element;

                    if (IsControlSelection)
                    {
                        IHTMLControlRange rng = CurrentSelectionControl;
                        element = rng.item(0);
                    }
                    else
                    {
                        IHTMLTxtRange rng = CurrentSelectionText;
                        element = rng == null ? null : rng.parentElement();
                    }

                    while (element != null)
                    {
                        string tagName = element.tagName.ToLowerInvariant();

                        if (tagName == @"tr")
                        {
                            return element as IHTMLTableRow;
                        }
                        else
                        {
                            // Go up.
                            element = element.parentElement;
                        }
                    }

                    // --

                    // Not found.
                    return null;
                }
            }
        }

        private IHTMLTableCell CurrentSelectionTableCell
        {
            get
            {
                if (CurrentSelectionTable == null)
                {
                    return null;
                }
                // A complete table.
                else if (IsTableSelection || IsControlSelection)
                {
                    return null;
                }
                else
                {
                    // --
                    // Go up until the TH or TD is found.

                    IHTMLElement element;

                    if (IsControlSelection)
                    {
                        IHTMLControlRange rng = CurrentSelectionControl;
                        element = rng.item(0);
                    }
                    else
                    {
                        IHTMLTxtRange rng = CurrentSelectionText;
                        element = rng == null ? null : rng.parentElement();
                    }

                    while (element != null)
                    {
                        string tagName = element.tagName.ToLowerInvariant();

                        if (tagName == @"th" || tagName == @"td")
                        {
                            return element as IHTMLTableCell;
                        }
                        else
                        {
                            // Go up.
                            element = element.parentElement;
                        }
                    }

                    // --

                    // Not found.
                    return null;
                }
            }
        }

        private IHTMLTableCell[] CurrentSelectionTableCells
        {
            get
            {
                var result = new List<IHTMLTableCell>();

                if (CurrentSelectionTable != null)
                {
                    IMarkupPointer mp1;
                    IMarkupPointer mp2;
                    GetCurrentSelection(out mp1, out mp2);

                    // --

                    // Walk from left to right of the current selection, 
                    // storing all TH and TD tags.

                    IMarkupPointer walk = mp1;

                    while (compareLte(walk, mp2))
                    {
                        // walk right.
                        _MARKUP_CONTEXT_TYPE context;
                        IHTMLElement element;
                        int minus1 = -1;
                        ushort unused;
                        walk.right(
                            NativeMethods.BOOL_TRUE,
                            out context,
                            out element,
                            ref minus1,
                            out unused);

                        if (element != null)
                        {
                            if (context ==
                                _MARKUP_CONTEXT_TYPE.CONTEXT_TYPE_EnterScope)
                            {
                                string tagName = element.tagName.ToLowerInvariant();

                                if (tagName == @"th" || tagName == @"td")
                                {
                                    result.Add(element as IHTMLTableCell);
                                }
                            }
                        }
                    }

                    // Nothing selected, just the carret inside a table cell?
                    if (result.Count <= 0)
                    {
                        // --
                        // Go up until the TH or TD is found.

                        IHTMLElement element;

                        if (IsControlSelection)
                        {
                            var rng = CurrentSelectionControl;
                            element = rng.item(0);
                        }
                        else
                        {
                            var rng = CurrentSelectionText;
                            element = rng.parentElement();
                        }

                        while (element != null)
                        {
                            string tagName = element.tagName.ToLowerInvariant();

                            if (tagName == @"th" || tagName == @"td")
                            {
                                result.Add(element as IHTMLTableCell);
                                break;
                            }
                            else
                            {
                                // Go up.
                                element = element.parentElement;
                            }
                        }
                    }
                }

                return result.ToArray();
            }
        }

        /// <summary>
        /// Returns "-1" if none/not found.
        /// </summary>
        /// <value>The index of the current selection table row.</value>
        private int CurrentSelectionTableRowIndex
        {
            get
            {
                IHTMLTableRow row =
                    CurrentSelectionTableRow;

                if (row == null)
                {
                    return -1;
                }
                else
                {
                    return row.rowIndex;
                }
            }
        }

        /// <summary>
        /// Returns "-1" if none/not found.
        /// </summary>
        /// <value>The index of the current selection table column.</value>
        private int CurrentSelectionTableColumnIndex
        {
            get
            {
                IHTMLTableCell cell =
                    CurrentSelectionTableCell;

                if (cell == null)
                {
                    return -1;
                }
                else
                {
                    return cell.cellIndex;
                }
            }
        }

        /// <summary>
        /// Returns "0" if none/not found.
        /// </summary>
        /// <value>The current selection table row count.</value>
        private int CurrentSelectionTableRowCount
        {
            get
            {
                var table =
                    CurrentSelectionTable as IHTMLTable;

                if (table == null)
                {
                    return 0;
                }
                else
                {
                    IHTMLElementCollection rows = table.rows;

                    if (rows == null)
                    {
                        return 0;
                    }
                    else
                    {
                        return rows.length;
                    }
                }
            }
        }

        /// <summary>
        /// Returns "0" if none/not found.
        /// </summary>
        /// <value>The current selection table column count.</value>
        private int CurrentSelectionTableColumnCount
        {
            get
            {
                IHTMLTableRow row =
                    CurrentSelectionTableRow;

                if (row == null)
                {
                    return 0;
                }
                else
                {
                    IHTMLElementCollection cells = row.cells;

                    if (cells == null)
                    {
                        return 0;
                    }
                    else
                    {
                        return cells.length;
                    }
                }
            }
        }

        public IHTMLDocument2 DomDocument
        {
            get
            {
                if (Document == null || Document.DomDocument == null)
                {
                    return null;
                }
                else
                {
                    return Document.DomDocument as IHTMLDocument2;
                }
            }
        }

        public IHTMLTxtRange CurrentSelectionText
        {
            get
            {
                if (DomDocument == null || DomDocument.all.length == 0)
                {
                    return null;
                }
                else
                {
                    var selection = DomDocument.selection;
                    var rangeDisp = selection.createRange();

                    var textRange = rangeDisp as IHTMLTxtRange;

                    return textRange;
                }
            }
        }

        public IHTMLControlRange CurrentSelectionControl
        {
            get
            {
                if (DomDocument == null || DomDocument.all.length == 0)
                {
                    return null;
                }
                else
                {
                    var selection = DomDocument.selection;
                    var rangeDisp = selection.createRange();

                    var textRange =
                        rangeDisp as IHTMLControlRange;

                    return textRange;
                }
            }
        }

        public bool IsControlSelection
        {
            get
            {
                IHTMLSelectionObject selection =
                    DomDocument.selection;

                string st = selection.type.ToLowerInvariant();

                return st == @"control";
            }
        }

        public bool IsTextSelection
        {
            get
            {
                var selection = DomDocument.selection;

                string st = selection.type.ToLowerInvariant();

                return st == @"text";
            }
        }

        public bool IsNoneSelection
        {
            get
            {
                var selection = DomDocument.selection;

                string st = selection.type.ToLowerInvariant();

                return st == @"none";
            }
        }

        #region Comparison of IMarkupPointer interfaces.
        // ------------------------------------------------------------------

        public static bool compareLt(
            IMarkupPointer p1,
            IMarkupPointer p2)
        {
            int flag;
            p1.IsLeftOf(p2, out flag);
            return flag == NativeMethods.BOOL_TRUE;
        }

        public static bool compareLte(
            IMarkupPointer p1,
            IMarkupPointer p2)
        {
            int flag;
            p1.IsLeftOfOrEqualTo(p2, out flag);
            return flag == NativeMethods.BOOL_TRUE;
        }

        public static bool CompareE(
            IMarkupPointer p1,
            IMarkupPointer p2)
        {
            int flag;
            p1.IsEqualTo(p2, out flag);
            return flag == NativeMethods.BOOL_TRUE;
        }

        public static bool compareGte(
            IMarkupPointer p1,
            IMarkupPointer p2)
        {
            int flag;
            p1.IsRightOfOrEqualTo(p2, out flag);
            return flag == NativeMethods.BOOL_TRUE;
        }

        public static bool compareGt(
            IMarkupPointer p1,
            IMarkupPointer p2)
        {
            int flag;
            p1.IsRightOf(p2, out flag);
            return flag == NativeMethods.BOOL_TRUE;
        }

        // ------------------------------------------------------------------
        #endregion

        /// <summary>
        /// Handles the Click event of the insertNewTableToolStripMenuItem 
        /// control.
        /// </summary>
        private void insertNewTableToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteInsertTable();
        }

        /// <summary>
        /// Handles the Click event of the 
        /// insertRowBeforeCurrentRowToolStripMenuItem control.
        /// </summary>
        private void insertRowBeforeCurrentRowToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteInsertTableRow();
        }

        /// <summary>
        /// Handles the Click event of the i
        /// nsertColumnBeforeCurrentColumnToolStripMenuItem control.
        /// </summary>
        private void insertColumnBeforeCurrentColumnToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteInsertTableColumn();
        }

        /// <summary>
        /// Handles the Click event of the 
        /// addRowAfterTheLastTableRowToolStripMenuItem control.
        /// </summary>
        private void addRowAfterTheLastTableRowToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteTableAddTableRow();
        }

        /// <summary>
        /// Handles the Click event of the 
        /// addColumnAfterTheLastTableColumnToolStripMenuItem control.
        /// </summary>
        private void addColumnAfterTheLastTableColumnToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteTableAddTableColumn();
        }

        private void tablePropertiesToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteTableProperties();
        }

        private void rowPropertiesToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteTableRowProperties();
        }

        private void columnPropertiesToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteTableColumnProperties();
        }

        private void cellPropertiesToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteTableCellProperties();
        }

        private void deleteRowToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteTableDeleteRow();
        }

        private void deleteColumnToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteTableDeleteColumn();
        }

        private void deleteTableToolStripMenuItem_Click(
            object sender,
            EventArgs e)
        {
            ExecuteTableDeleteTable();
        }

        protected virtual void OnUpdateUI()
        {
            if (UINeedsUpdate != null)
            {
                UINeedsUpdate(this, EventArgs.Empty);
            }
        }

        protected override void OnVisibleChanged(EventArgs e)
        {
            base.OnVisibleChanged(e);

            if (this.Visible && AllowWebBrowserDrop)
                AllowWebBrowserDrop = false;
        }

        /// <summary>
        /// Event handler indicating that the GUI needs to be updated
        /// (enabled/disable controls etc.).
        /// </summary>
        public event EventHandler UINeedsUpdate;

        #region IDocHostUIHandler members.
        // ------------------------------------------------------------------

        public int ShowContextMenu(
            int dwID,
            NativeMethods.POINT pt,
            object pcmdtReserved,
            object pdispReserved)
        {
            var kind =
                NativeMethods.ContextMenuKind.CONTEXT_MENU_DEFAULT;

            if (dwID == 0x02)
            {
                kind = NativeMethods.ContextMenuKind.CONTEXT_MENU_DEFAULT;
            }
            else if (dwID == 0x04)
            {
                kind = NativeMethods.ContextMenuKind.CONTEXT_MENU_CONTROL;
            }
            else if (dwID == 0x08)
            {
                kind = NativeMethods.ContextMenuKind.CONTEXT_MENU_TABLE;
            }
            else if (dwID == 0x10)
            {
                kind = NativeMethods.ContextMenuKind.CONTEXT_MENU_TEXTSELECT;
            }
            else if (dwID == 0x30)
            {
                kind = NativeMethods.ContextMenuKind.CONTEXT_MENU_ANCHOR;
            }
            else if (dwID == 0x20)
            {
                kind = NativeMethods.ContextMenuKind.CONTEXT_MENU_UNKNOWN;
            }

            var queryForStatus = pcmdtReserved as NativeMethods.IUnknown;
            var objectAtScreenCoordinates = pdispReserved as NativeMethods.IDispatch;

            if (OnNeedShowContextMenu(
                kind,
                new Point(pt.x, pt.y),
                queryForStatus,
                objectAtScreenCoordinates))
            {
                // Don't show MSHTML context menu but the one that will be attached
                // in a derived class.
                return NativeMethods.SRESULTS.S_OK;
            }
            else
            {
                // Let MSHTML show the context menu.
                return NativeMethods.SRESULTS.S_FALSE;
            }
        }

        protected virtual bool OnNeedShowContextMenu(
            NativeMethods.ContextMenuKind contextMenuKind,
            Point position,
            NativeMethods.IUnknown queryForStatus,
            NativeMethods.IDispatch objectAtScreenCoordinates)
        {
            if (ExternalInformationProvider != null)
            {
                var font = ExternalInformationProvider.Font;
                contextMenuStrip.Font = font ?? Font;
            }
            else
            {
                contextMenuStrip.Font = Font;
            }

            contextMenuStrip.Show(position);
            return true;
        }

        /// <summary>
        /// Gets the host info.
        /// </summary>
        /// <param name="info">The info.</param>
        /// <returns></returns>
        public int GetHostInfo(
            NativeMethods.DOCHOSTUIINFO info)
        {
            info.cbSize = Marshal.SizeOf(typeof(NativeMethods.DOCHOSTUIINFO));
            info.dwFlags = (int)(
                NativeMethods.DOCHOSTUIFLAG.DOCHOSTUIFLAG_NO3DOUTERBORDER |
                NativeMethods.DOCHOSTUIFLAG.DOCHOSTUIFLAG_NO3DBORDER |

                // http://msdn.microsoft.com/library/default.asp?url=/workshop/browser/hosting/reference/enum/dochostuiflag.asp
                // set the DOCHOSTUIFLAG_THEME if you want your buttons to have the XP look.
                NativeMethods.DOCHOSTUIFLAG.DOCHOSTUIFLAG_THEME);

            // default indicates we don't have info.
            return NativeMethods.SRESULTS.S_OK;
        }

        /// <summary>
        /// Shows the UI.
        /// </summary>
        /// <param name="dwID">The dw ID.</param>
        /// <param name="activeObject">The active object.</param>
        /// <param name="commandTarget">The command target.</param>
        /// <param name="frame">The frame.</param>
        /// <param name="doc">The doc.</param>
        /// <returns></returns>
        public int ShowUI(
            int dwID,
            UnsafeNativeMethods.IOleInPlaceActiveObject activeObject,
            NativeMethods.IOleCommandTarget commandTarget,
            UnsafeNativeMethods.IOleInPlaceFrame frame,
            UnsafeNativeMethods.IOleInPlaceUIWindow doc)
        {
            // default means we don't have any UI, and control should show its UI
            return NativeMethods.SRESULTS.S_FALSE;
        }

        public int HideUI()
        {
            // we don't have UI by default, so just pretend we hid it
            return NativeMethods.SRESULTS.S_OK;
        }

        public virtual int UpdateUI()
        {
            if (_documentLoaded)
            {
                OnUpdateUI();
            }

            return NativeMethods.SRESULTS.S_OK;
        }

        public int EnableModeless(
            bool fEnable)
        {
            // We don't have any UI by default, so pretend we updated it.
            return NativeMethods.SRESULTS.S_OK;
        }

        public int OnDocWindowActivate(
            bool fActivate)
        {
            // We don't have any UI by default, so pretend we updated it.
            return NativeMethods.SRESULTS.S_OK;
        }

        public int OnFrameWindowActivate(
            bool fActivate)
        {
            // We don't have any UI by default, so pretend we updated it.
            return NativeMethods.SRESULTS.S_OK;
        }

        public int ResizeBorder(
            NativeMethods.COMRECT rect,
            UnsafeNativeMethods.IOleInPlaceUIWindow doc,
            bool fFrameWindow)
        {
            // We don't have any UI by default, so pretend we updated it.
            return NativeMethods.SRESULTS.S_OK;
        }

        public int TranslateAccelerator(
            ref NativeMethods.MSG msg,
            ref Guid group,
            int nCmdID)
        {
            // No translation here.
            return NativeMethods.SRESULTS.S_FALSE;
        }

        public int GetOptionKeyPath(
            string[] pbstrKey,
            int dw)
        {
            // No replacement option key.
            return NativeMethods.SRESULTS.S_FALSE;
        }

        public int GetDropTarget(
            UnsafeNativeMethods.IOleDropTarget pDropTarget,
            out UnsafeNativeMethods.IOleDropTarget ppDropTarget)
        {
            // no additional drop target
            ppDropTarget = pDropTarget;
            return NativeMethods.SRESULTS.S_FALSE;
        }

        /// <summary>
        /// Gets the external.
        /// </summary>
        /// <param name="ppDispatch">The pp dispatch.</param>
        /// <returns></returns>
        public int GetExternal(
            out object ppDispatch)
        {
            // window.external from JavaScript.

            ppDispatch = this;
            return NativeMethods.SRESULTS.S_OK;
        }

        public int TranslateUrl(
            int dwTranslate,
            string strUrlIn,
            out string pstrUrlOut)
        {
            // no translation happens by default
            pstrUrlOut = strUrlIn;
            return NativeMethods.SRESULTS.S_FALSE;
        }

        public int FilterDataObject(
            System.Runtime.InteropServices.ComTypes.IDataObject pDo,
            out System.Runtime.InteropServices.ComTypes.IDataObject ppDoRet)
        {
            // no data object by default
            ppDoRet = pDo;
            return NativeMethods.SRESULTS.S_FALSE;
        }

        private void underlineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExecuteUnderline();
        }

        // ------------------------------------------------------------------
        #endregion

        public bool HasTextModules
        {
            get
            {
                checkGetTextModules();
                return _textModules != null && _textModules.Length > 0;
            }
        }

        internal void FillTextModules(
            ToolStripDropDownItem textModulesToolStripItem)
        {
            checkGetTextModules();

            textModulesToolStripItem.DropDownItems.Clear();

            foreach (var textModule in _textModules)
            {
                var mi = new ToolStripMenuItem(textModule.Name) { Tag = textModule };

                mi.Click += delegate
                    {
                        var tm = (TextModuleInfo)mi.Tag;
                        insertHtmlFragmentAtCurrentPosition(tm.Html);
                    };

                textModulesToolStripItem.DropDownItems.Add(mi);
            }
        }

        private void checkGetTextModules()
        {
            if (ExternalInformationProvider != null && !_everLoadedTextModules)
            {
                _everLoadedTextModules = true;
                _textModules = ExternalInformationProvider.GetTextModules();
            }
        }

        private void textModulesToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            if (!_textModulesFilled)
            {
                _textModulesFilled = true;
                FillTextModules(textModulesToolStripMenuItem);
            }
        }

        private readonly static Guid IID_IHTMLEditHost = new Guid(@"3050f6a0-98b5-11cf-bb82-00aa00bdce0b");
        private readonly static Guid SID_SHTMLEditHost = new Guid(@"3050f6a0-98b5-11cf-bb82-00aa00bdce0b");
        public const int E_NOINTERFACE = unchecked((int)0x80004002);
        public const int E_NOTIMPL = unchecked((int)0x80004001);
        public const int S_OK = 0;

        private readonly EditHost _editHost = new EditHost();

        private class EditHost :
            IHTMLEditHost
        {
            private readonly Dictionary<string, Size> _initialImgSizes = new Dictionary<string, Size>();

            void IHTMLEditHost.SnapRect(IHTMLElement pIElement, ref tagRECT prcNew, _ELEMENT_CORNER eHandle)
            {
                var img = pIElement as IHTMLImgElement;
                if (img != null)
                {
                    var key = string.Format(@"{0}-{1}", pIElement.id, img.src);

                    if (!_initialImgSizes.ContainsKey(key))
                    {
                        _initialImgSizes.Add(key, new Size(img.width, img.height));
                    }

                    var initialSize = _initialImgSizes[key];

                    switch (eHandle)
                    {
                        case _ELEMENT_CORNER.ELEMENT_CORNER_RIGHT:
                        case _ELEMENT_CORNER.ELEMENT_CORNER_LEFT:
                            {
                                var fac = initialSize.Height / (float)initialSize.Width;

                                var newWidth = prcNew.right - prcNew.left;
                                var newHeight = fac * newWidth;

                                // Niemals > 100%.
                                newWidth = Math.Min(newWidth, initialSize.Width);
                                newHeight = Math.Min(newHeight, initialSize.Height);

                                prcNew.right = prcNew.left + newWidth;
                                prcNew.bottom = (int)(prcNew.top + newHeight);

                                img.width = newWidth;
                                img.height = (int)newHeight;
                            }
                            break;
                        case _ELEMENT_CORNER.ELEMENT_CORNER_TOP:
                        case _ELEMENT_CORNER.ELEMENT_CORNER_BOTTOM:
                        case _ELEMENT_CORNER.ELEMENT_CORNER_BOTTOMLEFT:
                        case _ELEMENT_CORNER.ELEMENT_CORNER_BOTTOMRIGHT:
                        case _ELEMENT_CORNER.ELEMENT_CORNER_TOPLEFT:
                        case _ELEMENT_CORNER.ELEMENT_CORNER_TOPRIGHT:
                            {
                                var fac = initialSize.Width / (float)initialSize.Height;

                                var newHeight = prcNew.bottom - prcNew.top;
                                var newWidth = fac * newHeight;

                                // Niemals > 100%.
                                newWidth = Math.Min(newWidth, initialSize.Width);
                                newHeight = Math.Min(newHeight, initialSize.Height);

                                prcNew.right = (int)(prcNew.left + newWidth);
                                prcNew.bottom = prcNew.top + newHeight;

                                img.width = (int)newWidth;
                                img.height = newHeight;
                            }
                            break;

                        // TODO
                    }

                    //prcNew.right = prcNew.left + img.width;
                    //prcNew.bottom = prcNew.top + img.height;
                }
            }
        }

        // http://www.codeproject.com/Articles/6845/Implementing-snap-to-grid-in-an-MSHTML-based-appli
        // http://computer-programming-forum.com/4-csharp/42ab66e9d5d57b60.htm
        // https://code.google.com/p/slowandsteadyparser/source/browse/trunk/SlowAndSteadyParser/csExWB/General_Interfaces/IServiceProvider.cs


        int UnsafeNativeMethods.IServiceProvider.QueryService(ref Guid guidService, ref Guid riid, out IntPtr ppvObject)
        {
            if (guidService.Equals(SID_SHTMLEditHost) && riid.Equals(IID_IHTMLEditHost))
            {
                ppvObject = Marshal.GetComInterfaceForObject(_editHost, typeof(IHTMLEditHost));
                return S_OK;
            }
            else
            {
                ppvObject = IntPtr.Zero;
                return E_NOINTERFACE;
            }
        }

        int UnsafeNativeMethods.IOleClientSite.SaveObject()
        {
            return E_NOTIMPL;
        }

        int UnsafeNativeMethods.IOleClientSite.GetMoniker(uint dwAssign, uint dwWhichMoniker, out IMoniker ppmk)
        {
            ppmk = null;
            return E_NOTIMPL;
        }

        int UnsafeNativeMethods.IOleClientSite.ShowObject()
        {
            return S_OK;
        }

        int UnsafeNativeMethods.IOleClientSite.OnShowWindow(bool fShow)
        {
            return E_NOTIMPL;
        }

        int UnsafeNativeMethods.IOleClientSite.RequestNewObjectLayout()
        {
            return E_NOTIMPL;
        }
    }
}