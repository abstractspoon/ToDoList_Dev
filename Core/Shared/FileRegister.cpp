// FileRegister.cpp: implementation of the CFileRegister class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileRegister.h"
#include "regkey.h"
#include "Filemisc.h"
#include "misc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileRegister::CFileRegister(LPCTSTR szExt, LPCTSTR szFileType)
{
	m_sExt = GetExtension(szExt);

	m_sFileType = szFileType;
	m_sFileType.TrimRight();
	m_sFileType.TrimLeft();

	// get the app path
	m_sAppPath = FileMisc::GetAppFilePath();

	ASSERT (!m_sAppPath.IsEmpty());
}

CFileRegister::~CFileRegister()
{
}

BOOL CFileRegister::RegisterFileType(LPCTSTR szFileDesc, int nIcon, BOOL bAllowShellOpen, LPCTSTR szParams, BOOL bAskUser) const
{
	CRegKey2 reg;
	CString sKey;
	CString sEntry;
	int nRet = IDYES;
	CString sMessage;
	BOOL bSuccess = TRUE, bChange = FALSE;

	ASSERT (!m_sExt.IsEmpty());
	ASSERT (!m_sFileType.IsEmpty());

	if (m_sExt.IsEmpty() || m_sFileType.IsEmpty())
		return FALSE;

	if (reg.Open(HKEY_CLASSES_ROOT, m_sExt, FALSE) == ERROR_SUCCESS)
	{
		reg.Read(_T(""), sEntry);

		// if the current filetype is not correct query the use to reset it
		if (!sEntry.IsEmpty() && sEntry.CompareNoCase(m_sFileType) != 0)
		{
			if (bAskUser)
			{
				sMessage.Format(_T("The file extension %s is used by %s for its %s.\n\nWould you like %s to be the default application for this file type."),
								m_sExt, AfxGetAppName(), szFileDesc, AfxGetAppName());
				
				nRet = AfxMessageBox(sMessage, MB_YESNO | MB_ICONQUESTION);
			}

			bChange = TRUE;
		}
		else
		{
			bChange = sEntry.IsEmpty();
		}

		// if not no then set
		if (nRet != IDNO)
		{
			bSuccess = (reg.Write(_T(""), m_sFileType) == ERROR_SUCCESS);

			reg.Close();

			if (reg.Open(HKEY_CLASSES_ROOT, m_sFileType, FALSE) == ERROR_SUCCESS)
			{
				bSuccess &= (reg.Write(_T(""), szFileDesc) == ERROR_SUCCESS);
				reg.Close();

				// app to open file
				if (reg.Open(HKEY_CLASSES_ROOT, m_sFileType + _T("\\shell\\open\\command"), FALSE) == ERROR_SUCCESS)
				{
					CString sShellOpen;

					if (bAllowShellOpen)
					{
						if (szParams)
							sShellOpen = _T("\"") + m_sAppPath + _T("\" \"%1\" ") + CString(szParams);
						else
							sShellOpen = _T("\"") + m_sAppPath + _T("\" \"%1\"");
					}

					bSuccess &= (reg.Write(_T(""), sShellOpen) == ERROR_SUCCESS);
				}
				else
				{
					bSuccess = FALSE;
				}

				// icons
				reg.Close();

				if (reg.Open(HKEY_CLASSES_ROOT, m_sFileType + _T("\\DefaultIcon"), FALSE) == ERROR_SUCCESS)
				{
					CString sIconPath;
					sIconPath.Format(_T("%s,%d"), m_sAppPath, nIcon);
					bSuccess &= (reg.Write(_T(""), sIconPath) == ERROR_SUCCESS);
				}
				else
				{
					bSuccess = FALSE;
				}
			}
			else
			{
				bSuccess = FALSE; 
			}
		}
		else
		{
			bSuccess = FALSE;
		}
	}
	else
	{
		bSuccess = FALSE;
	}

	if (bSuccess && bChange)
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

	return bSuccess;
}

