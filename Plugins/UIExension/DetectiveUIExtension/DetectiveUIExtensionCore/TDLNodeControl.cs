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

namespace DetectiveUIExtension
{
    public delegate bool EditTaskLabelEventHandler(object sender, uint taskId);
    public delegate bool EditTaskIconEventHandler(object sender, uint taskId);
	public delegate bool EditTaskCompletionEventHandler(object sender, uint taskId, bool completed);

	// -----------------------------------------------------------------

	public class TaskNode
	{
		// Data
		public string Title { get;  private set;}
		public Color TextColor { get; private set; }

		public bool HasIcon { get; private set; }
		public bool IsFlagged { get; private set; }
		public bool IsParent { get; private set; }
		public bool IsTopLevel { get; private set; }
		public bool SomeSubtasksDone { get; private set; }
		public bool IsLocked { get; private set; }

		public uint UniqueId { get; private set; }
		public uint ParentId { get; private set; }

		public List<uint> ChildIds { get; private set; }
		public List<uint> DependIds { get; private set; }

		private bool Done;
		private bool GoodAsDone;

		// -----------------------------------------------------------------

		public TaskNode()
		{
		}
		
		public TaskNode(Task task)
		{
			Title = task.GetTitle();
			TextColor = task.GetTextDrawingColor();
			HasIcon = (task.GetIcon().Length > 0);
			IsFlagged = task.IsFlagged(false);
			IsParent = task.IsParent();
			IsTopLevel = (task.GetParentID() == 0);
			Done = task.IsDone();
            GoodAsDone = task.IsGoodAsDone();
            SomeSubtasksDone = task.HasSomeSubtasksDone();
			IsLocked = task.IsLocked(true);

			ParentId = task.GetParentID();
			UniqueId = task.GetID();

			ChildIds = new List<uint>();
			DependIds = task.GetLocalDependency();
		}

		public override string ToString() 
		{
			return Title;
		}

		public void Update(Task task, HashSet<Task.Attribute> attribs)
		{
			// TODO
		}

		public bool HasLocalDependencies {  get { return (DependIds != null) && (DependIds.Count > 0); } }

		public bool IsDone(bool includeGoodAsDone) 
        { 
            if (includeGoodAsDone && GoodAsDone)
                return true;

            return Done; 
        }

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
                Done = task.IsDone();

			if (task.IsAttributeAvailable(Task.Attribute.MetaData))
			{
				// TODO
			}

			IsParent = task.IsParent();
			IsLocked = task.IsLocked(true);
            GoodAsDone = task.IsGoodAsDone();

			Debug.Assert(task.GetParentID() == ParentId);
			IsTopLevel = (task.GetParentID() == 0);

