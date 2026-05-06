// FocusWatcher.cpp: implementation of the CFocusWatcher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FocusWatcher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CMap<HWND, HWND, BOOL, BOOL&> CFocusWatcher::s_mapCallbacks;

//////////////////////////////////////////////////////////////////////

BOOL CFocusWatcher::Initialize(HWND hwndCallback, BOOL bChildrenOnly)
{
	if (!::IsWindow(hwndCallback))
	{
		ASSERT(0);
		return FALSE;
	}

	if (s_mapCallbacks.GetCount() == 0)
	{
		if (!Instance().InitHooks(HM_CBT))
			return FALSE;
	}

	s_mapCallbacks[hwndCallback] = bChildrenOnly;
	return TRUE;
}

BOOL CFocusWatcher::OnCbt(int nCode, WPARAM wParam, LPARAM lParam)
{   
	ASSERT (m_hCbtHook);

	switch (nCode)
	{
	case HCBT_SETFOCUS:
		HandleFocusChange((HWND)wParam, (HWND)lParam);
		break;

	case HCBT_DESTROYWND:
		s_mapCallbacks.RemoveKey((HWND)wParam);

		if (s_mapCallbacks.GetCount() == 0)
			Instance().ReleaseHooks();
		break;
	}
	
	return FALSE; // continue routing
}

void CFocusWatcher::RefreshFocus(HWND hwndFocus)
{
	if (hwndFocus == NULL)
		hwndFocus = ::GetFocus();

	HandleFocusChange(hwndFocus, NULL);
}

void CFocusWatcher::HandleFocusChange(HWND hwndGotFocus, HWND hwndLostFocus)
{
	POSITION pos = s_mapCallbacks.GetStartPosition();

	while (pos)
	{
		HWND hwndCallback = NULL;
		BOOL bChildrenOnly = FALSE;

		s_mapCallbacks.GetNextAssoc(pos, hwndCallback, bChildrenOnly);

		// Windows should get cleaned up when they are destroyed
		// but it seems that the precise ordering of events is tricky
		if (!::IsWindow(hwndCallback))
			continue;

		if (bChildrenOnly)
		{
			BOOL bGotFocusIsChild = (hwndGotFocus && ::IsChild(hwndCallback, hwndGotFocus));
			BOOL bLostFocusIsChild = (hwndLostFocus && ::IsChild(hwndCallback, hwndLostFocus));
			
			if (!bGotFocusIsChild && !bLostFocusIsChild)
				continue;
		}

		// All else
		::SendMessage(hwndCallback, WM_FW_FOCUSCHANGE, (WPARAM)hwndGotFocus, (LPARAM)hwndLostFocus);
	}
}
