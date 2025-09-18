// tdlstruct.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcstruct.h"
#include "tdcmapping.h"
#include "tdcstatic.h"

#include "..\shared\datehelper.h"
#include "..\shared\timehelper.h"

#include "..\Interfaces\preferences.h"

//////////////////////////////////////////////////////////////////////

TDCDROPIMPORT::TDCDROPIMPORT(DWORD dwID, const CStringArray& sDropFiles) 
	: 
	dwTaskID(dwID)
{
	aFiles.Copy(sDropFiles);
}

TDCDROPIMPORT::TDCDROPIMPORT(DWORD dwID, const CString& sDropText) 
	: 
	dwTaskID(dwID), 
	sText(sDropText)
{
}

//////////////////////////////////////////////////////////////////////

TDCDATETIMEWIDTHS::TDCDATETIMEWIDTHS() 
	: 
	bIsoFormat(FALSE)
{
	ResetWidths();
}

void TDCDATETIMEWIDTHS::SetIsoFormat(BOOL bIso)
{
	if (!Misc::StatesDiffer(bIso, bIsoFormat))
		return;

	bIsoFormat = bIso;

	ResetWidths();
}

void TDCDATETIMEWIDTHS::ResetWidths()
{
	nMaxDateWidth = nMinDateWidth = nMaxTimeWidth = nMaxDowNameWidth = nSepWidth = -1;
}

void TDCDATETIMEWIDTHS::Initialise(CDC* pDC)
{
	if (nMaxDateWidth != -1)
	{
		// Sanity check
		ASSERT(nMaxDateWidth > 0);
		ASSERT(nMinDateWidth > 0);
		ASSERT(nMaxTimeWidth > 0);
		ASSERT(nMaxDowNameWidth > 0);
		ASSERT(nSepWidth > 0);

		return;
	}

	// Sanity check
	ASSERT(nMaxDateWidth == -1);
	ASSERT(nMinDateWidth == -1);
	ASSERT(nMaxTimeWidth == -1);
	ASSERT(nMaxDowNameWidth == -1);
	ASSERT(nSepWidth == -1);

	COleDateTime dtMax(2000, 12, 31, 0, 0, 0);
	DWORD dwDateFmt = (bIsoFormat ? DHFD_ISO : 0);

	CString sMaxDate = CDateHelper::FormatDate(dtMax, dwDateFmt);
	nMaxDateWidth = pDC->GetTextExtent(sMaxDate).cx;

	CString sMinDate = CDateHelper::FormatDate(dtMax, (dwDateFmt | DHFD_NOYEAR));
	nMinDateWidth = pDC->GetTextExtent(sMinDate).cx;

	if (bIsoFormat || Misc::GetAM().IsEmpty())
	{
		CString sMaxTime = CTimeHelper::FormatClockTime(23, 59, 0, FALSE, bIsoFormat);
		nMaxTimeWidth = pDC->GetTextExtent(sMaxTime).cx;
	}
	else
	{
		// We can't rely on AM/PM designators being of the same length
		CString sMaxAMTime = CTimeHelper::FormatClockTime(11, 59, 0, FALSE, FALSE);
		CString sMaxPMTime = CTimeHelper::FormatClockTime(23, 59, 0, FALSE, FALSE);

		int nMaxAMTimeWidth = pDC->GetTextExtent(sMaxAMTime).cx;
		int nMaxPMTimeWidth = pDC->GetTextExtent(sMaxPMTime).cx;

		nMaxTimeWidth = max(nMaxAMTimeWidth, nMaxPMTimeWidth);
	}

	nMaxDowNameWidth = CDateHelper::GetMaxDayOfWeekNameWidth(pDC, TRUE);
	nSepWidth = pDC->GetTextExtent(_T(" ")).cx;
}

int TDCDATETIMEWIDTHS::CalcMaxColumWidth(BOOL bIncTime, BOOL bIncDow) const
{
	// Always want date
	int nWidth = nMaxDateWidth;

	if (bIncTime)
	{
		nWidth += nSepWidth;
		nWidth += nMaxTimeWidth;
	}

	if (bIncDow)
	{
		nWidth += nSepWidth;
		nWidth += nMaxDowNameWidth;
	}

	return nWidth;
}

//////////////////////////////////////////////////////////////////////

TDCINFOTIPITEM::TDCINFOTIPITEM() // for CArray
	:
	nAttributeID(),
	nLabelWidth(0)
{
}

TDCINFOTIPITEM::TDCINFOTIPITEM(TDC_ATTRIBUTE nAttribID, CString sLab, const CString& sVal)
	:
	nAttributeID(nAttribID),
	sLabel(sLab),
	sValue(sVal),
	nLabelWidth(0)
{
}

TDCINFOTIPITEM::TDCINFOTIPITEM(TDC_ATTRIBUTE nAttribID, UINT nLabelStrID, const CString& sVal)
	:
	nAttributeID(nAttribID),
	sLabel(CEnString(nLabelStrID)),
	sValue(sVal),
	nLabelWidth(0)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////

void CTDCColorMap::AddColor(LPCTSTR szID, COLORREF color)
{
	SetAt(Misc::ToUpper(szID), color);
}

BOOL CTDCColorMap::GetColor(LPCTSTR szID, COLORREF& color) const
{
	return Lookup(Misc::ToUpper(szID), color);
}

void CTDCColorMap::RemoveAll()
{
	CMap<CString, LPCTSTR, COLORREF, COLORREF&>::RemoveAll();
}

void CTDCColorMap::Copy(const CTDCColorMap& other)
{
	Misc::CopyStrT<COLORREF>(other, *this);
}

