#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

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
#define		ASTYPE(obj, objtype) dynamic_cast<objtype^>(obj)
#define		ISTYPE(obj, objtype) (ASTYPE(obj, objtype) != nullptr)

			// ----------------------------------------------------------------------------

			// converts String to LPCWSTR, and frees memory on exit
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

#define		MS(x) MarshalledString(x)

			// ----------------------------------------------------------------------------

			public ref class StringUtil
			{
			public:
				static bool FindReplace(String^ source, String^ findText, String^ replaceText,
										bool matchWhole, bool matchCase, String^% result);

				static bool Find(String^ source, String^ findText, bool matchWhole, bool matchCase);
			};

			// ----------------------------------------------------------------------------

			public ref class Log
			{
			private:
				static String^ s_sLogPath = nullptr;

			public:
				static void LogText(String^ text);

			};

			// ----------------------------------------------------------------------------

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

