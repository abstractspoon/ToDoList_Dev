#if !defined(AFX_TDCSTRUCT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCSTRUCT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tdlstruct.h : header file
//

#include "taskfile.h"
#include "TDCImportExportMgr.h"
#include "tdccustomattribdata.h"
#include "tdccustomattributedef.h"

#include "..\shared\enstring.h"

//////////////////////////////////////////////////////////////////////

struct TDCDROPIMPORT
{
	TDCDROPIMPORT(DWORD dwID, const CStringArray& sDropFiles);
	TDCDROPIMPORT(DWORD dwID, const CString& sDropText);

	DWORD dwTaskID;
	CStringArray aFiles;
	CString sText;
};

//////////////////////////////////////////////////////////////////////

struct TDCDATETIMEWIDTHS
{
	TDCDATETIMEWIDTHS();

	void SetIsoFormat(BOOL bIso);
	void ResetWidths();
	void Initialise(CDC* pDC);

	int CalcMaxColumWidth(BOOL bIncTime, BOOL bIncDow) const;

	int nMaxDateWidth;
	int nMinDateWidth;		// No 'year'
	int nMaxTimeWidth;		// No 'seconds'
	int nMaxDowNameWidth;	// Short format
	int nSepWidth;

protected:
	BOOL bIsoFormat;
};

//////////////////////////////////////////////////////////////////////

struct TDCINFOTIPITEM
{
	TDCINFOTIPITEM(); // for CArray
	TDCINFOTIPITEM(TDC_ATTRIBUTE nAttribID, CString sLab, const CString& sVal);
	TDCINFOTIPITEM(TDC_ATTRIBUTE nAttribID, UINT nLabelStrID, const CString& sVal);

	TDC_ATTRIBUTE nAttributeID;
	CString sLabel;
	CString sValue;
	int nLabelWidth;
};

typedef CArray<TDCINFOTIPITEM, TDCINFOTIPITEM&> CTDCInfoTipItemArray;

/////////////////////////////////////////////////////////////////////////////////////////////

class CTDCColorMap : protected CMap<CString, LPCTSTR, COLORREF, COLORREF&>
{
public:
	void AddColor(LPCTSTR szID, COLORREF color);
	void Copy(const CTDCColorMap& other);
	void RemoveAll();

	BOOL GetColor(LPCTSTR szID, COLORREF& color) const;
	BOOL MatchAll(const CTDCColorMap& other) const;
};

/////////////////////////////////////////////////////////////////////////////////////////////

class CTDCStyleMap : public CMap<TDC_STYLE, TDC_STYLE, BOOL, BOOL&>
{
public:
	CTDCStyleMap();
	CTDCStyleMap(TDC_STYLE nStyle, BOOL bEnabled);
	CTDCStyleMap(const CTDCStyleMap& mapStyles);
	
	BOOL& operator[](TDC_STYLE nStyle);

