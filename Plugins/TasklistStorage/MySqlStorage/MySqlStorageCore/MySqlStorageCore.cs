
using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using MySql.Data.MySqlClient;
using Abstractspoon.Tdl.PluginHelpers;

namespace MySqlStorage
{
	public class MySqlStorageDefinition
	{
		public MySqlStorageDefinition(string encoded, string password)
		{
			if (Decode(encoded))
			{
				Password = password;
			}
#if DEBUG
			else
			{
				Server = "www.abstractspoon.com";
				Database = "Tasklists";
				UserName = "abstractspoon";
				Password = "&F*VQ]3p*z8B";

				TasklistKey = 0;
				TasklistName = "Untitled";
			}
#endif
		}

		public string Server;
		public string Database;
		public string UserName;
		public string Password;

		public uint TasklistKey = 0;
		public string TasklistName;

		public string DisplayName
		{
			get
			{
				return string.Format("{0}/{1}({2})", Server, Database, UserName);
			}
		}

		public string ConnectionString
		{
			get
			{
				return string.Format("Server={0};Database={1};Uid={2};Pwd={3};", 
									Server, Database, UserName, Password);
			}
		}

		public string Encode()
		{
			return string.Format("{0}::{1}::{2}::{3}::{4}",
								TasklistKey, TasklistName, Server, Database, UserName);
		}

		public bool Decode(string encoded)
		{
			var parts = encoded.Split(new[] {"::"}, StringSplitOptions.None);

			if (parts.Length != 5)
				return false;

			if (!uint.TryParse(parts[0], out TasklistKey) || (TasklistKey == 0))
			{
				TasklistKey = 0;
				return false;
			}

			TasklistName = parts[1];
			Server = parts[2];
			Database = parts[3];
			UserName = parts[4];

			return true;
		}
	}

	[System.ComponentModel.DesignerCategory("")]
    public class MySqlStorageCore
    {
        public MySqlStorageCore(Translator trans)
        {
            m_trans = trans;
        }

		public MySqlStorageDefinition RetrieveTasklist(string tasklistId, string password, string destPath, bool bSilent, Preferences prefs, string prefKey)
		{
			// Possibly display a dialog to get input on how to 
			// map ToDoList task attributes to the output format
			// TODO
			try
			{
				var def = new MySqlStorageDefinition(tasklistId, password);

				using (var connection = new MySqlConnection(def.ConnectionString))
				{
					connection.Open();

					var query = string.Format("SELECT Xml FROM Tasklists WHERE Id={0}", def.TasklistKey);

					using (var command = new MySqlCommand(query, connection))
					{
						using (var reader = command.ExecuteReader())
						{
							if (reader.Read())
							{
								File.WriteAllText(destPath, reader.GetString(0));
								return def;
							}
						}
					}
				}
			}
			catch(Exception e)
			{

			}

			return null;
        }

		public MySqlStorageDefinition StoreTasklist(string tasklistId, string password, string srcPath, bool bSilent, Preferences prefs, string prefKey)
		{
			try
			{
				var def = new MySqlStorageDefinition(tasklistId, password);

				using (var connection = new MySqlConnection(def.ConnectionString))
				{
					connection.Open();
					
					if (def.TasklistKey == 0)
					{
						string query = string.Format("INSERT INTO Tasklists (Name, Xml) VALUES('{0}', '{1}')", Path.GetFileNameWithoutExtension(srcPath), File.ReadAllText(srcPath));

						using (var command = new MySqlCommand(query, connection))
						{
							command.ExecuteNonQuery();
						}

						using (var command = new MySqlCommand("SELECT LAST_INSERT_ID()", connection))
						{
							using (var reader = command.ExecuteReader())
							{
								if (reader.Read())
									uint.TryParse(reader.GetString(0), out def.TasklistKey);
							}
						}
					}
					else
					{
						string query = string.Format("UPDATE Tasklists SET Xml='{0}' WHERE Id={1}", File.ReadAllText(srcPath), def.TasklistKey);

						using (var command = new MySqlCommand(query, connection))
						{
							command.ExecuteNonQuery();
						}
					}
				}

				return def;
			}
			catch (Exception e)
			{

			}

			return null;
        }

        // --------------------------------------------------------------------------------------
        private Translator m_trans;
    }
}
