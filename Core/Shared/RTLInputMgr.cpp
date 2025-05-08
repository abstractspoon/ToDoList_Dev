// MouseWheelMgr.cpp: implementation of the CMouseWheelMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RTLInputMgr.h"
#include "WinClasses.h"
#include "wclassdefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

// List of classes which we DON'T want to modify.
// Basically everything except non-edit type controls
// and non-containers because containers seem essential
// to getting RichEdit controls to work (for now).

LPCTSTR EXCLUSIONS[] = 
{
	WC_BUTTON,
	WC_STATIC,
//	WC_EDIT,
	WC_COMBOBOX,
	WC_COMBOBOXEX,
	WC_COMBOLBOX,
	WC_LISTBOX,
	WC_CHECKLISTBOX,
	WC_SCROLLBAR,
	WC_TOOLBAR,
	WC_SPIN,
	WC_PROGRESS,
	WC_SLIDER,
	WC_HOTKEY,
	WC_SHELLDLLDEFVIEW,
	WC_STATUSBAR,
	WC_ANIMATE,
//	WC_RICHEDIT,
//	WC_RICHEDIT20,
//	WC_RICHEDIT50,
	WC_DATETIMEPICK,
	WC_MONTHCAL,
	WC_REBAR,
//	WC_DIALOGBOX,
//	WC_MDICLIENT,
	WC_TOOLTIPS,
	WC_THUMBNAILVIEW,
	WC_IE,
	WC_SHDOCOBJVW,
	WC_SHELLEMBEDDING,
	WC_CONTROLBAR,
	WC_MENU,
	WC_DIRECTUIHWND,
	WC_CTRLNOTIFYSINK   
};

const int NUM_EXCLUSIONS = (sizeof(EXCLUSIONS) / sizeof(EXCLUSIONS[0]));

//////////////////////////////////////////////////////////////////////

BOOL WantClass(const CString& sClass)
{
	int nClass = NUM_EXCLUSIONS;

	while (nClass--)
	{
		if (CWinClasses::IsClass(sClass, EXCLUSIONS[nClass]))
			return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTLInputMgr::CRTLInputMgr()
{
}

CRTLInputMgr::~CRTLInputMgr()
{
}

BOOL CRTLInputMgr::Initialize()
{
	ASSERT(!IsEnabled());

	return (!IsEnabled() && Instance().InitHooks(HM_CALLWNDPROC));
}

BOOL CRTLInputMgr::IsEnabled()
{
	return (Instance().m_hCallWndHook != NULL);
}

void CRTLInputMgr::Release()
{
	ASSERT(IsEnabled());

	if (IsEnabled())
		Instance().ReleaseHooks();
}

BOOL CRTLInputMgr::OnCallWndProc(const MSG& msg)
{   
#ifdef _USRDLL
	// If this is a DLL, need to set up MFC state
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

	switch (msg.message)
	{
	case WM_CREATE:
		{
			CString sClass = CWinClasses::GetClass(msg.hwnd);

			if (WantClass(sClass))
			{
				DWORD dwExStyle = (DWORD)::GetWindowLong(msg.hwnd, GWL_EXSTYLE);
				dwExStyle |= WS_EX_RTLREADING;

				// extra for edits
				if (CWinClasses::IsClass(sClass, WC_EDIT))
					dwExStyle |= WS_EX_RIGHT;

				if (CWinClasses::IsWindowsFormsControl(sClass))
				{
					int breakpoint = 0;
				}

				::SetWindowLong(msg.hwnd, GWL_EXSTYLE, dwExStyle);
			}
		}
		break;
	}
	
	return FALSE; // continue routing
}
