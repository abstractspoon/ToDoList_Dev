// DateTimeCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "DateTimeCtrlEx.h"
#include "OSVersion.h"
#include "autoflag.h"
#include "misc.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef DTM_GETDATETIMEPICKERINFO
#	define DTM_GETDATETIMEPICKERINFO (DTM_FIRST + 14)

struct DATETIMEPICKERINFO
{
    DWORD cbSize;
	
    RECT rcCheck;
    DWORD stateCheck;
	
    RECT rcButton;
    DWORD stateButton;
	
    HWND hwndEdit;
    HWND hwndUD;
    HWND hwndDropDown;
};

#endif

#ifndef DTM_SETMCSTYLE
#	define DTM_SETMCSTYLE (DTM_FIRST + 11)
#	define DTM_GETMCSTYLE (DTM_FIRST + 12)
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateTimeCtrlEx

CDateTimeCtrlEx::CDateTimeCtrlEx(DWORD dwMonthCalStyle) 
	: 
	m_dwMonthCalStyle(dwMonthCalStyle), 
	m_bNotifyingParent(FALSE), 
	m_bEnableInlineEditing(TRUE),
	m_bShowCalendarOnCompleting(TRUE)
{
	// clear state
	ResetCalendarHandling();
}

CDateTimeCtrlEx::~CDateTimeCtrlEx()
{
}

void CDateTimeCtrlEx::ResetCalendarHandling()
{
	m_bLButtonDown = m_bDropped = m_bWasSet = FALSE;
	
	ZeroMemory(&m_nmdtcFirst, sizeof(m_nmdtcFirst));
	ZeroMemory(&m_nmdtcLast, sizeof(m_nmdtcLast));
}

BEGIN_MESSAGE_MAP(CDateTimeCtrlEx, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CDateTimeCtrlEx)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT_EX(DTN_DATETIMECHANGE, OnDateTimeChange)
	ON_NOTIFY_REFLECT_EX(DTN_CLOSEUP, OnCloseUp)
	ON_NOTIFY_REFLECT_EX(DTN_DROPDOWN, OnDropDown)
	ON_WM_SYSKEYDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_MESSAGE(DTM_GETMONTHCAL, OnGetMonthCal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateTimeCtrlEx message handlers

DWORD CDateTimeCtrlEx::SetMonthCalStyle(DWORD dwStyle)
{ 
	if (!GetSafeHwnd())
	{
		m_dwMonthCalStyle = dwStyle;
		return 0;
	}

	// else
	ASSERT(::IsWindow(m_hWnd)); 
	
	return (DWORD)::SendMessage(m_hWnd, DTM_SETMCSTYLE, 0, (LPARAM)dwStyle); 
}

DWORD CDateTimeCtrlEx::GetMonthCalStyle() const
{ 
	if (!GetSafeHwnd())
		return m_dwMonthCalStyle;

	ASSERT(::IsWindow(m_hWnd)); 
	
	return (DWORD)::SendMessage(m_hWnd, DTM_GETMCSTYLE, 0, 0);
}

BOOL CDateTimeCtrlEx::IsCalendarVisible() const 
{ 
	if (m_bLButtonDown || m_bDropped)
	{
		ASSERT(GetMonthCalCtrl());
		return TRUE;
	}

	return FALSE;
}

int CDateTimeCtrlEx::OnCreate(LPCREATESTRUCT pCreate)
{
// 	if (m_dwMonthCalStyle)
// 	{
// 		SetMonthCalStyle(m_dwMonthCalStyle);
// 		m_dwMonthCalStyle = 0;
// 	}

	return CDateTimeCtrl::OnCreate(pCreate);
}

void CDateTimeCtrlEx::PreSubclassWindow()
{
	if (m_dwMonthCalStyle)
	{
		SetMonthCalStyle(m_dwMonthCalStyle);
		m_dwMonthCalStyle = 0;
	}

	CDateTimeCtrl::PreSubclassWindow();
}

void CDateTimeCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// the sequence of logic once the dropdown button
	// has been clicked appears to be this:

	// 1. This message
	// 2. If the state was GDT_NONE then the date is 
	//    set to the current date and a DTN_DATECHANGE
	//    is sent to the parent and reflected here first
	// 3. If we have DTS_SHOWNONE then we will get no more 
	//    mouse messages until the MonthCal control closes 
	//	  which makes interpreting the user's intentions tricky
	// 4. If the user clicks outside the MonthCal then
	//    we receive DTN_CLOSEUP but no DTN_DATECHANGE
	// 5. If the user presses <escape> then we receive
	//    DTN_DATECHANGE followed by DTN_CLOSEUP
	// 6. If the user clicks on a day on the MonthCal
	//    we also get DTN_DATECHANGE followed by DTN_CLOSEUP
	// 7. If the user navigates using the cursor keys
	//    then we get multiple DTN_DATECHANGE
	// 8. If the user presses <enter> we get DTN_CLOSEUP
	//    without a DTN_DATECHANGE

	// clear state
	if (!m_bLButtonDown)
	{
		ResetCalendarHandling();

		if (GetDropButtonRect().PtInRect(point))
		{
			m_bLButtonDown = TRUE;

			// was the date set before ?
			COleDateTime date;
			m_bWasSet = (GetTime(date) && (date.GetStatus() == COleDateTime::valid));
		}
		else if (m_bShowCalendarOnCompleting && GetCheckboxRect().PtInRect(point))
		{
			if (!IsDateSet())
			{
				SendMessage(WM_SYSKEYDOWN, VK_DOWN);
				return; // eat it
			}
		}
	}

	CDateTimeCtrl::OnLButtonDown(nFlags, point);
}

