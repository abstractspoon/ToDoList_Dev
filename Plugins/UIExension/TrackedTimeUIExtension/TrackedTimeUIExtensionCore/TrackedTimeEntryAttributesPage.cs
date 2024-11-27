using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace TrackedTimeUIExtension
{
	public partial class TrackedTimeEntryAttributesPage : UserControl
	{
		private LogEntry m_Entry;

		public TrackedTimeEntryAttributesPage()
		{
			InitializeComponent();
		}

		public void Initialise(WorkingWeek workWeek, LogEntry entry)
		{
			TimeComboBox.SetWorkingWeek(workWeek);

			SetDates(workWeek, entry);

			m_Entry = entry;
		}

		public LogEntry Entry
		{
			get
			{
				double hours = 0.0;

				if (!double.TryParse(m_TimeSpentEdit.Text, out hours))
					hours = m_Entry.TimeSpentInHrs;

				return new LogEntry(m_Entry)
				{
					StartDate = (m_FromDateCtrl.Value.Date + m_FromTimeCombo.GetTime()),
					EndDate = (m_FromDateCtrl.Value.Date + m_ToTimeCombo.GetTime()),
					Comment = m_CommentEdit.Text,
					TimeSpentInHrs = hours
				};
			}
		}

		public bool WantAddToTimeSpent
		{
			get { return m_AddToTimeSpentCheckBox.Checked; }
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
				m_FromTimeCombo.SetTime(entry.Dates.Start.Date);
				m_ToTimeCombo.SetTime(entry.Dates.End.Date);
			}
		}

	}
}
