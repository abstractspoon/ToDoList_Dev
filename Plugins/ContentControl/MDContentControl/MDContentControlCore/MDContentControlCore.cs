
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

            InputTextChanged += new System.EventHandler(OnInputTextChanged);
            InputLostFocus += new System.EventHandler(OnInputTextLostFocus);
		}

		// ITDLContentControl ------------------------------------------------------------------

		public Byte[] GetContent()
        {
            return System.Text.Encoding.Unicode.GetBytes(InputText);
        }

        public bool SetContent(Byte[] content, bool bResetSelection)
        {
			InputText = System.Text.Encoding.Unicode.GetString(content);
            return true;
        }

        // text content if supported. return false if not supported
        public String GetTextContent()
        {
			return OutputText;
		}

        public bool SetTextContent(String content, bool bResetSelection)
        {
			InputText = content;
            return true;
        }

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
			const int WM_KEYDOWN = 0x0100;
			const int WM_SYSKEYDOWN = 0x0104;

			switch (message)
			{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				return ProcessMenuShortcut(wParam);
			}

			return false;
        }

        public new bool Undo()
        {
			return base.Undo();
        }

        public new bool Redo()
        {
			return base.Redo();
		}

		public void SetUITheme(UITheme theme)
        {
			SplitBarColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark);
		}

        public void SetReadOnly(bool bReadOnly)
        {
			ReadOnly = bReadOnly;
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

			SetPreviewFont(fontName, pointSize);
			
			if (!appOnly)
			{
				SplitPos = prefs.GetProfileInt(key, "SplitPos", ClientSize.Height / 2);
			}
		}

		public void SetContentFont(String fontName, int pointSize)
		{
			SetInputFont(fontName, pointSize);
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
