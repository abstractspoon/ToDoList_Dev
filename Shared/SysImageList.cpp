// SysImageList.cpp: implementation of the CSysImageList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysImageList.h"
#include "driveinfo.h"
#include "filemisc.h"
#include "webmisc.h"
#include "misc.h"

#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMap<CString, LPCTSTR, int, int&> CSysImageList::s_mapIndexCache;

CSysImageList::CSysImageList(BOOL bLargeIcons) : 
	m_bLargeIcons(bLargeIcons), 
	m_nFolderImage(-1), 
	m_nHtmlImage(-1), 
	m_hImageList(NULL),
	m_nRemoteFolderImage(-1),
	m_nUnknownTypeImage(-1)
{
	
}

CSysImageList::~CSysImageList()
{
	
}

BOOL CSysImageList::Initialize()
{
	if (m_hImageList)
		return TRUE;
	
	// set up system image list
	TCHAR szWindows[MAX_PATH+1] = { 0 };
	GetWindowsDirectory(szWindows, MAX_PATH);
	
	SHFILEINFO sfi;
	
	UINT nFlags = SHGFI_SYSICONINDEX | (m_bLargeIcons ? SHGFI_ICON : SHGFI_SMALLICON);
	HIMAGELIST hIL = (HIMAGELIST)SHGetFileInfo(szWindows, 0, &sfi, sizeof(sfi), nFlags);
	
	if (hIL)
	{
		m_hImageList = hIL;
		
		// intialize the stock icons
		m_nFolderImage = sfi.iIcon;
		
		// intialize html and remote folder images on demand
	}
	
	// else
	return (m_hImageList != NULL);
}

BOOL CSysImageList::Draw(CDC* pDC, LPCTSTR szFilePath, POINT pt, UINT nStyle, BOOL bFailUnKnown)
{
	int nImage = GetFileImageIndex(szFilePath, bFailUnKnown);

	return Draw(pDC, nImage, pt, nStyle);
}

BOOL CSysImageList::Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle)
{
	return GetImageList()->Draw(pDC, nImage, pt, nStyle);
}

int CSysImageList::GetImageIndex(LPCTSTR szFile)
{
	if (!m_hImageList && !Initialize() || Misc::IsEmpty(szFile))
		return -1;
	
	SHFILEINFO sfi = { 0 };
	
	UINT nFlags = (SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);
	nFlags |= (m_bLargeIcons ? SHGFI_ICON : SHGFI_SMALLICON);

#ifdef _DEBUG
	nFlags |= SHGFI_TYPENAME;
#endif

	HIMAGELIST hIL = (HIMAGELIST)SHGetFileInfo(szFile, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), nFlags);
	
	if (hIL)
		return sfi.iIcon;
	
	return -1;
}

int CSysImageList::GetFileImageIndex(LPCTSTR szFilePath, BOOL bFailUnKnown)
{
	if (!m_hImageList && !Initialize() || Misc::IsEmpty(szFilePath))
		return -1;
	
	// check index cache unless bFailUnknown is TRUE because the cache
	// will always only contain valid indices
	int nIndex = -1;
	
	if (!bFailUnKnown && s_mapIndexCache.Lookup(szFilePath, nIndex))
		return nIndex;
	
	// test for web protocol
	if (WebMisc::IsURL(szFilePath))
	{
		nIndex = GetWebImage();
	}
	else
	{
		// get the file's name and extension
		CString sFName, sExt;
		FileMisc::SplitPath(szFilePath, NULL, NULL, &sFName, &sExt);
		
		// check if its a folder first if it has no extension
		BOOL bRemotePath = CDriveInfo::IsRemotePath(szFilePath, FALSE);

		if (bRemotePath)
		{
			// do not access the file for any reason. 
			// ie. we need to make an educated guess

			// if no extension assume a folder
			if (sExt.IsEmpty() || sExt == _T("."))
				nIndex = GetRemoteFolderImage();
			else 
			{
				nIndex = GetImageIndex(sExt);

				// if the icon index is invalid or there's no filename
				// then assume it's a folder
				if (nIndex < 0 || sFName.IsEmpty())
					nIndex = GetRemoteFolderImage();
			}
		}
		else // local file so we can do whatever we like ;)
		{
			if (FileMisc::FolderExists(szFilePath))
			{
				nIndex = GetFolderImageIndex();
			}
			else if (sExt.CompareNoCase(_T(".lnk")) == 0)
			{
				// get icon for item pointed to
				CString sReferencedFile = FileMisc::ResolveShortcut(szFilePath);

				if (sReferencedFile.IsEmpty()) // not a 'real' file/folder -> let the shell do the work
					nIndex = GetImageIndex(szFilePath); 
				else
				{
					// RECURSIVE call to handle remote paths, etc
					nIndex = GetFileImageIndex(sReferencedFile); 
				}
			}
			else
			{
 				nIndex = GetImageIndex(szFilePath);

				// can fail with full path, so we then revert to extension only
				if (nIndex < 0)
					nIndex = GetImageIndex(sExt);
			}

			if (nIndex < 0 || nIndex == GetUnknownTypeImage())
			{
				if (bFailUnKnown && !IsPath(szFilePath))
				{
					return -1;
				}
				else if (sFName.IsEmpty() || sExt.IsEmpty()) // assume it's a folder unless it looks like a file
				{
					nIndex = GetFolderImageIndex();
				}
			}
		}
	}
	
	// record for posterity unless icon was not retrieved
	if (nIndex != -1 && nIndex != GetUnknownTypeImage())
	{
		s_mapIndexCache[szFilePath] = nIndex;
	}
	
	return nIndex;
}

