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
	public delegate Boolean EditTaskLabelEventHandler(object sender, UInt32 taskId);
	public delegate Boolean EditTaskIconEventHandler(object sender, UInt32 taskId);
	public delegate Boolean EditTaskCompletionEventHandler(object sender, UInt32 taskId, bool completed);

	[System.ComponentModel.DesignerCategory("")]

	class TaskMatchesListView : ListView, ILabelTipHandler
	{
		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;

		// -------------------------------------------------------------

		private UIExtension.TaskIcon m_TaskIcons;

		private ImageList m_ilItemHeight;
		private Size m_CheckBoxSize = Size.Empty;
		private LabelTip m_LabelTip;

		private Boolean m_TaskMatchesHaveIcons;
		private Boolean m_ShowParentAsFolder;
		private Boolean m_TaskColorIsBkgnd;
		private Boolean m_ShowCompletionCheckboxes;

		public TaskMatchesListView(IntPtr hwndParent)
		{
			m_TaskIcons = new UIExtension.TaskIcon(hwndParent);

			m_ilItemHeight = new ImageList();
			m_ilItemHeight.ImageSize = new Size(1, DPIScaling.Scale(17)); // minimum height

			m_LabelTip = new LabelTip(this);
		}

        // ILabelTipHandler implementation
        public Font GetFont()
        {
            return Font;
        }

        public Control GetOwner()
        {
            return this;
        }

        public UInt32 ToolHitTest(Point ptScreen, ref String tipText, ref Rectangle toolRect, ref bool multiLine)
        {
            var pt = PointToClient(ptScreen);
            var hit = HitTest(pt);

            if ((hit == null) || (hit.Item == null))
                return 0;

            // Only interested in first (label) column
            var labelRect = LabelTextRect(hit.Item.GetBounds(ItemBoundsPortion.Entire));

            if (!labelRect.Contains(pt))
                return 0;

            var item = (hit.Item.Tag as CloudTaskItem);

            if (item == null)
                return 0;

			// Check if there's enough room already
			if (m_LabelTip.CalcTipHeight(item.Title, labelRect.Width) <= labelRect.Height)
				return 0;

			tipText = item.Title;
			multiLine = false; // always

			toolRect = labelRect;
			toolRect.Offset(-1, -1);

			return item.Id;
        }

        public Boolean TaskColorIsBackground
		{
			get { return m_TaskColorIsBkgnd; }
			set
			{
				if (m_TaskColorIsBkgnd != value)
				{
					m_TaskColorIsBkgnd = value;
					Invalidate();
				}
			}
		}

		public Boolean ShowParentsAsFolders
		{
			get { return m_ShowParentAsFolder; }
			set
			{
				if (m_ShowParentAsFolder != value)
				{
					m_ShowParentAsFolder = value;
					Invalidate();
				}
			}
		}

		public Boolean ShowCompletionCheckboxes
		{
			get { return m_ShowCompletionCheckboxes; }
			set
			{
				if (m_ShowCompletionCheckboxes != value)
				{
					m_ShowCompletionCheckboxes = value;
					Invalidate();
				}
			}
		}

		public bool ShowLabelTips
		{
			set { m_LabelTip.Active = value; }
			get { return m_LabelTip.Active; }
		}

		private int TextIconOffset
        {
            get { return (m_TaskMatchesHaveIcons ? (ImageSize + 2) : 0); }
        }

        private int CheckboxOffset
        {
            get { return (m_ShowCompletionCheckboxes ? (m_CheckBoxSize.Width + 2) : 0); }
        }

        private int ImageSize
        {
            get { return DPIScaling.Scale(16); }
        }

		private int MinTaskMatchesWidth
		{
			get { return DPIScaling.Scale(100); }
		}

		private int LabelTipBorder
		{
			get { return DPIScaling.Scale(4); }
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
                this.LabelEdit = true;
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
            lvItem.Checked = item.IsDone(false);

			if ((item.IsParent && m_ShowParentAsFolder) || item.HasIcon)
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

                    if ((item.IsParent && m_ShowParentAsFolder) || item.HasIcon)
                    {
                        lvItem.ImageIndex = 1;
                        m_TaskMatchesHaveIcons = true;
 
 						someUpdated = true;
                    }
                    else
                    {
                        lvItem.ImageIndex = -1;

                        if (Items.Count == 1)
                        {
                            m_TaskMatchesHaveIcons = false;
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
				editRect = LabelTextRect(SelectedItems[0].GetBounds(ItemBoundsPortion.Label));

			return editRect;
		}

		public Rectangle LabelTextRect(Rectangle labelRect, bool includeIdColumn = false)
		{
			Rectangle textRect = new Rectangle(labelRect.Location, labelRect.Size);

			textRect.X = 2;
			textRect.X += CheckboxOffset;
			textRect.X += TextIconOffset;

			textRect.Width = (Columns[0].Width - textRect.X);

            if (includeIdColumn)
                textRect.Width += Columns[1].Width;

			return textRect;
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
            if (m_LabelTip != null)
                m_LabelTip.ProcessMessage(m);

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

                if (item != null)
                {
                    Cursor cursor = null;

                    if (item.IsLocked)
                    {
                        cursor = UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);
                    }
                    else if (IconRect(hit.Item.Bounds).Contains(e.Location))
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

			if (!m_TaskMatchesHaveIcons && !m_ShowCompletionCheckboxes && !doubleClick)
				return;

			var hit = HitTest(e.Location);

			if (hit.Item == null)
				return;

			var item = (hit.Item.Tag as CloudTaskItem);

			if ((item == null) || item.IsLocked)
				return;

			if (CheckboxRect(hit.Item.Bounds).Contains(e.Location))
			{
				if (EditTaskDone != null)
				{
					bool setDone = !item.IsDone(false);

					if (EditTaskDone(this, item.Id, setDone))
					{
						// If the app hasn't already updated this for us we must do it ourselves
						if (item.IsDone(false) != setDone)
						{
							item.SetDone(setDone);
							RedrawItems(hit.Item.Index, hit.Item.Index, false);
						}
					}

				}
			}
			else if (IconRect(hit.Item.Bounds).Contains(e.Location))
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

			// Draw the background
			var item = (e.Item.Tag as CloudTaskItem);

            var textColor = item.GetTextColor(e.Item.Selected, m_TaskColorIsBkgnd);
            var backColor = item.GetBackColor(m_TaskColorIsBkgnd);

			Brush textBrush = new SolidBrush(textColor);

			if (m_TaskColorIsBkgnd && !backColor.IsEmpty)
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
				Rectangle labelRect = LabelTextRect(e.Bounds, true);

				UIExtension.SelectionRect.Draw(Handle, 
												e.Graphics, 
												labelRect.X, 
												labelRect.Y, 
												labelRect.Width, 
												labelRect.Height, 
												false); // opaque
			}

			// Draw subitems
			StringFormat stringFormat = new StringFormat();
			stringFormat.Alignment = StringAlignment.Near;
			stringFormat.LineAlignment = StringAlignment.Center;
			stringFormat.FormatFlags = StringFormatFlags.NoWrap;
            
			Rectangle itemRect = new Rectangle(e.Bounds.Location, e.Bounds.Size);

			for (int colIndex = 0; colIndex < e.Item.SubItems.Count; colIndex++)
			{
				itemRect.X += 2;
				itemRect.Width = (Columns[colIndex].Width - 2);

				if (colIndex == 0)
                {
                    if (m_ShowCompletionCheckboxes)
                    {
                        if (m_CheckBoxSize.IsEmpty)
                            m_CheckBoxSize = CheckBoxRenderer.GetGlyphSize(e.Graphics, CheckBoxState.UncheckedNormal);

                        var checkRect = CheckboxRect(itemRect);

                        CheckBoxRenderer.DrawCheckBox(e.Graphics, checkRect.Location, GetItemCheckboxState(item));

                        itemRect.X += CheckboxOffset;
                        itemRect.Width -= CheckboxOffset;
                    }

                    if (m_TaskMatchesHaveIcons)
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
				}

				itemRect.Y++;
				itemRect.Height--;

				DrawText(e.Graphics, 
						e.Item.SubItems[colIndex].Text, 
						itemRect, 
						textBrush, 
						StringAlignment.Near,
						(colIndex == 0));

				// next subitem
				itemRect.X += itemRect.Width;
			}
		}

        private Rectangle CheckboxRect(Rectangle labelRect)
        {
            if (!m_ShowCompletionCheckboxes)
                return Rectangle.Empty;

            int top = (((labelRect.Top + labelRect.Bottom) / 2) - (m_CheckBoxSize.Height / 2));

            return new Rectangle(labelRect.X, top, m_CheckBoxSize.Width, m_CheckBoxSize.Height);
        }

		private Rectangle IconRect(Rectangle labelRect)
        {
            if (!m_TaskMatchesHaveIcons)
                return Rectangle.Empty;

            if (m_ShowCompletionCheckboxes)
                labelRect.X += (m_CheckBoxSize.Width + 2);

            int imageSize = ImageSize;
            int top = (((labelRect.Top + labelRect.Bottom) / 2) - (imageSize / 2));

            return new Rectangle(labelRect.X, top, imageSize, imageSize);
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

        CheckBoxState GetItemCheckboxState(CloudTaskItem taskItem)
        {
            if (taskItem.IsDone(false))
                return CheckBoxState.CheckedNormal;

            if (taskItem.HasSomeSubtasksDone)
                return CheckBoxState.MixedNormal;

            // else
            return CheckBoxState.UncheckedNormal;
        }
	}
}

