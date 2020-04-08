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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1400
#	define LVGROUPITEM LVITEM
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
#define LVM_ENABLEGROUPVIEW     (LVM_FIRST + 157)
#define LVM_REMOVEALLGROUPS     (LVM_FIRST + 160)

#endif

/////////////////////////////////////////////////////////////////////////////

const DWORD NOTSET = 0xffffffff;
DWORD CEnListCtrl::s_dwSelectionTheming = MAKELONG(TRUE, FALSE);

/////////////////////////////////////////////////////////////////////////////
// CEnListCtrl

IMPLEMENT_DYNAMIC(CEnListCtrl, CListCtrl)

#define IDC_HEADERCTRL 10001
#define WM_SHOWPOPUPMENU (WM_APP+1001)
#define ID_TIMER_HEADERPOS 1
const int MAX_HEADING_SIZE = 100;

CEnListCtrl::CEnListCtrl() : m_dwSelectionTheming(NOTSET)
{
	m_bVertGrid = m_bHorzGrid = FALSE;
	m_mapColumnData.RemoveAll();
	m_nCurView = -1;
	m_bLastColStretchy = FALSE;
	m_bFirstColStretchy = FALSE;
	m_nMinItemHeight = -1;
	m_bReadOnly = FALSE;
	m_nItemDropHilite = -1;
	m_bDropHiliteItemSelected = FALSE;
	m_bContextPopupEnabled = FALSE;
	m_bUserSelChange = FALSE;
	m_bSortingEnabled = TRUE;
	m_nSortColumn = -1;
	m_bSortAscending = TRUE;
	m_bInitColumns = FALSE;
	m_bAlternateRowColoring = FALSE;
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
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY(NM_CUSTOMDRAW, 0, OnHeaderCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnListCtrl message handlers

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
		COLORREF crBack = GetItemBackColor(0, FALSE, FALSE, FALSE);

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
				dc.DrawText( sText, rClient, DT_CENTER | DT_WORDBREAK | DT_NOPREFIX/* | DT_NOCLIP */);
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

CColumnData* CEnListCtrl::CreateColumnData(int nCol)
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

const CColumnData* CEnListCtrl::GetColumnData(int nCol) const
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

void CEnListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC;
	CRect rText, rItem, rClient, rHeader;
	COLORREF crOldText, crOldBack;
	CSize sizeText;
	LV_COLUMN lvc = { 0 };
	CImageList* pImageList;
	CImageList* pStateList;
	int nImage = -1;
	BOOL bItemFocused, bListFocused, bSelected, bDropHighlighted, bSelAlways;
	UINT uStyle, uState;
	CSize sizeState(0, 0), sizeImage(0, 0);
	int nIndent = 0;

	// get and prepare device context
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->SelectObject(GetFont());
	pDC->SetROP2(R2_COPYPEN);

	// init helper variables
	int nItem = lpDrawItemStruct->itemID;
	GetItemRect(nItem, rItem, LVIR_BOUNDS);
	GetClientRect(&rClient);

	// some problems with drophiliting items during drag and drop
	// so we need to make sure drawing is clipped to client area
	// this fixes it admirably!
	if (GetHeader())
	{
		GetHeader()->GetWindowRect(rHeader);
		ScreenToClient(rHeader);
		rClient.top = max(0, rHeader.bottom);
		pDC->IntersectClipRect(rClient);
	}

	rText = rItem;

	uStyle = GetStyle();
	uState = GetItemState(nItem, LVIS_DROPHILITED | LVIS_SELECTED);
	bDropHighlighted = (uState & LVIS_DROPHILITED);
	bItemFocused = (GetFocusedItem() == nItem);
	bListFocused = (GetFocus() == this);
	bSelAlways = ((uStyle & LVS_SHOWSELALWAYS) == LVS_SHOWSELALWAYS);
	bSelected = (uState & LVIS_SELECTED) && (bListFocused || bSelAlways);

	crOldText = pDC->SetTextColor(COLORREF(0)); // this will be overwritten on a per subitem basis
	crOldBack = pDC->SetBkColor(GetItemBackColor(nItem, bSelected, bDropHighlighted, bListFocused));

	// images and indentation
	pImageList = GetImageList(LVSIL_SMALL);

	if (pImageList)
	{
		CEnImageList::GetImageSize(*pImageList, sizeImage);
		nIndent = GetItemIndent(nItem) * sizeImage.cx;

		rText.left += nIndent;
		rItem.left += nIndent;
	}

	// state
	pStateList = GetImageList(LVSIL_STATE);

	if (pStateList)
		CEnImageList::GetImageSize(*pStateList, sizeState);

	if (lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT))
	{
		// setup colors and pens
		int nImageStyle = GetImageStyle(bSelected, bDropHighlighted, bListFocused);

		// draw item images if required
		int nImageWidth = 0;

		// make sure there is enough space
		lvc.mask = LVCF_WIDTH | LVCF_FMT;
		int nCol = 0;
		BOOL bRes = GetColumn(nCol, &lvc);
		
		// must paint the background of column 0 before the icons
		if (bRes && (pStateList || pImageList))
			pDC->ExtTextOut(0, rItem.top, ETO_CLIPPED | ETO_OPAQUE, CRect(0, rItem.top, lvc.cx, rItem.bottom), _T(""), NULL);

		// state
		if (pStateList && bRes)
		{
			int nState = (GetItemState(nItem, LVIS_STATEIMAGEMASK) & LVIS_STATEIMAGEMASK);
			nState = nState >> 12;

			if (lvc.cx > sizeState.cx)
				pStateList->Draw(pDC, nState, CPoint(rText.left + 1, rText.top), ILD_TRANSPARENT); 

			nImageWidth = sizeState.cx + 2; // 1 pixel border either side
		}

		// normal
		if (pImageList && bRes && nImage != -1)
		{
			if (lvc.cx > nImageWidth + sizeImage.cx)
				pImageList->Draw(pDC, nImage, CPoint(rText.left + 1 + nImageWidth, rText.top), nImageStyle); 

			nImageWidth += sizeImage.cx + 2; // 1 pixel border either side
		}

		// cycle thru columns formatting and drawing each subitem
		rText.left += nImageWidth;

		while (bRes)
		{
			// save width and format because GetItem overwrites
			// if first column deduct width of image if exists
			int nWidth = (nCol == 0) ? lvc.cx - nImageWidth - nIndent : lvc.cx;
			int nFormat = (lvc.fmt & LVCFMT_JUSTIFYMASK);

			// get next item
			bRes = GetColumn(nCol + 1, &lvc);

			// set pos of text rect
			if (nWidth < 0)
			{
				rText.left += nWidth;
				nWidth = 0;
			}
			rText.right = max(rText.left, rText.left + nWidth);

			// get item text and output
			CEnString sText(GetItemText(nItem, nCol));
			sizeText = sText.FormatDC(pDC, nWidth, GetColumnFormat(nCol));

			// set y pos of first char
			int nYPos = rText.top + (rText.Height() - sizeText.cy) / 2;

			// set x pos of first char
			int nXPos = 0;

			switch (nFormat)
			{
				case LVCFMT_CENTER:
					nXPos = rText.left + (rText.Width() - sizeText.cx) / 2;
					break;

				case LVCFMT_RIGHT:
					nXPos = rText.right - 2 - sizeText.cx;
					break;

				case LVCFMT_LEFT:
				default:
					nXPos = rText.left + 2;
					break;
			}

			// setup font
			CFont* pFont = GetItemFont(nItem, nCol);
			CFont* pOldFont = NULL;

			if (pFont)
				pOldFont = pDC->SelectObject(pFont);

			// setup text colour
			pDC->SetTextColor(GetItemTextColor(nItem, nCol, bSelected, bDropHighlighted, bListFocused));
			pDC->ExtTextOut(nXPos, nYPos, ETO_CLIPPED | ETO_OPAQUE, rText, sText, NULL);

			// reset font
			if (pFont)
				pDC->SelectObject(pOldFont);

			// draw vert grid if required
			if (m_bVertGrid && nCol > 0)
			{
				pDC->MoveTo(rText.left - 1, rItem.top);
				pDC->LineTo(rText.left - 1, rItem.bottom);
			}

			rText.left = rText.right;

			// next column
			nCol++;
		}

		// draw horz grid lines if required
		if (m_bHorzGrid)
		{
			pDC->MoveTo(rClient.left, rItem.bottom - 1);
			pDC->LineTo(rClient.right, rItem.bottom - 1);
		}

		// focus rect: normal method doesn't work because we are focusing whole line
		// note: if we're scrolled to the right the image may not be visible
		if (bItemFocused && bListFocused)
		{
			rItem.left += nImageWidth;
			pDC->DrawFocusRect(rItem);
		}

		pDC->SetTextColor(crOldText);
		pDC->SetBkColor(crOldBack);
	}
}

