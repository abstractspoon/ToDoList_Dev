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
#include "TDCImportExportMgr.h"
#include "tdccustomattribdata.h"
#include "tdccustomattributedef.h"

#include "..\shared\TreeSelectionHelper.h"
#include "..\shared\TreeCtrlHelper.h"
#include "..\shared\misc.h"
#include "..\shared\mapex.h"
#include "..\shared\datehelper.h"
#include "..\shared\encommandlineinfo.h"
#include "..\shared\enstring.h"

#include "..\Interfaces\preferences.h"

//////////////////////////////////////////////////////////////////////

struct TDCDROPIMPORT
{
	TDCDROPIMPORT(DWORD dwID, const CStringArray& sDropFiles) : dwTaskID(dwID)
	{
		aFiles.Copy(sDropFiles);
	}

	TDCDROPIMPORT(DWORD dwID, const CString& sDropText) : dwTaskID(dwID), sText(sDropText)
	{
	}

	DWORD dwTaskID;
	CStringArray aFiles;
	CString sText;
};

//////////////////////////////////////////////////////////////////////

struct TDCDATETIMEWIDTHS
{
	TDCDATETIMEWIDTHS() : bIsoFormat(FALSE)
	{
		ResetWidths();
	}

	void SetIsoFormat(BOOL bIso)
	{
		if ((bIso && bIsoFormat) || (!bIso && !bIsoFormat))
			return;

		bIsoFormat = bIso;

		ResetWidths();
	}

	void ResetWidths()
	{
		nMaxDateWidth = nMinDateWidth = nMaxTimeWidth = nMaxDowNameWidth = -1;
	}

	void Initialise(CDC* pDC)
	{
		if (nMaxDateWidth != -1)
		{
			// Sanity check
			ASSERT(nMaxDateWidth > 0);
			ASSERT(nMinDateWidth > 0);
			ASSERT(nMaxTimeWidth > 0);
			ASSERT(nMaxDowNameWidth > 0);

			return;
		}

		// Sanity check
		ASSERT(nMinDateWidth == -1);
		ASSERT(nMaxTimeWidth == -1);
		ASSERT(nMaxDowNameWidth == -1);
		
		COleDateTime dtMax(2000, 12, 31, 0, 0, 0);
		DWORD dwDateFmt = (bIsoFormat ? DHFD_ISO : 0);

		CString sMaxDate = CDateHelper::FormatDate(dtMax, dwDateFmt);
		nMaxDateWidth = pDC->GetTextExtent(sMaxDate).cx;

		CString sMinDate = CDateHelper::FormatDate(dtMax, (dwDateFmt | DHFD_NOYEAR));
		nMinDateWidth = pDC->GetTextExtent(sMinDate).cx;

		CString sMaxTime = CTimeHelper::FormatClockTime(23, 59, 0, FALSE, bIsoFormat);
		nMaxTimeWidth = pDC->GetTextExtent(sMaxTime).cx;

		nMaxDowNameWidth = CDateHelper::GetMaxDayOfWeekNameWidth(pDC, TRUE);
	}

	int nMaxDateWidth;
	int nMinDateWidth;		// No 'year'
	int nMaxTimeWidth;		// No 'seconds'
	int nMaxDowNameWidth;	// Short format

protected:
	BOOL bIsoFormat;
};

//////////////////////////////////////////////////////////////////////

struct TDCINFOTIPITEM
{
	TDCINFOTIPITEM() // for CArray
	{
		Init(TDCA_NONE, _T(""), _T(""));
	}

	TDCINFOTIPITEM(TDC_ATTRIBUTE nAttribID, CString sLab, const CString& sVal)
	{
		Init(nAttribID, sLab, sVal);
	}

	TDCINFOTIPITEM(TDC_ATTRIBUTE nAttribID, UINT nLabelStrID, const CString& sVal)
	{
		Init(nAttribID, CEnString(nLabelStrID), sVal);
	}

	TDC_ATTRIBUTE nAttributeID;
	CString sLabel;
	CString sValue;
	int nLabelWidth;

protected:
	void Init(TDC_ATTRIBUTE nAttribID, CString sLab, const CString& sVal)
	{
		nAttributeID = nAttribID;
		sLabel = sLab;
		sValue = sVal;
		nLabelWidth = 0;
	}
};

typedef CArray<TDCINFOTIPITEM, TDCINFOTIPITEM&> CTDCInfoTipItemArray;

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

class CTDCStyleMap : public CMap<TDC_STYLE, TDC_STYLE, BOOL, BOOL&>
{
public:
	CTDCStyleMap()
	{
	}

	CTDCStyleMap(TDC_STYLE nStyle, BOOL bEnabled) 
	{
		ASSERT(IsValidStyle(nStyle));

		SetAt(nStyle, bEnabled);
	}

	CTDCStyleMap(const CTDCStyleMap& mapStyles) 
	{
		Misc::CopyT<TDC_STYLE, BOOL>(mapStyles, *this);
	}

	virtual ~CTDCStyleMap() 
	{
	}

	BOOL IsStyleEnabled(TDC_STYLE nStyle) const
	{
		ASSERT(IsValidStyle(nStyle));

		BOOL bEnabled = FALSE;
		
		return (Lookup(nStyle, bEnabled) && bEnabled);
	}

	BOOL HasStyle(TDC_STYLE nStyle) const
	{
		ASSERT(IsValidStyle(nStyle));

		BOOL bUnused;
		
		return Lookup(nStyle, bUnused);
	}

	BOOL& operator[](TDC_STYLE nStyle)
	{
		ASSERT(IsValidStyle(nStyle));

		return CMap<TDC_STYLE, TDC_STYLE, BOOL, BOOL&>::operator[](nStyle);
	}

