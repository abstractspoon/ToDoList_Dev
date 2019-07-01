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

				double CalculateDurationInHours(DateTime^ from, DateTime^ to);
				double DayLengthInHours(bool includingLunch);
				double LunchBreakInHours();

				void Load(Preferences^ prefs);

			protected:
				double m_StartOfDayInHours;
				double m_WorkingLengthInHours;
				double m_StartOfLunchInHours;
				double m_EndOfLunchInHours;

			protected:
				double CalculateDurationInHours(double fromHour, double toHour);

				static Int32 HoursToMilleseconds(double hours);
				static double GetTimeOfDayInHours(DateTime^ date);

			};
			
			public ref class DateUtil
			{
			public:
				static int WeekOfYear(DateTime^ date);
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

