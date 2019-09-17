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

	if (date1 < date2)
		return -1;

	if (date1 > date2)
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

///////////////////////////////////////////////////////////////////////////

CStatsItemCalculator::CStatsItemCalculator(const CStatsItemArray& data)
	:
	m_data(data)
{
}

CStatsItemCalculator::~CStatsItemCalculator()
{

}

BOOL CStatsItemCalculator::SetDateRange(const COleDateTimeRange& dtExtents)
{
	if (!dtExtents.IsValid())
	{
		ASSERT(0);
		return FALSE;
	}

	m_dtStartExtents = dtExtents.GetStart();
	m_dtEndExtents = dtExtents.GetEndInclusive();

	m_nTotalDays = m_nTotalWeekdays = 0;

	return TRUE;
}

int CStatsItemCalculator::GetTotalDays() const
{
	if (m_nTotalDays == 0)
		m_nTotalDays = (int)(m_dtEndExtents.m_dt - m_dtStartExtents.m_dt);

	return m_nTotalDays;
}

int CStatsItemCalculator::GetTotalWeekdays() const
{
	if (m_nTotalWeekdays == 0)
		m_nTotalWeekdays = (int)CWorkingWeek().CalculateDurationInDays(m_dtStartExtents, m_dtEndExtents);

	return m_nTotalWeekdays;
}

int CStatsItemCalculator::GetIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nNextItemFrom) const
{
	// Sanity check
	if (date < m_dtStartExtents)
		return 0;
	
	int nNumItems = m_data.GetSize();
	int nNumStarted = 0, nNumDone = 0;
	
	for (int nItem = nItemFrom; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->dtStart < m_dtStartExtents)
			continue;

		if ((pSI->dtStart > date) || (pSI->dtStart > m_dtEndExtents))
			break;

		nNumStarted++;

		if (pSI->IsDone() && (pSI->dtDone < date))
			nNumDone++;

		// Optimisation: If we ever hit zero then we only need 
		// calculate from this point on for later dates
		if (nNumDone == nNumStarted)
			nNextItemFrom = (nItem + 1);
	}
	
	return (nNumStarted - nNumDone);
}

BOOL CStatsItemCalculator::GetStartedEndedTasks(const COleDateTime& date, int &nNumStarted, int &nNumDone) const
{
	nNumStarted = nNumDone = 0;

	// Sanity check
	if (date < m_dtStartExtents)
		return FALSE;
	
	int nNumItems = m_data.GetSize();
	
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->dtStart < m_dtStartExtents)
			continue;
		
		if ((pSI->dtStart > date) || (pSI->dtStart > m_dtEndExtents))
			break;
		
		nNumStarted++;

		if (pSI->IsDone() && (pSI->dtDone < date))
			nNumDone++;
	}

	ASSERT(nNumDone <= nNumStarted);
	
	return (nNumStarted > 0);
}

BOOL CStatsItemCalculator::GetDaysEstimatedSpent(const COleDateTime& date, double &dEstDays, double &dSpentDays) const
{
	dEstDays = dSpentDays = 0;

	// Sanity check
	if (date < m_dtStartExtents)
		return FALSE;
	
	int nNumItems = m_data.GetSize();
	
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->dtStart < m_dtStartExtents)
			continue;
		
		if ((pSI->dtStart > date) || (pSI->dtStart > m_dtEndExtents))
			break;
		
		dEstDays += GetAttribValue(*pSI, DAYS, ESTIMATE, date);
		dSpentDays += GetAttribValue(*pSI, DAYS, SPENT, date);
	}

	return (nNumItems > 0);
}

BOOL CStatsItemCalculator::GetCostEstimatedSpent(const COleDateTime& date, double &dEstCost, double &dSpentCost) const
{
	dEstCost = dSpentCost = 0;

	// Sanity check
	if (date < m_dtStartExtents)
		return FALSE;
	
	int nNumItems = m_data.GetSize();
	COleDateTime dtEndExtents = m_dtEndExtents;
	
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->dtStart < m_dtStartExtents)
			continue;
		
		if ((pSI->dtStart > date) || (pSI->dtStart > dtEndExtents))
			break;
		
		dEstCost += GetAttribValue(*pSI, COST, ESTIMATE, date);
		dSpentCost += GetAttribValue(*pSI, COST, SPENT, date);
	}

	return (nNumItems > 0);
}