BOOL CTDCColorMap::MatchAll(const CTDCColorMap& other) const
{
	return Misc::MatchAllStrT<COLORREF>(other, *this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

CTDCStyleMap::CTDCStyleMap()
{
}

CTDCStyleMap::CTDCStyleMap(TDC_STYLE nStyle, BOOL bEnabled)
{
	ASSERT(IsValidStyle(nStyle));

	SetAt(nStyle, bEnabled);
}

CTDCStyleMap::CTDCStyleMap(const CTDCStyleMap& mapStyles)
{
	Misc::CopyT<TDC_STYLE, BOOL>(mapStyles, *this);
}

BOOL CTDCStyleMap::IsStyleEnabled(TDC_STYLE nStyle) const
{
	ASSERT(IsValidStyle(nStyle));

	BOOL bEnabled = FALSE;

	return (Lookup(nStyle, bEnabled) && bEnabled);
}

BOOL CTDCStyleMap::HasStyle(TDC_STYLE nStyle) const
{
	ASSERT(IsValidStyle(nStyle));

	BOOL bUnused;

	return Lookup(nStyle, bUnused);
}

BOOL& CTDCStyleMap::operator[](TDC_STYLE nStyle)
{
	ASSERT(IsValidStyle(nStyle));

	return CMap<TDC_STYLE, TDC_STYLE, BOOL, BOOL&>::operator[](nStyle);
}

TDC_STYLE CTDCStyleMap::GetNext(POSITION& pos) const
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

BOOL CTDCStyleMap::IsValidStyle(TDC_STYLE nStyle) const
{
	return ((nStyle >= TDCS_FIRST) && (nStyle < TDCS_LAST));
}

/////////////////////////////////////////////////////////////////////////////////////////////

TDCITEMCOLORS::TDCITEMCOLORS() 
	: 
	dwItem(0), 
	crBack(CLR_NONE), 
	crText(CLR_NONE) 
{
}

/////////////////////////////////////////////////////////////////////////////////////////////

TDCNOTIFYMOD::TDCNOTIFYMOD(TDC_ATTRIBUTE nAttribID) 
	: 
	mapAttrib(nAttribID) 
{
}

TDCNOTIFYMOD::TDCNOTIFYMOD(TDC_ATTRIBUTE nAttribID, const CDWordArray& taskIDs) 
	: 
	mapAttrib(nAttribID)
{
	aTaskIDs.Copy(taskIDs);
}

TDCNOTIFYMOD::TDCNOTIFYMOD(const CTDCAttributeMap& attribIDs, const CDWordArray& taskIDs) 
	: 
	mapAttrib(attribIDs)
{
	aTaskIDs.Copy(taskIDs);
}

/////////////////////////////////////////////////////////////////////////////////////////////

TDCFINDWND::TDCFINDWND(HWND ignore, BOOL incClosing, DWORD procID)
	: 
	hWndIgnore(ignore), 
	bIncClosing(incClosing), 
	dwProcessID(procID)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////

TDCAUTOLISTDATA::TDCAUTOLISTDATA() 
{
}

TDCAUTOLISTDATA::TDCAUTOLISTDATA(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID)
{
	Copy(other, nAttribID);
}

int TDCAUTOLISTDATA::Copy(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID)
{
	return Copy(other, *this, FALSE, nAttribID);
}

int TDCAUTOLISTDATA::AppendUnique(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID)
{
	return Copy(other, *this, TRUE, nAttribID);
}

int TDCAUTOLISTDATA::RemoveItems(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID)
{
#define REMOVE_ITEMS(att, arr) if (WantAttribute(nAttribID, att)) nNumRemoved += Misc::RemoveItems(other.arr, arr)

	int nNumRemoved = 0;

	REMOVE_ITEMS(TDCA_CATEGORY, aCategory);
	REMOVE_ITEMS(TDCA_STATUS,	aStatus);
	REMOVE_ITEMS(TDCA_ALLOCTO,	aAllocTo);
	REMOVE_ITEMS(TDCA_ALLOCBY,	aAllocBy);
	REMOVE_ITEMS(TDCA_TAGS,		aTags);
	REMOVE_ITEMS(TDCA_VERSION,	aVersion);

	return nNumRemoved;
}

int TDCAUTOLISTDATA::GetSize() const
{
	return (aCategory.GetSize() +
			aStatus.GetSize() +
			aAllocTo.GetSize() +
			aAllocBy.GetSize() +
			aTags.GetSize() +
			aVersion.GetSize());
}

void TDCAUTOLISTDATA::RemoveAll(TDC_ATTRIBUTE nAttribID)
{
#define REMOVE_ALL(att, arr) if (WantAttribute(nAttribID, att)) arr.RemoveAll()

	REMOVE_ALL(TDCA_CATEGORY,	aCategory);
	REMOVE_ALL(TDCA_STATUS,		aStatus);
	REMOVE_ALL(TDCA_ALLOCTO,	aAllocTo);
	REMOVE_ALL(TDCA_ALLOCBY,	aAllocBy);
	REMOVE_ALL(TDCA_TAGS,		aTags);
	REMOVE_ALL(TDCA_VERSION,	aVersion);
}

CString TDCAUTOLISTDATA::Format(TDC_ATTRIBUTE nAttribID, TCHAR cSep) const
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

BOOL TDCAUTOLISTDATA::Matches(const TDCAUTOLISTDATA& other, TDC_ATTRIBUTE nAttribID) const
{
#define MATCH_ARR(att, arr) if (WantAttribute(nAttribID, att) && !Misc::MatchAll(arr, other.arr)) return FALSE

	MATCH_ARR(TDCA_CATEGORY,	aCategory);
	MATCH_ARR(TDCA_STATUS,		aStatus);
	MATCH_ARR(TDCA_ALLOCTO,		aAllocTo);
	MATCH_ARR(TDCA_ALLOCBY,		aAllocBy);
	MATCH_ARR(TDCA_TAGS,		aTags);
	MATCH_ARR(TDCA_VERSION,		aVersion);

	return TRUE;
}

BOOL TDCAUTOLISTDATA::WantAttribute(TDC_ATTRIBUTE nAttribID, TDC_ATTRIBUTE nWantedAttribID) const
{
	return ((nAttribID == TDCA_ALL) || (nAttribID == nWantedAttribID));
}

int TDCAUTOLISTDATA::Copy(const TDCAUTOLISTDATA& from, TDCAUTOLISTDATA& to, BOOL bAppend, TDC_ATTRIBUTE nAttribID)
{
#define COPY_ARR(att, arr) if (WantAttribute(nAttribID, att)) nNumCopied += CopyItems(from.arr, to.arr, bAppend);

	int nNumCopied = 0;

	COPY_ARR(TDCA_CATEGORY,	aCategory);
	COPY_ARR(TDCA_STATUS,	aStatus);
	COPY_ARR(TDCA_ALLOCTO,	aAllocTo);
	COPY_ARR(TDCA_ALLOCBY,	aAllocBy);
	COPY_ARR(TDCA_TAGS,		aTags);
	COPY_ARR(TDCA_VERSION,	aVersion);

	return nNumCopied;
}

int TDCAUTOLISTDATA::CopyItems(const CStringArray& aFrom, CStringArray& aTo, BOOL bAppend)
{
	if (!bAppend)
		aTo.RemoveAll();

	return Misc::AppendItems(aFrom, aTo, TRUE);
}

/////////////////////////////////////////////////////////////////////////////////////////////

TDCEXPORTTASKLIST::TDCEXPORTTASKLIST(HWND hwnd, const CString& sTDC, int nExp)
	:
	hWndNotify(hwnd),
	sTDCPath(sTDC),
	nExporter(nExp),
	pImpExpMgr(NULL),
	nPurpose(TDCTEP_NONE)
{
}

BOOL TDCEXPORTTASKLIST::IsValid() const
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

/////////////////////////////////////////////////////////////////////////////////////////////

TDCGETTASKS::TDCGETTASKS(TDC_GETTASKS filter, DWORD flags)
	: 
	nFilter(filter), 
	dwFlags(flags)
{
	InitDueByDate();
}

TDCGETTASKS::TDCGETTASKS(const TDCGETTASKS& filter)
{
	*this = filter;
}

TDCGETTASKS& TDCGETTASKS::operator=(const TDCGETTASKS& filter)
{
	nFilter = filter.nFilter;
	dateDueBy = filter.dateDueBy;
	dwFlags = filter.dwFlags;
	sAllocTo = filter.sAllocTo;

	mapAttribs.Copy(filter.mapAttribs);

	return *this;
}

BOOL TDCGETTASKS::WantAttribute(TDC_ATTRIBUTE nAttribID) const
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

BOOL TDCGETTASKS::IsSet(BOOL bIncAttrib) const
{
	if ((nFilter != TDCGT_ALL) ||
		dwFlags ||
		!sAllocTo.IsEmpty() ||
		CDateHelper::IsDateSet(dateDueBy))
	{
		return TRUE;
	}

	if (bIncAttrib)
	{
		return mapAttribs.GetCount();
	}

	// else
	return FALSE;
}

BOOL TDCGETTASKS::HasFlag(DWORD dwFlag) const
{
	// Dependent flags
	if ((dwFlag == TDCGSTF_IMMEDIATEPARENT) && HasFlag(TDCGSTF_ALLPARENTS))
		return TRUE;

	return Misc::HasFlag(dwFlags, dwFlag);
}

void TDCGETTASKS::InitDueByDate()
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

/////////////////////////////////////////////////////////////////////////////////////////////

TDCSELECTIONCACHE::TDCSELECTIONCACHE() 
	: 
	dwFocusedTaskID(0), 
	dwFirstVisibleTaskID(0) 
{
}

BOOL TDCSELECTIONCACHE::SelectionMatches(const TDCSELECTIONCACHE& cache, BOOL bAndFocus) const
{
	return (Misc::MatchAllT(aSelTaskIDs, cache.aSelTaskIDs, FALSE) &&
			(!bAndFocus || (dwFocusedTaskID == cache.dwFocusedTaskID)));
}

void TDCSELECTIONCACHE::Clear()
{
	aSelTaskIDs.RemoveAll();
	aBreadcrumbs.RemoveAll();

	dwFocusedTaskID = dwFirstVisibleTaskID = 0;
}

BOOL TDCSELECTIONCACHE::IsEmpty() const
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

/////////////////////////////////////////////////////////////////////////////////////////////

UINT TDCCOLUMN::GetColumnHeaderAlignment() const
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

/////////////////////////////////////////////////////////////////////////////////////////////

CTRLITEM::CTRLITEM()
	:
	nCtrlID(0),
	nLabelID(0),
	nAttributeID(TDCA_NONE)
{
}

CTRLITEM::CTRLITEM(UINT ctrlID, UINT labelID, TDC_ATTRIBUTE nAttribID)
	:
	nCtrlID(ctrlID),
	nLabelID(labelID),
	nAttributeID(nAttribID)
{
}

BOOL CTRLITEM::operator==(const CTRLITEM& other) const
{
	return ((nCtrlID == other.nCtrlID) &&
			(nLabelID == other.nLabelID) &&
			(nAttributeID == other.nAttributeID));
}

BOOL CTRLITEM::operator!=(const CTRLITEM& other) const
{
	return !(*this == other);
}

CWnd* CTRLITEM::GetCtrl(const CWnd* pParent) const
{
	return pParent->GetDlgItem(nCtrlID);
}

CWnd* CTRLITEM::GetLabel(const CWnd* pParent) const
{
	return pParent->GetDlgItem(nLabelID);
}

void CTRLITEM::DeleteCtrls(const CWnd* pParent)
{
	DeleteCtrl(pParent, nCtrlID);
	DeleteCtrl(pParent, nLabelID);
}

void CTRLITEM::DeleteCtrl(const CWnd* pParent, UINT& nID)
{
	CWnd* pCtrl = pParent->GetDlgItem(nID);

	if (pCtrl)
	{
		pCtrl->DestroyWindow();
		delete pCtrl;
	}

	nID = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////

CUSTOMATTRIBCTRLITEM::CUSTOMATTRIBCTRLITEM() 
	:
	nBuddyCtrlID(0),
	nBuddyLabelID(0),
	pBuddyClass(NULL)

{
}

BOOL CUSTOMATTRIBCTRLITEM::operator==(const CUSTOMATTRIBCTRLITEM& other) const
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

BOOL CUSTOMATTRIBCTRLITEM::operator!=(const CUSTOMATTRIBCTRLITEM& other) const
{
	return !(*this == other);
}

void CUSTOMATTRIBCTRLITEM::ShowBuddy(const CWnd* pParent, BOOL bShow)
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

BOOL CUSTOMATTRIBCTRLITEM::IsShowingBuddy() const
{
	return (HasBuddy() && pBuddyClass);
}

BOOL CUSTOMATTRIBCTRLITEM::HasBuddy() const
{
	return (nBuddyCtrlID && nBuddyLabelID);
}

BOOL CUSTOMATTRIBCTRLITEM::GetBuddy(CTRLITEM& ctrl) const
{
	if (!HasBuddy())
		return FALSE;

	ctrl.nAttributeID = nAttributeID;
	ctrl.nCtrlID = nBuddyCtrlID;
	ctrl.nLabelID = nBuddyLabelID;

	return TRUE;
}

CWnd* CUSTOMATTRIBCTRLITEM::GetBuddy(const CWnd* pParent) const
{
	return pParent->GetDlgItem(nBuddyCtrlID);
}

CWnd* CUSTOMATTRIBCTRLITEM::GetBuddyLabel(const CWnd* pParent) const
{
	return pParent->GetDlgItem(nBuddyLabelID);
}

void CUSTOMATTRIBCTRLITEM::DeleteCtrls(const CWnd* pParent)
{
	CTRLITEM::DeleteCtrls(pParent);

	DeleteBuddy(pParent);
}

void CUSTOMATTRIBCTRLITEM::DeleteBuddy(const CWnd* pParent)
{
	DeleteCtrl(pParent, nBuddyCtrlID);
	DeleteCtrl(pParent, nBuddyLabelID);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void CTDCCustomControlArray::DeleteCtrls(const CWnd* pParent)
{
	for (int nCtrl = 0; nCtrl < GetSize(); nCtrl++)
	{
		GetAt(nCtrl).DeleteCtrls(pParent);
	}
}

int CTDCCustomControlArray::Find(UINT nCtrlID, BOOL bIncludeLabels) const
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

int CTDCCustomControlArray::Find(TDC_ATTRIBUTE nAttribID) const
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

int CTDCCustomControlArray::Find(const CString& sAttribID) const
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

/////////////////////////////////////////////////////////////////////////////////////////////

SEARCHPARAM::SEARCHPARAM(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp)
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

SEARCHPARAM::SEARCHPARAM(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, CString sVal, BOOL and, FIND_ATTRIBTYPE nType)
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

SEARCHPARAM::SEARCHPARAM(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, double dVal, BOOL and)
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

SEARCHPARAM::SEARCHPARAM(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, int nVal, BOOL and)
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
	case FT_COLOR:
		nValue = nVal;
		break;

	default:
		ASSERT(0);
	}
}

BOOL SEARCHPARAM::operator!=(const SEARCHPARAM& rule) const
{
	return !(*this == rule);
}

BOOL SEARCHPARAM::operator==(const SEARCHPARAM& rule) const
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
		case FT_COLOR:
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

BOOL SEARCHPARAM::Set(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, CString sVal, BOOL and, FIND_ATTRIBTYPE nType)
{
	if (!SetAttribute(nAttribID, nType))
		return FALSE;

	SetOperator(nOp);
	SetAnd(and);
	SetValue(sVal);

	return TRUE;
}

BOOL SEARCHPARAM::Set(TDC_ATTRIBUTE nAttribID, const CString& sID, FIND_ATTRIBTYPE nType, FIND_OPERATOR nOp, CString sVal, BOOL and)
{
	if (!SetCustomAttribute(nAttribID, sID, nType))
		return FALSE;

	SetOperator(nOp);
	SetAnd(and);
	SetValue(sVal);

	return TRUE;
}

BOOL SEARCHPARAM::SetAttribute(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType)
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

	// update Attribute type
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

TDC_ATTRIBUTE SEARCHPARAM::GetAttribute() const 
{ 
	return nAttributeID; 
}

FIND_OPERATOR SEARCHPARAM::GetOperator() const 
{ 
	return nOperator; 
}

BOOL SEARCHPARAM::GetAnd() const 
{ 
	return bAnd; 
}

BOOL SEARCHPARAM::GetOr() const 
{ 
	return !bAnd; 
}

BOOL SEARCHPARAM::IsCustomAttribute() const 
{ 
	return TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttributeID); 
}

BOOL SEARCHPARAM::IsRelativeDate(BOOL bMustHaveValue) const
{
	if (GetAttribType() != FT_DATERELATIVE)
		return FALSE;

	if (sValue.IsEmpty() && !bMustHaveValue)
		return TRUE;

	return CDateHelper::IsValidRelativeDate(sValue, FALSE);
}

BOOL SEARCHPARAM::IsNowRelativeDate() const
{
	if (!IsRelativeDate())
		return FALSE;

	return ((sValue[0] == 'n') || (sValue[0] == 'N'));
}

BOOL SEARCHPARAM::SetCustomAttribute(TDC_ATTRIBUTE nAttribID, const CString& sID, FIND_ATTRIBTYPE nType)
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

CString SEARCHPARAM::GetCustomAttributeID() const
{
	ASSERT(IsCustomAttribute());
	ASSERT(!sCustAttribID.IsEmpty());

	return sCustAttribID;
}

BOOL SEARCHPARAM::SetOperator(FIND_OPERATOR nOp)
{
	if (IsValidOperator(GetAttribType(), nOp))
	{
		nOperator = nOp;
		return TRUE;
	}

	return FALSE;
}

FIND_ATTRIBTYPE SEARCHPARAM::GetAttribType() const
{
	if (nAttribType == FT_NONE)
		nAttribType = GetAttribType(nAttributeID, bRelativeDate);

	ASSERT((nAttribType != FT_NONE) ||
			(nAttributeID == TDCA_NONE) ||
			(nAttributeID == TDCA_SELECTION));

	return nAttribType;
}

void SEARCHPARAM::ClearValue()
{
	sValue.Empty();
	dValue = 0.0;
	nValue = 0;
}

FIND_ATTRIBTYPE SEARCHPARAM::GetAttribType(TDC_ATTRIBUTE nAttribID, BOOL bRelativeDate)
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
	case TDCA_PERCENT:
	case TDCA_RISK:
	case TDCA_ID:
	case TDCA_PARENTID:
	case TDCA_POSITION:
		return FT_INTEGER;

	case TDCA_RECURRENCE:
		return FT_RECURRENCE;

	case TDCA_COLOR:
		return FT_COLOR;

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

	case TDCA_MATCHGROUPSTART:
	case TDCA_MATCHGROUPEND:
		return FT_GROUP;
	}

	// custom attribute type must be set explicitly by caller
	return FT_NONE;
}

