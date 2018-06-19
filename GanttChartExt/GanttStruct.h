// GanttStruct.h: interface for the CGanttStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANTTSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_GANTTSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ganttenum.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

struct GANTTITEM 
{ 
	GANTTITEM();
	GANTTITEM(const GANTTITEM& gi);
	virtual ~GANTTITEM();
	
	GANTTITEM& operator=(const GANTTITEM& gi);
	BOOL operator==(const GANTTITEM& gi) const;
	
	CString sTitle;
	COleDateTime dtStart, dtMinStart;
	COleDateTime dtDue, dtMaxDue; 
	COleDateTime dtDone; 
	COLORREF color;
	CString sAllocTo;
	BOOL bParent;
	DWORD dwTaskID, dwRefID, dwOrgRefID;
	CDWordArray aDependIDs;
	CStringArray aTags;
	int nPercent;
	BOOL bGoodAsDone, bSomeSubtaskDone;
	int nPosition;
	BOOL bLocked, bHasIcon;
	
	void MinMaxDates(const GANTTITEM& giOther);
	BOOL IsDone(BOOL bIncGoodAs) const;
	BOOL HasStart() const;
	BOOL HasDue() const;
	
	COLORREF GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	COLORREF GetTextBkColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	COLORREF GetFillColor() const;
	COLORREF GetBorderColor() const;
	BOOL HasColor() const;
};

/////////////////////////////////////////////////////////////////////////////

class CGanttItemMap : public CMap<DWORD, DWORD, GANTTITEM*, GANTTITEM*&>
{
public:
	virtual ~CGanttItemMap();

	void RemoveAll();
	BOOL RemoveKey(DWORD dwKey);
	BOOL HasItem(DWORD dwKey) const;
	GANTTITEM* GetItem(DWORD dwKey) const;
	BOOL RestoreItem(const GANTTITEM& giPrev);
	BOOL ItemIsLocked(DWORD dwTaskID) const;
	BOOL ItemHasDependecies(DWORD dwTaskID) const;
	BOOL IsItemDependentOn(const GANTTITEM* pGI, DWORD dwOtherID) const;
};

/////////////////////////////////////////////////////////////////////////////

struct GANTTDEPENDENCY
{
	GANTTDEPENDENCY();

	void SetFrom(const CPoint& pt, DWORD dwTaskID = 0);
	void SetTo(const CPoint& pt, DWORD dwTaskID = 0);

	DWORD GetFromID() const;
	DWORD GetToID() const;

	BOOL Matches(DWORD dwFrom, DWORD dwTo) const;
	
	BOOL HitTest(const CRect& rect) const;
	BOOL HitTest(const CPoint& point, int nTol = 2) const;

	BOOL Draw(CDC* pDC, const CRect& rClient, BOOL bDragging);

#ifdef _DEBUG
	void Trace() const;
#endif
	
	static int STUB;
	
protected:
	CPoint ptFrom, ptTo;
	DWORD dwFromID, dwToID;
	
protected:
	void CalcDependencyPath(CPoint pts[3]) const;
	BOOL CalcBoundingRect(CRect& rect) const;
	void CalcDependencyArrow(const CPoint& pt, CPoint pts[3]) const;
	void DrawDependencyArrow(CDC* pDC, const CPoint& pt) const;
	BOOL IsFromAboveTo() const;
};

typedef CArray<GANTTDEPENDENCY, GANTTDEPENDENCY&> CGanttDependArray;

/////////////////////////////////////////////////////////////////////////////

struct GANTTDATERANGE
{
	GANTTDATERANGE();
	
	void Clear();
	void MinMax(const GANTTITEM& gi);
	void MinMax(const COleDateTime& date);

	COleDateTime GetStart() const { return dtStart; }
	COleDateTime GetEnd() const { return dtEnd; }
	COleDateTime GetStart(GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades = TRUE) const;
	COleDateTime GetEnd(GTLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades = TRUE) const;

	BOOL IsValid() const;
	BOOL IsEmpty() const;
	BOOL Contains(const GANTTITEM& gi) const;
	BOOL operator==(const GANTTDATERANGE& range) const;

protected:
	COleDateTime dtStart, dtEnd;
};

/////////////////////////////////////////////////////////////////////////////

struct GANTTSORTCOLUMN
{
	GANTTSORTCOLUMN();

	BOOL Sort(GTLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending);
	BOOL Matches(GTLC_COLUMN nBy, BOOL bAscending) const;
	BOOL operator==(const GANTTSORTCOLUMN& col) const;

	GTLC_COLUMN nBy;
	BOOL bAscending;
};

/////////////////////////////////////////////////////////////////////////////

struct GANTTSORTCOLUMNS
{
	GANTTSORTCOLUMNS();

	BOOL Sort(const GANTTSORTCOLUMNS& sort);
	BOOL operator==(const GANTTSORTCOLUMNS& sort) const;

	GANTTSORTCOLUMN cols[3];
};

/////////////////////////////////////////////////////////////////////////////

struct GANTTSORT
{
	GANTTSORT();

	BOOL IsSorting() const;
	BOOL IsSortingBy(GTLC_COLUMN nColID) const;
	BOOL IsSingleSortingBy(GTLC_COLUMN nColID) const;
	BOOL IsMultiSortingBy(GTLC_COLUMN nColID) const;
	BOOL Sort(GTLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending);
	BOOL Sort(const GANTTSORTCOLUMNS& sort);

	GANTTSORTCOLUMN single;
	GANTTSORTCOLUMNS multi;
	BOOL bMultiSort;
};

/////////////////////////////////////////////////////////////////////////////

class IGanttDependencyEditor
{
public:
	virtual BOOL SetFromTask(DWORD dwFromTaskID) = 0;
	virtual BOOL SetFromDependency(DWORD dwFromTaskID, DWORD dwCurToTaskID) = 0;
	virtual BOOL SetToTask(DWORD dwToTaskID) = 0;

	virtual DWORD GetFromTask() const = 0;
	virtual DWORD GetFromDependency(DWORD& dwCurToTaskID) const = 0;
	virtual DWORD GetToTask() const = 0;

	virtual BOOL IsPicking() const = 0;
	virtual BOOL IsPickingFromTask() const = 0;
	virtual BOOL IsPickingFromDependency() const = 0;
	virtual BOOL IsPickingToTask() const = 0;
	virtual BOOL IsPickingCancelled() const = 0;
	virtual BOOL IsPickingCompleted() const = 0;

	virtual BOOL IsAdding() const = 0;
	virtual BOOL IsEditing() const = 0;
	virtual BOOL IsDeleting() const = 0;

	virtual void Cancel() = 0;
};

/////////////////////////////////////////////////////////////////////////////

struct GTCDISPLAYMODE
{
	GTLC_MONTH_DISPLAY nDisplay;
	UINT nStringID;
};

/////////////////////////////////////////////////////////////////////////////

struct GANTTCOLUMN
{
	GTLC_COLUMN nColID;
	UINT nIDAttribName;
	UINT nIDColName;
	int nColAlign;
	BOOL bDefaultVis;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GANTTSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
