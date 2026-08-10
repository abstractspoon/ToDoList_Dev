using System;
using System.Drawing;
using System.Collections.Generic;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace TaskDatesUIExtension
{
	public class TaskItems : Dictionary<uint, TaskItem>
	{
		public List<uint> Update(TaskList tasks, UIExtension.UpdateType type)
		{
			List<uint> taskIds = null;

			switch (type)
			{
			case UIExtension.UpdateType.All:
				Clear();
				taskIds = Update(tasks);
				break;

			case UIExtension.UpdateType.Edit:
				taskIds = RemoveCompletedTasks(tasks);
				taskIds.AddRange(Update(tasks));
				break;

			case UIExtension.UpdateType.New:
				taskIds = Update(tasks);
				break;

			case UIExtension.UpdateType.Delete:
				taskIds = RemoveDeletedTasks(tasks);
				break;
			}

			return taskIds;
		}

		public bool HasItem(uint taskId)
		{
			return ContainsKey(taskId);
		}

		public TaskItem GetItem(uint taskId, bool autoCreate = false)
		{
			TaskItem taskItem;

			if (!TryGetValue(taskId, out taskItem) && autoCreate)
			{
				taskItem = new TaskItem("", taskId);
				Add(taskId, taskItem);
			}

			return taskItem;
		}

		// ---------------------------------

		private List<uint> Update(TaskList tasks)
		{
			var modifiedTaskIds = new List<uint>();

			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, modifiedTaskIds))
				task = task.GetNextTask();

			return modifiedTaskIds;
		}

		private List<uint> RemoveDeletedTasks(TaskList tasks)
		{
			var deletedTaskIds = new List<uint>();

			// 1. Locate the tasks
			foreach (var taskId in Keys)
			{
				if (!tasks.HasTask(taskId))
					deletedTaskIds.Add(taskId);
			}

			// 2. Remove them
			deletedTaskIds?.ForEach(id => Remove(id));

			return deletedTaskIds;
		}

		private List<uint> RemoveCompletedTasks(TaskList tasks)
		{
			var completedTaskIds = new List<uint>();

			// 1. Locate the tasks
			foreach (var taskId in Keys)
			{
				var task = tasks.FindTask(taskId);

				if (task.IsDone() || task.IsGoodAsDone())
					completedTaskIds.Add(taskId);
			}

			// 2. Remove them
			completedTaskIds?.ForEach(id => Remove(id));

			return completedTaskIds;
		}

		// -------------------------------------------------

		private bool ProcessTaskUpdate(Task task, List<uint> modifiedTaskIds)
		{
			if (!task.IsValid())
				return false;

			// Be careful not to re-add tasks which may have been completed
			if (!task.IsDone() && !task.IsGoodAsDone())
			{
				uint taskId = task.GetID();
				var item = GetItem(taskId, true);

				if (!item.ProcessTaskUpdate(task))
					return false;

				modifiedTaskIds?.Add(taskId);

				// Process children
				Task subtask = task.GetFirstSubtask();

				while (subtask.IsValid() && ProcessTaskUpdate(subtask, modifiedTaskIds)) // RECURSIVE CALL
					subtask = subtask.GetNextTask();
			}

			return true;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	public class TaskItem : ITaskBase
	{
		// ITaskBase
		public String Title { get; private set; }
		public String Position { get; private set; }
		public uint Id { get; private set; }
		public Color TextColor { get; private set; }
		public bool HasIcon { get; private set; }
		public bool IsParent { get; private set; }
		public bool IsLocked { get; private set; }
		public bool IsDone { get; private set; }

		// Local
		public bool IsFlagged { get; private set; }
		public bool IsPartlyDone { get; private set; }

		// -----------------------------------------------------------------

		public TaskItem(String label, uint id)
		{
			Title = label;
			Id = id;
			Position = string.Empty;
			TextColor = new Color();
			HasIcon = false;
			IsFlagged = false;
			IsParent = false;
            IsPartlyDone = false;
			IsLocked = false;
		}

		public override string ToString() 
		{
#if DEBUG
			return String.Format("{0} ({1})", Title, Id);
#else
			return Title;
#endif
		}

		public bool ProcessTaskUpdate(Task task)
		{
			if (task.GetID() != Id)
				return false;

			IsParent = task.IsParent();
			IsLocked = task.IsLocked(true); // Explicitly AND implicitly
			IsPartlyDone = task.IsPartlyDone();

			if (task.IsAttributeAvailable(Task.Attribute.Title))
				Title = task.GetTitle();

			if (task.IsAttributeAvailable(Task.Attribute.Position))
				Position = task.GetPositionString();

			if (task.IsAttributeAvailable(Task.Attribute.Icon))
				HasIcon = (task.GetIcon().Length > 0);

			if (task.IsAttributeAvailable(Task.Attribute.Flag))
				IsFlagged = task.IsFlagged(false);

			if (task.IsAttributeAvailable(Task.Attribute.Color))
				TextColor = task.GetTextDrawingColor();

			if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
                IsDone = (task.IsDone() || task.IsGoodAsDone());

			return true;
		}
	}

}

