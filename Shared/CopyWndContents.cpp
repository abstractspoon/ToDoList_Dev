#include "StdAfx.h"
#include "CopyWndContents.h"

#include "treectrlhelper.h"

/////////////////////////////////////////////////////////////////////////////////////

CCopyWndContents::CCopyWndContents(CWnd& wnd) : m_wnd(wnd)
{
}

CCopyWndContents::~CCopyWndContents()
{
}

BOOL CCopyWndContents::DoCopy(CBitmap& bmp)
{
	ASSERT(bmp.GetSafeHandle() == NULL);

	m_sizeContent = CalcContentsSize();
	m_sizePage = CalcPageSize();

	int nHeaderHeight = CalcHeaderHeight();

	ASSERT((m_sizeContent.cx > 0) && (m_sizeContent.cy > 0));
	ASSERT((m_sizePage.cx > 0) && (m_sizePage.cy > 0));

	int nNumHorzPages = (m_sizeContent.cx / m_sizePage.cx);
	int nNumVertPages = ((m_sizeContent.cy - nHeaderHeight) / m_sizePage.cy);
	
	if (m_sizeContent.cx % m_sizePage.cx)
		nNumHorzPages++;

	if (m_sizeContent.cy % m_sizePage.cy)
		nNumVertPages++;

	// create a temp dc to paint on
	CDC* pDC = m_wnd.GetDC();
	CDC dcContent, dcPage;
	CBitmap bmpPage;

	if (dcContent.CreateCompatibleDC(pDC) && dcPage.CreateCompatibleDC(pDC))
	{
		if (bmp.CreateCompatibleBitmap(pDC, m_sizeContent.cx, m_sizeContent.cy) &&
			bmpPage.CreateCompatibleBitmap(pDC, m_sizePage.cx, m_sizePage.cy))
		{
			// Move to origin
			m_wnd.SendMessage(WM_HSCROLL, SB_LEFT);
			m_wnd.SendMessage(WM_VSCROLL, SB_TOP);

			// prepare bitmaps
			CBitmap* pOldContent = dcContent.SelectObject(&bmp);
			dcContent.FillSolidRect(0, 0, m_sizeContent.cx, m_sizeContent.cy, GetSysColor(COLOR_WINDOW));

			CBitmap* pOldPage = dcPage.SelectObject(&bmpPage);
			CPoint ptPagePos(0, 0);

			for (int nVert = 0; nVert < nNumVertPages; nVert++)
			{
				for (int nHorz = 0; nHorz < nNumHorzPages; nHorz++)
				{
					dcPage.FillSolidRect(0, 0, m_sizePage.cx, m_sizePage.cy, GetSysColor(COLOR_WINDOW));
					DoPrint(dcPage, ptPagePos.x, ptPagePos.y);

					// Copy page to correct content position
					CPoint ptDCOrg = dcPage.GetWindowOrg();
					dcPage.SetWindowOrg(0, 0);

					dcContent.BitBlt(ptPagePos.x, ptPagePos.y, m_sizePage.cx, m_sizePage.cy, &dcPage, ptDCOrg.x, ptDCOrg.y, SRCCOPY);

					// Scroll one page to right
					ptPagePos.x = PageRight(ptPagePos.x);
				}

				// Reset horizontal scrollbar
				m_wnd.SendMessage(WM_HSCROLL, SB_LEFT);
				ptPagePos.x = 0;

				// Scroll one page down
				ptPagePos.y = PageDown(ptPagePos.y);
			}

			// cleanup
			dcContent.SelectObject(pOldContent);
			dcPage.SelectObject(pOldPage);
		}
	}

	m_wnd.ReleaseDC(pDC);

	return TRUE;
}

void CCopyWndContents::DoPrint(CDC& dc, int /*nHPos*/, int /*nVPos*/)
{
	DoPrint(m_wnd, dc);
}

void CCopyWndContents::DoPrint(HWND hwnd, HDC hdc, DWORD dwFlags)
{
	::SendMessage(hwnd, WM_PRINT, (WPARAM)hdc, (LPARAM)dwFlags);
}

int CCopyWndContents::PageDown(int nCurVertPos)
{
	DoPageDown();

	int nScrollPos = GetContentVScrollPos();

	if (nScrollPos > 0)
		return nScrollPos;

	int nNewVertPos = (nCurVertPos + m_sizePage.cy);
	nNewVertPos = min(nNewVertPos, m_sizeContent.cy - m_sizePage.cy);

	return nNewVertPos;
}

