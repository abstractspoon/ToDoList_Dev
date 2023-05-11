using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DayViewUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class DayViewMonthComboBox : System.Windows.Forms.ComboBox
	{
		public DayViewMonthComboBox()
		{
			InitializeComponent();
		}

		private void InitializeComponent()
		{
			DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;

			for (int month = 1; month <= 12; month++)
			{
				DateTime date = new DateTime(2000, month, 1);
				Items.Add(date.ToString("MMMM"));
			}

			SelectedMonth = DateTime.Now.Month;
		}

		public int SelectedMonth
		{
			get { return (SelectedIndex + 1); }
			set 
			{
				if (value == SelectedMonth)
					return;

				if ((value > 0) && (value <= 12))
				{
					SelectedIndex = (value - 1); 
				}
			}
		}
	}
}
