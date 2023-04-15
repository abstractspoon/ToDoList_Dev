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

namespace DetectiveBoardUIExtension
{
    public delegate bool EditTaskLabelEventHandler(object sender, uint taskId);
    public delegate bool EditTaskIconEventHandler(object sender, uint taskId);
	public delegate bool EditTaskCompletionEventHandler(object sender, uint taskId, bool completed);
	public delegate bool TaskModifiedEventHandler(object sender, IList<uint> taskIds);

	// ------------------------------------------------------------

	[Flags]
	enum DetectiveBoardOption
	{
		None					= 0x00,
		ShowParentChildLinks	= 0x01,
		ShowDependencies		= 0x02,
		ShowRootNode			= 0x04,
		DrawLinksOnTop			= 0x08,
	}

	// ------------------------------------------------------------

	[System.ComponentModel.DesignerCategory("")]
	class TDLNodeControl : NodeControl, IDragRenderer
	{
		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;
		public event TaskModifiedEventHandler TaskModified;

		public EventHandler UserLinkSelectionChange;
		public EventHandler DoubleClickUserLink;

		// -------------------------------------------------------------------------

		const DetectiveBoardOption DefaultOptions = (DetectiveBoardOption.ShowRootNode |
												DetectiveBoardOption.ShowParentChildLinks |
												DetectiveBoardOption.ShowDependencies |
												DetectiveBoardOption.DrawLinksOnTop);
		// -------------------------------------------------------------------------

		protected int LabelPadding { get { return ScaleByDPIFactor(2); } }

		// -------------------------------------------------------------------------

		// From Parent
		private Translator m_Trans;
		private UIExtension.TaskIcon m_TaskIcons;

		private bool m_ShowParentAsFolder = false;
		private bool m_TaskColorIsBkgnd = false;
		private bool m_StrikeThruDone = true;
		private bool m_ShowCompletionCheckboxes = true;

		private DetectiveBoardOption m_Options = DetectiveBoardOption.None;

		private Timer m_EditTimer;
		private Font m_BoldLabelFont, m_DoneLabelFont, m_BoldDoneLabelFont;
		//		private Size CheckboxSize;

		private TaskItem m_PreviouslySelectedTask;
		private UserLink m_SelectedUserLink;

		private DragImage m_DragImage;
		private Point m_LastDragPos;

		private TaskItems m_TaskItems;
		
		// -------------------------------------------------------------------------

		[Flags]
		protected enum DrawState
		{
			None				= 0x00,
			Selected			= 0x01,
			DropHighlighted		= 0x02,
			DragImage			= 0x08,
		}

		// -------------------------------------------------------------------------

		public TDLNodeControl(Translator trans, UIExtension.TaskIcon icons)
		{
			m_Trans = trans;
			m_TaskIcons = icons;

			m_EditTimer = new Timer();
			m_EditTimer.Interval = 500;
			m_EditTimer.Tick += new EventHandler(OnEditLabelTimer);

			m_DragImage = new DragImage();
			m_LastDragPos = Point.Empty;
			Options = DefaultOptions;

			m_TaskItems = null;

			int nodeHeight = (int)(2 * BaseFontHeight) + 4;
			int nodeWidth  = (4 * nodeHeight);

			base.NodeSize = new Size(nodeWidth, nodeHeight);
			base.PinRadius = ScaleByDPIFactor(4);

			DragDropChange += new DragDropChangeEventHandler(OnDragDrop);
			NodeSelectionChange += (s, ids) => { ClearUserLinkSelection(); };

			RebuildFonts();


			// 			using (Graphics graphics = Graphics.FromHwnd(Handle))
			// 				CheckboxSize = CheckBoxRenderer.GetGlyphSize(graphics, CheckBoxState.UncheckedNormal);

			base.AutoCalculateRadialIncrement = true;
		}

		private void RebuildFonts()
		{
			var newFont = TextFont;

			m_BoldLabelFont = new Font(newFont.Name, newFont.Size, FontStyle.Bold);

			if (m_StrikeThruDone)
			{
				m_BoldDoneLabelFont = new Font(newFont.Name, newFont.Size, FontStyle.Bold | FontStyle.Strikeout);
				m_DoneLabelFont = new Font(newFont.Name, newFont.Size, FontStyle.Strikeout);
			}
			else
			{
				m_BoldDoneLabelFont = m_BoldLabelFont;
				m_DoneLabelFont = null;
			}
		}

		protected override void OnTextFontChanged()
		{
			base.OnTextFontChanged();

			RebuildFonts();
		}

		public void SetFont(String fontName, int fontSize)
		{
			if ((this.Font.Name == fontName) && (this.Font.Size == fontSize))
				return;

			this.Font = new Font(fontName, fontSize, FontStyle.Regular);
		}

