// TDLUpdateScript.cpp: implementation of the CTDCWebUpdateScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCWebUpdateScript.h"

#include "..\shared\misc.h"
#include "..\shared\webmisc.h"
#include "..\shared\filemisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const LPCTSTR UPDATE_SCRIPT_URL		= _T("https://www.abstractspoon.com/todolist_update_new.txt");
const LPCTSTR PRERELEASE_SCRIPT_URL	= _T("https://www.abstractspoon.com/todolist_pre_new.txt");

const LPCTSTR SCRIPT_HEADER_VAL		= _T("ToDoList AbstractSpoon");

const LPCTSTR SCRIPT_HEADER_KEY		= _T("SCRIPT_HEADER");
const LPCTSTR SCRIPT_VER_KEY		= _T("SCRIPT_VERSION");
const LPCTSTR EXE_VER_KEY			= _T("EXE_VERSION");
const LPCTSTR DISPLAY_VER_KEY		= _T("DISPLAY_VERSION");
const LPCTSTR DESCRIPTION_KEY		= _T("DESCRIPTION");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCWebUpdateScript::CTDCWebUpdateScript()
{

}

CTDCWebUpdateScript::~CTDCWebUpdateScript()
{
	
}

LPCTSTR CTDCWebUpdateScript::GetScriptUrl(BOOL bPreRelease) const
{
	if (bPreRelease)
		return PRERELEASE_SCRIPT_URL;
	
	// else
	return UPDATE_SCRIPT_URL;
}

BOOL CTDCWebUpdateScript::CheckForPreReleaseUpdates()
{
	return CheckForUpdates(TRUE);
}

BOOL CTDCWebUpdateScript::CheckForUpdates()
{
	return CheckForUpdates(FALSE);
}

BOOL CTDCWebUpdateScript::CheckForUpdates(BOOL bPreRelease)
{
	// NEVER update stable-release with pre-release
	CString sAppVer(FileMisc::GetAppVersion());
	BOOL bAppIsPreRelease = IsPreRelease(sAppVer);

	if (bPreRelease && !bAppIsPreRelease)
	{
		return FALSE;
	}

	// else
	if (!LoadScript(GetScriptUrl(bPreRelease)))
	{
		return FALSE;
	}

	CString sScriptExeVer, sScriptDisplayVer;
	
	if (!GetScriptExeVersion(sScriptExeVer))
	{
		FileMisc::LogText(_T("CTDCWebUpdateScript::LoadScript(unable to get exe version from script)"));
		return FALSE;
	}

	// DON'T allow pre-release to change versions
	// ie. must update to a stable release first
	if (bAppIsPreRelease && bPreRelease && (FileMisc::CompareVersions(sAppVer, sScriptExeVer, 2) != 0))
	{
		return FALSE;
	}
		
	// simple compare
	return (FileMisc::CompareVersions(sAppVer, sScriptExeVer, 4) < 0);
}

BOOL CTDCWebUpdateScript::LoadScript(const CString& sScriptUrl)
{
	m_mapScript.RemoveAll();

	// download the update script to temp file
	CString sTempFile = FileMisc::GetTempFilePath(_T(""));
	FileMisc::DeleteFile(sTempFile, TRUE);

	// clear cache before downloading
	if (!WebMisc::DeleteCacheEntry(sScriptUrl))
	{
		if (GetLastError() == ERROR_ACCESS_DENIED)
		{
			FileMisc::LogText(_T("CTDCWebUpdateScript::LoadScript(unable to delete cache entry for %s)"), sScriptUrl);
			return FALSE;
		}
	}
	
	if (!WebMisc::DownloadFile(sScriptUrl, sTempFile))
		return FALSE;
	
	// verify the script contents
	CStringArray aLines;
	int nLine = FileMisc::LoadFile(sTempFile, aLines);
	
	// delete temp file
	FileMisc::DeleteFile(sTempFile, TRUE);
	
	if (nLine == 0)
	{
		FileMisc::LogText(_T("CTDCWebUpdateScript::LoadScript(temp %s is empty)"), sTempFile);
		return FALSE;
	}

	// build map of values
	while (nLine--)
	{
		CString sKey = aLines[nLine], sValue;

		if (Misc::Split(sKey, sValue, ':'))
			m_mapScript[sKey] = sValue;
	}

	if (GetScriptVersion() <= 0)
	{
		FileMisc::LogText(_T("CTDCWebUpdateScript::LoadScript(script version is invalid)"));
		return FALSE;
	}

	return TRUE;
}

BOOL CTDCWebUpdateScript::GetScriptDetails(CString& sExeVer, CString& sDisplayVer, CStringArray& aDescription) const
{
	return (GetScriptExeVersion(sExeVer) && GetScriptDisplayVersion(sDisplayVer) && GetUpdateDescription(aDescription));
}

BOOL CTDCWebUpdateScript::IsPreRelease(const CString& sVer)
{
	CDWordArray aVer;

	if (FileMisc::SplitVersionNumber(sVer, aVer) >= 3)
	{
		switch (aVer[2])
		{
		case DEV_PREVIEW_VER:
		case ALPHA_RELEASE_VER:
		case BETA_RELEASE_VER:
		case RELEASE_CANDIDATE_VER:
			return TRUE;
		}
	}

	// else
	return FALSE;
}

BOOL CTDCWebUpdateScript::VerifyHeader() const
{
	CString sHeader;

	return (m_mapScript.Lookup(SCRIPT_HEADER_VAL, sHeader) && !sHeader.IsEmpty());
}

int CTDCWebUpdateScript::GetScriptVersion() const
{
	CString sVer;

	if (!m_mapScript.Lookup(SCRIPT_VER_KEY, sVer))
		return -1;

	return _ttoi(sVer);
}

BOOL CTDCWebUpdateScript::GetScriptExeVersion(CString& sVer) const
{
	return m_mapScript.Lookup(EXE_VER_KEY, sVer);
}

BOOL CTDCWebUpdateScript::GetScriptDisplayVersion(CString& sVer) const
{
	if (m_mapScript.Lookup(DISPLAY_VER_KEY, sVer) && !sVer.IsEmpty())
		return TRUE;

	// else
	return GetScriptExeVersion(sVer);
}

int CTDCWebUpdateScript::GetUpdateDescription(CStringArray& aDesc) const
{
	CString sDesc;

	if (m_mapScript.Lookup(DESCRIPTION_KEY, sDesc) && !sDesc.IsEmpty())
		return Misc::Split(sDesc, aDesc, '|', TRUE);

	// else
	 return FALSE;
}