double CStatsItemCalculator::GetIntersectionProportion(const STATSITEM& si, BOOL bWeekdays) const
{
	// Optimised as much as possible
	if (!CDateHelper::IsDateSet(si.dtStart))
		return 0.0;

	if (si.dtStart >= m_dtEndExtents)
		return 0.0;

	COleDateTime dtEnd = si.GetEndDate();

	if (!CDateHelper::IsDateSet(dtEnd))
		return 0.0;

	if (dtEnd <= m_dtStartExtents)
		return 0.0;

	COleDateTime dtStart = si.dtStart;

	if ((dtStart >= m_dtStartExtents) && (dtEnd <= m_dtEndExtents))
		return 1.0;

	double dTotalDays = (bWeekdays ? 
						 CWorkingWeek().CalculateDurationInDays(dtStart, dtEnd) :
						 (dtEnd.m_dt - dtStart.m_dt));

	if (dTotalDays == 0.0)
		return 0.0;

	// Get intersection
	CDateHelper::Max(dtStart, m_dtStartExtents);
	CDateHelper::Min(dtEnd, m_dtEndExtents);

	double dPartDays = (bWeekdays ? 
						CWorkingWeek().CalculateDurationInDays(dtStart, dtEnd) :
						(dtEnd.m_dt - dtStart.m_dt));

	ASSERT(dTotalDays && dPartDays);
	ASSERT(dPartDays <= dTotalDays);

	return max(0.0, min((dPartDays / dTotalDays), 1.0));
}

// ----------------------------------------------------

double CStatsItemCalculator::GetDaysEstimated() const
{
	return GetTotalAttribValue(DAYS, ESTIMATE);
}

double CStatsItemCalculator::GetDaysEstimated(const COleDateTime& date) const
{
	return GetTotalAttribValue(DAYS, ESTIMATE, date);
}

// ----------------------------------------------------

double CStatsItemCalculator::GetDaysSpent() const
{
	return GetTotalAttribValue(DAYS, SPENT);
}

double CStatsItemCalculator::GetDaysSpent(const COleDateTime& date) const
{
	return GetTotalAttribValue(DAYS, SPENT, date);
}

// ----------------------------------------------------

double CStatsItemCalculator::GetCostEstimate() const
{
	return GetTotalAttribValue(COST, ESTIMATE);
}

double CStatsItemCalculator::GetCostEstimated(const COleDateTime& date) const
{
	return GetTotalAttribValue(COST, ESTIMATE, date);
}

// ----------------------------------------------------

double CStatsItemCalculator::GetCostSpent() const
{
	return GetTotalAttribValue(COST, SPENT);
}

double CStatsItemCalculator::GetCostSpent(const COleDateTime& date) const
{
	return GetTotalAttribValue(COST, SPENT, date);
}

// ----------------------------------------------------

double CStatsItemCalculator::GetTotalAttribValue(ATTRIB nAttrib, ATTRIBTYPE nType) const
{
	double dTotal = 0;
	int nNumItems = m_data.GetSize();

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->GetEndDate() < m_dtStartExtents)
			continue;

		if (pSI->dtStart > m_dtEndExtents)
			break;

		dTotal += GetAttribValue(*pSI, nAttrib, nType);
	}

	return dTotal;
}

double CStatsItemCalculator::GetTotalAttribValue(ATTRIB nAttrib, ATTRIBTYPE nType, const COleDateTime& date) const
{
	double dTotal = 0;
	int nNumItems = m_data.GetSize();

	COleDateTime dtItemEnd;

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (!pSI->GetEndDate(dtItemEnd) || (dtItemEnd < m_dtStartExtents))
			continue;

		if (pSI->dtStart > m_dtEndExtents)
			break;

		dTotal += GetAttribValue(*pSI, nAttrib, nType, date);
	}

	return dTotal;
}

double CStatsItemCalculator::GetAttribValue(const STATSITEM& si, ATTRIB nAttrib, ATTRIBTYPE nType) const
{
	double dValue = 0.0;

	switch (nAttrib)
	{
	case DAYS:
		{
			switch (nType)
			{
			case ESTIMATE:
				dValue = GetTimeInDays(si.dTimeEst, si.nTimeEstUnits);
				break;

			case SPENT:
				dValue = GetTimeInDays(si.dTimeSpent, si.nTimeSpentUnits);
				break;

			default:
				ASSERT(0);
				break;
			}
		}
		break;


	case COST:
		{
			switch (nType)
			{
			case ESTIMATE:
				dValue = (si.bCostIsRate ? (si.dTimeEst * si.dCost) : si.dCost);
				break;

			case SPENT:
				dValue = (si.bCostIsRate ? (si.dTimeSpent * si.dCost) : si.dCost);
				break;

			default:
				ASSERT(0);
				break;
			}
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	if (dValue == 0.0)
		return 0.0;

	// else
	double dProportion = GetIntersectionProportion(si, FALSE);

	return (dValue * dProportion);
}

double CStatsItemCalculator::GetAttribValue(const STATSITEM& si, ATTRIB nAttrib, ATTRIBTYPE nType, const COleDateTime& date) const
{
	double dValue = GetAttribValue(si, nAttrib, nType);

	return CalcProportionOfValue(si, dValue, date);
}

double CStatsItemCalculator::CalcProportionOfValue(const STATSITEM& si, double dValue, const COleDateTime& date) const
{
	if (dValue == 0.0)
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

	return (dValue * dProportion);
}

double CStatsItemCalculator::GetTimeInDays(double dTime, TDC_UNITS nUnits)
{
	if (dTime == 0.0)
		return 0.0;

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

