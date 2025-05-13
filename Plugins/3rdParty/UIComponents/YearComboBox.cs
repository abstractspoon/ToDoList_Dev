using System;
using System.Collections.Generic;
using System.Linq;
using System.Globalization;

namespace UIComponents
{
	public class YearComboBox : System.Windows.Forms.ComboBox
	{
		public YearComboBox()
		{
			InitializeComponent();
		}

		private void InitializeComponent()
		{
			DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;

			BuildCombo();

			SelectedYear = DateTime.Now.Year;
		}

		public int SelectedYear
		{
			get { return (SelectedIndex + m_MinYear); }
			set 
			{
				if (value == SelectedYear)
					return;
				
				if (value < m_MinYear)
				{
					m_MinYear = ((value / 10) * 10);
					BuildCombo();
				}
				else if (value > m_MaxYear)
				{
					m_MaxYear = (((value / 10) + 1) * 10);
					BuildCombo();
				}

				SelectedIndex = (value - m_MinYear);
			}
		}

		protected virtual void BuildCombo()
		{
			Items.Clear();

			for (int year = m_MinYear; year <= m_MaxYear; year++)
			{
				var date = new DateTime(year, 1, 1);
				Items.Add(date.ToString("yyyy", CultureInfo.CurrentCulture));
			}
		}

		private int m_MinYear = 1980, m_MaxYear = 2050;
	}
}
