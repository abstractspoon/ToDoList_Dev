// DateHelper.cpp: implementation of the CDateHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DateHelper.h"
#include "TimeHelper.h"
#include "misc.h"
#include "WorkingWeek.h"

#include "..\3rdParty\T64Utils.h"
#include "..\3rdParty\JalaliCalendar.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const double MINS_IN_DAY = (24 * 60);
const double SECS_IN_DAY = (24 * 60 * 60);
const double ONE_HOUR	= (1.0 / 24);
const double HALF_HOUR	= (ONE_HOUR / 2);
const double ONE_MINUTE = (1.0 / MINS_IN_DAY);
const float  ONE_SECOND	= (float)(1.0f / SECS_IN_DAY); // intentionally 'float' for less precision
const double END_OF_DAY = (1.0 - ONE_SECOND);
const double START_OF_DAY = ONE_SECOND;

//////////////////////////////////////////////////////////////////////

COleDateTimeRange::COleDateTimeRange()
{
	Reset();
}

COleDateTimeRange::COleDateTimeRange(const COleDateTimeRange& dtRange)
{
	*this = dtRange;
}

COleDateTimeRange::COleDateTimeRange(const COleDateTime& dtStart, const COleDateTime& dtEnd, BOOL bInclusive)
{
	Reset();
	VERIFY(Set(dtStart, dtEnd, bInclusive));
}

COleDateTimeRange::COleDateTimeRange(DH_DATE nStart, DH_DATE nEnd, BOOL bInclusive)
{
	Reset();
	VERIFY(Set(nStart, nEnd, bInclusive));
}

COleDateTimeRange::COleDateTimeRange(const COleDateTime& dtStart, DH_DATE nEnd, BOOL bInclusive)
{
	Reset();
	VERIFY(Set(dtStart, nEnd, bInclusive));
}

COleDateTimeRange::COleDateTimeRange(const COleDateTime& dtStart, int nEndOffset, DH_UNITS nOffsetUnits, BOOL bInclusive)
{
	Reset();
	VERIFY(Set(dtStart, nEndOffset, nOffsetUnits, bInclusive));
}

COleDateTimeRange::COleDateTimeRange(DH_DATE nStart, int nEndOffset, DH_UNITS nOffsetUnits, BOOL bInclusive)
{
	Reset();
	VERIFY(Set(nStart, nEndOffset, nOffsetUnits, bInclusive));
}

COleDateTimeRange& COleDateTimeRange::operator=(const COleDateTimeRange& other)
{
	m_dtStart = other.m_dtStart;
	m_dtEnd = other.m_dtEnd;
	m_bInclusive = other.m_bInclusive;

	return *this;
}

BOOL COleDateTimeRange::operator==(const COleDateTimeRange& other) const
{
	if (!IsValid() || !other.IsValid())
		return FALSE;

	return ((m_dtStart == other.m_dtStart) && 
			(m_dtEnd == other.m_dtEnd) && 
			(m_bInclusive == other.m_bInclusive));
}

BOOL COleDateTimeRange::operator!=(const COleDateTimeRange& other) const
{
	return !(*this == other);
}

void COleDateTimeRange::Reset()
{
	m_bInclusive = TRUE;

	CDateHelper::ClearDate(m_dtStart);
	CDateHelper::ClearDate(m_dtEnd);
}

BOOL COleDateTimeRange::Set(const COleDateTimeRange& dtRange)
{
	if (!dtRange.IsValid())
		return FALSE;

	*this = dtRange;
	return TRUE;
}

BOOL COleDateTimeRange::Set(const COleDateTime& dtStart, const COleDateTime& dtEnd, BOOL bInclusive)
{
	if (!CDateHelper::IsDateSet(dtStart) || !CDateHelper::IsDateSet(dtEnd))
	{
		ASSERT(0);
		return FALSE;
	}
	else if (GetEndInclusive(dtEnd, bInclusive) < dtStart)
	{
		ASSERT(0);
		return FALSE;
	}

	m_dtStart = dtStart;
	m_dtEnd = dtEnd;
	m_bInclusive = (bInclusive || CDateHelper::IsEndOfDay(dtEnd, FALSE));

	return IsValid();
}

BOOL COleDateTimeRange::Set(DH_DATE nStart, DH_DATE nEnd, BOOL bInclusive)
{
	return Set(CDateHelper::GetDate(nStart), CDateHelper::GetDate(nEnd), bInclusive);
}

BOOL COleDateTimeRange::Set(const COleDateTime& dtStart, DH_DATE nEnd, BOOL bInclusive)
{
	return Set(dtStart, CDateHelper::GetDate(nEnd), bInclusive);
}

BOOL COleDateTimeRange::Set(const COleDateTime& dtStart, int nEndOffset, DH_UNITS nOffsetUnits, BOOL bInclusive)
{
	COleDateTime dtEnd(dtStart);
	
	if (!CDateHelper().OffsetDate(dtEnd, nEndOffset, nOffsetUnits))
	{
		ASSERT(0);
		return FALSE;
	}
	
	return Set(dtStart, dtEnd, bInclusive);
}

BOOL COleDateTimeRange::Set(DH_DATE nStart, int nEndAmount, DH_UNITS nOffsetUnits, BOOL bInclusive)
{
	return Set(CDateHelper::GetDate(nStart), nEndAmount, nOffsetUnits, bInclusive);
}

BOOL COleDateTimeRange::IsValid() const
{
	return (CDateHelper::IsDateSet(m_dtStart) && 
			CDateHelper::IsDateSet(m_dtEnd) && 
			(GetEndInclusive() >= m_dtStart));
}

BOOL COleDateTimeRange::IsValid(const COleDateTime& dtStart, const COleDateTime& dtEnd, BOOL bInclusive)
{
	return (CDateHelper::IsDateSet(dtStart) &&
			CDateHelper::IsDateSet(dtEnd) &&
			(GetEndInclusive(dtEnd, bInclusive) >= dtStart));

}

BOOL COleDateTimeRange::IsNull() const
{
	return (!CDateHelper::IsDateSet(m_dtStart) && !CDateHelper::IsDateSet(m_dtEnd));
}

BOOL COleDateTimeRange::Contains(const COleDateTime& date, BOOL bInclusive) const
{
	if (!IsValid())
		return FALSE;
	
	if (date < m_dtStart)
		return FALSE;

	return (GetEndInclusive(date, bInclusive) <= GetEndInclusive());
}

BOOL COleDateTimeRange::Contains(const COleDateTimeRange& dtOther) const
{
	if (!IsValid() || !dtOther.IsValid())
		return FALSE;

	if (dtOther == *this)
		return TRUE;

	return (Contains(dtOther.GetStart()) && Contains(dtOther.GetEndInclusive()));
}

BOOL COleDateTimeRange::HasIntersection(const COleDateTimeRange& dtOther) const
{
	if (!IsValid() || !dtOther.IsValid())
		return FALSE;

	if (GetEndInclusive() < dtOther.GetStart())
		return FALSE;

	if (GetStart() > dtOther.GetEndInclusive())
		return FALSE;
		
	return TRUE;
}

BOOL COleDateTimeRange::GetIntersection(const COleDateTimeRange& dtOther1, const COleDateTimeRange& dtOther2)
{
	if (!dtOther1.HasIntersection(dtOther2))
		return FALSE;

	COleDateTime dtStart = max(dtOther1.m_dtStart, dtOther2.m_dtStart);
	COleDateTime dtEnd = min(dtOther1.GetEndInclusive(), dtOther2.GetEndInclusive());

	return Set(dtStart, dtEnd, (dtOther1.m_bInclusive || dtOther2.m_bInclusive));
}

BOOL COleDateTimeRange::IntersectWith(const COleDateTimeRange& dtOther)
{
	return GetIntersection(*this, dtOther);
}

BOOL COleDateTimeRange::Add(const COleDateTimeRange& dtOther1, const COleDateTimeRange& dtOther2)
{
	COleDateTimeRange dtTemp(dtOther1);

	if (!dtTemp.Add(dtOther2))
		return FALSE;

	*this = dtTemp;

	// Result must logically be valid
	ASSERT(IsValid());
	return TRUE;
}

BOOL COleDateTimeRange::Add(const COleDateTimeRange& dtOther, const COleDateTime& date, BOOL bInclusive)
{
	COleDateTimeRange dtTemp(dtOther);

	if (!dtTemp.Add(date, bInclusive))
		return FALSE;

	*this = dtTemp;

	// Result must logically be valid
	ASSERT(IsValid());
	return TRUE;
}

BOOL COleDateTimeRange::Add(const COleDateTimeRange& dtOther)
{
	if (!dtOther.IsValid())
		return FALSE;

	if (!Add(dtOther.m_dtStart, FALSE))
		return FALSE;

	if (!Add(dtOther.m_dtEnd, dtOther.m_bInclusive))
		return FALSE;
	
	// Result must logically be valid
	ASSERT(IsValid());
	return TRUE;
}

BOOL COleDateTimeRange::Add(const COleDateTime& date, BOOL bInclusive)
{
	if (!CDateHelper::IsDateSet(date))
		return FALSE;

	CDateHelper::Min(m_dtStart, date);

	m_dtEnd = GetEndInclusive();
	CDateHelper::Max(m_dtEnd, GetEndInclusive(date, bInclusive));

	m_bInclusive |= bInclusive;
			
	if (m_bInclusive && CDateHelper::IsEndOfDay(m_dtEnd, FALSE))
		m_dtEnd = CDateHelper::GetDateOnly(m_dtEnd);

	// Result must logically be valid
	ASSERT(IsValid());
	return TRUE;
}

COleDateTime COleDateTimeRange::GetStart() const
{
	return m_dtStart;
}

COleDateTime COleDateTimeRange::GetEnd() const
{
	return m_dtEnd;
}

COleDateTime COleDateTimeRange::GetEndInclusive() const
{
	return GetEndInclusive(m_dtEnd, m_bInclusive);
}

