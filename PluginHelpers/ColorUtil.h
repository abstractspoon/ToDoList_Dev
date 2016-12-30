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
			public ref class ColorUtil
			{
			public:
				static Windows::Media::Color LighterMedia(Windows::Media::Color^ color, float amount);
				static Windows::Media::Color DarkerMedia(Windows::Media::Color^ color, float amount);

				static Drawing::Color LighterDrawing(Drawing::Color^ color, float amount);
				static Drawing::Color DarkerDrawing(Drawing::Color^ color, float amount);

				static Windows::Media::Color GetMediaColor(UInt32 rgbColor);
				static Drawing::Color GetDrawingColor(UInt32 rgbColor);
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

