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

	// -----------------------------------------------------------------

	class PertNetworkItem : NetworkItem
	{
		// Data
		public Color TextColor { get; private set; }

		public bool HasIcon { get; private set; }
		public bool Flagged { get; private set; }
		public bool Parent { get; private set; }
		public bool TopLevel { get; private set; }
		public bool Done { get; private set; }
		public bool GoodAsDone { get; private set; }
		public bool SomeSubtasksDone { get; private set; }
		public bool Locked { get; private set; }
		public string TaskPosition { get; private set; }

		// -----------------------------------------------------------------

		public PertNetworkItem(Task task) 
			: 
			base(task.GetTitle(), task.GetID(), task.GetLocalDependency())
		{
			TextColor = task.GetTextDrawingColor();
			HasIcon = (task.GetIcon().Length > 0);
			Flagged = task.IsFlagged(false);
			Parent = task.IsParent();
			TopLevel = (task.GetParentID() == 0);
			Done = task.IsDone();
            GoodAsDone = task.IsGoodAsDone();
            SomeSubtasksDone = task.HasSomeSubtasksDone();
			Locked = task.IsLocked(true);
			TaskPosition = task.GetPositionString();
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
				Flagged = task.IsFlagged(false);

			if (task.IsAttributeAvailable(Task.Attribute.Color))
				TextColor = task.GetTextDrawingColor();

            if (task.IsAttributeAvailable(Task.Attribute.SubtaskDone))
                SomeSubtasksDone = task.HasSomeSubtasksDone();

            if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
                Done = task.IsDone();

			if (task.IsAttributeAvailable(Task.Attribute.Dependency))
				DependencyUniqueIds = task.GetLocalDependency();

			Parent = task.IsParent();
			Locked = task.IsLocked(true);
            GoodAsDone = task.IsGoodAsDone();
			TopLevel = (task.GetParentID() == 0);
			TaskPosition = task.GetPositionString();

			return true;
		}

		public override int CompareTo(object other)
		{
			return TaskPosition.CompareTo(((PertNetworkItem)other).TaskPosition);
		}

	}

	// ------------------------------------------------------------

	[Flags]
	enum PertNetworkOption
	{
		None = 0x00,
	}

	// ------------------------------------------------------------

	[System.ComponentModel.DesignerCategory("")]
	class PertNetworkControl : NetworkControl
	{
		public event EditTaskLabelEventHandler      EditTaskLabel;
        public event EditTaskIconEventHandler       EditTaskIcon;
        public event EditTaskCompletionEventHandler EditTaskDone;

		// From Parent
		private Translator Trans;
		private UIExtension.TaskIcon TaskIcons;

		private bool m_ShowParentAsFolder = false;
		private bool m_TaskColorIsBkgnd = false;
		private bool m_StrikeThruDone = true;
		private bool m_ShowCompletionCheckboxes = true;
		private PertNetworkOption m_Options;

		private Timer EditTimer;
		private Font BoldLabelFont, DoneLabelFont, BoldDoneLabelFont;
//		private Size CheckboxSize;
		private NetworkItem PreviouslySelectedItem;

		private List<NetworkPath> CriticalPaths;

		// -------------------------------------------------------------------------

		public PertNetworkControl(Translator trans, UIExtension.TaskIcon icons)
		{
			Trans = trans;
			TaskIcons = icons;
			LabelPadding = ScaleByDPIFactor(2);

			EditTimer = new Timer();
			EditTimer.Interval = 500;
			EditTimer.Tick += new EventHandler(OnEditLabelTimer);

			// 			using (Graphics graphics = Graphics.FromHwnd(Handle))
			// 				CheckboxSize = CheckBoxRenderer.GetGlyphSize(graphics, CheckBoxState.UncheckedNormal);

			FontChanged += new EventHandler(OnFontChanged);

			// Initialise our fonts
			OnFontChanged(this, EventArgs.Empty);
		}

		protected void OnFontChanged(object sender, EventArgs e)
		{
			var newFont = Font;

			BoldLabelFont = new Font(newFont.Name, newFont.Size, FontStyle.Bold);

			if (m_StrikeThruDone)
			{
				BoldDoneLabelFont = new Font(newFont.Name, newFont.Size, FontStyle.Bold | FontStyle.Strikeout);
				DoneLabelFont = new Font(newFont.Name, newFont.Size, FontStyle.Strikeout);
			}
			else
			{
				BoldDoneLabelFont = BoldLabelFont;
				DoneLabelFont = null;
			}
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
				break;

			case UIExtension.UpdateType.Delete:
			case UIExtension.UpdateType.All:
				Data.Clear();
				break;

			case UIExtension.UpdateType.Unknown:
				return;
			}

			UpdateTaskAttributes(tasks);

			CriticalPaths = UpdateCriticalPaths();
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
			case Task.Attribute.ProjectName:
			case Task.Attribute.Dependency:
				return true;
			}

			// all else
			return false;
		}

		public uint HitTest(Point screenPos)
		{
			var clientPos = PointToClient(screenPos);
			var item = HitTestItem(clientPos);

			return item?.UniqueId ?? 0;
		}

		public Rectangle GetSelectedItemLabelRect()
		{
			EnsureItemVisible(SelectedItem);

			var labelRect = GetSelectedItemRect();

			// 			labelRect.X -= LabelPadding;
			// 			labelRect.X += GetExtraWidth(SelectedNode);
			// 
			// Make sure the rect is big enough for the unscaled font
			labelRect.Height = (BaseFont.Height + (2 * LabelPadding)); 

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

			return false;
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

			return false;
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

		protected int ScaleByDPIFactor(int value)
		{
			return DPIScaling.Scale(value);
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
				RebuildPaths();
			else
				Invalidate();
		}

		private bool ProcessTaskUpdate(Task task)
		{
			if (!task.IsValid())
				return false;

			NetworkItem item = Data.GetItem(task.GetID());

			if (item == null)
			{
				Data.AddItem(new PertNetworkItem(task));
			}
			else
			{
				PertNetworkItem taskItem = (item as PertNetworkItem);
				taskItem.Update(task);
			}

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask))
				subtask = subtask.GetNextTask();

			return true;
		}

		protected override bool IsAcceptableDropTarget(Object draggedItemData, Object dropTargetItemData, DropPos dropPos, bool copy)
		{
			// 			if (dropPos == PertNetworkControl.DropPos.On)
			// 				return !TaskItem(dropTargetItemData).IsLocked;

			// else
			return true;
		}

		protected override bool IsAcceptableDragSource(Object itemData)
		{
			return false; //!TaskItem(itemData).IsLocked;
		}

		protected override bool DoDrop(NetworkDragEventArgs e)
		{
			// TODO

			return true;
		}

		protected Color GetItemBackgroundColor(PertNetworkItem taskItem, bool selected)
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

		protected Color GetItemTextColor(PertNetworkItem taskItem, bool selected)
		{
			if (taskItem.TextColor != Color.Empty)
			{
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

		protected Color GetItemBorderColor(PertNetworkItem taskItem, bool selected)
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

		protected Color GetItemLineColor(PertNetworkItem taskItem, bool selected)
		{
			if (taskItem.TextColor != Color.Empty)
			{
				if (selected || (m_TaskColorIsBkgnd && !taskItem.IsDone(true)))
					return DrawingColor.SetLuminance(taskItem.TextColor, 0.3f);

				// else
				return taskItem.TextColor;
			}

			// All else
			return SystemColors.ControlDarkDark;
		}

		protected Font GetItemFont(PertNetworkItem taskItem)
		{
			Font font = null;

			if (taskItem.TopLevel)
			{
				if (taskItem.Done)
					font = BoldDoneLabelFont;
				else
					font = BoldLabelFont;
			}
			else if (taskItem.Done)
			{
				font = DoneLabelFont;
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

		override protected void DoPaint(Graphics graphics, Rectangle clipRect)
		{
			base.DoPaint(graphics, clipRect);

			foreach (var path in CriticalPaths)
			{
				NetworkItem prevItem = null;

				foreach (var item in path.Items)
				{
					if (WantDrawItem(item, clipRect))
						DoPaintItem(graphics, item, path, WantDrawItemSelected(item), true);

					if ((prevItem != null) && WantDrawConnection(item, prevItem, clipRect))
						DoPaintConnection(graphics, item, prevItem, path, true);

					prevItem = item;
				}
			}
		}

		override protected void OnPaintItem(Graphics graphics, NetworkItem item, NetworkPath path, bool selected)
		{
			// Don't paint critical paths until the end
			if ((CriticalPaths.Contains(path)))
				return;

			foreach (var critPath in CriticalPaths)
			{
				if (critPath.Contains(item))
					return;
			}

			DoPaintItem(graphics, item, path, selected, false);
		}

		protected void DoPaintItem(Graphics graphics, NetworkItem item, NetworkPath path, bool selected, bool critical)
		{
			graphics.SmoothingMode = SmoothingMode.None;

			var itemRect = CalcItemRectangle(item);
			var taskItem = (item as PertNetworkItem);

			// Figure out the required colours
			Color backColor = GetItemBackgroundColor(taskItem, selected);
			Color borderColor = GetItemBorderColor(taskItem, selected);
			Color lineColor = GetItemLineColor(taskItem, selected);
			Color textColor = GetItemTextColor(taskItem, selected);

			// Draw background
			if (selected)
			{
				UIExtension.SelectionRect.Style style = (Focused ? UIExtension.SelectionRect.Style.Selected : UIExtension.SelectionRect.Style.SelectedNotFocused);

				UIExtension.SelectionRect.Draw(Handle,
												graphics,
												itemRect.X,
												itemRect.Y,
												itemRect.Width,
												itemRect.Height,
												style,
												false); // opaque

				backColor = UIExtension.SelectionRect.GetColor(style);
			}
			else if (backColor != Color.Empty)
			{
				using (var brush = new SolidBrush(backColor))
					graphics.FillRectangle(brush, itemRect);
			}
			else
			{
				backColor = SystemColors.Window;
			}

			// Optional border
			if (borderColor != Color.Empty)
			{
				// Pens behave weirdly
				itemRect.Width--;
				itemRect.Height--;

				using (var pen = new Pen(borderColor, critical ? 2f : 0f))
					graphics.DrawRectangle(pen, itemRect);
			}

			// PERT Lines
			using (var pen = new Pen(lineColor))
			{
				if (borderColor == Color.Empty)
				{
					// Pens behave weirdly
					itemRect.Width--;
					itemRect.Height--;
				}

				int midY = ((itemRect.Top + itemRect.Bottom) / 2);
				int midX = ((itemRect.Left + itemRect.Right) / 2);

				graphics.DrawLine(pen, itemRect.Left, midY, itemRect.Right, midY);
				graphics.DrawLine(pen, midX, midY, midX, itemRect.Bottom);
			}

			// Icon
			var titleRect = itemRect;

			if (TaskHasIcon(taskItem))
			{
				var iconRect = CalcIconRect(itemRect);

				if (TaskIcons.Get(item.UniqueId))
				{
					if (!IsZoomed)
					{
						TaskIcons.Draw(graphics, iconRect.X, iconRect.Y);
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
								TaskIcons.Draw(gTemp, 0, 0);

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

			using (var brush = new SolidBrush(textColor))
			{
				int iPath = (Data.Paths.IndexOf(path) + 1);
				graphics.DrawString(String.Format("{0} (id:{1}, pos:{2}, path:{3})", item.Title, item.UniqueId, taskItem.TaskPosition, iPath), GetItemFont(taskItem), brush, titleRect);
			}

			/*
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

			else if (ShowCompletionCheckboxes)
			{
				rect.Width = (rect.Right - checkRect.Right - 2);
				rect.X = checkRect.Right + 2;
			}
			*/

			// PERT specific info

		}

		override protected void OnPaintConnection(Graphics graphics, NetworkItem fromItem, NetworkItem toItem, NetworkPath path)
		{
			// Don't paint critical paths until the end
			if ((CriticalPaths.Contains(path)))
				return;

			foreach (var critPath in CriticalPaths)
			{
				if (critPath.Contains(fromItem) &&
					critPath.Contains(toItem) &&
					toItem.IsDependency(fromItem))
				{
					return;
				}
			}
			
			DoPaintConnection(graphics, fromItem, toItem, path, false);
		}

		protected void DoPaintConnection(Graphics graphics, NetworkItem fromItem, NetworkItem toItem, NetworkPath path, bool critical)
		{
			var fromRect = CalcItemRectangle(fromItem);
			var toRect = CalcItemRectangle(toItem);

			Point[] points = null;

			if (toItem.Position.Y == fromItem.Position.Y)
			{
				// 'To' on same level as 'From'
				// +------+                    +------+
				// |      +------------------->|      |
				// +------+                    +------+
				//
				points = new Point[2];

				points[0] = new Point(fromRect.Right, ((fromRect.Top + fromRect.Bottom) / 2) - 1);
				points[1] = new Point(toRect.Left, ((toRect.Top + toRect.Bottom) / 2) - 1);
			}
			else
			{
				Point firstPt, lastPt;

				if (toItem.Position.Y < fromItem.Position.Y)
				{
					firstPt = new Point(fromRect.Right, fromRect.Top);
					lastPt = new Point(toRect.Left, toRect.Bottom);
				}
				else // below
				{
					firstPt = new Point(fromRect.Right, fromRect.Bottom);
					lastPt = new Point(toRect.Left, toRect.Top);
				}

				if (toItem.Position.X == fromItem.Position.X + 1)
				{
					// 'To' one column to the right of 'From'
					// and anywhere above or below 'From'
					// +------+             
					// |      +
					// +------+             
					//         \
					//          ---->+------+
					//               |      |
					//               +------+
					points = new Point[3];
					points[0] = firstPt;
					points[2] = lastPt;

					if (toItem.Position.Y < fromItem.Position.Y)
					{
						// above
						points[1] = new Point((fromRect.Right + toRect.Left) / 2, toRect.Bottom);
					}
					else
					{
						// below
						points[1] = new Point((fromRect.Right + toRect.Left) / 2, toRect.Top);
					}
				}
				else
				{
					// 'To' more than one column to the right of 'From'
					// and anywhere above or below 'From'
					// +------+      +------+       
					// |      +		 |      |
					// +------+      +------+       
					//         \---------------\
					//               +------+   -->+------+
					//               |      |      |      |
					//               +------+      +------+
					points = new Point[5];
					points[0] = firstPt;
					points[4] = lastPt;

					if (toItem.Position.Y < fromItem.Position.Y)
					{
						// above
						points[1] = new Point(firstPt.X + ItemHorzSpacing / 2, firstPt.Y - ItemVertSpacing / 2);
						points[2] = new Point(lastPt.X - ItemHorzSpacing / 2, points[1].Y);
						points[3] = new Point(points[2].X + ItemVertSpacing / 2, lastPt.Y);
					}
					else
					{
						// below
						points[1] = new Point(firstPt.X + ItemHorzSpacing / 2, firstPt.Y + ItemVertSpacing / 2);
						points[2] = new Point(lastPt.X - ItemHorzSpacing / 2, points[1].Y);
						points[3] = new Point(points[2].X + ItemVertSpacing / 2, lastPt.Y);
					}
				}
			}

			graphics.SmoothingMode = SmoothingMode.AntiAlias;

			using (var pen = new Pen(Color.DarkGray, critical ? 2f : 0f))
				graphics.DrawLines(pen, points);

			// Draw Arrow head and box without smoothing to better match core app
			graphics.SmoothingMode = SmoothingMode.None;

			Point arrow = points[points.Length - 1];
			arrow.X--;

			UIExtension.TaskDependency.DrawHorizontalArrowHead(graphics, arrow.X, arrow.Y, Font, false);

			// Draw 3x3 box at 'to' end
			Rectangle box = new Rectangle(points[0].X - 2, points[0].Y - 1, 3, 3);
			graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x4f, 0x4f, 0x4f)), box);
		}

		protected void DoPaintConnection2(Graphics graphics, NetworkItem fromItem, NetworkItem toItem, NetworkPath path, bool critical)
		{
			var fromRect = CalcItemRectangle(fromItem);
			var toRect = CalcItemRectangle(toItem);

			Point[] points = null;

			int midFromHeight = ((fromRect.Top + fromRect.Bottom) / 2);
			int midToHeight = ((toRect.Top + toRect.Bottom) / 2);

			Point firstPt = new Point(fromRect.Right, midFromHeight);
			Point lastPt = new Point(toRect.Left, midToHeight);

			if (toItem.Position.Y == fromItem.Position.Y)
			{
				// 'To' on same level as 'From'
				// +------+        +------+
				// |    0 +------->| 1    |
				// +------+        +------+
				//
				points = new Point[2] { firstPt, lastPt };
			}
			else
			{
				int halfHorzSpacing = (ItemHorzSpacing / 2);

				if (toItem.Position.X == fromItem.Position.X + 1)
				{
					// 'To' one column to the right of 'From'
					// and anywhere above or below 'From'
					// +------+             
					// |    0 +---+ 1
					// +------+   |            
					//            |
					//            |   +------+
					//          2 +-->+ 3    |
					//                +------+
					points = new Point[4];
					points[0] = firstPt;
					points[1] = new Point(firstPt.X + halfHorzSpacing, firstPt.Y);
					points[2] = new Point(lastPt.X - halfHorzSpacing, lastPt.Y);
					points[3] = lastPt;
				}
				else
				{
					// 'To' more than one column to the right of 'From'
					// and anywhere above or below 'From'
					// +------+      +------+       
					// |    0 +--+ 1 |      |
					// +------+  |   +------+       
					//         2 +-------------+ 3
					//               +------+  |   +------+
					//               |      |4 +-->+ 5    |
					//               +------+      +------+
					points = new Point[6];
					points[0] = firstPt;
					points[1] = new Point(firstPt.X + halfHorzSpacing, firstPt.Y);
					points[2] = new Point(firstPt.X + halfHorzSpacing, (firstPt.Y + lastPt.Y) / 2);
					points[3] = new Point(lastPt.X - halfHorzSpacing, (firstPt.Y + lastPt.Y) / 2);
					points[4] = new Point(lastPt.X - halfHorzSpacing, lastPt.Y);
					points[5] = lastPt;
				}
			}

			graphics.SmoothingMode = SmoothingMode.AntiAlias;

			using (var pen = new Pen(Color.DarkGray, critical ? 2f : 0f))
				graphics.DrawLines(pen, points);

			// Draw Arrow head and box without smoothing to better match core app
			graphics.SmoothingMode = SmoothingMode.None;

			Point arrow = points[points.Length - 1];
			arrow.X--;

			UIExtension.TaskDependency.DrawHorizontalArrowHead(graphics, arrow.X, arrow.Y, Font, false);

			// Draw 3x3 box at 'to' end
			Rectangle box = new Rectangle(points[0].X - 2, points[0].Y - 1, 3, 3);
			graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x4f, 0x4f, 0x4f)), box);
		}

		private bool TaskHasIcon(PertNetworkItem taskItem)
		{
			if ((TaskIcons == null) || (taskItem == null))
				return false;

			return (taskItem.HasIcon || (m_ShowParentAsFolder && taskItem.Parent));
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
			if (HitTestItem(e.Location) != null)
				EditTaskLabel(this, SelectedItem.UniqueId);
		}

		protected override void OnMouseClick(MouseEventArgs e)
		{
			base.OnMouseClick(e);

			if (e.Button != MouseButtons.Left)
				return;

			var taskItem = (HitTestItem(e.Location) as PertNetworkItem);

			if (taskItem == null)
				return;

			if (!ReadOnly && !taskItem.Locked)
			{
/*
				if (HitTestCheckbox(node, e.Location))
				{
					if (EditTaskDone != null)
						EditTaskDone(this, taskItem.ID, !taskItem.IsDone(false));
				}
				else*/ if (HitTestIcon(taskItem, e.Location))
				{
					if (EditTaskIcon != null)
					    EditTaskIcon(this, taskItem.UniqueId);
				}
				else if (SelectedItemWasPreviouslySelected)
				{
					if (EditTaskLabel != null)
						EditTimer.Start();
				}
			}
		}

		private bool SelectedItemWasPreviouslySelected
		{
			get { return ((SelectedItem != null) && (SelectedItem == PreviouslySelectedItem)); }
		}

		private bool HitTestIcon(NetworkItem item, Point point)
        {
			var taskItem = (item as PertNetworkItem);
			
			if (taskItem.Locked || !TaskHasIcon(taskItem))
				return false;
			
			// else
			return CalcIconRect(CalcItemRectangle(item)).Contains(point);
        }

		protected override void OnMouseDown(MouseEventArgs e)
		{
			EditTimer.Stop();
			PreviouslySelectedItem = Focused ? SelectedItem : null;

			base.OnMouseDown(e);
		}

		private void OnEditLabelTimer(object sender, EventArgs e)
		{
			EditTimer.Stop();

			if (EditTaskLabel != null)
				EditTaskLabel(this, SelectedItem?.UniqueId ?? 0);
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
 			base.OnMouseMove(e);

			var taskItem = (HitTestItem(e.Location) as PertNetworkItem);

			if (!ReadOnly && (taskItem != null))
			{
				Cursor cursor = null;

				if (taskItem.Locked)
				{
					cursor = UIExtension.AppCursor(UIExtension.AppCursorType.LockedTask);
				}
				else if (TaskHasIcon(taskItem) && HitTestIcon(taskItem, e.Location))
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

		List<NetworkPath> UpdateCriticalPaths()
		{
			var criticalPaths = new List<NetworkPath>();

/*
			// TODO
			// Test by picking the greatest aggregated segment length
			double maxLength = 0;
			NetworkPath criticalPath = null;

			foreach (var group in Data.Groups)
			{
				foreach (var path in group.Paths)
				{
					double pathLen = path.Length;

					if (pathLen > maxLength)
					{
						maxLength = pathLen;
						criticalPath = path;
					}
				}
			}

			if (criticalPath != null)
				criticalPaths.Add(criticalPath);
*/

			return criticalPaths;
		}

	}
}

