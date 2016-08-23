// CFPSMiniCalendarListCtrl
// Author:	Matt Gullett
//			gullettm@yahoo.com
// Copyright (c) 2001
//
// This is a companion control for use by the CFPSMiniCalendarCtrl.
// It is probably useless for any other purpose.
//
// You may freely use this source code in personal, freeware, shareware
// or commercial applications provided that 1) my name is recognized in the
// code and if this code represents a substantial portion of the application
// that my name be included in the credits for the application (about box, etc)
//
// Use this code at your own risk.  This code is provided AS-IS.  No warranty
// is granted as to the correctness, usefulness or value of this code.
//
// Special thanks to Keith Rule for the CMemDC class
// http://www.codeproject.com/gdi/flickerfree.asp
//
// *******************************************************************
// TECHNICAL NOTES:
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "FPSMiniCalendarListCtrl.h"

#include "..\Shared\DateHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "math.h"
#include "memdc.h"

/////////////////////////////////////////////////////////////////////////////
// CFPSMiniCalendarListCtrl

CFPSMiniCalendarListCtrl::CFPSMiniCalendarListCtrl()
{
	SetCalendar(NULL);
	SetItemsPerPage(7);
	SetMiddleMonthYear(COleDateTime::GetCurrentTime().GetMonth(), COleDateTime::GetCurrentTime().GetYear());
	SetBackColor((COLORREF)::GetSysColor(COLOR_WINDOW));
	SetTextColor((COLORREF)::GetSysColor(COLOR_BTNTEXT));

	SetBackColor(RGB(255,255,255));
	SetTextColor(RGB(0,0,0));

	m_iSelMonth = COleDateTime::GetCurrentTime().GetMonth();
	m_iSelYear = COleDateTime::GetCurrentTime().GetYear();

	m_iUpFactor = 0;
	m_iDownFactor = 0;
}

CFPSMiniCalendarListCtrl::~CFPSMiniCalendarListCtrl()
{
}


BEGIN_MESSAGE_MAP(CFPSMiniCalendarListCtrl, CWnd)
	//{{AFX_MSG_MAP(CFPSMiniCalendarListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// utility function for converting long to CString 
CString CFPSMiniCalendarListCtrl::CStr(long lValue)
{
	char szValue[20];

	_ltoa(lValue, szValue, 10);

	return szValue;
}

/////////////////////////////////////////////////////////////////////////////
// CFPSMiniCalendarListCtrl message handlers

BOOL CFPSMiniCalendarListCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return FALSE;
}

void CFPSMiniCalendarListCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC memdc(&dc);
	CRect rectClient;

	// calendar must be set for this to work correctly
	ASSERT(m_pwndCalendar);

	// if needed, create font
	if (!m_FontInfo.m_pFont)
		m_FontInfo.CreateFont(&memdc);

	// determine client area
	GetClientRect(rectClient);

	// first, determine half of the items on the page
	// we use this to reverse calculate the top item
	int iHalfCount = m_iItemsPerPage / 2;
	if ((iHalfCount * 2) > m_iItemsPerPage)
		iHalfCount--;

	int iMonth = m_iMiddleMonth;
	int iYear = m_iMiddleYear;
	int iItem;

	for (iItem = 0; iItem < iHalfCount; iItem++)
	{
		iMonth--;
		if (iMonth < 1)
		{
			iMonth = 12;
			iYear--;
		}
	}	
	
	// prepare DC
	CFont* pOldFont = memdc.SelectObject(m_FontInfo.m_pFont);
	//CFont* pOldFont = memdc.SelectObject(m_pwndCalendar->GetDaysFont().m_pFont);
	int iPosY = 0;
	int iPosX = 10;
	
	// fill background
	memdc.FillSolidRect(0, 0, rectClient.Width(), rectClient.Height(), m_cBackColor);
	memdc.SetBkMode(TRANSPARENT);

	for (iItem = 0; iItem < m_iItemsPerPage; iItem++)
	{
		CString strDisplayText;
		CRect rectItem;

		if (m_pwndCalendar)
			strDisplayText = CDateHelper::GetMonthName(iMonth);

		strDisplayText += " ";
		strDisplayText += CStr(iYear);

		CSize szItem = memdc.GetTextExtent("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

		rectItem.SetRect(iPosX, iPosY, iPosX + szItem.cx, iPosY + szItem.cy);

		if (IsSelected(iItem, rectItem))
		{
			// this could be done better, but it is a very simple control so
			// I am just going to set the selected month/year right here in
			// the draw code
			m_iSelMonth = iMonth;
			m_iSelYear = iYear;

			memdc.SetTextColor(m_cBackColor);
			memdc.SetBkColor(m_cTextColor);

			memdc.FillSolidRect(0, iPosY, rectClient.Width(), szItem.cy, memdc.GetBkColor());
		}
		else
		{
			memdc.SetTextColor(m_cTextColor);
			memdc.SetBkColor(m_cBackColor);
		}

		memdc.DrawText(strDisplayText, rectItem, DT_SINGLELINE | DT_LEFT | DT_TOP);
				
		iMonth++;
		if (iMonth > 12)
		{
			iMonth = 1;
			iYear++;
		}

		iPosY += szItem.cy;
	}

	memdc.SelectObject(pOldFont);
}

