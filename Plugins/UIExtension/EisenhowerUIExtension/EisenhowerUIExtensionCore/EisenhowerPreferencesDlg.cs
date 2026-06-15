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
		public EisenhowerMatrices Matrices { get; private set; }

		// ---------------------------------------------

		public EisenhowerPreferencesDlg(Translator trans,
										EisenhowerVariables variables,
										EisenhowerMatrices matrices)
		{
			m_Trans = trans;

			Variables = variables;
			Matrices = matrices;

			InitializeComponent();

			m_SetupListCtrl.ChangeEvent += (s, e) => OnMatrixSetupChange(e);
			m_Error.ForeColor = DrawingColor.GetErrorLabelTextColor(BackColor);
		}

		public new DialogResult ShowDialog()
		{
			// Note: just because we prevent invalid matrices
			// within this dialog, there's no guarantee that
			// they're valid on receipt
			bool valid = (Matrices.Find(f => f.HasNullVar) == null);

			m_OK.Enabled = valid;
			m_Error.Visible = !valid;

			if (base.ShowDialog() == DialogResult.OK)
			{
				// Only return OK if there have been modifications
				var matrices = m_SetupListCtrl.Matrices;

				if (!matrices.Equals(Matrices))
				{
					Matrices = matrices;
					return DialogResult.OK;
				}
			}

			// all else
			return DialogResult.Cancel;
		}

		// ------------------------------------------------------

		protected void OnMatrixSetupChange(EventArgs e)
		{
			// Only allow closing if all matrices are valid
			bool valid = (m_SetupListCtrl.Matrices.Find(m => m.HasNullVar) == null);

			m_OK.Enabled = valid;
			m_Error.Visible = !valid;
		}

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			m_SetupListCtrl.Initialise(m_Trans, Variables, Matrices);
		}
	}
}


