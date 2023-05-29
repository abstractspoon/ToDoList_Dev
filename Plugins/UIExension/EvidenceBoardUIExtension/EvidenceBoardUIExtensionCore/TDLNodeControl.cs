using System;
using System.Linq;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

using BaseNode = RadialTree.TreeNode<uint>;

namespace EvidenceBoardUIExtension
{
    public delegate bool EditTaskLabelEventHandler(object sender, uint taskId);
    public delegate bool EditTaskIconEventHandler(object sender, uint taskId);
	public delegate bool EditTaskCompletionEventHandler(object sender, uint taskId, bool completed);

	public delegate bool TaskModifiedEventHandler(object sender, IList<uint> taskIds);
	public delegate bool ConnectionCreatedEventHandler(object sender, UserLink link);
	public delegate bool ConnectionEditedEventHandler(object sender, UserLink link);
	public delegate bool ConnectionDeletedEventHandler(object sender, uint taskId);

	// ------------------------------------------------------------

	[Flags]
	enum EvidenceBoardOption
	{
		None			= 0x00,

		// deprecated
		// deprecated
		ShowRootNode	= 0x04,
		DrawLinksOnTop	= 0x08,
		DrawPins		= 0x10,
	}

	// ------------------------------------------------------------

	[Flags]
	enum EvidenceBoardLinkType
	{
		None,

		ParentChild,
		Dependency,
		User,
	}

	// ------------------------------------------------------------

	[System.ComponentModel.DesignerCategory("")]
	class TDLNodeControl : NodeControl
	{
		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;

		public event TaskModifiedEventHandler TaskModified;
		public event ConnectionCreatedEventHandler ConnectionCreated;
		public event ConnectionEditedEventHandler ConnectionEdited;
		public event ConnectionDeletedEventHandler ConnectionDeleted;

		public EventHandler UserLinkSelectionChange;
		public EventHandler DoubleClickUserLink;

		// -------------------------------------------------------------------------

		public class LinkType
		{
			public LinkType(string name, EvidenceBoardLinkType type)
			{
				Name = name;
				Type = type;
			}

			public EvidenceBoardLinkType Type { get; private set; }
			public string Name { get; private set; }
		}
		
		// -------------------------------------------------------------------------

		const EvidenceBoardOption DefaultOptions = (EvidenceBoardOption.ShowRootNode |
													EvidenceBoardOption.DrawLinksOnTop |
													EvidenceBoardOption.DrawPins);

		// -------------------------------------------------------------------------

		protected int LabelPadding { get { return ScaleByDPIFactor(2); } }
		protected int DefaultPinRadius { get { return ScaleByDPIFactor(3); } }
		protected int LinkOffset { get { return ScaleByDPIFactor(6); } }

		// -------------------------------------------------------------------------

		// From Parent
		private Translator m_Trans;
		private UIExtension.TaskIcon m_TaskIcons;

		private bool m_ShowParentAsFolder = false;
		private bool m_TaskColorIsBkgnd = false;
		private bool m_StrikeThruDone = true;
		private bool m_ShowCompletionCheckboxes = true;

		private EvidenceBoardOption m_Options = EvidenceBoardOption.None;
		private List<LinkType> m_VisibleLinkTypes;
		private HashSet<string> m_UserLinkTypes;

		private Timer m_EditTimer;
		private Font m_BoldLabelFont, m_DoneLabelFont, m_BoldDoneLabelFont;
		private Color m_DependencyColor;

		private TaskItem m_PreviouslySelectedTask;
		private UserLink m_SelectedUserLink;
		private uint m_DropHighlightedTaskId, m_HotTaskId;

		private bool m_DraggingSelectedUserLink = false;
		private Point m_DraggedUserLinkEnd = Point.Empty;

		private TaskItems m_TaskItems;
		private Tuple<DateTime, DateTime> m_SelectedDateRange;

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
			ClearSelectedDateRange();

			m_Trans = trans;
			m_TaskIcons = icons;

			m_EditTimer = new Timer();
			m_EditTimer.Interval = 500;
			m_EditTimer.Tick += new EventHandler(OnEditLabelTimer);

			Options = DefaultOptions;

			m_TaskItems = null;
			m_DependencyColor = Color.CornflowerBlue;

			int nodeHeight = (int)Math.Max((2 * BaseFontHeight), (BaseFontHeight + UIExtension.TaskIcon.IconSize)) + (3 * LabelPadding);
			int nodeWidth  = (4 * nodeHeight);

			base.NodeSize = new Size(nodeWidth, nodeHeight);
			base.PinRadius = DefaultPinRadius;

			DragDropChange += new NodeDragDropChangeEventHandler(OnDragDropNodes);
			NodeSelectionChange += (s, ids) => { ClearUserLinkSelection(); };

			RebuildFonts();

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

		public bool GetMinMaxDateRange(out DateTime from, out DateTime to)
		{
			return m_TaskItems.GetDateRange(out from, out to);
		}

		public bool SetSelectedDateRange(DateTime from, DateTime to)
		{
			if (from >= to)
				return false;

			m_SelectedDateRange = new Tuple<DateTime, DateTime>(from, to);
			Invalidate();

			return true;
		}

		public void ClearSelectedDateRange()
		{
			m_SelectedDateRange = new Tuple<DateTime, DateTime>(DateTime.MinValue, DateTime.MaxValue);
			Invalidate();
		}

