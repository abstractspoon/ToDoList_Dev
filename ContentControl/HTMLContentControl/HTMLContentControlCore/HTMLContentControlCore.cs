
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD OTHER 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace HTMLContentControl
{
    [System.ComponentModel.DesignerCategory("")]
    public class HTMLContentControlCore : System.Windows.Forms.Panel, IContentControlWnd
    {
        private IntPtr m_hwndParent;
        private UIThemeToolbarRenderer m_toolbarRenderer;
        private System.Drawing.Font m_ControlsFont;
        private Boolean m_SettingContent = false;
        private MSDN.Html.Editor.HtmlEditorControl m_HtmlEditControl;

        // --------------------------------------------------------------------------------------

        private const string FontName = "Tahoma";

        // --------------------------------------------------------------------------------------

        public HTMLContentControlCore(IntPtr hwndParent)
        {
            m_hwndParent = hwndParent;
            m_toolbarRenderer = new UIThemeToolbarRenderer();
            m_ControlsFont = new System.Drawing.Font(FontName, 8);

            InitializeComponent();

            //HtmlEditControl.TextChanged += new System.EventHandler(OnInputTextChanged);
            //HtmlEditControl.LostFocus += new System.EventHandler(OnInputTextLostFocus);
        }

        // ITDLContentControl ------------------------------------------------------------------

        public Byte[] GetContent()
        {
            var html = "";// HtmlEditControl.DocumentText;

            return System.Text.Encoding.Unicode.GetBytes(html);
        }

        public bool SetContent(Byte[] content, bool bResetSelection)
        {
            m_SettingContent = true;

            try
            {
                var html = System.Text.Encoding.Unicode.GetString(content);
                //HtmlEditControl.DocumentText = html;
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
            return "";// HtmlEditControl.TextOnlyFromDocumentBody;
        }

        public bool SetTextContent(String content, bool bResetSelection)
        {
            m_SettingContent = true;

            try
            {
                //HtmlEditControl.SetDocumentText(content);
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

        public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
            // TODO
            return false;
        }

        public bool Undo()
        {
            return false;// HtmlEditControl.Undo();
        }

        public bool Redo()
        {
            return false;// HtmlEditControl.Redo();
        }

        public void SetUITheme(UITheme theme)
        {
            m_toolbarRenderer.SetUITheme(theme);

            BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
            //ToolBar.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
        }

        public void SetReadOnly(bool bReadOnly)
        {
            // TODO

        }

        public void SavePreferences(Preferences prefs, String key)
        {
            // TODO

        }

        public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
            if (!appOnly)
            {
                // private settings
                // TODO
            }

            String fontName = @"Tahoma";
            float fontSize = 8.25F;
            
            if (prefs.GetProfileInt("Preferences", "SpecifyCommentsFont", 0) != 0)
            {
                fontName = prefs.GetProfileString("Preferences", "CommentsFont", "Tahoma");
                fontSize = prefs.GetProfileInt("Preferences", "CommentsFontSize", 8);
            }
            else if ((prefs.GetProfileInt("Preferences", "SpecifyTreeFont", 0) != 0) &&
                    (prefs.GetProfileInt("Preferences", "CommentsUseTreeFont", 0) != 0))
            {
                fontName = prefs.GetProfileString("Preferences", "TreeFont", "Tahoma");
                fontSize = prefs.GetProfileInt("Preferences", "FontSize", 8);
            }

            //SetCssFontNameAndSize(fontName, fontSize);
        }

        // --------------------------------------------------------------------

        protected override void OnHandleCreated(EventArgs e)
        {
            base.OnHandleCreated(e);
 
            //HtmlEditControl.SetDocumentText(@"", @"C:\", true);
        }

        protected override void OnResize(System.EventArgs e)
        {
            base.OnResize(e);

//             if (this.m_HtmlEditControl != null)
//                 this.m_HtmlEditControl.SetBr = this.ClientSize;

            Win32.RemoveClientEdge(Handle);
        }

        protected override void OnPaintBackground(System.Windows.Forms.PaintEventArgs e)
        {
            // Do nothing
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();

            //this.ToolBar.Renderer = m_toolbarRenderer;
            //this.ToolBar.Font = m_ControlsFont;

            if (Win32.WantScaleByDPIFactor())
            {
                int imageSize = Win32.ScaleByDPIFactor(16);

                //this.ToolBar.ImageScalingSize = new System.Drawing.Size(imageSize, imageSize);
                //this.ToolBar.AutoSize = false;
                //this.ToolBar.Height = (imageSize + 10);
            }
            
            // 
            // HTMLContentControlCore
            // 
            this.ClientSize = new System.Drawing.Size(603, 716);
            this.Name = "HTMLContentControlCore";
            this.ResizeRedraw = true;
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Padding = new System.Windows.Forms.Padding(0);
            this.Font = m_ControlsFont;

            this.m_HtmlEditControl = new MSDN.Html.Editor.HtmlEditorControl();
            this.m_HtmlEditControl.ToolbarDock = DockStyle.Top;
            this.m_HtmlEditControl.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
//             this.m_HtmlEditControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
//                          | System.Windows.Forms.AnchorStyles.Left)
//                          | System.Windows.Forms.AnchorStyles.Right)));
            this.m_HtmlEditControl.InnerText = "Carl Nolan";
            this.m_HtmlEditControl.Location = new System.Drawing.Point(0, 0);
            this.m_HtmlEditControl.Name = "m_HtmlEditControl";
            this.m_HtmlEditControl.TabIndex = 26;
            this.m_HtmlEditControl.Size = this.ClientSize;
            this.m_HtmlEditControl.BorderStyle = System.Windows.Forms.BorderStyle.None;// FixedSingle;
            this.m_HtmlEditControl.BorderSize = 0;

            this.Controls.Add(this.m_HtmlEditControl);
            //this.m_HtmlEditControl.HtmlNavigation += new MSDN.Html.Editor.HtmlNavigationEventHandler(this.htmlEditorControl_HtmlNavigation); 

//             this.HtmlEditControl.AllowWebBrowserDrop = false;
//             this.HtmlEditControl.CssFontSize = "8pt";
//             this.HtmlEditControl.CssFontName = "Tahoma";
//             this.HtmlEditControl.IsWebBrowserContextMenuEnabled = false;

            this.ResumeLayout(false);
            this.PerformLayout();

            //this.OnResize(new EventArgs());
        }

        private void OnInputTextChanged(object sender, EventArgs e)
        {
            if (!m_SettingContent)
            {
                ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_hwndParent);

                notify.NotifyChange();
            }
        }

        private void OnInputTextLostFocus(object sender, EventArgs e)
        {
			ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_hwndParent);

            notify.NotifyKillFocus();
        }

    }
}
