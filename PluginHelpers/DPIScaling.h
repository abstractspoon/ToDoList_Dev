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
				
				static void Scale(System::Windows::Forms::Form^ window);
				static void Scale(System::Windows::Forms::Control::ControlCollection^ items);

			protected:
				static System::Drawing::Rectangle GetBoundingRect(System::Windows::Forms::Control::ControlCollection^ items);
			};
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

