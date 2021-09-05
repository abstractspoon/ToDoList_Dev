// WorkloadStruct.h: interface for the CWorkloadStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKLOADSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_WORKLOADSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Workloadenum.h"

#include "..\shared\DateHelper.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class COleDateTimeRange;

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR ALLOCTO_TOTALID = _T("_TOTAL_");

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADALLOCATION
{
	WORKLOADALLOCATION() : dDays(0.0), bOverlapping(FALSE) {}

	WORKLOADALLOCATION& operator=(const WORKLOADALLOCATION& wa)
	{
		dDays = wa.dDays;
		bOverlapping = wa.bOverlapping;

		return *this;
	}

	BOOL operator==(const WORKLOADALLOCATION& wa) const
	{
		return ((dDays == wa.dDays) && (bOverlapping == wa.bOverlapping));
	}

	BOOL operator!=(const WORKLOADALLOCATION& wa) const
	{
		return !(*this == wa);
	}

	void Reset()
	{
		dDays = 0.0;
		bOverlapping = FALSE;
	}

	double dDays;
	BOOL bOverlapping;
};

/////////////////////////////////////////////////////////////////////////////

class CMapDayAllocations : protected CMap<CString, LPCTSTR, WORKLOADALLOCATION, WORKLOADALLOCATION&> 
{
public:
	CMapDayAllocations();
	virtual ~CMapDayAllocations();

	BOOL Get(const CString& sAllocTo, WORKLOADALLOCATION& wa) const;
	int GetKeys(CStringArray& aKeys) const;

	double GetDays(const CString& sAllocTo) const;
	CString FormatDays(const CString& sAllocTo, int nDecimals) const;

	double GetTotalDays() const;
	CString FormatTotalDays(int nDecimals) const;

	BOOL SetDays(const CString& sAllocTo, double dValue);
	BOOL SetDays(const CString& sAllocTo, const CString& sValue);
	BOOL AddDays(const CString& sAllocTo, double dValue);
	BOOL AddDays(const CString& sAllocTo, const CString& sValue);

	BOOL AppendOverlaps(const CMapDayAllocations& mapAlloc);
	BOOL IsOverlapping(const CString& sAllocTo) const;
	void ClearOverlaps();
	
	BOOL IsAutoCalculated() const { return (bAutoCalculated || !GetCount()); }
	void Recalculate(const CStringArray& aAllocTo, double dTotal, BOOL bProportionally);

	BOOL Decode(const CString& sAllocations);
	CString Encode() const;

	BOOL MatchAll(const CMapDayAllocations& other) const;
	void Copy(const CMapDayAllocations& other);
	void RemoveAll();
	void RemoveKey(const CString& sAllocTo);

protected:
	BOOL bAutoCalculated;
	
protected:
	static CString FormatDays(double dValue, int nDecimals);
};

/////////////////////////////////////////////////////////////////////////////

class CMapAllocationTotals : protected CMap<CString, LPCTSTR, double, double&> 
{
public:
	CMapAllocationTotals(BOOL bReturnAverageForTotal = FALSE);
	virtual ~CMapAllocationTotals();

	double Get(const CString& sAllocTo) const;
	CString Format(const CString& sAllocTo, int nDecimals) const;

	double GetTotal() const;
	CString FormatTotal(int nDecimals) const;

	BOOL Set(const CString& sAllocTo, double dValue);
	BOOL Add(const CString& sAllocTo, double dValue);
	void Increment(const CString& sAllocTo);

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
	BOOL operator!=(const WORKLOADITEM& wi) const;
	
	BOOL HasStartDate() const;
	BOOL HasDueDate() const;
	BOOL HasValidDates() const;

	BOOL IsDone(BOOL bIncGoodAs) const;

	void UpdateAllocationCalculations(BOOL bAutoCalculatedOnly, BOOL bPreferTimeEstimate, BOOL bProportionally, BOOL bAllowParentAllocations);
	void ClearAllocations();

