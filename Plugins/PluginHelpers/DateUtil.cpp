// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DateUtil.h"
#include "Preferences.h"
#include "Win32.h"
#include "PluginHelpers.h"

#include <Shared\DateHelper.h>
#include <Shared\Misc.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

using namespace System::Collections::Generic;
using namespace System::Drawing;
using namespace System::Windows::Forms;

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
	return min(24, (m_StartOfDayInHours + DayLengthInHours(true)));
}

double WorkingDay::StartOfLunchInHours()
{
	return m_StartOfLunchInHours;
}

double WorkingDay::EndOfLunchInHours()
{
	return m_EndOfLunchInHours;
}

DateTime WorkingDay::StartOfDay(DateTime date)
{
	return (date.Date + StartOfDay());
}

DateTime WorkingDay::EndOfDay(DateTime date)
{
	return (date.Date + EndOfDay());
}

DateTime WorkingDay::StartOfLunch(DateTime date)
{
	return (date.Date + StartOfLunch());
}

DateTime WorkingDay::EndOfLunch(DateTime date)
{
	return (date.Date + EndOfLunch());
}

TimeSpan WorkingDay::StartOfDay()
{
	return TimeSpan::FromMilliseconds(HoursToMilleseconds(m_StartOfDayInHours));
}

TimeSpan WorkingDay::EndOfDay()
{
	return TimeSpan::FromMilliseconds(HoursToMilleseconds(EndOfDayInHours()));
}

TimeSpan WorkingDay::StartOfLunch()
{
	return TimeSpan::FromMilliseconds(HoursToMilleseconds(m_StartOfLunchInHours));
}

TimeSpan WorkingDay::EndOfLunch()
{
	return TimeSpan::FromMilliseconds(HoursToMilleseconds(m_EndOfLunchInHours));
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
		dDuration -= LunchLengthInHours();

	return Math::Max(dDuration, 0.0);
}

double WorkingDay::GetTimeOfDayInHours(DateTime date)
{
	return (double)(date.Hour + (date.Minute / 60.0) + (date.Second / 3600.0) + (date.Millisecond / 3600000));
}

double WorkingDay::DayLengthInHours(bool includingLunch)
{
	if (includingLunch)
		return (m_WorkingLengthInHours + LunchLengthInHours());

	// else
	return m_WorkingLengthInHours;
}

double WorkingDay::LunchLengthInHours()
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

WorkingWeek::WorkingWeek() 
	: 
	m_WorkingDay(gcnew WorkingDay()),
	m_WeekendDays(gcnew List<DayOfWeek>())
{
	m_WeekendDays->Add(DayOfWeek::Saturday);
	m_WeekendDays->Add(DayOfWeek::Sunday);
}

WorkingWeek::WorkingWeek(Preferences^ prefs)
	:
	m_WorkingDay(gcnew WorkingDay()),
	m_WeekendDays(gcnew List<DayOfWeek>())
{
	Load(prefs);
}

void WorkingWeek::Load(Preferences^ prefs)
{
	m_WorkingDay->Load(prefs);

	int dwWeekends = prefs->GetProfileInt("Preferences", "Weekends", 0);
	m_WeekendDays = DateUtil::MapDaysOfWeek(dwWeekends);
}

double WorkingWeek::CalculateDurationInHours(DateTime from, DateTime to)
{
	int nDaysDuration = (to.Date - from.Date).Days;

	if (nDaysDuration < 0)
		return 0;

	double fromTimeOfDay = m_WorkingDay->GetTimeOfDayInHours(from);
	double toTimeOfDay = m_WorkingDay->GetTimeOfDayInHours(to);
	double dHoursDuration = 0;

	if (nDaysDuration == 0)
	{
		// from and to are same day
		if (!IsWeekend(from))
			dHoursDuration = m_WorkingDay->CalculateDurationInHours(fromTimeOfDay, toTimeOfDay);
	}
	else
	{
		if (nDaysDuration > 1)
		{
			// count whole days
			from = from.AddDays(1).Date;

			while (from <= to)
			{
				if (!IsWeekend(from))
					dHoursDuration += m_WorkingDay->DayLengthInHours(false);

				from = from.AddDays(1);
			}
		}
		
		// part days
		if (!IsWeekend(from))
			dHoursDuration += m_WorkingDay->CalculateDurationInHours(fromTimeOfDay, m_WorkingDay->EndOfDayInHours());

		if (!IsWeekend(to))
			dHoursDuration += m_WorkingDay->CalculateDurationInHours(m_WorkingDay->StartOfDayInHours(), toTimeOfDay);
	}

	return dHoursDuration;
}

bool WorkingWeek::IsWeekend(DateTime date)
{
	return m_WeekendDays->Contains(date.DayOfWeek);
}

List<DayOfWeek>^ WorkingWeek::WeekDays()
{
	auto weekDays = DateUtil::AllDaysOfWeek();
	int i = m_WeekendDays->Count;

	while (i--)
		weekDays->Remove(m_WeekendDays[i]);

	return weekDays;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int DateUtil::WeekOfYear(DateTime date)
{
	return CDateHelper::GetWeekofYear(date.ToOADate());
}

int DateUtil::GetMaxDayOfWeekNameWidth(Graphics^ graphics, Font^ font, bool shortName)
{
	HDC hDC = Win32::GetHdc(graphics->GetHdc());

	HFONT hFont = Win32::GetHfont(font->ToHfont());
	HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);

	int width = CDateHelper::GetMaxDayOfWeekNameWidth(CDC::FromHandle(hDC), (shortName ? TRUE : FALSE));

	// cleanup
	::SelectObject(hDC, hOldFont);
	::DeleteObject(hFont);
	graphics->ReleaseHdc();

	return width;
}

