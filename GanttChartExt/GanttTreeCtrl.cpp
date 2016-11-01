// GanttTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "GanttChartExt.h"
#include "GanttTreeCtrl.h"

//////////////////////////////////////////////////////////////////////
// CGanttTreeCtrl

IMPLEMENT_DYNAMIC(CGanttTreeCtrl, CTreeCtrl)

//////////////////////////////////////////////////////////////////////

CGanttTreeCtrl::CGanttTreeCtrl() :	m_htiTooltip(NULL)
{

}

CGanttTreeCtrl::~CGanttTreeCtrl()
{
}

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGanttTreeCtrl, CTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY(TTN_SHOW, 0, OnShowTooltip)
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
	m_htiTooltip = HitTest(point);

	if (m_htiTooltip)
	{
		pTI->hwnd = GetSafeHwnd();
		pTI->uId = (DWORD)m_htiTooltip;
		pTI->uFlags |= (TTF_ALWAYSTIP | TTF_TRANSPARENT);

		// MFC will free the duplicated string
		pTI->lpszText = _tcsdup(GetItemText(m_htiTooltip));

		VERIFY(GetItemRect(m_htiTooltip, &pTI->rect, TRUE));

		return (DWORD)m_htiTooltip;
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
		//rLabel.OffsetRect(PADDING, 0);

		::SendMessage(pNMHDR->hwndFrom, TTM_ADJUSTRECT, TRUE, (LPARAM)(LPRECT)rLabel);
		::SetWindowPos(pNMHDR->hwndFrom, 0, rLabel.left, rLabel.top, 0, 0,
			(SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER));

		m_htiTooltip = 0;
	}
}
