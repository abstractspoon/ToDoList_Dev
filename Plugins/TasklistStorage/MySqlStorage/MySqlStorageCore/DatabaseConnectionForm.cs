using System;
using System.Data;
using System.Windows.Forms;

using MySql.Data.MySqlClient;

////////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{
	public partial class DatabaseConnectionForm : Form
	{
		private MySqlConnection m_Connection;
		private ConnectionInfo m_ConnectionInfo;

		// --------------------------------------------------------

		public DatabaseConnectionForm()
		{
			InitializeComponent();

			Shown += new EventHandler(OnShown);
		}

		public bool OpenConnection(MySqlConnection conn, ConnectionInfo connInfo, bool prompt)
		{
			if (prompt || !connInfo.OpenConnection(conn))
			{
				m_Connection = conn;
				m_ConnectionInfo = connInfo;

				if (ShowDialog() != DialogResult.OK)
					return false;
			}

			return (conn.State == ConnectionState.Open);
		}

		private void OnShown(object sender, EventArgs e)
		{
			m_ConnectionPage.Initialise(m_ConnectionInfo);
		}

		private void OnOK(object sender, EventArgs e)
		{
			Cursor = Cursors.WaitCursor;

			// Preserve database details
			var connInfo = new ConnectionInfo(m_ConnectionInfo);

			connInfo.Server = m_ConnectionPage.Server;
			connInfo.DatabaseName = m_ConnectionPage.Database;
			connInfo.Username = m_ConnectionPage.Username;
			connInfo.Password = m_ConnectionPage.Password;

			if (m_ConnectionPage.Visible)
			{
				// OK button means 'Connect'
				if (!connInfo.OpenConnection(m_Connection))
				{
					if (!connInfo.IsConnectionDefined())
					{
						// One of more empty inputs
						// TODO

					}
					else
					{
						// Show an error message and don't switch pages
						// TODO
					}

					DialogResult = DialogResult.None;
				}
				else if (!connInfo.IsValid(m_Connection))
				{ 
					// Switch to the 'database' page
					m_DatabasePage.Initialise(m_Connection, connInfo);
					m_DatabasePage.Visible = true;

					m_ConnectionPage.Visible = false;

					OK.Text = "OK";
					DialogResult = DialogResult.None;
				}
				else
				{
					m_ConnectionInfo.Copy(connInfo);

					Close();
				}
			}
			else // Database page is active
			{
				connInfo.TasklistsTable = m_DatabasePage.TasklistsTable;
				connInfo.KeyColumn = m_DatabasePage.KeyColumn;
				connInfo.NameColumn = m_DatabasePage.NameColumn;
				connInfo.XmlColumn = m_DatabasePage.XmlColumn;

				if (!connInfo.IsValid(m_Connection))
				{
					if (!connInfo.IsDatabaseDefined())
					{
						// One of more bad inputs
						// TODO
					}
					else
					{
						// Show an error message and don't quit
						// TODO
					}

					DialogResult = DialogResult.None;
				}
				else
				{
					m_ConnectionInfo.Copy(connInfo);

					Close();
				}
			}

			Cursor = Cursors.Default;
		}

		private void OnCancel(object sender, EventArgs e)
		{
			if (m_ConnectionPage.Visible)
			{
				Close();
			}
			else // Database page is active
			{
				// switch back to connection page
				m_DatabasePage.Visible = false;
				m_ConnectionPage.Visible = true;

				DialogResult = DialogResult.None;
				OK.Text = "Connect";
			}
		}
	}
}
