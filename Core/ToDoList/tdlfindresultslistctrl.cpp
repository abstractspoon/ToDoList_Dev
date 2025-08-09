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
#include "..\shared\DateHelper.h"

#include "..\3rdparty\shellicons.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

FTDRESULT::FTDRESULT() 
	: 
	dwTaskID(0), 
	pTDC(NULL), 
	dwFlags(0) 
{
}

FTDRESULT::FTDRESULT(const SEARCHRESULT& result, const CFilteredToDoCtrl* pTaskList)
	:
	dwTaskID(result.dwTaskID),
	pTDC(pTaskList),
	dwFlags(result.dwFlags)
{
}

int FTDRESULT::HasIcon() const 
{ 
	return (pTDC->GetTaskIconIndex(dwTaskID) != -1); 
}

void FTDRESULT::DrawIcon(CDC* pDC, const CRect& rText) const
{
	GraphicsMisc::DrawCentred(pDC,
							  pTDC->GetTaskIconImageList(),
							  pTDC->GetTaskIconIndex(dwTaskID),
							  rText,
							  FALSE,
							  TRUE); // vertically centred

	if (IsReference())
		GraphicsMisc::DrawShortcutOverlay(pDC, rText);
}

/////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_LABELTIP
#	define LVS_EX_LABELTIP	0x00004000
#endif

#ifndef LVN_ENDSCROLL
#	define LVN_ENDSCROLL	(LVN_FIRST - 81)

struct NMLVSCROLL
{
	NMHDR   hdr;
	int     dx;
	int     dy;
};
#endif

/////////////////////////////////////////////////////////////////////////////

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
	m_nHotItem(-1),
	m_bStrikeThruDone(FALSE),
	m_nTextOffset(0)
{
	SetMinItemHeight(GraphicsMisc::ScaleByDPIFactor(17));
}

CTDLFindResultsListCtrl::~CTDLFindResultsListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLFindResultsListCtrl, CEnListCtrl)
	//{{AFX_MSG_MAP(CTDLFindResultsListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_NOTIFY_REFLECT_EX(LVN_ENDSCROLL, OnVScroll)
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
	ListView_SetExtendedListViewStyleEx(*this, LVS_EX_LABELTIP, LVS_EX_LABELTIP);
	ListView_SetExtendedListViewStyleEx(*this, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
}

void CTDLFindResultsListCtrl::RecalcTextOffset()
{
	m_nTextOffset = 0;

	int nItem = GetItemCount();
	
	while (nItem-- && (m_nTextOffset == 0))
		m_nTextOffset = GetTextOffset(GetResult(nItem));
}

int CTDLFindResultsListCtrl::GetTextOffset(const FTDRESULT* pRes) const
{
	if (pRes && (pRes->HasIcon() || pRes->IsReference()))
		return (pRes->pTDC->GetTaskIconImageList().GetImageWidth() + 2);

	// else
	return NULL;
}

LRESULT CTDLFindResultsListCtrl::OnSetFont(WPARAM wp, LPARAM lp)
{
	if (wp)
		m_fonts.Initialise((HFONT)wp);
	else
		m_fonts.Initialise(GetSafeHwnd());

	return CEnListCtrl::OnSetFont(wp, lp);
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

		if (pRes && (pRes->pTDC == pTDC))
		{
			if (nItem == m_nHotItem)
				m_nHotItem = -1;

			DeleteItem(nItem);
			delete pRes;
		}
	}

	RecalcTextOffset();
}

void CTDLFindResultsListCtrl::DeleteAllResults()
{
	// work backwards from the last list res
	int nItem = GetItemCount();

	while (nItem--)
		delete GetResult(nItem);

	m_nTextOffset = 0;
	m_nCurGroupID = m_nHotItem = -1;

	DeleteAllItems();
}

void CTDLFindResultsListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	UpdateHotItem(point);
}

BOOL CTDLFindResultsListCtrl::OnVScroll(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVSCROLL* pScroll = (NMLVSCROLL*)pNMHDR;

	if (pScroll->dy)
	{
		CPoint point(GetMessagePos());
		ScreenToClient(&point);

		UpdateHotItem(point);
	}

	return FALSE;
}

void CTDLFindResultsListCtrl::UpdateHotItem(CPoint point)
{
	// Use drop-highlighting to mimic hot-ness
	int nHit = HitTest(point);

	if (nHit != m_nHotItem)
	{
		if (m_nHotItem != -1)
			SetItemState(m_nHotItem, 0, LVIS_DROPHILITED);

		if (nHit != -1)
			SetItemState(nHit, LVIS_DROPHILITED, LVIS_DROPHILITED);

		m_nHotItem = nHit;
	}
}

BOOL CTDLFindResultsListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT)
	{
		CPoint point(GetMessagePos());
		ScreenToClient(&point);

		if (HitTest(point) != -1)
			return GraphicsMisc::SetHandCursor();
	}

	// else
	return CEnListCtrl::OnSetCursor(pWnd, nHitTest, message);
}

