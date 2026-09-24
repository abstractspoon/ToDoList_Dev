using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace TodoTxtImpExp
{
	public partial class TodoTxtExporterOptionsForm : Form
	{
		public TodoTxtExporterOptionsForm(Translator trans)
		{
			InitializeComponent();

			FormsUtil.SetFont(this, UIExtension.ControlFont());
			trans.Translate(this);
		}

		public DialogResult ShowDialog(Preferences prefs, string prefKey)
		{
			// Restore previous state
			m_ContextAsCategoryBtn.Checked		= prefs.GetProfileBool(prefKey, "ImportContextAsCategory", true);
			m_ContextAsTagBtn.Checked			= prefs.GetProfileBool(prefKey, "ImportContextAsTag", false);
			m_ProjectAsCategoryBtn.Checked		= prefs.GetProfileBool(prefKey, "ImporProjectAsCategoryt", false);
			m_ProjectAsTagBtn.Checked			= prefs.GetProfileBool(prefKey, "ImportProjectAsTag", false);
			m_ProjectAsParentTaskBtn.Checked	= prefs.GetProfileBool(prefKey, "ImportProjectAsParentTask", true);

			var res = base.ShowDialog();

			if (res == DialogResult.OK)
			{
				// Save state
				prefs.WriteProfileBool(prefKey, "ImportContextAsCategory", m_ContextAsCategoryBtn.Checked);
				prefs.WriteProfileBool(prefKey, "ImportContextAsTag", m_ContextAsTagBtn.Checked);
				prefs.WriteProfileBool(prefKey, "ImporProjectAsCategoryt", m_ProjectAsCategoryBtn.Checked);
				prefs.WriteProfileBool(prefKey, "ImportProjectAsTag", m_ProjectAsTagBtn.Checked);
				prefs.WriteProfileBool(prefKey, "ImportProjectAsParentTask", m_ProjectAsParentTaskBtn.Checked);
			}

			return res;
		}

		public bool ImportContextAsCategory		{ get { return m_ContextAsCategoryBtn.Checked; } }
		public bool ImportContextAsTag			{ get { return m_ContextAsTagBtn.Checked; } }

		public bool ImportProjectAsCategory		{ get { return m_ProjectAsCategoryBtn.Checked; } }
		public bool ImportProjectAsTag			{ get { return m_ProjectAsTagBtn.Checked; } }
		public bool ImportProjectAsParentTask	{ get { return m_ProjectAsParentTaskBtn.Checked; } }
	}
}
