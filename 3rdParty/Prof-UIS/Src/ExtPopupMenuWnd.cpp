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

// ExtPopupMenuWnd.cpp : implementation file
//

#include "stdafx.h"

#define _AFX_NO_OLE_SUPPORT

#ifndef __AFXPRIV_H__
	#include <AfxPriv.h>
#endif

#if _MFC_VER < 0x700
	#include <../src/AfxImpl.h>
#else
	#include <../src/mfc/AfxImpl.h>
#endif

#include <math.h>

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	#if (!defined __EXT_POPUP_CTRL_MENU_H)
		#include <ExtPopupCtrlMenu.h>
	#endif
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#if (!defined __EXT_LOCALIZATION_H)
	#include <../Src/ExtLocalization.h>
#endif

#include <../profuisdll/resource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if (!defined __EXT_MFC_WINMMDLL_ALREADY_DYN_LIKNED)
	#define __EXT_MFC_WINMMDLL_ALREADY_DYN_LIKNED
	#pragma message("   Prof-UIS is automatically linking with WinMM library")
	#pragma message("      (Windows Multimedia System)")
	#pragma comment( lib, "winmm.lib" ) 
#endif

#ifdef _DEBUG
	static int g_nPopupCount = 0;
	//#define __VPC_VERIFY_0 ASSERT( g_nPopupCount == 0 )
	#define __VPC_VERIFY_0 { }
	#define __VPC_INC g_nPopupCount++
	#define __VPC_DEC g_nPopupCount--
#else // _DEBUG
	#define __VPC_VERIFY_0 { }
	#define __VPC_INC { }
	#define __VPC_DEC { }
#endif // _DEBUG

#define ID_TIMER_ANIMATION 666
#define ID_PERIOD_ANIMATION 30 // 20

#define ID_TIMER_ITEM_FOCUS_DELAY 667
#define ID_PERIOD_ITEM_FOCUS_DELAY 500

#define ID_TIMER_SCROLLING 668
#define ID_PERIOD_SCROLLING 15

#define ID_TIMER_DELAY_SHOW 669
#define ID_PERIOD_DELAY_SHOW 1000

#define ID_TIMER_DELAY_EXPAND 670
#define ID_FREQ_DELAY_EXPAND 50
#define ID_TOTAL_DELAY_EXPAND 1500

#define __SCROLLING_PIXEL_STEP 5

static const int g_nAnimStepMetric = 20;
static clock_t g_nLastAnimTime = 0;

#define __ANIM_CY 15

#define __EXCLUDE_AREA_GAP_DX 3
#define __EXCLUDE_AREA_GAP_DY __EXCLUDE_AREA_GAP_DX

#define __MI_HORZ_GAP_TO_BORDER 1

#define __BOX_ANIM_METRIC 32 // for __AT_BOXES,__AT_CIRCLES animation

CExtPopupMenuSite CExtPopupMenuSite::g_DefPopupMenuSite;

bool CExtPopupMenuWnd::g_bMenuWithShadows = true; // allow shadows
bool CExtPopupMenuWnd::g_bMenuExpanding = true; // allow hide rarely used
bool CExtPopupMenuWnd::g_bMenuDelayExpanding = true;  // show full menu after short delay
bool CExtPopupMenuWnd::g_bMenuHighlightRarely = true; // display rarely used in different style
bool CExtPopupMenuWnd::g_bMenuShowCoolTips = true; // allow display cool tooltips
bool CExtPopupMenuWnd::g_bMenuExpandAnimation = true; // allow animation when show rarely used
bool CExtPopupMenuWnd::g_bUseStretchOnExpandAnimation = false; // use StretchBlt() instead of BitBlt() for painting menu expand amimation
bool CExtPopupMenuWnd::g_bMenuLargeIcons = false; // use large icons

bool CExtPopupBaseWnd::g_bUseDesktopWorkArea = true; // align to desktop work area (false - to screen area)

bool CExtPopupBaseWnd::g_bEnableOnIdleCalls = false;

CExtPopupBaseWnd::e_animation_type_t
	CExtPopupBaseWnd::g_DefAnimationType =
		CExtPopupBaseWnd::__AT_FADE; // __AT_RANDOM

UINT CExtPopupBaseWnd::g_nMsgPrepareMenu =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::g_nMsgPrepareMenu")
		);
UINT CExtPopupBaseWnd::g_nMsgPopupNext =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::g_nMsgPopupNext")
		);
UINT CExtPopupBaseWnd::g_nMsgPopupPrev =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::g_nMsgPopupPrev")
		);
UINT CExtPopupBaseWnd::g_nMsgNotifyMenuExpanded =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::g_nMsgNotifyMenuExpanded")
		);
UINT CExtPopupBaseWnd::g_nMsgNotifyMenuClosed =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::g_nMsgNotifyMenuClosed")
		);
UINT CExtPopupBaseWnd::g_nMsgPopupDrawItem =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::g_nMsgPopupDrawItem")
		);
UINT CExtPopupBaseWnd::g_nMsgPopupDrawLeftArea =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::g_nMsgPopupDrawLeftArea")
		);
UINT CExtPopupBaseWnd::g_nMsgCreateInplaceEdit =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::g_nMsgCreateInplaceEdit")
		);

#define __POPUP_WNDCLASS_STYLES__ \
	( CS_SAVEBITS )
//	( CS_SAVEBITS \
//		|CS_HREDRAW \
//		|CS_VREDRAW \
//		|CS_NOCLOSE \
//	)

/////////////////////////////////////////////////////////////////////////////
// CExtSoundPlayer

CExtSoundPlayer::CExtSoundPlayerAutoPtr g_SoundPlayer;

IMPLEMENT_DYNCREATE( CExtSoundPlayer, CObject );

CExtSoundPlayer::InternalSoundEventInitDone_t::InternalSoundEventInitDone_t()
	: CEvent( FALSE, TRUE )
{
}

void CExtSoundPlayer::InternalSoundEventInitDone_t::SetInitDone()
{
	SetEvent();
}

void CExtSoundPlayer::InternalSoundEventInitDone_t::WaitInitDone()
{
HANDLE hEvent = *this;
	ASSERT( hEvent != NULL );
	::WaitForSingleObject( hEvent, INFINITE );
}

CExtSoundPlayer::InternalSoundThredadParms_t::InternalSoundThredadParms_t(
	__EXT_MFC_SAFE_LPCTSTR sSoundSpecBuffer,
	HANDLE hModule,
	DWORD dwPlayerFlags
	)
	: m_hModule( hModule )
	, m_dwPlayerFlags( dwPlayerFlags )
	, m_hThread( NULL )
	, m_dwThreadID( 0 )
	, m_pEventInitDone( NULL )
{
	::memset( m_sSoundSpecBuffer, 0, sizeof(m_sSoundSpecBuffer) );
	ASSERT( sSoundSpecBuffer != NULL );
	ASSERT( _tclen(sSoundSpecBuffer) > 0 );
	_tcsncpy(
		m_sSoundSpecBuffer,
		sSoundSpecBuffer,
		sizeof(m_sSoundSpecBuffer) / sizeof(m_sSoundSpecBuffer[0]) - 1
		);
}

CEvent CExtSoundPlayer::InternalSoundThredadParms_t::g_EventPlaySnd(
	TRUE,
	FALSE,
	_T("CExtSoundPlayer__InternalSoundThredadParms_t")
	);

CCriticalSection CExtSoundPlayer::InternalSoundThredadParms_t::g_CsPlaySnd;

DWORD WINAPI CExtSoundPlayer::InternalSoundThredadParms_t::stat_ParallelPlayerProc(
	CExtSoundPlayer::InternalSoundThredadParms_t * pParms
	)
{
	ASSERT( pParms != NULL );
	pParms->PlaySound();
	g_EventPlaySnd.SetEvent();
	return 0;
}

//#define __TRACE_SOUND_PLAYER_THREAD

void CExtSoundPlayer::InternalSoundThredadParms_t::PlaySound()
{
TCHAR sSoundSpecBuffer[ sizeof(m_sSoundSpecBuffer) / sizeof(m_sSoundSpecBuffer[0]) ];
	_tcscpy(
		sSoundSpecBuffer,
		m_sSoundSpecBuffer
		);
HANDLE hModule = m_hModule;
DWORD dwPlayerFlags = m_dwPlayerFlags;
	if( m_pEventInitDone != NULL )
		m_pEventInitDone->SetInitDone();
#ifdef __TRACE_SOUND_PLAYER_THREAD
	TRACE1("CExtSoundPlayer::InternalSoundThredadParms_t::PlaySound(\"%s\") - START\n",sSoundSpecBuffer);
#endif // __TRACE_SOUND_PLAYER_THREAD
	if( (m_dwPlayerFlags & SND_ASYNC) != 0 )
		::PlaySound( NULL, NULL, SND_PURGE );
	::PlaySound(
		sSoundSpecBuffer,
		(HMODULE)hModule,
		dwPlayerFlags
		);
	if( (m_dwPlayerFlags & SND_SYNC) != 0 )
		::PlaySound( NULL, NULL, SND_PURGE );
#ifdef __TRACE_SOUND_PLAYER_THREAD
	TRACE1("CExtSoundPlayer::InternalSoundThredadParms_t::PlaySound(\"%s\") - STOP\n",sSoundSpecBuffer);
#endif // __TRACE_SOUND_PLAYER_THREAD
}

void CExtSoundPlayer::InternalSoundThredadParms_t::PlaySoundParallel()
{
	ASSERT( m_hThread == NULL );
CSingleLock slCs( &g_CsPlaySnd );
	slCs.Lock();
HANDLE hEvent = (HANDLE)g_EventPlaySnd;
	ASSERT( hEvent != NULL );
	if( ::WaitForSingleObject(hEvent,0) == WAIT_OBJECT_0 )
	{
		ASSERT( m_pEventInitDone == NULL );
		InternalSoundEventInitDone_t EventInitDone;
		m_pEventInitDone = &EventInitDone;
		m_hThread =
			::CreateThread(
				NULL,
				0,
				*this,
				this,
				CREATE_SUSPENDED,
				&m_dwThreadID
				);
		ASSERT( m_hThread != NULL );
		if( m_hThread != NULL )
		{
			VERIFY(
				::SetThreadPriority(
					m_hThread,
					THREAD_PRIORITY_BELOW_NORMAL
					)
				);
			::ResumeThread( m_hThread );
			EventInitDone.WaitInitDone();
		} // if( m_hThread != NULL )
		else
			g_EventPlaySnd.SetEvent();
	} // if( ::WaitForSingleObject(hEvent,0) == WAIT_OBJECT_0 )
	if( m_hThread != NULL )
		::CloseHandle( m_hThread );
	slCs.Unlock();
}

CExtSoundPlayer::CExtSoundPlayerAutoPtr::CExtSoundPlayerAutoPtr()
{
	m_pPlayer = new CExtSoundPlayer;
}

CExtSoundPlayer::CExtSoundPlayerAutoPtr::~CExtSoundPlayerAutoPtr()
{
	if( m_pPlayer != NULL )
		delete m_pPlayer;
}

void CExtSoundPlayer::CExtSoundPlayerAutoPtr::InstallSoundPlayer(
	CExtSoundPlayer * pPlayer
	)
{
	ASSERT( pPlayer != NULL );
	ASSERT_VALID( pPlayer );
	ASSERT_KINDOF( CExtSoundPlayer, m_pPlayer );
	if( m_pPlayer != NULL )
		delete m_pPlayer;
	m_pPlayer = pPlayer;
}

CExtSoundPlayer::CExtSoundPlayer()
{
}

CExtSoundPlayer::~CExtSoundPlayer()
{
}

void CExtSoundPlayer::PlaySound(
	CExtSoundPlayer::e_ui_sounds_t eSoundID
	)
{
	ASSERT_VALID( this );
	switch( eSoundID )
	{
	case __NO_SOUND:
		return;
	case __ON_MENU_EXPAND_CLICKED:
	case __ON_MENU_POPUP_DISPLAYED:
		{
			InternalSoundThredadParms_t _istp(
				_T("MenuPopup"),
				NULL,
				SND_ALIAS|SND_NODEFAULT|SND_NOWAIT|SND_ASYNC
				);
			_istp.PlaySoundParallel();
			return;
		}
	case __ON_MENU_CMD_CLICKED:
		{
			InternalSoundThredadParms_t _istp(
				_T("MenuCommand"),
				NULL,
				SND_ALIAS|SND_NODEFAULT|SND_NOWAIT|SND_SYNC
				);
			_istp.PlaySoundParallel();
			return;
		}
	} // switch( eSoundID )
}

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuSite

CExtPopupMenuSite::CExtPopupMenuSite()
	: m_pTopMenu( NULL )
	, m_pWndCapture( NULL )
	, m_pWndAnimation( NULL )
	, m_bShutdownMode( false )
	, m_hMouseHook( NULL )
	, m_hKeyboardHook( NULL )
	, m_lpnResultCmdID( NULL )
{
}

CExtPopupMenuSite::~CExtPopupMenuSite()
{
	_Done();
}

CExtPopupMenuWnd * CExtPopupMenuSite::GetCapture()
{
	ASSERT( !IsEmpty() );
	ASSERT( !IsShutdownMode() );
	return m_pWndCapture;
}

void CExtPopupMenuSite::SetCapture(
	CExtPopupMenuWnd * pNewPopup // = NULL
	)
{
	ASSERT( !IsEmpty() );
	ASSERT( !IsShutdownMode() );

#ifdef _DEBUG
	if( pNewPopup != NULL )
	{
		ASSERT_VALID( pNewPopup );
	}
#endif // _DEBUG
	m_pWndCapture = pNewPopup;
}

void CExtPopupMenuSite::SetInstance(
	CExtPopupMenuWnd * pTopMenu // = NULL
	)
{
	ASSERT( !IsShutdownMode() );
	if( !IsEmpty() )
	{
		ASSERT( m_pTopMenu != NULL );
		ASSERT_VALID( m_pTopMenu );
		ASSERT( m_pTopMenu->m_bTopLevel );
		if( m_pTopMenu == pTopMenu )
			return;
		if( m_pTopMenu != pTopMenu )
			_Done();
	}
	ASSERT( m_pTopMenu == NULL );
	if( pTopMenu != NULL )
	{
		ASSERT_VALID( pTopMenu );
		ASSERT( pTopMenu->m_bTopLevel );
		m_pTopMenu = pTopMenu;
		_Hook( true );
	}
}

CExtPopupMenuWnd * CExtPopupMenuSite::GetAnimated()
{
	ASSERT( !IsEmpty() );
	ASSERT( !IsShutdownMode() );
	return m_pWndAnimation;
}

void CExtPopupMenuSite::SetAnimated(
	CExtPopupMenuWnd * pNewPopup // = NULL
	)
{
	ASSERT( !IsEmpty() );
	ASSERT( !IsShutdownMode() );
	m_pWndAnimation = pNewPopup;
}

CExtPopupMenuWnd * CExtPopupMenuSite::GetInstance()
{
	return m_pTopMenu;
}

void CExtPopupMenuSite::SetTargetCmdIdPtr(
	UINT * lpnResultCmdID // = NULL
	)
{
	m_lpnResultCmdID = lpnResultCmdID;
}

UINT * CExtPopupMenuSite::GetTargetCmdIdPtr()
{
	return m_lpnResultCmdID;
}

void CExtPopupMenuSite::DoneInstance()
{
	_Done();
}

bool CExtPopupMenuSite::IsTopPupup(
	CExtPopupMenuWnd * pTopMenu
	) const
{
	ASSERT( !IsEmpty() );
	ASSERT( !IsShutdownMode() );
	ASSERT_VALID( pTopMenu );
	return (pTopMenu==m_pTopMenu) ? true : false;
}

void CExtPopupMenuSite::_Done()
{
	ASSERT( !m_bShutdownMode );
	m_bShutdownMode = true;
	m_pWndCapture = NULL;
	m_pWndAnimation = NULL;
	m_lpnResultCmdID = NULL;
	if( m_pTopMenu != NULL )
	{
		ASSERT( m_pTopMenu->m_bTopLevel );
		VERIFY( m_pTopMenu->_BuildItems( NULL, true ) );
		if( m_pTopMenu->GetSafeHwnd() != NULL
			&& ::IsWindow( m_pTopMenu->GetSafeHwnd() )
			)
		{
			CExtControlBar::stat_SetMessageString(
				m_pTopMenu->GetOwner()
				);
			m_pTopMenu->DestroyWindow();
			CExtPopupMenuWnd::PassMsgLoop( CExtPopupBaseWnd::g_bEnableOnIdleCalls );
		}
		ASSERT( m_pTopMenu->GetSafeHwnd() == NULL );
		delete m_pTopMenu;
		m_pTopMenu = NULL;
	}
	_Hook( false );
	__VPC_VERIFY_0;
	m_bShutdownMode = false;
}

LRESULT CALLBACK CExtPopupMenuSite::_HookMouseProc(
	int nCode,      // hook code
	WPARAM wParam,  // message identifier
	LPARAM lParam   // mouse coordinates
	)
{

MOUSEHOOKSTRUCT* lpMS = (MOUSEHOOKSTRUCT*)lParam;
	ASSERT( lpMS != NULL );

	if( !g_DefPopupMenuSite.IsEmpty() && !g_DefPopupMenuSite.IsShutdownMode() )
	{
		CExtPopupMenuWnd * pWndCapture =
			g_DefPopupMenuSite.GetCapture();
		if( pWndCapture != NULL
			&& ::IsWindow( pWndCapture->GetSafeHwnd() )
			)
		{
			ASSERT_VALID( pWndCapture );
			switch( wParam )
			{
			//case WM_MOUSEACTIVATE:
			//	if( lpMS->hwnd != pWndCapture->GetSafeHwnd() )
			//		return 1;
			//break;

			case WM_MOUSEMOVE:
			{
				CPoint pt( lpMS->pt );
				pWndCapture->ScreenToClient( &pt );
				bool bNoEat = false;
				if(	pWndCapture->
						_OnMouseMove(
							wParam,
							pt,
							bNoEat
							)
					)
					return 1; // eat!
				if( bNoEat )
					return
						::CallNextHookEx(
							g_DefPopupMenuSite.m_hMouseHook,
							nCode,
							wParam,
							lParam
							);
			}
			break;

			case WM_MOUSEWHEEL:
			{
				bool bNoEat = false;
				if(	pWndCapture->
						_OnMouseWheel(
							wParam,
							lParam,
							bNoEat
							)
					)
					return 1; // eat!
				if( bNoEat )
					return
						::CallNextHookEx(
							g_DefPopupMenuSite.m_hMouseHook,
							nCode,
							wParam,
							lParam
							);
				return 1; // eat!
			}
			break;

			case WM_NCLBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
			case WM_NCMBUTTONDOWN:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:

			case WM_NCLBUTTONUP:
			case WM_NCRBUTTONUP:
			case WM_NCMBUTTONUP:
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			{
				//CRect rcWndCapture;
				//_pWndCapture->GetWindowRect( &rcWndCapture );
				//if( !rcWndCapture.PtInRect(lpMS->pt) )
				//	break;
				CPoint ptSrc( lpMS->pt );
				pWndCapture->ScreenToClient( &ptSrc );

				bool bNoEat = false;
				if(	pWndCapture->_OnMouseClick(
						wParam,
						ptSrc,
						bNoEat
						)
					)
					return 1; // eat!
				if( bNoEat )
					return
						::CallNextHookEx(
							g_DefPopupMenuSite.m_hMouseHook,
							nCode,
							wParam,
							lParam
							);
			
				// post processing			
				CPoint pt( lpMS->pt );
				for(	CExtPopupMenuWnd * pPopup = pWndCapture;
						pPopup != NULL;
						pPopup = pPopup->m_pWndParentMenu
						)
				{
					CPoint ptClient( pt );
					pPopup->ScreenToClient( &ptClient );
					if( pPopup->_PtInWndArea(ptClient) )
						return 1; // eat!
					HWND hWndTest = pPopup->m_wndToolTip.GetSafeHwnd();
					if(		hWndTest != NULL
						&&	hWndTest == (::WindowFromPoint(pt))
						)
						return 1; // eat!
				}

				pPopup = CExtPopupMenuSite::g_DefPopupMenuSite.GetInstance();
				ASSERT_VALID( pPopup );
				ASSERT( ::IsWindow(pPopup->GetSafeHwnd()) );
				pPopup->_OnCancelMode();
				
				if(		wParam != WM_LBUTTONDOWN
					&&	wParam != WM_RBUTTONDOWN
					&&	wParam != WM_MBUTTONDOWN
					)
					// currently NC areas are not safe on MDI apps
					return 1; // eat!
			}
			break;

			//default:
			//	return 1; // eat!
			} // switch( wParam )
		} // if( pWndCapture != NULL ....
		else
			return 1; // eat
	} // if( !g_DefPopupMenuSite.IsEmpty() && !g_DefPopupMenuSite.IsShutdownMode() )
	return
		::CallNextHookEx(
			g_DefPopupMenuSite.m_hMouseHook,
			nCode,
			wParam,
			lParam
			);
}

LRESULT CALLBACK CExtPopupMenuSite::_HookKeyboardProc(
	int nCode,      // hook code
	WPARAM wParam,  // virtual-key code
	LPARAM lParam   // keystroke-message information
	)
{

	if( !g_DefPopupMenuSite.IsEmpty() && !g_DefPopupMenuSite.IsShutdownMode() )
	{
		CExtPopupMenuWnd * pWndCapture =
			g_DefPopupMenuSite.GetCapture();
		if(	pWndCapture->GetSafeHwnd() != NULL
			&& ::IsWindow( pWndCapture->GetSafeHwnd() )
			)
		{
			ASSERT_VALID( pWndCapture );
			if( (lParam & (1<<31)) == 0 )
			{
				// clocks allows to fix problem with
				// duplicated fast key hook calls
				static clock_t _clock_step =
					CLOCKS_PER_SEC / 50; // 1/50 of second
				static clock_t _clock_last =
					0; //clock();
				clock_t _clock_curr =
					clock();
				//ASSERT( _clock_curr >= _clock_last );
				clock_t _clock_diff =
					abs(_clock_curr - _clock_last);
				if( _clock_diff >= _clock_step )
				{
					_clock_last = _clock_curr;
					bool bNoEat = false;
					if(	pWndCapture->
							_OnKeyDown(
								wParam,
								LOWORD(lParam),
								HIWORD(lParam),
								bNoEat
								)
						)
						return 1; // eat!
					if( bNoEat )
						return
							::CallNextHookEx(
								g_DefPopupMenuSite.m_hKeyboardHook,
								nCode,
								wParam,
								lParam
								);
					if( pWndCapture != NULL )
						return 1; // eat!
				}
			} // if( (lParam & (1<<31)) == 0 )
			else
			{
				// 2.21 fix for dlg sys menu
				return 1; // eat!
			}
		}
//		if( !CExtToolControlBar::g_bMenuTracking )
//			return 1; // eat!
	} // if( !g_DefPopupMenuSite.IsEmpty() && !g_DefPopupMenuSite.IsShutdownMode() )

	if(		(!CExtToolControlBar::g_bMenuTracking)
		&&	CExtPopupMenuWnd::IsCmdKeyActivation( lParam )
		&&	( lParam & (1<<31) ) != 0
		)
	{
		CExtPopupMenuWnd::CancelMenuTracking();
		return
			::CallNextHookEx(
				g_DefPopupMenuSite.m_hKeyboardHook,
				nCode,
				wParam,
				lParam
				);
	}

	return
		::CallNextHookEx(
			g_DefPopupMenuSite.m_hKeyboardHook,
			nCode,
			wParam,
			lParam
			);
}

void CExtPopupMenuSite::_Hook(
	bool bHook // = true
	)
{
	if( bHook )
	{
		if( m_hMouseHook == NULL )
		{
			m_hMouseHook =
				::SetWindowsHookEx(
					WH_MOUSE,
					_HookMouseProc, 
					0,
					::GetCurrentThreadId()
					);
			ASSERT( m_hMouseHook != NULL );
		}
		if( m_hKeyboardHook == NULL )
		{
			m_hKeyboardHook =
				::SetWindowsHookEx(
					WH_KEYBOARD,
					_HookKeyboardProc, 
					0,
					::GetCurrentThreadId()
					);
			ASSERT( m_hKeyboardHook != NULL );
		}
	} // if( bHook )
	else
	{
		if( m_hMouseHook != NULL )
		{
			::UnhookWindowsHookEx( m_hMouseHook );
			m_hMouseHook = NULL;
		}
		if( m_hKeyboardHook != NULL )
		{
			::UnhookWindowsHookEx( m_hKeyboardHook );
			m_hKeyboardHook = NULL;
		}
	} // else from if( bHook )
}

/////////////////////////////////////////////////////////////////////////////
// CExtWndShadow

CExtWndShadow::CExtWndShadow()
{
	Destroy();
}

CExtWndShadow::~CExtWndShadow()
{
	Destroy();
}

void CExtWndShadow::_DoPixelOvershadow(
	int nMakeSpec,
	int nPosX,
	int nPosY,
	COLORREF clrShadowAdjust
	)
{
	ASSERT( nMakeSpec >= 0 && nMakeSpec <= 100 );
	ASSERT( !m_rcWndArea.IsRectEmpty() );
	ASSERT( nPosX < m_rcWndArea.Width() + INT(m_nShadowSize) );
	ASSERT( nPosY < m_rcWndArea.Height() + INT(m_nShadowSize) );
	ASSERT( m_pHelperDibSurface != NULL );
int nTotalWidth = m_rcWndArea.Width()+m_nShadowSize;
int nTotalHeight = m_rcWndArea.Height()+m_nShadowSize;
COLORREF * ptr =
		m_pHelperDibSurface
		+ nPosX
		+ (nTotalHeight-nPosY)*nTotalWidth;
COLORREF clrAdj = *ptr;
	if( clrShadowAdjust != ((COLORREF)(-1)) )
	{
		int nMakeSpecR = nMakeSpec + ::MulDiv( (100-nMakeSpec), GetBValue(clrShadowAdjust), 255 );
		int nMakeSpecG = nMakeSpec + ::MulDiv( (100-nMakeSpec), GetGValue(clrShadowAdjust), 255 );
		int nMakeSpecB = nMakeSpec + ::MulDiv( (100-nMakeSpec), GetRValue(clrShadowAdjust), 255 );
		if( nMakeSpecR > 100 )
			nMakeSpecR = 100;
		if( nMakeSpecG > 100 )
			nMakeSpecG = 100;
		if( nMakeSpecB > 100 )
			nMakeSpecB = 100;
		clrAdj = 
			RGB(
				(( nMakeSpecR * int(GetRValue(clrAdj)) ) / 100),
				(( nMakeSpecG * int(GetGValue(clrAdj)) ) / 100),
				(( nMakeSpecB * int(GetBValue(clrAdj)) ) / 100)
				);
	} // if( clrShadowAdjust != ((COLORREF)(-1)) )
	else
	{
		clrAdj = 
			RGB(
				(( nMakeSpec * int(GetRValue(clrAdj)) ) / 100),
				(( nMakeSpec * int(GetGValue(clrAdj)) ) / 100),
				(( nMakeSpec * int(GetBValue(clrAdj)) ) / 100)
				);
	} // else from if( clrShadowAdjust != ((COLORREF)(-1)) )
	*ptr = clrAdj;
}

bool CExtWndShadow::Restore( CDC & dc )
{
	ASSERT( m_nShadowSize >= 0 );
	if( m_nShadowSize == 0 || m_rcWndArea.IsRectEmpty () )
		return true;
	if(		m_bmp0.GetSafeHandle() == NULL
		||	m_bmp1.GetSafeHandle() == NULL
		)
		return false;

INT nWndAreaDX = m_rcWndArea.Width ();
INT nWndAreaDY = m_rcWndArea.Height ();
	ASSERT( nWndAreaDX > 0 && nWndAreaDY > 0 );
CDC dcmm;
	if( !dcmm.CreateCompatibleDC(&dc) )
	{
		ASSERT( FALSE );
		return false;
	}
CBitmap * pbmpold = dcmm.SelectObject( &m_bmp0 );
	ASSERT( pbmpold != NULL );
	dc.BitBlt(
		m_rcWndArea.right, m_rcWndArea.top,
		m_nShadowSize, nWndAreaDY+m_nShadowSize,
		&dcmm,
		0, 0,
		SRCCOPY
		);
	dcmm.SelectObject( &m_bmp1 );
	dc.BitBlt(
		m_rcWndArea.left, m_rcWndArea.bottom,
		nWndAreaDX+m_nShadowSize, m_nShadowSize,
		&dcmm,
		0, 0,
		SRCCOPY
		);
	dcmm.SelectObject( pbmpold );
	return true;
}

bool CExtWndShadow::Paint(
	CDC & dc,
	const CRect & rcWndArea,
	const CRect & rc1stArea, // = CRect(0,0,0,0)
	const CRect & rc2ndArea, // = CRect(0,0,0,0)
	UINT nShadowSize, // = DEF_SHADOW_SIZE
	UINT nBr0, // = DEF_BRIGHTNESS_MIN
	UINT nBr1, // = DEF_BRIGHTNESS_MAX
	bool bEnablePhotos, // = true
	bool bEnablePaingMgrColor // = true
	)
{
	m_rcWndArea = rcWndArea;
	m_rc1stArea = rc1stArea;
	m_rc2ndArea = rc2ndArea;
	m_nShadowSize = nShadowSize;
	m_nBr0 = nBr0;
	m_nBr1 = nBr1;
	m_bEnablePhotos = bEnablePhotos;
	m_bEnablePaingMgrColor = bEnablePaingMgrColor;
	return Paint( dc );
}

bool CExtWndShadow::Paint( CDC & dc )
{
	ASSERT( m_nShadowSize >= 0 );
	ASSERT( !m_rcWndArea.IsRectEmpty() );
	if( m_nShadowSize == 0 || m_rcWndArea.IsRectEmpty() )
		return true;

	if( Restore( dc ) )
		return true;

	if( CExtPaintManager::stat_GetBPP() > 8 )
		return _PaintHi( dc );
	return _PaintLo( dc );
}

bool CExtWndShadow::_PaintLo( CDC & dc )
{
INT nWndAreaDX = m_rcWndArea.Width();
INT nWndAreaDY = m_rcWndArea.Height();
	ASSERT(
			m_bmp0.GetSafeHandle() == NULL
		&&	m_bmp1.GetSafeHandle() == NULL
		);
static int _Pattern[] =
{
	~0xAA,
	~0x55,
	~0xAA,
	~0x55,
	~0xAA,
	~0x55,
	~0xAA,
	~0x55
};
CBitmap bmpsh;
CBrush brsh;
	if(		!bmpsh.CreateBitmap(8, 8, 1, 1, _Pattern)
		||	!brsh.CreatePatternBrush( &bmpsh )
		)
		return false;
CExtMemoryDC dcmm(
		&dc,
		NULL,
		CExtMemoryDC::MDCOPT_TO_MEMORY
		);
	ASSERT( dcmm.GetSafeHdc() != NULL );
	if( dcmm.GetSafeHdc() == NULL )
		return false;
CPalette * pOldPalette = NULL;
    if( dcmm.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
        pOldPalette =
			dcmm.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE );
        dcmm.RealizePalette();
    }
	dcmm.BitBlt(
		m_rcWndArea.left, m_rcWndArea.top,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize,
		&dc,
		m_rcWndArea.left, m_rcWndArea.top,
		SRCCOPY
		);
UINT nPaintShadowSize =
		( m_nShadowSize > 4 )
			? 4
			: m_nShadowSize;
CRect rV(
		 m_rcWndArea.right,
		 m_rcWndArea.top+nPaintShadowSize,
		 m_rcWndArea.right+nPaintShadowSize,
		 m_rcWndArea.bottom
		 );
CRect rH(
		 m_rcWndArea.left+nPaintShadowSize,
		 m_rcWndArea.bottom,
		 m_rcWndArea.right+nPaintShadowSize,
		 m_rcWndArea.bottom+nPaintShadowSize
		 );
CBrush * pbrold = dcmm.SelectObject( &brsh );
	dcmm.PatBlt( rH.left, rH.top, rH.Width(), rH.Height(), 0xA000C9 );
	dcmm.PatBlt( rV.left, rV.top, rV.Width(), rV.Height(), 0xA000C9 );
	dcmm.SelectObject( pbrold );
	dc.BitBlt(
		m_rcWndArea.left, m_rcWndArea.top,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize, 
		&dcmm,
		m_rcWndArea.left, m_rcWndArea.top,
		SRCCOPY
		);
	if( m_bEnablePhotos )
	{
		if( !_MakePhotos(dc,dcmm) )
		{
			ASSERT( FALSE );
			return false;
		}
	}
	if( pOldPalette != NULL )
		dcmm.SelectPalette( pOldPalette, FALSE );
	dcmm.__Flush( FALSE );
	return true;
}

bool CExtWndShadow::_PaintHi( CDC & dc )
{
INT nWndAreaDX = m_rcWndArea.Width();
INT nWndAreaDY = m_rcWndArea.Height();
	ASSERT(
			m_bmp0.GetSafeHandle() == NULL
		&&	m_bmp1.GetSafeHandle() == NULL
		);
CDC dcmm;
	if( !dcmm.CreateCompatibleDC( &dc ) )
	{
		ASSERT( FALSE );
		return false;
	}

COLORREF clrShadowAdjust = ((COLORREF)(-1));
	if( m_bEnablePaingMgrColor )
		clrShadowAdjust = g_PaintManager->GetShadowAdjustColor();

BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = nWndAreaDX+m_nShadowSize;
	bih.biHeight = nWndAreaDY+m_nShadowSize;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = (nWndAreaDX+m_nShadowSize) * (nWndAreaDY+m_nShadowSize);
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	ASSERT( m_pHelperDibSurface == NULL );
HBITMAP hDIB =
		::CreateDIBSection(
			dcmm.GetSafeHdc(),
			(LPBITMAPINFO)&bih,
			DIB_RGB_COLORS,
			(void **)&m_pHelperDibSurface,
			NULL,
			NULL
			);
	if( hDIB == NULL || m_pHelperDibSurface == NULL )
	{
		ASSERT( FALSE );
		return false;
	}

	dcmm.SelectObject( hDIB );
	dcmm.BitBlt(
		0, 0,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize,
		&dc,
		m_rcWndArea.left, m_rcWndArea.top,
		SRCCOPY
		);

UINT nBrDiff = m_nBr1-m_nBr0;
LONG n2nd, nStep, nDist, nDist1;
LONG nDist2 = m_nShadowSize*m_nShadowSize;
INT nMakeSpec;
bool	bCmbaV = false, bCmbaH = false,
		bCmbaVrt = false, bCmbaVrb = false,
		bCmbaHrb = false, bCmbaHlb = false,
		bCmbaLA = false;
INT		nX0 = 0, nX1 = nWndAreaDX,
		nY0 = 0, nY1 = nWndAreaDY,
		nLa0 = 0, nLa1 = 0;
	if( !( m_rc1stArea.IsRectEmpty() || m_rc2ndArea.IsRectEmpty() ) )
	{
		bool bCmbaDetected = false;
		if( m_rc1stArea.right == m_rc2ndArea.right )
		{ // vertical-right combining
			bCmbaDetected = true;
			if( m_rc1stArea.top < m_rc2ndArea.top )
				bCmbaVrb = true;
			else
				bCmbaVrt = true;
		} // vertical-right combining
		if( (!bCmbaDetected) &&
			m_rc1stArea.left == m_rc2ndArea.left )
		{ // vertical-left combining
			if( m_rc2ndArea.bottom >= m_rc1stArea.top-1
				&& m_rc2ndArea.top < m_rc1stArea.top
				)
			{
				if( m_rc2ndArea.right >= m_rc1stArea.right )
				{
					bCmbaDetected = true;
					bCmbaV = true;
				}
			}
			if(	(!bCmbaDetected)
				&& m_rc1stArea.bottom >= m_rc2ndArea.top-1 
				&& m_rc1stArea.top < m_rc2ndArea.top
				)
			{
				if( m_rc1stArea.right >= m_rc2ndArea.right )
				{
					bCmbaDetected = true;
					bCmbaH = true;
					nX0 =
						m_rcWndArea.right  -
						(m_rc1stArea.right - m_rc2ndArea.right);
				}
			}
		} // vertical-left combining
		if( (!bCmbaDetected) &&
			m_rc1stArea.bottom == m_rc2ndArea.bottom
			)
		{ // horizontal-bottom combining
			bCmbaDetected = true;
			if( m_rc1stArea.left < m_rc2ndArea.left )
				bCmbaHrb = true;
			else
				bCmbaHlb = true;
		} // horizontal-bottom combining
		if( (!bCmbaDetected) &&
			m_rc1stArea.right == m_rc2ndArea.left+1
			)
		{
			bCmbaDetected = true;
			bCmbaLA = true;
			nLa0 = m_rcWndArea.top
				+ m_rc2ndArea.bottom - m_rc1stArea.top;
			nLa1 = nLa0 + m_nShadowSize;
		}
		if( (!bCmbaDetected) &&
			m_rc1stArea.left == m_rc2ndArea.right-1
			)
		{
			bCmbaDetected = true;
			bCmbaH = true;
		}
		bCmbaDetected;
	} // if( !( m_rc1stArea.IsRectEmpty() || m_rc2ndArea.IsRectEmpty() ) )
	for( nStep = 0; ULONG(nStep) < m_nShadowSize; nStep++ )
	{
		nMakeSpec = m_nBr0+(nStep*nBrDiff)/m_nShadowSize;
		for(	n2nd = nX0
					+ m_nShadowSize*2+1
					- ( bCmbaH ? (m_nShadowSize*2-nStep) : 0 )
					- ( bCmbaHlb ? m_nShadowSize : 0 )
					;
				n2nd < LONG(nX1) + (bCmbaHrb ? LONG(m_nShadowSize) : 0);
				n2nd++
				)
			_DoPixelOvershadow(
				nMakeSpec,
				n2nd,
				nY1 + nStep,
				clrShadowAdjust
				);
		for(	n2nd = nY0
					+ m_nShadowSize*2+1
					- ( bCmbaV ? (m_nShadowSize*2-nStep) : 0 )
					- ( bCmbaVrt ? m_nShadowSize : 0 )
					;
				n2nd < LONG(nY1) + (bCmbaVrb ? LONG(m_nShadowSize) : 0);
				n2nd++
				)
		{
			if( bCmbaLA
				&& n2nd >= nLa0
				&& n2nd < LONG(nLa1) - (LONG(m_nShadowSize) - nStep)
				)
			{
				continue;
			}
			_DoPixelOvershadow(
				nMakeSpec,
				nX1 + nStep,
				n2nd,
				clrShadowAdjust
				);
		}
		nDist1 = nStep*nStep;
		for( n2nd = 0; ULONG(n2nd) < m_nShadowSize; n2nd++ )
		{
			nDist = nDist1 + n2nd*n2nd;
			if( nDist > nDist2 )
				continue;
			nDist = (LONG)::sqrt( double(nDist) );
			nMakeSpec = m_nBr0+(nDist*nBrDiff)/m_nShadowSize;
			if( !(bCmbaVrb || bCmbaHrb) )
				_DoPixelOvershadow(
					nMakeSpec,
					nX1+nStep,
					nY1+n2nd,
					clrShadowAdjust
					);
			if( !(bCmbaH || bCmbaHlb) )
				_DoPixelOvershadow(
					nMakeSpec,
					m_nShadowSize+(m_nShadowSize-nStep),
					nY1 + n2nd,
					clrShadowAdjust
					);
			if( !(bCmbaV || bCmbaVrt) )
				_DoPixelOvershadow(
					nMakeSpec,
					nX1 + nStep,
					m_nShadowSize+(m_nShadowSize-n2nd),
					clrShadowAdjust
					);
		} // for( n2nd = 0; ULONG(n2nd) < m_nShadowSize; n2nd++ )
	} // for( nStep = 0; ULONG(nStep) < m_nShadowSize; nStep++ )
	dc.BitBlt(
		m_rcWndArea.left, m_rcWndArea.top,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize, 
		&dcmm,
		0, 0,
		SRCCOPY
		);

	if( m_bEnablePhotos )
	{
		if( !_MakePhotos(dc,dcmm) )
		{
			ASSERT( FALSE );
			return false;
		}
	}

	::DeleteObject( hDIB );
	m_pHelperDibSurface = NULL;

	return true;
}

bool CExtWndShadow::_MakePhotos( CDC & dc, CDC & dcmm )
{
	ASSERT( m_nShadowSize >= 0 );
	ASSERT( !m_rcWndArea.IsRectEmpty() );
INT nWndAreaDX = m_rcWndArea.Width();
INT nWndAreaDY = m_rcWndArea.Height();
	ASSERT(
			m_bmp0.GetSafeHandle() == NULL
		&&	m_bmp1.GetSafeHandle() == NULL
		);
	if(	!m_bmp0.CreateCompatibleBitmap(
			&dc, m_nShadowSize, nWndAreaDY+m_nShadowSize
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	dcmm.SelectObject( &m_bmp0 );
	if(	!dcmm.BitBlt(
			0, 0, m_nShadowSize, nWndAreaDY+m_nShadowSize,
			&dc,
			m_rcWndArea.right, m_rcWndArea.top,
			SRCCOPY
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	if(	!m_bmp1.CreateCompatibleBitmap(
			&dc, nWndAreaDX+m_nShadowSize, m_nShadowSize
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	dcmm.SelectObject( &m_bmp1 );
	if(	!dcmm.BitBlt(
			0, 0, nWndAreaDX+m_nShadowSize, m_nShadowSize,
			&dc,
			m_rcWndArea.left, m_rcWndArea.bottom,
			SRCCOPY
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	return true;
}

void CExtWndShadow::_FreeWinObjects()
{
	if( m_bmp0.GetSafeHandle() != NULL )
		m_bmp0.DeleteObject();
	if( m_bmp1.GetSafeHandle() != NULL )
		m_bmp1.DeleteObject();
	m_pHelperDibSurface = NULL;
}

void CExtWndShadow::Destroy()
{
	_FreeWinObjects();
	m_rcWndArea.SetRectEmpty();
	m_bEnablePhotos = false;
	m_bEnablePaingMgrColor = false;
}

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuWnd::MENUITEMDATA

CExtPopupMenuWnd::MENUITEMDATA::MENUITEMDATA()
	: m_cAccelChar( _T('\0') )
{
	// self constant type assertion
	ASSERT( TYPE_SEPARATOR==ID_SEPARATOR );
	m_pWndChild = NULL;
	m_nItemIndex = -100;
	m_nCmdID = TYPE_SEPARATOR;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	m_pCmdNode = NULL;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	m_sItemText = m_sAccelText = _T("");
	m_sizeItem.cx = m_sizeItem.cy = 0;
	m_nIconAreaWidth = 0;
	m_bSelected
		= m_bDisplayed
		= m_bForceDisplayed
		= m_bChildCombine
		= m_bToolButton
		= m_bForceEnabled
		= m_bForceNoLeftGradient
		= m_bCheck
		= m_bRadio
		= m_bIndeterminate
		= m_bXtraChecked
		= m_bNoCmdUI
		= false;
	m_pCbCmdDeliver = NULL;
	m_pCbXtraMarkState = NULL;
	m_bEnabled = true;
	m_hWndSpecCmdReciever = NULL;
	m_pHelperInplaceEditWnd = NULL;
	m_pInplaceEditStr = NULL;
	m_pCbVerifyTextInput = NULL;
	m_pCbPutTextInputResult = NULL;
	m_pInplaceEditCbWndProc = NULL;
	m_pInplaceEditCbCoockie = NULL;
	m_nInplaceEditWidth = 0;
	m_bAllowInplaceEditActivation = true;
	m_nLParam = 0L;
	m_bTempSelTextValid = false;
	m_sTempSelText.Empty();
}

CExtPopupMenuWnd::MENUITEMDATA::MENUITEMDATA(
	const CExtPopupMenuWnd::MENUITEMDATA & other
	)
	: m_cAccelChar( _T('\0') )
{
	// self constant type assertion
	ASSERT( TYPE_SEPARATOR==ID_SEPARATOR );
	m_pWndChild = NULL;
	m_nItemIndex = -100;
	m_nCmdID = TYPE_SEPARATOR;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	m_pCmdNode = NULL;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	m_sItemText = m_sAccelText = _T("");
	m_sizeItem.cx = m_sizeItem.cy = 0;
	m_nIconAreaWidth = 0;
	m_bSelected
		= m_bDisplayed
		= m_bForceDisplayed
		= m_bChildCombine
		= m_bToolButton
		= m_bForceEnabled
		= m_bForceNoLeftGradient
		= m_bCheck
		= m_bRadio
		= m_bIndeterminate
		= m_bXtraChecked
		= m_bNoCmdUI
		= false;
	m_pCbCmdDeliver = NULL;
	m_pCbXtraMarkState = NULL;
	m_bEnabled = true;
	m_hWndSpecCmdReciever = NULL;
	m_pHelperInplaceEditWnd = NULL;
	m_pInplaceEditStr = NULL;
	m_pCbVerifyTextInput = NULL;
	m_pCbPutTextInputResult = NULL;
	m_pInplaceEditCbWndProc = NULL;
	m_pInplaceEditCbCoockie = NULL;
	m_nInplaceEditWidth = 0;
	m_bAllowInplaceEditActivation = true;
	m_nLParam = 0L;
	m_bTempSelTextValid = false;
	m_sTempSelText.Empty();
	AssignFromOther( other );
}

CExtPopupMenuWnd::MENUITEMDATA::~MENUITEMDATA()
{
	// DestroyPopup();
}

CExtPopupMenuWnd::MENUITEMDATA &
	CExtPopupMenuWnd::MENUITEMDATA::operator=(
		const CExtPopupMenuWnd::MENUITEMDATA & other
		)
{
	AssignFromOther( other );
	return (*this);
}

void CExtPopupMenuWnd::MENUITEMDATA::AssignFromOther(
	const CExtPopupMenuWnd::MENUITEMDATA & other
	)
{
	// self constant type assertion
	ASSERT( TYPE_SEPARATOR==ID_SEPARATOR );

LPVOID lpvThis = (LPVOID)this;
LPVOID lpvOther = (LPVOID)(&other);
	if( lpvThis == lpvOther )
		return;
	
	m_pWndChild = other.m_pWndChild;
	m_nItemIndex = other.m_nItemIndex;
	m_nCmdID = other.m_nCmdID;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	m_pCmdNode = other.m_pCmdNode;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	m_sItemText = other.m_sItemText;
	m_sAccelText = other.m_sAccelText;
	m_iconPopup = other.m_iconPopup;
	m_sizeItem = other.m_sizeItem;
	m_nIconAreaWidth = other.m_nIconAreaWidth;
	m_bSelected = other.m_bSelected;
	m_bDisplayed = other.m_bDisplayed;
	m_bForceDisplayed = other.m_bForceDisplayed;
	m_bChildCombine = other.m_bChildCombine;
	m_bToolButton = other.m_bToolButton;
	m_bForceEnabled = other.m_bForceEnabled;
	m_bForceNoLeftGradient = other.m_bForceNoLeftGradient;
	m_cAccelChar = other.m_cAccelChar;
	m_hWndSpecCmdReciever = other.m_hWndSpecCmdReciever;
	m_bCheck = other.m_bCheck;
	m_bRadio = other.m_bRadio;
	m_pCbCmdDeliver = other.m_pCbCmdDeliver;
	m_pCbXtraMarkState = other.m_pCbXtraMarkState;
	m_bXtraChecked = other.m_bXtraChecked;
	m_bNoCmdUI = other.m_bNoCmdUI;
	m_bEnabled = other.m_bEnabled;
	m_bIndeterminate = other.m_bIndeterminate;
	m_pHelperInplaceEditWnd = other.m_pHelperInplaceEditWnd;
	m_pInplaceEditStr = other.m_pInplaceEditStr;
	m_pCbVerifyTextInput = other.m_pCbVerifyTextInput;
	m_pCbPutTextInputResult = other.m_pCbPutTextInputResult;
	m_pInplaceEditCbWndProc = other.m_pInplaceEditCbWndProc;
	m_pInplaceEditCbCoockie = other.m_pInplaceEditCbCoockie;
	m_nInplaceEditWidth = other.m_nInplaceEditWidth;
	m_bAllowInplaceEditActivation = other.m_bAllowInplaceEditActivation;
	m_nLParam = other.m_nLParam;
	m_bTempSelTextValid = other.m_bTempSelTextValid;
	m_sTempSelText = other.m_sTempSelText;
}

CRect CExtPopupMenuWnd::MENUITEMDATA::AdjustInplaceEditRect( const RECT & rcItem ) const
{
CRect rcInplaceEdit( rcItem );
int nInplaceEditWidth = GetInplaceEditWidth();
	ASSERT( nInplaceEditWidth > 0 );
	rcInplaceEdit.DeflateRect(
		1,
		2,
		1,
		2 + __EXT_MENU_VERT_DISTANCE_NORM
		);
	rcInplaceEdit.left = rcInplaceEdit.right - nInplaceEditWidth;
	if( IsPopup() )
//		rcInplaceEdit.OffsetRect(
//			- ::GetSystemMetrics(SM_CXHTHUMB),
//			0
//			);
		rcInplaceEdit.right -= ::GetSystemMetrics(SM_CXHTHUMB);
	return rcInplaceEdit;
}

UINT CExtPopupMenuWnd::MENUITEMDATA::GetCmdID() const
{
	if( IsSeparator() || IsPopup() )
		return ( (UINT) (IDC_STATIC) );
	return m_nCmdID;
}

UINT CExtPopupMenuWnd::MENUITEMDATA::SetCmdID( UINT nCmdID )
{
	// should be used only without references to command manager
	ASSERT( ! IsPopup() ); // should not be applied for popups
UINT nOldCmdID = m_nCmdID;
	m_nCmdID = nCmdID;
	return nOldCmdID;
}

bool CExtPopupMenuWnd::MENUITEMDATA::SetPopupText( __EXT_MFC_SAFE_LPCTSTR strText )
{
	SetText( strText );
	int nPos = m_sItemText.Find(_T('\t'));
	if( nPos >= 0 )
	{
		CExtSafeString sLeft = m_sItemText.Left(nPos);
		CExtSafeString sRight = m_sItemText.Right(m_sItemText.GetLength()-nPos-1);
		m_sItemText = sLeft;
		m_sAccelText = sRight;
	}
	return true;
}

bool CExtPopupMenuWnd::MENUITEMDATA::SetPopupAccelText( __EXT_MFC_SAFE_LPCTSTR strText ) // NULL if empty
{
	m_sAccelText =
		(strText == NULL) ? _T("") : strText;
	return true;
}

__EXT_MFC_SAFE_LPCTSTR CExtPopupMenuWnd::MENUITEMDATA::GetText() const
{
	if( IsSeparator() )
		return _T("");
	return m_sItemText;
}

void CExtPopupMenuWnd::MENUITEMDATA::SetText( __EXT_MFC_SAFE_LPCTSTR strText )
{
	if( IsSeparator() )
		return;
	m_sItemText = (strText != NULL) ? strText : _T("");
}

__EXT_MFC_SAFE_LPCTSTR CExtPopupMenuWnd::MENUITEMDATA::GetAccelText() const
{
	if( IsSeparator() )
		return _T("");
	return m_sAccelText;
}

void CExtPopupMenuWnd::MENUITEMDATA::SetAccelText( __EXT_MFC_SAFE_LPCTSTR strText )
{
	if( IsSeparator() )
		return;
	m_sAccelText = (strText != NULL) ? strText : _T("");
}

void CExtPopupMenuWnd::MENUITEMDATA::AccelCharInit()
{
	m_cAccelChar = _T('\0');
CExtSafeString sDisplayText( GetText() );
	if( sDisplayText.IsEmpty() )
		return;
int nIndex = sDisplayText.Find( _T('&') );
	if( nIndex + 1 == sDisplayText.GetLength() )
	{
		TRACE(_T("warning : & is bad position, access key is invalid.\n"));
		return;
	}
	if( nIndex < 0 )
	{
		if( !CExtPopupMenuWnd::g_bAllowNonAccelPositioning )
			return;
		m_cAccelChar = sDisplayText[0];
	}
	else
		m_cAccelChar = sDisplayText[nIndex + 1]; // -1 + 1 = 0; it's ok
	TCHAR szChar[2] = { m_cAccelChar, _T('\0') };
	::CharUpper( szChar );
	m_cAccelChar = szChar[0];
}

__EXT_MFC_SAFE_TCHAR CExtPopupMenuWnd::MENUITEMDATA::AccelCharGet() const
{
	return __EXT_MFC_SAFE_TCHAR( m_cAccelChar );
}

bool CExtPopupMenuWnd::MENUITEMDATA::AccelCharIsSet() const
{
	return (m_cAccelChar == _T('\0') ) ? false : true;
}

bool CExtPopupMenuWnd::MENUITEMDATA::UpdateCmdManagerCommand(
	CExtCmdItem * pCmdItem,
	int nItemIndex
	)
{
	ASSERT( pCmdItem != NULL );
	ASSERT( !IsPopup() );
	ASSERT( m_pWndChild == NULL );
	m_sItemText.Empty();
	m_sAccelText.Empty();
	m_nCmdID = pCmdItem->m_nCmdID;
	if( !CExtCmdManager::IsCommand(m_nCmdID) )
	{
		m_nCmdID = TYPE_SEPARATOR;
		m_nItemIndex = -100;
		ASSERT( FALSE );
		return false;
	}
	if( nItemIndex >= 0 )
		m_nItemIndex = nItemIndex;
	SetText( pCmdItem->m_sMenuText );
	SetAccelText( pCmdItem->m_sAccelText );
	AccelCharInit();
	MeasureItem();
	return true;
}

CExtCmdItem * CExtPopupMenuWnd::MENUITEMDATA::GetCmd() const
{
	if( IsSeparator() || IsNoCmdUI() )
		return NULL;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	CExtCustomizeCmdTreeNode * pNode = ((MENUITEMDATA*)this)->GetCmdNode();
	if(		pNode != NULL
		&&	(	(pNode->GetFlags() & __ECTN_TBB_COLOR) != NULL
			||	IsInplaceEdit()
			)
		)
	{
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( GetCmdReciever() ),
				pNode->GetCmdID( false )
				);
		ASSERT( pCmdItem != NULL );
		return pCmdItem;
	} // if( pNode != NULL ...
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( IsPopup() )
		return NULL;
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( GetCmdReciever() ),
			m_nCmdID
			);
	//ASSERT( pCmdItem != NULL );
	return pCmdItem;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeCmdTreeNode * CExtPopupMenuWnd::MENUITEMDATA::GetCmdNode()
{
#ifdef _DEBUG
	if( m_pCmdNode != NULL )
	{
		ASSERT_VALID( m_pCmdNode );
	}
#endif // _DEBUG
	return m_pCmdNode;
}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

void CExtPopupMenuWnd::MENUITEMDATA::MeasureItem()
{
	AccelCharInit();
	m_nIconAreaWidth = __EXT_MENU_ICON_CX;
	if( IsSeparator() )
	{
		m_sizeItem.cx = 0;
		m_sizeItem.cy = __EXT_MENU_SEPARATOR_HEIGHT;
		return;
	}
CExtSafeString sMeasureText( GetText() );
	if( !IsInplaceEdit() )
		sMeasureText += (LPCTSTR)GetAccelText();
	sMeasureText.Replace( _T("&"), _T("") );
CWindowDC dc(NULL);
CFont * pOldFont =
		dc.SelectObject( &(g_PaintManager->m_FontNormal) );
	ASSERT( pOldFont != NULL );
CRect rect( 0, 0, 0, 0 );
	::DrawText(
		dc.GetSafeHdc(),
		sMeasureText,
		sMeasureText.GetLength(),
		&rect,
		DT_CALCRECT|DT_SINGLELINE|DT_LEFT
		);
	m_sizeItem.cx = rect.Width();
	m_sizeItem.cy = rect.Height() + __EXT_MENU_IMG_RECT_OUT_GAP*2;
	dc.SelectObject( pOldFont );

CSize sizeIcon( 0, 0 );
int nIconHeightMetric = 0;
	if( !IsToolButton() )
	{
		CExtCmdIcon * pIcon = GetIconPtr();
		if( pIcon != NULL )
		{
			sizeIcon = pIcon->GetSize();
			if( g_bMenuLargeIcons )
			{
				sizeIcon.cx *= 2;
				sizeIcon.cy *= 2;
			} // if( g_bMenuLargeIcons )
			if( sizeIcon.cx < __EXT_MENU_ICON_CX )
				sizeIcon.cx = __EXT_MENU_ICON_CX;
			if( sizeIcon.cy < __EXT_MENU_ICON_CY )
				sizeIcon.cy = __EXT_MENU_ICON_CY;
			if(	sizeIcon.cx <= 18 && sizeIcon.cy < sizeIcon.cx )
				sizeIcon.cy = sizeIcon.cx;
			m_nIconAreaWidth =
				max( m_nIconAreaWidth, sizeIcon.cx );
			nIconHeightMetric = sizeIcon.cy + __EXT_MENU_IMG_RECT_OUT_GAP*2;
		} // if( pIcon != NULL )
	} // if( !IsToolButton() )

	m_sizeItem.cy = max( m_sizeItem.cy, nIconHeightMetric );
	if( m_sizeItem.cy < __EXT_MENU_MIN_HEIGHT )
		m_sizeItem.cy = __EXT_MENU_MIN_HEIGHT;
	if( IsToolButton() && sizeIcon.cy == 0 )
		m_sizeItem.cy -= 2;
	m_nIconAreaWidth = max( sizeIcon.cx, m_sizeItem.cy );

	if( IsExtraMark() )
	{
		m_nIconAreaWidth *= 2;
		m_nIconAreaWidth += 4;
	} // if( IsExtraMark() )
	
	m_sizeItem.cx +=
		__EXT_MENU_GAP * 7;
	if( IsInplaceEdit() )
	{
		m_sizeItem.cx += m_nInplaceEditWidth + 2 + 10;
//		if( IsPopup() )
//			m_sizeItem.cx += ::GetSystemMetrics(SM_CXHTHUMB);
	} // if( IsInplaceEdit() )
	else if( IsToolButton() )
	{
		m_sizeItem.cx +=
			__DROPDOWN_ARROW_GAP * 2
			+ __EXT_MENU_TOOLBTN_GAP * 2;
		m_sizeItem.cy += __EXT_MENU_VERT_DISTANCE_NORM;
	} // if( IsToolButton() )
	else
	{
		m_sizeItem.cx +=
			m_nIconAreaWidth // sizeIcon.cx
			+ __EXT_MENU_POPUP_ARROW_AREA_DX;
		m_sizeItem.cy += __EXT_MENU_VERT_DISTANCE_TOOL;
	} // else from if( IsToolButton() )

}

bool CExtPopupMenuWnd::MENUITEMDATA::UpdateFromMenu(
	HWND hWndCmdProfileInfo,
	CMenu * pTrackMenu,
	int nItemIndex
	)
{
	ASSERT( pTrackMenu != NULL );
	ASSERT( pTrackMenu->GetSafeHmenu() != NULL );
	
#ifdef _DEBUG
	int	nMenuItemCount = pTrackMenu->GetMenuItemCount();
	ASSERT( nMenuItemCount > 0 );
	ASSERT( nItemIndex >= 0 && nItemIndex<nMenuItemCount );
#endif // _DEBUG

MENUITEMINFO _mii;
	::memset( &_mii, 0, sizeof(MENUITEMINFO) );
	_mii.cbSize = sizeof(MENUITEMINFO);
	_mii.fMask =
		MIIM_CHECKMARKS
		|MIIM_DATA
		|MIIM_ID
		|MIIM_STATE
		|MIIM_SUBMENU
		|MIIM_TYPE
		;
	_mii.cch = __MAX_UI_ITEM_TEXT;
CExtSafeString _sItemText;
	_mii.dwTypeData =
		_sItemText.GetBuffer(__MAX_UI_ITEM_TEXT);
	ASSERT( _mii.dwTypeData != NULL );
	if( _mii.dwTypeData == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	if( !pTrackMenu->GetMenuItemInfo(
			nItemIndex,
			&_mii,
			TRUE
			)
		)
	{
		_sItemText.ReleaseBuffer();
		ASSERT( FALSE );
		return false;
	}
	_sItemText.ReleaseBuffer();

	m_nItemIndex = nItemIndex;
	if( (_mii.fType&MFT_SEPARATOR) != 0 )
	{
		m_sItemText.Empty();
		m_nCmdID = TYPE_SEPARATOR;
		MeasureItem();
		return true;
	}

	if( _mii.hSubMenu != NULL )
	{
		if( !ConstructPopup() )
		{
			ASSERT( FALSE );
			return false;
		}
		m_nCmdID = (UINT)TYPE_POPUP;
		SetPopupText( _sItemText );
		MeasureItem();
		return true;
	}

//	m_sItemText.Empty();
	if( !CExtCmdManager::IsCommand(_mii.wID) )
	{
		ASSERT( FALSE );
		return false;
	}
	m_nCmdID = _mii.wID;
	CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( hWndCmdProfileInfo ),
			m_nCmdID
			);
	ASSERT( pCmdItem != NULL );
	if( pCmdItem == NULL )
		return false;

	if( _sItemText.IsEmpty() )
		_sItemText = pCmdItem->m_sMenuText;
	if( _sItemText.IsEmpty() )
		_sItemText = pCmdItem->m_sToolbarText;
	SetPopupText( _sItemText );
	MeasureItem();
	return true;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)

#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)

bool CExtPopupMenuWnd::MENUITEMDATA::stat_CbVerifyTextInput(
	CInPlaceEditWnd & wndEdit,
	CExtCustomizeCmdTreeNode * pNode,
	__EXT_MFC_SAFE_LPCTSTR sTextOld,
	__EXT_MFC_SAFE_LPCTSTR sTextNew
	)
{
	ASSERT_VALID( pNode );
	ASSERT( sTextOld != NULL );
	ASSERT( sTextNew != NULL );
	ASSERT(
			wndEdit.GetSafeHwnd() != NULL
		&&	::IsWindow( wndEdit.GetSafeHwnd() )
		);
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( wndEdit.GetSafeHwnd() );
	if( pSite == NULL )
		return true;
	return
		pSite->OnTextFieldVerify(
			NULL,
			pNode,
			sTextOld,
			sTextNew
			);
}

void CExtPopupMenuWnd::MENUITEMDATA::stat_CbPutTextInputResult(
	CInPlaceEditWnd & wndEdit,
	CExtCustomizeCmdTreeNode * pNode,
	__EXT_MFC_SAFE_LPCTSTR sTextNew
	)
{
	ASSERT_VALID( pNode );
	ASSERT( sTextNew != NULL );
	ASSERT(
			wndEdit.GetSafeHwnd() != NULL
		&&	::IsWindow( wndEdit.GetSafeHwnd() )
		);
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( wndEdit.GetSafeHwnd() );
	if( pSite == NULL )
		return;
	pSite->OnTextFieldInplaceTextSet(
		NULL,
		pNode,
		pNode->m_sDefInplaceEditBuffer,
		sTextNew
		);
}

bool CExtPopupMenuWnd::MENUITEMDATA::stat_CbInplaceEditWndProc(
	LRESULT & lResult,
	UINT message,
	WPARAM wParam,
	LPARAM lParam,
	CEdit & wndEdit,
	CExtCustomizeCmdTreeNode * pNode
	)
{
	if( message == CExtCustomizeSite::g_nMsgQueryCustomizeSite )
		return false;
	if( (wndEdit.GetStyle() & WS_VISIBLE) == 0 )
		return false;
	ASSERT_VALID( pNode );
	ASSERT(
			wndEdit.GetSafeHwnd() != NULL
		&&	::IsWindow( wndEdit.GetSafeHwnd() )
		);
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( wndEdit.GetSafeHwnd() );
	if( pSite == NULL )
		return false;
	return
		pSite->OnTextFieldWndProcHook(
			lResult,
			message,
			wParam,
			lParam,
			wndEdit,
			NULL,
			pNode
			);
}

bool CExtPopupMenuWnd::MENUITEMDATA::stat_CbInitListBoxContent(
	CExtPopupInplaceListBox & wndListBox,
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( pNode );
	ASSERT(
			wndListBox.GetSafeHwnd() != NULL
		&&	::IsWindow( wndListBox.GetSafeHwnd() )
		);
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( wndListBox.GetSafeHwnd() );
	if( pSite == NULL )
		return false;
	return
		pSite->OnPopupListBoxInitContent(
			NULL,
			pNode,
			wndListBox
			);
}

bool CExtPopupMenuWnd::MENUITEMDATA::stat_CbListBoxSelection(
	CExtPopupInplaceListBox & wndListBox,
	CExtCustomizeCmdTreeNode * pNode,
	int eSAT // CExtPopupInplaceListBox::e_sel_action_t
	)
{
	ASSERT_VALID( pNode );
	ASSERT(
			wndListBox.GetSafeHwnd() != NULL
		&&	::IsWindow( wndListBox.GetSafeHwnd() )
		);
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( wndListBox.GetSafeHwnd() );
	if( pSite == NULL )
		return false;
	switch( ((CExtPopupInplaceListBox::e_sel_action_t)eSAT) )
	{
	case CExtPopupInplaceListBox::e_sel_action_t::__SAT_SELCHANGE:
		return
			pSite->OnPopupListBoxSelChange(
				NULL,
				pNode,
				wndListBox
				);
	case CExtPopupInplaceListBox::e_sel_action_t::__SAT_SELENDOK:
		return
			pSite->OnPopupListBoxSelEndOK(
				NULL,
				pNode,
				wndListBox
				);
	case CExtPopupInplaceListBox::e_sel_action_t::__SAT_SELENDCANCEL:
		return
			pSite->OnPopupListBoxSelEndCancel(
				NULL,
				pNode,
				wndListBox
				);
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
	break;
#endif // _DEBUG
	} // switch( ((CExtPopupInplaceListBox::e_sel_action_t)eSAT) )
	return false;
}

bool CExtPopupMenuWnd::MENUITEMDATA::stat_CbListBoxItemDraw(
	CExtPopupInplaceListBox & wndListBox,
	CExtCustomizeCmdTreeNode * pNode,
	LPDRAWITEMSTRUCT pDIS
	)
{
	ASSERT_VALID( pNode );
	ASSERT( pDIS != NULL && pDIS->hDC != NULL );
	ASSERT(
			wndListBox.GetSafeHwnd() != NULL
		&&	::IsWindow( wndListBox.GetSafeHwnd() )
		);
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( wndListBox.GetSafeHwnd() );
	if( pSite == NULL )
		return false;
	if(	pSite->OnPopupListBoxItemDraw(
			NULL,
			pNode,
			wndListBox,
			pDIS
			)
		)
		return true;

	if( (wndListBox.GetStyle() & LBS_HASSTRINGS) == 0 )
		return false;
	if( ((INT)pDIS->itemID) < 0 || ((INT)pDIS->itemID) >= wndListBox.GetCount() )
		return true;
CDC dc;
	dc.Attach( pDIS->hDC );

	dc.FillSolidRect(
		&pDIS->rcItem,
		g_PaintManager->GetColor(
			( pDIS->itemState & ODS_SELECTED )
				? COLOR_HIGHLIGHT
				: COLOR_WINDOW
			)
		);

CString sText;
	wndListBox.GetText( (INT)pDIS->itemID, sText );
	if( !sText.IsEmpty() )
	{
		COLORREF clrTextOld =
			dc.SetTextColor(
				g_PaintManager->GetColor(
					( pDIS->itemState & ODS_SELECTED )
						? COLOR_HIGHLIGHTTEXT
						: COLOR_BTNTEXT
					)
				);
		int nOldBkMode = dc.SetBkMode( TRANSPARENT );
		CRect rcText( pDIS->rcItem );
		rcText.DeflateRect(
			pSite->OnPopupListBoxCalcItemExtraSizes(
				NULL,
				pNode
				)
			);
		dc.DrawText(
			sText,
			&rcText,
			DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_NOPREFIX
			);
		dc.SetBkMode( nOldBkMode );
		dc.SetTextColor( clrTextOld );
	} // if( !sText.IsEmpty() )

	if( pDIS->itemState & ODS_SELECTED )
		dc.DrawFocusRect( &pDIS->rcItem );

	dc.Detach();
	return true;
}

bool CExtPopupMenuWnd::MENUITEMDATA::stat_CbListBoxItemMeasure(
	CExtPopupInplaceListBox & wndListBox,
	CExtCustomizeCmdTreeNode * pNode,
	LPMEASUREITEMSTRUCT pMIS
	)
{
	ASSERT_VALID( pNode );
	ASSERT( pMIS != NULL );
	ASSERT(
			wndListBox.GetSafeHwnd() != NULL
		&&	::IsWindow( wndListBox.GetSafeHwnd() )
		);
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( wndListBox.GetSafeHwnd() );
	if( pSite == NULL )
		return false;
	if(	pSite->OnPopupListBoxItemMeasure(
			NULL,
			pNode,
			wndListBox,
			pMIS
			)
		)
		return true;

	if( (wndListBox.GetStyle() & LBS_HASSTRINGS) == 0 )
		return false;
	pMIS->itemWidth = pMIS->itemHeight = 10;
	if( ((INT)pMIS->itemID) < 0 || ((INT)pMIS->itemID) >= wndListBox.GetCount() )
		return true;
CExtSafeString sMeasure;
	wndListBox.GetText( (INT)pMIS->itemID, *((CString*)&sMeasure) );
	if( sMeasure.IsEmpty() )
		return true;
CWindowDC dc( NULL );
CRect rcMeasure =
		CExtPaintManager::stat_CalcTextDimension(
			dc,
			g_PaintManager->m_FontNormal,
			sMeasure
			);
	rcMeasure.InflateRect(
		pSite->OnPopupListBoxCalcItemExtraSizes(
			NULL,
			pNode
			)
		);
	pMIS->itemWidth = rcMeasure.Width();
	pMIS->itemHeight = rcMeasure.Height();
	return true;
}
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)

bool CExtPopupMenuWnd::MENUITEMDATA::UpdateFromCmdTree(
	HWND hWndCmdRecv,
	CExtCustomizeCmdTreeNode * pNode,
	int nItemIndex
	)
{
	ASSERT_VALID( pNode );
	m_nItemIndex = nItemIndex;

CExtCustomizeSite * pSite = NULL;
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	if( (pNode->GetFlags() & __ECTN_TBB_TEXT_FIELD) != 0 )
	{
		ASSERT( m_hWndSpecCmdReciever != NULL );
		ASSERT( ::IsWindow(m_hWndSpecCmdReciever) );
		pSite =
			CExtCustomizeSite::GetCustomizeSite(
				m_hWndSpecCmdReciever
				);
		if( pSite == NULL )
		{
			ASSERT( FALSE );
			return false;
		}
		bool bComboField =
			( (pNode->GetFlags() & __ECTN_TBB_COMBO) != 0 )
				? true : false;
		m_nCmdID =
			bComboField
				? ((UINT)TYPE_POPUP)
				: pNode->GetCmdID( false )
				;
		m_pCmdNode = pNode;
		if( bComboField )
		{
			// construct a popup
			ASSERT( m_iconPopup.IsEmpty() );
			ASSERT( m_pWndChild == NULL );
			
			CExtPopupControlMenuWnd * pExtCrtPopup =
				pSite->OnTextFieldCreateDropPopup(
					NULL,
					pNode,
					m_hWndSpecCmdReciever,
					false
					);
			if( pExtCrtPopup != NULL )
			{
				m_pWndChild = pExtCrtPopup;
			} // if( pExtCrtPopup != NULL )
			else
			{
				CExtPopupListBoxMenuWnd * pPopup = new CExtPopupListBoxMenuWnd;
				pPopup->m_lParamListCookie = pNode->GetLParam();
				pPopup->m_pCbListBoxSelection = 
					(CExtPopupInplaceListBox::pCbListBoxSelection)
						stat_CbListBoxSelection;
				pPopup->m_pSelectionCoockie = (LPVOID)pNode;
				pPopup->m_pCbInitListBoxContent =
					(CExtPopupInplaceListBox::pCbInitListBoxContent)
						stat_CbInitListBoxContent;
				pPopup->m_pInitListBoxCoockie = (LPVOID)pNode;
				pPopup->m_pCbListBoxItemDraw =
					(CExtPopupInplaceListBox::pCbListBoxItemDraw)
						stat_CbListBoxItemDraw;
				pPopup->m_pCbListBoxItemMeasure =
					(CExtPopupInplaceListBox::pCbListBoxItemMeasure)
						stat_CbListBoxItemMeasure;
				pPopup->m_pListBoxItemCoockie = (LPVOID)pNode;
				pSite->OnPopupListBoxGetStyles(
					NULL,
					pNode,
					pPopup->m_dwListBoxStyles
					);
				CSize sizeMeasure =
					pSite->OnPopupListBoxMeasureTrackSize(
						NULL,
						pNode
						);
				if( sizeMeasure.cx == 0 && sizeMeasure.cy == 0 )
				{
					pPopup->m_sizeChildControl.cx =
						pNode->TextFieldWidthGet()
						//+ ::GetSystemMetrics( SM_CXHTHUMB )
						;
					pPopup->m_sizeChildControl.cy = pNode->DropDownHeightMaxGet();
				} // if( sizeMeasure.cx == 0 && sizeMeasure.cy == 0 )
				else
					pPopup->m_sizeChildControl = sizeMeasure;
				m_pWndChild = pPopup;
			} // else from if( pExtCrtPopup != NULL )
			m_pWndChild->m_hWndCmdReciever = m_hWndSpecCmdReciever;
			//
			m_sItemText = _T("");
			m_sAccelText = _T("");
			SetPopupText( pNode->GetTextInMenu( NULL ) );
		} // if( bComboField )
		else
			m_sItemText = pNode->GetTextInMenu( NULL );
		pSite->OnTextFieldInplaceTextGet(
			NULL,
			pNode,
			pNode->m_sDefInplaceEditBuffer
			);
		SetInplaceEdit(
			&pNode->m_sDefInplaceEditBuffer,
			(CExtPopupMenuWnd::pCbVerifyTextInput)stat_CbVerifyTextInput,
			(CExtPopupMenuWnd::pCbPutTextInputResult)stat_CbPutTextInputResult,
			(CExtPopupMenuWnd::pCbInplaceEditWndProc)stat_CbInplaceEditWndProc,
			(LPVOID)pNode,
			pNode->TextFieldWidthGet()
			);
		SetDisplayed( true );
		MeasureItem();
		return true;
	} // if( (pNode->GetFlags() & __ECTN_TBB_TEXT_FIELD) != 0 )
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)

	if( pNode->GetFlags() & __ECTN_TBB_COLOR )
	{
		ASSERT( m_iconPopup.IsEmpty() );
		ASSERT( m_pWndChild == NULL );
		 CExtPopupColorMenuWnd * pColorPopup= new CExtPopupColorMenuWnd;
		 m_pWndChild = pColorPopup;
		
		ASSERT( m_hWndSpecCmdReciever != NULL );
		ASSERT( ::IsWindow(m_hWndSpecCmdReciever) );
		m_pWndChild->m_hWndCmdReciever = m_hWndSpecCmdReciever;

		m_nCmdID = (UINT)TYPE_POPUP;
		m_sItemText = pNode->GetTextInMenu( NULL );
		m_pCmdNode = pNode;
		ASSERT( m_pWndChild != NULL );
		m_pWndChild->m_pNode = pNode;

		pColorPopup->m_lParamCookie = pNode->GetLParam();
		pColorPopup->m_bEnableBtnColorDefault =
			( (pNode->GetFlags() & __ECTN_TBB_MCB_DEFAULT) != 0 ) ? true : false;
		pColorPopup->m_bEnableBtnColorCustom =
			( (pNode->GetFlags() & __ECTN_TBB_MCB_CUSTOM) != 0 ) ? true : false;
		if( pSite == NULL )
			pSite =
				CExtCustomizeSite::GetCustomizeSite(
					m_hWndSpecCmdReciever
					);
		if( pSite == NULL )
		{
			ASSERT( FALSE );
			return false;
		}
		pSite->OnColorItemValueGet(
			NULL,
			pNode,
			pColorPopup->m_clrDefault,
			false
			);
		pSite->OnColorItemValueGet(
			NULL,
			pNode,
			pColorPopup->m_clrInitial,
			true
			);
		CExtSafeString sBtnText;
		if(	pSite->OnColorItemGetBtnTextDefault(
				NULL,
				pNode,
				sBtnText
				)
			)
			pColorPopup->m_sBtnTextColorDefault = sBtnText;
		sBtnText.Empty();
		if(	pSite->OnColorItemGetBtnTextCustom(
				NULL,
				pNode,
				sBtnText
				)
			)
			pColorPopup->m_sBtnTextColorCustom = sBtnText;
		HICON hIconColorItem =
			pSite->OnColorItemGenerateIcon(
				NULL,
				pNode,
				pColorPopup->m_clrInitial
				);
		if( hIconColorItem != NULL )
			m_iconPopup.AssignFromHICON( hIconColorItem, false );
		
		MeasureItem();
		return true;
	} // if( pNode->GetFlags() & __ECTN_TBB_COLOR )

	if(		(pNode->GetFlags() & __ECTN_AUTO_POPUP) != 0
		||	pNode->GetNodeCount() > 0
		)
	{
		if( !ConstructPopup() )
		{
			ASSERT( FALSE );
			return false;
		}
		m_nCmdID = (UINT)TYPE_POPUP;
		m_sItemText = pNode->GetTextInMenu( NULL );
		m_pCmdNode = pNode;
		ASSERT( m_pWndChild != NULL );
		m_pWndChild->m_pNode = pNode;
		MeasureItem();
		return true;
	}

//	m_sItemText.Empty();
	if( !CExtCmdManager::IsCommand(pNode->GetCmdID()) )
	{
		ASSERT( FALSE );
		return false;
	}
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	if( (pNode->GetFlags() & (__ECTN_TBB_COMBO|__ECTN_TBB_TEXT_FIELD) ) == 0 )
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
		m_nCmdID = pNode->GetCmdID();
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( hWndCmdRecv ),
			pNode->GetCmdID()
			);
	ASSERT( pCmdItem != NULL );
	if( pCmdItem == NULL )
		return false;
	m_sItemText = pNode->GetTextInMenu( pCmdItem );
	m_pCmdNode = pNode;
	MeasureItem();
	return true;
}

#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

/////////////////////////////////////////////////////////////////////////////
// CExtPopupBaseWnd

IMPLEMENT_DYNCREATE(CExtPopupBaseWnd, __BaseClassOfCExtPopupBaseWnd)

CExtPopupBaseWnd::CExtPopupBaseWnd()
{
	m_clrFadeBitsSrc
		= m_clrFadeBitsDst
		= m_clrFadeBitsTmp
		= NULL;
	m_AnimationType = g_DefAnimationType;
	_InitAnimation();
	m_ePlaySoundOnAnimationFinished =
		CExtSoundPlayer::__NO_SOUND;

	m_sizeFullItems.cx = m_sizeFullItems.cy = 1;

	m_bExcludeAreaSpec
		= m_bCombineWithEA
		= false;
	m_rcExcludeArea.SetRectEmpty();
	m_ptTrack.x = m_ptTrack.y;
}

CExtPopupBaseWnd::~CExtPopupBaseWnd()
{
}

void CExtPopupBaseWnd::_AdjustAnimation(
	CExtPopupBaseWnd::e_animation_type_t & eAT
	)
{
	ASSERT_VALID( this );
	eAT;
}

void CExtPopupBaseWnd::_InitAnimation()
{
	ASSERT_VALID( this );

	m_nAnimPercent = m_nAnimPercentOld = 0;
	//m_AnimationType = g_DefAnimationType;

	_AdjustAnimation( m_AnimationType );

	if( (m_AnimationType < 0
			|| m_AnimationType > __AT_MAX_VALID_VALUE)
		&&
		m_AnimationType != __AT_CONTENT_DISPLAY
		&&
		m_AnimationType != __AT_CONTENT_EXPAND
		)
		m_AnimationType = __AT_NONE;

	if( m_AnimationType == __AT_RANDOM )
	{
		int nRand = rand() % (__AT_MAX_VALID_VALUE+1);
		ASSERT( nRand >= 0 && nRand <= __AT_MAX_VALID_VALUE );
		if( nRand == __AT_NONE || nRand == __AT_RANDOM )
			nRand = __AT_FADE; // most cool of all effects
		m_AnimationType = (e_animation_type_t)nRand;
	}

int nBitsPerPixel = CExtPaintManager::stat_GetBPP();
	if( nBitsPerPixel <= 8
		&&
		m_AnimationType != __AT_CONTENT_EXPAND
		&&
		m_AnimationType != __AT_CONTENT_DISPLAY
		)
	{
		m_AnimationType = __AT_NONE;
		m_bAnimFinished = true;
	}

	else
	{
		m_bAnimFinished =
			(m_AnimationType == __AT_NONE) ? true : false;
	}

	ASSERT(
			m_bmpScreenDst.GetSafeHandle() == NULL
		&&	m_bmpScreenSrc.GetSafeHandle() == NULL
		&&	m_bmpScreenTmp.GetSafeHandle() == NULL
		&&	m_clrFadeBitsSrc == NULL
		&&	m_clrFadeBitsDst == NULL
		&&	m_clrFadeBitsTmp == NULL
		);
}

void CExtPopupBaseWnd::_StartAnimation()
{
	ASSERT_VALID( this );
	_InitAnimation();
	if( m_AnimationType != __AT_NONE )
	{
		SetTimer( ID_TIMER_ANIMATION, ID_PERIOD_ANIMATION, NULL );
		g_nLastAnimTime = clock();
	}
}

void CExtPopupMenuWnd::_StartAnimation()
{
	ASSERT_VALID( this );
	if( _FindCustomizeMode() )
		return;
	_ItemFocusDelay();
	_CoolTipHide( false );
	CExtPopupBaseWnd::_StartAnimation();
	GetSite().SetAnimated(
		m_bAnimFinished ? NULL : this
		);
}

BEGIN_MESSAGE_MAP(CExtPopupBaseWnd, __BaseClassOfCExtPopupBaseWnd)
	//{{AFX_MSG_MAP(CExtPopupBaseWnd)
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_SIZE()
	ON_WM_ACTIVATEAPP()
	ON_WM_CANCELMODE()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExtPopupBaseWnd::OnPaint() 
{
	ASSERT_VALID( this );

	if( !(::IsWindow(GetSafeHwnd())) )
		return;
	if( !(::IsWindowVisible(GetSafeHwnd())) )
		return;
CPaintDC dc(this); // device context for painting
	CExtPaintManager::stat_ExcludeChildAreas(
		dc.GetSafeHdc(),
		m_hWnd
		);
	if( !m_bAnimFinished )
		_DrawAnimatedState( dc );
	else
		_DoPaint( dc );
}

void CExtPopupBaseWnd::_DrawAnimatedState( CDC & dc )
{
	ASSERT_VALID( this );
	_DoPaint( dc );
}

void CExtPopupMenuWnd::_DrawAnimatedState( CDC & dc )
{
	ASSERT_VALID( this );

	if( !(::IsWindow(GetSafeHwnd())) )
		return;
CRect rcClient;
	GetClientRect( &rcClient );

int cx = rcClient.Width();
int cy = rcClient.Height();

CDC * pPaintDC = &dc;

CPalette * pOldPalette = NULL;
    if( pPaintDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
    {
        pOldPalette =
			pPaintDC->SelectPalette( &g_PaintManager->m_PaletteWide, FALSE );
        pPaintDC->RealizePalette();
    }

CDC dcmm;
	VERIFY( dcmm.CreateCompatibleDC(pPaintDC) );
bool bFail = false;
	if( m_bmpScreenDst.GetSafeHandle() == NULL )
	{
		ASSERT( m_bmpScreenSrc.GetSafeHandle() == NULL );
		ASSERT( m_bmpScreenTmp.GetSafeHandle() == NULL );
		
		int nBitsPerPixel = CExtPaintManager::stat_GetBPP();
		CBitmap * pBmpOld = NULL;
		if(	(m_AnimationType == __AT_FADE
				&& nBitsPerPixel > 8 )
			|| m_AnimationType == __AT_CONTENT_EXPAND
			|| m_AnimationType == __AT_CONTENT_DISPLAY
			|| m_AnimationType == __AT_ROLL
			|| m_AnimationType == __AT_SLIDE
			|| m_AnimationType == __AT_ROLL_AND_STRETCH
			|| m_AnimationType == __AT_SLIDE_AND_STRETCH
			|| m_AnimationType == __AT_NOISE
			|| m_AnimationType == __AT_BOXES
			|| m_AnimationType == __AT_CIRCLES
			|| m_AnimationType == __AT_HOLES
			)
		{
			BITMAPINFOHEADER bih;
			bih.biSize = sizeof(BITMAPINFOHEADER);
			bih.biWidth = cx;
			bih.biHeight = cy;
			bih.biPlanes = 1;
			bih.biBitCount = 32;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = cx * cy;
			bih.biXPelsPerMeter = 0;
			bih.biYPelsPerMeter = 0;
			bih.biClrUsed = 0;
			bih.biClrImportant = 0;

			HBITMAP hDIB =
				::CreateDIBSection(
					dcmm.GetSafeHdc(),
					(LPBITMAPINFO)&bih,
					DIB_RGB_COLORS,
					(void **)&m_clrFadeBitsSrc,
					NULL,
					NULL
					);
			if( hDIB != NULL && m_clrFadeBitsSrc != NULL )
			{
				m_bmpScreenSrc.Attach( hDIB );

				hDIB =
					::CreateDIBSection(
						dcmm.GetSafeHdc(),
						(LPBITMAPINFO)&bih,
						DIB_RGB_COLORS,
						(void **)&m_clrFadeBitsDst,
						NULL,
						NULL
						);
				if( hDIB != NULL && m_clrFadeBitsDst != NULL )
				{
					m_bmpScreenDst.Attach( hDIB );

					hDIB =
						::CreateDIBSection(
							dcmm.GetSafeHdc(),
							(LPBITMAPINFO)&bih,
							DIB_RGB_COLORS,
							(void **)&m_clrFadeBitsTmp,
							NULL,
							NULL
							);
					if( hDIB != NULL && m_clrFadeBitsTmp != NULL )
					{
						m_bmpScreenTmp.Attach( hDIB );

						pBmpOld =
							dcmm.SelectObject( &m_bmpScreenSrc );
						dcmm.BitBlt(
							0, 0, cx, cy, pPaintDC,
							rcClient.left,
							rcClient.top,
							SRCCOPY
							);

						::memcpy(
							m_clrFadeBitsDst,
							m_clrFadeBitsSrc,
							sizeof(COLORREF)*cx*cy
							);
						dcmm.SelectObject( &m_bmpScreenDst );
					} // if( hDIB != NULL && m_clrFadeBitsTmp != NULL )
					else
					{
						m_bmpScreenSrc.DeleteObject();
						m_bmpScreenDst.DeleteObject();
						//return;
						bFail = true;
					} // else from if( hDIB != NULL && m_clrFadeBitsTmp != NULL )
				} // if( hDIB != NULL && m_clrFadeBitsDst != NULL )
				else
				{
					m_bmpScreenSrc.DeleteObject();
					//return;
					bFail = true;
				} // else from if( hDIB != NULL && m_clrFadeBitsDst != NULL )
			} // if( hDIB != NULL && m_clrFadeBitsSrc != NULL )
		}
		else
		{
			m_bmpScreenDst.CreateCompatibleBitmap(
				pPaintDC,
				cx,
				cy
				);
			pBmpOld =
				dcmm.SelectObject( &m_bmpScreenDst );
		}

		if( !bFail )
			_DoPaint( dcmm, false );
		dcmm.SetViewportOrg( CPoint(0,0) );
		dcmm.SelectObject( pBmpOld );
	} // if( m_bmpScreenDst.GetSafeHandle() == NULL )

CBitmap * pBmpOld = NULL;
	if( !bFail )
	{
		COLORREF * src = m_clrFadeBitsSrc;
		COLORREF * dst = m_clrFadeBitsDst;
		COLORREF * tmp = m_clrFadeBitsTmp;

		switch( m_AnimationType )
		{
		case __AT_BOXES:
		case __AT_CIRCLES:
		case __AT_HOLES:
			if( tmp!= NULL && src!=NULL && dst != NULL )
			{
				ASSERT(
					m_nAnimPercent >= 0 && m_nAnimPercent <= 100
					&&
					m_nAnimPercentOld >= 0 && m_nAnimPercentOld <= 100
					&&
					m_nAnimPercentOld <= m_nAnimPercent
					);
				ASSERT( __BOX_ANIM_METRIC > 0 ); // self assert
				if( m_nAnimPercentOld != m_nAnimPercent )
				{
					pBmpOld = dcmm.SelectObject( &m_bmpScreenTmp );
					if( m_AnimationType == __AT_BOXES )
					{ // do boxes animation
						for( int y=0; y<cy; y++ )
						{
							int _y0 = y % __BOX_ANIM_METRIC;
							int _yPercent = 100 -
								(_y0 * 100) / __BOX_ANIM_METRIC;
							if( _yPercent < m_nAnimPercent )
							{
								::memcpy(
									tmp,
									src,
									cx*sizeof(COLORREF)
									);
								tmp += cx;
								src += cx;
								dst += cx;
								continue;
							}
							for( int x=0; x<cx; x++ )
							{
								int _x0 = x % __BOX_ANIM_METRIC;
								int _xPercent =
									(_x0 * 100) / __BOX_ANIM_METRIC;
								if( _xPercent < m_nAnimPercent )
									*tmp = *dst;
								else
									*tmp = *src;
								tmp++; src++; dst++;
							}
						} // for( int y=0; y<cy; y++ )
					} // do boxes animation
					else
					{ // do circles/holes animation
						double radius = 0;
						if( m_AnimationType == __AT_HOLES )
							radius = 
								((((double)(__BOX_ANIM_METRIC)) / 2) * (100-m_nAnimPercent))
								/ 100;
						else
							radius = 
								((((double)(__BOX_ANIM_METRIC)) / 2) * m_nAnimPercent)
								/ 100;
						if( radius > 0 )
						{
							for( int y=0; y<cy; y++ )
							{
								int _y0 = y % __BOX_ANIM_METRIC;
								int _y1 = y - _y0;
								//int _y2 = _y1 + __BOX_ANIM_METRIC;
								int _yCenter = _y1 + __BOX_ANIM_METRIC/2;
								int _yDist = abs( y - _yCenter );
								double _yDist2 = (double)_yDist*_yDist;
								for( int x=0; x<cx; x++ )
								{
									int _x0 = x % __BOX_ANIM_METRIC;
									int _x1 = x - _x0;
									//int _x2 = _x1 + __BOX_ANIM_METRIC;
									int _xCenter = _x1 + __BOX_ANIM_METRIC/2;
									int _xDist = abs( x - _xCenter );
									double _xDist2 = (double)_xDist*_xDist;

									double nDispance =
										::sqrt(_yDist2+_xDist2);
									if( m_AnimationType == __AT_HOLES )
									{ // holes
										if( nDispance >= radius )
											*tmp = *dst;
										else
											*tmp = *src;
									} // holes
									else
									{ // circles
										if( nDispance < radius )
											*tmp = *dst;
										else
											*tmp = *src;
									} // circles
									tmp++; src++; dst++;
								}
							} // for( int y=0; y<cy; y++ )
						} // if( radius > 0 )
					} // do circles/holes animation

					int nMenuShadowSize = _GetPopupShadowSize();
					if( m_bCombineWithEA )
					{ // paint combined area AS IS
						ASSERT( !m_rcExcludeArea.IsRectEmpty() );
						CRect rcExcludeArea( m_rcExcludeArea );
						ScreenToClient( &rcExcludeArea );
						pBmpOld = dcmm.SelectObject( &m_bmpScreenDst );
						pPaintDC->BitBlt(
							rcExcludeArea.left, rcExcludeArea.top,
							rcExcludeArea.Width(), rcExcludeArea.Height(), 
							&dcmm,
							rcExcludeArea.left, rcExcludeArea.top,
							SRCCOPY
							);
						dcmm.SelectObject( pBmpOld );
						pBmpOld = NULL;
						if( m_eCombineAlign != __CMBA_NONE )
						{
//							int nMenuShadowSize = _GetPopupShadowSize();
							rcExcludeArea.InflateRect(
								(m_eCombineAlign == __CMBA_RIGHT) ?
									0 : nMenuShadowSize,
								(m_eCombineAlign == __CMBA_BOTTOM) ?
									0 : nMenuShadowSize,
								(m_eCombineAlign == __CMBA_LEFT) ?
									0 : nMenuShadowSize,
								(m_eCombineAlign == __CMBA_TOP) ?
									0 : nMenuShadowSize
								);
						} // if( m_eCombineAlign != __CMBA_NONE )
						pPaintDC->ExcludeClipRect(
							&rcExcludeArea
							);
					} // paint combined area AS IS
					if( nMenuShadowSize > 0 )
					{
						CRect rcExcludeArea( rcClient );
						rcExcludeArea.left =
							rcExcludeArea.right
							- nMenuShadowSize;
						pPaintDC->ExcludeClipRect(
							&rcExcludeArea
							);
						rcExcludeArea = rcClient ;
						rcExcludeArea.top =
							rcExcludeArea.bottom
							- nMenuShadowSize;
						pPaintDC->ExcludeClipRect(
							&rcExcludeArea
							);
					}
					
					pPaintDC->BitBlt(
						rcClient.left, rcClient.top, cx, cy, 
						&dcmm,
						0,
						0,
						SRCCOPY
						);

					if( nMenuShadowSize > 0 || m_bCombineWithEA )
						pPaintDC->SelectClipRgn( NULL );
				} // if( m_nAnimPercentOld != m_nAnimPercent )
			}
		break; // cases __AT_BOXES,__AT_CIRCLES,__AT_HOLES
		case __AT_NOISE:
			if( tmp!= NULL && src!=NULL && dst != NULL )
			{
				ASSERT(
					m_nAnimPercent >= 0 && m_nAnimPercent <= 100
					&&
					m_nAnimPercentOld >= 0 && m_nAnimPercentOld <= 100
					&&
					m_nAnimPercentOld <= m_nAnimPercent
					);
				if( m_nAnimPercentOld != m_nAnimPercent )
				{
					int nDiff =
						m_nAnimPercent - m_nAnimPercentOld;
					ASSERT( nDiff > 0 );
					int nRandHalf = nDiff / 2 + 1;
					ASSERT( nRandHalf > 0 );
					pBmpOld = dcmm.SelectObject( &m_bmpScreenTmp );
					for( int pixel = 0; pixel < cx * cy; pixel++ )
					{
						int nRandValue = 
							rand() % nDiff;
						if( nRandValue > nRandHalf )
							*tmp = *dst;
//						else
//							*tmp = *src;
						tmp++; src++; dst++;
					} // for( int pixel = 0; pixel < cx * cy; pixel++ )

					int nMenuShadowSize = _GetPopupShadowSize();
					if( m_bCombineWithEA )
					{ // paint combined area AS IS
						ASSERT( !m_rcExcludeArea.IsRectEmpty() );
						CRect rcExcludeArea( m_rcExcludeArea );
						ScreenToClient( &rcExcludeArea );
						pBmpOld = dcmm.SelectObject( &m_bmpScreenDst );
						pPaintDC->BitBlt(
							rcExcludeArea.left, rcExcludeArea.top,
							rcExcludeArea.Width(), rcExcludeArea.Height(), 
							&dcmm,
							rcExcludeArea.left, rcExcludeArea.top,
							SRCCOPY
							);
						dcmm.SelectObject( pBmpOld );
						pBmpOld = NULL;
						if( m_eCombineAlign != __CMBA_NONE )
						{
//							int nMenuShadowSize = _GetPopupShadowSize();
							rcExcludeArea.InflateRect(
								(m_eCombineAlign == __CMBA_RIGHT) ?
									0 : nMenuShadowSize,
								(m_eCombineAlign == __CMBA_BOTTOM) ?
									0 : nMenuShadowSize,
								(m_eCombineAlign == __CMBA_LEFT) ?
									0 : nMenuShadowSize,
								(m_eCombineAlign == __CMBA_TOP) ?
									0 : nMenuShadowSize
								);
						} // if( m_eCombineAlign != __CMBA_NONE )
						pPaintDC->ExcludeClipRect(
							&rcExcludeArea
							);
					} // paint combined area AS IS
					if( nMenuShadowSize > 0 )
					{
						CRect rcExcludeArea( rcClient );
						rcExcludeArea.left =
							rcExcludeArea.right
							- nMenuShadowSize;
						pPaintDC->ExcludeClipRect(
							&rcExcludeArea
							);
						rcExcludeArea = rcClient ;
						rcExcludeArea.top =
							rcExcludeArea.bottom
							- nMenuShadowSize;
						pPaintDC->ExcludeClipRect(
							&rcExcludeArea
							);
					}
					
					pPaintDC->BitBlt(
						rcClient.left, rcClient.top, cx, cy, 
						&dcmm,
						0,
						0,
						SRCCOPY
						);

					if( nMenuShadowSize > 0 || m_bCombineWithEA )
						pPaintDC->SelectClipRgn( NULL );
				} // if( m_nAnimPercentOld != m_nAnimPercent )
			}
		break; // case __AT_NOISE
		case __AT_SLIDE:
		case __AT_ROLL:
		case __AT_ROLL_AND_STRETCH:
		case __AT_SLIDE_AND_STRETCH:
			if( tmp!= NULL && src!=NULL && dst != NULL )
			{
				ASSERT(
					m_nAnimPercent >= 0 && m_nAnimPercent <= 100
					&&
					m_nAnimPercentOld >= 0 && m_nAnimPercentOld <= 100
					&&
					m_nAnimPercentOld <= m_nAnimPercent
					);

				CRect _rcClient;
				_GetClientRect( &_rcClient );

				if( m_bCombineWithEA )
				{ // paint combined area AS IS
					ASSERT( !m_rcExcludeArea.IsRectEmpty() );
					CRect rcExcludeArea( m_rcExcludeArea );
					ScreenToClient( &rcExcludeArea );
					pBmpOld = dcmm.SelectObject( &m_bmpScreenDst );
					pPaintDC->BitBlt(
						rcExcludeArea.left, rcExcludeArea.top,
						rcExcludeArea.Width(), rcExcludeArea.Height(), 
						&dcmm,
						rcExcludeArea.left, rcExcludeArea.top,
						SRCCOPY
						);
					dcmm.SelectObject( pBmpOld );
					pBmpOld = NULL;
					if( m_eCombineAlign != __CMBA_NONE )
					{
						int nMenuShadowSize = _GetPopupShadowSize();
						rcExcludeArea.InflateRect(
							(m_eCombineAlign == __CMBA_RIGHT) ?
								0 : nMenuShadowSize,
							(m_eCombineAlign == __CMBA_BOTTOM) ?
								0 : nMenuShadowSize,
							(m_eCombineAlign == __CMBA_LEFT) ?
								0 : nMenuShadowSize,
							(m_eCombineAlign == __CMBA_TOP) ?
								0 : nMenuShadowSize
							);
					} // if( m_eCombineAlign != __CMBA_NONE )
					pPaintDC->ExcludeClipRect(
						&rcExcludeArea
						);
				} // paint combined area AS IS

				pBmpOld = dcmm.SelectObject( &m_bmpScreenDst );
				int cx =
					_rcClient.Width();
				int cy =
					_rcClient.Height();
				if( m_AnimationType == __AT_ROLL
					||
					m_AnimationType == __AT_SLIDE
					)
				{ // non-stratchable variants
					int _cx = 0;
					if( m_AnimationType == __AT_ROLL )
						_cx = (cx * (100-m_nAnimPercent)) / 100;
					int _cy =
						(cy * (100-m_nAnimPercent)) / 100;
					if( m_eCombineAlign == __CMBA_RIGHT )
					{
						CRgn rgnClip;
						if( rgnClip.CreateRectRgnIndirect( &_rcClient ) )
							pPaintDC->SelectClipRgn( &rgnClip );
						pPaintDC->BitBlt(
							_rcClient.left+_cx, _rcClient.top-_cy, cx, cy, 
							&dcmm,
							_rcClient.left, _rcClient.top,
							SRCCOPY
							);
						pPaintDC->SelectClipRgn( NULL );
					} // if m_eCombineAlign is __CMBA_RIGHT
					else if( m_eCombineAlign == __CMBA_BOTTOM )
					{
						CRgn rgnClip;
						if( rgnClip.CreateRectRgnIndirect( &_rcClient ) )
							pPaintDC->SelectClipRgn( &rgnClip );
						pPaintDC->BitBlt(
							_rcClient.left-_cx, _rcClient.top+_cy, cx, cy,
							&dcmm,
							_rcClient.left, _rcClient.top,
							SRCCOPY
							);
						pPaintDC->SelectClipRgn( NULL );
					} // if m_eCombineAlign is __CMBA_BOTTOM
					else
					{ // if m_eCombineAlign is __CMBA_TOP or __CMBA_LEFT
						pPaintDC->BitBlt(
							_rcClient.left-_cx, _rcClient.top-_cy, cx, cy,
							&dcmm,
							_rcClient.left, _rcClient.top,
							SRCCOPY
							);
					} // if m_eCombineAlign is __CMBA_TOP or __CMBA_LEFT
				} // non-stratchable variants
				else
				{ // stratchable variants
					int _cx = cx;
					if( m_AnimationType == __AT_ROLL_AND_STRETCH )
						_cx = (cx * m_nAnimPercent) / 100;
					int _cy =
						(cy * m_nAnimPercent) / 100;
					int nOldStretchBltMode =
						pPaintDC->SetStretchBltMode(
							g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
							);
					if(		m_eCombineAlign == __CMBA_RIGHT
						&&	m_AnimationType == __AT_ROLL_AND_STRETCH
						)
					{
						CRgn rgnClip;
						if( rgnClip.CreateRectRgnIndirect( &_rcClient ) )
							pPaintDC->SelectClipRgn( &rgnClip );
						pPaintDC->StretchBlt(
							_rcClient.left+cx-_cx, _rcClient.top, _cx, _cy,
							&dcmm,
							_rcClient.left, _rcClient.top, cx, cy, 
							SRCCOPY
							);
						pPaintDC->SelectClipRgn( NULL );
					} // if m_eCombineAlign is __CMBA_RIGHT with __AT_ROLL_AND_STRETCH animation
					else if( m_eCombineAlign == __CMBA_BOTTOM )
					{
						CRgn rgnClip;
						if( rgnClip.CreateRectRgnIndirect( &_rcClient ) )
							pPaintDC->SelectClipRgn( &rgnClip );
						pPaintDC->StretchBlt(
							_rcClient.left-cx+_cx, _rcClient.top+cy-_cy, _cx, _cy,
							&dcmm,
							_rcClient.left, _rcClient.top, cx, cy, 
							SRCCOPY
							);
						pPaintDC->SelectClipRgn( NULL );
					} // if m_eCombineAlign is __CMBA_BOTTOM
					else
					{ // if m_eCombineAlign is __CMBA_TOP or __CMBA_LEFT
						pPaintDC->StretchBlt(
							_rcClient.left, _rcClient.top, _cx, _cy,
							&dcmm,
							_rcClient.left, _rcClient.top, cx, cy, 
							SRCCOPY
							);
					} // if m_eCombineAlign is __CMBA_TOP or __CMBA_LEFT
					pPaintDC->SetStretchBltMode( nOldStretchBltMode );
				} // stratchable variants

				if( m_bCombineWithEA )
					pPaintDC->SelectClipRgn( NULL );
			}
		break; // cases __AT_ROLL, __AT_SLIDE, __AT_ROLL_AND_STRETCH, __AT_ROLL_AND_STRETCH

		case __AT_CONTENT_EXPAND:
			if( tmp!= NULL && src!=NULL && dst != NULL )
			{
				ASSERT(
					m_nAnimPercent >= 0 && m_nAnimPercent <= 100
					&&
					m_nAnimPercentOld >= 0 && m_nAnimPercentOld <= 100
					&&
					m_nAnimPercentOld <= m_nAnimPercent
					);

				CRect _rcClient;
				_GetClientRect( &_rcClient );

				if( m_bCombineWithEA )
				{ // paint combined area AS IS
					ASSERT( !m_rcExcludeArea.IsRectEmpty() );
					CRect rcExcludeArea( m_rcExcludeArea );
					ScreenToClient( &rcExcludeArea );
					pBmpOld = dcmm.SelectObject( &m_bmpScreenDst );
					pPaintDC->BitBlt(
						rcExcludeArea.left, rcExcludeArea.top,
						rcExcludeArea.Width(), rcExcludeArea.Height(), 
						&dcmm,
						rcExcludeArea.left, rcExcludeArea.top,
						SRCCOPY
						);
					dcmm.SelectObject( pBmpOld );
					pBmpOld = NULL;
				} // paint combined area AS IS

				pBmpOld = dcmm.SelectObject( &m_bmpScreenDst );
				int cx =
					_rcClient.Width();
				int cy =
					_rcClient.Height();
				int _cx = cx;
				int _cy =
					(cy * m_nAnimPercent) / 100;

				visible_items_t v;
					_GetVisibleItems(
						dcmm.GetSafeHdc(), // dc.GetSafeHdc()
						v
						);
				if( v.GetSize() == 0 )
				{ // if menu is empty
					int nOldStretchBltMode =
						pPaintDC->SetStretchBltMode(
							g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
							);
					pPaintDC->StretchBlt(
						_rcClient.left, _rcClient.top, _cx, _cy, 
						&dcmm,
						_rcClient.left, _rcClient.top, cx, cy,
						SRCCOPY
						);
					pPaintDC->SetStretchBltMode( nOldStretchBltMode );
				} // if menu is empty
				else
				{ // if menu is NOT empty
					int y_pos_src = _rcClient.top;
					int y_pos_dst = _rcClient.top;
					INT vis_iter = 0;
					int nItemIndex = 0;
					int nItemCount = v.GetSize();
					ASSERT( nItemCount > 0 );
					expand_effect_rects_container_t vRects;
					
					for( ; vis_iter < v.GetSize(); ++vis_iter, ++nItemIndex )
					{ // calc rects for visible items
						VisibleItemDefinition_t & vi = v[vis_iter];
#ifdef _DEBUG
						ASSERT(
							vi.m_nIndex >= 0
							&&
							vi.m_nIndex < m_items_all.GetSize()
							);
//						ASSERT( !vi.m_rcItem.IsRectEmpty() );
//						ASSERT( dc.RectVisible(&vi.m_rcItem) );
						MENUITEMDATA & mi =
							m_items_all[vi.m_nIndex];
						ASSERT( mi.IsDisplayed() );
						ASSERT( mi.GetCmdID() == vi.m_nHelperCmdID );
#endif // _DEBUG

						CRect rcItemSrc( vi.m_rcItem );
						rcItemSrc.left = _rcClient.left;
						rcItemSrc.right = _rcClient.right;
						if( nItemIndex == 0 )
							rcItemSrc.top = _rcClient.top;
						else if( nItemIndex == (nItemCount-1) )
							rcItemSrc.bottom = _rcClient.bottom;

						int y_pos_src_next = 
							y_pos_src + rcItemSrc.Height();
						CRect rcItemDst( rcItemSrc );
						rcItemDst.OffsetRect(
							0,
							y_pos_dst - rcItemDst.top
							);
						if( vi.m_bRarelyUsed )
							rcItemDst.bottom =
								rcItemDst.top
								+
								(rcItemDst.Height() * m_nAnimPercent)
									/ 100;
						int y_pos_dst_next = 
							y_pos_dst + rcItemDst.Height();
						
						ExpandEffectRects_t _eert(
							rcItemSrc,
							rcItemDst
							);
						vRects.Add( _eert );
						y_pos_dst = y_pos_dst_next;
						y_pos_src = y_pos_src_next;
					} // calc rects for visible items

					// y_pos_dst is now equal to size of
					// all compressed itsms
					ASSERT( y_pos_dst <= y_pos_src );
					if( m_bCombineWithEA
						&&
						m_eCombineAlign == __CMBA_BOTTOM
						&&
						y_pos_dst != y_pos_src
						)
					{ // expand from bottom to top
						INT iter_rects = 0;
						int nDstOffset =
							y_pos_src - y_pos_dst;
						for( ; iter_rects < vRects.GetSize(); ++iter_rects )
						{ // adjust destination rects
							ExpandEffectRects_t & eerc =
								vRects[ iter_rects ];
							eerc.m_rcDst.OffsetRect(
								0,
								nDstOffset
								);
						} // adjust destination rects
					} // expand from bottom to top

					INT iter_rects = 0;
					for( ; iter_rects < vRects.GetSize(); ++iter_rects )
					{ // paint visible items
						ExpandEffectRects_t & eerc =
							vRects[ iter_rects ];
						if( eerc.m_rcDst.IsRectEmpty() )
							continue;
						if( g_bUseStretchOnExpandAnimation )
						{
							int nOldStretchBltMode =
								pPaintDC->SetStretchBltMode(
									g_PaintManager.m_bIsWinNT ? HALFTONE : COLORONCOLOR
									);
							// something better then Office XP
							pPaintDC->StretchBlt(
								eerc.m_rcDst.left, eerc.m_rcDst.top,
								eerc.m_rcDst.Width(), eerc.m_rcDst.Height(), 
								&dcmm,
								eerc.m_rcSrc.left, eerc.m_rcSrc.top,
								eerc.m_rcSrc.Width(), eerc.m_rcSrc.Height(), 
								SRCCOPY
								);
							pPaintDC->SetStretchBltMode( nOldStretchBltMode );
						} // if( g_bUseStretchOnExpandAnimation )
						else
							// somthing really like Office XP
							pPaintDC->BitBlt(
								eerc.m_rcDst.left, eerc.m_rcDst.top,
								eerc.m_rcDst.Width(), eerc.m_rcDst.Height(), 
								&dcmm,
								eerc.m_rcSrc.left, eerc.m_rcSrc.top,
								SRCCOPY
								);
					} // paint visible items
				} // if menu is NOT empty

			}
		break; // case __AT_CONTENT_EXPAND

		case __AT_CONTENT_DISPLAY:
			if( tmp!= NULL && src!=NULL && dst != NULL )
			{
				pBmpOld = dcmm.SelectObject( &m_bmpScreenDst );
				pPaintDC->BitBlt(
					rcClient.left, rcClient.top, cx, cy, 
					&dcmm,
					0,
					0,
					SRCCOPY
					);
			}
		break; // case __AT_CONTENT_DISPLAY

		case __AT_FADE:
			if( tmp!= NULL && src!=NULL && dst != NULL )
			{
				ASSERT(
					m_nAnimPercent >= 0 && m_nAnimPercent <= 100
					&&
					m_nAnimPercentOld >= 0 && m_nAnimPercentOld <= 100
					&&
					m_nAnimPercentOld <= m_nAnimPercent
					);
				pBmpOld = dcmm.SelectObject( &m_bmpScreenTmp );
				for( int pixel = 0; pixel < cx * cy; pixel++ )
				{
					COLORREF c0 = *dst++;
					COLORREF c1 = *src++;
					*tmp++ =
						RGB(
							(m_nAnimPercent*long(GetRValue(c0)) + (100L-m_nAnimPercent)*long(GetRValue(c1)) ) / 100L,
							(m_nAnimPercent*long(GetGValue(c0)) + (100L-m_nAnimPercent)*long(GetGValue(c1)) ) / 100L,
							(m_nAnimPercent*long(GetBValue(c0)) + (100L-m_nAnimPercent)*long(GetBValue(c1)) ) / 100L
							);
				} // for( int pixel = 0; pixel < cx * cy; pixel++ )
				pPaintDC->BitBlt(
					rcClient.left, rcClient.top, cx, cy, 
					&dcmm,
					0,
					0,
					SRCCOPY
					);
			}
		break; // case __AT_FADE

#ifdef _DEBUG
		default:
			ASSERT( FALSE );
		break; // default
#endif // _DEBUG
		} // switch( m_AnimationType )
	} // if( !bFail )

	if( pBmpOld != NULL )
		dcmm.SelectObject( pBmpOld );

	if( pOldPalette != NULL )
		pPaintDC->SelectPalette( pOldPalette, FALSE );
}

void CExtPopupBaseWnd::_DoPaint( CDC & dcPaint, bool bUseBackBuffer /*= true*/ )
{
	ASSERT_VALID( this );

	dcPaint;
	bUseBackBuffer;
}

void CExtPopupBaseWnd::OnTimer(UINT nIDEvent) 
{
	ASSERT_VALID( this );

	switch( nIDEvent )
	{
	case ID_TIMER_DELAY_SHOW:
	{
		VERIFY( KillTimer(ID_TIMER_DELAY_SHOW) );
		ShowWindow( SW_SHOWNA );
		Invalidate( FALSE );
		UpdateWindow();
		return;
	}
	// case ID_TIMER_DELAY_SHOW

	case ID_TIMER_ANIMATION:
		if( !m_bAnimFinished )
		{
			clock_t nCurrAnimTime = clock();
			int nDuration =
				nCurrAnimTime - g_nLastAnimTime;
			int nSteps = (int)
				(0.5 + (float) nDuration / ID_PERIOD_ANIMATION);
			switch( m_AnimationType )
			{
			case __AT_CONTENT_DISPLAY:
				m_bAnimFinished = true;
			break;
			case __AT_FADE:
			case __AT_CONTENT_EXPAND:
			case __AT_ROLL:
			case __AT_SLIDE:
			case __AT_ROLL_AND_STRETCH:
			case __AT_SLIDE_AND_STRETCH:
			case __AT_NOISE:
			case __AT_BOXES:
			case __AT_CIRCLES:
			case __AT_HOLES:
				m_nAnimPercentOld = m_nAnimPercent;
				m_nAnimPercent += g_nAnimStepMetric;
				if( m_nAnimPercent >
						100 + nSteps * g_nAnimStepMetric
					)
					m_nAnimPercent = 101;
				if( m_nAnimPercent > 100 )
					m_bAnimFinished = true;
			break;
#ifdef _DEBUG
			default:
				ASSERT( FALSE );
			break;
#endif // _DEBUG
			} // switch( m_AnimationType )

			if( m_bAnimFinished )
			{
				if( !_FindCustomizeMode() )
				{
					g_SoundPlayer->PlaySound(
						m_ePlaySoundOnAnimationFinished
						);
					m_ePlaySoundOnAnimationFinished =
						CExtSoundPlayer::__NO_SOUND;
				} // if( !_FindCustomizeMode() )
				KillTimer(ID_TIMER_ANIMATION);
				m_AnimationType = g_DefAnimationType;
			} // if( m_bAnimFinished )

			if( GetSafeHwnd() != NULL )
			{
				if( m_bAnimFinished )
					_EndAnimation();
				else
					Invalidate( FALSE );
				g_nLastAnimTime = nCurrAnimTime;
			}
		} // if( !m_bAnimFinished )
		
		if( m_bAnimFinished )
			if( IsKindOf(RUNTIME_CLASS(CExtPopupMenuWnd)) )
				((CExtPopupMenuWnd *)this)->
					GetSite().SetAnimated( NULL );

		return;
	// case ID_TIMER_ANIMATION
	default:
		__BaseClassOfCExtPopupBaseWnd::OnTimer(nIDEvent);
	break; // default
	} // switch( nIDEvent )
}

int CExtPopupBaseWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	ASSERT_VALID( this );

	pDesktopWnd;
	nHitTest;
	message;
	if( _FindCustomizeMode() )
	{
		if( IsKindOf(RUNTIME_CLASS(CExtPopupMenuWnd)) )
		{
///			((CExtPopupMenuWnd*)this)->_ItemFocusCancel( FALSE );
			((CExtPopupMenuWnd*)this)->_DoResetChainPositions();
		} // if( IsKindOf(RUNTIME_CLASS(CExtPopupMenuWnd)) )
		else
			SetWindowPos(
				&CWnd::wndTopMost,
				-1, -1, -1, -1,
				SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE
				);
		return MA_NOACTIVATE;
	}

HWND hWndInPlace = ::GetWindow( m_hWnd, GW_CHILD );
	if( hWndInPlace != NULL )
	{
		CPoint ptScreenClick;
		if( ! ::GetCursorPos(&ptScreenClick) )
			return MA_NOACTIVATEANDEAT;
		HWND hWndFromPoint = ::WindowFromPoint( ptScreenClick );
		if(		hWndFromPoint != NULL
			&&	(::GetWindowLong(hWndFromPoint,GWL_STYLE)&WS_CHILD) != 0
			&&	::GetParent(hWndFromPoint) == m_hWnd
			)
		return MA_ACTIVATE;
	} // if( hWndInPlace != NULL )

	return MA_NOACTIVATEANDEAT;
	//return CExtPopupBaseWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CExtPopupMenuWnd::_DoShowChain(
	int nCmdShow // = SW_SHOWNA
	)
{
	ASSERT_VALID( this );
	if( m_hWnd == NULL || (! ::IsWindow(m_hWnd) ) )
		return;
	ShowWindow( nCmdShow );
	if( m_nCurIndex < 0 )
		return;
MENUITEMDATA & mi = _GetItemRef( m_nCurIndex );
	if( !mi.IsPopup() )
		return;
	mi.GetPopup()->_DoShowChain( nCmdShow );
}

void CExtPopupMenuWnd::_DoResetChainPositions()
{
	ASSERT_VALID( this );
CExtPopupMenuWnd * pChild = NULL;
	if( m_nCurIndex >= 0 )
	{
		MENUITEMDATA & mi = _GetItemRef( m_nCurIndex );
		if( mi.IsPopup() )
		{
			pChild = mi.GetPopup();
			if(		pChild->GetSafeHwnd() == NULL
				||	(! pChild->IsWindowVisible() )
				)
				pChild = NULL;
		} // if( mi.IsPopup() )
	} // if( m_nCurIndex >= 0 )
	if( pChild != NULL )
		pChild->_DoResetChainPositions();
	_DoResetChainPositionsImpl( NULL );
}

void CExtPopupMenuWnd::_DoResetChainPositionsImpl(
	CExtPopupMenuWnd * pPrev
	)
{
	SetWindowPos(
		( pPrev == NULL ) ? (&CWnd::wndTopMost) : pPrev,
		0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE
			|SWP_NOREDRAW
		);
	{ // BLOCK : DC painting
		CClientDC dc( this );
		_DoPaint( dc );
	} // BLOCK : DC painting
	if( m_pWndParentMenu != NULL )
		m_pWndParentMenu->_DoResetChainPositionsImpl( this );
}

BOOL CExtPopupBaseWnd::OnEraseBkgnd(CDC* pDC) 
{
	ASSERT_VALID( this );

	pDC;
	return TRUE;
}

void CExtPopupBaseWnd::OnNcPaint() 
{
	ASSERT_VALID( this );
}

void CExtPopupBaseWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	ASSERT_VALID( this );

	bCalcValidRects;
	lpncsp;
///	__BaseClassOfCExtPopupBaseWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CExtPopupBaseWnd::OnSize(UINT nType, int cx, int cy) 
{
	ASSERT_VALID( this );

	__BaseClassOfCExtPopupBaseWnd::OnSize(nType, cx, cy);
	Invalidate(FALSE);
}

BOOL CExtPopupBaseWnd::DestroyWindow()
{
	ASSERT_VALID( this );
	if( !(::IsWindow(GetSafeHwnd())) )
		return TRUE;
	return __BaseClassOfCExtPopupBaseWnd::DestroyWindow();
}

void CExtPopupBaseWnd::PostNcDestroy() 
{
	ASSERT_VALID( this );
}

void CExtPopupMenuWnd::PostNcDestroy() 
{
	ASSERT_VALID( this );
	if( _FindCustomizeMode() )
		return;
	if( m_bTopLevel )
	{
		ASSERT( m_hWndCmdReciever != NULL );
		if( ::IsWindow(m_hWndCmdReciever) )
			::SendMessage(
				m_hWndCmdReciever,
				g_nMsgNotifyMenuClosed,
				0,
				LPARAM( this )
				);
		CExtPopupMenuSite & _site = GetSite();
		ASSERT( !_site.IsEmpty() );
		if( //!_site.IsEmpty() &&
			!_site.IsShutdownMode()
			)
		{
			_site.DoneInstance();
			ASSERT( _site.IsEmpty() );
			ASSERT( !_site.IsShutdownMode() );
		}
	} // if( m_bTopLevel )
}

bool CExtPopupBaseWnd::_FindHelpMode() const
{
//	if( ::AfxGetApp()->m_bHelpMode )
//		return true;
	return false;
}

bool CExtPopupMenuWnd::_FindHelpMode() const
{
	if( (m_dwTrackFlags&TPMX_HELP_CTX_MODE) != 0 )
		return true;
	if( m_pWndParentMenu != NULL )
		return m_pWndParentMenu->_FindHelpMode();
	return CExtPopupBaseWnd::_FindHelpMode();
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * CExtPopupMenuWnd::_FindCustomizeSite() const
{
	if( m_pCustomizeSite != NULL )
		return (CExtCustomizeSite *)m_pCustomizeSite;
	if( m_pWndParentMenu != NULL )
		return m_pWndParentMenu->_FindCustomizeSite();
	return NULL;
}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

bool CExtPopupBaseWnd::_FindCustomizeMode() const
{
	return false;
}

bool CExtPopupMenuWnd::_FindCustomizeMode() const
{
	if( (m_dwTrackFlags&TPMX_CUSTOMIZE_MODE) != 0 )
		return true;
	if( m_pWndParentMenu != NULL )
		return m_pWndParentMenu->_FindCustomizeMode();
	return false;
}

BOOL CExtPopupBaseWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	ASSERT_VALID( this );
	if( _FindHelpMode() )
	{
		SetCursor( afxData.hcurHelp );
		return TRUE;
	}
	return __BaseClassOfCExtPopupBaseWnd::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuTipWnd

IMPLEMENT_DYNCREATE(CExtPopupMenuTipWnd, CExtPopupBaseWnd)

CExtPopupMenuTipWnd::CExtPopupMenuTipWnd()
{
	m_hIcon = NULL;
	m_IconSize.cx = m_IconSize.cy = 0;
	m_sText = _T("");
	m_bFlipHorz = false;
	m_bFlipVert = false;

	m_hIcon = ::LoadIcon( NULL, IDI_INFORMATION );
	m_IconSize.cx = m_IconSize.cy = 16;

	m_bAnimFinished = true;
	m_AnimationType = __AT_NONE;
}

CExtPopupMenuTipWnd::~CExtPopupMenuTipWnd()
{
	if( m_hIcon != NULL )
	{
		::DestroyIcon( m_hIcon );
		m_hIcon = NULL;
	}
}

BEGIN_MESSAGE_MAP(CExtPopupMenuTipWnd, CExtPopupBaseWnd)
	//{{AFX_MSG_MAP(CExtPopupMenuTipWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define CX_ROUNDED				12		// Tip horizontal roundness
#define CY_ROUNDED				10		// Tip vertical roundness
#define CX_LEADER				25		// Width of tip lead
#define CY_LEADER				25		// Height of tip lead
#define CX_ICON_MARGIN			5		// Width of margin between icon and tip text

#define __TIP_OVER_METRIC 12

bool CExtPopupMenuTipWnd::GetWindowRegion(
	CDC* pDC,
	HRGN* hRegion,
	CSize *Size /* = NULL */
	)
{
	ASSERT_VALID( this );

CRect	rcWnd(0,0,0,0);
POINT	ptLeader[3];
CRgn	LeaderRegion;
CRgn	CaptionRegion;
CFont	*pSysFont;
	
	ASSERT( pDC != NULL );
	ASSERT( hRegion != NULL );

	// Calculate the are for the tip text
	pSysFont =
		(CFont *)pDC->SelectObject(
			&(g_PaintManager->m_FontNormal)
			);
	pDC->DrawText(
		m_sText,
		m_sText.GetLength(),
		&rcWnd,
		DT_CALCRECT
		);
	pDC->SelectObject( pSysFont );

	// Adjust for the rounded corners
	rcWnd.InflateRect(CX_ROUNDED, CY_ROUNDED);

	// Adjust for icon
	if( m_hIcon != NULL )
		rcWnd.right = rcWnd.right + m_IconSize.cx + CX_ICON_MARGIN;
	if( rcWnd.Height() < m_IconSize.cy )
		rcWnd.bottom = rcWnd.top + m_IconSize.cy;

	// Calculate the leader triangle coordinates

	ptLeader[0].x = rcWnd.Width() - CX_ROUNDED;
	ptLeader[0].y = rcWnd.Height() - CY_ROUNDED;

	ptLeader[1].x = ptLeader[0].x;
	ptLeader[1].y = ptLeader[0].y + CY_LEADER;

	ptLeader[2].x = ptLeader[0].x - CX_LEADER;
	ptLeader[2].y = rcWnd.Height() - CY_ROUNDED;

CRect rcRoundRectRgn(CPoint(0,0),rcWnd.Size());
CRect rcRectRgn( 0, 0, rcWnd.Width(), rcWnd.Height() + CY_LEADER );
	m_bFlipHorz = false;
	m_bFlipVert = false;

CWnd * pWndSearchMon = GetParent();
	ASSERT_VALID( pWndSearchMon );
	ASSERT( pWndSearchMon->GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( pWndSearchMon->GetSafeHwnd() ) );
CExtPaintManager::monitor_parms_t _mp;
	CExtPaintManager::stat_GetMonitorParms( _mp, pWndSearchMon );
CRect rcDesktop = g_bUseDesktopWorkArea
		? _mp.m_rcWorkArea
		: _mp.m_rcMonitor
		;

	if( m_ptTrack.x - rcWnd.Width() < rcDesktop.left )
	{
		m_bFlipHorz = true;
		ptLeader[0].x = CX_ROUNDED;
		ptLeader[0].y = rcWnd.Height() - CY_ROUNDED;
		ptLeader[1].x = ptLeader[0].x;
		ptLeader[1].y = ptLeader[0].y + CY_LEADER;
		ptLeader[2].x = ptLeader[0].x + CX_LEADER;
		ptLeader[2].y = ptLeader[0].y;
	}

	if( m_ptTrack.y - rcWnd.Height() - CY_ROUNDED*2 < rcDesktop.top )
	{
		m_bFlipVert = true;
		ptLeader[0].y -= rcWnd.Height() - CY_ROUNDED*2;
		ptLeader[1].y = ptLeader[0].y - CY_LEADER;
		ptLeader[2].y -= ptLeader[0].y;
	}

	// Create the caption region
	CaptionRegion.CreateRoundRectRgn(
		rcRoundRectRgn.left,rcRoundRectRgn.top,rcRoundRectRgn.right,rcRoundRectRgn.bottom,
		CX_ROUNDED, CY_ROUNDED
		);
	// Create the leader region
	LeaderRegion.CreatePolygonRgn(ptLeader, 3, ALTERNATE);
	// Create window region
	*hRegion =  ::CreateRectRgn(
		rcRectRgn.left,rcRectRgn.top,rcRectRgn.right,rcRectRgn.bottom
		);
	// Combine the regions
	CombineRgn(*hRegion, CaptionRegion.operator HRGN(), LeaderRegion.operator HRGN(), RGN_OR);

	if( m_bFlipVert )
		OffsetRgn(*hRegion,0,CY_LEADER);

	// Set the window size
	if (Size != NULL)
	{
		Size->cx	= rcWnd.Width();
		Size->cy	= rcWnd.Height() + CY_LEADER;
	}

	return true;
}

void CExtPopupMenuTipWnd::_DoPaint( CDC & dcPaint, bool bUseBackBuffer /*= true*/ )
{
	ASSERT_VALID( this );

	bUseBackBuffer;
CDC & dc = dcPaint;

CRect	rc;
CBrush	WindowBrush;
CBrush	FrameBrush;
CBrush	InnerFrameBrush;
HRGN	hRegion;
CRgn	*pRegion;
CFont	*pSysFont;

	// Get the client rectangle
	GetClientRect(rc);

	// Create the brushes
	InnerFrameBrush.CreateSolidBrush(
		g_PaintManager->GetColor(COLOR_3DFACE)
		);
	FrameBrush.CreateSolidBrush(
		g_PaintManager->GetColor(COLOR_3DDKSHADOW)
		);
	WindowBrush.CreateSolidBrush(
		g_PaintManager->GetColor(COLOR_INFOBK)
		);

	// Get the window region
	hRegion = CreateRectRgnIndirect(&rc);
	GetWindowRgn(hRegion);
	pRegion = CRgn::FromHandle(hRegion);

	// Draw the frame
	dc.FillRgn(pRegion, &WindowBrush);
	dc.FrameRgn(pRegion, &InnerFrameBrush, 3, 3);
	dc.FrameRgn(pRegion, &FrameBrush, 1, 1);

	// Adjust the area for the icon
	rc.DeflateRect(CX_ROUNDED, CY_ROUNDED, 0, 0);
	if( m_hIcon != NULL )
		rc.left += m_IconSize.cx + CX_ICON_MARGIN;
	
	// Set the font
	pSysFont = (CFont *)dc.SelectObject(
		&(g_PaintManager->m_FontNormal)
		);
	// Draw the tip text	
	dc.SetBkMode( TRANSPARENT );

int nIconVertPos = CY_ROUNDED;
	if( m_bFlipVert )
	{
		rc.OffsetRect(0,CY_LEADER);
		nIconVertPos += CY_LEADER;
	}

COLORREF clrOltTextColor =
		dc.SetTextColor(
			g_PaintManager->GetColor(COLOR_INFOTEXT)
			);
	dc.DrawText( m_sText, m_sText.GetLength(), &rc, DT_TOP | DT_LEFT );
	dc.SetTextColor( clrOltTextColor );

	// Draw the icon
	if( m_hIcon != NULL )
		DrawIconEx(
			dc.GetSafeHdc(),
			CX_ROUNDED,
			nIconVertPos,
			m_hIcon,
			m_IconSize.cx,
			m_IconSize.cy,
			0,
			NULL,
			DI_NORMAL
			);

	// Clean up GDI
	::DeleteObject(hRegion);
	//if( pSysFont != NULL )
		dc.SelectObject(pSysFont);
}

void CExtPopupMenuTipWnd::SetText(__EXT_MFC_SAFE_LPCTSTR lpszText)
{
	ASSERT_VALID( this );

	m_sText = (lpszText==NULL) ? _T("") : lpszText;
	m_sText.TrimLeft();
	m_sText.TrimRight();
}

void CExtPopupMenuTipWnd::SetIcon(HICON hIcon) 
{
	ASSERT_VALID( this );

	if( m_hIcon != NULL )
	{
		::DestroyIcon( m_hIcon );
		m_hIcon = NULL;
	}
	if( hIcon == NULL )
	{
		m_IconSize = CSize( 0, 0 );
		return;
	}
ICONINFO IconInfo;
	m_hIcon = hIcon; 
	
	// Get the icon sizes	
	ZeroMemory( &IconInfo, sizeof(ICONINFO) );
	::GetIconInfo( m_hIcon, &IconInfo );
	ASSERT( IconInfo.hbmColor != NULL || IconInfo.hbmMask != NULL );

	m_IconSize.cx = (BYTE)(IconInfo.xHotspot * 2);
	m_IconSize.cy = (BYTE)(IconInfo.yHotspot * 2);
	if( m_IconSize.cx == 0 || m_IconSize.cy == 0 )
	{
		ASSERT( IconInfo.hbmColor != NULL || IconInfo.hbmMask != NULL );
		BITMAP bi;
		::memset( &bi, 0, sizeof(BITMAP) );
		::GetObject(
			(IconInfo.hbmColor != NULL) ? IconInfo.hbmColor : IconInfo.hbmMask,
			sizeof(BITMAP),
			&bi
			);
		ASSERT( bi.bmWidth > 0 && bi.bmHeight > 0 );
		m_IconSize.cx = bi.bmWidth;
		m_IconSize.cy = bi.bmHeight;
	} // if( m_IconSize.cx == 0 || m_IconSize.cy == 0 )
	ASSERT( m_IconSize.cx > 0 && m_IconSize.cy > 0 );

	::DeleteObject( IconInfo.hbmMask );
	::DeleteObject( IconInfo.hbmColor );

	if( ::IsWindow(m_hWnd) )
		RedrawWindow();
}

bool CExtPopupMenuTipWnd::Show(
	CWnd * pWndParent,
	const RECT & rcExcludeArea // = CExtPopupMenuTipWnd::rcNoExcludeArea
	)
{
	ASSERT_VALID( this );

	ASSERT_VALID( pWndParent );
	ASSERT( pWndParent->GetSafeHwnd() != NULL );

HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);
	ASSERT( hCursor != NULL );

	if( GetSafeHwnd() == NULL )
	{
		CExtSafeString strClassName =
			::AfxRegisterWndClass (
				__POPUP_WNDCLASS_STYLES__,
				hCursor,
				(HBRUSH)(COLOR_BTNFACE + 1),
				NULL
				);
		BOOL bCreateResult =
			CWnd::CreateEx(
				WS_EX_TOPMOST,
				strClassName,
				NULL,
				WS_POPUP,
				0, 0, 0, 0,
				pWndParent->GetSafeHwnd(),
				NULL,
				NULL
				);
		ASSERT( bCreateResult );
		if( !bCreateResult )
			return false;
	} // if( GetSafeHwnd() == NULL )
#ifdef _DEBUG
	else
	{
		ASSERT_VALID( pWndParent );
		ASSERT( ::IsWindow( pWndParent->GetSafeHwnd() ) );
//		CWnd * pExistingParent = GetParent();
//		ASSERT( pWndParent == pExistingParent );
	} // else from if( GetSafeHwnd() == NULL )
#endif // _DEBUG

	ASSERT_VALID( this );

	m_rcExcludeArea = rcExcludeArea;
	m_rcExcludeArea.NormalizeRect();
CPoint ptTipTarget( m_rcExcludeArea.TopLeft() );
	ptTipTarget += CSize( __TIP_OVER_METRIC/2, __TIP_OVER_METRIC/2 );
	m_ptTrack = ptTipTarget;

CDC * pDC = GetDC();
	ASSERT_VALID( pDC );
HRGN	hRegion;
CSize WindowSize;
	VERIFY( GetWindowRegion(pDC, &hRegion, &WindowSize) );
	ReleaseDC(pDC);
	VERIFY(
		::SetWindowRgn(
			m_hWnd,
			hRegion,
			TRUE
			)
		);

	if( m_bFlipHorz )
		m_ptTrack.x +=
			m_rcExcludeArea.Width() - __TIP_OVER_METRIC
			+ WindowSize.cx - CX_ROUNDED*2
			;
	if( m_bFlipVert )
		m_ptTrack.y += 
			m_rcExcludeArea.Height() - __TIP_OVER_METRIC
			+ WindowSize.cy - CX_ROUNDED*2
			;

CPoint ptWndPos(
		m_ptTrack.x - WindowSize.cx + CX_ROUNDED,
		m_ptTrack.y - WindowSize.cy + CY_ROUNDED
		);
	SetWindowPos(
		&CWnd::wndTopMost,
		ptWndPos.x,
		ptWndPos.y,
		WindowSize.cx,
		WindowSize.cy,
		SWP_NOACTIVATE //| SWP_SHOWWINDOW
		);
CRect rcWnd(ptWndPos,WindowSize);
	MoveWindow(&rcWnd);
//	ASSERT( IsWindowVisible() );
	::SetCursor(hCursor);

//	Invalidate( FALSE );
//	UpdateWindow();
	SetTimer( ID_TIMER_DELAY_SHOW, ID_PERIOD_DELAY_SHOW, NULL );

	return true;
}

void CExtPopupMenuTipWnd::Hide()
{
	ASSERT_VALID( this );

	if( GetSafeHwnd() != NULL )
	{
		KillTimer( ID_TIMER_DELAY_SHOW );
		ShowWindow( SW_HIDE );
	}
	m_rcExcludeArea.SetRectEmpty();
}

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuWnd

// is allowed menu item positioning without
// using only & - marked text
bool CExtPopupMenuWnd::g_bAllowNonAccelPositioning = false;

IMPLEMENT_DYNCREATE(CExtPopupMenuWnd, CExtPopupBaseWnd)

CExtPopupMenuWnd::CExtPopupMenuWnd()
	: m_pWndParentMenu( NULL )
	, m_hWndCmdReciever( m_hWndCmdReciever )
	, m_nLeftAreaWidth( 0 )
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	, m_pNode( NULL )
	, m_pDragSrcNode( NULL )
	, m_nDragSrcIdx( -1 )
	, m_bHelperNoRemoveSrc( false )
	, m_bHelperDragOverPassed( false )
	, m_pCustomizeSite( NULL )
	, m_nHelperDropIndexBefore( -1 )
	, m_bHelperNoHideChild( false )
	, m_ptHelperDragStart( 0, 0 )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
{
	__VPC_INC;
	_Init();
}

CExtPopupMenuWnd::~CExtPopupMenuWnd()
{
	_OnCancelMode( true );
	if( m_bTopLevel )
	{
		VERIFY( _BuildItems( NULL, true ) );
	}
	__VPC_DEC;
#ifdef _DEBUG
	if( m_bTopLevel )
	{
		__VPC_VERIFY_0;
	}
#endif // _DEBUG
}

bool CExtPopupMenuWnd::TestHoverEnabledFromActiveHWND(
	HWND hWndTestFrom,
	HWND hWndActive, // = ::GetActiveWindow(),
	bool bCheckEnabled, // = true,
	bool bCheckVisible, // = true,
	bool bCheckExtPopupMenuTracking // = true
	)
{
	if( bCheckExtPopupMenuTracking )
	{
		if( CExtPopupMenuWnd::IsMenuTracking() )
			return false;
	} // if( bCheckExtPopupMenuTracking )
	
	if(		hWndTestFrom == NULL
		||	( ! ::IsWindow( hWndTestFrom ) )
		)
		return false;
	if(		hWndActive == NULL
		||	( ! ::IsWindow( hWndActive ) )
		)
		return false;

	if( bCheckEnabled )
	{
		if( ! ::IsWindowEnabled( hWndTestFrom ) )
			return false;
		if( ! ::IsWindowEnabled( hWndActive ) )
			return false;
	} // if( bCheckEnabled )

DWORD dwWndStyle;
	if( bCheckVisible )
	{
		dwWndStyle =
			(DWORD) ::GetWindowLong(
				hWndTestFrom,
				GWL_STYLE
				);
		if( (dwWndStyle & WS_VISIBLE) == 0 )
			return false;

		dwWndStyle =
			(DWORD) ::GetWindowLong(
				hWndActive,
				GWL_STYLE
				);
		if( (dwWndStyle & WS_VISIBLE) == 0 )
			return false;
	} // if( bCheckVisible )

const DWORD dwTestStyles = (WS_VISIBLE|WS_POPUP);

	for(	HWND hWndParentOfTest = ::GetParent( hWndTestFrom );
			hWndParentOfTest != NULL;
			hWndParentOfTest = ::GetParent( hWndParentOfTest )
			)
	{ // walk parent popup windows of hWndTestFrom
		ASSERT( ::IsWindow( hWndParentOfTest ) );
		dwWndStyle =
			(DWORD) ::GetWindowLong(
				hWndParentOfTest,
				GWL_STYLE
				);
		if( (dwWndStyle&dwTestStyles) == 0L )
			continue;
		if( hWndParentOfTest == hWndActive )
			return true;

		for(	HWND hWndParentOfActive = hWndActive;
				hWndParentOfActive != NULL;
				hWndParentOfActive = ::GetParent( hWndParentOfActive )
				)
		{ // walk parent popup windows of hWndActive
			ASSERT( ::IsWindow( hWndParentOfActive ) );
			dwWndStyle =
				(DWORD) ::GetWindowLong(
					hWndParentOfActive,
					GWL_STYLE
					);
			if( (dwWndStyle&dwTestStyles) == 0L )
				continue;
			if( hWndParentOfActive == hWndParentOfTest )
				return true;
		} // walk parent popup windows of hWndActive

	} // walk parent popup windows of hWndTestFrom

	return false;
}

void CExtPopupMenuWnd::_Init()
{
	ASSERT_VALID( this );

	_InitAnimation();

	m_nDyScrollOffset = 0;
	m_bTopLevel
		= m_bScrollingAvailable
		= m_bExpandAvailable
		= m_bExpandWasPressed
		= m_bCombineWithEA 
		= false;
	m_dwTrackFlags = 0;
	m_nWaitingExpandTickCount = 0;
	m_nCurIndex
		= m_nDelayedFocusItemIndex
		= IDX_NOTHING;

	m_nScrollingDirection = 0;
	m_rcScrollTop.SetRectEmpty();
	m_rcScrollBottom.SetRectEmpty();
	m_rcExpandBtn.SetRectEmpty();

	m_eCombineAlign = __CMBA_NONE;

	m_pCbPaintCombinedCookie = NULL;
	m_pCbPaintCombinedContent = NULL;
	m_bCookieIsObject = false;
}

CObject * CExtPopupBaseWnd::GetCoockieAsObject()
{
	if(		(!m_bCookieIsObject)
		||	m_pCbPaintCombinedCookie == NULL
		)
		return NULL;
CObject * pObj = (CObject*)m_pCbPaintCombinedCookie;
	ASSERT_VALID( pObj );
	return pObj;
}

bool CExtPopupBaseWnd::IsToolBtnCombinedArea() const
{
	if( !IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) ) )
		return false;
CExtPopupMenuWnd * pPopup = (CExtPopupMenuWnd*)(this);
//	pPopup = pPopup->GetParentMenuWnd();
//	if( pPopup == NULL )
//		return false;
	if(		(LPVOID)(CExtPopupMenuWnd::stat_paint_combined_menu_toolbtn)
		!=	(LPVOID)(pPopup->GetCbPaintCombinedContent())
		)
		return false;
	return true;
}

int CExtPopupMenuWnd::_GetCurIndex() const
{
	ASSERT_VALID( this );

	return m_nCurIndex;
}

int CExtPopupMenuWnd::_GetSpecBtnHeight()
{
int nHeight = g_PaintManager->GetMenuExpandButtonHeight();
	ASSERT( nHeight > 0 );
	return nHeight;
}

void CExtPopupMenuWnd::_RecalcLayoutImpl()
{
	ASSERT_VALID( this );

CRect rcClient;
	_GetClientRect( &rcClient );
int nScrollBtnDy =
		_GetSpecBtnHeight();
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nMenuShadowSize = 0;
//		_GetPopupShadowSize();

int nScrollBottomOffsetDy = 0;
	if( m_bExpandAvailable )
	{
		m_rcExpandBtn.left =
			rcClient.left + nMenuBorderSize;
		m_rcExpandBtn.right =
			rcClient.right - (nMenuBorderSize + nMenuShadowSize);
		m_rcExpandBtn.bottom =
			rcClient.bottom - (nMenuBorderSize + nMenuShadowSize);
		m_rcExpandBtn.top =
			m_rcExpandBtn.bottom - nScrollBtnDy;
		nScrollBottomOffsetDy =
			- m_rcExpandBtn.Height();
		m_rcExpandBtn.DeflateRect(
			__MI_HORZ_GAP_TO_BORDER,
			0
			);
		m_rcExpandBtn.left += m_nLeftAreaWidth;
	} // if( m_bExpandAvailable )

	if( m_bScrollingAvailable )
	{
		m_rcScrollTop.left = m_rcScrollBottom.left =
			rcClient.left + nMenuBorderSize;
		m_rcScrollTop.right = m_rcScrollBottom.right =
			rcClient.right - (nMenuBorderSize + nMenuShadowSize);

		m_rcScrollTop.top =
			rcClient.top + nMenuBorderSize;
		m_rcScrollTop.bottom =
			m_rcScrollTop.top + nScrollBtnDy;
		
		m_rcScrollBottom.bottom =
			rcClient.bottom - (nMenuBorderSize + nMenuShadowSize);
		m_rcScrollBottom.top =
			m_rcScrollBottom.bottom - nScrollBtnDy;

		m_rcScrollBottom.OffsetRect(0,nScrollBottomOffsetDy);
		m_rcScrollTop.DeflateRect(
			__MI_HORZ_GAP_TO_BORDER,
			0
			);
		m_rcScrollBottom.DeflateRect(
			__MI_HORZ_GAP_TO_BORDER,
			0
			);
		m_rcScrollTop.left += m_nLeftAreaWidth;
		m_rcScrollBottom.left += m_nLeftAreaWidth;
	} // if( m_bScrollingAvailable )
}

class CExtPopupMenuWnd::CExtPopupMenuCmdUI : public CCmdUI
{
public: // re-implementations only
	HWND m_hWndCmdReciever;
	CExtPopupMenuWnd * m_pPopup;
	CExtPopupMenuWnd::MENUITEMDATA * m_pItemDataActive;
	CExtPopupMenuCmdUI(
		HWND hWndCmdReciever,
		CExtPopupMenuWnd * pPopup
		)
		: m_hWndCmdReciever( hWndCmdReciever )
		, m_pPopup( pPopup )
		, m_pItemDataActive( NULL )
	{
		ASSERT( m_hWndCmdReciever != NULL && ::IsWindow(m_hWndCmdReciever) );
		ASSERT_VALID( m_pPopup );
		CCmdUI::m_pOther = NULL; // menu
		CCmdUI::m_nIndexMax = (UINT)m_pPopup->ItemGetCount();
		CCmdUI::m_nIndex = 0;
	}
	void SetActiveIndex( int nIndex )
	{
		ASSERT( m_hWndCmdReciever != NULL && ::IsWindow(m_hWndCmdReciever) );
		ASSERT_VALID( m_pPopup );
		CCmdUI::m_nIndex = (UINT)nIndex;
		ASSERT( CCmdUI::m_nIndex >= 0 && CCmdUI::m_nIndex < CCmdUI::m_nIndexMax );
		m_pItemDataActive = &m_pPopup->_GetItemRef(nIndex);
		ASSERT( m_pItemDataActive != NULL );
		CCmdUI::m_nID = m_pItemDataActive->GetCmdID();
		if(		m_pItemDataActive->IsPopup()
			||	m_pItemDataActive->IsSeparator()
			||	CExtCmdManager::IsSystemCommand( CCmdUI::m_nID )
			)
			return;
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd(m_hWndCmdReciever),
				CCmdUI::m_nID
				);
		if( pCmdItem == NULL )
			return;
//		m_pItemDataActive->SetText( pCmdItem->m_sMenuText );
		m_pItemDataActive->SetAccelText( pCmdItem->m_sAccelText );
		m_pItemDataActive->AccelCharInit();
		m_pItemDataActive->MeasureItem();
	}
	virtual void Enable(BOOL bOn)
	{
		CCmdUI::m_bEnableChanged = TRUE;
		ASSERT( m_hWndCmdReciever != NULL && ::IsWindow(m_hWndCmdReciever) );
		ASSERT_VALID( m_pPopup );
		ASSERT( m_pItemDataActive != NULL );
/*
		if( m_pItemDataActive->IsPopup() )
		{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
			CExtCustomizeSite * pSite =
				CExtCustomizeSite::GetCustomizeSite(m_hWndCmdReciever);
			if(	!(		m_pItemDataActive->IsInplaceEdit()
					&&	pSite != NULL
					&&	m_pItemDataActive->GetCmdNode() != NULL
					)
				)
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
				return;
		}
*/
		if( m_pItemDataActive->IsPopup() && m_pItemDataActive->GetCmd() == NULL )
			return;

		if( m_pItemDataActive->IsForceEnabled() )
			bOn = TRUE;
		m_pItemDataActive->Enable( bOn ? true : false );
//		m_pItemDataActive->MeasureItem();
	}
	virtual void SetRadio(BOOL bOn)
	{
		ASSERT( m_hWndCmdReciever != NULL && ::IsWindow(m_hWndCmdReciever) );
		ASSERT_VALID( m_pPopup );
		ASSERT( m_pItemDataActive != NULL );
		if( m_pItemDataActive->IsPopup() )
			return;
		m_pItemDataActive->Radio( bOn ? true : false );
//		m_pItemDataActive->MeasureItem();
	}
	virtual void SetCheck(int nCheck)
	{
		ASSERT_VALID( m_pPopup );
		ASSERT( m_pItemDataActive != NULL );
		if( m_pItemDataActive->IsPopup() )
			return;
		m_pItemDataActive->Check( nCheck ? true : false );
//		m_pItemDataActive->MeasureItem();
	}
	virtual void SetText( LPCTSTR lpszText )
	{
		ASSERT( m_hWndCmdReciever != NULL && ::IsWindow(m_hWndCmdReciever) );
		ASSERT_VALID( m_pPopup );
		ASSERT( m_pItemDataActive != NULL );
		if( lpszText == NULL )
			return;
		if( m_pItemDataActive->IsPopup() )
			return;
		m_pItemDataActive->SetText( lpszText );
		m_pItemDataActive->MeasureItem();
	}
}; // class CExtPopupMenuWnd::CExtPopupMenuCmdUI

void CExtPopupMenuWnd::_UpdateCmdUI()
{
	ASSERT_VALID( this );

	if( m_bTopLevel )
		if( (m_dwTrackFlags&TPMX_NO_CMD_UI) != 0 )
			return;

	ASSERT( m_hWndCmdReciever != NULL );

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
__EXT_MFC_SAFE_LPCTSTR strProfileName = g_CmdManager->ProfileNameFromWnd(m_hWndCmdReciever);
CExtCustomizeSite * pSite = CExtCustomizeSite::GetCustomizeSite(m_hWndCmdReciever);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

BOOL bDisableIfNoHndler = TRUE;
CExtPopupMenuCmdUI _state( m_hWndCmdReciever, this );

bool bResyncItems = false;
	for(	INT _iter = 0;
			_iter < m_items_all.GetSize();
			_iter++
			)
	{ // walk all items
		_state.SetActiveIndex( _iter );
		ASSERT( _state.m_pItemDataActive != NULL );
		if( _state.m_pItemDataActive->IsSeparator() )
			continue;
		if( _state.m_pItemDataActive->IsPopup() )
		{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
			if(	/*	_state.m_pItemDataActive->IsInplaceEdit()
				&&*/	pSite != NULL
				&&	_state.m_pItemDataActive->GetCmdNode() != NULL
				)
			{
				UINT _nCmdIdSaved = _state.m_pItemDataActive->m_nCmdID;
				_state.m_nID =
					_state.m_pItemDataActive ->
						GetCmdNode() ->
							GetCmdID( false );
				ASSERT( CExtCmdManager::IsCommand( _state.m_nID ) );
				HWND hWndCmdReciever =
					_state.m_pItemDataActive->GetCmdReciever();
				ASSERT( hWndCmdReciever != NULL );
				ASSERT( ::IsWindow(hWndCmdReciever) );
				CWnd * pWndCmdReciever =
					CWnd::FromHandle( hWndCmdReciever );
				ASSERT_VALID( pWndCmdReciever );
				_state.DoUpdate( pWndCmdReciever, bDisableIfNoHndler );
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
				if(		pSite != NULL
					&&	pSite->IsCustomizeMode()
					)
					_state.m_pItemDataActive->SetForceEnabled( true );
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
				_state.m_pItemDataActive->m_nCmdID = _nCmdIdSaved;

				_state.m_pItemDataActive->GetPopup()->_UpdateCmdUI();
				
				continue;
			}
			else
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
			{
				_state.m_pItemDataActive->GetPopup()->_UpdateCmdUI();
				continue;
			}
		}
		if( _state.m_pItemDataActive->IsExtraMark() )
		{
			bResyncItems = true;
			continue;
		}
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		if(		pSite != NULL
			&&	pSite->IsUserBarCommand(
					_state.m_pItemDataActive->GetCmdID()
					)
			&&	strProfileName != NULL
			)
		{
			_state.m_pItemDataActive->SetForceEnabled( true );
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					strProfileName,
					_state.m_pItemDataActive->GetCmdID()
					);
			ASSERT( pCmdItem != NULL );
			CExtToolControlBar * pUserBar =
				pSite->GetUserBar( _state.m_pItemDataActive->GetCmdID() );
			ASSERT_VALID( pUserBar );
			bool bCheck = pUserBar->IsVisible() ? true : false;
			pCmdItem->StateSetCheck( bCheck );
			_state.SetCheck( bCheck );
			continue;
		}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		if(		CExtCmdManager::IsCommandNeedsSpecUpdate(
					_state.m_pItemDataActive->GetCmdID()
					)
			||	CExtCmdManager::IsSystemCommand(
					_state.m_pItemDataActive->GetCmdID()
					)
			)
			continue;

		HWND hWndCmdReciever =
			_state.m_pItemDataActive->GetCmdReciever();
		ASSERT( hWndCmdReciever != NULL );
		ASSERT( ::IsWindow(hWndCmdReciever) );
		CWnd * pWndCmdReciever =
			CWnd::FromHandle( hWndCmdReciever );
		ASSERT_VALID( pWndCmdReciever );
		_state.DoUpdate( pWndCmdReciever, bDisableIfNoHndler );
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		if(		pSite != NULL
			&&	pSite->IsCustomizeMode()
			)
			_state.m_pItemDataActive->SetForceEnabled( true );
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	}  // walk all items

	if( bResyncItems )
		_SyncItems();
}

bool CExtPopupMenuWnd::MENUITEMDATA::ConstructPopup()
{
	ASSERT( m_iconPopup.IsEmpty() );
	ASSERT( m_pWndChild == NULL );
	m_pWndChild = new CExtPopupMenuWnd;
	
	ASSERT( m_hWndSpecCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndSpecCmdReciever) );
	m_pWndChild->m_hWndCmdReciever = m_hWndSpecCmdReciever;
	
	return true;
}

void CExtPopupMenuWnd::MENUITEMDATA::DestroyPopup()
{
	VERIFY( SetPopupIcon(NULL) );
	if( m_pWndChild == NULL )
		return;
	if( m_pWndChild->GetSafeHwnd() != NULL
		&& ::IsWindow( m_pWndChild->GetSafeHwnd() )
		)
		m_pWndChild->DestroyWindow();
//	else
//		delete m_pWndChild;
	delete m_pWndChild;
	m_pWndChild = NULL;
}

bool CExtPopupMenuWnd::_BuildItems(
	CMenu * pBuildMenu,
	bool bTopLevel
	)
{
	ASSERT_VALID( this );
#ifdef _DEBUG
	if( pBuildMenu != NULL )
	{
		ASSERT( m_hWndCmdReciever != NULL );
		ASSERT( ::IsWindow(m_hWndCmdReciever) );
	}
#endif // _DEBUG
INT iter = 0;
	for( ; iter < m_items_all.GetSize(); iter++)
	{
		MENUITEMDATA & mi = m_items_all[ iter ];
		if( mi.IsPopup() )
		{
			mi.GetPopup()->m_hWndCmdReciever = m_hWndCmdReciever;
			mi.GetPopup()->_OnCancelMode();
			VERIFY( mi.GetPopup()->_BuildItems( NULL, false ) );
			mi.DestroyPopup();
		}
	} // for( ; iter < m_items_all.GetSize(); iter++)
	m_items_all.RemoveAll();

	if( pBuildMenu == NULL )
		return true;
	ASSERT( pBuildMenu->GetSafeHmenu() != NULL );

	m_bTopLevel = bTopLevel;
	if( m_bTopLevel )
		m_pWndParentMenu = NULL;

	// build items loop
bool bPrevWasSeparator = false;
int nMruUpdateIndex = -1, nInsertedIndex = 0;
int	nMenuItemCount = pBuildMenu->GetMenuItemCount();
	ASSERT( nMenuItemCount > 0 );
	m_items_all.RemoveAll();
	for( int nItemIndex=0; nItemIndex<nMenuItemCount; nItemIndex++ )
	{
		UINT nMenuItemID =
			pBuildMenu->GetMenuItemID( nItemIndex );
		if( ID_FILE_MRU_FIRST <= nMenuItemID
			&&
			nMenuItemID <= ID_FILE_MRU_LAST
			)
		{
			if( nMruUpdateIndex < 0 )
				nMruUpdateIndex = nInsertedIndex;
			continue;
		}
		MENUITEMDATA mi;
		mi.SetCmdReciever( m_hWndCmdReciever );
		VERIFY(
			mi.UpdateFromMenu(
				m_hWndCmdReciever,
				pBuildMenu,
				nItemIndex
				)
			);
		bool bIsSeparator = mi.IsSeparator();
		if( bPrevWasSeparator && bIsSeparator )
				continue;
		bPrevWasSeparator = bIsSeparator;
		
		if( mi.IsPopup() )
		{
			CMenu * pSubMenu =
					pBuildMenu->GetSubMenu(nItemIndex);
			ASSERT( pSubMenu != NULL );
			ASSERT( pSubMenu->GetSafeHmenu() != NULL );
			mi.GetPopup()->m_hWndCmdReciever = m_hWndCmdReciever;
			mi.GetPopup()->_BuildItems( pSubMenu, false );
			mi.GetPopup()->m_pWndParentMenu = this;
		}
		_InsertItem( -1, mi );
		nInsertedIndex++;
	} // for( int nItemIndex=0; nItemIndex<nMenuItemCount; nItemIndex++ )
	ASSERT( m_items_all.GetSize() <= nMenuItemCount );

int nMruInsertCount = 0;
	if( nMruUpdateIndex >= 0 )
	{ // insert recently opened file items
		ASSERT( nMruUpdateIndex < nMenuItemCount );
		CRecentFileList * pRecentFileList =
			InternalFriendlyWinApp::_GetFriendlyApp()->
				_GetRecentFileList();
		if( pRecentFileList != NULL )
		{ // can be null !?
			int nRecentCount =
				pRecentFileList->GetSize();
			TCHAR sCurrDir[_MAX_PATH+1];
			::memset(sCurrDir,0,sizeof(sCurrDir));
			::GetCurrentDirectory(_MAX_PATH,sCurrDir);
			int nLenCurDir = _tcslen(sCurrDir);
			for( nItemIndex=0; nItemIndex<nRecentCount; nItemIndex++ )
			{
				CExtSafeString sDisplayName( _T("") );
				if(	!pRecentFileList->GetDisplayName(
						*((CString *)&sDisplayName),
						nItemIndex,
						sCurrDir,
						nLenCurDir,
						TRUE
						)
					)
					continue;
				ASSERT( !sDisplayName.IsEmpty() );
				UINT nCmdID = ID_FILE_MRU_FIRST + nItemIndex;
				ASSERT( nCmdID <= ID_FILE_MRU_LAST );
				CExtCmdItem * pCmdItem =
					g_CmdManager->CmdGetPtr(
						g_CmdManager->ProfileNameFromWnd( m_hWndCmdReciever ),
						nCmdID
						);
				if( pCmdItem == NULL )
					pCmdItem =
						g_CmdManager->CmdAllocPtr(
							g_CmdManager->ProfileNameFromWnd( m_hWndCmdReciever ),
							nCmdID
							);
				ASSERT( pCmdItem != NULL );
				if( pCmdItem == NULL )
					return false;
				int nDisplayIndex = nItemIndex+1;
				if( nDisplayIndex < 10 )
					pCmdItem->m_sMenuText.Format(
						_T("&%d %s"),
						nDisplayIndex,
						sDisplayName
						);
				else
					pCmdItem->m_sMenuText.Format(
						_T("%d&%d %s"),
						nDisplayIndex/10,
						nDisplayIndex%10,
						sDisplayName
						);
				
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				
				CExtSafeString sRecentFileFmt1,sRecentFileFmt2;
				if( !sRecentFileFmt1.LoadString(IDS_RECENT_FILE_FMT_1) )
				{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
					ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
					sRecentFileFmt1 = _T("Recent file %d");
				}
				if( !sRecentFileFmt2.LoadString(IDS_RECENT_FILE_FMT_2) )
				{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
					ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
					sRecentFileFmt2 = _T("Recent file %d (\"%s\")");
				}
				pCmdItem->m_sToolbarText.Format(
					(LPCTSTR)sRecentFileFmt1,
					nItemIndex + 1
					);
				pCmdItem->m_sTipTool.Format(
					(LPCTSTR)sRecentFileFmt2,
					nItemIndex + 1,
					sDisplayName
					);
				pCmdItem->m_sTipStatus = pCmdItem->m_sTipTool;

				BOOL bInsRetVal =
					ItemInsert(
						nCmdID,
						nMruUpdateIndex + nItemIndex
						);
				
				if( !bInsRetVal )
				{
					ASSERT( FALSE );
					return false;
				}
				nMruInsertCount++;
			} // for( nItemIndex=0; nItemIndex<nRecentCount; nItemIndex++ )
		} // can be null !?
		if( nMruInsertCount > 0 )
		{
			if( !ItemInsert(
					ID_SEPARATOR,
					nMruUpdateIndex + nMruInsertCount
					)
				)
			{
				ASSERT( FALSE );
				return false;
			}
			nMruInsertCount++;
		} // if( nMruInsertCount > 0 )
	} // insert recently opened file items

	_SyncItems();
	return true;
}

void CExtPopupMenuWnd::_GetClientRect(RECT * pRectClient)
{
	ASSERT_VALID( this );

	ASSERT( pRectClient != NULL );
	ASSERT( GetSafeHwnd() != NULL );

	*pRectClient = m_rcClient;

	ASSERT( pRectClient->top <= pRectClient->bottom );
	ASSERT( pRectClient->left <= pRectClient->right );
}

BOOL CExtPopupMenuWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	ASSERT_VALID( this );
	if( _FindHelpMode() )
	{
		SetCursor( afxData.hcurHelp );
		return TRUE;
	}
CPoint ptCursor;
	if( ItemGetCount() > 0 && ::GetCursorPos(&ptCursor) )
	{
		ScreenToClient( &ptCursor );
		int nIndex = _HitTest( ptCursor );
		if( nIndex >= 0 )
		{
			MENUITEMDATA & mi = _GetItemRef( nIndex );
			if(		(! mi.IsExtraMark() )
				&&	mi.IsInplaceEdit()
				&&	mi.IsAllowInplaceEditActivation()
				&&	(! _FindHelpMode() )
				)
			{
				CRect rcItem;
				_GetItemRect( nIndex, rcItem );
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
				if( _FindCustomizeMode() )
				{
					ASSERT_VALID( mi.GetCmdNode() );
					if(		mi.IsSelected()
						&&	(mi.GetCmdNode()->GetFlags() & __ECTN_TBB_RESIZABLE) != 0
						)
					{ // if button can be resized
						int nDdWidth = g_PaintManager->GetDropDividerMerics().cx / 2;
						if( nDdWidth < 2 )
							nDdWidth = 2;
						CRect rcH( rcItem );
						rcH.right = rcH.left + nDdWidth;
						bool bSetCursor = false;
						if( rcH.PtInRect(ptCursor) )
							bSetCursor = true;
						else
						{
							rcH = rcItem;
							rcH.left = rcH.right - nDdWidth;
							if( rcH.PtInRect(ptCursor) )
								bSetCursor = true;
						} // else from if( rcH.PtInRect(ptCursor) )
						if( bSetCursor )
						{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
							CExtLocalResourceHelper _LRH;
							CWinApp * pApp = ::AfxGetApp();
							ASSERT_VALID( pApp );
							HCURSOR hCursor =
								pApp->LoadCursor(
									MAKEINTRESOURCE( IDC_EXT_RESIZE_H1 )
									);
							if( hCursor == NULL )
							{
								ASSERT( FALSE );
								hCursor = ::LoadCursor( NULL, IDC_SIZEWE );
							} // if( hCursor == NULL )
#else // if - (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
							HCURSOR hCursor = ::LoadCursor( NULL, IDC_SIZEWE );
#endif // if NOT - (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
							ASSERT( hCursor != NULL );
							::SetCursor( hCursor );
							return TRUE;
						} // if( bSetCursor )
					} // if button can be resized
				} // if( _FindCustomizeMode() )
				else
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
				{
					CRect rcInplaceCtrl = mi.AdjustInplaceEditRect( rcItem );
					if( rcInplaceCtrl.PtInRect(ptCursor) )
					{
						HCURSOR hCursor = ::LoadCursor( NULL, IDC_IBEAM );
						SetCursor( hCursor );
						return TRUE;
					} // if( rcInplaceCtrl.PtInRect(ptCursor) )
				} // else from if( _FindCustomizeMode() )
			}
		} // if( nIndex >= 0 )
	} // if( ItemGetCount() > 0 && ::GetCursorPos(&ptCursor) )
	return CExtPopupBaseWnd::OnSetCursor(pWnd, nHitTest, message);
}

int CExtPopupMenuWnd::_HitTest(
	const CPoint & point,
	bool * p_bInplaceControlArea, // = NULL
	bool * p_bInplaceDropDownArea // = NULL
	)
{
	ASSERT_VALID( this );

	if( p_bInplaceControlArea != NULL )
		*p_bInplaceControlArea = false;
	if( p_bInplaceDropDownArea != NULL )
		*p_bInplaceDropDownArea = false;

	if( GetSafeHwnd() == NULL )
		return IDX_NOTHING;

CRect rcClient;
	_GetClientRect( &rcClient );
	if( !rcClient.PtInRect(point) )
		return IDX_NOTHING;

	if( m_bScrollingAvailable )
	{
		CRect rcItem;
		if( m_nDyScrollOffset != 0 )
		{
			_GetItemRect(IDX_SCROLL_TOP,rcItem);
			if( rcItem.PtInRect(point) )
				return IDX_SCROLL_TOP;
		}
		int nMaxScrollPos = _GetMaxScrollPos();
		if( m_nDyScrollOffset != nMaxScrollPos )
		{
			_GetItemRect(IDX_SCROLL_BOTTOM,rcItem);
			if( rcItem.PtInRect(point) )
				return IDX_SCROLL_BOTTOM;
		}
	}
	if( m_bExpandAvailable )
	{
		CRect rcExpand;
		_GetItemRect(IDX_EXPAND,rcExpand);
		if( rcExpand.PtInRect(point) )
			return IDX_EXPAND;
	}

//int nMenuBorderSize =
//		g_PaintManager->GetMenuBorderSize();

//int y = m_nDyScrollOffset + nMenuBorderSize;

INT iter = 0;
int nIndex = 0;
	for( ; iter < m_items_all.GetSize(); iter++,nIndex++ )
	{
		MENUITEMDATA & mi = m_items_all[ iter ];
		if( !mi.IsDisplayed() )
			continue;

		CRect rcItem;
		_GetItemRect( nIndex, rcItem );
		if( rcItem.PtInRect(point) )
		{
			int nRetIndex = mi.GetIndex();
			ASSERT(
					nRetIndex >= 0
				&&	nRetIndex < m_items_all.GetSize()
				);
			if(		(	p_bInplaceControlArea != NULL
					||	p_bInplaceDropDownArea != NULL
					)
				&&	mi.IsInplaceEdit()
				)
			{
				CRect rcInplaceCtrl = mi.AdjustInplaceEditRect( rcItem );
				if( p_bInplaceControlArea != NULL )
				{
					if( rcInplaceCtrl.PtInRect(point) )
						*p_bInplaceControlArea = true;
				}
				if(		p_bInplaceDropDownArea != NULL
					&&	mi.IsPopup()
					)
				{
					rcInplaceCtrl.left = rcInplaceCtrl.right;
					rcInplaceCtrl.right += ::GetSystemMetrics(SM_CXHTHUMB);
					if( rcInplaceCtrl.PtInRect(point) )
						*p_bInplaceDropDownArea = true;
				}
			}
			return nRetIndex;
		}
	} // for( ; iter < m_items_all.GetSize(); iter++,nIndex++ )
	return IDX_NOTHING;
};


BEGIN_MESSAGE_MAP(CExtPopupMenuWnd, CExtPopupBaseWnd)
	//{{AFX_MSG_MAP(CExtPopupMenuWnd)
	ON_WM_CANCELMODE()
	ON_WM_TIMER()
	ON_WM_ACTIVATEAPP()
	ON_WM_SHOWWINDOW()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_SYSDEADCHAR()
	ON_WM_CHAR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuWnd message handlers

BOOL CExtPopupMenuWnd::LoadMenu( 
	HWND hWndCmdRecv,
	UINT nIDResource,
	bool bPopupMenu // = true
	)
{
	ASSERT_VALID( this );

CMenu menu;
	if( !menu.LoadMenu(nIDResource) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	return
		UpdateFromMenu(
			hWndCmdRecv,
			&menu,
			bPopupMenu
			);
}

BOOL CExtPopupMenuWnd::UpdateMdiWindowsMenu(
	CWnd * pWndStartSearchMdiFrameWnd // = NULL
	)
{
	ASSERT_VALID( this );

	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );
CMDIFrameWnd * pFrame = NULL;
CWnd * pWnd =
		(pWndStartSearchMdiFrameWnd != NULL)
			? pWndStartSearchMdiFrameWnd
			: FromHandle(m_hWndCmdReciever) // ::AfxGetMainWnd()
			;
	while( true )
	{
		ASSERT( pWnd != NULL );
		ASSERT_VALID( pWnd );
		pFrame =
			DYNAMIC_DOWNCAST(
				CMDIFrameWnd,
				pWnd
				);
		if( pFrame != NULL )
			break;
		pWnd = pWnd->GetParentFrame();
		if( pWnd == NULL )
			return FALSE;
	} // while( true )
	if( pFrame == NULL )
		return FALSE;
int nItemIndex = ItemGetCount();
	if( nItemIndex > 0 )
	{
		MENUITEMDATA & mi = _GetItemRef(nItemIndex-1);
		if( !mi.IsSeparator() )
		{
			ItemInsert(ID_SEPARATOR);
			nItemIndex++;
			ASSERT( nItemIndex == ItemGetCount() );
		}
	}

HWND m_hWndMDIClient = pFrame->m_hWndMDIClient;
	ASSERT( m_hWndMDIClient != NULL );
	ASSERT( ::IsWindow(m_hWndMDIClient) );
HWND hWndActiveChildFrame =
		(HWND)::SendMessage(
			m_hWndMDIClient,
			WM_MDIGETACTIVE,
			0,
			NULL
			);
int nCmdID = __ID_MDIWNDLIST_FIRST;
	for( int iWin=1; true; iWin++,nCmdID++ )
	{
		HWND hWndChildFrame =
			::GetDlgItem(m_hWndMDIClient, nCmdID);
		if( hWndChildFrame == NULL )
			break;

		if( iWin <= __ID_MDIWNDLIST_COUNT )
		{ // if head of MDI windows list
			ASSERT( ::IsWindow(hWndChildFrame) );
			CString sWinName( _T("") );
			CWnd::FromHandle( hWndChildFrame )-> GetWindowText( sWinName );
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd( pWndStartSearchMdiFrameWnd->GetSafeHwnd() ),
					nCmdID
					);
			if( pCmdItem == NULL )
				pCmdItem =
				g_CmdManager->CmdAllocPtr(
					g_CmdManager->ProfileNameFromWnd( pWndStartSearchMdiFrameWnd->GetSafeHwnd() ),
					nCmdID
					);
			ASSERT( pCmdItem != NULL );
			if( pCmdItem == NULL )
				return FALSE;
			pCmdItem->m_sMenuText.Format(
				_T("&%d %s"),
				iWin,
				sWinName
				);
			pCmdItem->m_sToolbarText = pCmdItem->m_sMenuText;
			
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			
			CExtSafeString sDocumentNameFmt;
			if( !sDocumentNameFmt.LoadString(IDS_DOCUMENT_NAME_FMT) )
			{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				sDocumentNameFmt = _T("Document \"%s\"");
			}
			pCmdItem->m_sTipTool.Format(
				(LPCTSTR)sDocumentNameFmt,
				sWinName
				);
			pCmdItem->m_sTipStatus = pCmdItem->m_sTipTool;
			bool bRadio =
				(hWndChildFrame == hWndActiveChildFrame)
					? true : false;
			pCmdItem->StateSetRadio( bRadio );
			if( !ItemInsert(nCmdID) )
			{
				ASSERT( FALSE );
				return FALSE;
			}
			MENUITEMDATA & mi = ItemGetInfo( ItemGetCount()-1 );
			mi.Radio( bRadio );
		} // if head of MDI windows list
		else
		{ // MDI windows list enough long, trim it
			if( !ItemInsert(TYPE_SEPARATOR) )
			{
				ASSERT( FALSE );
				return FALSE;
			}
			HWND hWndSearch = pWndStartSearchMdiFrameWnd->GetSafeHwnd();
			ASSERT( hWndSearch != NULL );
			ASSERT( ::IsWindow(hWndSearch) );
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd( hWndSearch ),
					__ID_MDIWND_DLGWINDOWS
					);
			if( pCmdItem == NULL )
				pCmdItem =
				g_CmdManager->CmdAllocPtr(
					g_CmdManager->ProfileNameFromWnd( hWndSearch ),
					__ID_MDIWND_DLGWINDOWS
					);
			ASSERT( pCmdItem != NULL );
			if( pCmdItem == NULL )
				return FALSE;
			
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			
			CExtSafeString sMoreWindows,sManageWindows;
			if( !sMoreWindows.LoadString(IDS_MDI_MORE_WINDOWS) )
			{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				sMoreWindows = _T("More Windows");
			}
			if( !sManageWindows.LoadString(IDS_MDI_MANAGE_OPENED_WINDOWS) )
			{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				sManageWindows = _T("Manage opened document windows");
			}
			pCmdItem->m_sMenuText = sMoreWindows;
			pCmdItem->m_sToolbarText = pCmdItem->m_sMenuText;
			pCmdItem->m_sTipTool = sManageWindows;
			pCmdItem->m_sTipStatus = pCmdItem->m_sTipTool;
			if( !ItemInsert(nCmdID) )
			{
				ASSERT( FALSE );
				return FALSE;
			}
			break;
		} // MDI windows list enough long, trim it
	} // for( int iWin=1; true; iWin++,nCmdID++ )
	
	_SyncItems();
	return TRUE;
}

HMENU CExtPopupMenuWnd::ExportToMenu(
	BOOL bDeep // = TRUE
	) const
{
	ASSERT_VALID( this );

CMenu menu;
	if( !menu.CreatePopupMenu() )
	{
		ASSERT( FALSE );
		return NULL;
	}

OSVERSIONINFO ov;
    ov.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	VERIFY( ::GetVersionEx( &ov ) );
bool bStupidNT4XX =
		(
		( ov.dwPlatformId == VER_PLATFORM_WIN32_NT )
		&&
		( ov.dwMajorVersion < 5 )
		);

int nCount = ItemGetCount();
	for( int nItem = 0; nItem < nCount; nItem++ )
	{
		const MENUITEMDATA & mi =
			_GetItemRef( nItem );

		if( mi.IsPopup() )
		{
			CExtSafeString sMiText( mi.GetText() );
			CExtSafeString sMiAccelText( mi.GetAccelText() );
			if( !sMiAccelText.IsEmpty() )
			{
				sMiText += _T("\t");
				sMiText += sMiAccelText;
			}
			const CExtPopupMenuWnd * pPopup = mi.GetPopup();
			ASSERT( pPopup != NULL );
			HMENU hPopupData;
			if( bDeep )
				hPopupData = pPopup->ExportToMenu();
			else
				hPopupData = ::CreatePopupMenu();
			if( ( ! (::IsMenu(hPopupData)) )
				||
				(!	menu.AppendMenu(
						MF_STRING|MF_POPUP,
						UINT(hPopupData),
						sMiText
						)
					)
				)
			{
				ASSERT( FALSE );
				//continue;
			}
			if( bStupidNT4XX )
			{
				VERIFY( ::DestroyMenu( hPopupData ) );
			}
			continue;
		} // if( mi.IsPopup() )

		if( mi.IsSeparator() )
		{
			VERIFY( menu.AppendMenu( MF_SEPARATOR ) );
			continue;
		} // if( mi.IsSeparator() )

		CExtSafeString sMiText( mi.GetText() );
		CExtSafeString sMiAccelText( mi.GetAccelText() );
		if( !sMiAccelText.IsEmpty() )
		{
			sMiText += _T("\t");
			sMiText += sMiAccelText;
		}
		VERIFY(
			menu.AppendMenu(
				MF_STRING,
				mi.GetCmdID(),
				sMiText
				)
			);

	} // for( int nItem = 0; nItem < nCount; nItem++ )

	return menu.Detach();
}

BOOL CExtPopupMenuWnd::UpdateFromMenu( 
	HWND hWndCmdRecv,
	CMenu * pBuildMenu,
	bool bPopupMenu, // = true
	bool bTopLevel // = true
	)
{
	ASSERT_VALID( this );

	if( hWndCmdRecv != NULL )
		m_hWndCmdReciever = hWndCmdRecv;
	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );

	if( pBuildMenu == NULL
		|| (!(::IsMenu(pBuildMenu->GetSafeHmenu())))
		)
	{
		ASSERT( FALSE );
		return false;
	}
	if( bPopupMenu )
	{
		pBuildMenu = pBuildMenu->GetSubMenu(0);
		if( pBuildMenu == NULL
			|| (!(::IsMenu(pBuildMenu->GetSafeHmenu())))
			)
		{
			ASSERT( FALSE );
			return false;
		}
	}

	if(	!g_CmdManager->UpdateFromMenu(
			g_CmdManager->ProfileNameFromWnd( m_hWndCmdReciever ),
			pBuildMenu->GetSafeHmenu()
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}

	if( !_BuildItems( pBuildMenu, bTopLevel ) )
		return FALSE;
	return TRUE;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
bool CExtPopupMenuWnd::UpdateFromCmdTree(
	HWND hWndCmdRecv,
	CExtCustomizeCmdTreeNode * pNode,
	bool bTopLevel // = true
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pNode );
	m_pNode = pNode;

	if( hWndCmdRecv != NULL )
		m_hWndCmdReciever = hWndCmdRecv;
	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );

__EXT_MFC_SAFE_LPCTSTR strProfileName =
		g_CmdManager->ProfileNameFromWnd( m_hWndCmdReciever );
bool bCustomizeMode = false;
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( m_hWndCmdReciever );
	if( pSite != NULL )
		bCustomizeMode = pSite->IsCustomizeMode();

INT iter = 0;
	for( ; iter < m_items_all.GetSize(); iter++)
	{
		MENUITEMDATA & mi = m_items_all[ iter ];
		if( mi.IsPopup() )
		{
			mi.GetPopup()->m_hWndCmdReciever = m_hWndCmdReciever;
			mi.GetPopup()->_OnCancelMode();
			VERIFY( mi.GetPopup()->_BuildItems( NULL, false ) );
			mi.DestroyPopup();
		}
	} // for( ; iter < m_items_all.GetSize(); iter++)
	m_items_all.RemoveAll();

	if( pNode == NULL )
		return true;
	ASSERT_VALID( pNode );

	m_bTopLevel = bTopLevel;
	if( m_bTopLevel )
		m_pWndParentMenu = NULL;

	// build items loop
int nMruUpdateIndex = -1, nInsertedIndex = 0;
int	nMenuItemCount = pNode->GetNodeCount();
	m_items_all.RemoveAll();
	for( int nItemIndex = 0; nItemIndex < nMenuItemCount; nItemIndex++ )
	{
		CExtCustomizeCmdTreeNode * pChildNode =
			pNode->ElementAt( nItemIndex );
		ASSERT_VALID( pChildNode );
		if( pChildNode->GetFlags() & __ECTN_GROUP_START )
		{
			VERIFY(
				ItemInsert(
					CExtPopupMenuWnd::TYPE_SEPARATOR,
					nInsertedIndex++
					)
				);
		} // if( pChildNode->GetFlags() & __ECTN_GROUP_START )
		UINT nMenuItemID = pChildNode->GetCmdID( false );
		if(		ID_FILE_MRU_FIRST <= nMenuItemID
			&&	nMenuItemID <= ID_FILE_MRU_LAST
			&&	(! bCustomizeMode )
			)
		{
			if( nMruUpdateIndex < 0 )
				nMruUpdateIndex = nInsertedIndex;
			continue;
		}
		MENUITEMDATA mi;
		mi.SetCmdReciever( m_hWndCmdReciever );
		VERIFY(
			mi.UpdateFromCmdTree(
				m_hWndCmdReciever,
				pChildNode,
				nInsertedIndex
				)
			);
		ASSERT( !mi.IsSeparator() );
		CExtCmdItem * pCmdItem = NULL;
		if( mi.IsPopup() )
		{
			mi.GetPopup()->m_hWndCmdReciever = m_hWndCmdReciever;
			mi.GetPopup()->UpdateFromCmdTree(
				hWndCmdRecv,
				pChildNode,
				false
				);
			mi.GetPopup()->m_pWndParentMenu = this;
		} // if( mi.IsPopup() )
		else
		{
			pCmdItem =
				g_CmdManager->CmdGetPtr(
					strProfileName,
					mi.GetCmdID()
					);
			mi.SetAccelText( pCmdItem->m_sAccelText );
		} // else from if( mi.IsPopup() )

		mi.SetText( pChildNode->GetTextInMenu( pCmdItem ) );
		mi.AccelCharInit();
		mi.MeasureItem();

		_InsertItem( -1, mi );
		nInsertedIndex++;
	} // for( int nItemIndex = 0; nItemIndex < nMenuItemCount; nItemIndex++ )

int nMruInsertCount = 0;
	if( nMruUpdateIndex >= 0 )
	{ // insert recently opened file items
		//ASSERT( nMruUpdateIndex <= nMenuItemCount );
		CRecentFileList * pRecentFileList =
			InternalFriendlyWinApp::_GetFriendlyApp()->
				_GetRecentFileList();
		if( pRecentFileList != NULL )
		{ // can be null !?
			int nRecentCount =
				pRecentFileList->GetSize();
			TCHAR sCurrDir[_MAX_PATH+1];
			::memset(sCurrDir,0,sizeof(sCurrDir));
			::GetCurrentDirectory(_MAX_PATH,sCurrDir);
			int nLenCurDir = _tcslen(sCurrDir);
			for( nItemIndex=0; nItemIndex<nRecentCount; nItemIndex++ )
			{
				CExtSafeString sDisplayName( _T("") );
				if(	!pRecentFileList->GetDisplayName(
						*((CString *)&sDisplayName),
						nItemIndex,
						sCurrDir,
						nLenCurDir,
						TRUE
						)
					)
					continue;
				ASSERT( !sDisplayName.IsEmpty() );
				UINT nCmdID = ID_FILE_MRU_FIRST + nItemIndex;
				ASSERT( nCmdID <= ID_FILE_MRU_LAST );
				CExtCmdItem * pCmdItem =
					g_CmdManager->CmdGetPtr(
						g_CmdManager->ProfileNameFromWnd( m_hWndCmdReciever ),
						nCmdID
						);
				if( pCmdItem == NULL )
					pCmdItem =
						g_CmdManager->CmdAllocPtr(
							g_CmdManager->ProfileNameFromWnd( m_hWndCmdReciever ),
							nCmdID
							);
				ASSERT( pCmdItem != NULL );
				if( pCmdItem == NULL )
					return false;
				int nDisplayIndex = nItemIndex+1;
				if( nDisplayIndex < 10 )
					pCmdItem->m_sMenuText.Format(
						_T("&%d %s"),
						nDisplayIndex,
						sDisplayName
						);
				else
					pCmdItem->m_sMenuText.Format(
						_T("%d&%d %s"),
						nDisplayIndex/10,
						nDisplayIndex%10,
						sDisplayName
						);
				
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				
				CExtSafeString sRecentFileFmt1,sRecentFileFmt2;
				if( !sRecentFileFmt1.LoadString(IDS_RECENT_FILE_FMT_1) )
				{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
					ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
					sRecentFileFmt1 = _T("Recent file %d");
				}
				if( !sRecentFileFmt2.LoadString(IDS_RECENT_FILE_FMT_2) )
				{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
					ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
					sRecentFileFmt2 = _T("Recent file %d (\"%s\")");
				}
				pCmdItem->m_sToolbarText.Format(
					(LPCTSTR)sRecentFileFmt1,
					nItemIndex + 1
					);
				pCmdItem->m_sTipTool.Format(
					(LPCTSTR)sRecentFileFmt2,
					nItemIndex + 1,
					sDisplayName
					);
				pCmdItem->m_sTipStatus = pCmdItem->m_sTipTool;

				BOOL bInsRetVal =
					ItemInsert(
						nCmdID,
						nMruUpdateIndex + nItemIndex
						);
				
				if( !bInsRetVal )
				{
					ASSERT( FALSE );
					return false;
				}
				nMruInsertCount++;
			} // for( nItemIndex=0; nItemIndex<nRecentCount; nItemIndex++ )
		} // can be null !?
/*
		if( nMruInsertCount > 0 )
		{
			if(		nMruUpdateIndex > 0
				&&	(! _GetItemRef( nMruUpdateIndex - 1 ).IsSeparator() )
				)
			{
				if( !ItemInsert(
						ID_SEPARATOR,
						nMruUpdateIndex
						)
					)
				{
					ASSERT( FALSE );
					return false;
				}
				nMruInsertCount++;
				nMruUpdateIndex++;
			}
			if(		( nMruUpdateIndex + nMruInsertCount ) < m_items_all.GetSize()
				&&	(! _GetItemRef( nMruUpdateIndex + nMruInsertCount ).IsSeparator() )
				)
			{
				if( !ItemInsert(
						ID_SEPARATOR,
						nMruUpdateIndex + nMruInsertCount
						)
					)
				{
					ASSERT( FALSE );
					return false;
				}
				nMruInsertCount++;
			}
		} // if( nMruInsertCount > 0 )
*/
		if(		nMruInsertCount == 0
			&&	nMruUpdateIndex > 0
			&&	nMruUpdateIndex < (ItemGetCount()-1)
			&&	_GetItemRef( nMruUpdateIndex ).IsSeparator()
			&&	_GetItemRef( nMruUpdateIndex-1 ).IsSeparator()
			)
			m_items_all.RemoveAt( nMruUpdateIndex );
	} // insert recently opened file items

	_SyncItems();
	return true;
}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)


CExtPopupMenuSite & CExtPopupMenuWnd::GetSite() const
{
	return CExtPopupMenuSite::g_DefPopupMenuSite;
}

CExtPopupMenuWnd * CExtPopupMenuWnd::GetTrackingMenu()
{
CExtPopupMenuWnd * pPopup =
		CExtPopupMenuSite::g_DefPopupMenuSite.GetInstance();
	if( !(::IsWindow(pPopup->GetSafeHwnd())) )
		return NULL;
	ASSERT_VALID( pPopup );
	return pPopup;
}

void CExtPopupMenuWnd::CancelMenuTracking()
{
	if( !CExtPopupMenuSite::g_DefPopupMenuSite.IsEmpty()
		&& !CExtPopupMenuSite::g_DefPopupMenuSite.IsShutdownMode()
		)
	{
		CExtPopupMenuSite::g_DefPopupMenuSite.DoneInstance();
		ASSERT( CExtPopupMenuSite::g_DefPopupMenuSite.IsEmpty() );
		ASSERT( !CExtPopupMenuSite::g_DefPopupMenuSite.IsShutdownMode() );
	}
}

BOOL CExtPopupMenuWnd::IsMenuTracking()
{
	if( CExtPopupMenuSite::g_DefPopupMenuSite.IsEmpty()
		//|| CExtPopupMenuSite::g_DefPopupMenuSite.GetCapture() == NULL
		)
		return FALSE;
	return TRUE;
}

BOOL CExtPopupMenuWnd::TrackPopupMenu(
	DWORD dwTrackFlags,
	int x,
	int y,
	LPCRECT lpRect, // = NULL
	LPVOID pCbPaintCombinedCookie, // = NULL
	pCbPaintCombinedContent pCbPaintCombinedContent, // = NULL
	UINT * lpnResultCmdID, // = NULL
	bool bCookieIsObject // = false
	)
{
	ASSERT_VALID( this );

	if( lpnResultCmdID != NULL )
		*lpnResultCmdID = 0;

	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );
CWnd * pWndCmdReciever =
		CWnd::FromHandlePermanent( m_hWndCmdReciever );
	if(		pWndCmdReciever != NULL
		&&	CExtControlBar::FindHelpMode( pWndCmdReciever )
		)
		dwTrackFlags |= TPMX_HELP_CTX_MODE;

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	m_pCustomizeSite =
		CExtCustomizeSite::GetCustomizeSite( m_hWndCmdReciever );
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	
	ASSERT(
		(dwTrackFlags&(TPMX_CUSTOMIZE_MODE|TPMX_HELP_CTX_MODE))
		!= (TPMX_CUSTOMIZE_MODE|TPMX_HELP_CTX_MODE)
		);

	if( (dwTrackFlags&TPMX_CUSTOMIZE_MODE) != 0 )
	{
		dwTrackFlags &= ~
			(	TPMX_COMBINE_MASK
				|TPMX_SELECT_ANY
				|TPMX_DO_MESSAGE_LOOP
				|TPMX_OWNERDRAW_FIXED
			);
		dwTrackFlags |=
			TPMX_NO_SHADOWS
			|TPMX_NO_HIDE_RARELY
			|TPMX_NO_WM_COMMAND
			|TPMX_NO_CMD_UI;
	} // if( (dwTrackFlags&TPMX_CUSTOMIZE_MODE) != 0 )

HWND hWndCurrentFocus = ::GetFocus();
	if( hWndCurrentFocus != NULL )
	{
		CWnd * pWndTest = CWnd::FromHandlePermanent( hWndCurrentFocus );
		if( pWndTest == NULL )
		{
			HWND hWnd = ::GetParent( hWndCurrentFocus );
			if( hWnd != NULL )
				pWndTest = CWnd::FromHandlePermanent( hWnd );
		} // if( pWndTest == NULL )
		if( pWndTest != NULL )
		{
			CComboBox * pComboTest =
				DYNAMIC_DOWNCAST( CComboBox, pWndTest );
			if( pComboTest == NULL )
			{
				pWndTest = pWndTest->GetParent();
				if( pWndTest != NULL )
				{
					pComboTest =
						DYNAMIC_DOWNCAST( CComboBox, pWndTest );
				}
			} // if( pComboTest == NULL )
			if(		pComboTest != NULL
				&&	(pComboTest->GetStyle() & (CBS_DROPDOWN|CBS_DROPDOWNLIST)) != 0
				&&	pComboTest->GetDroppedState()
				)
				pComboTest->ShowDropDown( FALSE );
		} // if( pWndTest != NULL )
	} // if( hWndCurrentFocus != NULL )

CExtPopupMenuSite & _site = GetSite();
bool bSiteWasEmpty = _site.IsEmpty();
	if( !bSiteWasEmpty )
	{
		if( _site.IsShutdownMode() )
			return FALSE;
		_site.DoneInstance();
	}

//bool bCaptureChanged = false;
HWND hWndCapture = ::GetCapture();
CWnd * pWndCapture = FromHandlePermanent( hWndCapture );
	if( pWndCapture != NULL )
	{
		ReleaseCapture();
		if( ::IsWindow(hWndCapture) )
		{
			ASSERT_VALID( pWndCapture );
			pWndCapture->SendMessage(WM_CANCELMODE);
		}
//		bCaptureChanged = true;
	}

	ASSERT( _site.IsEmpty() );
	ASSERT( !_site.IsShutdownMode() );

	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );

//	if( !bSiteWasEmpty
//		|| CExtToolControlBar::g_bMenuTracking
//		|| bCaptureChanged
//		)
//	{
//		PassMsgLoop( CExtPopupBaseWnd::g_bEnableOnIdleCalls );
//		if( ! ::IsWindow(m_hWndCmdReciever) )
//			return FALSE;
//	}
//	if( !_site.IsEmpty() )
//		return FALSE;

	ASSERT( m_bTopLevel );
	ASSERT( _site.IsEmpty() );

	if( (dwTrackFlags&TPMX_CUSTOMIZE_MODE) == 0 )
	{
		_site.SetInstance( this );
		if( lpnResultCmdID != NULL )
			_site.SetTargetCmdIdPtr( lpnResultCmdID );
	}

	m_dwTrackFlags = dwTrackFlags;

	if( (dwTrackFlags&TPMX_CUSTOMIZE_MODE) == 0 )
		_UpdateCmdUI();

	if(	! _TrackPopupMenu(
			dwTrackFlags,
			x,
			y,
			lpRect,
			pCbPaintCombinedCookie,
			pCbPaintCombinedContent,
			bCookieIsObject
			)
		)
	{
		//ASSERT( FALSE );
		if( (dwTrackFlags&TPMX_CUSTOMIZE_MODE) == 0 )
		{
			_site.DoneInstance();
			ASSERT( _site.IsEmpty() );
			ASSERT( !_site.IsShutdownMode() );
		} // if( (dwTrackFlags&TPMX_CUSTOMIZE_MODE) == 0 )
		return FALSE;
	}
	if( (dwTrackFlags&TPMX_DO_MESSAGE_LOOP) == 0 )
		return TRUE;
	// do popup menu message loop
	while( IsMenuTracking() )
	{
		WaitMessage();
		HWND hWndThis = GetSafeHwnd();
		CExtPopupMenuWnd::PassMsgLoop( CExtPopupBaseWnd::g_bEnableOnIdleCalls );
		if( !::IsWindow(hWndThis) )
			break;
	} // while( IsMenuTracking() )
	
	if( !CExtToolControlBar::g_bMenuTracking )
	{
		_site.DoneInstance();
		ASSERT( _site.IsEmpty() );
		ASSERT( !_site.IsShutdownMode() );
	}

	return TRUE;
}

void CExtPopupMenuWnd::PassMsgLoop(
	bool bEnableOnIdleCalls
	)
{
#ifdef _USRDLL
	// If this is a DLL, need to set up MFC state
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
MSG msg;
	// Process all the messages in the message queue
	while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
	{
		if( !AfxGetThread()->PumpMessage() )
		{
			PostQuitMessage(0);
			return;
		} // if( !AfxGetThread()->PumpMessage() )
	} // while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
	if( bEnableOnIdleCalls )
	{
		for(	LONG nIdleCounter = 0L;
				::AfxGetThread()->OnIdle(nIdleCounter);
				nIdleCounter ++
				);
	}
}

CSize CExtPopupMenuWnd::_CalcTrackSize()
{
	ASSERT_VALID( this );

int nMenuShadowSize = _GetPopupShadowSize();
int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
int nXtraSz = nMenuBorderSize*2 + nMenuShadowSize;
CSize _size( m_sizeFullItems );
	if( m_bExpandAvailable )
		_size.cy += _GetSpecBtnHeight(); // xpand btn
	_size += CSize( nXtraSz + m_nLeftAreaWidth, nXtraSz );
	if( _size.cx < (nMenuShadowSize*2 + 10) )
		_size.cx = nMenuShadowSize*2 + 10;
	if( _size.cy < (nMenuShadowSize*2 + 10) )
		_size.cy = nMenuShadowSize*2 + 10;
	return _size;
}

CRect CExtPopupMenuWnd::_CalcTrackRect()
{
	ASSERT_VALID( this );

	m_eCombineAlign = __CMBA_NONE;

	if( IsKeyPressed(VK_CONTROL) )
	{
		int i=1;
		i;
	}

CExtPaintManager::monitor_parms_t _mp;
	if( m_bExcludeAreaSpec )
		CExtPaintManager::stat_GetMonitorParms( _mp, m_rcExcludeArea );
	else if( m_pWndParentMenu != NULL )
	{
		ASSERT_VALID( m_pWndParentMenu );
		ASSERT( m_pWndParentMenu->GetSafeHwnd() != NULL );
		ASSERT( ::IsWindow( m_pWndParentMenu->GetSafeHwnd() ) );
		CExtPaintManager::stat_GetMonitorParms( _mp, m_pWndParentMenu );
	} // else if( m_pWndParentMenu != NULL )
	else
		CExtPaintManager::stat_GetMonitorParms( _mp, m_ptTrack );
CRect rcDesktop = g_bUseDesktopWorkArea
		? _mp.m_rcWorkArea
		: _mp.m_rcMonitor
		;

CSize _size = _CalcTrackSize();
int nMenuShadowSize =
		_GetPopupShadowSize();
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nGapShift = 0;
	if( !m_bCombineWithEA )
		nGapShift = nMenuBorderSize + nMenuShadowSize;

	if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == 0 && (!m_bCombineWithEA) )
	{
		if( m_pWndParentMenu->GetSafeHwnd() != NULL )
		{
			if( (m_pWndParentMenu->m_dwTrackFlags&TPMX_ALIGN_MASK) != 0 )
				m_dwTrackFlags |= m_pWndParentMenu->m_dwTrackFlags& (~TPMX_ALIGN_MASK);
			else
			{
				CRect rc1;
				m_pWndParentMenu->GetWindowRect(&rc1);
				if( m_pWndParentMenu->m_pWndParentMenu->GetSafeHwnd() != NULL )
				{
					CRect rc2;
					m_pWndParentMenu->m_pWndParentMenu->GetWindowRect(&rc2);
					int x1 = rc1.left + rc1.Width()/2;
					int x2 = rc2.left + rc2.Width()/2;
					if( x1 >= x2 )
						m_dwTrackFlags |= TPMX_LEFTALIGN;
					else
						m_dwTrackFlags |= TPMX_RIGHTALIGN;
				}
			}
		}
	}

CRect wr(m_ptTrack,_size);
//	wr.NormalizeRect();
int nCombineOffset = 0;
	if(	m_bCombineWithEA )
	{
		if( (m_dwTrackFlags&TPMX_ALIGN_MASK)
				== TPMX_TOPALIGN )
		{
			nCombineOffset = 1;
			wr.OffsetRect(0,-nCombineOffset);
		}
		if( (m_dwTrackFlags&TPMX_ALIGN_MASK)
				== TPMX_LEFTALIGN )
		{
			nCombineOffset = 1;
			wr.OffsetRect(-nCombineOffset,0);
		}
		if( (m_dwTrackFlags&TPMX_ALIGN_MASK)
				== TPMX_BOTTOMALIGN )
		{
			nCombineOffset =
				-nMenuShadowSize - 1;
			wr.OffsetRect(0,-nCombineOffset);
		}
		if( (m_dwTrackFlags&TPMX_ALIGN_MASK)
				== TPMX_RIGHTALIGN )
		{
			nCombineOffset =
				-nMenuShadowSize - 1;
			wr.OffsetRect(-nCombineOffset,0);
		}
	} // if(	m_bCombineWithEA )

	if(wr.bottom > rcDesktop.bottom)
	{
		if( m_bExcludeAreaSpec )
		{
			wr.OffsetRect(
				0,
				-wr.Height()
					+ m_rcExcludeArea.Height()
					+ nGapShift
					+ (m_bCombineWithEA ?
						nMenuShadowSize : 0)
				);
		} // if( m_bExcludeAreaSpec )
		if(wr.bottom > rcDesktop.bottom)
			wr.OffsetRect(
				0,
				m_bExcludeAreaSpec
					? -(wr.bottom - rcDesktop.bottom)
					: - wr.Height()
				);
	} // if(wr.bottom > rcDesktop.bottom)
	if(wr.top < rcDesktop.top)
		wr.OffsetRect( 0, rcDesktop.top - wr.top );

	if(wr.right > rcDesktop.right)
	{
		if( !m_bTopLevel && m_bExcludeAreaSpec )
			wr.OffsetRect(
				-wr.Width() - m_rcExcludeArea.Width(),
				0
				);
		if(wr.right > rcDesktop.right)
			wr.OffsetRect(
				m_bExcludeAreaSpec
					? -(wr.right - rcDesktop.right)
					: -wr.Width(),
				0
				);
	} // if(wr.right > rcDesktop.right)
	if(wr.left < rcDesktop.left)
	{
		if( !m_bTopLevel && m_bExcludeAreaSpec )
			wr.OffsetRect(
				wr.Width() + m_rcExcludeArea.Width(),
				0
				);
		if(wr.left < rcDesktop.left)
			wr.OffsetRect( rcDesktop.left - wr.left, 0 );
	} // if(wr.left < rcDesktop.left)

	if( m_bExcludeAreaSpec || m_bCombineWithEA )
	{
		CRect rcIntersection;
		if(	rcIntersection.IntersectRect(
				&wr,
				&m_rcExcludeArea
				)
			|| m_bCombineWithEA
			)
		{ // if need adjust with exclude area
			CRect rcFree(0,0,0,0);
			if( m_rcExcludeArea.left > rcDesktop.left )
				rcFree.left =
					m_rcExcludeArea.left - rcDesktop.left;
			if( m_rcExcludeArea.right < rcDesktop.right )
				rcFree.right =
					rcDesktop.right - m_rcExcludeArea.right;
			if( m_rcExcludeArea.top > rcDesktop.top )
				rcFree.top =
					m_rcExcludeArea.top - rcDesktop.top;
			if( m_rcExcludeArea.bottom < rcDesktop.bottom )
				rcFree.bottom =
					rcDesktop.bottom - m_rcExcludeArea.bottom;
			if( (m_dwTrackFlags&TPMX_ALIGN_MASK)
					== TPMX_LEFTALIGN
				&& rcFree.right > wr.Width()
				)
				wr.OffsetRect(
					wr.left - m_rcExcludeArea.right
						+ (m_bCombineWithEA ?
							nCombineOffset : 0)
						,
					0
					);
			if( (m_dwTrackFlags&TPMX_ALIGN_MASK)
					== TPMX_RIGHTALIGN
				&& rcFree.left > wr.Width()
				)
				wr.OffsetRect(
					m_rcExcludeArea.left - wr.right
						- (m_bCombineWithEA ?
							nCombineOffset : 0)
						,
					0
					);

//			if( (m_dwTrackFlags&TPMX_ALIGN_MASK)
//					== TPMX_TOPALIGN
//				&& rcFree.bottom > wr.Height()
//				)
//				wr.OffsetRect(
//					0,
//					wr.top - m_rcExcludeArea.bottom
//						+ (m_bCombineWithEA ?
//							nCombineOffset : 0)
//					);

			if( (m_dwTrackFlags&TPMX_ALIGN_MASK)
					== TPMX_TOPALIGN
				)
			{
				if( rcFree.bottom > wr.Height() )
					wr.OffsetRect(
						0,
						wr.top - m_rcExcludeArea.bottom
							+ (m_bCombineWithEA ?
								nCombineOffset : 0)
						);
				else
				{
					if( ::IsWindow(GetSafeHwnd()) )
					{
						CRect rcWndOld;
						GetWindowRect( &rcWndOld );
						ShowWindow( SW_HIDE );
						_FreeWinObjects();
						HWND hWndThis = GetSafeHwnd();
						bool bWasCreated =
							(hWndThis != NULL && ::IsWindow(hWndThis))
								? true : false;
						CExtPopupMenuWnd::PassMsgLoop( CExtPopupBaseWnd::g_bEnableOnIdleCalls );
						if( bWasCreated && !::IsWindow(hWndThis) )
							return CRect(0,0,0,0);
						if( !rcWndOld.IsRectEmpty() )
						{
							CWnd * pWndTLP = GetTopLevelParent();
							if( pWndTLP != NULL )
							{
								ASSERT_VALID( pWndTLP );
								if( pWndTLP->IsWindowVisible() )
								{
									CRect rcTLP;
									pWndTLP->GetWindowRect( &rcTLP );
									if( !rcTLP.IsRectEmpty() )
									{
										CRect rcIntersection;
										if(	rcIntersection.IntersectRect(
												&rcTLP,
												&rcWndOld
												)
											)
											pWndTLP->UpdateWindow();
									} // if( !rcTLP.IsRectEmpty() )
								} // if( pWndTLP->IsWindowVisible() )
							} // if( pWndTLP != NULL )
						} // if( !rcWndOld.IsRectEmpty() )
					}
					m_dwTrackFlags &= ~TPMX_ALIGN_MASK;
					m_dwTrackFlags |= TPMX_BOTTOMALIGN;
					return _CalcTrackRect();
				}

			}

			if( (m_dwTrackFlags&TPMX_ALIGN_MASK)
					== TPMX_BOTTOMALIGN
					&& rcFree.top > wr.Height()
				)
				wr.OffsetRect(
					0,
					m_rcExcludeArea.top - wr.bottom
						- (m_bCombineWithEA ?
							nCombineOffset : 0)
					);
		} // if need adjust with exclude area
	} // if( m_bExcludeAreaSpec || m_bCombineWithEA )

	if(wr.right > rcDesktop.right)
	{
		wr.OffsetRect( -(wr.right - rcDesktop.right), 0 );
		if( m_bTopLevel )
		{
			m_bExcludeAreaSpec = false;
			m_bCombineWithEA = false;
		}
	} // if(wr.right > rcDesktop.right)
	if(wr.left < rcDesktop.left)
	{
		wr.OffsetRect( rcDesktop.left - wr.left, 0 );
		if( m_bTopLevel )
		{
			m_bExcludeAreaSpec = false;
			m_bCombineWithEA = false;
		}
	} // if(wr.left < rcDesktop.left)
	if(wr.bottom > rcDesktop.bottom)
	{
		wr.OffsetRect( 0, -(wr.bottom - rcDesktop.bottom) );
		if( m_bTopLevel )
		{
			m_bExcludeAreaSpec = false;
			m_bCombineWithEA = false;
		}
	} // if(wr.bottom > rcDesktop.bottom)
	if(wr.top < rcDesktop.top)
	{
		wr.OffsetRect( 0, rcDesktop.top - wr.top );
		if( m_bTopLevel )
		{
			m_bExcludeAreaSpec = false;
			m_bCombineWithEA = false;
		}
	} // if(wr.top < rcDesktop.top)
	if(wr.bottom > rcDesktop.bottom)
	{
		wr.bottom = rcDesktop.bottom;
		m_bScrollingAvailable = true;
	}

	if( m_bCombineWithEA )
	{
		ASSERT( m_bExcludeAreaSpec );
		if( wr.bottom == m_rcExcludeArea.top - nCombineOffset )
			m_eCombineAlign = __CMBA_BOTTOM;
		if( wr.top == m_rcExcludeArea.bottom  - nCombineOffset )
			m_eCombineAlign = __CMBA_TOP;
		if( wr.left == m_rcExcludeArea.right - nCombineOffset )
			m_eCombineAlign = __CMBA_LEFT;
		if( wr.right == m_rcExcludeArea.left - nCombineOffset  )
			m_eCombineAlign = __CMBA_RIGHT;
		if( m_eCombineAlign == __CMBA_NONE )
		{
			m_bCombineWithEA = false;
			return _CalcTrackRect();
		} // if( m_eCombineAlign == __CMBA_NONE )
		// init window region
		if( m_rgnWnd.GetSafeHandle() != NULL )
		{
			VERIFY(
				m_rgnWnd.DeleteObject()
				);
		}
		CRect rcExcludeArea(m_rcExcludeArea);
		rcExcludeArea.InflateRect(
			0,
			0,
			nMenuShadowSize,
			nMenuShadowSize
			);
		CRect rcFullWnd(
			min(wr.left,rcExcludeArea.left),
			min(wr.top,rcExcludeArea.top),
			max(wr.right,rcExcludeArea.right),
			max(wr.bottom,rcExcludeArea.bottom)
			);

		if( !m_bTopLevel )
		{
			if( m_eCombineAlign == __CMBA_LEFT
				|| m_eCombineAlign == __CMBA_RIGHT
				)
				rcExcludeArea.DeflateRect( 0, 0, 0, nMenuShadowSize );
			else if( m_eCombineAlign == __CMBA_TOP
				|| m_eCombineAlign == __CMBA_BOTTOM
				)
				rcExcludeArea.DeflateRect( 0, 0, nMenuShadowSize, 0 );
		}
		
		CRgn rgnAreaWnd,rgnAreaExclude;
		if(	(!m_rgnWnd.CreateRectRgnIndirect(
				&rcFullWnd ))
			||
			(!rgnAreaExclude.CreateRectRgnIndirect(
				&rcExcludeArea ))
			||
			(!rgnAreaWnd.CreateRectRgnIndirect(
				&wr ))
			||
			m_rgnWnd.CombineRgn(
				&rgnAreaWnd,
				&rgnAreaExclude,
				RGN_OR
				) == ERROR
			||
			m_rgnWnd.OffsetRgn(
				-rcFullWnd.TopLeft()
				) == ERROR
			)
		{
			if( m_rgnWnd.GetSafeHandle() != NULL )
			{
				VERIFY(
					m_rgnWnd.DeleteObject()
					);
			}
			m_bCombineWithEA = false;
			return _CalcTrackRect();
		}
		ASSERT( m_rgnWnd.GetSafeHandle() != NULL );
		
//		VERIFY( m_rgnWnd.DeleteObject() );
//		VERIFY(
//			m_rgnWnd.CreateRectRgnIndirect(
//				&rcFullWnd
//				)
//			);

		m_rcClient = wr;
		m_rcClient.OffsetRect( -rcFullWnd.TopLeft() );
		m_rcClient.DeflateRect(
			0,
			0,
			nMenuShadowSize,
			nMenuShadowSize
			);
		ASSERT( m_rcClient.top <= m_rcClient.bottom );
		ASSERT( m_rcClient.left <= m_rcClient.right );

		if( !m_bTopLevel )
		{
			if( m_eCombineAlign == __CMBA_RIGHT )
				rcFullWnd.OffsetRect( nMenuShadowSize + 1, 0 );
			else if( m_eCombineAlign == __CMBA_LEFT )
				rcFullWnd.OffsetRect( -2, 0 );
		}

		return rcFullWnd;
	} // if( m_bCombineWithEA )

	m_rcClient = wr;
	m_rcClient.OffsetRect( -wr.TopLeft() );
	m_rcClient.DeflateRect(
		0,
		0,
		nMenuShadowSize,
		nMenuShadowSize
		);
	ASSERT( m_rcClient.top <= m_rcClient.bottom );
	ASSERT( m_rcClient.left <= m_rcClient.right );

	return wr;
}

BOOL CExtPopupMenuWnd::_TrackPopupMenu(
	DWORD dwTrackFlags,
	int x,
	int y,
	LPCRECT lpRect,
	LPVOID pCbPaintCombinedCookie, // = NULL
	pCbPaintCombinedContent pCbPaintCombinedContent, // = NULL
	bool bCookieIsObject // = false
	)
{
	ASSERT_VALID( this );

bool bForceExpandRarelyUsed = (dwTrackFlags&TPMX_NO_HIDE_RARELY)
		? true : false;
	if( !g_bMenuExpanding )
		bForceExpandRarelyUsed = true;

	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );

	ASSERT( GetSafeHwnd() == NULL );

CExtPopupMenuSite & _site = GetSite();

	if(		(!_FindCustomizeMode())
		&&	_site.IsTopPupup(this)
		)
	{
		ASSERT( m_bTopLevel );
		MsgPrepareMenuData_t _mpmd( this );
		_mpmd.SendMessage( m_hWndCmdReciever );
		if( _mpmd.m_bMenuCanceled )
			return FALSE;
		if( _mpmd.m_bMenuChanged )
		{
			_UpdateCmdUI();
		}
		ASSERT( m_bTopLevel );
		ASSERT( _site.IsTopPupup(this) );
	}

	if( !_FindCustomizeMode() )
	{ // BLOCK: update system commands
		INT iter = 0;
		for(; iter < m_items_all.GetSize(); ++iter )
		{
			MENUITEMDATA & mi = m_items_all[ iter ];
			if( mi.IsSeparator() )
				continue;
			UINT nCmdID = mi.GetCmdID();
			if( !CExtCmdManager::IsSystemCommand(nCmdID))
				continue;
			WINDOWPLACEMENT wndpl;
			::memset(&wndpl,0,sizeof(WINDOWPLACEMENT));
			wndpl.length = sizeof(WINDOWPLACEMENT);
			VERIFY(
				::GetWindowPlacement(
					mi.GetCmdReciever(),
					&wndpl
					)
				);
			DWORD dwWndStyle = (DWORD)
				::GetWindowLong(
					mi.GetCmdReciever(),
					GWL_STYLE
					);
			DWORD dwWndExStyle = (DWORD)
				::GetWindowLong(
					mi.GetCmdReciever(),
					GWL_EXSTYLE
					);
			bool bSysCmdEnabled = false;
			switch( nCmdID )
			{
			case SC_CLOSE:
				{
					bSysCmdEnabled = true;
					HMENU hSysMenu = ::GetSystemMenu( mi.GetCmdReciever(), FALSE );
					MENUITEMINFO _mii;
					::memset( &_mii, 0, sizeof(MENUITEMINFO) );
					_mii.cbSize = sizeof(MENUITEMINFO);
					_mii.fMask = MIIM_STATE;
					if(		hSysMenu != NULL
						&&	::GetMenuItemInfo(
								hSysMenu,
								SC_CLOSE,
								FALSE,
								&_mii
								)
						)
					{
						if( (_mii.fState & MFS_DISABLED) != 0 )
							bSysCmdEnabled = false;
					} // if( hSysMenu != NULL ...
				}
			break;
			case SC_SIZE:
			case SC_MOVE:
				if(		wndpl.showCmd != SW_SHOWMINIMIZED
					&&	wndpl.showCmd != SW_SHOWMAXIMIZED
					)
					bSysCmdEnabled = true;
			break;
			case SC_MINIMIZE:
				if(		(dwWndStyle & WS_MINIMIZEBOX) != 0
					&&	wndpl.showCmd != SW_SHOWMINIMIZED
					)
					bSysCmdEnabled = true;
			break;
			case SC_MAXIMIZE:
				if(		(dwWndStyle & WS_MAXIMIZEBOX) != 0
					&&	wndpl.showCmd != SW_SHOWMAXIMIZED
					)
					bSysCmdEnabled = true;
			break;
			case SC_RESTORE:
				if(		(dwWndStyle & (WS_MINIMIZEBOX|WS_MAXIMIZEBOX)) != 0
					&&	(	wndpl.showCmd == SW_SHOWMAXIMIZED
						||	wndpl.showCmd == SW_SHOWMINIMIZED
						//||wndpl.showCmd == SW_SHOWNORMAL
						)
					)
					bSysCmdEnabled = true;
			break;
			case SC_CONTEXTHELP:
				if( (dwWndExStyle & WS_EX_CONTEXTHELP) != 0 )
					bSysCmdEnabled = true;
			break;
//			case SC_NEXTWINDOW:
//			case SC_PREVWINDOW:
//			case SC_VSCROLL:
//			case SC_HSCROLL:
//			case SC_MOUSEMENU:
//			case SC_KEYMENU:
//			case SC_ARRANGE:
//			case SC_TASKLIST:
//			case SC_SCREENSAVE:
//#if(WINVER >= 0x0400)
//			case SC_DEFAULT:
//			case SC_MONITORPOWER:
//			case SC_SEPARATOR:
//#endif /* WINVER >= 0x0400 */
			case SC_HOTKEY:
			default:
				continue;
			} // switch( nCmdID )
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd( m_hWndCmdReciever ),
					nCmdID
					);
			ASSERT( pCmdItem != NULL );
			pCmdItem->StateEnable( bSysCmdEnabled );
			mi.Enable( bSysCmdEnabled );
		} // for(; iter < m_items_all.end(); ++iter )
	} // BLOCK: update system commands

CWnd * pWndCmdReciever =
		CWnd::FromHandle( m_hWndCmdReciever );
	ASSERT_VALID( pWndCmdReciever );
//	pWndCmdReciever->ActivateTopParent();
//	pWndCmdReciever->BringWindowToTop();
//	pWndCmdReciever->SetFocus();
	
	//_site._Hook( true );

	// adjust own data
bool bOldTopLevel = m_bTopLevel;
bool bOldExpandAvailable = m_bExpandAvailable;
	_Init();
	m_bTopLevel = bOldTopLevel;
	m_bExpandAvailable = bOldExpandAvailable;
	m_dwTrackFlags = dwTrackFlags;
	m_pCbPaintCombinedCookie = pCbPaintCombinedCookie;
	m_pCbPaintCombinedContent = pCbPaintCombinedContent;
	m_bCookieIsObject = bCookieIsObject;
	if( !m_bTopLevel )
	{
		ASSERT( m_pWndParentMenu != NULL );
		if( m_pWndParentMenu->m_bExpandWasPressed )
		{
			if( m_bExpandAvailable )
			{
				m_bExpandAvailable = false;
				m_bExpandWasPressed = true;
				_SyncItems();
			}
			else
				m_bExpandWasPressed = true;
		}
	} // if( !m_bTopLevel )
	else
	{
		if( bForceExpandRarelyUsed )
		{
			if( m_bExpandAvailable )
			{
				m_bExpandAvailable = false;
				m_bExpandWasPressed = true;
				_SyncItems();
			}
			else
				m_bExpandWasPressed = true;
		} // if( bForceExpandRarelyUsed )
		else
			_SyncItems();
	} // else from if( !m_bTopLevel )

	// adjust screen position
	m_ptTrack.x = x;
	m_ptTrack.y = y;
	if( m_ptTrack.x < 0 || m_ptTrack.y < 0 )
	{
		if( ! ::GetCursorPos(&m_ptTrack) )
			return FALSE;
	}

	if( lpRect != NULL )
	{
		m_rcExcludeArea = *lpRect;
		m_bExcludeAreaSpec = true;
	}
	else
	{
		m_bExcludeAreaSpec = false;
		m_rcExcludeArea.left = m_ptTrack.x - __EXCLUDE_AREA_GAP_DX;
		m_rcExcludeArea.right = m_ptTrack.x + __EXCLUDE_AREA_GAP_DX;
		m_rcExcludeArea.top = m_ptTrack.y - __EXCLUDE_AREA_GAP_DY;
		m_rcExcludeArea.bottom = m_ptTrack.y + __EXCLUDE_AREA_GAP_DY;
	}

	// adjust combine with exclude area mode
	m_bCombineWithEA = false;
	if( m_bExcludeAreaSpec
		// && m_bTopLevel 
		)
	{
		switch( (dwTrackFlags&TPMX_COMBINE_MASK) )
		{
		case TPMX_COMBINE_ANY_SUITABLE:
			m_bCombineWithEA = true;
		break;
		case TPMX_COMBINE_DEFAULT:
			m_bCombineWithEA =
				g_PaintManager->
					IsMenuMustCombineExcludeArea();
		break;
		} // switch( (dwTrackFlags&TPMX_COMBINE_MASK) )
	} // if( m_bExcludeAreaSpec ...

CSize _size = _CalcTrackSize();
bool bPointAdjusted = true;
	if( m_bExcludeAreaSpec )
	{
		switch( (m_dwTrackFlags & TPMX_ALIGN_MASK) )
		{
		case TPMX_LEFTALIGN:
			m_ptTrack.x = m_rcExcludeArea.right;
			m_ptTrack.y = m_rcExcludeArea.top;
		break;
		case TPMX_RIGHTALIGN:
			m_ptTrack.x = m_rcExcludeArea.left - _size.cx;
			m_ptTrack.y = m_rcExcludeArea.top;
		break;
		case TPMX_TOPALIGN:
			m_ptTrack.x = m_rcExcludeArea.left;
			m_ptTrack.y = m_rcExcludeArea.bottom;
		break;
		case TPMX_BOTTOMALIGN:
			m_ptTrack.x = m_rcExcludeArea.left;
			m_ptTrack.y = m_rcExcludeArea.top - _size.cy;
		break;
		default:
			bPointAdjusted = false;
		break;
		} // switch( (m_dwTrackFlags & TPMX_ALIGN_MASK) )
	} // if( m_bExcludeAreaSpec )
	if( !bPointAdjusted )
	{
		if( (m_dwTrackFlags & TPMX_ALIGN_MASK)
				== TPMX_RIGHTALIGN )
			m_ptTrack.x -= _size.cx;
		else
		{
			if( (m_dwTrackFlags & TPMX_ALIGN_MASK)
					== TPMX_CENTERALIGN )
				m_ptTrack.x -= _size.cx/2;
		}
		if( (m_dwTrackFlags & TPMX_ALIGN_MASK)
				== TPMX_BOTTOMALIGN )
			m_ptTrack.y -= _size.cy;
		else
		{
			if( (m_dwTrackFlags & TPMX_ALIGN_MASK)
					== TPMX_VCENTERALIGN )
				m_ptTrack.y -= _size.cy/2;
		}
	} // if( !bPointAdjusted )

//#ifdef _DEBUG
bool bCreateResult =
//#endif // _DEBUG
		_CreateHelper( pWndCmdReciever );
	//ASSERT( bCreateResult );
	if( !bCreateResult )
	{
		//_OnCancelMode();
		return FALSE;
	}

	if( dwTrackFlags & TPMX_SELECT_ANY )
	{
		int nItem = _GetNextItem(__NI_ANY);
		if( nItem >= 0 )
			_ItemFocusSet(
				nItem,
				FALSE,
				FALSE
				);
	}

	if( _FindCustomizeMode() )
		m_AnimationType = __AT_NONE;
	else
	{
		m_AnimationType = g_DefAnimationType;
		if( CExtToolControlBar::g_bMenuTracking )
			m_AnimationType = __AT_NONE;

		m_ePlaySoundOnAnimationFinished =
			CExtSoundPlayer::__ON_MENU_POPUP_DISPLAYED;
		_StartAnimation();
	} // if( _FindCustomizeMode() )
	if( m_AnimationType == __AT_NONE )
	{
		m_AnimationType = __AT_CONTENT_DISPLAY;
		_StartAnimation();
		ASSERT( m_AnimationType == __AT_CONTENT_DISPLAY );
	}

	if( m_rgnWnd.GetSafeHandle() != NULL )
	{
		ASSERT( m_bExcludeAreaSpec );
		ASSERT( m_bCombineWithEA );
		ASSERT( m_eCombineAlign != __CMBA_NONE );
		CRgn rgnTmp;
		VERIFY( rgnTmp.CreateRectRgn(0,0,0,0) );
		rgnTmp.CopyRgn( &m_rgnWnd );
		ASSERT( rgnTmp.GetSafeHandle() != NULL );
		VERIFY(
			SetWindowRgn(
				(HRGN)rgnTmp.Detach(),
				FALSE
				)
			);
	} // if( m_rgnWnd.GetSafeHandle() != NULL )
	SetWindowPos(
		&CWnd::wndTopMost, 0, 0, 0, 0,
		SWP_NOACTIVATE
			|SWP_NOMOVE|SWP_NOSIZE
			|SWP_SHOWWINDOW
			//|SWP_NOREDRAW //|SWP_NOSENDCHANGING
			|SWP_NOZORDER //|SWP_NOOWNERZORDER
			//|SWP_NOCOPYBITS
		);

	ASSERT( IsWindowVisible() );

	if( !_FindCustomizeMode() )
		_SetCapture();
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	else
	{
		CExtCustomizeSite * pSite = _FindCustomizeSite();
		ASSERT( pSite != NULL );
		pSite->RegisterCommandDropTarget( this, this );
	}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	return TRUE;
}

bool CExtPopupMenuWnd::_CreateHelper(
	CWnd * pWndCmdReciever
	)
{
	ASSERT_VALID( this );

bool bHasInplaceItems = false;
int nCount = ItemGetCount();
	for( int i = 0; i < nCount; i++ )
	{
		MENUITEMDATA & mi = _GetItemRef( i );
		if( !mi.IsInplaceEdit() )
			continue;
		//ASSERT( mi.GetInplaceEditPtr() == NULL );
		mi.ResetActiveInplaceEditPtr();
		bHasInplaceItems = true;
		break;
	} // for( int i = 0; i < nCount; i++ )
	if( bHasInplaceItems )
		_SyncItems();

CRect wr = _CalcTrackRect();
	if( wr.IsRectEmpty() )
		return false;

HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);
	ASSERT( hCursor != NULL );
CExtSafeString strMenuClassName =
		::AfxRegisterWndClass(
			__POPUP_WNDCLASS_STYLES__,
			hCursor,
			(HBRUSH)(COLOR_BTNFACE + 1),
			NULL
			);
	ASSERT( !strMenuClassName.IsEmpty() );

BOOL bCreateResult =
		__BaseClassOfCExtPopupBaseWnd::CreateEx(
			_FindCustomizeMode() ? 0 : WS_EX_TOPMOST,
			strMenuClassName,
			NULL,
			WS_POPUP,
			wr,
			pWndCmdReciever,
			0 // IDC_STATIC
			);
	if( !bCreateResult )
	{
		ASSERT( FALSE );
		return false;
	}

	_RecalcLayoutImpl();

	if( !_FindHelpMode() )
		::SetCursor( hCursor );

//HWND hWndOwn = GetSafeHwnd();
//	ASSERT( hWndOwn != NULL );
//	ASSERT( ::IsWindow(hWndOwn) );
//	PassMsgLoop( CExtPopupBaseWnd::g_bEnableOnIdleCalls ); // update all bottom
//	if( !::IsWindow(hWndOwn) )
//		return false;

	if( bHasInplaceItems )
	{
		visible_items_t v;
		_GetVisibleItems( NULL, v );
		int nCount = v.GetSize();
		for( i = 0; i < nCount; i++ )
		{
//			MENUITEMDATA & mi = _GetItemRef( i );
VisibleItemDefinition_t & vi = v[ i ];
MENUITEMDATA & mi = _GetItemRef( vi.m_nIndex );
			if( !mi.IsInplaceEdit() )
				continue;
			//ASSERT( mi.GetInplaceEditPtr() == NULL );
			mi.ResetActiveInplaceEditPtr();
//			VisibleItemDefinition_t & vi = v[ i ];
			CRect rc = mi.AdjustInplaceEditRect( vi.m_rcItem );
			mi.CreateInplaceEdit( this, rc );
		} // for( i = 0; i < nCount; i++ )
	} // if( bHasInplaceItems )

	return true;
}

BOOL CExtPopupMenuWnd::DestroyWindow()
{
	ASSERT_VALID( this );
int nCount = ItemGetCount();
	for( int i = 0; i < nCount; i++ )
	{
		MENUITEMDATA & mi = _GetItemRef( i );
		if( mi.IsInplaceEdit() )
		{
			if( mi.GetInplaceEditPtr() != NULL )
			{
				ASSERT_VALID( mi.GetInplaceEditPtr() );
				ASSERT( mi.GetInplaceEditPtr()->GetSafeHwnd() != NULL && (::IsWindow(mi.GetInplaceEditPtr()->GetSafeHwnd())) );
				mi.GetInplaceEditPtr()->DestroyWindow();
			} // if( mi.GetInplaceEditPtr() != NULL )
			mi.ResetActiveInplaceEditPtr();
		}
	} // for( int i = 0; i < nCount; i++ )
	if( !(::IsWindow(GetSafeHwnd())) )
		return TRUE;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( _FindCustomizeMode() )
	{
		CExtCustomizeSite * pSite = _FindCustomizeSite();
		ASSERT( pSite != NULL );
		pSite->UnRegisterCommandDropTarget( this );
	} // if( _FindCustomizeMode() )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	return CExtPopupBaseWnd::DestroyWindow();
}

void CExtPopupMenuWnd::_GetItemRect(
	int nIndex,
	RECT & rectItem
	)
{
	ASSERT_VALID( this );

CRect rcClient;
	_GetClientRect( &rcClient );

	rcClient.left += m_nLeftAreaWidth;

	if( m_bScrollingAvailable )
	{
		ASSERT( !m_rcScrollTop.IsRectEmpty() );
		ASSERT( !m_rcScrollBottom.IsRectEmpty() );
		if( nIndex == IDX_SCROLL_TOP )
		{
			rectItem = m_rcScrollTop;
			return;
		}
		if( nIndex == IDX_SCROLL_BOTTOM )
		{
			rectItem = m_rcScrollBottom;
			return;
		}
	}
	if( m_bExpandAvailable
		&& nIndex == IDX_EXPAND
		)
	{
		rectItem = m_rcExpandBtn;
		return;
	}

int nCountOfItems = m_items_all.GetSize();
	if( nCountOfItems == 0 )
	{
		((CRect *)(&rectItem))->SetRectEmpty();
		return;
	}
	ASSERT( nIndex >= 0 && nIndex < nCountOfItems );
MENUITEMDATA & mi = m_items_all[nIndex];
	if( !mi.IsDisplayed() )
	{
		::SetRectEmpty( &rectItem );
		return;
	}

int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int y = m_nDyScrollOffset + nMenuBorderSize;

CRect rcItem;
	rcItem.SetRectEmpty();
	if( mi.IsToolButton() )
	{
		rcItem.left =
			nMenuBorderSize
			+ 1 + __EXT_MENU_TOOLBTN_GAP
			;
		rcItem.right =
			mi.GetMeasuredWidth()
			;
	} // if( mi.IsToolButton() )
	else
	{
		rcItem.left =
			nMenuBorderSize
			+ 1
			;
		rcItem.right =
			m_sizeFullItems.cx
			+ nMenuBorderSize
			- 1 
			;
	} // else from if( mi.IsToolButton() )

INT iter = 0;
	for(	int i = 0;
			i < nIndex
				&& iter != m_items_all.GetSize();
			i++, iter++
			)
	{
		MENUITEMDATA & mi = m_items_all[ iter ];
		if( !mi.IsDisplayed() )
			continue;
		const RECT & rcBase =
			mi.GetBaseRect();
		y +=  rcBase.bottom - rcBase.top;
	}
	ASSERT( iter != m_items_all.GetSize() );
	rcItem.OffsetRect(0,y);
	mi = m_items_all[ iter ];
	ASSERT( mi.IsDisplayed() );
const RECT & rcBase = mi.GetBaseRect();
	rcItem.bottom =
		rcItem.top + rcBase.bottom - rcBase.top;
	
	// FIX for combined exclude area
	rcItem.OffsetRect( rcClient.TopLeft() );
	
	rectItem = rcItem;
}

void CExtPopupMenuWnd::_GetVisibleItems(
	HDC hDcTest, // may be NULL
	visible_items_t & v
	)
{
	ASSERT_VALID( this );

	v.RemoveAll();
INT iter = 0;
	for(	int nIndex = 0;
			iter != m_items_all.GetSize();
			iter++, nIndex++
			)
	{ // find visible items
		MENUITEMDATA & mi = m_items_all[ iter ];
		if( !mi.IsDisplayed() )
			continue;
		CRect rcItem;
		_GetItemRect( nIndex, rcItem );
		if( rcItem.IsRectEmpty() )
			continue;
		if(		hDcTest != NULL
			&&	(! ::RectVisible( hDcTest, &rcItem ) )
			)
			continue;
		VisibleItemDefinition_t vi;
		vi.m_nIndex = nIndex;
		vi.m_nHelperCmdID = mi.GetCmdID();
		vi.m_rcItem = rcItem;
		if( mi.IsSeparator() )
		{
			vi.m_bHelperIsSeparator = true;
			v.Add( vi );
			continue;
		}
		if( mi.IsPopup() )
		{
			vi.m_bHelperIsPopup = true;
			CExtCmdItem * pCmdItem = mi.GetCmd();
			if( pCmdItem != NULL )
			{
				vi.m_bRarelyUsed =
					(	(!_FindCustomizeMode())
					&&	pCmdItem->StateIsRarelyUsed()
					) ? true : false;
			} // if( pCmdItem != NULL )
			else
				vi.m_bRarelyUsed =
						(!_FindCustomizeMode())
					&&	mi.IsAllItemsRarelyUsed()
					&&	(!mi.IsForceDisplayed())
					;
			v.Add( vi );
			continue;
		}
		if( mi.IsExtraMark() )
			vi.m_bRarelyUsed = false;
		else
		{
			CExtCmdItem * pCmdItem = mi.GetCmd();
			if( pCmdItem != NULL )
				vi.m_bRarelyUsed =
					(	(!_FindCustomizeMode())
					&&	pCmdItem->StateIsRarelyUsed()
					) ? true : false;
			else
				vi.m_bRarelyUsed = false;
		} // else from if( mi.IsExtraMark() )
		v.Add( vi );
	} // find visible items
INT vis_iter = 0;
	for( ; vis_iter < v.GetSize(); ++vis_iter )
	{ // setup prev/next rarely used flags
		VisibleItemDefinition_t & vi = v[ vis_iter ];
		INT iter_prev = vis_iter;
		INT iter_next = vis_iter;
		if( vis_iter != 0 )
		{
			--iter_prev;
			if( v[iter_prev].m_bRarelyUsed )
				vi.m_bRarelyUsedPrev = true;
		} // if( vis_iter != v.begin() )
		++iter_next;
		if( iter_next != v.GetSize() )
		{
			if( v[iter_next].m_bRarelyUsed )
				vi.m_bRarelyUsedNext = true;
		} // if( iter_next != v.end() )

		if( vi.m_bHelperIsSeparator )
		{
			if(		vi.m_bRarelyUsedPrev
				&&	vi.m_bRarelyUsedNext
				&&	(!_FindCustomizeMode())
				)
			{
				vi.m_bRarelyUsed = true;
				ASSERT( vis_iter != 0 );
				v[iter_prev].m_bRarelyUsedNext = true;
				ASSERT( iter_next != 0 );
				v[iter_next].m_bRarelyUsedPrev = true;
			}
		}
	} // setup prev/next rarely used flags
}

void CExtPopupMenuWnd::stat_paint_combined_menu_toolbtn(
	LPVOID pCookie,
	CDC & dc,
	const CWnd & refWndMenu,
	const CRect & rcExcludeArea, // in screen coords
	int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
	)
{
	eCombineAlign;
INT nIndex = (INT)pCookie;
	ASSERT( refWndMenu.GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( refWndMenu.GetSafeHwnd() ) );
	ASSERT( dc.GetSafeHdc() != NULL );

	if( rcExcludeArea.IsRectEmpty() )
		return;

	ASSERT_KINDOF( CExtPopupMenuWnd, (&refWndMenu) );
CExtPopupMenuWnd * pPopup =
		(CExtPopupMenuWnd *)(&refWndMenu);
	ASSERT_VALID( pPopup );
	ASSERT( ! pPopup->m_bTopLevel );
CExtPopupMenuWnd * pPopup2 = pPopup->m_pWndParentMenu;
	ASSERT_VALID( pPopup2 );
	ASSERT( 0 <= nIndex && nIndex < pPopup2->ItemGetCount() );
CExtPopupMenuWnd::MENUITEMDATA & mi = pPopup2->ItemGetInfo(nIndex);

CRect rcItem;
	pPopup2->_GetItemRect( nIndex, rcItem );
CRect rcClientExcludeArea( rcExcludeArea );
	pPopup->ScreenToClient( &rcClientExcludeArea );
	rcItem.OffsetRect( - rcItem.TopLeft() );
	rcItem.OffsetRect( rcClientExcludeArea.TopLeft() );
CRect rcTargetClient;
	pPopup->_GetClientRect( &rcTargetClient );
	dc.ExcludeClipRect( &rcTargetClient );

CExtPopupMenuWnd::DRAWITEMDATA _DrawItemData(
		&dc,
		&rcItem,
		&mi,
		pPopup,
		( pPopup->GetSite()->m_dwTrackFlags&TPMX_OWNERDRAW_FIXED ) ? true : false,
		mi.IsPopup(),
		false, // g_bMenuHighlightRarely && vi.m_bRarelyUsed,
		false, // g_bMenuHighlightRarely && vi.m_bRarelyUsedPrev,
		false  // g_bMenuHighlightRarely && vi.m_bRarelyUsedNext
		);

bool bPainted =
		(_DrawItemData.m_bMenuOwnerDrawFixed)
			? _DrawItemData.DoOwnerDrawPainting()
			: false
			;
	if( !bPainted )
		_DrawItemData.PaintDefault();

	dc.SelectClipRgn( NULL );
}

void CExtPopupMenuWnd::DRAWITEMDATA::PaintDefault(
	bool bForceNoIcon, // = false
	bool bForceNoText, // = false
	bool bForceNoCheck, // = false
	bool bForceEnabled, // = false
	bool bForceUnselected // = false
	)
{
	ASSERT( m_pDC != NULL );
	ASSERT( m_pDC->GetSafeHdc() != NULL );
	ASSERT( m_pRectItem != NULL );
	ASSERT( m_pItemData != NULL );
	ASSERT( m_pPopup != NULL );

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite = NULL;
bool bCustomizeMode = false;
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
bool bResizingNode = false;
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	if( m_pPopup->_FindCustomizeMode() )
	{
		pSite = m_pPopup->_FindCustomizeSite();
		ASSERT( pSite != NULL );
		bCustomizeMode = true;
		bForceNoCheck = bForceEnabled = true;
//		bForceUnselected = m_pItemData->IsPopup() ? false : true;
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
		CExtCustomizeCmdTreeNode * pNode = ((MENUITEMDATA *)m_pItemData)->GetCmdNode();
		if( pNode != NULL )
		{
			ASSERT_VALID( pNode );
			if( pNode == pSite->GetResizingNode() )
				bResizingNode = true;
		}
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	if( m_pItemData->IsToolButton() )
	{
		bool bSelDraw =
			bForceUnselected ? false : m_pItemData->IsSelected();
		bool bTransparent = !m_pItemData->IsSelected();
		if( bSelDraw && m_pItemData->IsPopup() )
		{
			const CExtPopupMenuWnd * pChildPopup = m_pItemData->GetPopup();
			if(		pChildPopup->GetSafeHwnd() != NULL
				&&	::IsWindow( pChildPopup->GetSafeHwnd() )
				&&	(pChildPopup->GetStyle() & WS_VISIBLE) != NULL
				)
			{
				bSelDraw = false;
				bTransparent = true;
			}
		} // if( bSelDraw && m_pItemData->IsPopup() )
		CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
			m_pPopup,
			true,
			*m_pRectItem,
			bForceNoText ? _T("") : m_pItemData->GetText(),
			( bForceNoIcon ) ? NULL : ((MENUITEMDATA *)m_pItemData)->GetIconPtr(),
			true,
			bSelDraw, // hover
			bSelDraw, // pushed
			false,
			bForceEnabled || m_pItemData->IsEnabled() || m_pItemData->IsForceEnabled(),
			m_pItemData->IsSelected(), // border
			false,
			false,
			CExtPaintManager::__ALIGN_VERT,
			NULL,
			m_pItemData->IsPopup() ? true : false,
			0,
			bTransparent // transparent
			);
		g_PaintManager->PaintPushButton( *m_pDC, _ppbd );
	} // if( m_pItemData->IsToolButton() )
	else
	{
		CExtCmdIcon * pIcon =
			( bForceNoIcon )
				? NULL
				: ((MENUITEMDATA *)m_pItemData)->GetIconPtr();
		if( pIcon != NULL && pIcon->IsEmpty() )
			pIcon = NULL;
		CExtCmdIcon _iconLarge;
		if( g_bMenuLargeIcons && pIcon != NULL )
		{
			CSize _sizeIcon = pIcon->GetSize();
			_sizeIcon.cx *= 2;
			_sizeIcon.cy *= 2;
			if(	_iconLarge.CreateScaledCopy(
					*pIcon,
					_sizeIcon
					)
				)
				pIcon = &_iconLarge;
#ifdef _DEBUG
			else
			{
				ASSERT( FALSE );
			}
#endif // _DEBUG
		} // if( g_bMenuLargeIcons && pIcon != NULL )
		CExtPaintManager::PAINTMENUITEMDATA _pmid(
			m_pPopup,
			CRect( *m_pRectItem ),
			m_pItemData->GetIconAreaWidth(),
			bForceNoText ? _T("") : m_pItemData->GetText(),
			bForceNoText ? _T("") : m_pItemData->GetAccelText(),
			pIcon,
			m_bPopupItem,
			bForceUnselected ? false : m_pItemData->IsSelected(),
			bForceNoCheck ? false : m_pItemData->GetCheck(),
			bForceNoCheck ? false : m_pItemData->GetRadio(),
			m_pItemData->IsIndeterminated(),
			bForceEnabled || m_pItemData->IsEnabled() || m_pItemData->IsForceEnabled(),
			m_bRarelyUsed,
			m_bRarelyUsedPrevious,
			m_bRarelyUsedNext,
			m_pItemData->GetCmdID()
			);
		_pmid.m_bForceNoLeftGradient = m_pItemData->IsForceNoLeftGradient();
		if( ((MENUITEMDATA *)m_pItemData)->IsInplaceEdit() )
		{
			_pmid.m_rcInPlaceWnd = m_pItemData->AdjustInplaceEditRect( *m_pRectItem );
			_pmid.m_strInPlaceText = m_pItemData->GetInplaceEditText();
		} // if( ((MENUITEMDATA *)m_pItemData)->IsInplaceEdit() )
		if( m_pItemData->IsExtraMark() )
		{
			_pmid.m_bXtraMark = true;
			_pmid.m_bXtraChecked = m_pItemData->IsExtraChecked();
		} // if( m_pItemData->IsExtraMark() )
		ASSERT( !_pmid.m_bHelperPopupDropped );
		if(		m_pItemData->IsPopup()
			&&	m_pItemData->GetPopup()->GetSafeHwnd() != NULL
			&&	m_pItemData->GetPopup()->IsWindowVisible()
			)
			_pmid.m_bHelperPopupDropped = true;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
		if( bResizingNode )
			_pmid.m_bSelected = true;
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		g_PaintManager->PaintMenuItem( *m_pDC, _pmid );
	} // else from if( m_pItemData->IsToolButton() )
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( m_pPopup->m_nHelperDropIndexBefore >= 0 )
	{
		int nCount = m_pPopup->ItemGetCount();
		ASSERT( m_pPopup->m_nHelperDropIndexBefore <= nCount );
		int nAlignAt = 0;
		int nItemIndex = m_pItemData->GetIndex();
		if( nItemIndex == m_pPopup->m_nHelperDropIndexBefore )
			nAlignAt = -1;
		else if(	nItemIndex == ( nCount - 1 )
				&&	m_pPopup->m_nHelperDropIndexBefore == nCount
			)
			nAlignAt = 1;
		else if(	( nItemIndex + 1 ) == m_pPopup->m_nHelperDropIndexBefore
				&&	m_pPopup->_GetItemRef( nItemIndex + 1 ).IsSeparator()
				)
		{
			nItemIndex;
			nAlignAt = 1;
		}
		if( nAlignAt != 0 )
		{ // if paint drop divider
			CSize _sizeDDM = g_PaintManager->GetDropDividerMerics();
			CRect rcItem;
			ASSERT( !m_pPopup->_GetItemRef(nItemIndex).IsSeparator() );
			m_pPopup->_GetItemRect( nItemIndex, rcItem );
			int nY =
				( nAlignAt > 0 )
					? rcItem.bottom - _sizeDDM.cy/2
					: rcItem.top + _sizeDDM.cy/2
					;
			rcItem.top = rcItem.bottom = nY;
			g_PaintManager->PaintDropDivider(
				*m_pDC,
				rcItem,
				true,
				false
				);
		} // if( nAlignAt != 0 )
	} // if( m_pPopup->m_nHelperDropIndexBefore >= 0 )
	if( bCustomizeMode )
	{
		ASSERT( pSite != NULL );
		if(
				(
					pSite != NULL
				&&
							(	pSite->CustomizedNodeGet() != NULL
							&&	pSite->CustomizedNodeGet()  == ((MENUITEMDATA*)m_pItemData)->GetCmdNode()
							)
				)
			||
				(	m_pPopup->m_pDragSrcNode != NULL
				&&	((MENUITEMDATA*)m_pItemData)->GetCmdNode() == m_pPopup->m_pDragSrcNode
				)
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
			||	bResizingNode
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
			)
		{
			CRect rcItem = m_pRectItem;
			g_PaintManager->PaintDragSourceRect(
				*m_pDC,
				rcItem
				);
		}
	} // if( bCustomizeMode )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
}

bool CExtPopupMenuWnd::DRAWITEMDATA::DoOwnerDrawPainting()
{
	ASSERT( m_pDC != NULL );
	ASSERT( m_pDC->GetSafeHdc() != NULL );
	ASSERT( m_pRectItem != NULL );
	ASSERT( m_pItemData != NULL );
	ASSERT( m_pPopup != NULL );
	HWND hWndNotify =
		m_pPopup->m_hWndCmdReciever;
	ASSERT( hWndNotify != NULL );
	ASSERT( ::IsWindow(hWndNotify) );
	LRESULT lResult =
		::SendMessage(
			hWndNotify,
			CExtPopupMenuWnd::g_nMsgPopupDrawItem,
			0,
			LPARAM( *this )
			);
	return lResult ? true : false;
}

bool CExtPopupMenuWnd::DRAWLEFTAREADATA::DoOwnerDrawPainting()
{
	ASSERT( m_pDC != NULL );
	ASSERT( m_pDC->GetSafeHdc() != NULL );
	ASSERT( m_pRectLeftArea != NULL );
	ASSERT( m_pPopup != NULL );
	HWND hWndNotify =
		m_pPopup->m_hWndCmdReciever;
	ASSERT( hWndNotify != NULL );
	ASSERT( ::IsWindow(hWndNotify) );
	LRESULT lResult =
		::SendMessage(
			hWndNotify,
			CExtPopupMenuWnd::g_nMsgPopupDrawLeftArea,
			0,
			LPARAM( *this )
			);
	return lResult ? true : false;
}

void CExtPopupMenuWnd::_DoPaint(
	CDC & dcPaint,
	bool bUseBackBuffer // = true
	)
{
	ASSERT_VALID( this );

	ASSERT_VALID( (&dcPaint) );
	ASSERT( dcPaint.GetSafeHdc() != NULL );

	if( m_hWnd != NULL )
	{
		ASSERT( ::IsWindow(m_hWnd) );
		CExtPaintManager::stat_ExcludeChildAreas(
			dcPaint.GetSafeHdc(),
			m_hWnd
			);
	} // if( m_hWnd != NULL )

	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );

CRect rcRealClient;
	GetClientRect( &rcRealClient );
CRect rcClient;
	_GetClientRect( &rcClient );

CPalette * pOldPalette = NULL, * pOldPalette2 = NULL;
    if( dcPaint.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
    {
        pOldPalette =
			dcPaint.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE );
        dcPaint.RealizePalette();
    }

//bUseBackBuffer = false;
//bUseBackBuffer = true;
	//dcPaint.SetViewportOrg( CPoint(0,0) );
CExtMemoryDC mdc;
	if( bUseBackBuffer )
	{
		mdc.__InitMemoryDC(
			&dcPaint,
			&rcClient // &rcRealClient
			);
		//mdc.SetViewportOrg( CPoint(0,0) );
		if( pOldPalette != NULL )
		{
			pOldPalette2 =
				mdc.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE );
			mdc.RealizePalette();
		}
	}
CDC & dcDummyRef = mdc;
CDC & dc = bUseBackBuffer ? dcDummyRef : dcPaint;

CFont * pOldFont = (CFont *)
		dc.SelectObject(
			&g_PaintManager->m_FontNormal
			);

	dc.FillSolidRect(
		&rcClient,
		g_PaintManager->GetMenuFrameFillColor()
		);

	g_PaintManager->PaintMenuBorder(
		dc,
		rcClient,
		this
		);

CRect rcExpand;
	if( m_bExpandAvailable )
	{
		_GetItemRect(
			IDX_EXPAND,
			rcExpand
			);
		CRect rcExcludePart( rcExpand );
		rcExcludePart.InflateRect( 0, 1 );
		rcExcludePart.left = rcRealClient.left;
		rcExcludePart.right = rcRealClient.right;
		dc.ExcludeClipRect( rcExcludePart );
	}
CRect rcScrollTop,rcScrollBottom;
int nMaxScrollPos = 0;
	if( m_bScrollingAvailable )
	{
		if( m_nDyScrollOffset != 0 )
		{
			_GetItemRect(
				IDX_SCROLL_TOP,
				rcScrollTop
				);
			CRect rcExcludePart( rcScrollTop );
			rcExcludePart.InflateRect( 0, 1 );
			rcExcludePart.left = rcRealClient.left;
			rcExcludePart.right = rcRealClient.right;
			dc.ExcludeClipRect( rcExcludePart );
		}
		nMaxScrollPos = _GetMaxScrollPos();
		if( m_nDyScrollOffset != nMaxScrollPos )
		{
			_GetItemRect(
				IDX_SCROLL_BOTTOM,
				rcScrollBottom
				);
			CRect rcExcludePart( rcScrollBottom );
			rcExcludePart.InflateRect( 0, 1 );
			rcExcludePart.left = rcRealClient.left;
			rcExcludePart.right = rcRealClient.right;
			dc.ExcludeClipRect( rcExcludePart );
		}
	}
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nMenuShadowSize =
		_GetPopupShadowSize();

//	if( rcClient.left >= rcRealClient.left )
//	{
//		CRect rcExcludePart(rcRealClient);
//		rcExcludePart.right =
//			rcClient.left + nMenuBorderSize;
//		dc.ExcludeClipRect( rcExcludePart );
//	}
//	if( rcClient.right <= rcRealClient.right )
//	{
//		CRect rcExcludePart(rcRealClient);
//		rcExcludePart.left =
//			rcClient.right - nMenuBorderSize;
//		dc.ExcludeClipRect( rcExcludePart );
//	}

	if( rcClient.top >= rcRealClient.top )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.bottom =
			rcClient.top + nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}
	if( rcClient.bottom <= rcRealClient.bottom )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.top =
			rcClient.bottom - nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}

visible_items_t v;
	_GetVisibleItems( dc.GetSafeHdc(), v );
INT vis_iter = 0;
bool bPrevToolButton = false;
	for( ; vis_iter < v.GetSize(); ++vis_iter )
	{ // paint visible items
		VisibleItemDefinition_t & vi = v[ vis_iter ];
		ASSERT(
			vi.m_nIndex >= 0
			&&
			vi.m_nIndex < m_items_all.GetSize()
			);
		ASSERT( !vi.m_rcItem.IsRectEmpty() );
		ASSERT( dc.RectVisible(&vi.m_rcItem) );
		MENUITEMDATA & mi =
			m_items_all[vi.m_nIndex];
		ASSERT( mi.IsDisplayed() );
		ASSERT( mi.GetCmdID() == vi.m_nHelperCmdID );
		if( vi.m_bHelperIsSeparator )
		{
			ASSERT( mi.IsSeparator() );
			if( bPrevToolButton )
			{
				CRect rcSep( vi.m_rcItem );
				rcSep.top += rcSep.Height()/2 - 1;
				rcSep.bottom = rcSep.top + 2;
				g_PaintManager->PaintSeparator(
					dc,
					rcSep,
					false,
					false,
					this
					);
			} // if( bPrevToolButton )
			else
				g_PaintManager->PaintMenuSeparator(
					dc,
					vi.m_rcItem,
					mi.GetIconAreaWidth(),
					g_bMenuHighlightRarely && vi.m_bRarelyUsed,
					this
					);
			bPrevToolButton = false;
			continue;
		}
		ASSERT( !mi.IsSeparator() );
		ASSERT( mi.IsPopup() == vi.m_bHelperIsPopup );
		
		DRAWITEMDATA _DrawItemData(
			&dc,
			&vi.m_rcItem,
			&mi,
			this,
			(		(! _FindCustomizeMode() )
				&&	( GetSite()->m_dwTrackFlags&TPMX_OWNERDRAW_FIXED ) != 0 // fix 2.23 ( == replaced with != )
			) ? true : false,
			vi.m_bHelperIsPopup,
			g_bMenuHighlightRarely && vi.m_bRarelyUsed,
			g_bMenuHighlightRarely && vi.m_bRarelyUsedPrev,
			g_bMenuHighlightRarely && vi.m_bRarelyUsedNext
			);
		
		bool bPainted =
			(_DrawItemData.m_bMenuOwnerDrawFixed)
				? _DrawItemData.DoOwnerDrawPainting()
				: false
				;
		if( !bPainted )
			_DrawItemData.PaintDefault();

		bPrevToolButton = mi.IsToolButton();
	} // paint visible items

CPoint point( 0, 0 );
	::GetCursorPos( &point );
	ScreenToClient( &point );
	dc.SelectClipRgn( NULL );

	if( m_bExpandAvailable )
	{
		bool bHover =
			(rcExpand.PtInRect(point))? true : false;
		bool bPushed = bHover;
		g_PaintManager->PaintMenuExpandButton(
			dc,
			rcExpand,
			bHover,
			bPushed,
			this
			);
	} // if( m_bExpandAvailable )

	if( m_bScrollingAvailable )
	{
		if( m_nDyScrollOffset != 0 )
		{
			bool bHover =
				(rcScrollTop.PtInRect(point))? true : false;
			bool bPushed =
				( m_nScrollingDirection > 0 ) ? true : false;
			bPushed = bPushed || bHover;
			CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
				this,
				true,
				rcScrollTop,
				_T("t"), // up arrow
				NULL,
				true,false/*bHover*/,bPushed,false,
				true,true,false,false,
				CExtPaintManager::__ALIGN_HORIZ_CENTER
					| CExtPaintManager::__ALIGN_VERT,
				(HFONT)g_PaintManager->m_FontMarlett.GetSafeHandle()
				);
			g_PaintManager->PaintPushButton( dc, _ppbd );
		}
		if( m_nDyScrollOffset != nMaxScrollPos )
		{
			bool bHover =
				(rcScrollBottom.PtInRect(point))? true : false;
			bool bPushed =
				( m_nScrollingDirection < 0 ) ? true : false;
			bPushed = bPushed || bHover;
			CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
				this,
				true,
				rcScrollBottom,
				_T("u"), // down arrow
				NULL,
				true,false/*bHover*/,bPushed,false,
				true,true,false,false,
				CExtPaintManager::__ALIGN_HORIZ_CENTER
					| CExtPaintManager::__ALIGN_VERT,
				(HFONT)g_PaintManager->m_FontMarlett.GetSafeHandle()
				);
			g_PaintManager->PaintPushButton( dc, _ppbd );
		}
	} // if( m_bScrollingAvailable )

	if( m_nLeftAreaWidth > 0 )
	{
		CRect rcLeftArea( rcClient );
		rcLeftArea.right = rcLeftArea.left + m_nLeftAreaWidth;
		rcLeftArea.OffsetRect( nMenuBorderSize, 0 );
		rcLeftArea.DeflateRect( 0, nMenuBorderSize );
		DRAWLEFTAREADATA _DrawLeftAreaData( &dc, &rcLeftArea, this );
		_DrawLeftAreaData.DoOwnerDrawPainting();
	}

	if( m_bCombineWithEA )
	{
		ASSERT( m_eCombineAlign != __CMBA_NONE );
		dc.SelectClipRgn(NULL);
		CRect rcExcludeClient( m_rcExcludeArea );
		ScreenToClient( &rcExcludeClient );
		g_PaintManager->PaintMenuCombinedArea(
			dc,
			rcExcludeClient,
			rcClient,
			m_eCombineAlign,
			this
			);
		if( m_pCbPaintCombinedContent != NULL )
			m_pCbPaintCombinedContent(
				m_pCbPaintCombinedCookie,
				dc,
				*this,
				m_rcExcludeArea,
				m_eCombineAlign
				);
	} // if( m_bCombineWithEA )

	dc.SelectObject( pOldFont );
	if( bUseBackBuffer )
	{
		ASSERT( mdc.GetSafeHdc() != NULL );
		if( pOldPalette2 != NULL )
			mdc.SelectPalette( pOldPalette2, FALSE );
		mdc.__Flush();
	}

	if( _IsPopupWithShadows()
		&& m_bAnimFinished
		&& nMenuShadowSize > 0
		)
	{ // if we need to paint shadow for client area (and combined exclude area)
		dcPaint.SelectClipRgn(NULL);
		CRect
			rcExcludeClient( 0,0,0,0 ),
			rcExcludeScreen( 0,0,0,0 ),
			rcBaseScreen( 0,0,0,0 );
		if( m_bCombineWithEA )
		{
			if( _IsPopupWithShadows()
				&& m_bAnimFinished
				&& nMenuShadowSize > 0
				&& m_bTopLevel
				)
			{ // if we need to paint shadow for combined exclude area
				rcExcludeScreen = m_rcExcludeArea;
				rcExcludeClient = rcExcludeScreen;
				ScreenToClient( &rcExcludeClient );
				dcPaint.ExcludeClipRect( &rcClient );
				rcBaseScreen = rcClient;
				ClientToScreen( &rcBaseScreen );
				VERIFY(
					m_ShadowCMBA.Paint(
						dcPaint,
						rcExcludeClient,
						rcExcludeScreen,
						rcBaseScreen,
						nMenuShadowSize
						)
					);
				dcPaint.SelectClipRgn(NULL);
			} // if we need to paint shadow for combined exclude area

			CRect rcExcludePart( m_rcExcludeArea );
			ScreenToClient( &rcExcludePart );
			dcPaint.ExcludeClipRect( &rcExcludePart );
		} // if( m_bCombineWithEA )
		VERIFY(
			m_ShadowMain.Paint(
				dcPaint,
				rcClient,
				rcBaseScreen,
				rcExcludeScreen,
				nMenuShadowSize
				)
			);

	} // if we need to paint shadow for client area (and combined exclude area)

	if( pOldPalette != NULL )
		dcPaint.SelectPalette( pOldPalette, FALSE );
}

#if _MFC_VER < 0x700
void CExtPopupMenuWnd::OnActivateApp(BOOL bActive, HTASK hTask) 
#else
void CExtPopupMenuWnd::OnActivateApp(BOOL bActive, DWORD hTask) 
#endif
{
	ASSERT_VALID( this );
	bActive;
	hTask;
	_OnCancelMode();
}

bool CExtPopupMenuWnd::_StartScrolling(int nButtonIndex)
{
	ASSERT_VALID( this );

	if( GetSafeHwnd() == NULL )
		return FALSE;

	if( (nButtonIndex == IDX_SCROLL_TOP
			|| nButtonIndex == IDX_SCROLL_BOTTOM)
//		&& m_nCurIndex != nButtonIndex
		)
	{
		ASSERT( m_bScrollingAvailable );
		_ItemFocusDelay();
		_ItemFocusCancel( FALSE );
		m_nCurIndex = nButtonIndex;
		Invalidate( FALSE );
		SetTimer(
			ID_TIMER_SCROLLING,
			ID_PERIOD_SCROLLING,
			NULL
			);
		if( !_FindCustomizeMode() )
			_SetCapture();
		return true;
	}
	return false;
}

bool CExtPopupMenuWnd::_PtInWndArea( CPoint ptClient )
{
	if( GetSafeHwnd() == NULL )
		return false;

	ClientToScreen( &ptClient );
HWND hWndFromPoint = ::WindowFromPoint(ptClient);
	if( hWndFromPoint == NULL )
		return false;
	if( hWndFromPoint == m_hWnd )
		return true;
	return false;
}

bool CExtPopupMenuWnd::_OnMouseClick(
	UINT nFlags,
	CPoint point,
	bool & bNoEat
	)
{
	ASSERT_VALID( this );

	if( GetSafeHwnd() == NULL )
		return false;

	if( !m_bAnimFinished )
		return true;

CExtPopupMenuSite & _site = GetSite();
	if(	_site.IsShutdownMode()
		|| _site.IsEmpty()
		|| _site.GetAnimated() != NULL
		)
		return true;

CPoint ptScreenClick( point );
	ClientToScreen( &ptScreenClick );
HWND hWndFromPoint = ::WindowFromPoint( ptScreenClick );
	if(		hWndFromPoint != NULL
		&&	(::GetWindowLong(hWndFromPoint,GWL_STYLE)&WS_CHILD) != 0
		&&	::GetParent(hWndFromPoint) == m_hWnd
		)
	{
		CWnd * pWnd = CWnd::FromHandlePermanent( hWndFromPoint );
		if( pWnd != NULL )
		{
			int nCount = ItemGetCount();
			for( int i = 0; i < nCount; i++ )
			{
				MENUITEMDATA & mi = _GetItemRef( i );
				if( !mi.IsInplaceEdit() )
					continue;
				if( ((LPVOID)mi.GetInplaceEditPtr()) != ((LPVOID)pWnd) )
					continue;
				if( m_nCurIndex == i )
					break;
				_ItemFocusSet( i, FALSE, TRUE, TRUE );
				break;
			} // for( int i = 0; i < nCount; i++ )
		} // if( pWnd != NULL )
		bNoEat = true;
		return false;
	}

bool bLButtonUpCall =
		(nFlags==WM_LBUTTONUP || nFlags==WM_NCLBUTTONUP)
			? true : false;
/*
CRect rectWindow;
	GetWindowRect( &rectWindow );
	ScreenToClient( &rectWindow );
	if( !rectWindow.PtInRect(point) )
*/
	if( bLButtonUpCall && (!_PtInWndArea(point)) )
	{

		if( m_wndToolTip.GetSafeHwnd() != NULL )
		{
			CPoint ptScreen( point );
			ClientToScreen( &ptScreen );
			if( m_wndToolTip.GetSafeHwnd() == ::WindowFromPoint(ptScreen) )
				return true; // eat!
		}

		if( m_pWndParentMenu != NULL
			&& m_pWndParentMenu->GetSafeHwnd() != NULL
			)
		{
			ASSERT_VALID( m_pWndParentMenu );
			ClientToScreen( &point );
			m_pWndParentMenu->ScreenToClient( &point );

			HWND hWndOwn = GetSafeHwnd();
			ASSERT( hWndOwn != NULL );
			ASSERT( ::IsWindow(hWndOwn) );
			CExtPopupMenuWnd * pWndParentMenu = m_pWndParentMenu;
			if(	pWndParentMenu->_OnMouseClick(
					nFlags,
					point,
					bNoEat
					)
				)
			{
				if( bNoEat )
					return false;
				if( ::IsWindow(hWndOwn) )
					_OnCancelMode();
				return true;
			}
			return false;
		}

		// fixed in v. 2.20
		if( nFlags == WM_RBUTTONUP
			|| nFlags == WM_LBUTTONUP
			)
		{
			return true;
		}

		_OnCancelMode();
		return false;
	} // if( bLButtonUpCall && (!_PtInWndArea(point)) )

bool bInplaceControlArea = false, bInplaceDropDownArea = false;
int nHitTest =
		_HitTest(
			point,
			&bInplaceControlArea,
			&bInplaceDropDownArea
			);
	if( bLButtonUpCall )
	{
		if( _StartScrolling(nHitTest) )
			return true;
	}

	if( nHitTest < 0 )
	{
		if( bLButtonUpCall )
		{
			if( (nHitTest == IDX_SCROLL_TOP
					|| nHitTest == IDX_SCROLL_BOTTOM)
				//&& m_nCurIndex != nHitTest
				)
			{
				return true;
			}

			if( nHitTest == IDX_EXPAND )
			{
				if(	(!m_bExpandWasPressed)
					&& m_bExpandAvailable
					)
					_DoExpand();
				return true;
			}

			// fixed in v. 2.20
			if(		nFlags != WM_RBUTTONUP
				&&	nFlags != WM_LBUTTONUP
				)
			{
				_OnCancelMode();
				return true;
			}
		} // if( bLButtonUpCall )


		if(		m_eCombineAlign != __CMBA_NONE
			&&	m_bTopLevel
			&&	(!bLButtonUpCall)
			)
		{
			CRect rcExcludeAreaTest( m_rcExcludeArea );
			ScreenToClient( &rcExcludeAreaTest );
			if( rcExcludeAreaTest.PtInRect( point ) )
			{
				_OnCancelMode();
				return true;
			} // if( rcExcludeAreaTest.PtInRect( point ) )
		} // if( m_eCombineAlign != __CMBA_NONE ...

		return false;
	}
	ASSERT( nHitTest < m_items_all.GetSize() );

	if( !bLButtonUpCall )
		return true;

MENUITEMDATA & mi = _GetItemRef(nHitTest);
	if( (!mi.IsEnabled()) || mi.IsSeparator() )
		//return false;
		return true;
	if(		mi.IsInplaceEdit()
		&&	mi.IsPopup()
		&&	(! mi.IsAllowInplaceEditActivation() )
		)
	{
		bInplaceControlArea = false;
		bInplaceDropDownArea = true;
	}

	if(		mi.IsInplaceEdit()
		&&	(!mi.IsPopup())
		&&	(! bInplaceDropDownArea )
		&&	(! _FindHelpMode() )
		)
	{
		_SetCapture();
		_ItemFocusSet( nHitTest, FALSE, TRUE );
		CWnd * pWndInplace = mi.GetInplaceEditPtr();
		if( pWndInplace != NULL )
		{
			ASSERT_VALID( pWndInplace );
			ASSERT( pWndInplace->GetSafeHwnd() != NULL && (::IsWindow(pWndInplace->GetSafeHwnd())) );
			if( mi.IsAllowInplaceEditActivation() )
			{
				if( (pWndInplace->GetStyle() & WS_VISIBLE) == 0 )
				{
					CRect rcInplaceEdit;
					_GetItemRect( nHitTest, rcInplaceEdit );
					rcInplaceEdit = mi.AdjustInplaceEditRect( rcInplaceEdit );
					pWndInplace->SetWindowPos(
						NULL,
						rcInplaceEdit.left, rcInplaceEdit.top,
						rcInplaceEdit.Width(), rcInplaceEdit.Height(),
						SWP_NOZORDER|SWP_NOOWNERZORDER
							|SWP_NOACTIVATE|SWP_SHOWWINDOW
						);
				} // if( (pWndInplace->GetStyle() & WS_VISIBLE) == 0 )
				pWndInplace->SetFocus();
			} // if( mi.IsAllowInplaceEditActivation() )
		} // if( pWndInplace != NULL )
		return true;
	} // if( mi.IsInplaceEdit() ...
	if( mi.IsPopup() )
	{
		if( mi.GetPopup()->GetSafeHwnd() != NULL )
			return false;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		if( mi.IsInplaceEdit() && _FindHelpMode() )
		{
			CExtCustomizeCmdTreeNode * pNode = mi.GetCmdNode();
			if( pNode != NULL )
				_EndSequence(
					pNode->GetCmdID( false ),
					mi.GetCmdReciever()
					);
		} // if( mi.IsInplaceEdit() && _FindHelpMode() )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

		if( mi.IsInplaceEdit() && bInplaceDropDownArea )
			_ItemFocusCancel( FALSE );
		if( m_nDelayedFocusItemIndex < 0 ) // fix 2.23 condition for care about current animation status
			_ItemFocusSet(
				nHitTest,
				TRUE,
				TRUE
				);
		return true;
	} // if( mi.IsPopup() )

	if( mi.IsExtraMark() )
	{
		ASSERT( mi.GetExtraMarkCallback() != NULL );
		mi.GetExtraMarkCallback()( this, &mi );
		CClientDC dc( this );
		_DoPaint( dc );
	} // if( mi.IsExtraMark() )
	else if( mi.IsExecutableCmdID() )
	{
		if(	!(		mi.GetCmdDeliverCb() != NULL
				&&	mi.GetCmdDeliverCb()( this, &mi )
				)
			)
			_EndSequence( mi.GetCmdID(), mi.GetCmdReciever() );
	} // else if( mi.IsExecutableCmdID() )
	else
		_OnCancelMode();
//	return false;
	return true;
}

void CExtPopupMenuWnd::_EndSequence(
	UINT nCmdID, // = 0
	HWND hWndDeliver // = NULL
	)
{
	ASSERT_VALID( this );

#ifdef _DEBUG
	if( hWndDeliver != NULL )
	{
		ASSERT( ::IsWindow(hWndDeliver) );
	}
#endif // _DEBUG
	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );

CExtPopupMenuSite & _site = GetSite();
UINT * lpnResultCmdID = _site.GetTargetCmdIdPtr();
	if( lpnResultCmdID != NULL )

		*lpnResultCmdID = nCmdID;
CExtCmdItem * pCmdItem = NULL;
	if( nCmdID > 0 && ((m_dwTrackFlags&TPMX_NO_WM_COMMAND)==0) )
	{
		pCmdItem =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( m_hWndCmdReciever ),
				nCmdID
				);
		ASSERT( pCmdItem != NULL );
	} // if( nCmdID > 0 && ((m_dwTrackFlags&TPMX_NO_WM_COMMAND)==0) )

CExtPopupMenuWnd * pTopPopup = _site.GetInstance();
	ASSERT_VALID( pTopPopup );
HWND hWndTopPopup = pTopPopup->GetSafeHwnd();
	ASSERT(
		hWndTopPopup != NULL
		&& ::IsWindow( hWndTopPopup )
		);

	if( hWndDeliver == NULL )
		hWndDeliver = m_hWndCmdReciever;

CWnd * pTopLevelParent = GetTopLevelParent();
HWND hWndTopLevelParent = pTopLevelParent->GetSafeHwnd();

CRect rcBkUpdate( 0, 0, 0, 0 );
	// hide all sequence
	CExtPopupMenuWnd * pPopup = this;
	for( ; pPopup != NULL; pPopup = pPopup->m_pWndParentMenu )
	{
		ASSERT_VALID( pPopup );
		ASSERT( pPopup->GetSafeHwnd() != NULL );
		ASSERT( ::IsWindow( pPopup->GetSafeHwnd() ) );
		CRect rcPopup;
		pPopup->GetWindowRect( &rcPopup );
		if( rcBkUpdate.IsRectEmpty() )
			rcBkUpdate = rcPopup;
		else
		{
			CRect rcPrev( rcBkUpdate );
			rcBkUpdate.UnionRect( &rcPrev, &rcPopup );
		}
		if(	pPopup->m_wndToolTip.GetSafeHwnd() != NULL
			&& ::IsWindow( pPopup->m_wndToolTip.GetSafeHwnd() )
			&& pPopup->m_wndToolTip.IsWindowVisible()
			)
		{
			CRect rcToolTip;
			pPopup->m_wndToolTip.GetWindowRect( &rcToolTip );
			CRect rcPrev( rcBkUpdate );
			rcBkUpdate.UnionRect( &rcPrev, &rcToolTip );
		}
		pPopup->ShowWindow( SW_HIDE );
	} // for( ; pPopup != NULL; pPopup = pPopup->m_pWndParentMenu )

	// allow bottom windows repaint
	if(	!rcBkUpdate.IsRectEmpty()
		&&	hWndTopLevelParent != NULL
		&& ::IsWindow(hWndTopLevelParent)
		)
	{
		::ScreenToClient(
			hWndTopLevelParent, (LPPOINT)&rcBkUpdate);
		::ScreenToClient(
			hWndTopLevelParent, ((LPPOINT)&rcBkUpdate)+1
			);

#if (WINVER >= 0x0500)
		ASSERT( WS_EX_LAYOUTRTL == 0x00400000L );
#endif
		if(		(	(DWORD)
					::GetWindowLong(
						hWndTopLevelParent,
						GWL_EXSTYLE
						)
				) & 0x00400000L // WS_EX_LAYOUTRTL under Win 5x
			)
		{
			//CRect::SwapLeftRight( &rcBkUpdate );
			LONG temp = rcBkUpdate.left;
			rcBkUpdate.left = rcBkUpdate.right;
			rcBkUpdate.right = temp;
		}
		
		::RedrawWindow(
			hWndTopLevelParent,
			&rcBkUpdate,
			NULL,
			RDW_INVALIDATE|RDW_ERASE
				|RDW_UPDATENOW|RDW_ERASENOW
				|RDW_VALIDATE
				|RDW_FRAME|RDW_ALLCHILDREN
			);
	}

	// top popup (and all children) finally must be closed
	if( ::IsWindow( hWndTopPopup ) )
		::PostMessage( hWndTopPopup, WM_CLOSE, 0, 0 );

	if( CExtToolControlBar::g_bMenuTracking )
		CExtToolControlBar::_CloseTrackingMenus();
//CExtToolControlBar * pToolBar = CExtToolControlBar::_GetMenuTrackingBar();
//	if( pToolBar != NULL )
//		pToolBar->PostMessage( WM_CANCELMODE );

	if(		pCmdItem != NULL
		&&	::IsWindow(hWndDeliver)
		&&	(! _FindCustomizeMode() )
		)
	{
		// if command target still exists -> deliver command to it
		VERIFY( pCmdItem->Deliver(hWndDeliver) );
		g_SoundPlayer->PlaySound(
			CExtSoundPlayer::__ON_MENU_CMD_CLICKED
			);
	} // if( pCmdItem != NULL && ...
}

void CExtPopupMenuWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	nFlags;
	point;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( (!_FindCustomizeMode()) )
		return;
int nIndex = _HitTest( point );
	if( nIndex >= 0 )
		return;
	if(		nIndex == IDX_SCROLL_TOP
		||	nIndex == IDX_SCROLL_BOTTOM
		)
		_StartScrolling( nIndex );
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
}

void CExtPopupMenuWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	nFlags;
	point;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( (!_FindCustomizeMode()) )
		return;
int nIndex = _HitTest( point );
	if( nIndex < 0 )
		return;
	ASSERT( nIndex < ItemGetCount() );
MENUITEMDATA & mi = _GetItemRef( nIndex );
	if( mi.IsSeparator() )
		return;
CExtCustomizeSite * pSite = _FindCustomizeSite();
	ASSERT( pSite != NULL );
CExtCustomizeCmdTreeNode * pClickedCmdNode = mi.GetCmdNode();
	ASSERT_VALID( pClickedCmdNode );
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	if(		mi.IsSelected()
		&&	(pClickedCmdNode->GetFlags() & __ECTN_TBB_RESIZABLE) != 0
		)
	{ // if button can be resized
		int nDdWidth = g_PaintManager->GetDropDividerMerics().cx / 2;
		if( nDdWidth < 2 )
			nDdWidth = 2;
		CRect rcItem( 0, 0, 0, 0 );
		_GetItemRect( nIndex, rcItem );
		CRect rcH( rcItem );
		rcH.right = rcH.left + nDdWidth;
		int nInitialResizingStateH = 0;
		if( rcH.PtInRect(point) )
			nInitialResizingStateH = -1;
		else
		{
			rcH = rcItem;
			rcH.left = rcH.right - nDdWidth;
			if( rcH.PtInRect(point) )
				nInitialResizingStateH = 1;
		} // else from if( rcH.PtInRect(point) )
		if( nInitialResizingStateH != 0 )
		{
			int nWidthMin = 0, nWidthMax = 0;
			pSite->OnGetCmdItemMinMaxSizeH(
				NULL,
				pClickedCmdNode,
				&nWidthMin,
				&nWidthMax
				);
			CRect rcInplaceEdit = mi.AdjustInplaceEditRect( rcItem );
			rcInplaceEdit.top = rcItem.top;
			rcInplaceEdit.bottom = rcItem.bottom;
			int nDdExtent = 0;
			if( mi.IsPopup() )
			{
				nDdExtent = ::GetSystemMetrics(SM_CXHTHUMB);
				rcInplaceEdit.right += nDdExtent;
			}
			if( !pSite->OnCmdNodeResizeH(
					nInitialResizingStateH,
					m_hWnd,
					NULL,
					pClickedCmdNode,
					nWidthMin,
					nWidthMax,
					rcInplaceEdit // rcItem
					)
				)
				return; // resizing is canceled
//			if( nDdExtent != 0 )
//				pClickedCmdNode->TextFieldWidthSet(
//					pClickedCmdNode->TextFieldWidthGet() - nDdExtent
//					);
			int nSavedIndex = m_nCurIndex;
			m_nCurIndex = -1;
			_BuildItems( NULL, m_bTopLevel );
			VERIFY( UpdateFromCmdTree( m_hWndCmdReciever, m_pNode, m_bTopLevel ) );
			_SyncItems();
			ShowWindow( SW_HIDE	);
			_RecalcLayoutImpl();
			VERIFY(
				SetWindowRgn(
					NULL,
					FALSE
					)
				);
			ASSERT( !IsWindowVisible() );
			_FreeWinObjects();
			// adjust screen position
			CRect rcWnd = _CalcTrackRect();
			if( m_rgnWnd.GetSafeHandle() != NULL )
			{
				ASSERT( m_bExcludeAreaSpec );
				ASSERT( m_bCombineWithEA );
				ASSERT( m_eCombineAlign != __CMBA_NONE );
				CRgn rgnTmp;
				VERIFY( rgnTmp.CreateRectRgn(0,0,0,0) );
				rgnTmp.CopyRgn( &m_rgnWnd );
				ASSERT( rgnTmp.GetSafeHandle() != NULL );
				VERIFY(
					SetWindowRgn(
						(HRGN)rgnTmp.Detach(),
						FALSE
						)
					);
			} // if( m_rgnWnd.GetSafeHandle() != NULL )
			SetWindowPos(
				NULL,
				rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),
				SWP_NOACTIVATE
					|SWP_NOZORDER //|SWP_NOOWNERZORDER
				);
			if( m_bScrollingAvailable )
				_RecalcLayoutImpl();
			ShowWindow( SW_SHOWNA );
			UpdateWindow();
			_ItemFocusSet( nSavedIndex, FALSE, FALSE, FALSE );
			ASSERT( m_pDragSrcNode == NULL );
			m_pDragSrcNode = pClickedCmdNode;
			CClientDC dc( this );
			_DoPaint( dc );
			m_pDragSrcNode = NULL;
			return;
		} // if( nInitialResizingStateH != 0 )
	} // if button can be resized
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	ASSERT( m_pDragSrcNode == NULL );
	ASSERT( m_nDragSrcIdx < 0 );
	m_pDragSrcNode = pClickedCmdNode;
	m_nDragSrcIdx = nIndex;
	m_bHelperNoRemoveSrc = false;
CRect rcItem( 0, 0, 1, 1 );
	_DoResetChainPositions();
	m_ptHelperDragStart = point;
	m_bHelperNoHideChild = true;
	m_bHelperDragOverPassed = false;
	_ItemFocusSet( nIndex, TRUE, TRUE, FALSE );
HWND hWndOwn = m_hWnd;
ASSERT( hWndOwn != NULL && ::IsWindow(hWndOwn) );
	pSite->DoDragCmdNode(
		this,
		NULL,
		m_pDragSrcNode,
		rcItem
		);
	if( ! ::IsWindow(hWndOwn) )
		return;
	m_pDragSrcNode = NULL;
	m_bHelperNoHideChild = false;
	m_nDragSrcIdx = -1;
	m_bHelperNoRemoveSrc = false;
	m_bHelperDragOverPassed = false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
}

void CExtPopupMenuWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	nFlags;
	point;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( (!_FindCustomizeMode()) )
		return;
int nIndex = _HitTest( point );
	if( nIndex < 0 )
		return;
	ASSERT( nIndex < ItemGetCount() );
MENUITEMDATA & mi = _GetItemRef( nIndex );
	if( mi.IsSeparator() )
		return;
	_ItemFocusSet( mi.GetIndex(), TRUE, TRUE, FALSE );
CExtCustomizeCmdTreeNode * pNode = mi.GetCmdNode();
	ASSERT_VALID( pNode );
CExtCustomizeSite * pSite = _FindCustomizeSite();
	ASSERT( pSite != NULL );
CRect rcItem( 0, 0, 1, 1 );
	_DoResetChainPositions();

CExtCmdItem * pCmdItem =
	g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd(m_hWnd),
		pNode->GetCmdID( false )
		);
	ASSERT( pCmdItem != NULL );
	if( !pSite->CustomizedNodeSet( pNode ) )
		return;
	{ // BLOCK
		CClientDC dc( this );
		_DoPaint( dc );
	} // BLOCK
bool bCanAddRemoveSeparator = (nIndex > 0) ? true : false;
bool bDelete = false;
HWND hWndOwn = m_hWnd;
	ASSERT( hWndOwn != NULL && ::IsWindow(hWndOwn) );
	if( !pSite->OnCustomizeTreeNode(
			NULL,
			pNode,
			pCmdItem,
			bDelete,
			bCanAddRemoveSeparator,
			this
			)
		)
	{
		if( pSite->CustomizedNodeGet() == pNode )
		{
			VERIFY( pSite->CustomizedNodeSet( NULL ) );
		}
		if(		hWndOwn == NULL
			||	(! ::IsWindow(hWndOwn) )
			)
			return;
		CClientDC dc( this );
		_DoPaint( dc );
		return;
	}
	VERIFY( pSite->CustomizedNodeSet( NULL ) );

	if( !::IsWindow(hWndOwn) )
		return;
	
	m_nCurIndex = -1;
	_BuildItems( NULL, m_bTopLevel );
	if( bDelete )
	{
		ASSERT_VALID( pNode->GetParentNode() );
		int nOwnNodeIndex = pNode->GetOwnIndex();
		if( nOwnNodeIndex < (pNode->GetParentNode()->GetNodeCount()-1) )
			pNode->GetParentNode()->ElementAt(nOwnNodeIndex+1)
				->ModifyFlags( 0, __ECTN_GROUP_START );
		pNode->RemoveSelf( pCmdItem );
	}
	ASSERT_VALID( m_pNode );
	VERIFY( UpdateFromCmdTree( m_hWndCmdReciever, m_pNode, m_bTopLevel ) );
	
	_SyncItems();
	ShowWindow( SW_HIDE	);
	_RecalcLayoutImpl();
	VERIFY(
		SetWindowRgn(
			NULL,
			FALSE
			)
		);
	ASSERT( !IsWindowVisible() );
	_FreeWinObjects();
// adjust screen position
CRect rcWnd = _CalcTrackRect();
	if( m_rgnWnd.GetSafeHandle() != NULL )
	{
		ASSERT( m_bExcludeAreaSpec );
		ASSERT( m_bCombineWithEA );
		ASSERT( m_eCombineAlign != __CMBA_NONE );
		CRgn rgnTmp;
		VERIFY( rgnTmp.CreateRectRgn(0,0,0,0) );
		rgnTmp.CopyRgn( &m_rgnWnd );
		ASSERT( rgnTmp.GetSafeHandle() != NULL );
		VERIFY(
			SetWindowRgn(
				(HRGN)rgnTmp.Detach(),
				FALSE
				)
			);
	} // if( m_rgnWnd.GetSafeHandle() != NULL )
	SetWindowPos(
		NULL,
		rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),
		SWP_NOACTIVATE
			|SWP_NOZORDER //|SWP_NOOWNERZORDER
		);
	if( m_bScrollingAvailable )
		_RecalcLayoutImpl();
	ShowWindow( SW_SHOWNA );

#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
}

void CExtPopupMenuWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CExtPopupBaseWnd::OnShowWindow(bShow, nStatus);
	ASSERT_VALID( this );
	if(		_FindCustomizeMode()
		&&	bShow
		)
	{
		CClientDC dc( this );
		_DoPaint( dc );
	}
}

void CExtPopupMenuWnd::OnCancelMode() 
{
	ASSERT_VALID( this );

	if( _FindCustomizeMode() )
		return;

	//_OnCancelMode();
CExtPopupMenuSite & _site = GetSite();
	_site.DoneInstance();
	ASSERT( _site.IsEmpty() );
	ASSERT( !_site.IsShutdownMode() );

	if( CExtToolControlBar::g_bMenuTracking )
		CExtToolControlBar::_CloseTrackingMenus();
}

void CExtPopupMenuWnd::_FreeWinObjects()
{
	ASSERT_VALID( this );

	if( m_rgnWnd.GetSafeHandle() != NULL )
	{
		VERIFY(
			m_rgnWnd.DeleteObject()
			);
	}
	m_ShadowMain.Destroy();
	m_ShadowCMBA.Destroy();
	_SurfacesDelete();
	if( !(::IsWindow(GetSafeHwnd())) )
		return;
	KillTimer(ID_TIMER_ANIMATION);
	//KillTimer(ID_TIMER_DELAY_SHOW);
	//KillTimer(ID_TIMER_ITEM_FOCUS_DELAY);
	//KillTimer(ID_TIMER_SCROLLING);
}

void CExtPopupMenuWnd::_OnCancelMode(
	bool bNcDestroy // = false
	) 
{
	ASSERT_VALID( this );

	// update status bar message
	CExtControlBar::stat_SetMessageString(
		GetSafeOwner()
		);

bool bCusomizeMode = _FindCustomizeMode();
	if( !bCusomizeMode )
	{
		_CoolTipHide( false );
		_FreeWinObjects();
		_ReleaseCapture();
	} // if( !bCusomizeMode )

INT iter = 0;
	for( ; iter < m_items_all.GetSize(); iter++ )
	{
		MENUITEMDATA & mi = m_items_all[ iter ];
		mi.SetSelected( false );

		if( mi.IsPopup() )
		{
			if( mi.GetPopup()->GetSafeHwnd() != NULL )
				mi.GetPopup()->_OnCancelMode();
		}
	}
	m_nCurIndex = IDX_NOTHING;

	if( bNcDestroy )
		return;

	if( ! ::IsWindow(GetSafeHwnd()) )
		return;
bool bTopLevel = m_bTopLevel;
	ShowWindow( SW_HIDE );
	if( !bCusomizeMode )
		PostMessage( WM_CLOSE );

	if(		CExtToolControlBar::g_bMenuTracking && bTopLevel
		&&	(!bCusomizeMode)
		)
	{
		CExtToolControlBar::_CloseTrackingMenus();
//		CExtToolControlBar * pToolBar = CExtToolControlBar::_GetMenuTrackingBar();
//		if( pToolBar != NULL )
//			pToolBar->SendMessage( WM_CANCELMODE );
	}
}

void CExtPopupMenuWnd::_ItemFocusSet(
	int nCurIndex,
	BOOL bEnableDropChild,
	BOOL bRepaint,
	BOOL bSelectAnyInChild // = TRUE
	)
{
	ASSERT_VALID( this );

	if( m_nCurIndex != nCurIndex )
	{
		_ItemFocusCancel( FALSE );
		ASSERT( m_nCurIndex < 0 );
		m_nCurIndex = nCurIndex;
		if( m_nCurIndex < 0 )
			return;
	}

bool bResetMessageString = true;
bool bWasSet = false;
MENUITEMDATA & mi = _GetItemRef(m_nCurIndex);
	if( mi.IsEnabled() && mi.IsDisplayed() )
	{
		mi.SetSelected( true );
		bWasSet = true;

		// adjust scrolling
		int nMenuBorderSize =
			g_PaintManager->GetMenuBorderSize();
		if( m_bScrollingAvailable )
		{
			int nMetric;
			int nMaxScrollPos = _GetMaxScrollPos();
			CRect rcItem,rcClient,
				rcScrollBtnUp,rcScrollBtnDown;
			_GetItemRect( IDX_SCROLL_TOP, rcScrollBtnUp );
			_GetItemRect( IDX_SCROLL_BOTTOM, rcScrollBtnDown );
			_GetClientRect( rcClient );
			_GetItemRect( m_nCurIndex, rcItem );

			nMetric = rcClient.bottom;
			if( m_nDyScrollOffset != nMaxScrollPos )
			{
				nMetric -= rcScrollBtnDown.Height();
				if( m_bExpandAvailable )
				{
					CRect rcExpand;
					_GetItemRect( IDX_EXPAND, rcExpand );
					nMetric -= rcExpand.Height();
				}
			}
			if( rcItem.bottom >= nMetric )
			{
				m_nDyScrollOffset -=
					rcItem.bottom - nMetric
					+ nMenuBorderSize*2;
			}

			nMetric = rcClient.top;
			if( m_nDyScrollOffset != 0 )
			{
				nMetric += rcScrollBtnUp.Height();
			}
			if( rcItem.top <= nMetric )
			{
				m_nDyScrollOffset +=
					nMetric - rcItem.top
					+ nMenuBorderSize*2;
			}

			if( m_nDyScrollOffset > 0 )
				m_nDyScrollOffset = 0;
			else
			{
				if( m_nDyScrollOffset < nMaxScrollPos )
					m_nDyScrollOffset = nMaxScrollPos;
			}
		} // if( m_bScrollingAvailable )

	}

	// update status bar message
CWnd * pWndOwner = GetOwner();
	if( pWndOwner != NULL )
	{
		ASSERT_VALID( pWndOwner );
		if( bWasSet && (!mi.IsPopup()) && (!mi.IsSeparator()) )
		{
			UINT nCmdID = mi.GetCmdID();
			if( CExtCmdManager::IsCommand( nCmdID ) )
			{
				UINT nCmdIDToSend = nCmdID;
				bool bAbandonSend = false;
				if( CExtCmdManager::IsSystemCommand( nCmdID ) )
				{
					switch( nCmdID )
					{
					case SC_CLOSE:
						nCmdIDToSend = AFX_IDS_SCCLOSE;
					break;
					case SC_SIZE:
						nCmdIDToSend = AFX_IDS_SCSIZE;
					break;
					case SC_MOVE:
						nCmdIDToSend = AFX_IDS_SCMOVE;
					break;
					case SC_MINIMIZE:
						nCmdIDToSend = AFX_IDS_SCMINIMIZE;
					break;
					case SC_MAXIMIZE:
						nCmdIDToSend = AFX_IDS_SCMAXIMIZE;
					break;
					case SC_RESTORE:
						nCmdIDToSend = AFX_IDS_SCRESTORE;
					break;
					case SC_NEXTWINDOW:
						nCmdIDToSend = AFX_IDS_SCNEXTWINDOW;
					break;
					case SC_PREVWINDOW:
						nCmdIDToSend = AFX_IDS_SCPREVWINDOW;
					break;
					case SC_TASKLIST:
						nCmdIDToSend = AFX_IDS_SCTASKLIST;
					break;
					default:
						bAbandonSend = true;
					break;
					} // switch( nCmdID )
				} // if( CExtCmdManager::IsSystemCommand( nCmdID ) )
				if( !bAbandonSend )
				{
					CExtControlBar::stat_SetMessageString(
						pWndOwner,
						nCmdIDToSend
						);
					bResetMessageString = false;
				} // if( !bAbandonSend )
			} // if( CExtCmdManager::IsCommand( nCmdID ) )
		} // if( ! ( mi.IsPopup() || mi.IsSeparator() ) )
		if( bResetMessageString )
			CExtControlBar::stat_SetMessageString( pWndOwner );
	} // if( bWasSet )
	

	if( bRepaint && ::IsWindowVisible(GetSafeHwnd()) )
	{
//		if( m_rgnWnd.GetSafeHandle() != NULL )
//			RedrawWindow(
//				NULL,
//				&m_rgnWnd,
//				RDW_INVALIDATE|RDW_UPDATENOW|RDW_NOERASE
//					|RDW_INTERNALPAINT
//				);
//		else
//		{
			CClientDC dc( this );
			_DoPaint( dc );
//		}
	}
	
	if( mi.IsPopup() )
	{
		if( mi.IsInplaceEdit() )
		{
			HWND hWndInplaceEdit = mi.GetInplaceEditPtr()->GetSafeHwnd();
			if(		hWndInplaceEdit != NULL
				&&	::IsWindow( hWndInplaceEdit )
				&&	(::GetWindowLong(hWndInplaceEdit,GWL_STYLE)&WS_VISIBLE) != 0
				)
				bEnableDropChild = false;
		} // if( mi.IsInplaceEdit() )
	} // if( mi.IsPopup() )
	else
		bEnableDropChild = false;
	if(		bWasSet
		&&	bEnableDropChild
		&&	mi.GetPopup()->_CanStartLevelTracking()
		)
	{ // if new popup tracking
		if( !_FindCustomizeMode() )
			_ReleaseCapture();
		CRect rectItem;
		_GetItemRect(
			m_nCurIndex,
			rectItem
			);
		rectItem.left -= m_nLeftAreaWidth;
		CPoint point;
		point.x = rectItem.right;
		point.y = rectItem.top,
		ClientToScreen(&point);
		ClientToScreen( &rectItem );
		ASSERT( mi.GetPopup()->m_pWndParentMenu == this );
		bool bDisplayedSuccessfully = false;
		if( mi.GetPopup()->GetSafeHwnd() == NULL )
		{
			bDisplayedSuccessfully = 
				mi.GetPopup()->
					_TrackPopupMenu(
						( mi.IsChildCombine() ? TPMX_COMBINE_DEFAULT : 0 )
						| ( m_dwTrackFlags &
								(TPMX_NO_SHADOWS
								|TPMX_NO_CMD_UI
								|TPMX_NO_WM_COMMAND
								)
							)
						,
					point.x,point.y,
					&rectItem,
					(LPVOID)m_nCurIndex,
					stat_paint_combined_menu_toolbtn
					) ? true : false;
		} // if( mi.GetPopup()->GetSafeHwnd() == NULL )
		else
		{
			bDisplayedSuccessfully = true;
			if( !mi.GetPopup()->IsWindowVisible() )
			{
				mi.GetPopup()->SetWindowPos(
					&CWnd::wndTopMost, 0, 0, 0, 0,
					SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE
						|SWP_NOREDRAW
						|SWP_SHOWWINDOW
					);
				CClientDC dc( mi.GetPopup() );
				mi.GetPopup()->_DoPaint( dc );
			} // if( !mi.GetPopup()->IsWindowVisible() )
		} // else from
		if( bDisplayedSuccessfully && bSelectAnyInChild )
		{ // if track-popup succeeded and we need to select any child item
			int nCountOfPopupItems =
				mi.GetPopup()->m_items_all.GetSize();
			if( nCountOfPopupItems > 0 )
			{
				int nIndex = mi.GetPopup()->_GetNextItem(__NI_ANY);
				if( nIndex >= 0 )
					mi.GetPopup()->_ItemFocusSet(
						nIndex,
						FALSE,
						( GetSite().GetAnimated() == NULL ) ?
							TRUE : FALSE
						);
			}
		} // if track-popup succeeded and we need to select any child item
		if( bDisplayedSuccessfully && mi.IsInplaceEdit() )
		{
			CClientDC dc( this );
			_DoPaint( dc );
		}
	} // if new popup tracking
}

bool CExtPopupMenuWnd::_CoolTipIsVisible()
{
	ASSERT_VALID( this );

	if( _FindCustomizeMode() )
		return false;
	if( !(::IsWindow(GetSafeHwnd())) )
		return false;
	if( CExtPopupMenuSite::g_DefPopupMenuSite.GetCapture() != this )
		return false;
HWND hWndToolTip =
		m_wndToolTip.GetSafeHwnd();
	if(	hWndToolTip != NULL
		&&
		::IsWindow( hWndToolTip )
		&&
		::IsWindowVisible( hWndToolTip )
		)
		return true;
	return false;
}

void CExtPopupMenuWnd::_CoolTipHide(
	bool bAdvOperation // = true
	)
{
	ASSERT_VALID( this );

	if( !(::IsWindow(GetSafeHwnd())) )
		return;

HWND hWndToolTip =
		m_wndToolTip.GetSafeHwnd();
	if(	hWndToolTip == NULL
		||
		(!(::IsWindow(hWndToolTip)))
		)
		return;
	
	if( bAdvOperation && ( GetSite().GetAnimated() == NULL ) )
	{
		m_AnimationType = __AT_CONTENT_DISPLAY;
		_StartAnimation();
	}
	m_wndToolTip.Hide();

}

void CExtPopupMenuWnd::_ItemFocusCancel( BOOL bRepaint )
{
	ASSERT_VALID( this );

	if( !_FindCustomizeMode() )
		_CoolTipHide();

	if( m_nCurIndex < 0 )
	{
		m_nCurIndex = IDX_NOTHING;
	} // if( m_nCurIndex < 0 )
	else
	{
		ASSERT( m_nCurIndex < m_items_all.GetSize() );
		MENUITEMDATA & mi = _GetItemRef(m_nCurIndex);
		if( mi.IsInplaceEdit() )
		{
			mi.SetTempSelectedInplaceEditText( NULL );
			CWnd * pWndInplace = mi.GetInplaceEditPtr();
			if( pWndInplace != NULL )
			{
				ASSERT_VALID( pWndInplace );
				ASSERT( pWndInplace->GetSafeHwnd() != NULL && (::IsWindow(pWndInplace->GetSafeHwnd())) );
				if( (pWndInplace->GetStyle() & WS_VISIBLE) != 0 )
					pWndInplace->ShowWindow( SW_HIDE );
			} // if( pWndInplace != NULL )
		}
		mi.SetSelected( false );
		m_nCurIndex = IDX_NOTHING;

		if( mi.IsPopup() )
			mi.GetPopup()->_OnCancelMode();
	} // else from if( m_nCurIndex < 0 )
	
	// update status bar message
CWnd * pWndOwner = GetOwner();
	if( pWndOwner != NULL )
		CExtControlBar::stat_SetMessageString( pWndOwner );

	if( bRepaint && ::IsWindowVisible(GetSafeHwnd()) )
	{
		CClientDC dc( this );
		_DoPaint( dc );
	}
}

bool CExtPopupMenuWnd::_CanStartLevelTracking()
{
	ASSERT_VALID( this );
	return true;
}

bool CExtPopupMenuWnd::_OnMouseWheel(
	WPARAM wParam,
	LPARAM lParam,
	bool & bNoEat
	)
{
	ASSERT_VALID( this );
	wParam;
	lParam;
	bNoEat;
	return true;
}

bool CExtPopupMenuWnd::_OnMouseMove(
	UINT nFlags,
	CPoint point,
	bool & bNoEat
	)
{
	ASSERT_VALID( this );

	if( GetSafeHwnd() == NULL )
		return false;

	if( !m_bAnimFinished )
		return true;

CExtPopupMenuSite & _site = GetSite();
	if(	_site.IsShutdownMode()
		|| _site.IsEmpty()
		|| _site.GetAnimated() != NULL
		)
		return true;

CPoint ptScreenClick( point );
	ClientToScreen( &ptScreenClick );
HWND hWndFromPoint = ::WindowFromPoint( ptScreenClick );
	if(		hWndFromPoint != NULL
		&&	(::GetWindowLong(hWndFromPoint,GWL_STYLE)&WS_CHILD) != 0
		&&	::GetParent(hWndFromPoint) == m_hWnd
		)
	{
		CWnd * pWnd = CWnd::FromHandlePermanent( hWndFromPoint );
		if( pWnd != NULL )
		{
			int nCount = ItemGetCount();
			for( int i = 0; i < nCount; i++ )
			{
				MENUITEMDATA & mi = _GetItemRef( i );
				if( !mi.IsInplaceEdit() )
					continue;
				if( ((LPVOID)mi.GetInplaceEditPtr()) != ((LPVOID)pWnd) )
					continue;
				if( m_nCurIndex == i )
					break;
				_ItemFocusSet( i, FALSE, TRUE, TRUE );
				break;
			} // for( int i = 0; i < nCount; i++ )
		} // if( pWnd != NULL )
		bNoEat = true;
		return false;
	}
HWND hWndFocus = ::GetFocus();
	if(		hWndFocus != NULL
		&&	(::GetWindowLong(hWndFocus,GWL_STYLE)&WS_CHILD) != 0
		&&	::GetParent(hWndFocus) == m_hWnd
		)
		return true;

/*
CRect rectWindow;
	GetWindowRect( &rectWindow );
	ScreenToClient( &rectWindow );
	if( !rectWindow.PtInRect(point) )
*/
	if( !_PtInWndArea(point) )
	{
//		_ItemFocusCancel( TRUE );
		if( m_pWndParentMenu != NULL
			&& m_pWndParentMenu->GetSafeHwnd() != NULL
			)
		{
			ASSERT_VALID( m_pWndParentMenu );
			ClientToScreen( &point );
			m_pWndParentMenu->ScreenToClient( &point );
			if( m_pWndParentMenu->_OnMouseMove(
					nFlags,
					point,
					bNoEat
					)
				)
			{
				if( bNoEat )
					return false;
				_OnCancelMode();
				return true;
			}
		}
		return false;
	}

int nCurIndex = _HitTest(point);
	if( nCurIndex == IDX_EXPAND )
	{
		_ItemFocusCancel( TRUE );
		if( g_bMenuDelayExpanding )
		{
			_SetCapture();
			if( m_nWaitingExpandTickCount == 0 )
			{
				SetTimer(
					ID_TIMER_DELAY_EXPAND,
					ID_TOTAL_DELAY_EXPAND/ID_FREQ_DELAY_EXPAND,
					NULL
					);
				m_nWaitingExpandTickCount = 1;
			}
		} // if( g_bMenuDelayExpanding )
		return true;
	}
	if( _StartScrolling(nCurIndex) )
		return true;

	if( nCurIndex >= 0 )
	{
#ifdef _DEBUG
		int nCountOfItems = m_items_all.GetSize();
		ASSERT( nCurIndex < nCountOfItems );
#endif // _DEBUG
		MENUITEMDATA & mi = _GetItemRef(nCurIndex);
		if( (!mi.IsEnabled()) || mi.IsSeparator() )
			return false;
		int nOldCurIndex = m_nCurIndex; 
		if( nOldCurIndex != nCurIndex )
		{
			_ItemFocusDelay();
			_SetCapture();
			_ItemFocusSet(
				nCurIndex,
				FALSE,
				TRUE
				);
			MENUITEMDATA & mi = _GetItemRef(nCurIndex);
			if( mi.IsPopup() )
			{
				m_wndToolTip.Hide();
				if( mi.GetPopup()->GetSafeHwnd() == NULL )
					_ItemFocusDelay( nCurIndex );
				return true;
			}
			if(	g_bMenuShowCoolTips
				&& nCurIndex != nOldCurIndex
				&& GetSite().GetCapture() == this
				)
			{
				CExtSafeString sTipText = mi.GetTip();
				if( !sTipText.IsEmpty() )
				{
					CRect rcItem;
					_GetItemRect(nCurIndex,rcItem);
					ClientToScreen( &rcItem );
					m_wndToolTip.SetText( sTipText );
					VERIFY(
						m_wndToolTip.Show(
							this,
							rcItem
							)
						);
				} // if( !sTipText.IsEmpty() )
			} // if( g_bMenuShowCoolTips ....
		} // if( nOldCurIndex != nCurIndex )

		return (nOldCurIndex == nCurIndex) ? false : true;
	} // if( nCurIndex >= 0 )

	return false;
}

void CExtPopupMenuWnd::_ItemFocusDelay(
	int nItemIndex // = IDX_NOTHING
	)
{
	ASSERT_VALID( this );

	if( !(::IsWindow(GetSafeHwnd())) )
		return;
	if( m_nDelayedFocusItemIndex != nItemIndex )
	{
		if( m_nDelayedFocusItemIndex >= 0 )
			KillTimer( ID_TIMER_ITEM_FOCUS_DELAY );
		//m_nDelayedFocusItemIndex = IDX_NOTHING;
	}
	if( nItemIndex < 0 )
	{
		m_nDelayedFocusItemIndex = IDX_NOTHING;
		return;
	}
	if( m_nDelayedFocusItemIndex == nItemIndex )
		return;
	ASSERT( nItemIndex>=0 && nItemIndex<m_items_all.GetSize() );
	m_nDelayedFocusItemIndex = nItemIndex;
	ASSERT( _GetItemRef(m_nDelayedFocusItemIndex).IsPopup() );
	SetTimer(
		ID_TIMER_ITEM_FOCUS_DELAY,
		ID_PERIOD_ITEM_FOCUS_DELAY,
		NULL
		);
}

int CExtPopupMenuWnd::_GetMaxScrollPos()
{
	ASSERT_VALID( this );

	ASSERT( m_bScrollingAvailable );
CRect rcClient;
	_GetClientRect( &rcClient );
int nClientHeight =
		rcClient.Height();
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nMenuShadowSize =
		_GetPopupShadowSize();
int nMaxScrollPos = m_sizeFullItems.cy;
	nMaxScrollPos += nMenuBorderSize*2;
	nMaxScrollPos += nMenuShadowSize;
	nMaxScrollPos -= nClientHeight;
	ASSERT( nMaxScrollPos > 0 );
	if( m_bExpandAvailable )
		nMaxScrollPos += _GetSpecBtnHeight(); // xpand btn
	return -nMaxScrollPos;
}

void CExtPopupMenuWnd::OnTimer(UINT nIDEvent) 
{
	ASSERT_VALID( this );

	switch( nIDEvent )
	{
	case ID_TIMER_DELAY_EXPAND:
	{
		if( m_nWaitingExpandTickCount <
				ID_TOTAL_DELAY_EXPAND
			)
		{
			if( m_nWaitingExpandTickCount <= 1 )
				m_nWaitingExpandTickCount = 0;
			m_nWaitingExpandTickCount +=
				ID_FREQ_DELAY_EXPAND;
			return;
		}
		VERIFY( KillTimer(ID_TIMER_DELAY_EXPAND) );
		m_nWaitingExpandTickCount = 0;
		if( m_bExpandAvailable
			&& (!m_bExpandWasPressed)
			&& ( GetSite().GetAnimated() == NULL )
			)
		{ // if expand can be performed
			CPoint point;
			if( ! ::GetCursorPos(&point) )
				return;
			ScreenToClient( &point );
			CRect rcExpand;
			_GetItemRect( IDX_EXPAND, rcExpand );
			if( rcExpand.PtInRect(point) )
				_DoExpand();
		} // if expand can be performed
		return;
	}
	//case ID_TIMER_DELAY_EXPAND

	case ID_TIMER_SCROLLING:
	{
		_ItemFocusDelay();
		if( m_nCurIndex >= 0 )
			_ItemFocusCancel(TRUE);

		CPoint point;
		if( ! ::GetCursorPos(&point) )
			return;
		ScreenToClient( &point );

		m_nScrollingDirection = 0;

		CRect rcItem;
		_GetItemRect(IDX_SCROLL_TOP,rcItem);
		if( rcItem.PtInRect(point) )
			m_nScrollingDirection = __SCROLLING_PIXEL_STEP;
		else
		{
			_GetItemRect(IDX_SCROLL_BOTTOM,rcItem);
			if( rcItem.PtInRect(point) )
				m_nScrollingDirection = (-__SCROLLING_PIXEL_STEP);
		}

		m_nDyScrollOffset += m_nScrollingDirection;

		bool bKillThisTimer =
			(m_nScrollingDirection == 0) ? true : false;
		if( m_nDyScrollOffset > 0 )
		{
			m_nDyScrollOffset = 0;
			bKillThisTimer = true;
		}
		else
		{
			int nMaxScrollPos = _GetMaxScrollPos();
			if( m_nDyScrollOffset < nMaxScrollPos )
			{
				m_nDyScrollOffset = nMaxScrollPos;
				bKillThisTimer = true;
			}
		}

		if( bKillThisTimer )
		{
			VERIFY( KillTimer(ID_TIMER_SCROLLING) );
			m_nScrollingDirection = 0;
		}
		Invalidate( FALSE );
		return;
	}
	// case ID_TIMER_SCROLLING

	case ID_TIMER_ITEM_FOCUS_DELAY:
	{
		if( GetSite().GetAnimated() != NULL )
			return;
		KillTimer( ID_TIMER_ITEM_FOCUS_DELAY );
		if( GetSite().IsShutdownMode() )
			return;
		if( m_nDelayedFocusItemIndex >= 0 )
		{
			ASSERT( m_nDelayedFocusItemIndex>=0 && m_nDelayedFocusItemIndex<m_items_all.GetSize() );
			ASSERT( _GetItemRef(m_nDelayedFocusItemIndex).IsPopup() );
			CRect rcItem;
			_GetItemRect(m_nDelayedFocusItemIndex,rcItem);
			CPoint point;
			if( ! ::GetCursorPos( &point ) )
				return;
			ScreenToClient( &point );
			if( rcItem.PtInRect(point) )
			{
				_ItemFocusSet(
					m_nDelayedFocusItemIndex,
					TRUE,
					TRUE,
					FALSE
					);
			}
		} // if( m_nDelayedFocusItemIndex >= 0 )
		m_nDelayedFocusItemIndex = IDX_NOTHING;
		return;
	}
	// case ID_TIMER_ITEM_FOCUS_DELAY
	
	default:
		CExtPopupBaseWnd::OnTimer(nIDEvent);
	break; // default
	} // switch( nIDEvent )
}

CExtPopupMenuWnd * CExtPopupMenuWnd::_GetCapture()
{
	return GetSite().GetCapture();
}

void CExtPopupMenuWnd::_SetCapture()
{
	ASSERT_VALID( this );
	ASSERT( !_FindCustomizeMode() );

CExtPopupMenuSite & _site = GetSite();
	if( GetSafeHwnd() != NULL )
	{
		ASSERT( ::IsWindow(GetSafeHwnd()) );
		_site.SetCapture( this );
	}
	else
		_site.SetCapture( NULL );
}

void CExtPopupMenuWnd::_ReleaseCapture()
{
	ASSERT_VALID( this );
	ASSERT( !_FindCustomizeMode() );

CExtPopupMenuSite & _site = GetSite();
	if(		_site.IsEmpty()
		||	_site.IsShutdownMode()
		)
		return;
	if( _site.GetCapture() == this )
		_site.SetCapture( NULL );
}

int CExtPopupMenuWnd::_GetNextItem(
	next_item_t nit
	)
{
	ASSERT_VALID( this );

int nCountOfItems = m_items_all.GetSize();
bool bCanExpand = false;
	if( m_bExpandAvailable
		&& (!m_bExpandWasPressed)
		)
	{
		if( IsAllItemsRarelyUsed() )
			return IDX_EXPAND;
		bCanExpand = true;
	}
	if( nit == __NI_NOTHING ||  nCountOfItems == 0 )
	{
		if( bCanExpand )
			return IDX_EXPAND;
		return IDX_NOTHING;
	}

	if( nit == __NI_ANY && m_nCurIndex >= 0 )
		return m_nCurIndex;

	if( m_nCurIndex < 0 )
	{
		INT iter = 0;
		for( ; iter < m_items_all.GetSize(); ++iter )
		{
			MENUITEMDATA & mi = m_items_all[ iter ];
			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();
		}
		if( bCanExpand )
			return IDX_EXPAND;
		return IDX_NOTHING;
	}

	if( nit == __NI_NEXT )
	{
		if( nCountOfItems < 2 )
		{
			if( bCanExpand )
				return IDX_EXPAND;
			return IDX_NOTHING;
		}

		INT iter = m_nCurIndex + 1;
		for( ; iter < m_items_all.GetSize(); ++iter )
		{
			MENUITEMDATA & mi = m_items_all[ iter ];
			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();
		}
		if( bCanExpand )
			return IDX_EXPAND;
		if( m_nCurIndex == 0 )
			return IDX_NOTHING;
		iter = 0;
		INT iter2 = m_nCurIndex - 1;
		do
		{
			MENUITEMDATA & mi = m_items_all[ iter ];
			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();
			iter++;
		} while( iter != iter2 );
		return IDX_NOTHING;
	}
	if( nit == __NI_PREV )
	{
		if( nCountOfItems < 2 )
			return IDX_NOTHING;

		if( m_nCurIndex != 0 )
		{
			int nIndex = m_nCurIndex - 1;
			INT iter = nIndex;
			for( ; true;  )
			{
				MENUITEMDATA & mi = m_items_all[ iter ];
				if( mi.IsEnabled() && mi.IsDisplayed() )
					return mi.GetIndex();
				if( nIndex == 0 )
					break;
				--iter;
				--nIndex;
			}
		}
		int nIndex = m_nCurIndex + 1;
		INT iter2 = nIndex;
		INT iter = m_items_all.GetSize() - 1;
		do
		{
			MENUITEMDATA & mi = m_items_all[ iter ];
			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();
			iter--;
		} while( iter != iter2 );

		return IDX_NOTHING;
	}

	return IDX_NOTHING;
}

bool CExtPopupMenuWnd::_OnKeyDown(
	UINT nChar,
	UINT nRepCnt,
	UINT nFlags,
	bool & bNoEat
	)
{
	ASSERT_VALID( this );

	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );

	nRepCnt;
	if( GetSafeHwnd() == NULL )
		return true;

	if( !m_bAnimFinished )
		return true;

CExtPopupMenuSite & _site = GetSite();
	if(	_site.IsShutdownMode()
		|| _site.IsEmpty()
		|| _site.GetAnimated() != NULL
		)
		return true;

HWND hWndFocus = ::GetFocus();
	if(		hWndFocus != NULL
		&&	(::GetWindowLong(hWndFocus,GWL_STYLE)&WS_CHILD) != 0
		&&	::GetParent(hWndFocus) == m_hWnd
		)
	{
		bool bAlt =
			( (::GetAsyncKeyState(VK_MENU)&0x8000) != 0 )
				? true : false;
		bool bCtrl =
			( (::GetAsyncKeyState(VK_CONTROL)&0x8000) != 0 )
				? true : false;
		bool bShift =
			( (::GetAsyncKeyState(VK_SHIFT)&0x8000) != 0 )
				? true : false;
		if( bAlt )
			PostMessage( WM_CANCELMODE );
		else if( nChar == VK_ESCAPE )
			SetFocus();
		else if( (nChar == VK_INSERT || nChar == _T('C')) && bCtrl )
				::SendMessage( hWndFocus, WM_COPY, 0, 0 );
		else if(	( nChar == _T('X') && bCtrl )
				||	( nChar == VK_DELETE && bShift )
				)
				::SendMessage( hWndFocus, WM_CUT, 0, 0 );
		else if(	( nChar == _T('V') && bCtrl )
				||	( nChar == VK_INSERT && bShift )
				)
				::SendMessage( hWndFocus, WM_PASTE, 0, 0 );
		else if( nChar == VK_DELETE )
				::SendMessage( hWndFocus, CExtPopupMenuWnd::CInPlaceEditWnd::g_nMsgDeleteSelection, 0, 0 );
		else if( nChar == _T('A') && bCtrl )
				::SendMessage( hWndFocus, CExtPopupMenuWnd::CInPlaceEditWnd::g_nMsgSelectAll, 0, 0 );
		else
			bNoEat = true;
		return (!bNoEat);
	}

next_item_t nit = __NI_NOTHING;
	switch( nChar )
	{
	case VK_MENU:
		_EndSequence();
		return true;
	case VK_SPACE:
		return true;
	case VK_RETURN:
	{
		if( m_nCurIndex < 0 )
			return true; // break;
		MENUITEMDATA & mi = _GetItemRef(m_nCurIndex);
		if( mi.IsInplaceEdit() )
		{
			CWnd * pWndInplace = mi.GetInplaceEditPtr();
			if( pWndInplace != NULL )
			{
				ASSERT_VALID( pWndInplace );
				ASSERT( pWndInplace->GetSafeHwnd() != NULL && (::IsWindow(pWndInplace->GetSafeHwnd())) );
				if( mi.IsAllowInplaceEditActivation() )
				{
					if( (pWndInplace->GetStyle() & WS_VISIBLE) == 0 )
					{
						CRect rcInplaceEdit;
						_GetItemRect( m_nCurIndex, rcInplaceEdit );
						rcInplaceEdit = mi.AdjustInplaceEditRect( rcInplaceEdit );
						pWndInplace->SetWindowPos(
							NULL,
							rcInplaceEdit.left, rcInplaceEdit.top,
							rcInplaceEdit.Width(), rcInplaceEdit.Height(),
							SWP_NOZORDER|SWP_NOOWNERZORDER
								|SWP_NOACTIVATE|SWP_SHOWWINDOW
							);
					} // if( (pWndInplace->GetStyle() & WS_VISIBLE) == 0 )
					pWndInplace->SetFocus();
				} // if( mi.IsAllowInplaceEditActivation() )
			} // if( pWndInplace != NULL )
			return true;
		}
		if( mi.IsExtraMark() )
		{
			ASSERT( mi.GetExtraMarkCallback() != NULL );
			mi.GetExtraMarkCallback()( this, &mi );
			CClientDC dc( this );
			_DoPaint( dc );
			return true;
		} // if( mi.IsExtraMark() )
		if( mi.IsExecutableCmdID() )
		{
			if(	!(		mi.GetCmdDeliverCb() != NULL
					&&	mi.GetCmdDeliverCb()( this, &mi )
					)
				)
				_EndSequence( mi.GetCmdID(), mi.GetCmdReciever() );
			return true;
		}
	} // case VK_RETURN

	case VK_RIGHT:
	{
		bool bSendChangeMenu = false;
		if( m_nCurIndex < 0 )
		{
			if( m_items_all.GetSize() == 0 )
				return true; // false;
//			nit = __NI_ANY;
			bSendChangeMenu = true;
		}
		else
		{
			MENUITEMDATA & mi = _GetItemRef(m_nCurIndex);
			if( !mi.IsPopup() )
			{
				bSendChangeMenu = true;
			}
			else
			{
				if( mi.GetPopup()->GetSafeHwnd() == NULL )
				{
					_ItemFocusDelay();
					_ItemFocusSet(
						m_nCurIndex,
						TRUE,
						TRUE
						);
				}
			}
		}
		if( bSendChangeMenu && nChar==VK_RIGHT )
		{
			ASSERT( m_hWndCmdReciever != NULL );
			ASSERT( ::IsWindow(m_hWndCmdReciever) );
			if( CExtToolControlBar::g_bMenuTracking )
				::PostMessage(
					m_hWndCmdReciever,
					CExtPopupMenuWnd::g_nMsgPopupNext,
					0,
					LPARAM( this )
					);
			return true;
		}
	}
	break; // case VK_RIGHT

	case VK_LEFT:
		if( m_pWndParentMenu == NULL )
		{
			if( CExtToolControlBar::g_bMenuTracking )
				::PostMessage(
					m_hWndCmdReciever,
					CExtPopupMenuWnd::g_nMsgPopupPrev,
					0,
					LPARAM( this )
					);
			return true;
		}
		else
		{
			ASSERT( !m_bTopLevel );
			int nParentCurIndex =
				m_pWndParentMenu->m_nCurIndex;
			ASSERT(
				nParentCurIndex >= 0
				&&
				nParentCurIndex <=
					m_pWndParentMenu->m_items_all.GetSize()
				);
			m_pWndParentMenu->_ItemFocusCancel(
				FALSE
				);
			m_pWndParentMenu->_ItemFocusSet(
				nParentCurIndex,
				FALSE,
				TRUE
				);
			m_pWndParentMenu->_SetCapture();
			return true;
		}

	case VK_ESCAPE:
		_EndSequence();
		return true;
	// case VK_ESCAPE
	
	case VK_UP:
		nit = __NI_PREV;
	break; // case VK_UP
	case VK_DOWN:
		if( IsKeyPressed( VK_CONTROL ) )
		{
			if( m_bExpandAvailable )
				_DoExpand();
			return true;
		}
		nit = __NI_NEXT;
	break; // case VK_DOWN
	case VK_NEXT:
		nit = __NI_PAGE_DOWN;
	break; // case VK_NEXT
	case VK_PRIOR:
		nit = __NI_PAGE_UP;
	break; // case VK_PRIOR

	default:
	//if( _istalnum(nChar) )
	{ // BLOCK: try analyze quick char access
		if( IsKeyPressed(VK_SHIFT)
			|| IsKeyPressed(VK_CONTROL)
			|| IsKeyPressed(VK_MENU)
			|| ( (nFlags & (1<<29)) != 0 ) // ALT pressed?
			)
			return false;
		BYTE lpKeyState[256];
		::GetKeyboardState( lpKeyState );
		UINT wScanCode = ::MapVirtualKey( nChar, 0 );
		HKL hKeyboardLayout =
			::GetKeyboardLayout(
				( ::AfxGetThread() ) -> m_nThreadID
				);
#if (defined _UNICODE)
		TCHAR szChar[2] = { _T('\0'), _T('\0') };
		::ToUnicodeEx(
			nChar,
			wScanCode,
			lpKeyState,
			szChar, 1,
			1,
			hKeyboardLayout
			);
		WORD nMapped = WORD( szChar[0] );
#else
		WORD nMapped = 0;
		::ToAsciiEx(
			nChar,
			wScanCode,
			lpKeyState,
			&nMapped,
			1,
			hKeyboardLayout
			);
		//TCHAR szChar[2] = { (TCHAR)nMapped, _T('\0') };
#endif
		INT nNextIdx =
			ItemFindByAccessChar(
				(TCHAR)nMapped,
				m_nCurIndex // (m_nCurIndex >= 0) ? m_nCurIndex : -1
				);
		if(		nNextIdx < 0
			&&	m_nCurIndex >= 0
			&&	ItemFindByAccessChar( (TCHAR)nMapped, -1 )
					== m_nCurIndex
			)
		{ // if only 1 with this char
			MENUITEMDATA & mi =
				_GetItemRef( m_nCurIndex );
			if( mi.IsInplaceEdit() && (!mi.IsPopup()) )
			{
				CWnd * pWndInplace = mi.GetInplaceEditPtr();
				if( pWndInplace != NULL )
				{
					ASSERT_VALID( pWndInplace );
					ASSERT( pWndInplace->GetSafeHwnd() != NULL && (::IsWindow(pWndInplace->GetSafeHwnd())) );
					if( mi.IsAllowInplaceEditActivation() )
					{
						if( (pWndInplace->GetStyle() & WS_VISIBLE) == 0 )
						{
							CRect rcInplaceEdit;
							_GetItemRect( m_nCurIndex, rcInplaceEdit );
							rcInplaceEdit = mi.AdjustInplaceEditRect( rcInplaceEdit );
							pWndInplace->SetWindowPos(
								NULL,
								rcInplaceEdit.left, rcInplaceEdit.top,
								rcInplaceEdit.Width(), rcInplaceEdit.Height(),
								SWP_NOZORDER|SWP_NOOWNERZORDER
									|SWP_NOACTIVATE|SWP_SHOWWINDOW
								);
						} // if( (pWndInplace->GetStyle() & WS_VISIBLE) == 0 )
						pWndInplace->SetFocus();
					} // if( mi.IsAllowInplaceEditActivation() )
				} // if( pWndInplace != NULL )
				return true;
			} // if( mi.IsInplaceEdit() && (!mi.IsPopup()) )
			if( mi.IsExtraMark() )
			{
				ASSERT( mi.GetExtraMarkCallback() != NULL );
				mi.GetExtraMarkCallback()( this, &mi );
				CClientDC dc( this );
				_DoPaint( dc );
				return true;
			} // if( mi.IsExtraMark() )
			if( mi.IsExecutableCmdID() )
			{
				if(	!(		mi.GetCmdDeliverCb() != NULL
						&&	mi.GetCmdDeliverCb()( this, &mi )
						)
					)
					_EndSequence( mi.GetCmdID(), mi.GetCmdReciever() );
				return true;
			} // if( mi.IsExecutableCmdID() )
		} // if only 1 with this char
		if( m_nCurIndex != nNextIdx
			&& nNextIdx >= 0
			)
		{
			_ItemFocusSet( nNextIdx, TRUE, TRUE );
			if( ItemFindByAccessChar(
					(TCHAR)nMapped,
					nNextIdx
					) < 0
				)
			{ // if only 1 with this char
				MENUITEMDATA & mi =
					_GetItemRef( nNextIdx );
				if( mi.IsInplaceEdit() && (!mi.IsPopup()) )
				{
					CWnd * pWndInplace = mi.GetInplaceEditPtr();
					if( pWndInplace != NULL )
					{
						ASSERT_VALID( pWndInplace );
						ASSERT( pWndInplace->GetSafeHwnd() != NULL && (::IsWindow(pWndInplace->GetSafeHwnd())) );
						if( mi.IsAllowInplaceEditActivation() )
						{
							if( (pWndInplace->GetStyle() & WS_VISIBLE) == 0 )
							{
								CRect rcInplaceEdit;
								_GetItemRect( nNextIdx, rcInplaceEdit );
								rcInplaceEdit = mi.AdjustInplaceEditRect( rcInplaceEdit );
								pWndInplace->SetWindowPos(
									NULL,
									rcInplaceEdit.left, rcInplaceEdit.top,
									rcInplaceEdit.Width(), rcInplaceEdit.Height(),
									SWP_NOZORDER|SWP_NOOWNERZORDER
										|SWP_NOACTIVATE|SWP_SHOWWINDOW
									);
							} // if( (pWndInplace->GetStyle() & WS_VISIBLE) == 0 )
							pWndInplace->SetFocus();
						} // if( mi.IsAllowInplaceEditActivation() )
					} // if( pWndInplace != NULL )
					return true;
				} // if( mi.IsInplaceEdit() && (!mi.IsPopup()) )
				if( mi.IsExtraMark() )
				{
					ASSERT( mi.GetExtraMarkCallback() != NULL );
					mi.GetExtraMarkCallback()( this, &mi );
					CClientDC dc( this );
					_DoPaint( dc );
					return true;
				} // if( mi.IsExtraMark() )
				if( mi.IsExecutableCmdID() )
				{
					if(	!(		mi.GetCmdDeliverCb() != NULL
							&&	mi.GetCmdDeliverCb()( this, &mi )
							)
						)
						_EndSequence( mi.GetCmdID(), mi.GetCmdReciever() );
					return true;
				} // if( mi.IsExecutableCmdID() )
			} // if only 1 with this char
			return true;
		}

	} // BLOCK: try analyze quick char access

	} // switch( nChar )

	if( nit != __NI_NOTHING )
	{
		int nItemIndex = _GetNextItem(nit);
		if( m_items_all.GetSize() > 0
			&& nItemIndex >= 0
			&& nItemIndex != m_nCurIndex
			)
		{
			_ItemFocusSet(
				nItemIndex,
				FALSE,
				TRUE
				);
			return true; // false;
		}
		if( nItemIndex == IDX_EXPAND )
		{
			ASSERT( m_bExpandAvailable );
			ASSERT( !m_bExpandWasPressed );
			int nItemIndex = m_nCurIndex;
			_DoExpand();
			if( nItemIndex >= 0 )
			{
				_ItemFocusSet(
					nItemIndex,
					FALSE,
					//m_bAnimFinished
					( GetSite().GetAnimated() == NULL )
						? TRUE : FALSE
					);
				int i = _GetNextItem(__NI_NEXT);
				_ItemFocusSet(
					(i>0) ? i : nItemIndex,
					FALSE,
					//m_bAnimFinished
					( GetSite().GetAnimated() == NULL )
						? TRUE : FALSE
					);
			}
			return true;
		}
	} // if( nit != __NI_NOTHING )

	return false;
}

INT CExtPopupMenuWnd::ItemFindByText(
	__EXT_MFC_SAFE_LPCTSTR sText,
	INT nStartIdx, // = -1
	BOOL bRestartAt0 // = TRUE
	) const
{
	bRestartAt0;
	ASSERT_VALID( this );

	if( sText == NULL || sText[0] == _T('\0') )
	{
		ASSERT( FALSE );
		return -1;
	}

int nCount = m_items_all.GetSize(); //ItemGetCount();
int nIdx = (nStartIdx >= 0) ? (nStartIdx + 1) : 0;
	for( ; nIdx < nCount; nIdx++ )
	{
		const MENUITEMDATA & mi = _GetItemRef(nIdx);
//		if( !mi.AccelCharIsSet() )
//			continue;
//		if( !mi.IsDisplayed() )
//			continue;
//		if( !mi.IsEnabled() )
//			continue;
		if( _tcscmp( sText, mi.GetText() ) == 0 )
		{
			if( nStartIdx != nIdx )
				return nIdx;
		}
	} // for( nIdx = nStartIdx; nIdx < nCount; nIdx++ )
	if( nStartIdx == 0 )
		return -1;
	for( nIdx = 0; nIdx<nStartIdx; nIdx++ )
	{
		const MENUITEMDATA & mi = _GetItemRef(nIdx);
//		if( !mi.AccelCharIsSet() )
//			continue;
//		if( !mi.IsDisplayed() )
//			continue;
//		if( !mi.IsEnabled() )
//			continue;
		if( _tcscmp( sText, mi.GetText() ) == 0 )
		{
			if( nStartIdx != nIdx )
				return nIdx;
		}
	} // for( nIdx = 0; nIdx<nStartIdx; nIdx++ )
	return -1;
}


INT CExtPopupMenuWnd::ItemFindByAccessChar(
	__EXT_MFC_SAFE_TCHAR chrAccess,
	INT nStartIdx, // = -1
	BOOL bRestartAt0 // = TRUE
	) const
{
	ASSERT_VALID( this );

	bRestartAt0;
	if( chrAccess == _T('0') )
		return -1;
TCHAR szChar[2] = { chrAccess, _T('\0') };
	::CharUpper( szChar );

int cAccelSearch = szChar[0]; //toupper(nChar);
	if( cAccelSearch == 0 )
		return -1;
int nCount = m_items_all.GetSize(); //ItemGetCount();
int nIdx = (nStartIdx >= 0) ? (nStartIdx + 1) : 0;
	for( ; nIdx < nCount; nIdx++ )
	{
		const MENUITEMDATA & mi = _GetItemRef(nIdx);
		if( !mi.AccelCharIsSet() )
			continue;
		if( !mi.IsDisplayed() )
			continue;
		if( !mi.IsEnabled() )
			continue;
		TCHAR cAccel = mi.AccelCharGet();
		if( cAccelSearch ==
				cAccel // _totupper(cAccel)
			)
		{
			if( nStartIdx != nIdx )
				return nIdx;
		}
	} // for( nIdx = nStartIdx; nIdx < nCount; nIdx++ )
	if( nStartIdx == 0 )
		return -1;
	for( nIdx = 0; nIdx<nStartIdx; nIdx++ )
	{
		const MENUITEMDATA & mi = _GetItemRef(nIdx);
		if( !mi.AccelCharIsSet() )
			continue;
		if( !mi.IsDisplayed() )
			continue;
		if( !mi.IsEnabled() )
			continue;
		TCHAR cAccel = mi.AccelCharGet();
		if( cAccelSearch ==
				cAccel // _totupper(cAccel)
				)
		{
			if( nStartIdx != nIdx )
				return nIdx;
		}
	} // for( nIdx = 0; nIdx<nStartIdx; nIdx++ )
	return -1;
}

BOOL CExtPopupMenuWnd::IsCmdKeyActivation(
	LPARAM lParam // = 0
	)
{
	if( (lParam & (1<<29)) != 0 ) // ALT pressed?
		return TRUE;
	
static int nTestKeys[] = {
	VK_MENU,VK_RMENU,VK_LMENU,
	VK_CONTROL,VK_RCONTROL,VK_LCONTROL,
	VK_SHIFT,VK_RSHIFT,VK_LSHIFT
};
	for( int i=0; i<sizeof(nTestKeys)/sizeof(int); i++ )
	{
		if( IsKeyPressed(nTestKeys[i]) )
		{
//			if(		nTestKeys[i] == VK_CONTROL
//				||	nTestKeys[i] == VK_RCONTROL
//				||	nTestKeys[i] == VK_LCONTROL
//				)
//			{
//				if( IsKeyPressed(VK_DOWN) )
//					return FALSE;
//			}
			return TRUE;
		}
	}
	return FALSE;
}

INT CExtPopupMenuWnd::ItemGetCount() const
{
	ASSERT_VALID( this );

	ASSERT_VALID( this );

//	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = m_items_all.GetSize();
	return nCountOfItems;
}

INT CExtPopupMenuWnd::ItemFindPosForCmdID(
	UINT nCmdID,
	INT nPosStart // = -1
	) const
{
	ASSERT_VALID( this );

INT nPos = (nPosStart < 0) ? 0 : nPosStart + 1;
INT nCount = ItemGetCount();
	for( ; nPos < nCount; nPos++ )
	{
		UINT nCmdID2 = ItemGetCmdID( nPos );
		if( nCmdID == nCmdID2 )
			return nPos;
	}
	return -1; // not found
}

UINT CExtPopupMenuWnd::ItemGetCmdID( // menu_item_type_t values can be returned
	INT nPos
	) const
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return TYPE_SEPARATOR;
	}
const MENUITEMDATA & mi = _GetItemRef(nPos);
UINT nCmdID = mi.GetCmdID();
	return nCmdID;
}

bool CExtPopupMenuWnd::ItemSetPopupIcon(
	INT nPos,
	HICON hIcon // = NULL // no icon by default
	)
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return false;
	}
MENUITEMDATA & mi = _GetItemRef(nPos);
	return mi.SetPopupIcon( hIcon );
}

bool CExtPopupMenuWnd::ItemSetPopupText(
	INT nPos,
	__EXT_MFC_SAFE_LPCTSTR sText // NULL if empty
	)
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return false;
	}
MENUITEMDATA & mi = _GetItemRef(nPos);
	if( !mi.SetPopupText( sText ) )
		return false;
	mi.MeasureItem();
	return true;
}

bool CExtPopupMenuWnd::ItemSetPopupAccelText(
	INT nPos,
	__EXT_MFC_SAFE_LPCTSTR sText // NULL if empty
	)
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return false;
	}
MENUITEMDATA & mi = _GetItemRef(nPos);
	return mi.SetPopupAccelText( sText );
}

HICON CExtPopupMenuWnd::ItemGetIcon( INT nPos ) const
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return NULL;
	}
const MENUITEMDATA & mi = _GetItemRef(nPos);
CExtCmdIcon * pIcon = ((MENUITEMDATA &)mi).GetIconPtr();
	if( pIcon == NULL || pIcon->IsEmpty() )
		return NULL;
	return pIcon->GetIcon();
}

CExtPopupMenuWnd::MENUITEMDATA & CExtPopupMenuWnd::ItemGetInfo( INT nPos )
{
	ASSERT_VALID( this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		AfxThrowNotSupportedException();
	}
	return _GetItemRef( nPos );
}

const CExtPopupMenuWnd::MENUITEMDATA & CExtPopupMenuWnd::ItemGetInfo( INT nPos ) const
{
	ASSERT_VALID( this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		AfxThrowNotSupportedException();
	}
	return _GetItemRef( nPos );
}


CExtSafeString CExtPopupMenuWnd::ItemGetText( INT nPos ) const
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return CExtSafeString( _T("") );
	}
const MENUITEMDATA & mi = _GetItemRef(nPos);
	return CExtSafeString( mi.GetText() );
}

CExtSafeString CExtPopupMenuWnd::ItemGetAccelText( INT nPos ) const
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return CExtSafeString( _T("") );
	}
const MENUITEMDATA & mi = _GetItemRef(nPos);
	return CExtSafeString( mi.GetAccelText() );
}

bool CExtPopupMenuWnd::ItemIsDisplayed(
	INT nPos
	) const
{
const MENUITEMDATA & mi = _GetItemRef( nPos );
	return mi.IsDisplayed();
}

void CExtPopupMenuWnd::ItemSetDisplayed(
	INT nPos,
	bool bForceDisplayed // = true
	)
{
MENUITEMDATA & mi = _GetItemRef( nPos );
	mi.SetForceDisplayed( bForceDisplayed );
	_SyncItems();
}

CExtPopupMenuWnd * CExtPopupMenuWnd::ItemGetPopup(
	INT nPos
	)
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );
const CExtPopupMenuWnd * pChild =
		((const CExtPopupMenuWnd *)this)->
			ItemGetPopup( nPos );
	if( pChild == NULL )
		return NULL;
	return
		const_cast
			<CExtPopupMenuWnd *>
				(pChild);
}

void CExtPopupMenuWnd::_DoExpand()
{
	ASSERT_VALID( this );

	ASSERT( !_FindCustomizeMode() );

	_ItemFocusDelay();
	_CoolTipHide( false );
	if( ( GetSite().GetAnimated() != NULL )
		|| _CoolTipIsVisible()
		)
		return;

	ASSERT( m_bExpandAvailable );
	ASSERT( !m_bExpandWasPressed );
	ASSERT( ::IsWindow(GetSafeHwnd()) );
	ASSERT( ( GetSite().GetAnimated() == NULL ) );
	
	m_bExpandWasPressed = true;
CSize m_sizeFullItems2 = m_sizeFullItems;
	_SyncItems();
	_RecalcLayoutImpl();

	if( m_sizeFullItems != m_sizeFullItems2 )
	{ // if size of all items was changed
		ASSERT(
				m_sizeFullItems.cx >= m_sizeFullItems2.cx
			&&	m_sizeFullItems.cy >= m_sizeFullItems2.cy
			);

		CRect rcWndOld;
		GetWindowRect( &rcWndOld );
		
		VERIFY(
			SetWindowRgn(
				NULL,
				FALSE
				)
			);
		ShowWindow( SW_HIDE	);
		ASSERT( !IsWindowVisible() );

		if( !rcWndOld.IsRectEmpty() )
		{
			CWnd * pWndTLP = GetTopLevelParent();
			if( pWndTLP != NULL )
			{
				ASSERT_VALID( pWndTLP );
				if( pWndTLP->IsWindowVisible() )
				{
					CRect rcTLP;
					pWndTLP->GetWindowRect( &rcTLP );
					if( !rcTLP.IsRectEmpty() )
					{
						CRect rcIntersection;
						if(	rcIntersection.IntersectRect(
								&rcTLP,
								&rcWndOld
								)
							)
							pWndTLP->UpdateWindow();
					} // if( !rcTLP.IsRectEmpty() )
				} // if( pWndTLP->IsWindowVisible() )
			} // if( pWndTLP != NULL )
		} // if( !rcWndOld.IsRectEmpty() )

		_FreeWinObjects();

		ASSERT( ::IsWindow(GetSafeHwnd()) );
		ASSERT( ( GetSite().GetAnimated() == NULL ) );

		m_bAnimFinished = false;
		m_AnimationType = __AT_CONTENT_DISPLAY;

		// adjust screen position
		CRect rcWnd = _CalcTrackRect();
		
		if( m_rgnWnd.GetSafeHandle() != NULL )
		{
			ASSERT( m_bExcludeAreaSpec );
			ASSERT( m_bCombineWithEA );
			ASSERT( m_eCombineAlign != __CMBA_NONE );
			CRgn rgnTmp;
			VERIFY( rgnTmp.CreateRectRgn(0,0,0,0) );
			rgnTmp.CopyRgn( &m_rgnWnd );
			ASSERT( rgnTmp.GetSafeHandle() != NULL );
			VERIFY(
				SetWindowRgn(
					(HRGN)rgnTmp.Detach(),
					FALSE
					)
				);
		} // if( m_rgnWnd.GetSafeHandle() != NULL )
		
		m_bAnimFinished = true;

		m_ePlaySoundOnAnimationFinished =
			CExtSoundPlayer::__ON_MENU_EXPAND_CLICKED;
		m_AnimationType =
			g_bMenuExpandAnimation
				? __AT_CONTENT_EXPAND
				: __AT_CONTENT_DISPLAY
				;
		_StartAnimation();
		if( m_AnimationType == __AT_NONE )
		{
			m_AnimationType = __AT_CONTENT_DISPLAY;
			_StartAnimation();
			ASSERT( m_AnimationType == __AT_CONTENT_DISPLAY );
		}

		if( m_bScrollingAvailable )
			_RecalcLayoutImpl();
		SetWindowPos(
			NULL,
			rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),
			SWP_NOACTIVATE
				|SWP_SHOWWINDOW
				//|SWP_NOREDRAW //|SWP_NOSENDCHANGING
				|SWP_NOZORDER //|SWP_NOOWNERZORDER
				//|SWP_NOCOPYBITS
			);

		ASSERT( IsWindowVisible() );

	} // if size of all items was changed
	else
		UpdateWindow();

	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );
	::SendMessage(
		m_hWndCmdReciever,
		g_nMsgNotifyMenuExpanded,
		0,
		LPARAM( this )
		);
}

bool CExtPopupMenuWnd::IsAllItemsRarelyUsed() const
{
	ASSERT_VALID( this );

INT _iter = 0;
	for( ; _iter < m_items_all.GetSize(); ++_iter )
	{
		const MENUITEMDATA & mi = m_items_all[ _iter ];
		if( mi.IsDisplayed() || mi.IsExtraMark() )
			return false;
	} // for( ; _iter != m_items_all.end(); ++_iter )
	return true;
}

void CExtPopupMenuWnd::_SyncItems()
{
	ASSERT_VALID( this );

	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );

INT nIconAreaWidthLager = (__EXT_MENU_ICON_CX+__EXT_MENU_GAP*2);
	for( INT _iter = 0; _iter < m_items_all.GetSize(); ++_iter )
	{
		MENUITEMDATA & mi = m_items_all[ _iter ];
		int nWidth = mi.GetIconAreaWidth();
		nIconAreaWidthLager =
			max( nIconAreaWidthLager, nWidth );
	} // for( INT _iter = 0; _iter < m_items_all.GetSize(); ++_iter )
	for( _iter = 0; _iter < m_items_all.GetSize(); ++_iter )
	{
		MENUITEMDATA & mi = m_items_all[ _iter ];
		mi.UpdateIconAreaWidth( nIconAreaWidthLager );
	} // for( _iter = 0; _iter < m_items_all.GetSize(); ++_iter )

	m_bExpandAvailable = false;
	m_sizeFullItems.cx = m_sizeFullItems.cy = 1;
int nItemIndex = 0;
INT _iterLast = m_items_all.GetSize();
INT _iterLastVisSep = _iterLast;
bool bAtLeastOneDisplayed = false;
	for( _iter = 0; _iter < m_items_all.GetSize(); ++_iter )
	{
		MENUITEMDATA & mi = m_items_all[ _iter ];
		mi.SetIndex( nItemIndex++ );

		if( m_bExpandWasPressed )
		{
			mi.SetDisplayed( true );
		} // if( m_bExpandWasPressed )
		else
		{
			if( mi.IsPopup() )
			{ // if popup sub-menu
				ASSERT( mi.GetPopup() != NULL );
				mi.GetPopup()->_SyncItems();
				CExtCmdItem * pCmdItem = mi.GetCmd();
				if( pCmdItem != NULL )
				{
					bool bRarelyUsed =
						(	(!_FindCustomizeMode())
						&&	pCmdItem->StateIsRarelyUsed()
						) ? true : false;
					if( bRarelyUsed )
					{
						_iterLast = m_items_all.GetSize();
						mi.SetDisplayed( false );
					}
					else
					{
						_iterLast = _iter;
						_iterLastVisSep = m_items_all.GetSize();
						mi.SetDisplayed( true );
					}
				} // if( pCmdItem != NULL )
				else if( mi.IsAllItemsRarelyUsed() )
				{ // if all not displayed
					_iterLast = m_items_all.GetSize();
					mi.SetDisplayed( false );
				} // if all not displayed
				else
				{ // if at least one displayed
					_iterLast = _iter;
					_iterLastVisSep = m_items_all.GetSize();
					mi.SetDisplayed( true );
				} // if at least one displayed
			} // if popup sub-menu
			else
			{ // if separator or command item
				if( mi.IsSeparator() )
				{ // if separator
					if( _iterLast != m_items_all.GetSize()
						||
						(	 _iterLast == m_items_all.GetSize()
							&& (_iterLastVisSep == m_items_all.GetSize())
							&& bAtLeastOneDisplayed
							)
						)
					{
						_iterLastVisSep = _iter;
						mi.SetDisplayed( true );
					}
					else
					{
						_iterLast = m_items_all.GetSize();
						mi.SetDisplayed( false );
					}
				} // if separator
				else
				{ // if command item
					CExtCmdItem * pCmdItem = mi.GetCmd();
					if( pCmdItem == NULL || mi.IsExtraMark() )
					{
						_iterLast = m_items_all.GetSize();
						mi.SetDisplayed( true );
					}
					else if( pCmdItem->StateIsRarelyUsed() )
					{ // if command rarely used
						_iterLast = m_items_all.GetSize();
						mi.SetDisplayed( false );
					} // if command rarely used
					else
					{ // if basic command or frequently used
						_iterLast = _iter;
						_iterLastVisSep = m_items_all.GetSize();
						mi.SetDisplayed( true );
					} // if basic command or frequently used
				} // if command item
			} // if separator or command item
		} // else from if( m_bExpandWasPressed )

		if( mi.IsDisplayed() )
		{
			bAtLeastOneDisplayed = true;
			m_sizeFullItems.cy += mi.GetMeasuredHeight();
			if( m_sizeFullItems.cx < mi.GetMeasuredWidth() )
				m_sizeFullItems.cx = mi.GetMeasuredWidth();
		} // if( mi.IsDisplayed() )
		else
		{
			ASSERT( !m_bExpandWasPressed );
			m_bExpandAvailable = true;
		} // else from if( mi.IsDisplayed() )
	} // for( _iter = 0; _iter < m_items_all.GetSize(); ++_iter )

	if( _iterLastVisSep != m_items_all.GetSize() )
	{ // remove last separator
		if( _iterLastVisSep >= _iterLast
			||
			_iterLast == m_items_all.GetSize()
			)
		{
			m_items_all[_iterLastVisSep].SetDisplayed( false );
			m_sizeFullItems.cy -=
				m_items_all[_iterLastVisSep].GetMeasuredHeight();
		}
	} // remove last separator

static const INT nMixDx = 80;
	if( m_sizeFullItems.cx < nMixDx )
		m_sizeFullItems.cx = nMixDx;
}

const CExtPopupMenuWnd * CExtPopupMenuWnd::ItemGetPopup(
	INT nPos
	) const
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return NULL;
	}
const MENUITEMDATA & mi = _GetItemRef(nPos);
/*
UINT nCmdID = mi.GetCmdID();
	if( nCmdID != TYPE_POPUP )
		return NULL;
	ASSERT( mi.IsPopup() );
*/

	if( !mi.IsPopup() )
		return NULL;

	return mi.GetPopup();
}

BOOL CExtPopupMenuWnd::ItemRemove(
	INT nPos // = -1 // remove all
	)
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );

	if( nPos < 0 )
	{
		if( !_BuildItems( NULL, false ) )
		{
			ASSERT( FALSE );
			return FALSE;
		}
		_SyncItems();
		return TRUE;
	} // if( nPos < 0 )

INT nCountOfItems = ItemGetCount();
	if( nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return FALSE;
	}

MENUITEMDATA & mi = _GetItemRef(nPos);
	if( mi.IsPopup() )
	{
		mi.GetPopup()->_OnCancelMode();
		VERIFY( mi.GetPopup()->_BuildItems( NULL, false ) );
		mi.DestroyPopup();
	}
	m_items_all.RemoveAt( nPos );
	ASSERT( nCountOfItems == m_items_all.GetSize() + 1 );
	m_nCurIndex = -1;

	_SyncItems();

	return TRUE;
}

BOOL CExtPopupMenuWnd::ItemInsertSpecPopup( // insert specific popup implementation
	CExtPopupMenuWnd * pSpecPopup,
	INT nPos, // = -1 // append
	__EXT_MFC_SAFE_LPCTSTR sText, // = NULL
	HICON hIcon // = NULL
	)
{
	ASSERT_VALID( this );

	ASSERT( pSpecPopup != NULL );

INT nCountOfMenuItems = ItemGetCount();
	if( nPos < 0 )
		nPos = nCountOfMenuItems;
	if( nPos > nCountOfMenuItems )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );

MENUITEMDATA mi;
	mi.SetCmdReciever( m_hWndCmdReciever );

	pSpecPopup->m_bTopLevel = false;
	mi.SetPopup( nPos, sText, hIcon, pSpecPopup );
	ASSERT( mi.GetPopup()->m_pWndParentMenu == NULL ); // still not initialized
	ASSERT( !mi.GetPopup()->m_bTopLevel );
	mi.GetPopup()->m_pWndParentMenu = this;
	mi.SetDisplayed( true );

	_InsertItem( nPos, mi );

	if( m_bTopLevel )
		_SyncItems();
	else
	{
		ASSERT( m_pWndParentMenu != NULL );
		m_pWndParentMenu->_SyncItems();
	}

	return TRUE;
}

BOOL CExtPopupMenuWnd::ItemInsert(
	UINT nCmdID, // = TYPE_SEPARATOR,
	INT nPos, // = -1 // append
	__EXT_MFC_SAFE_LPCTSTR sText, // = NULL // for TYPE_POPUP only
	HICON hIcon, // = NULL // for TYPE_POPUP only
	HWND hWndSpecCmdReciever // = NULL // specific command reciever
	)
{
	ASSERT_VALID( this );
	ASSERT( GetTrackingMenu() != this );
INT nCountOfMenuItems = ItemGetCount();
	if( nPos < 0 )
		nPos = nCountOfMenuItems;
	if( nPos > nCountOfMenuItems )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );
MENUITEMDATA mi;
	mi.SetCmdReciever( 
		(hWndSpecCmdReciever != NULL)
			? hWndSpecCmdReciever
			: m_hWndCmdReciever
		);
	switch( nCmdID )
	{
	case TYPE_POPUP:
		mi.SetPopup( nPos, sText, hIcon );
		break; // case TYPE_POPUP
	case TYPE_SEPARATOR:
		mi.SetSeparator( nPos );
		break; // case TYPE_SEPARATOR
	default:
	{
		ASSERT( m_hWndCmdReciever != NULL );
		ASSERT( ::IsWindow(m_hWndCmdReciever) );
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd(
					(hWndSpecCmdReciever != NULL)
						? hWndSpecCmdReciever
						: m_hWndCmdReciever
					),
				nCmdID
				);
		ASSERT( pCmdItem != NULL );
		if( pCmdItem == NULL )
			return FALSE;
		VERIFY(
			mi.UpdateCmdManagerCommand(
				pCmdItem,
				nPos
				)
			);
	}
	break; // default
	} // switch( nCmdID )
	if( mi.IsPopup() )
	{
		ASSERT( mi.GetPopup()->m_pWndParentMenu == NULL ); // still not initialized
		mi.GetPopup()->m_pWndParentMenu = this;
		ASSERT( !mi.GetPopup()->m_bTopLevel );
	}
	_InsertItem( nPos, mi );
	if( m_bTopLevel )
		_SyncItems();
	else
	{
		//ASSERT( m_pWndParentMenu != NULL );
		//m_pWndParentMenu->_SyncItems();
		if( m_pWndParentMenu != NULL )
			m_pWndParentMenu->_SyncItems();
	}
	return TRUE;
}

BOOL CExtPopupMenuWnd::ItemInsertCommand( // no references to command manager
	UINT nCmdID, // = TYPE_SEPARATOR
	INT nPos, // = -1 // append
	__EXT_MFC_SAFE_LPCTSTR sItemText, // = NULL
	__EXT_MFC_SAFE_LPCTSTR sAccelText, // = NULL
	HICON hIcon, // = NULL
	bool bCopyIcon, // = true
	int nCheck, // = 0 // 0 - no check, 1 - checked, 2 - indeterminate, 3 - radio
	HWND hWndSpecCmdReciever // = NULL
	)
{
CExtCmdIcon icon;
	if( hIcon != NULL )
		icon.AssignFromHICON( hIcon, bCopyIcon );
BOOL bRetVal =
		ItemInsertCommand(
			nCmdID, nPos, sItemText, sAccelText,
			icon, nCheck, hWndSpecCmdReciever
			);
	return bRetVal;
}

BOOL CExtPopupMenuWnd::ItemInsertCommand( // no references to command manager
	UINT nCmdID,
	INT nPos, // -1 append
	__EXT_MFC_SAFE_LPCTSTR sItemText, // may be NULL
	__EXT_MFC_SAFE_LPCTSTR sAccelText, // may be NULL
	const CExtCmdIcon & icon,
	int nCheck, // = 0 // 0 - no check, 1 - checked, 2 - indeterminate, 3 - radio
	HWND hWndSpecCmdReciever // = NULL
	)
{
	ASSERT_VALID( this );
	ASSERT( GetTrackingMenu() != this );
INT nCountOfMenuItems = ItemGetCount();
	if( nPos < 0 )
		nPos = nCountOfMenuItems;
	if( nPos > nCountOfMenuItems )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );
MENUITEMDATA mi;
	mi.SetCmdReciever( 
		(hWndSpecCmdReciever != NULL)
			? hWndSpecCmdReciever
			: m_hWndCmdReciever
		);
	mi.SetNoCmdUI( true );

	switch( nCmdID )
	{
	case TYPE_POPUP:
		mi.SetPopup(
			nPos,
			(sItemText == NULL) ? _T("") : sItemText,
			NULL
			);
		mi.SetPopupIcon( icon );
		break; // case TYPE_POPUP
	case TYPE_SEPARATOR:
		mi.SetSeparator( nPos );
		break; // case TYPE_SEPARATOR
	default:
	{
		ASSERT( m_hWndCmdReciever != NULL );
		ASSERT( ::IsWindow(m_hWndCmdReciever) );
		mi.SetCmdID( nCmdID );
		mi.SetText( (sItemText == NULL) ? _T("") : sItemText );
		mi.SetAccelText( (sAccelText == NULL) ? _T("") : sAccelText );
		mi.AccelCharInit();
		mi.SetPopupIcon( icon );
		switch( nCheck )
		{
		case 1: mi.Check( true ); break;
		case 2: mi.Indeterminate( true ); break;
		case 3: mi.Radio( true ); break;
		} // switch( nCheck )
		mi.MeasureItem();
	}
	break; // default
	} // switch( nCmdID )
	mi.SetIndex( nPos );
	mi.SetDisplayed( true );
	if( mi.IsPopup() )
	{
		ASSERT( mi.GetPopup()->m_pWndParentMenu == NULL ); // still not initialized
		mi.GetPopup()->m_pWndParentMenu = this;
		ASSERT( !mi.GetPopup()->m_bTopLevel );
	}
	_InsertItem( nPos, mi );
	if( m_bTopLevel )
		_SyncItems();
	else
	{
		//ASSERT( m_pWndParentMenu != NULL );
		//m_pWndParentMenu->_SyncItems();
		if( m_pWndParentMenu != NULL )
			m_pWndParentMenu->_SyncItems();
	}
	return TRUE;
}

void CExtPopupMenuWnd::SetLeftAreaWidth(
	UINT nWidth // = 0
	)
{
	m_nLeftAreaWidth = nWidth;
}

UINT CExtPopupMenuWnd::GetLeftAreaWidth() const
{
	return m_nLeftAreaWidth;
}

BOOL CExtPopupMenuWnd::CreatePopupMenu( HWND hWndCmdRecv )
{
	ASSERT_VALID( this );
	ASSERT( ::IsWindow(hWndCmdRecv) );

	ASSERT( GetTrackingMenu() != this );
	if( m_bTopLevel
		|| m_pWndParentMenu != NULL
		|| ItemGetCount() != 0
		)
	{
		// already created
		ASSERT( FALSE );
		return FALSE;
	}
	m_sizeFullItems.cx = m_sizeFullItems.cy = 1;
	m_bTopLevel = true;
	m_pWndParentMenu = NULL;
	m_hWndCmdReciever = hWndCmdRecv;
	
	return TRUE;
}

void CExtPopupMenuWnd::SetCmdTargetToAllItems(
	HWND hWndSpecCmdReciever, // = NULL
	bool bOnlyThisLevel // = false
	)
{
	ASSERT_VALID( this );

	ASSERT( GetTrackingMenu() != this );

INT iter = 0;
	for( ; iter < m_items_all.GetSize(); iter++)
	{
		MENUITEMDATA & mi = m_items_all[ iter ];
		if( mi.IsSeparator() )
			continue;
		if( mi.IsPopup() )
		{
			if( !bOnlyThisLevel )
				mi.GetPopup()->SetCmdTargetToAllItems(
					hWndSpecCmdReciever,
					true
					);
		}
		else
		{
			mi.SetCmdReciever( hWndSpecCmdReciever );
		}
	}
}

//CExtPopupMenuWnd::const_items_container_t &
//	CExtPopupMenuWnd::_GetItemsContaner() const
//{
//	return m_items_all;
//}
//
//CExtPopupMenuWnd::items_container_t &
//	CExtPopupMenuWnd::_GetItemsContaner()
//{
//	return m_items_all;
//}

CExtPopupMenuWnd::MENUITEMDATA &
	CExtPopupMenuWnd::_GetItemRef(int nIndex)
{
	ASSERT_VALID( this );

	ASSERT( nIndex >= 0 && nIndex < m_items_all.GetSize() );
MENUITEMDATA & mi = m_items_all[nIndex];
	return mi;
}

const CExtPopupMenuWnd::MENUITEMDATA &
	CExtPopupMenuWnd::_GetItemRef(int nIndex) const
{
	ASSERT_VALID( this );

	return
		(const_cast< CExtPopupMenuWnd * > (this)) ->
			_GetItemRef(nIndex);
}

void CExtPopupMenuWnd::_InsertItem(
	int nInsertBefore,
	MENUITEMDATA & mi
	)
{
	ASSERT_VALID( this );

	m_items_all.InsertAt(
		(nInsertBefore < 0)
			? m_items_all.GetSize()
			: nInsertBefore
			,
		mi
		);
}

bool CExtPopupBaseWnd::_IsPopupWithShadows() const
{
	if( ! CExtPopupMenuWnd::g_bMenuWithShadows )
		return false;

	return true;
}

bool CExtPopupMenuWnd::_IsPopupWithShadows() const
{
	if( ! CExtPopupBaseWnd::_IsPopupWithShadows() )
		return false;

	if( m_dwTrackFlags & TPMX_NO_SHADOWS )
		return false;

	return true;
}

int CExtPopupBaseWnd::_GetPopupShadowSize() const
{
	if( !_IsPopupWithShadows() )
		return 0;
//int nBitsPerPixel = CExtPaintManager::stat_GetBPP();
//	if( nBitsPerPixel <= 8 )
//		return 0;
	return g_PaintManager->GetMenuShadowSize();
};

int CExtPopupMenuWnd::_GetPopupShadowSize() const
{
	return CExtPopupBaseWnd::_GetPopupShadowSize();
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
void CExtPopupMenuWnd::_SetDropIndexBefore(
	int nIndex // = -1 // reset
	)
{
	ASSERT_VALID( this );
	if( m_nHelperDropIndexBefore == nIndex )
		return;
	m_nHelperDropIndexBefore = nIndex;
	if(		GetSafeHwnd() == NULL
		||	(! ::IsWindow(GetSafeHwnd()) )
		)
		return;
CClientDC dc( this );
	_DoPaint( dc );
}

// CExtCustomizeSite::ICustomizeDropTarget
DROPEFFECT CExtPopupMenuWnd::OnCustomizeTargetOver(
	CExtCustomizeSite::CCmdDragInfo & _dragInfo,
	CPoint point,
	DWORD dwKeyState
	)
{
	ASSERT_VALID( this );
	ASSERT( !_dragInfo.IsEmpty() );
	dwKeyState;
	point;

	if( m_pDragSrcNode != NULL )
		m_bHelperDragOverPassed = true;

	ASSERT_VALID( m_pNode );
	ASSERT_VALID( _dragInfo.m_pDropNodeC );
	if(		m_pNode == _dragInfo.m_pDropNodeC
		||	_dragInfo.m_pDropNodeC->IsChild( m_pNode )
		)
		return DROPEFFECT_NONE;

//	_DoResetChainPositions();

	if( ItemGetCount() == 0 )
	{
		if( (dwKeyState & MK_CONTROL) != NULL )
			return DROPEFFECT_COPY;
		return DROPEFFECT_MOVE;
	}

int nIndex = _HitTest( point );
	if( nIndex < 0 )
	{
		if(		nIndex == IDX_SCROLL_TOP
			||	nIndex == IDX_SCROLL_BOTTOM
			)
			_StartScrolling( nIndex );
		_SetDropIndexBefore();
		//_ItemFocusCancel( TRUE );
		return DROPEFFECT_NONE;
	}
MENUITEMDATA & miPreTest = CExtPopupMenuWnd::_GetItemRef( nIndex );
	if( miPreTest.IsSeparator() )
	{
		if( nIndex == 0 )
		{
			ASSERT( FALSE );
			_SetDropIndexBefore();
			_ItemFocusCancel( TRUE );
			return DROPEFFECT_NONE;
		} // if( nIndex == 0 )
		nIndex--;
	} // if( miPreTest.IsSeparator() )
MENUITEMDATA & mi = CExtPopupMenuWnd::_GetItemRef( nIndex );
	if( mi.IsSeparator() )
	{
		ASSERT( FALSE );
		_SetDropIndexBefore();
		_ItemFocusCancel( TRUE );
		return DROPEFFECT_NONE;
	} // if( mi.IsSeparator() )
CRect rcItem;
	_GetItemRect( nIndex, rcItem );
int nMidY = rcItem.CenterPoint().y;
	if( point.y > nMidY )
		nIndex++;
	_SetDropIndexBefore( nIndex );
	if( mi.IsPopup() )
		_ItemFocusSet( mi.GetIndex(), TRUE, TRUE, FALSE );
	else
		_ItemFocusCancel( TRUE );
	if( (dwKeyState & MK_CONTROL) != NULL )
		return DROPEFFECT_COPY;
	return DROPEFFECT_MOVE;
}
void CExtPopupMenuWnd::OnCustomizeTargetLeave()
{
	ASSERT_VALID( this );
	_SetDropIndexBefore();
}
bool CExtPopupMenuWnd::OnCustomizeTargetDrop(
	CExtCustomizeSite::CCmdDragInfo & _dragInfo,
	CPoint point,
	DROPEFFECT de
	)
{
	ASSERT_VALID( this );
	ASSERT( !_dragInfo.IsEmpty() );
	_dragInfo;
	point;
	ASSERT(
			de == DROPEFFECT_NONE
		||	de == DROPEFFECT_MOVE
		||	de == DROPEFFECT_COPY
		);
int nItemCount = ItemGetCount();
	if(		de == DROPEFFECT_NONE
		||	( nItemCount > 0 && m_nHelperDropIndexBefore < 0 )
		)
	{
		_SetDropIndexBefore();
		m_ptHelperDragStart.x = m_ptHelperDragStart.y = 0;
		return false;
	}
CExtCustomizeSite * pSite = _FindCustomizeSite();
	ASSERT( pSite != NULL );
	ASSERT_VALID( m_pNode );
	ASSERT_VALID( _dragInfo.m_pDropNodeC );
bool bRetVal = true;
	m_bHelperNoHideChild = true;
int nNodeInsertPos = -1;
CExtCustomizeCmdTreeNode * pClonedNode = _dragInfo.m_pDropNodeC->CloneNode( pSite );
	ASSERT_VALID( pClonedNode );
	if( _dragInfo.m_pDropNodeI != NULL )
	{
		ASSERT_VALID( _dragInfo.m_pDropNodeI );
		// reset basic command ID
		pClonedNode->SetCmdID( _dragInfo.m_pDropNodeI->GetCmdID( false ), false );
		pClonedNode->SetCmdID( _dragInfo.m_pDropNodeC->GetCmdID( true ), true );
	} // if( _dragInfo.m_pDropNodeI != NULL )
	if( nItemCount == 0 )
	{
		nNodeInsertPos = 0;
		pClonedNode->ModifyFlags( 0, __ECTN_GROUP_START );
	} // if( nItemCount == 0 )
	else
	{
		ASSERT( m_nHelperDropIndexBefore >= 0 && m_nHelperDropIndexBefore <= nItemCount );
		if( m_nHelperDropIndexBefore == nItemCount )
		{
			nNodeInsertPos = m_pNode->GetNodeCount();
		} // if( m_nHelperDropIndexBefore == nItemCount )
		else
		{
			if( m_nHelperDropIndexBefore == 0 )
				pClonedNode->ModifyFlags( 0, __ECTN_GROUP_START );
			MENUITEMDATA & miDropPos = _GetItemRef( m_nHelperDropIndexBefore );
			CExtCustomizeCmdTreeNode * pDropBeforeNode =
				miDropPos.IsSeparator()
					? ( _GetItemRef( m_nHelperDropIndexBefore + 1 ).GetCmdNode() )
					: ( miDropPos.GetCmdNode() )
					;
			ASSERT_VALID( pDropBeforeNode );
			nNodeInsertPos = pDropBeforeNode->GetOwnIndex();
			if( (pDropBeforeNode->GetFlags() & __ECTN_GROUP_START) != 0 )
			{
				ASSERT( m_nHelperDropIndexBefore > 0 );
				pClonedNode->ModifyFlags( __ECTN_GROUP_START, 0 );
				pDropBeforeNode->ModifyFlags( 0, __ECTN_GROUP_START );
			} // if( (pDropBeforeNode->GetFlags() & __ECTN_GROUP_START) != 0 )
			else
			{
			} // else from if( (pDropBeforeNode->GetFlags() & __ECTN_GROUP_START) != 0 )
			if( m_pDragSrcNode != NULL )
			{
				ASSERT( 0 <= m_nDragSrcIdx && m_nDragSrcIdx < nItemCount );
				if( pDropBeforeNode == m_pDragSrcNode )
				{
					delete pClonedNode;
					_SetDropIndexBefore();
					ASSERT( m_nDragSrcIdx >= 0 && m_nDragSrcIdx < nItemCount );
					_ItemFocusSet( m_nDragSrcIdx, TRUE, TRUE, FALSE );
					m_bHelperNoHideChild = true;
					CClientDC dc( this );
					_DoPaint( dc );
					m_bHelperNoRemoveSrc = true;
					return true;
				} // if( pDropBeforeNode == m_pDragSrcNode )
				int nDragNodeIdx = m_pDragSrcNode->GetOwnIndex();
				if( nDragNodeIdx > 0 )
				{
					CExtCustomizeCmdTreeNode * pDragNodePr = 
						m_pDragSrcNode->GetParentNode()->ElementAt( nDragNodeIdx-1 );
					ASSERT_VALID( pDragNodePr );
					if( pDragNodePr == m_pDragSrcNode )
					{
						delete pClonedNode;
						_SetDropIndexBefore();
						ASSERT( m_nDragSrcIdx >= 0 && m_nDragSrcIdx < nItemCount );
						_ItemFocusSet( m_nDragSrcIdx, TRUE, TRUE, FALSE );
						m_bHelperNoHideChild = true;
						CClientDC dc( this );
						_DoPaint( dc );
						m_bHelperNoRemoveSrc = true;
						return true;
					} // if( pDragNodePr == m_pDragSrcNode )
				} // if( nDragNodeIdx > 0 )
			} // if( m_pDragSrcNode != NULL )
		} // else from if( m_nHelperDropIndexBefore == nItemCount )
	} // else from if( nItemCount == 0 )
	_ItemFocusCancel( TRUE );
	ASSERT( 0 <= nNodeInsertPos && nNodeInsertPos <= m_pNode->GetNodeCount() );
	m_pNode->InsertNode( pSite, pClonedNode, nNodeInsertPos );

MENUITEMDATA mi;
	mi.SetCmdReciever( m_hWndCmdReciever );
	VERIFY(
		mi.UpdateFromCmdTree(
			m_hWndCmdReciever,
			pClonedNode,
			m_nHelperDropIndexBefore
			)
		);
	if( mi.IsPopup() )
	{
		mi.GetPopup()->m_hWndCmdReciever = m_hWndCmdReciever;
		mi.GetPopup()->UpdateFromCmdTree(
			m_hWndCmdReciever,
			pClonedNode,
			false
			);
		mi.GetPopup()->m_pWndParentMenu = this;
	} // if( mi.IsPopup() )
	ASSERT( !mi.IsSeparator() );
	ASSERT( ((LPVOID)mi.GetCmdNode()) == ((LPVOID)pClonedNode) );
	_InsertItem( m_nHelperDropIndexBefore, mi );

	_SyncItems();
	ShowWindow( SW_HIDE	);
	_RecalcLayoutImpl();
	VERIFY( SetWindowRgn( NULL, FALSE ) );
	ASSERT( !IsWindowVisible() );
	_FreeWinObjects();
// adjust screen position
CRect rcWnd = _CalcTrackRect();
	if( m_rgnWnd.GetSafeHandle() != NULL )
	{
		ASSERT( m_bExcludeAreaSpec );
		ASSERT( m_bCombineWithEA );
		ASSERT( m_eCombineAlign != __CMBA_NONE );
		CRgn rgnTmp;
		VERIFY( rgnTmp.CreateRectRgn(0,0,0,0) );
		rgnTmp.CopyRgn( &m_rgnWnd );
		ASSERT( rgnTmp.GetSafeHandle() != NULL );
		VERIFY(
			SetWindowRgn(
				(HRGN)rgnTmp.Detach(),
				FALSE
				)
			);
	} // if( m_rgnWnd.GetSafeHandle() != NULL )
	SetWindowPos(
		NULL,
		rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),
		SWP_NOACTIVATE
			|SWP_NOZORDER //|SWP_NOOWNERZORDER
		);
	if( m_bScrollingAvailable )
		_RecalcLayoutImpl();
	ShowWindow( SW_SHOWNA );

	_SetDropIndexBefore();
	m_ptHelperDragStart.x = m_ptHelperDragStart.y = 0;
	return bRetVal;
}

// CExtCustomizeSite::ICustomizeDropSource
void CExtPopupMenuWnd::OnCustomizeSourceDragComplete(
	DROPEFFECT de,
	bool bCanceled,
	bool * p_bNoResetActiveItem
	)
{
	ASSERT_VALID( this );
	ASSERT(
			de == DROPEFFECT_NONE
		||	de == DROPEFFECT_MOVE
		||	de == DROPEFFECT_COPY
		);
	ASSERT( p_bNoResetActiveItem != NULL );
//	if(		m_ptHelperDragStart.y != 0
//		&&	m_bHelperNoHideChild
//		&&	de != DROPEFFECT_NONE
//		)
//	{
//		m_ptHelperDragStart.x = m_ptHelperDragStart.y = 0;
//		return;
//	}
	m_ptHelperDragStart.x = m_ptHelperDragStart.y = 0;
	if( GetSafeHwnd() != NULL && (!m_bHelperNoHideChild) )
		_ItemFocusCancel( TRUE );
	m_bHelperNoHideChild = false;
CExtCustomizeSite * pSite = _FindCustomizeSite();
	ASSERT( pSite != NULL );
	if(		(!m_bHelperDragOverPassed)
		&&	m_pDragSrcNode != NULL
		)
		m_bHelperNoRemoveSrc = true;
	if( bCanceled || m_bHelperNoRemoveSrc || de == DROPEFFECT_COPY )
	{
		m_bHelperNoRemoveSrc = false;
		if( pSite->IsCustomizeMode() )
			*p_bNoResetActiveItem = true;
		return;
	} // if( bCanceled || de == DROPEFFECT_COPY || m_bHelperAddRemSeparator )

CExtCustomizeCmdTreeNode * pNode = pSite->DraggedNodeGet();
	ASSERT_VALID( pNode );
	ASSERT_VALID( m_pNode );
	ASSERT( ((LPVOID)pNode->GetParentNode()) == ((LPVOID)m_pNode) );
int nItemCount = ItemGetCount();
bool bPrevSeparator = false;
	for( int nItem = 0; nItem < nItemCount; nItem++ )
	{
		MENUITEMDATA & mi = _GetItemRef( nItem );
		if( mi.IsSeparator() )
		{
			ASSERT( !bPrevSeparator );
			bPrevSeparator = true;
			continue;
		}
		CExtCustomizeCmdTreeNode * pCurrNode = mi.GetCmdNode();
		ASSERT_VALID( pCurrNode );
		if( ((LPVOID)pCurrNode) == ((LPVOID)pNode) )
			break;
		bPrevSeparator = false;
	} // for( int nItem = 0; nItem < nItemCount; nItem++ )
	ASSERT( nItem < nItemCount ); // should be found
	if( bPrevSeparator )
	{
//		ASSERT( (pNode->GetFlags() & __ECTN_GROUP_START) != 0 );
		pNode->ModifyFlags( __ECTN_GROUP_START, 0 );
		ASSERT( nItem > 0 );
		MENUITEMDATA & miRemove = _GetItemRef( nItem-1 );
		ASSERT( miRemove.IsSeparator() );
		pNode->ModifyFlags( 0, __ECTN_GROUP_START );
//		if( miRemove.IsPopup() )
//			miRemove.DestroyPopup();
		m_items_all.RemoveAt( nItem-1, 2 );
		m_nCurIndex = -1;
		nItemCount -= 2;
	} // if( bPrevSeparator )
	else
	{
//		ASSERT( (pNode->GetFlags() & __ECTN_GROUP_START) == 0 );
		pNode->ModifyFlags( 0, __ECTN_GROUP_START );
		MENUITEMDATA & miRemove = _GetItemRef( nItem );
		if( miRemove.IsPopup() )
			miRemove.DestroyPopup();
		m_items_all.RemoveAt( nItem, 1 );
		m_nCurIndex = -1;
		nItemCount--;
	} // else from if( bPrevSeparator )
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(m_hWnd),
			pNode->GetCmdID( false )
			);
	ASSERT( pCmdItem != NULL );
	pNode->RemoveSelf( pCmdItem );
	pSite->DraggedNodeSet( NULL );
	if(		nItemCount > 0
		&&	_GetItemRef(0).IsSeparator()
		)
	{
		m_items_all.RemoveAt( 0, 1 );
		m_nCurIndex = -1;
		m_pNode->ElementAt(0)->ModifyFlags( 0, __ECTN_GROUP_START );
		nItemCount--;
	} // if( nItemCount > 0 ...
	if(		nItemCount > 0
		&&	_GetItemRef(nItemCount-1).IsSeparator()
		)
	{
		m_items_all.RemoveAt( nItemCount-1, 1 );
		m_nCurIndex = -1;
		nItemCount--;
	} // if( nItemCount > 0 ...

	_SyncItems();
	_SetDropIndexBefore();

	if( m_hWnd == NULL || (! ::IsWindow(m_hWnd) ) )
		return;

	ShowWindow( SW_HIDE	);
	_RecalcLayoutImpl();
	VERIFY(
		SetWindowRgn(
			NULL,
			FALSE
			)
		);
	ASSERT( !IsWindowVisible() );
	_FreeWinObjects();
// adjust screen position
CRect rcWnd = _CalcTrackRect();
	if( m_rgnWnd.GetSafeHandle() != NULL )
	{
		ASSERT( m_bExcludeAreaSpec );
		ASSERT( m_bCombineWithEA );
		ASSERT( m_eCombineAlign != __CMBA_NONE );
		CRgn rgnTmp;
		VERIFY( rgnTmp.CreateRectRgn(0,0,0,0) );
		rgnTmp.CopyRgn( &m_rgnWnd );
		ASSERT( rgnTmp.GetSafeHandle() != NULL );
		VERIFY(
			SetWindowRgn(
				(HRGN)rgnTmp.Detach(),
				FALSE
				)
			);
	} // if( m_rgnWnd.GetSafeHandle() != NULL )
	SetWindowPos(
		NULL,
		rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),
		SWP_NOACTIVATE
			|SWP_NOZORDER //|SWP_NOOWNERZORDER
		);
	if( m_bScrollingAvailable )
		_RecalcLayoutImpl();
	ShowWindow( SW_SHOWNA );

}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

/////////////////////////////////////////////////////////////////////////////
// CExtPopupColorMenuWnd

IMPLEMENT_DYNCREATE(CExtPopupColorMenuWnd, CExtPopupMenuWnd)

BEGIN_MESSAGE_MAP(CExtPopupColorMenuWnd, CExtPopupMenuWnd)
	//{{AFX_MSG_MAP(CExtPopupColorMenuWnd)
	//}}AFX_MSG_MAP
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
END_MESSAGE_MAP()

UINT CExtPopupColorMenuWnd::g_nMsgNotifyColorChanged =
	::RegisterWindowMessage(
		_T("CExtPopupColorMenuWnd::g_nMsgNotifyColorChanged")
		);
UINT CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally =
	::RegisterWindowMessage(
		_T("CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally")
		);

UINT CExtPopupColorMenuWnd::g_nMsgNotifyCustColor =
	::RegisterWindowMessage(
		_T("CExtPopupColorMenuWnd::g_nMsgNotifyCustColor")
		);

#define __NCLR_DX 8
#define __NCLR_DY 5
#define __NCLR_COUNT (__NCLR_DX*__NCLR_DY)
#define __NCLR_BOX_DX_SIZE 16
#define __NCLR_BOX_DY_SIZE 16
#define __NCLR_BOX_DX_SPACE 2
#define __NCLR_BOX_DY_SPACE 2

// table captured from color picker
// control source by Chris Maunder
CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY
	CExtPopupColorMenuWnd::g_colors[] =
{
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x00, 0x00, 0x00),    _T("Black")             ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xA5, 0x2A, 0x00),    _T("Brown")             ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x00, 0x40, 0x40),    _T("Dark Olive Green")  ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x00, 0x55, 0x00),    _T("Dark Green")        ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x00, 0x00, 0x5E),    _T("Dark Teal")         ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x00, 0x00, 0x8B),    _T("Dark blue")         ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x4B, 0x00, 0x82),    _T("Indigo")            ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x28, 0x28, 0x28),    _T("Dark grey")         ),

    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x8B, 0x00, 0x00),    _T("Dark red")          ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xFF, 0x68, 0x20),    _T("Orange")            ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x8B, 0x8B, 0x00),    _T("Dark yellow")       ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x00, 0x93, 0x00),    _T("Green")             ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x38, 0x8E, 0x8E),    _T("Teal")              ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x00, 0x00, 0xFF),    _T("Blue")              ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x7B, 0x7B, 0xC0),    _T("Blue-grey")         ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x66, 0x66, 0x66),    _T("Grey - 40")         ),

    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xFF, 0x00, 0x00),    _T("Red")               ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xFF, 0xAD, 0x5B),    _T("Light orange")      ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x32, 0xCD, 0x32),    _T("Lime")              ), 
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x3C, 0xB3, 0x71),    _T("Sea green")         ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x7F, 0xFF, 0xD4),    _T("Aqua")              ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x7D, 0x9E, 0xC0),    _T("Light blue")        ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x80, 0x00, 0x80),    _T("Violet")            ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x7F, 0x7F, 0x7F),    _T("Grey - 50")         ),

    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xFF, 0xC0, 0xCB),    _T("Pink")              ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xFF, 0xD7, 0x00),    _T("Gold")              ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xFF, 0xFF, 0x00),    _T("Yellow")            ),    
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x00, 0xFF, 0x00),    _T("Bright green")      ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x40, 0xE0, 0xD0),    _T("Turquoise")         ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xC0, 0xFF, 0xFF),    _T("Skyblue")           ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x48, 0x00, 0x48),    _T("Plum")              ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xC0, 0xC0, 0xC0),    _T("Light grey")        ),

    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xFF, 0xE4, 0xE1),    _T("Rose")              ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xD2, 0xB4, 0x8C),    _T("Tan")               ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xFF, 0xFF, 0xE0),    _T("Light yellow")      ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x98, 0xFB, 0x98),    _T("Pale green ")       ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xAF, 0xEE, 0xEE),    _T("Pale turquoise")    ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0x68, 0x83, 0x8B),    _T("Pale blue")         ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xE6, 0xE6, 0xFA),    _T("Lavender")          ),
    CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY( RGB(0xFF, 0xFF, 0xFF),    _T("White")             ),
};

CExtPopupColorMenuWnd::CExtPopupColorMenuWnd()
{
	ASSERT( __NCLR_COUNT == sizeof(g_colors)/sizeof(COLORREF_TABLE_ENTRY) );

	m_nColorIdxCurr
		= m_nInitialColorIdx
		= -1;

	m_hWndNotifyColorChanged = NULL; // - use command targed

	m_clrInitial = (COLORREF)(-1); // unexisting
	m_clrDefault = RGB(0,0,0);
	m_bEnableBtnColorDefault = true;
	m_bEnableBtnColorCustom = true;

	m_rcDefColorText.SetRectEmpty();
	m_rcCustColorText.SetRectEmpty();

	m_lParamCookie = 0;

#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
	
	if( !m_sBtnTextColorDefault.LoadString(IDS_COLOR_DEFAULT) )
	{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		m_sBtnTextColorDefault = _T("Default Color");
	}
	if( !m_sBtnTextColorCustom.LoadString(IDS_COLOR_CUSTOM) )
	{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		m_sBtnTextColorCustom = _T("Custom Color ...");
	}
}

BOOL CExtPopupColorMenuWnd::OnQueryNewPalette() 
{
	ASSERT_VALID( this );

	Invalidate();
	return CWnd::OnQueryNewPalette();
}

void CExtPopupColorMenuWnd::OnPaletteChanged(CWnd* pFocusWnd) 
{
	ASSERT_VALID( this );

	CWnd::OnPaletteChanged(pFocusWnd);
	if( pFocusWnd->GetSafeHwnd() != GetSafeHwnd() )
	{
		Invalidate();
	}
}

CSize CExtPopupColorMenuWnd::_CalcTrackSize()
{
	ASSERT_VALID( this );

int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nMenuShadowSize = _GetPopupShadowSize();
CSize _size(
		(__NCLR_BOX_DX_SIZE + __NCLR_BOX_DX_SPACE*2) * __NCLR_DX
			+ nMenuBorderSize*2 + nMenuShadowSize
			,
		(__NCLR_BOX_DY_SIZE + __NCLR_BOX_DY_SPACE*2) * __NCLR_DY
			+ nMenuBorderSize*2 + nMenuShadowSize
		);
	if( m_bEnableBtnColorDefault )
	{
		_size.cy +=
			__EXT_MENU_SEPARATOR_HEIGHT
			+ __EXT_MENU_GAP*2;
		CExtSafeString sMeasureText( m_sBtnTextColorDefault );
		sMeasureText.Replace( _T("&"), _T("") );

		CWindowDC dc(NULL);
		CFont * pOldFont =
			dc.SelectObject( &(g_PaintManager->m_FontNormal) );
		ASSERT( pOldFont != NULL );
		CRect rect(0,0,0,0);
		CSize _sizeDefColorText;
		_sizeDefColorText.cy = DrawText(
			dc.GetSafeHdc(),
			sMeasureText,
			sMeasureText.GetLength(),
			&rect,
			DT_CALCRECT|DT_SINGLELINE
				|DT_LEFT|DT_VCENTER
			);
		_sizeDefColorText.cx = rect.Width();
		dc.SelectObject( pOldFont );
		_sizeDefColorText.cx += __EXT_MENU_GAP * 2;
		if( _sizeDefColorText.cy < __EXT_MENU_MIN_HEIGHT )
			_sizeDefColorText.cy = __EXT_MENU_MIN_HEIGHT;
		_size.cy += _sizeDefColorText.cy;
		if( _size.cx < _sizeDefColorText.cx )
			_size.cx = _sizeDefColorText.cx;
		CPoint pt(
			nMenuBorderSize + __EXT_MENU_GAP
				,
			nMenuBorderSize + __EXT_MENU_GAP
			);
		m_rcDefColorText.SetRect(
			pt,
			pt
			+
			CSize(
				max( _size.cx , _sizeDefColorText.cx )
					- __EXT_MENU_GAP*2
					- nMenuBorderSize*2
					- nMenuShadowSize
					,
				_sizeDefColorText.cy
				)
			);
	} // if( m_bEnableBtnColorDefault )
	if( m_bEnableBtnColorCustom )
	{
		_size.cy +=
			__EXT_MENU_SEPARATOR_HEIGHT
			+ __EXT_MENU_GAP*2;
		CExtSafeString sMeasureText( m_sBtnTextColorCustom );
		sMeasureText.Replace( _T("&"), _T("") );

		CWindowDC dc(NULL);
		CFont * pOldFont =
			dc.SelectObject( &(g_PaintManager->m_FontNormal) );
		ASSERT( pOldFont != NULL );
		CRect rect(0,0,0,0);
		CSize _sizeCustColorText;
		_sizeCustColorText.cy = DrawText(
			dc.GetSafeHdc(),
			sMeasureText,
			sMeasureText.GetLength(),
			&rect,
			DT_CALCRECT|DT_SINGLELINE
				|DT_LEFT|DT_VCENTER
			);
		_sizeCustColorText.cx = rect.Width();
		dc.SelectObject( pOldFont );
		_sizeCustColorText.cx += __EXT_MENU_GAP * 2;
		if( _sizeCustColorText.cy < __EXT_MENU_MIN_HEIGHT )
			_sizeCustColorText.cy = __EXT_MENU_MIN_HEIGHT;
		_size.cy += _sizeCustColorText.cy;
		if( _size.cx < _sizeCustColorText.cx )
			_size.cx = _sizeCustColorText.cx;
		CPoint pt(
			nMenuBorderSize + __EXT_MENU_GAP
				,
			nMenuBorderSize + __EXT_MENU_GAP
			);
		m_rcCustColorText.SetRect(
			pt,
			pt
			+
			CSize(
				max( _size.cx , _sizeCustColorText.cx )
					- __EXT_MENU_GAP*2
					- nMenuBorderSize*2
					- nMenuShadowSize
					,
				_sizeCustColorText.cy
				)
			);
		if( m_bEnableBtnColorDefault )
			m_rcCustColorText.OffsetRect(
				0,
				m_rcCustColorText.Height()
				+ __EXT_MENU_GAP*2
				+ __EXT_MENU_SEPARATOR_HEIGHT
				);
		m_rcCustColorText.OffsetRect(
			0,
			nMenuBorderSize +
				(__NCLR_BOX_DY_SIZE + __NCLR_BOX_DY_SPACE*2) * __NCLR_DY
				+ __NCLR_BOX_DY_SPACE
			);
	} // if( m_bEnableBtnColorCustom )

	_size.cx += m_nLeftAreaWidth;
	return _size;
}

CPoint CExtPopupColorMenuWnd::_GetColorItemCoord(int nIdx)
{
	ASSERT_VALID( this );

	ASSERT( nIdx >= 0 && nIdx < __NCLR_COUNT );
int nX = nIdx % __NCLR_DX;
	ASSERT( nX < __NCLR_DX );
int nY = nIdx / __NCLR_DX;
	ASSERT( nY < __NCLR_DY );
	return CPoint(nX,nY);
}

CRect CExtPopupColorMenuWnd::_GetColorItemRect(int nIdx)
{
	ASSERT_VALID( this );

	ASSERT( nIdx >= 0 && nIdx < __NCLR_COUNT );
CPoint ptCoord =  _GetColorItemCoord(nIdx);
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
CRect rcItem(
		CPoint(
			nMenuBorderSize +
				(__NCLR_BOX_DX_SIZE + __NCLR_BOX_DX_SPACE*2) * ptCoord.x
				+ __NCLR_BOX_DX_SPACE
				,
			nMenuBorderSize +
				(__NCLR_BOX_DY_SIZE + __NCLR_BOX_DY_SPACE*2) * ptCoord.y
				+ __NCLR_BOX_DY_SPACE
			),
		CSize(__NCLR_BOX_DX_SIZE,__NCLR_BOX_DY_SIZE)
		);
	if( m_bEnableBtnColorDefault )
	{
		rcItem.OffsetRect(
			0,
			m_rcDefColorText.Height()
			+ __EXT_MENU_SEPARATOR_HEIGHT
			+ __EXT_MENU_GAP*2
			);
	}
CRect rcClient;
	_GetClientRect( &rcClient );
	rcItem.OffsetRect( rcClient.TopLeft() );
	rcItem.OffsetRect( m_nLeftAreaWidth, 0 );
	return rcItem;
}

CRect CExtPopupColorMenuWnd::_CalcTrackRect()
{
	ASSERT_VALID( this );
	return CExtPopupMenuWnd::_CalcTrackRect();
}

bool CExtPopupColorMenuWnd::_CreateHelper(
	CWnd * pWndCmdReciever
	)
{
	ASSERT_VALID( this );

	if( !CExtPopupMenuWnd::_CreateHelper(
			pWndCmdReciever
			)
		)
		return false;
	int nIdx =
		_FindCellByColorRef(m_clrInitial);
	if( nIdx >= 0 )
	{
		m_nInitialColorIdx = nIdx;
		Invalidate();
	}
CRect rcClient;
	_GetClientRect( &rcClient );
	if( m_bEnableBtnColorDefault )
	{
		m_rcDefColorText.OffsetRect( rcClient.TopLeft() );
		m_rcDefColorText.OffsetRect( m_nLeftAreaWidth, 0 );
	}
	if( m_bEnableBtnColorCustom )
	{
		m_rcCustColorText.OffsetRect( rcClient.TopLeft() );
		m_rcCustColorText.OffsetRect( m_nLeftAreaWidth, 0 );
	}
	return true;
}

void CExtPopupColorMenuWnd::_DoPaint( CDC & dcPaint, bool bUseBackBuffer /*= true*/ )
{
	ASSERT_VALID( this );

	ASSERT_VALID( (&dcPaint) );
	ASSERT( dcPaint.GetSafeHdc() != NULL );

CRect rcRealClient;
	GetClientRect( &rcRealClient );
CRect rcClient;
	_GetClientRect( &rcClient );

    // Select and realize the palette
CPalette * pOldPalette = NULL;
    if( dcPaint.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
    {
        pOldPalette =
			dcPaint.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE );
        dcPaint.RealizePalette();
    }

CExtMemoryDC mdc;
	if( bUseBackBuffer )
	{
		mdc.__InitMemoryDC(
			&dcPaint,
			&rcClient, // &rcRealClient
			CExtMemoryDC::MDCOPT_TO_MEMORY
				|CExtMemoryDC::MDCOPT_FILL_SURFACE
				|CExtMemoryDC::MDCOPT_FORCE_DIB 
			);
	}
CDC & dcDummyRef = mdc;
CDC & dc = bUseBackBuffer ? dcDummyRef : dcPaint;

CFont * pOldFont = (CFont *)
		dc.SelectObject(
			&g_PaintManager->m_FontNormal
			);

	dc.FillSolidRect(
		&rcClient,
		g_PaintManager->GetMenuFrameFillColor()
		);

	g_PaintManager->PaintMenuBorder(
		dc,
		rcClient,
		this
		);

	ASSERT( !m_bExpandAvailable );
	ASSERT( !m_bScrollingAvailable );

int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nMenuShadowSize = _GetPopupShadowSize();

//	if( rcClient.left >= rcRealClient.left )
//	{
//		CRect rcExcludePart(rcRealClient);
//		rcExcludePart.right =
//			rcClient.left + nMenuBorderSize;
//		dc.ExcludeClipRect( rcExcludePart );
//	}
//	if( rcClient.right <= rcRealClient.right )
//	{
//		CRect rcExcludePart(rcRealClient);
//		rcExcludePart.left =
//			rcClient.right - nMenuBorderSize;
//		dc.ExcludeClipRect( rcExcludePart );
//	}

	if( rcClient.top >= rcRealClient.top )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.bottom =
			rcClient.top + nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}
	if( rcClient.bottom <= rcRealClient.bottom )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.top =
			rcClient.bottom - nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}

	// paint default color btn
	if( m_bEnableBtnColorDefault )
	{
		bool bSelected =
			(m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN) ?
			true : false;
		CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
			this,
			true,
			m_rcDefColorText,
			m_sBtnTextColorDefault,
			NULL,
			true,
			bSelected,
			false, // bSelected,
			false,
			true,
			bSelected,
			false,
			false,
			CExtPaintManager::__ALIGN_HORIZ_CENTER
				|CExtPaintManager::__ALIGN_VERT,
			NULL,
			false,
			0,
			!bSelected
			);
		g_PaintManager->PaintPushButton( dc, _ppbd );
		CRect rcItem( m_rcDefColorText );
		rcItem.OffsetRect(
			0,
			rcItem.Height() + __EXT_MENU_GAP
			);
		rcItem.bottom =
			rcItem.top + __EXT_MENU_SEPARATOR_HEIGHT;
//		g_PaintManager->PaintMenuSeparator(
//			dc,
//			rcItem,
//			false,
//			this
//			);
		rcItem.top += rcItem.Height()/2;
		rcItem.bottom = rcItem.top+1;
		g_PaintManager->PaintSeparator(
			dc,
			rcItem,
			false,
			false,
			this
			);
	}
	if( m_bEnableBtnColorCustom )
	{
		bool bSelected =
			(m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN) ?
			true : false;
		CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
			this,
			true,
			m_rcCustColorText,
			m_sBtnTextColorCustom,
			NULL,
			true,
			bSelected,
			false, // bSelected,
			false,
			true,
			bSelected,
			false,
			false,
			CExtPaintManager::__ALIGN_HORIZ_CENTER
				|CExtPaintManager::__ALIGN_VERT,
			NULL,
			false,
			0,
			!bSelected
			);
		g_PaintManager->PaintPushButton( dc, _ppbd );
		CRect rcItem( m_rcCustColorText );
		rcItem.OffsetRect(
			0,
			- __EXT_MENU_GAP
			);
		rcItem.bottom = rcItem.top;
		rcItem.top -= __EXT_MENU_SEPARATOR_HEIGHT;
//		g_PaintManager->PaintMenuSeparator(
//			dc,
//			rcItem,
//			false,
//			this
//			);
		rcItem.top += rcItem.Height()/2;
		rcItem.bottom = rcItem.top+1;
		g_PaintManager->PaintSeparator(
			dc,
			rcItem,
			false,
			false,
			this
			);
	}

	// paint color buttons
COLORREF clrColorBorder =
		g_PaintManager->GetColor(COLOR_3DDKSHADOW);
	for( int nIdx = 0; nIdx < __NCLR_COUNT; nIdx++ )
	{
		bool bSelected =
			(m_nColorIdxCurr == nIdx
			|| m_nInitialColorIdx == nIdx
			) ?
			true : false;
		CRect rcItem = _GetColorItemRect( nIdx );
		CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
			this,
			true,
			rcItem,
			_T(""),
			NULL,
			true,
			bSelected,
			bSelected,
			false,
			true,
			bSelected,
			false,
			false,
			CExtPaintManager::__ALIGN_HORIZ_CENTER
				|CExtPaintManager::__ALIGN_VERT,
			NULL,
			false,
			0,
			!bSelected
			);
		g_PaintManager->PaintPushButton( dc, _ppbd );
		rcItem.DeflateRect(
			__NCLR_BOX_DX_SPACE,
			__NCLR_BOX_DY_SPACE
			);
		dc.FillSolidRect(
			&rcItem,
			//dc.GetNearestColor(
				g_colors[nIdx].m_clr
				//) 
			);
		dc.Draw3dRect(
			&rcItem,
			clrColorBorder,clrColorBorder
			);
	}

	if( m_nLeftAreaWidth > 0 )
	{
		CRect rcLeftArea( rcClient );
		rcLeftArea.right = rcLeftArea.left + m_nLeftAreaWidth;
		rcLeftArea.OffsetRect( nMenuBorderSize, 0 );
		rcLeftArea.DeflateRect( 0, nMenuBorderSize );
		DRAWLEFTAREADATA _DrawLeftAreaData( &dc, &rcLeftArea, this );
		_DrawLeftAreaData.DoOwnerDrawPainting();
	}

CPoint point( 0, 0 );
	::GetCursorPos( &point );
	ScreenToClient( &point );
	dc.SelectClipRgn( NULL );

	if( m_bCombineWithEA )
	{
		ASSERT( m_eCombineAlign != __CMBA_NONE );
		dc.SelectClipRgn(NULL);
		CRect rcExcludeClient( m_rcExcludeArea );
		ScreenToClient( &rcExcludeClient );
		g_PaintManager->PaintMenuCombinedArea(
			dc,
			rcExcludeClient,
			rcClient,
			m_eCombineAlign,
			this
			);
		if( m_pCbPaintCombinedContent != NULL )
			m_pCbPaintCombinedContent(
				m_pCbPaintCombinedCookie,
				dc,
				*this,
				m_rcExcludeArea,
				m_eCombineAlign
				);
	} // if( m_bCombineWithEA )

	dc.SelectObject( pOldFont );

	if( bUseBackBuffer )
	{
		ASSERT( mdc.GetSafeHdc() != NULL );
		mdc.__Flush();
	}

	if( _IsPopupWithShadows()
		&& m_bAnimFinished
		&& nMenuShadowSize > 0
		)
	{ // if we need to paint shadow for client area (and combined exclude area)
		dcPaint.SelectClipRgn(NULL);
		CRect
			rcExcludeClient( 0,0,0,0 ),
			rcExcludeScreen( 0,0,0,0 ),
			rcBaseScreen( 0,0,0,0 );
		if( m_bCombineWithEA )
		{
			if( _IsPopupWithShadows()
				&& m_bAnimFinished
				&& nMenuShadowSize > 0
				)
			{ // if we need to paint shadow for combined exclude area
				rcExcludeScreen = m_rcExcludeArea;
				rcExcludeClient = rcExcludeScreen;
				ScreenToClient( &rcExcludeClient );
				dcPaint.ExcludeClipRect( &rcClient );
				rcBaseScreen = rcClient;
				ClientToScreen( &rcBaseScreen );
				VERIFY(
					m_ShadowCMBA.Paint(
						dcPaint,
						rcExcludeClient,
						rcExcludeScreen,
						rcBaseScreen,
						nMenuShadowSize
						)
					);
				dcPaint.SelectClipRgn(NULL);
			} // if we need to paint shadow for combined exclude area

			CRect rcExcludePart( m_rcExcludeArea );
			ScreenToClient( &rcExcludePart );
			dcPaint.ExcludeClipRect( &rcExcludePart );
		} // if( m_bCombineWithEA )
		VERIFY(
			m_ShadowMain.Paint(
				dcPaint,
				rcClient,
				rcBaseScreen,
				rcExcludeScreen,
				nMenuShadowSize
				)
			);
	} // if we need to paint shadow for client area (and combined exclude area)

	if( pOldPalette != NULL )
		dcPaint.SelectPalette( pOldPalette, FALSE );

}

int CExtPopupColorMenuWnd::_ColorItemHitTest(
	const CPoint & point
	)
{
	ASSERT_VALID( this );

	for( int nIdx = 0; nIdx < __NCLR_COUNT; nIdx++ )
	{
		CRect rcItem = _GetColorItemRect( nIdx );
		if( rcItem.PtInRect(point) )
			return nIdx;
	}
	if( m_rcDefColorText.PtInRect(point) )
		return IDX_DEFAULT_COLOR_BTN;
	if( m_rcCustColorText.PtInRect(point) )
		return IDX_CUSTOM_COLOR_BTN;
	return -1;
}

bool CExtPopupColorMenuWnd::_CanStartLevelTracking()
{
	ASSERT_VALID( this );
	if( _FindHelpMode() )
		return false;
	if( _FindCustomizeMode() )
		return false;
	return true;
}

bool CExtPopupColorMenuWnd::_OnMouseWheel(
	WPARAM wParam,
	LPARAM lParam,
	bool & bNoEat
	)
{
	ASSERT_VALID( this );
	wParam;
	lParam;
	bNoEat;
	return true;
}

bool CExtPopupColorMenuWnd::_OnMouseMove(
	UINT nFlags,
	CPoint point,
	bool & bNoEat
	)
{
	ASSERT_VALID( this );

	if( GetSafeHwnd() == NULL )
		return false;

	if( GetSite().GetAnimated() != NULL )
		return true;

CPoint ptScreenClick( point );
	ClientToScreen( &ptScreenClick );
HWND hWndFromPoint = ::WindowFromPoint( ptScreenClick );
	if(		hWndFromPoint != NULL
		&&	(::GetWindowLong(hWndFromPoint,GWL_STYLE)&WS_CHILD) != NULL
		&&	::GetParent(hWndFromPoint) == m_hWnd
		)
	{
		return false; // inplace edit
	}


//	ASSERT( GetCapture() == this );

/*
CRect rectWindow;
	GetWindowRect( &rectWindow );
	ScreenToClient( &rectWindow );
	if( !rectWindow.PtInRect(point) )
*/
	if( !_PtInWndArea(point) )
	{
//		_ItemFocusCancel( TRUE );
		if( m_pWndParentMenu != NULL
			&& m_pWndParentMenu->GetSafeHwnd() != NULL
			)
		{
			ASSERT_VALID( m_pWndParentMenu );
			ClientToScreen( &point );
			m_pWndParentMenu->ScreenToClient( &point );
			if( m_pWndParentMenu->_OnMouseMove(
					nFlags,
					point,
					bNoEat
					)
				)
			{
				if( bNoEat )
					return false;
				_OnCancelMode();
				return true;
			}
		}
		return false;
	}

bool bHoverChanged = false;
int nColorIdxCurr = _ColorItemHitTest(point);
	if( m_nColorIdxCurr != nColorIdxCurr )
	{
		m_nColorIdxCurr = nColorIdxCurr;
		bHoverChanged = true;
		Invalidate();
		if( m_nColorIdxCurr != IDX_CUSTOM_COLOR_BTN )
			_NotifyColorChanged();
	}

	if( m_nColorIdxCurr >= 0 )
	{
		if(	g_bMenuShowCoolTips
			&& bHoverChanged
			)
		{
			CExtSafeString sTipText =
				g_colors[m_nColorIdxCurr].m_sName;
			if( !sTipText.IsEmpty()
				&& GetSite().GetCapture() == this
				&& ( GetSite().GetAnimated() == NULL )
				)
			{
				CRect rcItem =
					_GetColorItemRect(m_nColorIdxCurr);
				ClientToScreen( &rcItem );
				m_wndToolTip.SetText( sTipText );
				VERIFY(
					m_wndToolTip.Show(
						this,
						rcItem
						)
					);
			}
		} // if( nOldCurIndex != nCurIndex )
	}
	else
		_CoolTipHide();

	return true;
}

bool CExtPopupColorMenuWnd::_OnMouseClick(
	UINT nFlags,
	CPoint point,
	bool & bNoEat
	)
{
	ASSERT_VALID( this );
	bNoEat;

	if( GetSafeHwnd() == NULL )
		return false;

	if( GetSite().GetAnimated() != NULL )
		return true;

bool bLButtonUpCall =
		(nFlags==WM_LBUTTONUP || nFlags==WM_NCLBUTTONUP)
			? true : false;

	if(		m_eCombineAlign != __CMBA_NONE
		&&	m_bTopLevel
		)
	{
		CRect rcExcludeAreaTest( m_rcExcludeArea );
		ScreenToClient( &rcExcludeAreaTest );
		if( rcExcludeAreaTest.PtInRect( point ) )
		{
			if( !bLButtonUpCall )
			{
				_OnCancelMode();
				return true;
			}
			return false;
		} // if( rcExcludeAreaTest.PtInRect( point ) )
	} // if( m_eCombineAlign != __CMBA_NONE ...
	if( !bLButtonUpCall )
		return false;

	nFlags;
int m_nColorIdxCurr = _ColorItemHitTest(point);
	if(	(		m_nColorIdxCurr >= 0
			||	m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN
			||	m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN
			)
		&&	( ! _FindCustomizeMode() )
		)
	{
		_NotifyColorChanged( true );
		g_SoundPlayer->PlaySound(
			CExtSoundPlayer::__ON_MENU_CMD_CLICKED
			);
	}
	_EndSequence();
	return true;
}

bool CExtPopupColorMenuWnd::_OnKeyDown(
	UINT nChar,
	UINT nRepCnt,
	UINT nFlags,
	bool & bNoEat
	)
{
	ASSERT_VALID( this );

	nRepCnt;
	nFlags;
	bNoEat;
	if( GetSafeHwnd() == NULL )
		return true; //false;

	if( GetSite().GetAnimated() != NULL )
		return true;

bool bEat = false;
//int nColorIdxCurr = m_nColorIdxCurr;
	switch( nChar )
	{
	case VK_RETURN:
	{

//		if( m_pWndParentMenu == NULL )
//			return true; // false;
//		int nParentCurIndex =
//			m_pWndParentMenu->_GetCurIndex();
//		ASSERT(
//			nParentCurIndex >= 0
//			&&
//			nParentCurIndex <=
//				m_pWndParentMenu->ItemGetCount()
//			);
//		m_pWndParentMenu->_ItemFocusCancel(
//			FALSE
//			);
//		HWND hWndThis = GetSafeHwnd();
//		CExtPopupMenuWnd::PassMsgLoop();
//		if( !::IsWindow(hWndThis) )
//			return false;
//		m_pWndParentMenu->_ItemFocusSet(
//			nParentCurIndex,
//			FALSE,
//			TRUE
//			);

		if(	(	m_nColorIdxCurr >= 0
				|| m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN
				|| m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN
				)
			&&	( ! _FindCustomizeMode() )
			)
		{
			_NotifyColorChanged( true );
			g_SoundPlayer->PlaySound(
				CExtSoundPlayer::__ON_MENU_CMD_CLICKED
				);
		}
		_EndSequence();
		return true;
	} // VK_RETURN
	
	case VK_MENU:
	case VK_ESCAPE:
		_EndSequence();
		return true;

	case VK_RIGHT:
		bEat = true;
		if( m_nColorIdxCurr < 0 )
			m_nColorIdxCurr = 0;
		else
		{
			CPoint ptCoord =
				_GetColorItemCoord(m_nColorIdxCurr);
			ptCoord.x++;
			if( ptCoord.x >= __NCLR_DX )
				ptCoord.x = 0;
			m_nColorIdxCurr = 
				ptCoord.x + ptCoord.y * __NCLR_DX;
		}
		break;
	case VK_LEFT:
		bEat = true;
		if( m_nColorIdxCurr < 0 )
			m_nColorIdxCurr = 0;
		else
		{
			CPoint ptCoord =
				_GetColorItemCoord(m_nColorIdxCurr);
			ptCoord.x--;
			if( ptCoord.x < 0 )
				ptCoord.x = __NCLR_DX - 1;
			m_nColorIdxCurr = 
				ptCoord.x + ptCoord.y * __NCLR_DX;
		}
		break;
	case VK_DOWN:
		bEat = true;
		if( m_nColorIdxCurr < 0 )
			m_nColorIdxCurr = 0;
		else
		{
			CPoint ptCoord =
				_GetColorItemCoord(m_nColorIdxCurr);
			ptCoord.y++;
			if( ptCoord.y >= __NCLR_DY )
				ptCoord.y = 0;
			m_nColorIdxCurr = 
				ptCoord.x + ptCoord.y * __NCLR_DX;
		}
		break;
	case VK_UP:
		bEat = true;
		if( m_nColorIdxCurr < 0 )
			m_nColorIdxCurr = 0;
		else
		{
			CPoint ptCoord =
				_GetColorItemCoord(m_nColorIdxCurr);
			ptCoord.y--;
			if( ptCoord.y < 0 )
				ptCoord.y = __NCLR_DY - 1;
			m_nColorIdxCurr = 
				ptCoord.x + ptCoord.y * __NCLR_DX;
		}
		break;

	} // switch( nChar )

	if( bEat )
	{
		_CoolTipHide();
		Invalidate();
		_NotifyColorChanged();
	}

	return bEat;
}

HWND CExtPopupColorMenuWnd::_GetWndNotifyColorChanged()
{
	ASSERT_VALID( this );
	if( m_hWndNotifyColorChanged != NULL )
	{
		ASSERT( ::IsWindow(m_hWndNotifyColorChanged) );
		return m_hWndNotifyColorChanged;
	}
	ASSERT( m_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(m_hWndCmdReciever) );
	return m_hWndCmdReciever;
}

void CExtPopupColorMenuWnd::_NotifyColorChanged(
	bool bFinal // = false
	)
{
	ASSERT_VALID( this );

HWND hWndNotify = _GetWndNotifyColorChanged();
	ASSERT( hWndNotify != NULL );
	ASSERT( ::IsWindow(hWndNotify) );

	if( m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN )
	{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		if( m_pNode != NULL )
		{
			ASSERT_VALID( m_pNode );
			CExtCustomizeSite * pSite = _FindCustomizeSite();
			if(		pSite != NULL
				&&	pSite->OnColorItemCustom( m_pNode )
				)
				return;
		} // if( m_pNode != NULL )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		::PostMessage(
			hWndNotify,
			g_nMsgNotifyCustColor,
			0,
			m_lParamCookie
			);
		return;
	}

COLORREF clr = RGB(0,0,0);
	if( m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN )
		clr = m_clrDefault;
	else
	{
		if( m_nColorIdxCurr < 0 )
			return;
		clr = g_colors[m_nColorIdxCurr].m_clr;
	}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( m_pNode != NULL )
	{
		ASSERT_VALID( m_pNode );
		CExtCustomizeSite * pSite = _FindCustomizeSite();
		if(		pSite != NULL
			&&	pSite->OnColorItemChanged(
					NULL,
					m_pNode,
					bFinal,
					clr,
					m_lParamCookie
					)
			)
			return;
	} // if( m_pNode != NULL )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	::PostMessage(
		hWndNotify,
		bFinal
			? g_nMsgNotifyColorChangedFinally
			: g_nMsgNotifyColorChanged
			,
		(WPARAM)clr,
		m_lParamCookie
		);
}

int CExtPopupColorMenuWnd::_FindCellByColorRef(COLORREF clr)
{
	ASSERT_VALID( this );

	for( int nIdx = 0; nIdx < __NCLR_COUNT; nIdx++ )
	{
		if( g_colors[nIdx].m_clr == clr )
			return nIdx;
	}
	return -1;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
// CExtCustomizeSite::ICustomizeDropTarget
DROPEFFECT CExtPopupColorMenuWnd::OnCustomizeTargetOver(
	CExtCustomizeSite::CCmdDragInfo & _dragInfo,
	CPoint point,
	DWORD dwKeyState
	)
{
	ASSERT_VALID( this );
	ASSERT( !_dragInfo.IsEmpty() );
	_dragInfo;
	point;
	dwKeyState;
	return DROPEFFECT_NONE;
}
void CExtPopupColorMenuWnd::OnCustomizeTargetLeave()
{
	ASSERT_VALID( this );
}
bool CExtPopupColorMenuWnd::OnCustomizeTargetDrop(
	CExtCustomizeSite::CCmdDragInfo & _dragInfo,
	CPoint point,
	DROPEFFECT de
	)
{
	ASSERT_VALID( this );
	ASSERT( !_dragInfo.IsEmpty() );;
	_dragInfo;
	point;
	de;
	return false;
}
// CExtCustomizeSite::ICustomizeDropSource
void CExtPopupColorMenuWnd::OnCustomizeSourceDragComplete(
	DROPEFFECT de,
	bool bCanceled,
	bool * p_bNoResetActiveItem
	)
{
	ASSERT_VALID( this );
	ASSERT( p_bNoResetActiveItem != NULL );
	de;
	bCanceled;
	p_bNoResetActiveItem;
}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuWnd::CInPlaceEditWnd

UINT CExtPopupMenuWnd::CInPlaceEditWnd::g_nMsgDeleteSelection =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::CInPlaceEditWnd::g_nMsgDeleteSelection")
		);
UINT CExtPopupMenuWnd::CInPlaceEditWnd::g_nMsgSelectAll =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::CInPlaceEditWnd::g_nMsgSelectAll")
		);

CExtPopupMenuWnd::CInPlaceEditWnd::CInPlaceEditWnd(
	CExtSafeString * pStr,
	CExtPopupMenuWnd::pCbVerifyTextInput pCbVerify, // = NULL
	CExtPopupMenuWnd::pCbPutTextInputResult pCbResult, // = NULL
	CExtPopupMenuWnd::pCbInplaceEditWndProc pCbWndProc, // = NULL
	LPVOID pCbCoockie // = NULL
	)
	: m_pStr( pStr )
	, m_pCbVerifyTextInput( pCbVerify )
	, m_pCbPutTextInputResult( pCbResult )
	, m_pCbWndProc( pCbWndProc )
	, m_pCbCoockie( pCbCoockie )
	, m_bNoCancelAtKillFocus( false )
{
	ASSERT( m_pStr != NULL );
}

CExtPopupMenuWnd::CInPlaceEditWnd::~CInPlaceEditWnd()
{
}

#ifdef _DEBUG
void CExtPopupMenuWnd::CInPlaceEditWnd::AssertValid() const
{
	CEdit::AssertValid();
	ASSERT( m_pStr != NULL );
}
#endif // _DEBUG

bool CExtPopupMenuWnd::CInPlaceEditWnd::Create(
	CExtPopupMenuWnd * pPopup,
	CRect rc,
	UINT nDlgCtrlID
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pPopup );
	ASSERT( pPopup->GetSafeHwnd() != NULL && ::IsWindow(pPopup->GetSafeHwnd()) );
	m_bNoCancelAtKillFocus = false;
	m_sTextSaved = *m_pStr;
	if( ! CEdit::Create(
			WS_CHILD //|WS_VISIBLE
				|ES_LEFT|ES_AUTOHSCROLL,
			rc,
			pPopup,
			nDlgCtrlID
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	SetFont( &g_PaintManager->m_FontNormal );
	SetWindowText( *m_pStr );
	SetSel( 0, 0 );
	return true;
}

LRESULT CExtPopupMenuWnd::CInPlaceEditWnd::WindowProc(
	UINT message,
	WPARAM wParam,
	LPARAM lParam
	)
{
	if( m_pCbWndProc != NULL )
	{
		LRESULT lResult = 0L;
		if( m_pCbWndProc(
				lResult,
				message,
				wParam,
				lParam,
				*this,
				m_pCbCoockie
				)
			)
			return lResult;
	} // if( m_pCbWndProc != NULL )
	if( message == WM_NCCALCSIZE )
	{
		NCCALCSIZE_PARAMS * pNCCSP =
			reinterpret_cast < NCCALCSIZE_PARAMS * > ( lParam );
		ASSERT( pNCCSP != NULL );
		CRect rcInBarWnd( pNCCSP->rgrc[0] );
		rcInBarWnd.DeflateRect( 2, 2, 0, 2 );
		::CopyRect( &(pNCCSP->rgrc[0]), rcInBarWnd );
		return 0;
	} // if( message == WM_NCCALCSIZE )
	if( message == WM_NCPAINT )
	{
		CRect rcInBarWnd, rcInBarClient;
		GetWindowRect( &rcInBarWnd );
		GetClientRect( &rcInBarClient );
		ClientToScreen( &rcInBarClient );
		if( rcInBarWnd == rcInBarClient )
			return 0;
		CPoint ptDevOffset = -rcInBarWnd.TopLeft();
		rcInBarWnd.OffsetRect( ptDevOffset );
		rcInBarClient.OffsetRect( ptDevOffset );
		CWindowDC dc( this );
		ASSERT( dc.GetSafeHdc() != NULL );
		dc.ExcludeClipRect( &rcInBarClient );
		dc.FillSolidRect(
			rcInBarWnd,
			g_PaintManager->GetColor( COLOR_WINDOW )
			);
		return 0;
	} // if( message == WM_NCPAINT )
	if( message == WM_GETDLGCODE )
		return DLGC_WANTALLKEYS|DLGC_WANTCHARS|DLGC_WANTTAB;
	if(		message == WM_RBUTTONDOWN
		||	message == WM_RBUTTONUP
		||	message == WM_RBUTTONDBLCLK
		||	message == WM_CONTEXTMENU
		)
		return 0;
	
	if( message == WM_KEYDOWN )
	{
		ASSERT( m_pStr != NULL );

		if( int(wParam) == VK_ESCAPE || int(wParam) == VK_MENU )
		{
//			if( m_pCbVerifyTextInput != NULL )
//			{
//				if(	m_pCbVerifyTextInput(
//						*this,
//						m_pCbCoockie,
//						m_sTextSaved.IsEmpty() ? _T("") : ((LPCTSTR)m_sTextSaved),
//						m_sTextSaved.IsEmpty() ? _T("") : ((LPCTSTR)m_sTextSaved)
//						)
//					)
//				{
//					if( m_pCbPutTextInputResult != NULL )
//						m_pCbPutTextInputResult(
//							*this,
//							m_pCbCoockie,
//							m_sTextSaved.IsEmpty() ? _T("") : ((LPCTSTR)m_sTextSaved)
//							);
//					else
//						*m_pStr = m_sTextSaved;
//				}
//			} // if( m_pCbVerifyTextInput != NULL )
//			else
//			{
//					if( m_pCbPutTextInputResult != NULL )
//						m_pCbPutTextInputResult(
//							*this,
//							m_pCbCoockie,
//							m_sTextSaved.IsEmpty() ? _T("") : ((LPCTSTR)m_sTextSaved)
//							);
//					else
//						*m_pStr = m_sTextSaved;
//			} // else from if( m_pCbVerifyTextInput != NULL )
			if( int(wParam) == VK_MENU )
			{
				GetPopupMenu()->PostMessage( WM_CANCELMODE );
				return 0;
			}
			CExtPopupMenuWnd * pPopup = GetPopupMenu();
			int nCurIndex = pPopup->_GetCurIndex();
			ASSERT(
					0 <= nCurIndex
				&&	nCurIndex < pPopup->ItemGetCount()
				);
			pPopup->SetFocus();
			pPopup->_ItemFocusCancel( FALSE );
			pPopup->_ItemFocusSet( nCurIndex, FALSE, TRUE );
			return 0;
		} // if( int(wParam) == VK_ESCAPE || int(wParam) == VK_MENU )

		if( int(wParam) == VK_RETURN )
		{
			ASSERT( m_pStr != NULL );
			m_bNoCancelAtKillFocus = true;
			CString sText;
			GetWindowText( sText );
			if( m_pCbVerifyTextInput != NULL )
			{
				if(	m_pCbVerifyTextInput(
						*this,
						m_pCbCoockie,
						sText.IsEmpty() ? _T("") : ((LPCTSTR)sText),
						sText.IsEmpty() ? _T("") : ((LPCTSTR)sText)
						)
					)
				{
					if( m_pCbPutTextInputResult != NULL )
						m_pCbPutTextInputResult(
							*this,
							m_pCbCoockie,
							sText.IsEmpty() ? _T("") : ((LPCTSTR)sText)
							);
					else
						*m_pStr = sText;
				}
			} // if( m_pCbVerifyTextInput != NULL )
			else
			{
					if( m_pCbPutTextInputResult != NULL )
						m_pCbPutTextInputResult(
							*this,
							m_pCbCoockie,
							sText.IsEmpty() ? _T("") : ((LPCTSTR)sText)
							);
					else
						*m_pStr = sText;
			} // else from if( m_pCbVerifyTextInput != NULL )
			CExtPopupMenuWnd * pPopup = GetPopupMenu();
			int nCurIndex = pPopup->_GetCurIndex();
			ASSERT(
					0 <= nCurIndex
				&&	nCurIndex < pPopup->ItemGetCount()
				);
			pPopup->SetFocus();
			pPopup->_ItemFocusCancel( FALSE );
			pPopup->_ItemFocusSet( nCurIndex, FALSE, TRUE );
			return 0;
		} // if( int(wParam) == VK_RETURN )
		bool bAlt =
			( (::GetAsyncKeyState(VK_MENU)&0x8000) != 0 )
				? true : false;
		if( bAlt )
		{
			GetPopupMenu()->PostMessage( WM_CANCELMODE );
			return 0;
		}
		
		ASSERT( m_pStr != NULL );
		CString sTextOld;
		GetWindowText( sTextOld );
		DWORD dwSelSaved = CEdit::GetSel();
		CEdit::SetRedraw( FALSE );
		LRESULT lResult = CEdit::WindowProc( message, wParam, lParam );
		CString sTextNew;
		GetWindowText( sTextNew );
		if( m_pCbVerifyTextInput != NULL )
		{
			if(	m_pCbVerifyTextInput(
					*this,
					m_pCbCoockie,
					sTextOld.IsEmpty() ? _T("") : ((LPCTSTR)sTextOld),
					sTextNew.IsEmpty() ? _T("") : ((LPCTSTR)sTextNew)
					)
				)
			{
				//*m_pStr = sTextNew;
			}
			else
			{
				CEdit::SetSel( 0, -1 );
				CEdit::ReplaceSel( sTextOld );
				CEdit::SetSel( dwSelSaved );
			}
		} // if( m_pCbVerifyTextInput != NULL )
		//else
			//*m_pStr = sTextNew;
		CEdit::SetRedraw( TRUE );
		Invalidate();
		UpdateWindow();
		
		return lResult;
	} // if( message == WM_KEYDOWN )
	else if( message == WM_CHAR )
	{
		ASSERT( m_pStr != NULL );
		CString sTextOld;
		GetWindowText( sTextOld );
		DWORD dwSelSaved = CEdit::GetSel();
		CEdit::SetRedraw( FALSE );
		LRESULT lResult = CEdit::WindowProc( message, wParam, lParam );
		CString sTextNew;
		GetWindowText( sTextNew );
		if( m_pCbVerifyTextInput != NULL )
		{
			if(	m_pCbVerifyTextInput(
					*this,
					m_pCbCoockie,
					sTextOld.IsEmpty() ? _T("") : ((LPCTSTR)sTextOld),
					sTextNew.IsEmpty() ? _T("") : ((LPCTSTR)sTextNew)
					)
				)
			{
				//*m_pStr = sTextNew;
			}
			else
			{
				CEdit::SetSel( 0, -1 );
				CEdit::ReplaceSel( sTextOld );
				CEdit::SetSel( dwSelSaved );
			}
		} // if( m_pCbVerifyTextInput != NULL )
		//else
			//*m_pStr = sTextNew;
		CEdit::SetRedraw( TRUE );
		Invalidate();
		UpdateWindow();

		return lResult;
	} // else if( message == WM_CHAR )
	else if( message == CExtPopupMenuWnd::CInPlaceEditWnd::g_nMsgDeleteSelection )
	{
		int nChrStart = 0, nChrEnd = 0;
		CEdit::GetSel( nChrStart, nChrEnd );
		if( nChrStart != nChrEnd )
			CEdit::ReplaceSel( _T("") );
		else
		{
			nChrEnd = nChrStart + 1;
			SetRedraw( FALSE );
			SetSel( nChrStart, nChrEnd );
			CEdit::ReplaceSel( _T("") );
			SetSel( nChrStart, nChrStart );
			SetRedraw( TRUE );
			Invalidate();
		} // else from if( nChrStart != nChrEnd )
		return 0;
	} // else if( message == CExtPopupMenuWnd::CInPlaceEditWnd::g_nMsgDeleteSelection )
	else if( message == CExtPopupMenuWnd::CInPlaceEditWnd::g_nMsgSelectAll )
	{
		CEdit::SetSel( 0, -1 );
		return 0;
	} // else if( message == CExtPopupMenuWnd::CInPlaceEditWnd::g_nMsgSelectAll )
	else if( message == WM_KILLFOCUS )
	{
		ASSERT( m_pStr != NULL );
		ShowWindow( SW_HIDE );
		if( !m_bNoCancelAtKillFocus )
			SetWindowText( *m_pStr );
//		ASSERT( m_pStr != NULL );
//		CString sText;
//		if( m_bNoCancelAtKillFocus )
//			GetWindowText( sText );
//		else
//			sText = (LPCTSTR)m_sTextSaved;
//		if( m_pCbVerifyTextInput != NULL )
//		{
//			if(	m_pCbVerifyTextInput(
//					*this,
//					m_pCbCoockie,
//					sText.IsEmpty() ? _T("") : ((LPCTSTR)sText),
//					sText.IsEmpty() ? _T("") : ((LPCTSTR)sText)
//					)
//				)
//			{
//					if( m_pCbPutTextInputResult != NULL )
//						m_pCbPutTextInputResult(
//							*this,
//							m_pCbCoockie,
//							sText.IsEmpty() ? _T("") : ((LPCTSTR)sText)
//							);
//					else
//						*m_pStr = sText;
//			}
//		} // if( m_pCbVerifyTextInput != NULL )
//		else
//		{
//					if( m_pCbPutTextInputResult != NULL )
//						m_pCbPutTextInputResult(
//							*this,
//							m_pCbCoockie,
//							sText.IsEmpty() ? _T("") : ((LPCTSTR)sText)
//							);
//					else
//						*m_pStr = sText;
//		} // else from if( m_pCbVerifyTextInput != NULL )
	} // else if( message == WM_KILLFOCUS )

	return CEdit::WindowProc( message, wParam, lParam );
}

void CExtPopupMenuWnd::CInPlaceEditWnd::PostNcDestroy()
{
	ASSERT_VALID( this );
	delete this;
}

CExtPopupMenuWnd * CExtPopupMenuWnd::CInPlaceEditWnd::GetPopupMenu()
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL && ::IsWindow(GetSafeHwnd()) );
CWnd * pWndParent = GetParent();
	ASSERT_VALID( pWndParent );
	ASSERT_KINDOF( CExtPopupMenuWnd, pWndParent );
	ASSERT( pWndParent->GetSafeHwnd() != NULL && ::IsWindow(pWndParent->GetSafeHwnd()) );
	return STATIC_DOWNCAST( CExtPopupMenuWnd, pWndParent );
}

