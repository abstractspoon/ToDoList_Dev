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
			public ref class UITheme
			{
			public:
				UITheme(const UITHEME* pTheme);

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
				UITheme();

				UInt32 GetColor(AppColor color);
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

