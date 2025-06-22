#if !defined(AFX_CALSTRUCT_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
#define AFX_CALSTRUCT_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskCalendarCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "..\shared\mapex.h"
#include "..\shared\DateHelper.h"

#include "..\Interfaces\ITaskList.h"
#include "..\Interfaces\IUIExtension.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

typedef CMap<CString, LPCTSTR, COleDateTime, COleDateTime&> CMapCustomDates; 

struct TASKCALITEMDATES
{
public:
	TASKCALITEMDATES();
	TASKCALITEMDATES(const TASKCALITEMDATES& tcid);
	TASKCALITEMDATES(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CMapStringToString& mapCustDateAttribIDs, DWORD dwCalcDates);

	TASKCALITEMDATES& operator=(const TASKCALITEMDATES& tcid);
	BOOL operator==(const TASKCALITEMDATES& tcid) const;

	void Update(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CMapStringToString& mapCustDateAttribIDs, DWORD dwCalcDates);
	void Recalc(DWORD dwCalcDates);

	BOOL IsValid() const;
	BOOL IsDone() const;
	BOOL IsCalculatedParent() const { return (bCalcedParentStart || bCalcedParentDue); }

	BOOL IsStartSet() const;
	BOOL IsEndSet() const;
	BOOL IsActive(const COleDateTime& date) const;

	COleDateTime GetAnyStart() const;
	COleDateTime GetAnyEnd() const;
	COleDateTime GetDone() const;

	BOOL HasAnyStart() const;
	BOOL HasAnyEnd() const;
	
	void SetStart(const COleDateTime& date);
	void SetDue(const COleDateTime& date);
	
	const CMapCustomDates& Custom() const { return mapCustomDates; }
	COleDateTime GetCustomDate(const CString& sCustAttribID) const;
	void SetCustomDate(const CString& sCustAttribID, const COleDateTime& date);
	void ClearCustomDate(const CString& sCustAttribID);
	void SetCustomDates(const CMapCustomDates& dates);

	void MinMax(COleDateTime& dtMin, COleDateTime& dtMax) const;

protected:
	COleDateTime dtCreation, dtStart, dtDue, dtDone;
	COleDateTime dtStartCalc, dtEndCalc;

	BOOL bCalcedParentStart, bCalcedParentDue;
	BOOL bTreatOverdueAsDueToday;

	CMapCustomDates mapCustomDates;

protected:
	void ClearCalculatedDates();

	static COleDateTime GetDate(time64_t tDate);
	static void MinMax(const COleDateTime& date, COleDateTime& dtMin, COleDateTime& dtMax);
};

/////////////////////////////////////////////////////////////////////////////

struct TASKCALITEM
{
public:
	TASKCALITEM();
	TASKCALITEM(const TASKCALITEM& tci);
	TASKCALITEM(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CMapStringToString& mapCustDateAttribIDs, DWORD dwCalcDates);
	virtual ~TASKCALITEM() {}

	TASKCALITEM& operator=(const TASKCALITEM& tci);
	TASKCALITEM& operator=(const TASKCALITEMDATES& tcid);
	BOOL operator==(const TASKCALITEM& tci) const;

	BOOL UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CMapStringToString& mapCustDateAttribIDs, DWORD dwCalcDates);
	inline DWORD GetTaskID() const { return dwTaskID; }

	virtual COLORREF GetFillColor(BOOL bTextIsBack) const;
	virtual COLORREF GetBorderColor(BOOL bSelected, BOOL bTextIsBack) const;
	virtual COLORREF GetTextColor(BOOL bSelected, BOOL bTextIsBack) const;
	BOOL HasColor() const;

	CString GetName(BOOL bFormatted = TRUE) const;

	BOOL IsParent() const;
	BOOL IsCalculatedParent() const;

	BOOL HasIcon(BOOL bShowParentsAsFolder) const;
	void DisableIcon() { bHasIcon = FALSE; }
	BOOL HasTag(LPCTSTR szTag) const;

	// Date wrappers
	void RecalcDates(DWORD dwCalcDates) { dates.Recalc(dwCalcDates); }
	BOOL IsValid() const { return dates.IsValid(); }
	BOOL IsDone(BOOL bIncGoodAs) const;
	BOOL IsActive(const COleDateTime& date) const { return dates.IsActive(date); }

	BOOL IsStartDateSet() const { return dates.IsStartSet(); }
	void SetStartDate(const COleDateTime& date);

	COleDateTime GetAnyStartDate() const { return dates.GetAnyStart(); }
	BOOL HasAnyStartDate() const { return dates.HasAnyStart(); }

	BOOL IsEndDateSet() const { return dates.IsEndSet(); }
	void SetDueDate(const COleDateTime& date) { dates.SetDue(date); }

	COleDateTime GetAnyEndDate() const { return dates.GetAnyEnd(); }
	COleDateTime GetDoneDate() const { return dates.GetDone(); }
	BOOL HasAnyEndDate() const { return dates.HasAnyEnd(); }

	void MinMax(COleDateTime& dtMin, COleDateTime& dtMax) const { dates.MinMax(dtMin, dtMax); }

	const TASKCALITEMDATES& Dates() const { return dates; }

	COleDateTime GetCustomDate(const CString& sCustAttribID) const { return dates.GetCustomDate(sCustAttribID); }
	void SetCustomDate(const CString& sCustAttribID, const COleDateTime& date) { dates.SetCustomDate(sCustAttribID, date); }
	void ClearCustomDate(const CString& sCustAttribID) { dates.ClearCustomDate(sCustAttribID); }
	void SetCustomDates(const CMapCustomDates& other) { dates.SetCustomDates(other); }

