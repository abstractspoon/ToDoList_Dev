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
				taskIds = Update(tasks);
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

		// -------------------------------------------------

		private bool ProcessTaskUpdate(Task task, List<uint> modifiedTaskIds)
		{
			if (!task.IsValid())
				return false;

			uint taskId = task.GetID();
			var item = GetItem(taskId, true);

			if (!item.ProcessTaskUpdate(task))
				return false;

			modifiedTaskIds?.Add(taskId);

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, modifiedTaskIds)) // RECURSIVE CALL
				subtask = subtask.GetNextTask();

			return true;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	public class TaskItem
	{
		TaskItemAttributes m_Attribs;
		TaskItemDates m_Dates;

		// -----------------------------------------------------------------

		public TaskItem(String title, uint id)
		{
			m_Attribs = new TaskItemAttributes(title, id);
			m_Dates = new TaskItemDates();
		}

		public IEnumerable<TaskItemDate> Dates { get { return m_Dates.Values; } }

		public bool ProcessTaskUpdate(Task task)
		{
			if (!m_Attribs.ProcessTaskUpdate(task))
				return false;

			// Date Attributes
			if (task.IsAttributeAvailable(Task.Attribute.StartDate))
			{
				var date = m_Dates.GetItem(m_Attribs, Task.Attribute.StartDate);

				date.Date = task.GetAttributeValue(Task.Attribute.StartDate, true, true); // TODO
				date.Type = "Start Date"; // TODO
			}

			if (task.IsAttributeAvailable(Task.Attribute.DueDate))
			{
				var date = m_Dates.GetItem(m_Attribs, Task.Attribute.DueDate);

				date.Date = task.GetAttributeValue(Task.Attribute.DueDate, true, true); // TODO
				date.Type = "Due Date";
			}

			if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
			{
				var date = m_Dates.GetItem(m_Attribs, Task.Attribute.DoneDate);

				date.Date = task.GetAttributeValue(Task.Attribute.DoneDate, true, true); // TODO
				date.Type = "Completion Date";
			}

			return true;
		}

		// -----------------------------------------------------------------
	}

	///////////////////////////////////////////////////////////////////////////

	public class TaskItemAttributes
	{
		public String Title { get; private set; }
		public String Position { get; private set; }
		public uint Id { get; private set; }
		public Color TextColor { get; private set; }
		public bool HasIcon { get; private set; }
		public bool IsParent { get; private set; }
		public bool IsLocked { get; private set; }
		public bool IsDone { get; private set; }
		public bool IsGoodAsDone { get; private set; }

		public TaskItemAttributes(String title, uint id)
		{
			Title = title;
			Id = id;
			TextColor = Color.Empty;
		}

		public bool ProcessTaskUpdate(Task task)
		{
			if (task.GetID() != Id)
				return false;

			IsParent = task.IsParent();
			IsLocked = task.IsLocked(true); // Explicitly AND implicitly

			if (task.IsAttributeAvailable(Task.Attribute.Title))
				Title = task.GetTitle();

			if (task.IsAttributeAvailable(Task.Attribute.Position))
				Position = task.GetPositionString();

			if (task.IsAttributeAvailable(Task.Attribute.Icon))
				HasIcon = (task.GetIcon().Length > 0);

			if (task.IsAttributeAvailable(Task.Attribute.Color))
				TextColor = task.GetTextDrawingColor();

			if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
			{
				IsDone = task.IsDone();
				IsGoodAsDone = task.IsGoodAsDone();
			}

			return true;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	public class TaskItemDates : Dictionary<string, TaskItemDate>
	{
		public TaskItemDate GetItem(TaskItemAttributes attribs, Task.Attribute attribId, bool autoAdd = true)
		{
			return GetItem(attribs, attribId.ToString(), autoAdd);
		}

		public TaskItemDate GetItem(TaskItemAttributes attribs, string attribId, bool autoAdd = true)
		{
			TaskItemDate date = null;

			if (!TryGetValue(attribId, out date) && autoAdd)
			{
				date = new TaskItemDate(attribs);
				this[attribId.ToString()] = date;
			}

			return date;
		}
	}

	///////////////////////////////////////////////////////////////////////////

		public class TaskItemDate : ITaskBase
	{
		private TaskItemAttributes m_Attrib;

		// -----------------------------------------------------------------

		// ITaskBase
		public String Title		{ get { return m_Attrib.Title; } }
		public String Position	{ get { return m_Attrib.Position; } }
		public uint Id			{ get { return m_Attrib.Id; } }
		public Color TextColor	{ get { return m_Attrib.TextColor; } }
		public bool HasIcon		{ get { return m_Attrib.HasIcon; } }
		public bool IsParent	{ get { return m_Attrib.IsParent; } }
		public bool IsLocked	{ get { return m_Attrib.IsLocked; } }
		public bool IsDone		{ get { return m_Attrib.IsDone; } }

		// local
		public string Date = string.Empty;
		public String Type = string.Empty;
		public String LeadIn = string.Empty;

		// -----------------------------------------------------------------

		public TaskItemDate(TaskItemAttributes attrib)
		{
			m_Attrib = attrib;
		}

		public override string ToString() 
		{
#if DEBUG
			return String.Format("{0} ({1})", Title, Id);
#else
			return Title;
#endif
		}

	}
}

