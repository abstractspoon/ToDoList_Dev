using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

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
		}

		public new DialogResult ShowDialog()
		{
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

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			m_SetupListCtrl.Initialise(m_Trans, Variables, Filters);
		}
	}
}


