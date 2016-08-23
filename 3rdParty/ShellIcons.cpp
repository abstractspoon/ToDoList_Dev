
#include "stdafx.h"
#include "ShellIcons.h"

#include <afxtempl.h>

HICON ShellIcons::ExtractIcon(SHELLICON nIndex, bool bLarge)
{
	HICON hIcon = NULL;

	// Shell icons can be customized by the registry:
	// HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Icons
	// "<ShellIconIndex>" = "<Filename>,<IconIndex>"
	// E.g.
	// "3" = "c:\MyFolderIcon.ico,1"
	HKEY hkeyShellIcons;

	if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Icons"), 0, KEY_READ, &hkeyShellIcons) == ERROR_SUCCESS)
	{
		TCHAR szBuffer[ MAX_PATH * sizeof TCHAR];
		DWORD dwSize = MAX_PATH * sizeof TCHAR;

		TCHAR szIndex[6] = {0};
		_stprintf (szIndex, _T("%d"), nIndex);

		if (RegQueryValueEx (hkeyShellIcons, szIndex, NULL, NULL, (LPBYTE)szBuffer, &dwSize) == ERROR_SUCCESS)
		{
			CString strFileName, strIndex;
			VERIFY (AfxExtractSubString (strFileName, szBuffer, 0, _T(',')));
			VERIFY (AfxExtractSubString (strIndex, szBuffer, 1, _T(',')));

			if (bLarge)
				ExtractIconEx(strFileName, _ttoi(strIndex), &hIcon, NULL, 1);
			else
				ExtractIconEx(strFileName, _ttoi(strIndex), NULL, &hIcon, 1);
		}

		RegCloseKey( hkeyShellIcons );
	}

	// Not customized? Then get the original icon from shell23.dll
	if (!hIcon)
	{
		if (bLarge)
			ExtractIconEx(_T("SHELL32.DLL"), nIndex, &hIcon, NULL, 1);
		else
			ExtractIconEx(_T("SHELL32.DLL"), nIndex, NULL, &hIcon, 1);
	}

	return hIcon;
}

BOOL ShellIcons::DrawIcon(CDC* pDC, SHELLICON nIndex, const CPoint& ptTopLeft, bool bLarge)
{
	// maintain a static list of large and small icons
	static CMap<SHELLICON, SHELLICON, HICON, HICON> mapLarge, mapSmall;

	// try lookup first
	HICON hIcon = NULL;

	if (bLarge)
		mapLarge.Lookup(nIndex, hIcon);
	else
		mapSmall.Lookup(nIndex, hIcon);

	// else extract
	if (hIcon == NULL)
		hIcon = ExtractIcon(nIndex, bLarge);

	ASSERT(hIcon);

	if (hIcon)
	{
		int nSize = bLarge ? 32 : 16;

		if (DrawIconEx(*pDC, ptTopLeft.x, ptTopLeft.y, hIcon, nSize, nSize, 0, NULL, DI_NORMAL))
		{
			if (bLarge)
				mapLarge[nIndex] = hIcon;
			else
				mapSmall[nIndex] = hIcon;

			return TRUE;
		}
	}

	// else
	return FALSE;
}
