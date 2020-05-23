
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Web.UI;
using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLHelpExporter
{
	[System.ComponentModel.DesignerCategory("")]
	public class HTMLHelpExporterCore
	{
		// --------------------------------------------------------------------------------------

		private Translator m_Trans;
		private String m_TypeId;

		// --------------------------------------------------------------------------------------

		public HTMLHelpExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
		}

		protected bool InitConsts(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			// TODO
			return true;
		}

		public bool Export(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			if (!InitConsts(tasks, destFilePath, silent, prefs, sKey))
				return false;

			try
			{
				// TODO
			}
			catch (Exception /*e*/)
			{
			}

			return false;
		}
	}
}

