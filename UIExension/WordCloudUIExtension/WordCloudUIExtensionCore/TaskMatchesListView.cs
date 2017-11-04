using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class TaskMatchesListView : ListView
    {

		private UIExtension.SelectionRect m_SelectionRect;
		private UIExtension.TaskIcon m_TaskIcons;
		private bool m_TaskMatchesHaveIcons;

		public TaskMatchesListView(IntPtr hwndParent)
		{
			m_SelectionRect = new UIExtension.SelectionRect();
			m_TaskIcons = new UIExtension.TaskIcon(hwndParent);
		}

		public bool Initialise(Translator trans)
        {
			if (this.Columns.Count == 0) // once only
			{
				this.Columns.Add(trans.Translate("Task Matches"));
				this.Columns.Add(trans.Translate("ID"));

				Columns[1].AutoResize(ColumnHeaderAutoResizeStyle.HeaderSize);

				this.View = View.Details;
				this.MultiSelect = false;
				this.FullRowSelect = true;
				this.HideSelection = false;
				this.OwnerDraw = true;
				this.Sorting = SortOrder.Ascending;
				this.HeaderStyle = ColumnHeaderStyle.Clickable;
			}

            return true;
        }

		public bool AddMatch(CloudTaskItem item)
		{
			var lvItem = new ListViewItem(item.Title);

			lvItem.Tag = item;
			lvItem.Selected = false;
			lvItem.SubItems.Add(item.Id.ToString());

			if (item.IsParent || item.HasIcon)
			{
				lvItem.ImageIndex = 1;
				m_TaskMatchesHaveIcons = true;
			}

			if (this.Items.Add(lvItem) == null)
				return false;

			this.Columns[0].Width = -1; // widest item text

			return true;
		}

		public void ClearMatches()
		{
			Items.Clear();
			SelectedItems.Clear();

			m_TaskMatchesHaveIcons = false;
		}

        public UInt32 SetSelectedMatchId(UInt32 matchId)
        {
			SelectedItems.Clear();
			SelectedIndices.Clear();

			if (Items.Count == 0)
				return 0;

			ListViewItem selLVItem = FindItem(matchId);

			if (selLVItem == null)
				selLVItem = Items[0];

			selLVItem.Selected = true;

			return (selLVItem.Tag as CloudTaskItem).Id;
		}

		public UInt32 GetSelectedMatchId()
		{
			if (SelectedItems.Count == 0)
				return 0;

			return (SelectedItems[0].Tag as CloudTaskItem).Id;
		}

		protected ListViewItem FindItem(UInt32 matchId)
		{
			foreach (ListViewItem lvItem in Items)
			{
				var item = (lvItem.Tag as CloudTaskItem);

				if ((item != null) && (item.Id == matchId))
					return lvItem;
			}

			// else
            return null;
        }

		protected override void WndProc(ref Message m)
		{
			// Suppress mouse clicks OUTSIDE of the items area
			const int WM_LBUTTONDOWN   = 0x0201;
			const int WM_LBUTTONUP     = 0x0202;
			const int WM_LBUTTONDBLCLK = 0x0203;

			switch (m.Msg)
			{
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_LBUTTONDBLCLK:
				{
					Point pos = new Point(m.LParam.ToInt32() & 0xffff, m.LParam.ToInt32() >> 16);

					switch (this.HitTest(pos).Location)
					{
						case ListViewHitTestLocations.AboveClientArea:
						case ListViewHitTestLocations.BelowClientArea:
						case ListViewHitTestLocations.LeftOfClientArea:
						case ListViewHitTestLocations.RightOfClientArea:
						case ListViewHitTestLocations.None:
							return; // eat
					}
				}
				break;
			}

			// else default handling
			base.WndProc(ref m);
		}

		protected override void OnDrawItem(DrawListViewItemEventArgs e)
		{
			if (e.Item == null)
				return;

			// Draw the background only
			if (Items[e.ItemIndex].Selected)
				m_SelectionRect.Draw(Handle, e.Graphics, e.Bounds.X, e.Bounds.Y, e.Bounds.Width, e.Bounds.Height);
			else
				e.DrawBackground();

			// Draw the icon in the first column
			if (m_TaskMatchesHaveIcons)
			{
				var item = (e.Item.Tag as CloudTaskItem);

				if ((e.Item.ImageIndex != -1) && m_TaskIcons.Get(item.Id))
					m_TaskIcons.Draw(e.Graphics, e.Bounds.Left, e.Bounds.Top);
			}
		}

		protected override void OnDrawSubItem(DrawListViewSubItemEventArgs e)
		{
			Rectangle textRect = new Rectangle(e.Bounds.Location, e.Bounds.Size);

			// Adjust for icon in the first column
			if ((e.ColumnIndex == 0) && m_TaskMatchesHaveIcons)
			{
				textRect.X += 18;
				textRect.Width -= 18;
			}

			// Draw the text
			StringFormat stringFormat = new StringFormat();
			stringFormat.Alignment = StringAlignment.Near;
			stringFormat.LineAlignment = StringAlignment.Center;
			stringFormat.FormatFlags = StringFormatFlags.NoWrap;

			// Adjust for icon
			e.Graphics.DrawString(e.SubItem.Text,
								  this.Font,
								  SystemBrushes.WindowText,
								  textRect,
								  stringFormat);

		}

		protected override void OnDrawColumnHeader(DrawListViewColumnHeaderEventArgs e)
		{
			e.DrawBackground();

			StringFormat stringFormat = new StringFormat();
			stringFormat.Alignment = StringAlignment.Near;
			stringFormat.LineAlignment = StringAlignment.Center;

			e.Graphics.DrawString(Columns[e.ColumnIndex].Text,
									this.Font,
									SystemBrushes.WindowText,
									e.Bounds,
									stringFormat);
		}

		protected override void OnColumnClick(ColumnClickEventArgs e)
		{
			// Flip sort direction
			this.Sorting = (this.Sorting == SortOrder.Ascending ? SortOrder.Descending : SortOrder.Ascending);
			Sort();
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);
		}


	}
}

