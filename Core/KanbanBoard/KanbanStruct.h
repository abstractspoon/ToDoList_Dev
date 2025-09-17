// KanbanStruct.h: interface for the CKanbanStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANBANSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_KANBANSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Kanbanenum.h"

#include "..\shared\mapex.h"

#include "..\Interfaces\iuiextension.h"
#include "..\Interfaces\itasklist.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

typedef CArray<TDC_ATTRIBUTE, TDC_ATTRIBUTE&> CKanbanAttributeArray;

/////////////////////////////////////////////////////////////////////////////

struct KANBANCUSTOMATTRIBDEF
{
	KANBANCUSTOMATTRIBDEF();

	BOOL operator==(const KANBANCUSTOMATTRIBDEF& kca) const;

	CString sAttribID, sAttribName;
	BOOL bMultiValue;
	BOOL bDate;
};

/////////////////////////////////////////////////////////////////////////////

class CKanbanCustomAttributeDefinitionArray : public CArray<KANBANCUSTOMATTRIBDEF, KANBANCUSTOMATTRIBDEF&>
{
public:
	int AddDefinition(const CString& sAttribID, const CString& sAttribName, BOOL bMultiVal, BOOL bDate);
	BOOL HasDefinition(const CString& sAttribID) const;
	int FindDefinition(const CString& sAttribID) const;

	TDC_ATTRIBUTE GetDefinitionID(const CString& sAttribID) const;
	BOOL HasDefinition(TDC_ATTRIBUTE nAttribID) const;

	CString GetDefinitionID(TDC_ATTRIBUTE nAttribID) const;
	CString GetDefinitionLabel(TDC_ATTRIBUTE nAttribID) const;

protected:
	int FindDefinition(TDC_ATTRIBUTE nAttribID) const;
};

/////////////////////////////////////////////////////////////////////////////

namespace KBUtils
{
	CString FormatAttribute(TDC_ATTRIBUTE nAttribID, const CString& sValue, KBC_ATTRIBLABELS nLabelVis,
							const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs);
	CString FormatDate(const COleDateTime& date, BOOL bISODates);

	UINT GetDisplayFormat(TDC_ATTRIBUTE nAttribID, BOOL bLong);
	CString GetAttributeLabel(TDC_ATTRIBUTE nAttribID, KBC_ATTRIBLABELS nLabelVis,
							 const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs);

	CString GetAttributeID(TDC_ATTRIBUTE nAttribID);
	CString GetAttributeID(TDC_ATTRIBUTE nAttribID, const CKanbanCustomAttributeDefinitionArray& aCustAttribs);

	BOOL IsCustomAttribute(TDC_ATTRIBUTE nAttribID);
	BOOL IsTrackableAttribute(TDC_ATTRIBUTE nAttribID);
	BOOL IsSortableAttribute(TDC_ATTRIBUTE nAttribID);
	BOOL IsDateAttribute(TDC_ATTRIBUTE nAttribID, const CKanbanCustomAttributeDefinitionArray& aCustAttribs);

	BOOL IsTrackableAttribute(TDC_ATTRIBUTE nAttribID, const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs);
	BOOL IsGroupableAttribute(TDC_ATTRIBUTE nAttribID, const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs);

	BOOL IsPriorityOrRisk(const CString& sAttribID);
}

/////////////////////////////////////////////////////////////////////////////

class CKanbanValueMap : public CMapStringToString
{
public:
	BOOL HasValue(const CString& sValue) const;
	void AddValue(const CString& sValue);
	void AddValues(const CStringArray& aValues);
	void GetNextValue(POSITION& pos, CString& sValue) const;
	int GetValues(CStringArray& aValues) const;
	void SetValues(const CStringArray& aValues);
};
typedef CMapStringToContainer<CKanbanValueMap> CKanbanAttributeValueMap;

/////////////////////////////////////////////////////////////////////////////

struct KANBANITEM 
{ 
	KANBANITEM(DWORD dwID = 0);
	KANBANITEM(const KANBANITEM& ki);
	virtual ~KANBANITEM();
	
	KANBANITEM& operator=(const KANBANITEM& ki);
	BOOL operator==(const KANBANITEM& ki) const;

	DWORD dwTaskID, dwParentID;
	BOOL bDone, bGoodAsDone, bSomeSubtaskDone;
	BOOL bParent, bFlagged, bPinned;
	int nPercent, nLevel, nPosition;
	BOOL bLocked, bHasIcon;

	CString sTitle, sFullPosition;
	double dTimeEst, dTimeSpent, dTimeRemaining, dCost;
	TDC_UNITS nTimeEstUnits, nTimeSpentUnits, nTimeRemainingUnits;
	COleDateTime dtDone, dtDue, dtStart, dtLastMod, dtCreate;
	CString sExternalID, sRecurrence, sCreatedBy;
	CStringArray aFileLinks;

	int GetTrackedAttributeValues(const CString& sAttribID, DWORD dwOptions, CStringArray& aValues) const;
	BOOL HasTrackedAttributeValues(const CString& sAttribID) const;
	CString GetAttributeDisplayValue(TDC_ATTRIBUTE nAttribID, BOOL bISODates = FALSE) const;
	CString GetAttributeDisplayValue(TDC_ATTRIBUTE nAttribID, const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs, BOOL bISODates = FALSE) const;
	BOOL HasAttributeDisplayValue(TDC_ATTRIBUTE nAttribID) const;
	COLORREF GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	COLORREF GetFillColor(BOOL bColorIsBkgnd) const;
	COLORREF GetBorderColor(BOOL bColorIsBkgnd) const;
	int GetPriority(DWORD dwOptions) const;
	int GetRisk(DWORD dwOptions) const;
	BOOL HasDueOrDonePriorityOrRisk(DWORD dwOptions) const;

