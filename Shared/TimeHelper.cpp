// TimeHelper.cpp: implementation of the TimeHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeHelper.h"
#include "misc.h"

#include <locale.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const double MINS2HOURS 	= 60;
const double HOURS2DAYS 	= 24;
const double DAYS2WEEKS 	= 7;
const double WEEKS2MONTHS 	= 4.348; // 365.25 / (7 * 12)
const double MONTHS2YEARS 	= 12;
const double FUDGE 			= 1e-6;

// user definables
double CTimeHelper::HOURS2WORKDAYS = 8; 
double CTimeHelper::WORKDAYS2WEEKS = 5;

//////////////////////////////////////////////////////////////////////

// assume working days pivot about 1.30pm
// eg. a standard working day of 8 hours (+1 for lunch)
// starts at 9am (13.50 - 4.5) and 
// ends at 6pm (13.30 + 4.5)
const double MIDDAY			= 13.5;
const double LUNCHSTARTTIME = (MIDDAY - 0.5);
const double LUNCHENDTIME	= (MIDDAY + 0.5);

//////////////////////////////////////////////////////////////////////

CMap<TH_UNITS, TH_UNITS, TCHAR, TCHAR&> CTimeHelper::MAPUNIT2CH; // user definable

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeHelper::CTimeHelper() : m_dHours2Workdays(HOURS2WORKDAYS), m_dWorkdays2Weeks(WORKDAYS2WEEKS)
{
}

CTimeHelper::CTimeHelper(double dHoursInWorkday, double dWorkdaysInWeek) 
	: 
	m_dHours2Workdays(dHoursInWorkday), 
	m_dWorkdays2Weeks(dWorkdaysInWeek)
{
}

double CTimeHelper::GetTimeOnly(double dDate)
{
	return (dDate - floor(dDate));
}

double CTimeHelper::GetTimeOnly(const COleDateTime& date)
{
	return GetTimeOnly(date.m_dt);
}

THU_WORKDAYPERIOD CTimeHelper::GetWorkdayPeriod(const COleDateTime& date) const
{
	double dTime = GetTimeOnly(date);

	if (dTime < GetStartOfWorkday())
	{
		return THU_BEFORE;
	}
	else if (dTime < GetStartOfWorkdayLunch())
	{
		return THU_MORNING;
	}
	else if (dTime < GetEndOfWorkdayLunch())
	{
		return THU_LUNCH;
	}
	else if (dTime < GetEndOfWorkday())
	{
		return THU_AFTERNOON;
	}

	// else
	return THU_AFTER;
}

double CTimeHelper::GetStartOfWorkday(BOOL bInDays) const
{
	double dHours = (LUNCHSTARTTIME - (m_dHours2Workdays / 2));
	
	return (bInDays ? (dHours / 24) : dHours);
}

double CTimeHelper::GetStartOfWorkdayLunch(BOOL bInDays) const
{
	double dHours = LUNCHSTARTTIME;

	return (bInDays ? (dHours / 24) : dHours);
}

double CTimeHelper::GetEndOfWorkday(BOOL bInDays) const
{
	double dHours = (LUNCHENDTIME + (m_dHours2Workdays / 2));

	return (bInDays ? (dHours / 24) : dHours);
}

double CTimeHelper::GetEndOfWorkdayLunch(BOOL bInDays) const
{
	double dHours = LUNCHENDTIME;

	return (bInDays ? (dHours / 24) : dHours);
}

void CTimeHelper::CalculatePartWorkdays(const COleDateTime& dtStart, const COleDateTime& dtEnd,
										double& dPartStartDay, double& dPartEndDay, BOOL bInDays) const
{
	// calculate time to end of day
	dPartStartDay = (GetTimeOnly(dtStart) * 24);

	switch (GetWorkdayPeriod(dtStart))
	{
	case THU_BEFORE:
		dPartStartDay = m_dHours2Workdays;
		break;

	case THU_MORNING:
		dPartStartDay = ((GetStartOfWorkdayLunch(FALSE) - dPartStartDay) + (m_dHours2Workdays / 2));
		break;

	case THU_LUNCH:
		dPartStartDay = (m_dHours2Workdays / 2);
		break;

	case THU_AFTERNOON:
		dPartStartDay = (GetEndOfWorkday(FALSE) - dPartStartDay);
		break;

	case THU_AFTER:
		dPartStartDay = 0.0;
		break;

	default:
		ASSERT(0);
		dPartStartDay = 0.0;
		break;
	}

	// calculate time from start of day
	dPartEndDay = (GetTimeOnly(dtEnd) * 24);

	switch (GetWorkdayPeriod(dtEnd))
	{
	case THU_BEFORE:
		dPartEndDay = 0.0;
		break;

	case THU_MORNING:
		dPartEndDay = (dPartEndDay - GetStartOfWorkday(FALSE));
		break;

	case THU_LUNCH:
		dPartEndDay = (m_dHours2Workdays / 2);
		break;

	case THU_AFTERNOON:
		dPartEndDay = ((dPartEndDay - GetEndOfWorkdayLunch(FALSE)) + (m_dHours2Workdays / 2));
		break;

	case THU_AFTER:
		dPartEndDay = m_dHours2Workdays;
		break;

	default:
		ASSERT(0);
		dPartEndDay = 0.0;
		break;
	}

	if (bInDays)
	{
		dPartStartDay /= m_dHours2Workdays;
		dPartEndDay /= m_dHours2Workdays;
	}
}

