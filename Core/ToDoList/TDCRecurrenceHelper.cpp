// ToDoCtrlData.cpp: implementation of the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCRecurrenceHelper.h"
#include "ToDoitem.h"
#include "tdcmapping.h"
#include "tdcstatic.h"

#include "..\shared\DateHelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CTDCRecurrenceHelper::CTDCRecurrenceHelper(const TODOITEM& tdi) : m_tdi(tdi)
{ 
}

BOOL CTDCRecurrenceHelper::CalcNextOccurrence(COleDateTimeRange& dtOccur) const
{
	COleDateTime dtNext;
	BOOL bDue = FALSE;

	if (!CalcNextOccurrence(dtNext, bDue))
		return FALSE;

	if (CalcNextOccurrence(dtNext, dtOccur))
		return TRUE;

	ASSERT(!m_tdi.HasStart() || !m_tdi.HasDue());

	if (bDue)
		dtOccur.m_dtEnd = dtNext;
	else
		dtOccur.m_dtStart = dtNext;

	return TRUE;
}

BOOL CTDCRecurrenceHelper::CalcNextOccurrence(COleDateTime& dtNext, BOOL& bDue) const
{
	ASSERT(m_tdi.IsDone());

	if (!m_tdi.trRecurrence.CanRecur() || !m_tdi.IsDone())
		return FALSE;

	BOOL bHasDue = m_tdi.HasDue();
	BOOL bHasStart = m_tdi.HasStart();

	switch (m_tdi.trRecurrence.nRecalcFrom)
	{
	case TDIRO_STARTDATE:
		if (bHasStart)
		{
			bDue = FALSE;
			return m_tdi.trRecurrence.CalcNextOccurence(m_tdi.dateStart, dtNext);
		}
		// fall thru

	case TDIRO_DUEDATE:
		if (bHasDue)
		{
			bDue = TRUE;
			return m_tdi.trRecurrence.CalcNextOccurence(m_tdi.dateDue, dtNext);
		}
		// fall thru

	case TDIRO_DONEDATE:
	default:
		bDue = (bHasDue || !bHasStart);
		break;
	}

	COleDateTime dtStartDue = (bDue ? m_tdi.dateDue : m_tdi.dateStart);
	COleDateTime dtFrom = m_tdi.dateDone;

	if (CDateHelper::IsSameDay(m_tdi.dateDone, dtStartDue) || (m_tdi.dateDone < dtStartDue))
	{
		// Nothing to do
	}
	else 
	{
		// completed date comes after start/due date
		ASSERT(m_tdi.dateDone > dtStartDue);

		// Special case:
		//
		// 1. Weekly occurrence
		// 2. > 1 week interval
		// 3. Completion date not in the same week as due date
		DWORD dwNumWeeks, dwNotUsed;
		TDC_REGULARITY nReg = m_tdi.trRecurrence.GetRegularity(dwNumWeeks, dwNotUsed);

		if ((nReg == TDIR_WEEK_SPECIFIC_DOWS_NWEEKS) && 
			(dwNumWeeks > 1) &&
			!CDateHelper::IsSameWeek(dtStartDue, m_tdi.dateDone))
		{
			// Move the date to the end of the week to prevent 
			// the base class implementation from selecting the
			// next available day in the current week
			dtFrom = CDateHelper::GetEndOfWeek(m_tdi.dateDone);
		}
	}
	ASSERT(CDateHelper::IsDateSet(dtFrom));

	if (!m_tdi.trRecurrence.CalcNextOccurence(dtFrom, dtNext))
		return FALSE;

	// Restore the previous due/start time
	if (bHasDue || bHasStart)
		dtNext = CDateHelper::MakeDate(dtNext, dtStartDue);
	else
		dtNext = CDateHelper::GetDateOnly(dtNext); // End of day

	return TRUE;
}

int CTDCRecurrenceHelper::CalcNextOccurrences(const COleDateTimeRange& dtRange, CArray<COleDateTimeRange, COleDateTimeRange&>& aOccur) const
{
	ASSERT(!m_tdi.IsDone() && !m_tdi.bLocked);

	if (!m_tdi.CanRecur())
		return FALSE;

	if (!m_tdi.HasStart() || !m_tdi.HasDue() || (m_tdi.dateDue < m_tdi.dateStart))
		return 0;

	CArray<double, double&> aDates;
	int nNumOccur = CalcNextOccurrences(dtRange, aDates);

	if (!nNumOccur)
		return 0;

	// Convert the individual dates into start->due date ranges
	CDateHelper dh;
	aOccur.SetSize(nNumOccur);

	for (int nOccur = 0; nOccur < nNumOccur; nOccur++)
	{
		VERIFY(CalcNextOccurrence(aDates[nOccur], aOccur[nOccur]));
	}

	return nNumOccur;
}

int CTDCRecurrenceHelper::CalcNextOccurrences(const COleDateTimeRange& dtRange, CArray<double, double&>& aDates) const
{
	COleDateTime dtFrom;

	if (!GetFromDate(dtFrom))
		return 0;

	// Expand the range by task duration to ensure full coverage
	COleDateTimeRange dtExtended(dtRange);
	dtExtended.Expand((int)(m_tdi.dateDue.m_dt - m_tdi.dateStart.m_dt), DHU_DAYS);

#ifdef _DEBUG
	CString sRange = dtRange.Format();
	CString sExtRange = dtExtended.Format();
#endif

	return m_tdi.trRecurrence.CalcNextOccurrences(dtFrom, dtExtended, aDates);
}

