using System;
using System.Drawing;
using System.Collections.Generic;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
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
				var custValues = task.GetCustomAttributeValues(false);

				foreach (var val in custValues)
				{
					double value = 0.0;

					if (double.TryParse(val.Value, out value))
						SetAttributeValue(val.Key, value);
				}
			}

			IsParent = task.IsParent();
			IsLocked = task.IsLocked(true); // Explicitly AND implicitly

			return true;
		}

		// ---------------------------------------------------------------

		protected bool SetAttributeValue(string attribId, double value)
		{
			if (value == GetAttributeValue(attribId))
				return false;

			m_AttribValues[attribId] = value;
			return true;
		}

		protected double GetAttributeValue(string attribId)
		{
			double value;

			if (m_AttribValues.TryGetValue(attribId, out value))
				return value;

			return 0.0;
		}

	}

}

