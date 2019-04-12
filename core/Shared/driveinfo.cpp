// CDriveInfo.cpp

#include "stdafx.h"
#include "driveinfo.h"

#include <direct.h>

float CDriveInfo::GetFreeBytes(int nDrive)
{
	CString sRoot;
	unsigned long totalClusters, freeClusters, sectors, bytes;

	ASSERT (nDrive > 0 && nDrive <= 26);

	if (IsDriveAvailable(nDrive))
	{
		sRoot = GetRoot(nDrive);
	
		::GetDiskFreeSpace(sRoot, &sectors, &bytes, &freeClusters, &totalClusters);
	
		// do maths like this to avoid truncation
		// errors
		float fSpace = (float)sectors;
		fSpace *= bytes;
		fSpace *= freeClusters;
		fSpace /= 1048576;

		return fSpace;
	}
	
	return 0.0f;
}

CString CDriveInfo::GetVolume(int nDrive)
{
	CString sVolume, sRoot;

	ASSERT (nDrive > 0 && nDrive <= 26);

	if (IsDriveAvailable(nDrive))
	{
		sRoot = GetRoot(nDrive);

		GetVolumeInformation(sRoot, sVolume.GetBuffer(20), 20, NULL, NULL, NULL, NULL, 0);
		sVolume.ReleaseBuffer();
		FormatName(sVolume);

		return sVolume;
	}

	return _T("");
}

CString CDriveInfo::GetRoot(int nDrive)
{
	CString sRoot;

	sRoot = GetLetter(nDrive);
	sRoot += _T(":\\");

	return sRoot;
}

CString CDriveInfo::GetFullName(int nDrive)
{
	CString sFullName, sLetter, sVolume;

	ASSERT (nDrive > 0 && nDrive <= 26);

	if (IsDriveAvailable(nDrive))
	{
		sLetter = GetLetter(nDrive);
		sVolume = GetVolume(nDrive);

		sFullName = (sVolume + _T(" (") + sLetter + _T(":)"));

		return sFullName;
	}

	return _T("");
}

float CDriveInfo::GetTotalBytes(int nDrive)
{
	CString sRoot;
	unsigned long totalClusters, freeClusters, sectors, bytes;

	ASSERT (nDrive > 0 && nDrive <= 26);

	if (IsDriveAvailable(nDrive))
	{
		sRoot = GetRoot(nDrive);
	
		::GetDiskFreeSpace(sRoot, &sectors, &bytes, &freeClusters, &totalClusters);
	
		// do maths like this to avoid truncation
		// errors
		float fSpace = (float)sectors;
		fSpace *= bytes;
		fSpace *= totalClusters;
		fSpace /= 1048576;

		return fSpace;
	}
	
	return 0.0f;
}

char CDriveInfo::GetLetter(int nDrive)
{
	ASSERT (nDrive > 0 && nDrive <= 26);

	return (char)(nDrive + 'A' - 1);
}

int CDriveInfo::GetType(int nDrive)
{
	CString sVolume;

	ASSERT (nDrive > 0 && nDrive <= 26);

	// shortcut to avoid floppy access
	if (nDrive ==1 || nDrive == 2)
		return DRIVE_REMOVABLE;

	return ::GetDriveType(GetRoot(nDrive));
}

int CDriveInfo::GetPathType(LPCTSTR szPathName) 
{ 
    int nDrive = GetDrive(szPathName);

	if (nDrive >= 0)
		return GetType(nDrive);

	else if (IsRemotePath(szPathName) > 0)
		return DRIVE_REMOTE;

	else
	    return DRIVE_UNKNOWN; 
}

void CDriveInfo::FormatName(CString& sFilename) 
{
	CString sTemp, sChar;
	int nLen, nChar;
	TCHAR cChar, cLastChar = ' '; // space 

	// this function accepts pathnames and names with spaces
	sFilename.MakeLower();
	nLen = sFilename.GetLength();

	// for each word make the first letter upper case
	for (nChar = 0; nChar < nLen; nChar++)
	{
		cChar = sFilename[nChar];

		if (cLastChar == ' ' || cLastChar == '\\')
		{
			sChar = CString(cChar);
			sChar.MakeUpper();
			cChar = sChar[0];
		}

		sTemp += cChar;
		cLastChar = cChar;
	}

	sFilename = sTemp;
}

BOOL CDriveInfo::IsDriveAvailable(int nDrive)
{
	int nCurDrive;
	int nRes;

	// save cur drive and try to change to drive
	nCurDrive = _getdrive();
	nRes = _chdrive(nDrive);

	// if change successful change back before return
	if (nRes == 0)
		_chdrive(nCurDrive);

	return (nRes == 0) ? TRUE : FALSE;
}

BOOL CDriveInfo::IsUNCPath(LPCTSTR szPathName)
{
	return (_tcsstr(szPathName, _T("\\\\")) == szPathName);
}

int CDriveInfo::GetDrive(LPCTSTR szPathName)
{
	int nDrive = 0;
	
	if (_tcsstr(szPathName, _T(":")) == szPathName + 1)
	{
		TCHAR cDrive = szPathName[0];
		cDrive = (TCHAR)toupper(cDrive);
		nDrive = cDrive - 64;
		
	}
	
	return nDrive ? nDrive : -1;
}

BOOL CDriveInfo::IsMappedPath(LPCTSTR szPathName)
{
	int nDrive;

	nDrive = GetDrive(szPathName);

	if (nDrive <= 0)
		return FALSE;

	return (GetType(nDrive) == DRIVE_REMOTE);
}

BOOL CDriveInfo::IsRemotePath(LPCTSTR szPathName, BOOL bAllowFileCheck)
{
	if (bAllowFileCheck)
	{
		DWORD dwAttr = ::GetFileAttributes(szPathName);
		
		if (dwAttr == 0xffffffff)
			return -1;
	}
	
	return (IsUNCPath(szPathName) || IsMappedPath(szPathName));
}

BOOL CDriveInfo::IsFixedPath(LPCTSTR szPathName)
{
	int nDrive = GetDrive(szPathName);

	if (nDrive == -1) // unknown
		return FALSE;

	return (GetType(nDrive) == DRIVE_FIXED);
}

BOOL CDriveInfo::IsRemovablePath(LPCTSTR szPathName)
{
	int nDrive = GetDrive(szPathName);

	if (nDrive == -1) // unknown
		return FALSE;

	return (GetType(nDrive) == DRIVE_REMOVABLE);
}

BOOL CDriveInfo::IsReadonlyPath(LPCTSTR szPathName)
{
	DWORD dwAttr = ::GetFileAttributes(szPathName);

	if (dwAttr == 0xffffffff)
		return -1;

	// else
	return (dwAttr & FILE_ATTRIBUTE_READONLY);
}

DWORD CDriveInfo::GetSerialNumber(int nDrive)
{
	if (GetType(nDrive) != DRIVE_FIXED)
		return 0;

	DWORD dwHDSerialNum = 0;

	if (!GetVolumeInformation(GetRoot(nDrive), NULL, 0, &dwHDSerialNum, NULL, NULL, NULL, 0))
		return (DWORD)-1;

	return dwHDSerialNum;
}
