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
			if (ResizingMode != Mode.Move)
				return false;

			Rectangle shortApptsRect = DayView.GetTrueRectangle();
			Rectangle longApptsRect = DayView.GetFullDayApptsRectangle();

			if (!shortApptsRect.Contains(e.Location) && !longApptsRect.Contains(e.Location))
				return false;

			// Add transition zone to the rect we are leaving
			Calendar.Appointment selection = DayView.SelectedAppointment;

			bool longAppt = selection.IsLongAppt();
			bool ptInLongApptsRect = longApptsRect.Contains(e.Location);

			bool shortAppt = !longAppt;
			bool ptInShortApptsRect = !ptInLongApptsRect;

			// Get date/time at mouse position
			DateTime dateAtCursor = DayView.GetDateTimeAt(e.X, e.Y, longAppt);

			if (shortAppt && ptInLongApptsRect)
			{
			}
			else if (longAppt && ptInShortApptsRect)
			{
			}

			// all else
			return false;
		}

	}
}
