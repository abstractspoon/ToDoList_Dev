#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

struct UITHEME;

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
				static System::Drawing::Point Scale(System::Drawing::Point point);
				static System::Drawing::Size Scale(System::Drawing::Size size);
				static System::Drawing::Rectangle Scale(System::Drawing::Rectangle rect);
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

