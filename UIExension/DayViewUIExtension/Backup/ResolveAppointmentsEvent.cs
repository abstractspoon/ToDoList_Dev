

using System;
using System.Collections.Generic;
using System.Text;

namespace Calendar
{
    public class ResolveAppointmentsEventArgs : EventArgs
    {
        public ResolveAppointmentsEventArgs(DateTime start, DateTime end)
        {
            m_StartDate = start;
            m_EndDate = end;
            m_Appointments = new List<Appointment>();
        }

        private DateTime m_StartDate;

        public DateTime StartDate
        {
            get { return m_StartDate; }
            set { m_StartDate = value; }
        }

        private DateTime m_EndDate;

        public DateTime EndDate
        {
            get { return m_EndDate; }
            set { m_EndDate = value; }
        }

        private List<Appointment> m_Appointments;

        public List<Appointment> Appointments
        {
            get { return m_Appointments; }
            set { m_Appointments = value; }
        }
    }

    public delegate void ResolveAppointmentsEventHandler(object sender, ResolveAppointmentsEventArgs args);
}
