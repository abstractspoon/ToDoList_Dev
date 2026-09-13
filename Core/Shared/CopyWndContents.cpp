#include "StdAfx.h"

#include "CopyWndContents.h"
#include "treectrlhelper.h"
#include "WinClasses.h"
#include "wclassdefines.h"

/////////////////////////////////////////////////////////////////////////////////////

CCopyWndContents::CCopyWndContents(HWND hWnd) : m_hWnd(hWnd)
{
}

CCopyWndContents::~CCopyWndContents()
{
}

BOOL CCopyWndContents::DoCopy(CBitmap& bmp, const CRect& rFromTo)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(bmp.GetSafeHandle() == NULL);

	m_sizeContent = CalcContentsSize();
	m_sizePage = CalcPageSize();

	ASSERT((m_sizeContent.cx > 0) && (m_sizeContent.cy > 0));
	ASSERT((m_sizePage.cx > 0) && (m_sizePage.cy > 0));

	int nNumHorzPages = CalcPageCount(FALSE);
	int nNumVertPages = CalcPageCount(TRUE);

	// Adjust content size to match passed-in rect
	if (rFromTo.right != -1)
		m_sizeContent.cx = rFromTo.Width();
	else
		m_sizeContent.cx -= rFromTo.left;
	
	if (rFromTo.bottom != -1)
		m_sizeContent.cy = rFromTo.Height();
	else
		m_sizeContent.cy -= rFromTo.top;

	ASSERT((m_sizeContent.cx > 0) && (m_sizeContent.cy > 0));

	// create a temp dc to paint on
	CDC* pDC = CDC::FromHandle(::GetDC(m_hWnd));
	CDC dcContent, dcPage;
	CBitmap bmpPage;

	if (dcContent.CreateCompatibleDC(pDC) && dcPage.CreateCompatibleDC(pDC))
	{
		if (bmp.CreateCompatibleBitmap(pDC, m_sizeContent.cx, m_sizeContent.cy) &&
			bmpPage.CreateCompatibleBitmap(pDC, m_sizePage.cx, m_sizePage.cy))
		{
			// Move to origin
			::SendMessage(m_hWnd, WM_HSCROLL, SB_LEFT, 0L);
			::SendMessage(m_hWnd, WM_VSCROLL, SB_TOP, 0L);

			// prepare bitmaps
			CBitmap* pOldContent = dcContent.SelectObject(&bmp);
			dcContent.FillSolidRect(0, 0, m_sizeContent.cx, m_sizeContent.cy, GetSysColor(COLOR_WINDOW));

			CBitmap* pOldPage = dcPage.SelectObject(&bmpPage);
			CPoint ptPagePos(0, 0);

			for (int nVert = 0; nVert < nNumVertPages; nVert++)
			{
				// Only draw pages within passed-in rect
				if ((rFromTo.bottom != -1) && (ptPagePos.y >= rFromTo.bottom))
					break;

				if ((ptPagePos.y + m_sizePage.cy) > rFromTo.top)
				{
					for (int nHorz = 0; nHorz < nNumHorzPages; nHorz++)
					{
						// Only draw pages within passed-in rect
						if ((rFromTo.right != -1) && (ptPagePos.x >= rFromTo.right))
							break;

						if ((ptPagePos.x + m_sizePage.cx) > rFromTo.left)
						{
							// Copy the current page
							dcPage.FillSolidRect(0, 0, m_sizePage.cx, m_sizePage.cy, GetSysColor(COLOR_WINDOW));
							DoPrint(dcPage, ptPagePos.x, ptPagePos.y);

							// GetWindowOrg contains the point from which
							// we copy the page bitmap to the content bitmap
							CPoint ptDCOrg = dcPage.GetWindowOrg();
							dcPage.SetWindowOrg(0, 0);

							dcContent.BitBlt((ptPagePos.x - rFromTo.left), 
											(ptPagePos.y - rFromTo.top), 
											m_sizePage.cx, 
											m_sizePage.cy, 
											&dcPage, 
											ptDCOrg.x, 
											ptDCOrg.y, 
											SRCCOPY);
						}

						// Scroll one page to right
						if (nHorz < (nNumHorzPages - 1))
							ptPagePos.x = PageRight(ptPagePos.x);
					}

					// Reset horizontal scrollbar
					::SendMessage(m_hWnd, WM_HSCROLL, SB_LEFT, 0L);
					ptPagePos.x = 0;
				}

				// Scroll one page down
				if (nVert < (nNumVertPages - 1))
					ptPagePos.y = PageDown(ptPagePos.y);
			}

			// cleanup
			dcContent.SelectObject(pOldContent);
			dcPage.SelectObject(pOldPage);
		}
	}

	::ReleaseDC(m_hWnd, *pDC);

	return (bmp.GetSafeHandle() != NULL);
}

