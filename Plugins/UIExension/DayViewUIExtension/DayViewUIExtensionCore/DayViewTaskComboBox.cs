using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	class DayViewTaskComboBox : ComboBox
	{
		private class ComboItem
		{
			string Title;
			bool HasIcon = false;
			int Level = 0;
		}

		// ------------------------------------------------------

		private UIExtension.TaskIcon m_TaskIcons;

		// ------------------------------------------------------

		public DayViewTaskComboBox()
		{
			DrawMode = DrawMode.OwnerDrawFixed;
		}

		public void Initialise(IEnumerable<TaskItem> taskItems, UIExtension.TaskIcon taskIcons)
		{
			m_TaskIcons = taskIcons;

			// Populate combo
			var sortedList = taskItems.OrderBy(x => x.Position);



		}

	}
}
