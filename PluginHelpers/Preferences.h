#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

class IPreferences;

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning( push )
#undef GetProfileInt
#undef GetProfileString

////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstractspoon
{
	namespace Tdl
	{
		namespace PluginHelpers
		{
			public ref class Preferences
			{
			public:
				Preferences(IPreferences* pPrefs);
				Preferences(const IPreferences* pPrefs);

				int GetProfileInt(String^ sSection, String^ sEntry, int nDefault);
				bool WriteProfileInt(String^ sSection, String^ sEntry, int nValue);

				String^ GetProfileString(String^ sSection, String^ sEntry, String^ sDefault);
				bool WriteProfileString(String^ sSection, String^ sEntry, String^ sValue);

				double GetProfileDouble(String^ sSection, String^ sEntry, double dDefault);
				bool WriteProfileDouble(String^ sSection, String^ sEntry, double dValue);

			private:
				IPreferences* m_pPrefs;
				const IPreferences* m_pConstPrefs;

			private:
				Preferences();
			};
		}
	}
}
#pragma warning( pop )

