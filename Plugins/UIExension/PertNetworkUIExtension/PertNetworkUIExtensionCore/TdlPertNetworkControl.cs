using System;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace PertNetworkUIExtension
{
    public delegate bool EditTaskLabelEventHandler(object sender, uint taskId);
    public delegate bool EditTaskIconEventHandler(object sender, uint taskId);
    public delegate bool EditTaskCompletionEventHandler(object sender, uint taskId, bool completed);

	class PertNetworkTaskItem
	{
		// Data
		private String m_Title;
		private uint m_TaskID;
        private uint m_ParentID;
		private uint m_ReferenceId;
		private List<uint> m_LocalDepends;
		private Color m_TextColor;
		private bool m_HasIcon;
		private bool m_IsFlagged;
		private bool m_IsParent;
        private bool m_IsDone;
        private bool m_IsGoodAsDone;
        private bool m_SomeSubtasksDone;
		private bool m_IsLocked;

		// -----------------------------------------------------------------

		public PertNetworkTaskItem(String label)
		{
			m_Title = label;
			m_TaskID = 0;
            m_ParentID = 0;
			m_TextColor = new Color();
			m_HasIcon = false;
			m_IsFlagged = false;
			m_IsParent = false;
			m_IsDone = false;
            m_IsGoodAsDone = false;
            m_SomeSubtasksDone = false;
			m_IsLocked = false;
			m_ReferenceId = 0;
			m_LocalDepends = null;
		}

		public PertNetworkTaskItem(Task task)
		{
			m_Title = task.GetTitle();
			m_TaskID = task.GetID();
			m_ParentID = task.GetParentID();
			m_TextColor = task.GetTextDrawingColor();
			m_HasIcon = (task.GetIcon().Length > 0);
			m_IsFlagged = task.IsFlagged(false);
			m_IsParent = task.IsParent();
            m_IsDone = task.IsDone();
            m_IsGoodAsDone = task.IsGoodAsDone();
            m_SomeSubtasksDone = task.HasSomeSubtasksDone();
			m_IsLocked = task.IsLocked(true);
			m_ReferenceId = task.GetReferenceID();
			m_LocalDepends = task.GetLocalDependency();
		}

		public void FixupParentID(PertNetworkTaskItem parent)
		{
            m_ParentID = parent.ID;
		}

        public bool FixupParentalStatus(int nodeCount, UIExtension.TaskIcon taskIcons)
        {
            bool wasParent = m_IsParent;

            if (nodeCount == 0)
            {
                m_IsParent = (!m_HasIcon && taskIcons.Get(m_TaskID));
            }
            else
            {
                m_IsParent = true;
            }

            return (m_IsParent != wasParent);
        }

		public override string ToString() 
		{
			return Title;
		}

		public void Update(Task task, HashSet<Task.Attribute> attribs)
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

			set // only works for the root
			{
				if (!IsTask && !String.IsNullOrWhiteSpace(value))
					m_Title = value;
			}
		}
		
		public uint ID { get { return m_TaskID; } }
        public uint ParentID { get { return m_ParentID; } }
		public uint ReferenceId { get { return m_ReferenceId; } }
		public Color TextColor { get { return m_TextColor; } }
		public bool HasIcon { get { return m_HasIcon; } }
		public bool IsFlagged { get { return m_IsFlagged; } }
		public bool IsParent { get { return m_IsParent; } }
		public bool IsLocked { get { return m_IsLocked; } }
		public bool IsReference { get { return (m_ReferenceId != 0); } }
		public bool IsTask { get { return (m_TaskID != 0); } }
        public bool HasSomeSubtasksDone { get { return m_SomeSubtasksDone; } }
		public bool HasLocalDependencies {  get { return (m_LocalDepends != null) && (m_LocalDepends.Count > 0); } }
		public List<uint> LocalDependencies { get { return m_LocalDepends; } }

		public bool IsDone(bool includeGoodAsDone) 
        { 
            if (includeGoodAsDone && m_IsGoodAsDone)
                return true;

            return m_IsDone; 
        }

		public bool SetDone(bool done = true)
		{
			if (m_IsDone == done)
				return false;

			m_IsDone = done;
			return true;
		}

		public bool ProcessTaskUpdate(Task task)
		{
			if (task.GetID() != m_TaskID)
				return false;

			if (task.IsAttributeAvailable(Task.Attribute.Title))
				m_Title = task.GetTitle();

			if (task.IsAttributeAvailable(Task.Attribute.Icon))
				m_HasIcon = (task.GetIcon().Length > 0);

			if (task.IsAttributeAvailable(Task.Attribute.Flag))
				m_IsFlagged = task.IsFlagged(false);

			if (task.IsAttributeAvailable(Task.Attribute.Color))
				m_TextColor = task.GetTextDrawingColor();

            if (task.IsAttributeAvailable(Task.Attribute.SubtaskDone))
                m_SomeSubtasksDone = task.HasSomeSubtasksDone();

            if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
                m_IsDone = task.IsDone();

			if (task.IsAttributeAvailable(Task.Attribute.Dependency))
				m_LocalDepends = task.GetLocalDependency();

			m_IsParent = task.IsParent();
			m_IsLocked = task.IsLocked(true);
            m_IsGoodAsDone = task.IsGoodAsDone();
			m_ReferenceId = task.GetReferenceID();

			return true;
		}
	}

	// ------------------------------------------------------------

	[Flags]
	enum PertNetworkOption
	{
		None = 0x00,
		ShowDependencies = 0x01,
	}

	// ------------------------------------------------------------

	[System.ComponentModel.DesignerCategory("")]
	class TdlPertNetworkControl : PertNetworkControl
	{
		public event EditTaskLabelEventHandler      EditTaskLabel;
        public event EditTaskIconEventHandler       EditTaskIcon;
        public event EditTaskCompletionEventHandler EditTaskDone;

		// From Parent
		private Translator m_Trans;
		private UIExtension.TaskIcon m_TaskIcons;
		private Dictionary<uint, PertNetworkTaskItem> m_Items;

		private bool m_ShowParentAsFolder;
		private bool m_TaskColorIsBkgnd;
		private bool m_IgnoreMouseClick;
        private bool m_ShowCompletionCheckboxes;
		private bool m_StrikeThruDone;

		private TreeNode m_PreviouslySelectedNode;
		private Timer m_EditTimer;
        private Font m_BoldLabelFont, m_DoneLabelFont, m_BoldDoneLabelFont;
        private Size m_CheckboxSize;
		private Pen m_DependencyPen;
		private PertNetworkOption m_Options;

		// -------------------------------------------------------------------------

		public TdlPertNetworkControl(Translator trans, UIExtension.TaskIcon icons)
		{
			m_Trans = trans;
			m_TaskIcons = icons;

			m_Items = new Dictionary<uint, PertNetworkTaskItem>();

			m_DependencyPen = new Pen(Color.Black);
			m_DependencyPen.DashStyle = DashStyle.Dot;

			m_TaskColorIsBkgnd = false;
			m_IgnoreMouseClick = false;
			m_ShowParentAsFolder = false;
            m_ShowCompletionCheckboxes = true;
			m_StrikeThruDone = true;

			m_EditTimer = new Timer();
			m_EditTimer.Interval = 500;
			m_EditTimer.Tick += new EventHandler(OnEditLabelTimer);

            using (Graphics graphics = Graphics.FromHwnd(Handle))
                m_CheckboxSize = CheckBoxRenderer.GetGlyphSize(graphics, CheckBoxState.UncheckedNormal);
		}
        
        public void SetStrikeThruDone(bool strikeThruDone)
		{
			m_StrikeThruDone = strikeThruDone;

			if (m_BoldLabelFont != null)
				SetFont(m_BoldLabelFont.Name, (int)m_BoldLabelFont.Size, m_StrikeThruDone);
		}

		public new void SetFont(String fontName, int fontSize)
		{
			SetFont(fontName, fontSize, m_StrikeThruDone);
		}

		protected void SetFont(String fontName, int fontSize, bool strikeThruDone)
		{
			bool baseFontChange = ((m_BoldLabelFont == null) || (m_BoldLabelFont.Name != fontName) || (m_BoldLabelFont.Size != fontSize));
            bool doneFontChange = (baseFontChange || (m_BoldDoneLabelFont.Strikeout != strikeThruDone));

            if (baseFontChange)
                m_BoldLabelFont = new Font(fontName, fontSize, FontStyle.Bold);

            if (doneFontChange)
            {
                if (strikeThruDone)
                {
                    m_BoldDoneLabelFont = new Font(fontName, fontSize, FontStyle.Bold | FontStyle.Strikeout);
                    m_DoneLabelFont = new Font(fontName, fontSize, FontStyle.Strikeout);
                }
                else
                {
                    m_BoldDoneLabelFont = m_BoldLabelFont;
                    m_DoneLabelFont = null;
                }
            }

//             if ((baseFontChange || doneFontChange) && RefreshNodeFont(RootNode, true))
//                 RecalculatePositions();
            
            base.SetFont(fontName, fontSize);
        }

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			switch (type)
			{
				case UIExtension.UpdateType.Edit:
				case UIExtension.UpdateType.New:
				// TODO
				break;

				case UIExtension.UpdateType.Delete:
				case UIExtension.UpdateType.All:
				// TODO
				break;

				case UIExtension.UpdateType.Unknown:
					return;
			}
		}

		public PertNetworkOption Options
		{
			get { return m_Options; }

			set
			{
				if (value != m_Options)
				{
					m_Options = value;
					Invalidate();
				}
			}
		}

