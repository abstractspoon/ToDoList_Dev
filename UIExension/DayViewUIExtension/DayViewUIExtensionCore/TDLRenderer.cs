using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
    public class TDLRenderer : Calendar.AbstractRenderer
    {
		private VisualStyleRenderer m_ExplorerSelection;
        private VisualStyleRenderer m_HeaderNormal, m_HeaderHot;

		private VisualStyleRenderer ExplorerSelection
		{
			get 
			{
				if (m_ExplorerSelection == null)
				{
					const int LVP_LISTITEM = 1;
					const int LISS_MORESELECTED = 6;

					m_ExplorerSelection = new VisualStyleRenderer("Explorer::ListView", LVP_LISTITEM, LISS_MORESELECTED);
				}

				return m_ExplorerSelection;
			}
		}

        private VisualStyleRenderer HeaderNormal
        {
            get
            {
                if (m_HeaderNormal == null)
                    m_HeaderNormal = new VisualStyleRenderer(VisualStyleElement.Header.Item.Normal);

                return m_HeaderNormal;
            }
        }
        private VisualStyleRenderer HeaderHot
        {
            get
            {
                if (m_HeaderHot == null)
                    m_HeaderHot = new VisualStyleRenderer(VisualStyleElement.Header.Item.Hot);

                return m_HeaderHot;
            }
        }

        private UITheme m_theme;

        public UITheme Theme
        {
            set { m_theme = value; }
        }

        protected override void Dispose(bool mainThread)
        {
            base.Dispose(mainThread);

            if (baseFont != null)
                baseFont.Dispose();
        }

        Font baseFont;

        public override Font BaseFont
        {
            get
            {
                if (baseFont == null)
                {
                    baseFont = new Font("Tahoma", 8, FontStyle.Regular);
                }

                return baseFont;
            }
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
                HeaderHot.DrawBackground(g, rHeader);
            }
            else
            {
                rHeader.X += 1;
                HeaderNormal.DrawBackground(g, rHeader);
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

                    using (Font fntDay = new Font("Tahoma", 8))
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

            /*
             * Logic for drawing the appointment: 
             * 1) Do something messy with the colours
             * 
             * 2) Determine background pattern
             * 2.1) App is locked -> HatchBrush
             * 2.2) Normal app -> Nothing
             * 
             * 3) Draw the background of appointment
             * 
             * 4) Draw the edges of appointment
             * 4.1) If app is selected -> just draw the selection rectangle
             * 4.2) If not -> draw the gripper, border (if required) and shadows
             */

            if (rect.Width != 0 && rect.Height != 0)
            {
				rect.Width--;

                using (StringFormat format = new StringFormat())
                {
                    format.Alignment = StringAlignment.Near;
                    format.LineAlignment = StringAlignment.Near;

                    // Draw the background of the appointment
					if (isSelected)
					{
						g.FillRectangle(System.Drawing.Brushes.White, rect);
						ExplorerSelection.DrawBackground(g, rect);
						ExplorerSelection.DrawBackground(g, rect);
					}
					else
					{
						using (SolidBrush brush = new SolidBrush(appointment.FillColor))
							g.FillRectangle(brush, rect);
					}

                    // Draw gripper bar
					gripRect = rect;
					gripRect.Inflate(-2, -2);
					gripRect.Width = 5;
                    gripRect.Height--;

					using (SolidBrush brush = new SolidBrush(appointment.BarColor))
						g.FillRectangle(brush, gripRect);

					// Draw gripper border
					using (Pen m_Pen = new Pen(ColorUtil.DarkerDrawing(appointment.BarColor, 0.5f), 1))
						g.DrawRectangle(m_Pen, gripRect);
					
					//  Draw appointment border if needed
					if (!isSelected && appointment.DrawBorder)
					{
						using (Pen pen = new Pen(appointment.BorderColor, 1))
                            g.DrawRectangle(pen, rect.Left, rect.Top, rect.Width - 1, rect.Height - 1);
					}

					// draw appointment text
                    rect.X = gripRect.Right/* + 2*/;
                    rect.Y += 2;

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
