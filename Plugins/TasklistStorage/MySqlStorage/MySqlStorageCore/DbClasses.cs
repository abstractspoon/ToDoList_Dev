using System;
using System.Collections.Generic;
using System.Windows.Forms;

using MySql.Data.MySqlClient;

////////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{

	public class ConnectionInfo
	{
		public ConnectionInfo() : this("")
		{
		}

		public ConnectionInfo(ConnectionInfo other)
		{
			Copy(other);
		}

		public ConnectionInfo(string encoded, string password = "", ConnectionInfo defaultInfo = null)
		{
			if (Decode(encoded))
			{
				Password = password;
			}
			else if ((defaultInfo != null) && defaultInfo.IsConnectionDefined(false))
			{
				Copy(defaultInfo);
			}
			else
			{
#if DEBUG
				Server = "localhost";
				DatabaseName = "Tasklists";
				Username = "root";
				Password = "password";
#endif
			}
		}

		public bool OpenConnection(MySqlConnection conn)
		{
			conn.Close();

			if (IsConnectionDefined())
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
			if (!IsConnectionDefined() || !IsDatabaseDefined())
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

		public void Copy(ConnectionInfo fromInfo)
		{
			Server = fromInfo.Server;
			DatabaseName = fromInfo.DatabaseName;
			Username = fromInfo.Username;
			Password = fromInfo.Password;

			TasklistsTable = fromInfo.TasklistsTable;
			KeyColumn = fromInfo.KeyColumn;
			NameColumn = fromInfo.NameColumn;
			XmlColumn = fromInfo.XmlColumn;
		}

		public string Encode() // Excludes password
		{
			return string.Join("::", new object[]
				{
					Server,
					DatabaseName,
					Username,
					TasklistsTable,
					KeyColumn,
					NameColumn,
					XmlColumn
				});
		}

		public bool IsConnectionDefined(bool incPassword = true)
		{
			if (string.IsNullOrEmpty(Server) ||
				string.IsNullOrEmpty(DatabaseName) ||
				string.IsNullOrEmpty(Username))
			{
				return false;
			}

			if (incPassword && string.IsNullOrEmpty(Password))
				return false;

			return true;
		}

		public bool IsDatabaseDefined()
		{
			if (string.IsNullOrEmpty(TasklistsTable) ||
				string.IsNullOrEmpty(KeyColumn) ||
				string.IsNullOrEmpty(NameColumn) ||
				string.IsNullOrEmpty(XmlColumn))
			{
				return false;
			}

			// Columns must be unique
			if ((KeyColumn == NameColumn) ||
				(KeyColumn == XmlColumn) ||
				(XmlColumn == NameColumn))
			{
				return true;
			}

			return true;
		}

		// --------------------------------------------------------

		public string Server = string.Empty;
		public string DatabaseName = string.Empty;
		public string Username = string.Empty;
		public string Password = string.Empty;

		public string TasklistsTable = string.Empty;
		public string KeyColumn = string.Empty;
		public string NameColumn = string.Empty;
		public string XmlColumn = string.Empty;

		// --------------------------------------------------------

		private bool Decode(string encoded)
		{
			var parts = encoded.Split(new[] { "::" }, StringSplitOptions.None);

			if (parts.Length != 7)
				return false;

			Server = parts[0];
			DatabaseName = parts[1];
			Username = parts[2];

			TasklistsTable = parts[3];
			KeyColumn = parts[4];
			NameColumn = parts[5];
			XmlColumn = parts[6];

			return true;
		}

		private string ConnectionString
		{
			get
			{
				return string.Format("Server={0};Database={1};Uid={2};Pwd={3};",
									 Server,
									 DatabaseName,
									 Username,
									 Password);
			}
		}
	}

	//////////////////////////////////////////////////////////////////

	public class TasklistConnectionInfo
	{
		public TasklistConnectionInfo(string tasklistId, string password, ConnectionInfo defConnection)
		{
			var parts = tasklistId.Split(new[] { "||" }, StringSplitOptions.None);

			if (parts.Length == 2)
			{
				Connection = new ConnectionInfo(parts[0], password, defConnection);
				Tasklist = new TasklistInfo(parts[1]);
			}
			else
			{
				if (defConnection != null)
					Connection = defConnection;
				else
					Connection = new ConnectionInfo();

				Tasklist = new TasklistInfo();
			}
		}

		public string TasklistId
		{
			get
			{
				return string.Format("{0}||{1}", Connection.Encode(), Tasklist.Encode());
			}
		}

		public ConnectionInfo Connection;
		public TasklistInfo Tasklist;
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
		public uint Key = 0;
		public string Name = string.Empty;

		// --------------------------------------------------------

		public TasklistInfo(string encoded = "")
		{
			Decode(encoded);
		}

		public override string ToString() { return Name; }

		public string Encode() // Excludes password
		{
			return string.Join("::", new object[]
				{
					Key,
					Name
				});
		}

		private bool Decode(string encoded)
		{
			var parts = encoded.Split(new[] { "::" }, StringSplitOptions.None);

			if (parts.Length != 2)
				return false;

			uint key;

			if (!uint.TryParse(parts[0], out key) || (key == 0))
				return false;

			Key = key;
			Name = parts[1];

			return true;
		}
	}

}
