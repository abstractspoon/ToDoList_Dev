using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;

namespace MindMapUIExtension
{

	public class TaskDataItem
	{
		public TaskDataItem(String label)
		{
			m_Title = label;
			m_TaskID = 0;
		}

		public TaskDataItem(Task task)
		{
			m_Title = task.GetTitle();
			m_TaskID = task.GetID();
		}

		public override string ToString() { return m_Title; }

		public void Update(Task task, HashSet<UIExtension.TaskAttribute> attribs)
		{
			// TODO
		}

		public String Title { get { return String.Format("{0} ({1})", m_Title, m_TaskID); } }
		public UInt32 ID { get { return m_TaskID; } }

		private String m_Title;
		private UInt32 m_TaskID;
	}

	// ------------------------------------------------------------

	[System.ComponentModel.DesignerCategory("")]

	public class TdlMindMapControl : MindMapControl
	{
		// From Parent
		private Translator m_Trans;
		private UIExtension.TaskIcon m_TaskIcons;

		private UIExtension.SelectionRect m_SelectionRect;
		private Dictionary<UInt32, TaskDataItem> m_Items;

		// -------------------------------------------------------------------------

		public TdlMindMapControl(Translator trans, UIExtension.TaskIcon icons)
		{
			m_Trans = trans;
			m_TaskIcons = icons;

			m_SelectionRect = new UIExtension.SelectionRect();
			m_Items = new Dictionary<UInt32, TaskDataItem>();
		}

		public void UpdateTasks(TaskList tasks,
								UIExtension.UpdateType type,
								HashSet<UIExtension.TaskAttribute> attribs)
		{
			switch (type)
			{
				case UIExtension.UpdateType.Edit:
					UpdateTaskAttributes(tasks, attribs);
					break;

				case UIExtension.UpdateType.New:
					break;

				case UIExtension.UpdateType.Delete:
				case UIExtension.UpdateType.All:
					RebuildTreeView(tasks);
					break;

				case UIExtension.UpdateType.Unknown:
					return;
			}
		}

		public Rectangle GetSelectedItemRect()
		{
			return GetSelectedItemPosition();
		}

		// Internal ------------------------------------------------------------

		protected void UpdateTaskAttributes(TaskList tasks,
								HashSet<UIExtension.TaskAttribute> attribs)
		{
			// TODO
		}

		protected void RebuildTreeView(TaskList tasks)
		{
			Clear();

			if (tasks.GetTaskCount() == 0)
				return;

			var task = tasks.GetFirstTask();
			bool taskIsRoot = !task.GetNextTask().IsValid(); // no siblings

			TreeNode rootNode = null;

			if (taskIsRoot)
			{
				rootNode = AddRootNode(new TaskDataItem(task), task.GetID());

				// First Child
				AddTaskToTree(task.GetFirstSubtask(), rootNode);
			}
			else
			{
				// There is more than one 'root' task so create a real root parent
				rootNode = AddRootNode(new TaskDataItem("Root"));

				AddTaskToTree(task, rootNode);
			}

			if (DebugMode())
				Expand(ExpandNode.ExpandAll);
			else
				rootNode.Expand();

			SetSelectedNode(0); // root
			RecalculatePositions();
		}

		protected override void DrawNodeLabel(Graphics graphics, String label, Rectangle rect,
												bool isSelected, bool leftOfRoot, Object itemData)
		{
			var taskItem = (itemData as TaskDataItem);
			Brush textColor = SystemBrushes.WindowText;

			if (isSelected)
			{
				m_SelectionRect.Draw(graphics, rect.X, rect.Y, rect.Width, rect.Height, this.Focused);
			}
			else
			{
				// Use task colours
				// TODO
				
				if (DebugMode())
					graphics.DrawRectangle(new Pen(Color.Green), rect);
			}

			graphics.DrawString(label, this.Font, textColor, rect, DefaultLabelFormat());
		}

		protected override void DrawNodeConnection(Graphics graphics, Point ptFrom, Point ptTo)
		{
			int midX = ((ptFrom.X + ptTo.X) / 2);
																		
			graphics.DrawBezier(new Pen(Color.Magenta), 
								ptFrom,
								new Point(midX, ptFrom.Y),
								new Point(midX, ptTo.Y),
								ptTo);
		}

		private new void Clear()
		{
			m_Items.Clear();

			base.Clear();
		}

		private bool AddTaskToTree(Task task, TreeNode parent)
		{
			if (!task.IsValid())
				return true; // not an error

			var taskID = task.GetID();
			var taskItem = new TaskDataItem(task);

			var node = AddNode(taskItem, parent, taskID);

			if (node == null)
				return false;

			// First Child
			if (!AddTaskToTree(task.GetFirstSubtask(), node))
				return false;

			// First Sibling
			if (!AddTaskToTree(task.GetNextTask(), parent))
				return false;

			m_Items.Add(taskID, taskItem);

			return true;
		}

		protected override int GetMinItemHeight()
		{
			return 18;
		}
	}
}

