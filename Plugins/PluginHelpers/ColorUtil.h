#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			namespace ColorUtil
			{
				public ref class MediaColor
				{
				public:
					static COLORREF ToRgb(Windows::Media::Color color);

					static Windows::Media::Color ToColor(COLORREF rgbColor);
					static Windows::Media::Color ToColor(COLORREF rgbColor, unsigned char opacity);
					static Windows::Media::Color GetBestTextColor(Windows::Media::Color backColor);
					
					static Windows::Media::Color SetLuminance(Windows::Media::Color color, float luminance);
					static Windows::Media::Color AdjustLighting(Windows::Media::Color color, float amount, bool rgbMethod);

					static float GetLuminance(Windows::Media::Color color);
				};

				public ref class DrawingColor
				{
				public:
					static COLORREF ToRgb(Drawing::Color color);

					static Drawing::Color ToColor(COLORREF rgbColor);
					static Drawing::Color ToColor(COLORREF rgbColor, unsigned char opacity);
					static Drawing::Color ToGray(Drawing::Color color);
					static Drawing::Color GetBestTextColor(Drawing::Color backColor);

					static bool IsGray(Drawing::Color color);

					static Drawing::Color SetLuminance(Drawing::Color color, float luminance);
					static Drawing::Color AdjustLighting(Drawing::Color color, float amount, bool rgbMethod);

					static float GetLuminance(Drawing::Color color);
					static float CalculateColorDifference(Drawing::Color color1, Drawing::Color color2);

					static bool IsTransparent(Drawing::Color color, bool whiteIsTransparent);
					static String^ ToHtml(Drawing::Color color, bool whiteIsTransparent);
					static Drawing::Color FromHtml(String^ color);
					static bool Equals(Drawing::Color color1, Drawing::Color color2);
					static Drawing::Color Copy(Drawing::Color color);
				};

				float GetLuminance(COLORREF rgbColor);
				COLORREF SetLuminance(COLORREF rgbColor, float luminance);
				COLORREF AdjustLighting(COLORREF rgbColor, float amount, bool rgbMethod);
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

