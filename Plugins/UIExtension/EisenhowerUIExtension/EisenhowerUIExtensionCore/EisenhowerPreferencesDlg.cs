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

namespace EisenhowerUIExtension
{
	public partial class EisenhowerPreferencesDlg : Form
	{
		private Translator m_Trans;

		// ---------------------------------------------

		public EisenhowerVariables Variables { get; private set; }
		public EisenhowerFilters Filters { get; private set; }

		// ---------------------------------------------

		public EisenhowerPreferencesDlg(Translator trans,
										EisenhowerVariables variables,
										EisenhowerFilters filters)
		{
			m_Trans = trans;

			Variables = variables;
			Filters = filters;

			InitializeComponent();

			m_SetupListCtrl.ChangeEvent += (s, e) => OnFilterSetupChange(e);
			m_Error.ForeColor = DrawingColor.GetErrorLabelTextColor(BackColor);
		}

		public new DialogResult ShowDialog()
		{
			// Note: just because we prevent invalid filters
			// within this dialog, there's no guarantee that
			// they're valid on receipt
			bool valid = (Filters.Find(f => f.HasNullVar) == null);

			m_OK.Enabled = valid;
			m_Error.Visible = !valid;

			if (base.ShowDialog() == DialogResult.OK)
			{
				// Only return OK if there have been modifications
				var filters = m_SetupListCtrl.Filters;

				if (!filters.Equals(Filters))
				{
					Filters = filters;
					return DialogResult.OK;
				}
			}

			// all else
			return DialogResult.Cancel;
		}

		// ------------------------------------------------------

		protected void OnFilterSetupChange(EventArgs e)
		{
			// Only allow closing if all filters are valid
			bool valid = (m_SetupListCtrl.Filters.Find(f => f.HasNullVar) == null);

			m_OK.Enabled = valid;
			m_Error.Visible = !valid;
		}

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			m_SetupListCtrl.Initialise(m_Trans, Variables, Filters);
		}
	}
}


