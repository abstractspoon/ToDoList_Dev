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

	public class DateSortedTasks
	{
		public class TaskList : List<TaskItem>
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
		private TaskItems m_TaskItems; // read-only
		private bool m_NeedsResort = true;

		// ------------------

		public DateSortedTasks(TaskItems items)
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
					m_SortedTaskList.Sort((a, b) => TaskItem.CompareDates(a, b));
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

	public class TaskItem
	{
		public TaskItem()
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

		public static int CompareDates(TaskItem a, TaskItem b)
		{
			int cmp = a.StartDate.CompareTo(b.StartDate);

			if (cmp != 0)
				return cmp;

			cmp = a.EndDate.CompareTo(b.EndDate);

			if (cmp != 0)
				return cmp;

			return a.Id.CompareTo(b.Id); // for stable sort
		}

		public string Title { get; private set; }
		public string Position { get; private set; }
		public uint Id { get; private set; }
		public DateTime StartDate { get; private set; }
		public DateTime EndDate { get; private set; }
		public bool Locked { get; private set; }
		public bool IsParent { get; private set; }
		public bool HasIcon { get; private set; }
        public bool IsDone { get; private set; }
        public bool IsGoodAsDone { get; private set; }
		public double TimeSpent { get; private set; }
        public Task.TimeUnits TimeSpentUnits { get; private set; }
		public int Depth { get; private set; }
		public Color TaskColor { get; private set; }

		public bool IsDoneOrGoodAsDone { get { return IsDone || IsGoodAsDone; } }

// 		public override TimeSpan Length
//         {
//             get
//             {
//                 // Handle 'end of day'
//                 if (IsEndOfDay(EndDate))
//                     return (EndDate.Date.AddDays(1) - StartDate);
// 
//                 return base.Length;
//             }
//         }

		public static bool IsEndOfDay(DateTime date)
		{
			return (date == EndOfDay(date));
		}

		public static bool IsStartOfDay(DateTime date)
		{
			return (date == date.Date);
		}

/*
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
*/

		public bool UpdateTaskAttributes(Task task, List<CustomAttributeDefinition> dateAttribs, UIExtension.UpdateType type, bool newTask, string metaDataKey, int depth)
		{
			if (!task.IsValid())
				return false;

			UInt32 taskID = task.GetID();

			// Always
			TaskColor = task.GetTextDrawingColor();
			Locked = task.IsLocked(true);
			IsParent = task.IsParent();

			if (newTask)
			{
				Title = task.GetTitle();
				HasIcon = task.HasIcon();
				Id = taskID;
				Depth = depth;

				Task.TimeUnits units = Task.TimeUnits.Unknown;
				TimeSpent = task.GetTimeEstimate(ref units, false);
				TimeSpentUnits = units;

				IsDone = task.IsDone();
                IsGoodAsDone = task.IsGoodAsDone();

				StartDate = task.GetStartDate(false);
				EndDate = CheckGetEndOfDay((IsDone ? task.GetDoneDate() : task.GetDueDate(false)));
			}
			else
			{
				if (task.IsAttributeAvailable(Task.Attribute.Title))
					Title = task.GetTitle();

				if (task.IsAttributeAvailable(Task.Attribute.TimeSpent))
				{
					Task.TimeUnits units = Task.TimeUnits.Unknown;
					TimeSpent = task.GetTimeSpent(ref units, false);
					TimeSpentUnits = units;
				}

				if (task.IsAttributeAvailable(Task.Attribute.Icon))
					HasIcon = task.HasIcon();

                // Dates
				if (task.IsAttributeAvailable(Task.Attribute.StartDate))
					StartDate = task.GetStartDate(false);

				if (task.IsAttributeAvailable(Task.Attribute.DueDate) && !task.IsDone())
					EndDate = CheckGetEndOfDay(task.GetDueDate(false));

				if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
				{
				    IsDone = task.IsDone();
                    IsGoodAsDone = task.IsGoodAsDone();

					if (IsDone)
						EndDate = CheckGetEndOfDay(task.GetDoneDate());
				}
			}

			return true;
		}

		static public DateTime EndOfDay(DateTime date)
		{
			return date.Date.AddDays(1).AddSeconds(-1);
		}

		static public DateTime CheckGetEndOfDay(DateTime date)
		{
			if ((date != Calendar.Appointment.NullDate) && (date == date.Date))
				return EndOfDay(date);

			// else
			return date;
		}
	}

}
