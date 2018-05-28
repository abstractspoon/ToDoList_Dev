// WorkloadStruct.cpp: implementation of the CWorkloadStruct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkloadStruct.h"

#include "..\shared\DateHelper.h"
#include "..\shared\graphicsMisc.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CMapAllocations::CMapAllocations(BOOL bReturnAverageForTotal) 
	:
	m_bReturnAverageForTotal(bReturnAverageForTotal)
{
}

CMapAllocations::~CMapAllocations() 
{
}

void CMapAllocations::Copy(const CMapAllocations& other)
{
	Misc::CopyStrT<double>(other, *this);
}

BOOL CMapAllocations::MatchAll(const CMapAllocations& other) const
{
	return Misc::MatchAllStrT<double>(*this, other);
}

void CMapAllocations::RemoveAll()
{
	CMap<CString, LPCTSTR, double, double&>::RemoveAll();
}

double CMapAllocations::Get(const CString& sAllocTo) const
{
	if (sAllocTo == ALLOCTO_TOTALID)
		return GetTotal();

	// else
	double dDays = 0.0;
	Lookup(Misc::ToUpper(sAllocTo), dDays);

	return dDays;
}

BOOL CMapAllocations::Set(const CString& sAllocTo, double dValue)
{
	if (dValue < 0)
	{
		ASSERT(0);
		return FALSE;
	}

	if (dValue == 0.0)
		RemoveKey(Misc::ToUpper(sAllocTo));
	else
		SetAt(Misc::ToUpper(sAllocTo), dValue);

	return TRUE;
}

CString CMapAllocations::Get(const CString& sAllocTo, int nDecimals) const
{
	double dValue = Get(sAllocTo);

	return Format(dValue, nDecimals);
}

BOOL CMapAllocations::Set(const CString& sAllocTo, const CString& sDays)
{
	return Set(sAllocTo, Misc::Atof(sDays));
}

BOOL CMapAllocations::Add(const CString& sAllocTo, double dValue)
{
	if (dValue <= 0)
		return FALSE;

	return Set(sAllocTo, (Get(sAllocTo) + dValue));
}

void CMapAllocations::Increment(const CString& sAllocTo)
{
	Add(sAllocTo, 1.0);
}

void CMapAllocations::Decode(const CString& sAllocations)
{
	RemoveAll();

	CStringArray aAllocations;
	int nAllocTo = Misc::Split(sAllocations, aAllocations, '\n');

	while (nAllocTo--)
	{
		CString sDays, sAllocTo = aAllocations[nAllocTo];
		Misc::Split(sAllocTo, sDays, ':');

		Set(sAllocTo, sDays);
	}
}

CString CMapAllocations::Encode() const
{
	CString sAllocations, sAllocTo;
	CStringArray aAllocations;
	double dValue;
	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, sAllocTo, dValue);
		ASSERT(!sAllocTo.IsEmpty());

		if (!sAllocTo.IsEmpty())
		{
			CString sDays;

			if (dValue > 0)
				aAllocations.Add(sAllocTo + ':' + Misc::Format(dValue));
		}
	}

	return Misc::FormatArray(aAllocations, '\n');
}

double CMapAllocations::GetTotal() const
{
	CString sAllocTo;
	CStringArray aAllocations;
	double dTotalDays = 0, dValue;
	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, sAllocTo, dValue);
		ASSERT(!sAllocTo.IsEmpty());

		if (!sAllocTo.IsEmpty())
			dTotalDays += dValue;
	}

	return dTotalDays;
}

CString CMapAllocations::GetTotal(int nDecimals) const
{
	double dValue = GetTotal();

	if (m_bReturnAverageForTotal && GetCount())
		dValue /= GetCount();

	return Format(dValue, nDecimals);
}

CString CMapAllocations::Format(double dValue, int nDecimals)
{
	return ((dValue == 0.0) ? _T("") : Misc::Format(dValue, nDecimals));
}

//////////////////////////////////////////////////////////////////////

WORKLOADITEM::WORKLOADITEM(DWORD dwID, LPCTSTR szTitle) 
	: 
	sTitle(szTitle),
	color(CLR_NONE), 
	bParent(FALSE), 
	dwTaskID(dwID), 
	dwRefID(0), 
	dwOrgRefID(0), 
	bGoodAsDone(FALSE),
	bDone(FALSE),
	nPosition(-1),
	bLocked(FALSE),
	bSomeSubtaskDone(FALSE)
{
}

WORKLOADITEM::WORKLOADITEM(const WORKLOADITEM& wi)
{
	*this = wi;
}

