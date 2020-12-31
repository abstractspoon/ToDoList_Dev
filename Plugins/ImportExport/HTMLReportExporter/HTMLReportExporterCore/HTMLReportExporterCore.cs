
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Web.UI;
using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLReportExporter
{
	[System.ComponentModel.DesignerCategory("")]
	public class HTMLReportExporterCore
	{
		// --------------------------------------------------------------------------------------

		private Translator m_Trans;
		private String m_TypeId;

		private HtmlReportTemplate m_Template;

		// --------------------------------------------------------------------------------------

		public HTMLReportExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
		}

		protected bool InitConsts(TaskList tasks, string destFilePath, bool silent, bool printing, Preferences prefs, string sKey)
		{
			// Display a dialog to get the report parameters
			// or to return the last saved template
			using (var dialog = new HtmlReportTemplateForm(m_TypeId, m_Trans, tasks, printing, prefs, sKey))
			{
				if (!silent && (dialog.ShowDialog() != DialogResult.OK))
					return false;

				m_Template = dialog.ReportTemplate;
			}

			return m_Template.HasContents();
		}

		public bool Export(TaskList tasks, string destFilePath, bool silent, bool printing, Preferences prefs, string sKey)
		{
			if (!InitConsts(tasks, destFilePath, silent, printing, prefs, sKey))
				return false;

			var report = new HtmlReportBuilder(m_Trans, tasks, prefs, m_Template, false, printing);

			return report.BuildReport(destFilePath);
		}
	}
}