COleDateTime COleDateTimeRange::GetEndInclusive(const COleDateTime& date, BOOL bInclusive)
{
	if (bInclusive && CDateHelper::IsDateSet(date) && !CDateHelper::DateHasTime(date))
		return CDateHelper::GetEndOfDay(date);

	// else
	return date;
}

BOOL COleDateTimeRange::HasStart() const
{
	return CDateHelper::IsDateSet(m_dtStart);
}

BOOL COleDateTimeRange::HasEnd() const
{
	return CDateHelper::IsDateSet(m_dtEnd);
}

int COleDateTimeRange::GetDayCount() const
{
	if (!IsValid())
		return 0;

	CTwentyFourSevenWeek week;
	CDateHelper dh(week);

	return dh.CalcDaysFromTo(m_dtStart, m_dtEnd, m_bInclusive);
}

int COleDateTimeRange::GetWeekdayCount() const
{
	if (!IsValid())
		return 0;

	return CDateHelper().CalcDaysFromTo(m_dtStart, m_dtEnd, m_bInclusive);
}

BOOL COleDateTimeRange::Offset(int nAmount, DH_UNITS nUnits, BOOL bPreserveEndOfMonth)
{
	if (!IsValid())
		return FALSE;

	COleDateTimeRange prevRange = *this;
	CDateHelper dh;

	if (!dh.OffsetDate(m_dtStart, nAmount, nUnits) ||
		!dh.OffsetDate(m_dtEnd, nAmount, nUnits, bPreserveEndOfMonth))
	{
		*this = prevRange;
		return FALSE;
	}

	return TRUE;
}

BOOL COleDateTimeRange::Expand(int nAmount, DH_UNITS nUnits)
{
	// Handles expansion and contraction
	return (OffsetStart(-nAmount, nUnits) && OffsetEnd(nAmount, nUnits));
}

BOOL COleDateTimeRange::OffsetStart(int nAmount, DH_UNITS nUnits)
{
	if (!IsValid())
		return FALSE;

	COleDateTimeRange prevRange = *this;

	if (!CDateHelper().OffsetDate(m_dtStart, nAmount, nUnits) || !IsValid())
	{
		*this = prevRange;
		return FALSE;
	}

	return TRUE;
}

BOOL COleDateTimeRange::OffsetEnd(int nAmount, DH_UNITS nUnits, BOOL bPreserveEndOfMonth)
{
	if (!IsValid())
		return FALSE;

	COleDateTimeRange prevRange = *this;

	if (!CDateHelper().OffsetDate(m_dtEnd, nAmount, nUnits, bPreserveEndOfMonth) || !IsValid())
	{
		*this = prevRange;
		return FALSE;
	}

	return TRUE;
}

CString COleDateTimeRange::Format(DWORD dwFlags, LPCTSTR szDelim) const
{
	CString sRange;

	if (IsValid())
	{
		CString sStart = CDateHelper::FormatDate(m_dtStart, dwFlags);
		CString sEnd = CDateHelper::FormatDate(m_dtEnd, dwFlags);

		if (Misc::IsEmpty(szDelim))
			szDelim = _T(" ");
	
		sRange = (sStart + szDelim + sEnd);
	}

	return sRange;
}

CString COleDateTimeRange::FormatDateOnly(LPCTSTR szFormat, LPCTSTR szDelim) const
{
	CString sRange;

	if (IsValid())
	{
		CString sStart = CDateHelper::FormatDateOnly(m_dtStart, szFormat);
		CString sEnd = CDateHelper::FormatDateOnly(m_dtEnd, szFormat);

		if (Misc::IsEmpty(szDelim))
			szDelim = _T(" ");

		sRange = (sStart + szDelim + sEnd);
	}

	return sRange;
}

double COleDateTimeRange::CalcProportion(const COleDateTime& date) const
{
	if (!IsValid())
		return -1;

	COleDateTime dtEnd = GetEndInclusive();

	if ((dtEnd == m_dtStart) || (date <= m_dtStart))
		return 0.0;

	if (date >= dtEnd)
		return 1.0;

	return ((date.m_dt - m_dtStart.m_dt) / (dtEnd.m_dt - m_dtStart.m_dt));
}

BOOL COleDateTimeRange::IsSameDay() const
{
	return CDateHelper::IsSameDay(m_dtStart, GetEndInclusive());
}

BOOL COleDateTimeRange::IsSameMonth() const
{
	return CDateHelper::IsSameMonth(m_dtStart, GetEndInclusive());
}

//////////////////////////////////////////////////////////////////////

CDateHelper::CDateHelper() : m_week()
{

}

CDateHelper::CDateHelper(const CWorkingWeek& week) : m_week(week)
{

}

int CDateHelper::CalcDaysFromTo(const COleDateTime& dateFrom, const COleDateTime& dateTo, BOOL bInclusive) const
{
	if (!IsDateSet(dateFrom) || !IsDateSet(dateTo))
	{
		ASSERT(0);
		return 0;
	}
	
	COleDateTime dFrom = GetDateOnly(dateFrom);
	COleDateTime dTo = GetDateOnly(dateTo);

	if (bInclusive)
		dTo.m_dt += 1;

	int nDays = (int)(double)(dTo - dFrom);

	if (m_week.HasWeekend() && (nDays > 0))
	{
		nDays = 0;

		while (dFrom < dTo)
		{
			OLE_DAYOFWEEK nDOW = GetDayOfWeek(dFrom);

			if (!m_week.Weekend().IsWeekend(nDOW))
				nDays++;

			dFrom += 1;
		}
	}

	return nDays;
}

int CDateHelper::CalcDaysFromTo(const COleDateTime& dateFrom, DH_DATE nTo, BOOL bInclusive) const
{
	return CalcDaysFromTo(dateFrom, GetDate(nTo), bInclusive);
}

int CDateHelper::CalcDaysFromTo(DH_DATE nFrom, DH_DATE nTo, BOOL bInclusive) const
{
	return CalcDaysFromTo(GetDate(nFrom), GetDate(nTo), bInclusive);
}

BOOL CDateHelper::OffsetDate(COleDateTime& date, int nAmount, DH_UNITS nUnits, BOOL bPreserveEndOfMonth) const
{
	// sanity checks
	if (!IsDateSet(date) || (!IsValidUnit(nUnits) && (nUnits != DHU_NULL)))
	{
		ASSERT(0);
		return FALSE;
	}

	if (!nAmount)
		return FALSE;

	switch (nUnits)
	{
	case DHU_WEEKDAYS:
		if (m_week.HasWeekend())
		{
			BOOL bForwards = (nAmount > 0);
			nAmount = abs(nAmount);

			while (nAmount--)
			{
				date.m_dt += (bForwards ? 1.0 : -1.0);

				// skip weekends but don't truncate time
				m_week.MakeWeekday(date, bForwards, FALSE);
			}

			break;
		}
		// else fall thru

	case DHU_DAYS:
		{
			// Handle negative dates whilst preserving time of day
			double dDateOnly = GetDateOnly(date).m_dt;
			date = MakeDate((dDateOnly + nAmount), date);
		}
		break;

	case DHU_WEEKS:
		{
			// Handle negative dates whilst preserving time of day
			double dDateOnly = GetDateOnly(date).m_dt;
			date = MakeDate((dDateOnly + (nAmount * 7)), date);
		}
		break;

	case DHU_MONTHS:
		IncrementMonth(date, nAmount, bPreserveEndOfMonth);
		break;

	case DHU_YEARS:
		IncrementYear(date, nAmount, bPreserveEndOfMonth);
		break;
	}

	return IsDateSet(date);
}

double CDateHelper::CalcDuration(const COleDateTime& dtFrom, const COleDateTime& dtTo, DH_UNITS nUnits, BOOL bNoTimeIsEndOfDay) const
{
	// Sanity check
	if (!COleDateTimeRange::IsValid(dtFrom, dtTo))
	{
		ASSERT(0);
		return 0.0;
	}

	// Handle due date 'end of day'
	COleDateTime dateEnd(dtTo);

	if (CDateHelper::IsEndOfDay(dateEnd, bNoTimeIsEndOfDay))
		dateEnd = CDateHelper::GetStartOfNextDay(dateEnd);

	switch (nUnits)
	{
	case DHU_DAYS:
		return (dateEnd.m_dt - dtFrom.m_dt);

	case DHU_MONTHS:
	case DHU_YEARS:
		{
			// Check for exact months
			double dDuration = (dateEnd.m_dt - dtFrom.m_dt); // in days

			if (dDuration == (int)dDuration) // exact days
			{
				int nStartDay = dtFrom.GetDay();
				int nEndDay = dateEnd.GetDay();

				if (nEndDay == nStartDay)
				{
					int nNumMonths = (CDateHelper::GetDateInMonths(dateEnd) - CDateHelper::GetDateInMonths(dtFrom));
					return ((nUnits == DHU_MONTHS) ? nNumMonths : (nNumMonths / 12.0));
				}
			}

			// else
			CTwentyFourSevenWeek week;
			CTimeHelper th(week);

			return th.Convert(dDuration, THU_DAYS, ((nUnits == DHU_MONTHS) ? THU_MONTHS : THU_YEARS));
		}
		break;

	case DHU_WEEKDAYS:
		return m_week.CalcDuration(dtFrom, dateEnd, WWD_DAYS);

	case DHU_WEEKS:
		return m_week.CalcDuration(dtFrom, dateEnd, WWD_WEEKS);
	}

	ASSERT(0);
	return 0.0;
}

// Static helpers ------------------------------------------------------------

BOOL CDateHelper::IsValidDayInMonth(int nDay, int nMonth, int nYear)
{
	return (nMonth >= 1 && nMonth <= 12) &&
		(nDay >= 1 && nDay <= GetDaysInMonth(nMonth, nYear));
}

