
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
        public bool Export(TaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
        {
            UInt32 taskCount = srcTasks.GetTaskCount();

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

            taskAttrib.Append("**`" + task.GetTitle() + "`**");
            taskAttrib.Append("  ").AppendLine().Append("Priority: " + task.GetPriority(false).ToString());
            taskAttrib.Append("  ").AppendLine().Append("Allocated to: " + task.FormatAllocatedTo(", "));
            taskAttrib.Append(task.GetComments());

            return taskAttrib.AppendLine().ToString();
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
