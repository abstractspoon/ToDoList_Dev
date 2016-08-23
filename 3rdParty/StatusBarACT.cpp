////////////////////////////////////////////////////////////////////////////
// File:	StatusBarACT.cpp
// Version:	3.1
// Created:	24 Jun 2004
//
// Author:	Paul S. Vickery
// E-mail:	paul@vickeryhome.freeserve.co.uk
//
// CStatusBar derived control to add auto-fit, tooltips and command handling
//
// You are free to use or modify this code, with no restrictions, other than
// you continue to acknowledge me as the original author in this source code,
// or any code derived from it.
//
// If you use this code, or use it as a base for your own code, it would be 
// nice to hear from you simply so I know it's not been a waste of time!
//
// Copyright (c) 2003-2004 Paul S. Vickery
//
////////////////////////////////////////////////////////////////////////////
// Version History:
//
// Version 3.1 - 24 Jun 2004
// =========================
// - Updated to support Unicode builds
// - fixed bugs:
//   - using ON_COMMAND macros causes assertion failure
//   - updating tip text re-adds the tool, and causes old one to overlap
//   - crash in GetPaneCursorIndex() if no pane cursors set and also in
//     GetPaneFlagsIndex() if no pane flags set (reported by Tom Mason)
//
// Version 3.0 - 28 May 2003
// =========================
// - Can now specify a custom cursor for a pane
// - Added ability to specify a hand cursor to use instead of the default
//   (the default is only available on Windows 98/ME and Windows 2000/XP)
//
// Version 2.0 - 15 Apr 2003
// =========================
// - Extended control to allow pane tool-tips to be specified as part of the 
//   pane's text, separated by a new line ('\n') character.
// - Added ability to show multi-line tool-tips by including carriage returns 
//   ('\r') and/or line breaks ('\n') in the tip text.
// 
// Version 1.0 - 18 Feb 2003
// =========================
// Initial version
// 
////////////////////////////////////////////////////////////////////////////
// PLEASE LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatusBarACT.h"

#include "..\shared\GraphicsMisc.h"
#include "..\shared\colordef.h"
#include "..\shared\enstring.h"
#include "..\shared\themed.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusBarACT

IMPLEMENT_DYNAMIC(CStatusBarACT, CStatusBar)

CStatusBarACT::CStatusBarACT()
{
	m_hCursorHand = NULL;
	m_bTryDefaultHandCursor = FALSE;
	m_crFrom = m_crTo = m_crText = CLR_NONE;
}

CStatusBarACT::~CStatusBarACT()
{
}

BEGIN_MESSAGE_MAP(CStatusBarACT, CStatusBar)
//{{AFX_MSG_MAP(CStatusBarACT)
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_SIZE()
//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusBarACT message handlers

int CStatusBarACT::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create tooltip control
	m_tooltip.Create(this);
	SetBarStyle(GetBarStyle() & ~CBRS_TOOLTIPS);
	
	// allow '\n' chars to wrap tips
	m_tooltip.SendMessage(TTM_SETMAXTIPWIDTH, 0, (UINT)(WORD)-1);
	
	return 0;
}

void CStatusBarACT::OnPaint()
{
	if ((m_crText == -1 && m_crFrom == -1 && m_crTo == -1) || !CThemed::IsAppThemed())
	{
		Default();
	}
	else
	{
		CPaintDC dc(this);
		CRect rClient;
		CFont* pOldFont = dc.SelectObject(GetFont());

		GetClientRect(rClient);
		DrawRectBkgnd(&dc, rClient);

		int nPane = GetStatusBarCtrl().GetParts(0, NULL);
		dc.SetTextColor(m_crText);
		dc.SetBkMode(TRANSPARENT);
			
		while (nPane--)
		{
			CRect rect;
			GetItemRect(nPane, &rect);

			const CString& sText = GetPaneText(nPane);

			if (sText.GetLength())
			{
				dc.TextOut(rect.left+2, rect.top, sText);
				dc.ExcludeClipRect(rect); // so adjacent item cannot overwrite
			}

			// draw divider if the pane has any text or it's not autofit
			BOOL bAutoFit = (GetPaneFlagsIndex(nPane) & SBACTF_AUTOFIT);

			if (nPane && (!sText.IsEmpty() || !bAutoFit)) // ignore first
			{
				rect.left -= 2;
				rect.right = rect.left + 1;
				rect.bottom -= 2;

				// pick color
				COLORREF color = m_crFrom;

				int nLum = RGBX(color).Luminance();
				color = (nLum < 128) ? GraphicsMisc::Lighter(m_crTo, 0.25) : GraphicsMisc::Darker(m_crTo, 0.4);
				dc.FillSolidRect(rect, color);
			}
		}

		dc.SelectObject(pOldFont);
	}
}