WORKLOADITEM& WORKLOADITEM::operator=(const WORKLOADITEM& wi)
{
	sTitle = wi.sTitle;
	dtStart = wi.dtStart;
	dtDue = wi.dtDue;
	bDone = wi.bDone;
	color = wi.color;
	bParent = wi.bParent;
	dwTaskID = wi.dwTaskID;
	dwRefID = wi.dwRefID;
	nPercent = wi.nPercent;
	bGoodAsDone = wi.bGoodAsDone;
	bDone = wi.bDone;
	nPosition = wi.nPosition;
	bLocked = wi.bLocked;
	bHasIcon = wi.bHasIcon;
	bSomeSubtaskDone = wi.bSomeSubtaskDone;
	
	aAllocTo.Copy(wi.aAllocTo);
	mapAllocatedDays.Copy(wi.mapAllocatedDays);
	
	return (*this);
}

BOOL WORKLOADITEM::operator==(const WORKLOADITEM& wi) const
{
	return ((sTitle == wi.sTitle) &&
			(dtStart == wi.dtStart) &&
			(dtDue == wi.dtDue) &&
			(bDone == wi.bDone) &&
			(color == wi.color) &&
			(bParent == wi.bParent) &&
			(dwTaskID == wi.dwTaskID) &&
			(dwRefID == wi.dwRefID) &&
			(nPercent == wi.nPercent) &&	
			(nPosition == wi.nPosition) &&
			(bGoodAsDone == wi.bGoodAsDone) &&
			(bDone == wi.bDone) &&
			(bLocked == wi.bLocked) &&
			(bHasIcon == wi.bHasIcon) &&
			(bSomeSubtaskDone == wi.bSomeSubtaskDone) &&
			Misc::MatchAll(aAllocTo, wi.aAllocTo) &&
			mapAllocatedDays.MatchAll(wi.mapAllocatedDays));
}

WORKLOADITEM::~WORKLOADITEM()
{
	
}

BOOL WORKLOADITEM::HasStart() const
{
	return CDateHelper::IsDateSet(dtStart);
}

BOOL WORKLOADITEM::GetDuration(double& dDays, BOOL bWeekdays) const
{
	if (!HasStart() || !HasDue())
		return FALSE;

	dDays = CDateHelper::CalcDaysFromTo(dtStart, dtDue, TRUE, bWeekdays);
	return TRUE;
}

BOOL WORKLOADITEM::HasDue() const
{
	return CDateHelper::IsDateSet(dtDue);
}

COLORREF WORKLOADITEM::GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const
{
	if (HasColor())
	{
		if (bColorIsBkgnd && !bSelected && !bDone && !bGoodAsDone)
			return GraphicsMisc::GetBestTextColor(color);
		else
			return color;
	}
	
	// else
	return GetSysColor(COLOR_WINDOWTEXT);
}

COLORREF WORKLOADITEM::GetTextBkColor(BOOL bSelected, BOOL bColorIsBkgnd) const
{
	if (!bSelected && HasColor())
	{
		if (bColorIsBkgnd && !bDone && !bGoodAsDone)
			return color;
	}
	
	// else
	return CLR_NONE;
}

BOOL WORKLOADITEM::HasColor() const
{
	return ((color != CLR_NONE) && (color != GetSysColor(COLOR_WINDOWTEXT)));
}

//////////////////////////////////////////////////////////////////////

CWorkloadItemMap::~CWorkloadItemMap()
{
	RemoveAll();
}

void CWorkloadItemMap::RemoveAll()
{
	DWORD dwTaskID = 0;
	WORKLOADITEM* pWI = NULL;
	
	POSITION pos = GetStartPosition();
	
	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pWI);
		ASSERT(pWI);
		
		delete pWI;
	}
	
	CMap<DWORD, DWORD, WORKLOADITEM*, WORKLOADITEM*&>::RemoveAll();
}

void CWorkloadItemMap::CalculateTotals(const COleDateTime& dtBegin, const COleDateTime& dtEndInclusive,
	CMapAllocations& mapTotalDays, CMapAllocations& mapTotalTasks) const
{
	mapTotalDays.RemoveAll();
	mapTotalTasks.RemoveAll();

	DWORD dwTaskID = 0;
	WORKLOADITEM* pWI = NULL;

	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pWI);
		ASSERT(pWI);

		// Count how many the days of the task fall within the specified period
		if (!pWI->HasDates() || pWI->IsDone())
			continue;

		COleDateTime dtFrom = max(pWI->dtStart, dtBegin);
		COleDateTime dtTo = min(pWI->dtDue, dtEndInclusive);

		double dTaskDays = CDateHelper::CalcDaysFromTo(dtFrom, dtTo, TRUE, TRUE);
			
		if (dTaskDays == 0.0)
			continue;

		double dTaskDuration;
		VERIFY(pWI->GetDuration(dTaskDuration, TRUE));

		double dProportion = (dTaskDays / dTaskDuration);

		for (int nAllocTo = 0; nAllocTo < pWI->aAllocTo.GetSize(); nAllocTo++)
		{
			CString sAllocTo(pWI->aAllocTo[nAllocTo]);
			double dDays = (pWI->mapAllocatedDays.Get(sAllocTo) * dProportion);

			if (mapTotalDays.Add(sAllocTo, dDays))
				mapTotalTasks.Increment(sAllocTo);
		}
	}
}

