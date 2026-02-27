// EnListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EnListCtrl.h" 
#include "EnHeaderCtrl.h"
#include "dlgunits.h"
#include "misc.h"
#include "themed.h"
#include "graphicsmisc.h"
#include "enimagelist.h"
#include "enstring.h"
#include "OsVersion.h"
#include "DateHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1400
#	define LVGROUPITEM LVITEM
#	define LVCUSTOMDRAW NMLVCUSTOMDRAW
#else
struct LVGROUPITEM
{
	UINT mask;
	int iItem;
	int iSubItem;
	UINT state;
	UINT stateMask;
	LPWSTR pszText;
	int cchTextMax;
	int iImage;
	LPARAM lParam;
	int iIndent;
	int iGroupId;
	UINT cColumns; // tile view columns
	PUINT puColumns;
	int* piColFmt;
	int iGroup; // readonly. only valid for owner data.
};

struct LVCUSTOMDRAW
{
	NMCUSTOMDRAW nmcd;
	COLORREF clrText;
	COLORREF clrTextBk;
	int iSubItem;
	DWORD dwItemType;

	// Item custom draw
	COLORREF clrFace;
	int iIconEffect;
	int iIconPhase;
	int iPartId;
	int iStateId;

	// Group Custom Draw
	RECT rcText;
	UINT uAlign;      // Alignment. Use LVGA_HEADER_CENTER, LVGA_HEADER_RIGHT, LVGA_HEADER_LEFT
};
#	define LVCDI_GROUP 1

#endif

#ifndef LVIF_GROUPID
#	define LVIF_GROUPID     0x0100
#	define LVIF_COLUMNS     0x0200
#endif

#ifndef LVGF_NONE

#define LVGF_NONE           0x00000000
#define LVGF_HEADER         0x00000001
#define LVGF_FOOTER         0x00000002
#define LVGF_STATE          0x00000004
#define LVGF_ALIGN          0x00000008
#define LVGF_GROUPID        0x00000010

#define LVGS_NORMAL         0x00000000
#define LVGS_COLLAPSED      0x00000001
#define LVGS_HIDDEN         0x00000002

#define LVGA_HEADER_LEFT    0x00000001
#define LVGA_HEADER_CENTER  0x00000002
#define LVGA_HEADER_RIGHT   0x00000004  // Don't forget to validate exclusivity
#define LVGA_FOOTER_LEFT    0x00000008
#define LVGA_FOOTER_CENTER  0x00000010
#define LVGA_FOOTER_RIGHT   0x00000020  // Don't forget to validate exclusivity

struct LVGROUP
{
    UINT    cbSize;
    UINT    mask;
    LPWSTR  pszHeader;
    int     cchHeader;
	
    LPWSTR  pszFooter;
    int     cchFooter;
	
    int     iGroupId;
	
    UINT    stateMask;
    UINT    state;
    UINT    uAlign;
};

#define LVM_INSERTGROUP         (LVM_FIRST + 145)
#define LVM_GETGROUPINFO        (LVM_FIRST + 149)
#define LVM_GETGROUPCOUNT       (LVM_FIRST + 152)
#define LVM_ENABLEGROUPVIEW     (LVM_FIRST + 157)
#define LVM_REMOVEALLGROUPS     (LVM_FIRST + 160)

#endif

/////////////////////////////////////////////////////////////////////////////

CListCtrlItemGrouping::CListCtrlItemGrouping(HWND hwndList) 
	: 
	m_hwndList(hwndList), 
	m_bEnabled(FALSE),
	m_crBkgnd(CLR_NONE) 
{
}

BOOL CListCtrlItemGrouping::IsSupported()
{
	// XP's API support for grouping is poor
	return (COSVersion() > OSV_XPP);
}

BOOL CListCtrlItemGrouping::EnableGroupView(BOOL bEnable)
{
	if (!m_hwndList)
	{
		ASSERT(0);
		return FALSE;
	}

	if (!IsSupported())
		return FALSE;

	if (::SendMessage(m_hwndList, LVM_ENABLEGROUPVIEW, (WPARAM)bEnable, 0) == -1)
		return FALSE;

	m_bEnabled = bEnable;
	return TRUE;
}

BOOL CListCtrlItemGrouping::EnableGroupView(HWND hwndList, BOOL bEnable)
{
	if (!hwndList)
	{
		ASSERT(0);
		return FALSE;
	}

	m_hwndList = hwndList;
	return EnableGroupView(bEnable);
}

BOOL CListCtrlItemGrouping::InsertGroupHeader(int nIndex, int nGroupID, const CString& strHeader)
{
	if (!m_bEnabled)
		return FALSE;

	LVGROUP lvg = { 0 };

	lvg.cbSize = sizeof(lvg);
	lvg.iGroupId = nGroupID;
	lvg.state = LVGS_NORMAL;
	lvg.mask = LVGF_GROUPID | LVGF_HEADER | LVGF_STATE | LVGF_ALIGN;
	lvg.uAlign = LVGA_HEADER_LEFT;
	lvg.pszHeader = (LPTSTR)(LPCTSTR)strHeader;
	lvg.cchHeader = strHeader.GetLength();

	return (-1 != ::SendMessage(m_hwndList, LVM_INSERTGROUP, (WPARAM)nIndex, (LPARAM)&lvg));
}

CString CListCtrlItemGrouping::GetGroupHeaderText(int nGroupID) const
{
	if (!m_bEnabled)
		return _T("");
	
	LVGROUP lvg = { 0 };
	TCHAR szHeader[256] = { 0 };

	lvg.cbSize = sizeof(lvg);
	lvg.mask = LVGF_HEADER;
	lvg.pszHeader = szHeader;
	lvg.cchHeader = 256;

	VERIFY(-1 != ::SendMessage(m_hwndList, LVM_GETGROUPINFO, nGroupID, (LPARAM)&lvg));

	return szHeader;
}

int CListCtrlItemGrouping::GetItemGroupId(int nRow) const
{
	if (!m_bEnabled)
		return -1;
	
	LVGROUPITEM lvgi = { 0 };

	lvgi.mask = LVIF_GROUPID;
	lvgi.iItem = nRow;

	VERIFY(::SendMessage(m_hwndList, LVM_GETITEM, 0, (LPARAM)&lvgi));

	return lvgi.iGroupId;
}

BOOL CListCtrlItemGrouping::SetItemGroupId(int nRow, int nGroupID)
{
	if (!m_bEnabled)
		return FALSE;
	
	LVGROUPITEM lvgi = { 0 };

	lvgi.mask = LVIF_GROUPID;
	lvgi.iItem = nRow;
	lvgi.iSubItem = 0;
	lvgi.iGroupId = nGroupID;

	return (BOOL)::SendMessage(m_hwndList, LVM_SETITEM, 0, (LPARAM)&lvgi);
}

