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
            InitialiseFeatures();

            this.ToolBar.Renderer = m_toolbarRenderer;
            this.ToolBar.Font = m_ControlsFont;

            if (DPIScaling.WantScaling())
            {
                int imageSize = DPIScaling.Scale(16);
                this.ToolBar.ImageScalingSize = new System.Drawing.Size(imageSize, imageSize);
            }

            this.ToolbarDock = DockStyle.Top;
            this.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            this.InnerText = "";
            this.TabIndex = 26;
            this.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.BorderSize = 0;
            this.NavigateAction = MSDN.Html.Editor.NavigateActionOption.NewWindow;

            m_Trans.Translate(ToolBar.Items);
            m_Trans.Translate(ContextMenu.Items);

            m_TextChangeTimer.Tick += OnTextChangeTimer;

            m_TextChangeTimer.Interval = 200;
            m_TextChangeTimer.Start();
        }

        void InitialiseFeatures()
        {
            // remove whole 'Document' submenu
            CommandHandling.HideCommand("contextDocument", ContextMenu.Items);
            CommandHandling.HideCommand("contextDocumentPrint", ContextMenu.Items);

            CommandHandling.HideCommand("toolstripDocumentPrint", ToolBar.Items);
// 
//             CommandHandling.SetMenuShortcut("contextFormatStrikeout", Keys.Control | Keys.Subtract, ContextMenu.Items);
//             CommandHandling.SetMenuShortcut("contextFormatIncrease", Keys.Control | Keys.Decimal, ContextMenu.Items);
//             CommandHandling.SetMenuShortcut("contextFormatDecrease", Keys.Control | Keys.Oemcomma, ContextMenu.Items);
            // TODO
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

        public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
            // Handle keyboard shortcuts
            if ((message == 0x0100) || (message == 0x0104) && (Control.ModifierKeys != Keys.None))
            {
                Keys keyPress = (Keys)wParam;

                if (keyPress == Keys.ControlKey)
                    return false;
                
                var modifiers = Control.ModifierKeys;

                if ((modifiers & Keys.Control) == Keys.Control)
                    keyPress |= Keys.Control;

                if ((modifiers & Keys.Shift) == Keys.Shift)
                    keyPress |= Keys.Shift;

                if ((modifiers & Keys.Alt) == Keys.Alt)
                    keyPress |= Keys.Alt;

                return CommandHandling.ProcessMenuShortcut(keyPress, ContextMenu.Items);
            }

            return false;
        }


    }

    

}
