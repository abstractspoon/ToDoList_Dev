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
		private bool m_Modified;

		const string VersionPrefix = "TODOTIMELOG VERSION";

		public LogEntries()
		{
			m_Entries = new List<LogEntry>();
		}

		public bool IsEmpty { get { return (m_Entries?.Count == 0); } }

		private void Reset()
		{
			m_Entries.Clear();
			m_Modified = false;
			m_NextEntryId = 1;
		}

		public bool HasEntry(uint entryId)
		{
			return (GetEntry(entryId) != null);
		}

		public bool AddEntry(uint taskId,
							string taskTitle,
							DateTime from,
							DateTime to,
							double timeSpentInHrs,
							string comment,
							string path,
							string type,
							Color altColor)
		{
			m_Entries.Add(new LogEntry(m_NextEntryId++,
										taskId,
										taskTitle,
										from,
										to,
										timeSpentInHrs,
										comment,
										Environment.UserName,
										path,
										type,
										altColor));

			m_Modified = true;
			return true;
		}

		public bool ModifyEntry(uint entryId, DateTime start, DateTime end, string comment, double timeSpentInHrs, Color fillColor)
		{
			var entry = GetEntry(entryId);

			if (entry == null)
				return false;

			if (!entry.Modify(start, end, comment, timeSpentInHrs, fillColor))
				return false;

			m_Modified = true;
			return true;
		}

		public bool DeleteEntry(uint entryId)
		{
			int iEntry = GetEntryIndex(entryId);

			if (iEntry == -1)
				return false;

			m_Entries.RemoveAt(iEntry);
			m_Modified = true;

			return true;
		}

		public bool Load(string filePath)
		{
			Reset();

			try
			{
				using (var reader = new TaskTimeLogReader(filePath))
				{
					int numEntries = reader.EntryCount;
					m_NextEntryId = 1;

					for (int entry = 0; entry < numEntries; entry++)
					{
						m_Entries.Add(new LogEntry(m_NextEntryId++, reader.GetEntry(entry)));
					}
				}

				return true;
			}
			catch (Exception)
			{
			}

			Reset();
			return false;
		}

		public LogEntry GetEntry(uint entryId)
		{
			return m_Entries.Find(x => (x.Id == entryId));
		}

		public int GetEntryIndex(uint entryId)
		{
			return m_Entries.FindIndex(x => (x.Id == entryId));
		}

		public List<LogEntry> GetEntries(DateTime from, DateTime to)
		{
			return m_Entries.Where(x => (x.StartDate >= from) && (x.EndDate <= to)).ToList();
		}

		public bool SaveLogFile(string filePath)
		{
			if (m_Modified && !string.IsNullOrEmpty(filePath))
			{
				try
				{
					using (var writer = new TaskTimeLogWriter(m_Entries.Count))
					{
						for (int entry = 0; entry < m_Entries.Count; entry++)
						{
							var logEntry = new TaskTimeLogEntry()
							{
								TaskId = m_Entries[entry].TaskId,
								From = m_Entries[entry].StartDate,
								To = m_Entries[entry].EndDate,
								TimeInHours = m_Entries[entry].TimeSpentInHrs,
								TaskTitle = m_Entries[entry].Title,
								Comment = m_Entries[entry].Comment,
								Person = m_Entries[entry].Person,
								TaskPath = m_Entries[entry].TaskPath,
								Type = m_Entries[entry].Type,
								AltColor = m_Entries[entry].FillColor
							};

							writer.SetEntry(entry, logEntry);
						}

						if (writer.Save(filePath))
						{
							m_Modified = false;
							return true;
						}
					}
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
		public int EndOfStart = -1, StartOfEnd = -1;
		public int StartOfToday = -1, EndOfToday = -1;
	}

	// ---------------------------------------------------------------

	public class LogEntry : Calendar.Appointment
	{
		private Calendar.AppointmentDates m_OrgDates = new Calendar.AppointmentDates();

		// --------------------

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

		public bool Modify(DateTime from, DateTime to, string comment, double timeSpentInHrs, Color fillColor)
		{
			bool modified = false;

			// To and from must point to the same day
			if ((int)from.ToOADate() != (int)to.ToOADate())
				return false;

			if (StartDate != from)
			{
				StartDate = from;
				modified = true;
			}

			if (EndDate != to)
			{
				EndDate = to;
				modified = true;
			}

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

		protected override void OnEndDateChanged()
		{
		}

		protected override void OnStartDateChanged()
		{
		}

		protected override void OnColorChanged()
		{
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

		public Color Color { get; private set; }

        public TimeSpan OriginalLength
        {
            get { return m_OrgDates.Length; }
        }

		public bool LengthDiffersFromOriginal()
		{
			return ((Length - OriginalLength).TotalSeconds != 0.0);
		}

		public override bool IsLongAppt(DateTime start, DateTime end)
		{
			return false; // never
		}

	}
}
