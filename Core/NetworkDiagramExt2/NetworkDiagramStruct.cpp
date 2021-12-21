// GanttStruct.cpp: implementation of the CGanttStruct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetworkDiagramStruct.h"
#include "NetworkDiagramStatic.h"

#include "..\shared\DateHelper.h"
#include "..\shared\graphicsMisc.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
const int MAX_YEAR = 2100;
#else
const int MAX_YEAR = 2200;
#endif

//////////////////////////////////////////////////////////////////////

int GANTTDEPENDENCY::STUB = 0;

GANTTDEPENDENCY::GANTTDEPENDENCY() 
	: 
	ptFrom(0), 
	ptTo(0), 
	dwFromID(0), 
	dwToID(0)
{
}

void GANTTDEPENDENCY::SetFrom(const CPoint& pt, DWORD dwTaskID)
{
	dwFromID = dwTaskID;
	ptFrom = pt;
}

void GANTTDEPENDENCY::SetTo(const CPoint& pt, DWORD dwTaskID)
{
	dwToID = dwTaskID;
	ptTo = pt;
}

DWORD GANTTDEPENDENCY::GetFromID() const
{
	ASSERT(dwFromID);
	return dwFromID;
}

DWORD GANTTDEPENDENCY::GetToID() const
{
	ASSERT(dwToID);
	return dwToID;
}

BOOL GANTTDEPENDENCY::Matches(DWORD dwFrom, DWORD dwTo) const
{
	ASSERT(dwFromID && dwToID);
	return ((dwFromID == dwFrom) && (dwToID == dwTo));
}

BOOL GANTTDEPENDENCY::Draw(CDC* pDC, const CRect& rClient, BOOL bDragging)
{
	if (!HitTest(rClient))
		return FALSE;
	
	// draw 3x3 box at ptTo
	if (!bDragging)
	{
		CRect rBox(ptTo.x - 1, ptTo.y - 1, ptTo.x + 2, ptTo.y + 2);
		pDC->FillSolidRect(rBox, 0);
	}
		
	CPoint pts[3];
	CalcDependencyPath(pts);
	
	int nOldROP2 = pDC->SetROP2(bDragging ? R2_NOT : R2_BLACK);
	pDC->Polyline(pts, 3);

	DrawDependencyArrow(pDC, pts[0]);
	pDC->SetROP2(nOldROP2);
	
	return TRUE;
}

#ifdef _DEBUG
void GANTTDEPENDENCY::Trace() const
{
	TRACE(_T("GANTTDEPENDENCY(from %d (pos = %d) to %d (pos = %d))\n"), dwFromID, ptFrom.x, dwToID, ptTo.x);
}
#endif

void GANTTDEPENDENCY::DrawDependencyArrow(CDC* pDC, const CPoint& pt) const
{
	CPoint pts[3], ptArrow(pt);

	CalcDependencyArrow(ptArrow, pts);
	pDC->Polyline(pts, 3);
	
	// offset and draw again
	if (IsFromAboveTo())
		ptArrow.y++;
	else
		ptArrow.y--;
	
	CalcDependencyArrow(ptArrow, pts);
	pDC->Polyline(pts, 3);
}

BOOL GANTTDEPENDENCY::HitTest(const CRect& rect) const
{
	CRect rThis;
	return (CalcBoundingRect(rThis) && CRect().IntersectRect(rect, rThis));
}

BOOL GANTTDEPENDENCY::HitTest(const CPoint& point, int nTol) const
{
	CRect rThis;
	
	if (!CalcBoundingRect(rThis))
		return FALSE;

	// add tolerance
	rThis.InflateRect(nTol, nTol);

	if (!rThis.PtInRect(point))
		return FALSE;

	// check each line segment
	CPoint pts[3];
	CalcDependencyPath(pts);

	nTol = max(nTol, 1);
	
	for (int i = 0; i < 2; i++)
	{
		CRect rSeg;

		rSeg.left	= min(pts[i].x, pts[i+1].x) - nTol;
		rSeg.right	= max(pts[i].x, pts[i+1].x) + nTol;
		rSeg.top	= min(pts[i].y, pts[i+1].y) - nTol;
		rSeg.bottom = max(pts[i].y, pts[i+1].y) + nTol;

		if (rSeg.PtInRect(point))
			return TRUE;
	}

	// no hit
	return FALSE;
}

BOOL GANTTDEPENDENCY::IsFromAboveTo() const
{
	return (ptFrom.y < ptTo.y);
}

