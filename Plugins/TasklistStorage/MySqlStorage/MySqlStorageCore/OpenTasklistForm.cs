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

	public partial class OpenTasklistForm : Form
	{
		public OpenTasklistForm(MySqlConnection conn, ConnectionDefinition def)
		{
			InitializeComponent();

			m_Database.Text = string.Format("{0}/{1}", def.Server, def.Database);
			m_Tasklists.Initialise(conn, true);
		}

		public TasklistInfo TasklistInfo
		{
			get { return (m_Tasklists.SelectedItem as TasklistInfo); }
		}
	}

	// ------------------------------------------------------------

	public class TasklistInfo
	{
		public override string ToString() { return Name; }

		public uint Key = 0;
		public string Name;
	}
	
	// ------------------------------------------------------------

	internal class TasklistsListBox : ListBox
	{
		public void Initialise(MySqlConnection conn, bool selectFirst)
		{
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

}
