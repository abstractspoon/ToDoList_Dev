using System;
using System.Diagnostics;
using System.Windows.Forms;

using MySql.Data.MySqlClient;
using Abstractspoon.Tdl.PluginHelpers;

////////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{
	public partial class OpenSaveTasklistForm : Form
	{
		bool m_OpenTasklist;

		MySqlConnection m_Connection;
		TasklistConnectionInfo m_TasklistInfo;

		// -----------------------------------------------------------------

		public OpenSaveTasklistForm(MySqlConnection conn, TasklistConnectionInfo tasklistInfo, bool openTasklist)
		{
			InitializeComponent();

			m_OpenTasklist = openTasklist;
			m_Connection = conn;
			m_TasklistInfo = tasklistInfo;

			if (m_OpenTasklist)
			{
				m_Tasklist.ReadOnly = true;
				Text = "Open Tasklist";
			}
			else
			{
				m_Tasklist.ReadOnly = false;
				Text = "Save Tasklist";
			}


			UpdateControlData();

			Shown += (s, e) =>
			{
				if (m_OpenTasklist)
					m_Tasklists.Focus();
				else
					m_Tasklist.Focus();
			};
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

		private void OnModifyDatabase(object sender, EventArgs e)
		{
			using (var dialog = new DatabaseConnectionForm())
			{
// 				FormsUtil.SetFont(dialog, m_ControlsFont);
// 				m_Trans.Translate(dialog);

				if (dialog.OpenConnection(m_Connection, m_TasklistInfo.Connection, true))
					UpdateControlData();
			}
		}

		private void UpdateControlData()
		{
			m_Database.Text = string.Format("{0}/{1}", 
											m_TasklistInfo.Connection.Server, 
											m_TasklistInfo.Connection.DatabaseName);

			m_Tasklist.Text = m_TasklistInfo.Tasklist.Name;
			m_Tasklists.Initialise(m_Connection, m_TasklistInfo.Connection, m_OpenTasklist);
		}
	}
}
