// tabctrlex.cpp : implementation file
//

#include "stdafx.h"
#include "tabctrlex.h"

#include "GraphicsMisc.h"
#include "autoflag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR  STR_CLOSEBTN	= _T("r");
const COLORREF RED			= RGB(200, 90, 90);
const COLORREF WHITE		= RGB(240, 240, 240);

const UINT WM_TCMUPDATETABWIDTH = (WM_USER + 1);

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx

CTabCtrlEx::CTabCtrlEx(DWORD dwFlags, ETabOrientation orientation) 
	: 
	CXPTabCtrl(orientation), 
	m_nDragTab(-1), 
	m_nDropTab(-1),
	m_nDropPos(-1),
	m_bDragging(FALSE),
	m_dwFlags(dwFlags), 
	m_nBtnDown(VK_CANCEL), 
	m_nMouseInCloseButton(-1),
	m_sizeClose(0, 0),
	m_bUpdatingTabWidth(FALSE),
	m_bFirstPaint(TRUE)
{
}

CTabCtrlEx::~CTabCtrlEx()
{
}

BEGIN_MESSAGE_MAP(CTabCtrlEx, CXPTabCtrl)
	//{{AFX_MSG_MAP(CTabCtrlEx)
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGING, OnTabSelChange)
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnTabSelChange)
//	ON_MESSAGE(TCM_SETCURSEL, OnTabSelChange)
	ON_MESSAGE(TCM_INSERTITEM, OnChangeTabItem)
	ON_MESSAGE(TCM_SETITEM, OnChangeTabItem)
	ON_MESSAGE(TCM_DELETEITEM, OnChangeTabItem)
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnTabSelChange)
	ON_MESSAGE(WM_TCMUPDATETABWIDTH, OnUpdateTabItemWidth)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx message handlers

BOOL CTabCtrlEx::ModifyFlags(DWORD dwRemove, DWORD dwAdd)
{
	DWORD dwPrevFlags = m_dwFlags;

	m_dwFlags &= ~dwRemove;
	m_dwFlags |= dwAdd;

	if (m_dwFlags != dwPrevFlags)
	{
		if ((dwPrevFlags & (TCE_CLOSEBUTTON | TCE_BOLDSELTEXT)) !=
			(m_dwFlags & (TCE_CLOSEBUTTON | TCE_BOLDSELTEXT)))
		{
			Invalidate();
		}

		return TRUE;
	}

	//no change
	return FALSE;
}

BOOL CTabCtrlEx::NeedCustomPaint() const
{
	BOOL bPostDraw = (m_dwFlags & (TCE_POSTDRAW | TCE_CLOSEBUTTON));

	return (bPostDraw || IsExtendedTabThemedXP());
}

BOOL CTabCtrlEx::OnTabSelChange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	if (m_dwFlags & (TCE_BOLDSELTEXT | TCE_CLOSEBUTTON))
		UpdateTabItemWidths(TRUE); // selected tab

	*pResult = 0;
	return FALSE; // continue routing
}

LRESULT CTabCtrlEx::OnChangeTabItem(WPARAM /*wp*/, LPARAM /*lp*/)
{
	LRESULT lr = Default();

	if (!m_bUpdatingTabWidth)
		UpdateTabItemWidths(FALSE); // all tabs

	return lr;
}

void CTabCtrlEx::UpdateTabItemWidths(BOOL bSel)
{
	PostMessage(WM_TCMUPDATETABWIDTH, bSel);
}

LRESULT CTabCtrlEx::OnUpdateTabItemWidth(WPARAM wp, LPARAM /*lp*/)
{
	CAutoFlag af(m_bUpdatingTabWidth, TRUE);

	if (wp) // selected tab
	{
		int nSel = GetCurSel();
		CString sTab = GetRequiredTabText(nSel);

		SetItemText(nSel, sTab);
	}
	else // all tabs
	{
		for (int nTab = 0; nTab < GetItemCount(); nTab++)
		{
			CString sTab = GetRequiredTabText(nTab);
			SetItemText(nTab, sTab);
		}
	}

	return 0L;
}