BOOL CStatusBarACT::OnEraseBkgnd(CDC* pDC)
{
	if ((m_crText == -1 && m_crFrom == -1 && m_crTo == -1) || !CThemed::IsAppThemed())
		return CStatusBar::OnEraseBkgnd(pDC);

	// else
	return TRUE;
}

void CStatusBarACT::DrawRectBkgnd(CDC* pDC, const CRect& rect)
{
	if (m_crTo == m_crFrom)
	{
		pDC->FillSolidRect(rect, m_crFrom);
	}
	else
	{
		GM_GRADIENT nType = GraphicsMisc::GetGradientType(m_bGlass, m_bGradient);

		GraphicsMisc::DrawGradient(nType, pDC, rect, m_crFrom, m_crTo, FALSE);
	}
}

void CStatusBarACT::SetUIColors(COLORREF crBackFrom, COLORREF crBackTo, COLORREF crText, BOOL bGradient, BOOL bGlass)
{
	m_crText = crText;
	m_crFrom = crBackFrom;
	
	if (crBackTo == CLR_NONE)
		m_crTo = m_crFrom;
	else
		m_crTo = crBackTo;
	
	m_bGradient = bGradient;
	m_bGlass = bGlass;
	
	if (GetSafeHwnd())
		Invalidate(TRUE);
}


BOOL CStatusBarACT::PreTranslateMessage(MSG* pMsg)
{
	m_tooltip.RelayEvent(pMsg);
	return CStatusBar::PreTranslateMessage(pMsg);
}

// returns index under point, where point is client co-ords
// returns -1 if no pane is under the point
int CStatusBarACT::HitTest(CPoint point)
{
	CStatusBarCtrl& bar = GetStatusBarCtrl();
	int nMax = bar.GetParts(0, NULL);
	for (int nIndex = 0; nIndex < nMax; nIndex++)
	{
		CRect rc;
		GetItemRect(nIndex, &rc);
		if (rc.PtInRect(point))
			return nIndex;
	}
	return -1;
}

// see what pane we're on, and send a WM_COMMAND message to the 
// parent window, with the pane id as the low word of wParam, 
// and the message causing the command as the lParam
void CStatusBarACT::SendPaneCommand(CPoint point, UINT message)
{
	CWnd* pParent = GetParent();
	if (pParent != NULL)
	{
		int nIndex = HitTest(point);
		if (nIndex != -1)
		{
			UINT nID = GetItemID(nIndex);
			BOOL bDblClk = message == WM_LBUTTONDBLCLK || message == WM_RBUTTONDBLCLK || message == WM_MBUTTONDBLCLK;
			BOOL bLeft = message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK;
			BOOL bRight = message == WM_RBUTTONDOWN || message == WM_RBUTTONDBLCLK;
			BOOL bMiddle = message == WM_MBUTTONDOWN || message == WM_MBUTTONDBLCLK;
			DWORD dwFlags = 0;
			if (m_adwFlags.GetSize() > nIndex && m_adwFlags[nIndex] & SBACTF_COMMAND)
			{
				dwFlags = m_adwFlags[nIndex];
				if (((bDblClk && (dwFlags & SBACTF_DOUBLECLICK)) || 
					(! bDblClk && (dwFlags & SBACTF_SINGLECLICK))))
				{
					BOOL bDoCommand = FALSE;
					if (((bLeft && dwFlags & SBACTF_LEFTBUTTON) || 
						(bRight && dwFlags & SBACTF_RIGHTBUTTON) || 
						(bMiddle && dwFlags & SBACTF_MIDDLEBUTTON)))
						bDoCommand = TRUE;
					if ((bLeft && !(dwFlags & SBACTF_LEFTBUTTON)) || 
						(bRight && !(dwFlags & SBACTF_RIGHTBUTTON)) || 
						(bMiddle && !(dwFlags & SBACTF_MIDDLEBUTTON)))
						bDoCommand = FALSE;
					if (bDoCommand)
						pParent->SendMessage(WM_COMMAND, MAKEWPARAM(nID, message), 0L/*(LPARAM)GetSafeHwnd()*/);
				}
			}
		}
	}
}

#ifndef IDC_HAND
#define IDC_HAND            MAKEINTRESOURCE(32649)
#endif	// IDC_HAND

