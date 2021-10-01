using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace LinkLabelEx
{

	// This class replaces the old Internet Explorer hand cursor
	// with the nice 'new' Edge hand cursor
	public class LinkLabelEx : LinkLabel
	{
		const int IDC_HAND = 32649;
		const int WM_SETCURSOR = 0x0020;
		const int HTCLIENT = 1;

		// -----------------------------------------

		[DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		static extern IntPtr LoadCursor(IntPtr hInstance, int lpCursorName);

		[DllImport("user32.dll", ExactSpelling = true, CharSet = CharSet.Auto)]
		public static extern IntPtr SetCursor(HandleRef hcursor);

		// -----------------------------------------

		static readonly Cursor Hand = new Cursor(LoadCursor(IntPtr.Zero, IDC_HAND));

		// -----------------------------------------

		protected override void WndProc(ref Message msg)
		{
			if (msg.Msg == WM_SETCURSOR)
			{
				if ((msg.WParam == Handle) && (((int)msg.LParam & 0xffff) == HTCLIENT))
				{
					if (OverrideCursor == Cursors.Hand)
					{
						SetCursor(new HandleRef(Hand, Hand.Handle));
					}
					else if (OverrideCursor != null)
					{
						SetCursor(new HandleRef(OverrideCursor, OverrideCursor.Handle));
					}
					else
					{
						SetCursor(new HandleRef(Cursor, Cursor.Handle));
					}

					return; // handled
				}
			}

			base.WndProc(ref msg);
		}
	}
}