void CCopyWndContents::DoPrint(CDC& dc, int /*nHPage*/, int /*nVPage*/)
{
	DoPrint(m_hWnd, dc);
}

void CCopyWndContents::DoPrint(HWND hwnd, HDC hdc, DWORD dwFlags)
{
	::SendMessage(hwnd, WM_PRINT, (WPARAM)hdc, (LPARAM)dwFlags);
}

int CCopyWndContents::PageDown(int nCurVertPos)
{
	DoPageDown();

	int nScrollPos = GetContentScrollPos(TRUE);

	if (nScrollPos > 0)
		return nScrollPos;

	int nNewVertPos = (nCurVertPos + m_sizePage.cy);
	nNewVertPos = min(nNewVertPos, m_sizeContent.cy - m_sizePage.cy);

	return nNewVertPos;
}

int CCopyWndContents::PageRight(int nCurHorzPos)
{
	DoPageRight();

	int nScrollPos = GetContentScrollPos(FALSE);

	if (nScrollPos > 0)
		return nScrollPos;

	// else
	int nNewHorzPos = (nCurHorzPos + m_sizePage.cx);
	nNewHorzPos = min(nNewHorzPos, (m_sizeContent.cx - m_sizePage.cx));

	return nNewHorzPos;
}

void CCopyWndContents::DoPageDown()
{
	::SendMessage(m_hWnd, WM_VSCROLL, SB_PAGEDOWN, 0L);
}

void CCopyWndContents::DoPageRight()
{
	::SendMessage(m_hWnd, WM_HSCROLL, SB_PAGERIGHT, 0L);
}

BOOL CCopyWndContents::HasStyle(DWORD dwStyle) const 
{ 
	return ((::GetWindowLong(m_hWnd, GWL_STYLE) & dwStyle) == dwStyle); 
}

CSize CCopyWndContents::CalcContentsSize() const
{
	CRect rClient;
	::GetClientRect(m_hWnd, rClient);

	CSize sizeContents(rClient.Width(), rClient.Height());

	if (HasStyle(WS_HSCROLL))
	{
		SCROLLINFO siHorz = { sizeof(SCROLLINFO), SIF_RANGE, 0 };
		::GetScrollInfo(m_hWnd, SB_HORZ, &siHorz);

		sizeContents.cx = (siHorz.nMax - siHorz.nMin + 1);
	}

	if (HasStyle(WS_VSCROLL))
	{
		SCROLLINFO siVert = { sizeof(SCROLLINFO), SIF_RANGE, 0 };
		::GetScrollInfo(m_hWnd, SB_VERT, &siVert);

		sizeContents.cy = (siVert.nMax - siVert.nMin + 1);
	}

	sizeContents.cx = max(1, sizeContents.cx);
	sizeContents.cy = max(1, sizeContents.cy);

	return sizeContents;
}

CSize CCopyWndContents::CalcPageSize() const
{
	CRect rClient;
	::GetClientRect(m_hWnd, rClient);

	rClient.right = max(1, rClient.right);
	rClient.bottom = max(1, rClient.bottom);

	return rClient.Size();
}

int CCopyWndContents::CalcPageCount(BOOL bVert) const
{
	CSize sizeContent = CalcContentsSize();
	CSize sizePage = CalcPageSize();

	int nCount = 0;

	if (bVert && (sizePage.cy > 0))
	{
		nCount = (sizeContent.cy / sizePage.cy);

		if (sizeContent.cy % sizePage.cy)
			nCount++;
	}
	else if (!bVert && (sizePage.cx > 0))
	{
		nCount = (sizeContent.cx / sizePage.cx);

		if (sizeContent.cx % sizePage.cx)
			nCount++;
	}

	return max(nCount, 0);
}

