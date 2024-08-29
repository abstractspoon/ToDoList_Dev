using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	class DayViewTaskComboBox : ComboBox
	{
		private UIExtension.TaskIcon m_TaskIcons;

		// ------------------------------------------------------

		public DayViewTaskComboBox()
		{
			DrawMode = DrawMode.OwnerDrawFixed;
		}

		public void Initialise(IEnumerable<TaskItem> taskItems, UIExtension.TaskIcon taskIcons, uint taskId)
		{
			m_TaskIcons = taskIcons;

			// Populate combo
			var sortedList = taskItems.OrderBy(x => x.Position);

			foreach (var task in taskItems)
			{
				Items.Add(task);

				if (task.Id == taskId)
					SelectedItem = task;
			}
		}

		public uint SelectedTaskId
		{
			get
			{
				var selItem = (SelectedItem as TaskItem);
				return ((selItem == null) ? 0 : selItem.Id);
			}
		}

		public Calendar.AppointmentDates SelectedTaskDates
		{
			get
			{
				var selItem = (SelectedItem as TaskItem);
				return ((selItem == null) ? null : selItem.Dates);
			}
		}

		protected override void OnMeasureItem(MeasureItemEventArgs e)
		{
			base.OnMeasureItem(e);

			e.ItemHeight = UIExtension.TaskIcon.IconSize;
		}

		protected override void OnDrawItem(DrawItemEventArgs e)
		{
			if (e.Index < 0)
				return;

			var taskItem = (Items[e.Index] as TaskItem);

			if (taskItem != null)
			{
				e.DrawBackground();

				var rect = e.Bounds;
				bool listItem = !e.State.HasFlag(DrawItemState.ComboBoxEdit);

				if (listItem)
				{
					for (int i = 0; i < taskItem.Depth; i++)
						rect.X += UIExtension.TaskIcon.IconSize;
				}

				if (taskItem.HasIcon && m_TaskIcons.Get(taskItem.Id))
				{
					m_TaskIcons.Draw(e.Graphics, rect.X, rect.Y);
					rect.X += UIExtension.TaskIcon.IconSize;
				}
				else if (listItem)
				{
					rect.X += UIExtension.TaskIcon.IconSize;
				}

				using (var brush = new SolidBrush(e.ForeColor))
					e.Graphics.DrawString(taskItem.Title, Font, brush, rect);

				e.DrawFocusRectangle();
			}
		}
	}
}