int CCopyWndContents::PageRight(int nCurHorzPos)
{
	DoPageRight();

	int nScrollPos = GetContentHScrollPos();

	if (nScrollPos > 0)
		return nScrollPos;

	// else
	int nNewHorzPos = (nCurHorzPos + m_sizePage.cx);
	nNewHorzPos = min(nNewHorzPos, (m_sizeContent.cx - m_sizePage.cx));

	return nNewHorzPos;
}

void CCopyWndContents::DoPageDown()
{
	m_wnd.SendMessage(WM_VSCROLL, SB_PAGEDOWN);
}

void CCopyWndContents::DoPageRight()
{
	m_wnd.SendMessage(WM_HSCROLL, SB_PAGERIGHT);
}

CSize CCopyWndContents::CalcContentsSize() const
{
	CRect rClient;
	m_wnd.GetClientRect(rClient);

	CSize sizeContents(rClient.Width(), rClient.Height());

	if (m_wnd.GetStyle() & WS_HSCROLL)
	{
		SCROLLINFO siHorz = { 0 };
		m_wnd.GetScrollInfo(SB_HORZ, &siHorz);

		sizeContents.cx = (siHorz.nMax - siHorz.nMin + 1);
	}

	if (m_wnd.GetStyle() & WS_VSCROLL)
	{
		SCROLLINFO siVert = { 0 };
		m_wnd.GetScrollInfo(SB_VERT, &siVert);

		sizeContents.cy = (siVert.nMax - siVert.nMin + 1);
	}

	return sizeContents;
}

CSize CCopyWndContents::CalcPageSize() const
{
	CRect rClient;
	m_wnd.GetClientRect(rClient);

	return rClient.Size();
}

int CCopyWndContents::GetContentVScrollPos() const
{
	int nPos = 0;

	if (m_wnd.GetStyle() & WS_VSCROLL)
	{
		SCROLLINFO siVert = { 0 };
		m_wnd.GetScrollInfo(SB_VERT, &siVert);

		nPos = siVert.nPos;
	}

	return nPos;
}

int CCopyWndContents::GetContentHScrollPos() const
{
	int nPos = 0;

	if (m_wnd.GetStyle() & WS_HSCROLL)
	{
		SCROLLINFO siHorz = { 0 };
		m_wnd.GetScrollInfo(SB_HORZ, &siHorz);

		nPos = siHorz.nPos;
	}

	return nPos;
}

/////////////////////////////////////////////////////////////////////////////////////

CCopyTreeCtrlContents::CCopyTreeCtrlContents(CTreeCtrl& tree) : CCopyWndContents(tree), m_tree(tree)
{
	HTREEITEM hti = (HTREEITEM)m_wnd.SendMessage(TVM_GETNEXTITEM, NULL, TVGN_CHILD);
	ASSERT(hti);

	m_nItemHeight = TreeView_GetItemHeight(m_wnd);
}

CCopyTreeCtrlContents::~CCopyTreeCtrlContents()
{

}

void CCopyTreeCtrlContents::DoPageDown()
{
	int nLine = (GetPageSize().cy / m_nItemHeight);

	while (nLine--)
		m_wnd.SendMessage(WM_VSCROLL, SB_LINEDOWN);
}

CSize CCopyTreeCtrlContents::CalcContentsSize() const
{
	CSize sizeContent(CCopyWndContents::CalcContentsSize());

	sizeContent.cx++;

	CTreeCtrlHelper tch(m_tree);
	int nTotalVisible = tch.BuildVisibleIndexMap();
	sizeContent.cy = (nTotalVisible * m_nItemHeight);

	return sizeContent;
}

CSize CCopyTreeCtrlContents::CalcPageSize() const
{
	CSize sizePage(CCopyWndContents::CalcPageSize());

	int nPageVisible = m_tree.GetVisibleCount();
	sizePage.cy = (nPageVisible * m_nItemHeight);

	return sizePage;
}

