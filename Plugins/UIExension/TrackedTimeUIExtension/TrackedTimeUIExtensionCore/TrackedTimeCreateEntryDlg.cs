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
		LogEntry m_DefaultAttrib;

		// ---------------------------------------

		public TrackedTimeCreateEntryDlg()
		{
			InitializeComponent();
		}

		public TrackedTimeCreateEntryDlg(IEnumerable<TaskItem> taskItems, 
										 UIExtension.TaskIcon taskIcons, 
										 WorkingWeek workWeek,
										 LogEntry attrib)
			:
			this()
		{
			m_DefaultAttrib = attrib;

			if (attrib.TaskId != 0)
				m_TaskIdLabel.Text = attrib.TaskId.ToString();

			m_TaskCombo.Initialise(taskItems.OrderBy(x => x.Position), taskIcons, attrib.TaskId);
			m_Attributes.Initialise(workWeek, attrib);
		}

		public uint SelectedTaskId
		{
			get { return m_TaskCombo.SelectedTaskId; }
		}

		public LogEntry Entry
		{
			get
			{
				return new LogEntry(m_DefaultAttrib, 0)
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
