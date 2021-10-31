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
				
				DateTime^ StartOfDay(DateTime^ date);
				DateTime^ EndOfDay(DateTime^ date);
				DateTime^ StartOfLunch(DateTime^ date);
				DateTime^ EndOfLunch(DateTime^ date);

				double CalculateDurationInHours(double fromHour, double toHour);
				double DayLengthInHours(bool includingLunch);
				double LunchLengthInHours();

				void Load(Preferences^ prefs);

				static Int32 HoursToMilleseconds(double hours);
				static double GetTimeOfDayInHours(DateTime^ date);

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

				double CalculateDurationInHours(DateTime^ from, DateTime^ to);
				bool IsWeekend(DateTime^ date);

				void Load(Preferences^ prefs);

				WorkingDay^ WorkDay() { return m_WorkingDay; }
				Collections::Generic::List<System::DayOfWeek>^ WeekendDays() { return m_WeekendDays; }

			protected:
				WorkingDay^ m_WorkingDay;
				Collections::Generic::List<System::DayOfWeek>^ m_WeekendDays;

			};

			// ------------------------------------------------------------------
						
			public ref class DateUtil
			{
			public:
				static int WeekOfYear(DateTime^ date);
				static int GetMaxDayOfWeekNameWidth(System::Drawing::Graphics^ graphics, System::Drawing::Font^ font, bool shortName);
				static int GetMaxMonthNameWidth(System::Drawing::Graphics^ graphics, System::Drawing::Font^ font, bool shortName);
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

