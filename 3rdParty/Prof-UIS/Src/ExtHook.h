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

#if (!defined __EXT_HOOK_H)
#define __EXT_HOOK_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

class __PROF_UIS_API CExtHookSink
{
public:

	struct HookSinkArray_t;
	struct HookChains_t;
	friend struct HookChains_t;


	bool m_bEnableDetailedWndHooks;

	typedef
		CList < HWND, HWND >
		HookedWndList_t;

	CExtHookSink(
		bool bEnableDetailedWndHooks = true
		);
	~CExtHookSink();

	virtual bool IsAutoDeleteHookWndSink();

	virtual LRESULT OnHookWndMsgNextProcCurrent(
		WPARAM wParam,
		LPARAM lParam
		);
	virtual LRESULT OnHookWndMsgNextProcInvoke(
		UINT nMessage,
		WPARAM wParam,
		LPARAM lParam
		);
	virtual LRESULT OnHookWndMsgDefault();
	virtual bool OnHookWndMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		UINT nMessage,
		WPARAM & wParam,
		LPARAM & lParam
		);
	virtual bool OnHookCmdMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		WORD wNotifyCode,
		WORD wID,
		HWND hWndCtrl
		);
	virtual bool OnHookNotifyMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		INT nIdCtrl,
		LPNMHDR lpnmhdr
		);
	virtual bool OnHookPaintMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		HDC hDC
		);
	virtual bool OnHookEraseBackgroundMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		HDC hDC
		);
	virtual bool OnHookPrintMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		HDC hDC
		);
	virtual bool OnHookNcPaintMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		HRGN hRgnUpdate
		);

	virtual void OnHookWndNcDestroy();

	virtual void OnHookWndAttach( HWND hWnd );
	virtual void OnHookWndDetach( HWND hWnd );

	virtual bool SetupHookWndSink(
		HWND hWnd,
		bool bRemove = false,
		bool bAddToHead = false
		);

	ULONG SetupHookWndSinkToChilds(
		HWND hWnd,
		UINT * pDlgCtrlIDs = NULL,
		ULONG nCountOfDlgCtrlIDs = 0,
		bool bDeep = false
		);

	virtual void RemoveAllWndHooks();

	void GetHookedWindows( HookedWndList_t & _list );
	bool IsHookedWindow( HWND hWnd );

}; // class CExtHookSink

#endif // __EXT_HOOK_H
