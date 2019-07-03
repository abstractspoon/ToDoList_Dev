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

static CWorkingDay	s_WorkDay(9, 8, 12, 13);
static CWeekend		s_Weekend(DHW_SATURDAY | DHW_SUNDAY);

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
	m_dStartOfDayInHours(0),
	m_dStartOfLunchInHours(0),
	m_dEndOfLunchInHours(0),
	m_dWorkingLengthInHours(0)
{
	*this = s_WorkDay;

	Initialise(dStartOfDayInHours, dWorkingLengthInHours, dStartOfLunchInHours, dEndOfLunchInHours);
}

BOOL CWorkingDay::IsValid(double dStartOfDayInHours,
						  double dWorkingLengthInHours,
						  double dStartOfLunchInHours,
						  double dEndOfLunchInHours)
{
	if ((dStartOfDayInHours + dWorkingLengthInHours) > 24)
		return FALSE;

	if (dStartOfLunchInHours < dStartOfDayInHours)
		return FALSE;

	if (dEndOfLunchInHours < dStartOfLunchInHours)
		return FALSE;

	if (dEndOfLunchInHours > (dStartOfDayInHours + dWorkingLengthInHours))
		return FALSE;

	return TRUE;
}

void CWorkingDay::Initialise(const CWorkingDay& workday)
{
	// workday must already be valid
	s_WorkDay = workday;

	// TEMPORARY
	CTimeHelper::SetHoursInWorkday(workday.GetLengthOfDayInHours());
	CTimeHelper::SetStartOfWorkday(workday.GetStartOfDayInHours());
	CTimeHelper::SetLunchBreak(workday.GetStartOfLunchInHours(), workday.GetEndOfLunchInHours());
}

