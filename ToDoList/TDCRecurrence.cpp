// TdcRecurrence.cpp: implementation of the TdcRecurrence class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TdcRecurrence.h"
#include "resource.h"

#include "..\Shared\DateHelper.h"
#include "..\Shared\enstring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int OCCURS_INFINITELY = -1;	

//////////////////////////////////////////////////////////////////////

//  nRegularity										dwSpecific1				dwSpecific2

//	TDIR_DAILY (TDIR_DAY_EVERY_NDAYS)
//	TDIR_DAY_EVERY_NDAYS							every 'n' days			--- (0)
//	TDIR_DAY_EVERY_WEEKDAY							--- (0)					--- (0)
//	TDIR_DAY_EVERY_NWEEKDAYS						every 'n' days			--- (0)

//	TDIR_WEEKLY	(TDIR_WEEK_SPECIFIC_DOWS_NWEEKS)				
//	TDIR_WEEK_SPECIFIC_DOWS_NWEEKS					every 'n' weeks			weekdays (TDIW_...)
//	TDIR_WEEK_EVERY_NWEEKS							every 'n' weeks			--- (0)

//	TDIR_MONTHLY (TDIR_MONTH_SPECIFIC_DAY_NMONTHS)				
//	TDIR_MONTH_EVERY_NMONTHS						every 'n' months		--- (0)
//	TDIR_MONTH_SPECIFIC_DAY_NMONTHS					every 'n' months		day of month (1-31)
//	TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS				first(0), last(!0)		every 'n' months
//	TDIR_MONTH_SPECIFIC_DOW_NMONTHS					LOWORD = which (1-5)	every 'n' months
//													HIWORD = DOW (1-7)		

//	TDIR_YEARLY	(TDIR_YEAR_SPECIFIC_DAY_MONTH)				
//	TDIR_YEAR_SPECIFIC_DAY_MONTH					month (1-12)			day of month (1-31)
//	TDIR_YEAR_EVERY_NYEARS							every 'n' years			--- (0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TDCRECURRENCE::TDCRECURRENCE() 
	: 
	nRegularity(TDIR_ONCE), 
	dwSpecific1(1), 
	dwSpecific2(0), 
	nRecalcFrom(TDIRO_DUEDATE), 
	nReuse(TDIRO_REUSE),
	nNumOccur(OCCURS_INFINITELY), 
	nRemainingOccur(OCCURS_INFINITELY)
{
}

BOOL TDCRECURRENCE::operator==(const TDCRECURRENCE& tr) const
{
	return Matches(tr, TRUE);
}

BOOL TDCRECURRENCE::operator!=(const TDCRECURRENCE& tr) const
{
	return !Matches(tr, TRUE);
}

BOOL TDCRECURRENCE::Matches(const TDCRECURRENCE& tr, BOOL bIncludeRemainingOccurrences) const
{
	BOOL bMatch = ((tr.nRegularity	== nRegularity) && 
					(tr.dwSpecific1	== dwSpecific1) &&
					(tr.dwSpecific2	== dwSpecific2) && 
					(tr.nRecalcFrom	== nRecalcFrom) &&
					(tr.nReuse		== nReuse) && 
					(tr.nNumOccur	== nNumOccur));

	if (bMatch && bIncludeRemainingOccurrences)
	{
		bMatch = (tr.nRemainingOccur == nRemainingOccur);
	}

	return bMatch;
}

BOOL TDCRECURRENCE::CanRecur() const
{
	return (IsRecurring() && ((nRemainingOccur > 1) || (nNumOccur < 0)));
}

void TDCRECURRENCE::ClearOccurrenceCount()
{
	nNumOccur = nRemainingOccur = OCCURS_INFINITELY;
}

BOOL TDCRECURRENCE::IsRecurring() const 
{ 
	return (nRegularity != TDIR_ONCE);
}

BOOL TDCRECURRENCE::SetOccurrenceCount(int nOccur, int nRemaining)
{
	if (nRegularity == TDIR_ONCE)
		return FALSE;
	
	if (((nOccur < 0) || (nRemaining > nOccur)) &&
		!((nOccur == OCCURS_INFINITELY) && (nRemaining == OCCURS_INFINITELY)))
		return FALSE;
	
	nNumOccur = nOccur;
	nRemainingOccur = nRemaining;
	
	return TRUE;
}

int TDCRECURRENCE::GetOccurrenceCount() const
{
	return nNumOccur;
}

int TDCRECURRENCE::GetRemainingOccurrenceCount() const
{
	return nRemainingOccur;
}

