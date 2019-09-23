// BurndownWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BurndownStruct.h"

#include "..\shared\datehelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

STATSITEM::STATSITEM(DWORD dwID)
	: 
	dwTaskID(dwID), 
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

	dtDue = GetDueDate(pTasks, hTask);
	dtDone = GetDoneDate(pTasks, hTask);

	dtStart = GetStartDate(pTasks, hTask);
	ValidateStartDate();

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
		dtDue = GetDueDate(pTasks, hTask);

	if (pTasks->IsAttributeAvailable(TDCA_DONEDATE))
		dtDone = GetDoneDate(pTasks, hTask);

	if (pTasks->IsAttributeAvailable(TDCA_STARTDATE))
	{
		dtStart = GetStartDate(pTasks, hTask);
		ValidateStartDate();
	}

	if (pTasks->IsAttributeAvailable(TDCA_TIMEEST))
		dTimeEst = pTasks->GetTaskTimeEstimate(hTask, nTimeEstUnits, false);

	if (pTasks->IsAttributeAvailable(TDCA_TIMESPENT))
		dTimeSpent = pTasks->GetTaskTimeSpent(hTask, nTimeSpentUnits, false);

	if (pTasks->IsAttributeAvailable(TDCA_COST))
		dCost = GetCost(pTasks, hTask, bCostIsRate);
}

void STATSITEM::ValidateStartDate()
{
	// make sure start is less than end
	if (IsDone())
		CDateHelper::Min(dtStart, dtDone);
	else
		CDateHelper::Min(dtStart, dtDue);
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

	if (pTasks->GetTaskStartDate64(hTask, false, tDate))
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

COleDateTime STATSITEM::GetDueDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	time64_t tDate = 0;
	COleDateTime date;

	if (pTasks->GetTaskDueDate64(hTask, false, tDate))
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

BOOL STATSITEM::HasDue() const
{
	return CDateHelper::IsDateSet(dtDue);
}

BOOL STATSITEM::IsDone() const
{
	return CDateHelper::IsDateSet(dtDone);
}

COleDateTime STATSITEM::GetEndDate() const
{
	if (IsDone())
		return dtDone;

	if (HasDue())
		return dtDue;

	return dtStart;
}

BOOL STATSITEM::GetEndDate(COleDateTime& dtEnd) const
{
	if (IsDone())
	{
		dtEnd = dtDone;
	}
	else if (HasDue())
	{
		dtEnd = dtDue;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void STATSITEM::MinMax(COleDateTimeRange& dtExtents) const
{
	MinMax(dtStart, dtExtents);
	MinMax(dtDue, dtExtents);
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

CStatsItemArray::CStatsItemArray() : m_bSorted(TRUE)
{

}

CStatsItemArray::~CStatsItemArray()
{
	RemoveAll();
}

STATSITEM* CStatsItemArray::AddItem(DWORD dwTaskID)
{
	STATSITEM* pSI = GetItem(dwTaskID);

	if (pSI)
	{
		ASSERT(0);
		return NULL;
	}

	pSI = new STATSITEM(dwTaskID);
	Add(pSI);

	m_mapTasks[dwTaskID] = pSI;
	m_bSorted = FALSE;

	return pSI;
}

STATSITEM* CStatsItemArray::operator[](int nIndex) const
{
	return CArray<STATSITEM*, STATSITEM*>::operator[](nIndex);
}

STATSITEM* CStatsItemArray::GetItem(DWORD dwTaskID) const
{
	STATSITEM* pSI = NULL;
	m_mapTasks.Lookup(dwTaskID, pSI);

	return pSI;
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
	m_mapTasks.RemoveAll();

	m_bSorted = TRUE;
}

void CStatsItemArray::RemoveAt(int nIndex, int nCount)
{
	ASSERT((nIndex >= 0) && ((nIndex + nCount) <= GetSize()));
	ASSERT(nCount > 0);

	nIndex += nCount;

	while (nIndex--)
	{
		STATSITEM* pSI = GetAt(nIndex);

		m_mapTasks.RemoveKey(pSI->dwTaskID);
		delete pSI;

		CArray<STATSITEM*, STATSITEM*>::RemoveAt(nIndex);
	}
}

void CStatsItemArray::Sort()
{
	if (!m_bSorted)
	{
		qsort(GetData(), GetSize(), sizeof(STATSITEM*), CompareItems);
		m_bSorted = TRUE;
	}
}

int CStatsItemArray::CompareItems(const void* pV1, const void* pV2)
{
	typedef STATSITEM* LPSTATSITEM;

	const STATSITEM* pSI1 = *(static_cast<const LPSTATSITEM*>(pV1));
	const STATSITEM* pSI2 = *(static_cast<const LPSTATSITEM*>(pV2));

	ASSERT(pSI1 && pSI2 && (pSI1 != pSI2));

	// Sort by start date and then done date
	int nCmp = CompareDates(pSI1->dtStart, pSI2->dtStart);

	if (nCmp == 0)
		nCmp = CompareDates(pSI1->dtDone, pSI2->dtDone);

	return nCmp;
}

int CStatsItemArray::CompareDates(const COleDateTime& date1, const COleDateTime& date2)
{
	// Tasks without a date come last to optimise searching
	BOOL bDate1 = CDateHelper::IsDateSet(date1);
	BOOL bDate2 = CDateHelper::IsDateSet(date2);

	if (bDate1 && !bDate2)
		return -1;

	if (!bDate1 && bDate2)
		return 1;

	if (!bDate1 && !bDate2)
		return 0;

	if (date1.m_dt < date2.m_dt)
		return -1;

	if (date1.m_dt > date2.m_dt)
		return 1;

	return 0;
}


void CStatsItemArray::GetDataExtents(COleDateTimeRange& dtExtents) const
{
	int nItem = GetSize();

	if (nItem > 0)
	{
		dtExtents.Reset();

		while (nItem--)
			GetAt(nItem)->MinMax(dtExtents);

		// Clip to day
		dtExtents.m_dtStart = CDateHelper::GetDateOnly(dtExtents.m_dtStart);
		dtExtents.m_dtEnd = CDateHelper::GetDateOnly(dtExtents.m_dtEnd); 

		// One day earlier to visually encapsulate the first date
		dtExtents.m_dtStart.m_dt--;
	}
	else
	{
		dtExtents.Set(DHD_NOW, DHD_NOW);
	}
}


