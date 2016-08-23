// endatetimectrl.cpp : implementation file
//

#include "stdafx.h"
#include "endatetimectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_POSTCLOSEUP (WM_APP+1)

/////////////////////////////////////////////////////////////////////////////
// CEnDateTimeCtrl

CEnDateTimeCtrl::CEnDateTimeCtrl() : m_bWantReset(FALSE)
{
}

CEnDateTimeCtrl::~CEnDateTimeCtrl()
{
}

BEGIN_MESSAGE_MAP(CEnDateTimeCtrl, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CEnDateTimeCtrl)
	ON_NOTIFY_REFLECT(DTN_CLOSEUP, OnCloseup)
	ON_NOTIFY_REFLECT(DTN_DROPDOWN, OnDropdown)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT_EX(DTN_DATETIMECHANGE, OnDatetimechange)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_POSTCLOSEUP, OnPostCloseUp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnDateTimeCtrl message handlers

void CEnDateTimeCtrl::OnCloseup(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMonthCalCtrl* pMCC = GetMonthCalCtrl();

	if (pMCC)
	{
		pMCC->GetCurSel(&m_st);
		PostMessage(WM_POSTCLOSEUP);
	}
	
	*pResult = 0;
}

LRESULT CEnDateTimeCtrl::OnPostCloseUp(WPARAM wp, LPARAM lp)
{
	SetTime(&m_st);
//	SendMessage(DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&m_st);

	NMHDR nmh = { *this, GetDlgCtrlID(), DTN_DATETIMECHANGE };
	GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmh);

	return 0L;
}

void CEnDateTimeCtrl::OnDropdown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CEnDateTimeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL bReset = FALSE;
	m_bWantReset = FALSE; // reset

	if (!(GetStyle() & DTS_UPDOWN) && (GetStyle() & DTS_SHOWNONE)) // has the drop arrow
	{
		CRect rBtn;
		GetClientRect(rBtn);
		rBtn.left = rBtn.right - GetSystemMetrics(SM_CXVSCROLL);

		if (rBtn.PtInRect(point))
		{
			// check if the current checkbox is unchecked
			m_bWantReset = (GetTime(CTime()) == GDT_NONE);
		}
	}
	
	CDateTimeCtrl::OnLButtonDown(nFlags, point);
}

BOOL CEnDateTimeCtrl::OnDatetimechange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// if its changed as a result of the auto checking
	// that windows does when you click the down button
	// then we eat the notification
	if (m_bWantReset)
	{
		SetTime();
//		SendMessage(DTM_SETSYSTEMTIME, GDT_NONE, 0);
		m_bWantReset = FALSE; // clear
		return TRUE;
	}
/*
	else // we may need to set the check if its not done
	{
		// incomplete
		SYSTEMTIME st;
		CMonthCalCtrl* pMCC = GetMonthCalCtrl();

		pMCC->GetCurSel(&st);
		
		SendMessage(DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&st);
	}
*/
	
	*pResult = 0;
	return FALSE;
}
