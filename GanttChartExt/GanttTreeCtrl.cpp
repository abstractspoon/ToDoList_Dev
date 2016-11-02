// GanttTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "GanttChartExt.h"
#include "GanttTreeCtrl.h"
#include "GanttMsg.h"

#include "..\shared\graphicsmisc.h"


//////////////////////////////////////////////////////////////////////
// CGanttTreeCtrl

IMPLEMENT_DYNAMIC(CGanttTreeCtrl, CTreeCtrl)

//////////////////////////////////////////////////////////////////////

CGanttTreeCtrl::CGanttTreeCtrl() 
	:	
	m_htiTooltip(NULL),
	m_nTitleColumnWidth(-1)
{

}

CGanttTreeCtrl::~CGanttTreeCtrl()
{
}

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGanttTreeCtrl, CTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY(TTN_SHOW, 0, OnShowTooltip)
	ON_REGISTERED_MESSAGE(WM_GTCN_TITLECOLUMNWIDTHCHANGE, OnTitleColumnWidthChange)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

// CGanttTreeCtrl message handlers

void CGanttTreeCtrl::PreSubclassWindow()
{
	EnableToolTips(TRUE);

	CTreeCtrl::PreSubclassWindow();
}

int CGanttTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	EnableToolTips(TRUE);

	return 0;
}

INT_PTR CGanttTreeCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	UINT nFlags = 0;
	m_htiTooltip = HitTest(point, &nFlags);

	if (m_htiTooltip && (nFlags & TVHT_ONITEMLABEL) && (point.x <= m_nTitleColumnWidth))
	{
		CRect rLabel;
		VERIFY(GetItemRect(m_htiTooltip, rLabel, TRUE));

		int nAvailWidth = (m_nTitleColumnWidth - rLabel.left);

		CString sTip = GetItemText(m_htiTooltip);

		if (GraphicsMisc::GetTextWidth(sTip, GetSafeHwnd()) > nAvailWidth)
		{
			InitTooltipFont();

			pTI->cbSize = sizeof(TOOLINFO);
			pTI->hwnd = GetSafeHwnd();
			pTI->uId = (DWORD)m_htiTooltip;
			pTI->uFlags |= (TTF_ALWAYSTIP | TTF_TRANSPARENT);
			pTI->lpszText = _tcsdup(sTip); // MFC will free the duplicated string
			pTI->rect = rLabel;

			return (DWORD)m_htiTooltip;
		}

		// else
		m_htiTooltip = NULL;
	}

	return CTreeCtrl::OnToolHitTest(point, pTI);
}

bool CGanttTreeCtrl::ProcessMessage(MSG* pMsg) 
{
	FilterToolTipMessage(pMsg);

	return false;
}

void CGanttTreeCtrl::OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_htiTooltip)
	{
		*pResult = TRUE; // we do the positioning

		CRect rLabel;
		VERIFY(GetItemRect(m_htiTooltip, rLabel, TRUE));

		ClientToScreen(rLabel);
		rLabel.InflateRect(0, 1, 0, 0);

		// Calculate exact width required
		CString sTip = GetItemText(m_htiTooltip);
		rLabel.right = (rLabel.left + GraphicsMisc::GetTextWidth(sTip, pNMHDR->hwndFrom));

		CRect rTip(rLabel);
		::SendMessage(pNMHDR->hwndFrom, TTM_ADJUSTRECT, TRUE, (LPARAM)(LPRECT)(LPCRECT)rTip);

		rTip.top = rLabel.top;
		rTip.bottom = rLabel.bottom;
		
		::SetWindowPos(pNMHDR->hwndFrom, 0, 
						rTip.left, rTip.top, rTip.Width(), rTip.Height(), 
						(SWP_NOACTIVATE | SWP_NOZORDER));

		m_htiTooltip = 0;
	}
}

LRESULT CGanttTreeCtrl::OnTitleColumnWidthChange(WPARAM wp, LPARAM lp)
{
	if ((HWND)lp == GetSafeHwnd())
	{
		m_nTitleColumnWidth = wp;
	}

	return 0L;
}

void CGanttTreeCtrl::InitTooltipFont() const
{
	// Nasty hack
#if _MSC_VER >= 1400
	CToolTipCtrl* pTT = AfxGetModuleThreadState()->m_pToolTip;
#else
	CToolTipCtrl* pTT = AfxGetThreadState()->m_pToolTip;
#endif

	if (pTT && pTT->GetSafeHwnd())
	{
		HFONT hFont = GraphicsMisc::GetFont(*this);
		pTT->SendMessage(WM_SETFONT, (WPARAM)hFont);
	}
}
