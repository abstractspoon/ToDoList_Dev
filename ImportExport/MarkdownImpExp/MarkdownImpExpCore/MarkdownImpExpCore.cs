
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
        public bool Export(CTaskList srcTasks, string sDestFilePath, bool bSilent, CPreferences prefs, string sKey)
        {
            UInt32 taskCount = srcTasks.GetTaskCount();

            BulletedMarkdownContainer mdTasks = new BulletedMarkdownContainer();

            IntPtr hTask = srcTasks.GetFirstTask(IntPtr.Zero);

            while (hTask != IntPtr.Zero)
            {
                ExportTask(srcTasks, hTask, mdTasks, true);

                hTask = srcTasks.GetNextTask(hTask);
            }

            Debug.Write(mdTasks.ToMarkdown());
            System.IO.File.WriteAllText(sDestFilePath, mdTasks.ToMarkdown());

            return true;
        }

        protected bool ExportTask(CTaskList srcTasks, IntPtr hTask, BulletedMarkdownContainer mdParent, bool root)
        {
            // add ourselves
            
            mdParent.Append(new RawMarkdown(FormatTaskAttributes(srcTasks, hTask, root)));

            // then our subtasks in a container
            BulletedMarkdownContainer mdSubtasks = new BulletedMarkdownContainer();

            IntPtr hSubTask = srcTasks.GetFirstTask(hTask);

            if (hSubTask != IntPtr.Zero)
            {
                while (hSubTask != IntPtr.Zero)
                {
                    ExportTask(srcTasks, hSubTask, mdSubtasks, false);

                    hSubTask = srcTasks.GetNextTask(hSubTask);
                }

                mdParent.Append(mdSubtasks);
            }

            return true;
        }

        protected string FormatTaskAttributes(CTaskList srcTasks, IntPtr hTask, bool root)
        {
            StringBuilder task = new StringBuilder();

            task.Append("**`" + srcTasks.GetTaskTitle(hTask) + "`**");
            task.Append("  ").AppendLine().Append("Priority: " + srcTasks.GetTaskPriority(hTask));
            task.Append("  ").AppendLine().Append("Allocated to: " + srcTasks.GetTaskAllocatedTo(hTask, 0));

            return task.AppendLine().ToString();
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
