#if !defined(AFX_TDCSTRUCT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCSTRUCT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tdlutil.h : header file
//

#include "tdcswitch.h"
#include "taskfile.h"
#include "tdcmapping.h"
#include "tdcenumcontainers.h"

#include "..\shared\TreeSelectionHelper.h"
#include "..\shared\TreeCtrlHelper.h"
#include "..\shared\misc.h"
#include "..\shared\datehelper.h"
#include "..\shared\encommandlineinfo.h"
#include "..\shared\enstring.h"
#include "..\shared\preferences.h"

/////////////////////////////////////////////////////////////////////////////////////////////

typedef CMap<TDC_STYLE, TDC_STYLE, BOOL, BOOL&> CTDCStylesMap;

/////////////////////////////////////////////////////////////////////////////////////////////

class CToDoCtrl;
class CFilteredToDoCtrl;
class CTDCImportExportMgr;

/////////////////////////////////////////////////////////////////////////////////////////////

class CTDCColorMap : protected CMap<CString, LPCTSTR, COLORREF, COLORREF&>
{
public:
	CTDCColorMap() {}
	virtual ~CTDCColorMap() {}

	void AddColor(LPCTSTR szID, COLORREF color)
	{
		SetAt(Misc::ToUpper(szID), color);
	}

	BOOL GetColor(LPCTSTR szID, COLORREF& color) const
	{
		return Lookup(Misc::ToUpper(szID), color);
	}

	void RemoveAll()
	{
		CMap<CString, LPCTSTR, COLORREF, COLORREF&>::RemoveAll();
	}

	void Copy(const CTDCColorMap& other)
	{
		Misc::CopyStrT<COLORREF>(other, *this);
	}