BOOL CListCtrlItemGrouping::HasGroups() const
{
	if (!m_bEnabled)
		return FALSE;

	return (::SendMessage(m_hwndList, LVM_GETGROUPCOUNT, 0, 0) > 0);
}

void CListCtrlItemGrouping::RemoveAllGroups()
{
	if (m_bEnabled)
		::SendMessage(m_hwndList, LVM_REMOVEALLGROUPS, 0, 0);
}

void CListCtrlItemGrouping::SetGroupHeaderBackColor(COLORREF crBack)
{
	if (crBack != m_crBkgnd)
	{
		m_crBkgnd = crBack;
		InvalidateRect(m_hwndList, NULL, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////

const DWORD NOTSET = 0xffffffff;
const COLORREF ELC_GRIDCOLOR = RGB(192, 192, 192);
const int TIMERID_EDITLABELS = 42;

/////////////////////////////////////////////////////////////////////////////

DWORD CEnListCtrl::s_dwSelectionTheming = MAKELONG(TRUE, FALSE);

/////////////////////////////////////////////////////////////////////////////

CEnListCtrl::CColumnData::CColumnData()
{
	crText = ::GetSysColor(COLOR_WINDOWTEXT);
	nFormat = ES_END;
}

/////////////////////////////////////////////////////////////////////////////
// CEnListCtrl

IMPLEMENT_DYNAMIC(CEnListCtrl, CListCtrl)

CEnListCtrl::CEnListCtrl() 
	: 
	m_dwSelectionTheming(NOTSET),
	m_bVertGrid(FALSE),
	m_bHorzGrid(FALSE),
	m_nCurView(-1),
	m_bLastColStretchy(FALSE),
	m_bFirstColStretchy(FALSE),
	m_nMinItemHeight(-1),
	m_nItemHeight(-1),
	m_bReadOnly(FALSE),
	m_nItemDropHilite(-1),
	m_bDropHiliteItemSelected(FALSE),
	m_bContextPopupEnabled(FALSE),
	m_bUserSelChange(FALSE),
	m_bSortingEnabled(TRUE),
	m_nSortColumn(-1),
	m_bSortAscending(TRUE),
	m_bInitColumns(FALSE),
	m_bAlternateRowColoring(FALSE),
	m_bSortEmptyBelow(TRUE),
	m_bAllowOffItemClickDeslection(TRUE),
	m_bResizingCols(FALSE)
{
}

CEnListCtrl::~CEnListCtrl()
{
	DeleteAllColumnData();
}

BEGIN_MESSAGE_MAP(CEnListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CEnListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
	ON_WM_SIZE()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT_EX(NM_CUSTOMDRAW, OnListCustomDraw)
	ON_NOTIFY(NM_CUSTOMDRAW, 0, OnHeaderCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnListCtrl message handlers

BOOL CEnListCtrl::EnableGroupView(BOOL bEnable)
{
	if (!Misc::StatesDiffer(bEnable, m_grouping.IsEnabled()))
		return TRUE;
	
	if (!m_grouping.EnableGroupView(GetSafeHwnd(), bEnable))
		return FALSE;

	ForceResize(TRUE);
	return TRUE;
}

BOOL CEnListCtrl::DeleteAllItems()
{
	m_grouping.RemoveAllGroups();
	
	return CListCtrl::DeleteAllItems();
}

void CEnListCtrl::OnPaint() 
{
	// if no items or in report mode draw to back buffer then blt
	if (GetItemCount() && GetView() != LVS_REPORT)
	{
		Default();
	}
	else
	{
		CPaintDC cleanup(this);

		CHeaderCtrl* pHeader = GetHeader();
		CDC& paintdc = *GetDC();

		CDC dc;
		dc.CreateCompatibleDC(&paintdc);
		
		CRect rClient;
		GetClientRect( &rClient );
		
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&paintdc, rClient.right, rClient.bottom);
		CBitmap *pOldBitmap = dc.SelectObject(& bitmap);
			
		if (pHeader && m_nCurView == LVS_REPORT && !(GetStyle() & LVS_NOCOLUMNHEADER))
		{
			CRect rHeader;
			pHeader->GetWindowRect(&rHeader);
			ScreenToClient(rHeader);
			dc.ExcludeClipRect(rHeader);
			rClient.top = rHeader.bottom;
		}

		// fill with back color
		COLORREF crBack = GetBkColor();

		if (crBack != CLR_NONE)
			dc.FillSolidRect(rClient, crBack);

		// default drawing
		CListCtrl::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
		
		// do empty text if nec
		if (GetItemCount() == 0)
		{
			CString sText = GetNoItemsText(); // virtual call
			Misc::Trim(sText);

			if (!sText.IsEmpty())
			{
				if (Misc::First(sText) != '(')
					sText = "(" + sText;

				if (Misc::Last(sText) != ')')
					sText += ")";
			
				dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
				dc.SetBkColor(crBack);
				dc.SelectStockObject( ANSI_VAR_FONT );

				rClient.top += 10;
				dc.DrawText( sText, rClient, DT_CENTER | DT_WORDBREAK | DT_NOPREFIX);
				rClient.top -= 10; // reset
			}
		}

		paintdc.BitBlt(0, rClient.top, rClient.right, rClient.bottom, 
					   &dc, 0, rClient.top, SRCCOPY);

		dc.SelectObject(pOldBitmap);
		ReleaseDC(&paintdc);
	}

	// Do not call CListView::OnPaint() for painting messages
}

void CEnListCtrl::DeleteAllColumnData()
{
	int nCol;
	CColumnData* pData;
	POSITION pos = m_mapColumnData.GetStartPosition();

	while (pos)
	{
		m_mapColumnData.GetNextAssoc(pos, nCol, pData);
		delete pData;
	}
}

CEnListCtrl::CColumnData* CEnListCtrl::CreateColumnData(int nCol)
{
	CColumnData* pData = NULL;

	if (!m_mapColumnData.Lookup(nCol, pData)) // see if its already added
	{
		pData = GetNewColumnData();
		
		if (pData)
			m_mapColumnData.SetAt(nCol, pData);

		// set the column format to the most appropriate
		LV_COLUMN lvc = { 0 };
		lvc.mask = LVCF_FMT;
		
		if (GetColumn(nCol, &lvc))
		{
			if ((lvc.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
				pData->nFormat = ES_START;
		}
	}

	return pData;
}

const CEnListCtrl::CColumnData* CEnListCtrl::GetColumnData(int nCol) const
{
	CColumnData* pData = NULL;
	m_mapColumnData.Lookup(nCol, pData);

	return pData;
}

CEnHeaderCtrl* CEnListCtrl::GetHeader()
{
	BOOL bRes = TRUE;
	ASSERT (m_hWnd);

	// subclass header as CEnHeaderCtrl
	if (!m_header.GetSafeHwnd())
	{
		HWND hWnd = (HWND)::SendMessage(m_hWnd, LVM_GETHEADER, 0, 0);
		bRes = m_header.SubclassWindow(hWnd);
	}

	return bRes ? &m_header : NULL;
}

const CEnHeaderCtrl* CEnListCtrl::GetHeader() const
{
	return m_header.GetSafeHwnd() ? &m_header : NULL;
}

int CEnListCtrl::GetFirstSel() const
{
	return GetNextItem(-1, LVNI_SELECTED);
}

int CEnListCtrl::GetNextSel(int nPrevSel) const
{
	ASSERT (nPrevSel >= 0 && nPrevSel < GetItemCount());

	return GetNextItem(nPrevSel, LVNI_SELECTED);
}

int CEnListCtrl::GetFocusedItem() const
{
	return GetNextItem(-1, LVNI_FOCUSED);
}

int CEnListCtrl::GetLastSel() const
{
	int nIndexSel, nLastSel;
	
	nIndexSel = GetFirstSel();

	if (nIndexSel == -1)
		return -1;

	do
	{
		nLastSel = nIndexSel;
	}
	while((nIndexSel = GetNextSel(nIndexSel)) != -1);
	
	return nLastSel;
}

int CEnListCtrl::GetPrevSel(int nNextSel) const
{
	ASSERT (nNextSel >= 0 && nNextSel <= GetItemCount());

	int nIndexSel, nPrevSel;
	
	nIndexSel = GetFirstSel();

	if (nIndexSel == -1)
		return -1;

	do
	{
		nPrevSel = nIndexSel;
	}
	while((nIndexSel = GetNextSel(nIndexSel)) != -1 &&
			nIndexSel < nNextSel);
	
	return nPrevSel;
}

int CEnListCtrl::GetCurSel() const
{
	return GetFirstSel();
}

void CEnListCtrl::SetMulSel(int nIndexStart, int nIndexEnd, BOOL bSelect, BOOL bNotifyParent)
{
	ASSERT (nIndexStart >= 0 && nIndexStart < GetItemCount() &&
			nIndexEnd >= 0 && nIndexEnd < GetItemCount());
	
	int nIndex;

	for (nIndex = nIndexStart; nIndex <= nIndexEnd; nIndex++)
		SetItemState(nIndex, bSelect ? LVIS_SELECTED : 0, LVIS_SELECTED);

	// if we have the focus then update parent 
	if (GetFocus() == this && bNotifyParent)
		NotifySelChange();
}

void CEnListCtrl::SetItemFocus(int nIndex, BOOL bFocused)
{
	if (!(nIndex >= 0 && nIndex < GetItemCount()))
		return;
	
	// set focus
	SetItemState(nIndex, bFocused ? LVIS_FOCUSED : 0, LVIS_FOCUSED);
}

void CEnListCtrl::SelectAll()
{
	if (GetItemCount())
		SetMulSel(0, GetItemCount() - 1, TRUE);

	// if we have the focus then update parent 
	if (GetFocus() == this)
		NotifySelChange();
}

void CEnListCtrl::ClearAll()
{
	if (GetItemCount())
		SetMulSel(0, GetItemCount() - 1, FALSE);

	// if we have the focus then update parent 
	if (GetFocus() == this)
		NotifySelChange();
}

int CEnListCtrl::SetCurSel(int nIndex, bool bNotifyParent)
{
	ASSERT (nIndex >= -1 && nIndex < GetItemCount());
	
	int nCurSel, nRes;
	UINT nState, nMask;
	CRect rItem;
	
	nState = nMask = LVIS_SELECTED | LVIS_FOCUSED;

	nCurSel = GetFirstSel();

	if (nCurSel != -1)
	{
		SetItemState(nCurSel, 0, nMask);
		Update(nCurSel);
	}
	SetItemState(nIndex, nState, nMask);
	nRes = Update(nIndex);

	// if we have the focus then update parent 
	if (GetFocus() == this && bNotifyParent)
		NotifySelChange();

	return nRes;
}

int CEnListCtrl::GetCountPerPage() const
{
	CRect rList, rItem;

	if (GetItemCount() == 0)
		return 0;

	GetClientRect(&rList);
	GetItemRect(0, &rItem, LVIR_BOUNDS);
	
	ASSERT (!rList.IsRectEmpty() && !rItem.IsRectEmpty());

	return	rList.Height() / rItem.Height();
}

BOOL CEnListCtrl::IsSelectionThemed(BOOL bClassic) const
{
	DWORD dwTheming = ((m_dwSelectionTheming == NOTSET) ? s_dwSelectionTheming : m_dwSelectionTheming);

	BOOL bThemed = LOWORD(dwTheming);
	
	if (bClassic)
		bThemed &= HIWORD(dwTheming); 
	
	return bThemed;
}

void CEnListCtrl::DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused)
{
	if (bDropHighlighted && IsSelectionThemed(FALSE))
	{
		DWORD dwFlags = (IsSelectionThemed(TRUE) ? GMIB_THEMECLASSIC : 0);
		GraphicsMisc::DrawExplorerItemSelection(pDC, *this, GMIS_DROPHILITED, rItem, dwFlags, rItem);
	}
	else if (bSelected && IsSelectionThemed(FALSE))
	{
		DWORD dwFlags = (IsSelectionThemed(TRUE) ? GMIB_THEMECLASSIC : 0);
		GM_ITEMSTATE nState = (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);

		GraphicsMisc::DrawExplorerItemSelection(pDC, *this, nState, rItem, dwFlags, rItem);
	}
	else if (crBack != CLR_NONE)
	{
		pDC->FillSolidRect(rItem, crBack);
	}
}

void CEnListCtrl::DrawCellBackground(CDC* /*pDC*/, int /*nItem*/, int /*nCol*/, 
									 const CRect& /*rCell*/, BOOL /*bSelected*/, 
									 BOOL /*bDropHighlighted*/, BOOL /*bFocused*/)
{
	// Do nothing by default
}

void CEnListCtrl::DrawCellText(CDC* pDC, int /*nItem*/, int /*nCol*/,
							   const CRect& rText, const CString& sText,
							   COLORREF crText, UINT nDrawTextFlags)
{
	if (!sText.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->DrawText(sText, (LPRECT)(LPCRECT)rText, nDrawTextFlags);
	}
}

UINT CEnListCtrl::GetTextDrawFlags(int nCol, BOOL bDate) const
{
	UINT nFlags = (DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);

	if (bDate && CDateHelper::WantRTLDates())
		nFlags |= DT_RTLREADING;

	LV_COLUMN lvc = { 0 };
	lvc.mask = LVCF_FMT;

	VERIFY(GetColumn(nCol, &lvc));

	switch (lvc.fmt & LVCFMT_JUSTIFYMASK)
	{
	case LVCFMT_CENTER:	nFlags |= DT_CENTER;	break;
	case LVCFMT_RIGHT:	nFlags |= DT_RIGHT;		break;
	case LVCFMT_LEFT:	nFlags |= DT_LEFT;		break;
	}

	return nFlags;
}

void CEnListCtrl::DrawCell(CDC* pDC, int nItem, int nCol, 
						   const CRect& rCell, const CString& sText, 
						   BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused)
{
	// get item text and output
	CFont* pFont = GetItemFont(nItem, nCol);
	CFont* pOldFont = NULL;

	if (pFont)
		pOldFont = pDC->SelectObject(pFont);

	// draw text
	CRect rText(rCell);
	rText.DeflateRect(2, 0);

	if (rText.Height() && (rText.Width() > 0))
	{
		DrawCellBackground(pDC, nItem, nCol, rCell, bSelected, bDropHighlighted, bFocused);

		COLORREF crText = GetItemTextColor(nItem, nCol, bSelected, bDropHighlighted, bFocused);

		if (bSelected && IsSelectionThemed(FALSE))
		{
			DWORD dwFlags = (IsSelectionThemed(TRUE) ? GMIB_THEMECLASSIC : 0);
			GM_ITEMSTATE nState = (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);

			crText = GraphicsMisc::GetExplorerItemSelectionTextColor(crText, nState, dwFlags);
		}

		DrawCellText(pDC, nItem, nCol, rText, sText, crText, GetTextDrawFlags(nCol));
	}

	// reset font
	if (pFont)
		pDC->SelectObject(pOldFont);
}

void CEnListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->SelectObject(GetFont());
	pDC->SetROP2(R2_COPYPEN);

	// init helper variables
	int nItem = lpDrawItemStruct->itemID;
	UINT uStyle = GetStyle();
	UINT uState = GetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED | LVIS_DROPHILITED);

	// init helper variables
	CRect rItem, rClient;
	GetItemRect(nItem, rItem, LVIR_BOUNDS);

	if (m_grouping.IsEnabled())
		rItem.top--;

	GetClientRect(&rClient);

	// some problems with drop-highlighting items during drag and drop
	// so we need to make sure drawing is clipped to client area
	// this fixes it admirably!
	if (GetHeader())
	{
		CRect rHeader;
		GetHeader()->GetWindowRect(rHeader);
		ScreenToClient(rHeader);
		rClient.top = max(0, rHeader.bottom);
		pDC->IntersectClipRect(rClient);
	}

	BOOL bListFocused = (GetFocus() == this);
	BOOL bSelAlways = ((uStyle & LVS_SHOWSELALWAYS) == LVS_SHOWSELALWAYS);
	BOOL bSelected = (IsWindowEnabled() && (uState & LVIS_SELECTED) && (bListFocused || bSelAlways));
	BOOL bDropHighlighted = (uState & LVIS_DROPHILITED);

	BOOL bItemFocused = (bListFocused && bSelected);
	BOOL bWantCellFocus = (bListFocused && !IsSelectionThemed(TRUE) && !CThemed::AreControlsThemed());

	// images and indentation
	CSize sizeState, sizeImage;
	CImageList* pImageList = GetImageList(LVSIL_SMALL);

	if (pImageList)
		CEnImageList::GetImageSize(*pImageList, sizeImage);

	CImageList* pStateList = GetImageList(LVSIL_STATE);

	if (pStateList)
		CEnImageList::GetImageSize(*pStateList, sizeState);

	if (lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT))
	{
		COLORREF crBack = GetItemBackColor(nItem, bSelected, FALSE, bItemFocused);
		DrawItemBackground(pDC, nItem, rItem, crBack, bSelected, bDropHighlighted, bItemFocused);

		// draw horz grid lines if required
		if (m_bHorzGrid)
		{
			int nGridEnd = m_bVertGrid ? rItem.right : rClient.right;
			GraphicsMisc::DrawHorzLine(pDC, rClient.left, nGridEnd, rItem.bottom - 1, ELC_GRIDCOLOR);
		}

		// cycle thru columns formatting and drawing each subitem
		int nNumCol = GetColumnCount();

		for (int nCol = 0; nCol < nNumCol; nCol++)
		{
			CRect rCell;
			GetCellRect(nItem, nCol, rCell);

			// Skip cells wholly outside client rect
			if (rCell.right <= rClient.left)
				continue;

			if (rCell.left >= rClient.right)
				break;

			// Draw images for column zero
			if (nCol == 0)
			{
				rCell.left += GetItemIndent(nItem);

				// draw item images if required
				int nImageWidth = 0;

				// state
				if (pStateList)
				{
					int nState = (GetItemState(nItem, LVIS_STATEIMAGEMASK) & LVIS_STATEIMAGEMASK);
					nState = nState >> 12;

					if (rCell.Width() > sizeState.cx)
						pStateList->Draw(pDC, nState, CPoint(rCell.left + 1, rCell.top), ILD_TRANSPARENT);

					nImageWidth = sizeState.cx + 2; // 1 pixel border either side
				}

				// normal
				int nImage = GetItemImage(nItem);

				if (pImageList && (nImage != -1))
				{
					if (rCell.Width() > (nImageWidth + sizeImage.cx))
						pImageList->Draw(pDC, nImage, CPoint(rCell.left + 1 + nImageWidth, rCell.top), ILD_TRANSPARENT);

					nImageWidth += sizeImage.cx + 2; // 1 pixel border either side
				}

				rCell.left += nImageWidth;
			}

			// draw vert grid if required and we're not tight up against the client edge
			if (m_bVertGrid && (rCell.right < rClient.right))
			{
				GraphicsMisc::DrawVertLine(pDC, rCell.bottom, rCell.top, (rCell.right - 1), ELC_GRIDCOLOR);
				rCell.right--;
			}

			if (m_bHorzGrid)
				rCell.bottom--;

			CEnString sText(GetItemText(nItem, nCol));
			DrawCell(pDC, nItem, nCol, rCell, sText, bSelected, bDropHighlighted, bListFocused);
		}

		// focus rect: normal method doesn't work because we are focusing whole line
		// note: if we're scrolled to the right the image may not be visible
		if (bItemFocused && bListFocused && bWantCellFocus)
		{
			//rItem.left += nImageWidth;
			pDC->DrawFocusRect(rItem);
		}
	}
}

COLORREF CEnListCtrl::GetItemTextColor(int /*nItem*/, int nSubItem, 
									   BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	BOOL bThemedSel = ((bSelected || bDropHighlighted) && (CThemed::AreControlsThemed() || IsSelectionThemed(TRUE)));

	if (!bThemedSel)
	{
		if (bSelected)
		{
			if (bWndFocus)
				return ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			else
				return ::GetSysColor(COLOR_WINDOWTEXT);
		}
		else if (bDropHighlighted)
		{
			return ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		}
	}

	// else 
	if (!IsWindowEnabled())
		return ::GetSysColor(COLOR_3DDKSHADOW);

	// else
	return GetColumnTextColor(nSubItem);
}

COLORREF CEnListCtrl::GetItemBackColor(int /*nItem*/, BOOL bSelected, 
									   BOOL bDropHighlighted, BOOL bWndFocus) const
{
	BOOL bThemedSel = (bSelected && (CThemed::AreControlsThemed() || IsSelectionThemed(TRUE)));

	if (!bThemedSel)
	{
		if (bSelected)
		{
			if (bWndFocus)
				return ::GetSysColor(COLOR_HIGHLIGHT);

			if (GetStyle() & LVS_SHOWSELALWAYS)
				return ::GetSysColor(COLOR_BTNFACE);
		} 
		else if (bDropHighlighted)
		{
			return ::GetSysColor(COLOR_HIGHLIGHT);
		}
	}

	// else
	COLORREF crBack = GetTextBkColor(); 
	crBack = (crBack != 0xff000000) ? crBack : ::GetSysColor(COLOR_WINDOW);

	return crBack;
}

void CEnListCtrl::EnableHeaderTracking(BOOL bAllow)
{
	m_header.EnableTracking(bAllow);
}

BOOL CEnListCtrl::SetTooltipText(CString sText, DWORD dwTipFlags)
{
	ASSERT (m_hWnd);

	if (sText.IsEmpty())
	{
		m_ttCtrl.DestroyToolTipCtrl();
		return FALSE;
	}

	if (m_ttCtrl.m_hWnd == NULL)
		m_ttCtrl.Create(this, dwTipFlags);
	
	m_ttCtrl.AddTool(this, sText);
	m_ttCtrl.Activate(TRUE);

	return TRUE;
}

void CEnListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nHit = HitTest(point);

	if (!WantSelChange(nHit))
	{
		// Make sure we finish editing
		if (GetEditControl() != NULL)
			SetFocus();

		return;
	}

	CListCtrl::OnLButtonDown(nFlags, point);

	if (nHit != -1)
	{
		int nSel = GetCurSel();

		if (nHit == nSel)
		{
			// if the list is in report mode AND supports label editing then
			// clicking anywhere on the item when its already selected will
			// edit the label of column one which is not what we want. 
			// So if we detect that situation we kill the label edit timer.
			if ((m_nCurView == LVS_REPORT) && (GetStyle() & LVS_EDITLABELS))
			{
				CRect rItem;
				GetSubItemRect(nHit, 0, LVIR_LABEL, rItem);

				if (!rItem.PtInRect(point))
				{
					KillTimer(TIMERID_EDITLABELS);
				}
			}
		}
		else // user clicked on another item
		{
			// determine whether a sel change has occurred
			// and tell our parent if it has
			NotifySelChange();
		}
	}
}

void CEnListCtrl::SetColumnTextColor(int nCol, COLORREF color)
{
	CColumnData* pData = CreateColumnData(nCol);

	if (pData)
	{
		pData->crText = color;

		if (m_hWnd)
			Invalidate(); // redraw
	}
}

COLORREF CEnListCtrl::GetColumnTextColor(int nCol) const
{
	const CColumnData* pData = GetColumnData(nCol);

	if (pData)
		return pData->crText;

	// else
	return ::GetSysColor(COLOR_WINDOWTEXT);
}

void CEnListCtrl::SetColumnFormat(int nCol, int nFormat)
{
	CColumnData* pData = CreateColumnData(nCol);

	if (pData)
	{
		pData->nFormat = nFormat;

		if (m_hWnd)
			Invalidate(); // redraw
	}
}

int CEnListCtrl::GetColumnFormat(int nCol) const
{
	const CColumnData* pData = GetColumnData(nCol);

	if (pData)
		return pData->nFormat;

	// else return most appropriate
	LV_COLUMN lvc = { 0 };
	lvc.mask = LVCF_FMT;
		
	if (GetColumn(nCol, &lvc))
	{
		if ((lvc.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
			return ES_START;
	}

	// else
	return ES_END;
}

void CEnListCtrl::ShowGrid(BOOL bVert, BOOL bHorz)
{
	m_bVertGrid = bVert;
	m_bHorzGrid = bHorz;

	if (m_hWnd)
		Invalidate(); // redraw
}

void CEnListCtrl::SetView(int nView)
{
	CRect rClient;

	ASSERT (nView == LVS_ICON || nView == LVS_LIST ||
			nView == LVS_SMALLICON || nView == LVS_REPORT);

	ASSERT (m_hWnd);
	
	if (!(nView == LVS_ICON || nView == LVS_LIST ||
		nView == LVS_SMALLICON || nView == LVS_REPORT))
		return;

	// Get the current window style. 
	DWORD dwStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE); 
 
    // Only set the window style if the view bits have changed. 
    if ((dwStyle & LVS_TYPEMASK) != (DWORD)nView) 
		SetWindowLong(GetSafeHwnd(), GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | (DWORD)nView); 

	// if we are in report view then we want to be owner-draw 
	// so we can take advantage of the special drawing
	if (nView == LVS_REPORT)
	{
		// make owner-draw
		if (!(dwStyle & LVS_OWNERDRAWFIXED))
			ModifyStyle(0, LVS_OWNERDRAWFIXED); 
	}
	// else we want default drawing
	else
	{
		// make non-owner-draw
		if (dwStyle & LVS_OWNERDRAWFIXED)
			ModifyStyle(LVS_OWNERDRAWFIXED, 0); 
	}

	m_nCurView = nView;
	Invalidate(FALSE);

	// force a resize to ensure that the column headers are correct
	ForceResize(FALSE);
}

int CEnListCtrl::GetItemImage(int nItem, int nSubItem) const
{
	LV_ITEM lvi =  { 0 };

	lvi.mask = LVIF_IMAGE;
	lvi.iItem = nItem;
	lvi.iSubItem = nSubItem;
	
	GetItem(&lvi);

	return lvi.iImage;
}

BOOL CEnListCtrl::SetItemImage(int nItem, int nImage)
{
	return SetItem(nItem, 0, LVIF_IMAGE, _T(""), nImage, 0, 0, 0L);
}

BOOL CEnListCtrl::SetItemImage(int nItem, int nSubItem, int nImage)
{
	LV_ITEM lvi = { 0 };

	lvi.mask = LVIF_IMAGE;
	lvi.iItem = nItem;
	lvi.iSubItem = nSubItem;

	return SetItem(&lvi);
}

void CEnListCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// check if user did a cut, copy, paste or delete and we're not readonly query parent
	// if parent returns TRUE then do action
	BOOL bCtrlDown = Misc::IsKeyPressed(VK_CONTROL);
	m_nmhdr.hwndFrom = m_hWnd;
	m_nmhdr.idFrom = GetDlgCtrlID();

	switch (nChar)
	{
		// copy = ctrl + 'c'
		case 'C':
		case 'c':
		{
			if (!bCtrlDown)
				return;

			m_nmhdr.code = LVN_COPY;

			if (!IsReadOnly() && !GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&m_nmhdr))
				Copy();
			break;
		}

		// paste = ctrl + 'v'
		case 'V':
		case 'v':
		{
			if (!bCtrlDown)
				return;

			m_nmhdr.code = LVN_PASTE;

			if (!IsReadOnly() && !GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&m_nmhdr))
				Paste();
			break;
		}

		// cut = ctrl + 'x'
		case 'X':
		case 'x':
		{
			if (!bCtrlDown)
				return;

			m_nmhdr.code = LVN_CUT;

			if (!IsReadOnly() && !GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&m_nmhdr))
				Cut();
			break;
		}

		// delete
		case VK_DELETE:
		{	
			m_nmhdr.code = LVN_DELETE;

			if (!IsReadOnly() && !GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&m_nmhdr))
				Delete();
			break;
		}

		// Select All = ctrl + 'x'
		case 'A':
		case 'a':
			if (bCtrlDown && (!(GetStyle() & LVS_SINGLESEL) || (GetItemCount() == 1)))
				SelectAll();
			break;
	}
	
	// otherwise do default processing 
	CListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
	UpdateWindow();
	
	// and determine whether a sel change has occured
	// and tell our parent if it has
	if (GetCurSel() != -1)
		NotifySelChange();
}

