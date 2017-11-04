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
		private const int MinTaskMatchesWidth = 100;

        // -------------------------------------------------------------

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

				Columns[1].Width = -2; // Header width

				this.View = View.Details;
				this.MultiSelect = false;
				this.FullRowSelect = true;
				this.HideSelection = false;
				this.OwnerDraw = true;
				this.Sorting = SortOrder.Ascending;
				this.HeaderStyle = ColumnHeaderStyle.Clickable;
				this.DoubleBuffered = true;
				this.HotTracking = false;
			}

            return true;
        }

		protected override void OnColumnWidthChanging(ColumnWidthChangingEventArgs e)
		{
			e.Cancel = true;
			e.NewWidth = Columns[e.ColumnIndex].Width;
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

			// Draw the background
			if (e.Item.Selected)
				m_SelectionRect.Draw(Handle, e.Graphics, e.Bounds.X, e.Bounds.Y, e.Bounds.Width, e.Bounds.Height);
			else
				e.DrawBackground();

			// Draw subitems
			StringFormat stringFormat = new StringFormat();
			stringFormat.Alignment = StringAlignment.Near;
			stringFormat.LineAlignment = StringAlignment.Center;
			stringFormat.FormatFlags = StringFormatFlags.NoWrap;

			var item = (e.Item.Tag as CloudTaskItem);
			var brush = new SolidBrush(item.TextColor);
			Rectangle itemRect = new Rectangle(e.Bounds.Location, e.Bounds.Size);

			for (int colIndex = 0; colIndex < e.Item.SubItems.Count; colIndex++)
			{
				itemRect.X += 2;
				itemRect.Width = (Columns[colIndex].Width - 2);

				if ((colIndex == 0) && m_TaskMatchesHaveIcons)
				{
					if ((e.Item.ImageIndex != -1) && m_TaskIcons.Get(item.Id))
						m_TaskIcons.Draw(e.Graphics, itemRect.Left, itemRect.Top);

					itemRect.X += 18;
					itemRect.Width -= 18;
				}

				itemRect.Y++;
				itemRect.Height--;

				DrawText(e.Graphics, 
						e.Item.SubItems[colIndex].Text, 
						itemRect, 
						brush, 
						StringAlignment.Near,
						(colIndex == 0));

				// next subitem
				itemRect.X += itemRect.Width;
			}
		}

		protected override void OnDrawColumnHeader(DrawListViewColumnHeaderEventArgs e)
		{
			e.DrawBackground();

			DrawText(e.Graphics, 
					Columns[e.ColumnIndex].Text, 
					e.Bounds, 
					SystemBrushes.WindowText, 
					StringAlignment.Center, 
					false);
		}

		protected override void OnColumnClick(ColumnClickEventArgs e)
		{
			// Flip sort direction
			this.Sorting = (this.Sorting == SortOrder.Ascending ? SortOrder.Descending : SortOrder.Ascending);
			Sort();
		}

		protected void DrawText(Graphics graphics, String text, Rectangle rect, Brush brush, StringAlignment horzAlign, bool endEllipsis)
		{
			StringFormat format = new StringFormat();

			format.Alignment = horzAlign;
			format.LineAlignment = StringAlignment.Center;
			format.FormatFlags = StringFormatFlags.NoWrap;

			if (endEllipsis)
				format.Trimming = StringTrimming.EllipsisCharacter;

			graphics.DrawString(text, this.Font, brush, rect, format);
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			if (ClientRectangle.Width <= MinTaskMatchesWidth)
				return;
			
			// Resize first column to fill remaining width
			Columns[0].Width = (ClientRectangle.Width - Columns[1].Width - 2);
			Update();
		}
	}
}