	TDC_STYLE GetNext(POSITION& pos) const
	{
		if (pos == NULL)
		{
			ASSERT(0);
			return TDCS_LAST;
		}

		TDC_STYLE nStyle;
		BOOL bUnused;
		GetNextAssoc(pos, nStyle, bUnused);

		return nStyle;
	}
	
protected:
	BOOL IsValidStyle(TDC_STYLE nStyle) const
	{
		return ((nStyle >= TDCS_FIRST) && (nStyle < TDCS_LAST));
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCITEMCOLORS
{
	TDCITEMCOLORS() : dwItem(0), crBack(CLR_NONE), crText(CLR_NONE) {}
	
	DWORD dwItem;		// in
	COLORREF crBack;	// out
	COLORREF crText;	// out
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCNOTIFYMOD
{
	TDCNOTIFYMOD(TDC_ATTRIBUTE nAttribID) : mapAttrib(nAttribID) {}
	TDCNOTIFYMOD(TDC_ATTRIBUTE nAttribID, const CDWordArray& taskIDs) : mapAttrib(nAttribID) 
	{
		aTaskIDs.Copy(taskIDs);
	}

	TDCNOTIFYMOD(const CTDCAttributeMap& attribIDs, const CDWordArray& taskIDs) : mapAttrib(attribIDs)
	{
		aTaskIDs.Copy(taskIDs);
	}
	
	CTDCAttributeMap mapAttrib;
	CDWordArray aTaskIDs;
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
	TDCAUTOLISTDATA() {}
	TDCAUTOLISTDATA(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID) 
	{
		Copy(other, nAttribID);
	}

	int Copy(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID)
	{
		return Copy(other, *this, FALSE, nAttribID);
	}

	int AppendUnique(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID)
	{
		return Copy(other, *this, TRUE, nAttribID);
	}

	int RemoveItems(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID)
	{
		int nNumRemoved = 0;

		if (WantAttribute(nAttribID, TDCA_CATEGORY))
			nNumRemoved += Misc::RemoveItems(other.aCategory, aCategory);

		if (WantAttribute(nAttribID, TDCA_STATUS))
			nNumRemoved += Misc::RemoveItems(other.aStatus, aStatus);

		if (WantAttribute(nAttribID, TDCA_ALLOCTO))
			nNumRemoved += Misc::RemoveItems(other.aAllocTo, aAllocTo);

		if (WantAttribute(nAttribID, TDCA_ALLOCBY))
			nNumRemoved += Misc::RemoveItems(other.aAllocBy, aAllocBy);

		if (WantAttribute(nAttribID, TDCA_TAGS))
			nNumRemoved += Misc::RemoveItems(other.aTags, aTags);

		if (WantAttribute(nAttribID, TDCA_VERSION))
			nNumRemoved += Misc::RemoveItems(other.aVersion, aVersion);

		return nNumRemoved;
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

	void RemoveAll(TDC_ATTRIBUTE nAttribID)
	{
		if (WantAttribute(nAttribID, TDCA_CATEGORY))
			aCategory.RemoveAll();

		if (WantAttribute(nAttribID, TDCA_STATUS))
			aStatus.RemoveAll();

		if (WantAttribute(nAttribID, TDCA_ALLOCTO))
			aAllocTo.RemoveAll();

		if (WantAttribute(nAttribID, TDCA_ALLOCBY))
			aAllocBy.RemoveAll();

		if (WantAttribute(nAttribID, TDCA_TAGS))
			aTags.RemoveAll();

		if (WantAttribute(nAttribID, TDCA_VERSION))
			aVersion.RemoveAll();
	}

	CString Format(TDC_ATTRIBUTE nAttribID, TCHAR cSep = 0)
	{
		switch (nAttribID)
		{
		case TDCA_ALLOCBY:	return Misc::FormatArray(aAllocBy, cSep);
		case TDCA_ALLOCTO:	return Misc::FormatArray(aAllocTo, cSep);
		case TDCA_CATEGORY:	return Misc::FormatArray(aCategory, cSep);
		case TDCA_STATUS:	return Misc::FormatArray(aStatus, cSep);
		case TDCA_TAGS:		return Misc::FormatArray(aTags, cSep);
		case TDCA_VERSION:	return Misc::FormatArray(aVersion, cSep);
		}

		ASSERT(0);
		return _T("");
	}

	CStringArray aCategory, aStatus, aAllocTo, aAllocBy, aTags, aVersion;

protected:
	BOOL WantAttribute(TDC_ATTRIBUTE nAttribID, TDC_ATTRIBUTE nWantedAttribID) const
	{
		return ((nAttribID == TDCA_ALL) || (nAttribID == nWantedAttribID));
	}

	int Copy(const TDCAUTOLISTDATA& from, TDCAUTOLISTDATA& to, BOOL bAppend, TDC_ATTRIBUTE nAttribID)
	{
		int nNumCopied = 0;

		if (WantAttribute(nAttribID, TDCA_CATEGORY))
			nNumCopied += CopyItems(from.aCategory, to.aCategory, bAppend);

		if (WantAttribute(nAttribID, TDCA_STATUS))
			nNumCopied += CopyItems(from.aStatus, to.aStatus, bAppend);

		if (WantAttribute(nAttribID, TDCA_ALLOCTO))
			nNumCopied += CopyItems(from.aAllocTo, to.aAllocTo, bAppend);

		if (WantAttribute(nAttribID, TDCA_ALLOCBY))
			nNumCopied += CopyItems(from.aAllocBy, to.aAllocBy, bAppend);

		if (WantAttribute(nAttribID, TDCA_TAGS))
			nNumCopied += CopyItems(from.aTags, to.aTags, bAppend);

		if (WantAttribute(nAttribID, TDCA_VERSION))
			nNumCopied += CopyItems(from.aVersion, to.aVersion, bAppend);

		return nNumCopied;
	}

	int CopyItems(const CStringArray& aFrom, CStringArray& aTo, BOOL bAppend)
	{
		if (bAppend)
			return Misc::AddUniqueItems(aFrom, aTo);

		// else
		aTo.Copy(aFrom);
		return aTo.GetSize();
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCEXPORTTASKLIST
{
	TDCEXPORTTASKLIST(HWND hwnd, const CString& sTDC, int nExp) 
		: 
		hWndNotify(hwnd),
		sTDCPath(sTDC), 
		nExporter(nExp),
		pImpExpMgr(NULL),
		nPurpose(TDCTEP_NONE)
	{
	}

	BOOL IsValid() const
	{
		// Note: sTDCPath can be empty

		if (!::IsWindow(hWndNotify))
			return FALSE;

		if (!tasks.GetTaskCount())
			return FALSE;

		if (!pImpExpMgr)
			return FALSE;

		if ((nExporter < 0) || (nExporter > pImpExpMgr->GetNumExporters()))
			return FALSE;

		if (sExportPath.IsEmpty())
			return FALSE;

		if (nPurpose == TDCTEP_NONE)
			return FALSE;

		return TRUE;
	}

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

		mapAttribs.Copy(filter.mapAttribs);

		return *this;
	}

	BOOL WantAttribute(TDC_ATTRIBUTE nAttribID) const
	{
		if (mapAttribs.Has(TDCA_NONE))
			return FALSE;

		// Html comments cannot be implicitly specified because of its cost
		if (nAttribID != TDCA_HTMLCOMMENTS)
		{
			if ((mapAttribs.GetCount() == 0) || mapAttribs.Has(TDCA_ALL))
				return TRUE;
		}

		return mapAttribs.Has(nAttribID);
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

	BOOL HasFlag(DWORD dwFlag) const 
	{ 
		// Dependent flags
		if ((dwFlag == TDCGSTF_IMMEDIATEPARENT) && HasFlag(TDCGSTF_ALLPARENTS))
			return TRUE;

		return Misc::HasFlag(dwFlags, dwFlag); 
	}

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
		return (Misc::MatchAllT(aSelTaskIDs, cache.aSelTaskIDs, FALSE) &&
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
		ASSERT(!dwFocusedTaskID);
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
	TDC_COLUMN nColumnID;
	UINT nIDName;
	UINT nIDLongName;
	UINT nTextAlignment;
	BOOL bSortAscending;
	TDCC_IMAGE iImage, iAlternateImage;

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
typedef CMap<TDC_COLUMN, TDC_COLUMN, const TDCCOLUMN*, const TDCCOLUMN*&> CTDCColumnMap;

/////////////////////////////////////////////////////////////////////////////////////////////

struct CTRLITEM
{
	CTRLITEM() 
		: 
		nCtrlID(0), 
		nLabelID(0), 
		nAttributeID(TDCA_NONE)
	{
	}
	
	CTRLITEM(UINT ctrlID, UINT labelID, TDC_ATTRIBUTE nAttribID)	
		: 
		nCtrlID(ctrlID), 
		nLabelID(labelID), 
		nAttributeID(nAttribID)
	{
	}

	BOOL operator==(const CTRLITEM& other) const
	{
		return ((nCtrlID == other.nCtrlID) && 
				(nLabelID == other.nLabelID) && 
				(nAttributeID == other.nAttributeID));
	}

	BOOL operator!=(const CTRLITEM& other) const
	{
		return !(*this == other);
	}

	CWnd* GetCtrl(const CWnd* pParent) const
	{
		return pParent->GetDlgItem(nCtrlID);
	}

	CWnd* GetLabel(const CWnd* pParent) const
	{
		return pParent->GetDlgItem(nLabelID);
	}

	void DeleteCtrls(const CWnd* pParent)
	{
		DeleteCtrl(pParent, nCtrlID);
		DeleteCtrl(pParent, nLabelID);
	}

	UINT nCtrlID;
	UINT nLabelID;
	TDC_ATTRIBUTE nAttributeID;

protected:
	void DeleteCtrl(const CWnd* pParent, UINT& nID)
	{
		CWnd* pCtrl = pParent->GetDlgItem(nID);

		if (pCtrl)
		{
			pCtrl->DestroyWindow();
			delete pCtrl;
		}

		nID = 0;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct CUSTOMATTRIBCTRLITEM : public CTRLITEM
{
	friend class CTDCCustomControlArray;

	CUSTOMATTRIBCTRLITEM()
	{
		nBuddyCtrlID = nBuddyLabelID = 0;
		pBuddyClass = NULL;
	}

	BOOL operator==(const CUSTOMATTRIBCTRLITEM& other) const
	{
		if (!CTRLITEM::operator==(other) || 
			(nBuddyLabelID != other.nBuddyLabelID) ||
			(nBuddyCtrlID != other.nBuddyCtrlID) ||
			(sAttribID != other.sAttribID))
		{
			return FALSE;
		}

		if (!(pBuddyClass && other.pBuddyClass))
		{
			return FALSE;
		}

		// Class name is an ANSI string
		return (strcmp(pBuddyClass->m_lpszClassName, other.pBuddyClass->m_lpszClassName) == 0);
	}
		
	BOOL operator!=(const CUSTOMATTRIBCTRLITEM& other) const
	{
		return !(*this == other);
	}

	void ShowBuddy(const CWnd* pParent, BOOL bShow = TRUE)
	{
		CWnd* pBuddy = GetBuddy(pParent);

		if (!pBuddy)
		{
			pBuddyClass = NULL;
			ASSERT(0);
		}
		else
		{
			pBuddy->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
			pBuddy->EnableWindow(bShow);

			pBuddyClass = (bShow ? pBuddy->GetRuntimeClass() : NULL);

			CWnd* pLabel = GetBuddyLabel(pParent);
			ASSERT_VALID(pLabel);

			if (pLabel)
			{
				pLabel->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
				pLabel->EnableWindow(bShow);
			}
		}
	}

	BOOL IsShowingBuddy() const
	{
		return (HasBuddy() && pBuddyClass);
	}

	BOOL HasBuddy() const
	{
		return (nBuddyCtrlID && nBuddyLabelID);
	}

	BOOL GetBuddy(CTRLITEM& ctrl) const
	{
		if (!HasBuddy())
			return FALSE;

		ctrl.nAttributeID = nAttributeID;
		ctrl.nCtrlID = nBuddyCtrlID;
		ctrl.nLabelID = nBuddyLabelID;

		return TRUE;
	}

	CWnd* GetBuddy(const CWnd* pParent) const
	{
		return pParent->GetDlgItem(nBuddyCtrlID);
	}

	CWnd* GetBuddyLabel(const CWnd* pParent) const
	{
		return pParent->GetDlgItem(nBuddyLabelID);
	}

	void DeleteCtrls(const CWnd* pParent)
	{
		CTRLITEM::DeleteCtrls(pParent);

		DeleteBuddy(pParent);
	}

	CString sAttribID;
	UINT nBuddyCtrlID;
	UINT nBuddyLabelID;
	CString sPrompt, sBuddyPrompt;

protected:
	CRuntimeClass* pBuddyClass;

	void DeleteBuddy(const CWnd* pParent)
	{
		DeleteCtrl(pParent, nBuddyCtrlID);
		DeleteCtrl(pParent, nBuddyLabelID);
	}

};

class CTDCControlArray : public CArray<CTRLITEM, CTRLITEM&> {};

/////////////////////////////////////////////////////////////////////////////////////////////

class CTDCCustomControlArray : public CArray<CUSTOMATTRIBCTRLITEM, CUSTOMATTRIBCTRLITEM&> 
{
public:
	void DeleteCtrls(const CWnd* pParent)
	{
		for (int nCtrl = 0; nCtrl < GetSize(); nCtrl++)
		{
			GetAt(nCtrl).DeleteCtrls(pParent);
		}
	}

	int Find(UINT nCtrlID, BOOL bIncludeLabels = FALSE) const
	{
		int nCtrl = GetSize();

		while (nCtrl--)
		{
			const CUSTOMATTRIBCTRLITEM& ctrl = GetAt(nCtrl);

			if ((ctrl.nCtrlID == nCtrlID) || (ctrl.nBuddyCtrlID == nCtrlID))
				return nCtrl;

			if (bIncludeLabels)
			{
				if ((ctrl.nLabelID == nCtrlID) || (ctrl.nBuddyLabelID == nCtrlID))
					return nCtrl;
			}
		}

		// not found
		return -1;
	}

	int Find(TDC_ATTRIBUTE nAttribID) const
	{
		int nCtrl = GetSize();

		while (nCtrl--)
		{
			if (GetAt(nCtrl).nAttributeID == nAttribID)
				return nCtrl;
		}

		// not found
		return -1;
	}

	int Find(const CString& sAttribID) const
	{
		int nCtrl = GetSize();

		while (nCtrl--)
		{
			if (GetAt(nCtrl).sAttribID == sAttribID)
				return nCtrl;
		}

		// not found
		return -1;
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

	SEARCHPARAM(TDC_ATTRIBUTE nAttribID = TDCA_NONE, FIND_OPERATOR nOp = FOP_NONE) 
		: 
		nAttributeID(TDCA_NONE), 
		nOperator(nOp), 
		dValue(0), 
		nValue(0), 
		dwFlags(0), 
		bAnd(TRUE), 
		nAttribType(FT_NONE)
	{
		SetAttribute(nAttribID);
	}
	
	SEARCHPARAM(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, CString sVal, BOOL and = TRUE, FIND_ATTRIBTYPE nType = FT_NONE) 
		: 
		nAttributeID(TDCA_NONE), 
		nOperator(nOp), 
		dValue(0), 
		nValue(0), 
		dwFlags(0), 
		bAnd(and), 
		nAttribType(nType)  
	{
		SetAttribute(nAttribID);

		if (!sVal.IsEmpty())
			SetValue(sVal);
	}
	
	SEARCHPARAM(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, double dVal, BOOL and = TRUE) 
		: 
		nAttributeID(TDCA_NONE), 
		nOperator(nOp), 
		dValue(0), 
		nValue(0), 
		dwFlags(0), 
		bAnd(and), 
		nAttribType(FT_NONE)  
	{
		SetAttribute(nAttribID);

		nAttribType = GetAttribType();

		switch (nAttribType)
		{
		case FT_DOUBLE:
		case FT_DATE:
			dValue = dVal;
			break;

		default:
			ASSERT(0);
		}
	}
	
	SEARCHPARAM(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, int nVal, BOOL and = TRUE) 
		: 
		nAttributeID(TDCA_NONE), 
		nOperator(nOp), 
		dValue(0), 
		nValue(0), 
		dwFlags(0), 
		bAnd(and), 
		nAttribType(FT_NONE)  
	{
		SetAttribute(nAttribID);

		nAttribType = GetAttribType();

		switch (nAttribType)
		{
		case FT_INTEGER:
		case FT_BOOL:
		case FT_RECURRENCE:
			nValue = nVal;
			break;

		default:
			ASSERT(0);
		}
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
		else if (nAttributeID != rule.nAttributeID)
		{
			return FALSE;
		}

		// test rest of attributes
		if ((nOperator == rule.nOperator) && (bAnd == rule.bAnd) && (dwFlags == rule.dwFlags))
		{
			switch (GetAttribType())
			{
			case FT_BOOL:
			case FT_DEPENDENCY:
				return TRUE; // handled by operator

			case FT_DATE:
			case FT_DOUBLE:
			case FT_TIMEPERIOD:
				return (dValue == rule.dValue);

			case FT_INTEGER:
			case FT_RECURRENCE:
				return (nValue == rule.nValue);

			case FT_STRING:
			case FT_DATERELATIVE:
			case FT_ICON:
				return (sValue == rule.sValue);
			}
		}

		// all else
		return FALSE;
	}

	BOOL Set(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, CString sVal, BOOL and = TRUE, FIND_ATTRIBTYPE nType = FT_NONE)
	{
		if (!SetAttribute(nAttribID, nType))
			return FALSE;

		SetOperator(nOp);
		SetAnd(and);
		SetValue(sVal);

		return TRUE;
	}

	BOOL Set(TDC_ATTRIBUTE nAttribID, const CString& sID, FIND_ATTRIBTYPE nType, FIND_OPERATOR nOp, CString sVal, BOOL and = TRUE)
	{
		if (!SetCustomAttribute(nAttribID, sID, nType))
			return FALSE;

		SetOperator(nOp);
		SetAnd(and);
		SetValue(sVal);

		return TRUE;
	}

	BOOL SetAttribute(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType = FT_NONE)
	{
		// custom attributes must have a custom ID
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			ASSERT(0);
			return FALSE;
		}

		FIND_ATTRIBTYPE nPrevType = nAttribType;

		nAttributeID = nAttribID;
		nAttribType = nType;
		
		// update Attrib type
		GetAttribType();

		// Reset 'extra' information stored in the union
		if (nAttribType != nPrevType)
		{
			if (nAttribType == FT_DATERELATIVE)
				bRelativeDate = TRUE;
			else
				dwFlags = 0;
		}

		ValidateOperator();
		return TRUE;
	}

	TDC_ATTRIBUTE GetAttribute() const { return nAttributeID; }
	FIND_OPERATOR GetOperator() const { return nOperator; }
	BOOL GetAnd() const { return bAnd; }
	BOOL GetOr() const { return !bAnd; }
	BOOL IsCustomAttribute() const { return TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttributeID); }

	BOOL IsRelativeDate(BOOL bMustHaveValue = TRUE) const
	{
		if (GetAttribType() != FT_DATERELATIVE)
			return FALSE;

		if (sValue.IsEmpty() && !bMustHaveValue)
			return TRUE;

		return CDateHelper::IsValidRelativeDate(sValue, FALSE);
	}

	BOOL IsNowRelativeDate() const
	{
		if (!IsRelativeDate())
			return FALSE;

		return ((sValue[0] == 'n') || (sValue[0] == 'N'));
	}

	BOOL SetCustomAttribute(TDC_ATTRIBUTE nAttribID, const CString& sID, FIND_ATTRIBTYPE nType)
	{
		if (!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID) || 
			sID.IsEmpty() || 
			(nType == FT_NONE))
		{
			ASSERT(0);
			return FALSE;
		}

		nAttributeID = nAttribID;
		nAttribType = nType;
		sCustAttribID = sID;

		// handle relative dates
		switch (nAttribType)
		{
		case FT_DATE:
			SetRelativeDate(FALSE);
			break;

		case FT_DATERELATIVE:
			SetRelativeDate(TRUE);
			break;
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

	BOOL SetOperator(FIND_OPERATOR nOp) 
	{
		if (IsValidOperator(GetAttribType(), nOp))
		{
			nOperator = nOp;
			return TRUE;
		}

		return FALSE;
	}

	FIND_ATTRIBTYPE GetAttribType() const
	{
		if (nAttribType == FT_NONE)
			nAttribType = GetAttribType(nAttributeID, bRelativeDate);

		ASSERT ((nAttribType != FT_NONE) || 
				(nAttributeID == TDCA_NONE) || 
				(nAttributeID == TDCA_SELECTION));

		return nAttribType;
	}

	void ClearValue()
	{
		sValue.Empty();
		dValue = 0.0;
		nValue = 0;
	}

	static FIND_ATTRIBTYPE GetAttribType(TDC_ATTRIBUTE nAttribID, BOOL bRelativeDate)
	{
		switch (nAttribID)
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
		case TDCA_FILELINK:
		case TDCA_PROJECTNAME:
		case TDCA_CREATEDBY:
		case TDCA_EXTERNALID: 
		case TDCA_TAGS: 
		case TDCA_PATH: 
		case TDCA_LASTMODBY:
		case TDCA_COMMENTSFORMAT:
			return FT_STRING;

		case TDCA_ICON: 
			return FT_ICON;

		case TDCA_DEPENDENCY: 
			return FT_DEPENDENCY;

		case TDCA_PRIORITY:
		case TDCA_COLOR:
		case TDCA_PERCENT:
		case TDCA_RISK:
		case TDCA_ID:
		case TDCA_PARENTID:
		case TDCA_POSITION:
			return FT_INTEGER;

		case TDCA_RECURRENCE: 
			return FT_RECURRENCE;

		case TDCA_TIMEESTIMATE:
		case TDCA_TIMESPENT:
			return FT_TIMEPERIOD;

		case TDCA_COST:
		case TDCA_SUBTASKDONE:
		case TDCA_COMMENTSSIZE:
			return FT_DOUBLE;

		case TDCA_FLAG:
		case TDCA_LOCK:
		case TDCA_RECENTMODIFIED:
		case TDCA_REMINDER:
			return FT_BOOL;

		case TDCA_STARTDATE:
		case TDCA_STARTTIME:
		case TDCA_CREATIONDATE:
		case TDCA_DONEDATE:
		case TDCA_DUEDATE:
		case TDCA_DUETIME:
		case TDCA_LASTMODDATE:
			return (bRelativeDate ? FT_DATERELATIVE : FT_DATE);
		}

		// custom attribute type must be set explicitly by caller
		return FT_NONE;
	}

	static BOOL IsValidOperator(FIND_ATTRIBTYPE nType, FIND_OPERATOR nOp)
	{
		switch (nOp)
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
			return (nType == FT_DATE || nType == FT_DATERELATIVE);

		case FOP_GREATER_OR_EQUAL:
		case FOP_GREATER:
		case FOP_LESS_OR_EQUAL:
		case FOP_LESS:
			return (nType == FT_INTEGER || nType == FT_DOUBLE || nType == FT_TIMEPERIOD);

		case FOP_IS_COMPLETE:
			return (nType == FT_DEPENDENCY);

		case FOP_SET:
		case FOP_NOT_SET:
			return TRUE;
		}

		return FALSE;
	}

	BOOL HasValidOperator() const
	{
		return IsValidOperator(GetAttribType(), nOperator);
	}

	void ValidateOperator()
	{
		if (!HasValidOperator())
			nOperator = FOP_NONE;
	}

	BOOL OperatorIs(FIND_OPERATOR nOp) const
	{
		return (nOperator == nOp);
	}

	BOOL AttributeIs(TDC_ATTRIBUTE nAttribID) const
	{
		return (nAttributeID == nAttribID);
	}

	BOOL TypeIs(FIND_ATTRIBTYPE nType) const
	{
		return (GetAttribType() == nType);
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

	void SetTimeUnits(TDC_UNITS nUnits)
	{
		ASSERT(TypeIs(FT_TIMEPERIOD));

		nTimeUnits = nUnits;
	}

	void SetMatchWholeWord(BOOL bMatchWhole)
	{
		ASSERT(TypeIs(FT_STRING));

		bMatchWholeWord = bMatchWhole;
	}

	void SetFlags(DWORD flags)
	{
		dwFlags = flags;
	}

	void SetRelativeDate(BOOL bRelative)
	{
		ASSERT(TypeIs(FT_DATE) || TypeIs(FT_DATERELATIVE));

		bRelativeDate = bRelative;
	}

	DWORD GetFlags() const
	{
		return dwFlags;
	}

	TDC_UNITS GetTimeUnits() const
	{
		ASSERT(TypeIs(FT_TIMEPERIOD));

		return nTimeUnits;
	}

	BOOL GetMatchWholeWord() const
	{
		ASSERT(TypeIs(FT_STRING));

		return bMatchWholeWord;
	}

	BOOL GetRelativeDate() const
	{
		ASSERT(TypeIs(FT_DATE) || TypeIs(FT_DATERELATIVE));

		return bRelativeDate;
	}

	void SetValue(const CString& sVal)
	{
		switch (GetAttribType())
		{
		case FT_STRING:
		case FT_DATERELATIVE:
		case FT_ICON:
		case FT_DEPENDENCY:
			sValue = sVal;
			break;
			
		case FT_DATE:
		case FT_DOUBLE:
		case FT_TIMEPERIOD:
			dValue = _ttof(sVal);
			break;
			
		case FT_INTEGER:
		case FT_BOOL:
		case FT_RECURRENCE:
			nValue = _ttoi(sVal);
			break;

		default:
			ASSERT(0);
			break;
		}
	}

	void SetValue(double dVal)
	{
		switch (GetAttribType())
		{
		case FT_DATE:
		case FT_DOUBLE:
		case FT_TIMEPERIOD:
			dValue = dVal;
			break;
			
		default:
			ASSERT(0);
			break;
		}
	}

	void SetValue(int nVal)
	{
		switch (GetAttribType())
		{
		case FT_INTEGER:
		case FT_BOOL:
		case FT_RECURRENCE:
			nValue = nVal;
			break;

		default:
			ASSERT(0);
			break;
		}
	}

	void SetValue(const COleDateTime& dtVal)
	{
		switch (GetAttribType())
		{
		case FT_DATE:
			dValue = dtVal;
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
		case FT_TIMEPERIOD:
			return Misc::Format(dValue, 3);

		case FT_INTEGER:
			switch (GetAttribute())
			{
			case TDCA_PRIORITY:
				if (nValue == FM_NOPRIORITY)
					return CEnString(IDS_TDC_NONE);
				break;

			case TDCA_RISK:
				if (nValue == FM_NORISK)
					return CEnString(IDS_TDC_NONE);
			}
			// else fall thru

		case FT_BOOL:
		case FT_RECURRENCE:
			return Misc::Format(nValue);

		case FT_STRING:
		case FT_DATERELATIVE:
		case FT_ICON:
		case FT_DEPENDENCY:
			return sValue;
		}

		// all else
		ASSERT(0);
		return _T("");
	}

	double ValueAsDouble() const
	{
		switch (GetAttribType())
		{
		case FT_DATE:
		case FT_TIMEPERIOD:
		case FT_DOUBLE:
			return dValue;

		case FT_DATERELATIVE:
			return ValueAsDate().m_dt;

		case FT_INTEGER:
		case FT_BOOL:
		case FT_RECURRENCE:
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
		case FT_TIMEPERIOD:
		case FT_DOUBLE:
			return (int)dValue;
		
		case FT_DATERELATIVE:
			return (int)ValueAsDouble();

		case FT_INTEGER:
		case FT_BOOL:
		case FT_RECURRENCE:
			return nValue;
		}

		// all else
		ASSERT(0);
		return 0;
	}

	COleDateTime ValueAsDate() const
	{
		switch (GetAttribType())
		{
		case FT_DATE:
			return dValue;

		case FT_DATERELATIVE:
			{
				CTwentyFourSevenWeek week;
				CDateHelper dh(week);
				COleDateTime date;

				if (!dh.DecodeRelativeDate(sValue, date, FALSE))
					CDateHelper::ClearDate(date);

				return date;
			}
			break;
		}

		// all else
		ASSERT(0);
		return CDateHelper::NullDate();
	}

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

typedef CArray<SEARCHPARAM, SEARCHPARAM&> CSearchParamArray;

// ------------------------------------------------------------------------

struct SEARCHPARAMS
{
	SEARCHPARAMS()
	{
		Clear();
	}

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
		bCaseSensitive = params.bCaseSensitive;

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
		return (Misc::MatchAllT(aRules, params.aRules, TRUE) && 
				Misc::MatchAllT(aAttribDefs, params.aAttribDefs, FALSE) &&
				(bIgnoreDone == params.bIgnoreDone) && 
				(bIgnoreOverDue == params.bIgnoreOverDue) && 
				(bIgnoreFilteredOut == params.bIgnoreFilteredOut) && 
				(bWantAllSubtasks == params.bWantAllSubtasks) &&
				(bCaseSensitive == params.bCaseSensitive));
	}

	void Clear()
	{
		bIgnoreDone = FALSE;
		bIgnoreOverDue = FALSE;
		bWantAllSubtasks = FALSE;
		bIgnoreFilteredOut = TRUE;
		bCaseSensitive = FALSE;
		
		aRules.RemoveAll();
		aAttribDefs.RemoveAll();
	}

	BOOL HasAttribute(TDC_ATTRIBUTE nAttribID) const
	{
		InitAttributeMap();

		switch (nAttribID)
		{
		case TDCA_TASKNAME:
		case TDCA_COMMENTS:
			return (mapAttrib.Has(nAttribID) || 
					mapAttrib.Has(TDCA_TASKNAMEORCOMMENTS) ||
					mapAttrib.Has(TDCA_ANYTEXTATTRIBUTE));

		case TDCA_STATUS:
		case TDCA_CATEGORY:
		case TDCA_ALLOCBY:
		case TDCA_ALLOCTO:
		case TDCA_VERSION:
		case TDCA_TAGS:
		case TDCA_EXTERNALID:
			return (mapAttrib.Has(nAttribID) ||
					mapAttrib.Has(TDCA_ANYTEXTATTRIBUTE));
		}

		// all else
		return mapAttrib.Has(nAttribID);
	}

	BOOL HasMultipleAttributes() const
	{
		int nNumRules = aRules.GetSize();

		if (nNumRules)
		{
			TDC_ATTRIBUTE nFirstAttribID = aRules[0].GetAttribute();

			// quick check for aggregate attributes
			if (nFirstAttribID == TDCA_ANYTEXTATTRIBUTE || (nFirstAttribID == TDCA_TASKNAMEORCOMMENTS))
				return TRUE;

			for (int nRule = 1; nRule < nNumRules; nRule++)
			{
				if (aRules[nRule].GetAttribute() != nFirstAttribID)
					return TRUE;
			}
		}

		return FALSE;
	}

	BOOL GetRuleCount() const
	{
		return aRules.GetSize();
	}

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
	void InitAttributeMap() const
	{
		if (mapAttrib.GetCount() == 0)
		{
			int nRule = aRules.GetSize();

			while (nRule--)
				mapAttrib.Add(aRules[nRule].nAttributeID);
		}
	}
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
		Misc::CopyT<TDC_ATTRIBUTE, CString>(res.mapMatched, mapMatched);
		dwFlags = res.dwFlags;

		return *this;
	}

	BOOL HasFlag(DWORD dwFlag) const 
	{
		return ((dwFlags & dwFlag) == dwFlag) ? 1 : 0;
	}

	BOOL GetWhatMatched(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aAttribDefs, CString& sWhatMatched) const
	{
		// RECURSIVE CALLS
		switch (nAttribID)
		{
		case TDCA_TASKNAMEORCOMMENTS:
			return (GetWhatMatched(TDCA_TASKNAME,	aAttribDefs, sWhatMatched) || 
					GetWhatMatched(TDCA_COMMENTS,	aAttribDefs, sWhatMatched));

		case TDCA_ANYTEXTATTRIBUTE:
			{
				if (GetWhatMatched(TDCA_TASKNAME,	aAttribDefs, sWhatMatched) ||
					GetWhatMatched(TDCA_COMMENTS,	aAttribDefs, sWhatMatched) ||
					GetWhatMatched(TDCA_STATUS,		aAttribDefs, sWhatMatched) ||
					GetWhatMatched(TDCA_CATEGORY,	aAttribDefs, sWhatMatched) ||
					GetWhatMatched(TDCA_ALLOCBY,	aAttribDefs, sWhatMatched) ||
					GetWhatMatched(TDCA_ALLOCTO,	aAttribDefs, sWhatMatched) ||
					GetWhatMatched(TDCA_VERSION,	aAttribDefs, sWhatMatched) ||
					GetWhatMatched(TDCA_TAGS,		aAttribDefs, sWhatMatched) ||
					GetWhatMatched(TDCA_EXTERNALID, aAttribDefs, sWhatMatched))
				{
					return TRUE;
				}

				int nDef = aAttribDefs.GetSize();

				while (nDef--)
				{
					const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nDef];

					if ((attribDef.GetDataType() == TDCCA_STRING) &&
						GetWhatMatched(attribDef.GetAttributeID(), aAttribDefs, sWhatMatched))
					{
						return TRUE;
					}
				}
			}
			return FALSE;
		}

		// All else
		return mapMatched.Lookup(nAttribID, sWhatMatched);
	}
	
	DWORD dwTaskID;
	DWORD dwFlags;
	CMap<TDC_ATTRIBUTE, TDC_ATTRIBUTE, CString, CString&> mapMatched;
};

typedef CArray<SEARCHRESULT, SEARCHRESULT&> CResultArray;

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCFILTER
{
	TDCFILTER() 
		: 
		nShow(FS_ALL), 
		nStartBy(FD_ANY), 
		nDueBy(FD_ANY), 
		nPriority(FM_ANYPRIORITY), 
		nRisk(FM_ANYRISK), 
		dwFlags(FO_ATTRIBUTES), 
		nTitleOption(FT_FILTERONTITLEONLY),
		nStartNextNDays(7), 
		nDueNextNDays(7),
		nRecurrence(TDIR_NONE)
	{
		dtUserStart = dtUserDue = COleDateTime::GetCurrentTime();
	}

	TDCFILTER(const TDCFILTER& filter)
	{
		*this = filter;
	}

	TDCFILTER& operator=(const TDCFILTER& filter)
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
		nRecurrence = filter.nRecurrence;

		aAllocTo.Copy(filter.aAllocTo);
		aStatus.Copy(filter.aStatus);
		aAllocBy.Copy(filter.aAllocBy);
		aCategories.Copy(filter.aCategories);
		aVersions.Copy(filter.aVersions);
		aTags.Copy(filter.aTags);

		mapCustomAttrib.Copy(filter.mapCustomAttrib);

		return *this;
	}

	BOOL operator==(const TDCFILTER& filter) const
	{
		return FiltersMatch(*this, filter);
	}

	BOOL operator!=(const TDCFILTER& filter) const
	{
		return !FiltersMatch(*this, filter);
	}
	
	BOOL IsSet() const
	{
		// Compare to empty filter
		return !FiltersMatch(*this, TDCFILTER());
	}

	BOOL IsAdvanced() const { return (nShow == FS_ADVANCED); }

	BOOL WantHideCompletedTasks() const
	{
		switch (nShow)
		{
		case FS_DONE:
			return FALSE; // Definitely

		case FS_NOTDONE:
			return TRUE; // Definitely

		case FS_ALL:
		case FS_FLAGGED:
		case FS_SELECTED:
		case FS_RECENTMOD:
		case FS_LOCKED:
			{
				if (HasFlag(FO_HIDEDONE))
					return TRUE; // Definitely

				// If the user is filtering by start date and is NOT
				// explicitly showing or hiding completed tasks then 
				// assume they probably don't want them
				if (nStartBy != FD_ANY)
					return TRUE;

				// Likewise for due tasks
				if (nDueBy != FD_ANY)
					return TRUE;
			}
			break;

		default:
			// catch unimplemented filters
			ASSERT(0);
			break;
		}

		return FALSE;
	}

	BOOL HasNowFilter(TDC_ATTRIBUTE& nAttrib) const
	{
		if (nDueBy == FD_NOW)
		{
			nAttrib = TDCA_DUEDATE;
			return TRUE;
		}
		else if (nStartBy == FD_NOW)
		{
			nAttrib = TDCA_STARTDATE;
			return TRUE;
		}

		// else
		return FALSE;
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
			case FO_ANYPERSON:
			case FO_ANYTAG:
			case FO_ANYSTATUS:
			case FO_ANYVERSION:
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

	void Reset(FILTER_SHOW nInit = FS_ALL)
	{
		*this = TDCFILTER(); // empty filter

		nShow = nInit;
	}

	BOOL Matches(const TDCFILTER& filter) const
	{
		return FiltersMatch(*this, filter);
	}

	CString GetTitleFilterLabel() const
	{
		CEnString sLabel; 

		switch (nTitleOption)
		{
		case FT_FILTERONANYTEXT: 
			sLabel.LoadString(IDS_TDLBC_ANYTEXTATTRIB); 
			break;

		case FT_FILTERONTITLECOMMENTS: 
			sLabel.LoadString(IDS_TDLBC_TITLEORCOMMENTS); 	
			break;

		case FT_FILTERONTITLEONLY:
		default:
			sLabel.LoadString(IDS_TDLBC_TITLE);
			break;
		}

		// Add an accelerator
		if (sLabel.Find('&') == -1)
			sLabel.Insert(0, '&');

		return sLabel;
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
	TDC_REGULARITY nRecurrence;

	CTDCCustomAttributeDataMap mapCustomAttrib;

protected:
	static BOOL FiltersMatch(const TDCFILTER& filter1, const TDCFILTER& filter2)
	{
		// simple exclusion test first
		if ((filter1.nShow != filter2.nShow) ||
			(filter1.nStartBy != filter2.nStartBy) ||
			(filter1.nDueBy != filter2.nDueBy) ||
			(filter1.nPriority != filter2.nPriority) ||
			(filter1.nRisk != filter2.nRisk) ||
			(filter1.nRecurrence != filter2.nRecurrence) ||
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

		// 'Global' flags
		if ((filter1.dwFlags & FO_GLOBALS) != (filter2.dwFlags & FO_GLOBALS))
			return FALSE;

		// compare categories disregarding dwFlags if there's not actually something to compare
		if (filter1.aCategories.GetSize() || filter2.aCategories.GetSize())
		{
			if (!Misc::MatchAll(filter1.aCategories, filter2.aCategories))
				return FALSE;

			if (filter1.HasFlag(FO_ANYCATEGORY) != filter2.HasFlag(FO_ANYCATEGORY))
				return FALSE;
		}

		// do the same with Alloc_To
		if (filter1.aAllocTo.GetSize() || filter2.aAllocTo.GetSize())
		{
			if (!Misc::MatchAll(filter1.aAllocTo, filter2.aAllocTo))
				return FALSE;

			if (filter1.HasFlag(FO_ANYPERSON) != filter2.HasFlag(FO_ANYPERSON))
				return FALSE;
		}

		// and the same with Tags
		if (filter1.aTags.GetSize() || filter2.aTags.GetSize())
		{
			if (!Misc::MatchAll(filter1.aTags, filter2.aTags))
				return FALSE;

			if (filter1.HasFlag(FO_ANYTAG) != filter2.HasFlag(FO_ANYTAG))
				return FALSE;
		}

		// and the same with Alloc_By
		if (filter1.aAllocBy.GetSize() || filter2.aAllocBy.GetSize())
		{
			if (!Misc::MatchAll(filter1.aAllocBy, filter2.aAllocBy))
				return FALSE;

			if (filter1.HasFlag(FO_ANYPERSON) != filter2.HasFlag(FO_ANYPERSON))
				return FALSE;
		}

		// and the same with Status
		if (filter1.aStatus.GetSize() || filter2.aStatus.GetSize())
		{
			if (!Misc::MatchAll(filter1.aStatus, filter2.aStatus))
				return FALSE;

			if (filter1.HasFlag(FO_ANYSTATUS) != filter2.HasFlag(FO_ANYSTATUS))
				return FALSE;
		}

		// and the same with Version
		if (filter1.aVersions.GetSize() || filter2.aVersions.GetSize())
		{
			if (!Misc::MatchAll(filter1.aVersions, filter2.aVersions))
				return FALSE;

			if (filter1.HasFlag(FO_ANYVERSION) != filter2.HasFlag(FO_ANYVERSION))
				return FALSE;
		}

		// Custom attributes
		if (!filter1.mapCustomAttrib.MatchAll(filter2.mapCustomAttrib))
			return FALSE;

		return TRUE;
	}

};

struct TDCADVANCEDFILTER
{
	TDCADVANCEDFILTER(LPCTSTR szName = NULL, DWORD flags = 0) : sName(szName), dwFlags(flags)
	{
	}

	TDCADVANCEDFILTER(const TDCADVANCEDFILTER& filter)
	{
		*this = filter;
	}
	
	TDCADVANCEDFILTER& operator=(const TDCADVANCEDFILTER& filter)
	{
		params = filter.params;
		dwFlags = filter.dwFlags;
		sName = filter.sName;
				
		return *this;
	}
	
	BOOL operator==(const TDCADVANCEDFILTER& filter) const
	{
		return Matches(filter);
	}
	
	BOOL operator!=(const TDCADVANCEDFILTER& filter) const
	{
		return !Matches(filter);
	}
	
	BOOL IsSet(DWORD dwIgnore = 0) const
	{
		TDCADVANCEDFILTER filterEmpty;
		
		return !Matches(filterEmpty, dwIgnore);
	}
	
	BOOL HasNowFilter(TDC_ATTRIBUTE& nAttrib) const
	{
		// look thru the rules for a 'Now' relative date
		int nRule = params.aRules.GetSize();

		while (nRule--)
		{
			const SEARCHPARAM& rule = params.aRules.GetData()[nRule];
			
			if (rule.IsNowRelativeDate())
			{
				nAttrib = rule.GetAttribute();
				return TRUE;
			}
		}

		// else
		return FALSE;

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

	BOOL Matches(const TDCADVANCEDFILTER& filter, DWORD dwIgnoreFlags = 0) const
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

struct TDCATTRIBUTE
{
	TDC_ATTRIBUTE nAttributeID;
	TDC_ATTRIBUTECATEGORY nCategory;
	UINT nAttribResID;
};

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCOLEDITVISIBILITY
{
	TDCCOLEDITVISIBILITY() 
		: 
		nShowFields(TDLSA_ASCOLUMN),
		bShowColorEditIfAsColumns(FALSE)
	{
	}

	TDCCOLEDITVISIBILITY(const TDCCOLEDITVISIBILITY& vis)
	{
		*this = vis;
	}
	
	TDCCOLEDITVISIBILITY& operator=(const TDCCOLEDITVISIBILITY& vis)
	{
		mapVisibleColumns.Copy(vis.mapVisibleColumns);
		mapVisibleEdits.Copy(vis.mapVisibleEdits);

		nShowFields = vis.nShowFields;
		bShowColorEditIfAsColumns = vis.bShowColorEditIfAsColumns;
		
		return *this;
	}
	
	BOOL operator!=(const TDCCOLEDITVISIBILITY& vis) const
	{
		return !(*this == vis);
	}
	
	BOOL operator==(const TDCCOLEDITVISIBILITY& vis) const
	{
		return (mapVisibleColumns.MatchAll(vis.mapVisibleColumns) && 
				mapVisibleEdits.MatchAll(vis.mapVisibleEdits) &&
				(nShowFields == vis.nShowFields) &&
				(bShowColorEditIfAsColumns == vis.bShowColorEditIfAsColumns));
	}

	void Clear()
	{
		mapVisibleColumns.RemoveAll();
		mapVisibleEdits.RemoveAll();

		nShowFields = TDLSA_ASCOLUMN;
		bShowColorEditIfAsColumns = FALSE;
	}

	BOOL HasDifferences(const TDCCOLEDITVISIBILITY& vis, 
						BOOL& bColumnChange, BOOL& bEditChange) const
	{
		bColumnChange = !mapVisibleColumns.MatchAll(vis.mapVisibleColumns);
		bEditChange = !mapVisibleEdits.MatchAll(vis.mapVisibleEdits);
		
		return (bEditChange || bColumnChange || (nShowFields != vis.nShowFields));
	}

	TDL_SHOWATTRIB GetShowFields() const 
	{
		return nShowFields;
	}
	
	virtual void SetShowFields(TDL_SHOWATTRIB nShow)
	{
		nShowFields = nShow;

		UpdateEditVisibility();
	}

	void ShowColorEditIfAsColumns(BOOL bShow)
	{
		bShowColorEditIfAsColumns = bShow;

		UpdateEditVisibility();
	}

	const CTDCColumnIDMap& GetVisibleColumns() const
	{
		return mapVisibleColumns;
	}

	void SetVisibleColumns(const CTDCColumnIDMap& mapColumns)
	{
		mapVisibleColumns.Copy(mapColumns);
	}

	void SetAllColumnsVisible(BOOL bVisible = TRUE)
	{
		mapVisibleColumns.RemoveAll();

		for (int nCol = TDCC_FIRST; nCol < TDCC_COUNT; nCol++)
		{
			if (bVisible && IsSupportedColumn((TDC_COLUMN)nCol))
				mapVisibleColumns.Add((TDC_COLUMN)nCol);
		}

		UpdateEditVisibility();
	}
	
	BOOL SetAllFieldsVisible(BOOL bVisible = TRUE)
	{
		ASSERT(nShowFields == TDLSA_ANY);

		if (nShowFields != TDLSA_ANY)
			return FALSE;

		mapVisibleEdits.RemoveAll();
		
		if (bVisible)
			GetAllEditFields(mapVisibleEdits);

		return TRUE;
	}

	const CTDCAttributeMap& GetVisibleEditFields() const
	{
		return mapVisibleEdits;
	}
	
	BOOL SetVisibleEditFields(const CTDCAttributeMap& mapAttrib)
	{
		// only supported for 'any' attribute
		if (nShowFields != TDLSA_ANY)
		{
			ASSERT(0);
			return FALSE;
		}

		// check and add attributes 
		mapVisibleEdits.RemoveAll();

		BOOL bAnySupported = FALSE;
		POSITION pos = mapAttrib.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE nAttribID = mapAttrib.GetNext(pos);

			if (IsSupportedEdit(nAttribID))
			{
				mapVisibleEdits.Add(nAttribID);
				bAnySupported = TRUE;
			}
		}
		
		return bAnySupported;
	}

	BOOL IsEditFieldVisible(TDC_ATTRIBUTE nAttribID) const
	{
		// weed out unsupported attributes
		if (!IsSupportedEdit(nAttribID))
			return FALSE;

		switch (nShowFields)
		{
		case TDLSA_ALL:
			return TRUE;

		case TDLSA_ASCOLUMN:
			if ((nAttribID == TDCA_COLOR) && bShowColorEditIfAsColumns)
				return TRUE;
			else
				return IsColumnVisible(TDC::MapAttributeToColumn(nAttribID));

		case TDLSA_ANY:
			return mapVisibleEdits.Has(nAttribID);
		}

		// how did we get here?
		ASSERT(0);
		return FALSE;
	}
	
	BOOL IsColumnVisible(TDC_COLUMN nColID) const
	{
		if (!IsSupportedColumn(nColID))
			return FALSE;

		// else
		return mapVisibleColumns.Has(nColID);
	}
	
	BOOL SetEditFieldVisible(TDC_ATTRIBUTE nAttribID, BOOL bVisible = TRUE)
	{
		// only supported for 'any' attribute
		if (nShowFields != TDLSA_ANY)
		{
			ASSERT(0);
			return FALSE;
		}

		// validate attribute
		if (!IsSupportedEdit(nAttribID))
		{
			ASSERT(0);
			return FALSE;
		}
		else if (bVisible)
		{
			// Times cannot be shown if corresponding date column is hidden
			switch (nAttribID)
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
			switch (nAttribID)
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

		BOOL bFound = mapVisibleEdits.Has(nAttribID);
		
		if (bVisible && !bFound)
		{
			mapVisibleEdits.Add(nAttribID);
		}
		else if (!bVisible && bFound)
		{
			mapVisibleEdits.Remove(nAttribID);
		}

		return TRUE;
	}
	
	void SetColumnVisible(TDC_COLUMN nColID, BOOL bVisible = TRUE)
	{
		// special cases
		if (nColID == TDCC_NONE)
		{
			return;
		}
		else if (bVisible)
		{
			// Times cannot be shown if corresponding date column is hidden
			switch (nColID)
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

			case TDCC_CREATIONTIME:
				bVisible &= IsColumnVisible(TDCC_CREATIONDATE);
				break;
			}
		}
		else // !bVisible
		{
			// Times cannot be shown if corresponding date column is hidden
			switch (nColID)
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

			case TDCC_CREATIONDATE:
				SetColumnVisible(TDCC_CREATIONTIME, FALSE);
				break;
			}
		}

		BOOL bFound = mapVisibleColumns.Has(nColID);
		
		if (bVisible && !bFound) // show
		{
			mapVisibleColumns.Add(nColID);
		}
		else if (!bVisible && bFound) // hide
		{
			mapVisibleColumns.Remove(nColID);
		}

		UpdateEditVisibility();
	}

	void Save(IPreferences* pPrefs, LPCTSTR szKey) const
	{
		pPrefs->WriteProfileInt(szKey, _T("ShowAttributes"), nShowFields);

		mapVisibleColumns.Save(pPrefs, Misc::MakeKey(_T("%s\\ColumnVisibility"), szKey));

		if (nShowFields == TDLSA_ANY)
			mapVisibleEdits.Save(pPrefs, Misc::MakeKey(_T("%s\\EditVisibility"), szKey));
	}

	BOOL Load(const IPreferences* pPrefs, LPCTSTR szKey)
	{
		nShowFields = (TDL_SHOWATTRIB)pPrefs->GetProfileInt(szKey, _T("ShowAttributes"), TDLSA_ASCOLUMN);

		// columns
		CString sKey = Misc::MakeKey(_T("%s\\ColumnVisibility"), szKey);

		if (!mapVisibleColumns.Load(pPrefs, sKey) && 
			!mapVisibleColumns.LegacyLoad(pPrefs, sKey, _T("Col%d")))
		{
			return FALSE; // old version OR first time
		}

		// else
		if (nShowFields == TDLSA_ANY)
		{
			sKey = Misc::MakeKey(_T("%s\\EditVisibility"), szKey);

			if (!mapVisibleEdits.Load(pPrefs, sKey))
				mapVisibleEdits.LegacyLoad(pPrefs, sKey, _T("Edit%d"));
		}
		else
		{
			UpdateEditVisibility();
		}

		return TRUE;
	}

	static BOOL IsSupportedEdit(TDC_ATTRIBUTE nAttribID)
	{
		switch (nAttribID)
		{
		// Editable
		case TDCA_DONEDATE:
		case TDCA_DUEDATE:
		case TDCA_STARTDATE:
		case TDCA_PRIORITY:
		case TDCA_COLOR:
		case TDCA_ALLOCTO:
		case TDCA_ALLOCBY:
		case TDCA_STATUS:
		case TDCA_CATEGORY:
		case TDCA_PERCENT:
		case TDCA_TIMEESTIMATE:
		case TDCA_TIMESPENT:
		case TDCA_FILELINK:
		case TDCA_RISK:			
		case TDCA_EXTERNALID:	
		case TDCA_COST:			
		case TDCA_DEPENDENCY:	
		case TDCA_RECURRENCE:	
		case TDCA_VERSION:		
		case TDCA_DUETIME:
		case TDCA_STARTTIME:
		case TDCA_DONETIME:
		case TDCA_TAGS:
		case TDCA_PROJECTNAME:
		case TDCA_FLAG:
		case TDCA_LOCK:
		case TDCA_ICON:
			return TRUE;

		// Display only
		case TDCA_CREATEDBY:
		case TDCA_PATH:
		case TDCA_POSITION:
		case TDCA_CREATIONDATE: // includes time
		case TDCA_LASTMODDATE:  // includes time
		case TDCA_COMMENTSSIZE:
		case TDCA_COMMENTSFORMAT:
		case TDCA_SUBTASKDONE:
		case TDCA_LASTMODBY:
		case TDCA_ID:
		case TDCA_PARENTID:
		case TDCA_TIMEREMAINING:
			return TRUE;
		}

		// all else
		return FALSE;
	}

	static BOOL IsSupportedFilter(TDC_ATTRIBUTE nAttribID)
	{
		switch (nAttribID)
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

		// all else
		return FALSE;
	}
	
	static BOOL IsSupportedColumn(TDC_COLUMN nColumn)
	{
		switch (nColumn)
		{
		case TDCC_PRIORITY:
		case TDCC_PERCENT:
		case TDCC_TIMEESTIMATE:
		case TDCC_TIMESPENT:
		case TDCC_STARTDATE:
		case TDCC_DUEDATE:
		case TDCC_DONEDATE:
		case TDCC_ALLOCTO:
		case TDCC_ALLOCBY:
		case TDCC_STATUS:
		case TDCC_CATEGORY:
		case TDCC_FILELINK:
		case TDCC_POSITION:
		case TDCC_ID:
		case TDCC_DONE:
		case TDCC_TRACKTIME:
		case TDCC_FLAG:
		case TDCC_LOCK:
		case TDCC_CREATIONDATE:
		case TDCC_CREATEDBY:
		case TDCC_LASTMODDATE:
		case TDCC_LASTMODBY:
		case TDCC_RISK:
		case TDCC_EXTERNALID:
		case TDCC_COST:
		case TDCC_DEPENDENCY:
		case TDCC_RECURRENCE:
		case TDCC_VERSION:
		case TDCC_RECENTEDIT:
		case TDCC_TIMEREMAINING:
		case TDCC_ICON:
		case TDCC_REMINDER:
		case TDCC_PARENTID:
		case TDCC_PATH:
		case TDCC_TAGS:
		case TDCC_SUBTASKDONE:
		case TDCC_STARTTIME:
		case TDCC_DUETIME:
		case TDCC_DONETIME:
		case TDCC_CREATIONTIME:
		case TDCC_COMMENTSSIZE:
		case TDCC_COMMENTSFORMAT:
			return TRUE;
		}

		// all else
		return FALSE;
	}

protected:
	CTDCColumnIDMap mapVisibleColumns;
	CTDCAttributeMap mapVisibleEdits;
	TDL_SHOWATTRIB nShowFields;
	BOOL bShowColorEditIfAsColumns;

protected:
	int UpdateEditVisibility()
	{
		if (nShowFields != TDLSA_ANY)
		{
			mapVisibleEdits.RemoveAll();
			
			for (int nEdit = TDCA_FIRST_ATTRIBUTE; nEdit <= TDCA_LAST_REALATTRIBUTE; nEdit++)
			{
				if (IsEditFieldVisible((TDC_ATTRIBUTE)nEdit))
					mapVisibleEdits.Add((TDC_ATTRIBUTE)nEdit);
			}
		}
		
		return mapVisibleEdits.GetCount();
	}
	
	static int GetAllEditFields(CTDCAttributeMap& mapAttrib)
	{
		mapAttrib.RemoveAll();
		
		for (int nAttribID = TDCA_FIRST_ATTRIBUTE; nAttribID <= TDCA_LAST_REALATTRIBUTE; nAttribID++)
		{
			if (IsSupportedEdit((TDC_ATTRIBUTE)nAttribID))
				mapAttrib.Add((TDC_ATTRIBUTE)nAttribID);
		}
		
		return mapAttrib.GetCount();
	}

};

struct TDCCOLEDITFILTERVISIBILITY : public TDCCOLEDITVISIBILITY
{
	TDCCOLEDITFILTERVISIBILITY() : TDCCOLEDITVISIBILITY() {}

	TDCCOLEDITFILTERVISIBILITY(const TDCCOLEDITFILTERVISIBILITY& vis)
	{
		*this = vis;
	}

	TDCCOLEDITFILTERVISIBILITY& operator=(const TDCCOLEDITFILTERVISIBILITY& vis)
	{
		TDCCOLEDITVISIBILITY::operator=(vis);
		mapVisibleFilters.Copy(vis.mapVisibleFilters);

		return *this;
	}

	BOOL operator!=(const TDCCOLEDITFILTERVISIBILITY& vis) const
	{
		return !(*this == vis);
	}

	BOOL operator==(const TDCCOLEDITFILTERVISIBILITY& vis) const
	{
		return (TDCCOLEDITVISIBILITY::operator==(vis) && 
				mapVisibleFilters.MatchAll(vis.mapVisibleFilters));
	}

	void Clear()
	{
		TDCCOLEDITVISIBILITY::Clear();

		mapVisibleFilters.RemoveAll();
	}

	BOOL HasDifferences(const TDCCOLEDITFILTERVISIBILITY& vis, 
						BOOL& bColumnChange, BOOL& bEditChange, BOOL& bFilterChange) const
	{
		TDCCOLEDITVISIBILITY::HasDifferences(vis, bColumnChange, bEditChange);
		bFilterChange = !mapVisibleFilters.MatchAll(vis.mapVisibleFilters);

		return (bEditChange || bColumnChange || bFilterChange);
	}

	void SetShowFields(TDL_SHOWATTRIB nShow)
	{
		TDCCOLEDITVISIBILITY::SetShowFields(nShow);

		UpdateFilterVisibility();
	}

	void SetAllColumnsVisible(BOOL bVisible = TRUE)
	{
		TDCCOLEDITVISIBILITY::SetAllColumnsVisible(bVisible);

		UpdateFilterVisibility();
	}

	BOOL SetAllFieldsVisible(BOOL bVisible = TRUE)
	{
		if (!TDCCOLEDITVISIBILITY::SetAllFieldsVisible(bVisible))
			return FALSE;

		mapVisibleFilters.RemoveAll();

		if (bVisible)
			GetAllFilterFields(mapVisibleFilters);

		return TRUE;
	}

	const CTDCAttributeMap& GetVisibleFilterFields() const
	{
		return mapVisibleFilters;
	}

	BOOL SetVisibleFilterFields(const CTDCAttributeMap& mapAttrib)
	{
		// only supported for 'any' attribute
		ASSERT(nShowFields == TDLSA_ANY);

		if (nShowFields != TDLSA_ANY)
			return FALSE;

		// check and add attributes 
		BOOL bAnySupported = FALSE;
		POSITION pos = mapAttrib.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE nAttribID = mapAttrib.GetNext(pos);

			if (IsSupportedFilter(nAttribID))
			{
				mapVisibleFilters.Add(nAttribID);
				bAnySupported = TRUE;
			}
		}

		return bAnySupported;
	}

	BOOL IsFilterFieldVisible(TDC_ATTRIBUTE nAttribID) const
	{
		// weed out unsupported attributes
		if (!IsSupportedFilter(nAttribID))
			return FALSE;

		switch (nShowFields)
		{
		case TDLSA_ALL:
			return TRUE;

		case TDLSA_ASCOLUMN:
			return IsColumnVisible(TDC::MapAttributeToColumn(nAttribID));

		case TDLSA_ANY:
			return mapVisibleFilters.Has(nAttribID);
		}

		// how did we get here?
		ASSERT(0);
		return FALSE;
	}

	BOOL SetFilterFieldVisible(TDC_ATTRIBUTE nAttribID, BOOL bVisible = TRUE)
	{
		// only supported for 'any' attribute
		ASSERT(nShowFields == TDLSA_ANY);

		if (nShowFields != TDLSA_ANY)
			return FALSE;

		// validate attribute
		ASSERT (IsSupportedFilter(nAttribID));

		if (!IsSupportedFilter(nAttribID))
			return FALSE;

		BOOL bFound = mapVisibleFilters.Has(nAttribID);

		if (bVisible && !bFound)
		{
			mapVisibleFilters.Add(nAttribID);
		}
		else if (!bVisible && bFound)
		{
			mapVisibleFilters.Remove(nAttribID);
		}

		return TRUE;
	}

	void Save(IPreferences* pPrefs, LPCTSTR szKey) const
	{
		TDCCOLEDITVISIBILITY::Save(pPrefs, szKey);

		if (nShowFields == TDLSA_ANY)
			mapVisibleFilters.Save(pPrefs, Misc::MakeKey(_T("%s\\FilterVisibility"), szKey));
	}

	BOOL Load(const IPreferences* pPrefs, LPCTSTR szKey)
	{
		if (!TDCCOLEDITVISIBILITY::Load(pPrefs, szKey))
			return FALSE;

		if (nShowFields == TDLSA_ANY)
		{
			CString sKey = Misc::MakeKey(_T("%s\\FilterVisibility"), szKey);

			if (!mapVisibleFilters.Load(pPrefs, sKey))
				mapVisibleFilters.LegacyLoad(pPrefs, sKey, _T("Filter%d"));
		}
		else
		{
			UpdateFilterVisibility();
		}

		return TRUE;
	}

	static BOOL IsSupportedFilter(TDC_ATTRIBUTE nAttribID)
	{
		switch (nAttribID)
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
		case TDCA_RECURRENCE:
			return TRUE;
		}

		// else
		return FALSE;
	}

protected:
	CTDCAttributeMap mapVisibleFilters;

protected:
	int UpdateFilterVisibility()
	{
		if (nShowFields != TDLSA_ANY)
		{
			mapVisibleFilters.RemoveAll();

			for (int nFilter = TDCA_FIRST_ATTRIBUTE; nFilter <= TDCA_LAST_REALATTRIBUTE; nFilter++)
			{
				if (IsFilterFieldVisible((TDC_ATTRIBUTE)nFilter))
					mapVisibleFilters.Add((TDC_ATTRIBUTE)nFilter);
			}
		}

		return mapVisibleFilters.GetCount();
	}

	static int GetAllFilterFields(CTDCAttributeMap& mapAttrib)
	{
		mapAttrib.RemoveAll();
		
		for (int nAttribID = TDCA_FIRST_ATTRIBUTE; nAttribID <= TDCA_LAST_REALATTRIBUTE; nAttribID++)
		{
			if (IsSupportedFilter((TDC_ATTRIBUTE)nAttribID))
				mapAttrib.Add((TDC_ATTRIBUTE)nAttribID);
		}
		
		return mapAttrib.GetCount();
	}
};

/////////////////////////////////////////////////////////////////////////////

struct TDCATTRIBUTEMAPPING
{
	TDCATTRIBUTEMAPPING() : nAttributeID(TDCA_NONE) {}
	TDCATTRIBUTEMAPPING(const CString& sName, TDC_ATTRIBUTE nAttribID, DWORD dwData = 0) 
	{ 
		sColumnName = sName; 
		nAttributeID = nAttribID;
		dwItemData = dwData;
	}

	TDCATTRIBUTEMAPPING(UINT nNameID, TDC_ATTRIBUTE nAttribID, DWORD dwData = 0) 
	{ 
		sColumnName.LoadString(nNameID); 
		nAttributeID = nAttribID;
		dwItemData = dwData;
	}

	TDC_ATTRIBUTE nAttributeID;
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

	int Find(TDC_ATTRIBUTE nAttribID) const
	{
		int nMap = GetSize();

		while (nMap--)
		{
			if (GetData()[nMap].nAttributeID == nAttribID)
				return nMap;
		}

		// not found
		return -1;
	}

	int Find(DWORD dwData) const
	{
		int nMap = GetSize();

		while (nMap--)
		{
			if (GetData()[nMap].dwItemData == dwData)
				return nMap;
		}

		// not found
		return -1;
	}

	int FindMappedAttribute(TDC_ATTRIBUTE nAttribID) const
	{
		int nMap = GetSize();

		while (nMap--)
		{
			const TDCATTRIBUTEMAPPING& col = GetData()[nMap];

			if ((col.nAttributeID == nAttribID) && !col.sColumnName.IsEmpty())
				return nMap;
		}

		// not found or mapped
		return -1;
	}

	BOOL CTDCAttributeMapping::IsAttributeMapped(TDC_ATTRIBUTE nAttribID) const
	{
		return (FindMappedAttribute(nAttribID) != -1);
	}

#ifdef _DEBUG
	void Trace() const
	{
		for (int nMap = 0; nMap < GetSize(); nMap++)
		{
			const TDCATTRIBUTEMAPPING& col = GetData()[nMap];

			TRACE(_T("CTDCAttributeMapping::Trace(%s, %d, %d)\n"), col.sColumnName, col.nAttributeID, col.dwItemData);
		}
	}
#else
	void Trace() const {}
#endif
};

/////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCSTRUCT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