int CSysImageList::GetWebImage()
{
	if (m_nHtmlImage == -1)
		m_nHtmlImage = GetImageIndex(_T(".html"));

	return m_nHtmlImage;
}

int CSysImageList::GetLocalFolderImage()
{
	ASSERT(m_nFolderImage != -1);
	return m_nFolderImage;
}

int CSysImageList::GetRemoteFolderImage()
{
	if (m_nRemoteFolderImage == -1)
		m_nRemoteFolderImage = GetImageIndex(_T("\\\\dummy\\."));

	return m_nRemoteFolderImage;
}

int CSysImageList::GetUnknownTypeImage()
{
	if (m_nUnknownTypeImage == -1)
		m_nUnknownTypeImage = GetImageIndex(_T(".6553BB15-9369-4227-BCA0-F523A35F1DAB"));

	return m_nUnknownTypeImage;
}

BOOL CSysImageList::IsPath(LPCTSTR szText)
{
	// check for back slashes
	return ((StrStr(szText, _T("\\\\")) != NULL) ||
			(StrStr(szText, _T(":\\")) != NULL) ||
			(StrStr(szText, _T("\\")) != NULL) ||
			(StrStr(szText, _T(".")) != NULL));
}

int CSysImageList::GetFolderImageIndex() 
{ 
	if (!m_hImageList && !Initialize())
		return -1;
	
	return m_nFolderImage;
}

HICON CSysImageList::ExtractAppIcon()
{
	return ExtractFileIcon(FileMisc::GetAppFilePath());
}

HICON CSysImageList::ExtractFileIcon(LPCTSTR szFilePath)
{
	if (!m_hImageList && !Initialize())
		return NULL;
	
	int nIndex = GetFileImageIndex(szFilePath);
	
	if (nIndex != -1)
		return ImageList_GetIcon(m_hImageList, nIndex, 0);
	
	return NULL;
}

HICON CSysImageList::ExtractFolderIcon()
{
	if (!m_hImageList && !Initialize())
		return NULL;
	
	return ImageList_GetIcon(m_hImageList, m_nFolderImage, 0);
}

const CImageList* CSysImageList::GetImageList() const
{ 
	CImageList* pIL = CImageList::FromHandle(m_hImageList); 
	
	if (pIL)
		return pIL;
	
	// else
	static CImageList il;
	return &il;
}

CImageList* CSysImageList::GetImageList() 
{ 
	if (!m_hImageList)
		Initialize();
	
	CImageList* pIL = CImageList::FromHandle(m_hImageList); 
	
	if (pIL)
		return pIL;
	
	// else backup plan
	static CImageList il;
	return &il;
}

HIMAGELIST CSysImageList::GetHImageList() 
{ 
	if (!m_hImageList)
		Initialize();
	
	return m_hImageList; 
}
