

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Calendar
{
    public class SelectionTool : ITool
    {
        private DayView m_dayView;

        public DayView DayView
        {
            get
            {
                return m_dayView;
            }
            set
            {
                m_dayView = value;
            }
        }

        private Mode m_mode;

        public Mode CurrentMode
        {
            get { return m_mode; }
        }

        private DateTime m_startDate;
        private TimeSpan m_length;
        private TimeSpan m_delta;
        private Point m_lastMouseMove;

        public void Reset()
        {
            m_length = TimeSpan.Zero;
            m_delta = TimeSpan.Zero;
        }

        public void MouseMove(System.Windows.Forms.MouseEventArgs e)
        {
            if (e == null)
                throw new ArgumentNullException("e");

            Appointment selection = m_dayView.SelectedAppointment;
            Rectangle viewrect = m_dayView.GetTrueRectangle();
            Rectangle fdrect = m_dayView.GetFullDayApptsRectangle();

            if (viewrect.Contains(e.Location) || fdrect.Contains(e.Location))
            {
                if ((selection != null) && (!selection.Locked))
                {
                    switch (e.Button)
                    {
                        case System.Windows.Forms.MouseButtons.Left:

                            // Get time at mouse position
                            DateTime dateAtCursor = m_dayView.GetDateTimeAt(e.X, e.Y);

                            switch (m_mode)
                            {
                                case Mode.Move:

                                    // This works for regular (i.e. non full-day or multi-day appointments)

                                    if (!selection.AllDayEvent && viewrect.Contains(e.Location))
                                    {
                                        if (m_length == TimeSpan.Zero)
                                        {
                                            m_startDate = selection.StartDate;
                                            m_length = selection.Length;
                                        }
                                        else
                                        {
                                            DateTime startDate = dateAtCursor.Add(m_delta);
                                            DateTime endDate = startDate.Add(m_length);

                                            if (startDate.Date < dateAtCursor.Date)
                                            {
                                                // User has dragged off the top
                                                startDate = dateAtCursor.Date;
                                                endDate = startDate.Add(m_length);
                                            }
                                            else if (endDate > dateAtCursor.Date.AddDays(1))
                                            {
                                                // User has dragged off the bottom
                                                startDate = endDate.Date.Subtract(m_length);
                                                endDate = (startDate + m_length);
                                            }

                                            // Handle horizontal drag
                                            DateTime date = m_dayView.GetDateAt(e.X);
                                            DateTime datePrev = m_dayView.GetDateAt(m_lastMouseMove.X);

                                            if (date != datePrev)
                                            {
                                                startDate = date.Date.Add(startDate.TimeOfDay);
                                                endDate = startDate.Add(m_length);
                                            }

                                            // Check for a change
                                            if (startDate != selection.StartDate)
                                            {
                                                selection.StartDate = startDate;
                                                selection.EndDate = endDate;

                                                m_dayView.Invalidate();
                                                m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(selection, m_mode, false));
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (fdrect.Contains(e.Location))
                                        {
                                            dateAtCursor = dateAtCursor.Add(m_delta);

                                            int dateDiff = dateAtCursor.Subtract(selection.StartDate).Days;

                                            if (dateDiff != 0)
                                            {
                                                if (selection.StartDate.AddDays(dateDiff) > m_dayView.StartDate)
                                                {
                                                    selection.StartDate = selection.StartDate.AddDays(dateDiff);
                                                    selection.EndDate = selection.EndDate.AddDays(dateDiff);
                                                    m_dayView.Invalidate();
													m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(selection, m_mode, false));
                                                }
                                            }
                                        }
                                    }

                                    break;

                                case Mode.ResizeBottom:

                                    if (dateAtCursor > selection.StartDate)
                                    {
                                        if (SameDay(selection.EndDate, dateAtCursor.Date))
                                        {
                                            selection.EndDate = dateAtCursor;
                                            m_dayView.Invalidate();
											m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(selection, m_mode, false));
                                        }
                                        else
                                        {
                                            int breakpoint = 0;
                                        }
                                    }

                                    break;

                                case Mode.ResizeTop:

                                    if (dateAtCursor < selection.EndDate)
                                    {
                                        if (selection.StartDate.Day == dateAtCursor.Day)
                                        {
                                            selection.StartDate = dateAtCursor;
                                            m_dayView.Invalidate();
											m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(selection, m_mode, false));
                                        }
                                    }
                                    break;

                                case Mode.ResizeLeft:
                                    if (dateAtCursor.Date < selection.EndDate.Date)
                                    {
                                        selection.StartDate = dateAtCursor.Date;
                                        m_dayView.Invalidate();
										m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(selection, m_mode, false));
                                    }
                                    break;

                                case Mode.ResizeRight:
                                    if (dateAtCursor.Date >= selection.StartDate.Date)
                                    {
                                        selection.EndDate = dateAtCursor.Date.AddDays(1);
                                        m_dayView.Invalidate();
										m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(selection, m_mode, false));
                                    }
                                    break;
                            }

                            break;

                        default:

                            Mode tmpNode = GetMode(e);

                            switch (tmpNode)
                            {
                                case Mode.Move:
                                    m_dayView.Cursor = System.Windows.Forms.Cursors.Default;
                                    break;
                                case Mode.ResizeBottom:
                                case Mode.ResizeTop:
                                    if (!selection.AllDayEvent)
                                        m_dayView.Cursor = System.Windows.Forms.Cursors.SizeNS;
                                    break;
                                case Mode.ResizeLeft: // changed by Gimlei
                                case Mode.ResizeRight:
                                    if (selection.AllDayEvent)
                                        DayView.Cursor = System.Windows.Forms.Cursors.SizeWE;
                                    break;
                            }

                            break;
                    }

                    m_lastMouseMove = e.Location;
                }
            }
        }

        static private bool SameDay(DateTime startDate, DateTime endDate)
        {
            if (endDate.Date == startDate.Date)
                return true;

            if ((endDate.Date == startDate.Date.AddDays(1)) && (endDate == endDate.Date))
                return true;

            // all else
            return false;
        }

        private Mode GetMode(System.Windows.Forms.MouseEventArgs e)
        {
            DayView.AppointmentView view = null;
            Boolean gotview = false;

            if (m_dayView.SelectedAppointment == null)
                return Mode.None;

            if (m_dayView.appointmentViews.ContainsKey(m_dayView.SelectedAppointment))
            {
                view = m_dayView.appointmentViews[m_dayView.SelectedAppointment];
                gotview = true;
            }

            else if (m_dayView.longappointmentViews.ContainsKey(m_dayView.SelectedAppointment))
            {
                view = m_dayView.longappointmentViews[m_dayView.SelectedAppointment];
                gotview = true;
            }

            if (gotview)
            {
                Rectangle topRect = view.Rectangle;
                Rectangle bottomRect = view.Rectangle;
                Rectangle leftRect = view.Rectangle;
                Rectangle rightRect = view.Rectangle;

                bottomRect.Y = bottomRect.Bottom - 5;
                bottomRect.Height = 5;
                topRect.Height = 5;
                leftRect.Width = 5;
                rightRect.X += rightRect.Width - 5;
                rightRect.Width = 5;

                if (m_dayView.AllowMode(Mode.ResizeTop) && topRect.Contains(e.Location))
                {
                    return Mode.ResizeTop;
                }
                else if (m_dayView.AllowMode(Mode.ResizeBottom) && bottomRect.Contains(e.Location))
                {
                    return Mode.ResizeBottom;
                }
                else if (m_dayView.AllowMode(Mode.ResizeRight) && rightRect.Contains(e.Location))
                {
                    return Mode.ResizeRight;
                }
                else if (m_dayView.AllowMode(Mode.ResizeLeft) && leftRect.Contains(e.Location))
                {
                    return Mode.ResizeLeft;
                }

                // all else
                return Mode.Move;
            }

            return Mode.None;
        }

        public void MouseUp(System.Windows.Forms.MouseEventArgs e)
        {
            if (e == null)
                throw new ArgumentNullException("e");

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                if (Complete != null)
                    Complete(this, EventArgs.Empty);
            }

			m_dayView.RaiseSelectionChanged(new AppointmentEventArgs(m_dayView.SelectedAppointment));
			m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(m_dayView.SelectedAppointment, m_mode, true));

            m_mode = Mode.None;
            m_delta = TimeSpan.Zero;
        }

        public void MouseDown(System.Windows.Forms.MouseEventArgs e)
        {
            if (m_dayView.SelectedAppointmentIsNew)
            {
                m_dayView.RaiseNewAppointment();
            }

            if (m_dayView.CurrentlyEditing)
                m_dayView.FinishEditing(false);

            m_mode = GetMode(e);

            if (m_mode != Mode.None)
            {
                if (m_dayView.SelectedAppointment != null)
                {
                    DateTime downPos = m_dayView.GetDateTimeAt(e.X, e.Y);
                    // Calculate delta time between selection and clicked point
                    m_delta = m_dayView.SelectedAppointment.StartDate - downPos;
                }
                else
                {
                    m_delta = TimeSpan.Zero;
                }

                m_length = TimeSpan.Zero;
                m_lastMouseMove = e.Location;
            }
        }

        public event EventHandler Complete;

        public enum Mode
        {
            ResizeTop,
            ResizeBottom,
            ResizeLeft,
            ResizeRight,
            Move,
            None
        }
    }
}