			return true;
		}

	}

	// ------------------------------------------------------------

	[Flags]
	enum DetectiveOption
	{
		None = 0x00,
	}

	// ------------------------------------------------------------

	[System.ComponentModel.DesignerCategory("")]
	class TDLNodeControl : NodeControl, IDragRenderer
	{
		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;

		// From Parent
		private Translator m_Trans;
		private UIExtension.TaskIcon m_TaskIcons;

		private bool m_ShowParentAsFolder = false;
		private bool m_TaskColorIsBkgnd = false;
		private bool m_StrikeThruDone = true;
		private bool m_ShowCompletionCheckboxes = true;

		private DetectiveOption m_Options;

		private Timer m_EditTimer;
		private Font m_BoldLabelFont, m_DoneLabelFont, m_BoldDoneLabelFont;
		//		private Size CheckboxSize;
		private TaskNode m_PreviouslySelectedNode;

		private DragImage m_DragImage;
		private Point m_LastDragPos;

		private Nodes m_Nodes;
		
		//private List<NodePath> CriticalPaths;

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

			m_Nodes = null;



			// 			using (Graphics graphics = Graphics.FromHwnd(Handle))
			// 				CheckboxSize = CheckBoxRenderer.GetGlyphSize(graphics, CheckBoxState.UncheckedNormal);

			FontChanged += new EventHandler(OnFontChanged);

			// Initialise our fonts
			Font = new Font("Tahoma", 8.25f);

			base.AutoCalculateRadialIncrement = true;
		}

		protected int NodeHeight { get { return (2 * Font.Height) + 4; } }
		protected int NodeWidth {  get {return (4 * NodeHeight); } }

		protected void OnFontChanged(object sender, EventArgs e)
		{
			var newFont = Font;

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

			base.EnableLayoutUpdates = false;
			base.NodeSize = new Size(NodeWidth, NodeHeight);

			if (RootNode != null)
			{
				base.InitialRadius = (float)((RootNode.Count * NodeSize.Width) / (2 * Math.PI));
				base.RadialIncrementOrSpacing = NodeSize.Width;
			}

			base.EnableLayoutUpdates = true;
		}

		public void SetStrikeThruDone(bool strikeThruDone)
		{
			if (m_StrikeThruDone != strikeThruDone)
			{
				m_StrikeThruDone = strikeThruDone;
				OnFontChanged(EventArgs.Empty);
			}
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			switch (type)
			{
			case UIExtension.UpdateType.Edit:
			case UIExtension.UpdateType.New:
				UpdateTaskAttributes(tasks);
				RecalcLayout();
				break;

			case UIExtension.UpdateType.Delete:
			case UIExtension.UpdateType.All:
				m_Nodes = new Nodes();
				UpdateTaskAttributes(tasks);
				break;

			case UIExtension.UpdateType.Unknown:
				return;
			}
		}

		public DetectiveOption Options
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
				return true;
			}

			// all else
			return false;
		}

		public uint HitTest(Point screenPos)
		{
			// 			var clientPos = PointToClient(screenPos);
			// 			var node = HitTestNode(clientPos);
			// 
			// 			return node?.UniqueId ?? 0;
			return 0;
		}

		public Rectangle GetSelectedNodeLabelRect()
		{
			// 			EnsureNodeVisible(SelectedNode);
			// 
			// 			var labelRect = GetSelectedNodeRect();
			// 
			// 			// 			labelRect.X -= LabelPadding;
			// 			// 			labelRect.X += GetExtraWidth(SelectedNode);
			// 			// 
			// 			// Make sure the rect is big enough for the unscaled font
			// 			labelRect.Height = (BaseFont.Height + (2 * LabelPadding)); 
			// 
			// 			return labelRect;
			return Rectangle.Empty;	
		}

		public bool CanMoveTask(uint taskId, uint destParentId, uint destPrevSiblingId)
		{
			return false;
		}

		public bool MoveTask(uint taskId, uint destParentId, uint destPrevSiblingId)
		{
			return false;
		}

		public new bool SelectNode(uint taskId)
		{
			if (base.SelectNode(taskId))
				return true;

			base.SelectNode(NodeControl.NullId);
			return false;
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
			//return !IsEmpty();
			return false;
		}

		// Internal ------------------------------------------------------------

		protected int ScaleByDPIFactor(int value)
		{
			return DPIScaling.Scale(value);
		}

		private void UpdateTaskAttributes(TaskList tasks)
		{
			RadialTree.TreeNode<uint> rootNode = base.RootNode;

			if (m_Nodes.Count == 0)
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
			base.RootNode = rootNode;

			base.EnableLayoutUpdates = true;

			Invalidate();
		}

		private bool ProcessTaskUpdate(Task task, RadialTree.TreeNode<uint> parentNode)
		{
			if (!task.IsValid())
				return false;

			var taskNode = m_Nodes.GetNode(task.GetID());

			if (taskNode == null)
			{
				taskNode = new TaskNode(task);
				m_Nodes.AddNode(taskNode);

				parentNode = parentNode.AddChild(task.GetID());
			}
			else
			{
				taskNode.Update(task);
			}

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, parentNode))
			{
				taskNode.ChildIds.Add(subtask.GetID());
				taskNode.DependIds.Add(subtask.GetID());

				subtask = subtask.GetNextTask();
			}

			return true;
		}

		protected Color GetNodeBackgroundColor(TaskNode taskNode, bool selected)
		{
			if (selected)
				return Color.Empty;

			if ((taskNode.TextColor != Color.Empty) && !taskNode.IsDone(true))
			{
				if (m_TaskColorIsBkgnd && !selected)
					return taskNode.TextColor;

				// else
				return DrawingColor.SetLuminance(taskNode.TextColor, 0.95f);
			}

			// all else
			return Color.Empty;
		}

		protected Color GetNodeTextColor(TaskNode taskNode, DrawState state)
		{
			if (state.HasFlag(DrawState.DragImage))
				return SystemColors.WindowText;

			if (taskNode.TextColor != Color.Empty)
			{
				bool selected = state.HasFlag(DrawState.Selected);

				if (m_TaskColorIsBkgnd && !selected && !taskNode.IsDone(true))
					return DrawingColor.GetBestTextColor(taskNode.TextColor);

				if (selected)
					return DrawingColor.SetLuminance(taskNode.TextColor, 0.3f);

				// else
				return taskNode.TextColor;
			}

			// All else
			return SystemColors.WindowText;
		}

		protected Color GetNodeBorderColor(TaskNode taskNode, bool selected)
		{
			if (selected)
				return Color.Empty;

			if (taskNode.TextColor != Color.Empty)
			{
				if (m_TaskColorIsBkgnd && !selected && !taskNode.IsDone(true))
					return DrawingColor.SetLuminance(taskNode.TextColor, 0.3f);

				// else
				return taskNode.TextColor;
			}

			// All else
			return SystemColors.ControlDarkDark;
		}

		protected Color GetNodeLineColor(TaskNode taskNode, DrawState state)
		{
			if (state.HasFlag(DrawState.DragImage))
				return SystemColors.WindowText;

			if (taskNode.TextColor != Color.Empty)
			{
				if (state.HasFlag(DrawState.Selected) || (m_TaskColorIsBkgnd && !taskNode.IsDone(true)))
					return DrawingColor.SetLuminance(taskNode.TextColor, 0.3f);

				// else
				return taskNode.TextColor;
			}

			// All else
			return SystemColors.ControlDarkDark;
		}

		protected Font GetNodeFont(TaskNode taskNode)
		{
			Font font = null;

			if (taskNode.IsTopLevel)
			{
				if (taskNode.IsDone(false))
					font = m_BoldDoneLabelFont;
				else
					font = m_BoldLabelFont;
			}
			else if (taskNode.IsDone(false))
			{
				font = m_DoneLabelFont;
			}
			
			return (font == null) ? Font : font;
		}

		protected void DrawZoomedIcon(Image image, Graphics graphics, Rectangle destRect)
		{
			Debug.Assert(IsZoomed);

			var gSave = graphics.Save();

			var attrib = new ImageAttributes();
			attrib.SetWrapMode(WrapMode.TileFlipXY);

			graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
			graphics.SmoothingMode = SmoothingMode.HighQuality;

			graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, attrib);
			graphics.Restore(gSave);
		}

