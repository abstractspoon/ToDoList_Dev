using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	public partial class DayViewTimeBlockAttributesPage : UserControl
	{
		[Flags]
		public enum Mask
		{
			None = 0x00,

			Date = 0x01,
			Time = 0x02,
			Dow  = 0x04,

			All  = 0xff
		}

		private bool m_EditMode = false;

		public DayViewTimeBlockAttributesPage()
		{
			InitializeComponent();
			EnableDisableControls();
		}

		public void Initialise(WorkingWeek workWeek, Calendar.AppointmentDates dates, bool editMode)
		{
			TimeComboBox.SetWorkingWeek(workWeek);

			m_DowListBox.SetSelectedDays(workWeek.WeekDays()); // default
			m_EditMode = editMode;

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

		private void EnableDisableControls()
		{
			m_DateCheckBox.Visible = m_EditMode;
			m_DateLabel.Visible = !m_EditMode;

			m_TimeCheckBox.Visible = m_EditMode;
			m_TimeLabel.Visible = !m_EditMode;

			m_FromDateCtrl.Enabled = m_FixedDatesRadioBtn.Checked;
			m_ToDateCtrl.Enabled = m_FixedDatesRadioBtn.Checked;

			m_DowListBox.Enabled = m_DowCheckBox.Checked;

			if (m_EditMode)
			{
				m_FromDateCtrl.Enabled &= m_DateCheckBox.Checked;
				m_ToDateCtrl.Enabled &= m_DateCheckBox.Checked;

				m_FromTimeCombo.Enabled &= m_TimeCheckBox.Checked;
				m_ToTimeCombo.Enabled &= m_TimeCheckBox.Checked;
			}
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
			EnableDisableControls();
		}

		private void OnDateOptionCheckChange(object sender, EventArgs e)
		{
			EnableDisableControls();
		}

	}
}
