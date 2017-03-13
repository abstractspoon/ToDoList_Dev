

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

//         public override bool Equals(object obj)
//         {
//             Appointment appt = obj as Appointment;
// 
//             if (appt == null)
//                 return false;
// 
//             if (base.Equals(appt))
//                 return true;
// 
//             // Check ID for efficiency
//             if ((id != 0) && (appt.id != 0))
//                 return (id == appt.id);
// 
//             // else value comparison
//             return ((title == appt.title) &&
//                     (layer == appt.layer) &&
//                     (group == appt.group) &&
//                     (startDate == appt.startDate) &&
//                     (endDate == appt.endDate) &&
//                     (locked == appt.locked) &&
//                     (barColor == appt.barColor) &&
//                     (textColor == appt.textColor) &&
//                     (allDayEvent == appt.allDayEvent));
//         }

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

        private Color barColor = Color.White;

        public Color BarColor
        {
            get
            {
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
