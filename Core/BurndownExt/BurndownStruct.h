#if !defined(AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BurndownEnum.h"

#include "..\Shared\mapex.h"
#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"

#include "..\Interfaces\ITasklist.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

// Because both the number of graphs and the number of colors
// is relatively small we accept the overhead of copying
class CColorArray : public CDWordArray
{
public:
	CColorArray& operator=(CColorArray& other);
};

class CGraphColorMap : public CMap<BURNDOWN_GRAPH, BURNDOWN_GRAPH, CColorArray, CColorArray&>
{
public:
	void Copy(const CGraphColorMap& other);
};

/////////////////////////////////////////////////////////////////////////////

struct TRENDLINE
{
	UINT nLabelID;
	BURNDOWN_TREND nTrend;
};

/////////////////////////////////////////////////////////////////////////////

struct FREQUENCYITEM
{
	CString sLabel;
	int nCount;
};

/////////////////////////////////////////////////////////////////////////////

struct GRAPHTYPE
{
	UINT nLabelID;
	BURNDOWN_GRAPHTYPE nType;
};

/////////////////////////////////////////////////////////////////////////////

struct STATSITEM 
{ 
	STATSITEM(DWORD dwTaskID = 0);
	virtual ~STATSITEM();

	void Set(const ITASKLISTBASE* pTasks, HTASKITEM hTask);
	void Update(const ITASKLISTBASE* pTasks, HTASKITEM hTask);

	BOOL HasStart() const;
	BOOL HasDue() const;
	BOOL IsDone() const;

	COleDateTime GetEndDate() const;
	BOOL GetEndDate(COleDateTime& dtEnd) const;
	void MinMax(COleDateTimeRange& dtExtents) const;
	
	COleDateTime dtStart, dtDue, dtDone; 
	double dTimeEst, dTimeSpent;
	TDC_UNITS nTimeEstUnits, nTimeSpentUnits;
	double dCost;
	BOOL bCostIsRate;
	DWORD dwTaskID;

	CStringArray aCategory;
	CStringArray aAllocatedTo;
	CStringArray aTags;

	CString sStatus;
	CString sAllocatedBy;
	CString sPriority;
	CString sRisk;
	CString sVersion;

protected:
	void ValidateStartDate();

	static void MinMax(const COleDateTime& date, COleDateTimeRange& dtExtents);

	static double GetCost(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL& bIsRate);
	static COleDateTime GetStartDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask);
	static COleDateTime GetDueDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask);
	static COleDateTime GetDoneDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask);
	static COleDateTime GetDate(time64_t tDate);

};

/////////////////////////////////////////////////////////////////////////////

class CStatsItemArray : protected CArray<STATSITEM*, STATSITEM*>
{
public:
	CStatsItemArray();
	virtual ~CStatsItemArray();

	STATSITEM* AddItem(DWORD dwTaskID);
	STATSITEM* GetItem(DWORD dwTaskID) const;
	BOOL IsEmpty() const;
	int GetSize() const;

	void RemoveAll();
	void RemoveAt(int nIndex, int nCount = 1);

	void Sort();

	STATSITEM* operator[](int nIndex) const;
	void GetDataExtents(COleDateTimeRange& dtExtents) const;

protected:
	CMap<DWORD, DWORD, STATSITEM*, STATSITEM*&> m_mapTasks;
	BOOL m_bSorted;
	
protected:
	static int CompareItems(const void* pV1, const void* pV2);
	static int CompareDates(const COleDateTime& date1, const COleDateTime& date2);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
