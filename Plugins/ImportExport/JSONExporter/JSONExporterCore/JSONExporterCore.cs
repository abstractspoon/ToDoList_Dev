﻿
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
			var exporter = new JSONExporter();
			var json = exporter.Export(srcTasks, bSilent, prefs, sKey, m_Trans);

			if (string.IsNullOrWhiteSpace(json))
				return false;

			var utf8 = new UTF8Encoding(false); // No BOM
			System.IO.File.WriteAllBytes(sDestFilePath, utf8.GetBytes(json));

			return true;
        }

	}
}
