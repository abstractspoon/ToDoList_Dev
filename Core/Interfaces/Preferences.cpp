// Preferences.cpp: implementation of the CPreferences class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Preferences.h"

#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\driveinfo.h"
#include "..\shared\regkey.h"
#include "..\shared\autoflag.h"

#include "..\3rdparty\stdiofileex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

INIENTRY::INIENTRY(LPCTSTR szName, LPCTSTR szValue, BOOL bQuote) 
	: sName(szName), sValue(szValue), bQuoted(bQuote) 
{
}

CString INIENTRY::Format() const
{
	CString sEntry;

	if (bQuoted)
		sEntry.Format(_T("%s=\"%s\""), sName, sValue);
	else
		sEntry.Format(_T("%s=%s"), sName, sValue);

	return sEntry;
}

BOOL INIENTRY::Parse(const CString& sEntry)
{
	sName = sEntry;

	if (!Misc::Split(sName, sValue, '='))
		return FALSE;

	// remove quotes
	bQuoted = sValue.Replace(_T("\""), _T(""));

	return !sName.IsEmpty();
}

BOOL INIENTRY::operator==(const INIENTRY& ie) const
{
	return ((sName == ie.sName) && 
			(sValue == ie.sValue) && 
			(bQuoted == ie.bQuoted));
}

/////////////////////////////////////////////////////////////////////////////////

INISECTION::INISECTION(LPCTSTR szName) : sSection(szName) 
{
	aEntries.InitHashTable(199); // prime number closest to 200
}

INISECTION::INISECTION(const INISECTION& other) : sSection(other.sSection)
{
	Misc::CopyStrT<INIENTRY>(other.aEntries, aEntries);
}

//////////////////////////////////////////////////////////////////////

static LPCTSTR ENDL = _T("\r\n");
static CString NULLSTR;

//////////////////////////////////////////////////////////////////////

CIniSectionMap		CPreferences::s_mapSections;
BOOL				CPreferences::s_bDirty = FALSE;
BOOL				CPreferences::s_bIni = FALSE;
CString				CPreferences::s_sPrefsPath;
int					CPreferences::s_nRef = 0;
CCriticalSection	CPreferences::s_cs;
BOOL				CPreferences::s_bLocked = FALSE;

//////////////////////////////////////////////////////////////////////

#define LOCKPREFS()                           \
	ASSERT(!s_bLocked);                       \
	if (s_bLocked) return;                    \
	CSingleLock lock(&s_cs, TRUE);            \
	CAutoFlag af(s_bLocked, lock.IsLocked());

#define LOCKPREFSRET(ret)                     \
	ASSERT(!s_bLocked);                       \
	if (s_bLocked) return ret;                \
	CSingleLock lock(&s_cs, TRUE);            \
	CAutoFlag af(s_bLocked, lock.IsLocked());

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreferences::CPreferences() : m_iPrefs(*this)
{
	// prevent anyone else changing the shared resources
	// for the duration of this function
	LOCKPREFS();

	if (!IsInitialised())
		return;

	// if no other object is active we need to reload the preferences
	if (s_nRef == 0)
	{
		if (s_bIni)
		{
			// check for existing backup file first
			CString sBackupPath = CFileBackup::BuildBackupPath(s_sPrefsPath, FBS_OVERWRITE);
			
			if (FileMisc::FileExists(sBackupPath))
				FileMisc::MoveFile(sBackupPath, s_sPrefsPath, TRUE, TRUE);

			CIniSectionMap mapItems;

			if (Load(s_sPrefsPath, mapItems))
			{
				Release(s_mapSections);
				Copy(mapItems, s_mapSections);
				Release(mapItems);
			}
		}

		s_bDirty = FALSE;
	}
				
	s_nRef++; // increment reference count
}

CPreferences::~CPreferences()
{
	// prevent anyone else changing the shared resources
	// for the duration of this function
	LOCKPREFS();
			
	s_nRef--; // decrement reference count
	
	// save ini?
	if ((s_nRef == 0) && s_bIni)
	{
		// Note: We no longer release the current data
		// until the next load
		if (!SaveInternal(FALSE))
			s_nRef++;
	}
}