		override protected bool IsNodeVisible(BaseNode node)
		{
			if (!base.IsNodeVisible(node))
				return false;

			if ((m_SelectedDateRange.Item1 == DateTime.MinValue) && (m_SelectedDateRange.Item2 == DateTime.MaxValue))
				return true;
#if DEBUG
			if ((node.Data == 0) && m_Options.HasFlag(EvidenceBoardOption.ShowRootNode))
				return true;
#endif

			var taskItem = GetTaskItem(node.Data);

			return (taskItem?.IntersectsWith(m_SelectedDateRange.Item1, m_SelectedDateRange.Item2) == true);
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

		public IList<LinkType> VisibleLinkTypes
		{
			get { return m_VisibleLinkTypes; }

			set
			{
				if ((value == null) && (m_VisibleLinkTypes == null))
					return;

				if (value == null)
				{
					m_VisibleLinkTypes = null;
				}
				else
				{
					if (m_VisibleLinkTypes != null)
						m_VisibleLinkTypes.Clear();
					else
						m_VisibleLinkTypes = new List<LinkType>();

					m_VisibleLinkTypes.AddRange(value);
				}
				Invalidate();
			}
		}

		bool ShowingDependencyLinks
		{
			get { return (m_VisibleLinkTypes?.Find(x => (x.Type == EvidenceBoardLinkType.Dependency)) != null); }
		}

		bool ShowingParentChildLinks
		{
			get { return (m_VisibleLinkTypes?.Find(x => (x.Type == EvidenceBoardLinkType.ParentChild)) != null); }
		}

		bool ShowingUntypedLinks
		{
			get { return (m_VisibleLinkTypes?.Find(x => (x.Type == EvidenceBoardLinkType.None)) != null); }
		}

		bool ShowingUserLinkType(string type)
		{
			if (String.IsNullOrWhiteSpace(type))
				return ShowingUntypedLinks;

			// else
			return (m_VisibleLinkTypes?.Find(x => ((x.Type == EvidenceBoardLinkType.User) && (x.Name == type))) != null);
		}

		public Color DependencyColor
		{
			get { return m_DependencyColor; }

			set
			{
				if (value != m_DependencyColor)
				{
					m_DependencyColor = value;

					if (ShowingDependencyLinks)
						Invalidate();
				}
			}
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			switch (type)
			{
			case UIExtension.UpdateType.Edit:
			case UIExtension.UpdateType.New:
				UpdateTaskAttributes(tasks, false);
				break;

			case UIExtension.UpdateType.Delete:
			case UIExtension.UpdateType.All:
				UpdateTaskAttributes(tasks, true);
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

		private void ApplyUserPositions(BaseNode node)
		{
			var task = GetTaskItem(node);

			if ((task != null) && task.HasUserPosition)
				node.Point.SetPosition(task.UserPosition);

			// children
			foreach (var child in node.Children)
				ApplyUserPositions(child);
		}
		
		public EvidenceBoardOption Options
		{
			get { return m_Options; }
		
			set
			{
				if (value != m_Options)
				{
					m_Options = value;

					base.DrawNodesOnTop = !m_Options.HasFlag(EvidenceBoardOption.DrawLinksOnTop);
					base.PinRadius = (m_Options.HasFlag(EvidenceBoardOption.DrawPins) ? DefaultPinRadius : 0);

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
			//get { return (ZoomFactor + ((1.0f - ZoomFactor) / 2)); }
			get { return ZoomFactor; }
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

		public bool UserLinkExists(uint fromId, uint toId)
		{
			return m_TaskItems.HasUserLink(fromId, toId);
		}

		public bool CanCreateUserLink(uint fromId, uint toId)
		{
			if (ReadOnly)
				return false;

			if (m_TaskItems.IsTaskLocked(fromId))
				return false;
			
			return !UserLinkExists(fromId, toId);
		}

		public UserLink SelectedUserLink
		{
			get
			{
				if (ReadOnly)
					return null;

				if ((SingleSelectedNode == null) || (m_SelectedUserLink == null))
					return null;

				if (SingleSelectedNode.Data != m_SelectedUserLink.FromId)
				{
					Debug.Assert(false);
					return null;
				}

				if (m_TaskItems.IsTaskLocked(m_SelectedUserLink.FromId))
					return null;

				return m_SelectedUserLink;
			}

			set
			{
				if ((value != null) && m_TaskItems.HasUserLink(value))
					SelectUserLink(value);
				else
					ClearUserLinkSelection();
			}
		}

		public bool HasSelectedUserLink { get { return (SelectedUserLink != null); } }

		public void ClearUserLinkSelection()
		{
			if (HasSelectedUserLink)
			{
				ResetUserLinkDrag();

				UserLinkSelectionChange?.Invoke(this, null);
			}
		}
		
		public UserLink CreateUserLink(uint fromId, uint toId, UserLinkAttributes attrib)
		{
			var fromTask = GetTaskItem(fromId);

			if (fromTask != null)
			{
				// task must be the only selection
				if (!SelectNode(fromId, true, false))
				{
					Debug.Assert(false);
					return null;
				}

				var newLink = fromTask.AddUserLink(toId, attrib);

				if (newLink != null)
				{
					m_UserLinkTypes.Add(attrib.Type);
					Invalidate();

					return newLink;
				}
			}

			return null;
		}

		public bool EditSelectedUserLink(UserLinkAttributes attrib, UserLinkAttributes.Mask mask = UserLinkAttributes.Mask.All, bool modAllOfSameType = false)
		{
			if (!HasSelectedUserLink)
				return false;

			if (mask == UserLinkAttributes.Mask.None)
				return false;

			if (!modAllOfSameType && m_SelectedUserLink.Attributes.Matches(attrib, mask))
				return false;

			// Select all the tasks having a user link of the specified type
			bool changes = false;

			if (modAllOfSameType)
			{
				var ids = new List<uint>();
				var matchingTasks = m_TaskItems.Values.Where(x => x.HasUserLink(attrib.Type));

				if (matchingTasks.Count() > 1)
				{
					m_SelectedUserLink = null;

					foreach (var taskItem in matchingTasks)
					{
						ids.Add(taskItem.TaskId);

						var matchingLinks = taskItem.UserLinks.Where(x => (x.Attributes.Type == attrib.Type));

						foreach (var link in matchingLinks)
							changes |= link.Attributes.Copy(attrib, mask);
					}

					if (changes)
					{
						base.SelectNodes(ids, true);
						return (TaskModified?.Invoke(this, ids) == true);
					}

					// else
					return false;
				}
			}
			
			changes = m_SelectedUserLink.Attributes.Copy(attrib, mask);

			if (changes)
			{
				m_UserLinkTypes.Add(attrib.Type);
				ConnectionEdited?.Invoke(this, m_SelectedUserLink);
			}

			ClearUserLinkSelection();

			return changes;
		}

		public bool DeleteSelectedUserLink()
		{
			if (!HasSelectedUserLink)
				return false;

			if (!m_TaskItems.DeleteUserLink(m_SelectedUserLink))
				return false;

			ConnectionDeleted?.Invoke(this, m_SelectedUserLink.FromId);

			ClearUserLinkSelection();
			return true;
		}

		public bool SelectTask(uint taskId)
		{
			return SelectNode(taskId, false, true);
		}

		public new bool SelectNode(uint taskId, bool notify, bool ensureVisible)
		{
			ClearUserLinkSelection();

			if (base.SelectNode(taskId, notify, ensureVisible))
				return true;

			base.SelectNode(NodeControl.NullId, notify, false);
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

		private void UpdateTaskAttributes(TaskList tasks, bool rebuild)
		{
			BaseNode rootNode = base.RootNode;

			if (rebuild)
			{
				m_UserLinkTypes = null;
				m_TaskItems = new TaskItems();

				rootNode = new BaseNode(0);
			}

			Task task = tasks.GetFirstTask();

			while (task.IsValid())
			{
				ProcessTaskUpdate(task, rootNode);
				task = task.GetNextTask();
			}

			base.EnableLayoutUpdates = false;
			base.RadialIncrementOrSpacing = NodeSize.Width;

			// If the root only has one task, make the initial radius close to zero
			// to place the only node in the position of the root node
			if (rootNode.Count == 1)
				base.InitialRadius = 0.1f;
			else
				base.InitialRadius = (float)((rootNode.Count * NodeSize.Width) / (2 * Math.PI));

			base.RootNode = rootNode;
			base.EnableLayoutUpdates = true;

			Invalidate();
		}

		public IEnumerable<string> UserLinkTypes
		{
			get
			{
				if (m_UserLinkTypes == null)
				{
					m_UserLinkTypes = new HashSet<string>();

					foreach (var taskItem in m_TaskItems.Values)
					{
						if (taskItem.HasUserLinks)
						{
							foreach (var link in taskItem.UserLinks)
								m_UserLinkTypes.Add(link.Attributes.Type);
						}
					}
				}
				
				return m_UserLinkTypes;
			}
		}

		private bool ProcessTaskUpdate(Task task, BaseNode parentNode)
		{
			if (!task.IsValid() || task.IsReference())
				return false;

			uint taskId = task.GetID();
			var taskItem = m_TaskItems.GetTaskItem(taskId);
			BaseNode node = null;

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

		protected override Size GetNodeSize(BaseNode node)
		{
			var size = base.GetNodeSize(node);

			if (node == RootNode)
				return new Size(size.Width / 2, size.Width / 2); // smaller square

			var task = GetTaskItem(node);

			if ((task != null) && (task.Image != null) && task.HasExpandedImage)
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

		protected override bool IsSelectableNode(BaseNode node)
		{
			return (base.IsSelectableNode(node) && (node.Data > 0));
		}

		protected override void OnAfterDrawConnections(Graphics graphics)
		{
			DrawTaskDependencies(graphics, RootNode);
			DrawUserLinks(graphics, RootNode);
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			if (HasSelectedUserLink)
			{
				DrawSelectedUserLink(e.Graphics);
			}

			if (!m_DraggingSelectedUserLink && 
				(m_HotTaskId != 0) && 
				!ModifierKeys.HasFlag(Keys.Control) &&
				(DragMode == DragMode.None))
			{
				// Draw a temporary 'pin' for initiating a new user link
				var hotNode = GetNode(m_HotTaskId);

				if (IsAcceptableDragSource(hotNode))
					DrawSelectionPin(e.Graphics, GetCreateLinkPinPos(hotNode), true);
			}
		}

		private Rectangle GetSelectionPinRect(Point pos)
		{
			// Always the unzoomed size
			return Geometry2D.GetCentredRect(pos, (DefaultPinRadius * 2));
		}

		protected void DrawTaskDependencies(Graphics graphics, BaseNode node)
		{
 			if (!ShowingDependencyLinks)
				return;

			var taskItem = GetTaskItem(node);

			if (taskItem?.DependIds?.Count > 0)
			{
				foreach (var dependId in taskItem.DependIds)
				{
					Point fromPos, toPos;

					// Don't draw a dependency which is overlaid by a user link
					if (/*!m_TaskItems.HasUserLink(taskItem.TaskId, dependId, true) &&*/
						IsConnectionVisible(node, dependId, out fromPos, out toPos, false))
					{
						using (var pen = new Pen(DependencyColor))
						{
							using (var brush = new SolidBrush(DependencyColor))
							{
								DrawConnection(graphics, Pens.Blue, Brushes.Blue, fromPos, toPos);
								DrawConnectionArrows(graphics, UserLinkAttributes.EndArrows.Start, 2, DependencyColor, fromPos, toPos, (PinRadius + 1));
							}
						}
					}
				}
			}

			// Children
			foreach (var child in node.Children)
				DrawTaskDependencies(graphics, child);
		}

		protected void DrawSelectedUserLink(Graphics graphics)
		{
			DrawUserLink(graphics, m_SelectedUserLink, true);
		}

		protected void DrawUserLinks(Graphics graphics, BaseNode node)
		{
			var taskItem = GetTaskItem(node);

			if ((taskItem != null) && taskItem.HasUserLinks)
			{
				foreach (var link in taskItem.UserLinks)
					DrawUserLink(graphics, link, false);
			}

			// Children
			foreach (var child in node.Children)
				DrawUserLinks(graphics, child);
		}

		protected void DrawUserLink(Graphics graphics, UserLink link, bool selected)
		{
			if (selected && !HasSelectedUserLink)
				return;

			if (selected && (link != m_SelectedUserLink))
				return;

			if (!selected && (link == m_SelectedUserLink))
				return;

			Point fromPos, toPos;

			if (IsConnectionVisible(link, out fromPos, out toPos) || (selected && m_DraggingSelectedUserLink))
			{
				if (selected && m_DraggingSelectedUserLink)
				{
					var node = GetNode(link.FromId);

					if (m_HotTaskId == link.FromId)
						fromPos = GetCreateLinkPinPos(node);

					toPos = m_DraggedUserLinkEnd;
				}

				var lineThickness = (selected ? 2 : link.Attributes.Thickness);
				var arrowThickness = Math.Max(2, lineThickness);
				var arrowOffset = (selected ? (DefaultPinRadius + 2) : PinRadius);
				var arrowSize = UIExtension.DependencyArrows.Size(TextFont);

				if (selected)
				{
					using (var pen = new Pen(SystemColors.WindowText, 2))
						DrawConnection(graphics, pen, null, fromPos, toPos);

					// Draw special pins
					DrawSelectionPin(graphics, fromPos, false);
					DrawSelectionPin(graphics, toPos, true);

					DrawConnectionArrows(graphics, link.Attributes.Arrows, arrowThickness, SystemColors.WindowText, fromPos, toPos, arrowOffset);
				}
				else
				{
					using (var pen = new Pen(link.Attributes.Color, link.Attributes.Thickness))
					{
						using (var brush = new SolidBrush(link.Attributes.Color))
						{
							DrawConnection(graphics, pen, brush, fromPos, toPos);
						}
					}

					DrawConnectionArrows(graphics, link.Attributes.Arrows, arrowThickness, link.Attributes.Color, fromPos, toPos, arrowOffset);
				}

				if (!string.IsNullOrWhiteSpace(link.Attributes.Label))
				{
					var matrix = new Matrix();
					var textOffset = Geometry2D.MidPoint(fromPos, toPos);
					float textAngle = Geometry2D.BestTextAngleInDegrees(fromPos, toPos, Geometry2D.AngleAxis.FromHorizontal);

					// This is a little trick to determine whether to place the line text above or below the line
					float lineAngle = Geometry2D.DegreesBetween(fromPos, toPos, Geometry2D.AngleAxis.FromHorizontal);

					var format = new StringFormat()
					{
						Alignment = StringAlignment.Center,
						LineAlignment = ((lineAngle == textAngle) ? StringAlignment.Near : StringAlignment.Far)
					};

					matrix.Rotate(textAngle, MatrixOrder.Append);
					matrix.Translate(textOffset.X, textOffset.Y, MatrixOrder.Append);

					graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
					graphics.Transform = matrix;
					graphics.DrawString(link.Attributes.Label, TextFont, SystemBrushes.WindowText/*new SolidBrush(color)*/, Point.Empty, format);
					graphics.ResetTransform();
				}
			}
		}

		void DrawSelectionPin(Graphics graphics, Point pos, bool draggable)
		{
			var pin = GetSelectionPinRect(pos);

			if (draggable)
			{
				// Draw a box
				graphics.FillRectangle(SystemBrushes.Window, pin);
				graphics.DrawRectangle(SystemPens.WindowText, pin);

			}
			else
			{
				// Draw circle
				graphics.FillEllipse(SystemBrushes.Window, pin);
				graphics.DrawEllipse(SystemPens.WindowText, pin);
			}
		}

		private Point GetCreateLinkPinPos(BaseNode node)
		{
			var pos = Geometry2D.Centroid(GetNodeClientRect(node));

			// Offset the hit rect left or right to avoid existing user links
			if (!DrawNodesOnTop && m_TaskItems.HasUserLink(node.Data))
			{
				var graphCentre = Geometry2D.Centroid(GraphToClient(Extents));
				int offset = (DefaultPinRadius * 4);

				if (pos.X < graphCentre.X)
					pos.Offset(-offset, 0);
				else
					pos.Offset(offset, 0);
			}

			return pos;
		}

		private Rectangle GetCreateLinkPinRect(BaseNode node)
		{
			return GetSelectionPinRect(GetCreateLinkPinPos(node));
		}

		DrawState GetDrawState(BaseNode node, bool selected)
		{
			if (selected)
				return DrawState.Selected;

			if (m_DraggingSelectedUserLink && (m_DropHighlightedTaskId == node.Data))
				return DrawState.DropHighlighted;

			// all else
			return DrawState.None;
		}

		protected override void DrawNode(Graphics graphics, BaseNode node, Rectangle rect, bool selected)
		{
			var taskItem = m_TaskItems.GetTaskItem(node.Data);

			if (taskItem != null)
			{
				DoPaintNode(graphics, taskItem, rect, GetDrawState(node, selected));
			}
#if DEBUG
			else if (m_Options.HasFlag(EvidenceBoardOption.ShowRootNode))
			{
				base.DrawNode(graphics, node, rect, selected);
			}
#endif
		}

		protected override void DrawParentAndChildConnections(Graphics graphics, BaseNode node)
		{
			if (ShowingParentChildLinks)
				base.DrawParentAndChildConnections(graphics, node);
		}

		protected bool IsConnectionVisible(BaseNode fromNode, uint toId, out Point fromPos, out Point toPos, bool userLink)
		{
			var toNode = GetNode(toId);

			if (!userLink)
				return base.IsConnectionVisible(fromNode, toNode, out fromPos, out toPos);

			if (DrawNodesOnTop)
			{
				// If the reverse link exists then we need to offset 'our' ends
				if (UserLinkExists(toId, fromNode.Data))
				{
					// need to offset BEFORE clipping
					fromPos = Geometry2D.Centroid(GetNodeClientRect(fromNode));
					toPos = Geometry2D.Centroid(GetNodeClientRect(toNode));

					if (!Geometry2D.OffsetLine(ref fromPos, ref toPos, LinkOffset))
						return false;

					ClipLineToNodeBounds(fromNode, toNode, ref fromPos, ref toPos);

					return base.IsConnectionVisible(fromPos, toPos);
				}

				// else
				return base.IsConnectionVisible(fromNode, toNode, out fromPos, out toPos);
			}

			// else
			if (!base.IsConnectionVisible(fromNode, toNode, out fromPos, out toPos))
				return false;

			if (UserLinkExists(toId, fromNode.Data) && 
				!Geometry2D.OffsetLine(ref fromPos, ref toPos, LinkOffset))
			{
				return false;
			}

			return true;
		}

		protected bool IsConnectionVisible(UserLink link, out Point fromPos, out Point toPos)
		{
			bool visible = ((link == m_SelectedUserLink) || ShowingUserLinkType(link.Attributes.Type));

			if (!visible)
			{
				fromPos = toPos = Point.Empty;
				return false;
			}

			return IsConnectionVisible(GetNode(link.FromId), link.ToId, out fromPos, out toPos, true);
		}

		protected override void DrawParentConnection(Graphics graphics, uint nodeId, Point nodePos, Point parentPos)
		{
			if (!ShowingParentChildLinks)
				return;

			var taskItem = m_TaskItems.GetTaskItem(nodeId);
			var parentId = taskItem?.ParentId;

			if (parentId == 0)
			{
#if DEBUG
				if (!m_Options.HasFlag(EvidenceBoardOption.ShowRootNode))
					return;
#else
				return;
#endif
			}

			// Don't draw parent/child connections if they are
			// overlaid either by dependencies or user links
			// 				if (m_TaskItems.HasDependency(nodeId, taskItem.ParentId)) 
			// 					return;
			// 
			// 				if (m_TaskItems.HasUserLink(nodeId, taskItem.ParentId))
			// 					return;

			using (var pen = new Pen(ParentConnectionColor, 1))
			{
				using (var brush = new SolidBrush(ParentConnectionColor))
				{
					DrawConnection(graphics, pen, brush, nodePos, parentPos);
				}
				DrawConnectionArrows(graphics, UserLinkAttributes.EndArrows.Finish, 2, ParentConnectionColor, nodePos, parentPos, (PinRadius + 1));
			}
		}

		protected void DrawConnectionArrows(Graphics graphics, UserLinkAttributes.EndArrows arrows, int thickness, Color color, Point fromPos, Point toPos, int offset)
		{
			if (arrows != UserLinkAttributes.EndArrows.None)
			{
				using (var pen = new Pen(color, thickness))
				{
					int size = UIExtension.DependencyArrows.Size(TextFont) + thickness;

					if ((arrows == UserLinkAttributes.EndArrows.Start) || (arrows == UserLinkAttributes.EndArrows.Both))
					{
						var degrees = Geometry2D.DegreesBetween(toPos, fromPos, Geometry2D.AngleAxis.FromVertical);
						UIExtension.ArrowHeads.Draw(graphics, pen, fromPos.X, fromPos.Y, size, offset, degrees);
					}

					if ((arrows == UserLinkAttributes.EndArrows.Finish) || (arrows == UserLinkAttributes.EndArrows.Both))
					{
						var degrees = Geometry2D.DegreesBetween(fromPos, toPos, Geometry2D.AngleAxis.FromVertical);
						UIExtension.ArrowHeads.Draw(graphics, pen, toPos.X, toPos.Y, size, offset, degrees);
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
			Color borderColor = GetTaskBorderColor(taskItem, selected || dropHighlight);
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

			// LHS icons
			Rectangle iconRect = Rectangle.Empty;

			DrawTaskIcon(graphics, taskItem, taskRect, backColor, ref iconRect);
			DrawTaskImageExpansionButton(graphics, taskItem, taskRect, backColor, ref iconRect);

			// Title
			var titleRect = taskRect;

			if (!iconRect.IsEmpty)
			{
				titleRect.Width = (titleRect.Right - iconRect.Right);
				titleRect.X = iconRect.Right;
			}

			titleRect.Inflate(-LabelPadding, -LabelPadding);
			graphics.DrawString(taskItem.ToString(), GetTaskLabelFont(taskItem), new SolidBrush(textColor), titleRect);

			// Image
			if ((taskItem.Image != null) && taskItem.HasExpandedImage)
			{
				var imageRect = CalcImageRect(taskItem, taskRect, selected || dropHighlight);

				graphics.DrawImage(taskItem.Image, imageRect);
			}

			graphics.SmoothingMode = SmoothingMode.AntiAlias;
		}

		void DrawTaskIcon(Graphics g, TaskItem taskItem, Rectangle nodeRect, Color backColor, ref Rectangle iconRect)
		{
			if (!TaskHasIcon(taskItem))
				return;

			iconRect = CalcIconRect(nodeRect);

			if (m_TaskIcons.Get(taskItem.TaskId))
			{
				if (!IsZoomed)
				{
					m_TaskIcons.Draw(g, iconRect.X, iconRect.Y);
				}
				else
				{
					int imageSize = UIExtension.TaskIcon.IconSize;

					using (var tempImage = new Bitmap(imageSize, imageSize, PixelFormat.Format32bppRgb)) // original size
					{
						tempImage.MakeTransparent();
						using (var gTemp = Graphics.FromImage(tempImage))
						{
							gTemp.Clear(backColor);
							m_TaskIcons.Draw(gTemp, 0, 0);

							DrawZoomedIcon(tempImage, g, iconRect, nodeRect);
						}
					}
				}
			}
		}

		void DrawTaskImageExpansionButton(Graphics graphics, TaskItem taskItem, Rectangle nodeRect, Color backColor, ref Rectangle iconRect)
		{
			if (taskItem.Image == null)
				return;

			iconRect = CalcImageExpansionButtonRect(nodeRect);

			if (!IsZoomed)
			{
				DrawTaskImageExpansionButton(graphics, iconRect, !taskItem.HasExpandedImage);
			}
			else
			{
				int imageSize = UIExtension.TaskIcon.IconSize;

				using (var tempImage = new Bitmap(imageSize, imageSize, PixelFormat.Format32bppRgb)) // original size
				{
					tempImage.MakeTransparent();

					using (var gTemp = Graphics.FromImage(tempImage))
					{
						var tempRect = new Rectangle(0, 0, imageSize, imageSize);
						gTemp.Clear(backColor);

						DrawTaskImageExpansionButton(gTemp, tempRect, !taskItem.HasExpandedImage);
						DrawZoomedIcon(tempImage, graphics, iconRect, nodeRect);
					}
				}
			}
		}

		static void DrawTaskImageExpansionButton(Graphics graphics, Rectangle rect, bool expand)
		{
			if (VisualStyleRenderer.IsSupported)
			{
				var renderer = new VisualStyleRenderer(expand ? VisualStyleElement.ExplorerBar.NormalGroupExpand.Pressed : VisualStyleElement.ExplorerBar.NormalGroupCollapse.Pressed);
				renderer.DrawBackground(graphics, rect);
			}
			else
			{
				ControlPaint.DrawScrollButton(graphics, rect, (expand ? ScrollButton.Down : ScrollButton.Up), ButtonState.Normal);
			}
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
			topLeft.Offset(2, 2); // border and padding

			int width = (int)(UIExtension.TaskIcon.IconSize * ImageZoomFactor);
			int height = width;

            return new Rectangle(topLeft.X, topLeft.Y, width, height);
		}

        private Rectangle CalcImageExpansionButtonRect(Rectangle labelRect)
		{
			var rect = CalcIconRect(labelRect);
			rect.Y += rect.Height;

            return rect;
		}

		private bool SelectedNodeWasPreviouslySelected
		{
			get { return ((SingleSelectedTask != null) && (SingleSelectedTask == m_PreviouslySelectedTask)); }
		}

		protected UserLink HitTestUserLink(Point ptClient)
		{
			return HitTestUserLink(RootNode, ptClient);
		}

		protected UserLink HitTestUserLink(BaseNode node, Point ptClient)
		{
			var fromTask = GetTaskItem(node.Data);

			if ((fromTask != null) && fromTask.HasUserLinks)
			{
				foreach (var link in fromTask.UserLinks)
				{
					Debug.Assert(node.Data == link.FromId);

					Point fromPos, toPos;
					const double Tolerance = 5.0;

					if (IsConnectionVisible(node, link.ToId, out fromPos, out toPos, true) && 
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
		
		protected UserLink HitTestUserLinkEnds(Point ptClient, ref bool from)
		{
			return HitTestUserLinkEnds(RootNode, ptClient, ref from);
		}

		protected UserLink HitTestUserLinkEnds(BaseNode node, Point ptClient, ref bool from)
		{
			var fromTask = GetTaskItem(node.Data);

			if ((fromTask != null) && !fromTask.IsLocked && fromTask.HasUserLinks)
			{
				foreach (var link in fromTask.UserLinks)
				{
					Debug.Assert(node.Data == link.FromId);

					if (HitTestUserLinkEnds(link, ptClient, ref from))
						return link;
				}
			}

			// check children
			foreach (var child in node.Children)
			{
				var hit = HitTestUserLinkEnds(child, ptClient, ref from);

				if (hit != null)
					return hit;
			}

			return null;
		}

		protected bool HitTestUserLinkEnds(UserLink link, Point ptClient, ref bool from)
		{
			Point fromPos, toPos;

			if (IsConnectionVisible(link, out fromPos, out toPos))
			{
				// Check for pin ends of selected link
				if (Geometry2D.GetCentredRect(fromPos, (DefaultPinRadius * 2)).Contains(ptClient))
				{
					from = true;
					return true;
				}

				if (Geometry2D.GetCentredRect(toPos, (DefaultPinRadius * 2)).Contains(ptClient))
				{
					from = false;
					return true;
				}
			}

			return false;
		}

		private void SelectUserLink(UserLink link)
		{
			if ((link != null) && (link != m_SelectedUserLink))
			{
				SelectNode(link.FromId, true, false);
				Invalidate();
			}
			
			m_SelectedUserLink = link;
			m_UserLinkTypes.Add(link.Attributes.Type);
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			m_EditTimer.Stop();
			m_PreviouslySelectedTask = (Focused ? SingleSelectedTask : null);

			if (!ReadOnly)
			{
				bool from = false;

				var link = HitTestUserLinkEnds(e.Location, ref from);

				if ((link != null) && !from)
				{
					DoUserLinkDragDrop(link);

					// Prevent base class handling
					return;
				}

				link = HitTestUserLink(e.Location);

				if ((link != null) && !m_TaskItems.IsTaskLocked(link.FromId))
				{
					// Cache the link to be checked in OnMouseClick
					m_SelectedUserLink = link;

					// Prevent base class handling
					return;
				}
				else if (HasSelectedUserLink)
				{
					ClearUserLinkSelection();
				}

				var node = HitTestNode(e.Location, true);

				if ((node != null) && GetCreateLinkPinRect(node).Contains(e.Location))
				{
					DoUserLinkDragDrop(new UserLink(node.Data, NullId, UserLinkAttributes.Defaults));

					// Prevent base class handling
					return;
				}

				// Check for icon click
				var taskItem = HitTestTaskIcon(e.Location);

				if ((taskItem != null) && !taskItem.IsLocked)
				{
					EditTaskIcon?.Invoke(this, taskItem.TaskId);
					return;
				}

				// Check for image expandion click
				taskItem = HitTestTaskImageExpansionButton(e.Location);

				if (taskItem != null)
				{
					taskItem.HasExpandedImage = !taskItem.HasExpandedImage;
					RecalcExtents();

					return;
				}
			}

			base.OnMouseDown(e);
		}

		private void DoUserLinkDragDrop(UserLink link)
		{
			// Create a special new link
			SelectUserLink(link);

			m_DraggingSelectedUserLink = true;
			m_DraggedUserLinkEnd = GetCreateLinkPinPos(GetNode(link.FromId));

			Invalidate();

			DoDragDrop(this, DragDropEffects.Copy);
		}

		protected override void OnMouseDoubleClick(MouseEventArgs e)
		{
			var hit = HitTestNode(e.Location, true);

			if (hit != null)
			{
				if ((HitTestTaskIcon(e.Location) == null) && 
					(HitTestTaskImageExpansionButton(e.Location) == null))
				{
					var task = GetTaskItem(hit);
					var imageRect = CalcImageRect(task, GetNodeClientRect(hit), false);

					if (imageRect.Contains(e.Location))
					{
						Process.Start(task.ImagePath);
					}
					else
					{
						EditTaskLabel(this, SingleSelectedNode.Data);
					}
				}
			}
			else
			{
				var link = HitTestUserLink(e.Location);

				if (link != null)
				{
					SelectNode(link.FromId, true, false);
					m_SelectedUserLink = link;

					DoubleClickUserLink?.Invoke(this, null);
				}
			}
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			if (e.Button != MouseButtons.Left)
				return;

			// Check for connection first
			if (!ReadOnly)
			{
				var link = HitTestUserLink(e.Location);

				if ((link != null) && (link == m_SelectedUserLink))
				{
					m_SelectedUserLink = null;
					SelectUserLink(link);

					UserLinkSelectionChange?.Invoke(this, null);
					return;
				}

				// else
				ClearUserLinkSelection();
			}

			base.OnMouseClick(e);
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

		protected TaskItem GetTaskItem(BaseNode node)
		{
			return (node == null) ? null : GetTaskItem(node.Data);
		}

		public TaskItem GetTaskItem(uint nodeId)
		{
			return m_TaskItems.GetTaskItem(nodeId);
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

			if (cursor != null)
				return cursor;

			bool from = false;
			var link = HitTestUserLinkEnds(ptClient, ref from);

			if (link != null)
			{
				if (from)
				{
					return UIExtension.AppCursor(UIExtension.AppCursorType.NoDrag);
				}
				else if (!m_TaskItems.IsTaskLocked(link.FromId))
				{
					return Cursors.SizeAll;
				}
			}


			link = HitTestUserLink(ptClient);

			if ((link != null) && m_TaskItems.IsTaskLocked(link.FromId))
				return UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);

			return null;
		}

		protected Cursor GetSelectedUserLinkCursor(Point ptClient)
		{
			bool from = false;

			if (HasSelectedUserLink && HitTestUserLinkEnds(m_SelectedUserLink, ptClient, ref from))
			{
				if (from)
					return UIExtension.AppCursor(UIExtension.AppCursorType.NoDrag);
				else
					return Cursors.SizeAll;
			}

			return null;
		}

		protected TaskItem HitTestTaskIcon(Point ptClient)
		{
			var node = HitTestNode(ptClient, true); // exclude root node

			if (node == null)
				return null;

			var taskItem = GetTaskItem(node.Data);

			if (taskItem == null)
				return null;

// 			if (!TaskHasIcon(taskItem))
// 				return null;
			
			if (!CalcIconRect(GetNodeClientRect(node)).Contains(ptClient))
				return null;

			return taskItem;
		}

		protected TaskItem HitTestTaskImageExpansionButton(Point ptClient)
		{
			var node = HitTestNode(ptClient, true); // exclude root node

			if (node == null)
				return null;

			var taskItem = GetTaskItem(node.Data);

			if (taskItem == null)
				return null;

			if (taskItem.Image == null)
				return null;
			
			if (!CalcImageExpansionButtonRect(GetNodeClientRect(node)).Contains(ptClient))
				return null;

			return taskItem;
		}

		protected Cursor GetNodeCursor(Point ptClient)
		{
			var taskItem = HitTestTaskIcon(ptClient);

			if (taskItem != null)
			{
				if (taskItem.IsLocked)
					return UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);

				return UIExtension.HandCursor();
			}

			taskItem = HitTestTaskImageExpansionButton(ptClient);

			if (taskItem != null)
			{
				return UIExtension.HandCursor();
			}

#if DEBUG
			if (m_Options.HasFlag(EvidenceBoardOption.ShowRootNode))
			{
				// The only time we DON'T exclude the root node
				if (HitTestNode(ptClient) == RootNode)
					return UIExtension.AppCursor(UIExtension.AppCursorType.NoDrag);
			}
#endif
			return null;
		}

		private void UpdateHotTask(Point ptMouse)
		{
			// Keep track of the 'hot' task so we can display a 
			// 'pin' for initiating a new link drag
			var node = HitTestNode(ptMouse, true);

			if (m_HotTaskId == node?.Data)
				return;

			if ((m_HotTaskId != 0) && m_TaskItems.Keys.Contains(m_HotTaskId))
			{
				var hot = GetNode(m_HotTaskId);
				var pin = GetCreateLinkPinRect(hot);

				pin.Inflate(1, 1);
				Invalidate(pin);
			}

			if (node == null)
			{
				m_HotTaskId = 0;
			}
			else
			{
				m_HotTaskId = node.Data;
				var pin = GetCreateLinkPinRect(node);

				pin.Inflate(1, 1);
				Invalidate();
			}
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
 			base.OnMouseMove(e);

			Cursor cursor = null;

			if (!ReadOnly)
			{
				UpdateHotTask(e.Location);

				if (m_HotTaskId != 0)
				{
					var hotNode = GetNode(m_HotTaskId);

					if ((hotNode != null) && GetCreateLinkPinRect(hotNode).Contains(e.Location))
						cursor = UIExtension.OleDragCursor(UIExtension.OleDragCursorType.Copy);
				}

				if (cursor == null)
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
			}

			Cursor = ((cursor != null) ? cursor : Cursors.Arrow);
		}

		protected override void OnDragOver(DragEventArgs e)
		{
			if (m_DraggingSelectedUserLink)
			{
				var ptClient = m_DraggedUserLinkEnd = PointToClient(new Point(e.X, e.Y));
				var node = HitTestNode(ptClient, true);

				if (IsAcceptableDropTarget(node))
				{
					m_DropHighlightedTaskId = node.Data;
					e.Effect = e.AllowedEffect;
				}
				else
				{
					m_DropHighlightedTaskId = 0;
					e.Effect = DragDropEffects.None;
				}

				Invalidate();
			}
			else
			{
				base.OnDragOver(e);
			}
		}

		protected bool IsAcceptableDropTarget(BaseNode node)
		{
			Debug.Assert(m_DraggingSelectedUserLink);

			if (node == null)
				return false;

			if (node.Data == m_SelectedUserLink.FromId)
				return false;

			var taskItem = GetTaskItem(node.Data);

			if (taskItem == null)
				return false;

			// Check for an existing link between the selected task and the link target
			var link = m_TaskItems.FindUserLink(m_SelectedUserLink.FromId, node.Data);

			if ((link != null) && (link != m_SelectedUserLink))
				return false;

			return true;
		}

		protected override bool IsAcceptableDragSource(BaseNode node)
		{
			if (!base.IsAcceptableDragSource(node))
				return false;
			
			if (node == RootNode)
				return false;

			var taskItem = GetTaskItem(node.Data);

			return ((taskItem != null) && !taskItem.IsLocked);
		}

		protected bool OnDragDropNodes(object sender, IList<uint> nodeIds)
		{
			// Update the tasks
			foreach (uint nodeId in nodeIds)
			{
				var node = GetNode(nodeId);
				var taskItem = GetTaskItem(nodeId);

				if ((node != null) && (taskItem != null))
				{
					taskItem.UserPosition = node.Point.GetPosition();
				}
			}

			TaskModified?.Invoke(this, nodeIds);
			return true;
		}

		override protected void OnDragDrop(DragEventArgs e)
		{
			if (m_DraggingSelectedUserLink)
			{
				if (m_DropHighlightedTaskId != 0)
				{
					if (m_SelectedUserLink.ToId == NullId) // New link
					{
						Debug.Assert(m_SelectedUserLink.FromId == SingleSelectedNode.Data);

						var taskItem = GetTaskItem(m_SelectedUserLink.FromId);

						if (taskItem != null)
						{
							var newLink = CreateUserLink(taskItem.TaskId, m_DropHighlightedTaskId, m_SelectedUserLink.Attributes);

							if ((newLink != null) && (ConnectionCreated != null) && !ConnectionCreated(this, newLink))
								taskItem.DeleteUserLink(newLink);
						}
					}
					else
					{
						m_SelectedUserLink.ChangeToId(m_DropHighlightedTaskId);

						ConnectionEdited?.Invoke(this, m_SelectedUserLink);
					}
				}

				ResetUserLinkDrag();
			}
			else
			{
				base.OnDragDrop(e);
			}
		}

		void ResetUserLinkDrag()
		{
			m_DraggingSelectedUserLink = false;
			m_DropHighlightedTaskId = 0;
			m_SelectedUserLink = null;

			Invalidate();
		}

		protected override void OnQueryContinueDrag(QueryContinueDragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			if (m_DraggingSelectedUserLink)
			{
				if (((MouseButtons & MouseButtons.Left) != MouseButtons.Left) && (m_DropHighlightedTaskId == 0))
				{
					e.Action = DragAction.Cancel;
					ResetUserLinkDrag();
				}
				else if (e.EscapePressed)
				{
					e.Action = DragAction.Cancel;
					ResetUserLinkDrag();
				}
			}
			else
			{
				base.OnQueryContinueDrag(e);
			}
		}
	}
}