BOOL TDCRECURRENCE::GetNextOccurence(const COleDateTime& dtFrom, COleDateTime& dtNext)
{
	if (!CDateHelper::IsDateSet(dtFrom))
		return FALSE;
	
	// have we got any occurrences left
	if (!CanRecur())
		return FALSE;
	
	dtNext = dtFrom; // starting point
	
	switch (nRegularity)		
	{
	case TDIR_DAY_EVERY_NDAYS:
		{
			if ((int)dwSpecific1 <= 0)
				return FALSE;
			
			// add number of days specified by dwSpecific1
			CDateHelper::OffsetDate(dtNext, (int)dwSpecific1, DHU_DAYS);
		}
		break;
		
	case TDIR_DAY_EVERY_NWEEKDAYS:
		{
			if ((int)dwSpecific1 <= 0)
				return FALSE;
			
			// add number of days specified by dwSpecific1
			CDateHelper::OffsetDate(dtNext, (int)dwSpecific1, DHU_WEEKDAYS);
		}
		break;
		
	case TDIR_DAY_EVERY_WEEKDAY:
		{
			// add one day ensuring that the result is also a weekday
			CDateHelper::OffsetDate(dtNext, 1, DHU_WEEKDAYS);
		}
		break;
		
	case TDIR_WEEK_SPECIFIC_DOWS_NWEEKS:
		{
			if (!dwSpecific2)
				return FALSE;
		}
		// else fall thru
		
	case TDIR_WEEK_EVERY_NWEEKS:
		{
			if ((int)dwSpecific1 <= 0)
				return FALSE;
			
			if (!dwSpecific2)
			{
				// if no days have been set we just add 
				// the specified number of weeks
				CDateHelper::OffsetDate(dtNext, (int)dwSpecific1, DHU_WEEKS);
			}
			else
			{
				// Look for the next valid day but before the end of the week
				COleDateTime dtTry = CDateHelper::GetNextAvailableDay(dtNext, dwSpecific2); 

				if ((dtTry <= CDateHelper::GetEndOfWeek(dtNext)) || (dwSpecific1 == 1))
				{
					dtNext = dtTry;
				}
				else
				{
					// Add any weeks greater than one, then increment the day
					CDateHelper::OffsetDate(dtNext, (int)(dwSpecific1 - 1), DHU_WEEKS);
					dtNext.m_dt += 1.0; 
					
					CDateHelper::ValidateDay(dtNext, dwSpecific2);
				}
			}
		}
		break;
		
	case TDIR_MONTH_SPECIFIC_DAY_NMONTHS:
		{
			if ((dwSpecific2 < 1) || (dwSpecific2 > 31))
				return FALSE;
		}
		// else fall thru
		
	case TDIR_MONTH_EVERY_NMONTHS:
		{
			if ((int)dwSpecific1 <= 0)
				return FALSE;
			
			// add number of months specified by dwSpecific1
			CDateHelper::OffsetDate(dtNext, (int)dwSpecific1, DHU_MONTHS);
			
			// then enforce the day
			SYSTEMTIME st;
			dtNext.GetAsSystemTime(st);
			
			if (dwSpecific2)
				st.wDay = (WORD)dwSpecific2;
			
			if (!ValidateDay(st))
				return FALSE;
			
			dtNext = COleDateTime(st);
		}
		break;
		
	case TDIR_MONTH_SPECIFIC_DOW_NMONTHS:
		{
			int nWhich = LOWORD(dwSpecific1);
			int nDOW = HIWORD(dwSpecific1);
			int nNumMonths = dwSpecific2;
			
			if (!CDateHelper::IsValidDayOfMonth(nDOW, nWhich, 1))
			{
				ASSERT(0);
				return FALSE;
			}
			
			// work out where we are
			int nMonth = dtNext.GetMonth();
			int nYear = dtNext.GetYear();
			
			// increment months
			nMonth += nNumMonths;
			
			while (nMonth > 12)
			{
				nMonth -= 12;
				nYear++;
			}
			
			// calculate next instance
			dtNext = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
		}
		break;
		
	case TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS:
		{
			int nNumMonths = dwSpecific2;
			
			// work out where we are
			int nMonth = dtNext.GetMonth();
			int nYear = dtNext.GetYear();
			
			// increment months
			nMonth += nNumMonths;
			
			while (nMonth > 12)
			{
				nMonth -= 12;
				nYear++;
			}
			
			// calculate next instance
			int bFirst = (dwSpecific1 == 0);
			int nDay = (bFirst ? 1 : CDateHelper::GetDaysInMonth(nMonth, nYear));
			
			dtNext.SetDate(nYear, nMonth, nDay);
			CDateHelper::MakeWeekday(dtNext, bFirst);
		}
		break;
		
	case TDIR_YEAR_EVERY_NYEARS:
		{
			if ((int)dwSpecific1 <= 0)
				return FALSE;
			
			// add number of years specified by dwSpecific1
			CDateHelper::OffsetDate(dtNext, (int)dwSpecific1, DHU_YEARS);
			
			// clip dates to the end of the month
			SYSTEMTIME st;
			dtNext.GetAsSystemTime(st);
			
			if (!ValidateDay(st))
				return FALSE;
			
			dtNext = COleDateTime(st);
		}
		break;
		
	case TDIR_YEAR_SPECIFIC_DAY_MONTH:
		{
			SYSTEMTIME st = { 0 };
			
			st.wDay = (WORD)dwSpecific2;
			st.wMonth = (WORD)dwSpecific1;
			st.wYear = (WORD)dtNext.GetYear();
			
			if (!ValidateDay(st))
				return FALSE;
			
			// see if this year would work before trying next year
			dtNext = COleDateTime(st);
			
			if (dtNext <= dtFrom)
			{
				// else try incrementing the year
				st.wYear++;
				
				if (!ValidateDay(st))
					return FALSE;
				
				// calculate date
				dtNext = COleDateTime(st);
				ASSERT(dtNext > dtFrom);
			}
		}
		break;
		
	case TDIR_YEAR_SPECIFIC_DOW_MONTH:
		{
			int nWhich = LOWORD(dwSpecific1);
			int nDOW = HIWORD(dwSpecific1);
			int nMonth = dwSpecific2;
			
			// Sanity check
			if (!CDateHelper::IsValidDayOfMonth(nDOW, nWhich, nMonth))
			{
				ASSERT(0);
				return FALSE;
			}
			
			// see if this year would work before trying next year
			int nYear = dtNext.GetYear();
			dtNext = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
			
			// else try incrementing the year
			if (dtNext <= dtFrom)
			{
				nYear++;
				
				dtNext = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
				ASSERT(dtNext > dtFrom);
			}
		}
		break;
		
	default:
		ASSERT(0);
		return FALSE;
	}
		
#if _DEBUG
	// Debug check that we are synced with FitDateToScheme
	COleDateTime dtDebug(dtNext);
	
	ASSERT(FitDayToScheme(dtDebug));
	ASSERT(dtDebug == dtNext);
#endif
	
	// decrement the remaining occurrence count
	if (nRemainingOccur > 0)
		nRemainingOccur--;
	
	return TRUE;
}
	
