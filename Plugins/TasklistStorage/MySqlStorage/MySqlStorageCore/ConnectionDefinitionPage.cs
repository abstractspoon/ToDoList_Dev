using System;
using System.Windows.Forms;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

////////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{
	public partial class ConnectionDefinitionPage : UserControl
	{
		public ConnectionDefinitionPage()
		{
			InitializeComponent();
		}

		public string Server { get { return m_Server.Text; } }
		public string Database { get { return m_Database.Text; } }
		public string Username { get { return m_Username.Text; } }
		public string Password { get { return m_Password.Text; } }

		public uint Port
		{
			get
			{
				uint port;

				if (uint.TryParse(m_Port.Text, out port))
					return port;

				return ConnectionInfo.DefaultPort;
			}
		}

		public void Initialise(ConnectionInfo conn)
		{
			m_Server.Text = conn.Server;
			m_Port.Text = conn.Port.ToString();
			m_Database.Text = conn.DatabaseName;
			m_Username.Text = conn.Username;
			m_Password.Text = conn.Password;

			Win32.SetEditCue(m_Port.Handle, ConnectionInfo.DefaultPort.ToString());

			if (Visible)
				UIUtils.SetFocusToFirstEmpty(Controls);
		}

		public void HandleError(DbError error)
		{
			var ctrl = MapErrorToField(error);

			if (ctrl != null)
			{
				ctrl.Focus();
				ctrl.SelectAll();
			}
		}

		// ------------------------------------------------

		TextBox MapErrorToField(DbError error)
		{
			switch (error)
			{
			case DbError.Server:		return m_Server;
			case DbError.Port:			return m_Port;
			case DbError.DatabaseName:	return m_Database;
			case DbError.Username:		return m_Username;
			case DbError.Password:		return m_Password;

			case DbError.Success:
			case DbError.Unknown:
				break;

			default:
				Debug.Assert(false);
				break;
			}

			return null;
		}

		private void OnPortKeypress(object sender, KeyPressEventArgs e)
		{
			// Simple masking
			e.Handled = (((e.KeyChar >= 'A') && (e.KeyChar <= 'Z')) ||
						 ((e.KeyChar >= 'a') && (e.KeyChar <= 'z')));
		}
	}
}
