using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace LoggedTimeUIExtension
{
	public partial class EditLoggedEntryDlg : Form
	{
		LogEntry m_Entry;

		// ---------------------------------------

		public EditLoggedEntryDlg()
		{
			InitializeComponent();
		}

		public EditLoggedEntryDlg(LogEntry entry, 
								  WorkingWeek workWeek, 
								  bool isoDateTimes,
								  bool readonlyTask, 
								  Translator trans)
			:
			this()
		{
			m_Entry = entry;

			if (entry.TaskId == 0)
			{
				m_TaskTitle.Text = m_TaskId.Text = "<none>";
			}
			else
			{
				m_TaskTitle.Text = entry.Title;
				m_TaskId.Text = entry.TaskId.ToString();
			}

			m_Attributes.Initialise(entry, workWeek, isoDateTimes, readonlyTask, true, trans);
		}

		public Calendar.AppointmentDates Dates
		{
			get { return m_Attributes.Dates; }
		}

		public string Comment
		{
			get { return m_Attributes.Comment; }
		}

		public double TimeSpent
		{
			get { return m_Attributes.TimeSpent; }
		}

		public Color FillColor
		{
			get { return m_Attributes.FillColor; }
		}

		public double HoursToAddToTimeSpent
		{
			get { return m_Attributes.HoursToAddToTimeSpent; }
		}


	}


}
