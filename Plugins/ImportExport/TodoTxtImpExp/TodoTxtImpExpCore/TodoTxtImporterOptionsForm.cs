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
	public partial class TodoTxtImporterOptionsForm : Form
	{
		public TodoTxtImporterOptionsForm(Translator trans)
		{
			InitializeComponent();

			FormsUtil.SetFont(this, UIExtension.ControlFont());
			trans.Translate(this);
		}

		public DialogResult ShowDialog(Preferences prefs, string prefKey)
		{
			// Restore previous state
			m_ContextAsTagBtn.Checked		= prefs.GetProfileBool(prefKey, "ImportContextAsTag", false);
			m_ContextAsCustomBtn.Checked	= prefs.GetProfileBool(prefKey,	"ImportContextAsCustom", true);
			m_ContextAsCategoryBtn.Checked	= prefs.GetProfileBool(prefKey, "ImportContextAsCategory", false);

			m_ProjectAsTagBtn.Checked		= prefs.GetProfileBool(prefKey, "ImportProjectAsTag", false);
			m_ProjectAsCustomBtn.Checked	= prefs.GetProfileBool(prefKey, "ImportProjectAsCustom", true);
			m_ProjectAsCategoryBtn.Checked	= prefs.GetProfileBool(prefKey, "ImportProjectAsCategory", false);

			var res = base.ShowDialog();

			if (res == DialogResult.OK)
			{
				// Save state
				prefs.WriteProfileBool(prefKey, "ImportContextAsTag", m_ContextAsTagBtn.Checked);
				prefs.WriteProfileBool(prefKey, "ImportContextAsCustom", m_ContextAsCustomBtn.Checked);
				prefs.WriteProfileBool(prefKey, "ImportContextAsCategory", m_ContextAsCategoryBtn.Checked);
				prefs.WriteProfileBool(prefKey, "ImportProjectAsTag", m_ProjectAsTagBtn.Checked);
				prefs.WriteProfileBool(prefKey, "ImportProjectAsCustom", m_ProjectAsCustomBtn.Checked);
				prefs.WriteProfileBool(prefKey, "ImportProjectAsCategory", m_ProjectAsCategoryBtn.Checked);
			}

			return res;
		}

		public bool ImportContextAsTag			{ get { return m_ContextAsTagBtn.Checked; } }
		public bool ImportContextAsCustom		{ get { return m_ContextAsCustomBtn.Checked; } }
		public bool ImportContextAsCategory		{ get { return m_ContextAsCategoryBtn.Checked; } }

		public bool ImportProjectAsCategory		{ get { return m_ProjectAsCategoryBtn.Checked; } }
		public bool ImportProjectAsCustom		{ get { return m_ProjectAsCustomBtn.Checked; } }
		public bool ImportProjectAsTag			{ get { return m_ProjectAsTagBtn.Checked; } }
	}
}