void CPreferences::Release()
{
	// prevent anyone else changing the shared resources
	// for the duration of this function
	LOCKPREFS();
			
	if (s_bIni)
	{
		Release(s_mapSections);
	}
	else
	{
		// Ini update of app version happens in SaveInternal
		WritePreferenceString(_T("AppVer"), _T("Version"), FileMisc::GetAppVersion(), FALSE);
	}
}

void CPreferences::Release(CIniSectionMap& mapSections)
{
	ASSERT(s_nRef == 0);
	ASSERT(s_bLocked);

	CString sUnused;
	INISECTION* pSection;

	POSITION pos = mapSections.GetStartPosition();

	while (pos)
	{
		mapSections.GetNextAssoc(pos, sUnused, pSection);
		delete pSection;
	}

	mapSections.RemoveAll();
}

BOOL CPreferences::Initialise(LPCTSTR szPrefsPath, BOOL bIni)
{
	if (Misc::IsEmpty(szPrefsPath))
	{
		ASSERT(0);
		return FALSE;
	}

	LOCKPREFSRET(FALSE);

	// Must be able to save existing ini file
	if (s_bIni)
	{
		if (!s_sPrefsPath.IsEmpty() && !Save())
		{
			ASSERT(0);
			return FALSE;
		}

		if (!bIni)
			Release(s_mapSections);
	}

	// Must be able to load the 'new' prefs
	if (bIni && FileMisc::FileExists(szPrefsPath))
	{
		CIniSectionMap mapItems;

		if (!Load(szPrefsPath, mapItems))
		{
			ASSERT(0);
			return FALSE;
		}
		else
		{
			// Take a copy in case next time we can't load the file
			Copy(mapItems, s_mapSections);
			Release(mapItems);
		}
	}

	s_sPrefsPath = szPrefsPath;
	s_bIni = bIni;
	s_nRef = 0;

	return TRUE;
}

BOOL CPreferences::IsInitialised()
{
	return !s_sPrefsPath.IsEmpty();
}

BOOL CPreferences::IsEmpty()
{
	return (s_mapSections.GetCount() == 0);
}

CString CPreferences::GetPath(BOOL bFriendly)
{
	if (UsesIni() || !bFriendly)
		return s_sPrefsPath;

	// else friendly registry path
	return (_T("HKEY_CURRENT_USER\\") + s_sPrefsPath);
}

BOOL CPreferences::Save()
{
	if (!s_bIni)
	{
		ASSERT(0);
		return FALSE;
	}

	if (!s_bDirty)
		return TRUE; // nothing to do

	// prevent anyone else changing the shared resources
	// for the duration of this function
	LOCKPREFSRET(FALSE);

	return SaveInternal(FALSE);
}

// THIS IS AN INTERNAL METHOD THAT ASSUMES CALLERS HAVE INITIALISED A LOCK ALREADY
BOOL CPreferences::SaveInternal(BOOL bExternal)
{
	ASSERT(s_bLocked);
	ASSERT(s_bIni);

	if (!s_bLocked)
		return FALSE;

	if (!s_bDirty)
		return TRUE; // nothing to do

	if (bExternal)
		FileMisc::LogTextRaw(_T("External call to CPreferences::Save()"));
	else
		FileMisc::LogTextRaw(_T("Saving preferences in ~CPreferences()"));
	
	// insert application version
	WriteIniString(_T("AppVer"), _T("Version"), FileMisc::GetAppVersion(), FALSE);

	// Build output as a single formatted string so that the 
	// time the prefs file is open is as short as possible
	CString sPrefsContents;
	
	CIniSectionArray aSections;
	int nNumSection = GetSortedSections(aSections);

	for (int nSection = 0; nSection < nNumSection; nSection++)
	{
		// write section line
		const INISECTION* pSection = aSections[nSection];
		
		CString sLine;
		sLine.Format(_T("[%s]%s"), pSection->sSection, ENDL);
		
		sPrefsContents += sLine;
		
		// write entries to a CStringArray, then sort it and write it to file
		CStringArray aEntries;
		aEntries.SetSize(pSection->aEntries.GetCount(), 10);
		
		// save map to array
		POSITION pos = pSection->aEntries.GetStartPosition();
		int nEntry = 0;
		
		while (pos)
		{
			CString sDummy;
			INIENTRY ie;
			
			pSection->aEntries.GetNextAssoc(pos, sDummy, ie);

			aEntries.SetAt(nEntry++, ie.Format());
		}
		
		// sort array
		Misc::SortArray(aEntries);
		
		// format by newlines
		sPrefsContents += Misc::FormatArray(aEntries, ENDL);
		sPrefsContents += ENDL;
		sPrefsContents += ENDL;
	}
	
	// backup file first
	CFileBackup backup(s_sPrefsPath, FBS_OVERWRITE | FBS_DATESTAMP, _T("ini.Backup"));
	
	// write prefs
	{
		CStdioFileEx file;
	
		if (!file.Open(s_sPrefsPath, CFile::modeWrite | CFile::modeCreate, SFEF_UTF16))
			return FALSE;

		file.WriteString(sPrefsContents);
	}
	
	s_bDirty = FALSE;

	return TRUE;
}