void CEnListCtrl::SetLastColumnStretchy(BOOL bStretchy)
{
	m_bLastColStretchy = bStretchy;
	m_bFirstColStretchy = FALSE;

	// invoke a resize to update last column
	if (m_bLastColStretchy && (m_nCurView == LVS_REPORT) && GetSafeHwnd())
	{
		ForceResize(FALSE);
	}
}

LRESULT CEnListCtrl::OnSetFont(WPARAM wp, LPARAM lp)
{
	LRESULT lr = Default();

	ForceResize(TRUE); // Recalc item height

	return lr;
}

void CEnListCtrl::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CListCtrl::OnWindowPosChanged(lpwndpos);

	ResizeStretchyColumns();
}

void CEnListCtrl::SetFirstColumnStretchy(BOOL bStretchy)
{
	m_bFirstColStretchy = bStretchy;
	m_bLastColStretchy = FALSE;

	// invoke a resize to update first column
	if (m_bFirstColStretchy && m_nCurView == LVS_REPORT && GetSafeHwnd())
	{
		ForceResize(FALSE);
	}
}

void CEnListCtrl::OnTimer(UINT nIDEvent)
{
	CListCtrl::OnTimer(nIDEvent);
}

void CEnListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

	ResizeStretchyColumns();
}

void CEnListCtrl::ResizeStretchyColumns()
{
	if (m_bResizingCols)
		return;

	m_bResizingCols = TRUE;

	// get header state
	CRect rClient;
	GetClientRect(rClient);

	HD_LAYOUT hdLayout = { 0 };
	WINDOWPOS wpos = { 0 };

	if (m_header.GetSafeHwnd())
	{
		GetClientRect(rClient);
		hdLayout.prc = &rClient;
		hdLayout.pwpos = &wpos;
		Header_Layout(GetHeader()->GetSafeHwnd(), &hdLayout);
	}

	// stretch the appropriate columns if in report mode
	if (m_nCurView == LVS_REPORT)
	{
		int nCol, nNumCols, nColEnd = 0;

		// get the start of the last column
		if (m_bLastColStretchy)
		{
			SetColumnWidth(GetColumnCount() - 1, LVSCW_AUTOSIZE_USEHEADER);
			ShowScrollBar(SB_HORZ, FALSE);
		}
		// get the end of the first column
		else if (m_bFirstColStretchy)
		{
			nNumCols = GetColumnCount();

			for (nCol = 1; nCol < nNumCols; nCol++)
				nColEnd += GetColumnWidth(nCol);

			nColEnd = rClient.Width() - nColEnd;

			// if its reasonably less than the client area adjust else do nothing
			if ((nColEnd > 100) && (nColEnd < rClient.Width() - 20) || (nNumCols == 1))
				SetColumnWidth(0, nColEnd - 2);

			ShowScrollBar(SB_HORZ, (nColEnd < 100));
		}

		// show header ctrl
		wpos.flags |= SWP_SHOWWINDOW;
	}
	else // hide header ctrl
		wpos.flags |= SWP_HIDEWINDOW;

	// ensure the header ctrl is correctly positioned
	if (m_header.GetSafeHwnd())
	{
		::SetWindowPos(GetHeader()->GetSafeHwnd(), wpos.hwndInsertAfter, wpos.x, wpos.y,
			wpos.cx, wpos.cy, wpos.flags);
	}

	// windows does not send wm_paint messages if the window size decreased
	// and we have no elements, so...
	if (!GetItemCount())
		Invalidate(FALSE);

	m_bResizingCols = FALSE;
}