BOOL TDCRECURRENCE::FitDayToScheme(COleDateTime& dtRecur) const
{
	if (!CDateHelper::IsDateSet(dtRecur))
		return FALSE;
	
	switch (nRegularity)
	{
	case TDIR_DAY_EVERY_WEEKDAY:
	case TDIR_DAY_EVERY_NWEEKDAYS:   
		return CDateHelper::MakeWeekday(dtRecur);
		
	case TDIR_ONCE:
	case TDIR_DAY_EVERY_NDAYS:  
	case TDIR_MONTH_EVERY_NMONTHS:
		return TRUE; // any day is okay
		
	case TDIR_WEEK_SPECIFIC_DOWS_NWEEKS:  
		if (dwSpecific2)
			return CDateHelper::ValidateDay(dtRecur, dwSpecific2);
		break;
		
	case TDIR_WEEK_EVERY_NWEEKS:
		if (dwSpecific2)
			return CDateHelper::ValidateDay(dtRecur, dwSpecific2);
		
		// else
		return TRUE; // can be any day of week
		
	case TDIR_MONTH_SPECIFIC_DOW_NMONTHS:
		{
			int nWhich = LOWORD(dwSpecific1);
			int nDOW = HIWORD(dwSpecific1);
			int nMonth = dtRecur.GetMonth();
			int nYear = dtRecur.GetYear();
			
			if (CDateHelper::IsValidDayOfMonth(nDOW, nWhich, nMonth))
			{
				dtRecur = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
				return TRUE;
			}
		}
		break;
		
	case TDIR_MONTH_SPECIFIC_DAY_NMONTHS: 
		{
			// clip day to the end of the month
			SYSTEMTIME st;
			dtRecur.GetAsSystemTime(st);
			
			if (!ValidateDay(st))
				return FALSE;
			
			dtRecur = COleDateTime(st);
			return TRUE;
		}
		break;
		
	case TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS: 
		{
			// work out where we are
			int nMonth = dtRecur.GetMonth();
			int nYear = dtRecur.GetYear();
			
			int bFirst = (dwSpecific1 == 0);
			int nDay = (bFirst ? 1 : CDateHelper::GetDaysInMonth(nMonth, nYear));
			
			dtRecur.SetDate(nYear, nMonth, nDay);
			return CDateHelper::MakeWeekday(dtRecur, bFirst);
		}
		break;
		
	case TDIR_YEAR_SPECIFIC_DOW_MONTH:
		{
			int nWhich = LOWORD(dwSpecific1);
			int nDOW = HIWORD(dwSpecific1);
			
			SYSTEMTIME st;
			dtRecur.GetAsSystemTime(st);
			
			dtRecur = CDateHelper::CalcDate(nDOW, nWhich, st.wMonth, st.wYear);
			return TRUE;
		}
		break;
		
	case TDIR_YEAR_SPECIFIC_DAY_MONTH:  
		{
			SYSTEMTIME st;
			dtRecur.GetAsSystemTime(st);
			
			st.wDay = (WORD)dwSpecific2;
			st.wMonth = (WORD)dwSpecific1;
			// year can be anything
			
			// clip day to the end of the month
			if (!ValidateDay(st))
				return FALSE;
			
			dtRecur = COleDateTime(st);
			return TRUE;
		}
		break;
		
	case TDIR_YEAR_EVERY_NYEARS:
		// clip day to the end of the month
		{
			SYSTEMTIME st;
			dtRecur.GetAsSystemTime(st);
			
			if (!ValidateDay(st))
				return FALSE;
			
			dtRecur = COleDateTime(st);
			return TRUE;
		}
		break;
		
	default: 
		// unknown or deprecated options
		ASSERT(0);
		return FALSE;
	}
		
	// Something bad happened
	ASSERT(0);
	return FALSE;
}
	
