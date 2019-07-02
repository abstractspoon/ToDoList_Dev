// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DateUtil.h"
#include "Preferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;
using namespace System::Collections::Generic;

////////////////////////////////////////////////////////////////////////////////////////////////

WorkingDay::WorkingDay()
	:
	m_StartOfDayInHours(9),
	m_StartOfLunchInHours(13),
	m_EndOfLunchInHours(14),
	m_WorkingLengthInHours(8)
{

}

WorkingDay::WorkingDay(Preferences^ prefs)
	:
	m_StartOfDayInHours(9),
	m_StartOfLunchInHours(13),
	m_EndOfLunchInHours(14),
	m_WorkingLengthInHours(8)
{
	Load(prefs);
}

double WorkingDay::StartOfDayInHours()
{
	return m_StartOfDayInHours;
}

double WorkingDay::EndOfDayInHours()
{
	return (m_StartOfDayInHours + DayLengthInHours(true));
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
	return date->Date.AddMilliseconds(HoursToMilleseconds(EndOfDayInHours()));
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

double WorkingDay::CalculateDurationInHours(double fromHour, double toHour)
{
	fromHour = Math::Max(fromHour, m_StartOfDayInHours);
	toHour = Math::Min(toHour, EndOfDayInHours());

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

double WorkingDay::DayLengthInHours(bool includingLunch)
{
	if (includingLunch)
		return (m_WorkingLengthInHours + LunchBreakInHours());

	// else
	return m_WorkingLengthInHours;
}

double WorkingDay::LunchBreakInHours()
{
	return (m_EndOfLunchInHours - m_StartOfLunchInHours);
}

void WorkingDay::Load(Preferences^ prefs)
{
	m_StartOfDayInHours = prefs->GetProfileDouble(L"Preferences", L"StartOfWorkdayInHours", 9.0);
	m_WorkingLengthInHours = prefs->GetProfileDouble(L"Preferences", L"HoursInDay", 8.0);
	m_StartOfLunchInHours = prefs->GetProfileDouble(L"Preferences", L"StartOfLunchInHours", 13.0);

	if (prefs->GetProfileBool(L"Preferences", L"HasLunchBreak", true))
		m_EndOfLunchInHours = prefs->GetProfileDouble(L"Preferences", L"EndOfLunchInHours", 14.0);
	else
		m_EndOfLunchInHours = m_StartOfLunchInHours;
	
	// Validate lunch break
	if ((m_StartOfLunchInHours < m_StartOfDayInHours) ||
		(m_EndOfLunchInHours > EndOfDayInHours()) ||
		(m_StartOfLunchInHours > m_EndOfLunchInHours))
	{
		m_StartOfLunchInHours = m_EndOfLunchInHours;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

enum 
{
	SUNDAY = 0X01,
	MONDAY = 0X02,
	TUESDAY = 0X04,
	WEDNESDAY = 0X08,
	THURSDAY = 0X10,
	FRIDAY = 0X20,
	SATURDAY = 0X40,
};

WorkingWeek::WorkingWeek() 
	: 
	m_WorkingDay(gcnew WorkingDay()),
	m_WeekendDays(gcnew List<System::DayOfWeek>())
{
	m_WeekendDays->Add(System::DayOfWeek::Saturday);
	m_WeekendDays->Add(System::DayOfWeek::Sunday);
}

WorkingWeek::WorkingWeek(Preferences^ prefs)
	:
	m_WorkingDay(gcnew WorkingDay()),
	m_WeekendDays(gcnew List<System::DayOfWeek>())
{
	Load(prefs);
}

void WorkingWeek::Load(Preferences^ prefs)
{
	m_WorkingDay->Load(prefs);
	m_WeekendDays->Clear();

	int dwWeekends = prefs->GetProfileInt("Preferences", "Weekends", 0);
	
	if ((dwWeekends & SUNDAY) == SUNDAY)
		m_WeekendDays->Add(System::DayOfWeek::Sunday);

	if ((dwWeekends & SATURDAY) == SATURDAY)
		m_WeekendDays->Add(System::DayOfWeek::Saturday);

	if ((dwWeekends & MONDAY) == MONDAY)
		m_WeekendDays->Add(System::DayOfWeek::Monday);

	if ((dwWeekends & TUESDAY) == TUESDAY)
		m_WeekendDays->Add(System::DayOfWeek::Tuesday);

	if ((dwWeekends & WEDNESDAY) == WEDNESDAY)
		m_WeekendDays->Add(System::DayOfWeek::Wednesday);

	if ((dwWeekends & THURSDAY) == THURSDAY)
		m_WeekendDays->Add(System::DayOfWeek::Thursday);

	if ((dwWeekends & FRIDAY) == FRIDAY)
		m_WeekendDays->Add(System::DayOfWeek::Friday);
}

double WorkingWeek::CalculateDurationInHours(System::DateTime^ from, DateTime^ to)
{
	int nDaysDuration = (to->Date - from->Date).Days;

	if (nDaysDuration < 0)
		return 0;

	double fromTimeOfDay = m_WorkingDay->GetTimeOfDayInHours(from);
	double toTimeOfDay = m_WorkingDay->GetTimeOfDayInHours(to);
	double dHoursDuration = 0;

	if (nDaysDuration == 0)
	{
		// from and to are same day
		dHoursDuration = m_WorkingDay->CalculateDurationInHours(fromTimeOfDay, toTimeOfDay);
	}
	else
	{
		// clear days
		dHoursDuration = ((nDaysDuration - 1) * m_WorkingDay->DayLengthInHours(false));

		// part days
		dHoursDuration += m_WorkingDay->CalculateDurationInHours(fromTimeOfDay, m_WorkingDay->EndOfDayInHours());
		dHoursDuration += m_WorkingDay->CalculateDurationInHours(m_WorkingDay->StartOfDayInHours(), toTimeOfDay);
	}

	return dHoursDuration;
}

bool WorkingWeek::IsWeekend(DateTime^ date)
{
	return m_WeekendDays->Contains(date->DayOfWeek);
}

////////////////////////////////////////////////////////////////////////////////////////////////

// This conforms to ISO 8601 specification for week number.

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
