using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace TimeLogUIExtension
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
			{
				// Copy date back to real task because custom dates are temporary
				if (TimeLog.SelectedAppointment is TaskCustomDateAttribute)
				{
					var custDate = (TimeLog.SelectedAppointment as TaskCustomDateAttribute);

					custDate.UpdateTaskDate();
				}
				else if (TimeLog.SelectedAppointment is TaskTimeBlock)
				{
					var taskBlock = (TimeLog.SelectedAppointment as TaskTimeBlock);

					taskBlock.UpdateTimeBlock();
				}

				return true;
			}

			// whilst we handle transitions BETWEEN the regions
			TaskItem selection = (TimeLog.SelectedAppointment as TaskItem);

			if (selection == null) // extension item
				return false;

			// Note: we need to duplicate some of the base checks
			// because we don't know what failed
			if (EditMode != Mode.Move)
				return false;

			Rectangle shortApptsRect = TimeLog.GetTrueRectangle();
			Rectangle longApptsRect = TimeLog.GetFullDayApptsRectangle();

            // Make sure the long appointments rect has a reasonable height
            if (longApptsRect.Height < TimeLog.LongAppointmentHeight)
            {
                int adjustment = (TimeLog.LongAppointmentHeight - longApptsRect.Height);

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

			bool longAppt = selection.IsLongAppt();
			bool shortAppt = !longAppt;

			bool curPtInLongApptsRect = longApptsRect.Contains(e.Location);
			bool prevPtInLongApptsRect = longApptsRect.Contains(m_lastMouseMove);

			bool curPtInShortApptsRect = shortApptsRect.Contains(e.Location);
			bool prevPtInShortApptsRect = shortApptsRect.Contains(m_lastMouseMove);

            TimeSpan length = TimeSpan.Zero;
            DateTime dateAtCursor = TimeLog.GetDateTimeAt(e.X, e.Y, curPtInLongApptsRect);

            if (shortAppt && prevPtInShortApptsRect && curPtInLongApptsRect)
			{
				// Short appointment dragged into long appointment rect
				m_Transitioned = !base.IsEditingLongAppt;

				// If the appointment was originally a long appointment we
				// restore the original length else create a day-long task
                length = (m_Transitioned ? new TimeSpan(24, 0, 0) : selection.OriginalLength);
			}
			else if (longAppt && prevPtInLongApptsRect && curPtInShortApptsRect)
			{
				// Long appointment dragged into short appointment rect
				m_Transitioned = base.IsEditingLongAppt;

                // If the appointment was originally a long appointment we
                // restore the original length else create a 3-hour task
                length = (m_Transitioned ? new TimeSpan(3, 0, 0) : selection.OriginalLength);
			}

            if (length != TimeSpan.Zero)
            {
                selection.StartDate = dateAtCursor.AddHours(-length.TotalHours / 2);
                selection.EndDate = selection.StartDate.Add(length);

                m_delta = (selection.StartDate - dateAtCursor);
                m_length = TimeSpan.Zero;

                return true;
            }

            // all else
            return false;
		}

	}
}
