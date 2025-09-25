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
			public ref class ClipboardUtil
			{
			public:
				// Converts from UTF8 to Unicode
				static bool GetHtmlFragment(String^% html); 
				static bool GetHtmlFragment(String^% html, String^% sourceUrl);

				static bool GetHtmlFragment(Windows::Forms::IDataObject^ obj, String^% html);
				static bool GetHtmlFragment(Windows::Forms::IDataObject^ obj, String^% html, String^% sourceUrl);
			};
		}
	}
}

