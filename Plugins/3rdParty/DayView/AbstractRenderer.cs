using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace Calendar
{
    public interface IRenderer
    {
		void DrawHourLabel(Graphics g, Rectangle rect, int hour, bool ampm);
		void DrawMinuteLine(Graphics g, Rectangle rect, int minute);
        void DrawDayHeader(Graphics g, Rectangle rect, DateTime date, bool firstDay);
        void DrawDayBackground(Graphics g, Rectangle rect);
        void DrawHourRange(Graphics g, Rectangle rect, bool drawBorder, bool hilight);
        void DrawDayGripper(Graphics g, Rectangle rect, int gripWidth);
        void DrawAppointment(Graphics g, Calendar.AppointmentView apptView, bool isSelected);
		void DrawAllDayBackground(Graphics g, Rectangle rect);

		Color AllDayEventsBackColor();
		Color HourSeperatorColor();
		Color HalfHourSeperatorColor();
		Color HourColor();
		Color WorkingHourColor();
		Color BackColor();
		Color SelectionColor();

		Font BaseFont();
		Font HourFont();
		Font MinuteFont();
	}

	public abstract class AbstractRenderer : IRenderer, IDisposable
    {
        ~AbstractRenderer()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool mainThread)
        {
            if (hourFont != null)
                hourFont.Dispose();

            if (minuteFont != null)
                minuteFont.Dispose();
        }

		// Derived classes must implement -------------------------------------------------
		public abstract void DrawHourLabel(Graphics g, Rectangle rect, int hour, bool ampm);
		public abstract void DrawMinuteLine(Graphics g, Rectangle rect, int minute);
		public abstract void DrawDayHeader(Graphics g, Rectangle rect, DateTime date, bool firstDay);
		public abstract void DrawDayBackground(Graphics g, Rectangle rect);
		public abstract void DrawAppointment(Graphics g, Calendar.AppointmentView apptView, bool isSelected);
		// --------------------------------------------------------------------------------

		public virtual Color AllDayEventsBackColor()
        {
            return InterpolateColors(BackColor(), Color.Black, 0.5f);
        }

        public virtual Font BaseFont()
        {
            return Control.DefaultFont;
        }

        public virtual Color HourSeperatorColor()
        {
            return Color.FromArgb(234, 208, 152);
        }

        public virtual Color HalfHourSeperatorColor()
        {
            return Color.FromArgb(243, 228, 177);
        }

        public virtual Color HourColor()
        {
            return Color.FromArgb(255, 244, 188);
        }

        public virtual Color WorkingHourColor()
        {
            return Color.FromArgb(255, 255, 213);
        }

        public virtual Color BackColor()
        {
            return SystemColors.Control;
        }

        public virtual Color SelectionColor()
        {
            return SystemColors.Highlight;
        }

        private Font hourFont;

        public virtual Font HourFont()
		{
			if (hourFont == null)
			{
				hourFont = new Font(BaseFont().FontFamily.Name, 12, FontStyle.Regular);
			}

			return hourFont;
		}

        private Font minuteFont;

        public virtual Font MinuteFont()
        {
			if (minuteFont == null)
			{
				minuteFont = new Font(BaseFont().FontFamily.Name, 8, FontStyle.Regular);
			}

			return minuteFont;
		}

		public virtual void DrawHourRange(Graphics g, Rectangle rect, bool drawBorder, bool hilight)
        {
			DrawHourRange(g, rect, drawBorder, (hilight ? SelectionColor() : WorkingHourColor()));
        }

        public virtual void DrawDayGripper(Graphics g, Rectangle rect, int gripWidth)
        {
			DrawDayGripper(g, rect, gripWidth, HourSeperatorColor());
        }

        public virtual void DrawAllDayBackground(Graphics g, Rectangle rect)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            using (Brush brush = new SolidBrush(InterpolateColors(BackColor(), Color.Black, 0.5f)))
                g.FillRectangle(brush, rect);
        }

		// static helpers ------------------------------------------------------------------------

		public static void DrawDayGripper(Graphics g, Rectangle rect, int gripWidth, Color color)
		{
            if (g == null)
                throw new ArgumentNullException("g");

			if (gripWidth <= 0)
				return;

			using (Pen pen = new Pen(color))
			{
				if (gripWidth > 2)
				{
					using (Brush brush = new SolidBrush(SystemColors.Window))
						g.FillRectangle(brush, rect.Left, rect.Top - 1, gripWidth, rect.Height);
				}

				if (gripWidth == 1)
					g.DrawLine(pen, rect.Left, rect.Top - 1, rect.Left, rect.Bottom);
				else 
					g.DrawRectangle(pen, rect.Left, rect.Top - 1, gripWidth - 1, rect.Height);
			}

		}

        public static void DrawHourRange(Graphics g, Rectangle rect, bool drawBorder, Color color)
        {
            if (g == null)
                throw new ArgumentNullException("g");

            using (SolidBrush brush = new SolidBrush(color))
            {
                g.FillRectangle(brush, rect);
            }

            if (drawBorder)
                g.DrawRectangle(SystemPens.WindowFrame, rect);
        }

        public static Color InterpolateColors(Color color1, Color color2, float percentage)
        {
            int num1 = ((int)color1.R);
            int num2 = ((int)color1.G);
            int num3 = ((int)color1.B);
            int num4 = ((int)color2.R);
            int num5 = ((int)color2.G);
            int num6 = ((int)color2.B);
            byte num7 = Convert.ToByte(((float)(((float)num1) + (((float)(num4 - num1)) * percentage))));
            byte num8 = Convert.ToByte(((float)(((float)num2) + (((float)(num5 - num2)) * percentage))));
            byte num9 = Convert.ToByte(((float)(((float)num3) + (((float)(num6 - num3)) * percentage))));
            return Color.FromArgb(num7, num8, num9);
        }
    }
}
