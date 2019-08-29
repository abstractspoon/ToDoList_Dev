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

void STATSITEM::Set(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	// Sanity Checks
	ASSERT(!pTasks->IsTaskReference(hTask));
	ASSERT(!pTasks->IsTaskParent(hTask));

	dtDone = GetDoneDate(pTasks, hTask);
	dtStart = GetStartDate(pTasks, hTask);

	// make sure start is less than done
	if (IsDone() && HasStart())
		dtStart = min(dtStart, dtDone);

	dTimeEst = pTasks->GetTaskTimeEstimate(hTask, nTimeEstUnits, false);
	dTimeSpent = pTasks->GetTaskTimeSpent(hTask, nTimeSpentUnits, false);
	dCost = GetCost(pTasks, hTask, bCostIsRate);
}

void STATSITEM::Update(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	// Sanity Checks
	ASSERT(!pTasks->IsTaskReference(hTask));
	ASSERT(!pTasks->IsTaskParent(hTask));

	if (pTasks->IsAttributeAvailable(TDCA_DONEDATE))
		dtDone = GetDoneDate(pTasks, hTask);

	if (pTasks->IsAttributeAvailable(TDCA_STARTDATE))
	{
		dtStart = GetStartDate(pTasks, hTask);

		// make sure start is less than done
		if (IsDone() && HasStart())
			dtStart = min(dtStart, dtDone);
	}

	if (pTasks->IsAttributeAvailable(TDCA_TIMEEST))
		dTimeEst = pTasks->GetTaskTimeEstimate(hTask, nTimeEstUnits, false);

	if (pTasks->IsAttributeAvailable(TDCA_TIMESPENT))
		dTimeSpent = pTasks->GetTaskTimeSpent(hTask, nTimeSpentUnits, false);

	if (pTasks->IsAttributeAvailable(TDCA_COST))
		dCost = GetCost(pTasks, hTask, bCostIsRate);
}

double STATSITEM::GetCost(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL& bIsRate)
{
	LPCTSTR szCost = pTasks->GetTaskAttribute(hTask, TDCA_COST, false);

	if (Misc::IsEmpty(szCost))
	{
		bIsRate = FALSE;
		return 0.0;
	}

	if (szCost[0] == '@')
	{
		bIsRate = TRUE;
		return _tcstod((szCost + 1), NULL);
	}

	// else
	bIsRate = FALSE;
	return _tcstod(szCost, NULL);
}

COleDateTime STATSITEM::GetStartDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	time64_t tDate = 0;
	COleDateTime date;

	if (pTasks->GetTaskStartDate64(hTask, FALSE, tDate))
		date = GetDate(tDate);

	if (!CDateHelper::IsDateSet(date) && pTasks->GetTaskCreationDate64(hTask, tDate))
		date = GetDate(tDate);

	return date;
}

COleDateTime STATSITEM::GetDoneDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	time64_t tDate = 0;
	COleDateTime date;

	if (pTasks->GetTaskDoneDate64(hTask, tDate))
		date = GetDate(tDate);

	return date;
}

COleDateTime STATSITEM::GetDate(time64_t tDate)
{
	return (tDate > 0) ? CDateHelper::GetDate(tDate) : COleDateTime();
}

BOOL STATSITEM::HasStart() const
{
	return CDateHelper::IsDateSet(dtStart);
}

BOOL STATSITEM::IsDone() const
{
	return CDateHelper::IsDateSet(dtDone);
}

void STATSITEM::MinMax(COleDateTimeRange& dtExtents) const
{
	MinMax(dtStart, dtExtents);
	MinMax(dtDone, dtExtents);
}

