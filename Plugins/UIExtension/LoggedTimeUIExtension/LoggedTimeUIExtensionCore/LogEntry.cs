using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Globalization;
using System.IO;
using System.Linq;

using Abstractspoon.Tdl.PluginHelpers;

namespace LoggedTimeUIExtension
{
	public class LogEntry : Calendar.Appointment
	{
		public LogEntry(uint entryId)
		{
			base.Id = entryId;
		}

		public LogEntry(uint entryId, TaskTimeLogEntry logEntry)
			:
			this(entryId,
				logEntry.TaskId,
				logEntry.TaskTitle,
				logEntry.From,
				logEntry.To,
				logEntry.TimeInHours,
				logEntry.Comment,
				logEntry.Person,
				logEntry.TaskPath,
				logEntry.Type,
				logEntry.AltColor)
		{
		}

		public LogEntry(uint entryId, LogEntry other)
			:
			this(entryId,
				other.TaskId,
				other.Title,
				other.StartDate,
				other.EndDate,
				other.TimeSpentInHrs,
				other.Comment,
				other.Person,
				other.TaskPath,
				other.Type,
				other.FillColor)
		{
		}

		public LogEntry(uint entryId,
						uint taskId,
						string taskTitle,
						DateTime from,
						DateTime to,
						double timeSpentInHrs,
						string comment,
						string person,
						string path,
						string type,
						Color altColor)
		{
			Id = entryId;
			TaskId = taskId;
			StartDate = from;
			EndDate = to;
			TimeSpentInHrs = timeSpentInHrs;
			Title = taskTitle;
			Comment = comment;
			Person = person;
			TaskPath = path;
			Type = type;
			FillColor = altColor;
		}

		public uint TaskId { get; private set; }
		public double TimeSpentInHrs { get; private set; }
		public string Person { get; private set; }
		public string TaskPath { get; private set; }
		public string Type { get; private set; }
		public string Comment { get; private set; }

		public bool HasTitle { get { return !string.IsNullOrWhiteSpace(Title); } }
		public bool HasComment { get { return !string.IsNullOrWhiteSpace(Comment); } }

		// If we set the fill colour to Color.Empty and then ask for it
		// back our base class will return SystemColors.Window instead
		// so we need this attribute to return to us the 'true' fill color
		public Color TrueFillColor
		{
			get { return ((FillColor == SystemColors.Window) ? Color.Empty : FillColor); }
		}

		public bool Matches(uint taskId, Calendar.AppointmentDates dates, double timeSpentInHrs, string comment, Color fillColor)
		{
			return ((taskId == TaskId) &&
					 DatesMatch(dates) && 
					(timeSpentInHrs == TimeSpentInHrs) && 
					(comment == Comment) && 
					(fillColor == FillColor));
		}

		private bool Modify(Calendar.AppointmentDates dates)
		{
			if (!dates.IsValid)
				return false;

			if (DatesMatch(dates))
				return false;

			StartDate = dates.Start;
			EndDate = dates.End;

			return true;
		}

		public bool Modify(Calendar.AppointmentDates dates, double timeSpentInHrs, string comment, Color fillColor)
		{
			bool modified = Modify(dates);

			if (TimeSpentInHrs != timeSpentInHrs)
			{
				TimeSpentInHrs = timeSpentInHrs;
				modified = true;
			}

			if (Comment != comment)
			{
				Comment = comment;
				modified = true;
			}

			if (FillColor != fillColor)
			{
				FillColor = fillColor;
				modified = true;
			}

			return modified;
		}

		public override bool IsLongAppt(DateTime start, DateTime end)
		{
			return false; // never
		}

	}

	// ---------------------------------------------------------------

	public class LogFile
	{
		private TaskTimeLogUtil m_LogUtil;
		private List<LogEntry> m_Entries;

		public uint TaskId { get; private set; }
		public string FilePath { get; private set; }
		public bool IsAccessible { get; private set; } = true;
		public bool WasAccessible { get; private set; } = true;

		public LogFile(TaskTimeLogUtil logUtil)
		{
			m_LogUtil = logUtil;
			m_Entries = new List<LogEntry>();
		}

		public bool IsEmpty { get { return (m_Entries?.Count == 0); } }

		public bool Exists
		{
			get
			{
				if (string.IsNullOrEmpty(FilePath))
					return false;

				return File.Exists(FilePath);
			}
		}

		private void Reset()
		{
			m_Entries.Clear();
		}

		public bool HasEntry(uint entryId)
		{
			return (GetEntry(entryId) != null);
		}

		public bool AddEntry(LogEntry entry, ref uint nextEntryId)
		{
			if (HasEntry(entry.Id))
			{
				Debug.Assert(false);
				return false;
			}

			if (entry.Id == 0)
				entry.Id = nextEntryId++;
			else
				nextEntryId = Math.Max(nextEntryId, (entry.Id + 1));

			m_Entries.Add(entry);
			return true;
		}

		public bool DeleteEntry(uint entryId)
		{
			int iEntry = GetEntryIndex(entryId);

			if (iEntry == -1)
				return false;

			m_Entries.RemoveAt(iEntry);
			return true;
		}

