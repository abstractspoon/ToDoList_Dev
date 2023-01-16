// GanttStruct.cpp: implementation of the CGanttStruct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GanttStruct.h"
#include "GanttStatic.h"

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
	ptTemp.y = ptTo.y;
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
	dtRange = gi.dtRange;
	dtMinMaxRange = gi.dtMinMaxRange;
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
			(dtRange == gi.dtRange) &&
			(dtMinMaxRange == gi.dtMinMaxRange) &&
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

void GANTTITEM::MinMaxDates(const GANTTITEM& giOther, BOOL bCalcParentDates, BOOL bCalcMissingStart, BOOL bCalcMissingDue)
{
	COleDateTime dtStart, dtEnd;
	giOther.GetStartEndDates(bCalcParentDates, bCalcMissingStart, bCalcMissingDue, dtStart, dtEnd);

	dtMinMaxRange.Add(dtStart, dtEnd);
}

BOOL GANTTITEM::IsDone(BOOL bIncGoodAs) const
{
	if (CDateHelper::IsDateSet(dtDone))
		return TRUE;

	// else
	return (bIncGoodAs && bGoodAsDone);
}

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

COLORREF GANTTITEM::GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const
{
	if (HasColor())
	{
		if (bSelected)
			return GraphicsMisc::GetExplorerItemSelectionTextColor(color, GMIS_SELECTED, GMIB_THEMECLASSIC);

		if (bColorIsBkgnd && !IsDone(TRUE))
			return GraphicsMisc::GetBestTextColor(color);

		// else
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

COLORREF GANTTITEM::GetFillColor(BOOL /*bSelected*/) const
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

COLORREF GANTTITEM::GetBorderColor(BOOL bSelected) const
{
	if (bSelected && Misc::IsHighContrastActive())
	{
		return GetSysColor(COLOR_HIGHLIGHTTEXT);
	}
	else if (IsDone(TRUE))
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
		VERIFY(RemoveKey(dwTaskID));

		delete pGI;
		RemoveAllDependenciesOn(dwTaskID);

		return TRUE;
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

void CGanttItemMap::RemoveAllDependenciesOn(DWORD dwDependencyID)
{
	DWORD dwTaskID = 0;
	GANTTITEM* pGI = NULL;

	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pGI);
		ASSERT(pGI);

		Misc::RemoveItemT(dwDependencyID, pGI->aDependIDs);
	}
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

COleDateTime CGanttItemMap::CalcMaxDependencyDate(const GANTTITEM& gi) const
{
	COleDateTime dtMax;
	CDateHelper::ClearDate(dtMax);

	int nDepend = gi.aDependIDs.GetSize();

	while (nDepend--)
	{
		DWORD dwDependID = gi.aDependIDs[nDepend];
		ASSERT(dwDependID);

		if (HasItem(dwDependID))
		{
			const GANTTITEM* pGIDepends = GetItem(dwDependID, FALSE);
			ASSERT(pGIDepends);

			if (pGIDepends)
				CDateHelper::Max(dtMax, pGIDepends->dtRange.GetEnd());
		}
	}

	return dtMax;
}

void CGanttItemMap::CalcDateRange(BOOL bCalcParentDates, BOOL bCalcMissingStart, BOOL bCalcMissingDue, GANTTDATERANGE& dtRange)
{
	dtRange.Reset();

	POSITION pos = GetStartPosition();
	GANTTITEM* pGI = NULL;
	DWORD dwTaskID = 0;

	while (pos)
	{
		GetNextAssoc(pos, dwTaskID, pGI);
		ASSERT(pGI);

		if (pGI)
		{
			COleDateTime dtStart, dtEnd;
			pGI->GetStartEndDates(bCalcParentDates, bCalcMissingStart, bCalcMissingDue, dtStart, dtEnd);

			dtRange.Add(dtStart, dtEnd);
		}
	}
}

//////////////////////////////////////////////////////////////////////

GANTTDATERANGE::GANTTDATERANGE()
{
	m_bInclusive = FALSE; // always
}

GANTTDATERANGE::GANTTDATERANGE(const GANTTDATERANGE& dtOther)
{
	m_bInclusive = FALSE; // always

	if (dtOther.IsValid())
		Set(dtOther);
}

GANTTDATERANGE::GANTTDATERANGE(const COleDateTimeRange& dtOther)
{
	m_bInclusive = FALSE; // always

	if (dtOther.IsValid())
		Set(dtOther);
}

CString GANTTDATERANGE::Format(GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades, BOOL /*bISODates*/, TCHAR cDelim) const
{
	COleDateTime dtStart(GetStart(nDisplay, bZeroBasedDecades)), dtEnd(GetEnd(nDisplay, bZeroBasedDecades));

	CString sStart, sEnd;
	sStart.Format(_T("%s %d"), CDateHelper::GetMonthName(dtStart.GetMonth(), TRUE), dtStart.GetYear());
	sEnd.Format(_T("%s %d"), CDateHelper::GetMonthName(dtEnd.GetMonth(), TRUE), dtEnd.GetYear());

	CString sRange;
	sRange.Format(_T("%s %c %s"), sStart, cDelim, sEnd);

	return sRange;
}

BOOL GANTTDATERANGE::IsValid() const
{
	ASSERT(!m_bInclusive);

	return (COleDateTimeRange::IsValid() && !m_bInclusive);
}

void GANTTDATERANGE::Add(const GANTTITEM& gi)
{
	if (gi.IsDone(FALSE))
		Add(gi.dtRange.m_dtStart, gi.dtDone);
	else
		Add(gi.dtRange.m_dtStart, gi.dtRange.m_dtEnd);
}

void GANTTDATERANGE::Add(const COleDateTime& dtStart, const COleDateTime& dtEnd) 
{ 
	// bInclusive always FALSE
	CDateHelper::Min(m_dtStart, dtStart);
	CDateHelper::Max(m_dtEnd, dtEnd);
}

int GANTTDATERANGE::GetStartYear(GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades) const
{
	return GetStart(nDisplay, bZeroBasedDecades).GetYear();
}

int GANTTDATERANGE::GetEndYear(GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades) const
{
	int nYear = GetEnd(nDisplay, bZeroBasedDecades).GetYear();

	// for now, do not let end year exceed MAX_YEAR
	return min(nYear, MAX_YEAR);
}

int GANTTDATERANGE::GetNumMonths(GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades) const
{
	if (!IsValid())
		return 0;

	COleDateTime dtStart(GetStart(nDisplay, bZeroBasedDecades)), dtEnd(GetEnd(nDisplay, bZeroBasedDecades));

	int nNumMonths = CDateHelper::CalcMonthsFromTo(dtStart, dtEnd, TRUE);
	ASSERT(nNumMonths > 0);

	return nNumMonths;
}

COleDateTime GANTTDATERANGE::GetStart(GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades) const
{
	ASSERT(!m_bInclusive); // always

	COleDateTime dtTemp = COleDateTime::GetCurrentTime();

	if (CDateHelper::IsDateSet(m_dtStart))
		dtTemp = m_dtStart;

	return GanttStatic::GetRangeStart(dtTemp, nDisplay, bZeroBasedDecades);
}

COleDateTime GANTTDATERANGE::GetEnd(GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades) const
{
	ASSERT(!m_bInclusive); // always

	COleDateTime dtTemp = COleDateTime::GetCurrentTime();

	if (CDateHelper::IsDateSet(m_dtEnd))
		dtTemp = m_dtEnd;

	return GanttStatic::GetRangeEnd(dtTemp, nDisplay, bZeroBasedDecades);
}

BOOL GANTTDATERANGE::Contains(const GANTTITEM& gi) const
{
	ASSERT(IsValid());

	if (!gi.HasStartDate() || (gi.dtRange.m_dtStart < m_dtStart) || (gi.dtRange.m_dtStart > m_dtEnd))
		return FALSE;

	if (!gi.HasDueDate() || (gi.dtRange.m_dtEnd < m_dtStart) || (gi.dtRange.m_dtEnd > m_dtEnd))
		return FALSE;

	return TRUE;
}

BOOL GANTTDATERANGE::Contains(const GANTTDATERANGE& dtRange) const
{
	if (!IsValid() || !dtRange.IsValid())
	{
		ASSERT(0);
		return FALSE;
	}

	if ((dtRange.m_dtStart < m_dtStart) || (dtRange.m_dtStart > m_dtEnd))
		return FALSE;

	if ((dtRange.m_dtEnd < m_dtStart) || (dtRange.m_dtEnd > m_dtEnd))
		return FALSE;

	return TRUE;
}

BOOL GANTTDATERANGE::Contains(const COleDateTime& date) const
{
	return COleDateTimeRange::Contains(date);
}

BOOL GANTTDATERANGE::HasIntersection(const COleDateTimeRange& dtRange) const
{
	return COleDateTimeRange::HasIntersection(dtRange);
}

BOOL GANTTDATERANGE::IntersectWith(const GANTTDATERANGE& dtRange)
{
	if (!COleDateTimeRange::IntersectWith(dtRange))
		return FALSE;

	m_bInclusive = FALSE; // always
	return TRUE;
}

BOOL GANTTDATERANGE::operator==(const GANTTDATERANGE& dtOther) const
{
	return operator==((const COleDateTimeRange&)dtOther);
}

BOOL GANTTDATERANGE::operator!=(const GANTTDATERANGE& dtOther) const
{
	return !(*this == dtOther);
}

BOOL GANTTDATERANGE::operator==(const COleDateTimeRange& dtOther) const
{
	// return false if one or other is not valid,
	// but allow both being unset
	if (Misc::StateChanged(IsValid(), dtOther.IsValid()))
		return FALSE;

	ASSERT(!m_bInclusive && !dtOther.m_bInclusive); // always

	return ((m_dtStart == dtOther.m_dtStart) && (m_dtEnd == dtOther.m_dtEnd));
}

BOOL GANTTDATERANGE::operator!=(const COleDateTimeRange& dtOther) const
{
	return !(*this == dtOther);
}

void GANTTDATERANGE::Set(const GANTTDATERANGE& dtOther)
{
	ASSERT(!dtOther.m_bInclusive); // always
	
	SetStart(dtOther.GetStart());
	SetEnd(dtOther.GetEnd());
}

void GANTTDATERANGE::Set(const COleDateTimeRange& dtOther)
{
	ASSERT(!dtOther.m_bInclusive); // always
	
	SetStart(dtOther.GetStart());
	SetEnd(dtOther.GetEnd());
}

void GANTTDATERANGE::SetStart(const COleDateTime& date)
{
	ASSERT(!m_bInclusive); // always

	m_dtStart = date;
}

void GANTTDATERANGE::SetEnd(const COleDateTime& date)
{
	ASSERT(!m_bInclusive); // always

	if (!CDateHelper::DateHasTime(date))
		m_dtEnd = CDateHelper::GetEndOfDay(date);
	else
		m_dtEnd = date;
}

void GANTTDATERANGE::Set(const GANTTDATERANGE& dtOther, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades)
{
	ASSERT(!dtOther.m_bInclusive); // always
	
	SetStart(dtOther.GetStart(nDisplay, bZeroBasedDecades));
	SetEnd(dtOther.GetEnd(nDisplay, bZeroBasedDecades));
}

void GANTTDATERANGE::Set(const COleDateTimeRange& dtOther, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades)
{
	ASSERT(!dtOther.m_bInclusive); // always
	
	SetStart(GanttStatic::GetRangeStart(dtOther.GetStart(), nDisplay, bZeroBasedDecades));
	SetEnd(GanttStatic::GetRangeEnd(dtOther.GetEnd(), nDisplay, bZeroBasedDecades));
}

void GANTTDATERANGE::SetStart(const COleDateTime& date, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades)
{
	ASSERT(!m_bInclusive); // always

	SetStart(GanttStatic::GetRangeStart(date, nDisplay, bZeroBasedDecades));
}

void GANTTDATERANGE::SetEnd(const COleDateTime& date, GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades)
{
	ASSERT(!m_bInclusive); // always

	SetEnd(GanttStatic::GetRangeEnd(date, nDisplay, bZeroBasedDecades));
}

void GANTTDATERANGE::ClearStart()
{
	ASSERT(!m_bInclusive); // always

	CDateHelper::ClearDate(m_dtStart);
}

void GANTTDATERANGE::ClearEnd()
{
	ASSERT(!m_bInclusive); // always

	CDateHelper::ClearDate(m_dtEnd);
}

void GANTTDATERANGE::Reset()
{
	COleDateTimeRange::Reset();

	m_bInclusive = FALSE; // always
}

COleDateTime GANTTDATERANGE::GetStart() const 
{ 
	ASSERT(!m_bInclusive); // always

	return COleDateTimeRange::GetStart(); 
}

COleDateTime GANTTDATERANGE::GetEnd() const 
{ 
	ASSERT(!m_bInclusive); // always

	return COleDateTimeRange::GetEnd(); 
}

BOOL GANTTDATERANGE::HasStart() const 
{ 
	ASSERT(!m_bInclusive); // always

	return COleDateTimeRange::HasStart(); 
}

BOOL GANTTDATERANGE::HasEnd() const 
{ 
	ASSERT(!m_bInclusive); // always

	return COleDateTimeRange::HasEnd(); 
}

//////////////////////////////////////////////////////////////////////

GANTTSORTCOLUMN::GANTTSORTCOLUMN() : nBy(GTLCC_NONE), bAscending(-1)
{

}

BOOL GANTTSORTCOLUMN::Matches(GTLC_COLUMN nSortBy, BOOL bSortAscending) const
{
	return ((nBy == nSortBy) && (bAscending == bSortAscending));
}

BOOL GANTTSORTCOLUMN::operator==(const GANTTSORTCOLUMN& other) const
{
	return Matches(other.nBy, other.bAscending);
}

BOOL GANTTSORTCOLUMN::operator!=(const GANTTSORTCOLUMN& other) const
{
	return !(*this == other);
}

BOOL GANTTSORTCOLUMN::Set(GTLC_COLUMN nSortBy, BOOL bAllowToggle, BOOL bSortAscending)
{
	if (!bAllowToggle && Matches(nSortBy, bSortAscending))
		return FALSE;

	GTLC_COLUMN nOldSort = nBy;
	nBy = nSortBy;

	if (nSortBy != GTLCC_NONE)
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

GANTTSORTCOLUMNS::GANTTSORTCOLUMNS()
{

}

BOOL GANTTSORTCOLUMNS::Set(const GANTTSORTCOLUMNS& sort)
{
	if (*this == sort)
		return FALSE;

	for (int nCol = 0; nCol < 3; nCol++)
		cols[nCol] = sort.cols[nCol];

	return TRUE;
}

BOOL GANTTSORTCOLUMNS::operator==(const GANTTSORTCOLUMNS& other) const
{
	for (int nCol = 0; nCol < 3; nCol++)
	{
		if (cols[nCol] != other.cols[nCol])
			return FALSE;
	}

	// else
	return TRUE;
}

BOOL GANTTSORTCOLUMNS::operator!=(const GANTTSORTCOLUMNS& other) const
{
	return !(*this == other);
}

/////////////////////////////////////////////////////////////////////////////

GANTTSORT::GANTTSORT() : bMultiSort(FALSE)
{

}

BOOL GANTTSORT::IsSorting() const
{
	if (!bMultiSort)
		return (single.nBy != GTLCC_NONE);

	// else
	return (multi.cols[0].nBy != GTLCC_NONE);
}

BOOL GANTTSORT::IsSortingBy(GTLC_COLUMN nColID) const
{
	if (!bMultiSort)
		return IsSingleSortingBy(nColID);

	return IsMultiSortingBy(nColID);
}

BOOL GANTTSORT::IsSingleSortingBy(GTLC_COLUMN nColID) const
{
	return (!bMultiSort && (single.nBy == nColID));
}

BOOL GANTTSORT::IsMultiSortingBy(GTLC_COLUMN nColID) const
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

BOOL GANTTSORT::Set(GTLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending)
{
	if (bMultiSort)
	{
		bMultiSort = FALSE;
		return single.Set(nBy, FALSE, bAscending);
	}

	return single.Set(nBy, bAllowToggle, bAscending);
}

BOOL GANTTSORT::Set(const GANTTSORTCOLUMNS& sort)
{
	bMultiSort = TRUE;
	return multi.Set(sort);
}

/////////////////////////////////////////////////////////////////////////////