int CEnListCtrl::FindItemFromData(DWORD dwItemData) const
{
	if (!GetItemCount())
		return -1;

	LV_FINDINFO lvfi;

	lvfi.flags = LVFI_PARAM;
	lvfi.lParam = (LPARAM)dwItemData;

	return FindItem(&lvfi, -1);
}

int CEnListCtrl::FindItemFromLabel(CString sLabel, BOOL bExact, int nFromIndex) const
{
	if (!GetItemCount())
		return -1;

	LV_FINDINFO lvfi;

	lvfi.flags = LVFI_STRING | LVFI_WRAP;

	if (!bExact)
		lvfi.flags |= LVFI_PARTIAL;

	lvfi.psz = (LPCTSTR)sLabel;

	return FindItem(&lvfi, nFromIndex ? -1 : nFromIndex);
}

BOOL CEnListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (::IsWindow(m_ttCtrl.m_hWnd) && (pMsg->hwnd == m_hWnd))
	{
		switch (pMsg->message)
		{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MOUSEMOVE:
				m_ttCtrl.RelayEvent(pMsg);
				break;
		}
	}
	
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CEnListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nItemHeight == -1)
	{
		// Default edit height
		int nBaseHeight = CDlgUnits(this, TRUE).ToPixelsY(9);

		// Font height
		TEXTMETRIC tm = { 0 };
		VERIFY(GraphicsMisc::GetFontMetrics(*this, tm));

		int nFontHeight = (tm.tmHeight + tm.tmExternalLeading);

		// Item height
		m_nItemHeight = max(m_nMinItemHeight, max(nBaseHeight, nFontHeight));

		if (m_grouping.IsEnabled())
			m_nItemHeight--;
	}

    lpMeasureItemStruct->itemHeight = m_nItemHeight; 
}

