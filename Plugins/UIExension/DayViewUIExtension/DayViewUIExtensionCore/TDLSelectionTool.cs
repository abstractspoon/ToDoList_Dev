using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	public class TDLSelectionTool : Calendar.SelectionTool
	{
		override protected bool MoveAppointment(System.Windows.Forms.MouseEventArgs e)
		{
			// Snapshot the previously handled mouse position
			// because the default implementation may update it
			Point prevMousePos = m_lastMouseMove;

			// Default implementation handles move WITHIN the respective
			// 'long/short appointments' regions, 
			if (base.MoveAppointment(e))
				return true;

			// whilst we handle transitions BETWEEN the regions

			// Note: we need to duplicate some of the base checks
			// because we don't know what failed
			if (EditMode != Mode.Move)
				return false;

			Rectangle shortApptsRect = DayView.GetTrueRectangle();
			Rectangle longApptsRect = DayView.GetFullDayApptsRectangle();

			if (!shortApptsRect.Contains(e.Location) && !longApptsRect.Contains(e.Location))
				return false;

			//              _  _________________________________________
			//                |   ____________                          |
			//                |  |            |               ^         |
			// longApptsRect  |  |   long     |      ---------|---------| -
			//                |  |____________|      transition zone    | longZoneRect
			//              _ |_________|_____________________|_________| _
			//                | transition zone               |         | shortZoneRect
			//                |---------|------               |         | -
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

			// Create transition zones for the rect we are entering
			Rectangle shortZoneRect = shortApptsRect;
			shortZoneRect.Height = DayView.LongAppointmentHeight; // arbitrary

			Rectangle longZoneRect = shortZoneRect;
			longZoneRect.Offset(0, -shortZoneRect.Height);





			Calendar.Appointment selection = DayView.SelectedAppointment;

			bool longAppt = selection.IsLongAppt();
			bool shortAppt = !longAppt;

			bool curPtInLongApptsRect = longApptsRect.Contains(e.Location);
			bool prevPtInLongApptsRect = longApptsRect.Contains(prevMousePos);

			bool curPtInShortApptsRect = shortApptsRect.Contains(e.Location);
			bool prevPtInShortApptsRect = shortApptsRect.Contains(prevMousePos);

			// Get date/time at mouse position
			DateTime dateAtCursor = DayView.GetDateTimeAt(e.X, e.Y, longAppt);

			if (shortAppt && prevPtInShortApptsRect && curPtInLongApptsRect)
			{
				// Short appointment dragged into long appointment rect
			}
			else if (longAppt && prevPtInLongApptsRect && curPtInShortApptsRect)
			{
			}

			// all else
			return false;
		}

	}
}
