

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

		protected Appointment(Appointment appt)
		{
            if (appt == null)
                return;

			title = appt.title;
			startDate = appt.startDate;
			endDate = appt.endDate;
			locked = appt.locked;

			Id = appt.Id;
			Layer = appt.Layer;
			Group = appt.Group;
			DrawBorder = appt.DrawBorder;

			barColor = appt.barColor;
			borderColor = appt.borderColor;
			fillColor = appt.fillColor;
			textColor = appt.textColor;
		}

        public UInt32 Id { get; set; }
        public int Layer { get; set; }
        public string Group { get; set; }
		public bool DrawBorder { get; set; }

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
			// for derived classes
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
			// for derived classes
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
			// for derived classes
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
			// for derived classes
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
			// for derived classes
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
			// for derived classes
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
			// for derived classes
		}

		public virtual bool IsLongAppt()
        {
            return IsLongAppt(startDate, endDate);
        }

		public bool IntersectsWith(Appointment other)
		{
			if (StartDate >= other.EndDate)
				return false;

			if (EndDate <= other.StartDate)
				return false;

			return true;
		}

		static public bool IsLongAppt(DateTime start, DateTime end)
		{
			return (start.Date != end.Date);
		}

    }
}
