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

using IIControls;
using ImageHelper;

using BaseNode = RadialTree.TreeNode<uint>;

namespace EvidenceBoardUIExtension
{
    public delegate bool EditTaskLabelEventHandler(object sender, uint taskId);
    public delegate bool EditTaskIconEventHandler(object sender, uint taskId);
	public delegate bool EditTaskCompletionEventHandler(object sender, uint taskId, bool completed);

	public delegate bool TaskModifiedEventHandler(object sender, IList<uint> taskIds);
	public delegate bool ImageDroppedEventHandler(object sender, uint taskId, string[] imagePaths);

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
		ShowDateSlider	= 0x20,
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

	class TDLNodeControl : NodeControl, ILabelTipHandler
	{
		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;

		public event TaskModifiedEventHandler TaskModified;
		public event ImageDroppedEventHandler ImageDropped;

		public event ConnectionCreatedEventHandler ConnectionCreated;
		public event ConnectionEditedEventHandler ConnectionEdited;
		public event ConnectionDeletedEventHandler ConnectionDeleted;

		public EventHandler UserLinkSelectionChange;
		public EventHandler DoubleClickUserLink;
		public EventHandler ImageExpansionChange;

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

		protected int LabelPadding { get { return DPIScaling.Scale(2); } }
		protected int DefaultPinRadius { get { return DPIScaling.Scale(3); } }
		protected int LinkOffset { get { return DPIScaling.Scale(6); } }

		// -------------------------------------------------------------------------

		// From Parent
		private Translator m_Trans;
		private UIExtension.TaskIcon m_TaskIcons;
		private LabelTip m_LabelTip;

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

		private List<uint> m_PrevExpandedImageIds, m_PrevExpandedTaskIds;
		private int m_PrevZoomLevel = -1;

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
			m_LabelTip = new LabelTip(this);

			m_EditTimer = new Timer();
			m_EditTimer.Interval = 500;
			m_EditTimer.Tick += new EventHandler(OnEditLabelTimer);

			Options = DefaultOptions;

			m_TaskItems = null;
			m_DependencyColor = Color.CornflowerBlue;

			int nodeHeight = (int)(2 * Math.Max(BaseFontHeight, UIExtension.TaskIcon.IconSize)) + (3 * LabelPadding);
			int nodeWidth  = (4 * nodeHeight);

			base.NodeSize = DPIScaling.UnScale(new Size(nodeWidth, nodeHeight));
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

		public bool ExpandSelectedTaskImages()
		{
			bool change = false;

			foreach (var id in SelectedNodeIds)
			{
				var taskItem = GetTaskItem(id);
				change |= ((taskItem != null) && taskItem.ExpandImage(true));
			}

			if (!change)
				return false;

			Invalidate();
			return true;
		}

		public bool CollapseSelectedTaskImages()
		{
			bool change = false;

			foreach (var id in SelectedNodeIds)
			{
				var taskItem = GetTaskItem(id);
				change |= ((taskItem != null) && taskItem.ExpandImage(false));
			}

			if (!change)
				return false;

			Invalidate();
			return true;
		}

		public bool CanExpandSelectedTaskImages
		{
			get
			{
				// Look for first selected task having a collapsed image
				foreach (var id in SelectedNodeIds)
				{
					if (GetTaskItem(id)?.IsImageCollapsed == true)
						return true;
				}

				return false;
			}
		}

		public bool CanCollapseSelectedTaskImages
		{
			get
			{
				// Look for first selected task having an expanded image
				foreach (var id in SelectedNodeIds)
				{
					if (GetTaskItem(id)?.IsImageExpanded == true)
						return true;
				}

				return false;
			}
		}

		public bool ExpandAllTaskImages()
		{
			if (!m_TaskItems.ExpandAllTaskImages())
				return false;

			if (!RecalcExtents(IsZoomedToExtents))
				Invalidate();

			ImageExpansionChange?.Invoke(this, null);

			return true;
		}

		public bool CollapseAllTaskImages()
		{
			if (!m_TaskItems.CollapseAllTaskImages())
				return false;

			if (!RecalcExtents(IsZoomedToExtents))
				Invalidate();

			ImageExpansionChange?.Invoke(this, null);

			return true;
		}

		public bool CanExpandAllTaskImages { get { return (m_TaskItems?.CanExpandAllTaskImages == true); } }
		public bool CanCollapseAllTaskImages { get { return (m_TaskItems?.CanCollapseAllTaskImages == true); } }

		protected override bool ExpandNode(BaseNode node, bool expand, bool andChildren)
		{
			if (node == RootNode)
				return node.ExpandChildren(expand);

			// else
			return node.Expand(expand, andChildren);
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileInt(key, "ZoomLevel", ZoomLevel);
			prefs.WriteProfileString(key, "ExpandedTaskIds", string.Join("|", ExpandedNodeIds));

			var imageIds = string.Join("|", m_TaskItems.ExpandedImageIds);
			prefs.WriteProfileString(key, "ExpandedImageIds", ((imageIds == string.Empty) ? "0" : imageIds));

		}