// 		public bool SelectNodeWasPreviouslySelected
// 		{
// 			get { return (SelectedNode == m_PreviouslySelectedNode); }
// 		}

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
                }
            }
        }

		protected float ImageZoomFactor
		{
			// Zoom images only half as much as text
			get { return (ZoomFactor + ((1.0f - ZoomFactor) / 2)); }
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
				case Task.Attribute.ProjectName:
				case Task.Attribute.Dependency:
					return true;
            }

            // all else
            return false;
        }
        		
		public new uint HitTest(Point screenPos)
		{
// 			var clientPos = PointToClient(screenPos);
// 			var node = HitTestPositions(clientPos);
// 
// 			if (node != null)
// 				return UniqueID(node);
			
			// else
			return 0;
		}

		public new Rectangle GetSelectedItemLabelRect()
		{
			EnsureItemVisible(SelectedItem);

			var labelRect = base.GetSelectedItemLabelRect();

// 			labelRect.X -= LabelPadding;
// 			labelRect.X += GetExtraWidth(SelectedNode);
// 
// 			// Make sure the rect is big enough for the unscaled font
// 			labelRect.Height = Math.Max(labelRect.Height, (this.Font.Height + (2 * LabelPadding))); 

			return labelRect;
		}

		public bool CanMoveTask(uint taskId, uint destParentId, uint destPrevSiblingId)
		{
// 			if (FindNode(taskId) == null)
// 				return false;
// 
// 			if (FindNode(destParentId) == null)
// 				return false;
// 
// 			if ((destPrevSiblingId != 0) && (FindNode(destPrevSiblingId) == null))
// 				return false;

			return true;
		}

		public bool MoveTask(uint taskId, uint destParentId, uint destPrevSiblingId)
		{
/*
			BeginUpdate();

            var node = FindNode(taskId);
   			var prevParentNode = node.Parent;

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

            FixupParentalStatus(destParentNode, 1);
            FixupParentalStatus(prevParentNode, -1);

            FixupParentID(node, destParentNode);

			SelectedNode = node;

			EndUpdate();
			EnsureItemVisible(Item(node));
*/

			return true;
		}

        public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
        {
            if ((text == String.Empty) || IsEmpty())
                return false;

/*
			TreeNode node = null; // start node
			bool forward = true;

			switch (selectTask)
            {
            case UIExtension.SelectTask.SelectFirstTask:
				node = RootNode.Nodes[0];
                break;

            case UIExtension.SelectTask.SelectNextTask:
				node = TreeCtrl.GetNextItem(SelectedNode);
                break;

            case UIExtension.SelectTask.SelectNextTaskInclCurrent:
				node = SelectedNode;
				break;

            case UIExtension.SelectTask.SelectPrevTask:
				node = TreeCtrl.GetPrevItem(SelectedNode);

				if ((node == null) || ((node == RootNode) && !NodeIsTask(RootNode)))
					node = LastNode;

				forward = false;
				break;

            case UIExtension.SelectTask.SelectLastTask:
				node = LastNode;
				forward = false;
				break;
            }
			
			// Avoid recursion
			while (node != null)
			{ 
				if (StringUtil.Find(node.Text, text, caseSensitive, wholeWord))
				{
					SelectedNode = node;
					return true;
				}

				if (forward)
					node = TreeCtrl.GetNextItem(node);
				else
					node = TreeCtrl.GetPrevItem(node);
			}
*/

			return false;
		}

		public bool GetTask(UIExtension.GetTask getTask, ref uint taskID)
		{
/*
			TreeNode node = FindNode(taskID);

			if (node == null)
				return false;

			switch (getTask)
			{
				case UIExtension.GetTask.GetNextTask:
					if (node.NextNode != null)
					{
						taskID = UniqueID(node.NextNode);
						return true;
					}
					break;

				case UIExtension.GetTask.GetPrevTask:
					if (node.PrevVisibleNode != null)
					{
						taskID = UniqueID(node.PrevNode);
						return true;
					}
					break;

				case UIExtension.GetTask.GetNextVisibleTask:
					if (node.NextVisibleNode != null)
					{
						taskID = UniqueID(node.NextVisibleNode);
						return true;
					}
					break;

				case UIExtension.GetTask.GetPrevVisibleTask:
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
*/

			// all else
			return false;
		}

        public bool CanSaveToImage()
        {
            return !IsEmpty();
        }
        		
        // Internal ------------------------------------------------------------

        override protected int ScaleByDPIFactor(int value)
        {
            return DPIScaling.Scale(value);
        }

		private PertNetworkTaskItem TaskItem(Object itemData)
		{
			if (itemData == null)
				return null;

			return (itemData as PertNetworkTaskItem);
		}

		private void UpdateTaskAttributes(TaskList tasks)
		{
/*
			var rootItem = TaskItem(RootNode);

			// A root item must already exist
			if (rootItem == null)
				return;

			var changedTaskIds = new HashSet<uint>();
			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, changedTaskIds))
				task = task.GetNextTask();

			if (tasks.IsAttributeAvailable(Task.Attribute.Title))
			{
				foreach (var id in changedTaskIds)
					RefreshNodeLabel(id, false);
			}

			if (tasks.IsAttributeAvailable(Task.Attribute.DoneDate))
			{
				foreach (var id in changedTaskIds)
				{
					var node = FindNode(id);
					RefreshNodeFont(node, false);
				}
			}

			RecalculatePositions();
*/
		}

		private bool ProcessTaskUpdate(Task task, HashSet<uint> taskIds)
		{
			if (!task.IsValid())
				return false;

			PertNetworkTaskItem item;
            uint taskId = task.GetID();
			bool newTask = !m_Items.TryGetValue(taskId, out item);

			if (newTask)
			{
/*
				var parentId = task.GetParentTask().GetID();
				var parentNode = FindNode(parentId);

				if ((parentId == 0) && (parentNode == null))
				{
					// If our parent's ID is zero and couldn't be found then it 
					// means the existing root item is a sibling of ours.
					// So we need to create a new non-task root, move the existing
					// root to the new root and use the new root as our parent
					var rootItem = TaskItem(RootNode);

					if (rootItem.IsTask)
					{
						var prevRootNode = RootNode;
						base.Clear();

						parentNode = AddRootNode(new PertNetworkTaskItem(m_Trans.Translate("Root")));
						parentNode.Nodes.Add(prevRootNode);
					}
					else
					{
						// ????
						parentNode = RootNode;
					}
				}

				AddTaskToTree(task, parentNode, true);
*/
			}
			else if (item.ProcessTaskUpdate(task))
			{
				// Process children
				Task subtask = task.GetFirstSubtask();

				while (subtask.IsValid() && ProcessTaskUpdate(subtask, taskIds))
					subtask = subtask.GetNextTask();
			}
			else
			{
				return false;
			}

			taskIds.Add(task.GetID());

			return true;
		}

		private String GetProjectName(TaskList tasks)
		{
			String rootName = tasks.GetProjectName();

			if (!String.IsNullOrWhiteSpace(rootName))
				return rootName;

			// else
			return m_Trans.Translate("Root");
		}

		protected override bool IsAcceptableDropTarget(Object draggedItemData, Object dropTargetItemData, DropPos dropPos, bool copy)
		{
			if (dropPos == PertNetworkControl.DropPos.On)
				return !TaskItem(dropTargetItemData).IsLocked;

			// else
			return true;
		}

		protected override bool IsAcceptableDragSource(Object itemData)
		{
			return !TaskItem(itemData).IsLocked;
		}

		protected override bool DoDrop(PertNetworkDragEventArgs e)
		{
			// TODO

			return true;
		}

		private PertNetworkTaskItem GetRealTaskItem(PertNetworkTaskItem taskItem)
		{
			if ((taskItem != null) && taskItem.IsReference)
			{
				PertNetworkTaskItem refItem;

				if (m_Items.TryGetValue(taskItem.ReferenceId, out refItem))
					return refItem;
			}

			// all else
			return taskItem;
		}

		protected override Color GetNodeBackgroundColor(Object itemData)
		{
			if (m_TaskColorIsBkgnd)
			{
				var taskItem = (itemData as PertNetworkTaskItem);
				var realItem = GetRealTaskItem(taskItem);

				if (!taskItem.TextColor.IsEmpty && !realItem.IsDone(true))
					return taskItem.TextColor;
			}

			// all else
			return base.GetNodeBackgroundColor(itemData);
		}

		protected void DrawZoomedImage(Image image, Graphics graphics, Rectangle destRect)
		{
			Debug.Assert(IsZoomed);

			var gSave = graphics.Save();

			graphics.CompositingMode = CompositingMode.SourceCopy;
			graphics.CompositingQuality = CompositingQuality.HighQuality;
			graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
			graphics.SmoothingMode = SmoothingMode.HighQuality;
			graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

			graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel);
			graphics.Restore(gSave);
		}

