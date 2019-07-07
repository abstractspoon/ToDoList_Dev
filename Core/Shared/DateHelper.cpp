// DateHelper.cpp: implementation of the CDateHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DateHelper.h"
#include "TimeHelper.h"
#include "misc.h"
#include "WorkingWeek.h"

#include "..\3rdParty\T64Utils.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const double ONE_HOUR	= (1.0 / 24);
const double HALF_HOUR	= (ONE_HOUR / 2);
const double ONE_MINUTE = (1.0 / (24 * 60));
const float  ONE_SECOND	= (1.0f / (24 * 60 * 60)); // intentionally 'float' for less precision
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

COleDateTimeRange& COleDateTimeRange::operator=(const COleDateTimeRange& range)
{
	m_dtStart = range.m_dtStart;
	m_dtEnd = range.m_dtEnd;
	m_bInclusive = range.m_bInclusive;

	return *this;
}

BOOL COleDateTimeRange::operator==(const COleDateTimeRange& range) const
{
	if (!IsValid() || !range.IsValid())
		return FALSE;

	return ((m_dtStart == range.m_dtStart) && 
			(m_dtEnd == range.m_dtEnd) && 
			(m_bInclusive == range.m_bInclusive));
}

void COleDateTimeRange::Reset()
{
	m_bInclusive = TRUE;

	CDateHelper::ClearDate(m_dtStart);
	CDateHelper::ClearDate(m_dtEnd);
}

BOOL COleDateTimeRange::Set(const COleDateTime& dtStart, const COleDateTime& dtEnd, BOOL bInclusive)
{
	if (!CDateHelper::IsDateSet(dtStart) || !CDateHelper::IsDateSet(dtEnd) || (dtEnd < dtStart))
	{
		ASSERT(0);
		return FALSE;
	}

	m_dtStart = dtStart;
	m_dtEnd = dtEnd;
	m_bInclusive = (bInclusive || CDateHelper::IsEndOfDay(dtEnd));

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
	
	if (!CDateHelper::OffsetDate(dtEnd, nEndOffset, nOffsetUnits))
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
			(m_dtEnd >= m_dtStart));
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

	return (Contains(dtOther.GetStart()) && Contains(dtOther.GetEndInclusive()));
}

BOOL COleDateTimeRange::IntersectsWith(const COleDateTimeRange& dtOther) const
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
	if (!dtOther1.IntersectsWith(dtOther2))
		return FALSE;

	COleDateTime dtStart = max(dtOther1.m_dtStart, dtOther2.m_dtStart);
	COleDateTime dtEnd = min(dtOther1.GetEndInclusive(), dtOther2.GetEndInclusive());

	return Set(dtStart, dtEnd, (dtOther1.m_bInclusive || dtOther2.m_bInclusive));
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
			
	if (m_bInclusive && CDateHelper::IsEndOfDay(m_dtEnd))
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

	return CDateHelper::CalcDaysFromTo(m_dtStart, m_dtEnd, m_bInclusive, FALSE);
}

int COleDateTimeRange::GetWeekdayCount() const
{
	if (!IsValid())
		return 0;

	return CDateHelper::CalcDaysFromTo(m_dtStart, m_dtEnd, m_bInclusive, TRUE);
}

BOOL COleDateTimeRange::Offset(int nAmount, DH_UNITS nUnits)
{
	if (!IsValid())
		return FALSE;

	COleDateTimeRange prevRange = *this;

	if (!CDateHelper::OffsetDate(m_dtStart, nAmount, nUnits) || 
		!CDateHelper::OffsetDate(m_dtEnd, nAmount, nUnits))
	{
		*this = prevRange;
		return FALSE;
	}

	return TRUE;
}

BOOL COleDateTimeRange::OffsetStart(int nAmount, DH_UNITS nUnits)
{
	if (!IsValid())
		return FALSE;

	COleDateTimeRange prevRange = *this;

	if (!CDateHelper::OffsetDate(m_dtStart, nAmount, nUnits) || !IsValid())
	{
		*this = prevRange;
		return FALSE;
	}

	return TRUE;
}

