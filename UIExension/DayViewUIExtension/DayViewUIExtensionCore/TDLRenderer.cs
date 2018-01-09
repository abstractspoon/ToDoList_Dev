using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms.VisualStyles;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
    public class TDLRenderer : Calendar.AbstractRenderer
    {
        public TDLRenderer(IntPtr hWnd, UIExtension.TaskIcon taskIcons)
        {
            // One time initialisation
            if (m_SelectionRect == null)
            {
                m_SelectionRect = new UIExtension.SelectionRect();

                if (VisualStyleRenderer.IsSupported)
                {
                    if (VisualStyleRenderer.IsElementDefined(VisualStyleElement.Header.Item.Normal))
                        m_HeaderNormal = new VisualStyleRenderer(VisualStyleElement.Header.Item.Normal);

                    if (VisualStyleRenderer.IsElementDefined(VisualStyleElement.Header.Item.Hot))
                        m_HeaderHot = new VisualStyleRenderer(VisualStyleElement.Header.Item.Hot);
                }
            }

			m_TaskIcons = taskIcons;
			m_hWnd = hWnd;
        }

		private UIExtension.SelectionRect m_SelectionRect;
        private VisualStyleRenderer m_HeaderNormal, m_HeaderHot;
        private UITheme m_theme;
		private UIExtension.TaskIcon m_TaskIcons;
		private IntPtr m_hWnd;
        private Font m_BaseFont;
        
        public UITheme Theme
        {
            set { m_theme = value; }
        }

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

        public void SetFont(String fontName, int fontSize)
        {
            if ((m_BaseFont.Name == fontName) && (m_BaseFont.Size == fontSize))
                return;

            m_BaseFont = new Font(fontName, fontSize, FontStyle.Regular);
        }
        
        public int GetFontHeight()
        {
            return BaseFont.Height;
        }

        public override System.Drawing.Color HourColor
        {
            get
            {
                return System.Drawing.Color.FromArgb(230, 237, 247);
            }
        }

        public override System.Drawing.Color HalfHourSeperatorColor
        {
            get
            {
                return System.Drawing.Color.FromArgb(213, 215, 241);
            }
        }

        public override System.Drawing.Color HourSeperatorColor
        {
            get
            {
                return System.Drawing.Color.FromArgb(165, 191, 225);
            }
        }

        public override System.Drawing.Color WorkingHourColor
        {
            get
            {
                return System.Drawing.Color.FromArgb(255, 255, 255);
            }
        }

        public override System.Drawing.Color BackColor
        {
            get
            {
                return m_theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
            }
        }

        public override System.Drawing.Color SelectionColor
        {
            get
            {
                return System.Drawing.Color.FromArgb(41, 76, 122);
            }
        }

        public System.Drawing.Color TextColor
        {
            get
            {
                return m_theme.GetAppDrawingColor(UITheme.AppColor.AppText);
            }
        }

        public override void DrawHourLabel(System.Drawing.Graphics g, System.Drawing.Rectangle rect, int hour, bool ampm)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            using (SolidBrush brush = new SolidBrush(this.TextColor))
            {
                string ampmtime;

                if (ampm)
                {
                    if (hour < 12)
                        ampmtime = "AM";
                    else
                        ampmtime = "PM";

                    if (hour != 12)
                        hour = hour % 12;
                }
                else
                    ampmtime = "00";

				g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;
				g.DrawString(hour.ToString("##00", System.Globalization.CultureInfo.InvariantCulture), HourFont, brush, rect);

                rect.X += 27;
                g.DrawString(ampmtime, MinuteFont, brush, rect);
				g.TextRenderingHint = TextRenderingHint.SystemDefault;
			}
        }

        public override void DrawMinuteLine(Graphics g, Rectangle rect, bool hour)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            if (hour)
            {
                using (Pen pen = new Pen(InterpolateColors(this.TextColor, System.Drawing.Color.White, 0.5f)))
                    g.DrawLine(pen, rect.Left, rect.Y, rect.Width, rect.Y);
            }
        }

        public override void DrawDayHeader(System.Drawing.Graphics g, System.Drawing.Rectangle rect, DateTime date)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            System.Drawing.Rectangle rHeader = rect;
            rHeader.Height += 2;

            if (date.Date.Equals(DateTime.Now.Date))
            {
                rHeader.Width += 1;

                if (m_HeaderHot != null)
                {
                    m_HeaderHot.DrawBackground(g, rHeader);
                }
                else
                {
                    using (SolidBrush brush = new SolidBrush(SystemColors.ButtonHighlight))
                        g.FillRectangle(brush, rHeader);
                }
            }
            else
            {
                rHeader.X += 1;

                if (m_HeaderNormal != null)
                {
                    m_HeaderNormal.DrawBackground(g, rHeader);
                }
                else
                {
                    using (SolidBrush brush = new SolidBrush(SystemColors.ButtonFace))
                        g.FillRectangle(brush, rHeader);
                }
            }

            using (StringFormat format = new StringFormat())
            {
                format.Alignment = StringAlignment.Center;
                format.FormatFlags = StringFormatFlags.NoWrap;
                format.LineAlignment = StringAlignment.Center;

                using (StringFormat formatdd = new StringFormat())
                {
                    formatdd.Alignment = StringAlignment.Near;
                    formatdd.FormatFlags = StringFormatFlags.NoWrap;
                    formatdd.LineAlignment = StringAlignment.Center;

                    g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;

                    //get short dayabbr. if narrow dayrect
                    string sTodaysName = System.Globalization.CultureInfo.CurrentCulture.DateTimeFormat.GetDayName(date.DayOfWeek);
                    if (rect.Width < 105)
                        sTodaysName = System.Globalization.CultureInfo.CurrentCulture.DateTimeFormat.GetAbbreviatedDayName(date.DayOfWeek);

                    rect.Offset(2, 1);

					using (Font fntDay = new Font("Tahoma", 8, FontStyle.Regular))
                        g.DrawString(sTodaysName, fntDay, SystemBrushes.WindowText, rect, format);

                    rect.Offset(-2, -1);

                    using (Font fntDayDate = new Font("Tahoma", 9, FontStyle.Bold))
                        g.DrawString(date.ToString(" d"), fntDayDate, SystemBrushes.WindowText, rect, formatdd);
                }
            }
        }

        public override void DrawDayBackground(System.Drawing.Graphics g, System.Drawing.Rectangle rect)
        {
            using (SolidBrush backBrush = new SolidBrush(System.Drawing.Color.FromArgb(0xe6, 0xed, 0xf7)))
                g.FillRectangle(backBrush, rect);
        }

        public override void DrawAppointment(System.Drawing.Graphics g, System.Drawing.Rectangle rect, Calendar.Appointment appointment, bool isSelected, System.Drawing.Rectangle gripRect)
        {
            if (appointment == null)
                throw new ArgumentNullException("appointment");

            if (g == null)
                throw new ArgumentNullException("g");

            if (rect.Width != 0 && rect.Height != 0)
            {
				bool longAppt = appointment.IsLongAppt();

				if (!longAppt && (appointment.StartDate.TimeOfDay.TotalHours == 0.0))
				{
					rect.Y++;
					rect.Height--;
				}

				rect.Width--;

                using (StringFormat format = new StringFormat())
                {
                    format.Alignment = StringAlignment.Near;
                    format.LineAlignment = StringAlignment.Near;

                    // Draw the background of the appointment
					if (isSelected)
					{
						m_SelectionRect.Draw(m_hWnd, g, rect.Left, rect.Top, rect.Width, rect.Height);
					}
					else
					{
						using (SolidBrush brush = new SolidBrush(appointment.FillColor))
							g.FillRectangle(brush, rect);
					}

					//  Draw appointment border if needed
					if (!isSelected && appointment.DrawBorder)
					{
						using (Pen pen = new Pen(appointment.BorderColor, 1))
                            g.DrawRectangle(pen, rect.Left, rect.Top, rect.Width, rect.Height);
					}

					// Draw appointment icon
					bool hasIcon = false;

					if (m_TaskIcons != null)
					{
						CalendarItem item = appointment as CalendarItem;

						if ((item != null) && (item.IsParent || item.HasIcon) && m_TaskIcons.Get(item.Id))
						{
							Point ptIcon = new Point(rect.Left + 2, rect.Top + 2);

							if (longAppt)
							{
								ptIcon.X = gripRect.Right;
							}
							else
							{
								gripRect.Y += 18;
								gripRect.Height -= 18;
							}

							m_TaskIcons.Draw(g, ptIcon.X, ptIcon.Y);
							hasIcon = true;

							rect.Width -= (ptIcon.X + 16 - rect.Left);
							rect.X = (ptIcon.X + 16);
						}
					}

                    // Draw gripper bar
					if (gripRect.Width > 0)
					{
						using (SolidBrush brush = new SolidBrush(appointment.BarColor))
							g.FillRectangle(brush, gripRect);

						// Draw gripper border
						using (Pen pen = new Pen(ColorUtil.DarkerDrawing(appointment.BarColor, 0.5f), 1))
							g.DrawRectangle(pen, gripRect);

						if (!hasIcon)
						{
							rect.X = gripRect.Right;
							rect.Width -= (gripRect.Width + 4);
						}
					}
					
					// draw appointment text
                    rect.Y += 3;

					if (longAppt)
						rect.Height = m_BaseFont.Height;
					else
						rect.Height -= 3;

                    g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;

					Color textColor = (isSelected ? ColorUtil.DarkerDrawing(appointment.TextColor, 0.5f) : appointment.TextColor);

					using (SolidBrush brush = new SolidBrush(textColor))
						g.DrawString(appointment.Title, this.BaseFont, brush, rect, format);

                    g.TextRenderingHint = TextRenderingHint.SystemDefault;
                }
            }
        }
    }
}
