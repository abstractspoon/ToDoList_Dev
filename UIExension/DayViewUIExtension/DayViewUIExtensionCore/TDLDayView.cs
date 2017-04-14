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

        public TDLDayView(System.Drawing.Font font)
        {
            m_Renderer = new TDLRenderer();

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

        public override bool AllowMode(Calendar.SelectionTool.Mode mode)
        {
            switch (mode)
            {
                case Calendar.SelectionTool.Mode.ResizeTop:
                case Calendar.SelectionTool.Mode.ResizeBottom:
                case Calendar.SelectionTool.Mode.Move:
                    return true;
            }

            // all else
            return false;
        }

        public override TimeSpan GetTimeAt(int y)
        {
            TimeSpan time = base.GetTimeAt(y);
            
            if (time == new TimeSpan(1, 0, 0, 0))
                time = time.Subtract(new TimeSpan(0, 0, 1));

            return time;
        }
    }
}