// if we're over a pane that has a custom cursor specified, then show it 
// else if pane has the SBACTF_HANDCURSOR style then change it to a hand
BOOL CStatusBarACT::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		int nIndex = HitTest(pt);
		if (nIndex != -1)
		{
			HCURSOR hCursor = GetPaneCursorIndex(nIndex);
			if (hCursor == NULL)
			{
				DWORD dwFlags = GetPaneFlagsIndex(nIndex);
				if (dwFlags & SBACTF_HANDCURSOR)
				{
					if (m_bTryDefaultHandCursor)
						hCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);
					if (! m_bTryDefaultHandCursor || hCursor == NULL)
						hCursor = m_hCursorHand;
				}
			}
			if (hCursor != NULL)
			{
				SetCursor(hCursor);
				return TRUE;
			}
		}
	}
	return CStatusBar::OnSetCursor(pWnd, nHitTest, message);
}

// sets the flags for a pane by the pane's index
BOOL CStatusBarACT::SetPaneFlagsIndex(int nIndex, DWORD dwFlags/*=SBACTF_NORMAL*/)
{
	// make sure array is big enough
	int nMax = GetStatusBarCtrl().GetParts(0, NULL);
	
	if (nIndex >= nMax || nIndex < 0)
		return FALSE;
	
	if ((dwFlags & SBACTF_CLICKFLAGMASK) == 0)
		dwFlags |= SBACTF_DOUBLECLICK;	// default to just double-click
	
	if ((dwFlags & SBACTF_BUTTONFLAGMASK) == 0)
		dwFlags |= SBACTF_LEFTBUTTON;	// default to just left button
	
	if (dwFlags & SBACTF_STRETCHY)
	{
		dwFlags &= ~SBACTF_AUTOFIT;
		SetPaneStyle(nIndex, GetPaneStyle(nIndex) | SBPS_STRETCH);
	}
	
	m_adwFlags.SetAtGrow(nIndex, dwFlags);
	
	return TRUE;
}

// sets the flags for a pane by the pane's ID
BOOL CStatusBarACT::SetPaneFlags(UINT nID, DWORD dwFlags/*=SBACTF_NORMAL*/)
{
	return SetPaneFlagsIndex(CommandToIndex(nID), dwFlags);
}

// gets the flags for a pane by the pane's index
DWORD CStatusBarACT::GetPaneFlagsIndex(int nIndex)
{
	// make sure array is big enough
	int nMax = GetStatusBarCtrl().GetParts(0, NULL);
	if (nIndex >= nMax || nIndex < 0 || m_adwFlags.GetSize() <= nIndex)
		return 0;
	DWORD dwFlags = m_adwFlags[nIndex];
	return dwFlags;
}

// gets the flags for a pane by the pane's ID
DWORD CStatusBarACT::GetPaneFlags(UINT nID)
{
	return GetPaneFlagsIndex(CommandToIndex(nID));
}

// sets a pane's tool-tip text, by the pane's ID
BOOL CStatusBarACT::SetPaneTooltip(UINT nID, LPCTSTR lpszText/*=NULL*/)
{
	return SetPaneTooltipIndex(CommandToIndex(nID), lpszText);
}

// sets a pane's tool-tip text, by the pane's index
BOOL CStatusBarACT::SetPaneTooltipIndex(int nIndex, LPCTSTR lpszText/*=NULL*/)
{
	int nMax = GetStatusBarCtrl().GetParts(0, NULL);
	if (nIndex >= nMax || nIndex < 0)
		return FALSE;
	CToolInfo ti;
	if (lpszText == NULL || *lpszText == _T('\0'))
	{
		m_tooltip.DelTool(this, nIndex + 1);
	}
	else if (! m_tooltip.GetToolInfo(ti, this, nIndex + 1))
	{
		// get bounding rect of pane, and add tool
		// add one to index, as tooltips don't allow ids of 0
		CRect rc;
		GetItemRect(nIndex, &rc);
		m_tooltip.AddTool(this, lpszText, rc, nIndex + 1);
	}
	else
		m_tooltip.UpdateTipText(lpszText, this, nIndex + 1);

	// turn off tip tracking
	if (m_tooltip.GetToolInfo(ti, this, nIndex + 1))
	{
		ti.uFlags &= ~TTF_TRACK;
		_tcsncpy(ti.szText, lpszText, 255);
		m_tooltip.SetToolInfo(&ti);
	}

	return TRUE;
}

