using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

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
		private UIExtension.SelectionRect m_SelectionRect;
		private Translator m_Trans;
		private UIExtension.TaskIcon m_TaskIcons;

		private System.Collections.Generic.Dictionary<UInt32, TaskDataItem> m_Items;

		public TdlMindMapControl(Translator trans, UIExtension.TaskIcon icons)
		{
			m_Trans = trans;
			m_TaskIcons = icons;

			m_SelectionRect = new UIExtension.SelectionRect();
			m_Items = new System.Collections.Generic.Dictionary<UInt32, TaskDataItem>();
		}

		public void RebuildTreeView(TaskList tasks)
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
				ExpandAll();
			else
				rootNode.Expand();

			SetSelectedNode(0); // root
			RecalculatePositions();
		}

		protected override void DrawNodeLabel(Graphics graphics, String label, Rectangle rect, bool isSelected, Object itemData)
		{
			var taskItem = (itemData as TaskDataItem);

			if (isSelected)
			{
				Rectangle selRect = Rectangle.Inflate(rect, -2, 0);
				m_SelectionRect.Draw(graphics, selRect.X, selRect.Y, selRect.Width, selRect.Height, this.Focused);

			}
			else if (DebugMode())
			{
				graphics.DrawRectangle(new Pen(Color.Green), rect);
			}

			var format = new StringFormat();

			format.LineAlignment = StringAlignment.Center;
			format.Alignment = StringAlignment.Center;

			Brush textColor = SystemBrushes.WindowText;

			graphics.DrawString(label, this.Font, textColor, rect, format);
		}

		protected override void DrawNodeConnection(Graphics graphics, Point ptFrom, Point ptTo)
		{
			base.DrawNodeConnection(graphics, ptFrom, ptTo);
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
	}
}

