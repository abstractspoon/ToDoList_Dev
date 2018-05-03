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
	nPosition(-1),
	bLocked(FALSE),
	bSomeSubtaskDone(FALSE)
{
}

WORKLOADITEM::WORKLOADITEM(const WORKLOADITEM& gi)
{
	*this = gi;
}

WORKLOADITEM& WORKLOADITEM::operator=(const WORKLOADITEM& gi)
{
	sTitle = gi.sTitle;
	dtStart = gi.dtStart;
	dtMinStart = gi.dtMinStart;
	dtDue = gi.dtDue;
	dtMaxDue = gi.dtMaxDue;
	dtDone = gi.dtDone;
	color = gi.color;
	sAllocTo = gi.sAllocTo;
	bParent = gi.bParent;
	dwTaskID = gi.dwTaskID;
	dwRefID = gi.dwRefID;
	nPercent = gi.nPercent;
	bGoodAsDone = gi.bGoodAsDone;
	nPosition = gi.nPosition;
	bLocked = gi.bLocked;
	bHasIcon = gi.bHasIcon;
	bSomeSubtaskDone = gi.bSomeSubtaskDone;
	
	aTags.Copy(gi.aTags);
	aDependIDs.Copy(gi.aDependIDs);
	
	return (*this);
}

BOOL WORKLOADITEM::operator==(const WORKLOADITEM& gi)
{
	return ((sTitle == gi.sTitle) &&
			(dtStart == gi.dtStart) &&
			(dtMinStart == gi.dtMinStart) &&
			(dtDue == gi.dtDue) &&
			(dtMaxDue == gi.dtMaxDue) &&
			(dtDone == gi.dtDone) &&
			(color == gi.color) &&
			(sAllocTo == gi.sAllocTo) &&
			(bParent == gi.bParent) &&
			(dwTaskID == gi.dwTaskID) &&
			(dwRefID == gi.dwRefID) &&
			(nPercent == gi.nPercent) &&	
			(nPosition == gi.nPosition) &&
			(bGoodAsDone == gi.bGoodAsDone) &&
			(bLocked == gi.bLocked) &&
			(bHasIcon == gi.bHasIcon) &&
			(bSomeSubtaskDone == gi.bSomeSubtaskDone) &&
			Misc::MatchAll(aTags, gi.aTags) &&
			Misc::MatchAll(aDependIDs, gi.aDependIDs));
}

WORKLOADITEM::~WORKLOADITEM()
{
	
}

void WORKLOADITEM::MinMaxDates(const WORKLOADITEM& giOther)
{
	if (giOther.bParent)
	{
		CDateHelper::Max(dtMaxDue, giOther.dtMaxDue);
		CDateHelper::Min(dtMinStart, giOther.dtMinStart);
	}
	else // leaf task
	{
		CDateHelper::Max(dtMaxDue, giOther.dtDue);
		CDateHelper::Max(dtMaxDue, giOther.dtDone);
		CDateHelper::Min(dtMinStart, giOther.dtStart);
	}
}

BOOL WORKLOADITEM::IsDone(BOOL bIncGoodAs) const
{
	if (CDateHelper::IsDateSet(dtDone))
		return TRUE;

	// else
	return (bIncGoodAs && bGoodAsDone);
}

BOOL CWorkloadItemMap::ItemIsLocked(DWORD dwTaskID) const
{
	const WORKLOADITEM* pGI = GetItem(dwTaskID);
	
	return (pGI && pGI->bLocked);
}

BOOL CWorkloadItemMap::ItemHasDependecies(DWORD dwTaskID) const
{
	const WORKLOADITEM* pGI = GetItem(dwTaskID);
	
	return (pGI && pGI->aDependIDs.GetSize());
}

BOOL WORKLOADITEM::HasStart() const
{
	return CDateHelper::IsDateSet(dtStart);
}

BOOL WORKLOADITEM::HasDue() const
{
	return CDateHelper::IsDateSet(dtDue);
}

COLORREF WORKLOADITEM::GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const
{
	if (HasColor())
	{
		if (bColorIsBkgnd && !bSelected && !IsDone(TRUE))
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
		if (bColorIsBkgnd && !IsDone(TRUE))
			return color;
	}
	
	// else
	return CLR_NONE;
}

COLORREF WORKLOADITEM::GetFillColor() const
{
	if (IsDone(TRUE))
	{
		if (!Misc::IsHighContrastActive())
			return GraphicsMisc::Lighter(color, 0.8);
	}
	else if (HasColor())
	{
		return color;
	}
	
	// else
	return CLR_NONE;
}

