using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms.VisualStyles;

using TDLPluginHelpers;

namespace DayViewUIExtension
{
    public class TDLRenderer : Calendar.AbstractRenderer
    {
		private VisualStyleRenderer m_explorerSelection;

		private VisualStyleRenderer ExplorerSelection
		{
			get 
			{
				if (m_explorerSelection == null)
				{
					const int LVP_LISTITEM = 1;
					const int LISS_MORESELECTED = 6;

					m_explorerSelection = new VisualStyleRenderer("Explorer::ListView", LVP_LISTITEM, LISS_MORESELECTED);
				}

				return m_explorerSelection;
			}
		}

        private TDLPluginHelpers.TDLTheme m_theme;

        public TDLPluginHelpers.TDLTheme Theme
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

        public override Color HourColor
        {
            get
            {
                return System.Drawing.Color.FromArgb(230, 237, 247);
            }
        }

        public override Color HalfHourSeperatorColor
        {
            get
            {
                return System.Drawing.Color.FromArgb(213, 215, 241);
            }
        }

        public override Color HourSeperatorColor
        {
            get
            {
                return System.Drawing.Color.FromArgb(165, 191, 225);
            }
        }

        public override Color WorkingHourColor
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
                return m_theme.GetAppColorAsDrawing(TDLPluginHelpers.TDLTheme.AppColor.AppBackLight);
            }
        }

        public override Color SelectionColor
        {
            get
            {
                return System.Drawing.Color.FromArgb(41, 76, 122);
            }
        }

        public Color TextColor
        {
            get
            {
                return m_theme.GetAppColorAsDrawing(TDLPluginHelpers.TDLTheme.AppColor.AppText);
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
                using (Pen pen = new Pen(InterpolateColors(this.TextColor, Color.White, 0.5f)))
                    g.DrawLine(pen, rect.Left, rect.Y, rect.Width, rect.Y);
            }
        }

        public override void DrawDayHeader(System.Drawing.Graphics g, System.Drawing.Rectangle rect, DateTime date)
        {
            if (g == null)
                throw new ArgumentNullException("g");

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

                    using (SolidBrush brush = new SolidBrush(this.BackColor))
                        g.FillRectangle(brush, rect);

                    using (Pen aPen = new Pen(Color.FromArgb(205, 219, 238)))
                        g.DrawLine(aPen, rect.Left, rect.Top + (int)rect.Height / 2, rect.Right, rect.Top + (int)rect.Height / 2);

                    using (Pen aPen = new Pen(Color.FromArgb(141, 174, 217)))
                        g.DrawRectangle(aPen, rect);

                    rect.X += 1;
                    rect.Width -= 1;
                    using (Pen aPen = new Pen(Color.FromArgb(141, 174, 217)))
                        g.DrawRectangle(aPen, rect);

                    Rectangle topPart = new Rectangle(rect.Left + 1, rect.Top + 1, rect.Width - 2, (int)(rect.Height / 2) - 1);
                    Rectangle lowPart = new Rectangle(rect.Left + 1, rect.Top + (int)(rect.Height / 2) + 1, rect.Width - 1, (int)(rect.Height / 2) - 1);

                    using (LinearGradientBrush aGB = new LinearGradientBrush(topPart, Color.FromArgb(228, 236, 246), Color.FromArgb(214, 226, 241), LinearGradientMode.Vertical))
                        g.FillRectangle(aGB, topPart);

                    using (LinearGradientBrush aGB = new LinearGradientBrush(lowPart, Color.FromArgb(194, 212, 235), Color.FromArgb(208, 222, 239), LinearGradientMode.Vertical))
                        g.FillRectangle(aGB, lowPart);

                    if (date.Date.Equals(DateTime.Now.Date))
                    {
                        topPart.Inflate((int)(-topPart.Width / 4 + 1), 1); //top left orange area
                        topPart.Offset(rect.Left - topPart.Left + 1, 1);
                        topPart.Inflate(1, 0);
                        using (LinearGradientBrush aGB = new LinearGradientBrush(topPart, Color.FromArgb(247, 207, 114), Color.FromArgb(251, 230, 148), LinearGradientMode.Horizontal))
                        {
                            topPart.Inflate(-1, 0);
                            g.FillRectangle(aGB, topPart);
                        }

                        topPart.Offset(rect.Right - topPart.Right, 0);        //top right orange
                        topPart.Inflate(1, 0);
                        using (LinearGradientBrush aGB = new LinearGradientBrush(topPart, Color.FromArgb(251, 230, 148), Color.FromArgb(247, 207, 114), LinearGradientMode.Horizontal))
                        {
                            topPart.Inflate(-1, 0);
                            g.FillRectangle(aGB, topPart);
                        }

                        using (Pen aPen = new Pen(Color.FromArgb(128, 240, 154, 30))) //center line
                            g.DrawLine(aPen, rect.Left, topPart.Bottom - 1, rect.Right, topPart.Bottom - 1);

                        topPart.Inflate(0, -1);
                        topPart.Offset(0, topPart.Height + 1); //lower right
                        using (LinearGradientBrush aGB = new LinearGradientBrush(topPart, Color.FromArgb(240, 157, 33), Color.FromArgb(250, 226, 142), LinearGradientMode.BackwardDiagonal))
                            g.FillRectangle(aGB, topPart);

                        topPart.Offset(rect.Left - topPart.Left + 1, 0); //lower left
                        using (LinearGradientBrush aGB = new LinearGradientBrush(topPart, Color.FromArgb(240, 157, 33), Color.FromArgb(250, 226, 142), LinearGradientMode.ForwardDiagonal))
                            g.FillRectangle(aGB, topPart);
                        using (Pen aPen = new Pen(Color.FromArgb(238, 147, 17)))
                            g.DrawRectangle(aPen, rect);
                    }

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
            using (SolidBrush backBrush = new SolidBrush(Color.FromArgb(0xe6, 0xed, 0xf7)))
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
				rect.X += 7;
				rect.Width -= 7;

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

					using (SolidBrush brush = new SolidBrush(appointment.BarColor))
						g.FillRectangle(brush, gripRect);

					// Draw gripper border
					using (Pen m_Pen = new Pen(TDLColor.DarkerDrawing(appointment.BarColor, 0.5f), 1))
						g.DrawRectangle(m_Pen, gripRect);
					
					//  Draw appointment border if needed
					if (!isSelected && appointment.DrawBorder)
					{
						using (Pen m_Pen = new Pen(appointment.BorderColor, 1))
							g.DrawRectangle(m_Pen, rect);
					}

					// Draw shadow lines
					int xLeft = rect.X + 6;
					int xRight = rect.Right + 1;
					int yTop = rect.Y + 1;
					int yButton = rect.Bottom + 1;

					for (int i = 0; i < 5; i++)
					{
						using (Pen shadow_Pen = new Pen(Color.FromArgb(70 - 12 * i, Color.Black)))
						{
							g.DrawLine(shadow_Pen, xLeft + i, yButton + i, xRight + i - 1, yButton + i); //horizontal lines
							g.DrawLine(shadow_Pen, xRight + i, yTop + i, xRight + i, yButton + i); //vertical
						}
					}

					// draw appointment text

                    rect.X = gripRect.Right + 2;
                    // width of shadow is 6.
                    rect.Width -= 6;

                    g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;

					Color textColor = (isSelected ? TDLColor.DarkerDrawing(appointment.TextColor, 0.5f) : appointment.TextColor);

					using (SolidBrush brush = new SolidBrush(textColor))
						g.DrawString(appointment.Title, this.BaseFont, brush, rect, format);

                    g.TextRenderingHint = TextRenderingHint.SystemDefault;
                }
            }
        }
    }
}
