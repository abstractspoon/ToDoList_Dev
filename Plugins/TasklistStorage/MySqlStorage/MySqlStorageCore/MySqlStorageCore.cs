﻿
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using MySql.Data.MySqlClient;
using Abstractspoon.Tdl.PluginHelpers;

namespace MySqlStorage
{
    [System.ComponentModel.DesignerCategory("")]
    public class MySqlStorageCore
    {
        public MySqlStorageCore(Translator trans)
        {
            m_trans = trans;
        }

		public bool RetrieveTasklist(TaskList tasklist, bool bSilent, Preferences prefs, string prefKey)
		{
			// Possibly display a dialog to get input on how to 
			// map ToDoList task attributes to the output format
			// TODO

			var connectionString = "Server=www.abstractspoon.com;Database=Tasklists;Uid=abstractspoon;Pwd=&F*VQ]3p*z8B;";

			var conn = new MySqlConnection(connectionString);

			conn.Open();


			return false;
        }

		public bool StoreTasklist(TaskList tasklist, bool bSilent, Preferences prefs, string prefKey)
		{
			// Process task's own attributes
			// TODO

			// Export task's children
// 			Task subtask = task.GetFirstSubtask();
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

            return false;
        }

        // --------------------------------------------------------------------------------------
        private Translator m_trans;
    }
}
