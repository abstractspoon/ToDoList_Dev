
using System;
using System.IO;
using System.Windows.Forms;
using System.Drawing;

using MySql.Data.MySqlClient;
using Abstractspoon.Tdl.PluginHelpers;

////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{
    public class MySqlStorageCore
    {
        private Translator m_Trans;
        private Font m_ControlsFont;

		private const string FontName = "Tahoma";

		// ------------------------------------------------------------------

		public MySqlStorageCore(Translator trans)
        {
            m_Trans = trans;
			m_ControlsFont = new Font(FontName, 8.25f);

			// Once only translation
			ColumnInfo.PrimaryKey = trans.Translate(ColumnInfo.PrimaryKey, Translator.Type.Text);
		}

		public TasklistConnectionInfo RetrieveTasklist(string tasklistId, string password, string destPath, bool prompt, Preferences prefs, string prefKey)
		{
			try
			{
				var defaultDbInfo = new ConnectionInfo(prefs.GetProfileString(prefKey, "DefaultConnection", ""), password);
				var details = new TasklistConnectionInfo(tasklistId, password, defaultDbInfo);

				using (var conn = new MySqlConnection())
				{
					if (!OpenConnection(conn, details.Connection))
						return null;

					if (prompt || (details.Tasklist.Key == 0))
					{
						// Prompt for tasklist 
						var dialog = new OpenSaveTasklistForm(conn, details, true, m_Trans, m_ControlsFont);

						if (dialog.ShowDialog() != DialogResult.OK)
							return null;

						var tasklist = dialog.TasklistInfo;

						details.Tasklist.Key = tasklist.Key;
						details.Tasklist.Name = tasklist.Name;
					}

					var query = string.Format("SELECT {0} FROM {1} WHERE {2}={3}", 
											  details.Connection.XmlColumn, 
											  details.Connection.TasklistsTable,
											  details.Connection.IdColumn,
											  details.Tasklist.Key);

					using (var command = new MySqlCommand(query, conn))
					{
						using (var reader = command.ExecuteReader())
						{
							if (reader.Read())
							{
								File.WriteAllText(destPath, reader.GetString(0));
								prefs.WriteProfileString(prefKey, "DefaultConnection", details.Connection.Encode());

								return details;
							}
						}
					}
				}
			}
			catch(MySqlException e)
			{
#if DEBUG
				MessageBox.Show(e.ToString());
#endif
			}

			return null;
        }

		public TasklistConnectionInfo StoreTasklist(string tasklistId, string tasklistName, string password, string srcPath, bool prompt, Preferences prefs, string prefKey)
		{
			try
			{
				var defaultDbInfo = new ConnectionInfo(prefs.GetProfileString(prefKey, "DefaultConnection", ""), password);
				var details = new TasklistConnectionInfo(tasklistId, password, defaultDbInfo);

				using (var conn = new MySqlConnection())
				{
					if (!OpenConnection(conn, details.Connection))
						return null;

					if (string.IsNullOrEmpty(details.Tasklist.Name))
						details.Tasklist.Name = tasklistName;

					if (prompt || (details.Tasklist.Key == 0))
					{
						var dialog = new OpenSaveTasklistForm(conn, details, false, m_Trans, m_ControlsFont);

						if (dialog.ShowDialog() != DialogResult.OK)
							return null;

						details.Tasklist = dialog.TasklistInfo;
					}

					bool newTasklist = (details.Tasklist.Key == 0);
					string query;

					if (newTasklist)
					{
						query = string.Format("INSERT INTO {0} ({1}, {2}) VALUES(@Name, @Xml)", 
						 					  details.Connection.TasklistsTable,
											  details.Connection.NameColumn,
											  details.Connection.XmlColumn);
					}
					else
					{
						query = string.Format("UPDATE {0} SET {1}=@Name, {2}=@Xml WHERE {3}={4}",
											  details.Connection.TasklistsTable,
											  details.Connection.NameColumn,
											  details.Connection.XmlColumn,
											  details.Connection.IdColumn,
											  details.Tasklist.Key);
					}

					using (var command = new MySqlCommand(query, conn))
					{
						command.Parameters.AddWithValue("@Name", details.Tasklist.Name);
						command.Parameters.AddWithValue("@Xml", File.ReadAllText(srcPath));

						command.ExecuteNonQuery();

						if (newTasklist)
						{
							// Retrieve the new primary key
							command.CommandText = "SELECT LAST_INSERT_ID()";
							command.Parameters.Clear();

							using (var reader = command.ExecuteReader())
							{
								if (reader.Read())
									uint.TryParse(reader.GetString(0), out details.Tasklist.Key);
							}
						}
					}
				}

				prefs.WriteProfileString(prefKey, "DefaultConnection", details.Connection.Encode());
				return details;
			}
			catch (MySqlException e)
			{
#if DEBUG
				MessageBox.Show(e.ToString());
#endif
			}

			return null;
        }

		// ------------------------------------------------------------------

		bool OpenConnection(MySqlConnection conn, ConnectionInfo connInfo)
		{
			using (var dialog = new DatabaseConnectionForm(m_Trans, m_ControlsFont))
			{
				return dialog.OpenConnection(conn, connInfo, false);
			}
		}
	}
}
