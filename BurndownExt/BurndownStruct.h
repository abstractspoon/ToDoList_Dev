#if !defined(AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "..\Shared\mapex.h"
#include "..\shared\timehelper.h"

/////////////////////////////////////////////////////////////////////////////

struct STATSITEM 
{ 
	STATSITEM();
	virtual ~STATSITEM();

	BOOL HasStart() const;
	BOOL IsDone() const;
	
	void MinMax(COleDateTime& dtMin, COleDateTime& dtMax) const;

	double CalcTimeSpentInDays(const COleDateTime& date, int nDaysInWeek, double dHoursInDay) const;
	double CalcTimeEstimateInDays(int nDaysInWeek, double dHoursInDay) const;
	
	COleDateTime dtStart, dtDone; 
	double dTimeEst, dTimeSpent;
	TDC_UNITS nTimeEstUnits, nTimeSpentUnits;
	DWORD dwTaskID;

protected:
	static void MinMax(const COleDateTime& date, COleDateTime& dtMin, COleDateTime& dtMax);
	static double CalcTimeInDays(double dTime, TDC_UNITS nUnits, int nDaysInWeek, double dHoursInDay);
	static TH_UNITS MapUnitsToTHUnits(TDC_UNITS nUnits);

};

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

	double CalcTimeSpentInDays(const COleDateTime& date, int nDaysInWeek, double dHoursInDay) const;
	double CalcTotalTimeEstimateInDays(int nDaysInWeek, double dHoursInDay) const;
	int CalculateIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nNextItemFrom) const;

	void GetDataExtents(COleDateTime& dtEarliestDate, COleDateTime& dtLatestDate) const;

	STATSITEM* operator[](int nIndex) const;

protected:
	CDWordSet m_setTaskIDs;
	
protected:
	int FindItem(DWORD dwTaskID) const;

	static int CompareItems(const void* pV1, const void* pV2);
};

#endif // !defined(AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
