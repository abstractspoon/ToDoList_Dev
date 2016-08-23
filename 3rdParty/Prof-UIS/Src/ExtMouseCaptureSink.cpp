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

#include "stdafx.h"

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

#ifdef __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS
	#if (!defined __EXT_POPUP_MENU_WND_H)
		#include <ExtPopupMenuWnd.h>
	#endif
#endif // __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtMouseCaptureSink

#ifdef __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS
volatile HHOOK CExtMouseCaptureSink::g_hMouseHook = NULL;
volatile HWND CExtMouseCaptureSink::g_hWndCapture = NULL;
CCriticalSection CExtMouseCaptureSink::g_cs;
#endif // __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

CExtMouseCaptureSink::CExtMouseCaptureSink()
{
}

CExtMouseCaptureSink::~CExtMouseCaptureSink()
{
}

HWND CExtMouseCaptureSink::SetCapture( HWND hWnd )
{
#ifdef __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

	if(		hWnd == NULL
		||	( ! ::IsWindow(hWnd) )
		)
	{
		ReleaseCapture();
		return NULL;
	}

HWND hWndOldCapture = NULL;
CSingleLock _slo( &g_cs, TRUE );
	if( g_hMouseHook == NULL )
	{
		ASSERT( g_hWndCapture == NULL );
		g_hMouseHook =
			::SetWindowsHookEx(
				WH_MOUSE,
				_HookMouseProc, 
				0,
				::GetCurrentThreadId()
				);
		ASSERT( g_hMouseHook != NULL );
	}
	else
	{
		ASSERT( g_hWndCapture != NULL );
		hWndOldCapture = g_hWndCapture;
	}

	if( g_hMouseHook != NULL )
		g_hWndCapture = hWnd;
	else
	{
		hWnd = g_hWndCapture = NULL;
	}

	_slo.Unlock();
	
	if( hWndOldCapture != NULL
		&& ::IsWindow( hWndOldCapture )
		)
		::SendMessage( hWndOldCapture, WM_CAPTURECHANGED, 0L, LPARAM(hWnd) );
	///// (debug/preview) ::SendMessage( hWnd, WM_CAPTURECHANGED, 0L, LPARAM(hWnd) );

	return hWnd;

#else // __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

	return ::SetCapture( hWnd );

#endif // else from __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS
}

HWND CExtMouseCaptureSink::GetCapture()
{
#ifdef __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

	return g_hWndCapture;

#else // __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

	return ::GetCapture();

#endif // else from __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS
}

BOOL CExtMouseCaptureSink::ReleaseCapture()
{
#ifdef __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

	if( g_hWndCapture == NULL )
		return TRUE;

HWND hWndOldCapture = NULL;
CSingleLock _slo( &g_cs, TRUE );
	if( g_hMouseHook != NULL )
	{
		ASSERT( g_hWndCapture != NULL );
		::UnhookWindowsHookEx( g_hMouseHook );
		g_hMouseHook = NULL;
		hWndOldCapture = g_hWndCapture;
		g_hWndCapture = NULL;
	}
	else
	{
		ASSERT( g_hWndCapture == NULL );
	}
	_slo.Unlock();
	
	if( hWndOldCapture != NULL
		&& ::IsWindow( hWndOldCapture )
		)
		::SendMessage( hWndOldCapture, WM_CAPTURECHANGED, 0L, 0L );
	
	return TRUE;

#else // __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

	return ::ReleaseCapture();

#endif // else from __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS
}

#ifdef __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

LRESULT CALLBACK CExtMouseCaptureSink::_HookMouseProc(
	int nCode,      // hook code
	WPARAM wParam,  // message identifier
	LPARAM lParam   // mouse coordinates
	)
{

MOUSEHOOKSTRUCT * lpMS = (MOUSEHOOKSTRUCT*)lParam;
	ASSERT( lpMS != NULL );
HWND hWndHook = (HWND)g_hWndCapture;
	if(		lpMS->hwnd != hWndHook
		&&	hWndHook != NULL
		&&	::IsWindow(hWndHook)
		)
	{

		UINT nMsgToSend = WM_NULL;
		
		switch( wParam )
		{
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
				nMsgToSend = wParam;
			break;

		case WM_NCMOUSEMOVE:
			nMsgToSend = WM_MOUSEMOVE;
			break;
		case WM_NCLBUTTONDOWN:
			nMsgToSend = WM_LBUTTONDOWN;
			break;
		case WM_NCLBUTTONUP:
			nMsgToSend = WM_NCLBUTTONUP;
			break;
		case WM_NCLBUTTONDBLCLK:
			nMsgToSend = WM_LBUTTONDBLCLK;
			break;
		case WM_NCRBUTTONDOWN:
			nMsgToSend = WM_RBUTTONDOWN;
			break;
		case WM_NCRBUTTONUP:
			nMsgToSend = WM_RBUTTONUP;
			break;
		case WM_NCRBUTTONDBLCLK:
			nMsgToSend = WM_RBUTTONDBLCLK;
			break;
		case WM_NCMBUTTONDOWN:
			nMsgToSend = WM_MBUTTONDOWN;
			break;
		case WM_NCMBUTTONUP:
			nMsgToSend = WM_MBUTTONUP;
			break;
		case WM_NCMBUTTONDBLCLK:
			nMsgToSend = WM_MBUTTONDBLCLK;
			break;

		} // switch( wParam )

		if( nMsgToSend != WM_NULL )
		{
			WPARAM wParamMsg = 0L;
			if( CExtPopupMenuWnd::IsKeyPressed(VK_LBUTTON) )
				wParamMsg |= MK_LBUTTON;
			if( CExtPopupMenuWnd::IsKeyPressed(VK_RBUTTON) )
				wParamMsg |= MK_RBUTTON;
			if( CExtPopupMenuWnd::IsKeyPressed(VK_MBUTTON) )
				wParamMsg |= MK_MBUTTON;
			if( CExtPopupMenuWnd::IsKeyPressed(VK_SHIFT) )
				wParamMsg |= MK_SHIFT;
			if( CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL) )
				wParamMsg |= MK_CONTROL;

			CPoint pt( lpMS->pt );
			VERIFY( ::ScreenToClient( hWndHook, &pt ) );
			LPARAM lParamMsg = MAKELPARAM( pt.x, pt.y );

			::SendMessage(
				hWndHook,
				nMsgToSend,
				wParamMsg,
				lParamMsg
				);
			return -1; // eat!
		
		} // if( nMsgToSend != WM_NULL )
	} // if( hWndHook != NULL && ::IsWindow(hWndHook) )

	return
		::CallNextHookEx(
			g_hMouseHook,
			nCode,
			wParam,
			lParam
			);
}

#endif // __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

