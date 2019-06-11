#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

#include "Toolbars.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;
using namespace System::Drawing;

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
				UITheme();
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
					ToolbarHot,
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

				UInt32 GetColor(AppColor color);
				void InitTheme(const UITHEME* pTheme);
			};

			// --------------------------------------------------------------

			public ref class UIThemeToolbarRenderer : BaseToolbarRenderer
			{
			public:
				UIThemeToolbarRenderer();

				void SetUITheme(UITheme^ theme);

			protected:
				virtual void OnRenderButtonBackground(ToolStripItemRenderEventArgs^ e) override;
				virtual void OnRenderMenuItemBackground(ToolStripItemRenderEventArgs^ e) override;
				virtual void OnRenderDropDownButtonBackground(ToolStripItemRenderEventArgs^ e) override;
				virtual void DrawRowBackground(Drawing::Graphics^ g, Drawing::Rectangle^ rowRect, bool firstRow, bool lastRow) override;

			private:
				Color^ m_HotFillColor;
				Color^ m_HotBorderColor;
				Color^ m_PressedFillColor;
				Color^ m_BkgndLightColor;
				Color^ m_BkgndDarkColor;

				bool ValidColours();
				bool RenderButtonBackground(ToolStripItemRenderEventArgs^ e);
			};

		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

