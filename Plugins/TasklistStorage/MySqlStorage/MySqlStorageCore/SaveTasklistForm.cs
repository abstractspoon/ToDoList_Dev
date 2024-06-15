using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using MySql.Data.MySqlClient;

namespace MySqlStorage
{
	public partial class SaveTasklistForm : Form
	{
		public SaveTasklistForm(MySqlConnection conn, ConnectionDefinition def)
		{
			InitializeComponent();

			m_Database.Text = string.Format("{0}/{1}", def.Server, def.Database);

			m_Tasklists.Initialise(conn, false);
			m_Tasklist.Text = def.TasklistName;
		}

		public TasklistInfo TasklistInfo
		{
			get
			{
				var tasklist = (m_Tasklists.SelectedItem as TasklistInfo);

				if (tasklist != null)
				{
					Debug.Assert(m_Tasklist.Text.Equals(tasklist.Name, StringComparison.InvariantCultureIgnoreCase));
					return tasklist;
				}

				// else
				return new TasklistInfo() { Name = m_Tasklist.Text };
			}
		}

		private void OnTasklistTextChanged(object sender, EventArgs e)
		{
			// If the typed text matches one of the existing tasklists
			// select the tasklist else null the selection
			m_Tasklists.SelectedItem = (m_Tasklists.FindItem(m_Tasklist.Text));
		}

		private void OnTasklistsSelectionChange(object sender, EventArgs e)
		{
			// If the new selection does not match the current text
			// change the text to the selection
			var tasklist = (m_Tasklists.SelectedItem as TasklistInfo);

			if (tasklist != null)
			{
				if (!m_Tasklist.Text.Equals(tasklist.Name, StringComparison.InvariantCultureIgnoreCase))
					m_Tasklist.Text = tasklist.Name;
			}
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
