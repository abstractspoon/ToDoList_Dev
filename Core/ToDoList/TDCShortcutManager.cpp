// TDCShortcutManager.cpp: implementation of the CTDCShortcutManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCShortcutManager.h"
#include "TDLFindTasksDlg.h"
#include "TDLShowReminderDlg.h"

#include "..\shared\WinClasses.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCShortcutManager::CTDCShortcutManager(BOOL bAutoSendCmds) : CShortcutManager(bAutoSendCmds)
{

}

CTDCShortcutManager::~CTDCShortcutManager()
{

}

UINT CTDCShortcutManager::ProcessMessage(const MSG* pMsg, DWORD* pShortcut) const
{
	UINT nCmdID = CShortcutManager::ProcessMessage(pMsg, pShortcut);

	if (nCmdID > 0)
		return nCmdID;

	// only process accelerators if we are enabled and visible
	if (!IsWindowEnabled() || !IsWindowVisible())
		return 0;

	// we only process keypresses
	if ((pMsg->message != WM_KEYDOWN) && (pMsg->message != WM_SYSKEYDOWN))
		return 0;

	// Special cases not restricted to having the focus on the main window
	CWnd* pFocus = CWnd::FromHandle(pMsg->hwnd);

	if (!pFocus)
		return 0;

	CWnd* pParent = pFocus->GetParentOwner();

	if (!pParent)
		return 0;

	if (CWinClasses::IsKindOf(*pParent, RUNTIME_CLASS(CTDLFindTasksDlg)) ||
		CWinClasses::IsKindOf(*pParent, RUNTIME_CLASS(CTDLShowReminderDlg)))
	{
		nCmdID = CShortcutManager::ProcessKeyDown(pMsg, *pParent, ID_VIEW_INCREMENTTASKVIEWFONTSIZE, pShortcut);

		if (nCmdID)
			return nCmdID;

		nCmdID = CShortcutManager::ProcessKeyDown(pMsg, *pParent, ID_VIEW_DECREMENTTASKVIEWFONTSIZE, pShortcut);

		if (nCmdID)
			return nCmdID;
	}

	return 0;
}

DWORD CTDCShortcutManager::GetShortcut(UINT nCmdID) const
{
	switch (nCmdID)
	{
	case ID_COMMENTS_INSERTDATE:
		nCmdID = ID_EDIT_INSERTDATE;
		break;

	case ID_COMMENTS_INSERTTIME:
		nCmdID = ID_EDIT_INSERTTIME;
		break;

	case ID_COMMENTS_INSERTDATETIME:
		nCmdID = ID_EDIT_INSERTDATETIME;
		break;
	}

	return CShortcutManager::GetShortcut(nCmdID);
}
