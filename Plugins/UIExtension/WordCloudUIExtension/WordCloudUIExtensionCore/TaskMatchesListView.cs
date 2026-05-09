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
	class TaskMatchesListView : TaskListView
	{
		const int DefaultMaxTaskId = 100;

		// -------------------------------------------------------------

		private uint m_MaxTaskId = DefaultMaxTaskId;
		private TaskListView.NoTrackHeaderControl m_Header;

		public TaskMatchesListView() : base()
		{
		}

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			// Prevent column resizing to save us having to save/restore the widths
			m_Header = new TaskListView.NoTrackHeaderControl(this);
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
			var lvItem = AddTask(item);

			if (lvItem == null)
				return false;

			m_MaxTaskId = Math.Max(m_MaxTaskId, item.Id);
			return true;
		}

		public void ClearMatches()
		{
			Items.Clear();
			SelectedItems.Clear();

			ItemsHaveIcons = false;
			m_MaxTaskId = DefaultMaxTaskId;
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

		protected override bool TaskMatches(ITaskBase task, String phrase, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			var item = (task as CloudTaskItem);

			if (item == null)
				return false;

			return item.Matches(phrase, caseSensitive, wholeWord, findReplace);
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
            if (taskItem.IsDone)
                return CheckBoxState.CheckedNormal;

            if (taskItem.HasSomeSubtasksDone && ShowMixedCompletionState)
                return CheckBoxState.MixedNormal;

            // else
            return CheckBoxState.UncheckedNormal;
        }
	}
}