BOOL CTimeHelper::IsValidUnit(int nUnits)
{
	switch (nUnits)
	{
	case THU_MINS:
	case THU_HOURS:
	case THU_DAYS:
	case THU_WEEKDAYS:
	case THU_WEEKS:
	case THU_MONTHS:
	case THU_YEARS:
		return TRUE;
	}
	
	return FALSE;
}

double CTimeHelper::GetTime(double dTime, TH_UNITS nFromUnits, TH_UNITS nToUnits) const
{
	// sanity check
	if (!IsValidUnit(nFromUnits) || !IsValidUnit(nToUnits))
	{	
		ASSERT(0);
		return 0.0;
	}
	
	if (nFromUnits == nToUnits)
	{
		return dTime;
	}
	else if ((nFromUnits == THU_DAYS) && (nToUnits == THU_WEEKDAYS))
	{
		if (m_dWorkdays2Weeks != DAYS2WEEKS)
			dTime *= (m_dWorkdays2Weeks / DAYS2WEEKS);
	}
	else if ((nFromUnits == THU_WEEKDAYS) && (nToUnits == THU_DAYS))
	{
		if (m_dWorkdays2Weeks != DAYS2WEEKS)
			dTime *= (DAYS2WEEKS / m_dWorkdays2Weeks);
	}
	else if (Compare(nFromUnits, nToUnits) > 0)
	{
		while (Compare(nFromUnits, nToUnits) > 0)
		{
			switch (nFromUnits)
			{
			case THU_HOURS:
				dTime *= MINS2HOURS;
				nFromUnits = THU_MINS;
				break;
				
			case THU_WEEKDAYS:
				dTime *= m_dHours2Workdays;
				nFromUnits = THU_HOURS;
				break;

			case THU_DAYS:
				dTime *= HOURS2DAYS;
				nFromUnits = THU_HOURS;
				break;
				
			case THU_WEEKS:
				nFromUnits = GetDaysToWeeksUnits(nToUnits);
				dTime *= GetDaysToWeeksFactor(nToUnits);
				break;
				
			case THU_MONTHS:
				dTime *= WEEKS2MONTHS;
				nFromUnits = THU_WEEKS;
				break;
				
			case THU_YEARS:
				dTime *= MONTHS2YEARS;
				nFromUnits = THU_MONTHS;
				break;
			}
		}
	}
	else // nFromUnits < nToUnits
	{
		while (Compare(nFromUnits, nToUnits) < 0)
		{
			switch (nFromUnits)
			{
			case THU_MINS:
				dTime /= MINS2HOURS;
				nFromUnits = THU_HOURS;
				break;

			case THU_HOURS:
				if (IsWeekdays(nToUnits))
					dTime /= m_dHours2Workdays;
				else
					dTime /= HOURS2DAYS;

				nFromUnits = GetDaysToWeeksUnits(nToUnits);
				break;

			case THU_WEEKDAYS:
				if (IsWeekdays(nFromUnits))
					dTime /= m_dWorkdays2Weeks;
				else
					dTime /= DAYS2WEEKS;

				nFromUnits = THU_WEEKS;
				break;

			case THU_DAYS:
				dTime /= DAYS2WEEKS;
				nFromUnits = THU_WEEKS;
				break;
				
			case THU_WEEKS:
				dTime /= WEEKS2MONTHS;
				nFromUnits = THU_MONTHS;
				break;
				
			case THU_MONTHS:
				dTime /= MONTHS2YEARS;
				nFromUnits = THU_YEARS;
				break;
			}
		}
	}

	return dTime;
}

int CTimeHelper::Compare(double dTime1, TH_UNITS nUnits1, double dTime2, TH_UNITS nUnits2) const
{
	// Compare equivalent times
	if (nUnits2 != nUnits1)
		dTime2 = GetTime(dTime2, nUnits2, nUnits1);

	return ((dTime1 > dTime2) ? 1 : ((dTime1 < dTime2) ? -1 : 0));
}

