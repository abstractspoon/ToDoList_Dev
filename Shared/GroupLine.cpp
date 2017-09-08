// EditPrompt.cpp: implementation of the CGroupLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "groupline.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "themed.h"
#include "graphicsmisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupLine::CGroupLine()
{

}

CGroupLine::~CGroupLine()
{

}

BOOL CGroupLine::Initialize(HWND hwndStatic)
{
	ASSERT (hwndStatic);
	ASSERT (!IsHooked());
	ASSERT (CWinClasses::IsClass(hwndStatic, WC_STATIC));

	if (!IsHooked() && hwndStatic && HookWindow(hwndStatic))
	{
		GraphicsMisc::CreateFont(m_fontLabel, (HFONT)SendMessage(WM_GETFONT), GMFS_BOLD, GMFS_BOLD);

		Invalidate();
		return TRUE;
	}

	return FALSE;
}

LRESULT CGroupLine::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_SETTEXT:
		{
			Invalidate(TRUE);

			CRect rWindow;
			GetWindowRect(rWindow);

			CWnd* pParent = CWnd::FromHandle(GetParent());

			pParent->ScreenToClient(rWindow);
			pParent->InvalidateRect(rWindow, TRUE);
		}
		break;

	case WM_PAINT:
		{
			CWnd* pWnd = GetCWnd();
			CPaintDC dc(pWnd);

			CRect rClient;
			GetClientRect(rClient);

			CString sText;
			pWnd->GetWindowText(sText);

			DWORD dwStyle = pWnd->GetStyle();
			CThemed theme;

			UINT nFlags = (DT_TOP | DT_NOPREFIX);
			
			if (dwStyle & SS_RIGHT)
				nFlags |= DT_RIGHT;
			
			else if (dwStyle & SS_CENTER)
				nFlags |= DT_CENTER;
			else
				nFlags |= DT_LEFT;
			
			HFONT hOld = (HFONT)::SelectObject(dc, m_fontLabel);
			dc.SetBkMode(TRANSPARENT);

			CSize sizeText = dc.GetTextExtent(sText); // default
			
			if (theme.Open(pWnd, _T("BUTTON")))
			{
				theme.DrawText(&dc, BP_GROUPBOX, GBS_NORMAL, sText, nFlags, 0, rClient);

				CRect rExtents;
				theme.GetTextExtent(&dc, BP_GROUPBOX, GBS_NORMAL, sText, nFlags, rExtents, rClient);

				sizeText = rExtents.Size();
			}
			else // unthemed
			{
				dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
				dc.DrawText(sText, rClient, nFlags);
			}

			// draw line
			sizeText.cx += 4;
			
			int nYPos = ((rClient.top + rClient.bottom) / 2);
			
			if (dwStyle & SS_RIGHT)
			{
				GraphicsMisc::DrawHorzLine(&dc, rClient.left, rClient.right - sizeText.cx, nYPos,
											GetSysColor(COLOR_WINDOW), GetSysColor(COLOR_3DDKSHADOW));
			}
			else if (dwStyle & SS_CENTER)
			{
				GraphicsMisc::DrawHorzLine(&dc, rClient.left, rClient.right - (sizeText.cx / 2), nYPos,
											GetSysColor(COLOR_WINDOW), GetSysColor(COLOR_3DDKSHADOW));
				GraphicsMisc::DrawHorzLine(&dc, rClient.left + (sizeText.cx / 2), rClient.right, nYPos,
											GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_WINDOW));
			}
			else
			{
				GraphicsMisc::DrawHorzLine(&dc, rClient.left + sizeText.cx, rClient.right, nYPos,
											GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_WINDOW));
			}

			// cleanup
			::SelectObject(dc, hOld);


			return 0;
		}
		break;
	}

	return CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////

CGroupLineManager::CGroupLineManager()
{
}

CGroupLineManager::~CGroupLineManager()
{
	// cleanup
	HWND hwnd;
	CGroupLine* pGroupLine;

	POSITION pos = m_mapGroupLines.GetStartPosition();

	while (pos)
	{
		m_mapGroupLines.GetNextAssoc(pos, hwnd, pGroupLine);

		if (pGroupLine->IsValid())
			pGroupLine->HookWindow(NULL);

		delete pGroupLine;
	}
}

BOOL CGroupLineManager::AddGroupLine(UINT nIDStatic, HWND hwndParent)
{
	HWND hwndStatic = ::GetDlgItem(hwndParent, nIDStatic);
	
	if (!hwndStatic)
		return FALSE;

	// have we already got it?
	CGroupLine* pGroupLine = NULL;

	if (m_mapGroupLines.Lookup(hwndStatic, pGroupLine))
		return TRUE;

	// else create new editprompt
	pGroupLine = new CGroupLine;

	if (pGroupLine && pGroupLine->Initialize(hwndStatic))
	{
		m_mapGroupLines[hwndStatic] = pGroupLine;
		return TRUE;
	}
	
	// else
	delete pGroupLine;
	return FALSE;
}

