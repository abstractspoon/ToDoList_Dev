/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "WindowIcons.h"
#include "graphicsmisc.h"
#include "osversion.h"
#include "regkey.h"

#include <Shlwapi.h.>

/////////////////////////////////////////////////////////////////////////////

CWindowIcons::CWindowIcons() : m_nIconID(0)
{
}


CWindowIcons::~CWindowIcons()
{
}

BOOL CWindowIcons::Initialise(HWND hWnd, UINT nIconID)
{
	if (!IsHooked() && HookWindow(hWnd))
	{
		if (ModifyIcon(nIconID))
			return TRUE;

		// else unhook
		HookWindow(NULL);
	}

	ASSERT(0);
	return FALSE;
}

BOOL CWindowIcons::ModifyIcon(UINT nIconID)
{
	if (!IsHooked())
	{
		ASSERT(0);
		return FALSE;
	}

	// Update visible icons
	int nBigIconSize = ((COSVersion() <= OSV_WIN81) ? 32 : 24);

	CRegKey2 reg;
	DWORD dwSmallIcons = 0;

	if (reg.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced"), TRUE) == ERROR_SUCCESS)
	{
		if ((reg.Read(_T("TaskbarSmallIcons"), dwSmallIcons) == ERROR_SUCCESS) && dwSmallIcons)
			nBigIconSize = 16;
	}

	CIcon iconSmall(GraphicsMisc::LoadIcon(nIconID, 16));
	CIcon iconBig(GraphicsMisc::LoadIcon(nIconID, nBigIconSize));

	if (iconBig.IsValid() && iconSmall.IsValid())
	{
		m_iconBig.SetIcon(iconBig.Detach());
		m_iconSmall.SetIcon(iconSmall.Detach());
		m_nIconID = nIconID;

		SendMessage(WM_SETICON, ICON_SMALL, (LPARAM)(HICON)m_iconSmall);
		SendMessage(WM_SETICON, ICON_BIG, (LPARAM)(HICON)m_iconBig);

		return TRUE;
	}

	return FALSE;
}

LRESULT CWindowIcons::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_SETTINGCHANGE:
		{
			UINT nFlags = wp;
			LPCTSTR szSection = (LPCTSTR)lp;

			if (StrCmp(szSection, _T("TraySettings")) == 0)
			{
				ModifyIcon(m_nIconID);
			}
		}
	}

	return Default();
}
