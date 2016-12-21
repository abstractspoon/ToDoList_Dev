#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Runtime::InteropServices;

////////////////////////////////////////////////////////////////////////////////////////////////

struct UITHEME;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace TDLPluginHelpers
{
	// converts System::String to LPCWSTR, and frees memory on exit
	// DO NOT USE TO RETURN STRINGS
	public ref class TDLMarshalledString
	{
	public:
		TDLMarshalledString(String^ str);
		~TDLMarshalledString();

		operator LPCWSTR();

	private:
		LPCWSTR m_wszGlobal;
	};

	#define MS(str) TDLMarshalledString(str)

	//--------------------------------

	public ref class TDLColor
	{
	public:
		static Windows::Media::Color LighterMedia(Windows::Media::Color^ color, float amount);
		static Windows::Media::Color DarkerMedia(Windows::Media::Color^ color, float amount);

		static Drawing::Color LighterDrawing(Drawing::Color^ color, float amount);
		static Drawing::Color DarkerDrawing(Drawing::Color^ color, float amount);

		static Windows::Media::Color GetMediaColor(UInt32 rgbColor);
		static Drawing::Color GetDrawingColor(UInt32 rgbColor);
	};

	//--------------------------------

	public ref class TDLTheme
	{
	public:
		TDLTheme(const UITHEME* pTheme);

		enum class RenderStyle 
		{ 
			Gradient, 
			Glass, 
			GlassWithGradient 
		};
		RenderStyle GetRenderStyle();

		enum class AppColor 
		{ 
			AppBackDark, 
			AppBackLight, 
			AppLinesDark, 
			AppLinesLight,
			AppText,
			MenuBack,
			ToolbarDark,
			ToolbarLight,
			StatusBarDark,
			StatusBarLight,
			StatusBarText,
		};
		Windows::Media::Color GetAppMediaColor(AppColor color);
		Drawing::Color GetAppDrawingColor(AppColor color);

		String^ GetToolBarImagePath();
		Windows::Media::Color GetToolbarTransparencyMediaColor();
		Drawing::Color GetToolbarTransparencyDrawingColor();

	private:
		UITHEME* m_pTheme;

	private:
		TDLTheme();

		UInt32 GetColor(AppColor color);
	};

	public ref class TDLWin32
	{
	public:
		static void RemoveBorder(IntPtr hWnd);
		static void AddBorder(IntPtr hWnd);
		static void RemoveClientEdge(IntPtr hWnd);
		static void AddClientEdge(IntPtr hWnd);
		static HWND GetHwnd(IntPtr hWnd);
	};

}

////////////////////////////////////////////////////////////////////////////////////////////////

