using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	internal class NoTrackHeaderControl : NativeWindow
	{
		public NoTrackHeaderControl(ListView lv)
		{
			const uint LVM_GETHEADER = (0x1000 + 31);

			//Get the header control handle
			IntPtr header = new IntPtr(Win32.SendMessage(lv.Handle, LVM_GETHEADER, UIntPtr.Zero, IntPtr.Zero));
			this.AssignHandle(header);
		}

		protected override void WndProc(ref Message m)
		{
			const int WM_SETCURSOR = 0x0020;

			switch (m.Msg)
			{
			case WM_SETCURSOR:
				Win32.SetArrowCursor();
				return;
			}

			base.WndProc(ref m);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////

	public delegate Boolean EditTaskLabelEventHandler(object sender, UInt32 taskId);
	public delegate Boolean EditTaskIconEventHandler(object sender, UInt32 taskId);
	public delegate Boolean EditTaskCompletionEventHandler(object sender, UInt32 taskId, bool completed);

	[System.ComponentModel.DesignerCategory("")]
	class TaskMatchesListView : TaskListView//, ILabelTipHandler
	{
		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;

		// -------------------------------------------------------------

		const int DefaultMaxTaskId = 100;

		// -------------------------------------------------------------

		private uint m_MaxTaskId = DefaultMaxTaskId;
		private NoTrackHeaderControl m_Header;

		public TaskMatchesListView() : base()
		{
		}

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			m_Header = new NoTrackHeaderControl(this);
		}

		private int MinTaskMatchesWidth
		{
			get { return DPIScaling.Scale(100); }
		}

		private int LabelTipBorder
		{
			get { return DPIScaling.Scale(4); }
		}
		
		public new bool Initialize(Translator trans, UIExtension.TaskIcon taskIcons)
        {
			if (this.Columns.Count == 0) // once only
			{
				base.Initialize(trans, taskIcons);

				this.Columns.Add(Translate("Task Matches", Translator.Type.Header));
				this.Columns.Add(Translate("ID", Translator.Type.Header));

				Columns[1].Width = -2; // Header width
			}

            return true;
        }

		// The other part of NoTrackHeaderControl
		protected override void OnColumnWidthChanging(ColumnWidthChangingEventArgs e)
		{
			// Prevent column resizing to save us having to save/restore the widths
			e.Cancel = true;
			e.NewWidth = Columns[e.ColumnIndex].Width;
		}

		public new void EndUpdate()
		{
			base.EndUpdate();

			RefreshIDColumnWidth();
		}

		private void RefreshIDColumnWidth()
		{
			using (var graphics = CreateGraphics())
			{
				int headerPadding = (int)(graphics.MeasureString("o", Font).Width); // Fudge
				int headerWidth   = (int)(graphics.MeasureString(Columns[1].Text, Font).Width + (headerPadding * 2));
				int maxItemWidth  = (int)(graphics.MeasureString(m_MaxTaskId.ToString(), Font).Width + (2 * DPIScaling.Scale(2)));

				Columns[1].Width = Math.Max(headerWidth, maxItemWidth);
				Columns[0].Width = (ClientRectangle.Width - Columns[1].Width - 2);
			}
		}

		protected override void OnFontChanged(EventArgs e)
		{
			base.OnFontChanged(e);

			RefreshIDColumnWidth();
		}

		public bool AddMatch(CloudTaskItem item)
		{
			var lvItem = new ListViewItem(item.Title);

			lvItem.Tag = item;
			lvItem.Selected = false;
			lvItem.SubItems.Add(item.Id.ToString());
            lvItem.Checked = item.IsDone(false);

			if ((item.IsParent && ShowParentsAsFolders) || item.HasIcon)
			{
				lvItem.ImageIndex = 1;
				ItemsHaveIcons = true;
			}

			m_MaxTaskId = Math.Max(m_MaxTaskId, item.Id);

			if (this.Items.Add(lvItem) == null)
				return false;

			return true;
		}

		public void ClearMatches()
		{
			Items.Clear();
			SelectedItems.Clear();

			ItemsHaveIcons = false;
			m_MaxTaskId = DefaultMaxTaskId;
		}

		public bool HasMatchId(UInt32 matchId)
		{
			return (FindItem(matchId) != null);
		}

		public bool UpdateMatchItems(HashSet<UInt32> matchIds)
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

                    if ((item.IsParent && ShowParentsAsFolders) || item.HasIcon)
                    {
                        lvItem.ImageIndex = 1;
                        ItemsHaveIcons = true;
 
 						someUpdated = true;
                    }
                    else
                    {
                        lvItem.ImageIndex = -1;

                        if (Items.Count == 1)
                        {
                            ItemsHaveIcons = false;
                            someUpdated = true;
                        }
                    }
				}
			}

            Invalidate();

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
			if ((SelectedItems.Count > 0) && (Items.Count > 1))
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
            if ((SelectedItems.Count > 0) && (Items.Count > 1))
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
				editRect = CalcLabelTextRect(SelectedItems[0].GetBounds(ItemBoundsPortion.Label), false);

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
			const int LVM_GETTOPINDEX  = 0x1039;
			
			switch (m.Msg)
			{
			case LVM_GETTOPINDEX:
				{
					// There's a very strange bug where the first
					// mouseover of an item causes it to be redrawn
					// and it flickers regardless of whether we are
					// double-buffered or not. The workaround is to
					// not draw the item under these circumstances.
					// See also OnDrawItem()
					m_IgnoreNextItemDraw = true;
					base.WndProc(ref m);
					m_IgnoreNextItemDraw = false;
				}
				return;
			}

			// else default handling
			base.WndProc(ref m);
		}

		private bool m_IgnoreNextItemDraw = false;

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            var hit = HitTest(e.Location);

            if (hit.Item != null)
            {
                var item = (hit.Item.Tag as CloudTaskItem);

                if (item != null)
                {
                    Cursor cursor = null;

                    if (item.IsLocked)
                    {
                        cursor = UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);
                    }
                    else if (CalcIconRect(hit.Item.Bounds).Contains(e.Location))
                    {
                        cursor = UIExtension.HandCursor();
                    }

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

		protected override void OnMouseDown(MouseEventArgs e)
        {
			// disable label editing if not on the item text
			int leftMargin = (CheckboxOffset + TextIconOffset);
            int rightMargin = Columns[0].Width;

            this.LabelEdit = ((e.Location.X > leftMargin) && (e.Location.X < rightMargin));
            
            base.OnMouseDown(e);
        }

		private void HandleMouseClick(MouseEventArgs e, bool doubleClick)
		{
			if (e.Button != MouseButtons.Left)
				return;

			if (!ItemsHaveIcons && !ShowCompletionCheckboxes && !doubleClick)
				return;

			var hit = HitTest(e.Location);

			if (hit.Item == null)
				return;

			var item = (hit.Item.Tag as CloudTaskItem);

			if ((item == null) || item.IsLocked)
				return;

			if (CalcCheckboxRect(hit.Item.Bounds).Contains(e.Location))
			{
				if (EditTaskDone != null)
					EditTaskDone(this, item.Id, !item.IsDone(false));
			}
			else if (CalcIconRect(hit.Item.Bounds).Contains(e.Location))
			{
				if (EditTaskIcon != null)
					EditTaskIcon(this, item.Id);
			}
			else if (doubleClick)
			{
				if (EditTaskLabel != null)
					EditTaskLabel(this, item.Id);
			}
		}

		protected override void OnMouseClick(MouseEventArgs e)
        {
            base.OnMouseClick(e);

			HandleMouseClick(e, false);
        }

		protected override void OnMouseDoubleClick(MouseEventArgs e)
		{
			base.OnMouseDoubleClick(e);

			HandleMouseClick(e, true);
		}

		protected override void OnBeforeLabelEdit(LabelEditEventArgs e)
        {
            if ((e.Item != -1) && (EditTaskLabel != null))
            {
                var item = (Items[e.Item].Tag as CloudTaskItem);

                if (item != null)
                {
                    EditTaskLabel(this, item.Id);

                    e.CancelEdit = true;
                    return;
                }
            }

            base.OnBeforeLabelEdit(e);
        }

		protected override void OnDrawItem(DrawListViewItemEventArgs e)
		{
			if (e.Item == null)
				return;

			if (m_IgnoreNextItemDraw && RectangleToScreen(e.Item.Bounds).Contains(MousePosition))
				return;

			// Draw the background
			var item = (e.Item.Tag as CloudTaskItem);

            var textColor = item.GetTextColor(e.Item.Selected, TaskColorIsBackground);
            var backColor = item.GetBackColor(TaskColorIsBackground);

			Brush textBrush = new SolidBrush(textColor);

			if (TaskColorIsBackground)
			{
				using (Brush backBrush = new SolidBrush(backColor))
					e.Graphics.FillRectangle(backBrush, e.Bounds);
			}
			else if (!e.Item.Selected)
			{
				e.DrawBackground();
			}

			if (e.Item.Selected)
			{
				// Selection rect just around text label
				Rectangle labelRect = CalcLabelTextRect(e.Bounds, true);

				UIExtension.SelectionRect.Draw(Handle, 
												e.Graphics, 
												labelRect.X, 
												labelRect.Y, 
												labelRect.Width, 
												labelRect.Height, 
												false); // opaque
			}

			// Draw subitems
			Rectangle itemRect = e.Bounds;

			for (int colIndex = 0; colIndex < e.Item.SubItems.Count; colIndex++)
			{
				var horzAlign = StringAlignment.Near;

				itemRect.X += 2;
				itemRect.Width = (Columns[colIndex].Width - 2);

				if (colIndex == 0)
                {
                    if (ShowCompletionCheckboxes)
                    {
//                         if (m_CheckBoxSize.IsEmpty)
//                             m_CheckBoxSize = CheckBoxRenderer.GetGlyphSize(e.Graphics, CheckBoxState.UncheckedNormal);

                        var checkRect = CalcCheckboxRect(itemRect);

                        CheckBoxRenderer.DrawCheckBox(e.Graphics, checkRect.Location, GetItemCheckboxState(item));

                        itemRect.X += CheckboxOffset;
                        itemRect.Width -= CheckboxOffset;
                    }

                    if (ItemsHaveIcons)
                    {
                        if ((e.Item.ImageIndex != -1) && TaskIcons.Get(item.Id))
                        {
                            int imageSize = ImageSize;
                            Rectangle iconRect = new Rectangle(itemRect.Location, new Size(imageSize, imageSize));
                            iconRect.Y += ((itemRect.Height - imageSize) / 2);

                            TaskIcons.Draw(e.Graphics, iconRect.Left, iconRect.Top);
                        }

                        itemRect.X += TextIconOffset;
                        itemRect.Width -= TextIconOffset;
                    }
				}
				else
				{
					horzAlign = StringAlignment.Far;
				}

				itemRect.Y++;
				itemRect.Height--;

				DrawText(e.Graphics, 
						e.Item.SubItems[colIndex].Text, 
						itemRect, 
						textBrush, 
						horzAlign,
						(colIndex == 0));

				// next subitem
				itemRect.X += itemRect.Width;
			}
		}

		protected void DrawText(Graphics graphics, String text, Rectangle rect, Brush brush, StringAlignment horzAlign, bool endEllipsis)
		{
			StringFormat format = new StringFormat()
			{
				Alignment = horzAlign,
				LineAlignment = StringAlignment.Center,
				FormatFlags = StringFormatFlags.NoWrap,
				Trimming = (endEllipsis ? StringTrimming.EllipsisCharacter : StringTrimming.None)
			};

			graphics.DrawString(text, this.Font, brush, rect, format);
		}

		protected override void OnDrawColumnHeader(DrawListViewColumnHeaderEventArgs e)
		{
			e.DrawBackground();
			e.DrawText(TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter |TextFormatFlags.SingleLine);
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

			if (ClientRectangle.Width <= MinTaskMatchesWidth)
				return;

			// Resize first column to fill remaining width
			base.BeginUpdate();
			Columns[0].Width = (ClientRectangle.Width - Columns[1].Width - 2);
			base.EndUpdate();
		}

        CheckBoxState GetItemCheckboxState(CloudTaskItem taskItem)
        {
            if (taskItem.IsDone(false))
                return CheckBoxState.CheckedNormal;

            if (taskItem.HasSomeSubtasksDone && ShowMixedCompletionState)
                return CheckBoxState.MixedNormal;

            // else
            return CheckBoxState.UncheckedNormal;
        }
	}
}

