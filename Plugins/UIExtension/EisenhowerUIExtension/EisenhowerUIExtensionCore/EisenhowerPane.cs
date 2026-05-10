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

	public partial class EisenhowerPane : UserControl
	{
		private TaskItems m_Tasks;
		private EisenhowerPaneFilter m_Filter;

		private bool m_ShowMixedCompletionState;

		// ---------------------------------------------

		public EisenhowerPane()
		{
			InitializeComponent();
		}

		public void Initialize(Translator trans, 
							   TaskItems taskItems, 
							   UIExtension.TaskIcon taskIcons,
							   Bitmap paneIcon,
							   EisenhowerPaneFilter filter = null)
		{
			m_Tasks = taskItems;
			m_Filter = filter;
			m_Icon.Image = paneIcon;

			m_List.Initialize(trans, taskIcons);

			if (m_Filter != null)
				RefreshList();
		}

		public EisenhowerPaneFilter Filter
		{
			set
			{
				if (!m_Filter.Equals(value))
				{
					m_Filter = value;
					RefreshList();
				}
			}
		}

		public Color GridlineColor
		{
			get { return m_List.GridlineColor; }
			set	{ m_List.GridlineColor = value; }
		}

		public Color AlternateLineColor
		{
			get { return m_List.AlternateLineColor; }
			set { m_List.AlternateLineColor = value; }
		}

		public bool TaskColorIsBackground
		{
			get { return m_List.TaskColorIsBackground; }
			set	{ m_List.TaskColorIsBackground = value; }
		}

		public bool ShowMixedCompletionState
		{
			get { return m_ShowMixedCompletionState; }
			set
			{
				if (m_ShowMixedCompletionState != value)
				{
					m_ShowMixedCompletionState = value;
					Invalidate();
				}
			}
		}

		public bool ShowParentsAsFolders
		{
			get { return m_List.ShowParentsAsFolders; }
			set { m_List.ShowParentsAsFolders = value; }
		}

		public bool ShowCompletionCheckboxes
		{
			get { return m_List.ShowCompletionCheckboxes; }
			set { m_List.ShowCompletionCheckboxes = value; }
		}

		public bool SelectTask(uint taskID)
		{
			// TODO 
			return true;
		}

		public bool SelectTasks(uint[] taskIDs)
		{
			// TODO 
			return true;
		}

		public bool HasSelection
		{
			get
			{
				return (m_List.SelectedItems.Count > 0);
			}
		}

		public bool RefreshList()
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
						lvItem.SubItems.Add(m_Filter.Attribute1.MaxValue.ToString());
						lvItem.SubItems.Add(m_Filter.Attribute2.MaxValue.ToString());
					}
				}
			}

			return true;
		}

		// --------------------------------------------------------
		// Message Handlers

// 		private CheckBoxState GetItemCheckboxState(TaskItem taskItem)
// 		{
// 			if (taskItem.SomeSubtasksDone && ShowMixedCompletionState)
// 				return CheckBoxState.MixedNormal;
// 
// 			// else
// 			return CheckBoxState.UncheckedNormal;
// 		}
// 
// 		private bool TaskHasIcon(TaskItem taskItem)
// 		{
// 			if ((m_TaskIcons == null) || (taskItem == null))
// 				return false;
// 
// 			return (taskItem.HasIcon || (m_ShowParentAsFolder && taskItem.IsParent));
// 		}
	}
}
