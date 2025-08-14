// tabctrlex.cpp : implementation file
//

#include "stdafx.h"
#include "tabctrlex.h"

#include "Misc.h"
#include "GraphicsMisc.h"
#include "autoflag.h"
#include "osversion.h"

#include "..\3rdParty\XNamedColors.h"
#include "..\3rdParty\ColorDef.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int COLORBAR_WIDTH	= GraphicsMisc::ScaleByDPIFactor(3);
const int PADDING			= GraphicsMisc::ScaleByDPIFactor(2);
const int SIZE_CLOSEBTN		= (GraphicsMisc::ScaleByDPIFactor(8) + (GraphicsMisc::WantDPIScaling() ? 1 : 0));

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
	m_bUpdatingTabWidth(FALSE),
	m_bFirstPaint(TRUE)
{
	RemoveUnsupportedFlags(m_dwFlags);
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
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnTabSelChange)
	ON_MESSAGE(TCM_INSERTITEM, OnChangeTabItem)
	ON_MESSAGE(TCM_SETITEM, OnChangeTabItem)
	ON_MESSAGE(TCM_DELETEITEM, OnChangeTabItem)
	ON_MESSAGE(TCM_SETCURSEL, OnSetCurSel)
	ON_MESSAGE(WM_TCMUPDATETABWIDTH, OnUpdateTabItemWidth)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx message handlers

BOOL CTabCtrlEx::IsSupportedFlag(DWORD dwFlag)
{
	if (!IsExtendedTabThemedXP())
		return !(dwFlag & (TCE_TABCOLORS | TCE_BOLDSELTEXT));

	// else
	return TRUE;
}

void CTabCtrlEx::RemoveUnsupportedFlags(DWORD& dwFlags)
{
	if (!IsSupportedFlag(TCE_TABCOLORS))
		dwFlags &= ~TCE_TABCOLORS;

	if (!IsSupportedFlag(TCE_BOLDSELTEXT))
		dwFlags &= ~TCE_BOLDSELTEXT;
}

BOOL CTabCtrlEx::ModifyFlags(DWORD dwRemove, DWORD dwAdd)
{
	DWORD dwPrevFlags = m_dwFlags;

	Misc::ModifyFlags(m_dwFlags, dwRemove, dwAdd);
	RemoveUnsupportedFlags(m_dwFlags);

	if (Misc::FlagHasChanged(TCE_CLOSEBUTTON | TCE_BOLDSELTEXT, dwPrevFlags, m_dwFlags))
		UpdateTabItemWidths();

	return (m_dwFlags == dwPrevFlags);
}

BOOL CTabCtrlEx::NeedCustomPaint() const
{
	BOOL bPreDraw = ((m_dwFlags & TCE_TABCOLORS) && IsSupportedFlag(TCE_TABCOLORS));
	BOOL bPostDraw = (m_dwFlags & (TCE_POSTDRAW | TCE_CLOSEBUTTON));

	return (bPreDraw || bPostDraw || IsExtendedTabThemedXP());
}

BOOL CTabCtrlEx::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateTabItemWidths();

	*pResult = 0;
	return FALSE; // continue routing
}

LRESULT CTabCtrlEx::OnChangeTabItem(WPARAM wp, LPARAM lp)
{
	TC_ITEM* pItem = (TC_ITEM*)lp;
	CString sReqText;

	if (!m_bUpdatingTabWidth && pItem && (pItem->mask & TCIF_TEXT))
	{
		sReqText = GetRequiredTabText((int)wp, pItem->pszText);
		pItem->pszText = (LPTSTR)(LPCTSTR)sReqText;
	}

	return Default();
}

LRESULT CTabCtrlEx::OnSetCurSel(WPARAM wp, LPARAM lp)
{
	LRESULT lr = Default();

	UpdateTabItemWidths();

	return lr;
}

void CTabCtrlEx::UpdateTabItemWidths()
{
	if (m_dwFlags & (TCE_BOLDSELTEXT | TCE_CLOSEBUTTON))
		PostMessage(WM_TCMUPDATETABWIDTH);
}

