using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace LoggedTimeUIExtension
{
	public partial class LoggedEntryAttributesPage : UserControl
	{
		private double m_OrgTimeSpent = 0.0;

		public LoggedEntryAttributesPage()
		{
			InitializeComponent();
		}

		public void Initialise(LogEntry entry, WorkingWeek workWeek, bool readonlyTask)
		{
			TimeComboBox.SetWorkingWeek(workWeek);

			SetDates(workWeek, entry);

			m_OrgTimeSpent = entry.TimeSpentInHrs;
			m_TimeSpentEdit.Text = entry.TimeSpentInHrs.ToString();
			m_CommentEdit.Text = entry.Comment;

			m_AddToTimeSpentCheckBox.Enabled = !readonlyTask;
		}

		public DateTime From
		{
			get { return (m_FromDateCtrl.Value.Date + m_FromTimeCombo.GetTime()); }
		}

		public DateTime To
		{
			get { return (m_FromDateCtrl.Value.Date + m_ToTimeCombo.GetTime()); }
		}

		public string Comment
		{
			get	{ return m_CommentEdit.Text; }
		}

		public double TimeSpent
		{
			get
			{
				double hours = 0.0;

				return (double.TryParse(m_TimeSpentEdit.Text, out hours) ? hours : m_OrgTimeSpent);
			}
		}

		public bool WantAddToTimeSpent
		{
			get
			{
				if (m_AddToTimeSpentCheckBox.Enabled)
					return m_AddToTimeSpentCheckBox.Checked;
			
				// else	
				return false;
			}
		}

		public void SetDates(WorkingWeek workWeek, LogEntry entry)
		{
			if ((entry == null) || (entry.Dates.Start.Date == DateTime.MinValue))
			{
				m_FromDateCtrl.Value = DateTime.Now.Date;
				m_FromTimeCombo.SetTime(m_FromDateCtrl.Value);
				m_ToTimeCombo.SetTime(m_FromDateCtrl.Value.AddHours(1));
			}
			else // valid start date
			{
				m_FromDateCtrl.Value = entry.Dates.Start.Date;
				m_FromTimeCombo.SetTime(entry.Dates.Start);
				m_ToTimeCombo.SetTime(entry.Dates.End);
			}
		}

	}
}
