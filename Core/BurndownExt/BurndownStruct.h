#if !defined(AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BURNDOWNSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BurndownEnum.h"

#include "..\Interfaces\ITasklist.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class CGraphsMap;
class COleDateTimeRange;
class IPreferences;

/////////////////////////////////////////////////////////////////////////////

// Because both the number of graphs and the number of colors
// is relatively small we accept the overhead of copying
class CColorArray : public CDWordArray
{
public:
	CColorArray() {}
	
	CColorArray& operator=(const CColorArray& other);
	BOOL operator==(const CColorArray& other) const;

	int Set(COLORREF color1, COLORREF color2 = CLR_NONE, COLORREF color3 = CLR_NONE);
	BOOL SetAt(int nIndex, COLORREF color);

	COLORREF GetAt(int nIndex) const;
	BOOL Has(COLORREF color) const;
};

/////////////////////////////////////////////////////////////////////////////

class CGraphColorMap : public CMap<CString, LPCTSTR, CColorArray, CColorArray&>
{
public:
	BOOL Set(const CGraphColorMap& other);

	int GetColors(BURNDOWN_GRAPH nGraph, CColorArray& aColors) const;
	BOOL SetColors(BURNDOWN_GRAPH nGraph, const CColorArray& aColors);

	int GetColors(const CString& sCustAttribID, CColorArray& aColors) const;
	BOOL SetColors(const CString& sCustAttribID, const CColorArray& aColors);
};

/////////////////////////////////////////////////////////////////////////////

struct GRAPHATTRIBUTES
{
	GRAPHATTRIBUTES() : nOption(BGO_INVALID) {}

	BURNDOWN_GRAPHOPTION nOption;
	CColorArray aColors;
};

/////////////////////////////////////////////////////////////////////////////

typedef CMap<CString, LPCTSTR, BURNDOWN_GRAPHOPTION, BURNDOWN_GRAPHOPTION&> CGraphOptionsMap;

class CGraphAttributes
{
public:
	BOOL Initialise(const CGraphsMap& mapGraphs);
	BOOL Update(const CGraphsMap& mapGraphs);

	BOOL IsEmpty() const { return (m_mapColors.GetCount() == 0); }
	BOOL SetColors(const CGraphColorMap& mapColors);

	int GetColors(BURNDOWN_GRAPH nGraph, CColorArray& aColors) const;
	BURNDOWN_GRAPHOPTION GetOption(BURNDOWN_GRAPH nGraph) const;
	BOOL SetOption(BURNDOWN_GRAPH nGraph, BURNDOWN_GRAPHOPTION nOption);

	int GetColors(const CString& sCustAttribID, CColorArray& aColors) const;
	BURNDOWN_GRAPHOPTION GetOption(const CString& sCustAttribID) const;
	BOOL SetOption(const CString& sCustAttribID, BURNDOWN_GRAPHOPTION nOption);

	void Save(IPreferences* pPrefs, LPCTSTR szKey) const;
	BOOL Load(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CGraphColorMap m_mapColors;
	CGraphOptionsMap m_mapOptions;
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
	BURNDOWN_GRAPHTYPE nType;
	UINT nLabelID;
};

/////////////////////////////////////////////////////////////////////////////

struct GRAPHOPTION
{
	BURNDOWN_GRAPHOPTION nOption;
	BURNDOWN_GRAPHTYPE nType;
};

/////////////////////////////////////////////////////////////////////////////

struct CUSTOMATTRIBDEF
{
	CUSTOMATTRIBDEF();

	BOOL operator==(const CUSTOMATTRIBDEF& other) const;
	BOOL operator!=(const CUSTOMATTRIBDEF& other) const;

	CString sUniqueID;
	CString sLabel;
	CString sListData;

	BURNDOWN_GRAPH nGraph;
	BURNDOWN_GRAPHTYPE nType;
};

// ------------------------------------------------------

class CCustomAttributeDefinitionArray : public CArray<CUSTOMATTRIBDEF, CUSTOMATTRIBDEF&>
{
public:
	int Find(const CString& sID) const;
	int Find(BURNDOWN_GRAPH nGraph) const;

	BOOL Update(const ITASKLISTBASE* pTasks);

protected:
	BURNDOWN_GRAPH GetFirstUnusedGraph() const;

};

/////////////////////////////////////////////////////////////////////////////

struct STATSITEM 
{ 
	STATSITEM(DWORD dwTaskID = 0);
	virtual ~STATSITEM();

	void Set(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CCustomAttributeDefinitionArray& aCustAttribDef);
	void Update(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CCustomAttributeDefinitionArray& aCustAttribDef);

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

	CString sTitle;
	CString sStatus;
	CString sAllocatedBy;
	CString sPriority;
	CString sRisk;
	CString sVersion;

	CMapStringToString mapCustomAttrib; // Maps unique ID to value

protected:
	void ValidateStartDate();

	static void MinMax(const COleDateTime& date, COleDateTimeRange& dtExtents);

	static void GetCustomAttributeValues(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CCustomAttributeDefinitionArray& aCustAttribDef, CMapStringToString& mapValues);
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
