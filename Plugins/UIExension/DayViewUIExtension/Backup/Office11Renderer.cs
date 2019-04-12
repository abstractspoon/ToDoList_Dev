using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace Calendar
{
    public class Office11Renderer : AbstractRenderer
    {
        protected override void Dispose(bool mainThread)
        {
            base.Dispose(mainThread);

            if (minuteFont != null)
                minuteFont.Dispose();
        }

        private Font minuteFont;

        public override Font MinuteFont
        {
            get
            {
                if (minuteFont == null)
                    minuteFont = new Font(BaseFont, FontStyle.Italic);

                return minuteFont;
            }
        }

        public override void DrawHourLabel(Graphics g, Rectangle rect, int hour, bool ampm)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            g.DrawString(hour.ToString("##00", System.Globalization.CultureInfo.InvariantCulture), HourFont, SystemBrushes.ControlText, rect);

            rect.X += 27;

            g.DrawString("00", MinuteFont, SystemBrushes.ControlText, rect);
        }

        public override void DrawMinuteLine(Graphics g, Rectangle rect)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            Color m_Color = ControlPaint.LightLight(SystemColors.WindowFrame);

            m_Color = ControlPaint.Light(m_Color);

            using (Pen m_Pen = new Pen(m_Color))
                g.DrawLine(m_Pen, rect.Left, rect.Y, rect.Width, rect.Y);
        }

        public override void DrawDayHeader(Graphics g, Rectangle rect, DateTime date)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            using (StringFormat format = new StringFormat())
            {
                format.Alignment = StringAlignment.Center;
                format.FormatFlags = StringFormatFlags.NoWrap;
                format.LineAlignment = StringAlignment.Center;

                ControlPaint.DrawButton(g, rect, ButtonState.Inactive);
                ControlPaint.DrawBorder3D(g, rect, Border3DStyle.Etched);

                g.DrawString(
                    System.Globalization.CultureInfo.CurrentCulture.DateTimeFormat.GetDayName(date.DayOfWeek),
                    BaseFont,
                    SystemBrushes.WindowText,
                    rect,
                    format
                    );
            }
        }

        public override void DrawDayBackground(Graphics g, Rectangle rect)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            using (Brush m_Brush = new SolidBrush(this.HourColor))
                g.FillRectangle(m_Brush, rect);
        }

        public override void DrawAppointment(Graphics g, Rectangle rect, Appointment appointment, bool isSelected, Rectangle gripRect)
        {
            if (appointment == null)
                throw new ArgumentNullException("appointment");

            if (g == null)
                throw new ArgumentNullException("g");

            if (rect.Width != 0 && rect.Height != 0)
                using (StringFormat format = new StringFormat())
                {
                    format.Alignment = StringAlignment.Near;
                    format.LineAlignment = StringAlignment.Near;

                    if ((appointment.Locked) && isSelected)
                    {
                        // Draw back
                        using (Brush m_Brush = new System.Drawing.Drawing2D.HatchBrush(System.Drawing.Drawing2D.HatchStyle.Wave, Color.LightGray, appointment.Color))
                            g.FillRectangle(m_Brush, rect);
                    }
                    else
                    {
                        // Draw back
                        using (SolidBrush m_Brush = new SolidBrush(appointment.Color))
                            g.FillRectangle(m_Brush, rect);
                    }

                    if (isSelected)
                    {
                        using (Pen m_Pen = new Pen(appointment.BorderColor, 4))
                            g.DrawRectangle(m_Pen, rect);

                        Rectangle m_BorderRectangle = rect;

                        m_BorderRectangle.Inflate(2, 2);

                        using (Pen m_Pen = new Pen(SystemColors.WindowFrame, 1))
                            g.DrawRectangle(m_Pen, m_BorderRectangle);

                        m_BorderRectangle.Inflate(-4, -4);

                        using (Pen m_Pen = new Pen(SystemColors.WindowFrame, 1))
                            g.DrawRectangle(m_Pen, m_BorderRectangle);
                    }
                    else
                    {
                        // Draw gripper
                        gripRect.Width += 1;

                        using (SolidBrush m_Brush = new SolidBrush(appointment.BorderColor))
                            g.FillRectangle(m_Brush, gripRect);

                        using (Pen m_Pen = new Pen(SystemColors.WindowFrame, 1))
                            g.DrawRectangle(m_Pen, rect);
                    }

                    rect.X += gripRect.Width;
                    g.DrawString(appointment.Title, this.BaseFont, SystemBrushes.WindowText, rect, format);
                }
        }
    }

}