BOOL SEARCHPARAM::IsValidOperator(FIND_ATTRIBTYPE nType, FIND_OPERATOR nOp)
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

	case FOP_DEPENDS_COMPLETE:
		return (nType == FT_DEPENDENCY);

	case FOP_SET:
	case FOP_NOT_SET:
		return TRUE;
	}

	return FALSE;
}

BOOL SEARCHPARAM::HasValidOperator() const
{
	return IsValidOperator(GetAttribType(), nOperator);
}

void SEARCHPARAM::ValidateOperator()
{
	if (!HasValidOperator())
		nOperator = FOP_NONE;
}

BOOL SEARCHPARAM::OperatorIs(FIND_OPERATOR nOp) const
{
	return (nOperator == nOp);
}

BOOL SEARCHPARAM::AttributeIs(TDC_ATTRIBUTE nAttribID) const
{
	return (nAttributeID == nAttribID);
}

BOOL SEARCHPARAM::TypeIs(FIND_ATTRIBTYPE nType) const
{
	return (GetAttribType() == nType);
}

BOOL SEARCHPARAM::HasString() const
{
	return (TypeIs(FT_STRING) && !sValue.IsEmpty());
}

BOOL SEARCHPARAM::HasIcon() const
{
	return (TypeIs(FT_ICON) && !sValue.IsEmpty());
}

