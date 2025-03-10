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
											bool isoDateTimes, 
											TimeBlockSeriesAttributes attrbs,
											TimeBlockSeriesAttributes.EditMask mask)
			:
			this()
		{
			m_TaskTitle.Text = taskTitle;
			m_Attributes.Initialise(workWeek, isoDateTimes, attrbs, true, mask);
		}
		
		public TimeBlockSeriesAttributes Attributes
		{
			get { return m_Attributes.Attributes; }
		}

		public TimeBlockSeriesAttributes.EditMask EditMask
		{
			get { return m_Attributes.EditMask; }
		}
	}


}
