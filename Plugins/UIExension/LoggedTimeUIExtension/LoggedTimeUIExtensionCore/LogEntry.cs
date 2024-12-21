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
	// ---------------------------------------------------------------

	public class LogEntries
	{
		private List<LogEntry> m_Entries;
		private uint m_NextEntryId;

		public LogEntries()
		{
			m_Entries = new List<LogEntry>();
		}

		public bool IsEmpty { get { return (m_Entries?.Count == 0); } }

		private void Reset()
		{
			m_Entries.Clear();
			m_NextEntryId = 1;
		}

		public bool HasEntry(uint entryId)
		{
			return (GetEntry(entryId) != null);
		}

		public bool AddEntry(LogEntry entry)
		{
			if (HasEntry(entry.Id))
			{
				Debug.Assert(false);
				return false;
			}

			if (entry.Id == 0)
				entry.Id = m_NextEntryId++;
			else
				m_NextEntryId = Math.Max(m_NextEntryId, (entry.Id + 1));

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

		public bool Load(string filePath)
		{
			try
			{
				var logEntries = TaskTimeLog.Load(filePath);

				if (logEntries != null)
				{
					m_Entries.Clear();
					m_NextEntryId = 1;

					foreach (var entry in logEntries)
						m_Entries.Add(new LogEntry(m_NextEntryId++, entry));

					return true;
				}
			}
			catch (Exception)
			{
				Reset();
			}

			return false;
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

		public bool SaveLogFile(string filePath)
		{
			if (!string.IsNullOrEmpty(filePath))
			{
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
						AltColor = entry.FillColor
					});
				}

				try
				{
					return TaskTimeLog.Save(filePath, logEntries);
				}
				catch (Exception)
				{
				}
			}

			return false;
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

	// ---------------------------------------------------------------

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

		public Color TrueFillColor
		{
			get	{ return ((FillColor == SystemColors.Window) ? Color.Empty : FillColor); }
		}

		private bool Modify(Calendar.AppointmentDates dates)
		{
			if (dates.IsValid)
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
}
