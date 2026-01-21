using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms.VisualStyles;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace LoggedTimeUIExtension
{

	public partial class LoggedTimeView : Calendar.IRenderer
	{
		private DayViewUIExtension.RenderHelper m_RenderHelper = new DayViewUIExtension.RenderHelper(); 
		private int m_DayWidth = -1;

		// ------------------------------------------------------------------------

		public UITheme Theme
		{
			get { return m_RenderHelper.Theme; }
			set { m_RenderHelper.Theme = value; }
		}

		public Color GridlineColor
		{
			get { return m_RenderHelper.GridlineColor; }
			set { m_RenderHelper.GridlineColor = value; }
		}

		protected override void Dispose(bool mainThread)
        {
            base.Dispose(mainThread);

            m_RenderHelper.DisposeFonts();
        }

        public virtual Font BaseFont()
        {
			return m_RenderHelper.BaseFont;
		}

		public virtual Font HourFont()
		{
			return m_RenderHelper.HourFont;
		}


		public virtual Font MinuteFont()
		{
			return m_RenderHelper.MinuteFont;
		}

		public Font BoldFont()
		{
			return m_RenderHelper.BoldFont;
		}

		private void UpdateHeaderStyles(Graphics g)
		{
			m_RenderHelper.UpdateHeaderStyles(g, m_DayWidth);
		}

		public int DayWidth { get { return m_DayWidth; } }

		private void OnNotifyDayWidth(object sender, Graphics g, int colWidth)
		{
			if (m_DayWidth == colWidth)
				return;

			m_DayWidth = colWidth;

			// Update the visibility of the day of week component
			m_RenderHelper.UpdateHeaderStyles(g, m_DayWidth);
		}

		public void SetFont(String fontName, int fontSize)
        {
			m_RenderHelper.SetFont(Handle, fontName, fontSize, m_DayWidth);

			LongAppointmentHeight = m_RenderHelper.CalcLongAppointmentHeight(longAppointmentSpacing);
		}

		public virtual Color HourColor()
        {
            return m_RenderHelper.HourColor;
        }

        public virtual Color HalfHourSeperatorColor()
        {
            return m_RenderHelper.HalfHourSeperatorColor;
        }

        public virtual Color HourSeperatorColor()
        {
            return m_RenderHelper.HourSeperatorColor;
        }

        public virtual Color WorkingHourColor()
        {
            return m_RenderHelper.WorkingHourColor;
        }

        public virtual Color BackColor()
        {
			return m_RenderHelper.BackColor;
        }

		public virtual Color AllDayEventsBackColor()
		{
			return m_RenderHelper.AllDayEventsBackColor;
		}

		public virtual Color SelectionColor()
        {
			return m_RenderHelper.SelectionColor;
		}

		protected override void DrawDay(PaintEventArgs e, Rectangle rect, DateTime date)
		{
			e.Graphics.FillRectangle(SystemBrushes.Window, rect);

			if (SystemInformation.HighContrast)
			{
				// Draw selection first because it's opaque
				DrawDaySelection(e, rect, date);

				DrawDaySlotSeparators(e, rect, date);
				DrawNonWorkHours(e, rect, date);
				m_RenderHelper.DrawTodayBackground(e, rect, date);
				DrawDayAppointments(e, rect, date);
			}
			else
			{
				DrawDaySlotSeparators(e, rect, date);
				DrawNonWorkHours(e, rect, date);
				m_RenderHelper.DrawTodayBackground(e, rect, date);
				DrawDayAppointments(e, rect, date);

				// Draw selection last because it's translucent
				DrawDaySelection(e, rect, date);
			}

			DrawTodayTime(e, rect, date);
			DrawDayGripper(e, rect);
		}

		public virtual void DrawDayHeader(Graphics g, Rectangle rect, DateTime date, bool firstDay)
		{
			m_RenderHelper.DrawDayHeader(g, rect, date, firstDay);
		}

		public virtual void DrawHourLabel(Graphics g, Rectangle rect, int hour, bool ampm)
        {
			m_RenderHelper.DrawHourLabel(g, rect, hour, ampm);
		}

        public virtual void DrawMinuteLine(Graphics g, Rectangle rect, int minute)
        {
			m_RenderHelper.DrawMinuteLine(g, rect, minute);
		}

		public virtual void DrawDayGripper(Graphics g, Rectangle rect, int gripWidth)
		{
			m_RenderHelper.DrawDayGripper(g, rect, gripWidth);
		}

		public virtual void DrawAllDayBackground(Graphics g, Rectangle rect)
		{
			m_RenderHelper.DrawAllDayBackground(g, rect, DaysShowing, allDayEventsHeaderHeight);
		}

		public virtual void DrawDayBackground(Graphics g, Rectangle rect)
        {
			// Not needed
        }

		public virtual void DrawHourRange(Graphics g, Rectangle rect, bool drawBorder, bool hilight)
		{
			m_RenderHelper.DrawHourRange(Handle, g, rect, drawBorder, hilight);
		}

		public bool TaskHasIcon(TaskItem taskItem)
		{
			return ((m_RenderHelper.TaskIcons != null) &&
					(taskItem != null) &&
					(taskItem.HasIcon || (ShowParentsAsFolder && taskItem.IsParent)));
		}

		LogEntry GetLogEntry(Calendar.Appointment appt)
		{
			return (appt as LogEntry);
		}

		TaskItem GetTaskItem(Calendar.Appointment appt)
		{
			var entry = GetLogEntry(appt);

			if (entry.TaskId <= 0)
				return null;

			return m_TaskItems.GetItem(entry.TaskId);
		}

		void GetTaskColors(Calendar.AppointmentView apptView, bool isSelected, out Color textColor, out Color fillColor, out Color borderColor, out Color barColor)
		{
			var logEntry = GetLogEntry(apptView.Appointment);
			var taskItem = m_TaskItems.GetItem(logEntry.TaskId);

			bool isLong = apptView.IsLong;

			textColor = borderColor = fillColor = barColor = Color.Empty;

			if (isSelected)
			{
				if (SystemInformation.HighContrast)
				{
					textColor = borderColor = SystemColors.HighlightText;
					fillColor = SystemColors.Highlight;
				}
				else if (logEntry.TrueFillColor != Color.Empty)
				{
					textColor = UIExtension.SelectionRect.GetTextColor(UIExtension.SelectionRect.Style.Selected, logEntry.TrueFillColor);
					barColor = logEntry.TrueFillColor;
				}
				else if ((taskItem != null) && taskItem.HasTaskTextColor)
				{
					textColor = UIExtension.SelectionRect.GetTextColor(UIExtension.SelectionRect.Style.Selected, taskItem.TaskTextColor);
					barColor = taskItem.TaskTextColor;
				}
				else
				{
					textColor = UIExtension.SelectionRect.GetTextColor(UIExtension.SelectionRect.Style.Selected, Color.Black);
					barColor = textColor;
				}
			}
			else if (logEntry.TrueFillColor != Color.Empty)
			{
				fillColor = logEntry.TrueFillColor;
				textColor = DrawingColor.GetBestTextColor(fillColor, true);

				borderColor = DrawingColor.AdjustLighting(fillColor, -0.5f, true);
				barColor = fillColor;
			}
			else if ((taskItem != null) && taskItem.HasTaskTextColor)
			{
				textColor = borderColor = taskItem.TaskTextColor;
				fillColor = DrawingColor.SetLuminance(textColor, 0.95f);

				if (TaskColorIsBackground)
				{
					barColor = textColor;
					fillColor = textColor;

					textColor = DrawingColor.GetBestTextColor(textColor, true);
					borderColor = DrawingColor.AdjustLighting(textColor, -0.5f, true);
				}
			}
			else
			{
				textColor = borderColor = SystemColors.WindowText;
				fillColor = (isLong ? SystemColors.Control : SystemColors.Window);
			}
		}

		void DrawTaskBackground(Graphics g, 
								Rectangle rect,
								Calendar.AppointmentView apptView,
								bool isSelected,
								Color fillColor, 
								Color borderColor)
		{
			m_RenderHelper.DrawTaskBackground(Handle, 
												g, 
												rect, 
												apptView, 
												GetAppointmentSelectedState(apptView.Appointment), 
												fillColor, 
												borderColor);
		}

		/*
		void DrawTaskIconAndGripper(Graphics g, Calendar.AppointmentView apptView, bool isSelected, Color barColor, ref Rectangle rect)
		{
			if (rect.Width <= 0)
				return;

			TaskItem taskItem = GetTaskItem(apptView.Appointment);
			uint iconId = (TaskHasIcon(taskItem) ? taskItem.Id : 0);

			if (!isSelected && apptView.Appointment is TaskTimeBlock)
				barColor = Color.FromArgb(128, barColor);

			Rectangle iconRect;
			m_RenderHelper.DrawTaskIconAndGripper(g, apptView, iconId, barColor, ref rect, out iconRect);

			var tdlView = (apptView as TDLAppointmentView);
			tdlView.IconRect = iconRect;
		}
		*/

		static string FormatTime(double hours, string hoursFormat, string minutesFormat, Translator trans)
		{
			if (hours >= 1)
				return string.Format(trans.Translate(hoursFormat, Translator.Type.Label), hours.ToString("0.###"));

			// else
			int minutes = (int)((hours * 60) + 0.5);

			return string.Format(trans.Translate(minutesFormat, Translator.Type.Label), minutes);
		}

		static string FormatTimeSpent(LogEntry entry, Translator trans)
		{
			return FormatTime(entry.TimeSpentInHrs, "{0}H logged", "{0}m logged", trans);
		}

		static string FormatDuration(LogEntry entry, Translator trans)
		{
			return FormatTime(entry.Length.TotalHours, "{0}H duration", "{0}m duration", trans);
		}

		void DrawTaskText(Graphics g, Calendar.AppointmentView apptView, Rectangle rect, Color textColor)
		{
			var entry = (apptView.Appointment as LogEntry);

			var text = new List<string>()
			{
				FormatTimeSpent(entry, m_Trans),
				FormatDuration(entry, m_Trans)
			};

			if (entry.HasComment)
				text.Insert(0, entry.Comment);

			if (entry.HasTitle)
				text.Insert(0, entry.Title);
#if DEBUG
			text.Insert(0, string.Format("[{0}]", entry.Id));
#endif
			m_RenderHelper.DrawItemText(g, string.Join("\n", text), rect, textColor, FontStyle.Regular, false);
		}

		public void DrawAppointment(Graphics g, Rectangle daysRect, Calendar.AppointmentView apptView, bool isSelected)
        {
            if (apptView == null)
                throw new ArgumentNullException("appointment view");

            if (g == null)
                throw new ArgumentNullException("g");

			if (!m_RenderHelper.CalcAppointmentRects(apptView, StartDate, EndDate))
				return;

			Color textColor, fillColor, borderColor, barColor;
			GetTaskColors(apptView, isSelected, out textColor, out fillColor, out borderColor, out barColor);

			g.SmoothingMode = SmoothingMode.None;

			isSelected = WantDrawAppointmentSelected(apptView.Appointment);

			var apptRect = apptView.Rectangle;

			DrawTaskBackground(g, apptRect, apptView, isSelected, fillColor, borderColor);
			//DrawTaskIconAndGripper(g, apptView, isSelected, barColor, ref apptRect);
			DrawTaskText(g, apptView, apptRect, textColor);
		}

		protected void DrawTodayTime(PaintEventArgs e, Rectangle rect, DateTime date)
		{
			m_RenderHelper.DrawTodayTime(e.Graphics, rect, date, GetHourScrollPos(DateTime.Now));
		}

		protected void DrawNonWorkHours(PaintEventArgs e, Rectangle rect, DateTime date)
		{
			if (m_RenderHelper.Theme.HasAppColor(UITheme.AppColor.Weekends) && WeekendDays.Contains(date.DayOfWeek))
			{
				var weekendColor = m_RenderHelper.Theme.GetAppDrawingColor(UITheme.AppColor.Weekends, 128);

				// If this is also 'today' then convert to gray so it doesn't 
				// impose too much when the today colour is laid on top
				if (m_RenderHelper.WantDrawToday(date))
					weekendColor = DrawingColor.ToGray(weekendColor);

				using (var brush = new SolidBrush(weekendColor))
					e.Graphics.FillRectangle(brush, rect);
			}
			else if (m_RenderHelper.Theme.HasAppColor(UITheme.AppColor.NonWorkingHours))
			{
				var nonWorkColor = m_RenderHelper.Theme.GetAppDrawingColor(UITheme.AppColor.NonWorkingHours, 128);

				// If this is also 'today' then convert to gray so it doesn't 
				// impose too much when the today colour is laid on top
				if (m_RenderHelper.WantDrawToday(date))
					nonWorkColor = DrawingColor.ToGray(nonWorkColor);

				using (SolidBrush brush = new SolidBrush(nonWorkColor))
				{
					DrawNonWorkHours(e, new HourMin(0, 0), WorkStart, rect, brush);
					DrawNonWorkHours(e, LunchStart, LunchEnd, rect, brush);
					DrawNonWorkHours(e, WorkEnd, new HourMin(24, 0), rect, brush);
				}
			}
		}

		protected void DrawNonWorkHours(PaintEventArgs e, HourMin start, HourMin end, Rectangle rect, Brush brush)
		{
			if (start < end)
			{
				Rectangle hoursRect = GetHourRangeRectangle(start, end, rect);

				if (hoursRect.Y < HeaderHeight)
				{
					hoursRect.Height -= HeaderHeight - hoursRect.Y;
					hoursRect.Y = HeaderHeight;
				}

				e.Graphics.FillRectangle(brush, hoursRect);
			}
		}

		protected override bool WantDrawAppointmentSelected(Calendar.Appointment appt)
		{
			return (GetAppointmentSelectedState(appt) != UIExtension.SelectionRect.Style.None);
		}

		protected UIExtension.SelectionRect.Style GetAppointmentSelectedState(Calendar.Appointment appt)
		{
			if (!base.SavingToImage && (m_SelectedLogEntryId == appt.Id))
				return (Focused ? UIExtension.SelectionRect.Style.Selected : UIExtension.SelectionRect.Style.SelectedNotFocused);

			// else
			return UIExtension.SelectionRect.Style.None;
		}


	}
}