int CPreferences::GetSortedSections(CIniSectionArray& aSections)
{
	ASSERT(s_bLocked);
	ASSERT(s_bIni);

	if (!s_bLocked)
		return 0;

	aSections.RemoveAll();

	CString sUnused;
	INISECTION* pSection;
	POSITION pos = s_mapSections.GetStartPosition();

	while (pos)
	{
		s_mapSections.GetNextAssoc(pos, sUnused, pSection);
		aSections.Add(pSection);
	}

	Misc::SortArrayT<INISECTION*>(aSections, SectionSortProc);
	return aSections.GetSize();
}

int CPreferences::SectionSortProc(const void* pV1, const void* pV2)
{
	typedef INISECTION* PSECTION;

	const PSECTION* pS1 = (PSECTION*)pV1;
	const PSECTION* pS2 = (PSECTION*)pV2;

	return Misc::NaturalCompare((*pS1)->sSection, (*pS2)->sSection);
}

UINT CPreferences::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault) const
{
	if (s_bIni)
	{
		CString sValue = GetProfileString(lpszSection, lpszEntry);
		
		if (!sValue.IsEmpty()) 
			return _ttol(sValue);
	}
	else
	{
		CString sKeyName;
		sKeyName.Format(_T("%s\\%s"), s_sPrefsPath, lpszSection);

		CRegKey2 reg;

		if (reg.Open(HKEY_CURRENT_USER, sKeyName, TRUE) == ERROR_SUCCESS)
		{
			DWORD dwValue = 0;

			if (reg.Read(lpszEntry, dwValue) == ERROR_SUCCESS)
				return (UINT)dwValue;
		}
	}
	
	// all else
	return nDefault;
}

BOOL CPreferences::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
{
	if (s_bIni)
	{
		// FALSE -> We don't quote numbers
		return WritePreferenceString(lpszSection, lpszEntry, Misc::Format(nValue), FALSE);
	}
	else
	{
		CString sKeyName;
		sKeyName.Format(_T("%s\\%s"), s_sPrefsPath, lpszSection);

		CRegKey2 reg;

		if (reg.Open(HKEY_CURRENT_USER, sKeyName, FALSE) == ERROR_SUCCESS)
		{
			return (reg.Write(lpszEntry, (DWORD)nValue) == ERROR_SUCCESS);
		}
	}

	return FALSE;
}

CString CPreferences::GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault) const
{
	if (s_bIni)
	{
		// prevent anyone else changing the shared resources
		// for the duration of this function
		LOCKPREFSRET(NULLSTR);

		return GetIniString(lpszSection, lpszEntry, lpszDefault);
	}

	// else
	CString sKeyName;
	sKeyName.Format(_T("%s\\%s"), s_sPrefsPath, lpszSection);

	CRegKey2 reg;

	if (reg.Open(HKEY_CURRENT_USER, sKeyName, TRUE) == ERROR_SUCCESS)
	{
		CString sValue;

		if (reg.Read(lpszEntry, sValue) == ERROR_SUCCESS)
			return sValue;
	}

	// else
	return lpszDefault;
}

BOOL CPreferences::WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
	// quote string values passed in from outside
	return WritePreferenceString(lpszSection, lpszEntry, lpszValue, TRUE);
}

