using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Runtime.InteropServices;

//////////////////////////////////////////////////////////////////////

namespace Luminous.Windows.Forms
{

	public class PopupListBox : Popup
	{
		class TrackingListBox : ListBox
		{
			public TrackingListBox()
			{
				m_LastMouseMove = PointToClient(MousePosition);
			}

			Point m_LastMouseMove;

			protected override void OnMouseMove(MouseEventArgs e)
			{
				if (m_LastMouseMove != e.Location)
				{
					m_LastMouseMove = e.Location;

					int hit = IndexFromPoint(e.Location);

					if (hit != -1)
						SelectedIndex = hit;
				}
			}
		}

		// --------------------------------------------------------------

		public PopupListBox() : base(new TrackingListBox())
		{
			ListBox.MouseDown += new MouseEventHandler(OnListMouseDown);
			ListBox.KeyDown += new KeyEventHandler(OnListKeyDown);
		}

		public PopupListBox(object[] items) : this()
		{
			ListBox.Items.AddRange(items);
			ListBox.SelectedIndex = 0;
		}

		public ListBox ListBox
		{
			get { return base.Content as ListBox; }
		}

		protected void OnListMouseDown(object sender, MouseEventArgs e)
		{
			// We receive a bogus mouse-down immediately 
			// after showing so we check it first
			Point mousePos = MousePosition;
			mousePos = ListBox.PointToClient(mousePos);

			if (e.Location == mousePos)
			{
				int hit = ListBox.IndexFromPoint(e.Location);

				if (hit != -1)
				{
					Close(ToolStripDropDownCloseReason.ItemClicked);
				}
			}
		}

		protected void OnListKeyDown(object sender, KeyEventArgs e)
		{
			if ((e.KeyCode == Keys.Return) && (ListBox.SelectedIndex != -1))
			{
				Close(ToolStripDropDownCloseReason.ItemClicked);
			}
		}
	}

}
