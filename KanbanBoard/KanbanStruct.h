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

typedef CArray<IUI_ATTRIBUTE, IUI_ATTRIBUTE&> CKanbanAttributeArray;

/////////////////////////////////////////////////////////////////////////////

class CKanbanValueMap : public CMapStringToString
{
public:
	BOOL HasValue(const CString& sValue) const;
	void AddValue(const CString& sValue);
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
	BOOL bDone, bGoodAsDone;
	BOOL bParent, bFlag;
	int nPercent;

	CString sTitle, sPath;
	double dTimeEst, dTimeSpent, dCost;
	TDC_UNITS nTimeEstUnits, nTimeSpentUnits;
	COleDateTime dtDone, dtDue, dtStart, dtLastMod, dtCreate;
	CString sExternalID, sRecurrence, sCreatedBy, sFileRef;

	void SetAttributeValue(LPCTSTR szAttrib, LPCTSTR szValue);
	void SetAttributeValue(LPCTSTR szAttrib, int nValue);
	CString GetAttributeValue(LPCTSTR szAttrib) const;
	CString GetAttributeValue(IUI_ATTRIBUTE nAttrib) const;
	void SetColor(COLORREF cr);
	COLORREF GetTextColor(BOOL bSelected, BOOL bColorIsBkgnd) const;
	COLORREF GetFillColor(BOOL bColorIsBkgnd) const;
	COLORREF GetBorderColor(BOOL bColorIsBkgnd) const;
	BOOL HasColor() const;
	BOOL IsDone(BOOL bIncludeGoodAs) const;
	int GetPriority() const;
	
	static CString GetAttribID(IUI_ATTRIBUTE nAttrib);

protected:
	CMapStringToString mapAttribValues;
	COLORREF color;
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
	int BuildTempItemMaps(LPCTSTR szAttribID, CKanbanItemArrayMap& map) const;
	KANBANITEM* GetItem(DWORD dwTaskID) const;
	KANBANITEM* NewItem(DWORD dwTaskID, const CString& sTitle);
	
#ifdef _DEBUG
	void TraceSummary(LPCTSTR szAttribID) const;
#endif

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

	const CKanbanItemMap& data;
	IUI_ATTRIBUTE nBy;
	CString sAttribID;
	BOOL bAscending;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_KANBANSTRUCT_H__C83C53D4_887E_4D5C_A8A7_85C8FDB19307__INCLUDED_)
