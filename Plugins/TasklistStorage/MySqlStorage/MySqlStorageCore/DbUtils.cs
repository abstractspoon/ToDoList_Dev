using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MySql.Data.MySqlClient;

namespace MySqlStorage
{
	internal class DbUtils
	{
		public static bool HasTable(MySqlConnection conn, string table)
		{
			string query = string.Format("SHOW COLUMNS FROM {0}", table);

			try
			{
				using (var command = new MySqlCommand(query, conn))
				{
					using (var reader = command.ExecuteReader())
					{
						return true;
					}
				}
			}
			catch (Exception e)
			{

			}

			return false;
		}

		public static bool HasColumn(MySqlConnection conn, string table, string column)
		{
			string query = string.Format("SHOW COLUMNS FROM {0} WHERE field = '{1}';", table, column);

			try
			{
				using (var command = new MySqlCommand(query, conn))
				{
					using (var reader = command.ExecuteReader())
					{
						return true;
					}
				}
			}
			catch (Exception e)
			{

			}

			return false;
		}

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
