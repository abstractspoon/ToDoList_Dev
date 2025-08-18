using System;
using System.Drawing;
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

		Translator m_Trans;
		Font m_ControlsFont;

		// -----------------------------------------------------------------

		public OpenSaveTasklistForm(MySqlConnection conn, 
									TasklistConnectionInfo tasklistInfo, 
									bool openTasklist,
									Translator trans,
									Font ctrlsFont)
		{
			InitializeComponent();

			m_OpenTasklist = openTasklist;
			m_Connection = conn;
			m_TasklistInfo = tasklistInfo;
			m_Trans = trans;
			m_ControlsFont = ctrlsFont;

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

			FormsUtil.SetFont(this, m_ControlsFont);
			m_Trans.Translate(this);

			UpdateControlData();

			Shown += (s, e) =>
			{
				if (m_OpenTasklist)
					m_Tasklists.Focus();
				else
					m_Tasklist.Focus();
			};

			Win32.EnableExplorerTheming(m_Tasklists.Handle);
			Win32.SetEditCue(m_Filter.Handle, m_Trans.Translate("<optional>", Translator.Type.Text));
		}

		public TasklistInfo TasklistInfo
		{
			get
			{
				var tasklist = m_Tasklists.SelectedTasklist;

				if (tasklist != null)
				{
					Debug.Assert(m_Tasklist.Text.Equals(tasklist.Name, StringComparison.InvariantCultureIgnoreCase));
					return tasklist;
				}

				// else
				return new TasklistInfo() { Name = m_Tasklist.Text };
			}
		}

		private bool m_HandlingSelectionChange = false;

		private void OnTasklistTextChanged(object sender, EventArgs e)
		{
			// If the typed text matches one of the existing tasklists
			// select the tasklist else null the selection
			if (!m_HandlingSelectionChange)
			{
				m_Tasklists.SelectedTasklist = m_Tasklists.FindTasklist(m_Tasklist.Text);
			}
		}

		private void OnTasklistsSelectionChange(object sender, EventArgs e)
		{
			// If the new selection does not match the current text
			// change the text to the selection
			var tasklist = m_Tasklists.SelectedTasklist;

			if (tasklist != null)
			{
				if (!m_Tasklist.Text.Equals(tasklist.Name, StringComparison.InvariantCultureIgnoreCase))
				{
					m_HandlingSelectionChange = true;
					m_Tasklist.Text = tasklist.Name;
					m_HandlingSelectionChange = false;
				}
			}

			EnableDisableOK();
		}

		private void OnDoubleClickTaskLists(object sender, EventArgs e)
		{
			Point pos = m_Tasklists.PointToClient(Cursor.Position);

			if (m_Tasklists.HitTest(pos.X, pos.Y) != null)
			{
				DialogResult = DialogResult.OK;
				Close();
			}
		}

		private void OnModifyDatabase(object sender, EventArgs e)
		{
			using (var dialog = new DatabaseConnectionForm(m_Trans, m_ControlsFont))
			{
				// Hide ourselves
				dialog.Shown += (s, e2) => { Visible = false; };

				if (dialog.OpenConnection(m_Connection, m_TasklistInfo.Connection, true))
					UpdateControlData();

				Visible = true;
			}
		}

		private void UpdateControlData()
		{
			m_Database.Text = string.Format("{0}:{1}/{2}", 
											m_TasklistInfo.Connection.Server,
											m_TasklistInfo.Connection.Port,
											m_TasklistInfo.Connection.DatabaseName);

			m_Tasklist.Text = m_TasklistInfo.Tasklist.Name;
			m_Tasklists.Initialise(m_Connection, m_TasklistInfo.Connection, m_OpenTasklist);

			EnableDisableOK();
		}

		private void OnFilterTextChanged(object sender, EventArgs e)
		{
			m_Tasklists.Filter = m_Filter.Text;

			EnableDisableOK();
		}

		private void EnableDisableOK()
		{
			OK.Enabled = (m_Tasklists.SelectedTasklist != null);
		}

	}
}
