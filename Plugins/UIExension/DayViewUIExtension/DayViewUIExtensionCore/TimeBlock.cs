using System;
using System.Diagnostics;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;

namespace DayViewUIExtension
{

	// --------------------------------------------------------------

	public class TimeBlocks
	{
		private Dictionary<uint, TimeBlockSeriesList> m_TaskSeries = new Dictionary<uint, TimeBlockSeriesList>();

		public TimeBlockSeriesList GetTaskSeries(uint taskId, bool autoCreate)
		{
			TimeBlockSeriesList seriesList = null;

			if ((taskId != 0) && !m_TaskSeries.TryGetValue(taskId, out seriesList) && autoCreate)
			{
				seriesList = new TimeBlockSeriesList(taskId);
				m_TaskSeries[taskId] = seriesList;
			}

			return seriesList;
		}

		public void Load(Preferences prefs, String key)
		{
			string prefsKey = (key + "\\TimeBlocks");

			int count = prefs.GetProfileInt(prefsKey, "Count", 0);
			m_TaskSeries.Clear();

			for (int i = 0; i < count; i++)
			{
				var entry = string.Format("Task{0}", i);
				var value = prefs.GetProfileString(prefsKey, entry, "");

				var seriesList = TimeBlockSeriesList.Decode(value);

				if (seriesList != null)
					m_TaskSeries[seriesList.TaskId] = seriesList;
			}
		}

		public void Save(Preferences prefs, String key)
		{
			string prefsKey = (key + "\\TimeBlocks");
			prefs.DeleteProfileSection(prefsKey, true);

			prefs.WriteProfileInt(prefsKey, "Count", m_TaskSeries.Count);

			int i = 0;
			foreach (var task in m_TaskSeries)
			{
				var value = task.Value.Encode();

				if (!string.IsNullOrWhiteSpace(value))
				{
					var entry = string.Format("Task{0}", i++);
					prefs.WriteProfileString(prefsKey, entry, value);
				}
			}
		}
	}

	// --------------------------------------------------------------

	public class TimeBlockSeriesList
	{
		private List<TimeBlockSeries> m_Series = new List<TimeBlockSeries>();

		// ----------------------------

		public IEnumerable<TimeBlockSeries> Series { get { return m_Series; } }
		public uint TaskId { get; private set; }

		// ----------------------------

		public TimeBlockSeriesList(uint taskId)
		{
			Debug.Assert(taskId != 0);

			TaskId = taskId;
		}

		public bool AddSeries(DateTime fromDate,
								DateTime toDate,
								TimeSpan fromTime,
								TimeSpan toTime,
								List<DayOfWeek> days,
								bool syncToTaskDates)
		{
			if ((fromDate > toDate) || (fromTime >= toTime))
				return false;

			var series = new TimeBlockSeries(fromTime, toTime, days, syncToTaskDates);
			var date = fromDate;

			do
			{
				var dates = new Calendar.AppointmentDates((date + fromTime), (date + toTime));

				if (days.Contains(date.DayOfWeek))
					series.AddTimeBlock(dates);

				date = date.AddDays(1);
			}
			while (date.Date <= toDate.Date);

			m_Series.Add(series);
			return true;
		}

		public bool DuplicateBlock(TimeBlock block)
		{
			foreach (var series in m_Series)
			{
				if (series.DuplicateTimeBlock(block))
					return true;
			}

			return false;
		}

		public bool RemoveBlock(TimeBlock block)
		{
			foreach (var series in m_Series)
			{
				if (series.RemoveTimeBlock(block))
				{
					if (series.BlockCount == 0)
						m_Series.Remove(series);

					return true;
				}
			}

			return false;
		}

		public bool RemoveSeries(TimeBlock block)
		{
			foreach (var series in m_Series)
			{
				if (series.RemoveTimeBlock(block))
				{
					m_Series.Remove(series);
					return true;
				}
			}

			return false;
		}

		public string Encode()
		{
			string seriesList = string.Empty;

			foreach (var series in m_Series)
				seriesList = seriesList + series.Encode() + '+';

			if (!string.IsNullOrWhiteSpace(seriesList))
				return string.Format("{0}={1}", TaskId, seriesList.TrimEnd('+'));
			
			return string.Empty;
		}