		public bool LoadEntries(string tasklistPath, ref uint nextEntryId)
		{
			return LoadEntries(tasklistPath, 0, ref nextEntryId);
		}

		public bool LoadEntries(string tasklistPath, uint taskId, ref uint nextEntryId)
		{
			var logEntries = m_LogUtil.LoadEntries(tasklistPath, taskId);

			if (logEntries != null)
			{
				m_Entries.Clear();

				foreach (var entry in logEntries)
				{
					// 'To' date/time should be greater than 'From' date/time
					// 'To' date should be same as 'From' date OR the next day
					Debug.Assert(entry.To > entry.From);
					Debug.Assert((entry.To.Date == entry.From.Date) ||
								(entry.To.Date == entry.From.Date.AddDays(1.0)));

					// If the entry spans midnight we split it into 2
					if ((entry.To.Date > entry.From.Date) &&
						(entry.To.TimeOfDay.Ticks > 0))
					{
						var dates = new Calendar.AppointmentDates();

						// Calculate part before midnight
						var beforeEntry = new LogEntry(nextEntryId++, entry);

						dates.Start = entry.From;
						dates.End = entry.To.Date;
						Debug.Assert(dates.IsValid);

						var daysDuration = (entry.To - entry.From).TotalDays;

						double hoursSpentBeforeMidnight = ((entry.TimeInHours * (entry.To.Date - entry.From).TotalDays) / daysDuration);
						Debug.Assert(hoursSpentBeforeMidnight > 0.0);

						beforeEntry.Modify(dates, hoursSpentBeforeMidnight, beforeEntry.Comment, beforeEntry.FillColor);
						m_Entries.Add(beforeEntry);

						// Calculate proportion after midnight
						var afterEntry = new LogEntry(nextEntryId++, entry);

						dates.Start = entry.To.Date;
						dates.End = entry.To;
						Debug.Assert(dates.IsValid);

						double hoursSpentAfterMidnight = (entry.TimeInHours - hoursSpentBeforeMidnight);
						Debug.Assert(hoursSpentAfterMidnight > 0.0);

						afterEntry.Modify(dates, hoursSpentAfterMidnight, afterEntry.Comment, afterEntry.FillColor);
						m_Entries.Add(afterEntry);
					}
					else
					{
						m_Entries.Add(new LogEntry(nextEntryId++, entry));
					}
				}

				TaskId = taskId;
				FilePath = TaskTimeLogUtil.GetLogPath(tasklistPath, taskId);
			}

			WasAccessible = IsAccessible;
			IsAccessible = (logEntries != null);

			return IsAccessible;
		}

		public LogEntry GetEntry(uint entryId)
		{
			return m_Entries.Find(x => (x.Id == entryId));
		}

		private int GetEntryIndex(uint entryId)
		{
			return m_Entries.FindIndex(x => (x.Id == entryId));
		}

		public List<LogEntry> GetEntries(DateTime from, DateTime to)
		{
			return m_Entries.Where(x => (x.StartDate < x.EndDate) &&
										(x.StartDate >= from) &&
										(x.EndDate <= to)).ToList();
		}

		public LogEntry FindEntry(uint taskId, Calendar.AppointmentDates dates, double timeSpentInHrs, string comment, Color fillColor)
		{
			return m_Entries.Find(x => x.Matches(taskId, dates, timeSpentInHrs, comment, fillColor));
		}

		public bool SaveEntries(string tasklistPath)
		{
			Debug.Assert(!string.IsNullOrEmpty(tasklistPath));

			var logEntries = new List<TaskTimeLogEntry>();

			foreach (var entry in m_Entries)
			{
				logEntries.Add(new TaskTimeLogEntry()
				{
					TaskId = entry.TaskId,
					From = entry.StartDate,
					To = entry.EndDate,
					TimeInHours = entry.TimeSpentInHrs,
					TaskTitle = entry.Title,
					Comment = entry.Comment,
					Person = entry.Person,
					TaskPath = entry.TaskPath,
					Type = entry.Type,
					AltColor = entry.TrueFillColor
				});
			}

			WasAccessible = IsAccessible;

			try
			{
				IsAccessible = m_LogUtil.SaveEntries(tasklistPath, logEntries, TaskId);
			}
			catch (Exception)
			{
				IsAccessible = false;
			}

			return IsAccessible;
		}
	}

	// ---------------------------------------------------------------

	public class LogFiles
	{
		private TaskTimeLogUtil m_LogUtil;
		private List<LogFile> m_LogFiles;
		private uint m_NextEntryId;

		public LogFiles(TaskTimeLogUtil logUtil)
		{
			m_LogUtil = logUtil;
			m_LogFiles = new List<LogFile>();
		}

		public bool IsEmpty { get { return (m_LogFiles?.Count == 0); } }
		public string TasklistPath { get; private set; }

		private void Reset()
		{
			m_LogFiles.Clear();
			m_NextEntryId = 1;
		}

		public bool HasLogFile(string logPath)
		{
			return (GetLogFile(logPath) != null);
		}

