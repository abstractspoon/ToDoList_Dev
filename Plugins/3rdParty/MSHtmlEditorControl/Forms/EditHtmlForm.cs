#region Using directives

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Text.RegularExpressions;

using RichEditExtensions;

#endregion


namespace MSDN.Html.Editor
{

	/// <summary>
	/// Form used to Edit or View Html contents
	/// If a property RedOnly is true contents are considered viewable
	/// No Html parsing is performed on the resultant data
	/// </summary>
	public partial class EditHtmlForm : Form
	{
		private class HtmlSyntaxPattern
		{
			public HtmlSyntaxPattern(string pattern, RegexOptions options, Color textColor, Color backColor, FontStyle style = FontStyle.Regular)
			{
				Pattern = new Regex(pattern, options);
				Style = FontStyle.Regular;
				TextColor = textColor;
				BackColor = backColor;
			}

			public Regex Pattern { get; set; }
			public FontStyle Style { get; set; }
			public Color TextColor { get; set; }
			public Color BackColor { get; set; }
		}

		// -------------------------------------------------------------------

		private bool _readOnly = false;
		private bool _highlighting = false;
		private bool _highlightNeeded = false;

		// string values for the form title
		private const string editCommand = "Cancel";
		private const string viewCommand = "Close";
		
		private List<HtmlSyntaxPattern> _syntaxPatterns = null;
		private HtmlSyntaxPattern _commentsPattern = null; // always run last

		private Timer _changeTimer = null;
		private RichOLE _reOLE = null;

		// -------------------------------------------------------------------

		/// <summary>
		/// Public Form constructor
		/// </summary>
		public EditHtmlForm()
        {
            //
            // Required for Windows Form Designer support
            //
            InitializeComponent();

			// Set up syntax highlighting
			InitializeSyntaxHighlighting();
			
			// ensure content is empty
			this.HTML = string.Empty;
            this.ReadOnly = true;

		} //EditHtmlForm

