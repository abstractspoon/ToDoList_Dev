
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Abstractspoon.Tdl.PluginHelpers;

namespace GoogleCalendarImpExp
{
    public class GoogleCalendarImportCore
    {
        public bool Import(string srcFilePath, TaskList destTasks, bool bSilent, Preferences prefs, string sKey)
        {
//             int nVal = prefs.GetProfileInt("bob", "dave", 20);
//             int nVal2 = prefs.GetProfileInt("bob", "phil", 20);
// 
//             // add some dummy values to prefs
//             prefs.WriteProfileInt("bob", "dave", 10);
// 
//             Task task = srcTasks.GetFirstTask();
// 
//             String sTitle = task.GetTitle();
// 

            return true;
        }
    }

    public class GoogleCalendarExportCore
    {
        public bool Export(TaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
        {
            int nVal = prefs.GetProfileInt("bob", "dave", 20);
            int nVal2 = prefs.GetProfileInt("bob", "phil", 20);

            // add some dummy values to prefs
            prefs.WriteProfileInt("bob", "dave", 10);

            Task task = srcTasks.GetFirstTask();

            String sTitle = task.GetTitle();


            return true;
        }
    }
}
