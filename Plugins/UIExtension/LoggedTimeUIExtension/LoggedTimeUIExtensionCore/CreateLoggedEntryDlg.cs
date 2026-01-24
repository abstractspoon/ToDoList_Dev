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
	public partial class CreateLoggedEntryDlg : Form
	{
		IEnumerable<TaskItem> m_TaskItems;
		Translator m_Trans;

		// -----------------------------------

		public CreateLoggedEntryDlg()
		{
			InitializeComponent();
		}

		public CreateLoggedEntryDlg(IEnumerable<TaskItem> taskItems, 
									UIExtension.TaskIcon taskIcons, 
									WorkingWeek workWeek,
									bool isoDateTimes,
									LogEntry attrib,
									Translator trans)
			:
			this()
		{
			m_TaskItems = taskItems;
			m_Trans = trans;

			m_Attributes.Initialise(attrib, workWeek, isoDateTimes, false, false, trans);
			m_Attributes.ChangeEvent += (s, e) => ValidateInputs();

			m_TaskCombo.Initialise(taskItems, taskIcons, attrib.TaskId, TaskItem.None(trans.Translate("<none>", Translator.Type.ComboBox)));
			m_TaskCombo.SelectedIndex = 0;
			m_TaskCombo.SearchUpdated += (s, e) => ValidateInputs();
			m_TaskCombo.SelectedIndexChanged += (s, e) => ValidateInputs();

			trans.Translate(this);

			ValidateInputs();
		}

		protected void ValidateInputs()
		{
			bool validInputs = ((m_TaskCombo.SelectedIndex != -1) && m_Attributes.Dates.IsValid &&
								((m_Attributes.TimeSpent > 0.0) || !String.IsNullOrWhiteSpace(m_Attributes.Comment)));

			if (validInputs)
			{
				OK.Enabled = true;
				m_Error.Text = string.Empty;

				uint taskId = m_TaskCombo.SelectedTaskId;

				if (taskId == 0)
				{
					m_TaskId.Text = m_Trans.Translate("<none>", Translator.Type.Text);
					m_Attributes.ReadOnlyTask = true;
				}
				else
				{
					m_TaskId.Text = taskId.ToString();

					var taskItem = m_TaskItems.Where(x => (x.Id == taskId)).FirstOrDefault();
					m_Attributes.ReadOnlyTask = ((taskItem == null) || taskItem.Locked);
				}
			}
			else
			{
				OK.Enabled = false;

				m_TaskId.Text = String.Empty;
				m_Attributes.ReadOnlyTask = true;

				if (m_TaskCombo.SelectedIndex == -1)
				{
					m_Error.Text = m_Trans.Translate("Invalid task", Translator.Type.Text);
				}
				else if (!m_Attributes.Dates.IsValid)
				{
					m_Error.Text = m_Trans.Translate("Invalid 'End' time", Translator.Type.Text);
				}
				else
				{
					m_Error.Text = m_Trans.Translate("'Time Spent' or 'Comment' required", Translator.Type.Text);
				}
			}
		}

		public uint TaskId
		{
			get { return m_TaskCombo.SelectedTaskId; }
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
