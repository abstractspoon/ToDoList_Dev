using System;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms.VisualStyles;

using ImageHelper;

namespace TreeViewHelper
{
    public class TreeViewUtils
    {
		[DllImport("User32.dll")]
		static extern IntPtr GetDesktopWindow();

		// --------------------------

		static public void DrawExpansionButton(Graphics graphics, Rectangle btnRect, bool opened, bool pressed)
		{
			if (btnRect.IsEmpty)
				return;

			int btnSize = ExpansionButtonSize;

			if ((btnRect.Width == btnSize) && (btnRect.Height == btnSize))
			{
				// Expand as-is - no resizing
				if (VisualStyleRenderer.IsSupported)
				{
					var renderer = new VisualStyleRenderer(opened ?
															VisualStyleElement.TreeView.Glyph.Opened : 
															VisualStyleElement.TreeView.Glyph.Closed);

					renderer.DrawBackground(graphics, btnRect);
				}
				else
				{
                    // Drawing adds 1 to size
                    btnRect.Width--;
                    btnRect.Height--;

					graphics.FillRectangle((pressed ? SystemBrushes.Control : SystemBrushes.Window), btnRect);
					graphics.DrawRectangle(SystemPens.ControlDark, btnRect);

					int midY = ((btnRect.Top + btnRect.Bottom) / 2);
					graphics.DrawLine(SystemPens.WindowText, btnRect.Left + 2, midY, btnRect.Right - 2, midY);

					if (!opened)
					{
						int midX = ((btnRect.Left + btnRect.Right) / 2);
						graphics.DrawLine(SystemPens.WindowText, midX, btnRect.Top + 2, midX, btnRect.Bottom - 2);
					}
				}
			}
			else // resize to fit
			{
				// Draw into a temporary dc
				using (var tempImage = new Bitmap(btnSize, btnSize, PixelFormat.Format32bppRgb)) // unscaled size
				{
					tempImage.MakeTransparent();

					using (var gTemp = Graphics.FromImage(tempImage))
					{
						var tempRect = new Rectangle(0, 0, btnSize, btnSize);
						gTemp.Clear(SystemColors.Window);

						DrawExpansionButton(gTemp, tempRect, opened, pressed); // RECURSIVE CALL

						// Resize to output dc
						ImageUtils.DrawZoomedImage(tempImage, graphics, btnRect, btnRect);
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
