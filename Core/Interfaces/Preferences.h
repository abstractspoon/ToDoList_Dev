// Preferences.h: interface for the CPreferences class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREFERENCES_H__DF763543_F9D5_4C94_BBD9_DF7E6E41B8C2__INCLUDED_)
#define AFX_PREFERENCES_H__DF763543_F9D5_4C94_BBD9_DF7E6E41B8C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\IPreferences.h"

#include <afxtempl.h>
#include <afxmt.h>

/////////////////////////////////////////////////////////////////////////////////

struct INIENTRY
{
	INIENTRY(LPCTSTR szName = NULL, LPCTSTR szValue = NULL, BOOL bQuote = FALSE);

	CString Format() const;
	BOOL Parse(const CString& sEntry);
	BOOL operator==(const INIENTRY& ie) const;
	
	CString sName;
	CString sValue;
	BOOL bQuoted;
};
typedef CMap<CString, LPCTSTR, INIENTRY, INIENTRY&> CIniEntryMap;

/////////////////////////////////////////////////////////////////////////////////

struct INISECTION
{
	INISECTION(LPCTSTR szName = NULL);
	INISECTION(const INISECTION& other);
	
	CString sSection;
	CIniEntryMap aEntries;
};
typedef CMap<CString, LPCTSTR, INISECTION*, INISECTION*&> CIniSectionMap;
typedef CArray<INISECTION*, INISECTION*&> CIniSectionArray; // For sorting

/////////////////////////////////////////////////////////////////////////////////

class CPreferences 
{
public:
	CPreferences(); 
	~CPreferences();

	operator IPreferences*() { return &m_iPrefs; }
	operator const IPreferences*() const { return &m_iPrefs; }
 
	static BOOL Initialise(LPCTSTR szPrefsPath, BOOL bIni);
	static BOOL Save();
	static void Release();

	static BOOL IsInitialised();
	static BOOL UsesIni() { return s_bIni; }
	static BOOL IsEmpty();

	static CString GetPath(BOOL bFriendly = FALSE);

	// directly from CWinApp
	UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault = 0) const;
	BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);
	CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL) const;
	BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);

	template <class T>
	T GetProfileEnum(LPCTSTR lpszSection, LPCTSTR lpszEntry, T nDefault) const
	{
		return (T)GetProfileInt(lpszSection, lpszEntry, nDefault);
	}

	// note: Binary not currently supported by ini file

	// extras
	double GetProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dDefault = 0) const;
	bool WriteProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dValue);
	
	int GetProfileSectionNames(CStringArray& aSections, LPCTSTR szPrefix = NULL);
	BOOL HasProfileSection(LPCTSTR lpszSection) const;
	BOOL DeleteProfileSection(LPCTSTR lpszSection, BOOL bIncSubSections = FALSE);
	BOOL DeleteProfileEntry(LPCTSTR lpszSection, LPCTSTR lpszEntry);

	int GetProfileArray(LPCTSTR lpszSection, CStringArray& aItems, BOOL bAllowEmpty = FALSE) const;
	void WriteProfileArray(LPCTSTR lpszSection, const CStringArray& aItems, BOOL bPreDeleteSection = TRUE);
	int GetProfileArray(LPCTSTR lpszSection, CDWordArray& aItems) const;
	void WriteProfileArray(LPCTSTR lpszSection, const CDWordArray& aItems, BOOL bPreDeleteSection = TRUE);
	
	static int GetProfileArray(const IPreferences* pPrefs, LPCTSTR lpszSection, CStringArray& aItems, BOOL bAllowEmpty = FALSE);
	static void WriteProfileArray(IPreferences* pPrefs, LPCTSTR lpszSection, const CStringArray& aItems, BOOL bPreDeleteSection = TRUE);
	static int GetProfileArray(const IPreferences* pPrefs, LPCTSTR lpszSection, CDWordArray& aItems);
	static void WriteProfileArray(IPreferences* pPrefs, LPCTSTR lpszSection, const CDWordArray& aItems, BOOL bPreDeleteSection = TRUE);
		
	static CString KeyFromFile(LPCTSTR szFilePath, BOOL bFilenameOnly = TRUE);

protected:
	static CIniSectionMap s_mapSections;
	static BOOL s_bDirty;
	static BOOL s_bIni;
	static CString s_sPrefsPath;
	static int s_nRef;
	static CCriticalSection s_cs;
	static BOOL s_bLocked;
	
protected:
	static BOOL WritePreferenceString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue, BOOL bQuoted);

protected:
	// internal helpers that require pre-locking
	static BOOL SaveInternal(BOOL bExternal);
	static void Release(CIniSectionMap& mapSections);

	static int GetSortedSections(CIniSectionArray& aSections);
	static int SectionSortProc(const void* pV1, const void* pV2);

	static CString GetIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault);
	static BOOL WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue, BOOL bQuoted);
	
	static INISECTION* GetSection(LPCTSTR lpszSection, BOOL bCreateNotExist);
	
	static BOOL GetEntryValue(const INISECTION& section, LPCTSTR lpszEntry, CString& sValue);
	static void SetEntryValue(INISECTION& section, LPCTSTR lpszEntry, LPCTSTR szValue, BOOL bQuoted);
	static void SetEntryValue(INISECTION& section, const INIENTRY& ie);

	static void Copy(const CIniSectionMap& mapSrc, CIniSectionMap& mapDest);
	static BOOL Load(LPCTSTR szFilePath, CIniSectionMap& mapSections);

private:
	class CIPreferencesImpl : public IPreferences
	{
	public:
		CIPreferencesImpl(CPreferences& prefs) : m_prefs(prefs) {}
		virtual ~CIPreferencesImpl() {}

		UINT GetProfileInt(LPCWSTR lpszSection, LPCWSTR lpszEntry, int nDefault) const;
		bool WriteProfileInt(LPCWSTR lpszSection, LPCWSTR lpszEntry, int nValue);
		
		LPCWSTR GetProfileString(LPCWSTR lpszSection, LPCWSTR lpszEntry, LPCWSTR lpszDefault) const;
		bool WriteProfileString(LPCWSTR lpszSection, LPCWSTR lpszEntry, LPCWSTR lpszValue);
		
		double GetProfileDouble(LPCWSTR lpszSection, LPCWSTR lpszEntry, double dDefault) const;
		bool WriteProfileDouble(LPCWSTR lpszSection, LPCWSTR lpszEntry, double dValue);
		
		bool DeleteProfileEntry(LPCWSTR lpszSection, LPCWSTR lpszEntry);
		bool DeleteProfileSection(LPCWSTR lpszSection, bool bIncSubSections);

		bool HasProfileSection(LPCWSTR lpszSection) const;

	protected:
		CPreferences& m_prefs;
	};

	CIPreferencesImpl m_iPrefs;
};

#endif // !defined(AFX_PREFERENCES_H__DF763543_F9D5_4C94_BBD9_DF7E6E41B8C2__INCLUDED_)
