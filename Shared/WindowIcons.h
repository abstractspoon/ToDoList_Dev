#pragma once

/////////////////////////////////////////////////////////////////////////////

#include "subclass.h"
#include "icon.h"

/////////////////////////////////////////////////////////////////////////////

class CWindowIcons : protected CSubclassWnd
{
public:
	CWindowIcons();
	virtual ~CWindowIcons();

	BOOL Initialise(HWND hWnd, UINT nIconID);
	BOOL IsInitialised() { return IsHooked(); }
	BOOL ModifyIcon(UINT nIconID);

	HICON GetBigIcon() const { return m_iconBig; }
	HICON GetSmallIcon() const { return m_iconSmall; }
	
protected:
	CIcon m_iconBig, m_iconSmall;
	UINT m_nIconID;

protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

/////////////////////////////////////////////////////////////////////////////