COLORREF CEnListCtrl::GetItemTextColor(int /*nItem*/, int nSubItem, 
									   BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
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

	// else
	return GetColumnTextColor(nSubItem);
}

COLORREF CEnListCtrl::GetItemBackColor(int /*nItem*/, BOOL bSelected, 
									   BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (bSelected)
	{
		if (bWndFocus)
		{
			return ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else if (GetStyle() & LVS_SHOWSELALWAYS)
		{
			return ::GetSysColor(COLOR_BTNFACE);
		}
	} 
	else if (bDropHighlighted)
	{
		return ::GetSysColor(COLOR_HIGHLIGHT);
	}

	// else
	COLORREF crBack = GetTextBkColor(); 

	crBack = (crBack != 0xff000000) ? crBack : ::GetSysColor(COLOR_WINDOW);

	return crBack;
}

int CEnListCtrl::GetImageStyle(BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	int nStyle = ILD_TRANSPARENT;

	if (bSelected && bWndFocus)
		nStyle |= ILD_BLEND25;

	else if (bDropHighlighted)
		nStyle |= ILD_BLEND25;
	
	// else
	return nStyle;
}

void CEnListCtrl::EnableHeaderTracking(BOOL bAllow)
{
	m_header.EnableTracking(bAllow);
}

void CEnListCtrl::EnableTooltipCtrl(BOOL bEnable)
{
	ASSERT (m_hWnd);

	if (m_ttCtrl.m_hWnd == NULL)
		m_ttCtrl.Create(this);
	
	m_bTooltipsEnabled = bEnable;
	m_ttCtrl.Activate(bEnable);
}

BOOL CEnListCtrl::SetTooltipCtrlText(CString sText)
{
	ASSERT (m_hWnd);

	if (sText.IsEmpty())
		return FALSE;

	if (m_ttCtrl.m_hWnd == NULL)
		m_ttCtrl.Create(this);
	
	m_ttCtrl.AddTool(this, sText);

	return TRUE;
}

void CEnListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nSel = HitTest(point);
	BOOL bChangedEditLabels = FALSE;

	// if the list is in report mode AND supports label editing then
	// clicking anywhere on the item when its already selected will
	// edit the label of column one which is not what we want. 
	// so we selectively disable it
	if ((nSel != -1) && m_nCurView == LVS_REPORT)
	{
		if (GetStyle() & LVS_EDITLABELS)
		{
			CRect rItem;
			GetSubItemRect(nSel, 0, LVIR_BOUNDS, rItem);

			if (GetImageList(LVSIL_SMALL))
				rItem.left += 19;

			if (GetImageList(LVSIL_STATE))
				rItem.left += 19;

			if ((point.x < rItem.left) || (point.x > rItem.right))
			{
				ModifyStyle(LVS_EDITLABELS, 0);
				bChangedEditLabels = TRUE;
			}
		}
	}
	
	CListCtrl::OnLButtonDown(nFlags, point);

	if (nSel != -1) // user clicked on an item
	{
		// determine whether a sel change has occured
		// and tell our parent if it has
		NotifySelChange();
	}

	if (bChangedEditLabels)
		ModifyStyle(0, LVS_EDITLABELS);
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

	// if we are in report view then we want to be ownerdraw 
	// so we can take advatage of the special drawing
	if (nView == LVS_REPORT)
	{
		// make ownerdraw
		if (!(dwStyle & LVS_OWNERDRAWFIXED))
			ModifyStyle(0, LVS_OWNERDRAWFIXED); 
	}
	// else we want default drawing
	else
	{
		// make non-ownerdraw
		if (dwStyle & LVS_OWNERDRAWFIXED)
			ModifyStyle(LVS_OWNERDRAWFIXED, 0); 
	}

	m_nCurView = nView;
	Invalidate(FALSE);

	// force a resize to ensure that the column headers are correct
	WINDOWPOS wp = { GetSafeHwnd(), NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER };
	PostMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp); 
}

