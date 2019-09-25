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

CWorkingDay::CWorkingDay(double dWorkingLengthInHours)
{
	if (dWorkingLengthInHours <= 24)
	{
		m_dWorkingLengthInHours = dWorkingLengthInHours;
		m_dStartOfDayInHours = ((24 - m_dWorkingLengthInHours) / 2);
		m_dStartOfLunchInHours = m_dEndOfLunchInHours = 12; // No lunch break
	}
}

CWorkingDay::CWorkingDay(double dStartOfDayInHours, double dWorkingLengthInHours, double dStartOfLunchInHours, double dEndOfLunchInHours)
	:
	m_dStartOfDayInHours(0),
	m_dStartOfLunchInHours(0),
	m_dEndOfLunchInHours(0),
	m_dWorkingLengthInHours(0)
{
	*this = s_WorkDay;

	if (IsValid(dStartOfDayInHours, dWorkingLengthInHours, dStartOfLunchInHours, dEndOfLunchInHours))
	{
		m_dStartOfDayInHours = dStartOfDayInHours;
		m_dStartOfLunchInHours = dStartOfLunchInHours;
		m_dEndOfLunchInHours = dEndOfLunchInHours;
		m_dWorkingLengthInHours = dWorkingLengthInHours;
	}
}

CWorkingDay::CWorkingDay(const CWorkingDay& workDay)
{
	// Work day must already be valid
	*this = workDay;
}

