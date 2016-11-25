// Preferences.h: interface for the CPreferences class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPREFERENCES_H__DF763543_F9D5_4C94_BBD9_DF7E6E41B8C2__INCLUDED_)
#define AFX_IPREFERENCES_H__DF763543_F9D5_4C94_BBD9_DF7E6E41B8C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////

class IPreferences
{
public:
	virtual UINT GetProfileInt(LPCWSTR lpszSection, LPCWSTR lpszEntry, int nDefault = 0) const = 0;
	virtual bool WriteProfileInt(LPCWSTR lpszSection, LPCWSTR lpszEntry, int nValue) = 0;

	virtual LPCWSTR GetProfileString(LPCWSTR lpszSection, LPCWSTR lpszEntry, LPCWSTR lpszDefault = NULL) const = 0;
	virtual bool WriteProfileString(LPCWSTR lpszSection, LPCWSTR lpszEntry, LPCWSTR lpszValue) = 0;

	virtual double GetProfileDouble(LPCWSTR lpszSection, LPCWSTR lpszEntry, double dDefault = 0.0) const = 0;
	virtual bool WriteProfileDouble(LPCWSTR lpszSection, LPCWSTR lpszEntry, double dValue) = 0;

	virtual bool DeleteProfileEntry(LPCWSTR lpszSection, LPCWSTR lpszEntry) = 0;
	virtual bool DeleteProfileSection(LPCWSTR lpszSection) = 0;
};

/////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_PREFERENCES_H__DF763543_F9D5_4C94_BBD9_DF7E6E41B8C2__INCLUDED_)
