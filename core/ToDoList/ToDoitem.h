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

#include <afxtempl.h>

///////////////////////////////////////////////////////////////////////////////////

const int TDC_NOPRIORITYORISK = -2;
const int TDC_MINPRIORITYORISK = 0;
const int TDC_MAXPRIORITYORISK = 10;

///////////////////////////////////////////////////////////////////////////////////

struct TDCTIMEPERIOD
{
	TDCTIMEPERIOD(double dAmount = 0.0, TDC_UNITS nUnits = TDCU_HOURS);

	BOOL operator==(const TDCTIMEPERIOD& other) const;
	TDCTIMEPERIOD& operator=(const TDCTIMEPERIOD& other);

	TH_UNITS GetTHUnits() const;
	BOOL SetTHUnits(TH_UNITS nTHUnits);

	double GetTime(TH_UNITS nUnits) const;
	double GetTime(TH_UNITS nUnits, const CTimeHelper& th) const;

	BOOL SetUnits(TDC_UNITS nNewUnits, BOOL bRecalc);
	BOOL AddTime(double dAmount, TDC_UNITS nUnits);

	double dAmount;
	TDC_UNITS nUnits;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCOST
{
	TDCCOST(LPCTSTR szCost);
	TDCCOST(double dCost = 0.0, BOOL bCostIsRate = FALSE);

	BOOL operator==(const TDCCOST& other) const;
	TDCCOST& operator=(const TDCCOST& other);

	CString Format() const;
	BOOL Parse(LPCTSTR szCost);

	static CString Format(double dAmount, BOOL bIsRate);

	double dAmount;
	BOOL bIsRate;
};

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
	CString GetDependency(int Depends) const;
	CString GetTag(int nTag) const;
	CString GetFileRef(int nFile) const;

	BOOL GetNextOccurence(COleDateTime& dtNext, BOOL& bDue);
	BOOL IsRecurring() const;
	BOOL CanRecur() const;
	BOOL RecurrenceMatches(const TODOITEM& tdi, BOOL bIncludeRemainingOccurrences) const;
	
	BOOL IsRecentlyModified() const; // 1 hour default
	COleDateTimeSpan GetRemainingDueTime() const; // in days
	COleDateTime GetDate(TDC_DATE nDate) const;
	
	BOOL HasCustomAttributeValue(const CString& sAttribID) const;
	BOOL GetCustomAttributeValue(const CString& sAttribID, TDCCADATA& data) const;
	void SetCustomAttributeValue(const CString& sAttribID, const TDCCADATA& data);
	const CTDCCustomAttributeDataMap& GetCustomAttributeValues() const { return mapCustomData; }
	void SetCustomAttributeValues(const CTDCCustomAttributeDataMap& data) { mapCustomData.Copy(data); }

	CString GetMetaData(const CString& sKey) const;
	void SetMetaData(const CString& sKey, const CString& sData);
	const CTDCMetaDataMap& GetMetaData() const { return mapMetaData; }
	void SetMetaData(const CTDCMetaDataMap& mapData) { mapMetaData.Copy(mapData); }

	// only applies to dependencies within this tasklist
	BOOL RemoveLocalDependency(DWORD dwDependID);
	BOOL IsLocallyDependentOn(DWORD dwDependID) const;
	int GetLocalDependencies(CDWordArray& aDependIDs) const;
	DWORD GetFirstLocalDependency() const;

	float GetCommentsSizeInKB() const;
	
	static COleDateTimeSpan GetRemainingDueTime(const COleDateTime& date); // in days
	static BOOL HasTime(const COleDateTime& date);
	static BOOL ParseTaskLink(const CString& sLink, DWORD& dwTaskID, CString& sFile);
	static CString FormatTaskDependency(DWORD dwTaskID, const CString& sFile = _T(""));
	static void SetRecentlyModifiedPeriod(double dDays);
	static void SetModifierName(const CString sModifier);
	static BOOL IsRecentlyModified(const COleDateTime& date);
	static BOOL IsValidPriorityOrRisk(int nValue);

	// Attributes
	CString sTitle;
	CString sComments;
	CString sCommentsTypeID;
	CString sAllocBy;
	CString sStatus;
	CString sCreatedBy;
	CString sExternalID;
	CString sVersion;
	CString sIcon;
	CString sLastModifiedBy;
	
	CStringArray aAllocTo;
	CStringArray aCategories;
	CStringArray aDependencies;
	CStringArray aTags;
	CStringArray aFileLinks;
	
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
	int FindLocalDependency(DWORD dwDependID, int nSearchFrom = 0) const;
	
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
	POSITION GetStartPosition() const;
	void GetNextAssoc(POSITION& rNextPosition, DWORD& dwTaskID, TODOITEM*& pTDI) const;

