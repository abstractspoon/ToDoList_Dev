// CRecurrence.cpp: implementation of the CRecurrence class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Recurrence.h"

#include "DateHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int OCCURS_INFINITELY = -1;	

//////////////////////////////////////////////////////////////////////

//  nRegularity										dwSpecific1				dwSpecific2

//	RECURS_DAILY (RECURS_DAY_EVERY_NDAYS)
//	RECURS_DAY_EVERY_NDAYS							every 'n' days			--- (0)
//	RECURS_DAY_EVERY_WEEKDAY						--- (0)					--- (0)
//	RECURS_DAY_EVERY_NWEEKDAYS						every 'n' days			--- (0)

//	RECURS_WEEKLY	(RECURS_WEEK_SPECIFIC_DOWS_NWEEKS)				
//	RECURS_WEEK_SPECIFIC_DOWS_NWEEKS				every 'n' weeks			weekdays (TDIW_...)
//	RECURS_WEEK_EVERY_NWEEKS						every 'n' weeks			--- (0)

//	RECURS_MONTHLY (RECURS_MONTH_SPECIFIC_DAY_NMONTHS)				
//	RECURS_MONTH_EVERY_NMONTHS						every 'n' months		--- (0)
//	RECURS_MONTH_SPECIFIC_DAY_NMONTHS				every 'n' months		day of month (1-31)
//	RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTH			first(0), last(!0)		every 'n' months
//	RECURS_MONTH_SPECIFIC_DOW_NMONTHS				LOWORD = which (1-5)	every 'n' months
//													HIWORD = DOW (1-7)		

//	RECURS_YEARLY	(RECURS_YEAR_SPECIFIC_DAY_MONTH)				
//	RECURS_YEAR_SPECIFIC_DAY_MONTH					month (1-12)			day of month (1-31)
//	RECURS_YEAR_EVERY_NYEARS						every 'n' years			--- (0)
//  RECURS_YEAR_SPECIFIC_DOW_MONTH					LOWORD = which (1-5)	specific month (1-12)
//													HIWORD = DOW (1-7)		

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecurrence::CRecurrence() 
	: 
	m_nRegularity(RECURS_ONCE), 
	m_dwSpecific1(1), 
	m_dwSpecific2(0), 
	m_nNumOccur(OCCURS_INFINITELY), 
	m_nRemainingOccur(OCCURS_INFINITELY)
{
}

BOOL CRecurrence::operator==(const CRecurrence& tr) const
{
	return Matches(tr, TRUE);
}

BOOL CRecurrence::operator!=(const CRecurrence& tr) const
{
	return !Matches(tr, TRUE);
}

BOOL CRecurrence::Matches(const CRecurrence& tr, BOOL bIncludeRemainingOccurrences) const
{
	BOOL bMatch = ((tr.m_nRegularity	== m_nRegularity) && 
					(tr.m_dwSpecific1	== m_dwSpecific1) &&
					(tr.m_dwSpecific2	== m_dwSpecific2) && 
					(tr.m_nNumOccur	== m_nNumOccur));

	if (bMatch && bIncludeRemainingOccurrences)
	{
		bMatch = (tr.m_nRemainingOccur == m_nRemainingOccur);
	}

	return bMatch;
}

BOOL CRecurrence::CanRecur() const
{
	return (IsRecurring() && ((m_nRemainingOccur > 1) || (m_nNumOccur < 0)));
}

void CRecurrence::ClearOccurrenceCount()
{
	m_nNumOccur = m_nRemainingOccur = OCCURS_INFINITELY;
}

BOOL CRecurrence::IsRecurring() const 
{ 
	return (m_nRegularity != RECURS_ONCE);
}

BOOL CRecurrence::SetOccurrenceCount(int nOccur, int nRemaining)
{
	if (m_nRegularity == RECURS_ONCE)
		return FALSE;
	
	if (((nOccur < 0) || (nRemaining > nOccur)) &&
		!((nOccur == OCCURS_INFINITELY) && (nRemaining == OCCURS_INFINITELY)))
		return FALSE;
	
	m_nNumOccur = nOccur;
	m_nRemainingOccur = nRemaining;
	
	return TRUE;
}

int CRecurrence::GetOccurrenceCount() const
{
	return m_nNumOccur;
}

