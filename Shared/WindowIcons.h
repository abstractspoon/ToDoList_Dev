#pragma once

/////////////////////////////////////////////////////////////////////////////

#include "subclass.h"

/////////////////////////////////////////////////////////////////////////////

class CWindowIcons : protected CSubclassWnd
{
public:
	CWindowIcons();
	virtual ~CWindowIcons();

	BOOL Initialise(HWND hWnd, UINT nIconID);
	BOOL IsInitialised() { return IsHooked(); }
	BOOL ModifyIcon(UINT nIconID);

	HICON GetBigIcon() const { return m_hIconBig; }
	HICON GetSmallIcon() const { return m_hIconSmall; }
	
protected:
	HICON m_hIconBig, m_hIconSmall;
	UINT m_nIconID;

protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

/////////////////////////////////////////////////////////////////////////////

