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

	return (IsRecurring() && ((m_nRemainingOccur > 1) || (m_nNumOccur == OCCURS_INFINITELY)));
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

BOOL CRecurrence::DecrementRemainingOccurrenceCount()
{
	if (m_nNumOccur == OCCURS_INFINITELY)
		return TRUE;

	// else
	if (m_nRemainingOccur > 0)
	{
		m_nRemainingOccur--;
		return TRUE;
	}

	return FALSE;
}

BOOL CRecurrence::GetNextOccurence(const COleDateTime& dtPrev, COleDateTime& dtNext)
{
	if (!CalcNextOccurence(dtPrev, dtNext))
		return FALSE;

	VERIFY(DecrementRemainingOccurrenceCount());
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

#ifdef _DEBUG
	CString sPrev = dtPrev.Format();
#endif
	
	COleDateTime dtPrevDateOnly = CDateHelper::GetDateOnly(dtPrev);
	COleDateTime dtDateOnly = dtPrevDateOnly; // starting point

	CDateHelper dh; // uses static working week
	
	switch (m_nRegularity)		
	{
	case RECURS_DAY_EVERY_NDAYS:
		{
			// add number of days specified by dwSpecific1
			dtDateOnly.m_dt += (int)m_dwSpecific1;
		}
		break;
		
	case RECURS_DAY_EVERY_NWEEKDAYS:
		{
			// add number of days specified by dwSpecific1
			dh.OffsetDate(dtDateOnly, (int)m_dwSpecific1, DHU_WEEKDAYS);
		}
		break;
		
	case RECURS_DAY_EVERY_WEEKDAY:
		{
			// add one day, ensuring that the result is also a weekday
			dh.OffsetDate(dtDateOnly, 1, DHU_WEEKDAYS);
		}
		break;
		
	case RECURS_WEEK_SPECIFIC_DOWS_NWEEKS:
	case RECURS_WEEK_EVERY_NWEEKS:
		{
			if (!m_dwSpecific2)
			{
				// if no days have been set we just add 
				// the specified number of weeks
				dh.OffsetDate(dtDateOnly, (int)m_dwSpecific1, DHU_WEEKS);
			}
			else
			{
				// Get the next valid day
				COleDateTime dtTry = CDateHelper::GetNextAvailableDay(dtDateOnly, m_dwSpecific2); 

				if ((m_dwSpecific1 == 1) || (dtTry <= CDateHelper::GetEndOfWeek(dtDateOnly)))
				{
					// The days repeat weekly or
					// The day is still within the current week
					dtDateOnly = dtTry;
				}
				else
				{
					// Add any weeks greater than one, then increment the day
					dh.OffsetDate(dtDateOnly, (int)(m_dwSpecific1 - 1), DHU_WEEKS);
					dtDateOnly.m_dt += 1.0; 
					
					CDateHelper::ValidateDay(dtDateOnly, m_dwSpecific2);
				}
			}
		}
		break;
		
	case RECURS_MONTH_SPECIFIC_DAY_NMONTHS:
		{
			// work out where we are
			int nYear, nMonth, nDay;
			CDateHelper::FromDate(dtDateOnly, nDay, nMonth, nYear);

			int nNumMonths = (int)m_dwSpecific1;
			int nDayInMonth = (int)m_dwSpecific2;

			// Work out whether the month of the previous occurrence needs
			// to be evaluated as a possibility. ie. If the previous 
			// occurrence precedes the specified day in that same month.
			{
				int nNumDaysInPrevMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);

				if (nDay < min(nDayInMonth, nNumDaysInPrevMonth))
					nNumMonths--;
			}
			
			dh.IncrementMonth(nMonth, nYear, nNumMonths);

			int nNumDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);
			nDay = min(nDayInMonth, nNumDaysInMonth);

			dtDateOnly = CDateHelper::ToDate(nDay, nMonth, nYear);
		}
		break;
		
	case RECURS_MONTH_EVERY_NMONTHS:
		{
			// add number of years specified by dwSpecific1, preserving end of month
			dh.IncrementMonth(dtDateOnly, (int)m_dwSpecific1, TRUE);
		}
		break;
		
	case RECURS_MONTH_SPECIFIC_DOW_NMONTHS:
		{
			// work out where we are
			int nUnused, nMonth, nYear;
			CDateHelper::FromDate(dtDateOnly, nUnused, nMonth, nYear);
			
			int nNumMonths = (int)m_dwSpecific2;
			int nWhich = LOWORD(m_dwSpecific1);
			OLE_DAYOFWEEK nDOW = (OLE_DAYOFWEEK)HIWORD(m_dwSpecific1);

			// Work out whether the month of the previous occurrence needs
			// to be evaluated as a possibility. ie. If the previous 
			// occurrence precedes the specified day in that same month.
			{
				COleDateTime dt = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);

				if (CDateHelper::Compare(dt, dtPrevDateOnly, 0) > 0)
					nNumMonths--;
			}

			// add adjusted number of months 
			dh.IncrementMonth(nMonth, nYear, nNumMonths);
			
			// calculate next instance
			dtDateOnly = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
		}
		break;
		
	case RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS:
		{
			// work out where we are
			int nDay, nMonth, nYear;
			CDateHelper::FromDate(dtDateOnly, nDay, nMonth, nYear);
			
			int bFirst = (m_dwSpecific1 == 0);
			int nNumMonths = (int)m_dwSpecific2;

			// Work out whether the month of the previous occurrence needs
			// to be evaluated as a possibility. ie. If the previous 
			// occurrence precedes the specified day in that same month.
			{
				nDay = (bFirst ? 1 : 31);
				ValidateDay(nDay, nMonth, nYear);

				COleDateTime dt = CDateHelper::ToDate(nDay, nMonth, nYear);
				dh.WorkingWeek().MakeWeekday(dt, bFirst);

				if (CDateHelper::Compare(dt, dtPrevDateOnly, 0) > 0)
					nNumMonths--;
			}

			dh.IncrementMonth(nMonth, nYear, nNumMonths);

			nDay = (bFirst ? 1 : 31);
			ValidateDay(nDay, nMonth, nYear);
			
			dtDateOnly = CDateHelper::ToDate(nDay, nMonth, nYear);
			dh.WorkingWeek().MakeWeekday(dtDateOnly, bFirst);
		}
		break;
		
	case RECURS_YEAR_EVERY_NYEARS:
		{
			// add number of years specified by dwSpecific1, preserving end of month
			dh.OffsetDate(dtDateOnly, (int)m_dwSpecific1, DHU_YEARS, TRUE);
		}
		break;
		
	case RECURS_YEAR_SPECIFIC_DAY_MONTHS:
		{
			// work out where we are
			int nDay, nMonth, nYear;
			CDateHelper::FromDate(dtDateOnly, nDay, nMonth, nYear);
			
			// Work out whether the month of the previous occurrence needs
			// to be evaluated as a possibility. ie. If the previous 
			// occurrence precedes the specified day in that same month.
			{
				DWORD dwMonth = CDateHelper::MapMonthIndexToDHMonth(nMonth);

				if (m_dwSpecific1 & dwMonth) // Must fit recurring pattern
				{
					nDay = (int)m_dwSpecific2;
					ValidateDay(nDay, nMonth, nYear);

					COleDateTime dt = CDateHelper::ToDate(nDay, nMonth, nYear);

					if (CDateHelper::Compare(dt, dtPrevDateOnly, 0) > 0)
						CDateHelper::IncrementMonth(nMonth, nYear, -1);
				}
			}

			// Now we calculate the next matching month
			while (GetNextSpecificMonth(m_dwSpecific1, nMonth, nYear))
			{
				nDay = (int)m_dwSpecific2;
				ValidateDay(nDay, nMonth, nYear);

				COleDateTime dt = CDateHelper::ToDate(nDay, nMonth, nYear);

				if (CDateHelper::Compare(dt, dtPrevDateOnly, 0) > 0)
				{
					dtDateOnly = dt;
					break;
				}
			}
		}
		break;
		
	case RECURS_YEAR_SPECIFIC_DOW_MONTHS:
		{
			// work out where we are
			int nDay, nMonth, nYear;
			CDateHelper::FromDate(dtDateOnly, nDay, nMonth, nYear);

			int nWhich = LOWORD(m_dwSpecific1);
			OLE_DAYOFWEEK nDOW = (OLE_DAYOFWEEK)HIWORD(m_dwSpecific1);

			// Work out whether the month of the previous occurrence needs
			// to be evaluated as a possibility. ie. If the previous 
			// occurrence precedes the specified day in that same month.
			{
				DWORD dwMonth = CDateHelper::MapMonthIndexToDHMonth(nMonth);

				if (m_dwSpecific2 & dwMonth) // Month must fit recurring pattern
				{
					dtDateOnly = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);

					if (CDateHelper::Compare(dtDateOnly, dtPrevDateOnly, 0) > 0)
						CDateHelper::IncrementMonth(nMonth, nYear, -1);
				}
			}

			// Now we calculate the next matching month
			while (GetNextSpecificMonth(m_dwSpecific2, nMonth, nYear))
			{
				dtDateOnly = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);

				if (CDateHelper::Compare(dtDateOnly, dtPrevDateOnly, 0) > 0)
					break;
			}
		}
		break;
		
	default:
		ASSERT(0);
		return FALSE;
	}
		