/*
		protected override void DrawNodeLabel(Graphics graphics, String label, Rectangle rect,
											  NodeDrawState nodeState, NodeDrawPos nodePos,
                                              Font nodeFont, Object itemData)
		{
            var taskItem = (itemData as PertNetworkTaskItem);
			var realItem = GetRealTaskItem(taskItem);

			bool isSelected = (nodeState != NodeDrawState.None);
			Rectangle iconRect = Rectangle.Empty;

            if (taskItem.IsTask) // not root
            {
                // Checkbox
                Rectangle checkRect = CalcCheckboxRect(rect);

                if (m_ShowCompletionCheckboxes)
				{
					if (!IsZoomed)
					{
						CheckBoxRenderer.DrawCheckBox(graphics, checkRect.Location, GetItemCheckboxState(realItem));
					}
					else
					{
						var tempImage = new Bitmap(m_CheckboxSize.Width, m_CheckboxSize.Height); // original size

						using (var gTemp = Graphics.FromImage(tempImage))
						{
							CheckBoxRenderer.DrawCheckBox(gTemp, new Point(0, 0), GetItemCheckboxState(realItem));

							DrawZoomedImage(tempImage, graphics, checkRect);
						}
					}
				}

				// Task icon
				if (TaskHasIcon(realItem))
                {
                    iconRect = CalcIconRect(rect);

                    if (m_TaskIcons.Get(realItem.ID))
					{
						if (!IsZoomed)
						{
							m_TaskIcons.Draw(graphics, iconRect.X, iconRect.Y);
						}
						else
						{
							int imageSize = ScaleByDPIFactor(16);
							var tempImage = new Bitmap(imageSize, imageSize); // original size

							using (var gTemp = Graphics.FromImage(tempImage))
							{
								gTemp.FillRectangle(SystemBrushes.Window, 0, 0, imageSize, imageSize);
								m_TaskIcons.Draw(gTemp, 0, 0);

								DrawZoomedImage(tempImage, graphics, iconRect);
							}
						}
					}

					rect.Width = (rect.Right - iconRect.Right - 2);
                    rect.X = iconRect.Right + 2;
                }
                else if (m_ShowCompletionCheckboxes)
                {
                    rect.Width = (rect.Right - checkRect.Right - 2);
                    rect.X = checkRect.Right + 2;
                }
            }

			// Text Colour
			Color textColor = SystemColors.WindowText;

			if (!taskItem.TextColor.IsEmpty)
			{
				if (m_TaskColorIsBkgnd && !isSelected && !realItem.IsDone(true))
				{
					textColor = DrawingColor.GetBestTextColor(taskItem.TextColor);
				}
				else if (isSelected)
				{
					textColor = DrawingColor.SetLuminance(taskItem.TextColor, 0.3f);
				}
                else
                {
                    textColor = taskItem.TextColor;
                }
            }

			switch (nodeState)
			{
				case NodeDrawState.Selected:
                    UIExtension.SelectionRect.Draw(this.Handle, 
													graphics, 
													rect.X, 
													rect.Y, 
													rect.Width, 
													rect.Height, 
													false); // opaque
					break;

				case NodeDrawState.DropTarget:
                    UIExtension.SelectionRect.Draw(this.Handle, 
													graphics, 
													rect.X, 
													rect.Y, 
													rect.Width, 
													rect.Height,
													UIExtension.SelectionRect.Style.DropHighlighted,
													false); // opaque
					break;
			}

			if (DebugMode())
               graphics.DrawRectangle(new Pen(Color.Green), rect);

			// Text
			var format = DefaultLabelFormat(nodePos, isSelected);

            graphics.DrawString(label, nodeFont, new SolidBrush(textColor), rect, format);

			// Draw Windows shortcut icon if task is a reference
			if (taskItem.IsReference)
			{
				if (iconRect == Rectangle.Empty)
					iconRect = rect;
				else
					iconRect.Y = (rect.Bottom - iconRect.Height); // don't want shortcut icon centred vertically

				UIExtension.ShortcutOverlay.Draw(graphics, iconRect.X, iconRect.Y, iconRect.Width, iconRect.Height);
			}
		}
*/

		private bool TaskHasIcon(PertNetworkTaskItem taskItem)
		{
			if ((m_TaskIcons == null) || (taskItem == null) || !taskItem.IsTask)
				return false;

			return (taskItem.HasIcon || (m_ShowParentAsFolder && taskItem.IsParent));
		}

		private Point[] CalcHorzDependencyArrow(Point point, int itemHeight, bool left)
		{
			Point[] arrow = new Point[] { point, point, point };

			// Size to match Gantt Chart
			int ARROW = (itemHeight / 4);

			if (left)
			{
				// <----
				//
				arrow[0].Offset(ARROW, -ARROW);
				arrow[2].Offset(ARROW, ARROW);
			}
			else // right
			{
				// --->
				//
				arrow[0].Offset(-ARROW, -ARROW);
				arrow[2].Offset(-ARROW, ARROW);
			}

			return arrow;
		}

		private Point[] CalcVertDependencyArrow(Point point, int itemHeight, bool up)
		{
			Point[] arrow = new Point[] { point, point, point };

			// Size to match Gantt Chart
			int ARROW = (itemHeight / 4);

			if (up)
			{
				//  ^
				//  |
				//
				arrow[0].Offset(-ARROW, ARROW);
				arrow[2].Offset(ARROW, ARROW);
			}
			else // down
			{
				//  |
				//  V
				//
				arrow[0].Offset(-ARROW, -ARROW);
				arrow[2].Offset(ARROW, -ARROW);
			}

			return arrow;
		}

		private void DrawHorzDependencyArrowHead(Graphics graphics, Point point, int itemHeight, bool left)
		{
			graphics.DrawLines(/*Pens.Red*/Pens.Black, CalcHorzDependencyArrow(point, itemHeight, left));

			// Offset and draw again
			if (left)
				point.X++;
			else
				point.X--;

			graphics.DrawLines(/*Pens.Red*/Pens.Black, CalcHorzDependencyArrow(point, itemHeight, left));
		}

		private void DrawVertDependencyArrowHead(Graphics graphics, Point point, int itemHeight, bool up)
		{
			graphics.DrawLines(/*Pens.Red*/Pens.Black, CalcVertDependencyArrow(point, itemHeight, up));

			// Offset and draw again
			if (up)
				point.Y++;
			else
				point.Y--;

			graphics.DrawLines(/*Pens.Red*/Pens.Black, CalcVertDependencyArrow(point, itemHeight, up));
		}