void CEnListCtrl::ForceResize(BOOL bRecalcItemHeight)
{
	if (bRecalcItemHeight)
		m_nItemHeight = -1;

	if (GetSafeHwnd())
	{
		CRect rClient;
		GetClientRect(rClient);

		WINDOWPOS wp = { GetSafeHwnd(), NULL, 0, 0, rClient.Width(), rClient.Height(), SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER };
		SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
	}
}

BOOL CEnListCtrl::SetMinItemHeight(int nHeight)
{
	if (nHeight < 10 || nHeight > 40) // arbitrary limits
	{
		ASSERT(0);
		return FALSE;
	}

	if (nHeight != m_nMinItemHeight)
	{
		m_nMinItemHeight = nHeight;
		ForceResize(m_nItemHeight != m_nMinItemHeight);
	}

	return TRUE;
}

void CEnListCtrl::DeleteAllColumns()
{
	int nNumCols;
	
	nNumCols = GetColumnCount();

	while (nNumCols--)
		DeleteColumn(nNumCols);
}

BOOL CEnListCtrl::WantSelChange(int nSel) const
{
	return (m_bAllowOffItemClickDeslection || !(GetStyle() & LVS_SINGLESEL) || (nSel != -1));
}

void CEnListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nSel = HitTest(point);

	if (!WantSelChange(nSel))
		return;

	CListCtrl::OnLButtonDblClk(nFlags, point);

	if (nSel != -1) // user clicked on an item
	{
		// determine whether a sel change has occured
		// and tell our parent if it has
		m_nmhdr.hwndFrom = m_hWnd;
		m_nmhdr.idFrom = GetDlgCtrlID();
		m_nmhdr.code = LVN_USERSELCHANGEDBLCLK;
		GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&m_nmhdr);
	}
}

