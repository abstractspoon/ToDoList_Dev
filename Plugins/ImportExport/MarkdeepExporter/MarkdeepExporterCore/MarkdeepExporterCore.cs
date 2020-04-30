
using System;
using System.Collections.Generic;
using System.Text;
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

		// --------------------------------------------------------------------------------------

		public MarkdeepExporterCore(String typeId, Translator trans)
		{
			m_TypeId = typeId;
			m_Trans = trans;
		}

		// ------------------------------------------------------------------------

		public bool Export(TaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
        {
			if (!InitialiseAttributeList(srcTasks))
				return false;

            BulletedMarkdownContainer mdTasks = new BulletedMarkdownContainer();
            Task task = srcTasks.GetFirstTask();

            while (task.IsValid())
            {
                ExportTask(task, mdTasks, true);

                task = task.GetNextTask();
            }

            StringBuilder mdFile = new StringBuilder();

            mdFile.AppendLine("<meta charset=\"utf-8\">");
            mdFile.AppendLine(mdTasks.ToMarkdown());

            mdFile.AppendLine("<!-- Markdeep: -->");
            mdFile.AppendLine("<style class=\"fallback\">body{visibility:hidden;white-space:pre;font-family:monospace}</style>");
            mdFile.AppendLine("<script src=\"markdeep.min.js\" charset=\"utf-8\"></script>");
            mdFile.AppendLine("<script src=\"https://casual-effects.com/markdeep/latest/markdeep.min.js\" charset=\"utf-8\"></script>");
            mdFile.AppendLine("<script>window.alreadyProcessedMarkdeep||(document.body.style.visibility=\"visible\")</script>");

            Debug.Write(mdFile.ToString());
            System.IO.File.WriteAllText(sDestFilePath, mdFile.ToString(), Encoding.UTF8);

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
                    ExportTask(subtask, mdSubtasks, false);

                    subtask = subtask.GetNextTask();
                }

                mdParent.Append(mdSubtasks);
            }

            return true;
        }

        protected string FormatTaskAttributes(Task task, bool root)
        {
            StringBuilder taskAttrib = new StringBuilder();

            taskAttrib.AppendLine("**`" + task.GetTitle() + "`**");
//             taskAttrib.Append("  ").AppendLine("Priority: " + task.GetPriority(false).ToString());
//             taskAttrib.Append("  ").AppendLine("Allocated to: " + task.FormatAllocatedTo(", "));
            taskAttrib.Append("  ").AppendLine(task.GetComments());

            return taskAttrib.AppendLine().ToString();
        }

		private bool InitialiseAttributeList(TaskList tasks)
		{
			m_Attributes = new List<Tuple<String, Task.Attribute>>();

			// Build a list of sorted attributes
			// excluding certain attributes whose positions are fixed
			foreach (Task.Attribute attrib in Enum.GetValues(typeof(Task.Attribute)))
			{
				switch (attrib)
				{
					case Task.Attribute.Title:
						// Always first
						break;

					case Task.Attribute.Comments:
						// Always last
						break;

					case Task.Attribute.HtmlComments:
						// Always last
						break;

					default:
						if (tasks.IsAttributeAvailable(attrib))
						{
							var attribName = TaskList.GetAttributeName(attrib);

							if (!String.IsNullOrEmpty(attribName))
								m_Attributes.Add(new Tuple<string, Task.Attribute>(m_Trans.Translate(attribName), attrib));
						}
						break;
				}
			}

			if (m_Attributes.Count > 0)
			{
				
			}

			return (m_Attributes.Count > 0);
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
                    string md = ("* " + element.ToMarkdown()).Indent(indent * 4);

                    mdRaw.Append(md);
                    mdRaw.AppendLine();
                }
            }
            indent--;

            return mdRaw.ToString();
        }
    }
}