/*
		protected void DrawTaskDependency(Graphics graphics, TreeNode nodeFrom, TreeNode nodeTo)
		{
			if ((nodeFrom == null) || (nodeTo == null))
				return;

			// Disable anti-aliasing for drawing arrow heads
			// to better match the core app
			var prevSmoothing = graphics.SmoothingMode;
			graphics.SmoothingMode = SmoothingMode.None;

			PertNetworkItem itemFrom = Item(nodeFrom);
			PertNetworkItem itemTo = Item(nodeTo);

			Rectangle rectFrom = GetItemDrawRect(itemFrom.ItemBounds);
			Rectangle rectTo = GetItemDrawRect(itemTo.ItemBounds);

			bool fromIsAboveTo = (rectFrom.Bottom <= rectTo.Top);
			bool fromIsBelowTo = (rectFrom.Top >= rectTo.Bottom);

			int itemHeight = (rectFrom.Height - ItemVertSeparation);
			Point ptFrom, ptTo, ptControlFrom, ptControlTo;

			// Leaf tasks on the same side of the root
			// are a special case
			if (IsDependencyLeafNode(nodeFrom) && 
				IsDependencyLeafNode(nodeTo) &&
				(itemFrom.IsFlipped == itemTo.IsFlipped))
			{
				int controlX = 0;

				if (itemFrom.IsFlipped)
				{
					// Left side
					ptFrom = RectUtil.MiddleLeft(rectFrom);
					ptTo = RectUtil.MiddleLeft(rectTo);

					controlX = (Math.Min(ptFrom.X, ptTo.X) - DependencyOffset);
				}
				else // right side
				{
					ptFrom = RectUtil.MiddleRight(rectFrom);
					ptTo = RectUtil.MiddleRight(rectTo);

					controlX = (Math.Max(ptFrom.X, ptTo.X) + DependencyOffset);
				}

				ptControlFrom = new Point(controlX, ptFrom.Y);
				ptControlTo = new Point(controlX, ptTo.Y);

				DrawHorzDependencyArrowHead(graphics, ptFrom, itemHeight, !itemFrom.IsFlipped);
			}
			else // All other arrangements are just variations on a theme
			{
				bool fromIsLeftOfTo = (rectFrom.Right <= rectTo.Left);
				bool fromIsRightOfTo = (rectFrom.Left >= rectTo.Right);

				// Deflate the rects vertically to allow for the inter task spacing
				rectFrom.Inflate(0, -ItemVertSeparation);
				rectTo.Inflate(0, -ItemVertSeparation);

				if (fromIsLeftOfTo)
				{
					if (fromIsAboveTo)
					{
						ptFrom = RectUtil.MiddleRight(rectFrom);
						ptTo = RectUtil.TopCentre(rectTo);

						ptControlFrom = new Point(ptTo.X, ptFrom.Y);
						ptControlTo = ptControlFrom;
					}
					else if (fromIsBelowTo)
					{
						ptFrom = RectUtil.MiddleRight(rectFrom);
						ptTo = RectUtil.BottomCentre(rectTo);

						ptControlFrom = new Point(ptTo.X, ptFrom.Y);
						ptControlTo = ptControlFrom;
					}
					else // horizontally aligned
					{
						ptFrom = RectUtil.MiddleRight(rectFrom);
						ptTo = RectUtil.MiddleLeft(rectTo);

						int diff = PointUtil.Distance(ptFrom, ptTo);

						ptControlFrom = new Point(ptFrom.X + diff / 3, ptFrom.Y);
						ptControlTo = new Point(ptTo.X - diff / 3, ptTo.Y); ;
					}

					DrawHorzDependencyArrowHead(graphics, ptFrom, itemHeight, true);
				}
				else if (fromIsRightOfTo)
				{
					if (fromIsAboveTo)
					{
						ptFrom = RectUtil.MiddleLeft(rectFrom);
						ptTo = RectUtil.TopCentre(rectTo);

						ptControlFrom = new Point(ptTo.X, ptFrom.Y);
						ptControlTo = ptControlFrom;
					}
					else if (fromIsBelowTo)
					{
						ptFrom = RectUtil.MiddleLeft(rectFrom);
						ptTo = RectUtil.BottomCentre(rectTo);

						ptControlFrom = new Point(ptTo.X, ptFrom.Y);
						ptControlTo = ptControlFrom;
					}
					else // horizontally aligned
					{
						ptFrom = RectUtil.MiddleLeft(rectFrom);
						ptTo = RectUtil.MiddleRight(rectTo);

						int diff = PointUtil.Distance(ptFrom, ptTo);

						ptControlFrom = new Point(ptFrom.X - diff / 3, ptFrom.Y);
						ptControlTo = new Point(ptTo.X + diff / 3, ptTo.Y); ;
					}

					DrawHorzDependencyArrowHead(graphics, ptFrom, itemHeight, false);
				}
				else if (fromIsAboveTo)
				{
					ptFrom = RectUtil.BottomCentre(rectFrom);
					ptTo = RectUtil.TopCentre(rectTo);

					int diff = PointUtil.Distance(ptFrom, ptTo);

					ptControlFrom = new Point(ptFrom.X, ptFrom.Y + diff / 3);
					ptControlTo = new Point(ptTo.X, ptTo.Y - diff / 3);

					DrawVertDependencyArrowHead(graphics, ptFrom, itemHeight, true);
				}
				else if (fromIsBelowTo)
				{
					ptFrom = RectUtil.TopCentre(rectFrom);
					ptTo = RectUtil.BottomCentre(rectTo);

					int diff = PointUtil.Distance(ptFrom, ptTo);

					ptControlFrom = new Point(ptFrom.X, ptFrom.Y - diff / 3);
					ptControlTo = new Point(ptTo.X, ptTo.Y + diff / 3); ;

					DrawVertDependencyArrowHead(graphics, ptFrom, itemHeight, false);
				}
				else
				{
					// Overlaps ??
					return;
				}
			}

			// Draw 3x3 box at 'to' end
			Rectangle box = new Rectangle(ptTo.X - 1, ptTo.Y - 1, 3, 3);
			graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x4f, 0x4f, 0x4f)), box);

			// Re-enable anti-aliasing for dependency lines
			graphics.SmoothingMode = prevSmoothing;
			
			graphics.DrawBezier(/ *Pens.Red* /m_DependencyPen, ptFrom, ptControlFrom, ptControlTo, ptTo);

		}
*/

        private Rectangle CalcIconRect(Rectangle labelRect)
		{
            int left = (labelRect.X + 2);
            
            if (m_ShowCompletionCheckboxes)
                left += (int)(m_CheckboxSize.Width * ImageZoomFactor);

			int width = (int)(ScaleByDPIFactor(16) * ImageZoomFactor);
			int height = width;

			int top = labelRect.Top;// (CentrePoint(labelRect).Y - (height / 2));

            return new Rectangle(left, top, width, height);
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
			var taskItem = new PertNetworkTaskItem(task);

/*
			var node = AddNode(taskItem, parent, taskID);

			if (node == null)
				return false;

            RefreshNodeFont(node, false);

			// First Child
			if (!AddTaskToTree(task.GetFirstSubtask(), node))
				return false;

			// First Sibling
			if (!AddTaskToTree(task.GetNextTask(), parent))
				return false;

			m_Items.Add(taskID, taskItem);

			if (select)
				SelectedNode = node;
*/

			return true;
		}

