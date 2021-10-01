using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Runtime.InteropServices;

//////////////////////////////////////////////////////////////////////

namespace CustomComboBox
{
	class TrackingListBox : ListBox
	{
		public TrackingListBox()
		{
			m_LastScreenMouseMove = MousePosition;
		}

		Point m_LastScreenMouseMove;

		protected override void OnMouseMove(MouseEventArgs e)
		{
			if (m_LastScreenMouseMove != PointToScreen(e.Location))
			{
				m_LastScreenMouseMove = PointToScreen(e.Location);

				int hit = IndexFromPoint(e.Location);

				if (hit != -1)
					SelectedIndex = hit;
			}
		}
	}

	// -----------------------------------------------------------------

	public class PopupListBox : PopupControl
	{
		// --------------------------------------------------------------

		private TrackingListBox m_ListBox;

		// --------------------------------------------------------------

		public PopupListBox()
		{
			m_ListBox = new TrackingListBox();
			m_ListBox.BorderStyle = BorderStyle.None;
			m_ListBox.Sorted = true;

			m_ListBox.MouseDown += new MouseEventHandler(OnListMouseDown);
			m_ListBox.KeyDown += new KeyEventHandler(OnListKeyDown);
		}

		public ListBox ListBox
		{
			get { return m_ListBox; }
		}

		public void Show(int x, int y)
		{
			Show(x, y, -1, -1, PopupResizeMode.None);
		}

		public void Show(int x, int y, PopupResizeMode resizeMode)
		{
			Show(x, y, -1, -1, resizeMode);
		}

		public void Show(int x, int y, int width, int height, PopupResizeMode resizeMode)
		{
			if (m_ListBox.Items.Count > 0)
			{
				if (height == -1)
					m_ListBox.Height = Math.Min(200, (m_ListBox.Items.Count * m_ListBox.GetItemRectangle(0).Height));

				m_ListBox.SelectedIndex = 0;
			}

			base.Show(m_ListBox, x, y, width, height, resizeMode);
		}

		protected void OnListMouseDown(object sender, MouseEventArgs e)
		{
			Close(ToolStripDropDownCloseReason.ItemClicked);
		}

		protected void OnListKeyDown(object sender, KeyEventArgs e)
		{
			if ((e.KeyCode == Keys.Return) && (m_ListBox.SelectedIndex != -1))
				Close(ToolStripDropDownCloseReason.ItemClicked);
		}

	}

}