void SEARCHPARAM::SetAnd(BOOL and)
{
	bAnd = and;
}

void SEARCHPARAM::SetTimeUnits(TDC_UNITS nUnits)
{
	ASSERT(TypeIs(FT_TIMEPERIOD));

	nTimeUnits = nUnits;
}

void SEARCHPARAM::SetMatchWholeWord(BOOL bMatchWhole)
{
	ASSERT(TypeIs(FT_STRING));

	bMatchWholeWord = bMatchWhole;
}

void SEARCHPARAM::SetFlags(DWORD flags)
{
	dwFlags = flags;
}

void SEARCHPARAM::SetRelativeDate(BOOL bRelative)
{
	ASSERT(TypeIs(FT_DATE) || TypeIs(FT_DATERELATIVE));

	bRelativeDate = bRelative;
}

DWORD SEARCHPARAM::GetFlags() const
{
	return dwFlags;
}

TDC_UNITS SEARCHPARAM::GetTimeUnits() const
{
	ASSERT(TypeIs(FT_TIMEPERIOD));

	return nTimeUnits;
}

BOOL SEARCHPARAM::GetMatchWholeWord() const
{
	ASSERT(TypeIs(FT_STRING) || TypeIs(FT_DEPENDENCY) || TypeIs(FT_ICON));

	return bMatchWholeWord;
}

BOOL SEARCHPARAM::GetRelativeDate() const
{
	ASSERT(TypeIs(FT_DATE) || TypeIs(FT_DATERELATIVE));

	return bRelativeDate;
}

void SEARCHPARAM::SetValue(const CString& sVal)
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
	case FT_COLOR:
		nValue = _ttoi(sVal);
		break;

	case FT_GROUP:
		ASSERT(sVal.IsEmpty());
		break;

	default:
		ASSERT(0);
		break;
	}
}

void SEARCHPARAM::SetValue(double dVal)
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

void SEARCHPARAM::SetValue(int nVal)
{
	switch (GetAttribType())
	{
	case FT_INTEGER:
	case FT_BOOL:
	case FT_RECURRENCE:
	case FT_COLOR:
		nValue = nVal;
		break;

	default:
		ASSERT(0);
		break;
	}
}

void SEARCHPARAM::SetValue(const COleDateTime& dtVal)
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

CString SEARCHPARAM::ValueAsString() const
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
		case TDCA_RISK:
			if (nValue == FM_NOPRIORITYORRISK)
				return CEnString(IDS_TDC_NONE);
			break;
		}
		// else fall thru

	case FT_BOOL:
	case FT_RECURRENCE:
	case FT_COLOR:
		return Misc::Format(nValue);

	case FT_STRING:
	case FT_DATERELATIVE:
	case FT_ICON:
	case FT_DEPENDENCY:
		return sValue;

	case FT_GROUP:
		break;

	default:
		ASSERT(0);
		break;
	}

	// all else
	return _T("");
}

double SEARCHPARAM::ValueAsDouble() const
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
	case FT_COLOR:
		return (double)nValue;
	}

	// all else
	ASSERT(0);
	return 0.0;
}

