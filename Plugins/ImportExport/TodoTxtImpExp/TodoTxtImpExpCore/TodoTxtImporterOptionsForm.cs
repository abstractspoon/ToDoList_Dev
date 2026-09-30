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
		const Task.Attribute DefaultProjectsAttribId = Task.Attribute.Tags;
		const Task.Attribute DefaultContextsAttribId = Task.Attribute.Category;

		// ------------------------------------------------------

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
			var contextsAttribId = prefs.GetProfileEnum(prefKey, "ImportContextAsAttrib", DefaultContextsAttribId);
			var contextsCustAttribId = prefs.GetProfileString(prefKey, "ImportContextAsCustomId", string.Empty);

			if (!m_ContextsAttribCombo.SelectAttribute(contextsAttribId, contextsCustAttribId))
				m_ContextsAttribCombo.SelectAttribute(DefaultContextsAttribId);

			var projectsAttribId = prefs.GetProfileEnum(prefKey, "ImportProjectAsAttrib", DefaultProjectsAttribId);
			var projectsCustAttribId = prefs.GetProfileString(prefKey, "ImportProjectAsCustomId", string.Empty);

			if (!m_ProjectsAttribCombo.SelectAttribute(projectsAttribId, projectsCustAttribId))
				m_ProjectsAttribCombo.SelectAttribute(DefaultProjectsAttribId);

			var res = base.ShowDialog();

			if (res == DialogResult.OK)
			{
				// Save state
				prefs.WriteProfileEnum(prefKey, "ImportContextAsAttrib", m_ContextsAttribCombo.SelectedAttributeId);
				prefs.WriteProfileEnum(prefKey, "ImportProjectAsAttrib", m_ProjectsAttribCombo.SelectedAttributeId);
				prefs.WriteProfileString(prefKey, "ImportContextAsCustomId", ImportContextsAsCustomId);
				prefs.WriteProfileString(prefKey, "ImportProjectAsCustomId", ImportProjectsAsCustomId);
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
