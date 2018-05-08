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

const double NO_ALLOCATION = 0xffffffff;

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
	
	BOOL HasStart() const;
	BOOL HasDue() const;
	BOOL GetDuration(double& dDays) const;

	void DecodeAllocations(const CString& sAllocations);
	CString EncodeAllocations() const;
	void ClearAllocations();
	BOOL AllocatedDaysMatch(const WORKLOADITEM& wi) const;

	BOOL GetAllocatedDays(const CString& sAllocTo, double& dDays) const;
	BOOL GetAllocatedDays(const CString& sAllocTo, CString& sDays, int nDecimals = -1) const;
	BOOL SetAllocatedDays(const CString& sAllocTo, double dDays);
	BOOL SetAllocatedDays(const CString& sAllocTo, const CString& sDays);
	
	COLORREF GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	COLORREF GetTextBkColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	BOOL HasColor() const;

protected:
	CMap<CString, LPCTSTR, double, double&> mapAllocatedDays;
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

	void CalculateTotals(WORKLOADITEM& wiAllocatedDays, 
						 WORKLOADITEM& wiAllocatedTasks,
						 WORKLOADITEM& wiPercentLoading) const;

protected:
	static double GetAllocatedDays(const WORKLOADITEM& wi, const CString& sAllocTo);
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

struct WORKLOADCOLUMN
{
	WLC_COLUMN nColID;
	UINT nIDAttribName;
	UINT nIDColName;
	int nColAlign;
	BOOL bDefaultVis;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WORKLOADSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
