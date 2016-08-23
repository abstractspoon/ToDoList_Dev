// tdlfindresultslistctrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdlfindresultslistctrl.h"

#include "..\shared\preferences.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\osversion.h"

#include "..\3rdparty\shellicons.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP     0x00004000
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLFindResultsListCtrl

CTDLFindResultsListCtrl::CTDLFindResultsListCtrl() : m_nCurGroupID(-1), m_bStrikeThruDone(FALSE) 
{
}

CTDLFindResultsListCtrl::~CTDLFindResultsListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLFindResultsListCtrl, CEnListCtrl)
	//{{AFX_MSG_MAP(CTDLFindResultsListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFindResultsListCtrl message handlers

void CTDLFindResultsListCtrl::PreSubclassWindow()
{
	CEnListCtrl::PreSubclassWindow();

	// setup up result list
	InsertColumn(0, CEnString(IDS_FT_TASK), LVCFMT_LEFT, 250);
	InsertColumn(1, CEnString(IDS_FT_WHATMATCHED), LVCFMT_LEFT, 150);
	InsertColumn(2, CEnString(IDS_FT_PATH), LVCFMT_LEFT, 100);

	ListView_SetExtendedListViewStyleEx(*this, LVS_EX_ONECLICKACTIVATE, LVS_EX_ONECLICKACTIVATE);
	ListView_SetExtendedListViewStyleEx(*this, LVS_EX_UNDERLINEHOT, LVS_EX_UNDERLINEHOT);
	ListView_SetExtendedListViewStyleEx(*this, LVS_EX_LABELTIP, LVS_EX_LABELTIP);
	ListView_SetExtendedListViewStyleEx(*this, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	RefreshUserPreferences();
}

int CTDLFindResultsListCtrl::GetColumnWidths(CIntArray& aWidths) const
{
	const CEnHeaderCtrl* pHeader = GetHeader();
	ASSERT(pHeader);

	if (pHeader)
		return pHeader->GetItemWidths(aWidths);

	// else
	return 0;
}

BOOL CTDLFindResultsListCtrl::SetColumnWidths(const CIntArray& aWidths)
{
	CEnHeaderCtrl* pHeader = GetHeader();
	ASSERT(pHeader);

	if (pHeader)
		return pHeader->SetItemWidths(aWidths);

	// else
	return FALSE;

}

int CTDLFindResultsListCtrl::GetResultCount() const
{
	return GetResultCount(NULL);
}

int CTDLFindResultsListCtrl::GetResultCount(const CFilteredToDoCtrl* pTDC) const
{
	int nCount = 0;
	int nItem = GetItemCount();
	
	while (nItem--)
	{
		FTDRESULT* pRes = GetResult(nItem);

		if (pRes && (pTDC == NULL || pRes->pTDC == pTDC))
			nCount++;
	}
	
	return nCount;
}

int CTDLFindResultsListCtrl::GetAllResults(CFTDResultsArray& aResults) const
{
	return GetResults(NULL, aResults);
}

int CTDLFindResultsListCtrl::GetResults(const CFilteredToDoCtrl* pTDC, CFTDResultsArray& aResults) const
{
	int nNumItem = GetItemCount();
	int nCount = 0;

	aResults.RemoveAll();
	aResults.SetSize(GetResultCount(pTDC));

	for (int nItem = 0; nItem < nNumItem; nItem++)
	{
		FTDRESULT* pRes = GetResult(nItem);

		if (pRes && (pTDC == NULL || pRes->pTDC == pTDC))
		{
			aResults.SetAt(nCount, *pRes);
			nCount++;
		}
	}

	return aResults.GetSize();
}

int CTDLFindResultsListCtrl::GetResultIDs(const CFilteredToDoCtrl* pTDC, CDWordArray& aTaskIDs) const
{
	CFTDResultsArray aResults;
	int nNumRes = GetResults(pTDC, aResults);

	for (int nRes = 0; nRes < nNumRes; nRes++)
		aTaskIDs.Add(aResults[nRes].dwTaskID);

	return aResults.GetSize();
}

void CTDLFindResultsListCtrl::DeleteResults(const CFilteredToDoCtrl* pTDC)
{
	// work backwards from the last list res
	int nItem = GetItemCount();

	while (nItem--)
	{
		FTDRESULT* pRes = GetResult(nItem);

		if (pRes && pRes->pTDC == pTDC)
		{
			DeleteItem(nItem);
			delete pRes;
		}
	}
}

void CTDLFindResultsListCtrl::DeleteAllResults()
{
	// work backwards from the last list res
	int nItem = GetItemCount();

	while (nItem--)
	{
		FTDRESULT* pRes = GetResult(nItem);
		delete pRes;

		DeleteItem(nItem);
	}
}

BOOL CTDLFindResultsListCtrl::IsResultHot(const RECT& rResult) const
{
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	ptCursor.x = rResult.left; // we want the whole row

	return ::PtInRect(&rResult, ptCursor);
}

void CTDLFindResultsListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = CDRF_DODEFAULT;
	LPNMLVCUSTOMDRAW pLVCD = (LPNMLVCUSTOMDRAW)pNMHDR;

	int nItem = (int)pLVCD->nmcd.dwItemSpec;
	int nSubItem = (int)pLVCD->iSubItem;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = (CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT);
		break;

	case CDDS_ITEMPREPAINT:
		{
			const FTDRESULT* pRes = (FTDRESULT*)pLVCD->nmcd.lItemlParam;
			ASSERT(pRes);

			// background
			BOOL bHot = IsResultHot(pLVCD->nmcd.rc);
			BOOL bSelected = IsItemSelected(nItem);
			
			if (bSelected || bHot)
			{
				CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
				BOOL bFocused = (GetFocus() == this);

				CRect rRow(pLVCD->nmcd.rc);
				
				// extra for XP
				if (OsIsXP())
					GetItemRect(nItem, rRow, LVIR_BOUNDS);

				GM_ITEMSTATE nState = ((bFocused && bSelected) ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);
				GraphicsMisc::DrawExplorerItemBkgnd(pDC, *this, nState, rRow, GMIB_THEMECLASSIC);
			}
			
			// set the font once per item
			CFont* pFont = GetResultFont(pRes, bHot);
			ASSERT(pFont);

			if (pFont)
			{
				::SelectObject(pLVCD->nmcd.hdc, pFont->GetSafeHandle());
				*pResult = CDRF_NEWFONT;
			}

			// hide text because we will draw it in SUBITEMPREPAINT
			pLVCD->clrTextBk = pLVCD->clrText = GetSysColor(COLOR_WINDOW);
		}
		*pResult |= CDRF_NOTIFYSUBITEMDRAW;
		break;
		
	case (CDDS_ITEMPREPAINT | CDDS_SUBITEM):
		{
			const FTDRESULT* pRes = (FTDRESULT*)pLVCD->nmcd.lItemlParam;
			ASSERT(pRes);

			if (pRes)
			{
				CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
				BOOL bHot = IsResultHot(pLVCD->nmcd.rc);
				BOOL bSelected = IsItemSelected(nItem);

				COLORREF crText = GetResultTextColor(pRes, bSelected, bHot);
				pDC->SetTextColor(crText);

				CRect rRow(pLVCD->nmcd.rc);
				
				// extra for XP
				if (OsIsXP())
					GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rRow);

				pDC->DrawText(GetItemText(nItem, nSubItem), rRow, (DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS));

				// references
				if ((nSubItem == 0) && (pLVCD->nmcd.rc.top > 0) && pRes->IsReference())
				{
					ShellIcons::DrawIcon(pDC, ShellIcons::SI_SHORTCUT, CRect(pLVCD->nmcd.rc).TopLeft());
				}
			}

			*pResult = CDRF_SKIPDEFAULT;
		}
		break;

	default:
		break;
	}
}

