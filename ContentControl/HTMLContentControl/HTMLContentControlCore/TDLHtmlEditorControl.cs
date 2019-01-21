using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLContentControl
{
    [System.ComponentModel.DesignerCategory("")]
    public class TDLHtmlEditorControl : MSDN.Html.Editor.HtmlEditorControl
    {
        private UIThemeToolbarRenderer m_toolbarRenderer;
        private System.Drawing.Font m_ControlsFont;
        private Translator m_Trans;

        // ---------------------------------------------------------------

        public TDLHtmlEditorControl(System.Drawing.Font font, Translator trans)
        {
            m_toolbarRenderer = new UIThemeToolbarRenderer();
            m_ControlsFont = font;
            m_Trans = trans;

            InitializeComponent();

            //HtmlEditControl.TextChanged += new System.EventHandler(OnInputTextChanged);
            //HtmlEditControl.LostFocus += new System.EventHandler(OnInputTextLostFocus);
        }

        public void SetUITheme(UITheme theme)
        {
            m_toolbarRenderer.SetUITheme(theme);

            BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
            ToolBar.BackColor = BackColor;
        }

        private void InitializeComponent()
        {
            this.ToolBar.Renderer = m_toolbarRenderer;
            this.ToolBar.Font = m_ControlsFont;

            if (Win32.WantScaleByDPIFactor())
            {
                int imageSize = Win32.ScaleByDPIFactor(16);

                this.ToolBar.ImageScalingSize = new System.Drawing.Size(imageSize, imageSize);
                this.ToolBar.AutoSize = false;
                //this.ToolBar.Height = (imageSize + 10);
            }

            this.ToolbarDock = DockStyle.Top;
            this.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            this.InnerText = "Carl Nolan";
            this.TabIndex = 26;
            this.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.BorderSize = 0;
        }



    }

    

}
