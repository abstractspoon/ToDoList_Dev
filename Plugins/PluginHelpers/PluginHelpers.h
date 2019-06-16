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

#define MS(x) MarshalledString(x)

			// ----------------------------------------------------------------------------

			public ref class DialogUtils
			{
			public:
				static void SetFont(Windows::Forms::Control^ ctrl, Drawing::Font^ font);
				static void SetFont(Windows::Forms::Control::ControlCollection^ ctrls, Drawing::Font^ font);

				static bool SetEditCue(Windows::Forms::Control^ ctrl, String^ sCueText);
				static bool SetEditCue(Windows::Forms::Control^ parent, String^ childName, String^ sCueText, bool recursiveSearch);

				static Windows::Forms::Control^ Find(Windows::Forms::Control^ parent, String^ childName, bool recursive);

				static void RecalcDropWidth(Windows::Forms::ComboBox^ combo);
			};

			// ----------------------------------------------------------------------------

			public ref class Log
			{
			private:
				static String^ s_sLogPath = nullptr;

			public:
				static void LogText(String^ text);

			};

#define		ASTYPE(obj, objtype) dynamic_cast<objtype^>(obj)
#define		ISTYPE(obj, objtype) (ASTYPE(obj, objtype) != nullptr)

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

