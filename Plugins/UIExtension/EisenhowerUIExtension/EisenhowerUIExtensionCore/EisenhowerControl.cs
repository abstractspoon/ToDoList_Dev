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
	public class AttributeChangeEventArgs : EventArgs
	{
		public EisenhowerVariable XAttrib;
		public double XValue;
		public EisenhowerVariable YAttrib;
		public double YValue;
	};

	public delegate bool AttributeChangeEventHandler(Object sender, AttributeChangeEventArgs args);

	// ---------------------------------------------

	public partial class EisenhowerControl : UserControl
	{
		const int SplitWidth = 6;

		// ---------------------------------------------

		// From Parent
		private Translator m_Trans;

		// local
		private EisenhowerTasks m_Tasks;
		private bool m_DraggingHorzSplitBar, m_DraggingVertSplitBar;
		private DragImage m_DragImage;
		private Point m_SplitPos;
		private List<EisenhowerPane> m_Panes;

		// ---------------------------------------------

		// From TaskListView
		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;

		// From EisenhowerPane
		public event SelectionChangeEventHandler SelectionChange;

		// local
		public event AttributeChangeEventHandler AttributeChange;

		// ---------------------------------------------

		public EisenhowerControl() // For designer
		{
			InitializeComponent();

			m_Panes = new List<EisenhowerPane>() { m_TopLeftPane, m_TopRightPane, m_BottomLeftPane, m_BottomRightPane };
			m_SplitPos = new Point(50, 50); // 0-100
		}

		public void Initialize(EisenhowerTasks tasks, Translator trans, UIExtension.TaskIcon icons)
		{
			m_Trans = trans;
			m_Tasks = tasks;

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
				p.SelectionChange += new SelectionChangeEventHandler(OnPaneSelectionChange);

 				p.DragBegin += new EventHandler(OnDragBegin);
 				p.DragLeave += new EventHandler(OnDragLeave);
				p.DragCancel += new EventHandler(OnDragCancel);

				p.DragOver += new DragEventHandler(OnDragOver);
				p.DragDrop += new DragEventHandler(OnDragDrop);
				p.GotFocus += new EventHandler(OnPaneGotFocus);
			});
		}

		public void SetFilter(EisenhowerVariable xAttrib, EisenhowerVariable yAttrib)
		{
			double xCutoff = xAttrib.ValueMidPoint;
			double yCutoff = yAttrib.ValueMidPoint;

			// Top-left => High xAttrib - High yAttrib
			m_TopLeftPane.SetFilter(new EisenhowerFilterVariable(xAttrib, EisenhowerPaneFilterAttributeRange.High, xCutoff), 
									new EisenhowerFilterVariable(yAttrib, EisenhowerPaneFilterAttributeRange.High, yCutoff));

			// Top-right => Low xAttrib - High yAttrib
			m_TopRightPane.SetFilter(new EisenhowerFilterVariable(xAttrib, EisenhowerPaneFilterAttributeRange.Low, xCutoff),
									 new EisenhowerFilterVariable(yAttrib, EisenhowerPaneFilterAttributeRange.High, yCutoff));

			// Bottom-left => High xAttrib - Low yAttrib
			m_BottomLeftPane.SetFilter(new EisenhowerFilterVariable(xAttrib, EisenhowerPaneFilterAttributeRange.High, xCutoff),
									   new EisenhowerFilterVariable(yAttrib, EisenhowerPaneFilterAttributeRange.Low, yCutoff));

			// Bottom-right => Low xAttrib - Low yAttrib
			m_BottomRightPane.SetFilter(new EisenhowerFilterVariable(xAttrib, EisenhowerPaneFilterAttributeRange.Low, xCutoff),
										new EisenhowerFilterVariable(yAttrib, EisenhowerPaneFilterAttributeRange.Low, yCutoff));
		}

		public EisenhowerVariable XFilterVariable { get { return m_Panes[0].Filter.XVariable; } }
		public EisenhowerVariable YFilterVariable { get { return m_Panes[0].Filter.YVariable; } }

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
			ShowLabelTips = !prefs.GetProfileBool("Preferences", "ShowInfoTips", false);

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
			SelectedPane?.EnsureSelectionVisible();
		}

		public bool PrepareNewTask(ref Task task)
		{
			var pane = SelectedPane;

			if (pane == null)
				pane = m_Panes[0];

			// TODO
			task.SetPriority((byte)pane.Filter.XVariable.Cutoff);
			task.SetRisk((byte)pane.Filter.XVariable.Cutoff);

			return true;
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

		public bool SelectTasks(IList<uint> taskIDs)
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

				if (Focused)
					value?.Focus();
			}
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

		public void OnUpdateTasks(UIExtension.UpdateType type, List<uint> taskIds)
		{
			switch (type)
			{
			case UIExtension.UpdateType.All:
				m_Panes.ForEach(p => p.RebuildTaskList());
				break;

			case UIExtension.UpdateType.Edit:
			case UIExtension.UpdateType.New:
			case UIExtension.UpdateType.Delete:
				m_Panes.ForEach(p => p.RefilterTasks(taskIds));
				break;
			}
		}

		public bool TaskColorIsBackground		{ set { m_Panes.ForEach(p => p.TaskColorIsBackground = value); } }
		public bool ShowMixedCompletionState	{ set { m_Panes.ForEach(p => p.ShowMixedCompletionState = value); } }
		public bool ShowParentsAsFolders		{ set {	m_Panes.ForEach(p => p.ShowParentsAsFolders = value); } }
		public bool ShowCompletionCheckboxes	{ set {	m_Panes.ForEach(p => p.ShowCompletionCheckboxes = value); } }
		public bool ShowLabelTips				{ set {	m_Panes.ForEach(p => p.ShowLabelTips = value); } }
		public bool ReadOnly					{ set {	m_Panes.ForEach(p => p.ReadOnly = value); } }

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
			EisenhowerPane unused;
			return HitTestTask(screenPos, out unused);
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

				pane = GetNextPane(pane, next);
			}

			return 0;
		}

		public Rectangle SelectedTaskLabelRect
		{
			get
			{
				var pane = SelectedPane;

				if (pane != null)
				{
					var labelRect = pane.SelectedTaskLabelRect;

					if (!labelRect.IsEmpty)
						return RectangleToClient(pane.RectangleToScreen(labelRect));
				}

				// else
				return Rectangle.Empty;
			}
		}

		public IList<uint> SelectedTaskIds
		{
			get
			{
				return SelectedPane?.SelectedTaskIds;
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

					if (pane.FirstSelectedTaskId == pane.LastTaskId)
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

					if (pane.LastSelectedTaskId == pane.FirstTaskId)
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

				pane = GetNextPane(pane, forward);
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
			RecalcPaneRects(false);
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

		private void OnPaneSelectionChange(object sender, IList<uint> taskIds)
		{
			SelectionChange?.Invoke(sender, taskIds);
		}

		private void OnPaneGotFocus(object sender, EventArgs e)
		{
			// Prevent selection from moving to a pane without a selected task
			var pane = (sender as EisenhowerPane);

			if (pane != null)
			{
				if (pane.HasSelection || pane.BoundSelecting)
				{
					SelectedPane = pane;
					SelectionChange?.Invoke(this, pane.SelectedTaskIds);
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

			RecalcPaneRects(false);
		}

		protected void RecalcPaneRects(bool update)
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

			if (update)
				Update();
		}

		protected bool IsSplitting
		{
			get { return (m_DraggingHorzSplitBar || m_DraggingVertSplitBar); }
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			base.OnMouseDown(e);

			if (e.Button == MouseButtons.Left)
			{
				m_DraggingHorzSplitBar = GetHorzSplitBarRect().Contains(e.Location);
				m_DraggingVertSplitBar = GetVertSplitBarRect().Contains(e.Location);

				Capture = IsSplitting;
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

				RecalcPaneRects(false);
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

			if (IsSplitting)
			{
				if (m_DraggingHorzSplitBar)
					m_SplitPos.Y = Math.Max(0, Math.Min(100, ((e.Y * 100) / Height)));

				if (m_DraggingVertSplitBar)
					m_SplitPos.X = Math.Max(0, Math.Min(100, ((e.X * 100) / Width)));

				RecalcPaneRects(true);
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

		EisenhowerPane GetNextPane(EisenhowerPane pane, bool next)
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

		private void OnDragBegin(object sender, EventArgs e)
		{
			// Create the drag image
			Debug.Assert(m_DragImage == null);

			m_DragImage = new DragImage();
			m_DragImage.Begin(Handle, SelectedPane, null);
		}

		private void OnDragLeave(object sender, EventArgs e)
		{
			m_Panes.ForEach(p => p.DropHighlighted = false);
		}

		private void OnDragCancel(object sender, EventArgs e)
		{
			OnDragLeave(sender, e);

			m_DragImage.End();
			m_DragImage = null;
		}

		private bool GetDragPanes(DragEventArgs e, out EisenhowerPane src, out EisenhowerPane dest)
		{
			src = (e.Data.GetData(typeof(EisenhowerPane)) as EisenhowerPane);
			dest = HitTestPane(new Point(e.X, e.Y));

			return IsValidDrop(src, dest);
		}

		private bool IsValidDrop(EisenhowerPane src, EisenhowerPane dest)
		{
			return ((src != null) && (dest != null) && (dest != src));
		}

		private void OnDragOver(object sender, DragEventArgs e)
		{
			EisenhowerPane srcPane, destPane;

			if (GetDragPanes(e, out srcPane, out destPane))
				e.Effect = e.AllowedEffect;
			else
				e.Effect = DragDropEffects.None;

			{
				m_DragImage?.ShowNoLock(false);

				m_Panes.ForEach(p =>
				{
					p.DropHighlighted = ((p != srcPane) && (p == destPane));
					p.Update();
				});

				m_DragImage?.ShowNoLock(true);
				m_DragImage?.Move(e.X, e.Y);
			}
		}

		private void OnDragDrop(object sender, DragEventArgs e)
		{
			OnDragCancel(sender, e);

			// Notify parent of changes
			Debug.Assert(AttributeChange != null);

			EisenhowerPane srcPane, destPane;

			if (GetDragPanes(e, out srcPane, out destPane))
			{
				var attribChangeArgs = new AttributeChangeEventArgs();

				var srcXRange = srcPane.Filter.XVariable.Range;
				var srcYRange = srcPane.Filter.YVariable.Range;
				var destXRange = destPane.Filter.XVariable.Range;
				var destYRange = destPane.Filter.YVariable.Range;

				if (destXRange != srcXRange)
				{
					attribChangeArgs.XAttrib = destPane.Filter.XVariable;
					attribChangeArgs.XValue = destPane.Filter.XVariable.Cutoff;

					if (destXRange == EisenhowerPaneFilterAttributeRange.High)
						attribChangeArgs.XValue++;
				}

				if (destYRange != srcYRange)
				{
					attribChangeArgs.YAttrib = destPane.Filter.YVariable;
					attribChangeArgs.YValue = destPane.Filter.YVariable.Cutoff;

					if (destYRange == EisenhowerPaneFilterAttributeRange.High)
						attribChangeArgs.YValue++;
				}

				if (AttributeChange.Invoke(this, attribChangeArgs))
				{
					var modTaskIds = SelectedTaskIds;
					int i = modTaskIds.Count;

					while (i-- > 0)
					{
						var task = m_Tasks.GetItem(modTaskIds[i]);

						if ((task != null) && !task.IsLocked)
						{
							if (destXRange != srcXRange)
								task.SetAttributeValue(attribChangeArgs.XAttrib, attribChangeArgs.XValue);

							if (destYRange != srcYRange)
								task.SetAttributeValue(attribChangeArgs.YAttrib, attribChangeArgs.YValue);
						}
						else
						{
							modTaskIds.RemoveAt(i);
						}
					}

					srcPane.RebuildTaskList();
					destPane.RebuildTaskList();

					SelectTasks(modTaskIds);
					destPane.Focus();
				}
			}
		}

		// 		protected override void OnDragOver(DragEventArgs e)
		// 		{
		// 			m_DragImage.ShowNoLock(false);
		// 
		// 			base.OnDragOver(e);
		// 
		// 			m_DragImage.ShowNoLock(true);
		// 			m_DragImage.Move(e.X, e.Y);
		// 		}
		// 
		// 		protected override void OnDragEnter(DragEventArgs e)
		// 		{
		// 			base.OnDragEnter(e);
		// 
		// 			var rect = SelectedTaskLabelRect;
		// 			rect.Offset(-rect.Left, -rect.Top);
		// 
		// 			m_DragImage.Begin(Handle, 
		// 								this,
		// 								SelectedPane, 
		// 								rect.Width, 
		// 								rect.Height, 
		// 								rect.Width, 
		// 								rect.Height);
		// 		}

		private uint HitTestTask(Point screenPos, out EisenhowerPane pane)
		{
			foreach (var p in m_Panes)
			{
				uint taskId = p.HitTestTask(screenPos);

				if (taskId != 0)
				{
					pane = p;
					return taskId;
				}
			}

			// else
			pane = null;
			return 0;
		}

		private EisenhowerPane HitTestPane(Point screenPos)
		{
			foreach (var p in m_Panes)
			{
				if (p.Bounds.Contains(PointToClient(screenPos)))
					return p;
			}

			// else
			return null;
		}
	}
}
