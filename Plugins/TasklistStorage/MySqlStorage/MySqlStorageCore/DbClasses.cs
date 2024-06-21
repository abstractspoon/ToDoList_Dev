using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using MySql.Data.MySqlClient;

namespace MySqlStorage
{

	public class ConnectionInfo
	{
		public ConnectionInfo() : this("")
		{
		}

		public ConnectionInfo(string encoded, string password = "", ConnectionInfo defaultInfo = null)
		{
			if (Decode(encoded))
			{
				Password = password;
			}
			else if ((defaultInfo != null) && defaultInfo.IsDefined(false))
			{
				SetConnection(defaultInfo);
			}
			else
			{
#if DEBUG
				Server = "localhost";
				DatabaseName = "Tasklists";
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

			if (IsDefined())
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
			if (!IsDefined())
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

		private void SetConnection(ConnectionInfo dbInfo)
		{
			Server = dbInfo.Server;
			DatabaseName = dbInfo.DatabaseName;
			Username = dbInfo.Username;
			Password = dbInfo.Password;

			TasklistsTable = dbInfo.TasklistsTable;
			KeyColumn = dbInfo.KeyColumn;
			NameColumn = dbInfo.NameColumn;
			XmlColumn = dbInfo.XmlColumn;
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

			Server = parts[2];
			DatabaseName = parts[3];
			Username = parts[4];

			TasklistsTable = parts[5];
			KeyColumn = parts[6];
			NameColumn = parts[7];
			XmlColumn = parts[8];

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

		private bool IsDefined(bool incPassword = true)
		{
			if (string.IsNullOrEmpty(Server) ||
				string.IsNullOrEmpty(DatabaseName) ||
				string.IsNullOrEmpty(Username) ||
				string.IsNullOrEmpty(TasklistsTable) ||
				string.IsNullOrEmpty(KeyColumn) ||
				string.IsNullOrEmpty(NameColumn) ||
				string.IsNullOrEmpty(XmlColumn))
			{
				return false;
			}

			if (incPassword && string.IsNullOrEmpty(Password))
				return false;

			return true;
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
		public string Name;

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