BOOL CEnListCtrl::SelectDropTarget(int nItem)
{
	int nCurDropHilited;

	// remove current drop selection provided item was not 
	// originally selected
	nCurDropHilited = GetNextItem(-1, LVNI_DROPHILITED);

	if (nCurDropHilited != -1 && nCurDropHilited != nItem)
	{
		SetItemState(nCurDropHilited, 0, LVIS_DROPHILITED);
		RedrawItems(nCurDropHilited, nCurDropHilited);
		UpdateWindow();
	}

	if (nItem < 0 || nItem >= GetItemCount())
		return FALSE;

	// set new drop selection provided hilite has changed and
	// the item isn't already selected
	if ((nCurDropHilited != nItem) && 
		((GetItemState(nItem, LVIS_SELECTED) & LVIS_SELECTED) != LVIS_SELECTED))
	{
		SetItemState(nItem, LVIS_DROPHILITED, LVIS_DROPHILITED);
		RedrawItems(nItem, nItem);
		UpdateWindow();
	}

	return TRUE;
}

BOOL CEnListCtrl::IsItemSelected(int nItem) const
{
	ASSERT (nItem >= 0 && nItem < GetItemCount());

	return (GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED);
}

void CEnListCtrl::GetCellRect(int nRow, int nCol, CRect& rCell) const
{
	// Cast required for VC6
	const_cast<CEnListCtrl*>(this)->GetSubItemRect(nRow, nCol, LVIR_LABEL, rCell);

	// By default adds a 4 pixel offset for the first column
	// which is weird so we do a bit of trickery
	if (nCol == 0)
		rCell.left = -GetScrollPos(SB_HORZ);

	if (m_grouping.IsEnabled())
		rCell.top--;
}

