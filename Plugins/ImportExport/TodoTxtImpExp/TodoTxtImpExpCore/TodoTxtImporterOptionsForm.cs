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
	public partial class TodoTxtImporterOptionsForm : PreferencesFormBase
	{
		const Task.Attribute DefaultProjectsAttribId = Task.Attribute.Tags;
		const Task.Attribute DefaultContextsAttribId = Task.Attribute.Category;

		// ------------------------------------------------------

		public TodoTxtImporterOptionsForm(TaskList tasks, Translator trans)
		{
			InitializeComponent();

			FormsUtil.SetFont(this, UIExtension.ControlFont());
			trans.Translate(this);

			var availAttribs = tasks.GetAvailableAttributes(trans);
			var customAttribs = tasks.GetCustomAttributes(CustomAttributeDefinition.Attribute.String);

			m_ContextsAttribCombo.Populate(availAttribs, customAttribs, "Context", "CUST_TDT_CONTEXTS", trans);
			m_ProjectsAttribCombo.Populate(availAttribs, customAttribs, "Project", "CUST_TDT_PROJECTS", trans);
		}

		public DialogResult ShowDialog(Preferences prefs, string prefKey)
		{
			// Restore previous state
			var contextsAttribId = prefs.GetProfileEnum(prefKey, "ImportContextsToAttrib", DefaultContextsAttribId);
			var contextsCustAttribId = prefs.GetProfileString(prefKey, "ImportContextsToCustomId", string.Empty);

			if (!m_ContextsAttribCombo.SelectAttribute(contextsAttribId, contextsCustAttribId))
				m_ContextsAttribCombo.SelectAttribute(DefaultContextsAttribId);

			var projectsAttribId = prefs.GetProfileEnum(prefKey, "ImportProjectsToAttrib", DefaultProjectsAttribId);
			var projectsCustAttribId = prefs.GetProfileString(prefKey, "ImportProjectsToCustomId", string.Empty);

			if (!m_ProjectsAttribCombo.SelectAttribute(projectsAttribId, projectsCustAttribId))
				m_ProjectsAttribCombo.SelectAttribute(DefaultProjectsAttribId);

			var res = base.ShowDialog();

			if (res == DialogResult.OK)
			{
				// Save state
				prefs.WriteProfileEnum(prefKey, "ImportContextsToAttrib", m_ContextsAttribCombo.SelectedAttributeId);
				prefs.WriteProfileEnum(prefKey, "ImportProjectsToAttrib", m_ProjectsAttribCombo.SelectedAttributeId);
				prefs.WriteProfileString(prefKey, "ImportContextsToCustomId", ImportContextsToCustomId);
				prefs.WriteProfileString(prefKey, "ImportProjectsToCustomId", ImportProjectsToCustomId);
			}

			return res;
		}

		public bool ImportContextsToTags		{ get { return (m_ContextsAttribCombo.SelectedAttributeId == Task.Attribute.Tags); } }
		public bool ImportContextsToCategory	{ get { return (m_ContextsAttribCombo.SelectedAttributeId == Task.Attribute.Category); } }
		public bool ImportContextsToCustom		{ get { return !string.IsNullOrEmpty(ImportContextsToCustomId); } }

		public string ImportContextsToCustomId	{ get { return m_ContextsAttribCombo.SelectedAttributeCustomId; } }

		public bool ImportProjectsToTags		{ get { return (m_ProjectsAttribCombo.SelectedAttributeId == Task.Attribute.Tags); } }
		public bool ImportProjectsToCategory	{ get { return (m_ProjectsAttribCombo.SelectedAttributeId == Task.Attribute.Category); } }
		public bool ImportProjectsToCustom		{ get { return !string.IsNullOrEmpty(ImportProjectsToCustomId); } }

		public string ImportProjectsToCustomId	{ get { return m_ProjectsAttribCombo.SelectedAttributeCustomId; } }
	}

	/////////////////////////////////////////////////////////////////

	class TodoTxtImporterAttributeComboBox : TodoTxtAttributeComboBox
	{
		public void Populate(IEnumerable<TaskAttributeItem> availAttribs,
							 IEnumerable<CustomAttributeDefinition> customAttribs,
							 string newCustAttribLabel, string newCustAttribId, Translator trans)
		{
			base.Populate(availAttribs, customAttribs, trans);

			// Add the relevant 'new' custom attribute
			Items.Add(new TaskAttributeItem()
			{
				AttributeId = Task.Attribute.CustomAttribute,
				CustomAttributeId = newCustAttribId,
				Label = string.Format(trans.Translate("{0} (Custom)", Translator.Type.Text), trans.Translate(newCustAttribLabel, Translator.Type.ComboBox))
			});
		}
	}

}