int CCopyTreeCtrlContents::GetContentVScrollPos() const
{
	int nPos = CCopyWndContents::GetContentVScrollPos();

	if (nPos != 0)
		return (nPos * m_nItemHeight);

	// else
	HTREEITEM hti = m_tree.GetFirstVisibleItem();
	CRect rItem;

	if (hti && m_tree.GetItemRect(hti, rItem, FALSE))
		return rItem.top;

	// else
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

CCopyHeaderCtrlContents::CCopyHeaderCtrlContents(CHeaderCtrl& hdr) : CCopyWndContents(hdr)
{
}

CCopyHeaderCtrlContents::~CCopyHeaderCtrlContents()
{
}

/////////////////////////////////////////////////////////////////////////////////////

CCopyListCtrlContents::CCopyListCtrlContents(CListCtrl& list) : CCopyWndContents(list), m_list(list)
{
	CRect rect;
	
	if (m_list.GetItemRect(0, &rect, LVIR_BOUNDS))
		m_nItemHeight = rect.Height();
	else
		m_nItemHeight = 10;
}

CCopyListCtrlContents::~CCopyListCtrlContents()
{

}

int CCopyListCtrlContents::PageDown(int nCurVertPos)
{
	int nNewVPos = CCopyWndContents::PageDown(nCurVertPos);

	if (nCurVertPos == 0)
		nNewVPos += CalcHeaderHeight();

	return nNewVPos;
}


void CCopyListCtrlContents::DoPageDown()
{
	int nLine = m_list.GetCountPerPage();

	while (nLine--)
		m_wnd.SendMessage(WM_VSCROLL, SB_LINEDOWN);
}

CSize CCopyListCtrlContents::CalcContentsSize() const
{
	CSize sizeContent(CCopyWndContents::CalcContentsSize());

	// for whole lines
	int nTotalVisible = m_list.GetItemCount();
	sizeContent.cy = (nTotalVisible * m_nItemHeight);

	// add height of header
	sizeContent.cy += CalcHeaderHeight();

	return sizeContent;
}

CSize CCopyListCtrlContents::CalcPageSize() const
{
	CSize sizePage(CCopyWndContents::CalcPageSize());

// 	// deduct height of header
// 	sizePage.cy -= CalcHeaderHeight();
// 
// 	// adjust for whole lines
 	int nHeaderHeight = CalcHeaderHeight();
 	sizePage.cy = ((m_list.GetCountPerPage() * m_nItemHeight) + nHeaderHeight);
//  	sizePage.cy = ((((sizePage.cy - nHeaderHeight) / m_nItemHeight) * m_nItemHeight) + nHeaderHeight);

	return sizePage;
}

int CCopyListCtrlContents::CalcHeaderHeight() const
{
	HWND hwndHdr = ListView_GetHeader(m_wnd);

	if (hwndHdr && ::IsWindowVisible(hwndHdr))
	{
		CRect rHeader;
		VERIFY(::GetWindowRect(hwndHdr, rHeader));

		return rHeader.Height();
	}

	// else
	return 0;
}

void CCopyListCtrlContents::DoPrint(CDC& dc, int nHPos, int nVPos)
{
	CPoint ptOrg = dc.GetWindowOrg();


	CCopyWndContents::DoPrint(m_wnd, dc, PRF_CLIENT | PRF_CHILDREN);

	if (nVPos != 0)
		dc.SetWindowOrg(ptOrg.x, CalcHeaderHeight());

// 	if (nVPos != 0)
// 		dc.SetWindowOrg(ptOrg);
}

int CCopyListCtrlContents::GetContentVScrollPos() const
{
	int nPos = CCopyWndContents::GetContentVScrollPos();

	return (nPos * m_nItemHeight);
}

/////////////////////////////////////////////////////////////////////////////////////

CCopyEditContents::CCopyEditContents(CEdit& edit) 
	: 
	CCopyWndContents(edit), 
	m_nLineHeight(-1), 
	m_nLineCount(-1)
{
	int nFirstCharLine0 = edit.LineIndex(0);
	int nFirstCharLine1 = edit.LineIndex(1);

	CPoint ptTopLeftLine0 = edit.PosFromChar(nFirstCharLine0);
	CPoint ptTopLeftLine1 = edit.PosFromChar(nFirstCharLine1);

	m_nLineHeight = (ptTopLeftLine1 - ptTopLeftLine0).cy;
	m_nLineCount = edit.GetLineCount();
}

CCopyEditContents::~CCopyEditContents()
{

}

void CCopyEditContents::DoPageDown()
{
	int nLine = (GetPageSize().cy / m_nLineHeight);

	while (nLine--)
		m_wnd.SendMessage(WM_VSCROLL, SB_LINEDOWN);
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
	m_wnd.SendMessage(EM_GETRECT, 0, (LPARAM)&rect);

	return CSize(rect.Width(), ((rect.Height() / m_nLineHeight) * m_nLineHeight));
}

/////////////////////////////////////////////////////////////////////////////////////
