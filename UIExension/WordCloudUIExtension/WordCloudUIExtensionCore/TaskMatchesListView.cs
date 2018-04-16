using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

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
        private ImageList m_ilItemHeight;

		public TaskMatchesListView(IntPtr hwndParent)
		{
			m_SelectionRect = new UIExtension.SelectionRect();
			m_TaskIcons = new UIExtension.TaskIcon(hwndParent);

            m_ilItemHeight = new ImageList();
            m_ilItemHeight.ImageSize = new Size(1, Win32.ScaleByDPIFactor(17)); // minimum height
		}

        private int TextIconOffset
        {
            get { return (ImageSize + 2); }
        }

        private int ImageSize
        {
            get { return Win32.ScaleByDPIFactor(16); }
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
                this.StateImageList = m_ilItemHeight;
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

		public bool HasMatchId(UInt32 matchId)
		{
			return (FindItem(matchId) != null);
		}

		public bool UpdateMatchItemsText(HashSet<UInt32> matchIds)
		{
			bool someUpdated = false;

			foreach (ListViewItem lvItem in Items)
			{
				var item = (lvItem.Tag as CloudTaskItem);

				if ((item != null) && (matchIds.Contains(item.Id)))
				{
					if (someUpdated || !lvItem.Text.Equals(item.Title))
					{
						lvItem.Text = item.Title;
						someUpdated = true;
					}
				}
			}

			return someUpdated;
		}

        public void EnsureSelectionVisible()
        {
            if ((SelectedIndices.Count == 0) || (Items.Count == 0))
                return;

            int itemIndex = SelectedItems.Count;

            while (itemIndex-- > 0)
            {
                Rectangle itemRect = SelectedItems[itemIndex].Bounds;

                if (Rectangle.Intersect(ClientRectangle, itemRect) == itemRect)
                    return;
            }

            // else
            EnsureVisible(SelectedIndices[0]);
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
            EnsureSelectionVisible();

			return (selLVItem.Tag as CloudTaskItem).Id;
		}

		public bool SelectMatch(String words, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			if (Items.Count == 0)
				return false;

			if (SelectedIndices.Count == 0)
				SelectedIndices.Add(0);

			int selIndex = SelectedIndices[0];
			int matchIndex = -1;

			switch (selectTask)
			{
				case UIExtension.SelectTask.SelectFirstTask:
					matchIndex = FindTask(words, 0, true, caseSensitive, wholeWord, findReplace);
					break;

				case UIExtension.SelectTask.SelectNextTask:
                    matchIndex = FindTask(words, (selIndex + 1), true, caseSensitive, wholeWord, findReplace);
					break;

				case UIExtension.SelectTask.SelectNextTaskInclCurrent:
                    matchIndex = FindTask(words, selIndex, true, caseSensitive, wholeWord, findReplace);
					break;

				case UIExtension.SelectTask.SelectPrevTask:
                    matchIndex = FindTask(words, (selIndex - 1), false, caseSensitive, wholeWord, findReplace);
					break;

				case UIExtension.SelectTask.SelectLastTask:
                    matchIndex = FindTask(words, (Items.Count - 1), false, caseSensitive, wholeWord, findReplace);
					break;
			}

			if (matchIndex != -1)
			{
				SelectedIndices.Clear();
				SelectedIndices.Add(matchIndex);

                EnsureSelectionVisible();
				return true;
			}

			return false;
		}

		private int FindTask(String words, int startIndex, bool forward, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			int fromIndex = startIndex;
			int toIndex = forward ? Items.Count : -1;
			int increment = forward ? 1 : -1;

			for (int i = fromIndex; i != toIndex; i += increment)
			{
				var item = (Items[i].Tag as CloudTaskItem);

                if (item.Matches(words, caseSensitive, wholeWord, findReplace))
					return i;
			}

			return -1; // no match
		}
		
		public UInt32 GetSelectedMatchId()
		{
			if (SelectedItems.Count == 0)
				return 0;

			return (SelectedItems[0].Tag as CloudTaskItem).Id;
		}

		public UInt32 GetNextSelectedMatchId()
		{
			if (Items.Count > 1)
			{
				var nextIndex = (SelectedItems[0].Index + 1);

				if (nextIndex < Items.Count)
					return (Items[nextIndex].Tag as CloudTaskItem).Id;
			}

			// all else 
			return 0;
		}

		public UInt32 GetPrevSelectedMatchId()
		{
			if (Items.Count > 1)
			{
				var prevIndex = (SelectedItems[0].Index - 1);

				if (prevIndex >= 0)
					return (Items[prevIndex].Tag as CloudTaskItem).Id;
			}

			// all else 
			return 0;
		}

		public CloudTaskItem GetSelectedMatch()
		{
			if (SelectedItems.Count == 0)
				return null;

			return (SelectedItems[0].Tag as CloudTaskItem);
		}

		public Rectangle GetSelectedMatchEditRect()
		{
			Rectangle editRect = new Rectangle(0, 0, 0, 0);

			if (SelectedItems.Count > 0)
			{
				editRect = SelectedItems[0].GetBounds(ItemBoundsPortion.Label);

				editRect.Width += (editRect.X - 2);
				editRect.X = 2;

				// adjust for icon
				if (m_TaskMatchesHaveIcons)
				{
					editRect.X += TextIconOffset;
					editRect.Width -= TextIconOffset;
				}
			}

			return editRect;
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
							Focus();
							return; // eat
					}
				}
				break;
			}

			// else default handling
			base.WndProc(ref m);
		}

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            var hit = HitTest(e.Location);

            if (hit.Item != null)
            {
                var item = (hit.Item.Tag as CloudTaskItem);

                if ((item != null) && item.IsLocked)
                {
                    var cursor = UIExtension.AppCursor.Load(UIExtension.AppCursor.CursorType.LockedTask);

                    if (cursor != null)
                    {
                        Cursor = cursor;
                        return;
                    }
                }
            }

            // all else
            Cursor = Cursors.Arrow;
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
			
			Brush brush;

			if (e.Item.Selected)
				brush = new SolidBrush(DrawingColor.AdjustLighting(item.TextColor, -0.3f, true));
			else
				brush = new SolidBrush(item.TextColor);

			Rectangle itemRect = new Rectangle(e.Bounds.Location, e.Bounds.Size);

			for (int colIndex = 0; colIndex < e.Item.SubItems.Count; colIndex++)
			{
				itemRect.X += 2;
				itemRect.Width = (Columns[colIndex].Width - 2);

				if ((colIndex == 0) && m_TaskMatchesHaveIcons)
				{
                    if ((e.Item.ImageIndex != -1) && m_TaskIcons.Get(item.Id))
                    {
                        int imageSize = ImageSize;
                        Rectangle iconRect = new Rectangle(itemRect.Location, new Size(imageSize, imageSize));
                        iconRect.Y += ((itemRect.Height - imageSize) / 2);

                        m_TaskIcons.Draw(e.Graphics, iconRect.Left, iconRect.Top);
                    }

					itemRect.X += TextIconOffset;
					itemRect.Width -= TextIconOffset;
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

