// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Preferences.h"

#include <Interfaces\IPreferences.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

Preferences::Preferences(IPreferences* pPrefs) : m_pPrefs(pPrefs), m_pConstPrefs(nullptr)  
{
} 

Preferences::Preferences(const IPreferences* pPrefs) : m_pPrefs(nullptr), m_pConstPrefs(pPrefs) 
{
} 

// private constructor
Preferences::Preferences() : m_pPrefs(nullptr), m_pConstPrefs(nullptr) 
{

}

// ---------------------------------------------------------

#define GETPREF(fn, defval) \
	(m_pConstPrefs ? m_pConstPrefs->fn(MS(sSection), MS(sEntry), defval) : \
	(m_pPrefs ? m_pPrefs->fn(MS(sSection), MS(sEntry), defval) : defval))

#define SETPREF(fn, val) \
   (m_pPrefs ? (m_pPrefs->fn(MS(sSection), MS(sEntry), val) != FALSE) : false)

// ---------------------------------------------------------

bool Preferences::GetProfileBool(String^ sSection, String^ sEntry, bool bDefault)
{
	return (GetProfileInt(sSection, sEntry, (bDefault ? 1 : 0)) != 0);
}

bool Preferences::WriteProfileBool(String^ sSection, String^ sEntry, bool bValue)
{
	return WriteProfileInt(sSection, sEntry, (bValue ? 1 : 0));
}

int Preferences::GetProfileInt(String^ sSection, String^ sEntry, int nDefault)
{
   return GETPREF(GetProfileInt, nDefault);
}

bool Preferences::WriteProfileInt(String^ sSection, String^ sEntry, int nValue)
{
   return SETPREF(WriteProfileInt, nValue);
}

String^ Preferences::GetProfileString(String^ sSection, String^ sEntry, String^ sDefault)
{
   return gcnew String(GETPREF(GetProfileString, MS(sDefault)));
}

bool Preferences::WriteProfileString(String^ sSection, String^ sEntry, String^ sValue)
{
   return SETPREF(WriteProfileString, MS(sValue));
}

double Preferences::GetProfileDouble(String^ sSection, String^ sEntry, double dDefault)
{
   return GETPREF(GetProfileDouble, dDefault);
}

bool Preferences::WriteProfileDouble(String^ sSection, String^ sEntry, double dValue)
{
   return SETPREF(WriteProfileDouble, dValue);
}

bool Preferences::DeleteProfileEntry(String^ sSection, String^ sEntry)
{
	return (m_pPrefs ? m_pPrefs->DeleteProfileEntry(MS(sSection), MS(sEntry)) : false);
}

bool Preferences::DeleteProfileSection(String^ sSection, bool bIncSubSections)
{
	return (m_pPrefs ? m_pPrefs->DeleteProfileSection(MS(sSection), bIncSubSections) : false);
}

bool Preferences::HasProfileSection(String^ sSection)
{
	return (m_pConstPrefs ? m_pConstPrefs->HasProfileSection(MS(sSection)) : 
			(m_pPrefs ? m_pPrefs->HasProfileSection(MS(sSection)) : false));
}
