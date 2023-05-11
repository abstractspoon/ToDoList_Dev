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

			if (dates != null)
			{
				m_FromDateCtrl.Value = dates.Start.Date;
				m_ToDateCtrl.Value = dates.End.Date;
				m_ToDateCtrl.Checked = (dates.Start.Date != dates.End.Date);

				m_FromTimeCombo.SetTime(DateUtil.TimeOnly(dates.Start));
				m_ToTimeCombo.SetTime(DateUtil.TimeOnly(dates.End));
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
			get { return m_DowListBox.GetSelectedDays(); }
			set { m_DowListBox.SetSelectedDays(value); }
		}
	}
}
