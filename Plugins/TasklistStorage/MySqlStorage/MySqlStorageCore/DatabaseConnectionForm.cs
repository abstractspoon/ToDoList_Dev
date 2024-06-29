using System;
using System.Data;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;

using MySql.Data.MySqlClient;
using Abstractspoon.Tdl.PluginHelpers;

////////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{
	public partial class DatabaseConnectionForm : Form
	{
		private MySqlConnection m_Connection;
		private ConnectionInfo m_ConnectionInfo;

		bool m_Prompt = true;

		// --------------------------------------------------------

		public DatabaseConnectionForm(Translator trans,	Font ctrlsFont)
		{
			InitializeComponent();

			FormsUtil.SetFont(this, ctrlsFont);
			trans.Translate(this);

			Shown += new EventHandler(OnShown);
		}

		public bool OpenConnection(MySqlConnection conn, ConnectionInfo connInfo, bool prompt)
		{
			if (prompt || !connInfo.OpenConnection(conn))
			{
				m_Connection = conn;
				m_ConnectionInfo = connInfo;
				m_Prompt = prompt;

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
				DbError error;

				if (!connInfo.IsConnectionDefined(out error) ||
					!connInfo.OpenConnection(m_Connection, out error))
				{
					// Highlight the faulty input
					m_ConnectionPage.HandleError(error);

					// and don't switch pages
					DialogResult = DialogResult.None;
				}
				else if (m_Prompt || !connInfo.IsValid(m_Connection, out error))
				{ 
					// Switch to the 'database' page
					m_DatabasePage.Initialise(m_Connection, connInfo);
					m_DatabasePage.HandleError(error);

					m_DatabasePage.Visible = true;
					m_ConnectionPage.Visible = false;

					OK.Text = "OK";
					DialogResult = DialogResult.None;
				}
				else // Finished
				{
					m_ConnectionInfo.Copy(connInfo);
					Close();
				}
			}
			else // Database page is active
			{
				connInfo.TasklistsTable = m_DatabasePage.TasklistsTable;
				connInfo.IdColumn = m_DatabasePage.IdColumn;
				connInfo.NameColumn = m_DatabasePage.NameColumn;
				connInfo.XmlColumn = m_DatabasePage.XmlColumn;

				DbError error;

				if (!connInfo.IsDatabaseDefined(out error) ||
					!connInfo.IsValid(m_Connection, out error))
				{
					// Highlight the faulty input
					m_DatabasePage.HandleError(error);

					// and don't switch pages
					DialogResult = DialogResult.None;
				}
				else // Finished
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
