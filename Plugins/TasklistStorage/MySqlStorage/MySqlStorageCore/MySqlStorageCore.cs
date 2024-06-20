
using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
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
        }

		public ConnectionDefinition RetrieveTasklist(string tasklistId, string password, string destPath, bool bSilent, Preferences prefs, string prefKey)
		{
			try
			{
				var def = new ConnectionDefinition(tasklistId, password);

				using (var conn = new MySqlConnection())
				{
					if (!OpenConnection(conn, def))
						return null;

					if (def.TasklistKey == 0)
					{
						// Prompt for tasklist 
						var dialog = new OpenTasklistForm(conn, def);

						FormsUtil.SetFont(dialog, m_ControlsFont);
						m_Trans.Translate(dialog);

						if (dialog.ShowDialog() != DialogResult.OK)
							return null;

						var tasklist = dialog.TasklistInfo;

						def.TasklistKey = tasklist.Key;
						def.TasklistName = tasklist.Name;
					}

					var query = string.Format("SELECT {0} FROM {1} WHERE {2}={3}", 
											  def.XmlColumn, 
											  def.TasklistsTable,
											  def.KeyColumn,
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
			catch(Exception e)
			{
#if DEBUG
				MessageBox.Show(e.ToString());
#endif
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
					if (!OpenConnection(conn, def))
						return null;

					if (string.IsNullOrEmpty(def.TasklistName))
						def.TasklistName = tasklistName;

					if (def.TasklistKey == 0)
					{
						var dialog = new SaveTasklistForm(conn, def);

						FormsUtil.SetFont(dialog, m_ControlsFont);
						m_Trans.Translate(dialog);

						if (dialog.ShowDialog() != DialogResult.OK)
							return null;

						var tasklist = dialog.TasklistInfo;

						def.TasklistKey = tasklist.Key;
						def.TasklistName = tasklist.Name;
					}

					bool newTasklist = (def.TasklistKey == 0);
					string query;

					if (newTasklist)
					{
						query = string.Format("INSERT INTO {0} ({1}, {2}) VALUES(@Name, @Xml)", 
						 					  def.TasklistsTable,
											  def.NameColumn,
											  def.XmlColumn);
					}
					else
					{
						query = string.Format("UPDATE {0} SET {1}=@Name, {2}=@Xml WHERE {3}={4}",
											  def.TasklistsTable,
											  def.NameColumn,
											  def.XmlColumn,
											  def.KeyColumn,
											  def.TasklistKey);
					}

					using (var command = new MySqlCommand(query, conn))
					{
						command.Parameters.AddWithValue("@Name", def.TasklistName);
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
									uint.TryParse(reader.GetString(0), out def.TasklistKey);
							}
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

		bool OpenConnection(MySqlConnection conn, ConnectionDefinition def)
		{
			while (!def.OpenConnection(conn))
			{
				// Prompt for connection details
				using (var dialog = new ConnectionDefinitionForm(def))
				{
					FormsUtil.SetFont(dialog, m_ControlsFont);
					m_Trans.Translate(dialog);

					if (dialog.ShowDialog() != DialogResult.OK)
						return false;

					def.Server = dialog.Server;
					def.Database = dialog.Database;
					def.Username = dialog.Username;
					def.Password = dialog.Password;
				}
			}

			while (!def.IsValid(conn))
			{
				// Prompt for database details
				using (var dialog = new DatabaseDefinitionForm(conn, def))
				{
					FormsUtil.SetFont(dialog, m_ControlsFont);
					m_Trans.Translate(dialog);

					if (dialog.ShowDialog() != DialogResult.OK)
						return false;

					def.TasklistsTable = dialog.TasklistsTable;
					def.KeyColumn = dialog.KeyColumn;
					def.NameColumn = dialog.NameColumn;
					def.XmlColumn = dialog.XmlColumn;
				}
			}

			return true;
		}
	}
}
