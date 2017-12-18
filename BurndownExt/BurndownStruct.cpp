// BurndownWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownExt.h"
#include "BurndownWnd.h"

#include "..\shared\mapex.h"
#include "..\shared\misc.h"
#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\enstring.h"

#include "..\Interfaces\ipreferences.h"

#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

STATSITEM::STATSITEM() 
	: 
	dwTaskID(0), 
	dTimeEst(0.0), 
	nTimeEstUnits(TDCU_DAYS),
	dTimeSpent(0.0),
	nTimeSpentUnits(TDCU_DAYS)
{
	CDateHelper::ClearDate(dtStart);
	CDateHelper::ClearDate(dtDone);
}

STATSITEM::~STATSITEM() 
{
}

BOOL STATSITEM::HasStart() const
{
	return CDateHelper::IsDateSet(dtStart);
}

BOOL STATSITEM::IsDone() const
{
	return CDateHelper::IsDateSet(dtDone);
}

void STATSITEM::MinMax(COleDateTime& dtMin, COleDateTime& dtMax) const
{
	MinMax(dtStart, dtMin, dtMax);
	MinMax(dtDone, dtMin, dtMax);
}

void STATSITEM::MinMax(const COleDateTime& date, COleDateTime& dtMin, COleDateTime& dtMax)
{
	if (CDateHelper::IsDateSet(date))
	{
		if (CDateHelper::IsDateSet(dtMin))
			dtMin = min(dtMin, date);
		else
			dtMin = date;
		
		if (CDateHelper::IsDateSet(dtMax))
			dtMax = max(dtMax, date);
		else
			dtMax = date;
	}
}

double STATSITEM::CalcTimeSpentInDays(const COleDateTime& date, int nDaysInWeek, double dHoursInDay) const
{
	// Ignore tasks with no time spent
	if (dTimeSpent == 0.0)
		return 0.0;

	double dTimeSpentDays = CalcTimeInDays(dTimeSpent, nTimeSpentUnits, nDaysInWeek, dHoursInDay);

	BOOL bHasStart = HasStart();
	double dDays = 0.0;

	if (IsDone())
	{
		if (date >= dtDone)
		{
			dDays += dTimeSpentDays;
		}
		else if (bHasStart && (date > dtStart))
		{
			double dProportion = (date.m_dt - dtStart.m_dt) / (dtDone.m_dt - dtStart.m_dt);
			
			dDays += (dTimeSpentDays * min(dProportion, 1.0));
		}
	}
	else if (bHasStart && (date > dtStart))
	{
		COleDateTime dtNow(COleDateTime::GetCurrentTime());
		double dProportion = (date.m_dt - dtStart.m_dt) / (dtNow.m_dt - dtStart.m_dt);
		
		dDays += (dTimeSpentDays * min(dProportion, 1.0));
	}
	
	return dDays;
}

double STATSITEM::CalcTimeEstimateInDays(int nDaysInWeek, double dHoursInDay) const
{
	return CalcTimeInDays(dTimeEst, nTimeEstUnits, nDaysInWeek, dHoursInDay);
}

double STATSITEM::CalcTimeInDays(double dTime, TDC_UNITS nUnits, int nDaysInWeek, double dHoursInDay)
{
	switch (nUnits)
	{
	case TDCU_WEEKDAYS:
	case TDCU_DAYS:
		return dTime;
	}

	// all the rest
	TH_UNITS nTHUnits = MapUnitsToTHUnits(nUnits);
	CTimeHelper th(dHoursInDay, nDaysInWeek);

	return th.GetTime(dTime, nTHUnits, THU_WEEKDAYS);
}

TH_UNITS STATSITEM::MapUnitsToTHUnits(TDC_UNITS nUnits)
{
	switch (nUnits)
	{
	case TDCU_NULL:		return THU_NULL;
	case TDCU_MINS:		return THU_MINS;
	case TDCU_HOURS:	return THU_HOURS;
	case TDCU_DAYS:		return THU_DAYS;
	case TDCU_WEEKDAYS:	return THU_WEEKDAYS;
	case TDCU_WEEKS:	return THU_WEEKS;
	case TDCU_MONTHS:	return THU_MONTHS;
	case TDCU_YEARS:	return THU_YEARS;
	}

	// all else
	ASSERT(0);
	return THU_NULL;
}

/////////////////////////////////////////////////////////////////////////////

CStatsItemArray::CStatsItemArray()
{

}

CStatsItemArray::~CStatsItemArray()
{
	RemoveAll();
}

STATSITEM* CStatsItemArray::AddItem(DWORD dwTaskID)
{
	if (HasItem(dwTaskID))
	{
		ASSERT(0);
		return NULL;
	}

	STATSITEM* pSI = new STATSITEM();
	pSI->dwTaskID = dwTaskID;

	Add(pSI);
	m_setTaskIDs.Add(dwTaskID);

	return pSI;
}

STATSITEM* CStatsItemArray::operator[](int nIndex) const
{
	return CArray<STATSITEM*, STATSITEM*>::operator[](nIndex);
}

BOOL CStatsItemArray::HasItem(DWORD dwTaskID) const
{
	return m_setTaskIDs.Has(dwTaskID);
}

int CStatsItemArray::FindItem(DWORD dwTaskID) const
{
	if (!HasItem(dwTaskID))
		return -1;

	int nIndex = GetSize();

	while (nIndex--)
	{
		const STATSITEM* pSI = GetAt(nIndex);

		if (pSI->dwTaskID == dwTaskID)
			return nIndex;
	}

	// not found
	ASSERT(0);
	return -1;
}

STATSITEM* CStatsItemArray::GetItem(DWORD dwTaskID) const
{
	int nFind = FindItem(dwTaskID);

	if (nFind == -1)
		return NULL;

	return GetAt(nFind);
}