/*
		override protected void DoPaint(Graphics graphics, Rectangle clipRect)
		{
			base.DoPaint(graphics, clipRect);
		}

		override protected void OnPaintNode(Graphics graphics, Node node, SelectionState selState)
		{
			DrawState drawState = DrawState.None;

			switch (selState)
			{
			case SelectionState.DropHighlighted:
				drawState = DrawState.DropHighlighted;
				break;

			case SelectionState.Selected:
				drawState = DrawState.Selected;
				break;
			}

			DoPaintNode(graphics, node, drawState);
		}

*/
		protected void DoPaintNode(Graphics graphics, TaskNode node, DrawState drawState)
		{
/*
			graphics.SmoothingMode = SmoothingMode.None;

			bool selected = drawState.HasFlag(DrawState.Selected);
			bool dropHighlight = drawState.HasFlag(DrawState.DropHighlighted);
			bool dragImage = drawState.HasFlag(DrawState.DragImage);

			var nodeRect = CalcNodeRectangle(node);

			if (dragImage)
				nodeRect.Offset(-nodeRect.Left, -nodeRect.Top);

			var taskNode = (node as TaskNode);

			// Figure out the required colours
			Color backColor = GetNodeBackgroundColor(taskNode, selected);
			Color borderColor = GetNodeBorderColor(taskNode, selected);
			Color lineColor = GetNodeLineColor(taskNode, drawState);
			Color textColor = GetNodeTextColor(taskNode, drawState);

			// Draw background
			if (selected)
			{
				UIExtension.SelectionRect.Style style = (Focused ? UIExtension.SelectionRect.Style.Selected : UIExtension.SelectionRect.Style.SelectedNotFocused);

				UIExtension.SelectionRect.Draw(Handle,
												graphics,
												nodeRect.X,
												nodeRect.Y,
												nodeRect.Width,
												nodeRect.Height,
												style,
												false); // opaque

				backColor = UIExtension.SelectionRect.GetColor(style);
			}
			else if (dropHighlight)
			{
				UIExtension.SelectionRect.Draw(Handle,
												graphics,
												nodeRect.X,
												nodeRect.Y,
												nodeRect.Width,
												nodeRect.Height,
												UIExtension.SelectionRect.Style.DropHighlighted,
												false); // opaque

				backColor = UIExtension.SelectionRect.GetColor(UIExtension.SelectionRect.Style.DropHighlighted);
			}
			else if (backColor != Color.Empty)
			{
				using (var brush = new SolidBrush(backColor))
					graphics.FillRectangle(brush, nodeRect);
			}
			else
			{
				backColor = SystemColors.Window;
			}

			// Optional border
			if (borderColor != Color.Empty)
			{
				// Pens behave weirdly
				nodeRect.Width--;
				nodeRect.Height--;

				using (var pen = new Pen(borderColor, 0f))
					graphics.DrawRectangle(pen, nodeRect);
			}

			// Icon
			var titleRect = nodeRect;

			if (TaskHasIcon(taskNode))
			{
				var iconRect = CalcIconRect(nodeRect);

				if (m_TaskIcons.Get(node.UniqueId))
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

								DrawZoomedIcon(tempImage, graphics, iconRect);
							}
						}
					}
				}

				titleRect.Width = (titleRect.Right - iconRect.Right);
				titleRect.X = iconRect.Right;
			}

			// Title
			titleRect.Inflate(-LabelPadding, -LabelPadding);
			titleRect.Height /= 2;
*/

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


		}