BOOL CFileRegister::UnRegisterFileType() const
{
	CRegKey2 reg;
	CString sKey;
	CString sEntry;
	BOOL bSuccess = FALSE;

	ASSERT (!m_sExt.IsEmpty());

	if (m_sExt.IsEmpty())
		return FALSE;

	if (reg.Open(HKEY_CLASSES_ROOT, m_sExt, TRUE) == ERROR_SUCCESS)
	{
		reg.Read(_T(""), sEntry);

		if (sEntry.IsEmpty())
			return TRUE; // we werent the register app so all's well

		ASSERT (!m_sFileType.IsEmpty());

		if (m_sFileType.IsEmpty())
			return FALSE;

		if (sEntry.CompareNoCase(m_sFileType) != 0)
			return TRUE; // we werent the register app so all's well

		// else delete the keys
		reg.Close();
		CRegKey2::DeleteKey(HKEY_CLASSES_ROOT, m_sExt);
		CRegKey2::DeleteKey(HKEY_CLASSES_ROOT, m_sFileType);
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

		bSuccess = TRUE;
	}

	return bSuccess;
}

// statics
BOOL CFileRegister::IsRegisteredApp(LPCTSTR szExt, LPCTSTR szAppPath, BOOL bFilenameOnly)
{
	CString sRegAppPath = GetRegisteredAppPath(szExt, bFilenameOnly);
	CString sAppPath = (bFilenameOnly ? FileMisc::GetFileNameFromPath(szAppPath) : szAppPath);

	return (sAppPath.CompareNoCase(sRegAppPath) == 0);
}

BOOL CFileRegister::IsRegisteredAppInstalled(LPCTSTR szExt)
{
	CString sRegAppPath = GetRegisteredAppPath(szExt);
	return FileMisc::FileExists(sRegAppPath);
}

HICON CFileRegister::GetRegisteredIcon(LPCTSTR szExt, BOOL bLargeIcon)
{
	CString sAppPath = GetRegisteredAppEntry(szExt, _T("DefaultIcon")), sIconIndex;

	Misc::Split(sAppPath, sIconIndex, ',');
	Misc::MakeUnquoted(sAppPath, FALSE);

	HICON hIcon = NULL;

	if (bLargeIcon)
		VERIFY(ExtractIconEx(sAppPath, _ttoi(sIconIndex), &hIcon, NULL, 1));
	else
		VERIFY(ExtractIconEx(sAppPath, _ttoi(sIconIndex), NULL, &hIcon, 1));

	return hIcon;
}

CString CFileRegister::GetRegisteredAppPath(LPCTSTR szExt, BOOL bFilenameOnly)
{
	CString sAppPath = GetRegisteredAppEntry(szExt, _T("shell\\open\\command"));

	if (!sAppPath.IsEmpty())
	{
		// extract the file path if quoted
		if (sAppPath[0] == '"')
		{
			int nEnd = sAppPath.Find('"', 1);
			ASSERT(nEnd != -1);

			sAppPath = sAppPath.Mid(1, nEnd - 1);
		}
		
		// note: apps often have parameters after so we do this
		CString sDrive, sDir, sFName;
		FileMisc::SplitPath(sAppPath, &sDrive, &sDir, &sFName);
		
		sFName += _T(".exe");
		
		if (bFilenameOnly)
			sAppPath = sFName;
		else
			FileMisc::MakePath(sAppPath, sDrive, sDir, sFName);
	}
	
	return sAppPath;
}

CString CFileRegister::GetRegisteredAppEntry(LPCTSTR szExt, LPCTSTR szRegKey)
{
	CString sExt;
	
	if (FileMisc::IsPath(szExt))
		sExt = FileMisc::GetExtension(szExt);
	else
		sExt = GetExtension(szExt);

	ASSERT (sExt.IsEmpty() || (sExt[0] == '.'));

	if (sExt.IsEmpty() || (sExt[0] != '.'))
		return _T("");

	CRegKey2 reg;
	CString sEntry, sAppEntry;

	if (reg.Open(HKEY_CLASSES_ROOT, sExt, TRUE) == ERROR_SUCCESS)
	{
		reg.Read(_T(""), sEntry);

		if (!sEntry.IsEmpty() && CRegKey2::KeyExists(HKEY_CLASSES_ROOT, sEntry))
		{
			reg.Close();

			// app to open file
			sEntry += '\\';
			sEntry += szRegKey;
			
			if (reg.Open(HKEY_CLASSES_ROOT, sEntry, TRUE) == ERROR_SUCCESS)
			{
				reg.Read(_T(""), sAppEntry);
				sAppEntry.TrimLeft();
			}
		}
	}

	return sAppEntry;
}

CString CFileRegister::GetExtension(LPCTSTR szExt)
{
	// ensure leading period
	CString sExt(szExt);

	sExt.TrimRight();
	sExt.TrimLeft();

	if (!sExt.IsEmpty() && sExt[0] != '.')
		sExt = "." + sExt;

	return sExt;
}
