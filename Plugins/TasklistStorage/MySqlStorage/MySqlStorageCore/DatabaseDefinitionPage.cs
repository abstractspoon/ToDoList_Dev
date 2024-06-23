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

			SelectOneOnly(m_TasklistsTable);

			SelectColumn(m_KeyColumn, connInfo.KeyColumn);
			SelectColumn(m_NameColumn, connInfo.NameColumn);
			SelectColumn(m_XmlColumn, connInfo.XmlColumn);
		}

		public string TasklistsTable { get { return m_TasklistsTable.Text; } }
		public string KeyColumn { get { return GetSelectedColumnName(m_KeyColumn); } }
		public string NameColumn { get { return GetSelectedColumnName(m_NameColumn); } }
		public string XmlColumn { get { return GetSelectedColumnName(m_XmlColumn); } }

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

			SelectOneOnly(m_KeyColumn);
		}

		private static void SelectOneOnly(ComboBox combo)
		{
			if (combo.Items.Count == 1)
				combo.SelectedIndex = 0;
		}

		private static string GetSelectedColumnName(ComboBox combo)
		{
			if (combo.SelectedItem == null)
				return string.Empty;

			return (combo.SelectedItem as ColumnInfo).Name;
		}

		private static void SelectColumn(ComboBox combo, string colName)
		{
			if (string.IsNullOrWhiteSpace(colName))
				return;

			foreach (var item in combo.Items)
			{
				if ((item as ColumnInfo).Name == colName)
				{
					combo.SelectedItem = item;
					break;
				}
			}
		}
	}

	// ------------------------------------------------------------

}
