using System.Windows.Forms;

namespace ZetaHtmlEditControl
{
	using System;

	public partial class HtmlEditUserControl : UserControl
	{
		private readonly float _initialTopHeight;
		private bool _textModulesFilled;

		public HtmlEditUserControl()
		{
			InitializeComponent();

			_initialTopHeight = tableLayoutPanel.RowStyles[0].Height;

			htmlEditControl.UINeedsUpdate += htmlEditControl_UINeedsUpdate;
		}

        protected override void OnGotFocus(EventArgs e)
        {
            htmlEditControl.Focus();
        }

        protected override void OnHandleCreated(EventArgs e)
        {
            base.OnHandleCreated(e);
            applyFont();
            
            foreach (var family in System.Drawing.FontFamily.Families) 
            { 
                HTMLcmbFont.Items.Add(family.Name); 
            }

            HTMLcmbFont.SelectedIndex = HTMLcmbFont.FindStringExact(HtmlEditControl.CssFontName);
            HTMLcmbFontSize.SelectedIndex = HTMLcmbFontSize.FindStringExact("2");

            HTMLcmbFont.SelectedIndexChanged += HTMLcmbFontSelectedIndexChanged;
            HTMLcmbFontSize.SelectedIndexChanged += HTMLcmbFontSizeSelectedIndexChanged;

            SetCssFontNameAndSize(HtmlEditControl.CssFontName);
        }

        public bool SetCssFontNameAndSize(String fontName, float fontSize = 0, bool points = true)
        {
            if (string.IsNullOrEmpty(fontName))
                return false;

            // Save the current document to update later
            var docText = HtmlEditControl.DocumentText;

            // Reset the font name selection and then update to the current selection
            if (HTMLcmbFont.Items.Count > 0)
            {
                int fontIndex = HTMLcmbFont.FindStringExact(fontName);

                if (fontIndex == -1)
                    return false;

                HTMLcmbFont.SelectedIndex = fontIndex;

                updateButtons();
            }

            HtmlEditControl.CssFontName = fontName;

            if (fontSize > 0)
                HtmlEditControl.CssFontSize = string.Format((points ? "{0}pt" : "{0}px"), fontSize);

            // Rebuild the document text to update the browser CSS
            HtmlEditControl.DocumentText = docText;

            return true;
        }

		private void htmlEditControl_UINeedsUpdate(object sender, EventArgs e)
        {
			updateButtons();
		}

		public HtmlEditControl HtmlEditControl
		{
			get { return htmlEditControl; }
		}

        public ToolStrip ToolBar
        {
            get { return topToolStrip; }
        }

		public bool IsToolbarVisible
		{
			get { return topToolStrip.Visible; }
			set
			{
			    applyFont();

                // --

				if (topToolStrip.Visible != value)
				{
					topToolStrip.Visible = value;

					tableLayoutPanel.RowStyles[0].Height = value ? _initialTopHeight : 0;
				}
			}
		}

	    private void applyFont()
	    {
            if (htmlEditControl.ExternalInformationProvider != null)
            {
                var font = htmlEditControl.ExternalInformationProvider.Font;
                if (font != null)
                {
                    Font = font;
                }
            }

            topToolStrip.Font = Font;
	    }

        public void HTMLcmbFontSizeSelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                var selection = htmlEditControl.CurrentSelectionText;

