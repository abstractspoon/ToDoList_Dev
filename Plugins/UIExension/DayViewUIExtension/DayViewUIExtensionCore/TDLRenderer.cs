using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms.VisualStyles;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

namespace DayViewUIExtension
{
	public partial class TDLDayView : Calendar.IRenderer
	{
		private UIExtension.TaskIcon m_TaskIcons;
		private Font m_BaseFont, m_BoldFont, m_HourFont, m_MinuteFont;

		private int m_DayWidth = -1;
		private int m_HeaderPadding = DPIScaling.Scale(3);

		enum DowNameStyle
		{
			None,
			Short,
			Long
		}
		private DowNameStyle DowStyle = DowNameStyle.Long;

		enum MonthNameStyle
		{
			None,
			Number,
			Short,
			Long
		}
		private MonthNameStyle MonthStyle = MonthNameStyle.Long;

		// ------------------------------------------------------------------------

        public UITheme Theme { get; set; }
		public int TextPadding { get { return 2; } }
		public int TextOffset { get { return 3; } }

        protected override void Dispose(bool mainThread)
        {
            base.Dispose(mainThread);

            if (m_BaseFont != null)
                m_BaseFont.Dispose();
        }

        public virtual Font BaseFont()
        {
			if (m_BaseFont == null)
			{
				m_BaseFont = new Font("Tahoma", 8.25f);
				RecalcLongAppointmentHeight();
			}

			return m_BaseFont;
		}

		public virtual Font HourFont()
		{
			if (m_HourFont == null)
			{
				m_HourFont = new Font(BaseFont().FontFamily.Name, 12);
			}

			return m_HourFont;
		}


		public virtual Font MinuteFont()
		{
			if (m_MinuteFont == null)
			{
				m_MinuteFont = new Font(BaseFont().FontFamily.Name, 8.25f);
			}

			return m_MinuteFont;
		}

		public Font BoldFont
		{
			get
			{
				if (m_BoldFont == null)
				{
					m_BoldFont = new Font(BaseFont(), FontStyle.Bold);
				}

				return m_BoldFont;
			}
		}

		private static string FormatHeaderText(DateTime date, DowNameStyle dowStyle, MonthNameStyle monthStyle, bool firstDay)
		{
			// Day of week
			string format = "";

			switch (dowStyle)
			{
			case DowNameStyle.Long:
				format += "dddd ";
				break;

			case DowNameStyle.Short:
				format += "ddd ";
				break;
			}

			// Day of month
			if (firstDay || (date.Day == 1))
			{
				var dateFormat = CultureInfo.CurrentCulture.DateTimeFormat.ShortDatePattern;
				bool monthBeforeDay = (dateFormat.IndexOf('M') < dateFormat.IndexOf('d'));

				switch (monthStyle)
				{
				case MonthNameStyle.Long:
					format += (monthBeforeDay ? "MMMM d" : "d MMMM");
					break;

				case MonthNameStyle.Short:
					format += (monthBeforeDay ? "MMM d" : "d MMM");
					break;

				case MonthNameStyle.Number:
					format += (monthBeforeDay ? "M/d" : "d/M");
					break;
				}
			}
			else
			{
				format += "d ";
			}

			if (format == "")
				return date.Day.ToString();

			return date.ToString(format);
		}

		public int CalculateMinimumDayWidthForImage(Graphics g)
		{
			return (int)Math.Ceiling(g.MeasureString("31/12", BaseFont()).Width);
		}

