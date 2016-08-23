// EnRecentFileList.cpp: implementation of the CEnRecentFileList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnRecentFileList.h"
#include "FileMisc.h"

#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnRecentFileList::CEnRecentFileList(UINT nStart, LPCTSTR lpszSection,
	    LPCTSTR lpszEntryFormat, int nSize, int nMaxDispLen, LPCTSTR szOriginal) 
       : CRecentFileList(nStart, lpszSection, lpszEntryFormat, nSize, nMaxDispLen) 
{
   m_strOriginal = szOriginal;
}

CEnRecentFileList::~CEnRecentFileList()
{

}

void CEnRecentFileList::UpdateMenu(CCmdUI* pCmdUI)
{
	UINT nFirstID = pCmdUI->m_nID;

    if (m_arrNames[0].IsEmpty())
	{
		if (pCmdUI->m_pMenu == NULL)
			return;
		
		// delete all but the first item
		for (int iMRU = 1; iMRU < m_nSize; iMRU++)
			pCmdUI->m_pMenu->DeleteMenu(nFirstID + iMRU, MF_BYCOMMAND);
	}
	
	CRecentFileList::UpdateMenu(pCmdUI);
	
	// fix up default MFC implementation which does not 
	// add the leading '1' for items above 9
	CString sItem;
	
	for (int iMRU = 9; iMRU < (int)pCmdUI->m_nIndexMax; iMRU++)
	{
		UINT nID = nFirstID + iMRU;
		pCmdUI->m_pMenu->GetMenuString(nID, sItem, MF_BYCOMMAND);
		
		if (!sItem.IsEmpty() && (sItem[0] == '&'))
		{
			sItem = _T("1") + sItem;
			
			MENUITEMINFO mii = { sizeof(mii), MIIM_STRING, MIIM_STRING, 0 };
			mii.dwTypeData = (LPTSTR)(LPCTSTR)sItem;
			
			::SetMenuItemInfo(pCmdUI->m_pMenu->GetSafeHmenu(), nID, FALSE, &mii);
		}
	}
}

void CEnRecentFileList::RemoveAll(BOOL bClearProfile)
{
	int nItem = GetSize();

	while (nItem--)
		Remove(nItem);

	if (bClearProfile)
		CRecentFileList::WriteList();
}

void CEnRecentFileList::WriteList(CPreferences& prefs, BOOL bRelativeToExe) const
{
	// always delete the MRU and then rebuild it
	prefs.DeleteProfileSection(_T("MRU"));

	for (int nFile = 0; nFile < GetSize(); nFile++)
	{
		CString sFile(m_arrNames[nFile]);

		if (sFile.IsEmpty())
			break;
		
		CString sItem;
		sItem.Format(m_strEntryFormat, nFile + 1);

		if (bRelativeToExe)
			FileMisc::MakeRelativePath(sFile, FileMisc::GetAppFolder(), FALSE);

		prefs.WriteProfileString(_T("MRU"), sItem, sFile);
	}
}

void CEnRecentFileList::ReadList(const CPreferences& prefs)
{
	for (int nFile = 0; nFile < GetSize(); nFile++)
	{
		CString sItem, sFile;
		sItem.Format(m_strEntryFormat, nFile + 1);
		sFile = prefs.GetProfileString(_T("MRU"), sItem);

		if (!sFile.IsEmpty())
			m_arrNames[nFile] = FileMisc::GetFullPath(sFile, FileMisc::GetAppFolder());
	}
}