COLORREF WORKLOADITEM::GetBorderColor() const
{
	if (IsDone(TRUE))
	{
		if (!Misc::IsHighContrastActive())
			return color;
	}
	else if (HasColor())
	{
		return GraphicsMisc::Darker(color, 0.4);
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

BOOL CWorkloadItemMap::RestoreItem(const WORKLOADITEM& giPrev)
{
	WORKLOADITEM* pGI = NULL;

	if (Lookup(giPrev.dwTaskID, pGI) && pGI)
	{
		*pGI = giPrev;
		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CWorkloadItemMap::IsItemDependentOn(const WORKLOADITEM* pGI, DWORD dwOtherID) const
{
	if (!pGI)
	{
		ASSERT(0);
		return FALSE;
	}

	int nDepend = pGI->aDependIDs.GetSize();

	while (nDepend--)
	{
		DWORD dwDependID = pGI->aDependIDs[nDepend];
		ASSERT(dwDependID);

		if (dwDependID == dwOtherID)
			return TRUE;

		// else check dependents of dwDependID
		if (IsItemDependentOn(GetItem(dwDependID), dwOtherID)) // RECURSIVE
			return TRUE;
	}
	
	// all else
	return FALSE;
}

//////////////////////////////////////////////////////////////////////

WORKLOADDATERANGE::WORKLOADDATERANGE()
{
	Clear();
}

void WORKLOADDATERANGE::Clear()
{
	CDateHelper::ClearDate(dtStart);
	CDateHelper::ClearDate(dtEnd);
}

void WORKLOADDATERANGE::MinMax(const WORKLOADITEM& gi)
{
	MinMax(gi.dtStart);
	MinMax(gi.dtDue);
	MinMax(gi.dtDone);
}

void WORKLOADDATERANGE::MinMax(const COleDateTime& date)
{
	CDateHelper::Min(dtStart, date);
	CDateHelper::Max(dtEnd, date);
}

COleDateTime WORKLOADDATERANGE::GetStart(WLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades) const
{
	COleDateTime dtTemp = COleDateTime::GetCurrentTime();

	if (CDateHelper::IsDateSet(dtStart))
		dtTemp = dtStart;

	switch (nDisplay)
	{
	case WLC_DISPLAY_QUARTERCENTURIES:
		return CDateHelper::GetStartOfQuarterCentury(dtTemp, bZeroBasedDecades);

	case WLC_DISPLAY_DECADES:
		return CDateHelper::GetStartOfDecade(dtTemp, bZeroBasedDecades);

	case WLC_DISPLAY_YEARS:
		return CDateHelper::GetStartOfYear(dtTemp);

	case WLC_DISPLAY_QUARTERS_SHORT:
	case WLC_DISPLAY_QUARTERS_MID:
	case WLC_DISPLAY_QUARTERS_LONG:
		return CDateHelper::GetStartOfQuarter(dtTemp);

	case WLC_DISPLAY_MONTHS_SHORT:
	case WLC_DISPLAY_MONTHS_MID:
	case WLC_DISPLAY_MONTHS_LONG:
	case WLC_DISPLAY_WEEKS_SHORT:
	case WLC_DISPLAY_WEEKS_MID:
	case WLC_DISPLAY_WEEKS_LONG:
	case WLC_DISPLAY_DAYS_SHORT:
	case WLC_DISPLAY_DAYS_MID:
	case WLC_DISPLAY_DAYS_LONG:
	case WLC_DISPLAY_HOURS:
		return CDateHelper::GetStartOfMonth(dtTemp);
	}

	ASSERT(0);
	return dtTemp;
}

COleDateTime WORKLOADDATERANGE::GetEnd(WLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades) const
{
	COleDateTime dtTemp = COleDateTime::GetCurrentTime();

	if (CDateHelper::IsDateSet(dtEnd))
		dtTemp = dtEnd;

	switch (nDisplay)
	{
	case WLC_DISPLAY_QUARTERCENTURIES:
		return CDateHelper::GetEndOfQuarterCentury(dtTemp, bZeroBasedDecades);

	case WLC_DISPLAY_DECADES:
		return CDateHelper::GetEndOfDecade(dtTemp, bZeroBasedDecades);

	case WLC_DISPLAY_YEARS:
		return CDateHelper::GetEndOfYear(dtTemp);

	case WLC_DISPLAY_QUARTERS_SHORT:
	case WLC_DISPLAY_QUARTERS_MID:
	case WLC_DISPLAY_QUARTERS_LONG:
		return CDateHelper::GetEndOfQuarter(dtTemp);

	case WLC_DISPLAY_MONTHS_SHORT:
	case WLC_DISPLAY_MONTHS_MID:
	case WLC_DISPLAY_MONTHS_LONG:
	case WLC_DISPLAY_WEEKS_SHORT:
	case WLC_DISPLAY_WEEKS_MID:
	case WLC_DISPLAY_WEEKS_LONG:
	case WLC_DISPLAY_DAYS_SHORT:
	case WLC_DISPLAY_DAYS_MID:
	case WLC_DISPLAY_DAYS_LONG:
	case WLC_DISPLAY_HOURS:
		return CDateHelper::GetEndOfMonth(dtTemp);
	}

	ASSERT(0);
	return dtTemp;
}

int WORKLOADDATERANGE::Compare(const COleDateTime& date) const
{
	ASSERT(CDateHelper::IsDateSet(date) && IsValid());

	if (date < dtStart)
		return -1;

	if (date > dtEnd)
		return 1;

	// else
	return 0; // contains
}

BOOL WORKLOADDATERANGE::IsValid() const
{
	return (CDateHelper::IsDateSet(dtStart) && CDateHelper::IsDateSet(dtEnd) &&
			(dtStart <= dtEnd));
}

BOOL WORKLOADDATERANGE::IsEmpty() const
{
	ASSERT(IsValid());

	return (dtEnd == dtStart);
}

BOOL WORKLOADDATERANGE::Contains(const WORKLOADITEM& gi)
{
	return ((Compare(gi.dtStart) == 0) && (Compare(gi.dtDue) == 0));
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

