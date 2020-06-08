// DateHelper.cpp: implementation of the CDateHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DateHelper.h"
#include "TimeHelper.h"
#include "WorkingWeek.h"
#include "Misc.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static CWorkingDay	s_WorkDay(8, 9, 12, 13);
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

CWorkingDay::CWorkingDay(double dWorkingLengthInHours, double dStartOfDayInHours, double dStartOfLunchInHours, double dEndOfLunchInHours)
	:
	m_dStartOfDayInHours(0),
	m_dStartOfLunchInHours(0),
	m_dEndOfLunchInHours(0),
	m_dWorkingLengthInHours(0)
{
	*this = s_WorkDay;

	if (IsValid(dWorkingLengthInHours, dStartOfDayInHours, dStartOfLunchInHours, dEndOfLunchInHours))
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

BOOL CWorkingDay::operator==(const CWorkingDay& other) const
{
	// A twenty-four hour day has no start of day or lunch break
	if ((m_dWorkingLengthInHours == 24.0) && (other.m_dWorkingLengthInHours == 24.0))
		return TRUE;

	// else check all attributes
	return ((m_dWorkingLengthInHours == other.m_dWorkingLengthInHours) &&
			(m_dStartOfDayInHours == other.m_dStartOfDayInHours) &&
			(m_dStartOfLunchInHours == other.m_dStartOfDayInHours) &&
			(m_dEndOfLunchInHours == other.m_dEndOfLunchInHours));
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
		s_WorkDay.m_dWorkingLengthInHours = dWorkingLengthInHours;
		s_WorkDay.m_dStartOfDayInHours = dStartOfDayInHours;
		s_WorkDay.m_dStartOfLunchInHours = dStartOfLunchInHours;
		s_WorkDay.m_dEndOfLunchInHours = dEndOfLunchInHours;

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

// NOTE: Caller's responsibility to check for weekends
void CWorkingDay::AddDurationInHours(COleDateTime& date, double& dHours) const
{
#ifdef _DEBUG
	COleDateTime dtOrg(date);
	double dOrgHours(dHours);
#endif

	do // easy to break out of
	{
		if (dHours > 0)
		{
			if (date < GetStartOfDay(date))
				date = GetStartOfDay(date);

			// Hours before lunch
			double dStartHour = GetTimeOfDayInHours(date);
			double dHoursBeforeLunch = (GetStartOfLunchInHours() - dStartHour);

			if (dHoursBeforeLunch > 0)
			{
				if (dHours <= dHoursBeforeLunch)
				{
					date.m_dt += (dHours / 24.0);
					dHours = 0.0;

					break; // done
				}

				// else
				date = GetEndOfLunch(date);
				dHours -= dHoursBeforeLunch;

				dStartHour = GetEndOfLunchInHours();
			}

			// Hours after lunch
			double dHoursAfterLunch = (GetEndOfDayInHours() - dStartHour);

			if (dHoursAfterLunch > 0)
			{
				if (dHours <= dHoursAfterLunch)
				{
					date.m_dt += (dHours / 24.0);
					dHours = 0.0;

					break; // done
				}

				// else
				date = GetEndOfDay(date);
				dHours -= dHoursAfterLunch;
			}
		}
		else if (dHours < 0)
		{
			if (date > GetEndOfDay(date))
				date = GetEndOfDay(date);

			double dStartHour = GetTimeOfDayInHours(date);

			// Hours after lunch
			double dHoursAfterLunch = (dStartHour - GetEndOfLunchInHours());

			if (dHoursAfterLunch > 0)
			{
				if (fabs(dHours) <= dHoursAfterLunch)
				{
					date.m_dt += (dHours / 24.0);
					dHours = 0.0;

					break; // done
				}

				// else
				date = GetStartOfLunch(date);
				dHours -= dHoursAfterLunch;

				dStartHour = GetStartOfLunchInHours();
			}

			// Hours before lunch
			double dHoursBeforeLunch = (dStartHour - GetStartOfDayInHours());

			if (dHoursBeforeLunch > 0)
			{
				if (fabs(dHours) <= dHoursBeforeLunch)
				{
					date.m_dt += (dHours / 24.0);
					dHours = 0.0;

					break; // done
				}

				// else
				date = GetStartOfDay(date);
				dHours -= dHoursBeforeLunch;
			}
		}
	} 
	while (false);

#ifdef _DEBUG
	ASSERT(CDateHelper::IsSameDay(date, dtOrg));

	double dCheckHours = CalculateDurationInHours(GetTimeOfDayInHours(dtOrg), GetTimeOfDayInHours(date));
	ASSERT(fabs(dCheckHours - (dOrgHours - dHours)) < 0.001);
#endif
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

BOOL CWorkingDay::IsEndOfDay(const COleDateTime& date) const
{
	return (GetTimeOfDayInHours(date) >= GetEndOfDayInHours());
}

BOOL CWorkingDay::IsStartOfDay(const COleDateTime& date) const
{
	return (GetTimeOfDayInHours(date) <= GetStartOfDayInHours());
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

BOOL CWeekend::operator==(const CWeekend& other) const
{
	return (m_dwDays == other.m_dwDays);
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

CWorkingWeek::CWorkingWeek(DWORD dwWeekendDays, double dWorkingLengthInHours, double dStartOfDayInHours, double dStartOfLunchInHours, double dEndOfLunchInHours)
	:
	m_WorkDay(dWorkingLengthInHours, dStartOfDayInHours, dStartOfLunchInHours, dEndOfLunchInHours),
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

BOOL CWorkingWeek::operator==(const CWorkingWeek& other) const
{
	return ((m_Weekend == other.m_Weekend) && 
			(m_WorkDay == other.m_WorkDay));
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
	if (!CWorkingDay::IsValid(dWorkingLengthInHours, dStartOfDayInHours, dStartOfLunchInHours, dEndOfLunchInHours))
		return FALSE;

	if (!CWeekend::IsValid(dwWeekendDays))
		return FALSE;

	CWeekend::Initialise(dwWeekendDays);
	CWorkingDay::Initialise(dWorkingLengthInHours, dStartOfDayInHours, dStartOfLunchInHours, dEndOfLunchInHours);

	return TRUE;
}

double CWorkingWeek::CalculateDurationInHours(const COleDateTime& dtFrom, const COleDateTime& dtTo) const
{
	double dFromTime = m_WorkDay.GetTimeOfDayInHours(dtFrom);
	double dToTime = m_WorkDay.GetTimeOfDayInHours(dtTo);

	double dHoursDuration = 0.0; 

	if (CDateHelper::IsSameDay(dtFrom, dtTo))
	{
		if (!m_Weekend.IsWeekend(dtFrom))
			dHoursDuration = m_WorkDay.CalculateDurationInHours(dFromTime, dToTime);
	}
	else
	{
		// First day
		if (!m_Weekend.IsWeekend(dtFrom))
			dHoursDuration = m_WorkDay.CalculateDurationInHours(dFromTime, m_WorkDay.GetEndOfDayInHours());

		// whole days
		double dStart = CDateHelper::GetDateOnly(dtFrom).m_dt + 1;
		double dEnd = CDateHelper::GetDateOnly(dtTo).m_dt;

		while (dStart < dEnd)
		{
			if (!m_Weekend.IsWeekend(dStart))
				dHoursDuration += m_WorkDay.GetLengthInHours();

			dStart++;
		}

		// Last day
		if (!m_Weekend.IsWeekend(dStart))
			dHoursDuration += m_WorkDay.CalculateDurationInHours(m_WorkDay.GetStartOfDayInHours(), dToTime);
	}

	// round to the nearest second
	dHoursDuration = (Misc::Round(dHoursDuration * 60 * 60) / 3600.0);

	return dHoursDuration;
}

double CWorkingWeek::CalculateDurationInMinutes(const COleDateTime& dtFrom, const COleDateTime& dtTo) const
{
	return (CalculateDurationInHours(dtFrom, dtTo) * 60.0);
}

double CWorkingWeek::CalculateDurationInDays(const COleDateTime& dtFrom, const COleDateTime& dtTo) const
{
	double dDayLen = m_WorkDay.GetLengthInHours();

	if (dDayLen == 0.0)
		return 0.0;

	return (CalculateDurationInHours(dtFrom, dtTo) / dDayLen);
}

double CWorkingWeek::CalculateDurationInWeeks(const COleDateTime& dtFrom, const COleDateTime& dtTo) const
{
	int nNumWeekdays = (7 - m_Weekend.GetLengthInDays());

	if (nNumWeekdays == 0)
		return 0.0;

	return (CalculateDurationInDays(dtFrom, dtTo) / nNumWeekdays);
}

double CWorkingWeek::CalculateDuration(const COleDateTime& dtFrom, const COleDateTime& dtTo, WW_UNITS nUnits) const
{
	switch (nUnits)
	{
		case WWD_MINS:	return CalculateDurationInMinutes(dtFrom, dtTo);
		case WWD_HOURS:	return CalculateDurationInHours(dtFrom, dtTo);
		case WWD_DAYS:	return CalculateDurationInDays(dtFrom, dtTo);
		case WWD_WEEKS: return CalculateDurationInWeeks(dtFrom, dtTo);
	}

	ASSERT(0);
	return 0.0;
}

COleDateTime CWorkingWeek::AddDuration(COleDateTime& dtFrom, double dAmount, WW_UNITS nUnits) const
{
	switch (nUnits)
	{
		case WWD_MINS:	return AddDurationInMinutes(dtFrom, dAmount);
		case WWD_HOURS:	return AddDurationInHours(dtFrom, dAmount);
		case WWD_DAYS:	return AddDurationInDays(dtFrom, dAmount);
		case WWD_WEEKS: return AddDurationInWeeks(dtFrom, dAmount);
	}

	ASSERT(0);
	return dtFrom;
}

COleDateTime CWorkingWeek::AddDurationInHours(COleDateTime& dtFrom, double dHours) const
{
	// Sanity checks
	if ((dHours == 0.0) || !CDateHelper::IsDateSet(dtFrom))
	{
		ASSERT(0);
		return dtFrom;
	}

#ifdef _DEBUG
	COleDateTime dtOrgFrom(dtFrom);
	double dOrgHours(dHours);
#endif

	// Make sure from date is valid
	MakeWeekday(dtFrom, (dHours > 0.0), TRUE); // truncates time if it was a weekend

	COleDateTime dtTo(dtFrom);

	if (dHours > 0)
	{
		do 
		{
			// First day
			m_WorkDay.AddDurationInHours(dtTo, dHours);

			if (dHours == 0.0)
				break;

			// Move to start of next day
			dtTo = m_WorkDay.GetStartOfDay(dtTo.m_dt + 1.0);

			// Jump over weekends leaving time of day as-is
			MakeWeekday(dtTo, TRUE, FALSE);

			// Subsequent whole days
			ASSERT(dtTo == m_WorkDay.GetStartOfDay(dtTo));

			while (dHours > m_WorkDay.GetLengthInHours(false))
			{
				dtTo.m_dt++;

				if (!m_Weekend.IsWeekend(dtTo))
					dHours -= m_WorkDay.GetLengthInHours(false);
			}

			ASSERT(dtTo == m_WorkDay.GetStartOfDay(dtTo));

			// Whatever is left
			m_WorkDay.AddDurationInHours(dtTo, dHours);

			ASSERT(dHours == 0.0);
		}
		while (false);
	}
	else // if (dHours < 0)
	{
		do 
		{
			// First day
			m_WorkDay.AddDurationInHours(dtTo, dHours);

			if (dHours == 0.0)
				break;

			// Move to end of previous day
			dtTo = m_WorkDay.GetEndOfDay(dtTo.m_dt - 1.0);

			// Jump over weekends leaving time of day as-is
			MakeWeekday(dtTo, FALSE, FALSE);

			// Subsequent whole days
			ASSERT(dtTo == m_WorkDay.GetEndOfDay(dtTo));

			while (fabs(dHours) > m_WorkDay.GetLengthInHours(false))
			{
				dtTo.m_dt--;

				if (!m_Weekend.IsWeekend(dtTo))
					dHours += m_WorkDay.GetLengthInHours(false);
			}

			ASSERT(dtTo == m_WorkDay.GetEndOfDay(dtTo));

			// Whatever is left
			m_WorkDay.AddDurationInHours(dtTo, dHours);

			ASSERT(dHours == 0.0);
		}
		while (false);
	}

#ifdef _DEBUG
	double dCheckHours = CalculateDurationInHours(dtOrgFrom, dtTo);
	ASSERT(fabs(dCheckHours - (dOrgHours - dHours)) < 0.001);
#endif

	return dtTo;
}

COleDateTime CWorkingWeek::AddDurationInMinutes(COleDateTime& dtFrom, double dMins) const
{
	return AddDurationInHours(dtFrom, (dMins / 60.0));
}

COleDateTime CWorkingWeek::AddDurationInDays(COleDateTime& dtFrom, double dDays) const
{
	double dDayLen = m_WorkDay.GetLengthInHours();

	if (dDayLen == 0.0)
		return dtFrom;

	return AddDurationInHours(dtFrom, (dDays * dDayLen));
}

COleDateTime CWorkingWeek::AddDurationInWeeks(COleDateTime& dtFrom, double dWeeks) const
{
	int nNumWeekdays = (7 - m_Weekend.GetLengthInDays());

	if (nNumWeekdays == 0)
		return dtFrom;

	return AddDurationInDays(dtFrom, (dWeeks * nNumWeekdays));
}

BOOL CWorkingWeek::HasWeekend() const
{
	return (m_Weekend.GetDays() != 0);
}

int CWorkingWeek::GetLengthInDays() const
{
	return (7 - m_Weekend.GetLengthInDays());
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
	return (DHW_EVERYDAY & ~m_Weekend.GetDays());
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

	if (m_Weekend.IsWeekend(date))
	{
		if (bTruncateTime)
			date = CDateHelper::GetDateOnly(date);

		while (m_Weekend.IsWeekend(date))
		{
			if (bForwards)
				date.m_dt++;
			else
				date.m_dt--;
		}
	}

	return TRUE;
}

