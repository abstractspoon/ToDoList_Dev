
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Abstractspoon.Tdl.PluginHelpers;

namespace TodoTxtImpExp
{
    [System.ComponentModel.DesignerCategory("")]

    public class TodoTxtImporter
    {
		private Translator m_Trans;

        // ----------------------------------------------------------

		public TodoTxtImporter(Translator trans)
        {
            m_Trans = trans;
        }

		public bool Import(string srcFilePath, TaskList destTaskFile, uint flags, Preferences prefs, string prefKey)
        {
            // Possibly display a dialog to get input on how to 
            // map ToDoList task attributes to the output format
            // TODO

            // Process the tasks
//             Task task = srcTasks.GetFirstTask();
// 
//             while (task.IsValid())
//             {
//                 if (!ExportTask(task /*, probably with some additional parameters*/ ))
//                 {
//                     // Decide whether to stop or not
//                     // TODO
//                 }
// 
//                 task = task.GetNextTask();
//             }

            return true;
        }

//         protected bool ExportTask(Task task /*, probably with some additional parameters*/)
//         {
//             // Process task's own attributes
//             // TODO
// 
//             // Export task's children
//             Task subtask = task.GetFirstSubtask();
// 
//             while (subtask.IsValid())
//             {
//                 if (!ExportTask(subtask /*, probably with some additional parameters*/ ))
//                 {
//                     // Decide whether to stop or not
//                     // TODO
//                 }
// 
//                 subtask = subtask.GetNextTask();
//             }
// 
//             return true;
//         }

        // ----------------------------------------------------------
    }
}
