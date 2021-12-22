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
	dwTaskID(0), 
	bGoodAsDone(FALSE),
	bLocked(FALSE),
	bSomeSubtaskDone(FALSE),
	nPercent(0)
{
}

NETWORKITEM::NETWORKITEM(const NETWORKITEM& ni)
{
	*this = ni;
}

NETWORKITEM& NETWORKITEM::operator=(const NETWORKITEM& ni)
{
	sTitle = ni.sTitle;
	dtDone = ni.dtDone;
	color = ni.color;
	dwTaskID = ni.dwTaskID;
	nPercent = ni.nPercent;
	bGoodAsDone = ni.bGoodAsDone;
	bLocked = ni.bLocked;
	bHasIcon = ni.bHasIcon;
	bSomeSubtaskDone = ni.bSomeSubtaskDone;
	sAllocTo = ni.sAllocTo;
	
	aTags.Copy(ni.aTags);
	aDependIDs.Copy(ni.aDependIDs);
	
	return (*this);
}

NETWORKITEM::~NETWORKITEM()
{
	
}

BOOL NETWORKITEM::operator==(const NETWORKITEM& ni) const
{
	return ((sTitle == ni.sTitle) &&
			(dtDone == ni.dtDone) &&
			(color == ni.color) &&
			(sAllocTo == ni.sAllocTo) &&
			(dwTaskID == ni.dwTaskID) &&
			(nPercent == ni.nPercent) &&	
			(bGoodAsDone == ni.bGoodAsDone) &&
			(bLocked == ni.bLocked) &&
			(bHasIcon == ni.bHasIcon) &&
			(bSomeSubtaskDone == ni.bSomeSubtaskDone) &&
			Misc::MatchAll(aTags, ni.aTags) &&
			Misc::MatchAll(aDependIDs, ni.aDependIDs));
}

BOOL NETWORKITEM::operator!=(const NETWORKITEM& ni) const
{
	return !(*this == ni);
}

BOOL NETWORKITEM::Set(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	if (hTask == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	// Only interested in non-references
	if (pTasks->GetTaskReferenceID(hTask))
	{
		ASSERT(0);
		return FALSE;
	}

	dwTaskID = pTasks->GetTaskID(hTask);
	color = pTasks->GetTaskTextColor(hTask);
	sTitle = pTasks->GetTaskTitle(hTask);
	bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);
	nPercent = pTasks->GetTaskPercentDone(hTask, TRUE);
	bLocked = pTasks->IsTaskLocked(hTask, true);
	bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

	LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
	bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));

	time64_t tDate = 0;

	if (pTasks->GetTaskStartDate64(hTask, false, tDate))
		dtStart = CDateHelper::GetDate(tDate);

	if (pTasks->GetTaskDueDate64(hTask, false, tDate))
		dtDue = CDateHelper::GetDate(tDate);

	if (pTasks->GetTaskDoneDate64(hTask, tDate))
		dtDone = CDateHelper::GetDate(tDate);

	int nTag = pTasks->GetTaskTagCount(hTask);

	while (nTag--)
		aTags.Add(pTasks->GetTaskTag(hTask, nTag));

	CStringArray aAllocTo;
	int nAllocTo = pTasks->GetTaskAllocatedToCount(hTask);

	while (nAllocTo--)
		aAllocTo.Add(pTasks->GetTaskAllocatedTo(hTask, nAllocTo));

	sAllocTo = Misc::FormatArray(aAllocTo);

	// Local dependencies only
	int nDepend = pTasks->GetTaskDependencyCount(hTask);

	while (nDepend--)
	{
		DWORD dwDependID = _ttoi(pTasks->GetTaskDependency(hTask, nDepend));

		if (dwDependID)
			aDependIDs.Add(dwDependID);
	}
}

