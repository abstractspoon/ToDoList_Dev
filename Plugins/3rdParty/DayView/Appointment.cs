

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Calendar
{
    public class Appointment
    {
        public Appointment(string t = "New Appointment")
        {
            barColor = Color.White;
            borderColor = Color.Blue;
            title = t;
        }

        private UInt32 id;

        public UInt32 Id
        {
            get { return id; }
            set { id = value; }
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

        public virtual DateTime StartDate
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

        public virtual DateTime EndDate
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

        public virtual TimeSpan Length
        {
            get { return (endDate - startDate); }
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

        private Color barColor = Color.White;

        public Color BarColor
        {
            get
            {
                if (barColor.IsEmpty)
                    return SystemColors.Window;

                return barColor;
            }
            set
            {
                barColor = value;
                OnColorChanged();
            }
        }

		private Color fillColor = Color.White;

		public Color FillColor
		{
			get
			{
                if (fillColor.IsEmpty)
                    return SystemColors.Window;

				return fillColor;
			}
			set
			{
				fillColor = value;
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
                if (textColor.IsEmpty)
                    return SystemColors.WindowText;

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
                if (textColor.IsEmpty)
                    return SystemColors.ControlDark;

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

        public virtual bool IsLongAppt()
        {
            return IsLongAppt(startDate, endDate);
        }

		static public bool IsLongAppt(DateTime start, DateTime end)
		{
			return (start.Date != end.Date);
		}

    }
}
