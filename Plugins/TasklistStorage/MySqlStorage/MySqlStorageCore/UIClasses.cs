using System;
using System.Windows.Forms;

using MySql.Data.MySqlClient;

////////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{
	internal class TasklistsListBox : ListBox
	{
		public void Initialise(MySqlConnection conn, ConnectionInfo connInfo, bool selectFirst)
		{
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

						Items.Add(tasklist);
					}
				}
			}

			if (selectFirst && (Items.Count > 0))
				SelectedIndex = 0;
		}

		public object FindItem(string name)
		{
			foreach (var item in Items)
			{
				if (name.Equals(item.ToString(), StringComparison.InvariantCultureIgnoreCase))
					return item;
			}

			// else
			return null;
		}
	}

	///////////////////////////////////////////////////////////////////////////

}
