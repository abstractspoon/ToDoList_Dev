using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Globalization;
using System.IO;

using Abstractspoon.Tdl.PluginHelpers;

namespace TimeLogUIExtension
{
	// ---------------------------------------------------------------

	public class LogEntries : List<LogEntry>
	{
		private String m_Version = string.Empty;
		private String m_Header = string.Empty;
		private string m_Delim = CultureInfo.CurrentCulture.TextInfo.ListSeparator;
		private Encoding m_Encoding = Encoding.UTF8;

		public LogEntries()
		{
			// TODO
		}

		public bool Load(string fileName)
		{
			Clear();

			try
			{
				using (var reader = new FileInfo(fileName).OpenText())
				{
					m_Version = reader.ReadLine();

					if (string.IsNullOrEmpty(m_Version) || !m_Version.Contains("TODOTIMELOG VERSION"))
						return false;

					// Must be version 1 or higher
					if (m_Version[m_Version.Length - 1] == '0')
						return false;

					m_Header = reader.ReadLine();

					if (string.IsNullOrEmpty(m_Header))
						return false;

					if (m_Header.Contains("\t"))
						m_Delim = "\t";

					string line = reader.ReadLine();

					while (line != null)
					{
						Add(new LogEntry(line, m_Delim));
						line = reader.ReadLine();
					}
				}
			}
			catch (Exception )
			{
				if (File.Exists(fileName))
					return false;

				// Check for Excel => use tab as delimiter
				// todo
			}

			return true;
		}

		public bool SaveLogFile(string fileName)
		{

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

		private uint m_TaskId;
		private double m_TimeSpentInHrs;

		bool m_Decoded = false;

		private Calendar.AppointmentDates m_OrgDates = new Calendar.AppointmentDates();

		// --------------------

		public LogEntry(string delim)
		{
			m_Delim = delim;
			m_Entry = string.Empty;
		}

		public LogEntry(string logEntry, string delim) : this(delim)
		{
			m_Entry = logEntry;
		}

		public uint TaskId
		{
			get { Decode(); return m_TaskId; }
		}

		public string UserId
		{
			get { Decode(); return m_UserId; }
			set { m_UserId = value; Encode(); }
		}

		public double TimeSpentInHrs
		{
			get { Decode(); return m_TimeSpentInHrs; }
			set { m_TimeSpentInHrs = value; Encode(); }
		}

		public string Comment
		{
			get { Decode(); return m_Comment; }
			set { m_Comment = value; Encode(); }
		}

		public string EntryType
		{
			get { Decode(); return m_Type; }
			set { m_Type = value; Encode(); }
		}

		public override DateTime StartDate
		{
			get	{ Decode();	return base.StartDate; }
			set { base.StartDate = value; }
		}

		public override DateTime EndDate
		{
			get { Decode(); return base.EndDate; }
			set { base.EndDate = value; }
		}

		private void Decode()
		{
			if (!m_Decoded)
			{


				m_Decoded = true;
			}
		}

		public string Encode()
		{
			if (!m_Decoded)
				return m_Entry;
			
			// else
			var elements = new string[]
				{
					m_TaskId.ToString(),
					m_UserId,
					base.StartDate.Date.ToString(), // must be IDO
					base.StartDate.ToString(),
					base.EndDate.Date.ToString(),
					base.EndDate.ToString(),
					m_TimeSpentInHrs.ToString(),
					m_Comment,
					m_Type,
					m_Path,
					base.FillColor.ToString()
				};

			m_Entry = string.Join(m_Delim, elements);
			return m_Entry;
		}

		protected override void OnEndDateChanged()
		{
			Encode();
		}

		protected override void OnStartDateChanged()
		{
			Encode();
		}

		protected override void OnColorChanged()
		{
			Encode();
		}

		protected override void OnTitleChanged()
		{
			Encode();
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
