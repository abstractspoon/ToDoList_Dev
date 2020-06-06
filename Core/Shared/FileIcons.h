// FileIcons.h: interface for the CFileIcons class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEICONS_H__6280CAC5_7A95_48C8_8BBA_3EB7284234F7__INCLUDED_)
#define AFX_FILEICONS_H__6280CAC5_7A95_48C8_8BBA_3EB7284234F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IconCache.h"

#include <afxtempl.h>

class CFileIcons  
{
public:
	// Call in response to a WM_SETTINGCHANGE message when the SPI_SETNONCLIENTMETRICS flag is set
	static BOOL Reinitialise() { return Initialise(TRUE); }

	static HIMAGELIST GetImageList(BOOL bLargeIcons = FALSE);
	static int GetImageSize(BOOL bLargeIcons = FALSE);
	
	// caller is responsible for clean up
	static HICON ExtractIcon(LPCTSTR szFilePath, BOOL bLargeIcon = FALSE);
	static HICON ExtractUnknownFileTypeIcon(BOOL bLargeIcon = FALSE);
	static BOOL GetImage(LPCTSTR szFilePath, CBitmap& bmp, COLORREF crBkgnd, BOOL bLargeIcon = FALSE);

	static int GetIndex(LPCTSTR szFilePath, BOOL bLargeIcon = FALSE);
	static int GetFolderIndex(BOOL bLargeIcon = FALSE);
	static int GetUnknownFileTypeIndex(BOOL bLargeIcon = FALSE);

	static BOOL Draw(CDC* pDC, LPCTSTR szFilePath, POINT pt, BOOL bLargeIcon = FALSE, UINT nStyle = ILD_TRANSPARENT);
	static BOOL DrawFolder(CDC* pDC, POINT pt, BOOL bLargeIcon = FALSE, UINT nStyle = ILD_TRANSPARENT);

protected:
	static CIconCache s_fallbackBig, s_fallbackSmall;
	
protected:
	static BOOL Initialise(BOOL bReInit = FALSE);
	static BOOL GetFileImage(LPCTSTR szFilePath, BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex);
		
	// raw version that resolves directly to SHGetFileInfo
	static BOOL GetImage(LPCTSTR szFile, BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex);
	static BOOL GetFolderImage(BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex);

	static BOOL GetWebImage(BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex);
	static BOOL GetRemoteFolderImage(BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex);
	
	static BOOL IsPath(LPCTSTR szText);

	static CIconCache& FallbackCache(BOOL bLargeIcon) { return (bLargeIcon ? s_fallbackBig : s_fallbackSmall); }
};

#endif // !defined(AFX_FILEICONS_H__6280CAC5_7A95_48C8_8BBA_3EB7284234F7__INCLUDED_)
