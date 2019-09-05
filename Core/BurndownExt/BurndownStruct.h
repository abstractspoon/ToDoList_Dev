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

struct DISPLAYITEM
{
	UINT nYAxisID;
	BURNDOWN_CHARTTYPE nDisplay;
};

/////////////////////////////////////////////////////////////////////////////

struct STATSITEM 
{ 
	STATSITEM();
	virtual ~STATSITEM();

	void Set(const ITASKLISTBASE* pTasks, HTASKITEM hTask);
	void Update(const ITASKLISTBASE* pTasks, HTASKITEM hTask);

	BOOL HasStart() const;
	BOOL HasDue() const;
	BOOL IsDone() const;

	COleDateTime GetEndDate() const;
	BOOL GetRange(COleDateTimeRange& dtRange) const;
	BOOL GetIntersection(const COleDateTimeRange& dtExtents, COleDateTimeRange& dtIntersection) const;
	
	double GetIntersectionProportion(const COleDateTimeRange& dtExtents, BOOL bWeekdays) const; // return 0.0-1.0
	double GetIntersectionProportionAtDate(const COleDateTimeRange& dtExtents, const COleDateTime& date, BOOL bWeekdays) const; // return 0.0-1.0

	void MinMax(COleDateTimeRange& dtExtents) const;
	
	COleDateTime dtStart, dtDue, dtDone; 
	double dTimeEst, dTimeSpent;
	TDC_UNITS nTimeEstUnits, nTimeSpentUnits;
	double dCost;
	BOOL bCostIsRate;
	DWORD dwTaskID;

protected:
	static void MinMax(const COleDateTime& date, COleDateTimeRange& dtExtents);
	static double GetIntersectionProportion(const COleDateTime& dtStart, const COleDateTime& dtEnd, const COleDateTimeRange& dtExtents, BOOL bWeekdays); // return 0.0-1.0

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
	BOOL HasItem(DWORD dwTaskID) const;
	BOOL IsEmpty() const;
	int GetSize() const;

	void RemoveAll();
	void RemoveAt(int nIndex, int nCount = 1);

	void Sort();
	BOOL IsSorted() const;

	STATSITEM* operator[](int nIndex) const;
	void GetDataExtents(COleDateTimeRange& dtExtents) const;

protected:
	CDWordSet m_setTaskIDs;
	
protected:
	int FindItem(DWORD dwTaskID) const;

	static int CompareItems(const void* pV1, const void* pV2);
};

/////////////////////////////////////////////////////////////////////////////

class CStatsItemCalculator
{
public:
	CStatsItemCalculator(const CStatsItemArray& data);
	virtual ~CStatsItemCalculator();

	BOOL SetDateRange(const COleDateTimeRange& dtExtents);

	COleDateTime GetStartDate() const { return m_dtExtents.GetStart(); }
	COleDateTime GetEndDate() const { return m_dtExtents.GetEndInclusive(); }
	
	// Totals
	double GetTotalTimeEstimateInDays() const;
	double GetTotalTimeSpentInDays() const;
	double GetTotalCostEstimate() const;
	double GetTotalCostSpent() const;

	// Proportions
	double GetTimeSpentInDays(const COleDateTime& date) const;
	double GetTimeEstimateInDays(const COleDateTime& date) const;
	double GetCostSpent(const COleDateTime& date) const;
	double GetCostEstimate(const COleDateTime& date) const;

	int GetIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nNextItemFrom) const;

protected:
	const CStatsItemArray& m_data;

	COleDateTimeRange m_dtExtents;

protected:
	double CalcProportionOfValue(const STATSITEM& si, double dValue, const COleDateTime& date) const;

	enum ATTRIB { TIME, COST };
	enum ATTRIBTYPE { ESTIMATE, SPENT };

	double GetTotalAttribValue(ATTRIB nAttrib, ATTRIBTYPE nType) const;
	double GetTotalAttribValue(ATTRIB nAttrib, ATTRIBTYPE nType, const COleDateTime& date) const;

	double GetAttribValue(const STATSITEM& si, ATTRIB nAttrib, ATTRIBTYPE nType) const;
	double GetAttribValue(const STATSITEM& si, ATTRIB nAttrib, ATTRIBTYPE nType, const COleDateTime& date) const;

	static double GetTimeInDays(double dTime, TDC_UNITS nUnits);
	static TH_UNITS MapUnitsToTHUnits(TDC_UNITS nUnits);
};

#endif // !defined(AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