BOOL CTDCRecurrenceHelper::CalcNextOccurrence(const COleDateTime& dtNext, COleDateTimeRange& dtOccur) const
{
	ASSERT(!m_tdi.IsDone() && !m_tdi.bLocked);
	ASSERT(m_tdi.CanRecur());
	ASSERT(m_tdi.HasStart() && m_tdi.HasDue() && (m_tdi.dateDue >= m_tdi.dateStart));

	int nDaysOffset = CalcDaysToNextOccurrence(dtNext);

	switch (m_tdi.trRecurrence.nRecalcFrom)
	{
	case TDIRO_STARTDATE:
		{
			COleDateTime dtNewDue = m_tdi.dateDue;
			VERIFY(CDateHelper().OffsetDate(dtNewDue, nDaysOffset, DHU_DAYS));

			ASSERT((dtNext <= dtNewDue) ||
				(CDateHelper::IsSameDay(dtNext, dtNewDue) && !CDateHelper::DateHasTime(dtNewDue)));

			VERIFY(dtOccur.Set(dtNext, dtNewDue));
		}
		break;

	case TDIRO_DUEDATE:
	case TDIRO_DONEDATE:
		{
			COleDateTime dtNewStart = m_tdi.dateStart;

			if (nDaysOffset)
				VERIFY(CDateHelper().OffsetDate(dtNewStart, nDaysOffset, DHU_DAYS));

			ASSERT((dtNewStart <= dtNext) ||
				(CDateHelper::IsSameDay(dtNext, dtNewStart) && !CDateHelper::DateHasTime(dtNext)));

			VERIFY(dtOccur.Set(dtNewStart, dtNext));
		}
		break;
	}

	return dtOccur.IsValid();
}

int CTDCRecurrenceHelper::CalcDaysToNextOccurrence(const COleDateTime& dtNext) const
{
	COleDateTime dtFrom;

	if (!GetFromDate(dtFrom))
		return 0;

#ifdef _DEBUG
		CString sTo = dtNext.Format();
		CString sFrom = dtFrom.Format();
#endif

	int nDaysOffset = (int)Misc::Round(dtNext.m_dt - dtFrom.m_dt, 4);

	// If a task's duration is one more precise months the 
	// number of days offset need special handling
	double dDurationInMonths = CDateHelper().CalcDuration(m_tdi.dateStart, m_tdi.dateDue, DHU_MONTHS, TRUE);
	BOOL bExactMonths = (dDurationInMonths == (int)dDurationInMonths);

	switch (m_tdi.trRecurrence.nRecalcFrom)
	{
	case TDIRO_STARTDATE:
		{
			if (bExactMonths)
			{
				COleDateTime dtNewDue = dtNext;
				CDateHelper::IncrementMonth(dtNewDue, (int)dDurationInMonths, TRUE);
#ifdef _DEBUG
				CString sNewDue = dtNewDue.Format();
#endif 
				nDaysOffset = (int)Misc::Round(dtNewDue - m_tdi.dateDue, 4);

				if (!CDateHelper::DateHasTime(dtNext))
					nDaysOffset--; // we want the day before
			}
		}
		break;

	case TDIRO_DUEDATE:
		{
			if (bExactMonths)
			{
				COleDateTime dtNewStart = dtNext;
				CDateHelper::IncrementMonth(dtNewStart, -(int)dDurationInMonths, TRUE);
#ifdef _DEBUG
				CString sNewStart = dtNewStart.Format();
#endif
				nDaysOffset = (int)Misc::Round(dtNewStart - m_tdi.dateStart, 4);

				if (!CDateHelper::DateHasTime(dtNext))
					nDaysOffset++; // we want the day after
			}
		}
		break;

	case TDIRO_DONEDATE:
		{
			if (bExactMonths)
			{
				COleDateTime dtNewStart = dtNext;
				CDateHelper::IncrementMonth(dtNewStart, -(int)dDurationInMonths, TRUE);
#ifdef _DEBUG
				CString sNewStart = dtNewStart.Format();
#endif
				nDaysOffset = (int)Misc::Round(dtNewStart - m_tdi.dateStart, 4);

				if (!CDateHelper::DateHasTime(dtNext))
					nDaysOffset++; // we want the day after
			}

			// Offset by the difference between the original due date
			// and the completion (today's) date
			ASSERT(dtFrom.m_dt == CDateHelper::GetDate(DHD_TODAY));

			nDaysOffset += (int)Misc::Round(dtFrom.m_dt - m_tdi.dateDue.m_dt, 4);
		}
		break;
	}

	return nDaysOffset;
}

BOOL CTDCRecurrenceHelper::GetFromDate(COleDateTime& dtFrom) const
{
	switch (m_tdi.trRecurrence.nRecalcFrom)
	{
	case TDIRO_STARTDATE:
		dtFrom = m_tdi.dateStart;
		break;

	case TDIRO_DUEDATE:
		dtFrom = m_tdi.dateDue;
		break;

	case TDIRO_DONEDATE:
		dtFrom = CDateHelper::GetDate(DHD_TODAY);
		break;

	default:
		dtFrom = CDateHelper::NullDate();
		break;
	}

	if (CDateHelper::IsDateSet(dtFrom))
		return TRUE;

	ASSERT(0);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
