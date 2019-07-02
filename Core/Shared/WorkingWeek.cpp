// DateHelper.cpp: implementation of the CDateHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DateHelper.h"
#include "TimeHelper.h"
#include "WorkingWeek.h"

/////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

// Used by CWorkingWeek
static CWorkingDay	s_WorkDay(9, 8, 12, 13);
static CWeekend		s_Weekend(DHW_SATURDAY | DHW_SUNDAY);

//////////////////////////////////////////////////////////////////////

// Global
WD_DAYOFWEEK Map(OLE_DAYOFWEEK nDOW)
{
	switch (nDOW)
	{
	case DHO_SUNDAY:	return WD_SUNDAY;
	case DHO_MONDAY:	return WD_MONDAY;
	case DHO_TUESDAY:	return WD_TUESDAY;
	case DHO_WEDNESDAY:	return WD_WEDNESDAY;
	case DHO_THURSDAY:	return WD_THURSDAY;
	case DHO_FRIDAY:	return WD_FRIDAY;
	case DHO_SATURDAY:	return WD_SATURDAY;
	}

	ASSERT(0);
	return WD_NONE;
}

OLE_DAYOFWEEK Map(WD_DAYOFWEEK nDOW)
{
	switch (nDOW)
	{
	case WD_SUNDAY:		return DHO_SUNDAY;
	case WD_MONDAY:		return DHO_MONDAY;
	case WD_TUESDAY:	return DHO_TUESDAY;
	case WD_WEDNESDAY:	return DHO_WEDNESDAY;
	case WD_THURSDAY:	return DHO_THURSDAY;
	case WD_FRIDAY:		return DHO_FRIDAY;
	case WD_SATURDAY:	return DHO_SATURDAY;
	}

	ASSERT(0);
	return DHO_UNDEF;
}

//////////////////////////////////////////////////////////////////////

CWorkingDay::CWorkingDay()
	:
	m_dStartOfDayInHours(0),
	m_dStartOfLunchInHours(0),
	m_dEndOfLunchInHours(0),
	m_dWorkingLengthInHours(0)
{
	*this = s_WorkDay;
}

CWorkingDay::CWorkingDay(double dStartOfDayInHours, double dWorkingLengthInHours, double dStartOfLunchInHours, double dEndOfLunchInHours)
	:
	m_dStartOfDayInHours(dStartOfDayInHours),
	m_dStartOfLunchInHours(dStartOfLunchInHours),
	m_dEndOfLunchInHours(dEndOfLunchInHours),
	m_dWorkingLengthInHours(dWorkingLengthInHours)
{
}

double CWorkingDay::GetStartOfDayInHours() const
{
	return m_dStartOfDayInHours;
}

double CWorkingDay::GetEndOfDayInHours() const
{
	return (m_dStartOfDayInHours + GetDayLengthInHours(true));
}

double CWorkingDay::GetStartOfLunchInHours() const
{
	return m_dStartOfLunchInHours;
}

double CWorkingDay::GetEndOfLunchInHours() const
{
	return m_dEndOfLunchInHours;
}

COleDateTime CWorkingDay::GetStartOfDay(const COleDateTime& date) const
{
	return CDateHelper::MakeDate(date, (m_dStartOfDayInHours / 24));
}

COleDateTime CWorkingDay::GetEndOfDay(const COleDateTime& date) const
{
	return CDateHelper::MakeDate(date, (GetEndOfDayInHours() / 24));
}

COleDateTime CWorkingDay::GetStartOfLunch(const COleDateTime& date) const
{
	return CDateHelper::MakeDate(date, (m_dStartOfLunchInHours / 24));
}

COleDateTime CWorkingDay::GetEndOfLunch(const COleDateTime& date) const
{
	return CDateHelper::MakeDate(date, (m_dEndOfLunchInHours / 24));
}

double CWorkingDay::CalculateDurationInHours(double fromHour, double toHour) const
{
	fromHour = max(fromHour, m_dStartOfDayInHours);
	toHour = min(toHour, GetEndOfDayInHours());

	if (fromHour >= toHour)
		return 0;

	if (fromHour >= m_dStartOfLunchInHours)
		fromHour = max(fromHour, m_dEndOfLunchInHours);

	if (toHour <= m_dEndOfLunchInHours)
		toHour = min(toHour, m_dStartOfLunchInHours);

	double dDuration = (toHour - fromHour);

	if ((fromHour < m_dStartOfLunchInHours) && (toHour > m_dEndOfLunchInHours))
		dDuration -= GetLunchLengthInHours();

	return max(dDuration, 0.0);
}

double CWorkingDay::GetTimeOfDayInHours(const COleDateTime& date)
{
	return (CDateHelper::GetTimeOnly(date).m_dt * 24);
}

double CWorkingDay::GetDayLengthInHours(bool includingLunch) const
{
	if (includingLunch)
		return (m_dWorkingLengthInHours + GetLunchLengthInHours());

	// else
	return m_dWorkingLengthInHours;
}

