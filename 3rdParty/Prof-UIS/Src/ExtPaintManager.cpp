// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2004 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#include "StdAfx.h"

#if (defined __PROF_UIS_BUILTIN_MULTIMON_STUB)
	#define COMPILE_MULTIMON_STUBS
#endif // __PROF_UIS_BUILTIN_MULTIMON_STUB

// disable warning 4706
#pragma warning( push )
#pragma warning ( disable : 4706 )
	#include <multimon.h>
// rollback warning 4706
#pragma warning( pop )

#if _MFC_VER < 0x700
	#include <../src/AfxImpl.h>
#else
	#include <../src/mfc/AfxImpl.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#if (!defined __EXT_MENUCONTROLBAR_H)
	#include <ExtMenuControlBar.h>
#endif

#if (!defined __EXTDOCKBAR_H)
	#include <../Src/ExtDockBar.h>
#endif

#if (!defined __EXT_STATUSCONTROLBAR_H)
	#include <ExtStatusControlBar.h>
#endif

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	#if (!defined __EXT_CONTROLBAR_TABBED_FEATURES_H)
		#include "ExtControlBarTabbedFeatures.h"
	#endif // __EXT_CONTROLBAR_TABBED_FEATURES_H
#endif

#if( !defined __EXTMINIDOCKFRAMEWND_H)
	#include "ExtMiniDockFrameWnd.h"
#endif

#if (!defined __EXT_MFC_NO_TAB_CTRL)
	#if (!defined __EXT_TABWND_H)
		#include <ExtTabWnd.h>
	#endif
#endif // (!defined __EXT_MFC_NO_TAB_CTRL)

#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
	#if (!defined __EXTSHORTCUTLISTWND_H)
		#include <ExtShortcutListWnd.h>
	#endif
#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)


#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE( CExtPaintManager, CObject )
IMPLEMENT_DYNCREATE( CExtPaintManagerXP, CExtPaintManager )
IMPLEMENT_DYNCREATE( CExtPaintManagerOffice2003, CExtPaintManagerXP )

//#define __TRACE_PAINT_SESSIONS

#define __ExtMfc_CXTEXTMARGIN (-1) // 1
#define __ExtMfc_CYTEXTMARGIN (0)  // 1

CExtPaintManager::CExtPaintManagerAutoPtr g_PaintManager;

CExtPaintManager::CExtPaintManagerAutoPtr::
	CExtPaintManagerAutoPtr()
	: m_pPaintManager( NULL )
	, m_bHaveScrollLines( false )
	, m_uCachedScrollLines( 0 )
	, m_hInstUserExTheme( NULL )
	, m_bUxApiInitPassed( false )
	, m_bUxValidColorsExtracted( false )
	, m_bUxAppThemed( false )
	, m_bUxUseIfAvailOnWinXpOrLater( true )
	, m_strOsVer( _T("Unknown") )
	, m_bIsWin32s( false )
	, m_bIsWin9x( false )
	, m_bIsWin98( false )
	, m_bIsWin98orLater( false )
	, m_bIsWinNT( false )
	, m_bIsWinNT4( false )
	, m_bIsWin2000( false )
	, m_bIsWinXP( false )
	, m_bIsWinNT4orLater( false )
	, m_bIsWin2000orLater( false )
	, m_bIsWinXPorLater( false )
{
#ifdef WM_THEMECHANGED
	ASSERT( __ExtMfc_WM_THEMECHANGED == WM_THEMECHANGED );
#endif // WM_THEMECHANGED
	
	memset((char *)&m_osVerData,0,sizeof(OSVERSIONINFO));
	m_osVerData.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

CExtSafeString sTmp( _T("") );
	VERIFY( ::GetVersionEx(&m_osVerData) );
	switch(m_osVerData.dwPlatformId)
	{
	case VER_PLATFORM_WIN32s:
		m_bIsWin32s = true;
		sTmp = _T("Win32s");
	break;
	case VER_PLATFORM_WIN32_WINDOWS:
	{
		m_bIsWin9x = true;
		m_bIsWin98orLater = 
			(m_osVerData.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
			( (m_osVerData.dwMajorVersion > 4) ||
			( (m_osVerData.dwMajorVersion == 4) && (m_osVerData.dwMinorVersion > 0) )
			);
		sTmp = _T("Windows ");
		if(m_bIsWin98orLater)
		{
			if((m_osVerData.dwMajorVersion == 4) && (m_osVerData.dwMinorVersion > 0))
			{
				m_bIsWin98 = true;
				sTmp += _T("98");
			}
			else
				sTmp += _T("98 or later");
		}
		else
			sTmp += _T("95");
	}
	break;
	case VER_PLATFORM_WIN32_NT:
		m_bIsWinNT = true;
		if( m_osVerData.dwMajorVersion == 5 )
		{
			if( m_osVerData.dwMinorVersion == 0 )
			{
				m_bIsWin2000 = true;
				sTmp = _T("Windows 2000");
			}
			else if( m_osVerData.dwMinorVersion == 1 )
			{
				m_bIsWinXP = true;
				sTmp = _T("Windows XP");
			}
		}
		else if( m_osVerData.dwMajorVersion == 4 )
		{
			m_bIsWinNT4 = true;
			sTmp = _T("Windows NT");
		}
		else
			sTmp = _T("Windows NT");
		
		if( m_osVerData.dwMajorVersion >= 4 )
			m_bIsWinNT4orLater = true;
		if( m_osVerData.dwMajorVersion >= 5 )
		{
			m_bIsWin2000orLater = true;
			if(	!	(	m_osVerData.dwMajorVersion == 5
					&&	m_osVerData.dwMinorVersion == 0
					)
				)
				m_bIsWinXPorLater = true;
		}
	break;
	default:
	break;
	} // switch(m_osVerData.dwPlatformId)

	m_strOsVer.Format(
		_T("%s v.%lu.%lu (build %lu) %s"),
		sTmp,
		m_osVerData.dwMajorVersion, 
		m_osVerData.dwMinorVersion, 
		m_osVerData.dwBuildNumber,
		m_osVerData.szCSDVersion
		);
	
	// init uxtheme.dll data
	InitUserExApi();

	VERIFY(
		InitPaintManagerInstance()
		);
}

void CExtPaintManager::CExtPaintManagerAutoPtr::InitUserExApi()
{
	// init uxtheme.dll data
	if( m_bUxApiInitPassed && m_bUxValidColorsExtracted )
		return;
	m_bUxValidColorsExtracted = false;
	m_bUxAppThemed = false;
struct
{
	FARPROC * m_pFarProc;
	LPCSTR m_strProcName;
}arrUserExApi[] =
{
	{ (FARPROC*)&m_pUxApi_OpenThemeData, "OpenThemeData" },
	{ (FARPROC*)&m_pUxApi_CloseThemeData, "CloseThemeData" },
	{ (FARPROC*)&m_pUxApi_DrawThemeBackground, "DrawThemeBackground" },
	{ (FARPROC*)&m_pUxApi_DrawThemeText, "DrawThemeText" },
	{ (FARPROC*)&m_pUxApi_GetThemeBackgroundContentRect, "GetThemeBackgroundContentRect" },
	{ (FARPROC*)&m_pUxApi_GetThemeBackgroundExtent, "GetThemeBackgroundExtent" },
	{ (FARPROC*)&m_pUxApi_GetThemePartSize, "GetThemePartSize" },
	{ (FARPROC*)&m_pUxApi_GetThemeTextExtent, "GetThemeTextExtent" },
	{ (FARPROC*)&m_pUxApi_GetThemeTextMetrics, "GetThemeTextMetrics" },
	{ (FARPROC*)&m_pUxApi_GetThemeBackgroundRegion, "GetThemeBackgroundRegion" },
	{ (FARPROC*)&m_pUxApi_HitTestThemeBackground, "HitTestThemeBackground" },
	{ (FARPROC*)&m_pUxApi_DrawThemeEdge, "DrawThemeEdge" },
	{ (FARPROC*)&m_pUxApi_DrawThemeIcon, "DrawThemeIcon" },
	{ (FARPROC*)&m_pUxApi_IsThemePartDefined, "IsThemePartDefined" },
	{ (FARPROC*)&m_pUxApi_IsThemeBackgroundPartiallyTransparent, "IsThemeBackgroundPartiallyTransparent" },
	{ (FARPROC*)&m_pUxApi_GetThemeColor, "GetThemeColor" },
	{ (FARPROC*)&m_pUxApi_GetThemeMetric, "GetThemeMetric" },
	{ (FARPROC*)&m_pUxApi_GetThemeString, "GetThemeString" },
	{ (FARPROC*)&m_pUxApi_GetThemeBool, "GetThemeBool" },
	{ (FARPROC*)&m_pUxApi_GetThemeInt, "GetThemeInt" },
	{ (FARPROC*)&m_pUxApi_GetThemeEnumValue, "GetThemeEnumValue" },
	{ (FARPROC*)&m_pUxApi_GetThemePosition, "GetThemePosition" },
	{ (FARPROC*)&m_pUxApi_GetThemeFont, "GetThemeFont" },
	{ (FARPROC*)&m_pUxApi_GetThemeRect, "GetThemeRect" },
	{ (FARPROC*)&m_pUxApi_GetThemeMargins, "GetThemeMargins" },
	{ (FARPROC*)&m_pUxApi_GetThemeIntList, "GetThemeIntList" },
	{ (FARPROC*)&m_pUxApi_GetThemePropertyOrigin, "GetThemePropertyOrigin" },
	{ (FARPROC*)&m_pUxApi_SetWindowTheme, "SetWindowTheme" },
	{ (FARPROC*)&m_pUxApi_GetThemeFilename, "GetThemeFilename" },
	{ (FARPROC*)&m_pUxApi_GetThemeSysColor, "GetThemeSysColor" },
	{ (FARPROC*)&m_pUxApi_GetThemeSysColorBrush, "GetThemeSysColorBrush" },
	{ (FARPROC*)&m_pUxApi_GetThemeSysBool, "GetThemeSysBool" },
	{ (FARPROC*)&m_pUxApi_GetThemeSysSize, "GetThemeSysSize" },
	{ (FARPROC*)&m_pUxApi_GetThemeSysFont, "GetThemeSysFont" },
	{ (FARPROC*)&m_pUxApi_GetThemeSysString, "GetThemeSysString" },
	{ (FARPROC*)&m_pUxApi_GetThemeSysInt, "GetThemeSysInt" },
	{ (FARPROC*)&m_pUxApi_IsThemeActive, "IsThemeActive" },
	{ (FARPROC*)&m_pUxApi_IsAppThemed, "IsAppThemed" },
	{ (FARPROC*)&m_pUxApi_GetWindowTheme, "GetWindowTheme" },
	{ (FARPROC*)&m_pUxApi_EnableThemeDialogTexture, "EnableThemeDialogTexture" },
	{ (FARPROC*)&m_pUxApi_IsThemeDialogTextureEnabled, "IsThemeDialogTextureEnabled" },
	{ (FARPROC*)&m_pUxApi_GetThemeAppProperties, "GetThemeAppProperties" },
	{ (FARPROC*)&m_pUxApi_SetThemeAppProperties, "SetThemeAppProperties" },
	{ (FARPROC*)&m_pUxApi_GetCurrentThemeName, "GetCurrentThemeName" },
	{ (FARPROC*)&m_pUxApi_GetThemeDocumentationProperty, "GetThemeDocumentationProperty" },
	{ (FARPROC*)&m_pUxApi_DrawThemeParentBackground, "DrawThemeParentBackground" },
	{ (FARPROC*)&m_pUxApi_EnableTheming, "EnableTheming" },
	{ (FARPROC*)&m_pUxApi_DrawThemeBackgroundEx, "DrawThemeBackgroundEx" },
};
	if( m_hInstUserExTheme == NULL )
	{
		m_hInstUserExTheme =
			::GetModuleHandle( _T("uxtheme.dll") );
		if( m_hInstUserExTheme == NULL )
			m_hInstUserExTheme =
				::LoadLibrary( _T("uxtheme.dll") );
	} // if( m_hInstUserExTheme == NULL )
int nApiAvailCount = 0;
	for(	int nFunc = 0;
			nFunc < ( sizeof(arrUserExApi)/sizeof(arrUserExApi[0]) );
			nFunc++
			)
	{
		FARPROC pFarProc = NULL;
		if( m_hInstUserExTheme != NULL )
		{
			pFarProc = 
				::GetProcAddress(
					m_hInstUserExTheme,
					arrUserExApi[nFunc].m_strProcName
					);
			//ASSERT( pFarProc != NULL );
		} // if( m_hInstUserExTheme != NULL )
		if( pFarProc != NULL )
			nApiAvailCount ++;
		*(arrUserExApi[nFunc].m_pFarProc) = pFarProc;
	}

	if(		m_hInstUserExTheme != NULL
		&&	nApiAvailCount > 0
		&&	m_pUxApi_IsAppThemed != NULL
		&&	m_pUxApi_OpenThemeData != NULL
		&&	m_pUxApi_CloseThemeData != NULL
		&&	m_pUxApi_GetThemeColor != NULL
		)
	{
		m_bUxApiInitPassed = true;
		m_bUxAppThemed =
			m_pUxApi_IsAppThemed()
				? true
				: false;
		
		HWND hWndDesktop = ::GetDesktopWindow();
		ASSERT( hWndDesktop != NULL );
		EXT_UX_HTHEME hThemeTaskBand =
			m_pUxApi_OpenThemeData( hWndDesktop, L"taskband" );
		if( hThemeTaskBand != NULL )
		{
			if(
//					m_pUxApi_GetThemeColor(hThemeTaskBand,TDP_FLASHBUTTON,0,TMT_EDGELIGHTCOLOR,&m_clrUxTaskBandEdgeLightColor) == S_OK
//				&&	m_pUxApi_GetThemeColor(hThemeTaskBand,TDP_FLASHBUTTON,0,TMT_EDGEHIGHLIGHTCOLOR,&m_clrUxTaskBandEdgeHighLightColor) == S_OK
//				&&	m_pUxApi_GetThemeColor(hThemeTaskBand,TDP_FLASHBUTTON,0,TMT_EDGESHADOWCOLOR,&m_clrUxTaskBandEdgeShadowColor) == S_OK
//				&&	m_pUxApi_GetThemeColor(hThemeTaskBand,TDP_FLASHBUTTON,0,TMT_EDGEDKSHADOWCOLOR,&m_clrUxTaskBandEdgeDkShadowColor) == S_OK
//				&&	m_pUxApi_GetThemeColor(hThemeTaskBand,TDP_FLASHBUTTON,0,TMT_EDGEFILLCOLOR,&m_clrUxTaskBandEdgeFillColor) == S_OK
//				&&	m_pUxApi_GetThemeColor(hThemeTaskBand,TDP_FLASHBUTTON,0,TMT_FILLCOLORHINT,&m_clrUxTaskBandFillColorHint) == S_OK
//				&&	m_pUxApi_GetThemeColor(hThemeTaskBand,TDP_FLASHBUTTON,0,TMT_ACCENTCOLORHINT,&m_clrUxTaskBandAccentColorHint) == S_OK

					m_pUxApi_GetThemeColor( hThemeTaskBand, 0x02 /* TDP_FLASHBUTTON */, 0, 0x0EDC /* TMT_EDGELIGHTCOLOR */, &m_clrUxTaskBandEdgeLightColor ) == S_OK
				&&	m_pUxApi_GetThemeColor( hThemeTaskBand, 0x02 /* TDP_FLASHBUTTON */, 0, 0x0EDD /* TMT_EDGEHIGHLIGHTCOLOR */, &m_clrUxTaskBandEdgeHighLightColor ) == S_OK
				&&	m_pUxApi_GetThemeColor( hThemeTaskBand, 0x02 /* TDP_FLASHBUTTON */, 0, 0x0EDE /* TMT_EDGESHADOWCOLOR */, &m_clrUxTaskBandEdgeShadowColor ) == S_OK
				&&	m_pUxApi_GetThemeColor( hThemeTaskBand, 0x02 /* TDP_FLASHBUTTON */, 0, 0x0EDF /* TMT_EDGEDKSHADOWCOLOR */, &m_clrUxTaskBandEdgeDkShadowColor ) == S_OK
				&&	m_pUxApi_GetThemeColor( hThemeTaskBand, 0x02 /* TDP_FLASHBUTTON */, 0, 0x0EE0 /* TMT_EDGEFILLCOLOR */, &m_clrUxTaskBandEdgeFillColor ) == S_OK
				&&	m_pUxApi_GetThemeColor( hThemeTaskBand, 0x02 /* TDP_FLASHBUTTON */, 0, 0x0EED /* TMT_FILLCOLORHINT */, &m_clrUxTaskBandFillColorHint ) == S_OK
				&&	m_pUxApi_GetThemeColor( hThemeTaskBand, 0x02 /* TDP_FLASHBUTTON */, 0, 0x0EEF /* TMT_ACCENTCOLORHINT */, &m_clrUxTaskBandAccentColorHint ) == S_OK
				)
				m_bUxValidColorsExtracted = true;
			m_pUxApi_CloseThemeData( hThemeTaskBand );
		} // if( hThemeTaskBand != NULL )
	} // if( m_hInstUserExTheme != NULL ...
}

CExtPaintManager::CExtPaintManagerAutoPtr::
	~CExtPaintManagerAutoPtr()
{
	if( m_pPaintManager )
		delete m_pPaintManager;
	m_pPaintManager = NULL;
}

bool CExtPaintManager::CExtPaintManagerAutoPtr::
	InitPaintManagerInstance()
{
	if( m_pPaintManager != NULL )
		return true;
	return
		InstallPaintManager(
			(CExtPaintManager *)NULL
			);
}

CExtPaintManager *
	CExtPaintManager::CExtPaintManagerAutoPtr::
		operator->()
{
//	InitPaintManagerInstance();
	ASSERT( m_pPaintManager != NULL );
	return m_pPaintManager;
}

bool CExtPaintManager::CExtPaintManagerAutoPtr::
	InstallPaintManager(
		CExtPaintManager * pPaintManager
		)
{
	if( m_pPaintManager != NULL )
	{
		if( pPaintManager == m_pPaintManager )
			return true;
		if( m_pPaintManager != NULL )
		{
			delete m_pPaintManager;
			m_pPaintManager = NULL;
		}
	} // if( m_pPaintManager != NULL )

	if( pPaintManager != NULL )
		m_pPaintManager = pPaintManager;
	else
	{
		m_pPaintManager = new __DEFAULT_PAINT_MANAGER_CLASS;
		if(	!m_pPaintManager->
				IsKindOf(
					RUNTIME_CLASS( CExtPaintManager )
					)
			)
		{
			ASSERT( FALSE );
			return false;
		}
	}

	m_pPaintManager->SyncSysColors();
	m_pPaintManager->InitTranslatedColors();
	return true;
}

bool CExtPaintManager::CExtPaintManagerAutoPtr::
	InstallPaintManager(
		CRuntimeClass * pRtcPaintManager
		)
{
	if( pRtcPaintManager == NULL )
		return InitPaintManagerInstance();
CObject* pObj = pRtcPaintManager->CreateObject();
	if( pObj == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	ASSERT_VALID( pObj );
CExtPaintManager * pPaintManager =
		DYNAMIC_DOWNCAST(CExtPaintManager,pObj);
	if( pPaintManager == NULL )
	{
		delete pObj;
		ASSERT( FALSE );
		return false;
	}
	return InstallPaintManager( pPaintManager );
}

UINT CExtPaintManager::CExtPaintManagerAutoPtr::GetMouseWheelScrollLines()
{
	if( m_bHaveScrollLines )
		return m_uCachedScrollLines;
	m_bHaveScrollLines = true;
	m_uCachedScrollLines = 3;
#if _MFC_VER < 0x700
	if( !afxData.bWin4 )
	{
		HKEY hKey;
		if(	::RegOpenKeyEx(
				HKEY_CURRENT_USER,
				_T("Control Panel\\Desktop"),
				0,
				KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS
				)
		{
			TCHAR szData[256];
			DWORD dwKeyDataType;
			DWORD dwDataBufSize = _countof(szData);
			if( RegQueryValueEx(
					hKey,
					_T("WheelScrollLines"),
					NULL,
					&dwKeyDataType,
					(LPBYTE) &szData,
					&dwDataBufSize
					) == ERROR_SUCCESS
					)
			{
				m_uCachedScrollLines =
					_tcstoul( szData, NULL, 10 );
			}
			RegCloseKey(hKey);
		}
	}
	else
#endif // _MFC_VER < 0x700
		if( !afxData.bWin95 )
	{
		::SystemParametersInfo(
			SPI_GETWHEELSCROLLLINES,
			0,
			&m_uCachedScrollLines,
			0
			);
	}
	return m_uCachedScrollLines;
}

bool CExtPaintManager::CExtPaintManagerAutoPtr::IsWndUpdateSource(
	CWnd * pWndNotifySrc
	)
{
	ASSERT_VALID( pWndNotifySrc );
	ASSERT( pWndNotifySrc->GetSafeHwnd() != NULL && ::IsWindow(pWndNotifySrc->GetSafeHwnd()) );
	ASSERT( m_pPaintManager != NULL );
	if( pWndNotifySrc->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar)) )
		return true;
CFrameWnd * pFrame = DYNAMIC_DOWNCAST( CFrameWnd, pWndNotifySrc );
	if( pFrame == NULL )
		pFrame = pWndNotifySrc->GetParentFrame();
	if( pFrame == NULL )
		return true;
	if( pFrame->IsKindOf(RUNTIME_CLASS(CExtMiniDockFrameWnd)) )
		pFrame = pFrame->GetParentFrame();
	if(		pFrame == NULL
		||	pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd))
		)
		return true;
CWnd * pWnd = pFrame->GetControlBar( AFX_IDW_STATUS_BAR );
	if(		pWnd != NULL
		&&	pWnd->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar))
		)
	{
		if( pWnd == pWndNotifySrc )
			return true;
		return false;
	}
	pWnd = pFrame->GetControlBar( AFX_IDW_DOCKBAR_TOP );
	if(		pWnd != NULL
		&&	pWnd->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))
		)
	{
		if( pWnd == pWndNotifySrc )
			return true;
		return false;
	}
	pWnd = pFrame->GetControlBar( AFX_IDW_DOCKBAR_BOTTOM );
	if(		pWnd != NULL
		&&	pWnd->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))
		)
	{
		if( pWnd == pWndNotifySrc )
			return true;
		return false;
	}
	pWnd = pFrame->GetControlBar( AFX_IDW_DOCKBAR_LEFT );
	if(		pWnd != NULL
		&&	pWnd->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))
		)
	{
		if( pWnd == pWndNotifySrc )
			return true;
		return false;
	}
	pWnd = pFrame->GetControlBar( AFX_IDW_DOCKBAR_RIGHT );
	if(		pWnd != NULL
		&&	pWnd->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))
		)
	{
		if( pWnd == pWndNotifySrc )
			return true;
		return false;
	}
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	for(	pWnd = pFrame->GetWindow(GW_CHILD);
			pWnd != NULL;
			pWnd = pWnd->GetWindow(GW_HWNDNEXT)
			)
	{
		if( pWnd->IsKindOf(RUNTIME_CLASS(CExtTabMdiWnd)) )
		{
			if( pWnd == pWndNotifySrc )
				return true;
			return false;
		}
	}
#endif // if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	return true;
}

void CExtPaintManager::CExtPaintManagerAutoPtr::OnSysColorChange(
	CWnd * pWndNotifySrc
	)
{
	ASSERT_VALID( pWndNotifySrc );
	ASSERT( pWndNotifySrc->GetSafeHwnd() != NULL && ::IsWindow(pWndNotifySrc->GetSafeHwnd()) );
	ASSERT( m_pPaintManager != NULL );
	if( !IsWndUpdateSource(pWndNotifySrc) )
		return;
	m_pPaintManager->OnSysColorChange();
}

void CExtPaintManager::CExtPaintManagerAutoPtr::OnSettingChange(
	CWnd * pWndNotifySrc,
	UINT uFlags,
	__EXT_MFC_SAFE_LPCTSTR lpszSection
	)
{
	ASSERT_VALID( pWndNotifySrc );
	ASSERT( pWndNotifySrc->GetSafeHwnd() != NULL && ::IsWindow(pWndNotifySrc->GetSafeHwnd()) );
	ASSERT( m_pPaintManager != NULL );
	if( !IsWndUpdateSource(pWndNotifySrc) )
		return;
	m_bHaveScrollLines = false;
	m_pPaintManager->OnSettingChange( uFlags, lpszSection );
}

void CExtPaintManager::CExtPaintManagerAutoPtr::OnDisplayChange(
	CWnd * pWndNotifySrc,
	INT nDepthBPP,
	CPoint ptSizes
	)
{
	ASSERT_VALID( pWndNotifySrc );
	ASSERT( pWndNotifySrc->GetSafeHwnd() != NULL && ::IsWindow(pWndNotifySrc->GetSafeHwnd()) );
	ASSERT( m_pPaintManager != NULL );
	if( !IsWndUpdateSource(pWndNotifySrc) )
		return;
	m_bHaveScrollLines = false;
	m_pPaintManager->OnDisplayChange( nDepthBPP, ptSizes );
}

void CExtPaintManager::CExtPaintManagerAutoPtr::OnThemeChanged(
	CWnd * pWndNotifySrc,
	WPARAM wParam,
	LPARAM lParam
	)
{
	ASSERT_VALID( pWndNotifySrc );
	ASSERT( pWndNotifySrc->GetSafeHwnd() != NULL && ::IsWindow(pWndNotifySrc->GetSafeHwnd()) );
	ASSERT( m_pPaintManager != NULL );
	if( !IsWndUpdateSource(pWndNotifySrc) )
		return;
	m_bHaveScrollLines = false;
	m_pPaintManager->OnThemeChanged( wParam, lParam );
}

CExtPaintManager::CExtPaintManager()
	: m_bUseTahomaFont( true )
{
	ASSERT( this != NULL );

	m_DSS_DISABLED_style = DSS_DISABLED;

/**/
#define __3D_COLOR_DEPTH_R 7
#define __3D_COLOR_DEPTH_G 6
#define __3D_COLOR_DEPTH_B 6

struct
{
    LOGPALETTE    LogPalette;
    PALETTEENTRY  PalEntry[ (__3D_COLOR_DEPTH_R*__3D_COLOR_DEPTH_G*__3D_COLOR_DEPTH_B) ];
} pal;
LOGPALETTE * pLogPalette = (LOGPALETTE*) &pal;
	pLogPalette->palVersion    = 0x300;
	pLogPalette->palNumEntries = (WORD)(__3D_COLOR_DEPTH_R*__3D_COLOR_DEPTH_G*__3D_COLOR_DEPTH_B); 
int i = 0;
    for( int nX = 0; nX < __3D_COLOR_DEPTH_R; nX++ )
    {
		for( int nY = 0; nY < __3D_COLOR_DEPTH_G; nY++ )
		{
			for( int nZ = 0; nZ < __3D_COLOR_DEPTH_B; nZ++ )
			{
				pLogPalette->palPalEntry[i].peRed   =
					BYTE( (nX*255)/(__3D_COLOR_DEPTH_R-1) );
				pLogPalette->palPalEntry[i].peGreen =
					BYTE( (nY*255)/(__3D_COLOR_DEPTH_G-1) );
				pLogPalette->palPalEntry[i].peBlue  =
					BYTE( (nZ*255)/(__3D_COLOR_DEPTH_B-1) );
				pLogPalette->palPalEntry[i].peFlags = 0;
				i++;
			}
		}
    }
    VERIFY( m_PaletteWide.CreatePalette(pLogPalette) );
/**/
}

CExtPaintManager::~CExtPaintManager()
{
	ASSERT( this != NULL );

	m_colors.RemoveAll();
	m_brushes.RemoveAll();
	CachedDcFreeAll();
}

CExtPaintManagerXP::CExtPaintManagerXP()
	: m_bEnabledHoverIconShadows( true )
	, m_bHelperXpStyle8BitBarRect( true )
	, m_bExpBtnSwapVGlyphs( false )
	, m_pGlyphTbEpBtnH0( &g_glyph_btn_expand_bottom )
	, m_pGlyphTbEpBtnH1( &g_glyph_btn_expand_right2 )
	, m_pGlyphTbEpBtnV0( &g_glyph_btn_expand_left )
	, m_pGlyphTbEpBtnV1( &g_glyph_btn_expand_bottom2 )
	, m_nIdxClrMlaNormLeft( -1 )
	, m_nIdxClrMlaNormRight( -1 )
	, m_nIdxClrMlaRarelyLeft( -1 )
	, m_nIdxClrMlaRarelyRight( -1 )
	, m_nIdxClrBtnHoverLeft( -1 )
	, m_nIdxClrBtnHoverRight( -1 )
	, m_nIdxClrBtnPressedLeft( -1 )
	, m_nIdxClrBtnPressedRight( -1 )
	, m_nIdxClrBtnHovPresLeft( -1 )
	, m_nIdxClrBtnHovPresRight( -1 )
{
}

CExtPaintManagerXP::~CExtPaintManagerXP()
{
}

CExtPaintManagerOffice2003::CExtPaintManagerOffice2003()
	: m_sizeToolBarRgnRounders( 4, 4 )
	, m_sizeMenuExpBtnCircle( 16, 16 )
	, m_nIdxClrTbFillMargin( -1 )
{
	m_bEnabledHoverIconShadows = false;
	m_bHelperXpStyle8BitBarRect = false;
	m_bExpBtnSwapVGlyphs = true;
	m_pGlyphTbEpBtnH0 = &g_glyph_btn_expand_bottom_2003;
	m_pGlyphTbEpBtnH1 = &g_glyph_btn_expand_right2_2003;
	m_pGlyphTbEpBtnV0 = &g_glyph_btn_expand_right_2003;
	m_pGlyphTbEpBtnV1 = &g_glyph_btn_expand_bottom2_2003;
	m_nIdxClrMlaNormLeft = _2003CLR_MLA_NORM_LEFT;
	m_nIdxClrMlaNormRight = _2003CLR_MLA_NORM_RIGHT;
	m_nIdxClrMlaRarelyLeft = _2003CLR_MLA_RARELY_LEFT;
	m_nIdxClrMlaRarelyRight = _2003CLR_MLA_RARELY_RIGHT;
	POINT arrMenuExpBtnCirclePoints[] =
	{
		{  4,  0 },
		{ 11,  0 },
		{ 15,  4 },
		{ 15, 11 },
		{ 11, 15 },
		{  4, 15 },
		{  0, 11 },
		{  0,  4 },
	};
	VERIFY(
		m_rgnMenuExpBtnCircle.CreatePolygonRgn(
			arrMenuExpBtnCirclePoints,
			sizeof(arrMenuExpBtnCirclePoints)/sizeof(arrMenuExpBtnCirclePoints[0]),
			ALTERNATE
			)
		);
}

CExtPaintManagerOffice2003::~CExtPaintManagerOffice2003()
{
}

CDC & CExtPaintManager::CachedDcGet(
	HGDIOBJ hBmp,
	INT nIdx // = 0
	)
{
#ifdef __TRACE_PAINT_SESSIONS
		TRACE1( "   PAINT_SESSION: DC %d\n", nIdx );
#endif // __TRACE_PAINT_SESSIONS
	ASSERT( hBmp != NULL );
	ASSERT( 0 <= nIdx && nIdx < (sizeof(m_arrSurfaceCache)/sizeof(m_arrSurfaceCache[0])) );
	return m_arrSurfaceCache[nIdx].Alloc( hBmp );
}

void CExtPaintManager::CachedDcFreeAll()
{
	for(	INT nIdx = 0;
			nIdx < (sizeof(m_arrSurfaceCache)/sizeof(m_arrSurfaceCache[0]));
			nIdx++
			)
	{
		m_arrSurfaceCache[nIdx].Free();
	}
}

bool CExtPaintManager::glyph_t::
	stat_DefGlyphRgnTransparencyFunc(
		const glyph_t & _glyph,
		int x,
		int y,
		UINT nColor,
		LPVOID lpCoockie
		)
{
	_glyph;
	x;
	y;
	lpCoockie;
	return (nColor == 0) ? true : false;
}

void CExtPaintManager::stat_PassPaintMessages(
	bool bPassNcPaint, // = true
	bool bPassEraseBkgnd, // = true
	bool bPassPaint // = true
	)
{
MSG msg;
	if( bPassNcPaint )
	{
		while( ::PeekMessage( &msg, NULL, WM_NCPAINT, WM_NCPAINT, PM_NOREMOVE ) )
		{
			if( ! ::GetMessage( &msg, NULL, WM_NCPAINT, WM_NCPAINT ) )
				break;
			::DispatchMessage( &msg );
		} // while( ::PeekMessage( &msg, NULL, WM_NCPAINT, WM_NCPAINT, PM_NOREMOVE ) )
	} // if( bPassNcPaint )
	if( bPassEraseBkgnd )
	{
		while( ::PeekMessage( &msg, NULL, WM_ERASEBKGND, WM_ERASEBKGND, PM_NOREMOVE ) )
		{
			if( ! ::GetMessage( &msg, NULL, WM_ERASEBKGND, WM_ERASEBKGND ) )
				break;
			::DispatchMessage( &msg );
		} // while( ::PeekMessage( &msg, NULL, WM_ERASEBKGND, WM_ERASEBKGND, PM_NOREMOVE ) )
	} // if( bPassEraseBkgnd )
	if( bPassPaint )
	{
		while( ::PeekMessage( &msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE ) )
		{
			if( ! ::GetMessage( &msg, NULL, WM_PAINT, WM_PAINT ) )
				break;
			::DispatchMessage( &msg );
		} // while( ::PeekMessage( &msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE ) )
	} // if( bPassPaint )
}

bool CExtPaintManager::stat_ResizeBitmap(
	CBitmap & bmpSrc,
	CSize _size,
	CBrush & brushFill
	)
{
	ASSERT( bmpSrc.GetSafeHandle() != NULL );
CDC dcSrc, dcDst;
CBitmap * pBmpOldSrc, *pBmpOldDst, bmpNew;
	{ // block
		if(		(! dcSrc.CreateCompatibleDC( NULL ) )
			||	(! dcDst.CreateCompatibleDC( NULL ) )
			)
		{
			ASSERT( FALSE );
			return false;
		}
		pBmpOldSrc = dcSrc.SelectObject( &bmpSrc );
		if( !bmpNew.CreateCompatibleBitmap(
				&dcSrc,
				_size.cx,
				_size.cy
				)
			)
		{
			ASSERT( FALSE );
			dcSrc.SelectObject( pBmpOldSrc );
			return false;
		}
		pBmpOldDst = dcSrc.SelectObject( &bmpNew );
	} // block

CRect rcFill( 0, 0, _size.cx, _size.cy );
	dcDst.FillRect( &rcFill, &brushFill );
	VERIFY(
		dcDst.BitBlt(
			0, 0, _size.cx, _size.cy,
			&dcSrc,
			0, 0,
			SRCCOPY
			)
		);
	dcSrc.SelectObject( pBmpOldSrc );
	dcDst.SelectObject( pBmpOldDst );
	bmpSrc.DeleteObject();
	bmpSrc.Attach( bmpNew.Detach() );
	return true;
}

HICON CExtPaintManager::stat_HBITMAP2HICON(
	HBITMAP hBmpSrc,
	COLORREF clrTransparent,
	const RECT * pRcPart, // = NULL
	bool bReplaceSysColors // = true
	)
{
	if( hBmpSrc == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}

BITMAP bmpDescription;
	::memset( &bmpDescription, 0, sizeof(BITMAP) );
	::GetObject( hBmpSrc, sizeof(BITMAP), &bmpDescription );
	ASSERT( bmpDescription.bmWidth > 0 && bmpDescription.bmHeight > 0 );
CImageList _il;
	if( !_il.Create(
			bmpDescription.bmWidth,
			bmpDescription.bmHeight,
			ILC_COLOR32|ILC_MASK,
			0,
			1
			)
		)
	{
		ASSERT( FALSE );
		return NULL;
	}

COLORREF * pSurface = NULL;
HBITMAP hBmpExtracted = stat_CloneBitmap( hBmpSrc, pRcPart, &pSurface );
	if( hBmpExtracted == NULL || pSurface == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
CBitmap bmpExtracted;
	bmpExtracted.Attach( hBmpExtracted );
	if( bReplaceSysColors )
	{
		COLORREF quadClrTransparent =
			RGB(
				GetBValue(clrTransparent),
				GetGValue(clrTransparent),
				GetRValue(clrTransparent)
				);
		struct
		{
			COLORREF m_quadClrSrc;
			COLORREF m_quadClrDst;
			INT m_nDstSysColorIdx;
			bool m_bReplace;
		} _repl_arr[] =
		{
			{ RGB(128,128,128), RGB(0,0,0), COLOR_3DSHADOW, true },
			{ RGB(192,192,192), RGB(0,0,0), COLOR_3DFACE, true },
			{ RGB(223,223,223), RGB(0,0,0), COLOR_3DLIGHT, true },
		};
		const int nReplCount =
			sizeof(_repl_arr) / sizeof(_repl_arr[0]);
		int nReplReal = 0;
		for( int n = 0; n < nReplCount; n++ )
		{
			if( _repl_arr[n].m_quadClrSrc == quadClrTransparent )
			{
				_repl_arr[n].m_bReplace = false;
				continue;
			}
			_repl_arr[n].m_quadClrDst =
				::GetSysColor( _repl_arr[n].m_nDstSysColorIdx );
			_repl_arr[n].m_quadClrDst =
				RGB(
					GetBValue(_repl_arr[n].m_quadClrDst),
					GetGValue(_repl_arr[n].m_quadClrDst),
					GetRValue(_repl_arr[n].m_quadClrDst)
					);
			if( _repl_arr[n].m_quadClrDst == _repl_arr[n].m_quadClrSrc )
			{
				_repl_arr[n].m_bReplace = false;
				continue;
			}
			nReplReal++;
		} // for( int n = 0; n < nReplCount; n++ )
		if( nReplReal > 0 )
		{
			LONG nPixelCount =
				bmpDescription.bmWidth * bmpDescription.bmHeight;
			for( LONG nPos = 0; nPos < nPixelCount; nPos++ )
			{
				if( pSurface[nPos] == quadClrTransparent )
					continue;
				for( int n = 0; n < nReplCount; n++ )
				{
					if( ! (_repl_arr[n].m_bReplace) )
						continue;
					if( pSurface[nPos] != _repl_arr[n].m_quadClrSrc )
						continue;
					pSurface[nPos] = _repl_arr[n].m_quadClrDst;
				} // for( int n = 0; n < nReplCount; n++ )
			} // for( LONG nPos = 0; nPos < nPixelCount; nPos++ )
		} // if( nReplReal > 0 )
	} // if( bReplaceSysColors )
int nImgIdx =
		_il.Add( &bmpExtracted, clrTransparent );
	if( nImgIdx < 0 )
	{
		ASSERT( FALSE );
		return NULL;
	}
HICON hIcon = _il.ExtractIcon( nImgIdx );
	ASSERT( hIcon != NULL );
	return hIcon;
}

HBITMAP CExtPaintManager::stat_CloneBitmap(
	HBITMAP hBmpSrc,
	const RECT * pRcPart, // = NULL
	COLORREF ** ppSurface // = NULL // if not NULL - create 32 bit surface
	)
{
	if( hBmpSrc == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
BITMAP _bmpInfo;
	::memset( &_bmpInfo, 0, sizeof(BITMAP) );
	::GetObject( hBmpSrc, sizeof(BITMAP), &_bmpInfo );
#ifdef _DEBUG
DWORD dwLastError = ::GetLastError();
	dwLastError;
#endif // _DEBUG
	ASSERT( _bmpInfo.bmWidth > 0 && _bmpInfo.bmHeight > 0 );
CPoint _ptSrc( 0, 0 );
CSize _sizeDest( _bmpInfo.bmWidth, _bmpInfo.bmHeight );
	if( pRcPart != NULL )
	{
		ASSERT( pRcPart->left < pRcPart->right );
		ASSERT( pRcPart->top < pRcPart->bottom );
		ASSERT( 0 <= pRcPart->left && pRcPart->left <= _bmpInfo.bmWidth );
		ASSERT( 0 <= pRcPart->right && pRcPart->right <= _bmpInfo.bmWidth );
		ASSERT( 0 <= pRcPart->top && pRcPart->top <= _bmpInfo.bmHeight );
		ASSERT( 0 <= pRcPart->bottom && pRcPart->bottom <= _bmpInfo.bmHeight );
		_sizeDest.cx = pRcPart->right - pRcPart->left;
		_sizeDest.cy = pRcPart->bottom - pRcPart->top;
		ASSERT( _sizeDest.cx <= _bmpInfo.bmWidth );
		ASSERT( _sizeDest.cy <= _bmpInfo.bmHeight );
		_ptSrc.x = pRcPart->left;
		_ptSrc.y = pRcPart->top;
	} // if( pRcPart != NULL )

HDC hSrcDC = ::CreateCompatibleDC( NULL );
	if( hSrcDC == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
HGDIOBJ hBmpSrcOld = ::SelectObject( hSrcDC, (HGDIOBJ)hBmpSrc );
HBITMAP hBmpDst = NULL;
	if( ppSurface != NULL )
	{
		BITMAPINFOHEADER bih;
		::memset( &bih, 0, sizeof(BITMAPINFOHEADER) );
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biWidth = _sizeDest.cx;
		bih.biHeight = _sizeDest.cy;
		bih.biPlanes = 1;
		bih.biBitCount = 32;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = _sizeDest.cx * _sizeDest.cy;
		hBmpDst =
			::CreateDIBSection(
				hSrcDC,
				(LPBITMAPINFO)&bih,
				DIB_RGB_COLORS,
				(void **)ppSurface,
				NULL,
				NULL
				);
		ASSERT( hBmpDst != NULL );
		ASSERT( (*ppSurface) != NULL );
	} // if( ppSurface != NULL )
	else
	{
		hBmpDst =
			::CreateCompatibleBitmap(
				hSrcDC,
				_sizeDest.cx,
				_sizeDest.cy
				);
		ASSERT( hBmpDst != NULL );
	} // else from if( ppSurface != NULL )
	if( hBmpDst == NULL )
	{
		ASSERT( FALSE );
		::SelectObject( hSrcDC, (HGDIOBJ)hBmpSrcOld );
		::DeleteDC( hSrcDC );
		return NULL;
	}
HDC hDstDC = ::CreateCompatibleDC( NULL );
	if( hDstDC == NULL )
	{
		ASSERT( FALSE );
		::DeleteObject( hBmpDst );
		::SelectObject( hSrcDC, (HGDIOBJ)hBmpSrcOld );
		::DeleteDC( hSrcDC );
		return NULL;
	}
HGDIOBJ hBmpDstOld = ::SelectObject( hDstDC, (HGDIOBJ)hBmpDst );
	VERIFY(
		::BitBlt(
			hDstDC,
			0, 0, _sizeDest.cx, _sizeDest.cy,
			hSrcDC,
			_ptSrc.x, _ptSrc.y,
			SRCCOPY
			)
		);
	::SelectObject( hDstDC, (HGDIOBJ)hBmpDstOld );
	::DeleteDC( hDstDC );
	::SelectObject( hSrcDC, (HGDIOBJ)hBmpSrcOld );
	::DeleteDC( hSrcDC );
	return hBmpDst;
}

bool CExtPaintManager::stat_CloneBitmap(
	CBitmap & bmpDst,
	const CBitmap & bmpSrc,
	const RECT * pRcPart, // = NULL
	COLORREF ** ppSurface // = NULL // if not NULL - create 32 bit surface
	)
{
	if( bmpSrc.GetSafeHandle() == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	if( bmpDst.GetSafeHandle() != NULL )
		bmpDst.DeleteObject();
HBITMAP hBmpDst =
		stat_CloneBitmap(
			(HBITMAP)bmpSrc.GetSafeHandle(),
			pRcPart,
			ppSurface
			);
	if( hBmpDst == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	bmpDst.Attach( hBmpDst );
	return true;
}

INT CExtPaintManager::stat_GetBitmapColorMap( // returns bitmap's color bit count or zero if error
	HBITMAP hBmpSrc,
	CMap < COLORREF, COLORREF, DWORD, DWORD > & _map // key is color, value is count of color entries in bitmap
	)
{
	_map.RemoveAll();
	if( hBmpSrc == NULL )
	{
		ASSERT( FALSE );
		return 0;
	}
BITMAP bmpSrcDescription;
	::memset( &bmpSrcDescription, 0, sizeof(BITMAP) );
	::GetObject( hBmpSrc, sizeof(BITMAP), &bmpSrcDescription );
	ASSERT( bmpSrcDescription.bmWidth > 0 && bmpSrcDescription.bmHeight > 0 );

HDC hSrcDC = ::CreateCompatibleDC( NULL );
	if( hSrcDC == NULL )
	{
		ASSERT( FALSE );
		return 0;
	}
LONG nPixelCount =
		bmpSrcDescription.bmWidth*bmpSrcDescription.bmHeight;
HGDIOBJ hBmpSrcOld = ::SelectObject( hSrcDC, (HGDIOBJ)hBmpSrc );
BITMAPINFOHEADER bih;
	::memset( &bih, 0, sizeof(BITMAPINFOHEADER) );
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = bmpSrcDescription.bmWidth;
	bih.biHeight = bmpSrcDescription.bmHeight;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = nPixelCount;
RGBQUAD * pColor = NULL;
HBITMAP hBmpDst =
		::CreateDIBSection(
			hSrcDC,
			(LPBITMAPINFO)&bih,
			DIB_RGB_COLORS,
			(void **)&pColor,
			NULL,
			NULL
			);
	if( hBmpDst == NULL )
	{
		ASSERT( FALSE );
		::SelectObject( hSrcDC, (HGDIOBJ)hBmpSrcOld );
		::DeleteDC( hSrcDC );
		return 0;
	}
	ASSERT( pColor != NULL );
HDC hDstDC = ::CreateCompatibleDC( NULL );
	if( hDstDC == NULL )
	{
		ASSERT( FALSE );
		::DeleteObject( hBmpDst );
		::SelectObject( hSrcDC, (HGDIOBJ)hBmpSrcOld );
		::DeleteDC( hSrcDC );
		return NULL;
	}
HGDIOBJ hBmpDstOld = ::SelectObject( hDstDC, (HGDIOBJ)hBmpDst );
	VERIFY(
		::BitBlt(
			hDstDC,
			0, 0,
			bmpSrcDescription.bmWidth, bmpSrcDescription.bmHeight,
			hSrcDC,
			0, 0,
			SRCCOPY
			)
		);

	for( ; nPixelCount > 0; pColor++, nPixelCount-- )
	{
		COLORREF clrToInsert =
			RGB(
				pColor->rgbRed,
				pColor->rgbGreen,
				pColor->rgbBlue
				);
		DWORD dwEntries = 0;
		if( _map.Lookup( clrToInsert, dwEntries ) )
		{
			ASSERT( dwEntries > 0 );
			dwEntries++;
			_map.SetAt( clrToInsert, dwEntries );
		} // if( _map.Lookup( clrToInsert, dwEntries ) )
		else
			_map.SetAt( clrToInsert, 1 );
	} // for( ; nPixelCount > 0; pColor++, nPixelCount-- )

	::SelectObject( hDstDC, (HGDIOBJ)hBmpDstOld );
	::DeleteDC( hDstDC );
	::SelectObject( hSrcDC, (HGDIOBJ)hBmpSrcOld );
	::DeleteDC( hSrcDC );
	::DeleteObject( hBmpDst );

	return bmpSrcDescription.bmBitsPixel;
}

INT CExtPaintManager::stat_GetBitmapColorArray( // returns bitmap's color bit count or zero if error
	HBITMAP hBmpSrc,
	CArray < COLORREF, COLORREF > & arr,
	bool bMakeUniqueColors // = true // used only if bit count <= 8
	)
{
	arr.RemoveAll();
	if( hBmpSrc == NULL )
	{
		ASSERT( FALSE );
		return 0;
	}
BITMAP bmpSrcDescription;
	::memset( &bmpSrcDescription, 0, sizeof(BITMAP) );
	::GetObject( hBmpSrc, sizeof(BITMAP), &bmpSrcDescription );
	ASSERT( bmpSrcDescription.bmWidth > 0 && bmpSrcDescription.bmHeight > 0 );
	if( bmpSrcDescription.bmBitsPixel <= 8 )
	{
		HGLOBAL hGlobalDst = stat_HBITMAPtoHGLOBAL( hBmpSrc );
		if( hGlobalDst == NULL )
		{
			ASSERT( FALSE );
			return 0;
		}
		INT nBitCount =  (INT) ((LPBITMAPINFOHEADER)hGlobalDst)->biBitCount;
		ASSERT( nBitCount <= 8 );
		INT nColorCount = 1L << nBitCount;
		RGBQUAD * pColor = (RGBQUAD *)
			(	((LPBYTE)hGlobalDst)
				+ ((LPBITMAPINFOHEADER)hGlobalDst)->biSize
			);
		for( INT i = 0; i < nColorCount; i++ )
		{
			COLORREF clrToInsert =
				RGB(
					pColor[i].rgbRed,
					pColor[i].rgbGreen,
					pColor[i].rgbBlue
					);
			arr.Add( clrToInsert );
		} // for( INT i = 0; i < nColorCount; i++ )
		::GlobalFree( hGlobalDst );
		if( bMakeUniqueColors )
		{
			INT nCount = arr.GetSize();
			if( nCount < 2 )
				return bmpSrcDescription.bmBitsPixel;
			for( INT i = 0; i < (nCount-1); i++ )
			{
				COLORREF clrTest = arr[i];
				for( INT j = i+1; j < nCount;  )
				{
					COLORREF clrToCmp = arr[j];
					if( clrTest == clrToCmp )
					{
						arr.RemoveAt( j, 1 );
						nCount --;
					} // if( clrTest == clrToCmp )
					else
						j++;
				} // for( INT j = i+1; j < nCount; )
			} // for( INT i = 0; i < (nCount-1); i++ )
		} // if( bMakeUniqueColors )
		return bmpSrcDescription.bmBitsPixel;
	} // if( bmpSrcDescription.bmBitsPixel <= 8 )

CMap < COLORREF, COLORREF, DWORD, DWORD > _map;
INT nVal = stat_GetBitmapColorMap( hBmpSrc, _map );
	if( nVal == 0 )
	{
		ASSERT( FALSE );
		return 0;
	}
	ASSERT( nVal == bmpSrcDescription.bmBitsPixel );
#ifdef _DEBUG
INT nCount = _map.GetCount();
#endif // _DEBUG
	ASSERT( nCount > 0 );
POSITION pos = _map.GetStartPosition();
	ASSERT( pos != NULL );
	for( ; pos != NULL; )
	{
		COLORREF clrToInsert = RGB(0,0,0);
		DWORD dwEntries = 0;
		_map.GetNextAssoc( pos, clrToInsert, dwEntries );
		ASSERT( dwEntries > 0 );
		arr.Add( clrToInsert );
	} // for( ; pos != NULL; )
	ASSERT( nCount == arr.GetSize() );

	return bmpSrcDescription.bmBitsPixel;
}

HGLOBAL CExtPaintManager::stat_HBITMAPtoHGLOBAL(
	HBITMAP hBmpSrc,
    HPALETTE hPalDst // = NULL
	)
{
	if( hBmpSrc == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
	if( hPalDst == NULL )
		hPalDst = (HPALETTE)::GetStockObject( DEFAULT_PALETTE );

BITMAP bmpSrcDescription;
	::memset( &bmpSrcDescription, 0, sizeof(BITMAP) );
	::GetObject( hBmpSrc, sizeof(BITMAP), &bmpSrcDescription );
	ASSERT( bmpSrcDescription.bmWidth > 0 && bmpSrcDescription.bmHeight > 0 );

BITMAPINFOHEADER bmpSrcInfoHdr;
	::memset( &bmpSrcInfoHdr, 0, sizeof(BITMAPINFOHEADER) );
	bmpSrcInfoHdr.biSize = sizeof(BITMAPINFOHEADER);
	bmpSrcInfoHdr.biWidth = bmpSrcDescription.bmWidth;
	bmpSrcInfoHdr.biHeight = bmpSrcDescription.bmHeight;
	bmpSrcInfoHdr.biPlanes = 1;
	bmpSrcInfoHdr.biBitCount = (WORD)
		( bmpSrcDescription.bmPlanes * bmpSrcDescription.bmBitsPixel );
	bmpSrcInfoHdr.biCompression = BI_RGB;

INT nColors =
		(bmpSrcInfoHdr.biBitCount > 8)
			? 0
			: (1 << bmpSrcInfoHdr.biBitCount)
		;
DWORD dwLenAllocPart =
		bmpSrcInfoHdr.biSize
		+ nColors * sizeof(RGBQUAD)
		;

HDC hDcSrc = ::CreateCompatibleDC( NULL );
	if( hDcSrc == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
	hPalDst = ::SelectPalette( hDcSrc, hPalDst, FALSE );
	::RealizePalette( hDcSrc );

HGLOBAL	hGlobalDst =
		::GlobalAlloc(
			GMEM_FIXED,
			dwLenAllocPart
			);
	if( hGlobalDst == NULL )
	{
		ASSERT( FALSE );
		::SelectPalette( hDcSrc, hPalDst, FALSE );
		::DeleteDC( hDcSrc );
		return NULL;
	} // if( hBmpDst == NULL )
	
LPBITMAPINFOHEADER  lpBmpSrcInfoHdr =
		(LPBITMAPINFOHEADER)hGlobalDst;
	*lpBmpSrcInfoHdr = bmpSrcInfoHdr;
	::GetDIBits(
		hDcSrc,
		hBmpSrc,
		0L,
		(DWORD)bmpSrcInfoHdr.biHeight,
		(LPBYTE)NULL,
		(LPBITMAPINFO)lpBmpSrcInfoHdr,
		(DWORD)DIB_RGB_COLORS
		);
	bmpSrcInfoHdr = *lpBmpSrcInfoHdr;
	if( bmpSrcInfoHdr.biSizeImage == 0 )
		bmpSrcInfoHdr.biSizeImage = 
			(
				(
					(
						(
							bmpSrcInfoHdr.biWidth
							* bmpSrcInfoHdr.biBitCount
						) + 31
					) & ~31
				) / 8
			) * bmpSrcInfoHdr.biHeight;

	dwLenAllocPart += bmpSrcInfoHdr.biSizeImage;
HGLOBAL hGlobalTmp =
		::GlobalReAlloc(
			hGlobalDst,
			dwLenAllocPart,
			GMEM_MOVEABLE
			);
	if( hGlobalTmp == NULL  )
	{
		ASSERT( FALSE );
		::GlobalFree( hGlobalDst ) ;
		::SelectPalette( hDcSrc, hPalDst, FALSE );
		::DeleteDC( hDcSrc );
		return NULL;
	} // if( bmpSrcInfoHdr.biSizeImage == 0 )
	hGlobalDst = hGlobalTmp;

	lpBmpSrcInfoHdr = (LPBITMAPINFOHEADER)hGlobalDst;
	if(	!	(::GetDIBits(
				hDcSrc,
				hBmpSrc,
				0L,
				(DWORD)bmpSrcInfoHdr.biHeight,
				(LPBYTE)lpBmpSrcInfoHdr + (bmpSrcInfoHdr.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpBmpSrcInfoHdr,
				(DWORD)DIB_RGB_COLORS
				)
			)
		)
	{
		ASSERT( FALSE );
		::GlobalFree( hGlobalDst );
		hGlobalDst = NULL;
	}
	::SelectPalette( hDcSrc, hPalDst, FALSE );
	::DeleteDC( hDcSrc );
	return hGlobalDst;
}

bool CExtPaintManager::stat_SerializeBitmap(
	CArchive & ar,
	HBITMAP * p_hBmp,
	DWORD dwResourceSize // = 0L // not 0 only if loaded from resources
	)
{
	ASSERT( p_hBmp != NULL );
	if( ar.IsStoring() )
	{
		ASSERT( (*p_hBmp) != NULL );
		HGLOBAL hGlobalDst = stat_HBITMAPtoHGLOBAL( *p_hBmp  );
		if( hGlobalDst == NULL )
		{
			ASSERT( FALSE );
			return false;
		}
		DWORD dwSizeofBmpTmp = ::GlobalSize( hGlobalDst );
		ASSERT( dwSizeofBmpTmp > 0 );
		BITMAPFILEHEADER bmpFileHdr;
		::memset( &bmpFileHdr, 0, sizeof(BITMAPFILEHEADER) );
		bmpFileHdr.bfType = ( (WORD)( 'M' << 8 ) | (WORD)('B') );
		bmpFileHdr.bfSize = sizeof(BITMAPFILEHEADER) + dwSizeofBmpTmp;
		bmpFileHdr.bfOffBits = (DWORD)
			(	sizeof(BITMAPFILEHEADER)
				+	((LPBITMAPINFOHEADER)hGlobalDst)->biSize
				+	(
						( ((LPBITMAPINFOHEADER)hGlobalDst)->biBitCount > 8 )
							? 0L
							: ( 1L << ((LPBITMAPINFOHEADER)hGlobalDst)->biBitCount )
					) * sizeof(RGBQUAD)
			);

		ar.Write( (LPVOID)&bmpFileHdr, sizeof(BITMAPFILEHEADER) );
		ar.Write( (LPVOID)hGlobalDst, dwSizeofBmpTmp );
		::GlobalFree( hGlobalDst );
	} // if( ar.IsStoring() )
	else
	{
		ASSERT( (*p_hBmp) == NULL );
		BITMAPFILEHEADER bmpFileHdr;
		::memset( &bmpFileHdr, 0, sizeof(BITMAPFILEHEADER) );
		DWORD dwLenAllocPart = 0L;
		if( dwResourceSize == 0L )
		{
			ar.Read( (LPVOID)&bmpFileHdr, sizeof(BITMAPFILEHEADER) );
			dwLenAllocPart = bmpFileHdr.bfSize - sizeof(BITMAPFILEHEADER);
			if( dwLenAllocPart == 0 )
			{
				ASSERT( FALSE );
				return false;
			}
		} // if( dwResourceSize == 0L )
		else
			dwLenAllocPart = dwResourceSize;
		HGLOBAL hGlobalDst =
			::GlobalAlloc(
				GMEM_FIXED,
				dwLenAllocPart
				);
		if( hGlobalDst == NULL )
		{
			ASSERT( FALSE );
			return false;
		}
		ar.Read( (LPVOID)hGlobalDst, dwLenAllocPart );
		LPVOID pBits = NULL;
		HDC hDC = ::CreateCompatibleDC( NULL );
		(*p_hBmp) =
			::CreateDIBSection(
				hDC,
				(LPBITMAPINFO)hGlobalDst,
				DIB_RGB_COLORS,
				&pBits,
				NULL,
				0
				);
		::DeleteDC( hDC );
		if( (*p_hBmp) == NULL || pBits == NULL )
		{
			ASSERT( FALSE );
			(*p_hBmp) = NULL;
			return false;
		}
		DWORD sizeof_BITMAPINFOHEADER =
			((LPBITMAPINFOHEADER)hGlobalDst)->biSize;
		if( sizeof_BITMAPINFOHEADER < sizeof(BITMAPINFOHEADER) )
		{
			ASSERT( FALSE );
			::DeleteObject( (*p_hBmp) );
			::GlobalFree( hGlobalDst );
			return false;
		}
		::memcpy(
			pBits,
			((LPBYTE)hGlobalDst)
				+ (	( dwResourceSize == 0L )
						? (bmpFileHdr.bfOffBits - sizeof(BITMAPFILEHEADER))
						: (sizeof_BITMAPINFOHEADER
							+ (
									( ((LPBITMAPINFOHEADER)hGlobalDst)->biBitCount > 8 )
										? 0L
										: ( 1L << ((LPBITMAPINFOHEADER)hGlobalDst)->biBitCount )
								) * sizeof(RGBQUAD)
							)
					)
				,
			dwLenAllocPart
				- sizeof_BITMAPINFOHEADER
				- (
						( ((LPBITMAPINFOHEADER)hGlobalDst)->biBitCount > 8 )
							? 0L
							: ( 1L << ((LPBITMAPINFOHEADER)hGlobalDst)->biBitCount )
					) * sizeof(RGBQUAD)

			);
		::GlobalFree( hGlobalDst );
	} // else from if( ar.IsStoring() )
	return true;
}

bool CExtPaintManager::stat_SerializeBitmap(
	__EXT_MFC_SAFE_LPCTSTR strFileName,
	bool bRead,
	HBITMAP * p_hBmp
	)
{
	ASSERT( strFileName != NULL );
	if( strFileName == NULL )
		return false;
bool bRetVal = true;
	try
	{
		CFile _file(
			strFileName,
			bRead
				? (CFile::modeRead | CFile::typeBinary)
				: (CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)
			);
		CArchive ar(
			&_file,
			bRead ? CArchive::load : CArchive::store
			);
		bRetVal = stat_SerializeBitmap( ar, p_hBmp );
		ASSERT( bRetVal );
		if( !bRead )
			ar.Flush();
		ar.Close();
		_file.Close();
	} // try
	catch( CException * pXept )
	{
		ASSERT( FALSE );
		pXept->Delete();
		return false;
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
		return false;
	} // catch( ... )
	return bRetVal;
}

HBITMAP CExtPaintManager::stat_LoadBitmapResource(
	HINSTANCE hInst,
	HRSRC hRsrc
	)
{
	if( hInst == NULL || hRsrc == NULL )
		return NULL;
HGLOBAL hGlobal = ::LoadResource( hInst, hRsrc );
	if( hGlobal == NULL )
		return NULL;
LPBYTE pData = (LPBYTE)::LockResource( hGlobal );
	if( pData == NULL )
	{
		::FreeResource( hGlobal );
		return NULL;
	} // if( pData == NULL )
DWORD dwResourceSize = ::SizeofResource( hInst, hRsrc );
	ASSERT( dwResourceSize > 0 );
CMemFile _file;
	_file.Attach( pData, dwResourceSize );
	_file.Seek( 0, CFile::begin );
CArchive ar( &_file, CArchive::load );
HBITMAP hBmp = NULL;
bool bSerializationPassed =
		stat_SerializeBitmap( ar, &hBmp, dwResourceSize );
	ASSERT( bSerializationPassed );
	ar.Close();
	_file.Detach();
	::UnlockResource( hGlobal );
	::FreeResource( hGlobal );
	if( !bSerializationPassed )
		return NULL;
	ASSERT( hBmp != NULL );
	return hBmp;
}

HBITMAP CExtPaintManager::stat_LoadBitmapResource(
	__EXT_MFC_SAFE_LPCTSTR sResourceID,
	__EXT_MFC_SAFE_LPCTSTR sResourceType, // = RT_BITMAP, // default bitmap section
	HINSTANCE hInst // = NULL // find it automatically
	)
{
	if( hInst == NULL )
	{
		hInst =
			::AfxFindResourceHandle(
				sResourceID,
				sResourceType
				);
		if( hInst == NULL )
			return NULL;
	} // if( hInst == NULL )
HRSRC hRsrc =
		::FindResource(
			hInst,
			sResourceID,
			sResourceType
			);
	if( hRsrc == NULL )
		return NULL;
	return stat_LoadBitmapResource( hInst, hRsrc );
}

void CExtPaintManager::stat_GetMonitorParms(
	CExtPaintManager::monitor_parms_t & _mp
	)
{
	VERIFY(
		::GetWindowRect(
			::GetDesktopWindow(),
			&_mp.m_rcMonitor
			)
		);
	VERIFY(
		::SystemParametersInfo(
			SPI_GETWORKAREA,
			0,
			PVOID(&_mp.m_rcWorkArea),
			0
			)
		);
	_mp.m_bPrimaryMonitor = true;
}

void CExtPaintManager::stat_GetMonitorParms(
	CExtPaintManager::monitor_parms_t & _mp,
	const POINT & _ptScreenFrom
	)
{
MONITORINFO _MonitorInfo;
	::memset( &_MonitorInfo, 0, sizeof(MONITORINFO) );
	_MonitorInfo.cbSize = sizeof(MONITORINFO);
	if(	!::GetMonitorInfo(
			MonitorFromPoint(
				_ptScreenFrom, 
				MONITOR_DEFAULTTONEAREST
				),
				&_MonitorInfo
			)
		)
	{
		ASSERT( FALSE );
		stat_GetMonitorParms( _mp );
		return;
	}
	_mp.m_rcMonitor = _MonitorInfo.rcMonitor;
	_mp.m_rcWorkArea = _MonitorInfo.rcWork;
	_mp.m_bPrimaryMonitor = 
		( (_MonitorInfo.dwFlags&MONITORINFOF_PRIMARY) != 0 )
			? true
			: false
			;
}

void CExtPaintManager::stat_GetMonitorParms(
	CExtPaintManager::monitor_parms_t & _mp,
	const RECT & _rcScreenFrom
	)
{
MONITORINFO _MonitorInfo;
	::memset( &_MonitorInfo, 0, sizeof(MONITORINFO) );
	_MonitorInfo.cbSize = sizeof(MONITORINFO);
	if(	!::GetMonitorInfo(
			MonitorFromRect(
				&_rcScreenFrom, 
				MONITOR_DEFAULTTONEAREST
				),
				&_MonitorInfo
			)
		)
	{
		ASSERT( FALSE );
		stat_GetMonitorParms( _mp );
		return;
	}
	_mp.m_rcMonitor = _MonitorInfo.rcMonitor;
	_mp.m_rcWorkArea = _MonitorInfo.rcWork;
	_mp.m_bPrimaryMonitor = 
		( (_MonitorInfo.dwFlags&MONITORINFOF_PRIMARY) != 0 )
			? true
			: false
			;
}

void CExtPaintManager::stat_GetMonitorParms(
	monitor_parms_t & _mp,
	HWND hWndFrom
	)
{
	ASSERT( hWndFrom != NULL && ::IsWindow(hWndFrom) );
RECT rcWnd = { 0, 0, 0, 0 };
	VERIFY( ::GetWindowRect( hWndFrom, &rcWnd ) );
	stat_GetMonitorParms( _mp, rcWnd );
}

void CExtPaintManager::stat_GetMonitorParms(
	monitor_parms_t & _mp,
	CWnd * pWndFrom
	)
{
	ASSERT_VALID( pWndFrom );
	stat_GetMonitorParms( _mp, pWndFrom->GetSafeHwnd() );
}

CRect CExtPaintManager::stat_AlignWndRectToMonitor(
	CRect rcInitial,
	bool bUseWorkArea, // = true // if false - use all monitor area
	bool bNoPartialVisibility // = false
	)
{
monitor_parms_t _mp;
	stat_GetMonitorParms( _mp, rcInitial );
CRect rcDesktop = bUseWorkArea ? _mp.m_rcWorkArea : _mp.m_rcMonitor;
	if( rcInitial.left >= rcDesktop.right )
		rcInitial.OffsetRect( rcDesktop.right - rcInitial.right, 0 );
	if( rcInitial.top >= rcDesktop.bottom )
		rcInitial.OffsetRect( 0, rcDesktop.bottom - rcInitial.bottom );
	if( rcInitial.right <= rcDesktop.left )
		rcInitial.OffsetRect( rcDesktop.left - rcInitial.left, 0 );
	if( rcInitial.bottom <= rcDesktop.top )
		rcInitial.OffsetRect( 0, rcDesktop.top - rcInitial.top );
	if( bNoPartialVisibility )
	{
		if( rcInitial.right > rcDesktop.right )
			rcInitial.OffsetRect( rcDesktop.right - rcInitial.right, 0 );
		if( rcInitial.bottom > rcDesktop.bottom )
			rcInitial.OffsetRect( 0, rcDesktop.bottom - rcInitial.bottom );
		if( rcInitial.left < rcDesktop.left )
			rcInitial.OffsetRect( rcDesktop.left - rcInitial.left, 0 );
		if( rcInitial.top < rcDesktop.top )
			rcInitial.OffsetRect( 0, rcDesktop.top - rcInitial.top );
	} // if( bNoPartialVisibility )
	return rcInitial;
}


COLORREF CExtPaintManager::stat_RGBtoCMY( COLORREF clr )
{
int nR = GetRValue( clr );
int nG = GetGValue( clr );
int nB = GetBValue( clr );
int nC = 255-nR;
int nM = 255-nG;
int nY = 255-nB;
COLORREF clrCMY = RGB( nC, nM, nY );
	return clrCMY;
}

COLORREF CExtPaintManager::stat_CMYtoRGB( COLORREF clr )
{
int nC = GetRValue( clr );
int nM = GetGValue( clr );
int nY = GetBValue( clr );
int nR = 255 - nC;
int nG = 255 - nM;
int nB = 255 - nY;
COLORREF clrRGB = RGB( nR, nG, nB );
	return clrRGB;
}

//----------------------------------------------------------------------
// Conversion between the HSL (Hue, Saturation, and Luminosity) 
// and RBG color model.
//----------------------------------------------------------------------
// The conversion algorithms presented here come from the book by 
// Fundamentals of Interactive Computer Graphics by Foley and van Dam. 
// In the example code, HSL values are represented as floating point 
// number in the range 0 to 1. RGB tridrants use the Windows convention 
// of 0 to 255 of each element. 
//----------------------------------------------------------------------
double CExtPaintManager::stat_HuetoRGB(double m1, double m2, double h )
{
   if( h < 0 ) h += 1.0;
   if( h > 1 ) h -= 1.0;
   if( 6.0*h < 1 )
      return (m1+(m2-m1)*h*6.0);
   if( 2.0*h < 1 )
      return m2;
   if( 3.0*h < 2.0 )
      return (m1+(m2-m1)*((2.0/3.0)-h)*6.0);
   return m1;
}

COLORREF CExtPaintManager::stat_HLStoRGB( double H, double L, double S )
{
double r,g,b;
double m1, m2;
	if( S==0 )
	{
		r = g = b = L;
	}
	else
	{
		if( L <= 0.5 )
			m2 = L*(1.0+S);
		else
			m2 = L+S-L*S;
		m1 = 2.0*L-m2;
		r = stat_HuetoRGB(m1,m2,H+1.0/3.0);
		g = stat_HuetoRGB(m1,m2,H);
		b = stat_HuetoRGB(m1,m2,H-1.0/3.0);
	}
	return RGB((BYTE)(r*255),(BYTE)(g*255),(BYTE)(b*255));
}

void CExtPaintManager::stat_RGBtoHSL( COLORREF rgb, double *H, double *S, double *L )
{   
double delta;
double r = (double)GetRValue(rgb)/255;
double g = (double)GetGValue(rgb)/255;
double b = (double)GetBValue(rgb)/255;   
double cmax = max(r,max(g,b));
double cmin = min(r,min(g,b));   
	*L = (cmax + cmin) / 2.0;   
	
	if(cmax==cmin) 
	{
		*S = 0;      
		*H = 0; // it's really undefined   
	}
	else 
	{
		if( *L < 0.5 ) 
			*S = (cmax-cmin)/(cmax+cmin);      
		else
			*S = (cmax-cmin)/(2.0-cmax-cmin);      
		delta = cmax - cmin;
		if( r == cmax ) 
			*H = (g-b)/delta;      
		else if( g == cmax )
			*H = 2.0 +(b-r)/delta;
		else          
			*H = 4.0 + (r-g)/delta;
		*H /= 6.0; 
		if( *H < 0.0 )
			*H += 1;  
	}
}

BYTE CExtPaintManager::stat_GetRGBFromHue(float rm1, float rm2, float rh)
{
	if (rh > 360.0f)
		rh -= 360.0f;
	else if (rh < 0.0f)
		rh += 360.0f;
	if (rh <  60.0f)
		rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
	else if (rh < 180.0f)
		rm1 = rm2;
	else if (rh < 240.0f)
		rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      
	return static_cast<BYTE>(rm1 * 255);
}

COLORREF CExtPaintManager::stat_GetRGBFromHLSExtend( double H, double L, double S )
{
WORD R, G, B;
	if( S == 0.0 )
	{
		R = G = B = unsigned char(L * 255.0);
	}
	else
	{
		float rm1, rm2;
		
		if( L <= 0.5f )
			rm2 = (float)(L + L * S);
		else
			rm2 = (float)(L + S - L * S);
		
		rm1 = (float)(2.0f * L - rm2);
		R = stat_GetRGBFromHue(rm1, rm2, (float)(H + 120.0f));
		G = stat_GetRGBFromHue(rm1, rm2, (float)(H));
		B = stat_GetRGBFromHue(rm1, rm2, (float)(H - 120.0f));
	}
	return RGB(R, G, B);
}

COLORREF CExtPaintManager::stat_HLS_Adjust(
	COLORREF clr,
	double percentH, // = 0.0
	double percentL, // = 0.0
	double percentS  // = 0.0
	)
{
double H = 0.0, L = 0.0, S = 0.0;
	stat_RGBtoHSL( clr, &H, &S, &L );

    if( percentH > 0.0 )
        H =
			H +
			(1.0 - H) * percentH
			;
    else if ( percentH < 0.0 )
        H =
			H * (1.0 + percentH);
	if( H < 0.0 )
		H = 0.0;
	else if( H > 1.0 )
		H = 1.0;

    if( percentL > 0.0 )
        L =
			L +
			(1.0 - L) * percentL
			;
    else if ( percentL < 0.0 )
        L =
			L * (1.0 + percentL);
	if( L < 0.0 )
		L = 0.0;
	else if( L > 1.0 )
		L = 1.0;

    if ( percentS > 0.0 )
        S =
			S +
			(1.0 - S) * percentS
			;
    else if ( percentS < 0.0 )
        S =
			S * (1.0 + percentS);
	if( S < 0.0 )
		S = 0.0;
	else if( S > 1.0 )
		S = 1.0;

    return stat_HLStoRGB( H, L, S );
}

BOOL CExtPaintManager::stat_PaintParentBk(
	HWND hWnd,
	HDC hDC,
	LPCRECT rectClip // = NULL
	)
{
	ASSERT( hDC != NULL );
	ASSERT( ::IsWindow(hWnd) );
	if( rectClip != NULL )
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect( rectClip );
		::SelectClipRgn( hDC, (HRGN)rgn.GetSafeHandle() );
	}
HWND hWndParent = ::GetParent( hWnd );
	ASSERT( ::IsWindow(hWndParent) );
CPoint pt( 0, 0 );
	::MapWindowPoints(hWnd,hWndParent,&pt,1);
	VERIFY(
		::OffsetWindowOrgEx(
			hDC,
			pt.x,
			pt.y,
			&pt
			)
		);
LRESULT lRes =
		::SendMessage(
			hWndParent,
			WM_ERASEBKGND,
			(WPARAM)hDC,
			0
			);
	VERIFY(
		::SetWindowOrgEx(
			hDC,
			pt.x,
			pt.y,
			&pt
			)
		);
	::SelectClipRgn( hDC, NULL );
	return (BOOL)lRes;
}

bool CExtPaintManager::stat_DefIsHwndNeedsDirectRepaint(
	HWND hWndChild
	)
{
DWORD dwChildStyle =
		DWORD( GetWindowLong(hWndChild,GWL_STYLE) );
	if( (dwChildStyle & WS_CHILD) == 0 )
		return true;
CExtSafeString sClassName;
	::GetClassName(
		hWndChild,
		sClassName.GetBuffer( _MAX_PATH+1 ),
		_MAX_PATH
		);
	sClassName.ReleaseBuffer();
	sClassName.MakeLower();
	if( sClassName == LPCTSTR( _T("static") ) )
		return true;
	if( sClassName == LPCTSTR( _T("systabcontrol32") ) )
		return true;
	if( sClassName == LPCTSTR( _T("msctls_trackbar32") ) )
		return true;
	if(		sClassName == LPCTSTR( _T("button") )
		&&	(	( dwChildStyle & (BS_GROUPBOX|BS_CHECKBOX) ) != 0
			||	g_PaintManager.m_bUxApiInitPassed
			)
		)
		return true;
	return false;
}

bool CExtPaintManager::stat_DefExcludeChildAreaCallback(
	HDC hDC,
	HWND hWnd,
	HWND hWndChild,
	LPVOID pCookie
	)
{
	ASSERT( hDC != NULL );
	ASSERT( hWnd != NULL );
	ASSERT( IsWindow(hWnd) );
	ASSERT( hWndChild != NULL );
	ASSERT( IsWindow(hWndChild) );
	hDC;
	hWnd;
	hWndChild;
	pCookie;
	return stat_DefIsHwndNeedsDirectRepaint( hWndChild );
}

int CExtPaintManager::stat_GetBPP()
{
//	return 4; // <-- test
//	return 8; // <-- test
CWindowDC dc_desktop(NULL);
int nBitsPerPixel =
		dc_desktop.GetDeviceCaps(BITSPIXEL);
	return nBitsPerPixel;
}

CSize CExtPaintManager::GetTextSizes(bool bBold /*= false*/)
{
CWindowDC dc (NULL);
CFont * pOldFont =
		dc.SelectObject(
			bBold ? &m_FontBold : &m_FontNormal
			);
	ASSERT( pOldFont != NULL );
TEXTMETRIC tm;
	dc.GetTextMetrics( &tm );
int nTextHeightHorz = tm.tmHeight + 2;
	dc.SelectObject( &m_FontNormal );
	dc.GetTextMetrics( &tm );
int nTextHeightVert = tm.tmHeight + 2;
	dc.SelectObject( pOldFont );
	return CSize(nTextHeightHorz,nTextHeightVert);
}

int CExtPaintManager::GetTextHeight(bool bHorz)
{
CSize _size = GetTextSizes();
	return bHorz ? _size.cx : _size.cy;
}

static const UINT stat_pixels_glyph_btn_expand_bottom[] =
{
	1,1,1,1,1,
	0,1,1,1,0,
	0,0,1,0,0,
};

static const UINT stat_pixels_glyph_btn_expand_bottom_2003[] =
{
	1,1,1,1,1,
	0,0,0,0,0,
	0,0,0,0,0,
	1,1,1,1,1,
	0,1,1,1,0,
	0,0,1,0,0,
};

const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_bottom(
		5,3,2,
		stat_pixels_glyph_btn_expand_bottom
		);
const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_bottom_2003(
		5,6,2,
		stat_pixels_glyph_btn_expand_bottom_2003
		);

static const UINT stat_pixels_glyph_btn_expand_right2[] =
{
	1,1,0,0,1,1,0,0,
	0,1,1,0,0,1,1,0,
	0,0,1,1,0,0,1,1,
	0,1,1,0,0,1,1,0,
	1,1,0,0,1,1,0,0,
};
static const UINT stat_pixels_glyph_btn_expand_right2_2003[] =
{
	0,0,0,0,0,0,
	1,0,0,0,1,0,
	1,1,0,0,1,1,
	1,0,0,0,1,0,
	0,0,0,0,0,0,
};

const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_right2(
		8,5,2,
		stat_pixels_glyph_btn_expand_right2
		);
const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_right2_2003(
		6,5,2,
		stat_pixels_glyph_btn_expand_right2_2003
		);

const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_left(
		CExtPaintManager::g_glyph_btn_expand_bottom,
		90
		);

const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_left_2003(
		CExtPaintManager::g_glyph_btn_expand_bottom_2003,
		90
		);

const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_right(
		CExtPaintManager::g_glyph_btn_expand_bottom,
		270
		);
const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_right_2003(
		CExtPaintManager::g_glyph_btn_expand_bottom_2003,
		270
		);

const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_bottom2(
		CExtPaintManager::g_glyph_btn_expand_right2,
		90
		);
const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_bottom2_2003(
		CExtPaintManager::g_glyph_btn_expand_right2_2003,
		90
		);

static const UINT stat_pixels_glyph_customize_popup_src[] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,1,0,0,0,
	0,0,0,0,0,0,1,1,1,1,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,
	0,0,0,0,0,0,1,1,1,1,1,1,
	0,0,0,0,0,0,1,1,1,1,1,0,
	0,0,0,0,0,0,1,1,1,1,0,0,
	0,0,0,0,0,0,1,1,1,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
};
const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_customize_popup_src(
		12,16,2,
		stat_pixels_glyph_customize_popup_src
		);
static const UINT stat_pixels_glyph_customize_dropdown_src[] =
{
	1,0,0,0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,1,0,0,0,0,0,
	1,0,0,0,0,0,1,1,0,0,0,0,
	1,0,0,0,0,0,1,1,1,0,0,0,
	1,0,0,0,0,0,1,1,1,1,0,0,
	1,0,0,0,0,0,1,1,1,1,1,0,
	1,0,0,0,0,0,1,1,1,1,1,1,
	1,0,0,0,0,0,1,1,1,1,1,0,
	1,0,0,0,0,0,1,1,1,1,0,0,
	1,0,0,0,0,0,1,1,1,0,0,0,
	1,0,0,0,0,0,1,1,0,0,0,0,
	1,0,0,0,0,0,1,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0,0,0,0,
};
const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_customize_dropdown_src(
		12,16,2,
		stat_pixels_glyph_customize_dropdown_src
		);

void CExtPaintManager::PaintGlyph(
	CDC & dc,
	POINT ptDest,
	const glyph_t & glyph,
	COLORREF * pColorValues
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( pColorValues != NULL );
	ASSERT( glyph.Size().cx > 0 );
	ASSERT( glyph.Size().cy > 0 );
CRect rcTestVisiblity(ptDest,glyph.Size());
	if( !dc.RectVisible(&rcTestVisiblity) )
		return;
int x_dest = ptDest.x + glyph.Size().cx;
int y_dest = ptDest.y + glyph.Size().cy;
const UINT * pColorIndex = glyph.GetSurface();
	ASSERT( pColorIndex != NULL );
	for( int y = ptDest.y; y < y_dest; y++ )
	{
		for( int x = ptDest.x; x < x_dest; x++, pColorIndex++ )
		{
			UINT nColorIndex = *pColorIndex;
			ASSERT( nColorIndex < glyph.GetColorsCount() );
			if( nColorIndex == 0 )
				continue;
			COLORREF clr = pColorValues[nColorIndex];
			dc.SetPixel( x, y, clr );
		}
	}
}

void CExtPaintManager::PaintGlyphCentered(
	CDC & dc,
	const RECT & rect,
	const glyph_t & glyph,
	COLORREF * pColorValues
	)
{
CRect _rect(rect);
	_rect.NormalizeRect();
	if( _rect.IsRectEmpty() )
		return;
	ASSERT( glyph.Size().cx > 0 );
	ASSERT( glyph.Size().cy > 0 );
CPoint ptDest = _rect.TopLeft();
	ptDest.x +=
		(_rect.Width() - glyph.Size().cx) / 2;
	ptDest.y +=
		(_rect.Height() - glyph.Size().cy) / 2;
	PaintGlyph(
		dc,
		ptDest,
		glyph,
		pColorValues
		);
}

int CExtPaintManager::GetMenuExpandButtonHeight()
{
int nHeight = ::GetSystemMetrics(SM_CYMENUCHECK);
	ASSERT( nHeight > 0 );
	return nHeight;
}

int CExtPaintManagerXP::GetMenuExpandButtonHeight()
{
	return CExtPaintManager::GetMenuExpandButtonHeight();
}

int CExtPaintManagerOffice2003::GetMenuExpandButtonHeight()
{
int nHeightNormal = CExtPaintManager::GetMenuExpandButtonHeight();
	ASSERT( nHeightNormal > 0 );
	return max( nHeightNormal, (m_sizeMenuExpBtnCircle.cy+2) );
}

void CExtPaintManager::PaintMenuExpandButton(
	CDC & dc,
	const RECT & rectButton,
	bool bPushed,
	bool bHover,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
	bHover;
CRect rcGlyph(rectButton);
	if( bPushed )
	{
		dc.FillRect(
			&rcGlyph,
			&m_brushLightDefault
			);
		dc.Draw3dRect(
			&rcGlyph,
			GetColor(CLR_3DHILIGHT_OUT),
			GetColor(CLR_3DSHADOW_OUT)
			);
	}
	else
	{
		CExtPaintManager::PAINTMENUITEMDATA _pmid(
			this,
			rectButton, 0, _T(""),
			_T(""), NULL,
			false, false, false,
			false, false, true, false, false, false,
			0
			);
		PaintMenuItem( dc, _pmid );
	}
COLORREF ColorValues[] =
{
	RGB(0,0,0),
	GetColor(CLR_TEXT_OUT)
};
	PaintGlyphCentered(
		dc,
		rcGlyph,
		g_glyph_btn_expand_bottom2,
		ColorValues
		);
}

void CExtPaintManagerXP::PaintMenuExpandButton(
	CDC & dc,
	const RECT & rectButton,
	bool bPushed,
	bool bHover,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
	bHover;
int nIconAreaWidth = 0;
	if(		pHelperSrc != NULL // (+ 2.23)
		&&	pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) )
		&&	((CExtPopupMenuWnd*)pHelperSrc) ->
				ItemGetCount() > 0
		)
	{
		CExtPopupMenuWnd::MENUITEMDATA & _mii =
			((CExtPopupMenuWnd*)pHelperSrc) ->
				ItemGetInfo(
					((CExtPopupMenuWnd*)pHelperSrc) ->
						ItemGetCount()
					- 1
					);
		if( !_mii.IsToolButton() )
			nIconAreaWidth = _mii.GetIconAreaWidth();
	}
	if( !bPushed )
	{
		CRect rc(rectButton);
		rc.top--;
		CExtPaintManager::PAINTMENUITEMDATA _pmid(
			this,
			rc, nIconAreaWidth, _T(""),
			_T(""), NULL,
			false, false, false,
			false, false, true, false, false, false,
			0
			);
		PaintMenuItem( dc, _pmid );
	} // if( !bPushed )
CRect rcGlyph(rectButton);
	if( bPushed )
	{
		PAINTPUSHBUTTONDATA _ppbd(
			this,
			true,rectButton,_T(""),NULL,
			true,false,bPushed,false,
			false,true,false,false,
			__ALIGN_HORIZ_CENTER|__ALIGN_VERT,
			NULL,
			false
			);
		PaintPushButton( dc, _ppbd );
		rcGlyph.OffsetRect(
			GetPushedOffset()
			);
	}
	else
	{
		CExtPaintManager::PAINTMENUITEMDATA _pmid(
			this,
			rectButton, nIconAreaWidth, _T(""),
			_T(""), NULL,
			false, false, false,
			false, false, true, false, false, false,
			0
			);
		PaintMenuItem( dc, _pmid );
	}
COLORREF ColorValues[] =
{
	RGB(0,0,0),
	GetColor(CLR_TEXT_OUT)
};
	PaintGlyphCentered(
		dc,
		rcGlyph,
		g_glyph_btn_expand_bottom2,
		ColorValues
		);
}

void CExtPaintManagerOffice2003::PaintMenuExpandButton(
	CDC & dc,
	const RECT & rectButton,
	bool bPushed,
	bool bHover,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT( m_sizeMenuExpBtnCircle.cx > 0 );
	ASSERT( m_sizeMenuExpBtnCircle.cy > 0 );
CRect rcButton( rectButton ); // (+ 2.23)
	if( ( rcButton.Width() & 0x01 ) != 0 ) // (+ 2.23)
		rcButton.right --; // (+ 2.23)
int nIconAreaWidth = 0;
	if(		pHelperSrc != NULL // (+ 2.23)
		&&	pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) )
		&&	((CExtPopupMenuWnd*)pHelperSrc) ->
				ItemGetCount() > 0
		)
	{
		CExtPopupMenuWnd::MENUITEMDATA & _mii =
			((CExtPopupMenuWnd*)pHelperSrc) ->
				ItemGetInfo(
					((CExtPopupMenuWnd*)pHelperSrc) ->
						ItemGetCount()
					- 1
					);
		if( !_mii.IsToolButton() )
			nIconAreaWidth = _mii.GetIconAreaWidth();
	}
CPoint ptRgnEffectiveOffset(
		( rcButton.right - rcButton.left - m_sizeMenuExpBtnCircle.cx ) / 2
			+ rcButton.left,
		( rcButton.bottom - rcButton.top - m_sizeMenuExpBtnCircle.cy ) / 2
			+ rcButton.top
		);
CRgn rgnEffective;
	if(		m_rgnMenuExpBtnCircle.GetSafeHandle() == NULL
		||	m_bmpCtBarGradientHorz.GetSafeHandle() == NULL
		||	m_bmpCtBarGradientVert.GetSafeHandle() == NULL
		||	m_bmpDockerGradient.GetSafeHandle() == NULL
		||	stat_GetBPP() <= 8
		||	(! rgnEffective.CreateRectRgn(0,0,1,1) )
		||	rgnEffective.CopyRgn( &m_rgnMenuExpBtnCircle ) == ERROR
		||	rgnEffective.OffsetRgn(ptRgnEffectiveOffset) == ERROR
		)
	{
		CExtPaintManagerXP::PaintMenuExpandButton(
			dc,
			rectButton,
			bPushed,
			bHover,
			pHelperSrc,
			lParam
			);
		return;
	}
	ASSERT( rgnEffective.GetSafeHandle() != NULL );

	if( !bPushed )
	{
		CRect rc(rcButton);
		rc.top--;
		CExtPaintManager::PAINTMENUITEMDATA _pmid(
			this,
			rc, nIconAreaWidth, _T(""),
			_T(""), NULL,
			false, false, false,
			false, false, true, false, false, false,
			0
			);
		PaintMenuItem( dc, _pmid );
	}
CRect rcGlyph( rcButton );
	if( bPushed )
	{
		PAINTPUSHBUTTONDATA _ppbd(
			this,
			true,rectButton,_T(""),NULL,
			true,false,bPushed,false,
			false,true,false,false,
			__ALIGN_HORIZ_CENTER|__ALIGN_VERT,
			NULL,
			false
			);
		PaintPushButton( dc, _ppbd );
		rcGlyph.OffsetRect(
			GetPushedOffset()
			);
	}
	else
	{
		CExtPaintManager::PAINTMENUITEMDATA _pmid(
			this,
			rectButton, nIconAreaWidth, _T(""),
			_T(""), NULL,
			false, false, false,
			false, false, true, false, false, false,
			0
			);
		PaintMenuItem( dc, _pmid );
	}

CBrush _brBaloon( GetColor(_2003CLR_MENUEXPBALOON) );
	ASSERT( _brBaloon.GetSafeHandle() != NULL );
	::FillRgn( dc, rgnEffective, (HBRUSH)_brBaloon.GetSafeHandle() );

COLORREF ColorValues[] =
{
	RGB(0,0,0),
	GetColor(CLR_TEXT_OUT)
};
	PaintGlyphCentered(
		dc,
		rcGlyph,
		g_glyph_btn_expand_bottom2,
		ColorValues
		);
}

void CExtPaintManager::PaintToolbarExpandButton(
	CDC & dc,
	const RECT & rcButtonArea,
	bool bHorz, // if false - down
	bool bBarIsCompletelyVisible,
	bool bEnabled,
	bool bPushed,
	bool bHover,
	CObject * pHelperSrc,
	LPARAM lParam, // = 0L
	bool bTransparentBackground // = false
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;
CRect rect(rcButtonArea);
	rect.NormalizeRect();
	if( !dc.RectVisible( &rect ) )
		return;
CRect rectButton(rect);

//	ASSERT( bLeft ); // temporary
const glyph_t * pGlyph = NULL, * pGlyph2 = NULL;
	if( bHorz )
	{
		pGlyph = &g_glyph_btn_expand_bottom;
		if( !bBarIsCompletelyVisible )
			pGlyph2 = &g_glyph_btn_expand_right2;
	}
	else
	{
		pGlyph = &g_glyph_btn_expand_left;
		if( !bBarIsCompletelyVisible )
			pGlyph2 = &g_glyph_btn_expand_bottom2;
	}
	ASSERT( pGlyph != NULL );

PAINTPUSHBUTTONDATA _ppbd(
		this,
		true,rect,_T(""),NULL,true,
		bHover,bPushed,false,bEnabled,
		true,false,false,
		__ALIGN_HORIZ_CENTER|__ALIGN_VERT,
		NULL,false,0,bTransparentBackground
		);
	PaintPushButton( dc, _ppbd );

COLORREF clr =
	GetColor(
		bEnabled? CLR_TEXT_OUT : COLOR_3DHILIGHT
		);
COLORREF ColorValues[] =
{
	0,
	clr
};
	if( bPushed )
		rect.OffsetRect(
			GetPushedOffset()
			);

CRect rectGlyph(rectButton.TopLeft(),pGlyph->Size());
CRect rectGlyph2(rectGlyph);
CSize sizePushedOffset = GetPushedOffset();
	if( bPushed )
	{
		rectGlyph.OffsetRect( sizePushedOffset );
		rectGlyph2.OffsetRect( sizePushedOffset );
	}

	if( bHorz )
	{
		int nGap = (max(sizePushedOffset.cy,1)) * 3;
		rectGlyph.OffsetRect(
			(rectButton.Size().cx - pGlyph->Size().cx) / 2,
			rectButton.Size().cy - pGlyph->Size().cy - nGap
			);
		if( !bBarIsCompletelyVisible )
		{
			ASSERT( pGlyph2 != NULL );
			rectGlyph2.OffsetRect(
				(rectButton.Size().cx - pGlyph2->Size().cx) / 2,
				nGap
				);
		}
	}
	else
	{
		int nGap = (max(sizePushedOffset.cx,1)) * 3;
		rectGlyph.OffsetRect(
			nGap,
			(rectButton.Size().cy - pGlyph->Size().cy)/2
			);
		if( !bBarIsCompletelyVisible )
		{
			ASSERT( pGlyph2 != NULL );
			rectGlyph2.OffsetRect(
				rectButton.Size().cx - pGlyph2->Size().cx - nGap,
				(rectButton.Size().cy - pGlyph2->Size().cy) / 2
				);
		}
	}

	if( bEnabled )
	{
		PaintGlyph(
			dc,rectGlyph.TopLeft(),*pGlyph,ColorValues
			);
		if( !bBarIsCompletelyVisible )
		{
			ASSERT( pGlyph2 != NULL );
			PaintGlyph(
				dc,rectGlyph2.TopLeft(),*pGlyph2,ColorValues
				);
		}
	} // if( bEnabled )
	else
	{
//		rect.OffsetRect(1,1);
//		PaintGlyphCentered(
//			dc,rect,*pGlyph,ColorValues
//			);
//		ColorValues[1] = GetColor(COLOR_3DSHADOW);
//		rect.OffsetRect(-1,-1);
//		PaintGlyphCentered(
//			dc,rect,*pGlyph,ColorValues
//			);
		rectGlyph.OffsetRect(1,1);
		PaintGlyph(
			dc,rectGlyph.TopLeft(),*pGlyph,ColorValues
			);
		rectGlyph.OffsetRect(-1,-1);
		ColorValues[1] = GetColor(COLOR_3DSHADOW);
		PaintGlyph(
			dc,rectGlyph.TopLeft(),*pGlyph,ColorValues
			);
	} // else from if( bEnabled )
}

void CExtPaintManagerXP::PaintToolbarExpandButton(
	CDC & dc,
	const RECT & rcButtonArea,
	bool bHorz, // if false - down
	bool bBarIsCompletelyVisible,
	bool bEnabled,
	bool bPushed,
	bool bHover,
	CObject * pHelperSrc,
	LPARAM lParam, // = 0L
	bool bTransparentBackground // = false
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;
CRect rect(rcButtonArea);
	rect.NormalizeRect();
	if( !dc.RectVisible( &rect ) )
		return;
CRect rectButton(rect);

const glyph_t * pGlyph = NULL, * pGlyph2 = NULL;
	if( bHorz )
	{
		pGlyph = m_pGlyphTbEpBtnH0;
		pGlyph2 = m_pGlyphTbEpBtnH1;
	} // if( bHorz )
	else
	{
		pGlyph = m_pGlyphTbEpBtnV0;
		pGlyph2 = m_pGlyphTbEpBtnV1;
	} // else from if( bHorz )
	ASSERT( pGlyph != NULL );
	ASSERT( pGlyph2 != NULL );

PAINTPUSHBUTTONDATA _ppbd(
		this,
		true,rect,_T(""),NULL,true,
		bHover,bPushed,false,bEnabled,
		true,false,false,
		__ALIGN_HORIZ_CENTER|__ALIGN_VERT,
		NULL,false,0,bTransparentBackground
		);
	PaintPushButton( dc, _ppbd );

COLORREF clr =
	GetColor(
		bEnabled? CLR_TEXT_OUT : COLOR_3DHILIGHT
		);
COLORREF ColorValues[] =
{
	0,
	clr
};
	if( bPushed )
		rect.OffsetRect( GetPushedOffset() );
CRect rectGlyph(rectButton.TopLeft(),pGlyph->Size());
CRect rectGlyph2(rectGlyph);
CSize sizePushedOffset = GetPushedOffset();
	if( bPushed )
	{
		rectGlyph.OffsetRect( sizePushedOffset );
		rectGlyph2.OffsetRect( sizePushedOffset );
	} // if( bPushed )

	if( bHorz )
	{
		int nGap = (max(sizePushedOffset.cy,1)) * 3;
		rectGlyph.OffsetRect(
			(rectButton.Size().cx - pGlyph->Size().cx) / 2, // + 1
			rectButton.Size().cy - pGlyph->Size().cy - nGap
			);
		rectGlyph2.OffsetRect(
			(rectButton.Size().cx - pGlyph2->Size().cx) / 2 + 1,
			nGap
			);
	} // if( bHorz )
	else
	{
		int nGap = (max(sizePushedOffset.cx,1)) * 3;
		rectGlyph.OffsetRect(
			nGap,
			(rectButton.Size().cy - pGlyph->Size().cy)/2 // + 1
			);
		rectGlyph2.OffsetRect(
			rectButton.Size().cx - pGlyph2->Size().cx - nGap,
			(rectButton.Size().cy - pGlyph2->Size().cy) / 2 + 1
			);
		if( m_bExpBtnSwapVGlyphs )
		{
			CRect rcTmp( rectGlyph );
			rectGlyph = rectGlyph2;
			rectGlyph2 = rcTmp;
		}
	} // else from if( bHorz )

	if( bEnabled )
	{
//		if( bHover && (!bPushed) )
//		{
//			COLORREF ColorValuesHover[] =
//			{
//				0,
//				GetColor(COLOR_3DDKSHADOW)
//			};
//			rectGlyph.OffsetRect(-1,-1);
//			rectGlyph2.OffsetRect(-1,-1);
//			PaintGlyph(
//				dc,rectGlyph.TopLeft(),*pGlyph,ColorValuesHover
//				);
//			if( !bBarIsCompletelyVisible )
//				PaintGlyph(
//					dc,rectGlyph2.TopLeft(),*pGlyph2,ColorValuesHover
//					);
//			rectGlyph.OffsetRect(1,1);
//			rectGlyph2.OffsetRect(1,1);
//		}
		PaintGlyph(
			dc,rectGlyph.TopLeft(),*pGlyph,ColorValues
			);
		if( !bBarIsCompletelyVisible )
			PaintGlyph(
				dc,rectGlyph2.TopLeft(),*pGlyph2,ColorValues
				);
	} // if( bEnabled )
	else
	{
		ColorValues[1] = GetColor(COLOR_3DSHADOW);
		PaintGlyph(
			dc,rectGlyph.TopLeft(),*pGlyph,ColorValues
			);
	} // else from if( bEnabled )
}

void CExtPaintManagerOffice2003::PaintToolbarExpandButton(
	CDC & dc,
	const RECT & rcButtonArea,
	bool bHorz, // if false - down
	bool bBarIsCompletelyVisible,
	bool bEnabled,
	bool bPushed,
	bool bHover,
	CObject * pHelperSrc,
	LPARAM lParam, // = 0L
	bool bTransparentBackground // = false
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;
CRect rect(rcButtonArea);
	rect.NormalizeRect();
	if( !dc.RectVisible( &rect ) )
		return;

CExtToolControlBar * pToolBar = NULL;
bool bStyleOffice2003 = true;
	if(		pHelperSrc == NULL
		||	(! bEnabled)
		||	(! pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
		||	(! GetCb2DbTransparentMode(pHelperSrc) )
		||	stat_GetBPP() <= 8
		)
	{
		bStyleOffice2003 = false;
	}
	else
	{
		pToolBar = ((CExtBarContentExpandButton*)pHelperSrc)->GetBar();
		ASSERT_VALID( pToolBar );
		if(		pToolBar->m_bPaletteMode
			||	pToolBar->m_pDockSite == NULL
			||	pToolBar->IsFloating()
			||	pToolBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar))
			||	pToolBar->IsKindOf(RUNTIME_CLASS(CExtPanelControlBar))
			||	pToolBar->m_bPresubclassDialogMode
			||	pToolBar->m_pDockSite == NULL
			)
			bStyleOffice2003 = false;
	}
	if( !bStyleOffice2003 )
	{
		CExtPaintManagerXP::PaintToolbarExpandButton(
			dc,
			rcButtonArea,
			bHorz,
			bBarIsCompletelyVisible,
			bEnabled,
			bPushed,
			bHover,
			pHelperSrc,
			lParam,
			bTransparentBackground
			);
		return;
	}
	ASSERT_VALID( pToolBar );
CRect rcToolbarClientArea;
	pToolBar->GetClientRect( &rcToolbarClientArea );
CRect rectButton( rect );
CRect rcBaloonExclude( rcToolbarClientArea );
	if( bHorz )
	{
		rectButton.OffsetRect(
			rcToolbarClientArea.right - rectButton.right,
			0
			);
		rectButton.InflateRect( 1, 0, 0, 0 );
		rectButton.top = rcToolbarClientArea.top;
		rectButton.bottom = rcToolbarClientArea.bottom - 1;
		rcBaloonExclude.right = rectButton.left + 1;
		rectButton.left -= m_sizeToolBarRgnRounders.cx;
	} // if( bHorz )
	else
	{
		rectButton.OffsetRect(
			0,
			rcToolbarClientArea.bottom - rectButton.bottom
			);
		rectButton.InflateRect( 0, 1, 0, 0 );
		rectButton.left = rcToolbarClientArea.left;
		rectButton.right = rcToolbarClientArea.right - 1;
		rcBaloonExclude.bottom = rectButton.top + 1;
		rectButton.top -= m_sizeToolBarRgnRounders.cy;
	} // else from if( bHorz )

CRgn rgnButton, rgnBE;
	if(		(! rgnButton.CreateRoundRectRgn(
				rcToolbarClientArea.left, rcToolbarClientArea.top,
				rcToolbarClientArea.right, rcToolbarClientArea.bottom,
				m_sizeToolBarRgnRounders.cx, m_sizeToolBarRgnRounders.cy
				)
			)
		||	(! rgnBE.CreateRoundRectRgn(
				rcBaloonExclude.left, rcBaloonExclude.top,
				rcBaloonExclude.right, rcBaloonExclude.bottom,
				m_sizeToolBarRgnRounders.cx, m_sizeToolBarRgnRounders.cy
				)
			)
		||	rgnButton.CombineRgn(
				&rgnButton,
				&rgnBE,
				RGN_DIFF
				) == ERROR
		)
	{
		ASSERT( FALSE );
		CExtPaintManagerXP::PaintToolbarExpandButton(
			dc,
			rcButtonArea,
			bHorz,
			bBarIsCompletelyVisible,
			bEnabled,
			bPushed,
			bHover,
			pHelperSrc,
			lParam,
			bTransparentBackground
			);
		return;
	}

	dc.SelectClipRgn( &rgnButton );

	if( bHover || bPushed )
	{
		CRect rcSolidFill( rectButton );
		rcSolidFill.InflateRect( 1, 1 );
//		dc.FillSolidRect(
//			&rcSolidFill,
//			GetColor(
//				bPushed
//					? _2003CLR_EXPBTN_PRESSED
//					: _2003CLR_EXPBTN_HOVER
//				)
//			);
		stat_PaintGradientRect(
			dc,
			&rcSolidFill,
			GetColor(
				bPushed
					? ( bHorz ? _2003CLR_EXPBTN_PRESSED_LIGHT : _2003CLR_EXPBTN_PRESSED_DARK )
					: ( bHorz ? _2003CLR_EXPBTN_HOVER_DARK : _2003CLR_EXPBTN_HOVER_LIGHT )
				),
			GetColor(
				bPushed
					? ( bHorz ? _2003CLR_EXPBTN_PRESSED_DARK : _2003CLR_EXPBTN_PRESSED_LIGHT )
					: ( bHorz ? _2003CLR_EXPBTN_HOVER_LIGHT : _2003CLR_EXPBTN_HOVER_DARK )
				),
			bHorz
			);
	} // if( bHover || bPushed )
	else
	{
		if(		g_PaintManager.m_bUxApiInitPassed
			&&	g_PaintManager.m_bUxValidColorsExtracted
			&&	g_PaintManager.m_bUxUseIfAvailOnWinXpOrLater
			)
		{
			stat_PaintGradientRect(
				dc,
				rectButton,
				GetColor(
					bHorz
						? _2003CLR_EXPBTN_DARK
						: _2003CLR_EXPBTN_LIGHT
					),
				GetColor(
					bHorz
						? _2003CLR_EXPBTN_LIGHT
						: _2003CLR_EXPBTN_DARK
					),
				bHorz
				);
		}
		else
		{
			CRect rectButton1(rectButton), rectButton2(rectButton);
			if( bHorz )
			{
				rectButton1.bottom =
					rectButton1.top
					+ MulDiv( rectButton1.Height(), 3, 5 );
				rectButton2.top = rectButton1.bottom;
			} // if( bHorz )
			else
			{
				rectButton1.right =
					rectButton1.left
					+ MulDiv( rectButton1.Width(), 3, 5 );
				rectButton2.left = rectButton1.right;
			} // else from if( bHorz )
			dc.FillSolidRect(
				&rectButton2,
				GetColor( _2003CLR_EXPBTN_DARK )
				);
			stat_PaintGradientRect(
				dc,
				rectButton1,
				GetColor(
					bHorz
						? _2003CLR_EXPBTN_DARK
						: _2003CLR_EXPBTN_LIGHT
					),
				GetColor(
					bHorz
						? _2003CLR_EXPBTN_LIGHT
						: _2003CLR_EXPBTN_DARK
					),
				bHorz
				);
		}
	} // else from if( bHover || bPushed )

const glyph_t * pGlyph = NULL, * pGlyph2 = NULL;
	if( bHorz )
	{
		pGlyph = m_pGlyphTbEpBtnH0;
		pGlyph2 = m_pGlyphTbEpBtnH1;
	} // if( bHorz )
	else
	{
		pGlyph = m_pGlyphTbEpBtnV0;
		pGlyph2 = m_pGlyphTbEpBtnV1;
	} // else from if( bHorz )
	ASSERT( pGlyph != NULL );
	ASSERT( pGlyph2 != NULL );

COLORREF ColorValuesLight[] =
{
	0,
	GetColor( _2003CLR_EXPGLYPH_LIGHT )
};
COLORREF ColorValuesDark[] =
{
	0,
	GetColor( _2003CLR_EXPGLYPH_DARK )
};
CRect rectGlyph(rectButton.TopLeft(),pGlyph->Size());
CRect rectGlyph2(rectGlyph);
CSize sizePushedOffset = GetPushedOffset();
	if( bHorz )
	{
		int nGap = (max(sizePushedOffset.cy,1)) * 3;
		rectGlyph.OffsetRect(
			(rectButton.Size().cx - pGlyph->Size().cx) / 2 + 1,
			rectButton.Size().cy - pGlyph->Size().cy - nGap - 1
			);
		rectGlyph2.OffsetRect(
			(rectButton.Size().cx - pGlyph2->Size().cx) / 2 + 1,
			nGap + 1
			);
	} // if( bHorz )
	else
	{
		int nGap = (max(sizePushedOffset.cx,1)) * 3;
		rectGlyph.OffsetRect(
			nGap + 1,
			(rectButton.Size().cy - pGlyph->Size().cy)/2 + 1
			);
		rectGlyph2.OffsetRect(
			rectButton.Size().cx - pGlyph2->Size().cx - nGap - 1,
			(rectButton.Size().cy - pGlyph2->Size().cy) / 2 + 1
			);
		ASSERT( m_bExpBtnSwapVGlyphs );
		CRect rcTmp( rectGlyph );
		rectGlyph = rectGlyph2;
		rectGlyph2 = rcTmp;
	} // else from if( bHorz )

	rectGlyph.OffsetRect(1,1);
	rectGlyph2.OffsetRect(1,1);
	PaintGlyph(
		dc,rectGlyph.TopLeft(),*pGlyph,ColorValuesLight
		);
	if( !bBarIsCompletelyVisible )
		PaintGlyph(
			dc,rectGlyph2.TopLeft(),*pGlyph2,ColorValuesLight
			);
	
	rectGlyph.OffsetRect(-1,-1);
	rectGlyph2.OffsetRect(-1,-1);
	PaintGlyph(
		dc,rectGlyph.TopLeft(),*pGlyph,ColorValuesDark
		);
	if( !bBarIsCompletelyVisible )
		PaintGlyph(
			dc,rectGlyph2.TopLeft(),*pGlyph2,ColorValuesDark
			);

	dc.SelectClipRgn( NULL );
}

void CExtPaintManager::stat_ExcludeChildAreas(
	HDC hDC,
	HWND hWnd,
	CExtPaintManager::pfnExcludeChildAreaCallback pCallback, // = NULL
	LPVOID pCallbackCookie // = NULL
	)
{
	if( hDC == NULL || !IsWindow(hWnd) )
		return;
HWND hWndChild = GetWindow(hWnd,GW_CHILD);
	for(; hWndChild != NULL; hWndChild = GetWindow(hWndChild,GW_HWNDNEXT))
	{
		if( !IsWindowVisible(hWndChild) )
			continue;
		DWORD dwChildStyle =
			DWORD( GetWindowLong(hWndChild,GWL_STYLE) );
		if( (dwChildStyle & WS_CHILD) == 0 )
			continue;
		RECT rc;
		if( !GetWindowRect(hWndChild,&rc) )
			continue;
		if( !ScreenToClient(hWnd, (LPPOINT)(&rc)) )
			continue;
		if( !ScreenToClient(hWnd, ((LPPOINT)(&rc))+1) )
			continue;
		if( pCallback != NULL )
		{
			if( pCallback(
					hDC,
					hWnd,
					hWndChild,
					pCallbackCookie
					)
				)
				continue;
		}
		ExcludeClipRect(
			hDC,
			rc.left,rc.top,
			rc.right,rc.bottom
			);
	} // for(; hWndChild != NULL; hWndChild = GetWindow(hWndChild,GW_HWNDNEXT))
}

//static volatile bool stat_bTrySysTransparentBlt = true;
//static volatile HMODULE stat_hModuleMsimg32 = NULL;
//typedef BOOL (*pMsimg32_TransparentBlt_t)(
//		HDC hdcDest,        // handle to destination DC
//		int nXOriginDest,   // x-coord of destination upper-left corner
//		int nYOriginDest,   // y-coord of destination upper-left corner
//		int nWidthDest,     // width of destination rectangle
//		int hHeightDest,    // height of destination rectangle
//		HDC hdcSrc,         // handle to source DC
//		int nXOriginSrc,    // x-coord of source upper-left corner
//		int nYOriginSrc,    // y-coord of source upper-left corner
//		int nWidthSrc,      // width of source rectangle
//		int nHeightSrc,     // height of source rectangle
//		UINT crTransparent  // color to make transparent
//	);
//static volatile pMsimg32_TransparentBlt_t pMsimg32_TransparentBlt = NULL;

void CExtPaintManager::stat_TransparentBlt(
	HDC hdcDest,        // handle to destination DC
	int nXOriginDest,   // x-coord of destination upper-left corner
	int nYOriginDest,   // y-coord of destination upper-left corner
	int nWidthDest,     // width of destination rectangle
	int hHeightDest,    // height of destination rectangle
	HDC hdcSrc,         // handle to source DC
	int nXOriginSrc,    // x-coord of source upper-left corner
	int nYOriginSrc,    // y-coord of source upper-left corner
	int nWidthSrc,      // width of source rectangle
	int nHeightSrc,     // height of source rectangle
	UINT crTransparent  // color to make transparent
	)
{
	ASSERT( hdcDest != NULL && hdcSrc != NULL );
CDC * pDcDest = CDC::FromHandle( hdcDest );
	ASSERT( pDcDest != NULL );
CExtMemoryDC
		dcmm( pDcDest ),
		dcmsk(
			pDcDest,
			NULL,
			CExtMemoryDC::MDCOPT_TO_MEMORY
				|CExtMemoryDC::MDCOPT_FORCE_BLACK
			)
		;
	if(		( nWidthDest != nWidthSrc || hHeightDest != nHeightSrc )
		&&	nWidthDest >= 0
		&&	hHeightDest >= 0
		)
	{
		VERIFY(
			::StretchBlt(
				dcmm.GetSafeHdc(),
				0,
				0,
				nWidthDest,
				hHeightDest,
				hdcSrc,
				nXOriginSrc,
				nYOriginSrc,
				nWidthSrc,
				nHeightSrc,
				SRCCOPY
				)
			);
	}
	else
	{
		VERIFY(
			::BitBlt(
				dcmm.GetSafeHdc(),
				0,
				0,
				nWidthSrc,
				nHeightSrc,
				hdcSrc,
				nXOriginSrc,
				nYOriginSrc,
				SRCCOPY
				)
			);
	}
int nRealWidthDest  =
		(nWidthDest < 0) ? nWidthSrc : nWidthDest;
int nRealHeightDest =
		(hHeightDest < 0) ? nHeightSrc : hHeightDest;
	dcmm.SetBkColor( crTransparent );
	VERIFY(
		dcmsk.BitBlt(
			0,
			0,
			nRealWidthDest,
			nRealHeightDest,
			&dcmm,
			0,
			0,
			SRCCOPY
			)
		);
	dcmm.SetBkColor( RGB(0,0,0) );
	dcmm.SetTextColor( RGB(255,255,255) );
	VERIFY(
		dcmm.BitBlt(
			0,
			0,
			nRealWidthDest,
			nRealHeightDest,
			&dcmsk,
			0,
			0,
			SRCAND
			)
		);
COLORREF clrBkOld = ::SetBkColor( hdcDest, RGB(255,255,255) );
COLORREF clrTextOld = ::SetTextColor( hdcDest, RGB(0,0,0) );
	VERIFY(
		::BitBlt(
			hdcDest,
			nXOriginDest,
			nYOriginDest,
			nRealWidthDest,
			nRealHeightDest,
			dcmsk.GetSafeHdc(),
			0,
			0,
			SRCAND
			)
		);
	VERIFY(
		::BitBlt(
			hdcDest,
			nXOriginDest,
			nYOriginDest,
			nRealWidthDest,
			nRealHeightDest,
			dcmm.GetSafeHdc(),
			0,
			0,
			SRCPAINT
			)
		);
	dcmm.__Flush( FALSE );
	dcmsk.__Flush( FALSE );
	::SetBkColor( hdcDest, clrBkOld );
	::SetTextColor( hdcDest, clrTextOld );
}

// generate DIB for UI light brush (when BPP <= 8 )
HBITMAP CExtPaintManager::stat_GenLBDIB(
	COLORREF clrDarker, // = COLORREF(-1) // by default COLORREF(-1) is changed to ::GetSysColor(COLOR_BTNFACE)
	COLORREF clrLighter, // = COLORREF(-1) // by default COLORREF(-1) is changed to ::GetSysColor(COLOR_BTNHIGHLIGHT)
	int nColorMaskHorz, // = 1
	int nColorMaskVert, // = 1
	int nSkipMaskHorz, // = 0
	int nSkipMaskVert, // = 0
	COLORREF clrSkip // = COLORREF(-1L) // // default COLORREF(-1) is changed to clrDarker
	)
{
	if( clrSkip == COLORREF(-1L) )
		clrSkip = clrDarker;
CWindowDC dcAlign( NULL );
CDC dc;
	if( !dc.CreateCompatibleDC( &dcAlign ) )
	{
		ASSERT( FALSE );
		return NULL;
	}
CBitmap bmp;
	if( !bmp.CreateCompatibleBitmap( &dcAlign, 16, 16 ) )
	{
		ASSERT( FALSE );
		return NULL;
	}
CBitmap * pOldBmp = dc.SelectObject( &bmp );
COLORREF clrEffectiveDarker =
		( clrDarker == COLORREF(-1) )
			? ( ::GetSysColor( COLOR_BTNFACE ) )
			: clrDarker
			;
COLORREF clrEffectiveLighter =
		( clrLighter == COLORREF(-1) )
			? ( ::GetSysColor( COLOR_BTNHIGHLIGHT ) )
			: clrLighter
			;
	for( int nY = 0; nY < 16; nY++ )
	{
		for( int nX = 0; nX < 16; nX++ )
		{
			if(		(nX & nSkipMaskHorz) != 0
				||	(nY & nSkipMaskVert) != 0
				)
			{
				dc.SetPixel( nX, nY, clrSkip );
				continue;
			}
			COLORREF clr = ( (nY&nColorMaskVert) != 0 )
				? ( ( (nX&nColorMaskHorz) != 0 ) ? clrEffectiveDarker : clrEffectiveLighter )
				: ( ( (nX&nColorMaskHorz) != 0 ) ? clrEffectiveLighter : clrEffectiveDarker )
				;
			dc.SetPixel( nX, nY, clr );
		}
	}
	dc.SelectObject( pOldBmp );
	return (HBITMAP)bmp.Detach();
}

void CExtPaintManager::stat_PaintGradientRect(
	CDC & dc,
	const CRect & rcPaintGradient,
	COLORREF clrLeft,
	COLORREF clrRight,
	bool bHorz, // = false
	UINT nCountOfSteps // = 256
	)
{
	if(		rcPaintGradient.left >= rcPaintGradient.right
		||	rcPaintGradient.top >= rcPaintGradient.bottom
		)
		return;
	ASSERT( nCountOfSteps > 0 && nCountOfSteps <= 256 );
UINT nWidth = rcPaintGradient.Width();
UINT nHeight = rcPaintGradient.Height();
	if( bHorz )
	{
		if( nCountOfSteps > nHeight )
			nCountOfSteps = nHeight;
	}
	else
	{
		if( nCountOfSteps > nWidth )
			nCountOfSteps = nWidth;
	}

int nBitsPerPixel = stat_GetBPP();
	if( nBitsPerPixel <= 8 )
	{
		UINT nCalcStepSize = 
			bHorz
				? nHeight / nCountOfSteps
				: nWidth  / nCountOfSteps
				;
static const UINT nLowColorStepMinSize = 4;
		if( nCalcStepSize < nLowColorStepMinSize )
			nCountOfSteps =
				bHorz
					? nHeight / nLowColorStepMinSize
					: nWidth  / nLowColorStepMinSize
					;
	} // if( nBitsPerPixel <= 8 )

CRect rcItem( rcPaintGradient );
	for(	UINT nStep = 0, nBackStep = nCountOfSteps;
			nStep < nCountOfSteps;
			nStep++, nBackStep--
			)
	{
		COLORREF clrItem =
			RGB(
				(	GetRValue(clrLeft)*nBackStep
					+ GetRValue(clrRight)*nStep
					) / nCountOfSteps,
				(	GetGValue(clrLeft)*nBackStep
					+ GetGValue(clrRight)*nStep
					) / nCountOfSteps,
				(	GetBValue(clrLeft)*nBackStep
					+ GetBValue(clrRight)*nStep
					) / nCountOfSteps
				);
		if( bHorz )
		{
			rcItem.top =
				rcPaintGradient.bottom -
				MulDiv( nStep, nHeight, nCountOfSteps )
				;
			rcItem.top--;
			if( nStep == nCountOfSteps
				&& rcItem.top < rcPaintGradient.top
				)
			{
				rcItem.top = rcPaintGradient.top;
				if( rcItem.top >= rcItem.bottom )
					break;
			}
			ASSERT( rcItem.Height() >= 1 );
			if( nBitsPerPixel <= 8 )
			{
				CBrush br( clrItem );
				dc.FillRect( rcItem, &br );
			} // if( nBitsPerPixel <= 8 )
			else
			{
				dc.FillSolidRect( rcItem, clrItem );
			} // else from if( nBitsPerPixel <= 8 )
			rcItem.bottom = rcItem.top;
		}
		else
		{
			rcItem.right =
				MulDiv( nStep, nWidth, nCountOfSteps )
				+ rcPaintGradient.left
				;
			rcItem.right++;
			if( nStep == nCountOfSteps
				&& rcItem.right > rcPaintGradient.right
				)
			{
				rcItem.right = rcPaintGradient.right;
				if( rcItem.right <= rcItem.left )
					break;
			}
			ASSERT( rcItem.Width() >= 1 );
			if( nBitsPerPixel <= 8 )
			{
				CBrush br( clrItem );
				dc.FillRect( rcItem, &br );
			} // if( nBitsPerPixel <= 8 )
			else
			{
				dc.FillSolidRect( rcItem, clrItem );
			} // else from if( nBitsPerPixel <= 8 )
			rcItem.left = rcItem.right;
		}
	}
}

static int CALLBACK stat_FindFontByNameCallBack(
	ENUMLOGFONT * pEnumLogFont,
	NEWTEXTMETRIC * pNewTextMetric,
	DWORD dwFontType,
	LPARAM lParam // zero terminated string
	)
{
	ASSERT( pEnumLogFont != NULL );
	pNewTextMetric;
	dwFontType;
__EXT_MFC_SAFE_LPCTSTR sFontToFind =
		reinterpret_cast < const TCHAR * > ( lParam );
	ASSERT( sFontToFind != NULL );
	return
		_tcsicoll(
			pEnumLogFont->elfLogFont.lfFaceName,
			sFontToFind
			);
}

static HFONT stat_menu_HFONT_from_current_NONCLIENTMETRICS(
	bool bUseTahomaFont
	)
{
NONCLIENTMETRICS _ncNfo;
	memset( &_ncNfo, 0, sizeof(NONCLIENTMETRICS) );
	_ncNfo.cbSize = sizeof(NONCLIENTMETRICS);
	if(	!SystemParametersInfo(
			SPI_GETNONCLIENTMETRICS,
			sizeof(_ncNfo),
			&_ncNfo,
			0
			)
		)
	{
		return NULL;
	}
	_ncNfo.lfMenuFont.lfCharSet = (BYTE)GetTextCharset( NULL );
	if( bUseTahomaFont )
	{
		static __EXT_MFC_SAFE_LPCTSTR g_sTahoma = _T("Tahoma");
		if(	!EnumFontFamilies(
				NULL,
				NULL,
				(FONTENUMPROC)stat_FindFontByNameCallBack,
				reinterpret_cast < LPARAM > ( LPCTSTR( g_sTahoma ) )
				)
			)
		{
			_tcscpy( _ncNfo.lfMenuFont.lfFaceName, g_sTahoma );
		}
	} // if( bUseTahomaFont )
	return CreateFontIndirect( &_ncNfo.lfMenuFont );
}

void CExtPaintManager::SyncSysColors()
{
	ASSERT( this != NULL );
	g_PaintManager.InitUserExApi();

	// init fonts
HFONT hDefaultGuiFont =
		stat_menu_HFONT_from_current_NONCLIENTMETRICS(
			m_bUseTahomaFont
			);
	if( hDefaultGuiFont == NULL )
		hDefaultGuiFont =
			(HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	if( hDefaultGuiFont == NULL )
		hDefaultGuiFont =
			(HFONT)::GetStockObject( SYSTEM_FONT );
	ASSERT( hDefaultGuiFont != NULL );
	if( m_FontNormal.GetSafeHandle() )
		m_FontNormal.DeleteObject();
	m_FontNormal.Attach( hDefaultGuiFont );

static __EXT_MFC_SAFE_LPCTSTR sVertFontFaceForNt4 = _T("Arial");

LOGFONT lf;
	VERIFY( m_FontNormal.GetLogFont(&lf) );
	lf.lfWeight = 900;
	if( m_FontBold.GetSafeHandle() )
		m_FontBold.DeleteObject();
	VERIFY( m_FontBold.CreateFontIndirect(&lf) );

	VERIFY( m_FontNormal.GetLogFont(&lf) );
	lf.lfCharSet = SYMBOL_CHARSET;
	lf.lfWeight = 0;
	lf.lfHeight = ::GetSystemMetrics(SM_CYMENUCHECK) - 1;
	_tcscpy( lf.lfFaceName, _T("Marlett") );
	if( m_FontMarlett.GetSafeHandle() )
		m_FontMarlett.DeleteObject();
	VERIFY( m_FontMarlett.CreateFontIndirect(&lf) );

	VERIFY( m_FontNormal.GetLogFont(&lf) );
	lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT__;
	if( m_FontNormalVert.GetSafeHandle() )
		m_FontNormalVert.DeleteObject();
	if( g_PaintManager.m_bIsWinNT4 || g_PaintManager.m_bIsWin9x )
		_tcscpy( lf.lfFaceName, sVertFontFaceForNt4 );
	VERIFY( m_FontNormalVert.CreateFontIndirect(&lf) );

	lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT_X__;
	if( m_FontNormalVertX.GetSafeHandle() )
		m_FontNormalVertX.DeleteObject();
	if( g_PaintManager.m_bIsWinNT4 || g_PaintManager.m_bIsWin9x )
		_tcscpy( lf.lfFaceName, sVertFontFaceForNt4 );
	VERIFY( m_FontNormalVertX.CreateFontIndirect(&lf) );

	VERIFY( m_FontBold.GetLogFont(&lf) );
	lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT__;
	if( m_FontBoldVert.GetSafeHandle() )
		m_FontBoldVert.DeleteObject();
	if( g_PaintManager.m_bIsWinNT4 || g_PaintManager.m_bIsWin9x )
		_tcscpy( lf.lfFaceName, sVertFontFaceForNt4 );
	VERIFY( m_FontBoldVert.CreateFontIndirect(&lf) );

	lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT_X__;
	if( m_FontBoldVertX.GetSafeHandle() )
		m_FontBoldVertX.DeleteObject();
	if( g_PaintManager.m_bIsWinNT4 || g_PaintManager.m_bIsWin9x )
		_tcscpy( lf.lfFaceName, sVertFontFaceForNt4 );
	VERIFY( m_FontBoldVertX.CreateFontIndirect(&lf) );

CFont * arrBarCaptFonts[6*2] =
{
	&m_FontNormalBC,		&m_FontNormal,
	&m_FontNormalVertBC,	&m_FontNormalVert,
	&m_FontNormalVertXBC,	&m_FontNormalVertX,
	&m_FontBoldBC,			&m_FontBold,
	&m_FontBoldVertBC,		&m_FontBoldVert,
	&m_FontBoldVertXBC,		&m_FontBoldVertX
};
	for(	int i = 0;
			i < sizeof(arrBarCaptFonts)/sizeof(arrBarCaptFonts[0]);
			i += 2
			)
	{
		CFont * pFontSrc = arrBarCaptFonts[i+1];
		ASSERT( pFontSrc->GetSafeHandle() != NULL );
		CFont * pFontDst = arrBarCaptFonts[i];
		if( pFontDst->GetSafeHandle() != NULL )
			pFontDst->DeleteObject();
		VERIFY( pFontSrc->GetLogFont(&lf) );
		if( lf.lfHeight < -11 )
			lf.lfHeight = -11;
		VERIFY( pFontDst->CreateFontIndirect(&lf) );
	}

	/// init colors

COLORREF clrDefaultExtendColor = ::GetSysColor( COLOR_3DFACE );
	for(	i = m_colors.GetSize();
			i <= __ExtMfc_MAX_SYS_COLOR_VAL;
			i++
			)
	{
		m_colors.Add( clrDefaultExtendColor );
	}

HBRUSH hDefaultExtendBrush = ::GetSysColorBrush( COLOR_3DFACE );
	for(	i = m_brushes.GetSize();
			i <= __ExtMfc_MAX_SYS_COLOR_VAL;
			i++
			)
	{
		m_brushes.Add( hDefaultExtendBrush );
	}

	//m_colors.clear();
	//m_brushes.clear();
	for(	i = __ExtMfc_MIN_SYS_COLOR_VAL;
			i <= __ExtMfc_MAX_SYS_COLOR_VAL;
			i++
			)
	{
		m_colors[i] = ::GetSysColor( i );
		m_brushes[i] = ::GetSysColorBrush( i );
	}
	ASSERT( m_colors.GetSize() >= __ExtMfc_MAX_SYS_COLOR_VAL+1 );
	ASSERT( m_brushes.GetSize() >= __ExtMfc_MAX_SYS_COLOR_VAL+1 );

	if( m_brushLightDefault.GetSafeHandle() != NULL )
		m_brushLightDefault.DeleteObject();
	if( m_brushLightSystem.GetSafeHandle() != NULL )
		m_brushLightSystem.DeleteObject();
	if( m_brushDarkDefault.GetSafeHandle() != NULL )
		m_brushDarkDefault.DeleteObject();
	if( m_brushDarkSystem.GetSafeHandle() != NULL )
		m_brushDarkSystem.DeleteObject();
	if( m_brushLighterDefault.GetSafeHandle() != NULL )
		m_brushLighterDefault.DeleteObject();
	if( m_brushLighterSystem.GetSafeHandle() != NULL )
		m_brushLighterSystem.DeleteObject();
	if( m_brushDarkerDefault.GetSafeHandle() != NULL )
		m_brushDarkerDefault.DeleteObject();
	if( m_brushDarkerSystem.GetSafeHandle() != NULL )
		m_brushDarkerSystem.DeleteObject();
	if( m_brushLightestDefault.GetSafeHandle() != NULL )
		m_brushLightestDefault.DeleteObject();
	if( m_brushLightestSystem.GetSafeHandle() != NULL )
		m_brushLightestSystem.DeleteObject();
	if( m_brushDarkestDefault.GetSafeHandle() != NULL )
		m_brushDarkestDefault.DeleteObject();
	if( m_brushDarkestSystem.GetSafeHandle() != NULL )
		m_brushDarkestSystem.DeleteObject();

int nBitsPerPixel = stat_GetBPP();
	if( nBitsPerPixel > 8 )
	{
		COLORREF clrDarker = GetColor( COLOR_3DFACE );
		COLORREF clrLighter = GetColor( COLOR_3DHILIGHT );
		COLORREF clrMixed = RGB (
			GetRValue(clrDarker) + ((GetRValue(clrLighter) -
				GetRValue(clrDarker)) / 2),
			GetGValue(clrDarker) + ((GetGValue(clrLighter) -
				GetGValue(clrDarker)) / 2),
			GetBValue(clrDarker) + ((GetBValue(clrLighter) -
				GetBValue(clrDarker)) / 2)
			);
		m_brushLightDefault.CreateSolidBrush( clrMixed );
		clrMixed = RGB (
			GetRValue(clrDarker) +
				::MulDiv((GetRValue(clrLighter) -
					GetRValue(clrDarker)),3,5),
			GetGValue(clrDarker) +
				::MulDiv((GetGValue(clrLighter) -
					GetGValue(clrDarker)),3,5),
			GetBValue(clrDarker) +
				::MulDiv((GetBValue(clrLighter) -
					GetBValue(clrDarker)),3,5)
			);
		m_brushLighterDefault.CreateSolidBrush( clrMixed );
		clrMixed = RGB (
			GetRValue(clrDarker) +
				::MulDiv((GetRValue(clrLighter) -
					GetRValue(clrDarker)),4,5),
			GetGValue(clrDarker) +
				::MulDiv((GetGValue(clrLighter) -
					GetGValue(clrDarker)),4,5),
			GetBValue(clrDarker) +
				::MulDiv((GetBValue(clrLighter) -
					GetBValue(clrDarker)),4,5)
			);
		m_brushLightestDefault.CreateSolidBrush( clrMixed );
		
		clrLighter = clrDarker;
		clrDarker = GetColor( COLOR_3DSHADOW );
		clrMixed = RGB (
			GetRValue(clrDarker) + ((GetRValue(clrLighter) -
				GetRValue(clrDarker)) / 2 ),
			GetGValue(clrDarker) + ((GetGValue(clrLighter) -
				GetGValue(clrDarker)) / 2),
			GetBValue(clrDarker) + ((GetBValue(clrLighter) -
				GetBValue(clrDarker)) / 2)
			);
		m_brushDarkDefault.CreateSolidBrush( clrMixed );
		clrMixed = RGB (
			GetRValue(clrDarker) +
				::MulDiv((GetRValue(clrLighter) -
					GetRValue(clrDarker)),2,5),
			GetGValue(clrDarker) +
				::MulDiv((GetGValue(clrLighter) -
					GetGValue(clrDarker)),2,5),
			GetBValue(clrDarker) +
				::MulDiv((GetBValue(clrLighter) -
					GetBValue(clrDarker)),2,5)
			);
		m_brushDarkerDefault.CreateSolidBrush( clrMixed );
		clrMixed = RGB (
			GetRValue(clrDarker) +
				::MulDiv((GetRValue(clrLighter) -
					GetRValue(clrDarker)),1,5),
			GetGValue(clrDarker) +
				::MulDiv((GetGValue(clrLighter) -
					GetGValue(clrDarker)),1,5),
			GetBValue(clrDarker) +
				::MulDiv((GetBValue(clrLighter) -
					GetBValue(clrDarker)),1,5)
			);
		m_brushDarkestDefault.CreateSolidBrush( clrMixed );

		clrDarker = ::GetSysColor( COLOR_3DFACE );
		clrLighter = ::GetSysColor( COLOR_3DHILIGHT );
		clrMixed = RGB (
			GetRValue(clrDarker) + ((GetRValue(clrLighter) -
				GetRValue(clrDarker)) / 2),
			GetGValue(clrDarker) + ((GetGValue(clrLighter) -
				GetGValue(clrDarker)) / 2),
			GetBValue(clrDarker) + ((GetBValue(clrLighter) -
				GetBValue(clrDarker)) / 2)
			);
		m_brushLightSystem.CreateSolidBrush( clrMixed );
		clrMixed = RGB (
			GetRValue(clrDarker) +
				::MulDiv((GetRValue(clrLighter) -
					GetRValue(clrDarker)),3,5),
			GetGValue(clrDarker) +
				::MulDiv((GetGValue(clrLighter) -
					GetGValue(clrDarker)),3,5),
			GetBValue(clrDarker) +
				::MulDiv((GetBValue(clrLighter) -
					GetBValue(clrDarker)),3,5)
			);
		m_brushLighterSystem.CreateSolidBrush( clrMixed );
		clrMixed = RGB (
			GetRValue(clrDarker) +
				::MulDiv((GetRValue(clrLighter) -
					GetRValue(clrDarker)),4,5),
			GetGValue(clrDarker) +
				::MulDiv((GetGValue(clrLighter) -
					GetGValue(clrDarker)),4,5),
			GetBValue(clrDarker) +
				::MulDiv((GetBValue(clrLighter) -
					GetBValue(clrDarker)),4,5)
			);
		m_brushLightestSystem.CreateSolidBrush( clrMixed );
		
		clrLighter = clrDarker;
		clrDarker = ::GetSysColor( COLOR_3DSHADOW );
		clrMixed = RGB (
			GetRValue(clrDarker) + ((GetRValue(clrLighter) -
				GetRValue(clrDarker)) / 2),
			GetGValue(clrDarker) + ((GetGValue(clrLighter) -
				GetGValue(clrDarker)) / 2),
			GetBValue(clrDarker) + ((GetBValue(clrLighter) -
				GetBValue(clrDarker)) / 2)
			);
		m_brushDarkSystem.CreateSolidBrush( clrMixed );
		clrMixed = RGB (
			GetRValue(clrDarker) +
				::MulDiv((GetRValue(clrLighter) -
					GetRValue(clrDarker)),2,5),
			GetGValue(clrDarker) +
				::MulDiv((GetGValue(clrLighter) -
					GetGValue(clrDarker)),2,5),
			GetBValue(clrDarker) +
				::MulDiv((GetBValue(clrLighter) -
					GetBValue(clrDarker)),2,5)
			);
		m_brushDarkerSystem.CreateSolidBrush( clrMixed );
		clrMixed = RGB (
			GetRValue(clrDarker) +
				::MulDiv((GetRValue(clrLighter) -
					GetRValue(clrDarker)),1,5),
			GetGValue(clrDarker) +
				::MulDiv((GetGValue(clrLighter) -
					GetGValue(clrDarker)),1,5),
			GetBValue(clrDarker) +
				::MulDiv((GetBValue(clrLighter) -
					GetBValue(clrDarker)),1,5)
			);
		m_brushDarkestSystem.CreateSolidBrush( clrMixed );
	} // if( nBitsPerPixel > 8 )
	else
	{
		CBitmap bmp;
		COLORREF clrDarker = GetColor( COLOR_3DFACE );
		COLORREF clrLighter = GetColor( COLOR_3DHILIGHT );
		HBITMAP hBmp = stat_GenLBDIB( clrDarker, clrLighter );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushLightDefault.CreatePatternBrush( &bmp );
		bmp.DeleteObject();
		hBmp = stat_GenLBDIB( clrDarker, clrLighter, 1, 1, 1, 1, clrLighter );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushLighterDefault.CreatePatternBrush( &bmp );
		bmp.DeleteObject();
		hBmp = stat_GenLBDIB( clrDarker, clrLighter, 1, 1, 2, 2, clrLighter );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushLightestDefault.CreatePatternBrush( &bmp );
		bmp.DeleteObject();

		clrLighter = clrDarker;
		clrDarker = GetColor( COLOR_3DSHADOW );
		hBmp = stat_GenLBDIB( clrDarker, clrLighter );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushDarkDefault.CreatePatternBrush( &bmp );
		bmp.DeleteObject();
		hBmp = stat_GenLBDIB( clrDarker, clrLighter, 1, 1, 1, 1 );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushDarkerDefault.CreatePatternBrush( &bmp );
		bmp.DeleteObject();
		clrLighter = clrDarker;
		clrDarker = GetColor( COLOR_3DDKSHADOW );
		hBmp = stat_GenLBDIB( clrDarker, clrLighter );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushDarkestDefault.CreatePatternBrush( &bmp );
		bmp.DeleteObject();

		clrDarker = ::GetSysColor( COLOR_3DFACE );
		clrLighter = ::GetSysColor( COLOR_3DHILIGHT );
		hBmp = stat_GenLBDIB( clrDarker, clrLighter );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushLightSystem.CreatePatternBrush( &bmp );
		bmp.DeleteObject();
		hBmp = stat_GenLBDIB( clrDarker, clrLighter, 1, 1, 1, 1, clrLighter );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushLighterSystem.CreatePatternBrush( &bmp );
		bmp.DeleteObject();
		hBmp = stat_GenLBDIB( clrDarker, clrLighter, 1, 1, 2, 2, clrLighter );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushLightestSystem.CreatePatternBrush( &bmp );
		bmp.DeleteObject();

		clrLighter = clrDarker;
		clrDarker = ::GetSysColor( COLOR_3DSHADOW );
		hBmp = stat_GenLBDIB( clrDarker, clrLighter );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushDarkSystem.CreatePatternBrush( &bmp );
		bmp.DeleteObject();
		hBmp = stat_GenLBDIB( clrDarker, clrLighter, 1, 1, 1, 1 );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushDarkerSystem.CreatePatternBrush( &bmp );
		bmp.DeleteObject();
		clrLighter = clrDarker;
		clrDarker = ::GetSysColor( COLOR_3DDKSHADOW );
		hBmp = stat_GenLBDIB( clrDarker, clrLighter );
		ASSERT( hBmp != NULL );
		bmp.Attach( hBmp );
		m_brushDarkestSystem.CreatePatternBrush( &bmp );
		bmp.DeleteObject();
	} // else from if( nBitsPerPixel > 8 )
}

// get any system color based brush
HBRUSH CExtPaintManager::GetBrush(
	int nColorIndex
	)
{
int nBrushesCount = m_brushes.GetSize();
	if(
			!(
				__ExtMfc_MIN_SYS_COLOR_VAL <= nColorIndex
				&&
				nColorIndex < nBrushesCount
			)
		)
	{
		ASSERT( FALSE );
		nColorIndex = COLOR_3DFACE;
	}
HBRUSH hBrush = m_brushes[nColorIndex];
	return hBrush;
}

COLORREF CExtPaintManager::GetColor(int nColorIndex)
{
	ASSERT( this != NULL );

	if( nColorIndex >= __ExtMfc_COLOR_MAP_BASE )
	{
		int nColorIndex2 = 0;
		if( m_mapColorTranslate.Lookup(
				nColorIndex,
				nColorIndex2
				)
			)
			nColorIndex = nColorIndex2;
		else
		{
			ASSERT( FALSE );
			nColorIndex = COLOR_3DFACE;
		}
	}
int nColorCount = m_colors.GetSize();
	if(
			!(
				__ExtMfc_MIN_SYS_COLOR_VAL <= nColorIndex
				&&
				nColorIndex < nColorCount // <= __ExtMfc_MAX_SYS_COLOR_VAL
			)
		)
	{
		ASSERT( FALSE );
		nColorIndex = COLOR_3DFACE;
	}
COLORREF clr = m_colors[nColorIndex];
	return clr;
}

int CExtPaintManager::InstallColor(
	COLORREF clr,
	int nColorIndex // = -1
	)
{
	ASSERT( this != NULL );
int nColorCount = m_colors.GetSize();
	if( nColorIndex < 0 || nColorIndex >= nColorCount )
	{
		nColorIndex = m_colors.Add( clr );
	}
	else
		m_colors[nColorIndex] = clr;
	return nColorIndex;
}


void CExtPaintManager::InitTranslatedColors()
{
	ASSERT( this != NULL );

	m_mapColorTranslate.RemoveAll();

	m_mapColorTranslate[CLR_3DFACE_OUT]				= COLOR_3DFACE;
	m_mapColorTranslate[CLR_3DFACE_IN]				= COLOR_3DFACE;
	m_mapColorTranslate[CLR_3DFACE_DISABLED]		= COLOR_3DFACE;

	m_mapColorTranslate[CLR_3DLIGHT_OUT]			= COLOR_3DLIGHT;
	m_mapColorTranslate[CLR_3DLIGHT_IN]				= COLOR_3DLIGHT;
	m_mapColorTranslate[CLR_3DLIGHT_DISABLED]		= COLOR_3DLIGHT;

	m_mapColorTranslate[CLR_3DHILIGHT_OUT]			= COLOR_3DHILIGHT;
	m_mapColorTranslate[CLR_3DHILIGHT_IN]			= COLOR_3DHILIGHT;
	m_mapColorTranslate[CLR_3DHILIGHT_DISABLED]		= COLOR_3DHILIGHT;

	m_mapColorTranslate[CLR_3DSHADOW_OUT]			= COLOR_3DSHADOW;
	m_mapColorTranslate[CLR_3DSHADOW_IN]			= COLOR_3DSHADOW;
	m_mapColorTranslate[CLR_3DSHADOW_DISABLED]		= COLOR_3DSHADOW;

	m_mapColorTranslate[CLR_3DDKSHADOW_OUT]			= COLOR_3DDKSHADOW;
	m_mapColorTranslate[CLR_3DDKSHADOW_IN]			= COLOR_3DDKSHADOW;
	m_mapColorTranslate[CLR_3DDKSHADOW_DISABLED]	= COLOR_3DDKSHADOW;
	
	m_mapColorTranslate[CLR_TEXT_OUT]				= COLOR_BTNTEXT;
	m_mapColorTranslate[CLR_TEXT_IN]				= COLOR_BTNTEXT;
	m_mapColorTranslate[CLR_TEXT_DISABLED]			= COLOR_3DSHADOW;
}

static const UINT stat_pixels_dock_btn_empty[] =
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_btn_close[] =
{
	0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,1,1,
	0,0,1,1,0,0,1,1,0,
	0,0,0,1,1,1,1,0,0,
	0,0,0,0,1,1,0,0,0,
	0,0,0,1,1,1,1,0,0,
	0,0,1,1,0,0,1,1,0,
	0,1,1,0,0,0,0,1,1,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_btn_close_dc2k[] =
{
	0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,1,1,0,
	0,0,1,1,0,1,1,0,0,
	0,0,0,1,1,1,0,0,0,
	0,0,0,1,1,1,0,0,0,
	0,0,1,1,0,1,1,0,0,
	0,1,1,0,0,0,1,1,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_btn_thin_close[] =
{
	0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,0,0,1,0,
	0,0,1,0,0,0,1,0,0,
	0,0,0,1,0,1,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,1,0,1,0,0,0,
	0,0,1,0,0,0,1,0,0,
	0,1,0,0,0,0,0,1,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_btn_arrow_up[] =
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,1,1,1,0,0,0,
	0,0,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_btn_hollow_up[] =
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,1,0,1,0,0,0,
	0,0,1,0,0,0,1,0,0,
	0,1,0,0,0,0,0,1,0,
	1,1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_btn_arrow_up_l[] =
{
	0,0,0,0,1,0,0,0,0,
	0,0,0,1,1,1,0,0,0,
	0,0,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_wnd_minimize[] =
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_wnd_restore[] =
{
	0,0,1,1,1,1,1,1,0,
	0,0,1,0,0,0,0,1,0,
	1,1,1,1,1,1,0,1,0,
	1,1,1,1,1,1,0,1,0,
	1,0,0,0,0,1,1,1,0,
	1,0,0,0,0,1,0,0,0,
	1,0,0,0,0,1,0,0,0,
	1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_wnd_maximize[] =
{
	1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,
};

static const UINT stat_pixels_dock_wnd_keep[] =
{
	0,0,1,1,1,1,1,0,0,
	0,0,1,0,0,1,1,0,0,
	0,0,1,0,0,1,1,0,0,
	0,0,1,0,0,1,1,0,0,
	0,0,1,0,0,1,1,0,0,
	0,1,1,1,1,1,1,1,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_btn_contexthelp[] =
{
	0,0,0,1,1,1,1,0,0,
	0,0,1,1,0,0,1,1,0,
	0,0,1,1,0,0,1,1,0,
	0,0,0,0,0,1,1,0,0,
	0,0,0,0,1,1,0,0,0,
	0,0,0,0,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,1,0,0,0,
	0,0,0,0,1,1,0,0,0,
};

static const UINT stat_pixels_hollow_plus[] =
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,1,1,1,1,1,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_hollow_minus[] =
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_tree_plus[] =
{
	1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,1,0,0,0,1,
	1,0,0,0,1,0,0,0,1,
	1,0,1,1,1,1,1,0,1,
	1,0,0,0,1,0,0,0,1,
	1,0,0,0,1,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,
};

static const UINT stat_pixels_tree_minus[] =
{
	1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,1,1,1,1,1,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,
};

static const UINT stat_pixels_circle_plus[] =
{
	0,0,1,1,1,1,1,0,0,
	0,1,0,0,0,0,0,1,0,
	1,0,0,0,1,0,0,0,1,
	1,0,0,0,1,0,0,0,1,
	1,0,1,1,1,1,1,0,1,
	1,0,0,0,1,0,0,0,1,
	1,0,0,0,1,0,0,0,1,
	0,1,0,0,0,0,0,1,0,
	0,0,1,1,1,1,1,0,0,
};

static const UINT stat_pixels_circle_minus[] =
{
	0,0,1,1,1,1,1,0,0,
	0,1,0,0,0,0,0,1,0,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,1,1,1,1,1,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	0,1,0,0,0,0,0,1,0,
	0,0,1,1,1,1,1,0,0,
};

static const UINT stat_pixels_arrow_pt_up[] =
{
	0,0,0,0,0,1,0,0,0,0,0,
	0,0,0,0,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,
	0,0,0,1,1,1,1,1,0,0,0,
	0,0,0,1,1,1,1,1,0,0,0,
	0,0,0,1,1,1,1,1,0,0,0,
	0,0,0,1,1,1,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_arrow_pt_lt[] =
{
	1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,0,0,0,0,
	1,1,1,1,1,1,0,0,0,0,0,
	1,1,1,1,1,1,0,0,0,0,0,
	1,1,1,1,1,1,1,0,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,
	1,1,0,0,1,1,1,1,1,0,0,
	1,0,0,0,0,1,1,1,0,0,0,
	0,0,0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_arrow_small_up[] =
{
	0,0,1,0,0,
	0,1,1,1,0,
	1,1,1,1,1,
};

static const UINT stat_pixels_ellipsis[] =
{
	0,0,0,0,0,0,0,0,0,
	0,1,0,0,1,0,0,1,0,
	0,0,0,0,0,0,0,0,0,
};

static CExtPaintManager::glyph_t stat_glyph_dock_empty(
	9,9,2,
	stat_pixels_dock_btn_empty
	);
static CExtPaintManager::glyph_t stat_glyph_dock_close(
	9,9,2,
	stat_pixels_dock_btn_close
	);
static CExtPaintManager::glyph_t stat_glyph_dock_close_dc2k(
	9,9,2,
	stat_pixels_dock_btn_close_dc2k
	);
static CExtPaintManager::glyph_t stat_glyph_dock_xp_thin_close(
	9,9,2,
	stat_pixels_dock_btn_thin_close
	);

static CExtPaintManager::glyph_t stat_glyph_dock_arrow_up(
	9,9,2,
	stat_pixels_dock_btn_arrow_up
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_down(
	stat_glyph_dock_arrow_up,
	180
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_left(
	stat_glyph_dock_arrow_up,
	270
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_right(
	stat_glyph_dock_arrow_left,
	180
	);
static CExtPaintManager::glyph_t stat_glyph_dock_wnd_minimize(
	9,9,2,
	stat_pixels_dock_wnd_minimize
	);
static CExtPaintManager::glyph_t stat_glyph_dock_wnd_restore(
	9,9,2,
	stat_pixels_dock_wnd_restore
	);
static CExtPaintManager::glyph_t stat_glyph_dock_wnd_maximize(
	9,9,2,
	stat_pixels_dock_wnd_maximize
	);
static CExtPaintManager::glyph_t stat_glyph_dock_wnd_keep(
	9,9,2,
	stat_pixels_dock_wnd_keep
	);
static CExtPaintManager::glyph_t stat_glyph_dock_wnd_unkeep(
	stat_glyph_dock_wnd_keep,
	90
	);

static CExtPaintManager::glyph_t stat_glyph_dock_wnd_contexthelp(
	9,9,2,
	stat_pixels_dock_btn_contexthelp
	);

static CExtPaintManager::glyph_t stat_glyph_dock_arrow_up_l(
	9,9,2,
	stat_pixels_dock_btn_arrow_up_l
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_down_l(
	stat_glyph_dock_arrow_up_l,
	180
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_left_l(
	stat_glyph_dock_arrow_up_l,
	270
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_right_l(
	stat_glyph_dock_arrow_left_l,
	180
	);

static CExtPaintManager::glyph_t stat_glyph_dock_hollow_up(
	9,9,2,
	stat_pixels_dock_btn_hollow_up
	);
static CExtPaintManager::glyph_t stat_glyph_dock_hollow_down(
	stat_glyph_dock_hollow_up,
	180
	);
static CExtPaintManager::glyph_t stat_glyph_dock_hollow_left(
	stat_glyph_dock_hollow_up,
	270
	);
static CExtPaintManager::glyph_t stat_glyph_dock_hollow_right(
	stat_glyph_dock_hollow_left,
	180
	);

static CExtPaintManager::glyph_t stat_glyph_hollow_plus(
	9,9,2,
	stat_pixels_hollow_plus
	);
static CExtPaintManager::glyph_t stat_glyph_hollow_minus(
	9,9,2,
	stat_pixels_hollow_minus
	);
static CExtPaintManager::glyph_t stat_glyph_tree_plus(
	9,9,2,
	stat_pixels_tree_plus
	);
static CExtPaintManager::glyph_t stat_glyph_tree_minus(
	9,9,2,
	stat_pixels_tree_minus
	);
static CExtPaintManager::glyph_t stat_glyph_circle_plus(
	9,9,2,
	stat_pixels_circle_plus
	);
static CExtPaintManager::glyph_t stat_glyph_circle_minus(
	9,9,2,
	stat_pixels_circle_minus
	);

static CExtPaintManager::glyph_t stat_glyph_arrow_pt_up(
	11,11,2,
	stat_pixels_arrow_pt_up
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_pt_down(
	stat_glyph_arrow_pt_up,
	180
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_pt_left(
	stat_glyph_arrow_pt_up,
	270
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_pt_right(
	stat_glyph_arrow_pt_left,
	180
	);

static CExtPaintManager::glyph_t stat_glyph_arrow_small_up(
	5,3,2,
	stat_pixels_arrow_small_up
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_small_down(
	stat_glyph_arrow_small_up,
	180
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_small_left(
	stat_glyph_arrow_small_up,
	270
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_small_right(
	stat_glyph_arrow_small_left,
	180
	);
static CExtPaintManager::glyph_t stat_glyph_ellipsis(
	9,3,2,
	stat_pixels_ellipsis
	);

static CExtPaintManager::glyph_t stat_glyph_arrow_pt_lt(
	11,11,2,
	stat_pixels_arrow_pt_lt
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_pt_rt(
	stat_glyph_arrow_pt_lt,
	90
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_pt_rb(
	stat_glyph_arrow_pt_rt,
	90
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_pt_lb(
	stat_glyph_arrow_pt_rb,
	90
	);

/*
static const UINT stat_pixels_arrow_sort_up[] =
{
	0,0,0,1,1,0,0,0,
	0,0,1,3,1,1,0,0,
	0,0,3,0,0,1,0,0,
	0,1,3,0,0,1,1,0,
	0,3,0,0,0,0,1,0,
	1,3,0,0,0,0,1,1,
	1,2,2,2,2,2,2,2,
};

static const UINT stat_pixels_arrow_sort_down[] =
{
	2,3,3,3,3,3,3,3,
	2,2,0,0,0,0,1,1,
	0,2,0,0,0,0,3,0,
	0,2,2,0,0,1,1,0,
	0,0,2,0,0,3,0,0,
	0,0,2,2,1,1,0,0,
	0,0,0,2,1,0,0,0,
};
*/
static const UINT stat_pixels_arrow_sort_up[] =
{
	0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,1,1,0,0,0,0,
	0,0,0,1,3,1,1,0,0,0,
	0,0,0,3,0,0,1,0,0,0,
	0,0,1,1,0,0,1,1,0,0,
	0,0,3,0,0,0,0,1,0,0,
	0,1,1,0,0,0,0,1,1,0,
	0,3,0,0,0,0,0,0,1,0,
	1,1,0,0,0,0,0,0,1,1,
	1,2,2,2,2,2,2,2,2,2,
};

static const UINT stat_pixels_arrow_sort_down[] =
{
	3,3,3,3,3,3,3,3,3,2,
	1,1,0,0,0,0,0,0,2,2,
	0,3,0,0,0,0,0,0,2,0,
	0,1,1,0,0,0,0,2,2,0,
	0,0,3,0,0,0,0,2,0,0,
	0,0,1,1,0,0,2,2,0,0,
	0,0,0,3,0,0,2,0,0,0,
	0,0,0,1,3,2,2,0,0,0,
	0,0,0,0,1,2,0,0,0,0,
	0,0,0,0,1,2,0,0,0,0,
};

static CExtPaintManager::glyph_t stat_glyph_arrow_sort_up(
	10,10,4, // 8,7,4,
	stat_pixels_arrow_sort_up
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_sort_down(
	10,10,4, // 8,7,4,
	stat_pixels_arrow_sort_down
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_sort_left(
	stat_glyph_arrow_sort_down,
	90
	);
static CExtPaintManager::glyph_t stat_glyph_arrow_sort_right(
	stat_glyph_arrow_sort_up,
	90
	);

CExtPaintManager::glyph_t *
	CExtPaintManager::g_DockingCaptionGlyphs[__DCBT_VALUE_MAX+1] =
{
	& stat_glyph_dock_empty,
	& stat_glyph_dock_close,
	& stat_glyph_dock_arrow_up,
	& stat_glyph_dock_arrow_down,
	& stat_glyph_dock_arrow_left,
	& stat_glyph_dock_arrow_right,
	& stat_glyph_dock_wnd_minimize,
	& stat_glyph_dock_wnd_restore,
	& stat_glyph_dock_wnd_maximize,
	& stat_glyph_dock_wnd_contexthelp,
	& stat_glyph_dock_wnd_keep,
	& stat_glyph_dock_wnd_unkeep,
	& stat_glyph_dock_arrow_up_l,
	& stat_glyph_dock_arrow_down_l,
	& stat_glyph_dock_arrow_left_l,
	& stat_glyph_dock_arrow_right_l,
	& stat_glyph_dock_xp_thin_close,
	& stat_glyph_dock_hollow_up,
	& stat_glyph_dock_hollow_down,
	& stat_glyph_dock_hollow_left,
	& stat_glyph_dock_hollow_right,
	& stat_glyph_dock_close_dc2k,
	& stat_glyph_hollow_plus,
	& stat_glyph_hollow_minus,
	& stat_glyph_tree_plus,
	& stat_glyph_tree_minus,
	& stat_glyph_circle_plus,
	& stat_glyph_circle_minus,
	& stat_glyph_arrow_pt_up,
	& stat_glyph_arrow_pt_down,
	& stat_glyph_arrow_pt_left,
	& stat_glyph_arrow_pt_right,
	& stat_glyph_arrow_pt_lt,
	& stat_glyph_arrow_pt_rt,
	& stat_glyph_arrow_pt_rb,
	& stat_glyph_arrow_pt_lb,
	& stat_glyph_arrow_small_up,
	& stat_glyph_arrow_small_down,
	& stat_glyph_arrow_small_left,
	& stat_glyph_arrow_small_right,
	& stat_glyph_ellipsis,
	& stat_glyph_arrow_sort_up,
	& stat_glyph_arrow_sort_down,
	& stat_glyph_arrow_sort_left,
	& stat_glyph_arrow_sort_right,
};

CExtPaintManager::PAINTDOCKINGCAPTIONBUTTONDATA::PAINTDOCKINGCAPTIONBUTTONDATA()
	: m_pHelperSrc( NULL )
	, m_lParam( 0L )
	, m_rcClient( 0,0,0,0 )
	, m_eType( CExtPaintManager::e_docking_caption_button_t::__DCBT_EMPTY )
	, m_bHover( false )
	, m_bPushed( false )
	, m_bEnabled( false )
	, m_bSideBar( false )
	, m_bBarWndActive( false )
	, m_bFloating( false )
{
}

CExtPaintManager::PAINTDOCKINGCAPTIONBUTTONDATA::PAINTDOCKINGCAPTIONBUTTONDATA(
	CObject * pHelperSrc,
	const RECT & rcClient,
	int eType, // CExtPaintManager::e_docking_caption_button_t
	bool bHover,
	bool bPushed,
	bool bEnabled,
	bool bSideBar,
	bool bBarWndActive,
	bool bFloating
	)
	: m_pHelperSrc( pHelperSrc )
	, m_lParam( 0L )
	, m_rcClient( rcClient )
	, m_eType( eType )
	, m_bHover( bHover )
	, m_bPushed( bPushed )
	, m_bEnabled( bEnabled )
	, m_bSideBar( bSideBar )
	, m_bBarWndActive( bBarWndActive )
	, m_bFloating( bFloating )
{
}

void CExtPaintManager::PaintDockingCaptionButton(
	CDC & dc,
	CExtPaintManager::PAINTDOCKINGCAPTIONBUTTONDATA & _pdcbd
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT(
		__DCBT_VALUE_MIN <= _pdcbd.m_eType
		&&
		_pdcbd.m_eType <= __DCBT_VALUE_MAX
		);
	if( _pdcbd.m_rcClient.IsRectEmpty() )
		return;
bool bHover = _pdcbd.m_bHover;
	if( !_pdcbd.m_bEnabled )
		bHover = false;
COLORREF ColorValues[2] =
{
	RGB(0,0,0),
	RGB(0,0,0),
};

COLORREF clr3dFace = GetColor( COLOR_3DFACE );
COLORREF clr3dShadow = GetColor( COLOR_3DSHADOW );
COLORREF clr3dDkShadow = GetColor( COLOR_3DDKSHADOW );
COLORREF clr3dHiLight = GetColor( COLOR_3DHILIGHT );

	if( !_pdcbd.m_bEnabled )
		ColorValues[1] = clr3dShadow;
CRect rcGlyph( _pdcbd.m_rcClient );
	rcGlyph.NormalizeRect();
	if( rcGlyph.Width() & 1 )
		rcGlyph.left++;
	if( rcGlyph.Height() & 1 )
		rcGlyph.top++;
	if( _pdcbd.m_eType == __DCBT_CLOSE )
		_pdcbd.m_eType = __DCBT_CLOSE_DC2K;
glyph_t * pGlyph = g_DockingCaptionGlyphs[_pdcbd.m_eType];
	ASSERT( pGlyph != NULL );

	if( !_pdcbd.m_bFloating )
//		rcGlyph.InflateRect( 2, 2 );
//	else
		rcGlyph.InflateRect( 1, 1 );

	rcGlyph.InflateRect( 1, 1, 0, 0 );

	dc.FillSolidRect(
		&rcGlyph,
		clr3dFace
		);
	

CRect rcGlyphShape( rcGlyph );
	if( _pdcbd.m_bPushed )
		rcGlyphShape.OffsetRect( 1, 1 );
	PaintGlyphCentered(
		dc,
		rcGlyphShape,
		*pGlyph,
		ColorValues
		);
/*
	if( bEnabled && bPushed )
	{
		dc.Draw3dRect(
			&rcGlyph,
			clr3dDkShadow, clr3dFace
			);
		rcGlyph.DeflateRect( 1, 1 );
		dc.Draw3dRect(
			&rcGlyph,
			clr3dShadow, clr3dHiLight
			);
	} // if( bPushed )
	else
	{
		dc.Draw3dRect(
			&rcGlyph,
			clr3dFace, clr3dDkShadow
			);
		rcGlyph.DeflateRect( 1, 1 );
		dc.Draw3dRect(
			&rcGlyph,
			clr3dHiLight, clr3dShadow
			);
	} // else if( bHover )
*/


	if( _pdcbd.m_bEnabled && _pdcbd.m_bPushed )
	{
		dc.Draw3dRect(
			&rcGlyph,
			clr3dShadow, clr3dHiLight
			);
	} // if( _pdcbd.m_bEnabled && _pdcbd.m_bPushed )
	else if( _pdcbd.m_bEnabled && bHover )
	{
		dc.Draw3dRect(
			&rcGlyph,
			clr3dFace, clr3dDkShadow
			);
		rcGlyph.DeflateRect( 1, 1 );
		dc.Draw3dRect(
			&rcGlyph,
			clr3dHiLight, clr3dShadow
			);
	} // else if( _pdcbd.m_bEnabled && bHover )
	else
	{
		dc.Draw3dRect(
			&rcGlyph,
			clr3dHiLight, clr3dShadow
			);
	} // else ...
	
}

void CExtPaintManagerXP::PaintDockingCaptionButton(
	CDC & dc,
	CExtPaintManager::PAINTDOCKINGCAPTIONBUTTONDATA & _pdcbd
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT(
		__DCBT_VALUE_MIN <= _pdcbd.m_eType
		&&
		_pdcbd.m_eType <= __DCBT_VALUE_MAX
		);
	if( _pdcbd.m_rcClient.IsRectEmpty() )
		return;
bool bHover = _pdcbd.m_bHover;
	if( !_pdcbd.m_bEnabled )
		bHover = false;
COLORREF ColorValues[2] =
{
	RGB(0,0,0),
	RGB(0,0,0),
};
	if( _pdcbd.m_bEnabled )
	{
		if( ! (bHover || _pdcbd.m_bPushed) )
			ColorValues[1] =
				GetColor(
					_pdcbd.m_bSideBar ?
						(_pdcbd.m_bBarWndActive
							? COLOR_HIGHLIGHTTEXT
							: COLOR_3DDKSHADOW // COLOR_BTNTEXT
							)
						:
						COLOR_3DHILIGHT
					);
	} // if( bEnabled )
	else
	{
		ColorValues[1] =
			GetColor(
				COLOR_3DSHADOW // COLOR_3DFACE
				);
	} // else from if( bEnabled )
CRect rcGlyph( _pdcbd.m_rcClient );
	rcGlyph.NormalizeRect();
	if( bHover || _pdcbd.m_bPushed )
	{
		dc.FillSolidRect(
			&rcGlyph,
			//clr3dHilight
			GetColor(
				_pdcbd.m_bPushed
					? XPCLR_HILIGHT
					: CLR_3DFACE_IN
				)
			);
		CRect rcBtnBorder(rcGlyph);
		rcBtnBorder.InflateRect(1,1);
		COLORREF clrBtnBorder =
			GetColor(
				_pdcbd.m_bBarWndActive
					? XPCLR_HILIGHT
					: XPCLR_HILIGHT_BORDER
				);
		dc.Draw3dRect(
			rcBtnBorder,
			clrBtnBorder,
			clrBtnBorder
			);
	}
	if( rcGlyph.Width() & 1 )
		rcGlyph.left++;
	if( rcGlyph.Height() & 1 )
		rcGlyph.top++;

int eType = _pdcbd.m_eType;
	if( _pdcbd.m_bSideBar && !_pdcbd.m_bFloating && eType == __DCBT_CLOSE )
		eType = __DCBT_XP_THIN_CLOSE;

glyph_t * pGlyph = g_DockingCaptionGlyphs[eType];
	ASSERT( pGlyph != NULL );
	PaintGlyphCentered(
		dc,
		rcGlyph,
		*pGlyph,
		ColorValues
		);
}

CExtPaintManager::PAINTPUSHBUTTONDATA::PAINTPUSHBUTTONDATA()
	: m_pHelperSrc( NULL )
	, m_lParam( 0L )
	, m_bHorz( false )
	, m_rcClient( 0,0,0,0 )
	, m_sText( _T("") )
	, m_pIcon( NULL )
	, m_bFlat( false )
	, m_bHover( false )
	, m_bPushed( false )
	, m_bIndeterminate( false )
	, m_bEnabled( false )
	, m_bDrawBorder( false )
	, m_bDrawFocusRect( false )
	, m_bDefaultPushButton( false )
	, m_eAlign( __ALIGN_VERT )
	, m_hFont( NULL )
	, m_bDropDown( false )
	, m_nHelperSysCmdID( 0 )
	, m_bTransparentBackground( false )
	, m_bSeparatedDropDown( false )
	, m_bPushedDropDown( false )
	, m_clrForceTextDisabled( ((COLORREF)-1L) )
	, m_clrForceTextNormal( ((COLORREF)-1L) )
	, m_clrForceTextHover( ((COLORREF)-1L) )
	, m_clrForceTextPressed( ((COLORREF)-1L) )
	, m_nDrawTextFlagsH( DT_SINGLELINE|DT_CENTER|DT_VCENTER )
	, m_bForceUseDrawTextApiH( false )
{
}

CExtPaintManager::PAINTPUSHBUTTONDATA::PAINTPUSHBUTTONDATA(
	CObject * pHelperSrc,
	bool bHorz,
	const RECT & rcClient,
	__EXT_MFC_SAFE_LPCTSTR sText,
	CExtCmdIcon * pIcon,
	bool bFlat,
	bool bHover,
	bool bPushed,
	bool bIndeterminate,
	bool bEnabled,
	bool bDrawBorder,
	bool bDrawFocusRect,
	bool bDefaultPushButton,
	int eAlign,
	HFONT hFont, // = NULL
	bool bDropDown, // = false
	UINT nHelperSysCmdID, // = 0
	bool bTransparentBackground // = false
	)
	: m_pHelperSrc( pHelperSrc )
	, m_lParam( 0L )
	, m_bHorz( bHorz )
	, m_rcClient( rcClient )
	, m_sText( (sText==NULL) ? _T("") : sText )
	, m_pIcon( pIcon )
	, m_bFlat( bFlat )
	, m_bHover( bHover )
	, m_bPushed( bPushed )
	, m_bIndeterminate( bIndeterminate )
	, m_bEnabled( bEnabled )
	, m_bDrawBorder( bDrawBorder )
	, m_bDrawFocusRect( bDrawFocusRect )
	, m_bDefaultPushButton( bDefaultPushButton )
	, m_eAlign( eAlign )
	, m_hFont( hFont )
	, m_bDropDown( bDropDown )
	, m_nHelperSysCmdID( nHelperSysCmdID )
	, m_bTransparentBackground( bTransparentBackground )
	, m_bSeparatedDropDown( false )
	, m_bPushedDropDown( false )
	, m_clrForceTextDisabled( ((COLORREF)-1L) )
	, m_clrForceTextNormal( ((COLORREF)-1L) )
	, m_clrForceTextHover( ((COLORREF)-1L) )
	, m_clrForceTextPressed( ((COLORREF)-1L) )
	, m_nDrawTextFlagsH( DT_SINGLELINE|DT_CENTER|DT_VCENTER )
	, m_bForceUseDrawTextApiH( false )
{
}

void CExtPaintManager::PaintPushButton(
	CDC & dc,
	CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
	)
{
	if( _ppbd.m_rcClient.IsRectEmpty() )
		return;

	if( !_ppbd.m_bEnabled )
	{
		_ppbd.m_bHover = false;
		//_ppbd.m_bDrawBorder = false;
		_ppbd.m_bDrawFocusRect = false;
		_ppbd.m_bIndeterminate = false;
	}

	if( _ppbd.m_bSeparatedDropDown && (_ppbd.m_bPushed || _ppbd.m_bPushedDropDown) )
		_ppbd.m_bHover = false;

COLORREF clrOldText = dc.GetTextColor();
COLORREF clrOldBk = dc.GetBkColor();
int nOldBkMode = dc.SetBkMode( TRANSPARENT );

	// Prepare draw... paint button's area with background color
COLORREF bgColor;
	if(	_ppbd.m_bHover || _ppbd.m_bPushed )
		bgColor = GetColor(CLR_3DFACE_IN);
	else
		bgColor = GetColor(CLR_3DFACE_OUT);

CExtSafeString sTitle( _T("") );
	if( _ppbd.m_sText != NULL )
		sTitle = _ppbd.m_sText;

bool bPushedDropDown = false;
	if( (_ppbd.m_bPushed && !_ppbd.m_bSeparatedDropDown)
		|| _ppbd.m_bPushedDropDown
		)
		bPushedDropDown = true;

CRect rectClient( _ppbd.m_rcClient );
CRect rectTotal( _ppbd.m_rcClient );
CRect rectCaption( _ppbd.m_rcClient );
CRect rectDropDown( 0,0,0,0 );
	if( _ppbd.m_bDropDown )
	{
		rectDropDown = rectClient;
		if( _ppbd.m_bHorz )
		{
			rectCaption.right -=
				__DROPDOWN_ARROW_GAP*2
				+ g_glyph_btn_expand_bottom.Size().cx
				;
			rectDropDown.left = rectCaption.right;
		} // if( _ppbd.m_bHorz )
		else
		{
			rectCaption.bottom -=
				__DROPDOWN_ARROW_GAP*2
				+ g_glyph_btn_expand_right.Size().cy
				;
			rectDropDown.top = rectCaption.bottom;
		} // if( _ppbd.m_bHorz )

		if( !_ppbd.m_bTransparentBackground )
		{
			if(			_ppbd.m_bEnabled
					&&	bPushedDropDown // &&	_ppbd.m_bPushedDropDown
					&&	_ppbd.m_bHover
					//&&	_ppbd.m_bFlat
				)
				dc.FillRect( &rectDropDown, &m_brushLightDefault );
			else
			{
				CBrush br( bgColor );
				dc.FillRect( &rectDropDown, &br );
			}
		} // if( !_ppbd.m_bTransparentBackground )

		if( _ppbd.m_bSeparatedDropDown )
		{
			rectClient = rectCaption;

			if( bPushedDropDown )
			{
				
				if( _ppbd.m_bFlat )
				{
					if( _ppbd.m_bDrawBorder )
						dc.Draw3dRect(
							rectDropDown,
							GetColor(CLR_3DSHADOW_IN),
							GetColor(CLR_3DHILIGHT_IN)
							);
				} // if( _ppbd.m_bFlat )
				else    
					dc.Draw3dRect(
						rectDropDown,
						GetColor(CLR_3DSHADOW_IN),
						GetColor(CLR_3DHILIGHT_IN)
						);
			} // if( bPushedDropDown )
			else
			{

				if( _ppbd.m_bFlat )
				{
					if( _ppbd.m_bHover && _ppbd.m_bDrawBorder )
						dc.Draw3dRect(
							rectDropDown,
							GetColor(CLR_3DHILIGHT_OUT),
							GetColor(CLR_3DSHADOW_OUT)
							);
				} // if( _ppbd.m_bFlat )
				else
						dc.Draw3dRect(
							rectDropDown,
							GetColor(CLR_3DHILIGHT_OUT),
							GetColor(CLR_3DSHADOW_OUT)
							);
			} // else from if( bPushedDropDown )
		} // if( _ppbd.m_bSeparatedDropDown )
	} // if( _ppbd.m_bDropDown )

	if( !_ppbd.m_bTransparentBackground )
	{
		if(			_ppbd.m_bEnabled
				&&	_ppbd.m_bPushed
				&&	_ppbd.m_bHover
				//&&	_ppbd.m_bFlat
			)
			dc.FillRect( &rectClient, &m_brushLightDefault );
		else
		{
			CBrush br( bgColor );
			dc.FillRect( &rectClient, &br );
		}
	} // if( !_ppbd.m_bTransparentBackground )
	
	// Draw pressed button
	if( _ppbd.m_bPushed )
	{
			if( _ppbd.m_bFlat )
			{
				if( _ppbd.m_bDrawBorder )
					dc.Draw3dRect(
						rectClient,
						GetColor(CLR_3DSHADOW_IN),
						GetColor(CLR_3DHILIGHT_IN)
						);
			}
			else    
				dc.Draw3dRect(
					rectClient,
					GetColor(CLR_3DSHADOW_IN),
					GetColor(CLR_3DHILIGHT_IN)
					);
	} // if( bPushed )
	else // ...else draw non pressed button
	{
		if( _ppbd.m_bFlat )
		{
			if( _ppbd.m_bHover && _ppbd.m_bDrawBorder )
				dc.Draw3dRect(
					rectClient,
					GetColor(CLR_3DHILIGHT_OUT),
					GetColor(CLR_3DSHADOW_OUT)
					);
		} // if( _ppbd.m_bFlat )
		else
		{
			dc.Draw3dRect(
				rectClient,
				GetColor(CLR_3DHILIGHT_OUT),
				GetColor(CLR_3DSHADOW_OUT)
				);
			if( _ppbd.m_bHover )
			{
				CRect cc( rectTotal );
				dc.Draw3dRect(
					cc,
					GetColor(CLR_3DSHADOW_OUT),
					GetColor(CLR_3DDKSHADOW_OUT)
					);
				cc.DeflateRect(1,1);
				dc.Draw3dRect(
					cc,
					GetColor(CLR_3DHILIGHT_OUT),
					GetColor(CLR_3DSHADOW_OUT)
					);
			} // if( _ppbd.m_bHover )
		}  // else from if( _ppbd.m_bFlat )
	} // else from if( _ppbd.m_bPushed )

	if( _ppbd.m_bDropDown )
	{
		CRect rectGlyph( rectDropDown );
		if( bPushedDropDown )
			rectGlyph.OffsetRect( GetPushedOffset() );
		COLORREF ColorValues[2] =
		{
			RGB(0,0,0),
			GetColor(
				_ppbd.m_bEnabled
					? ( ( _ppbd.m_clrForceTextNormal == ((COLORREF)-1L) ) ? CLR_TEXT_OUT : _ppbd.m_clrForceTextNormal )
					: ( ( _ppbd.m_clrForceTextDisabled == ((COLORREF)-1L) ) ? CLR_3DSHADOW_OUT : _ppbd.m_clrForceTextDisabled )
				)
		};
		PaintGlyphCentered(
			dc,
			rectGlyph,
			_ppbd.m_bHorz ? g_glyph_btn_expand_bottom : g_glyph_btn_expand_right,
			ColorValues
			);
	} // if( _ppbd.m_bDropDown )

	rectClient = rectCaption;
	// Draw the icon
	if(	rectClient.bottom > rectClient.top
		&& rectClient.right > rectClient.left
		)
	{
		if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		{
			PaintIcon(
				dc,
				_ppbd.m_bHorz,
				sTitle,
				_ppbd.m_pIcon,
				rectClient,
				rectCaption,
				_ppbd.m_bPushed,
				_ppbd.m_bEnabled,
				_ppbd.m_bHover,
				_ppbd.m_eAlign
				);
		} // if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		else
		{
			PaintSysCmdGlyph(
				dc,
				_ppbd.m_nHelperSysCmdID,
				rectClient,
				false,
				_ppbd.m_bPushed,
				_ppbd.m_bEnabled
				);
		} // else from if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
	}

	if( rectCaption.bottom > rectCaption.top
		&& rectCaption.right > rectCaption.left
		)
	{ // if we have valid area for text & focus rect
		if( !sTitle.IsEmpty() )
		{
			if( _ppbd.m_bPushed )
			{
				CSize sizePushedOffset = GetPushedOffset();
				rectCaption.OffsetRect(sizePushedOffset);
			}
			dc.SetBkColor( bgColor );

			if( _ppbd.m_bEnabled )
			{
				if( _ppbd.m_bHover || _ppbd.m_bPushed )
				{
					if(			_ppbd.m_bPushed
							&&	_ppbd.m_clrForceTextPressed != ((COLORREF)-1L)
							)
							dc.SetTextColor( GetColor(_ppbd.m_clrForceTextPressed ) );
					else if(	_ppbd.m_bHover
							&&	_ppbd.m_clrForceTextHover != ((COLORREF)-1L)
							)
							dc.SetTextColor( GetColor(_ppbd.m_clrForceTextHover ) );
					else
							dc.SetTextColor( GetColor(COLOR_3DDKSHADOW) );
				} // if( _ppbd.m_bHover || _ppbd.m_bPushed )
				else 
					( _ppbd.m_clrForceTextNormal == ((COLORREF)-1L) )
						? GetColor( COLOR_BTNTEXT )
						: _ppbd.m_clrForceTextNormal
						;
			} // if( _ppbd.m_bEnabled )
			else
			{
				dc.SetTextColor(
					( _ppbd.m_clrForceTextDisabled == ((COLORREF)-1L) )
						? GetColor( COLOR_3DFACE )
						: _ppbd.m_clrForceTextDisabled
					);
			} // else from if( _ppbd.m_bEnabled )

/*
			// center text
			CRect centerRect = rectCaption;
*/

			CFont * pOldBtnFont = NULL;
			CFont * pCurrFont = NULL;
			CFont fontDummy;

			if( _ppbd.m_hFont != NULL )
			{
				if( _ppbd.m_bHorz )
					pCurrFont = CFont::FromHandle(_ppbd.m_hFont);
				else
				{
					LOGFONT lf;
					::memset(&lf,0,sizeof(LOGFONT));
					::GetObject(_ppbd.m_hFont, sizeof(LOGFONT), &lf);
					if( lf.lfEscapement == 0 )
					{
						lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT__;
						VERIFY(
							fontDummy.CreateFontIndirect(&lf)
							);
						pCurrFont = &fontDummy;
					}
					else
					{
						// suppose font already prepared
						pCurrFont = CFont::FromHandle( _ppbd.m_hFont );
					}
				}
			} // if( _ppbd.m_hFont != NULL )
			else
			{
				if( _ppbd.m_bHorz )
				{
					if( _ppbd.m_bDefaultPushButton )
						pCurrFont = &m_FontBold;
					else
						pCurrFont = &m_FontNormal;
				}
				else
				{
					if( _ppbd.m_bDefaultPushButton )
						pCurrFont = &m_FontBoldVert;
					else
						pCurrFont = &m_FontNormalVert;
				}
			} // else from if( _ppbd.m_hFont != NULL )
			ASSERT( pCurrFont != NULL );
			pOldBtnFont = dc.SelectObject( pCurrFont );
			ASSERT( pOldBtnFont != NULL );

			int nTextLength = sTitle.GetLength();
			int nAmpIndex = sTitle.Find( _T('&') );
			CExtSafeString sBtn;
			if( nAmpIndex < 0 )
				sBtn = sTitle;
			else
				sBtn =
					sTitle.Left(nAmpIndex)
					+ sTitle.Right( nTextLength - (nAmpIndex+1) )
					;
			ASSERT( sBtn.Find( _T('&') ) < 0 );

			CRect rcText(0,0,0,0);
			dc.DrawText(
				sBtn,
				-1,
				rcText,
				DT_SINGLELINE|DT_CALCRECT
				);
			CRect rcCenteredCaption( rectCaption );
			rcCenteredCaption.OffsetRect(
				( ( _ppbd.m_nDrawTextFlagsH & DT_CENTER ) != 0 )
					? ( ( rectCaption.Width() - rcText.Width() ) / 2 )
					: 0
					,
				( ( _ppbd.m_nDrawTextFlagsH & DT_VCENTER ) != 0 )
					? ( ( rectCaption.Height() - rcText.Height() ) / 2 )
					: 0
				);

			if( _ppbd.m_bHorz )
			{
				if( _ppbd.m_bForceUseDrawTextApiH )
				{
					dc.DrawText(
						sTitle,
						-1,
						rectCaption,
						_ppbd.m_nDrawTextFlagsH
						);
				} // if( _ppbd.m_bForceUseDrawTextApiH )
				else
				{
					dc.DrawState(
						rcCenteredCaption.TopLeft(),
						rcCenteredCaption.Size(),
						sTitle,
						( _ppbd.m_bEnabled || ( _ppbd.m_clrForceTextDisabled != ((COLORREF)-1L) ) )
							? DSS_NORMAL
							: m_DSS_DISABLED_style
							, 
						TRUE,
						0,
						(CBrush*)NULL
						);
				} // else from if( _ppbd.m_bForceUseDrawTextApiH )
			} // if( _ppbd.m_bHorz )
			else
			{
				LOGFONT lf;
				::memset(&lf,0,sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);
				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin =
					(rectCaption.Width() - _cyHorzFont)  / 2
					;

				CPoint
					ptLineFrom(0,0),
					ptLineTo(0,0);
				if( nAmpIndex >= 0 )
				{
					ptLineFrom =
						CPoint(
							__ExtMfc_CXTEXTMARGIN
								+ _cyTextMargin,
							__ExtMfc_CYTEXTMARGIN
								+ stat_CalcTextWidth(
									dc,
									_ppbd.m_bDefaultPushButton ?
										m_FontBold : m_FontNormal,
									sBtn.Left(nAmpIndex)
									)
							);
					ptLineTo =
						CPoint(
							__ExtMfc_CXTEXTMARGIN
								+ _cyTextMargin,
							__ExtMfc_CYTEXTMARGIN
								+ stat_CalcTextWidth(
									dc,
									_ppbd.m_bDefaultPushButton ?
										m_FontBold : m_FontNormal,
									sBtn.Left(nAmpIndex+1)
									)
							);
					int nXOffsPlus =
						rectCaption.Width()
						- stat_CalcTextDimension(
							dc,
							_ppbd.m_bDefaultPushButton ? m_FontBold : m_FontNormal,
							sBtn
							).Height();
					nXOffsPlus /= 2;
					nXOffsPlus -= 4;
					ptLineFrom.x += nXOffsPlus;
					ptLineTo.x   += nXOffsPlus;
				} // if( nAmpIndex >= 0 )

				CRect rcString = 
					CRect(
						CPoint(
							rectCaption.right - _cyTextMargin + __ExtMfc_CXTEXTMARGIN,
							rectCaption.top + __ExtMfc_CYTEXTMARGIN
							),
						rectCaption.Size() //m_sizeHorz
						);
				rcString.DeflateRect(2,2);

				CSize ptFix(-rcString.Width()-1,0);
				ptLineFrom += rcString.TopLeft() + ptFix;
				ptLineTo += rcString.TopLeft() + ptFix;
				if( _ppbd.m_bEnabled || ( _ppbd.m_clrForceTextDisabled != ((COLORREF)-1L) ) )
				{
					dc.DrawText(
						sBtn,
						sBtn.GetLength(),
						rcString,
						DT_SINGLELINE|DT_NOCLIP|DT_NOPREFIX
						);
					if( nAmpIndex >= 0 )
					{
						CPen pen(
							PS_SOLID,
							0,
							dc.GetTextColor()
							);
						CPen * pOldPen = dc.SelectObject( &pen );
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
						dc.SelectObject( pOldPen );
					} // if( nAmpIndex >= 0 )
				} // if( _ppbd.m_bEnabled || ( _ppbd.m_clrForceTextDisabled != ((COLORREF)-1L) ) )
				else
				{
					CPoint ptOffsetPre(1,1);
					CPoint ptOffsetPost(-1,-1);
					COLORREF clrText = GetColor(COLOR_3DSHADOW);
					COLORREF clrShadow = GetColor(COLOR_3DHILIGHT);
					rcString.OffsetRect(ptOffsetPre.x,ptOffsetPre.y);
					dc.SetTextColor( clrShadow );
					dc.DrawText(
						sBtn,
						sBtn.GetLength(),
						rcString,
						DT_SINGLELINE //|DT_CENTER|DT_VCENTER
							|DT_NOCLIP|DT_NOPREFIX
						); // don't forget DT_NOCLIP
					rcString.OffsetRect(ptOffsetPost.x,ptOffsetPost.y);
					dc.SetTextColor( clrText );
					dc.DrawText(
						sBtn,
						sBtn.GetLength(),
						rcString,
						DT_SINGLELINE //|DT_CENTER|DT_VCENTER
							|DT_NOCLIP|DT_NOPREFIX
						); // don't forget DT_NOCLIP
					if( nAmpIndex >= 0 )
					{
						CPen pen(
							PS_SOLID,
							0,
							clrShadow
							);
						CPen * pOldPen = dc.SelectObject( &pen );
						ptLineFrom += ptOffsetPre;
						ptLineTo += ptOffsetPre;
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
						pen.DeleteObject();
						pen.CreatePen(
							PS_SOLID,
							0,
							clrText
							);
						dc.SelectObject( &pen );
						ptLineFrom += ptOffsetPost;
						ptLineTo += ptOffsetPost;
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
						dc.SelectObject( pOldPen );
					} // if( nAmpIndex >= 0 )
				} // else from if( _ppbd.m_bEnabled || ( _ppbd.m_clrForceTextDisabled != ((COLORREF)-1L) ) )
			} // else from if( _ppbd.m_bHorz )

			dc.SelectObject(pOldBtnFont);
		} // if( !sTitle.IsEmpty() )

		if( _ppbd.m_bDrawFocusRect )
		{
			CRect focusRect = rectCaption; // rectClient
			focusRect.DeflateRect( 3, 3 );
			dc.DrawFocusRect(&focusRect);
		} // if( _ppbd.m_bDrawFocusRect )
	} // if we have valid area for text & focus rect


	dc.SetBkMode( nOldBkMode );
	dc.SetBkColor( clrOldBk );
	dc.SetTextColor( clrOldText );
}

CSize CExtPaintManager::GetDropDividerMerics()
{
	ASSERT_VALID( this );
	return CSize( 6, 6 );
}

void CExtPaintManager::PaintDropDivider(
	CDC & dc,
	const RECT & rcItem,
	bool bBefore,
	bool bHorz
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
CRect rc( rcItem );
CSize _sizeDDM = GetDropDividerMerics();
COLORREF clr = GetColor( COLOR_BTNTEXT );
	if( bHorz )
	{
		rc.InflateRect( 0, 1 );
		if( bBefore )
		{
			rc.right = rc.left + 1;
			rc.left--;
		} // if( bBefore )
		else
		{
			rc.left = rc.right - 1;
			rc.right++;
		} // else from if( bBefore )
		dc.FillSolidRect( &rc, clr );
		CRect rc2( rc );
		rc2.bottom = rc2.top + 1;
		rc2.InflateRect( 2, 0 );
		dc.FillSolidRect( &rc2, clr );
		rc2.DeflateRect( 1, 0 );
		rc2.bottom++;
		dc.FillSolidRect( &rc2, clr );
		rc2 = rc;
		rc2.top = rc2.bottom - 1;
		rc2.InflateRect( 2, 0 );
		dc.FillSolidRect( &rc2, clr );
		rc2.DeflateRect( 1, 0 );
		rc2.top--;
		dc.FillSolidRect( &rc2, clr );
	} // if( bHorz )
	else
	{
		rc.InflateRect( 1, 0 );
		if( bBefore )
		{
			rc.bottom = rc.top + 1;
			rc.top--;
		} // if( bBefore )
		else
		{
			rc.top = rc.bottom - 1;
			rc.bottom++;
			rc.right--;
		} // else from if( bBefore )
		dc.FillSolidRect( &rc, clr );
		CRect rc2( rc );
		rc2.right = rc2.left + 1;
		rc2.InflateRect( 0, 2 );
		dc.FillSolidRect( &rc2, clr );
		rc2.DeflateRect( 0, 1 );
		rc2.right++;
		dc.FillSolidRect( &rc2, clr );
		rc2 = rc;
		rc2.left = rc2.right - 1;
		rc2.InflateRect( 0, 2 );
		dc.FillSolidRect( &rc2, clr );
		rc2.DeflateRect( 0, 1 );
		rc2.left--;
		dc.FillSolidRect( &rc2, clr );
	} // else from if( bHorz )
}

void CExtPaintManager::PaintDragSourceRect(
	CDC & dc,
	const RECT & rcItem
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( !dc.RectVisible(&rcItem) )
		return;
COLORREF clr = GetColor( COLOR_BTNTEXT );
CRect rc( rcItem );
	dc.Draw3dRect( &rc, clr, clr );
	rc.DeflateRect( 1, 1 );
	dc.Draw3dRect( &rc, clr, clr );
}

CRect CExtPaintManager::stat_CalcTextDimension(
	CDC & dc,
	CFont & font,
	const CExtSafeString & strText
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( font.GetSafeHandle() != NULL );
CRect rcText(0,0,0,0);
CFont * pOldFont = dc.SelectObject(
		&font
		);
	ASSERT( pOldFont != NULL );
	dc.DrawText(
		strText,
		strText.GetLength(),
		&rcText,
		DT_SINGLELINE|DT_LEFT|DT_CALCRECT
		);
	dc.SelectObject(pOldFont);
	rcText.OffsetRect( -rcText.TopLeft() );
	return rcText;
}

int CExtPaintManager::stat_CalcTextWidth(
	CDC & dc,
	CFont & font,
	const CExtSafeString & strText
	)
{
	return
		CExtPaintManager::stat_CalcTextDimension(
			dc,
			font,
			strText
			).Width();
}

void CExtPaintManager::PaintMenuSeparator(
	CDC & dc,
	const RECT & rectItem,
	int nIconAreaWidth,
	bool bRarelyUsed,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
	nIconAreaWidth;
	if( ::IsRectEmpty(&rectItem) )
		return;
CRect rectItem2(rectItem);
	if( bRarelyUsed )
	{
		rectItem2.InflateRect(3,0,4,0);
		dc.FillRect(
			&rectItem2,
			&m_brushLightDefault
			);
		rectItem2.DeflateRect(3,0,4,0);
	}
	else
		dc.FillSolidRect(
			&rectItem2,
			GetColor(CLR_3DFACE_OUT)
			);

int nFunnyDeflateMetric =
		::GetSystemMetrics(SM_CXSMICON) / 2;
	rectItem2.DeflateRect(
		nFunnyDeflateMetric,
		0
		);

COLORREF clrBtnShadow =
		GetColor(
			bRarelyUsed ?
				CLR_3DFACE_OUT : CLR_3DSHADOW_OUT
			);
COLORREF clrBtnHilight =
		GetColor(CLR_3DHILIGHT_OUT);
int y = (rectItem2.top+rectItem2.bottom)/2;
rectItem2.top = y-1; rectItem2.bottom = y+1;
	dc.Draw3dRect(
		rectItem2,
		clrBtnShadow,
		clrBtnHilight
		);
}

void CExtPaintManagerXP::PaintMenuSeparator(
	CDC & dc,
	const RECT & rectItem,
	int nIconAreaWidth,
	bool bRarelyUsed,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
	nIconAreaWidth;
	if( ::IsRectEmpty(&rectItem) )
		return;
COLORREF clr3dFace = // GetColor(CLR_3DFACE_OUT);
		GetColor(
			bRarelyUsed ?
				XPCLR_RARELY_BORDER : CLR_3DFACE_OUT
		);
CRect rc( rectItem ), rcBottomFill( rectItem );
	rcBottomFill.right = rcBottomFill.left + nIconAreaWidth;
	dc.FillSolidRect( rcBottomFill, clr3dFace );

COLORREF clrGradientLeft = ((COLORREF)(-1L)), clrGradientRight((COLORREF)(-1L));
bool b2StepGradient = false;
	if( bRarelyUsed )
	{
		if(		m_nIdxClrMlaRarelyLeft >= 0
			&&	m_nIdxClrMlaRarelyRight >= 0
			&&	stat_GetBPP() > 8
			)
		{ // if paint gradient on rarely used item's left part
			clrGradientLeft = GetColor( m_nIdxClrMlaRarelyLeft );
			clrGradientRight = GetColor( m_nIdxClrMlaRarelyRight );
		} // if paint gradient on rarely used item's left part
	} // if( bRarelyUsed )
	else
	{
		if(	(!(		g_PaintManager.m_bUxApiInitPassed
				&&	g_PaintManager.m_bUxValidColorsExtracted
				&&	g_PaintManager.m_bUxUseIfAvailOnWinXpOrLater
			))
			)
			b2StepGradient = true;
		if(		m_nIdxClrMlaNormLeft >= 0
			&&	m_nIdxClrMlaNormRight >= 0
			&&	stat_GetBPP() > 8
			)
		{ // if paint gradient on rarely used item's left part
			clrGradientLeft = GetColor( m_nIdxClrMlaNormLeft );
			clrGradientRight = GetColor( m_nIdxClrMlaNormRight );
		} // if paint gradient on rarely used item's left part
	} // else from if( bRarelyUsed )
	if( clrGradientLeft != ((COLORREF)(-1L)) )
	{
		ASSERT( clrGradientRight != ((COLORREF)(-1L)) );
		CRect rcGradient( rectItem );
		rcGradient.right = rcGradient.left + nIconAreaWidth;
		if( b2StepGradient )
		{
			CRect
				rcGradient1( rcGradient ),
				rcGradient2( rcGradient );
			rcGradient1.right =
				rcGradient1.left
				+ ::MulDiv( rcGradient1.Width(), 7, 10 );
			rcGradient2.left = rcGradient1.right;
			dc.FillSolidRect(
				&rcGradient2,
				clrGradientRight
				);
			stat_PaintGradientRect(
				dc,
				&rcGradient1,
				clrGradientLeft,
				clrGradientRight,
				false
				);
		} // if( b2StepGradient )
		else
			stat_PaintGradientRect(
				dc,
				&rcGradient,
				clrGradientLeft,
				clrGradientRight,
				false
				);
	} // if( clrGradientLeft != ((COLORREF)(-1L)) )

	rc.left += nIconAreaWidth;
	dc.FillSolidRect( rc, GetColor(XPCLR_3DFACE_NORMAL) );
	rc.left += __EXT_MENU_GAP;
	rc.top += rc.Height()/2;
	rc.bottom = rc.top+1;
	dc.FillSolidRect(
		rc,
		clr3dFace
		);
}

void CExtPaintManager::PaintSysCmdGlyph(
	CDC & dc,
	UINT nHelperSysCmdID,
	const RECT & rcItem,
	bool bHover,
	bool bPushed,
	bool bEnabled
	)
{
e_docking_caption_button_t eGlyph = __DCBT_EMPTY;
	switch( nHelperSysCmdID )
	{
	case SC_CLOSE:
		eGlyph = __DCBT_CLOSE;
	break;
	case SC_RESTORE:
		eGlyph = __DCBT_WND_RESTORE;
	break;
	case SC_MINIMIZE:
		eGlyph = __DCBT_WND_MINIMIZE;
	break;
	case SC_MAXIMIZE:
		eGlyph = __DCBT_WND_MAXIMIZE;
	break;
	case SC_CONTEXTHELP:
		eGlyph = __DCBT_WND_CONTEXTHELP;
	break;
	} // switch( nHelperSysCmdID )
	if( eGlyph == __DCBT_EMPTY )
		return;
COLORREF ColorValues[2] = {
	RGB(0,0,0),
	RGB(0,0,0),
};
CRect _rcItem( rcItem );
	if( bPushed )
		_rcItem.OffsetRect(
			GetPushedOffset()
			);
	if( bEnabled )
	{
		ColorValues[1] =
			GetColor(
				bHover ?
					COLOR_HIGHLIGHTTEXT : CLR_TEXT_OUT
				);
	} // if( bEnabled )
	else
	{
		ColorValues[1] =
			GetColor( CLR_3DHILIGHT_OUT );
		_rcItem.OffsetRect(1,1);
		PaintGlyphCentered(
			dc,
			_rcItem,
			*g_DockingCaptionGlyphs[eGlyph],
			ColorValues
			);
		_rcItem.OffsetRect( -1, -1 );
		ColorValues[1] =
			GetColor( CLR_3DSHADOW_OUT );
	} // if( bEnabled )
	PaintGlyphCentered(
		dc,
		_rcItem,
		*g_DockingCaptionGlyphs[eGlyph],
		ColorValues
		);
}

void CExtPaintManagerXP::PaintSysCmdGlyph(
	CDC & dc,
	UINT nHelperSysCmdID,
	const RECT & rcItem,
	bool bHover,
	bool bPushed,
	bool bEnabled
	)
{
e_docking_caption_button_t eGlyph = __DCBT_EMPTY;
	switch( nHelperSysCmdID )
	{
	case SC_CLOSE:
		eGlyph = __DCBT_CLOSE;
	break;
	case SC_RESTORE:
		eGlyph = __DCBT_WND_RESTORE;
	break;
	case SC_MINIMIZE:
		eGlyph = __DCBT_WND_MINIMIZE;
	break;
	case SC_MAXIMIZE:
		eGlyph = __DCBT_WND_MAXIMIZE;
	break;
	case SC_CONTEXTHELP:
		eGlyph = __DCBT_WND_CONTEXTHELP;
	break;
	} // switch( nHelperSysCmdID )
	if( eGlyph == __DCBT_EMPTY )
		return;
COLORREF ColorValues[2] = {
	RGB(0,0,0),
	RGB(0,0,0),
};
CRect _rcItem( rcItem );
	if( bPushed )
		_rcItem.OffsetRect(
			GetPushedOffset()
			);
	if( bEnabled )
	{
		if( m_bEnabledHoverIconShadows && bHover && (!bPushed) )
		{
			// COLOR_3DDKSHADOW changed to COLOR_3DSHADOW
			// by the Dmitry Yakovlev's advice
			ColorValues[1] = GetColor(
				COLOR_3DSHADOW // COLOR_3DDKSHADOW
				);
			PaintGlyphCentered(
				dc,
				_rcItem,
				*g_DockingCaptionGlyphs[eGlyph],
				ColorValues
				);
			// corrected by the Dmitry Yakovlev's advice
			_rcItem.OffsetRect( -2, -2 );
		} // if( m_bEnabledHoverIconShadows && bHover && (!bPushed) )
		ColorValues[1] = GetColor( CLR_TEXT_OUT );
	} // if( bEnabled )
	else
		ColorValues[1] = GetColor( COLOR_3DSHADOW );
	PaintGlyphCentered(
		dc,
		_rcItem,
		*g_DockingCaptionGlyphs[eGlyph],
		ColorValues
		);
}

CExtPaintManager::PAINTMENUITEMDATA::PAINTMENUITEMDATA()
	: m_pHelperSrc( NULL )
	, m_lParam( 0L )
	, m_rcItem( 0, 0, 0, 0 )
	, m_nIconAreaWidth( 0 )
	, m_sText( _T("") )
	, m_sAccelText( _T("") )
	, m_pIcon( NULL )
	, m_bPopup( false )
	, m_bSelected( false )
	, m_bPushedCheck( false )
	, m_bPushedRadio( false )
	, m_bIndeterminate( false )
	, m_bEnabled( false )
	, m_bRarelyUsed( false )
	, m_bRarelyUsedPrev( false )
	, m_bRarelyUsedNext( false )
	, m_nHelperSysCmdID( 0L )
	, m_bForceNoLeftGradient( false )
	, m_bCustomizeListMode( false )
	, m_bSeparatedDropDown( false )
	, m_rcInPlaceWnd( 0, 0, 0, 0 )
	, m_strInPlaceText( _T("") )
	, m_bXtraMark( false )
	, m_bXtraChecked( false )
	, m_bHelperPopupDropped( false )
{
}

CExtPaintManager::PAINTMENUITEMDATA::PAINTMENUITEMDATA(
	CObject * pHelperSrc,
	const RECT & rcItem,
	INT nIconAreaWidth,
	__EXT_MFC_SAFE_LPCTSTR sText,
	__EXT_MFC_SAFE_LPCTSTR sAccelText,
	CExtCmdIcon * pIcon,
	bool bPopup,
	bool bSelected,
	bool bPushedCheck,
	bool bPushedRadio,
	bool bIndeterminate,
	bool bEnabled,
	bool bRarelyUsed,
	bool bRarelyUsedPrev,
	bool bRarelyUsedNext,
	UINT nHelperSysCmdID // for painting system menu glyphs
	)
	: m_pHelperSrc( pHelperSrc )
	, m_lParam( 0L )
	, m_rcItem( rcItem )
	, m_nIconAreaWidth( nIconAreaWidth )
	, m_sText( (sText == NULL) ? _T("") : sText )
	, m_sAccelText( (sAccelText == NULL) ? _T("") : sAccelText )
	, m_pIcon( pIcon )
	, m_bPopup( bPopup )
	, m_bSelected( bSelected )
	, m_bPushedCheck( bPushedCheck )
	, m_bPushedRadio( bPushedRadio )
	, m_bIndeterminate( bIndeterminate )
	, m_bEnabled( bEnabled )
	, m_bRarelyUsed( bRarelyUsed )
	, m_bRarelyUsedPrev( bRarelyUsedPrev )
	, m_bRarelyUsedNext( bRarelyUsedNext )
	, m_nHelperSysCmdID( nHelperSysCmdID )
	, m_bForceNoLeftGradient( false )
	, m_bCustomizeListMode( false )
	, m_bSeparatedDropDown( false )
	, m_rcInPlaceWnd( 0, 0, 0, 0 )
	, m_strInPlaceText( _T("") )
	, m_bXtraMark( false )
	, m_bXtraChecked( false )
	, m_bHelperPopupDropped( false )
{
}

void CExtPaintManager::PaintMenuItem(
	CDC & dc,
	CExtPaintManager::PAINTMENUITEMDATA & _pmid
	)
{
	if( _pmid.m_rcItem.IsRectEmpty() )
		return;
CExtSafeString _sText =
		(_pmid.m_sText==NULL) ? _T("") : _pmid.m_sText;
CExtSafeString _sAccelText =
		(_pmid.m_sAccelText==NULL) ? _T("") : _pmid.m_sAccelText;

COLORREF clr3dFace = GetColor( CLR_3DFACE_OUT );
COLORREF clrBk = clr3dFace;
COLORREF clrText = GetColor(CLR_TEXT_OUT);
CRect rcBk(_pmid.m_rcItem);
	dc.FillSolidRect(rcBk,clrBk);
	if( _pmid.m_bSelected )
	{
		ASSERT( _pmid.m_bEnabled );
		clrBk = GetColor(COLOR_HIGHLIGHT);
		clrText = GetColor(COLOR_HIGHLIGHTTEXT);
	}
	//else
	{
		if( _pmid.m_bRarelyUsed )
		{
			rcBk.InflateRect(3,0,4,0);
			dc.FillRect(rcBk,&m_brushLightDefault);
			if( !_pmid.m_bRarelyUsedPrev )
			{
				CPen penShadow(
					PS_SOLID,
					1,
					GetColor(COLOR_3DSHADOW)
					);
				CPen * pOldPen =
					dc.SelectObject( &penShadow );
				ASSERT( pOldPen != NULL );
				dc.MoveTo(rcBk.left,rcBk.top);
				dc.LineTo(rcBk.right,rcBk.top);
				dc.SelectObject( pOldPen );
			}
			if( !_pmid.m_bRarelyUsedNext )
			{
				CPen penLight(
					PS_SOLID,
					1,
					GetColor(COLOR_3DHIGHLIGHT)
					);
				CPen * pOldPen =
					dc.SelectObject( &penLight );
				ASSERT( pOldPen != NULL );
				dc.MoveTo(rcBk.left,rcBk.bottom-1);
				dc.LineTo(rcBk.right,rcBk.bottom-1);
				dc.SelectObject( pOldPen );
			}
			rcBk.DeflateRect(3,0,4,0);
		} // if( _pmid.m_bRarelyUsed )
	}

bool bSmallerSelRect = 
		(		(_pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()))
			||	_pmid.m_bPushedCheck
			||	_pmid.m_bPushedRadio
			) ? true : false;
	if( bSmallerSelRect )
		rcBk.left += _pmid.m_nIconAreaWidth;
	if( _pmid.m_bSelected || (!_pmid.m_bRarelyUsed) )
		dc.FillSolidRect(rcBk,clrBk);
	if( !bSmallerSelRect )
		rcBk.left += _pmid.m_nIconAreaWidth;
	rcBk.DeflateRect(__EXT_MENU_GAP,__EXT_MENU_GAP);
COLORREF clrOldText = dc.SetTextColor(clrText);
int nOldBkMode = dc.SetBkMode(TRANSPARENT);

	rcBk.DeflateRect( __EXT_MENU_GAP, 0 );
CFont * pOldFont = dc.SelectObject(&m_FontNormal);
	ASSERT( pOldFont != NULL );

CRect rcMeasureText( 0, 0, 0, 0 );
	dc.DrawText(
		_sText,
		_sText.GetLength(),
		&rcMeasureText,
		DT_CALCRECT|DT_SINGLELINE|DT_LEFT
		);
CRect rcDrawText( rcBk );
	rcDrawText.OffsetRect(
		0,
		( rcDrawText.Height() - rcMeasureText.Height() ) / 2
		);
	dc.DrawState(
		rcDrawText.TopLeft(),
		rcDrawText.Size(),
		_sText,
		(_pmid.m_bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
		TRUE,
		0,
		(CBrush*)NULL
		);
	if( !_sAccelText.IsEmpty() )
	{
		rcMeasureText.SetRect( 0, 0, 0, 0 );
		dc.DrawText(
			_sAccelText,
			_sAccelText.GetLength(),
			&rcMeasureText,
			DT_CALCRECT|DT_SINGLELINE|DT_LEFT
			);
		rcDrawText = rcBk;
		rcDrawText.OffsetRect(
			0,
			( rcDrawText.Height() - rcMeasureText.Height() ) / 2
			);
		rcDrawText.left =
			rcDrawText.right
			- rcMeasureText.Width()
			- __EXT_MENU_POPUP_ARROW_AREA_DX
			;
		rcBk.left = rcDrawText.left;
		dc.DrawState(
			rcDrawText.TopLeft(),
			rcDrawText.Size(),
			_sAccelText,
			(_pmid.m_bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
			TRUE,
			0,
			(CBrush*)NULL
			);
	} // if( !_sAccelText.IsEmpty() )

	if( _pmid.m_bPopup )
	{
		CRect rcPopupArrow(_pmid.m_rcItem);
		rcPopupArrow.DeflateRect(__EXT_MENU_GAP,__EXT_MENU_GAP);
		rcPopupArrow.left =
			rcPopupArrow.right - __EXT_MENU_POPUP_ARROW_AREA_DX;
		if( _pmid.m_bCustomizeListMode )
		{
			COLORREF ColorValues[2] =
			{
				RGB(0,0,0),
				GetColor( _pmid.m_bSelected ? COLOR_HIGHLIGHTTEXT : CLR_TEXT_OUT )
			};
			PaintGlyphCentered(
				dc,
				rcPopupArrow,
				_pmid.m_bSeparatedDropDown
					? g_glyph_customize_dropdown_src
					: g_glyph_customize_popup_src,
				ColorValues
				);
		} // if( _pmid.m_bCustomizeListMode )
		else
		{
			dc.SelectObject(&m_FontMarlett);
			dc.DrawText(
				_T("8"), // popup rightArrow
				rcPopupArrow,
				DT_CENTER|DT_VCENTER|DT_SINGLELINE
				);
		} // else from if( _pmid.m_bCustomizeListMode )
	} // if( _pmid.m_bPopup )
	
	dc.SelectObject(&pOldFont);
	dc.SetBkMode(nOldBkMode);
	dc.SetTextColor(clrOldText);

CSize sizeIcon( __EXT_MENU_ICON_CX, __EXT_MENU_ICON_CY );
	if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )
	{
		sizeIcon = _pmid.m_pIcon->GetSize();
		if(	sizeIcon.cx <= 18 && sizeIcon.cy < sizeIcon.cx )
			sizeIcon.cy = sizeIcon.cx;
	}
CRect rcIcon( _pmid.m_rcItem.TopLeft(), sizeIcon );
	if( _pmid.m_bXtraMark )
	{
		rcIcon.OffsetRect(
			( _pmid.m_nIconAreaWidth/2 - rcIcon.Width() ) / 2 + _pmid.m_nIconAreaWidth/2 - 2,
			( _pmid.m_rcItem.Height() - rcIcon.Height() ) / 2
			);
		if( _pmid.m_bXtraChecked )
		{
			CRect rcXtraMark(
				_pmid.m_rcItem.left+1,
				_pmid.m_rcItem.top+1,
				_pmid.m_rcItem.left+_pmid.m_rcItem.Height()-2,
				_pmid.m_rcItem.bottom-2
				);
			dc.FillRect(
				rcXtraMark,
				&m_brushLightDefault
				);
			COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
			int x = rcXtraMark.left + rcXtraMark.Width()/2 - 3;
			int y = rcXtraMark.top + rcXtraMark.Height()/2 - 3;
			ASSERT( !_pmid.m_bPushedRadio );
			stat_PaintCheckMark(
				dc,
				x,y,
				clrCheck
				);
			dc.Draw3dRect(
				rcXtraMark,
				GetColor(CLR_3DSHADOW_OUT),
				GetColor(CLR_3DHILIGHT_OUT)
				);
		} // if( _pmid.m_bXtraChecked )
	} // if( _pmid.m_bXtraMark )
	else
	{
		rcIcon.OffsetRect(
			( _pmid.m_nIconAreaWidth - rcIcon.Width() ) / 2,
			( _pmid.m_rcItem.Height() - rcIcon.Height() ) / 2
			);
	} // else from if( _pmid.m_bXtraMark )

CRect rcIconFrame(rcIcon);
	rcIconFrame.InflateRect(__EXT_MENU_IMG_RECT_OUT_GAP,__EXT_MENU_IMG_RECT_OUT_GAP);
	if( _pmid.m_bPushedCheck || _pmid.m_bPushedRadio )
		dc.FillRect(
			rcIconFrame,
			&m_brushLightDefault
			);
	if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )
	{
//		dc.DrawState(
//			rcIcon.TopLeft(),
//			rcIcon.Size(), 
//			_pmid.m_pIcon->GetIcon(), 
//			(_pmid.m_bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
//			(CBrush*)NULL
//			);
		_pmid.m_pIcon->Paint(
			dc,
			rcIcon.TopLeft(),
			_pmid.m_bEnabled
				? CExtCmdIcon::__DRAW_NORMAL
				: CExtCmdIcon::__DRAW_DISABLED
			);
		if( _pmid.m_bSelected
			&& _pmid.m_bEnabled
			&& (!(_pmid.m_bPushedCheck || _pmid.m_bPushedRadio))
			)
			dc.Draw3dRect(
				&rcIconFrame,
				GetColor(CLR_3DHILIGHT_OUT),
				GetColor(CLR_3DSHADOW_OUT)
				);
	} // if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )
	else
	{
		PaintSysCmdGlyph(
			dc,
			_pmid.m_nHelperSysCmdID,
			rcIcon,
			_pmid.m_bSelected,
			false,
			_pmid.m_bEnabled
			);
	} // else from if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )

	if( _pmid.m_bPushedCheck || _pmid.m_bPushedRadio )
	{
		if( _pmid.m_pIcon == NULL || _pmid.m_pIcon->IsEmpty() )
		{
			COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
			int x = rcIcon.left + rcIcon.Width()/2 - 3;
			int y = rcIcon.top + rcIcon.Height()/2 - 3;
			if( _pmid.m_bPushedCheck )
			{
				ASSERT( !_pmid.m_bPushedRadio );
				stat_PaintCheckMark(
					dc,
					x,y,
					clrCheck
					);
			}
			else
			{
				ASSERT( !_pmid.m_bPushedCheck );
				stat_PaintRadioDot(
					dc,
					x,y,
					clrCheck
					);
			}
		} // if( _pmid.m_pIcon == NULL || _pmid.m_pIcon->IsEmpty() )
		dc.Draw3dRect(
			rcIconFrame,
			GetColor(CLR_3DSHADOW_OUT),
			GetColor(CLR_3DHILIGHT_OUT)
			);
	}

	if( !_pmid.m_rcInPlaceWnd.IsRectEmpty() )
	{
		CRect rcInPlaceWndOuterBorder( _pmid.m_rcInPlaceWnd );
		rcInPlaceWndOuterBorder.InflateRect( 2, 2, 2, 3 );
		if( _pmid.m_bPopup )
		{
			INT nDD = (INT) ::GetSystemMetrics(SM_CXHTHUMB);
			rcInPlaceWndOuterBorder.right += nDD;
//			rcInPlaceWndOuterBorder.InflateRect( 1, 1 );
			PAINTCOMBOFRAMEDATA _pcfd(
				_pmid.m_pHelperSrc,
				rcInPlaceWndOuterBorder,
				_pmid.m_bSelected, // hover
				_pmid.m_bSelected && _pmid.m_bHelperPopupDropped, // pushed
				_pmid.m_bEnabled
				);
			_pcfd.m_bNoTopOuterBorder = true;
			PaintComboFrame( dc, _pcfd );
			rcInPlaceWndOuterBorder.DeflateRect( 1, 1 );
			rcInPlaceWndOuterBorder.right -= nDD;
		} // if( _pmid.m_bPopup )
		else
		{
//			dc.Draw3dRect(
//				rcInPlaceWndOuterBorder,
//				GetColor(COLOR_3DSHADOW),
//				GetColor(COLOR_3DHIGHLIGHT)
//				);
//			rcInPlaceWndOuterBorder.DeflateRect( 1, 1 );
//			dc.Draw3dRect(
//				rcInPlaceWndOuterBorder,
//				GetColor(COLOR_3DDKSHADOW),
//				GetColor(COLOR_3DFACE)
//				);
			COLORREF clrFace = GetColor( COLOR_3DFACE );
			dc.Draw3dRect(
				rcInPlaceWndOuterBorder,
				clrFace,
				clrFace
				);
			rcInPlaceWndOuterBorder.DeflateRect( 1, 1 );
			if( _pmid.m_bSelected )
				dc.Draw3dRect(
					rcInPlaceWndOuterBorder,
					GetColor(COLOR_3DSHADOW),
					GetColor(COLOR_3DHIGHLIGHT)
					);
			else
				dc.Draw3dRect(
					rcInPlaceWndOuterBorder,
					clrFace,
					clrFace
					);
		} // else from if( _pmid.m_bPopup )
		rcInPlaceWndOuterBorder.DeflateRect( 1, 1 );
		dc.FillSolidRect(
			&rcInPlaceWndOuterBorder,
			GetColor( _pmid.m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE )
			);
		if( !_pmid.m_strInPlaceText.IsEmpty() )
		{
			rcInPlaceWndOuterBorder.DeflateRect( 5, 2, 1, 2 );
			COLORREF clrOldText =
				dc.SetTextColor(
					GetColor( COLOR_BTNTEXT )
					);
			int nOldBkMode = dc.SetBkMode(TRANSPARENT);
			CFont * pOldFont = dc.SelectObject(&m_FontNormal);
			ASSERT( pOldFont != NULL );
			dc.DrawText(
				_pmid.m_strInPlaceText,
				_pmid.m_strInPlaceText.GetLength(),
				&rcInPlaceWndOuterBorder,
				DT_SINGLELINE|DT_NOPREFIX|DT_LEFT|DT_TOP
				);
			dc.SelectObject(&pOldFont);
			dc.SetBkMode(nOldBkMode);
			dc.SetTextColor(clrOldText);
		} // if( !_pmid.m_strInPlaceText.IsEmpty() )
	} // if( !_pmid.m_rcInPlaceWnd.IsRectEmpty() )
}

void CExtPaintManagerXP::PaintMenuItem(
	CDC & dc,
	CExtPaintManager::PAINTMENUITEMDATA & _pmid
	)
{
	if( _pmid.m_rcItem.IsRectEmpty() )
		return;
CExtSafeString _sText =
		(_pmid.m_sText==NULL) ? _T("") : _pmid.m_sText;
CExtSafeString _sAccelText =
		(_pmid.m_sAccelText==NULL) ? _T("") : _pmid.m_sAccelText;

COLORREF clr3dFace = // GetColor( CLR_3DFACE_OUT );
		GetColor(
			_pmid.m_bRarelyUsed
				? XPCLR_RARELY_BORDER
				: CLR_3DFACE_OUT
			);
COLORREF clrDkBorder = GetColor( XPCLR_HILIGHT_BORDER );
COLORREF clrBk = clr3dFace;
COLORREF clrText = GetColor(CLR_TEXT_OUT);
COLORREF clrBkHilight = GetColor(XPCLR_HILIGHT);
COLORREF clrIconBkHilight =
			_pmid.m_bSelected
				? clrBkHilight
				: GetColor( CLR_3DFACE_IN );
COLORREF clrRight = GetColor(XPCLR_3DFACE_NORMAL);
CRect rcBk( _pmid.m_rcItem ), rcSelBk( _pmid.m_rcItem );

	{ // BLOCK: paint left area
		COLORREF clrGradientLeft = ((COLORREF)(-1L)), clrGradientRight((COLORREF)(-1L));
		bool b2StepGradient = false;
		if( stat_GetBPP() > 8 )
		{
			if( _pmid.m_bRarelyUsed )
			{
				if(		m_nIdxClrMlaRarelyLeft >= 0
					&&	m_nIdxClrMlaRarelyRight >= 0
					)
				{ // if paint gradient on rarely used item's left part
					clrGradientLeft = GetColor( m_nIdxClrMlaRarelyLeft );
					clrGradientRight = GetColor( m_nIdxClrMlaRarelyRight );
				} // if paint gradient on rarely used item's left part
			} // if( _pmid.m_bRarelyUsed )
			else
			{
				if(	(!(		g_PaintManager.m_bUxApiInitPassed
						&&	g_PaintManager.m_bUxValidColorsExtracted
						&&	g_PaintManager.m_bUxUseIfAvailOnWinXpOrLater
					))
					)
					b2StepGradient = true;
				if(		m_nIdxClrMlaNormLeft >= 0
					&&	m_nIdxClrMlaNormRight >= 0
					)
				{ // if paint gradient on rarely used item's left part
					clrGradientLeft = GetColor( m_nIdxClrMlaNormLeft );
					clrGradientRight = GetColor( m_nIdxClrMlaNormRight );
				} // if paint gradient on rarely used item's left part
			} // else from if( _pmid.m_bRarelyUsed )
		} // if( stat_GetBPP() > 8 )
		
		CRect rcGradient( rcBk );
		rcGradient.right = rcGradient.left + _pmid.m_nIconAreaWidth;
		if( (!_pmid.m_bForceNoLeftGradient) && clrGradientLeft != ((COLORREF)(-1L)) )
		{
			ASSERT( clrGradientRight != ((COLORREF)(-1L)) );
			if( b2StepGradient )
			{
				CRect
					rcGradient1( rcGradient ),
					rcGradient2( rcGradient );
				rcGradient1.right =
					rcGradient1.left
					+ ::MulDiv( rcGradient1.Width(), 7, 10 );
				rcGradient2.left = rcGradient1.right;
				dc.FillSolidRect(
					&rcGradient2,
					clrGradientRight
					);
				stat_PaintGradientRect(
					dc,
					&rcGradient1,
					clrGradientLeft,
					clrGradientRight,
					false
					);
			} // if( b2StepGradient )
			else
				stat_PaintGradientRect(
					dc,
					&rcGradient,
					clrGradientLeft,
					clrGradientRight,
					false
					);
		} // if( (!_pmid.m_bForceNoLeftGradient) && clrGradientLeft != ((COLORREF)(-1L)) )
		else
			dc.FillSolidRect( &rcGradient, clrBk );
		CRect rcFillRightPart( rcBk );
		if( (!_pmid.m_bForceNoLeftGradient) )
			rcFillRightPart.left += _pmid.m_nIconAreaWidth;
		dc.FillSolidRect( &rcFillRightPart, clrRight );
	} // BLOCK: paint left area

	if( _pmid.m_bSelected )
	{
		ASSERT( _pmid.m_bEnabled );
		rcSelBk.bottom -= __EXT_MENU_VERT_DISTANCE_NORM;
		clrBk = GetColor( CLR_3DFACE_IN );
		dc.FillSolidRect( &rcSelBk, clrBk );	
		dc.Draw3dRect( &rcSelBk, clrDkBorder, clrDkBorder );
	} // if( _pmid.m_bSelected )
	if( (!_pmid.m_bForceNoLeftGradient) )
		rcBk.left += _pmid.m_nIconAreaWidth;

	rcBk.DeflateRect( __EXT_MENU_GAP, __EXT_MENU_IMG_RECT_OUT_GAP-1 );
COLORREF clrOldText = dc.SetTextColor(clrText);
int nOldBkMode = dc.SetBkMode(TRANSPARENT);
CFont * pOldFont = dc.SelectObject(&m_FontNormal);
	ASSERT( pOldFont != NULL );

CRect rcMeasureText( 0, 0, 0, 0 );
	dc.DrawText(
		_sText,
		_sText.GetLength(),
		&rcMeasureText,
		DT_CALCRECT|DT_SINGLELINE|DT_LEFT
		);

CRect rcDrawText( rcBk );
	rcDrawText.OffsetRect(
		0,
		( rcDrawText.Height() - rcMeasureText.Height() ) / 2
		);
	dc.DrawState(
		rcDrawText.TopLeft(),
		rcDrawText.Size(),
		_sText,
		(_pmid.m_bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
		TRUE,
		0,
		_pmid.m_bEnabled ?
			(CBrush*)NULL
			:
			CBrush::FromHandle(
				GetBrush(COLOR_3DSHADOW)
				)
		);
	if( !_sAccelText.IsEmpty() )
	{
		rcMeasureText.SetRect( 0, 0, 0, 0 );
		dc.DrawText(
			_sAccelText,
			_sAccelText.GetLength(),
			&rcMeasureText,
			DT_CALCRECT|DT_SINGLELINE|DT_LEFT
			);
		rcDrawText = rcBk;
		rcDrawText.OffsetRect(
			0,
			( rcDrawText.Height() - rcMeasureText.Height() ) / 2
			);
		rcDrawText.left =
			rcDrawText.right
			- rcMeasureText.Width()
			- __EXT_MENU_POPUP_ARROW_AREA_DX
			;
		rcBk.left = rcDrawText.left;
		dc.DrawState(
			rcDrawText.TopLeft(),
			rcDrawText.Size(),
			_sAccelText,
			(_pmid.m_bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
			TRUE,
			0,
			_pmid.m_bEnabled ?
				(CBrush*)NULL
				:
				CBrush::FromHandle(
					GetBrush(COLOR_3DSHADOW)
					)
			);
	} // if( !_sAccelText.IsEmpty() )

	if( _pmid.m_bPopup )
	{
		CRect rcPopupArrow(_pmid.m_rcItem);
		rcPopupArrow.DeflateRect(__EXT_MENU_GAP,__EXT_MENU_GAP);
		rcPopupArrow.left =
			rcPopupArrow.right - __EXT_MENU_POPUP_ARROW_AREA_DX;
		if( _pmid.m_bCustomizeListMode )
		{
			COLORREF ColorValues[2] =
			{
				RGB(0,0,0),
				GetColor(CLR_TEXT_OUT)
			};
			PaintGlyphCentered(
				dc,
				rcPopupArrow,
				_pmid.m_bSeparatedDropDown
					? g_glyph_customize_dropdown_src
					: g_glyph_customize_popup_src,
				ColorValues
				);
		} // if( _pmid.m_bCustomizeListMode )
		else
		{
			dc.SelectObject(&m_FontMarlett);
			dc.DrawText(
				_T("8"), // popup rightArrow
				rcPopupArrow,
				DT_CENTER|DT_VCENTER|DT_SINGLELINE
				);
		} // else from if( _pmid.m_bCustomizeListMode )
	} // if( _pmid.m_bPopup )
	
	dc.SelectObject(&pOldFont);
	dc.SetBkMode(nOldBkMode);
	dc.SetTextColor(clrOldText);

CSize sizeIcon( __EXT_MENU_ICON_CX, __EXT_MENU_ICON_CY );
	if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )
	{
		sizeIcon = _pmid.m_pIcon->GetSize();
		if(	sizeIcon.cx <= 18 && sizeIcon.cy < sizeIcon.cx )
			sizeIcon.cy = sizeIcon.cx;
	}
CRect rcIcon( _pmid.m_rcItem.TopLeft(), sizeIcon );
	if( _pmid.m_bXtraMark )
	{
		rcIcon.OffsetRect(
			( _pmid.m_nIconAreaWidth/2 - rcIcon.Width() ) / 2 + _pmid.m_nIconAreaWidth/2 - 2,
			( _pmid.m_rcItem.Height() - rcIcon.Height() ) / 2
			);
		if( _pmid.m_bXtraChecked )
		{
			CRect rcXtraMark(
				_pmid.m_rcItem.left+1,
				_pmid.m_rcItem.top+1,
				_pmid.m_rcItem.left+_pmid.m_rcItem.Height()-2,
				_pmid.m_rcItem.bottom-2
				);
			int nIdxMainGradientLeft = -1;
			int nIdxMainGradientRight = -1;
			if( _pmid.m_bSelected )
			{
				nIdxMainGradientLeft = m_nIdxClrBtnHovPresLeft;
				nIdxMainGradientRight = m_nIdxClrBtnHovPresRight;
			} // if( _pmid.m_bSelected )
			else
			{
				nIdxMainGradientLeft = m_nIdxClrBtnPressedLeft;
				nIdxMainGradientRight = m_nIdxClrBtnPressedRight;
			} // else if( _pmid.m_bSelected )
			if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
			{
				stat_PaintGradientRect(
					dc,
					rcXtraMark,
					GetColor( nIdxMainGradientRight ),
					GetColor( nIdxMainGradientLeft ),
					true
					);
			} // if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
			else
			{
				dc.FillSolidRect(
					rcXtraMark,
					clrIconBkHilight
					);
			} // else from if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
			COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
			int x = rcXtraMark.left + rcXtraMark.Width()/2 - 3;
			int y = rcXtraMark.top + rcXtraMark.Height()/2 - 3;
			ASSERT( !_pmid.m_bPushedRadio );
			stat_PaintCheckMark(
				dc,
				x,y,
				clrCheck
				);
			dc.Draw3dRect(
				rcXtraMark,
				clrDkBorder,
				clrDkBorder
				);
		} // if( _pmid.m_bXtraChecked )
	} // if( _pmid.m_bXtraMark )
	else
	{
		rcIcon.OffsetRect(
			( _pmid.m_nIconAreaWidth - rcIcon.Width() ) / 2,
			( _pmid.m_rcItem.Height() - rcIcon.Height() ) / 2
			);
	} // else from if( _pmid.m_bXtraMark )
CRect rcIconFrame( rcIcon );
	rcIconFrame.InflateRect(__EXT_MENU_IMG_RECT_OUT_GAP,__EXT_MENU_IMG_RECT_OUT_GAP);
	if( _pmid.m_bPushedCheck || _pmid.m_bPushedRadio )
	{
		int nIdxMainGradientLeft = -1;
		int nIdxMainGradientRight = -1;
		if( _pmid.m_bSelected )
		{
			nIdxMainGradientLeft = m_nIdxClrBtnHovPresLeft;
			nIdxMainGradientRight = m_nIdxClrBtnHovPresRight;
		} // if( _pmid.m_bSelected )
		else
		{
			nIdxMainGradientLeft = m_nIdxClrBtnPressedLeft;
			nIdxMainGradientRight = m_nIdxClrBtnPressedRight;
		} // else if( _pmid.m_bSelected )
		if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
		{
			stat_PaintGradientRect(
				dc,
				rcIconFrame,
				GetColor( nIdxMainGradientRight ),
				GetColor( nIdxMainGradientLeft ),
				true
				);
		} // if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
		else
		{
			dc.FillSolidRect(
				rcIconFrame,
				clrIconBkHilight
				);
		} // else from if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
	}
	if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )
	{
		CRect rcDummy;
		CExtSafeString sTitle( _T("") );
		PaintIcon(
			dc,
			true,
			sTitle,
			_pmid.m_pIcon,
			rcIcon,
			rcDummy,
			_pmid.m_bPushedCheck || _pmid.m_bPushedRadio,
			_pmid.m_bEnabled,
			_pmid.m_bSelected,
			0
			);
	} // if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )
	else
	{
		PaintSysCmdGlyph(
			dc,
			_pmid.m_nHelperSysCmdID,
			rcIcon,
			_pmid.m_bSelected,
			false,
			_pmid.m_bEnabled
			);
	} // else from if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )

	if( _pmid.m_bPushedCheck || _pmid.m_bPushedRadio )
	{
		if( _pmid.m_pIcon == NULL || _pmid.m_pIcon->IsEmpty() )
		{
			COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
			int x = rcIcon.left + rcIcon.Width()/2 - 3;
			int y = rcIcon.top + rcIcon.Height()/2 - 3;
			if( _pmid.m_bPushedCheck )
			{
				ASSERT( !_pmid.m_bPushedRadio );
				stat_PaintCheckMark(
					dc,
					x,y,
					clrCheck
					);
			}
			else
			{
				ASSERT( !_pmid.m_bPushedCheck );
				stat_PaintRadioDot(
					dc,
					x,y,
					clrCheck
					);
			}
		} // if( _pmid.m_pIcon == NULL || _pmid.m_pIcon->IsEmpty() )
		dc.Draw3dRect(
			rcIconFrame,
			clrDkBorder,
			clrDkBorder
			);
	}

	if( !_pmid.m_rcInPlaceWnd.IsRectEmpty() )
	{
		CRect rcInPlaceWndOuterBorder( _pmid.m_rcInPlaceWnd );
		rcInPlaceWndOuterBorder.InflateRect( 1, 2 );
		if( _pmid.m_bPopup )
		{
			INT nDD = (INT) ::GetSystemMetrics(SM_CXHTHUMB);
			rcInPlaceWndOuterBorder.right += nDD;
			rcInPlaceWndOuterBorder.InflateRect( 1, 1 );
			PAINTCOMBOFRAMEDATA _pcfd(
				_pmid.m_pHelperSrc,
				rcInPlaceWndOuterBorder,
				_pmid.m_bSelected, // hover
				_pmid.m_bSelected && _pmid.m_bHelperPopupDropped, // pushed
				_pmid.m_bEnabled
				);
			_pcfd.m_bNoTopOuterBorder = true;
			PaintComboFrame( dc, _pcfd );
			rcInPlaceWndOuterBorder.DeflateRect( 1, 1 );
			if( _pmid.m_bSelected )
			{
				dc.Draw3dRect(
					&rcInPlaceWndOuterBorder,
					clrDkBorder,
					clrDkBorder
					);
			} // if( _pmid.m_bSelected )
			else
			{
				COLORREF clrBorder = GetColor( CLR_3DFACE_OUT );
				dc.Draw3dRect(
					&rcInPlaceWndOuterBorder,
					clrBorder,
					clrBorder
					);
				COLORREF clrWindow = GetColor( COLOR_WINDOW );
				CRect rcBtn( rcInPlaceWndOuterBorder );
				rcBtn.left = rcBtn.right - nDD;
				rcBtn.DeflateRect( 0, 1, 1, 1 );
				dc.Draw3dRect(
					&rcBtn,
					clrWindow,
					clrWindow
					);
			} // else from if( _pmid.m_bSelected )
			rcInPlaceWndOuterBorder.right -= nDD;
		} // if( _pmid.m_bPopup )
		else
		{
			COLORREF clr =
				_pmid.m_bSelected
					? clrDkBorder
					: GetColor(COLOR_3DLIGHT)
					;
			dc.Draw3dRect(
				&rcInPlaceWndOuterBorder,
				clr,
				clr
				);
		} // else from if( _pmid.m_bPopup )
		rcInPlaceWndOuterBorder.DeflateRect( 1, 1 );
		dc.FillSolidRect(
			&rcInPlaceWndOuterBorder,
			GetColor( _pmid.m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE )
			);
		if( !_pmid.m_strInPlaceText.IsEmpty() )
		{
			rcInPlaceWndOuterBorder.DeflateRect( 5, 3, 1, 2 );
			COLORREF clrOldText =
				dc.SetTextColor(
					GetColor( COLOR_BTNTEXT )
					);
			int nOldBkMode = dc.SetBkMode(TRANSPARENT);
			CFont * pOldFont = dc.SelectObject(&m_FontNormal);
			ASSERT( pOldFont != NULL );
			dc.DrawText(
				_pmid.m_strInPlaceText,
				_pmid.m_strInPlaceText.GetLength(),
				&rcInPlaceWndOuterBorder,
				DT_SINGLELINE|DT_NOPREFIX|DT_LEFT|DT_TOP
				);
			dc.SelectObject(&pOldFont);
			dc.SetBkMode(nOldBkMode);
			dc.SetTextColor(clrOldText);
		} // if( !_pmid.m_strInPlaceText.IsEmpty() )
	} // if( !_pmid.m_rcInPlaceWnd.IsRectEmpty() )
}

void CExtPaintManager::PaintIcon(
	CDC & dc,
	bool bHorz,
	CExtSafeString & sTitle,
	CExtCmdIcon * pIcon,
	const CRect & rectItem,
	CRect & rectCaption,
	bool bPushed,
	bool bEnabled,
	bool bHover,
	int eAlign
	)
{
	bHover;
	rectCaption = rectItem;
	if( pIcon == NULL || pIcon->IsEmpty() )
		return;

CSize _sizeIcon = pIcon->GetSize();
	ASSERT( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 );

CRect iconRect = rectItem;
CRect btnRect;
	if( bHorz )
	{
		if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		{
			if( sTitle.IsEmpty() )
				// Center the icon horizontally
				iconRect.left += ((iconRect.Width() - _sizeIcon.cx)/2);
			else
			{
				iconRect.left += 3;  
				rectCaption.left += _sizeIcon.cx + 3;
			}
		} // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		else
		{
			if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
			{
				btnRect = rectCaption;
				if( sTitle.IsEmpty() )
					// Center the icon horizontally
					iconRect.left += ((iconRect.Width() - _sizeIcon.cx)/2);
				else
				{
					rectCaption.right = rectCaption.Width() - _sizeIcon.cx - 3;
					rectCaption.left = 3;
					iconRect.left = btnRect.right - _sizeIcon.cx - 3;
					// Center the icon vertically
					iconRect.top += ((iconRect.Height() - _sizeIcon.cy)/2);
				}
			} // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
		} // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		if( (eAlign&__ALIGN_VERT) != 0 )
		{
			// Center the icon horizontally
			iconRect.top += ((iconRect.Height() - _sizeIcon.cy)/2);
		} // if( (eAlign&__ALIGN_VERT) != 0 )
	} // if( bHorz )
	else
	{
		if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		{
			if( sTitle.IsEmpty() )
				// Center the icon horizontally
				iconRect.top += ((iconRect.Height() - _sizeIcon.cy)/2);
			else
			{
				iconRect.top += 3;  
				rectCaption.top += _sizeIcon.cy + 3;
			}
		} // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		else
		{
			if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
			{
				btnRect = rectCaption;
				if( sTitle.IsEmpty() )
					// Center the icon horizontally
					iconRect.top += ((iconRect.Height() - _sizeIcon.cy)/2);
				else
				{
					rectCaption.top = rectCaption.Height() - _sizeIcon.cy - 3;
					rectCaption.top = 3;
					iconRect.top = btnRect.top - _sizeIcon.cy - 3;
					// Center the icon vertically
					iconRect.left += ((iconRect.Width() - _sizeIcon.cx)/2);
				}
			} // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
		} // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		if( (eAlign&__ALIGN_VERT) != 0 )
		{
			// Center the icon horizontally
			iconRect.left += ((iconRect.Width() - _sizeIcon.cx)/2);
		} // if( (eAlign&__ALIGN_VERT) != 0 )
	} // else from if( bHorz )
    
	// If button is pressed then press the icon also
	if( bPushed )
	{
		CSize sizePushedOffset = GetPushedOffset();
		iconRect.OffsetRect(sizePushedOffset.cx, sizePushedOffset.cy);
	}
//	dc.DrawState(
//		iconRect.TopLeft(),
//		iconRect.Size(), 
//		pIcon->GetIcon(), 
//		(bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
//		(CBrush*)NULL
//		);
	pIcon->Paint(
		dc,
		iconRect.TopLeft(),
		bEnabled
			? CExtCmdIcon::__DRAW_NORMAL
			: CExtCmdIcon::__DRAW_DISABLED
		);
}

void CExtPaintManagerOffice2003::PaintSeparator(
	CDC & dc,
	const RECT & rectItem,
	bool bHorz,
	bool bTransparentBk,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	if(		pHelperSrc == NULL
		||	(!(	pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtToolControlBar))
			||	pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtBarButton))
			||	pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar))
			))
		||	stat_GetBPP() <= 8
		)
	{
		CExtPaintManagerXP::PaintSeparator(
			dc,
			rectItem,
			bHorz,
			bTransparentBk,
			pHelperSrc,
			lParam
			);
		return;
	}
CRect rectItem2(rectItem);
	if( !bTransparentBk )
		dc.FillSolidRect(
			rectItem2,
			GetColor(CLR_3DFACE_OUT)
			);

COLORREF clrBtnShadow = GetColor(_2003CLR_SEPARATOR_DARK);
COLORREF clrBtnHilight = GetColor(_2003CLR_SEPARATOR_LIGHT);
	if( bHorz )
	{
		int x = (rectItem2.left + rectItem2.right) / 2;
		rectItem2.left = x-1; rectItem2.right = x+1;
		if(		pHelperSrc == NULL
			||	(! pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar)) )
			)
			rectItem2.DeflateRect( 0, 3, 0, 4 );
		dc.Draw3dRect(
			rectItem2,
			clrBtnShadow,
			clrBtnHilight
		);
	} // if( bHorz )
	else
	{
		int y = (rectItem2.top+rectItem2.bottom)/2;
		rectItem2.top = y-1; rectItem2.bottom = y+1;
		if(		pHelperSrc == NULL
			||	(! pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar)) )
			)
			rectItem2.DeflateRect( 4, 0, 5, 0 );
		dc.Draw3dRect(
			rectItem2,
			clrBtnShadow,
			clrBtnHilight
			);
	} // else from if( bHorz )
}

void CExtPaintManagerXP::PaintSeparator(
	CDC & dc,
	const RECT & rectItem,
	bool bHorz,
	bool bTransparentBk,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
CRect rectItem2(rectItem);
	if( !bTransparentBk )
		dc.FillSolidRect(
			rectItem2,
			GetColor(CLR_3DFACE_OUT)
			);
	if( bHorz )
	{
		rectItem2.left =
			rectItem2.left + rectItem2.Width()/2 - 1;
		rectItem2.right =
			rectItem2.left + 1;
		rectItem2.DeflateRect( 0, 1 );
	}
	else
	{
		rectItem2.top =
			rectItem2.top + rectItem2.Height()/2 - 1;
		rectItem2.bottom =
			rectItem2.top + 1;
		rectItem2.DeflateRect( 1, 0 );
	}
	dc.FillSolidRect(
		&rectItem2,
		GetColor(XPCLR_SEPARATOR)
		);
}

void CExtPaintManager::PaintSeparator(
	CDC & dc,
	const RECT & rectItem,
	bool bHorz,
	bool bTransparentBk,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
CRect rectItem2(rectItem);
	if( !bTransparentBk )
		dc.FillSolidRect(
			rectItem2,
			GetColor(CLR_3DFACE_OUT)
			);

COLORREF clrBtnShadow =  GetColor(CLR_3DSHADOW_OUT);
COLORREF clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);
	if( bHorz )
	{
		int x = (rectItem2.left + rectItem2.right) / 2;
		rectItem2.left = x-1; rectItem2.right = x+1;
		rectItem2.DeflateRect( 0, 0, 0, 1 );
		dc.Draw3dRect(
			rectItem2,
			clrBtnShadow,
			clrBtnHilight
		);
	} // if( bHorz )
	else
	{
		int y = (rectItem2.top+rectItem2.bottom)/2;
		rectItem2.top = y-1; rectItem2.bottom = y+1;
		rectItem2.DeflateRect( 0, 0, 1, 0 );
		dc.Draw3dRect(
			rectItem2,
			clrBtnShadow,
			clrBtnHilight
			);
	} // else from if( bHorz )
}

CExtPaintManager::PAINTCONTROLFRAMEDATA::PAINTCONTROLFRAMEDATA()
	: m_pHelperSrc( NULL )
	, m_lParam( 0L )
	, m_rcClient( 0,0,0,0 )
	, m_bFlat( false )
	, m_bHover( false )
	, m_bEnabled( false )
	, m_bFocus( false )
{
}

CExtPaintManager::PAINTCONTROLFRAMEDATA::PAINTCONTROLFRAMEDATA(
	CObject * pHelperSrc,
	const RECT & rcClient,
	bool bFlat,
	bool bHover,
	bool bEnabled,
	bool bFocus
	)
	: m_pHelperSrc( pHelperSrc )
	, m_lParam( 0L )
	, m_rcClient( rcClient )
	, m_bFlat( bFlat )
	, m_bHover( bHover )
	, m_bEnabled( bEnabled )
	, m_bFocus( bFocus )
{
}

void CExtPaintManager::PaintControlFrame(
	CDC & dc,
	CExtPaintManager::PAINTCONTROLFRAMEDATA & _pcfd
	)
{
CRect rectItem( _pcfd.m_rcClient );
COLORREF clrInner = GetColor( COLOR_WINDOW );
COLORREF clrIn1, clrIn2, clrOut1, clrOut2;
	clrIn1 = clrIn2 = clrOut1 = clrOut2 = GetColor( CLR_3DFACE_OUT );
	if( _pcfd.m_bEnabled )
	{
		if( _pcfd.m_bHover && _pcfd.m_bFocus )
		{
				clrIn1 = GetColor( COLOR_3DFACE );
				clrIn2 = GetColor( COLOR_3DDKSHADOW );
				clrOut1 = GetColor( COLOR_3DHILIGHT );
				clrOut2 = GetColor( COLOR_3DSHADOW );
		} // if( _pcfd.m_bHover && _pcfd.m_bFocus )
		else
		{
			if( _pcfd.m_bHover || _pcfd.m_bFocus )
			{
				clrIn1 = GetColor( COLOR_3DHILIGHT );
				clrIn2 = GetColor( COLOR_3DSHADOW );
			} // if( _pcfd.m_bHover || _pcfd.m_bFocus )
		} // elsefrom if( _pcfd.m_bHover && _pcfd.m_bFocus )
	} // else from if( _pcfd.m_bEnabled )
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrIn2, clrIn1 );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrOut2, clrOut1 );
}

void CExtPaintManagerXP::PaintControlFrame(
	CDC & dc,
	CExtPaintManager::PAINTCONTROLFRAMEDATA & _pcfd
	)
{
CRect rectItem( _pcfd.m_rcClient );
COLORREF clrInner = GetColor( COLOR_WINDOW );
COLORREF clrIn, clrOut;
	clrIn = clrOut = GetColor( CLR_3DFACE_OUT );
	if( _pcfd.m_bEnabled )
	{
		if( _pcfd.m_bHover )
		{
			if( _pcfd.m_bFocus )
				clrIn = clrOut = GetColor( XPCLR_HILIGHT_BORDER );
			else
				clrIn = GetColor( XPCLR_HILIGHT_BORDER );
		} // if( _pcfd.m_bHover )
		else
		{
			if( _pcfd.m_bFocus )
				clrIn = GetColor( XPCLR_HILIGHT_BORDER );
		} // else from if( _pcfd.m_bHover )
	} // else from if( _pcfd.m_bEnabled )
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrIn, clrIn );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrOut, clrOut );
}

CExtPaintManager::PAINTCOMBOFRAMEDATA::PAINTCOMBOFRAMEDATA()
	: m_pHelperSrc( NULL )
	, m_lParam( 0L )
	, m_rcClient( 0,0,0,0 )
	, m_bHover( false )
	, m_bPushed( false )
	, m_bEnabled( false )
	, m_clrForceNABorder( COLORREF(-1L) )
	, m_clrForceNAContent( COLORREF(-1L) )
	, m_clrForceActiveOuterBorder( COLORREF(-1L) )
	, m_bNoTopOuterBorder( false )
{
}

CExtPaintManager::PAINTCOMBOFRAMEDATA::PAINTCOMBOFRAMEDATA(
	CObject * pHelperSrc,
	const RECT & rcClient,
	bool bHover,
	bool bPushed,
	bool bEnabled
	)
	: m_pHelperSrc( pHelperSrc )
	, m_lParam( 0L )
	, m_rcClient( rcClient )
	, m_bHover( bHover )
	, m_bPushed( bPushed )
	, m_bEnabled( bEnabled )
	, m_clrForceNABorder( COLORREF(-1L) )
	, m_clrForceNAContent( COLORREF(-1L) )
	, m_clrForceActiveOuterBorder( COLORREF(-1L) )
	, m_bNoTopOuterBorder( false )
{
}

void CExtPaintManager::PaintComboFrame(
	CDC &dc,
	CExtPaintManager::PAINTCOMBOFRAMEDATA & _pcfd
	)
{
COLORREF clrTopLeft, clrBottomRight;

	if( _pcfd.m_clrForceNAContent == COLORREF(-1L) )
		clrTopLeft = clrBottomRight =
			GetColor( CLR_3DFACE_OUT );
	else
		clrTopLeft = clrBottomRight =
			_pcfd.m_clrForceNAContent;
	if( _pcfd.m_bPushed || _pcfd.m_bHover )
	{
		clrTopLeft =
			GetColor( CLR_3DSHADOW_IN );
		clrBottomRight =
			GetColor( CLR_3DHILIGHT_IN );
	}
COLORREF clrHilightOut =
		( _pcfd.m_clrForceNABorder == COLORREF(-1L) || _pcfd.m_bPushed || _pcfd.m_bHover )
			? GetColor( CLR_3DHILIGHT_OUT )
			: _pcfd.m_clrForceNABorder
			;

COLORREF clrFaceOut =
		( _pcfd.m_clrForceNABorder == COLORREF(-1L) || _pcfd.m_bPushed || _pcfd.m_bHover )
			? GetColor( CLR_3DFACE_OUT )
			: _pcfd.m_clrForceNABorder
			;

COLORREF clrWindow =
		( _pcfd.m_clrForceNABorder == COLORREF(-1L) || _pcfd.m_bPushed || _pcfd.m_bHover )
			? GetColor(
				_pcfd.m_bEnabled
					? COLOR_WINDOW
					: COLOR_3DFACE
				)
			: _pcfd.m_clrForceNABorder
			;

CRect rectClient( _pcfd.m_rcClient );
	dc.Draw3dRect(
		rectClient,
		clrTopLeft,
		clrBottomRight
		);
	rectClient.DeflateRect(1,1);
	dc.Draw3dRect(
		rectClient,
		_pcfd.m_bEnabled ? clrFaceOut : clrHilightOut,
		_pcfd.m_bEnabled ? clrFaceOut : clrHilightOut
		);

	rectClient.left =
		rectClient.right -
		::GetSystemMetrics(SM_CXHTHUMB);
	dc.Draw3dRect(
		rectClient,
		clrFaceOut,
		clrFaceOut
		);
	rectClient.DeflateRect(1,1);
	dc.Draw3dRect(
		rectClient,
		clrHilightOut,
		clrHilightOut
		);

	dc.FillSolidRect(
		rectClient.left-2,
		rectClient.top,
		3,
		rectClient.Height(),
		_pcfd.m_bEnabled ? clrWindow : clrFaceOut
		);

	if( _pcfd.m_bHover || _pcfd.m_bPushed )
		dc.FillSolidRect(
			rectClient,
			clrFaceOut
			);

	rectClient.left++;

CFont font;
int ppi = dc.GetDeviceCaps(LOGPIXELSX);
int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
	font.CreatePointFont(pointsize, _T("Marlett"));

PAINTPUSHBUTTONDATA _ppbd(
		this,
		true,
		rectClient,
		_T("6"),
		NULL,
		true,
		(_pcfd.m_bHover&&(!_pcfd.m_bPushed)) ? true : false, //(bHover||bPushed) ? true : false,
		_pcfd.m_bPushed ? true : false,
		false,
		_pcfd.m_bEnabled,
		true,
		false,
		false,
		CExtPaintManager::__ALIGN_HORIZ_CENTER
			| CExtPaintManager::__ALIGN_VERT,
		(HFONT)font.GetSafeHandle() // = NULL
		);
	PaintPushButton( dc, _ppbd );

	if( !(_pcfd.m_bHover || _pcfd.m_bPushed) )
	{
		if( !_pcfd.m_bHover )
			dc.Draw3dRect(
				rectClient,
				clrHilightOut,
				clrHilightOut
				);
		else
			dc.Draw3dRect(
				rectClient,
				clrFaceOut,
				clrFaceOut
				);
	}
}

void CExtPaintManager::stat_PaintDotNet3dRect(
	CDC & dc,
	LPCRECT lpRect,
	COLORREF clrTopLeft,
	COLORREF clrBottomRight
	)
{
	stat_PaintDotNet3dRect(
		dc,
		lpRect->left,
		lpRect->top,
		lpRect->right - lpRect->left,
		lpRect->bottom - lpRect->top,
		clrTopLeft,
		clrBottomRight
		);
}

void CExtPaintManager::stat_PaintDotNet3dRect(
	CDC & dc,
	int x,
	int y,
	int cx,
	int cy,
	COLORREF clrTopLeft,
	COLORREF clrBottomRight
	)
{
const int nPixelGap = 1;
	dc.FillSolidRect(
		x + nPixelGap,
		y,
		cx - 1 - nPixelGap,
		1,
		clrTopLeft
		);
	dc.FillSolidRect(
		x,
		y + nPixelGap,
		1,
		cy - 1 - nPixelGap,
		clrTopLeft
		);
	dc.FillSolidRect(
		x + cx,
		y + nPixelGap,
		-1,
		cy - nPixelGap*2,
		clrBottomRight
		);
	dc.FillSolidRect(
		x + nPixelGap,
		y + cy,
		cx - nPixelGap*2,
		-1,
		clrBottomRight
		);
}

void CExtPaintManager::stat_PaintRadioDot(
	CDC & dc,
	int x,
	int y,
	COLORREF color
	)
{
CBrush brush,*pOldBrush;
CPen pen,*pOldPen;
CRect rcDot(x,y,x+6,y+6);

COLORREF color2 =
		dc.GetNearestColor(
			RGB(
				255-GetRValue(color),
				255-GetGValue(color),
				255-GetBValue(color)
			)
		);
	rcDot.OffsetRect(1,1);
	brush.CreateSolidBrush(color2);
	pen.CreatePen(PS_SOLID,0,color2);
	pOldBrush=dc.SelectObject(&brush);
	pOldPen=dc.SelectObject(&pen);
	dc.Ellipse(&rcDot);
	pen.DeleteObject();
	brush.DeleteObject();
	rcDot.OffsetRect(-1,-1);

	brush.CreateSolidBrush(color);
	pen.CreatePen(PS_SOLID,0,color);
	dc.SelectObject(&brush);
	dc.SelectObject(&pen);
	dc.Ellipse(&rcDot);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
	brush.DeleteObject();
}

void CExtPaintManager::stat_PaintCheckMark(
	CDC & dc,
	int x,
	int y,
	COLORREF color
	)
{
COLORREF color2 =
		dc.GetNearestColor(
			RGB(
				255-GetRValue(color),
				255-GetGValue(color),
				255-GetBValue(color)
			)
		);
 	dc.SetPixel(x,y+2,color);
	dc.SetPixel(x,y+3,color);
	dc.SetPixel(x,y+4,color2);

	dc.SetPixel(x+1,y+3,color);
	dc.SetPixel(x+1,y+4,color);
	dc.SetPixel(x+1,y+5,color2);

	dc.SetPixel(x+2,y+4,color);
	dc.SetPixel(x+2,y+5,color);
	dc.SetPixel(x+2,y+6,color2);

	dc.SetPixel(x+3,y+3,color);
	dc.SetPixel(x+3,y+4,color);
	dc.SetPixel(x+3,y+5,color2);

	dc.SetPixel(x+4,y+2,color);
	dc.SetPixel(x+4,y+3,color);
	dc.SetPixel(x+4,y+4,color2);

	dc.SetPixel(x+5,y+1,color);
	dc.SetPixel(x+5,y+2,color);
	dc.SetPixel(x+5,y+3,color2);

	dc.SetPixel(x+6,y,color);
	dc.SetPixel(x+6,y+1,color);
	dc.SetPixel(x+6,y+2,color2);
}


void CExtPaintManager::stat_PaintTabItemImpl(
	CDC & dc,
	CRect & rcTabItemsArea,
	bool bTopLeft,
	bool bHorz,
	bool bSelected,
	bool bCenteredText,
	bool bGroupedMode,
	bool bInGroupActive,
	bool bInvertedVerticalMode,
	const CRect & rcEntireItem,
	CSize sizeTextMeasured,
	CFont * pFont,
	__EXT_MFC_SAFE_LPCTSTR sText,
	CExtCmdIcon * pIcon,
	COLORREF clrText,
	COLORREF clrTabBk,
	COLORREF clrTabBorderLT,
	COLORREF clrTabBorderRB,
	COLORREF clrTabSeparator
	)
{
#if (!defined __EXT_MFC_NO_TAB_CTRL)
	ASSERT( dc.GetSafeHdc() != NULL );

	rcTabItemsArea;
	sizeTextMeasured;
	bInGroupActive;

CRect rcItem( rcEntireItem );

	if( bGroupedMode )
	{
		CRect rcTabMargin( rcItem );
		rcTabMargin.InflateRect(
			0,
			0,
			bHorz ? 1 : 0,
			bHorz ? 0 : 1
			);
		rcTabMargin.InflateRect(
			bHorz ? 0 : 2,
			bHorz ? 2 : 0
			);
		dc.FillSolidRect(
			&rcTabMargin,
			clrTabBk
			);
		dc.Draw3dRect(
			rcTabMargin,
			clrTabBorderLT,
			clrTabBorderRB
			);
	} // if( bGroupedMode )
	else
	{
		if( bSelected )
		{
			CRect rcTabSel( rcItem );
			rcTabSel.InflateRect(
				bHorz ? __EXTTAB_SEPARATOR_DX : 0,
				bHorz ? 0 : __EXTTAB_SEPARATOR_DY,
				0,
				0
				);
			CRect rcSetMargins(
				( (!bHorz) && (!bTopLeft) ) ? 1 : 0,
				(   bHorz  && (!bTopLeft) ) ? 1 : 0,
				( (!bHorz) &&   bTopLeft  ) ? 1 : 0,
				(   bHorz  &&   bTopLeft  ) ? 1 : 0
				);
			rcTabSel.InflateRect(
				rcSetMargins.left,
				rcSetMargins.top,
				rcSetMargins.right,
				rcSetMargins.bottom
				);
			dc.Draw3dRect(
				rcTabSel,
				clrTabBorderLT,
				clrTabBorderRB
				);
			if( bHorz )
				dc.SetPixel(
					rcTabSel.left,
					bTopLeft ? (rcTabSel.bottom - 1) : rcTabSel.top,
					clrTabBk
					);
			else
				dc.SetPixel(
					bTopLeft ? (rcTabSel.right - 1) : rcTabSel.left,
					rcTabSel.top,
					clrTabBk
					);
			rcTabSel.DeflateRect(
				(rcSetMargins.left == 0)	? 1 : 0,
				(rcSetMargins.top == 0)		? 1 : 0,
				(rcSetMargins.right == 0)	? 1 : 0,
				(rcSetMargins.bottom == 0)	? 1 : 0
				);
			dc.FillSolidRect( &rcTabSel, clrTabBk );
		} // if( bSelected )
		else
		{
			CRect rcSeparator( rcItem );
			if( bHorz )
			{
				rcSeparator.left = rcSeparator.right - 1;
				rcSeparator.DeflateRect( 0, __EXTTAB_SEPARATOR_GAP_HORZ );
			}
			else
			{
				rcSeparator.top = rcSeparator.bottom - 1;
				rcSeparator.DeflateRect( __EXTTAB_SEPARATOR_GAP_VERT, 0 );
			}
			dc.FillSolidRect( &rcSeparator, clrTabSeparator );
		} // else from if( bSelected )
	} // else from if( bGroupedMode )

	rcItem.DeflateRect(
		bHorz ? __EXTTAB_MARGIN_BORDER_HX : __EXTTAB_MARGIN_BORDER_VX,
		bHorz ? __EXTTAB_MARGIN_BORDER_HY : __EXTTAB_MARGIN_BORDER_VY
		);

CSize _sizeIcon( 0, 0 );
	if( pIcon != NULL && (!pIcon->IsEmpty()) )
	{
		_sizeIcon = pIcon->GetSize();
		ASSERT( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 );
	} // if( pIcon != NULL && (!pIcon->IsEmpty()) )

CRect rcItemForIcon( rcItem );
	if(		pIcon != NULL
		&&	(! pIcon->IsEmpty())
		&&	_sizeIcon.cx > 0
		&&	_sizeIcon.cy > 0
		)
	{
//		rcItemForIcon.DeflateRect(
//			bHorz ? 0 : ( rcItem.Width() - _sizeIcon.cx),
//			bHorz ? ( rcItem.Height() - _sizeIcon.cy) : 0
//			);
		rcItemForIcon.right = rcItemForIcon.left + _sizeIcon.cx;
		rcItemForIcon.bottom = rcItemForIcon.top + _sizeIcon.cy;
		rcItemForIcon.OffsetRect(
			bHorz ? 0 : ((rcItem.Width() - _sizeIcon.cx) / 2),
			bHorz ? ((rcItem.Height() - _sizeIcon.cy) / 2) : 0
			);
		if( rcItemForIcon.left < (rcItem.left+1) )
			rcItemForIcon.left = (rcItem.left+1);
		if( rcItemForIcon.right < (rcItem.right-1) )
			rcItemForIcon.right = (rcItem.right-1);
		if( rcItemForIcon.top < (rcItem.top+1) )
			rcItemForIcon.top = (rcItem.top+1);
		if( rcItemForIcon.bottom < (rcItem.bottom-1) )
			rcItemForIcon.bottom = (rcItem.bottom-1);
	}

CExtSafeString sItemText( (sText == NULL) ? _T("") : sText );

// IMPORTANT:  the rcText calculation fixed by Genka
CRect rcText(
		rcItem.left // rcItemForIcon.left
			+	(	bHorz
					? (_sizeIcon.cx +
						((_sizeIcon.cx > 0) ? __EXTTAB_MARGIN_ICON2TEXT_X : 0)
						)
					: 0
				),
		rcItem.top // rcItemForIcon.left
			+	(	bHorz
					? 0
					: (_sizeIcon.cy +
						((_sizeIcon.cy > 0) ? __EXTTAB_MARGIN_ICON2TEXT_Y : 0)
						)
				),
		rcItem.right,
		rcItem.bottom
		);
	if( !bHorz )
	{
		int nWidth0 = rcText.Width();
		int nWidth1 = rcItem.Width() + __EXTTAB_MARGIN_ICON2TEXT_X*2;
		if( nWidth1 > nWidth0 )
		{
			if( bInvertedVerticalMode )
			{
				rcText.right -= __EXTTAB_MARGIN_ICON2TEXT_X;
				rcText.left = rcText.right - nWidth1;
			} // if( bInvertedVerticalMode )
			else
			{
				rcText.left += __EXTTAB_MARGIN_ICON2TEXT_X;
				rcText.right = rcText.left + nWidth1;
			} // else from if( bInvertedVerticalMode )
		} // if( nWidth1 > nWidth0 )
	} // if( !bHorz )

	//dc.FillSolidRect( &rcText, RGB(0,0,255) );
CSize _sizeText = rcText.Size();

bool bDrawText = false;
	if(		( bHorz		&& _sizeText.cx >= ( max(16,_sizeIcon.cx) ) )
		||	( (!bHorz)	&& _sizeText.cy >= ( max(16,_sizeIcon.cy) ) )
		)
		bDrawText = true;

	if( !bDrawText && !( bGroupedMode && ! bInGroupActive ) )
	{
		rcItemForIcon.OffsetRect(
			bHorz ? (rcItem.Width() - _sizeIcon.cx)/2 : 0,
			bHorz ? 0 : (rcItem.Height() - _sizeIcon.cy)/2
			);
	}

	if( pIcon != NULL && (! pIcon->IsEmpty()) )
	{
		if(		(bHorz && rcItemForIcon.Width() >= _sizeIcon.cx )
			||	(!bHorz && rcItemForIcon.Height() >= _sizeIcon.cy)
			)
		{
			CRect rcTmpText( 0, 0, 0, 0 );
			rcItemForIcon.OffsetRect( -1, -1 );
			g_PaintManager->PaintIcon(
				dc,
				true,
				sItemText,
				pIcon,
				rcItemForIcon,
				rcTmpText,
				false,
				true,
				false,
				0
				);
		}
	} // if( pIcon != NULL && (! pIcon->IsEmpty()) )

	if( bDrawText )
	{ // if we have sense to paint text on tab item
		ASSERT( pFont != NULL );
		ASSERT( pFont->GetSafeHandle() != NULL );
		COLORREF clrOldText = dc.SetTextColor( clrText );
		INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
		
		CFont * pOldFont = dc.SelectObject( pFont );
		if( !bHorz )
		{
			if( bCenteredText )
			{
				UINT nOldTA = dc.SetTextAlign(
					TA_CENTER | TA_BASELINE
					);
				rcText.OffsetRect(
					bInvertedVerticalMode
						?   sizeTextMeasured.cy/2
						: - sizeTextMeasured.cy/2
						,
					0
					);
				CPoint ptCenter = rcText.CenterPoint();
				dc.ExtTextOut(
					ptCenter.x,
					ptCenter.y,
					ETO_CLIPPED,
					&rcText,
					sItemText,
					sItemText.GetLength(),
					NULL
					);
				dc.SetTextAlign( nOldTA );
			} // if( bCenteredText )
			else
			{
				UINT nOldTA = dc.SetTextAlign(
					TA_TOP | TA_BASELINE
					);
				rcText.OffsetRect(
					bInvertedVerticalMode
						?   sizeTextMeasured.cy/2
						: - sizeTextMeasured.cy/2
						,
					0
					);
				CPoint ptCenter = rcText.CenterPoint();
				if( bInvertedVerticalMode )
					ptCenter.y =
						rcText.bottom - 4
						- (rcText.Height() - sizeTextMeasured.cx)
						;
				else
					ptCenter.y =
						rcText.top + 4
						;
				dc.ExtTextOut(
					ptCenter.x,
					ptCenter.y,
					ETO_CLIPPED,
					&rcText,
					sItemText,
					sItemText.GetLength(),
					NULL
					);
				dc.SetTextAlign( nOldTA );
			} // else from if( bCenteredText )
		} // if( !bHorz )
		else
		{
			UINT nFormat =
				DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS;
			if( bCenteredText )
				nFormat |= DT_CENTER;
			else
				nFormat |= DT_LEFT;
//rcText.InflateRect( 0, 3 );
			dc.DrawText(
				sItemText,
				sItemText.GetLength(),
				rcText,
				nFormat
				);
		}
		dc.SelectObject( pOldFont );

		dc.SetBkMode( nOldBkMode );
		dc.SetTextColor( clrOldText );
	} // if we have sense to paint text on tab item
#else // (!defined __EXT_MFC_NO_TAB_CTRL)
	dc;
	rcTabItemsArea;
	bTopLeft;
	bHorz;
	bSelected;
	bCenteredText;
	bGroupedMode;
	bInGroupActive;
	bInvertedVerticalMode;
	rcEntireItem;
	sizeTextMeasured;
	pFont;
	sText;
	pIcon;
	clrText;
	clrTabBk;
	clrTabBorderLT;
	clrTabBorderRB;
	clrTabSeparator;
#endif // else from(!defined __EXT_MFC_NO_TAB_CTRL)
}

void CExtPaintManager::stat_PaintTabClientAreaImpl(
	CDC & dc,
	CRect & rcClient,
	CRect & rcTabItemsArea,
	CRect & rcTabNearBorderArea,
	DWORD dwOrientation,
	bool bGroupedMode,
	COLORREF clrTabItemsAreaBk,
	COLORREF clrTabNearBorderAreaBk,
	COLORREF clrTabNearMargin
	)
{
#if (!defined __EXT_MFC_NO_TAB_CTRL)
	ASSERT( dc.GetSafeHdc() != NULL );

	rcTabItemsArea;

CRect rcTabNearMargin( rcTabNearBorderArea ); // prepare tab border margin rect

	switch( dwOrientation )
	{
	case __ETWS_ORIENT_TOP:
		rcTabNearMargin.bottom = rcTabNearMargin.top + 1;
	break;
	case __ETWS_ORIENT_BOTTOM:
		rcTabNearMargin.top = rcTabNearMargin.bottom - 1;
	break;
	case __ETWS_ORIENT_LEFT:
		rcTabNearMargin.right = rcTabNearMargin.left + 1;
	break;
	case __ETWS_ORIENT_RIGHT:
		rcTabNearMargin.left = rcTabNearMargin.right - 1;
	break;
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
	break;
#endif // _DEBUG
	} // switch( dwOrientation )

	// fill all area with tab items background color
	dc.FillSolidRect(
		&rcClient,
		clrTabItemsAreaBk
		);

	if( !bGroupedMode )
	{
		// fill tab border area
		dc.FillSolidRect(
			&rcTabNearBorderArea,
			clrTabNearBorderAreaBk
			);
		// paint tab border margin
		dc.FillSolidRect(
			&rcTabNearMargin,
			clrTabNearMargin
			);
	} // if( !bGroupedMode )
#else // (!defined __EXT_MFC_NO_TAB_CTRL)
	dc;
	rcClient;
	rcTabItemsArea;
	rcTabNearBorderArea;
	dwOrientation;
	bGroupedMode;
	clrTabItemsAreaBk;
	clrTabNearBorderAreaBk;
	clrTabNearMargin;
#endif // else from(!defined __EXT_MFC_NO_TAB_CTRL)
}

void CExtPaintManager::stat_PaintTabButtonImpl(
	CDC & dc,
	CRect & rcButton,
	LONG nHitTest,
	bool bTopLeft,
	bool bHorz,
	bool bEnabled,
	bool bHover,
	bool bPushed,
	bool bGroupedMode,
	COLORREF clrGlyph,
	COLORREF clrTL,
	COLORREF clrBR
	)
{
#if (!defined __EXT_MFC_NO_TAB_CTRL)
	bTopLeft;
	bGroupedMode;

CExtPaintManager::glyph_t * pGlyph = NULL;
	switch( nHitTest )
	{
	case __ETWH_BUTTON_LEFTUP:
		pGlyph =
			CExtPaintManager::g_DockingCaptionGlyphs[
				bHorz
					? (	bEnabled
							? CExtPaintManager::__DCBT_ARROW_LEFT
							: CExtPaintManager::__DCBT_HOLLOW_LEFT
						)
					: (	bEnabled
							? CExtPaintManager::__DCBT_ARROW_UP
							: CExtPaintManager::__DCBT_HOLLOW_UP
						)
				];
		ASSERT( pGlyph != NULL );
	break;
	case __ETWH_BUTTON_RIGHTDOWN:
		pGlyph =
			CExtPaintManager::g_DockingCaptionGlyphs[
				bHorz
					? (	bEnabled
						? CExtPaintManager::__DCBT_ARROW_RIGHT
						: CExtPaintManager::__DCBT_HOLLOW_RIGHT
						)
					: (	bEnabled
						? CExtPaintManager::__DCBT_ARROW_DOWN
						: CExtPaintManager::__DCBT_HOLLOW_DOWN
						)
				];
		ASSERT( pGlyph != NULL );
	break;
	case __ETWH_BUTTON_HELP:
		pGlyph =
			CExtPaintManager::g_DockingCaptionGlyphs[
				CExtPaintManager::__DCBT_WND_CONTEXTHELP
				];
		ASSERT( pGlyph != NULL );
	break;
	case __ETWH_BUTTON_CLOSE:
		pGlyph =
			CExtPaintManager::g_DockingCaptionGlyphs[
				CExtPaintManager::__DCBT_CLOSE
				];
		ASSERT( pGlyph != NULL );
	break;
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
	break;
#endif // _DEBUG
	} // switch( nHitTest )

	ASSERT( pGlyph != NULL );
//dc.FillSolidRect( &rcButton, RGB( 255, 128, 128 ) );

COLORREF ColorValues[2] =
{
	RGB(0,0,0),
	clrGlyph
};

	g_PaintManager->PaintGlyphCentered(
		dc,
		rcButton,
		*pGlyph,
		ColorValues
		);

	if( bHover || bPushed )
		dc.Draw3dRect( &rcButton, clrTL, clrBR );
#else // (!defined __EXT_MFC_NO_TAB_CTRL)
	dc;
	rcButton;
	nHitTest;
	bTopLeft;
	bHorz;
	bEnabled;
	bHover;
	bPushed;
	bGroupedMode;
	clrGlyph;
	clrTL;
	clrBR;
#endif // else from(!defined __EXT_MFC_NO_TAB_CTRL)
}

void CExtPaintManager::PaintTabItem(
	CDC & dc,
	CRect & rcTabItemsArea,
	bool bTopLeft,
	bool bHorz,
	bool bSelected,
	bool bCenteredText,
	bool bGroupedMode,
	bool bInGroupActive,
	bool bInvertedVerticalMode,
	const CRect & rcEntireItem,
	CSize sizeTextMeasured,
	CFont * pFont,
	__EXT_MFC_SAFE_LPCTSTR sText,
	CExtCmdIcon * pIcon,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
COLORREF clrText =
		GetColor(
			(bSelected && !bGroupedMode)
				? COLOR_BTNTEXT
				: ( bGroupedMode ? COLOR_BTNTEXT : COLOR_3DHILIGHT)
			);
COLORREF clrTabBk = GetColor( COLOR_BTNFACE );
COLORREF clrTabBorderLT = GetColor(
			bGroupedMode ? COLOR_3DSHADOW : COLOR_3DHILIGHT
			);
COLORREF clrTabBorderRB = GetColor(
			bGroupedMode ? COLOR_3DSHADOW : COLOR_3DDKSHADOW
			);
COLORREF clrTabSeparator =
		GetColor(
			( bGroupedMode ? COLOR_3DSHADOW : COLOR_3DLIGHT)
			);

	CExtPaintManager::stat_PaintTabItemImpl(
		dc,
		rcTabItemsArea,
		bTopLeft,
		bHorz,
		bSelected,
		bCenteredText,
		bGroupedMode,
		bInGroupActive,
		bInvertedVerticalMode,
		rcEntireItem,
		sizeTextMeasured,
		pFont,
		sText,
		pIcon,
		clrText,
		clrTabBk,
		clrTabBorderLT,
		clrTabBorderRB,
		clrTabSeparator
		);
}

void CExtPaintManagerXP::PaintTabItem(
	CDC & dc,
	CRect & rcTabItemsArea,
	bool bTopLeft,
	bool bHorz,
	bool bSelected,
	bool bCenteredText,
	bool bGroupedMode,
	bool bInGroupActive,
	bool bInvertedVerticalMode,
	const CRect & rcEntireItem,
	CSize sizeTextMeasured,
	CFont * pFont,
	__EXT_MFC_SAFE_LPCTSTR sText,
	CExtCmdIcon * pIcon,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
COLORREF clrText =
		GetColor(
			(bSelected || bGroupedMode)
				? COLOR_BTNTEXT
				: COLOR_3DSHADOW
			);
COLORREF clrTabBk = GetColor(
			bGroupedMode ? COLOR_3DFACE : XPCLR_3DFACE_DARK
			);
COLORREF clrTabBorderLT = GetColor(
			bGroupedMode ? COLOR_3DSHADOW : COLOR_3DHILIGHT
			);
COLORREF clrTabBorderRB = GetColor(
			bGroupedMode ? COLOR_3DSHADOW : COLOR_3DDKSHADOW
			);
COLORREF clrTabSeparator = GetColor(
			COLOR_3DSHADOW
			);

	CExtPaintManager::stat_PaintTabItemImpl(
		dc,
		rcTabItemsArea,
		bTopLeft,
		bHorz,
		bSelected,
		bCenteredText,
		bGroupedMode,
		bInGroupActive,
		bInvertedVerticalMode,
		rcEntireItem,
		sizeTextMeasured,
		pFont,
		sText,
		pIcon,
		clrText,
		clrTabBk,
		clrTabBorderLT,
		clrTabBorderRB,
		clrTabSeparator
		);
}

void CExtPaintManager::PaintTabClientArea(
	CDC & dc,
	CRect & rcClient,
	CRect & rcTabItemsArea,
	CRect & rcTabNearBorderArea,
	DWORD dwOrientation,
	bool bGroupedMode,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
#if (!defined __EXT_MFC_NO_TAB_CTRL)
int idxClrTabNearMargin = COLOR_3DDKSHADOW;
	switch( dwOrientation )
	{
	case __ETWS_ORIENT_TOP:
		idxClrTabNearMargin = COLOR_3DHILIGHT;
	break;
	case __ETWS_ORIENT_BOTTOM:
	break;
	case __ETWS_ORIENT_LEFT:
		idxClrTabNearMargin = COLOR_3DHILIGHT;
	break;
	case __ETWS_ORIENT_RIGHT:
	break;
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
	break;
#endif // _DEBUG
	} // switch( dwOrientation )

COLORREF clrTabItemsAreaBk =
		GetColor(
			bGroupedMode ? COLOR_3DFACE : COLOR_3DSHADOW
			);
COLORREF clrTabNearBorderAreaBk =
		GetColor( COLOR_3DFACE );
COLORREF clrTabNearMargin = 
		GetColor( idxClrTabNearMargin );

	CExtPaintManager::stat_PaintTabClientAreaImpl(
		dc,
		rcClient,
		rcTabItemsArea,
		rcTabNearBorderArea,
		dwOrientation,
		bGroupedMode,
		clrTabItemsAreaBk,
		clrTabNearBorderAreaBk,
		clrTabNearMargin
		);
#else // (!defined __EXT_MFC_NO_TAB_CTRL)
	dc;
	rcClient;
	rcTabItemsArea;
	rcTabNearBorderArea;
	dwOrientation;
	bGroupedMode;
#endif // else from(!defined __EXT_MFC_NO_TAB_CTRL)
}

void CExtPaintManagerXP::PaintTabClientArea(
	CDC & dc,
	CRect & rcClient,
	CRect & rcTabItemsArea,
	CRect & rcTabNearBorderArea,
	DWORD dwOrientation,
	bool bGroupedMode,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
#if (!defined __EXT_MFC_NO_TAB_CTRL)
int idxClrTabNearMargin = COLOR_3DSHADOW;
	switch( dwOrientation )
	{
	case __ETWS_ORIENT_TOP:
		idxClrTabNearMargin = COLOR_3DHILIGHT;
	break;
	case __ETWS_ORIENT_BOTTOM:
	break;
	case __ETWS_ORIENT_LEFT:
		idxClrTabNearMargin = COLOR_3DHILIGHT;
	break;
	case __ETWS_ORIENT_RIGHT:
	break;
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
	break;
#endif // _DEBUG
	} // switch( dwOrientation )

COLORREF clrTabItemsAreaBk =
		GetColor( XPCLR_3DFACE_NORMAL );
COLORREF clrTabNearBorderAreaBk =
		GetColor( XPCLR_3DFACE_DARK );
COLORREF clrTabNearMargin = 
		GetColor( idxClrTabNearMargin );

	CExtPaintManager::stat_PaintTabClientAreaImpl(
		dc,
		rcClient,
		rcTabItemsArea,
		rcTabNearBorderArea,
		dwOrientation,
		bGroupedMode,
		clrTabItemsAreaBk,
		clrTabNearBorderAreaBk,
		clrTabNearMargin
		);
#else // (!defined __EXT_MFC_NO_TAB_CTRL)
	dc;
	rcClient;
	rcTabItemsArea;
	rcTabNearBorderArea;
	dwOrientation;
	bGroupedMode;
#endif // else from(!defined __EXT_MFC_NO_TAB_CTRL)
}

void CExtPaintManagerOffice2003::PaintTabClientArea(
	CDC & dc,
	CRect & rcClient,
	CRect & rcTabItemsArea,
	CRect & rcTabNearBorderArea,
	DWORD dwOrientation,
	bool bGroupedMode,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
#if (!defined __EXT_MFC_NO_TAB_CTRL)
	bool bFrameBackground = false;
	if(		pHelperSrc != NULL
		&&	pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtTabWnd))
		&&	m_bmpDockerGradient.GetSafeHandle() != NULL
		&&	stat_GetBPP() > 8
		)
	{
		if(
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
			pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtTabMdiWnd))
			||
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)
			((CExtTabWnd*)pHelperSrc)->m_bReflectParentSizing
			)
			bFrameBackground = true;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		else if(	pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtDynTabWnd))
				||	pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtDynAutoHideArea))
			)
			bFrameBackground = true;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	}
	if( bFrameBackground )
	{
		CRect rcTabNearMargin( rcTabNearBorderArea ); // prepare tab border margin rect
		switch( dwOrientation )
		{
		case __ETWS_ORIENT_TOP:
			rcTabNearMargin.bottom = rcTabNearMargin.top + 1;
		break;
		case __ETWS_ORIENT_BOTTOM:
			rcTabNearMargin.top = rcTabNearMargin.bottom - 1;
		break;
		case __ETWS_ORIENT_LEFT:
			rcTabNearMargin.right = rcTabNearMargin.left + 1;
		break;
		case __ETWS_ORIENT_RIGHT:
			rcTabNearMargin.left = rcTabNearMargin.right - 1;
		break;
#ifdef _DEBUG
		default:
			ASSERT( FALSE );
		break;
#endif // _DEBUG
		} // switch( dwOrientation )
		CWnd * pFrame =
			((CExtTabWnd*)pHelperSrc)->GetParentFrame();
		if( pFrame == NULL )
			pFrame =
				((CExtTabWnd*)pHelperSrc)->GetParent();
		ASSERT_VALID( pFrame );
		CRect rcPaintGradient;
		pFrame->GetClientRect( &rcPaintGradient );
		pFrame->ClientToScreen( &rcPaintGradient );
		((CExtTabWnd*)pHelperSrc)->ScreenToClient( &rcPaintGradient );
		CDC & dcCompat = CachedDcGet( m_bmpDockerGradient );
		if( dcCompat.GetSafeHdc() == NULL )
		{
			ASSERT( FALSE );
			return;
		}
		int nOldStretchBltMode =
			dc.SetStretchBltMode(
				g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
				);
		::SetBrushOrgEx( dc, 0, 0, NULL );
		
		dc.StretchBlt(
			rcPaintGradient.left, rcPaintGradient.top,
			rcPaintGradient.Width(), rcPaintGradient.Height(),
			&dcCompat,
			0, 0, m_sizeDockerGradient.cx, m_sizeDockerGradient.cy,
			SRCCOPY
			);
		
		dc.SetStretchBltMode( nOldStretchBltMode );
		if( !bGroupedMode )
		{
COLORREF clrTabNearMargin = GetColor( COLOR_3DSHADOW );
COLORREF clrTabNearBorderAreaBk = GetColor( CLR_3DFACE_OUT );
			// fill tab border area
			dc.FillSolidRect(
				&rcTabNearBorderArea,
				clrTabNearBorderAreaBk
				);
			// paint tab border margin
			dc.FillSolidRect(
				&rcTabNearMargin,
				clrTabNearMargin
				);
		} // if( !bGroupedMode )
		return;
	} // if( bFrameBackground )
	CExtPaintManagerXP::PaintTabClientArea(
		dc,
		rcClient,
		rcTabItemsArea,
		rcTabNearBorderArea,
		dwOrientation,
		bGroupedMode,
		pHelperSrc,
		lParam
		);
#else // (!defined __EXT_MFC_NO_TAB_CTRL)
	dc;
	rcClient;
	rcTabItemsArea;
	rcTabNearBorderArea;
	dwOrientation;
	bGroupedMode;
#endif // else from(!defined __EXT_MFC_NO_TAB_CTRL)
}

void CExtPaintManager::PaintTabButton(
	CDC & dc,
	CRect & rcButton,
	LONG nHitTest,
	bool bTopLeft,
	bool bHorz,
	bool bEnabled,
	bool bHover,
	bool bPushed,
	bool bGroupedMode,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
COLORREF clrGlyph = 
		g_PaintManager->GetColor(
			bGroupedMode
				? (
					bEnabled
						? COLOR_BTNTEXT
						: COLOR_3DSHADOW
					)
				: (
					bEnabled
						? COLOR_3DHIGHLIGHT
						: COLOR_3DFACE
					)
			);
COLORREF clrTL =
		g_PaintManager->GetColor(
			bPushed
				? COLOR_3DDKSHADOW
				: COLOR_3DHILIGHT
			);
COLORREF clrBR =
		g_PaintManager->GetColor(
			bPushed
				? COLOR_3DHILIGHT
				: COLOR_3DDKSHADOW
			);

	CExtPaintManager::stat_PaintTabButtonImpl(
		dc,
		rcButton,
		nHitTest,
		bTopLeft,
		bHorz,
		bEnabled,
		bHover,
		bPushed,
		bGroupedMode,
		clrGlyph,
		clrTL,
		clrBR
		);
}

void CExtPaintManagerXP::PaintTabButton(
	CDC & dc,
	CRect & rcButton,
	LONG nHitTest,
	bool bTopLeft,
	bool bHorz,
	bool bEnabled,
	bool bHover,
	bool bPushed,
	bool bGroupedMode,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
COLORREF clrGlyph = 
		g_PaintManager->GetColor(
			bEnabled
				? COLOR_BTNTEXT
				: COLOR_3DSHADOW
			);
COLORREF clrTL =
		g_PaintManager->GetColor(
			bPushed
				? COLOR_3DDKSHADOW
				: COLOR_3DHILIGHT
			);
COLORREF clrBR =
		g_PaintManager->GetColor(
			bPushed
				? COLOR_3DHILIGHT
				: COLOR_3DDKSHADOW
			);

	CExtPaintManager::stat_PaintTabButtonImpl(
		dc,
		rcButton,
		nHitTest,
		bTopLeft,
		bHorz,
		bEnabled,
		bHover,
		bPushed,
		bGroupedMode,
		clrGlyph,
		clrTL,
		clrBR
		);
}

void CExtPaintManager::PaintTabNcAreaRect(
	CDC & dc,
	const RECT & rc,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
COLORREF clrTL = GetColor( COLOR_3DSHADOW );
COLORREF clrBR = GetColor( COLOR_3DHILIGHT );
	dc.Draw3dRect(
		&rc,
		clrTL,
		clrBR
		);
}

void CExtPaintManagerXP::PaintTabNcAreaRect(
	CDC & dc,
	const RECT & rc,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
COLORREF clrTL = GetColor( COLOR_3DSHADOW );
COLORREF clrBR = clrTL;
	dc.Draw3dRect(
		&rc,
		clrTL,
		clrBR
		);
}

void CExtPaintManager::PaintDockBarClientArea(
	CDC & dc,
	const RECT & rcClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;
	if(	::IsRectEmpty( &rcClient )
		|| ( ! dc.RectVisible( &rcClient ) )
		)
		return;
	dc.FillSolidRect(
		&rcClient,
		GetColor( COLOR_3DFACE )
		);
}

void CExtPaintManagerXP::PaintDockBarClientArea(
	CDC & dc,
	const RECT & rcClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	CExtPaintManager::PaintDockBarClientArea(
		dc,
		rcClient,
		pHelperSrc,
		lParam
		);
}

bool CExtPaintManager::GetCb2DbTransparentMode(
	CObject * pObjHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pObjHelperSrc;
	lParam;
	return false;
}

bool CExtPaintManagerXP::GetCb2DbTransparentMode(
	CObject * pObjHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pObjHelperSrc;
	lParam;
	return false;
}

bool CExtPaintManagerOffice2003::GetCb2DbTransparentMode(
	CObject * pObjHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pObjHelperSrc;
	lParam;
	if( stat_GetBPP() <= 8 )
		return false;
	return true;
}

bool CExtPaintManager::PaintDockerBkgnd(
	CDC & dc,
	CWnd * pWnd,
	LPARAM lParam // = NULL
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( pWnd->GetSafeHwnd() != NULL && ::IsWindow(pWnd->GetSafeHwnd()) );
	dc;
	pWnd;
	lParam;
	return false;
}

bool CExtPaintManagerXP::PaintDockerBkgnd(
	CDC & dc,
	CWnd * pWnd,
	LPARAM lParam // = NULL
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( pWnd->GetSafeHwnd() != NULL && ::IsWindow(pWnd->GetSafeHwnd()) );
	dc;
	pWnd;
	lParam;
	return false;
}

bool CExtPaintManagerOffice2003::PaintDockerBkgnd(
	CDC & dc,
	CWnd * pWnd,
	LPARAM lParam // = NULL
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( pWnd->GetSafeHwnd() != NULL && ::IsWindow(pWnd->GetSafeHwnd()) );
	lParam;
	if(		(! GetCb2DbTransparentMode(pWnd) )
		||	m_bmpDockerGradient.GetSafeHandle() == NULL
		)
		return false;
COLORREF clrSolidShade0 = ((COLORREF)(-1L));
COLORREF clrSolidShade1 = ((COLORREF)(-1L));
bool bToolbarShade = false;
bool bToolbarHorz = false;
	for(	CWnd * pFrame = pWnd;
			pFrame != NULL;
			pFrame = pFrame->GetParent()
			)
	{
		if( (pFrame->GetStyle() & WS_CHILD) == 0 )
		{
			if(! pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
				return false;
			break;
		}
		if(		pFrame != pWnd
			&&	pFrame->IsKindOf( RUNTIME_CLASS(CExtToolControlBar) )
			)
		{
			if( ((CExtToolControlBar*)pFrame)->m_pDockSite != NULL )
			{
				if(	!(	pFrame->IsKindOf( RUNTIME_CLASS(CExtPanelControlBar) )
					||	pFrame->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) )
						)
					)
				{
					if( ((CExtToolControlBar*)pFrame)->IsFloating() )
					{
						clrSolidShade0 = GetColor( _2003CLR_MLA_NORM_RIGHT );
						clrSolidShade1 = GetColor( _2003CLR_SEPARATOR_LIGHT );
					}
					else
					{
						bToolbarShade = true;
						bToolbarHorz = ((CExtToolControlBar*)pFrame)->IsDockedHorizontally();
					}
				}
			}
			break;
		}
	}
	if( pFrame == NULL )
		return false;

CRect rcPaintGradient;
	pFrame->GetClientRect( &rcPaintGradient );
	pFrame->ClientToScreen( &rcPaintGradient );
	pWnd->ScreenToClient( &rcPaintGradient );
	if(	::IsRectEmpty( &rcPaintGradient )
		|| ( ! dc.RectVisible( &rcPaintGradient ) )
		)
		return true;

	if( clrSolidShade0 != ((COLORREF)(-1L)) )
	{
		if( clrSolidShade1 != ((COLORREF)(-1L)) )
			stat_PaintGradientRect( dc, rcPaintGradient, clrSolidShade0, clrSolidShade1, true );
		else
			dc.FillSolidRect( rcPaintGradient, clrSolidShade0 );
		return true;
	} // if( clrSolidShade0 != ((COLORREF)(-1L)) )

CDC & dcCompat =
		CachedDcGet(
			bToolbarShade
				? ( bToolbarHorz ? m_bmpCtBarGradientHorz : m_bmpCtBarGradientVert )
				: m_bmpDockerGradient
			);
	if( dcCompat.GetSafeHdc() == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
CSize sizeShade =
		bToolbarShade
			? ( bToolbarHorz ? m_sizeCtBarGradientHorz : m_sizeCtBarGradientVert )
			: m_sizeDockerGradient
			;
int nOldStretchBltMode =
		dc.SetStretchBltMode(
			g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
			);
	::SetBrushOrgEx( dc, 0, 0, NULL );
	
	dc.StretchBlt(
		rcPaintGradient.left, rcPaintGradient.top,
		rcPaintGradient.Width(), rcPaintGradient.Height(),
		&dcCompat,
		0, 0, sizeShade.cx, sizeShade.cy,
		SRCCOPY
		);
	
	dc.SetStretchBltMode( nOldStretchBltMode );
	
	return true;
}

void CExtPaintManager::PaintControlBarClientArea(
	CDC & dc,
	const RECT & rcClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
	if(	::IsRectEmpty( &rcClient )
		|| ( ! dc.RectVisible( &rcClient ) )
		)
		return;
	dc.FillSolidRect(
		&rcClient,
		GetColor( CLR_3DFACE_OUT )
		);
}

void CExtPaintManagerXP::PaintControlBarClientArea(
	CDC & dc,
	const RECT & rcClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	CExtPaintManager::PaintControlBarClientArea(
		dc,
		rcClient,
		pHelperSrc,
		lParam
		);
}

void CExtPaintManagerOffice2003::InitTranslatedColors()
{
	m_nIdxClrBtnHoverLeft = -1;
	m_nIdxClrBtnHoverRight = -1;
	m_nIdxClrBtnPressedLeft = -1;
	m_nIdxClrBtnPressedRight = -1;
	m_nIdxClrBtnHovPresLeft = -1;
	m_nIdxClrBtnHovPresRight = -1;
	m_nIdxClrTbFillMargin = -1;
	g_PaintManager.m_bUxApiInitPassed
		= g_PaintManager.m_bUxValidColorsExtracted
		= false;
	g_PaintManager.InitUserExApi();
	CExtPaintManagerXP::InitTranslatedColors();
	if( m_bmpCtBarGradientHorz.GetSafeHandle() != NULL )
		m_bmpCtBarGradientHorz.DeleteObject();
	if( m_bmpCtBarGradientVert.GetSafeHandle() != NULL )
		m_bmpCtBarGradientVert.DeleteObject();
	if( m_bmpDockerGradient.GetSafeHandle() != NULL )
		m_bmpDockerGradient.DeleteObject();
	if( stat_GetBPP() > 8 )
	{
		static const LONG g_nBarGradientSizeMetric = 256L;
		static const LONG g_nBarGradientPixelCount = 256L*256L;
		m_sizeCtBarGradientHorz.cx
			= m_sizeCtBarGradientHorz.cy
			= m_sizeCtBarGradientVert.cx
			= m_sizeCtBarGradientVert.cy
			= m_sizeDockerGradient.cx
			= m_sizeDockerGradient.cy
			= g_nBarGradientSizeMetric;
		CWindowDC dcDesktop( NULL );
		if(		g_PaintManager.m_bUxApiInitPassed
			&&	g_PaintManager.m_bUxValidColorsExtracted
			&&	g_PaintManager.m_bUxUseIfAvailOnWinXpOrLater
			)
		{ // if use WinXP themed colors

			double	lfEdgeLightHue, lfEdgeLightSat, lfEdgeLightLum,
				lfEdgeShadowHue, lfEdgeShadowSat, lfEdgeShadowLum,
				lfCustFillHue, lfCustFillSat, lfCustFillLum;
			stat_RGBtoHSL(
				g_PaintManager.m_clrUxTaskBandEdgeLightColor,
				&lfEdgeLightHue, &lfEdgeLightSat, &lfEdgeLightLum
				);
			stat_RGBtoHSL(
				g_PaintManager.m_clrUxTaskBandEdgeShadowColor,
				&lfEdgeShadowHue, &lfEdgeShadowSat, &lfEdgeShadowLum
				);
			stat_RGBtoHSL(
				g_PaintManager.m_clrUxTaskBandFillColorHint,
				&lfCustFillHue, &lfCustFillSat, &lfCustFillLum
				);
			COLORREF clrFillHint =
				stat_HLStoRGB(
					lfCustFillHue,
					(lfEdgeLightLum+lfEdgeShadowLum)/2.0,
					lfCustFillSat
					);

			COLORREF clrAccentHint = g_PaintManager.m_clrUxTaskBandAccentColorHint;

			// re-install XP colors

			// xp - menu area light ver
			COLORREF xpclr_MenuLight = stat_HLS_Adjust( clrFillHint, 0.00, 0.95, -0.05 );
			xpclr_MenuLight = RGB(
				GetRValue(xpclr_MenuLight) + ::MulDiv( 255 - GetRValue(xpclr_MenuLight), 40, 100 ),
				GetGValue(xpclr_MenuLight) + ::MulDiv( 255 - GetGValue(xpclr_MenuLight), 20, 100 ),
				GetBValue(xpclr_MenuLight) );
			xpclr_MenuLight = stat_HLS_Adjust( xpclr_MenuLight, 0.00, 0.06, 0.05 );
			// 2003 - float light
			COLORREF xpclr_FloatFace =
				stat_HLS_Adjust( clrFillHint, 0.00, 0.50, 0.00 );
			// xp - rarely used item left side
			COLORREF xpclr_RarelyUsedMenuLeft = RGB(
				( 999L*long(GetRValue(clrFillHint)) ) / 1000L,
				( 995L*long(GetGValue(clrFillHint)) ) / 1000L,
				( 995L*long(GetBValue(clrFillHint)) ) / 1000L );
			// xp - control bar funny background
			COLORREF xpclr_ControlBarBk = stat_HLS_Adjust( clrFillHint, 0.00, 0.20, 0.00 );
			// xp - orange like light ver
			COLORREF xpclr_Highlight = stat_HLS_Adjust( clrAccentHint, 0.00, 0.60, 0.45 );
			// 2003 - dark orange
			COLORREF clr2003faceIn = stat_HLS_Adjust( clrAccentHint, 0.00, -0.30, 0.45 );
			// xp - blue like dark ver
			COLORREF clrTmp = stat_HLS_Adjust( clrFillHint, 0.00, 0.20, 0.00 );
			COLORREF xpclr_HighlightDarked = stat_HLS_Adjust( clrTmp, 0.00, -0.25, 0.10 );
			// xp - dark selected border
			COLORREF xpclr_HighlightBorder = stat_HLS_Adjust( xpclr_HighlightDarked, 0.00, -0.50, 0.00 );
			// xp - dark gray separator
			COLORREF xpclr_Separator = RGB(
					( 857L*long(GetRValue(clrFillHint)) ) / 1000L,
					( 857L*long(GetGValue(clrFillHint)) ) / 1000L,
					( 857L*long(GetBValue(clrFillHint)) ) / 1000L );
			// xp - dark panel border (for floating controlbars)
			COLORREF xpclr_PanelBorder =
				stat_HLS_Adjust( clrFillHint, 0.00, -0.25, 0.00 );

			m_mapColorTranslate[CLR_3DFACE_OUT]				= InstallColor( xpclr_ControlBarBk );
			m_mapColorTranslate[CLR_3DFACE_IN] = InstallColor( xpclr_Highlight );
			m_mapColorTranslate[CLR_3DFACE_DISABLED]		= COLOR_3DFACE;

			int idxClrDark = InstallColor( xpclr_PanelBorder );

			m_mapColorTranslate[CLR_3DLIGHT_OUT]			= idxClrDark;
			m_mapColorTranslate[CLR_3DLIGHT_IN]				= idxClrDark;
			m_mapColorTranslate[CLR_3DLIGHT_DISABLED]		= idxClrDark;

			m_mapColorTranslate[CLR_3DHILIGHT_OUT]			= idxClrDark;
			m_mapColorTranslate[CLR_3DHILIGHT_IN]			= idxClrDark;
			m_mapColorTranslate[CLR_3DHILIGHT_DISABLED]		= idxClrDark;

			m_mapColorTranslate[CLR_3DSHADOW_OUT]			= idxClrDark;
			m_mapColorTranslate[CLR_3DSHADOW_IN]			= idxClrDark;
			m_mapColorTranslate[CLR_3DSHADOW_DISABLED]		= idxClrDark;

			m_mapColorTranslate[CLR_3DDKSHADOW_OUT]			= idxClrDark;
			m_mapColorTranslate[CLR_3DDKSHADOW_IN]			= idxClrDark;
			m_mapColorTranslate[CLR_3DDKSHADOW_DISABLED]	= idxClrDark;
			
			m_mapColorTranslate[CLR_TEXT_OUT] = COLOR_BTNTEXT;
			m_mapColorTranslate[CLR_TEXT_IN] = COLOR_BTNTEXT;
			m_mapColorTranslate[CLR_TEXT_DISABLED] = COLOR_3DSHADOW;
			m_mapColorTranslate[XPCLR_PUSHEDHOVERTEXT] = COLOR_BTNTEXT;

			m_mapColorTranslate[XPCLR_3DFACE_DARK]			= InstallColor( xpclr_ControlBarBk );
			m_mapColorTranslate[XPCLR_3DFACE_NORMAL]		= InstallColor( xpclr_MenuLight );
			m_mapColorTranslate[XPCLR_SEPARATOR]			= InstallColor( xpclr_Separator ); 

			m_mapColorTranslate[XPCLR_HILIGHT] =
				//InstallColor( xpclr_HighlightDarked ); 
				InstallColor( clr2003faceIn ); 

			m_mapColorTranslate[XPCLR_HILIGHT_BORDER]		= InstallColor( xpclr_HighlightBorder ); 
			
			m_mapColorTranslate[XPCLR_RARELY_BORDER]		= InstallColor( xpclr_RarelyUsedMenuLeft );

			m_mapColorTranslate[XPCLR_MENU_BORDER] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, -0.68, 0.00 )
					);
			m_mapColorTranslate[XPCLR_3DFACE_FLOAT_F]			= InstallColor( xpclr_FloatFace );
			
			// install new colors
			m_mapColorTranslate[_2003CLR_GRADIENT_LIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, 0.30, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_GRADIENT_DARK] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, 0.00, 0.10 )
					);
			m_mapColorTranslate[_2003CLR_SEPARATOR_LIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, 0.98, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_SEPARATOR_DARK] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, -0.45, -0.00 )
					);
			m_mapColorTranslate[_2003CLR_GRIPPER_DOT_DARK] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, -0.60, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_GRIPPER_DOT_LIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, 0.95, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_EXPBTN_LIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, -0.10, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_EXPBTN_DARK] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, -0.60, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_EXPBTN_HOVER_LIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrAccentHint, 0.00, 0.85, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_EXPBTN_HOVER_DARK] =
				InstallColor(
					stat_HLS_Adjust( clrAccentHint, 0.00, -0.05, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_EXPBTN_PRESSED_LIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrAccentHint, 0.00, 0.30, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_EXPBTN_PRESSED_DARK] =
				InstallColor(
					stat_HLS_Adjust( clrAccentHint, 0.00, -0.30, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_EXPGLYPH_LIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, 0.97, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_EXPGLYPH_DARK] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, -0.97, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_STATUSBAR_ITEM] =
				COLOR_3DSHADOW;
//				InstallColor(
//					stat_HLS_Adjust(
//						GetColor(XPCLR_3DFACE_NORMAL),
//						0.0, -0.05, 0.0
//						)
//					);
			m_mapColorTranslate[_2003CLR_MLA_NORM_LEFT] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, 0.75, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_MLA_NORM_RIGHT] =
				InstallColor(
					stat_HLS_Adjust( GetColor( XPCLR_3DFACE_DARK ), 0.00, -0.10, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_MLA_RARELY_LEFT] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, 0.30, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_MLA_RARELY_RIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, -0.25, 0.00 )
					);

			m_mapColorTranslate[_2003CLR_BTN_HOVER_LEFT] =
				InstallColor(
					stat_HLS_Adjust( clrAccentHint, 0.00, 0.80, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_BTN_HOVER_RIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrAccentHint, 0.00, 0.20, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_BTN_PRESSED_LEFT] =
				InstallColor(
					stat_HLS_Adjust( clrAccentHint, 0.00, 0.40, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_BTN_PRESSED_RIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrAccentHint, 0.00, -0.05, 0.00 )
					);
			m_mapColorTranslate[_2003CLR_BTN_HP_LEFT] =
				InstallColor(
					stat_HLS_Adjust( clrAccentHint, 0.00, -0.30, 0.75 )
					);
			m_mapColorTranslate[_2003CLR_BTN_HP_RIGHT] =
				InstallColor(
					stat_HLS_Adjust( clrAccentHint, 0.00, 0.50, 0.00 )
					);

			m_nIdxClrBtnHoverLeft = _2003CLR_BTN_HOVER_LEFT;
			m_nIdxClrBtnHoverRight = _2003CLR_BTN_HOVER_RIGHT;
			m_nIdxClrBtnPressedLeft = _2003CLR_BTN_PRESSED_LEFT;
			m_nIdxClrBtnPressedRight = _2003CLR_BTN_PRESSED_RIGHT;
			m_nIdxClrBtnHovPresLeft = _2003CLR_BTN_HP_LEFT;
			m_nIdxClrBtnHovPresRight = _2003CLR_BTN_HP_RIGHT;
			m_nIdxClrTbFillMargin = _2003CLR_EXPBTN_DARK;

			m_mapColorTranslate[_2003CLR_MENUEXPBALOON] =
				InstallColor(
					stat_HLS_Adjust( clrFillHint, 0.00, 0.77, 0.00 )
					);

			m_colors[COLOR_3DFACE] =
				clrFillHint;
			m_colors[COLOR_3DLIGHT] =
				stat_HLS_Adjust( clrFillHint, 0.00, 0.45, 0.10 );
			m_colors[COLOR_3DHIGHLIGHT] =
				stat_HLS_Adjust( clrFillHint, 0.00, 0.75, 0.00 );
			m_colors[COLOR_3DSHADOW] =
				stat_HLS_Adjust( clrFillHint, 0.00, -0.45, 0.10 );
			m_colors[COLOR_3DDKSHADOW] =
				stat_HLS_Adjust( clrFillHint, 0.00, -0.75, 0.00 );

			// init non-linear gradient bitmaps
			COLORREF clrLeft =
				stat_HLS_Adjust(
					GetColor( _2003CLR_GRADIENT_LIGHT ),
					0.00, 0.50, 0.20
					);
			COLORREF clrRight =
				stat_HLS_Adjust(
					GetColor( _2003CLR_GRADIENT_DARK ),
					0.00, -0.30, -0.20
					);
			COLORREF clrQuadLeft =
				RGB(
					GetBValue( clrLeft ),
					GetGValue( clrLeft ),
					GetRValue( clrLeft )
					);
			COLORREF clrQuadRight =
				RGB(
					GetBValue( clrRight ),
					GetGValue( clrRight ),
					GetRValue( clrRight )
					);
			double lfColorDist[3] =
			{
				double( GetRValue( clrQuadRight ) ) - double( GetRValue( clrQuadLeft ) ),
				double( GetGValue( clrQuadRight ) ) - double( GetGValue( clrQuadLeft ) ),
				double( GetBValue( clrQuadRight ) ) - double( GetBValue( clrQuadLeft ) ),
			};

			// horizontal bar gradient
			BITMAPINFOHEADER bih;
			::memset( &bih, 0, sizeof(BITMAPINFOHEADER) );
			bih.biSize = sizeof(BITMAPINFOHEADER);
			bih.biWidth = bih.biHeight = g_nBarGradientSizeMetric;
			bih.biPlanes = 1;
			bih.biBitCount = 32;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = g_nBarGradientPixelCount;
			COLORREF * pSurfaceCB = NULL;
			HBITMAP hDib =
				::CreateDIBSection(
					dcDesktop,
					(LPBITMAPINFO)&bih,
					DIB_RGB_COLORS,
					(void **)&pSurfaceCB,
					NULL,
					NULL
					);
			ASSERT( hDib != NULL && pSurfaceCB != NULL );
			if( hDib != NULL && pSurfaceCB != NULL )
			{
				m_bmpCtBarGradientHorz.Attach( hDib );
				for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
				{
					for( LONG y = 0; y < g_nBarGradientSizeMetric; y++ )
					{
						COLORREF * pClr =
							pSurfaceCB + g_nBarGradientPixelCount
							- (y * g_nBarGradientSizeMetric)
							- g_nBarGradientSizeMetric + x
							;
						double lfColorOffset[3] =
						{
							lfColorDist[0],
							lfColorDist[1],
							lfColorDist[2],
						};
						double lfQ =
							( double(g_nBarGradientSizeMetric) - double(y)*2.0 )
							/ double(g_nBarGradientSizeMetric)
							;
						ASSERT( (-1.0) <= lfQ && lfQ <= (1.0) );
						double lfQ2 = lfQ * ( (lfQ > 0.0) ? lfQ : (-lfQ) );
						lfQ = (lfQ*4.0 + lfQ2) / 5.0;
						for( int i = 0; i < 3; i++ )
						{
							double & lfCV = lfColorOffset[i];
							lfCV /= lfColorDist[i];
							if( lfQ > 0.0 )
								lfCV = lfCV + (1.0 - lfCV) * lfQ;
							else if ( lfQ < 0.0 )
								lfCV = lfCV * (1.0 + lfQ);
							if( lfCV < 0.0 )
								lfCV = 0.0;
							else if( lfCV > 1.0 )
								lfCV = 1.0;
							lfCV *= lfColorDist[i];
						} // for( int i = 0; i < 3; i++ )
						*pClr =
							RGB(
								DWORD( double(GetRValue(clrQuadRight)) - lfColorOffset[0] ),
								DWORD( double(GetGValue(clrQuadRight)) - lfColorOffset[1] ),
								DWORD( double(GetBValue(clrQuadRight)) - lfColorOffset[2] )
								);
					}
				} // for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
			} // if( hDib != NULL && pSurfaceCB != NULL )

			// vertical bar gradient
			::memset( &bih, 0, sizeof(BITMAPINFOHEADER) );
			bih.biSize = sizeof(BITMAPINFOHEADER);
			bih.biWidth = bih.biHeight = g_nBarGradientSizeMetric;
			bih.biPlanes = 1;
			bih.biBitCount = 32;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = g_nBarGradientPixelCount;
			pSurfaceCB = NULL;
			hDib =
				::CreateDIBSection(
					dcDesktop,
					(LPBITMAPINFO)&bih,
					DIB_RGB_COLORS,
					(void **)&pSurfaceCB,
					NULL,
					NULL
					);
			ASSERT( hDib != NULL && pSurfaceCB != NULL );
			if( hDib != NULL && pSurfaceCB != NULL )
			{
				m_bmpCtBarGradientVert.Attach( hDib );
				for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
				{
					for( LONG y = 0; y < g_nBarGradientSizeMetric; y++ )
					{
						COLORREF * pClr =
							pSurfaceCB + g_nBarGradientPixelCount
							- (y * g_nBarGradientSizeMetric)
							- g_nBarGradientSizeMetric + x
							;
						double lfColorOffset[3] =
						{
							lfColorDist[0],
							lfColorDist[1],
							lfColorDist[2],
						};
						double lfQ =
							( double(g_nBarGradientSizeMetric) - double(x)*2.0 )
							/ double(g_nBarGradientSizeMetric)
							;
						ASSERT( (-1.0) <= lfQ && lfQ <= (1.0) );
						double lfQ2 = lfQ * ( (lfQ > 0.0) ? lfQ : (-lfQ) );
						lfQ = (lfQ*4.0 + lfQ2) / 5.0;
						for( int i = 0; i < 3; i++ )
						{
							double & lfCV = lfColorOffset[i];
							lfCV /= lfColorDist[i];
							if( lfQ > 0.0 )
								lfCV = lfCV + (1.0 - lfCV) * lfQ;
							else if ( lfQ < 0.0 )
								lfCV = lfCV * (1.0 + lfQ);
							if( lfCV < 0.0 )
								lfCV = 0.0;
							else if( lfCV > 1.0 )
								lfCV = 1.0;
							lfCV *= lfColorDist[i];
						} // for( int i = 0; i < 3; i++ )
						*pClr =
							RGB(
								DWORD( double(GetRValue(clrQuadRight)) - lfColorOffset[0] ),
								DWORD( double(GetGValue(clrQuadRight)) - lfColorOffset[1] ),
								DWORD( double(GetBValue(clrQuadRight)) - lfColorOffset[2] )
								);
					}
				} // for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
			} // if( hDib != NULL && pSurfaceCB != NULL )
		} // if use WinXP themed colors
		else
		{ // if use system colors
			// install new colors
			m_mapColorTranslate[_2003CLR_GRADIENT_LIGHT] =
				COLOR_WINDOW;
			m_mapColorTranslate[_2003CLR_GRADIENT_DARK] =
				COLOR_3DFACE;
			m_mapColorTranslate[_2003CLR_SEPARATOR_LIGHT] =
				COLOR_WINDOW;
			m_mapColorTranslate[_2003CLR_SEPARATOR_DARK] =
				InstallColor(
					stat_HLS_Adjust(
						GetColor(COLOR_3DFACE),
						0.0, -0.20, 0.0
						)
					);
			m_mapColorTranslate[_2003CLR_GRIPPER_DOT_DARK] =
				InstallColor(
					stat_HLS_Adjust(
						GetColor(COLOR_3DSHADOW),
						0.0, 0.20, 0.0
						)
					);
			m_mapColorTranslate[_2003CLR_GRIPPER_DOT_LIGHT] =
				COLOR_WINDOW;
			m_mapColorTranslate[_2003CLR_EXPBTN_LIGHT] =
				InstallColor(
					stat_HLS_Adjust(
						GetColor(COLOR_3DSHADOW),
						0.0, 0.25, 0.0
						)
					);
			m_mapColorTranslate[_2003CLR_EXPBTN_DARK] =
				InstallColor(
					stat_HLS_Adjust(
						GetColor(COLOR_3DSHADOW),
						0.0, -0.10, 0.0
						)
					);
			m_mapColorTranslate[_2003CLR_EXPBTN_HOVER_LIGHT] =
				m_mapColorTranslate[_2003CLR_EXPBTN_HOVER_DARK] =
				InstallColor(
					stat_HLS_Adjust(
						GetColor(XPCLR_HILIGHT),
						0.0, 0.30, 0.0
						)
					);
			m_mapColorTranslate[_2003CLR_EXPBTN_PRESSED_LIGHT] =
				m_mapColorTranslate[_2003CLR_EXPBTN_PRESSED_DARK] =
				InstallColor(
					stat_HLS_Adjust(
						GetColor(XPCLR_HILIGHT),
						0.0, 0.60, 0.0
						)
					);
			m_mapColorTranslate[_2003CLR_EXPGLYPH_LIGHT] =
				COLOR_WINDOW;
			m_mapColorTranslate[_2003CLR_EXPGLYPH_DARK] =
				COLOR_3DDKSHADOW;
			m_mapColorTranslate[_2003CLR_STATUSBAR_ITEM] =
				COLOR_3DSHADOW;
//				InstallColor(
//					stat_HLS_Adjust(
//						GetColor(XPCLR_3DFACE_NORMAL),
//						0.0, -0.05, 0.0
//						)
//					);
			m_mapColorTranslate[_2003CLR_MLA_NORM_LEFT] =
				COLOR_WINDOW;
			m_mapColorTranslate[_2003CLR_MLA_NORM_RIGHT] =
				InstallColor(
					GetColor( XPCLR_3DFACE_DARK )
					);
			m_mapColorTranslate[_2003CLR_MLA_RARELY_LEFT] =
				InstallColor(
					GetColor( XPCLR_3DFACE_NORMAL )
					);
			m_mapColorTranslate[_2003CLR_MLA_RARELY_RIGHT] =
				COLOR_3DFACE;

			m_mapColorTranslate[_2003CLR_MENUEXPBALOON] =
				COLOR_WINDOW;

			// reinit XP colors
			m_mapColorTranslate[XPCLR_3DFACE_NORMAL] = 
				InstallColor(
					stat_HLS_Adjust( GetColor(XPCLR_3DFACE_NORMAL), 0.0, 0.30, 0.0 )
					);
		
			// init non-linear gradient bitmaps
			COLORREF clrLeft = GetColor( _2003CLR_GRADIENT_LIGHT );
			COLORREF clrRight = GetColor( _2003CLR_GRADIENT_DARK );
			COLORREF clrQuadLeft =
				RGB(
					GetBValue( clrLeft ),
					GetGValue( clrLeft ),
					GetRValue( clrLeft )
					);
			COLORREF clrQuadRight =
				RGB(
					GetBValue( clrRight ),
					GetGValue( clrRight ),
					GetRValue( clrRight )
					);
			double lfColorDist[3] =
			{
				double( GetRValue( clrQuadRight ) ) - double( GetRValue( clrQuadLeft ) ),
				double( GetGValue( clrQuadRight ) ) - double( GetGValue( clrQuadLeft ) ),
				double( GetBValue( clrQuadRight ) ) - double( GetBValue( clrQuadLeft ) ),
			};
			double lfTmp =
				::sqrt(
					double(g_nBarGradientSizeMetric)
					* double(g_nBarGradientSizeMetric)
					* 2.0
					);
			double lfMaxDist = lfTmp + ::sqrt( lfTmp );

			// horizontal bar gradient
			BITMAPINFOHEADER bih;
			::memset( &bih, 0, sizeof(BITMAPINFOHEADER) );
			bih.biSize = sizeof(BITMAPINFOHEADER);
			bih.biWidth = bih.biHeight = g_nBarGradientSizeMetric;
			bih.biPlanes = 1;
			bih.biBitCount = 32;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = g_nBarGradientPixelCount;
			COLORREF * pSurfaceCB = NULL;
			HBITMAP hDib =
				::CreateDIBSection(
					dcDesktop,
					(LPBITMAPINFO)&bih,
					DIB_RGB_COLORS,
					(void **)&pSurfaceCB,
					NULL,
					NULL
					);
			ASSERT( hDib != NULL && pSurfaceCB != NULL );
			if( hDib != NULL && pSurfaceCB != NULL )
			{
				m_bmpCtBarGradientHorz.Attach( hDib );
				for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
				{
					for( LONG y = 0; y < g_nBarGradientSizeMetric; y++ )
					{
						COLORREF * pClr =
							pSurfaceCB + g_nBarGradientPixelCount
							- (y * g_nBarGradientSizeMetric)
							- g_nBarGradientSizeMetric + x
							;
						lfTmp =
							::sqrt(
								double(x) * double(x)
								+ double(y) * double(y)
								);
						double lfPixelDist =
							lfTmp + ::sqrt( lfTmp );
						ASSERT( lfPixelDist <= lfMaxDist );
						double lfPercent =
							::sqrt( 1.0 - lfPixelDist / lfMaxDist );
						double lfColorOffset[3] =
						{
							lfColorDist[0]*lfPercent,
							lfColorDist[1]*lfPercent,
							lfColorDist[2]*lfPercent,
						};
						double lfQ =
							( double(g_nBarGradientSizeMetric) - double(y)*2.0 )
							/ double(g_nBarGradientSizeMetric)
							;
						ASSERT( (-1.0) <= lfQ && lfQ <= (1.0) );
						lfQ *= lfQ*lfQ;
						for( int i = 0; i < 3; i++ )
						{
							double & lfCV = lfColorOffset[i];
							lfCV /= lfColorDist[i];
							if( lfQ > 0.0 )
								lfCV = lfCV + (1.0 - lfCV) * lfQ;
							else if ( lfQ < 0.0 )
								lfCV = lfCV * (1.0 + lfQ);
							if( lfCV < 0.0 )
								lfCV = 0.0;
							else if( lfCV > 1.0 )
								lfCV = 1.0;
							lfCV *= lfColorDist[i];
						} // for( int i = 0; i < 3; i++ )
						*pClr =
							RGB(
								DWORD( double(GetRValue(clrQuadRight)) - lfColorOffset[0] ),
								DWORD( double(GetGValue(clrQuadRight)) - lfColorOffset[1] ),
								DWORD( double(GetBValue(clrQuadRight)) - lfColorOffset[2] )
								);
					}
				} // for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
			} // if( hDib != NULL && pSurfaceCB != NULL )

			// vertical bar gradient
			::memset( &bih, 0, sizeof(BITMAPINFOHEADER) );
			bih.biSize = sizeof(BITMAPINFOHEADER);
			bih.biWidth = bih.biHeight = g_nBarGradientSizeMetric;
			bih.biPlanes = 1;
			bih.biBitCount = 32;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = g_nBarGradientPixelCount;
			pSurfaceCB = NULL;
			hDib =
				::CreateDIBSection(
					dcDesktop,
					(LPBITMAPINFO)&bih,
					DIB_RGB_COLORS,
					(void **)&pSurfaceCB,
					NULL,
					NULL
					);
			ASSERT( hDib != NULL && pSurfaceCB != NULL );
			if( hDib != NULL && pSurfaceCB != NULL )
			{
				m_bmpCtBarGradientVert.Attach( hDib );
				for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
				{
					for( LONG y = 0; y < g_nBarGradientSizeMetric; y++ )
					{
						COLORREF * pClr =
							pSurfaceCB + g_nBarGradientPixelCount
							- (y * g_nBarGradientSizeMetric)
							- g_nBarGradientSizeMetric + x
							;
						lfTmp =
							::sqrt(
								double(x) * double(x)
								+ double(y) * double(y)
								);
						double lfPixelDist =
							lfTmp + ::sqrt( lfTmp );
						ASSERT( lfPixelDist <= lfMaxDist );
						double lfPercent =
							::sqrt( 1.0 - lfPixelDist / lfMaxDist );
						double lfColorOffset[3] =
						{
							lfColorDist[0]*lfPercent,
							lfColorDist[1]*lfPercent,
							lfColorDist[2]*lfPercent,
						};
						double lfQ =
							( double(g_nBarGradientSizeMetric) - double(x)*2.0 )
							/ double(g_nBarGradientSizeMetric)
							;
						ASSERT( (-1.0) <= lfQ && lfQ <= (1.0) );
						lfQ *= lfQ*lfQ;
						for( int i = 0; i < 3; i++ )
						{
							double & lfCV = lfColorOffset[i];
							lfCV /= lfColorDist[i];
							if( lfQ > 0.0 )
								lfCV = lfCV + (1.0 - lfCV) * lfQ;
							else if ( lfQ < 0.0 )
								lfCV = lfCV * (1.0 + lfQ);
							if( lfCV < 0.0 )
								lfCV = 0.0;
							else if( lfCV > 1.0 )
								lfCV = 1.0;
							lfCV *= lfColorDist[i];
						} // for( int i = 0; i < 3; i++ )
						*pClr =
							RGB(
								DWORD( double(GetRValue(clrQuadRight)) - lfColorOffset[0] ),
								DWORD( double(GetGValue(clrQuadRight)) - lfColorOffset[1] ),
								DWORD( double(GetBValue(clrQuadRight)) - lfColorOffset[2] )
								);
					}
				} // for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
			} // if( hDib != NULL && pSurfaceCB != NULL )
		} // if use system colors

		// docker gradient
		COLORREF clrLeft = GetColor( _2003CLR_GRADIENT_LIGHT );
		COLORREF clrRight = GetColor( _2003CLR_GRADIENT_DARK );
		COLORREF clrQuadLeft =
			RGB(
				GetBValue( clrLeft ),
				GetGValue( clrLeft ),
				GetRValue( clrLeft )
				);
		COLORREF clrQuadRight =
			RGB(
				GetBValue( clrRight ),
				GetGValue( clrRight ),
				GetRValue( clrRight )
				);
		double lfColorDist[3] =
		{
			double( GetRValue( clrQuadRight ) ) - double( GetRValue( clrQuadLeft ) ),
			double( GetGValue( clrQuadRight ) ) - double( GetGValue( clrQuadLeft ) ),
			double( GetBValue( clrQuadRight ) ) - double( GetBValue( clrQuadLeft ) ),
		};

		BITMAPINFOHEADER bih;
		::memset( &bih, 0, sizeof(BITMAPINFOHEADER) );
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biWidth = bih.biHeight = g_nBarGradientSizeMetric;
		bih.biPlanes = 1;
		bih.biBitCount = 32;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = g_nBarGradientPixelCount;
		COLORREF * pSurfaceDB = NULL;
		HBITMAP hDib =
			::CreateDIBSection(
				dcDesktop,
				(LPBITMAPINFO)&bih,
				DIB_RGB_COLORS,
				(void **)&pSurfaceDB,
				NULL,
				NULL
				);
		ASSERT( hDib != NULL && pSurfaceDB != NULL );
		if( hDib != NULL && pSurfaceDB != NULL )
		{
			m_bmpDockerGradient.Attach( hDib );
			double lfMaxDist =
				::sqrt(
					double(g_nBarGradientSizeMetric) * double(g_nBarGradientSizeMetric)
						* 1.4
					+
					( double(g_nBarGradientSizeMetric) * double(g_nBarGradientSizeMetric) )
						/ ( 4.0 * 4.0 )
					);
			for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
			{
				for( LONG y = 0; y < g_nBarGradientSizeMetric; y++ )
				{
					COLORREF * pClr =
						pSurfaceDB + g_nBarGradientPixelCount
						- (y * g_nBarGradientSizeMetric)
						- g_nBarGradientSizeMetric + x
						;
					double lfPixelDist =
						::sqrt(
							double(x) * double(x)
							+ ( double(y) * double(y) ) / ( 8.0 * 8.0 )
							);
					ASSERT( lfPixelDist <= lfMaxDist );
					double lfPercent = 1.0 - lfPixelDist / lfMaxDist;
					double lfColorOffset[3] =
					{
						lfColorDist[0]*lfPercent,
						lfColorDist[1]*lfPercent,
						lfColorDist[2]*lfPercent,
					};
					*pClr =
						RGB(
							DWORD( double(GetRValue(clrQuadLeft)) + lfColorOffset[0] ),
							DWORD( double(GetGValue(clrQuadLeft)) + lfColorOffset[1] ),
							DWORD( double(GetBValue(clrQuadLeft)) + lfColorOffset[2] )
							);
				}
			} // for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
		} // if( hDib != NULL && pSurfaceCB != NULL )
	} // if( stat_GetBPP() > 8 )
	else
	{
		m_mapColorTranslate[_2003CLR_STATUSBAR_ITEM] =
			InstallColor( GetColor(COLOR_3DSHADOW) );
	} // else from if( stat_GetBPP() > 8 )
}

void CExtPaintManagerOffice2003::PaintControlBarClientArea(
	CDC & dc,
	const RECT & rcClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
	if(	::IsRectEmpty( &rcClient )
		|| ( ! dc.RectVisible( &rcClient ) )
		)
		return;
CExtToolControlBar * pToolBar = NULL;
CWnd * pFrame = NULL;
	if( pHelperSrc != NULL )
	{
		pToolBar =
			DYNAMIC_DOWNCAST(
				CExtToolControlBar,
				pHelperSrc
				);
		if( pToolBar != NULL )
		{
			if(	!(	pToolBar->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) )
					||	pToolBar->IsKindOf( RUNTIME_CLASS(CExtPanelControlBar) )
					)
				)
			{ // not menu bar, not panel bar
				if( pToolBar->m_pDockSite != NULL )
				{
					if( pToolBar->IsFloating() )
						pToolBar = NULL;
				} // if( pToolBar->m_pDockSite != NULL )
				else
				{
					CWnd * pWndOwner = pToolBar->GetOwner();
					if( pWndOwner != NULL )
					{
						pFrame =
							DYNAMIC_DOWNCAST( CFrameWnd, pWndOwner );
						if( pFrame == NULL )
						{
							if( (pWndOwner->GetStyle() & WS_CHILD) != 0 )
								pFrame = pWndOwner->GetParentFrame();
						} // if( pFrame == NULL )
						if( pFrame != NULL )
						{
							if( pFrame->IsKindOf( RUNTIME_CLASS(CExtMiniDockFrameWnd) ) )
							{ // if ext mini dock frame
							} // if ext mini dock frame
							else
							{
								CControlBar * pDocker =
									((CFrameWnd *)pFrame)->
										GetControlBar( AFX_IDW_DOCKBAR_TOP );
								if( pDocker == NULL )
								{
									pFrame = NULL;
									pToolBar = NULL;
								}
								else if( !pDocker->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar)) )
								{
									pFrame = NULL;
									pToolBar = NULL;
								}
							}
						} // if( pFrame != NULL )
					} // if( pWndOwner != NULL )
				} // else from if( pToolBar->m_pDockSite != NULL )
			} // not menu bar, not panel bar
		} // if( pToolBar != NULL )
	} // if( pHelperSrc != NULL )
	if(		pToolBar == NULL
		||	m_bmpCtBarGradientHorz.GetSafeHandle() == NULL
		||	m_bmpCtBarGradientVert.GetSafeHandle() == NULL
		||	m_bmpDockerGradient.GetSafeHandle() == NULL
		||	stat_GetBPP() <= 8
		)
	{
		CExtPaintManagerXP::PaintControlBarClientArea(
			dc,
			rcClient,
			pHelperSrc,
			lParam
			);
		return;
	}
CRect rcPaintGradient;
	if( pFrame == NULL )
		pFrame = (pToolBar->m_pDockSite == NULL)
			? pToolBar->GetParent()
			: pToolBar->GetParentFrame()
			;
	ASSERT_VALID( pFrame );
	pFrame->GetClientRect( &rcPaintGradient );
	pFrame->ClientToScreen( &rcPaintGradient );
	pToolBar->ScreenToClient( &rcPaintGradient );
	if(	::IsRectEmpty( &rcPaintGradient )
		|| ( ! dc.RectVisible( &rcPaintGradient ) )
		)
		return;
CDC & dcCompat = CachedDcGet( m_bmpDockerGradient );
	if( dcCompat.GetSafeHdc() == NULL )
	{
		ASSERT( FALSE );
		return;
	}
int nOldStretchBltMode =
		dc.SetStretchBltMode(
			g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
			);
	::SetBrushOrgEx( dc, 0, 0, NULL );
	
	dc.StretchBlt(
		rcPaintGradient.left, rcPaintGradient.top,
		rcPaintGradient.Width(), rcPaintGradient.Height(),
		&dcCompat,
		0, 0, m_sizeDockerGradient.cx, m_sizeDockerGradient.cy,
		SRCCOPY
		);
	
	dc.SetStretchBltMode( nOldStretchBltMode );
	
bool bHorz = true;
	if( pToolBar->IsDocked() )
		bHorz = (! pToolBar->IsDockedVertically() );

	if(		pToolBar->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) )
		||	pToolBar->IsKindOf( RUNTIME_CLASS(CExtPanelControlBar) )
		||	pToolBar->m_pDockSite == NULL
		)
	{ // if dockbar-like background style
		return;
	} // if dockbar-like background style

	pToolBar->GetWindowRect( &rcPaintGradient );
	pToolBar->ScreenToClient( &rcPaintGradient );
CBitmap * pBmpOld =
		dcCompat.SelectObject(
			bHorz ? ( &m_bmpCtBarGradientHorz ) : ( &m_bmpCtBarGradientVert )
			);
CSize sizeShade = bHorz
		? m_sizeCtBarGradientHorz
		: m_sizeCtBarGradientVert
		;
	nOldStretchBltMode =
		dc.SetStretchBltMode(
			g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
			);
	::SetBrushOrgEx( dc, 0, 0, NULL );

int nIdxClrTbFillMargin = -1;
CRgn _rgnBaloonRight, _rgnBaloonLeft, _rgnBaloonSet;
	if(		m_sizeToolBarRgnRounders.cx > 0
		&&	m_sizeToolBarRgnRounders.cy > 0
		&&	pToolBar->IsDocked()
		)
	{ // if baloon style
		nIdxClrTbFillMargin = m_nIdxClrTbFillMargin;
		if(		_rgnBaloonRight.CreateRoundRectRgn(
					0,
					0,
					rcClient.right - rcClient.left,
					rcClient.bottom - rcClient.top,
					m_sizeToolBarRgnRounders.cx,
					m_sizeToolBarRgnRounders.cy
					)
			&&	_rgnBaloonLeft.CreateRectRgn(
					0,
					0,
					bHorz
						? (m_sizeToolBarRgnRounders.cx + 2)
						: (rcClient.right - rcClient.left - 1),
					bHorz
						? (rcClient.bottom - rcClient.top - 1)
						: (m_sizeToolBarRgnRounders.cy + 2)
					)
			&&	_rgnBaloonSet.CreateRectRgn( 0, 0, 1, 1 )
			&&	_rgnBaloonSet.CombineRgn(
					&_rgnBaloonLeft,
					&_rgnBaloonRight,
					RGN_OR
					) != ERROR
			)
		{
			dc.SelectClipRgn( &_rgnBaloonSet );
		}
#ifdef _DEBUG
		else
		{
			ASSERT( FALSE );
		}
#endif // _DEBUG
	}  // if baloon style

	dc.StretchBlt(
		rcPaintGradient.left, rcPaintGradient.top,
		rcPaintGradient.Width(), rcPaintGradient.Height(),
		&dcCompat,
		0, 0, sizeShade.cx, sizeShade.cy,
		SRCCOPY
		);
	if( nIdxClrTbFillMargin >= 0 )
	{
		CPen _pen( PS_SOLID, 1, GetColor(m_nIdxClrTbFillMargin) );
		CPen  * pOldPen = dc.SelectObject( &_pen );
		CRect rcClient;
		pToolBar->GetClientRect( &rcClient );
		if( bHorz )
		{
			dc.MoveTo(
				rcClient.left,
				rcClient.bottom-2
				);
			dc.LineTo(
				rcClient.right,
				rcClient.bottom-2
				);
		} // if( bHorz )
		else
		{
			dc.MoveTo(
				rcClient.right-2,
				rcClient.top
				);
			dc.LineTo(
				rcClient.right-2,
				rcClient.bottom
				);
		} // else from if( bHorz )
		dc.SelectObject( pOldPen );
	} // if( nIdxClrTbFillMargin >= 0 )
	if( _rgnBaloonSet.GetSafeHandle() != NULL )
		dc.SelectClipRgn( NULL );
	
	dc.SetStretchBltMode( nOldStretchBltMode );
	dcCompat.SelectObject( pBmpOld );
}

void CExtPaintManagerOffice2003::PaintDockBarClientArea(
	CDC & dc,
	const RECT & rcClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;
	if(	::IsRectEmpty( &rcClient )
		|| ( ! dc.RectVisible( &rcClient ) )
		)
		return;
CRect rcPaintGradient( 0, 0, 0, 0 );
CExtDockOuterBar * pOuterDocker = NULL;
	if( pHelperSrc != NULL )
	{
		pOuterDocker =
			DYNAMIC_DOWNCAST(
				CExtDockOuterBar,
				pHelperSrc
				);
		if( pOuterDocker != NULL )
		{
			CFrameWnd * pFrame =
				pOuterDocker->GetParentFrame();
			ASSERT_VALID( pFrame );
			pFrame->GetClientRect( &rcPaintGradient );
			pFrame->ClientToScreen( &rcPaintGradient );
			pOuterDocker->ScreenToClient( &rcPaintGradient );
			if(	::IsRectEmpty( &rcPaintGradient )
				|| ( ! dc.RectVisible( &rcPaintGradient ) )
				)
				return;
		}
	}
	if(		pOuterDocker == NULL
		||	m_bmpDockerGradient.GetSafeHandle() == NULL
		||	stat_GetBPP() <= 8
		)
	{
		CExtPaintManagerXP::PaintDockBarClientArea(
			dc,
			rcClient,
			pHelperSrc,
			lParam
			);
		return;
	}
CDC & dcCompat = CachedDcGet( m_bmpDockerGradient );
	if( dcCompat.GetSafeHdc() == NULL )
	{
		ASSERT( FALSE );
		return;
	}
int nOldStretchBltMode =
		dc.SetStretchBltMode(
			g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
			);
	::SetBrushOrgEx( dc, 0, 0, NULL );
	
	dc.StretchBlt(
		rcPaintGradient.left, rcPaintGradient.top,
		rcPaintGradient.Width(), rcPaintGradient.Height(),
		&dcCompat,
		0, 0, m_sizeDockerGradient.cx, m_sizeDockerGradient.cy,
		SRCCOPY
		);
	
	dc.SetStretchBltMode( nOldStretchBltMode );
}


CExtPaintManager::PAINTDOCKINGFRAMEDATA::PAINTDOCKINGFRAMEDATA()
	: m_pHelperSrc( NULL )
	, m_lParam( 0L )
	, m_rcWindow( 0,0,0,0 )
	, m_rcClient( 0,0,0,0 )
	, m_bFloating( false )
	, m_bExtBar( false )
	, m_bSideBar( false )
{
}

CExtPaintManager::PAINTDOCKINGFRAMEDATA::PAINTDOCKINGFRAMEDATA(
	CObject * pHelperSrc,
	const RECT & rcWindow,
	const RECT & rcClient,
	bool bFloating,
	bool bExtBar,
	bool bSideBar // = false
	)
	: m_pHelperSrc( pHelperSrc )
	, m_lParam( 0L )
	, m_rcWindow( rcWindow )
	, m_rcClient( rcClient )
	, m_bFloating( bFloating )
	, m_bExtBar( bExtBar )
	, m_bSideBar( bSideBar )
{
}

void CExtPaintManager::PaintDockingFrame(
	CDC & dc,
	CExtPaintManager::PAINTDOCKINGFRAMEDATA & _pdfd
	)
{
	if( _pdfd.m_rcWindow.IsRectEmpty() )
		return;
	dc.ExcludeClipRect( _pdfd.m_rcClient );
CRect rcWnd( _pdfd.m_rcWindow );
COLORREF clr3dFace = GetColor(CLR_3DFACE_OUT);
COLORREF clr3dHilight = GetColor(CLR_3DHILIGHT_OUT);
COLORREF clr3dShadow = GetColor(CLR_3DSHADOW_OUT);
COLORREF clr3dDkShadow = GetColor(CLR_3DDKSHADOW_OUT);
	dc.FillSolidRect(
		&rcWnd,
		clr3dFace
		);
	if( _pdfd.m_bFloating )
	{
		dc.Draw3dRect(
			&rcWnd, clr3dFace, clr3dDkShadow);
		rcWnd.DeflateRect(1,1);
		dc.Draw3dRect(
			&rcWnd, clr3dHilight, clr3dShadow);
	}
	else
	{
		if( !_pdfd.m_bExtBar )
		{
			dc.Draw3dRect(
				&rcWnd, clr3dHilight, clr3dShadow);
		}
	}
	dc.IntersectClipRect( _pdfd.m_rcWindow );
}

void CExtPaintManagerXP::PaintDockingFrame(
	CDC & dc,
	CExtPaintManager::PAINTDOCKINGFRAMEDATA & _pdfd
	)
{
	if( _pdfd.m_rcWindow.IsRectEmpty() )
		return;
	dc.ExcludeClipRect( _pdfd.m_rcClient );
CRect rcWnd( _pdfd.m_rcWindow );
COLORREF clr3dFace = GetColor(COLOR_3DFACE);
COLORREF clrDark = GetColor(COLOR_3DSHADOW);
COLORREF clrBkOld = dc.GetBkColor();
COLORREF clrBkNew =
		GetColor(
			( _pdfd.m_bFloating )
				? (	_pdfd.m_bExtBar
						? XPCLR_3DFACE_FLOAT_R
						: XPCLR_3DFACE_FLOAT_F
					)
				: CLR_3DFACE_OUT
			);
	dc.FillSolidRect( &rcWnd, clrBkNew );
	if( _pdfd.m_bFloating )
	{
		dc.Draw3dRect( &rcWnd, clrDark, clrDark);
		rcWnd.DeflateRect(1,1);
		dc.Draw3dRect( &rcWnd, clrDark, clrDark);
		rcWnd.DeflateRect(1,1,2,2);
		dc.SetPixel( rcWnd.left, rcWnd.top, clrDark );
		dc.SetPixel( rcWnd.left, rcWnd.bottom, clrDark );
		dc.SetPixel( rcWnd.right, rcWnd.top, clrDark );
		dc.SetPixel( rcWnd.right, rcWnd.bottom, clrDark );
	} // if( _pdfd.m_bFloating )
	else
	{
		if( !_pdfd.m_bExtBar )
			dc.Draw3dRect( &rcWnd, clr3dFace, clr3dFace);
		if( !_pdfd.m_bSideBar && stat_GetBPP() <= 8 )
		{
			static WORD hatchBits[8] =
			{
				0xAA,
				0x55,
				0xAA,
				0x55,
				0xAA,
				0x55,
				0xAA,
				0x55,
			};
			CBrush br;
			CBitmap bmp;
			bmp.CreateBitmap( 8, 8, 1, 1, hatchBits );
			br.CreatePatternBrush( &bmp );
			LOGBRUSH lbr;
			br.GetLogBrush( &lbr );
			HPEN hPenDesired =
				::ExtCreatePen(
					PS_GEOMETRIC,
					1,
					&lbr,
					0,
					NULL
					);
			if( hPenDesired == NULL )
			{
				COLORREF clrDesiredSolidPen =
					GetColor( COLOR_3DSHADOW );
				hPenDesired =
					::CreatePen(
						PS_SOLID,
						1,
						clrDesiredSolidPen
						);
				if( hPenDesired == NULL )
				{
					hPenDesired = (HPEN)
						::GetStockObject(BLACK_PEN);
					ASSERT( hPenDesired != NULL );
				} // if( hPenDesired == NULL )
			} // if( hPenDesired == NULL )
//			CPen pen( PS_GEOMETRIC, 1, &lbr );
			CPen pen;
			VERIFY( pen.Attach( hPenDesired ) );
			CPen * pOldPen = dc.SelectObject( &pen );
			if( m_bHelperXpStyle8BitBarRect )
			{
				dc.MoveTo( rcWnd.left, rcWnd.top );
				dc.LineTo( rcWnd.right-1, rcWnd.top );
				dc.LineTo( rcWnd.right-1, rcWnd.bottom-1 );
				dc.LineTo( rcWnd.left, rcWnd.bottom-1 );
				dc.LineTo( rcWnd.left, rcWnd.top );
			} // if( m_bHelperXpStyle8BitBarRect )
			else
			{
				static const SIZE g_sizeRO = { 2, 2 };
				dc.MoveTo( rcWnd.left+g_sizeRO.cx, rcWnd.top );
				dc.LineTo( rcWnd.right-1-g_sizeRO.cx, rcWnd.top );
				dc.LineTo( rcWnd.right-1, rcWnd.top+g_sizeRO.cy );
				dc.LineTo( rcWnd.right-1, rcWnd.bottom-1-g_sizeRO.cy );
				dc.LineTo( rcWnd.right-1-g_sizeRO.cx, rcWnd.bottom-1 );
				dc.LineTo( rcWnd.left+g_sizeRO.cx, rcWnd.bottom-1 );
				dc.LineTo( rcWnd.left, rcWnd.bottom-1-g_sizeRO.cy );
				dc.LineTo( rcWnd.left, rcWnd.top+g_sizeRO.cy );
				dc.LineTo( rcWnd.left+g_sizeRO.cx, rcWnd.top );
			} // else from if( m_bHelperXpStyle8BitBarRect )
			dc.SelectObject( pOldPen );
		} // if( !_pdfd.m_bSideBar && stat_GetBPP() < 8 )
	} // else from if( _pdfd.m_bFloating )
	dc.SetBkColor( clrBkOld );
	//dc.IntersectClipRect(rectWindow);
	dc.SelectClipRgn(NULL);
}

void CExtPaintManagerOffice2003::PaintDockingFrame(
	CDC & dc,
	CExtPaintManager::PAINTDOCKINGFRAMEDATA & _pdfd
	)
{
	if( _pdfd.m_rcWindow.IsRectEmpty() )
		return;
	if(		_pdfd.m_bFloating
		//||	(!_pdfd.m_bExtBar)
		//||	_pdfd.m_bSideBar
		||	_pdfd.m_pHelperSrc == NULL
		||	(! _pdfd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtControlBar) ) )
		||	m_bmpCtBarGradientHorz.GetSafeHandle() == NULL
		||	m_bmpCtBarGradientVert.GetSafeHandle() == NULL
		||	m_bmpDockerGradient.GetSafeHandle() == NULL
		||	stat_GetBPP() <= 8
		)
	{
		CExtPaintManagerXP::PaintDockingFrame(
			dc,
			_pdfd
			);
		return;
	}

CExtControlBar * pControlBar =
		STATIC_DOWNCAST(
			CExtControlBar,
			_pdfd.m_pHelperSrc
			);
	ASSERT_VALID( pControlBar );

CWnd * pFrame = NULL;
	if( pControlBar->m_pDockSite == NULL )
	{
		CExtToolControlBar * pToolBar =
			DYNAMIC_DOWNCAST(
				CExtToolControlBar,
				pControlBar
				);
		if( pToolBar != NULL )
		{
			if(	!(	pToolBar->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) )
					||	pToolBar->IsKindOf( RUNTIME_CLASS(CExtPanelControlBar) )
					)
				)
			{ // not menu bar, not panel bar
				CWnd * pWndOwner = pToolBar->GetOwner();
				if( pWndOwner != NULL )
				{
					pFrame =
						DYNAMIC_DOWNCAST( CFrameWnd, pWndOwner );
					if( pFrame == NULL )
					{
						if( (pWndOwner->GetStyle() & WS_CHILD) != 0 )
							pFrame = pWndOwner->GetParentFrame();
					} // if( pFrame == NULL )
					if( pFrame != NULL )
					{
						if( pFrame->IsKindOf( RUNTIME_CLASS(CExtMiniDockFrameWnd) ) )
						{ // if ext mini dock frame
						} // if ext mini dock frame
						else
						{
							CControlBar * pDocker =
								((CFrameWnd *)pFrame)->
									GetControlBar( AFX_IDW_DOCKBAR_TOP );
							if( pDocker == NULL )
								pFrame = NULL;
							else if( !pDocker->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar)) )
								pFrame = NULL;
						}
					} // if( pFrame != NULL )
				} // if( pWndOwner != NULL )
			} // not menu bar, not panel bar
		} // if( pToolBar != NULL )
	} // if( pControlBar->m_pDockSite == NULL )

	if( pFrame == NULL )
		pFrame = (pControlBar->m_pDockSite == NULL)
			? pControlBar->GetParent()
			: pControlBar->GetParentFrame()
			;
	ASSERT_VALID( pFrame );
CRect rcPaintGradient;
	pFrame->GetClientRect( &rcPaintGradient );
	pFrame->ClientToScreen( &rcPaintGradient );
CRect rcBar;
	pControlBar->GetWindowRect( &rcBar );
	rcPaintGradient.OffsetRect( -rcBar.TopLeft() );
	if(	::IsRectEmpty( &rcPaintGradient )
		|| ( ! dc.RectVisible( &rcPaintGradient ) )
		)
		return;
CDC & dcCompat = CachedDcGet( m_bmpDockerGradient );
	if( dcCompat.GetSafeHdc() == NULL )
	{
		ASSERT( FALSE );
		return;
	}
int nOldStretchBltMode =
		dc.SetStretchBltMode(
			g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
			);
	::SetBrushOrgEx( dc, 0, 0, NULL );

	dc.StretchBlt(
		rcPaintGradient.left, rcPaintGradient.top,
		rcPaintGradient.Width(), rcPaintGradient.Height(),
		&dcCompat,
		0, 0, m_sizeDockerGradient.cx, m_sizeDockerGradient.cy,
		SRCCOPY
		);

	dc.SetStretchBltMode( nOldStretchBltMode );
}

CExtPaintManager::PAINTGRIPPERDATA::PAINTGRIPPERDATA()
	: m_pHelperSrc( NULL )
	, m_lParam( 0L )
	, m_rcGripper( 0,0,0,0 )
	, m_rcText( 0,0,0,0 )
	, m_bActive( false )
	, m_bFloating( false )
	, m_bHorz( false )
	, m_bSideBar( false )
	, m_sCaption( _T("") )
	, m_bHelperNoRect( false )
	, m_bHelperNoFill( false )
{
}

CExtPaintManager::PAINTGRIPPERDATA::PAINTGRIPPERDATA(
	CObject * pHelperSrc,
	const RECT & rcGripper,
	const RECT & rcText,
	bool bActive,
	bool bFloating,
	bool bHorz,
	bool bSideBar, // = false
	__EXT_MFC_SAFE_LPCTSTR sCaption // = NULL
	)
	: m_pHelperSrc( pHelperSrc )
	, m_lParam( 0L )
	, m_rcGripper( rcGripper )
	, m_rcText( rcText )
	, m_bActive( bActive )
	, m_bFloating( bFloating )
	, m_bHorz( bHorz )
	, m_bSideBar( bSideBar )
	, m_sCaption( (sCaption == NULL) ? _T("") : sCaption )
	, m_bHelperNoRect( false )
	, m_bHelperNoFill( false )
{
}

void CExtPaintManager::PaintGripper(
	CDC & dc,
	CExtPaintManager::PAINTGRIPPERDATA & _pgd
	)
{ 
CRect rectGripper2( _pgd.m_rcGripper );
COLORREF clrBtnShadow =  GetColor(CLR_3DSHADOW_OUT);
COLORREF clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);
	if( _pgd.m_bSideBar || _pgd.m_bFloating )
	{
		if( !_pgd.m_bFloating )
			rectGripper2.DeflateRect( 1, 1 );

#if (defined COLOR_GRADIENTACTIVECAPTION && defined COLOR_GRADIENTACTIVECAPTION)
		ASSERT( COLOR_GRADIENTACTIVECAPTION == 27 );
		ASSERT( COLOR_GRADIENTINACTIVECAPTION == 28 );
#endif
		INT nTextColorIndex = COLOR_HIGHLIGHTTEXT;
		if( _pgd.m_bSideBar )
		{
			if( _pgd.m_bFloating || _pgd.m_bActive )
			{
				nTextColorIndex = COLOR_CAPTIONTEXT;
				if( stat_GetBPP() > 8 )
				{
					stat_PaintGradientRect(
						dc,
						rectGripper2,
						GetColor( COLOR_ACTIVECAPTION ),
						GetColor( 27 ),
						_pgd.m_bHorz
						);
				} // if( stat_GetBPP() > 8 )
				else
				{
					dc.FillSolidRect(
						rectGripper2,
						GetColor( COLOR_ACTIVECAPTION )
						);
				} // else from if( stat_GetBPP() > 8 )
			}
			else
			{
				nTextColorIndex = COLOR_INACTIVECAPTIONTEXT;
				if( stat_GetBPP() > 8 )
				{
					stat_PaintGradientRect(
						dc,
						rectGripper2,
						GetColor( COLOR_INACTIVECAPTION ),
						GetColor( 28 ),
						_pgd.m_bHorz
						);
				} // if( stat_GetBPP() > 8 )
				else
				{
					dc.FillSolidRect(
						rectGripper2,
						GetColor( COLOR_INACTIVECAPTION )
						);
				} // else from if( stat_GetBPP() > 8 )
			}
		} // if( _pgd.m_bSideBar )
		else
		{
			dc.FillSolidRect(
				rectGripper2,
				GetColor( COLOR_HIGHLIGHT )
				);
		} // else from if( _pgd.m_bSideBar )

		int nTextLen = 0;
		if(		_pgd.m_sCaption != NULL
			&&	( nTextLen = _tcslen(_pgd.m_sCaption) ) > 0
			&&	!_pgd.m_rcText.IsRectEmpty()
			&&	_pgd.m_rcText.right > _pgd.m_rcText.left
			&&	_pgd.m_rcText.bottom > _pgd.m_rcText.top
			)
		{
			COLORREF clrText =
				GetColor(
					nTextColorIndex /* COLOR_HIGHLIGHTTEXT */
					);
			COLORREF clrOldText =
				dc.SetTextColor(clrText);
			int nOldBkMode = dc.SetBkMode(TRANSPARENT);
			CFont * pCurrFont =
				(!_pgd.m_bHorz) ?
					&m_FontBoldBC : &m_FontBoldVertXBC
					;
			CFont * pOldFont =
				dc.SelectObject( pCurrFont );
			ASSERT( pOldFont != NULL );
			if( !_pgd.m_bHorz )
			{ // if text is horizontal
				dc.DrawText(
					_pgd.m_sCaption,
					nTextLen,
					(LPRECT)&_pgd.m_rcText,
					DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS
					);
			} // if text is horizontal
			else
			{ // if text is vertical
				LOGFONT lf;
				::memset(&lf,0,sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);
				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin =
					(_pgd.m_rcText.Width() - _cyHorzFont)  / 2
					;

				CRect rcString = 
					CRect(
						CPoint(
							_pgd.m_rcText.left + _cyTextMargin - __ExtMfc_CXTEXTMARGIN,
							_pgd.m_rcText.bottom - __ExtMfc_CYTEXTMARGIN
							),
						_pgd.m_rcText.Size() //m_sizeHorz
						);
				rcString.DeflateRect(2,2);

				CRect rcExclude;
				rcExclude.SetRect(
					_pgd.m_rcGripper.left,
					_pgd.m_rcGripper.top,
					_pgd.m_rcGripper.right,
					_pgd.m_rcText.top
					);
				dc.ExcludeClipRect( &rcExclude );

				dc.DrawText(
					_pgd.m_sCaption,
					nTextLen,
					rcString,
					DT_SINGLELINE //|DT_CENTER|DT_VCENTER
						|DT_NOCLIP |DT_NOPREFIX
					); // don't forget DT_NOCLIP

				//dc.IntersectClipRect( &rectGripper );
				dc.SelectClipRgn(NULL);
			} // if text is vertical
			dc.SelectObject( pOldFont );
			dc.SetBkMode(nOldBkMode);
			dc.SetTextColor(clrOldText);
		}
	} // if( _pgd.m_bSideBar || _pgd.m_bFloating )
	else
	{
		if( _pgd.m_bHorz )
		{
			rectGripper2.DeflateRect( 4, 1, 4, 0 );
			rectGripper2.right = rectGripper2.left + 3;
			dc.Draw3dRect(
				rectGripper2,
				clrBtnHilight,
				clrBtnShadow
			);
			rectGripper2.OffsetRect( 3, 0 );
			dc.Draw3dRect(
				rectGripper2,
				clrBtnHilight,
				clrBtnShadow
			);
		} // if( bHorz )
		else
		{
			rectGripper2.OffsetRect( 1, 0 );
			rectGripper2.DeflateRect( 0, 4, 1, 4 );
			rectGripper2.bottom = rectGripper2.top + 3;
			dc.Draw3dRect(
				rectGripper2,
				clrBtnHilight,
				clrBtnShadow
			);
			rectGripper2.OffsetRect( 0, 3 );
			dc.Draw3dRect(
				rectGripper2,
				clrBtnHilight,
				clrBtnShadow
			);
		} // else from if( bHorz )
	} // else from if( _pgd.m_bSideBar || _pgd.m_bFloating )
}

void CExtPaintManagerXP::PaintGripper(
	CDC & dc,
	CExtPaintManager::PAINTGRIPPERDATA & _pgd
	)
{ 
CRect rectGripper2( _pgd.m_rcGripper );
	if( _pgd.m_bSideBar || _pgd.m_bFloating )
	{
		COLORREF clrGrip =
			GetColor(
				//COLOR_HIGHLIGHT
				_pgd.m_bSideBar ?
					(_pgd.m_bActive ? COLOR_HIGHLIGHT : XPCLR_3DFACE_DARK )
					:
					COLOR_3DSHADOW
				);
		if( !_pgd.m_bHelperNoFill )
			dc.FillSolidRect( rectGripper2, clrGrip );
		if(		_pgd.m_bSideBar
			&&	(!_pgd.m_bHelperNoFill)
			&&	(!_pgd.m_bFloating)
			&&	(!_pgd.m_bActive)
			)
		{ // rect border of docked bar caption like VS-.NET-7.0
			COLORREF clrSideRect = GetColor( COLOR_3DSHADOW );
			stat_PaintDotNet3dRect(
				dc,
				rectGripper2,
				clrSideRect,
				clrSideRect
				);
		} // rect border of docked bar caption like VS-.NET-7.0
		int nTextLen = 0;
		if(		_pgd.m_sCaption != NULL
			&&	( nTextLen = _tcslen( _pgd.m_sCaption ) ) > 0
			&&	!_pgd.m_rcText.IsRectEmpty()
			&&	_pgd.m_rcText.right > _pgd.m_rcText.left
			&&	_pgd.m_rcText.bottom > _pgd.m_rcText.top
			)
		{
			COLORREF clrText =
				GetColor(
					(_pgd.m_bSideBar) ?
						(_pgd.m_bActive ? COLOR_HIGHLIGHTTEXT : COLOR_BTNTEXT)
						:
						COLOR_HIGHLIGHTTEXT
					);
			COLORREF clrOldText =
				dc.SetTextColor(clrText);
			int nOldBkMode = dc.SetBkMode(TRANSPARENT);
			CFont * pCurrFont =
				(!_pgd.m_bHorz)
					? ( _pgd.m_bSideBar
							? &m_FontNormalBC
							: &m_FontBoldBC
						)
					: ( _pgd.m_bSideBar
							? &m_FontNormalVertXBC
							: &m_FontBoldVertXBC
						)
					;
			CFont * pOldFont =
				dc.SelectObject( pCurrFont );
			ASSERT( pOldFont != NULL );
			if( !_pgd.m_bHorz )
			{ // if text is horizontal
				dc.DrawText(
					_pgd.m_sCaption,
					nTextLen,
					(LPRECT)&_pgd.m_rcText,
					DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS
					);
			} // if text is horizontal
			else
			{ // if text is vertical
				LOGFONT lf;
				::memset(&lf,0,sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);
				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin =
					(_pgd.m_rcText.Width() - _cyHorzFont)  / 2
					;

				CRect rcString = 
					CRect(
						CPoint(
							_pgd.m_rcText.left + _cyTextMargin - __ExtMfc_CXTEXTMARGIN,
							_pgd.m_rcText.bottom - __ExtMfc_CYTEXTMARGIN
							),
						_pgd.m_rcText.Size() //m_sizeHorz
						);
				rcString.DeflateRect(2,2);

				CRect rcExclude;
				rcExclude.SetRect(
					_pgd.m_rcGripper.left,
					_pgd.m_rcGripper.top,
					_pgd.m_rcGripper.right,
					_pgd.m_rcText.top
					);
				dc.ExcludeClipRect( &rcExclude );

				dc.DrawText(
					_pgd.m_sCaption,
					nTextLen,
					rcString,
					DT_SINGLELINE //|DT_CENTER|DT_VCENTER
						|DT_NOCLIP |DT_NOPREFIX
					); // don't forget DT_NOCLIP

				//dc.IntersectClipRect( &rectGripper );
				dc.SelectClipRgn(NULL);

			} // if text is vertical
			dc.SelectObject( pOldFont );
			dc.SetBkMode(nOldBkMode);
			dc.SetTextColor(clrOldText);
		}
	} // if( _pgd.m_bSideBar || _pgd.m_bFloating )
	else
	{
		COLORREF clrGrip =
			GetColor(CLR_3DFACE_OUT);
		CRect rcGripToClear( _pgd.m_rcGripper );
		rcGripToClear.DeflateRect( 1, 1 );
		dc.FillSolidRect(
			rcGripToClear,
			clrGrip
			);
		int nBitsPerPixel = stat_GetBPP();
		static WORD horzHatchBits[8] =
		{
			0xFF, 0x00, 0xFF, 0x00,
			0xFF, 0x00, 0xFF, 0x00
		};
		static WORD vertHatchBits[8] =
		{
			0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA
		};
		if( _pgd.m_bHorz )
		{
			CBrush brGripperHorz;
			CBitmap bmpGripperHorz;
			if( nBitsPerPixel <= 8 )
				bmpGripperHorz.CreateBitmap( 8, 8, 1, 1, horzHatchBits );
			else
			{
				CDC mdc;
				VERIFY( mdc.CreateCompatibleDC( &dc ) );
				VERIFY( bmpGripperHorz.CreateCompatibleBitmap( &dc, 8, 8 ) );
				CBitmap * pOldBmp = mdc.SelectObject( &bmpGripperHorz );
				mdc.FillSolidRect( 0, 0, 8, 8, clrGrip );
				COLORREF clr1 = GetColor( COLOR_3DFACE );
				COLORREF clr2 = GetColor( COLOR_3DSHADOW );
				COLORREF clrGripLines =
					RGB(
						(GetRValue(clr1)*2+GetRValue(clr2))/3,
						(GetGValue(clr1)*2+GetGValue(clr2))/3,
						(GetBValue(clr1)*2+GetBValue(clr2))/3
						);
				CPen pen(
					PS_SOLID,
					1,
					mdc.GetNearestColor(clrGripLines)
					);
				CPen * pOldPen = mdc.SelectObject( &pen );
				for( INT nPos = 0; nPos < 8; nPos += 2 )
				{
					mdc.MoveTo( 0, nPos );
					mdc.LineTo( 8, nPos );
				}
				mdc.SelectObject( pOldPen );
				mdc.SelectObject( pOldBmp );
			} // else from if( nBitsPerPixel < 8 )
			brGripperHorz.CreatePatternBrush( &bmpGripperHorz );

			rectGripper2.DeflateRect( 5, 2 );
			rectGripper2.right = rectGripper2.left + 5;
			rectGripper2.DeflateRect( 1, 0 );
			COLORREF clrOldBk = dc.SetBkColor( GetColor(XPCLR_3DFACE_NORMAL) );
			CBrush * pBrushOld = dc.SelectObject( &brGripperHorz );
			dc.PatBlt(
				rectGripper2.left, rectGripper2.top,
				rectGripper2.Width(), rectGripper2.Height(),
				0xA000C9
				);
			dc.SelectObject( pBrushOld );
			dc.SetBkColor( clrOldBk );
		} // if( _pgd.m_bHorz )
		else
		{
			CBrush brGripperVert;
			CBitmap bmpGripperVert;
			if( nBitsPerPixel <= 8 )
				bmpGripperVert.CreateBitmap( 8, 8, 1, 1, vertHatchBits );
			else
			{
				CDC mdc;
				VERIFY( mdc.CreateCompatibleDC( &dc ) );
				VERIFY( bmpGripperVert.CreateCompatibleBitmap( &dc, 8, 8 ) );
				CBitmap * pOldBmp = mdc.SelectObject( &bmpGripperVert );
				mdc.FillSolidRect( 0, 0, 8, 8, clrGrip );
				COLORREF clr1 = GetColor( COLOR_3DFACE );
				COLORREF clr2 = GetColor( COLOR_3DSHADOW );
				COLORREF clrGripLines =
					RGB(
						(GetRValue(clr1)*2+GetRValue(clr2))/3,
						(GetGValue(clr1)*2+GetGValue(clr2))/3,
						(GetBValue(clr1)*2+GetBValue(clr2))/3
						);
				CPen pen(
					PS_SOLID,
					1,
					mdc.GetNearestColor(clrGripLines)
					);
				CPen * pOldPen = mdc.SelectObject( &pen );
				for( INT nPos = 0; nPos < 8; nPos += 2 )
				{
					mdc.MoveTo( nPos, 0 );
					mdc.LineTo( nPos, 8 );
				}
				mdc.SelectObject( pOldPen );
				mdc.SelectObject( pOldBmp );
			} // else from if( nBitsPerPixel < 8 )
			brGripperVert.CreatePatternBrush( &bmpGripperVert );

			rectGripper2.OffsetRect( 1, 0 );
			rectGripper2.DeflateRect( 2, 5 );
			rectGripper2.bottom = rectGripper2.top + 5;
			rectGripper2.DeflateRect( 0, 1 );
			COLORREF clrOldBk = dc.SetBkColor( GetColor(XPCLR_3DFACE_NORMAL) );
			CBrush * pBrushOld = dc.SelectObject( &brGripperVert );
			dc.PatBlt(
				rectGripper2.left, rectGripper2.top,
				rectGripper2.Width(), rectGripper2.Height(),
				0xA000C9
				);
			dc.SelectObject( pBrushOld );
			dc.SetBkColor( clrOldBk );
		} // else from if( _pgd.m_bHorz )
	} // else from if( _pgd.m_bSideBar || _pgd.m_bFloating )
}

void CExtPaintManagerOffice2003::PaintGripper(
	CDC & dc,
	CExtPaintManager::PAINTGRIPPERDATA & _pgd
	)
{ 
bool bStyleOffice2003 = true;
	if(		_pgd.m_bFloating
		//||	_pgd.m_bSideBar
		||	_pgd.m_pHelperSrc == NULL
		||	(!(
				_pgd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtControlBar))
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
				|| _pgd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtDynAutoHideSlider))
#endif
			))
		||	m_bmpCtBarGradientHorz.GetSafeHandle() == NULL
		||	m_bmpCtBarGradientVert.GetSafeHandle() == NULL
		||	m_bmpDockerGradient.GetSafeHandle() == NULL
		||	stat_GetBPP() <= 8
		)
		bStyleOffice2003 = false;
CWnd * pBar = NULL;
bool bForceNoBaloon = false;
bool bInactiveResizableBarCapt = false;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if(		bStyleOffice2003
		&&	_pgd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtDynAutoHideSlider))
		)
	{ // auto-hide slider
		pBar = STATIC_DOWNCAST( CWnd, _pgd.m_pHelperSrc );
		bInactiveResizableBarCapt = true;
	} // auto-hide slider
	else 
#endif
		if( bStyleOffice2003 )
	{ // non-dockers
		pBar =
			STATIC_DOWNCAST(
				CWnd,
				_pgd.m_pHelperSrc
				);
		ASSERT_VALID( pBar );
		if( _pgd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtPanelControlBar) ) )
		{ // if panel bar
			bForceNoBaloon = true;
		} // if panel bar
		if( _pgd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtControlBar) ) )
		{ // if control bar
			if(	((CExtControlBar*)pBar)->m_pDockSite != NULL )
			{ // if docked
				if( ((CExtControlBar*)pBar)->IsFloating() )
					bStyleOffice2003 = false;
			} // if docked
			else
			{ // if simple
				bForceNoBaloon = true;
			} // if simple
		} // if control bar
		else
		{ // if NOT control bar
			bStyleOffice2003 = false;
		} // if NOT control bar
	} // non-dockers
	if( !bStyleOffice2003 )
	{
		CExtPaintManagerXP::PaintGripper( dc, _pgd );
		return;
	} // if( !bStyleOffice2003 )
	ASSERT_VALID( pBar );

CRect rcBarWnd, rcBarClient;
	pBar->GetWindowRect( &rcBarWnd );
	pBar->GetClientRect( &rcBarClient );
	pBar->ClientToScreen( &rcBarClient );

	if( _pgd.m_bSideBar || bInactiveResizableBarCapt )
	{ // if resizable bar caption
		_pgd.m_bHelperNoFill
			= _pgd.m_bHelperNoRect
			= !_pgd.m_bActive;
		if( _pgd.m_bSideBar )
			bInactiveResizableBarCapt = !_pgd.m_bActive;
		if( bInactiveResizableBarCapt )
		{
			CRect rcHelper( rcBarClient );
			rcHelper.OffsetRect( -rcBarWnd.TopLeft() );
			CRect rcGripper( _pgd.m_rcGripper );
			rcGripper.bottom = rcHelper.top + 4;
			stat_PaintGradientRect(
				dc,
				rcGripper,
				GetColor( _2003CLR_GRADIENT_LIGHT ),
				GetColor( _2003CLR_GRADIENT_DARK )
				);
		} // if( bInactiveResizableBarCapt )
		CExtPaintManagerXP::PaintGripper( dc, _pgd );
		return;
	} // if resizable bar caption

	if(		(! pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
		&&	(!bForceNoBaloon)
		)
	{ // if toolbar balloon style
		CDC & dcCompat =
			CachedDcGet(
				_pgd.m_bHorz
					? m_bmpCtBarGradientHorz
					: m_bmpCtBarGradientVert
				);
		if( dcCompat.GetSafeHdc() == NULL )
		{
			ASSERT( FALSE );
			return;
		}

		CRect rcPaintGradient( rcBarWnd );
		if( _pgd.m_bHorz )
		{
			rcPaintGradient.top = rcBarClient.top;
			rcPaintGradient.bottom = rcBarClient.bottom; // + 1;
			rcPaintGradient.right = rcBarClient.right;
			rcPaintGradient.OffsetRect(
				-rcPaintGradient.left,
				-rcPaintGradient.top
					+ rcBarClient.top - rcBarWnd.top
				);
		} // if( _pgd.m_bHorz )
		else
		{
			rcPaintGradient.left = rcBarClient.left;
			rcPaintGradient.right = rcBarClient.right; // + 1;
			rcPaintGradient.bottom = rcBarClient.bottom;
			rcPaintGradient.OffsetRect(
				-rcPaintGradient.left
					+ rcBarClient.left - rcBarWnd.left,
				-rcPaintGradient.top
				);
		} // else from if( _pgd.m_bHorz )
		int nIdxClrTbFillMargin = -1;
		CRgn _rgnBaloonSet;
		if(		m_sizeToolBarRgnRounders.cx > 0
			&&	m_sizeToolBarRgnRounders.cy > 0
			)
		{ // if baloon style
			nIdxClrTbFillMargin = m_nIdxClrTbFillMargin;
			if(		_rgnBaloonSet.CreateRoundRectRgn(
						rcPaintGradient.left,
						rcPaintGradient.top,
						rcPaintGradient.Width(),
						rcPaintGradient.Height(),
						m_sizeToolBarRgnRounders.cx,
						m_sizeToolBarRgnRounders.cy
						)
				)
			{
				dc.SelectClipRgn( &_rgnBaloonSet );
			}
#ifdef _DEBUG
			else
			{
				ASSERT( FALSE );
			}
#endif // _DEBUG
		}  // if baloon style

		CSize sizeShade = _pgd.m_bHorz
			? m_sizeCtBarGradientHorz
			: m_sizeCtBarGradientVert
			;
		int nOldStretchBltMode =
			dc.SetStretchBltMode(
				g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
				);
		::SetBrushOrgEx( dc, 0, 0, NULL );

		dc.StretchBlt(
			rcPaintGradient.left, rcPaintGradient.top,
			rcPaintGradient.Width(), rcPaintGradient.Height(),
			&dcCompat,
			0, 0, sizeShade.cx, sizeShade.cy,
			SRCCOPY
			);

		if( nIdxClrTbFillMargin >= 0 )
		{
			CPen _pen( PS_SOLID, 1, GetColor(m_nIdxClrTbFillMargin) );
			CPen  * pOldPen = dc.SelectObject( &_pen );
			if( _pgd.m_bHorz )
			{
				dc.MoveTo(
					rcPaintGradient.left,
					rcPaintGradient.bottom-2
					);
				dc.LineTo(
					rcPaintGradient.right,
					rcPaintGradient.bottom-2
					);
			} // if( _pgd.m_bHorz )
			else
			{
				dc.MoveTo(
					rcPaintGradient.right-2,
					rcPaintGradient.top
					);
				dc.LineTo(
					rcPaintGradient.right-2,
					rcPaintGradient.bottom
					);
			} // else from if( _pgd.m_bHorz )
			dc.SelectObject( pOldPen );
		} // if( nIdxClrTbFillMargin >= 0 )
		dc.SetStretchBltMode( nOldStretchBltMode );
		
		if( _rgnBaloonSet.GetSafeHandle() != NULL )
			dc.SelectClipRgn( NULL );
	} // if toolbar balloon style

COLORREF clrDotShadow = GetColor( _2003CLR_GRIPPER_DOT_LIGHT );
COLORREF clrDotFace = GetColor( _2003CLR_GRIPPER_DOT_DARK );
static const CSize g_sizeGripDot( 2, 2 );
static const CSize g_sizeGripDist( 1, 1 );
static const CSize g_sizeGripShadowOffset( 1, 1 );
CRect rcGripHelper( _pgd.m_rcGripper );
CRect rcBarClientW( rcBarClient );
	rcBarClientW.OffsetRect( -rcBarWnd.TopLeft() );
	if( _pgd.m_bHorz )
	{
		rcGripHelper.top = rcBarClientW.top;
		rcGripHelper.bottom = rcBarClientW.bottom;
		rcGripHelper.OffsetRect( 1, 0 );
		rcGripHelper.left +=
			(rcGripHelper.Width() - g_sizeGripDot.cx) / 2;
		rcGripHelper.right =
			rcGripHelper.left + g_sizeGripDot.cx;
		rcGripHelper.DeflateRect( 0, g_sizeGripDot.cy + g_sizeGripDist.cy + g_sizeGripShadowOffset.cy );
		int nDotCount = 
			rcGripHelper.Height()
			/ (g_sizeGripDot.cy + g_sizeGripDist.cy + g_sizeGripShadowOffset.cy);
		rcGripHelper.top +=
			rcGripHelper.Height()
			- (g_sizeGripDot.cy + g_sizeGripDist.cy + g_sizeGripShadowOffset.cy)
				* nDotCount;
		CRect rcDotFace( rcGripHelper );
		rcDotFace.bottom = rcDotFace.top + g_sizeGripDot.cy;
		CRect rcDotShadow( rcDotFace );
		rcDotShadow.OffsetRect( g_sizeGripShadowOffset );
		for( int nDot = 0; nDot < nDotCount; nDot++ )
		{
			dc.FillSolidRect( &rcDotShadow, clrDotShadow );
			dc.FillSolidRect( &rcDotFace, clrDotFace );
			rcDotFace.OffsetRect( 0, g_sizeGripDot.cy + g_sizeGripDist.cy + g_sizeGripShadowOffset.cy );
			rcDotShadow.OffsetRect( 0, g_sizeGripDot.cy + g_sizeGripDist.cy + g_sizeGripShadowOffset.cy );
		} // for( int nDot = 0; nDot < nDotCount; nDot++ )
	} // if( _pgd.m_bHorz )
	else
	{
		rcGripHelper.left = rcBarClientW.left;
		rcGripHelper.right = rcBarClientW.right;
rcGripHelper.OffsetRect( 0, 1 );
		rcGripHelper.top +=
			(rcGripHelper.Height() - g_sizeGripDot.cy) / 2;
		rcGripHelper.bottom =
			rcGripHelper.top + g_sizeGripDot.cy;
		rcGripHelper.DeflateRect( g_sizeGripDot.cx + g_sizeGripDist.cx + g_sizeGripShadowOffset.cx, 0 );
		int nDotCount = 
			rcGripHelper.Width()
			/ (g_sizeGripDot.cx + g_sizeGripDist.cx + g_sizeGripShadowOffset.cx);
		rcGripHelper.left +=
			rcGripHelper.Width()
			- (g_sizeGripDot.cx + g_sizeGripDist.cx + g_sizeGripShadowOffset.cx)
				* nDotCount;
		CRect rcDotFace( rcGripHelper );
		rcDotFace.right = rcDotFace.left + g_sizeGripDot.cx;
		CRect rcDotShadow( rcDotFace );
		rcDotShadow.OffsetRect( g_sizeGripShadowOffset );
		for( int nDot = 0; nDot < nDotCount; nDot++ )
		{
			dc.FillSolidRect( &rcDotShadow, clrDotShadow );
			dc.FillSolidRect( &rcDotFace, clrDotFace );
			rcDotFace.OffsetRect( g_sizeGripDot.cx + g_sizeGripDist.cx + g_sizeGripShadowOffset.cx, 0 );
			rcDotShadow.OffsetRect( g_sizeGripDot.cx + g_sizeGripDist.cx + g_sizeGripShadowOffset.cx, 0 );
		} // for( int nDot = 0; nDot < nDotCount; nDot++ )
	} // else from if( _pgd.m_bHorz )
}

CSize CExtPaintManager::GetPushedOffset()
{
	return CSize(1,1);
}

CSize CExtPaintManagerXP::GetPushedOffset()
{
	return CSize(0,0);
}

CSize CExtPaintManagerOffice2003::GetPushedOffset()
{
	return CSize(0,0);
}

#define __XP_DSS_MONO DSS_MONO

void CExtPaintManagerXP::InitTranslatedColors()
{
	ASSERT( this != NULL );

	m_DSS_DISABLED_style = __XP_DSS_MONO;

	CExtPaintManager::InitTranslatedColors();

int nBitsPerPixel = CExtPaintManager::stat_GetBPP();

COLORREF clrSysWindow = ::GetSysColor(COLOR_WINDOW);
COLORREF clrSys3dFace = ::GetSysColor(COLOR_3DFACE);
COLORREF clrSys3dHilight = ::GetSysColor(COLOR_3DHILIGHT);
COLORREF clrSys3dShadow = ::GetSysColor(COLOR_3DSHADOW);
COLORREF clrSys3dDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);
COLORREF clrSysBtnText = ::GetSysColor(COLOR_BTNTEXT);
COLORREF clrSysItemHilight = ::GetSysColor(COLOR_HIGHLIGHT);

COLORREF xpclr_MenuLight = clrSysWindow;
COLORREF xpclr_ControlBarBk = clrSys3dFace;
COLORREF xpclr_Highlight = clrSys3dFace;
COLORREF xpclr_Separator = clrSys3dShadow;
COLORREF xpclr_PanelBorder = clrSys3dShadow;
COLORREF xpclr_RarelyUsedMenuLeft = clrSys3dShadow; // clrSys3dFace;
COLORREF xpclr_HighlightDarked = clrSys3dShadow;
COLORREF xpclr_HighlightBorder = clrSysBtnText;

	if( nBitsPerPixel > 8 )
	{
		// xp - menu area light ver (like system COLOR_WINDOW but more brown)
		xpclr_MenuLight =
			stat_HLS_Adjust( clrSys3dFace, 0.00, 0.70, -0.50 );
		xpclr_MenuLight =
			RGB(
				GetRValue(xpclr_MenuLight) + ::MulDiv( 255 - GetRValue(xpclr_MenuLight), 40, 100 ),
				GetGValue(xpclr_MenuLight) + ::MulDiv( 255 - GetGValue(xpclr_MenuLight), 20, 100 ),
				GetBValue(xpclr_MenuLight)
				);
		xpclr_MenuLight =
			stat_HLS_Adjust(
				xpclr_MenuLight,
				0.00, 0.06, 0.05
				);
		// xp - rarely used item left side
		xpclr_RarelyUsedMenuLeft =
			RGB(
				( 999L*long(GetRValue(clrSys3dFace)) ) / 1000L,
				( 995L*long(GetGValue(clrSys3dFace)) ) / 1000L,
				( 995L*long(GetBValue(clrSys3dFace)) ) / 1000L
				);
		// xp - control bar funny background
		xpclr_ControlBarBk =
			stat_HLS_Adjust(
				xpclr_ControlBarBk,
				0.00, 0.20, 0.00
				);
		// xp - blue like light ver
		xpclr_Highlight =
			stat_HLS_Adjust(
				clrSysItemHilight,
				0.00, 0.75, -0.75
				);
		// xp - blue like dark ver
		xpclr_HighlightDarked =
			stat_HLS_Adjust(
				xpclr_Highlight,
				0.00, -0.25, 0.10
				);
		// xp - blue dark selected border
		xpclr_HighlightBorder =
			stat_HLS_Adjust(
				xpclr_HighlightDarked,
				0.00, -0.50, 0.00
				);
		// xp - dark gray separator
		xpclr_Separator = 
			RGB(
				( 857L*long(GetRValue(clrSys3dFace)) ) / 1000L,
				( 857L*long(GetGValue(clrSys3dFace)) ) / 1000L,
				( 857L*long(GetBValue(clrSys3dFace)) ) / 1000L
				);
		// xp - dark panel border (for floating controlbars)
		xpclr_PanelBorder =
			RGB(
				( 750L*long(GetRValue(clrSys3dShadow)) + 250L*long(GetRValue(clrSys3dDkShadow)) ) / 1000L,
				( 750L*long(GetGValue(clrSys3dShadow)) + 250L*long(GetGValue(clrSys3dDkShadow)) ) / 1000L,
				( 750L*long(GetBValue(clrSys3dShadow)) + 250L*long(GetBValue(clrSys3dDkShadow)) ) / 1000L
				);
	} // if( nBitsPerPixel > 8 )

	m_mapColorTranslate[CLR_3DFACE_OUT]				= InstallColor( xpclr_ControlBarBk );
	m_mapColorTranslate[CLR_3DFACE_IN]				= InstallColor( xpclr_Highlight );
	m_mapColorTranslate[CLR_3DFACE_DISABLED]		= COLOR_3DFACE;

int idxClrDark = InstallColor( xpclr_PanelBorder );

	m_mapColorTranslate[CLR_3DLIGHT_OUT]			= idxClrDark;
	m_mapColorTranslate[CLR_3DLIGHT_IN]				= idxClrDark;
	m_mapColorTranslate[CLR_3DLIGHT_DISABLED]		= idxClrDark;

	m_mapColorTranslate[CLR_3DHILIGHT_OUT]			= idxClrDark;
	m_mapColorTranslate[CLR_3DHILIGHT_IN]			= idxClrDark;
	m_mapColorTranslate[CLR_3DHILIGHT_DISABLED]		= idxClrDark;

	m_mapColorTranslate[CLR_3DSHADOW_OUT]			= idxClrDark;
	m_mapColorTranslate[CLR_3DSHADOW_IN]			= idxClrDark;
	m_mapColorTranslate[CLR_3DSHADOW_DISABLED]		= idxClrDark;

	m_mapColorTranslate[CLR_3DDKSHADOW_OUT]			= idxClrDark;
	m_mapColorTranslate[CLR_3DDKSHADOW_IN]			= idxClrDark;
	m_mapColorTranslate[CLR_3DDKSHADOW_DISABLED]	= idxClrDark;
	
	m_mapColorTranslate[CLR_TEXT_OUT]				= COLOR_BTNTEXT;
	m_mapColorTranslate[CLR_TEXT_IN] =
		InstallColor(
			(nBitsPerPixel > 8)
				? RGB(0,0,0)
				: clrSys3dHilight
			);
	m_mapColorTranslate[CLR_TEXT_DISABLED]			= COLOR_3DSHADOW;
	m_mapColorTranslate[XPCLR_PUSHEDHOVERTEXT]		= COLOR_HIGHLIGHTTEXT;

	m_mapColorTranslate[XPCLR_3DFACE_DARK]			= InstallColor( xpclr_ControlBarBk );
	m_mapColorTranslate[XPCLR_3DFACE_NORMAL]		= InstallColor( xpclr_MenuLight );
	m_mapColorTranslate[XPCLR_SEPARATOR]			= InstallColor( xpclr_Separator ); 

	m_mapColorTranslate[XPCLR_HILIGHT]				= InstallColor( xpclr_HighlightDarked ); 

	m_mapColorTranslate[XPCLR_HILIGHT_BORDER]		= InstallColor( xpclr_HighlightBorder ); 
	
	m_mapColorTranslate[XPCLR_RARELY_BORDER]		= InstallColor( xpclr_RarelyUsedMenuLeft );

	m_mapColorTranslate[XPCLR_MENU_BORDER]			= COLOR_3DDKSHADOW;

	m_mapColorTranslate[XPCLR_3DFACE_FLOAT_R]			= InstallColor( xpclr_MenuLight );
	m_mapColorTranslate[XPCLR_3DFACE_FLOAT_F]			= InstallColor( GetColor(CLR_3DFACE_OUT) );
	
}

void CExtPaintManagerXP::PaintPushButton(
	CDC & dc,
	CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
	)
{

	if( _ppbd.m_rcClient.IsRectEmpty() )
		return;

	if( !_ppbd.m_bEnabled )
	{
		_ppbd.m_bHover = false;
		//_ppbd.m_bDrawBorder = false;
		_ppbd.m_bDrawFocusRect = false;
		_ppbd.m_bIndeterminate = false;
	}

COLORREF clrOldText = dc.GetTextColor();
COLORREF clrOldBk = dc.GetBkColor();
int nOldBkMode = dc.SetBkMode( TRANSPARENT );

int nIdxMainGradientLeft = -1;
int nIdxMainGradientRight = -1;
	// Prepare draw... paint button's area with background color
COLORREF bgColor, bgColorDD;
	if(	_ppbd.m_bHover || _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
	{
		bgColor = bgColorDD = GetColor( CLR_3DFACE_IN );
		if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
		{
			(( _ppbd.m_bPushedDropDown ) ? bgColorDD : bgColor) =
				GetColor( _ppbd.m_bHover ? XPCLR_HILIGHT : CLR_3DFACE_IN);
		} // if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
		if(		(!_ppbd.m_bHover)
			&&	_ppbd.m_bPushed
			&&	(!_ppbd.m_bPushedDropDown)
			&&	_ppbd.m_bSeparatedDropDown
			)
			bgColorDD =  GetColor( CLR_3DFACE_OUT );
		if( _ppbd.m_bHover )
		{
			if( _ppbd.m_bPushed )
			{
				nIdxMainGradientLeft = m_nIdxClrBtnHovPresLeft;
				nIdxMainGradientRight = m_nIdxClrBtnHovPresRight;
			} // if( _ppbd.m_bPushed )
			else
			{
				nIdxMainGradientLeft = m_nIdxClrBtnHoverLeft;
				nIdxMainGradientRight = m_nIdxClrBtnHoverRight;
			} // else from if( _ppbd.m_bPushed )
		} // if( _ppbd.m_bHover )
		else if( _ppbd.m_bPushed )
		{
			nIdxMainGradientLeft = m_nIdxClrBtnPressedLeft;
			nIdxMainGradientRight = m_nIdxClrBtnPressedRight;
		} // else if( _ppbd.m_bPushed )
	} // if(	_ppbd.m_bHover || _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
	else
	{
		bgColor = bgColorDD =
			GetColor(
				_ppbd.m_bFlat ? CLR_3DFACE_OUT : COLOR_3DFACE
				);
	}

COLORREF clrDkBorder =
		GetColor(
			(_ppbd.m_bHover || _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
				? XPCLR_HILIGHT_BORDER
				: ( _ppbd.m_bFlat ? XPCLR_HILIGHT_BORDER : COLOR_3DDKSHADOW )
			);
bool bPushedDropDown = false;
	if( (_ppbd.m_bPushed && !_ppbd.m_bSeparatedDropDown)
		|| _ppbd.m_bPushedDropDown
		)
		bPushedDropDown = true;

CRect rectClient( _ppbd.m_rcClient );
CRect rectTotal( _ppbd.m_rcClient );
CRect rectCaption( _ppbd.m_rcClient );
CRect rectDropDown( 0,0,0,0 );
	if( _ppbd.m_bDropDown )
	{
		rectDropDown = rectClient;
		if( _ppbd.m_bHorz )
		{
			rectCaption.right -=
				__DROPDOWN_ARROW_GAP*2
				+ g_glyph_btn_expand_bottom.Size().cx
				;
			rectDropDown.left = rectCaption.right - 1;
		} // if( _ppbd.m_bHorz )
		else
		{
			rectCaption.bottom -=
				__DROPDOWN_ARROW_GAP*2
				+ g_glyph_btn_expand_right.Size().cy
				;
			rectDropDown.top = rectCaption.bottom - 1;
		} // if( _ppbd.m_bHorz )

		if( !_ppbd.m_bTransparentBackground )
		{
			CBrush br( bgColorDD );
			dc.FillRect( &rectDropDown, &br );
		} // if( !_ppbd.m_bTransparentBackground )

		if( _ppbd.m_bSeparatedDropDown )
		{
			rectClient = rectCaption;

			if( bPushedDropDown )
			{
				
				if( _ppbd.m_bFlat )
				{
					if( _ppbd.m_bDrawBorder )
						dc.Draw3dRect( rectDropDown, clrDkBorder, clrDkBorder );
				} // if( _ppbd.m_bFlat )
				else    
					dc.Draw3dRect( rectDropDown, clrDkBorder, clrDkBorder );
			} // if( bPushedDropDown )
			else
			{

				if( _ppbd.m_bFlat )
				{
					if( _ppbd.m_bHover && _ppbd.m_bDrawBorder )
						dc.Draw3dRect( rectDropDown, clrDkBorder, clrDkBorder );
				} // if( _ppbd.m_bFlat )
				else
						dc.Draw3dRect( rectDropDown, clrDkBorder, clrDkBorder );
			} // else from if( bPushedDropDown )
		} // if( _ppbd.m_bSeparatedDropDown )
	} // if( _ppbd.m_bDropDown )

	if( !_ppbd.m_bTransparentBackground )
	{
		if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
		{
			stat_PaintGradientRect(
				dc,
				rectClient,
				GetColor( _ppbd.m_bHorz ? nIdxMainGradientRight : nIdxMainGradientLeft ),
				GetColor( _ppbd.m_bHorz ? nIdxMainGradientLeft : nIdxMainGradientRight ),
				_ppbd.m_bHorz
				);
		} // if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
		else
		{
			CBrush br( bgColor );
			dc.FillRect( &rectClient, &br );
		} // else from if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
	} // if( !_ppbd.m_bTransparentBackground )
	
	// Draw pressed button
	if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
	{

		if( _ppbd.m_bFlat )
		{
			if( _ppbd.m_bDrawBorder )
				dc.Draw3dRect( rectClient, clrDkBorder, clrDkBorder );
		}
		else    
			dc.Draw3dRect( rectClient, clrDkBorder, clrDkBorder );
	
	} // if( bPushed )
	else // ...else draw non pressed button
	{
		if( _ppbd.m_bFlat )
		{
			if( _ppbd.m_bHover && _ppbd.m_bDrawBorder )
				dc.Draw3dRect( rectClient, clrDkBorder, clrDkBorder );
		} // if( _ppbd.m_bFlat )
		else
		{
			dc.Draw3dRect( rectClient, clrDkBorder, clrDkBorder );
			if( _ppbd.m_bHover )
				dc.Draw3dRect( rectTotal, clrDkBorder, clrDkBorder );
		}  // else from if( _ppbd.m_bFlat )
	} // else from if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )

	if( _ppbd.m_bDropDown )
	{
		CRect rectGlyph( rectDropDown );
		if( bPushedDropDown )
			rectGlyph.OffsetRect( GetPushedOffset() );
		COLORREF ColorValues[2] =
		{
			RGB(0,0,0),
			GetColor(
//				_ppbd.m_bEnabled
//					?	(_ppbd.m_bHover || /*_ppbd.m_bPushed ||*/ _ppbd.m_bPushedDropDown)
//							? ((_ppbd.m_bHover && (/*_ppbd.m_bPushed ||*/ _ppbd.m_bPushedDropDown)) ? COLOR_HIGHLIGHTTEXT : CLR_TEXT_IN )
//							: ( _ppbd.m_bFlat ? CLR_TEXT_OUT : COLOR_BTNTEXT )
//					: CLR_3DSHADOW_OUT
				_ppbd.m_bEnabled
					?	(_ppbd.m_bHover || _ppbd.m_bPushedDropDown)
							? ( ( _ppbd.m_clrForceTextPressed == ((COLORREF)-1L) )
									?	( (_ppbd.m_bHover && _ppbd.m_bPushedDropDown)
											? COLOR_HIGHLIGHTTEXT : CLR_TEXT_IN )
									: _ppbd.m_clrForceTextPressed )
							: ( ( _ppbd.m_clrForceTextNormal == ((COLORREF)-1L) ) ? ( _ppbd.m_bFlat ? CLR_TEXT_OUT : COLOR_BTNTEXT ) : _ppbd.m_clrForceTextNormal )
					: ( ( _ppbd.m_clrForceTextDisabled == ((COLORREF)-1L) ) ? CLR_3DSHADOW_OUT : _ppbd.m_clrForceTextDisabled )
				)
		};
		PaintGlyphCentered(
			dc,
			rectGlyph,
			_ppbd.m_bHorz ? g_glyph_btn_expand_bottom : g_glyph_btn_expand_right,
			ColorValues
			);
	} // if( _ppbd.m_bDropDown )


	rectClient = rectCaption;

CExtSafeString sTitle( _T("") );
	if( _ppbd.m_sText != NULL )
		sTitle = _ppbd.m_sText;

	// Draw the icon
	if(	rectClient.bottom > rectClient.top
		&& rectClient.right > rectClient.left
		)
	{
		if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		{
			PaintIcon(
				dc,
				_ppbd.m_bHorz,
				sTitle,
				_ppbd.m_pIcon,
				rectClient,
				rectCaption,
				_ppbd.m_bPushed || _ppbd.m_bPushedDropDown,
				_ppbd.m_bEnabled,
				_ppbd.m_bHover,
				_ppbd.m_eAlign
				);
		} // if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		else
		{
			PaintSysCmdGlyph(
				dc,
				_ppbd.m_nHelperSysCmdID,
				rectClient,
				false,
				_ppbd.m_bPushed || _ppbd.m_bPushedDropDown,
				_ppbd.m_bEnabled
				);
		} // else from if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
	}

	if( rectCaption.bottom > rectCaption.top
		&& rectCaption.right > rectCaption.left
		)
	{ // if we have valid area for text & focus rect
		if( !sTitle.IsEmpty() )
		{
			if( _ppbd.m_bPushed || _ppbd.m_bPushedDropDown )
			{
				CSize sizePushedOffset = GetPushedOffset();
				rectCaption.OffsetRect(sizePushedOffset);
			}
			dc.SetBkColor( bgColor );
			dc.SetBkMode( TRANSPARENT );

			if( _ppbd.m_bEnabled ) 
			{
				int nColorIdx =
					(_ppbd.m_bHover || _ppbd.m_bPushed )
						? (	(_ppbd.m_bHover && _ppbd.m_bPushed)
								? XPCLR_PUSHEDHOVERTEXT
								: CLR_TEXT_IN
							)
						: ( _ppbd.m_bFlat ? CLR_TEXT_OUT : COLOR_BTNTEXT )
					;
				if(			nColorIdx == XPCLR_PUSHEDHOVERTEXT
						&&	_ppbd.m_clrForceTextPressed != ((COLORREF)-1L)
						)
						dc.SetTextColor( _ppbd.m_clrForceTextPressed );
				else if(	nColorIdx == CLR_TEXT_IN
						&&	_ppbd.m_clrForceTextHover != ((COLORREF)-1L)
						)
						dc.SetTextColor( _ppbd.m_clrForceTextHover );
				else if(	( ! (_ppbd.m_bHover || _ppbd.m_bPushed ) )
						&&	_ppbd.m_clrForceTextNormal != ((COLORREF)-1L)
						)
						dc.SetTextColor( _ppbd.m_clrForceTextNormal );
				else
						dc.SetTextColor( GetColor( nColorIdx ) );
			} // if( _ppbd.m_bEnabled ) 
			else 
			{
				if( _ppbd.m_clrForceTextDisabled != ((COLORREF)-1L) )
					dc.SetTextColor( _ppbd.m_clrForceTextDisabled );
				else
					dc.SetTextColor( GetColor( COLOR_3DSHADOW ) );
			} // else from if( _ppbd.m_bEnabled ) 

//			// center text
//			CRect centerRect = rectCaption;

			CFont * pOldBtnFont = NULL;
			CFont * pCurrFont = NULL;
			CFont fontDummy;

			if( _ppbd.m_hFont != NULL )
			{
				if( _ppbd.m_bHorz )
					pCurrFont = CFont::FromHandle( _ppbd.m_hFont );
				else
				{
					LOGFONT lf;
					::memset(&lf,0,sizeof(LOGFONT));
					::GetObject( _ppbd.m_hFont, sizeof(LOGFONT), &lf );
					if( lf.lfEscapement == 0 )
					{
						lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT__;
						VERIFY(
							fontDummy.CreateFontIndirect(&lf)
							);
						pCurrFont = &fontDummy;
					}
					else
					{
						// suppose font already prepared
						pCurrFont = CFont::FromHandle( _ppbd.m_hFont );
					}
				}
			} // if( _ppbd.m_hFont != NULL )
			else
			{
				if( _ppbd.m_bHorz )
				{
					if( _ppbd.m_bDefaultPushButton )
						pCurrFont = &m_FontBold;
					else
						pCurrFont = &m_FontNormal;
				}
				else
				{
					if( _ppbd.m_bDefaultPushButton )
						pCurrFont = &m_FontBoldVert;
					else
						pCurrFont = &m_FontNormalVert;
				}
			} // else from if( _ppbd.m_hFont != NULL )
			ASSERT( pCurrFont != NULL );
			pOldBtnFont = dc.SelectObject( pCurrFont );
			ASSERT( pOldBtnFont != NULL );

			int nTextLength = sTitle.GetLength();
			int nAmpIndex = sTitle.Find( _T('&') );
			CExtSafeString sBtn;
			if( nAmpIndex < 0 )
				sBtn = sTitle;
			else
				sBtn =
					sTitle.Left( nAmpIndex )
					+ sTitle.Right( nTextLength - (nAmpIndex+1) )
					;
			ASSERT( sBtn.Find( _T('&') ) < 0 );

			CRect rcText(0,0,0,0);
			dc.DrawText(
				sBtn,
				-1,
				rcText,
				DT_SINGLELINE|DT_CALCRECT
				);
			CRect rcCenteredCaption(rectCaption);
			rcCenteredCaption.OffsetRect(
				(rectCaption.Width() - rcText.Width())/2,
				(rectCaption.Height() - rcText.Height())/2
				);

			if( _ppbd.m_bHorz )
			{
				dc.DrawText(
					sTitle,
					-1,
					rectCaption,
					_ppbd.m_nDrawTextFlagsH
					);
			}
			else
			{
				LOGFONT lf;
				::memset(&lf,0,sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);
				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin =
					(rectCaption.Width() - _cyHorzFont)  / 2
					;

				CPoint
					ptLineFrom(0,0),
					ptLineTo(0,0);
				if( nAmpIndex >= 0 )
				{
					ptLineFrom =
						CPoint(
							__ExtMfc_CXTEXTMARGIN
								+ _cyTextMargin,
							__ExtMfc_CYTEXTMARGIN
								+ stat_CalcTextWidth(
									dc,
									_ppbd.m_bDefaultPushButton ?
										m_FontBold : m_FontNormal,
									sBtn.Left(nAmpIndex)
									)
							);
					ptLineTo =
						CPoint(
							__ExtMfc_CXTEXTMARGIN
								+ _cyTextMargin,
							__ExtMfc_CYTEXTMARGIN
								+ stat_CalcTextWidth(
									dc,
									_ppbd.m_bDefaultPushButton ?
										m_FontBold : m_FontNormal,
									sBtn.Left(nAmpIndex+1)
									)
							);
					int nXOffsPlus =
						rectCaption.Width()
						- stat_CalcTextDimension(
							dc,
							_ppbd.m_bDefaultPushButton ? m_FontBold : m_FontNormal,
							sBtn
							).Height();
					nXOffsPlus /= 2;
					nXOffsPlus -= 4;
					ptLineFrom.x += nXOffsPlus;
					ptLineTo.x   += nXOffsPlus;
				} // if( nAmpIndex >= 0 )

				CRect rcString = 
					CRect(
						CPoint(
							rectCaption.right - _cyTextMargin + __ExtMfc_CXTEXTMARGIN,
							rectCaption.top + __ExtMfc_CYTEXTMARGIN
							),
						rectCaption.Size() //m_sizeHorz
						);
				rcString.DeflateRect(2,2);

				CSize ptFix(-rcString.Width()-1,0);
				ptLineFrom += rcString.TopLeft() + ptFix;
				ptLineTo += rcString.TopLeft() + ptFix;
				if( _ppbd.m_bEnabled )
				{
/*VTX*/
					dc.DrawText(
						sBtn,
						sBtn.GetLength(),
						rcString,
						DT_SINGLELINE|DT_NOCLIP|DT_NOPREFIX
						);
					if( nAmpIndex >= 0 )
					{
						CPen pen(
							PS_SOLID,
							0,
							dc.GetTextColor()
							);
						CPen * pOldPen = dc.SelectObject( &pen );
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
						dc.SelectObject( pOldPen );
					} // if( nAmpIndex >= 0 )
				} // if( _ppbd.m_bEnabled )
				else
				{
//					CPoint ptOffsetPre(1,1);
//					CPoint ptOffsetPost(-1,-1);
//					COLORREF clrText = GetColor(COLOR_3DSHADOW);
//					COLORREF clrShadow = GetColor(COLOR_3DHILIGHT);
//					rcString.OffsetRect(ptOffsetPre.x,ptOffsetPre.y);
//					dc.SetTextColor( clrShadow );
					dc.DrawText(
						sBtn,
						sBtn.GetLength(),
						rcString,
						DT_SINGLELINE //|DT_CENTER|DT_VCENTER
							|DT_NOCLIP|DT_NOPREFIX
						); // don't forget DT_NOCLIP
//					rcString.OffsetRect(ptOffsetPost.x,ptOffsetPost.y);
//					dc.SetTextColor( clrText );
//					dc.DrawText(
//						sBtn,
//						rcString,
//						DT_SINGLELINE //|DT_CENTER|DT_VCENTER
//							|DT_NOCLIP|DT_NOPREFIX
//						); // don't forget DT_NOCLIP
					if( nAmpIndex >= 0 )
					{
						CPen pen(
							PS_SOLID,
							0,
							dc.GetTextColor() // clrShadow
							);
						CPen * pOldPen = dc.SelectObject( &pen );
//						ptLineFrom += ptOffsetPre;
//						ptLineTo += ptOffsetPre;
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
//						pen.DeleteObject();
//						pen.CreatePen(
//							PS_SOLID,
//							0,
//							clrText
//							);
//						dc.SelectObject( &pen );
//						ptLineFrom += ptOffsetPost;
//						ptLineTo += ptOffsetPost;
//						dc.MoveTo( ptLineFrom );
//						dc.LineTo( ptLineTo );
						dc.SelectObject( pOldPen );
					} // if( nAmpIndex >= 0 )
				} // else from if( _ppbd.m_bEnabled )

			}

			dc.SelectObject(pOldBtnFont);
		} // if( !sTitle.IsEmpty() )

		if( _ppbd.m_bDrawFocusRect )
		{
			CRect focusRect = rectCaption; // rectClient
			focusRect.DeflateRect( 3, 3 );
			dc.DrawFocusRect(&focusRect);
		} // if( _ppbd.m_bDrawFocusRect )
	} // if we have valid area for text & focus rect


	dc.SetBkMode( nOldBkMode );
	dc.SetBkColor( clrOldBk );
	dc.SetTextColor( clrOldText );
}

void CExtPaintManagerXP::PaintIcon(
	CDC & dc,
	bool bHorz,
	CExtSafeString & sTitle,
	CExtCmdIcon * pIcon,
	const CRect & rectItem,
	CRect & rectCaption,
	bool bPushed,
	bool bEnabled,
	bool bHover,
	int eAlign
	)
{
	rectCaption = rectItem;
	if( pIcon == NULL || pIcon->IsEmpty() )
		return;

CSize _sizeIcon = pIcon->GetSize();
	ASSERT( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 );

CRect iconRect = rectItem;
CRect btnRect;
	if( bHorz )
	{
		if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		{
			if( sTitle.IsEmpty() )
				// Center the icon horizontally
				iconRect.left += ((iconRect.Width() - _sizeIcon.cx)/2);
			else
			{
				iconRect.left += 3;  
				rectCaption.left += _sizeIcon.cx + 3;
			}
		} // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		else
		{
			if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
			{
				btnRect = rectCaption;
				if( sTitle.IsEmpty() )
					// Center the icon horizontally
					iconRect.left += ((iconRect.Width() - _sizeIcon.cx)/2);
				else
				{
					rectCaption.right = rectCaption.Width() - _sizeIcon.cx - 3;
					rectCaption.left = 3;
					iconRect.left = btnRect.right - _sizeIcon.cx - 3;
					// Center the icon vertically
					iconRect.top += ((iconRect.Height() - _sizeIcon.cy)/2);
				}
			} // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
		} // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		if( (eAlign&__ALIGN_VERT) != 0 )
		{
			// Center the icon horizontally
			iconRect.top += ((iconRect.Height() - _sizeIcon.cy)/2);
		} // if( (eAlign&__ALIGN_VERT) != 0 )
	} // if( bHorz )
	else
	{
		if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		{
			if( sTitle.IsEmpty() )
				// Center the icon horizontally
				iconRect.top += ((iconRect.Height() - _sizeIcon.cy)/2);
			else
			{
				iconRect.top += 3;  
				rectCaption.top += _sizeIcon.cy + 3;
			}
		} // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		else
		{
			if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
			{
				btnRect = rectCaption;
				if( sTitle.IsEmpty() )
					// Center the icon horizontally
					iconRect.top += ((iconRect.Height() - _sizeIcon.cy)/2);
				else
				{
					rectCaption.top = rectCaption.Height() - _sizeIcon.cy - 3;
					rectCaption.top = 3;
					iconRect.top = btnRect.top - _sizeIcon.cy - 3;
					// Center the icon vertically
					iconRect.left += ((iconRect.Width() - _sizeIcon.cx)/2);
				}
			} // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
		} // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		if( (eAlign&__ALIGN_VERT) != 0 )
		{
			// Center the icon horizontally
			iconRect.left += ((iconRect.Width() - _sizeIcon.cx)/2);
		} // if( (eAlign&__ALIGN_VERT) != 0 )
	} // else from if( bHorz )
    
	// If button is pressed then press the icon also
	if( bPushed )
	{
		CSize sizePushedOffset = GetPushedOffset();
		iconRect.OffsetRect(sizePushedOffset.cx, sizePushedOffset.cy);
	}

CPoint ptTopLeft = iconRect.TopLeft();
	if( m_bEnabledHoverIconShadows )
		ptTopLeft += CSize(1,1);
	if( bEnabled )
	{
		CPoint ptOffs( 0, 0 );
		if( m_bEnabledHoverIconShadows && bHover && (!bPushed) )
		{
			CBrush brBack;
			if( !brBack.CreateSolidBrush( GetColor( COLOR_3DSHADOW ) ) )
			{
				ASSERT(FALSE);
				return;
			} // if( !brBack.CreateSolidBrush( GetColor( COLOR_3DSHADOW ) ) )
//			dc.DrawState(
//				ptTopLeft + ptOffs,
//				iconRect.Size(), 
//				pIcon->GetIcon(), 
//				__XP_DSS_MONO, 
//				&brBack
//				);
			pIcon->Paint(
				dc,
				ptTopLeft + ptOffs,
				CExtCmdIcon::__DRAW_MONO,
				(HBRUSH)brBack.GetSafeHandle()
				);
			ptOffs.x = ptOffs.y = -2;
		} // if( m_bEnabledHoverIconShadows && bHover && (!bPushed) )
//		dc.DrawState(
//			ptTopLeft + ptOffs,
//			iconRect.Size(), 
//			pIcon->GetIcon(), 
//			DSS_NORMAL, 
//			(CBrush*)NULL
//			);
		pIcon->Paint(
			dc,
			ptTopLeft + ptOffs,
			CExtCmdIcon::__DRAW_NORMAL
			);
	} // DSS_NORMAL
	else
	{
		CBrush brDisabled;
		if( !brDisabled.CreateSolidBrush(GetColor(XPCLR_SEPARATOR)) )
		{
			ASSERT(FALSE);
			return;
		}
//		dc.DrawState(
//			ptTopLeft,
//			iconRect.Size(), 
//			pIcon->GetIcon(), 
//			__XP_DSS_MONO, 
//			&brDisabled
//			);
		COLORREF clrAlpha = GetIconAlphaColor();
		if( clrAlpha != (COLORREF(-1L)) )
		{
			if( clrAlpha != pIcon->AlphaColorGet() )
				pIcon->AlphaColorSet( clrAlpha );
			double lfAlphaAdjustPercent = GetIconAlphaAdjustPercent();
			if( lfAlphaAdjustPercent != pIcon->AlphaAdjustPercentGet() )
				pIcon->AlphaAdjustPercentSet( lfAlphaAdjustPercent );
		} // if( clrAlpha != (COLORREF(-1L)) )
		pIcon->Paint(
			dc,
			iconRect.TopLeft(),
			( clrAlpha == (COLORREF(-1L)) )
				? CExtCmdIcon::__DRAW_MONO
				: CExtCmdIcon::__DRAW_ALPHA,
			(HBRUSH)brDisabled.GetSafeHandle()
			);
	} // DSS_NORMAL
}

void CExtPaintManagerXP::PaintComboFrame(
	CDC &dc,
	CExtPaintManager::PAINTCOMBOFRAMEDATA & _pcfd
	)
{
CRect rectClient2( _pcfd.m_rcClient );
int nSavedDC = dc.SaveDC();

COLORREF clrWindow =
		( _pcfd.m_clrForceNABorder == COLORREF(-1L) || _pcfd.m_bHover || _pcfd.m_bPushed )
		? GetColor(
			_pcfd.m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE
			)
		: _pcfd.m_clrForceNABorder
		;
COLORREF clrFaceOut =
		( _pcfd.m_clrForceNAContent == COLORREF(-1L) || _pcfd.m_bHover || _pcfd.m_bPushed )
		? GetColor( CLR_3DFACE_OUT )
		: _pcfd.m_clrForceNAContent
		;
COLORREF clrHilight =
		( _pcfd.m_clrForceNABorder == COLORREF(-1L) || _pcfd.m_bHover || _pcfd.m_bPushed )
		? GetColor( XPCLR_3DFACE_NORMAL )
		: _pcfd.m_clrForceNABorder
		;
COLORREF clrShadow =
		( _pcfd.m_clrForceNABorder == COLORREF(-1L) || _pcfd.m_bHover || _pcfd.m_bPushed )
		? GetColor( CLR_3DFACE_OUT )
		: _pcfd.m_clrForceNABorder
		;

CRect rectClient( _pcfd.m_rcClient );
COLORREF clrOuterBorder =
		(_pcfd.m_clrForceActiveOuterBorder != COLORREF(-1L) && (_pcfd.m_bHover || _pcfd.m_bPushed) )
			? _pcfd.m_clrForceActiveOuterBorder
			: clrFaceOut
			;
	if( !_pcfd.m_bNoTopOuterBorder )
		dc.Draw3dRect(
			rectClient,
			clrOuterBorder,
			clrOuterBorder 
			);
	rectClient.DeflateRect(1,1);
	dc.Draw3dRect(
		rectClient,
		clrHilight,
		clrHilight
		);

	rectClient.left =
		rectClient.right -
		::GetSystemMetrics(SM_CXHTHUMB);
	dc.Draw3dRect(
		rectClient,
		clrHilight,
		clrHilight
		);
	rectClient.DeflateRect(1,1);
	dc.Draw3dRect(
		rectClient,
		clrHilight,
		clrHilight
		);

	dc.FillSolidRect(
		rectClient.left-2,
		rectClient.top,
		3,
		rectClient.Height(),
		clrWindow
		);

	if( _pcfd.m_bHover || _pcfd.m_bPushed )
		dc.FillSolidRect(
			rectClient,
			clrHilight
			);

	rectClient.InflateRect(1,1);

	dc.FillSolidRect(
		&rectClient,
		( _pcfd.m_clrForceNAContent == COLORREF(-1L) || _pcfd.m_bHover || _pcfd.m_bPushed )
			? clrFaceOut
			: _pcfd.m_clrForceNAContent
		);

CFont font;
int ppi = dc.GetDeviceCaps(LOGPIXELSX);
int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
	font.CreatePointFont(pointsize, _T("Marlett"));

PAINTPUSHBUTTONDATA _ppbd(
		this,
		true,
		rectClient,
		_T("6"),
		NULL,
		true,
		(_pcfd.m_bHover||_pcfd.m_bPushed) ? true : false,
		_pcfd.m_bPushed ? true : false,
		false,
		_pcfd.m_bEnabled,
		true,
		false,
		false,
		CExtPaintManager::__ALIGN_HORIZ_CENTER
			| CExtPaintManager::__ALIGN_VERT,
		(HFONT)font.GetSafeHandle(), // = NULL
		false,
		0,
		( _pcfd.m_clrForceNAContent == COLORREF(-1L) || _pcfd.m_bHover || _pcfd.m_bPushed )
			? false
			: true
		);
	PaintPushButton( dc, _ppbd );

	if( !(_pcfd.m_bHover || _pcfd.m_bPushed) )
	{
		dc.Draw3dRect(
			rectClient,
			clrHilight,
			clrHilight
			);
	}

	if(_pcfd.m_bHover || _pcfd.m_bPushed)
	{
		rectClient2.DeflateRect(1,1);
		HBRUSH hOldBrush = (HBRUSH)
			::SelectObject(
				dc.GetSafeHdc(),
				::GetStockObject(NULL_BRUSH)
				);
		COLORREF clrOld = dc.SetTextColor( clrShadow );
		dc.Rectangle(
			&rectClient2
			);
		dc.SetTextColor( clrOld );
		if( hOldBrush )
			::SelectObject(
				dc.GetSafeHdc(),
				hOldBrush
				);
	}

	dc.RestoreDC( nSavedDC );
}

void CExtPaintManagerOffice2003::PaintComboFrame(
	CDC &dc,
	CExtPaintManager::PAINTCOMBOFRAMEDATA & _pcfd
	)
{
	if(		_pcfd.m_pHelperSrc == NULL
		||	(! _pcfd.m_bEnabled )
		||	_pcfd.m_clrForceNABorder != COLORREF(-1L)
		||	_pcfd.m_clrForceNAContent != COLORREF(-1L)
		||	_pcfd.m_clrForceActiveOuterBorder != COLORREF(-1L)
		||	(!	(	_pcfd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CComboBox))
				||	_pcfd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtBarButton))
				)
			)
		||	(! GetCb2DbTransparentMode(_pcfd.m_pHelperSrc) )
		||	m_bmpCtBarGradientHorz.GetSafeHandle() == NULL
		||	m_bmpCtBarGradientVert.GetSafeHandle() == NULL
		||	m_bmpDockerGradient.GetSafeHandle() == NULL
		||	stat_GetBPP() <= 8
		)
	{
		CExtPaintManagerXP::PaintComboFrame( dc, _pcfd );
		return;
	}

int nDxHumb = ::GetSystemMetrics( SM_CXHTHUMB );
CRect rcClient( _pcfd.m_rcClient );
CRect rcEraser( rcClient );
	rcEraser.DeflateRect(1,1);
CRect rcHoverFrame( rcEraser );
CRect rcButton( rcEraser );
	rcButton.left = rcButton.right - nDxHumb;
CRect rcClientItem( rcEraser );
	rcClientItem.DeflateRect(1,1,nDxHumb-1+3,1);

CRgn rgnClip, rgnClientItem;
	if(		rgnClip.CreateRectRgnIndirect(&rcClient)
		&&	rgnClientItem.CreateRectRgnIndirect(&rcClientItem)
		&&	rgnClip.CombineRgn(
				&rgnClip,
				&rgnClientItem, 
				RGN_DIFF
				) != ERROR
		)
		dc.SelectClipRgn( &rgnClip );

CComboBox * pWndCombo =
		DYNAMIC_DOWNCAST(
			CComboBox,
			_pcfd.m_pHelperSrc
			);
	if( pWndCombo != NULL )
	{
		ASSERT_VALID( pWndCombo );
		if( !PaintDockerBkgnd( dc, pWndCombo ) )
		{
			if( rgnClip.GetSafeHandle() != NULL )
				dc.SelectClipRgn( NULL );
			CExtPaintManagerXP::PaintComboFrame( dc, _pcfd );
			return;
		}
	} // if( pWndCombo != NULL )

int nSavedDC = dc.SaveDC();

COLORREF clrWindow = GetColor( COLOR_WINDOW );
	dc.Draw3dRect( rcEraser, clrWindow, clrWindow );
	rcEraser.DeflateRect(0,1,nDxHumb-1,1);
	dc.Draw3dRect( rcEraser, clrWindow, clrWindow );
	rcEraser.DeflateRect(0,0,1,0);
	dc.Draw3dRect( rcEraser, clrWindow, clrWindow );
	rcEraser.DeflateRect(0,0,1,0);
	dc.Draw3dRect( rcEraser, clrWindow, clrWindow );
	rcEraser.DeflateRect(0,0,1,0);
	dc.Draw3dRect( rcEraser, clrWindow, clrWindow );
	if( _pcfd.m_bHover || _pcfd.m_bPushed )
	{
		COLORREF clrBorder = GetColor( XPCLR_HILIGHT_BORDER );
		dc.Draw3dRect( rcHoverFrame, clrBorder, clrBorder );
	}

CFont font;
int ppi = dc.GetDeviceCaps(LOGPIXELSX);
int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
	font.CreatePointFont(pointsize, _T("Marlett"));

PAINTPUSHBUTTONDATA _ppbd(
		this,
		true,
		rcButton,
		_T("6"),
		NULL,
		true,
		(_pcfd.m_bHover||_pcfd.m_bPushed) ? true : false,
		_pcfd.m_bPushed ? true : false,
		false,
		_pcfd.m_bEnabled,
		true,
		false,
		false,
		CExtPaintManager::__ALIGN_HORIZ_CENTER
			| CExtPaintManager::__ALIGN_VERT,
		(HFONT)font.GetSafeHandle(), // = NULL
		false,
		0,
		( _pcfd.m_bHover || _pcfd.m_bPushed ) ? false : true
		);
	PaintPushButton( dc, _ppbd );

	if( rgnClip.GetSafeHandle() != NULL )
		dc.SelectClipRgn( NULL );

	dc.RestoreDC( nSavedDC );
}

int CExtPaintManager::GetMenuBorderSize()
{
	return 4;
}

void CExtPaintManager::PaintMenuBorder(
	CDC & dc,
	const RECT & rectClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
	if( ::IsRectEmpty( &rectClient ) )
		return;
CRect rc(rectClient);
	dc.Draw3dRect(
		&rc,
		GetColor(CLR_3DFACE_OUT),
		GetColor(CLR_3DDKSHADOW_OUT)
		);
	rc.DeflateRect(1,1);
	dc.Draw3dRect(
		&rc,
		GetColor(CLR_3DHILIGHT_OUT),
		GetColor(CLR_3DSHADOW_OUT)
		);
COLORREF clrFace = GetColor(CLR_3DFACE_OUT);
	rc.DeflateRect(1,1);
	dc.Draw3dRect(&rc,clrFace,clrFace);
	rc.DeflateRect(1,1);
	dc.Draw3dRect(&rc,clrFace,clrFace);
}

int CExtPaintManager::GetMenuShadowSize()
{
	return 4; // 5; // (+ 2.23)
}

int CExtPaintManagerXP::GetMenuShadowSize()
{
	return 4; // 5; // (+ 2.23)
}

int CExtPaintManagerXP::GetMenuBorderSize()
{
	return 2;
}

void CExtPaintManagerXP::PaintMenuBorder(
	CDC & dc,
	const RECT & rectClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
	if( ::IsRectEmpty( &rectClient ) )
		return;
CRect rc(rectClient);
COLORREF clr = GetColor( XPCLR_MENU_BORDER );
	dc.Draw3dRect(&rc,clr,clr);
COLORREF clrFace = GetColor(XPCLR_3DFACE_NORMAL);
	rc.DeflateRect(1,1);
	dc.Draw3dRect(&rc,clrFace,clrFace);
	rc.right--;
	rc.bottom--;
	dc.Draw3dRect(&rc,clrFace,clrFace);
}

bool CExtPaintManager::IsMenuMustCombineExcludeArea()
{
//
// Implementadion of CExtPaintManager::PaintMenuCombinedArea()
// is provided, but office 98/2k does not combine its menu area
// with exclude area. interesting? uncomment and play with it!
//
//	return true; 
//
	return false;
}

bool CExtPaintManagerXP::IsMenuMustCombineExcludeArea()
{
	return true;
}

void CExtPaintManager::PaintMenuCombinedArea(
	CDC & dc,
	const RECT & rcArea,
	const RECT & rcClient,
	int eCombineAlign, // CExtPopupMenuWnd::e_combine_align_t values
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
	if( ::IsRectEmpty( &rcArea ) )
		return;
	dc.SelectClipRgn( NULL );
	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );
int nExpandSize =
		GetMenuBorderSize() - 1;
CRect _rcClient( rcClient );
CRect _rcArea( rcArea );
	_rcArea.InflateRect(
		(eCombineAlign==CExtPopupMenuWnd::__CMBA_RIGHT) ?
			nExpandSize : 0,
		(eCombineAlign==CExtPopupMenuWnd::__CMBA_BOTTOM) ?
			nExpandSize : 0,
		(eCombineAlign==CExtPopupMenuWnd::__CMBA_LEFT) ?
			nExpandSize : 0,
		(eCombineAlign==CExtPopupMenuWnd::__CMBA_TOP) ?
			nExpandSize : 0
		);
	dc.FillSolidRect(
		&_rcArea,
		GetColor( CLR_3DFACE_OUT )
		);
//	_rcArea = rcArea;

COLORREF clrMenuBorderLTo =
		GetColor( CLR_3DFACE_OUT );
COLORREF clrMenuBorderLTi =
		GetColor( CLR_3DHILIGHT_OUT );
COLORREF clrMenuBorderRBo =
		GetColor( CLR_3DSHADOW_OUT );
COLORREF clrMenuBorderRBi =
		GetColor( CLR_3DDKSHADOW_OUT );

	switch( eCombineAlign )
	{
	case CExtPopupMenuWnd::__CMBA_TOP:
		_rcArea.InflateRect( 0, 0, 0, nExpandSize );
		_rcClient.InflateRect( -nExpandSize, 0 );
	break;
	case CExtPopupMenuWnd::__CMBA_BOTTOM:
		_rcArea.InflateRect( 0, nExpandSize, 0, 0 );
		_rcClient.InflateRect( -nExpandSize, 0 );
	break;
	case CExtPopupMenuWnd::__CMBA_LEFT:
		_rcArea.InflateRect( 0, 0, nExpandSize, 0 );
		_rcClient.InflateRect( 0, -nExpandSize );
	break;
	case CExtPopupMenuWnd::__CMBA_RIGHT:
		_rcArea.InflateRect( nExpandSize, 0, 0, 0 );
		_rcClient.InflateRect( 0, -nExpandSize );
	break;
#ifdef _DEBUG
	default:
		ASSERT(FALSE);
	break;
#endif // _DEBUG
	} // switch( eCombineAlign )

	dc.ExcludeClipRect( &_rcClient );

int cx = _rcArea.Width();
int cy = _rcArea.Height();
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_BOTTOM )
		dc.FillSolidRect(_rcArea.left, _rcArea.top, cx - 1, 1, clrMenuBorderLTo);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_RIGHT )
		dc.FillSolidRect(_rcArea.left, _rcArea.top, 1, cy - 1, clrMenuBorderLTo);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_LEFT )
		dc.FillSolidRect(_rcArea.left + cx, _rcArea.top, -1, cy, clrMenuBorderRBo);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_TOP )
		dc.FillSolidRect(_rcArea.left, _rcArea.top + cy, cx, -1, clrMenuBorderRBo);
	_rcArea.DeflateRect(1,1,1,1);
	cx = _rcArea.Width();
	cy = _rcArea.Height();
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_BOTTOM )
		dc.FillSolidRect(_rcArea.left, _rcArea.top, cx - 1, 1, clrMenuBorderLTi);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_RIGHT )
		dc.FillSolidRect(_rcArea.left, _rcArea.top, 1, cy - 1, clrMenuBorderLTi);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_LEFT )
		dc.FillSolidRect(_rcArea.left + cx, _rcArea.top, -1, cy, clrMenuBorderRBi);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_TOP )
		dc.FillSolidRect(_rcArea.left, _rcArea.top + cy, cx, -1, clrMenuBorderRBi);
	
/*
	dc.ExcludeClipRect( &_rcClient );
	dc.Draw3dRect(
		&_rcArea,
		clrMenuBorderLTo,
		clrMenuBorderRBo
		);
	_rcArea.DeflateRect(1,1,1,1);
	dc.Draw3dRect(
		&_rcArea,
		clrMenuBorderLTi,
		clrMenuBorderRBi
		);
*/
	dc.SelectClipRgn( NULL );
}

void CExtPaintManagerXP::PaintMenuCombinedArea(
	CDC & dc,
	const RECT & rcArea,
	const RECT & rcClient,
	int eCombineAlign, // CExtPopupMenuWnd::e_combine_align_t values
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	pHelperSrc;
	lParam;
	if( ::IsRectEmpty( &rcArea ) )
		return;
	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );
CRect _rcArea( rcArea );
CRect _rcClient( rcClient );
	dc.FillSolidRect( &_rcArea, GetColor( XPCLR_3DFACE_DARK ) );
	switch( eCombineAlign )
	{
	case CExtPopupMenuWnd::__CMBA_TOP:
	case CExtPopupMenuWnd::__CMBA_BOTTOM:
		_rcClient.InflateRect( -1, 0 );
	break;
	case CExtPopupMenuWnd::__CMBA_LEFT:
	case CExtPopupMenuWnd::__CMBA_RIGHT:
		_rcClient.InflateRect( 0, -1 );
	break;
#ifdef _DEBUG
	default:
		ASSERT(FALSE);
	break;
#endif // _DEBUG
	} // switch( eCombineAlign )
	dc.ExcludeClipRect( &_rcClient );
COLORREF clrMenuBorder = GetColor( XPCLR_MENU_BORDER );
	dc.Draw3dRect( &_rcArea, clrMenuBorder, clrMenuBorder );
	dc.SelectClipRgn( NULL );
}

void CExtPaintManagerOffice2003::PaintMenuCombinedArea(
	CDC & dc,
	const RECT & rcArea,
	const RECT & rcClient,
	int eCombineAlign, // CExtPopupMenuWnd::e_combine_align_t values
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	if( ::IsRectEmpty( &rcArea ) )
		return;
	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );
bool bStyleOffice2003 = true;
CExtPopupMenuWnd * pPopup = NULL;
	if(		pHelperSrc == NULL
		||	(! pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) ) )
		||	m_bmpCtBarGradientHorz.GetSafeHandle() == NULL
		||	m_bmpCtBarGradientVert.GetSafeHandle() == NULL
		||	m_bmpDockerGradient.GetSafeHandle() == NULL
		||	stat_GetBPP() <= 8
		)
		bStyleOffice2003 = false;
	else
	{
		pPopup = STATIC_DOWNCAST( CExtPopupMenuWnd, pHelperSrc );
		ASSERT_VALID( pPopup );
	}
	if( !bStyleOffice2003 )
	{
		CExtPaintManagerXP::PaintMenuCombinedArea(
			dc,
			rcArea,
			rcClient,
			eCombineAlign,
			pHelperSrc,
			lParam
			);
		return;
	}
	ASSERT_VALID( pPopup );
CObject * pObjCoockie = pPopup->GetCoockieAsObject();
CExtToolControlBar * pToolBar = NULL;
int nTbbMenuTrackingBtnIdx = -1;
CExtBarButton * pTBB = NULL;
	if(		pObjCoockie != NULL
		&&	pObjCoockie->IsKindOf( RUNTIME_CLASS(CExtToolControlBar) )
		)
	{ // if menu coockie for combined content callback is a CExtToolControlBar
		pToolBar = STATIC_DOWNCAST( CExtToolControlBar, pObjCoockie );
		ASSERT_VALID( pToolBar );
		if(		pToolBar->m_bPresubclassDialogMode
			||	pToolBar->m_pDockSite == NULL
			)
		{
			CExtPaintManagerXP::PaintMenuCombinedArea(
				dc,
				rcArea,
				rcClient,
				eCombineAlign,
				pHelperSrc,
				lParam
				);
			return;
		}
		nTbbMenuTrackingBtnIdx = pToolBar->GetMenuTrackingButton();
		if( nTbbMenuTrackingBtnIdx >= 0 )
		{
			pTBB = pToolBar->GetButton(nTbbMenuTrackingBtnIdx);
			ASSERT_VALID( pTBB );
		} // if( nTbbMenuTrackingBtnIdx >= 0 )
	} // if menu coockie for combined content callback is a CExtToolControlBar

	if(		pToolBar != NULL && pTBB != NULL
		&&	pTBB->IsKindOf( RUNTIME_CLASS(CExtBarContentExpandButton) )
		&&	(! pToolBar->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) ) )
		)
	{ // if toolbar content expand button
		if(	(!(	g_PaintManager.m_bUxApiInitPassed
				&&	g_PaintManager.m_bUxValidColorsExtracted
				&&	g_PaintManager.m_bUxUseIfAvailOnWinXpOrLater
			))
			)
		{
			COLORREF clrFillUpdate =
				GetColor( _2003CLR_EXPBTN_PRESSED_LIGHT );
			switch( eCombineAlign )
			{
			case CExtPopupMenuWnd::__CMBA_TOP:
				dc.FillSolidRect(
					rcArea.right - m_sizeToolBarRgnRounders.cx - 2,
					rcArea.bottom - m_sizeToolBarRgnRounders.cy - 1,
					m_sizeToolBarRgnRounders.cx + 2,
					m_sizeToolBarRgnRounders.cy,
					clrFillUpdate
					);
			break;
			case CExtPopupMenuWnd::__CMBA_BOTTOM:
				dc.FillSolidRect(
					rcArea.right - m_sizeToolBarRgnRounders.cx - 2,
					rcArea.top + 1,
					m_sizeToolBarRgnRounders.cx + 2,
					m_sizeToolBarRgnRounders.cy,
					clrFillUpdate
					);
				dc.FillSolidRect(
					rcArea.right - m_sizeToolBarRgnRounders.cx,
					rcArea.bottom - m_sizeToolBarRgnRounders.cy - 1,
					m_sizeToolBarRgnRounders.cx,
					m_sizeToolBarRgnRounders.cy,
					clrFillUpdate
					);
			break;
			case CExtPopupMenuWnd::__CMBA_LEFT:
				dc.FillSolidRect(
					rcArea.right - m_sizeToolBarRgnRounders.cx - 2,
					rcArea.bottom - m_sizeToolBarRgnRounders.cy,
					m_sizeToolBarRgnRounders.cx + 1,
					m_sizeToolBarRgnRounders.cy,
					clrFillUpdate
					);
			break;
			case CExtPopupMenuWnd::__CMBA_RIGHT:
				dc.FillSolidRect(
					rcArea.right - m_sizeToolBarRgnRounders.cx,
					rcArea.bottom - m_sizeToolBarRgnRounders.cy - 1,
					m_sizeToolBarRgnRounders.cx,
					m_sizeToolBarRgnRounders.cy,
					clrFillUpdate
					);
			break;
#ifdef _DEBUG
			default:
				ASSERT(FALSE);
			break;
#endif // _DEBUG
			} // switch( eCombineAlign )
		}
		return;
	} // if toolbar content expand button

CRect _rcArea( rcArea );
CRect _rcClient( rcClient );
	switch( eCombineAlign )
	{
	case CExtPopupMenuWnd::__CMBA_TOP:
	case CExtPopupMenuWnd::__CMBA_BOTTOM:
		_rcClient.InflateRect( -1, 0 );
	break;
	case CExtPopupMenuWnd::__CMBA_LEFT:
	case CExtPopupMenuWnd::__CMBA_RIGHT:
		_rcClient.InflateRect( 0, -1 );
	break;
#ifdef _DEBUG
	default:
		ASSERT(FALSE);
	break;
#endif // _DEBUG
	} // switch( eCombineAlign )
bool b2003StyleMiddleBorder = true;
bool bNoGradientOverCombineArea = false;
	if(		pToolBar != NULL && pTBB != NULL
		&&	(	pToolBar->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) )
			||	(	pTBB->GetIconPtr() != NULL
				&&	(! pTBB->GetIconPtr()->IsEmpty() )
				)
			)
		)
		b2003StyleMiddleBorder = false;
	else
	{
		if( pPopup->IsToolBtnCombinedArea() )
			bNoGradientOverCombineArea = true;
		else
			b2003StyleMiddleBorder = false;
	}

bool bPreEmptyClipRgn = false;
COLORREF clrBottomFill =
		GetColor(
			bNoGradientOverCombineArea
				? XPCLR_3DFACE_DARK
				: XPCLR_3DFACE_NORMAL
			);
	if( !b2003StyleMiddleBorder )
	{
		dc.FillSolidRect( &_rcArea, clrBottomFill );
		dc.ExcludeClipRect( &_rcClient );
	} // if( !b2003StyleMiddleBorder )
	else
	{
		if( eCombineAlign == CExtPopupMenuWnd::__CMBA_LEFT )
			_rcArea.InflateRect( 0, 0, -1, 0 );
		dc.ExcludeClipRect( &_rcClient );
		dc.FillSolidRect( &_rcArea, clrBottomFill );
		bPreEmptyClipRgn = true;
	} // else from if( !b2003StyleMiddleBorder )

	if( !bNoGradientOverCombineArea )
	{
		CDC & dcCompat = CachedDcGet( m_bmpCtBarGradientHorz );
		if( dcCompat.GetSafeHdc() != NULL )
		{
			CSize sizeShade = m_sizeCtBarGradientHorz;
			int nOldStretchBltMode =
				dc.SetStretchBltMode(
					g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
					);
			::SetBrushOrgEx( dc, 0, 0, NULL );
			dc.StretchBlt( // (/2) makes area lighter and more smoth fill
				_rcArea.left,
				_rcArea.top,
				_rcArea.Width(),
				_rcArea.Height(),
				&dcCompat,
				0,
				0,
				sizeShade.cx/2,
				sizeShade.cy/2,
				SRCCOPY
				);
			dc.SetStretchBltMode( nOldStretchBltMode );
		} // if( dcCompat.GetSafeHdc() != NULL )
#ifdef _DEBUG
		else
		{
			ASSERT( FALSE );
		} // else from if( dcCompat.GetSafeHdc() != NULL )
#endif // _DEBUG
	} // if( !bNoGradientOverCombineArea )

	if( bPreEmptyClipRgn )
		dc.SelectClipRgn( NULL );
COLORREF clrMenuBorder = GetColor( XPCLR_MENU_BORDER );
	dc.Draw3dRect( &_rcArea, clrMenuBorder, clrMenuBorder );
	dc.SelectClipRgn( NULL );
}

CExtPaintManager::PAINTCONTROLBARBORDERSDATA::PAINTCONTROLBARBORDERSDATA()
	: m_pHelperSrc( NULL )
	, m_lParam( 0L )
	, m_eCBBT( CExtPaintManager::e_control_bar_border_type_t::__CB_OUTER_DOCKBAR )
	, m_dwStyle( 0L )
	, m_rc( 0,0,0,0 )
{
}

CExtPaintManager::PAINTCONTROLBARBORDERSDATA::PAINTCONTROLBARBORDERSDATA(
	CObject * pHelperSrc,
	int eCBBT, // CExtPaintManager::e_control_bar_border_type_t
	DWORD dwStyle,
	const RECT & rc
	)
	: m_pHelperSrc( pHelperSrc )
	, m_lParam( 0L )
	, m_eCBBT( eCBBT )
	, m_dwStyle( dwStyle )
	, m_rc( rc )
{
}

void CExtPaintManager::PaintControlBarBordersDefImpl(
	CDC & dc,
	CExtPaintManager::PAINTCONTROLBARBORDERSDATA & _pcbbd,
	COLORREF clrHi,
	COLORREF clrLo
	)
{
	if( !(_pcbbd.m_dwStyle & CBRS_BORDER_ANY) )
		return;

	// prepare for dark lines
//	ASSERT( _pcbbd.m_rc.top == 0 && _pcbbd.m_rc.left == 0 );
CRect rc1( _pcbbd.m_rc ), rc2( _pcbbd.m_rc );
COLORREF clr = clrLo;
//		afxData.bWin4
//			? afxData.clrBtnShadow
//			: afxData.clrWindowFrame;
		GetColor(
			CExtPaintManager::CLR_3DFACE_OUT
			);

	// draw dark line one pixel back/up
	if( _pcbbd.m_dwStyle & CBRS_BORDER_3D )
	{
		rc1.right -= CX_BORDER;
		rc1.bottom -= CY_BORDER;
	}
	if( _pcbbd.m_dwStyle & CBRS_BORDER_TOP )
		rc2.top += afxData.cyBorder2;
	if( _pcbbd.m_dwStyle & CBRS_BORDER_BOTTOM )
		rc2.bottom -= afxData.cyBorder2;

	// draw left and top
	if( _pcbbd.m_dwStyle & CBRS_BORDER_LEFT )
		dc.FillSolidRect(
			0, rc2.top, CX_BORDER, rc2.Height(),
			clr
			);
	if( _pcbbd.m_dwStyle & CBRS_BORDER_TOP )
		dc.FillSolidRect(
			0, 0, _pcbbd.m_rc.right, CY_BORDER,
			clr
			);

	// draw right and bottom
	if( _pcbbd.m_dwStyle & CBRS_BORDER_RIGHT )
		dc.FillSolidRect(
			rc1.right, rc2.top, -CX_BORDER, rc2.Height(),
			clr
			);
	if( _pcbbd.m_dwStyle & CBRS_BORDER_BOTTOM )
		dc.FillSolidRect(
			0, rc1.bottom, _pcbbd.m_rc.right, -CY_BORDER,
			clr
			);

	if( _pcbbd.m_dwStyle & CBRS_BORDER_3D )
	{
		// prepare for hilite lines
//		clr = afxData.clrBtnHilite;
		clr = clrHi;

		// draw left and top
		if( _pcbbd.m_dwStyle & CBRS_BORDER_LEFT )
			dc.FillSolidRect(
				1, rc2.top, CX_BORDER, rc2.Height(),
				clr
				);
		if( _pcbbd.m_dwStyle & CBRS_BORDER_TOP )
			dc.FillSolidRect(
				0, 1, _pcbbd.m_rc.right, CY_BORDER,
				clr
				);

		// draw right and bottom
		if( _pcbbd.m_dwStyle & CBRS_BORDER_RIGHT )
			dc.FillSolidRect(
				_pcbbd.m_rc.right, rc2.top, -CX_BORDER, rc2.Height(),
				clr
				);
		if( _pcbbd.m_dwStyle & CBRS_BORDER_BOTTOM )
			dc.FillSolidRect(
				0, _pcbbd.m_rc.bottom, _pcbbd.m_rc.right, -CY_BORDER,
				clr
				);
	}

//	if( _pcbbd.m_dwStyle & CBRS_BORDER_LEFT )
//		_pcbbd.m_rc.left += afxData.cxBorder2;
//	if( _pcbbd.m_dwStyle & CBRS_BORDER_TOP )
//		_pcbbd.m_rc.top += afxData.cyBorder2;
//	if( _pcbbd.m_dwStyle & CBRS_BORDER_RIGHT )
//		_pcbbd.m_rc.right -= afxData.cxBorder2;
//	if( _pcbbd.m_dwStyle & CBRS_BORDER_BOTTOM )
//		_pcbbd.m_rc.bottom -= afxData.cyBorder2;
}

void CExtPaintManager::PaintControlBarBorders(
	CDC & dc,
	CExtPaintManager::PAINTCONTROLBARBORDERSDATA & _pcbbd
	)
{
	if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )
	{
		dc.Draw3dRect(
			&_pcbbd.m_rc,
			GetColor( CLR_3DSHADOW_OUT ),
			GetColor( CLR_3DHILIGHT_OUT )
			);
		return;
	} // if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )
COLORREF clr = GetColor( CLR_3DFACE_OUT );
	PaintControlBarBordersDefImpl(
		dc, _pcbbd, clr, clr
		);
}

void CExtPaintManagerXP::PaintControlBarBorders(
	CDC & dc,
	CExtPaintManager::PAINTCONTROLBARBORDERSDATA & _pcbbd
	)
{
	if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )
	{
		COLORREF clr = GetColor( COLOR_3DSHADOW );
		dc.Draw3dRect( &_pcbbd.m_rc, clr, clr );
		return;
	} // if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )
COLORREF clr = GetColor( CLR_3DFACE_OUT );
	PaintControlBarBordersDefImpl(
		dc, _pcbbd, clr, clr
		);
}

void CExtPaintManagerOffice2003::PaintControlBarBorders(
	CDC & dc,
	CExtPaintManager::PAINTCONTROLBARBORDERSDATA & _pcbbd
	)
{
	if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )
	{
		COLORREF clr = GetColor( _2003CLR_STATUSBAR_ITEM );
		dc.Draw3dRect( &_pcbbd.m_rc, clr, clr );
		return;
	} // if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )
	CExtPaintManagerXP::PaintControlBarBorders( dc, _pcbbd );
}

void CExtPaintManager::PaintResizableBarChildNcAreaRect(
	CDC & dc,
	const RECT & rc,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	PaintTabNcAreaRect(
		dc,
		rc,
		pHelperSrc,
		lParam
		);
}

void CExtPaintManagerXP::PaintResizableBarChildNcAreaRect(
	CDC & dc,
	const RECT & rc,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	CExtPaintManager::PaintResizableBarChildNcAreaRect(
		dc,
		rc,
		pHelperSrc,
		lParam
		);
}

HICON CExtPaintManager::stat_GenerateColorIconBox(
	COLORREF clrIcon
	)
{
CWindowDC dc( NULL );

ICONINFO iiNewIcon;
	::memset( &iiNewIcon, 0, sizeof(ICONINFO) );
	iiNewIcon.fIcon = true;
HDC hIconDC = ::CreateCompatibleDC( dc.GetSafeHdc() );
HDC hMaskDC = ::CreateCompatibleDC( dc.GetSafeHdc() );
	ASSERT(
		hIconDC != NULL && hMaskDC != NULL
		);
	iiNewIcon.hbmColor = ::CreateCompatibleBitmap(
		dc.GetSafeHdc(),
		16,
		16
		);
	ASSERT( iiNewIcon.hbmColor != NULL );
	iiNewIcon.hbmMask = ::CreateCompatibleBitmap(
		hMaskDC,
		16,
		16
		);
	ASSERT( iiNewIcon.hbmMask != NULL );
HGDIOBJ hOldIconDC =
		::SelectObject(
			hIconDC,
			iiNewIcon.hbmColor
			);
	ASSERT( hOldIconDC != NULL );
HGDIOBJ hOldMaskDC =
		::SelectObject(
			hMaskDC,
			iiNewIcon.hbmMask
			);
	ASSERT( hOldMaskDC != NULL );

CBrush brColorItem( clrIcon );
CRect rcColorItem(0,0,16,16);
	::FillRect(
		hIconDC,
		&rcColorItem,
		(HBRUSH)brColorItem.GetSafeHandle()
		);
	::FillRect(
		hMaskDC,
		&rcColorItem,
		(HBRUSH)::GetStockObject(BLACK_BRUSH)
		);

	::SelectObject(hIconDC,hOldIconDC);
	::SelectObject(hMaskDC,hOldMaskDC);
HICON hIcon = ::CreateIconIndirect(&iiNewIcon);
	ASSERT( hIcon != NULL );
	::DeleteObject(iiNewIcon.hbmColor);
	::DeleteObject(iiNewIcon.hbmMask);
	::DeleteDC(hMaskDC);
	::DeleteDC(hIconDC);

	return hIcon;
}

HICON CExtPaintManager::stat_GenerateColorIconFrame(
	COLORREF clrIcon
	)
{
CWindowDC dc( NULL );

ICONINFO iiNewIcon;
	iiNewIcon.fIcon = true;
HDC hIconDC = ::CreateCompatibleDC( dc.GetSafeHdc() );
HDC hMaskDC = ::CreateCompatibleDC( dc.GetSafeHdc() );
	ASSERT(
		hIconDC != NULL && hMaskDC != NULL
		);
	iiNewIcon.hbmColor = ::CreateCompatibleBitmap(
		dc.GetSafeHdc(),
		16,
		16
		);
	ASSERT( iiNewIcon.hbmColor != NULL );
	iiNewIcon.hbmMask = ::CreateCompatibleBitmap(
		hMaskDC,
		16,
		16
		);
	ASSERT( iiNewIcon.hbmMask != NULL );
HGDIOBJ hOldIconDC =
		::SelectObject(
			hIconDC,
			iiNewIcon.hbmColor
			);
	ASSERT( hOldIconDC != NULL );
HGDIOBJ hOldMaskDC =
		::SelectObject(
			hMaskDC,
			iiNewIcon.hbmMask
			);
	ASSERT( hOldMaskDC != NULL );

CBrush brColorItem( clrIcon );
CRect rcColorItem(0,0,16,16);
	::FillRect(
		hMaskDC,
		&rcColorItem,
		(HBRUSH)::GetStockObject(WHITE_BRUSH)
		);
	rcColorItem.DeflateRect( 1, 2 );
	::FillRect(
		hMaskDC,
		&rcColorItem,
		(HBRUSH)::GetStockObject(BLACK_BRUSH)
		);
	::FillRect(
		hIconDC,
		&rcColorItem,
		(HBRUSH)brColorItem.GetSafeHandle()
		);
	rcColorItem.DeflateRect( 2, 2 );
	::FillRect(
		hMaskDC,
		&rcColorItem,
		(HBRUSH)::GetStockObject(WHITE_BRUSH)
		);
	::FillRect(
		hIconDC,
		&rcColorItem,
		(HBRUSH)::GetStockObject(BLACK_BRUSH)
		);

	::SelectObject(hIconDC,hOldIconDC);
	::SelectObject(hMaskDC,hOldMaskDC);
HICON hIcon = ::CreateIconIndirect(&iiNewIcon);
	ASSERT( hIcon != NULL );
	::DeleteObject(iiNewIcon.hbmColor);
	::DeleteObject(iiNewIcon.hbmMask);
	::DeleteDC(hMaskDC);
	::DeleteDC(hIconDC);

	return hIcon;
}

HICON CExtPaintManager::stat_GenerateColorIconChar(
	COLORREF clrIcon,
	HFONT hFontForLetter,
	__EXT_MFC_SAFE_TCHAR tChr // = _T('A')
	)
{
	ASSERT( tChr != _T('\0') );
	ASSERT( hFontForLetter != NULL );
CWindowDC dc( NULL );

ICONINFO iiNewIcon;
	iiNewIcon.fIcon = true;
HDC hIconDC = ::CreateCompatibleDC( dc.GetSafeHdc() );
HDC hMaskDC = ::CreateCompatibleDC( dc.GetSafeHdc() );
	ASSERT(
		hIconDC != NULL && hMaskDC != NULL
		);
	iiNewIcon.hbmColor = ::CreateCompatibleBitmap(
		dc.GetSafeHdc(),
		16,
		16
		);
	ASSERT( iiNewIcon.hbmColor != NULL );
	iiNewIcon.hbmMask = ::CreateCompatibleBitmap(
		hMaskDC,
		16,
		16
		);
	ASSERT( iiNewIcon.hbmMask != NULL );
HGDIOBJ hOldIconDC =
		::SelectObject(
			hIconDC,
			iiNewIcon.hbmColor
			);
	ASSERT( hOldIconDC != NULL );
HGDIOBJ hOldMaskDC =
		::SelectObject(
			hMaskDC,
			iiNewIcon.hbmMask
			);
	ASSERT( hOldMaskDC != NULL );

CRect rcColorItem(0,0,16,16);
	::FillRect(
		hMaskDC,
		&rcColorItem,
		(HBRUSH)::GetStockObject(WHITE_BRUSH)
		);

	ASSERT( hFontForLetter != NULL );

HGDIOBJ hOldFontIconDC = ::SelectObject( hIconDC, hFontForLetter );
HGDIOBJ hOldFontMaskDC = ::SelectObject( hMaskDC, hFontForLetter );

CRect rcText( rcColorItem );
	rcText.bottom -= 4;
CExtSafeString sText( (TCHAR)tChr, 1 );

	::SetTextColor( hIconDC, RGB(0,0,0) );
	::SetBkMode( hIconDC, TRANSPARENT );
	::DrawText(
		hIconDC,
		sText,
		sText.GetLength(),
		&rcText,
		DT_SINGLELINE|DT_CENTER|DT_VCENTER
		);
	
	::SetTextColor( hMaskDC, RGB(0,0,0) );
	::SetBkMode( hMaskDC, TRANSPARENT );
	::DrawText(
		hMaskDC,
		sText,
		sText.GetLength(),
		&rcText,
		DT_SINGLELINE|DT_CENTER|DT_VCENTER
		);

	::SelectObject( hIconDC, hOldFontIconDC );
	::SelectObject( hMaskDC, hOldFontMaskDC );

CRect rcColorBar( rcText );
	rcColorBar.OffsetRect( 0, rcColorBar.Height() + 1 );
	rcColorBar.bottom = rcColorBar.top + 3;
	rcColorBar.DeflateRect( 2, 0 );

CBrush brColor( clrIcon );
	::FillRect(
		hIconDC,
		&rcColorBar,
		(HBRUSH)brColor.GetSafeHandle()
		);
	::FillRect(
		hMaskDC,
		&rcColorBar,
		(HBRUSH)::GetStockObject(BLACK_BRUSH)
		);

	::SelectObject(hIconDC,hOldIconDC);
	::SelectObject(hMaskDC,hOldMaskDC);
HICON hIcon = ::CreateIconIndirect(&iiNewIcon);
	ASSERT( hIcon != NULL );
	::DeleteObject(iiNewIcon.hbmColor);
	::DeleteObject(iiNewIcon.hbmMask);
	::DeleteDC(hMaskDC);
	::DeleteDC(hIconDC);
	return hIcon;
}

#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)

void CExtPaintManager::PaintShortcutListBkgnd(
	CDC & dc,
	const RECT & rcClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;

	if(		rcClient.left >= rcClient.right
		||	rcClient.top >= rcClient.bottom
		||	(! dc.RectVisible( &rcClient ) )
		)
		return;

COLORREF clrBk = GetColor( COLOR_3DSHADOW );
	dc.FillSolidRect(
		&rcClient,
		clrBk
		);
}

void CExtPaintManagerXP::PaintShortcutListBkgnd(
	CDC & dc,
	const RECT & rcClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;

	if(		rcClient.left >= rcClient.right
		||	rcClient.top >= rcClient.bottom
		||	(! dc.RectVisible( &rcClient ) )
		)
		return;

COLORREF clrBk = GetColor( CLR_3DFACE_OUT );
	dc.FillSolidRect(
		&rcClient,
		clrBk
		);
}

void CExtPaintManager::PaintShortcutListScrollButton(
	CDC & dc,
	bool bHorzLayout,
	const RECT & rcButton,
	bool bUpOrLeftBtn,
	bool bHover,
	bool bPressed,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;

	if(		::IsRectEmpty( &rcButton )
		||	( ! dc.RectVisible( &rcButton ) )
		)
		return;

COLORREF clr3dFace = GetColor( COLOR_3DFACE );
	dc.FillSolidRect( &rcButton, clr3dFace );
CRect rcGlyph( rcButton );
COLORREF clrOuterLT = clr3dFace;
COLORREF clrOuterRB = clr3dFace;
COLORREF clrInnerLT = clr3dFace;
COLORREF clrInnerRB = clr3dFace;
	if( bPressed )
	{
		rcGlyph.OffsetRect( 1, 1 );
		clrOuterLT = GetColor( COLOR_3DDKSHADOW );
		clrOuterRB = GetColor( COLOR_3DHILIGHT );
		clrInnerLT = GetColor( COLOR_3DSHADOW );
	} // if( bPressed )
	else
	{
		if( bHover )
		{
			clrOuterRB = GetColor( COLOR_3DDKSHADOW );
			clrInnerLT = GetColor( COLOR_3DHILIGHT );
			clrInnerRB = GetColor( COLOR_3DSHADOW );
		} // if( bHover )
		else
		{
			clrOuterLT = GetColor( COLOR_3DHILIGHT );
			clrOuterRB = GetColor( COLOR_3DSHADOW );
		} // else from if( bHover )
	} // else from if( bPressed )

COLORREF ColorValues[2] =
{
	RGB(0,0,0),
	GetColor(COLOR_BTNTEXT)
};
	PaintGlyphCentered(
		dc,
		rcGlyph,
		* g_DockingCaptionGlyphs[
			bHorzLayout
				? ( bUpOrLeftBtn
						? __DCBT_ARROW_LEFT
						: __DCBT_ARROW_RIGHT
						)
				: ( bUpOrLeftBtn
						? __DCBT_ARROW_UP
						: __DCBT_ARROW_DOWN
						)
			],
		ColorValues
		);
	dc.Draw3dRect(
		&rcButton,
		clrOuterLT,
		clrOuterRB
		);
CRect rcButton2(rcButton);
	rcButton2.DeflateRect( 1, 1 );
	dc.Draw3dRect(
		&rcButton2,
		clrInnerLT,
		clrInnerRB
		);
}

void CExtPaintManagerXP::PaintShortcutListScrollButton(
	CDC & dc,
	bool bHorzLayout,
	const RECT & rcButton,
	bool bUpOrLeftBtn,
	bool bHover,
	bool bPressed,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	CExtPaintManager::PaintShortcutListScrollButton(
		dc,
		bHorzLayout,
		rcButton,
		bUpOrLeftBtn,
		bHover,
		bPressed,
		pHelperSrc,
		lParam
		);
}

CExtPaintManager::PAINTSHORTCUTLISTITEMSDATA::PAINTSHORTCUTLISTITEMSDATA()
	: m_pHelperSrc( NULL )
	, m_lParam( 0L )
	, m_dwShortcutListStyle( 0L )
	, m_hItemTextFont( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) )
	, m_pIconBig( NULL )
	, m_pIconSmall( NULL )
	, m_lParamItem( 0L )
	, m_rcBoundsAll( 0,0,0,0 )
	, m_rcBoundsIcon( 0,0,0,0 )
	, m_rcBoundsText( 0,0,0,0 )
	, m_rcCtrlClient( 0,0,0,0 )
	, m_sText( _T("") )
	, m_bHover( false )
	, m_bPressed( false )
	, m_bVisible( false )
	, m_bChecked( false )
	, m_bExpandMode( false )
	, m_bHorzLayout( false )
	, m_bBigIconView( false )
	, m_bEraseQuery( false )
{
	ASSERT( m_hItemTextFont != NULL );
}

CExtPaintManager::PAINTSHORTCUTLISTITEMSDATA::PAINTSHORTCUTLISTITEMSDATA(
	CObject * pHelperSrc,
	DWORD dwShortcutListStyle,
	HFONT hItemTextFont,
	CExtCmdIcon * pIconBig,
	CExtCmdIcon * pIconSmall,
	LPARAM lParamItem,
	const RECT & rcBoundsAll,
	const RECT & rcBoundsIcon,
	const RECT & rcBoundsText,
	const RECT & rcCtrlClient,
	__EXT_MFC_SAFE_LPCTSTR sText,
	bool bHover,
	bool bPressed,
	bool bVisible,
	bool bChecked,
	bool bExpandMode,
	bool bHorzLayout,
	bool bBigIconView,
	bool bEraseQuery
	)
	: m_pHelperSrc( pHelperSrc )
	, m_lParam( 0L )
	, m_dwShortcutListStyle( dwShortcutListStyle )
	, m_hItemTextFont( hItemTextFont )
	, m_pIconBig( pIconBig )
	, m_pIconSmall( pIconSmall )
	, m_lParamItem( lParamItem )
	, m_rcBoundsAll( rcBoundsAll )
	, m_rcBoundsIcon( rcBoundsIcon )
	, m_rcBoundsText( rcBoundsText )
	, m_rcCtrlClient( rcCtrlClient )
	, m_sText( (sText == NULL) ? _T("") : sText )
	, m_bHover( bHover )
	, m_bPressed( bPressed )
	, m_bVisible( bVisible )
	, m_bChecked( bChecked )
	, m_bExpandMode( bExpandMode )
	, m_bHorzLayout( bHorzLayout )
	, m_bBigIconView( bBigIconView )
	, m_bEraseQuery( bEraseQuery )
{
	ASSERT( m_hItemTextFont != NULL );
}

void CExtPaintManager::PaintShortcutListItem(
	CDC & dc,
	CExtPaintManager::PAINTSHORTCUTLISTITEMSDATA & _pslid
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );

	if( _pslid.m_bEraseQuery )
	{
		if( !_pslid.m_bExpandMode )
			return;
		if(		_pslid.m_rcBoundsText.left < _pslid.m_rcBoundsText.right
			&&	_pslid.m_rcBoundsText.top < _pslid.m_rcBoundsText.bottom
			&&	dc.RectVisible( &_pslid.m_rcBoundsText )
			)
		{
			CRect rcExpandErase( _pslid.m_rcBoundsText );
			rcExpandErase.InflateRect(
				__EXT_SHORTCUTLISTWND_EXPANDED_TEXT_GAP_X,
				__EXT_SHORTCUTLISTWND_EXPANDED_TEXT_GAP_Y
				);
			COLORREF clrDarkBorder = GetColor( COLOR_3DDKSHADOW );
			COLORREF clrInfoBk = GetColor( COLOR_INFOBK );
			dc.FillSolidRect( &rcExpandErase, clrInfoBk );
			dc.Draw3dRect( &rcExpandErase, clrDarkBorder, clrDarkBorder );
		}
		return;
	} // if( _pslid.m_bEraseQuery )


	if( _pslid.m_rcBoundsAll.IsRectEmpty()
		||	( ! dc.RectVisible( &_pslid.m_rcBoundsAll ) )
		)
		return;

	if( (! _pslid.m_rcBoundsIcon.IsRectEmpty() )
		&& dc.RectVisible( &_pslid.m_rcBoundsIcon )
		)
	{
		CExtCmdIcon * pIcon = _pslid.m_bBigIconView
			? _pslid.m_pIconBig
			: _pslid.m_pIconSmall
			;
		if( pIcon == NULL || pIcon->IsEmpty() )
		{
			pIcon = _pslid.m_pIconBig;
			if( pIcon == NULL || pIcon->IsEmpty() )
				pIcon = _pslid.m_pIconSmall;
		} // if( pIcon == NULL || pIcon->IsEmpty() )
		CRect rcUnderIcon( _pslid.m_rcBoundsIcon );
		rcUnderIcon.InflateRect(
			__EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_X,
			__EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_Y
			);
		if( _pslid.m_bPressed || _pslid.m_bHover )
			dc.FillSolidRect(
				&rcUnderIcon,
				GetColor( COLOR_3DSHADOW )
				);
		if( pIcon != NULL && (! pIcon->IsEmpty()) )
		{
			::DrawIconEx(
				dc.GetSafeHdc(),
				_pslid.m_rcBoundsIcon.left,
				_pslid.m_rcBoundsIcon.top,
				pIcon->GetIcon(),
				_pslid.m_rcBoundsIcon.Width(),
				_pslid.m_rcBoundsIcon.Height(),
				0,
				(HBRUSH)NULL,
				DI_NORMAL
				);
		} // if( pIcon != NULL && (! pIcon->IsEmpty()) )
		if( _pslid.m_bPressed || _pslid.m_bHover )
		{
			COLORREF clrLT =
				GetColor(
					_pslid.m_bPressed ? COLOR_3DDKSHADOW : COLOR_3DFACE
					);
			COLORREF clrRB =
				GetColor(
					_pslid.m_bPressed ? COLOR_3DFACE : COLOR_3DDKSHADOW
					);
			dc.Draw3dRect(
				&rcUnderIcon,
				clrLT,
				clrRB
				);
		}
	}

CExtSafeString sItemText = _pslid.m_sText;
	if( (! _pslid.m_rcBoundsText.IsRectEmpty() )
		&& dc.RectVisible( &_pslid.m_rcBoundsText )
		&& ( ! sItemText.IsEmpty() )
		)
	{
		LONG nItemTextLen = sItemText.GetLength();
		CRect rcCaption( _pslid.m_rcBoundsText );
		ASSERT( _pslid.m_hItemTextFont != NULL );
		CFont * pItemFont = CFont::FromHandle( _pslid.m_hItemTextFont );
		ASSERT( pItemFont != NULL );
		ASSERT( pItemFont->GetSafeHandle() != NULL );
		CFont * pOldFont = dc.SelectObject( pItemFont );
		COLORREF clrTextOld =
			dc.SetTextColor(
				GetColor(
					_pslid.m_bExpandMode ? COLOR_INFOTEXT : COLOR_3DHILIGHT
					//_pslid.m_bEnabled ? COLOR_BTNTEXT : COLOR_3DHILIGHT
					)
				);
		int nOldBkMode = dc.SetBkMode( TRANSPARENT );
		if( _pslid.m_bHorzLayout )
		{
			rcCaption.InflateRect( 3, 0 );
			CSize sizeTextMeasured = _pslid.m_rcBoundsText.Size();
			UINT nOldTA = dc.SetTextAlign(
				TA_TOP | TA_BASELINE
				);
			rcCaption.OffsetRect(
				( (_pslid.m_dwShortcutListStyle & __ESLW_INV_VFONT) == 0 )
					?   sizeTextMeasured.cx/2
					: - sizeTextMeasured.cx/2 + 2
					,
				0
				);
			CPoint ptCenter = rcCaption.CenterPoint();
			if( (_pslid.m_dwShortcutListStyle & __ESLW_INV_VFONT) == 0 )
				ptCenter.y =
					rcCaption.bottom - 4
					- (rcCaption.Height() - sizeTextMeasured.cy)
					;
			else
				ptCenter.y =
					rcCaption.top + 4
					;
			dc.ExtTextOut(
				ptCenter.x,
				ptCenter.y,
				ETO_CLIPPED,
				&rcCaption,
				sItemText,
				sItemText.GetLength(),
				NULL
				);
			dc.SetTextAlign( nOldTA );
		} // if( _pslid.m_bHorzLayout )
		else
		{
			CRect rcClient( _pslid.m_rcCtrlClient );
			rcClient.DeflateRect( 1, 0 );
			CRect rcText(
				max(rcCaption.left, rcClient.left),
				rcCaption.top,
				min(rcCaption.right, rcClient.right),
				rcCaption.bottom
				);
			if( rcText.left < rcText.right )
			{
				UINT nFormat =
					DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
				dc.DrawText(
					sItemText,
					nItemTextLen,
					&rcText,
					nFormat
					);
			} // if( rcText.left < rcText.right )
		} // else from if( _pslid.m_bHorzLayout )
		dc.SetBkMode( nOldBkMode );
		dc.SetTextColor( clrTextOld );
		dc.SelectObject( pOldFont );
	}

}

void CExtPaintManagerXP::PaintShortcutListItem(
	CDC & dc,
	CExtPaintManager::PAINTSHORTCUTLISTITEMSDATA & _pslid
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );

	if( _pslid.m_bEraseQuery )
	{
		if( !_pslid.m_bExpandMode )
			return;
		if(		_pslid.m_rcBoundsText.left < _pslid.m_rcBoundsText.right
			&&	_pslid.m_rcBoundsText.top < _pslid.m_rcBoundsText.bottom
			&&	dc.RectVisible( &_pslid.m_rcBoundsText )
			)
		{
			CRect rcExpandErase( _pslid.m_rcBoundsText );
			rcExpandErase.InflateRect(
				__EXT_SHORTCUTLISTWND_EXPANDED_TEXT_GAP_X,
				__EXT_SHORTCUTLISTWND_EXPANDED_TEXT_GAP_Y
				);
			COLORREF clrDarkBorder = GetColor( COLOR_3DDKSHADOW );
			COLORREF clrInfoBk = GetColor( COLOR_INFOBK );
			dc.FillSolidRect( &rcExpandErase, clrInfoBk );
			dc.Draw3dRect( &rcExpandErase, clrDarkBorder, clrDarkBorder );
		}
		return;
	} // if( _pslid.m_bEraseQuery )


	if( _pslid.m_rcBoundsAll.IsRectEmpty()
		||	( ! dc.RectVisible( &_pslid.m_rcBoundsAll ) )
		)
		return;

	if( (! _pslid.m_rcBoundsIcon.IsRectEmpty() )
		&& dc.RectVisible( &_pslid.m_rcBoundsIcon )
		)
	{
		CExtCmdIcon * pIcon = _pslid.m_bBigIconView
			? _pslid.m_pIconBig
			: _pslid.m_pIconSmall
			;
		if( pIcon == NULL || pIcon->IsEmpty() )
		{
			pIcon = _pslid.m_pIconBig;
			if( pIcon == NULL || pIcon->IsEmpty() )
				pIcon = _pslid.m_pIconSmall;
		}
		CRect rcUnderIcon( _pslid.m_rcBoundsIcon );
		rcUnderIcon.InflateRect(
			__EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_X,
			__EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_Y
			);

//		dc.FillSolidRect(
//			&rcUnderIcon,
//			GetColor(
//				_pslid.m_bPressed
//					? XPCLR_HILIGHT
//					: (	_pslid.m_bHover
//						? CLR_3DFACE_IN
//						: CLR_3DFACE_OUT
//						)
//				)
//			);
		if( _pslid.m_bPressed || _pslid.m_bHover )
			dc.FillSolidRect(
				&rcUnderIcon,
				GetColor(
					_pslid.m_bPressed
						? XPCLR_HILIGHT
						: CLR_3DFACE_IN
					)
				);
		
		if( pIcon != NULL && (! pIcon->IsEmpty()) )
		{
			::DrawIconEx(
				dc.GetSafeHdc(),
				_pslid.m_rcBoundsIcon.left,
				_pslid.m_rcBoundsIcon.top,
				pIcon->GetIcon(),
				_pslid.m_rcBoundsIcon.Width(),
				_pslid.m_rcBoundsIcon.Height(),
				0,
				(HBRUSH)NULL,
				DI_NORMAL
				);
		} // if( pIcon != NULL && (! pIcon->IsEmpty()) )
		if( _pslid.m_bPressed || _pslid.m_bHover )
		{
			COLORREF clr =
				GetColor( XPCLR_HILIGHT_BORDER );
			dc.Draw3dRect(
				&rcUnderIcon,
				clr,
				clr
				);
		}
	}

CExtSafeString sItemText = _pslid.m_sText;
	if( (! _pslid.m_rcBoundsText.IsRectEmpty() )
		&& dc.RectVisible( &_pslid.m_rcBoundsText )
		&& ( ! sItemText.IsEmpty() )
		)
	{
		LONG nItemTextLen = sItemText.GetLength();
		CRect rcCaption( _pslid.m_rcBoundsText );
		ASSERT( _pslid.m_hItemTextFont != NULL );
		CFont * pItemFont = CFont::FromHandle( _pslid.m_hItemTextFont );
		ASSERT( pItemFont != NULL );
		ASSERT( pItemFont->GetSafeHandle() != NULL );
		CFont * pOldFont = dc.SelectObject( pItemFont );
		COLORREF clrTextOld =
			dc.SetTextColor(
				GetColor(
					_pslid.m_bExpandMode
						? COLOR_INFOTEXT
						: COLOR_BTNTEXT
					)
				);
		int nOldBkMode = dc.SetBkMode( TRANSPARENT );
		if( _pslid.m_bHorzLayout )
		{
			rcCaption.InflateRect( 3, 0 );
			CSize sizeTextMeasured = _pslid.m_rcBoundsText.Size();
			UINT nOldTA = dc.SetTextAlign(
				TA_TOP | TA_BASELINE
				);
			rcCaption.OffsetRect(
				( (_pslid.m_dwShortcutListStyle & __ESLW_INV_VFONT) == 0 )
					?   sizeTextMeasured.cx/2
					: - sizeTextMeasured.cx/2 + 2
					,
				0
				);
			CPoint ptCenter = rcCaption.CenterPoint();
			if( (_pslid.m_dwShortcutListStyle & __ESLW_INV_VFONT) == 0 )
				ptCenter.y =
					rcCaption.bottom - 4
					- (rcCaption.Height() - sizeTextMeasured.cy)
					;
			else
				ptCenter.y =
					rcCaption.top + 4
					;
			dc.ExtTextOut(
				ptCenter.x,
				ptCenter.y,
				ETO_CLIPPED,
				&rcCaption,
				sItemText,
				sItemText.GetLength(),
				NULL
				);
			dc.SetTextAlign( nOldTA );
		} // if( _pslid.m_bHorzLayout )
		else
		{
			CRect rcClient( _pslid.m_rcCtrlClient );
			rcClient.DeflateRect( 1, 0 );
			CRect rcText(
				max(rcCaption.left, rcClient.left),
				rcCaption.top,
				min(rcCaption.right, rcClient.right),
				rcCaption.bottom
				);
			if( rcText.left < rcText.right )
			{
				UINT nFormat =
					DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
				dc.DrawText(
					sItemText,
					nItemTextLen,
					&rcText,
					nFormat
					);
			} // if( rcText.left < rcText.right )
		} // else from if( _pslid.m_bHorzLayout )
		dc.SetBkMode( nOldBkMode );
		dc.SetTextColor( clrTextOld );
		dc.SelectObject( pOldFont );
	}
}

#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)

COLORREF CExtPaintManager::GetShadowAdjustColor()
{
	return ((COLORREF)(-1));
}

COLORREF CExtPaintManagerXP::GetShadowAdjustColor()
{
	return ((COLORREF)(-1));
}

COLORREF CExtPaintManagerOffice2003::GetShadowAdjustColor()
{
	if(		g_PaintManager.m_bUxApiInitPassed
		&&	g_PaintManager.m_bUxValidColorsExtracted
		&&	g_PaintManager.m_bUxUseIfAvailOnWinXpOrLater
		)
	{
		return g_PaintManager.m_clrUxTaskBandFillColorHint;
	}
	return ((COLORREF)(-1));
}

void CExtPaintManager::FixedBar_AdjustClientRect(
	CExtControlBar * pBar,
	CRect & rcPreCalc
	)
{
	ASSERT_VALID( pBar );
	ASSERT_KINDOF( CExtControlBar, pBar );
	ASSERT( pBar->IsFixedMode() );
	if( pBar->IsDockedVertically() )
		rcPreCalc.DeflateRect(
			__EXT_TB_ROW_DISTANCE/4,
			__EXT_TB_ROW_DISTANCE/2
			);
	else
		rcPreCalc.DeflateRect(
			__EXT_TB_ROW_DISTANCE/2,
			__EXT_TB_ROW_DISTANCE/4
			);
}

void CExtPaintManagerXP::FixedBar_AdjustClientRect(
	CExtControlBar * pBar,
	CRect & rcPreCalc
	)
{
	CExtPaintManager::FixedBar_AdjustClientRect(
		pBar,
		rcPreCalc
		);
}

void CExtPaintManagerOffice2003::FixedBar_AdjustClientRect(
	CExtControlBar * pBar,
	CRect & rcPreCalc
	)
{
	ASSERT_VALID( pBar );
	ASSERT_KINDOF( CExtControlBar, pBar );
	ASSERT( pBar->IsFixedMode() );
	if( stat_GetBPP() <= 8 )
	{
		CExtPaintManagerXP::FixedBar_AdjustClientRect(
			pBar,
			rcPreCalc
			);
		if( pBar->IsDockedVertically() )
			rcPreCalc.InflateRect( 0, 2 );
		else
			rcPreCalc.InflateRect( 2, 0 );
	}
}

INT CExtPaintManager::FixedBar_GetRowDistance(
	CExtToolControlBar * pBar
	)
{
	ASSERT_VALID( pBar );
	ASSERT_KINDOF( CExtToolControlBar, pBar );
	ASSERT( pBar->IsFixedMode() );
	if( pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
		return 0;
	return __EXT_TB_ROW_DISTANCE;
}

INT CExtPaintManagerXP::FixedBar_GetRowDistance(
	CExtToolControlBar * pBar
	)
{
	return CExtPaintManager::FixedBar_GetRowDistance( pBar );
}

INT CExtPaintManagerOffice2003::FixedBar_GetRowDistance(
	CExtToolControlBar * pBar
	)
{
	ASSERT_VALID( pBar );
	ASSERT_KINDOF( CExtToolControlBar, pBar );
	ASSERT( pBar->IsFixedMode() );
	if(		(	(! pBar->IsFloating() )
//			&&	pBar->OnQueryMultiRowLayout()
			)
		||	pBar->m_bPaletteMode
		||	stat_GetBPP() <= 8
		)
		return CExtPaintManagerXP::FixedBar_GetRowDistance( pBar );
	return 0;
}

bool CExtPaintManager::FixedBar_IsPaintRowBkMode(
	CExtToolControlBar * pBar
	)
{
	ASSERT_VALID( pBar );
	ASSERT_KINDOF( CExtToolControlBar, pBar );
	ASSERT( pBar->IsFixedMode() );
	pBar;
	return false;
}

bool CExtPaintManagerXP::FixedBar_IsPaintRowBkMode(
	CExtToolControlBar * pBar
	)
{
	return CExtPaintManager::FixedBar_IsPaintRowBkMode( pBar );
}

bool CExtPaintManagerOffice2003::FixedBar_IsPaintRowBkMode(
	CExtToolControlBar * pBar
	)
{
	ASSERT_VALID( pBar );
	ASSERT_KINDOF( CExtToolControlBar, pBar );
	ASSERT( pBar->IsFixedMode() );
	if(		pBar->m_pDockSite == NULL
		||	pBar->m_bPresubclassDialogMode
		||	pBar->m_bPaletteMode
		||	pBar->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) )
		||	pBar->IsKindOf( RUNTIME_CLASS(CExtPanelControlBar) )
		||	(! pBar->IsFloating() )
		||	stat_GetBPP() <= 8
		)
		return false;
	return true;
}

void CExtPaintManager::PaintFloatToolbarRowBk(
	CDC & dc,
	CExtToolControlBar * pBar,
	int nLastReviewBtnIdx,
	CRect & rcRowBk
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pBar );
	ASSERT_KINDOF( CExtToolControlBar, pBar );
	ASSERT( nLastReviewBtnIdx >= 0 );
	dc;
	pBar;
	nLastReviewBtnIdx;
	rcRowBk;
}

void CExtPaintManagerXP::PaintFloatToolbarRowBk(
	CDC & dc,
	CExtToolControlBar * pBar,
	int nLastReviewBtnIdx,
	CRect & rcRowBk
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pBar );
	ASSERT_KINDOF( CExtToolControlBar, pBar );
	ASSERT( nLastReviewBtnIdx >= 0 );
	dc;
	pBar;
	nLastReviewBtnIdx;
	rcRowBk;
}

void CExtPaintManagerOffice2003::PaintFloatToolbarRowBk(
	CDC & dc,
	CExtToolControlBar * pBar,
	int nLastReviewBtnIdx,
	CRect & rcRowBk
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pBar );
	ASSERT_KINDOF( CExtToolControlBar, pBar );
	ASSERT( nLastReviewBtnIdx >= 0 );
	pBar;
	nLastReviewBtnIdx;
COLORREF clrTop = GetColor( _2003CLR_MLA_NORM_RIGHT );
COLORREF clrBottom = GetColor( _2003CLR_SEPARATOR_LIGHT );
	stat_PaintGradientRect( dc, rcRowBk, clrTop, clrBottom, true );
}

void CExtPaintManager::OnSysColorChange()
{
	OnPaintSessionComplete( this );
	SyncSysColors();
	InitTranslatedColors();
}

void CExtPaintManager::OnSettingChange(
	UINT uFlags,
	__EXT_MFC_SAFE_LPCTSTR lpszSection
	)
{
	uFlags;
	lpszSection;
	OnPaintSessionComplete( this );
	SyncSysColors();
	InitTranslatedColors();
}

void CExtPaintManager::OnDisplayChange(
	INT nDepthBPP,
	CPoint ptSizes
	)
{
	nDepthBPP;
	ptSizes;
	OnPaintSessionComplete( this );
	SyncSysColors();
	InitTranslatedColors();
}

void CExtPaintManager::OnThemeChanged(
	WPARAM wParam,
	LPARAM lParam
	)
{
	wParam;
	lParam;
	OnPaintSessionComplete( this );
	SyncSysColors();
	InitTranslatedColors();
}

void CExtPaintManager::OnPaintSessionComplete(
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
#ifdef __TRACE_PAINT_SESSIONS
		TRACE( "   PAINT_SESSION: Complete\n" );
#endif // __TRACE_PAINT_SESSIONS
	pHelperSrc;
	lParam;
	CachedDcFreeAll();
}

void CExtPaintManagerXP::OnPaintSessionComplete(
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	CExtPaintManager::OnPaintSessionComplete( pHelperSrc, lParam );
}

void CExtPaintManagerOffice2003::OnPaintSessionComplete(
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	CExtPaintManagerXP::OnPaintSessionComplete( pHelperSrc, lParam );
}

CSize CExtPaintManager::GetToolBarRgnRoundersSize()
{
	return CSize( 0, 0 );
}

CSize CExtPaintManagerXP::GetToolBarRgnRoundersSize()
{
	return CSize( 0, 0 );
}

CSize CExtPaintManagerOffice2003::GetToolBarRgnRoundersSize()
{
	return m_sizeToolBarRgnRounders;
}

CExtPaintManager::PAINTTOOLBARTEXTFIELDDATA::PAINTTOOLBARTEXTFIELDDATA()
	: m_pHelperSrc( NULL )
	, m_lParam( 0L )
	, m_nDrawTextFlags( DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOPREFIX )
	, m_sText( _T("") )
	, m_rcButton( 0, 0, 0, 0 )
	, m_rcTextField( 0, 0, 0, 0 )
	, m_bComboField( false )
	, m_bHover( false )
	, m_bPushed( false )
	, m_bEnabled( false )
{
}

CExtPaintManager::PAINTTOOLBARTEXTFIELDDATA::PAINTTOOLBARTEXTFIELDDATA(
	CObject * pHelperSrc,
	__EXT_MFC_SAFE_LPCTSTR sText,
	const RECT & rcButton,
	const RECT & rcTextField,
	bool bComboField,
	bool bHover,
	bool bPushed,
	bool bEnabled
	)
	: m_pHelperSrc( pHelperSrc )
	, m_lParam( 0L )
	, m_nDrawTextFlags( DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOPREFIX )
	, m_sText( (sText == NULL) ? _T("") : sText )
	, m_rcButton( rcButton )
	, m_rcTextField( rcTextField )
	, m_bComboField( bComboField )
	, m_bHover( bHover )
	, m_bPushed( bPushed )
	, m_bEnabled( bEnabled )
{
}

void CExtPaintManager::PaintToolbarTextField(
	CDC & dc,
	PAINTTOOLBARTEXTFIELDDATA & _ptbtfd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( _ptbtfd.m_bComboField )
	{
		PAINTCOMBOFRAMEDATA _pcfd(
			_ptbtfd.m_pHelperSrc,
			_ptbtfd.m_rcButton,
			_ptbtfd.m_bHover,
			_ptbtfd.m_bPushed,
			_ptbtfd.m_bEnabled
			);
		_pcfd.m_bNoTopOuterBorder = true;
		PaintComboFrame( dc, _pcfd );
		dc.SelectClipRgn( NULL );
	} // if( _ptbtfd.m_bComboField )
	else
	{
		if( _ptbtfd.m_bEnabled && (_ptbtfd.m_bHover || _ptbtfd.m_bPushed) )
			dc.Draw3dRect(
				&_ptbtfd.m_rcButton,
				GetColor( COLOR_3DSHADOW ),
				GetColor( COLOR_3DHIGHLIGHT )
				);
	} // else from if( _ptbtfd.m_bComboField )
	dc.FillSolidRect(
		&_ptbtfd.m_rcTextField,
		GetColor( _ptbtfd.m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE )
		);
	if( !_ptbtfd.m_sText.IsEmpty() )
	{
		COLORREF clrTextOld =
			dc.SetTextColor( GetColor(COLOR_BTNTEXT) );
		CFont * pFontOld =
			dc.SelectObject( &m_FontNormal );
		int nBkModeOld = dc.SetBkMode( TRANSPARENT );
		CRect rcText( _ptbtfd.m_rcTextField );
		rcText.left += 5;
		rcText.top += 2;
		dc.DrawText(
			_ptbtfd.m_sText,
			_ptbtfd.m_sText.GetLength(),
			&rcText,
			_ptbtfd.m_nDrawTextFlags
			);
		dc.SetBkMode( nBkModeOld );
		dc.SelectObject( pFontOld );
		dc.SetTextColor( clrTextOld );
	} // if( !_ptbtfd.m_sText.IsEmpty() )
}

void CExtPaintManagerXP::PaintToolbarTextField(
	CDC & dc,
	PAINTTOOLBARTEXTFIELDDATA & _ptbtfd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( _ptbtfd.m_bComboField )
	{
		CExtPaintManager::PaintToolbarTextField( dc, _ptbtfd );
		return;
	}
	if( _ptbtfd.m_bEnabled && (_ptbtfd.m_bHover || _ptbtfd.m_bPushed) )
	{
		COLORREF clrDkBorder = GetColor( XPCLR_HILIGHT_BORDER );
		CRect rcTextFieldOuter( _ptbtfd.m_rcTextField );
		rcTextFieldOuter.InflateRect( 1, 1 );
		dc.Draw3dRect(
			&rcTextFieldOuter,
			clrDkBorder,
			clrDkBorder
			);
	} // if( _ptbtfd.m_bEnabled && (_ptbtfd.m_bHover || _ptbtfd.m_bPushed) )
	dc.FillSolidRect(
		&_ptbtfd.m_rcTextField,
		GetColor( _ptbtfd.m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE )
		);
	if( !_ptbtfd.m_sText.IsEmpty() )
	{
		COLORREF clrTextOld =
			dc.SetTextColor( GetColor(COLOR_BTNTEXT) );
		CFont * pFontOld =
			dc.SelectObject( &m_FontNormal );
		int nBkModeOld = dc.SetBkMode( TRANSPARENT );
		CRect rcText( _ptbtfd.m_rcTextField );
		rcText.left += 5;
		rcText.top += 2;
		dc.DrawText(
			_ptbtfd.m_sText,
			_ptbtfd.m_sText.GetLength(),
			&rcText,
			_ptbtfd.m_nDrawTextFlags
			);
		dc.SetBkMode( nBkModeOld );
		dc.SelectObject( pFontOld );
		dc.SetTextColor( clrTextOld );
	} // if( !_ptbtfd.m_sText.IsEmpty() )
}

COLORREF CExtPaintManager::GetIconAlphaColor() const
{
	return (COLORREF(-1L)); // not alpha icon effect
}

COLORREF CExtPaintManagerXP::GetIconAlphaColor() const
{
	return (COLORREF(-1L)); // not alpha icon effect
}

COLORREF CExtPaintManagerOffice2003::GetIconAlphaColor() const
{
COLORREF clrAlpha =
		( const_cast < CExtPaintManagerOffice2003 * > ( this ) )
			-> GetColor( COLOR_3DFACE );
	return clrAlpha;
}

double CExtPaintManager::GetIconAlphaAdjustPercent() const
{
	return 0.0; // no alpha adjustment
}

double CExtPaintManagerXP::GetIconAlphaAdjustPercent() const
{
	return 0.0; // no alpha adjustment
}

double CExtPaintManagerOffice2003::GetIconAlphaAdjustPercent() const
{
	return 0.70; // 70% enlighten
}

void CExtPaintManager::PaintResizingGripper(
	CDC &dc,
	const RECT & rcGrip,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;
	if(		rcGrip.left > rcGrip.right
		||	rcGrip.top > rcGrip.bottom
		||	( ! dc.RectVisible(&rcGrip) )
		)
		return;
CFont * pOldFont = dc.SelectObject( &m_FontMarlett );
	ASSERT( pOldFont != NULL );
COLORREF clrOld = dc.SetTextColor(
		GetColor( CExtPaintManager::CLR_TEXT_OUT )
		);
INT nOldBkMode = dc.SetBkMode(TRANSPARENT);
	dc.DrawText(
		_T("o"),
		1,
		(LPRECT)&rcGrip,
		DT_SINGLELINE|DT_RIGHT|DT_BOTTOM
		);
	dc.SetBkMode(nOldBkMode);
	dc.SetTextColor(clrOld);
	dc.SelectObject(pOldFont);
}

void CExtPaintManagerXP::PaintResizingGripper(
	CDC &dc,
	const RECT & rcGrip,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	CExtPaintManager::PaintResizingGripper(
		dc,
		rcGrip,
		pHelperSrc,
		lParam
		);
}

void CExtPaintManagerOffice2003::PaintResizingGripper(
	CDC &dc,
	const RECT & rcGrip,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;
	if( stat_GetBPP() <= 8 )
	{
		CExtPaintManagerXP::PaintResizingGripper(
			dc,
			rcGrip,
			pHelperSrc,
			lParam
			);
		return;
	}

COLORREF clrDotShadow = GetColor( _2003CLR_GRIPPER_DOT_LIGHT );
COLORREF clrDotFace = GetColor( _2003CLR_GRIPPER_DOT_DARK );
static const CSize g_sizeGripDot( 2, 2 );
static const CSize g_sizeGripDist( 1, 1 );
static const CSize g_sizeGripShadowOffset( 1, 1 );

CRect rcDotFace( CPoint(rcGrip.right, rcGrip.bottom), g_sizeGripDot );
	rcDotFace.OffsetRect( -g_sizeGripDot );
CRect rcDotShadow( rcDotFace );
	rcDotFace.OffsetRect( -g_sizeGripShadowOffset );
CRect rcDotFaceSave( rcDotFace );
CRect rcDotShadowSave( rcDotShadow );
int nStepH = -(g_sizeGripDot.cx + g_sizeGripDist.cx + g_sizeGripShadowOffset.cx);
int nStepV = -(g_sizeGripDot.cy + g_sizeGripDist.cy + g_sizeGripShadowOffset.cy);
	for( int nDot = 0; nDot < 3; nDot++ )
	{
		dc.FillSolidRect( &rcDotShadow, clrDotShadow );
		dc.FillSolidRect( &rcDotFace, clrDotFace );
		rcDotFace.OffsetRect( 0, nStepV );
		rcDotShadow.OffsetRect( 0, nStepV );
	} // for( int nDot = 0; nDot < nDotCount; nDot++ )
	
	rcDotFace = rcDotFaceSave;
	rcDotShadow = rcDotShadowSave;
	rcDotFace.OffsetRect( nStepH, 0 );
	rcDotShadow.OffsetRect( nStepH, 0 );
	for( nDot = 0; nDot < 2; nDot++ )
	{
		dc.FillSolidRect( &rcDotShadow, clrDotShadow );
		dc.FillSolidRect( &rcDotFace, clrDotFace );
		rcDotFace.OffsetRect( 0, nStepV );
		rcDotShadow.OffsetRect( 0, nStepV );
	} // for( nDot = 0; nDot < 2; nDot++ )

	rcDotFace = rcDotFaceSave;
	rcDotShadow = rcDotShadowSave;
	rcDotFace.OffsetRect( nStepH*2, 0 );
	rcDotShadow.OffsetRect( nStepH*2, 0 );
	dc.FillSolidRect( &rcDotShadow, clrDotShadow );
	dc.FillSolidRect( &rcDotFace, clrDotFace );
}

