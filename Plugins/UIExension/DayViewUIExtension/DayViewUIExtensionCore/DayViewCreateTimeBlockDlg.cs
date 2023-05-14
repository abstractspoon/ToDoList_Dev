using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	public partial class DayViewCreateTimeBlockDlg : Form
	{
		public DayViewCreateTimeBlockDlg()
		{
			InitializeComponent();
		}

		public DayViewCreateTimeBlockDlg(IEnumerable<TaskItem> taskItems, 
										 UIExtension.TaskIcon taskIcons, 
										 WorkingWeek workWeek,
										 uint taskId,
										 Calendar.AppointmentDates dates)
			:
			this()
		{
			m_TaskCombo.Initialise(taskItems, taskIcons, taskId);
			TimeComboBox.SetWorkingWeek(workWeek);

			m_DowListBox.SetSelectedDays(workWeek.WeekDays()); // default

			if (dates != null)
			{
				m_FromDateCtrl.Value = dates.Start.Date;
				m_ToDateCtrl.Value = dates.End.Date.AddDays(1);
				m_ToDateCtrl.Checked = (dates.Start.Date != dates.End.Date);

				m_FromTimeCombo.SetTime(DateUtil.TimeOnly(dates.Start));
				m_ToTimeCombo.SetTime(DateUtil.TimeOnly(dates.End));

				if (m_FromDateCtrl.Value <= m_ToDateCtrl.Value)
				{
					var days = new List<DayOfWeek>();
					var date = m_FromDateCtrl.Value;

					do
					{
						if (!workWeek.IsWeekend(date))
							days.Add(date.DayOfWeek);

						date = date.AddDays(1);
					}
					while ((date < m_ToDateCtrl.Value) && (days.Count <= 7));

					m_DowListBox.SetSelectedDays(days);
				}
			}
		}

		public uint SelectedTaskId
		{
			get { return m_TaskCombo.SelectedTaskId; }
		}

		public DateTime FromDate { get { return m_FromDateCtrl.Value.Date; } }
		public DateTime ToDate
		{
			get
			{
				if (m_ToDateCtrl.Checked)
					return m_ToDateCtrl.Value.Date;

				return FromDate;
			}
		}

		public TimeSpan FromTime { get { return m_FromTimeCombo.GetTime(); } }
		public TimeSpan ToTime { get { return m_ToTimeCombo.GetTime(); } }

		public List<DayOfWeek> DaysOfWeek
		{
			get
			{
				if (m_DowCheckBox.Checked)
					return m_DowListBox.GetSelectedDays();
				
				// else
				return DateUtil.DaysOfWeek();
			}

			set { m_DowListBox.SetSelectedDays(value); }
		}

		public bool SyncTimeBlocksToTaskDates { get { return m_SyncToDatesRadioBtn.Checked; } }

		private void OnDowCheckChange(object sender, EventArgs e)
		{
			m_DowListBox.Enabled = m_DowCheckBox.Checked;
		}

		private void OnDateOptionCheckChange(object sender, EventArgs e)
		{
			m_FromDateCtrl.Enabled = m_FixedDatesRadioBtn.Checked;
			m_ToDateCtrl.Enabled = m_FixedDatesRadioBtn.Checked;
		}
	}
}
