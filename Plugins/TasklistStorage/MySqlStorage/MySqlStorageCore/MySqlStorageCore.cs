
using System;
using System.IO;
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

		public bool RetrieveTasklist(string tasklistId, string destPath, bool bSilent, Preferences prefs, string prefKey)
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

					using (var command = new MySqlCommand("SELECT Xml FROM Tasklists WHERE Id=2", connection))
					{
						using (var reader = command.ExecuteReader())
						{
							if (reader.Read())
							{
								File.WriteAllText(destPath, reader.GetString(0));
								return true;
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

		public bool StoreTasklist(string tasklistId, string srcPath, bool bSilent, Preferences prefs, string prefKey)
		{
			try
			{
				var connectionString = "Server=www.abstractspoon.com;Database=Tasklists;Uid=abstractspoon;Pwd=&F*VQ]3p*z8B;";

				using (var connection = new MySqlConnection(connectionString))
				{
					connection.Open();

					if (string.IsNullOrEmpty(tasklistId))
					{
						string query = string.Format("INSERT INTO Tasklists (Name, Xml) VALUES('{0}', '{1}')", Path.GetFileNameWithoutExtension(srcPath), File.ReadAllText(srcPath));

						using (var command = new MySqlCommand(query, connection))
						{
							command.ExecuteNonQuery();
						}
					}

				}
			}
			catch (Exception e)
			{

			}

			return false;
        }

        // --------------------------------------------------------------------------------------
        private Translator m_trans;
    }
}
