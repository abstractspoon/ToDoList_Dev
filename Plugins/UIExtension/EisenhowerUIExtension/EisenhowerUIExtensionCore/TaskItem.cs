using System;
using System.Drawing;
using System.Collections.Generic;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public class TaskItems : Dictionary<uint, TaskItem>
	{
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
	}

	// ---------------------------------------------------------------

	public class TaskItem : ITaskBase
	{
		// Data

		public String Title { get; private set; }
		public String Position { get; private set; }
		public uint Id { get; private set; }
		public Color TextColor { get; private set; }
		public bool HasIcon { get; private set; }
		public bool IsFlagged { get; private set; }
		public bool IsParent { get; private set; }
        public bool SomeSubtasksDone { get; private set; }
		public bool IsLocked { get; private set; }
		public bool IsDone { get; private set; }

		private Dictionary<String, double> m_AttribValues = new Dictionary<String, double>();

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
			double value;

			if (m_AttribValues.TryGetValue(var.Key, out value))
				return value;

			return 0.0;
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
                IsDone = task.IsDone();

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

		private void SetAttributeValue(string attribId, double value)
		{
			m_AttribValues[attribId] = value;
		}
	}

}

