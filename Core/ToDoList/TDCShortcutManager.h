// TDCShortcutManager.h: interface for the CTDCShortcutManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCSHORTCUTMANAGER_H__297C7AA8_F0E1_468D_962A_C0313FEA16DA__INCLUDED_)
#define AFX_TDCSHORTCUTMANAGER_H__297C7AA8_F0E1_468D_962A_C0313FEA16DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\ShortcutManager.h"

//////////////////////////////////////////////////////////////////////

class CTDCShortcutManager : public CShortcutManager  
{
public:
	CTDCShortcutManager(BOOL bAutoSendCmds = TRUE);
	virtual ~CTDCShortcutManager();

	UINT ProcessMessage(const MSG* pMsg, DWORD* pShortcut) const;

	virtual DWORD GetShortcut(UINT nCmdID) const;

};

#endif // !defined(AFX_TDCSHORTCUTMANAGER_H__297C7AA8_F0E1_468D_962A_C0313FEA16DA__INCLUDED_)
