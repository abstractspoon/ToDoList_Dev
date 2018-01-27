using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;

namespace MindMapUIExtension
{
	public delegate void EditTaskLabelEventHandler(object sender, UInt32 taskId);

	public class MindMapTaskItem
	{
		// Data
		private String m_Title;
		private UInt32 m_TaskID;
		private Color m_TextColor;
		private Boolean m_HasIcon;
		private Boolean m_IsFlagged;
		private Boolean m_IsParent;
        private Boolean m_IsDone;

		// -----------------------------------------------------------------

		public MindMapTaskItem(String label)
		{
			m_Title = label;
			m_TaskID = 0;
			m_TextColor = new Color();
			m_HasIcon = false;
			m_IsFlagged = false;
			m_IsParent = false;
		}

		public MindMapTaskItem(Task task)
		{
			m_Title = task.GetTitle();
			m_TaskID = task.GetID();
			m_TextColor = task.GetTextDrawingColor();
			m_HasIcon = (task.GetIcon().Length > 0);
			m_IsFlagged = task.IsFlagged();
			m_IsParent = task.IsParent();
            m_IsDone = (task.IsDone() || task.IsGoodAsDone());
		}

		public override string ToString() 
		{
			return Title;
		}

		public void Update(Task task, HashSet<UIExtension.TaskAttribute> attribs)
		{
			// TODO
		}

		public String Title 
		{ 
			get 
			{ 
#if DEBUG
				return String.Format("{0} ({1})", m_Title, m_TaskID); 
#else
				return m_Title;
#endif
			} 
		}
		
		public UInt32 ID { get { return m_TaskID; } }
		public Color TextColor { get { return m_TextColor; } }
		public Boolean HasIcon { get { return m_HasIcon; } }
		public Boolean IsFlagged { get { return m_IsFlagged; } }
		public Boolean IsParent { get { return m_IsParent; } }
		public Boolean IsDone { get { return m_IsDone; } }

		public bool ProcessTaskUpdate(Task task, HashSet<UIExtension.TaskAttribute> attribs)
		{
			if (task.GetID() != m_TaskID)
				return false;

			if (attribs.Contains(UIExtension.TaskAttribute.Title))
				m_Title = task.GetTitle();

			if (attribs.Contains(UIExtension.TaskAttribute.Icon))
				m_HasIcon = (task.GetIcon().Length > 0);

			if (attribs.Contains(UIExtension.TaskAttribute.Flag))
				m_IsFlagged = task.IsFlagged();

			if (attribs.Contains(UIExtension.TaskAttribute.Color))
				m_TextColor = task.GetTextDrawingColor();

            if (attribs.Contains(UIExtension.TaskAttribute.DoneDate))
                m_IsDone = (task.IsDone() || task.IsGoodAsDone());

			m_IsParent = task.IsParent();

			return true;
		}
	}

	// ------------------------------------------------------------

	[System.ComponentModel.DesignerCategory("")]

	public class TdlMindMapControl : MindMapControl
	{
		public event EditTaskLabelEventHandler EditTaskLabel;

		// From Parent
		private Translator m_Trans;
		private UIExtension.TaskIcon m_TaskIcons;

		private UIExtension.SelectionRect m_SelectionRect;
		private Dictionary<UInt32, MindMapTaskItem> m_Items;

		private Boolean m_ShowParentAsFolder;
		private Boolean m_TaskColorIsBkgnd;
		private Boolean m_IgnoreMouseClick;
		private TreeNode m_PreviouslySelectedNode;
		private Timer m_EditTimer;

		// -------------------------------------------------------------------------

		public TdlMindMapControl(Translator trans, UIExtension.TaskIcon icons)
		{
			m_Trans = trans;
			m_TaskIcons = icons;

			m_SelectionRect = new UIExtension.SelectionRect();
			m_Items = new Dictionary<UInt32, MindMapTaskItem>();

			m_TaskColorIsBkgnd = false;
			m_IgnoreMouseClick = false;
			m_ShowParentAsFolder = false;

			m_EditTimer = new Timer();
			m_EditTimer.Interval = 500;
			m_EditTimer.Tick += new EventHandler(OnEditLabelTimer);
		}

