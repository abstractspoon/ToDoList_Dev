// ToDoitem.h: interface for the CToDoitem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOITEM_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
#define AFX_TODOITEM_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////////

#include "tdcstruct.h"
#include "tdccustomattribdata.h"
#include "tdcrecurrence.h"

#include "..\shared\binarydata.h"
#include "..\shared\timehelper.h"

#include "..\interfaces\ITasklist.h"
#include "..\interfaces\ContentCtrl.h"

#include <afxtempl.h>

///////////////////////////////////////////////////////////////////////////////////

const int TDC_NOPRIORITYORISK = -2;
const int TDC_MINPRIORITYORISK = 0;
const int TDC_MAXPRIORITYORISK = 10;

///////////////////////////////////////////////////////////////////////////////////

struct TDCTIMEPERIOD
{
	// Note: Different units
	TDCTIMEPERIOD(double dAmount = 0.0, TDC_UNITS nUnits = TDCU_HOURS);
	TDCTIMEPERIOD(double dAmount, TH_UNITS nUnits);
	
	TDCTIMEPERIOD& operator=(const TDCTIMEPERIOD& other);

	BOOL operator==(const TDCTIMEPERIOD& other) const;
	BOOL operator!=(const TDCTIMEPERIOD& other) const;

	CString Format(int nDecPlaces) const;
	TH_UNITS GetTHUnits() const;

	double GetTime(TH_UNITS nUnits) const;
	double GetTime(TH_UNITS nUnits, const CTimeHelper& th) const;

	BOOL SetTHUnits(TH_UNITS nTHUnits, BOOL bRecalc);
	BOOL SetUnits(TDC_UNITS nNewUnits, BOOL bRecalc);
	BOOL HasValidUnits() const;

	BOOL SetTime(double dTime, TDC_UNITS nUnits);
	BOOL AddTime(double dTime, TDC_UNITS nUnits);
	BOOL AddTime(const TDCTIMEPERIOD& time);