	COLORREF GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	COLORREF GetTextBkColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	BOOL HasColor() const;

	int GetNames(const CStringArray& aAllNames, CStringArray& aNames) const;

	CString sTitle;
	COleDateTimeRange dtRange; 
	COLORREF color;
	CStringArray aAllocTo;
	DWORD dwTaskID, dwRefID, dwOrgRefID;
	int nPercent;
	int nPosition;
	double dTimeEst; // weekdays
	CMapDayAllocations mapAllocatedDays;
	
	BOOL bDone;
	bool bParent; // 'bool' to match ITaskList
	BOOL bLocked, bHasIcon;
	BOOL bGoodAsDone, bSomeSubtaskDone;

};

/////////////////////////////////////////////////////////////////////////////

class CWorkloadItemMap : public CMap<DWORD, DWORD, WORKLOADITEM*, WORKLOADITEM*&>
{
public:
	virtual ~CWorkloadItemMap();

	void RemoveAll();
	BOOL RemoveItem(DWORD dwKey);
	BOOL HasItem(DWORD dwKey) const;
	WORKLOADITEM* GetItem(DWORD dwKey, BOOL bResolveReferences) const;
	WORKLOADITEM* GetNextItem(POSITION& pos) const;

	BOOL ItemIsLocked(DWORD dwTaskID, BOOL bTreatRefsAsUnlocked) const;
	BOOL ItemIsReference(DWORD dwTaskID) const;
	BOOL ItemIsDone(DWORD dwTaskID, BOOL bIncGoodAs) const;

	void CalculateTotals(const COleDateTimeRange& dtPeriod,
						 CMapAllocationTotals& mapTotalDays,
						 CMapAllocationTotals& mapTotalTasks,
						 BOOL bAllowParentAllocations,
						 BOOL bIncludeTasksWithoutDates) const;

	void RecalculateOverlaps();
	BOOL CalcDateRange(COleDateTimeRange& dtRange) const;

protected:
	int BuildDateSortedList(CArray<WORKLOADITEM*, WORKLOADITEM*&>& aItems) const;
	
	static int CompareItems(const void* pV1, const void* pV2);
	static BOOL GetValidatedDateRange(const WORKLOADITEM* pWI, COleDateTimeRange& dtRange);

};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADSORTCOLUMN
{
	WORKLOADSORTCOLUMN();

	BOOL Set(WLC_COLUMNID nBy, BOOL bAllowToggle, BOOL bAscending);
	BOOL Matches(WLC_COLUMNID nBy, BOOL bAscending) const;

	BOOL operator==(const WORKLOADSORTCOLUMN& col) const;
	BOOL operator!=(const WORKLOADSORTCOLUMN& col) const;

	WLC_COLUMNID nBy;
	CString sAllocTo;
	BOOL bAscending;
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADSORTCOLUMNS
{
	WORKLOADSORTCOLUMNS();

	BOOL Set(const WORKLOADSORTCOLUMNS& sort);

	BOOL operator==(const WORKLOADSORTCOLUMNS& other) const;
	BOOL operator!=(const WORKLOADSORTCOLUMNS& other) const;

	WORKLOADSORTCOLUMN cols[3];
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADSORT
{
	WORKLOADSORT();

	BOOL IsSorting() const;
	BOOL IsSortingBy(WLC_COLUMNID nColID) const;
	BOOL IsSingleSortingBy(WLC_COLUMNID nColID) const;
	BOOL IsMultiSortingBy(WLC_COLUMNID nColID) const;
	BOOL Set(WLC_COLUMNID nBy, BOOL bAllowToggle, BOOL bAscending);
	BOOL Set(const WORKLOADSORTCOLUMNS& sort);

	WORKLOADSORTCOLUMN single;
	WORKLOADSORTCOLUMNS multi;
	BOOL bMultiSort;
};

/////////////////////////////////////////////////////////////////////////////

struct WORKLOADCOLUMN
{
	WLC_COLUMNID nColID;
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
