using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	public class TDLSelectionTool : Calendar.SelectionTool
	{
		private bool m_Transitioned = false;

		override public bool IsEditingLongAppt
		{
			get
			{
				switch (EditMode)
				{
				case Mode.None:			return false;

				case Mode.ResizeBottom: return false;
				case Mode.ResizeTop:	return false;

				case Mode.ResizeLeft:	return true;
				case Mode.ResizeRight:	return true;
				}

				// Task move 
				Debug.Assert(EditMode == Mode.Move);

				bool longAppt = base.IsEditingLongAppt;

				return (m_Transitioned ? !longAppt : longAppt);
			}
		}

		public override void MouseUp(System.Windows.Forms.MouseEventArgs e)
		{
			m_Transitioned = false;

			base.MouseUp(e);
		}

		override protected bool MoveAppointment(System.Windows.Forms.MouseEventArgs e)
		{
			// Default implementation handles move WITHIN the respective
			// 'long/short appointments' regions 
			if (base.MoveAppointment(e))
				return true;

			// whilst we handle transitions BETWEEN the regions

			// Note: we need to duplicate some of the base checks
			// because we don't know what failed
			if (EditMode != Mode.Move)
				return false;

			Rectangle shortApptsRect = DayView.GetTrueRectangle();
			Rectangle longApptsRect = DayView.GetFullDayApptsRectangle();

            // Make sure the long appointments rect has a reasonable height
            if (longApptsRect.Height < DayView.LongAppointmentHeight)
            {
                int adjustment = (DayView.LongAppointmentHeight - longApptsRect.Height);

                longApptsRect.Y -= adjustment;
                longApptsRect.Height += adjustment;
            }

			if (!shortApptsRect.Contains(e.Location) && !longApptsRect.Contains(e.Location))
				return false;

			//              _  _________________________________________
			//                |   ____________                          |
			//                |  |            |                         |
			// longApptsRect  |  |   long     |               ^         | 
			//                |  |____________|               |         | 
			//              _ |---------|---------------------|---------| 
			//                |         |                     |         |
			//                |         V                   __|__       |
			//                |                            |     |      |
			//                |                            |     |      |
			//                |                            |     |      |
			// shortApptsRect |                            |short|      |
			//                |                            |     |      |
			//                |                            |     |      |
			//                |                            |     |      |
			//                |                            |_____|      |
			//                |                                         |
			//              _ |_________________________________________|

			CalendarItem selection = (DayView.SelectedAppointment as CalendarItem);

			bool longAppt = selection.IsLongAppt();
			bool shortAppt = !longAppt;

			bool curPtInLongApptsRect = longApptsRect.Contains(e.Location);
			bool prevPtInLongApptsRect = longApptsRect.Contains(m_lastMouseMove);

			bool curPtInShortApptsRect = shortApptsRect.Contains(e.Location);
			bool prevPtInShortApptsRect = shortApptsRect.Contains(m_lastMouseMove);

			if (shortAppt && prevPtInShortApptsRect && curPtInLongApptsRect)
			{
				// Short appointment dragged into long appointment rect
				m_Transitioned = !base.IsEditingLongAppt;

				// If the appointment was originally a long appointment we
				// restore the original length else create a day-long task
				DateTime dateAtCursor = DayView.GetDateTimeAt(e.X, e.Y, true);
                selection.StartDate = dateAtCursor.Date;

				if (base.IsEditingLongAppt)
				{
                    selection.EndDate = selection.StartDate.Add(selection.OriginalLength);
				}
				else
				{
					selection.EndDate = selection.StartDate.AddDays(1).AddSeconds(-1);
				}

				m_delta = TimeSpan.Zero;
				m_length = selection.Length;

				return true;
			}
			else if (longAppt && prevPtInLongApptsRect && curPtInShortApptsRect)
			{
				// Long appointment dragged into short appointment rect
				m_Transitioned = base.IsEditingLongAppt;

                // If the appointment was originally a long appointment we
                // restore the original length else create a 3-hour task
                DateTime dateAtCursor = DayView.GetDateTimeAt(e.X, e.Y, false);
                selection.StartDate = dateAtCursor.Date;

                if (!base.IsEditingLongAppt)
				{
                    selection.EndDate = selection.StartDate.Add(selection.OriginalLength);
				}
				else
				{
					selection.EndDate = selection.StartDate.AddHours(3);
				}

				m_delta = TimeSpan.Zero;
				m_length = selection.Length;

				return true;
			}

			// all else
			return false;
		}

	}
}