public:
	COLORREF color;
	BOOL bGoodAsDone, bTopLevel;
	BOOL bLocked, bHasDepends, bRecurring;

protected:
	CString sName, sFormattedName;
	CStringArray aTags;
	DWORD dwTaskID;
	BOOL bHasIcon, bIsParent;

	TASKCALITEMDATES dates;

protected:
	void ReformatName();
};

/////////////////////////////////////////////////////////////////////////////

struct TASKCALEXTENSIONITEM : public TASKCALITEM
{
	TASKCALEXTENSIONITEM(const TASKCALITEM& tciOrg, DWORD dwExtID);

	COLORREF GetFillColor(BOOL bTextIsBack) const;
	COLORREF GetBorderColor(BOOL bSelected, BOOL bTextIsBack) const;
	COLORREF GetTextColor(BOOL bSelected, BOOL bTextIsBack) const;

	const DWORD dwRealTaskID;
};
/////////////////////////////////////////////////////////////////////////////

struct TASKCALFUTUREOCURRENCE : public TASKCALEXTENSIONITEM
{
	TASKCALFUTUREOCURRENCE(const TASKCALITEM& tciOrg, DWORD dwExtID, const COleDateTimeRange& dtRange);
};

/////////////////////////////////////////////////////////////////////////////

struct TASKCALCUSTOMDATE : public TASKCALEXTENSIONITEM
{
	TASKCALCUSTOMDATE(const TASKCALITEM& tciOrg, DWORD dwExtID, const CString& sCustAttribID, const COleDateTime& date);

	void SetDate(const COleDateTime& date);
	COleDateTime GetDate() const { return dates.GetAnyStart(); }

	const CString sCustomAttribID;
};

/////////////////////////////////////////////////////////////////////////////

struct TASKCALFUTUREDATES
{
	// Range within which to retrieve occurrences
	COleDateTimeRange dtRange;

	int nNumOccurrences;
	COleDateTimeRange dtOccurrences[IUI_MAXNEXTOCCURRENCES];
};

/////////////////////////////////////////////////////////////////////////////

class CTaskCalItemMap : public CMap<DWORD, DWORD, TASKCALITEM*, TASKCALITEM*&>
{
public:
	virtual ~CTaskCalItemMap();

	void RemoveAll();
	void RemoveKey(DWORD dwTaskID);

	TASKCALITEM* GetTaskItem(DWORD dwTaskID) const;
	TASKCALITEM* GetNextTask(POSITION& pos) const;

	DWORD GetNextTaskID(POSITION& pos) const;
	BOOL HasTask(DWORD dwTaskID) const;
	BOOL IsParentTask(DWORD dwTaskID) const;

	static BOOL WantHideTask(const TASKCALITEM* pTCI, DWORD dwOptions, LPCTSTR szHideParentTag);

};

/////////////////////////////////////////////////////////////////////////////

class CTaskCalExtensionItemMap : public CTaskCalItemMap
{
public:
	DWORD GetRealTaskID(DWORD dwTaskID) const;
};

/////////////////////////////////////////////////////////////////////////////

class CTaskCalItemArray : public CArray<TASKCALITEM*, TASKCALITEM*&>
{
public:
	void SortItems(TDC_ATTRIBUTE nSortBy, BOOL bSortAscending);
	int FindItem(DWORD dwTaskID) const;
	int GetNextItem(DWORD dwTaskID, BOOL bForwards = TRUE) const;

protected:
	static int CompareItems(const void* pV1, const void* pV2);
};

/////////////////////////////////////////////////////////////////////////////

class CSortedTaskCalItemArray : protected CTaskCalItemArray
{
public:
	CSortedTaskCalItemArray(const CTaskCalItemMap& mapTasks);

	const CTaskCalItemArray& GetTasks();

	void SetNeedsRebuild();
	void SetNeedsResort(TDC_ATTRIBUTE nSortBy, BOOL bSortAscending);

protected:
	const CTaskCalItemMap& m_mapTasks;

	TDC_ATTRIBUTE m_nSortBy;
	BOOL m_bNeedsResort, m_bSortAscending;
};

/////////////////////////////////////////////////////////////////////////////

typedef CSet<double> CSpecialDateSet;

/////////////////////////////////////////////////////////////////////////////

class CHeatMap
{
public:
	CHeatMap(int nMinHeatCutoff = 5);

	void ClearHeat();
	BOOL HasHeat() const { return m_mapHeat.GetCount(); }

	BOOL SetColorPalette(const CDWordArray& aColors);
	BOOL Recalculate(const CTaskCalItemMap& mapData, TDC_ATTRIBUTE nAttribID, DWORD dwOption, LPCTSTR szHideParentTag);

	int GetHeat(const COleDateTime& date) const;
	COLORREF GetColor(const COleDateTime& date) const;

protected:
	CMap<double, double, int, int&> m_mapHeat;
	CDWordArray m_aColorPalette;
	int m_nMaxHeatCutoff;

	const int m_nMinHeatCutoff;
};

/////////////////////////////////////////////////////////////////////////////

struct CONTINUOUSDRAWINFO
{
	CONTINUOUSDRAWINFO(DWORD dwID = 0);

	void Reset();

	DWORD dwTaskID;
	int nIconOffset;
	int nTextOffset;
	int nVertPos;
};

class CCalContinuousDrawInfo : public CMap<DWORD, DWORD, CONTINUOUSDRAWINFO*, CONTINUOUSDRAWINFO*&>
{
public:
	CCalContinuousDrawInfo() {}
	virtual ~CCalContinuousDrawInfo();

	void RemoveAll();
	CONTINUOUSDRAWINFO& GetTaskInfo(DWORD dwTaskID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALSTRUCT_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