		public void UpdateTasks(TaskList tasks,
								UIExtension.UpdateType type,
								HashSet<UIExtension.TaskAttribute> attribs)
		{
			switch (type)
			{
				case UIExtension.UpdateType.Edit:
				case UIExtension.UpdateType.New:
					UpdateTaskAttributes(tasks, attribs);
					break;

				case UIExtension.UpdateType.Delete:
				case UIExtension.UpdateType.All:
					RebuildTreeView(tasks);
					break;

				case UIExtension.UpdateType.Unknown:
					return;
			}
		}

		public Boolean SelectNodeWasPreviouslySelected
		{
			get { return (SelectedNode == m_PreviouslySelectedNode); }
		}

		public Boolean TaskColorIsBackground
		{
			get { return m_TaskColorIsBkgnd; }
			set
			{
				if (m_TaskColorIsBkgnd != value)
				{
					m_TaskColorIsBkgnd = value;
					Invalidate();
				}
			}
		}

		public Boolean ShowParentsAsFolders
		{
			get { return m_ShowParentAsFolder; }
			set
			{
				if (m_ShowParentAsFolder != value)
				{
					m_ShowParentAsFolder = value;
					Invalidate();
				}
			}
		}
		
		public UInt32 HitTest(Point screenPos)
		{
			var clientPos = PointToClient(screenPos);
			var node = HitTestPositions(clientPos);

			if (node != null)
				return UniqueID(node);
			
			// else
			return 0;
		}

		public Rectangle GetSelectedItemLabelRect()
		{
			var labelRect = base.GetSelectedItemLabelRect();

			labelRect.X += (GetExtraWidth(SelectedNode) + 2);

			return labelRect;
		}

		public Boolean CanMoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			if (FindNode(taskId) == null)
				return false;

			if (FindNode(destParentId) == null)
				return false;

			if ((destPrevSiblingId != 0) && (FindNode(destPrevSiblingId) == null))
				return false;

			return true;
		}

		public Boolean MoveTask(UInt32 taskId, UInt32 destParentId, UInt32 destPrevSiblingId)
		{
			BeginUpdate();

			var node = FindNode(taskId);
			var destParentNode = FindNode(destParentId);
			var destPrevSiblingNode = FindNode(destPrevSiblingId);

			if ((node == null) || (destParentNode == null))
				return false;

			var srcParentNode = node.Parent;
			int srcPos = srcParentNode.Nodes.IndexOf(node);

			srcParentNode.Nodes.RemoveAt(srcPos);

			int destPos = 0; // insert at top
			
			if (destPrevSiblingNode != null)
				destPos = (destParentNode.Nodes.IndexOf(destPrevSiblingNode) + 1);

			destParentNode.Nodes.Insert(destPos, node);
			SelectedNode = node;

			EndUpdate();
			EnsureItemVisible(Item(node));

			return true;
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
			TreeNode node = FindNode(taskID);

			switch (getTask)
			{
				case UIExtension.GetTask.GetNextTask:
					if (node.NextVisibleNode != null)
					{
						taskID = UniqueID(node.NextVisibleNode);
						return true;
					}
					else
					{

					}
					break;

				case UIExtension.GetTask.GetPrevTask:
					if (node.PrevVisibleNode != null)
					{
						taskID = UniqueID(node.PrevVisibleNode);
						return true;
					}
					break;

				case UIExtension.GetTask.GetNextTopLevelTask:
					{
						var topLevelParent = TopLevelParent(node);

						if ((topLevelParent != null) && (topLevelParent.NextNode != null))
						{
							taskID = UniqueID(topLevelParent.NextNode);
							return true;
						}
					}
					break;

				case UIExtension.GetTask.GetPrevTopLevelTask:
					{
						var topLevelParent = TopLevelParent(node);

						if ((topLevelParent != null) && (topLevelParent.PrevNode != null))
						{
							taskID = UniqueID(topLevelParent.PrevNode);
							return true;
						}
					}
					break;
			}

			// all else
			return false;
		}
		
		
		// Internal ------------------------------------------------------------

		protected TreeNode TopLevelParent(TreeNode node)
		{
			while ((node != null) && !IsRoot(node))
			{
				var parentNode = node.Parent;

				if (IsRoot(parentNode))
					return node;

				node = parentNode;
			}

			// else node was already null or root
			return null;
		}

