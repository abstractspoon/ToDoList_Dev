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

		public bool AddEntry(uint taskId, string taskTitle, DateTime start, DateTime end, string comment, double timeSpentInHrs, Color fillColor)
		{
			// TODO
			return false;
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
					LogEntry le = null;

					for (int entry = 0; entry < numEntries; entry++)
					{
						if (LogEntry.TryReadEntry(reader, entry, out le))
							m_Entries.Add(le);
					}

					m_NextEntryId = (m_Entries.LastOrDefault().Id + 1);
				}

				return true;
			}
			catch (Exception )
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
			using (var writer = new TaskTimeLogWriter(m_Entries.Count))
			{
				for (int entry = 0; entry < m_Entries.Count; entry++)
				{
					writer.SetEntry(entry,
										m_Entries[entry].TaskId,
										m_Entries[entry].StartDate,
										m_Entries[entry].EndDate,
										m_Entries[entry].TimeSpentInHrs,
										m_Entries[entry].Title,
										m_Entries[entry].Comment,
										m_Entries[entry].UserId,
										m_Entries[entry].Path,
										m_Entries[entry].Type,
										m_Entries[entry].FillColor);
				}

				if (writer.Save(filePath))
				{
					m_Modified = false;
					return true;
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

		public static bool TryReadEntry(TaskTimeLogReader logReader, int entry, out LogEntry le)
		{
			le = null;

			UInt32 taskID;
			DateTime fromDate;
			DateTime toDate;
			double timeInHours;
			String taskTitle;
			String comment;
			String person;
			String path;
			String type;
			Color altColor;

			if (!logReader.GetEntry(entry,
									out taskID,
									out fromDate,
									out toDate,
									out timeInHours,
									out taskTitle,
									out comment,
									out person,
									out path,
									out type,
									out altColor))
			{
				return false;
			}

			le = new LogEntry((uint)entry + 1)
				{
					TaskId = taskID,
					StartDate = fromDate,
					EndDate = toDate,
					TimeSpentInHrs = timeInHours,
					Title = taskTitle,
					Comment = comment,
					UserId = person,
					Path = path,
					Type = type,
					FillColor = altColor
				};

			return true;
		}

		public LogEntry(uint entryId, uint taskId, string taskTitle, DateTime start, DateTime end, string comment, double timeSpentInHrs, Color fillColor)
		{

		}

		public uint TaskId { get; private set; }
		public double TimeSpentInHrs { get; private set; }
		public string UserId { get; private set; }
		public string Path { get; private set; }
		public string Type { get; private set; }
		public string Comment { get; private set; }

		public bool Modify(DateTime start, DateTime end, string comment, double timeSpentInHrs, Color fillColor)
		{
			bool modified = false;

			if (StartDate != start)
			{
				StartDate = start;
				modified = true;
			}

			if (EndDate != end)
			{
				EndDate = end;
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