int CCopyWndContents::GetContentScrollPos(BOOL bVert) const
{
	SCROLLINFO si = { sizeof(SCROLLINFO), SIF_POS, 0 };

	if (bVert && HasStyle(WS_VSCROLL))
	{
		VERIFY(::GetScrollInfo(m_hWnd, SB_VERT, &si));
	}
	else if (!bVert && HasStyle(WS_HSCROLL))
	{
		VERIFY(::GetScrollInfo(m_hWnd, SB_HORZ, &si));
	}

	return si.nPos;
}

/////////////////////////////////////////////////////////////////////////////////////

CCopyTreeCtrlContents::CCopyTreeCtrlContents(HWND hWnd) 
	: 
	CCopyWndContents(hWnd),
	m_nItemHeight(-1)
{
	ASSERT(CWinClasses::IsClass(hWnd, WC_TREEVIEW));

	HTREEITEM hti = (HTREEITEM)SendMessage(m_hWnd, TVM_GETNEXTITEM, NULL, TVGN_CHILD);
	ASSERT(hti);

	m_nItemHeight = TreeView_GetItemHeight(m_hWnd);
}

CCopyTreeCtrlContents::~CCopyTreeCtrlContents()
{

}

BOOL CCopyTreeCtrlContents::DoCopy(CBitmap& bmp, const CRect& rFromTo)
{
	return CCopyWndContents::DoCopy(bmp, rFromTo);
}

void CCopyTreeCtrlContents::DoPageDown()
{
	int nLine = (GetPageSize().cy / m_nItemHeight);

	while (nLine--)
		::SendMessage(m_hWnd, WM_VSCROLL, SB_LINEDOWN, 0L);
}

CSize CCopyTreeCtrlContents::CalcContentsSize() const
{
	CSize sizeContents(CCopyWndContents::CalcContentsSize());

	sizeContents.cx++;

	// Temporary for CTreeCtrlHelper
	CTreeCtrl tree;
	tree.m_hWnd = m_hWnd;

	CTreeCtrlHelper tch(tree);

	HTREEITEM htiFirst = tch.GetFirstItem();
	HTREEITEM htiLast = tch.GetLastVisibleItem();

	CRect rFirst, rLast;

	if (TreeView_GetItemRect(m_hWnd, htiFirst, &rFirst, FALSE) &&
		TreeView_GetItemRect(m_hWnd, htiLast, &rLast, FALSE))
	{
		sizeContents.cy = (rLast.bottom - rFirst.top);
	}
	else
	{
		int nTotalVisible = tch.BuildVisibleIndexMap();
		sizeContents.cy = (nTotalVisible * m_nItemHeight);
	}

	sizeContents.cy = max(1, sizeContents.cy);
	tree.m_hWnd = NULL;

	return sizeContents;
}

CSize CCopyTreeCtrlContents::CalcPageSize() const
{
	CSize sizePage(CCopyWndContents::CalcPageSize());
	int nPageVisible = TreeView_GetVisibleCount(m_hWnd);

	sizePage.cy = (nPageVisible * m_nItemHeight);
	sizePage.cy = max(1, sizePage.cy);

	return sizePage;
}

int CCopyTreeCtrlContents::GetContentScrollPos(BOOL bVert) const
{
	int nPos = CCopyWndContents::GetContentScrollPos(bVert);

	if (bVert)
	{
		if (nPos != 0)
		{
			nPos *= m_nItemHeight;
		}
		else
		{
			HTREEITEM hti = TreeView_GetFirstVisible(m_hWnd);
			CRect rItem;
			
			if (hti && TreeView_GetItemRect(m_hWnd, hti, &rItem, FALSE))
				nPos = rItem.top;
		}
	}

	return nPos;
}

/////////////////////////////////////////////////////////////////////////////////////

CCopyHeaderCtrlContents::CCopyHeaderCtrlContents(HWND hWnd) : CCopyWndContents(hWnd)
{
	ASSERT(CWinClasses::IsClass(hWnd, WC_HEADER));
}

