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
	public delegate void AttributeChangeEventHandler(object sender, EventArgs args);

	public partial class LoggedEntryAttributesPage : UserControl
	{
		private double m_OrgTimeSpent = 0.0;
		private bool m_EditMode = false;
		private bool m_ReadOnlyTask = false;
		private Translator m_Trans = null;

		// --------------------------------------------------------

		public event AttributeChangeEventHandler ChangeEvent;

		// --------------------------------------------------------

		public LoggedEntryAttributesPage()
		{
			InitializeComponent();
		}

		public void Initialise(LogEntry entry, 
								WorkingWeek workWeek, 
								bool isoDateTimes, 
								bool readonlyTask, 
								bool editMode, 
								Translator trans)
		{
			m_Trans = trans;

			TimeComboBox.SetWorkingWeek(workWeek);
			SetDates(workWeek, entry);

			m_FromDateCtrl.ValueChanged += (s, e) => OnAttributeChanged();

			m_FromTimeCombo.SetISOFormat(isoDateTimes);
			m_FromTimeCombo.ChangeEvent += (s, e) => OnAttributeChanged();

			m_ToTimeCombo.SetISOFormat(isoDateTimes);
			m_ToTimeCombo.ChangeEvent += (s, e) => OnAttributeChanged();

			DateUtil.SetShortDateFormat(m_FromDateCtrl, isoDateTimes);
			m_EditMode = editMode;
			m_OrgTimeSpent = entry.TimeSpentInHrs;

			m_TimeSpentEdit.Text = entry.TimeSpentInHrs.ToString("N3");
			m_TimeSpentEdit.DecimalMode = true;
			m_CommentEdit.Text = entry.Comment;

			m_FillColorButton.Color = entry.TrueFillColor;
			m_FillColorCheckBox.Checked = (entry.TrueFillColor != Color.Empty);

			m_FillColorCheckBox.CheckedChanged += OnFillColorCheckChange;
			OnFillColorCheckChange(this, null);

			m_TimeSpentEdit.TextChanged += OnTimeSpentChanged; 
			OnTimeSpentChanged(this, null);

			ReadOnlyTask = readonlyTask;
		}

		public bool ReadOnlyTask
		{
			get { return m_ReadOnlyTask; }
			set
			{
				m_ReadOnlyTask = value;
				m_AddToTimeSpentCheckBox.Enabled = !value;
			}
		}

		private void OnAttributeChanged()
		{
			ChangeEvent?.Invoke(this, new EventArgs());
		}
		
		private void OnFillColorCheckChange(object sender, EventArgs e)
		{
			m_FillColorButton.Enabled = m_FillColorCheckBox.Checked;

			OnAttributeChanged();
		}

		private void OnTimeSpentChanged(object sender, EventArgs e)
		{
			if (m_EditMode && m_AddToTimeSpentCheckBox.Enabled)
			{
				string format = m_Trans.Translate("Also modify task 'Time Spent' by {0:0.###} hours", Translator.Type.CheckBox);
				m_AddToTimeSpentCheckBox.Text = string.Format(format, (TimeSpent - m_OrgTimeSpent));
			}

			OnAttributeChanged();
		}

		public Calendar.AppointmentDates Dates
		{
			get
			{
				return new Calendar.AppointmentDates(m_FromDateCtrl.Value.Date + m_FromTimeCombo.GetTime(),
													(m_FromDateCtrl.Value.Date + m_ToTimeCombo.GetTime()));
			}
		}

		public string Comment
		{
			get	{ return m_CommentEdit.Text; }
		}

		public Color FillColor
		{
			get
			{
				if (m_FillColorCheckBox.Checked)
					return m_FillColorButton.Color;

				// else
				return Color.Empty;
			}
		}

		public double TimeSpent
		{
			get { return m_TimeSpentEdit.GetAmount(m_OrgTimeSpent); }
		}

		bool WantAddToTimeSpent
		{
			get { return (m_AddToTimeSpentCheckBox.Enabled && m_AddToTimeSpentCheckBox.Checked); }
		}

		public double HoursToAddToTimeSpent
		{
			get
			{
				if (WantAddToTimeSpent)
					return (m_EditMode ? (TimeSpent - m_OrgTimeSpent) : TimeSpent);

				// else
				return 0.0;
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
