
using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Globalization;

using Newtonsoft.Json.Linq;

using Abstractspoon.Tdl.PluginHelpers;

namespace JSONExporterPlugin
{
    public class JSONExporterCore
    {
		private Translator m_Trans;
		private String m_TypeId;

		// --------------------------------------------------------------------------------------

		public JSONExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
		}

		// ------------------------------------------------------------------------

		public bool Export(TaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
        {
			var tasklists = new List<TaskList>() { srcTasks };

			return ExportTasklists(tasklists, srcTasks.GetReportTitle(), srcTasks.GetReportDate(), sDestFilePath, bSilent, prefs, sKey);
		}

		public bool Export(MultiTaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
		{
			var tasklists = srcTasks.GetTaskLists();

			return ExportTasklists(tasklists, srcTasks.GetReportTitle(), srcTasks.GetReportDate(), sDestFilePath, bSilent, prefs, sKey);
		}

		private bool ExportTasklists(IList<TaskList> srcTasks, 
									 string reportTitle,
									 string reportDate,
									 string sDestFilePath, 
									 bool bSilent, 
									 Preferences prefs, 
									 string sKey)
		{
			var exporter = new JSONExporter(m_Trans);
			var json = exporter.Export(srcTasks, reportTitle, reportDate);

			if (string.IsNullOrWhiteSpace(json))
				return false;

			var utf8 = new UTF8Encoding(false); // No BOM
			System.IO.File.WriteAllBytes(sDestFilePath, utf8.GetBytes(json));

			return true;
		}
	}
}