int CRecurrence::GetRemainingOccurrenceCount() const
{
	return m_nRemainingOccur;
}

BOOL CRecurrence::GetNextOccurence(const COleDateTime& dtFrom, COleDateTime& dtNext)
{
	if (!CDateHelper::IsDateSet(dtFrom))
		return FALSE;
	
	// have we got any occurrences left
	if (!CanRecur())
		return FALSE;
	
	dtNext = dtFrom; // starting point
	
	switch (m_nRegularity)		
	{
	case RECURS_DAY_EVERY_NDAYS:
		{
			if ((int)m_dwSpecific1 <= 0)
				return FALSE;
			
			// add number of days specified by dwSpecific1
			CDateHelper::OffsetDate(dtNext, (int)m_dwSpecific1, DHU_DAYS);
		}
		break;
		
	case RECURS_DAY_EVERY_NWEEKDAYS:
		{
			if ((int)m_dwSpecific1 <= 0)
				return FALSE;
			
			// add number of days specified by dwSpecific1
			CDateHelper::OffsetDate(dtNext, (int)m_dwSpecific1, DHU_WEEKDAYS);
		}
		break;
		
	case RECURS_DAY_EVERY_WEEKDAY:
		{
			// add one day ensuring that the result is also a weekday
			CDateHelper::OffsetDate(dtNext, 1, DHU_WEEKDAYS);
		}
		break;
		
	case RECURS_WEEK_SPECIFIC_DOWS_NWEEKS:
		{
			if (!m_dwSpecific2)
				return FALSE;
		}
		// else fall thru
		
	case RECURS_WEEK_EVERY_NWEEKS:
		{
			if ((int)m_dwSpecific1 <= 0)
				return FALSE;
			
			if (!m_dwSpecific2)
			{
				// if no days have been set we just add 
				// the specified number of weeks
				CDateHelper::OffsetDate(dtNext, (int)m_dwSpecific1, DHU_WEEKS);
			}
			else
			{
				// Look for the next valid day but before the end of the week
				COleDateTime dtTry = CDateHelper::GetNextAvailableDay(dtNext, m_dwSpecific2); 

				if ((dtTry <= CDateHelper::GetEndOfWeek(dtNext)) || (m_dwSpecific1 == 1))
				{
					dtNext = dtTry;
				}
				else
				{
					// Add any weeks greater than one, then increment the day
					CDateHelper::OffsetDate(dtNext, (int)(m_dwSpecific1 - 1), DHU_WEEKS);
					dtNext.m_dt += 1.0; 
					
					CDateHelper::ValidateDay(dtNext, m_dwSpecific2);
				}
			}
		}
		break;
		
	case RECURS_MONTH_SPECIFIC_DAY_NMONTHS:
		{
			if ((m_dwSpecific2 < 1) || (m_dwSpecific2 > 31))
				return FALSE;
		}
		// else fall thru
		
	case RECURS_MONTH_EVERY_NMONTHS:
		{
			if ((int)m_dwSpecific1 <= 0)
				return FALSE;
			
			// add number of months specified by dwSpecific1
			CDateHelper::OffsetDate(dtNext, (int)m_dwSpecific1, DHU_MONTHS);
			
			// then enforce the day
			SYSTEMTIME st;
			dtNext.GetAsSystemTime(st);
			
			if (m_dwSpecific2)
				st.wDay = (WORD)m_dwSpecific2;
			
			if (!ValidateDay(st))
				return FALSE;
			
			dtNext = COleDateTime(st);
		}
		break;
		
	case RECURS_MONTH_SPECIFIC_DOW_NMONTHS:
		{
			int nWhich = LOWORD(m_dwSpecific1);
			int nDOW = HIWORD(m_dwSpecific1);
			int nNumMonths = m_dwSpecific2;
			
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
		
	case RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS:
		{
			int nNumMonths = m_dwSpecific2;
			
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
			int bFirst = (m_dwSpecific1 == 0);
			int nDay = (bFirst ? 1 : CDateHelper::GetDaysInMonth(nMonth, nYear));
			
			dtNext.SetDate(nYear, nMonth, nDay);
			CDateHelper::MakeWeekday(dtNext, bFirst);
		}
		break;
		
	case RECURS_YEAR_EVERY_NYEARS:
		{
			if ((int)m_dwSpecific1 <= 0)
				return FALSE;
			
			// add number of years specified by dwSpecific1
			CDateHelper::OffsetDate(dtNext, (int)m_dwSpecific1, DHU_YEARS);
			
			// clip dates to the end of the month
			SYSTEMTIME st;
			dtNext.GetAsSystemTime(st);
			
			if (!ValidateDay(st))
				return FALSE;
			
			dtNext = COleDateTime(st);
		}
		break;
		
	case RECURS_YEAR_SPECIFIC_DAY_MONTH:
		{
			SYSTEMTIME st = { 0 };
			
			st.wDay = (WORD)m_dwSpecific2;
			st.wMonth = (WORD)m_dwSpecific1;
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
		
	case RECURS_YEAR_SPECIFIC_DOW_MONTH:
		{
			int nWhich = LOWORD(m_dwSpecific1);
			int nDOW = HIWORD(m_dwSpecific1);
			int nMonth = m_dwSpecific2;
			
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
	if (m_nRemainingOccur > 0)
		m_nRemainingOccur--;
	
	return TRUE;
}
	
BOOL CRecurrence::FitDayToScheme(COleDateTime& dtRecur) const
{
	if (!CDateHelper::IsDateSet(dtRecur))
		return FALSE;
	
	switch (m_nRegularity)
	{
	case RECURS_DAY_EVERY_WEEKDAY:
	case RECURS_DAY_EVERY_NWEEKDAYS:   
		return CDateHelper::MakeWeekday(dtRecur);
		
	case RECURS_ONCE:
	case RECURS_DAY_EVERY_NDAYS:  
	case RECURS_MONTH_EVERY_NMONTHS:
		return TRUE; // any day is okay
		
	case RECURS_WEEK_SPECIFIC_DOWS_NWEEKS:  
		if (m_dwSpecific2)
			return CDateHelper::ValidateDay(dtRecur, m_dwSpecific2);
		break;
		
	case RECURS_WEEK_EVERY_NWEEKS:
		if (m_dwSpecific2)
			return CDateHelper::ValidateDay(dtRecur, m_dwSpecific2);
		
		// else
		return TRUE; // can be any day of week
		
	case RECURS_MONTH_SPECIFIC_DOW_NMONTHS:
		{
			int nWhich = LOWORD(m_dwSpecific1);
			int nDOW = HIWORD(m_dwSpecific1);
			int nMonth = dtRecur.GetMonth();
			int nYear = dtRecur.GetYear();
			
			if (CDateHelper::IsValidDayOfMonth(nDOW, nWhich, nMonth))
			{
				dtRecur = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
				return TRUE;
			}
		}
		break;
		
	case RECURS_MONTH_SPECIFIC_DAY_NMONTHS: 
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
		
	case RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS: 
		{
			// work out where we are
			int nMonth = dtRecur.GetMonth();
			int nYear = dtRecur.GetYear();
			
			int bFirst = (m_dwSpecific1 == 0);
			int nDay = (bFirst ? 1 : CDateHelper::GetDaysInMonth(nMonth, nYear));
			
			dtRecur.SetDate(nYear, nMonth, nDay);
			return CDateHelper::MakeWeekday(dtRecur, bFirst);
		}
		break;
		
	case RECURS_YEAR_SPECIFIC_DOW_MONTH:
		{
			int nWhich = LOWORD(m_dwSpecific1);
			int nDOW = HIWORD(m_dwSpecific1);
			
			SYSTEMTIME st;
			dtRecur.GetAsSystemTime(st);
			
			dtRecur = CDateHelper::CalcDate(nDOW, nWhich, st.wMonth, st.wYear);
			return TRUE;
		}
		break;
		
	case RECURS_YEAR_SPECIFIC_DAY_MONTH:  
		{
			SYSTEMTIME st;
			dtRecur.GetAsSystemTime(st);
			
			st.wDay = (WORD)m_dwSpecific2;
			st.wMonth = (WORD)m_dwSpecific1;
			// year can be anything
			
			// clip day to the end of the month
			if (!ValidateDay(st))
				return FALSE;
			
			dtRecur = COleDateTime(st);
			return TRUE;
		}
		break;
		
	case RECURS_YEAR_EVERY_NYEARS:
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
	
BOOL CRecurrence::SetRegularity(RECURRENCE_REGULARITY nReg, DWORD dwSpec1, DWORD dwSpec2)
{
	if (!IsValidRegularity(nReg, dwSpec1, dwSpec2))
	{
//		ASSERT(0);
		return FALSE;
	}

	// All good
	m_nRegularity = nReg;
	m_dwSpecific1 = dwSpec1;
	m_dwSpecific2 = dwSpec2;

	return TRUE;
}

BOOL CRecurrence::IsValidRegularity(RECURRENCE_REGULARITY nReg, DWORD dwSpec1, DWORD dwSpec2)
{
	switch (nReg)
	{
	case RECURS_DAY_EVERY_NDAYS:
	case RECURS_DAY_EVERY_NWEEKDAYS:
		// Every 'n' days/weekdays
		if ((int)dwSpec1 <= 0)
			return FALSE;

		// Must be zero
		if (dwSpec2)
			return FALSE;
		break;
		
	case RECURS_DAY_EVERY_WEEKDAY:
		// Must be zero
		if (dwSpec1 || dwSpec2)
			return FALSE;
		break;
		
	case RECURS_WEEK_SPECIFIC_DOWS_NWEEKS:
		// Every 'n' weeks
		if ((int)dwSpec1 <= 0)
			return FALSE;
		
		// Days of week > 0
		if (!dwSpec2)
			return FALSE;
		break;
		
	case RECURS_WEEK_EVERY_NWEEKS:
		// Every 'n' weeks
		if ((int)dwSpec1 <= 0)
			return FALSE;
		
		// Must be zero
		if (dwSpec2)
			return FALSE;
		break;
		
	case RECURS_MONTH_SPECIFIC_DAY_NMONTHS:
		// Every 'n' months
		if ((int)dwSpec1 <= 0)
			return FALSE;

		// Day of month
		if ((dwSpec2 < 1) || (dwSpec2 > 31))
			return FALSE;
		break;
		
	case RECURS_MONTH_EVERY_NMONTHS:
		// Every 'n' months
		if ((int)dwSpec1 <= 0)
			return FALSE;
		
		// Must be zero
		if (dwSpec2)
			return FALSE;
		break;
		
	case RECURS_MONTH_SPECIFIC_DOW_NMONTHS:
		// Every 'n' months
		if ((int)dwSpec2 <= 0)
			return FALSE;

		// LOWORD = which week (1-5)
		// HIWORD = DOW (1-7)		
		if (!CDateHelper::IsValidDayOfMonth(HIWORD(dwSpec1), LOWORD(dwSpec1), 1))
			return FALSE;
		break;
		
	case RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS:
		// Every 'n' months
		if ((int)dwSpec2 <= 0)
			return FALSE;

		// dwSpec2 can be zero or non-zero
		break;
		
	case RECURS_YEAR_EVERY_NYEARS:
		// Every 'n' years
		if ((int)dwSpec1 <= 0)
			return FALSE;
		
		// Must be zero
		if (dwSpec2)
			return FALSE;
		break;
		
	case RECURS_YEAR_SPECIFIC_DAY_MONTH:
		// Specific month
		if ((dwSpec1 < 1) || (dwSpec1 > 12))
			return FALSE;

		// Day of month
		if ((dwSpec2 < 1) || (dwSpec2 > 31))
			return FALSE;
		break;
		
	case RECURS_YEAR_SPECIFIC_DOW_MONTH:
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

RECURRENCE_REGULARITY CRecurrence::GetRegularity(DWORD& dwSpec1, DWORD& dwSpec2) const
{
	dwSpec1 = m_dwSpecific1;
	dwSpec2 = m_dwSpecific2;

	return m_nRegularity;
}

RECURRENCE_REGULARITY CRecurrence::GetRegularity() const
{
	return m_nRegularity;
}

BOOL CRecurrence::ValidateDay(SYSTEMTIME& st) const
{
	if ((st.wDay == 29) && (st.wMonth == 2))
	{
		int nDaysInMonth = CDateHelper::GetDaysInMonth(st.wMonth, st.wYear);
		st.wDay = min((WORD)m_dwSpecific2, (WORD)nDaysInMonth);
		
		return TRUE;
	}
	
	// else
	return CDateHelper::IsValidDayInMonth(st.wDay, st.wMonth, st.wYear);
}

