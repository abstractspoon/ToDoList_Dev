// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Win32.h"
#include "CommCtrl.h"
#include "PluginHelpers.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

HWND Win32::GetHwnd(IntPtr hWnd)
{
	return static_cast<HWND>(hWnd.ToPointer());
}

void Win32::RemoveClientEdge(IntPtr hWnd)
{
	// remove client edge
	if (RemoveStyle(hWnd, WS_EX_CLIENTEDGE, true))
		DoFrameChange(hWnd);
}

void Win32::AddClientEdge(IntPtr hWnd)
{
	if (AddStyle(hWnd, WS_EX_CLIENTEDGE, true))
		DoFrameChange(hWnd);
}

void Win32::RemoveBorder(IntPtr hWnd)
{
	if (RemoveStyle(hWnd, WS_BORDER, false))
		DoFrameChange(hWnd);
}

void Win32::AddBorder(IntPtr hWnd)
{
	if (AddStyle(hWnd, WS_BORDER, false))
		DoFrameChange(hWnd);
}

void Win32::DoFrameChange(IntPtr hWnd)
{
	SetWindowPos(GetHwnd(hWnd), NULL, 0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
}

bool Win32::RemoveStyle(IntPtr hWnd, UInt32 nStyle, bool bExStyle)
{
	int nStyleType = (bExStyle ? GWL_EXSTYLE : GWL_STYLE);
	int nCurStyle = GetWindowLong(GetHwnd(hWnd), nStyleType);

	if ((nCurStyle & nStyle) == nStyle)
	{
		nCurStyle &= ~nStyle;
		SetWindowLong(GetHwnd(hWnd), nStyleType, nCurStyle);

		return true;
	}

	return false;
}

bool Win32::AddStyle(IntPtr hWnd, UInt32 nStyle, bool bExStyle)
{
	int nStyleType = (bExStyle ? GWL_EXSTYLE : GWL_STYLE);
	int nCurStyle = GetWindowLong(GetHwnd(hWnd), nStyleType);

	if ((nCurStyle & nStyle) == 0)
	{
		nCurStyle |= nStyle;
		SetWindowLong(GetHwnd(hWnd), nStyleType, nCurStyle);

		return true;
	}

	return false;
}

int Win32::GetVScrollPos(IntPtr hWnd)
{
	return ::GetScrollPos(GetHwnd(hWnd), SB_VERT);
}

int Win32::GetHScrollPos(IntPtr hWnd)
{
	return ::GetScrollPos(GetHwnd(hWnd), SB_HORZ);
}

int Win32::GetSystemDPI()
{
	static int nDPI = 0;

	if (nDPI == 0)
	{
		HDC	hdc = ::GetDC(NULL);
		nDPI = GetDeviceCaps(hdc, LOGPIXELSX);
		::ReleaseDC(NULL, hdc);
	}

	return nDPI;
}

int Win32::GetMouseHoverRectSize()
{
	int nSize = 0;

	if (::SystemParametersInfo(SPI_GETMOUSEHOVERWIDTH, 0, &nSize, 0))
		return nSize;

	// else
	return 2;
}

int Win32::GetMouseHoverDelay()
{
	int nDelay = 0;

	if (::SystemParametersInfo(SPI_GETMOUSEHOVERTIME, 0, &nDelay, 0))
		return nDelay;

	// else
	return 500;
}

int Win32::GetSystemMetric(int nMetric)
{
	return ::GetSystemMetrics(nMetric);
}

String^ Win32::GetFaceName(HFONT hFont)
{
	if (!hFont)
		return gcnew String("Tahoma");

	LOGFONT lf = { 0 };
	::GetObject(hFont, sizeof(lf), &lf);

	return gcnew String(lf.lfFaceName);
}

int Win32::GetPointSize(HFONT hFont)
{
	if (!hFont)
		return 8;

	LOGFONT lf = { 0 };
	::GetObject(hFont, sizeof(lf), &lf);

	HDC hDC = ::GetDC(NULL);
	int nPPI = GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(NULL, hDC);

	return MulDiv(abs(lf.lfHeight), 72, nPPI);
}

float Win32::PointsToEms(int nPointSize)
{
	const float PtToEm = 0.08365f; // https://www.convertunits.com/from/pt/to/em

	return (nPointSize * PtToEm);
}

bool Win32::SetEditCue(IntPtr hWnd, String^ sCueText)
{
	return (0 != ::SendMessage(GetHwnd(hWnd), EM_SETCUEBANNER, FALSE, (LPARAM)(LPCWSTR)MS(sCueText)));
}

void Win32::ActivateApp(IntPtr hWnd)
{
	HWND hwndApp = GetTopWindow(GetHwnd(hWnd));

	SetForegroundWindow(hwndApp);
}

////////////////////////////////////////////////////////////////////////////////////////////////

DlgUnits::DlgUnits(IntPtr hWnd) : m_hWnd(Win32::GetHwnd(hWnd))
{
}

int DlgUnits::ToPixelsX(int x)
{
	int unused;
	ToPixels(x, unused);

	return x;
}

int DlgUnits::ToPixelsY(int y)
{
	int unused;
	ToPixels(unused, y);

	return y;
}

void DlgUnits::ToPixels(int& x, int& y)
{
	if (m_hWnd)
	{
		RECT rect = { 0, 0, x, y };
		::MapDialogRect(m_hWnd, &rect);

		x = rect.right;
		y = rect.bottom;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
