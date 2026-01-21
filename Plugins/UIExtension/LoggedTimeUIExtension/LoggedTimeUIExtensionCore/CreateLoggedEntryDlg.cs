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
			m_TaskCombo.Initialise(taskItems, taskIcons, attrib.TaskId, TaskItem.None(trans.Translate("<none>", Translator.Type.ComboBox)));

			m_TaskCombo.SearchUpdated += (s, e) => UpdateSelectedTask();
			m_TaskCombo.SelectedIndexChanged += (s, e) => UpdateSelectedTask();
			m_TaskCombo.SelectedIndex = 0;

			trans.Translate(this);
		}

		protected void UpdateSelectedTask()
		{
			if (m_TaskCombo.SelectedIndex == -1)
			{
				OK.Enabled = false;

				m_TaskId.Text = String.Empty;
				m_Attributes.ReadOnlyTask = true;
			}
			else
			{
				OK.Enabled = true;

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
		}

		protected void OnEditTextChanged(object sender, EventArgs e)
		{
			OK.Enabled = (m_TaskCombo.SelectedIndex != -1);
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
