// WorkloadStruct.h: interface for the CWorkloadStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKLOADSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_WORKLOADSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Workloadenum.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class CMapAllocations : protected CMap<CString, LPCTSTR, double, double&> 
{
public:
	CMapAllocations(BOOL bReturnAverageForTotal = FALSE);
	virtual ~CMapAllocations();

	double Get(const CString& sAllocTo) const;
	CString Get(const CString& sAllocTo, int nDecimals) const;

	double GetTotal() const;
	CString GetTotal(int nDecimals) const;

	BOOL Set(const CString& sAllocTo, double dValue);
	BOOL Set(const CString& sAllocTo, const CString& sValue);

	BOOL Add(const CString& sAllocTo, double dValue);
	void Increment(const CString& sAllocTo);
	
	void Decode(const CString& sAllocations);
	CString Encode() const;

	BOOL MatchAll(const CMapAllocations& other) const;
	void Copy(const CMapAllocations& other);
	void RemoveAll();

protected:
	BOOL m_bReturnAverageForTotal;
	
protected:
	static CString Format(double dValue, int nDecimals);
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADITEM 
{ 
	WORKLOADITEM(DWORD dwID = 0, LPCTSTR szTitle = NULL);
	WORKLOADITEM(const WORKLOADITEM& wi);
	virtual ~WORKLOADITEM();
	
	WORKLOADITEM& operator=(const WORKLOADITEM& wi);
	BOOL operator==(const WORKLOADITEM& wi) const;
	
	CString sTitle;
	COleDateTime dtStart, dtDue; 
	BOOL bDone;
	COLORREF color;
	CStringArray aAllocTo;
	bool bParent;
	DWORD dwTaskID, dwRefID, dwOrgRefID;
	int nPercent;
	BOOL bGoodAsDone, bSomeSubtaskDone;
	int nPosition;
	BOOL bLocked, bHasIcon;
	CMapAllocations mapAllocatedDays;
	
	BOOL HasStart() const;
	BOOL HasDue() const;
	BOOL GetDuration(double& dDays) const;

	COLORREF GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	COLORREF GetTextBkColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
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
	BOOL ItemIsLocked(DWORD dwTaskID) const;

	void CalculateTotals(CMapAllocations& mapTotalDays, 
						 CMapAllocations& mapTotalTasks) const;

};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADSORTCOLUMN
{
	WORKLOADSORTCOLUMN();

	BOOL Sort(WLC_TREECOLUMN nBy, BOOL bAllowToggle, BOOL bAscending);
	BOOL Matches(WLC_TREECOLUMN nBy, BOOL bAscending) const;
	BOOL operator==(const WORKLOADSORTCOLUMN& col) const;

	WLC_TREECOLUMN nBy;
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
	BOOL IsSortingBy(WLC_TREECOLUMN nColID) const;
	BOOL IsSingleSortingBy(WLC_TREECOLUMN nColID) const;
	BOOL IsMultiSortingBy(WLC_TREECOLUMN nColID) const;
	BOOL Sort(WLC_TREECOLUMN nBy, BOOL bAllowToggle, BOOL bAscending);
	BOOL Sort(const WORKLOADSORTCOLUMNS& sort);

	WORKLOADSORTCOLUMN single;
	WORKLOADSORTCOLUMNS multi;
	BOOL bMultiSort;
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADCOLUMN
{
	WLC_TREECOLUMN nColID;
	UINT nIDAttribName;
	UINT nIDColName;
	int nColAlign;
	BOOL bDefaultVis;
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADTOTAL
{
	UINT nTextResID;
	WORKLOAD_TOTALS nTotal;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WORKLOADSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
