// KanbanStruct.h: interface for the CKanbanStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANBANSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
#define AFX_KANBANSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
};

class CKanbanCustomAttributeDefinitionArray : public CArray<KANBANCUSTOMATTRIBDEF, KANBANCUSTOMATTRIBDEF&>
{
public:
	int AddDefinition(const CString& sAttribID, const CString& sAttribName, BOOL bMultiVal = FALSE);
	BOOL HasDefinition(const CString& sAttribID) const;
	int FindDefinition(const CString& sAttribID) const;

	BOOL SetMultiValue(int nDef, BOOL bMultiVal = TRUE);
};

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
	BOOL bParent, bFlag;
	int nPercent, nLevel, nPosition;
	BOOL bLocked, bHasIcon;

	CString sTitle, sCost;
	double dTimeEst, dTimeSpent;
	TDC_UNITS nTimeEstUnits, nTimeSpentUnits;
	COleDateTime dtDone, dtDue, dtStart, dtLastMod, dtCreate;
	CString sExternalID, sRecurrence, sCreatedBy, sFileRef;

	int GetTrackedAttributeValues(LPCTSTR szAttrib, CStringArray& aValues) const;
	BOOL HasTrackedAttributeValues(LPCTSTR szAttrib) const;
	CString GetAttributeDisplayValue(TDC_ATTRIBUTE nAttrib) const;
	BOOL HasAttributeDisplayValue(TDC_ATTRIBUTE nAttrib) const;
	COLORREF GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	COLORREF GetFillColor(BOOL bColorIsBkgnd) const;
	COLORREF GetBorderColor(BOOL bColorIsBkgnd) const;
	int GetPriority() const;

	BOOL HasColor() const;
	BOOL IsDone(BOOL bIncludeGoodAs) const;
	BOOL AttributeValuesMatch(LPCTSTR szAttrib, const KANBANITEM& ki) const;
	BOOL AttributeValuesMatch(LPCTSTR szAttrib, const CStringArray& aValues) const;
	BOOL MatchesAttribute(const IUISELECTTASK& select) const;

	void AddTrackedAttributeValue(LPCTSTR szAttrib, LPCTSTR szValue);
	void RemoveTrackedAttributeValue(LPCTSTR szAttrib, LPCTSTR szValue);
	void RemoveTrackedAttributeValues(LPCTSTR szAttrib, const CStringArray& aValues);
	void RemoveAllTrackedAttributeValues(LPCTSTR szAttrib);
	void SetTrackedAttributeValue(LPCTSTR szAttrib, LPCTSTR szValue);
	void SetTrackedAttributeValue(TDC_ATTRIBUTE nAttribID, LPCTSTR szValue);
	void SetTrackedAttributeValues(LPCTSTR szAttrib, const CStringArray& aValues);
	void SetTrackedAttributeValues(TDC_ATTRIBUTE nAttribID, const CStringArray& aValues);
	void SetTrackedAttributeValue(TDC_ATTRIBUTE nAttribID, int nValue);
	void SetColor(COLORREF cr);
	
	static CString GetAttributeID(TDC_ATTRIBUTE nAttrib);
	static BOOL IsTrackableAttribute(TDC_ATTRIBUTE nAttrib);

protected:
	CMapStringToStringArray mapAttribValues;
	COLORREF color;

protected:
	CString GetTrackedAttributeValue(LPCTSTR szAttrib) const;
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
	BOOL HasFlag(DWORD dwTaskID) const;

	CString GetItemTitle(DWORD dwTaskID) const;

	KANBANITEM* GetItem(DWORD dwTaskID) const;
	KANBANITEM* NewItem(DWORD dwTaskID, const CString& sTitle);

	void RemoveDeletedItems(const CDWordSet& mapCurIDs);
	int BuildTempItemMaps(LPCTSTR szAttribID, CKanbanItemArrayMap& map) const;
		
#ifdef _DEBUG
	void TraceSummary(LPCTSTR szAttribID) const;
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
	COLORREF crExcess;
};

class CKanbanColumnArray : public CArray<KANBANCOLUMN, KANBANCOLUMN&>
{
public:
	BOOL MatchesAll(const CKanbanColumnArray& other, BOOL bIncDisplayAttribs = TRUE) const;

};

/////////////////////////////////////////////////////////////////////////////

struct KANBANSORT
{
	KANBANSORT(const CKanbanItemMap& map);

	BOOL IsParent(DWORD dwTaskID, const KANBANITEM* pKIChild) const;
	const KANBANITEM* GetParent(const KANBANITEM* pKIChild) const;

	const CKanbanItemMap& data;
	TDC_ATTRIBUTE nBy;
	CString sAttribID;
	BOOL bAscending;
	BOOL bSubtasksBelowParent;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_KANBANSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
