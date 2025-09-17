

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Calendar
{
	public class AppointmentDates
	{
		public DateTime Start;
		public DateTime End;

		public AppointmentDates()
		{
			Start = new System.DateTime(0L);
			End = new System.DateTime(0L);
		}

		public AppointmentDates(AppointmentDates other)
		{
			Start = other.Start;
			End = other.End;
		}

		public AppointmentDates(DateTime start, DateTime end)
		{
			Start = start;
			End = end;
		}

		public bool IsValid
		{
			get
			{
				return ((Start != NullDate) &&
						(End != NullDate) &&
						(End > Start));
			}
		}

		public bool Intersects(AppointmentDates other)
		{
			if (!IsValid || !other.IsValid)
				return false;

			if (Start >= other.End)
				return false;

			if (End <= other.Start)
				return false;

			return true;
		}

		public bool IsLongAppt { get { return (Start.Date < End.Date); } }
		public TimeSpan Length { get { return (End - Start); } }

		static public DateTime NullDate { get { return DateTime.MinValue; } }

	}

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
			dates.Start = appt.dates.Start; // copy
			dates.End = appt.dates.End; // copy
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

		static public DateTime NullDate { get { return AppointmentDates.NullDate; } }

		virtual public bool HasValidDates()
		{
			return dates.IsValid;
		}
		
		private AppointmentDates dates = new AppointmentDates();

		public AppointmentDates Dates
		{
			get
			{
				return new AppointmentDates(StartDate, EndDate);
			}
		}

		public bool IntersectsToday
		{
			get
			{
				var today = DateTime.Now.Date;
				return ((today >= StartDate) && (today <= EndDate));
			}
		}

		public virtual DateTime StartDate
        {
            get
            {
                return dates.Start;
            }
            set
            {
				if (dates.Start != value)
				{
					dates.Start = value;
					OnStartDateChanged();
				}
			}
		}

        protected virtual void OnStartDateChanged()
        {
			// for derived classes
		}

        public virtual DateTime EndDate
        {
            get
            {
                return dates.End;
            }
            set
            {
				if (dates.End != value)
				{
					dates.End = value;
					OnEndDateChanged();
				}
			}
		}

		public bool DatesMatch(AppointmentDates other)
		{
			return ((other != null) &&
					(other.Start == StartDate) &&
					(other.End == EndDate));
		}

        protected virtual void OnEndDateChanged()
        {
			// for derived classes
		}

		public virtual TimeSpan Length
        {
            get { return dates.Length; }
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

		private Color fillColor = SystemColors.Window;

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

		private Color textColor = SystemColors.WindowText;

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
                if (borderColor.IsEmpty)
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
            return IsLongAppt(StartDate, EndDate);
        }

		public bool Intersects(Appointment other)
		{
			if (!Dates.Intersects(other.Dates))
				return false;

			if (IsLongAppt() != other.IsLongAppt())
				return false;

			return true;
		}

		public virtual bool IsLongAppt(DateTime start, DateTime end)
		{
			if (!HasValidDates())
				return false;

			return (start.Date < end.Date);
		}

    }

	public class AppointmentView
	{
		public AppointmentView(Appointment appt)
		{
			Appointment = appt;
		}

		public Appointment Appointment;
		public Rectangle Rectangle = Rectangle.Empty;
		public Rectangle GripRect = Rectangle.Empty;
		public bool IsLong = false;
	}

	class AppointmentList : List<Appointment>
	{
		public AppointmentList() : base() { }
		public AppointmentList(IEnumerable<Appointment> appts) : base(appts) { }

		public void SortByStartDate()
		{
			// If two tasks sort the same, attempt to keep things stable 
			// by retaining their existing layer order
			Sort((x, y) => ((x.StartDate < y.StartDate) ? -1 : ((x.StartDate > y.StartDate) ? 1 : (x.Layer - y.Layer))));
		}
	}
}