CString CTabCtrlEx::GetRequiredTabText(int nTab) 
{
	CString sTab(GetItemText(nTab));
	sTab.TrimRight();
	sTab.TrimLeft();

	int nExtra = 0;

	// add space for close button
	if (HasFlag(TCE_CLOSEBUTTON) && WantTabCloseButton(nTab))
		nExtra += (m_sizeClose.cx + 2);

	// add space for bold font
	if ((nTab == GetCurSel()) && HasFlag(TCE_BOLDSELTEXT))
	{
		// handle '&'
		sTab.Replace(_T("&"), _T("&&"));

		int nWidth = GraphicsMisc::GetTextWidth(sTab, *this);
		int nBoldWidth = GraphicsMisc::GetTextWidth(sTab, *this, GetTabFont(nTab));

		nExtra += (nBoldWidth - nWidth);

		// restore '&'
		sTab.Replace(_T("&&"), _T("&"));
	}

	if (nExtra)
	{
		// calculate the sixe of a space char
		const LPCTSTR SPACE = _T(" ");

		int nSpaceWidth = GraphicsMisc::GetTextWidth(SPACE, *this, GetTabFont(nTab));
		ASSERT(nSpaceWidth);

		sTab += CString(' ', (nExtra / nSpaceWidth));
	}

	// add leading spaces if no image
	if (GetItemImage(nTab) == -1)
		sTab = (_T("   ") + sTab);

	return sTab;
}

BOOL CTabCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	// If we are custom painting do background in OnPaint to reduce flicker
	if (NeedCustomPaint())
		return TRUE;

	// else default
	return CXPTabCtrl::OnEraseBkgnd(pDC);
}

void CTabCtrlEx::OnPaint() 
{
	if (!NeedCustomPaint())
	{
		Default();
		return;
	}

	CPaintDC dc(this); // device context for painting

	// default drawing
	if (IsExtendedTabThemedXP())
	{
		CXPTabCtrl::DoPaint(&dc); // handles bkgnd
	}
	else // default
	{
		DefWindowProc(WM_ERASEBKGND, (WPARAM)dc.m_hDC, 0L);
		DefWindowProc(WM_PRINTCLIENT, (WPARAM)dc.m_hDC, 0L);
	}

	// then post draw if required
	if (m_dwFlags & (TCE_POSTDRAW | TCE_CLOSEBUTTON))
	{
		CRect rClip;
		dc.GetClipBox(rClip);

		// paint the tabs 
		int nTab = GetItemCount();
		int nSel = GetCurSel();
		
		while (nTab--)
		{
			if (nTab != nSel)
				PostDrawTab(dc, nTab, FALSE, rClip);
		}
		
		// now selected tab
		if (nSel != -1)
			PostDrawTab(dc, nSel, TRUE, rClip);

		// kludge because until the first paint, it doesn't
		// seem possible to resize the tabs correctly
		if (m_bFirstPaint)
		{
			m_bFirstPaint = FALSE;
			UpdateTabItemWidths();
		}

	}

	// draw drop marker
	DrawTabDropMark(&dc);
}


CFont* CTabCtrlEx::GetTabFont(int nTab)
{
	// first time init of selected tab font
	if (HasFlag(TCE_BOLDSELTEXT) && (nTab == GetCurSel()))
	{
		if (!m_fontBold.GetSafeHandle())
			VERIFY(GraphicsMisc::CreateFont(m_fontBold, (HFONT)SendMessage(WM_GETFONT), GMFS_BOLD, GMFS_BOLD));

		return &m_fontBold;
	}
	
	// else
	return CXPTabCtrl::GetFont();
}

CRect CTabCtrlEx::GetTabTextRect(int nTab, LPCRECT pRect)
{
	CRect rTab(pRect);

	// handle close button
	if (HasFlag(TCE_CLOSEBUTTON) && WantTabCloseButton(nTab))
		rTab.right -= m_sizeClose.cx;

	return rTab;
}

