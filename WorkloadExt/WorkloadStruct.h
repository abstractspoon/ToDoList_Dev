// WorkloadStruct.h: interface for the CWorkloadStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WorkloadSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_WorkloadSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Workloadenum.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADITEM 
{ 
	WORKLOADITEM();
	WORKLOADITEM(const WORKLOADITEM& gi);
	virtual ~WORKLOADITEM();
	
	WORKLOADITEM& operator=(const WORKLOADITEM& gi);
	BOOL operator==(const WORKLOADITEM& gi);
	
	CString sTitle;
	COleDateTime dtStart, dtMinStart;
	COleDateTime dtDue, dtMaxDue; 
	COleDateTime dtDone; 
	COLORREF color;
	CString sAllocTo;
	bool bParent;
	DWORD dwTaskID, dwRefID, dwOrgRefID;
	CDWordArray aDependIDs;
	CStringArray aTags;
	int nPercent;
	BOOL bGoodAsDone, bSomeSubtaskDone;
	int nPosition;
	BOOL bLocked, bHasIcon;
	
	void MinMaxDates(const WORKLOADITEM& giOther);
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

class CWorkloadItemMap : public CMap<DWORD, DWORD, WORKLOADITEM*, WORKLOADITEM*&>
{
public:
	virtual ~CWorkloadItemMap();

	void RemoveAll();
	BOOL RemoveKey(DWORD dwKey);
	BOOL HasItem(DWORD dwKey) const;
	WORKLOADITEM* GetItem(DWORD dwKey) const;
	BOOL RestoreItem(const WORKLOADITEM& giPrev);
	BOOL ItemIsLocked(DWORD dwTaskID) const;
	BOOL ItemHasDependecies(DWORD dwTaskID) const;
	BOOL IsItemDependentOn(const WORKLOADITEM* pGI, DWORD dwOtherID) const;
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADDATERANGE
{
	WORKLOADDATERANGE();

	void Clear();
	void MinMax(const WORKLOADITEM& gi);
	void MinMax(const COleDateTime& date);

	COleDateTime GetStart() const { return dtStart; }
	COleDateTime GetEnd() const { return dtEnd; }
	COleDateTime GetStart(WLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades = TRUE) const;
	COleDateTime GetEnd(WLC_MONTH_DISPLAY nDisplay, BOOL bZeroBasedDecades = TRUE) const;

	BOOL IsValid() const;
	BOOL IsEmpty() const;
	BOOL Contains(const WORKLOADITEM& gi);
	int Compare(const COleDateTime& date) const;

protected:
	COleDateTime dtStart, dtEnd;
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADSORTCOLUMN
{
	WORKLOADSORTCOLUMN();

	BOOL Sort(WLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending);
	BOOL Matches(WLC_COLUMN nBy, BOOL bAscending) const;
	BOOL operator==(const WORKLOADSORTCOLUMN& col) const;

	WLC_COLUMN nBy;
	BOOL bAscending;
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADSORTCOLUMNS
{
	WORKLOADSORTCOLUMNS();

	BOOL Sort(const WORKLOADSORTCOLUMNS& sort);
	BOOL operator==(const WORKLOADSORTCOLUMNS& sort) const;

	WORKLOADSORTCOLUMN cols[3];
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADSORT
{
	WORKLOADSORT();

	BOOL IsSorting() const;
	BOOL IsSortingBy(WLC_COLUMN nColID) const;
	BOOL IsSingleSortingBy(WLC_COLUMN nColID) const;
	BOOL IsMultiSortingBy(WLC_COLUMN nColID) const;
	BOOL Sort(WLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending);
	BOOL Sort(const WORKLOADSORTCOLUMNS& sort);

	WORKLOADSORTCOLUMN single;
	WORKLOADSORTCOLUMNS multi;
	BOOL bMultiSort;
};

/////////////////////////////////////////////////////////////////////////////

struct WLCDISPLAYMODE
{
	WLC_MONTH_DISPLAY nDisplay;
	UINT nStringID;
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADCOLUMN
{
	WLC_COLUMN nColID;
	UINT nIDAttribName;
	UINT nIDColName;
	int nColAlign;
	BOOL bDefaultVis;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WorkloadSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
