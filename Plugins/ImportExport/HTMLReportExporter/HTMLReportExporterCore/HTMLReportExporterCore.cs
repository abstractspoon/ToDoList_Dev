
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

		protected bool InitConsts(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			if (!silent)
			{
				// Display a dialog to get the report parameters
				using (var dialog = new HtmlReportTemplateForm(m_TypeId, m_Trans, tasks, prefs))
				{
					if (dialog.ShowDialog() != DialogResult.OK)
						return false;

					m_Template = dialog.ReportTemplate;
#if DEBUG
					return false;
#endif
				}
			}
			return true;
		}

		public bool Export(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			if (!InitConsts(tasks, destFilePath, silent, prefs, sKey))
				return false;

			try
			{
				using (var file = new System.IO.StreamWriter(destFilePath))
				{
					using (var html = new HtmlTextWriter(file))
					{
						var report = new HtmlReportBuilder(tasks, m_Template, prefs);

						return report.BuildReport(html);
					}
				}
			}
			catch (Exception /*e*/)
			{
			}

			return false;
		}
	}
}