CString CTimeHelper::FormatClockTime(const COleDateTime& dtTime, BOOL bIncSeconds, BOOL bISO)
{
	return FormatClockTime(dtTime.GetHour(), dtTime.GetMinute(), dtTime.GetSecond(), bIncSeconds, bISO);
}

CString CTimeHelper::FormatClockTime(int nHour, int nMin, int nSec, BOOL bIncSeconds, BOOL bISO)
{
	CString sFormat;
	LPCTSTR szFormat = NULL;

	if (bISO)
	{
		sFormat = _T("HH:mm");
			
		if (bIncSeconds)
			sFormat += _T(":ss");

		szFormat = sFormat;
	}
	
	CString sTime;
	SYSTEMTIME st = { 0, 0, 0, 0, (WORD)nHour, (WORD)nMin, (WORD)nSec, 0 };
	
	::GetTimeFormat(0, bIncSeconds ? 0 : TIME_NOSECONDS, &st, szFormat, sTime.GetBuffer(50), 49);
	sTime.ReleaseBuffer();

	return sTime;
}

double CTimeHelper::DecodeClockTime(LPCTSTR szTime)
{
	CString sTime(szTime);
	Misc::Trim(sTime);

	if (sTime.IsEmpty())
	{
		return 0; // midnight
	}

	// else
	double dTime = 0;

	// Look for AM/PM
	BOOL bPM = RemovePM(sTime);
	BOOL bAM = (!bPM && RemoveAM(sTime));

	// look for a minute separator
	CStringArray aBits;

	if (Misc::Split(sTime, aBits, Misc::GetTimeSeparator()))
	{
		switch (aBits.GetSize())
		{
		case 3: dTime += (_ttof(aBits[2]) / 3600);	// seconds
		case 2: dTime += (_ttof(aBits[1]) / 60);	// minutes
		case 1: dTime += (_ttof(aBits[0]));			// minutes
		}
	}
	// test for military time
	else if (Misc::IsNumber(sTime) && sTime.GetLength() >= 3)
	{
		// if only 3 digits have been typed, add a zero
		if (sTime.GetLength() == 3)
			sTime += '0';

		dTime = _ttof(sTime.Left(2)) + _ttof(sTime.Mid(2)) / 60;
	}
	else // simple number
	{
		dTime = _ttof(sTime);
	}

	// Modify for PM signifier
	if (bPM && dTime < 12)
		dTime += 12;

	// truncate to 0-24
	return min(max(dTime, 0.0), 24.0);
}

BOOL CTimeHelper::RemovePM(CString& sTime)
{
	CString sPM(Misc::GetPM());
	return (Misc::RemovePrefix(sTime, sPM) || Misc::RemoveSuffix(sTime, sPM));
}

BOOL CTimeHelper::RemoveAM(CString& sTime)
{
	CString sAM(Misc::GetAM());
	return (Misc::RemovePrefix(sTime, sAM) || Misc::RemoveSuffix(sTime, sAM));
}

CString CTimeHelper::FormatTime(double dTime, int nDecPlaces) const
{
	return FormatTime(dTime, THU_NULL, nDecPlaces);
}

CString CTimeHelper::FormatTime(double dTime, TH_UNITS nUnits, int nDecPlaces) const
{
	// sanity check
	if (nUnits && !IsValidUnit(nUnits))
	{	
		ASSERT(0);
		return _T("");
	}

	CString sTime = Misc::Format(dTime, nDecPlaces);
	TCHAR cUnits;
	
	if (nUnits && MAPUNIT2CH.Lookup(nUnits, cUnits))
	{
		CString sTemp(sTime);
		sTime.Format(_T("%s %c"), sTemp, cUnits);
	}
	
	return sTime;
}

void CTimeHelper::SetUnits(TH_UNITS nUnits, TCHAR cUnits)
{
	// sanity check
	if (!IsValidUnit(nUnits))
	{	
		ASSERT(0);
		return ;
	}

	MAPUNIT2CH[nUnits] = cUnits;
}

void CTimeHelper::SetUnits(TH_UNITS nUnits, LPCTSTR szUnits)
{
	// sanity check
	if (!IsValidUnit(nUnits))
	{	
		ASSERT(0);
		return ;
	}

	if (!Misc::IsEmpty(szUnits))
		SetUnits(nUnits, szUnits[0]);
}

