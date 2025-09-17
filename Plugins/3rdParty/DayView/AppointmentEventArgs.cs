

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
        public MoveAppointmentEventArgs(Appointment appointment, SelectionTool.Mode mode, SelectionTool.State state) : base(appointment)
        {
            m_Mode = mode;
			m_State = state;
		}

		private SelectionTool.Mode m_Mode;
		private SelectionTool.State m_State;

		public SelectionTool.Mode Mode { get { return m_Mode; } }
		public SelectionTool.State State { get { return m_State; } }

		public bool Started { get { return (m_State == SelectionTool.State.Started); } }
		public bool Moved { get { return (m_State == SelectionTool.State.Moved); } }
		public bool Finished { get { return (m_State == SelectionTool.State.Finished); } }
	}

	public delegate void AppointmentEventHandler(object sender, AppointmentEventArgs args);
}
