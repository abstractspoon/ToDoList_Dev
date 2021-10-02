

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
			get	{ return m_dayView;	}
			set	{ m_dayView = value; }
		}

		private Mode m_mode = Mode.None;

		public Boolean IsEditing
		{
			get { return (m_mode != Mode.None); }
		}

		public Mode EditMode
		{
			get { return m_mode; }
		}

		protected DateTime m_startDate;
		protected TimeSpan m_length;
		protected TimeSpan m_delta;
		protected Point m_lastMouseMove;
		protected bool m_longAppointment;

		virtual public bool IsEditingLongAppt
		{
			get { return (IsEditing && m_longAppointment); }
		}

		public void Reset()
		{
			m_mode = Mode.None;
			m_length = TimeSpan.Zero;
			m_delta = TimeSpan.Zero;
		}

		public bool UpdateCursor(System.Windows.Forms.MouseEventArgs e)
		{
			m_dayView.Cursor = System.Windows.Forms.Cursors.Default;

			if (!CanModifyAppointment(e, false))
				return false;

			Mode mode = GetMode(e.Location, m_dayView.SelectedAppointment);

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
				m_dayView.Cursor = System.Windows.Forms.Cursors.Default;
				break;

			default:
				return false;
			}

			return true;
		}

		protected bool CanModifyAppointment(System.Windows.Forms.MouseEventArgs e, bool checkMouseDown)
		{
			if (e == null)
				return false;

			if (checkMouseDown && (e.Button != System.Windows.Forms.MouseButtons.Left))
				return false;

			Appointment selection = m_dayView.SelectedAppointment;

			if ((selection == null) || selection.Locked || (m_mode == Mode.None))
				return false;

			return true;
		}

		virtual public void MouseMove(System.Windows.Forms.MouseEventArgs e)
		{
			if (!UpdateCursor(e))
				return;

			if (IsEditing && (e.Button != System.Windows.Forms.MouseButtons.Left))
			{
				MouseUp(e);
				return;
			}

			bool resized = false;

			switch (m_mode)
			{
			case Mode.Move:
				resized = MoveAppointment(e);
				break;

			case Mode.ResizeBottom:
				resized = ResizeShortAppointment(e);
				break;

			case Mode.ResizeTop:
				resized = ResizeShortAppointment(e);
				break;

			case Mode.ResizeLeft:
				resized = ResizeLongAppointment(e);
				break;

			case Mode.ResizeRight:
				resized = ResizeLongAppointment(e);
				break;
			}

			m_lastMouseMove = e.Location;

			if (resized)
			{
				m_dayView.Invalidate();
				m_dayView.RaiseAppointmentMove(new MoveAppointmentEventArgs(m_dayView.SelectedAppointment, m_mode, false));
			}
		}

		virtual protected bool MoveAppointment(System.Windows.Forms.MouseEventArgs e)
		{
			Debug.Assert(CanModifyAppointment(e, true));

			if (m_mode != Mode.Move)
				return false;

			Rectangle shortApptsRect = m_dayView.GetTrueRectangle();
			Rectangle longApptsRect = m_dayView.GetFullDayApptsRectangle();

			if (!shortApptsRect.Contains(e.Location) && !longApptsRect.Contains(e.Location))
				return false;

			Appointment selection = m_dayView.SelectedAppointment;

			bool longAppt = IsEditingLongAppt;
			bool ptInLongApptsRect = longApptsRect.Contains(e.Location);

			bool shortAppt = !longAppt;
			bool ptInShortApptsRect = !ptInLongApptsRect;

			// Get date/time at mouse position
			DateTime dateAtCursor = m_dayView.GetDateTimeAt(e.X, e.Y, longAppt);

			if (shortAppt && ptInShortApptsRect)
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

						return true;
					}
				}
			}
			else if (longAppt && ptInLongApptsRect)
			{
				dateAtCursor = dateAtCursor.Add(m_delta);

				int hoursDiff = dateAtCursor.Subtract(selection.StartDate).Hours;
				TimeSpan apptLen = selection.Length;

				if (hoursDiff != 0)
				{
					System.DateTime newStart = selection.StartDate.AddHours(hoursDiff);

					if (newStart != selection.StartDate)
					{
						selection.StartDate = newStart;
						selection.EndDate = (newStart + apptLen);

						return true;
					}
				}
			}

			// all else
			return false;
		}

		virtual protected bool ResizeLongAppointment(System.Windows.Forms.MouseEventArgs e)
		{
			Debug.Assert(CanModifyAppointment(e, true));

			Appointment selection = m_dayView.SelectedAppointment;

			if (!IsEditingLongAppt)
				return false;

			// Note: we allow the cursor to move outside the long appt rect
			// because the date/time calculation will still be valid
			DateTime dateAtCursor = m_dayView.GetDateTimeAt(e.X, e.Y, true);

			switch (m_mode)
			{
			case Mode.ResizeLeft:
				{
					// Prevent resizing shorter than one hour
					if (dateAtCursor.Date < selection.EndDate.AddHours(-1))
					{
						selection.StartDate = dateAtCursor;
						return true;
					}
				}
				break;

			case Mode.ResizeRight:
				{
					// Prevent resizing shorter than one hour
					if (dateAtCursor >= selection.StartDate.AddHours(1))
					{
						selection.EndDate = dateAtCursor;
						return true;
					}
				}
				break;
			}

			// all else
			return false;
		}

		virtual protected bool ResizeShortAppointment(System.Windows.Forms.MouseEventArgs e)
		{
			Debug.Assert(CanModifyAppointment(e, true));

			Appointment selection = m_dayView.SelectedAppointment;

			if (IsEditingLongAppt)
				return false;

			Rectangle shortApptsRect = m_dayView.GetTrueRectangle();

			if (!shortApptsRect.Contains(e.Location))
				return false;

			switch (m_mode)
			{
			case Mode.ResizeBottom:
				{
					// Note: the current algorithm tends to 'floor' the time
					// which makes selecting all the way to midnight tricky.
					// We solve it by adding half the slot height to the 
					// mouse position
					DateTime dateAtCursor = m_dayView.GetDateTimeAt(e.X, (e.Y + m_dayView.SlotHeight), false);

					if (dateAtCursor == dateAtCursor.Date)
					{
						dateAtCursor = dateAtCursor.AddDays(1).AddSeconds(-1);
					}
					// If the cursor drifts outside the day column treat the
					// time component as still being valid
					else if (!SameDay(selection.StartDate, dateAtCursor))
					{
						dateAtCursor = selection.StartDate.Date + dateAtCursor.TimeOfDay;
					}

					// Prevent resizing shorter than one slot
					if (dateAtCursor > selection.StartDate)
					{
						if (SameDay(selection.EndDate, dateAtCursor))
						{
							selection.EndDate = dateAtCursor;
							return true;
						}
					}
				}
				break;

			case Mode.ResizeTop:
				{
					DateTime dateAtCursor = m_dayView.GetDateTimeAt(e.X, e.Y, false);

					// If the cursor drifts outside the day column treat the
					// time component as still being valid
					if (!SameDay(selection.StartDate, dateAtCursor))
						dateAtCursor = selection.StartDate.Date + dateAtCursor.TimeOfDay;

					// Prevent resizing shorter than one slot
					// and prevent the appointment start moving to the previous day
					if ((dateAtCursor < selection.EndDate) && SameDay(selection.StartDate, dateAtCursor))
					{
						selection.StartDate = dateAtCursor;
						return true;
					}
				}
				break;
			}

			// all else
			return false;
		}

		static protected bool SameDay(DateTime startDate, DateTime endDate)
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

		public Mode GetMode(System.Drawing.Point mousePos, Appointment appt)
		{
			if (m_mode != Mode.None)
				return m_mode;

			if ((appt == null) || appt.Locked)
				return Mode.None;

			Rectangle apptRect = Rectangle.Empty;

			if (m_dayView.GetAppointmentRect(appt, ref apptRect))
			{
				Rectangle topRect = apptRect;
				Rectangle bottomRect = apptRect;
				Rectangle leftRect = apptRect;
				Rectangle rightRect = apptRect;

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

				// Must fall within appt rect
				if (apptRect.Contains(mousePos))
				{
					return Mode.Move;
				}
			}

			// all else
			return Mode.None;
		}

		public virtual void MouseUp(System.Windows.Forms.MouseEventArgs e)
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

		public virtual void MouseDown(System.Windows.Forms.MouseEventArgs e)
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
				DateTime downPos = m_dayView.GetDateTimeAt(e.X, e.Y);
				m_delta = m_dayView.SelectedAppointment.StartDate - downPos;

				m_length = TimeSpan.Zero;
				m_lastMouseMove = e.Location;
				m_longAppointment = m_dayView.SelectedAppointment.IsLongAppt();
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
