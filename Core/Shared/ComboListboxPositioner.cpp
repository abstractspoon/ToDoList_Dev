// ComboListboxPositioner.cpp: implementation of the CComboListboxPositioner class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComboListboxPositioner.h"
#include "WClassDefines.h"
#include "GraphicsMisc.h"
#include "Misc.h"
#include "AutoFlag.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComboListboxPositioner::CComboListboxPositioner() : m_bMovingListBox(FALSE)
{

}

CComboListboxPositioner::~CComboListboxPositioner()
{

}

BOOL CComboListboxPositioner::Initialize()
{
	// We handle our own class name checks for efficiency
	return Instance().InitHooks(HM_CALLWNDPROCRET/*, WC_COMBOLBOX*/);
}

void CComboListboxPositioner::Release()
{
	GetInstance().ReleaseHooks();
}

BOOL CComboListboxPositioner::OnCallWndRetProc(const MSG& msg, LRESULT lr)
{   
	if (!m_bMovingListBox)
	{
		ASSERT (m_hCallWndRetHook);

		if (msg.message == WM_WINDOWPOSCHANGED)
		{
			const WINDOWPOS* pWPos = (const WINDOWPOS*)msg.lParam;

			if (!Misc::HasFlag(pWPos->flags, (SWP_NOMOVE | SWP_NOSIZE)) && (pWPos->cx || pWPos->cy))
			{
				if (::ClassMatches(msg.hwnd, WC_COMBOLBOX))
				{
					CAutoFlag af(m_bMovingListBox, TRUE);
					FixupListBoxPosition(msg.hwnd, *pWPos);
				}
			}
		}
	}
	
	return FALSE; // continue routing
}

void CComboListboxPositioner::FixupListBoxPosition(HWND hwndListbox, const WINDOWPOS& wpos)
{
 	CRect rNewPos(CPoint(wpos.x, wpos.y), CSize(wpos.cx, wpos.cy));
	CPoint ptCursor(::GetMessagePos());

	// WE only fixup the X position
	if (GraphicsMisc::FitRectToScreen(rNewPos, &ptCursor) && (rNewPos.left != wpos.x))
	{
		::MoveWindow(hwndListbox, rNewPos.left, wpos.y, wpos.cx, wpos.cy, TRUE);
	}
}