int CEnListCtrl::GetImageIndex(int nItem, int nSubItem) const
{
	LV_ITEM lvi =  { 0 };

	lvi.mask = LVIF_IMAGE;
	lvi.iItem = nItem;
	lvi.iSubItem = nSubItem;
	
	GetItem(&lvi);

	return lvi.iImage;
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
		WINDOWPOS wp = { GetSafeHwnd(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER };
		PostMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp); 
	}
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
		WINDOWPOS wp = { GetSafeHwnd(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER };
		PostMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp); 
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
	static BOOL bResizingCols = FALSE;

	if (bResizingCols)
		return;

	bResizingCols = TRUE;

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
		int nCol, nNumCols, /*nColStart = 0,*/ nColEnd = 0;

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

	bResizingCols = FALSE;
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
	if (::IsWindow(m_ttCtrl.m_hWnd) && pMsg->hwnd == m_hWnd && m_bTooltipsEnabled)
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

int CEnListCtrl::CalcItemHeight() const
{
	ASSERT(GetSafeHwnd());

	int nBaseHeight = CDlgUnits(this, TRUE).ToPixelsY(9); // default edit height
	int nFontHeight = GraphicsMisc::GetFontPixelSize(GetSafeHwnd());

	return max(nBaseHeight, max(m_nMinItemHeight, nFontHeight));
}

void CEnListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    lpMeasureItemStruct->itemHeight = CalcItemHeight(); 
}

void CEnListCtrl::RefreshItemHeight()
{
	if (GetSafeHwnd())
	{
		// I've tried everything I can think of but
		// this is the only thing that seems to be able
		// to trigger a WM_MEASUREITEM message
		CRect rWindow;
		GetWindowRect(rWindow);

		GetParent()->ScreenToClient(rWindow);

		rWindow.right--;
		MoveWindow(rWindow);

		rWindow.right++;
		MoveWindow(rWindow);
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

		if (GetItemCount())
		{
			CRect rItem;
			GetItemRect(0, rItem, LVIR_BOUNDS);

			if (nHeight != rItem.Height())
				RefreshItemHeight();
		}
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

void CEnListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nSel;

	CListCtrl::OnLButtonDblClk(nFlags, point);

	nSel = GetCurSel();
	
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

void CEnListCtrl::SetItemImage(int nItem, int nImage)
{
	if (nItem < 0 || nItem >= GetItemCount())
		return;

	SetItem(nItem, 0, LVIF_IMAGE, _T(""), nImage, 0, 0, 0L);
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

	if (nCol == 0)
		rCell.left = 0;
}

void CEnListCtrl::GetCellEditRect(int nRow, int nCol, CRect& rCell) const
{
	GetCellRect(nRow, nCol, rCell);
	rCell.OffsetRect(-2, 0);
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

void CEnListCtrl::SetSortColumn(int nColumn, BOOL bResort)
{
	if (!m_bSortingEnabled)
		return;

	if (nColumn != m_nSortColumn)
	{
		m_nSortColumn = nColumn;
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
	// rebuild sort map
	BuildSortMap(m_nSortColumn);

	// do sort
	SortItems(CompareProc, (LPARAM)this);

	// cleanup
	m_mapSortStrings.RemoveAll();

	EnsureVisible(GetCurSel(), FALSE);
}

int CALLBACK CEnListCtrl::CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParam)
{
	CEnListCtrl* pListCtrl;
	int nResult;

	pListCtrl = (CEnListCtrl*)lParam;
	ASSERT (pListCtrl->IsKindOf(RUNTIME_CLASS(CEnListCtrl)));

	nResult = pListCtrl->CompareItems(lParam1, lParam2, pListCtrl->m_nSortColumn);

	if (!pListCtrl->m_bSortAscending)
		nResult = -nResult;

	return nResult;
}

void CEnListCtrl::BuildSortMap(int nCol)
{
	// because we can't reliably get back from the itemdata to the item index
	// during a sort, we map the itemdata of each item index directly to
	// the column string
	BuildSortMap(nCol, m_mapSortStrings);
}

void CEnListCtrl::BuildSortMap(int nCol, CMap<DWORD, DWORD, CString, CString&>& mapSortStrings) const
{
	mapSortStrings.RemoveAll();
	int nItem = GetItemCount();

	while (nItem--)
		mapSortStrings[GetItemData(nItem)] = GetItemText(nItem, nCol);
}

CString CEnListCtrl::GetSortString(DWORD dwItemData) const
{
	CString sItem;
	m_mapSortStrings.Lookup(dwItemData, sItem);

	return sItem;
}

int CEnListCtrl::CompareItems(DWORD dwItemData1, DWORD dwItemData2, int /*nSortColumn*/)
{
	// -1 if dwItemData1 should go BEFORE dwItemData2
	//  1 if dwItemData1 should go AFTER dwItemData2
	//  0 if it doesn't matter

	// default comparison just compares text
	CString sItem1, sItem2;

	m_mapSortStrings.Lookup(dwItemData1, sItem1); // this is quicker than helper method
	m_mapSortStrings.Lookup(dwItemData2, sItem2);

	// empty items always appear AFTER others
	if (sItem1.IsEmpty())
		return m_bSortAscending ? 1 : -1;

	else if (sItem2.IsEmpty())
		return m_bSortAscending ? -1 : 1;

	// else
	return sItem1.CompareNoCase(sItem2);
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
	
	else // we do all the work in OnPaint
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

BOOL CEnListCtrl::EnableGroupView(BOOL bEnable)
{
	return (SendMessage(LVM_ENABLEGROUPVIEW, (WPARAM)bEnable, 0) != -1);
}

BOOL CEnListCtrl::InsertGroupHeader(int nIndex, int nGroupID, const CString& strHeader/*, DWORD dwState = LVGS_NORMAL, DWORD dwAlign = LVGA_HEADER_LEFT*/)
{
	LVGROUP lvg = { 0 };

	lvg.cbSize = sizeof(lvg);
	lvg.iGroupId = nGroupID;
	lvg.state = LVGS_NORMAL;//dwState;
	lvg.mask = LVGF_GROUPID | LVGF_HEADER | LVGF_STATE | LVGF_ALIGN;
	lvg.uAlign = LVGA_HEADER_LEFT;//dwAlign;
	
	// Header-title must be unicode (Convert if necessary)
#ifdef UNICODE
	lvg.pszHeader = (LPWSTR)(LPCTSTR)strHeader;
	lvg.cchHeader = strHeader.GetLength();
#else
	CComBSTR header = strHeader;
	lvg.pszHeader = header;
	lvg.cchHeader = header.Length();
#endif

	return (S_OK == SendMessage(LVM_INSERTGROUP, (WPARAM)nIndex, (LPARAM)&lvg));
}

int CEnListCtrl::GetItemGroupId(int nRow)
{
	LVGROUPITEM lvgi = { 0 };

    lvgi.mask = LVIF_GROUPID;
    lvgi.iItem = nRow;

	VERIFY(GetItem((LVITEM*)&lvgi));

    return lvgi.iGroupId;
}

BOOL CEnListCtrl::SetItemGroupId(int nRow, int nGroupID)
{
	LVGROUPITEM lvgi = { 0 };
	
	lvgi.mask = LVIF_GROUPID;
	lvgi.iItem = nRow;
	lvgi.iSubItem = 0;
	lvgi.iGroupId = nGroupID;

	return SetItem((LVITEM*)&lvgi);
}

void CEnListCtrl::RemoveAllGroups()
{
	SendMessage(LVM_REMOVEALLGROUPS);
}
