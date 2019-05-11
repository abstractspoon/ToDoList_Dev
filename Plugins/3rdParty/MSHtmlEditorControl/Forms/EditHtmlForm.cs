#region Using directives

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

#endregion

namespace MSDN.Html.Editor
{

    /// <summary>
    /// Form used to Edit or View Html contents
    /// If a property RedOnly is true contents are considered viewable
    /// No Html parsing is performed on the resultant data
    /// </summary>
    internal partial class EditHtmlForm : Form
    {

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

            // ensure content is empty
            this.htmlText.Text = string.Empty;
            this.ReadOnly = true;

        } //EditHtmlForm

        /// <summary>
        /// Property to modify the caption of the display
        /// </summary>
        public void SetCaption(string caption)
        {
            this.Text = caption;
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
