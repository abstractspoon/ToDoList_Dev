﻿
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Abstractspoon.Tdl.PluginHelpers;

namespace SampleImpExp
{
    public class SampleImpExpCore
    {
        public bool Export(TaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
        {
            // Possibly display a dialog to get input on how to 
            // map ToDoList task attributes to the output format
            // TODO

            // Process the tasks
            Task task = srcTasks.GetFirstTask();

            while (task.IsValid())
            {
                if (!ExportTask(task /*, probably with some additional parameters*/ ))
                {
                    // Decide whether to stop or not
                    // TODO
                }

                task = task.GetNextTask();
            }

            return true;
        }

        protected bool ExportTask(Task task /*, probably with some additional parameters*/)
        {
            // TODO
            return true;
        }
    }
}
