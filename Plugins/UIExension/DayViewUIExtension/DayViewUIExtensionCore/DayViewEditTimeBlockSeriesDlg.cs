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
	public partial class DayViewEditTimeBlockSeriesDlg : Form
	{
		public DayViewEditTimeBlockSeriesDlg()
		{
			InitializeComponent();
		}

		public DayViewEditTimeBlockSeriesDlg(string taskTitle,
											WorkingWeek workWeek,
											TimeBlockSeries series)
			:
			this()
		{
			m_TaskTitle.Text = taskTitle;
			m_Attributes.Initialise(workWeek, series.Dates, true);
		}

		public DateTime FromDate { get { return m_Attributes.FromDate; } }
		public DateTime ToDate { get { return m_Attributes.ToDate; } }

		public TimeSpan FromTime { get { return m_Attributes.FromTime; } }
		public TimeSpan ToTime { get { return m_Attributes.ToTime; } }

		public List<DayOfWeek> DaysOfWeek { get { return m_Attributes.DaysOfWeek; } }
		public bool SyncTimeBlocksToTaskDates { get { return m_Attributes.SyncTimeBlocksToTaskDates; } }
	}


}
