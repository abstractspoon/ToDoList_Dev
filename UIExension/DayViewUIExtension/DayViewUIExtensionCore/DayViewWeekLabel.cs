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
			m_NumWeeks = 1;
		}

		private DateTime m_StartDate;
		private int m_NumWeeks;

		public int NumWeeks
		{
			set
			{
				if ((value >= 1) && (value <= 3) && (value != m_NumWeeks))
				{
					m_NumWeeks = value;
					StartDate = m_StartDate;
				}
			}
		}

		protected int NumDays
		{
			get { return (m_NumWeeks * 7); }
		}

		public DateTime StartDate
		{
			set
			{
				m_StartDate = value;

				DateTime endDate = m_StartDate.AddDays(this.NumDays);

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

        protected override void OnPaint(System.Windows.Forms.PaintEventArgs pe)
        {
            pe.Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.SystemDefault;
            base.OnPaint(pe);
        }
    }
}