                if (selection != null)
                    selection.execCommand(@"fontsize", false, HTMLcmbFontSize.SelectedItem);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString(), "Error");
            }
        }
        public void HTMLcmbFontSelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                var selection = htmlEditControl.CurrentSelectionText;

                if (selection != null)
                    selection.execCommand(@"fontName", false, HTMLcmbFont.SelectedText);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString(), "Error");
            }
        }

	    private void updateButtons()
		{
			boldToolStripMenuItem.Enabled = htmlEditControl.CanBold;
			italicToolStripMenuItem.Enabled = htmlEditControl.CanItalic;
			underlineToolStripButton.Enabled = htmlEditControl.CanUnderline;
			bullettedListToolStripMenuItem.Enabled = htmlEditControl.CanBullettedList;
			numberedListToolStripMenuItem.Enabled = htmlEditControl.CanOrderedList;
			indentToolStripMenuItem.Enabled = htmlEditControl.CanIndent;
			outdentToolStripMenuItem.Enabled = htmlEditControl.CanOutdent;
			insertTableToolStripMenuItem.Enabled = htmlEditControl.CanInsertTable;
			foreColorToolStripMenuItem.Enabled = htmlEditControl.CanForeColor;
			backColorToolStripMenuItem.Enabled = htmlEditControl.CanBackColor;
			undoToolStripButton.Enabled = htmlEditControl.CanUndo;
			justifyLeftToolStripButton.Enabled = htmlEditControl.CanJustifyLeft;
			justifyCenterToolStripButton.Enabled = htmlEditControl.CanJustifyCenter;
			justifyRightToolStripButton.Enabled = htmlEditControl.CanJustifyRight;

			// --

			boldToolStripMenuItem.Checked = htmlEditControl.IsBold;
			italicToolStripMenuItem.Checked = htmlEditControl.IsItalic;
			underlineToolStripButton.Checked = htmlEditControl.IsUnderline;
			numberedListToolStripMenuItem.Checked = htmlEditControl.IsBullettedList;
			bullettedListToolStripMenuItem.Checked = htmlEditControl.IsOrderedList;
			justifyLeftToolStripButton.Checked = htmlEditControl.IsJustifyLeft;
			justifyCenterToolStripButton.Checked = htmlEditControl.IsJustifyCenter;
			justifyRightToolStripButton.Checked = htmlEditControl.IsJustifyRight;

			textModulesToolStripItem.Visible = htmlEditControl.HasTextModules;
	
            // --

            var selection = htmlEditControl.CurrentSelectionText;

            if (selection != null)
            {
                var name = selection.queryCommandValue("FontName").ToString();
                int x = HTMLcmbFont.FindStringExact(name);

                if (x != -1)
                    HTMLcmbFont.SelectedIndex = x;

                var size = selection.queryCommandValue("FontSize").ToString();

                if (string.IsNullOrEmpty(size))
                    size = "2";

                x = HTMLcmbFontSize.FindStringExact(size);

                if (x != -1)
                    HTMLcmbFontSize.SelectedIndex = x;
            }
		}

		private void boldToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteBold();
		}

		private void italicToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteItalic();
		}

		private void bullettedListToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteBullettedList();
		}

		private void numberedListToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteNumberedList();
		}

		private void indentToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteIndent();
		}

		private void outdentToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteOutdent();
		}

		private void insertTableToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteInsertTable();
		}

		private void foreColorNoneToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColorNone();
		}

		private void foreColor01ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColor01();
		}

		private void foreColor02ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColor02();
		}

		private void foreColor03ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColor03();
		}

		private void foreColor04ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColor04();
		}

		private void foreColor05ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColor05();
		}

		private void foreColor06ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColor06();
		}

		private void foreColor07ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColor07();
		}

		private void foreColor08ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColor08();
		}

		private void foreColor09ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColor09();
		}

		private void foreColor10ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetForeColor10();
		}

		private void BackColorNoneToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetBackColorNone();
		}

		private void BackColor01ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetBackColor01();
		}

		private void BackColor02ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetBackColor02();
		}

		private void BackColor03ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetBackColor03();
		}

		private void BackColor04ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetBackColor04();
		}

		private void BackColor05ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteSetBackColor05();
		}

		private void HtmlEditUserControl_Load(object sender, EventArgs e)
		{
			updateButtons();
		}

		private void justifyLeftToolStripButton_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteJustifyLeft();
		}

		private void justifyCenterToolStripButton_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteJustifyCenter();
		}

		private void justifyRightToolStripButton_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteJustifyRight();
		}

		private void undoToolStripButton_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteUndo();
		}

		private void underlineToolStripButton_Click(object sender, EventArgs e)
		{
			htmlEditControl.ExecuteUnderline();
		}

		private void textModulesToolStripItem_DropDownOpening(object sender, EventArgs e)
		{
			if (!_textModulesFilled)
			{
				_textModulesFilled = true;
				htmlEditControl.FillTextModules(textModulesToolStripItem);
			}
		}
	}
}