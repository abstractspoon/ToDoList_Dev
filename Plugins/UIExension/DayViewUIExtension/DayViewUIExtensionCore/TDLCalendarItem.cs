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
	public class TDLAppointmentView : Calendar.AppointmentView
	{
		public TDLAppointmentView(Calendar.Appointment appt, Rectangle rect, Rectangle gripRect,
								bool isLong, bool drawLongContinuous, int endOfStart, int startOfEnd) 
			: 
			base(appt, rect, gripRect, isLong, drawLongContinuous, endOfStart, startOfEnd)
		{
			IconRect = Rectangle.Empty;
			TextHorzOffset = 0;
		}

		public Rectangle IconRect;
		public int TextHorzOffset;
	}

	// ---------------------------------------------------------------

	public class TimeBlockHelper
	{
		private const long TicksPerMinute = (60 * 10000000);
		private const long MinutesPerDay = (60 * 24);

		private static string EncodeTimeBlock(Calendar.AppointmentDates block)
		{
			long startMin = (block.Start.Ticks / TicksPerMinute);
			long lenMinutes = (block.Length.Ticks / TicksPerMinute);

			return string.Format("{0}:{1}", startMin, lenMinutes);
		}

		public static string EncodeTimeBlocks(List<Calendar.AppointmentDates> blocks)
		{
			string timeBlocks = String.Empty;

			if (blocks != null)
			{
				foreach (var block in blocks)
				{
					timeBlocks = timeBlocks + EncodeTimeBlock(block) + '|';
				}
			}

			return timeBlocks;
		}

		private static Calendar.AppointmentDates DecodeTimeBlock(string block)
		{
			var parts = block.Split(':');

			if (parts.Length != 2)
				return null;

			long startMin = 0, lenMinutes = 0;

			if (!long.TryParse(parts[0], out startMin) || !long.TryParse(parts[1], out lenMinutes))
				return null;

			if ((startMin < 0) || (lenMinutes <= 0))
				return null;

			var startDate = new DateTime(startMin * TicksPerMinute);
			var endDate = new DateTime((startMin + lenMinutes) * TicksPerMinute);

			if (endDate <= startDate)
				return null;

			return new Calendar.AppointmentDates(startDate, endDate);
		}

		public static List<Calendar.AppointmentDates> DecodeTimeBlocks(string blocks)
		{
			List<Calendar.AppointmentDates> timeBlocks = null;

			if (!string.IsNullOrWhiteSpace(blocks))
			{
				var pairs = blocks.Split(new char[1] { '|' }, StringSplitOptions.RemoveEmptyEntries);

				foreach (var pair in pairs)
				{
					var dates = DecodeTimeBlock(pair);

					if (dates != null)
					{
						if (timeBlocks == null)
							timeBlocks = new List<Calendar.AppointmentDates>();

						timeBlocks.Add(dates);
					}
				}
			}

			return timeBlocks;
		}
	}

	// ---------------------------------------------------------------

	public class TaskItem : Calendar.Appointment
	{
		private Calendar.AppointmentDates m_OrgDates = new Calendar.AppointmentDates();
		private DateTime m_PrevDueDate = NullDate;

		private Color m_TaskTextColor = Color.Empty;
		private List<string> m_Tags = null;
		private List<Calendar.AppointmentDates> m_TimeBlocks;

		// --------------------

		public TaskItem()
		{
		}

		public Dictionary<string, DateTime> CustomDates
		{
			get; private set;
		}

		public IEnumerable<Calendar.AppointmentDates> TimeBlocks
		{
			get { return m_TimeBlocks; }
		}

		public Boolean HasTaskTextColor
		{
			get { return !m_TaskTextColor.IsEmpty; }
		}

		public Color TaskTextColor
		{
			get
			{
				if (m_TaskTextColor.IsEmpty)
					return base.TextColor;

				return m_TaskTextColor;
			}
			set { m_TaskTextColor = value; }
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

		public String AllocTo { get; set; }
		public Boolean IsParent { get; set; }
        public Boolean HasIcon { get; set; }
        public Boolean IsDone { get; set; }
        public Boolean IsGoodAsDone { get; set; }
        public Boolean IsDoneOrGoodAsDone { get { return IsDone || IsGoodAsDone; } }
		public Boolean HasDependencies { get; set; }
		public Boolean IsRecurring { get; set; }
		public double TimeEstimate { get; set; }
        public Task.TimeUnits TimeEstUnits { get; set; }

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

		public bool UpdateTaskAttributes(Task task, List<CustomAttributeDefinition> dateAttribs, UIExtension.UpdateType type, bool newTask, string metaDataKey)
		{
			if (!task.IsValid())
				return false;

			UInt32 taskID = task.GetID();

			if (newTask)
			{
				Title = task.GetTitle();
				AllocTo = String.Join(", ", task.GetAllocatedTo());
				HasIcon = task.HasIcon();
				Id = taskID;
				IsParent = task.IsParent();
				TaskTextColor = task.GetTextDrawingColor();
				DrawBorder = true;
				Locked = task.IsLocked(true);
				HasDependencies = (task.GetDependency().Count > 0);
				IsRecurring = task.IsRecurring();

				m_Tags = task.GetTag();

				Task.TimeUnits units = Task.TimeUnits.Unknown;
				TimeEstimate = task.GetTimeEstimate(ref units, false);
				TimeEstUnits = units;

				StartDate = task.GetStartDate(false);
				IsDone = task.IsDone();
                IsGoodAsDone = task.IsGoodAsDone();

				m_PrevDueDate = CheckGetEndOfDay(task.GetDueDate(false));
				EndDate = (IsDone ? CheckGetEndOfDay(task.GetDoneDate()) : m_PrevDueDate);

				UpdateCustomDateAttributes(task, dateAttribs);
				m_TimeBlocks = TimeBlockHelper.DecodeTimeBlocks(task.GetMetaDataValue(metaDataKey));
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

				if (task.IsAttributeAvailable(Task.Attribute.AllocatedTo))
					AllocTo = String.Join(", ", task.GetAllocatedTo());

				if (task.IsAttributeAvailable(Task.Attribute.Icon))
					HasIcon = task.HasIcon();

				if (task.IsAttributeAvailable(Task.Attribute.Tags))
					m_Tags = task.GetTag();

				if (task.IsAttributeAvailable(Task.Attribute.Recurrence))
					IsRecurring = task.IsRecurring();

				if (task.IsAttributeAvailable(Task.Attribute.Dependency))
					HasDependencies = (task.GetDependency().Count > 0);

				TaskTextColor = task.GetTextDrawingColor();
				Locked = task.IsLocked(true);

                // Dates
                bool hadValidDates = HasValidDates();

				if (task.IsAttributeAvailable(Task.Attribute.StartDate))
					StartDate = task.GetStartDate(false);

				if (task.IsAttributeAvailable(Task.Attribute.DueDate))
				{
					m_PrevDueDate = task.GetDueDate(false); // always

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

		public bool AddTimeBlock(DateTime start, DateTime end)
		{
			if (start >= end)
			{
				Debug.Assert(false);
				return false;
			}

			if (m_TimeBlocks == null)
				m_TimeBlocks = new List<Calendar.AppointmentDates>();

			m_TimeBlocks.Add(new Calendar.AppointmentDates(start, end));

			return true;
		}

		public bool DeleteTimeBlock(Calendar.AppointmentDates dates)
		{
			if (m_TimeBlocks != null)
				return m_TimeBlocks.Remove(dates);

			return false;
		}

		static public DateTime CheckGetEndOfDay(DateTime date)
		{
			if ((date != NullDate) && (date == date.Date))
				return date.AddDays(1).AddSeconds(-1);

			// else
			return date;
		}

		public string EncodeTimeBlocks()
		{
			return TimeBlockHelper.EncodeTimeBlocks(m_TimeBlocks);
		}
	}

	// ---------------------------------------------------------------

	public class TaskExtensionItem : Calendar.Appointment
	{
		protected TaskItem m_RealItem;

		protected TaskExtensionItem(TaskItem item, UInt32 id) : base(item)
		{
			m_RealItem = item;
			Id = id;
			TextColor = SystemColors.ControlDarkDark; // GetSysColor(COLOR_3DDKSHADOW);
		}

		public UInt32 RealTaskId { get { return m_RealItem.Id; } }
		public TaskItem RealTask { get { return m_RealItem; } }
	}

	// ---------------------------------------------------------------

	public class FutureOccurrence : TaskExtensionItem
	{
		public FutureOccurrence(TaskItem item, UInt32 id, DateTime start, DateTime end) : base(item, id)
		{
			Locked = true; // always (for now)

			StartDate = start;
			EndDate = TaskItem.CheckGetEndOfDay(end);
		}

		public override bool IsLongAppt(DateTime start, DateTime end)
		{
			return RealTask.IsLongAppt(start, end);
		}
	}

	// ---------------------------------------------------------------

	public class CustomDateAttribute : TaskExtensionItem
	{
		public CustomDateAttribute(TaskItem item, UInt32 id, string attribId, DateTime date) : base(item, id)
		{
			AttributeId = attribId;
			StartDate = OriginalDate = date;
			EndDate = StartDate.AddDays(1);

			if (EndDate == EndDate.Date)
				EndDate = EndDate.AddSeconds(-1);
		}

		public override bool IsLongAppt(DateTime start, DateTime end)
		{
			return true; // always 24 hours
		}

		public void RestoreOriginalDate()
		{
			RealTask.CustomDates[AttributeId] = OriginalDate;
		}

		public void UpdateTaskDate()
		{
			RealTask.CustomDates[AttributeId] = StartDate;
		}

		public void ClearDate()
		{
			RealTask.CustomDates[AttributeId] = DateTime.MinValue;
		}

		public string AttributeId { get; private set; }
		public DateTime OriginalDate { get; private set; }
	}

	// ---------------------------------------------------------------

	public class TimeBlock : TaskExtensionItem
	{
		private Calendar.AppointmentDates m_OrgDates = new Calendar.AppointmentDates();
		private Calendar.AppointmentDates m_Dates = null; // Reference back to task 

		public TimeBlock(TaskItem item, UInt32 id, Calendar.AppointmentDates dates) : base(item, id)
		{
			Locked = false; // Never

			// Copy dates
			StartDate = m_OrgDates.Start = dates.Start;
			EndDate = m_OrgDates.End = TaskItem.CheckGetEndOfDay(dates.End);

			// Reference back to original
			m_Dates = dates;
		}

		public override bool IsLongAppt(DateTime start, DateTime end)
		{
			return false; // Always
		}

		public void RestoreOriginalDates()
		{
			m_Dates.Start = m_OrgDates.Start;
			m_Dates.End = m_OrgDates.End;
		}

		public void UpdateTaskDates()
		{
			m_Dates.Start = StartDate;
			m_Dates.End = EndDate;
		}

		public void DeleteBlock()
		{
			RealTask.DeleteTimeBlock(m_Dates);
		}
	}

}