BOOL CWorkloadItemMap::ItemIsLocked(DWORD dwTaskID) const
{
	const WORKLOADITEM* pWI = GetItem(dwTaskID);
	
	return (pWI && pWI->bLocked);
}

BOOL CWorkloadItemMap::RemoveKey(DWORD dwKey)
{
	WORKLOADITEM* pWI = NULL;
	
	if (Lookup(dwKey, pWI))
	{
		delete pWI;
		return CMap<DWORD, DWORD, WORKLOADITEM*, WORKLOADITEM*&>::RemoveKey(dwKey);
	}
	
	// else
	return FALSE;
}

BOOL CWorkloadItemMap::HasItem(DWORD dwKey) const
{
	if (dwKey == 0)
		return FALSE;

	return (GetItem(dwKey) != NULL);
}

WORKLOADITEM* CWorkloadItemMap::GetItem(DWORD dwKey) const
{
	if (dwKey == 0)
		return NULL;

	WORKLOADITEM* pWI = NULL;
	
	if (Lookup(dwKey, pWI))
		ASSERT(pWI);
	
	return pWI;
}

//////////////////////////////////////////////////////////////////////

WORKLOADSORTCOLUMN::WORKLOADSORTCOLUMN() : nBy(WLCC_NONE), bAscending(-1)
{

}

BOOL WORKLOADSORTCOLUMN::Matches(WLC_COLUMNID nSortBy, BOOL bSortAscending) const
{
	return ((nBy == nSortBy) && (bAscending == bSortAscending));
}

BOOL WORKLOADSORTCOLUMN::operator==(const WORKLOADSORTCOLUMN& col) const
{
	return Matches(col.nBy, col.bAscending);
}

BOOL WORKLOADSORTCOLUMN::Sort(WLC_COLUMNID nSortBy, BOOL bAllowToggle, BOOL bSortAscending)
{
	if (!bAllowToggle && Matches(nSortBy, bSortAscending))
		return FALSE;

	WLC_COLUMNID nOldSort = nBy;
	nBy = nSortBy;

	if (nSortBy != WLCC_NONE)
	{
		// if it's the first time or we are changing columns 
		// we always reset the direction
		if ((bAscending == -1) || (nSortBy != nOldSort))
		{
			if (bSortAscending != -1)
			{
				bAscending = bSortAscending;
			}
			else
			{
				bAscending = 1;
			}
		}
		else if (bAllowToggle)
		{
			ASSERT(bAscending != -1);
			bAscending = !bAscending;
		}
		else
		{
			ASSERT(bSortAscending != -1);
			bAscending = bSortAscending;
		}
	}
	else
	{
		// Always ascending for 'unsorted' to match app
		bAscending = 1;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

WORKLOADSORTCOLUMNS::WORKLOADSORTCOLUMNS()
{

}

BOOL WORKLOADSORTCOLUMNS::Sort(const WORKLOADSORTCOLUMNS& sort)
{
	if (*this == sort)
		return FALSE;

	for (int nCol = 0; nCol < 3; nCol++)
		cols[nCol] = sort.cols[nCol];

	return TRUE;
}

BOOL WORKLOADSORTCOLUMNS::operator==(const WORKLOADSORTCOLUMNS& sort) const
{
	for (int nCol = 0; nCol < 3; nCol++)
	{
		if (!(cols[nCol] == sort.cols[nCol]))
			return FALSE;
	}

	// else
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

WORKLOADSORT::WORKLOADSORT() : bMultiSort(FALSE)
{

}

BOOL WORKLOADSORT::IsSorting() const
{
	if (!bMultiSort)
		return (single.nBy != WLCC_NONE);

	// else
	return (multi.cols[0].nBy != WLCC_NONE);
}

BOOL WORKLOADSORT::IsSortingBy(WLC_COLUMNID nColID) const
{
	if (!bMultiSort)
		return IsSingleSortingBy(nColID);

	return IsMultiSortingBy(nColID);
}

BOOL WORKLOADSORT::IsSingleSortingBy(WLC_COLUMNID nColID) const
{
	return (!bMultiSort && (single.nBy == nColID));
}

BOOL WORKLOADSORT::IsMultiSortingBy(WLC_COLUMNID nColID) const
{
	if (bMultiSort)
	{
		for (int nCol = 0; nCol < 3; nCol++)
		{
			if (multi.cols[nCol].nBy == nColID)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL WORKLOADSORT::Sort(WLC_COLUMNID nBy, BOOL bAllowToggle, BOOL bAscending)
{
	if (bMultiSort)
	{
		bMultiSort = FALSE;
		return single.Sort(nBy, FALSE, bAscending);
	}

	return single.Sort(nBy, bAllowToggle, bAscending);
}

BOOL WORKLOADSORT::Sort(const WORKLOADSORTCOLUMNS& sort)
{
	bMultiSort = TRUE;
	return multi.Sort(sort);
}

/////////////////////////////////////////////////////////////////////////////