LRESULT CTabCtrlEx::OnUpdateTabItemWidth(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT(m_dwFlags & (TCE_BOLDSELTEXT | TCE_CLOSEBUTTON));

	CAutoFlag af(m_bUpdatingTabWidth, TRUE);

	for (int nTab = 0; nTab < GetItemCount(); nTab++)
		SetItemText(nTab, GetRequiredTabText(nTab, GetItemText(nTab)));

	return 0L;
}

CString CTabCtrlEx::GetRequiredTabText(int nTab)
{
	return GetRequiredTabText(nTab, GetItemText(nTab));
}

CString CTabCtrlEx::GetRequiredTabText(int nTab, const CString& sCurText) 
{
	CString sReqText(sCurText);
	sReqText.TrimRight();
	sReqText.TrimLeft();

	int nExtra = 0;

	// add space for close button
	if (HasFlag(TCE_CLOSEBUTTON) && WantTabCloseButton(nTab))
		nExtra += (SIZE_CLOSEBTN + 2);

	// add space for bold font
	if ((nTab == GetCurSel()) && HasFlag(TCE_BOLDSELTEXT))
	{
		// handle '&'
		int nNumAmpersand = sReqText.Replace(_T("&"), _T("&&"));

		int nWidth = GraphicsMisc::GetTextWidth(sReqText, *this);
		int nBoldWidth = GraphicsMisc::GetTextWidth(sReqText, *this, GetTabFont(nTab));

		nExtra += (nBoldWidth - nWidth);

		// restore '&'
		if (nNumAmpersand)
			sReqText.Replace(_T("&&"), _T("&"));
	}
	else 
	{
		// Add space for embedded '&' which the tab control 
		// does not handle well
		int nNumAmpersand = sReqText.Replace(_T("&"), _T("&&"));

		if (nNumAmpersand)
		{
			int nWidth = GraphicsMisc::GetTextWidth(_T("&"), *this);
			nExtra += (nWidth * nNumAmpersand);

			// restore '&'
			sReqText.Replace(_T("&&"), _T("&"));
		}
	}

	if (nExtra > 0)
	{
		// calculate the size of a space char
		const LPCTSTR SPACE = _T(" ");

		int nSpaceWidth = GraphicsMisc::GetTextWidth(SPACE, *this, GetTabFont(nTab));
		ASSERT(nSpaceWidth);

		int nNumSpace = (nExtra / nSpaceWidth);

		if (nExtra % nSpaceWidth)
			nNumSpace++;

		// If the tab has no icon move one of the extra spaces to the front
		if (nNumSpace && (GetItemImage(nTab) == -1))
		{
			sReqText = ' ' + sReqText;
			nNumSpace--;
		}

		sReqText += CString(' ', nNumSpace);
	}

	return sReqText;
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
				PostDrawTab(dc, nTab, rClip);
		}
		
		// now selected tab
		if (nSel != -1)
			PostDrawTab(dc, nSel, rClip);

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
		rTab.right -= SIZE_CLOSEBTN;

	return rTab;
}

COLORREF CTabCtrlEx::GetItemBkColor(int nTab)
{
	if (HasFlag(TCE_TABCOLORS))
	{
		NMTABCTRLEX nmtce = { 0 };
		
		nmtce.iTab = nTab;
		nmtce.dwExtra = CLR_NONE;

		nmtce.hdr.code = TCN_GETBACKCOLOR;
		nmtce.hdr.hwndFrom = GetSafeHwnd();
		nmtce.hdr.idFrom = GetDlgCtrlID();
		
		if (GetParent()->SendMessage(WM_NOTIFY, nmtce.hdr.idFrom, (LPARAM)&(nmtce.hdr)))
			return nmtce.dwExtra;
	}

	// all else
	return CLR_NONE;
}

COLORREF CTabCtrlEx::GetItemTagColor(int nTab)
{
	NMTABCTRLEX nmtce = { 0 };
		
	nmtce.iTab = nTab;
	nmtce.dwExtra = CLR_NONE;

	nmtce.hdr.code = TCN_GETTAGCOLOR;
	nmtce.hdr.hwndFrom = GetSafeHwnd();
	nmtce.hdr.idFrom = GetDlgCtrlID();
		
	if (GetParent()->SendMessage(WM_NOTIFY, nmtce.hdr.idFrom, (LPARAM)&(nmtce.hdr)))
		return nmtce.dwExtra;

	// all else
	return CLR_NONE;
}

