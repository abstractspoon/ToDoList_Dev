

using System;
using System.Collections.Generic;
using System.Text;

namespace Calendar
{
    public class NewAppointmentEventArgs : EventArgs
    {
        public NewAppointmentEventArgs( string title, DateTime start, DateTime end )
        {
            m_Title = title;
            m_StartDate = start;
            m_EndDate = end;
        }

        private string m_Title;

        public string Title
        {
            get { return m_Title; }
        }

        private DateTime m_StartDate;

        public DateTime StartDate
        {
            get { return m_StartDate; }
        }

        private DateTime m_EndDate;

        public DateTime EndDate
        {
            get { return m_EndDate; }
        }
    }

    public delegate void NewAppointmentEventHandler( object sender, NewAppointmentEventArgs args );
}
