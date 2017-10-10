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
	BOOL operator==(const GANTTITEM& gi);
	
	CString sTitle;
	COleDateTime dtStart, dtMinStart;
	COleDateTime dtDue, dtMaxDue; 
	COleDateTime dtDone; 
	COLORREF color;
	CString sAllocTo, sIcon;
	bool bParent;
	DWORD dwTaskID, dwRefID, dwOrgRefID;
	CStringArray aDepends;
	CStringArray aTags;
	int nPercent;
	BOOL bGoodAsDone, bSomeSubtaskDone;
	int nPosition;
	BOOL bLocked;
	
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
	BOOL Contains(const GANTTITEM& gi);
	int Compare(const COleDateTime& date) const;

protected:
	COleDateTime dtStart, dtEnd;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GANTTSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
