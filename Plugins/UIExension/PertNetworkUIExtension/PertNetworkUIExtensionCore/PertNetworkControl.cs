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

			return true;
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

		private bool m_ShowParentAsFolder;
		private bool m_TaskColorIsBkgnd;
		private bool m_StrikeThruDone;
		private bool m_ShowCompletionCheckboxes;
		private PertNetworkOption m_Options;

		private Timer EditTimer;
		private Font BoldLabelFont, DoneLabelFont, BoldDoneLabelFont;
		private Size CheckboxSize;
		private NetworkItem PreviouslySelectedItem;

		// -------------------------------------------------------------------------

		private int LabelPadding { get { return ScaleByDPIFactor(2); } }

		// -------------------------------------------------------------------------

		public PertNetworkControl(Translator trans, UIExtension.TaskIcon icons)
		{
			Trans = trans;
			TaskIcons = icons;

			m_TaskColorIsBkgnd = false;
			m_ShowParentAsFolder = false;
			m_ShowCompletionCheckboxes = true;
			m_StrikeThruDone = true;

			EditTimer = new Timer();
			EditTimer.Interval = 500;
			EditTimer.Tick += new EventHandler(OnEditLabelTimer);

			using (Graphics graphics = Graphics.FromHwnd(Handle))
				CheckboxSize = CheckBoxRenderer.GetGlyphSize(graphics, CheckBoxState.UncheckedNormal);

			RecalculateItemSize();
		}

		public void SetStrikeThruDone(bool strikeThruDone)
		{
			m_StrikeThruDone = strikeThruDone;

			if (BoldLabelFont != null)
				SetFont(BoldLabelFont.Name, (int)BoldLabelFont.Size, m_StrikeThruDone);
		}

		public new void SetFont(String fontName, int fontSize)
		{
			SetFont(fontName, fontSize, m_StrikeThruDone);
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

			if (baseFontChange || doneFontChange)
				RebuildGroups();

			base.SetFont(fontName, fontSize);
		}

		void RecalculateItemSize()
		{
			ItemHeight = ((Font.Height + LabelPadding) * 4);
			ItemWidth = (ItemHeight * 3);
			ItemVertSpacing = ScaleByDPIFactor(ItemVertSpacing);
			ItemHorzSpacing = ScaleByDPIFactor(ItemHorzSpacing);
			GraphBorder = ScaleByDPIFactor(GraphBorder);

			base.RecalculateItemSize(Data.Items.CalcMaximumPosition());
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

		public bool SelectedItemWasPreviouslySelected
		{
			get { return (SelectedItem == PreviouslySelectedItem); }
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
			labelRect.Height = (this.Font.Height + (2 * LabelPadding)); 

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

		override protected int ScaleByDPIFactor(int value)
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
				RebuildGroups();
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

			if (taskItem.TextColor != Color.Empty)
			{
				if (m_TaskColorIsBkgnd && !selected && !taskItem.IsDone(true))
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
			
			return (font == null) ? this.Font : font;
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

		override protected void OnPaintItem(Graphics graphics, NetworkItem item, int iGroup, bool selected)
		{
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

				UIExtension.SelectionRect.Draw(this.Handle,
												graphics,
												itemRect.X,
												itemRect.Y,
												itemRect.Width,
												itemRect.Height,
												style,
												false); // opaque
			}
			else if (backColor != Color.Empty)
			{
				using (var brush = new SolidBrush(backColor))
					graphics.FillRectangle(brush, itemRect);
			}

			// Optional border
			if (borderColor != Color.Empty)
			{
				// Pens behave weirdly
				itemRect.Width--;
				itemRect.Height--;

				using (var pen = new Pen(borderColor))
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
						var tempImage = new Bitmap(imageSize, imageSize); // original size

						using (var gTemp = Graphics.FromImage(tempImage))
						{
							gTemp.FillRectangle(SystemBrushes.Window, 0, 0, imageSize, imageSize);
							TaskIcons.Draw(gTemp, 0, 0);

							DrawZoomedImage(tempImage, graphics, iconRect);
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
				graphics.DrawString(String.Format("{0} (id: {1}, grp: {2})", item.Title, item.UniqueId, iGroup), GetItemFont(taskItem), brush, titleRect);
			}

			// PERT specific info


			//itemRect.Y += titleRect.Height 

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
								}
			*/
		}

		override protected void OnPaintConnection(Graphics graphics, NetworkItem fromItem, NetworkItem toItem)
		{
			var fromRect = CalcItemRectangle(fromItem);
			var toRect = CalcItemRectangle(toItem);

			Point[] points = new Point[3];
			
			points[0].X = fromRect.Right;
			points[0].Y = ((fromRect.Top + fromRect.Bottom) / 2);

			points[2].X = toRect.Left;
			points[2].Y = ((toRect.Top + toRect.Bottom) / 2);

			if (points[0].Y > points[2].Y) // below
			{
				points[0].Y = fromRect.Top;
				points[2].Y = toRect.Bottom;
			}
			else if (points[0].Y < points[2].Y) // above
			{
				points[0].Y = fromRect.Bottom;
				points[2].Y = toRect.Top;
			}
			
			points[1].X = ((fromRect.Right + toRect.Left) / 2);
			points[1].Y = points[2].Y;

			graphics.DrawLines(Pens.DarkGray, points);

			// Draw Arrow head and box without smoothing to better match core app
			var smoothing = graphics.SmoothingMode;
			graphics.SmoothingMode = SmoothingMode.None;

			Point arrow = points[2];
			arrow.X--;

			UIExtension.TaskDependency.DrawHorizontalArrowHead(graphics, arrow.X, arrow.Y, Font, false);

			// Draw 3x3 box at 'to' end
			Rectangle box = new Rectangle(points[0].X - 1, points[0].Y - 1, 3, 3);
			graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x4f, 0x4f, 0x4f)), box);

			graphics.SmoothingMode = smoothing;
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

		protected override int GetMinItemHeight()
		{
            return (ScaleByDPIFactor(16) + 1);
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
				else if (SelectedNodeWasPreviouslySelected)
				{
					if (EditTaskLabel != null)
						EditTimer.Start();
				}
			}
		}

		private bool SelectedNodeWasPreviouslySelected
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
			PreviouslySelectedItem = SelectedItem;

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
	}
}

