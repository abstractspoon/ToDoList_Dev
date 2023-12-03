using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace TreeViewHelper
{
    public class Utils
    {
		static public void DrawExpansionButton(Graphics graphics, Rectangle button, bool opened, bool hot)
		{
			if (!button.IsEmpty)
			{
				if (VisualStyleRenderer.IsSupported)
				{
					var renderer = new VisualStyleRenderer(opened ?
															VisualStyleElement.TreeView.Glyph.Opened : 
															VisualStyleElement.TreeView.Glyph.Closed);

					renderer.DrawBackground(graphics, button);
				}
				else
				{
					graphics.FillRectangle((hot ? SystemBrushes.Window : SystemBrushes.Control), button);
					graphics.DrawRectangle(SystemPens.ControlDark, button);

					using (var pen = SystemPens.WindowText)
					{
						int midY = ((button.Top + button.Bottom) / 2);
						graphics.DrawLine(pen, button.Left + 2, midY, button.Right - 2, midY);

						if (!opened)
						{
							int midX = ((button.Left + button.Right) / 2);
							graphics.DrawLine(pen, midX, button.Top + 2, midX, button.Bottom - 2);
						}
					}
				}
			}
		}
	}
}
