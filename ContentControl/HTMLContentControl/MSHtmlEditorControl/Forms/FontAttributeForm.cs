#region Using directives

using System;
using System.Drawing;
using System.Drawing.Text;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

#endregion

namespace MSDN.Html.Editor
{

    /// <summary>
    /// Form used to enter an Html Font attribute
    /// Input based on the HtmlFontAttribute struct
    /// </summary>
    internal partial class FontAttributeForm : Form
    {

        // variable for passing back and forth the font attributes
        HtmlFontProperty _font;

        /// <summary>
        /// Public form constructor
        /// </summary>
        public FontAttributeForm()
        {
            //
            // Required for Windows Form Designer support
            //
            InitializeComponent();

            // populate the list of available fonts for selection
            LoadFonts();

        } //FontAttributeForm
        
        
        /// <summary>
        /// Property to define the Font attribute for the text
        /// Uses the HtmlFontProperty class
        /// </summary>
        public HtmlFontProperty HtmlFont
        {
            get
            {
                // define the font attributes
                string fontName = this.listFontName.Text;
                HtmlFontSize fontSize = (HtmlFontSize)this.listFontSize.SelectedIndex;
                bool fontBold = this.checkBold.Checked;
                bool fontUnderline = this.checkUnderline.Checked;
                bool fontItalic = this.checkItalic.Checked;
                bool fontStrikeout = this.checkStrikeout.Checked;
                bool fontSuperscript = this.checkSuperscript.Checked;
                bool fontSubscript = this.checkSubscript.Checked;
                _font = new HtmlFontProperty(fontName, fontSize, fontBold, fontItalic, fontUnderline, fontStrikeout, fontSubscript, fontSuperscript);
                return _font;
            }
            set
            {
                _font = value;
                // define font name
                int selection = this.listFontName.FindString(_font.Name);
                this.listFontName.SelectedIndex = selection;
                // define font size
                this.listFontSize.SelectedIndex = (int)_font.Size;
                // define font properties
                this.checkBold.Checked = _font.Bold;
                this.checkUnderline.Checked = _font.Underline;
                this.checkItalic.Checked = _font.Italic;
                this.checkStrikeout.Checked = _font.Strikeout;
                this.checkSubscript.Checked = _font.Subscript;
                this.checkSuperscript.Checked = _font.Superscript;
                // set the sample text font
                SetFontTextSample();
            }

        } //HtmlFont


        /// <summary>
        /// Loads into the list of font names a series of font objects
        /// These represent the available fonts
        /// </summary>
        private void LoadFonts()
        {
            // suspend drawing
            this.listFontName.SuspendLayout();

            // load the installed fonts and iterate through the collections
            InstalledFontCollection fonts = new InstalledFontCollection();
            foreach (FontFamily family in fonts.Families) // FontFamily.Families
            {
                // ensure font supports regular, bolding, underlining, and italics
                if (family.IsStyleAvailable(FontStyle.Regular & FontStyle.Bold & FontStyle.Italic & FontStyle.Underline)) 
                {
                    this.listFontName.Items.Add(family.Name);
                }
            }

            // define the selected item and resume drawing
            this.listFontName.SelectedIndex = 0;
            this.listFontName.ResumeLayout();

        } //LoadFonts


        /// <summary>
        /// Event handler for all functions that affect font sample
        /// Font name list and checkboxes for bold, itaic, underline
        /// </summary>
        private void FontSelectionChanged(object sender, System.EventArgs e)
        {
            SetFontTextSample();

        } //FontSelectionChanged


        /// <summary>
        /// Sets the sample font text based on the user selection
        /// </summary>
        private void SetFontTextSample()
        {
            string fontName = ((string)this.listFontName.SelectedItem);
            float fontSize = this.Font.Size + 2;
            bool fontBold = this.checkBold.Checked;
            bool fontUnderline = this.checkUnderline.Checked;
            bool fontItalic = this.checkItalic.Checked;
            bool fontStrikeout = this.checkStrikeout.Checked;
            FontStyle fontStyle = (fontBold ? FontStyle.Bold : FontStyle.Regular) | (fontItalic ? FontStyle.Italic : FontStyle.Regular) | (fontUnderline ? FontStyle.Underline : FontStyle.Regular) | (fontStrikeout ? FontStyle.Strikeout : FontStyle.Regular);
            // create the font object and define the labels fonts
            Font font = new Font(fontName, fontSize, fontStyle);
            this.labelSample.Font = font;

        } //SetFontTextSample

    }
}