COLORREF CTDLFindResultsListCtrl::GetResultTextColor(const FTDRESULT* pRes, BOOL /*bSelected*/, BOOL bHot) const
{
	ASSERT(pRes);

	COLORREF crText = CLR_NONE;

	if ((m_crDone != CLR_NONE) || (m_crRef != CLR_NONE))
	{
		if (pRes->IsDone() || pRes->IsGoodAsDone())
		{
			crText = m_crDone;
		}
		else if (pRes->IsReference())
		{
			crText = m_crRef;
		}

		// darken the colour for hot items
		if (bHot && (crText != CLR_NONE))
			crText = GraphicsMisc::Darker(crText, 0.3);
	}

	return ((crText == CLR_NONE) ? GetSysColor(COLOR_WINDOWTEXT) : crText);
}

CFont* CTDLFindResultsListCtrl::GetResultFont(const FTDRESULT* pRes, BOOL bHot)
{
	ASSERT(pRes);

	if (pRes)
	{
		if (!m_fonts.GetHwnd() && !m_fonts.Initialise(*this))
			return NULL;

		BOOL bStrikeThru = (m_bStrikeThruDone && pRes->IsDone());
		BOOL bBold = pRes->IsTopmost();
		BOOL bUnderline = bHot;

		return m_fonts.GetFont(bBold, FALSE, bUnderline, bStrikeThru);
	}

	ASSERT(0);
	return NULL;
}