int CStatsItemArray::GetSize() const
{
	return CArray<STATSITEM*, STATSITEM*>::GetSize();
}

BOOL CStatsItemArray::IsEmpty() const
{
	return (GetSize() == 0);
}

void CStatsItemArray::RemoveAll()
{
	int nIndex = GetSize();

	while (nIndex--)
	{
		STATSITEM* pSI = GetAt(nIndex);
		delete pSI;
	}

	CArray<STATSITEM*, STATSITEM*>::RemoveAll();
	m_setTaskIDs.RemoveAll();
}

void CStatsItemArray::RemoveAt(int nIndex, int nCount)
{
	ASSERT(nIndex >= 0 && nIndex < GetSize());
	ASSERT(nCount > 0);

	nIndex += nCount;

	while (nIndex--)
	{
		STATSITEM* pSI = GetAt(nIndex);

		m_setTaskIDs.RemoveKey(pSI->dwTaskID);
		delete pSI;

		CArray<STATSITEM*, STATSITEM*>::RemoveAt(nIndex);
	}
}

void CStatsItemArray::Sort()
{
	if (!IsSorted())
	{
		qsort(GetData(), GetSize(), sizeof(STATSITEM*), CompareItems);
	}
}

BOOL CStatsItemArray::IsSorted() const
{
	int nNumItems = GetSize();

	if (nNumItems < 2)
		return TRUE;
	
	const STATSITEM* pSI = GetAt(0);
	ASSERT(pSI);

	COleDateTime dtPrev = pSI->dtStart;

	for (int nItem = 1; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = GetAt(nItem);
		ASSERT(pSI);

		if (pSI->HasStart())
		{
			// Stop if the preceding task has no start date
			// OR the preceding task has a later date
			if (!CDateHelper::IsDateSet(dtPrev) || (pSI->dtStart < dtPrev))
			{
				return FALSE;
			}
		}

		dtPrev = pSI->dtStart;
	}

	return TRUE;
}

int CStatsItemArray::CompareItems(const void* pV1, const void* pV2)
{
	typedef STATSITEM* LPSTATSITEM;

	const STATSITEM* pSI1 = *(static_cast<const LPSTATSITEM*>(pV1));
	const STATSITEM* pSI2 = *(static_cast<const LPSTATSITEM*>(pV2));

	ASSERT(pSI1 && pSI2 && (pSI1 != pSI2));

	// Sort by start date
	// Tasks without a start date come last to optimise searching
	BOOL bStart1 = CDateHelper::IsDateSet(pSI1->dtStart);
	BOOL bStart2 = CDateHelper::IsDateSet(pSI2->dtStart);

	if (bStart1 != bStart2)
	{
		return (bStart1 ? -1 : 1);
	}
	else if (!bStart1 && !bStart2)
	{
		return 0;
	}

	if (pSI1->dtStart < pSI2->dtStart)
		return -1;

	if (pSI1->dtStart > pSI2->dtStart)
		return 1;

	return 0;
}

double CStatsItemArray::CalcTimeSpentInDays(const COleDateTime& date, int nDaysInWeek, double dHoursInDay) const
{
	int nNumItems = GetSize();
	double dDays = 0;
	
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = GetAt(nItem);
		
		// We can stop as soon as we pass a task's start date
		if (pSI->HasStart() && (pSI->dtStart >= date))
			break;

		dDays += pSI->CalcTimeSpentInDays(date, nDaysInWeek, dHoursInDay);
	}
	
	return dDays;
}

double CStatsItemArray::CalcTotalTimeEstimateInDays(int nDaysInWeek, double dHoursInDay) const
{
	double dDays = 0;
	int nItem = GetSize();
	
	while (nItem--)
	{
		dDays += GetAt(nItem)->CalcTimeEstimateInDays(nDaysInWeek, dHoursInDay);
	}
	
	return dDays;
}

void CStatsItemArray::GetDataExtents(COleDateTime& dtEarliestDate, COleDateTime& dtLatestDate) const
{
	if (GetSize())
	{
		CDateHelper::ClearDate(dtEarliestDate);
		CDateHelper::ClearDate(dtLatestDate);
		
		int nItem = GetSize();
		
		while (nItem--)
			GetAt(nItem)->MinMax(dtEarliestDate, dtLatestDate);
	}
	else
	{
		dtEarliestDate = dtLatestDate = COleDateTime::GetCurrentTime();
	}
}

int CStatsItemArray::CalculateIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nNextItemFrom) const
{
	int nNumItems = GetSize();
	int nNumNotDone = 0;
	int nEarliestNotDone = -1, nLatestDone = -1;
	
	for (int nItem = nItemFrom; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = GetAt(nItem);
		
		if (pSI->dtStart > date)
			break;
		
		if (!pSI->IsDone() || (pSI->dtDone > date))
		{
			nNumNotDone++;
			
			if ((nEarliestNotDone == -1) && pSI->HasStart())
				nEarliestNotDone = nItem;
		}
		else if (nLatestDone == -1)
		{
			nLatestDone = nItem;
		}
		else if (pSI->dtDone > GetAt(nLatestDone)->dtDone)
		{
			nLatestDone = nItem;
		}
	}
	
	// If the earliest incomplete task in the sequence starts before 
	// the very last completed task then in the next iteration we need 
	// only process tasks beginning with the earliest incomplete task
	// because we have proved that all tasks starting before this
	// task have also been completed
	if ((nEarliestNotDone != -1) && (nLatestDone != -1) && (nEarliestNotDone > nLatestDone))
		nNextItemFrom = nEarliestNotDone;
	
	return nNumNotDone;
}

