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
		private UIExtension.TaskIcon m_Icons;
		private Translator m_Trans;
		private EisenhowerPaneFilter m_Filter;

		private bool m_ShowParentAsFolder;
		private bool m_TaskColorIsBkgnd;
		private bool m_ShowMixedCompletionState;
		private bool m_ShowCompletionCheckboxes;

		// ---------------------------------------------

		public EisenhowerPane()
		{
			InitializeComponent();
		}

		public void Initialize(Translator trans, 
							   TaskItems taskItems, 
							   UIExtension.TaskIcon taskIcons,
							   EisenhowerPaneFilter filter = null)
		{
			m_Trans = trans;
			m_Tasks = taskItems;
			m_Icons = taskIcons;
			m_Filter = filter;

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
					m_List.Items.Add(task.ToString());
			}

			return true;
		}

		// --------------------------------------------------------
		// Message Handlers

		private CheckBoxState GetItemCheckboxState(TaskItem taskItem)
		{
			if (taskItem.SomeSubtasksDone && ShowMixedCompletionState)
				return CheckBoxState.MixedNormal;

			// else
			return CheckBoxState.UncheckedNormal;
		}

		private bool TaskHasIcon(TaskItem taskItem)
		{
			if ((m_Icons == null) || (taskItem == null))
				return false;

			return (taskItem.HasIcon || (m_ShowParentAsFolder && taskItem.IsParent));
		}


	}
}
