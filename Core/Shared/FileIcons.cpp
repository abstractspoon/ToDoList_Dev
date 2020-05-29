// FileIcons.cpp: implementation of the CFileIcons class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileIcons.h"
#include "driveinfo.h"
#include "filemisc.h"
#include "webmisc.h"
#include "misc.h"
#include "graphicsmisc.h"
#include "enbitmap.h"

#include "..\3rdParty\ShellIcons.h"

#include <shlwapi.h>
#include <shellapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CFileIcons::GetImageSize(BOOL bLargeIcons)
{
	return GraphicsMisc::ScaleByDPIFactor(bLargeIcons ? 32 : 16);
}

BOOL CFileIcons::Draw(CDC* pDC, LPCTSTR szFilePath, POINT pt, BOOL bLargeIcon, UINT nStyle)
{
	HIMAGELIST hIL = NULL;
	int nIndex = -1;
	
	if (!GetFileImage(szFilePath, bLargeIcon, hIL, nIndex))
		return FALSE;

	return ImageList_Draw(hIL, nIndex, *pDC, pt.x, pt.y, nStyle);
}

BOOL CFileIcons::DrawFolder(CDC* pDC, POINT pt, BOOL bLargeIcon, UINT nStyle)
{
	HIMAGELIST hIL = NULL;
	int nIndex = -1;

	if (!GetFolderImage(bLargeIcon, hIL, nIndex))
		return FALSE;

	return ImageList_Draw(hIL, nIndex, *pDC, pt.x, pt.y, nStyle);
}

BOOL CFileIcons::GetImage(LPCTSTR szFile, BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex)
{
	Initialise();

	if (Misc::IsEmpty(szFile) || !IsPath(szFile))
		return FALSE;
		
	SHFILEINFO sfi = { 0 };
	
	UINT nFlags = (SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);
	nFlags |= (bLargeIcon ? SHGFI_ICON : SHGFI_SMALLICON);

#ifdef _DEBUG
	nFlags |= SHGFI_TYPENAME;
#endif

	hIL = (HIMAGELIST)SHGetFileInfo(szFile, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), nFlags);
	nIndex = sfi.iIcon;
	
	return (hIL && (nIndex != -1));
}

BOOL CFileIcons::GetFolderImage(BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex)
{
	Initialise();

	SHFILEINFO sfi = { 0 };
	UINT nFlags = (SHGFI_SYSICONINDEX | (bLargeIcon ? SHGFI_ICON : SHGFI_SMALLICON));

	hIL = (HIMAGELIST)SHGetFileInfo(_T(""), FILE_ATTRIBUTE_DIRECTORY, &sfi, sizeof(sfi), nFlags);
	nIndex = sfi.iIcon;

	return (hIL && (nIndex != -1));
}

BOOL CFileIcons::GetFileImage(LPCTSTR szFilePath, BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex)
{
	if (WebMisc::IsURL(szFilePath))
	{
		VERIFY(GetWebImage(bLargeIcon, hIL, nIndex));
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
			{
				VERIFY(GetRemoteFolderImage(bLargeIcon, hIL, nIndex));
			}
			else 
			{
				if (sFName.IsEmpty() || !GetImage(sExt, bLargeIcon, hIL, nIndex))
					VERIFY(GetRemoteFolderImage(bLargeIcon, hIL, nIndex));
			}
		}
		else // local file so we can do whatever we like ;)
		{
			if (FileMisc::FolderExists(szFilePath))
			{
				VERIFY(GetFolderImage(bLargeIcon, hIL, nIndex));
			}
			else if (sExt.CompareNoCase(_T(".lnk")) == 0)
			{
				// get icon for item pointed to
				CString sReferencedFile;
				
				if (FileMisc::ResolveShortcut(szFilePath, sReferencedFile))
				{
					// RECURSIVE call to handle remote paths, etc
					VERIFY(GetFileImage(sReferencedFile, bLargeIcon, hIL, nIndex)); 
				}
				else
				{
					// not a 'real' file/folder -> let the shell do the work
					VERIFY(GetImage(szFilePath, bLargeIcon, hIL, nIndex)); 
				}
			}
			else if (!GetImage(szFilePath, bLargeIcon, hIL, nIndex))
			{
				// can fail with full path, so we then revert to extension only
				VERIFY(GetImage(sExt, bLargeIcon, hIL, nIndex));
			}

			if ((nIndex == -1) && (sFName.IsEmpty() || sExt.IsEmpty()))
			{
				// else assume it's a folder unless it looks like a file
				VERIFY(GetFolderImage(bLargeIcon, hIL, nIndex));
			}
		}
	}
	
	return ((nIndex >= 0) && (hIL != NULL));
}

