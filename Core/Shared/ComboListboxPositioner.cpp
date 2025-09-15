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
	// We don't pass WC_COMBOLBOX here because it's more
	// efficient to check the class in OnCallWndRetProc
	return Instance().InitHooks(HM_CALLWNDPROCRET);
}

void CComboListboxPositioner::Release()
{
	GetInstance().ReleaseHooks();
}

BOOL CComboListboxPositioner::OnCallWndRetProc(const MSG& msg, LRESULT lr)
{   
	// Prevent re-entrancy
	if (m_bMovingListBox)
		return FALSE;
		
	switch (msg.message)
	{
	case WM_WINDOWPOSCHANGED:
		{
			// Sequence our checks to do the least work necessary
			const WINDOWPOS* pWPos = (const WINDOWPOS*)msg.lParam;

			// We're only interested in visible move/size events where the size is non-zero
			if (Misc::HasFlag(pWPos->flags, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW)) || (!pWPos->cx && !pWPos->cy))
				return FALSE;

			// Ensure that it's parented to the desktop
			if (::GetParent(msg.hwnd) != ::GetDesktopWindow())
				return FALSE;
				
			// And that it's a combo-listbox
			if (!::ClassMatches(msg.hwnd, WC_COMBOLBOX))
				return FALSE;
				
			// Do the fixup
			CAutoFlag af(m_bMovingListBox, TRUE);
			FixupListBoxPosition(msg.hwnd, *pWPos);
		}
		break;
	}
	
	return FALSE; // continue routing
}

void CComboListboxPositioner::FixupListBoxPosition(HWND hwndListbox, const WINDOWPOS& wpos)
{
	CRect rMonitor, rNewPos(CPoint(wpos.x, wpos.y), CSize(wpos.cx, wpos.cy));
	GraphicsMisc::GetAvailableScreenSpace(rNewPos, rMonitor);

	// Make sure at least some part of the listbox is visible
	if (CRect().IntersectRect(rNewPos, rMonitor) && GraphicsMisc::FitRect(rNewPos, rMonitor) && (rNewPos.left != wpos.x))
		::MoveWindow(hwndListbox, rNewPos.left, rNewPos.top, rNewPos.Width(), rNewPos.Height(), TRUE);
}
