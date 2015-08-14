
using System;
using System.Collections.Generic;
using System.Text;
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
            IntPtr hTask = srcTasks.GetFirstTask(IntPtr.Zero);

            while (hTask != IntPtr.Zero)
            {
                String sTitle = srcTasks.GetTaskTitle(hTask);





                hTask = srcTasks.GetNextTask(hTask);
            }

            return true;
        }
    }
}