#if _DEBUG
	CString sTemp = dtDateOnly.Format();

	// Debug check that we are synced with FitDateToScheme
	switch (m_nRegularity)
	{
	case RECURS_YEAR_SPECIFIC_DOW_MONTHS:
	case RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS:
	case RECURS_MONTH_SPECIFIC_DOW_NMONTHS:
		break;

	default:
		{
			COleDateTime dtDebug(dtDateOnly);

			ASSERT(FitDayToScheme(dtDebug));
			ASSERT(dtDebug == dtDateOnly);
		}
		break;
	}
#endif

	if (!CDateHelper::IsDateSet(dtDateOnly))
	{
		ASSERT(0);
		return FALSE;
	}

	dtNext = CDateHelper::MakeDate(dtDateOnly, dtPrev);
	return TRUE;
}

BOOL CRecurrence::GetNextSpecificMonth(DWORD dwMonths, int& nMonth, int& nYear)
{
	if (!IsValidSpecificMonths(dwMonths))
	{
		ASSERT(0);
		return FALSE;
	}

	DWORD dwMonth = 0;

	do
	{
		CDateHelper::IncrementMonth(nMonth, nYear);
		dwMonth = CDateHelper::MapMonthIndexToDHMonth(nMonth);
	} 
	while ((dwMonths & dwMonth) == 0);

	return TRUE;
}

