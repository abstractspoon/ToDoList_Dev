using System;
using System.Drawing;
using System.Collections.Generic;

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
	}

	// ---------------------------------------------------------------

	public class TaskItem : ITaskBase
	{
		// Data

		public String Title { get; private set; }
		public String Position { get; private set; }
		public uint Id { get; private set; }
//         public uint ParentID { get; private set; }
		public Color TextColor { get; private set; }
		public bool HasIcon { get; private set; }
		public bool IsFlagged { get; private set; }
		public bool IsParent { get; private set; }
        public bool SomeSubtasksDone { get; private set; }
		public bool IsLocked { get; private set; }
		public bool IsDone { get; private set; }
		public int Priority { get; private set; }
		public int Risk { get; private set; }

		private Dictionary<String, int> m_CustAttribValues;

		// -----------------------------------------------------------------

		public TaskItem(String label, uint id)
		{
			Title = label;
			Id = id;
//             ParentID = 0;
			TextColor = new Color();
			HasIcon = false;
			IsFlagged = false;
			IsParent = false;
            SomeSubtasksDone = false;
			IsLocked = false;
			Priority = -2;
			Risk = -2;
		}

		public TaskItem(Task task)
		{
			Title = task.GetTitle();
			Position = task.GetPositionString();
			Id = task.GetID();
// 			ParentID = task.GetParentID();
			TextColor = task.GetTextDrawingColor();
			HasIcon = (task.GetIcon().Length > 0);
			IsFlagged = task.IsFlagged(false);
			IsParent = task.IsParent();
            SomeSubtasksDone = task.HasSomeSubtasksDone();
			IsLocked = task.IsLocked(true);
			Priority = task.GetPriority(false); // TODO
			Risk = task.GetRisk(false); // TODO
		}

		public override string ToString() 
		{
#if DEBUG
			return String.Format("{0} ({1})", Title, Id);
#else
			return Title;
#endif
		}

		public void Update(Task task, HashSet<Task.Attribute> attribs)
		{
			// TODO
		}

		public bool ProcessTaskUpdate(Task task)
		{
			if (task.GetID() != Id)
				return false;

			if (task.IsAttributeAvailable(Task.Attribute.Title))
				Title = task.GetTitle();

			if (task.IsAttributeAvailable(Task.Attribute.Icon))
				HasIcon = (task.GetIcon().Length > 0);

			if (task.IsAttributeAvailable(Task.Attribute.Flag))
				IsFlagged = task.IsFlagged(false);

			if (task.IsAttributeAvailable(Task.Attribute.Color))
				TextColor = task.GetTextDrawingColor();

			if (task.IsAttributeAvailable(Task.Attribute.Priority))
				Priority = task.GetPriority(false); // TODO

			if (task.IsAttributeAvailable(Task.Attribute.Risk))
				Risk = task.GetRisk(false); // TODO

			//if (task.IsAttributeAvailable(Task.Attribute.CustomAttribute))
			//	CustAttribValues...; // TODO

			if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
                IsDone = task.IsDone();

			if (task.IsAttributeAvailable(Task.Attribute.SubtaskDone))
                SomeSubtasksDone = task.HasSomeSubtasksDone();

			IsParent = task.IsParent();
			IsLocked = task.IsLocked(true);

			return true;
		}
	}

}

