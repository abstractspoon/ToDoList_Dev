

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Diagnostics;

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

        private Mode m_mode = Mode.None;

		public Boolean IsResizing
		{
			get { return (m_mode != Mode.None); }
		}

		public Mode ResizingMode
		{
			get { return m_mode; }
		}

		public Boolean IsResizingLongAppt()
		{
			switch (m_mode)
			{
				case Mode.ResizeLeft:
				case Mode.ResizeRight:
					return true;

				case Mode.Move:
					return m_dayView.SelectedAppointment.IsLongAppt();
			}

			// all else
			return false;
		}

        private DateTime m_startDate;
        private TimeSpan m_length;
        private TimeSpan m_delta;
        private Point m_lastMouseMove;

        public void Reset()
        {
			m_mode = Mode.None;
			m_length = TimeSpan.Zero;
            m_delta = TimeSpan.Zero;
        }

        public void UpdateCursor(System.Windows.Forms.MouseEventArgs e, Appointment appointment)
        {
            if (e == null)
                throw new ArgumentNullException("e");

            m_dayView.Cursor = System.Windows.Forms.Cursors.Default;

			if ((appointment != null) && !appointment.Locked)
			{
				Mode mode = GetMode(e.Location, appointment);

				switch (mode)
				{
					case Mode.ResizeBottom:
					case Mode.ResizeTop:
						m_dayView.Cursor = System.Windows.Forms.Cursors.SizeNS;
						break;

					case Mode.ResizeLeft:
					case Mode.ResizeRight:
						DayView.Cursor = System.Windows.Forms.Cursors.SizeWE;
						break;

					case Mode.Move:
					default:
						m_dayView.Cursor = System.Windows.Forms.Cursors.Default;
						break;
                }
            }
        }

        public void MouseMove(System.Windows.Forms.MouseEventArgs e)
        {
            if (e == null)
                throw new ArgumentNullException("e");

            Appointment selection = m_dayView.SelectedAppointment;

            UpdateCursor(e, selection);

            if ((selection == null) || selection.Locked || (m_mode == Mode.None))
                return;

            Rectangle viewrect = m_dayView.GetTrueRectangle();
            Rectangle fdrect = m_dayView.GetFullDayApptsRectangle();

            if ((e.Button == System.Windows.Forms.MouseButtons.Left) &&
                 (viewrect.Contains(e.Location) || fdrect.Contains(e.Location)))
            {
                // Get time at mouse position
                bool longAppt = IsResizingLongAppt();
                bool ptInLongAptRect = fdrect.Contains(e.Location);

                DateTime dateAtCursor = m_dayView.GetDateTimeAt(e.X, e.Y, longAppt);

                switch (m_mode)
                {
                    case Mode.Move:
                        if (!selection.IsLongAppt() && !ptInLongAptRect)
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
                                DateTime date = m_dayView.GetDateAt(e.X, false);
                                DateTime datePrev = m_dayView.GetDateAt(m_lastMouseMove.X, false);

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
                        else if (selection.IsLongAppt() && ptInLongAptRect)
                        {
                            dateAtCursor = dateAtCursor.Add(m_delta);

                            int hoursDiff = dateAtCursor.Subtract(selection.StartDate).Hours;
                            TimeSpan apptLen = selection.Length;

                            if (hoursDiff != 0)
                            {
                                System.DateTime newStart = selection.StartDate.AddHours(hoursDiff);

                                if (newStart < m_dayView.StartDate)
                                {
                                    newStart = m_dayView.StartDate;
                                }
                                else if ((newStart + apptLen) >= m_dayView.EndDate)
                                {
                                    newStart = (m_dayView.EndDate - apptLen);
                                }

                                if (newStart != selection.StartDate)
                                {
                                    selection.StartDate = newStart;
                                    selection.EndDate = (newStart + apptLen);

                                    m_dayView.Invalidate();
                                    m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(selection, m_mode, false));
                                }
                            }
                        }
                        break;

                    case Mode.ResizeBottom:
                        if (!ptInLongAptRect)
                        {
							// Note: the current algorithm tends to 'floor' the time
							// which makes selecting all the way to midnight tricky.
							// We solve it by adding half the slot height to the 
							// mouse position
							dateAtCursor = m_dayView.GetDateTimeAt(e.X, e.Y + m_dayView.SlotHeight, longAppt);

							if (dateAtCursor == dateAtCursor.Date)
								dateAtCursor = dateAtCursor.AddDays(1).AddSeconds(-1);

							if (dateAtCursor > selection.StartDate)
							{
								if (SameDay(selection.EndDate, dateAtCursor.Date))
								{
									selection.EndDate = dateAtCursor;

									m_dayView.Invalidate();
									m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(selection, m_mode, false));
								}
							}
						}
                        break;

                    case Mode.ResizeTop:
                        if (!ptInLongAptRect && (dateAtCursor < selection.EndDate))
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
                        if (ptInLongAptRect && (dateAtCursor.Date < selection.EndDate.AddHours(-1)))
                        {
                            selection.StartDate = dateAtCursor;

                            m_dayView.Invalidate();
                            m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(selection, m_mode, false));
                        }
                        break;

                    case Mode.ResizeRight:
                        if (ptInLongAptRect && (dateAtCursor >= selection.StartDate.AddHours(1)))
                        {
                            selection.EndDate = dateAtCursor;

                            m_dayView.Invalidate();
                            m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(selection, m_mode, false));
                        }
                        break;
                }

                m_lastMouseMove = e.Location;
			}
        }

        static private bool SameDay(DateTime startDate, DateTime endDate)
        {
            if (endDate.Date == startDate.Date)
                return true;

            if (endDate == startDate.Date.AddDays(1)) // midnight
                return true;

            // all else
            return false;
        }

        public Mode GetMode(System.Drawing.Point mousePos)
        {
            return GetMode(mousePos, m_dayView.SelectedAppointment);
        }

        public Mode GetMode(System.Drawing.Point mousePos, Appointment appointment)
        {
			if (m_mode != Mode.None)
				return m_mode;

            DayView.AppointmentView view = null;
            Boolean gotview = false;

            if (appointment == null)
                return Mode.None;

            if (m_dayView.appointmentViews.ContainsKey(appointment))
            {
                view = m_dayView.appointmentViews[appointment];
                gotview = true;
            }
            else if (m_dayView.longAppointmentViews.ContainsKey(appointment))
            {
                view = m_dayView.longAppointmentViews[appointment];
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

                if (m_dayView.GetFullDayApptsRectangle().Contains(mousePos))
				{
					if (rightRect.Contains(mousePos))
					{
						return Mode.ResizeRight;
					}
					else if (leftRect.Contains(mousePos))
					{
						return Mode.ResizeLeft;
					}
				}
				else
				{
					if (topRect.Contains(mousePos))
					{
						return Mode.ResizeTop;
					}
					else if (bottomRect.Contains(mousePos))
					{
						return Mode.ResizeBottom;
					}
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

			m_dayView.Invalidate();
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
			else
			{
				m_dayView.RaiseSelectionChanged(new AppointmentEventArgs(m_dayView.SelectedAppointment));
			}

            if (m_dayView.CurrentlyEditing)
                m_dayView.FinishEditing(false);

            m_mode = GetMode(e.Location);

            if (m_mode != Mode.None)
            {
                // Calculate delta time between selection and clicked point
                if (m_dayView.SelectedAppointment != null)
                {
                    DateTime downPos = m_dayView.GetDateTimeAt(e.X, e.Y);
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
