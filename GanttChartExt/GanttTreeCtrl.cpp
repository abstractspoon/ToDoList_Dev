// GanttTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "GanttChartExt.h"
#include "GanttTreeCtrl.h"
#include "GanttMsg.h"

#include "..\shared\graphicsmisc.h"

//////////////////////////////////////////////////////////////////////

int TIPPADDING = 2;

//////////////////////////////////////////////////////////////////////
// CGanttTreeCtrl

IMPLEMENT_DYNAMIC(CGanttTreeCtrl, CTreeCtrl)

//////////////////////////////////////////////////////////////////////

CGanttTreeCtrl::CGanttTreeCtrl() 
	:	
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
	CTreeCtrl::PreSubclassWindow();

	InitTooltip();
}

int CGanttTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitTooltip();

	return 0;
}

int CGanttTreeCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	UINT nFlags = 0;
	HTREEITEM hti = HitTest(point, &nFlags);

	if (hti && (nFlags & TVHT_ONITEMLABEL) && (point.x <= m_nTitleColumnWidth))
	{
		CRect rLabel;
		VERIFY(GetItemRect(hti, rLabel, TRUE));

		if (rLabel.right > m_nTitleColumnWidth)
		{
			pTI->hwnd = GetSafeHwnd();
			pTI->uId = (DWORD)hti;
			pTI->uFlags |= TTF_TRANSPARENT;
			pTI->lpszText = _tcsdup(GetItemText(hti)); // MFC will free the duplicated string
			pTI->rect = rLabel;

			return (DWORD)hti;
		}
	}

	// else
	return CTreeCtrl::OnToolHitTest(point, pTI);
}

bool CGanttTreeCtrl::ProcessMessage(MSG* pMsg) 
{
	m_tooltip.FilterToolTipMessage(pMsg);

	return false;
}

void CGanttTreeCtrl::OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hti = (HTREEITEM)m_tooltip.GetToolInfo().uId;

	if (!hti)
	{
		ASSERT(0);
		return;
	}

	*pResult = TRUE; // we do the positioning

	CRect rLabel;
	VERIFY(GetItemRect(hti, rLabel, TRUE));

	ClientToScreen(rLabel);
	rLabel.InflateRect(0, 1, 0, 0);

	// Calculate exact width required
	CString sTip = GetItemText(hti);
	rLabel.right = (rLabel.left + GraphicsMisc::GetTextWidth(sTip, pNMHDR->hwndFrom));

	CRect rTip(rLabel);
	m_tooltip.AdjustRect(rTip, TRUE);
	rTip.OffsetRect(TIPPADDING, 0);

	rTip.top = rLabel.top;
	rTip.bottom = rLabel.bottom;

	m_tooltip.SetWindowPos(NULL, rTip.left, rTip.top, rTip.Width(), rTip.Height(), 
							(SWP_NOACTIVATE | SWP_NOZORDER));
}

LRESULT CGanttTreeCtrl::OnTitleColumnWidthChange(WPARAM wp, LPARAM lp)
{
	if ((HWND)lp == GetSafeHwnd())
	{
		m_nTitleColumnWidth = wp;
	}

	return 0L;
}

BOOL CGanttTreeCtrl::InitTooltip()
{
	if (!m_tooltip.GetSafeHwnd())
	{
		if (!m_tooltip.Create(this))
			return FALSE;

		m_tooltip.ModifyStyleEx(0, WS_EX_TRANSPARENT);
		m_tooltip.SetFont(CFont::FromHandle(GraphicsMisc::GetFont(*this)));
		m_tooltip.SetDelayTime(TTDT_INITIAL, 50);
		m_tooltip.SetDelayTime(TTDT_AUTOPOP, 10000);
	}

	return TRUE;
}