int DateUtil::GetMaxMonthNameWidth(Graphics^ graphics, Font^ font, bool shortName)
{
	HDC hDC = Win32::GetHdc(graphics->GetHdc());

	HFONT hFont = Win32::GetHfont(font->ToHfont());
	HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);

	int width = CDateHelper::GetMaxMonthNameWidth(CDC::FromHandle(hDC), (shortName ? TRUE : FALSE));

	// cleanup
	::SelectObject(hDC, hOldFont);
	::DeleteObject(hFont);
	graphics->ReleaseHdc();

	return width;
}

String^ DateUtil::GetMonthName(int nMonth, bool shortName)
{
	return gcnew String(CDateHelper::GetMonthName(nMonth, (shortName ? TRUE : FALSE)));
}

int DateUtil::DateInMonths(DateTime date)
{
	return CDateHelper::GetDateInMonths(date.ToOADate());
}

DateTime DateUtil::DateFromMonths(int nMonths)
{
	return DateTime::FromOADate(CDateHelper::GetDateFromMonths(nMonths));
}

String^ DateUtil::FormatRange(DateTime dateFrom, DateTime dateTo, bool bWithTime, bool bISO)
{
	DWORD dwFlags = 0;
	
	if (bWithTime)
		dwFlags |= (DHFD_TIME | DHFD_NOSEC);

	if (bISO)
		dwFlags |= DHFD_ISO;

	return gcnew String(COleDateTimeRange(dateFrom.ToOADate(), dateTo.ToOADate()).Format(dwFlags));
}

String^ DateUtil::FormatDateOnlyRange(DateTime dateFrom, DateTime dateTo, String^ format)
{
	return gcnew String(COleDateTimeRange(dateFrom.ToOADate(), dateTo.ToOADate()).FormatDateOnly(MS(format), L" - "));
}

TimeSpan DateUtil::TimeOnly(DateTime date)
{
	return (date - date.Date);
}

int DateUtil::MapDayOfWeek(DayOfWeek day)
{
	switch (day)
	{
		case DayOfWeek::Sunday:		return DHW_SUNDAY;
		case DayOfWeek::Saturday:	return DHW_SATURDAY;
		case DayOfWeek::Monday:		return DHW_MONDAY;
		case DayOfWeek::Tuesday:	return DHW_TUESDAY;
		case DayOfWeek::Wednesday:	return DHW_WEDNESDAY;
		case DayOfWeek::Thursday:	return DHW_THURSDAY;
		case DayOfWeek::Friday:		return DHW_FRIDAY;
	}

	ASSERT(0);
	return 0;
}

int DateUtil::MapDaysOfWeek(List<DayOfWeek>^ days)
{
	int dwDays = 0;

	for (int i = 0; i < days->Count; i++)
		dwDays |= MapDayOfWeek(days[i]);

	return dwDays;
}

List<DayOfWeek>^ DateUtil::MapDaysOfWeek(int dwDays)
{
	auto days = gcnew List<DayOfWeek>();

	if (Misc::HasFlag(dwDays, DHW_SUNDAY))
		days->Add(DayOfWeek::Sunday);

	if (Misc::HasFlag(dwDays, DHW_SATURDAY))
		days->Add(DayOfWeek::Saturday);

	if (Misc::HasFlag(dwDays, DHW_MONDAY))
		days->Add(DayOfWeek::Monday);

	if (Misc::HasFlag(dwDays, DHW_TUESDAY))
		days->Add(DayOfWeek::Tuesday);

	if (Misc::HasFlag(dwDays, DHW_WEDNESDAY))
		days->Add(DayOfWeek::Wednesday);

	if (Misc::HasFlag(dwDays, DHW_THURSDAY))
		days->Add(DayOfWeek::Thursday);

	if (Misc::HasFlag(dwDays, DHW_FRIDAY))
		days->Add(DayOfWeek::Friday);

	return days;
}

List<DayOfWeek>^ DateUtil::AllDaysOfWeek()
{
	auto days = gcnew List<DayOfWeek>();

	days->Add(DayOfWeek::Sunday);
	days->Add(DayOfWeek::Saturday);
	days->Add(DayOfWeek::Monday);
	days->Add(DayOfWeek::Tuesday);
	days->Add(DayOfWeek::Wednesday);
	days->Add(DayOfWeek::Thursday);
	days->Add(DayOfWeek::Friday);

	return days;
}

bool DateUtil::IsAllDaysOfWeek(Collections::Generic::List<DayOfWeek>^ days)
{
	if (days->Count < 7)
		return false;

	return (MapDaysOfWeek(days) == DHW_ALL);
}

void DateUtil::SetShortDateFormat(Windows::Forms::DateTimePicker^ ctrl, bool isoFormat)
{
	if (isoFormat)
	{
		ctrl->Format = DateTimePickerFormat::Custom;
		ctrl->CustomFormat = gcnew String(Misc::GetShortDateFormat(FALSE, TRUE));
	}
	else
	{
		ctrl->Format = DateTimePickerFormat::Short;
	}
}

bool DateUtil::WantRTLDates()
{
	return (CDateHelper::WantRTLDates() != FALSE);
}