		public LogFile GetLogFile(string logPath)
		{
			return m_LogFiles.Find(x => LogFiles.IsSamePath(x.FilePath, logPath));
		}

		public LogFile GetLogFile(string tasklistPath, uint taskId)
		{
			string logPath = TaskTimeLogUtil.GetLogPath(tasklistPath, taskId);

			return m_LogFiles.Find(x => LogFiles.IsSamePath(x.FilePath, logPath));
		}

		public LogFile GetLogFile(uint entryId)
		{
			return m_LogFiles.Find(x => x.HasEntry(entryId));
		}

		public bool HasEntry(uint entryId)
		{
			return (GetEntry(entryId) != null);
		}

		public LogEntry GetEntry(uint entryId)
		{
			foreach (var logFile in m_LogFiles)
			{
				var entry = logFile.GetEntry(entryId);

				if (entry != null)
					return entry;
			}

			return null;
		}

		public LogEntry FindEntry(uint taskId, Calendar.AppointmentDates dates, double timeSpentInHrs, string comment, Color fillColor)
		{
			foreach (var logFile in m_LogFiles)
			{
				var entry = logFile.FindEntry(taskId, dates, timeSpentInHrs, comment, fillColor);

				if (entry != null)
					return entry;
			}

			return null;
		}

		public bool AddEntry(LogEntry entry, bool logSeparately)
		{
			var logPath = TaskTimeLogUtil.GetLogPath(TasklistPath, (logSeparately ? entry.TaskId : 0));
			var logFile = GetLogFile(logPath);

			if (logFile == null)
			{
				logFile = new LogFile(m_LogUtil);
				m_LogFiles.Add(logFile);
			}

			return logFile.AddEntry(entry, ref m_NextEntryId);
		}

		public bool LoadLogFiles(string tasklistPath)
		{
			Reset();
			TasklistPath = tasklistPath;

			// 'Main' log file
			LoadLogFile(tasklistPath, 0);

			// Separate 'Task' log files
			var filter = TaskTimeLogUtil.GetLogFileFilter(tasklistPath, true);

			if (Directory.Exists(Path.GetDirectoryName(filter)))
			{
				var logFiles = Directory.GetFiles(Path.GetDirectoryName(filter), Path.GetFileName(filter));

				foreach (var file in logFiles)
					ReloadLogFile(file);
			}

			return true;
		}

		public static bool IsSamePath(string path1, string path2)
		{
			if (string.IsNullOrWhiteSpace(path1) || string.IsNullOrWhiteSpace(path2))
				return false;

			return (string.Compare(Path.GetFullPath(path1), Path.GetFullPath(path2), StringComparison.InvariantCultureIgnoreCase) == 0);
		}

		private bool ExtractTaskIdFromLogPath(string logPath, out uint taskId)
		{
			taskId = 0;

			var filename = Path.GetFileNameWithoutExtension(logPath);
			string taskIdStr = filename.Substring(0, filename.Length - 4); // remove '_Log'

			return uint.TryParse(taskIdStr, out taskId);
		}

		public bool IsLogFile(string logPath, bool main)
		{
			if (main)
				return IsSamePath(logPath, TaskTimeLogUtil.GetLogPath(TasklistPath));

			// else
			if (!IsSamePath(Path.GetDirectoryName(logPath), Path.ChangeExtension(TasklistPath, null)))
				return false;

			uint taskId = 0;
			return (ExtractTaskIdFromLogPath(logPath, out taskId) && (taskId != 0));
		}

		public bool ReloadLogFile(string logPath)
		{
			uint taskId = 0;
			ExtractTaskIdFromLogPath(logPath, out taskId);

			return LoadLogFile(TasklistPath, taskId);
		}

		private bool LoadLogFile(string tasklistPath, uint taskId)
		{
			var logPath = TaskTimeLogUtil.GetLogPath(tasklistPath, taskId);
			var logFile = GetLogFile(logPath);

			bool newLogFile = (logFile == null);

			if (newLogFile)
				logFile = new LogFile(m_LogUtil);

			if (!logFile.LoadEntries(tasklistPath, taskId, ref m_NextEntryId))
				return false;

			if (newLogFile)
				m_LogFiles.Add(logFile);

			return true;
		}

		public List<LogEntry> GetEntries(DateTime from, DateTime to)
		{
			var logEntries = new List<LogEntry>();

			foreach (var logFile in m_LogFiles)
				logEntries.AddRange(logFile.GetEntries(from, to));

			return logEntries;
		}

		public bool DeleteLogFile(string logPath)
		{
			var logFile = GetLogFile(logPath);

			return m_LogFiles.Remove(logFile);
		}

		public bool DeleteAllTaskLogFiles()
		{
			return (m_LogFiles.RemoveAll(x => (x.TaskId != 0)) > 0);
		}
	}

	// ---------------------------------------------------------------

	public class LogEntryAppointmentView : Calendar.AppointmentView
	{
		public LogEntryAppointmentView(Calendar.Appointment appt) : base(appt)
		{
		}

		public Rectangle IconRect = Rectangle.Empty;
		public int TextHorzOffset = 0;
	}

}
