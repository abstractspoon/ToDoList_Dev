// CRecurrence.cpp: implementation of the CRecurrence class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Recurrence.h"

#include "DateHelper.h"
#include "WorkingWeek.h"

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

CRecurrence::CRecurrence(const CRecurrence& tr)
{
	ASSERT(IsValidRegularity(tr.m_nRegularity, tr.m_dwSpecific1, tr.m_dwSpecific2));

	m_nRegularity = tr.m_nRegularity;
	m_dwSpecific1 = tr.m_dwSpecific1;
	m_dwSpecific2 = tr.m_dwSpecific2;
	m_nNumOccur = tr.m_nNumOccur;
	m_nRemainingOccur = tr.m_nRemainingOccur;
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
					(tr.m_nNumOccur		== m_nNumOccur));

	if (bMatch && bIncludeRemainingOccurrences)
	{
		bMatch = (tr.m_nRemainingOccur == m_nRemainingOccur);
	}

	return bMatch;
}

BOOL CRecurrence::CanRecur() const
{
	if (!IsValidRegularity(m_nRegularity, m_dwSpecific1, m_dwSpecific2))
	{
		ASSERT(0);
		return FALSE;
	}

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

BOOL CRecurrence::GetNextOccurence(const COleDateTime& dtPrev, COleDateTime& dtNext)
{
	if (!CalcNextOccurence(dtPrev, dtNext))
		return FALSE;

	// decrement the remaining occurrence count
	if (m_nRemainingOccur > 0)
		m_nRemainingOccur--;

	return TRUE;
}

BOOL CRecurrence::CalcNextOccurence(const COleDateTime& dtPrev, COleDateTime& dtNext) const
{
	if (!CDateHelper::IsDateSet(dtPrev))
		return FALSE;
	
	// This checks:
	// 1. Have we got valid parameters
	// 2. Are we recurring
	// 3. have we got any occurrences left
	if (!CanRecur())
		return FALSE;
	
	COleDateTime dtTemp = dtPrev; // starting point
	CDateHelper dh; // uses static working week
	
	switch (m_nRegularity)		
	{
	case RECURS_DAY_EVERY_NDAYS:
		{
			// add number of days specified by dwSpecific1
			dtTemp.m_dt += (int)m_dwSpecific1;
		}
		break;
		
	case RECURS_DAY_EVERY_NWEEKDAYS:
		{
			// add number of days specified by dwSpecific1
			dh.OffsetDate(dtTemp, (int)m_dwSpecific1, DHU_WEEKDAYS);
		}
		break;
		
	case RECURS_DAY_EVERY_WEEKDAY:
		{
			// add one day ensuring that the result is also a weekday
			dh.OffsetDate(dtTemp, 1, DHU_WEEKDAYS);
		}
		break;
		
	case RECURS_WEEK_SPECIFIC_DOWS_NWEEKS:
	case RECURS_WEEK_EVERY_NWEEKS:
		{
			if (!m_dwSpecific2)
			{
				// if no days have been set we just add 
				// the specified number of weeks
				dh.OffsetDate(dtTemp, (int)m_dwSpecific1, DHU_WEEKS);
			}
			else
			{
				// Get the next valid day
				COleDateTime dtTry = CDateHelper::GetNextAvailableDay(dtTemp, m_dwSpecific2); 

				if ((m_dwSpecific1 == 1) || (dtTry <= CDateHelper::GetEndOfWeek(dtTemp)))
				{
					// The days repeat weekly or
					// The day is still within the current week
					dtTemp = dtTry;
				}
				else
				{
					// Add any weeks greater than one, then increment the day
					dh.OffsetDate(dtTemp, (int)(m_dwSpecific1 - 1), DHU_WEEKS);
					dtTemp.m_dt += 1.0; 
					
					CDateHelper::ValidateDay(dtTemp, m_dwSpecific2);
				}
			}
		}
		break;
		
	case RECURS_MONTH_SPECIFIC_DAY_NMONTHS:
		{
			SYSTEMTIME st;
			dtTemp.GetAsSystemTime(st); // Preserves time component
			
			// Set day first and allow IncrementMonth() to clip it as necessary
			st.wDay = (WORD)m_dwSpecific2;

			// add number of months specified by m_dwSpecific1 
			dh.IncrementMonth(st, (int)m_dwSpecific1);
			
			dtTemp = st;
		}
		break;
		
	case RECURS_MONTH_EVERY_NMONTHS:
		{
			SYSTEMTIME st;
			dtTemp.GetAsSystemTime(st); // Preserves time component
			
			// add number of months specified by m_dwSpecific1 
			dh.IncrementMonth(st, (int)m_dwSpecific1);
			
			dtTemp = st;
		}
		break;
		
	case RECURS_MONTH_SPECIFIC_DOW_NMONTHS:
		{
			// Cache time component
			double dTimeOnly = CDateHelper::GetTimeOnly(dtTemp).m_dt;

			// work out where we are
			int nMonth = dtTemp.GetMonth();
			int nYear = dtTemp.GetYear();
			
			// add number of months specified by dwSpecific2 
			dh.IncrementMonth(nMonth, nYear, (int)m_dwSpecific2);
			
			// calculate next instance
			int nWhich = LOWORD(m_dwSpecific1);
			OLE_DAYOFWEEK nDOW = (OLE_DAYOFWEEK)HIWORD(m_dwSpecific1);

			dtTemp = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
			dtTemp.m_dt += dTimeOnly;
		}
		break;
		
	case RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS:
		{
			SYSTEMTIME st;
			dtTemp.GetAsSystemTime(st); // Preserves time component

			// add number of months specified by dwSpecific2 
			dh.IncrementMonth(st, (int)m_dwSpecific2);
			
			// calculate next instance
			int bFirst = (m_dwSpecific1 == 0);
			st.wDay = (bFirst ? 1 : (WORD)CDateHelper::GetDaysInMonth(st));
			
			dtTemp = st;
			dh.WorkingWeek().MakeWeekday(dtTemp, bFirst);
		}
		break;
		
	case RECURS_YEAR_EVERY_NYEARS:
		{
			// add number of years specified by dwSpecific1
			dh.OffsetDate(dtTemp, (int)m_dwSpecific1, DHU_YEARS);
		}
		break;
		
	case RECURS_YEAR_SPECIFIC_DAY_MONTH:
		{
			SYSTEMTIME st;
			dtTemp.GetAsSystemTime(st); // Preserves time component

			st.wDay = (WORD)m_dwSpecific2;
			st.wMonth = (WORD)m_dwSpecific1;
			st.wYear = (WORD)dtTemp.GetYear();
			
			if (!ValidateDay(st))
				return FALSE;
			
			// see if this year would work before trying next year
			dtTemp = st;
			
			if (dtTemp <= dtPrev)
			{
				// else try restoring the original day and incrementing the year
				st.wDay = (WORD)m_dwSpecific2;
				st.wYear++;
				
				if (!ValidateDay(st))
					return FALSE;
				
				// calculate date
				dtTemp = st;
				ASSERT(dtTemp > dtPrev);
			}
		}
		break;
		
	case RECURS_YEAR_SPECIFIC_DOW_MONTH:
		{
			// Cache time component
			double dTimeOnly = CDateHelper::GetTimeOnly(dtTemp).m_dt;

			int nWhich = LOWORD(m_dwSpecific1);
			OLE_DAYOFWEEK nDOW = (OLE_DAYOFWEEK)HIWORD(m_dwSpecific1);
			int nMonth = m_dwSpecific2;
			
			// see if this year would work before trying next year
			int nYear = dtTemp.GetYear();

			dtTemp = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
			dtTemp.m_dt += dTimeOnly;
			
			// else try incrementing the year
			if (dtTemp <= dtPrev)
			{
				nYear++;
				
				dtTemp = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
				dtTemp.m_dt += dTimeOnly;

				ASSERT(dtTemp > dtPrev);
			}
		}
		break;
		
	default:
		ASSERT(0);
		return FALSE;
	}
		
#if _DEBUG
	// Debug check that we are synced with FitDateToScheme
	switch (m_nRegularity)
	{
	case RECURS_YEAR_SPECIFIC_DOW_MONTH:
	case RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS:
	case RECURS_MONTH_SPECIFIC_DOW_NMONTHS:
		break;

	default:
		{
			COleDateTime dtDebug(dtTemp);

			ASSERT(FitDayToScheme(dtDebug));
			ASSERT(dtDebug == dtTemp);
		}
		break;
	}
#endif

	if (!CDateHelper::IsDateSet(dtTemp))
	{
		ASSERT(0);
		return FALSE;
	}

	dtNext = dtTemp;
	return TRUE;
}
	
int CRecurrence::CalcNextOccurences(const COleDateTime& dtPrev, const COleDateTimeRange& dtRange, CArray<double, double&>& aDates) const
{
	if (!dtRange.IsValid())
	{
		ASSERT(0);
		return 0;
	}

	// Take a copy of ourselves because we need to able
	// to modify the remaining recurrence count
	CRecurrence tr(*this);
	COleDateTime dtNext;

	// Move to start of range
	if (!tr.GetNextOccurence(dtPrev, dtNext))
		return 0;

	aDates.RemoveAll();

	while (dtNext < dtRange.GetStart())
	{
		if (!tr.GetNextOccurence(dtNext, dtNext))
			return 0;
	}

	// Process range itself
	while (dtNext < dtRange.GetEnd())
	{
		aDates.Add(dtNext.m_dt);

		if (!tr.GetNextOccurence(dtNext, dtNext))
			break;
	}

	return aDates.GetSize();
}

BOOL CRecurrence::FitDayToScheme(COleDateTime& dtRecur) const
{
	if (!CDateHelper::IsDateSet(dtRecur))
		return FALSE;
	
	switch (m_nRegularity)
	{
	case RECURS_DAY_EVERY_WEEKDAY:
	case RECURS_DAY_EVERY_NWEEKDAYS:   
		return CWorkingWeek().MakeWeekday(dtRecur);
		
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
			OLE_DAYOFWEEK nDOW = (OLE_DAYOFWEEK)HIWORD(m_dwSpecific1);
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
			return CWorkingWeek().MakeWeekday(dtRecur, bFirst);
		}
		break;
		
	case RECURS_YEAR_SPECIFIC_DOW_MONTH:
		{
			int nWhich = LOWORD(m_dwSpecific1);
			OLE_DAYOFWEEK nDOW = (OLE_DAYOFWEEK)HIWORD(m_dwSpecific1);
			
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
		return FALSE;

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
	case RECURS_ONCE:
		return TRUE;

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
		if (!CDateHelper::IsValidDayOfMonth((OLE_DAYOFWEEK)HIWORD(dwSpec1), LOWORD(dwSpec1), 1))
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
		if (!CDateHelper::IsValidDayOfMonth((OLE_DAYOFWEEK)HIWORD(dwSpec1), LOWORD(dwSpec1), 1))
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
	// Clip date to valid day range for this month
	int nDaysInMonth = CDateHelper::GetDaysInMonth(st.wMonth, st.wYear);
	st.wDay = max(1, min((WORD)nDaysInMonth, (WORD)st.wDay));
	
	return CDateHelper::IsValidDayInMonth(st.wDay, st.wMonth, st.wYear);
}