		public void SetStrikeThruDone(bool strikeThruDone)
		{
			if (m_StrikeThruDone != strikeThruDone)
			{
				m_StrikeThruDone = strikeThruDone;
				RebuildFonts();
			}
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			switch (type)
			{
			case UIExtension.UpdateType.Edit:
			case UIExtension.UpdateType.New:
				UpdateTaskAttributes(tasks);
				break;

			case UIExtension.UpdateType.Delete:
			case UIExtension.UpdateType.All:
				m_TaskItems = new TaskItems();
				UpdateTaskAttributes(tasks);
				RecalcLayout();
				break;

			case UIExtension.UpdateType.Unknown:
				return;
			}
		}

		protected override void OnAfterRecalcLayout()
		{
			ApplyUserPositions(RootNode);
		}

		private void ApplyUserPositions(RadialTree.TreeNode<uint> node)
		{
			var task = GetTaskItem(node);

			if ((task != null) && task.HasUserPosition)
				node.Point.SetPosition(task.UserPosition);

			// children
			foreach (var child in node.Children)
				ApplyUserPositions(child);
		}
		
		public DetectiveBoardOption Options
		{
			get { return m_Options; }
		
			set
			{
				if (value != m_Options)
				{
					m_Options = value;
					base.DrawNodesOnTop = !m_Options.HasFlag(DetectiveBoardOption.DrawLinksOnTop);

					Invalidate();
				}
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

		public bool StrikeThruDone
		{
			get { return m_StrikeThruDone; }
			set
			{
				if (m_StrikeThruDone != value)
				{
					m_StrikeThruDone = value;
					Invalidate();
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
			case Task.Attribute.MetaData:
			case Task.Attribute.FileLink:
			case Task.Attribute.Dependency:
				return true;
			}

			// all else
			return false;
		}

		public uint HitTest(Point screenPos)
		{
			var node = base.HitTestNode(PointToClient(screenPos), true);
			
			return node?.Data ?? 0;
		}

		public Rectangle GetSelectedTaskLabelRect()
		{
			//EnsureNodeVisible(SelectedNode);
			
			var labelRect = base.GetSingleSelectedNodeRect();
			
			labelRect.X -= LabelPadding;
			//labelRect.X += GetExtraWidth(SelectedNode);
					 
			// Make sure the rect is big enough for the unscaled font
			labelRect.Height = (Font.Height + (2 * LabelPadding)); 
			
			return labelRect;
		}

		public bool IsTaskLocked(uint taskId)
		{
			return m_TaskItems.IsTaskLocked(taskId);
		}

		public bool CanMoveTask(uint taskId, uint destParentId, uint destPrevSiblingId)
		{
			return false;
		}

		public bool MoveTask(uint taskId, uint destParentId, uint destPrevSiblingId)
		{
			return false;
		}

		public bool CanCreateUserLink(uint fromId, uint toId)
		{
			return !(m_TaskItems.IsTaskLocked(fromId) || m_TaskItems.HasUserLink(fromId, toId));
		}

		public bool CreateUserLink(uint fromId, uint toId, Color color, int thickness, 
									UserLink.EndArrows arrows, string text, string type)
		{
			var fromTask = GetTaskItem(fromId);

			if (fromTask != null)
			{
				// task must be selected
				if (!SelectedNodeIds.Contains(fromId))
				{
					Debug.Assert(false);
					return false;
				}
				
				// If link (or its reverse) already exists we just update it
				var link = m_TaskItems.FindUserLink(fromId, toId);

				if (link == null)
				{
					link = new UserLink(fromId, toId);
					fromTask.UserLinks.Add(link);
				}

				link.Color = color;
				link.Thickness = thickness;
				link.Arrows = arrows;
				link.Label = text;
				link.Type = type;

				Invalidate();

				// Notify parent
				return ((TaskModified == null) ? false : TaskModified(this, new List<uint>() { fromId }));
			}

			return false;
		}

		public UserLink SelectedUserLink
		{
			get
			{
				if (ReadOnly)
					return null;

				if ((SingleSelectedNode == null) || (m_SelectedUserLink == null))
					return null;

				if (!SingleSelectedNode.Data.Equals(m_SelectedUserLink.FromId))
				{
					Debug.Assert(false);
					return null;
				}

				if (m_TaskItems.IsTaskLocked(m_SelectedUserLink.FromId))
					return null;

				return m_SelectedUserLink;
			}
		}

		public bool HasSelectedUserLink { get { return (SelectedUserLink != null); } }

		public void ClearUserLinkSelection()
		{
			if (HasSelectedUserLink)
			{
				m_SelectedUserLink = null;
				Invalidate();

				UserLinkSelectionChange?.Invoke(this, null);
			}
		}
		
		public bool EditSelectedUserLink(Color color, int thickness, UserLink.EndArrows arrows,
										string text, string type)
		{
			if (!HasSelectedUserLink)
				return false;

			m_SelectedUserLink.Color = color;
			m_SelectedUserLink.Thickness = thickness;
			m_SelectedUserLink.Arrows = arrows;
			m_SelectedUserLink.Label = text;
			m_SelectedUserLink.Type = type;

			// Clear selection so the changes are visible
			ClearUserLinkSelection();

			TaskModified?.Invoke(this, SelectedNodeIds);
			return true;
		}

		public bool DeleteSelectedUserLink()
		{
			if (!HasSelectedUserLink)
				return false;

			if (!m_TaskItems.DeleteUserLink(m_SelectedUserLink))
				return false;

			ClearUserLinkSelection();

			TaskModified?.Invoke(this, SelectedNodeIds);
			return true;
		}

		public bool SelectNode(uint taskId)
		{
			ClearUserLinkSelection();

			if (base.SelectNode(taskId))
				return true;

			base.SelectNode(NodeControl.NullId);
			return false;
		}

		public TaskItem SingleSelectedTask
		{
			get	{ return GetTaskItem(base.SingleSelectedNode); }
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
// 			if ((text == String.Empty) || IsEmpty())
// 				return false;

			/*
			TreeNode node = null; // start node
			bool forward = true;

			switch (selectTask)
			{
			case UIExtension.SelectTask.SelectFirstTask:
				node = RootNode.Nodes[0];
				break;

			case UIExtension.SelectTask.SelectNextTask:
				node = TreeCtrl.GetNextNode(SelectedNode);
				break;

			case UIExtension.SelectTask.SelectNextTaskInclCurrent:
				node = SelectedNode;
				break;

			case UIExtension.SelectTask.SelectPrevTask:
				node = TreeCtrl.GetPrevNode(SelectedNode);

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
					node = TreeCtrl.GetNextNode(node);
				else
					node = TreeCtrl.GetPrevNode(node);
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
			return (m_TaskItems.Count != 0);
		}

		// Internal ------------------------------------------------------------

		protected int ScaleByDPIFactor(int value)
		{
			return DPIScaling.Scale(value);
		}

		private void UpdateTaskAttributes(TaskList tasks)
		{
			RadialTree.TreeNode<uint> rootNode = base.RootNode;

			if (m_TaskItems.Count == 0)
			{
				rootNode = new RadialTree.TreeNode<uint>(0);
			}

			Task task = tasks.GetFirstTask();

			while (task.IsValid())
			{
				ProcessTaskUpdate(task, rootNode);
				task = task.GetNextTask();
			}

			base.EnableLayoutUpdates = false;

			base.InitialRadius = (float)((rootNode.Count * NodeSize.Width) / (2 * Math.PI));
			base.RadialIncrementOrSpacing = NodeSize.Width;

			// If the root only has one task, make it the root
// 			if (rootNode.Count == 1)
// 			{
// 				var newRoot = rootNode.Children[0];
// 				rootNode.RemoveChild(newRoot);
// 				rootNode = newRoot;
// 			}

			base.RootNode = rootNode;
			base.EnableLayoutUpdates = true;

			Invalidate();
		}

		private bool ProcessTaskUpdate(Task task, RadialTree.TreeNode<uint> parentNode)
		{
			if (!task.IsValid())
				return false;

			uint taskId = task.GetID();
			var taskItem = m_TaskItems.GetTask(taskId);
			RadialTree.TreeNode<uint> node = null;

			if (taskItem == null)
			{
				taskItem = new TaskItem(task);
				m_TaskItems.AddTask(taskItem);

				node = parentNode.AddChild(taskId);
			}
			else
			{
				taskItem.Update(task);
				node = GetNode(taskId);
			}

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, node))
			{
				taskItem.ChildIds.Add(subtask.GetID());

				subtask = subtask.GetNextTask();
			}

			return true;
		}

		protected override Size GetNodeSize(RadialTree.TreeNode<uint> node)
		{
			var size = base.GetNodeSize(node);

			if (node == RootNode)
				return new Size(size.Width / 2, size.Width / 2); // smaller square

			var task = GetTaskItem(node);

			if ((task != null) && (task.Image != null))
			{
				size.Height += task.CalcImageHeight(base.NodeSize.Width);
			}

			return size;
		}

		protected Color GetTaskBackgroundColor(TaskItem taskItem, bool selected)
		{
			if (selected)
				return Color.Empty;

			if ((taskItem.TextColor != Color.Empty) && !taskItem.IsDone(true))
			{
				if (m_TaskColorIsBkgnd && !selected)
					return taskItem.TextColor;

				// else
				return DrawingColor.SetLuminance(taskItem.TextColor, 0.95f);
			}

			// all else
			return Color.Empty;
		}

		protected Color GetTaskTextColor(TaskItem taskItem, DrawState state)
		{
			if (state.HasFlag(DrawState.DragImage))
				return SystemColors.WindowText;

			if (taskItem.TextColor != Color.Empty)
			{
				bool selected = state.HasFlag(DrawState.Selected);

				if (m_TaskColorIsBkgnd && !selected && !taskItem.IsDone(true))
					return DrawingColor.GetBestTextColor(taskItem.TextColor);

				if (selected)
					return DrawingColor.SetLuminance(taskItem.TextColor, 0.3f);

				// else
				return taskItem.TextColor;
			}

			// All else
			return SystemColors.WindowText;
		}

		protected Color GetTaskBorderColor(TaskItem taskItem, bool selected)
		{
			if (selected)
				return Color.Empty;

			if (taskItem.TextColor != Color.Empty)
			{
				if (m_TaskColorIsBkgnd && !selected && !taskItem.IsDone(true))
					return DrawingColor.SetLuminance(taskItem.TextColor, 0.3f);

				// else
				return taskItem.TextColor;
			}

			// All else
			return SystemColors.ControlDarkDark;
		}

		protected Font GetTaskLabelFont(TaskItem taskItem)
		{
			if (taskItem.IsTopLevel)
			{
				if (taskItem.IsDone(false))
					return m_BoldDoneLabelFont;

				// else
				return m_BoldLabelFont;
			}

			if (taskItem.IsDone(false))
				return m_DoneLabelFont;

			return TextFont;
		}

		protected override bool IsSelectableNode(uint nodeId)
		{
			return (nodeId != 0);
		}

		protected override void OnAfterDrawNodes(Graphics graphics)
		{
			if (DrawNodesOnTop)
				DrawSelectedUserLink(graphics);
		}

		protected override void OnAfterDrawConnections(Graphics graphics)
		{
			DrawTaskDependencies(graphics, RootNode);
			DrawUserLinks(graphics, RootNode);

			if (!DrawNodesOnTop)
				DrawSelectedUserLink(graphics);
		}

		protected void DrawSelectedUserLink(Graphics graphics)
		{
			if (HasSelectedUserLink)
			{
				Point fromPos, toPos;
				
				if (IsConnectionVisible(m_SelectedUserLink, out fromPos, out toPos))
				{
					graphics.DrawLine(SystemPens.WindowText, fromPos, toPos);

					// Draw regular pin at the 'from end'
					var pin = GetPinRect(fromPos);
					graphics.FillEllipse(SystemBrushes.Window, pin);
					graphics.DrawEllipse(SystemPens.WindowText, pin);

					// Draw a box at the 'to end' which can be moved
					pin = GetPinRect(toPos);
					graphics.FillRectangle(SystemBrushes.Window, pin);
					graphics.DrawRectangle(SystemPens.WindowText, pin);
				}
			}
		}

		protected void DrawTaskDependencies(Graphics graphics, RadialTree.TreeNode<uint> node)
		{
			if (m_Options.HasFlag(DetectiveBoardOption.ShowDependencies))
			{
				var taskItem = GetTaskItem(node);

				if (taskItem?.DependIds?.Count > 0)
				{
					foreach (var dependId in taskItem.DependIds)
					{
						Point fromPos, toPos;

						// Don't draw a dependency which is overlaid by a user link
						if (!m_TaskItems.HasUserLink(taskItem.TaskId, dependId) &&
							IsConnectionVisible(node, dependId, out fromPos, out toPos))
						{
							DrawConnection(graphics, Pens.Blue, Brushes.Blue, fromPos, toPos);
							DrawConnectionArrows(graphics, UserLink.EndArrows.Start, 2, Color.Blue, fromPos, toPos);
						}
					}
				}

				// Children
				foreach (var child in node.Children)
					DrawTaskDependencies(graphics, child);
			}
		}

		protected void DrawUserLinks(Graphics graphics, RadialTree.TreeNode<uint> node)
		{
			var taskItem = GetTaskItem(node);

			if (taskItem?.UserLinks?.Count > 0)
			{
				foreach (var link in taskItem.UserLinks)
				{
					Point fromPos, toPos;

					// Don't draw the selected connection until the very end
					if (!link.IdsMatch(m_SelectedUserLink) &&
						IsConnectionVisible(node, link.ToId, out fromPos, out toPos))
					{
						DrawConnection(graphics, new Pen(link.Color, link.Thickness), new SolidBrush(link.Color), fromPos, toPos);
						DrawConnectionArrows(graphics, link.Arrows, link.Thickness + 1, link.Color, fromPos, toPos);
					}
				}
			}

			// Children
			foreach (var child in node.Children)
				DrawUserLinks(graphics, child);
		}
		
		protected override void DrawNode(Graphics graphics, uint nodeId, Rectangle rect)
		{
			var taskItem = m_TaskItems.GetTask(nodeId);

			if (taskItem != null)
			{
				var drawState = (SelectedNodeIds.Contains(nodeId) ? DrawState.Selected : DrawState.None);
				DoPaintNode(graphics, taskItem, rect, drawState);
			}
			else if (m_Options.HasFlag(DetectiveBoardOption.ShowRootNode))
			{
				base.DrawNode(graphics, nodeId, rect);
			}
		}

		protected override void DrawParentAndChildConnections(Graphics graphics, RadialTree.TreeNode<uint> node)
		{
			if (m_Options.HasFlag(DetectiveBoardOption.ShowParentChildLinks))
				base.DrawParentAndChildConnections(graphics, node);
		}

		protected bool IsConnectionVisible(RadialTree.TreeNode<uint> fromNode, uint toId, out Point fromPos, out Point toPos)
		{
			return base.IsConnectionVisible(fromNode, GetNode(toId), out fromPos, out toPos);
		}

		protected bool IsConnectionVisible(UserLink link, out Point fromPos, out Point toPos)
		{
			return IsConnectionVisible(GetNode(link.FromId), link.ToId, out fromPos, out toPos);
		}

		protected override void DrawParentConnection(Graphics graphics, uint nodeId, Point nodePos, Point parentPos)
		{
			if (m_Options.HasFlag(DetectiveBoardOption.ShowParentChildLinks))
			{
				var taskItem = m_TaskItems.GetTask(nodeId);

				// Don't draw parent/child connections if they are
				// overlaid either by dependencies or user links
				if (m_TaskItems.HasDependency(nodeId, taskItem.ParentId)) 
					return;

				if (m_TaskItems.HasUserLink(nodeId, taskItem.ParentId))
					return;
				
				if ((taskItem?.ParentId != 0) || m_Options.HasFlag(DetectiveBoardOption.ShowRootNode))
				{
					DrawConnection(graphics, Pens.Gray, Brushes.Gray, nodePos, parentPos);
					DrawConnectionArrows(graphics, UserLink.EndArrows.Finish, 2, Color.Gray, nodePos, parentPos);
				}
			}
		}

		protected void DrawConnectionArrows(Graphics graphics, UserLink.EndArrows arrows, int thickness, Color color, Point fromPos, Point toPos)
		{
			if (arrows != UserLink.EndArrows.None)
			{
				using (var pen = new Pen(color, thickness))
				{
					int size = UIExtension.DependencyArrows.Size(Font) + 2;

					if ((arrows == UserLink.EndArrows.Start) || (arrows == UserLink.EndArrows.Both))
					{
						var degrees = Geometry2D.AngleFromVertical(toPos, fromPos, true);
						UIExtension.ArrowHeads.Draw(graphics, pen, fromPos.X, fromPos.Y, size, PinRadius, degrees);

					}

					if ((arrows == UserLink.EndArrows.Finish) || (arrows == UserLink.EndArrows.Both))
					{
						var degrees = Geometry2D.AngleFromVertical(fromPos, toPos, true);
						UIExtension.ArrowHeads.Draw(graphics, pen, toPos.X, toPos.Y, size, PinRadius, degrees);
					}
				}
			}
		}

		protected void DoPaintNode(Graphics graphics, TaskItem taskItem, Rectangle taskRect, DrawState drawState)
		{
			graphics.SmoothingMode = SmoothingMode.None;

			bool selected = drawState.HasFlag(DrawState.Selected);
			bool dropHighlight = drawState.HasFlag(DrawState.DropHighlighted);
			bool dragImage = drawState.HasFlag(DrawState.DragImage);

			if (dragImage)
				taskRect.Offset(-taskRect.Left, -taskRect.Top);

			// Figure out the required colours
			Color backColor = GetTaskBackgroundColor(taskItem, selected);
			Color borderColor = GetTaskBorderColor(taskItem, selected);
			Color textColor = GetTaskTextColor(taskItem, drawState);

			// Draw background
			if (selected)
			{
				UIExtension.SelectionRect.Style style = (Focused ? UIExtension.SelectionRect.Style.Selected : UIExtension.SelectionRect.Style.SelectedNotFocused);

				UIExtension.SelectionRect.Draw(Handle,
												graphics,
												taskRect.X,
												taskRect.Y,
												taskRect.Width,
												taskRect.Height,
												style,
												false); // opaque

				backColor = UIExtension.SelectionRect.GetColor(style);
			}
			else if (dropHighlight)
			{
				UIExtension.SelectionRect.Draw(Handle,
												graphics,
												taskRect.X,
												taskRect.Y,
												taskRect.Width,
												taskRect.Height,
												UIExtension.SelectionRect.Style.DropHighlighted,
												false); // opaque

				backColor = UIExtension.SelectionRect.GetColor(UIExtension.SelectionRect.Style.DropHighlighted);
			}
			else if (backColor != Color.Empty)
			{
				using (var brush = new SolidBrush(backColor))
					graphics.FillRectangle(brush, taskRect);
			}
			else
			{
				graphics.FillRectangle(SystemBrushes.Window, taskRect);
				backColor = SystemColors.Window;
			}

			// Optional border
			if (borderColor != Color.Empty)
			{
				// Pens behave weirdly
				taskRect.Width--;
				taskRect.Height--;

				using (var pen = new Pen(borderColor, 0f))
					graphics.DrawRectangle(pen, taskRect);
			}

			// Icon
			var titleRect = taskRect;

			if (TaskHasIcon(taskItem))
			{
				var iconRect = CalcIconRect(taskRect);

				if (m_TaskIcons.Get(taskItem.TaskId))
				{
					if (!IsZoomed)
					{
						m_TaskIcons.Draw(graphics, iconRect.X, iconRect.Y);
					}
					else
					{
						int imageSize = ScaleByDPIFactor(16);

						using (var tempImage = new Bitmap(imageSize, imageSize, PixelFormat.Format32bppRgb)) // original size
						{
							tempImage.MakeTransparent();
							using (var gTemp = Graphics.FromImage(tempImage))
							{
								gTemp.Clear(backColor);
								m_TaskIcons.Draw(gTemp, 0, 0);

								DrawZoomedIcon(tempImage, graphics, iconRect, taskRect);
							}
						}
					}
				}

				titleRect.Width = (titleRect.Right - iconRect.Right);
				titleRect.X = iconRect.Right;
			}

			// Title
			titleRect.Inflate(-LabelPadding, -LabelPadding);
			graphics.DrawString(taskItem.ToString(), GetTaskLabelFont(taskItem), new SolidBrush(textColor), titleRect);

			/*
			// Checkbox
			Rectangle checkRect = CalcCheckboxRect(rect);

			if (ShowCompletionCheckboxes)
			{
				if (!IsZoomed)
				{
					CheckBoxRenderer.DrawCheckBox(graphics, checkRect.Location, GetNodeCheckboxState(realNode));
				}
				else
				{
					var tempImage = new Bitmap(CheckboxSize.Width, CheckboxSize.Height); // original size

					using (var gTemp = Graphics.FromImage(tempImage))
					{
						CheckBoxRenderer.DrawCheckBox(gTemp, new Point(0, 0), GetNodeCheckboxState(realNode));

						DrawZoomedImage(tempImage, graphics, checkRect);
					}
				}
			}

			else if (ShowCompletionCheckboxes)
			{
				rect.Width = (rect.Right - checkRect.Right - 2);
				rect.X = checkRect.Right + 2;
			}
			*/

			// Image
			if (taskItem.Image != null)
			{
				var imageRect = CalcImageRect(taskItem, taskRect, selected || dropHighlight);

				graphics.DrawImage(taskItem.Image, imageRect);
			}

			graphics.SmoothingMode = SmoothingMode.AntiAlias;
		}

		protected override void DrawSelectionBox(Graphics graphics, Rectangle rect)
		{
			Debug.Assert(DragMode == DragMode.SelectionBox);

			if (VisualStyleRenderer.IsSupported)
			{
				using (var brush = new SolidBrush(Color.FromArgb(96, SystemColors.Highlight)))
					graphics.FillRectangle(brush, rect);

				graphics.DrawRectangle(SystemPens.Highlight, rect);
			}
			else
			{
				base.DrawSelectionBox(graphics, rect);
			}
		}

		private Rectangle CalcImageRect(TaskItem task, Rectangle nodeRect, bool selected)
		{
			nodeRect.Inflate(-1, -1);

			if (!selected)
			{
				nodeRect.Width++;
				nodeRect.Height++;
			}

			int height = task.CalcImageHeight(nodeRect.Width);

			return new Rectangle(nodeRect.X, nodeRect.Bottom - height, nodeRect.Width, height);
		}

		protected void DrawZoomedIcon(Image image, Graphics graphics, Rectangle destRect, Rectangle clipRect)
		{
			Debug.Assert(IsZoomed);

			var gSave = graphics.Save();

			var attrib = new ImageAttributes();
			attrib.SetWrapMode(WrapMode.TileFlipXY);

			graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
			graphics.SmoothingMode = SmoothingMode.HighQuality;

			graphics.IntersectClip(clipRect);
			graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, attrib);
			graphics.Restore(gSave);
		}

		private bool TaskHasIcon(TaskItem task)
		{
			if ((m_TaskIcons == null) || (task == null))
				return false;

			return (task.HasIcon || (m_ShowParentAsFolder && task.IsParent));
		}

        private Rectangle CalcIconRect(Rectangle labelRect)
		{
            Point topLeft = labelRect.Location;
			topLeft.Offset(1, 1);
            
//             if (ShowCompletionCheckboxes)
//                 left += (int)(CheckboxSize.Width * ImageZoomFactor);

			int width = (int)(ScaleByDPIFactor(16) * ImageZoomFactor);
			int height = width;

            return new Rectangle(topLeft.X, topLeft.Y, width, height);
		}

		private bool SelectedNodeWasPreviouslySelected
		{
			get { return ((SingleSelectedTask != null) && (SingleSelectedTask == m_PreviouslySelectedTask)); }
		}

		private bool HitTestIcon(RadialTree.TreeNode<uint> node, Point point)
        {
			var task = GetTaskItem(node);
			
			if (task.IsLocked || !TaskHasIcon(task))
				return false;

			// else
			return CalcIconRect(GetNodeClientRect(node)).Contains(point);
        }

		protected UserLink HitTestUserLink(Point ptClient)
		{
			return HitTestUserLink(RootNode, ptClient);
		}

		protected UserLink HitTestUserLink(RadialTree.TreeNode<uint> node, Point ptClient)
		{
			var fromTask = GetTaskItem(node.Data);

			if (fromTask?.UserLinks?.Count > 0)
			{
				foreach (var link in fromTask.UserLinks)
				{
					Debug.Assert(node.Data == link.FromId);

					Point fromPos, toPos;
					const double Tolerance = 5.0;

					if (IsConnectionVisible(node, link.ToId, out fromPos, out toPos) && 
						Geometry2D.HitTestSegment(fromPos, toPos, ptClient, Tolerance))
					{
						return link;
					}
				}
			}

			// check children
			foreach (var child in node.Children)
			{
				var hit = HitTestUserLink(child, ptClient);

				if (hit != null)
					return hit;
			}

			return null;
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			m_EditTimer.Stop();
			m_PreviouslySelectedTask = (Focused ? SingleSelectedTask : null);

			// Check for connection first to simplify logic
			if (!ReadOnly)
			{
				var link = HitTestUserLink(e.Location);

				if ((link != null) && !m_TaskItems.IsTaskLocked(link.FromId))
				{
					// Cache the link to be checked in OnMouseClick
					m_SelectedUserLink = link;
					return;
				}
				else if (HasSelectedUserLink)
				{
					ClearUserLinkSelection();
				}
			}

			base.OnMouseDown(e);
		}

		protected override void OnMouseDoubleClick(MouseEventArgs e)
		{
			var hit = HitTestNode(e.Location, true);

			if (hit != null)
			{
				var task = GetTaskItem(hit);
				var imageRect = CalcImageRect(task, GetNodeClientRect(hit), false);

				if (imageRect.Contains(e.Location))
					Process.Start(task.ImagePath);
				else
					EditTaskLabel(this, SingleSelectedNode.Data);
			}
			else
			{
				var link = HitTestUserLink(e.Location);

				if (link != null)
					DoubleClickUserLink?.Invoke(this, null);
			}
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			if (e.Button != MouseButtons.Left)
				return;

			// Check for connection first to simplify logic
			if (!ReadOnly)
			{
				var link = HitTestUserLink(e.Location);

				if ((link != null) && (link == m_SelectedUserLink))
				{
					// Set the owning node first because that will clear the selected connection
					SelectNode(link.FromId, true);
					Invalidate();

					m_SelectedUserLink = link;
					UserLinkSelectionChange?.Invoke(this, null);

					return;
				}
				else if (HasSelectedUserLink)
				{
					ClearUserLinkSelection();
				}
			}

			base.OnMouseClick(e);

			// 			TaskNode task = SelectedTaskNode;
			// 
			// 			if (task == null)
			// 				return;
			// 
			// 			if (!ReadOnly && !task.IsLocked && (HitTestTask(e.Location) == task))
			// 			{
			// /*
			// 				if (HitTestCheckbox(node, e.Location))
			// 				{
			// 					if (EditTaskDone != null)
			// 						EditTaskDone(this, task.ID, !task.IsDone(false));
			// 				}
			// 				else*/ if (HitTestIcon(SelectedNode, e.Location))
			// 				{
			// 					if (EditTaskIcon != null)
			// 					    EditTaskIcon(this, task.TaskId);
			// 				}
			// 				else if (SelectedNodeWasPreviouslySelected)
			// 				{
			// 					if (EditTaskLabel != null)
			// 						m_EditTimer.Start();
			// 				}
			// 			}
		}


		private void OnEditLabelTimer(object sender, EventArgs e)
		{
			m_EditTimer.Stop();

			EditTaskLabel?.Invoke(this, SingleSelectedNode?.Data ?? 0);
		}

		protected TaskItem HitTestTask(Point ptClient)
		{
			var node = HitTestNode(ptClient, true);

			return GetTaskItem(node);
		}

		protected TaskItem GetTaskItem(RadialTree.TreeNode<uint> node)
		{
			return (node == null) ? null : GetTaskItem(node.Data);
		}

		public TaskItem GetTaskItem(uint nodeId)
		{
			return m_TaskItems.GetTask(nodeId);
		}

		protected override bool IsAcceptableDragSource(RadialTree.TreeNode<uint> node)
		{
			return (base.IsAcceptableDragSource(node) && (node != RootNode));
		}

		protected bool OnDragDrop(object sender, IList<uint> nodeIds)
		{
			// Update the tasks
			foreach (uint nodeId in nodeIds)
			{
				var node = GetNode(nodeId);
				var task = GetTaskItem(nodeId);

				if ((node != null) && (task != null))
				{
					task.UserPosition = node.Point.GetPosition();
				}
			}

			TaskModified?.Invoke(this, nodeIds);
			return true;
		}

		protected bool SelectedTasksAreAllEditable
		{
			get
			{
				foreach (var id in SelectedNodeIds)
				{
					var task = GetTaskItem(id);

					if ((task == null) || task.IsLocked)
						return false;
				}

				return true;
			}
		}

		protected Cursor GetUserLinkCursor(Point ptClient)
		{
			Cursor cursor = GetSelectedUserLinkCursor(ptClient);

			if (cursor == null)
			{
				var link = HitTestUserLink(ptClient);

				if ((link != null) && m_TaskItems.IsTaskLocked(link.FromId))
					cursor = UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);
			}

			return cursor;			
		}

		protected Cursor GetSelectedUserLinkCursor(Point ptClient)
		{
			if (HasSelectedUserLink)
			{
				Point fromPos, toPos;

				if (IsConnectionVisible(m_SelectedUserLink, out fromPos, out toPos))
				{
					// Check for pin ends of selected link
					if (GetPinRect(fromPos).Contains(ptClient))
						return UIExtension.AppCursor(UIExtension.AppCursorType.NoDrag);

					if (GetPinRect(toPos).Contains(ptClient))
						return Cursors.SizeAll;
				}
			}

			return null;
		}

		protected Cursor GetNodeCursor(Point ptClient)
		{
			var node = HitTestNode(ptClient, true); // exclude root node

			if (node != null)
			{
				var task = GetTaskItem(node.Data);

				if (task != null)
				{
					if (task.IsLocked)
						return UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);

					if (TaskHasIcon(task) && HitTestIcon(node, ptClient))
						return UIExtension.HandCursor();
				}
			}
			else if (m_Options.HasFlag(DetectiveBoardOption.ShowRootNode))
			{
				if (HitTestNode(ptClient) == RootNode)
					return UIExtension.AppCursor(UIExtension.AppCursorType.NoDrag);
			}

			return null;
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
 			base.OnMouseMove(e);

			Cursor cursor = null;

			if (!ReadOnly)
			{
				if (DrawNodesOnTop)
				{
					// Selected link ends overlap node edges
					// so they take precedent
					cursor = GetSelectedUserLinkCursor(e.Location);

					if (cursor == null)
						cursor = GetNodeCursor(e.Location);

					if (cursor == null)
						cursor = GetUserLinkCursor(e.Location);
				}
				else
				{
					cursor = GetUserLinkCursor(e.Location);

					if (cursor == null)
						cursor = GetNodeCursor(e.Location);
				}
			}

			Cursor = ((cursor != null) ? cursor : Cursors.Arrow);
		}

/*
		protected override void OnDragOver(DragEventArgs e)
		{
			var hitTest = DragHitTest(e);
			bool segChange = !NodeHitTestResult.Match(hitTest, DropPos);

			if (segChange)
				m_DragImage.ShowNoLock(false);

			base.OnDragOver(e);

			if (segChange)
				m_DragImage.ShowNoLock(true);

			m_DragImage.Move(e.X, e.Y);
		}

		protected override void OnDragEnter(DragEventArgs e)
		{
			base.OnDragEnter(e);

			m_DragImage.Begin(Handle, 
								this, 
								SelectedNode, 
								NodeWidth, 
								NodeHeight, 
								NodeWidth / 2,          // Middle of task
								-DPIScaling.Scale(16)); // below the cursor

			m_LastDragPos = PointToClient(new Point(e.X, e.Y));
		}
		protected override void DoDragCleanUp()
		{
			base.DoDragCleanUp();

			m_DragImage.End();
		}

		override protected bool IsAcceptableDropTarget(NodeDragEventArgs e)
		{
			if (!base.IsAcceptableDropTarget(e))
				return false;

			return !(e.DraggedNode as TaskNode).IsLocked;
		}

		override protected bool IsAcceptableDragSource(Node node)
		{
			if (!base.IsAcceptableDragSource(node))
				return false;

			return !(node as TaskNode).IsLocked;
		}

		override protected bool DoDrop(NodeDragEventArgs e)
		{
			m_DragImage.End();
			// TODO

			return base.DoDrop(e);
		}

*/
		public void DrawDragImage(Graphics graphics, Object obj, int width, int height)
		{
// 			DoPaintNode(graphics, 
// 						(obj as TaskNode), 
// 						DrawState.Selected | DrawState.DragImage);
		}

	}
}