void STATSITEM::MinMax(const COleDateTime& date, COleDateTimeRange& dtExtents)
{
	if (CDateHelper::IsDateSet(date))
	{
		if (CDateHelper::IsDateSet(dtExtents.m_dtStart))
			dtExtents.m_dtStart = min(dtExtents.m_dtStart, date);
		else
			dtExtents.m_dtStart = date;
		
		if (CDateHelper::IsDateSet(dtExtents.m_dtEnd))
			dtExtents.m_dtEnd = max(dtExtents.m_dtEnd, date);
		else
			dtExtents.m_dtEnd = date;
	}
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

///////////////////////////////////////////////////////////////////////////

CStatsItemCalculator::CStatsItemCalculator(const CStatsItemArray& aItems)
	:
	m_aItems(aItems)
{
}

CStatsItemCalculator::~CStatsItemCalculator()
{

}

void CStatsItemCalculator::GetDataExtents(COleDateTimeRange& dtExtents) const
{
	int nItem = m_aItems.GetSize();

	if (nItem > 0)
	{
		dtExtents.Reset();
		
		while (nItem--)
			m_aItems[nItem]->MinMax(dtExtents);
	}
	else
	{
		dtExtents.Set(DHD_NOW, DHD_NOW);
	}
}

int CStatsItemCalculator::GetIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nNextItemFrom) const
{
	int nNumItems = m_aItems.GetSize();
	int nNumNotDone = 0;
	int nEarliestNotDone = -1, nLatestDone = -1;
	
	for (int nItem = nItemFrom; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_aItems[nItem];
		
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
		else if (pSI->dtDone > m_aItems[nLatestDone]->dtDone)
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

double CStatsItemCalculator::GetTimeEstimateInDays(const COleDateTime& date) const
{
	return GetTotalAttribValue(TIME, ESTIMATE, date);
}

double CStatsItemCalculator::GetTimeSpentInDays(const COleDateTime& date) const
{
	return GetTotalAttribValue(TIME, SPENT, date);
}

double CStatsItemCalculator::GetTotalTimeEstimateInDays() const
{
	return GetTotalAttribValue(TIME, ESTIMATE);
}

double CStatsItemCalculator::GetTotalTimeSpentInDays() const
{
	return GetTotalAttribValue(TIME, SPENT);
}

double CStatsItemCalculator::GetCostSpent(const COleDateTime& date) const
{
	return GetTotalAttribValue(COST, SPENT, date);
}

double CStatsItemCalculator::GetCostEstimate(const COleDateTime& date) const
{
	return GetTotalAttribValue(COST, ESTIMATE, date);
}

double CStatsItemCalculator::GetTotalCostEstimate() const
{
	return GetTotalAttribValue(COST, ESTIMATE);
}

double CStatsItemCalculator::GetTotalCostSpent() const
{
	return GetTotalAttribValue(COST, SPENT);
}

double CStatsItemCalculator::GetTotalAttribValue(ATTRIB nAttrib, ATTRIBTYPE nType) const
{
	double dTotal = 0;
	int nItem = m_aItems.GetSize();

	while (nItem--)
	{
		const STATSITEM* pSI = m_aItems[nItem];

		dTotal += GetAttribValue(*pSI, nAttrib, nType);
	}

	return dTotal;
}

double CStatsItemCalculator::GetTotalAttribValue(ATTRIB nAttrib, ATTRIBTYPE nType, const COleDateTime& date) const
{
	double dTotal = 0;
	int nItem = m_aItems.GetSize();

	while (nItem--)
	{
		const STATSITEM* pSI = m_aItems[nItem];

		dTotal += GetAttribValue(*pSI, nAttrib, nType, date);
	}

	return dTotal;
}

double CStatsItemCalculator::GetAttribValue(const STATSITEM& si, ATTRIB nAttrib, ATTRIBTYPE nType) const
{
	switch (nAttrib)
	{
	case TIME:
		{
			switch (nType)
			{
			case ESTIMATE:
				return GetTimeInDays(si.dTimeEst, si.nTimeEstUnits);

			case SPENT:
				return GetTimeInDays(si.dTimeSpent, si.nTimeSpentUnits);
			}
		}
		break;


	case COST:
		{
			switch (nType)
			{
			case ESTIMATE:
				return (si.bCostIsRate ? (si.dTimeEst * si.dCost) : si.dCost);

			case SPENT:
				return (si.bCostIsRate ? (si.dTimeSpent * si.dCost) : si.dCost);
			}
		}
		break;
	}

	// all else
	ASSERT(0);
	return 0.0;
}

double CStatsItemCalculator::GetAttribValue(const STATSITEM& si, ATTRIB nAttrib, ATTRIBTYPE nType, const COleDateTime& date) const
{
	double dValue = GetAttribValue(si, nAttrib, nType);

	return CalcProportionOfValue(si, dValue, date);
}

double CStatsItemCalculator::CalcProportionOfValue(const STATSITEM& si, double dDays, const COleDateTime& date) const
{
	// Ignore tasks with no time spent
	if (dDays == 0.0)
		return 0.0;

	BOOL bHasStart = si.HasStart();
	double dProportion = 0.0;

	if (si.IsDone())
	{
		if (date >= si.dtDone)
		{
			dProportion = 1.0;
		}
		else if (bHasStart && (date > si.dtStart))
		{
			dProportion = (date.m_dt - si.dtStart.m_dt) / (si.dtDone.m_dt - si.dtStart.m_dt);
		}
	}
	else if (bHasStart && (date > si.dtStart))
	{
		COleDateTime dtNow(COleDateTime::GetCurrentTime());
		dProportion = (date.m_dt - si.dtStart.m_dt) / (dtNow.m_dt - si.dtStart.m_dt);
	}

	dProportion = max(0.0, min(dProportion, 1.0));

	return (dDays * dProportion);
}

double CStatsItemCalculator::GetTimeInDays(double dTime, TDC_UNITS nUnits)
{
	switch (nUnits)
	{
	case TDCU_WEEKDAYS:
	case TDCU_DAYS:
		return dTime;
	}

	// all the rest
	TH_UNITS nTHUnits = MapUnitsToTHUnits(nUnits);
	return CTimeHelper().GetTime(dTime, nTHUnits, THU_WEEKDAYS);
}

TH_UNITS CStatsItemCalculator::MapUnitsToTHUnits(TDC_UNITS nUnits)
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

