using System;
using System.Windows.Forms;

using MySql.Data.MySqlClient;

////////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{
	internal class UIUtils
	{
		public static bool SetFocusToFirstEmpty(Control.ControlCollection ctrls)
		{
			for (int i = 0; i < ctrls.Count; i++)
			{
				if (string.IsNullOrEmpty(ctrls[i].Text))
				{
					ctrls[i].Focus();
					return true;
				}
			}

			return false;
		}

		public static bool SelectOneOnly(ComboBox combo)
		{
			if (combo.Items.Count == 1)
			{
				combo.SelectedIndex = 0;
				return true;
			}

			return false;
		}

	}

	////////////////////////////////////////////////////////////////////////

	internal class ColumnComboBox : ComboBox
	{
		public bool SelectOneOnly()
		{
			return UIUtils.SelectOneOnly(this);
		}

		public string SelectedColumnName
		{
			get
			{
				if (SelectedItem == null)
					return string.Empty;

				return (SelectedItem as ColumnInfo).Name;
			}
		}

		public bool SelectColumn(string colName)
		{
			if (!string.IsNullOrWhiteSpace(colName))
			{
				foreach (var item in Items)
				{
					if ((item as ColumnInfo).Name == colName)
					{
						SelectedItem = item;
						return true;
					}
				}
			}

			return false;
		}

	}

	////////////////////////////////////////////////////////////////////////

	internal class TasklistsListView : ListView
	{
		public void Initialise(MySqlConnection conn, ConnectionInfo connInfo, bool selectFirst)
		{
			Items.Clear();

			string query = string.Format("SELECT Id, Name FROM {0}", connInfo.TasklistsTable);

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

						var item = new ListViewItem(tasklist.Name);
						item.Tag = tasklist;

						Items.Add(item); 
					}
				}
			}

			if (selectFirst && (Items.Count > 0))
				SelectedIndices.Add(0);
		}

		public TasklistInfo FindTasklist(string name)
		{
			foreach (ListViewItem item in Items)
			{
				if (name.Equals(item.Text, StringComparison.InvariantCultureIgnoreCase))
					return (item.Tag as TasklistInfo);
			}

			// else
			return null;
		}

		public TasklistInfo SelectedTasklist
		{
			get
			{
				if (SelectedItems.Count == 0)
					return null;

				return (SelectedItems[0].Tag as TasklistInfo);
			}

			set
			{
				SelectedIndices.Clear();

				foreach (ListViewItem item in Items)
				{
					if (item.Tag == value)
					{
						SelectedIndices.Add(item.Index);
						FocusedItem = item;
						break;
					}
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////

}
