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
	public partial class TodoTxtExporterOptionsForm : PreferencesFormBase
	{
		const Task.Attribute DefaultProjectsAttribId = Task.Attribute.Tags;
		const Task.Attribute DefaultContextsAttribId = Task.Attribute.Category;

		// ------------------------------------------------------

		public TodoTxtExporterOptionsForm(TaskList tasks, Translator trans)
		{
			InitializeComponent();

			FormsUtil.SetFont(this, UIExtension.ControlFont());
			trans.Translate(this);

			var availAttribs = tasks.GetAvailableAttributes(trans);
			var customAttribs = tasks.GetCustomAttributes(CustomAttributeDefinition.Attribute.String);

			m_ContextsAttribCombo.Populate(availAttribs, customAttribs, trans);
			m_ProjectsAttribCombo.Populate(availAttribs, customAttribs, trans);
		}

		public DialogResult ShowDialog(Preferences prefs, string prefKey)
		{
			// Restore previous state
			var contextsAttribId = prefs.GetProfileEnum(prefKey, "ExportContextsFromAttrib", DefaultContextsAttribId);
			var contextsCustAttribId = prefs.GetProfileString(prefKey, "ExportContextsFromCustomId", string.Empty);

			if (!m_ContextsAttribCombo.SelectAttribute(contextsAttribId, contextsCustAttribId))
				m_ContextsAttribCombo.SelectAttribute(DefaultContextsAttribId);

			var projectsAttribId = prefs.GetProfileEnum(prefKey, "ExportProjectsFromAttrib", DefaultProjectsAttribId);
			var projectsCustAttribId = prefs.GetProfileString(prefKey, "ExportProjectsFromCustomId", string.Empty);

			if (!m_ProjectsAttribCombo.SelectAttribute(projectsAttribId, projectsCustAttribId))
				m_ProjectsAttribCombo.SelectAttribute(DefaultProjectsAttribId);

			var res = base.ShowDialog();

			if (res == DialogResult.OK)
			{
				// Save state
				prefs.WriteProfileEnum(prefKey, "ExportContextsFromAttrib", m_ContextsAttribCombo.SelectedAttributeId);
				prefs.WriteProfileEnum(prefKey, "ExportProjectsFromAttrib", m_ProjectsAttribCombo.SelectedAttributeId);
				prefs.WriteProfileString(prefKey, "ExportContextsFromCustomId", ExportContextsFromCustomId);
				prefs.WriteProfileString(prefKey, "ExportProjectsFromCustomId", ExportProjectsFromCustomId);
			}

			return res;
		}

		public bool ExportContextsFromTags			{ get { return (m_ContextsAttribCombo.SelectedAttributeId == Task.Attribute.Tags); } }
		public bool ExportContextsFromCategory		{ get { return (m_ContextsAttribCombo.SelectedAttributeId == Task.Attribute.Category); } }
		public bool ExportContextsFromCustom		{ get { return !string.IsNullOrEmpty(ExportContextsFromCustomId); } }

		public string ExportContextsFromCustomId	{ get { return m_ContextsAttribCombo.SelectedAttributeCustomId; } }

		public bool ExportProjectsFromTags			{ get { return (m_ProjectsAttribCombo.SelectedAttributeId == Task.Attribute.Tags); } }
		public bool ExportProjectsFromCategory		{ get { return (m_ProjectsAttribCombo.SelectedAttributeId == Task.Attribute.Category); } }
		public bool ExportProjectsFromCustom		{ get { return !string.IsNullOrEmpty(ExportProjectsFromCustomId); } }

		public string ExportProjectsFromCustomId	{ get { return m_ProjectsAttribCombo.SelectedAttributeCustomId; } }
	}
}