void CTDLFindResultsListCtrl::RefreshUserPreferences()
{
	CPreferences prefs;
	
	// update user completed tasks colour
	if (prefs.GetProfileInt(_T("Preferences"), _T("SpecifyDoneColor"), FALSE))
		m_crDone = (COLORREF)prefs.GetProfileInt(_T("Preferences\\Colors"), _T("TaskDone"), -1);
	else
		m_crDone = CLR_NONE;
	
	// update user reference tasks colour
	if (prefs.GetProfileInt(_T("Preferences"), _T("ReferenceColor"), FALSE))
		m_crRef = (COLORREF)prefs.GetProfileInt(_T("Preferences\\Colors"), _T("Reference"), -1);
	else
		m_crRef = CLR_NONE;

	// update strike thru font
	BOOL bWasStrikeThru = m_bStrikeThruDone;
	m_bStrikeThruDone = prefs.GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), FALSE);

	// clear the font cache if 'strike thru' has changed
	if (m_bStrikeThruDone != bWasStrikeThru)
		m_fonts.Clear();

	if (IsWindowVisible())
		Invalidate();
}

int CTDLFindResultsListCtrl::AddResult(const SEARCHRESULT& result, LPCTSTR szTask, LPCTSTR szPath, const CFilteredToDoCtrl* pTDC)
{
	int nPos = GetItemCount();
		
	// add result
	int nIndex = InsertItem(nPos, szTask);
	SetItemText(nIndex, 1, Misc::FormatArray(result.aMatched));
	SetItemText(nIndex, 2, szPath);

	if (m_nCurGroupID != -1)
		SetItemGroupId(nIndex, m_nCurGroupID);

	UpdateWindow();
		
	// map identifying data
	FTDRESULT* pRes = new FTDRESULT(result, pTDC);
	SetItemData(nIndex, (DWORD)pRes);

	return nIndex;
}

BOOL CTDLFindResultsListCtrl::AddHeaderRow(LPCTSTR szText)
{
	if (m_nCurGroupID == -1)
		EnableGroupView();

	return InsertGroupHeader(-1, ++m_nCurGroupID, szText);
}

BOOL CTDLFindResultsListCtrl::OsIsXP()
{
	static BOOL bXP(COSVersion() < OSV_VISTA);
	return bXP;
}
