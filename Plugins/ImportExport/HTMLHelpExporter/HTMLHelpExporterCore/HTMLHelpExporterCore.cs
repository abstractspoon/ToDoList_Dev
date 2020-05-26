
using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
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
        private String m_TempFolder;

		// --------------------------------------------------------------------------------------

		public HTMLHelpExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
			m_TempFolder = (Path.GetTempPath() + typeId);
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
				// Build temporary Html Help Project file
				Directory.CreateDirectory(m_TempFolder);

				String projPath = (m_TempFolder + '\\' + "proj.hhp");

				using (var proj = new System.IO.StreamWriter(projPath))
				{
					proj.WriteLine("[OPTIONS]");
					proj.WriteLine("Compatibility=1.1 or later");
					proj.WriteLine("Compiled file={0}", destFilePath);
					proj.WriteLine("Contents file=toc.hhc");
					proj.WriteLine("Default topic=Task{0}.html", tasks.GetFirstTask().GetID());
					proj.WriteLine("Display compile notes=No");
					proj.WriteLine("Display compile progress=No");
					proj.WriteLine("Error log file=err.log");
					//file.WriteLine("Index file=index.hhk");
					//file.WriteLine("Language=0xc09 English (Australia)");
					proj.WriteLine("[INFOTYPES]");
				}

				// Create temporary Html Help Table of Contents and associated html files
				String tocPath = (m_TempFolder + '\\' + "toc.hhc");

				using (var toc = new System.IO.StreamWriter(tocPath))
				{
					// Header
					toc.WriteLine("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">");
					toc.WriteLine("<HTML>");
					toc.WriteLine("<HEAD>");
					toc.WriteLine("<meta name=\"GENERATOR\" content=\"Microsoft&reg; HTML Help Workshop 4.1\">");
					toc.WriteLine("<!-- Sitemap 1.0 -->");
					toc.WriteLine("</HEAD>");
					toc.WriteLine("<BODY>");
					toc.WriteLine("<OBJECT type=\"text/site properties\">");
					toc.WriteLine("	<param name=\"ImageType\" value=\"Folder\">");
					toc.WriteLine("</OBJECT>");
					toc.WriteLine("<UL>");

					// Top-level tasks
					var task = tasks.GetFirstTask();

					while (task.IsValid())
					{
						if (!ExportTaskAndSubtasks(task, toc))
						{
							// TODO
						}

						task = task.GetNextTask();
					}

					// Footer
					toc.WriteLine("</UL>");
					toc.WriteLine("</BODY>");
					toc.WriteLine("</HTML>");
				}

				// Run the Html Help Compiler
				string hhcPath = "C:\\Program Files (x86)\\HTML Help Workshop\\hhc.exe";

				var compiler = Process.Start(new ProcessStartInfo(hhcPath, ('\"' + projPath + '\"')));

				if (compiler != null)
				{
					compiler.WaitForExit();
					compiler.Close();

					return true;
				}
			}
			catch (Exception /*e*/)
			{
			}

			return false;
		}

		protected bool ExportTaskAndSubtasks(Task task, StreamWriter toc)
		{
			// Create the Html page for this task
			string htmlFileName = string.Format("Task{0}.html", task.GetID());
			String htmlPath = (m_TempFolder + '\\' + htmlFileName);

			using (var html = new System.IO.StreamWriter(htmlPath, false, Encoding.UTF8))
			{
				// Header
				html.WriteLine("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">");
				html.WriteLine("<HTML>");
				html.WriteLine("<HEAD>");
				html.WriteLine("</HEAD>");
				html.WriteLine("<BODY>");
				html.WriteLine("<H1>{0}</H1>", task.GetTitle());

				var comments = task.GetHtmlComments();

				if (String.IsNullOrWhiteSpace(comments))
				{
					comments = task.GetComments();
					comments = comments.Replace("\n", "<BR>");
				}

				html.WriteLine(comments);

				// Footer
				html.WriteLine("</BODY>");
				html.WriteLine("</HTML>");
			}

			// Create the TOC entry for this task
			toc.WriteLine("<LI><OBJECT type=\"text/sitemap\">");
			toc.WriteLine("<param name=\"Name\" value=\"{0}\">", task.GetTitle());
			toc.WriteLine("<param name=\"Local\" value=\"{0}\">", htmlFileName);
			toc.WriteLine("</OBJECT>");

			// Recursively process subtasks
			var subtask = task.GetFirstSubtask();

			if (subtask.IsValid())
			{
				toc.WriteLine("<UL>");

				do
				{
					if (!ExportTaskAndSubtasks(subtask, toc)) // RECURSIVE CALL
					{
						// TODO
					}

					subtask = subtask.GetNextTask();
				}
				while (subtask.IsValid());

				toc.WriteLine("</UL>");
			}

			//file.WriteLine("</LI>");

			return true;
		}
	}
}