BOOL CDateTimeCtrlEx::OnDateTimeChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (!m_bNotifyingParent)
	{
		*pResult = 0;
		NMDATETIMECHANGE* pNMDTC = (NMDATETIMECHANGE*)pNMHDR;

		// if we get a date change between clicking the
		// button and receiving DTN_DROPDOWN then we must
		// have been unset
		if (m_bLButtonDown)
		{
			// capture the first change in case we need it later
			m_nmdtcFirst = (*pNMDTC);
			return TRUE; 
		}
		else if (m_bDropped)
		{
			// capture the last change for sending to our 
			// parent at the end and then eat it
			m_nmdtcLast = (*pNMDTC);
			return TRUE; 
		}
	}
	
	return FALSE; // route to parent
}

BOOL CDateTimeCtrlEx::OnCloseUp(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	*pResult = 0;

	// Note: m_monthCal will clean itself up when HWND is destroyed

	// if DTS_SHOWNONE is NOT set then we can receive this before the
	// the left mouse button is released if the user re-clicks the
	// drop button to hide the calendar
#ifdef _DEBUG
	if (!(GetStyle() & DTS_SHOWNONE))
	{
		CPoint pt(GetMessagePos());
		ScreenToClient(&pt);

		if (GetDropButtonRect().PtInRect(pt))
		{
			ASSERT(m_bLButtonDown);
			ASSERT(!m_bDropped);
		}
		else
		{
			ASSERT(!m_bLButtonDown);
			ASSERT(m_bDropped);
		}
	}
	else
	{
		ASSERT(!m_bLButtonDown);
		ASSERT(m_bDropped);
	}
#endif

	// see if we can figure out what key was pressed
	BOOL bCancel = (GetKeyState(VK_ESCAPE) & 0x8000);
	BOOL bSetDate = (GetKeyState(VK_RETURN) & 0x8000);

	ASSERT(!(bCancel && bSetDate));

	m_bLButtonDown = m_bDropped = FALSE;

	// if neither is pressed then try figure out whether 
	// the date is set
	if (!bCancel && !bSetDate)
	{
		COleDateTime date(m_nmdtcLast.st);
		
		bSetDate = ((date.m_dt != 0.0) && (date.GetStatus() == COleDateTime::valid));
		bCancel = !bSetDate;
	}
	
	if (bCancel)
	{
		// just set the checkbox state if originally unset
		if (!m_bWasSet)
			SendMessage(DTM_SETSYSTEMTIME, GDT_NONE);
	}
	else if (bSetDate)
	{
		// notify our parent of the last change
		// because we've been eating them all

		// special case: User pressed <return> with no
		// other action in which case we never received
		// a DTN_DATECHANGE and we have to fix it up here
		CAutoFlag af(m_bNotifyingParent, TRUE);

		if (m_nmdtcLast.nmhdr.hwndFrom != NULL)
		{
			GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)(&m_nmdtcLast));
		}
		else
		{
			// fall back on the first notification
			ASSERT(m_nmdtcFirst.nmhdr.hwndFrom != NULL);
			GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)(&m_nmdtcFirst));
		}
	}

	// else clear state and continue routing to parent
	ResetCalendarHandling();
	return FALSE;
}

BOOL CDateTimeCtrlEx::OnDropDown(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	*pResult = 0;

	if (m_monthCal.GetSafeHwnd() == NULL)
	{
		HWND hMonthCal = (HWND)SendMessage(DTM_GETMONTHCAL);
		ASSERT(hMonthCal);

		VERIFY(m_monthCal.SubclassWindow(hMonthCal));
	}

	if (m_bLButtonDown)
	{
		m_bDropped = TRUE;
		m_bLButtonDown = FALSE;
	}

	return FALSE; // route to parent
}

