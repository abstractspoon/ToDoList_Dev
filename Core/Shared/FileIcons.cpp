// SysImageList.cpp: implementation of the CFileIcons class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileIcons.h"
#include "driveinfo.h"
#include "filemisc.h"
#include "webmisc.h"
#include "misc.h"
#include "graphicsmisc.h"

#include "..\3rdParty\ShellIcons.h"

#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CFileIcons::s_bInitialised = FALSE;

//////////////////////////////////////////////////////////////////////

int CFileIcons::GetImageSize(BOOL bLargeIcons)
{
	return GraphicsMisc::ScaleByDPIFactor(bLargeIcons ? 32 : 16);
}

BOOL CFileIcons::Draw(CDC* pDC, LPCTSTR szFilePath, POINT pt, BOOL bLargeIcon, UINT nStyle, BOOL bFailUnKnown)
{
	HIMAGELIST hIL = NULL;
	int nIndex = -1;
	
	if (!GetFileImage(szFilePath, bLargeIcon, bFailUnKnown, hIL, nIndex))
		return FALSE;

	return ImageList_Draw(hIL, nIndex, *pDC, pt.x, pt.y, nStyle);
}

BOOL CFileIcons::DrawFolder(CDC* pDC, POINT pt, BOOL bLargeIcon)
{
	return ShellIcons::DrawIcon(pDC, ShellIcons::SI_FOLDER_CLOSED, pt, (bLargeIcon != FALSE));
}

BOOL CFileIcons::GetImage(LPCTSTR szFile, BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex)
{
	//ASSERT(s_bInitialised);

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

BOOL CFileIcons::GetFileImage(LPCTSTR szFilePath, BOOL bLargeIcon, BOOL bFailUnKnown, HIMAGELIST& hIL, int& nIndex)
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
				nIndex = GetFolderIndex(bLargeIcon);
			}
			else if (sExt.CompareNoCase(_T(".lnk")) == 0)
			{
				// get icon for item pointed to
				CString sReferencedFile;
				
				if (FileMisc::ResolveShortcut(szFilePath, sReferencedFile))
				{
					// RECURSIVE call to handle remote paths, etc
					VERIFY(GetFileImage(sReferencedFile, bLargeIcon, bFailUnKnown, hIL, nIndex)); 
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

			int nUnknownType = -1;

			if ((nIndex == -1) || (GetUnknownTypeImage(bLargeIcon, hIL, nUnknownType) && (nIndex == nUnknownType)))
			{
				if (bFailUnKnown && !IsPath(szFilePath))
					return FALSE;

				if (sFName.IsEmpty() || sExt.IsEmpty()) 
				{
					// else assume it's a folder unless it looks like a file
					nIndex = GetFolderIndex(bLargeIcon);
				}
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

BOOL CFileIcons::GetUnknownTypeImage(BOOL bLargeIcon, HIMAGELIST& hIL, int& nIndex)
{
	return GetImage(_T(".6553BB15-9369-4227-BCA0-F523A35F1DAB"), bLargeIcon, hIL, nIndex);
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
	//ASSERT(s_bInitialised);

	HIMAGELIST hIL = NULL;
	int nIndex = -1;

	if (GetFileImage(szFilePath, bLargeIcon, FALSE, hIL, nIndex))
		return ImageList_GetIcon(hIL, nIndex, ILD_TRANSPARENT);
	
	return NULL;
}

int CFileIcons::GetIndex(LPCTSTR szFilePath, BOOL bLargeIcon)
{
	//ASSERT(s_bInitialised);

	HIMAGELIST hUnused = NULL;
	int nIndex = -1;

	if (GetFileImage(szFilePath, bLargeIcon, FALSE, hUnused, nIndex))
		return nIndex;

	return -1;
}

int CFileIcons::GetFolderIndex(BOOL bLargeIcon)
{
	//ASSERT(s_bInitialised);

	//static CString sWindows = FileMisc::GetSpecialFolder(CSIDL_WINDOWS);

	SHFILEINFO sfi = { 0 };
	UINT nFlags = (SHGFI_SYSICONINDEX | (bLargeIcon ? SHGFI_ICON : SHGFI_SMALLICON));

#ifdef _DEBUG
	nFlags |= SHGFI_TYPENAME;
#endif

	HIMAGELIST hUnused = (HIMAGELIST)SHGetFileInfo(_T(".."), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), nFlags);

	return sfi.iIcon;
}

HIMAGELIST CFileIcons::GetHImageList(BOOL bLargeIcons) 
{ 
	//ASSERT(s_bInitialised);

	// From <commoncontrols.h>
	// {46EB5926-582E-4017-9FDF-E8998DAA0950}
	static const GUID IID_IIMAGELIST = { 0x46EB5926, 0x582E, 0x4017, { 0x9F, 0xDF, 0xE8, 0x99, 0x8D, 0xAA, 0x09, 0x50 } };

	HIMAGELIST hIL = NULL;

	if (S_OK == SHGetImageList((bLargeIcons ? SHIL_LARGE : SHIL_SMALL), IID_IIMAGELIST, reinterpret_cast<void**>(&hIL)))
		return reinterpret_cast<HIMAGELIST>(hIL);

	ASSERT(0);
	return FALSE; 
}

BOOL CFileIcons::Initialise(BOOL bReInit)
{
	ASSERT(!s_bInitialised || bReInit);

	if (!s_bInitialised || bReInit)
	{
		typedef BOOL(WINAPI* PFNFILEICONINIT)(BOOL fFullInit);

		PFNFILEICONINIT pfnFileIconInit = (PFNFILEICONINIT)GetProcAddress(LoadLibrary(_T("shell32.dll")), (LPCSTR)660);

		if (pfnFileIconInit)
			s_bInitialised = pfnFileIconInit(TRUE);
	}
	
	return s_bInitialised;
}
