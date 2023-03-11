

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

		public AppointmentDates(DateTime start, DateTime end)
		{
			Start = start;
			End = end;
		}

		public TimeSpan Length { get { return (End - Start); } }
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

		static public DateTime NullDate { get { return DateTime.MinValue; } }

		public bool HasValidDates()
		{
			return ((StartDate != NullDate) &&
					(EndDate != NullDate) &&
					(EndDate > StartDate));
		}
		
		private AppointmentDates dates = new AppointmentDates();

        public virtual DateTime StartDate
        {
            get
            {
                return dates.Start;
            }
            set
            {
                dates.Start = value;
                OnStartDateChanged();
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
                dates.End = value;
                OnEndDateChanged();
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
            return IsLongAppt(dates.Start, dates.End);
        }

		public bool IntersectsWith(Appointment other, bool displayLongAppointmentsContinuous)
		{
			if (!HasValidDates() || !other.HasValidDates())
				return false;

			if (IsLongAppt() != other.IsLongAppt())
				return false;

			if (IsLongAppt() && !displayLongAppointmentsContinuous)
			{
				return ((StartDate.Date == other.StartDate.Date) ||
						(StartDate.Date == other.EndDate.Date) ||
						(EndDate.Date == other.EndDate.Date) ||
						(EndDate.Date == other.StartDate.Date));
			}

			if (StartDate >= other.EndDate)
				return false;

			if (EndDate <= other.StartDate)
				return false;

			return true;
		}

		public virtual bool IsLongAppt(DateTime start, DateTime end)
		{
			if (!HasValidDates())
				return false;

			return (start.Date != end.Date);
		}

    }

	public class AppointmentView
	{
		public AppointmentView(Appointment appt, Rectangle rect, Rectangle gripRect,
								bool isLong, bool drawLongContinuous, int endOfStart, int startOfEnd)
		{
			Appointment = appt;
			Rectangle = rect;
			GripRect = gripRect;
			IsLong = isLong;
			DrawLongContinuous = IsLong ? drawLongContinuous : true;
			EndOfStart = DrawLongContinuous ? -1 : endOfStart;
			StartOfEnd = DrawLongContinuous ? -1 : startOfEnd;
		}

		public Appointment Appointment;
		public Rectangle Rectangle;
		public Rectangle GripRect;

		public bool IsLong;
		public bool DrawLongContinuous;
		public int EndOfStart;
		public int StartOfEnd;
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
