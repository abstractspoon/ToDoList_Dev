using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace EisenhowerUIExtension
{
	public delegate void SelectionChangeEventHandler(Object sender, IList<uint> taskIds);

	// ---------------------------------------------

	public partial class EisenhowerPane : UserControl, IDragRenderer
	{
		private EisenhowerTasks m_Tasks;
		private EisenhowerPaneMatrix m_Matrix;
		private Translator m_Trans;
		private UITheme m_Theme = new UITheme();

		private bool m_ShowMixedCompletionState;
		private bool m_DropHighlight;
		private bool m_ReadOnly;

		// ---------------------------------------------

		// From TaskListView
		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;
		public event IsTaskDraggableEventHandler IsTaskDraggable;

		// Local 
		public event SelectionChangeEventHandler SelectionChange;

		public	   event EventHandler DragBegin;
		public new event EventHandler DragLeave;
 		public new event DragEventHandler DragOver;
		public new event DragEventHandler DragDrop;

		public new event EventHandler GotFocus;

		// ---------------------------------------------

		public EisenhowerPane()
		{
			InitializeComponent();
		}

		public void Initialize(string paneTitle,
							   Bitmap paneIcon,
							   Translator trans, 
							   EisenhowerTasks taskItems, 
							   UIExtension.TaskIcon taskIcons)
		{
			m_TitleBar.Text = paneTitle;
			m_Icon.Image = paneIcon;

			m_Tasks = taskItems;
			m_Matrix = null;
			m_Trans = trans;
			m_Theme = new UITheme();

			m_List.Initialize(trans, taskIcons);
			m_List.ReadOnly = false;

			m_List.EditTaskDone += new EditTaskCompletionEventHandler(OnListEditTaskDone);
			m_List.EditTaskIcon += new EditTaskIconEventHandler(OnListEditTaskIcon);
			m_List.EditTaskLabel += new EditTaskLabelEventHandler(OnListEditTaskLabel);
			m_List.IsTaskDraggable += new IsTaskDraggableEventHandler(OnListIsTaskDraggable);

			m_List.ItemDrag += new ItemDragEventHandler(OnListBeginItemDrag);
			m_List.SelectedIndexChanged += new EventHandler(OnListSelectionChange);
			m_List.KeyUp += new KeyEventHandler(OnListKeyUp);

			m_List.BoundSelectionEnded += (s, e) => { SelectionChange?.Invoke(this, SelectedTaskIds); };
			m_List.GotFocus += (s, e) => { GotFocus?.Invoke(this, new EventArgs()); };

			base.DragOver += new DragEventHandler(OnDragOver);
			base.DragLeave += new EventHandler(OnDragLeave);
			base.DragDrop += new DragEventHandler(OnDragDrop);

			foreach (Control c in Controls)
			{
				c.DragOver += new DragEventHandler(OnDragOver);
				c.DragLeave += new EventHandler(OnDragLeave);
				c.DragDrop += new DragEventHandler(OnDragDrop);
			}
		}

		public EisenhowerPaneMatrix Matrix
		{
			get { return m_Matrix; }

			set
			{
				if ((m_Matrix == null) || !m_Matrix.Equals(value))
				{
					m_Matrix = value;
					UpdateTitle();

					m_List.SetAttributeNames(value.XVariable.Attribute.Label, value.YVariable.Attribute.Label, false);
					RebuildTaskList();
				}
			}
		}

		public bool Selected
		{
			get { return m_List.Selected; }

			set
			{
				if (value != m_List.Selected)
				{
					m_List.Selected = value; // Must come first

					m_TitleBar.Font = new Font(Font, (value ? FontStyle.Bold : FontStyle.Regular));
					m_TitleBar.ForeColor = TitleTextColor;

					BackColor = TitleBackColor;
				}
			}
		}

		public Color GridlineColor				{ set { m_List.GridlineColor = value; } }
		public Color AlternateLineColor			{ set { m_List.AlternateLineColor = value; } }

		public bool TaskColorIsBackground		{ set { m_List.TaskColorIsBackground = value; } }
		public bool ShowParentsAsFolders		{ set { m_List.ShowParentsAsFolders = value; } }
		public bool ShowCompletionCheckboxes	{ set { m_List.ShowCompletionCheckboxes = value; } }
		public bool ShowLabelTips				{ set { m_List.ShowLabelTips = value; } }

		public bool HasSelection				{ get { return m_List.HasSelection; } }
		public List<uint> SelectedTaskIds		{ get {	return m_List.SelectedTaskIds; } }
		public uint FirstTaskId					{ get { return m_List.FirstTaskId; } }
		public uint LastTaskId					{ get { return m_List.LastTaskId; } }
		public uint FirstSelectedTaskId			{ get { return m_List.FirstSelectedTaskId; } }
		public uint LastSelectedTaskId			{ get { return m_List.LastSelectedTaskId; } }
		public bool IsBoundSelecting			{ get { return m_List.IsBoundSelecting; } }

		public bool ReadOnly
		{
			get { return m_ReadOnly; }
			set
			{
				m_ReadOnly = m_List.ReadOnly = value;
				AllowDrop = !m_ReadOnly;

				foreach (Control c in Controls)
					c.AllowDrop = (value == false);
			}
		}

		public void SetUITheme(UITheme theme)
		{
			m_Theme = theme; // Must come first

			BackColor = TitleBackColor;
			m_TitleBar.ForeColor = TitleTextColor;
		}

		public bool DropHighlight
		{
			set
			{
				Debug.Assert(!value || !m_ReadOnly);

				if (value != m_DropHighlight)
				{
					m_DropHighlight = value; // Must come first

					BackColor = TitleBackColor;
					m_TitleBar.ForeColor = TitleTextColor;

					m_List.BackColor = (m_DropHighlight ? BackColor : Color.Empty);
				}
			}
		}

		public bool ShowMixedCompletionState
		{
			set
			{
				if (m_ShowMixedCompletionState != value)
				{
					m_ShowMixedCompletionState = value;
					Invalidate();
				}
			}
		}

		public bool SelectTask(uint taskID)
		{
			return m_List.SelectTask(taskID);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return m_List.SelectTask(text, selectTask, caseSensitive, wholeWord, wholeWord);
		}

		public Rectangle SelectedTaskLabelRect
		{
			get
			{
				var labelRect = m_List.SelectedTaskLabelRect;

				if (!labelRect.IsEmpty)
					return RectangleToClient(m_List.RectangleToScreen(labelRect));

				return Rectangle.Empty;
			}
		}

		public Rectangle GetTaskLabelRect(uint taskId)
		{
			var labelRect = m_List.GetTaskLabelRect(taskId);

			if (!labelRect.IsEmpty)
				return RectangleToClient(m_List.RectangleToScreen(labelRect));

			return Rectangle.Empty;
		}

		public bool SelectTasks(IList<uint> taskIDs)
		{
			return m_List.SelectTasks(taskIDs);
		}

		public uint HitTestTask(Point screenPos)
		{
			return m_List.HitTestTask(screenPos);
		}

		public uint GetTaskId(UIExtension.GetTask getTask)
		{
			return m_List.GetTaskId(getTask);
		}

		public new bool Focus()
		{
			if (Focused)
				return false;

			// else
			return Win32.SetFocus(m_List.Handle);
		}

		public new bool Focused
		{
			get { return m_List.Focused; }
		}

		public bool RebuildTaskList()
		{
			var selTaskIds = (Selected ? SelectedTaskIds : null);

			m_List.BeginUpdate();
			m_List.RemoveAll();

			if ((m_Tasks == null) || (m_Matrix == null))
			{
				Debug.Assert(false);
				return false;
			}

			foreach (var task in m_Tasks.Values)
				AddTask(task);

			m_List.RefreshColumnWidths();
			m_List.EndUpdate();

			// Restore as much of the previous selection as possible
			if (selTaskIds?.Count > 0)
			{
				bool change = (selTaskIds.RemoveAll(id => !m_List.HasTaskId(id)) != 0);
				m_List.SelectTasks(selTaskIds);

				if (change)
					SelectionChange?.Invoke(this, selTaskIds);
			}

			return true;
		}

		public bool UpdateTasks(List<uint> taskIds)
		{
			if ((m_Tasks == null) || (m_Matrix == null))
			{
				Debug.Assert(false);
				return false;
			}

			bool someModified = false;

			foreach (var taskId in taskIds)
			{
				var task = m_Tasks.GetItem(taskId);

				// Check if we need to remove or add it
				if ((task == null) || !m_Matrix.TaskMatches(task))
				{
					// No need to check if we already have it,
					// the list will do that automatically
					someModified |= m_List.RemoveTask(taskId);
				}
				else if (!m_List.HasTaskId(taskId))
				{
					someModified |= AddTask(task);
				}
				else // update values
				{
					m_List.SetTaskValues(taskId, 
										 task.GetAttributeDisplayValue(m_Matrix.XVariable),
										 task.GetAttributeDisplayValue(m_Matrix.YVariable));
				}
			}

			if (someModified)
				m_List.RefreshColumnWidths();

			return someModified;
		}

		public void DrawDragImage(Graphics graphics, object unused, Size size)
		{
			m_List.DrawDragImage(graphics, size);
		}

		public Size GetDragImageSize()
		{
			return m_List.GetDragImageSize();
		}

		public void EnsureSelectionVisible()
		{
			m_List.EnsureSelectionVisible();
		}

		// --------------------------------------------------------
		// Message Handlers

		private Rectangle TitleRect
		{
			get
			{
				var titleRect = ClientRectangle;

				titleRect.Height = m_List.Top;
				titleRect.Width--;

				return titleRect;
			}
		}

		private Color TitleBorderColor
		{
			get
			{
				if (Selected)
					return UIExtension.SelectionRect.GetBorderColor(UIExtension.SelectionRect.Style.Selected);

				if (m_DropHighlight)
					return UIExtension.SelectionRect.GetBorderColor(UIExtension.SelectionRect.Style.DropHighlighted);

				if (UITheme.IsDarkMode())
					return SystemColors.ButtonHighlight;
					
				if (DrawingColor.GetLuminance(BackColor) > 0.5)
					return SystemColors.ButtonShadow;

				//else
				return SystemColors.ButtonHighlight;
			}
		}

		private Color TitleBackColor
		{
			get
			{
				if (Selected)
					return UIExtension.SelectionRect.GetColor(UIExtension.SelectionRect.Style.Selected);

				if (m_DropHighlight)
					return UIExtension.SelectionRect.GetColor(UIExtension.SelectionRect.Style.DropHighlighted);

				// else
				return m_Theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			}
		}

		private Color TitleTextColor
		{
			get
			{
				if (Selected || m_DropHighlight)
					return SystemColors.ControlText;

				// else
				return m_Theme.GetAppDrawingColor(UITheme.AppColor.AppText);
			}
		}

		protected override void OnPaintBackground(PaintEventArgs e)
		{
			base.OnPaintBackground(e);

			var titleRect = TitleRect;

			if (titleRect.IntersectsWith(e.ClipRectangle))
			{
				using (var pen = new Pen(TitleBorderColor))
					e.Graphics.DrawRectangle(pen, titleRect);
			}
		}

		private void OnListSelectionChange(object sender, EventArgs e)
		{
			// Don't forward selection changes:

			// 1. If bounds selecting
			if (m_List.IsBoundSelecting)
				return;

			// 2. If NOT bounds selecting and nothing is selected
			if (m_List.SelectionCount == 0)
				return;

			// 3. During keyboard navigation
			if (System.Windows.Input.Keyboard.IsKeyDown(System.Windows.Input.Key.Up) ||
				System.Windows.Input.Keyboard.IsKeyDown(System.Windows.Input.Key.Down) ||
				System.Windows.Input.Keyboard.IsKeyDown(System.Windows.Input.Key.PageUp) ||
				System.Windows.Input.Keyboard.IsKeyDown(System.Windows.Input.Key.PageDown))
				return;

			SelectionChange?.Invoke(this, m_List.SelectedTaskIds);
		}

		private void OnListKeyUp(object sender, KeyEventArgs e)
		{
			switch (e.KeyCode)
			{
			case Keys.Up:
			case Keys.Down:
			case Keys.PageUp:
			case Keys.PageDown:
				SelectionChange?.Invoke(this, m_List.SelectedTaskIds);
				return;
			}
		}

		protected override void OnFontChanged(EventArgs e)
		{
			base.OnFontChanged(e);

			if (Selected)
				m_TitleBar.Font = new Font(Font, (Selected ? FontStyle.Bold : FontStyle.Regular));

			m_List.Font = Font;
			ResizeList();
		}

		private void ResizeList()
		{
			// Winforms scales the list size whenever the font changes 
			// but without resizing it to fit within the pane's extents.
			// So we do it ourselves
			var listRect = m_List.Bounds;

			listRect.Width = Width;
			listRect.Height = (Height - listRect.Top);

			m_List.Bounds = listRect;
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);

			Invalidate(TitleRect);
			ResizeList();
		}

		private bool OnListEditTaskDone(object sender, ITaskBase task)
		{
			if (m_ReadOnly)
				return false;

			return (bool)EditTaskDone?.Invoke(sender, task);
		}

		private bool OnListEditTaskIcon(object sender, ITaskBase task)
		{
			if (m_ReadOnly)
				return false;

			return (bool)EditTaskIcon?.Invoke(this, task);
		}

		private bool OnListEditTaskLabel(object sender, ITaskBase task)
		{
			if (m_ReadOnly)
				return false;

			return (bool)EditTaskLabel?.Invoke(this, task);
		}

		private bool OnListIsTaskDraggable(object sender, ITaskBase task)
		{
			if (m_Matrix.XVariable.ReadOnly && m_Matrix.YVariable.ReadOnly)
				return false;

			return (bool)IsTaskDraggable?.Invoke(this, task);
		}

		private void OnListBeginItemDrag(object sender, ItemDragEventArgs e)
		{
#if DEBUG
			var item = (e.Item as ListViewItem);
			var task = m_List.GetTask(item.Index);

			Debug.Assert(OnListIsTaskDraggable(m_List, task));
#endif

			DragBegin?.Invoke(this, new EventArgs());
			DoDragDrop(this, DragDropEffects.Move);
		}

		private bool IsValidDragData(IDataObject obj)
		{
			return (obj.GetData(typeof(EisenhowerPane)) != null);
		}

		private void OnDragLeave(object sender, EventArgs e)
		{
			DragLeave?.Invoke(this, e);
		}

		private void OnDragOver(object sender, DragEventArgs e)
		{
			if (IsValidDragData(e.Data))
				DragOver?.Invoke(this, e);
		}

		private void OnDragDrop(object sender, DragEventArgs e)
		{
			if (IsValidDragData(e.Data))
				DragDrop?.Invoke(this, e);
		}

		private void UpdateTitle()
		{
			m_TitleBar.Text = m_Matrix?.ToString(m_Trans);
		}

		private bool AddTask(EisenhowerTask task)
		{
			if (!m_Matrix.TaskMatches(task))
				return false;

			return m_List.AddTask(task,
								  task.GetAttributeDisplayValue(m_Matrix.XVariable),
								  task.GetAttributeDisplayValue(m_Matrix.YVariable));
		}
	}
}
