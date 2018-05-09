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
	Misc::CopyStrT<double>(wi.mapAllocatedDays, mapAllocatedDays);
	
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
			AllocatedDaysMatch(wi));
}

WORKLOADITEM::~WORKLOADITEM()
{
	
}

BOOL WORKLOADITEM::AllocatedDaysMatch(const WORKLOADITEM& wi) const
{
	return Misc::MatchAllStrT<double>(mapAllocatedDays, wi.mapAllocatedDays);
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

double WORKLOADITEM::GetAllocatedDays(const CString& sAllocTo) const
{
	double dDays = 0.0;
	mapAllocatedDays.Lookup(Misc::ToUpper(sAllocTo), dDays);

	return dDays;
}

BOOL WORKLOADITEM::SetAllocatedDays(const CString& sAllocTo, double dDays)
{
	if (dDays < 0)
	{
		ASSERT(0);
		return FALSE;
	}

	if (dDays == 0.0)
		mapAllocatedDays.RemoveKey(Misc::ToUpper(sAllocTo));
	else
		mapAllocatedDays[Misc::ToUpper(sAllocTo)] = dDays;

	return TRUE;
}

CString WORKLOADITEM::GetAllocatedDays(const CString& sAllocTo, int nDecimals) const
{
	double dDays = GetAllocatedDays(sAllocTo);

	return Format(dDays, nDecimals);
}

BOOL WORKLOADITEM::SetAllocatedDays(const CString& sAllocTo, const CString& sDays)
{
	return SetAllocatedDays(sAllocTo, Misc::Atof(sDays));
}

void WORKLOADITEM::DecodeAllocations(const CString& sAllocations)
{
	mapAllocatedDays.RemoveAll();

	CStringArray aAllocations;
	int nAllocTo = Misc::Split(sAllocations, aAllocations, '\n');

	while (nAllocTo--)
	{
		CString sDays, sAllocTo = aAllocations[nAllocTo];
		Misc::Split(sAllocTo, sDays, ':');

		SetAllocatedDays(sAllocTo, sDays);
	}
}

CString WORKLOADITEM::EncodeAllocations() const
{
	CString sAllocations, sAllocTo;
	CStringArray aAllocations;
	double dDays;
	POSITION pos = mapAllocatedDays.GetStartPosition();

	while (pos)
	{
		mapAllocatedDays.GetNextAssoc(pos, sAllocTo, dDays);
		ASSERT(!sAllocTo.IsEmpty());

		if (!sAllocTo.IsEmpty())
		{
			CString sDays;

			if (dDays > 0)
				aAllocations.Add(sAllocTo + ':' + Misc::Format(dDays));
		}
	}

	return Misc::FormatArray(aAllocations, '\n');
}

void WORKLOADITEM::ClearAllocations()
{
	mapAllocatedDays.RemoveAll();
}

double WORKLOADITEM::GetTotalAllocatedDays() const
{
	CString sAllocTo;
	CStringArray aAllocations;
	double dTotalDays = 0, dDays;
	POSITION pos = mapAllocatedDays.GetStartPosition();

	while (pos)
	{
		mapAllocatedDays.GetNextAssoc(pos, sAllocTo, dDays);
		ASSERT(!sAllocTo.IsEmpty());

		if (!sAllocTo.IsEmpty())
			dTotalDays += dDays;
	}

	return dTotalDays;
}

CString WORKLOADITEM::GetTotalAllocatedDays(int nDecimals) const
{
	double dDays = GetTotalAllocatedDays();

	return Format(dDays, nDecimals);
}

CString WORKLOADITEM::Format(double dDays, int nDecimals)
{
	return ((dDays == 0.0) ? _T("") : Misc::Format(dDays, nDecimals));
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

void CWorkloadItemMap::CalculateTotals(WORKLOADITEM& wiAllocatedDays, 
									 WORKLOADITEM& wiAllocatedTasks) const
{
	wiAllocatedDays.ClearAllocations();
	wiAllocatedTasks.ClearAllocations();

	DWORD dwTaskID = 0;
	WORKLOADITEM* pWI = NULL;

	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pWI);
		ASSERT(pWI);

		for (int nAllocTo = 0; nAllocTo < pWI->aAllocTo.GetSize(); nAllocTo++)
		{
			CString sAllocTo(pWI->aAllocTo[nAllocTo]);
			double dDays = pWI->GetAllocatedDays(sAllocTo);

			if (dDays > 0.0)
			{
				double dTotalDays = wiAllocatedDays.GetAllocatedDays(sAllocTo);
				VERIFY(wiAllocatedDays.SetAllocatedDays(sAllocTo, (dTotalDays + dDays)));

				double dTotalTasks = wiAllocatedTasks.GetAllocatedDays(sAllocTo);
				VERIFY(wiAllocatedTasks.SetAllocatedDays(sAllocTo, (dTotalTasks + 1.0)));
			}
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