int SEARCHPARAM::ValueAsInteger() const
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
	case FT_COLOR:
		return nValue;
	}

	// all else
	ASSERT(0);
	return 0;
}

COleDateTime SEARCHPARAM::ValueAsDate() const
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

///////////////////////////////////////////////////////////////////////

BOOL CSearchParamArray::IsValid() const
{
	int nNumBegin, nNumEnd;
	CountGroupings(nNumBegin, nNumEnd);

	return ((nNumBegin == nNumEnd) && (GetSize() > (nNumBegin + nNumEnd)));
}

BOOL CSearchParamArray::IsBalanced() const
{
	int nNumBegin, nNumEnd;
	CountGroupings(nNumBegin, nNumEnd);

	return (nNumEnd == nNumBegin);
}

void CSearchParamArray::CountGroupings(int& nNumBegin, int& nNumEnd) const
{
	nNumBegin = nNumEnd = 0;

	int nNumRules = GetSize();

	for (int nRule = 0; nRule < nNumRules; nRule++)
	{
		switch (GetAt(nRule).GetAttribute())
		{
		case TDCA_MATCHGROUPSTART:	nNumBegin++;	break;
		case TDCA_MATCHGROUPEND:	nNumEnd++;		break;
		}
	}
}

BOOL CSearchParamArray::IsStartOfGroup(int nRule) const
{
	return (GetAt(nRule).GetAttribute() == TDCA_MATCHGROUPSTART);
}

BOOL CSearchParamArray::IsLastRule(int nRule) const
{
	return (nRule >= (GetSize() - 1));
}

BOOL CSearchParamArray::IsLastRuleInGroup(int nRule) const
{
	if (IsLastRule(nRule))
		return FALSE;

	return (GetAt(nRule + 1).GetAttribute() == TDCA_MATCHGROUPEND);
}

BOOL CSearchParamArray::RuleSupportsAndOr(int nRule) const
{
	return !(IsStartOfGroup(nRule) || IsLastRule(nRule) || IsLastRuleInGroup(nRule));
}

///////////////////////////////////////////////////////////////////////

SEARCHPARAMS::SEARCHPARAMS()
{
	Clear();
}

SEARCHPARAMS::SEARCHPARAMS(const SEARCHPARAMS& params)
{
	*this = params;
}

SEARCHPARAMS& SEARCHPARAMS::operator=(const SEARCHPARAMS& params)
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

BOOL SEARCHPARAMS::operator!=(const SEARCHPARAMS& params) const
{
	return !(*this == params);
}

BOOL SEARCHPARAMS::operator==(const SEARCHPARAMS& params) const
{
	return (Misc::MatchAllT(aRules, params.aRules, TRUE) &&
			Misc::MatchAllT(aAttribDefs, params.aAttribDefs, FALSE) &&
			(bIgnoreDone == params.bIgnoreDone) &&
			(bIgnoreOverDue == params.bIgnoreOverDue) &&
			(bIgnoreFilteredOut == params.bIgnoreFilteredOut) &&
			(bWantAllSubtasks == params.bWantAllSubtasks) &&
			(bCaseSensitive == params.bCaseSensitive));
}

void SEARCHPARAMS::Clear()
{
	bIgnoreDone = FALSE;
	bIgnoreOverDue = FALSE;
	bWantAllSubtasks = FALSE;
	bIgnoreFilteredOut = TRUE;
	bCaseSensitive = FALSE;

	aRules.RemoveAll();
	aAttribDefs.RemoveAll();
}

BOOL SEARCHPARAMS::HasAttribute(TDC_ATTRIBUTE nAttribID) const
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

BOOL SEARCHPARAMS::HasRule(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp) const
{
	int nRule = aRules.GetSize();

	while (nRule--)
	{
		const SEARCHPARAM& rule = aRules[nRule];

		if (rule.AttributeIs(nAttribID) && rule.OperatorIs(nOp))
			return TRUE;
	}

	return FALSE;
}

BOOL SEARCHPARAMS::HasMultipleAttributes() const
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

BOOL SEARCHPARAMS::GetRuleCount() const
{
	return aRules.GetSize();
}

