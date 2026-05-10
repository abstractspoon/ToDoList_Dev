using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerControl : UserControl, IDragRenderer, ILabelTipHandler
	{
		const int SplitWidth = 6;

		// ---------------------------------------------

		// From Parent
		private Translator m_Trans;
		private TaskItems m_Tasks;
		private LabelTip m_LabelTip;

		private bool m_DraggingHorzSplitBar, m_DraggingVertSplitBar;

		private DragImage m_DragImage;
		private Point m_SplitPos = new Point(50, 50); // 0-100

		private List<EisenhowerPane> m_Panes;

		// ---------------------------------------------

		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;
		public event TaskSelectionEventHandler SelectionChange;

		// ---------------------------------------------

		public EisenhowerControl() // For designer
		{
			InitializeComponent();

			m_Panes = new List<EisenhowerPane>() { m_TopLeftPane, m_TopRightPane, m_BotLeftPane, m_BotRightPane };
		}

		public void Initialize(Translator trans, UIExtension.TaskIcon icons)
		{
			m_Trans = trans;

			m_Tasks = new TaskItems();
			m_DragImage = new DragImage();
			m_LabelTip = new LabelTip(this);

			// Top-left => High Attrib1 - High Attrib2
			{
				// Dummy filter to get us started
				var filter = new EisenhowerPaneFilter()
				{
					Attribute1 = new EisenhowerPaneFilterAttribute(Task.Attribute.Priority)
					{
						MaxValue = 10,
						MaxValueOperator = EisenhowerPaneFilterOperator.LTEQ,
						MinValue = 5,
						MinValueOperator = EisenhowerPaneFilterOperator.GTEQ
					},

					Attribute2 = new EisenhowerPaneFilterAttribute(Task.Attribute.Risk)
					{
						MaxValue = 10,
						MaxValueOperator = EisenhowerPaneFilterOperator.LTEQ,
						MinValue = 5,
						MinValueOperator = EisenhowerPaneFilterOperator.GTEQ
					}
				};

				m_TopLeftPane.Initialize(m_Trans, m_Tasks, icons, Properties.Resources.TopLeftPane, filter);
			}

			// Top-right => Low Attrib1 - High Attrib2
			{
				// Dummy filter to get us started
				var filter = new EisenhowerPaneFilter()
				{
					Attribute1 = new EisenhowerPaneFilterAttribute(Task.Attribute.Priority)
					{
						MaxValue = 5,
						MaxValueOperator = EisenhowerPaneFilterOperator.LT,
						MinValue = -2,
						MinValueOperator = EisenhowerPaneFilterOperator.GTEQ
					},

					Attribute2 = new EisenhowerPaneFilterAttribute(Task.Attribute.Risk)
					{
						MaxValue = 10,
						MaxValueOperator = EisenhowerPaneFilterOperator.LTEQ,
						MinValue = 5,
						MinValueOperator = EisenhowerPaneFilterOperator.GTEQ
					}
				};

				m_TopRightPane.Initialize(m_Trans, m_Tasks, icons, Properties.Resources.TopRightPane, filter);
			}

			// Bottom-left => High Attrib1 - Low Attrib2
			{
				// Dummy filter to get us started
				var filter = new EisenhowerPaneFilter()
				{
					Attribute1 = new EisenhowerPaneFilterAttribute(Task.Attribute.Priority)
					{
						MaxValue = 10,
						MaxValueOperator = EisenhowerPaneFilterOperator.LTEQ,
						MinValue = 5,
						MinValueOperator = EisenhowerPaneFilterOperator.GTEQ
					},

					Attribute2 = new EisenhowerPaneFilterAttribute(Task.Attribute.Risk)
					{
						MaxValue = 5,
						MaxValueOperator = EisenhowerPaneFilterOperator.LT,
						MinValue = -2,
						MinValueOperator = EisenhowerPaneFilterOperator.GTEQ
					}
				};

				m_BotLeftPane.Initialize(m_Trans, m_Tasks, icons, Properties.Resources.BotLeftPane, filter);
			}

			// Bottom-right => Low Attrib1 - Low Attrib2
			{
				// Dummy filter to get us started
				var filter = new EisenhowerPaneFilter()
				{
					Attribute1 = new EisenhowerPaneFilterAttribute(Task.Attribute.Priority)
					{
						MaxValue = 5,
						MaxValueOperator = EisenhowerPaneFilterOperator.LT,
						MinValue = -2,
						MinValueOperator = EisenhowerPaneFilterOperator.GTEQ
					},

					Attribute2 = new EisenhowerPaneFilterAttribute(Task.Attribute.Risk)
					{
						MaxValue = 5,
						MaxValueOperator = EisenhowerPaneFilterOperator.LT,
						MinValue = -2,
						MinValueOperator = EisenhowerPaneFilterOperator.GTEQ
					}
				};

				m_BotRightPane.Initialize(m_Trans, m_Tasks, icons, Properties.Resources.BotRightPane, filter);
			}

			m_Panes.ForEach(p => 
			{
				p.EditTaskDone += new EditTaskCompletionEventHandler(OnPaneEditTaskDone);
				p.EditTaskIcon += new EditTaskIconEventHandler(OnPaneEditTaskIcon);
				p.EditTaskLabel += new EditTaskLabelEventHandler(OnPaneEditTaskLabel);
				p.SelectionChange += new TaskSelectionEventHandler(OnPaneSelectionChange);
			});
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark);

			m_Panes.ForEach(p => p.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight));
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileInt(key, "XSplit", m_SplitPos.X);
			prefs.WriteProfileInt(key, "YSplit", m_SplitPos.Y);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			ShowParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
			ShowCompletionCheckboxes = prefs.GetProfileBool("Preferences", "AllowCheckboxAgainstTreeItem", false);
			ShowMixedCompletionState = prefs.GetProfileBool("Preferences", "ShowMixedCompletionState", true);

			// Alternate line color
			int color = -1;

			if (prefs.GetProfileBool("Preferences", "AlternateLineColor", true))
				color = prefs.GetProfileInt("Preferences\\Colors", "AlternateLines", -1);

			AlternateLineColor = DrawingColor.ToColor((UInt32)color);

			// Grid color
			color = -1;

			if (prefs.GetProfileBool("Preferences", "SpecifyGridColor", true))
				color = prefs.GetProfileInt("Preferences\\Colors", "GridLines", -1);

			GridlineColor = DrawingColor.ToColor((UInt32)color);

			if (!appOnly)
			{
				m_SplitPos.X = prefs.GetProfileInt(key, "XSplit", 50);
				m_SplitPos.Y = prefs.GetProfileInt(key, "YSplit", 50);
			}
		}

		public void EnsureSelectionVisible()
		{
			// TODO
		}

		public bool SelectTask(UInt32 taskID)
		{
			foreach (var p in m_Panes)
			{
				if (p.SelectTask(taskID))
					return true;
			}

			return false;
		}

		public bool SelectTasks(uint[] taskIDs)
		{
			foreach (var p in m_Panes)
			{
				if (p.SelectTasks(taskIDs))
					return true;
			}

			return false;
		}

		public bool HasSelection
		{
			get
			{
				foreach (var p in m_Panes)
				{
					if (p.HasSelection)
						return true;
				}

				return false;
			}
		}

		public bool ReadOnly { get; set; }

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			switch (type)
			{
			case UIExtension.UpdateType.All:
			case UIExtension.UpdateType.Edit:
			case UIExtension.UpdateType.New:
				UpdateTaskAttributes(tasks);
				break;

			case UIExtension.UpdateType.Delete:
				// TODO
				break;
			}
		}

		public bool TaskColorIsBackground		{ set { m_Panes.ForEach(p => p.TaskColorIsBackground = value); } }
		public bool ShowMixedCompletionState	{ set { m_Panes.ForEach(p => p.ShowMixedCompletionState = value); } }
		public bool ShowParentsAsFolders		{ set {	m_Panes.ForEach(p => p.ShowParentsAsFolders = value); } }
		public bool ShowCompletionCheckboxes	{ set {	m_Panes.ForEach(p => p.ShowCompletionCheckboxes = value); } }

		public Color AlternateLineColor			{ set { m_Panes.ForEach(p => p.AlternateLineColor = value); } }
		public Color GridlineColor				{ set { m_Panes.ForEach(p => p.GridlineColor = value); } }

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			switch (attrib)
			{
			// Note: lock state is always provided
			case Task.Attribute.Title:
			case Task.Attribute.Icon:
			case Task.Attribute.Flag:
			case Task.Attribute.Color:
			case Task.Attribute.DoneDate:
			case Task.Attribute.Position:
			case Task.Attribute.SubtaskDone:
			case Task.Attribute.CustomAttribute:
			case Task.Attribute.Priority:
			case Task.Attribute.Risk:
				return true;
			}

			// all else
			return false;
		}

		public UInt32 HitTestTask(Point screenPos)
		{
			// var clientPos = PointToClient(screenPos);
			// var node = HitTestPositions(clientPos);
			// 
			// if (node != null)
			// 	return UniqueID(node);

			// else
			return 0;
		}

		public Rectangle GetSelectedItemLabelRect()
		{
			// EnsureItemVisible(SelectedItem);
			// return GetItemLabelRect(SelectedNode);
			return Rectangle.Empty;
		}

		public Rectangle GetItemLabelRect(TreeNode node)
		{
			// var labelRect = base.GetItemLabelRect(node);
			// 
			// labelRect.X -= LabelPadding;
			// labelRect.X += GetExtraWidth(node);
			// 
			// // Make sure the rect is big enough for the unscaled font
			// labelRect.Height = Math.Max(labelRect.Height, (this.Font.Height + (2 * LabelPadding))); 
			// 
			// return labelRect;
			return Rectangle.Empty;
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			// if ((text == String.Empty) || IsEmpty())
			//     return false;
			// 
			// TreeNode node = null; // start node
			// bool forward = true;
			// 
			// switch (selectTask)
			//    {
			//    case UIExtension.SelectTask.SelectFirstTask:
			// 	node = RootNode.Nodes[0];
			//        break;
			// 
			//    case UIExtension.SelectTask.SelectNextTask:
			// 	node = TreeCtrl.GetNextItem(SelectedNode, false); // no wrap
			//        break;
			// 
			//    case UIExtension.SelectTask.SelectNextTaskInclCurrent:
			// 	node = SelectedNode;
			// 	break;
			// 
			//    case UIExtension.SelectTask.SelectPrevTask:
			// 	node = TreeCtrl.GetPrevItem(SelectedNode, false); // no wrap
			// 
			// 	if ((node == null) || ((node == RootNode) && !NodeIsTask(RootNode)))
			// 		node = LastNode;
			// 
			// 	forward = false;
			// 	break;
			// 
			//    case UIExtension.SelectTask.SelectLastTask:
			// 	node = LastNode;
			// 	forward = false;
			// 	break;
			//    }
			// 
			// // Avoid recursion
			// while (node != null)
			// { 
			// 	if (StringUtil.Find(node.Text, text, caseSensitive, wholeWord))
			// 	{
			// 		SelectedNode = node;
			// 		return true;
			// 	}
			// 
			// 	if (forward)
			// 		node = TreeCtrl.GetNextItem(node, false); // no wrap
			// 	else
			// 		node = TreeCtrl.GetPrevItem(node, false); // no wrap
			// }

			return false;
		}

		protected bool SelectNextTask(string startingWith)
		{
			// if (IsEmpty())
			// 	return false;
			// 
			// TreeNode next = TreeCtrl.GetNextVisibleItem(SelectedNode, true); // wrap
			// 
			// while ((next != null) && (next != SelectedNode))
			// {
			// 	if (TaskItem(next).ID == 0)
			// 	{
			// 		// Skip root node
			// 	}
			// 	else if (next.Text.StartsWith(startingWith, StringComparison.InvariantCultureIgnoreCase))
			// 	{
			// 		SelectedNode = next;
			// 		return true;
			// 	}
			// 
			// 	next = TreeCtrl.GetNextVisibleItem(next, true); // wrap
			// }

			return false;
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
			// TreeNode node = FindNode(taskID);
			// 
			// if (node == null)
			// 	return false;
			// 
			// switch (getTask)
			// {
			// 	case UIExtension.GetTask.GetNextTask:
			// 		if (node.NextNode != null)
			// 		{
			// 			taskID = UniqueID(node.NextNode);
			// 			return true;
			// 		}
			// 		break;
			// 
			// 	case UIExtension.GetTask.GetPrevTask:
			// 		if (node.PrevNode != null)
			// 		{
			// 			taskID = UniqueID(node.PrevNode);
			// 			return true;
			// 		}
			// 		break;
			// 
			// 	case UIExtension.GetTask.GetNextVisibleTask:
			// 		if (node.NextVisibleNode != null)
			// 		{
			// 			taskID = UniqueID(node.NextVisibleNode);
			// 			return true;
			// 		}
			// 		break;
			// 
			// 	case UIExtension.GetTask.GetPrevVisibleTask:
			// 		if (node.PrevVisibleNode != null)
			// 		{
			// 			taskID = UniqueID(node.PrevVisibleNode);
			// 			return true;
			// 		}
			// 		break;
			// 
			// 	case UIExtension.GetTask.GetNextTopLevelTask:
			// 		{
			// 			var topLevelParent = TopLevelParent(node);
			// 
			// 			if ((topLevelParent != null) && (topLevelParent.NextNode != null))
			// 			{
			// 				taskID = UniqueID(topLevelParent.NextNode);
			// 				return true;
			// 			}
			// 		}
			// 		break;
			// 
			// 	case UIExtension.GetTask.GetPrevTopLevelTask:
			// 		{
			// 			var topLevelParent = TopLevelParent(node);
			// 
			// 			if ((topLevelParent != null) && (topLevelParent.PrevNode != null))
			// 			{
			// 				taskID = UniqueID(topLevelParent.PrevNode);
			// 				return true;
			// 			}
			// 		}
			// 		break;
			// }

			// all else
			return false;
		}

		public bool CanSaveToImage()
		{
			return (m_Tasks.Count != 0);
		}

		public Bitmap SaveToImage()
		{
			// TODO
			return null;
		}

		public bool DoIdleProcessing()
		{
			// TODO
			return false;
		}
		
		// Message handlers --------------------------------

		private bool OnPaneEditTaskDone(object sender, UInt32 taskId, bool completed)
		{
			return (bool)EditTaskDone?.Invoke(sender, taskId, completed);
		}

		private bool OnPaneEditTaskIcon(object sender, UInt32 taskId)
		{
			return (bool)EditTaskIcon?.Invoke(sender, taskId);
		}

		private bool OnPaneEditTaskLabel(object sender, UInt32 taskId)
		{
			return (bool)EditTaskLabel?.Invoke(sender, taskId);
		}

		private void OnPaneSelectionChange(object sender, UInt32 taskId)
		{
			SelectionChange?.Invoke(sender, taskId);
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			RecalcPaneRects();
		}

		protected void RecalcPaneRects()
		{
			Rectangle hSplitRect = GetHorzSplitBarRect();
			Rectangle vSplitRect = GetVertSplitBarRect();

			// Top-left
			Rectangle topLeftRect = ClientRectangle;

			topLeftRect.Width = vSplitRect.Left;
			topLeftRect.Height = hSplitRect.Top;

			m_TopLeftPane.Bounds = topLeftRect;

			// Top-right
			Rectangle topRightRect = ClientRectangle;

			topRightRect.X = vSplitRect.Right;
			topRightRect.Y = topLeftRect.Y;
			topRightRect.Width -= topRightRect.X;
			topRightRect.Height = topLeftRect.Height;

			m_TopRightPane.Bounds = topRightRect;
			
			// Bottom-left
			Rectangle botLeftRect = ClientRectangle;

			botLeftRect.Y = hSplitRect.Bottom;
			botLeftRect.Width = topLeftRect.Width;
			botLeftRect.Height = (Height - botLeftRect.Y);

			m_BotLeftPane.Bounds = botLeftRect;

			// Bottom-right
			Rectangle botRightRect = ClientRectangle;

			botRightRect.X = topRightRect.X;
			botRightRect.Y = botLeftRect.Y;
			botRightRect.Width = topRightRect.Width;
			botRightRect.Height = botLeftRect.Height;

			m_BotRightPane.Bounds = botRightRect;

			Update();
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			base.OnMouseDown(e);

			if (e.Button == MouseButtons.Left)
			{
				m_DraggingHorzSplitBar = GetHorzSplitBarRect().Contains(e.Location);
				m_DraggingVertSplitBar = GetVertSplitBarRect().Contains(e.Location);

				Capture = (m_DraggingHorzSplitBar || m_DraggingVertSplitBar);
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			base.OnMouseUp(e);

			Capture = false;
			m_DraggingHorzSplitBar = m_DraggingVertSplitBar = false;
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);

			if (m_DraggingHorzSplitBar || m_DraggingVertSplitBar)
			{
				if (m_DraggingHorzSplitBar)
					m_SplitPos.Y = Math.Max(0, Math.Min(100, ((e.Y * 100) / Height)));

				if (m_DraggingVertSplitBar)
					m_SplitPos.X = Math.Max(0, Math.Min(100, ((e.X * 100) / Width)));

				RecalcPaneRects();
			}
			else // Set split cursor
			{
				bool inHorzSplit = GetHorzSplitBarRect().Contains(e.Location);
				bool inVertSplit = GetVertSplitBarRect().Contains(e.Location);

				if (inHorzSplit && inVertSplit)
				{
					Cursor = Cursors.SizeAll;
				}
				else if (inHorzSplit)
				{
					Cursor = Cursors.SizeNS;
				}
				else if (inVertSplit)
				{
					Cursor = Cursors.SizeWE;
				}
				else
				{
					Cursor = Cursors.Default;
				}
			}
		}

		protected Rectangle GetVertSplitBarRect()
		{
			Rectangle rect = ClientRectangle;

			rect.X = (((m_SplitPos.X * Width) / 100) - (SplitWidth / 2));
			rect.Width = SplitWidth;

			return rect;
		}

		protected Rectangle GetHorzSplitBarRect()
		{
			Rectangle rect = ClientRectangle;

			rect.Y = (((m_SplitPos.Y * Height) / 100) - (SplitWidth / 2));
			rect.Height = SplitWidth;

			return rect;
		}

				public void SetFont(String fontName, int fontSize)
		{
			// TODO
		}

		// ILabelTipHandler implementation
		public Control GetOwner()
		{
			return this;
		}

		public LabelTipInfo ToolHitTest(Point ptScreen)
		{
// 			var pt = PointToClient(ptScreen);
// 			var hit = HitTestPositions(pt);
// 
// 			if ((hit == null) || IsRoot(hit))
// 				return null;
// 
// 			var labelRect = GetItemLabelRect(hit);
// 
// 			if (!labelRect.Contains(pt))
// 				return null;
// 
// 			if (ClientRectangle.Contains(labelRect))
// 				return null;
// 
// 			labelRect.Offset(-1, -1);
// 
// 			return new LabelTipInfo()
// 			{
// 				Id = UniqueID(hit),
// 				Text = hit.Text,
// 				MultiLine = false,
// 				Rect = labelRect,
// 				Font = GetNodeTooltipFont(hit),
// 			};
			return null;
		}

		protected override void WndProc(ref Message m)
		{
			if (m_LabelTip != null)
				m_LabelTip.ProcessMessage(m);

			// When 'show window contents while dragging' is DISABLED
			// WinForms also disables 'live' scrollbar thumb dragging
			// and only performs the scroll when the thumb is released.
			FormsUtil.FixThumbScrolling(ref m);

			base.WndProc(ref m);
		}

		private void UpdateTaskAttributes(TaskList tasks)
		{
			var changedTaskIds = new HashSet<UInt32>();
			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, changedTaskIds))
				task = task.GetNextTask();

			// Only Update the panes if the attributes we are tracking were changed
			// TODO
			if (changedTaskIds.Count > 0)
			{
				m_TopLeftPane.RefreshList();
				m_TopRightPane.RefreshList();
				m_BotLeftPane.RefreshList();
				m_BotRightPane.RefreshList();
			}
		}

		private bool ProcessTaskUpdate(Task task, HashSet<UInt32> taskIds)
		{
			if (!task.IsValid())
				return false;

            UInt32 taskId = task.GetID();
			TaskItem item = m_Tasks.GetItem(taskId, true);

			if (!item.ProcessTaskUpdate(task))
				return false;

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, taskIds)) // RECURSIVE CALL
				subtask = subtask.GetNextTask();

			taskIds.Add(task.GetID());

			return true;
		}

