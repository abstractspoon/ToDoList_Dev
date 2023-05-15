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

		public void Initialise(WorkingWeek workWeek, TimeBlockSeriesAttributes attribs, bool editMode)
		{
			TimeComboBox.SetWorkingWeek(workWeek);

			m_DowListBox.SetSelectedDays(workWeek.WeekDays()); // default
			m_EditMode = editMode;

			if (attribs != null)
			{
				m_FromDateCtrl.Value = attribs.FromDate.Date;
				m_ToDateCtrl.Value = attribs.ToDate.Date;
				m_ToDateCtrl.Checked = (attribs.ToDate.Date > attribs.FromDate.Date);

				m_FromTimeCombo.SetTime(DateUtil.TimeOnly(attribs.FromDate));
				m_ToTimeCombo.SetTime(DateUtil.TimeOnly(attribs.ToDate));

				m_DowListBox.SetSelectedDays(attribs.DaysOfWeek);
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
					DaysOfWeek = (m_DowCheckBox.Checked ? m_DowListBox.GetSelectedDays() : DateUtil.DaysOfWeek()),
					SyncToTaskDates = m_SyncToDatesRadioBtn.Checked,
				};
			}
		}

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