		static public TimeBlockSeriesList Decode(string blocks)
		{
			var items = blocks.Split('=');
			uint taskId = 0;

			if (items.Length != 2)
				return null;

			if (!uint.TryParse(items[0], out taskId) || (taskId == 0))
				return null;
			
			items = items[1].Split('+');

			if (items.Length == 0)
				return null;

			var seriesList = new TimeBlockSeriesList(taskId);

			foreach (var item in items)
			{
				var series = TimeBlockSeries.Decode(item);

				if (series != null)
					seriesList.m_Series.Add(series);
			}

			if (seriesList.m_Series.Count == 0)
				return null;
			
			return seriesList;
		}

	}

	// --------------------------------------------------------------

	public class TimeBlockSeries
	{
		private List<TimeBlock> m_Blocks = new List<TimeBlock>();

		private int m_FromMinute, m_ToMinute;
		private int m_DaysOfWeek;
		private bool m_SyncToTaskDates = false;

		// ----------------------------
		
		public IEnumerable<TimeBlock> Blocks { get { return m_Blocks; } }
		public int BlockCount { get { return m_Blocks.Count; } }

		// ----------------------------

		public TimeBlockSeries(TimeSpan from, TimeSpan to, List<DayOfWeek> days, bool syncToDates)
		{
			m_FromMinute = (int)from.TotalMinutes;
			m_ToMinute = (int)to.TotalMinutes;
			m_DaysOfWeek = DateUtil.MapDaysOfWeek(days);
			m_SyncToTaskDates = syncToDates;
		}

		public bool AddTimeBlock(Calendar.AppointmentDates dates)
		{
			if (dates.Start >= dates.End)
				return false;

			m_Blocks.Add(new TimeBlock(dates));
			return false;
		}

		public bool RemoveTimeBlock(TimeBlock block)
		{
			return m_Blocks.Remove(block);
		}

		public bool DuplicateTimeBlock(TimeBlock block)
		{
			if (!m_Blocks.Contains(block))
				return false;

			m_Blocks.Add(new TimeBlock(block));
			return true;
		}
		
		public string Encode()
		{
			if (BlockCount == 0)
				return string.Empty;

			string timeBlocks = string.Empty;

			foreach (var block in m_Blocks)
				timeBlocks = timeBlocks + block.Encode() + '|';

			return string.Format("{0}:{1}:{2}:{3}/{4}", 
								m_FromMinute,
								m_ToMinute,
								m_DaysOfWeek,
								(m_SyncToTaskDates ? 1 : 0), timeBlocks.TrimEnd('|'));
		}

		static public TimeBlockSeries Decode(string blocks)
		{
			if (string.IsNullOrWhiteSpace(blocks))
				return null;

			// Split the header from the blocks
			var items = blocks.Split('/');

			if ((items.Length != 2) ||
				string.IsNullOrWhiteSpace(items[0]) ||
				string.IsNullOrWhiteSpace(items[1]))
			{
				return null;
			}

			var header = items[0].Split(':');

			if (header.Length != 4)
				return null;

			int fromMins, toMins;
			int daysOfWeek;
			int syncToDates;

			if (!int.TryParse(header[0], out fromMins) ||
				!int.TryParse(header[1], out toMins) ||
				!int.TryParse(header[2], out daysOfWeek) ||
				!int.TryParse(header[3], out syncToDates))
			{
				return null;
			}

			var series = new TimeBlockSeries(TimeSpan.FromMinutes(fromMins), 
											TimeSpan.FromMinutes(toMins), 
											DateUtil.MapDaysOfWeek(daysOfWeek), 
											(syncToDates != 0));
			var pairs = items[1].Split('|');

			foreach (var pair in pairs)
			{
				var dates = TimeBlock.Decode(pair);

				if (dates != null)
					series.m_Blocks.Add(dates);
			}

			if (series.m_Blocks.Count == 0)
				return null;

			return series;
		}
	}

	// ---------------------------------------------------------

	public class TimeBlock : Calendar.AppointmentDates
	{
		public TimeBlock(DateTime start, DateTime end)
		{
			Start = start;
			End = end;
		}

		public TimeBlock(Calendar.AppointmentDates dates)
			:
			this(dates.Start, dates.End)
		{
		}

		private const long TicksPerMinute = (60 * 10000000);
		private const long MinutesPerDay = (60 * 24);

		public string Encode()
		{
			long startMin = (Start.Ticks / TicksPerMinute);
			long lenMinutes = (Length.Ticks / TicksPerMinute);

			return string.Format("{0}:{1}", startMin, lenMinutes);
		}

		public static TimeBlock Decode(string block)
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

			return new TimeBlock(startDate, endDate);
		}
	}
}