COLORREF CTDLFindResultsListCtrl::GetItemTextColor(int nItem, int nSubItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	FTDRESULT* pRes = GetResult(nItem);
	ASSERT(pRes);

	if (pRes)
	{
		COLORREF crText, crUnused;
		
		if (pRes->pTDC->GetTaskTextColors(pRes->dwTaskID, crText, crUnused, (bSelected || bDropHighlighted)))
			return crText;
	}

	return CEnListCtrl::GetItemTextColor(nItem, nSubItem, bSelected, bDropHighlighted, bWndFocus);
}

COLORREF CTDLFindResultsListCtrl::GetItemBackColor(int nItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	FTDRESULT* pRes = GetResult(nItem);
	ASSERT(pRes);

	if (pRes)
	{
		COLORREF crUnused, crBack;

		if (pRes->pTDC->GetTaskTextColors(pRes->dwTaskID, crUnused, crBack, (bSelected || bDropHighlighted)))
			return crBack;
	}

	return CEnListCtrl::GetItemBackColor(nItem, bSelected, bDropHighlighted, bWndFocus);
}

void CTDLFindResultsListCtrl::DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags)
{
	if ((nCol != COL_TASKTITLE) || (m_nTextOffset == 0))
	{
		CEnListCtrl::DrawCellText(pDC, nItem, nCol, rText, sText, crText, nDrawTextFlags);
		return;
	}

	// else
	FTDRESULT* pRes = GetResult(nItem);
	ASSERT(pRes);

	if (pRes)
		pRes->DrawIcon(pDC, rText);

	CRect rRest(rText);
	rRest.left += m_nTextOffset;

	CEnListCtrl::DrawCellText(pDC, nItem, nCol, rRest, sText, crText, nDrawTextFlags);
}

void CTDLFindResultsListCtrl::DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused)
{
	if (m_nTextOffset == 0)
		CEnListCtrl::DrawItemBackground(pDC, nItem, rItem, crBack, bSelected, bDropHighlighted, bFocused);

	CRect rText(rItem);
	rText.left += m_nTextOffset;

	CEnListCtrl::DrawItemBackground(pDC, nItem, rText, crBack, bSelected, bDropHighlighted, bFocused);
}

CFont* CTDLFindResultsListCtrl::GetItemFont(int nItem, int nSubItem) const
{
	FTDRESULT* pRes = GetResult(nItem);
	ASSERT(pRes);

	if (pRes)
	{
		if (!m_fonts.GetHwnd() && !m_fonts.Initialise(*this))
			return NULL;

		BOOL bTitleCol = (nSubItem == COL_TASKTITLE);

		BOOL bUnderline = (bTitleCol ? (nItem == m_nHotItem) : FALSE);
		BOOL bStrikeThru = (m_bStrikeThruDone && pRes->IsDone());
		BOOL bBold = (bTitleCol ? pRes->IsTopmost() : FALSE);

		return m_fonts.GetFont(bBold, FALSE, bUnderline, bStrikeThru);
	}

	return CEnListCtrl::GetItemFont(nItem, nSubItem);
}

void CTDLFindResultsListCtrl::SetStrikeThroughCompletedTasks(BOOL bStrikeThru)
{
	if (Misc::StatesDiffer(m_bStrikeThruDone, bStrikeThru))
	{
		m_bStrikeThruDone = bStrikeThru;
		m_fonts.Clear();

		Invalidate();
	}
}

void CTDLFindResultsListCtrl::SetGroupHeaderBackColor(COLORREF crBack)
{
	if (crBack != GetGrouping().GetGroupHeaderBackColor())
	{
		GetGrouping().SetGroupHeaderBackColor(crBack);
		Invalidate();
	}
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
		
	// map identifying data
	FTDRESULT* pRes = new FTDRESULT(result, pTDC);
	SetItemData(nIndex, (DWORD)pRes);

	if (m_nTextOffset == 0)
		m_nTextOffset = GetTextOffset(pRes);

	if (m_nCurGroupID != -1)
		GetGrouping().SetItemGroupId(nIndex, m_nCurGroupID);

	UpdateWindow();

	return nIndex;
}

BOOL CTDLFindResultsListCtrl::AddHeaderRow(LPCTSTR szText)
{
	return GetGrouping().InsertGroupHeader(-1, ++m_nCurGroupID, szText);
}

CString CTDLFindResultsListCtrl::FormatWhatMatched(const SEARCHRESULT& result, const CFilteredToDoCtrl* pTDC, BOOL bShowValueOnly) const
{
	ASSERT(result.mapMatched.GetCount());

	DWORD dwTaskID = result.dwTaskID;
	POSITION pos = result.mapMatched.GetStartPosition();

	CStringArray aWhatMatched;

	while (pos)
	{
		TDC_ATTRIBUTE nAttribID = TDCA_NONE;
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
	CString sAttrib = TDC::GetAttributeLabel(nAttribID);

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

int CTDLFindResultsListCtrl::CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn) const
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
