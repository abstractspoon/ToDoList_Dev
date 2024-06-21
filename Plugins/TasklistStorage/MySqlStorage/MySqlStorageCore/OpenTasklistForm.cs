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
		public OpenTasklistForm(MySqlConnection conn, ConnectionInfo dbInfo)
		{
			InitializeComponent();

			m_Database.Text = string.Format("{0}/{1}", dbInfo.Server, dbInfo.DatabaseName);
			m_Tasklists.Initialise(conn, dbInfo, true);
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
	
	// ------------------------------------------------------------

	internal class TasklistsListBox : ListBox
	{
		public void Initialise(MySqlConnection conn, ConnectionInfo dbInfo, bool selectFirst)
		{
			string query = string.Format("SELECT Id, Name FROM {0}", dbInfo.TasklistsTable);

			using (var command = new MySqlCommand(query, conn))
			{
				using (var reader = command.ExecuteReader())
				{
					while (reader.Read())
					{
						var tasklist = new TasklistInfo()
						{
							Key = reader.GetUInt32(0),
							Name = reader.GetString(1)
						};

						Items.Add(tasklist);
					}
				}
			}

			if (selectFirst && (Items.Count > 0))
				SelectedIndex = 0;
		}

		public object FindItem(string name)
		{
			foreach (var item in Items)
			{
				if (name.Equals(item.ToString(), StringComparison.InvariantCultureIgnoreCase))
					return item;
			}

			// else
			return null;
		}
	}

}
