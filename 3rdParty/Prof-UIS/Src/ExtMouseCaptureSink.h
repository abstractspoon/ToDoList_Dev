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

#if (!defined __EXT_MOUSECAPTURESINK_H)
#define __EXT_MOUSECAPTURESINK_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

///// (debug/preview) #define __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

#ifdef __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS
	#ifndef __AFXMT_H__
		#include <AfxMT.h>
	#endif
#endif // __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CExtMouseCaptureSink

class __PROF_UIS_API CExtMouseCaptureSink
{
	
#ifdef __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS
	static volatile HHOOK g_hMouseHook;
	static volatile HWND g_hWndCapture;
	static CCriticalSection g_cs;

	static LRESULT CALLBACK _HookMouseProc(
		int nCode,      // hook code
		WPARAM wParam,  // message identifier
		LPARAM lParam   // mouse coordinates
		);
#endif // __EXT_MOUSECAPTURESINK_USES_WIN_HOOKS

public:

	CExtMouseCaptureSink();
	~CExtMouseCaptureSink();

	virtual HWND SetCapture( HWND hWnd );
	static HWND GetCapture();
	static BOOL ReleaseCapture();

}; // class __PROF_UIS_API CExtMouseCaptureSink

#endif // __EXT_MOUSECAPTURESINK_H