void CTabCtrlEx::PostDrawTab(CDC& dc, int nTab, BOOL bSelected, const CRect& rClip)
{
	BOOL bPostDraw = HasFlag(TCE_POSTDRAW);
	BOOL bCloseBtn = (HasFlag(TCE_CLOSEBUTTON) && WantTabCloseButton(nTab));

	if (!(bPostDraw || bCloseBtn))
		return;
	
	// check for anything to draw
	CRect rTab;
	VERIFY(GetItemRect(nTab, rTab));

	if (bSelected)
		rTab.bottom += 2;
	else
		rTab.DeflateRect(2, 2);
				
	if (!CRect().IntersectRect(rClip, rTab))
		return;

	// post draw first
	if (bPostDraw)
	{
		DRAWITEMSTRUCT dis = { 0 };
		
		dis.CtlType = ODT_TAB;
		dis.CtlID = GetDlgCtrlID();
		dis.hwndItem = GetSafeHwnd();
		dis.hDC = dc;
		dis.itemAction = ODA_DRAWENTIRE;
		dis.itemID = nTab;
		dis.itemState = (bSelected ? ODS_SELECTED : 0);
		dis.rcItem = rTab;

		GetParent()->SendMessage(WM_DRAWITEM, dis.CtlID, (LPARAM)&dis);
	}
	
	// then close button
	if (bCloseBtn)
		DrawTabCloseButton(dc, nTab);
}

void CTabCtrlEx::DrawTabCloseButton(CDC& dc, int nTab)
{
	ASSERT(HasFlag(TCE_CLOSEBUTTON));

	int nSaveDC = dc.SaveDC();
	
	// create font first time
	if (m_fontClose.GetSafeHandle() == NULL)
		GraphicsMisc::CreateFont(m_fontClose, _T("Marlett"), 6);

	dc.SelectObject(&m_fontClose);
	
	// calc button size first time
	if (!m_sizeClose.cx || !m_sizeClose.cy)
	{
		m_sizeClose = dc.GetTextExtent(STR_CLOSEBTN);

		// make height same as width
		m_sizeClose.cy = m_sizeClose.cx + 1;
	}

	// set the color to white-on-red if the cursor is over the 'x' else gray
	CRect rBtn;
	VERIFY(GetTabCloseButtonRect(nTab, rBtn));
	
	if (m_nMouseInCloseButton == nTab)
	{
		dc.SetTextColor(WHITE);
		dc.FillSolidRect(rBtn, RED);
	}
	else
	{
		dc.SetTextColor(GetSysColor(COLOR_3DDKSHADOW));
		
#ifdef _DEBUG
//		dc.FillSolidRect(rBtn, 0);
#endif
	}
	
	dc.SetTextAlign(TA_TOP | TA_LEFT);
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(rBtn.left + 1, rBtn.top + 1, STR_CLOSEBTN);
	dc.RestoreDC(nSaveDC);

}

void CTabCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (m_dwFlags & TCE_POSTDRAW)
		return; // ignore because we probably sent it

	CXPTabCtrl::DrawItem(lpDrawItemStruct);
}

void CTabCtrlEx::OnMButtonDown(UINT nFlags, CPoint point) 
{
	OnButtonDown(VK_MBUTTON, nFlags, point);
}

void CTabCtrlEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	CXPTabCtrl::OnMouseMove(nFlags, point);

	if (HasFlag(TCE_DRAGDROP) && m_bDragging)
	{
		DrawTabDropMark(NULL); // hide old pos

		m_nDropTab = GetTabDropIndex(point, m_nDropPos);
		DrawTabDropMark(NULL); // draw new pos
	}
	else	
	{
		int nTab = HitTestCloseButton(point);
	
		if (m_nMouseInCloseButton != nTab)
		{
			CRect rBtn;
	
			if (nTab != -1)
			{
				VERIFY(GetTabCloseButtonRect(nTab, rBtn));
				InvalidateRect(rBtn, FALSE);
	
				// track when the cursor leaves the tab
				TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, m_hWnd, 0 };
				TrackMouseEvent(&tme);
			}
	
			if (m_nMouseInCloseButton != -1)
			{
				VERIFY(GetTabCloseButtonRect(m_nMouseInCloseButton, rBtn));
				InvalidateRect(rBtn, FALSE);
			}
	
			m_nMouseInCloseButton = nTab;
		}
	}	
}

LRESULT CTabCtrlEx::OnMouseLeave(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (m_nMouseInCloseButton != -1)
	{
		CRect rBtn;
		VERIFY(GetTabCloseButtonRect(m_nMouseInCloseButton, rBtn));

		InvalidateRect(rBtn, FALSE);
	}

	// cancel tracking
	TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE | TME_CANCEL, m_hWnd, 0 };
	TrackMouseEvent(&tme);
	
	m_nMouseInCloseButton = -1;

	return Default();
}

void CTabCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	OnButtonDown(VK_LBUTTON, nFlags, point);
}

void CTabCtrlEx::OnButtonDown(UINT nBtn, UINT /*nFlags*/, CPoint point) 
{
	int nHitTab = HitTest(point);

	if (nHitTab != -1)
	{
		switch (nBtn)
		{
		case VK_LBUTTON:
			if (HasFlag(TCE_CLOSEBUTTON) && (HitTestCloseButton(point) != -1))
			{
				m_nBtnDown = nBtn;
				m_ptBtnDown = point;

				SetCapture();

				// eat so that it does not cause a selection change
				return;
			}
			else if (HasFlag(TCE_DRAGDROP) && (GetItemCount() > 1) && ::DragDetect(GetSafeHwnd(), point))
			{
				m_bDragging = TRUE;
				m_nDragTab = nHitTab;
				m_nDropTab = GetTabDropIndex(point, m_nDropPos);
				m_ptBtnDown = point;

				SetCapture();

				DrawTabDropMark(NULL);

				// eat so that it does not cause a selection change
				return;
			}
			break;

		case VK_MBUTTON:
			if (HasFlag(TCE_MBUTTONCLICK) || HasFlag(TCE_MBUTTONCLOSE))
			{
				m_nBtnDown = nBtn;
				m_ptBtnDown = point;
				
				SetCapture();
			}
			break;

		default:
			ASSERT(0);
		}
	}

	// this might be LBtn or MBtn
	Default();
}

void CTabCtrlEx::OnButtonUp(UINT nBtn, UINT nFlags, CPoint point) 
{
	NMTABCTRLEX nmtce = { 0 };
	BOOL bTabMoved = FALSE;

	if (HasFlag(TCE_DRAGDROP) && m_bDragging)
	{
		ASSERT(GetItemCount() > 1);

		m_nDropTab = GetTabDropIndex(point, m_nDropPos);
		bTabMoved = HasTabMoved();

		Invalidate(FALSE);

		// notify parent
		if (bTabMoved)
		{
			nmtce.iTab = m_nDragTab;
			nmtce.hdr.code = TCN_ENDDRAG;

			// calculating number of positions needs care
			nmtce.nExtra = (m_nDropTab - m_nDragTab);

			if (m_nDropTab > m_nDragTab)
				nmtce.nExtra--;
		}
		// fall thru
	}	

	if (!bTabMoved && IsValidClick(nBtn, point))
	{
		switch (nBtn)
		{
		case VK_LBUTTON:
			{
				if (HasFlag(TCE_CLOSEBUTTON))
				{
					int nTab = HitTestCloseButton(point);
					
					if (nTab >= 0)
					{
						nmtce.iTab = nTab;
						nmtce.hdr.code = TCN_CLOSETAB;

						break;
					}
				}

				// test for selection change
				int nHitTab = HitTest(point);

				if ((nHitTab != -1) && (nHitTab != GetCurSel()))
				{
					// notify parent
					nmtce.hdr.code = TCN_SELCHANGING;
					nmtce.hdr.hwndFrom = GetSafeHwnd();
					nmtce.hdr.idFrom = GetDlgCtrlID();

					BOOL bDisallow = GetParent()->SendMessage(WM_NOTIFY, nmtce.hdr.idFrom, (LPARAM)&(nmtce.hdr));

					if (bDisallow)
					{
						nmtce.hdr.code = 0;
					}
					else
					{
						SetCurSel(nHitTab);
						nmtce.hdr.code = TCN_SELCHANGE;
					}
				}
			}
			break;
			
		case VK_MBUTTON:
			{
				int nTab = HitTest(point);
				
				if (nTab >= 0)
				{
					if (HasFlag(TCE_MBUTTONCLOSE))
					{
						nmtce.iTab = nTab;
						nmtce.hdr.code = TCN_CLOSETAB;
					}
					else if (HasFlag(TCE_MBUTTONCLICK))
					{
						nmtce.iTab = nTab;
						nmtce.nExtra = nFlags;
						nmtce.hdr.code = TCN_MCLICK;
					}
				}
			}
			break;
			
		default:
			ASSERT(0);
			break;
		}
	}

	// always
	ReleaseCapture();

	if (nmtce.hdr.code)
	{
		nmtce.hdr.hwndFrom = GetSafeHwnd();
		nmtce.hdr.idFrom = GetDlgCtrlID();
		
		GetParent()->SendMessage(WM_NOTIFY, nmtce.hdr.idFrom, (LPARAM)&nmtce);
	}
}