CString TDCRECURRENCE::GetRegularityText(BOOL bIncOnce) const
{
	return GetRegularityText(nRegularity, bIncOnce);
}

CString TDCRECURRENCE::GetRegularityText(TDI_REGULARITY nRegularity, BOOL bIncOnce)
{
	enum { ONCE, DAILY, WEEKLY, MONTHLY, YEARLY, COUNT };
	
	static CEnString sRegularity[COUNT];
	
	switch (nRegularity)
	{
	case TDIR_DAY_EVERY_WEEKDAY:
	case TDIR_DAY_EVERY_NDAYS:   
	case TDIR_DAY_EVERY_NWEEKDAYS:   
		if (sRegularity[DAILY].IsEmpty())
			sRegularity[DAILY].LoadString(IDS_DAILY);    
		
		return sRegularity[DAILY];
		
	case TDIR_WEEK_EVERY_NWEEKS:
	case TDIR_WEEK_SPECIFIC_DOWS_NWEEKS:  
		if (sRegularity[WEEKLY].IsEmpty())
			sRegularity[WEEKLY].LoadString(IDS_WEEKLY);   
		
		return sRegularity[WEEKLY];
		
	case TDIR_MONTH_SPECIFIC_DOW_NMONTHS:
	case TDIR_MONTH_EVERY_NMONTHS:
	case TDIR_MONTH_SPECIFIC_DAY_NMONTHS: 
	case TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS: 
		if (sRegularity[MONTHLY].IsEmpty())
			sRegularity[MONTHLY].LoadString(IDS_MONTHLY);  
		
		return sRegularity[MONTHLY];
		
	case TDIR_YEAR_SPECIFIC_DOW_MONTH:
	case TDIR_YEAR_EVERY_NYEARS:
	case TDIR_YEAR_SPECIFIC_DAY_MONTH:  
		if (sRegularity[YEARLY].IsEmpty())
			sRegularity[YEARLY].LoadString(IDS_YEARLY);   
		
		return sRegularity[YEARLY];
		
	case TDIR_ONCE:
		if (bIncOnce)
		{
			if (sRegularity[ONCE].IsEmpty())
				sRegularity[ONCE].LoadString(IDS_ONCEONLY); 
			
			return sRegularity[ONCE];
		}
		break;
		
	default: 
		// unknown or deprecated options
		ASSERT(0);
		break;
	}
	
	return _T("");
}