		private void InitializeSyntaxHighlighting()
		{
			// Match Visual Studio as far as possible
			_syntaxPatterns = new List<HtmlSyntaxPattern>();
			_reOLE = new RichOLE(this.htmlText);

			// Tags within but excluding angled brackets
			{
				var tags = @"[a-zA-Z][a-zA-Z0-9]*";
				var withinAngledBrackets = @"(?<=</?)({0})(?=[/?>| ])";

				AddSyntaxPattern(string.Format(withinAngledBrackets, tags), Color.DarkRed);
			}

			// Attributes within tags
			{
				var attribs = @"([a-zA-Z][a-zA-Z0-9]*)";
				var withinTags = @"(?<=<[^>]+\s)({0})(?=[\s=>])";

				AddSyntaxPattern(string.Format(withinTags, attribs), Color.Red);
			}

			// Attribute values
			{
				var besideAttrib = @"(?<=<([^>]+))({0})(?=[>/ ])";

				// Unquoted values
				var values = @"(=\s*.+?)";
				AddSyntaxPattern(string.Format(besideAttrib, values), Color.Blue);

				// Double quoted values
				values = @"\""([^""]|\""\"")*\""";
				AddSyntaxPattern(string.Format(besideAttrib, values), Color.Blue);
			}

			// Angled brackets
			AddSyntaxPattern(@"</?", Color.Blue);
			AddSyntaxPattern(@"/?>", Color.Blue);

			// Comments kept separate to overwrite any other (custom) formatting
			_commentsPattern = new HtmlSyntaxPattern(@"(<!--.*?-->)|(<!--.*)", RegexOptions.Singleline, Color.Green, Color.Transparent, FontStyle.Regular);
			
			// Change handling
			_changeTimer = new Timer();
			_changeTimer.Tick += new EventHandler(OnChangeTimer);
			_changeTimer.Interval = 500;
			_changeTimer.Start();

			this.htmlText.TextChanged += new System.EventHandler(this.OnTextChanged);
		}

		public void AddSyntaxPattern(string pattern, Color textColor, FontStyle style = FontStyle.Regular)
		{
			AddSyntaxPattern(pattern, RegexOptions.None, textColor, SystemColors.Window, style);
		}

		public void AddSyntaxPattern(string pattern, Color textColor, Color backColor, FontStyle style = FontStyle.Regular)
		{
			AddSyntaxPattern(pattern, RegexOptions.None, textColor, backColor, style);
		}

		public void AddSyntaxPattern(string pattern, RegexOptions options, Color textColor, FontStyle style = FontStyle.Regular)
		{
			AddSyntaxPattern(pattern, options, textColor, SystemColors.Window, style);
		}

		public void AddSyntaxPattern(string pattern, RegexOptions options, Color textColor, Color backColor, FontStyle style = FontStyle.Regular)
		{
			_syntaxPatterns.Add(new HtmlSyntaxPattern(pattern, options, textColor, backColor, style));
			_highlightNeeded = true;
		}

		private void OnChangeTimer(object sender, EventArgs e)
		{
			if (IsDisposed)
				return;

			RefreshHighlighting();
		}

		/// <summary>
		/// Property to modify the caption of the display
		/// </summary>
		public void SetCaption(string caption)
        {
            this.Text = caption;
        }

		private void RefreshHighlighting()
		{
			if (_highlightNeeded && (_syntaxPatterns != null) && (htmlText != null) && !String.IsNullOrWhiteSpace(htmlText.Text))
			{
				htmlText.DoActionWithHoldRedraw(DoHighlighting);
			}
		}

		private void DoHighlighting()
		{
			_highlightNeeded = false;
			_highlighting = true;
			_reOLE.EnableUndo(false);

			// Clear any existing formatting
			htmlText.SelectionStart = 0;
			htmlText.SelectionLength = htmlText.Text.Length;
			htmlText.SelectionColor = htmlText.ForeColor;
			htmlText.SelectionFont = htmlText.Font;

			foreach (var syntax in _syntaxPatterns)
			{
				HightlightPattern(syntax);
			}

			// Comments always comes last
			HightlightPattern(_commentsPattern);

			_reOLE.EnableUndo(true);
			_highlighting = false;
		}

		private void HightlightPattern(HtmlSyntaxPattern syntax)
		{
			MatchCollection matches = syntax.Pattern.Matches(htmlText.Text);

			foreach (Match match in matches)
			{
				htmlText.SelectionStart = match.Index;
				htmlText.SelectionLength = match.Length;
				htmlText.SelectionColor = syntax.TextColor;
				htmlText.SelectionBackColor = syntax.BackColor;

				if (syntax.Style == FontStyle.Regular)
					htmlText.SelectionFont = htmlText.Font;
				else
					htmlText.SelectionFont = new Font(htmlText.Font, syntax.Style);
			}
		}

		public string FontName
		{
			set
			{
				this.htmlText.Font = new Font(value, 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

				_highlightNeeded = true;
			}
		}

        /// <summary>
        /// Property to set and get the HTML contents
        /// </summary>
        public string HTML
        {
            get
            {
                return this.htmlText.Text.Trim();
            }
            set
            {
                this.htmlText.Text = (value != null)?value.Trim():string.Empty;
                this.htmlText.SelectionStart = 0;
                this.htmlText.SelectionLength = 0;

				_highlightNeeded = true;
            }

        } //HTML

        /// <summary>
        /// Property that determines if the html is editable
        /// </summary>
        public bool ReadOnly
        {
            get
            {
                return _readOnly;
            }
            set
            {
                _readOnly = value;
                this.bOK.Visible = !_readOnly;
                this.htmlText.ReadOnly = _readOnly;
                this.bCancel.Text = _readOnly?viewCommand:editCommand;
            }

        } //ReadOnly

		private void OnTextChanged(object sender, EventArgs e)
		{
			if (!_highlighting)
				_highlightNeeded = true;
		}
	}
}
