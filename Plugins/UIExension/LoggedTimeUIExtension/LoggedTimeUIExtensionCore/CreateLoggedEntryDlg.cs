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

			m_Attributes.Initialise(attrib, workWeek, isoDateTimes, false, false, trans);
			m_TaskCombo.Initialise(taskItems, taskIcons, attrib.TaskId, TaskItem.None);

			m_TaskCombo.SelectedIndexChanged += OnSelectedTaskChange;
			m_TaskCombo.SelectedIndex = 0;
		}

		protected void OnSelectedTaskChange(object sender, EventArgs e)
		{
			uint taskId = m_TaskCombo.SelectedTaskId;

			if (taskId == 0)
			{
				m_TaskId.Text = "<none>";
				m_Attributes.ReadOnlyTask = true;
			}
			else
			{
				m_TaskId.Text = taskId.ToString();

				var taskItem = m_TaskItems.Where(x => (x.Id == taskId)).FirstOrDefault();
				m_Attributes.ReadOnlyTask = ((taskItem == null) || taskItem.Locked);
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
