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

	class PertNetworkTaskItem : PertNetworkItem
	{
		// Data
		public Color TextColor { get; private set; }
		public bool HasIcon { get; private set; }
		public bool IsFlagged { get; private set; }
		public bool IsParent { get; private set; }
		public bool IsDone { get; private set; }
		public bool IsGoodAsDone { get; private set; }
		public bool SomeSubtasksDone { get; private set; }
		public bool IsLocked { get; private set; }

		// -----------------------------------------------------------------

		public PertNetworkTaskItem(String label, uint taskId) : base(label, taskId)
		{
			TextColor = new Color();
			HasIcon = false;
			IsFlagged = false;
			IsDone = false;
            IsGoodAsDone = false;
            SomeSubtasksDone = false;
			IsLocked = false;
		}

		public PertNetworkTaskItem(Task task) : base(task.GetTitle(), task.GetID())
		{
			TextColor = task.GetTextDrawingColor();
			HasIcon = (task.GetIcon().Length > 0);
			IsFlagged = task.IsFlagged(false);
            IsDone = task.IsDone();
            IsGoodAsDone = task.IsGoodAsDone();
            SomeSubtasksDone = task.HasSomeSubtasksDone();
			IsLocked = task.IsLocked(true);
			DependencyUniqueIds = task.GetLocalDependency();
		}

		public override string ToString() 
		{
			return Title;
		}

		public void Update(Task task, HashSet<Task.Attribute> attribs)
		{
			// TODO
		}

		public bool HasLocalDependencies {  get { return (DependencyUniqueIds != null) && (DependencyUniqueIds.Count > 0); } }

/*
		public bool IsDone(bool includeGoodAsDone) 
        { 
            if (includeGoodAsDone && IsGoodAsDone)
                return true;

            return IsDone; 
        }

		public bool SetDone(bool done = true)
		{
			if (IsDone == done)
				return false;

			IsDone = done;
			return true;
		}
*/

		public bool Update(Task task)
		{
			if (task.GetID() != UniqueId)
				return false;

			if (task.GetReferenceID() != 0)
				return false;

			if (task.IsAttributeAvailable(Task.Attribute.Title))
				Title = task.GetTitle();

			if (task.IsAttributeAvailable(Task.Attribute.Icon))
				HasIcon = (task.GetIcon().Length > 0);

			if (task.IsAttributeAvailable(Task.Attribute.Flag))
				IsFlagged = task.IsFlagged(false);

			if (task.IsAttributeAvailable(Task.Attribute.Color))
				TextColor = task.GetTextDrawingColor();

            if (task.IsAttributeAvailable(Task.Attribute.SubtaskDone))
                SomeSubtasksDone = task.HasSomeSubtasksDone();

            if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
                IsDone = task.IsDone();

			if (task.IsAttributeAvailable(Task.Attribute.Dependency))
				DependencyUniqueIds = task.GetLocalDependency();

			IsParent = task.IsParent();
			IsLocked = task.IsLocked(true);
            IsGoodAsDone = task.IsGoodAsDone();

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
		private Translator Trans;
		private UIExtension.TaskIcon TaskIcons;

		private bool ShowParentAsFolder;
		private bool TaskColorIsBkgnd;
		private bool IgnoreMouseClick;
		private bool StrikeThruDone;

		private Timer EditTimer;
        private Font BoldLabelFont, DoneLabelFont, BoldDoneLabelFont;
        private Size CheckboxSize;
		private Pen DependencyPen;

		// -------------------------------------------------------------------------

		public TdlPertNetworkControl(Translator trans, UIExtension.TaskIcon icons)
		{
			Trans = trans;
			TaskIcons = icons;

			DependencyPen = new Pen(Color.Black);
			DependencyPen.DashStyle = DashStyle.Dot;

			TaskColorIsBkgnd = false;
			IgnoreMouseClick = false;
			ShowParentAsFolder = false;
            ShowCompletionCheckboxes = true;
			StrikeThruDone = true;

			EditTimer = new Timer();
			EditTimer.Interval = 500;
			EditTimer.Tick += new EventHandler(OnEditLabelTimer);

            using (Graphics graphics = Graphics.FromHwnd(Handle))
                CheckboxSize = CheckBoxRenderer.GetGlyphSize(graphics, CheckBoxState.UncheckedNormal);
		}
        
        public void SetStrikeThruDone(bool strikeThruDone)
		{
			StrikeThruDone = strikeThruDone;

			if (BoldLabelFont != null)
				SetFont(BoldLabelFont.Name, (int)BoldLabelFont.Size, StrikeThruDone);
		}

		public new void SetFont(String fontName, int fontSize)
		{
			SetFont(fontName, fontSize, StrikeThruDone);
		}

		protected void SetFont(String fontName, int fontSize, bool strikeThruDone)
		{
			bool baseFontChange = ((BoldLabelFont == null) || (BoldLabelFont.Name != fontName) || (BoldLabelFont.Size != fontSize));
            bool doneFontChange = (baseFontChange || (BoldDoneLabelFont.Strikeout != strikeThruDone));

            if (baseFontChange)
                BoldLabelFont = new Font(fontName, fontSize, FontStyle.Bold);

            if (doneFontChange)
            {
                if (strikeThruDone)
                {
                    BoldDoneLabelFont = new Font(fontName, fontSize, FontStyle.Bold | FontStyle.Strikeout);
                    DoneLabelFont = new Font(fontName, fontSize, FontStyle.Strikeout);
                }
                else
                {
                    BoldDoneLabelFont = BoldLabelFont;
                    DoneLabelFont = null;
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
				break;

			case UIExtension.UpdateType.All:
				Data.Clear();
				break;

			case UIExtension.UpdateType.Unknown:
				return;
			}

			UpdateTaskAttributes(tasks);
		}

		public PertNetworkOption Options;
// 		{
// 			get { return Options; }
// 
// 			set
// 			{
// 				if (value != Options)
// 				{
// 					Options = value;
// 					Invalidate();
// 				}
// 			}
// 		}

		// 		public bool SelectNodeWasPreviouslySelected
		// 		{
		// 			get { return (SelectedNode == PreviouslySelectedNode); }
		// 		}

		public bool TaskColorIsBackground;
// 		{
// 			get { return TaskColorIsBkgnd; }
// 			set
// 			{
// 				if (TaskColorIsBkgnd != value)
// 				{
// 					TaskColorIsBkgnd = value;
// 					Invalidate();
// 				}
// 			}
// 		}

		public bool ShowParentsAsFolders;
// 		{
// 			get { return ShowParentAsFolder; }
// 			set
// 			{
// 				if (ShowParentAsFolder != value)
// 				{
// 					ShowParentAsFolder = value;
// 					Invalidate();
// 				}
// 			}
// 		}

		public bool ShowCompletionCheckboxes;
//         {
//             get { return ShowCompletionCheckboxes; }
//             set
//             {
//                 if (ShowCompletionCheckboxes != value)
//                 {
//                     ShowCompletionCheckboxes = value;
//                 }
//             }
//         }

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
			Task task = tasks.GetFirstTask();

			bool dependencyChanges = false;

			while (task.IsValid())
			{
				dependencyChanges |= ProcessTaskUpdate(task);
				task = task.GetNextTask();
			}

			if (dependencyChanges)
				RebuildGroups();
			else
				Invalidate();
		}

		private bool ProcessTaskUpdate(Task task)
		{
			if (!task.IsValid())
				return false;

			PertNetworkItem item = Data.GetItem(task.GetID());

			if (item == null)
			{
				Data.AddItem(new PertNetworkTaskItem(task));
			}
			else
			{
				PertNetworkTaskItem taskItem = (item as PertNetworkTaskItem);
				taskItem.Update(task);
			}

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask))
				subtask = subtask.GetNextTask();

			return true;
		}

		private String GetProjectName(TaskList tasks)
		{
			String rootName = tasks.GetProjectName();

			if (!String.IsNullOrWhiteSpace(rootName))
				return rootName;

			// else
			return Trans.Translate("Root");
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

		protected override Color GetNodeBackgroundColor(Object itemData)
		{
			if (TaskColorIsBkgnd)
			{
				var taskItem = (itemData as PertNetworkTaskItem);

				if (!taskItem.TextColor.IsEmpty && !taskItem.IsDone && !taskItem.IsGoodAsDone)
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

                if (ShowCompletionCheckboxes)
				{
					if (!IsZoomed)
					{
						CheckBoxRenderer.DrawCheckBox(graphics, checkRect.Location, GetItemCheckboxState(realItem));
					}
					else
					{
						var tempImage = new Bitmap(CheckboxSize.Width, CheckboxSize.Height); // original size

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

                    if (TaskIcons.Get(realItem.ID))
					{
						if (!IsZoomed)
						{
							TaskIcons.Draw(graphics, iconRect.X, iconRect.Y);
						}
						else
						{
							int imageSize = ScaleByDPIFactor(16);
							var tempImage = new Bitmap(imageSize, imageSize); // original size

							using (var gTemp = Graphics.FromImage(tempImage))
							{
								gTemp.FillRectangle(SystemBrushes.Window, 0, 0, imageSize, imageSize);
								TaskIcons.Draw(gTemp, 0, 0);

								DrawZoomedImage(tempImage, graphics, iconRect);
							}
						}
					}

					rect.Width = (rect.Right - iconRect.Right - 2);
                    rect.X = iconRect.Right + 2;
                }
                else if (ShowCompletionCheckboxes)
                {
                    rect.Width = (rect.Right - checkRect.Right - 2);
                    rect.X = checkRect.Right + 2;
                }
            }

			// Text Colour
			Color textColor = SystemColors.WindowText;

			if (!taskItem.TextColor.IsEmpty)
			{
				if (TaskColorIsBkgnd && !isSelected && !realItem.IsDone(true))
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
			if ((TaskIcons == null) || (taskItem == null))
				return false;

			return (taskItem.HasIcon || (ShowParentAsFolder && taskItem.IsParent));
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
			
			graphics.DrawBezier(/ *Pens.Red* /DependencyPen, ptFrom, ptControlFrom, ptControlTo, ptTo);

		}
*/

        private Rectangle CalcIconRect(Rectangle labelRect)
		{
            int left = (labelRect.X + 2);
            
            if (ShowCompletionCheckboxes)
                left += (int)(CheckboxSize.Width * ImageZoomFactor);

			int width = (int)(ScaleByDPIFactor(16) * ImageZoomFactor);
			int height = width;

			int top = labelRect.Top;// (CentrePoint(labelRect).Y - (height / 2));

            return new Rectangle(left, top, width, height);
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

			Items.Add(taskID, taskItem);

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
//             if (ShowCompletionCheckboxes && taskItem.IsTask)
//                 extraWidth += (int)(CheckboxSize.Width * ImageZoomFactor);
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

			if (IgnoreMouseClick)
			{
				IgnoreMouseClick = false;
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
				        EditTimer.Start();
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
			EditTimer.Stop();

			base.OnMouseDown(e);
		}

		private void OnEditLabelTimer(object sender, EventArgs e)
		{
			EditTimer.Stop();

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

