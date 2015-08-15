
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

            MarkdownContainer mdTasks = new MarkdownContainer();

            IntPtr hTask = srcTasks.GetFirstTask(IntPtr.Zero);

            while (hTask != IntPtr.Zero)
            {
                ExportTask(srcTasks, hTask, mdTasks);

                hTask = srcTasks.GetNextTask(hTask);
            }

            Debug.Write(mdTasks.ToMarkdown());

            return true;
        }

        public bool ExportTask(CTaskList srcTasks, IntPtr hTask, MarkdownContainer mdParent)
        {
            // each task is a container, containing itself 
            // and a bulleted list of subtasks
            MarkdownContainer mdTask = new MarkdownContainer();

            // add ourselves
            String sTitle = srcTasks.GetTaskTitle(hTask);
            mdTask.Append(new Paragraph(sTitle));

            // then our subtasks as a bulleted list
            BulletedList subtasks = new BulletedList();

            IntPtr hSubTask = srcTasks.GetFirstTask(hTask);

            while (hSubTask != IntPtr.Zero)
            {
                ExportTask(srcTasks, hSubTask, mdTask);

                hSubTask = srcTasks.GetNextTask(hSubTask);
            }

            mdParent.Append(mdTask);

            return true;
        }
    }
}
