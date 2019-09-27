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
				if ((value == 1) || (value == 7) && (value != m_NumDays))
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
						Text = m_StartDate.ToString("MMMM yyyy");
					}
					else
					{
						Text = String.Format("{0} - {1}",
											m_StartDate.ToString("MMMM"),
											endDate.ToString("MMMM yyyy"));
					}
				}
				else
				{
					Text = String.Format("{0} - {1}",
										m_StartDate.ToString("MMMM yyyy"),
										endDate.ToString("MMMM yyyy"),
										m_StartDate.Year);
				}

                String weekNum = String.Format(m_Trans.Translate("(Week {0})"), DateUtil.WeekOfYear(m_StartDate));
                Text = (Text + ' ' + weekNum);

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
