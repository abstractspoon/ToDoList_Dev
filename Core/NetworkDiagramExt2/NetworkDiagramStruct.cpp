// NetworkDiagramStruct.cpp: implementation of the NetworkDiagramStruct classes.
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

int NETWORKDEPENDENCY::STUB = 0;

NETWORKDEPENDENCY::NETWORKDEPENDENCY() 
	: 
	ptFrom(0), 
	ptTo(0), 
	dwFromID(0), 
	dwToID(0)
{
}

void NETWORKDEPENDENCY::SetFrom(const CPoint& pt, DWORD dwTaskID)
{
	dwFromID = dwTaskID;
	ptFrom = pt;
}

void NETWORKDEPENDENCY::SetTo(const CPoint& pt, DWORD dwTaskID)
{
	dwToID = dwTaskID;
	ptTo = pt;
}

DWORD NETWORKDEPENDENCY::GetFromID() const
{
	ASSERT(dwFromID);
	return dwFromID;
}

DWORD NETWORKDEPENDENCY::GetToID() const
{
	ASSERT(dwToID);
	return dwToID;
}

BOOL NETWORKDEPENDENCY::Matches(DWORD dwFrom, DWORD dwTo) const
{
	ASSERT(dwFromID && dwToID);
	return ((dwFromID == dwFrom) && (dwToID == dwTo));
}

BOOL NETWORKDEPENDENCY::Draw(CDC* pDC, const CRect& rClient, BOOL bDragging)
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
void NETWORKDEPENDENCY::Trace() const
{
	TRACE(_T("NETWORKDEPENDENCY(from %d (pos = %d) to %d (pos = %d))\n"), dwFromID, ptFrom.x, dwToID, ptTo.x);
}
#endif

void NETWORKDEPENDENCY::DrawDependencyArrow(CDC* pDC, const CPoint& pt) const
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

BOOL NETWORKDEPENDENCY::HitTest(const CRect& rect) const
{
	CRect rThis;
	return (CalcBoundingRect(rThis) && CRect().IntersectRect(rect, rThis));
}

BOOL NETWORKDEPENDENCY::HitTest(const CPoint& point, int nTol) const
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

BOOL NETWORKDEPENDENCY::IsFromAboveTo() const
{
	return (ptFrom.y < ptTo.y);
}

void NETWORKDEPENDENCY::CalcDependencyPath(CPoint pts[3]) const
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

void NETWORKDEPENDENCY::CalcDependencyArrow(const CPoint& pt, CPoint pts[3]) const
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

BOOL NETWORKDEPENDENCY::CalcBoundingRect(CRect& rect) const
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

NETWORKITEM::NETWORKITEM() 
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

NETWORKITEM::NETWORKITEM(const NETWORKITEM& gi)
{
	*this = gi;
}

NETWORKITEM& NETWORKITEM::operator=(const NETWORKITEM& gi)
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

NETWORKITEM::~NETWORKITEM()
{
	
}

BOOL NETWORKITEM::operator==(const NETWORKITEM& gi) const
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

BOOL NETWORKITEM::operator!=(const NETWORKITEM& gi) const
{
	return !(*this == gi);
}

BOOL NETWORKITEM::IsDone(BOOL bIncGoodAs) const
{
	if (CDateHelper::IsDateSet(dtDone))
		return TRUE;

	// else
	return (bIncGoodAs && bGoodAsDone);
}

/*
BOOL NETWORKITEM::HasStartDate() const
{
	return dtRange.HasStart();
}

BOOL NETWORKITEM::HasDueDate() const
{
	return dtRange.HasEnd();
}

BOOL NETWORKITEM::HasDoneDate(BOOL bCalcParentDates) const
{
	if (bParent && bCalcParentDates)
		return FALSE;

	// else
	return CDateHelper::IsDateSet(dtDone);
}
*/

COLORREF NETWORKITEM::GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const
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

COLORREF NETWORKITEM::GetTextBkColor(BOOL bSelected, BOOL bColorIsBkgnd) const
{
	if (!bSelected && HasColor())
	{
		if (bColorIsBkgnd && !IsDone(TRUE))
			return color;
	}
	
	// else
	return CLR_NONE;
}