		private List<uint> ParseTaskIds(string prevIds)
		{
			if (!string.IsNullOrEmpty(prevIds))
			{
				var taskIds = new List<uint>();
				var ids = prevIds.Split('|');

				foreach (var prevId in ids)
				{
					uint id = 0;

					if (uint.TryParse(prevId, out id))
						taskIds.Add(id);
				}

				return taskIds;
			}

			return null;
		}

		public void LoadPreferences(Preferences prefs, String key)
		{
			// Cache the previously collapsed items until we get our first task update
			m_PrevZoomLevel = prefs.GetProfileInt(key, "ZoomLevel", -1);
			m_PrevExpandedTaskIds = ParseTaskIds(prefs.GetProfileString(key, "ExpandedTaskIds", string.Empty));
			m_PrevExpandedImageIds = ParseTaskIds(prefs.GetProfileString(key, "ExpandedImageIds", string.Empty));
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
				ClearSelection();
				UpdateTaskAttributes(tasks, true);
				RecalcLayout();
				break;

			case UIExtension.UpdateType.All:
				UpdateBackgroundImage(tasks);
				UpdateTaskAttributes(tasks, true);
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

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			switch (attrib)
			{
			// Note: lock state is always provided
			case Task.Attribute.Title:
			case Task.Attribute.Icon:
			case Task.Attribute.Flag:
			case Task.Attribute.Color:
			case Task.Attribute.StartDate:
			case Task.Attribute.DueDate:
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

		public uint HitTestTaskId(Point ptClient)
		{
			var node = base.HitTestNode(ptClient, true);
			
			return node?.Data ?? 0;
		}

		Rectangle CalcTaskLabelRect(TaskItem taskItem, Rectangle rect, bool multiLine)
		{
			if (taskItem.HasIcon || taskItem.HasImage || (taskItem.IsParent && ShowParentsAsFolders))
			{
				var iconRect = CalcIconRect(rect);

				rect.X = (iconRect.Right + LabelPadding);
				rect.Width -= (iconRect.Width + (2 * LabelPadding));
			}

			rect.Y += LabelPadding;
			rect.Height = GetTaskLabelFont(taskItem).Height;

			if (multiLine)
				rect.Height += GetTaskLabelFont(taskItem).Height;

			return rect;
		}
		
		Rectangle CalcTaskLabelRect(BaseNode node, bool multiLine)
		{
			if (node == null || node.IsRoot)
				return Rectangle.Empty;

			return CalcTaskLabelRect(GetTaskItem(node), GetNodeClientRect(node), multiLine);
		}

		public Rectangle GetSelectedTaskLabelRect()
		{
			var rect = CalcTaskLabelRect(SingleSelectedNode, false);

			// Use the unscaled font for the label height
			rect.Height = (Font.Height + (2 * LabelPadding));

			return rect;
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
			return CreateUserLink(fromId, new UserLinkTarget(toId), attrib);
		}
		
		public UserLink CreateUserLink(uint fromId, UserLinkTarget target, UserLinkAttributes attrib)
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

				var newLink = fromTask.AddUserLink(target, attrib);

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

			base.SelectNode(NullId, notify, false);
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

		private void UpdateBackgroundImage(TaskList tasks)
		{
			string filePath;
			Rectangle rect;

			if (NodeControlBackgroundImage.TryParse(tasks.GetMetaData(TaskItem.MetaDataKey), out filePath, out rect))
				SetBackgroundImage(filePath, rect, false);
			else
				ClearBackgroundImage();
		}

		public string EncodeBackgroundImageState()
		{
			return BackgroundImage.Encode();
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

			// Restore previous session state
			if (rebuild)
			{
				if (m_PrevExpandedTaskIds?.Count > 0)
				{
					ExpandedNodeIds = m_PrevExpandedTaskIds;
					m_PrevExpandedTaskIds = null;
				}

				if (m_PrevExpandedImageIds?.Count > 0)
				{
					m_TaskItems.ExpandedImageIds = m_PrevExpandedImageIds;
					m_PrevExpandedImageIds = null;
				}

				if (m_PrevZoomLevel != -1)
				{
					ZoomTo(m_PrevZoomLevel);
					m_PrevZoomLevel = -1;
				}
			}

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

		public bool ShowLabelTips
		{
			set { m_LabelTip.Active = value; }
			get { return m_LabelTip.Active; }
		}

		protected override void WndProc(ref Message m)
		{
			if (m_LabelTip != null)
				m_LabelTip.ProcessMessage(m);

			base.WndProc(ref m);
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

			if (task?.IsImageExpanded == true)
			{
				size.Height += task.CalcActiveImageHeight(base.NodeSize.Width);
			}

			return size;
		}

		protected Color GetTaskBackgroundColor(TaskItem taskItem, bool selected)
		{
			if (selected)
				return Color.Empty;

			if (!taskItem.TextColor.IsEmpty)
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

			if (!taskItem.TextColor.IsEmpty)
			{
				bool selected = state.HasFlag(DrawState.Selected);

				if (m_TaskColorIsBkgnd && !selected)
					return DrawingColor.GetBestTextColor(taskItem.TextColor, true);

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

			if (!taskItem.TextColor.IsEmpty)
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

		protected Font GetTaskTooltipFont(TaskItem taskItem)
		{
			if (taskItem.IsTopLevel)
				return new Font(Font.Name, Font.Size, FontStyle.Bold);

			// else
			return Font;
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
						IsConnectionVisible(node, new UserLinkTarget(dependId), out fromPos, out toPos, false))
					{
						using (var pen = NewPen(DependencyColor))
						{
							using (var brush = new SolidBrush(DependencyColor))
							{
								DrawConnection(graphics, pen, brush, fromPos, toPos);
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
					var fromNode = GetNode(link.FromId);

					if (m_HotTaskId == link.FromId)
						fromPos = GetCreateLinkPinPos(fromNode);
					else
						IsConnectionVisible(fromNode, m_DraggedUserLinkEnd, out fromPos);

					toPos = m_DraggedUserLinkEnd;
				}

				var lineThickness = (selected ? 2 : link.Attributes.Thickness);
				var arrowThickness = Math.Max(2, lineThickness);
				var arrowOffset = (selected ? (DefaultPinRadius + 2) : PinRadius);
				var arrowSize = UIExtension.DependencyArrows.Size(TextFont);

				if (selected)
				{
					using (var pen = NewPen(SystemColors.WindowText, lineThickness))
						DrawConnection(graphics, pen, null, fromPos, toPos);

					// Draw special pins
					DrawSelectionPin(graphics, fromPos, false);
					DrawSelectionPin(graphics, toPos, true);

					DrawConnectionArrows(graphics, link.Attributes.Arrows, arrowThickness, SystemColors.WindowText, fromPos, toPos, arrowOffset);
				}
				else
				{
					using (var pen = NewPen(link.Attributes.Color, link.Attributes.Thickness))
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
				var graphCentre = Geometry2D.Centroid(GraphToClient(DataExtents));
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

			if ((m_DropHighlightedTaskId != 0) && (m_DropHighlightedTaskId == node.Data))
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

		protected bool IsConnectionVisible(BaseNode fromNode, UserLinkTarget target, out Point fromPos, out Point toPos, bool userLink)
		{
			fromPos = toPos = Point.Empty;

			if (!target.IsValid())
				return false;

			if (!target.UsesId)
			{
				toPos = GraphToClient(BackgroundImage.RelativeToAbsolute(target.RelativeImageCoords));
				return base.IsConnectionVisible(fromNode, toPos, out fromPos);
			}
			
			// else using Id
			var toNode = GetNode(target.Id);

			if (!userLink)
				return base.IsConnectionVisible(fromNode, toNode, out fromPos, out toPos);

			if (DrawNodesOnTop)
			{
				// If the reverse link exists then we need to offset 'our' ends
				if (UserLinkExists(target.Id, fromNode.Data))
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

			if (UserLinkExists(target.Id, fromNode.Data) &&
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

			return IsConnectionVisible(GetNode(link.FromId), link.Target, out fromPos, out toPos, true);
		}

		protected override void DrawParentConnection(Graphics graphics, uint childId, Point childPos, Point parentPos)
		{
			if (!ShowingParentChildLinks)
				return;

			var taskItem = m_TaskItems.GetTaskItem(childId);
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

			using (var pen = NewPen(ParentConnectionColor, 1))
			{
				using (var brush = new SolidBrush(ParentConnectionColor))
				{
					DrawConnection(graphics, pen, brush, childPos, parentPos);
				}
				DrawConnectionArrows(graphics, UserLinkAttributes.EndArrows.Finish, 2, ParentConnectionColor, childPos, parentPos, (PinRadius + 1));
			}
		}

		protected new void DrawConnection(Graphics graphics, Pen linePen, Brush pinBrush, Point node1Pos, Point node2Pos)
		{
			if (!SavingToImage)
			{
				// Draw a thicker version of the line in the background colour
				// so the overdrawn line is always visible
				if (linePen.Color != SystemColors.Window)
				{
					using (var backPen = NewPen(SystemColors.Window, (linePen.Width + ScaleByDpi(1))))
						base.DrawConnection(graphics, backPen, pinBrush, node1Pos, node2Pos);
				}
			}

			base.DrawConnection(graphics, linePen, pinBrush, node1Pos, node2Pos);
		}
		
		protected void DrawConnectionArrows(Graphics graphics, UserLinkAttributes.EndArrows arrows, int thickness, Color color, Point fromPos, Point toPos, int offset)
		{
			if (arrows != UserLinkAttributes.EndArrows.None)
			{
				using (var arrowPen = NewPen(color, thickness))
				{
					int size = (UIExtension.DependencyArrows.Size(TextFont) + (int)ScaleByDpi(thickness));

					if ((arrows == UserLinkAttributes.EndArrows.Start) || (arrows == UserLinkAttributes.EndArrows.Both))
					{
						var degrees = Geometry2D.DegreesBetween(toPos, fromPos, Geometry2D.AngleAxis.FromVertical);
						DrawConnectionArrow(graphics, arrowPen, fromPos, degrees, size, offset);
					}

					if ((arrows == UserLinkAttributes.EndArrows.Finish) || (arrows == UserLinkAttributes.EndArrows.Both))
					{
						var degrees = Geometry2D.DegreesBetween(fromPos, toPos, Geometry2D.AngleAxis.FromVertical);
						DrawConnectionArrow(graphics, arrowPen, toPos, degrees, size, offset);
					}
				}
			}
		}

		protected void DrawConnectionArrow(Graphics graphics, Pen arrowPen, Point arrowPos, float degrees, int size, int offset)
		{
			//if (HasBackgroundImage)
			{
				// Under-draw a thicker version of the arrow in the background colour
				// so the overdrawn line is always visible
				if (arrowPen.Color != SystemColors.Window)
				{
					using (var backPen = NewPen(SystemColors.Window, (arrowPen.Width + ScaleByDpi(1))))
						UIExtension.ArrowHeads.Draw(graphics, arrowPen, arrowPos.X, arrowPos.Y, size, offset, degrees);
				}
			}

			UIExtension.ArrowHeads.Draw(graphics, arrowPen, arrowPos.X, arrowPos.Y, size, offset, degrees);
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
			if (selected || dropHighlight)
			{
				UIExtension.SelectionRect.Style style = (dropHighlight ? UIExtension.SelectionRect.Style.DropHighlighted :
														(Focused ? UIExtension.SelectionRect.Style.Selected : 
																	UIExtension.SelectionRect.Style.SelectedNotFocused));
				UIExtension.SelectionRect.Draw(Handle,
												graphics,
												taskRect.X,
												taskRect.Y,
												taskRect.Width,
												taskRect.Height,
												style,
												false); // opaque

				backColor = UIExtension.SelectionRect.GetColor(style);
				textColor = UIExtension.SelectionRect.GetTextColor(style, textColor);
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

				using (var pen = NewPen(borderColor, 0))
					graphics.DrawRectangle(pen, taskRect);
			}

			// LHS icons
			DrawTaskIcon(graphics, taskItem, taskRect, backColor);
			DrawTaskImageExpansionButton(graphics, taskItem, taskRect, backColor);

			// Title
			var titleRect = CalcTaskLabelRect(taskItem, taskRect, true);
			graphics.DrawString(taskItem.ToString(), GetTaskLabelFont(taskItem), new SolidBrush(textColor), titleRect);

			// Image
			if (taskItem.IsImageExpanded)
			{
				var imageRect = CalcImageRect(taskItem, taskRect, selected || dropHighlight);
				graphics.DrawImage(taskItem.ActiveImage, imageRect);

				// Image spin button
				DrawTaskImageSpinButtons(graphics, taskItem, imageRect);
			}


			graphics.SmoothingMode = SmoothingMode.AntiAlias;
		}

		void DrawTaskIcon(Graphics g, TaskItem taskItem, Rectangle nodeRect, Color backColor)
		{
			if (!TaskHasIcon(taskItem))
				return;

			var iconRect = CalcIconRect(nodeRect);

			if (m_TaskIcons.Get(taskItem.TaskId))
			{
				if (!IsZoomed)
				{
					m_TaskIcons.Draw(g, iconRect.X, iconRect.Y);
				}
				else
				{
					int imageSize = UIExtension.TaskIcon.IconSize;

					using (var tempImage = new Bitmap(imageSize, imageSize, PixelFormat.Format32bppRgb)) // unscaled size
					{
						tempImage.MakeTransparent();
						using (var gTemp = Graphics.FromImage(tempImage))
						{
							gTemp.Clear(backColor);
							m_TaskIcons.Draw(gTemp, 0, 0);

							ImageUtils.DrawZoomedImage(tempImage, g, iconRect, nodeRect);
						}
					}
				}
			}
		}

		private Rectangle CalcImageExpansionButtonRect(Rectangle labelRect)
		{
			var rect = CalcIconRect(labelRect);
			rect.Y += rect.Height + LabelPadding;
//			rect.Height--;

			return rect;
		}

		void DrawTaskImageExpansionButton(Graphics graphics, TaskItem taskItem, Rectangle nodeRect, Color backColor)
		{
			if (SavingToImage)
				return;

			if (!taskItem.HasImage)
				return;

			var iconRect = CalcImageExpansionButtonRect(nodeRect);

			var mousePos = PointToClient(MousePosition);
			ScrollButton btn = (taskItem.IsImageCollapsed ? ScrollButton.Down : ScrollButton.Up);

			if (!IsZoomed)
			{
				DrawTaskImageScrollButton(graphics, iconRect, btn, mousePos, true);
			}
			else
			{
				int imageSize = UIExtension.TaskIcon.IconSize;

				using (var tempImage = new Bitmap(imageSize, imageSize, PixelFormat.Format32bppRgb)) // unscaled size
				{
					tempImage.MakeTransparent();

					using (var gTemp = Graphics.FromImage(tempImage))
					{
						var tempRect = new Rectangle(0, 0, imageSize, imageSize);
						gTemp.Clear(backColor);

						DrawTaskImageScrollButton(gTemp, tempRect, btn, mousePos, true);
						ImageUtils.DrawZoomedImage(tempImage, graphics, iconRect, nodeRect);
					}
				}
			}
		}

		private Rectangle CalcImageSpinButtonRect(Rectangle imageRect)
		{
			imageRect.Inflate(1, 1);

			var rect = CalcIconRect(imageRect);
			rect.Width *= 2;

			return rect;
		}

		private Rectangle CalcImageSpinButtonRect(Rectangle imageRect, bool forward)
		{
			var rect = CalcImageSpinButtonRect(imageRect);
			rect.Width /= 2;

			if (forward)
				rect.X = rect.Right;

			return rect;
		}

		void DrawTaskImageSpinButtons(Graphics graphics, TaskItem taskItem, Rectangle imageRect)
		{
			Debug.Assert(taskItem.IsImageExpanded);

			if (ReadOnly || SavingToImage || taskItem.IsLocked || (taskItem.ImageCount <= 1))
				return;

			var mousePos = PointToClient(MousePosition);
			var spinRect = CalcImageSpinButtonRect(imageRect);

			bool backEnabled = taskItem.CanSelectNextImage(false);
			bool forwardEnabled = taskItem.CanSelectNextImage(true);

			if (!IsZoomed)
			{
				DrawTaskImageSpinButtons(graphics, spinRect, mousePos, backEnabled, forwardEnabled);
			}
			else
			{
				int imageSize = UIExtension.TaskIcon.IconSize;

				using (var tempImage = new Bitmap(2 * imageSize, imageSize, PixelFormat.Format32bppRgb)) // unscaled size
				{
					tempImage.MakeTransparent();

					using (var gTemp = Graphics.FromImage(tempImage))
					{
						var tempRect = new Rectangle(0, 0, spinRect.Width, spinRect.Height);

						DrawTaskImageSpinButtons(gTemp, tempRect, mousePos, backEnabled, forwardEnabled);
						ImageUtils.DrawZoomedImage(tempImage, graphics, spinRect, imageRect);
					}
				}
			}
		}

		static void DrawTaskImageSpinButtons(Graphics graphics, Rectangle spinRect, Point mousePos, bool backEnabled, bool forwardEnabled)
		{
			var backRect = spinRect;
			backRect.Width /= 2;

			DrawTaskImageScrollButton(graphics, backRect, ScrollButton.Left, mousePos, backEnabled);

			var forwardRect = backRect;
			forwardRect.X = forwardRect.Right;

			DrawTaskImageScrollButton(graphics, forwardRect, ScrollButton.Right, mousePos, forwardEnabled);
		}

		private static VisualStyleRenderer visualStyleRenderer = null;

		static void DrawTaskImageScrollButton(Graphics graphics, Rectangle rect, ScrollButton btn, Point mousePos, bool enabled)
		{
			bool pressed = (MouseButtons.HasFlag(MouseButtons.Left) && rect.Contains(mousePos));

			if (VisualStyleRenderer.IsSupported)
			{
				// Map to spin button visual styles (nicer than scroll buttons)
				VisualStyleElement vsBtn;

				switch (btn)
				{
				case ScrollButton.Down:
					if (!enabled)
						vsBtn = VisualStyleElement.Spin.Down.Disabled;
					else if (pressed)
						vsBtn = VisualStyleElement.Spin.Down.Pressed;
					else
						vsBtn = VisualStyleElement.Spin.Down.Normal;
					break;

				case ScrollButton.Up:
					if (!enabled)
						vsBtn = VisualStyleElement.Spin.Up.Disabled;
					else if (pressed)
						vsBtn = VisualStyleElement.Spin.Up.Pressed;
					else
						vsBtn = VisualStyleElement.Spin.Up.Normal;
					break;

				case ScrollButton.Left:
					if (!enabled)
						vsBtn = VisualStyleElement.Spin.DownHorizontal.Disabled;
					else if (pressed)
						vsBtn = VisualStyleElement.Spin.DownHorizontal.Pressed;
					else
						vsBtn = VisualStyleElement.Spin.DownHorizontal.Normal;
					break;

				case ScrollButton.Right:
					if (!enabled)
						vsBtn = VisualStyleElement.Spin.UpHorizontal.Disabled;
					else if (pressed)
						vsBtn = VisualStyleElement.Spin.UpHorizontal.Pressed;
					else
						vsBtn = VisualStyleElement.Spin.UpHorizontal.Normal;
					break;

				default:
					return;
				}

				if (visualStyleRenderer == null)
					visualStyleRenderer = new VisualStyleRenderer(vsBtn.ClassName, vsBtn.Part, vsBtn.State);
				else
					visualStyleRenderer.SetParameters(vsBtn.ClassName, vsBtn.Part, vsBtn.State);

				// spin buttons have a 1-pixel border so we draw slightly larger
				rect.Inflate(1, 1);
				visualStyleRenderer.DrawBackground(graphics, rect);

				// Try to match core app with a border
//				graphics.DrawRectangle(Pens.LightGray, rect);
			}
			else
			{
				ButtonState state = (!enabled ? ButtonState.Inactive : (pressed ? ButtonState.Pushed : ButtonState.Normal));

				ControlPaint.DrawScrollButton(graphics, rect, btn, state);
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
			int headerHeight = (int)(NodeSize.Height * OverallScaleFactor);

			nodeRect.Y += headerHeight;
			nodeRect.Height -= headerHeight;

			nodeRect.Inflate(-1, -1);

			if (!selected)
			{
				nodeRect.Width++;
				nodeRect.Height++;
			}

			return nodeRect;
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

			int width = (int)(UIExtension.TaskIcon.IconSize * ZoomFactor);
			int height = width;

            return new Rectangle(topLeft.X, topLeft.Y, width, height);
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

					if (IsConnectionVisible(node, link.Target, out fromPos, out toPos, true) && 
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

			TaskItem taskItem = null;
			BaseNode node = HitTestNode(e.Location, true);

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

				if (HitTestHotNodeCreateLink(e.Location))
				{
					DoUserLinkDragDrop(new UserLink(node.Data, NullId, UserLinkAttributes.Defaults));

					// Prevent base class handling
					return;
				}

				// Check for icon click
				taskItem = HitTestTaskIcon(e.Location);

				if ((taskItem != null) && !taskItem.IsLocked)
				{
					SelectNode(taskItem.TaskId, true, false);
					EditTaskIcon?.Invoke(this, taskItem.TaskId);

					return;
				}

				// Check for image cycling
				bool forwardBtn = false;
				taskItem = HitTestTaskImageSpinButton(e.Location, ref forwardBtn);

				if ((taskItem != null) && taskItem.CanSelectNextImage(forwardBtn))
				{
					// Redraw the spin control
					Invalidate(CalcImageSpinButtonRect(CalcImageRect(taskItem, GetNodeClientRect(node), false)));
					Update();

					SelectNode(taskItem.TaskId, true, false);

					// Next/Prev image
					if (taskItem.SelectNextImage(forwardBtn))
					{
						RecalcExtents(false);
						TaskModified?.Invoke(this, new List<uint>() { taskItem.TaskId });
					}

					return;
				}
			}

			// Check for image expansion click
			if (node != null)
			{
				taskItem = HitTestTaskImageExpansionButton(node, e.Location);

				if (taskItem != null)
				{
					// Redraw the button
					Invalidate(CalcImageExpansionButtonRect(GetNodeClientRect(node)));
					Update();

					// Toggle expansion state
					taskItem.ExpandImage(!taskItem.IsImageExpanded);

					SelectNode(taskItem.TaskId, true, false);
					RecalcExtents(false);

					ImageExpansionChange?.Invoke(this, null);

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
				var taskItem = HitTestTaskIcon(e.Location);

				if (taskItem != null)
					return;

				taskItem = HitTestTaskImageExpansionButton(e.Location);

				if (taskItem != null)
					return;

				taskItem = GetTaskItem(hit);
				var imageRect = CalcImageRect(taskItem, GetNodeClientRect(hit), false);

				if ((taskItem.ImageCount < 2) || !CalcImageSpinButtonRect(imageRect).Contains(e.Location))
				{
					if (imageRect.Contains(e.Location))
					{
						Process.Start(taskItem.ActiveImagePath);
					}
					else
					{
						ScrollToSelection(false);
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

			if (!CalcIconRect(GetNodeClientRect(node)).Contains(ptClient))
				return null;

			return taskItem;
		}

		protected TaskItem HitTestTaskImageSpinButton(Point ptClient, ref bool forward)
		{
			var node = HitTestNode(ptClient, true); // exclude root node

			if (node == null)
				return null;

			var taskItem = GetTaskItem(node.Data);

			if (taskItem?.IsImageExpanded == false)
				return null;

			var imageRect = CalcImageRect(taskItem, GetNodeClientRect(node), false);

			if (CalcImageSpinButtonRect(imageRect, true).Contains(ptClient)) // forward button
			{
				forward = true;
				return taskItem;
			}

			if (CalcImageSpinButtonRect(imageRect, false).Contains(ptClient)) // back button
			{
				forward = false;
				return taskItem;
			}

			// All else
			return null;
		}

		protected TaskItem HitTestTaskImageExpansionButton(Point ptClient)
		{
			var node = HitTestNode(ptClient, true); // exclude root node

			return HitTestTaskImageExpansionButton(node, ptClient);
		}

		protected TaskItem HitTestTaskImageExpansionButton(BaseNode node, Point ptClient)
		{
			if (node == null)
				return null;

			var taskItem = GetTaskItem(node.Data);

			if ((taskItem == null) || !taskItem.HasImage)
				return null;
			
			if (!CalcImageExpansionButtonRect(GetNodeClientRect(node)).Contains(ptClient))
				return null;

			return taskItem;
		}

		protected Cursor GetNodeCursor(Point ptClient)
		{
			var taskItem = HitTestTask(ptClient);

			if (taskItem != null)
			{
				if (taskItem.IsLocked)
					return UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);

				if (HitTestTaskIcon(ptClient) != null)
					return UIExtension.HandCursor();

				return Cursors.Arrow;
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

			if ((node == null) || m_TaskItems.IsTaskLocked(node.Data))
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

		private bool HitTestHotNodeCreateLink(Point ptClient)
		{
			if (m_HotTaskId != 0)
			{
				var hotNode = GetNode(m_HotTaskId);

				if (hotNode == null)
					return false;

				if (Rectangle.Inflate(GetCreateLinkPinRect(hotNode), 1, 1).Contains(ptClient))
					return true;
			}

			return false;
		}
		
		protected override Cursor GetCursor(MouseEventArgs e)
		{
			Cursor cursor = null;

			if (!ReadOnly)
			{
				UpdateHotTask(e.Location);

				if (HitTestHotNodeCreateLink(e.Location))
				{
					cursor = UIExtension.OleDragCursor(UIExtension.OleDragCursorType.Copy);
				}
				else
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

			return ((cursor != null) ? cursor : base.GetCursor(e));
		}

		protected override void OnDragOver(DragEventArgs e)
		{
			if (m_DraggingSelectedUserLink || e.Data.GetDataPresent(DataFormats.FileDrop))
			{
				var prevDropId = m_DropHighlightedTaskId;

				m_DropHighlightedTaskId = 0;
				e.Effect = DragDropEffects.None;

				var ptClient = PointToClient(new Point(e.X, e.Y));
				var node = HitTestNode(ptClient, true);

				if (m_DraggingSelectedUserLink)
				{
					m_DraggedUserLinkEnd = ptClient;

					if (IsAcceptableLinkDropTarget(node))
					{
						m_DropHighlightedTaskId = node.Data;
						e.Effect = e.AllowedEffect;
					}
					else if (node != null)
					{
						e.Effect = DragDropEffects.None;
					}
					else if (HitTestBackgroundImage(ptClient) == DragMode.BackgroundImage)
					{
						e.Effect = e.AllowedEffect;
					}
				}
				else if ((node != null) && !ReadOnly && !IsTaskLocked(node.Data))
				{
					// file drag-drop
					string[] filePaths = (string[])(e.Data.GetData(DataFormats.FileDrop));

					// Check for at least one image file
					foreach (string file in filePaths)
					{
						if (TaskItem.IsImageFile(file))
						{
							m_DropHighlightedTaskId = node.Data;
							e.Effect = e.AllowedEffect;
						}
					}

					if (m_DropHighlightedTaskId == prevDropId)
						return;
				}

				Invalidate();
				return;
			}
			
			// else
			base.OnDragOver(e);
		}

		protected bool IsAcceptableLinkDropTarget(BaseNode node)
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
			m_HotTaskId = 0;

			if (m_DraggingSelectedUserLink)
			{
				UserLinkTarget target = null;

				if (m_DropHighlightedTaskId != 0)
				{
					target = new UserLinkTarget(m_DropHighlightedTaskId);
				}
				else
				{
					var ptClient = PointToClient(new Point(e.X, e.Y));

					if (HitTestBackgroundImage(ptClient) == DragMode.BackgroundImage)
					{
						var ptImage = BackgroundImage.AbsoluteToRelative(ClientToGraph(ptClient));
						target = new UserLinkTarget(ptImage);
					}
				}

				if (target != null)
				{
					if (m_SelectedUserLink.Target.Id == NullId) // New link
					{
						Debug.Assert(m_SelectedUserLink.FromId == SingleSelectedNode.Data);

						var taskItem = GetTaskItem(m_SelectedUserLink.FromId);

						if (taskItem != null)
						{
							var newLink = CreateUserLink(taskItem.TaskId, target, m_SelectedUserLink.Attributes);

							if ((newLink != null) && (ConnectionCreated != null) && !ConnectionCreated(this, newLink))
								taskItem.DeleteUserLink(newLink);
						}
					}
					else
					{
						m_SelectedUserLink.SetTarget(target);

						ConnectionEdited?.Invoke(this, m_SelectedUserLink);
					}
				}

				ResetUserLinkDrag();
			}
			else if (e.Data.GetDataPresent(DataFormats.FileDrop))
			{
				// Retrieve all the image files
				string[] filePaths = (string[])(e.Data.GetData(DataFormats.FileDrop));

				filePaths = filePaths.Where(file => TaskItem.IsImageFile(file)).ToArray();

				if (filePaths.Count() > 0)
				{
					// Select the highlighted task
					if (!SelectedNodeIds.Contains(m_DropHighlightedTaskId))
						SelectNode(m_DropHighlightedTaskId, true, true);

					// Notify the app
					if ((ImageDropped != null) && ImageDropped(this, m_DropHighlightedTaskId, filePaths))
					{
						// Add the images to each of the selected tasks
						foreach (var id in SelectedNodeIds)
						{
							var taskItem = GetTaskItem(id);

							if ((taskItem != null) && (taskItem.AddImages(filePaths) > 0))
								taskItem.ExpandImage(true);
						}
					}
				}
			}
			else
			{
				base.OnDragDrop(e);
			}
		}

		protected override void CancelDrag()
		{
			base.CancelDrag();

			ResetUserLinkDrag();
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
				bool cancel = e.EscapePressed;

				if (!cancel && !MouseButtons.HasFlag(MouseButtons.Left) && (m_DropHighlightedTaskId == 0))
				{
					if (HitTestTask(m_DraggedUserLinkEnd) != null)
					{
						// Must be an invalid drop target
						cancel = true;
					}
					else if (!HasBackgroundImage)
					{
						cancel = true;
					}
					else if (HitTestBackgroundImage(m_DraggedUserLinkEnd) != DragMode.BackgroundImage)
					{
						cancel = true;
					}
				}

				if (cancel)
				{
					e.Action = DragAction.Cancel;
					CancelDrag();
				}

				return;
			}

			base.OnQueryContinueDrag(e);
		}

		// ILabelTipHandler implementation
		public Control GetOwner()
		{
			return this;
		}

		const uint NoTip = 0xffffffff;

		enum TipId
		{
			TaskTitle,
			ExpansionBtn,
			SpinForwardBtn,
			SpinBackBtn,
			CreateLinkPin,

			NumTips,
		}

		uint TooltipId(TaskItem taskItem, TipId tipId)
		{
			return ((taskItem.TaskId * (uint)TipId.NumTips) + (uint)tipId);
		}

		public LabelTipInfo ToolHitTest(Point ptScreen)
		{
			if (DragMode != DragMode.None)
				return null;

			Point clientPos = PointToClient(ptScreen);
			var node = HitTestNode(clientPos, true);

			if (node == null)
				return null;

			var taskItem = GetTaskItem(node);
			var nodeRect = GetNodeClientRect(node);

			var tip = new LabelTipInfo();

			if (Rectangle.Inflate(GetCreateLinkPinRect(node), 1, 1).Contains(clientPos))
			{
				tip.Text = m_Trans.Translate("New Connection", Translator.Type.ToolTip);
				tip.Id = TooltipId(taskItem, TipId.CreateLinkPin);
			}
			else if (CalcExpansionButtonRect(nodeRect).Contains(clientPos))
			{
				tip.Id = NoTip;
			}
			else if (taskItem.HasImage)
			{
				var imageRect = CalcImageRect(taskItem, nodeRect, false);

				// Image expansion button
				tip.Rect = CalcImageExpansionButtonRect(nodeRect);

				if (tip.Rect.Contains(clientPos))
				{
					tip.Text = m_Trans.Translate("Toggle Image Visibility", Translator.Type.ToolTip);
					tip.Id = TooltipId(taskItem, TipId.ExpansionBtn);
				}
				else if (taskItem.ImageCount > 1)
				{
					// Image spin buttons
					// Forward button
					tip.Rect = CalcImageSpinButtonRect(imageRect, true);

					if (tip.Rect.Contains(clientPos))
					{
						tip.Text = m_Trans.Translate("Next Image", Translator.Type.ToolTip);
						tip.Id = TooltipId(taskItem, TipId.SpinForwardBtn);
					}
					else // Back button
					{
						tip.Rect = CalcImageSpinButtonRect(imageRect, false);

						if (tip.Rect.Contains(clientPos))
						{
							tip.Text = m_Trans.Translate("Previous Image", Translator.Type.ToolTip);
							tip.Id = TooltipId(taskItem, TipId.SpinBackBtn);
						}
					}
				}
			}

			if (tip.Id != 0)
			{
				if (tip.Id != NoTip)
				{
					// These are really tooltips not label tips so offset them
					clientPos.Offset(0, ToolStripEx.GetActualCursorHeight(Cursor));

					tip.Rect.Location = clientPos;
					tip.InitialDelay = 500;
					tip.MultiLine = false;
					tip.Font = Font;
				}
			}
			else // check for title tip
			{
				tip.Rect = CalcTaskLabelRect(node, true);
				tip.Text = taskItem.ToString();
				tip.Font = GetTaskTooltipFont(taskItem);

				var sizeText = TextRenderer.MeasureText(tip.Text, tip.Font, new Size(tip.Rect.Width, 0), TextFormatFlags.WordBreak);

				tip.MultiLine = (sizeText.Height > tip.Font.Height);

				if (!tip.MultiLine)
					tip.Rect.Size = sizeText;

				if (!ClientRectangle.Contains(tip.Rect))
				{
					// If the text rectangle is not wholly visible we always 
					// need a label tip so we just clip to the avail space
					tip.Rect.X = Math.Max(0, Math.Min(ClientRectangle.Right - tip.Rect.Width, tip.Rect.Left));
					tip.Rect.Y = Math.Max(0, Math.Min(ClientRectangle.Bottom - tip.Rect.Height, tip.Rect.Top));
				}
				else if (!tip.Rect.Contains(clientPos)) // check available space
				{
					return null;
				}
				else if ((sizeText.Width <= tip.Rect.Width) && (sizeText.Height <= tip.Rect.Height))
				{
					return null;
				}

				tip.Rect.Inflate(LabelPadding, LabelPadding);
				tip.Id = TooltipId(taskItem, TipId.TaskTitle);
			}

			return tip;
		}


		public bool SetBackgroundImage(string filePath)
		{
			if (!SetBackgroundImage(filePath, Rectangle.Empty, false))
				return false;

			BackgroundImage.ResizeToFit(DataExtents);
			ZoomToExtents();

			BackgroundImageChanged?.Invoke(this, null);

			return true;
		}

	}

}

