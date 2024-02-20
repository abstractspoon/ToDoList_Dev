using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{
	public class TaskItems : Dictionary<uint, TaskItem>
	{
		public TaskItem GetItem(uint taskId, bool autoCreate = false)
		{
			TaskItem taskItem;

			if (!TryGetValue(taskId, out taskItem) && autoCreate)
			{
				taskItem = new TaskItem();
				Add(taskId, taskItem);
			}

			return taskItem;
		}
	}

	// ---------------------------------------------------------------

	public class TDLAppointmentView : Calendar.AppointmentView
	{
		public TDLAppointmentView(Calendar.Appointment appt) : base(appt)
		{
		}

		public Rectangle IconRect = Rectangle.Empty;
		public int TextHorzOffset = 0;
		public int EndOfStart = -1, StartOfEnd = -1;
		public int StartOfToday = -1, EndOfToday = -1;
	}

	// ---------------------------------------------------------------

	public class TaskItem : Calendar.Appointment
	{
		private Calendar.AppointmentDates m_OrgDates = new Calendar.AppointmentDates();
		private DateTime m_PrevDueDate = NullDate;

		private Color m_TaskTextColor = Color.Empty;
		private List<string> m_Tags = null;

		// --------------------

		public TaskItem()
		{
		}

		public Dictionary<string, DateTime> CustomDates
		{
			get; private set;
		}

		public bool HasTaskTextColor
		{
			get { return !m_TaskTextColor.IsEmpty; }
		}

		public override string ToString()
		{
			return Title;
		}

		public Color TaskTextColor
		{
			get
			{
// 				if (m_TaskTextColor.IsEmpty)
// 					return base.TextColor;

				return m_TaskTextColor;
			}
			set { m_TaskTextColor = value; }
		}

		private bool IsUsingParentCalcedStartDate;
		private bool IsUsingParentCalcedEndDate;

		public bool IsCalculatedParent
		{
			get
			{
				if (!IsUsingParentCalcedStartDate && !IsUsingParentCalcedEndDate)
					return false;

				Debug.Assert(IsParent);
				return true;
			}
		}

		public void UpdateOriginalDates()
		{
			m_OrgDates.Start = StartDate;
			m_OrgDates.End = EndDate;
		}

		public void RestoreOriginalDates()
		{
			StartDate = m_OrgDates.Start;
			EndDate = m_OrgDates.End;
		}

		public bool EndDateDiffersFromOriginal()
		{
			return ((EndDate - m_OrgDates.End).TotalSeconds != 0.0);
		}

		public bool StartDateDiffersFromOriginal()
		{
			return ((StartDate - m_OrgDates.Start).TotalSeconds != 0.0);
		}

		public string Position { get; private set; }
		public bool IsParent { get; private set; }
        public bool HasIcon { get; private set; }
        public bool IsDone { get; private set; }
        public bool IsGoodAsDone { get; private set; }
		public bool HasDependencies { get; private set; }
		public bool IsRecurring { get; private set; }
		public double TimeEstimate { get; private set; }
        public Task.TimeUnits TimeEstUnits { get; private set; }
		public int Depth { get; private set; }

		public bool IsDoneOrGoodAsDone { get { return IsDone || IsGoodAsDone; } }

		public bool HasTag(string tag)
		{
			if ((m_Tags == null) || string.IsNullOrWhiteSpace(tag))
				return false;

			// case-insensitive search
			return (m_Tags.FindIndex(item => string.Equals(item, tag, StringComparison.InvariantCultureIgnoreCase)) != -1);
		}
		public override TimeSpan Length
        {
            get
            {
                // Handle 'end of day'
                if (IsEndOfDay(EndDate))
                    return (EndDate.Date.AddDays(1) - StartDate);

                return base.Length;
            }
        }

        public TimeSpan OriginalLength
        {
            get
            {
                // Handle 'end of day'
                if (IsEndOfDay(m_OrgDates.End))
                    return (m_OrgDates.End.Date.AddDays(1) - m_OrgDates.Start);

                return m_OrgDates.Length;
            }
        }

		public void RecalcTimeEstimate(WorkingWeek workWeek)
		{
			TimeEstimate = LengthAsTimeEstimate(workWeek, false);
		}

		public bool LengthDiffersFromOriginal()
		{
			return ((Length - OriginalLength).TotalSeconds != 0.0);
		}

        public double LengthAsTimeEstimate(WorkingWeek workWeek, bool original)
        {
			if (!TimeEstimateIsMinsOrHours)
				return 0.0;

			double hours = 0.0;

			if (original)
				hours = workWeek.CalculateDurationInHours(m_OrgDates.Start, m_OrgDates.End);
			else
				hours = workWeek.CalculateDurationInHours(StartDate, EndDate);

			if (TimeEstUnits == Task.TimeUnits.Minutes)
				return (hours * 60);

			// else
            return hours;
        }

		protected override void OnEndDateChanged()
		{
			if (IsUsingParentCalcedEndDate)
				return;

			// Prevent end date being set to exactly midnight
			if ((EndDate != NullDate) && (EndDate == EndDate.Date))
				EndDate = EndDate.AddSeconds(-1);
		}

		public bool TimeEstimateMatchesOriginalLength(WorkingWeek workWeek)
        {
            return (TimeEstimate == LengthAsTimeEstimate(workWeek, true));
        }

        public bool TimeEstimateIsMinsOrHours
        {
            get 
            { 
                return ((TimeEstUnits == Task.TimeUnits.Minutes) || 
                        (TimeEstUnits == Task.TimeUnits.Hours)); 
            }
        }

		public static bool IsEndOfDay(DateTime date)
		{
			return (date == date.Date.AddDays(1).AddSeconds(-1));
		}

		public static bool IsStartOfDay(DateTime date)
		{
			return (date == date.Date);
		}

		public bool IsSingleDay
		{
			get { return !IsLongAppt(); }
		}

		public override bool IsLongAppt(DateTime start, DateTime end)
		{
			if (base.IsLongAppt(start, end))
				return true;

			// Also check for 'end of day' if the start time is midnight
			if ((start.TimeOfDay == TimeSpan.Zero) && IsEndOfDay(end))
				return true;

			return false;
		}

		public bool Intersects(Calendar.Appointment other, bool displayLongAppointmentsContinuous)
		{
			if (!base.Intersects(other))
				return false;

			if (IsLongAppt() && !displayLongAppointmentsContinuous)
			{
				return ((StartDate.Date == other.StartDate.Date) ||
						(StartDate.Date == other.EndDate.Date) ||
						(EndDate.Date == other.EndDate.Date) ||
						(EndDate.Date == other.StartDate.Date));
			}

			return true;
		}

		private void UpdateCustomDateAttributes(Task task, List<CustomAttributeDefinition> dateAttribs)
		{
			CustomDates = new Dictionary<string, DateTime>();

			foreach (var attrib in dateAttribs)
			{
				DateTime date;

				if (DateTime.TryParse(task.GetCustomAttributeValue(attrib.Id, true), out date))
					CustomDates.Add(attrib.Id, date);
			}
		}

		public bool UpdateTaskAttributes(Task task, List<CustomAttributeDefinition> dateAttribs, UIExtension.UpdateType type, bool newTask, string metaDataKey, int depth)
		{
			if (!task.IsValid())
				return false;

			UInt32 taskID = task.GetID();

			// Always
			Position = task.GetPositionString();
			TaskTextColor = task.GetTextDrawingColor();
			Locked = task.IsLocked(true);
			IsParent = task.IsParent();

			if (newTask)
			{
				Title = task.GetTitle();
				HasIcon = task.HasIcon();
				Id = taskID;
				DrawBorder = true;
				HasDependencies = (task.GetDependency().Count > 0);
				IsRecurring = task.IsRecurring();
				Depth = depth;

				m_Tags = task.GetTag();

				Task.TimeUnits units = Task.TimeUnits.Unknown;
				TimeEstimate = task.GetTimeEstimate(ref units, false);
				TimeEstUnits = units;

				StartDate = task.GetStartDate(IsParent);
				IsUsingParentCalcedStartDate = (IsParent && task.HasCalculatedAttribute(Task.Attribute.StartDate));

				IsDone = task.IsDone();
                IsGoodAsDone = task.IsGoodAsDone();

				var dueDate = task.GetDueDate(IsParent);
				IsUsingParentCalcedEndDate = (IsParent && task.HasCalculatedAttribute(Task.Attribute.DueDate));

				m_PrevDueDate = CheckGetEndOfDay(dueDate);
				EndDate = (IsDone ? CheckGetEndOfDay(task.GetDoneDate()) : m_PrevDueDate);

				UpdateCustomDateAttributes(task, dateAttribs);
			}
			else
			{
				if (task.IsAttributeAvailable(Task.Attribute.Title))
					Title = task.GetTitle();

				if (task.IsAttributeAvailable(Task.Attribute.TimeEstimate))
				{
					Task.TimeUnits units = Task.TimeUnits.Unknown;
					TimeEstimate = task.GetTimeEstimate(ref units, false);
					TimeEstUnits = units;
				}

				if (task.IsAttributeAvailable(Task.Attribute.Icon))
					HasIcon = task.HasIcon();

				if (task.IsAttributeAvailable(Task.Attribute.Tags))
					m_Tags = task.GetTag();

				if (task.IsAttributeAvailable(Task.Attribute.Recurrence))
					IsRecurring = task.IsRecurring();

				if (task.IsAttributeAvailable(Task.Attribute.Dependency))
					HasDependencies = (task.GetDependency().Count > 0);

                // Dates
                bool hadValidDates = HasValidDates();

				if (task.IsAttributeAvailable(Task.Attribute.StartDate))
				{
					StartDate = task.GetStartDate(IsParent); // calculated if parent
					IsUsingParentCalcedStartDate = (IsParent && task.HasCalculatedAttribute(Task.Attribute.StartDate));
				}

				if (task.IsAttributeAvailable(Task.Attribute.DueDate))
				{
					var dueDate = task.GetDueDate(IsParent); // calculated if parent
					IsUsingParentCalcedEndDate = (IsParent && task.HasCalculatedAttribute(Task.Attribute.DueDate));

					m_PrevDueDate = dueDate; // always

					if (!IsDone)
						EndDate = CheckGetEndOfDay(m_PrevDueDate);
				}

				if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
				{
					bool wasDone = IsDone;

				    IsDone = task.IsDone();
                    IsGoodAsDone = task.IsGoodAsDone();

					if (IsDone)
					{
						if (!wasDone)
							m_PrevDueDate = CheckGetEndOfDay(EndDate);

						EndDate = CheckGetEndOfDay(task.GetDoneDate());
					}
					else if (wasDone && !IsDone)
					{
						EndDate = CheckGetEndOfDay(m_PrevDueDate);
					}
				}

				if (task.IsAttributeAvailable(Task.Attribute.CustomAttribute))
					UpdateCustomDateAttributes(task, dateAttribs);
			}

			UpdateOriginalDates();

			return true;
		}

		static public DateTime CheckGetEndOfDay(DateTime date)
		{
			if ((date != NullDate) && (date == date.Date))
				return date.AddDays(1).AddSeconds(-1);

			// else
			return date;
		}
	}

}