BOOL COleDateTimeRange::OffsetEnd(int nAmount, DH_UNITS nUnits)
{
	if (!IsValid())
		return FALSE;

	COleDateTimeRange prevRange = *this;

	if (!CDateHelper::OffsetDate(m_dtEnd, nAmount, nUnits) || !IsValid())
	{
		*this = prevRange;
		return FALSE;
	}

	return TRUE;
}

CString COleDateTimeRange::Format(DWORD dwFlags, TCHAR cDelim) const
{
	CString sRange;

	if (IsValid())
	{
		if (cDelim)
			sRange.Format(_T("%s %c %s"), CDateHelper::FormatDate(m_dtStart, dwFlags), cDelim, CDateHelper::FormatDate(m_dtEnd, dwFlags));
		else
			sRange.Format(_T("%s %s"), CDateHelper::FormatDate(m_dtStart, dwFlags), CDateHelper::FormatDate(m_dtEnd, dwFlags));
	}

	return sRange;
}

//////////////////////////////////////////////////////////////////////

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

int CDateHelper::CalcDaysFromTo(const COleDateTime& dateFrom, const COleDateTime& dateTo, BOOL bInclusive, BOOL bWeekdays)
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

	if (bWeekdays && (nDays > 0))
	{
		nDays = 0;

		while (dFrom < dTo)
		{
			OLE_DAYOFWEEK nDOW = GetDayOfWeek(dFrom);

			if (!CWeekend().IsWeekend(nDOW))
				nDays++;

			dFrom += 1;
		}
	}

	return nDays;
}

int CDateHelper::CalcDaysFromTo(const COleDateTime& dateFrom, DH_DATE nTo, BOOL bInclusive, BOOL bWeekdays)
{
	return CalcDaysFromTo(dateFrom, GetDate(nTo), bInclusive, bWeekdays);
}

int CDateHelper::CalcDaysFromTo(DH_DATE nFrom, DH_DATE nTo, BOOL bInclusive, BOOL bWeekdays)
{
	return CalcDaysFromTo(GetDate(nFrom), GetDate(nTo), bInclusive, bWeekdays);
}