BOOL CPreferences::WritePreferenceString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue, BOOL bQuoted)
{
	if (s_bIni)
	{
		// prevent anyone else changing the shared resources
		// for the duration of this function
		LOCKPREFSRET(FALSE);

		return WriteIniString(lpszSection, lpszEntry, lpszValue, bQuoted);
	}

	// else registry
	CString sKeyName;
	sKeyName.Format(_T("%s\\%s"), s_sPrefsPath, lpszSection);

	CRegKey2 reg;

	if (reg.Open(HKEY_CURRENT_USER, sKeyName, FALSE) == ERROR_SUCCESS)
		return (reg.Write(lpszEntry, lpszValue) == ERROR_SUCCESS);

	return FALSE;
}

// THIS IS AN INTERNAL METHOD THAT ASSUMES CALLERS HAVE INITIALISED A LOCK ALREADY
CString CPreferences::GetIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
	ASSERT(s_bIni);
	ASSERT(s_bLocked);

	if (s_bIni && s_bLocked)
	{
		INISECTION* pSection = GetSection(lpszSection, FALSE);
		
		if (pSection)
		{
			CString sValue;
			
			if (GetEntryValue(*pSection, lpszEntry, sValue))
				return sValue;
		}
	
		// else
		if (lpszDefault && *lpszDefault) // non-empty
			return lpszDefault;
	}

	// else
	return NULLSTR;
}

// THIS IS AN INTERNAL METHOD THAT ASSUMES CALLERS HAVE INITIALISED A LOCK ALREADY
BOOL CPreferences::WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue, BOOL bQuoted)
{
	ASSERT(s_bIni);
	ASSERT(s_bLocked);

	if (s_bIni && s_bLocked)
	{
		INISECTION* pSection = GetSection(lpszSection, TRUE);
		ASSERT(pSection);
		
		if (pSection)
		{
			SetEntryValue(*pSection, lpszEntry, lpszValue, bQuoted);
			return TRUE;
		}
	}
	
	// else
	return FALSE;
}

double CPreferences::GetProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dDefault) const
{
	CString sValue = GetProfileString(lpszSection, lpszEntry, Misc::Format(dDefault, 6));
	
	if (sValue.IsEmpty())
		return dDefault;
	else
		return Misc::Atof(sValue);
}

bool CPreferences::WriteProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dValue)
{
	// FALSE -> We don't quote numbers
	return (WritePreferenceString(lpszSection, lpszEntry, Misc::Format(dValue, 6), FALSE) != FALSE);
}

int CPreferences::GetProfileArray(LPCTSTR lpszSection, CStringArray& aItems, BOOL bAllowEmpty) const
{
	return GetProfileArray(&m_iPrefs, lpszSection, aItems, bAllowEmpty);
}

int CPreferences::GetProfileArray(const IPreferences* pPrefs, LPCTSTR lpszSection, CStringArray& aItems, BOOL bAllowEmpty)
{
	aItems.RemoveAll();
	int nCount = pPrefs->GetProfileInt(lpszSection, _T("ItemCount"), 0);
	
	// items
	for (int nItem = 0; nItem < nCount; nItem++)
	{
		CString sItemKey, sItem;
		sItemKey.Format(_T("Item%d"), nItem);
		sItem = pPrefs->GetProfileString(lpszSection, sItemKey);
		
		if (bAllowEmpty || !sItem.IsEmpty())
			aItems.Add(sItem);
	}
	
	return aItems.GetSize();
}

void CPreferences::WriteProfileArray(LPCTSTR lpszSection, const CStringArray& aItems, BOOL bPreDeleteSection)
{
	WriteProfileArray(&m_iPrefs, lpszSection, aItems, bPreDeleteSection);
}

void CPreferences::WriteProfileArray(IPreferences* pPrefs, LPCTSTR lpszSection, const CStringArray& aItems, BOOL bPreDeleteSection)
{
	// pre-delete?
	if (bPreDeleteSection)
		pPrefs->DeleteProfileSection(lpszSection);

	int nCount = aItems.GetSize();
	
	// items
	for (int nItem = 0; nItem < nCount; nItem++)
	{
		CString sItemKey;
		sItemKey.Format(_T("Item%d"), nItem);
		pPrefs->WriteProfileString(lpszSection, sItemKey, aItems[nItem]);
	}
	
	// item count
	pPrefs->WriteProfileInt(lpszSection, _T("ItemCount"), nCount);
}

