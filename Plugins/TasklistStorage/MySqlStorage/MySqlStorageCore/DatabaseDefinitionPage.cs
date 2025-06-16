using System;
using System.Windows.Forms;
using System.Diagnostics;

using MySql.Data.MySqlClient;

////////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{
	public partial class DatabaseDefinitionPage : UserControl
	{
		MySqlConnection m_Connection;

		string m_CurrentTable = string.Empty;

		// ---------------------------------------------------------------

		public DatabaseDefinitionPage()
		{
			InitializeComponent();
		}

		public void Initialise(MySqlConnection conn, ConnectionInfo connInfo)
		{
			m_Connection = conn;
			m_TasklistsTable.SelectedIndexChanged += new EventHandler(OnTasklistTableSelChange);

			// Populate table combobox
			foreach (var table in DbUtils.GetTableNames(conn))
				m_TasklistsTable.Items.Add(table);

			UIUtils.SelectOneOnly(m_TasklistsTable);

			m_IdColumn.SelectColumn(connInfo.IdColumn);
			m_NameColumn.SelectColumn(connInfo.NameColumn);
			m_XmlColumn.SelectColumn(connInfo.XmlColumn);

			m_IdColumn.Enabled = false; // MUST REVERT FOR PR

			VisibleChanged += (s, e) =>
			{
				if (Visible)
					UIUtils.SetFocusToFirstEmpty(Controls);
			};
		}

		public string TasklistsTable { get { return m_TasklistsTable.Text; } }
		public string IdColumn { get { return m_IdColumn.SelectedColumnName; } }
		public string NameColumn { get { return m_NameColumn.SelectedColumnName; } }
		public string XmlColumn { get { return m_XmlColumn.SelectedColumnName; } }

		public void HandleError(DbError error)
		{
			var ctrl = MapErrorToField(error);

			if (ctrl != null)
				ctrl.Focus();
		}

		// ---------------------------------------------------------------

		private void OnTasklistTableSelChange(object sender, EventArgs e)
		{
			var newTable = m_TasklistsTable.SelectedItem.ToString();

			if (newTable == m_CurrentTable)
				return;

			// (Re)populate column names
			var columns = DbUtils.GetTableColumns(m_Connection, newTable);

			m_IdColumn.Populate(columns, true);
			m_NameColumn.Populate(columns, false);
			m_XmlColumn.Populate(columns, false);
		}

		ComboBox MapErrorToField(DbError error)
		{
			switch (error)
			{
			case DbError.TasklistsTable:	return m_TasklistsTable;
			case DbError.IdColumn:			return m_IdColumn;
			case DbError.NameColumn:		return m_NameColumn;
			case DbError.XmlColumn:			return m_XmlColumn;

			case DbError.Success:
				break;

			default:
				Debug.Assert(false);
				break;
			}

			return null;
		}
	}

	// ------------------------------------------------------------
}
