using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

///////////////////////////////////////////////////////////////////////

namespace TodoTxtImpExp
{
	public partial class TodoTxtImporterOptionsForm : Form
	{
		public TodoTxtImporterOptionsForm(TaskList tasks, Translator trans)
		{
			InitializeComponent();

			FormsUtil.SetFont(this, UIExtension.ControlFont());
			trans.Translate(this);

			// combo items are pre-translated
			m_ContextsAttribCombo.Populate(true, tasks, trans);
			m_ProjectsAttribCombo.Populate(true, tasks, trans);
		}

		public DialogResult ShowDialog(Preferences prefs, string prefKey)
		{
			// Restore previous state
// 			m_ContextAsTagBtn.Checked		= prefs.GetProfileBool(prefKey, "ImportContextAsTag", false);
// 			m_ContextAsCustomBtn.Checked	= prefs.GetProfileBool(prefKey,	"ImportContextAsCustom", true);
// 			m_ContextAsCategoryBtn.Checked	= prefs.GetProfileBool(prefKey, "ImportContextAsCategory", false);
// 
// 			m_ProjectAsTagBtn.Checked		= prefs.GetProfileBool(prefKey, "ImportProjectAsTag", false);
// 			m_ProjectAsCustomBtn.Checked	= prefs.GetProfileBool(prefKey, "ImportProjectAsCustom", true);
// 			m_ProjectAsCategoryBtn.Checked	= prefs.GetProfileBool(prefKey, "ImportProjectAsCategory", false);

			var res = base.ShowDialog();

			if (res == DialogResult.OK)
			{
				// Save state
// 				prefs.WriteProfileBool(prefKey, "ImportContextAsTag", m_ContextAsTagBtn.Checked);
// 				prefs.WriteProfileBool(prefKey, "ImportContextAsCustom", m_ContextAsCustomBtn.Checked);
// 				prefs.WriteProfileBool(prefKey, "ImportContextAsCategory", m_ContextAsCategoryBtn.Checked);
// 				prefs.WriteProfileBool(prefKey, "ImportProjectAsTag", m_ProjectAsTagBtn.Checked);
// 				prefs.WriteProfileBool(prefKey, "ImportProjectAsCustom", m_ProjectAsCustomBtn.Checked);
// 				prefs.WriteProfileBool(prefKey, "ImportProjectAsCategory", m_ProjectAsCategoryBtn.Checked);
			}

			return res;
		}

		public bool ImportContextsAsTags		{ get { return m_ContextsAttribCombo.SelectedAttributeIsTags; } }
		public bool ImportContextsAsCategory	{ get { return m_ContextsAttribCombo.SelectedAttributeIsCategory; } }
		public bool ImportContextsAsCustom		{ get { return m_ContextsAttribCombo.SelectedAttributeIsCustom; } }

		public string ImportContextsAsCustomId	{ get { return m_ContextsAttribCombo.SelectedAttributeCustomId; } }

		public bool ImportProjectsAsTags		{ get { return m_ProjectsAttribCombo.SelectedAttributeIsTags; } }
		public bool ImportProjectsAsCategory	{ get { return m_ProjectsAttribCombo.SelectedAttributeIsCategory; } }
		public bool ImportProjectsAsCustom		{ get { return m_ProjectsAttribCombo.SelectedAttributeIsCustom; } }

		public string ImportProjectsAsCustomId	{ get { return m_ProjectsAttribCombo.SelectedAttributeCustomId; } }
	}
}
