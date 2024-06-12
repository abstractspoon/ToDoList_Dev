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
	public partial class ConnectionDetailsForm : Form
	{
		public ConnectionDetailsForm()
		{
			InitializeComponent();
		}

		public string Server
		{
			get { return m_Server.Text; }
			set { m_Server.Text = value; }
		}

		public string Database
		{
			get { return m_Database.Text; }
			set { m_Database.Text = value; }
		}

		public string Username
		{
			get { return m_UserName.Text; }
			set { m_UserName.Text = value; }
		}

		public string Password
		{
			get { return m_Password.Text; }
			set { m_Password.Text = value; }
		}
	}
}