		protected void UpdateTaskAttributes(TaskList tasks,
								HashSet<UIExtension.TaskAttribute> attribs)
		{
			var changedTaskIds = new HashSet<UInt32>();
            
			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, attribs, changedTaskIds))
				task = task.GetNextTask();

			if (attribs.Contains(UIExtension.TaskAttribute.Title))
			{
				foreach (var id in changedTaskIds)
					RefreshNodeLabel(id, false);
			}

			RecalculatePositions();
		}

		private bool ProcessTaskUpdate(Task task,
									   HashSet<UIExtension.TaskAttribute> attribs,
									   HashSet<UInt32> taskIds)
		{
			if (!task.IsValid())
				return false;

			MindMapTaskItem item;
			bool newTask = !m_Items.TryGetValue(task.GetID(), out item);

			if (newTask)
			{
				var parentId = task.GetParentTask().GetID();
				var parentNode = FindNode(parentId);

				AddTaskToTree(task, parentNode, true);
			}
			else if (item.ProcessTaskUpdate(task, attribs))
			{
				// Process children
				Task subtask = task.GetFirstSubtask();

				while (subtask.IsValid() && ProcessTaskUpdate(subtask, attribs, taskIds))
					subtask = subtask.GetNextTask();
			}
			else
			{
				return false;
			}

			taskIds.Add(task.GetID());

			return true;
		}

		protected void RebuildTreeView(TaskList tasks)
		{
			// Snapshot the expanded tasks so we can restore them afterwards
			var expandedIDs = GetExpandedItems(RootNode);

			// And the selection
			var selID = UniqueID(SelectedNode);
			
			Clear();

			if (tasks.GetTaskCount() == 0)
				return;

			BeginUpdate();

			var task = tasks.GetFirstTask();
			bool taskIsRoot = !task.GetNextTask().IsValid(); // no siblings

			TreeNode rootNode = null;

			if (taskIsRoot)
			{
				rootNode = AddRootNode(new MindMapTaskItem(task), task.GetID());

				// First Child
				AddTaskToTree(task.GetFirstSubtask(), rootNode);
			}
			else
			{
				// There is more than one 'root' task so create a real root parent
				rootNode = AddRootNode(new MindMapTaskItem(m_Trans.Translate("Root")));

				AddTaskToTree(task, rootNode);
			}

			// Restore expanded state
			if (expandedIDs != null)
				SetExpandedItems(expandedIDs);
			else
				rootNode.Expand();

			EndUpdate();
			SetSelectedNode(selID);
		}

		protected List<UInt32> GetExpandedItems(TreeNode node)
		{
			List<UInt32> expandedIDs = null;

			if ((node != null) && node.IsExpanded)
			{
				expandedIDs = new List<UInt32>();
				expandedIDs.Add(UniqueID(node));

				foreach (TreeNode child in node.Nodes)
				{
					var childIDs = GetExpandedItems(child);

					if (childIDs != null)
						expandedIDs.AddRange(childIDs);
				}
			}

			return expandedIDs;
		}

		protected void SetExpandedItems(List<UInt32> expandedNodes)
		{
			foreach (var id in expandedNodes)
			{
				var node = FindNode(id);

				if (node != null)
					node.Expand();
			}
		}

		protected override Boolean IsAcceptableDropTarget(Object draggedItemData, Object dropTargetItemData)
		{
			// TODO
			return true;
		}

		protected override Boolean DoDrop(MindMapDragEventArgs e)
		{
			base.DoDrop(e);

			return true; // We handled it
		}
		
		protected override void DrawNodeLabel(Graphics graphics, String label, Rectangle rect,
												NodeDrawState nodeState, NodeDrawPos nodePos, Object itemData)
		{
			Brush textColor = SystemBrushes.WindowText;
			Brush backColor = null;

            var taskItem = (itemData as MindMapTaskItem);
			Color taskColor = taskItem.TextColor;
			bool isSelected = (nodeState != NodeDrawState.None);

			// Draw Background
			if (!taskColor.IsEmpty)
			{
				if (m_TaskColorIsBkgnd && !isSelected && !taskItem.IsDone)
				{
					backColor = new SolidBrush(taskColor);
					textColor = new SolidBrush(ColorUtil.GetBestTextDrawingColor(taskColor));
				}
				else
				{
					if (nodeState != MindMapControl.NodeDrawState.None)
						taskColor = ColorUtil.DarkerDrawing(taskColor, 0.3f);

					textColor = new SolidBrush(taskColor);
				}
			}

			switch (nodeState)
			{
				case NodeDrawState.Selected:
					m_SelectionRect.Draw(graphics, rect.X, rect.Y, rect.Width, rect.Height, this.Focused);
					break;

				case NodeDrawState.DropTarget:
					m_SelectionRect.Draw(graphics, rect.X, rect.Y, rect.Width, rect.Height, false);
					break;

				case NodeDrawState.None:
					if (backColor != null)
						graphics.FillRectangle(backColor, rect);

					if (DebugMode())
						graphics.DrawRectangle(new Pen(Color.Green), rect);
					break;
			}

			// Draw icon
			if (TaskHasIcon(taskItem))
			{
				Rectangle iconRect = CalcIconRect(rect);

				if (m_TaskIcons.Get(taskItem.ID))
					m_TaskIcons.Draw(graphics, iconRect.X, iconRect.Y);

				int xOffset = (iconRect.Right - rect.Left);

				rect.X += xOffset;
				rect.Width -= xOffset;
			}
			
			// Draw Text
			var format = DefaultLabelFormat(nodePos, isSelected);

			graphics.DrawString(label, this.Font, textColor, rect, format);
		}

		protected Boolean NodeHasIcon(TreeNode node)
		{
			return TaskHasIcon(ItemData(node) as MindMapTaskItem);
		}

		protected Boolean TaskHasIcon(MindMapTaskItem taskItem)
		{
			return ((m_TaskIcons != null) &&
					(taskItem != null) &&
					(taskItem.HasIcon || (m_ShowParentAsFolder && taskItem.IsParent)));
		}

		protected override void DrawNodeConnection(Graphics graphics, Point ptFrom, Point ptTo)
		{
			int midX = ((ptFrom.X + ptTo.X) / 2);

			graphics.DrawBezier(new Pen(base.ConnectionColor), 
								ptFrom,
								new Point(midX, ptFrom.Y),
								new Point(midX, ptTo.Y),
								ptTo);
		}

		private Rectangle CalcIconRect(Rectangle labelRect)
		{
			return new Rectangle((labelRect.X + 2), (CentrePoint(labelRect).Y) - 8, 16, 16);
		}

		private new void Clear()
		{
			m_Items.Clear();

			base.Clear();
		}

		private bool AddTaskToTree(Task task, TreeNode parent, bool select = false)
		{
			if (!task.IsValid())
				return true; // not an error

			var taskID = task.GetID();
			var taskItem = new MindMapTaskItem(task);

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

			if (select)
				SelectedNode = node;

			return true;
		}

		protected override int GetExtraWidth(TreeNode node)
		{
			if (NodeHasIcon(node))
				return 16;

			// else
			return 0;
		}

		protected override int GetMinItemHeight()
		{
			return 18;
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			base.OnMouseClick(e);

			if (e.Button != MouseButtons.Left)
				return;

			if (m_IgnoreMouseClick)
			{
				m_IgnoreMouseClick = false;
				return;
			}

			TreeNode node = HitTestPositions(e.Location);

			if ((node != SelectedNode) || IsRoot(node))
				return;

			if (HitTestExpansionButton(node, e.Location))
				return;

			if (SelectNodeWasPreviouslySelected)
			{
				if (EditTaskLabel != null)
					m_EditTimer.Start();
			}
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			m_EditTimer.Stop();

			switch (e.Button)
			{
				case MouseButtons.Left:
					{
						// Cache the previous selected item
						m_PreviouslySelectedNode = SelectedNode;

						TreeNode hit = HitTestPositions(e.Location);

						if (hit != null)
							m_IgnoreMouseClick = HitTestExpansionButton(hit, e.Location);
						else
							m_IgnoreMouseClick = false;
					}
					break;

				case MouseButtons.Right:
					{
						TreeNode hit = HitTestPositions(e.Location);

						if (hit != null)
							SelectedNode = hit;
					}
					break;
			}

			base.OnMouseDown(e);
		}

		protected void OnEditLabelTimer(object sender, EventArgs e)
		{
			m_EditTimer.Stop();

			if (EditTaskLabel != null)
				EditTaskLabel(this, UniqueID(SelectedNode));
		}
	}
}

