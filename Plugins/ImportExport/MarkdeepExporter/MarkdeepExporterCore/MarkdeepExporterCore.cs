
using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using MarkdownLog;

using Abstractspoon.Tdl.PluginHelpers;

namespace MarkdeepExporter
{
    public class MarkdeepExporterCore
    {
		private Translator m_Trans;
		private String m_TypeId;

		private List<Tuple<String, Task.Attribute>> m_Attributes;
		private bool m_WantComments, m_WantPosition;
		private string m_FontName;

		// --------------------------------------------------------------------------------------

		public MarkdeepExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
		}

		// ------------------------------------------------------------------------

		public bool Export(TaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
        {
			var tasklists = new List<TaskList>() { srcTasks };
			
			return ExportTasklists(tasklists, sDestFilePath, bSilent, prefs, sKey);
        }

		public bool Export(MultiTaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
        {
			var tasklists = srcTasks.GetTaskLists();

			return ExportTasklists(tasklists, sDestFilePath, bSilent, prefs, sKey);
		}

		private bool ExportTasklists(IList<TaskList> srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
        {
			m_FontName = prefs.GetProfileString("Preferences", "HtmlFont", "Verdana");

			InitialiseAttributeList(srcTasks[0]);

			bool multiTasklist = (srcTasks.Count > 1);
			var mdFile = new MarkdownContainer();

			foreach (var tasklist in srcTasks)
			{
				var mdTasks = new BulletedMarkdownContainer();

				if (srcTasks.Count > 1)
				{
					string title = tasklist.GetProjectName();

					if (string.IsNullOrWhiteSpace(title))
						title = Path.GetFileNameWithoutExtension(tasklist.GetFilePath());

					mdFile.Append(new RawMarkdown("<h3>" + title + "</h3>\n"));
				}

				Task task = tasklist.GetFirstTask();

				while (task.IsValid())
				{
					ExportTask(task, mdTasks, true);
					task = task.GetNextTask();
				}

				mdFile.Append(mdTasks);

				if (srcTasks.Count > 1)
					mdFile.Append(new RawMarkdown("---"));
			}

			StringBuilder fileContents = new StringBuilder();

            fileContents.AppendLine("<meta charset=\"utf-8\">");
            fileContents.AppendLine(mdFile.ToMarkdown());

            fileContents.AppendLine("<!-- Markdeep: -->");
            fileContents.AppendLine(string.Format("<style class=\"fallback\">body{{visibility:hidden;white-space:pre;font-family:{0}}}</style>", m_FontName));
            fileContents.AppendLine("<script src=\"markdeep.min.js\" charset=\"utf-8\"></script>");
            fileContents.AppendLine("<script src=\"https://casual-effects.com/markdeep/latest/markdeep.min.js\" charset=\"utf-8\"></script>");
            fileContents.AppendLine("<script>window.alreadyProcessedMarkdeep||(document.body.style.visibility=\"visible\")</script>");

            Debug.Write(fileContents.ToString());
            System.IO.File.WriteAllText(sDestFilePath, fileContents.ToString(), Encoding.UTF8);

            return true;
        }

        protected bool ExportTask(Task task, BulletedMarkdownContainer mdParent, bool root)
        {
            // add ourselves
            mdParent.Append(new RawMarkdown(FormatTaskAttributes(task, root)));

            Task subtask = task.GetFirstSubtask();

            if (subtask.IsValid())
            {
                // then our subtasks in a container
                BulletedMarkdownContainer mdSubtasks = new BulletedMarkdownContainer();

                while (subtask.IsValid())
                {
                    ExportTask(subtask, mdSubtasks, false); // RECURSIVE CALL

                    subtask = subtask.GetNextTask();
                }

                mdParent.Append(mdSubtasks);
            }

            return true;
        }

        protected string FormatTaskAttributes(Task task, bool root)
        {
            StringBuilder taskAttrib = new StringBuilder();

			// Task title
			taskAttrib.Append("**");

			if (m_WantPosition)
			{
				var pos = task.GetPositionString();

				if (!string.IsNullOrWhiteSpace(pos))
					taskAttrib.Append(pos).Append(" ");
			}

			taskAttrib.Append(task.GetTitle())
					  .Append("**")
					  .AppendLine("<br>");

			// Rest of attributes
			foreach (var item in m_Attributes)
			{
				var attribValue = task.GetAttributeValue(item.Item2, true, true);

				if (!String.IsNullOrWhiteSpace(attribValue))
				{
					taskAttrib.Append("  ")
							  .Append(item.Item1)
							  .Append(": ")
							  .Append(attribValue)
							  .AppendLine("<br>");
				}
			}

			// Comments
			if (m_WantComments)
			{
				var comments = task.GetComments().Trim().Replace("\n", "<br>");

				if (!string.IsNullOrWhiteSpace(comments))
				{
					// Italicise
					taskAttrib.Append("  <br><i>")
							  .Append(comments)
							  .AppendLine("</i>  ");
				}
			}

			return taskAttrib.ToString();
        }

		private void InitialiseAttributeList(TaskList tasks)
		{
			m_Attributes = new List<Tuple<String, Task.Attribute>>();
			m_WantComments = m_WantPosition = false;

			// Construct basic list of attributes
			// excluding those whose positions are fixed
			foreach (Task.Attribute attrib in Enum.GetValues(typeof(Task.Attribute)))
			{
				switch (attrib)
				{
					case Task.Attribute.Title:
						// Always first
						break;

					case Task.Attribute.Comments:
						// Always last if present
						m_WantComments = true;
						break;

					case Task.Attribute.HtmlComments:
						// Always last if present
						m_WantComments = true;
						break;

					case Task.Attribute.Position:
						// Precedes task title
						m_WantPosition = true;
						break;

					case Task.Attribute.ProjectName:
						// Not a task attribute
						break;

					default:
						if (tasks.IsAttributeAvailable(attrib))
							AddAttribute(attrib, m_Attributes, true);
						break;
				}
			}

			// Sort alphabetically
			if (m_Attributes.Count > 1)
				m_Attributes.Sort((x, y) => x.Item1.CompareTo(y.Item1));
		}

		private void AddAttribute(Task.Attribute attrib, List<Tuple<String, Task.Attribute>> attribs, bool atEnd)
		{
			var attribName = TaskList.GetAttributeName(attrib);

			if (!String.IsNullOrEmpty(attribName))
			{
				var item = new Tuple<string, Task.Attribute>(m_Trans.Translate(attribName, Translator.Type.Text), attrib);

				if (atEnd)
					attribs.Add(item);
				else
					attribs.Insert(0, item);
			}
		}
	}

	public class BulletedMarkdownContainer : MarkdownContainer
    {
        public override string ToMarkdown()
        {
            return ToMarkdown(-1);
        }

        protected string ToMarkdown(int indent)
        {
            indent++;
            StringBuilder mdRaw = new StringBuilder();

            foreach (var element in _elements)
            {
                if (element is BulletedMarkdownContainer)
                {
                    string md = (element as BulletedMarkdownContainer).ToMarkdown(indent);

                    mdRaw.Append(md);
                }
                else
                {
                    string md = ("* " + element.ToMarkdown()).Indent(indent);

                    mdRaw.Append(md);
                    mdRaw.AppendLine();
                }
            }
            indent--;

            return mdRaw.ToString();
        }
    }
}
