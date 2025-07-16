using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;

namespace LoggedTimeUIExtension
{
	public class TaskItems : Dictionary<uint, TaskItem>
	{
		public TaskItem GetItem(uint taskId, bool autoCreate = false)
		{
			TaskItem taskItem;

			if (!TryGetValue(taskId, out taskItem) && autoCreate)
			{
				taskItem = new TaskItem();
				Add(taskId, taskItem);
			}

			return taskItem;
		}
	}

	// ---------------------------------------------------------------

/*
	public class DateSortedTasks
	{
		public class TaskList : List<TaskItem>
		{
			public int FindItem(uint taskID)
			{
				return FindIndex(x => (x.Id == taskID));
			}

			public int NextIndex(uint taskID, bool forwards)
			{
				return NextIndex(FindItem(taskID), forwards);
			}

			public int NextIndex(int item, bool forwards)
			{
				if (item == -1)
					return -1;

				item = (forwards ? item + 1 : item - 1);

				if ((item < 0) || (item >= Count))
					return -1;

				return item;
			}
		}

		// ------------------

		private TaskList m_SortedTaskList;
		private TaskItems m_TaskItems; // read-only
		private bool m_NeedsResort = true;

		// ------------------

		public DateSortedTasks(TaskItems items)
		{
			m_TaskItems = items;
			m_SortedTaskList = new TaskList();
			m_NeedsResort = true;
		}

		public TaskList Items
		{
			get
			{
				if (m_SortedTaskList.Count == 0)
					m_SortedTaskList.AddRange(m_TaskItems.Values);

				if (m_NeedsResort)
				{
					m_NeedsResort = false;
					m_SortedTaskList.Sort((a, b) => TaskItem.CompareDates(a, b));
				}

				return m_SortedTaskList;
			}
		}

		public void SetNeedsRebuild()
		{
			m_SortedTaskList.Clear();
			m_NeedsResort = true;
		}

		public void SetNeedsResort()
		{
			m_NeedsResort = true;
		}
	}
*/

	// ---------------------------------------------------------------

	public class TaskItem : ITask
	{
		public TaskItem()
		{
		}

		public override string ToString()
		{
			return string.Format("{0} {1}", Id, Title);
		}

		public bool TitleMatches(string[] words, bool caseSensitive, bool wholeWord)
		{
			var compareCase = (caseSensitive ? StringComparison.InvariantCulture : StringComparison.InvariantCultureIgnoreCase);

			foreach (var word in words)
			{
				if (wholeWord)
				{
					foreach (var tWord in Title.Split(' '))
					{
						if (tWord.Equals(word, compareCase))
							return true;
					}
				}
				else
				{
					if (Title.IndexOf(word, compareCase) >= 0)
						return true;
				}
			}

			return false;
		}

		public bool HasTaskTextColor
		{
			get { return !TaskTextColor.IsEmpty; }
		}

		static public TaskItem None
		{
			get { return new TaskItem() { Title = "<none>", Id = 0 }; }
		}

		public string Title { get; private set; }
		public string Position { get; private set; }
		public uint Id { get; private set; }
		public bool Locked { get; private set; }
		public bool IsParent { get; private set; }
		public bool HasIcon { get; private set; }
        public bool IsDone { get; private set; }
        public bool IsGoodAsDone { get; private set; }
		public int Depth { get; private set; }
		public Color TaskTextColor { get; private set; }

		public bool IsDoneOrGoodAsDone { get { return IsDone || IsGoodAsDone; } }

		public bool UpdateTaskAttributes(Task task, UIExtension.UpdateType type, bool newTask, int depth)
		{
			if (!task.IsValid())
				return false;

			UInt32 taskID = task.GetID();

			// Always
			TaskTextColor = task.GetTextDrawingColor();
			Locked = task.IsLocked(true);
			IsParent = task.IsParent();
			Position = task.GetPositionString();

			if (newTask)
			{
				Title = task.GetTitle();
				HasIcon = task.HasIcon();
				Id = taskID;
				Depth = depth;
				IsDone = task.IsDone();
                IsGoodAsDone = task.IsGoodAsDone();
			}
			else
			{
				if (task.IsAttributeAvailable(Task.Attribute.Title))
					Title = task.GetTitle();

				if (task.IsAttributeAvailable(Task.Attribute.Icon))
					HasIcon = task.HasIcon();

				if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
				{
				    IsDone = task.IsDone();
                    IsGoodAsDone = task.IsGoodAsDone();
				}
			}

			return true;
		}
	}

}