CCopyHeaderCtrlContents::~CCopyHeaderCtrlContents()
{
}

/////////////////////////////////////////////////////////////////////////////////////

CCopyListCtrlContents::CCopyListCtrlContents(HWND hWnd) 
	: 
	CCopyWndContents(hWnd),
	m_bWinForms(FALSE),
	m_nItemHeight(10)
{
	m_bWinForms = CWinClasses::IsWinFormsControl(hWnd, WC_LISTVIEW);
	ASSERT(m_bWinForms || CWinClasses::IsClass(hWnd, WC_LISTVIEW));

	CRect rect;
	
	if (ListView_GetItemRect(m_hWnd, 0, &rect, LVIR_BOUNDS))
		m_nItemHeight = rect.Height();
}

CCopyListCtrlContents::~CCopyListCtrlContents()
{

}

BOOL CCopyListCtrlContents::DoCopy(CBitmap& bmp, const CRect& rFromTo)
{
	return CCopyWndContents::DoCopy(bmp, rFromTo);
}

int CCopyListCtrlContents::PageDown(int nCurVertPos)
{
	ASSERT(!HasStyle(LVS_NOSCROLL));

	int nNewVPos = CCopyWndContents::PageDown(nCurVertPos);

	return (nNewVPos + CalcHeaderHeight());
}

int CCopyListCtrlContents::CalcPageCount(BOOL bVert) const
{
	if (HasStyle(LVS_NOSCROLL))
		return 1;

	if (!bVert)
		return CCopyWndContents::CalcPageCount(FALSE);

	// else
	int nHeaderHeight = CalcHeaderHeight();
	int nSizeContent = CalcContentsSize().cy - nHeaderHeight;
	int nSizePage = CalcPageSize().cy - nHeaderHeight;

	if (nSizePage == 0)
		return 1;

	// else
	int nCount = (nSizeContent / nSizePage);

	if (nSizeContent % nSizePage)
		nCount++;

	return max(nCount, 1);
}

void CCopyListCtrlContents::DoPageDown()
{
	ASSERT(!HasStyle(LVS_NOSCROLL));

	// Avoid animations by not using WM_VSCROLL
	ListView_Scroll(m_hWnd, 0, (ListView_GetCountPerPage(m_hWnd) * m_nItemHeight));
}

void CCopyListCtrlContents::DoPageRight()
{
	ASSERT(!HasStyle(LVS_NOSCROLL));

	// Avoid animations by not using WM_VHCROLL
	CRect rClient;
	::GetClientRect(m_hWnd, rClient);

	ListView_Scroll(m_hWnd, rClient.Width(), 0);
}

CSize CCopyListCtrlContents::CalcContentsSize() const
{
	if (HasStyle(LVS_NOSCROLL))
		return CalcPageSize();

	CSize sizeContents(CCopyWndContents::CalcContentsSize());

	// for whole lines
	int nTotalVisible = ListView_GetItemCount(m_hWnd);
	sizeContents.cy = (nTotalVisible * m_nItemHeight);

	// add height of header
	sizeContents.cy += CalcHeaderHeight();

	// Restrict width to total width of columns
	HWND hwndHdr = ListView_GetHeader(m_hWnd);

	if (hwndHdr)
	{
		int nCol = Header_GetItemCount(hwndHdr);
		int nWidth = (m_bWinForms ? 2 : 0); // FUDGE

		while (nCol--)
			nWidth += ListView_GetColumnWidth(m_hWnd, nCol);

		sizeContents.cx = min(sizeContents.cx, nWidth);
	}

	sizeContents.cx = max(1, sizeContents.cx);
	sizeContents.cy = max(1, sizeContents.cy);

	return sizeContents;
}

CSize CCopyListCtrlContents::CalcPageSize() const
{
	CSize sizePage(CCopyWndContents::CalcPageSize());

	if (!HasStyle(LVS_NOSCROLL))
	{
		// adjust for header and whole lines
		int nHeaderHeight = CalcHeaderHeight();
		int nCountPerPage = ListView_GetCountPerPage(m_hWnd);

		if (nCountPerPage > 0)
		{
			sizePage.cy = ((nCountPerPage * m_nItemHeight) + nHeaderHeight);
		}
		else // item height > page size
		{
			sizePage.cy += nHeaderHeight;
		}
		sizePage.cy = max(1, sizePage.cy);
	}

	return sizePage;
}

