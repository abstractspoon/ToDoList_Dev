// EnRecentFileList.h: interface for the CEnRecentFileList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENRECENTFILELIST_H__AA4B1746_135C_4E81_96E1_DE6CE758EFDC__INCLUDED_)
#define AFX_ENRECENTFILELIST_H__AA4B1746_135C_4E81_96E1_DE6CE758EFDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "preferences.h"

#include <afxadv.h> // for CRecentFileList


class CEnRecentFileList : public CRecentFileList  
{
public:
	CEnRecentFileList(UINT nStart, LPCTSTR lpszSection, LPCTSTR lpszEntryFormat, 
                     int nSize, int nMaxDispLen = AFX_ABBREV_FILENAME_LEN,
                     LPCTSTR szOriginal = NULL);
	virtual ~CEnRecentFileList();

	void RemoveAll(BOOL bClearProfile = TRUE);
  	virtual void UpdateMenu(CCmdUI* pCmdUI);

	void ReadList(const CPreferences& prefs);
	void WriteList(CPreferences& prefs, BOOL bRelativeToExe = FALSE) const;

};

#endif // !defined(AFX_ENRECENTFILELIST_H__AA4B1746_135C_4E81_96E1_DE6CE758EFDC__INCLUDED_)
