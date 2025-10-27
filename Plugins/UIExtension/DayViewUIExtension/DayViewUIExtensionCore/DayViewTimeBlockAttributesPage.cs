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
		}

		public void Initialise(WorkingWeek workWeek,
								bool isoDateTimes,
								TimeBlockSeriesAttributes attribs, 
								bool editMode, 
								TimeBlockSeriesAttributes.EditMask mask = TimeBlockSeriesAttributes.EditMask.None)
		{
			TimeComboBox.SetWorkingWeek(workWeek);

			m_DowListBox.SetSelectedDays(workWeek.WeekDays()); // default
			m_EditMode = editMode;

			m_FromTimeCombo.SetISOFormat(isoDateTimes);
			m_ToTimeCombo.SetISOFormat(isoDateTimes);

			DateUtil.SetShortDateFormat(m_FromDateCtrl, isoDateTimes);
			DateUtil.SetShortDateFormat(m_ToDateCtrl, isoDateTimes);

			if (attribs != null)
			{
				SetDates(attribs.Dates);

				m_FromTimeCombo.SetTime(attribs.FromTime);
				m_ToTimeCombo.SetTime(attribs.ToTime);

				m_DowListBox.SetSelectedDays(attribs.DaysOfWeek);
				m_SyncToDatesRadioBtn.Checked = attribs.SyncToTaskDates;
				m_FixedDatesRadioBtn.Checked = !attribs.SyncToTaskDates;
			}

			if (editMode)
			{
				m_DateCheckBox.Checked = mask.HasFlag(TimeBlockSeriesAttributes.EditMask.Dates);
				m_TimeCheckBox.Checked = mask.HasFlag(TimeBlockSeriesAttributes.EditMask.Times);
				m_DowCheckBox.Checked = mask.HasFlag(TimeBlockSeriesAttributes.EditMask.Dow);
			}

			EnableDisableControls();
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
				m_SyncToDatesRadioBtn.Enabled = m_DateCheckBox.Checked;
				m_FixedDatesRadioBtn.Enabled = m_DateCheckBox.Checked;

				m_FromDateCtrl.Enabled &= m_DateCheckBox.Checked;
				m_ToDateCtrl.Enabled &= m_DateCheckBox.Checked;

				m_FromTimeCombo.Enabled = m_TimeCheckBox.Checked;
				m_ToTimeCombo.Enabled = m_TimeCheckBox.Checked;
			}
		}
		
		public TimeBlockSeriesAttributes Attributes
		{
			get
			{
				return new TimeBlockSeriesAttributes()
				{
					FromDate = m_FromDateCtrl.Value.Date,
					ToDate = (m_ToDateCtrl.Checked ? m_ToDateCtrl.Value.Date : m_FromDateCtrl.Value.Date),
					FromTime = m_FromTimeCombo.GetTime(),
					ToTime = m_ToTimeCombo.GetTime(),
					DaysOfWeek = (m_DowCheckBox.Checked ? m_DowListBox.GetSelectedDays() : DateUtil.AllDaysOfWeek()),
					SyncToTaskDates = m_SyncToDatesRadioBtn.Checked,
				};
			}
		}

		public void SetDates(Calendar.AppointmentDates dates)
		{
			if (dates.Start.Date == DateTime.MinValue) // invalid
			{
				m_FromDateCtrl.Value = DateTime.Now.Date;

				m_ToDateCtrl.Value = DateTime.Now.Date;
				m_ToDateCtrl.Checked = false;
			}
			else // valid start date
			{
				m_FromDateCtrl.Value = dates.Start.Date;

				if (dates.End.Date == DateTime.MinValue) // invalid
				{
					m_ToDateCtrl.Value = dates.Start.Date;
					m_ToDateCtrl.Checked = false;
				}
				else
				{ 
					m_ToDateCtrl.Value = dates.End.Date;
					m_ToDateCtrl.Checked = (dates.End.Date > dates.Start.Date);
				}
			}
		}

		public TimeBlockSeriesAttributes.EditMask EditMask
		{
			get
			{
				var mask = TimeBlockSeriesAttributes.EditMask.None;

				if (m_EditMode)
				{
					if (m_DateCheckBox.Checked)
						mask |= TimeBlockSeriesAttributes.EditMask.Dates;

					if (m_TimeCheckBox.Checked)
						mask |= TimeBlockSeriesAttributes.EditMask.Times;
				}
				else
				{
					mask = TimeBlockSeriesAttributes.EditMask.Dates |
							TimeBlockSeriesAttributes.EditMask.Times;
				}

				if (m_DowCheckBox.Checked)
					mask |= TimeBlockSeriesAttributes.EditMask.Dow;

				return mask;
			}
		}

		private void OnDowCheckBoxCheckChange(object sender, EventArgs e)
		{
			EnableDisableControls();
		}

		private void OnDateOptionCheckChange(object sender, EventArgs e)
		{
			EnableDisableControls();
		}

		private void OnDateCheckBoxCheckChange(object sender, EventArgs e)
		{
			EnableDisableControls();
		}

		private void OnTimeCheckBoxCheckChange(object sender, EventArgs e)
		{
			EnableDisableControls();
		}
	}
}
