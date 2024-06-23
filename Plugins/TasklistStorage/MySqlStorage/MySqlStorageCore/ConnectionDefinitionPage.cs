using System;
using System.Windows.Forms;

using System.Diagnostics;

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
			case DbError.DatabaseName:	return m_Database;
			case DbError.Username:		return m_Username;
			case DbError.Password:		return m_Password;
			}

			Debug.Assert(error == DbError.Unknown);
			return null;
		}
	}
}
