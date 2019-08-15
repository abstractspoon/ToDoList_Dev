#region Using directives

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

using WinFormsSyntaxHighlighter;
using System.Text.RegularExpressions;

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

		private SyntaxHighlighter _syntaxHighlighting = null;

        // read only property for the form
        private bool _readOnly;

        // string values for the form title
        private const string editCommand = "Cancel";
        private const string viewCommand = "Close";

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
			this.htmlText.Text = string.Empty;
            this.ReadOnly = true;

        } //EditHtmlForm

		private void InitializeSyntaxHighlighting()
		{
			// Match Visual Studio as far as possible
			_syntaxHighlighting = new SyntaxHighlighter(htmlText);

			// Double quote strings
			//AddSyntaxPattern(@"\""([^""]|\""\"")*\""", Color.Blue);

			// Single quote strings
			//AddSyntaxPattern(@"\'([^']|\'\')*\'", Color.Blue);

			AddSyntaxPattern(new Regex(@"(<!--.*?-->)|(<!--.*)", RegexOptions.Singleline), Color.Green);

			// Attributes within tags
			// Must come before the tags themselves (strangely)
			{
				var attribs = @"([a-zA-Z][a-zA-Z0-9]*)";
				var withinTags = @"(?<=<[^>]+\s)({0})(?=[\s=>])";

				AddSyntaxPattern(string.Format(withinTags, attribs), Color.Red);
			}

			// Attribute equals (=)
			{/*
				var equals = @"(=)";
				var besideAttrib = @"(?<=<([^>]+))({0})(?=[a-zA-Z0-9""])";

				AddSyntaxPattern(string.Format(besideAttrib, equals), Color.Blue);
			*/}

			// Attribute values
			{/*
				var values = @"(.+?)";
				var besideAttrib = @"(?<=<([^>]+)=)({0})(?=[>/ ])";

				AddSyntaxPattern(string.Format(besideAttrib, values), Color.Blue);
			*/}

			// Tags within but excluding angled brackets
			{
				var tags = @"[a-zA-Z][a-zA-Z0-9]*";
				var withinAngledBrackets = @"(?<=</?)({0})(?=[/?>| ])";

				AddSyntaxPattern(string.Format(withinAngledBrackets, tags), Color.DarkRed);
			}

			// Angled brackets
			AddSyntaxPattern(@"</?", Color.Blue);
			AddSyntaxPattern(@"/?>", Color.Blue);
		}

		public void AddSyntaxPattern(string pattern, Color textColor, Color backColor, bool bold = false, bool italic = false, bool reHighlight = false)
		{
			// Ignore back color For now
			AddSyntaxPattern(pattern, textColor, bold, italic, reHighlight);
		}

		public void AddSyntaxPattern(string pattern, Color textColor, bool bold = false, bool italic = false, bool reHighlight = false)
		{
			AddSyntaxPattern(new PatternDefinition(pattern), new SyntaxStyle(textColor, bold, italic), reHighlight);
		}

		public void AddSyntaxPattern(Regex regex, Color textColor, Color backColor, bool bold = false, bool italic = false, bool reHighlight = false)
		{
			// Ignore back color For now
			AddSyntaxPattern(regex, textColor, bold, italic, reHighlight);
		}

		public void AddSyntaxPattern(Regex regex, Color textColor, bool bold = false, bool italic = false, bool reHighlight = false)
		{
			AddSyntaxPattern(new PatternDefinition(regex), new SyntaxStyle(textColor, bold, italic), reHighlight);
		}

		public void AddSyntaxPattern(PatternDefinition pattern, SyntaxStyle style, bool reHighlight = false)
		{
			_syntaxHighlighting.AddPattern(pattern, style);

			if (reHighlight)
				_syntaxHighlighting.ReHighlight();
		}

		/// <summary>
		/// Property to modify the caption of the display
		/// </summary>
		public void SetCaption(string caption)
        {
            this.Text = caption;
        }

		public string FontName
		{
			set
			{
				this.htmlText.Font = new Font(value, 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

				_syntaxHighlighting.ReHighlight();
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

				if (_syntaxHighlighting != null)
					_syntaxHighlighting.ReHighlight();
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
	}
}
