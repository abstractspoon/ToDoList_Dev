// TDCUIExtensionHelper.cpp: implementation of the CTDCUIExtensionHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIExtensionHelper.h"
#include "UIExtensionMgr.h"

#include "..\shared\misc.h"
#include "..\shared\enmenu.h"

#include <afxtempl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

// IUI_SAVETOIMAGE
CUIExtensionAppCmdData::CUIExtensionAppCmdData(LPCWSTR szImagePath)
{
	lstrcpyn(szFilePath, szImagePath, MAX_PATH);
}

// IUI_SORT
// IUI_TOGGLABLESORT
CUIExtensionAppCmdData::CUIExtensionAppCmdData(TDC_ATTRIBUTE nSortCol, BOOL bAscending)
{
	ASSERT((nSortCol == TDCA_NONE) || (bAscending != -1));

	nSortBy = nSortCol;
	bSortAscending = ((bAscending == -1) ? true : (bAscending != FALSE));
}

// IUI_GETNEXTTASK
// IUI_GETNEXTTOPLEVELTASK
// IUI_GETPREVTASK
// IUI_GETPREVTOPLEVELTASK, 
CUIExtensionAppCmdData::CUIExtensionAppCmdData(DWORD taskID)
{
	dwTaskID = taskID;
}

// IUI_MOVETASK
CUIExtensionAppCmdData::CUIExtensionAppCmdData(DWORD dwSelTaskID, 
	DWORD dwParentID, 
	DWORD dwAfterSiblingID)
{
	move.dwSelectedTaskID = dwSelTaskID;
	move.dwParentID = dwParentID;
	move.dwAfterSiblingID = dwAfterSiblingID;
	move.bCopy = false;
}

// IUI_MULTISORT
CUIExtensionAppCmdData::CUIExtensionAppCmdData(const IUIMULTISORT& sortCols)
{
	sort = sortCols;
}

// IUI_SELECTFIRSTTASK
// IUI_SELECTNEXTTASK
// IUI_SELECTNEXTTASKINCLCURRENT
// IUI_SELECTPREVTASK
// IUI_SELECTLASTTASK
CUIExtensionAppCmdData::CUIExtensionAppCmdData(TDC_ATTRIBUTE nAttrib, 
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

CUIExtensionHelper::CUIExtensionHelper(UINT nFirstMenuID, int nMaxNumExtensions) 
	: 
	m_nFirstMenuID(nFirstMenuID),
	m_nMaxNumExtensions(nMaxNumExtensions)
{

}

void CUIExtensionHelper::UpdateExtensionVisibilityState(CMenu* pMenu, const CUIExtensionMgr& mgr, const CStringArray& aTypeIDs) const
{
	// Tick those that are visible
	int nNumExt = min(m_nMaxNumExtensions, mgr.GetNumUIExtensions());
	
	for (int nExt = 0; nExt < nNumExt; nExt++)
	{
		UINT nMenuID = (m_nFirstMenuID + nExt);
		CString sTypeID = mgr.GetUIExtensionTypeID(nExt);
		BOOL bVisible = Misc::Contains(sTypeID, aTypeIDs, FALSE, TRUE);
		
		pMenu->CheckMenuItem(nMenuID, bVisible ? MF_CHECKED : 0);
	}
}

BOOL CUIExtensionHelper::ProcessExtensionVisibilityMenuCmd(UINT nCmdID, const CUIExtensionMgr& mgr, CStringArray& aTypeIDs) const
{
	int nExt = (nCmdID - m_nFirstMenuID);

	if (nExt >= 0 && nExt < m_nMaxNumExtensions)
	{
		CString sTypeID = mgr.GetUIExtensionTypeID(nExt);
		int nFind = Misc::Find(sTypeID, aTypeIDs);
		
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

void CUIExtensionHelper::AddAllExtensionsToMenu(CMenu* pMenu, const CUIExtensionMgr& mgr) const
{
	CStringArray aTypeIDs;
	mgr.GetExtensionTypeIDs(aTypeIDs);

	AddExtensionsToMenu(pMenu, mgr, aTypeIDs);
}

void CUIExtensionHelper::AddExtensionsToMenu(CMenu* pMenu, const CUIExtensionMgr& mgr, const CStringArray& aTypeIDs) const
{
	ASSERT(pMenu);

	// cache position of first visible item because
	// m_nFirstMenuID may not actually be present
	int nPos = -1, nExt;

	for (nExt = 0; (nExt < m_nMaxNumExtensions) && (nPos == -1); nExt++)
		nPos = CEnMenu::FindMenuItem(*pMenu, m_nFirstMenuID + nExt);

	if (nPos == -1)
	{
		nPos = pMenu->GetMenuItemCount();
	}
	else // delete all extension items
	{
		int nExt = m_nMaxNumExtensions;

		while (nExt--)
			pMenu->DeleteMenu(m_nFirstMenuID + nExt, MF_BYCOMMAND);
	}
	
	// re-add
	int nNumExt = min(m_nMaxNumExtensions, mgr.GetNumUIExtensions());
	
	for (nExt = 0; nExt < nNumExt; nExt++)
	{
		CString sTypeID = mgr.GetUIExtensionTypeID(nExt);

		if (Misc::Contains(sTypeID, aTypeIDs, FALSE, TRUE))
		{
			UINT nMenuID = (m_nFirstMenuID + nExt);
			CString sUIExt = mgr.GetUIExtensionMenuText(nExt);

			VERIFY(pMenu->InsertMenu(nPos++, (MF_STRING | MF_BYPOSITION), nMenuID, sUIExt));
		}
	}

}