int CPreferences::GetProfileArray(LPCTSTR lpszSection, CDWordArray& aItems) const
{
	return GetProfileArray(&m_iPrefs, lpszSection, aItems);
}

int CPreferences::GetProfileArray(const IPreferences* pPrefs, LPCTSTR lpszSection, CDWordArray& aItems)
{
	aItems.RemoveAll();

	CString sItems = pPrefs->GetProfileString(lpszSection, _T("Items"));

	if (!Misc::Split(sItems, aItems, '|'))
	{
		// backwards compatibility
		int nCount = pPrefs->GetProfileInt(lpszSection, _T("ItemCount"), 0);
	
		// items
		for (int nItem = 0; nItem < nCount; nItem++)
		{
			CString sItemKey, sItem;

			sItemKey.Format(_T("Item%d"), nItem);
			aItems.Add(pPrefs->GetProfileInt(lpszSection, sItemKey));
		}
	}
	
	return aItems.GetSize();
}

void CPreferences::WriteProfileArray(LPCTSTR lpszSection, const CDWordArray& aItems, BOOL bPreDeleteSection)
{
	WriteProfileArray(&m_iPrefs, lpszSection, aItems, bPreDeleteSection);
}

void CPreferences::WriteProfileArray(IPreferences* pPrefs, LPCTSTR lpszSection, const CDWordArray& aItems, BOOL bPreDeleteSection)
{
	if (bPreDeleteSection)
	{
		pPrefs->DeleteProfileSection(lpszSection);
	}
	else
	{
		// Cleanup old format
		int nItem = pPrefs->GetProfileInt(lpszSection, _T("ItemCount"), -1);

		if (nItem >= 0)
		{
			while (nItem--)
				pPrefs->DeleteProfileEntry(lpszSection, Misc::MakeKey(_T("Item%d"), nItem));

			pPrefs->DeleteProfileEntry(lpszSection, _T("ItemCount"));
		}
	}

	pPrefs->WriteProfileString(lpszSection, _T("Items"), Misc::FormatArray(aItems, '|'));
}

// THIS IS AN INTERNAL METHOD THAT ASSUMES CALLERS HAVE INITIALISED A LOCK ALREADY
BOOL CPreferences::GetEntryValue(const INISECTION& section, LPCTSTR lpszEntry, CString& sValue)
{
	ASSERT(s_bIni);
	ASSERT(s_bLocked);
	
	if (s_bIni && s_bLocked)
	{
		INIENTRY ie;
		
		if (section.aEntries.Lookup(Misc::ToUpper(lpszEntry), ie))
		{
			sValue = ie.sValue;
			return TRUE;
		}
	}

	// else
	return FALSE;
}

void CPreferences::SetEntryValue(INISECTION& section, LPCTSTR lpszEntry, LPCTSTR szValue, BOOL bQuoted)
{
	SetEntryValue(section, INIENTRY(lpszEntry, szValue, bQuoted));
}

// THIS IS AN INTERNAL METHOD THAT ASSUMES CALLERS HAVE INITIALISED A LOCK ALREADY
void CPreferences::SetEntryValue(INISECTION& section, const INIENTRY& ie)
{
	ASSERT(s_bIni);
	ASSERT(s_bLocked);

	if (s_bIni && s_bLocked)
	{
		INIENTRY ieExist;
		CString sKey = Misc::ToUpper(ie.sName);

		if (!section.aEntries.Lookup(sKey, ieExist) || !(ie == ieExist))
		{
			section.aEntries[sKey] = ie;
			s_bDirty = TRUE;
		}
	}
}

// THIS IS AN INTERNAL METHOD THAT ASSUMES CALLERS HAVE INITIALISED A LOCK ALREADY
INISECTION* CPreferences::GetSection(LPCTSTR lpszSection, BOOL bCreateNotExist)
{
	ASSERT(s_bIni);
	ASSERT(s_bLocked);

	if (s_bIni && s_bLocked)
	{
		CString sSectionKey = (Misc::ToUpper(lpszSection));
		INISECTION* pSection = NULL;

		if (s_mapSections.Lookup(sSectionKey, pSection))
		{
			ASSERT(pSection);
		}
		else if (bCreateNotExist)
		{
			// add a new section
			pSection = new INISECTION(lpszSection);
			s_mapSections[sSectionKey] = pSection;
		}

		return pSection;
	}
	
	// else
	return NULL;
}