	BOOL IsStyleEnabled(TDC_STYLE nStyle) const;
	BOOL HasStyle(TDC_STYLE nStyle) const;
	TDC_STYLE GetNext(POSITION& pos) const;
	
protected:
	BOOL IsValidStyle(TDC_STYLE nStyle) const;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCITEMCOLORS
{
	TDCITEMCOLORS();
	
	DWORD dwItem;		// in
	COLORREF crBack;	// out
	COLORREF crText;	// out
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCNOTIFYMOD
{
	TDCNOTIFYMOD(TDC_ATTRIBUTE nAttribID);
	TDCNOTIFYMOD(TDC_ATTRIBUTE nAttribID, const CDWordArray& taskIDs);
	TDCNOTIFYMOD(const CTDCAttributeMap& attribIDs, const CDWordArray& taskIDs);
	
	CTDCAttributeMap mapAttrib;
	CDWordArray aTaskIDs;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCFINDWND
{
	TDCFINDWND(HWND ignore = NULL, BOOL incClosing = FALSE, DWORD procID = 0);

	HWND hWndIgnore;
	DWORD dwProcessID;
	BOOL bIncClosing;
	CArray<HWND, HWND> aResults;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCAUTOLISTDATA
{
	TDCAUTOLISTDATA();
	TDCAUTOLISTDATA(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID);

	int Copy(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID);
	int AppendUnique(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID);
	int RemoveItems(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID);
	void RemoveAll(TDC_ATTRIBUTE nAttribID);

	int GetSize() const;
	CString Format(TDC_ATTRIBUTE nAttribID, TCHAR cSep = 0) const;
	BOOL Matches(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID) const;

	CStringArray aCategory;
	CStringArray aStatus;
	CStringArray aAllocTo;
	CStringArray aAllocBy;
	CStringArray aTags;
	CStringArray aVersion;

protected:
	BOOL WantAttribute(TDC_ATTRIBUTE nAttribID, TDC_ATTRIBUTE nWantedAttribID) const;

	int Copy(const TDCAUTOLISTDATA& from, TDCAUTOLISTDATA& to, BOOL bAppend, TDC_ATTRIBUTE nAttribID);
	int CopyItems(const CStringArray& aFrom, CStringArray& aTo, BOOL bAppend);
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCEXPORTTASKLIST
{
	TDCEXPORTTASKLIST(HWND hwnd, const CString& sTDC, int nExp);

	BOOL IsValid() const;

	CString sTDCPath;
	CString sExportPath;
	CString sStylesheet;
	CString sSaveIntermediatePath;

	CTaskFile tasks;
	int nExporter;
	HWND hWndNotify;
	CTDCImportExportMgr* pImpExpMgr;
	TDC_THREADEDEXPORTPURPOSE nPurpose;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCGETTASKS
{
	TDCGETTASKS(TDC_GETTASKS filter = TDCGT_ALL, DWORD flags = 0);
	TDCGETTASKS(const TDCGETTASKS& filter);

	TDCGETTASKS& operator=(const TDCGETTASKS& filter);

	BOOL WantAttribute(TDC_ATTRIBUTE nAttribID) const;
	BOOL IsSet(BOOL bIncAttrib) const;
	BOOL HasFlag(DWORD dwFlag) const;

	TDC_GETTASKS nFilter;
	COleDateTime dateDueBy;
	DWORD dwFlags;
	CString sAllocTo;
	CTDCAttributeMap mapAttribs;

protected:
	void InitDueByDate();
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCSELECTIONCACHE
{
	TDCSELECTIONCACHE();

	BOOL SelectionMatches(const TDCSELECTIONCACHE& cache, BOOL bAndFocus = FALSE) const;
	BOOL IsEmpty() const;

	void Clear();

	CDWordArray aSelTaskIDs;
	DWORD dwFocusedTaskID;
	CDWordArray aBreadcrumbs;
	DWORD dwFirstVisibleTaskID;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCONTROL
{
	LPCTSTR szClass;
	UINT nIDCaption;
	DWORD dwStyle;
	DWORD dwExStyle; 
	int nX;
	int nY;
	int nCx;
	int nCy;
	UINT nID;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCOLUMN
{
	TDC_COLUMN nColumnID;
	UINT nIDName;
	UINT nIDLongName;
	UINT nTextAlignment;
	BOOL bSortAscending;
	TDCC_IMAGE iImage, iAlternateImage;

	UINT GetColumnHeaderAlignment() const;
};

class CTDCColumnArray : public CArray<TDCCOLUMN, TDCCOLUMN&> {};
typedef CMap<TDC_COLUMN, TDC_COLUMN, const TDCCOLUMN*, const TDCCOLUMN*&> CTDCColumnMap;

/////////////////////////////////////////////////////////////////////////////////////////////

struct CTRLITEM
{
	CTRLITEM();
	CTRLITEM(UINT ctrlID, UINT labelID, TDC_ATTRIBUTE nAttribID);

	BOOL operator==(const CTRLITEM& other) const;
	BOOL operator!=(const CTRLITEM& other) const;

	CWnd* GetCtrl(const CWnd* pParent) const;
	CWnd* GetLabel(const CWnd* pParent) const;

	void DeleteCtrls(const CWnd* pParent);

	UINT nCtrlID;
	UINT nLabelID;
	TDC_ATTRIBUTE nAttributeID;

protected:
	void DeleteCtrl(const CWnd* pParent, UINT& nID);
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct CUSTOMATTRIBCTRLITEM : public CTRLITEM
{
	friend class CTDCCustomControlArray;

	CUSTOMATTRIBCTRLITEM();

	BOOL operator==(const CUSTOMATTRIBCTRLITEM& other) const;
	BOOL operator!=(const CUSTOMATTRIBCTRLITEM& other) const;

	void ShowBuddy(const CWnd* pParent, BOOL bShow = TRUE);

	BOOL IsShowingBuddy() const;
	BOOL HasBuddy() const;
	BOOL GetBuddy(CTRLITEM& ctrl) const;

	CWnd* GetBuddy(const CWnd* pParent) const;
	CWnd* GetBuddyLabel(const CWnd* pParent) const;

	void DeleteCtrls(const CWnd* pParent);

	CString sAttribID;
	UINT nBuddyCtrlID;
	UINT nBuddyLabelID;
	CString sPrompt, sBuddyPrompt;

protected:
	CRuntimeClass* pBuddyClass;

	void DeleteBuddy(const CWnd* pParent);
};

class CTDCControlArray : public CArray<CTRLITEM, CTRLITEM&> {};

/////////////////////////////////////////////////////////////////////////////////////////////

class CTDCCustomControlArray : public CArray<CUSTOMATTRIBCTRLITEM, CUSTOMATTRIBCTRLITEM&> 
{
public:
	void DeleteCtrls(const CWnd* pParent);

	int Find(UINT nCtrlID, BOOL bIncludeLabels = FALSE) const;
	int Find(TDC_ATTRIBUTE nAttribID) const;
	int Find(const CString& sAttribID) const;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCOPERATOR
{
	FIND_OPERATOR op;
	UINT nOpResID;
};

// ------------------------------------------------------------------------

struct SEARCHPARAM
{
	friend struct SEARCHPARAMS;

	SEARCHPARAM(TDC_ATTRIBUTE nAttribID = TDCA_NONE, FIND_OPERATOR nOp = FOP_NONE);
	SEARCHPARAM(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, CString sVal, BOOL and = TRUE, FIND_ATTRIBTYPE nType = FT_NONE);
	SEARCHPARAM(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, double dVal, BOOL and = TRUE);
	SEARCHPARAM(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, int nVal, BOOL and = TRUE);

	BOOL operator==(const SEARCHPARAM& rule) const;
	BOOL operator!=(const SEARCHPARAM& rule) const;

	BOOL Set(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, CString sVal, BOOL and = TRUE, FIND_ATTRIBTYPE nType = FT_NONE);
	BOOL Set(TDC_ATTRIBUTE nAttribID, const CString& sID, FIND_ATTRIBTYPE nType, FIND_OPERATOR nOp, CString sVal, BOOL and = TRUE);
	BOOL SetAttribute(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType = FT_NONE);
	BOOL SetCustomAttribute(TDC_ATTRIBUTE nAttribID, const CString& sID, FIND_ATTRIBTYPE nType);

	void SetAnd(BOOL and = TRUE);
	void SetTimeUnits(TDC_UNITS nUnits);
	void SetMatchWholeWord(BOOL bMatchWhole);
	void SetFlags(DWORD flags);
	void SetRelativeDate(BOOL bRelative);

	void SetValue(const CString& sVal);
	void SetValue(double dVal);
	void SetValue(int nVal);
	void SetValue(const COleDateTime& dtVal);
	void ClearValue();

	CString GetCustomAttributeID() const;
	BOOL SetOperator(FIND_OPERATOR nOp);

	FIND_ATTRIBTYPE GetAttribType() const;
	TDC_ATTRIBUTE GetAttribute() const;
	FIND_OPERATOR GetOperator() const;
	BOOL GetAnd() const;
	BOOL GetOr() const;
	DWORD GetFlags() const;
	TDC_UNITS GetTimeUnits() const;
	BOOL GetMatchWholeWord() const;
	BOOL GetRelativeDate() const;

	BOOL IsCustomAttribute() const;
	BOOL IsRelativeDate(BOOL bMustHaveValue = TRUE) const;
	BOOL IsNowRelativeDate() const;

	BOOL HasValidOperator() const;
	BOOL HasString() const;
	BOOL HasIcon() const;

	void ValidateOperator();
	BOOL OperatorIs(FIND_OPERATOR nOp) const;
	BOOL AttributeIs(TDC_ATTRIBUTE nAttribID) const;
	BOOL TypeIs(FIND_ATTRIBTYPE nType) const;

	CString ValueAsString() const;
	double ValueAsDouble() const;
	int ValueAsInteger() const;
	COleDateTime ValueAsDate() const;

	static FIND_ATTRIBTYPE GetAttribType(TDC_ATTRIBUTE nAttribID, BOOL bRelativeDate);
	static BOOL IsValidOperator(FIND_ATTRIBTYPE nType, FIND_OPERATOR nOp);

protected:
	TDC_ATTRIBUTE nAttributeID;
	CString sCustAttribID;
	FIND_OPERATOR nOperator;
	CString sValue;
	int nValue;
	double dValue;
	BOOL bAnd;

	union
	{
		TDC_UNITS nTimeUnits;
		BOOL bMatchWholeWord;
		BOOL bRelativeDate;
		DWORD dwFlags; // Backwards compatibility
	};
	
	mutable FIND_ATTRIBTYPE nAttribType;
};

// ------------------------------------------------------------------------

class CSearchParamArray : public CArray<SEARCHPARAM, SEARCHPARAM&>
{
public:
	BOOL IsValid() const; // Has some actual rules 
	BOOL IsBalanced() const; // Begin/End groups
	BOOL IsBalanced(int& nNumGroupStarts, int& nNumGroupEnds) const;

	BOOL IsStartOfGroup(int nRule) const;
	BOOL IsLastRule(int nRule) const;
	BOOL IsLastRuleInGroup(int nRule) const;
	BOOL RuleSupportsAndOr(int nRule) const;
	BOOL GetRuleDepth(int nRule) const;

protected:
	void CountGroupings(int& nNumStarts, int& nNumEnds) const;
};

// ------------------------------------------------------------------------

struct SEARCHPARAMS
{
	SEARCHPARAMS();
	SEARCHPARAMS(const SEARCHPARAMS& params);

	SEARCHPARAMS& operator=(const SEARCHPARAMS& params);

	BOOL operator==(const SEARCHPARAMS& params) const;
	BOOL operator!=(const SEARCHPARAMS& params) const;

	void Clear();

	BOOL HasAttribute(TDC_ATTRIBUTE nAttribID) const;
	BOOL HasRule(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp) const;
	BOOL HasMultipleAttributes() const;

	BOOL GetRuleCount() const;

	CSearchParamArray aRules;
	CTDCCustomAttribDefinitionArray aAttribDefs;

	BOOL bIgnoreDone;
	BOOL bIgnoreOverDue;
	BOOL bWantAllSubtasks;
	BOOL bIgnoreFilteredOut;
	BOOL bCaseSensitive;

protected:
	mutable CTDCAttributeMap mapAttrib;

protected:
	void InitAttributeMap() const;
};

// ------------------------------------------------------------------------

struct SEARCHRESULT
{
	SEARCHRESULT();
	SEARCHRESULT(const SEARCHRESULT& res);

	SEARCHRESULT& operator=(const SEARCHRESULT& res);

	BOOL HasFlag(DWORD dwFlag) const;
	BOOL GetWhatMatched(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aAttribDefs, CString& sWhatMatched) const;
	
	DWORD dwTaskID;
	DWORD dwFlags;
	CMap<TDC_ATTRIBUTE, TDC_ATTRIBUTE, CString, CString&> mapMatched;
};

typedef CArray<SEARCHRESULT, SEARCHRESULT&> CResultArray;

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCFILTER
{
	TDCFILTER();
	TDCFILTER(const TDCFILTER& filter);

	TDCFILTER& operator=(const TDCFILTER& filter);

	BOOL operator==(const TDCFILTER& filter) const;
	BOOL operator!=(const TDCFILTER& filter) const;
	
	BOOL IsSet() const;
	BOOL IsAdvanced() const;
	BOOL WantHideCompletedTasks() const;

	DWORD GetFlags() const;

	BOOL HasAttribute(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const;
	BOOL HasNowFilter(TDC_ATTRIBUTE& nAttrib) const;
	BOOL HasFlag(DWORD dwFlag) const;

	void SetFlag(DWORD dwFlag, BOOL bOn = TRUE);
	void Reset(FILTER_SHOW nInit = FS_ALL);

	BOOL Matches(const TDCFILTER& filter) const;
	CString GetTitleFilterLabel() const;

	FILTER_SHOW nShow;
	FILTER_DATE nStartBy, nDueBy;
	CString sTitle;
	FILTER_TITLE nTitleOption;
	int nPriority, nRisk;
	CStringArray aCategories, aAllocTo, aTags;
	CStringArray aStatus, aAllocBy, aVersions;
	DWORD dwFlags;
	COleDateTime dtUserStart, dtUserDue;
	int nStartNextNDays, nDueNextNDays;
	TDC_REGULARITY nRecurrence;

	CTDCCustomAttributeDataMap mapCustomAttrib;

protected:
	static BOOL FiltersMatch(const TDCFILTER& filter1, const TDCFILTER& filter2);

};

struct TDCADVANCEDFILTER
{
	TDCADVANCEDFILTER(LPCTSTR szName = NULL, DWORD flags = 0);
	TDCADVANCEDFILTER(const TDCADVANCEDFILTER& filter);
	
	TDCADVANCEDFILTER& operator=(const TDCADVANCEDFILTER& filter);
	
	BOOL operator==(const TDCADVANCEDFILTER& filter) const;
	BOOL operator!=(const TDCADVANCEDFILTER& filter) const;
	
	BOOL IsSet(DWORD dwIgnore = 0) const;
	BOOL HasNowFilter(TDC_ATTRIBUTE& nAttrib) const;

	DWORD GetFlags() const;
	BOOL HasFlag(DWORD dwFlag) const;

	void Reset();

	BOOL Matches(const TDCADVANCEDFILTER& filter, DWORD dwIgnoreFlags = 0) const;
	BOOL Matches(LPCTSTR szCustom, DWORD dwCustomFlags, DWORD dwIgnoreFlags = 0) const;
	
	SEARCHPARAMS params;
	CString sName;
	DWORD dwFlags;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCATTRIBUTE
{
	TDC_ATTRIBUTE nAttributeID;
	TDC_ATTRIBUTEGROUP nGroup;
	UINT nLabelResID;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCOLEDITVISIBILITY
{
	TDCCOLEDITVISIBILITY();
	TDCCOLEDITVISIBILITY(const TDCCOLEDITVISIBILITY& vis);
	
	TDCCOLEDITVISIBILITY& operator=(const TDCCOLEDITVISIBILITY& vis);
	
	BOOL operator==(const TDCCOLEDITVISIBILITY& vis) const;
	BOOL operator!=(const TDCCOLEDITVISIBILITY& vis) const;	

	void Clear();

	BOOL HasDifferences(const TDCCOLEDITVISIBILITY& vis, 
						BOOL& bColumnChange, BOOL& bEditChange) const;

	virtual void SetShowFields(TDL_SHOWATTRIB nShow);

	void ShowColorEditIfAsColumns(BOOL bShow);
	void SetVisibleColumns(const CTDCColumnIDMap& mapColumns);
	void SetAllColumnsVisible(BOOL bVisible = TRUE);
	BOOL SetAllFieldsVisible(BOOL bVisible = TRUE);
	BOOL SetVisibleEditFields(const CTDCAttributeMap& mapAttrib);
	BOOL SetEditFieldVisible(TDC_ATTRIBUTE nAttribID, BOOL bVisible = TRUE);
	void SetColumnVisible(TDC_COLUMN nColID, BOOL bVisible = TRUE);

	TDL_SHOWATTRIB GetShowFields() const;
	const CTDCColumnIDMap& GetVisibleColumns() const;
	const CTDCAttributeMap& GetVisibleEditFields() const;

	BOOL IsEditFieldVisible(TDC_ATTRIBUTE nAttribID) const;
	BOOL IsColumnVisible(TDC_COLUMN nColID) const;

	void Save(IPreferences* pPrefs, LPCTSTR szKey) const;
	BOOL Load(const IPreferences* pPrefs, LPCTSTR szKey);

	static BOOL IsSupportedEdit(TDC_ATTRIBUTE nAttribID);
	static BOOL IsSupportedFilter(TDC_ATTRIBUTE nAttribID);
	static BOOL IsSupportedColumn(TDC_COLUMN nColumn);

protected:
	CTDCColumnIDMap mapVisibleColumns;
	CTDCAttributeMap mapVisibleEdits;
	TDL_SHOWATTRIB nShowFields;
	BOOL bShowColorEditIfAsColumns;

protected:
	int UpdateEditVisibility();
	
	static int GetAllEditFields(CTDCAttributeMap& mapAttrib);
};

struct TDCCOLEDITFILTERVISIBILITY : public TDCCOLEDITVISIBILITY
{
	TDCCOLEDITFILTERVISIBILITY();
	TDCCOLEDITFILTERVISIBILITY(const TDCCOLEDITFILTERVISIBILITY& vis);

	TDCCOLEDITFILTERVISIBILITY& operator=(const TDCCOLEDITFILTERVISIBILITY& vis);

	BOOL operator==(const TDCCOLEDITFILTERVISIBILITY& vis) const;
	BOOL operator!=(const TDCCOLEDITFILTERVISIBILITY& vis) const;

	void SetShowFields(TDL_SHOWATTRIB nShow);
	void SetAllColumnsVisible(BOOL bVisible = TRUE);
	BOOL SetAllFieldsVisible(BOOL bVisible = TRUE);
	BOOL SetVisibleFilterFields(const CTDCAttributeMap& mapAttrib);
	BOOL SetFilterFieldVisible(TDC_ATTRIBUTE nAttribID, BOOL bVisible = TRUE);
	void Clear();

	const CTDCAttributeMap& GetVisibleFilterFields() const;
	BOOL IsFilterFieldVisible(TDC_ATTRIBUTE nAttribID) const;
	BOOL HasDifferences(const TDCCOLEDITFILTERVISIBILITY& vis,
						BOOL& bColumnChange, BOOL& bEditChange, BOOL& bFilterChange) const;

	void Save(IPreferences* pPrefs, LPCTSTR szKey) const;
	BOOL Load(const IPreferences* pPrefs, LPCTSTR szKey);

	static BOOL IsSupportedFilter(TDC_ATTRIBUTE nAttribID);

protected:
	CTDCAttributeMap mapVisibleFilters;

protected:
	int UpdateFilterVisibility();

	static int GetAllFilterFields(CTDCAttributeMap& mapAttrib);
};

/////////////////////////////////////////////////////////////////////////////

struct TDCATTRIBUTEMAPPING
{
	TDCATTRIBUTEMAPPING();
	TDCATTRIBUTEMAPPING(const CString& sName, TDC_ATTRIBUTE nAttribID, DWORD dwData = 0);
	TDCATTRIBUTEMAPPING(UINT nNameID, TDC_ATTRIBUTE nAttribID, DWORD dwData = 0);

	TDC_ATTRIBUTE nAttributeID;
	CString sColumnName;
	DWORD dwItemData;
};

/////////////////////////////////////////////////////////////////////////////

class CTDCAttributeMapping : public CArray<TDCATTRIBUTEMAPPING, TDCATTRIBUTEMAPPING&>
{
public:
	int Find(const CString& sCol) const;
	int Find(TDC_ATTRIBUTE nAttribID) const;
	int Find(DWORD dwData) const;
	int FindMappedAttribute(TDC_ATTRIBUTE nAttribID) const;

	BOOL CTDCAttributeMapping::IsAttributeMapped(TDC_ATTRIBUTE nAttribID) const;

	void Trace() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCSTRUCT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
