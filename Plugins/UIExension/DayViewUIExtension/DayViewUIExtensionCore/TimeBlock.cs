using System;
using System.Collections.Generic;

namespace DayViewUIExtension
{

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

		private string Encode()
		{
			long startMin = (Start.Ticks / TicksPerMinute);
			long lenMinutes = (Length.Ticks / TicksPerMinute);

			return string.Format("{0}:{1}", startMin, lenMinutes);
		}

		public static string EncodeTimeBlocks(uint taskId, List<TimeBlock> blocks)
		{
			if (blocks != null)
			{
				string timeBlocks = String.Empty;

				foreach (var block in blocks)
					timeBlocks = timeBlocks + block.Encode() + '|';

				if (!string.IsNullOrWhiteSpace(timeBlocks))
					return string.Format("{0}={1}", taskId, timeBlocks);
			}

			// all else
			return string.Empty;
		}

		private static TimeBlock DecodeTimeBlock(string block)
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

		public static bool DecodeTimeBlocks(string blocks, out uint taskId, out List<TimeBlock> timeBlocks)
		{
			taskId = 0;
			timeBlocks = null;

			if (!string.IsNullOrWhiteSpace(blocks))
			{
				// Split the task id from the blocks
				var items = blocks.Split('=');

				if (items.Length == 2)
				{
					if (uint.TryParse(items[0], out taskId) && (taskId > 0))
					{
						timeBlocks = DecodeTimeBlocks(items[1]);
					}
				}
			}

			return ((taskId > 0) && (timeBlocks != null));
		}

		public static List<TimeBlock> DecodeTimeBlocks(string blocks)
		{
			List<TimeBlock> timeBlocks = null;

			if (!string.IsNullOrWhiteSpace(blocks))
			{
				var pairs = blocks.Split(new char[1] { '|' }, StringSplitOptions.RemoveEmptyEntries);

				foreach (var pair in pairs)
				{
					var dates = DecodeTimeBlock(pair);

					if (dates != null)
					{
						if (timeBlocks == null)
							timeBlocks = new List<TimeBlock>();

						timeBlocks.Add(dates);
					}
				}
			}

			return timeBlocks;
		}
	}
}