BOOL TDCRECURRENCE::SetRegularity(TDI_REGULARITY nReg, DWORD dwSpec1, DWORD dwSpec2)
{
	if (!IsValidRegularity(nReg, dwSpec1, dwSpec2))
	{
//		ASSERT(0);
		return FALSE;
	}

	// All good
	nRegularity = nReg;
	dwSpecific1 = dwSpec1;
	dwSpecific2 = dwSpec2;

	return TRUE;
}

BOOL TDCRECURRENCE::IsValidRegularity(TDI_REGULARITY nReg, DWORD dwSpec1, DWORD dwSpec2)
{
	switch (nReg)
	{
	case TDIR_DAY_EVERY_NDAYS:
	case TDIR_DAY_EVERY_NWEEKDAYS:
		// Every 'n' days/weekdays
		if ((int)dwSpec1 <= 0)
			return FALSE;

		// Must be zero
		if (dwSpec2)
			return FALSE;
		break;
		
	case TDIR_DAY_EVERY_WEEKDAY:
		// Must be zero
		if (dwSpec1 || dwSpec2)
			return FALSE;
		break;
		
	case TDIR_WEEK_SPECIFIC_DOWS_NWEEKS:
		// Every 'n' weeks
		if ((int)dwSpec1 <= 0)
			return FALSE;
		
		// Days of week > 0
		if (!dwSpec2)
			return FALSE;
		break;
		
	case TDIR_WEEK_EVERY_NWEEKS:
		// Every 'n' weeks
		if ((int)dwSpec1 <= 0)
			return FALSE;
		
		// Must be zero
		if (dwSpec2)
			return FALSE;
		break;
		
	case TDIR_MONTH_SPECIFIC_DAY_NMONTHS:
		// Every 'n' months
		if ((int)dwSpec1 <= 0)
			return FALSE;

		// Day of month
		if ((dwSpec2 < 1) || (dwSpec2 > 31))
			return FALSE;
		break;
		
	case TDIR_MONTH_EVERY_NMONTHS:
		// Every 'n' months
		if ((int)dwSpec1 <= 0)
			return FALSE;
		
		// Must be zero
		if (dwSpec2)
			return FALSE;
		break;
		
	case TDIR_MONTH_SPECIFIC_DOW_NMONTHS:
		// Every 'n' months
		if ((int)dwSpec2 <= 0)
			return FALSE;

		// LOWORD = which week (1-5)
		// HIWORD = DOW (1-7)		
		if (!CDateHelper::IsValidDayOfMonth(HIWORD(dwSpec1), LOWORD(dwSpec1), 1))
			return FALSE;
		break;
		
	case TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS:
		// Every 'n' months
		if ((int)dwSpec2 <= 0)
			return FALSE;

		// dwSpec2 can be zero or non-zero
		break;
		
	case TDIR_YEAR_EVERY_NYEARS:
		// Every 'n' years
		if ((int)dwSpec1 <= 0)
			return FALSE;
		
		// Must be zero
		if (dwSpec2)
			return FALSE;
		break;
		
	case TDIR_YEAR_SPECIFIC_DAY_MONTH:
		// Specific month
		if ((dwSpec1 < 1) || (dwSpec1 > 12))
			return FALSE;

		// Day of month
		if ((dwSpec2 < 1) || (dwSpec2 > 31))
			return FALSE;
		break;
		
	case TDIR_YEAR_SPECIFIC_DOW_MONTH:
		// Specific month
		if ((dwSpec2 < 1) || (dwSpec2 > 12))
			return FALSE;
		
		// LOWORD = which week (1-5)
		// HIWORD = DOW (1-7)		
		if (!CDateHelper::IsValidDayOfMonth(HIWORD(dwSpec1), LOWORD(dwSpec1), 1))
			return FALSE;
		break;
		
	default:
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

TDI_REGULARITY TDCRECURRENCE::GetRegularity(DWORD& dwSpec1, DWORD& dwSpec2) const
{
	dwSpec1 = dwSpecific1;
	dwSpec2 = dwSpecific2;

	return nRegularity;
}

TDI_REGULARITY TDCRECURRENCE::GetRegularity() const
{
	return nRegularity;
}

BOOL TDCRECURRENCE::ValidateDay(SYSTEMTIME& st) const
{
	if ((st.wDay == 29) && (st.wMonth == 2))
	{
		int nDaysInMonth = CDateHelper::GetDaysInMonth(st.wMonth, st.wYear);
		st.wDay = min((WORD)dwSpecific2, (WORD)nDaysInMonth);
		
		return TRUE;
	}
	
	// else
	return CDateHelper::IsValidDayInMonth(st.wDay, st.wMonth, st.wYear);
}

