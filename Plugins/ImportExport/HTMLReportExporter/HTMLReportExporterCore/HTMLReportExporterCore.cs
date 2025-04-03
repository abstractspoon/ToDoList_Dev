
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

		protected bool InitConsts(TaskList tasks, 
								  string reportTitle, 
								  string reportDate, 
								  string destFilePath, 
								  bool silent, 
								  bool printing, 
								  Preferences prefs, 
								  string sKey)
		{
			if (silent)
			{
				var lastTemplate = prefs.GetProfileString(sKey, "LastOpenTemplate", "");

				m_Template = new HtmlReportTemplate(lastTemplate);
			}
			else
			{
				using (var dialog = new HtmlReportTemplateForm(m_TypeId, 
															   m_Trans, 
															   tasks, 
															   reportTitle, 
															   reportDate, 
															   printing, 
															   prefs, 
															   sKey))
				{
					if (!silent && (dialog.ShowDialog() != DialogResult.OK))
						return false;

					m_Template = dialog.ReportTemplate;
				}
			}

			return m_Template.HasContents();
		}

		public bool Export(TaskList srcTasks, string sDestFilePath, bool silent, bool printing, Preferences prefs, string sKey)
		{
			var tasklists = new List<TaskList>() { srcTasks };

			return ExportTasklists(tasklists, 
								   srcTasks.GetReportTitle(), 
								   srcTasks.GetReportDate(), 
								   sDestFilePath, 
								   silent, 
								   printing, 
								   prefs, 
								   sKey);
		}

		public bool Export(MultiTaskList srcTasks, string sDestFilePath, bool silent, bool printing, Preferences prefs, string sKey)
		{
			var tasklists = srcTasks.GetTaskLists();

			return ExportTasklists(tasklists, 
								   srcTasks.GetReportTitle(), 
								   srcTasks.GetReportDate(), 
								   sDestFilePath, 
								   silent, 
								   printing, 
								   prefs, 
								   sKey);
		}

		public bool ExportTasklists(IList<TaskList> tasklists, 
									string reportTitle, 
									string reportDate, 
									string destFilePath,
									bool silent, 
									bool printing, 
									Preferences prefs, 
									string sKey)
		{
			if (!InitConsts(tasklists[0], reportTitle, reportDate, destFilePath, silent, printing, prefs, sKey))
				return false;

			var report = new HtmlReportBuilder(m_Trans, prefs, m_Template, reportTitle, reportDate, false, printing);

			return report.BuildReport(tasklists, destFilePath);
		}
	}
}

