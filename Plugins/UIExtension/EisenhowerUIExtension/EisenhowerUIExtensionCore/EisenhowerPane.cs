using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public delegate void SelectionChangeEventHandler(Object sender, UInt32 taskId);

	// ---------------------------------------------

	public partial class EisenhowerPane : UserControl
	{
		private TaskItems m_Tasks;
		private EisenhowerPaneFilter m_Filter;
		private Translator m_Trans;

		private bool m_ShowMixedCompletionState;
		private bool m_DropHighlighted;
		private Color m_BackColor;

		// ---------------------------------------------

		// From TaskListView
		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;

		// Local 
		public event SelectionChangeEventHandler SelectionChange;

		public new event EventHandler DragLeave;
 		public new event DragEventHandler DragOver;
		public new event DragEventHandler DragDrop;

		new public event EventHandler GotFocus;

		// ---------------------------------------------

		public EisenhowerPane()
		{
			InitializeComponent();
		}

		public void Initialize(string paneTitle,
							   Bitmap paneIcon,
							   Translator trans, 
							   TaskItems taskItems, 
							   UIExtension.TaskIcon taskIcons)
		{
			m_TitleBar.Text = paneTitle;
			m_Icon.Image = paneIcon;

			m_Tasks = taskItems;
			m_Filter = null;
			m_Trans = trans;

			m_List.Initialize(trans, taskIcons);
			m_List.AllowDrop = true;

			m_List.EditTaskDone += new EditTaskCompletionEventHandler(OnTaskMatchesEditTaskDone);
			m_List.EditTaskIcon += new EditTaskIconEventHandler(OnTaskMatchesEditTaskIcon);
			m_List.EditTaskLabel += new EditTaskLabelEventHandler(OnTaskMatchesEditTaskLabel);

			m_List.ItemDrag  += (s, e) =>
			{
				if (m_List.AllowDrop)
					DoDragDrop(this, DragDropEffects.Move);
			};

			m_List.SelectedIndexChanged += (s, e) =>
			{
				UInt32 selTaskId = m_List.SelectedTaskId;

				if (selTaskId != 0)
					SelectionChange?.Invoke(this, selTaskId);
			};

			m_List.GotFocus += (s, e) => { GotFocus?.Invoke(this, new EventArgs()); };

			base.DragOver += (s, e) => DragOver?.Invoke(this, e);
			base.DragLeave += (s, e) => DragLeave?.Invoke(this, e);
			base.DragDrop += (s, e) => DragDrop?.Invoke(this, e);

			base.QueryContinueDrag += (s, e) =>
			{
				if (e.EscapePressed)
				{
					e.Action = DragAction.Cancel;
					DragLeave?.Invoke(this, e);
				}
			};

			foreach (Control c in Controls)
			{
				c.DragOver += (s, e) => DragOver?.Invoke(this, e);
				c.DragLeave += (s, e) => DragLeave?.Invoke(this, e);
				c.DragDrop += (s, e) => DragDrop?.Invoke(this, e);
			}
		}

		public EisenhowerPaneFilter Filter { get { return m_Filter; } }

		public bool SetFilter(string xAttribTitle,
							  EisenhowerPaneFilterAttribute xAttrib,
							  string yAttribTitle,
							  EisenhowerPaneFilterAttribute yAttrib)
		{
			var newFilter = new EisenhowerPaneFilter(xAttrib, yAttrib);

			if ((m_Filter == null) || !m_Filter.Equals(newFilter))
			{
				m_List.XAttribTitle = xAttribTitle;
				m_List.YAttribTitle = yAttribTitle;
				m_Filter = newFilter;

				UpdateTitle();
				RefreshListItems();

				return true;
			}

			return false; // no change
		}

		public bool Selected
		{
			get { return m_List.Selected; }

			set
			{
				if (value != m_List.Selected)
					m_TitleBar.Font = new Font(Font, (value ? FontStyle.Bold : FontStyle.Regular));

				m_List.Selected = value;
			}
		}

		public Color GridlineColor				{ set { m_List.GridlineColor = value; } }
		public Color AlternateLineColor			{ set { m_List.AlternateLineColor = value; } }
// 		public Color ListBackColor				{ set { m_List.BackColor = value; } }

		public bool TaskColorIsBackground		{ set { m_List.TaskColorIsBackground = value; } }
		public bool ShowParentsAsFolders		{ set { m_List.ShowParentsAsFolders = value; } }
		public bool ShowCompletionCheckboxes	{ set { m_List.ShowCompletionCheckboxes = value; } }
		public bool ShowLabelTips				{ set { m_List.ShowLabelTips = value; } }

		public bool HasSelection				{ get { return (m_List.SelectedItems.Count > 0); } }
		public uint SelectedTaskId				{ get {	return m_List.SelectedTaskId; } }
		public uint FirstTaskId					{ get { return m_List.FirstTaskId; } }
		public uint LastTaskId					{ get { return m_List.LastTaskId; } }

		public bool ReadOnly
		{
			set
			{
				AllowDrop = (value == false);

				foreach (Control c in Controls)
					c.AllowDrop = (value == false);
			}
		}

		public bool DropHighlighted
		{
			set
			{
				if (value != m_DropHighlighted)
				{
					m_DropHighlighted = value;

					if (m_DropHighlighted)
					{
						m_BackColor = BackColor;

						BackColor = UIExtension.SelectionRect.GetColor(UIExtension.SelectionRect.Style.Selected);
						m_List.BackColor = UIExtension.SelectionRect.GetColor(UIExtension.SelectionRect.Style.DropHighlighted);
					}
					else
					{
						BackColor = m_BackColor;
						m_List.BackColor = Color.Empty;
					}
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

		public bool SelectTasks(uint[] taskIDs)
		{
			// TODO 
			return true;
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
			return m_List.Focus();
		}

		public new bool Focused
		{
			get { return m_List.Focused; }
		}

		public bool RefreshListItems()
		{
			m_List.Items.Clear();

			if ((m_Tasks == null) || (m_Filter == null))
			{
				Debug.Assert(false);
				return false;
			}

			foreach (var task in m_Tasks.Values)
			{
				if (m_Filter.TaskMatches(task))
				{
					var lvItem = m_List.AddTask(task);

					if (lvItem != null)
					{
						lvItem.SubItems.Add(task.GetAttributeValue(m_Filter.XAttribute.Id).ToString());
						lvItem.SubItems.Add(task.GetAttributeValue(m_Filter.YAttribute.Id).ToString());
					}
				}
			}

			return true;
		}

		public void DrawDragImage(Graphics graphics, Rectangle rect)
		{
			m_List.DrawDragImage(graphics, rect);
		}

		// --------------------------------------------------------
		// Message Handlers

// 		private void OnDragEnter(object sender, DragEventArgs e)
// 		{
// 			if (!(sender is EisenhowerPaneListView) || (sender == m_List))
// 				e.Effect = DragDropEffects.None;
// 			else
// 				e.Effect = e.AllowedEffect;
// 		}
// 
// 		private void OnDragOver(object sender, DragEventArgs e)
// 		{
// 			int breakpoint = 0;
// 
// 		}
// 
// 		private void OnDragDrop(object sender, DragEventArgs e)
// 		{
// 			int breakpoint = 0;
// 
// 		}

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

			ResizeList();
		}

		private bool OnTaskMatchesEditTaskDone(object sender, UInt32 taskId, bool completed)
		{
			return (bool)EditTaskDone?.Invoke(sender, taskId, completed);
		}

		private bool OnTaskMatchesEditTaskIcon(object sender, UInt32 taskId)
		{
			return (bool)EditTaskIcon?.Invoke(sender, taskId);
		}

		private bool OnTaskMatchesEditTaskLabel(object sender, UInt32 taskId)
		{
			return (bool)EditTaskLabel?.Invoke(sender, taskId);
		}
		
		private void UpdateTitle()
		{
			if (m_Filter != null)
			{
				m_TitleBar.Text = (FormatAttribute(m_List.XAttribTitle, m_Filter.XAttribute) +
									" - " +
									FormatAttribute(m_List.YAttribTitle, m_Filter.YAttribute));
			}
		}

		private string FormatAttribute(string title, EisenhowerPaneFilterAttribute attrib)
		{
			if (attrib.Range == EisenhowerPaneFilterAttributeRange.High)
				return string.Format("High {0} (>= {1})", title, attrib.Cutoff);

			// else
			return string.Format("Low {0} (< {1})", title, attrib.Cutoff);
		}

	}
}
