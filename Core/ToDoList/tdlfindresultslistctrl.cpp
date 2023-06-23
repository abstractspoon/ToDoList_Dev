// tdlfindresultslistctrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdlfindresultslistctrl.h"
#include "FilteredToDoCtrl.h"
#include "tdcstatic.h"

#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\osversion.h"

#include "..\3rdparty\shellicons.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP     0x00004000
#endif

enum
{
	COL_TASKTITLE,
	COL_WHATMATCHED,
	COL_TASKPATH
};

/////////////////////////////////////////////////////////////////////////////
// CTDLFindResultsListCtrl

CTDLFindResultsListCtrl::CTDLFindResultsListCtrl() 
	: 
	m_nCurGroupID(-1), 
	m_bStrikeThruDone(FALSE), 
	m_lcGrouping(*this)
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
	InsertColumn(COL_TASKTITLE, CEnString(IDS_FT_TASK), LVCFMT_LEFT, 250);
	InsertColumn(COL_WHATMATCHED, CEnString(IDS_FT_WHATMATCHED), LVCFMT_LEFT, 150);
	InsertColumn(COL_TASKPATH, CEnString(IDS_FT_PATH), LVCFMT_LEFT, 100);

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

	m_nCurGroupID = -1;
	m_lcGrouping.RemoveAllGroups();
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
				GraphicsMisc::DrawExplorerItemSelection(pDC, *this, nState, rRow, GMIB_THEMECLASSIC);
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
				CString sColText = GetItemText(nItem, nSubItem);

				if (!sColText.IsEmpty())
				{
					CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
					BOOL bHot = IsResultHot(pLVCD->nmcd.rc);
					BOOL bSelected = IsItemSelected(nItem);

					COLORREF crText = GetResultTextColor(pRes, bSelected, bHot);
					pDC->SetTextColor(crText);

					// set the font for each column item
					CFont* pFont = GetResultFont(pRes, nSubItem, bHot);
					ASSERT(pFont);

					if (pFont)
					{
						::SelectObject(pLVCD->nmcd.hdc, pFont->GetSafeHandle());
						*pResult = CDRF_NEWFONT;
					}

					CRect rRow(pLVCD->nmcd.rc);
				
					// extra for XP
					if (OsIsXP())
						GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rRow);

					int nFlags = (DT_SINGLELINE | DT_NOPREFIX | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);

					if ((nSubItem == COL_TASKTITLE) && pRes->IsReference())
					{
						// Offset the task title to avoid the reference icon
						rRow.left += GraphicsMisc::ScaleByDPIFactor(10);
						pDC->DrawText(sColText, rRow, nFlags);
						
						if (pLVCD->nmcd.rc.top > 0)
							GraphicsMisc::DrawShortcutOverlay(pDC, &pLVCD->nmcd.rc);
					}
					else
					{
						pDC->DrawText(sColText, rRow, nFlags);
					}
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

CFont* CTDLFindResultsListCtrl::GetResultFont(const FTDRESULT* pRes, int nCol, BOOL bHot)
{
	ASSERT(pRes);

	if (pRes)
	{
		if (!m_fonts.GetHwnd() && !m_fonts.Initialise(*this))
			return NULL;

		BOOL bStrikeThru = (m_bStrikeThruDone && pRes->IsDone());
		BOOL bUnderline = bHot;
		BOOL bBold = (nCol == COL_TASKTITLE) ? pRes->IsTopmost() : FALSE;

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
		m_crDone = (COLORREF)prefs.GetProfileInt(_T("Preferences\\Colors"), _T("TaskDone"), CLR_NONE);
	else
		m_crDone = CLR_NONE;
	
	// update user reference tasks colour
	if (prefs.GetProfileInt(_T("Preferences"), _T("ReferenceColor"), FALSE))
		m_crRef = (COLORREF)prefs.GetProfileInt(_T("Preferences\\Colors"), _T("Reference"), CLR_NONE);
	else
		m_crRef = CLR_NONE;

	// update strike thru font
	BOOL bWasStrikeThru = m_bStrikeThruDone;
	m_bStrikeThruDone = prefs.GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), FALSE);

	// clear the font cache if 'strike thru' has changed
	if (Misc::StateChanged(m_bStrikeThruDone, bWasStrikeThru))
		m_fonts.Clear();

	if (IsWindowVisible())
		Invalidate();
}

