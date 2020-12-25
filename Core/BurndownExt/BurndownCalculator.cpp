// BurndownWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BurndownCalculator.h"
#include "BurndownStruct.h"

#include "..\shared\misc.h"
#include "..\shared\datehelper.h"

#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CStatsItemCalculator::CStatsItemCalculator(const CStatsItemArray& data)
	:
	m_data(data),
	m_nTotalWeekdays(0),
	m_bShowEmptyFrequencyValues(TRUE),
	m_dStartExtents(0.0),
	m_dEndExtents(0.0)
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

	m_dStartExtents = dtExtents.GetStart();
	m_dEndExtents = CDateHelper::GetEndOfMonth(dtExtents.GetEndInclusive());
	m_nTotalWeekdays = 0;

	return TRUE;
}

BOOL CStatsItemCalculator::SetShowEmptyFrequencyValues(BOOL bShowEmpty) 
{ 
	if ((m_bShowEmptyFrequencyValues && !bShowEmpty) ||
		(!m_bShowEmptyFrequencyValues && bShowEmpty))
	{
		m_bShowEmptyFrequencyValues = bShowEmpty;
		return TRUE;
	}

	// else
	return FALSE;
}

int CStatsItemCalculator::GetTotalDays() const
{
	return (int)(m_dEndExtents - m_dStartExtents);
}

int CStatsItemCalculator::GetTotalWeekdays() const
{
	if (m_nTotalWeekdays == 0)
		m_nTotalWeekdays = (int)CWorkingWeek().CalculateDurationInDays(m_dStartExtents, m_dEndExtents);

	return m_nTotalWeekdays;
}

int CStatsItemCalculator::GetIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nNextItemFrom) const
{
	nNextItemFrom = 0;

	// Sanity check
	if (date.m_dt < m_dStartExtents)
		return 0;
	
	int nNumItems = m_data.GetSize();
	int nNumStarted = 0, nNumDone = 0;
	
	for (int nItem = nItemFrom; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->dtStart.m_dt < m_dStartExtents)
			continue;

		if ((pSI->dtStart.m_dt > date.m_dt) || (pSI->dtStart.m_dt > m_dEndExtents))
			break;

		nNumStarted++;

		if (pSI->IsDone() && (pSI->dtDone.m_dt < date.m_dt))
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
	if (date.m_dt < m_dStartExtents)
		return FALSE;
	
	int nNumItems = m_data.GetSize();
	
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->dtStart.m_dt < m_dStartExtents)
			continue;
		
		if ((pSI->dtStart.m_dt > date.m_dt) || (pSI->dtStart.m_dt > m_dEndExtents))
			break;
		
		nNumStarted++;

		if (pSI->IsDone() && (pSI->dtDone.m_dt < date.m_dt))
			nNumDone++;
	}

	ASSERT(nNumDone <= nNumStarted);
	
	return (nNumStarted > 0);
}

BOOL CStatsItemCalculator::GetDaysEstimatedSpent(const COleDateTime& date, double &dEstDays, double &dSpentDays) const
{
	dEstDays = dSpentDays = 0;

	// Sanity check
	if (date.m_dt < m_dStartExtents)
		return FALSE;
	
	int nNumItems = m_data.GetSize();
	
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->dtStart.m_dt < m_dStartExtents)
			continue;
		
		if ((pSI->dtStart.m_dt > date.m_dt) || (pSI->dtStart.m_dt > m_dEndExtents))
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
	if (date.m_dt < m_dStartExtents)
		return FALSE;
	
	int nNumItems = m_data.GetSize();
	COleDateTime dtEndExtents = m_dEndExtents;
	
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->dtStart.m_dt < m_dStartExtents)
			continue;
		
		if ((pSI->dtStart.m_dt > date.m_dt) || (pSI->dtStart.m_dt > dtEndExtents))
			break;
		
		dEstCost += GetAttribValue(*pSI, COST, ESTIMATE, date);
		dSpentCost += GetAttribValue(*pSI, COST, SPENT, date);
	}

	return (nNumItems > 0);
}

