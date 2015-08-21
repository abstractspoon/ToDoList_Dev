
using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;
using PluginHelpers;
using MarkdownLog;

namespace MarkdownImpExp
{
    public class MarkdownImpExpCore
    {
        public bool Export(TDLTaskList srcTasks, string sDestFilePath, bool bSilent, TDLPreferences prefs, string sKey)
        {
            UInt32 taskCount = srcTasks.GetTaskCount();

            BulletedMarkdownContainer mdTasks = new BulletedMarkdownContainer();

            TDLTask task = srcTasks.GetFirstTask();

            while (task.IsValid())
            {
                ExportTask(task, mdTasks, true);

                task = task.GetNextTask();
            }

            Debug.Write(mdTasks.ToMarkdown());
            System.IO.File.WriteAllText(sDestFilePath, mdTasks.ToMarkdown());

            return true;
        }

        protected bool ExportTask(TDLTask task, BulletedMarkdownContainer mdParent, bool root)
        {
            // add ourselves
            
            mdParent.Append(new RawMarkdown(FormatTaskAttributes(task, root)));

            // then our subtasks in a container
            BulletedMarkdownContainer mdSubtasks = new BulletedMarkdownContainer();

            TDLTask subtask = task.GetFirstSubtask();

            if (subtask.IsValid())
            {
                while (subtask.IsValid())
                {
                    ExportTask(subtask, mdSubtasks, false);

                    subtask = subtask.GetNextTask();
                }

                mdParent.Append(mdSubtasks);
            }

            return true;
        }

        protected string FormatTaskAttributes(TDLTask task, bool root)
        {
            StringBuilder taskAttrib = new StringBuilder();

            taskAttrib.Append("**`" + task.GetTitle() + "`**");
            taskAttrib.Append("  ").AppendLine().Append("Priority: " + task.GetPriority());
            taskAttrib.Append("  ").AppendLine().Append("Allocated to: " + task.GetAllocatedTo(0));

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