int CTDLFindResultsListCtrl::AddResult(const SEARCHRESULT& result, const CFilteredToDoCtrl* pTDC, BOOL bShowValueOnly)
{
	int nPos = GetItemCount();
	CString sTitle = pTDC->GetTaskTitle(result.dwTaskID);
	CString sPath = pTDC->GetTaskPath(result.dwTaskID);

	// add result
	int nIndex = InsertItem(nPos, sTitle);
	
	SetItemText(nIndex, COL_WHATMATCHED, FormatWhatMatched(result, pTDC, bShowValueOnly));
	SetItemText(nIndex, COL_TASKPATH, sPath);

	if (m_nCurGroupID != -1)
		m_lcGrouping.SetItemGroupId(nIndex, m_nCurGroupID);

	UpdateWindow();
		
	// map identifying data
	FTDRESULT* pRes = new FTDRESULT(result, pTDC);
	SetItemData(nIndex, (DWORD)pRes);

	return nIndex;
}

BOOL CTDLFindResultsListCtrl::AddHeaderRow(LPCTSTR szText)
{
	if (m_nCurGroupID == -1)
		m_lcGrouping.EnableGroupView();

	return m_lcGrouping.InsertGroupHeader(-1, ++m_nCurGroupID, szText);
}

BOOL CTDLFindResultsListCtrl::OsIsXP()
{
	static BOOL bXP(COSVersion() < OSV_VISTA);
	return bXP;
}

CString CTDLFindResultsListCtrl::FormatWhatMatched(const SEARCHRESULT& result, const CFilteredToDoCtrl* pTDC, BOOL bShowValueOnly) const
{
	ASSERT(result.mapMatched.GetCount());

	DWORD dwTaskID = result.dwTaskID;
	POSITION pos = result.mapMatched.GetStartPosition();

	CStringArray aWhatMatched;

	while (pos)
	{
		TDC_ATTRIBUTE nAttribID;
		CString sWhatMatched, sFormatted;

		result.mapMatched.GetNextAssoc(pos, nAttribID, sWhatMatched);

		// We never show the values of 'Title' or 'Comments'
		BOOL bTitleOrComments = ((nAttribID == TDCA_TASKNAME) || (nAttribID == TDCA_COMMENTS));

		if (sWhatMatched.IsEmpty() && !bTitleOrComments)
			sWhatMatched = CEnString(IDS_ATTRIBNOTSET);
		
		if (bTitleOrComments)
		{
			sFormatted = GetAttributeName(nAttribID, pTDC);
		}
		else if (!bShowValueOnly)
		{
			sFormatted.Format(_T("%s (%s)"), GetAttributeName(nAttribID, pTDC), sWhatMatched);
		}
		else
		{
			sFormatted = sWhatMatched;
		}

		// Leave early if only one item matched
		if (result.mapMatched.GetCount() == 1)
			return sFormatted;

		aWhatMatched.Add(sFormatted);
	}

	return Misc::FormatArray(aWhatMatched);
}

CString CTDLFindResultsListCtrl::GetAttributeName(TDC_ATTRIBUTE nAttribID, const CFilteredToDoCtrl* pTDC) const
{
	CString sAttrib = TDC::GetAttributeName(nAttribID);

	if (sAttrib.IsEmpty())
	{
		// custom attributes
		for (int nCust = 0; nCust < pTDC->GetCustomAttributeDefs().GetSize(); nCust++)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = pTDC->GetCustomAttributeDefs()[nCust];

			if (attribDef.GetAttributeID() == nAttribID)
			{
				sAttrib = attribDef.sLabel;
				break;
			}
		}
	}

	ASSERT(!sAttrib.IsEmpty());
	return sAttrib;
}

int CTDLFindResultsListCtrl::CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn)
{
	// Handle date sorting
	if (nSortColumn == COL_WHATMATCHED)
	{
		COleDateTime date1, date2;
		
		BOOL bIsDate1 = CDateHelper::DecodeDate(GetSortString(dwItemData1), date1, TRUE);
		BOOL bIsDate2 = CDateHelper::DecodeDate(GetSortString(dwItemData2), date2, TRUE);

		if (bIsDate1 || bIsDate2)
			return CDateHelper::Compare(date1, date2);
	}

	// all else
	return CEnListCtrl::CompareItems(dwItemData1, dwItemData2, nSortColumn);
}