/*
		override protected void OnPaintLink(Graphics graphics, Node fromNode, Node toNode)
		{
			DoPaintLink(graphics, fromNode, toNode);
		}
*/

		protected void DoPaintLink(Graphics graphics, TaskNode fromNode, TaskNode toNode)
		{
/*
			graphics.SmoothingMode = SmoothingMode.AntiAlias;

			Point[] points = GetConnectionPoints(fromNode, toNode);

			using (var pen = new Pen(Color.DarkGray, 0f))
				graphics.DrawLines(pen, points);

			// Draw Arrow head and box without smoothing to better match core app
			graphics.SmoothingMode = SmoothingMode.None;

			Point arrow = points[points.Length - 1];
			arrow.X--;

			UIExtension.TaskDependency.DrawHorizontalArrowHead(graphics, arrow.X, arrow.Y, Font, false);

			// Draw 3x3 box at 'to' end
			Rectangle box = new Rectangle(points[0].X - 2, points[0].Y - 1, 3, 3);
			graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x4f, 0x4f, 0x4f)), box);
*/
		}

		private bool TaskHasIcon(TaskNode taskNode)
		{
			if ((m_TaskIcons == null) || (taskNode == null))
				return false;

			return (taskNode.HasIcon || (m_ShowParentAsFolder && taskNode.IsParent));
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

		protected override void OnMouseDoubleClick(MouseEventArgs e)
		{
// 			if (HitTestNode(e.Location) != null)
// 				EditTaskLabel(this, SelectedNode.UniqueId);
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			base.OnMouseClick(e);

			if (e.Button != MouseButtons.Left)
				return;

			TaskNode taskNode = null;
			//(HitTestNode(e.Location) as TaskNode);

			if (taskNode == null)
				return;

			if (/*!ReadOnly &&*/ !taskNode.IsLocked)
			{
/*
				if (HitTestCheckbox(node, e.Location))
				{
					if (EditTaskDone != null)
						EditTaskDone(this, taskNode.ID, !taskNode.IsDone(false));
				}
				else*/ if (HitTestIcon(taskNode, e.Location))
				{
					if (EditTaskIcon != null)
					    EditTaskIcon(this, taskNode.UniqueId);
				}
				else if (SelectedNodeWasPreviouslySelected)
				{
					if (EditTaskLabel != null)
						m_EditTimer.Start();
				}
			}
		}

		private bool SelectedNodeWasPreviouslySelected
		{
			get { return false; /*((SelectedNode != null) && (SelectedNode == m_PreviouslySelectedNode));*/ }
		}

		private bool HitTestIcon(TaskNode node, Point point)
        {
			var taskNode = (node as TaskNode);
			
			if (taskNode.IsLocked || !TaskHasIcon(taskNode))
				return false;

			// else
			return false;//			CalcIconRect(CalcNodeRectangle(node)).Contains(point);
        }

		protected override void OnMouseDown(MouseEventArgs e)
		{
// 			m_EditTimer.Stop();
// 			m_PreviouslySelectedNode = Focused ? SelectedNode : null;

			base.OnMouseDown(e);
		}

		private void OnEditLabelTimer(object sender, EventArgs e)
		{
			m_EditTimer.Stop();

// 			if (EditTaskLabel != null)
// 				EditTaskLabel(this, SelectedNode?.UniqueId ?? 0);
		}

/*
		protected override void OnMouseMove(MouseEventArgs e)
		{
 			base.OnMouseMove(e);

			var taskNode = (HitTestNode(e.Location) as TaskNode);

			if (!ReadOnly && (taskNode != null))
			{
				Cursor cursor = null;

				if (taskNode.IsLocked)
				{
					cursor = UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);
				}
				else if (TaskHasIcon(taskNode) && HitTestIcon(taskNode, e.Location))
				{
					cursor = UIExtension.HandCursor();
				}

				if (cursor != null)
				{
					Cursor = cursor;
					return;
				}
			}

			// all else
			Cursor = Cursors.Arrow;
		}
*/

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
			DoPaintNode(graphics, 
						(obj as TaskNode), 
						DrawState.Selected | DrawState.DragImage);
		}

	}
}