int CCopyListCtrlContents::CalcHeaderHeight() const
{
	if (!HasStyle(LVS_NOSCROLL))
	{
		HWND hwndHdr = ListView_GetHeader(m_hWnd);

		if (hwndHdr && ::IsWindowVisible(hwndHdr))
		{
			CRect rHeader;
			VERIFY(::GetWindowRect(hwndHdr, rHeader));

			return rHeader.Height();
		}
	}

	// else
	return 0;
}

void CCopyListCtrlContents::DoPrint(CDC& dc, int /*nHPage*/, int nVPage)
{
	CPoint ptOrg = dc.GetWindowOrg();

	CCopyWndContents::DoPrint(m_hWnd, dc, PRF_CLIENT | PRF_CHILDREN);

	if (CWinClasses::IsWinFormsControl(m_hWnd) && (nVPage == 0))
	{
		int nSaveDC = dc.SaveDC();

		dc.SetWindowOrg(ptOrg.x + 2, ptOrg.y);
		dc.ExcludeClipRect(CRect(CPoint(0, CalcHeaderHeight()), GetContentsSize()));

		CCopyWndContents::DoPrint(m_hWnd, dc, PRF_CLIENT | PRF_CHILDREN);

		dc.RestoreDC(nSaveDC);
	}
	else if (nVPage && !HasStyle(LVS_NOSCROLL))
	{
		dc.SetWindowOrg(ptOrg.x, CalcHeaderHeight());
	}
}

int CCopyListCtrlContents::GetContentScrollPos(BOOL bVert) const
{
	if (HasStyle(LVS_NOSCROLL))
		return 0;

	int nPos = CCopyWndContents::GetContentScrollPos(bVert);

	if (bVert)
	{
		if (nPos != 0)
		{
			nPos *= m_nItemHeight;
		}
		else
		{
			int nItem = ListView_GetTopIndex(m_hWnd);
			CRect rItem;
			
			if ((nItem != -1) && ListView_GetItemRect(m_hWnd, nItem, rItem, LVIR_BOUNDS))
				nPos = rItem.top;
		}
	}

	return nPos;
}

/////////////////////////////////////////////////////////////////////////////////////

CCopyEditContents::CCopyEditContents(HWND hWnd) 
	: 
	CCopyWndContents(hWnd), 
	m_nLineHeight(-1), 
	m_nLineCount(-1)
{
	ASSERT(CWinClasses::IsClass(hWnd, WC_EDIT));

	CEdit edit;
	edit.m_hWnd = hWnd;

	int nFirstCharLine0 = edit.LineIndex(0);
	int nFirstCharLine1 = edit.LineIndex(1);

	CPoint ptTopLeftLine0 = edit.PosFromChar(nFirstCharLine0);
	CPoint ptTopLeftLine1 = edit.PosFromChar(nFirstCharLine1);

	m_nLineHeight = (ptTopLeftLine1 - ptTopLeftLine0).cy;
	m_nLineCount = edit.GetLineCount();

	edit.m_hWnd = NULL;
}

CCopyEditContents::~CCopyEditContents()
{

}

void CCopyEditContents::DoPageDown()
{
	int nLine = (GetPageSize().cy / m_nLineHeight);

	while (nLine--)
		SendMessage(m_hWnd, WM_VSCROLL, SB_LINEDOWN, 0L);
}

CSize CCopyEditContents::CalcContentsSize() const
{
	CSize sizeContent(CCopyWndContents::CalcContentsSize());

	sizeContent.cx++;
	sizeContent.cy = (m_nLineCount * m_nLineHeight) + 2;

	return sizeContent;
}

CSize CCopyEditContents::CalcPageSize() const
{
	CRect rect;
	::SendMessage(m_hWnd, EM_GETRECT, 0, (LPARAM)&rect);

	return CSize(rect.Width(), ((rect.Height() / m_nLineHeight) * m_nLineHeight));
}

/////////////////////////////////////////////////////////////////////////////////////
