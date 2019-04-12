// FocusWatcher.h: interface for the CFocusWatcher class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOCUSWATCHER_H__8AE280AF_E103_447C_AF22_8AA12C03E9BD__INCLUDED_)
#define AFX_FOCUSWATCHER_H__8AE280AF_E103_447C_AF22_8AA12C03E9BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hookmgr.h"

const UINT WM_FW_FOCUSCHANGE = ::RegisterWindowMessage(_T("WM_FW_FOCUSCHANGE")); 

class CFocusWatcher : public CHookMgr<CFocusWatcher>  
{
	friend class CHookMgr<CFocusWatcher>;

public:
	CFocusWatcher();
	virtual ~CFocusWatcher();

	static BOOL Initialize(CWnd* pMainWnd);
	static void Release();
	static void UpdateFocus(CWnd* pFocus = NULL);

protected:
	CWnd* m_pMainWnd;

protected:		
	static CFocusWatcher& Instance() { return CHookMgr<CFocusWatcher>::GetInstance(); }
	virtual BOOL OnCbt(int nCode, WPARAM wParam, LPARAM lParam); 
};

#endif // !defined(AFX_FOCUSWATCHER_H__8AE280AF_E103_447C_AF22_8AA12C03E9BD__INCLUDED_)
