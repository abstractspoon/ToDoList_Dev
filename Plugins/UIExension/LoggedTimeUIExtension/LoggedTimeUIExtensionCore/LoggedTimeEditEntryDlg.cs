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
	public partial class LoggedTimeEditEntryDlg : Form
	{
		LogEntry m_Entry;

		// ---------------------------------------

		public LoggedTimeEditEntryDlg()
		{
			InitializeComponent();
		}

		public LoggedTimeEditEntryDlg(LogEntry entry, WorkingWeek workWeek, bool readonlyTask)
			:
			this()
		{
			m_Entry = entry;

			m_TaskTitleLabel.Text = entry.Title;
			m_TaskIdLabel.Text = entry.TaskId.ToString();

			m_Attributes.Initialise(entry, workWeek, readonlyTask);
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
			get { return Color.Empty;/*m_Attributes.FillColor*/; }
		}

		public bool WantAddToTimeSpent
		{
			get { return m_Attributes.WantAddToTimeSpent; }
		}


	}


}
