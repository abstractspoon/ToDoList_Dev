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
		LogEntry m_DefaultAttrib;

		// ---------------------------------------

		public CreateLoggedEntryDlg()
		{
			InitializeComponent();
		}

		public CreateLoggedEntryDlg(IEnumerable<TaskItem> taskItems, 
									UIExtension.TaskIcon taskIcons, 
									WorkingWeek workWeek,
									LogEntry attrib)
			:
			this()
		{
			m_DefaultAttrib = attrib;

			if (attrib.TaskId != 0)
				m_TaskIdLabel.Text = attrib.TaskId.ToString();

			m_TaskCombo.Initialise(taskItems.OrderBy(x => x.Position), taskIcons, attrib.TaskId);
			m_Attributes.Initialise(attrib, workWeek, false);
		}

		public uint TaskId
		{
			get { return m_TaskCombo.SelectedTaskId; }
		}

		public DateTime From
		{
			get { return m_Attributes.From; }
		}

		public DateTime To
		{
			get { return m_Attributes.To; }
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

		public bool WantAddToTimeSpent
		{
			get { return m_Attributes.WantAddToTimeSpent; }
		}
	}
}
