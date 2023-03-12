

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Calendar
{
    public class DrawTool : ITool
    {
        Tuple<DateTime, DateTime> m_SelectionStart = null;
        bool m_SelectionStarted;

        public void Reset()
        {
            m_SelectionStarted = false;
        }

        public void MouseMove(MouseEventArgs e)
        {
            if (e == null)
                throw new ArgumentNullException("e");

            if (e.Button == MouseButtons.Left)
            {
                if (m_SelectionStarted)
                {
                    DateTime time = m_DayView.GetDateTimeAt(e.X, e.Y, false); // not a long appointment

                    if (time < m_SelectionStart.Item1)
                    {
                        m_DayView.SelectedDates.Start = time;
                        m_DayView.SelectedDates.End = m_SelectionStart.Item2;
                    }
                    else
                    {
						m_DayView.SelectedDates.Start = m_SelectionStart.Item1;
						m_DayView.SelectedDates.End = time;
					}

					m_DayView.Invalidate();
                }
            }
        }

        public void MouseUp(MouseEventArgs e)
        {
            if (e == null)
                throw new ArgumentNullException("e");

            if (e.Button == MouseButtons.Left)
            {
                m_DayView.Capture = false;
                m_SelectionStarted = false;

                m_DayView.RaiseSelectionChanged(new AppointmentEventArgs());

                if (Complete != null)
                    Complete(this, EventArgs.Empty);
            }
        }

        public void MouseDown(MouseEventArgs e)
        {
            if (e == null)
                throw new ArgumentNullException("e");

            if (e.Button == MouseButtons.Left)
            {
                DateTime time = m_DayView.GetDateTimeAt(e.X, e.Y);

				m_SelectionStart = new Tuple<DateTime, DateTime>(time, time.AddMinutes(60 / m_DayView.SlotsPerHour));

                m_DayView.SelectedDates.Start = m_SelectionStart.Item1;
                m_DayView.SelectedDates.End = m_SelectionStart.Item2;

                m_SelectionStarted = true;

                m_DayView.Invalidate();
                m_DayView.Capture = true;
            }
        }

        private DayView m_DayView;

        public DayView DayView
        {
            get { return m_DayView; }
            set { m_DayView = value; }
        }

        public event EventHandler Complete;
    }
}
