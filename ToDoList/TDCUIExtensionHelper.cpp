// TDCUIExtensionHelper.cpp: implementation of the CTDCUIExtensionHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCUIExtensionHelper.h"
#include "TDCStruct.h"
#include "TDCMapping.h"

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

// IUI_SAVETOIMAGE
CTDCUIExtensionAppCmdData::CTDCUIExtensionAppCmdData(LPCWSTR szImagePath)
{
	lstrcpyn(szFilePath, szImagePath, MAX_PATH);
}

// IUI_SORT
// IUI_TOGGLABLESORT
CTDCUIExtensionAppCmdData::CTDCUIExtensionAppCmdData(IUI_ATTRIBUTE nSortCol, BOOL bAscending)
{
	ASSERT(bAscending != -1);

	nSortBy = nSortCol;
	bSortAscending = (bAscending != FALSE);
}

// IUI_SELECTTASK
// IUI_GETNEXTTASK
// IUI_GETNEXTTOPLEVELTASK
// IUI_GETPREVTASK
// IUI_GETPREVTOPLEVELTASK, 
CTDCUIExtensionAppCmdData::CTDCUIExtensionAppCmdData(DWORD taskID)
{
	dwTaskID = taskID;
}

// IUI_SETTASKFONT
CTDCUIExtensionAppCmdData::CTDCUIExtensionAppCmdData(HFONT font)
{
	hFont = font;
}

// IUI_MOVETASK
CTDCUIExtensionAppCmdData::CTDCUIExtensionAppCmdData(DWORD dwSelTaskID, 
	DWORD dwParentID, 
	DWORD dwAfterSiblingID)
{
	move.dwSelectedTaskID = dwSelTaskID;
	move.dwParentID = dwParentID;
	move.dwAfterSiblingID = dwAfterSiblingID;
	move.bCopy = false;
}

// IUI_MULTISORT
CTDCUIExtensionAppCmdData::CTDCUIExtensionAppCmdData(const TDSORTCOLUMN* pCols)
{
	sort.nAttrib1 = TDC::MapColumnToIUIAttribute(pCols[0].nBy);
	sort.bAscending1 = (pCols[0].bAscending != FALSE);

	sort.nAttrib2 = TDC::MapColumnToIUIAttribute(pCols[1].nBy);
	sort.bAscending2 = (pCols[1].bAscending != FALSE);

	sort.nAttrib3 = TDC::MapColumnToIUIAttribute(pCols[2].nBy);
	sort.bAscending3 = (pCols[2].bAscending != FALSE);
}

// IUI_SELECTFIRSTTASK
// IUI_SELECTNEXTTASK
// IUI_SELECTNEXTTASKINCLCURRENT
// IUI_SELECTPREVTASK
// IUI_SELECTLASTTASK
CTDCUIExtensionAppCmdData::CTDCUIExtensionAppCmdData(IUI_ATTRIBUTE nAttrib, 
	BOOL bFindReplace, 
	LPCWSTR szWords, 
	BOOL bCaseSensitive, 
	BOOL bWholeWord)
{
	select.nAttrib = nAttrib;
	select.bFindReplace = (bFindReplace != FALSE);
	select.szWords = szWords;
	select.bCaseSensitive = (bCaseSensitive != FALSE);
	select.bWholeWord = (bWholeWord != FALSE);
}


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
		BOOL bVisible = Misc::Contains(sTypeID, aTypeIDs, FALSE, FALSE);
		
		pMenu->CheckMenuItem(nMenuID, bVisible ? MF_CHECKED : 0);
	}
}

BOOL CTDCUIExtensionHelper::ProcessViewVisibilityMenuCmd(UINT nCmdID, const CUIExtensionMgr& mgr, CStringArray& aTypeIDs)
{
	int nExt = (nCmdID - ID_SHOWVIEW_UIEXTENSION1);

	if (nExt >= 0 && nExt < 16)
	{
		CString sTypeID = mgr.GetUIExtensionTypeID(nExt);
		int nFind = Misc::Find(sTypeID, aTypeIDs, FALSE, FALSE);
		
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
