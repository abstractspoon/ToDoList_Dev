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

			Definition = def;

			// Set the focus to the first empty field
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
		}

		public ConnectionDefinition Definition
		{
			get
			{
				return new ConnectionDefinition()
				{
					Server = m_Server.Text,
					Database = m_Database.Text,
					Username = m_Username.Text,
					Password = m_Password.Text
				};
			}

			set
			{
				m_Server.Text = value.Server;
				m_Database.Text = value.Database;
				m_Username.Text = value.Username;
				m_Password.Text = value.Password;
			}
		}
	}
}
