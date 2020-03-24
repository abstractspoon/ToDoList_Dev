
#include "stdafx.h"
#include "ShellIcons.h"

#include <afxtempl.h>

// maintain a static list of large and small icons
typedef CMap<ShellIcons::SHELLICON, ShellIcons::SHELLICON, HICON, HICON> CIconMap;

CIconMap iconsLarge, iconsSmall;

void DeleteIcons(CIconMap& icons)
{
	POSITION pos = icons.GetStartPosition();

	while (pos)
	{
		ShellIcons::SHELLICON key;
		HICON hIcon;

		icons.GetNextAssoc(pos, key, hIcon);
		VERIFY(::DestroyIcon(hIcon));
	}
}

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
#if _MSC_VER >= 1400
		_stprintf_s(szIndex, 6, _T("%d"), nIndex);
#else
		_stprintf(szIndex, _T("%d"), nIndex);
#endif

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

HICON ShellIcons::GetIcon(SHELLICON nIndex, bool bLarge)
{
	// try lookup first
	HICON hIcon = NULL;

	if (bLarge)
		iconsLarge.Lookup(nIndex, hIcon);
	else
		iconsSmall.Lookup(nIndex, hIcon);

	// else extract
	if (hIcon == NULL)
		hIcon = ExtractIcon(nIndex, bLarge);

	ASSERT(hIcon);

	if (hIcon)
	{
		if (bLarge)
			iconsLarge[nIndex] = hIcon;
		else
			iconsSmall[nIndex] = hIcon;
	}

	return hIcon;
}

BOOL ShellIcons::DrawIcon(CDC* pDC, SHELLICON nIndex, const CPoint& ptTopLeft, bool bLarge)
{
	HICON hIcon = GetIcon(nIndex, bLarge); // cached

	if (hIcon == NULL)
		return FALSE;

	// Scale image by DPI
	double dScale = (pDC->GetDeviceCaps(LOGPIXELSY) / 96.0);
	int nSize = (int)((bLarge ? 32 : 16) * dScale);

	return DrawIconEx(*pDC, ptTopLeft.x, ptTopLeft.y, hIcon, nSize, nSize, 0, NULL, DI_NORMAL);
}

void ShellIcons::Release()
{
	// Cleanup cached icons
	DeleteIcons(iconsLarge);
	DeleteIcons(iconsSmall);
}
