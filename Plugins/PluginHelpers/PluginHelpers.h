#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Runtime::InteropServices;

////////////////////////////////////////////////////////////////////////////////////////////////

struct UITHEME;

enum TDC_ATTRIBUTE;

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			// ----------------------------------------------------------------------------
			// converts System::String to LPCWSTR, and frees memory on exit
			// DO NOT USE TO RETURN STRINGS

			public ref class MarshalledString
			{
			public:
				MarshalledString(String^ str);
				~MarshalledString();

				operator LPCWSTR();

			private:
				LPCWSTR m_wszGlobal;
			};

#define MS(x) MarshalledString(x)

			// ----------------------------------------------------------------------------

			public ref class DialogUtils
			{
			public:
				static void SetFont(System::Windows::Forms::Control^ ctrl, System::Drawing::Font^ font);
				static void SetFont(System::Windows::Forms::Control::ControlCollection^ ctrls, System::Drawing::Font^ font);

			};

			// ----------------------------------------------------------------------------

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

