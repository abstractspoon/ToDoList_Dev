#if !defined(AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "..\Shared\mapex.h"

/////////////////////////////////////////////////////////////////////////////

struct STATSITEM 
{ 
	STATSITEM();
	virtual ~STATSITEM();

	BOOL HasStart() const;
	BOOL IsDone() const;
	
	void MinMax(COleDateTime& dtMin, COleDateTime& dtMax) const;

	double CalcTimeSpentInDays(const COleDateTime& date, double dMultiplier) const;
	double CalcTimeEstimateInDays(double dMultiplier) const;
	
	COleDateTime dtStart, dtDone; 
	double dTimeEstDays, dTimeSpentDays;
	DWORD dwTaskID;

protected:
	static void MinMax(const COleDateTime& date, COleDateTime& dtMin, COleDateTime& dtMax);

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

	double CalcTimeSpentInDays(const COleDateTime& date, double dMultiplier) const;
	double CalcTotalTimeEstimateInDays(double dMultiplier) const;
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
