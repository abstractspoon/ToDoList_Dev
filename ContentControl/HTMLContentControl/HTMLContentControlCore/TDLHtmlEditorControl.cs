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
        private Timer m_TextChangeTimer;
        private String m_PrevTextChange = "";

        // ---------------------------------------------------------------

        public new event EventHandler TextChanged;

        // ---------------------------------------------------------------

        public TDLHtmlEditorControl(System.Drawing.Font font, Translator trans)
        {
            m_ControlsFont = font;
            m_Trans = trans;
            m_toolbarRenderer = new UIThemeToolbarRenderer();
            m_TextChangeTimer = new Timer();

            InitializeComponent();
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
            }

            this.ToolbarDock = DockStyle.Top;
            this.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            this.InnerText = "";
            this.TabIndex = 26;
            this.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.BorderSize = 0;

            m_TextChangeTimer.Tick += OnTextChangeTimer;

            m_TextChangeTimer.Interval = 200;
            m_TextChangeTimer.Start();
        }

        private void OnTextChangeTimer(object sender, EventArgs e)
        {
            if (!IsDisposed)
            {
                var s = InnerHtml ?? string.Empty;

                if ((m_PrevTextChange.Length != s.Length) || 
                    (m_PrevTextChange != s))
                {
                    m_PrevTextChange = s;

                    if (TextChanged != null)
                    {
                        TextChanged(this, new EventArgs());
                    }
                }
            }
        }

        new public string InnerHtml
        {
            get { return base.InnerHtml; }
            set
            {
                m_PrevTextChange = value;
                base.InnerHtml = value;
            }
        }

        new public string InnerText
        {
            get { return base.InnerText; }
            set
            {
                base.InnerText = value;
                m_PrevTextChange = InnerHtml;
            }
        }

    }

    

}
