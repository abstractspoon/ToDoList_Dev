using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerControl : UserControl, IDragRenderer, ILabelTipHandler
	{
		public delegate bool EditTaskLabelEventHandler(object sender, UInt32 taskId);
		public delegate bool EditTaskIconEventHandler(object sender, UInt32 taskId);
		public delegate bool EditTaskCompletionEventHandler(object sender, UInt32 taskId, bool completed);

		// ---------------------------------------------

		const int SplitWidth = 6;

		// ---------------------------------------------

		// From Parent
		private Translator m_Trans;
		private UIExtension.TaskIcon m_TaskIcons;
		private TaskItems m_Items = new TaskItems();

		private bool m_ShowParentAsFolder;
		private bool m_TaskColorIsBkgnd;
		private bool m_ShowMixedCompletionState;
		private bool m_ShowCompletionCheckboxes;
		private bool m_DraggingHorzSplitBar, m_DraggingVertSplitBar;

		private DragImage m_DragImage;
		private LabelTip m_LabelTip;
		private Point m_SplitPos = new Point(50, 50); // 0-100

		// ---------------------------------------------

		public EisenhowerControl() // For designer
		{
			InitializeComponent();
		}

		public EisenhowerControl(Translator trans, UIExtension.TaskIcon icons) : this()
		{
			m_Trans = trans;
			m_TaskIcons = icons;

			m_Items = new TaskItems();
			m_DragImage = new DragImage();
			m_LabelTip = new LabelTip(this);
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark);

			m_TopLeftPane.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_TopRightPane.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_BotLeftPane.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_BotRightPane.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
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

			if (!appOnly)
			{
				m_SplitPos.X = prefs.GetProfileInt(key, "XSplit", 50);
				m_SplitPos.Y = prefs.GetProfileInt(key, "YSplit", 50);
			}
		}

		public void EnsureSelectionVisible()
		{

		}

		public bool HasSelection
		{
			get { return false; }
		}

		public bool ReadOnly { get; set; }

		// Message handlers --------------------------------

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

		public bool TaskColorIsBackground
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

		public bool ShowMixedCompletionState
		{
			get { return m_ShowMixedCompletionState; }
			set
			{
				if (m_ShowMixedCompletionState != value)
				{
					m_ShowMixedCompletionState = value;
					Invalidate();
				}
			}
		}

		public bool ShowParentsAsFolders
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

        public bool ShowCompletionCheckboxes
        {
            get { return m_ShowCompletionCheckboxes; }
            set
            {
                if (m_ShowCompletionCheckboxes != value)
                {
                    m_ShowCompletionCheckboxes = value;
					Invalidate();
                }
            }
        }

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
// 			var clientPos = PointToClient(screenPos);
// 			var node = HitTestPositions(clientPos);
// 
// 			if (node != null)
// 				return UniqueID(node);
			
			// else
			return 0;
		}

		public Rectangle GetSelectedItemLabelRect()
		{
// 			EnsureItemVisible(SelectedItem);
// 			return GetItemLabelRect(SelectedNode);
			return Rectangle.Empty;
		}

		public Rectangle GetItemLabelRect(TreeNode node)
		{
// 			var labelRect = base.GetItemLabelRect(node);
// 
// 			labelRect.X -= LabelPadding;
// 			labelRect.X += GetExtraWidth(node);
// 
// 			// Make sure the rect is big enough for the unscaled font
// 			labelRect.Height = Math.Max(labelRect.Height, (this.Font.Height + (2 * LabelPadding))); 
// 
// 			return labelRect;
			return Rectangle.Empty;
		}

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
        {
//          if ((text == String.Empty) || IsEmpty())
//              return false;
// 
// 			TreeNode node = null; // start node
// 			bool forward = true;
// 
// 			switch (selectTask)
//             {
//             case UIExtension.SelectTask.SelectFirstTask:
// 				node = RootNode.Nodes[0];
//                 break;
// 
//             case UIExtension.SelectTask.SelectNextTask:
// 				node = TreeCtrl.GetNextItem(SelectedNode, false); // no wrap
//                 break;
// 
//             case UIExtension.SelectTask.SelectNextTaskInclCurrent:
// 				node = SelectedNode;
// 				break;
// 
//             case UIExtension.SelectTask.SelectPrevTask:
// 				node = TreeCtrl.GetPrevItem(SelectedNode, false); // no wrap
// 
// 				if ((node == null) || ((node == RootNode) && !NodeIsTask(RootNode)))
// 					node = LastNode;
// 
// 				forward = false;
// 				break;
// 
//             case UIExtension.SelectTask.SelectLastTask:
// 				node = LastNode;
// 				forward = false;
// 				break;
//             }
// 			
// 			// Avoid recursion
// 			while (node != null)
// 			{ 
// 				if (StringUtil.Find(node.Text, text, caseSensitive, wholeWord))
// 				{
// 					SelectedNode = node;
// 					return true;
// 				}
// 
// 				if (forward)
// 					node = TreeCtrl.GetNextItem(node, false); // no wrap
// 				else
// 					node = TreeCtrl.GetPrevItem(node, false); // no wrap
// 			}

			return false;
		}

		protected bool SelectNextTask(string startingWith)
		{
// 			if (IsEmpty())
// 				return false;
// 
// 			TreeNode next = TreeCtrl.GetNextVisibleItem(SelectedNode, true); // wrap
// 
// 			while ((next != null) && (next != SelectedNode))
// 			{
// 				if (TaskItem(next).ID == 0)
// 				{
// 					// Skip root node
// 				}
// 				else if (next.Text.StartsWith(startingWith, StringComparison.InvariantCultureIgnoreCase))
// 				{
// 					SelectedNode = next;
// 					return true;
// 				}
// 
// 				next = TreeCtrl.GetNextVisibleItem(next, true); // wrap
// 			}

			return false;
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskID)
		{
// 			TreeNode node = FindNode(taskID);
// 
// 			if (node == null)
// 				return false;
// 
// 			switch (getTask)
// 			{
// 				case UIExtension.GetTask.GetNextTask:
// 					if (node.NextNode != null)
// 					{
// 						taskID = UniqueID(node.NextNode);
// 						return true;
// 					}
// 					break;
// 
// 				case UIExtension.GetTask.GetPrevTask:
// 					if (node.PrevNode != null)
// 					{
// 						taskID = UniqueID(node.PrevNode);
// 						return true;
// 					}
// 					break;
// 
// 				case UIExtension.GetTask.GetNextVisibleTask:
// 					if (node.NextVisibleNode != null)
// 					{
// 						taskID = UniqueID(node.NextVisibleNode);
// 						return true;
// 					}
// 					break;
// 
// 				case UIExtension.GetTask.GetPrevVisibleTask:
// 					if (node.PrevVisibleNode != null)
// 					{
// 						taskID = UniqueID(node.PrevVisibleNode);
// 						return true;
// 					}
// 					break;
// 
// 				case UIExtension.GetTask.GetNextTopLevelTask:
// 					{
// 						var topLevelParent = TopLevelParent(node);
// 
// 						if ((topLevelParent != null) && (topLevelParent.NextNode != null))
// 						{
// 							taskID = UniqueID(topLevelParent.NextNode);
// 							return true;
// 						}
// 					}
// 					break;
// 
// 				case UIExtension.GetTask.GetPrevTopLevelTask:
// 					{
// 						var topLevelParent = TopLevelParent(node);
// 
// 						if ((topLevelParent != null) && (topLevelParent.PrevNode != null))
// 						{
// 							taskID = UniqueID(topLevelParent.PrevNode);
// 							return true;
// 						}
// 					}
// 					break;
// 			}

			// all else
			return false;
		}

        public bool CanSaveToImage()
        {
            return (m_Items.Count != 0);
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

		private void UpdateTaskAttributes(TaskList tasks)
		{
			var changedTaskIds = new HashSet<UInt32>();
			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, changedTaskIds))
				task = task.GetNextTask();
		}

		private bool ProcessTaskUpdate(Task task, HashSet<UInt32> taskIds)
		{
			if (!task.IsValid())
				return false;

            UInt32 taskId = task.GetID();
			TaskItem item = m_Items.GetItem(taskId, true);

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

// 		protected override Color GetNodeBackgroundColor(Object itemData)
// 		{
// 			if (m_TaskColorIsBkgnd)
// 			{
// 				var taskItem = (itemData as TaskItem);
// 
// 				if (!taskItem.TextColor.IsEmpty)
// 					return taskItem.TextColor;
// 			}
// 
// 			// all else
// 			return base.GetNodeBackgroundColor(itemData);
// 		}

// 		protected void DrawItemLabel(Graphics graphics, TaskItem taskItem, Rectangle rect,
// 									  NodeDrawState nodeState, NodeDrawPos nodePos,
//                                       Font nodeFont, bool isDragImage)
// 		{
// 			var realItem = GetRealTaskItem(taskItem);
// 
// 			bool isSelected = (nodeState != NodeDrawState.None);
// 			bool showCheckBoxes = (m_ShowCompletionCheckboxes && !isDragImage);
// 
// 			Rectangle iconRect = Rectangle.Empty;
// 
//             if (taskItem.IsTask) // not root
//             {
//                 // Checkbox
//                 Rectangle checkRect = CalcCheckboxRect(rect);
// 
//                 if (showCheckBoxes)
// 				{
// 					if (!IsZoomed)
// 					{
// 						CheckBoxRenderer.DrawCheckBox(graphics, checkRect.Location, GetItemCheckboxState(realItem));
// 					}
// 					else
// 					{
// 						var tempImage = new Bitmap(m_CheckboxSize.Width, m_CheckboxSize.Height); // original size
// 
// 						using (var gTemp = Graphics.FromImage(tempImage))
// 						{
// 							CheckBoxRenderer.DrawCheckBox(gTemp, new Point(0, 0), GetItemCheckboxState(realItem));
// 							ImageUtils.DrawZoomedImage(tempImage, graphics, checkRect, rect);
// 						}
// 					}
// 				}
// 
// 				// Task icon
// 				if (TaskHasIcon(realItem))
//                 {
//                     iconRect = CalcIconRect(rect);
// 
//                     if (m_TaskIcons.Get(realItem.ID))
// 					{
// 						if (!IsZoomed)
// 						{
// 							m_TaskIcons.Draw(graphics, iconRect.X, iconRect.Y);
// 						}
// 						else
// 						{
// 							int imageSize = ScaleByDPIFactor(16);
// 							var tempImage = new Bitmap(imageSize, imageSize); // original size
// 
// 							using (var gTemp = Graphics.FromImage(tempImage))
// 							{
// 								gTemp.FillRectangle(SystemBrushes.Window, 0, 0, imageSize, imageSize);
// 								m_TaskIcons.Draw(gTemp, 0, 0);
// 
// 								ImageUtils.DrawZoomedImage(tempImage, graphics, iconRect, rect);
// 							}
// 						}
// 					}
// 
// 					rect.Width = (rect.Right - iconRect.Right - 2);
//                     rect.X = iconRect.Right + 2;
//                 }
//                 else if (showCheckBoxes)
//                 {
//                     rect.Width = (rect.Right - checkRect.Right - 2);
//                     rect.X = checkRect.Right + 2;
//                 }
//             }
// 
// 			// Text Colour
// 			Color textColor = SystemColors.WindowText;
// 
// 			if (isSelected)
// 			{
// 				if (SystemInformation.HighContrast)
// 					textColor = SystemColors.HighlightText;
// 				else
// 					textColor = UIExtension.SelectionRect.GetTextColor(UIExtension.SelectionRect.Style.Selected, taskItem.TextColor);
// 			}
// 			else if (!taskItem.TextColor.IsEmpty)
// 			{
// 				if (m_TaskColorIsBkgnd)
// 					textColor = DrawingColor.GetBestTextColor(taskItem.TextColor, true);
//                 else
//                     textColor = taskItem.TextColor;
//             }
// 
// 			switch (nodeState)
// 			{
// 				case NodeDrawState.Selected:
//                     UIExtension.SelectionRect.Draw(this.Handle, 
// 													graphics, 
// 													rect.X, 
// 													rect.Y, 
// 													rect.Width, 
// 													rect.Height, 
// 													false); // opaque
// 					break;
// 
// 				case NodeDrawState.DropTarget:
//                     UIExtension.SelectionRect.Draw(this.Handle, 
// 													graphics, 
// 													rect.X, 
// 													rect.Y, 
// 													rect.Width, 
// 													rect.Height,
// 													UIExtension.SelectionRect.Style.DropHighlighted,
// 													false); // opaque
// 					break;
// 			}
// 
// 			if (DebugMode())
//                graphics.DrawRectangle(new Pen(Color.Green), rect);
// 
// 			// Text
// 			var format = DefaultLabelFormat(nodePos, isSelected);
// 
//             graphics.DrawString(taskItem.Title, nodeFont, new SolidBrush(textColor), rect, format);
// 
// 			// Draw Windows shortcut icon if task is a reference
// 			if (taskItem.IsReference)
// 			{
// 				if (iconRect == Rectangle.Empty)
// 					iconRect = rect;
// 				else
// 					iconRect.Y = (rect.Bottom - iconRect.Height); // don't want shortcut icon centred vertically
// 
// 				UIExtension.ShortcutOverlay.Draw(graphics, iconRect.X, iconRect.Y, iconRect.Width, iconRect.Height);
// 			}
// 		}

        CheckBoxState GetItemCheckboxState(TaskItem taskItem)
        {
            if (taskItem.SomeSubtasksDone && ShowMixedCompletionState)
                return CheckBoxState.MixedNormal;

            // else
            return CheckBoxState.UncheckedNormal;
        }

		private bool TaskHasIcon(TaskItem taskItem)
		{
			if ((m_TaskIcons == null) || (taskItem == null))
				return false;

			return (taskItem.HasIcon || (m_ShowParentAsFolder && taskItem.IsParent));
		}

// 		protected override void OnMouseDoubleClick(MouseEventArgs e)
// 		{
// 			if (base.HandleMouseDoubleClick(e) || (EditTaskLabel == null))
// 				return;
// 
// 			if (HitTestPositions(e.Location) != null)
// 				EditTaskLabel(this, UniqueID(SelectedNode));
// 		}

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