	double dAmount;
	TDC_UNITS nUnits;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCOST
{
	TDCCOST(LPCTSTR szCost);
	TDCCOST(double dCost = 0.0, BOOL bCostIsRate = FALSE);

	TDCCOST& operator=(const TDCCOST& other);

	BOOL operator==(const TDCCOST& other) const;
	BOOL operator!=(const TDCCOST& other) const;

	CString Format(int nDecimals = -1) const;
	BOOL Parse(LPCTSTR szCost);

	BOOL SetCost(double dCost, BOOL bCostIsRate);
	BOOL AddCost(double dCost, BOOL bCostIsRate);
	BOOL AddCost(const TDCCOST& cost);
	BOOL AddCost(double dCost);

	static CString Format(double dAmount, BOOL bIsRate, int nDecimals = -1);

	double dAmount;
	BOOL bIsRate;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCDEPENDENCY
{
	TDCDEPENDENCY(LPCTSTR szDepends);
	TDCDEPENDENCY(DWORD dwDependsID = 0, const CString& sFile = _T(""));

	TDCDEPENDENCY& operator=(const TDCDEPENDENCY& other);

	BOOL operator==(const TDCDEPENDENCY& other) const;
	BOOL operator!=(const TDCDEPENDENCY& other) const;

	CString Format(const CString& sFolder = _T(""), BOOL bUrl = FALSE) const;
	CString GetFullPath(const CString& sFolder) const;

	BOOL Parse(LPCTSTR szDepends);
	BOOL IsLocal() const;
	BOOL IsValid() const;

	static CString Format(DWORD dwTaskID, const CString& sFile = _T(""), const CString& sFolder = _T(""), BOOL bUrl = FALSE);
	static BOOL IsValid(DWORD dwTaskID, const CString& sFile = _T(""));

	DWORD dwTaskID;
	CString sTasklist;
	int nDaysLeadIn;
};

class CTDCDependencyArray : public CArray<TDCDEPENDENCY, TDCDEPENDENCY&>
{
public:
	CTDCDependencyArray();

	int Set(const CDWordArray& aDepends);
	int Set(const CStringArray& aDepends);

	BOOL Add(DWORD dwDependID, const CString& sFile = _T(""));
	BOOL Add(const TDCDEPENDENCY& depend);
	BOOL Remove(const TDCDEPENDENCY& depend);
	int Append(const CTDCDependencyArray& aDepends);
	int Remove(const CTDCDependencyArray& aDepends);

	BOOL RemoveLocalDependency(DWORD dwDependID);
	BOOL ReplaceLocalDependency(DWORD dwOldID, DWORD dwNewID);
	BOOL HasLocalDependency(DWORD dwDependID) const;
	int GetLocalDependencies(CDWordArray& aDependIDs) const;

	int GetDependencies(CDWordArray& aLocalDepends, CStringArray& aOtherDepends) const;
	BOOL HasDependency(const TDCDEPENDENCY& depend) const;

	int Format(CStringArray& aDepends, const CString& sFolder = _T("")) const;
	CString Format(LPCTSTR szSep = NULL, const CString& sFolder = _T("")) const;
	
	BOOL MatchAll(const CTDCDependencyArray& other, BOOL bIncludeAttributes = TRUE) const;

	// Mfc42 versions return value not reference
	const TDCDEPENDENCY& GetAt(int nIndex) const;
	TDCDEPENDENCY& GetAt(int nIndex);
	const TDCDEPENDENCY& operator[](int nIndex) const;
	TDCDEPENDENCY& operator[](int nIndex);

private:
	int FindLocalDependency(DWORD dwDependID) const;
	int FindDependency(const TDCDEPENDENCY& depend) const;
};

/////////////////////////////////////////////////////////////////////////////////////////////

namespace TDCTASKLINK
{
	BOOL IsTaskLink(const CString& sLink, BOOL bURL);
	BOOL Parse(const CString& sLink, BOOL bURL, const CString& sFolder, DWORD& dwTaskID, CString& sFile);
	CString Format(DWORD dwTaskID, BOOL bURL, const CString& sFile = _T(""));
}

/////////////////////////////////////////////////////////////////////////////////////////////

struct TODOITEM
{
	friend class CToDoCtrlData;
	
public:
	TODOITEM(LPCTSTR szTitle = NULL, LPCTSTR szComments = NULL); 
	TODOITEM(const TODOITEM& tdi); 
	
	TODOITEM& operator=(const TODOITEM& tdi); 
	
	BOOL operator==(const TODOITEM& tdi); 
	BOOL operator!=(const TODOITEM& tdi); 
	
	// helpers
	BOOL HasLastMod() const;
	BOOL HasCreation() const;
	BOOL HasStart() const;
	BOOL HasStartTime() const;
	BOOL HasDue() const;
	BOOL HasDueTime() const;
	BOOL IsDone() const;
	BOOL HasDoneTime() const;
	
	void ClearStart();
	void ClearDue();
	void ClearDone();
	
	BOOL IsDue() const;
	BOOL IsDue(const COleDateTime& dateDueBy) const;
	
	void SetModified();
	BOOL IsReference() const;
	
	CString GetCategory(int nCat) const;
	CString GetAllocTo(int nAllocTo) const;
	CString GetTag(int nTag) const;
	CString GetFileLink(int nFile) const;

	BOOL GetNextOccurence(COleDateTime& dtNext, BOOL& bDue);
	BOOL CalcNextOccurences(const COleDateTimeRange& dtRange, CArray<double, double&>& aDates, BOOL& bDue) const;
	BOOL IsRecurring() const;
	BOOL CanRecur() const;
	BOOL RecurrenceMatches(const TODOITEM& tdi, BOOL bIncludeRemainingOccurrences) const;
	
	BOOL IsRecentlyModified() const; // 1 hour default
	COleDateTimeSpan GetRemainingDueTime() const; // in days
	COleDateTime GetDate(TDC_DATE nDate) const;
	BOOL GetStartDueDates(COleDateTimeRange& dtRange) const;

	BOOL HasCustomAttributeValue(const CString& sAttribID) const;
	BOOL GetCustomAttributeValue(const CString& sAttribID, TDCCADATA& data) const;
	void SetCustomAttributeValue(const CString& sAttribID, const TDCCADATA& data);
	BOOL HasCustomAttributeValues() const { return mapCustomData.GetCount(); }
	const CTDCCustomAttributeDataMap& GetCustomAttributeValues() const { return mapCustomData; }
	void SetCustomAttributeValues(const CTDCCustomAttributeDataMap& data) { mapCustomData.Copy(data); }

	CString GetMetaData(const CString& sKey) const;
	void SetMetaData(const CString& sKey, const CString& sData);
	const CTDCMetaDataMap& GetMetaData() const { return mapMetaData; }
	void SetMetaData(const CTDCMetaDataMap& mapData) { mapMetaData.Copy(mapData); }

	// only applies to dependencies within this tasklist
	BOOL RemoveLocalDependency(DWORD dwDependID);
	BOOL ReplaceLocalDependency(DWORD dwOldID, DWORD dwNewID);
	BOOL HasLocalDependency(DWORD dwDependID) const;
	int GetLocalDependencies(CDWordArray& aDependIDs) const;

	float GetCommentsSizeInKB() const;

	BOOL GetAttributeValues(TDC_ATTRIBUTE nAttribID, TDCCADATA& data) const;
	BOOL HasAttributeValue(TDC_ATTRIBUTE nAttribID) const;
		
	static COleDateTimeSpan GetRemainingTime(const COleDateTime& date); // in days
	static BOOL HasTime(const COleDateTime& date);
	static void SetRecentlyModifiedPeriod(double dDays);
	static void SetModifierName(const CString sModifier);
	static BOOL IsRecentlyModified(const COleDateTime& date);
	static BOOL IsValidPriorityOrRisk(int nValue);

	// Attributes
	CString sTitle;
	CString sComments;
	CString sAllocBy;
	CString sStatus;
	CString sCreatedBy;
	CString sExternalID;
	CString sVersion;
	CString sIcon;
	CString sLastModifiedBy;
	
	CStringArray aAllocTo;
	CStringArray aCategories;
	CStringArray aTags;
	CStringArray aFileLinks;

	CTDCDependencyArray aDependencies;
	
	COleDateTime dateStart;
	COleDateTime dateDue;
	COleDateTime dateDone;
	COleDateTime dateCreated;
	COleDateTime dateLastMod;
	
	int nPriority;
	int nPercentDone;
	int nRisk;
	
	TDCCOST cost;
	TDCTIMEPERIOD timeEstimate, timeSpent;
	CONTENTFORMAT cfComments;
	
	BOOL bFlagged;
	BOOL bLocked;

	COLORREF color;
	CBinaryData customComments;
	TDCRECURRENCE trRecurrence;
	DWORD dwTaskRefID;
	
	const static COleDateTime dtUseCreationDateOnly;
	const static COleDateTime dtUseCreationDateAndTime;

protected:
	// custom attributes
	CTDCCustomAttributeDataMap mapCustomData;
	
	// meta-data for 3rd-party applications only
	CTDCMetaDataMap mapMetaData; 

private:
	static COleDateTime GetDefaultStartDueDate(const COleDateTime& dtCreation, const COleDateTime& dtStartDue);
	
	static COleDateTimeSpan dtsRecentModPeriod;
	static CString sModifierName;
};

//////////////////////////////////////////////////////////////////////

class CToDoCtrlDataItems : protected CMap<DWORD, DWORD, TODOITEM*, TODOITEM*&>
{
public:
	CToDoCtrlDataItems();
	virtual ~CToDoCtrlDataItems();

	int GetCount() const;
	TODOITEM* GetTask(DWORD dwTaskID) const;
	BOOL HasTask(DWORD dwTaskID) const;

	POSITION GetStart() const;
	void GetNext(POSITION& rNextPosition, DWORD& dwTaskID, TODOITEM*& pTDI) const;
	DWORD GetNextTask(POSITION& pos, const TODOITEM*& pTDI) const;
	DWORD GetNextTaskID(POSITION& pos) const;

	void DeleteAll() { CleanUp(); }
	BOOL DeleteTask(DWORD dwTaskID);
	BOOL AddTask(DWORD dwTaskID, TODOITEM* pTDI);
	BOOL SetTask(DWORD dwTaskID, const TODOITEM* pTDI);

protected:
	void CleanUp();
};

///////////////////////////////////////////////////////////////////////////////////

// This is a const class publicly because we don't want any changes 
// to be made without proper supervision (aka CToDoCtrlDataStructure)
class TODOSTRUCTURE
{
	friend class CToDoCtrlDataStructure;

public:
	TODOSTRUCTURE() : m_dwID(0), m_pTDSParent(NULL) {}
	TODOSTRUCTURE(DWORD dwID);
	~TODOSTRUCTURE();
	
	DWORD GetTaskID() const { return m_dwID; }
	DWORD GetSubTaskID(int nPos) const;
	DWORD GetPreviousSubTaskID(int nPos) const;
	DWORD GetNextSubTaskID(int nPos) const;
	BOOL HasParent(DWORD dwParentID, BOOL bImmediate = TRUE) const;
		
	TODOSTRUCTURE* GetParentTask() const;
	DWORD GetParentTaskID() const;
	int GetParentTaskIDs(CDWordArray& aParentIDs) const;
	BOOL HasSameParent(const TODOSTRUCTURE* pTDS) const;
	
	BOOL ParentIsRoot() const { return (GetParentTaskID() == 0); }
	BOOL IsRoot() const { return (GetTaskID() == 0); }
	
	TODOSTRUCTURE* GetSubTask(int nPos) const;
	BOOL CanMoveSubTask(int nPos, const TODOSTRUCTURE* pTDSDestParent, int nDestPos) const;

	int GetSubTaskCount() const { return m_aSubTasks.GetSize(); }
	BOOL HasSubTasks() const { return GetSubTaskCount() > 0; }
	int GetLeafCount() const;
	
#ifdef _DEBUG
	void Trace(const CString& sIndent) const;
#endif
	
protected:
	DWORD m_dwID;
	TODOSTRUCTURE* m_pTDSParent;
	CArray<TODOSTRUCTURE*, TODOSTRUCTURE*&> m_aSubTasks; 
	
private:
	// For CToDoCtrlDataStructure only ----------------------
	TODOSTRUCTURE(const TODOSTRUCTURE& tds);
	const TODOSTRUCTURE& operator=(const TODOSTRUCTURE& tds); 

	int MoveSubTask(int nPos, TODOSTRUCTURE* pTDSDestParent, int nDestPos);
	void DeleteAll() { CleanUp(); }
	void CleanUp();
	BOOL DeleteSubTask(int nPos);
	BOOL InsertSubTask(TODOSTRUCTURE* pTDS, int nPos);
	TODOSTRUCTURE* InsertSubTask(DWORD dwID, int nPos);
	// ------------------------------------------------------

#ifdef _DEBUG
	int GetTaskPosition(DWORD dwID) const;
#endif
};

///////////////////////////////////////////////////////////////////////////////////

class CToDoCtrlDataStructure : public TODOSTRUCTURE
{
public:
	CToDoCtrlDataStructure();
	CToDoCtrlDataStructure(const CToDoCtrlDataStructure& tds); 
	~CToDoCtrlDataStructure();
	
	TODOSTRUCTURE* GetParentTask(DWORD dwID) const;
	DWORD GetParentTaskID(DWORD dwID) const;
	DWORD GetPreviousTaskID(DWORD dwID) const;
	
	TODOSTRUCTURE* AddTask(DWORD dwID, TODOSTRUCTURE* pTDSParent);
	TODOSTRUCTURE* InsertTask(DWORD dwID, TODOSTRUCTURE* pTDSParent, int nPos);
	
	TODOSTRUCTURE* FindTask(DWORD dwID) const;
	BOOL FindTask(DWORD dwID, TODOSTRUCTURE*& pTDSParent, int& nPos) const;

	int GetTaskPosition(DWORD dwTaskID, BOOL bZeroBased = TRUE) const;
	int GetTaskPositions(DWORD dwTaskID, CArray<int, int>& aPositions, BOOL bZeroBased = TRUE) const;

	int MoveSubTask(TODOSTRUCTURE* pTDSSrcParent, int nSrcPos, TODOSTRUCTURE* pTDSDestParent, int nDestPos);
	BOOL CanMoveSubTask(const TODOSTRUCTURE* pTDSSrcParent, int nSrcPos, const TODOSTRUCTURE* pTDSDestParent, int nDestPos) const;

	BOOL DeleteTask(DWORD dwID);
	void DeleteAll();

	int GetTaskIDs(CDWordArray& aTaskIDs) const;
	
protected:
	CMap<DWORD, DWORD, TODOSTRUCTURE*, TODOSTRUCTURE*&> m_mapStructure;

	mutable	CMap<DWORD, DWORD, int, int> m_mapSubtaskPositions;
		
protected:
	void BuildStructureMap();
	void AddToStructureMap(const TODOSTRUCTURE* pTDS);
	void RemoveFromStructureMap(const TODOSTRUCTURE* pTDS);

	// These must be callable from const Getters
	void BuildPositionMap() const;
	void AddSubtasksToPositionMap(const TODOSTRUCTURE* pTDS) const;
	void ClearPositionMap() { m_mapSubtaskPositions.RemoveAll(); }

	const CToDoCtrlDataStructure& operator=(const CToDoCtrlDataStructure& tds); 
	
	static void AddTaskIDs(TODOSTRUCTURE* pTDS, CDWordArray& aTaskIDs);
};

///////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TODOITEM_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