	BOOL HasColor() const;
	BOOL IsDone(BOOL bIncludeGoodAs) const;
	BOOL IsDue() const;
	BOOL AttributeValuesMatch(const CString& sAttribID, const KANBANITEM& ki) const;
	BOOL AttributeValuesMatch(const CString& sAttribID, const CStringArray& aValues) const;
	BOOL MatchesAttribute(const IUISELECTTASK& select) const;

	void AddTrackedAttributeValue(const CString& sAttribID, LPCTSTR szValue);
	void RemoveTrackedAttributeValue(const CString& sAttribID, LPCTSTR szValue);
	void RemoveTrackedAttributeValues(const CString& sAttribID, const CStringArray& aValues);
	void RemoveAllTrackedAttributeValues(const CString& sAttribID);
	void SetTrackedAttributeValue(const CString& sAttribID, LPCTSTR szValue);
	void SetTrackedAttributeValue(TDC_ATTRIBUTE nAttribID, LPCTSTR szValue);
	void SetTrackedAttributeValues(const CString& sAttribID, const CStringArray& aValues);
	void SetTrackedAttributeValues(TDC_ATTRIBUTE nAttribID, const CStringArray& aValues);
	void SetTrackedAttributeValue(TDC_ATTRIBUTE nAttribID, int nValue);
	void SetColor(COLORREF cr);

protected:
	CMapStringToStringArray mapAttribValues;
	COLORREF color;

protected:
	CString GetTrackedAttributeValue(const CString& sAttribID) const;
	int GetPriorityOrRisk(TDC_ATTRIBUTE nAttribID, DWORD dwOptions) const;
};
typedef CArray<const KANBANITEM*, const KANBANITEM*> CKanbanItemArray;

/////////////////////////////////////////////////////////////////////////////

class CKanbanItemArrayMap : public CMapStringToContainer<CKanbanItemArray>
{
public:
	virtual ~CKanbanItemArrayMap();

	void RemoveAll();
};

/////////////////////////////////////////////////////////////////////////////

class CKanbanItemMap : public CMap<DWORD, DWORD, KANBANITEM*, KANBANITEM*&>
{
public:
	virtual ~CKanbanItemMap();

	void RemoveAll();
	BOOL RemoveKey(DWORD dwKey);
	BOOL HasItem(DWORD dwTaskID) const;
	BOOL IsLocked(DWORD dwTaskID) const;
	BOOL IsFlagged(DWORD dwTaskID) const;
	BOOL IsPinned(DWORD dwTaskID) const;
	BOOL IsDone(DWORD dwTaskID, BOOL bIncGoodAsDone = FALSE) const;
	BOOL IsTopLevel(DWORD dwTaskID) const;

	CString GetItemTitle(DWORD dwTaskID) const;

	KANBANITEM* GetItem(DWORD dwTaskID) const;
	KANBANITEM* GetParentItem(DWORD dwTaskID) const;
	KANBANITEM* GetParentItem(const KANBANITEM* pKI) const;
	KANBANITEM* NewItem(DWORD dwTaskID, const CString& sTitle);

	void RemoveDeletedItems(const CDWordSet& mapCurIDs);
	int BuildTempItemMaps(const CString& sAttribID, DWORD dwOptions, int nNumPriorityRiskLevels, CKanbanItemArrayMap& map) const;

	int GetPinnedItems(CDWordArray& aTaskIDs) const;
	void SetPinnedItems(const CDWordArray& aTaskIDs, BOOL bReset = TRUE);
	void ClearPinnedItems();

	BOOL IsParent(const KANBANITEM* pKIParent, const KANBANITEM* pKIChild) const;
	BOOL CalcInheritedPinState(const KANBANITEM* pKI) const;
	BOOL HasSameParent(const KANBANITEM* pKI1, const KANBANITEM* pKI2) const;
		
#ifdef _DEBUG
	void TraceSummary(const CString& sAttribID, DWORD dwOptions) const;
#endif

protected:
	DWORD GetNextKey(POSITION& pos);

	static void AddItemToMap(const KANBANITEM* pKI, const CString& sValue, CKanbanItemArrayMap& map);
};

/////////////////////////////////////////////////////////////////////////////

struct KANBANCOLUMN
{
	KANBANCOLUMN();
	KANBANCOLUMN(const KANBANCOLUMN& kc);

	KANBANCOLUMN& operator=(const KANBANCOLUMN& kc);
	BOOL operator==(const KANBANCOLUMN& kc) const;

	BOOL AttributeMatches(const KANBANCOLUMN& kc) const;
	BOOL Matches(const KANBANCOLUMN& kc, BOOL bIncDisplayAttribs = TRUE) const;

	int GetAttributeValueIDs(CStringArray& aAttribValueIDs) const;

	CString sTitle;
	CString sAttribID;
	CStringArray aAttribValues;
	int nMaxTaskCount;
	COLORREF crBackground;
};

/////////////////////////////////////////////////////////////////////////////

class CKanbanColumnArray : public CArray<KANBANCOLUMN, KANBANCOLUMN&>
{
public:
	BOOL MatchesAll(const CKanbanColumnArray& other, BOOL bIncDisplayAttribs = TRUE) const;

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_KANBANSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