BOOL NETWORKITEM::Update(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	if (hTask == NULL)
		return FALSE;

	// Only interested in non-references
	if (pTasks->GetTaskReferenceID(hTask))
	{
		ASSERT(0);
		return FALSE;
	}

	// Take a snapshot we can check changes against
	NETWORKITEM niOrg = *this;

	// Update colour for all tasks
	color = pTasks->GetTaskTextColor(hTask);

	// can't use a switch here because we also need to check for IUI_ALL
	if (pTasks->IsAttributeAvailable(TDCA_TASKNAME))
		sTitle = pTasks->GetTaskTitle(hTask);

	if (pTasks->IsAttributeAvailable(TDCA_ICON))
		bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

	if (pTasks->IsAttributeAvailable(TDCA_PERCENT))
		nPercent = pTasks->GetTaskPercentDone(hTask, TRUE);

	if (pTasks->IsAttributeAvailable(TDCA_STARTDATE))
	{
		time64_t tDate = 0;

		if (pTasks->GetTaskStartDate64(hTask, false, tDate))
			SetStartDate(tDate);
		else
			ClearStartDate();
	}

	if (pTasks->IsAttributeAvailable(TDCA_DUEDATE))
	{
		time64_t tDate = 0;

		if (pTasks->GetTaskDueDate64(hTask, false, tDate))
			SetDueDate(tDate);
		else
			ClearDueDate();
	}

	if (pTasks->IsAttributeAvailable(TDCA_DONEDATE))
	{
		time64_t tDate = 0;

		if (pTasks->GetTaskDoneDate64(hTask, tDate))
			SetDoneDate(tDate);
		else
			ClearDoneDate();
	}

	if (pTasks->IsAttributeAvailable(TDCA_TAGS))
	{
		int nTag = pTasks->GetTaskTagCount(hTask);
		aTags.RemoveAll();

		while (nTag--)
			aTags.Add(pTasks->GetTaskTag(hTask, nTag));
	}

	if (pTasks->IsAttributeAvailable(TDCA_ALLOCTO))
	{
		CStringArray aAllocTo;
		int nAllocTo = pTasks->GetTaskAllocatedToCount(hTask);

		while (nAllocTo--)
			aAllocTo.Add(pTasks->GetTaskAllocatedTo(hTask, nAllocTo));

		sAllocTo = Misc::FormatArray(aAllocTo);
	}

	if (pTasks->IsAttributeAvailable(TDCA_DEPENDENCY))
	{
		int nDepend = pTasks->GetTaskDependencyCount(hTask);
		aDependIDs.RemoveAll();

		while (nDepend--)
		{
			// Local dependencies only
			DWORD dwDependID = _ttoi(pTasks->GetTaskDependency(hTask, nDepend));

			if (dwDependID)
				aDependIDs.Add(dwDependID);
		}
	}

	// Always update these
	bLocked = pTasks->IsTaskLocked(hTask, true);
	bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);

	// detect update
	return (*this != niOrg);
}

BOOL NETWORKITEM::IsDone(BOOL bIncGoodAs) const
{
	if (CDateHelper::IsDateSet(dtDone))
		return TRUE;

	// else
	return (bIncGoodAs && bGoodAsDone);
}

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

void NETWORKITEM::SetStartDate(time64_t tDate)
{
	dtStart = CDateHelper::GetDate(tDate);
}

void NETWORKITEM::SetDueDate(time64_t tDate)
{
	dtDue = CDateHelper::GetDate(tDate);
}

void NETWORKITEM::SetDoneDate(time64_t tDate)
{
	dtDone = CDateHelper::GetDate(tDate);
}

void NETWORKITEM::ClearStartDate()
{
	CDateHelper::ClearDate(dtDone);
}

void NETWORKITEM::ClearDueDate()
{
	CDateHelper::ClearDate(dtDue);
}

void NETWORKITEM::ClearDoneDate()
{
	CDateHelper::ClearDate(dtDone);
}

BOOL NETWORKITEM::IsMilestone(const CString& sMilestoneTag) const
{
	return Misc::Contains(sMilestoneTag, aTags, FALSE, TRUE);
}

//////////////////////////////////////////////////////////////////////

CNetworkItemMap::~CNetworkItemMap()
{
	RemoveAll();
}

BOOL CNetworkItemMap::ItemIsLocked(DWORD dwTaskID) const
{
	const NETWORKITEM* pNI = GetItem(dwTaskID);

	return (pNI && pNI->bLocked);
}

BOOL CNetworkItemMap::ItemIsDone(DWORD dwTaskID, BOOL bIncGoodAs) const
{
	const NETWORKITEM* pNI = GetItem(dwTaskID);

	return (pNI && pNI->IsDone(bIncGoodAs));
}

BOOL CNetworkItemMap::ItemHasDependecies(DWORD dwTaskID) const
{
	const NETWORKITEM* pNI = GetItem(dwTaskID);

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

NETWORKITEM* CNetworkItemMap::GetItem(DWORD dwTaskID) const
{
	if (dwTaskID == 0)
		return NULL;

	NETWORKITEM* pNI = NULL;

	if (!Lookup(dwTaskID, pNI) || !pNI)
	{
		ASSERT(pNI);
		return NULL;
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
		const NETWORKITEM* pNIDepends = GetItem(dwDependID);
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
	const NETWORKITEM* pNI = GetItem(dwTaskID);
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