void CEnListCtrl::GetCellEditRect(int nRow, int nCol, CRect& rCell) const
{
	GetCellRect(nRow, nCol, rCell);

	// Overlap the edit rect with the previous cell's grid borders
	if (m_bVertGrid && (nCol > 0))
		rCell.left--;

	if (m_bHorzGrid && (nRow > 0))
		rCell.top--;
}

void CEnListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CListCtrl::OnRButtonDown(nFlags, point);
}

void CEnListCtrl::NotifySelChange()
{
	m_bUserSelChange = TRUE;

	m_nmhdr.hwndFrom = m_hWnd;
	m_nmhdr.idFrom = GetDlgCtrlID();
	m_nmhdr.code = LVN_USERSELCHANGE;
	GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&m_nmhdr);

	m_bUserSelChange = FALSE;
}

BOOL CEnListCtrl::OnColumnClick(NMHDR* pNMHDR, LPARAM* /*lParam*/)
{
	NMLISTVIEW* pNMLV = (NM_LISTVIEW*)pNMHDR;

	if (m_bSortingEnabled)
		SetSortColumn(pNMLV->iSubItem);

	return FALSE; // continue routing
}

BOOL CEnListCtrl::IsSelectionChange(NMLISTVIEW* pNMLV, int* pItem)
{
	ASSERT(pNMLV);

	if (pNMLV->uChanged & LVIF_STATE)
	{
		BOOL bWasSel = (pNMLV->uOldState & LVIS_SELECTED);
		BOOL bIsSel = (pNMLV->uNewState & LVIS_SELECTED);

		if (Misc::StatesDiffer(bIsSel, bWasSel))
		{
			if (pItem)
				*pItem = pNMLV->iItem;

			return TRUE;
		}
	}

	// else
	return FALSE;
}

BOOL CEnListCtrl::OnListCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = CDRF_DODEFAULT;
	LVCUSTOMDRAW* pLVCD = (LVCUSTOMDRAW*)pNMHDR;

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		if (pLVCD->dwItemType == LVCDI_GROUP)
		{
			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

			CRect rRow(pLVCD->rcText);
			rRow.OffsetRect(0, -2);

			DrawGroupHeader(pDC, 
							rRow,
							m_grouping.GetGroupHeaderText(pLVCD->nmcd.dwItemSpec),
							m_grouping.GetGroupHeaderBackColor());

			if (m_bHorzGrid)
			{
				if (m_bVertGrid)
					rRow.right = pLVCD->nmcd.rc.right;

				GraphicsMisc::DrawHorzLine(pDC, 0, rRow.right, rRow.bottom, ELC_GRIDCOLOR);
			}

			*pResult = CDRF_SKIPDEFAULT;
			return TRUE;
		}
	}

	return FALSE;
}

