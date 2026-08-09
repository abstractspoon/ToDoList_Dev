using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace TaskDatesUIExtension
{
	public partial class TaskDatesPreferencesDlg : Form
	{
		private Translator m_Trans;

		// ---------------------------------------------

		public TaskDatesPreferencesDlg(Translator trans)
		{
			m_Trans = trans;

			InitializeComponent();

			m_Error.ForeColor = DrawingColor.GetErrorLabelTextColor(BackColor);
		}

		// ------------------------------------------------------

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			// TODO
		}
	}
}


