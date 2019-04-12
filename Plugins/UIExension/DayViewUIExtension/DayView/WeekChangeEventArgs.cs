using System;
using System.Collections.Generic;
using System.Text;

namespace Calendar
{
	public class WeekChangeEventArgs : EventArgs
	{
        public WeekChangeEventArgs(DateTime start)
        {
            m_StartDate = start;
        }

        private DateTime m_StartDate;

        public DateTime StartDate
        {
            get { return m_StartDate; }
        }

        public DateTime EndDate
        {
            get { return m_StartDate.AddDays(7); }
        }
	}

	public delegate void WeekChangeEventHandler(object sender, WeekChangeEventArgs args);
}