BOOL CWorkingDay::IsValid(double dWorkingLengthInHours,
						  double dStartOfDayInHours,
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

BOOL CWorkingDay::Initialise(double dWorkingLengthInHours,
							 double dStartOfDayInHours,
							 double dStartOfLunchInHours,
							 double dEndOfLunchInHours)
{
	if (IsValid(dWorkingLengthInHours, 
				dStartOfDayInHours, 
				dStartOfLunchInHours, 
				dEndOfLunchInHours))
	{
		s_WorkDay.m_dStartOfDayInHours = dStartOfDayInHours;
		s_WorkDay.m_dStartOfLunchInHours = dStartOfLunchInHours;
		s_WorkDay.m_dEndOfLunchInHours = dEndOfLunchInHours;
		s_WorkDay.m_dWorkingLengthInHours = dWorkingLengthInHours;

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
	return (m_dStartOfDayInHours + GetLengthInHours(true));
}

double CWorkingDay::GetStartOfLunchInHours() const
{
	return m_dStartOfLunchInHours;
}

double CWorkingDay::GetEndOfLunchInHours() const
{
	return m_dEndOfLunchInHours;
}

double CWorkingDay::GetMiddleOfDayInHours() const
{
	return ((GetStartOfDayInHours() + GetEndOfDayInHours()) / 2);
}

COleDateTime CWorkingDay::GetMiddleOfDay(const COleDateTime& date) const
{
	return CDateHelper::MakeDate(date, (GetMiddleOfDayInHours() / 24));
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

double CWorkingDay::GetLengthInHours(bool includingLunch) const
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

BOOL CWorkingDay::Contains(const COleDateTime& date) const
{
	double dTime = GetTimeOfDayInHours(date);

	return ((dTime >= m_dStartOfDayInHours) && (dTime <= GetEndOfDayInHours()));
}

BOOL CWorkingDay::Contains(const COleDateTime& dtStart, const COleDateTime& dtEnd) const
{
	if (!CDateHelper::IsSameDay(dtStart, dtEnd))
		return FALSE;

	return (Contains(dtStart) && Contains(dtEnd));
}

BOOL CWorkingDay::Contains(const COleDateTimeRange& dtRange) const
{
	return Contains(dtRange.GetStart(), dtRange.GetEndInclusive());
}

BOOL CWorkingDay::IsDuringLunch(const COleDateTime& date) const
{
	double dTime = GetTimeOfDayInHours(date);

	return ((dTime > m_dStartOfLunchInHours) && (dTime < m_dEndOfLunchInHours));
}

////////////////////////////////////////////////////////////////////////////////////////////////

CWeekend::CWeekend() : m_dwDays(0), m_nLength(0)
{
	*this = s_Weekend;
}

CWeekend::CWeekend(DWORD dwDays) : m_dwDays(0)
{
	*this = s_Weekend;
	
	if (IsValid(dwDays))
	{
		m_dwDays = dwDays;
		m_nLength = CalcLength(m_dwDays);
	}
}

CWeekend::CWeekend(const CWeekend& weekend)
{
	// weekend must already be valid
	*this = weekend;
}

BOOL CWeekend::Initialise(DWORD dwWeekendDays)
{
	if (!IsValid(dwWeekendDays))
	{
		ASSERT(0);
		return FALSE;
	}

	s_Weekend.m_dwDays = dwWeekendDays;
	s_Weekend.m_nLength = CalcLength(dwWeekendDays);

	return TRUE;
}

int CWeekend::CalcLength(DWORD dwDays)
{
	if (!IsValid(dwDays))
	{
		ASSERT(0);
		return 0;
	}

	int nLength = 0;

	for (int nDay = 0; nDay < 7; nDay++)
	{
		if (dwDays & CDateHelper::Map(OLE_DAYSOFWEEK[nDay]))
			nLength++;
	}

	return nLength;
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

CWorkingWeek::CWorkingWeek(DWORD dwWeekendDays,	double dWorkingLengthInHours)
	:
	m_WorkDay(dWorkingLengthInHours),
	m_Weekend(dwWeekendDays)
{

}

CWorkingWeek::CWorkingWeek(const CWorkingWeek& week)
{
	// Week must already be valid
	*this = week;
}

// static 
BOOL CWorkingWeek::Initialise(DWORD dwWeekendDays, double dWorkingLengthInHours)
{
	return Initialise(dwWeekendDays, dWorkingLengthInHours, 
					  0, 12, 12); // don't care
}

BOOL CWorkingWeek::Initialise(DWORD dwWeekendDays, double dWorkingLengthInHours, double dStartOfDayInHours, double dStartOfLunchInHours, double dEndOfLunchInHours)
{
	// prevent partial initialisation
	if (!CWorkingDay::IsValid(dStartOfDayInHours, dWorkingLengthInHours, dStartOfLunchInHours, dEndOfLunchInHours))
		return FALSE;

	if (!CWeekend::IsValid(dwWeekendDays))
		return FALSE;

	CWeekend::Initialise(dwWeekendDays);
	CWorkingDay::Initialise(dWorkingLengthInHours, dStartOfDayInHours, dStartOfLunchInHours, dEndOfLunchInHours);

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
			double dFrom = CDateHelper::GetDateOnly(dtFrom).m_dt + 1;
			double dTo = CDateHelper::GetDateOnly(dtTo).m_dt - 1;

			while (dFrom < dTo)
			{
				if (!Weekend().IsWeekend(dFrom))
					dHoursDuration += m_WorkDay.GetLengthInHours();

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
	double dDayLen = m_WorkDay.GetLengthInHours();

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

double CWorkingWeek::GetLengthInHours(bool bIncludingLunch) const
{
	return (GetLengthInDays() * WorkDay().GetLengthInHours(bIncludingLunch));
}

double CWorkingWeek::GetLengthInDaysAsRatio() const
{
	return (GetLengthInDays() / 7.0);
}

double CWorkingWeek::GetLengthInHoursAsRatio(bool bIncludingLunch) const
{
	return (GetLengthInHours(bIncludingLunch) / (7.0 * 24));
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
	
	if (!HasWeekend())
	{
		return FALSE;
	}

	if (Weekend().IsWeekend(date))
	{
		if (bTruncateTime)
			date = CDateHelper::GetDateOnly(date);

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

