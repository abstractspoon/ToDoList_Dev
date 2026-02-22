// DateTimeCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "DateTimeCtrlEx.h"
#include "autoflag.h"
#include "misc.h"
#include "Graphicsmisc.h"
#include "themed.h"
#include "DateHelper.h"

#include "..\3rdParty\OSVersion.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef DTM_SETMCSTYLE
#	define DTM_SETMCSTYLE (DTM_FIRST + 11)
#	define DTM_GETMCSTYLE (DTM_FIRST + 12)
#endif

/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER <= 1200
	// Under VC6 this definition seems to be missing - causes link error
	IMPLEMENT_DYNAMIC(CDateTimeCtrl, CWnd)
#endif

IMPLEMENT_DYNAMIC(CDateTimeCtrlEx, CDateTimeCtrl)

/////////////////////////////////////////////////////////////////////////////
// CDateTimeCtrlEx

CDateTimeCtrlEx::CDateTimeCtrlEx(DWORD dwMonthCalStyle) 
	: 
	m_dwMonthCalStyle(dwMonthCalStyle), 
	m_bNotifyingParent(FALSE), 
	m_bEnableInlineEditing(TRUE),
	m_bShowCalendarOnCompleting(TRUE),
	m_bISOFormat(FALSE),
	m_bShowSeconds(TRUE) // default
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
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_MESSAGE(DTM_GETMONTHCAL, OnGetMonthCal)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
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
	ASSERT_VALID(this); 
	
	return (DWORD)SendMessage(DTM_SETMCSTYLE, 0, (LPARAM)dwStyle); 
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

