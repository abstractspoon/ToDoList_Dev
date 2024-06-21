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

namespace MySqlStorage
{
	public partial class DatabaseDefinitionForm : Form
	{
		MySqlConnection m_Connection;
		string m_CurrentTable = string.Empty;

		// ---------------------------------------------------------------

		public DatabaseDefinitionForm(MySqlConnection conn, ConnectionInfo dbInfo)
		{
			InitializeComponent();

			m_Connection = conn;
			m_Database.Text = string.Format("{0}/{1}", dbInfo.Server, dbInfo.DatabaseName);
			m_TasklistsTable.SelectedIndexChanged += new EventHandler(OnTasklistTableSelChange);

			// Populate table combobox
			foreach (var table in DbUtils.GetTableNames(conn))
				m_TasklistsTable.Items.Add(table);

			SelectOneOnly(m_TasklistsTable);
		}

		public string TasklistsTable { get { return m_TasklistsTable.Text; } }
		public string KeyColumn { get { return m_KeyColumn.Text; } }
		public string NameColumn { get { return m_NameColumn.Text; } }
		public string XmlColumn { get { return m_XmlColumn.Text; } }

		// ---------------------------------------------------------------

		private void SelectOneOnly(ComboBox combo)
		{
			if (combo.Items.Count == 1)
				combo.SelectedIndex = 0;
		}

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
	}

	// ------------------------------------------------------------

}