int CTabCtrlEx::HitTest(TCHITTESTINFO* pHitTestInfo) const
{
	return CXPTabCtrl::HitTest(pHitTestInfo);
}

int CTabCtrlEx::HitTest(const CPoint& point) const
{
	TCHITTESTINFO tchi = { { point.x, point.y }, 0 };

	return HitTest(&tchi);
}

int CTabCtrlEx::HitTestCloseButton(const CPoint& point) const
{
	int nTab = HitTest(point);
				
	if (nTab >= 0)
	{
		CRect rBtn;
		VERIFY(GetTabCloseButtonRect(nTab, rBtn));
		
		if (!rBtn.PtInRect(point))
			nTab = -1;
	}
	
	return nTab;
}

BOOL CTabCtrlEx::GetTabCloseButtonRect(int nTab, CRect& rBtn) const
{
	if (!GetItemRect(nTab, rBtn))
		return FALSE;

	int nSel = GetCurSel();

	BOOL bSel = (nTab == nSel);

	switch (GetOrientation())
	{
	case e_tabTop:
		rBtn.left = rBtn.right - m_sizeClose.cx;
		rBtn.bottom = rBtn.top + m_sizeClose.cy;
		
		if (bSel)
		{
			rBtn.OffsetRect(0, 1);
		}
		else
		{
 			rBtn.OffsetRect(-2, 3);
		}
		break;

	case e_tabBottom:
		rBtn.left = rBtn.right - m_sizeClose.cx;
		rBtn.top = rBtn.bottom - m_sizeClose.cy;
		
		if (bSel)
		{
			// rBtn.OffsetRect(0, 1);
		}
		else
		{
			rBtn.OffsetRect(-2, -2);
		}
		break;
		
	case e_tabLeft:
		break;
		
	case e_tabRight:
		break;
		
	case e_tabNOF:
	default:
		return FALSE;
	}
	
	return TRUE;
}

void CTabCtrlEx::OnMButtonUp(UINT nFlags, CPoint point) 
{
	OnButtonUp(VK_MBUTTON, nFlags, point);
}

void CTabCtrlEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	OnButtonUp(VK_LBUTTON, nFlags, point);
}

void CTabCtrlEx::OnCaptureChanged(CWnd *pWnd) 
{
	m_nBtnDown = VK_CANCEL;
	m_bDragging = FALSE;
	m_nMouseInCloseButton = -1;
	m_nDragTab = m_nDropTab = m_nDropPos = -1;
	m_ptBtnDown = 0;

	CXPTabCtrl::OnCaptureChanged(pWnd);
}

BOOL CTabCtrlEx::IsValidClick(UINT nBtn, const CPoint& ptUp) const
{
	switch (nBtn)
	{
	case VK_LBUTTON:
		if (!HasFlag(TCE_CLOSEBUTTON))
			return FALSE;
		break;
		
	case VK_MBUTTON:
		if (!(HasFlag(TCE_MBUTTONCLICK) || HasFlag(TCE_MBUTTONCLOSE)))
			return FALSE;
		break;
		
	default:
		ASSERT(0);
		return FALSE;
	}

	int nXBorder = GetSystemMetrics(SM_CXDOUBLECLK) / 2;
	int nYBorder = GetSystemMetrics(SM_CYDOUBLECLK) / 2;
	
	CRect rect(m_ptBtnDown, CSize(0, 0));
	rect.InflateRect(nXBorder, nYBorder, (nXBorder + 1), (nYBorder + 1));

	return rect.PtInRect(ptUp);
}

BOOL CTabCtrlEx::SetItemText(int nTab, LPCTSTR szText)
{
	TCITEM tci = { TCIF_TEXT, 0 };
	tci.pszText = (LPTSTR)szText;

	return SetItem(nTab, &tci);
}

