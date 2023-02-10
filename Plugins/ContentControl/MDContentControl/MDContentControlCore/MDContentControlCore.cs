
using System;
using System.Runtime.InteropServices;

using Abstractspoon.Tdl.PluginHelpers;

namespace MDContentControl
{
    [System.ComponentModel.DesignerCategory("")]
    public class MDContentControlCore : MDContentControlForm, IContentControlWnd
    {
        private IntPtr m_hwndParent;

        public MDContentControlCore(IntPtr hwndParent)
        {
            m_hwndParent = hwndParent;

            InputText.TextChanged += new System.EventHandler(OnInputTextChanged);
            InputText.LostFocus += new System.EventHandler(OnInputTextLostFocus);

			Win32.SetEditMargins(InputText.Handle, DPIScaling.Scale(4));
			Win32.AddBorder(HtmlPreview.Handle);
		}

		// ITDLContentControl ------------------------------------------------------------------

		public Byte[] GetContent()
        {
            return System.Text.Encoding.Unicode.GetBytes(InputText.Text);
        }

        public bool SetContent(Byte[] content, bool bResetSelection)
        {
			InputText.Text = System.Text.Encoding.Unicode.GetString(content);
            return true;
        }

        // text content if supported. return false if not supported
        public String GetTextContent()
        {
			return HtmlPreview.Document.Body.InnerText ?? String.Empty;
		}

        public bool SetTextContent(String content, bool bResetSelection)
        {
			InputText.Text = content;
            return true;
        }

        public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
            // TODO
            return false;
        }

        public bool Undo()
        {
			if (!InputText.CanUndo)
				return false;

			// else 
			InputText.Undo();
            return true;
        }

        public bool Redo()
        {
			if (!InputText.CanRedo)
				return false;

			// else 
			InputText.Redo();
			return true;
		}

		public void SetUITheme(UITheme theme)
        {
			SetSplitBarColor(theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark));
		}

        public void SetReadOnly(bool bReadOnly)
        {
			InputText.ReadOnly = bReadOnly;
        }

        public void SavePreferences(Preferences prefs, String key)
        {
			prefs.WriteProfileInt(key, "SplitPos", SplitPos);
        }

        public void LoadPreferences(Preferences prefs, String key, bool appOnly)
        {
			var fontName = prefs.GetProfileString("Preferences", "HtmlFont", "Verdana");
			var htmlSize = prefs.GetProfileInt("Preferences", "HtmlFontSize", 2);
			int pointSize = MSDN.Html.Editor.HtmlFontConversion.PointsFromHtml(htmlSize);

			SetHtmlFont(fontName, pointSize);
			
			if (!appOnly)
			{
				SplitPos = prefs.GetProfileInt(key, "SplitPos", ClientSize.Height / 2);
			}
		}

		public void SetContentFont(String fontName, int pointSize)
		{
			InputText.Font = new System.Drawing.Font(fontName, pointSize);
		}

		// --------------------------------------------------------------------

		protected override void OnResize(System.EventArgs e)
        {
            base.OnResize(e);

            Win32.RemoveClientEdge(Handle);
		}

        private void OnInputTextChanged(object sender, EventArgs e)
        {
			ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_hwndParent);

            notify.NotifyChange();
        }

        private void OnInputTextLostFocus(object sender, EventArgs e)
        {
			ContentControlWnd.ParentNotify notify = new ContentControlWnd.ParentNotify(m_hwndParent);

            notify.NotifyKillFocus();
        }

    }
}
