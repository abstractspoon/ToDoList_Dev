// WndMisc.cpp: implementation of the WndMisc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WndMisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHideWnd::CHideWnd(HWND hWnd) : m_hWnd(NULL)
{
	if (::IsWindowVisible(hWnd))
	{
		m_hWnd = hWnd;
		::ShowWindow(hWnd, SW_HIDE);
	}
	// else already hidden
}

CHideWnd::~CHideWnd()
{
	if (m_hWnd)
		::ShowWindow(m_hWnd, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////
