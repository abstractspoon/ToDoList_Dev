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

	ASSERT((m_sizeContent.cx > 0) && (m_sizeContent.cy > 0));
	ASSERT((m_sizePage.cx > 0) && (m_sizePage.cy > 0));

	int nNumHorzPages = (m_sizeContent.cx / m_sizePage.cx);
	int nNumVertPages = (m_sizeContent.cy / m_sizePage.cy);
	
	if (m_sizeContent.cx % m_sizePage.cx)
		nNumHorzPages++;

	if (m_sizeContent.cy % m_sizePage.cy)
		nNumVertPages++;

	// create a temp dc to paint on
	CDC* pDC = m_wnd.GetDC();
	CDC dcTemp;

	if (dcTemp.CreateCompatibleDC(pDC))
	{
		if (bmp.CreateCompatibleBitmap(pDC, m_sizeContent.cx, m_sizeContent.cy))
		{
			m_wnd.SendMessage(WM_HSCROLL, SB_LEFT);
			m_wnd.SendMessage(WM_VSCROLL, SB_TOP);

			CBitmap* pOld = dcTemp.SelectObject(&bmp);
			dcTemp.FillSolidRect(0, 0, m_sizeContent.cx, m_sizeContent.cy, GetSysColor(COLOR_WINDOW));

			CPoint ptWinOrg(0, 0);

			for (int nVert = 0; nVert < nNumVertPages; nVert++)
			{
				for (int nHorz = 0; nHorz < nNumHorzPages; nHorz++)
				{
					dcTemp.SetWindowOrg(ptWinOrg);
					
					DoPrint(dcTemp);

					ptWinOrg.x -= PageRight(-ptWinOrg.x);
				}

				m_wnd.SendMessage(WM_HSCROLL, SB_LEFT);
				ptWinOrg.x = 0;

				ptWinOrg.y -= PageDown(-ptWinOrg.y);
			}

			// cleanup
			dcTemp.SelectObject(pOld);
		}
	}

	m_wnd.ReleaseDC(pDC);

	return TRUE;
}

void CCopyWndContents::DoPrint(CDC& dc)
{
	DoPrint(m_wnd, dc);
}

void CCopyWndContents::DoPrint(HWND hwnd, HDC hdc, DWORD dwFlags)
{
	::SendMessage(hwnd, WM_PRINT, (WPARAM)hdc, (LPARAM)dwFlags);
}

int CCopyWndContents::PageDown(int nCurVertPos)
{
	int nNewVertPos = (nCurVertPos + m_sizePage.cy);
	nNewVertPos = min(nNewVertPos, m_sizeContent.cy);

	DoPageDown();

	return (nNewVertPos - nCurVertPos);
}

int CCopyWndContents::PageRight(int nCurHorzPos)
{
	int nNewHorzPos = (nCurHorzPos + m_sizePage.cx);
	nNewHorzPos = min(nNewHorzPos, (m_sizeContent.cx - m_sizePage.cx));

	DoPageRight();

	return (nNewHorzPos - nCurHorzPos - 1);
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

	//sizeContent.cx++;

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
	int nOffset = CCopyWndContents::PageDown(nCurVertPos);

	return (nOffset/* - CalcHeaderHeight()*/);
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

//	int nPageVisible = m_list.GetCountPerPage();
//	sizePage.cy = (nPageVisible * m_nItemHeight);

	// deduct height of header
	sizePage.cy -= CalcHeaderHeight();

	// adjust for whole lines
	sizePage.cy = ((sizePage.cy / m_nItemHeight) * m_nItemHeight);

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

void CCopyListCtrlContents::DoPrint(CDC& dc)
{
	if (dc.GetWindowOrg().y == 0)
		CCopyWndContents::DoPrint(m_wnd, dc, PRF_CLIENT | PRF_CHILDREN);
 	else
 		CCopyWndContents::DoPrint(dc);
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