void CDateTimeCtrlEx::PreSubclassWindow()
{
	if (m_dwMonthCalStyle)
	{
		SetMonthCalStyle(m_dwMonthCalStyle);
		m_dwMonthCalStyle = 0;
	}

	UpdateFormat();

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

		DTC_HITTEST nHit = HitTest(point);
		ASSERT(nHit != DTCHT_OUTSIDE);

		switch (nHit)
		{
		case DTCHT_DROPBUTTON:
			{
				m_bLButtonDown = TRUE;
				m_bWasSet = IsDateSet();
			}
			break;

		case DTCHT_CHECKBOX:
			if (m_bShowCalendarOnCompleting && !IsDateSet())
			{
				SendMessage(WM_SYSKEYDOWN, VK_DOWN);
				return; // eat it
			}
			break;
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

BOOL CDateTimeCtrlEx::OnCloseUp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Note: m_monthCal will clean itself up when HWND is destroyed
	*pResult = 0;

	// Figure out what caused the calendar to close
	BOOL bCancel = Misc::IsKeyPressed(VK_ESCAPE);
	BOOL bSetDate = Misc::IsKeyPressed(VK_RETURN);

	ASSERT(!(bCancel && bSetDate));

	m_bLButtonDown = m_bDropped = FALSE;

	if (!bCancel && !bSetDate)
	{
		// If neither key was pressed, check to see if the user
		// clicked somewhere within the date control and decide what to do
		if (Misc::IsKeyPressed(VK_LBUTTON))
		{
			switch (HitTest(GetMessagePos(), TRUE))
			{
			case DTCHT_CHECKBOX:
				{
					// The checkbox must have been ticked for the month
					// calendar to be visible so we toggle the checkbox off
					m_nmdtcLast.nmhdr = *pNMHDR;
					m_nmdtcLast.nmhdr.code = DTN_DATETIMECHANGE;
					m_nmdtcLast.dwFlags = GDT_NONE;

					bSetDate = TRUE;
				}
				break;

			case DTCHT_DATETIME:
				{
					// the user clicked in the date part of the 
					// control so we accept the date
					bSetDate = TRUE;
				}
				break;

			case DTCHT_DROPBUTTON:
				bCancel = TRUE;
				break;
			}
		}

		// else try to figure out from the state of the date
		if (!bSetDate && !bCancel)
		{
			COleDateTime date(m_nmdtcLast.st);
		
			bSetDate = ((date.m_dt != 0.0) && (date.GetStatus() == COleDateTime::valid));
			bCancel = !bSetDate;
		}
	}
	
	if (bCancel)
	{
		if (!m_bWasSet)
		{
			// just clear the checkbox state if originally unset
			SendMessage(DTM_SETSYSTEMTIME, GDT_NONE);
		}
		else if (m_nmdtcLast.st.wYear != 0)
		{
			// Restore original date
			SetTime(&m_nmdtcLast.st);
		}
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
		else // fall back on the first notification
		{
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

	// We only need hook ISO calendars to fix the week numbers
	if ((m_monthCal.GetSafeHwnd() == NULL) && CDateHelper::WantISOWeekOfYear())
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

LRESULT CDateTimeCtrlEx::OnSetFont(WPARAM wp, LPARAM lp)
{
	if (COSVersion() == OSV_LINUX)
		SetMonthCalFont((HFONT)wp);

	return Default();
}

BOOL CDateTimeCtrlEx::GetPickerInfo(DATETIMEPICKERINFO& dtpi) const
{
	if (GetSafeHwnd() && (COSVersion() >= OSV_VISTA))
	{
		dtpi.cbSize = sizeof(dtpi);
		return ::SendMessage(m_hWnd, DTM_GETDATETIMEPICKERINFO, 0, (LPARAM)&dtpi);
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

DTC_HITTEST CDateTimeCtrlEx::HitTest(CPoint point, BOOL bScreen) const
{
	if (bScreen)
		ScreenToClient(&point);

	CRect rClient;
	GetClientRect(rClient);

	if (!rClient.PtInRect(point))
		return DTCHT_OUTSIDE;

	if (GetCheckboxRect().PtInRect(point))
		return DTCHT_CHECKBOX;

	if (GetDropButtonRect().PtInRect(point))
		return DTCHT_DROPBUTTON;

	// all else
	return DTCHT_DATETIME;
}

void CDateTimeCtrlEx::OnPaint()
{
	BOOL bCheckboxFocused = IsCheckboxFocused();
	BOOL bWantDPIScaling = GraphicsMisc::WantDPIScaling();

	if (bCheckboxFocused || bWantDPIScaling)
	{
		CPaintDC dc(this);
		
		CRect rCheck;
		DATETIMEPICKERINFO dtpi = { 0 };

		if (GetPickerInfo(dtpi))
		{
			rCheck = dtpi.rcCheck;
		}
		else
		{
			GetClientRect(rCheck);

			rCheck.right = rCheck.Height();
			rCheck.DeflateRect(2, 2);
		}

		// Fill the checkbox background depending on state
		if (bCheckboxFocused)
		{
			dc.FillSolidRect(rCheck, GetSysColor(COLOR_HIGHLIGHT));
		}
		else if (bWantDPIScaling)
		{
			BOOL bEnabled = IsWindowEnabled();

			if (bEnabled)
			{
				SYSTEMTIME st;
				bEnabled = (GDT_VALID == SendMessage(DTM_GETSYSTEMTIME, 0, (LPARAM)&st));
			}

			dc.FillSolidRect(rCheck, GetSysColor(bEnabled ? COLOR_WINDOW : COLOR_3DFACE));
		}

		// Always draw the checkbox because Windows gets
		// the scaling wrong for high DPI
		UINT nState = DFCS_BUTTONCHECK;
		
		if (IsDateSet())
			nState |= DFCS_CHECKED;

		if (!IsWindowEnabled())
			nState |= DFCS_INACTIVE;
		
		CThemed::DrawFrameControl(this, &dc, rCheck, DFC_BUTTON, nState);

		// Clip out our drawing
		dc.ExcludeClipRect(rCheck);
		
		// default drawing
		CDateTimeCtrl::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
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

void CDateTimeCtrlEx::ShowSeconds(BOOL bShow)
{
	if (Misc::StatesDiffer(m_bShowSeconds, bShow))
	{
		m_bShowSeconds = bShow;

		if (GetSafeHwnd())
		{
			ASSERT(GetStyle() & DTS_TIMEFORMAT);
			UpdateFormat();
		}
	}	
}

void CDateTimeCtrlEx::SetISOFormat(BOOL bEnable)
{
	if (Misc::StatesDiffer(m_bISOFormat, bEnable))
	{
		m_bISOFormat = bEnable;

		if (GetSafeHwnd())
			UpdateFormat();
	}
}

void CDateTimeCtrlEx::UpdateFormat()
{
	ASSERT(GetSafeHwnd());

	if (GetStyle() & DTS_TIMEFORMAT)
		SetFormat(Misc::GetTimeFormat(m_bShowSeconds, m_bISOFormat));
	else
		SetFormat(Misc::GetShortDateFormat(FALSE, m_bISOFormat));
}
