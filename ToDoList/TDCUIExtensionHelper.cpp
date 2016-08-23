// TDCUIExtensionHelper.cpp: implementation of the CTDCUIExtensionHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCUIExtensionHelper.h"

#include "..\Shared\UIExtensionMgr.h"
#include "..\Shared\misc.h"
#include "..\Shared\enmenu.h"

#include <afxtempl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CTDCUIExtensionHelper::PrepareViewVisibilityMenu(CMenu* pMenu, const CUIExtensionMgr& mgr, const CStringArray& aTypeIDs)
{
	ASSERT(pMenu);

	// cache position of first item
	int nPos = CEnMenu::GetMenuItemPos(*pMenu, ID_SHOWVIEW_UIEXTENSION1);

	// delete all extension items
	int nExt = 16;

	while (nExt--)
		pMenu->DeleteMenu(ID_SHOWVIEW_UIEXTENSION1 + nExt, MF_BYCOMMAND);
	
	// re-add and tick those that are visible
	int nNumExt = mgr.GetNumUIExtensions();
	
	for (nExt = 0; nExt < nNumExt; nExt++)
	{
		UINT nMenuID = (ID_SHOWVIEW_UIEXTENSION1 + nExt);
		
		// text
		CString sUIExt = mgr.GetUIExtensionMenuText(nExt);
		pMenu->InsertMenu(nPos++, (MF_STRING | MF_BYPOSITION), nMenuID, sUIExt);
		
		// check state
		CString sTypeID = mgr.GetUIExtensionTypeID(nExt);
		BOOL bVisible = (Misc::Find(aTypeIDs, sTypeID, FALSE, FALSE) != -1);
		
		pMenu->CheckMenuItem(nMenuID, bVisible ? MF_CHECKED : 0);
	}
}

BOOL CTDCUIExtensionHelper::ProcessViewVisibilityMenuCmd(UINT nCmdID, const CUIExtensionMgr& mgr, CStringArray& aTypeIDs)
{
	int nExt = (nCmdID - ID_SHOWVIEW_UIEXTENSION1);

	if (nExt >= 0 && nExt < 16)
	{
		CString sTypeID = mgr.GetUIExtensionTypeID(nExt);
		int nFind = Misc::Find(aTypeIDs, sTypeID, FALSE, FALSE);
		
		// toggle visibility
		if (nFind == -1)
			aTypeIDs.Add(sTypeID);
		else
			aTypeIDs.RemoveAt(nFind);
		
		return TRUE;
	}

	// all else
	return FALSE;
}
