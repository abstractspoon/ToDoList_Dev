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

        public ToolTip Tooltips;
        public int ToolTipInterval = 4000;
        public string ToolTipText;
        public bool ToolTipShowUp;

		public void RemapSysColors()
		{
			foreach (ToolStripItem item in Items)
			{
				if (item.Image != null)
				{
					Bitmap image = (item.Image as Bitmap);

					if (image != null)
					{
						for (int i = 0; i < image.Width; i++)
						{
							for (int j = 0; j < image.Height; j++)
							{
								Color color = image.GetPixel(i, j);

								if (MapColor(ref color))
									image.SetPixel(i, j, color);
							}
						}
					}
				}
			}
		}

		protected bool MapColor(ref Color color)
		{
			Color Black		= Color.FromArgb(0x00, 0x00, 0x00); // -> COLOR_BTNTEXT        
			Color DarkGray	= Color.FromArgb(0x80, 0x80, 0x80); // -> COLOR_BTNSHADOW      
			Color LightGray = Color.FromArgb(0xC0, 0xC0, 0xC0);	// -> COLOR_BTNFACE        
			Color White		= Color.FromArgb(0xFF, 0xFF, 0xFF);	// -> COLOR_BTNHIGHLIGHT   

			if (color == Black)
			{
				color = SystemColors.ControlText;
			}
			else if (color == DarkGray)
			{
				color = SystemColors.ControlDark;
			}
			else if (color == LightGray)
			{
				color = SystemColors.Control;
			}
			else if (color == White)
			{
				color = SystemColors.ControlLight;
			}
			else
			{
				return false;
			}

			return true;
		}

		protected override void OnMouseMove(MouseEventArgs mea)
        {
            base.OnMouseMove(mea);
            ToolStripItem newMouseOverItem = this.GetItemAt(mea.Location);
            if (mouseOverItem != newMouseOverItem ||
                (Math.Abs(mouseOverPoint.X - mea.X) > SystemInformation.MouseHoverSize.Width || (Math.Abs(mouseOverPoint.Y - mea.Y) > SystemInformation.MouseHoverSize.Height)))
            {
                mouseOverItem = newMouseOverItem;
                mouseOverPoint = mea.Location;
                Tooltips.Hide(this);
                timer.Stop();
                timer.Start();
            }
        }

        protected override void OnMouseClick(MouseEventArgs e)
        {
            base.OnMouseClick(e);
            ToolStripItem newMouseOverItem = this.GetItemAt(e.Location);
            if (newMouseOverItem != null)
            {
                Tooltips.Hide(this);
            }
        }

        protected override void OnMouseUp(MouseEventArgs mea)
        {
            base.OnMouseUp(mea);
            ToolStripItem newMouseOverItem = this.GetItemAt(mea.Location);
        }

        protected override void OnMouseLeave(EventArgs e)
        {
            base.OnMouseLeave(e);
            timer.Stop();
            Tooltips.Hide(this);
            mouseOverPoint = new Point(-50, -50);
            mouseOverItem = null;
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
                        Tooltips.Show(ToolTipText, this, currentMouseOverPoint, ToolTipInterval);
                    }
                }
                else if ((!(mouseOverItem is ToolStripDropDownButton) && !(mouseOverItem is ToolStripSplitButton)) ||
                    ((mouseOverItem is ToolStripDropDownButton) && !((ToolStripDropDownButton)mouseOverItem).DropDown.Visible) ||
                    (((mouseOverItem is ToolStripSplitButton) && !((ToolStripSplitButton)mouseOverItem).DropDown.Visible)))
                {
                    if (mouseOverItem.ToolTipText != null && mouseOverItem.ToolTipText.Length > 0)
                    {
                        Tooltips.Show(mouseOverItem.ToolTipText, this, currentMouseOverPoint, ToolTipInterval);
                    }
                }
            }
            catch
            { }
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
            if (disposing)
            {
                timer.Dispose();
                Tooltips.Dispose();
            }
        }

        public ToolStripEx()
            : base()
        {
            Tooltips = new ToolTip();
            ShowItemToolTips = false;

            ControlAdded += new ControlEventHandler(OnControlAdded);
            
            timer = new Timer();
            timer.Enabled = false;
            timer.Interval = SystemInformation.MouseHoverTime;
            timer.Tick += new EventHandler(timer_Tick);
        }

		public bool RefreshControlTooltip(ToolStripItem item)
		{
			if ((item == null) || !(item is ToolStripControlHost))
				return false;

			Tooltips.SetToolTip((item as ToolStripControlHost).Control, item.ToolTipText);
			return true;
		}

		protected void OnControlAdded(object sender, ControlEventArgs e)
        {
            var item = FindControlItem(e.Control);

            if (item != null)
                Tooltips.SetToolTip(e.Control, item.ToolTipText);
        }

        ToolStripItem FindControlItem(Control control)
        {
            foreach (ToolStripItem item in Items)
            {
                var controlHost = (item as ToolStripControlHost);

                if ((controlHost != null) && (controlHost.Control == control))
                    return item;
            }

            return null;
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

		public static int GetActualCursorHeight(Cursor cursor)
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