void CTabCtrlEx::GetTabContentRect(const CRect& rTab, int nTab, CRect& rContent) const
{
	rContent = rTab;

	// Default adjustment
	rContent.DeflateRect(2, 2, 1, -1);

	// Further horizontal adjustments influenced by OS
	OSVERSION nOsVer = COSVersion();
	int nOffset = (nTab - GetCurSel());

	switch (nOffset)
	{
	case -1: // == immediately before selected tab
		if (nOsVer < OSV_WIN10)
		{
			rContent.DeflateRect(0, 0, 1, 0);
		}
		else
		{
			rContent.DeflateRect(-1, 0, 1, 0);
		}
		break;

	case 0: // == selected tab
		if (nOsVer <= OSV_XPP)
		{
			rContent.DeflateRect(0, 0, -1, -1);
		}
		else if (nOsVer <= OSV_WIN7)
		{
			rContent.DeflateRect(0, 0, 0, -1);
		}
		break;

	case 1: // == immediately after selected tab
		if (nOsVer <= OSV_XPP)
		{
			rContent.DeflateRect(1, 0, 1, 0);
		}
		else // Win 7, 8, 8.1, 10
		{
			rContent.DeflateRect(1, 0, 0, 0);
		}
		break;

	default:
		if (nOsVer <= OSV_XPP)
		{
			rContent.DeflateRect(0, 0, 1, 0);
		}
		else if (nOsVer >= OSV_WIN10)
		{
			rContent.DeflateRect(-1, 0, 0, 0);
		}
		break;
	}
}

void CTabCtrlEx::DrawTabItem(CDC* pDC, int nTab, const CRect& rcItem, UINT uiFlags)
{
	CRect rTab;
	GetTabContentRect(rcItem, nTab, rTab);

	// DrawTabBackColor may modify the tab rect (to be passed to DrawTabItem)
	// but we need to preserve the original rect for DrawTabTag
	CRect rTabOrg(rTab);
	COLORREF crText(CLR_NONE);

	if (HasFlag(TCE_TABCOLORS))
		DrawTabBackColor(pDC, nTab, (uiFlags & 4), rTab, crText);

	// Then tag
	if (HasFlag(TCE_TAGCOLORS))
		DrawTabTag(pDC, nTab, rTabOrg);

	if (crText != CLR_NONE)
		pDC->SetTextColor(crText);

	CXPTabCtrl::DrawTabItem(pDC, nTab, rTab, uiFlags);
}

void CTabCtrlEx::PostDrawTab(CDC& dc, int nTab, const CRect& rClip)
{
	BOOL bPostDraw = HasFlag(TCE_POSTDRAW);
	BOOL bCloseBtn = (HasFlag(TCE_CLOSEBUTTON) && WantTabCloseButton(nTab));

	if (!(bPostDraw || bCloseBtn))
		return;
	
	// check for anything to draw
	CRect rTab;
	VERIFY(GetItemRect(nTab, rTab));

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
		dis.itemState = ((nTab == GetCurSel()) ? ODS_SELECTED : 0);
		dis.rcItem = rTab;

		// notify parent
		NMTABCTRLEX nmtce = { 0 };

		nmtce.hdr.code = TCN_POSTDRAW;
		nmtce.hdr.hwndFrom = GetSafeHwnd();
		nmtce.hdr.idFrom = GetDlgCtrlID();
		nmtce.iTab = nTab;
		nmtce.dwExtra = (DWORD)&dis;
		
		GetParent()->SendMessage(WM_NOTIFY, nmtce.hdr.idFrom, (LPARAM)&(nmtce.hdr));
	}
	
	// then close button
	if (bCloseBtn)
		DrawTabCloseButton(&dc, nTab);
}

void CTabCtrlEx::DrawTabCloseButton(CDC* pDC, int nTab)
{
	ASSERT(HasFlag(TCE_CLOSEBUTTON));

	CSaveDC sdc(pDC);

	// create font first time
	if (m_fontClose.GetSafeHandle() == NULL)
		GraphicsMisc::CreateFont(m_fontClose, _T("Marlett"), 6, GMFS_SYMBOL);

	CRect rBtn;
	GetTabCloseButtonRect(nTab, rBtn);
	
	// set the color to white-on-red if the cursor is over the 'x' else gray
	if (m_nMouseInCloseButton == nTab)
	{
		pDC->SetTextColor(colorWhite);
		pDC->FillSolidRect(rBtn, colorIndianRed);
	}
	else
	{
		COLORREF crTab = GetItemBkColor(nTab);

		if ((crTab != CLR_NONE) && (nTab != GetCurSel()))
			pDC->SetTextColor(GraphicsMisc::GetBestTextColor(crTab));
		else
			pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));
	}
	
	rBtn.OffsetRect(1, 1);
	GraphicsMisc::DrawAnsiSymbol(pDC, MARLETT_CLOSE, rBtn, DT_CENTER | DT_VCENTER, &m_fontClose);
}