TCHAR CTimeHelper::GetUnits(TH_UNITS nUnits)
{
	// sanity check
	if (!IsValidUnit(nUnits))
	{	
		ASSERT(0);
		return 0;
	}

	// handle first time
	if (MAPUNIT2CH.GetCount() == 0)
	{
		SetUnits(THU_MINS,		'm');	
		SetUnits(THU_HOURS,		'H');	
		SetUnits(THU_WEEKDAYS,	'K');	
		SetUnits(THU_DAYS,		'D');	
		SetUnits(THU_WEEKS,		'W');	
		SetUnits(THU_MONTHS,	'M');	
		SetUnits(THU_YEARS,		'Y');	
	}

	TCHAR cUnits = 0;
	MAPUNIT2CH.Lookup(nUnits, cUnits);

	return cUnits;
}

double CTimeHelper::GetHoursInOneDay(BOOL bStatic) const 
{ 
	return (bStatic ? HOURS2WORKDAYS : m_dHours2Workdays); 
}

double CTimeHelper::GetWeekdaysInOneWeek(BOOL bStatic) const 
{ 
	return (bStatic ? WORKDAYS2WEEKS : m_dWorkdays2Weeks); 
}

BOOL CTimeHelper::IsWeekdays(TH_UNITS nUnits)
{
	return (nUnits == THU_WEEKDAYS);
}

double CTimeHelper::GetDaysToWeeksFactor(TH_UNITS nUnits) const
{
	// Give preference to days unless explicitly set to 'weekdays'
	return (IsWeekdays(nUnits) ? m_dWorkdays2Weeks : DAYS2WEEKS);
}

TH_UNITS CTimeHelper::GetDaysToWeeksUnits(TH_UNITS nUnits) const
{
	// Give preference to days unless explicitly set to 'weekdays'
	return (IsWeekdays(nUnits) ? THU_WEEKDAYS : THU_DAYS);
}

CString CTimeHelper::FormatTimeHMS(double dTime, TH_UNITS nUnitsFrom, BOOL bDecPlaces, BOOL bAllowZero) const
{
	// sanity check
	if (!IsValidUnit(nUnitsFrom))
	{	
		ASSERT(0);
		return _T("");
	}

	// handle negative times
	BOOL bNegative = (dTime < 0.0);

	if (bNegative)
		dTime = -dTime;

	// convert the time to minutes 
	double dMins = GetTime(dTime, nUnitsFrom, THU_MINS);
	
	// and all the others up to years
	double dHours = (dMins / MINS2HOURS);
	double dDays = (dHours / m_dHours2Workdays);
	double dWeeks = (dDays / GetDaysToWeeksFactor(nUnitsFrom));
	double dMonths = (dWeeks / WEEKS2MONTHS);
	double dYears = (dMonths / MONTHS2YEARS);
	
	CString sTime;
	
	if (dYears >= 1.0)
	{
		sTime = FormatTimeHMS(dYears, THU_YEARS, THU_MONTHS, MONTHS2YEARS, bDecPlaces);
	}
	else if (dMonths >= 1.0)
	{
		sTime = FormatTimeHMS(dMonths, THU_MONTHS, THU_WEEKS, WEEKS2MONTHS, bDecPlaces);
	}
	else if (dWeeks >= 1.0)
	{
		sTime = FormatTimeHMS(dWeeks, THU_WEEKS, THU_DAYS, GetDaysToWeeksFactor(nUnitsFrom), bDecPlaces);
	}
	else if (dDays >= 1.0)
	{
		sTime = FormatTimeHMS(dDays, THU_DAYS, THU_HOURS, m_dHours2Workdays, bDecPlaces);
	}
	else if (dHours >= 1.0)
	{
		sTime = FormatTimeHMS(dHours, THU_HOURS, THU_MINS, MINS2HOURS, bDecPlaces);
	}
	else if (dMins >= 1.0)
	{
		sTime = FormatTimeHMS(dMins, THU_MINS, THU_MINS, 0, FALSE);
	}
	else if (bAllowZero)
	{
		sTime = FormatTimeHMS(0.0, nUnitsFrom, THU_NULL, 0, FALSE);
	}

	// handle negative times
	if (bNegative)
		sTime = "-" + sTime;
	
	return sTime;
	
}

