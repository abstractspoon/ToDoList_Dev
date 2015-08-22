// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TDLPreferences.h"

#include "..\Interfaces\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace TDLPluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

TDLPreferences::TDLPreferences(IPreferences* pPrefs) : m_pPrefs(pPrefs), m_pConstPrefs(nullptr)  
{
} 

TDLPreferences::TDLPreferences(const IPreferences* pPrefs) : m_pPrefs(nullptr), m_pConstPrefs(pPrefs) 
{
} 

// private constructor
TDLPreferences::TDLPreferences() : m_pPrefs(nullptr), m_pConstPrefs(nullptr) 
{

}

// ---------------------------------------------------------

#define GETPREF(fn, defval) \
	(m_pConstPrefs ? m_pConstPrefs->fn(MS(sSection), MS(sEntry), defval) : \
	(m_pPrefs ? m_pPrefs->fn(MS(sSection), MS(sEntry), defval) : defval))

#define SETPREF(fn, val) \
   (m_pPrefs ? (m_pPrefs->fn(MS(sSection), MS(sEntry), val) != FALSE) : false)

// ---------------------------------------------------------

int TDLPreferences::GetProfileInt(String^ sSection, String^ sEntry, int nDefault)
{
   return GETPREF(GetProfileInt, nDefault);
}

bool TDLPreferences::WriteProfileInt(String^ sSection, String^ sEntry, int nValue)
{
   return SETPREF(WriteProfileInt, nValue);
}

String^ TDLPreferences::GetProfileString(String^ sSection, String^ sEntry, String^ sDefault)
{
   return gcnew String(GETPREF(GetProfileString, MS(sDefault)));
}

bool TDLPreferences::WriteProfileString(String^ sSection, String^ sEntry, String^ sValue)
{
   return SETPREF(WriteProfileString, MS(sValue));
}

double TDLPreferences::GetProfileDouble(String^ sSection, String^ sEntry, double dDefault)
{
   return GETPREF(GetProfileDouble, dDefault);
}

bool TDLPreferences::WriteProfileDouble(String^ sSection, String^ sEntry, double dValue)
{
   return SETPREF(WriteProfileDouble, dValue);
}

