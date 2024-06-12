
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
			try
			{
				var connectionString = "Server=www.abstractspoon.com;Database=Tasklists;Uid=abstractspoon;Pwd=&F*VQ]3p*z8B;";

				List<String> Tablenames = new List<String>();

				using (var connection = new MySqlConnection(connectionString))
				{
					connection.Open();

					using (var command = new MySqlCommand("SHOW TABLES", connection))
					{
						using (var reader = command.ExecuteReader())
						{
							while (reader.Read())
							{
								Tablenames.Add(reader.GetString(0));
							}
						}
					}
				}
			}
			catch(Exception e)
			{

			}

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