/*
		protected override bool IsAcceptableDropTarget(Object draggedItemData, Object dropTargetItemData, DropPos dropPos, bool copy)
		{
// 			if (dropPos == EisenhowerControl.DropPos.On)
// 				return !TaskItem(dropTargetItemData).IsLocked;

			// else
			return true;
		}

		protected override bool IsAcceptableDragSource(Object itemData)
		{
			return !TaskItem(itemData).IsLocked;
		}

		protected override bool DoDrop(EisenhowerDragEventArgs e)
		{
			TreeNode prevParentNode = e.dragged.node.Parent;

			if (!base.DoDrop(e) || e.copyItem)
				return false;

			if (e.targetParent.node != prevParentNode)
			{
                // Fixup parent states
                // Note: our tree nodes haven't been moved yet but 
                // the application will have updated it's data structures
                // so we have to account for this in the node count passed
                FixupParentalStatus(e.targetParent.node, 1);
                FixupParentalStatus(prevParentNode, -1);

                FixupParentID(e.dragged.node, e.targetParent.node);
			}

			return true;
		}
*/

/*
		protected override void OnMouseClick(MouseEventArgs e)
		{
			base.OnMouseClick(e);

			if (e.Button != MouseButtons.Left)
				return;

// 			if (m_IgnoreMouseClick)
// 			{
// 				m_IgnoreMouseClick = false;
// 				return;
// 			}
// 
// 			TreeNode node = HitTestPositions(e.Location);
// 
// 			if ((node == null) || (node != SelectedNode) || !NodeIsTask(node))
// 				return;
// 
// 			if (HitTestExpansionButton(node, e.Location))
// 				return;
// 
//             var taskItem = RealTaskItem(node);
// 
//             if (!ReadOnly && !taskItem.IsLocked)
//             {
//                 if (HitTestCheckbox(node, e.Location))
//                 {
// 					if (EditTaskDone != null)
// 						EditTaskDone(this, taskItem.ID, !taskItem.IsDone(false));
// 				}
//                 else if (HitTestIcon(node, e.Location))
//                 {
//                     // Performing icon editing from a 'MouseUp' or 'MouseClick' event 
//                     // causes the edit icon dialog to fail to correctly get focus but
//                     // counter-intuitively it works from 'MouseDown'
//                     //if (EditTaskIcon != null)
//                     //    EditTaskIcon(this, UniqueID(SelectedNode));
//                 }
//                 else if (SelectNodeWasPreviouslySelected)
// 			    {
// 			        if (EditTaskLabel != null)
// 				        m_EditTimer.Start();
//                 }
//             }
		}

		private bool HitTestCheckbox(TreeNode node, Point point)
        {
//          if (!m_ShowCompletionCheckboxes)
//              return false;
// 
//          return CalcCheckboxRect(base.GetItemLabelRect(node)).Contains(point);
			return false;
		}

		private bool HitTestIcon(TreeNode node, Point point)
        {
// 			var taskItem = RealTaskItem(node);
// 
// 			if (taskItem.IsLocked || !TaskHasIcon(taskItem))
// 				return false;
// 
// 			// else
// 			return CalcIconRect(base.GetItemLabelRect(node)).Contains(point);
			return false;
        }

*/
// 		private void OnEditLabelTimer(object sender, EventArgs e)
// 		{
// 			m_EditTimer.Stop();
// 
// 			if (EditTaskLabel != null)
// 				EditTaskLabel(this, UniqueID(SelectedNode));
// 		}

