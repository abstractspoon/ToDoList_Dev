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
	public partial class ConnectionDefinitionPage : UserControl
	{
		public ConnectionDefinitionPage()
		{
			InitializeComponent();
		}

		public void Initialise(ConnectionInfo def)
		{
			m_Server.Text = def.Server;
			m_Database.Text = def.DatabaseName;
			m_Username.Text = def.Username;
			m_Password.Text = def.Password;

			VisibleChanged += (s, e) =>
			{
				if (Visible)
					SetFocusToFirstEmpty();
			};
		}

		public bool SetFocusToFirstEmpty()
		{
			return UIUtils.SetFocusToFirstEmpty(Controls);
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
