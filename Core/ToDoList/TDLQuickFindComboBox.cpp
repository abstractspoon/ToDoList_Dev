// TDLQuickFindComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLQuickFindComboBox.h"

#include "..\shared\EnString.h"

// CTDLQuickFindComboBox ///////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTDLQuickFindComboBox, CAutoComboBox)

CTDLQuickFindComboBox::CTDLQuickFindComboBox(DWORD dwFlags) : CAutoComboBox(dwFlags)
{
	m_iconPrev.Load(IDI_QUICKFIND_PREV);
	m_iconNext.Load(IDI_QUICKFIND_NEXT);

	m_edit.AddButton(TDLQFB_PREV, m_iconPrev, CEnString(IDS_QUICKFIND_PREVMATCH));
	m_edit.AddButton(TDLQFB_NEXT, m_iconNext, CEnString(IDS_QUICKFIND_NEXTMATCH));
}

CTDLQuickFindComboBox::~CTDLQuickFindComboBox()
{
}

BEGIN_MESSAGE_MAP(CTDLQuickFindComboBox, CAutoComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEEBtnClick)
END_MESSAGE_MAP()

// CTDLQuickFindComboBox message handlers //////////////////////////////

HBRUSH CTDLQuickFindComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CAutoComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	if ((nCtlColor == CTLCOLOR_EDIT) && (m_edit.GetSafeHwnd() == NULL))
	{
		VERIFY(m_edit.SubclassDlgItem(1001, this));
		ResizeEdit();

		// CEnEdit disables its tooltips when embedded in a combobox
		// simply because they don't seem to work
		//ASSERT(!m_edit.m_tooltip.GetSafeHwnd());

		// So we have to handle it ourselves
		EnableToolTips(TRUE);
	}

	return hbr;
}

void CTDLQuickFindComboBox::OnDestroy()
{
	if (m_edit.GetSafeHwnd())
		m_edit.UnsubclassWindow();

	CAutoComboBox::OnDestroy();
}

int CTDLQuickFindComboBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT(m_edit.GetSafeHwnd());

	ClientToScreen(&point);
	m_edit.ScreenToClient(&point);

	int nTool = m_edit.OnToolHitTest(point, pTI);

	if (nTool != -1)
	{
		pTI->hwnd = m_hWnd;

		m_edit.ClientToScreen(&pTI->rect);
		ScreenToClient(&pTI->rect);

		return nTool;
	}

	// else
	return CAutoComboBox::OnToolHitTest(point, pTI);
}

void CTDLQuickFindComboBox::OnSize(UINT nType, int cx, int cy)
{
	CAutoComboBox::OnSize(nType, cx, cy);

	ResizeEdit();
}

void CTDLQuickFindComboBox::ResizeEdit()
{
	// resize the edit to better fit the combo
	if (m_edit.GetSafeHwnd())
	{
		CRect rCombo;
		GetClientRect(rCombo);

		CRect rEdit(rCombo);
		rEdit.DeflateRect(1, 3, 2, 3);
		rEdit.right -= GetSystemMetrics(SM_CXVSCROLL);

		m_edit.MoveWindow(rEdit, FALSE);
	}
}

LRESULT CTDLQuickFindComboBox::OnEEBtnClick(WPARAM wp, LPARAM lp)
{
	// Forward to parent
	if (wp == 1001)
	{
		switch (lp)
		{
		case TDLQFB_PREV:
			return AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_EDIT_QUICKFINDPREV);

		case TDLQFB_NEXT:
			return AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_EDIT_QUICKFINDNEXT);
		}
	}

	ASSERT(0);
	return 0L;
}
