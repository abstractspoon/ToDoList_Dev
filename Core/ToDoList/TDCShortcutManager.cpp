// TDCShortcutManager.cpp: implementation of the CTDCShortcutManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCShortcutManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCShortcutManager::CTDCShortcutManager(BOOL bAutoSendCmds) : CShortcutManager(bAutoSendCmds)
{

}

CTDCShortcutManager::~CTDCShortcutManager()
{

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