CString CTabCtrlEx::GetItemText(int nTab) const
{
	TCITEM tci = { TCIF_TEXT, 0 };

	CString sText;
	tci.pszText = sText.GetBuffer(128);
	tci.cchTextMax = 128;

	GetItem(nTab, &tci);
	sText.ReleaseBuffer();

	return sText;
}

BOOL CTabCtrlEx::SetItemImage(int nTab, int nImage)
{
	TCITEM tci;
	tci.mask = TCIF_IMAGE;
	tci.iImage = nImage;
	
	return SetItem(nTab, &tci);
}

int CTabCtrlEx::GetItemImage(int nTab) const
{
	TCITEM tci = { TCIF_IMAGE, 0 };
	
	if (GetItem(nTab, &tci))
		return tci.iImage;

	// else
	return -1;
}

DWORD CTabCtrlEx::GetItemData(int nTab) const
{
	TCITEM tci = { TCIF_PARAM, 0 };

	if (GetItem(nTab, &tci))
		return tci.lParam;

	// else
	return 0;
}

BOOL CTabCtrlEx::SetItemData(int nTab, DWORD dwItemData)
{
	TCITEM tci = { TCIF_PARAM, 0 };
	tci.lParam = dwItemData;

	return SetItem(nTab, &tci);
}

int CTabCtrlEx::FindItemByData(DWORD dwItemData) const
{
	int nTab = GetItemCount();

	while (nTab--)
	{
		if (GetItemData(nTab) == dwItemData)
			return nTab;
	}

	return -1;
}

int CTabCtrlEx::GetTabDropIndex(CPoint point, int& nDropPos) const
{
	ASSERT(HasFlag(TCE_DRAGDROP));
	
	if (!HasFlag(TCE_DRAGDROP))
		return -1;
	
	int nNumTab = GetItemCount();

	if (nNumTab == 0)
		return -1;

	// note: that the drop index can be past the
	// last tab
	CRect rTab;

	for (int nTab = 0; nTab < nNumTab; nTab++)
	{
		if (GetItemRect(nTab, rTab) && rTab.PtInRect(point))
		{
			nDropPos = rTab.left;
			return nTab;
		}
	}
	
	// handle beyond last tab
	rTab.left = rTab.right;
	rTab.right += 100;

	if (rTab.PtInRect(point))
	{
		nDropPos = rTab.left;
		return nNumTab;
	}
	// and before first tab
	else if (GetItemRect(0, rTab))
	{
		rTab.right = rTab.left;
		rTab.left -= 100;

		if (rTab.PtInRect(point))
		{
			nDropPos = rTab.right;
			return 0;
		}
	}

	return -1;
}

void CTabCtrlEx::DrawTabDropMark(CDC* pDC)
{
	if (!(m_dwFlags & TCE_POSTDRAW) || !m_bDragging)
		return;

	// must have a valid/different drop tab
	//TRACE(_T("CTabCtrlEx::DrawTabDropMark(Drag = %d, Drop = %d)\n"), m_nDragTab, m_nDropTab);
	
	if (HasTabMoved())
	{
		BOOL bReleaseDC = (pDC == NULL);
		
		if (bReleaseDC)
			pDC = GetDC();
		
		CRect rTab;
		GetItemRect(0, rTab); // only need top and bottom
		
		if (m_nDropTab == 0) // special case
			rTab.left = m_nDropPos;
		else
			rTab.left = (m_nDropPos - 2);
		
		rTab.right = (rTab.left + 2);
		
		pDC->SetROP2(R2_NOT);
		pDC->SelectStockObject(BLACK_PEN);
		pDC->Rectangle(rTab);
		
		if (bReleaseDC)
			ReleaseDC(pDC);
	}
}

BOOL CTabCtrlEx::HasTabMoved() const
{
	ASSERT((m_dwFlags & TCE_POSTDRAW) && m_bDragging);
	
	if (!(m_dwFlags & TCE_POSTDRAW) || !m_bDragging)
		return FALSE;
	
	// else
	return ((m_nDropTab >= 0) &&
			((m_nDropTab < m_nDragTab) || (m_nDropTab > (m_nDragTab + 1))));
}