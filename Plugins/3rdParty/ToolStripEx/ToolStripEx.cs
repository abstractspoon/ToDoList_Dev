using System;
using System.Drawing;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IIControls
{
    public class ToolStripEx : ToolStrip
    {
        ToolStripItem mouseOverItem = null;
        Point mouseOverPoint;
        Timer timer;
		bool isDPIScaled = false;

        public ToolTip Tooltip;
        public int ToolTipInterval = 4000;
        public string ToolTipText;
        public bool ToolTipShowUp;

		public ToolStripEx() : base()
		{
			// This class only needs to operate in DPI scaled mode
			// because Winforms calculates the tooltip position wrong.
			// Also it can't handle ToolStripComboBox because the 
			// mouse move notifications go to the combobox
			using (var graphics = this.CreateGraphics())
			{
				isDPIScaled = (graphics.DpiX != 96);

				if (isDPIScaled)
				{
					ShowItemToolTips = false;
					timer = new Timer();
					timer.Enabled = false;
					timer.Interval = SystemInformation.MouseHoverTime;
					timer.Tick += new EventHandler(timer_Tick);
					Tooltip = new ToolTip();
				}
			}
		}

		protected override void OnMouseMove(MouseEventArgs mea)
        {
            base.OnMouseMove(mea);

			if (isDPIScaled)
			{
				ToolStripItem newMouseOverItem = this.GetItemAt(mea.Location);

				if (mouseOverItem != newMouseOverItem ||
					(Math.Abs(mouseOverPoint.X - mea.X) > SystemInformation.MouseHoverSize.Width || (Math.Abs(mouseOverPoint.Y - mea.Y) > SystemInformation.MouseHoverSize.Height)))
				{
					mouseOverItem = newMouseOverItem;
					mouseOverPoint = mea.Location;
					if (Tooltip != null)
						Tooltip.Hide(this);
					timer.Stop();
					timer.Start();
				}
			}
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			base.OnMouseClick(e);

			if (isDPIScaled)
			{
				ToolStripItem newMouseOverItem = this.GetItemAt(e.Location);
				if (newMouseOverItem != null && Tooltip != null)
				{
					Tooltip.Hide(this);
				}
			}
		}

		protected override void OnMouseUp(MouseEventArgs mea)
        {
            base.OnMouseUp(mea);
            //ToolStripItem newMouseOverItem = this.GetItemAt(mea.Location);
        }

        protected override void OnMouseLeave(EventArgs e)
        {
            base.OnMouseLeave(e);

			if (isDPIScaled)
			{
				timer.Stop();
				if (Tooltip != null)
					Tooltip.Hide(this);
				mouseOverPoint = new Point(-50, -50);
				mouseOverItem = null;
			}
		}

		void timer_Tick(object sender, EventArgs e)
        {
            timer.Stop();
            try
            {
                Point currentMouseOverPoint;
				int cursorHeight = GetActualCursorHeight(Cursor.Current);

                if (ToolTipShowUp)
                    currentMouseOverPoint = this.PointToClient(new Point(Control.MousePosition.X, Control.MousePosition.Y - (cursorHeight * 2) + Cursor.Current.HotSpot.Y));
                else
                    currentMouseOverPoint = this.PointToClient(new Point(Control.MousePosition.X, Control.MousePosition.Y + cursorHeight - Cursor.Current.HotSpot.Y));

                if (mouseOverItem == null)
                {
                    if (ToolTipText != null && ToolTipText.Length > 0)
                    {
                        if (Tooltip == null)
                            Tooltip = new ToolTip();
                        Tooltip.Show(ToolTipText, this, currentMouseOverPoint, ToolTipInterval);
                    }
                }
                else if ((!(mouseOverItem is ToolStripDropDownButton) && !(mouseOverItem is ToolStripSplitButton)) ||
                    ((mouseOverItem is ToolStripDropDownButton) && !((ToolStripDropDownButton)mouseOverItem).DropDown.Visible) ||
                    (((mouseOverItem is ToolStripSplitButton) && !((ToolStripSplitButton)mouseOverItem).DropDown.Visible)))
                {
                    if (mouseOverItem.ToolTipText != null && mouseOverItem.ToolTipText.Length > 0 && Tooltip != null)
                    {
                        if (Tooltip == null)
                            Tooltip = new ToolTip();
                        Tooltip.Show(mouseOverItem.ToolTipText, this, currentMouseOverPoint, ToolTipInterval);
                    }
                }
            }
            catch
            { }
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            if (disposing && isDPIScaled)
            {
                timer.Dispose();
                Tooltip.Dispose();
            }
        }

		[StructLayout(LayoutKind.Sequential)]
		private struct ICONINFO
		{
			public bool fIcon;
			public int xHotspot;
			public int yHotspot;
			public IntPtr hbmMask;
			public IntPtr hbmColor;
		}

		[DllImport("user32")]
		private static extern bool GetIconInfo(IntPtr hIcon, out ICONINFO pIconInfo);

		[DllImport("gdi32.dll", SetLastError = true)]
		private static extern bool DeleteObject(IntPtr hObject);

		private int GetActualCursorHeight(Cursor cursor)
		{
			ICONINFO ii;

			if (GetIconInfo(cursor.Handle, out ii))
			{
				Bitmap bmp = Bitmap.FromHbitmap(ii.hbmMask);

				int bmpWidth = bmp.Width;
				int bmpHeight = ((ii.hbmColor == IntPtr.Zero) ? (bmp.Height / 2) : bmp.Height);

				DeleteObject(ii.hbmColor);
				DeleteObject(ii.hbmMask);

				// Find the first black pixel bottom-up
				for (int y = bmpHeight - 1; y >= 0; y--)
				{
					for (int x = bmpWidth - 1; x >= 0; x--)
					{
						Color color = bmp.GetPixel(x, y);

						if (color.Equals(Color.FromArgb(255, 0, 0, 0)))
						{
							return y;
						}
					}
				}
			}

			return cursor.Size.Height;
		}
    }
}