BOOL CDateHelper::DecodeDate(const CString& sDate, double& date, BOOL bAndTime)
{
	COleDateTime dt;

	if (DecodeDate(sDate, dt, bAndTime))
	{
		date = dt;
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
BOOL CDateHelper::DecodeOffset(LPCTSTR szDate, double& dAmount, DH_UNITS& nUnits, BOOL bMustHaveSign)
{
	return DecodeOffsetEx(szDate, dAmount, nUnits, DHU_DAYS, bMustHaveSign);
}

// internal
BOOL CDateHelper::DecodeOffsetEx(LPCTSTR szDate, double& dAmount, 
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

	// Rest is number (note: ttof ignores any trailing letters)
	dAmount = (nSign * _ttof(sDate));
	return TRUE;
}

BOOL CDateHelper::DecodeRelativeDate(LPCTSTR szDate, COleDateTime& date, BOOL bForceWeekday, BOOL bMustHaveSign)
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
	double dAmount = 0.0;

	if (!sDate.IsEmpty())
	{
		DH_UNITS nUnits = DHU_NULL;

		if (!DecodeOffsetEx(sDate, dAmount, nUnits, DHU_NULL, bMustHaveSign))
			return FALSE;

		// Handle missing units
		if (!nUnits)
		{
			if (IsValidUnit(nFirst))
				nUnits = (DH_UNITS)nFirst;
			else
				nUnits = DHU_DAYS; // default
		}

		if ((dAmount != 0.0) && !OffsetDate(date, (int)dAmount, nUnits))
			return FALSE;
	}

	// does the caller only want weekdays
	if (CWorkingWeek().HasWeekend() && bForceWeekday)
		CWorkingWeek().MakeWeekday(date, (dAmount >= 0.0), bTruncateTime);

	return TRUE;
}

BOOL CDateHelper::IsValidRelativeDate(LPCTSTR szDate, BOOL bMustHaveSign)
{
	COleDateTime dtUnused;

	return DecodeRelativeDate(szDate, dtUnused, FALSE, bMustHaveSign);
}

BOOL CDateHelper::DecodeDate(const CString& sDate, COleDateTime& date, BOOL bAndTime)
{
	// check for valid date string
	if (date.ParseDateTime(sDate, VAR_DATEVALUEONLY))
	{
		if (bAndTime)
			return date.ParseDateTime(sDate, 0);

		// else
		return TRUE;
	}

	// Perhaps it's in ISO format
	if (DecodeISODate(sDate, date, bAndTime))
		return TRUE;

	// all else
	return DecodeLocalShortDate(sDate, date);
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

	timeT = (time_t)date;
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
	COleDateTime date;

	switch (nDate)
	{
	case DHD_NOW:
		return COleDateTime::GetCurrentTime(); // no truncation

	// All the rest have 'time of day' removed
	case DHD_TODAY:
		date = COleDateTime::GetCurrentTime();
		break;

	case DHD_YESTERDAY:
		return (GetDate(DHD_TODAY) - 1.0);
		
	case DHD_TOMORROW:
		return (GetDate(DHD_TODAY) + 1.0);

	case DHD_BEGINTHISWEEK:
		return (GetDate(DHD_ENDTHISWEEK) - 6);

	case DHD_ENDTHISWEEK:
		{
			// we must get the locale info to find out when this 
			// user's week starts
			date = COleDateTime::GetCurrentTime();
			
			// increment the date until we hit the last day of the week
			// note: we could have kept checking date.GetDayOfWeek but
			// it's a lot of calculation that's just not necessary
			OLE_DAYOFWEEK nLastDOW = GetLastDayOfWeek();
			OLE_DAYOFWEEK nDOW = GetDayOfWeek(date);
			
			while (nDOW != nLastDOW) 
			{
				date += 1;
				nDOW = GetNextDayOfWeek(nDOW);
			}
		}
		break;

	case DHD_ENDNEXTWEEK:
		return (GetDate(DHD_ENDTHISWEEK) + 7.0);

	case DHD_BEGINTHISMONTH:
		date = COleDateTime::GetCurrentTime();
		return COleDateTime(date.GetYear(), date.GetMonth(), 1, 0, 0, 0);

	case DHD_ENDTHISMONTH:
		{
			date = COleDateTime::GetCurrentTime();
			int nThisMonth = date.GetMonth();

			while (date.GetMonth() == nThisMonth)
				date += 10; // much quicker than doing it one day at a time

			date -= date.GetDay(); // because we went into next month
		}
		break;

	case DHD_ENDNEXTMONTH:
		{
			date = GetDate(DHD_ENDTHISMONTH) + 1; // first day of next month
			int nNextMonth = date.GetMonth();

			while (date.GetMonth() == nNextMonth)
				date += 20; // much quicker than doing it one day at a time

			date -= date.GetDay(); // because we went into next month + 1
		}
		break;

	case DHD_BEGINTHISYEAR:
		date = COleDateTime::GetCurrentTime();
		return COleDateTime(date.GetYear(), 1, 1, 0, 0, 0);

	case DHD_ENDTHISYEAR:
		date = COleDateTime::GetCurrentTime(); // for current year
		return COleDateTime(date.GetYear(), 12, 31, 0, 0, 0);

	case DHD_ENDNEXTYEAR:
		date = COleDateTime::GetCurrentTime(); // for current year
		return COleDateTime(date.GetYear() + 1, 12, 31, 0, 0, 0);

	default:
		ASSERT (0);
		return 0;
	}

	return GetDateOnly(date);
}

BOOL CDateHelper::Min(COleDateTime& date, const COleDateTime& dtOther)
{
	if (IsDateSet(date))
	{
		if (IsDateSet(dtOther))
		{
			date.m_dt = min(dtOther.m_dt, date.m_dt);
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
	if (IsDateSet(date))
	{
		if (IsDateSet(dtOther))
		{
			date.m_dt = max(dtOther.m_dt, date.m_dt);
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

	if ((dwCompareFlags & DHC_COMPARETIME) == 0)
	{
		// Compare dates only
		double dDateOnly1 = CDateHelper::GetDateOnly(date1).m_dt;
		double dDateOnly2 = CDateHelper::GetDateOnly(date2).m_dt;

		return ((dDateOnly1 < dDateOnly2) ? -1 : ((dDateOnly1 > dDateOnly2) ? 1 : 0));
	}

	// else include time
	double dDateTime1(date1.m_dt), dDateTime2(date2.m_dt);

	if (dwCompareFlags & DHC_NOTIMEISENDOFDAY)
	{
		if (!CDateHelper::DateHasTime(date1))
			dDateTime1 = CDateHelper::GetEndOfDay(date1).m_dt;

		if (!CDateHelper::DateHasTime(date2))
			dDateTime2 = CDateHelper::GetEndOfDay(date2).m_dt;
	}

	if ((dwCompareFlags & DHC_COMPARESECONDS) == 0)
	{
		if (fabs(dDateTime1 - dDateTime2) < ONE_MINUTE)
			return 0;
	}

	return ((dDateTime1 < dDateTime2) ? -1 : (dDateTime1 > dDateTime2) ? 1 : 0);
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
	UINT nWeekdays[14] = 
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

DH_DAYOFWEEK CDateHelper::Map(OLE_DAYOFWEEK nDOW)
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

OLE_DAYOFWEEK CDateHelper::Map(DH_DAYOFWEEK nDOW)
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

COleDateTime CDateHelper::GetStartOfMonth(const COleDateTime& date)
{
	return COleDateTime(date.GetYear(), date.GetMonth(), 1, 0, 0, 0);
}

COleDateTime CDateHelper::GetEndOfMonth(const COleDateTime& date)
{
	COleDateTime dtEnd = GetStartOfMonth(date);

	return (dtEnd.m_dt + GetDaysInMonth(date) - 1);
}

COleDateTime CDateHelper::GetStartOfQuarter(const COleDateTime& date)
{
	int nQuarter = ((date.GetMonth() - 1) / 3);
	int nMonth = (1 + (nQuarter * 3));

	return COleDateTime(date.GetYear(), nMonth, 1, 0, 0, 0);
}

COleDateTime CDateHelper::GetEndOfQuarter(const COleDateTime& date)
{
	COleDateTime dtEnd = GetStartOfQuarter(date);
	VERIFY(OffsetDate(dtEnd, 3, DHU_MONTHS));

	return (dtEnd.m_dt - 1.0);
}

COleDateTime CDateHelper::GetStartOfYear(const COleDateTime& date)
{
	return COleDateTime(date.GetYear(), 1, 1, 0, 0, 0);
}

COleDateTime CDateHelper::GetEndOfYear(const COleDateTime& date)
{
	return COleDateTime(date.GetYear(), 12, 31, 0, 0, 0);
}

COleDateTime CDateHelper::GetStartOfDecade(const COleDateTime& date, BOOL bZeroBased)
{
	int nYear = date.GetYear();

	if (bZeroBased)
		nYear = ((nYear / 10) * 10);
	else
		nYear = ((((nYear - 1) / 10) * 10) + 1);

	return COleDateTime(nYear, 1, 1, 0, 0, 0);
}

COleDateTime CDateHelper::GetEndOfDecade(const COleDateTime& date, BOOL bZeroBased)
{
	int nYear = GetStartOfDecade(date, bZeroBased).GetYear();
	
	return COleDateTime((nYear + 9), 12, 31, 0, 0, 0);
}

COleDateTime CDateHelper::GetStartOfQuarterCentury(const COleDateTime& date, BOOL bZeroBased)
{
	int nYear = date.GetYear();
	
	if (bZeroBased)
		nYear = ((nYear / 25) * 25);
	else
		nYear = ((((nYear - 1) / 25) * 25) + 1);
	
	return COleDateTime(nYear, 1, 1, 0, 0, 0);
}

COleDateTime CDateHelper::GetEndOfQuarterCentury(const COleDateTime& date, BOOL bZeroBased)
{
	int nYear = GetStartOfQuarterCentury(date, bZeroBased).GetYear();
	
	return COleDateTime((nYear + 24), 12, 31, 0, 0, 0);
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

	::GetDateFormat(0, 0, &st, sFormat, sDate.GetBuffer(50), 49);
	sDate.ReleaseBuffer();

	// Day of week
	if (dwFlags & DHFD_DOW)
		sDow = GetDayOfWeekName(GetDayOfWeek(st), TRUE);
	else
		sDow.Empty();

	// Time
	if (dwFlags & DHFD_TIME)
		sTime = CTimeHelper::FormatClockTime(st.wHour, st.wMinute, st.wSecond, !(dwFlags & DHFD_NOSEC), (dwFlags & DHFD_ISO));
	else 
		sTime.Empty();
	
	return TRUE;
}

BOOL CDateHelper::FormatCurrentDate(DWORD dwFlags, CString& sDate, CString& sTime, CString& sDow)
{
	return FormatDate(COleDateTime::GetCurrentTime(), dwFlags, sDate, sTime, sDow);
}

CString CDateHelper::GetDayOfWeekName(OLE_DAYOFWEEK nWeekday, BOOL bShort)
{
	LCTYPE lct = bShort ? LOCALE_SABBREVDAYNAME1 : LOCALE_SDAYNAME1;
	CString sWeekday;

	// data check
	if (nWeekday < 1 || nWeekday > 7)
		return "";

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
	
	GetLocaleInfo(LOCALE_USER_DEFAULT, lct, sWeekday.GetBuffer(30),	29);
	sWeekday.ReleaseBuffer();

	return sWeekday;
}

int CDateHelper::CalcLongestDayOfWeekName(CDC* pDC, BOOL bShort)
{
	int nLongestWDWidth = 0;
		
	// figure out the longest day in pixels
	for (int nDOW = 0; nDOW < 7; nDOW++)
	{
		int nWDWidth = pDC->GetTextExtent(GetDayOfWeekName(OLE_DAYSOFWEEK[nDOW], bShort)).cx;
		nLongestWDWidth = max(nLongestWDWidth, nWDWidth);
	}
	
	return nLongestWDWidth;
}

BOOL CDateHelper::OffsetDate(COleDateTime& date, int nAmount, DH_UNITS nUnits)
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
		{
			BOOL bForwards = (nAmount > 0);
			nAmount = abs(nAmount);

			while (nAmount--)
			{
				date.m_dt += (bForwards ? 1.0 : -1.0);

				// skip weekends
				CWorkingWeek().MakeWeekday(date, bForwards);
			}
		}
		break;

	case DHU_DAYS:
		date += (double)nAmount;
		break;

	case DHU_WEEKS:
		date += nAmount * 7.0;
		break;

	case DHU_MONTHS:
		{
			SYSTEMTIME st;
			date.GetAsSystemTime(st);

			// are we at months end?
			int nDaysInMonth = GetDaysInMonth(st.wMonth, st.wYear);
			BOOL bEndOfMonth = (st.wDay == nDaysInMonth);

			// convert amount to years and months
			int nYear = st.wYear, nMonth = st.wMonth;

			nYear = (nYear + (nAmount / 12));
			nMonth = (nMonth + (nAmount % 12));

			// handle over/underflow
			while (nMonth > 12)
			{
				nYear++;
				nMonth -= 12;
			}
			
			while (nMonth < 1)
			{
				nYear--;
				nMonth += 12;
			}

			st.wYear = (WORD)nYear;
			st.wMonth = (WORD)nMonth;

			// if our start date was the end of the month make
			// sure out end date is too
			nDaysInMonth = GetDaysInMonth(st.wMonth, st.wYear);

			if (bEndOfMonth)
			{
				st.wDay = (WORD)nDaysInMonth;
			}
			else 
			{
				// clip dates to the end of the month
				st.wDay = min(st.wDay, (WORD)nDaysInMonth);
			}

			// update time
			date = COleDateTime(st);
		}
		break;

	case DHU_YEARS:
		{
			SYSTEMTIME st;
			date.GetAsSystemTime(st);

			// update year
			st.wYear = (WORD)((int)st.wYear + nAmount);

			// update time
			date = COleDateTime(st);
		}
		break;
	}

	return (IsDateSet(date));
}

void CDateHelper::GetDayOfWeekNames(BOOL bShort, CStringArray& aNames)
{
	aNames.RemoveAll();

	for (int nDay = 0; nDay < 7; nDay++)
		aNames.Add(GetDayOfWeekName(OLE_DAYSOFWEEK[nDay], bShort));
}

int CDateHelper::GetDaysInMonth(const COleDateTime& date)
{
	return GetDaysInMonth(date.GetMonth(), date.GetYear());
}

int CDateHelper::GetDaysInMonth(int nMonth, int nYear)
{
	// data check
	ASSERT(nMonth >= 1 && nMonth <= 12);

	if (nMonth < 1 || nMonth> 12)
		return 0;

	switch (nMonth)
	{
	case 1:  return 31; // jan
	case 2:  return (IsLeapYear(nYear) ? 29 : 28); // feb
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
	if (!IsDateSet(date))
		return FALSE;

	double dToday(GetDate(DHD_TODAY));

	return ((date.m_dt >= dToday) && (date.m_dt < (dToday + 1.0)));
}

BOOL CDateHelper::IsSameDay(const COleDateTime& date1, const COleDateTime& date2)
{
	ASSERT(IsDateSet(date1) && IsDateSet(date2));

	return (GetDateOnly(date1) == GetDateOnly(date2));
}

BOOL CDateHelper::IsLeapYear(const COleDateTime& date)
{
	return IsLeapYear(date.GetYear());
}

BOOL CDateHelper::IsEndOfDay(const COleDateTime& date)
{
	double dTime = GetTimeOnly(date).m_dt;

	// Handle rounding
	return (fabs(dTime - END_OF_DAY) < ONE_SECOND);
}

BOOL CDateHelper::IsLeapYear(int nYear)
{
	return ((nYear % 4 == 0) && ((nYear % 100 != 0) || (nYear % 400 == 0)));
}

CString CDateHelper::GetMonthName(int nMonth, BOOL bShort)
{
	LCTYPE lct = LOCALE_SABBREVMONTHNAME1;
	CString sMonth;

	// data check
	if (nMonth < 1 || nMonth> 12)
		return "";

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

	if (nMonth < 1 || nMonth> 12 || nDOW < 1 || nDOW > 7 || nWhich < 1 || nWhich > 5)
		return -1;

	// start with first day of month
	int nDay = 1;
	COleDateTime date(nYear, nMonth, nDay, 0, 0, 0);

	// get it's day of week
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

int CDateHelper::GetWeekofYear(const COleDateTime& date)
{
	int nWeek = 0;
	int nDayOfYear = date.GetDayOfYear();

	// ISO weeks can only begin on Mondays 
	if (GetFirstDayOfWeek() == DHO_MONDAY)
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
			nWeek = (GetWeekofYear(date.m_dt - 7) + 1);
			ASSERT((nWeek == 52) || (nWeek == 53));
			break;

		case 53:
			// Since week 53 could be week 1 of the next year
			// we check the week number a week later
			if (GetWeekofYear(date.m_dt + 7) == 2)
				nWeek = 1;
			break;
		}
	}
	else // defer to US - Week 1 contains 1st Jan
	{
		COleDateTime dtJan1(date.GetYear(), 1, 1, 0, 0, 0);
		int nJan1DOW = dtJan1.GetDayOfWeek();

		nWeek = (((nDayOfYear + nJan1DOW - 1) / 7) + 1);
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

	return (GetDateOnly(date).m_dt + END_OF_DAY);
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

COleDateTime CDateHelper::GetNearestYear(const COleDateTime& date, BOOL bEnd)
{
	COleDateTime dtNearest;

	int nYear  = date.GetYear();
	int nMonth = date.GetMonth();

	if (nMonth > 6)
	{
		// beginning of next year
		dtNearest.SetDate(nYear+1, 1, 1);
	}
	else
	{
		// beginning of this year
		dtNearest.SetDate(nYear, 1, 1);
	}

	ASSERT(IsDateSet(dtNearest));

	// handle end - last second of day before
	if (bEnd)
		dtNearest = GetEndOfPreviousDay(dtNearest);

	return dtNearest;
}

COleDateTime CDateHelper::GetNearestQuarterCentury(const COleDateTime& date, BOOL bEnd, BOOL bZeroBased)
{
	COleDateTime dtNearest;

	int nYear = (date.GetYear() - (bZeroBased ? 0 : 1));
	int nMonth = date.GetMonth();

	if (((nYear % 25) > 5) || (((nYear % 25) == 5) && (nMonth > 6)))
	{
		// beginning of next decade
		dtNearest = (GetEndOfQuarterCentury(date).m_dt + 1.0);
	}
	else
	{
		// beginning of this year
		dtNearest = GetStartOfQuarterCentury(date);
	}

	ASSERT(IsDateSet(dtNearest));

	// handle end - last second of day before
	if (bEnd)
		dtNearest = GetEndOfPreviousDay(dtNearest);

	return dtNearest;
}

COleDateTime CDateHelper::GetNearestDecade(const COleDateTime& date, BOOL bEnd, BOOL bZeroBased)
{
	COleDateTime dtNearest;

	int nYear = (date.GetYear() - (bZeroBased ? 0 : 1));

	if ((nYear % 10) > 5)
	{
		// beginning of next decade
		dtNearest = (GetEndOfDecade(date).m_dt + 1.0);
	}
	else
	{
		// beginning of this year
		dtNearest = GetStartOfDecade(date);
	}

	ASSERT(IsDateSet(dtNearest));

	// handle end - last second of day before
	if (bEnd)
		dtNearest = GetEndOfPreviousDay(dtNearest);

	return dtNearest;
}

COleDateTime CDateHelper::GetNearestHalfYear(const COleDateTime& date, BOOL bEnd)
{
	COleDateTime dtHalfYear;

	int nYear  = date.GetYear();
	int nMonth = date.GetMonth();

	if (nMonth > 9)
	{
		// beginning of next year
		dtHalfYear.SetDate(nYear+1, 1, 1);

	}
	else if (nMonth > 3)
	{
		// beginning of july
		dtHalfYear.SetDate(nYear, 7, 1);
	}
	else
	{
		// beginning of this year
		dtHalfYear.SetDate(nYear, 1, 1);
	}

	ASSERT(IsDateSet(dtHalfYear));

	// handle end - last second of day before
	if (bEnd)
		dtHalfYear = GetEndOfPreviousDay(dtHalfYear);

	return dtHalfYear;
}

COleDateTime CDateHelper::GetNearestQuarter(const COleDateTime& date, BOOL bEnd)
{
	COleDateTime dtQuarter;

	int nYear  = date.GetYear();

	if (date > COleDateTime(nYear, 11, 15, 0, 0, 0))
	{
		// beginning of next year
		dtQuarter.SetDate(nYear+1, 1, 1);
	}
	else if (date > COleDateTime(nYear, 8, 15, 0, 0, 0))
	{
		// beginning of october
		dtQuarter.SetDate(nYear, 10, 1);
	}
	else if (date > COleDateTime(nYear, 5, 15, 0, 0, 0))
	{
		// beginning of july
		dtQuarter.SetDate(nYear, 7, 1);
	}
	else if (date > COleDateTime(nYear, 2, 14, 0, 0, 0))
	{
		// beginning of april
		dtQuarter.SetDate(nYear, 4, 1);
	}
	else
	{
		// beginning of this year
		dtQuarter.SetDate(nYear, 1, 1);
	}

	ASSERT(IsDateSet(dtQuarter));

	// handle end - last second of day before
	if (bEnd)
		dtQuarter = GetEndOfPreviousDay(dtQuarter);

	return dtQuarter;
}

int CDateHelper::GetDateInMonths(int nMonth, int nYear)
{
	ASSERT(nYear > 0 && nMonth > 0 && nMonth <= 12);

	return ((nYear * 12) + (nMonth - 1));
}

int CDateHelper::GetDateInMonths(const COleDateTime& date)
{
	return GetDateInMonths(date.GetMonth(), date.GetYear());
}

int CDateHelper::CalcMonthsFromTo(const COleDateTime& dateFrom, const COleDateTime& dateTo, BOOL bInclusive)
{
	ASSERT(dateFrom <= dateTo);

	int nNumMonthsFrom = GetDateInMonths(dateFrom);
	int nNumMonthsTo = GetDateInMonths(dateTo);

	return ((nNumMonthsTo - nNumMonthsFrom) + (bInclusive ? 1 : 0));
}

void CDateHelper::IncrementMonth(SYSTEMTIME& st, int nBy)
{
	// convert month/year to int
	int nMonth = st.wMonth;
	int nYear = st.wYear;

	IncrementMonth(nMonth, nYear, nBy);

	// Validate day
	int nDayInMonth = GetDaysInMonth(nMonth, nYear);

	st.wDay = max(st.wDay, 1);
	st.wDay = min(st.wDay, nDayInMonth);

	st.wMonth = (WORD)nMonth;
	st.wYear = (WORD)nYear;
}

void CDateHelper::IncrementMonth(COleDateTime& date, int nBy)
{
	SYSTEMTIME st = { 0 };
	
	if (date.GetAsSystemTime(st))
	{
		IncrementMonth(st, nBy);
		date = COleDateTime(st);
	}
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

COleDateTime CDateHelper::GetNearestMonth(const COleDateTime& date, BOOL bEnd)
{
	COleDateTime dtMonth;

	int nYear  = date.GetYear();
	int nMonth = date.GetMonth();
	int nDay   = date.GetDay();

	if (nDay > 15)
	{
		// start of next month
		GetNextMonth(nMonth, nYear);
	}
	// else start of this month

	dtMonth.SetDate(nYear, nMonth, 1);

	// handle end - last second of day before
	if (bEnd)
		dtMonth = GetEndOfPreviousDay(dtMonth);

	return dtMonth;
}

COleDateTime CDateHelper::GetNearestWeek(const COleDateTime& date, BOOL bEnd)
{
	COleDateTime dtWeek = GetDateOnly(date);

	// work forward until the week changes
	int nWeek = GetWeekofYear(date);

	do 
	{
		dtWeek.m_dt += 1.0;
	}
	while (GetWeekofYear(dtWeek) == nWeek);

	// if the number of days added >= 4 then subtract a week
	if (CalcDaysFromTo(date, dtWeek, TRUE, FALSE) >= 4)
		dtWeek.m_dt -= 7.0;

	// handle end - last second of day before
	if (bEnd)
		dtWeek = GetEndOfPreviousDay(dtWeek);

	return dtWeek;
}

