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
    class TDLRenderer : Calendar.AbstractRenderer
    {
		private UIExtension.TaskIcon m_TaskIcons;
		private IntPtr m_hWnd;
		private Font m_BaseFont, m_BoldFont;
		private int m_ColWidth = -1;
		private int m_HeaderPadding = DPIScaling.Scale(3);

		enum DowNameStyle
		{
			None,
			Short,
			Long
		}
		private DowNameStyle DowStyle { get; set; }

		enum MonthNameStyle
		{
			None,
			Number,
			Short,
			Long
		}
		private MonthNameStyle MonthStyle { get; set; }

		// ------------------------------------------------------------------------

		public TDLRenderer(IntPtr hWnd, UIExtension.TaskIcon taskIcons)
		{
			m_TaskIcons = taskIcons;
			m_hWnd = hWnd;

			ShowParentsAsFolder = false;
            TaskColorIsBackground = false;
            StrikeThruDoneTasks = true;
            GridlineColor = Color.Gray;
			DowStyle = DowNameStyle.Long;
			MonthStyle = MonthNameStyle.Long;
		}

		public bool ShowParentsAsFolder { get; set; }
		public bool TaskColorIsBackground { get; set; }
        public bool StrikeThruDoneTasks { get; set; }

        public Color GridlineColor { get; set; }
        public UITheme Theme { get; set; }
		public int TextPadding { get { return 2; } }

        protected override void Dispose(bool mainThread)
        {
            base.Dispose(mainThread);

            if (m_BaseFont != null)
                m_BaseFont.Dispose();
        }

        public override Font BaseFont
        {
            get
            {
                if (m_BaseFont == null)
                {
                    m_BaseFont = new Font("Tahoma", 8, FontStyle.Regular);
                }

                return m_BaseFont;
            }
        }

		public Font BoldFont
		{
			get
			{
				if (m_BoldFont == null)
				{
					m_BoldFont = new Font(BaseFont, FontStyle.Bold);
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
				switch (monthStyle)
				{
				case MonthNameStyle.Long:
					format += "d MMMM";
					break;

				case MonthNameStyle.Short:
					format += "d MMM";
					break;

				case MonthNameStyle.Number:
					format += "d/M";
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
			return (int)Math.Ceiling(g.MeasureString("31/12", BaseFont).Width);
		}

		private void UpdateHeaderStyles(Graphics g)
		{
			int availWidth = (m_ColWidth - (m_HeaderPadding * 2));

			// Basic header string format is '<Day of week> <Day of month> <Month>'
			int maxDayNum = (int)(g.MeasureString("31", BaseFont).Width);
			int maxDayAndMonthNum = (int)(g.MeasureString("31/12", BaseFont).Width);

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

		public int ColumnWidth { get { return m_ColWidth; } }

		public override void SetColumnWidth(Graphics g, int colWidth)
		{
			if (m_ColWidth == colWidth)
				return;

			m_ColWidth = colWidth;

			// Update the visibility of the day of week component
			UpdateHeaderStyles(g);
		}

		public void SetFont(String fontName, int fontSize)
        {
            if ((m_BaseFont.Name == fontName) && (m_BaseFont.Size == fontSize))
                return;

            m_BaseFont = new Font(fontName, fontSize, FontStyle.Regular);
            m_BoldFont = null;
 
			// Update the visibility of the day of week component
			using (Graphics g = Graphics.FromHwnd(m_hWnd))
			{
				UpdateHeaderStyles(g);
			}
		}

		public int GetFontHeight()
        {
            return BaseFont.Height;
        }

        public override Color HourColor
        {
            get
            {
                return Color.FromArgb(230, 237, 247);
            }
        }

        public override Color HalfHourSeperatorColor
        {
            get
            {
                return GridlineColor;
            }
        }

        public override Color HourSeperatorColor
        {
            get
            {
				// Slightly darker
                return DrawingColor.AdjustLighting(GridlineColor, -0.2f, false);
            }
        }

        public override Color WorkingHourColor
        {
            get
            {
                return Color.FromArgb(255, 255, 255);
            }
        }

        public override Color BackColor
        {
            get
            {
				return Theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
            }
        }

        public override Color SelectionColor
        {
            get
            {
                return Color.FromArgb(41, 76, 122);
            }
        }

        public Color TextColor
        {
            get
            {
                return Theme.GetAppDrawingColor(UITheme.AppColor.AppText);
            }
        }

		public override void DrawHourLabel(Graphics g, Rectangle rect, int hour, bool ampm)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            using (SolidBrush brush = new SolidBrush(this.TextColor))
            {
				// Ignore 'ampm' and format for the current regional settings
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
				g.DrawString(hourStr, HourFont, brush, rect);

                rect.X += ((int)g.MeasureString(hourStr, HourFont).Width + 2);

                g.DrawString(amPmTime, MinuteFont, brush, rect);
				g.TextRenderingHint = TextRenderingHint.SystemDefault;
			}
        }

        public override void DrawMinuteLine(Graphics g, Rectangle rect, int minute)
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
                    else if (rect.Height > MinuteFont.Height)
                    {
                        // 30 min mark - halve line width
                        rect.X += rect.Width / 2;
                        rect.Width /= 2;

						g.DrawLine(pen, rect.Left, rect.Y, rect.Right, rect.Y);

                        // Draw label beneath
                        using (SolidBrush brush = new SolidBrush(this.TextColor)) 
                        {
                            g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;
                            g.DrawString("30", MinuteFont, brush, rect);
                            g.TextRenderingHint = TextRenderingHint.SystemDefault;
                        }
                    }
                }
            }
        }

        private Color MinuteLineColor
        {
            get
            {
                Color appLineColor = Theme.GetAppDrawingColor(UITheme.AppColor.AppLinesDark);

                if (appLineColor == BackColor)
                    appLineColor = Theme.GetAppDrawingColor(UITheme.AppColor.AppLinesLight);

                return appLineColor;
            }
        }

        public override void DrawDayHeader(Graphics g, Rectangle rect, DateTime date, bool firstDay)
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
			Font font = ((date.Day == 1) ? BoldFont : BaseFont);

			Rectangle rText = rect;
			rText.X += m_HeaderPadding;
			rText.Width -= m_HeaderPadding;

			g.DrawString(text, font, SystemBrushes.WindowText, rText, fmt);
		}

		public override void DrawDayBackground(Graphics g, Rectangle rect)
        {
            //using (SolidBrush backBrush = new SolidBrush(Theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark)))
            //    g.FillRectangle(backBrush, rect);
        }

		public override void DrawHourRange(Graphics g, Rectangle rect, bool drawBorder, bool hilight)
		{
			if (hilight)
			{
				// Draw selection rect
				UIExtension.SelectionRect.Draw(m_hWnd, 
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
				base.DrawHourRange(g, rect, drawBorder, hilight);
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
			if (appt is FutureOccurrence)
				return (appt as FutureOccurrence).RealTaskId;

			return appt.Id;
		}

		public override void DrawAppointment(Graphics g, Calendar.AppointmentView apptView, bool isLong, bool isSelected)
        {
            if (apptView == null)
                throw new ArgumentNullException("appointment view");

            if (g == null)
                throw new ArgumentNullException("g");

			var appt = apptView.Appointment;
			var rect = apptView.Rectangle;
			var gripRect = apptView.GripRect;

			var tdlView = (apptView as TDLAppointmentView);

			if (rect.Width != 0 && rect.Height != 0)
            {
				TaskItem taskItem;
				bool isExtItem = (appt is TaskExtensionItem);

				if (isExtItem)
					taskItem = (appt as TaskExtensionItem).RealTask;
				else
					taskItem = (appt as TaskItem);

				UInt32 realTaskId = taskItem.Id;
				
				// Recalculate colours
				Color textColor = taskItem.TaskTextColor;
				Color fillColor = DrawingColor.SetLuminance(textColor, 0.95f);

				bool isFutureItem = (appt is FutureOccurrence);

				if (isFutureItem)
				{
					fillColor = SystemColors.Window;
					textColor = appt.TextColor;
				}

				Color borderColor = textColor;
				Color barColor = textColor;

				if (taskItem.HasTaskTextColor)
				{
					if (isSelected)
					{
						textColor = DrawingColor.SetLuminance(textColor, 0.3f);
					}
					else if (TaskColorIsBackground && !taskItem.IsDoneOrGoodAsDone && !isFutureItem)
					{
						barColor = textColor;
						fillColor = textColor;

						borderColor = DrawingColor.AdjustLighting(textColor, -0.5f, true);
						textColor = DrawingColor.GetBestTextColor(textColor);
					}
				}

                // Draw the background of the appointment
                g.SmoothingMode = SmoothingMode.None;

                if (isSelected)
                {
                    if (isLong)
                        rect.Height++;

					if (isFutureItem)
					{
						UIExtension.SelectionRect.Draw(m_hWnd,
														g,
														rect.Left,
														rect.Top,
														rect.Width,
														rect.Height,
														UIExtension.SelectionRect.Style.DropHighlighted,
														false); // opaque
					}
					else
					{
						UIExtension.SelectionRect.Draw(m_hWnd,
														g,
														rect.Left,
														rect.Top,
														rect.Width,
														rect.Height,
														false); // opaque
					}
                }
                else
                {
                    using (SolidBrush brush = new SolidBrush(fillColor))
                        g.FillRectangle(brush, rect);

                    if (taskItem.DrawBorder)
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

                // Draw appointment icon
                bool hasIcon = false;
				tdlView.IconRect = Rectangle.Empty;

                if (TaskHasIcon(taskItem))
                {
                    Rectangle rectIcon;
                    int imageSize = DPIScaling.Scale(16);

                    if (isLong)
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
                        if (isLong)
                        {
                            rectIcon.X = (gripRect.Right + TextPadding);
                        }
                        else
                        {
                            gripRect.Y += (imageSize + TextPadding);
                            gripRect.Height -= (imageSize + TextPadding);
                        }

                        var clipRgn = g.Clip;

                        if (rect.Bottom < (rectIcon.Y + imageSize))
                            g.Clip = new Region(RectangleF.Intersect(rect, g.ClipBounds));

                        m_TaskIcons.Draw(g, rectIcon.X, rectIcon.Y);

                        g.Clip = clipRgn;

                        hasIcon = true;
						tdlView.IconRect = rectIcon;

                        rect.Width -= (rectIcon.Right - rect.Left);
                        rect.X = rectIcon.Right;
                    }
                }

                // Draw gripper bar
                if (gripRect.Width > 0)
                {
                    using (SolidBrush brush = new SolidBrush(barColor))
                        g.FillRectangle(brush, gripRect);

                    if (!isLong)
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

                // draw appointment text
				if (rect.Width > 0)
				{
					using (StringFormat format = new StringFormat())
					{
						format.Alignment = StringAlignment.Near;
						format.LineAlignment = (isLong ? StringAlignment.Center : StringAlignment.Near);

						if (isLong)
							format.FormatFlags |= (StringFormatFlags.NoClip | StringFormatFlags.NoWrap);

						rect.Y += 3;

						if (isLong)
							rect.Height = BaseFont.Height;
						else
							rect.Height -= 3;

						tdlView.TextRect = rect;
						g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;

						using (SolidBrush brush = new SolidBrush(textColor))
						{
							if (taskItem.IsDone && StrikeThruDoneTasks)
							{
								using (Font font = new Font(this.BaseFont, FontStyle.Strikeout))
								{
									g.DrawString(appt.Title, font, brush, rect, format);
								}
							}
							else
							{
								g.DrawString(appt.Title, this.BaseFont, brush, rect, format);
							}
						}

						g.TextRenderingHint = TextRenderingHint.SystemDefault;
					}
				}
			}
		}
    }
}
