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

////////////////////////////////////////////////////////////////////////

namespace MySqlStorage
{

	public partial class TasklistSelectionForm : Form
	{
		public TasklistSelectionForm(MySqlConnection conn, ConnectionDefinition def)
		{
			InitializeComponent();

			m_Database.Text = string.Format("{0}/{1}", def.Server, def.Database);

			// Populate the combobox
			using (var command = new MySqlCommand("SELECT Id, Name FROM Tasklists", conn))
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

						m_Tasklists.Items.Add(tasklist);
					}
				}
			}

			// If there is only one tasklist, select it
			if (m_Tasklists.Items.Count == 1)
				m_Tasklists.SelectedIndex = 0;
		}

		public uint SelectedTasklistKey
		{
			get
			{
				if (m_Tasklists.SelectedItem != null)
					return (m_Tasklists.SelectedItem as TasklistInfo).Key; 

				return 0;
			}

		}
	}

	// ------------------------------------------------------------

	class TasklistInfo
	{
		public override string ToString() { return Name; }

		public uint Key;
		public string Name;
	}

}
