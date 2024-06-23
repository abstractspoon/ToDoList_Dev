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
			DbError unused;

			return OpenConnection(conn, out unused);
		}

		public bool OpenConnection(MySqlConnection conn, out DbError error)
		{
			conn.Close();

			error = DbError.Unknown;
			string errMsg = string.Empty;

			if (IsConnectionDefined())
			{
				try
				{
					conn.ConnectionString = ConnectionString;
					conn.Open();

					if (conn.State == System.Data.ConnectionState.Open)
					{
						error = DbError.Success;
					}
				}
				catch (MySqlException e)
				{
					var innerEx = (e.InnerException as MySqlException);

					if (innerEx != null)
					{
						switch (innerEx.Number)
						{
						case 1049:
							error = DbError.DatabaseName;
							break;

						case 1524:
							error = DbError.Username;
							break;

						case 1045:
							error = DbError.Password;
							break;

						default:
							errMsg = innerEx.Message;
							break;
						}
					}
					else
					{
						errMsg = e.InnerException.Message;
					}
				}
				catch (Exception e)
				{
					switch ((uint)e.InnerException.HResult)
					{
					case 0x80004005:
						error = DbError.Server;
						break;

					default:
						errMsg = e.InnerException.Message;
						break;
					}
				}
			}
#if DEBUG
			if (!string.IsNullOrWhiteSpace(errMsg))
				MessageBox.Show(errMsg);
#endif
			return (error == DbError.Success);
		}

		public bool IsValid(MySqlConnection conn, out DbError error)
		{
			if (!IsConnectionDefined(out error) || !IsDatabaseDefined(out error))
				return false;

			// Server/Database details must be correct if the connection is open
			if (conn.State != System.Data.ConnectionState.Open)
			{
				error = DbError.Unknown;
				return false;
			}

			// Table/column names
			try
			{
				var colNames = new List<string>() { IdColumn, NameColumn, XmlColumn };

				foreach (var column in DbUtils.GetTableColumns(conn, TasklistsTable))
				{
					colNames.RemoveAll(x => (x == column.Name));

					if (colNames.Count == 0)
					{
						error = DbError.Success;
						return true;
					}
					else if (colNames.Contains(IdColumn))
					{
						error = DbError.IdColumn;
					}
					else if (colNames.Contains(NameColumn))
					{
						error = DbError.NameColumn;
					}
					else if (colNames.Contains(XmlColumn))
					{
						error = DbError.XmlColumn;
					}
				}
			}
			catch (MySqlException /*e*/)
			{
				// Bad table name
				error = DbError.TasklistsTable;
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
			IdColumn = fromInfo.IdColumn;
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
					IdColumn,
					NameColumn,
					XmlColumn
				});
		}

		public bool IsConnectionDefined(bool incPassword = true)
		{
			DbError unused;

			return IsConnectionDefined(out unused, incPassword);
		}

		public bool IsConnectionDefined(out DbError error, bool incPassword = true)
		{
			if (string.IsNullOrEmpty(Server))
			{
				error = DbError.Server;
				return false;
			}

			if (string.IsNullOrEmpty(DatabaseName))
			{
				error = DbError.DatabaseName;
				return false;
			}

			if (string.IsNullOrEmpty(Username))
			{
				error = DbError.Username;
				return false;
			}

			if (incPassword && string.IsNullOrEmpty(Password))
			{
				error = DbError.Password;
				return false;
			}

			// else
			error = DbError.Success;
			return true;
		}

		public bool IsDatabaseDefined(out DbError error)
		{
			if (string.IsNullOrEmpty(TasklistsTable))
			{
				error = DbError.TasklistsTable;
				return false;
			}

			if (string.IsNullOrEmpty(IdColumn))
			{
				error = DbError.IdColumn;
				return false;
			}

			if (string.IsNullOrEmpty(NameColumn))
			{
				error = DbError.NameColumn;
				return false;
			}

			if (string.IsNullOrEmpty(XmlColumn))
			{
				error = DbError.XmlColumn;
				return false;
			}

			// Columns must be unique
			if (NameColumn == IdColumn)
			{
				error = DbError.NameColumn;
				return false;
			}

			if (XmlColumn == IdColumn)
			{
				error = DbError.XmlColumn;
				return false;
			}

			if (XmlColumn == NameColumn)
			{
				error = DbError.XmlColumn;
				return false;
			}

			// else
			error = DbError.Success;
			return true;
		}

		// --------------------------------------------------------

		public string Server = string.Empty;
		public string DatabaseName = string.Empty;
		public string Username = string.Empty;
		public string Password = string.Empty;

		public string TasklistsTable = string.Empty;
		public string IdColumn = string.Empty;
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
			IdColumn = parts[4];
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
