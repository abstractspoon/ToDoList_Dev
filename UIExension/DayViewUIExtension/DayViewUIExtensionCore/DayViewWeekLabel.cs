using System;
using System.Collections.Generic;
using System.Text;

namespace DayViewUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class DayViewWeekLabel : System.Windows.Forms.Label
	{
		public DayViewWeekLabel()
		{
		}

		private DateTime m_StartDate;

		public DateTime StartDate
		{
			set
			{
				m_StartDate = value;

				DateTime endDate = m_StartDate.AddDays(7);

				if (endDate.Year == m_StartDate.Year)
				{
					if (endDate.Month == m_StartDate.Month)
						Text = m_StartDate.ToString("MMMM yyyy");
					else
						Text = String.Format("{0} - {1}",
														m_StartDate.ToString("MMMM"),
														endDate.ToString("MMMM yyyy"));
				}
				else
				{
					Text = String.Format("{0} - {1}",
													m_StartDate.ToString("MMMM yyyy"),
													endDate.ToString("MMMM yyyy"),
													m_StartDate.Year);
				}

				Invalidate();
			}
		}
	}
}