// 		protected override int GetExtraWidth(TreeNode node)
// 		{
//             int extraWidth = base.GetExtraWidth(node);
//             var taskItem = RealTaskItem(node);
// 
//             if (m_ShowCompletionCheckboxes && taskItem.IsTask)
//                 extraWidth += (int)(m_CheckboxSize.Width * ImageZoomFactor);
// 
// 			if (TaskHasIcon(taskItem))
// 				extraWidth += (int)((ScaleByDPIFactor(16) + 2) * ImageZoomFactor);
// 
// 			return extraWidth;
// 		}

		protected override int GetMinItemHeight()
		{
            return (ScaleByDPIFactor(16) + 1);
		}

		protected override void OnMouseDoubleClick(MouseEventArgs e)
		{
// 			if (HitTestPositions(e.Location) != null)
// 				EditTaskLabel(this, UniqueID(SelectedNode));
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

/*
			TreeNode node = HitTestPositions(e.Location);

			if ((node == null) || (node != SelectedNode) || !NodeIsTask(node))
				return;

			if (HitTestExpansionButton(node, e.Location))
				return;

            var taskItem = RealTaskItem(node);

            if (!ReadOnly && !taskItem.IsLocked)
            {
                if (HitTestCheckbox(node, e.Location))
                {
					if (EditTaskDone != null)
						EditTaskDone(this, taskItem.ID, !taskItem.IsDone(false));
				}
                else if (HitTestIcon(node, e.Location))
                {
                    // Performing icon editing from a 'MouseUp' or 'MouseClick' event 
                    // causes the edit icon dialog to fail to correctly get focus but
                    // counter-intuitively it works from 'MouseDown'
                    //if (EditTaskIcon != null)
                    //    EditTaskIcon(this, UniqueID(SelectedNode));
                }
                else if (SelectNodeWasPreviouslySelected)
			    {
			        if (EditTaskLabel != null)
				        m_EditTimer.Start();
                }
            }
*/
		}

		private bool HitTestIcon(TreeNode node, Point point)
        {
			/*
						var taskItem = RealTaskItem(node);

						if (taskItem.IsLocked || !TaskHasIcon(taskItem))
							return false;

						// else
						return CalcIconRect(GetItemLabelRect(node)).Contains(point);
			*/
			return false;
        }

		protected override void OnMouseDown(MouseEventArgs e)
		{
			m_EditTimer.Stop();

			base.OnMouseDown(e);
		}

		private void OnEditLabelTimer(object sender, EventArgs e)
		{
			m_EditTimer.Stop();

// 			if (EditTaskLabel != null)
// 				EditTaskLabel(this, UniqueID(SelectedNode));
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
 			base.OnMouseMove(e);

/*
			var node = HitTestPositions(e.Location);

			if (!ReadOnly && (node != null) && !HitTestExpansionButton(node, e.Location))
			{
				var taskItem = RealTaskItem(node);

				if (taskItem != null)
                {
                    Cursor cursor = null;

                    if (taskItem.IsLocked)
                    {
                        cursor = UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);
                    }
                    else if (TaskHasIcon(taskItem) && HitTestIcon(node, e.Location))
                    {
                        cursor = UIExtension.HandCursor();
                    }
                    
                    if (cursor != null)
                    {
                        Cursor = cursor;
                        return;
                    }
				}
			}
*/

			// all else
			Cursor = Cursors.Arrow;
		}
	}
}

