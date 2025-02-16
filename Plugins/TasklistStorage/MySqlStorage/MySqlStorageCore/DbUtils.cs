using System;
using System.Collections.Generic;

using MySql.Data.MySqlClient;

////////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{
	public enum DbError
	{
		Unknown = -1,
		Success = 0,
		Server,
		Port,
		DatabaseName,
		Username,
		Password,
		TasklistsTable,
		IdColumn,
		NameColumn,
		XmlColumn,
	}

	internal class DbUtils
	{
		public static IEnumerable<string> GetTableNames(MySqlConnection conn)
		{
			using (var command = new MySqlCommand("SHOW TABLES", conn))
			{
				using (var reader = command.ExecuteReader())
				{
					while (reader.Read())
					{
						yield return reader.GetString(0);
					}
				}
			}
		}

		public static IEnumerable<ColumnInfo> GetTableColumns(MySqlConnection conn, string table)
		{
			string query = string.Format("SHOW COLUMNS FROM {0}", table);

			using (var command = new MySqlCommand(query, conn))
			{
				using (var reader = command.ExecuteReader())
				{
					while (reader.Read())
					{
						var column = new ColumnInfo() { Name = reader.GetString(0) };

						for (int field = 1; field < reader.FieldCount; field++)
						{
							if (reader.IsDBNull(field))
								column.Attribs.Add("Null");
							else
								column.Attribs.Add(reader.GetString(field));
						}

						yield return column;
					}
				}
			}
		}
	}
}
