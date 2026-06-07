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
		private EisenhowerVariables m_Variables;
		private EisenhowerFilters m_Filters;

		// ---------------------------------------------


		public EisenhowerFilters Filters
		{
			get
			{
				return (IsHandleCreated ? m_SetupListCtrl.Filters : m_Filters);
			}
		}

		// ---------------------------------------------

		public EisenhowerPreferencesDlg(Translator trans, 
										EisenhowerVariables variables,
										EisenhowerFilters filters)
		{
			m_Trans = trans;
			m_Variables = variables;
			m_Filters = filters;

			InitializeComponent();
		}

		// ------------------------------------------------------

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			m_SetupListCtrl.Initialise(m_Trans, m_Variables, m_Filters);
		}
	}
}


