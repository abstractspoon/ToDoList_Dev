using System;
using System.Collections.Generic;
using System.Text;

namespace Calendar
{
	public class WeekChangeEventArgs : EventArgs
	{
        public WeekChangeEventArgs(DateTime start, int daysShowing)
        {
            StartDate = start.Date;
			EndDate = StartDate.AddDays(daysShowing);
        }

        public DateTime StartDate { private set; get; }
		public DateTime EndDate { private set; get; }
	}

	public delegate void WeekChangeEventHandler(object sender, WeekChangeEventArgs args);
}
