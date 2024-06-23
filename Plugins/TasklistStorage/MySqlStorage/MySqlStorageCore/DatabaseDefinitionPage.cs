using System;
using System.Windows.Forms;

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

			m_KeyColumn.SelectColumn(connInfo.KeyColumn);
			m_NameColumn.SelectColumn(connInfo.NameColumn);
			m_XmlColumn.SelectColumn(connInfo.XmlColumn);

			VisibleChanged += (s, e) =>
			{
				if (Visible)
					SetFocusToFirstEmpty();
			};
		}

		public string TasklistsTable { get { return m_TasklistsTable.Text; } }
		public string KeyColumn { get { return m_KeyColumn.SelectedColumnName; } }
		public string NameColumn { get { return m_NameColumn.SelectedColumnName; } }
		public string XmlColumn { get { return m_XmlColumn.SelectedColumnName; } }

		public bool SetFocusToFirstEmpty()
		{
			return UIUtils.SetFocusToFirstEmpty(Controls);
		}

		// ---------------------------------------------------------------

		private void OnTasklistTableSelChange(object sender, EventArgs e)
		{
			var newTable = m_TasklistsTable.SelectedItem.ToString();

			if (newTable == m_CurrentTable)
				return;

			// (Re)populate column names
			m_KeyColumn.Items.Clear();
			m_NameColumn.Items.Clear();
			m_XmlColumn.Items.Clear();

			foreach (var column in DbUtils.GetTableColumns(m_Connection, newTable))
			{
				if (column.IsPrimaryKey)
				{
					m_KeyColumn.Items.Add(column);
				}
				else
				{
					m_NameColumn.Items.Add(column);
					m_XmlColumn.Items.Add(column);
				}
			}

			m_KeyColumn.SelectOneOnly();
		}

	}

	// ------------------------------------------------------------

}