BOOL CDateHelper::IsValidDayOfMonth(OLE_DAYOFWEEK nDOW, int nWhich, int nMonth)
{
	return (nWhich >= 1 && nWhich <= 5) &&
		(nDOW >= 1 && nDOW <= 7) &&
		(nMonth >= 1 && nMonth <= 12);
}

BOOL CDateHelper::DecodeDate(const CString& sDate, double& date, BOOL bAndTime)
{
	COleDateTime dt;

	if (DecodeDate(sDate, dt, bAndTime))
	{
		date = dt.m_dt;
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CDateHelper::IsValidUnit(TCHAR nUnits)
{
	switch (nUnits)
	{
	case DHU_WEEKDAYS:
	case DHU_DAYS:
	case DHU_WEEKS:
	case DHU_MONTHS:
	case DHU_YEARS:
		return TRUE;
	}

	return FALSE;
}

// external
BOOL CDateHelper::DecodeOffset(LPCTSTR szDate, int& nAmount, DH_UNITS& nUnits, BOOL bMustHaveSign)
{
	return DecodeOffsetEx(szDate, nAmount, nUnits, DHU_DAYS, bMustHaveSign);
}

// internal
BOOL CDateHelper::DecodeOffsetEx(LPCTSTR szDate, int& nAmount, 
								DH_UNITS& nUnits, DH_UNITS nDefUnits, BOOL bMustHaveSign)
{
	// sanity checks
	ASSERT(IsValidUnit(nDefUnits) || (nDefUnits == DHU_NULL));

	CString sDate(szDate);
	Misc::Trim(sDate);

	if (sDate.IsEmpty())
		return FALSE;

	// Sign defaults to +ve
	int nSign = 1;
	TCHAR nFirst = Misc::First(sDate);

	switch (nFirst)
	{
	case '+': 
		nSign = 1;  
		Misc::TrimFirst(sDate);
		break;

	case '-': 
		nSign = -1; 
		Misc::TrimFirst(sDate);
		break;

	default: 
		// Must have sign or be number
		if (bMustHaveSign || ((nFirst < '0') || (nFirst > '9')))				
			return FALSE;
	}

	// units
	TCHAR nLast = Misc::Last(sDate);

	if (IsValidUnit(nLast))
		nUnits = (DH_UNITS)nLast;
	else
		nUnits = nDefUnits;

	// Rest is number (note: ttoi ignores any trailing letters)
	nAmount = (nSign * _ttoi(sDate));
	return TRUE;
}

BOOL CDateHelper::DecodeRelativeDate(LPCTSTR szDate, COleDateTime& date, BOOL bMustHaveSign) const
{
	// sanity check
	CString sDate(szDate);
	Misc::Trim(sDate);

	if (sDate.IsEmpty())
		return FALSE;

	// clear date so we know if it changed
	ClearDate(date);

	// leading char can define starting date 
	TCHAR nFirst(Misc::First(sDate));
	BOOL bTruncateTime = TRUE;

	switch (nFirst)
	{
	case 'N': 			
	case 'n': 			
		date = GetDate(DHD_NOW);
		bTruncateTime = FALSE;
		break;

	case 'T': 			
	case 't': 			
	case DHU_WEEKDAYS:	
	case DHU_DAYS:		date = GetDate(DHD_TODAY);			break;

	case DHU_WEEKS:		date = GetDate(DHD_ENDTHISWEEK);	break;
	case DHU_MONTHS:	date = GetDate(DHD_ENDTHISMONTH);	break;
	case DHU_YEARS:		date = GetDate(DHD_ENDTHISYEAR);	break;
	}

	if (IsDateSet(date))
		Misc::TrimFirst(sDate);
	else
		date = GetDate(DHD_TODAY); // default

	// The rest should be a relative date offset
	int nAmount = 0;

	if (!sDate.IsEmpty())
	{
		DH_UNITS nUnits = DHU_NULL;

		if (!DecodeOffsetEx(sDate, nAmount, nUnits, DHU_NULL, bMustHaveSign))
			return FALSE;

		// Handle missing units
		if (!nUnits)
		{
			if (IsValidUnit(nFirst))
				nUnits = (DH_UNITS)nFirst;
			else
				nUnits = DHU_DAYS; // default
		}

		if (nAmount && !OffsetDate(date, nAmount, nUnits))
			return FALSE;
	}

	return TRUE;
}

BOOL CDateHelper::IsValidRelativeDate(LPCTSTR szDate, BOOL bMustHaveSign)
{
	COleDateTime dtUnused;
	CTwentyFourSevenWeek week;

	return CDateHelper(week).DecodeRelativeDate(szDate, dtUnused, bMustHaveSign);
}

BOOL CDateHelper::DecodeDate(const CString& sDate, COleDateTime& date, BOOL bAndTime)
{
	if (sDate.IsEmpty())
		return FALSE;

	if (bAndTime && date.ParseDateTime(sDate))
	{
		// success
	}
	else if (!bAndTime && date.ParseDateTime(sDate, VAR_DATEVALUEONLY))
	{
		// success
	}
	else if (DecodeISODate(sDate, date, bAndTime))
	{
		// success
	}
	else if (DecodeLocalShortDate(sDate, date))
	{
		// success
	}
	else
	{
		return FALSE;
	}

	// Treat a negative date as Persian/Jalali
	if ((date.m_dt < 0.0) && CJalaliCalendar::IsActive())
		date = CJalaliCalendar::ToGregorian(date.GetYear(), date.GetMonth(), date.GetDay());

	return TRUE;
}

BOOL CDateHelper::GetTimeT(const COleDateTime& date, time_t& timeT)
{
	time64_t t64;
	
	if (!GetTimeT64(date, t64))
		return FALSE;

#ifdef _DEBUG
	COleDateTime dtCheck = GetDate(t64);
	ASSERT(fabs(dtCheck.m_dt - date.m_dt) < 0.00001);
#endif

	return GetTimeT(t64, timeT);
}

BOOL CDateHelper::GetTimeT(time64_t date, time_t& timeT)
{
	if ((date < 0) || (date > LONG_MAX))
		return FALSE;

	SYSTEMTIME st = { 0 };
	T64Utils::T64ToSystemTime(&date, &st);

	timeT = CTime(st).GetTime();
	return TRUE;
}

BOOL CDateHelper::GetTimeT64(const COleDateTime& date, time64_t& timeT)
{
	if (!IsDateSet(date))
		return FALSE;

	SYSTEMTIME st = { 0 };
	
	if (!date.GetAsSystemTime(st))
		return FALSE;

	T64Utils::SystemTimeToT64(&st, &timeT);
	ASSERT(timeT != 0);

	return TRUE;
}

COleDateTime CDateHelper::GetDate(time64_t date)
{
	if (date == T64Utils::T64_NULL)
		return NullDate();

	// else
	SYSTEMTIME st = { 0 };
	T64Utils::T64ToSystemTime(&date, &st);

	return COleDateTime(st);
}

COleDateTime CDateHelper::GetDate(double dDate, COleDateTime::DateTimeStatus nStatus)
{
	COleDateTime date;

	date.m_dt = dDate;
	date.m_status = nStatus;

	return date;
}

#if _MSC_VER < 1400
BOOL CDateHelper::DecodeDate(const CString& sDate, time_t& date, BOOL bAndTime)
{
	time64_t t64 = 0;

	if (!DecodeDate(sDate, t64, bAndTime))
		return FALSE;

	return GetTimeT(t64, date);
}
#endif

BOOL CDateHelper::DecodeDate(const CString& sDate, time64_t& date, BOOL bAndTime)
{
	COleDateTime dt;

	if (!DecodeDate(sDate, dt, bAndTime))
		return FALSE;

	return GetTimeT64(dt, date);
}

BOOL CDateHelper::DecodeISODate(const CString& sDate, COleDateTime& date, BOOL bAndTime)
{
	int nYear = -1, nMonth = -1, nDay = -1, nHour = 0, nMin = 0, nSec = 0;
	const LPCTSTR DATETIME_FMT = _T("%4d-%2d-%2dT%2d:%2d:%2d");

#if _MSC_VER >= 1400
	if (_stscanf_s(sDate, DATETIME_FMT, &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec) >= 3)
#else
	if (_stscanf(sDate, DATETIME_FMT, &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec) >= 3)
#endif
	{
		if (bAndTime)
			return (date.SetDateTime(nYear, nMonth, nDay, nHour, nMin, nSec) == 0);
		else
			return (date.SetDate(nYear, nMonth, nDay) == 0);
	}
			
	return FALSE;
}

BOOL CDateHelper::DecodeLocalShortDate(const CString& sDate, COleDateTime& date)
{
	if (!sDate.IsEmpty())
	{
		// split the string and format by the delimiter
		CString sFormat(Misc::GetShortDateFormat()), sDelim(Misc::GetDateSeparator());
		CStringArray aDateParts, aFmtParts;

		Misc::Split(sDate, aDateParts, sDelim, TRUE);
		Misc::Split(sFormat, aFmtParts, sDelim, TRUE);

		if (aDateParts.GetSize() != aFmtParts.GetSize())
			return FALSE;

		// process the parts, deciphering the format
		int nYear = -1, nMonth = -1, nDay = -1;

		for (int nPart = 0; nPart < aFmtParts.GetSize(); nPart++)
		{
			if (aFmtParts[nPart].FindOneOf(_T("Yy")) != -1)
				nYear = _ttoi(aDateParts[nPart]);

			if (aFmtParts[nPart].FindOneOf(_T("Mm")) != -1)
				nMonth = _ttoi(aDateParts[nPart]);

			if (aFmtParts[nPart].FindOneOf(_T("Dd")) != -1)
				nDay = _ttoi(aDateParts[nPart]);
		}

		if (nYear != -1 && nMonth != -1 && nDay != -1)
			return (date.SetDate(nYear, nMonth, nDay) == 0);
	}

	// else
	ClearDate(date);
	return FALSE;
}

double CDateHelper::GetDate(DH_DATE nDate)
{
	COleDateTime date = COleDateTime::GetCurrentTime();

	switch (nDate)
	{
	case DHD_NOW:
		return date; // as-is ie. no time truncation

	// All the rest have 'time of day' removed
	case DHD_TODAY:
		date = GetDateOnly(date);
		break;

	case DHD_YESTERDAY:
		date = (GetDate(DHD_TODAY) - 1.0); // RECURSIVE CALL
		break;
		
	case DHD_TOMORROW:
		date = (GetDate(DHD_TODAY) + 1.0); // RECURSIVE CALL
		break;

	case DHD_BEGINTHISWEEK:
		date = GetStartOfWeek(date);
		break;

	case DHD_ENDTHISWEEK:
		date = (GetDate(DHD_BEGINTHISWEEK) + 7); // RECURSIVE CALL
		break;

	case DHD_ENDNEXTWEEK:
		date = (GetDate(DHD_ENDTHISWEEK) + 7.0); // RECURSIVE CALL
		break;

	case DHD_BEGINTHISMONTH:
		date = GetStartOfMonth(date);
		break;

	case DHD_ENDTHISMONTH:
		date = GetEndOfMonth(date);
		break;

	case DHD_ENDNEXTMONTH:
		date = GetEndOfMonth(GetDate(DHD_ENDTHISMONTH) + 1.0); // RECURSIVE CALL
		break;

	case DHD_BEGINTHISYEAR:
		date = GetStartOfYear(date);
		break;

	case DHD_ENDTHISYEAR:
		date = GetEndOfYear(date);
		break;

	case DHD_ENDNEXTYEAR:
		date = GetEndOfYear(GetDate(DHD_ENDNEXTYEAR) + 1.0); // RECURSIVE CALL
		break;

	default:
		ASSERT (0);
		return 0;
	}

	ASSERT(!DateHasTime(date));
	return date;
}

COleDateTime CDateHelper::GetDate(const COleDateTime& date, BOOL bNoTimeIsEndOfDay)
{
	if (!bNoTimeIsEndOfDay || DateHasTime(date))
		return date;

	return GetEndOfDay(date);
}

BOOL CDateHelper::Min(COleDateTime& date, const COleDateTime& dtOther)
{
	return Min(date, dtOther, FALSE);
}

BOOL CDateHelper::Min(COleDateTime& date, const COleDateTime& dtOther, BOOL bNoTimeIsEndOfDay)
{
	if (IsDateSet(date))
	{
		if (IsDateSet(dtOther))
		{
			if (GetDate(dtOther, bNoTimeIsEndOfDay) < GetDate(date, bNoTimeIsEndOfDay))
				date = dtOther;
		}
		// else no change
	}
	else if (IsDateSet(dtOther))
	{
		date = dtOther;
	}

	return IsDateSet(date);
}

BOOL CDateHelper::Max(COleDateTime& date, const COleDateTime& dtOther)
{
	return Max(date, dtOther, FALSE);
}

BOOL CDateHelper::Max(COleDateTime& date, const COleDateTime& dtOther, BOOL bNoTimeIsEndOfDay)
{
	if (IsDateSet(date))
	{
		if (IsDateSet(dtOther))
		{
			if (GetDate(dtOther, bNoTimeIsEndOfDay) > GetDate(date, bNoTimeIsEndOfDay))
				date = dtOther;
		}
		// else no change
	}
	else if (IsDateSet(dtOther))
	{
		date = dtOther;
	}

	return IsDateSet(date);
}

int CDateHelper::Compare(const COleDateTime& date1, const COleDateTime& date2, DWORD dwCompareFlags)
{
	// Sort Non-dates below others
	BOOL bHas1 = IsDateSet(date1);
	BOOL bHas2 = IsDateSet(date2);

	if (bHas1 != bHas2)
	{
		return (bHas1 ? -1 : 1);
	}
	else if (!bHas1 && !bHas2)
	{
		return 0;
	}

	double dDateTime1 = GetDate(date1, (dwCompareFlags & DHC_NOTIMEISENDOFDAY)).m_dt;
	double dDateTime2 = GetDate(date2, (dwCompareFlags & DHC_NOTIMEISENDOFDAY)).m_dt;

	if ((dwCompareFlags & DHC_COMPARETIME) == 0)
	{
		// Compare dates only
		dDateTime1 = GetDateOnly(dDateTime1).m_dt;
		dDateTime2 = GetDateOnly(dDateTime2).m_dt;
	}
	else if ((dwCompareFlags & DHC_COMPARESECONDS) == 0)
	{
		dDateTime1 = TruncateSeconds(dDateTime1).m_dt;
		dDateTime2 = TruncateSeconds(dDateTime2).m_dt;
	}

	// Negative dates need care because time component is still positive
	// eg. -44000.125 < -44000.25
	if ((dDateTime1 < 0.0) && IsSameDay(date1, date2))
	{
		dDateTime1 = GetTimeOnly(dDateTime1).m_dt;
		dDateTime2 = GetTimeOnly(dDateTime2).m_dt;
	}

	double dDiff = (dDateTime1 - dDateTime2);

	if (fabs(dDiff) < ONE_SECOND)
		return 0;

	return (dDiff < 0) ? -1 : 1;
}

BOOL CDateHelper::IsDateSet(const COleDateTime& date)
{
	return (date.m_status == COleDateTime::valid && date.m_dt != 0.0) ? TRUE : FALSE;
}

void CDateHelper::ClearDate(COleDateTime& date)
{
	date.m_dt = 0.0;
	date.m_status = COleDateTime::null;
}

COleDateTime CDateHelper::NullDate()
{
	COleDateTime null;
	ClearDate(null);

	return null;
}

int CDateHelper::GetISODayOfWeek(const COleDateTime& date) 
{
	// ISO DOWs: 1=Mon, 2=Tue, ..., 7=Sun
	switch (GetDayOfWeek(date))
	{
	case DHO_MONDAY:	return 1;
	case DHO_TUESDAY:	return 2;
	case DHO_WEDNESDAY:	return 3;
	case DHO_THURSDAY:	return 4;
	case DHO_FRIDAY:	return 5;
	case DHO_SATURDAY:	return 6;
	case DHO_SUNDAY:	return 7;
	}

	ASSERT(0);
	return 0;
}

OLE_DAYOFWEEK CDateHelper::GetDayOfWeek(const COleDateTime& date)
{
	return (OLE_DAYOFWEEK)date.GetDayOfWeek();
}

OLE_DAYOFWEEK CDateHelper::GetDayOfWeek(const SYSTEMTIME& date)
{
	return (OLE_DAYOFWEEK)(date.wDayOfWeek + 1);
}

OLE_DAYOFWEEK CDateHelper::GetFirstDayOfWeek()
{
	TCHAR szFDW[3] = { 0 }; // 2 + NULL
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IFIRSTDAYOFWEEK, szFDW, 2);

	int nFirstDOW = _ttoi(szFDW);

	// we need days to have same order as COleDateTime::GetDayOfWeek()
	// which is 1 (sun) - 7 (sat)
	switch (nFirstDOW)
	{
	case 0: /* mon */ return DHO_MONDAY;
	case 1: /* tue */ return DHO_TUESDAY;
	case 2: /* wed */ return DHO_WEDNESDAY;
	case 3: /* thu */ return DHO_THURSDAY;
	case 4: /* fri */ return DHO_FRIDAY;
	case 5: /* sat */ return DHO_SATURDAY;
	case 6: /* sun */ return DHO_SUNDAY;
	}

	ASSERT (0);
	return DHO_UNDEF;
}

OLE_DAYOFWEEK CDateHelper::GetLastDayOfWeek()
{
	switch (GetFirstDayOfWeek())
	{
	case DHO_MONDAY:	return DHO_SUNDAY;
	case DHO_TUESDAY:	return DHO_MONDAY;
	case DHO_WEDNESDAY:	return DHO_TUESDAY;
	case DHO_THURSDAY:	return DHO_WEDNESDAY;
	case DHO_FRIDAY:	return DHO_THURSDAY;
	case DHO_SATURDAY:	return DHO_FRIDAY;
	case DHO_SUNDAY:	return DHO_SATURDAY;
	}

	ASSERT (0);
	return DHO_UNDEF;
}

OLE_DAYOFWEEK CDateHelper::GetNextDayOfWeek(OLE_DAYOFWEEK nDOW)
{
	switch (nDOW)
	{
	case DHO_MONDAY:	return DHO_TUESDAY;
	case DHO_TUESDAY:	return DHO_WEDNESDAY;
	case DHO_WEDNESDAY:	return DHO_THURSDAY;
	case DHO_THURSDAY:	return DHO_FRIDAY;
	case DHO_FRIDAY:	return DHO_SATURDAY;
	case DHO_SATURDAY:	return DHO_SUNDAY;
	case DHO_SUNDAY:	return DHO_MONDAY;
	}

	ASSERT (0);
	return DHO_UNDEF;
}

COleDateTime CDateHelper::GetNextAvailableDay(const COleDateTime& date, DWORD dwAvailDays)
{
	COleDateTime dtNext(date);
	dtNext.m_dt += 1.0; // at least

	VERIFY(ValidateDay(dtNext, dwAvailDays));
	return dtNext;
}

BOOL CDateHelper::ValidateDay(COleDateTime& date, DWORD dwAvailDays)
{
	ASSERT(dwAvailDays);

	if (!dwAvailDays)
		return FALSE;
	
	// build a 2 week weekday array so we don't have to deal with 
	// wrapping around
	const UINT nWeekdays[14] = 
	{
		(dwAvailDays & DHW_SUNDAY),
		(dwAvailDays & DHW_MONDAY),
		(dwAvailDays & DHW_TUESDAY),
		(dwAvailDays & DHW_WEDNESDAY),
		(dwAvailDays & DHW_THURSDAY),
		(dwAvailDays & DHW_FRIDAY),
		(dwAvailDays & DHW_SATURDAY),
		(dwAvailDays & DHW_SUNDAY),
		(dwAvailDays & DHW_MONDAY),
		(dwAvailDays & DHW_TUESDAY),
		(dwAvailDays & DHW_WEDNESDAY),
		(dwAvailDays & DHW_THURSDAY),
		(dwAvailDays & DHW_FRIDAY),
		(dwAvailDays & DHW_SATURDAY),
	};
	
	int nStart = (date.GetDayOfWeek() - 1); // 0-based
	int nEnd = (nStart + 7); // week later
	
	for (int nWeekday = nStart; nWeekday < nEnd; nWeekday++)
	{
		if (nWeekdays[nWeekday] == 0)
			date += 1.0; 
		else
			break;
	}

	return TRUE;
}

int CDateHelper::GetDayCount(DWORD dwDays)
{
	int nCount = 0;

	for (int nDay = 0; nDay < 7; nDay++)
	{
		if (dwDays & DH_DAYSOFWEEK[nDay])
			nCount++;
	}

	return nCount;
}

DH_DAYOFWEEK CDateHelper::MapOleDowToDH(OLE_DAYOFWEEK nDOW)
{
	switch (nDOW)
	{
	case DHO_SUNDAY:	return DHW_SUNDAY;
	case DHO_MONDAY:	return DHW_MONDAY;
	case DHO_TUESDAY:	return DHW_TUESDAY;
	case DHO_WEDNESDAY:	return DHW_WEDNESDAY;
	case DHO_THURSDAY:	return DHW_THURSDAY;
	case DHO_FRIDAY:	return DHW_FRIDAY;
	case DHO_SATURDAY:	return DHW_SATURDAY;
	}

	ASSERT(0);
	return DHW_NONE;
}

OLE_DAYOFWEEK CDateHelper::MapDHDowToOLE(DH_DAYOFWEEK nDOW)
{
	switch (nDOW)
	{
	case DHW_SUNDAY:	return DHO_SUNDAY;
	case DHW_MONDAY:	return DHO_MONDAY;
	case DHW_TUESDAY:	return DHO_TUESDAY;
	case DHW_WEDNESDAY:	return DHO_WEDNESDAY;
	case DHW_THURSDAY:	return DHO_THURSDAY;
	case DHW_FRIDAY:	return DHO_FRIDAY;
	case DHW_SATURDAY:	return DHO_SATURDAY;
	}

	ASSERT(0);
	return DHO_UNDEF;
}

DH_MONTH CDateHelper::MapMonthIndexToDHMonth(int nMonth)
{
	switch (nMonth)
	{
	case 1:		return DHM_JANUARY;
	case 2:		return DHM_FEBRUARY;
	case 3:		return DHM_MARCH;
	case 4:		return DHM_APRIL;
	case 5:		return DHM_MAY;
	case 6:		return DHM_JUNE;
	case 7:		return DHM_JULY;
	case 8:		return DHM_AUGUST;
	case 9:		return DHM_SEPTEMBER;
	case 10:	return DHM_OCTOBER;
	case 11:	return DHM_NOVEMBER;
	case 12:	return DHM_DECEMBER;
	}

	return DHM_NONE;
}

COleDateTime CDateHelper::GetStartOfWeek(const COleDateTime& date)
{
	return (GetEndOfWeek(date).m_dt - 6);
}

COleDateTime CDateHelper::GetEndOfWeek(const COleDateTime& date)
{
	// increment the date until we hit the last day of the week
	// note: we could have kept checking date.GetDayOfWeek but
	// it's a lot of calculation that's just not necessary
	COleDateTime dtEnd(GetDateOnly(date));

	OLE_DAYOFWEEK nLastDOW = GetLastDayOfWeek();
	OLE_DAYOFWEEK nDOW = GetDayOfWeek(date);
	
	while (nDOW != nLastDOW) 
	{
		dtEnd += 1;
		nDOW = GetNextDayOfWeek(nDOW);
	}

	return dtEnd;
}

BOOL CDateHelper::IsDayOfMonth(const COleDateTime& date, int nDay)
{
	if (CJalaliCalendar::IsActive())
	{
		int JYear, JMonth, JDay;
		CJalaliCalendar::FromGregorian(date, &JYear, &JMonth, &JDay);

		return (JDay == nDay);
	}

	return (date.GetDay() == nDay);
}

COleDateTime CDateHelper::GetStartOfMonth(const COleDateTime& date)
{
	if (CJalaliCalendar::IsActive())
	{
		int JYear, JMonth, JDay;
		CJalaliCalendar::FromGregorian(date, &JYear, &JMonth, &JDay);

		return CJalaliCalendar::ToGregorian(JYear, JMonth, 1);
	}

	// else
	return COleDateTime(date.GetYear(), date.GetMonth(), 1, 0, 0, 0);
}

COleDateTime CDateHelper::GetEndOfMonth(const COleDateTime& date)
{
	if (CJalaliCalendar::IsActive())
	{
		int JYear, JMonth, JDay;
		CJalaliCalendar::FromGregorian(date, &JYear, &JMonth, &JDay);

		int nDaysInMonth = CJalaliCalendar::GetDaysInMonth(JYear, JMonth);
		return CJalaliCalendar::ToGregorian(JYear, JMonth, nDaysInMonth);
	}

	return (GetStartOfMonth(date).m_dt + GetDaysInMonth(date) - 1);
}

COleDateTime CDateHelper::GetStartOfQuarter(const COleDateTime& date)
{
	int nStartInMonths = ((GetDateInMonths(date) / 3) * 3);
	
	return GetDateFromMonths(nStartInMonths);
}

COleDateTime CDateHelper::GetEndOfQuarter(const COleDateTime& date)
{
	int nStartInMonths = ((GetDateInMonths(date) / 3) * 3);
	int nStartNextInMonths = (nStartInMonths + 3);

	return (GetDateFromMonths(nStartNextInMonths).m_dt - 1.0); // day before next quarter
}

COleDateTime CDateHelper::GetStartOfYear(const COleDateTime& date)
{
	int nStartInMonths = ((GetDateInMonths(date) / 12) * 12);

	return GetDateFromMonths(nStartInMonths);
}

COleDateTime CDateHelper::GetEndOfYear(const COleDateTime& date)
{
	int nStartInMonths = ((GetDateInMonths(date) / 12) * 12);
	int nStartNextInMonths = (nStartInMonths + 12);

	return (GetDateFromMonths(nStartNextInMonths).m_dt - 1.0); // day before next year
}

COleDateTime CDateHelper::GetStartOfDecade(const COleDateTime& date, BOOL bZeroBased)
{
	return GetStartOfEpoch(date, 10, bZeroBased);
}

COleDateTime CDateHelper::GetEndOfDecade(const COleDateTime& date, BOOL bZeroBased)
{
	return GetEndOfEpoch(date, 10, bZeroBased);
}

COleDateTime CDateHelper::GetStartOfQuarterCentury(const COleDateTime& date, BOOL bZeroBased)
{
	return GetStartOfEpoch(date, 25, bZeroBased);
}

COleDateTime CDateHelper::GetEndOfQuarterCentury(const COleDateTime& date, BOOL bZeroBased)
{
	return GetEndOfEpoch(date, 25, bZeroBased);
}

COleDateTime CDateHelper::GetStartOfEpoch(const COleDateTime& date, int nEpochLen, BOOL bZeroBased)
{
	int nStartEpochYear = GetStartOfEpochYear(date, nEpochLen, bZeroBased);

	return GetDateFromMonths(nStartEpochYear * 12);
}

COleDateTime CDateHelper::GetEndOfEpoch(const COleDateTime& date, int nEpochLen, BOOL bZeroBased)
{
	int nStartEpochYear = GetStartOfEpochYear(date, nEpochLen, bZeroBased);
	int nStartNextEpochYear = (nStartEpochYear + nEpochLen);

	return (GetDateFromMonths(nStartNextEpochYear * 12).m_dt - 1.0); // day before next epoch
}

int CDateHelper::GetStartOfEpochYear(const COleDateTime& date, int nEpochLength, BOOL bZeroBased)
{
	int nYear = (GetDateInMonths(date) / 12);

	if (bZeroBased)
		return ((nYear / nEpochLength) * nEpochLength);

	// else
	return ((((nYear - 1) / nEpochLength) * nEpochLength) + 1);
}

CString CDateHelper::FormatDate(const COleDateTime& date, DWORD dwFlags, TCHAR cDateTimeSep)
{
	CString sDate, sTime, sDow;
	
	if (FormatDate(date, dwFlags, sDate, sTime, sDow))
	{
		if (!sDow.IsEmpty())
		{
			sDate = ' ' + sDate;
			sDate = sDow + sDate;
		}

		if (!sTime.IsEmpty())
		{
			sDate += cDateTimeSep;
			sDate += sTime;
		}
	}

	return sDate;
}

CString CDateHelper::FormatCurrentDate(DWORD dwFlags)
{
	return FormatDate(COleDateTime::GetCurrentTime(), dwFlags);
}

BOOL CDateHelper::FormatDate(const COleDateTime& date, DWORD dwFlags, CString& sDate, CString& sTime, CString& sDow)
{
	if (!IsDateSet(date))
		return FALSE;

	SYSTEMTIME st;

	if (!date.GetAsSystemTime(st))
		return FALSE;

	// Date
	CString sFormat;

	if (dwFlags & DHFD_ISO)
	{
		if (dwFlags & DHFD_NOYEAR)
		{
			if (dwFlags & DHFD_NODAY)
				sFormat = _T("MM");
			else
				sFormat = _T("MM-dd");
		}
		else if (dwFlags & DHFD_NODAY)
		{
			if (dwFlags & DHFD_NOCENTURY)
				sFormat = _T("yy-MM");
			else
				sFormat = _T("yyyy-MM");
		}
		else
		{
			if (dwFlags & DHFD_NOCENTURY)
				sFormat = _T("yy-MM-dd");
			else
				sFormat = _T("yyyy-MM-dd");
		}
	}
	else
	{
		sFormat = Misc::GetShortDateFormat();

		if (dwFlags & DHFD_NOYEAR)
		{
			sFormat.Replace(_T("y"), _T(""));
		}
		else if (dwFlags & DHFD_NOCENTURY)
		{
			sFormat.Replace(_T("yyyy"), _T("yy"));
		}

		if (dwFlags & DHFD_NODAY)
		{
			sFormat.Replace(_T("d"), _T(""));
		}
		
		// Handle doubled-up and/or trailing date separators
		if (dwFlags & (DHFD_NOYEAR | DHFD_NODAY))
		{
			CString sSep = Misc::GetDateSeparator();
			Misc::Trim(sFormat, sSep);
			sFormat.Replace((sSep + sSep), sSep);
		}
	}

	sDate = FormatDateOnly(date, sFormat);

	// Day of week
	if (dwFlags & DHFD_DOW)
		sDow = GetDayOfWeekName(GetDayOfWeek(st), TRUE);
	else
		sDow.Empty();

	// Time
	if ((dwFlags & DHFD_TIME) && (!(dwFlags & DHFD_HASTIME) || DateHasTime(date))) 
		sTime = CTimeHelper::FormatClockTime(st.wHour, st.wMinute, st.wSecond, !(dwFlags & DHFD_NOSEC), (dwFlags & DHFD_ISO));
	else 
		sTime.Empty();
	
	return TRUE;
}

CString CDateHelper::FormatDateOnly(const COleDateTime& date, LPCTSTR szFormat)
{
	CString sDate;

	if (!Misc::IsEmpty(szFormat) && IsDateSet(date))
	{
		SYSTEMTIME st;

		if (date.GetAsSystemTime(st))
		{
			// RTL dates
			CString sFormat;

			if ((szFormat[1] != 0) && CJalaliCalendar::IsActive()) // longer than 1 character
			{
				sFormat = szFormat;
				Misc::Reverse(sFormat);
				szFormat = sFormat;
			}

			::GetDateFormat(0, 0, &st, szFormat, sDate.GetBuffer(50), 49);
			sDate.ReleaseBuffer();
		}
	}

	return sDate;
}

// This is just for external use
BOOL CDateHelper::WantRTLDates()
{
	return CJalaliCalendar::IsActive();
}

BOOL CDateHelper::FormatCurrentDate(DWORD dwFlags, CString& sDate, CString& sTime, CString& sDow)
{
	return FormatDate(COleDateTime::GetCurrentTime(), dwFlags, sDate, sTime, sDow);
}

CString CDateHelper::GetDayOfWeekName(OLE_DAYOFWEEK nWeekday, BOOL bShort)
{
	CString sWeekday;

	// data check
	if ((nWeekday >= 1) && (nWeekday <= 7))
	{
		LCTYPE lct = 0;

		switch (nWeekday)
		{
		case DHO_SUNDAY:	lct = (bShort ? LOCALE_SABBREVDAYNAME7 : LOCALE_SDAYNAME7); break;
		case DHO_MONDAY:	lct = (bShort ? LOCALE_SABBREVDAYNAME1 : LOCALE_SDAYNAME1); break;
		case DHO_TUESDAY:	lct = (bShort ? LOCALE_SABBREVDAYNAME2 : LOCALE_SDAYNAME2); break;
		case DHO_WEDNESDAY:	lct = (bShort ? LOCALE_SABBREVDAYNAME3 : LOCALE_SDAYNAME3); break;
		case DHO_THURSDAY:	lct = (bShort ? LOCALE_SABBREVDAYNAME4 : LOCALE_SDAYNAME4); break;
		case DHO_FRIDAY:	lct = (bShort ? LOCALE_SABBREVDAYNAME5 : LOCALE_SDAYNAME5); break;
		case DHO_SATURDAY:	lct = (bShort ? LOCALE_SABBREVDAYNAME6 : LOCALE_SDAYNAME6); break;
		}
		ASSERT(lct);

		GetLocaleInfo(LOCALE_USER_DEFAULT, lct, sWeekday.GetBuffer(30), 29);
		sWeekday.ReleaseBuffer();
	}

	return sWeekday;
}

int CDateHelper::GetMaxDayOfWeekNameWidth(CDC* pDC, BOOL bShort)
{
	return GetMaxDayOfWeekNameExtent(pDC, bShort).cx;
}

CSize CDateHelper::GetMaxDayOfWeekNameExtent(CDC* pDC, BOOL bShort)
{
	CSize sizeMax(0, 0);
		
	// figure out the longest day in pixels
	for (int nDOW = 0; nDOW < 7; nDOW++)
	{
		CString sDOW = GetDayOfWeekName(OLE_DAYSOFWEEK[nDOW], bShort);
		CSize sizeDOW = pDC->GetTextExtent(sDOW);

		sizeMax.cx = max(sizeMax.cx, sizeDOW.cx);
		sizeMax.cy = max(sizeMax.cy, sizeDOW.cy);
	}
	
	return sizeMax;
}

int CDateHelper::GetMaxMonthNameWidth(CDC* pDC, BOOL bShort)
{
	return GetMaxMonthNameExtent(pDC, bShort).cx;
}

CSize CDateHelper::GetMaxMonthNameExtent(CDC* pDC, BOOL bShort)
{
	CSize sizeMax(0, 0);
		
	// figure out the longest day in pixels
	for (int nMonth = 1; nMonth <= 12; nMonth++)
	{
		CString sMonth = GetMonthName(nMonth, bShort);
		CSize sizeMonth = pDC->GetTextExtent(sMonth);

		sizeMax.cx = max(sizeMax.cx, sizeMonth.cx);
		sizeMax.cy = max(sizeMax.cy, sizeMonth.cy);
	}
	
	return sizeMax;
}

void CDateHelper::GetDayOfWeekNames(BOOL bShort, CStringArray& aNames)
{
	aNames.RemoveAll();

	for (int nDay = 0; nDay < 7; nDay++)
		aNames.Add(GetDayOfWeekName(OLE_DAYSOFWEEK[nDay], bShort));
}

int CDateHelper::GetDaysInMonth(const COleDateTime& date)
{
	int nYear, nMonth;
	GetDateFromMonths(GetDateInMonths(date), nMonth, nYear);

	return GetDaysInMonth(nMonth, nYear);
}

int CDateHelper::GetDaysInMonth(const SYSTEMTIME& st)
{
	return GetDaysInMonth(COleDateTime(st));
}

int CDateHelper::GetDaysInMonth(int nMonth, int nYear)
{
	// Sanity check
	if ((nMonth < 1) || (nMonth > 12))
	{
		ASSERT(0);
		return 0;
	}

	if (CJalaliCalendar::IsActive())
		return CJalaliCalendar::GetDaysInMonth(nYear, nMonth);

	// else
	return GetGregorianDaysInMonth(nMonth, nYear);
}

int CDateHelper::GetGregorianDaysInMonth(int GMonth, int GYear)
{
	switch (GMonth)
	{
	case 1:  return 31; // jan
	case 2:	 return (IsGregorianLeapYear(GYear) ? 29 : 28); // feb
	case 3:  return 31; // mar
	case 4:  return 30; // apr
	case 5:  return 31; // may
	case 6:  return 30; // jun
	case 7:  return 31; // jul
	case 8:  return 31; // aug
	case 9:  return 30; // sep
	case 10: return 31; // oct
	case 11: return 30; // nov
	case 12: return 31; // dec
	}

	ASSERT(0);
	return 0;
}

BOOL CDateHelper::IsToday(const COleDateTime& date)
{
	int nToday = (int)COleDateTime::GetCurrentTime().m_dt;

	return ((int)date.m_dt == nToday);
}

BOOL CDateHelper::IsSameDay(const COleDateTime& date1, const COleDateTime& date2)
{
	if (!IsDateSet(date1) || !IsDateSet(date2))
	{
		ASSERT(0);
		return FALSE;
	}

	return ((int)date1.m_dt == (int)date2.m_dt);
}

BOOL CDateHelper::IsSameWeek(const COleDateTime& date1, const COleDateTime& date2)
{
	ASSERT(IsDateSet(date1) && IsDateSet(date2));

	if (!IsSameMonth(date1, date2))
		return FALSE;

	return (GetWeekOfYear(date1) == GetWeekOfYear(date2));
}

BOOL CDateHelper::IsThisWeek(const COleDateTime& date)
{
	return IsSameWeek(date, COleDateTime::GetCurrentTime());
}

BOOL CDateHelper::IsSameMonth(const COleDateTime& date1, const COleDateTime& date2)
{
	return (GetDateInMonths(date1) == GetDateInMonths(date2));
}

BOOL CDateHelper::IsEndOfMonth(const COleDateTime& date)
{
	return (date.GetDay() == GetDaysInMonth(date));
}

BOOL CDateHelper::IsEndOfMonth(const SYSTEMTIME& st)
{
	return ((int)st.wDay >= GetDaysInMonth(st));
}

BOOL CDateHelper::IsSameYear(const COleDateTime& date1, const COleDateTime& date2)
{
	return (date1.GetYear() == date2.GetYear());
}

BOOL CDateHelper::IsLeapYear(const COleDateTime& date)
{
	if (!IsDateSet(date))
	{
		ASSERT(0);
		return FALSE;
	}

	int nUnused, nYear;
	GetDateFromMonths(GetDateInMonths(date), nUnused, nYear);

	return IsLeapYear(nYear);
}

BOOL CDateHelper::IsLeapYear(int nYear)
{
	ASSERT(nYear);

	if (CJalaliCalendar::IsActive())
		return CJalaliCalendar::IsLeapYear(nYear);

	// else
	return IsGregorianLeapYear(nYear);
}

BOOL CDateHelper::IsGregorianLeapYear(int GYear)
{
	ASSERT(GYear);

	return ((GYear % 4 == 0) && ((GYear % 100 != 0) || (GYear % 400 == 0)));
}

BOOL CDateHelper::IsEndOfDay(const COleDateTime& date, BOOL bNoTimeIsEndOfDay)
{
	if (!IsDateSet(date))
	{
		ASSERT(0);
		return FALSE;
	}

	double dTime = GetTimeOnly(date).m_dt;

	if ((dTime == 0.0) && bNoTimeIsEndOfDay)
		return TRUE;

	// Handle rounding
	return (fabs(dTime - END_OF_DAY) < ONE_SECOND);
}

CString CDateHelper::GetMonthName(int nMonth, BOOL bShort)
{
	if (CJalaliCalendar::IsActive())
		return CJalaliCalendar::GetMonthName(nMonth); // no short version

	// data check
	if ((nMonth < 1) || (nMonth > 12))
		return "";

	LCTYPE lct = LOCALE_SABBREVMONTHNAME1;
	CString sMonth;

	switch (nMonth)
	{
	case 1:  lct = (bShort ? LOCALE_SABBREVMONTHNAME1  : LOCALE_SMONTHNAME1);  break; // jan
	case 2:  lct = (bShort ? LOCALE_SABBREVMONTHNAME2  : LOCALE_SMONTHNAME2);  break; // feb
	case 3:  lct = (bShort ? LOCALE_SABBREVMONTHNAME3  : LOCALE_SMONTHNAME3);  break; // mar
	case 4:  lct = (bShort ? LOCALE_SABBREVMONTHNAME4  : LOCALE_SMONTHNAME4);  break; // apr
	case 5:  lct = (bShort ? LOCALE_SABBREVMONTHNAME5  : LOCALE_SMONTHNAME5);  break; // may
	case 6:  lct = (bShort ? LOCALE_SABBREVMONTHNAME6  : LOCALE_SMONTHNAME6);  break; // jun
	case 7:  lct = (bShort ? LOCALE_SABBREVMONTHNAME7  : LOCALE_SMONTHNAME7);  break; // jul
	case 8:  lct = (bShort ? LOCALE_SABBREVMONTHNAME8  : LOCALE_SMONTHNAME8);  break; // aug
	case 9:  lct = (bShort ? LOCALE_SABBREVMONTHNAME9  : LOCALE_SMONTHNAME9);  break; // sep
	case 10: lct = (bShort ? LOCALE_SABBREVMONTHNAME10 : LOCALE_SMONTHNAME10); break; // oct
	case 11: lct = (bShort ? LOCALE_SABBREVMONTHNAME11 : LOCALE_SMONTHNAME11); break; // nov
	case 12: lct = (bShort ? LOCALE_SABBREVMONTHNAME12 : LOCALE_SMONTHNAME12); break; // dec
	}

	GetLocaleInfo(LOCALE_USER_DEFAULT, lct, sMonth.GetBuffer(30),	29);
	sMonth.ReleaseBuffer();

	return sMonth;
}

void CDateHelper::GetMonthNames(BOOL bShort, CStringArray& aMonths)
{
	aMonths.RemoveAll();

	for (int nMonth = 1; nMonth <= 12; nMonth++)
		aMonths.Add(GetMonthName(nMonth, bShort));
}

COleDateTime CDateHelper::GetTimeOnly(const COleDateTime& date)
{
	double dTime = (date.m_dt - GetDateOnly(date));
	
	if (date.m_dt < 0.0)
	{
		ASSERT((dTime <= 0.0) && (dTime > -1.0));
		dTime = -dTime;
	}

	ASSERT((dTime >= 0) && (dTime < 1.0));

	if (dTime < START_OF_DAY)
		return 0.0;

	return dTime;
}

BOOL CDateHelper::DateHasTime(const COleDateTime& date)
{
	return (GetTimeOnly(date).m_dt > 0.0);
}

COleDateTime CDateHelper::GetDateOnly(const COleDateTime& date)
{
	return (double)((int)date.m_dt);
}

COleDateTime CDateHelper::TruncateSeconds(const COleDateTime& date)
{
	double dTime = GetTimeOnly(date).m_dt;
	ASSERT(dTime >= 0.0); // always positive

	if (dTime <= 0)
		return date;

	// Round UP/DOWN to nearest second
	dTime = (Misc::Round(dTime * SECS_IN_DAY) / SECS_IN_DAY);

	// Round DOWN to nearest minute
	dTime = (((int)(dTime * MINS_IN_DAY)) / MINS_IN_DAY);

	return MakeDate(date, dTime);
}

void CDateHelper::SplitDate(const COleDateTime& date, double& dDateOnly, double& dTimeOnly)
{
	dDateOnly = GetDateOnly(date);
	dTimeOnly = GetTimeOnly(date);
}

COleDateTime CDateHelper::MakeDate(const COleDateTime& dtDateOnly, const COleDateTime& dtTimeOnly)
{
	double dDateOnly = GetDateOnly(dtDateOnly);
	double dTimeOnly = GetTimeOnly(dtTimeOnly);

	if (dDateOnly < 0.0)
		return (dDateOnly - dTimeOnly);
	
	return (dDateOnly + dTimeOnly);
}

int CDateHelper::CalcDayOfMonth(OLE_DAYOFWEEK nDOW, int nWhich, int nMonth, int nYear)
{
	// data check
	ASSERT(nMonth >= 1 && nMonth <= 12);
	ASSERT(nDOW >= 1 && nDOW <= 7);
	ASSERT(nWhich >= 1 && nWhich <= 5);

	if ((nMonth < 1) || (nMonth > 12) || (nDOW < 1) || (nDOW > 7) || (nWhich < 1) || (nWhich > 5))
		return -1;

	// start with first day of month
	int nDay = 1;
	COleDateTime date;

	if (CJalaliCalendar::IsActive())
		CJalaliCalendar::ToGregorian(nYear, nMonth, nDay);
	else
		date.SetDate(nYear, nMonth, nDay);

	// get its day of week
	OLE_DAYOFWEEK nWeekDay = GetDayOfWeek(date);

	// move forwards until we hit the requested day of week
	while (nWeekDay != nDOW)
	{
		nDay++;
		nWeekDay = GetNextDayOfWeek(nWeekDay);
	}

	// add a week at a time
	nWhich--;

	while (nWhich--)
		nDay += 7;

	// if we've gone passed the end of the month
	// deduct a week
	if (nDay > GetDaysInMonth(nMonth, nYear))
		nDay -= 7;

	return nDay;
}

COleDateTime CDateHelper::CalcDate(OLE_DAYOFWEEK nDOW, int nWhich, int nMonth, int nYear)
{
	int nDay = CalcDayOfMonth(nDOW, nWhich, nMonth, nYear);

	if (nDay == -1)
		return COleDateTime((time_t)-1);

	return COleDateTime(nYear, nMonth, nDay, 0, 0, 0);
}

BOOL CDateHelper::WantISOWeekOfYear()
{
	// Jalali uses US week number algorithm
	if (CJalaliCalendar::IsActive())
		return FALSE;
	
	// ISO weeks can only begin on Mondays 
	return (GetFirstDayOfWeek() == DHO_MONDAY);
}

int CDateHelper::GetWeekOfYear(const COleDateTime& date)
{
	if (CJalaliCalendar::IsActive())
	{
		int JYear, JMonth, JDay;
		CJalaliCalendar::FromGregorian(date, &JYear, &JMonth, &JDay);

		return CJalaliCalendar::GetWeekOfYear(JYear, JMonth, JDay);
	}

	// else
	int nWeek = 0;
	int nDayOfYear = date.GetDayOfYear();

	if (WantISOWeekOfYear())
	{
		// http://en.wikipedia.org/wiki/ISO_week_date#Calculating_the_week_number_of_a_given_date
		//
		// Using ISO weekday numbers (running from 1 for Monday to 7 for Sunday), 
		// subtract the weekday from the ordinal date, then add 10. Divide the result by 7. 
		// Ignore the remainder; the quotient equals the week number.
		int nISODOW = GetISODayOfWeek(date);

		nWeek = ((nDayOfYear - nISODOW + 10) / 7);

		switch (nWeek)
		{
		case 0:
			// Could be week 52 or 53 of the previous year
			nWeek = (GetWeekOfYear(date.m_dt - 7) + 1); // RECURSIVE CALL
			ASSERT((nWeek == 52) || (nWeek == 53));
			break;

		case 53:
			// Since week 53 could be week 1 of the next year
			// we check the week number a week later
			if (GetWeekOfYear(date.m_dt + 7) == 2) // RECURSIVE CALL
				nWeek = 1;
			break;
		}
	}
	else // defer to US - Week 1 contains 1st Jan
	{
		COleDateTime dtJan1(date.GetYear(), 1, 1, 0, 0, 0);
		int nJan1DOW = dtJan1.GetDayOfWeek();

		nWeek = (((nDayOfYear + nJan1DOW - 1) / 7) + 1);

		switch (nWeek)
		{
		case 53:
			{
				// Since week 53 could be week 1 of the next year
				// we check the week number a week later
				if (GetWeekOfYear(date.m_dt + 7) == 2) // RECURSIVE CALL
					nWeek = 1;
			}
			break;

		case 54:
			nWeek = 1;
			break;
		}
	}
	ASSERT((nWeek >= 1) && (nWeek <= 53));

	return nWeek;
}

COleDateTime CDateHelper::GetNearestDay(const COleDateTime& date, BOOL bEnd)
{
	return GetNearestDayPart(date, 1, bEnd);
}

COleDateTime CDateHelper::GetNearestHalfDay(const COleDateTime& date, BOOL bEnd)
{
	return GetNearestDayPart(date, 2, bEnd);
}

COleDateTime CDateHelper::GetNearestHour(const COleDateTime& date, BOOL bEnd)
{
	return GetNearestDayPart(date, 24, bEnd);
}

COleDateTime CDateHelper::GetNearestHalfHour(const COleDateTime& date, BOOL bEnd)
{
	return GetNearestDayPart(date, 48, bEnd);
}

COleDateTime CDateHelper::GetNearestDayPart(const COleDateTime& date, int nNumParts, BOOL bEnd)
{
	COleDateTime dtDay = GetDateOnly(date);

	double dPart = (1.0 / nNumParts);
	double dTest = (dtDay.m_dt + (dPart / 2));

	for (int nPart = 0; nPart < nNumParts; nPart++)
	{
		if (date.m_dt < dTest)
		{
			// special case: dragging end and first part
			if (bEnd && (nPart == 0))
			{
				// end of previous day
				return GetEndOfPreviousDay(dtDay);
			}

			// else return part 
			return MakeDate(dtDay, (nPart * dPart));
		}

		// test for next part
		dTest += dPart;
	}

	// handle end of day
	if (bEnd)
		return MakeDate(dtDay, END_OF_DAY);

	// else start of next day
	return (dtDay.m_dt + 1.0);
}

COleDateTime CDateHelper::GetEndOfPreviousDay(const COleDateTime& date)
{
	return (GetEndOfDay(date).m_dt - 1.0);
}

COleDateTime CDateHelper::GetEndOfDay(const COleDateTime& date)
{
	ASSERT(IsDateSet(date));

	return MakeDate(date, END_OF_DAY);
}

COleDateTime CDateHelper::GetStartOfNextDay(const COleDateTime& date)
{
	return (GetStartOfDay(date).m_dt + 1.0);
}

COleDateTime CDateHelper::GetStartOfDay(const COleDateTime& date)
{
	ASSERT(IsDateSet(date));

	return GetDateOnly(date);
}

COleDateTime CDateHelper::GetNearestQuarterCentury(const COleDateTime& date, BOOL bEnd, BOOL bZeroBased)
{
	return GetNearestEpoch(date, 25, bEnd, bZeroBased);
}

COleDateTime CDateHelper::GetNearestDecade(const COleDateTime& date, BOOL bEnd, BOOL bZeroBased)
{
	return GetNearestEpoch(date, 10, bEnd, bZeroBased);
}

COleDateTime CDateHelper::GetNearestEpoch(const COleDateTime& date, int nEpochLen, BOOL bEnd, BOOL bZeroBased)
{
	COleDateTime dtThisEpoch = GetStartOfEpoch(date, nEpochLen, bZeroBased);
	COleDateTime dtNextEpoch = (GetEndOfEpoch(date, nEpochLen, bZeroBased).m_dt + 1.0);

	COleDateTime dtNearest;

	if ((date - dtThisEpoch) < (dtNextEpoch - date))
		dtNearest = dtThisEpoch;
	else
		dtNearest = dtNextEpoch;

	// handle end - last second of day before
	if (bEnd)
		dtNearest = GetEndOfPreviousDay(dtNearest);

	return dtNearest;
}

COleDateTime CDateHelper::GetNearestYear(const COleDateTime& date, BOOL bEnd)
{
	return GetNearestMonth(date, 12, bEnd);
}

COleDateTime CDateHelper::GetNearestHalfYear(const COleDateTime& date, BOOL bEnd)
{
	return GetNearestMonth(date, 6, bEnd);
}

COleDateTime CDateHelper::GetNearestQuarter(const COleDateTime& date, BOOL bEnd)
{
	return GetNearestMonth(date, 3, bEnd);
}

COleDateTime CDateHelper::GetNearestMonth(const COleDateTime& date, BOOL bEnd)
{
	return GetNearestMonth(date, 1, bEnd);
}

COleDateTime CDateHelper::GetNearestMonth(const COleDateTime& date, int nInterval, BOOL bEnd)
{
	ASSERT(nInterval > 0);

	int nNumMonths = GetDateInMonths(date);

	if (nInterval > 1)
		nNumMonths = ((nNumMonths / nInterval) * nInterval);

	COleDateTime dtMonth = GetDateFromMonths(nNumMonths);
	COleDateTime dtNextMonth = GetDateFromMonths(nNumMonths + nInterval);;

	COleDateTime dtNearestMonth;

	if ((date - dtMonth) < (dtNextMonth - date))
		dtNearestMonth = dtMonth;
	else
		dtNearestMonth = dtNextMonth;

	if (bEnd)
		dtNearestMonth = GetEndOfPreviousDay(dtNearestMonth);

	return dtNearestMonth;
}

COleDateTime CDateHelper::GetNearestWeek(const COleDateTime& date, BOOL bEnd)
{
	COleDateTime dtWeek = GetDateOnly(date);

	// work forward until the week changes
	int nWeek = GetWeekOfYear(date);

	do
	{
		dtWeek.m_dt += 1.0;
	}
	while (GetWeekOfYear(dtWeek) == nWeek);

	// if the number of days added >= 4 then subtract a week
	CTwentyFourSevenWeek week;

	if (CDateHelper(week).CalcDaysFromTo(date, dtWeek, TRUE) >= 4)
	{
		dtWeek.m_dt -= 7.0;
	}

	// handle end - last second of day before
	if (bEnd)
		dtWeek = GetEndOfPreviousDay(dtWeek);

	return dtWeek;
}

int CDateHelper::GetDateInMonths(int nMonth, int nYear)
{
	ASSERT((nYear > 0) && (nMonth > 0) && (nMonth <= 12));

	return ((nYear * 12) + (nMonth - 1));
}

void CDateHelper::GetDateFromMonths(int nNumMonths, int& nMonth, int& nYear)
{
	nYear = (nNumMonths / 12);
	nMonth = ((nNumMonths % 12) + 1);
}

COleDateTime CDateHelper::GetDateFromMonths(int nNumMonths)
{
	int nMonth, nYear;
	GetDateFromMonths(nNumMonths, nMonth, nYear);

	if (CJalaliCalendar::IsActive())
		return CJalaliCalendar::ToGregorian(nYear, nMonth, 1);

	// else
	return COleDateTime(nYear, nMonth, 1, 0, 0, 0);
}

int CDateHelper::GetDateInMonths(const COleDateTime& date)
{
	ASSERT(IsDateSet(date));

	if (CJalaliCalendar::IsActive())
	{
		int JYear, JMonth, JDay;
		CJalaliCalendar::FromGregorian(date, &JYear, &JMonth, &JDay);

		return GetDateInMonths(JMonth, JYear);
	}

	// else
	return GetDateInMonths(date.GetMonth(), date.GetYear());
}

int CDateHelper::CalcMonthsFromTo(const COleDateTime& dateFrom, const COleDateTime& dateTo, BOOL bInclusive)
{
	ASSERT(dateFrom <= dateTo);

	int nNumMonthsFrom = GetDateInMonths(dateFrom);
	int nNumMonthsTo = GetDateInMonths(dateTo);

	return ((nNumMonthsTo - nNumMonthsFrom) + (bInclusive ? 1 : 0));
}

void CDateHelper::IncrementMonth(SYSTEMTIME& st, int nBy, BOOL bPreserveEndOfMonth)
{
	int GDay = st.wDay;
	int GMonth = st.wMonth;
	int GYear = st.wYear;

	if (CJalaliCalendar::IsActive())
	{
		// The Jalali conversion code expects a fully validated date
		int nDaysInMonth = GetGregorianDaysInMonth(GMonth, GYear);
		GDay = min(GDay, nDaysInMonth);
		
		int JDay, JMonth, JYear;
		CJalaliCalendar::FromGregorian(GYear, GMonth, GDay, &JYear, &JMonth, &JDay);

		BOOL bWasEndOfMonth = CJalaliCalendar::IsEndOfMonth(JYear, JMonth, JDay);
		IncrementMonth(JMonth, JYear, nBy);

		int JDaysInMonth = CJalaliCalendar::GetDaysInMonth(JYear, JMonth);

		if (bPreserveEndOfMonth && bWasEndOfMonth)
		{
			JDay = JDaysInMonth;
		}
		else
		{
			JDay = max(JDay, 1);
			JDay = min(JDay, JDaysInMonth);
		}

		CJalaliCalendar::ToGregorian(JYear, JMonth, JDay, &GYear, &GMonth, &GDay);
	}
	else
	{
		IncrementMonth(GMonth, GYear, nBy);

		// Validate day and preserve 'end-of month'
		int GDaysInMonth = GetDaysInMonth(GMonth, GYear);

		if (bPreserveEndOfMonth && IsEndOfMonth(st))
		{
			GDay = GDaysInMonth;
		}
		else
		{
			GDay = max(GDay, 1);
			GDay = min(GDay, GDaysInMonth);
		}
	}

	st.wDay = (WORD)GDay;
	st.wMonth = (WORD)GMonth;
	st.wYear = (WORD)GYear;
}

void CDateHelper::IncrementMonth(COleDateTime& date, int nBy, BOOL bPreserveEndOfMonth)
{
	SYSTEMTIME st = { 0 };
	
	if (date.GetAsSystemTime(st))
	{
		IncrementMonth(st, nBy, bPreserveEndOfMonth);
		date = COleDateTime(st);
	}
}

void CDateHelper::IncrementYear(COleDateTime& date, int nBy, BOOL bPreserveEndOfMonth)
{
	IncrementMonth(date, (nBy * 12), bPreserveEndOfMonth);
}

void CDateHelper::IncrementMonth(int& nMonth, int& nYear, int nBy)
{
	BOOL bNext = (nBy >= 0);
	nBy = abs(nBy);

	while (nBy--)
		GetNextMonth(nMonth, nYear, bNext);
}

void CDateHelper::GetNextMonth(int& nMonth, int& nYear, BOOL bNext)
{
	if (bNext)
	{
		if (nMonth == 12)
		{
			nYear++;
			nMonth = 1;
		}
		else
		{
			nMonth++;
		}
	}
	else // previous
	{
		if (nMonth == 1)
		{
			nYear--;
			nMonth = 12;
		}
		else
		{
			nMonth--;
		}
	}
}