void CTabCtrlEx::DrawTabTag(CDC* pDC, int nTab, const CRect& rTab)
{
	ASSERT(HasFlag(TCE_TAGCOLORS));

	COLORREF crTag = GetItemTagColor(nTab);

	if (crTag == CLR_NONE)
		return;

	CSaveDC sdc(pDC);

	const int TAG_SIZE = GraphicsMisc::ScaleByDPIFactor(6);

	for (int nHPos = 0; nHPos < TAG_SIZE; nHPos++)
	{
		for (int nVPos = 0; nVPos < TAG_SIZE - nHPos; nVPos++)
		{
			pDC->SetPixelV(rTab.left + nHPos, rTab.top + nVPos, crTag);
		}
	}

	// draw a dividing line if the tab has colour and the 
	// difference in luminance is small(ish)
	COLORREF crTab = GetItemBkColor(nTab);

	if ((crTab != CLR_NONE) && (crTab != colorWhite))
	{
		int nTabLum = RGBX(crTab).Luminance();
		int nTagLum = RGBX(crTag).Luminance();

		if (fabs(nTagLum - nTabLum) < 128)
		{
			int nAveLum = ((nTabLum + nTagLum) / 2);

			pDC->SelectStockObject((nAveLum > 128) ? BLACK_PEN : WHITE_PEN);
			pDC->MoveTo(rTab.left, rTab.top + TAG_SIZE);
			pDC->LineTo(rTab.left + TAG_SIZE + 1, rTab.top - 1);
		}
	}
}

void CTabCtrlEx::DrawTabBackColor(CDC* pDC, int nTab, BOOL bHot, CRect& rTab, COLORREF& crText)
{
	ASSERT(HasFlag(TCE_TABCOLORS));

	COLORREF crTab = GetItemBkColor(nTab);

	if (crTab == CLR_NONE)
		return;

	CSaveDC sdc(pDC);
	CRect rColor(rTab);

	// Show the selected tab as a thin bar only
	// and adjust tab height to make space
	if (nTab == GetCurSel())
	{
		crText = GetSysColor(COLOR_BTNTEXT);

		int nCloseBtnSize = 0;

		if (HasFlag(TCE_CLOSEBUTTON) && WantTabCloseButton(nTab))
			nCloseBtnSize = (SIZE_CLOSEBTN + PADDING);

		switch (m_eTabOrientation)
		{
		case e_tabTop:
			rColor.bottom = (rColor.top + COLORBAR_WIDTH);
			rColor.right -= nCloseBtnSize;
			rColor.left += PADDING;
			rTab.top += (COLORBAR_WIDTH - PADDING);
			break;

		case e_tabBottom:
			rColor.top = (rColor.bottom - COLORBAR_WIDTH);
			rColor.right -= nCloseBtnSize;
			rColor.left += PADDING;
			rTab.bottom -= (COLORBAR_WIDTH - PADDING);
			break;

		case e_tabLeft:
			rColor.right = (rColor.left + COLORBAR_WIDTH);
			rColor.top += nCloseBtnSize;
			rColor.bottom += PADDING;
			rTab.left += (COLORBAR_WIDTH - PADDING);
			break;

		case e_tabRight:
			rColor.left = (rColor.right - COLORBAR_WIDTH);
			rColor.top += nCloseBtnSize;
			rColor.bottom -= PADDING;
			rTab.right -= (COLORBAR_WIDTH - PADDING);
			break;
		}

		GraphicsMisc::DrawRect(pDC, rColor, crTab);
	}
	else
	{
		crText = GraphicsMisc::GetBestTextColor(crTab);

		if (bHot)
			crTab = GraphicsMisc::Lighter(crTab, 0.3, TRUE);

		GraphicsMisc::DrawRect(pDC, rColor, crTab, CLR_NONE, 2);
	}
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
			nmtce.dwExtra = (m_nDropTab - m_nDragTab);

			if (m_nDropTab > m_nDragTab)
				nmtce.dwExtra--;
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
						nmtce.dwExtra = nFlags;
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
		rBtn.left = rBtn.right - SIZE_CLOSEBTN;
		rBtn.bottom = rBtn.top + SIZE_CLOSEBTN;
		
		if (bSel)
			rBtn.OffsetRect(0, 1);
		else
 			rBtn.OffsetRect(-2, 2);
		break;

	case e_tabBottom:
		rBtn.left = rBtn.right - SIZE_CLOSEBTN;
		rBtn.top = rBtn.bottom - SIZE_CLOSEBTN;
		
		if (!bSel)
			rBtn.OffsetRect(-2, -2);
		break;
		
	case e_tabLeft:
		break;
		
	case e_tabRight:
		break;
		
	case e_tabNOF:
	default:
		return FALSE;
	}

	if (nTab == (nSel - 1))
		rBtn.OffsetRect(-1, 0);
		
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
	tci.pszText = sText.GetBuffer(256);
	tci.cchTextMax = 256;

	GetItem(nTab, &tci);
	sText.ReleaseBuffer();

	return sText;
}