BOOL CPreferences::DeleteProfileSection(LPCTSTR lpszSection, BOOL bIncSubSections)
{
	ASSERT(IsInitialised());
	ASSERT(lpszSection && *lpszSection);

	if (s_bIni)
	{
		// prevent anyone else changing the shared resources
		// for the duration of this function
		LOCKPREFSRET(FALSE);
		
		INISECTION* pSection = GetSection(lpszSection, FALSE);
		 
		if (pSection)
		{
			CString sSectionKey = Misc::ToUpper(lpszSection);

			s_mapSections.RemoveKey(sSectionKey);
			delete pSection;

			if (bIncSubSections)
			{
				CString sSubSectionKey;
				POSITION pos = s_mapSections.GetStartPosition();

				while (pos)
				{
					s_mapSections.GetNextAssoc(pos, sSubSectionKey, pSection);

					// look for parent section at head of subsection
					if (sSubSectionKey.Find(sSectionKey) == 0)
					{
						s_mapSections.RemoveKey(sSubSectionKey);
						delete pSection;
					}
				}
			}

			s_bDirty = TRUE;
			return TRUE;
		}

		// not found
		return FALSE;
	}

	// else registry
	CString sFullKey;
	sFullKey.Format(_T("%s\\%s"), s_sPrefsPath, lpszSection);

	return CRegKey2::DeleteKey(HKEY_CURRENT_USER, sFullKey);
}

BOOL CPreferences::DeleteProfileEntry(LPCTSTR lpszSection, LPCTSTR lpszEntry)
{
	ASSERT(IsInitialised());
	ASSERT(lpszSection && *lpszSection);
	ASSERT(lpszEntry && *lpszEntry);

	if (s_bIni)
	{
		// prevent anyone else changing the shared resources
		// for the duration of this function
		LOCKPREFSRET(FALSE);
		
		INISECTION* pSection = GetSection(lpszSection, FALSE);

		if (pSection)
		{
			pSection->aEntries.RemoveKey(Misc::ToUpper(lpszEntry));

			s_bDirty = TRUE;
			return TRUE;
		}

		// not found
		return FALSE;
	}

	// else registry
	CString sFullKey;
	sFullKey.Format(_T("%s\\%s"), s_sPrefsPath, lpszSection);

	CRegKey2 reg;
	
	if (reg.Open(HKEY_CURRENT_USER, sFullKey, FALSE) == ERROR_SUCCESS)
		return (reg.DeleteValue(lpszEntry) == ERROR_SUCCESS);

	// else
	return FALSE;
}

BOOL CPreferences::HasProfileSection(LPCTSTR lpszSection) const
{
	if (s_bIni)
	{
		// prevent anyone else changing the shared resources
		// for the duration of this function
		LOCKPREFSRET(FALSE);
		
		return (GetSection(lpszSection, FALSE) != NULL);
	}

	// else registry
	CString sFullKey;
	sFullKey.Format(_T("%s\\%s"), s_sPrefsPath, lpszSection);

	return CRegKey2::KeyExists(HKEY_CURRENT_USER, sFullKey);
}

int CPreferences::GetProfileSectionNames(CStringArray& aSections, LPCTSTR szPrefix)
{
	// prevent anyone else changing the shared resources
	// for the duration of this function
	LOCKPREFSRET(0);
	
	aSections.RemoveAll();

	if (s_bIni)
	{
		aSections.RemoveAll();

		CString sUnused;
		INISECTION* pSection;
		POSITION pos = s_mapSections.GetStartPosition();

		while (pos)
		{
			s_mapSections.GetNextAssoc(pos, sUnused, pSection);
			ASSERT(pSection);

			if (Misc::IsEmpty(szPrefix) || (pSection->sSection.Find(szPrefix) == 0))
				aSections.Add(pSection->sSection);
		}
	}

	return aSections.GetSize();
}

CString CPreferences::KeyFromFile(LPCTSTR szFilePath, BOOL bFileNameOnly)
{
	CString sKey = (bFileNameOnly ? FileMisc::GetFileNameFromPath(szFilePath) : szFilePath);
	sKey.Replace('\\', '_');

	// if the filepath is on a removable drive then we strip off the drive letter
	if (!bFileNameOnly)
	{
		int nType = CDriveInfo::GetPathType(szFilePath);

		if (nType == DRIVE_REMOVABLE)
			sKey = sKey.Mid(1);
	}

	return sKey;
}

