

using System;
using System.Collections.Generic;
using System.Text;

namespace Calendar
{
    public class AppointmentEventArgs : EventArgs
    {
        public AppointmentEventArgs()
        {
            m_Appointment = null;
        }
        public AppointmentEventArgs(Appointment appointment)
        {
            m_Appointment = appointment;
        }

        private Appointment m_Appointment;

        public Appointment Appointment
        {
            get { return m_Appointment; }
        }

    }

    public class MoveAppointmentEventArgs : AppointmentEventArgs
    {
        public MoveAppointmentEventArgs(Appointment appointment, SelectionTool.Mode mode, bool finished) : base(appointment)
        {
            m_Mode = mode;
			m_Finished = finished;
        }

        private SelectionTool.Mode m_Mode;
        public SelectionTool.Mode Mode
        { 
            get { return m_Mode; }
        }

		private bool m_Finished;
		public bool Finished
		{
			get { return m_Finished; }
		}
    }

    public delegate void AppointmentEventHandler(object sender, AppointmentEventArgs args);
}
