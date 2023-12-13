using System;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Windows.Forms.VisualStyles;

namespace TreeViewHelper
{
    public class TreeViewUtils
    {
		[DllImport("User32.dll")]
		static extern IntPtr GetDesktopWindow();

		// --------------------------

		static public void DrawExpansionButton(Graphics graphics, Rectangle btnRect, bool opened, bool pressed)
		{
			if (!btnRect.IsEmpty)
			{
				if (VisualStyleRenderer.IsSupported)
				{
					var renderer = new VisualStyleRenderer(opened ?
															VisualStyleElement.TreeView.Glyph.Opened : 
															VisualStyleElement.TreeView.Glyph.Closed);

					renderer.DrawBackground(graphics, btnRect);
				}
				else
				{
					graphics.FillRectangle((pressed ? SystemBrushes.Control : SystemBrushes.Window), btnRect);
					graphics.DrawRectangle(SystemPens.ControlDark, btnRect);

					using (var pen = SystemPens.WindowText)
					{
						int midY = ((btnRect.Top + btnRect.Bottom) / 2);
						graphics.DrawLine(pen, btnRect.Left + 2, midY, btnRect.Right - 2, midY);

						if (!opened)
						{
							int midX = ((btnRect.Left + btnRect.Right) / 2);
							graphics.DrawLine(pen, midX, btnRect.Top + 2, midX, btnRect.Bottom - 2);
						}
					}
				}
			}
		}

		static int s_ExpansionButtonSize = 0;

		static public int ExpansionButtonSize
		{
			get
			{
				if (s_ExpansionButtonSize == 0)
				{
					var desktop = GetDesktopWindow();

					if (VisualStyleRenderer.IsSupported)
					{
						var renderer = new VisualStyleRenderer(VisualStyleElement.TreeView.Glyph.Opened);

						using (var graphics = Graphics.FromHwnd(desktop))
						{
							s_ExpansionButtonSize = renderer.GetPartSize(graphics, ThemeSizeType.Draw).Width;
						}
					}
					else
					{
						using (var graphics = Graphics.FromHwnd(desktop))
						{
							s_ExpansionButtonSize = (int)((9 * graphics.DpiX) / 96);
						}
					}
				}

				return s_ExpansionButtonSize;
			}
		}
	}
}
