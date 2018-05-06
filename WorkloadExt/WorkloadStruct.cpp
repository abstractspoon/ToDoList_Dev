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

WORKLOADITEM::WORKLOADITEM() 
	: 
	color(CLR_NONE), 
	bParent(FALSE), 
	dwTaskID(0), 
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
	Misc::Copy(wi.mapAllocatedDays, mapAllocatedDays);
	
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
			Misc::MatchAll(mapAllocatedDays, wi.mapAllocatedDays));
}

WORKLOADITEM::~WORKLOADITEM()
{
	
}

BOOL WORKLOADITEM::AllocationsMatch(const WORKLOADITEM& wi) const
{
	return Misc::MatchAll(mapAllocatedDays, wi.mapAllocatedDays);
}

BOOL WORKLOADITEM::HasStart() const
{
	return CDateHelper::IsDateSet(dtStart);
}

BOOL WORKLOADITEM::GetDuration(double& dDays) const
{
	if (!HasStart() || !HasDue())
		return FALSE;

	dDays = (dtDue.m_dt - dtStart.m_dt);

	if (!CDateHelper::DateHasTime(dtDue))
		dDays += 1.0;

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

BOOL WORKLOADITEM::GetAllocation(const CString& sAllocTo, double& dDays) const
{
	CString sDays;
	
	if (!GetAllocation(sAllocTo, sDays))
		return FALSE;

	dDays = Misc::Atof(sDays);
	return TRUE;
}

void WORKLOADITEM::SetAllocation(const CString& sAllocTo, double dDays)
{
	SetAllocation(sAllocTo, Misc::Format(dDays));
}

BOOL WORKLOADITEM::GetAllocation(const CString& sAllocTo, CString& sDays) const
{
	if (!mapAllocatedDays.Lookup(Misc::ToUpper(sAllocTo), sDays))
		return FALSE;

	return !sDays.IsEmpty();
}

void WORKLOADITEM::SetAllocation(const CString& sAllocTo, const CString& sDays)
{
	mapAllocatedDays[Misc::ToUpper(sAllocTo)] = sDays;
}

void WORKLOADITEM::DecodeAllocations(const CString& sAllocations)
{
	mapAllocatedDays.RemoveAll();

	CStringArray aAllocTo;
	int nAllocTo = Misc::Split(sAllocations, aAllocTo, '\n');

	while (nAllocTo--)
	{
		CString sDays, sPerson = aAllocTo[nAllocTo];
		Misc::Split(sPerson, sDays, ':');

		SetAllocation(sPerson, sDays);
	}
}

CString WORKLOADITEM::EncodeAllocations() const
{
	CString sAllocations, sPerson, sDays;
	CStringArray aAllocations;
	POSITION pos = mapAllocatedDays.GetStartPosition();

	while (pos)
	{
		mapAllocatedDays.GetNextAssoc(pos, sPerson, sDays);
		ASSERT(!sPerson.IsEmpty());

		if (!sPerson.IsEmpty())
			aAllocations.Add(sPerson + ':' + sDays);
	}

	return Misc::FormatArray(aAllocations, '\n');
}

void WORKLOADITEM::ClearAllocations()
{
	mapAllocatedDays.RemoveAll();
}

//////////////////////////////////////////////////////////////////////

CWorkloadItemMap::~CWorkloadItemMap()
{
	RemoveAll();
}

void CWorkloadItemMap::RemoveAll()
{
	DWORD dwTaskID = 0;
	WORKLOADITEM* pGI = NULL;
	
	POSITION pos = GetStartPosition();
	
	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pGI);
		ASSERT(pGI);
		
		delete pGI;
	}
	
	CMap<DWORD, DWORD, WORKLOADITEM*, WORKLOADITEM*&>::RemoveAll();
}

BOOL CWorkloadItemMap::ItemIsLocked(DWORD dwTaskID) const
{
	const WORKLOADITEM* pGI = GetItem(dwTaskID);
	
	return (pGI && pGI->bLocked);
}

BOOL CWorkloadItemMap::RemoveKey(DWORD dwKey)
{
	WORKLOADITEM* pGI = NULL;
	
	if (Lookup(dwKey, pGI))
	{
		delete pGI;
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

	WORKLOADITEM* pGI = NULL;
	
	if (Lookup(dwKey, pGI))
		ASSERT(pGI);
	
	return pGI;
}

//////////////////////////////////////////////////////////////////////

WORKLOADSORTCOLUMN::WORKLOADSORTCOLUMN() : nBy(WLCC_NONE), bAscending(-1)
{

}

BOOL WORKLOADSORTCOLUMN::Matches(WLC_COLUMN nSortBy, BOOL bSortAscending) const
{
	return ((nBy == nSortBy) && (bAscending == bSortAscending));
}

BOOL WORKLOADSORTCOLUMN::operator==(const WORKLOADSORTCOLUMN& col) const
{
	return Matches(col.nBy, col.bAscending);
}

BOOL WORKLOADSORTCOLUMN::Sort(WLC_COLUMN nSortBy, BOOL bAllowToggle, BOOL bSortAscending)
{
	if (!bAllowToggle && Matches(nSortBy, bSortAscending))
		return FALSE;

	WLC_COLUMN nOldSort = nBy;
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

BOOL WORKLOADSORT::IsSortingBy(WLC_COLUMN nColID) const
{
	if (!bMultiSort)
		return IsSingleSortingBy(nColID);

	return IsMultiSortingBy(nColID);
}

BOOL WORKLOADSORT::IsSingleSortingBy(WLC_COLUMN nColID) const
{
	return (!bMultiSort && (single.nBy == nColID));
}

BOOL WORKLOADSORT::IsMultiSortingBy(WLC_COLUMN nColID) const
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

BOOL WORKLOADSORT::Sort(WLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending)
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

