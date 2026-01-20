// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Win32.h"
#include "CommCtrl.h"
#include "PluginHelpers.h"
#include "DPIScaling.h"

#include <Shared\MessageBox.h>
#include <Shared\GraphicsMisc.h>
#include <Shared\Themed.h>
#include <Shared\WinClasses.h>
#include <Shared\wclassdefines.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

static int s_nDPI = 0;

////////////////////////////////////////////////////////////////////////////////////////////////

HWND Win32::GetHwnd(IntPtr hWnd)
{
	return static_cast<HWND>(hWnd.ToPointer());
}

HMENU Win32::GetHMenu(IntPtr hMenu)
{
	return static_cast<HMENU>(hMenu.ToPointer());
}

HFONT Win32::GetHfont(IntPtr hFont)
{
	return static_cast<HFONT>(hFont.ToPointer());
}

HDC Win32::GetHdc(IntPtr hDC)
{
	return static_cast<HDC>(hDC.ToPointer());
}

HBITMAP Win32::GetHBitmap(IntPtr hBM)
{
	return static_cast<HBITMAP>(hBM.ToPointer());
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

void Win32::DoFrameChangeEx(IntPtr hWnd, bool bIncrementWidth)
{
	HWND hwnd = GetHwnd(hWnd);
	int nWidth = 0;

	DWORD dwFlags = (SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | 
					 SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

	if (bIncrementWidth)
	{
		CRect rWnd;
		::GetWindowRect(hwnd, rWnd);

		nWidth = (rWnd.Width() + 1);
		dwFlags &= ~SWP_NOSIZE;
	}

	SetWindowPos(GetHwnd(hWnd), NULL, 0, 0, nWidth, 0, dwFlags);
}

bool Win32::HasStyle(IntPtr hWnd, UInt32 nStyle, bool bExStyle)
{
	int nStyleType = (bExStyle ? GWL_EXSTYLE : GWL_STYLE);
	int nCurStyle = GetWindowLong(GetHwnd(hWnd), nStyleType);

	return ((nStyle & nCurStyle) == nStyle);
}

bool Win32::RemoveStyle(IntPtr hWnd, UInt32 nStyle, bool bExStyle)
{
	if (!HasStyle(hWnd, nStyle, bExStyle))
		return false;

	int nStyleType = (bExStyle ? GWL_EXSTYLE : GWL_STYLE);
	int nCurStyle = GetWindowLong(GetHwnd(hWnd), nStyleType);
	
	nCurStyle &= ~nStyle;
	SetWindowLong(GetHwnd(hWnd), nStyleType, nCurStyle);

	// Sanity check
	return !HasStyle(hWnd, nStyle, bExStyle);
}

bool Win32::AddStyle(IntPtr hWnd, UInt32 nStyle, bool bExStyle)
{
	if (HasStyle(hWnd, nStyle, bExStyle))
		return false;

	int nStyleType = (bExStyle ? GWL_EXSTYLE : GWL_STYLE);
	int nCurStyle = GetWindowLong(GetHwnd(hWnd), nStyleType);

	nCurStyle |= nStyle;
	SetWindowLong(GetHwnd(hWnd), nStyleType, nCurStyle);

	// Sanity check
	return HasStyle(hWnd, nStyle, bExStyle);
}

bool Win32::SetRTLReading(IntPtr hWnd, bool rtl)
{
	if (!::IsWindow(GetHwnd(hWnd)))
		return false;

	Control^ ctrl = Control::FromHandle(hWnd);

	if (ctrl == nullptr)
	{
		// Unwrapped HWND
		if (rtl)
		{
			AddStyle(hWnd, WS_EX_RTLREADING, true);
			RemoveStyle(hWnd, WS_EX_LEFTSCROLLBAR, true);
			RemoveStyle(hWnd, WS_EX_RIGHT, true);

			if (CWinClasses::IsClass(GetHwnd(hWnd), WC_COMBOLBOX))
				DoFrameChangeEx(hWnd, true);
		}
		else
		{
			RemoveStyle(hWnd, WS_EX_RTLREADING, true);
		}
	}
	else if (rtl)
	{
		auto handler = gcnew EventHandler(&RTLChangeEventReceiver::Handler);

		ctrl->RightToLeftChanged += handler;
		ctrl->RightToLeft = RightToLeft::Yes;
		ctrl->RightToLeftChanged -= handler;
	}
	else
	{
		ctrl->RightToLeft = RightToLeft::No;
	}
	
	return true;
}

void Win32::RTLChangeEventReceiver::Handler(Object^ sender, EventArgs^ e)
{
	if (ISTYPE(sender, Control))
	{
		// For consistency with core app
		Win32::RemoveStyle(ASTYPE(sender, Control)->Handle, WS_EX_LEFTSCROLLBAR, true);
		Win32::RemoveStyle(ASTYPE(sender, Control)->Handle, WS_EX_RIGHT, true);
	}
}

bool Win32::SyncRTLReadingWithParent(IntPtr hWnd)
{
	HWND hwndParent = ::GetParent(GetHwnd(hWnd));

	return SetRTLReading(hWnd, HasRTLReading(IntPtr(hWnd)));
}

bool Win32::HasRTLReading(IntPtr hWnd)
{
	return HasStyle(hWnd, WS_EX_RTLREADING, true);
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
	if (s_nDPI == 0)
	{
		HDC	hdc = ::GetDC(NULL);
		s_nDPI = GetDeviceCaps(hdc, LOGPIXELSX);
		::ReleaseDC(NULL, hdc);
	}

	return s_nDPI;
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

	HDC hDC = ::GetDC(NULL);
	int nPPI = GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(NULL, hDC);

	return MulDiv(GetPixelHeight(hFont), 72, nPPI);
}

int Win32::GetPixelHeight(HFONT hFont)
{
	LOGFONT lf = { 0 };
	::GetObject(hFont, sizeof(lf), &lf);

	return abs(lf.lfHeight);
}

int Win32::GetPixelHeight(IntPtr hFont)
{
	return GetPixelHeight(GetHfont(hFont));
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

bool Win32::SetComboBoxCue(IntPtr hWnd, String^ sCueText)
{
	HWND hEdit = ::GetDlgItem(GetHwnd(hWnd), 1001);

	return (0 != ::SendMessage(hEdit, EM_SETCUEBANNER, FALSE, (LPARAM)(LPCWSTR)MS(sCueText)));
}

bool Win32::SetEditMargins(IntPtr hWnd, int nAllMargins)
{
	return SetEditMargins(hWnd, nAllMargins, nAllMargins, nAllMargins, nAllMargins);
}

bool Win32::SetEditMargins(IntPtr hWnd, int nLeft, int nTop, int nRight, int nBottom)
{
	RECT rect = { 0 };
	::SendMessage(GetHwnd(hWnd), EM_GETRECT, 0, (LPARAM)&rect);

	if (::IsRectEmpty(&rect))
		return false;

	rect.left += nLeft;
	rect.top += nTop;
	rect.right -= nRight;
	rect.bottom -= nBottom;

	::SendMessage(GetHwnd(hWnd), EM_SETRECT, 0, (LPARAM)&rect);
	return true;
}

void Win32::ActivateApp(IntPtr hWnd)
{
	HWND hwndApp = GetTopWindow(GetHwnd(hWnd));

	SetForegroundWindow(hwndApp);
}

int Win32::SendMessage(IntPtr hWnd, UInt32 wMsg, UIntPtr wParam, IntPtr lParam)
{
	return ::SendMessage(GetHwnd(hWnd), wMsg, (WPARAM)wParam, (LPARAM)lParam.ToInt32());
}

int Win32::PostMessage(IntPtr hWnd, UInt32 wMsg, UIntPtr wParam, IntPtr lParam)
{
	return ::PostMessage(GetHwnd(hWnd), wMsg, (WPARAM)wParam, (LPARAM)lParam.ToInt32());
}

int Win32::GetWmNotifyCode(IntPtr lParam)
{
	if (lParam == IntPtr::Zero)
		return 0;

	NMHDR* pNMHDR = (NMHDR*)lParam.ToPointer();
	return (int)pNMHDR->code;
}

void Win32::SetArrowCursor()
{
	GraphicsMisc::SetStandardCursor(IDC_ARROW);
}

HICON Win32::LoadHIcon(LPCWSTR szDllPath, UINT nIDIcon, int nSize, bool bScaleByDPI)
{
	HMODULE hMod = LoadLibrary(szDllPath);

	if (bScaleByDPI)
		nSize = DPIScaling::Scale(nSize);

	HICON hIcon = (HICON)::LoadImage(hMod,
									 MAKEINTRESOURCE(nIDIcon),
									 IMAGE_ICON,
									 nSize,
									 nSize,
									 LR_LOADMAP3DCOLORS);

	FreeLibrary(hMod);

	return hIcon;
}

void Win32::EnableExplorerTheming(IntPtr hWnd)
{
	CWnd* pWnd = CWnd::FromHandle(GetHwnd(hWnd));

	CThemed::SetWindowTheme(pWnd, _T("Explorer"));
}

String^ Win32::GetWindowText(IntPtr hWnd)
{
	if (!GetWindowTextLength(GetHwnd(hWnd)))
		return String::Empty;

	CWnd* pWnd = CWnd::FromHandle(GetHwnd(hWnd));

	CString sText;
	pWnd->GetWindowText(sText);

	return gcnew String(sText);
}

void Win32::SetWindowText(IntPtr hWnd, String^ text)
{
	CWnd* pWnd = CWnd::FromHandle(GetHwnd(hWnd));
	pWnd->SetWindowText(MS(text));
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

DialogResult AppMessageBox::Show(String^ instruction, String^ text, MessageBoxButtons buttons)
{
	MarshalledString msText(text), msInstruction(instruction);
	CString sText(msText), sInstruction(instruction);

	return (DialogResult)CMessageBox::AfxShow(sInstruction, sText, (UINT)buttons);
}

////////////////////////////////////////////////////////////////////////////////////////////////