CString CTimeHelper::FormatTimeHMS(double dTime, TH_UNITS nUnits, TH_UNITS nLeftOverUnits, 
								   double dLeftOverMultiplier, BOOL bDecPlaces)
{
	// sanity check
	if (!IsValidUnit(nUnits) || !IsValidUnit(nLeftOverUnits) && (nLeftOverUnits != THU_NULL))
	{	
		ASSERT(0);
		return _T("");
	}

	CString sTime;
	
	if (bDecPlaces && (nLeftOverUnits != THU_NULL))
	{
		double dLeftOver = ((dTime - (int)dTime) * dLeftOverMultiplier + FUDGE);

		// omit second element if zero
		if (((int)dLeftOver) == 0)
		{
			sTime.Format(_T("%d%c"), (int)dTime, GetUnits(nUnits));
		}
		else
		{
			sTime.Format(_T("%d%c%d%c"), (int)dTime, GetUnits(nUnits), 
						(int)dLeftOver, GetUnits(nLeftOverUnits));
		}
	}
	else
	{
		sTime.Format(_T("%d%c"), (int)(dTime + 0.5), GetUnits(nUnits));
	}
	
	return sTime;
}

int CTimeHelper::Compare(TH_UNITS nFromUnits, TH_UNITS nToUnits)
{
	// sanity check
	if (!IsValidUnit(nFromUnits) || !IsValidUnit(nToUnits))
	{	
		ASSERT(0);
		return 0;
	}

	if (nFromUnits == nToUnits)
		return 0;

	switch (nFromUnits)
	{
	case THU_MINS:
		return -1; // less than everything else
	
	case THU_HOURS:
		switch(nToUnits)
		{
		case THU_MINS:  
			return 1;
		}
		return -1; // all else
	
	case THU_DAYS:
	case THU_WEEKDAYS:
		switch(nToUnits)
		{
		case THU_MINS:
		case THU_HOURS:
			return 1;

		case THU_DAYS:
		case THU_WEEKDAYS:
			return 0;
		}
		return -1; // all else
	
	case THU_WEEKS:
		switch(nToUnits)
		{
		case THU_MINS:
		case THU_HOURS:
		case THU_DAYS:
		case THU_WEEKDAYS:
			return 1;
		}
		return -1; // all else
	
	case THU_MONTHS:
		switch(nToUnits)
		{
		case THU_MINS:
		case THU_HOURS:
		case THU_DAYS:
		case THU_WEEKDAYS:
		case THU_WEEKS:
			return 1;
		}
		return -1; // all else
	
	case THU_YEARS:
		return 1; // greater than everything else

	default:
		ASSERT(0);
		break;
	}

	// else same
	return 0;
}

// static
BOOL CTimeHelper::SetHoursInWorkday(double dHours)
{
	if (dHours <= 0 || dHours > 24)
		return FALSE;

	HOURS2WORKDAYS = dHours;
	return TRUE;
}

// static
BOOL CTimeHelper::SetWorkdaysInWeek(double dDays)
{
	if (dDays <= 0 || dDays > 7)
		return FALSE;

	WORKDAYS2WEEKS = dDays;
	return TRUE;
}

TH_UNITS CTimeHelper::DecodeUnits(LPCTSTR szUnits)
{
	if (Misc::IsEmpty(szUnits))
	{
		ASSERT(0);
		return THU_NULL;
	}

	return DecodeUnits(szUnits[0]);
}

TH_UNITS CTimeHelper::DecodeUnits(TCHAR cUnits)
{
	POSITION pos = MAPUNIT2CH.GetStartPosition();

	while (pos)
	{
		TH_UNITS nTHUnits;
		TCHAR cValidUnit;

		MAPUNIT2CH.GetNextAssoc(pos, nTHUnits, cValidUnit);
		ASSERT(IsValidUnit(nTHUnits));

		if (cUnits == cValidUnit)
			return nTHUnits;
	}

	ASSERT(0);
	return THU_NULL;
}

BOOL CTimeHelper::DecodeOffset(LPCTSTR szTime, double& dAmount, TH_UNITS& nUnits, BOOL bMustHaveSign)
{
	// sanity checks
	CString sTime(szTime);
	Misc::Trim(sTime);
	
	if (sTime.IsEmpty())
		return FALSE;

	// Sign defaults to +ve
	int nSign = 1;
	TCHAR nFirst = Misc::First(sTime);
	
	switch (nFirst)
	{
	case '+': 
		nSign = 1;  
		Misc::TrimFirst(sTime);
		break;
		
	case '-': 
		nSign = -1; 
		Misc::TrimFirst(sTime);
		break;
		
	default: 
		// Must have sign or be number
		if (bMustHaveSign || ((nFirst < '0') || (nFirst > '9')))				
			return FALSE;
	}

	// Trailing units
	TCHAR nLast = Misc::Last(sTime);
	
	if (IsValidUnit(nLast))
		nUnits = (TH_UNITS)nLast;
	else
		nUnits = THU_HOURS;
	
	// Rest is number (note: ttof ignores any trailing letters)
	dAmount = (nSign * _ttof(sTime));
	return TRUE;
}