void GANTTDEPENDENCY::CalcDependencyPath(CPoint pts[3]) const
{
	CPoint ptTemp(ptFrom);

	// first point
	if (IsFromAboveTo())
		ptTemp.y -= (2 - STUB);
	else
		ptTemp.y += (1 - STUB);

	pts[0] = ptTemp;

	// mid point
//	ptTemp.y = ptTo.y;
	pts[1] = ptTemp;

	// last point
	pts[2] = ptTo;
}

void GANTTDEPENDENCY::CalcDependencyArrow(const CPoint& pt, CPoint pts[3]) const
{
	pts[0] = pts[1] = pts[2] = pt;

	const int ARROW = (STUB / 2);
	
	if (IsFromAboveTo())
	{
		pts[0].Offset(-ARROW, ARROW);
		pts[2].Offset(ARROW+1, ARROW+1);
	}
	else
	{
		pts[0].Offset(-ARROW, -ARROW);
		pts[2].Offset(ARROW+1, -(ARROW+1));
	}
}

BOOL GANTTDEPENDENCY::CalcBoundingRect(CRect& rect) const
{
	if (ptFrom == ptTo)
		return FALSE;
	
	// allow for stub overhang
	rect.left	= min(ptFrom.x, ptTo.x) - STUB;
	rect.right	= max(ptFrom.x, ptTo.x) + STUB;
	rect.top	= min(ptFrom.y, ptTo.y);
	rect.bottom = max(ptFrom.y, ptTo.y);
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////

GANTTITEM::GANTTITEM() 
	: 
	color(CLR_NONE), 
	bParent(FALSE), 
	dwTaskID(0), 
	dwRefID(0), 
	dwOrgRefID(0), 
	bGoodAsDone(FALSE),
	nPosition(-1),
	bLocked(FALSE),
	bSomeSubtaskDone(FALSE),
	nPercent(0)
{
}

GANTTITEM::GANTTITEM(const GANTTITEM& gi)
{
	*this = gi;
}

GANTTITEM& GANTTITEM::operator=(const GANTTITEM& gi)
{
	sTitle = gi.sTitle;
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

GANTTITEM::~GANTTITEM()
{
	
}

BOOL GANTTITEM::operator==(const GANTTITEM& gi) const
{
	return ((sTitle == gi.sTitle) &&
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

BOOL GANTTITEM::operator!=(const GANTTITEM& gi) const
{
	return !(*this == gi);
}

BOOL GANTTITEM::IsDone(BOOL bIncGoodAs) const
{
	if (CDateHelper::IsDateSet(dtDone))
		return TRUE;

	// else
	return (bIncGoodAs && bGoodAsDone);
}

/*
BOOL GANTTITEM::HasStartDate() const
{
	return dtRange.HasStart();
}

BOOL GANTTITEM::HasDueDate() const
{
	return dtRange.HasEnd();
}

BOOL GANTTITEM::HasDoneDate(BOOL bCalcParentDates) const
{
	if (bParent && bCalcParentDates)
		return FALSE;

	// else
	return CDateHelper::IsDateSet(dtDone);
}
*/

COLORREF GANTTITEM::GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const
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

COLORREF GANTTITEM::GetTextBkColor(BOOL bSelected, BOOL bColorIsBkgnd) const
{
	if (!bSelected && HasColor())
	{
		if (bColorIsBkgnd && !IsDone(TRUE))
			return color;
	}
	
	// else
	return CLR_NONE;
}

COLORREF GANTTITEM::GetFillColor() const
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

COLORREF GANTTITEM::GetBorderColor() const
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

BOOL GANTTITEM::HasColor() const
{
	return ((color != CLR_NONE) && (color != GetSysColor(COLOR_WINDOWTEXT)));
}

COleDateTime GANTTITEM::GetDate(time64_t tDate, BOOL bEndOfDay)
{
	COleDateTime date = CDateHelper::GetDate(tDate);

	// only implement 'end of day' if the date has no time
	if (CDateHelper::IsDateSet(date) && bEndOfDay && !CDateHelper::DateHasTime(date))
		date = CDateHelper::GetEndOfDay(date);

	return date;
}

/*
void GANTTITEM::SetStartDate(time64_t tDate, BOOL bAndMinMax)
{
	SetStartDate(GetDate(tDate, FALSE), bAndMinMax);
}

void GANTTITEM::SetDueDate(time64_t tDate, BOOL bAndMinMax)
{
	SetDueDate(GetDate(tDate, TRUE), bAndMinMax);
}

void GANTTITEM::SetStartDate(const COleDateTime& date, BOOL bAndMinMax)
{
	dtRange.SetStart(date);

	if (bAndMinMax)
		dtMinMaxRange.SetStart(date);
}

void GANTTITEM::SetDueDate(const COleDateTime& date, BOOL bAndMinMax)
{
	dtRange.SetEnd(date);

	if (bAndMinMax)
		dtMinMaxRange.SetEnd(date);
}

void GANTTITEM::SetDoneDate(time64_t tDate)
{
	dtDone = GetDate(tDate, TRUE);
}

void GANTTITEM::ClearStartDate(BOOL bAndMinMax)
{
	dtRange.ClearStart();

	if (bAndMinMax)
		dtMinMaxRange.ClearStart();
}

void GANTTITEM::ClearDueDate(BOOL bAndMinMax)
{
	dtRange.ClearEnd();

	if (bAndMinMax)
		dtMinMaxRange.ClearStart();
}

void GANTTITEM::ClearDoneDate()
{
	CDateHelper::ClearDate(dtDone);
}

BOOL GANTTITEM::GetStartEndDates(BOOL bCalcParentDates, BOOL bCalcMissingStart, BOOL bCalcMissingDue, COleDateTime& dtStart, COleDateTime& dtDue) const
{
	BOOL bDoneSet = FALSE;

	if (bParent && bCalcParentDates)
	{
		dtStart = dtMinMaxRange.GetStart();
		dtDue = dtMinMaxRange.GetEnd();
	}
	else
	{
		dtStart = dtRange.GetStart();
		dtDue = dtRange.GetEnd();

		bDoneSet = CDateHelper::IsDateSet(dtDone);

		// do we need to calculate due date?
		if (!CDateHelper::IsDateSet(dtDue) && bCalcMissingDue)
		{
			// always take completed date if that is set
			if (bDoneSet)
			{
				dtDue = dtDone;
			}
			else // take later of start date and today
			{
				dtDue = CDateHelper::GetDateOnly(dtStart);
				CDateHelper::Max(dtDue, CDateHelper::GetDate(DHD_TODAY));
	
				// and move to end of the day
				dtDue = CDateHelper::GetEndOfDay(dtDue);
			}
	
			ASSERT(CDateHelper::IsDateSet(dtDue));
		}
	
		// do we need to calculate start date?
		if (!CDateHelper::IsDateSet(dtStart) && bCalcMissingStart)
		{
			// take earlier of due or completed date
			dtStart = CDateHelper::GetDateOnly(dtDue);
			CDateHelper::Min(dtStart, CDateHelper::GetDateOnly(dtDone));
	
			// take the earlier of that and 'today'
			CDateHelper::Min(dtStart, CDateHelper::GetDate(DHD_TODAY));
	
			ASSERT(CDateHelper::IsDateSet(dtStart));
		}
	}

	return (CDateHelper::IsDateSet(dtStart) && 
			CDateHelper::IsDateSet(dtDue) &&
			(dtDue >= dtStart));
}

BOOL GANTTITEM::IsMilestone(const CString& sMilestoneTag) const
{
	if (sMilestoneTag.IsEmpty() || (aTags.GetSize() == 0))
		return FALSE;

	if (!bParent && !dtRange.HasEnd())
		return FALSE;

	if (bParent && !dtMinMaxRange.HasEnd())
		return FALSE;

	// else
	return Misc::Contains(sMilestoneTag, aTags, FALSE, TRUE);
}
*/

//////////////////////////////////////////////////////////////////////

CGanttItemMap::~CGanttItemMap()
{
	RemoveAll();
}

BOOL CGanttItemMap::ItemIsLocked(DWORD dwTaskID, BOOL bTreatRefsAsUnlocked) const
{
	const GANTTITEM* pGI = GetItem(dwTaskID, TRUE);

	return (pGI && pGI->bLocked && (!bTreatRefsAsUnlocked || !pGI->dwOrgRefID));
}

BOOL CGanttItemMap::ItemIsReference(DWORD dwTaskID) const
{
	const GANTTITEM* pGI = GetItem(dwTaskID, TRUE); 

	return (pGI && pGI->dwOrgRefID);
}

BOOL CGanttItemMap::ItemIsDone(DWORD dwTaskID, BOOL bIncGoodAs) const
{
	const GANTTITEM* pGI = GetItem(dwTaskID, TRUE);

	return (pGI && pGI->IsDone(bIncGoodAs));
}

BOOL CGanttItemMap::ItemHasDependecies(DWORD dwTaskID) const
{
	const GANTTITEM* pGI = GetItem(dwTaskID, TRUE);

	return (pGI && pGI->aDependIDs.GetSize());
}

void CGanttItemMap::RemoveAll()
{
	DWORD dwTaskID = 0;
	GANTTITEM* pGI = NULL;
	
	POSITION pos = GetStartPosition();
	
	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pGI);
		ASSERT(pGI);
		
		delete pGI;
	}
	
	CMap<DWORD, DWORD, GANTTITEM*, GANTTITEM*&>::RemoveAll();
}

BOOL CGanttItemMap::DeleteItem(DWORD dwTaskID)
{
	GANTTITEM* pGI = NULL;
	
	if (Lookup(dwTaskID, pGI))
	{
		delete pGI;
		return CMap<DWORD, DWORD, GANTTITEM*, GANTTITEM*&>::RemoveKey(dwTaskID);
	}
	
	// else
	return FALSE;
}

BOOL CGanttItemMap::HasItem(DWORD dwTaskID) const
{
	if (dwTaskID == 0)
		return FALSE;

	GANTTITEM* pGIUnused = NULL;
	
	return Lookup(dwTaskID, pGIUnused);
}

GANTTITEM* CGanttItemMap::GetItem(DWORD dwTaskID, BOOL bResolveReferences) const
{
	if (dwTaskID == 0)
		return NULL;

	GANTTITEM* pGI = NULL;

	if (!Lookup(dwTaskID, pGI) || !pGI)
	{
		ASSERT(pGI);
		return NULL;
	}

	// Resolves references
	pGI->dwOrgRefID = 0;

	if (pGI && pGI->dwRefID && bResolveReferences)
	{
		ASSERT(pGI->dwOrgRefID == 0);
		ASSERT(pGI->dwRefID != dwTaskID);

		DWORD dwRefID = pGI->dwRefID;

		if ((dwRefID != dwTaskID) && Lookup(dwRefID, pGI))
		{
			// copy over the reference id so that the caller can still detect it
			ASSERT(pGI->dwRefID == 0);

			pGI->dwOrgRefID = dwTaskID;
			pGI->dwRefID = 0;
		}
	}

	return pGI;
}

BOOL CGanttItemMap::RestoreItem(const GANTTITEM& giPrev)
{
	GANTTITEM* pGI = NULL;

	if (Lookup(giPrev.dwTaskID, pGI) && pGI)
	{
		*pGI = giPrev;
		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CGanttItemMap::IsItemDependentOn(const GANTTITEM& gi, DWORD dwOtherID) const
{
	int nDepend = gi.aDependIDs.GetSize();

	while (nDepend--)
	{
		DWORD dwDependID = gi.aDependIDs[nDepend];
		ASSERT(dwDependID);

		if (dwDependID == dwOtherID)
			return TRUE;

		// else check dependents of dwDependID
		const GANTTITEM* pGIDepends = GetItem(dwDependID, FALSE);
		ASSERT(pGIDepends);

		if (pGIDepends && IsItemDependentOn(*pGIDepends, dwOtherID)) // RECURSIVE CALL
			return TRUE;
	}
	
	// all else
	return FALSE;
}

int CGanttItemMap::BuildDependencyChainLengths(CMap<DWORD, DWORD, int, int>& mapLengths) const
{
	mapLengths.RemoveAll();

	POSITION pos = GetStartPosition();
	GANTTITEM* pGIUnused = NULL;
	DWORD dwTaskID = 0;

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pGIUnused);
		ASSERT(pGIUnused);

		mapLengths[dwTaskID] = CalcMaxDependencyChainLength(dwTaskID);
	}

	return mapLengths.GetCount();
}

int CGanttItemMap::CalcMaxDependencyChainLength(DWORD dwTaskID) const
{
	const GANTTITEM* pGI = GetItem(dwTaskID, FALSE);
	ASSERT(pGI);

	if (!pGI || !pGI->aDependIDs.GetSize())
		return 0;

	int nDepend = pGI->aDependIDs.GetSize();
	int nMaxDependLen = 0;

	while (nDepend--)
	{
		DWORD dwDependID = pGI->aDependIDs[nDepend];
		ASSERT(dwDependID);

		int nDependLen = CalcMaxDependencyChainLength(dwDependID);
		nMaxDependLen = max(nMaxDependLen, nDependLen);
	}

	return nMaxDependLen + 1; // the dependency itself
}

