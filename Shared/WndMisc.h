// WndMisc.h: interface for the WndMisc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDMISC_H__710EF397_5AF5_458F_BA5B_A8F8484DE126__INCLUDED_)
#define AFX_WNDMISC_H__710EF397_5AF5_458F_BA5B_A8F8484DE126__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHideWnd  
{
public:
	CHideWnd(HWND hWnd);
	virtual ~CHideWnd();

protected:
	HWND m_hWnd;
};

#endif // !defined(AFX_WNDMISC_H__710EF397_5AF5_458F_BA5B_A8F8484DE126__INCLUDED_)
