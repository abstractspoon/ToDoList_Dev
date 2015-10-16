

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Calendar
{
    public class Appointment
    {
        public Appointment()
        {
            color = Color.White;
            borderColor = Color.Blue;
            title = "New Appointment";
        }

        private int layer;

        public int Layer
        {
            get { return layer; }
            set { layer = value; }
        }

        private string group;

        public string Group
        {
            get { return group; }
            set { group = value; }
        }

        private DateTime startDate;

        public DateTime StartDate
        {
            get
            {
                return startDate;
            }
            set
            {
                startDate = value;
                OnStartDateChanged();

            }
        }
        protected virtual void OnStartDateChanged()
        {
        }

        private DateTime endDate;

        public DateTime EndDate
        {
            get
            {
                return endDate;
            }
            set
            {
                endDate = value;
                OnEndDateChanged();
            }
        }
        protected virtual void OnEndDateChanged()
        {
        }

        private bool locked;

        [System.ComponentModel.DefaultValue(false)]
        public bool Locked
        {
            get
            {
                return locked;
            }
            set
            {
                locked = value;
                OnLockedChanged();
            }
        }

        protected virtual void OnLockedChanged()
        {
        }

        private Color color = Color.White;

        public Color Color
        {
            get
            {
                return color;
            }
            set
            {
                color = value;
                OnColorChanged();
            }
        }

        protected virtual void OnColorChanged()
        {
        }

        private Color textColor = Color.Black;

        public Color TextColor
        {
            get
            {
                return textColor;
            }
            set
            {
                textColor = value;
                OnTextColorChanged();
            }
        }

        protected virtual void OnTextColorChanged()
        {
        }

        private bool drawBorder = false;

        public bool DrawBorder
        {
            get
            {
                return drawBorder;
            }

            set
            {
                drawBorder = value;
            }
        }
       

        private Color borderColor = Color.Blue;

        public Color BorderColor
        {
            get
            {
                return borderColor;
            }
            set
            {
                borderColor = value;
                OnBorderColorChanged();
            }
        }

        protected virtual void OnBorderColorChanged()
        {
        }

        private string title = "";

        [System.ComponentModel.DefaultValue("")]
        public string Title
        {
            get
            {
                return title;
            }
            set
            {
                title = value;
                OnTitleChanged();
            }
        }

        protected virtual void OnTitleChanged()
        {
        }

        private bool allDayEvent = false;

        public bool AllDayEvent
        {
            get
            {
                return allDayEvent;
            }
            set
            {
                allDayEvent = value;
                OnAllDayEventChanged();
            }
        }

        protected virtual void OnAllDayEventChanged()
        {
        }
        internal int conflictCount;
    }
}
