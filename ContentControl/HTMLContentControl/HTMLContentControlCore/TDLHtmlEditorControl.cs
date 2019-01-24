using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

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
        private Boolean m_SettingContent = false;

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
                var p = m_PrevTextChange ?? string.Empty;

                if ((p.Length != s.Length) || (p != s))
                {
                    m_PrevTextChange = s;

                    if (!m_SettingContent && !IsLoading && !IsNavigating && (TextChanged != null))
                    {
                        TextChanged(this, new EventArgs());
                    }
                }
            }
        }

        public Byte[] GetContent()
        {
            var html = InnerHtml ?? String.Empty;

            return System.Text.Encoding.Unicode.GetBytes(html);
        }

        public bool SetContent(Byte[] content, bool bResetSelection)
        {
            m_SettingContent = true;

            try
            {
                var html = System.Text.Encoding.Unicode.GetString(content).TrimEnd('\0').Trim();

                InnerHtml = html;
                m_PrevTextChange = InnerHtml;
            }
            // catch (Exception exception)
            // {
            // }
            finally
            {
                m_SettingContent = false;
            }

            return true;
        }

        // text content if supported. return false if not supported
        public String GetTextContent()
        {
            return InnerText ?? String.Empty;
        }

        public bool SetTextContent(String content, bool bResetSelection)
        {
            m_SettingContent = true;

            try
            {
                InnerText = content;
                m_PrevTextChange = InnerHtml;
            }
            // catch (Exception exception)
            // {
            // }
            finally
            {
                m_SettingContent = false;
            }

            return true;
        }

        protected override void DefineDialogProperties(Form dialog)
        {
            base.DefineDialogProperties(dialog);

            // Translate first as this may change size
            m_Trans.Translate(dialog);

            // Centre dialogs over our client area
            dialog.StartPosition = FormStartPosition.Manual;

            Rectangle parentPos = RectangleToScreen(Bounds);
            Point parentCentre = new Point((parentPos.Right + parentPos.Left) / 2, (parentPos.Bottom + parentPos.Top) / 2);

            int dialogLeft = (parentCentre.X - (dialog.Width / 2));
            int dialogTop = (parentCentre.Y - (dialog.Height / 2));

            dialog.Location = new Point(dialogLeft, dialogTop);

            // Add icon for identification
            dialog.ShowIcon = true;
            dialog.Icon = HTMLContentControlCore.html;
        }

    }

    

}