BOOL CWorkingDay::Initialise(double dStartOfDayInHours,
							 double dWorkingLengthInHours,
							 double dStartOfLunchInHours,
							 double dEndOfLunchInHours)
{
	if (IsValid(dStartOfDayInHours, 
				dWorkingLengthInHours, 
				dStartOfLunchInHours, 
				dEndOfLunchInHours))
	{
		m_dStartOfDayInHours = dStartOfDayInHours;
		m_dStartOfLunchInHours = dStartOfLunchInHours;
		m_dEndOfLunchInHours = dEndOfLunchInHours;
		m_dWorkingLengthInHours = dWorkingLengthInHours;

		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

double CWorkingDay::GetStartOfDayInHours() const
{
	return m_dStartOfDayInHours;
}

double CWorkingDay::GetEndOfDayInHours() const
{
	return (m_dStartOfDayInHours + GetLengthOfDayInHours(true));
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

double CWorkingDay::GetLengthOfDayInHours(bool includingLunch) const
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

CWeekend::CWeekend(DWORD dwDays) : m_dwDays(0)
{
	*this = s_Weekend;
	
	Initialise(dwDays);
}

void CWeekend::Initialise(const CWeekend& weekend)
{
	// weekend must already be valid
	s_Weekend = weekend;

	// TEMPORARY
	CTimeHelper::SetWorkdaysInWeek(7 - weekend.GetLengthInDays());
}

BOOL CWeekend::Initialise(DWORD dwWeekendDays)
{
	if (!IsValid(dwWeekendDays))
	{
		ASSERT(0);
		return FALSE;
	}

	m_dwDays = (dwWeekendDays & DHW_EVERYDAY);

	// Calculate duration once only
	m_nLength = 0;

	if (m_dwDays != 0)
	{
		for (int nDay = 0; nDay < 7; nDay++)
		{
			if (IsWeekend(OLE_DAYSOFWEEK[nDay]))
				m_nLength++;
		}
	}
	
	return TRUE;
}

BOOL CWeekend::IsValid(DWORD dwWeekendDays)
{
	if (dwWeekendDays != 0)
	{
		dwWeekendDays &= DHW_EVERYDAY;

		if (dwWeekendDays == 0)
			return FALSE;

		if (dwWeekendDays == DHW_EVERYDAY)
			return FALSE;
	}

	return TRUE;
}

BOOL CWeekend::IsWeekend(const COleDateTime& date) const
{
	ASSERT(CDateHelper::IsDateSet(date));

	if (m_dwDays)
		return IsWeekend(CDateHelper::GetDayOfWeek(date));

	// else
	return FALSE;
}

BOOL CWeekend::IsWeekend(double dDate) const
{
	if (m_dwDays)
		return IsWeekend(COleDateTime(dDate));

	// else
	return FALSE;
}

BOOL CWeekend::IsWeekend(DH_DAYOFWEEK nDOW) const
{
	return ((m_dwDays & nDOW) != 0);
}

BOOL CWeekend::IsWeekend(OLE_DAYOFWEEK nDOW) const
{
	if (m_dwDays)
		return IsWeekend(CDateHelper::Map(nDOW));

	// else
	return FALSE;
}

int CWeekend::GetLengthInDays() const
{
	return m_nLength;
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

void CWorkingWeek::Initialise(const CWorkingWeek& week)
{
	// week must already be valid
	CWorkingDay::Initialise(week.WorkDay());
	CWeekend::Initialise(week.Weekend());
}

BOOL CWorkingWeek::Initialise(DWORD dwWeekendDays, double dStartOfDayInHours, double dWorkingLengthInHours, double dStartOfLunchInHours, double dEndOfLunchInHours)
{
	// prevent partial initialisation
	if (!CWorkingDay::IsValid(dStartOfDayInHours, dWorkingLengthInHours, dStartOfLunchInHours, dEndOfLunchInHours))
		return FALSE;

	if (!CWeekend::IsValid(dwWeekendDays))
		return FALSE;

	m_Weekend.Initialise(dwWeekendDays);
	m_WorkDay.Initialise(dStartOfDayInHours, dWorkingLengthInHours, dStartOfLunchInHours, dEndOfLunchInHours);

	return TRUE;
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
		if (!Weekend().IsWeekend(dtFrom))
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
				if (!Weekend().IsWeekend(dFrom))
					dHoursDuration += m_WorkDay.GetLengthOfDayInHours();

				dFrom++;
			}
		}

		// part days
		if (!Weekend().IsWeekend(dtFrom))
			dHoursDuration += m_WorkDay.CalculateDurationInHours(fromTimeOfDay, m_WorkDay.GetEndOfDayInHours());

		if (!Weekend().IsWeekend(dtTo))
			dHoursDuration += m_WorkDay.CalculateDurationInHours(m_WorkDay.GetStartOfDayInHours(), toTimeOfDay);
	}

	return dHoursDuration;
}

double CWorkingWeek::CalculateDurationInDays(const COleDateTime& dtFrom, const COleDateTime& dtTo)
{
	double dDayLen = m_WorkDay.GetLengthOfDayInHours();

	if (dDayLen == 0.0)
		return 0.0;

	return (CalculateDurationInHours(dtFrom, dtTo) / dDayLen);
}

double CWorkingWeek::CalculateDurationInWeeks(const COleDateTime& dtFrom, const COleDateTime& dtTo)
{
	int nNumWeekdays = (7 - Weekend().GetLengthInDays());

	if (nNumWeekdays == 0)
		return 0.0;

	// else
	return (CalculateDurationInDays(dtFrom, dtTo) / nNumWeekdays);
}

BOOL CWorkingWeek::HasWeekend() const
{
	return (Weekend().GetDays() != 0);
}

int CWorkingWeek::GetLengthInDays() const
{
	return (7 - Weekend().GetLengthInDays());
}

DWORD CWorkingWeek::GetWorkingDays() const
{
	return (DHW_EVERYDAY & ~Weekend().GetDays());
}

COleDateTime CWorkingWeek::ToWeekday(const COleDateTime& date, BOOL bForwards) const
{
	COleDateTime weekday(date);
	MakeWeekday(weekday, bForwards);

	return weekday;
}

BOOL CWorkingWeek::MakeWeekday(COleDateTime& date, BOOL bForwards, BOOL bTruncateTime) const
{
	if (!CDateHelper::IsDateSet(date))
	{
		ASSERT(0);
		return FALSE;
	}

	if (bTruncateTime)
		date = CDateHelper::GetDateOnly(date);

	if (HasWeekend())
	{
		while (Weekend().IsWeekend(date))
		{
			if (bForwards)
				date.m_dt++;
			else
				date.m_dt--;
		}
	}

	return TRUE;
}

