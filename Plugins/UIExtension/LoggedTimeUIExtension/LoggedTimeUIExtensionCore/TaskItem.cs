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

	public class TaskItem : ITaskBase
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
			get { return !TextColor.IsEmpty; }
		}

		static public TaskItem None(string title)
		{
			return new TaskItem() { Title = title, Id = 0 };
		}

		public string Title { get; private set; }
		public string Position { get; private set; }
		public uint Id { get; private set; }
		public bool IsLocked { get; private set; }
		public bool IsParent { get; private set; }
		public bool HasIcon { get; private set; }
        public bool IsDone { get; private set; }
		public Color TextColor { get; private set; }

		public bool UpdateTaskAttributes(Task task, UIExtension.UpdateType type, bool newTask)
		{
			if (!task.IsValid())
				return false;

			UInt32 taskID = task.GetID();

			// Always
			TextColor = task.GetTextDrawingColor();
			IsLocked = task.IsLocked(true);
			IsParent = task.IsParent();
			Position = task.GetPositionString();

			if (newTask)
			{
				Title = task.GetTitle();
				HasIcon = task.HasIcon();
				Id = taskID;
				IsDone = task.IsDone();
			}
			else
			{
				if (task.IsAttributeAvailable(Task.Attribute.Title))
					Title = task.GetTitle();

				if (task.IsAttributeAvailable(Task.Attribute.Icon))
					HasIcon = task.HasIcon();

				if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
				    IsDone = task.IsDone();
			}

			return true;
		}
	}

}