BOOL CFileIcons::GetWebImage(BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex)
{
	return GetImage(_T(".html"), bLargeIcon, hIL, nIndex);
}

BOOL CFileIcons::GetRemoteFolderImage(BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex)
{
	return GetImage(_T("\\\\dummy\\."), bLargeIcon, hIL, nIndex);
}

BOOL CFileIcons::IsPath(LPCTSTR szText)
{
	// check for back slashes
	return ((StrStr(szText, _T("\\\\")) != NULL) ||
			(StrStr(szText, _T(":\\")) != NULL) ||
			(StrStr(szText, _T("\\")) != NULL) ||
			(StrStr(szText, _T(".")) != NULL));
}

HICON CFileIcons::ExtractIcon(LPCTSTR szFilePath, BOOL bLargeIcon)
{
	HIMAGELIST hIL = NULL;
	int nIndex = -1;

	if (!GetFileImage(szFilePath, bLargeIcon, hIL, nIndex))
		return NULL;

	// else
	return ImageList_GetIcon(hIL, nIndex, ILD_TRANSPARENT);
}

BOOL CFileIcons::GetImage(LPCTSTR szFilePath, CBitmap& bmp, COLORREF crBkgnd, BOOL bLargeIcon)
{
	HIMAGELIST hIL = NULL;
	int nIndex = -1;

	if (!GetFileImage(szFilePath, bLargeIcon, hIL, nIndex))
		return FALSE;

	CEnBitmap ebmp;

	if (!ebmp.CopyImage(hIL, nIndex, crBkgnd))
		return FALSE;

	return bmp.Attach(ebmp.Detach());
}

int CFileIcons::GetIndex(LPCTSTR szFilePath, BOOL bLargeIcon)
{
	HIMAGELIST hUnused = NULL;
	int nIndex = -1;

	if (GetFileImage(szFilePath, bLargeIcon, hUnused, nIndex))
		return nIndex;

	return -1;
}

int CFileIcons::GetFolderIndex(BOOL bLargeIcon)
{
	HIMAGELIST hUnused = NULL;
	int nIndex = -1;

	if (GetFolderImage(bLargeIcon, hUnused, nIndex))
		return nIndex;

	return -1;
}

HIMAGELIST CFileIcons::GetImageList(BOOL bLargeIcons) 
{ 
	Initialise();

	HIMAGELIST hIL = NULL;

#if _MSC_VER <= 1200
	// VC6 compatible version
	int nUnused = -1;

	if (GetFolderImage(bLargeIcons, hIL, nUnused))
		return hIL;
#else
	// From <commoncontrols.h>
	// {46EB5926-582E-4017-9FDF-E8998DAA0950}
	static const GUID IID_IIMAGELIST = { 0x46EB5926, 0x582E, 0x4017, { 0x9F, 0xDF, 0xE8, 0x99, 0x8D, 0xAA, 0x09, 0x50 } };

	if (S_OK == SHGetImageList((bLargeIcons ? SHIL_LARGE : SHIL_SMALL), IID_IIMAGELIST, reinterpret_cast<void**>(&hIL)))
		return reinterpret_cast<HIMAGELIST>(hIL);
#endif

	ASSERT(0);
	return FALSE; 
}

BOOL CFileIcons::Initialise(BOOL bReInit)
{
	static BOOL bInitialised = FALSE;

	if (!bInitialised || bReInit)
	{
		typedef BOOL(WINAPI* PFNFILEICONINIT)(BOOL fFullInit);

		PFNFILEICONINIT pfnFileIconInit = (PFNFILEICONINIT)GetProcAddress(LoadLibrary(_T("shell32.dll")), (LPCSTR)660);

		if (pfnFileIconInit)
			bInitialised = pfnFileIconInit(TRUE);
	}
	
	return bInitialised;
}
