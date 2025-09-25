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
				static bool GetHtml(String^% html, String^% sourceUrl); // Converts from UTF8 to Unicode
				static bool GetHtml(Windows::Forms::IDataObject^ obj, String^% html, String^% sourceUrl); // Converts from UTF8 to Unicode
			};
		}
	}
}

