// FocusWatcher.h: interface for the CFocusWatcher class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOCUSWATCHER_H__8AE280AF_E103_447C_AF22_8AA12C03E9BD__INCLUDED_)
#define AFX_FOCUSWATCHER_H__8AE280AF_E103_447C_AF22_8AA12C03E9BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hookmgr.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

const UINT WM_FW_FOCUSCHANGE = ::RegisterWindowMessage(_T("WM_FW_FOCUSCHANGE")); 

//////////////////////////////////////////////////////////////////////

class CFocusWatcher : protected CHookMgr<CFocusWatcher>  
{
	friend class CHookMgr<CFocusWatcher>;

public:
	virtual ~CFocusWatcher() {}

	static BOOL Initialize(HWND hwndCallback, BOOL bChildrenOnly = FALSE);
	static void RefreshFocus(HWND hwndFocus = NULL);

protected:
	static CMap<HWND, HWND, BOOL, BOOL&> s_mapCallbacks;

protected:		
	static void HandleFocusChange(HWND hwndGotFocus, HWND hwndLostFocus);
	static CFocusWatcher& Instance() { return CHookMgr<CFocusWatcher>::GetInstance(); }

	// Base class override
	virtual BOOL OnCbt(int nCode, WPARAM wParam, LPARAM lParam); 
};

#endif // !defined(AFX_FOCUSWATCHER_H__8AE280AF_E103_447C_AF22_8AA12C03E9BD__INCLUDED_)