BOOL CTabCtrlEx::SetItemImage(int nTab, int nImage)
{
	TCITEM tci = { TCIF_IMAGE, 0 };
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
	if (!(m_dwFlags & TCE_DRAGDROP) || !m_bDragging)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// else
	return ((m_nDropTab >= 0) &&
			((m_nDropTab < m_nDragTab) || (m_nDropTab > (m_nDragTab + 1))));
}

void CTabCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	InvalidateTabs(nSBCode, nPos, pScrollBar);
	
	CXPTabCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTabCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	InvalidateTabs(nSBCode, nPos, pScrollBar);
	
	CXPTabCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTabCtrlEx::InvalidateTabs(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Prevent render artifacts on the tab that was beneath the spin control
	if (pScrollBar == (CWnd*)GetSpinButtonCtrl())
	{
		CRect rSpin, rTab;
		GetSpinButtonCtrlRect(rSpin);
		
		int iTab = GetItemCount();

		while (iTab--)
		{
			GetItemRect(iTab, rTab);

			if (CRect().IntersectRect(rSpin, rTab))
				InvalidateRect(rTab, FALSE);
		}
	}
}

int CTabCtrlEx::GetScrollPos() const
{
	const CSpinButtonCtrl* pSpin = GetSpinButtonCtrl();

	if (pSpin == NULL)
		return 0;

	// else
	return (int)LOWORD(pSpin->GetPos());
}

BOOL CTabCtrlEx::SetScrollPos(int nPos)
{
	if ((nPos < 0) || ((nPos > 0) && (nPos >= GetItemCount())))
	{
		ASSERT(0);
		return FALSE;
	}

	SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nPos), 0L);
	return TRUE;
}

void CTabCtrlEx::EnsureSelVisible()
{
	if (GetItemCount() == 0)
		return;

	CRect rSpin;
	
	if (!GetSpinButtonCtrlRect(rSpin))
	{
		SetScrollPos(0);
		return;
	}

	CRect rActiveTab;
	int nSelTab = GetCurSel();
	GetItemRect(nSelTab, rActiveTab);

	if ((rActiveTab.left >= 0) && (rActiveTab.right <= rSpin.left))
		return;

	int nScrollPos = GetScrollPos();

	if (rActiveTab.left < 0)
	{
		// Scroll left
		while (nScrollPos--)
		{
			SetScrollPos(nScrollPos);
			GetItemRect(nSelTab, rActiveTab);

			if (rActiveTab.left >= 0)
				break;
		}
	}
	else
	{
		// Scroll right
		for (int nPos = (nScrollPos + 1); nPos < GetItemCount(); nPos++)
		{
			SetScrollPos(nPos);
			GetItemRect(nSelTab, rActiveTab);

			if (rActiveTab.right <= rSpin.left)
				break;
		}
	}
}

void CTabCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CXPTabCtrl::OnSize(nType, cx, cy);

	EnsureSelVisible();
}