// sets a pane's tool-tip text to a resource ID, by the pane's ID
BOOL CStatusBarACT::SetPaneTooltip(UINT nID, UINT nTipID)
{
	return SetPaneTooltipIndex(CommandToIndex(nID), nTipID);
}

// sets a pane's tool-tip text to a resource ID, by the pane's index
BOOL CStatusBarACT::SetPaneTooltipIndex(int nIndex, UINT nTipID)
{
	return SetPaneTooltipIndex(nIndex, CEnString(nTipID));
}

// need to tell the tooltips when the 'tools' have moved
// for each pane, get new rect, and pass to the tooltip control
void CStatusBarACT::UpdateTooltipRects()
{
	int nMax = GetStatusBarCtrl().GetParts(0, NULL);
	for (int n = 0; n < nMax; n++)
	{
		CRect rc;
		GetItemRect(n, &rc);
		m_tooltip.SetToolRect(this, n + 1, &rc);
	}
}

// need to tell the tooltips that the 'tools' have moved
void CStatusBarACT::OnSize(UINT nType, int cx, int cy) 
{
	CStatusBar::OnSize(nType, cx, cy);
	
	if (::IsWindow(m_hWnd))
		UpdateTooltipRects();
}

// re-size the specified pane to exactly fit its text
void CStatusBarACT::AutoFitPane(int nIndex)
{
	if (nIndex < 0 || nIndex >= GetStatusBarCtrl().GetParts(0, NULL))
		return;
	// get dc, and get text width on bar, then re-size pane to fit
	try{
		CClientDC dc(this);
		CRect rc(0, 0, 0, 0);
		CFont* pFont = GetFont(); // get the status bar font, so it gets the size correct
		CFont* pFontOld = NULL;
		if (pFont == NULL)
			pFontOld = (CFont*)dc.SelectStockObject(ANSI_VAR_FONT);
		else
			pFontOld = dc.SelectObject(pFont);
		CEnString sText = GetPaneText(nIndex);
		int nPos = sText.Find(_T('\n'));
		if (nPos != -1)
			sText = sText.Left(nPos);
		dc.DrawText(sText, &rc, DT_CALCRECT | DT_SINGLELINE | DT_NOPREFIX);
		dc.SelectObject(pFontOld);
		int nWidth;
		UINT nID, nStyle;
		GetPaneInfo(nIndex, nID, nStyle, nWidth);
		nWidth = rc.Width();
		SetPaneInfo(nIndex, nID, nStyle, nWidth);
		
		// need to tell the tooltips that the 'tools' have moved
		UpdateTooltipRects();
		
  }catch(...){ ; }
}

LRESULT CStatusBarACT::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case SB_SETTEXT:
		{
			CString sText((LPCTSTR)lParam);
			CString sTip;
			BOOL bHasTip = FALSE;
			int nPos = sText.Find(_T('\n'));
			if (nPos != -1)
			{
				bHasTip = TRUE;
				sTip = sText.Mid(nPos+1);
				sText = sText.Left(nPos);
				lParam = (LPARAM)(LPCTSTR)sText;
			}
			LRESULT lResult = CStatusBar::DefWindowProc(message, wParam, lParam);
			int nIndex = wParam & 0xff;
			
			if (m_adwFlags.GetSize() > nIndex && m_adwFlags[nIndex] & SBACTF_AUTOFIT)
				AutoFitPane(nIndex);
			
			if (bHasTip)
				SetPaneTooltipIndex(nIndex, sTip);
			
			return lResult;
		}
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		{
			CPoint pt(lParam);
			SendPaneCommand(pt, message);
		}
		break;
	};
	return CStatusBar::DefWindowProc(message, wParam, lParam);
}

// called to add panes to the status bar
BOOL CStatusBarACT::SetPanes(LPSBACTPANEINFO lpsbactpi, UINT nPaneInfoCount)
{
	if (lpsbactpi == NULL || nPaneInfoCount <= 0)
		return FALSE;
	// first set the standard info (IDs)
	UINT* anIDs = new UINT[nPaneInfoCount];
	UINT n;
	
	for (n = 0; n < nPaneInfoCount; n++)
		anIDs[n] = lpsbactpi[n].nID;
	BOOL bResult = SetIndicators(anIDs, nPaneInfoCount);
	delete [] anIDs;
	// now set our info
	for (n = 0; n < nPaneInfoCount; n++)
	{
		const SBACTPANEINFO& sbactpi = lpsbactpi[n];
		
		if (sbactpi.dwFlags & SBACTF_RESOURCETIP)
			SetPaneTooltip(sbactpi.nID, (UINT)sbactpi.lpszTip);
		else
			SetPaneTooltip(sbactpi.nID, sbactpi.lpszTip);
		
		SetPaneFlags(sbactpi.nID, sbactpi.dwFlags);
		SetPaneCursor(sbactpi.nID, sbactpi.lpszCursor);
	}
	
	return bResult;
}

