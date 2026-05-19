using System;
using System.Drawing;
using System.Collections.Generic;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public class EisenhowerData
	{
		public EisenhowerTasks Tasks { get; private set; }
		public EisenhowerVariables Variables { get; private set; }

		// ---------------------------------

		public EisenhowerData()
		{
			Tasks = new EisenhowerTasks();
			Variables = new EisenhowerVariables();
		}
	}
	
	///////////////////////////////////////////////////////////////////////////

	public class EisenhowerVariables
	{
		// TODO
	}

	///////////////////////////////////////////////////////////////////////////

	public class EisenhowerVariable : TaskAttributeItem
	{
		public bool TypeIsDouble { get; private set; }
		public string Key { get; private set; }

		// ------------------------------

		public EisenhowerVariable(TaskAttributeItem attrib, bool isDouble) : base(attrib)
		{
			TypeIsDouble = isDouble;

			if (string.IsNullOrEmpty(attrib.CustomAttributeId))
				Key = attrib.AttributeId.ToString();
			else
				Key = attrib.CustomAttributeId;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	public class EisenhowerTasks : Dictionary<uint, EisenhowerTask>
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

		public EisenhowerTask GetItem(uint taskId, bool autoCreate = false)
		{
			EisenhowerTask taskItem;

			if (!TryGetValue(taskId, out taskItem) && autoCreate)
			{
				taskItem = new EisenhowerTask("", taskId);
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
			EisenhowerTask item = GetItem(taskId, true); // auto-create

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

	//////////////////////////////////////////////////////////////

	public class EisenhowerTask : TaskItem
	{
		public EisenhowerTask(String label, uint id) : base(label, id)
		{
		}

		public double GetAttributeValue(EisenhowerVariable var)
		{
			return GetAttributeValue(var.Key);
		}

		public void SetAttributeValue(EisenhowerVariable var, double value)
		{
			SetAttributeValue(var.Key, value);
		}
	}
}

