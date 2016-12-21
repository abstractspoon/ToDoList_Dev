// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Preferences.h"

#include "..\..\ToDoList_Dev\Interfaces\IPreferences.h"

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

