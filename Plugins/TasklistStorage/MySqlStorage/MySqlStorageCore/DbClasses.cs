using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using MySql.Data.MySqlClient;

namespace MySqlStorage
{

	public class ConnectionDefinition
	{
		public ConnectionDefinition() : this("", "")
		{
		}

		public ConnectionDefinition(string tasklistId, string password)
		{
			if (Decode(tasklistId))
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
#endif
			}
		}

		public bool OpenConnection(MySqlConnection conn)
		{
			conn.Close();

			if (IsDefined)
			{
				try
				{
					conn.ConnectionString = ConnectionString;
					conn.Open();
				}
				catch (Exception e)
				{
#if DEBUG
					MessageBox.Show(e.ToString());
#endif
				}
			}

			return (conn.State == System.Data.ConnectionState.Open);
		}

		public bool IsValid(MySqlConnection conn)
		{
			if (!IsDefined)
				return false;

			// Server/Database details must be correct if the connection is open
			if (conn.State != System.Data.ConnectionState.Open)
				return false;

			// Table/column names
			try
			{
				var colNames = new List<string>() { KeyColumn, NameColumn, XmlColumn };

				foreach (var column in DbUtils.GetTableColumns(conn, TasklistsTable))
				{
					colNames.RemoveAll(x => (x == column.Name));

					if (colNames.Count == 0)
						return true;
				}
			}
			catch (Exception /*e*/)
			{
				// Bad table name
			}

			return false;
		}

		public string TasklistId
		{
			get { return Encode(); }
		}

		// --------------------------------------------------------

		public string Server = string.Empty;
		public string Database = string.Empty;
		public string Username = string.Empty;
		public string Password = string.Empty;

		public string TasklistsTable = string.Empty;
		public string KeyColumn = string.Empty;
		public string NameColumn = string.Empty;
		public string XmlColumn = string.Empty;

		public uint TasklistKey = 0;
		public string TasklistName = string.Empty;

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

		private bool IsDefined
		{
			get
			{
				return (!string.IsNullOrEmpty(Server) &&
						!string.IsNullOrEmpty(Database) &&
						!string.IsNullOrEmpty(Username) &&
						!string.IsNullOrEmpty(Password) &&
						!string.IsNullOrEmpty(TasklistsTable) &&
						!string.IsNullOrEmpty(KeyColumn) &&
						!string.IsNullOrEmpty(NameColumn) &&
						!string.IsNullOrEmpty(XmlColumn));
			}
		}

		private string Encode()
		{
			// Excludes password
			return string.Join("::", new object[]
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
			var parts = encoded.Split(new[] { "::" }, StringSplitOptions.None);

			if (parts.Length != 9)
				return false;

			if (!uint.TryParse(parts[0], out TasklistKey) || (TasklistKey == 0))
			{
				TasklistKey = 0;
				return false;
			}

			// Excludes password
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

	//////////////////////////////////////////////////////////////////

	class ColumnInfo
	{
		public override string ToString()
		{
			if (IsPrimaryKey)
				return string.Format("{0} (Primary Key)", Name);

			return string.Format("{0} ({1})", Name, Attribs[0]);
		}

		public bool IsPrimaryKey
		{
			get { return Attribs.Contains("PRI"); }
		}

		public string Name = string.Empty;
		public List<string> Attribs = new List<string>();
	}

	//////////////////////////////////////////////////////////////////

	public class TasklistInfo
	{
		public override string ToString() { return Name; }

		public uint Key = 0;
		public string Name;
	}

}
