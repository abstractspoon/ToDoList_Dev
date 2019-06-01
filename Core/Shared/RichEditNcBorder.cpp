// RichEditNcBorder.cpp: implementation of the CRichEditNcBorder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RichEditNcBorder.h"
#include "themed.h"
#include "wclassdefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef WM_THEMECHANGED 
#	define WM_THEMECHANGED 0x031A
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRichEditNcBorder::CRichEditNcBorder() : m_bThemedBorder(FALSE)
{

}

CRichEditNcBorder::~CRichEditNcBorder()
{

}

BOOL CRichEditNcBorder::Initialize(HWND hwndRichEdit)
{
	if (hwndRichEdit) // hook
	{
		if (IsHooked())
			return TRUE;

		// else
		if (HookWindow(hwndRichEdit))
		{
			VerifyThemedBorderState();
			return TRUE;
		}
	
		// else
		return FALSE;
	}

	// else unhook
	if (IsHooked())
		return HookWindow(NULL);

	// else
	return TRUE;
}

LRESULT CRichEditNcBorder::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_NCPAINT:
		NcDrawBorder();
		return 0L;

	case WM_THEMECHANGED:
		VerifyThemedBorderState();
		break;

	case WM_STYLECHANGED:
		VerifyThemedBorderState();
		break;

	case WM_NCCALCSIZE:
		// If wParam is FALSE, we don't need to make any calculation
		if (m_bThemedBorder && wp)
		{
			NCCALCSIZE_PARAMS *pNCP = (NCCALCSIZE_PARAMS*)lp;

			// Ask the control to first calculate the space it needs
			CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);
			
			// Alter the size for our own border
			::InflateRect(&(pNCP->rgrc[0]), -1, -1);

			return WVR_REDRAW; // and redraw
		}
		break;

	case WM_ENABLE:
		if (m_bThemedBorder)
			RedrawWindow(hRealWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_UPDATENOW | RDW_FRAME);
	}

	return CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);
}

void CRichEditNcBorder::NcDrawBorder()
{
	Default();

	if (m_bThemedBorder)
	{
		CThemed th(GetCWnd(), WC_EDIT);
		CWindowDC dc(GetCWnd());

		CRect rBorder, rClient;
		GetWindowRect(rBorder);

		th.GetThemeBackgroundContentRect(&dc, EP_EDITTEXT, ETS_NORMAL, rBorder, rClient);
		
		// convert to window coordinates
		rClient.OffsetRect(-rBorder.left, -rBorder.top);
		rBorder.OffsetRect(-rBorder.left, -rBorder.top);

		dc.ExcludeClipRect(rClient);

		// determine the current border state
		int nState;

		if (!IsWindowEnabled())
			nState = ETS_DISABLED;

		else if (HasStyle(ES_READONLY) || SendMessage(EM_GETOPTIONS, NULL, NULL) & ECO_READONLY)
			nState = ETS_READONLY;

		else
			nState = ETS_NORMAL;
		
		th.DrawBackground(&dc, EP_EDITTEXT, nState, rBorder);
	}
}

void CRichEditNcBorder::VerifyThemedBorderState()
{
	BOOL bHadThemedBorder = m_bThemedBorder;
	m_bThemedBorder = FALSE;
	
	// First, check if the control is supposed to have a border
	if (bHadThemedBorder || HasStyle(WS_BORDER) || HasExStyle(WS_EX_CLIENTEDGE))
	{
		// Check if a theme is presently active
		if (CThemed().AreControlsThemed())
		{
			// Remove the border style, we don't want the control to draw its own border
			m_bThemedBorder = TRUE;
			
			CWnd::ModifyStyle(GetHwnd(), WS_BORDER, 0, 0);
			CWnd::ModifyStyleEx(GetHwnd(), WS_EX_CLIENTEDGE, 0, 0);
		}
		else // restore the border
			CWnd::ModifyStyleEx(GetHwnd(), 0, WS_EX_CLIENTEDGE, 0);
	}

	// Recalculate the NC area and repaint the window
	SetWindowPos(GetHwnd(), NULL, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	RedrawWindow(GetHwnd(), NULL, NULL, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_UPDATENOW | RDW_FRAME);
}
