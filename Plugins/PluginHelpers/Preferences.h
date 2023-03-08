#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;

////////////////////////////////////////////////////////////////////////////////////////////////

class IPreferences;

////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning( push )
#undef GetProfileInt
#undef GetProfileString
#undef WriteProfileString

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

				bool GetProfileBool(String^ sSection, String^ sEntry, bool bDefault);
				bool WriteProfileBool(String^ sSection, String^ sEntry, bool bValue);

				int GetProfileInt(String^ sSection, String^ sEntry, int nDefault);
				bool WriteProfileInt(String^ sSection, String^ sEntry, int nValue);

				String^ GetProfileString(String^ sSection, String^ sEntry, String^ sDefault);
				bool WriteProfileString(String^ sSection, String^ sEntry, String^ sValue);

				double GetProfileDouble(String^ sSection, String^ sEntry, double dDefault);
				bool WriteProfileDouble(String^ sSection, String^ sEntry, double dValue);

				bool DeleteProfileEntry(String^ sSection, String^ sEntry);
				bool DeleteProfileSection(String^ sSection, bool bIncSubSections);

				bool HasProfileSection(String^ sSection);

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