void SEARCHPARAMS::InitAttributeMap() const
{
	if (mapAttrib.GetCount() == 0)
	{
		int nRule = aRules.GetSize();

		while (nRule--)
		{
			const SEARCHPARAM& rule = aRules[nRule];

			if (rule.GetAttribType() != FT_GROUP)
				mapAttrib.Add(rule.nAttributeID);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////

SEARCHRESULT::SEARCHRESULT() 
	: 
	dwTaskID(0), 
	dwFlags(0) 
{
}

SEARCHRESULT::SEARCHRESULT(const SEARCHRESULT& res)
{
	*this = res;
}

SEARCHRESULT& SEARCHRESULT::operator=(const SEARCHRESULT& res)
{
	dwTaskID = res.dwTaskID;
	Misc::CopyT<TDC_ATTRIBUTE, CString>(res.mapMatched, mapMatched);
	dwFlags = res.dwFlags;

	return *this;
}

BOOL SEARCHRESULT::HasFlag(DWORD dwFlag) const
{
	return ((dwFlags & dwFlag) == dwFlag) ? 1 : 0;
}

BOOL SEARCHRESULT::GetWhatMatched(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aAttribDefs, CString& sWhatMatched) const
{
	// RECURSIVE CALLS
	switch (nAttribID)
	{
	case TDCA_TASKNAMEORCOMMENTS:
		return (GetWhatMatched(TDCA_TASKNAME, aAttribDefs, sWhatMatched) ||
				GetWhatMatched(TDCA_COMMENTS, aAttribDefs, sWhatMatched));

	case TDCA_ANYTEXTATTRIBUTE:
		{
			if (GetWhatMatched(TDCA_TASKNAME, aAttribDefs, sWhatMatched) ||
				GetWhatMatched(TDCA_COMMENTS, aAttribDefs, sWhatMatched) ||
				GetWhatMatched(TDCA_STATUS, aAttribDefs, sWhatMatched) ||
				GetWhatMatched(TDCA_CATEGORY, aAttribDefs, sWhatMatched) ||
				GetWhatMatched(TDCA_ALLOCBY, aAttribDefs, sWhatMatched) ||
				GetWhatMatched(TDCA_ALLOCTO, aAttribDefs, sWhatMatched) ||
				GetWhatMatched(TDCA_VERSION, aAttribDefs, sWhatMatched) ||
				GetWhatMatched(TDCA_TAGS, aAttribDefs, sWhatMatched) ||
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

/////////////////////////////////////////////////////////////////////////////////////////////

TDCFILTER::TDCFILTER()
	:
	nShow(FS_ALL),
	nStartBy(FD_ANY),
	nDueBy(FD_ANY),
	nPriority(FM_ANYPRIORITYORRISK),
	nRisk(FM_ANYPRIORITYORRISK),
	dwFlags(FO_ATTRIBUTES),
	nTitleOption(FT_FILTERONTITLEONLY),
	nStartNextNDays(7),
	nDueNextNDays(7),
	nRecurrence(TDIR_NONE)
{
	dtUserStart = dtUserDue = COleDateTime::GetCurrentTime();
}

TDCFILTER::TDCFILTER(const TDCFILTER& filter)
{
	*this = filter;
}

TDCFILTER& TDCFILTER::operator=(const TDCFILTER& filter)
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

BOOL TDCFILTER::operator==(const TDCFILTER& filter) const
{
	return FiltersMatch(*this, filter);
}

BOOL TDCFILTER::operator!=(const TDCFILTER& filter) const
{
	return !FiltersMatch(*this, filter);
}

BOOL TDCFILTER::IsSet() const
{
	// Compare to empty filter
	return !FiltersMatch(*this, TDCFILTER());
}

BOOL TDCFILTER::IsAdvanced() const 
{ 
	return (nShow == FS_ADVANCED); 
}

BOOL TDCFILTER::WantHideCompletedTasks() const
{
	switch (nShow)
	{
	case FS_DONE:
		return FALSE; // Always

	case FS_NOTDONE:
		return TRUE; // Always

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

BOOL TDCFILTER::HasAttribute(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const
{
	switch (nAttribID)
	{
	case TDCA_ALL:
		return TRUE; // More detailed check done elsewhere

	case TDCA_TASKNAME:
		return !sTitle.IsEmpty();

	case TDCA_PRIORITY:
		return (nPriority != FM_ANYPRIORITYORRISK);

	case TDCA_RISK:
		return (nRisk != FM_ANYPRIORITYORRISK);

	case TDCA_FLAG:
		return (nShow == FS_FLAGGED);

	case TDCA_LOCK:
		return (nShow == FS_LOCKED);

	case TDCA_DEPENDENCY:
		return HasFlag(FO_HIDEUNDONEDEPENDS);

	case TDCA_ALLOCBY:
		return (aAllocBy.GetSize() > 0);

	case TDCA_STATUS:
		return (aStatus.GetSize() > 0);

	case TDCA_VERSION:
		return (aVersions.GetSize() > 0);

	case TDCA_CATEGORY:
		return (aCategories.GetSize() > 0);

	case TDCA_TAGS:
		return (aTags.GetSize() > 0);

	case TDCA_ALLOCTO:
		return (aAllocTo.GetSize() > 0);

	case TDCA_PERCENT:
		return ((nShow == FS_DONE) || (nShow == FS_NOTDONE));

	case TDCA_DONEDATE:
		// changing the DONE date requires refiltering if:
		return
			// 1. The user wants to hide completed tasks
			(HasFlag(FO_HIDEDONE) ||
			 // 2. OR the user wants only tasks with completed dependencies
			 HasFlag(FO_HIDEUNDONEDEPENDS) ||
			 // 3. OR the user wants only completed tasks
			 (nShow == FS_DONE) ||
			 // 4. OR the user wants only incomplete tasks
			 (nShow == FS_NOTDONE) ||
			 // 5. OR a due date filter is active
			 (nDueBy != FD_ANY) ||
			 // 6. OR a start date filter is active
			 (nStartBy != FD_ANY) ||
			 // 7. OR the user is filtering on priority
			 (nPriority > 0));

	case TDCA_DUEDATE:
		// changing the DUE date requires refiltering if:
		return
			// 1. The user wants to hide overdue tasks
			((HasFlag(FO_HIDEOVERDUE) ||
			  // 2. OR the user is filtering on priority
			  (nPriority > 0) ||
			  // 3. OR a due date filter is active
			  (nDueBy != FD_ANY)) &&
			  // 4. AND the user doesn't want only completed tasks
			  (nShow != FS_DONE));

	case TDCA_STARTDATE:
		// changing the START date requires refiltering if:
		return
			// 1. A start date filter is active
			((nStartBy != FD_ANY) &&
			 // 2. AND the user doesn't want only completed tasks
			 (nShow != FS_DONE));

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			CString sAttribID = aCustomAttribDefs.GetAttributeTypeID(nAttribID);
			return mapCustomAttrib.HasKey(sAttribID);
		}
		break;
	}

	// all else
	return FALSE;
}

BOOL TDCFILTER::HasNowFilter(TDC_ATTRIBUTE& nAttrib) const
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

DWORD TDCFILTER::GetFlags() const 
{ 
	return dwFlags; 
}

BOOL TDCFILTER::HasFlag(DWORD dwFlag) const
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
			bHasFlag = ((nDueBy == FD_TODAY) ||
						(nDueBy == FD_YESTERDAY) ||
						(nDueBy == FD_TOMORROW) ||
						(nDueBy == FD_NEXTNDAYS) ||
						(nDueBy == FD_ENDTHISWEEK) ||
						(nDueBy == FD_ENDNEXTWEEK) ||
						(nDueBy == FD_ENDTHISMONTH) ||
						(nDueBy == FD_ENDNEXTMONTH) ||
						(nDueBy == FD_ENDTHISYEAR) ||
						(nDueBy == FD_ENDNEXTYEAR));
			break;

		case FO_HIDEDONE:
			bHasFlag = ((nShow != FS_NOTDONE) && 
						(nShow != FS_DONE));
			break;

		case FO_HIDEUNDONEDEPENDS:
			// TODO
			break;

		default:
			ASSERT(0);
			break;
		}
	}

	return bHasFlag;
}

void TDCFILTER::SetFlag(DWORD dwFlag, BOOL bOn)
{
	if (bOn)
		dwFlags |= dwFlag;
	else
		dwFlags &= ~dwFlag;
}

void TDCFILTER::Reset(FILTER_SHOW nInit)
{
	*this = TDCFILTER(); // empty filter

	nShow = nInit;
}

BOOL TDCFILTER::Matches(const TDCFILTER& filter) const
{
	return FiltersMatch(*this, filter);
}

CString TDCFILTER::GetTitleFilterLabel() const
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

BOOL TDCFILTER::FiltersMatch(const TDCFILTER& filter1, const TDCFILTER& filter2)
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

//////////////////////////////////////////////////////////////////////////////

TDCADVANCEDFILTER::TDCADVANCEDFILTER(LPCTSTR szName, DWORD flags) 
	: 
	sName(szName), 
	dwFlags(flags)
{
}

TDCADVANCEDFILTER::TDCADVANCEDFILTER(const TDCADVANCEDFILTER& filter)
{
	*this = filter;
}

TDCADVANCEDFILTER& TDCADVANCEDFILTER::operator=(const TDCADVANCEDFILTER& filter)
{
	params = filter.params;
	dwFlags = filter.dwFlags;
	sName = filter.sName;

	return *this;
}

BOOL TDCADVANCEDFILTER::operator==(const TDCADVANCEDFILTER& filter) const
{
	return Matches(filter);
}

BOOL TDCADVANCEDFILTER::operator!=(const TDCADVANCEDFILTER& filter) const
{
	return !Matches(filter);
}

BOOL TDCADVANCEDFILTER::IsSet(DWORD dwIgnore) const
{
	TDCADVANCEDFILTER filterEmpty;

	return !Matches(filterEmpty, dwIgnore);
}

BOOL TDCADVANCEDFILTER::HasNowFilter(TDC_ATTRIBUTE& nAttrib) const
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

DWORD TDCADVANCEDFILTER::GetFlags() const 
{ 
	return dwFlags; 
}

BOOL TDCADVANCEDFILTER::HasFlag(DWORD dwFlag) const
{
	return Misc::HasFlag(dwFlags, dwFlag);
}

void TDCADVANCEDFILTER::Reset()
{
	params.Clear();
	sName.Empty();
	dwFlags = 0;
}

BOOL TDCADVANCEDFILTER::Matches(const TDCADVANCEDFILTER& filter, DWORD dwIgnoreFlags) const
{
	if (!Matches(filter.sName, filter.dwFlags, dwIgnoreFlags))
		return FALSE;

	return (params == filter.params);
}

BOOL TDCADVANCEDFILTER::Matches(LPCTSTR szCustom, DWORD dwCustomFlags, DWORD dwIgnoreFlags) const
{
	return ((sName == szCustom) && ((dwFlags & ~dwIgnoreFlags) == (dwCustomFlags & ~dwIgnoreFlags)));
}

/////////////////////////////////////////////////////////////////////////////////////////////

TDCCOLEDITVISIBILITY::TDCCOLEDITVISIBILITY()
	:
	nShowFields(TDLSA_ASCOLUMN),
	bShowColorEditIfAsColumns(FALSE)
{
}

TDCCOLEDITVISIBILITY::TDCCOLEDITVISIBILITY(const TDCCOLEDITVISIBILITY& vis)
{
	*this = vis;
}

TDCCOLEDITVISIBILITY& TDCCOLEDITVISIBILITY::operator=(const TDCCOLEDITVISIBILITY& vis)
{
	mapVisibleColumns.Copy(vis.mapVisibleColumns);
	mapVisibleEdits.Copy(vis.mapVisibleEdits);

	nShowFields = vis.nShowFields;
	bShowColorEditIfAsColumns = vis.bShowColorEditIfAsColumns;

	return *this;
}

BOOL TDCCOLEDITVISIBILITY::operator!=(const TDCCOLEDITVISIBILITY& vis) const
{
	return !(*this == vis);
}

BOOL TDCCOLEDITVISIBILITY::operator==(const TDCCOLEDITVISIBILITY& vis) const
{
	return (mapVisibleColumns.MatchAll(vis.mapVisibleColumns) &&
			mapVisibleEdits.MatchAll(vis.mapVisibleEdits) &&
			(nShowFields == vis.nShowFields) &&
			(bShowColorEditIfAsColumns == vis.bShowColorEditIfAsColumns));
}

void TDCCOLEDITVISIBILITY::Clear()
{
	mapVisibleColumns.RemoveAll();
	mapVisibleEdits.RemoveAll();

	nShowFields = TDLSA_ASCOLUMN;
	bShowColorEditIfAsColumns = FALSE;
}

BOOL TDCCOLEDITVISIBILITY::HasDifferences(const TDCCOLEDITVISIBILITY& vis,
										  BOOL& bColumnChange, 
										  BOOL& bEditChange) const
{
	bColumnChange = !mapVisibleColumns.MatchAll(vis.mapVisibleColumns);
	bEditChange = !mapVisibleEdits.MatchAll(vis.mapVisibleEdits);

	return (bEditChange || bColumnChange || (nShowFields != vis.nShowFields));
}

TDL_SHOWATTRIB TDCCOLEDITVISIBILITY::GetShowFields() const
{
	return nShowFields;
}

void TDCCOLEDITVISIBILITY::SetShowFields(TDL_SHOWATTRIB nShow)
{
	nShowFields = nShow;

	UpdateEditVisibility();
}

void TDCCOLEDITVISIBILITY::ShowColorEditIfAsColumns(BOOL bShow)
{
	bShowColorEditIfAsColumns = bShow;

	UpdateEditVisibility();
}

const CTDCColumnIDMap& TDCCOLEDITVISIBILITY::GetVisibleColumns() const
{
	return mapVisibleColumns;
}

void TDCCOLEDITVISIBILITY::SetVisibleColumns(const CTDCColumnIDMap& mapColumns)
{
	mapVisibleColumns.Copy(mapColumns);
}

void TDCCOLEDITVISIBILITY::SetAllColumnsVisible(BOOL bVisible)
{
	mapVisibleColumns.RemoveAll();

	for (int nCol = TDCC_FIRST; nCol < TDCC_COUNT; nCol++)
	{
		if (bVisible && IsSupportedColumn((TDC_COLUMN)nCol))
			mapVisibleColumns.Add((TDC_COLUMN)nCol);
	}

	UpdateEditVisibility();
}

BOOL TDCCOLEDITVISIBILITY::SetAllFieldsVisible(BOOL bVisible)
{
	ASSERT(nShowFields == TDLSA_ANY);

	if (nShowFields != TDLSA_ANY)
		return FALSE;

	mapVisibleEdits.RemoveAll();

	if (bVisible)
		GetAllEditFields(mapVisibleEdits);

	return TRUE;
}

const CTDCAttributeMap& TDCCOLEDITVISIBILITY::GetVisibleEditFields() const
{
	return mapVisibleEdits;
}

BOOL TDCCOLEDITVISIBILITY::SetVisibleEditFields(const CTDCAttributeMap& mapAttrib)
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

BOOL TDCCOLEDITVISIBILITY::IsEditFieldVisible(TDC_ATTRIBUTE nAttribID) const
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

BOOL TDCCOLEDITVISIBILITY::IsColumnVisible(TDC_COLUMN nColID) const
{
	if (!IsSupportedColumn(nColID))
		return FALSE;

	// else
	return mapVisibleColumns.Has(nColID);
}

BOOL TDCCOLEDITVISIBILITY::SetEditFieldVisible(TDC_ATTRIBUTE nAttribID, BOOL bVisible)
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

void TDCCOLEDITVISIBILITY::SetColumnVisible(TDC_COLUMN nColID, BOOL bVisible)
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

void TDCCOLEDITVISIBILITY::Save(IPreferences* pPrefs, LPCTSTR szKey) const
{
	pPrefs->WriteProfileInt(szKey, _T("ShowAttributes"), nShowFields);

	mapVisibleColumns.Save(pPrefs, Misc::MakeKey(_T("%s\\ColumnVisibility"), szKey));

	if (nShowFields == TDLSA_ANY)
		mapVisibleEdits.Save(pPrefs, Misc::MakeKey(_T("%s\\EditVisibility"), szKey));
}

BOOL TDCCOLEDITVISIBILITY::Load(const IPreferences* pPrefs, LPCTSTR szKey)
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

BOOL TDCCOLEDITVISIBILITY::IsSupportedEdit(TDC_ATTRIBUTE nAttribID)
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
	case TDCA_REMINDER:
		return TRUE;
	}

	// all else
	return FALSE;
}

BOOL TDCCOLEDITVISIBILITY::IsSupportedFilter(TDC_ATTRIBUTE nAttribID)
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

BOOL TDCCOLEDITVISIBILITY::IsSupportedColumn(TDC_COLUMN nColumn)
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

int TDCCOLEDITVISIBILITY::UpdateEditVisibility()
{
	if (nShowFields != TDLSA_ANY)
	{
		mapVisibleEdits.RemoveAll();

		for (int nAttrib = 0; nAttrib < ATTRIB_COUNT; nAttrib++)
		{
			if (IsEditFieldVisible(TASKATTRIBUTES[nAttrib].nAttributeID))
				mapVisibleEdits.Add(TASKATTRIBUTES[nAttrib].nAttributeID);
		}
	}

	return mapVisibleEdits.GetCount();
}

int TDCCOLEDITVISIBILITY::GetAllEditFields(CTDCAttributeMap& mapAttrib)
{
	mapAttrib.RemoveAll();

	for (int nAttrib = 0; nAttrib < ATTRIB_COUNT; nAttrib++)
	{
		if (IsSupportedEdit(TASKATTRIBUTES[nAttrib].nAttributeID))
			mapAttrib.Add(TASKATTRIBUTES[nAttrib].nAttributeID);
	}

	return mapAttrib.GetCount();
}

////////////////////////////////////////////////////////////////////////////

TDCCOLEDITFILTERVISIBILITY::TDCCOLEDITFILTERVISIBILITY()
{
}

TDCCOLEDITFILTERVISIBILITY::TDCCOLEDITFILTERVISIBILITY(const TDCCOLEDITFILTERVISIBILITY& vis)
{
	*this = vis;
}

TDCCOLEDITFILTERVISIBILITY& TDCCOLEDITFILTERVISIBILITY::operator=(const TDCCOLEDITFILTERVISIBILITY& vis)
{
	TDCCOLEDITVISIBILITY::operator=(vis);
	mapVisibleFilters.Copy(vis.mapVisibleFilters);

	return *this;
}

BOOL TDCCOLEDITFILTERVISIBILITY::operator!=(const TDCCOLEDITFILTERVISIBILITY& vis) const
{
	return !(*this == vis);
}

BOOL TDCCOLEDITFILTERVISIBILITY::operator==(const TDCCOLEDITFILTERVISIBILITY& vis) const
{
	return (TDCCOLEDITVISIBILITY::operator==(vis) &&
			mapVisibleFilters.MatchAll(vis.mapVisibleFilters));
}

void TDCCOLEDITFILTERVISIBILITY::Clear()
{
	TDCCOLEDITVISIBILITY::Clear();

	mapVisibleFilters.RemoveAll();
}

BOOL TDCCOLEDITFILTERVISIBILITY::HasDifferences(const TDCCOLEDITFILTERVISIBILITY& vis,
												BOOL& bColumnChange, 
												BOOL& bEditChange, 
												BOOL& bFilterChange) const
{
	TDCCOLEDITVISIBILITY::HasDifferences(vis, bColumnChange, bEditChange);
	bFilterChange = !mapVisibleFilters.MatchAll(vis.mapVisibleFilters);

	return (bEditChange || bColumnChange || bFilterChange);
}

void TDCCOLEDITFILTERVISIBILITY::SetShowFields(TDL_SHOWATTRIB nShow)
{
	TDCCOLEDITVISIBILITY::SetShowFields(nShow);

	UpdateFilterVisibility();
}

void TDCCOLEDITFILTERVISIBILITY::SetAllColumnsVisible(BOOL bVisible)
{
	TDCCOLEDITVISIBILITY::SetAllColumnsVisible(bVisible);

	UpdateFilterVisibility();
}

BOOL TDCCOLEDITFILTERVISIBILITY::SetAllFieldsVisible(BOOL bVisible)
{
	if (!TDCCOLEDITVISIBILITY::SetAllFieldsVisible(bVisible))
		return FALSE;

	mapVisibleFilters.RemoveAll();

	if (bVisible)
		GetAllFilterFields(mapVisibleFilters);

	return TRUE;
}

const CTDCAttributeMap& TDCCOLEDITFILTERVISIBILITY::GetVisibleFilterFields() const
{
	return mapVisibleFilters;
}

BOOL TDCCOLEDITFILTERVISIBILITY::SetVisibleFilterFields(const CTDCAttributeMap& mapAttrib)
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

BOOL TDCCOLEDITFILTERVISIBILITY::IsFilterFieldVisible(TDC_ATTRIBUTE nAttribID) const
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

BOOL TDCCOLEDITFILTERVISIBILITY::SetFilterFieldVisible(TDC_ATTRIBUTE nAttribID, BOOL bVisible)
{
	// only supported for 'any' attribute
	ASSERT(nShowFields == TDLSA_ANY);

	if (nShowFields != TDLSA_ANY)
		return FALSE;

	// validate attribute
	ASSERT(IsSupportedFilter(nAttribID));

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

void TDCCOLEDITFILTERVISIBILITY::Save(IPreferences* pPrefs, LPCTSTR szKey) const
{
	TDCCOLEDITVISIBILITY::Save(pPrefs, szKey);

	if (nShowFields == TDLSA_ANY)
		mapVisibleFilters.Save(pPrefs, Misc::MakeKey(_T("%s\\FilterVisibility"), szKey));
}

BOOL TDCCOLEDITFILTERVISIBILITY::Load(const IPreferences* pPrefs, LPCTSTR szKey)
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

BOOL TDCCOLEDITFILTERVISIBILITY::IsSupportedFilter(TDC_ATTRIBUTE nAttribID)
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

int TDCCOLEDITFILTERVISIBILITY::UpdateFilterVisibility()
{
	if (nShowFields != TDLSA_ANY)
	{
		mapVisibleFilters.RemoveAll();

		for (int nAttrib = 0; nAttrib < ATTRIB_COUNT; nAttrib++)
		{
			if (IsFilterFieldVisible(TASKATTRIBUTES[nAttrib].nAttributeID))
				mapVisibleFilters.Add(TASKATTRIBUTES[nAttrib].nAttributeID);
		}
	}

	return mapVisibleFilters.GetCount();
}

int TDCCOLEDITFILTERVISIBILITY::GetAllFilterFields(CTDCAttributeMap& mapAttrib)
{
	mapAttrib.RemoveAll();

	for (int nAttrib = 0; nAttrib < ATTRIB_COUNT; nAttrib++)
	{
		if (IsSupportedFilter(TASKATTRIBUTES[nAttrib].nAttributeID))
			mapAttrib.Add(TASKATTRIBUTES[nAttrib].nAttributeID);
	}

	return mapAttrib.GetCount();
}

/////////////////////////////////////////////////////////////////////////////

TDCATTRIBUTEMAPPING::TDCATTRIBUTEMAPPING() 
	: 
	nAttributeID(TDCA_NONE),
	dwItemData(0)
{
}

TDCATTRIBUTEMAPPING::TDCATTRIBUTEMAPPING(const CString& sName, TDC_ATTRIBUTE nAttribID, DWORD dwData)
{
	sColumnName = sName;
	nAttributeID = nAttribID;
	dwItemData = dwData;
}

TDCATTRIBUTEMAPPING::TDCATTRIBUTEMAPPING(UINT nNameID, TDC_ATTRIBUTE nAttribID, DWORD dwData)
{
	sColumnName.LoadString(nNameID);
	nAttributeID = nAttribID;
	dwItemData = dwData;
}

/////////////////////////////////////////////////////////////////////////////

int CTDCAttributeMapping::Find(const CString& sCol) const
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

int CTDCAttributeMapping::Find(TDC_ATTRIBUTE nAttribID) const
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

int CTDCAttributeMapping::Find(DWORD dwData) const
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

int CTDCAttributeMapping::FindMappedAttribute(TDC_ATTRIBUTE nAttribID) const
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

void CTDCAttributeMapping::Trace() const
{
#ifdef _DEBUG
	for (int nMap = 0; nMap < GetSize(); nMap++)
	{
		const TDCATTRIBUTEMAPPING& col = GetData()[nMap];

		TRACE(_T("CTDCAttributeMapping::Trace(%s, %d, %d)\n"), col.sColumnName, col.nAttributeID, col.dwItemData);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
