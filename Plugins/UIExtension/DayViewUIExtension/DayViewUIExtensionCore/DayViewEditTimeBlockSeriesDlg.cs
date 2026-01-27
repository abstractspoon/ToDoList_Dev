using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Diagnostics;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

// ---------------------------------------------------------------

namespace DayViewUIExtension
{
	public partial class DayViewEditTimeBlockSeriesDlg : Form
	{
		Translator m_Trans;

		// -----------------------------------

		public DayViewEditTimeBlockSeriesDlg() // for designer
		{
			InitializeComponent();
		}

		public DayViewEditTimeBlockSeriesDlg(string taskTitle,
											uint taskId,											
											WorkingWeek workWeek,
											bool isoDateTimes, 
											TimeBlockSeriesAttributes attribs,
											TimeBlockSeriesAttributes.EditMask mask,
											Translator trans)
			:
			this()
		{
			m_TaskTitle.Text = taskTitle;
			m_TaskId.Text = taskId.ToString();

			m_Attributes.Initialise(workWeek, isoDateTimes, attribs, true, mask);
			m_Attributes.ChangeEvent += (s, e) => ValidateInputs();

			var color = BackColor;
			m_Error.ForeColor = DrawingColor.GetErrorLabelTextColor(BackColor);

			m_Trans = trans;
			m_Trans.Translate(this);

			ValidateInputs();
		}

		public TimeBlockSeriesAttributes Attributes
		{
			get { return m_Attributes.Attributes; }
		}

		public TimeBlockSeriesAttributes.EditMask EditMask
		{
			get { return m_Attributes.EditMask; }
		}

		protected void ValidateInputs()
		{
			// Error text
			var attrib = Attributes;
			var mask = EditMask;

			if (mask == 0)
			{
				OK.Enabled = false;
				m_Error.Text = m_Trans.Translate("No options selected", Translator.Type.Text);
			}
			else if (mask.HasFlag(TimeBlockSeriesAttributes.EditMask.Dates) &&
					!attrib.IsValid(TimeBlockSeriesAttributes.EditMask.Dates))
			{
				OK.Enabled = false;
				m_Error.Text = m_Trans.Translate("Invalid 'Date range'", Translator.Type.Text);
			}
			else if (mask.HasFlag(TimeBlockSeriesAttributes.EditMask.Times) &&
					!attrib.IsValid(TimeBlockSeriesAttributes.EditMask.Times))
			{
				OK.Enabled = false;
				m_Error.Text = m_Trans.Translate("Invalid 'Time of day'", Translator.Type.Text);
			}
			else if (mask.HasFlag(TimeBlockSeriesAttributes.EditMask.Dow) &&
					!attrib.IsValid(TimeBlockSeriesAttributes.EditMask.Dow))
			{
				OK.Enabled = false;
				m_Error.Text = m_Trans.Translate("Invalid 'Specific days'", Translator.Type.Text);
			}
			else
			{
				OK.Enabled = true;
				m_Error.Text = string.Empty;
			}
		}
	}


}
