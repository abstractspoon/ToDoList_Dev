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
    public class TDLDayView : Calendar.DayView
    {
        private TDLRenderer m_Renderer;

		System.Windows.Forms.Timer m_RedrawTimer;

        public TDLDayView(System.Drawing.Font font, UIExtension.TaskIcon taskIcons)
        {
            m_Renderer = new TDLRenderer(Handle, taskIcons);

            InitializeComponent(font);
        }

        protected void InitializeComponent(System.Drawing.Font font)
        {
            Calendar.DrawTool drawTool = new Calendar.DrawTool();
            drawTool.DayView = this;

            this.ActiveTool = drawTool;
            this.AllowInplaceEditing = true;
            this.AllowNew = true;
            this.AmPmDisplay = true;
            this.Anchor = (System.Windows.Forms.AnchorStyles.Bottom |
                                     System.Windows.Forms.AnchorStyles.Left |
                                     System.Windows.Forms.AnchorStyles.Right);
            this.AppHeightMode = Calendar.DayView.AppHeightDrawMode.TrueHeightAll;
            this.DaysToShow = 7;
            this.DrawAllAppBorder = false;
            this.Font = font;
            this.Location = new System.Drawing.Point(0, 0);
            this.MinHalfHourApp = false;
            this.Name = "m_dayView";
            this.Renderer = m_Renderer;
            this.SelectionEnd = new System.DateTime(((long)(0)));
            this.SelectionStart = new System.DateTime(((long)(0)));
            this.Size = new System.Drawing.Size(798, 328);
            this.SlotsPerHour = 4;
            this.TabIndex = 0;
            this.Text = "m_dayView";
            this.WorkingHourEnd = 19;
            this.WorkingHourStart = 9;
            this.WorkingMinuteEnd = 0;
            this.WorkingMinuteStart = 0;
        }

        public int GetFontHeight()
        {
            return m_Renderer.GetFontHeight();
        }

   		public void SetUITheme(UITheme theme)
		{
            m_Renderer.Theme = theme;
            Invalidate(true);
		}

		public override DateTime GetDateAt(int x, bool longAppt)
		{
			DateTime date = base.GetDateAt(x, longAppt);

			if (longAppt && (date >= EndDate))
			{
				date = EndDate.AddSeconds(-1);
			}

			return date;
		}

        public override TimeSpan GetTimeAt(int y)
        {
            TimeSpan time = base.GetTimeAt(y);
            
            if (time == new TimeSpan(1, 0, 0, 0))
                time = time.Subtract(new TimeSpan(0, 0, 1));

            return time;
        }

		public void OnUpdateTasks()
		{
			StartUpdateTimer();
		}

		protected void StartUpdateTimer()
		{
			if (m_RedrawTimer == null)
			{
				m_RedrawTimer = new System.Windows.Forms.Timer();
			}

			m_RedrawTimer.Tick += OnUpdateTimer;
			m_RedrawTimer.Interval = 10;
			m_RedrawTimer.Start();
		}

		protected void OnUpdateTimer(object sender, EventArgs e)
		{
			m_RedrawTimer.Stop();

			AdjustScrollbar();
			Invalidate();
			Update();
		}

		protected override void DrawAppointment(Graphics g, Rectangle rect, Calendar.Appointment appointment, bool isSelected, Rectangle gripRect)
		{
			// Our custom gripper bar
			gripRect = rect;
			gripRect.Inflate(-2, -2);
			gripRect.Width = 5;
			gripRect.Height--;

			// If the start date precedes the start of the week then extend the
			// draw rect to the left so the edge is clipped and likewise for the right.
			Rectangle clipRect = rect;

			if (appointment.StartDate < StartDate)
			{
				// Enough to hide the grip Rect
				int offset = (gripRect.Right - rect.Left);

				rect.X -= offset;
				rect.Width += offset;

				gripRect.Width = 0;
			}
			else if (appointment.EndDate >= EndDate)
			{
				rect.Width += 5;
			}
			
			m_Renderer.DrawAppointment(g, rect, appointment, isSelected, gripRect);
		}
	}
}
