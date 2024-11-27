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
	public partial class TrackedTimeEditEntryDlg : Form
	{
		LogEntry m_Entry;

		// ---------------------------------------

		public TrackedTimeEditEntryDlg()
		{
			InitializeComponent();
		}

		public TrackedTimeEditEntryDlg(LogEntry entry, WorkingWeek workWeek, bool readonlyTask)
			:
			this()
		{
			m_Entry = entry;

			m_TaskTitleLabel.Text = entry.Title;
			m_TaskIdLabel.Text = entry.TaskId.ToString();

			m_Attributes.Initialise(entry, workWeek, readonlyTask);
		}

		public void GetAttributes(ref LogEntry entry)
		{
			entry.StartDate = m_Attributes.From;
			entry.EndDate = m_Attributes.To;
			entry.Comment = m_Attributes.Comment;
			entry.TimeSpentInHrs = m_Attributes.TimeSpent;
		}

		public bool WantAddToTimeSpent
		{
			get { return m_Attributes.WantAddToTimeSpent; }
		}


	}


}
