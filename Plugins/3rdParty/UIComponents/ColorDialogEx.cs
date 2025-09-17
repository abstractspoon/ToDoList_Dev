using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Drawing;


namespace UIComponents
{
	public class ColorDialogEx : ColorDialog
	{
		const int ID_BASICCOLORS	= 0x02D0;
		const int ID_CUSTOMCOLORS	= 0x02D1;

		const int NUM_COLS			= 8;
		const int NUM_BASICROWS		= 6;
		const int NUM_CUSTOMROWS	= 2;

		// -----------------------------------------------

		const int WM_LBUTTONDBLCLK	= 0x0203;
		const int WM_INITDIALOG		= 0x0110;
		const int WM_COMMAND		= 0x0111;
		
		
		const int BN_CLICKED		= 0;
		const int IDOK				= 1;

		// -----------------------------------------------

		[DllImport("USER32.DLL")]
		public static extern int SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

		[DllImport("USER32.DLL")]
		public static extern IntPtr GetDlgItem(IntPtr hWnd, int nCtrlId);

		[DllImport("user32.dll")]
		internal static extern bool GetClientRect(IntPtr hwnd, ref RECT lpRect);

		[StructLayout(LayoutKind.Sequential)]
		internal struct RECT
		{
			internal int left;
			internal int top;
			internal int right;
			internal int bottom;
		}

		[DllImport("user32.dll")]
		static extern bool ClientToScreen(IntPtr hwnd, ref POINT lpPoint);

		[DllImport("user32.dll")]
		static extern bool ScreenToClient(IntPtr hwnd, ref POINT lpPoint);

		[StructLayout(LayoutKind.Sequential)]
		internal struct POINT
		{
			internal int x;
			internal int y;
		}

		// -----------------------------------------------

		IntPtr m_DlgHandle = IntPtr.Zero;

		// -----------------------------------------------

		protected override IntPtr HookProc(IntPtr hWnd, int msg, IntPtr wparam, IntPtr lparam)
		{
			switch (msg)
			{
			case WM_INITDIALOG:
				m_DlgHandle = hWnd;
				break;

			case WM_LBUTTONDBLCLK:
				if (m_DlgHandle != IntPtr.Zero)
				{
					Point pos = new Point(lparam.ToInt32() & 0xffff, lparam.ToInt32() >> 16);

					if (!HandleColorGridDblClk(pos, ID_BASICCOLORS, NUM_BASICROWS))
						HandleColorGridDblClk(pos, ID_CUSTOMCOLORS, NUM_CUSTOMROWS);
				}
				break;
			}

			return base.HookProc(hWnd, msg, wparam, lparam);
		}

		IntPtr MAKEWPARAM(int low, int high)
		{
			return (IntPtr)((low & 0xFFFF) | (high << 16));
		}

		bool HandleColorGridDblClk(Point point, int nCtrlID, int nNumRows)
		{
			var gridHandle = GetDlgItem(m_DlgHandle, nCtrlID);

			if (gridHandle == null)
				return false;

 			var rect = new RECT();

			if (!GetClientRect(gridHandle, ref rect))
				return false;

			var pos = new POINT { x = point.X, y = point.Y };
 
 			ClientToScreen(m_DlgHandle, ref pos);
 			ScreenToClient(gridHandle, ref pos);

			var gridRect = Rectangle.FromLTRB(rect.left, rect.top, rect.right, rect.bottom);

			if (!gridRect.Contains(pos.x, pos.y))
 				return false;

 			int nCXCell = (gridRect.Width / NUM_COLS);
 			int nCYCell = (gridRect.Height / nNumRows);
 
 			int nXCell = (pos.x / nCXCell);
 			int nYCell = (pos.y / nCYCell);

 			var cellRect = new Rectangle(new Point(nXCell * nCXCell, nYCell * nCYCell), new Size(nCXCell, nCYCell));
 			cellRect.Inflate(-4, -3);
 
 			if (cellRect.Contains(pos.x, pos.y))
 				SendMessage(m_DlgHandle, WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED), gridHandle);

			return true;
		}

	}
}
