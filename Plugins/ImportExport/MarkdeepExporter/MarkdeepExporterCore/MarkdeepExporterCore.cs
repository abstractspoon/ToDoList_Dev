
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
		private class AttribItem
		{
			public string Label = "";
			public Task.Attribute AttribId = Task.Attribute.Unknown;
			public string CustAttribId = "";
			public CustomAttributeDefinition.Attribute CustAttribType = CustomAttributeDefinition.Attribute.Unknown;
		}

		// --------------------------------------------------------------------------------------

		private Translator m_Trans;
		private String m_TypeId;

		private bool m_WantComments, m_WantPosition;
		private string m_FontName;

		// --------------------------------------------------------------------------------------

		public MarkdeepExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
		}

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

			bool multiTasklist = (srcTasks.Count > 1);
			var mdFile = new MarkdownContainer();

			foreach (var tasklist in srcTasks)
			{
				var mdTasks = new BulletedMarkdownContainer();

				mdFile.Append(new Header(tasklist.GetReportTitle()));
				mdFile.Append(new RawMarkdown(tasklist.GetReportDate()));
				mdFile.Append(new RawMarkdown(tasklist.GetFilePath() + "\n"));

				var attribList = GetAttributeList(tasklist);
				Task task = tasklist.GetFirstTask();

				while (task.IsValid())
				{
					ExportTask(task, attribList, mdTasks, true);
					task = task.GetNextTask();
				}

				mdFile.Append(mdTasks);
			}

			StringBuilder fileContents = new StringBuilder();

            fileContents.AppendLine("<meta charset=\"utf-8\">");
            fileContents.AppendLine(mdFile.ToMarkdown());

            fileContents.AppendLine("<!-- Markdeep: -->");
            fileContents.AppendLine(string.Format("<style class=\"fallback\">body{{visibility:hidden;white-space:pre;font-family:{0}}}</style>", m_FontName));
            fileContents.AppendLine("<script src=\"markdeep.min.js\" charset=\"utf-8\"></script>");
            fileContents.AppendLine("<script src=\"https://morgan3d.github.io/markdeep/latest/markdeep.min.js\" charset=\"utf-8\"></script>");
            fileContents.AppendLine("<script>window.alreadyProcessedMarkdeep||(document.body.style.visibility=\"visible\")</script>");

            Debug.Write(fileContents.ToString());
            System.IO.File.WriteAllText(sDestFilePath, fileContents.ToString(), Encoding.UTF8);

            return true;
        }

        private bool ExportTask(Task task, IList<AttribItem> attribList, BulletedMarkdownContainer mdParent, bool root)
        {
            // add ourselves
            mdParent.Append(new RawMarkdown(FormatTaskAttributes(task, attribList, root)));

            Task subtask = task.GetFirstSubtask();

            if (subtask.IsValid())
            {
                // then our subtasks in a container
                BulletedMarkdownContainer mdSubtasks = new BulletedMarkdownContainer();

                while (subtask.IsValid())
                {
                    ExportTask(subtask, attribList, mdSubtasks, false); // RECURSIVE CALL

                    subtask = subtask.GetNextTask();
                }

                mdParent.Append(mdSubtasks);
            }

            return true;
        }

        private string FormatTaskAttributes(Task task, IList<AttribItem> attribList, bool root)
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
			foreach (var attrib in attribList)
			{
				var attribVal = string.Empty;

				if (attrib.AttribId == Task.Attribute.CustomAttribute)
					attribVal = task.GetCustomAttributeValue(attrib.CustAttribId, true);
				else
					attribVal = task.GetAttributeValue(attrib.AttribId, true, true);

				if (!String.IsNullOrWhiteSpace(attribVal))
				{
					taskAttrib.Append("  ")
							  .Append(attrib.Label)
							  .Append(": ")
							  .Append(attribVal)
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

		private IList<AttribItem> GetAttributeList(TaskList tasks)
		{
			var attribList = new List<AttribItem>();

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

				case Task.Attribute.CustomAttribute:
					if (tasks.IsAttributeAvailable(attrib))
					{
						var custAttribs = tasks.GetCustomAttributes();

						foreach (var custAttrib in custAttribs)
						{
							attribList.Add(new AttribItem()
							{
								Label = custAttrib.Label,
								AttribId = attrib,
								CustAttribId = custAttrib.Id,
								CustAttribType = custAttrib.AttributeType
							});
						}
					}
					break;

				default:
					{
						var attribName = TaskList.GetAttributeName(attrib);

						if (!String.IsNullOrEmpty(attribName))
						{
							attribList.Add(new AttribItem()
							{
								Label = m_Trans.Translate(attribName, Translator.Type.Text),
								AttribId = attrib
							});
						}
					}
					break;
				}
			}

			// Sort alphabetically
			if (attribList.Count > 1)
				attribList.Sort((x, y) => x.Label.CompareTo(y.Label));

			return attribList;
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