	BOOL MatchAll(const CTDCColorMap& other) const
	{
		return Misc::MatchAllStrT<COLORREF>(other, *this);
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCITEMCOLORS
{
	TDCITEMCOLORS() 
		: 
		dwItem(0), 
		crBack(CLR_NONE), 
		crText(CLR_NONE)
	{
		
	}
	
	DWORD dwItem;		// in
	COLORREF crBack;	// out
	COLORREF crText;	// out
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCFINDWND
{
	TDCFINDWND(HWND ignore = NULL, BOOL incClosing = FALSE, DWORD procID = 0) 
		: hWndIgnore(ignore), bIncClosing(incClosing), dwProcessID(procID)
	{
	}

	HWND hWndIgnore;
	DWORD dwProcessID;
	BOOL bIncClosing;
	CArray<HWND, HWND> aResults;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCAUTOLISTDATA
{
	TDCAUTOLISTDATA(){}

	void Copy(const TDCAUTOLISTDATA& other)
	{
		Copy(other, *this, FALSE);
	}

	void AppendUnique(const TDCAUTOLISTDATA& other)
	{
		Copy(other, *this, TRUE);
	}

	void RemoveItems(const TDCAUTOLISTDATA& other)
	{
		Misc::RemoveItems(other.aCategory, aCategory);
		Misc::RemoveItems(other.aStatus, aStatus);
		Misc::RemoveItems(other.aAllocTo, aAllocTo);
		Misc::RemoveItems(other.aAllocBy, aAllocBy);
		Misc::RemoveItems(other.aTags, aTags);
		Misc::RemoveItems(other.aVersion, aVersion);
	}

	int GetSize() const
	{
		return (aCategory.GetSize() + 
				aStatus.GetSize() + 
				aAllocTo.GetSize() + 
				aAllocBy.GetSize() + 
				aTags.GetSize() + 
				aVersion.GetSize());
	}

	void RemoveAll()
	{
		aCategory.RemoveAll();
		aStatus.RemoveAll();
		aAllocTo.RemoveAll();
		aAllocBy.RemoveAll();
		aTags.RemoveAll();
		aVersion.RemoveAll();
	}

	CStringArray aCategory, aStatus, aAllocTo, aAllocBy, aTags, aVersion;

protected:
	void Copy(const TDCAUTOLISTDATA& from, TDCAUTOLISTDATA& to, BOOL bAppend)
	{
		CopyItems(from.aCategory, to.aCategory, bAppend);
		CopyItems(from.aStatus, to.aStatus, bAppend);
		CopyItems(from.aAllocTo, to.aAllocTo, bAppend);
		CopyItems(from.aAllocBy, to.aAllocBy, bAppend);
		CopyItems(from.aTags, to.aTags, bAppend);
		CopyItems(from.aVersion, to.aVersion, bAppend);
	}

	void CopyItems(const CStringArray& aFrom, CStringArray& aTo, BOOL bAppend)
	{
		if (bAppend)
			Misc::AddUniqueItems(aFrom, aTo);
		else
			aTo.Copy(aFrom);
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCDUETASKNOTIFY
{
	TDCDUETASKNOTIFY(HWND hwnd, const CString sTDC, BOOL html) 
		: 
		hWndNotify(hwnd),
		sTDCPath(sTDC), 
		bHtml(html),
		pImpExpMgr(NULL)
	{
	}

	BOOL IsValid() const
	{
		if (!::IsWindow(hWndNotify))
			return FALSE;

		if (!tasks.GetTaskCount())
			return FALSE;

		if (!pImpExpMgr)
			return FALSE;

		if (sExportPath.IsEmpty())
			return FALSE;

		if (sTDCPath.IsEmpty())
			return FALSE;

		return TRUE;
	}

	CString sTDCPath;
	CString sExportPath;
	CString sStylesheet;

	CTaskFile tasks;
	BOOL bHtml;
	HWND hWndNotify;
	CTDCImportExportMgr* pImpExpMgr;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCSTATUSBARINFO
{
	TDCSTATUSBARINFO() 
		: 
		nSelCount(0), 
		dwSelTaskID(0), 
		nTimeEstUnits(TDCU_NULL), 
		dTimeEst(0.0), 
		nTimeSpentUnits(TDCU_NULL), 
		dTimeSpent(0.0), 
		dCost(0.0)
	{
	}

	BOOL operator==(const TDCSTATUSBARINFO& sbi) const
	{
		return (nSelCount == sbi.nSelCount &&
				dwSelTaskID == sbi.dwSelTaskID &&
				nTimeEstUnits == sbi.nTimeEstUnits &&
				dTimeEst == sbi.dTimeEst &&
				nTimeSpentUnits == sbi.nTimeSpentUnits &&
				dTimeSpent == sbi.dTimeSpent &&
				dCost == sbi.dCost);
	}

	int nSelCount;
	DWORD dwSelTaskID;
	TDC_UNITS nTimeEstUnits;
	double dTimeEst;
	TDC_UNITS nTimeSpentUnits;
	double dTimeSpent;
	double dCost;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCGETTASKS
{
	TDCGETTASKS(TDC_GETTASKS filter = TDCGT_ALL, DWORD flags = 0) 
		: nFilter(filter), dwFlags(flags) 
	{ 
		InitDueByDate(); 
	}

	TDCGETTASKS(const TDCGETTASKS& filter)
	{ 
		*this = filter;
	}

	TDCGETTASKS& operator=(const TDCGETTASKS& filter)
	{
		nFilter = filter.nFilter;
		dateDueBy = filter.dateDueBy;
		dwFlags = filter.dwFlags;
		sAllocTo = filter.sAllocTo;

		mapAttribs.CopyAttributes(filter.mapAttribs);

		return *this;
	}

	BOOL WantAttribute(TDC_ATTRIBUTE nAttrib) const
	{
		// always want custom attributes
		if (nAttrib == TDCA_CUSTOMATTRIB)
			return TRUE;

		// if TDCGTF_CUSTOMCOLUMNS is set then
		// 'no' attributes really means 'no' attributes
		// else it means 'all attributes' 
		if (mapAttribs.GetCount() == 0)
		{
			if (HasFlag(TDCGTF_USERCOLUMNS))
				return FALSE;
			else
				return TRUE;
		}
		else if (mapAttribs.HasAttribute(TDCA_ALL))
		{
			return TRUE;
		}

		return mapAttribs.HasAttribute(nAttrib);
	}

	BOOL IsSet(BOOL bIncAttrib) const
	{
		if ((nFilter != TDCGT_ALL) || 
			CDateHelper::IsDateSet(dateDueBy) ||
			dwFlags ||
			!sAllocTo.IsEmpty())
		{
			return TRUE;
		}
		else if (bIncAttrib)
		{
			return mapAttribs.GetCount();
		}
		
		// else
		return FALSE;
	}

	BOOL HasFlag(DWORD dwFlag) const { return Misc::HasFlag(dwFlags, dwFlag); }

	TDC_GETTASKS nFilter;
	COleDateTime dateDueBy;
	DWORD dwFlags;
	CString sAllocTo;
	CTDCAttributeMap mapAttribs;

protected:
	void InitDueByDate()
	{
		// initialize filter:dateDueBy as required
		switch (nFilter)
		{
		case TDCGT_DUE:
			dateDueBy = CDateHelper::GetDate(DHD_TODAY);
			break; // done
			
		case TDCGT_DUETOMORROW:
			dateDueBy = CDateHelper::GetDate(DHD_TOMORROW);
			break;
			
		case TDCGT_DUETHISWEEK:
			dateDueBy = CDateHelper::GetDate(DHD_ENDTHISWEEK);
			break;
			
		case TDCGT_DUENEXTWEEK:
			dateDueBy = CDateHelper::GetDate(DHD_ENDNEXTWEEK);
			break;
			
		case TDCGT_DUETHISMONTH:
			dateDueBy = CDateHelper::GetDate(DHD_ENDTHISMONTH);
			break;
			
		case TDCGT_DUENEXTMONTH:
			dateDueBy = CDateHelper::GetDate(DHD_ENDNEXTMONTH);
			break;

		default:
			CDateHelper::ClearDate(dateDueBy);
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCSELECTIONCACHE
{
	TDCSELECTIONCACHE() : dwFocusedTaskID(0), dwFirstVisibleTaskID(0) {}

	BOOL SelectionMatches(const TDCSELECTIONCACHE& cache, BOOL bAndFocus = FALSE)
	{
		return (Misc::MatchAllT(aSelTaskIDs, cache.aSelTaskIDs) &&
			    (!bAndFocus || (dwFocusedTaskID == cache.dwFocusedTaskID)));
	}

	void Clear()
	{
		aSelTaskIDs.RemoveAll();
		aBreadcrumbs.RemoveAll();

		dwFocusedTaskID = dwFirstVisibleTaskID = 0;
	}

	BOOL IsEmpty() const
	{
		if (aSelTaskIDs.GetSize())
		{
			ASSERT(dwFocusedTaskID);
			return FALSE;
		}

		// else
		return TRUE;
	}

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
	TDC_COLUMN nColID;
	UINT nIDName;
	UINT nIDLongName;
	UINT nTextAlignment;
	BOOL bSortAscending;
	int iImage;

	UINT GetColumnHeaderAlignment() const
	{
		switch (nTextAlignment)
		{
		case DT_CENTER: return HDF_CENTER;
		case DT_RIGHT:	return HDF_RIGHT;
		case DT_LEFT:	return HDF_LEFT;
		}

		// all else
		ASSERT(0);
		return HDF_LEFT;
	}
};

class CTDCColumnArray : public CArray<TDCCOLUMN, TDCCOLUMN&> {};

/////////////////////////////////////////////////////////////////////////////////////////////

struct CTRLITEM
{
	UINT nCtrlID;
	UINT nLabelID;
	TDC_ATTRIBUTE nAttrib;
};

struct CUSTOMATTRIBCTRLITEM : public CTRLITEM
{
	CUSTOMATTRIBCTRLITEM()
	{
		nCtrlID = nLabelID = nBuddyCtrlID = nBuddyLabelID = 0;
		nAttrib = TDCA_NONE;
	}

	BOOL HasBuddy() const
	{
		return (nBuddyCtrlID && nBuddyLabelID);
	}

	BOOL GetBuddy(CTRLITEM& ctrl) const
	{
		if (!HasBuddy())
			return FALSE;

		ctrl.nAttrib = nAttrib;
		ctrl.nCtrlID = nBuddyCtrlID;
		ctrl.nLabelID = nBuddyLabelID;

		return TRUE;
	}
	
	CString sAttribID;
	UINT nBuddyCtrlID;
	UINT nBuddyLabelID;
};

class CTDCControlArray : public CArray<CTRLITEM, CTRLITEM&> {};
class CTDCCustomControlArray : public CArray<CUSTOMATTRIBCTRLITEM, CUSTOMATTRIBCTRLITEM&> {};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCUSTOMATTRIBUTEDEFINITION
{
	friend class CTDCCustomAttribDefinitionArray;

	TDCCUSTOMATTRIBUTEDEFINITION() 
		: 
	dwAttribType(TDCCA_STRING), 
	nTextAlignment(DT_LEFT), 
	dwFeatures(0),
	nColID(TDCC_NONE),
	nAttribID(TDCA_NONE),
	bEnabled(TRUE)
	{

	}

	TDCCUSTOMATTRIBUTEDEFINITION(const TDCCUSTOMATTRIBUTEDEFINITION& otherDef)
	{
		*this = otherDef;
	}

	TDCCUSTOMATTRIBUTEDEFINITION& operator=(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
	{
		dwAttribType = attribDef.dwAttribType;
		sUniqueID = attribDef.sUniqueID;
		sColumnTitle = attribDef.sColumnTitle;
		sLabel = attribDef.sLabel;
		nTextAlignment = attribDef.nTextAlignment;
		dwFeatures = attribDef.dwFeatures;
		nColID = attribDef.nColID;
		nAttribID = attribDef.nAttribID;
		bEnabled = attribDef.bEnabled;

		if (attribDef.IsList())
		{
			aDefaultListData.Copy(attribDef.aDefaultListData);
			
			if (attribDef.IsAutoList())
				aAutoListData.Copy(attribDef.aAutoListData);
		}

		return *this;
	}

	BOOL operator==(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const
	{
		// NOTE: we ignore auto data which is temporary
		return ((dwAttribType == attribDef.dwAttribType) &&
				(sUniqueID.CompareNoCase(attribDef.sUniqueID) == 0) &&
				(sColumnTitle == attribDef.sColumnTitle) &&
				(sLabel == attribDef.sLabel) &&
				(nTextAlignment == attribDef.nTextAlignment) &&
				(dwFeatures == attribDef.dwFeatures) &&
				(nColID == attribDef.nColID) &&
				(nAttribID == attribDef.nAttribID) &&
				(bEnabled == attribDef.bEnabled) &&
				Misc::MatchAll(aDefaultListData, attribDef.aDefaultListData));
	}

	CString GetColumnTitle() const
	{
		return (sColumnTitle.IsEmpty() ? sLabel : sColumnTitle);
	}

	inline TDC_COLUMN GetColumnID() const { return nColID; }
	inline TDC_ATTRIBUTE GetAttributeID() const { return nAttribID; }
	inline DWORD GetAttributeType() const { return dwAttribType; }

	UINT GetColumnHeaderAlignment() const
	{
		switch (nTextAlignment)
		{
		case DT_CENTER: return HDF_CENTER;
		case DT_RIGHT:	return HDF_RIGHT;
		case DT_LEFT:	return HDF_LEFT;
		}
		
		// all else
		ASSERT(0);
		return HDF_LEFT;
	}

	void SetAttributeType(DWORD dwType)
	{
		SetTypes((dwType & TDCCA_DATAMASK), (dwType & TDCCA_LISTMASK));
	}

	void SetDataType(DWORD dwDataType)
	{
		SetTypes(dwDataType, GetListType());
	}

	void SetListType(DWORD dwListType)
	{
		SetTypes(GetDataType(), dwListType);
	}

	inline DWORD GetDataType() const { return (dwAttribType & TDCCA_DATAMASK); }
	inline DWORD GetListType() const { return (dwAttribType & TDCCA_LISTMASK); }
	inline BOOL HasFeature(DWORD dwFeature) const { return (dwFeatures & dwFeature); }

	inline BOOL IsList() const { return (GetListType() != TDCCA_NOTALIST); }
	inline BOOL IsAutoList() const { return ((GetListType() == TDCCA_AUTOLIST) || (GetListType() == TDCCA_AUTOMULTILIST)); }

	int GetUniqueListData(CStringArray& aData) const
	{
		aData.Copy(aAutoListData);
		Misc::AddUniqueItems(aDefaultListData, aData);

		return aData.GetSize();
	}

	CString EncodeListData() const
	{
		if (!IsList())
			return _T("");

		CString sListData = Misc::FormatArray(aDefaultListData, '\n');

		if (IsAutoList())
		{
			sListData += '\t';
			sListData += Misc::FormatArray(aAutoListData, '\n');
		}

		return sListData;
	}
	
	BOOL DecodeListData(const CString& sListData)
	{
		if (IsList() && !sListData.IsEmpty())
		{
			CString sDefData(sListData), sAutoData;

			Misc::Split(sDefData, sAutoData, '\t');
			Misc::Split(sDefData, aDefaultListData, '\n');

			if (IsAutoList())
				Misc::Split(sAutoData, aAutoListData, '\n');

			return TRUE;
		}

		// else
		return FALSE;
	}

	BOOL SupportsFeature(DWORD dwFeature) const
	{
		// sorting works on all data types
		if (dwFeature == TDCCAF_SORT)
			return TRUE;

		// calculations not supported by multi-list types
		DWORD dwListType = GetListType();

		if (dwListType == TDCCA_AUTOMULTILIST || dwListType == TDCCA_FIXEDMULTILIST)
			return FALSE;

		DWORD dwDataType = GetDataType();

		switch (dwDataType)
		{
		case TDCCA_DOUBLE:
		case TDCCA_INTEGER:
			return ((dwFeature == TDCCAF_ACCUMULATE) || 
					(dwFeature == TDCCAF_MAXIMIZE) || 
					(dwFeature == TDCCAF_MINIMIZE) ||
					(dwFeature == TDCCAF_HIDEZERO));
			
		case TDCCA_DATE:
			return ((dwFeature == TDCCAF_MAXIMIZE) || 
					(dwFeature == TDCCAF_MINIMIZE) ||
					(dwFeature == TDCCAF_SHOWTIME));
			
		case TDCCA_STRING:
		case TDCCA_BOOL:
		case TDCCA_ICON:
			break;

		default:
			ASSERT(0);
			break;
		}
		
		return FALSE;
	}

	BOOL SupportsCalculation() const
	{
		return SupportsFeature(TDCCAF_ACCUMULATE) ||
				SupportsFeature(TDCCAF_MAXIMIZE) ||
				SupportsFeature(TDCCAF_MINIMIZE);
	}

	CString GetNextListItem(const CString& sItem, BOOL bNext) const
	{
		DWORD dwListType = GetListType();

		ASSERT (dwListType != TDCCA_NOTALIST);

		switch (aDefaultListData.GetSize())
		{
		case 0:
			return sItem;

		default:
			{
				int nFind = Misc::Find(aDefaultListData, sItem);
				ASSERT((nFind != -1) || sItem.IsEmpty());

				if (bNext)
				{
					nFind++;

					if (nFind < aDefaultListData.GetSize())
						return aDefaultListData[nFind];
				}
				else // prev
				{
					if (nFind == -1)
						nFind = aDefaultListData.GetSize();

					nFind--;

					if (nFind >= 0)
						return aDefaultListData[nFind];
				}

				// all else
				return _T("");
			}
			break;
		}
	}

	CString GetImageName(const CString& sImage) const
	{
		int nTag = Misc::Find(aDefaultListData, sImage);

		CString sName, sDummy;

		if (nTag != -1)
		{
			CString sTag = aDefaultListData[nTag], sDummy;

			VERIFY(DecodeImageTag(sTag, sDummy, sName));
			//ASSERT(sImage == sDummy);
		}

		return sName;
	}

	static CString EncodeImageTag(const CString& sImage, const CString& sName) 
	{ 
		CString sTag;
		sTag.Format(_T("%s:%s"), sImage, sName);
		return sTag;
	}

	static BOOL DecodeImageTag(const CString& sTag, CString& sImage, CString& sName)
	{
		CStringArray aParts;
		int nNumParts = Misc::Split(sTag, aParts, ':');

		switch (nNumParts)
		{
		case 2:
			sName = aParts[1];
			// fall thru

		case 1:
			sImage = aParts[0];
			break;

		case 0:
			return FALSE;
		}

		return TRUE;
	}


	///////////////////////////////////////////////////////////////////////////////
	CString sUniqueID;
	CString sColumnTitle;
	CString sLabel;
	UINT nTextAlignment;
	DWORD dwFeatures;
	CStringArray aDefaultListData;
	mutable CStringArray aAutoListData;
	BOOL bEnabled;

private:
	// these are managed internally
	DWORD dwAttribType;
	TDC_COLUMN nColID;
	TDC_ATTRIBUTE nAttribID;
	///////////////////////////////////////////////////////////////////////////////

	void SetTypes(DWORD dwDataType, DWORD dwListType)
	{
		dwAttribType = (dwDataType | ValidateListType(dwDataType, dwListType));
	}

	static DWORD ValidateListType(DWORD dwDataType, DWORD dwListType)
	{
		// ensure list-type cannot be applied to dates,
		// or only partially to icons or flags
		switch (dwDataType)
		{
		case TDCCA_DATE:
			dwListType = TDCCA_NOTALIST;
			break;

		case TDCCA_BOOL:
			if (dwListType)
			{
				if (dwListType != TDCCA_FIXEDLIST)
					dwListType = TDCCA_FIXEDLIST;
			}
			break;

		case TDCCA_ICON:
			if (dwListType)
			{
				if ((dwListType != TDCCA_FIXEDLIST) && (dwListType != TDCCA_FIXEDMULTILIST))
					dwListType = TDCCA_FIXEDLIST;
			}
			break;
		}

		return dwListType;
	}
};

class CTDCCustomAttribDefinitionArray : public CArray<TDCCUSTOMATTRIBUTEDEFINITION, TDCCUSTOMATTRIBUTEDEFINITION&>
{
public:
	int Add(TDCCUSTOMATTRIBUTEDEFINITION& newElement)
	{
		int nIndex = CArray<TDCCUSTOMATTRIBUTEDEFINITION, TDCCUSTOMATTRIBUTEDEFINITION&>::Add(newElement);
		RebuildIDs();

		return nIndex;
	};

	void InsertAt(int nIndex, TDCCUSTOMATTRIBUTEDEFINITION& newElement, int nCount = 1)
	{
		CArray<TDCCUSTOMATTRIBUTEDEFINITION, TDCCUSTOMATTRIBUTEDEFINITION&>::InsertAt(nIndex, newElement, nCount);
		RebuildIDs();
	}

	void RemoveAt(int nIndex, int nCount = 1)
	{
		CArray<TDCCUSTOMATTRIBUTEDEFINITION, TDCCUSTOMATTRIBUTEDEFINITION&>::RemoveAt(nIndex, nCount);
		RebuildIDs();
	}

protected:
	void RebuildIDs()
	{
		int nColID = TDCC_CUSTOMCOLUMN_FIRST;
		int nAttribID = TDCA_CUSTOMATTRIB_FIRST;

		for (int nAttrib = 0; nAttrib < GetSize(); nAttrib++)
		{
			TDCCUSTOMATTRIBUTEDEFINITION& attribDef = ElementAt(nAttrib);

			attribDef.nColID = (TDC_COLUMN)nColID++;
			attribDef.nAttribID = (TDC_ATTRIBUTE)nAttribID++;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCOPERATOR
{
	FIND_OPERATOR op;
	UINT nOpResID;
};

struct SEARCHPARAM
{
	friend struct SEARCHPARAMS;

	SEARCHPARAM(TDC_ATTRIBUTE a = TDCA_NONE, FIND_OPERATOR o = FOP_NONE) : 
		attrib(TDCA_NONE), op(o), dValue(0), nValue(0), bAnd(TRUE), dwFlags(0), nType(FT_NONE) 
	{
		SetAttribute(a);
	}
	
	SEARCHPARAM(TDC_ATTRIBUTE a, FIND_OPERATOR o, CString s, BOOL and = TRUE, FIND_ATTRIBTYPE ft = FT_NONE) : 
		attrib(TDCA_NONE), op(o), dValue(0), nValue(0), bAnd(and), dwFlags(0), nType(ft)  
	{
		SetAttribute(a);

		if (!s.IsEmpty())
			SetValue(s);
	}
	
	SEARCHPARAM(TDC_ATTRIBUTE a, FIND_OPERATOR o, double d, BOOL and = TRUE) : 
		attrib(TDCA_NONE), op(o), dValue(0), nValue(0), bAnd(and), dwFlags(0), nType(FT_NONE)  
	{
		SetAttribute(a);

		nType = GetAttribType();

		if (nType == FT_DOUBLE || nType == FT_DATE)
			dValue = d;
	}
	
	SEARCHPARAM(TDC_ATTRIBUTE a, FIND_OPERATOR o, int n, BOOL and = TRUE) : 
		attrib(TDCA_NONE), op(o), dValue(0), nValue(0), bAnd(and), dwFlags(0), nType(FT_NONE)  
	{
		SetAttribute(a);

		nType = GetAttribType();

		if (nType == FT_INTEGER || nType == FT_BOOL)
			nValue = n;
	}

	BOOL operator!=(const SEARCHPARAM& rule) const
	{ 
		return !(*this == rule);
	}

	BOOL operator==(const SEARCHPARAM& rule) const
	{ 
		// simple check
		if (IsCustomAttribute() != rule.IsCustomAttribute())
		{
			return FALSE;
		}

		// test for attribute equivalence
		if (IsCustomAttribute())
		{
			if (sCustAttribID.CompareNoCase(rule.sCustAttribID) != 0)
			{
				return FALSE;
			}
		}
		else if (attrib != rule.attrib)
		{
			return FALSE;
		}

		// test rest of attributes
		if (op == rule.op && bAnd == rule.bAnd && dwFlags == rule.dwFlags)
		{
			switch (GetAttribType())
			{
			case FT_BOOL:
				return TRUE; // handled by operator

			case FT_DATE:
			case FT_DOUBLE:
			case FT_TIME:
				return (dValue == rule.dValue);

			case FT_INTEGER:
				return (nValue == rule.nValue);

			case FT_STRING:
			case FT_DATE_REL:
			case FT_ICON:
				return (sValue == rule.sValue);
			}
		}

		// all else
		return FALSE;
	}

	BOOL Set(TDC_ATTRIBUTE a, FIND_OPERATOR o, CString s, BOOL and = TRUE, FIND_ATTRIBTYPE t = FT_NONE)
	{
		if (!SetAttribute(a, t))
			return FALSE;

		SetOperator(o);
		SetAnd(and);
		SetValue(s);

		return TRUE;
	}

	BOOL Set(TDC_ATTRIBUTE a, const CString& id, FIND_ATTRIBTYPE t, FIND_OPERATOR o, CString s, BOOL and = TRUE)
	{
		if (!SetCustomAttribute(a, id, t))
			return FALSE;

		SetOperator(o);
		SetAnd(and);
		SetValue(s);

		return TRUE;
	}

	BOOL SetAttribute(TDC_ATTRIBUTE a, FIND_ATTRIBTYPE t = FT_NONE)
	{
		ASSERT(!IsCustomAttribute(a));

		// custom attributes must have a custom ID
		if (IsCustomAttribute(a))
			return FALSE;

		// handle deprecated relative date attributes
		switch (attrib)
		{
		case TDCA_STARTDATE_RELATIVE:
			a = TDCA_STARTDATE;
			t = FT_DATE_REL;
			break;

		case TDCA_DUEDATE_RELATIVE:
			a = TDCA_DUEDATE;
			t = FT_DATE_REL;
			break;

		case TDCA_DONEDATE_RELATIVE:
			a = TDCA_DONEDATE;
			t = FT_DATE_REL;
			break;

		case TDCA_CREATIONDATE_RELATIVE:
			a = TDCA_CREATIONDATE;
			t = FT_DATE_REL;
			break;

		case TDCA_LASTMOD_RELATIVE:
			a = TDCA_LASTMOD;
			t = FT_DATE_REL;
			break;
		}

		attrib = a;
		nType = t;
		
		// update Attrib type
		GetAttribType();

		// handle relative dates
		if (nType == FT_DATE || nType == FT_DATE_REL) 
		{
			dwFlags = (nType == FT_DATE_REL);
		}

		ValidateOperator();
		return TRUE;
	}

	TDC_ATTRIBUTE GetAttribute() const { return attrib; }
	FIND_OPERATOR GetOperator() const { return op; }
	BOOL GetAnd() const { return bAnd; }
	DWORD GetFlags() const { return dwFlags; }
	BOOL IsCustomAttribute() const { return IsCustomAttribute(attrib); }

	BOOL SetCustomAttribute(TDC_ATTRIBUTE a, const CString& id, FIND_ATTRIBTYPE t)
	{
		ASSERT (IsCustomAttribute(a));
		ASSERT (t != FT_NONE);
		ASSERT (!id.IsEmpty());

		if (!IsCustomAttribute(a) || id.IsEmpty() || t == FT_NONE)
			return FALSE;

		attrib = a;
		nType = t;
		sCustAttribID = id;

		// handle relative dates
		if ((nType == FT_DATE) || (nType == FT_DATE_REL)) 
		{
			dwFlags = (nType == FT_DATE_REL);
		}

		ValidateOperator();
		return TRUE;
	}

	CString GetCustomAttributeID() const
	{
		ASSERT (IsCustomAttribute());
		ASSERT (!sCustAttribID.IsEmpty());

		return sCustAttribID;

	}

	BOOL SetOperator(FIND_OPERATOR o) 
	{
		if (IsValidOperator(GetAttribType(), o))
		{
			op = o;
			return TRUE;
		}

		return FALSE;
	}

	FIND_ATTRIBTYPE GetAttribType() const
	{
		if (nType == FT_NONE)
			nType = GetAttribType(attrib, dwFlags);

		ASSERT ((nType != FT_NONE) || 
				(attrib == TDCA_NONE) || 
				(attrib == TDCA_SELECTION));

		return nType;
	}

	void SetAttribType(FIND_ATTRIBTYPE nAttribType)
	{
		ASSERT (nAttribType != FT_NONE);
		ASSERT (IsCustomAttribute(attrib));

		if ((nAttribType != FT_NONE) && IsCustomAttribute(attrib))
		{
			nType = nAttribType;

			// handle relative dates
			if ((nType == FT_DATE) || (nType == FT_DATE_REL)) 
			{
				dwFlags = (nType == FT_DATE_REL);
			}
		}
	}

	void ClearValue()
	{
		sValue.Empty();
		dValue = 0.0;
		nValue = 0;
	}

	static BOOL IsCustomAttribute(TDC_ATTRIBUTE attrib)
	{
		return (attrib >= TDCA_CUSTOMATTRIB_FIRST && attrib <= TDCA_CUSTOMATTRIB_LAST);
	}

	static FIND_ATTRIBTYPE GetAttribType(TDC_ATTRIBUTE attrib, BOOL bRelativeDate)
	{
		switch (attrib)
		{
		case TDCA_TASKNAME:
		case TDCA_TASKNAMEORCOMMENTS:
		case TDCA_ANYTEXTATTRIBUTE:
		case TDCA_ALLOCTO:
		case TDCA_ALLOCBY:
		case TDCA_STATUS:
		case TDCA_CATEGORY:
		case TDCA_VERSION:
		case TDCA_COMMENTS:
		case TDCA_FILEREF:
		case TDCA_PROJNAME:
		case TDCA_CREATEDBY:
		case TDCA_EXTERNALID: 
		case TDCA_TAGS: 
		case TDCA_PATH: 
			return FT_STRING;

		case TDCA_ICON: 
			return FT_ICON;

		case TDCA_PRIORITY:
		case TDCA_COLOR:
		case TDCA_PERCENT:
		case TDCA_RISK:
		case TDCA_ID:
		case TDCA_PARENTID:
		case TDCA_POSITION:
			return FT_INTEGER;

		case TDCA_TIMEEST:
		case TDCA_TIMESPENT:
			return FT_TIME;

		case TDCA_COST:
			return FT_DOUBLE;

		case TDCA_FLAG:
		case TDCA_RECENTMODIFIED:
		case TDCA_DEPENDENCY: 
			return FT_BOOL;

		case TDCA_STARTDATE:
		case TDCA_STARTTIME:
		case TDCA_CREATIONDATE:
		case TDCA_DONEDATE:
		case TDCA_DUEDATE:
		case TDCA_DUETIME:
		case TDCA_LASTMOD:
			return (bRelativeDate ? FT_DATE_REL : FT_DATE);

	//	case TDCA_RECURRENCE: 
		}

		// custom attribute type must be set explicitly by caller

		return FT_NONE;
	}

	static BOOL IsValidOperator(FIND_ATTRIBTYPE nType, FIND_OPERATOR op)
	{
		switch (op)
		{
		case FOP_NONE:
			return TRUE;

		case FOP_EQUALS:
		case FOP_NOT_EQUALS:
			return (nType != FT_BOOL);

		case FOP_INCLUDES:
		case FOP_NOT_INCLUDES:
			return ((nType == FT_STRING) || (nType == FT_ICON));

		case FOP_ON_OR_BEFORE:
		case FOP_BEFORE:
		case FOP_ON_OR_AFTER:
		case FOP_AFTER:
			return (nType == FT_DATE || nType == FT_DATE_REL);

		case FOP_GREATER_OR_EQUAL:
		case FOP_GREATER:
		case FOP_LESS_OR_EQUAL:
		case FOP_LESS:
			return (nType == FT_INTEGER || nType == FT_DOUBLE || nType == FT_TIME);

		case FOP_SET:
		case FOP_NOT_SET:
			return TRUE;
		}

		return FALSE;
	}

	BOOL HasValidOperator() const
	{
		return IsValidOperator(GetAttribType(), op);
	}

	void ValidateOperator()
	{
		if (!HasValidOperator())
			op = FOP_NONE;
	}

	BOOL OperatorIs(FIND_OPERATOR o) const
	{
		return (op == o);
	}

	BOOL AttributeIs(TDC_ATTRIBUTE a) const
	{
		return (attrib == a);
	}

	BOOL TypeIs(FIND_ATTRIBTYPE t) const
	{
		return (GetAttribType() == t);
	}

	BOOL HasString() const
	{
		return (TypeIs(FT_STRING) && !sValue.IsEmpty());
	}

	BOOL HasIcon() const
	{
		return (TypeIs(FT_ICON) && !sValue.IsEmpty());
	}

	void SetAnd(BOOL and = TRUE)
	{
		bAnd = and;
	}

	void SetFlags(DWORD flags)
	{
		dwFlags = flags;
	}

	void SetValue(const CString& val)
	{
		switch (GetAttribType())
		{
		case FT_STRING:
		case FT_DATE_REL:
		case FT_ICON:
			sValue = val;
			break;
			
		case FT_DATE:
		case FT_DOUBLE:
		case FT_TIME:
			dValue = _ttof(val);
			break;
			
		case FT_INTEGER:
		case FT_BOOL:
			nValue = _ttoi(val);
			break;
		}
	}

	void SetValue(double val)
	{
		switch (GetAttribType())
		{
		case FT_DATE:
		case FT_DOUBLE:
		case FT_TIME:
			dValue = val;
			break;
			
		default:
			ASSERT(0);
			break;
		}
	}

	void SetValue(int val)
	{
		switch (GetAttribType())
		{
		case FT_INTEGER:
		case FT_BOOL:
			nValue = val;
			break;

		default:
			ASSERT(0);
			break;
		}
	}

	void SetValue(const COleDateTime& val)
	{
		switch (GetAttribType())
		{
		case FT_DATE:
			dValue = val;
			break;

		default:
			ASSERT(0);
			break;
		}
	}

	CString ValueAsString() const
	{
		switch (GetAttribType())
		{
		case FT_DATE:
		case FT_DOUBLE:
		case FT_TIME:
			return Misc::Format(dValue, 3);

		case FT_INTEGER:
			if (AttributeIs(TDCA_PRIORITY) && (nValue == FM_NOPRIORITY))
			{
				return CEnString(IDS_TDC_NONE);
			}
			else if (AttributeIs(TDCA_RISK) && (nValue == FM_NORISK))
			{
				return CEnString(IDS_TDC_NONE);
			}
			// else
			return Misc::Format(nValue);

		case FT_BOOL:
			return Misc::Format(nValue);
		}

		// all else
		return sValue;
	}

	double ValueAsDouble() const
	{
		switch (GetAttribType())
		{
		case FT_DATE:
		case FT_TIME:
		case FT_DOUBLE:
			return dValue;

		case FT_DATE_REL:
			return ValueAsDate().m_dt;

		case FT_INTEGER:
		case FT_BOOL:
			return (double)nValue;
		}

		// all else
		ASSERT(0);
		return 0.0;
	}

	int ValueAsInteger() const
	{
		switch (GetAttribType())
		{
		case FT_DATE:
		case FT_TIME:
		case FT_DOUBLE:
			return (int)dValue;
		
		case FT_DATE_REL:
			return (int)ValueAsDouble();

		case FT_INTEGER:
		case FT_BOOL:
			return nValue;
		}

		// all else
		ASSERT(0);
		return 0;
	}

	COleDateTime ValueAsDate() const
	{
		COleDateTime date;

		switch (GetAttribType())
		{
		case FT_DATE:
			date = dValue;
			break;

		case FT_DATE_REL:
			if (!CDateHelper::DecodeRelativeDate(sValue, date, FALSE, FALSE))
				CDateHelper::ClearDate(date);
			break;

		default:
			// all else
			ASSERT(0);
			break;
		}

		return date;
	}

protected:
	TDC_ATTRIBUTE attrib;
	CString sCustAttribID;
	FIND_OPERATOR op;
	CString sValue;
	int nValue;
	double dValue;
	BOOL bAnd;
	DWORD dwFlags;

	mutable FIND_ATTRIBTYPE nType;

};
typedef CArray<SEARCHPARAM, SEARCHPARAM> CSearchParamArray;

struct SEARCHPARAMS
{
	SEARCHPARAMS() : bIgnoreDone(FALSE), bIgnoreOverDue(FALSE), bWantAllSubtasks(FALSE), bIgnoreFilteredOut(TRUE) {}
	SEARCHPARAMS(const SEARCHPARAMS& params)
	{
		*this = params;
	}

	SEARCHPARAMS& operator=(const SEARCHPARAMS& params)
	{
		bIgnoreDone = params.bIgnoreDone;
		bIgnoreOverDue = params.bIgnoreOverDue;
		bWantAllSubtasks = params.bWantAllSubtasks;
		bIgnoreFilteredOut = params.bIgnoreFilteredOut;

		aRules.Copy(params.aRules);
		aAttribDefs.Copy(params.aAttribDefs);

		return *this;
	}

	BOOL operator!=(const SEARCHPARAMS& params) const
	{
		return !(*this == params);
	}

	BOOL operator==(const SEARCHPARAMS& params) const
	{
		return Misc::MatchAllT(aRules, params.aRules, TRUE) && 
				Misc::MatchAllT(aAttribDefs, params.aAttribDefs) &&
				(bIgnoreDone == params.bIgnoreDone) && 
				(bIgnoreOverDue == params.bIgnoreOverDue) && 
				(bIgnoreFilteredOut == params.bIgnoreFilteredOut) && 
				(bWantAllSubtasks == params.bWantAllSubtasks);
	}

	void Clear()
	{
		bIgnoreDone = FALSE;
		bIgnoreOverDue = FALSE;
		bWantAllSubtasks = FALSE;
		bIgnoreFilteredOut = TRUE;

		aRules.RemoveAll();
		aAttribDefs.RemoveAll();
	}

	BOOL HasAttribute(TDC_ATTRIBUTE attrib) const
	{
		int nRule = aRules.GetSize();

		while (nRule--)
		{
			if (aRules[nRule].attrib == attrib)
				return TRUE;

			// special cases
			if (aRules[nRule].attrib == TDCA_TASKNAMEORCOMMENTS &&
				(attrib == TDCA_TASKNAME || attrib == TDCA_COMMENTS))
				return TRUE;

			else if (aRules[nRule].attrib == TDCA_ANYTEXTATTRIBUTE &&
				(attrib == TDCA_TASKNAME || attrib == TDCA_COMMENTS ||
				 attrib == TDCA_STATUS || attrib == TDCA_CATEGORY ||
				 attrib == TDCA_ALLOCBY || attrib == TDCA_ALLOCTO ||
				 attrib == TDCA_VERSION || attrib == TDCA_TAGS ||
				 attrib == TDCA_EXTERNALID))
				return TRUE;
		}

		// else
		return FALSE;
	}

	BOOL GetRuleCount() const
	{
		return aRules.GetSize();
	}

	CSearchParamArray aRules;
	CTDCCustomAttribDefinitionArray aAttribDefs;
	BOOL bIgnoreDone, bIgnoreOverDue, bWantAllSubtasks, bIgnoreFilteredOut;
};

struct SEARCHRESULT
{
	SEARCHRESULT() : dwTaskID(0), dwFlags(0) {}

	SEARCHRESULT(const SEARCHRESULT& res)
	{
		*this = res;
	}

	SEARCHRESULT& operator=(const SEARCHRESULT& res)
	{
		dwTaskID = res.dwTaskID;
		aMatched.Copy(res.aMatched);
		dwFlags = res.dwFlags;

		return *this;
	}

	BOOL HasFlag(DWORD dwFlag) const 
	{
		return ((dwFlags & dwFlag) == dwFlag) ? 1 : 0;
	}

	DWORD dwTaskID;
	DWORD dwFlags;
	CStringArray aMatched;
};

typedef CArray<SEARCHRESULT, SEARCHRESULT&> CResultArray;

/////////////////////////////////////////////////////////////////////////////////////////////

struct FTDRESULT
{
	FTDRESULT() : dwTaskID(0), pTDC(NULL), dwFlags(0) {}
	FTDRESULT(const SEARCHRESULT& result, const CFilteredToDoCtrl* pTaskList) 
		: 
		dwTaskID(result.dwTaskID), 
		pTDC(pTaskList), 
		dwFlags(result.dwFlags) 
	{
	}

	BOOL IsReference() const	{ return (dwFlags & RF_REFERENCE); }
	BOOL IsDone() const			{ return (dwFlags & RF_DONE); }
	BOOL IsGoodAsDone() const	{ return (dwFlags & RF_GOODASDONE); }
	BOOL IsParent() const		{ return (dwFlags & RF_PARENT); }
	BOOL IsTopmost() const		{ return (dwFlags & RF_TOPMOST); }
	
	DWORD dwTaskID, dwFlags;
	const CFilteredToDoCtrl* pTDC;
};

typedef CArray<FTDRESULT, FTDRESULT&> CFTDResultsArray;

/////////////////////////////////////////////////////////////////////////////////////////////

struct FTDCFILTER
{
	FTDCFILTER() 
		: 
		nShow(FS_ALL), 
		nStartBy(FD_ANY), 
		nDueBy(FD_ANY), 
		nPriority(FM_ANYPRIORITY), 
		nRisk(FM_ANYRISK), 
		dwFlags(FO_ANYALLOCTO | FO_ANYCATEGORY | FO_ANYTAG), 
		nTitleOption(FT_FILTERONTITLEONLY),
		nStartNextNDays(7), 
		nDueNextNDays(7)

	{
		dtUserStart = dtUserDue = COleDateTime::GetCurrentTime();
	}

	FTDCFILTER(const FTDCFILTER& filter)
	{
		*this = filter;
	}

	FTDCFILTER& operator=(const FTDCFILTER& filter)
	{
		nShow = filter.nShow;
		nStartBy = filter.nStartBy;
		nDueBy = filter.nDueBy;
		dtUserStart = filter.dtUserStart;
		dtUserDue = filter.dtUserDue;
		sTitle = filter.sTitle;
		nTitleOption = filter.nTitleOption;
		nPriority = filter.nPriority;
		nRisk = filter.nRisk;
		dwFlags = filter.dwFlags;
		nStartNextNDays = filter.nStartNextNDays; 
		nDueNextNDays = filter.nDueNextNDays;

		aAllocTo.Copy(filter.aAllocTo);
		aStatus.Copy(filter.aStatus);
		aAllocBy.Copy(filter.aAllocBy);
		aCategories.Copy(filter.aCategories);
		aVersions.Copy(filter.aVersions);
		aTags.Copy(filter.aTags);

		return *this;
	}

	BOOL operator==(const FTDCFILTER& filter) const
	{
		return FiltersMatch(*this, filter);
	}

	BOOL operator!=(const FTDCFILTER& filter) const
	{
		return !FiltersMatch(*this, filter);
	}
	
	BOOL IsSet(DWORD dwIgnore = 0) const
	{
		FTDCFILTER filterEmpty;

		return !FiltersMatch(*this, filterEmpty, dwIgnore);
	}

	DWORD GetFlags() const { return dwFlags; }

	BOOL HasFlag(DWORD dwFlag) const
	{
		BOOL bHasFlag = Misc::HasFlag(dwFlags, dwFlag);

		// some flags depend also on the main filter
		if (bHasFlag)
		{
			switch (dwFlag)
			{
			case FO_ANYCATEGORY:
			case FO_ANYALLOCTO:
			case FO_ANYTAG:
			case FO_HIDEPARENTS:
			case FO_HIDECOLLAPSED:
			case FO_SHOWALLSUB:
				break; // always allowed

			case FO_HIDEOVERDUE:
				bHasFlag = (nDueBy == FD_TODAY) ||
							(nDueBy == FD_YESTERDAY) ||
							(nDueBy == FD_TOMORROW) ||
							(nDueBy == FD_NEXTNDAYS) ||
							(nDueBy == FD_ENDTHISWEEK) || 
							(nDueBy == FD_ENDNEXTWEEK) || 
							(nDueBy == FD_ENDTHISMONTH) ||
							(nDueBy == FD_ENDNEXTMONTH) ||
							(nDueBy == FD_ENDTHISYEAR) ||
							(nDueBy == FD_ENDNEXTYEAR);
				break;

			case FO_HIDEDONE:
				bHasFlag = (nShow != FS_NOTDONE && nShow != FS_DONE);
				break;

			default:
				ASSERT(0);
				break;		
			}	
		}

		return bHasFlag;
	}

	void SetFlag(DWORD dwFlag, BOOL bOn = TRUE)
	{
		if (bOn)
			dwFlags |= dwFlag;
		else
			dwFlags &= ~dwFlag;
	}

	void Reset()
	{
		*this = FTDCFILTER(); // empty filter
	}

	BOOL Matches(const FTDCFILTER& filter, DWORD dwIgnore = 0) const
	{
		return FiltersMatch(*this, filter, dwIgnore);
	}

	static BOOL FiltersMatch(const FTDCFILTER& filter1, const FTDCFILTER& filter2, DWORD dwIgnore = 0)
	{
		// simple exclusion test first
		if ((filter1.nShow != filter2.nShow) ||
			(filter1.nStartBy != filter2.nStartBy) ||
			(filter1.nDueBy != filter2.nDueBy) ||
			(filter1.nPriority != filter2.nPriority) ||
			(filter1.nRisk != filter2.nRisk) ||
			(filter1.sTitle != filter2.sTitle) ||
			((filter1.nTitleOption != filter2.nTitleOption) && !filter1.sTitle.IsEmpty()))
		{
			return FALSE;
		}

		// user-specified due date
		if (filter1.nDueBy == FD_USER) // == filter2.nDueBy
		{
			if (filter1.dtUserDue != filter2.dtUserDue)
				return FALSE;
		}

		// user-specified start date
		if (filter1.nStartBy == FD_USER) // == filter2.nStartBy
		{
			if (filter1.dtUserStart != filter2.dtUserStart)
				return FALSE;
		}

		// user-specified day range
		if (filter1.nDueBy == FD_NEXTNDAYS) // == filter2.nStartBy
		{
			if (filter1.nDueNextNDays != filter2.nDueNextNDays)
				return FALSE;
		}

		// user-specified day range
		if (filter1.nStartBy == FD_NEXTNDAYS) // == filter2.nStartBy
		{
			if (filter1.nStartNextNDays != filter2.nStartNextNDays)
				return FALSE;
		}

		// options
		if ((dwIgnore & FO_HIDEPARENTS) == 0)
		{
			if (filter1.HasFlag(FO_HIDEPARENTS) != filter2.HasFlag(FO_HIDEPARENTS))
				return FALSE;
		}

		if ((dwIgnore & FO_HIDEDONE) == 0)
		{
			if (filter1.HasFlag(FO_HIDEDONE) != filter2.HasFlag(FO_HIDEDONE))
				return FALSE;
		}

		if ((dwIgnore & FO_HIDEOVERDUE) == 0)
		{
			if (filter1.HasFlag(FO_HIDEOVERDUE) != filter2.HasFlag(FO_HIDEOVERDUE))
				return FALSE;
		}

		if ((dwIgnore & FO_HIDECOLLAPSED) == 0)
		{
			if (filter1.HasFlag(FO_HIDECOLLAPSED) != filter2.HasFlag(FO_HIDECOLLAPSED))
				return FALSE;
		}

		if ((dwIgnore & FO_SHOWALLSUB) == 0)
		{
			if (filter1.HasFlag(FO_SHOWALLSUB) != filter2.HasFlag(FO_SHOWALLSUB))
				return FALSE;
		}

		// compare categories disregarding dwFlags if there's not actually something to compare
		if (filter1.aCategories.GetSize() || filter2.aCategories.GetSize())
		{
			if (!Misc::MatchAll(filter1.aCategories, filter2.aCategories) ||
				filter1.HasFlag(FO_ANYCATEGORY) != filter2.HasFlag(FO_ANYCATEGORY))
			{
				return FALSE;
			}
		}

		// do the same with Alloc_To
		if (filter1.aAllocTo.GetSize() || filter2.aAllocTo.GetSize())
		{
			if (!Misc::MatchAll(filter1.aAllocTo, filter2.aAllocTo) ||
				filter1.HasFlag(FO_ANYALLOCTO) != filter2.HasFlag(FO_ANYALLOCTO))
			{
				return FALSE;
			}
		}

		// and the same with Tags
		if (filter1.aTags.GetSize() || filter2.aTags.GetSize())
		{
			if (!Misc::MatchAll(filter1.aTags, filter2.aTags) ||
				filter1.HasFlag(FO_ANYTAG) != filter2.HasFlag(FO_ANYTAG))
			{
				return FALSE;
			}
		}

		// and the same with Alloc_By
		if (filter1.aAllocBy.GetSize() || filter2.aAllocBy.GetSize())
		{
			if (!Misc::MatchAll(filter1.aAllocBy, filter2.aAllocBy))
				return FALSE;
		}

		// and the same with Status
		if (filter1.aStatus.GetSize() || filter2.aStatus.GetSize())
		{
			if (!Misc::MatchAll(filter1.aStatus, filter2.aStatus))
				return FALSE;
		}

		// and the same with Version
		if (filter1.aVersions.GetSize() || filter2.aVersions.GetSize())
		{
			if (!Misc::MatchAll(filter1.aVersions, filter2.aVersions))
				return FALSE;
		}

		return TRUE;
	}
	
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
};

struct FTDCCUSTOMFILTER
{
	FTDCCUSTOMFILTER(LPCTSTR szName = NULL, DWORD flags = 0) : sName(szName), dwFlags(flags)
	{
	}

	FTDCCUSTOMFILTER(const FTDCCUSTOMFILTER& filter)
	{
		*this = filter;
	}
	
	FTDCCUSTOMFILTER& operator=(const FTDCCUSTOMFILTER& filter)
	{
		params = filter.params;
		dwFlags = filter.dwFlags;
		sName = filter.sName;
				
		return *this;
	}
	
	BOOL operator==(const FTDCCUSTOMFILTER& filter) const
	{
		return Matches(filter);
	}
	
	BOOL operator!=(const FTDCCUSTOMFILTER& filter) const
	{
		return !Matches(filter);
	}
	
	BOOL IsSet(DWORD dwIgnore = 0) const
	{
		FTDCCUSTOMFILTER filterEmpty;
		
		return !Matches(filterEmpty, dwIgnore);
	}
	
	DWORD GetFlags() const { return dwFlags; }
	
	BOOL HasFlag(DWORD dwFlag) const
	{
		return Misc::HasFlag(dwFlags, dwFlag);
	}

	void Reset()
	{
		params.Clear();
		sName.Empty();
		dwFlags = 0;
	}

	BOOL Matches(const FTDCCUSTOMFILTER& filter, DWORD dwIgnoreFlags = 0) const
	{
		if (!Matches(filter.sName, filter.dwFlags, dwIgnoreFlags))
			return FALSE;

		return (params == filter.params);
	}
	
	BOOL Matches(LPCTSTR szCustom, DWORD dwCustomFlags, DWORD dwIgnoreFlags = 0) const
	{
		return ((sName == szCustom) && ((dwFlags & ~dwIgnoreFlags) == (dwCustomFlags & ~dwIgnoreFlags)));
	}
	
	SEARCHPARAMS params;
	CString sName;
	DWORD dwFlags;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDSORTCOLUMN
{
	TDSORTCOLUMN(TDC_COLUMN nSortBy = TDCC_NONE, BOOL bSortAscending = -1) 
		: nBy(nSortBy), bAscending(bSortAscending)
	{
	}

	BOOL IsSorting() const
	{
		return (nBy != TDCC_NONE);
	}

	BOOL IsSortingBy(TDC_COLUMN nSortBy) const
	{
		return (nBy == nSortBy);
	}
	
	BOOL IsSortingByCustom() const
	{
		return ((nBy >= TDCC_CUSTOMCOLUMN_FIRST) && (nBy <= TDCC_CUSTOMCOLUMN_LAST));
	}

	BOOL Verify() const
	{
		return Verify(nBy);
	}

	void Validate()
	{
		if (!Verify())
			nBy = TDCC_NONE;
	}

	static BOOL Verify(TDC_COLUMN nSortBy) 
	{
		return (((nSortBy >= TDCC_FIRST) && (nSortBy < TDCC_COUNT)) ||
				(nSortBy == TDCC_CLIENT) || (nSortBy == TDCC_NONE));
	}

	TDC_COLUMN nBy;
	BOOL bAscending;
};

struct TDSORTCOLUMNS
{
	TDSORTCOLUMNS() : col1(TDCC_NONE), col2(TDCC_NONE), col3(TDCC_NONE)
	{
	}
	
	TDSORTCOLUMNS(TDC_COLUMN nBy, BOOL bAscending = -1) 
		:  col1(nBy, bAscending), col2(TDCC_NONE), col3(TDCC_NONE)

	{
	}
	
	BOOL IsSorting() const
	{
		return col1.IsSorting();
	}
	
	BOOL IsSortingBy(TDC_COLUMN nSortBy) const
	{
		if (col1.IsSortingBy(nSortBy))
			return TRUE;

		if (col1.IsSorting())
		{
			if (col2.IsSortingBy(nSortBy))
				return TRUE;
			
			if (col2.IsSorting())
			{
				return col3.IsSortingBy(nSortBy);
			}
		}

		// else
		return FALSE;
	}

	BOOL Verify() const
	{
		return (col1.Verify() && col2.Verify() && col3.Verify());
	}

	void Validate()
	{
		col1.Validate();
		col2.Validate();
		col3.Validate();
	}

	TDSORTCOLUMN col1, col2, col3;
};

struct TDSORT
{
	TDSORT(TDC_COLUMN nBy = TDCC_NONE, BOOL bAscending = -1) 
		: 
		single(nBy, bAscending),
		multi(nBy, bAscending),
		bModSinceLastSort(FALSE), 
		bMulti(FALSE)
	{
	}

	BOOL IsSorting() const
	{
		if (bMulti)
			return multi.IsSorting();

		// else
		return single.IsSorting();
	}

	BOOL IsSortingBy(TDC_COLUMN nBy, BOOL bCheckMulti) const
	{
		if (!bMulti)
			return single.IsSortingBy(nBy);

		// else multi-sorting
		if (bCheckMulti)
			return multi.IsSortingBy(nBy);

		// else
		return FALSE;
	}

	BOOL SetSortBy(TDC_COLUMN nBy, BOOL bAscending)
	{
		single.nBy = nBy;
		single.bAscending = bAscending;
		bMulti = FALSE;

		return TRUE;
	}

	BOOL SetSortBy(const TDSORTCOLUMN& col)
	{
		return SetSortBy(col.nBy, col.bAscending);
	}

	BOOL SetSortBy(const TDSORTCOLUMNS& cols)
	{
		multi = cols;
		bMulti = TRUE;
		
		return TRUE;
	}

	BOOL Verify() const
	{
		return (single.Verify() && multi.Verify());
	}

	void Validate()
	{
		single.Validate();
		multi.Validate();
	}

	TDSORTCOLUMN single;
	TDSORTCOLUMNS multi;

	BOOL bMulti;
	BOOL bModSinceLastSort;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCATTRIBUTE
{
	TDC_ATTRIBUTE attrib;
	UINT nAttribResID;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCADATA
{
	TDCCADATA(const CString& sValue = _T("")) { Set(sValue); }
	TDCCADATA(double dValue) { Set(dValue); }
	TDCCADATA(const CStringArray& aValues) { Set(aValues); }
	TDCCADATA(int nValue) { Set(nValue); }
	TDCCADATA(const COleDateTime& dtValue) { Set(dtValue); }
	TDCCADATA(bool bValue) { Set(bValue); }

	TDCCADATA(const TDCCADATA& data)
	{
		*this = data;
	}

	TDCCADATA& operator=(const TDCCADATA& data)
	{
		sData = data.sData;
		return *this;
	}

	BOOL operator==(const TDCCADATA& data) const { return sData == data.sData; }
	BOOL operator!=(const TDCCADATA& data) const { return sData != data.sData; }

	BOOL IsEmpty() const { return sData.IsEmpty(); }

	CString AsString() const { return sData; }
	double AsDouble() const { return _ttof(sData); }
	int AsArray(CStringArray& aValues) const { return Misc::Split(sData, aValues, '\n', TRUE); }
	int AsInteger() const { return _ttoi(sData); } 
	COleDateTime AsDate() const { return _ttof(sData); }
	bool AsBool() const { return !IsEmpty(); }

	void Set(const CString& sValue) { sData = sValue; }
	void Set(double dValue) { sData.Format(_T("%lf"), dValue); }
	void Set(const CStringArray& aValues) { sData = Misc::FormatArray(aValues, '\n'); }
	void Set(int nValue) { sData.Format(_T("%d"), nValue); }
	void Set(const COleDateTime& dtValue) { sData.Format(_T("%lf"), dtValue); }

	void Set(bool bValue, TCHAR nChar = 0) 
	{ 
		if (bValue)
		{
			if (nChar > 0)
				sData.Insert(0, nChar);
			else
				sData.Insert(0, '+');
		}
		else
			sData.Empty(); 
	}

protected:
	CString sData;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TASKFILE_HEADER
{
	TASKFILE_HEADER() : bArchive(-1), bUnicode(-1), dwNextID(0), nFileFormat(-1), nFileVersion(-1) {}

	CString sXmlHeader;
	CString sXslHeader;
	CString sProjectName;
	CString sFileName;
	CString sCheckedOutTo;
	BOOL bArchive;
	BOOL bUnicode;
	COleDateTime dtEarliestDue;
	DWORD dwNextID;
	int nFileFormat;
	int nFileVersion;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCOLEDITFILTERVISIBILITY
{
	TDCCOLEDITFILTERVISIBILITY() : nShowEditsAndFilters(TDLSA_ASCOLUMN) {}

	TDCCOLEDITFILTERVISIBILITY(const TDCCOLEDITFILTERVISIBILITY& vis)
	{
		*this = vis;
	}
	
	TDCCOLEDITFILTERVISIBILITY& operator=(const TDCCOLEDITFILTERVISIBILITY& vis)
	{
		mapVisibleColumns.CopyColumns(vis.mapVisibleColumns);
		mapVisibleEdits.CopyAttributes(vis.mapVisibleEdits);
		mapVisibleFilters.CopyAttributes(vis.mapVisibleFilters);

		nShowEditsAndFilters = vis.nShowEditsAndFilters;
		
		return *this;
	}
	
	BOOL operator!=(const TDCCOLEDITFILTERVISIBILITY& vis) const
	{
		return !(*this == vis);
	}
	
	BOOL operator==(const TDCCOLEDITFILTERVISIBILITY& vis) const
	{
		return (mapVisibleColumns.MatchAllColumns(vis.mapVisibleColumns) && 
				mapVisibleEdits.MatchAllAttributes(vis.mapVisibleEdits) &&
				mapVisibleFilters.MatchAllAttributes(vis.mapVisibleFilters) &&
				(nShowEditsAndFilters == vis.nShowEditsAndFilters));
	}

	void Clear()
	{
		mapVisibleColumns.RemoveAll();
		mapVisibleEdits.RemoveAll();
		mapVisibleFilters.RemoveAll();

		nShowEditsAndFilters = TDLSA_ASCOLUMN;
	}

	BOOL CheckForDiff(const TDCCOLEDITFILTERVISIBILITY& vis, 
						BOOL& bColumnChange, BOOL& bEditChange, BOOL& bFilterChange) const
	{
		bColumnChange = !mapVisibleColumns.MatchAllColumns(vis.mapVisibleColumns);
		bEditChange = !mapVisibleEdits.MatchAllAttributes(vis.mapVisibleEdits);
		bFilterChange = !mapVisibleEdits.MatchAllAttributes(vis.mapVisibleEdits);
		
		return (bEditChange || bColumnChange || bFilterChange);
	}

	TDL_SHOWATTRIB GetShowEditsAndFilters() const 
	{
		return nShowEditsAndFilters;
	}
	
	void SetShowEditsAndFilters(TDL_SHOWATTRIB nShow)
	{
		nShowEditsAndFilters = nShow;

		UpdateEditAndFilterVisibility();
	}

	const CTDCColumnIDMap& GetVisibleColumns() const
	{
		return mapVisibleColumns;
	}

	void SetVisibleColumns(const CTDCColumnIDMap& mapColumns)
	{
		mapVisibleColumns.CopyColumns(mapColumns);
	}

	void SetAllColumnsVisible(BOOL bVisible = TRUE)
	{
		mapVisibleColumns.RemoveAll();

		for (int nCol = TDCC_FIRST; nCol < TDCC_COUNT; nCol++)
		{
			if (bVisible && IsSupportedColumn((TDC_COLUMN)nCol))
				mapVisibleColumns.AddColumn((TDC_COLUMN)nCol);
		}

		UpdateEditAndFilterVisibility();
	}
	
	BOOL SetAllEditsAndFiltersVisible(BOOL bVisible = TRUE)
	{
		ASSERT(nShowEditsAndFilters == TDLSA_ANY);

		if (nShowEditsAndFilters != TDLSA_ANY)
			return FALSE;

		mapVisibleEdits.RemoveAll();
		mapVisibleFilters.RemoveAll();
		
		if (bVisible)
		{
			GetAllEditFields(mapVisibleEdits);
			GetAllFilterFields(mapVisibleFilters);
		}

		return TRUE;
	}

	const CTDCAttributeMap& GetVisibleEditFields() const
	{
		return mapVisibleEdits;
	}
	
	const CTDCAttributeMap& GetVisibleFilterFields() const
	{
		return mapVisibleFilters;
	}

	BOOL SetVisibleEditFields(const CTDCAttributeMap& mapAttrib)
	{
		// only supported for 'any' attribute
		ASSERT(nShowEditsAndFilters == TDLSA_ANY);
		
		if (nShowEditsAndFilters != TDLSA_ANY)
			return FALSE;

		// check and add attributes 
		mapVisibleEdits.RemoveAll();

		BOOL bAnySupported = FALSE;
		POSITION pos = mapAttrib.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE nAttrib = mapAttrib.GetNextAttribute(pos);

			if (IsSupportedEdit(nAttrib))
			{
				mapVisibleEdits.AddAttribute(nAttrib);
				bAnySupported = TRUE;
			}
		}
		
		return bAnySupported;
	}

	BOOL SetVisibleFilterFields(const CTDCAttributeMap& mapAttrib)
	{
		// only supported for 'any' attribute
		ASSERT(nShowEditsAndFilters == TDLSA_ANY);
		
		if (nShowEditsAndFilters != TDLSA_ANY)
			return FALSE;

		// check and add attributes 
		BOOL bAnySupported = FALSE;
		POSITION pos = mapAttrib.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE nAttrib = mapAttrib.GetNextAttribute(pos);

			if (IsSupportedFilter(nAttrib))
			{
				mapVisibleFilters.AddAttribute(nAttrib);
				bAnySupported = TRUE;
			}
		}
		
		return bAnySupported;
	}
	
	BOOL IsEditFieldVisible(TDC_ATTRIBUTE nAttrib) const
	{
		// weed out unsupported attributes
		if (!IsSupportedEdit(nAttrib))
			return FALSE;

		switch (nShowEditsAndFilters)
		{
		case TDLSA_ALL:
			return TRUE;

		case TDLSA_ASCOLUMN:
			return IsColumnVisible(TDC::MapAttributeToColumn(nAttrib));

		case TDLSA_ANY:
			return mapVisibleEdits.HasAttribute(nAttrib);
		}

		// how did we get here?
		ASSERT(0);
		return FALSE;
	}
	
	BOOL IsFilterFieldVisible(TDC_ATTRIBUTE nAttrib) const
	{
		// weed out unsupported attributes
		if (!IsSupportedFilter(nAttrib))
			return FALSE;

		switch (nShowEditsAndFilters)
		{
		case TDLSA_ALL:
			return TRUE;

		case TDLSA_ASCOLUMN:
			return IsColumnVisible(TDC::MapAttributeToColumn(nAttrib));

		case TDLSA_ANY:
			return mapVisibleFilters.HasAttribute(nAttrib);
		}

		// how did we get here?
		ASSERT(0);
		return FALSE;
	}

	BOOL IsColumnVisible(TDC_COLUMN nCol) const
	{
		// special cases
		switch (nCol)
		{
		case TDCC_NONE:
		case TDCC_COLOR:
			return FALSE;
		}

		// else
		return mapVisibleColumns.HasColumn(nCol);
	}
	
	BOOL SetEditFieldVisible(TDC_ATTRIBUTE nAttrib, BOOL bVisible = TRUE)
	{
		// only supported for 'any' attribute
		ASSERT(nShowEditsAndFilters == TDLSA_ANY);

		if (nShowEditsAndFilters != TDLSA_ANY)
			return FALSE;

		// validate attribute
		ASSERT (IsSupportedEdit(nAttrib));

		if (!IsSupportedEdit(nAttrib))
		{
			return FALSE;
		}
		else if (bVisible)
		{
			// Times cannot be shown if corresponding date column is hidden
			switch (nAttrib)
			{
			case TDCA_DUETIME:
				bVisible &= IsEditFieldVisible(TDCA_DUEDATE);
				break;
				
			case TDCA_DONETIME:
				bVisible &= IsEditFieldVisible(TDCA_DONEDATE);
				break;
				
			case TDCA_STARTTIME:
				bVisible &= IsEditFieldVisible(TDCA_STARTDATE);
				break;
			}
		}
		else // !bVisible
		{
			// Times cannot be shown if corresponding date column is hidden
			switch (nAttrib)
			{
			case TDCA_DUEDATE:
				SetEditFieldVisible(TDCA_DUETIME, FALSE);
				break;
				
			case TDCA_DONEDATE:
				SetEditFieldVisible(TDCA_DONETIME, FALSE);
				break;
				
			case TDCA_STARTDATE:
				SetEditFieldVisible(TDCA_STARTTIME, FALSE);
				break;
			}
		}

		BOOL bFound = mapVisibleEdits.HasAttribute(nAttrib);
		
		if (bVisible && !bFound)
		{
			mapVisibleEdits.AddAttribute(nAttrib);
		}
		else if (!bVisible && bFound)
		{
			mapVisibleEdits.RemoveKey(nAttrib);
		}

		return TRUE;
	}
	
	BOOL SetFilterFieldVisible(TDC_ATTRIBUTE nAttrib, BOOL bVisible = TRUE)
	{
		// only supported for 'any' attribute
		ASSERT(nShowEditsAndFilters == TDLSA_ANY);

		if (nShowEditsAndFilters != TDLSA_ANY)
			return FALSE;

		// validate attribute
		ASSERT (IsSupportedFilter(nAttrib));

		if (!IsSupportedFilter(nAttrib))
			return FALSE;

		BOOL bFound = mapVisibleFilters.HasAttribute(nAttrib);
		
		if (bVisible && !bFound)
		{
			mapVisibleFilters.AddAttribute(nAttrib);
		}
		else if (!bVisible && bFound)
		{
			mapVisibleFilters.RemoveAttribute(nAttrib);
		}

		return TRUE;
	}

	void SetColumnVisible(TDC_COLUMN nCol, BOOL bVisible = TRUE)
	{
		// special cases
		if (nCol == TDCC_NONE)
		{
			return;
		}
		else if (bVisible)
		{
			// Times cannot be shown if corresponding date column is hidden
			switch (nCol)
			{
			case TDCC_DUETIME:
				bVisible &= IsColumnVisible(TDCC_DUEDATE);
				break;
				
			case TDCC_DONETIME:
				bVisible &= IsColumnVisible(TDCC_DONEDATE);
				break;
				
			case TDCC_STARTTIME:
				bVisible &= IsColumnVisible(TDCC_STARTDATE);
				break;
			}
		}
		else // !bVisible
		{
			// Times cannot be shown if corresponding date column is hidden
			switch (nCol)
			{
			case TDCC_DUEDATE:
				SetColumnVisible(TDCC_DUETIME, FALSE);
				break;
				
			case TDCC_DONEDATE:
				SetColumnVisible(TDCC_DONETIME, FALSE);
				break;
				
			case TDCC_STARTDATE:
				SetColumnVisible(TDCC_STARTTIME, FALSE);
				break;
			}
		}

		BOOL bFound = mapVisibleColumns.HasColumn(nCol);
		
		if (bVisible && !bFound) // show
		{
			mapVisibleColumns.AddColumn(nCol);
		}
		else if (!bVisible && bFound) // hide
		{
			mapVisibleColumns.RemoveColumn(nCol);
		}

		UpdateEditAndFilterVisibility();
	}

	void Save(IPreferences* pPrefs, LPCTSTR szKey) const
	{
		pPrefs->WriteProfileInt(szKey, _T("ShowAttributes"), nShowEditsAndFilters);

		CString sKey;
		
		sKey.Format(_T("%s\\ColumnVisibility"), szKey);
		mapVisibleColumns.SaveColumns(pPrefs, sKey, _T("Col%d"));

		sKey.Format(_T("%s\\EditVisibility"), szKey);
		mapVisibleEdits.SaveAttributes(pPrefs, sKey, _T("Edit%d"));

		sKey.Format(_T("%s\\FilterVisibility"), szKey);
		mapVisibleFilters.SaveAttributes(pPrefs, sKey, _T("Filter%d"));
	}

	BOOL Load(const IPreferences* pPrefs, LPCTSTR szKey)
	{
		nShowEditsAndFilters = (TDL_SHOWATTRIB)pPrefs->GetProfileInt(szKey, _T("ShowAttributes"));

		// columns
		CString sKey;
		sKey.Format(_T("%s\\ColumnVisibility"), szKey);

		int nCol = pPrefs->GetProfileInt(sKey, _T("Count"), -1);

		if (nCol == -1)
			return FALSE; // old version OR first time

		// else
		mapVisibleColumns.LoadColumns(pPrefs, sKey, _T("Col%d"));

		sKey.Format(_T("%s\\EditVisibility"), szKey);
		mapVisibleEdits.LoadAttributes(pPrefs, sKey, _T("Edit%d"));

		sKey.Format(_T("%s\\FilterVisibility"), szKey);
		mapVisibleFilters.LoadAttributes(pPrefs, sKey, _T("Filter%d"));

		return TRUE;
	}

	static BOOL IsSupportedEdit(TDC_ATTRIBUTE nAttrib)
	{
		// weed out unsupported attributes
		switch (nAttrib)
		{
		case TDCA_TASKNAME:
		case TDCA_COMMENTS:
		case TDCA_PROJNAME:
		case TDCA_FLAG:
		case TDCA_ICON:
		case TDCA_CREATIONDATE:
		case TDCA_CREATEDBY:
		case TDCA_POSITION:
		case TDCA_ID:
		case TDCA_LASTMOD:
			return FALSE;
		}

		// else
		return ((nAttrib >= TDCA_FIRSTATTRIBUTE) && (nAttrib < TDCA_ATTRIBUTECOUNT));
	}

	static BOOL IsSupportedFilter(TDC_ATTRIBUTE nAttrib)
	{
		switch (nAttrib)
		{
		case TDCA_DUEDATE:
		case TDCA_STARTDATE:
		case TDCA_PRIORITY:
		case TDCA_ALLOCTO:
		case TDCA_ALLOCBY:
		case TDCA_STATUS:
		case TDCA_CATEGORY:
		case TDCA_RISK:			
		case TDCA_VERSION:		
		case TDCA_TAGS:
			return TRUE;
		}

		// else
		return FALSE;
	}
	
	static BOOL IsSupportedColumn(TDC_COLUMN nColumn)
	{
		// weed out unsupported columns
		switch (nColumn)
		{
		case TDCC_COLOR:
			return FALSE;
		}

		// else
		return ((nColumn >= TDCC_FIRST) && (nColumn < TDCC_CUSTOMCOLUMN_FIRST));
	}

protected:
	CTDCColumnIDMap mapVisibleColumns;
	CTDCAttributeMap mapVisibleEdits;
	CTDCAttributeMap mapVisibleFilters;
	TDL_SHOWATTRIB nShowEditsAndFilters;

protected:
	int UpdateEditAndFilterVisibility()
	{
		if (nShowEditsAndFilters != TDLSA_ANY)
		{
			mapVisibleEdits.RemoveAll();
			
			for (int nEdit = TDCA_FIRSTATTRIBUTE; nEdit < TDCA_ATTRIBUTECOUNT; nEdit++)
			{
				if (IsEditFieldVisible((TDC_ATTRIBUTE)nEdit))
					mapVisibleEdits.AddAttribute((TDC_ATTRIBUTE)nEdit);
			}

			mapVisibleFilters.RemoveAll();
			
			for (int nFilter = TDCA_FIRSTATTRIBUTE; nFilter < TDCA_ATTRIBUTECOUNT; nFilter++)
			{
				if (IsFilterFieldVisible((TDC_ATTRIBUTE)nFilter))
					mapVisibleFilters.AddAttribute((TDC_ATTRIBUTE)nFilter);
			}
		}
		
		return (mapVisibleEdits.GetCount() || mapVisibleFilters.GetCount());
	}
	
	static int GetAllEditFields(CTDCAttributeMap& mapAttrib)
	{
		mapAttrib.RemoveAll();
		
		for (int nAttrib = TDCA_FIRSTATTRIBUTE; nAttrib < TDCA_ATTRIBUTECOUNT; nAttrib++)
		{
			if (IsSupportedEdit((TDC_ATTRIBUTE)nAttrib))
				mapAttrib.AddAttribute((TDC_ATTRIBUTE)nAttrib);
		}
		
		return mapAttrib.GetCount();
	}
	
	static int GetAllFilterFields(CTDCAttributeMap& mapAttrib)
	{
		mapAttrib.RemoveAll();
		
		for (int nAttrib = TDCA_FIRSTATTRIBUTE; nAttrib < TDCA_ATTRIBUTECOUNT; nAttrib++)
		{
			if (IsSupportedFilter((TDC_ATTRIBUTE)nAttrib))
				mapAttrib.AddAttribute((TDC_ATTRIBUTE)nAttrib);
		}
		
		return mapAttrib.GetCount();
	}
};

/////////////////////////////////////////////////////////////////////////////

struct TDCATTRIBUTEMAPPING
{
	TDCATTRIBUTEMAPPING() : nTDCAttrib(TDCA_NONE) {}
	TDCATTRIBUTEMAPPING(const CString& sName, TDC_ATTRIBUTE tdcAttrib, DWORD dwData = 0) 
	{ 
		sColumnName = sName; 
		nTDCAttrib = tdcAttrib;
		dwItemData = dwData;
	}

	TDCATTRIBUTEMAPPING(UINT nNameID, TDC_ATTRIBUTE tdcAttrib, DWORD dwData = 0) 
	{ 
		sColumnName.LoadString(nNameID); 
		nTDCAttrib = tdcAttrib;
		dwItemData = dwData;
	}

	TDC_ATTRIBUTE nTDCAttrib;
	CEnString sColumnName;
	DWORD dwItemData;
};

/////////////////////////////////////////////////////////////////////////////

class CTDCAttributeMapping : public CArray<TDCATTRIBUTEMAPPING, TDCATTRIBUTEMAPPING&>
{
public:
	int Find(const CString& sCol) const
	{
		int nMap = GetSize();

		while (nMap--)
		{
			if (GetData()[nMap].sColumnName.CompareNoCase(sCol) == 0)
				return nMap;
		}

		// not found
		return -1;
	}

	int Find(TDC_ATTRIBUTE nAttrib) const
	{
		int nMap = GetSize();

		while (nMap--)
		{
			if (GetData()[nMap].nTDCAttrib == nAttrib)
				return nMap;
		}

		// not found
		return -1;
	}

	int FindMappedAttribute(TDC_ATTRIBUTE nAttrib) const
	{
		int nMap = GetSize();

		while (nMap--)
		{
			const TDCATTRIBUTEMAPPING& col = GetData()[nMap];

			if ((col.nTDCAttrib == nAttrib) && !col.sColumnName.IsEmpty())
				return nMap;
		}

		// not found or mapped
		return -1;
	}

	BOOL CTDCAttributeMapping::IsAttributeMapped(TDC_ATTRIBUTE nAttrib) const
	{
		return (FindMappedAttribute(nAttrib) != -1);
	}

#ifdef _DEBUG
	void Trace() const
	{
		for (int nMap = 0; nMap < GetSize(); nMap++)
		{
			const TDCATTRIBUTEMAPPING& col = GetData()[nMap];

			TRACE(_T("CTDCAttributeMapping::Trace(%s, %d, %d)\n"), col.sColumnName, col.nTDCAttrib, col.dwItemData);
		}
	}
#else
	void Trace() const {}
#endif
};
/////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCSTRUCT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
