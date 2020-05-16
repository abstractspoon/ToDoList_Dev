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
				static void RemoveBorder(IntPtr hWnd);
				static void AddBorder(IntPtr hWnd);
				static void RemoveClientEdge(IntPtr hWnd);
				static void AddClientEdge(IntPtr hWnd);

				static bool RemoveStyle(IntPtr hWnd, UInt32 nStyle, bool bExStyle);
				static bool AddStyle(IntPtr hWnd, UInt32 nStyle, bool bExStyle);

				static HWND GetHwnd(IntPtr hWnd);

				static int GetHScrollPos(IntPtr hWnd);
				static int GetVScrollPos(IntPtr hWnd);

				static int GetSystemDPI();
				static int GetMouseHoverRectSize();
				static int GetMouseHoverDelay();
				static int GetSystemMetric(int nMetric);

				static String^ GetFaceName(HFONT hFont);
				static int GetPointSize(HFONT hFont);
				static float PointsToEms(int nPointSize);

				static bool SetEditCue(IntPtr hWnd, String^ sCueText);

				static void ActivateApp(IntPtr hWnd);


			protected:
				static void DoFrameChange(IntPtr hWnd);
			};

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
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

