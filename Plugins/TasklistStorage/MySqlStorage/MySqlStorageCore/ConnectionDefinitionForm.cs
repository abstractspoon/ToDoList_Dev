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
				foreach (var field in Fields)
				{
					if (string.IsNullOrEmpty(field.Text))
					{
						field.Focus();
						break;
					}
				}
			};
		}

		public string Server { get { return m_Server.Text; } }
		public string Database { get { return m_Database.Text; } }
		public string Username { get { return m_Username.Text; } }
		public string Password { get { return m_Password.Text; } }

		private Control[] Fields
		{
			get { return new [] { m_Server, m_Database, m_Username, m_Password }; }
		}
	}
}
