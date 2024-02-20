using System;
using System.Collections.Generic;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class DayViewWeekLabel : System.Windows.Forms.Label
	{
		public DayViewWeekLabel(Translator trans)
		{
			m_Trans = trans;
			m_NumDays = 1;
		}

		private DateTime m_StartDate;
		private int m_NumDays;
		private Translator m_Trans;

		public int NumDays
		{
			get { return m_NumDays; }

			set
			{
				if (value != m_NumDays)
				{
					m_NumDays = value;
					StartDate = m_StartDate;
				}
			}
		}

		public DateTime StartDate
		{
			set
			{
				m_StartDate = value;
				DateTime endDate = m_StartDate.AddDays(this.NumDays).AddSeconds(-1);
                
				if (endDate.Year == m_StartDate.Year)
				{
					if (endDate.Month == m_StartDate.Month)
					{
						Text = m_StartDate.ToString("MMM yyyy ");
					}
					else
					{
						Text = String.Format("{0} - {1} ",
											m_StartDate.ToString("MMM"),
											endDate.ToString("MMM yyyy"));
					}
				}
				else
				{
					Text = String.Format("{0} - {1} ",
										m_StartDate.ToString("MMM yyyy"),
										endDate.ToString("MMM yyyy"),
										m_StartDate.Year);
				}

                int startWeek = DateUtil.WeekOfYear(m_StartDate);
                int numWeeks = (NumDays / 7);

                if (numWeeks <= 1)
                    Text = (Text + String.Format(m_Trans.Translate("(Week {0})"), startWeek));
                else
                    Text = (Text + String.Format(m_Trans.Translate("(Weeks {0}-{1})"), startWeek, (startWeek + numWeeks - 1)));

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
