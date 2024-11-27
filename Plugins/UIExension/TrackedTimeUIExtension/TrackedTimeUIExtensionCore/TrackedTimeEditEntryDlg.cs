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

		public TrackedTimeEditEntryDlg(TaskItem taskItem, LogEntry entry, WorkingWeek workWeek)
			:
			this()
		{
			m_Entry = entry;

			m_TaskLabel.Text = taskItem.Title;
			m_TaskIdLabel.Text = entry.TaskId.ToString();

			m_Attributes.Initialise(workWeek, entry);
		}

		public LogEntry Entry
		{
			get
			{
				return new LogEntry(m_Entry)
				{
					StartDate = m_Attributes.From,
					EndDate = m_Attributes.To,
					Comment = m_Attributes.Comment,
					TimeSpentInHrs = m_Attributes.TimeSpent
				};
			}
		}
	}


}
