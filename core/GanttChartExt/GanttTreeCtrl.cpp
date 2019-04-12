// GanttTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
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
	m_nTitleColumnWidth(-1),
#pragma warning (disable: 4355)
	m_tch(*this)
#pragma warning (default: 4355)
{

}

CGanttTreeCtrl::~CGanttTreeCtrl()
{
}

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGanttTreeCtrl, CTreeCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY(TTN_SHOW, 0, OnShowTooltip)
	ON_REGISTERED_MESSAGE(WM_GTCN_TITLECOLUMNWIDTHCHANGE, OnTitleColumnWidthChange)
	ON_REGISTERED_MESSAGE(WM_TTC_TOOLHITTEST, OnToolHitTest)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

// CGanttTreeCtrl message handlers

void CGanttTreeCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();

	InitTooltip();
	m_fonts.Initialise(*this);
}

int CGanttTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitTooltip();
	m_fonts.Initialise(*this);

	return 0;
}

LRESULT CGanttTreeCtrl::OnToolHitTest(WPARAM wp, LPARAM lp)
{
	CPoint point(wp);
	TOOLINFO* pTI = (TOOLINFO*)lp;
	
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

bool CGanttTreeCtrl::ProcessMessage(MSG* /*pMsg*/) 
{
	return false;
}

void CGanttTreeCtrl::FilterToolTipMessage(MSG* pMsg) 
{
	m_tooltip.FilterToolTipMessage(pMsg);
}

void CGanttTreeCtrl::OnShowTooltip(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	HTREEITEM hti = (HTREEITEM)m_tooltip.GetLastHitToolInfo().uId;

	if (!hti)
	{
		ASSERT(0);
		return;
	}

	*pResult = TRUE; // we do the positioning

	// First set up the font
	BOOL bTopLevel = (GetParentItem(hti) == NULL);
	m_tooltip.SetFont(m_fonts.GetFont(bTopLevel ? GMFS_BOLD : 0));

	CRect rLabel;
	VERIFY(GetItemRect(hti, rLabel, TRUE));

	ClientToScreen(rLabel);
	rLabel.InflateRect(0, 1, 0, 0);

	// Calculate exact position required
	CRect rTip(rLabel);
	m_tooltip.AdjustRect(rTip, TRUE);
	rTip.OffsetRect(TIPPADDING, 0);

	rTip.top = rLabel.top;
	rTip.bottom = rLabel.bottom;

	m_tooltip.SetWindowPos(NULL, rTip.left, rTip.top, 0, 0, (SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE));
}

LRESULT CGanttTreeCtrl::OnTitleColumnWidthChange(WPARAM wp, LPARAM lp)
{
	if ((HWND)lp == GetSafeHwnd())
	{
		m_nTitleColumnWidth = wp;
	}

	return 0L;
}

LRESULT CGanttTreeCtrl::OnSetFont(WPARAM /*wp*/, LPARAM /*lp*/)
{
	m_fonts.Clear();

	return Default();
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

void CGanttTreeCtrl::ShowCheckboxes(BOOL bShow)
{
	if (bShow)
	{
		if (!m_ilCheckboxes.GetSafeHandle())
			VERIFY(GraphicsMisc::InitCheckboxImageList(*this, m_ilCheckboxes, IDB_CHECKBOXES, 255));

		SetImageList(&m_ilCheckboxes, TVSIL_STATE);
	}
	else
	{
		SetImageList(NULL, TVSIL_STATE);
	}
}

void CGanttTreeCtrl::ShowTaskIcons(BOOL bShow)
{
	if (bShow)
	{
		// Create dummy image to make a space for drawing in
		int nImageSize = GraphicsMisc::ScaleByDPIFactor(16);
		
		if (!m_ilTaskIcons.GetSafeHandle())
			VERIFY(m_ilTaskIcons.Create(nImageSize, nImageSize, 0, 1, 0));

		SetImageList(&m_ilTaskIcons, TVSIL_NORMAL);
	}
	else
	{
		SetImageList(NULL, TVSIL_NORMAL);
	}
}

HIMAGELIST CGanttTreeCtrl::GetTaskIcon(DWORD dwTaskID, int& iImageIndex) const
{
	if (TreeView_GetImageList(m_hWnd, TVSIL_NORMAL) == NULL)
		return NULL;

	HWND hwndParent = ::GetParent(GetSafeHwnd());
	return (HIMAGELIST)::SendMessage(hwndParent, WM_GTLC_GETTASKICON, dwTaskID, (LPARAM)&iImageIndex);
}

void CGanttTreeCtrl::OnDestroy()
{
	// Prevent icon requests during destruction
	ShowTaskIcons(FALSE);

	CTreeCtrl::OnDestroy();
}
