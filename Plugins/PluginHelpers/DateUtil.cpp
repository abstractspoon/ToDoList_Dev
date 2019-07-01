// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DateUtil.h"
#include "Preferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

WorkingDay::WorkingDay() 
	: 
	m_StartOfDayInHours(9), 
	m_StartOfLunchInHours(13),
	m_EndOfLunchInHours(14),
	m_EndOfDayInHours(18)
{

}

WorkingDay::WorkingDay(Preferences^ prefs)
	:
	m_StartOfDayInHours(9),
	m_StartOfLunchInHours(12.5),
	m_EndOfLunchInHours(13.5),
	m_EndOfDayInHours(17.5)
{
	Load(prefs);
}

double WorkingDay::StartOfDayInHours()
{
	return m_StartOfDayInHours;
}

double WorkingDay::EndOfDayInHours()
{
	return m_EndOfDayInHours;
}

double WorkingDay::StartOfLunchInHours()
{
	return m_StartOfLunchInHours;
}

double WorkingDay::EndOfLunchInHours()
{
	return m_EndOfLunchInHours;
}

DateTime^ WorkingDay::StartOfDay(DateTime^ date)
{
	return date->Date.AddMilliseconds(HoursToMilleseconds(m_StartOfDayInHours));
}

DateTime^ WorkingDay::EndOfDay(DateTime^ date)
{
	return date->Date.AddMilliseconds(HoursToMilleseconds(m_EndOfDayInHours));
}

DateTime^ WorkingDay::StartOfLunch(DateTime^ date)
{
	return date->Date.AddMilliseconds(HoursToMilleseconds(m_StartOfLunchInHours));
}

DateTime^ WorkingDay::EndOfLunch(DateTime^ date)
{
	return date->Date.AddMilliseconds(HoursToMilleseconds(m_EndOfLunchInHours));
}

Int32 WorkingDay::HoursToMilleseconds(double hours)
{
	return (Int32)(hours * 60 * 60 * 1000);
}

double WorkingDay::CalculateDurationInHours(System::DateTime^ from, DateTime^ to)
{
	int nDaysDuration = (to->Date - from->Date).Days;

	if (nDaysDuration < 0)
		return 0;

	double fromTimeOfDay = GetTimeOfDayInHours(from);
	double toTimeOfDay = GetTimeOfDayInHours(to);
	double dHoursDuration = 0;
	
	if (nDaysDuration == 0)
	{
		// from and to are same day
		dHoursDuration = CalculateDurationInHours(fromTimeOfDay, toTimeOfDay);
	}
	else
	{
		// clear days
		dHoursDuration = ((nDaysDuration - 1) * DayLengthInHours());

		// part days
		dHoursDuration += CalculateDurationInHours(fromTimeOfDay, m_EndOfDayInHours);
		dHoursDuration += CalculateDurationInHours(m_StartOfDayInHours, toTimeOfDay);
	}

	return dHoursDuration;
}

double WorkingDay::CalculateDurationInHours(double fromHour, double toHour)
{
	fromHour = Math::Max(fromHour, m_StartOfDayInHours);
	toHour = Math::Min(toHour, m_EndOfDayInHours);

	if (fromHour >= toHour)
		return 0;

	if (fromHour >= m_StartOfLunchInHours)
		fromHour = Math::Max(fromHour, m_EndOfLunchInHours);

	if (toHour <= m_EndOfLunchInHours)
		toHour = Math::Min(toHour, m_StartOfLunchInHours);

	double dDuration = (toHour - fromHour);

	if (fromHour < m_StartOfLunchInHours && toHour > m_EndOfLunchInHours)
		dDuration -= LunchBreakInHours();

	return Math::Max(dDuration, 0.0);
}

double WorkingDay::GetTimeOfDayInHours(DateTime^ date)
{
	return (double)(date->Hour + (date->Minute / 60.0) + (date->Second / 3600.0) + (date->Millisecond / 3600000));
}

double WorkingDay::DayLengthInHours()
{
	return (m_EndOfDayInHours - m_StartOfDayInHours - LunchBreakInHours());
}

double WorkingDay::LunchBreakInHours()
{
	return (m_EndOfLunchInHours - m_StartOfLunchInHours);
}

void WorkingDay::Load(Preferences^ prefs)
{
	m_StartOfDayInHours = prefs->GetProfileDouble(L"Preferences", L"StartOfWorkdayInHours", 9.0);
	m_StartOfLunchInHours = prefs->GetProfileDouble(L"Preferences", L"StartOfLunchInHours", 13.0);
	m_EndOfLunchInHours = prefs->GetProfileDouble(L"Preferences", L"EndOfLunchInHours", 14.0);

	m_EndOfDayInHours = (m_StartOfDayInHours + 
						(m_EndOfLunchInHours - m_StartOfLunchInHours) + 
						prefs->GetProfileDouble(L"Preferences", L"HoursInDay", 8.0));

	if ((m_StartOfLunchInHours < m_StartOfDayInHours) ||
		(m_EndOfLunchInHours > m_EndOfDayInHours) ||
		(m_StartOfLunchInHours > m_EndOfLunchInHours))
	{
		m_StartOfLunchInHours = m_EndOfLunchInHours;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// Get the Week number of the year
/// (In the range 1..53)
/// This conforms to ISO 8601 specification for week number.
/// </summary>
/// <param name="date"></param>
/// <returns>Week of year</returns>
int DateUtil::WeekOfYear(DateTime^ date)
{
	const int moveByDays[] = { 6, 7, 8, 9, 10, 4, 5 };

	DateTime^ startOfYear = gcnew DateTime(date->Year, 1, 1);
	DateTime^ endOfYear = gcnew DateTime(date->Year, 12, 31);

	// ISO 8601 weeks start with Monday 
	// The first week of a year includes the first Thursday 
	// This means that Jan 1st could be in week 51, 52, or 53 of the previous year...
	int numberDays = date->Subtract(*startOfYear).Days + moveByDays[(int)startOfYear->DayOfWeek];
	int weekNumber = numberDays / 7;

	switch (weekNumber)
	{
	case 0:
		// Before start of first week of this year - in last week of previous year
		weekNumber = WeekOfYear(startOfYear->AddDays(-1));
		break;
	case 53:
		// In first week of next year.
		if (endOfYear->DayOfWeek < DayOfWeek::Thursday)
		{
			weekNumber = 1;
		}
		break;
	}

	return weekNumber;
}
