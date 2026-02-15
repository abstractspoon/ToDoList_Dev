// TDLQuickFindComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLQuickFindComboBox.h"

#include "..\shared\EnString.h"

// CTDLQuickFindComboBox ///////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTDLQuickFindComboBox, CAutoComboBox)

CTDLQuickFindComboBox::CTDLQuickFindComboBox(DWORD dwFlags) 
	: 
	CAutoComboBox(&m_quickEdit, dwFlags)
{
	m_iconPrev.Load(IDI_QUICKFIND_PREV);
	m_iconNext.Load(IDI_QUICKFIND_NEXT);

	m_quickEdit.AddButton(TDLQFB_PREV, m_iconPrev, CEnString(IDS_QUICKFIND_PREVMATCH));
	m_quickEdit.AddButton(TDLQFB_NEXT, m_iconNext, CEnString(IDS_QUICKFIND_NEXTMATCH));
}

CTDLQuickFindComboBox::~CTDLQuickFindComboBox()
{
}

BEGIN_MESSAGE_MAP(CTDLQuickFindComboBox, CAutoComboBox)
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEEBtnClick)
END_MESSAGE_MAP()

// CTDLQuickFindComboBox message handlers //////////////////////////////

int CTDLQuickFindComboBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT(m_quickEdit.GetSafeHwnd());

	::MapWindowPoints(m_hWnd, m_quickEdit, &point, 1);

	int nTool = m_quickEdit.OnToolHitTest(point, pTI);

	if (nTool != -1)
	{
		pTI->hwnd = m_hWnd;
		pTI->uFlags |= TTF_ALWAYSTIP;

		::MapWindowPoints(m_quickEdit, m_hWnd, (LPPOINT)&pTI->rect, 2);

		return nTool;
	}

	// else
	return CAutoComboBox::OnToolHitTest(point, pTI);
}

void CTDLQuickFindComboBox::OnSubclassChild(HWND hwndChild)
{
	CAutoComboBox::OnSubclassChild(hwndChild);

	if (m_quickEdit.GetSafeHwnd() == hwndChild)
	{
		// CEnEdit disables its tooltips when embedded in a combobox
		// simply because they don't seem to work
		// So we have to handle it ourselves
		EnableToolTips(TRUE);
	}
}

LRESULT CTDLQuickFindComboBox::OnEEBtnClick(WPARAM wp, LPARAM lp)
{
	// Forward to app
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
