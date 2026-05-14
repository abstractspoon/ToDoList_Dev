using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerControl : UserControl, IDragRenderer, ILabelTipHandler
	{
		const int SplitWidth = 6;

		// ---------------------------------------------

		// From Parent
		private Translator m_Trans;
		private TaskItems m_Tasks;
		private LabelTip m_LabelTip;

		// local
		private bool m_DraggingHorzSplitBar, m_DraggingVertSplitBar;
		private DragImage m_DragImage;
		private Point m_SplitPos;

		private List<EisenhowerPane> m_Panes;

		// ---------------------------------------------

		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;
		public event TaskSelectionEventHandler SelectionChange;

		// ---------------------------------------------

		public EisenhowerControl() // For designer
		{
			InitializeComponent();

			m_Panes = new List<EisenhowerPane>() { m_TopLeftPane, m_TopRightPane, m_BottomLeftPane, m_BottomRightPane };
			m_SplitPos = new Point(50, 50); // 0-100
		}

		public void Initialize(Translator trans, UIExtension.TaskIcon icons)
		{
			m_Trans = trans;

			m_Tasks = new TaskItems();
			m_DragImage = new DragImage();
			m_LabelTip = new LabelTip(this);

			// Panes
			m_TopLeftPane.Initialize("Top-Left Pane", Properties.Resources.TopLeftPane, m_Trans, m_Tasks, icons);
			m_TopRightPane.Initialize("Top-Right Pane", Properties.Resources.TopRightPane, m_Trans, m_Tasks, icons);
			m_BottomLeftPane.Initialize("Bottom-Left Pane", Properties.Resources.BotLeftPane, m_Trans, m_Tasks, icons);
			m_BottomRightPane.Initialize("Bottom-Right Pane", Properties.Resources.BotRightPane, m_Trans, m_Tasks, icons);

			// Callbacks
			m_Panes.ForEach(p => 
			{
				p.EditTaskDone += new EditTaskCompletionEventHandler(OnPaneEditTaskDone);
				p.EditTaskIcon += new EditTaskIconEventHandler(OnPaneEditTaskIcon);
				p.EditTaskLabel += new EditTaskLabelEventHandler(OnPaneEditTaskLabel);
				p.SelectionChange += new TaskSelectionEventHandler(OnPaneSelectionChange);

				p.GotFocus += new EventHandler(OnPaneGotFocus);
			});
		}

		public void SetFilter(string xAttribTitle, string xAttribId,
							  string yAttribTitle, string yAttribId)
		{
			// Top-left => High Attrib1 - High Attrib2
			var xAttrib = new EisenhowerPaneFilterAttribute()
			{
				Id = xAttribId,
				Range = EisenhowerPaneFilterAttributeRange.High,
				Cutoff = 5
			};

			var yAttrib = new EisenhowerPaneFilterAttribute()
			{
				Id = yAttribId,
				Range = EisenhowerPaneFilterAttributeRange.High,
				Cutoff = 5
			};

			m_TopLeftPane.SetFilter(xAttribTitle, xAttrib, yAttribTitle, yAttrib);

			// Top-right => Low Attrib1 - High Attrib2
			xAttrib.Range = EisenhowerPaneFilterAttributeRange.Low;

			m_TopRightPane.SetFilter(xAttribTitle, xAttrib, yAttribTitle, yAttrib);

			// Bottom-left => High Attrib1 - Low Attrib2
			xAttrib.Range = EisenhowerPaneFilterAttributeRange.High;
			yAttrib.Range = EisenhowerPaneFilterAttributeRange.Low;

			// Dummy filter to get us started
			m_BottomLeftPane.SetFilter(xAttribTitle, xAttrib, yAttribTitle, yAttrib);

			// Bottom-right => Low Attrib1 - Low Attrib2
			xAttrib.Range = EisenhowerPaneFilterAttributeRange.Low;

			m_BottomRightPane.SetFilter(xAttribTitle, xAttrib, yAttribTitle, yAttrib);
		}

		public void SetUITheme(UITheme theme)
		{
			BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark);

			m_Panes.ForEach(p => p.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight));
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileInt(key, "XSplit", m_SplitPos.X);
			prefs.WriteProfileInt(key, "YSplit", m_SplitPos.Y);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			// App settings
			TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			ShowParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
			ShowCompletionCheckboxes = prefs.GetProfileBool("Preferences", "AllowCheckboxAgainstTreeItem", false);
			ShowMixedCompletionState = prefs.GetProfileBool("Preferences", "ShowMixedCompletionState", true);

			if (prefs.GetProfileBool("Preferences", "AlternateLineColor", true))
				AlternateLineColor = prefs.GetProfileColor("Preferences\\Colors", "AlternateLines", Color.Empty);
			else
				AlternateLineColor = Color.Empty;

			if (prefs.GetProfileBool("Preferences", "SpecifyGridColor", true))
				GridlineColor = prefs.GetProfileColor("Preferences\\Colors", "GridLines", Color.Empty);
			else
				GridlineColor = Color.Empty;

			if (!appOnly)
			{
				m_SplitPos.X = prefs.GetProfileInt(key, "XSplit", 50);
				m_SplitPos.Y = prefs.GetProfileInt(key, "YSplit", 50);
			}
		}

		public void EnsureSelectionVisible()
		{
			// TODO
		}

		public bool SelectTask(uint taskID)
		{
			foreach (var p in m_Panes)
			{
				if (p.SelectTask(taskID))
				{
					SelectedPane = p;
					return true;
				}
			}

			return false;
		}

		protected EisenhowerPane SelectedPane
		{
			get
			{
				foreach (var p in m_Panes)
				{
					if (p.Selected)
						return p;
				}

				return null;
			}

			set
			{
				m_Panes.ForEach(p => p.Selected = (p == value));
			}
		}

		public bool SelectTasks(uint[] taskIDs)
		{
			foreach (var p in m_Panes)
			{
				if (p.SelectTasks(taskIDs))
				{
					SelectedPane = p;
					return true;
				}
			}

			return false;
		}

		public bool HasSelection
		{
			get
			{
				foreach (var p in m_Panes)
				{
					if (p.HasSelection)
						return true;
				}

				return false;
			}
		}

		public bool ReadOnly { get; set; }

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			switch (type)
			{
			case UIExtension.UpdateType.All:
				m_Tasks.Clear();
				UpdateTaskAttributes(tasks);
				break;

			case UIExtension.UpdateType.Edit:
			case UIExtension.UpdateType.New:
				UpdateTaskAttributes(tasks);
				break;

			case UIExtension.UpdateType.Delete:
				// TODO
				break;
			}
		}

		public bool TaskColorIsBackground		{ set { m_Panes.ForEach(p => p.TaskColorIsBackground = value); } }
		public bool ShowMixedCompletionState	{ set { m_Panes.ForEach(p => p.ShowMixedCompletionState = value); } }
		public bool ShowParentsAsFolders		{ set {	m_Panes.ForEach(p => p.ShowParentsAsFolders = value); } }
		public bool ShowCompletionCheckboxes	{ set {	m_Panes.ForEach(p => p.ShowCompletionCheckboxes = value); } }

		public Color AlternateLineColor			{ set { m_Panes.ForEach(p => p.AlternateLineColor = value); } }
		public Color GridlineColor				{ set { m_Panes.ForEach(p => p.GridlineColor = value); } }

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			switch (attrib)
			{
			// Note: lock state is always provided
			case Task.Attribute.Title:
			case Task.Attribute.Icon:
			case Task.Attribute.Color:
			case Task.Attribute.DoneDate:
			case Task.Attribute.Position:
			case Task.Attribute.SubtaskDone:
			case Task.Attribute.CustomAttribute:
			case Task.Attribute.Priority:
			case Task.Attribute.Risk:
			case Task.Attribute.Cost:
				return true;
			}

			// all else
			return false;
		}

		public uint HitTestTask(Point screenPos)
		{
			uint taskId = 0;
			m_Panes.ForEach(p => { taskId |= p.HitTestTask(screenPos); } );

			return taskId;
		}

		public uint GetTaskId(UIExtension.GetTask getTask)
		{
			var pane = SelectedPane;
			bool next = true;

			switch (getTask)
			{
			case UIExtension.GetTask.GetPrevTask:
			case UIExtension.GetTask.GetPrevVisibleTask:
			case UIExtension.GetTask.GetPrevTopLevelTask:
				next = false;
				break;
			};

			while (pane != null)
			{
				uint taskId = pane.GetTaskId(getTask);

				if (taskId > 0)
					return taskId;

				pane = GetNextPane(pane, next/*, false*/);
			}

			return 0;
		}

		public Rectangle SelectedItemLabelRect
		{
			get
			{
				var pane = SelectedPane;

				if (pane != null)
				{
					var labelRect = pane.SelectedItemLabelRect;

					if (!labelRect.IsEmpty)
						return RectangleToClient(pane.RectangleToScreen(labelRect));
				}

				// else
				return Rectangle.Empty;
			}
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			EisenhowerPane pane = null;
			bool forward = true;

			switch (selectTask)
			{
			case UIExtension.SelectTask.SelectFirstTask:
				{
					pane = m_Panes[0];
				}
				break;

			case UIExtension.SelectTask.SelectNextTask:
				{
					pane = SelectedPane;

					if (pane.SelectedTaskId == pane.LastTaskId)
						pane = GetNextPane(pane, true);
				}
				break;

			case UIExtension.SelectTask.SelectNextTaskInclCurrent:
				{
					pane = SelectedPane;
				}
				break;

			case UIExtension.SelectTask.SelectPrevTask:
				{
					forward = false;
					pane = SelectedPane;

					if (pane.SelectedTaskId == pane.FirstTaskId)
						pane = GetNextPane(pane, false);
				}
				break;

			case UIExtension.SelectTask.SelectLastTask:
				{
					forward = false;
					pane = m_Panes[m_Panes.Count - 1];
				}
				break;
			}

			while (pane != null)
			{
				if (pane.SelectTask(text, selectTask, caseSensitive, wholeWord, findReplace))
				{
					SelectedPane = pane;
					return true;
				}

				pane = GetNextPane(pane, forward/*, false*/);
			}

			return false;
		}

		public new bool Focus()
		{
			if (Focused)
				return false;

			// else
			var pane = SelectedPane;

			if (pane == null)
				pane = m_Panes[0];

			return pane.Focus();
		}

		public new bool Focused
		{
			get
			{
				bool focused = false;
				m_Panes.ForEach(p => focused |= p.Focused);

				return focused;
			}
		}

		public bool CanSaveToImage()
		{
			return (m_Tasks.Count != 0);
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

		public void SetFont(String fontName, int fontSize)
		{
			Font font = new Font(fontName, fontSize);

			m_Panes.ForEach(p => p.Font = font);
			RecalcPaneRects();
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
		
		// Message handlers --------------------------------

		private bool OnPaneEditTaskDone(object sender, uint taskId, bool completed)
		{
			return (bool)EditTaskDone?.Invoke(sender, taskId, completed);
		}

		private bool OnPaneEditTaskIcon(object sender, uint taskId)
		{
			return (bool)EditTaskIcon?.Invoke(sender, taskId);
		}

		private bool OnPaneEditTaskLabel(object sender, uint taskId)
		{
			return (bool)EditTaskLabel?.Invoke(sender, taskId);
		}

		private void OnPaneSelectionChange(object sender, uint taskId)
		{
			SelectionChange?.Invoke(sender, taskId);
		}

		private void OnPaneGotFocus(object sender, EventArgs e)
		{
			// Prevent selection from moving to a pane without a selected task
			var pane = (sender as EisenhowerPane);

			if (pane != null)
			{
				if (pane.HasSelection)
				{
					SelectedPane = pane;
					SelectionChange?.Invoke(this, pane.SelectedTaskId);
				}
				else
				{
					var selPane = SelectedPane;

					if (selPane != null)
						selPane.Focus();
				}
			}
		}

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

			m_BottomLeftPane.Bounds = botLeftRect;

			// Bottom-right
			Rectangle botRightRect = ClientRectangle;

			botRightRect.X = topRightRect.X;
			botRightRect.Y = botLeftRect.Y;
			botRightRect.Width = topRightRect.Width;
			botRightRect.Height = botLeftRect.Height;

			m_BottomRightPane.Bounds = botRightRect;

			Update();
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

		protected override void OnMouseDoubleClick(MouseEventArgs e)
		{
			base.OnMouseDown(e);

			if (e.Button == MouseButtons.Left)
			{
				if (GetHorzSplitBarRect().Contains(e.Location))
					m_SplitPos.Y = 50;

				if (GetVertSplitBarRect().Contains(e.Location))
					m_SplitPos.X = 50;

				RecalcPaneRects();
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

		private void UpdateTaskAttributes(TaskList tasks)
		{
			var changedTaskIds = new HashSet<uint>();
			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, changedTaskIds))
				task = task.GetNextTask();

			// Only Update the panes if the attributes we are tracking were changed
			if (changedTaskIds.Count > 0)
			{
				// TODO
				m_Panes.ForEach(p => p.RefreshListItems());
			}
		}

		private bool ProcessTaskUpdate(Task task, HashSet<uint> taskIds)
		{
			if (!task.IsValid())
				return false;

            uint taskId = task.GetID();
			TaskItem item = m_Tasks.GetItem(taskId, true);

			if (!item.ProcessTaskUpdate(task))
				return false;

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, taskIds)) // RECURSIVE CALL
				subtask = subtask.GetNextTask();

			taskIds.Add(task.GetID());

			return true;
		}

		EisenhowerPane GetNextPane(EisenhowerPane pane, bool next/*, bool wrap*/)
		{
			int iPane = m_Panes.IndexOf(pane);

			if (next)
				iPane++;
			else
				iPane--;

			if ((iPane < 0) || (iPane >= m_Panes.Count))
				return null;

			return m_Panes[iPane];
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
