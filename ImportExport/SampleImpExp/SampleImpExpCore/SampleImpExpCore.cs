
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using PluginHelpers;

namespace SampleImpExp
{
    public class SampleImpExpCore
    {
//        public bool Export(string sTaskFilePath, string sDestFilePath, bool bSilent, Dictionary<string, string> prefs)
        public bool Export(CTaskList srcTasks, string sDestFilePath, bool bSilent, CPreferences prefs, string sKey)
        {
            int nVal = prefs.GetProfileInt("bob", "dave", 20);
            int nVal2 = prefs.GetProfileInt("bob", "phil", 20);

            // add some dummy values to prefs
            prefs.WriteProfileInt("bob", "dave", 10);

            IntPtr hTask = srcTasks.GetFirstTask(IntPtr.Zero);

            String sTitle = srcTasks.GetTaskTitle(hTask);

//             Array categories = new Array;
//             UInt32 numCat = srcTasks.GetTaskCategories(hTask, categories);

            return true;
        }
    }
}
