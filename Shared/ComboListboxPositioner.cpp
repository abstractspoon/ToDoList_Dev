// ComboListboxPositioner.cpp: implementation of the CComboListboxPositioner class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComboListboxPositioner.h"
#include "WClassDefines.h"
#include "GraphicsMisc.h"
#include "Misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComboListboxPositioner::CComboListboxPositioner()
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
	ASSERT (m_hCallWndRetHook);

	if ((msg.message == WM_WINDOWPOSCHANGED) && ::ClassMatches(msg.hwnd, WC_COMBOLBOX))
	{
		WINDOWPOS* pWPos = (WINDOWPOS*)msg.lParam;

		FixupListBoxPosition(msg.hwnd, *pWPos);
	}
	
	return FALSE; // continue routing
}

void CComboListboxPositioner::FixupListBoxPosition(HWND hwndListbox, const WINDOWPOS& wpos)
{
	if (Misc::HasFlag(wpos.flags, (SWP_NOMOVE | SWP_NOSIZE)))
		return;

 	if (!wpos.cx && !wpos.cy)
		return;

	CRect rMonitor, rNewPos(wpos.x, wpos.y, (wpos.x + wpos.cx), (wpos.y + wpos.cy));
	GraphicsMisc::GetAvailableScreenSpace(rNewPos, rMonitor);

	if (rNewPos.right > rMonitor.right)
	{
		rNewPos.left = rMonitor.right - rNewPos.Width();
		rNewPos.right = rMonitor.right;
	}
	else if (rNewPos.left < rMonitor.left)
	{
		rNewPos.right = rMonitor.left + rNewPos.Width();
		rNewPos.left = rMonitor.left;
	}

	if (rNewPos.left != wpos.x)
		::MoveWindow(hwndListbox, rNewPos.left, rNewPos.top, rNewPos.Width(), rNewPos.Height(), TRUE);
}
