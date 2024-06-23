using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using MySql.Data.MySqlClient;

////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{

	public partial class OpenTasklistForm : Form
	{
		public OpenTasklistForm(MySqlConnection conn, ConnectionInfo connInfo)
		{
			InitializeComponent();

			m_Database.Text = string.Format("{0}/{1}", connInfo.Server, connInfo.DatabaseName);
			m_Tasklists.Initialise(conn, connInfo, true);
		}

		public TasklistInfo TasklistInfo
		{
			get { return (m_Tasklists.SelectedItem as TasklistInfo); }
		}

		private void OnDoubleClickTaskLists(object sender, MouseEventArgs e)
		{
			if (m_Tasklists.IndexFromPoint(e.X, e.Y) != -1)
			{
				DialogResult = DialogResult.OK;
				Close();
			}
		}
	}
	
}
