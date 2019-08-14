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
			_syntaxHighlighting = new SyntaxHighlighter(htmlText);

			// double quote strings
			_syntaxHighlighting.AddPattern(new PatternDefinition(@"\""([^""]|\""\"")*\"""), new SyntaxStyle(Color.Blue));

			// single quote strings
			_syntaxHighlighting.AddPattern(new PatternDefinition(@"\'([^']|\'\')*\'"), new SyntaxStyle(Color.Salmon));

			//_syntaxHighlighting.AddPattern(new PatternDefinition(@"</?([A-Za-z][^\s>/]*)(?:\s*(?:""[^\""]*\""|'[^']*'|[^\s>]+)|[^>])*(?:>|$)"), new SyntaxStyle(Color.Salmon));
			AddHtmlPattern("SPAN", Color.Red);
			AddHtmlPattern("P", Color.DarkCyan);
			AddHtmlPattern("A", Color.Blue);
			AddHtmlPattern("TABLE", Color.Tan);
			AddHtmlPattern("TR", Color.Brown);
			AddHtmlPattern("TD", Color.Brown);
			AddHtmlPattern("IMG", Color.Red);
			// 
			// 			// <, >
			// 			_syntaxHighlighting.AddPattern(new PatternDefinition(@"</?"), new SyntaxStyle(Color.Blue));
			// 			_syntaxHighlighting.AddPattern(new PatternDefinition(@"/?>"), new SyntaxStyle(Color.Blue));
		}

		private void AddHtmlPattern(string keyword, Color color)
		{
			var regex = string.Format(@"</?({0}[^\s>/]*)(?:\s*(?:""[^\""]*\""|'[^']*'|[^\s>]+)|[^>])*(?:>|$)", keyword);
			_syntaxHighlighting.AddPattern(new PatternDefinition(regex), new SyntaxStyle(color));
			// Begin tag
// 			var regex = string.Format(@"<({0}\b)", keyword);
// 			_syntaxHighlighting.AddPattern(new PatternDefinition(new Regex(regex, RegexOptions.IgnoreCase)), new SyntaxStyle(color));
// 
// 			// End Tag
// 			regex = string.Format("(/{0})>", keyword);
// 			_syntaxHighlighting.AddPattern(new PatternDefinition(new Regex(regex, RegexOptions.IgnoreCase)), new SyntaxStyle(color));
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

		public SyntaxHighlighter SyntaxHighlighting
		{
			get { return _syntaxHighlighting; }
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
