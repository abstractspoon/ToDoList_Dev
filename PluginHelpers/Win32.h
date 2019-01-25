#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Runtime::InteropServices;

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

				static HWND GetHwnd(IntPtr hWnd);

				static int GetHScrollPos(IntPtr hWnd);
				static int GetVScrollPos(IntPtr hWnd);

				static int GetSystemDPI();
			};

			public ref class DlgUnits
			{
			public:
				DlgUnits(IntPtr hWnd);
				DlgUnits();

				int ToPixelsX(int x);
				int ToPixelsY(int y);
				void ToPixels(int& x, int& y);

			protected:
				HWND m_hWnd;
				int m_DlgBaseUnitsX, m_DlgBaseUnitsY;
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

