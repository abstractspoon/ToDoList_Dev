// DlgUnits.cpp: implementation of the CDlgUnits class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DlgUnits.h"
#include "winclasses.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgUnits::CDlgUnits(HWND hWnd, BOOL bDlgParent) 
	: m_hWnd(NULL), m_dlgBaseUnits(0)
{
	Initialise(hWnd, bDlgParent);
}

CDlgUnits::CDlgUnits(const CWnd* pWnd, BOOL bDlgParent) 
	: m_hWnd(NULL), m_dlgBaseUnits(0)
{
	Initialise((pWnd ? pWnd->GetSafeHwnd() : NULL), bDlgParent);
}

CDlgUnits::~CDlgUnits()
{
}

void CDlgUnits::Initialise(HWND hWnd, BOOL bDlgParent)
{
	// Figure out what hWnd we are really interested in
	if (hWnd && bDlgParent)
	{
		HWND hwndDlg(hWnd);

		while (hwndDlg && !CWinClasses::IsDialog(hwndDlg))
			hwndDlg = ::GetParent(hwndDlg);
		
		if (hwndDlg)
			hWnd = hwndDlg;
	}

	// Get the dialog units for that window
	if (!hWnd || !CWinClasses::IsDialog(hWnd))
	{
		DWORD dwDLBU = ::GetDialogBaseUnits();
		
		m_dlgBaseUnits.cx = LOWORD(dwDLBU);
		m_dlgBaseUnits.cy = HIWORD(dwDLBU);
	}
	else
	{
		m_hWnd = hWnd;
		m_dlgBaseUnits.cx = m_dlgBaseUnits.cy = 0;
	}
}

int CDlgUnits::ToPixelsX(int x) const
{
	int nUnused;
	ToPixels(x, nUnused);

	return x;
}

int CDlgUnits::ToPixelsY(int y) const
{
	int nUnused;
	ToPixels(nUnused, y);

	return y;
}

int CDlgUnits::FromPixelsX(int x) const
{
	int nUnused;
	FromPixels(x, nUnused);

	return x;
}

int CDlgUnits::FromPixelsY(int y) const
{
	int nUnused;
	FromPixels(nUnused, y);

	return y;
}

void CDlgUnits::ToPixels(long& x, long& y) const
{
	if (m_hWnd)
	{
		CRect rect(0, 0, x, y);
		MapDialogRect(m_hWnd, rect);

		x = rect.right;
		y = rect.bottom;
	}
	else
	{
		x = MulDiv(x, m_dlgBaseUnits.cx, 4);
		y = MulDiv(y, m_dlgBaseUnits.cy, 8);
	}
}

void CDlgUnits::ToPixels(int& x, int& y) const
{
	long lx(x), ly(y);
	ToPixels(lx, ly);

	x = (int)lx;
	y = (int)ly;
}

void CDlgUnits::ToPixels(short& x, short& y) const
{
	long lx(x), ly(y);
	ToPixels(lx, ly);

	x = (short)lx;
	y = (short)ly;
}

void CDlgUnits::ToPixels(POINT& point) const
{
	ToPixels(point.x, point.y);
}

void CDlgUnits::ToPixels(RECT& rect) const
{
	ToPixels(rect.left, rect.top);
	ToPixels(rect.right, rect.bottom);
}

void CDlgUnits::ToPixels(SIZE& size) const
{
	ToPixels(size.cx, size.cy);
}

void CDlgUnits::FromPixels(long& x, long& y) const
{
	if (m_hWnd)
	{
		// this is a bit messy because there is no MapDialogRect() in reverse
		CRect rect(0, 0, 1000, 1000);
		MapDialogRect(m_hWnd, rect);

		x = MulDiv(x, 1000, rect.right);
		y = MulDiv(y, 1000, rect.right);
	}
	else
	{
		x = MulDiv(x, 4, m_dlgBaseUnits.cx);
		y = MulDiv(y, 8, m_dlgBaseUnits.cy);
	}
}

void CDlgUnits::FromPixels(int& x, int& y) const
{
	long lx(x), ly(y);
	FromPixels(lx, ly);

	x = (int)lx;
	y = (int)ly;
}

void CDlgUnits::FromPixels(short& x, short& y) const
{
	long lx(x), ly(y);
	FromPixels(lx, ly);

	x = (short)lx;
	y = (short)ly;
}

void CDlgUnits::FromPixels(POINT& point) const
{
	FromPixels(point.x, point.y);
}

void CDlgUnits::FromPixels(RECT& rect) const
{
	FromPixels(rect.left, rect.top);
	FromPixels(rect.right, rect.bottom);
}

void CDlgUnits::FromPixels(SIZE& size) const
{
	FromPixels(size.cx, size.cy);
}
