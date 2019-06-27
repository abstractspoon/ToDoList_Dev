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
					static Windows::Media::Color GetColor(UInt32 rgbColor);
					static Windows::Media::Color GetBestTextColor(Windows::Media::Color^ backColor);
					
					static Windows::Media::Color SetLuminance(Windows::Media::Color^ color, float luminance);
					static Windows::Media::Color AdjustLighting(Windows::Media::Color^ color, float amount, bool rgbMethod);

					static float GetLuminance(Windows::Media::Color^ color);
				};

				public ref class DrawingColor
				{
				public:
					static Drawing::Color GetColor(UInt32 rgbColor);
					static Drawing::Color GetBestTextColor(Drawing::Color^ backColor);

					static Drawing::Color SetLuminance(Drawing::Color^ color, float luminance);
					static Drawing::Color AdjustLighting(Drawing::Color^ color, float amount, bool rgbMethod);

					static float GetLuminance(Drawing::Color^ color);
				};

				float GetLuminance(UInt32 rgbColor);
				UInt32 SetLuminance(UInt32 rgbColor, float luminance);
				UInt32 AdjustLighting(UInt32 rgbColor, float amount, bool rgbMethod);
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

