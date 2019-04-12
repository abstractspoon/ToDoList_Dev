

using System;
using System.Collections.Generic;
using System.Text;

namespace Calendar
{
    public class AppointmentEventArgs : EventArgs
    {
        public AppointmentEventArgs( Appointment appointment )
        {
            m_Appointment = appointment;
        }

        private Appointment m_Appointment;

        public Appointment Appointment
        {
            get { return m_Appointment; }
        }

    }
}
