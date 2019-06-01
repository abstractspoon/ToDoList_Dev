// EditShortcutMgr.h: interface for the CEditShortcutMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITSHORTCUTMGR_H__C595858F_2058_446B_AA9F_AC9F68F7B1D4__INCLUDED_)
#define AFX_EDITSHORTCUTMGR_H__C595858F_2058_446B_AA9F_AC9F68F7B1D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HookMgr.h"

class CEditShortcutMgr : public CHookMgr<CEditShortcutMgr>  
{
   friend class CHookMgr<CEditShortcutMgr>;

public:
	virtual ~CEditShortcutMgr();
 	static BOOL Initialize(DWORD dwSelectAllShortcut = MAKELONG('A', HOTKEYF_CONTROL));
 	static void Release();

protected:
	DWORD m_dwSelectAllShortcut;

protected:
	CEditShortcutMgr();
 	static CEditShortcutMgr& Instance() { return CHookMgr<CEditShortcutMgr>::GetInstance(); }

	virtual BOOL OnKeyboard(UINT uVirtKey, UINT uFlags);
};

#endif // !defined(AFX_EDITSHORTCUTMGR_H__C595858F_2058_446B_AA9F_AC9F68F7B1D4__INCLUDED_)