LRESULT CDateTimeCtrlEx::OnGetMonthCal(WPARAM wp, LPARAM lp)
{
	return Default();
}

BOOL CDateTimeCtrlEx::GetPickerInfo(DATETIMEPICKERINFO& dtpi) const
{
	if (COSVersion() >= OSV_VISTA)
	{
		CDateTimeCtrlEx* pThis = const_cast<CDateTimeCtrlEx*>(this);
		dtpi.cbSize = sizeof(dtpi);
		
		return pThis->SendMessage(DTM_GETDATETIMEPICKERINFO, 0, (LPARAM)&dtpi);
	}

	return FALSE;
}

void CDateTimeCtrlEx::OnSetFocus(CWnd* pOldWnd)
{
	DATETIMEPICKERINFO dtpi = { 0 };
	
	if (GetPickerInfo(dtpi))
		InvalidateRect(&dtpi.rcCheck);

	CDateTimeCtrl::OnSetFocus(pOldWnd);
}

void CDateTimeCtrlEx::OnKillFocus(CWnd* pNewWnd)
{
	DATETIMEPICKERINFO dtpi = { 0 };
	
	if (GetPickerInfo(dtpi))
		InvalidateRect(&dtpi.rcCheck);

	CDateTimeCtrl::OnKillFocus(pNewWnd);
}

CRect CDateTimeCtrlEx::GetDropButtonRect() const
{
	DATETIMEPICKERINFO dtpi = { 0 };

	if (GetPickerInfo(dtpi))
		return dtpi.rcButton;

	// else fallback for versions <= XP
	CRect rButton;
	GetClientRect(rButton);

	rButton.left = (rButton.right - GetSystemMetrics(SM_CXVSCROLL));

	return rButton;
}

CRect CDateTimeCtrlEx::GetCheckboxRect() const
{
	if (!(GetStyle() & DTS_SHOWNONE))
		return CRect(0, 0, 0, 0);

	DATETIMEPICKERINFO dtpi = { 0 };

	if (GetPickerInfo(dtpi))
		return dtpi.rcCheck;

	// else fallback for versions <= XP
	CRect rButton;
	GetClientRect(rButton);

	rButton.right = (rButton.left + GetSystemMetrics(SM_CXVSCROLL));

	return rButton;
}

void CDateTimeCtrlEx::OnPaint()
{
	if (IsCheckboxFocused())
	{
		CPaintDC dc(this);
		
		// default drawing
		CDateTimeCtrl::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
		
		// selection rect
		DATETIMEPICKERINFO dtpi = { 0 };
		VERIFY (GetPickerInfo(dtpi));
		
		CRect rClip(dtpi.rcCheck);
		rClip.DeflateRect(2, 2);
		dc.ExcludeClipRect(rClip);
		
		dc.FillSolidRect(&dtpi.rcCheck, GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		Default();
	}
}

BOOL CDateTimeCtrlEx::IsCheckboxFocused() const
{
	DATETIMEPICKERINFO dtpi = { 0 };
	
	if (GetPickerInfo(dtpi))
	{
		if (dtpi.stateCheck & STATE_SYSTEM_FOCUSED)
			return TRUE;
		
		// else fallback
		return (::GetFocus() == *this);
	}
	
	// else XP and below
	return FALSE;
}

BOOL CDateTimeCtrlEx::IsDateSet() const
{
	COleDateTime date;
	return (GetTime(date) && (date.GetStatus() == COleDateTime::valid));
}

void CDateTimeCtrlEx::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// this handles opening the calendar with 'ALT + Down'
	m_bLButtonDown = FALSE;
	ZeroMemory(&m_nmdtcLast, sizeof(m_nmdtcLast));
	
	if (nChar == VK_DOWN)
	{
		m_bLButtonDown = TRUE;
		m_bWasSet = IsDateSet();
	}
	
	CDateTimeCtrl::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CDateTimeCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bEnableInlineEditing && !IsCalendarVisible())
	{
		switch (nChar)
		{
		case VK_UP:		// increment value
		case VK_DOWN:   // decrement value
		case VK_HOME:	// select minimum value
		case VK_END:	// select maximum value
			return;
		}
	}
	else if (m_bShowCalendarOnCompleting)
	{
		if ((nChar == VK_SPACE) && !IsDateSet())
		{
			SendMessage(WM_SYSKEYDOWN, VK_DOWN);
			return; // eat it
		}
	}
	
	CDateTimeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDateTimeCtrlEx::ShowSeconds(BOOL bShow)
{
	if (!(GetStyle() & DTS_TIMEFORMAT))
	{
		ASSERT(0);
		return FALSE;
	}

	CString sFormat = Misc::GetTimeFormat(bShow);
	return SetFormat(sFormat);
}
