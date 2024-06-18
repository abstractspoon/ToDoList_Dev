using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MySqlStorage
{
	public partial class ConnectionDefinitionForm : Form
	{
		public ConnectionDefinitionForm(ConnectionDefinition def)
		{
			InitializeComponent();

			m_Server.Text = def.Server;
			m_Database.Text = def.Database;
			m_Username.Text = def.Username;
			m_Password.Text = def.Password;

			// Set the focus to the first empty field
			Shown += (s, e) =>
			{
				if (string.IsNullOrEmpty(def.Server))
				{
					m_Server.Focus();
				}
				else if (string.IsNullOrEmpty(def.Database))
				{
					m_Database.Focus();
				}
				else if (string.IsNullOrEmpty(def.Username))
				{
					m_Username.Focus();
				}
				else if (string.IsNullOrEmpty(def.Password))
				{
					m_Password.Focus();
				}
			};
		}

		public string Server { get { return m_Server.Text; } }
		public string Database { get { return m_Database.Text; } }
		public string Username { get { return m_Username.Text; } }
		public string Password { get { return m_Password.Text; } }
	}
}
