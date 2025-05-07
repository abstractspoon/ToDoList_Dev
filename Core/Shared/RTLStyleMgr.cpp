// MouseWheelMgr.cpp: implementation of the CMouseWheelMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RTLStyleMgr.h"
#include "WinClasses.h"
#include "wclassdefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTLStyleMgr::CRTLStyleMgr()
{
}

CRTLStyleMgr::~CRTLStyleMgr()
{
}

BOOL CRTLStyleMgr::Initialize()
{
	ASSERT(!IsRTL());

	return (!IsRTL() && Instance().InitHooks(HM_CALLWNDPROC));
}

BOOL CRTLStyleMgr::IsRTL()
{
	return (Instance().m_hCallWndHook != NULL);
}

void CRTLStyleMgr::Release()
{
	ASSERT(IsRTL());

	if (IsRTL())
		Instance().ReleaseHooks();
}

BOOL CRTLStyleMgr::OnCallWndProc(const MSG& msg)
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

 			if (CWinClasses::IsClass(sClass, WC_HEADER))
			{
				// don't change header controls because they don't like it!
			}
			else if (!CWinClasses::IsClass(sClass, WC_STATIC)) // all else except static text
			{
				DWORD dwExStyle = (DWORD)::GetWindowLong(msg.hwnd, GWL_EXSTYLE);
				dwExStyle |= WS_EX_RTLREADING;

				// extra for edits
				if (CWinClasses::IsEditControl(sClass, FALSE))
					dwExStyle |= WS_EX_RIGHT;

				::SetWindowLong(msg.hwnd, GWL_EXSTYLE, dwExStyle);

				// extra for trees
				if (IsClass(msg.hwnd, WC_TREEVIEW))
				{
					DWORD dwStyle = (DWORD)::GetWindowLong(msg.hwnd, GWL_STYLE);

					::SetWindowLong(msg.hwnd, GWL_STYLE, (dwStyle | TVS_RTLREADING));
				}
			}
		}
		break;

	case TCM_INSERTITEM:
	case TCM_SETITEM:
		if (IsClass(msg.hwnd, WC_TABCONTROL))
		{
			TCITEM* pTCI = (TCITEM*)msg.lParam;
			pTCI->mask |= TCIF_RTLREADING;
		}
		break;

		// don't change header controls because they don't like it!
// 	case HDM_SETITEM:
// 		if (IsClass(msg.hwnd, WC_HEADER))
// 		{
// 			HDITEM* pHDI = (HDITEM*)msg.lParam;
// 			
// 			pHDI->mask |= HDI_FORMAT;
// 			pHDI->fmt |= HDF_RTLREADING;
// 		}
// 		break;

	case TTM_ADDTOOL:
	case TTM_SETTOOLINFO:
	case TTM_UPDATETIPTEXT:
		if (IsClass(msg.hwnd, WC_TOOLTIPS))
		{
			TOOLINFO* pTI = (TOOLINFO*)msg.lParam;
			pTI->uFlags |= TTF_RTLREADING;		
		}
		break;

// 	case SB_SETTEXT:
// 		if (IsClass(msg.hwnd, WC_STATUSBAR))
// 		{
// 			BYTE nIndex = LOBYTE(LOWORD(msg.wParam));
// 			BYTE nStyle = HIBYTE(LOWORD(msg.wParam));
// 			
// 			msg.wParam = MAKEWPARAM(nIndex, (nStyle | SBT_RTLREADING));		
// 		}
// 		break;
	}
	
	return FALSE; // continue routing
}

BOOL CRTLStyleMgr::IsClass(HWND hWnd, LPCTSTR szClass)
{
	static TCHAR szWndClass[512] = { 0 };
	szWndClass[0] = 0;

	if (hWnd && ::GetClassName(hWnd, szWndClass, 511))
	{
		return (lstrcmpi(szClass, szWndClass) == 0);
	}

	// else
	return FALSE;
}