void CFPSMiniCalendarListCtrl::ForwardMessage(MSG *pMSG)
{
	if (pMSG->message == WM_MOUSEMOVE || pMSG->message == WM_TIMER)
	{
		CRect rectClient;

		// determine cursor position
		DWORD dwPos = ::GetMessagePos();
		POINTS ptsPos = MAKEPOINTS(dwPos);
		CPoint ptPos;
		ptPos.x = ptsPos.x;
		ptPos.y = ptsPos.y;

		//ScreenToClient(&ptPos);

		GetClientRect(rectClient);
		ClientToScreen(rectClient);

		if (ptPos.y < rectClient.top)
		{
			int iDiff = rectClient.top - ptPos.y;

			if (iDiff <= 15)
				ScrollUp(1);
			else
				ScrollUp(2);
		}
		else if (ptPos.y > rectClient.bottom)
		{
			int iDiff = ptPos.y - rectClient.bottom;

			if (iDiff <= 15)
				ScrollDown(1);
			else
				ScrollDown(2);
		}
		else
		{
			m_iUpFactor = 0;
			m_iDownFactor = 0;
		}

		RedrawWindow();
	}
}

void CFPSMiniCalendarListCtrl::SetMiddleMonthYear(int iMonth, int iYear)
{
	ASSERT(iYear >= 100);
	ASSERT(iYear <= 9999);
	ASSERT(iMonth >= 1);
	ASSERT(iMonth <= 12);

	if (iMonth >= 1 && iMonth <= 12)
		m_iMiddleMonth = iMonth;

	if (iYear >= 100 && iYear <= 9999)
		m_iMiddleYear = iYear;
}

void CFPSMiniCalendarListCtrl::SetItemsPerPage(int iValue)
{
	// we require at least 3 items per page and the count
	// must be odd (same number of items on either side
	// of middle)
	ASSERT(iValue >= 3);
	ASSERT(iValue % 2);

	if (iValue >= 3 && (iValue % 2))
		m_iItemsPerPage = iValue;
}

void CFPSMiniCalendarListCtrl::SetFontInfo(const CFPSMiniCalendarCtrlFontInfo& font)
{
	// make sure font object gets recreated
	if (m_FontInfo.m_pFont)
		delete m_FontInfo.m_pFont;

	// copy font attributes
	m_FontInfo = font;

	// except font itself
	m_FontInfo.m_pFont = NULL;
}

void CFPSMiniCalendarListCtrl::SetCalendar(CFPSMiniCalendarCtrl* pWnd) 
{
	ASSERT(!pWnd || ::IsWindow(pWnd->GetSafeHwnd()));

	if (!pWnd || ::IsWindow(pWnd->GetSafeHwnd()))
		m_pwndCalendar = pWnd;
}

