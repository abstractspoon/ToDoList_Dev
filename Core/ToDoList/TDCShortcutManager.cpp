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

CTDCShortcutManager::CTDCShortcutManager(BOOL bAutoSendCmds) 
	: 
	CShortcutManager(bAutoSendCmds),
	m_bRemindersUseTreeFont(FALSE),
	m_bFindTasksUseTreeFont(FALSE)
{

}

CTDCShortcutManager::~CTDCShortcutManager()
{

}

BOOL CTDCShortcutManager::NeedCustomProcessing() const
{
	return (m_bRemindersUseTreeFont || 
			m_bFindTasksUseTreeFont);
}

UINT CTDCShortcutManager::ProcessMessage(const MSG* pMsg, DWORD* pShortcut) const
{
	UINT nCmdID = CShortcutManager::ProcessMessage(pMsg, pShortcut);

	if (!nCmdID && NeedCustomProcessing() && WantProcessMessage(pMsg))
	{
		// Special cases not restricted to having the focus on the main window
		HWND hwndParent = CDialogHelper::GetParentOwner(pMsg->hwnd);

		if (hwndParent)
		{
			if ((m_bFindTasksUseTreeFont && CWinClasses::IsKindOf(hwndParent, RUNTIME_CLASS(CTDLFindTasksDlg))) ||
				(m_bRemindersUseTreeFont && CWinClasses::IsKindOf(hwndParent, RUNTIME_CLASS(CTDLShowReminderDlg))))
			{
				nCmdID = CShortcutManager::ProcessKeyDown(pMsg, hwndParent, ID_VIEW_INCREMENTTASKVIEWFONTSIZE, pShortcut);

				if (nCmdID == 0)
					nCmdID = CShortcutManager::ProcessKeyDown(pMsg, hwndParent, ID_VIEW_DECREMENTTASKVIEWFONTSIZE, pShortcut);

				if (nCmdID)
					return nCmdID;
			}
		}
	}

	return nCmdID;
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