double CWorkingDay::GetLunchLengthInHours() const
{
	return (m_dEndOfLunchInHours - m_dStartOfLunchInHours);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CWeekend::CWeekend() : m_dwDays(0)
{
	*this = s_Weekend;
}

CWeekend::CWeekend(DWORD dwDays) : m_dwDays(dwDays)
{

}

COleDateTime CWeekend::ToWeekday(const COleDateTime& date, BOOL bForwards) const
{
	COleDateTime weekday(date);
	MakeWeekday(weekday, bForwards);

	return weekday;
}

BOOL CWeekend::MakeWeekday(COleDateTime& date, BOOL bForwards, BOOL bTruncateTime) const
{
	ASSERT(CDateHelper::IsDateSet(date));

	// check we don't have a 7-day weekend
	if (!HasWeekend() || !CDateHelper::IsDateSet(date))
	{
		ASSERT(0);
		return FALSE;
	}

	COleDateTime dtOrg(date);

	while (IsWeekend(date))
	{
		if (bTruncateTime)
			date = CDateHelper::GetDateOnly(date);

		if (bForwards)
			date.m_dt++;
		else
			date.m_dt--;
	}

	return TRUE;
}

BOOL CWeekend::IsWeekend(const COleDateTime& date) const
{
	ASSERT(CDateHelper::IsDateSet(date));

	return IsWeekend(CDateHelper::GetDayOfWeek(date));
}

BOOL CWeekend::IsWeekend(double dDate) const
{
	return IsWeekend(COleDateTime(dDate));
}

BOOL CWeekend::IsWeekend(WD_DAYOFWEEK nDOW) const
{
	return ((m_dwDays & nDOW) != 0);
}

BOOL CWeekend::IsWeekend(OLE_DAYOFWEEK nDOW) const
{
	return IsWeekend(Map(nDOW));
}

int CWeekend::GetWeekendDuration() const
{
	int nDuration = 0;

	for (int nDay = 1; nDay <= 7; nDay++)
	{
		if (IsWeekend(OLE_DAYSOFWEEK[nDay]))
			nDuration++;
	}

	return nDuration;
}

BOOL CWeekend::HasWeekend() const
{
	return ((m_dwDays != 0) && ((m_dwDays & WD_EVERYDAY) != WD_EVERYDAY));
}

////////////////////////////////////////////////////////////////////////////////////////////////

CWorkingWeek::CWorkingWeek()
{
	m_WorkDay = s_WorkDay;
	m_Weekend = s_Weekend;
}

CWorkingWeek::CWorkingWeek(DWORD dwWeekendDays, double dStartOfDayInHours, double dWorkingLengthInHours, double dStartOfLunchInHours, double dEndOfLunchInHours)
	:
	m_WorkDay(dStartOfDayInHours, dWorkingLengthInHours, dStartOfLunchInHours, dEndOfLunchInHours),
	m_Weekend(dwWeekendDays)
{
}

void CWorkingWeek::Initialise(DWORD dwWeekendDays, double dStartOfDayInHours, double dWorkingLengthInHours, double dStartOfLunchInHours, double dEndOfLunchInHours)
{
	s_Weekend.m_dwDays = dwWeekendDays;

	s_WorkDay.m_dStartOfDayInHours = dStartOfDayInHours;
	s_WorkDay.m_dWorkingLengthInHours = dWorkingLengthInHours;
	s_WorkDay.m_dStartOfLunchInHours = dStartOfLunchInHours;
	s_WorkDay.m_dEndOfLunchInHours = dEndOfLunchInHours;
}

double CWorkingWeek::CalculateDurationInHours(const COleDateTime& dtFrom, const COleDateTime& dtTo)
{
	int nDaysDuration = (int)((int)dtTo.m_dt - (int)dtFrom.m_dt);

	if (nDaysDuration < 0)
		return 0;

	double fromTimeOfDay = m_WorkDay.GetTimeOfDayInHours(dtFrom);
	double toTimeOfDay = m_WorkDay.GetTimeOfDayInHours(dtTo);
	double dHoursDuration = 0;

	if (nDaysDuration == 0)
	{
		// from and to are same day
		if (!CDateHelper::IsWeekend(dtFrom))
			dHoursDuration = m_WorkDay.CalculateDurationInHours(fromTimeOfDay, toTimeOfDay);
	}
	else
	{
		if (nDaysDuration > 1)
		{
			// count whole days
			double dFrom = CDateHelper::GetDateOnly(dtFrom).m_dt;
			double dTo = CDateHelper::GetDateOnly(dtTo).m_dt;

			while (dFrom <= dtTo)
			{
				if (!CDateHelper::IsWeekend(dFrom))
					dHoursDuration += m_WorkDay.GetDayLengthInHours();

				dFrom++;
			}
		}

		// part days
		if (!CDateHelper::IsWeekend(dtFrom))
			dHoursDuration += m_WorkDay.CalculateDurationInHours(fromTimeOfDay, m_WorkDay.GetEndOfDayInHours());

		if (!CDateHelper::IsWeekend(dtTo))
			dHoursDuration += m_WorkDay.CalculateDurationInHours(m_WorkDay.GetStartOfDayInHours(), toTimeOfDay);
	}

	return dHoursDuration;
}

double CWorkingWeek::CalculateDurationInDays(const COleDateTime& dtFrom, const COleDateTime& dtTo)
{
	double dDayLen = m_WorkDay.GetDayLengthInHours();

	if (dDayLen == 0.0)
		return 0.0;

	return (CalculateDurationInHours(dtFrom, dtTo) / dDayLen);
}

double CWorkingWeek::CalculateDurationInWeeks(const COleDateTime& dtFrom, const COleDateTime& dtTo)
{
	int nNumWeekdays = (7 - CDateHelper::GetWeekendDuration());

	if (nNumWeekdays == 0)
		return 0.0;

	// else
	return (CalculateDurationInDays(dtFrom, dtTo) / nNumWeekdays);
}