void CEnListCtrl::DrawGroupHeader(CDC* pDC, CRect& rRow, const CString& sText, COLORREF crBack)
{
	GraphicsMisc::DrawGroupHeaderRow(pDC, *this, rRow, sText, CLR_NONE, crBack);
}

void CEnListCtrl::OnHeaderCustomDraw(NMHDR* pNMHDR, LPARAM* lResult)
{
	NMCUSTOMDRAW* pNMCD = (NMCUSTOMDRAW*)pNMHDR;
	*lResult = CDRF_DODEFAULT;

	// draw sort direction
	if (m_header.GetItemCount())
	{
		switch (pNMCD->dwDrawStage)
		{
		case CDDS_PREPAINT:
			*lResult = CDRF_NOTIFYITEMDRAW;
			break;
			
		case CDDS_ITEMPREPAINT:
			if ((int)pNMCD->dwItemSpec == m_nSortColumn)
				*lResult = CDRF_NOTIFYPOSTPAINT;
			break;
			
		case CDDS_ITEMPOSTPAINT:
			if ((int)pNMCD->dwItemSpec == m_nSortColumn)
			{
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);
				m_header.DrawItemSortArrow(pDC, m_nSortColumn, m_bSortAscending);
			}
			break;
		}
	}
}

void CEnListCtrl::SetSortColumn(int nCol, BOOL bResort)
{
	if (!m_bSortingEnabled)
		return;

	if (nCol != m_nSortColumn)
	{
		m_nSortColumn = nCol;
		m_bSortAscending = TRUE;

		if (m_header.GetSafeHwnd())
			m_header.Invalidate(FALSE);
	}
	else
	{
		m_bSortAscending = !m_bSortAscending;
	}

	if (bResort)
		Sort();
}

void CEnListCtrl::Sort()
{
	if (BuildSortMap(m_nSortColumn))
	{
		SortItems(CompareProc, (LPARAM)this);
		EnsureVisible(GetCurSel(), FALSE);

		m_mapSortStrings.RemoveAll();
	}
}

int CALLBACK CEnListCtrl::CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParam)
{
	const CEnListCtrl* pListCtrl = (const CEnListCtrl*)lParam;
	ASSERT (pListCtrl->IsKindOf(RUNTIME_CLASS(CEnListCtrl)));

	int nResult = pListCtrl->CompareItems(lParam1, lParam2, pListCtrl->m_nSortColumn);

	if (!pListCtrl->m_bSortAscending)
		nResult = -nResult;

	return nResult;
}

BOOL CEnListCtrl::BuildSortMap(int nCol)
{
	// because we can't reliably get back from the itemdata to the item index
	// during a sort, we map the itemdata of each item index directly to
	// the column string
	return BuildSortMap(nCol, m_mapSortStrings);
}

BOOL CEnListCtrl::BuildSortMap(int nCol, CMap<DWORD, DWORD, CString, CString&>& mapSortStrings) const
{
	int nItem = GetItemCount();

	if (nItem < 2)
		return FALSE;

	mapSortStrings.RemoveAll();

	while (nItem--)
		mapSortStrings[GetItemData(nItem)] = GetItemText(nItem, nCol);

	return TRUE;
}

CString CEnListCtrl::GetSortString(DWORD dwItemData) const
{
	CString sItem;
	m_mapSortStrings.Lookup(dwItemData, sItem);

	return sItem;
}

int CEnListCtrl::CompareItems(DWORD dwItemData1, DWORD dwItemData2, int /*nSortColumn*/) const
{
	// default comparison just compares text
	CString sItem1, sItem2;

	m_mapSortStrings.Lookup(dwItemData1, sItem1); 
	m_mapSortStrings.Lookup(dwItemData2, sItem2);

	int nCompare = CompareEmptiness(sItem1.IsEmpty(), sItem2.IsEmpty());

	if (nCompare != 0)
		return nCompare;

	// else
	return sItem1.CompareNoCase(sItem2);
}

int CEnListCtrl::CompareEmptiness(BOOL bItem1Empty, BOOL bItem2Empty) const
{
	if (m_bSortEmptyBelow)
	{
		if (bItem1Empty && bItem2Empty)
			return 0;

		if (bItem1Empty)
			return m_bSortAscending ? 1 : -1;

		if (bItem2Empty)
			return m_bSortAscending ? -1 : 1;
	}

	return 0;
}

CString CEnListCtrl::GetSortText(DWORD dwItemData) const // this is for derived classes
{
	CString sText;

	m_mapSortStrings.Lookup(dwItemData, sText);

	return sText;
}

BOOL CEnListCtrl::SetColumnText(int nCol, LPCTSTR szText)
{
	LV_COLUMN lvc = { 0 };
	lvc.mask = LVCF_TEXT;
	lvc.pszText = (LPTSTR)szText;

	return SetColumn(nCol, &lvc);
}

void CEnListCtrl::OnDestroy() 
{
	if (m_header.GetSafeHwnd())
		m_header.UnsubclassWindow();

	m_bInitColumns = FALSE;

	CListCtrl::OnDestroy();
}

BOOL CEnListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	
	return CListCtrl::PreCreateWindow(cs);
}

int CEnListCtrl::GetColumnCount() const
{
	if (GetHeader())
		return GetHeader()->GetItemCount();

	// else
	LV_COLUMN lvc = { 0 };
	lvc.mask = LVCF_WIDTH;
	int nCol = 0;

	while (GetColumn(nCol, &lvc))
		nCol++;

	return nCol;
}

void CEnListCtrl::SetItemIndent(int nItem, int nIndent)
{
	LV_ITEM lvi =  { 0 };
	
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_INDENT;
	lvi.iIndent = nIndent;
	
	VERIFY(SetItem(&lvi));
}

int CEnListCtrl::GetItemIndent(int nItem) const
{
	LV_ITEM lvi =  { 0 };
	
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_INDENT;
	
	return GetItem(&lvi) ? lvi.iIndent : 0;
}

void CEnListCtrl::PreSubclassWindow() 
{
	CListCtrl::PreSubclassWindow();

	if (m_nCurView == -1)
		m_nCurView = (GetStyle() & LVS_TYPEMASK);
}

BOOL CEnListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	if (GetItemCount() && GetView() != LVS_REPORT)
		return CListCtrl::OnEraseBkgnd(pDC);
	
	// else we do all the work in OnPaint
	return TRUE;
}

void CEnListCtrl::EnableAlternateRowColoring(BOOL bEnable)
{
	if (m_bAlternateRowColoring != bEnable)
	{
		m_bAlternateRowColoring = bEnable;

		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