		private void UpdateHeaderStyles(Graphics g)
		{
			int availWidth = (m_DayWidth - (m_HeaderPadding * 2));

			// Basic header string format is '<Day of week> <Day of month> <Month>'
			int maxDayNum = (int)(g.MeasureString("31", BaseFont()).Width);
			int maxDayAndMonthNum = (int)(g.MeasureString("31/12", BaseFont()).Width);

			int maxLongDow = DateUtil.GetMaxDayOfWeekNameWidth(g, BoldFont, false);
			int maxShortDow = DateUtil.GetMaxDayOfWeekNameWidth(g, BoldFont, true);

			int maxLongMonth = DateUtil.GetMaxMonthNameWidth(g, BoldFont, false);
			int maxShortMonth = DateUtil.GetMaxMonthNameWidth(g, BoldFont, true);

			DowStyle = DowNameStyle.Long;
			MonthStyle = MonthNameStyle.Long;

			if (availWidth < (maxLongDow + maxDayNum + maxLongMonth))
			{
				if (availWidth >= (maxLongDow + maxDayNum + maxShortMonth))
				{
					DowStyle = DowNameStyle.Long;
					MonthStyle = MonthNameStyle.Short;
				}
				else if (availWidth >= (maxShortDow + maxDayNum + maxShortMonth))
				{
					DowStyle = DowNameStyle.Short;
					MonthStyle = MonthNameStyle.Short;
				}
				else if (availWidth >= (maxShortDow + maxDayAndMonthNum))
				{
					DowStyle = DowNameStyle.Short;
					MonthStyle = MonthNameStyle.Number;
				}
				else if (availWidth >= (maxDayNum + maxShortMonth))
				{
					DowStyle = DowNameStyle.None;
					MonthStyle = MonthNameStyle.Short;
				}
				else if (availWidth >= maxDayAndMonthNum)
				{
					DowStyle = DowNameStyle.None;
					MonthStyle = MonthNameStyle.Number;
				}
				else
				{
					DowStyle = DowNameStyle.None;
					MonthStyle = MonthNameStyle.None;
				}
			}
		}

		public int DayWidth { get { return m_DayWidth; } }

		private void OnNotifyDayWidth(object sender, Graphics g, int colWidth)
		{
			if (m_DayWidth == colWidth)
				return;

			m_DayWidth = colWidth;

			// Update the visibility of the day of week component
			UpdateHeaderStyles(g);
		}

		private void RecalcLongAppointmentHeight()
		{
			int fontHeight = 0;

			if (DPIScaling.WantScaling())
				fontHeight = BaseFont().Height;
			else
				fontHeight = Win32.GetPixelHeight(BaseFont().ToHfont());

			int itemHeight = (fontHeight + 6 - longAppointmentSpacing);

			LongAppointmentHeight = Math.Max(itemHeight, 17);

		}

		public void SetFont(String fontName, int fontSize)
        {
			if ((m_BaseFont.Name == fontName) && (m_BaseFont.Size == fontSize))
                return;

            m_BaseFont = new Font(fontName, fontSize, FontStyle.Regular);
            m_BoldFont = null;
 
			// Update the visibility of the day of week component
			using (Graphics g = Graphics.FromHwnd(Handle))
			{
				UpdateHeaderStyles(g);
			}

			RecalcLongAppointmentHeight();
		}

		private Color m_GridlineColor = Color.Gray;

		public Color GridlineColor
		{
			set
			{
				if (value != m_GridlineColor)
				{
					m_GridlineColor = value;
					Invalidate();
				}
			}
		}

		public int GetFontHeight()
        {
            return BaseFont().Height;
        }

        public virtual Color HourColor()
        {
            return Color.FromArgb(230, 237, 247);
        }

        public virtual Color HalfHourSeperatorColor()
        {
            return m_GridlineColor;
        }

        public virtual Color HourSeperatorColor()
        {
			// Slightly darker
            return DrawingColor.AdjustLighting(m_GridlineColor, -0.2f, false);
        }

        public virtual Color WorkingHourColor()
        {
            return Color.FromArgb(255, 255, 255);
        }

        public virtual Color BackColor()
        {
			return Theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
        }

		public virtual Color AllDayEventsBackColor()
		{
			return SystemColors.ControlDarkDark;
		}

		public virtual Color SelectionColor()
        {
            return Color.FromArgb(41, 76, 122);
        }

        public Color TextColor
        {
            get
            {
                return Theme.GetAppDrawingColor(UITheme.AppColor.AppText);
            }
        }