COLORREF NETWORKITEM::GetFillColor() const
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

COLORREF NETWORKITEM::GetBorderColor() const
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

BOOL NETWORKITEM::HasColor() const
{
	return ((color != CLR_NONE) && (color != GetSysColor(COLOR_WINDOWTEXT)));
}

COleDateTime NETWORKITEM::GetDate(time64_t tDate, BOOL bEndOfDay)
{
	COleDateTime date = CDateHelper::GetDate(tDate);

	// only implement 'end of day' if the date has no time
	if (CDateHelper::IsDateSet(date) && bEndOfDay && !CDateHelper::DateHasTime(date))
		date = CDateHelper::GetEndOfDay(date);

	return date;
}

/*
void NETWORKITEM::SetStartDate(time64_t tDate, BOOL bAndMinMax)
{
	SetStartDate(GetDate(tDate, FALSE), bAndMinMax);
}

void NETWORKITEM::SetDueDate(time64_t tDate, BOOL bAndMinMax)
{
	SetDueDate(GetDate(tDate, TRUE), bAndMinMax);
}

void NETWORKITEM::SetStartDate(const COleDateTime& date, BOOL bAndMinMax)
{
	dtRange.SetStart(date);

	if (bAndMinMax)
		dtMinMaxRange.SetStart(date);
}

void NETWORKITEM::SetDueDate(const COleDateTime& date, BOOL bAndMinMax)
{
	dtRange.SetEnd(date);

	if (bAndMinMax)
		dtMinMaxRange.SetEnd(date);
}

void NETWORKITEM::SetDoneDate(time64_t tDate)
{
	dtDone = GetDate(tDate, TRUE);
}

void NETWORKITEM::ClearStartDate(BOOL bAndMinMax)
{
	dtRange.ClearStart();

	if (bAndMinMax)
		dtMinMaxRange.ClearStart();
}

void NETWORKITEM::ClearDueDate(BOOL bAndMinMax)
{
	dtRange.ClearEnd();

	if (bAndMinMax)
		dtMinMaxRange.ClearStart();
}

void NETWORKITEM::ClearDoneDate()
{
	CDateHelper::ClearDate(dtDone);
}

BOOL NETWORKITEM::GetStartEndDates(BOOL bCalcParentDates, BOOL bCalcMissingStart, BOOL bCalcMissingDue, COleDateTime& dtStart, COleDateTime& dtDue) const
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

BOOL NETWORKITEM::IsMilestone(const CString& sMilestoneTag) const
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

CNetworkItemMap::~CNetworkItemMap()
{
	RemoveAll();
}

BOOL CNetworkItemMap::ItemIsLocked(DWORD dwTaskID, BOOL bTreatRefsAsUnlocked) const
{
	const NETWORKITEM* pNI = GetItem(dwTaskID, TRUE);

	return (pNI && pNI->bLocked && (!bTreatRefsAsUnlocked || !pNI->dwOrgRefID));
}

BOOL CNetworkItemMap::ItemIsReference(DWORD dwTaskID) const
{
	const NETWORKITEM* pNI = GetItem(dwTaskID, TRUE); 

	return (pNI && pNI->dwOrgRefID);
}

BOOL CNetworkItemMap::ItemIsDone(DWORD dwTaskID, BOOL bIncGoodAs) const
{
	const NETWORKITEM* pNI = GetItem(dwTaskID, TRUE);

	return (pNI && pNI->IsDone(bIncGoodAs));
}

BOOL CNetworkItemMap::ItemHasDependecies(DWORD dwTaskID) const
{
	const NETWORKITEM* pNI = GetItem(dwTaskID, TRUE);

	return (pNI && pNI->aDependIDs.GetSize());
}

void CNetworkItemMap::RemoveAll()
{
	DWORD dwTaskID = 0;
	NETWORKITEM* pNI = NULL;
	
	POSITION pos = GetStartPosition();
	
	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pNI);
		ASSERT(pNI);
		
		delete pNI;
	}
	
	CMap<DWORD, DWORD, NETWORKITEM*, NETWORKITEM*&>::RemoveAll();
}

BOOL CNetworkItemMap::DeleteItem(DWORD dwTaskID)
{
	NETWORKITEM* pNI = NULL;
	
	if (Lookup(dwTaskID, pNI))
	{
		delete pNI;
		return CMap<DWORD, DWORD, NETWORKITEM*, NETWORKITEM*&>::RemoveKey(dwTaskID);
	}
	
	// else
	return FALSE;
}

BOOL CNetworkItemMap::HasItem(DWORD dwTaskID) const
{
	if (dwTaskID == 0)
		return FALSE;

	NETWORKITEM* pNIUnused = NULL;
	
	return Lookup(dwTaskID, pNIUnused);
}

NETWORKITEM* CNetworkItemMap::GetItem(DWORD dwTaskID, BOOL bResolveReferences) const
{
	if (dwTaskID == 0)
		return NULL;

	NETWORKITEM* pNI = NULL;

	if (!Lookup(dwTaskID, pNI) || !pNI)
	{
		ASSERT(pNI);
		return NULL;
	}

	// Resolves references
	pNI->dwOrgRefID = 0;

	if (pNI && pNI->dwRefID && bResolveReferences)
	{
		ASSERT(pNI->dwOrgRefID == 0);
		ASSERT(pNI->dwRefID != dwTaskID);

		DWORD dwRefID = pNI->dwRefID;

		if ((dwRefID != dwTaskID) && Lookup(dwRefID, pNI))
		{
			// copy over the reference id so that the caller can still detect it
			ASSERT(pNI->dwRefID == 0);

			pNI->dwOrgRefID = dwTaskID;
			pNI->dwRefID = 0;
		}
	}

	return pNI;
}

DWORD CNetworkItemMap::GetNextTaskID(POSITION& pos) const
{
	DWORD dwTaskID = 0;
	NETWORKITEM* pNI = NULL;

	GetNextAssoc(pos, dwTaskID, pNI);

	return dwTaskID;
}

BOOL CNetworkItemMap::RestoreItem(const NETWORKITEM& giPrev)
{
	NETWORKITEM* pNI = NULL;

	if (Lookup(giPrev.dwTaskID, pNI) && pNI)
	{
		*pNI = giPrev;
		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CNetworkItemMap::IsItemDependentOn(const NETWORKITEM& gi, DWORD dwOtherID) const
{
	int nDepend = gi.aDependIDs.GetSize();

	while (nDepend--)
	{
		DWORD dwDependID = gi.aDependIDs[nDepend];
		ASSERT(dwDependID);

		if (dwDependID == dwOtherID)
			return TRUE;

		// else check dependents of dwDependID
		const NETWORKITEM* pNIDepends = GetItem(dwDependID, FALSE);
		ASSERT(pNIDepends);

		if (pNIDepends && IsItemDependentOn(*pNIDepends, dwOtherID)) // RECURSIVE CALL
			return TRUE;
	}
	
	// all else
	return FALSE;
}

int CNetworkItemMap::BuildDependencyChainLengths(CMap<DWORD, DWORD, int, int>& mapLengths) const
{
	mapLengths.RemoveAll();

	POSITION pos = GetStartPosition();
	NETWORKITEM* pNIUnused = NULL;
	DWORD dwTaskID = 0;

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pNIUnused);
		ASSERT(pNIUnused);

		mapLengths[dwTaskID] = CalcMaxDependencyChainLength(dwTaskID);
	}

	return mapLengths.GetCount();
}

int CNetworkItemMap::CalcMaxDependencyChainLength(DWORD dwTaskID) const
{
	const NETWORKITEM* pNI = GetItem(dwTaskID, FALSE);
	ASSERT(pNI);

	if (!pNI || !pNI->aDependIDs.GetSize())
		return 0;

	int nDepend = pNI->aDependIDs.GetSize();
	int nMaxDependLen = 0;

	while (nDepend--)
	{
		DWORD dwDependID = pNI->aDependIDs[nDepend];
		ASSERT(dwDependID);

		int nDependLen = CalcMaxDependencyChainLength(dwDependID);
		nMaxDependLen = max(nMaxDependLen, nDependLen);
	}

	return nMaxDependLen + 1; // the dependency itself
}

