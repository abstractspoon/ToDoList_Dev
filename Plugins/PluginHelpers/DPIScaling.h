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
			public ref class DPIScaling
			{
			public:
				static bool WantScaling();

				static int Scale(int nValue);
				static float Scale(float fValue);

				static Drawing::Point Scale(Drawing::Point point);
				static Drawing::Size Scale(Drawing::Size size);
				static Drawing::Rectangle Scale(Drawing::Rectangle rect);

				static void ScaleColumnWidths(Windows::Forms::ListView^ list);

				static int UnScale(int nValue);
				static float UnScale(float fValue);

				static Drawing::Point UnScale(Drawing::Point point);
				static Drawing::Size UnScale(Drawing::Size size);
				static Drawing::Rectangle UnScale(Drawing::Rectangle rect);
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

