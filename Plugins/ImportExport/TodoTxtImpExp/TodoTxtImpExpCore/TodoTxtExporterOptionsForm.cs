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
			var contextsAttribId = prefs.GetProfileEnum(prefKey, "ExportContextAsAttrib", DefaultContextsAttribId);
			var contextsCustAttribId = prefs.GetProfileString(prefKey, "ExportContextAsCustomId", string.Empty);

			if (!m_ContextsAttribCombo.SelectAttribute(contextsAttribId, contextsCustAttribId))
				m_ContextsAttribCombo.SelectAttribute(DefaultContextsAttribId);

			var projectsAttribId = prefs.GetProfileEnum(prefKey, "ExportProjectAsAttrib", DefaultProjectsAttribId);
			var projectsCustAttribId = prefs.GetProfileString(prefKey, "ExportProjectAsCustomId", string.Empty);

			if (!m_ProjectsAttribCombo.SelectAttribute(projectsAttribId, projectsCustAttribId))
				m_ProjectsAttribCombo.SelectAttribute(DefaultProjectsAttribId);

			var res = base.ShowDialog();

			if (res == DialogResult.OK)
			{
				// Save state
				prefs.WriteProfileEnum(prefKey, "ExportContextAsAttrib", m_ContextsAttribCombo.SelectedAttributeId);
				prefs.WriteProfileEnum(prefKey, "ExportProjectAsAttrib", m_ProjectsAttribCombo.SelectedAttributeId);
				prefs.WriteProfileString(prefKey, "ExportContextAsCustomId", ExportContextsAsCustomId);
				prefs.WriteProfileString(prefKey, "ExportProjectAsCustomId", ExportProjectsAsCustomId);
			}

			return res;
		}

		public bool ExportContextsAsTags		{ get { return (m_ContextsAttribCombo.SelectedAttributeId == Task.Attribute.Tags); } }
		public bool ExportContextsAsCategory	{ get { return (m_ContextsAttribCombo.SelectedAttributeId == Task.Attribute.Category); } }
		public bool ExportContextsAsCustom		{ get { return !string.IsNullOrEmpty(ExportContextsAsCustomId); } }

		public string ExportContextsAsCustomId	{ get { return m_ContextsAttribCombo.SelectedAttributeCustomId; } }

		public bool ExportProjectsAsTags		{ get { return (m_ProjectsAttribCombo.SelectedAttributeId == Task.Attribute.Tags); } }
		public bool ExportProjectsAsCategory	{ get { return (m_ProjectsAttribCombo.SelectedAttributeId == Task.Attribute.Category); } }
		public bool ExportProjectsAsCustom		{ get { return !string.IsNullOrEmpty(ExportProjectsAsCustomId); } }

		public string ExportProjectsAsCustomId	{ get { return m_ProjectsAttribCombo.SelectedAttributeCustomId; } }
	}
}
