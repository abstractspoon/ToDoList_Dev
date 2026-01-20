#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

struct UITHEME;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class Win32
			{
			public:
				static HDC GetHdc(IntPtr hDC);
				static HWND GetHwnd(IntPtr hWnd);
				static HFONT GetHfont(IntPtr hFont);
				static HMENU GetHMenu(IntPtr hMenu);
				static HBITMAP GetHBitmap(IntPtr hBM);

				static void RemoveBorder(IntPtr hWnd);
				static void AddBorder(IntPtr hWnd);
				static void RemoveClientEdge(IntPtr hWnd);
				static void AddClientEdge(IntPtr hWnd);

				static bool HasStyle(IntPtr hWnd, UInt32 nStyle, bool bExStyle);
				static bool RemoveStyle(IntPtr hWnd, UInt32 nStyle, bool bExStyle);
				static bool AddStyle(IntPtr hWnd, UInt32 nStyle, bool bExStyle);

				static bool SetRTLReading(IntPtr hWnd, bool rtl);
				static bool SyncRTLReadingWithParent(IntPtr hWnd);
				static bool HasRTLReading(IntPtr hWnd);

				static int GetHScrollPos(IntPtr hWnd);
				static int GetVScrollPos(IntPtr hWnd);

				static String^ GetFaceName(HFONT hFont);
				static int GetPointSize(HFONT hFont);
				static int GetPixelHeight(HFONT hFont);
				static int GetPixelHeight(IntPtr hFont);
				static float PointsToEms(int nPointSize);

				static bool SetEditCue(IntPtr hWnd, String^ sCueText);
				static bool SetComboBoxCue(IntPtr hWnd, String^ sCueText);
				static bool SetEditMargins(IntPtr hWnd, int nAllMargins);
				static bool SetEditMargins(IntPtr hWnd, int nLeft, int nTop, int nRight, int nBottom);

				static int SendMessage(IntPtr hWnd, UInt32 wMsg, UIntPtr wParam, IntPtr lParam);
				static int PostMessage(IntPtr hWnd, UInt32 wMsg, UIntPtr wParam, IntPtr lParam);

				// Misc
				static void ActivateApp(IntPtr hWnd);
				static int GetSystemDPI();
				static int GetWmNotifyCode(IntPtr lParam);
				static void SetArrowCursor();
				static HICON LoadHIcon(LPCWSTR szDllPath, UINT nIDIcon, int nSize, bool bScaleByDPI);
				static void EnableExplorerTheming(IntPtr hWnd);
				static String^ GetWindowText(IntPtr hWnd);
				static void SetWindowText(IntPtr hWnd, String^ text);

				// Message cracking
				static int LoWord(int n) { return LOWORD(n); }
				static int LoWord(IntPtr n)	{ return LoWord(n.ToInt32()); }
				static int HiWord(int n) { return HIWORD(n); }
				static int HiWord(IntPtr n)	{ return HiWord(n.ToInt32()); }
				static int MakeLParam(int low, int high) { return MAKELPARAM(low, high); }
				static int MakeWParam(int low, int high) { return MAKEWPARAM(low, high); }

			protected:
				static void DoFrameChange(IntPtr hWnd) { DoFrameChangeEx(hWnd, false); }
				static void DoFrameChangeEx(IntPtr hWnd, bool bIncrementWidth);

				ref class RTLChangeEventReceiver
				{
				public:
					static void Handler(Object^ sender, EventArgs^ e);
				};
			};

			// ---------------------------------------------------------

			public ref class DlgUnits
			{
			public:
				DlgUnits(IntPtr hWnd);

				int ToPixelsX(int x);
				int ToPixelsY(int y);
				void ToPixels(int& x, int& y);

			protected:
				HWND m_hWnd;
			};

			// ---------------------------------------------------------

			public ref class AppMessageBox
			{
			public:
				static Windows::Forms::DialogResult Show(String^ instruction, String^ text, Windows::Forms::MessageBoxButtons buttons);
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