BOOL CFPSMiniCalendarListCtrl::IsSelected(int iX, CRect rectItem)
{
	BOOL bReturn = FALSE;

	CRect rectClient;

	// determine cursor position
	DWORD dwPos = ::GetMessagePos();
	POINTS ptsPos = MAKEPOINTS(dwPos);
	CPoint ptPos;
	ptPos.x = ptsPos.x;
	ptPos.y = ptsPos.y;

	ScreenToClient(&ptPos);

	GetClientRect(rectClient);

	if (ptPos.y < 0 && iX == 0)
		bReturn = TRUE;
	else if (ptPos.y > rectClient.Height() && iX == (m_iItemsPerPage-1))
		bReturn = TRUE;
	else if (ptPos.y >= rectItem.top && ptPos.y <= rectItem.bottom-1)
		bReturn = TRUE;

	return bReturn;
}

void CFPSMiniCalendarListCtrl::AutoConfigure()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if (::IsWindow(GetSafeHwnd()))
	{
		// determine cursor position
		CSize szMaxItem(0,0);
		DWORD dwPos = ::GetMessagePos();
		POINTS ptsPos = MAKEPOINTS(dwPos);
		CPoint ptPos;
		ptPos.x = ptsPos.x;
		ptPos.y = ptsPos.y;

		CClientDC dc(this);

		// calendar must be set for this to work correctly
		ASSERT(m_pwndCalendar);

		// if needed, create font
		if (!m_FontInfo.m_pFont)
			m_FontInfo.CreateFont(&dc);

		CFont* pOldFont = dc.SelectObject(m_FontInfo.m_pFont);
		//CFont* pOldFont = dc.SelectObject(m_pwndCalendar->GetDaysFont().m_pFont);

		szMaxItem.cy = dc.GetTextExtent("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789").cy;

		for (int iMonth = 1; iMonth <= 12; iMonth++)
		{
			CString strText;

			if (m_pwndCalendar)
				strText = CDateHelper::GetMonthName(iMonth);

			strText += " 0000";

			int iWidth = dc.GetTextExtent(strText).cx;

			if (iWidth > szMaxItem.cx)
				szMaxItem.cx = iWidth;
		}

		dc.SelectObject(pOldFont);

		// calculate rectangle
		CSize szCalendar((szMaxItem.cx + 20), ((szMaxItem.cy * m_iItemsPerPage) + 3));

		int iLeftX = ptPos.x - (szCalendar.cx / 2);
		int iTopY = ptPos.y - (szCalendar.cy / 2);
		int iRightX = ptPos.x + (szCalendar.cx / 2);
		int iBottomY = ptPos.y + (szCalendar.cy / 2);

		SetWindowPos(NULL, iLeftX, iTopY, (iRightX-iLeftX), (iBottomY-iTopY), 0);
		RedrawWindow();
	}
}

void CFPSMiniCalendarListCtrl::ScrollUp(int iCount)
{
	m_iDownFactor = 0;
	
	m_iUpFactor += iCount;

	if (m_iUpFactor >= 10)
	{
		for (int iX = 0; iX < iCount; iX++)
		{
			if (m_iMiddleYear == 100)
				break;

			m_iMiddleMonth--;

			if (m_iMiddleMonth < 1)
			{
				m_iMiddleMonth = 12;
				m_iMiddleYear--;
			}
		}
		
		m_iUpFactor = 0;
	}
}

void CFPSMiniCalendarListCtrl::ScrollDown(int iCount)
{
	m_iUpFactor = 0;

	m_iDownFactor += iCount;

	if (m_iDownFactor >= 10)
	{
		for (int iX = 0; iX < iCount; iX++)
		{
			if (m_iMiddleYear == 9999)
				break;

			m_iMiddleMonth++;

			if (m_iMiddleMonth > 12)
			{
				m_iMiddleMonth = 1;
				m_iMiddleYear++;
			}
		}

		m_iDownFactor = 0;
	}
}

