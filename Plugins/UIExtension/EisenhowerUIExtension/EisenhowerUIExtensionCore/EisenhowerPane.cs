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
		private EisenhowerPaneFilter m_Filter;
		private Translator m_Trans;
		private UITheme m_Theme = new UITheme();

		private bool m_ShowMixedCompletionState;
		private bool m_DropHighlighted;

		// ---------------------------------------------

		// From TaskListView
		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;

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
			m_Filter = null;
			m_Trans = trans;
			m_Theme = new UITheme();

			m_List.Initialize(trans, taskIcons);
			m_List.AllowDrop = true;

			m_List.EditTaskDone += new EditTaskCompletionEventHandler(OnListEditTaskDone);
			m_List.EditTaskIcon += new EditTaskIconEventHandler(OnListEditTaskIcon);
			m_List.EditTaskLabel += new EditTaskLabelEventHandler(OnListEditTaskLabel);
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

		public bool SetFilter(EisenhowerFilterVariable xVar, EisenhowerFilterVariable yVar)
		{
			var newFilter = new EisenhowerPaneFilter(xVar, yVar);

			if ((m_Filter == null) || !m_Filter.Equals(newFilter))
			{
				m_Filter = newFilter;
				UpdateTitle();

				// Note: column widths will be recalculated in RebuildTaskList
				m_List.SetAttributeNames(xVar.Attribute.Label, yVar.Attribute.Label, false); 

				RebuildTaskList();
				return true;
			}

			return false; // no change
		}

		public EisenhowerPaneFilter Filter { get { return m_Filter; } }

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
		public IList<uint> SelectedTaskIds		{ get {	return m_List.SelectedTaskIds; } }
		public uint FirstTaskId					{ get { return m_List.FirstTaskId; } }
		public uint LastTaskId					{ get { return m_List.LastTaskId; } }
		public uint FirstSelectedTaskId			{ get { return m_List.FirstSelectedTaskId; } }
		public uint LastSelectedTaskId			{ get { return m_List.LastSelectedTaskId; } }
		public bool IsBoundSelecting			{ get { return m_List.IsBoundSelecting; } }

		public bool ReadOnly
		{
			set
			{
				AllowDrop = (value == false);

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

		public bool DropHighlighted
		{
			set
			{
				if (value != m_DropHighlighted)
				{
					m_DropHighlighted = value; // Must come first

					BackColor = TitleBackColor;
					m_TitleBar.ForeColor = TitleTextColor;

					m_List.BackColor = (m_DropHighlighted ? BackColor : Color.Empty);
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
			m_List.BeginUpdate();
			m_List.RemoveAll();

			if ((m_Tasks == null) || (m_Filter == null))
			{
				Debug.Assert(false);
				return false;
			}

			foreach (var task in m_Tasks.Values)
				AddTask(task);

			m_List.RefreshColumnWidths();
			m_List.EndUpdate();

			return true;
		}

		public bool RefilterTasks(List<uint> taskIds)
		{
			if ((m_Tasks == null) || (m_Filter == null))
			{
				Debug.Assert(false);
				return false;
			}

			bool someModified = false;
			//m_List.BeginUpdate();

			foreach (var taskId in taskIds)
			{
				var task = m_Tasks.GetItem(taskId);

				// Check if we need to remove or add it
				if ((task == null) || !m_Filter.TaskMatches(task))
				{
					// No need to check if we already have it,
					// the list will do that automatically
					someModified |= m_List.RemoveTask(taskId);
				}
				else if (!m_List.HasTaskId(taskId))
				{
					someModified |= AddTask(task);
				}
			}
			//m_List.EndUpdate();

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

				if (m_DropHighlighted)
					return UIExtension.SelectionRect.GetBorderColor(UIExtension.SelectionRect.Style.DropHighlighted);

				if (UITheme.IsDarkMode())
					return SystemColors.ButtonHighlight;
					
				if (DrawingColor.GetLuminance(BackColor) < 0.5)
					return m_Theme.GetAppDrawingColor(UITheme.AppColor.AppLinesLight);

				//else
				return m_Theme.GetAppDrawingColor(UITheme.AppColor.AppLinesDark);
			}
		}

		private Color TitleBackColor
		{
			get
			{
				if (Selected)
					return UIExtension.SelectionRect.GetColor(UIExtension.SelectionRect.Style.Selected);

				if (m_DropHighlighted)
					return UIExtension.SelectionRect.GetColor(UIExtension.SelectionRect.Style.DropHighlighted);

				// else
				return m_Theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			}
		}

		private Color TitleTextColor
		{
			get
			{
				if (Selected || m_DropHighlighted)
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

		private bool OnListEditTaskDone(object sender, UInt32 taskId, bool completed)
		{
			return (bool)EditTaskDone?.Invoke(sender, taskId, completed);
		}

		private bool OnListEditTaskIcon(object sender, UInt32 taskId)
		{
			return (bool)EditTaskIcon?.Invoke(sender, taskId);
		}

		private bool OnListEditTaskLabel(object sender, UInt32 taskId)
		{
			return (bool)EditTaskLabel?.Invoke(sender, taskId);
		}

		private void OnListBeginItemDrag(object sender, ItemDragEventArgs e)
		{
			if (!m_List.AllowDrop)
				return;

			var item = (e.Item as ListViewItem);
			var task = m_List.GetTask(item.Index);

			if ((task == null) || task.IsLocked)
				return;

			if (!item.Selected)
				SelectTask(task.Id);

			Focus();
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
			m_TitleBar.Text = m_Filter?.ToString();
		}

		private bool AddTask(EisenhowerTask task)
		{
			if (!m_Filter.TaskMatches(task))
				return false;

			return m_List.AddTask(task,
								  task.GetAttributeValue(m_Filter.XVariable).ToString(),
								  task.GetAttributeValue(m_Filter.YVariable).ToString());
		}
	}
}
