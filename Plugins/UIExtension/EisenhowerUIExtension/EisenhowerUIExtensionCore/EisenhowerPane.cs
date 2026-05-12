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
	public delegate void TaskSelectionEventHandler(Object sender, UInt32 taskId);

	// ---------------------------------------------
	
	public partial class EisenhowerPane : UserControl
	{
		private TaskItems m_Tasks;
		private EisenhowerPaneFilter m_Filter;
		private Translator m_Trans;

		private bool m_ShowMixedCompletionState;
		private string m_XAttribTitle, m_YAttribTitle;

		// ---------------------------------------------

		public event EditTaskLabelEventHandler EditTaskLabel;
		public event EditTaskIconEventHandler EditTaskIcon;
		public event EditTaskCompletionEventHandler EditTaskDone;
		public event TaskSelectionEventHandler SelectionChange;

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

			m_List.EditTaskDone += new EditTaskCompletionEventHandler(OnTaskMatchesEditTaskDone);
			m_List.EditTaskIcon += new EditTaskIconEventHandler(OnTaskMatchesEditTaskIcon);
			m_List.EditTaskLabel += new EditTaskLabelEventHandler(OnTaskMatchesEditTaskLabel);

			m_List.SelectedIndexChanged += (s, e) =>
			{
				UInt32 selTaskId = m_List.SelectedTaskId;

				if (selTaskId != 0)
					SelectionChange?.Invoke(this, selTaskId);
			};

			m_List.GotFocus += (s, e) => { GotFocus?.Invoke(this, new EventArgs()); };
		}

		public bool SetFilter(string xAttribTitle,
							  EisenhowerPaneFilterAttribute xAttrib,
							  string yAttribTitle,
							  EisenhowerPaneFilterAttribute yAttrib)
		{
			var newFilter = new EisenhowerPaneFilter(xAttrib, yAttrib);

			if ((m_Filter == null) || !m_Filter.Equals(newFilter))
			{
				m_XAttribTitle = xAttribTitle;
				m_YAttribTitle = yAttribTitle;
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
		public bool TaskColorIsBackground		{ set { m_List.TaskColorIsBackground = value; } }
		public bool ShowParentsAsFolders		{ set { m_List.ShowParentsAsFolders = value; } }
		public bool ShowCompletionCheckboxes	{ set { m_List.ShowCompletionCheckboxes = value; } }
		public bool HasSelection				{ get { return (m_List.SelectedItems.Count > 0); } }

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
			return m_List.SelectTaskEx(text, selectTask, caseSensitive, wholeWord, wholeWord);
		}

		public Rectangle SelectedItemLabelRect
		{
			get
			{
				var labelRect = m_List.SelectedTaskLabelRect;

				if (!labelRect.IsEmpty)
					return RectangleToClient(m_List.RectangleToScreen(labelRect));

				return Rectangle.Empty;
			}
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
			return m_List.GetTaskIdEx(getTask);
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

		// --------------------------------------------------------
		// Message Handlers

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
				m_TitleBar.Text = (FormatAttribute(m_XAttribTitle, m_Filter.XAttribute) + " - " + FormatAttribute(m_YAttribTitle, m_Filter.YAttribute));
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
