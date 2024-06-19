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
		public DatabaseDefinitionForm(MySqlConnection conn, ConnectionDefinition def)
		{
			InitializeComponent();

			// Populate comboboxes
		}
	}
}