		public virtual void DrawHourLabel(Graphics g, Rectangle rect, int hour, bool ampm)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            using (SolidBrush brush = new SolidBrush(TextColor))
            {
				// Ignore 'am/pm' and format for the current regional settings
                string amPmTime = "00";

                if (!String.IsNullOrEmpty(DateTimeFormatInfo.CurrentInfo.AMDesignator))
				{
					if (hour < 12)
						amPmTime = DateTimeFormatInfo.CurrentInfo.AMDesignator;
					else
						amPmTime = DateTimeFormatInfo.CurrentInfo.PMDesignator;

					if (hour != 12)
						hour = hour % 12;
				}

				String hourStr = hour.ToString("##00", System.Globalization.CultureInfo.InvariantCulture);
                
				g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;
				g.DrawString(hourStr, HourFont(), brush, rect);

                rect.X += ((int)g.MeasureString(hourStr, HourFont()).Width + 2);

                g.DrawString(amPmTime, MinuteFont(), brush, rect);
				g.TextRenderingHint = TextRenderingHint.SystemDefault;
			}
        }

        public virtual void DrawMinuteLine(Graphics g, Rectangle rect, int minute)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            if ((minute % 30) == 0)
            {
                using (Pen pen = new Pen(MinuteLineColor))
                {
					g.SmoothingMode = SmoothingMode.None;

                    if (minute == 0)
                    {
                        g.DrawLine(pen, rect.Left, rect.Y, rect.Right, rect.Y);
                    }
                    else if (rect.Height > MinuteFont().Height)
                    {
                        // 30 min mark - halve line width
                        rect.X += rect.Width / 2;
                        rect.Width /= 2;

						g.DrawLine(pen, rect.Left, rect.Y, rect.Right, rect.Y);

                        // Draw label beneath
                        using (SolidBrush brush = new SolidBrush(TextColor)) 
                        {
                            g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;
                            g.DrawString("30", MinuteFont(), brush, rect);
                            g.TextRenderingHint = TextRenderingHint.SystemDefault;
                        }
                    }
                }
            }
        }

		public virtual void DrawDayGripper(Graphics g, Rectangle rect, int gripWidth)
		{
			Calendar.AbstractRenderer.DrawDayGripper(g, rect, gripWidth, HourSeperatorColor());
		}

		public virtual void DrawAllDayBackground(Graphics g, Rectangle rect)
		{
			using (Brush brush = new SolidBrush(Calendar.AbstractRenderer.InterpolateColors(BackColor(), Color.Black, 0.5f)))
				g.FillRectangle(brush, rect);
		}

		private Color MinuteLineColor
        {
            get
            {
                Color appLineColor = Theme.GetAppDrawingColor(UITheme.AppColor.AppLinesDark);

                if (appLineColor == BackColor())
                    appLineColor = Theme.GetAppDrawingColor(UITheme.AppColor.AppLinesLight);

                return appLineColor;
            }
        }

        public virtual void DrawDayHeader(Graphics g, Rectangle rect, DateTime date, bool firstDay)
        {
            if (g == null)
                throw new ArgumentNullException("g");

			// Header background
			bool isToday = date.Date.Equals(DateTime.Now.Date);

			Rectangle rHeader = rect;
			rHeader.X++;

			if (VisualStyleRenderer.IsSupported)
			{
				bool hasTodayColor = Theme.HasAppColor(UITheme.AppColor.Today);
				var headerState = VisualStyleElement.Header.Item.Normal;

				if (isToday && !hasTodayColor)
					headerState = VisualStyleElement.Header.Item.Hot;

				var renderer = new VisualStyleRenderer(headerState);
				renderer.DrawBackground(g, rHeader);

				if (isToday && hasTodayColor)
				{
					rHeader.X--;

					using (var brush = new SolidBrush(Theme.GetAppDrawingColor(UITheme.AppColor.Today, 64)))
						g.FillRectangle(brush, rHeader);
				}
			}
			else // classic theme
			{
				rHeader.Y++;

				var headerBrush = (isToday ? SystemBrushes.ButtonHighlight : SystemBrushes.ButtonFace);
                g.FillRectangle(headerBrush, rHeader);

				ControlPaint.DrawBorder3D(g, rHeader, Border3DStyle.Raised);
            }

			// Header text
			g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;

			var fmt = new StringFormat();

			fmt.LineAlignment = StringAlignment.Center;
			fmt.Alignment = StringAlignment.Near;
			fmt.FormatFlags |= StringFormatFlags.NoWrap | StringFormatFlags.NoClip;

			// Use bold font for first-day-of-month
			string text = FormatHeaderText(date, DowStyle, MonthStyle, firstDay);
			Font font = ((date.Day == 1) ? BoldFont : BaseFont());

			Rectangle rText = rect;
			rText.X += m_HeaderPadding;
			rText.Width -= m_HeaderPadding;

			g.DrawString(text, font, SystemBrushes.ControlText, rText, fmt);
		}

		public virtual void DrawDayBackground(Graphics g, Rectangle rect)
        {
			// Not needed
        }

		public virtual void DrawHourRange(Graphics g, Rectangle rect, bool drawBorder, bool hilight)
		{
			if (hilight)
			{
				// Draw selection rect
				UIExtension.SelectionRect.Draw(Handle, 
												g, 
												rect.X, 
												rect.Y, 
												rect.Width, 
												rect.Height, 
												UIExtension.SelectionRect.Style.SelectedNotFocused, 
												true); // transparent
			}
			else
			{
				Calendar.AbstractRenderer.DrawHourRange(g, rect, drawBorder, (hilight ? SelectionColor() : WorkingHourColor()));
			}
		}

		public bool TaskHasIcon(TaskItem taskItem)
		{
			return ((m_TaskIcons != null) &&
					(taskItem != null) &&
					(taskItem.HasIcon || (ShowParentsAsFolder && taskItem.IsParent)));
		}

		private UInt32 GetRealTaskId(Calendar.Appointment appt)
		{
			if (appt is FutureTaskOccurrence)
				return (appt as FutureTaskOccurrence).RealTaskId;

			return appt.Id;
		}

		TaskItem GetTaskItem(Calendar.Appointment appt)
		{
			bool isExtItem = (appt is TaskExtensionItem);

			if (isExtItem)
				return (appt as TaskExtensionItem).RealTask;

			// else
			return (appt as TaskItem);
		}

		void GetTaskColors(Calendar.AppointmentView apptView, bool isSelected, out Color textColor, out Color fillColor, out Color borderColor, out Color barColor)
		{
			TaskItem taskItem = GetTaskItem(apptView.Appointment);

			bool isFutureItem = (apptView.Appointment is FutureTaskOccurrence);
			bool isTimeBlock = (apptView.Appointment is TaskTimeBlock);
			bool isLong = apptView.IsLong;

			textColor = borderColor = fillColor = barColor = Color.Empty;

			if (SystemInformation.HighContrast)
			{
				if (isSelected)
				{
					textColor = borderColor = SystemColors.HighlightText;
					fillColor = SystemColors.Highlight;

					if (!isFutureItem)
						barColor = (taskItem.HasTaskTextColor ? taskItem.TaskTextColor : textColor);
				}
				else if (taskItem.HasTaskTextColor)
				{
					textColor = borderColor = taskItem.TaskTextColor;
					fillColor = DrawingColor.SetLuminance(textColor, 0.95f);

					if (!isFutureItem)
						barColor = textColor;
				}
				else
				{
					textColor = borderColor = SystemColors.WindowText;
					fillColor = (isLong ? SystemColors.Control : SystemColors.Window);

					if (!isFutureItem)
						barColor = textColor;
				}
			}
			else // NOT high contrast
			{
				if (isSelected)
				{
					textColor = UIExtension.SelectionRect.GetTextColor(UIExtension.SelectionRect.Style.Selected, taskItem.TaskTextColor);

					if (isFutureItem)
 						borderColor = (isLong ? AllDayEventsBackColor() : textColor);
					else
						barColor = (taskItem.HasTaskTextColor ? taskItem.TaskTextColor : textColor);
				}
				else if (taskItem.HasTaskTextColor)
				{
					textColor = borderColor = taskItem.TaskTextColor;
					fillColor = DrawingColor.SetLuminance(textColor, 0.95f);

					if (!isFutureItem)
						barColor = textColor;
				}
				else
				{
					textColor = borderColor = SystemColors.WindowText;
					fillColor = (isLong ? SystemColors.Control : SystemColors.Window);

					if (!isFutureItem)
						barColor = textColor;
				}
			}

			if (TaskColorIsBackground && 
				taskItem.HasTaskTextColor && 
				!isSelected && 
				!isFutureItem && 
				!isTimeBlock)
			{
				barColor = textColor;
				fillColor = textColor;

				textColor = DrawingColor.GetBestTextColor(textColor, true);
				borderColor = DrawingColor.AdjustLighting(textColor, -0.5f, true);
			}
		}

		void DrawTaskBackground(Graphics g, 
								Rectangle rect,
								Calendar.AppointmentView apptView,
								bool isSelected,
								Color fillColor, 
								Color borderColor)
		{
			if (rect.Width <= 0)
				return;

			bool isLong = apptView.IsLong;
			bool isFutureItem = (apptView.Appointment is FutureTaskOccurrence);
			bool isTimeBlock = (apptView.Appointment is TaskTimeBlock);

			if (isSelected)
			{
				if (isLong)
					rect.Height++;

				var style = (isFutureItem || isTimeBlock) ? UIExtension.SelectionRect.Style.DropHighlighted :
															UIExtension.SelectionRect.Style.Selected;
				UIExtension.SelectionRect.Draw(Handle,
												g,
												rect.Left,
												rect.Top,
												rect.Width,
												rect.Height,
												style,
												isTimeBlock);

				if (isFutureItem && !borderColor.IsEmpty)
				{
					rect.Height--; // drawing with pen adds 1 to height

					using (Pen pen = new Pen(borderColor, 1))
					{
						pen.DashStyle = DashStyle.Dash;
						g.DrawRectangle(pen, rect);
					}
				}
			}
			else
			{
				if (isTimeBlock && !SystemInformation.HighContrast)
					fillColor = Color.FromArgb(64, fillColor);

				using (SolidBrush brush = new SolidBrush(fillColor))
					g.FillRectangle(brush, rect);

				if (borderColor != Color.Empty)
				{
					if (!isLong)
					{
						rect.Height--; // drawing with pen adds 1 to height
						rect.Width--;
					}

					using (Pen pen = new Pen(borderColor, 1))
					{
						if (isFutureItem)
							pen.DashStyle = DashStyle.Dash;

						g.DrawRectangle(pen, rect);
					}
				}
			}
		}

		void DrawTaskIconAndGripper(Graphics g, Calendar.AppointmentView apptView, bool isSelected, Color barColor, ref Rectangle rect)
		{
			if (rect.Width <= 0)
				return;

			bool hasIcon = false;
			Rectangle gripRect = apptView.GripRect;

			var tdlView = (apptView as TDLAppointmentView);
			tdlView.IconRect = Rectangle.Empty;

			TaskItem taskItem = GetTaskItem(apptView.Appointment);
			UInt32 realTaskId = taskItem.Id;

			if (TaskHasIcon(taskItem))
			{
				Rectangle rectIcon;
				int imageSize = DPIScaling.Scale(16);

				if (apptView.IsLong)
				{
					int yCentre = ((rect.Top + rect.Bottom + 1) / 2);
					rectIcon = new Rectangle((rect.Left + TextPadding), (yCentre - (imageSize / 2)), imageSize, imageSize);
				}
				else
				{
					rectIcon = new Rectangle(rect.Left + TextPadding, rect.Top + TextPadding, imageSize, imageSize);
				}

				if (g.IsVisible(rectIcon) && m_TaskIcons.Get(realTaskId))
				{
					if (apptView.IsLong)
					{
						rectIcon.X = (gripRect.Right + TextPadding);
					}
					else
					{
						gripRect.Y += (imageSize + TextPadding);
						gripRect.Height -= (imageSize + TextPadding);
					}

					if (((rect.Right - rectIcon.Left) < imageSize) || (rect.Height < imageSize))
					{
						var clipRgn = g.Clip;
						g.Clip = new Region(RectangleF.Intersect(rect, g.ClipBounds));

						m_TaskIcons.Draw(g, rectIcon.X, rectIcon.Y);

						g.Clip = clipRgn;
					}
					else
					{
						m_TaskIcons.Draw(g, rectIcon.X, rectIcon.Y);
					}

					hasIcon = true;
					tdlView.IconRect = rectIcon;

					rect.Width -= (rectIcon.Right - rect.Left);
					rect.X = rectIcon.Right;
				}
			}

			if (gripRect.Width > 0)
			{
				if (!isSelected && apptView.Appointment is TaskTimeBlock)
					barColor = Color.FromArgb(128, barColor);

				using (SolidBrush brush = new SolidBrush(barColor))
					g.FillRectangle(brush, gripRect);

				if (!apptView.IsLong)
					gripRect.Height--; // drawing with pen adds 1 to height

				// Draw gripper border
				using (Pen pen = new Pen(DrawingColor.AdjustLighting(barColor, -0.5f, true), 1))
					g.DrawRectangle(pen, gripRect);

				if (!hasIcon)
				{
					rect.X = gripRect.Right;
					rect.Width -= gripRect.Width;
				}
			}
		}

		void DrawTaskText(Graphics g, Calendar.AppointmentView apptView, Rectangle rect, Color textColor)
		{
			if (rect.Width <= 0)
				return;

			using (StringFormat format = new StringFormat())
			{
				format.Alignment = StringAlignment.Near;
				format.LineAlignment = (apptView.IsLong ? StringAlignment.Center : StringAlignment.Near);

				if (apptView.IsLong)
					format.FormatFlags |= (StringFormatFlags.NoClip | StringFormatFlags.NoWrap);

				rect.Y += TextOffset;

				if (apptView.IsLong)
					rect.Height = BaseFont().Height;
				else
					rect.Height -= TextOffset;

				g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;

				using (SolidBrush brush = new SolidBrush(textColor))
				{
					TaskItem taskItem = GetTaskItem(apptView.Appointment);

					if (taskItem.IsDone && StrikeThruDoneTasks)
					{
						using (Font font = new Font(BaseFont(), FontStyle.Strikeout))
						{
							g.DrawString(taskItem.Title, font, brush, rect, format);
						}
					}
					else
					{
						g.DrawString(taskItem.Title, BaseFont(), brush, rect, format);
					}
				}

				g.TextRenderingHint = TextRenderingHint.SystemDefault;
			}
		}

		public bool CalcAppointmentRects(Calendar.AppointmentView apptView)
		{
			if (apptView.Rectangle.Width == 0 || apptView.Rectangle.Height == 0)
			{
				return false;
			}

			var appt = apptView.Appointment;
			var apptRect = apptView.Rectangle;

			// Our custom gripper bar
			var gripRect = apptRect;
			gripRect.Inflate(-2, -2);
			gripRect.Width = 5;

			bool longAppt = apptView.IsLong;

			if (longAppt)
			{
				// If and the start date precedes the 
				// start of the week then extend the draw rect to the left 
				// so the edge is clipped and likewise for the end date.
				if (appt.StartDate < StartDate)
				{
					apptRect.X -= 4;
					apptRect.Width += 4;

					gripRect.X = apptRect.X;
					gripRect.Width = 0;
				}
				else if (appt.StartDate > StartDate)
				{
					apptRect.X++;
					apptRect.Width--;

					gripRect.X++;
				}

				if (appt.EndDate >= EndDate)
				{
					apptRect.Width += 5;
				}
			}
			else // day appt
			{
				if (appt.StartDate.TimeOfDay.TotalHours == 0.0)
				{
					apptRect.Y++;
					apptRect.Height--;
				}

				apptRect.Width -= 1;
			}

			apptView.Rectangle = apptRect;
			apptView.GripRect = gripRect;

			return true;
		}

		public bool CalcDiscontinousAppointmentRects(TDLAppointmentView tdlView, Rectangle daysRect,  
													out Rectangle startRect, out Rectangle endRect, out Rectangle todayRect)
		{
			var appt = tdlView.Appointment;

			startRect = Rectangle.Empty;
			endRect = Rectangle.Empty;
			todayRect = Rectangle.Empty;

			if (!tdlView.IsLong || DisplayLongTasksContinuous)
				return false;

			double startDay = (appt.StartDate - StartDate).TotalDays;
			double endDay = (appt.EndDate - StartDate).TotalDays;

			tdlView.EndOfStart = (daysRect.X + ((int)(startDay + 1)) * m_DayWidth);
			tdlView.StartOfEnd = (daysRect.X + ((int)endDay) * m_DayWidth);

			if (tdlView.EndOfStart >= tdlView.StartOfEnd)
			{
				// Task is effectively continuous
				tdlView.StartOfEnd = tdlView.EndOfStart;
				return false;
			}

			// Start is discontinuous from end, so now we calculate today's extents
			// and see if they is continuous with either the start or end
			if (DisplayActiveTasksToday && IsTodayVisible)
			{
				var taskItem = (appt as TaskItem);

				if (taskItem != null)
				{
					double startToday = (DateTime.Now.Date - StartDate).TotalDays;
					double endToday = (startToday + 1);

					// Today must not coincide with start day or end day
					if ((startToday > startDay) && (endToday < endDay))
					{
						tdlView.StartOfToday = (daysRect.X + ((int)startToday) * m_DayWidth);
						tdlView.EndOfToday = (tdlView.StartOfToday + m_DayWidth);

						// if 'today' is continuous with the start or end piece
						// absorb it into that adjacent piece
						if (tdlView.StartOfToday == tdlView.EndOfStart)
						{
							tdlView.EndOfStart = tdlView.EndOfToday;
						}
						else if (tdlView.EndOfToday == tdlView.StartOfEnd)
						{
							tdlView.StartOfEnd = tdlView.StartOfToday;
						}
						else
						{
							todayRect = tdlView.Rectangle;
							todayRect.X = tdlView.StartOfToday;
							todayRect.Width = m_DayWidth;
						}
					}

					// Check again
					if (tdlView.EndOfStart >= tdlView.StartOfEnd)
					{
						// Task is effectively continuous
						tdlView.StartOfEnd = tdlView.EndOfStart;
						return false;
					}
				}
			}

			//else
			var apptRect = tdlView.Rectangle;

			startRect = apptRect;
			startRect.Width = tdlView.EndOfStart - apptRect.Left;

			endRect = apptRect;
			endRect.X = tdlView.StartOfEnd;
			endRect.Width = apptRect.Right - endRect.X;

			return true; // Discontinuous to some degree
		}

		public void DrawAppointment(Graphics g, Rectangle daysRect, Calendar.AppointmentView apptView, bool isSelected)
        {
            if (apptView == null)
                throw new ArgumentNullException("appointment view");

            if (g == null)
                throw new ArgumentNullException("g");

			if (!CalcAppointmentRects(apptView))
				return;

			Color textColor, fillColor, borderColor, barColor;
			GetTaskColors(apptView, isSelected, out textColor, out fillColor, out borderColor, out barColor);

			g.SmoothingMode = SmoothingMode.None;

			isSelected = WantDrawAppointmentSelected(apptView.Appointment);

			var tdlView = (apptView as TDLAppointmentView);
			Rectangle startRect, endRect, todayRect;

			if (CalcDiscontinousAppointmentRects(tdlView, daysRect, out startRect, out endRect, out todayRect))
			{
				// Start Part ------------------------------------------
				DrawTaskBackground(g, startRect, apptView, isSelected, fillColor, borderColor);
				DrawTaskIconAndGripper(g, apptView, isSelected, barColor, ref startRect);
				DrawTaskText(g, apptView, startRect, textColor);

				tdlView.TextHorzOffset = (startRect.X - apptView.Rectangle.X);

				// Today Part ------------------------------------------
				if (!todayRect.IsEmpty)
				{
					DrawTaskBackground(g, todayRect, apptView, isSelected, fillColor, borderColor);
					DrawTaskText(g, apptView, todayRect, textColor);
				}

				// End Part --------------------------------------------
				DrawTaskBackground(g, endRect, apptView, isSelected, fillColor, borderColor);
				DrawTaskText(g, apptView, endRect, textColor);
			}
			else // draw continuous)
			{
				var apptRect = apptView.Rectangle;

				DrawTaskBackground(g, apptRect, apptView, isSelected, fillColor, borderColor);
				DrawTaskIconAndGripper(g, apptView, isSelected, barColor, ref apptRect);
				DrawTaskText(g, apptView, apptRect, textColor);

				tdlView.TextHorzOffset = (apptView.Rectangle.X - apptRect.X);
			}
		}
	}
}
