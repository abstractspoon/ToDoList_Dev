// MouseWheelMgr.cpp: implementation of the CMouseWheelMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RTLStyleMgr.h"
#include "wclassdefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const DWORD RTL_STYLES			= (WS_EX_RTLREADING | WS_EX_RIGHT);

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
 		if (IsClass(msg.hwnd, WC_HEADER))
		{
			// don't change header controls because they don't like it!
		}
		else // all else
		{
			DWORD dwExStyle = (DWORD)::GetWindowLong(msg.hwnd, GWL_EXSTYLE);
			
			if ((dwExStyle & RTL_STYLES) != RTL_STYLES)
				::SetWindowLong(msg.hwnd, GWL_EXSTYLE, (dwExStyle | RTL_STYLES));

			// extra for trees
			if (IsClass(msg.hwnd, WC_TREEVIEW))
			{
				DWORD dwStyle = (DWORD)::GetWindowLong(msg.hwnd, GWL_STYLE);

				::SetWindowLong(msg.hwnd, GWL_STYLE, (dwStyle | TVS_RTLREADING));
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

BOOL CRTLStyleMgr::IsSystemFontRTL()
{
	LOGFONT lf = { 0 };
	
	if (::GetObject(::GetStockObject(SYSTEM_FONT), sizeof(lf), &lf))
	{
		switch (lf.lfCharSet)
		{
		case HEBREW_CHARSET:
		case ARABIC_CHARSET:
			return TRUE;

		case ANSI_CHARSET:
		case DEFAULT_CHARSET:
		case SYMBOL_CHARSET:
		case SHIFTJIS_CHARSET:
		case HANGUL_CHARSET:
		case GB2312_CHARSET:
		case CHINESEBIG5_CHARSET:
		case OEM_CHARSET:
		case JOHAB_CHARSET:
		case GREEK_CHARSET:
		case TURKISH_CHARSET:
		case VIETNAMESE_CHARSET:
		case THAI_CHARSET:
		case EASTEUROPE_CHARSET:
		case RUSSIAN_CHARSET:
		case MAC_CHARSET:
		case BALTIC_CHARSET:
			break;
		}
	}

	return FALSE;
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
