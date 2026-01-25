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
		Translator m_Trans;
	
		// -----------------------------------

		public DayViewCreateTimeBlockDlg()
		{
			InitializeComponent();
		}

		public DayViewCreateTimeBlockDlg(IEnumerable<TaskItem> taskItems, 
										 UIExtension.TaskIcon taskIcons, 
										 WorkingWeek workWeek,
										 bool isoDateTimes,
										 uint taskId,
										 TimeBlockSeriesAttributes attribs,
										 Translator trans)
			:
			this()
		{
			m_TaskCombo.Initialise(taskItems, taskIcons, taskId);
			m_TaskCombo.SearchUpdated += (s, e) => ValidateInputs();
			m_TaskCombo.SelectedIndexChanged += (s, e) => ValidateInputs();

			m_Attributes.Initialise(workWeek, isoDateTimes, attribs, false);
			m_Attributes.ChangeEvent += (s, e) => ValidateInputs();

			m_Trans = trans;
			m_Trans.Translate(this);

			ValidateInputs();
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

		protected void ValidateInputs()
		{
			bool validTask = (SelectedTaskId > 0);

			if (validTask)
				m_TaskId.Text = SelectedTaskId.ToString();
			else
				m_TaskId.Text = string.Empty;

			// Error text
			var attrib = Attributes;
			bool validInputs = (validTask && attrib.IsValid);

			if (validInputs)
			{
				OK.Enabled = true;
				m_Error.Text = string.Empty;
			}
			else
			{
				OK.Enabled = false;

				if (!validTask)
				{
					m_Error.Text = m_Trans.Translate("Invalid task", Translator.Type.Text);
				}
				else if ((attrib.FromDate.Date == DateTime.MinValue) ||
						 (attrib.ToDate.Date == DateTime.MaxValue) ||
						 (attrib.FromDate.Date > attrib.ToDate.Date))
				{
					m_Error.Text = m_Trans.Translate("Invalid 'Date range'", Translator.Type.Text);
				}
				else if (attrib.FromTime >= attrib.ToTime)
				{
					m_Error.Text = m_Trans.Translate("Invalid 'Time of day'", Translator.Type.Text);
				}
				else
				{
					m_Error.Text = m_Trans.Translate("Invalid 'Specific days'", Translator.Type.Text);
				}
			}
		}

	}
}
