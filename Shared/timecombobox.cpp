// timecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "timecombobox.h"
#include "timehelper.h"
#include "misc.h"
#include "localizer.h"
#include "holdredraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeComboBox

CTimeComboBox::CTimeComboBox(DWORD dwStyle) : m_dwStyle(dwStyle), m_hwndListBox(NULL)
{
}

CTimeComboBox::~CTimeComboBox()
{
}


BEGIN_MESSAGE_MAP(CTimeComboBox, CComboBox)
	//{{AFX_MSG_MAP(CTimeComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeComboBox message handlers

void CTimeComboBox::PreSubclassWindow() 
{
	VERIFY(Initialize());

	CComboBox::PreSubclassWindow();
}

int CTimeComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	VERIFY(Initialize());
	
	return 0;
}

BOOL CTimeComboBox::Initialize()
{
	BuildCombo();
	
	// hook the edit ctrl so we can convert '.' and ',' to ':'
	CWnd* pEdit = GetDlgItem(1001);

	if (pEdit)
		return m_scEdit.HookWindow(*pEdit, this);

	return TRUE;
}

void CTimeComboBox::BuildCombo(BOOL bReset)
{
	CLocalizer::EnableTranslation(*this, FALSE);
	
	ASSERT(bReset || GetCount() == 0);
	
	if (!bReset && GetCount())
		return;
	
	ResetContent();
	
	for (int nHour = 0; nHour < 24; nHour++)
	{
		CString sTime;
		
		if (!(m_dwStyle & TCB_NOTIME) || (nHour >  0))
			sTime = CTimeHelper::FormatClockTime(nHour, 0, 0, FALSE, (m_dwStyle & TCB_ISO));
		
		// add empty string representing 'no time'
		AddString(sTime);
		
		if (m_dwStyle & TCB_HALFHOURS)
		{
			sTime = CTimeHelper::FormatClockTime(nHour, 30, 0, FALSE, (m_dwStyle & TCB_ISO));
			AddString(sTime);
		}
	}
}

double CTimeComboBox::GetOleTime() const
{
	return (Get24HourTime() / 24.0);
}

BOOL CTimeComboBox::SetOleTime(double dTime)
{
	// truncate to extract the time only component if it has one
	dTime -= (int)dTime;

	if (dTime <= 0)
	{
		if (m_dwStyle & TCB_NOTIME)
			SetCurSel(0);
		else
			SetCurSel(CB_ERR);

		return TRUE;
	}
	
	// else
	return Set24HourTime(dTime * 24.0);
}

void CTimeComboBox::SetStyle(DWORD dwStyle)
{
	BOOL bWasISO = (m_dwStyle & TCB_ISO);
	BOOL bIsISO = (dwStyle & TCB_ISO);

	m_dwStyle = dwStyle;

	if (bWasISO != bIsISO)
	{
		double date = GetOleTime();
		BuildCombo(TRUE);
		SetOleTime(date);
	}
}

double CTimeComboBox::Get24HourTime() const
{
	// is this a hack? I'm not sure
	// but we look at the current Windows message being handled and if it's a
	// combo sel change then we use the current selection else we use the
	// window text (it may be an edit change notification)
	const MSG* pMsg = CWnd::GetCurrentMessage();

	if ((pMsg->message == WM_COMMAND) && 
		(pMsg->lParam == (LPARAM)GetSafeHwnd()) &&
		(HIWORD(pMsg->wParam) == CBN_SELCHANGE))
	{
		// since the items in the combo are ordered from 1am to 11pm
		// we can use the selection index as a direct link to the hour
		int nSel = GetCurSel();

		if (nSel <= 0) // 'no time'
			return 0;

		// else
		if (m_dwStyle & TCB_HALFHOURS)
			return (nSel * 0.5);
		else
			return nSel;
	}

	// else use window text
	CString sTime;
	GetWindowText(sTime);

	if (Misc::Trim(sTime).IsEmpty() && !(m_dwStyle & TCB_NOTIME))
		return -1; // error

	// else
	return CTimeHelper::DecodeClockTime(sTime);
}

BOOL CTimeComboBox::Set24HourTime(double dTime)
{
	if (dTime < 0)
	{
		SetCurSel(0);
		return TRUE;
	}
	
	// else
	if (dTime >= 24.0)
		return FALSE;

	// add on half a minute to handle floating point inaccuracies
	dTime += (0.5 / (24 * 60));

	int nHour = (int)dTime;
	int nMin = (int)((dTime - nHour) * 60); // nearest 1 minute

	CString sTime = CTimeHelper::FormatClockTime(nHour, nMin, 0, FALSE, (m_dwStyle & TCB_ISO));

	if (SelectString(-1, sTime) == CB_ERR)
		SetWindowText(sTime);

	return TRUE;
}

LRESULT CTimeComboBox::ScWindowProc(HWND hRealWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CHAR:
		// Convert comma and period to time separator
		if ((wParam == ',') || (wParam == '.'))
		{
			CString sSep = Misc::GetTimeSeparator();

			if (!sSep.IsEmpty())
				return CSubclasser::ScWindowProc(m_scEdit, message, sSep[0], lParam);
		}
		break;
	}

	// else
	return CSubclasser::ScDefault(m_scEdit);
}

void CTimeComboBox::OnCaptureChanged(CWnd* pWnd)
{
	// Receiving this message tells us that
	// the listbox is now fully visible
	CComboBox::OnCaptureChanged(pWnd);

	ScrollListBox();
}

HBRUSH CTimeComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_LISTBOX)
		m_hwndListBox = *pWnd;

	return CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTimeComboBox::ScrollListBox()
{
	if (!GetDroppedState())
	{
		ASSERT(m_hwndListBox == NULL);
		return;
	}

	ASSERT(m_hwndListBox);

	// Scroll to beginning of working day unless the
	// current time is earlier than that
	const int STARTOFWORKDAY = 7;

	double dCurTime = Get24HourTime();

	if ((dCurTime < 0) || (dCurTime > STARTOFWORKDAY))
	{
		int nCurScrollPos = ::GetScrollPos(m_hwndListBox, SB_VERT);
		int nMinScrollPos = STARTOFWORKDAY; 

		if (m_dwStyle & TCB_HALFHOURS)
			nMinScrollPos *= 2;

		if (nMinScrollPos > nCurScrollPos)
		{
			CHoldRedraw hr(m_hwndListBox);
			::SendMessage(m_hwndListBox, WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nMinScrollPos), 0);
		}
	}
	
	m_hwndListBox = NULL; // always
}
