#if !defined(AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BurndownEnum.h"

#include "..\Shared\mapex.h"
#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"

#include "..\Interfaces\IEnums.h"

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

	BOOL HasStart() const;
	BOOL IsDone() const;
	
	void MinMax(COleDateTimeRange& dtExtents) const;
	
	COleDateTime dtStart, dtDone; 
	double dTimeEst, dTimeSpent;
	TDC_UNITS nTimeEstUnits, nTimeSpentUnits;
	double dCost;
	BOOL bCostIsRate;
	DWORD dwTaskID;

protected:
	static void MinMax(const COleDateTime& date, COleDateTimeRange& dtExtents);


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
	CStatsItemCalculator(const CStatsItemArray& aItems);
	virtual ~CStatsItemCalculator();

	void GetDataExtents(COleDateTimeRange& dtExtents) const;
	int GetIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nNextItemFrom) const;
	
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

protected:
	const CStatsItemArray& m_aItems;

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