	void DeleteAll() { CleanUp(); }
	BOOL DeleteTask(DWORD dwTaskID);
	BOOL AddTask(DWORD dwTaskID, TODOITEM* pTDI);
	BOOL SetTask(DWORD dwTaskID, const TODOITEM* pTDI);

protected:
	void CleanUp();
};

///////////////////////////////////////////////////////////////////////////////////

class TODOSTRUCTURE
{
	friend class CToDoCtrlDataStructure;
	
public:
	TODOSTRUCTURE() : m_dwID(0), m_pTDSParent(NULL) {}
	TODOSTRUCTURE(DWORD dwID);
	~TODOSTRUCTURE();
	
	DWORD GetTaskID() const { return m_dwID; }
	DWORD GetSubTaskID(int nPos) const;
	BOOL HasSubTask(DWORD dwSubtaskID, BOOL bImmediate = TRUE) const;
	
	int GetSubTaskPosition(DWORD dwID) const;
	int GetPosition() const;
	
	TODOSTRUCTURE* GetParentTask() const;
	DWORD GetParentTaskID() const;
	BOOL HasSameParent(const TODOSTRUCTURE* pTDS) const;
	
	BOOL ParentIsRoot() const { return (GetParentTaskID() == 0); }
	BOOL IsRoot() const { return (GetTaskID() == 0); }
	
	DWORD GetPreviousSubTaskID(int nPos);
	
	int GetSubTaskCount() const { return m_aSubTasks.GetSize(); }
	BOOL HasSubTasks() const { return GetSubTaskCount() > 0; }
	int GetLeafCount() const;
	
	TODOSTRUCTURE* GetSubTask(int nPos) const;
	int GetSubTaskPosition(const TODOSTRUCTURE* pTDS) const;
	
	void DeleteAll() { CleanUp(); }
	
	int MoveSubTask(int nPos, TODOSTRUCTURE* pTDSDestParent, int nDestPos);
	
protected:
	DWORD m_dwID;
	TODOSTRUCTURE* m_pTDSParent;
	CArray<TODOSTRUCTURE*, TODOSTRUCTURE*&> m_aSubTasks; 
	
protected:
	// For CToDoCtrlDataStructure only
	TODOSTRUCTURE(const TODOSTRUCTURE& tds);
	const TODOSTRUCTURE& operator=(const TODOSTRUCTURE& tds); 
	
private:
	void CleanUp();
	BOOL DeleteSubTask(int nPos);
	BOOL InsertSubTask(TODOSTRUCTURE* pTDS, int nPos);
	BOOL AddSubTask(TODOSTRUCTURE* pTDS);
	TODOSTRUCTURE* AddSubTask(DWORD dwID);
};

///////////////////////////////////////////////////////////////////////////////////

class CToDoCtrlDataStructure : public TODOSTRUCTURE
{
public:
	CToDoCtrlDataStructure() : TODOSTRUCTURE() {}
	CToDoCtrlDataStructure(const CToDoCtrlDataStructure& tds); 
	~CToDoCtrlDataStructure();
	
	TODOSTRUCTURE* GetParentTask(DWORD dwID) const;
	DWORD GetParentTaskID(DWORD dwID) const;
	DWORD GetPreviousTaskID(DWORD dwID) const;
	
	TODOSTRUCTURE* AddTask(DWORD dwID, TODOSTRUCTURE* pTDSParent);
	BOOL InsertTask(DWORD dwID, TODOSTRUCTURE* pTDSParent, int nPos);
	
	TODOSTRUCTURE* FindTask(DWORD dwID) const;
	BOOL FindTask(DWORD dwID, TODOSTRUCTURE*& pTDSParent, int& nPos) const;

	BOOL DeleteTask(DWORD dwID);
	void DeleteAll();
	
protected:
	CMap<DWORD, DWORD, TODOSTRUCTURE*, TODOSTRUCTURE*&> m_mapStructure;
	
protected:
	void BuildMap();
	void AddToMap(const TODOSTRUCTURE* pTDS);
	void RemoveFromMap(const TODOSTRUCTURE* pTDS);

	const CToDoCtrlDataStructure& operator=(const CToDoCtrlDataStructure& tds); 
	
	BOOL InsertTask(TODOSTRUCTURE* pTDS, TODOSTRUCTURE* pTDSParent, int nPos);
};

///////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TODOITEM_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
