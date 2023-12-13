using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace TreeViewHelper
{
    public class TreeViewUtils
    {
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

		static int ExpansionButtonSize = 0;

		static public int GetExpansionButtonSize(Control ctrl, int defaultSize = 0)
		{
			if (ExpansionButtonSize == 0)
			{
				if (VisualStyleRenderer.IsSupported)
				{
					var renderer = new VisualStyleRenderer(VisualStyleElement.TreeView.Glyph.Opened);

					using (var graphics = Graphics.FromHwnd(ctrl.Handle))
					{
						ExpansionButtonSize = renderer.GetPartSize(graphics, ThemeSizeType.Draw).Width;
					}
				}
				else if (defaultSize > 0)
				{
					ExpansionButtonSize = defaultSize;
				}
				else 
				{
					using (var graphics = Graphics.FromHwnd(ctrl.Handle))
					{
						ExpansionButtonSize = (int)((8 * graphics.DpiX) / 96);
					}
				}
			}

			return ExpansionButtonSize;
		}
	}
}
