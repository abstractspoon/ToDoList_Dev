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
		private bool m_ShowMixedCompletionState;

		// -------------------------------------------------------------

		public TaskMatchesListView() : base()
		{
			MultiSelect = false;
			LabelEdit = true;
			EnableHeaderTracking = false;
			MinTaskColumnWidth = DPIScaling.Scale(100);
			SizeTaskColumnToFit = true;
		}


		public new bool Initialize(Translator trans, UIExtension.TaskIcon taskIcons)
        {
			if (this.Columns.Count == 0) // once only
			{
				base.Initialize(trans, taskIcons);

				Columns.Add(Translate("Task Matches", Translator.Type.Header));
				Columns.Add(Translate("ID", Translator.Type.Header));

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

		public uint GetTaskId(UIExtension.GetTask getTask)
		{
			return GetTaskIdEx(getTask, true);
		}

		protected override void OnFontChanged(EventArgs e)
		{
			base.OnFontChanged(e);

			RefreshIDColumnWidth();
		}

		public bool ShowMixedCompletionState
		{
			get { return m_ShowMixedCompletionState; }

			set
			{
				if (m_ShowMixedCompletionState != value)
				{
					m_ShowMixedCompletionState = value;
					Invalidate();
				}
			}
		}

		protected override CheckBoxState GetTaskCheckboxState(ITaskBase task)
		{
			if (m_ShowMixedCompletionState)
			{
				var item = (task as CloudTaskItem);

				if ((item != null) && item.HasSomeSubtasksDone)
					return CheckBoxState.MixedNormal;
			}

			// all else
			return base.GetTaskCheckboxState(task);
		}

		public new UInt32 SelectTask(UInt32 taskId)
		{
			if (!base.SelectTask(taskId))
			{
				if (Items.Count == 0)
				{
					taskId = 0;
				}
				else
				{
					Items[0].Selected = true;
					EnsureSelectionVisible();

					taskId = GetTaskId(0);
				}
			}

			return taskId;
		}

		public bool AddMatch(CloudTaskItem item)
		{
			var lvItem = AddTask(item);

			if (lvItem == null)
				return false;

			lvItem.SubItems.Add(item.Id.ToString());
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

		protected override void OnDrawColumnHeader(DrawListViewColumnHeaderEventArgs e)
		{
			e.DrawBackground();
			e.DrawText(TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter | TextFormatFlags.SingleLine);
		}

		protected override void OnColumnClick(ColumnClickEventArgs e)
		{
			// Flip sort direction
			this.Sorting = (this.Sorting == SortOrder.Ascending ? SortOrder.Descending : SortOrder.Ascending);
			Sort();
		}
	}
}

