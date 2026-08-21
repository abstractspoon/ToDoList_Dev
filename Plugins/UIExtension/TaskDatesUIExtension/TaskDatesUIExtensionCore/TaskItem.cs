using System;
using System.Drawing;
using System.Collections.Generic;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace TaskDatesUIExtension
{
	public class TaskItems : Dictionary<uint, TaskItem>
	{
		public HashSet<uint> Update(TaskList tasks, UIExtension.UpdateType type, IEnumerable<TaskAttributeItem> availAttribs)
		{
			HashSet<uint> taskIds = null;

			switch (type)
			{
			case UIExtension.UpdateType.All:
				Clear();
				taskIds = Update(tasks, availAttribs);
				break;

			case UIExtension.UpdateType.Edit:
				taskIds = Update(tasks, availAttribs);
				break;

			case UIExtension.UpdateType.New:
				taskIds = Update(tasks, availAttribs);
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

		public TaskItem GetItem(uint taskId, bool autoAdd = false)
		{
			TaskItem taskItem;

			if (!TryGetValue(taskId, out taskItem) && autoAdd)
			{
				taskItem = new TaskItem("", taskId);
				Add(taskId, taskItem);
			}

			return taskItem;
		}

		// ---------------------------------

		private HashSet<uint> Update(TaskList tasks, IEnumerable<TaskAttributeItem> dateAttribs)
		{
			var modifiedTaskIds = new HashSet<uint>();

			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, dateAttribs, modifiedTaskIds))
				task = task.GetNextTask();

			return modifiedTaskIds;
		}

		private HashSet<uint> RemoveDeletedTasks(TaskList tasks)
		{
			var deletedTaskIds = new HashSet<uint>();

			// 1. Locate the tasks
			foreach (var taskId in Keys)
			{
				if (!tasks.HasTask(taskId))
					deletedTaskIds.Add(taskId);
			}

			// 2. Remove them
			foreach (var id in deletedTaskIds)
				Remove(id);

			return deletedTaskIds;
		}

		private bool ProcessTaskUpdate(Task task, IEnumerable<TaskAttributeItem> availAttribs, HashSet<uint> modifiedTaskIds)
		{
			if (!task.IsValid())
				return false;

			uint taskId = task.GetID();
			var item = GetItem(taskId, true);

			if (!item.ProcessTaskUpdate(task, availAttribs))
				return false;

			modifiedTaskIds?.Add(taskId);

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid())
			{
				ProcessTaskUpdate(subtask, availAttribs, modifiedTaskIds); // RECURSIVE CALL
				subtask = subtask.GetNextTask();
			}

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

		public IEnumerable<TaskItemDate> Dates
		{
			get
			{
				return m_Dates.Values;
			}
		}

		public bool ProcessTaskUpdate(Task task, IEnumerable<TaskAttributeItem> availAttribs)
		{
			if (!m_Attribs.ProcessTaskUpdate(task))
				return false;

			// Date Attributes
			foreach (var attrib in availAttribs)
			{
				if (attrib.IsDate())
				{
					var date = m_Dates.GetItem(attrib.GetId(), m_Attribs);

					if (attrib.IsCustom())
						DateTime.TryParse(task.GetCustomAttributeValue(attrib.CustomAttributeId, true), out date.Date);
					else
						date.Date = task.GetDate(attrib.AttributeId, true); // TODO
				}
			}

			return true;
		}
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
		public TaskItemDate GetItem(Task.Attribute attribId, TaskItemAttributes attribs = null)
		{
			return GetItem(attribId.ToString(), attribs);
		}

		public TaskItemDate GetItem(string attribId, TaskItemAttributes attribs = null)
		{
			TaskItemDate date = null;

			if (!TryGetValue(attribId, out date) && (attribs != null))
			{
				date = new TaskItemDate(attribId, attribs);
				this[attribId.ToString()] = date;
			}

			return date;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	public static class TaskAttributeItemExtensions
	{
		public static bool IsDate(this TaskAttributeItem attrib)
		{
			switch (attrib.AttributeId)
			{
			case Task.Attribute.CreationDate:
			case Task.Attribute.StartDate:
			case Task.Attribute.DueDate:
			case Task.Attribute.DoneDate:
			case Task.Attribute.LastModifiedDate:
				return true;

			case Task.Attribute.CustomAttribute:
				return (attrib.CustomAttributeType == CustomAttributeDefinition.Attribute.Date);
			}

			return false;
		}

		public static string GetId(this TaskAttributeItem attrib)
		{
			if (attrib.IsCustom())
				return attrib.CustomAttributeId;

			return attrib.AttributeId.ToString();
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
		public bool IsGoodAsDone { get { return m_Attrib.IsGoodAsDone; } }

		public DateTime Date = DateTime.MinValue;
		public String AttributeId { get; private set; }

		// -----------------------------------------------------------------

		public TaskItemDate(string attribId, TaskItemAttributes attrib)
		{
			AttributeId = attribId;

			m_Attrib = attrib;
		}

		public string FormatDate(bool iso)
		{
			if (DateIsSet)
				return (iso ? Date.ToString("yyyy-MM-dd") : Date.ToShortDateString());

			// else
			return string.Empty;
		}

		public string FormatOffset(DateTime from)
		{
			if (DateIsSet)
				return DateTime.Today.Subtract(Date).Days.ToString();

			// else
			return string.Empty;
		}

		public bool DateIsSet
		{
			get { return (Date != DateTime.MinValue); }
		}

		public static int CompareDates(TaskItemDate date1, TaskItemDate date2, bool ascending)
		{
			if (ascending)
				return DateTime.Compare(date1.Date, date2.Date);

			// else
			return DateTime.Compare(date2.Date, date1.Date);
		}
	}
}

