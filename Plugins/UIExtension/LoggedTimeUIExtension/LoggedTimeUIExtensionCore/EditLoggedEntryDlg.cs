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
		Translator m_Trans;

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
				m_TaskTitle.Text = m_TaskId.Text = trans.Translate("<none>", Translator.Type.Text);
			}
			else
			{
				m_TaskTitle.Text = entry.Title;
				m_TaskId.Text = entry.TaskId.ToString();
			}

			m_Attributes.Initialise(entry, workWeek, isoDateTimes, readonlyTask, true, trans);
			m_Attributes.ChangeEvent += (s, e) => ValidateInputs();

			m_Trans = trans;
			m_Trans.Translate(this);
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

		protected void ValidateInputs()
		{
			bool validInputs = (m_Attributes.Dates.IsValid &&
								((m_Attributes.TimeSpent > 0.0) || !String.IsNullOrWhiteSpace(m_Attributes.Comment)));

			if (validInputs)
			{
				OK.Enabled = true;
				m_Error.Text = string.Empty;
			}
			else
			{
				OK.Enabled = false;

				if (!m_Attributes.Dates.IsValid)
				{
					m_Error.Text = m_Trans.Translate("Invalid 'End' time", Translator.Type.Text);
				}
				else
				{
					m_Error.Text = m_Trans.Translate("'Time Spent' or 'Comment' required", Translator.Type.Text);
				}
			}
		}

	}


}
