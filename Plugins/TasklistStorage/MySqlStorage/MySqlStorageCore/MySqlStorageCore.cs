
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

				TasklistsTable = "Tasklists";
				KeyColumn = "Id";
				NameColumn = "Name";
				XmlColumn = "Xml";

				TasklistKey = 0;
				TasklistName = "";
#endif
			}
		}

		public string Server;
		public string Database;
		public string Username;
		public string Password;

		public string TasklistsTable;
		public string KeyColumn;
		public string NameColumn;
		public string XmlColumn;

		public uint TasklistKey = 0;
		public string TasklistName;

		public string TasklistId { get { return Encode(); } }

		public bool OpenConnection(MySqlConnection connection)
		{
			try
			{
				connection.ConnectionString = ConnectionString;
				connection.Open();
			}
			catch (Exception /*e*/)
			{

			}

			return (connection.State == System.Data.ConnectionState.Open);
		}

		// --------------------------------------------------------

		private string ConnectionString
		{
			get
			{
				return string.Format("Server={0};Database={1};Uid={2};Pwd={3};", 
									 Server, 
									 Database, 
									 Username, 
									 Password);
			}
		}

		private string Encode()
		{
			return string.Join("::", 
				new object[] 
				{
					TasklistKey,
					TasklistName,
					Server,
					Database,
					Username,
					TasklistsTable,
					KeyColumn,
					NameColumn,
					XmlColumn
				});
		}

		private bool Decode(string encoded)
		{
			var parts = encoded.Split(new[] {"::"}, StringSplitOptions.None);

			if (parts.Length < 9)
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
			TasklistsTable = parts[5];
			KeyColumn = parts[6];
			NameColumn = parts[7];
			XmlColumn = parts[8];

			return true;
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

						var tasklist = dialog.TasklistInfo;

						def.TasklistKey = tasklist.Key;
						def.TasklistName = tasklist.Name;
					}

					var query = string.Format("SELECT {0} FROM {1} WHERE Id={2}", 
											  def.XmlColumn, 
											  def.TasklistsTable, 
											  def.TasklistKey);

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
			catch(Exception /*e*/)
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
						string query = string.Format("INSERT INTO {0} ({1}, {2}) VALUES('{3}', '{4}')", 
													 def.TasklistsTable,
													 def.NameColumn,
													 def.XmlColumn,
							                         def.TasklistName, 
													 File.ReadAllText(srcPath));

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
						string query = string.Format("UPDATE {0} SET {1}='{2}', {3}='{4}' WHERE Id={5}", 
													 def.TasklistsTable,
													 def.NameColumn,
													 def.TasklistName, 
													 def.XmlColumn,
													 File.ReadAllText(srcPath), 
													 def.TasklistKey);

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
