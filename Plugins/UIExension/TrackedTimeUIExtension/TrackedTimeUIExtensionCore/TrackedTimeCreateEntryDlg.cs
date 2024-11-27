using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace TrackedTimeUIExtension
{
	public partial class TrackedTimeCreateEntryDlg : Form
	{
		LogEntry m_DefaultEntry;

		// ---------------------------------------

		public TrackedTimeCreateEntryDlg()
		{
			InitializeComponent();
		}

		public TrackedTimeCreateEntryDlg(IEnumerable<TaskItem> taskItems, 
										 UIExtension.TaskIcon taskIcons, 
										 WorkingWeek workWeek,
										 LogEntry defaultEntry)
			:
			this()
		{
			m_DefaultEntry = defaultEntry;

			m_TaskCombo.Initialise(taskItems.OrderBy(x => x.Position), taskIcons, 0);
			m_Attributes.Initialise(workWeek, defaultEntry);
		}

		public uint SelectedTaskId
		{
			get { return m_TaskCombo.SelectedTaskId; }
		}

		public LogEntry Entry
		{
			get
			{
				return new LogEntry(m_DefaultEntry, 0)
				{
					TaskId = m_TaskCombo.SelectedTaskId,
					Title = m_TaskCombo.SelectedTaskTitle,
					UserId = string.Empty, // TODO
					StartDate = m_Attributes.From,
					EndDate = m_Attributes.To,
					Comment = m_Attributes.Comment,
					TimeSpentInHrs = m_Attributes.TimeSpent
				};
			}
		}

		private void OnTaskComboSelChange(object sender, EventArgs e)
		{
			OK.Enabled = (SelectedTaskId != 0);
		}
	}
}
