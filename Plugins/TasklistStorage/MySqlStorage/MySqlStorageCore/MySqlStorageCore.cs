
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
				Username = "abstractspoon";
				Password = "&F*VQ]3p*z8B";

				TasklistKey = 0;
				TasklistName = "Untitled";
			}
#endif
		}

		public string Server;
		public string Database;
		public string Username;
		public string Password;

		public uint TasklistKey = 0;
		public string TasklistName;

// 		public bool IsDefined
// 		{
// 			get 
// 		}
// 
// 		public bool HasPassword
// 		{
// 
// 		}

		public string DisplayName
		{
			get
			{
				return string.Format("{0}/{1}({2})", Server, Database, Username);
			}
		}

		public string ConnectionString
		{
			get
			{
				return string.Format("Server={0};Database={1};Uid={2};Pwd={3};", 
									Server, Database, Username, Password);
			}
		}

		public string Encode()
		{
			return string.Format("{0}::{1}::{2}::{3}::{4}",
								TasklistKey, TasklistName, Server, Database, Username);
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
			Username = parts[4];

			return true;
		}

		public MySqlConnection OpenConnection()
		{
			var connection = new MySqlConnection(ConnectionString);

			try
			{
				connection.Open();
			}
			catch (Exception e)
			{

			}

			return connection;
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
			try
			{
				var def = new MySqlStorageDefinition(tasklistId, password);

				if (def.TasklistKey == 0)
				{
					var dialog = new ConnectionDetailsForm()
					{
						Server = def.Server,
						Database = def.Database,
						Username = def.Username,
						Password = def.Password
					};

					if (dialog.ShowDialog() != DialogResult.OK)
						return null;

					def.Server = dialog.Server;
					def.Database = dialog.Database;
					def.Username = dialog.Username;
					def.Password = dialog.Password;
				}

				using (var connection = def.OpenConnection())
				{
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

		public MySqlStorageDefinition StoreTasklist(string tasklistId, string tasklistName, string password, string srcPath, bool bSilent, Preferences prefs, string prefKey)
		{
			try
			{
				var def = new MySqlStorageDefinition(tasklistId, password);

				if (string.IsNullOrEmpty(tasklistName))
					tasklistName = def.TasklistName;

				// If this is a new tasklist or has no name prompt the user
				if ((def.TasklistKey == 0) || string.IsNullOrEmpty(tasklistName))
				{
					var dialog = new ConnectionDetailsForm()
					{
						Server = def.Server,
						Database = def.Database,
						Username = def.Username,
						Password = def.Password
					};

					if (dialog.ShowDialog() != DialogResult.OK)
						return null;

					tasklistName = "Untitled";
				}
					
				using (var connection = def.OpenConnection())
				{
					if (def.TasklistKey == 0)
					{
						string query = string.Format("INSERT INTO Tasklists (Name, Xml) VALUES('{0}', '{1}')", tasklistName, File.ReadAllText(srcPath));

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
						string query = string.Format("UPDATE Tasklists SET Name='{0}' Xml='{1}' WHERE Id={2}", tasklistName, File.ReadAllText(srcPath), def.TasklistKey);

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
