
using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using MySql.Data.MySqlClient;
using Abstractspoon.Tdl.PluginHelpers;

////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{
	public class ConnectionDefinition
	{
		public ConnectionDefinition()
		{
		}

		public ConnectionDefinition(string encoded, string password)
		{
			if (Decode(encoded))
			{
				Password = password;
			}
			else
			{
#if DEBUG
				Server = "localhost";
				Database = "Tasklists";
				Username = "root";
				Password = "password";

				TasklistKey = 0;
				TasklistName = "";
#endif
			}
		}

		public string Server;
		public string Database;
		public string Username;
		public string Password;

		public uint TasklistKey = 0;
		public string TasklistName;

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

		public bool OpenConnection(MySqlConnection connection)
		{
			try
			{
				connection.ConnectionString = ConnectionString;
				connection.Open();
			}
			catch (Exception e)
			{

			}

			return (connection.State == System.Data.ConnectionState.Open);
		}
	}

	//////////////////////////////////////////////////////////////////////////

    public class MySqlStorageCore
    {
        private Translator m_trans;

		// ------------------------------------------------------------------

		public MySqlStorageCore(Translator trans)
        {
            m_trans = trans;
        }

		public ConnectionDefinition RetrieveTasklist(string tasklistId, string password, string destPath, bool bSilent, Preferences prefs, string prefKey)
		{
			try
			{
				var def = new ConnectionDefinition(tasklistId, password);

				using (var conn = new MySqlConnection())
				{
					if (!OpenConnection(conn, ref def))
						return null;

					if (def.TasklistKey == 0)
					{
						// Prompt for tasklist 
						var dialog = new OpenTasklistForm(conn, def);

						if (dialog.ShowDialog() != DialogResult.OK)
							return null;

						def.TasklistKey = dialog.TasklistInfo.Key;
					}

					var query = string.Format("SELECT Xml FROM Tasklists WHERE Id={0}", def.TasklistKey);

					using (var command = new MySqlCommand(query, conn))
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

		public ConnectionDefinition StoreTasklist(string tasklistId, string tasklistName, string password, string srcPath, bool bSilent, Preferences prefs, string prefKey)
		{
			try
			{
				var def = new ConnectionDefinition(tasklistId, password);

				using (var conn = new MySqlConnection())
				{
					if (!OpenConnection(conn, ref def))
						return null;

					if (string.IsNullOrEmpty(def.TasklistName))
						def.TasklistName = tasklistName;

					// Always prompt for tasklist name
					var dialog = new SaveTasklistForm(conn, def);

					if (dialog.ShowDialog() != DialogResult.OK)
						return null;

					var tasklist = dialog.TasklistInfo;

					def.TasklistKey = tasklist.Key;
					def.TasklistName = tasklist.Name;

					if (def.TasklistKey == 0)
					{
						string query = string.Format("INSERT INTO Tasklists (Name, Xml) VALUES('{0}', '{1}')", def.TasklistName, File.ReadAllText(srcPath));

						using (var command = new MySqlCommand(query, conn))
						{
							command.ExecuteNonQuery();
						}

						using (var command = new MySqlCommand("SELECT LAST_INSERT_ID()", conn))
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
						string query = string.Format("UPDATE Tasklists SET Name='{0}', Xml='{1}' WHERE Id={2}", def.TasklistName, File.ReadAllText(srcPath), def.TasklistKey);

						using (var command = new MySqlCommand(query, conn))
						{
							command.ExecuteNonQuery();
						}
					}
				}

				return def;
			}
			catch (Exception e)
			{
#if DEBUG
				MessageBox.Show(e.ToString());
#endif
			}

			return null;
        }

		// ------------------------------------------------------------------

		bool OpenConnection(MySqlConnection conn, ref ConnectionDefinition def)
		{
			while (!def.OpenConnection(conn))
			{
				// Prompt for connection details
				var dialog = new ConnectionDefinitionForm(def);

				if (dialog.ShowDialog() != DialogResult.OK)
					return false;

				def = dialog.Definition;
			}

			return true;
		}
	}
}
