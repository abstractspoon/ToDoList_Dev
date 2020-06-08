// TimeHelper.cpp: implementation of the TimeHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeHelper.h"
#include "misc.h"

#include <locale.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const double SECS2MINS	 	= 60.0;
const double MINS2HOURS 	= 60.0;
const double DAYS2WEEKS 	= 7.0;
const double WEEKS2MONTHS 	= 4.348; // 365.25 / (7 * 12)
const double MONTHS2YEARS 	= 12.0;
const double FUDGE 			= 1e-6;

//////////////////////////////////////////////////////////////////////

CMap<TH_UNITS, TH_UNITS, TCHAR, TCHAR&> CTimeHelper::MAPUNIT2CH; // user definable

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeHelper::CTimeHelper() : m_week()
{
}

CTimeHelper::CTimeHelper(const CWorkingWeek& week) : m_week(week)
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

BOOL CTimeHelper::IsValidUnit(TH_UNITS nUnits)
{
	switch (nUnits)
	{
	case THU_SECONDS:
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

double CTimeHelper::Convert(double dTime, TH_UNITS nFromUnits, TH_UNITS nToUnits) const
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
		dTime *= m_week.GetLengthInDaysAsRatio();
	}
	else if ((nFromUnits == THU_WEEKDAYS) && (nToUnits == THU_DAYS))
	{
		dTime /= m_week.GetLengthInDaysAsRatio();
	}
	else if (Compare(nFromUnits, nToUnits) > 0)
	{
		while (Compare(nFromUnits, nToUnits) > 0)
		{
			switch (nFromUnits)
			{
			case THU_MINS:
				dTime *= SECS2MINS;
				nFromUnits = THU_SECONDS;
				break;
				
			case THU_HOURS:
				dTime *= MINS2HOURS;
				nFromUnits = THU_MINS;
				break;
				
			case THU_WEEKDAYS:
			case THU_DAYS:
				dTime *= m_week.WorkDay().GetLengthInHours();
				nFromUnits = THU_HOURS;
				break;

			case THU_WEEKS:
				dTime *= GetDaysToWeeksFactor(nToUnits);
				nFromUnits = GetDaysToWeeksUnits(nToUnits);
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
			case THU_SECONDS:
				dTime /= SECS2MINS;
				nFromUnits = THU_MINS;
				break;

			case THU_MINS:
				dTime /= MINS2HOURS;
				nFromUnits = THU_HOURS;
				break;
				
			case THU_HOURS:
				dTime /= m_week.WorkDay().GetLengthInHours();
				nFromUnits = GetDaysToWeeksUnits(nToUnits);
				break;

			case THU_WEEKDAYS:
				dTime /= m_week.GetLengthInDays();
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
		dTime2 = Convert(dTime2, nUnits2, nUnits1);

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

double CTimeHelper::DecodeClockTime(LPCTSTR szTime, BOOL bIncSeconds)
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
		case 3: 
			if (bIncSeconds)
				dTime += (_ttof(aBits[2]) / 3600);	// seconds
			// fall thru
		case 2: dTime += (_ttof(aBits[1]) / 60);	// minutes
			// fall thru
		case 1: dTime += (_ttof(aBits[0]));			// hours
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
		sTime += ' ';
		sTime += cUnits;
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
		SetUnits(THU_SECONDS,	'S');
		SetUnits(THU_MINS,		'm');	
		SetUnits(THU_HOURS,		'H');	
		SetUnits(THU_WEEKDAYS,	'K');	
		SetUnits(THU_DAYS,		'D');	
		SetUnits(THU_WEEKS,		'W');	
		SetUnits(THU_MONTHS,	'M');	
		SetUnits(THU_YEARS,		'Y');	
	}

	TCHAR cUnits = 0;
	VERIFY(MAPUNIT2CH.Lookup(nUnits, cUnits));

	return cUnits;
}

double CTimeHelper::GetDaysToWeeksFactor(TH_UNITS nUnits) const
{
	// Give preference to weekdays unless explicitly set to 'days'
	return ((nUnits != THU_DAYS) ? m_week.GetLengthInDays() : DAYS2WEEKS);
}

TH_UNITS CTimeHelper::GetDaysToWeeksUnits(TH_UNITS nUnits) const
{
	// Give preference to weekdays unless explicitly set to 'days'
	return ((nUnits != THU_DAYS) ? THU_WEEKDAYS : THU_DAYS);
}

CString CTimeHelper::FormatTimeHMS(double dTime, TH_UNITS nUnitsFrom, DWORD dwFlags) const
{
	// sanity check
	if (!IsValidUnit(nUnitsFrom))
	{	
		ASSERT(0);
		return _T("");
	}

	BOOL bDecPlaces = (dwFlags & HMS_DECIMALPLACES);
	TCHAR cDelim = ((dwFlags & HMS_FORMATSPACED) ? ' ' : 0);

	// handle negative times
	BOOL bNegative = (dTime < 0.0);

	if (bNegative)
		dTime = -dTime;

	// convert the time to minutes 
	double dMins = Convert(dTime, nUnitsFrom, THU_MINS);
	
	// and all the others up to years
	double dHours = (dMins / MINS2HOURS);
	double dDays = (dHours / m_week.WorkDay().GetLengthInHours());
	double dWeeks = (dDays / GetDaysToWeeksFactor(nUnitsFrom));
	double dMonths = (dWeeks / WEEKS2MONTHS);
	double dYears = (dMonths / MONTHS2YEARS);
	
	CString sTime;
	
	if (dYears >= 1.0)
	{
		sTime = FormatTimeHMS(dYears, THU_YEARS, THU_MONTHS, MONTHS2YEARS, bDecPlaces, cDelim);
	}
	else if (dMonths >= 1.0)
	{
		sTime = FormatTimeHMS(dMonths, THU_MONTHS, THU_WEEKS, WEEKS2MONTHS, bDecPlaces, cDelim);
	}
	else if (dWeeks >= 1.0)
	{
		sTime = FormatTimeHMS(dWeeks, THU_WEEKS, THU_DAYS, GetDaysToWeeksFactor(nUnitsFrom), bDecPlaces, cDelim);
	}
	else if (dDays >= 1.0)
	{
		sTime = FormatTimeHMS(dDays, THU_DAYS, THU_HOURS, m_week.WorkDay().GetLengthInHours(), bDecPlaces, cDelim);
	}
	else if (dHours >= 1.0)
	{
		sTime = FormatTimeHMS(dHours, THU_HOURS, THU_MINS, MINS2HOURS, bDecPlaces, cDelim);
	}
	else if (dMins > 0.0)
	{
		if (dwFlags & HMS_WANTSECONDS)
			sTime = FormatTimeHMS(dMins, THU_MINS, THU_SECONDS, SECS2MINS, bDecPlaces, cDelim);
		else
			sTime = FormatTimeHMS(dMins, THU_MINS, THU_MINS, 0, FALSE, 0);
	}
	else if (dwFlags & HMS_ALLOWZERO)
	{
		sTime = FormatTimeHMS(0.0, nUnitsFrom, THU_NULL, 0, FALSE, 0);
	}

	// handle negative times
	if (bNegative)
		sTime = "-" + sTime;
	
	return sTime;
}

CString CTimeHelper::FormatTimeHMS(double dTime, TH_UNITS nUnits, TH_UNITS nLeftOverUnits, 
								   double dLeftOverMultiplier, BOOL bDecPlaces, TCHAR cDelim)
{
	// sanity check
	if (!IsValidUnit(nUnits) || !IsValidUnit(nLeftOverUnits) && (nLeftOverUnits != THU_NULL))
	{	
		ASSERT(0);
		return _T("");
	}

	CString sTime = FormatTimeHMS((int)dTime, nUnits);
	
	if (bDecPlaces && (nLeftOverUnits != THU_NULL))
	{
		double dLeftOver = ((dTime - (int)dTime) * dLeftOverMultiplier + FUDGE);

		// Include second element if at least 1
		if (dLeftOver >= 1.0)
		{
			if (cDelim)
				sTime += cDelim;

			sTime += FormatTimeHMS((int)dLeftOver, nLeftOverUnits);
		}
	}
	
	return sTime;
}

CString CTimeHelper::FormatTimeHMS(int nTime, TH_UNITS nUnits)
{
	CString sTime;

	// sanity check
	if (!IsValidUnit(nUnits))
		ASSERT(0);
	else
		sTime.Format(_T("%d%c"), nTime, GetUnits(nUnits));

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
	case THU_SECONDS:
		return -1; // less than everything else
	
	case THU_MINS:
		switch(nToUnits)
		{
		case THU_SECONDS:  
			return 1;
		}
		return -1; // all else
	
	case THU_HOURS:
		switch(nToUnits)
		{
		case THU_SECONDS:  
		case THU_MINS:  
			return 1;
		}
		return -1; // all else
	
	case THU_DAYS:
	case THU_WEEKDAYS:
		switch(nToUnits)
		{
		case THU_SECONDS:  
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
		case THU_SECONDS:  
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
		case THU_SECONDS:  
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
TH_UNITS CTimeHelper::DecodeUnits(LPCTSTR szValueWithUnits)
{
	if (Misc::IsEmpty(szValueWithUnits))
	{
		ASSERT(0);
		return THU_NULL;
	}

	return DecodeUnits(Misc::Last(szValueWithUnits));
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
	nUnits = CTimeHelper::DecodeUnits(sTime);
	
	if (!IsValidUnit(nUnits))
		nUnits = THU_HOURS;
	
	// Rest is number (note: ttof ignores any trailing letters)
	dAmount = (nSign * _ttof(sTime));
	return TRUE;
}
