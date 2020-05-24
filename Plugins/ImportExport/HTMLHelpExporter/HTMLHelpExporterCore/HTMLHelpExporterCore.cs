
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

		private static bool FileIsWritable(string filePath)
		{
			try
			{
				using (var fs = new FileStream(filePath, FileMode.Open))
				{
					return fs.CanWrite;
				}
			}
			catch (Exception e)
			{
				return false;
			}
		}

		public bool Export(TaskList tasks, string destFilePath, bool silent, Preferences prefs, string sKey)
		{
			if (!InitConsts(tasks, destFilePath, silent, prefs, sKey))
				return false;

			// Output file must be writable
			while (!FileIsWritable(destFilePath))
			{
				var message = String.Format("Please close any open instances of {0} and click OK to continue...", Path.GetFileName(destFilePath));

				if (AppMessageBox.Show("Unable to Access File", message, MessageBoxButtons.OKCancel) == DialogResult.Cancel)
					return false;

				// else retest file
			}

			try
			{
				// Build temporary Html Help Project file
				String tempFolder = (Path.GetTempPath() + m_TypeId);
				Directory.CreateDirectory(tempFolder);

				String projPath = (tempFolder + '\\' + "proj.hhp");
				String tocPath = (tempFolder + '\\' + "toc.hhc");
				//String idxPath = (tempFolder + '\\' + "index.hhc");
				String logPath = (tempFolder + '\\' + "err.log");

				using (var proj = new System.IO.StreamWriter(projPath))
				{
					proj.WriteLine("[OPTIONS]");
					proj.WriteLine("Compatibility=1.1 or later");
					proj.WriteLine("Compiled file={0}", destFilePath);
					proj.WriteLine("Contents file={0}", tocPath);
					proj.WriteLine("Display compile progress=No");
					proj.WriteLine("Error log file={0}", logPath);
					//file.WriteLine("Index file={0}", idxPath);
					//file.WriteLine("Language=0xc09 English (Australia)");
					proj.WriteLine("[INFOTYPES]");
				}

				// Create temporary Html Help Table of Contents and associated html files
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
						if (!ExportTaskAndSubtasks(task, toc, tempFolder))
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
				var compiler = new Process();

				compiler.StartInfo = new ProcessStartInfo(hhcPath, projPath);

				if (compiler.Start())
				{
					compiler.WaitForExit();
					return true;
				}
			}
			catch (Exception e)
			{
			}

			return false;
		}

		protected bool ExportTaskAndSubtasks(Task task, StreamWriter toc, String tempFolder)
		{
			// Create the Html page for this task
			string htmlFileName = string.Format("Task{0}.html", task.GetID());
			String htmlPath = (tempFolder + '\\' + htmlFileName);

			using (var html = new System.IO.StreamWriter(htmlPath))
			{
				// Header
				html.WriteLine("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">");
				html.WriteLine("<HTML>");
				html.WriteLine("<HEAD>");
				html.WriteLine("</HEAD>");
				html.WriteLine("<BODY>");

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
					if (!ExportTaskAndSubtasks(subtask, toc, tempFolder)) // RECURSIVE CALL
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

