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
		public bool IsFlagged { get; private set; }
		public bool IsParent { get; private set; }
		public bool IsDone { get; private set; }
		public bool IsGoodAsDone { get; private set; }
		public bool SomeSubtasksDone { get; private set; }
		public bool IsLocked { get; private set; }

		// -----------------------------------------------------------------

		public PertNetworkItem(String label, uint taskId) : base(label, taskId)
		{
			TextColor = new Color();
			HasIcon = false;
			IsFlagged = false;
			IsDone = false;
            IsGoodAsDone = false;
            SomeSubtasksDone = false;
			IsLocked = false;
		}

		public PertNetworkItem(Task task) : base(task.GetTitle(), task.GetID())
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

		private bool ShowParentAsFolder;
		private bool TaskColorIsBkgnd;
		private bool IgnoreMouseClick;
		private bool StrikeThruDone;

		private Timer EditTimer;
		private Font BoldLabelFont, DoneLabelFont, BoldDoneLabelFont;
		private Size CheckboxSize;

		// -------------------------------------------------------------------------

		private int LabelPadding { get { return ScaleByDPIFactor(2); } }

		// -------------------------------------------------------------------------

		public PertNetworkControl(Translator trans, UIExtension.TaskIcon icons)
		{
			Trans = trans;
			TaskIcons = icons;

			TaskColorIsBkgnd = false;
			IgnoreMouseClick = false;
			ShowParentAsFolder = false;
			ShowCompletionCheckboxes = true;
			StrikeThruDone = true;

			ItemHeight = ScaleByDPIFactor(ItemHeight);
			ItemWidth = ScaleByDPIFactor(ItemWidth);
			ItemVertSpacing = ScaleByDPIFactor(ItemVertSpacing);
			ItemHorzSpacing = ScaleByDPIFactor(ItemHorzSpacing);
			GraphBorder = ScaleByDPIFactor(GraphBorder);

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

			if (baseFontChange || doneFontChange)
				RebuildGroups();

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

		public bool TaskColorIsBackground
		{
			get { return TaskColorIsBkgnd; }
			set
			{
				if (TaskColorIsBkgnd != value)
				{
					TaskColorIsBkgnd = value;
					Invalidate();
				}
			}
		}

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

		public uint HitTest(Point screenPos)
		{
			var clientPos = PointToClient(screenPos);
			var item = HitTestItem(clientPos);

			if (item != null)
				return item.UniqueId;

			// else
			return 0;
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

		protected Color GetItemBackgroundColor(NetworkItem item)
		{
			if (TaskColorIsBkgnd)
			{
				var taskItem = (item as PertNetworkItem);

				if (!taskItem.TextColor.IsEmpty && !taskItem.IsDone && !taskItem.IsGoodAsDone)
					return taskItem.TextColor;
			}

			// all else
			return Color.Empty;
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

			// Background
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
			else
			{
				Color backColor = GetItemBackgroundColor(item);
				Color borderColor = SystemColors.WindowText;

				itemRect.Width--;
				itemRect.Height--;

				if (backColor != Color.Empty)
				{
					using (var brush = new SolidBrush(taskItem.TextColor))
						graphics.FillRectangle(brush, itemRect);
				}

				if (taskItem.TextColor != Color.Empty)
					borderColor = taskItem.TextColor;

				using (var pen = new Pen(borderColor))
					graphics.DrawRectangle(pen, itemRect);
			}

			// Text
			Color textColor = SystemColors.WindowText;

			if (!taskItem.TextColor.IsEmpty)
			{
				if (TaskColorIsBkgnd && !selected && !(taskItem.IsDone || taskItem.IsGoodAsDone))
				{
					textColor = DrawingColor.GetBestTextColor(taskItem.TextColor);
					textColor = DrawingColor.GetBestTextColor(taskItem.TextColor);
				}
				else if (selected)
				{
					textColor = DrawingColor.SetLuminance(taskItem.TextColor, 0.3f);
				}
				else
				{
					textColor = taskItem.TextColor;
				}
			}

			using (var brush = new SolidBrush(textColor))
				graphics.DrawString(String.Format("{0} (id: {1}, grp: {2})", item.Title, item.UniqueId, iGroup), this.Font, brush, itemRect);

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

			return (taskItem.HasIcon || (ShowParentAsFolder && taskItem.IsParent));
		}

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

			if (IgnoreMouseClick)
			{
				IgnoreMouseClick = false;
				return;
			}

			var taskItem = (HitTestItem(e.Location) as PertNetworkItem);

			if (taskItem == null)
				return;

			if (!ReadOnly && !taskItem.IsLocked)
			{
/*
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
*/
			}
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