int CRecurrence::CalcNextOccurrences(const COleDateTime& dtPrev, const COleDateTimeRange& dtRange, CArray<double, double&>& aDates) const
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
		{
			return CDateHelper::ValidateDay(dtRecur, m_dwSpecific2);
		}
		break;
		
	case RECURS_WEEK_EVERY_NWEEKS:
		if (m_dwSpecific2)
		{
			return CDateHelper::ValidateDay(dtRecur, m_dwSpecific2);
		}
		return TRUE; // can be any day of week
		
	case RECURS_MONTH_SPECIFIC_DOW_NMONTHS:
		{
			int nUnused, nMonth, nYear;
			CDateHelper::FromDate(dtRecur, nUnused, nMonth, nYear);

			int nWhich = LOWORD(m_dwSpecific1);
			OLE_DAYOFWEEK nDOW = (OLE_DAYOFWEEK)HIWORD(m_dwSpecific1);

			if (CDateHelper::IsValidDayOfMonth(nDOW, nWhich, nMonth))
			{
				dtRecur = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
				return TRUE;
			}
		}
		break;
		
	case RECURS_MONTH_SPECIFIC_DAY_NMONTHS: 
		return TRUE;
		
	case RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS: 
		{
			int nDay, nMonth, nYear;
			CDateHelper::FromDate(dtRecur, nDay, nMonth, nYear);
			
			int bFirst = (m_dwSpecific1 == 0);
			nDay = (bFirst ? 1 : CDateHelper::GetDaysInMonth(nMonth, nYear));
			
			dtRecur = CDateHelper::ToDate(nDay, nMonth, nYear);
			return CWorkingWeek().MakeWeekday(dtRecur, bFirst);
		}
		break;
		
	case RECURS_YEAR_SPECIFIC_DOW_MONTHS:
		{
			int nDay, nMonth, nYear;
			CDateHelper::FromDate(dtRecur, nDay, nMonth, nYear);
			
			int nWhich = LOWORD(m_dwSpecific1);
			OLE_DAYOFWEEK nDOW = (OLE_DAYOFWEEK)HIWORD(m_dwSpecific1);
			
			dtRecur = CDateHelper::CalcDate(nDOW, nWhich, nMonth, nYear);
			return TRUE;
		}
		break;
		
	case RECURS_YEAR_SPECIFIC_DAY_MONTHS:  
		{
			int nDay, nMonth, nYear;
			CDateHelper::FromDate(dtRecur, nDay, nMonth, nYear);
			
			nDay = (int)m_dwSpecific2;
			ValidateDay(nDay, nMonth, nYear);
			
			dtRecur = CDateHelper::ToDate(nDay, nMonth, nYear);
			return TRUE;
		}
		break;
		
	case RECURS_YEAR_EVERY_NYEARS:
		return TRUE;
		
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

	// Backward compatibility
	switch (nReg)
	{
	case RECURS_YEAR_SPECIFIC_DAY_MONTHS:
		// If the month argument looks like a month index, remap it
		if ((dwSpec1 >= 1) && (dwSpec1 <= 12))
			dwSpec1 = CDateHelper::MapMonthIndexToDHMonth((int)dwSpec1);
		break;

	case RECURS_YEAR_SPECIFIC_DOW_MONTHS:
		// If the month argument looks like a month index, remap it
		if ((dwSpec2 >= 1) && (dwSpec2 <= 12))
			dwSpec2 = CDateHelper::MapMonthIndexToDHMonth((int)dwSpec2);
		break;
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

		// dwSpec2 (Preserve Weekday) can be zero or non-zero
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
		
		// dwSpec2 (Preserve Weekday) can be zero or non-zero
		break;
		
	case RECURS_YEAR_SPECIFIC_DAY_MONTHS:
		// Specific month
		if (!IsValidSpecificMonths(dwSpec1))
			return FALSE;

		// Day of month
		if ((dwSpec2 < 1) || (dwSpec2 > 31))
			return FALSE;
		break;
		
	case RECURS_YEAR_SPECIFIC_DOW_MONTHS:
		// Specific month
		if (!IsValidSpecificMonths(dwSpec2))
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

BOOL CRecurrence::IsValidSpecificMonths(DWORD dwMonths)
{
	if (dwMonths <= 12)
		return (dwMonths > 0);

	if (dwMonths < DHM_JANUARY)
		return FALSE;

	if (dwMonths > DHM_ALL)
		return FALSE;

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

void CRecurrence::ValidateDay(int& nDay, int nMonth, int nYear) const
{
	// Clip date to valid day range for this month
	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);
	nDay = max(1, min(nDay, nDaysInMonth));
}

