using System;
using System.Drawing;
using System.Collections.Generic;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace TaskDatesUIExtension
{
	public static class MyExtensions
	{
		public static string AttributeKey(this TaskAttributeItem attrib)
		{
			if (attrib == null)
				return string.Empty;

			if (attrib.IsCustom())
				return attrib.CustomAttributeId;

			return attrib.AttributeId.ToString();
		}
	}

	///////////////////////////////////////////////////////////////////

	public class TaskItems : Dictionary<uint, TaskItem>
	{
		public HashSet<uint> Update(TaskList tasks, 
									UIExtension.UpdateType type, 
									IEnumerable<TaskAttributeItem> availAttribs,
									IEnumerable<TaskAttributeItem> groupAttribs)
		{
			switch (type)
			{
			case UIExtension.UpdateType.All:
				Clear();
				return Update(tasks, availAttribs, groupAttribs);

			case UIExtension.UpdateType.Edit:
			case UIExtension.UpdateType.New:
				return Update(tasks, availAttribs, groupAttribs);

			case UIExtension.UpdateType.Delete:
				return RemoveDeletedTasks(tasks);
			}

			Debug.Assert(false);
			return null;
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

		public HashSet<string> GetGroupValues(TaskAttributeItem attrib, bool excDone, bool excParent, bool excNone)
		{
			var groupVals = new HashSet<string>();

			foreach (var task in Values)
			{
				if (excDone && (task.Attributes.IsDone || task.Attributes.IsGoodAsDone))
					continue;

				if (excParent && task.Attributes.IsParent)
					continue;

				var value = task.GetGroupValue(attrib);

				if (excNone && string.IsNullOrEmpty(value))
					continue;

				groupVals.Add(value);
			}

			return groupVals;
		}

		// ---------------------------------

		private HashSet<uint> Update(TaskList tasks, 
									 IEnumerable<TaskAttributeItem> dateAttribs, 
									 IEnumerable<TaskAttributeItem> groupAttribs)
		{
			var modifiedTaskIds = new HashSet<uint>();

			Task task = tasks.GetFirstTask();

			while (task.IsValid())
			{
				if (!ProcessTaskUpdate(task, dateAttribs, groupAttribs, modifiedTaskIds))
					break;

				task = task.GetNextTask();
			}

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

		private bool ProcessTaskUpdate(Task task, 
									   IEnumerable<TaskAttributeItem> availAttribs,
									   IEnumerable<TaskAttributeItem> groupAttribs,
									   HashSet<uint> modifiedTaskIds)
		{
			if (!task.IsValid())
				return false;

			uint taskId = task.GetID();
			var item = GetItem(taskId, true);

			if (!item.ProcessTaskUpdate(task, availAttribs, groupAttribs))
				return false;

			modifiedTaskIds?.Add(taskId);

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid())
			{
				ProcessTaskUpdate(subtask, availAttribs, groupAttribs, modifiedTaskIds); // RECURSIVE CALL
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

		public TaskItem(string title, uint id)
		{
			m_Attribs = new TaskItemAttributes(title, id);
			m_Dates = new TaskItemDates();
		}

		public IEnumerable<TaskItemDate> Dates
		{
			get	{ return m_Dates.Values; }
		}

		public TaskItemAttributes Attributes
		{
			get { return m_Attribs; }
		}

		public string GetGroupValue(TaskAttributeItem attrib)
		{
			return m_Attribs.GetGroupValue(attrib);
		}

		public DateTime GetDate(string attribId)
		{
			var date = m_Dates.GetItem(attribId);
			return ((date != null) ? date.Date : TaskItemDate.NullDate);
		}

		public bool ProcessTaskUpdate(Task task, 
									  IEnumerable<TaskAttributeItem> availAttribs, 
									  IEnumerable<TaskAttributeItem> groupAttribs)
		{
			if (!m_Attribs.ProcessTaskUpdate(task, groupAttribs))
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
		private Dictionary<string, string> m_GroupValues;

		// ------------------------------------------

		public string Title { get; private set; }
		public string Position { get; private set; }
		public uint Id { get; private set; }
		public Color TextColor { get; private set; }
		public bool HasIcon { get; private set; }
		public bool IsParent { get; private set; }
		public bool IsLocked { get; private set; }
		public bool IsDone { get; private set; }
		public bool IsGoodAsDone { get; private set; }

		public TaskItemAttributes(string title, uint id)
		{
			Title = title;
			Id = id;
			TextColor = Color.Empty;

			m_GroupValues = new Dictionary<string, string>();
		}

		public bool ProcessTaskUpdate(Task task, IEnumerable<TaskAttributeItem> groupAttribs)
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

			// Group attributes stored as strings only
			foreach (var attrib in groupAttribs)
			{
				var attribId = attrib.AttributeId;
				var groupKey = attrib.AttributeKey();

				switch (attribId)
				{
				case Task.Attribute.CustomAttribute:
					m_GroupValues[groupKey] = task.GetCustomAttributeValue(groupKey, true);
					break;

				default:
					if (task.IsAttributeAvailable(attribId))
						m_GroupValues[groupKey] = task.GetAttributeValue(attribId, true, true);
					break;
				}
			}

			return true;
		}

		public string GetGroupValue(TaskAttributeItem attrib)
		{
			string value = null;
			m_GroupValues?.TryGetValue(attrib.AttributeKey(), out value);

			return (value ?? string.Empty);
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

	public class TaskItemDate : IListViewTask
	{
		private TaskItemAttributes m_TaskAttribs;

		// -----------------------------------------------------------------

		// ITaskBase
		public string Title		{ get { return m_TaskAttribs.Title; } }
		public string Position	{ get { return m_TaskAttribs.Position; } }
		public uint Id			{ get { return m_TaskAttribs.Id; } }
		public Color TextColor	{ get { return m_TaskAttribs.TextColor; } }
		public bool HasIcon		{ get { return m_TaskAttribs.HasIcon; } }

		// IListViewTask
		public bool IsParent	{ get { return m_TaskAttribs.IsParent; } }
		public bool IsLocked	{ get { return m_TaskAttribs.IsLocked; } }
		public bool IsDone		{ get { return m_TaskAttribs.IsDone; } }

		// local
		public bool IsGoodAsDone { get { return m_TaskAttribs.IsGoodAsDone; } }

		public DateTime Date = NullDate;
		public string AttributeId { get; private set; }

		// -----------------------------------------------------------------

		public static readonly DateTime NullDate = DateTime.MinValue;

		// -----------------------------------------------------------------

		public TaskItemDate(string attribId, TaskItemAttributes taskAttribs)
		{
			AttributeId = attribId;

			m_TaskAttribs = taskAttribs;
		}

		public string FormatDate(bool iso, bool dow)
		{
			if (!DateIsSet)
				return string.Empty;

			var date = (iso ? Date.ToString("yyyy-MM-dd") : Date.ToShortDateString());

			if (dow)
				date = Date.ToString("ddd" + " " + date);

			return date;
		}

		public string FormatOffsetFromToday()
		{
			if (DateIsSet)
				return Date.Date.Subtract(DateTime.Today.Date).Days.ToString();

			// else
			return string.Empty;
		}

		public bool DateIsSet
		{
			get { return (Date != NullDate); }
		}

		public string GetGroupValue(object groupById)
		{
			return m_TaskAttribs.GetGroupValue(groupById as TaskAttributeItem);
		}

		public bool HasGroupValue(object groupById)
		{
			var value = m_TaskAttribs.GetGroupValue(groupById as TaskAttributeItem);
			return !string.IsNullOrEmpty(value);
		}

		public static int CompareDates(TaskItemDate date1, TaskItemDate date2, bool ascending)
		{
			if (ascending)
				return DateTime.Compare(date1.Date, date2.Date);

			// else
			return DateTime.Compare(date2.Date, date1.Date);
		}
	}

	///////////////////////////////////////////////////////////////////////////
}

