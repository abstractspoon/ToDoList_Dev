using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using MySql.Data.MySqlClient;

namespace MySqlStorage
{

	public class DatabaseDefinition
	{
		public DatabaseDefinition(string encoded = "")
		{
			if (!Decode(encoded))
			{
#if DEBUG
				TasklistsTable = "Tasklists";
				KeyColumn = "Id";
				NameColumn = "Name";
				XmlColumn = "Xml";
#endif
			}
		}

		public bool IsDefined
		{
			get
			{
				return (!string.IsNullOrEmpty(TasklistsTable) &&
						!string.IsNullOrEmpty(KeyColumn) &&
						!string.IsNullOrEmpty(NameColumn) &&
						!string.IsNullOrEmpty(XmlColumn));
			}
		}

		public string Encode()
		{
			return string.Join("||", new object[]
				{
					TasklistsTable,
					KeyColumn,
					NameColumn,
					XmlColumn
				});
		}

		public bool IsValid(MySqlConnection conn)
		{
			if (!IsDefined)
				return false;

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

		// --------------------------------------------------------

		public string TasklistsTable = string.Empty;
		public string KeyColumn = string.Empty;
		public string NameColumn = string.Empty;
		public string XmlColumn = string.Empty;

		// --------------------------------------------------------

		private bool Decode(string encoded)
		{
			var parts = encoded.Split(new[] { "||" }, StringSplitOptions.None);

			if (parts.Length != 4)
				return false;

			TasklistsTable = parts[0];
			KeyColumn = parts[1];
			NameColumn = parts[2];
			XmlColumn = parts[3];

			return true;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	public class ConnectionDefinition
	{
		public ConnectionDefinition() : this("", "")
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
				DatabaseDefinition = new DatabaseDefinition();
#if DEBUG
				Server = "localhost";
				Database = "Tasklists";
				Username = "root";
				Password = "password";
#endif
			}
		}

		public bool OpenConnection(MySqlConnection connection)
		{
			connection.Close();

			if (IsDefined)
			{
				try
				{
					connection.ConnectionString = ConnectionString;
					connection.Open();
				}
				catch (Exception e)
				{
#if DEBUG
					MessageBox.Show(e.ToString());
#endif
				}
			}

			return (connection.State == System.Data.ConnectionState.Open);
		}

		// --------------------------------------------------------

		public string Server = string.Empty;
		public string Database = string.Empty;
		public string Username = string.Empty;
		public string Password = string.Empty;

		public DatabaseDefinition DatabaseDefinition;

		public uint TasklistKey = 0;
		public string TasklistName = string.Empty;

		public string TasklistId { get { return Encode(); } }

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
						DatabaseDefinition.IsDefined);
			}
		}

		private string Encode()
		{
			return string.Join("::", new object[]
				{
					TasklistKey,
					TasklistName,
					Server,
					Database,
					Username,
					DatabaseDefinition.Encode()
				});
		}

		private bool Decode(string encoded)
		{
			var parts = encoded.Split(new[] { "::" }, StringSplitOptions.None);

			if (parts.Length != 6)
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

			DatabaseDefinition = new DatabaseDefinition(parts[5]);
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
