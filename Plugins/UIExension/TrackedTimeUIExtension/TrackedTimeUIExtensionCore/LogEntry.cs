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

namespace TrackedTimeUIExtension
{
	// ---------------------------------------------------------------

	public class LogEntries
	{
		private String m_Version;
		private String m_Header;
		private string m_Delim;
		private Encoding m_Encoding;
		private List<LogEntry> m_Entries;

		const string VersionPrefix = "TODOTIMELOG VERSION";

		public LogEntries()
		{
			// TODO
		}

		public bool IsEmpty { get { return (m_Entries?.Count == 0); } }

		private void Reset()
		{
			m_Version = null;
			m_Header = null;
			m_Delim = null;
			m_Encoding = Encoding.UTF8;
			m_Entries = null;
		}

		public bool Load(string filePath)
		{
			Reset();

			try
			{
				using (var reader = new StreamReader(filePath))
				{
					string version = reader.ReadLine().Trim();

					if (string.IsNullOrEmpty(version) || (version.IndexOf(VersionPrefix) != 0))
						return false;

					// Must be version 1 or higher
					string versionNum = version.Substring(VersionPrefix.Length + 1, 1);

					int ver = 0;

					if (!int.TryParse(versionNum, out ver) || (ver < 1))
						return false;

					string header = reader.ReadLine();

					if (string.IsNullOrEmpty(header))
						return false;

					if (header.Contains("\t"))
						m_Delim = "\t";

					m_Version = version;
					m_Header = header;
					m_Encoding = reader.CurrentEncoding;
					m_Entries = new List<LogEntry>();

					string line = reader.ReadLine();
					uint entryId = 1;

					while (line != null)
					{
						m_Entries.Add(new LogEntry(entryId++, line, m_Delim));
						line = reader.ReadLine();
					}
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

		public List<LogEntry> GetEntries(DateTime from, DateTime to)
		{
			return m_Entries.Where(x => (x.StartDate >= from) && (x.EndDate <= to)).ToList();
		}

		public bool SaveLogFile(string filePath)
		{
			if (string.IsNullOrEmpty(m_Version) || string.IsNullOrEmpty(m_Header))
				return false;

			try
			{
				using (var writer = new StreamWriter(filePath, false, m_Encoding))
				{
					writer.WriteLine(m_Version);
					writer.WriteLine(m_Header);

					foreach (var entry in m_Entries)
						writer.WriteLine(entry.Encode());
				}
			}
			catch (Exception )
			{
				// File locked?
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
		private string m_Delim;
		private string m_Entry;
		private string m_UserId;
		private string m_Comment;
		private string m_Type;
		private string m_Path;

		private uint m_TaskId = 0;
		private double m_TimeSpentInHrs = 0.0;

		bool m_NeedsEncoding = false;

		private Calendar.AppointmentDates m_OrgDates = new Calendar.AppointmentDates();

		// --------------------

		public LogEntry(uint entryId, string delim)
		{
			m_Delim = delim;
			m_Entry = string.Empty;

			base.Id = entryId;
		}

		public LogEntry(uint entryId, string logEntry, string delim)
			: 
			this(entryId, delim)
		{
			m_Entry = logEntry;
			Decode();
		}

		public LogEntry(LogEntry entry)
			:
			this(entry, entry.Id)
		{

		}

		public LogEntry(LogEntry entry, uint entryId)
		{
			m_Delim = entry.m_Delim;
			m_Entry = entry.m_Entry;
			m_UserId = entry.m_UserId;
			m_Comment = entry.m_Comment;
			m_Type = entry.m_Type;
			m_Path = entry.m_Path;

			m_TaskId = entry.m_TaskId;
			m_TimeSpentInHrs = entry.m_TimeSpentInHrs;

			base.Id = entryId;
		}

		public uint TaskId
		{
			get { return m_TaskId; }
			set
			{
				if (m_TaskId == 0)
				{
					m_TaskId = value;
					m_NeedsEncoding = true;
				}
			}
		}

		public string UserId
		{
			get { return m_UserId; }
			set
			{
				if (string.IsNullOrEmpty(m_UserId))
				{
					m_UserId = value;
					m_NeedsEncoding = true;
				}
			}
		}

		public double TimeSpentInHrs
		{
			get { return m_TimeSpentInHrs; }
			set
			{
				if (m_TimeSpentInHrs != value)
				{
					m_TimeSpentInHrs = value;
					m_NeedsEncoding = true;
				}
			}
		}

		public string Comment
		{
			get { return m_Comment; }
			set
			{
				if (m_Comment != value)
				{
					m_Comment = value;
					m_NeedsEncoding = true;
				}
			}
		}

		public string EntryType
		{
			get { return m_Type; }
			set
			{
				if (m_Type != value)
				{
					m_Type = value;
					m_NeedsEncoding = true;
				}
			}
		}

		private void Decode()
		{
			var parts = m_Entry.Split(new string[] { m_Delim }, StringSplitOptions.None);

			if (parts.Length >= 11)
			{
				DateTime start, end;
				int rgbColor = -1;

				uint.TryParse(parts[0], out m_TaskId);
				Title = parts[1];
				m_UserId = parts[2];
				DateTime.TryParse(parts[3] + ' ' + parts[4], out start);
				DateTime.TryParse(parts[5] + ' ' + parts[6], out end);
				double.TryParse(parts[7], out m_TimeSpentInHrs);
				m_Comment = parts[8];
				m_Type = parts[9];
				m_Path = parts[10];

				if (parts.Length > 11)
					int.TryParse(parts[11], out rgbColor);

				StartDate = start;
				EndDate = end;
				FillColor = ((rgbColor == -1) ? Color.Empty : Color.FromArgb(rgbColor));
			}

			m_NeedsEncoding = false;
		}

		public string Encode()
		{
			if (m_NeedsEncoding)
			{
				var elements = new string[]
					{
					m_TaskId.ToString(),
					m_UserId,
					base.StartDate.ToString("yyyy-MM-dd"), // ISO
					base.StartDate.ToString("HH:mm"),      // ISO
					base.EndDate.ToString("yyyy-MM-dd"),   // ISO
					base.EndDate.ToString("HH:mm"),        // ISO
					m_TimeSpentInHrs.ToString(),
					m_Comment,
					m_Type,
					m_Path,
					base.FillColor.ToArgb().ToString()
					};

				m_Entry = string.Join(m_Delim, elements);
				m_NeedsEncoding = false;
			}

			return m_Entry;
		}

		protected override void OnEndDateChanged()
		{
			m_NeedsEncoding = true;
		}

		protected override void OnStartDateChanged()
		{
			m_NeedsEncoding = true;
		}

		protected override void OnColorChanged()
		{
			m_NeedsEncoding = true;
		}

		protected override void OnTitleChanged()
		{
			m_NeedsEncoding = true;
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
