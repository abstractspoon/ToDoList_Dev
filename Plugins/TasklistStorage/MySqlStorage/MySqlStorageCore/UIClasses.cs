using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;

using UIComponents;
using MySql.Data.MySqlClient;

using Abstractspoon.Tdl.PluginHelpers;

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

	internal class ColumnComboBox : OwnerdrawComboBoxBase
	{
		int m_MaxColNameWidth = -1;

		static string m_Separator = "  -  ";

		// --------------------------------------------------

		public ColumnComboBox() : base(true) // fixed
		{
		}

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

		public int Populate(IEnumerable<ColumnInfo> columns, bool primaryKey)
		{
			Items.Clear();
			m_MaxColNameWidth = -1;
			int maxColTypeWidth = -1; // only required for drop down width

			using (var graphics = CreateGraphics())
			{
				foreach (var column in columns)
				{
					if (column.IsPrimaryKey == primaryKey)
						Items.Add(column);

					m_MaxColNameWidth = Math.Max(m_MaxColNameWidth, (int)graphics.MeasureString(column.Name, Font).Width);
					maxColTypeWidth = Math.Max(maxColTypeWidth, (int)graphics.MeasureString(column.Name, Font).Width);
				}

				DropDownWidth = Math.Max(Width, (m_MaxColNameWidth + maxColTypeWidth + (int)graphics.MeasureString(m_Separator, Font).Width));
			}

			if (primaryKey)
				SelectOneOnly();

			return Items.Count;
		}

		protected override void OnDrawItem(DrawItemEventArgs e)
		{
			base.OnDrawItem(e);

			if (e.Index >= 0)
			{
				var column = (Items[e.Index] as ColumnInfo);

				using (var brush = TextBrush(e))
				{
					var format = new StringFormat()
					{
						LineAlignment = StringAlignment.Center,
						Trimming = StringTrimming.None
					};

					e.Graphics.DrawString(column.Name, e.Font, brush, e.Bounds, format);

					var typeRect = e.Bounds;
					typeRect.Offset(m_MaxColNameWidth, 0);

					e.Graphics.DrawString(m_Separator + column.Type, e.Font, brush, typeRect, format);
				}
			}
		}

	}

	////////////////////////////////////////////////////////////////////////

	internal class TasklistsListView : ListView
	{
		class TasklistItem
		{
			public uint Id;
			public string Name;
			public string Size;
			public string LastMod;
		}

		// -----------------------------------------------------

		private List<TasklistItem> m_Tasklists = new List<TasklistItem>();

		// -----------------------------------------------------

		public void Initialise(MySqlConnection conn, ConnectionInfo connInfo, bool selectFirst)
		{
			DPIScaling.ScaleColumnWidths(this);

			Items.Clear();

			try
			{
				string query = string.Format("SELECT {0}, {1}, LENGTH({2}), ExtractValue({2}, '/TODOLIST/attribute::LASTMODSTRING') FROM {3}", 
											 connInfo.IdColumn,
											 connInfo.NameColumn, 
											 connInfo.XmlColumn,
											 connInfo.TasklistsTable);

				using (var command = new MySqlCommand(query, conn))
				{
					using (var reader = command.ExecuteReader())
					{
						while (reader.Read())
						{
							m_Tasklists.Add(new TasklistItem()
							{
								Id = reader.GetUInt32(0),
								Name = reader.GetString(1),
								Size = FormatSize(reader.GetUInt32(2)),
								LastMod = reader.GetString(3)
							});
						}
					}
				}

				Populate();

				if (selectFirst && (Items.Count > 0))
					SelectedIndices.Add(0);
			}
			catch (Exception /*e*/)
			{

			}
		}

		private void Populate(string filter = "")
		{
			// Cache selected tasklist
			var selTasklist = SelectedTasklist;

			Items.Clear();

			foreach (var tasklist in m_Tasklists)
			{
				if (tasklist.Name.IndexOf(filter, StringComparison.InvariantCultureIgnoreCase) == 0)
				{
					var item = new ListViewItem(tasklist.Name);

					item.SubItems.Add(tasklist.Size);
					item.SubItems.Add(tasklist.LastMod);
					item.Tag = tasklist;

					Items.Add(item);
				}
			}

			if (Items.Count == 1)
			{
				SelectedIndices.Add(0);
			}
			else if (selTasklist != null)
			{
				SelectedTasklist = selTasklist;
			}
		}

		public string Filter
		{
			set { Populate(value); }
		}

		private string FormatSize(uint size)
		{
			if (size == 0)
				return "0 KB";

			if (size < 1024)
				return "1 KB";

			if (size < (1024 * 1024))
				return string.Format("{0} KB", (size / 1024)); // no decimal

			// else
			return string.Format("{0:0.00} MB", (size / (1024.0 * 1024.0)));
		}

		public TasklistInfo FindTasklist(string name)
		{
			foreach (ListViewItem item in Items)
			{
				var tasklist = (item.Tag as TasklistItem);

				if (name.Equals(tasklist.Name, StringComparison.InvariantCultureIgnoreCase))
					return GetTaskListInfo(tasklist);
			}

			// else
			return null;
		}

		ListViewItem FindTasklistItem(uint id)
		{
			foreach (ListViewItem item in Items)
			{
				var tasklist = (item.Tag as TasklistItem);

				if (tasklist.Id == id)
					return item;
			}

			// else
			return null;
		}

		TasklistInfo GetTaskListInfo(TasklistItem item)
		{
			return new TasklistInfo()
			{
				Key = item.Id,
				Name = item.Name
			};
		}

		public TasklistInfo SelectedTasklist
		{
			get
			{
				if (SelectedItems.Count == 0)
					return null;

				return GetTaskListInfo(SelectedItems[0].Tag as TasklistItem);
			}

			set
			{
				SelectedIndices.Clear();

				if (value != null)
				{
					var item = FindTasklistItem(value.Key);

					if (item != null)
					{
						SelectedIndices.Add(item.Index);
						FocusedItem = item;
					}
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////

}