double CStatsItemCalculator::GetIntersectionProportion(const STATSITEM& si, BOOL bWeekdays) const
{
	ASSERT(m_dEndExtents > m_dStartExtents);

	// Optimised as much as possible
	if (!CDateHelper::IsDateSet(si.dtStart))
		return 0.0;

	if (si.dtStart.m_dt >= m_dEndExtents)
		return 0.0;

	COleDateTime dtEnd = si.GetEndDate();

	if (!CDateHelper::IsDateSet(dtEnd))
		return 0.0;

	if (dtEnd.m_dt <= m_dStartExtents)
		return 0.0;

	if ((si.dtStart.m_dt >= m_dStartExtents) && (dtEnd.m_dt <= m_dEndExtents))
		return 1.0;

	double dTotalDays = (bWeekdays ? 
						 CWorkingWeek().CalculateDurationInDays(si.dtStart, dtEnd) :
						 (dtEnd.m_dt - si.dtStart.m_dt));

	if (dTotalDays == 0.0)
		return 0.0;

	// Get intersection
	COleDateTime dtStart = si.dtStart;

	CDateHelper::Max(dtStart, m_dStartExtents);
	CDateHelper::Min(dtEnd, m_dEndExtents);

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

double CStatsItemCalculator::GetTotalAttribValue(TIMESERIES_ATTRIB nAttrib, TIMESERIES_ATTRIBTYPE nType) const
{
	double dTotal = 0;
	int nNumItems = m_data.GetSize();

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->GetEndDate().m_dt < m_dStartExtents)
			continue;

		if (pSI->dtStart.m_dt > m_dEndExtents)
			break;

		dTotal += GetAttribValue(*pSI, nAttrib, nType);
	}

	return dTotal;
}

double CStatsItemCalculator::GetTotalAttribValue(TIMESERIES_ATTRIB nAttrib, TIMESERIES_ATTRIBTYPE nType, const COleDateTime& date) const
{
	double dTotal = 0;
	int nNumItems = m_data.GetSize();

	COleDateTime dtItemEnd;

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (!pSI->GetEndDate(dtItemEnd) || (dtItemEnd.m_dt < m_dStartExtents))
			continue;

		if (pSI->dtStart.m_dt > m_dEndExtents)
			break;

		dTotal += GetAttribValue(*pSI, nAttrib, nType, date);
	}

	return dTotal;
}

double CStatsItemCalculator::GetAttribValue(const STATSITEM& si, TIMESERIES_ATTRIB nAttrib, TIMESERIES_ATTRIBTYPE nType) const
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

double CStatsItemCalculator::GetAttribValue(const STATSITEM& si, TIMESERIES_ATTRIB nAttrib, TIMESERIES_ATTRIBTYPE nType, const COleDateTime& date) const
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
		if (date.m_dt >= si.dtDone.m_dt)
		{
			dProportion = 1.0;
		}
		else if (bHasStart && (date.m_dt > si.dtStart.m_dt))
		{
			dProportion = (date.m_dt - si.dtStart.m_dt) / (si.dtDone.m_dt - si.dtStart.m_dt);
		}
	}
	else if (bHasStart && (date.m_dt > si.dtStart.m_dt))
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
	return CTimeHelper().Convert(dTime, nTHUnits, THU_DAYS);
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

int CStatsItemCalculator::HitTest(const COleDateTime& date) const
{
	if ((date.m_dt < GetStartDate()) || (date.m_dt >= GetEndDate()))
		return -1;

	// else
	return (int)(date.m_dt - GetStartDate());
}

int CStatsItemCalculator::GetAttribFrequencies(FREQUENCY_ATTRIB nAttrib, CMap<CString, LPCTSTR, int, int&>& mapFrequencies) const
{
	mapFrequencies.RemoveAll();
	int nNumItems = m_data.GetSize();

	COleDateTime dtItemEnd;

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		switch (nAttrib)
		{
		case CATEGORY:	AppendFrequencyAttribs(pSI->aCategory,		mapFrequencies); break;
		case ALLOCTO:	AppendFrequencyAttribs(pSI->aAllocatedTo,	mapFrequencies); break;
		case TAGS:		AppendFrequencyAttribs(pSI->aTags,			mapFrequencies); break;

		case STATUS:	AppendFrequencyAttrib(pSI->sStatus,			mapFrequencies); break;
		case ALLOCBY:	AppendFrequencyAttrib(pSI->sAllocatedBy,	mapFrequencies); break;
		case PRIORITY:	AppendFrequencyAttrib(pSI->sPriority,		mapFrequencies); break;
		case RISK:		AppendFrequencyAttrib(pSI->sRisk,			mapFrequencies); break;
		case VERSION:	AppendFrequencyAttrib(pSI->sVersion,		mapFrequencies); break;
		}
	}

	return mapFrequencies.GetCount();
}

