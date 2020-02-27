// SysImageList.h: interface for the CFileIcons class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEICONS_H__6280CAC5_7A95_48C8_8BBA_3EB7284234F7__INCLUDED_)
#define AFX_FILEICONS_H__6280CAC5_7A95_48C8_8BBA_3EB7284234F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CFileIcons  
{
public:
	// Call once only per process
	static BOOL Initialise(BOOL bReInit = FALSE); 

	static HIMAGELIST GetHImageList(BOOL bLargeIcons = FALSE);
	static int GetImageSize(BOOL bLargeIcons = FALSE);
	
	// caller is responsible for clean up
	static HICON ExtractIcon(LPCTSTR szFilePath, BOOL bLargeIcon = FALSE);

	static int GetIndex(LPCTSTR szFilePath, BOOL bLargeIcon = FALSE);
	static int GetFolderIndex(BOOL bLargeIcon = FALSE);

	static BOOL Draw(CDC* pDC, LPCTSTR szFilePath, POINT pt, BOOL bLargeIcon = FALSE, UINT nStyle = ILD_TRANSPARENT, BOOL bFailUnKnown = FALSE);
	static BOOL DrawFolder(CDC* pDC, POINT pt, BOOL bLargeIcon = FALSE);
	
protected:
	static BOOL s_bInitialised;

protected:
	static BOOL GetFileImage(LPCTSTR szFilePath, BOOL bLargeIcon, BOOL bFailUnKnown, HIMAGELIST& hIL, int& nIndex);
		
	// raw version that resolves directly to SHGetFileInfo
	static BOOL GetImage(LPCTSTR szFile, BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex);

	static BOOL GetWebImage(BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex);
	static BOOL GetRemoteFolderImage(BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex);
	static BOOL GetUnknownTypeImage(BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex);
	
	static BOOL IsPath(LPCTSTR szText);
};

#endif // !defined(AFX_FILEICONS_H__6280CAC5_7A95_48C8_8BBA_3EB7284234F7__INCLUDED_)
