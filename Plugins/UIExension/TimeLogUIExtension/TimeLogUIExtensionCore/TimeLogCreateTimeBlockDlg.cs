﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace TimeLogUIExtension
{
	public partial class TimeLogCreateTimeBlockDlg : Form
	{
/*
		public TimeLogCreateTimeBlockDlg()
		{
			InitializeComponent();
		}

		public TimeLogCreateTimeBlockDlg(IEnumerable<TaskItem> taskItems, 
										 UIExtension.TaskIcon taskIcons, 
										 WorkingWeek workWeek,
										 uint taskId,
										 TimeBlockSeriesAttributes attribs)
			:
			this()
		{
			m_TaskCombo.Initialise(taskItems.OrderBy(x => x.Position);, taskIcons, taskId);
			m_Attributes.Initialise(workWeek, attribs, false);
		}

		public uint SelectedTaskId
		{
			get { return m_TaskCombo.SelectedTaskId; }
		}

		public TimeBlockSeriesAttributes Attributes
		{
			get { return m_Attributes.Attributes; }
		}

		private void OnTaskComboSelChange(object sender, EventArgs e)
		{
			OK.Enabled = (SelectedTaskId != 0);
		}
*/
	}
}
