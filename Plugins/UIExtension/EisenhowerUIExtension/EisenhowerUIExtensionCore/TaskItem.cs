using System;
using System.Drawing;
using System.Collections.Generic;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public class EisenhowerData
	{

	}
	
	///////////////////////////////////////////////////////////////////////////

	public class TaskAttributes
	{

	}
	
	///////////////////////////////////////////////////////////////////////////

	public class TaskItems : Dictionary<uint, TaskItem>
	{
		public void Rebuild(TaskList tasks)
		{
			Clear();
			Update(tasks);
		}

		public List<uint> Update(TaskList tasks)
		{
			var processedTaskIds = new List<uint>();
			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, processedTaskIds))
				task = task.GetNextTask();

			return processedTaskIds;
		}

		public List<uint> RemoveDeletedTasks(TaskList tasks)
		{
			var removedIds = new List<uint>();

			// Find the deleted tasks
			foreach (var taskId in Keys)
			{
				if (!tasks.HasTask(taskId))
					removedIds.Add(taskId);
			}

			// Remove them
			removedIds.ForEach(id => Remove(id));

			return removedIds;
		}

		public List<uint> RemoveCompletedTasks(TaskList tasks)
		{
			var removedIds = new List<uint>();

			// Find the completed tasks
			foreach (var taskId in Keys)
			{
				var task = tasks.FindTask(taskId);

				if (task.IsDone() || task.IsGoodAsDone())
					removedIds.Add(taskId);
			}

			// Remove them
			removedIds.ForEach(id => Remove(id));

			return removedIds;
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

		public double CalculateAttributeValueMidpoint(EisenhowerVariable var)
		{
			switch (var.AttributeId)
			{
			case Task.Attribute.Priority:
			case Task.Attribute.Risk:
				return 5;
			}

			// All the rest
			double minVal = double.MaxValue, maxVal = double.MinValue;

			foreach (var task in Values)
			{
				double attribVal = task.GetAttributeValue(var);

				minVal = Math.Min(minVal, attribVal);
				maxVal = Math.Max(maxVal, attribVal);
			}

			if (minVal > 0.0)
				return (maxVal / 2); // minVal => 0

			// else
			// TODO - Make this a 'nice' number
			return ((maxVal - minVal) / 2);
		}

		// -------------------------------------------------

		private bool ProcessTaskUpdate(Task task, List<uint> processedTaskIds)
		{
			if (!task.IsValid())
				return false;

			uint taskId = task.GetID();
			TaskItem item = GetItem(taskId, true); // auto-create

			if (!item.ProcessTaskUpdate(task))
				return false;

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, processedTaskIds)) // RECURSIVE CALL
				subtask = subtask.GetNextTask();

			processedTaskIds.Add(task.GetID());

			return true;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	public class TaskItem : ITaskBase
	{
		private Dictionary<String, double> m_AttribValues = new Dictionary<String, double>();

		// -----------------------------------------------------------------

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
		public bool SomeSubtasksDone { get; private set; }

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
            SomeSubtasksDone = false;
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

		public double GetAttributeValue(EisenhowerVariable var)
		{
			return GetAttributeValue(var.Key);
		}

		public void SetAttributeValue(EisenhowerVariable var, double value)
		{
			m_AttribValues[var.Key] = value;
		}

		public bool ProcessTaskUpdate(Task task)
		{
			if (task.GetID() != Id)
				return false;

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

			if (task.IsAttributeAvailable(Task.Attribute.SubtaskDone))
                SomeSubtasksDone = task.HasSomeSubtasksDone();

			// Filterable attributes
			if (task.IsAttributeAvailable(Task.Attribute.Priority))
				SetAttributeValue("Priority", task.GetPriority(true));

			if (task.IsAttributeAvailable(Task.Attribute.Risk))
				SetAttributeValue("Risk", task.GetRisk(true));

			if (task.IsAttributeAvailable(Task.Attribute.Cost))
				SetAttributeValue("Cost", task.GetCost(true));

			if (task.IsAttributeAvailable(Task.Attribute.CustomAttribute))
			{
				// TODO
			}

			IsParent = task.IsParent();
			IsLocked = task.IsLocked(true);

			return true;
		}

		// ---------------------------------------------------------------

		private bool SetAttributeValue(string attribId, double value)
		{
			if (value == GetAttributeValue(attribId))
				return false;

			m_AttribValues[attribId] = value;
			return true;
		}

		public double GetAttributeValue(string attribId)
		{
			double value;

			if (m_AttribValues.TryGetValue(attribId, out value))
				return value;

			return 0.0;
		}

	}

}

