#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			ref class Preferences;

			public ref class WorkingDay
			{
			public:
				WorkingDay();
				WorkingDay(Preferences^ prefs);

				double StartOfDayInHours();
				double EndOfDayInHours();
				double StartOfLunchInHours();
				double EndOfLunchInHours();
				
				DateTime StartOfDay(DateTime date);
				DateTime EndOfDay(DateTime date);
				DateTime StartOfLunch(DateTime date);
				DateTime EndOfLunch(DateTime date);

				TimeSpan StartOfDay();
				TimeSpan EndOfDay();
				TimeSpan StartOfLunch();
				TimeSpan EndOfLunch();

				double CalculateDurationInHours(double fromHour, double toHour);
				double DayLengthInHours(bool includingLunch);
				double LunchLengthInHours();

				void Load(Preferences^ prefs);

				static Int32 HoursToMilleseconds(double hours);
				static double GetTimeOfDayInHours(DateTime date);

			protected:
				double m_StartOfDayInHours;
				double m_WorkingLengthInHours;
				double m_StartOfLunchInHours;
				double m_EndOfLunchInHours;
			};

			// ------------------------------------------------------------------

			public ref class WorkingWeek
			{
			public:
				WorkingWeek();
				WorkingWeek(Preferences^ prefs);

				double CalculateDurationInHours(DateTime from, DateTime to);
				bool IsWeekend(DateTime date);

				void Load(Preferences^ prefs);

				WorkingDay^ WorkDay() { return m_WorkingDay; }
				Collections::Generic::List<DayOfWeek>^ WeekendDays() { return m_WeekendDays; }
				Collections::Generic::List<DayOfWeek>^ WeekDays();

			protected:
				WorkingDay^ m_WorkingDay;
				Collections::Generic::List<DayOfWeek>^ m_WeekendDays;
			};

			// ------------------------------------------------------------------
						
			public ref class DateUtil
			{
			public:
				static int WeekOfYear(DateTime date);
				static int GetMaxDayOfWeekNameWidth(Drawing::Graphics^ graphics, Drawing::Font^ font, bool shortName);
				static int GetMaxMonthNameWidth(Drawing::Graphics^ graphics, Drawing::Font^ font, bool shortName);

				static String^ GetMonthName(int nMonth, bool shortName);
				static String^ FormatRange(DateTime dateFrom, DateTime dateTo, bool bWithTime, bool bISO);

				static String^ FormatDateOnly(DateTime date, String^ format);
				static String^ FormatDateOnlyRange(DateTime dateFrom, DateTime dateTo, String^ format); // delim = " - "

				static int DateInMonths(DateTime date);
				static DateTime DateFromMonths(int nMonths);

				static int GetDay(DateTime date);
				static int GetMonth(DateTime date);
				static int GetYear(DateTime date);

				static TimeSpan TimeOnly(DateTime date);

				static int MapDaysOfWeek(Collections::Generic::List<DayOfWeek>^ days);
				static Collections::Generic::List<DayOfWeek>^ MapDaysOfWeek(int dwDays);
				static int MapDayOfWeek(DayOfWeek day);
				
				static Collections::Generic::List<DayOfWeek>^ AllDaysOfWeek();
				static bool IsAllDaysOfWeek(Collections::Generic::List<DayOfWeek>^ days);

				static void SetShortDateFormat(Windows::Forms::DateTimePicker^ ctrl, bool isoFormat);
				static bool WantRTLDates();
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

