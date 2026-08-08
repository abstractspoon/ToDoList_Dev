using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Diagnostics;

/////////////////////////////////////////////////////////////////////////////////////

namespace MindMapUIExtension
{
	public class MindMapDragEventItem
	{
		public MindMapDragEventItem(TreeNode n)
		{
			if (n == null)
				return;

			var item = (n.Tag as MindMapItem);

			if (item == null)
				return;

			node = n;
			uniqueID = Convert.ToUInt32(node.Name);
			itemData = item.ItemData;
		}

		public TreeNode node = null;
		public UInt32 uniqueID = 0;
		public Object itemData = null;
	}

	public class MindMapDragEventArgs : EventArgs
	{
		public MindMapDragEventArgs(TreeNode node, TreeNode parent, TreeNode sibling, bool copy)
		{
			dragged = new MindMapDragEventItem(node);
			targetParent = new MindMapDragEventItem(parent);
			afterSibling = new MindMapDragEventItem(sibling);
			copyItem = copy;
		}

		public MindMapDragEventItem dragged = null;
		public MindMapDragEventItem targetParent = null;
		public MindMapDragEventItem afterSibling = null;

		public bool copyItem = false;
	}

	//////////////////////////////////////////////////////////////////////////

	public class TreeViewEx : TreeView
	{
		[StructLayout(LayoutKind.Sequential)]
		public struct NMHDR
		{
			public IntPtr hwndFrom;
			public IntPtr idFrom;
			public int code;
		}


		// Win32 Imports -----------------------------------------------------------------

		[DllImport("User32.dll")]
		static extern int GetScrollPos(IntPtr hWnd, int nScrollBar);

		static int SB_HORZ = 0;
		static int SB_VERT = 1;

		// --------------------------

		[DllImport("User32.dll")]
		static extern int SendMessage(IntPtr hWnd, int msg, int wParam = 0, int lParam = 0);

		const int WM_SETREDRAW = 0x000B;
		const int WM_NOTIFY = 0x004E;
		const int WM_REFLECT = (0x0400 + 0x1C00);

		const int NM_CUSTOMDRAW = (0x00 - 12);

		const int TVM_SETITEMHEIGHT = (0x1100 + 27);
		const int TVM_GETITEMHEIGHT = (0x1100 + 28);

		// --------------------------

		private bool m_HoldRedraw = false;

		public bool HoldRedraw
		{
			get { return m_HoldRedraw; }

			set
			{
				if (value != m_HoldRedraw)
				{
					m_HoldRedraw = value;
					SendMessage(Handle, WM_SETREDRAW, (m_HoldRedraw ? 0 : 1), 0);
				}
			}
		}

		public int HorzScrollPos { get { return GetScrollPos(Handle, SB_HORZ); } }
		public int VertScrollPos { get { return GetScrollPos(Handle, SB_VERT); } }

		public new int ItemHeight
		{
			get { return SendMessage(Handle, TVM_GETITEMHEIGHT); }
			set { SendMessage(Handle, TVM_SETITEMHEIGHT, value); }
		}

		protected override void WndProc(ref Message m)
		{
			switch (m.Msg)
			{
			case (WM_NOTIFY + WM_REFLECT):
				{
					var nmhdr = (NMHDR)m.GetLParam(typeof(NMHDR));

					if ((nmhdr.code == NM_CUSTOMDRAW) && m_HoldRedraw)
						return;
				}
				break;
			}

			base.WndProc(ref m);
		}
	}

	//////////////////////////////////////////////////////////////////////////

}
