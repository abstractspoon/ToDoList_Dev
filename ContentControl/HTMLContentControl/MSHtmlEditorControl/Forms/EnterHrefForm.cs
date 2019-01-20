#region Using directives

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

using NavigateActionOption = MSDN.Html.Editor.NavigateActionOption;

#endregion

namespace MSDN.Html.Editor
{

    /// <summary>
    /// Form used to enter an Html Anchor attribute
    /// Consists of Href, Text and Target Frame
    /// </summary>
    public partial class EnterHrefForm : Form
    {

        /// <summary>
        /// Public form constructor
        /// </summary>
        public EnterHrefForm()
        {
            //
            // Required for Windows Form Designer support
            //
            InitializeComponent();

            // define the text for the targets
            this.listTargets.Items.AddRange(Enum.GetNames(typeof(NavigateActionOption)));

            // ensure default value set for target
            this.listTargets.SelectedIndex = 0;

        } //EnterHrefForm


        /// <summary>
        /// Property for the text to display
        /// </summary>
        public string HrefText
        {
            get
            {
                return this.hrefText.Text;
            }
            set
            {
                this.hrefText.Text = value;
            }

        } //HrefText

        /// <summary>
        /// Property for the href target
        /// </summary>
        public NavigateActionOption HrefTarget
        {
            get
            {
                return (NavigateActionOption)this.listTargets.SelectedIndex;
            }
        }

        /// <summary>
        /// Property for the href for the text
        /// </summary>
        public string HrefLink
        {
            get
            {
                return this.hrefLink.Text.Trim();
            }
            set
            {
                this.hrefLink.Text = value.Trim();
            }

        } //HrefLink

    }
}
