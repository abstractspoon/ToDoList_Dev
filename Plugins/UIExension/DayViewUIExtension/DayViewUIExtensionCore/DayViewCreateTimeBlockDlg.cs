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

		public DayViewCreateTimeBlockDlg(IEnumerable<TaskItem> taskItems, UIExtension.TaskIcon taskIcons, 
										uint taskId = 0, float fromHours = -1, float toHours = -1)
			:
			this()
		{
			m_TaskCombo.Initialise(taskItems, taskIcons);
		}
	}
}