void CStatsItemCalculator::AppendFrequencyAttrib(const CString& sAttrib, CMap<CString, LPCTSTR, int, int&>& mapFrequencies) const
{
	if (m_bShowEmptyFrequencyValues || !sAttrib.IsEmpty())
		mapFrequencies[sAttrib]++;
}

void CStatsItemCalculator::AppendFrequencyAttribs(const CStringArray& aAttrib, CMap<CString, LPCTSTR, int, int&>& mapFrequencies) const
{
	int nAttrib = aAttrib.GetSize();

	if (nAttrib == 0)
	{
		AppendFrequencyAttrib(_T(""), mapFrequencies);
	}
	else
	{
		while (nAttrib--)
			AppendFrequencyAttrib(aAttrib[nAttrib], mapFrequencies);
	}
}

int CStatsItemCalculator::GetCategoryFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const
{
	CMap<CString, LPCTSTR, int, int&> mapFrequencies;
	GetAttribFrequencies(CATEGORY, mapFrequencies);

	return AsSortedArray(mapFrequencies, aFrequencies);
}

int CStatsItemCalculator::GetStatusFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const
{
	CMap<CString, LPCTSTR, int, int&> mapFrequencies;
	GetAttribFrequencies(STATUS, mapFrequencies);

	return AsSortedArray(mapFrequencies, aFrequencies);
}

int CStatsItemCalculator::GetAllocatedToFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const
{
	CMap<CString, LPCTSTR, int, int&> mapFrequencies;
	GetAttribFrequencies(ALLOCTO, mapFrequencies);

	return AsSortedArray(mapFrequencies, aFrequencies);
}

int CStatsItemCalculator::GetAllocatedByFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const
{
	CMap<CString, LPCTSTR, int, int&> mapFrequencies;
	GetAttribFrequencies(ALLOCBY, mapFrequencies);

	return AsSortedArray(mapFrequencies, aFrequencies);
}

int CStatsItemCalculator::GetPriorityFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const
{
	if (!m_data.GetSize())
		return 0L;

	CMap<CString, LPCTSTR, int, int&> mapFrequencies;
	GetAttribFrequencies(PRIORITY, mapFrequencies);

	// Fill in any missing values
	for (int nVal = 0; nVal <= 10; nVal++)
		mapFrequencies[Misc::Format(nVal)];

	return AsSortedArray(mapFrequencies, aFrequencies);
}

int CStatsItemCalculator::GetRiskFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const
{
	if (!m_data.GetSize())
		return 0L;

	CMap<CString, LPCTSTR, int, int&> mapFrequencies;
	GetAttribFrequencies(RISK, mapFrequencies);
	
	// Fill in any missing values
	for (int nVal = 0; nVal <= 10; nVal++)
		mapFrequencies[Misc::Format(nVal)];

	return AsSortedArray(mapFrequencies, aFrequencies);
}

int CStatsItemCalculator::GetTagFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const
{
	CMap<CString, LPCTSTR, int, int&> mapFrequencies;
	GetAttribFrequencies(TAGS, mapFrequencies);

	return AsSortedArray(mapFrequencies, aFrequencies);
}

int CStatsItemCalculator::GetVersionFrequencies(CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies) const
{
	CMap<CString, LPCTSTR, int, int&> mapFrequencies;
	GetAttribFrequencies(VERSION, mapFrequencies);

	return AsSortedArray(mapFrequencies, aFrequencies);
}

int CStatsItemCalculator::AsSortedArray(const CMap<CString, LPCTSTR, int, int&>& mapFrequencies, CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies)
{
	CString sLabel;
	int nCount, nItem = 0;

	aFrequencies.SetSize(mapFrequencies.GetCount());
	POSITION pos = mapFrequencies.GetStartPosition();

	while (pos)
	{
		mapFrequencies.GetNextAssoc(pos, sLabel, nCount);

		aFrequencies[nItem].sLabel = sLabel;
		aFrequencies[nItem].nCount = nCount;

		nItem++;
	}

	if (nItem > 1)
		Misc::SortArrayT<FREQUENCYITEM>(aFrequencies, CompareFrequencyItems);

	return nItem;
}

int CStatsItemCalculator::CompareFrequencyItems(const void* pV1, const void* pV2)
{
	const FREQUENCYITEM* pFI1 = (const FREQUENCYITEM*)pV1;
	const FREQUENCYITEM* pFI2 = (const FREQUENCYITEM*)pV2;

	return Misc::NaturalCompare(pFI1->sLabel, pFI2->sLabel);
}
