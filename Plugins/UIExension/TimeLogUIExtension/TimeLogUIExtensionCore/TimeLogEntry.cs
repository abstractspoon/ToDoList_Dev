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

namespace TimeLogUIExtension
{
	public class TimeLogEntries : Dictionary<uint, TimeLogEntry>
	{
		public TimeLogEntry GetItem(uint taskId, bool autoCreate = false)
		{
			TimeLogEntry taskItem;

			if (!TryGetValue(taskId, out taskItem) && autoCreate)
			{
				taskItem = new TimeLogEntry();
				Add(taskId, taskItem);
			}

			return taskItem;
		}
	}

	// ---------------------------------------------------------------

/*
	public class DateSortedTasks
	{
		public class TaskList : List<TaskLogEntry>
		{
			public int FindItem(uint taskID)
			{
				return FindIndex(x => (x.Id == taskID));
			}

			public int NextIndex(uint taskID, bool forwards)
			{
				return NextIndex(FindItem(taskID), forwards);
			}

			public int NextIndex(int item, bool forwards)
			{
				if (item == -1)
					return -1;

				item = (forwards ? item + 1 : item - 1);

				if ((item < 0) || (item >= Count))
					return -1;

				return item;
			}
		}

		// ------------------

		private TaskList m_SortedTaskList;
		private TaskLogEntries m_TaskItems; // read-only
		private bool m_NeedsResort = true;

		// ------------------

		public DateSortedTasks(TaskLogEntries items)
		{
			m_TaskItems = items;
			m_SortedTaskList = new TaskList();
			m_NeedsResort = true;
		}

		public TaskList Items
		{
			get
			{
				if (m_SortedTaskList.Count == 0)
					m_SortedTaskList.AddRange(m_TaskItems.Values);

				if (m_NeedsResort)
				{
					m_NeedsResort = false;
					m_SortedTaskList.Sort((a, b) => TaskLogEntry.CompareDates(a, b));
				}

				return m_SortedTaskList;
			}
		}

		public void SetNeedsRebuild()
		{
			m_SortedTaskList.Clear();
			m_NeedsResort = true;
		}

		public void SetNeedsResort()
		{
			m_NeedsResort = true;
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

*/
	// ---------------------------------------------------------------

	public class TimeLogEntry : Calendar.Appointment
	{
		private Calendar.AppointmentDates m_OrgDates = new Calendar.AppointmentDates();
		private DateTime m_PrevDueDate = NullDate;

		private List<string> m_Tags = null;

		// --------------------

		public TimeLogEntry()
		{
		}

		public override string ToString()
		{
			return string.Format("{0} {1}", Id, Title);
		}

		public bool TitleMatches(string[] words, bool caseSensitive, bool wholeWord)
		{
			var compareCase = (caseSensitive ? StringComparison.InvariantCulture : StringComparison.InvariantCultureIgnoreCase);

			foreach (var word in words)
			{
				if (wholeWord)
				{
					foreach (var tWord in Title.Split(' '))
					{
						if (tWord.Equals(word, compareCase))
							return true;
					}
				}
				else
				{
					if (Title.IndexOf(word, compareCase) >= 0)
						return true;
				}
			}

			return false;
		}

		public static int CompareDates(Calendar.Appointment a, Calendar.Appointment b)
		{
			int cmp = a.StartDate.CompareTo(b.StartDate);

			if (cmp != 0)
				return cmp;

			cmp = a.EndDate.CompareTo(b.EndDate);

			if (cmp != 0)
				return cmp;

			return a.Id.CompareTo(b.Id); // for stable sort
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
		public bool IsTopLevel { get; private set; }
		public bool HasIcon { get; private set; }
        public bool IsDone { get; private set; }
        public bool IsGoodAsDone { get; private set; }
		public bool HasDependencies { get; private set; }
		public bool IsRecurring { get; private set; }
		public double TimeEstimate { get; private set; }
        public Task.TimeUnits TimeEstUnits { get; private set; }
		public int Depth { get; private set; }
		public Color TaskTextColor { get; private set; }

		public bool IsDoneOrGoodAsDone { get { return IsDone || IsGoodAsDone; } }

		public bool HasTag(string tag)
		{
			if ((m_Tags == null) || string.IsNullOrWhiteSpace(tag))
				return false;

			// case-insensitive search
			return (m_Tags.FindIndex(item => string.Equals(item, tag, StringComparison.InvariantCultureIgnoreCase)) != -1);
		}

        public TimeSpan OriginalLength
        {
            get
            {
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

		public override bool IsLongAppt(DateTime start, DateTime end)
		{
			return false;
		}

		public bool UpdateTaskAttributes(Task task, List<CustomAttributeDefinition> dateAttribs, UIExtension.UpdateType type, bool newTask, string metaDataKey, int depth)
		{
			if (!task.IsValid())
				return false;

/*
			UInt32 taskID = task.GetID();

			// Always
			Position = task.GetPositionString();
			TaskTextColor = task.GetTextDrawingColor();
			Locked = task.IsLocked(true);
			IsParent = task.IsParent();
			IsTopLevel = (task.GetParentID() == 0);

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

				base.StartDate = task.GetStartDate(IsParent);

				IsDone = task.IsDone();
                IsGoodAsDone = task.IsGoodAsDone();

				var dueDate = task.GetDueDate(IsParent);

				m_PrevDueDate = CheckGetEndOfDay(dueDate);
				base.EndDate = (IsDone ? CheckGetEndOfDay(task.GetDoneDate()) : m_PrevDueDate);

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

			}

			UpdateOriginalDates();
*/

			return true;
		}
	}

}