void CPreferences::Copy(const CIniSectionMap& mapSrc, CIniSectionMap& mapDest)
{
	ASSERT(mapDest.GetCount() == 0);

	CString sSectionKey;
	INISECTION* pSection;
	POSITION pos = mapSrc.GetStartPosition();

	while (pos)
	{
		mapSrc.GetNextAssoc(pos, sSectionKey, pSection);
		mapDest[sSectionKey] = new INISECTION(*pSection);
	}

	ASSERT(mapDest.GetCount() == mapSrc.GetCount());
}

BOOL CPreferences::Load(LPCTSTR szFilePath, CIniSectionMap& mapSections)
{
	// read the ini file
	CStdioFileEx file;

	if (!file.Open(szFilePath, CFile::modeRead))
		return FALSE;

	CString sLine;
	INISECTION* pCurSection = NULL;

	while (file.ReadString(sLine))
	{
		if (!Misc::Trim(sLine).IsEmpty())
		{
			// is it a section ?
			if (Misc::TrimFirstIf('[', sLine))
			{
				VERIFY(Misc::TrimLastIf(']', sLine));

				// assume (for speed) that the section is already unique
				pCurSection = new INISECTION(sLine);
				mapSections[Misc::ToUpper(pCurSection->sSection)] = pCurSection;
			}
			else if (pCurSection)
			{
				// else an entry
				INIENTRY ie;

				if (ie.Parse(sLine))
					pCurSection->aEntries[Misc::ToUpper(ie.sName)] = ie;
			}
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
	
UINT CPreferences::CIPreferencesImpl::GetProfileInt(LPCWSTR lpszSection, LPCWSTR lpszEntry, int nDefault) const
{
	return m_prefs.GetProfileInt(lpszSection, lpszEntry, nDefault);
}

bool CPreferences::CIPreferencesImpl::WriteProfileInt(LPCWSTR lpszSection, LPCWSTR lpszEntry, int nValue)
{
	return (m_prefs.WriteProfileInt(lpszSection, lpszEntry, nValue) != FALSE);
}

LPCWSTR CPreferences::CIPreferencesImpl::GetProfileString(LPCWSTR lpszSection, LPCWSTR lpszEntry, LPCWSTR lpszDefault) const
{
	static CString sValue;
	sValue = m_prefs.GetProfileString(lpszSection, lpszEntry, lpszDefault);

	return sValue;
}

bool CPreferences::CIPreferencesImpl::WriteProfileString(LPCWSTR lpszSection, LPCWSTR lpszEntry, LPCWSTR lpszValue)
{
	return (m_prefs.WriteProfileString(lpszSection, lpszEntry, lpszValue) != FALSE);
}
	
double CPreferences::CIPreferencesImpl::GetProfileDouble(LPCWSTR lpszSection, LPCWSTR lpszEntry, double dDefault) const
{
	return m_prefs.GetProfileDouble(lpszSection, lpszEntry, dDefault);
}

bool CPreferences::CIPreferencesImpl::WriteProfileDouble(LPCWSTR lpszSection, LPCWSTR lpszEntry, double dValue)
{
	return (m_prefs.WriteProfileDouble(lpszSection, lpszEntry, dValue) != FALSE);
}
	
bool CPreferences::CIPreferencesImpl::DeleteProfileEntry(LPCWSTR lpszSection, LPCWSTR lpszEntry)
{
	return (m_prefs.DeleteProfileEntry(lpszSection, lpszEntry) != FALSE);
}

bool CPreferences::CIPreferencesImpl::DeleteProfileSection(LPCWSTR lpszSection, bool bIncSubSections)
{
	return (m_prefs.DeleteProfileSection(lpszSection, bIncSubSections ? TRUE : FALSE) != FALSE);
}

bool CPreferences::CIPreferencesImpl::HasProfileSection(LPCWSTR lpszSection) const
{
	return (m_prefs.HasProfileSection(lpszSection) != FALSE);
}

//////////////////////////////////////////////////////////////////////
	