// 		protected override void OnMouseMove(MouseEventArgs e)
// 		{
//  			base.OnMouseMove(e);
// 
// 			var node = HitTestPositions(e.Location);
// 
// 			if (!ReadOnly && (node != null) && !HitTestExpansionButton(node, e.Location))
// 			{
// 				var taskItem = RealTaskItem(node);
// 
// 				if (taskItem != null)
//                 {
//                     Cursor cursor = null;
// 
//                     if (taskItem.IsLocked)
//                     {
//                         cursor = UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);
//                     }
//                     else if (TaskHasIcon(taskItem) && HitTestIcon(node, e.Location))
//                     {
//                         cursor = UIExtension.HandCursor();
//                     }
//                     
//                     if (cursor != null)
//                     {
//                         Cursor = cursor;
//                         return;
//                     }
// 				}
// 			}
// 
// 			// all else
// 			Cursor = Cursors.Arrow;
// 		}

		protected override void OnDragOver(DragEventArgs e)
		{
			m_DragImage.ShowNoLock(false);

			base.OnDragOver(e);

			m_DragImage.ShowNoLock(true);
			m_DragImage.Move(e.X, e.Y);
		}

		protected override void OnDragEnter(DragEventArgs e)
		{
			base.OnDragEnter(e);

// 			var rect = GetItemLabelRect(SelectedNode);
// 			rect.Offset(-rect.Left, -rect.Top);
// 
// 			m_DragImage.Begin(Handle, 
// 								this,
// 								SelectedNode, 
// 								rect.Width, 
// 								rect.Height, 
// 								rect.Width, 
// 								rect.Height);
		}


		public void DrawDragImage(Graphics graphics, Object obj, int width, int height)
		{
			var node = (obj as TreeNode);

// 			DrawItemLabel(graphics,
// 							TaskItem(node), 
// 							new Rectangle(0, 0, width, height),
// 							NodeDrawState.Selected,
// 							NodeDrawPos.Root,
// 							GetNodeTitleFont(node),
// 							true); // drag image
		}

		protected override void OnDragDrop(DragEventArgs e)
		{
			m_DragImage.End();

			base.OnDragDrop(e);
		}

		protected override void OnDragLeave(EventArgs e)
		{
			m_DragImage.End();

			base.OnDragLeave(e);
		}

	}
}