// called to modify a pane
BOOL CStatusBarACT::SetPane(LPSBACTPANEINFO lpsbactpi)
{
	if (lpsbactpi == NULL)
		return FALSE;
	int nIndex = CommandToIndex(lpsbactpi->nID);
	int nMax = GetStatusBarCtrl().GetParts(0, NULL);
	if (nIndex < nMax)
		return FALSE;
	SetPaneTooltipIndex(nIndex, lpsbactpi->lpszTip);
	SetPaneFlagsIndex(nIndex, lpsbactpi->dwFlags);
	SetPaneCursorIndex(nIndex, lpsbactpi->lpszCursor);
	return TRUE;
}

// sets the cursor for a pane by the pane's index
BOOL CStatusBarACT::SetPaneCursorIndex(int nIndex, HCURSOR hCursor)
{
	// make sure array is big enough
	int nMax = GetStatusBarCtrl().GetParts(0, NULL);
	if (nIndex >= nMax || nIndex < 0)
		return FALSE;
	m_adwCursors.SetAtGrow(nIndex, hCursor);
	return TRUE;
}

// sets the cursor for a pane by the pane's ID
BOOL CStatusBarACT::SetPaneCursor(UINT nID, HCURSOR hCursor)
{
	return SetPaneCursorIndex(CommandToIndex(nID), hCursor);
}

// sets the cursor for a pane by the pane's index
BOOL CStatusBarACT::SetPaneCursorIndex(int nIndex, LPCTSTR lpszCursor)
{
	// make sure array is big enough
	int nMax = GetStatusBarCtrl().GetParts(0, NULL);
	if (nIndex >= nMax || nIndex < 0)
		return FALSE;
	// get cursor handle and add to array of handles
	HCURSOR hCursor = NULL;
	if (lpszCursor != NULL)
	{
		CWinApp* pApp = AfxGetApp();
		ASSERT(pApp != NULL);
		if (pApp == NULL)
			return FALSE;
		hCursor = pApp->LoadCursor(lpszCursor);
	}
	SetPaneCursorIndex(nIndex, hCursor);
	return TRUE;
}

// sets the cursor for a pane by the pane's ID
BOOL CStatusBarACT::SetPaneCursor(UINT nID, LPCTSTR lpszCursor)
{
	return SetPaneCursorIndex(CommandToIndex(nID), lpszCursor);
}

// sets the cursor for a pane by the pane's index
BOOL CStatusBarACT::SetPaneCursorIndex(int nIndex, UINT nCursorID)
{
	return SetPaneCursorIndex(nIndex, MAKEINTRESOURCE(nCursorID));
}

// sets the cursor for a pane by the pane's ID
BOOL CStatusBarACT::SetPaneCursor(UINT nID, UINT nCursorID)
{
	return SetPaneCursorIndex(CommandToIndex(nID), MAKEINTRESOURCE(nCursorID));
}

HCURSOR CStatusBarACT::GetPaneCursorIndex(int nIndex)
{
	// make sure array is big enough
	int nMax = GetStatusBarCtrl().GetParts(0, NULL);
	if (nIndex >= nMax || nIndex < 0 || m_adwCursors.GetSize() <= nIndex)
		return 0;
	return m_adwCursors[nIndex];
}

HCURSOR CStatusBarACT::GetPaneCursor(UINT nID)
{
	return GetPaneCursorIndex(CommandToIndex(nID));
}

BOOL CStatusBarACT::SetHandCursor(UINT nCursorID, BOOL bTryDefault/*=TRUE*/)
{
	return SetHandCursor(MAKEINTRESOURCE(nCursorID), bTryDefault);
}

BOOL CStatusBarACT::SetHandCursor(LPCTSTR lpszCursorID, BOOL bTryDefault/*=TRUE*/)
{
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	if (pApp == NULL)
		return FALSE;
	SetHandCursor(pApp->LoadCursor(lpszCursorID), bTryDefault);
	return (m_hCursorHand != NULL);
}

void CStatusBarACT::SetHandCursor(HCURSOR hCursor, BOOL bTryDefault/*=TRUE*/)
{
	m_hCursorHand = hCursor;
	m_bTryDefaultHandCursor = bTryDefault;
}

