using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	public partial class DayViewCreateTimeBlockDlg : Form
	{
		public DayViewCreateTimeBlockDlg()
		{
			InitializeComponent();
		}

		public DayViewCreateTimeBlockDlg(IEnumerable<TaskItem> taskItems, 
										 UIExtension.TaskIcon taskIcons, 
										 WorkingWeek workWeek,
										 uint taskId)
			:
			this()
		{
			m_TaskCombo.Initialise(taskItems, taskIcons, taskId);
			TimeComboBox.SetWorkingWeek(workWeek);
		}

		public TimeSpan FromTime
		{
			get { return m_FromTimeCombo.GetTime();	}
			set { m_FromTimeCombo.SetTime(value); }
		}

		public TimeSpan ToTime
		{
			get { return m_ToTimeCombo.GetTime(); }
			set { m_ToTimeCombo.SetTime(value); }
		}

		public uint SelectedTaskId
		{
			get { return m_TaskCombo.SelectedTaskId; }
		}
	}
}
