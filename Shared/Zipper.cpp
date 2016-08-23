// Zipper.cpp: implementation of the CZipper class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <tchar.h>
#include "Zipper.h"

#include "..\3rdparty\zlib\zip.h"
#include "..\3rdparty\zlib\iowin32.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const UINT BUFFERSIZE = 2048;

CZipper::CZipper(LPCTSTR szFilePath, LPCTSTR szRootFolder, bool bAppend) : m_uzFile(0)
{
	CloseZip();
	ZeroMemory(&m_info, sizeof(m_info));

	if (szFilePath)
		OpenZip(szFilePath, szRootFolder, bAppend);
}

CZipper::~CZipper()
{
	CloseZip();
}

bool CZipper::CloseZip()
{
	int nRet = m_uzFile ? zipClose(m_uzFile, NULL) : ZIP_OK;

	m_uzFile = NULL;
	m_szRootFolder[0] = 0;
	// note: we don't clear m_info until the object is re-used or deleted

	return (nRet == ZIP_OK);
}

void CZipper::GetFileInfo(Z_FileInfo& info)
{
	info = m_info;
}

// simple interface
bool CZipper::ZipFile(LPCTSTR szFilePath)
{
	// make zip path
	TCHAR szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szName[_MAX_FNAME];
	_tsplitpath(szFilePath, szDrive, szFolder, szName, NULL);

	TCHAR szZipPath[MAX_PATH];
	_tmakepath(szZipPath, szDrive, szFolder, szName, _T("zip"));

	CZipper zip;

	if (zip.OpenZip(szZipPath, false))
		return zip.AddFileToZip(szFilePath, false);

	return false;
}
	
bool CZipper::ZipFolder(LPCTSTR szFilePath, bool bIgnoreFilePath)
{
	// make zip path
	TCHAR szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szName[_MAX_FNAME], szExt[_MAX_EXT];
	_tsplitpath(szFilePath, szDrive, szFolder, szName, szExt);

	lstrcat(szName, szExt); // save extension if any

	// set root path to include the folder name
	TCHAR szRootPath[MAX_PATH];
	_tmakepath(szRootPath, szDrive, szFolder, szName, NULL);

	TCHAR szZipPath[MAX_PATH];
	_tmakepath(szZipPath, szDrive, szFolder, szName, _T("zip"));

	CZipper zip;

	if (zip.OpenZip(szZipPath, szRootPath, false))
		return zip.AddFolderToZip(szFilePath, bIgnoreFilePath);

	return false;
}
	
// works with prior opened zip
bool CZipper::AddFileToZip(LPCTSTR szFilePath, bool bIgnoreFilePath)
{
	if (!m_uzFile)
		return false;

	// we don't allow paths beginning with '..\' because this would be outside
	// the root folder
	if (!bIgnoreFilePath && _tcsstr(szFilePath, _T("..\\")) == szFilePath)
		return false;

	bool bFullPath = (_tcschr(szFilePath, ':') != NULL);

	// if the file is relative then we need to append the root before opening
	TCHAR szFullFilePath[MAX_PATH];
	
	lstrcpy(szFullFilePath, szFilePath);
	PrepareSourcePath(szFullFilePath);

	// if the file is a fullpath then remove the root path bit
	TCHAR szFileName[MAX_PATH] = _T("");

	if (bIgnoreFilePath)
	{
		TCHAR szName[_MAX_FNAME], szExt[_MAX_EXT];
		_tsplitpath(szFilePath, NULL, NULL, szName, szExt);

		_tmakepath(szFileName, NULL, NULL, szName, szExt);
	}
	else if (bFullPath)
	{
		// check the root can be found
		if (0 != _tcsncicmp(szFilePath, m_szRootFolder, lstrlen(m_szRootFolder)))
			return false;

		// else
		lstrcpy(szFileName, szFilePath + lstrlen(m_szRootFolder));
	}
	else // relative path
	{
		// if the path begins with '.\' then remove it
		if (_tcsstr(szFilePath, _T(".\\")) == szFilePath)
			lstrcpy(szFileName, szFilePath + 2);
		else
			lstrcpy(szFileName, szFilePath);
	}

	PrepareZipPath(szFileName);

	// save file attributes
	zip_fileinfo zfi;

	zfi.internal_fa = 0;
	zfi.external_fa = (::GetFileAttributes(szFilePath) & ~FILE_ATTRIBUTE_READONLY);
	
	// save file time
	SYSTEMTIME st;

	GetLastModified(szFullFilePath, st);

	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;
	
	// load input file
	HANDLE hInputFile = ::CreateFile(szFullFilePath, 
									GENERIC_READ,
									0,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_READONLY,
									NULL);

	if (hInputFile == INVALID_HANDLE_VALUE)
		return false;

	// convert unicode to ansi as required
#ifdef _UNICODE
	char szAnsiPath[MAX_PATH] = { 0 };
	
	::WideCharToMultiByte(CP_ACP, 0, szFileName, lstrlen(szFileName), szAnsiPath, MAX_PATH, NULL, NULL);
	const char* pPath = szAnsiPath;
#else
	const char* pPath = szFileName;
#endif

	int nRet = zipOpenNewFileInZip(m_uzFile, 
									pPath,
									&zfi, 
									NULL, 
									0,
									NULL,
									0, 
									NULL,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION);

	if (nRet == ZIP_OK)
	{
		m_info.nFileCount++;

		// read the file and output to zip
		TCHAR pBuffer[BUFFERSIZE];
		DWORD dwBytesRead = 0, dwFileSize = 0;

		while (nRet == ZIP_OK && ::ReadFile(hInputFile, pBuffer, BUFFERSIZE, &dwBytesRead, NULL))
		{
			dwFileSize += dwBytesRead;

			if (dwBytesRead)
				nRet = zipWriteInFileInZip(m_uzFile, pBuffer, dwBytesRead);
			else
				break;
		}

		m_info.dwUncompressedSize += dwFileSize;
	}

	zipCloseFileInZip(m_uzFile);
	::CloseHandle(hInputFile);

	return (nRet == ZIP_OK);
}

bool CZipper::AddFileToZip(LPCTSTR szFilePath, LPCTSTR szRelFolderPath)
{
	if (!m_uzFile)
		return false;

	// szRelFolderPath cannot contain drive info
	if (szRelFolderPath && _tcschr(szRelFolderPath, ':'))
		return false;

	// if the file is relative then we need to append the root before opening
	TCHAR szFullFilePath[MAX_PATH];
	
	lstrcpy(szFullFilePath, szFilePath);
	PrepareSourcePath(szFullFilePath);

	// save file attributes and time
	zip_fileinfo zfi;

	zfi.internal_fa = 0;
	zfi.external_fa = ::GetFileAttributes(szFilePath);
	
	// save file time
	SYSTEMTIME st;

	GetLastModified(szFullFilePath, st);

	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;

	// load input file
	HANDLE hInputFile = ::CreateFile(szFullFilePath, 
									GENERIC_READ,
									0,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_READONLY,
									NULL);

	if (hInputFile == INVALID_HANDLE_VALUE)
		return false;

	// strip drive info off filepath
	TCHAR szName[_MAX_FNAME], szExt[_MAX_EXT];
	_tsplitpath(szFilePath, NULL, NULL, szName, szExt);

	// prepend new folder path 
	TCHAR szFileName[MAX_PATH];
	_tmakepath(szFileName, NULL, szRelFolderPath, szName, szExt);

	PrepareZipPath(szFileName);
	// convert unicode to ansi as required

#ifdef _UNICODE
	char szAnsiPath[MAX_PATH] = { 0 };
	
	::WideCharToMultiByte(CP_ACP, 0, szFileName, lstrlen(szFileName), szAnsiPath, MAX_PATH, NULL, NULL);
	const char* pPath = szAnsiPath;
#else
	const char* pPath = szFileName;
#endif

	// open the file in the zip making sure we remove any leading '\'
	int nRet = zipOpenNewFileInZip(m_uzFile, 
									pPath,
									&zfi, 
									NULL, 
									0,
									NULL,
									0, 
									NULL,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION);

	if (nRet == ZIP_OK)
	{
		m_info.nFileCount++;

		// read the file and output to zip
		TCHAR pBuffer[BUFFERSIZE];
		DWORD dwBytesRead = 0, dwFileSize = 0;

		while (nRet == ZIP_OK && ::ReadFile(hInputFile, pBuffer, BUFFERSIZE, &dwBytesRead, NULL))
		{
			dwFileSize += dwBytesRead;

			if (dwBytesRead)
				nRet = zipWriteInFileInZip(m_uzFile, pBuffer, dwBytesRead);
			else
				break;
		}

		m_info.dwUncompressedSize += dwFileSize;
	}

	zipCloseFileInZip(m_uzFile);
	::CloseHandle(hInputFile);

	return (nRet == ZIP_OK);
}

bool CZipper::AddFileToZip(const BYTE* pFileContents, int nSize, LPCTSTR szRelFilePath)
{
	if (!m_uzFile)
		return false;

	// szRelFilePath cannot contain drive info
	if (szRelFilePath && _tcschr(szRelFilePath, ':'))
		return false;

	// save file attributes and time
	zip_fileinfo zfi;

	zfi.internal_fa = 0;
	zfi.external_fa = FILE_ATTRIBUTE_NORMAL;
	
	// use time now
	SYSTEMTIME st;

	GetSystemTime(&st);

	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;

	// open the file in the zip making sure we remove any leading '\'
	TCHAR szFilePath[MAX_PATH];

	lstrcpy(szFilePath, szRelFilePath);
	PrepareZipPath(szFilePath);

	// convert unicode to ansi as required
#ifdef _UNICODE
	char szAnsiPath[MAX_PATH] = { 0 };
	
	::WideCharToMultiByte(CP_ACP, 0, szFilePath, lstrlen(szFilePath), szAnsiPath, MAX_PATH, NULL, NULL);
	const char* pPath = szAnsiPath;
#else
	const char* pPath = szFilePath;
#endif

	int nRet = zipOpenNewFileInZip(m_uzFile, 
									pPath,
									&zfi, 
									NULL, 
									0,
									NULL,
									0, 
									NULL,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION);

	if (nRet == ZIP_OK)
	{
		m_info.nFileCount++;

		// read the file and output to zip
		const BYTE* pBuf = pFileContents;

		do
		{
			DWORD dwBytesRead = min(BUFFERSIZE, pFileContents + nSize - pBuf);
			nRet = zipWriteInFileInZip(m_uzFile, pBuf, dwBytesRead);

			pBuf += dwBytesRead;
		}
		while (nRet == ZIP_OK && pBuf < pFileContents + nSize);

		m_info.dwUncompressedSize += nSize;
	}

	zipCloseFileInZip(m_uzFile);

	return (nRet == ZIP_OK);
}

bool CZipper::AddFolderToZip(LPCTSTR szFolderPath, bool bIgnoreFilePath)
{
	if (!m_uzFile)
		return false;

	m_info.nFolderCount++;

	// if the path is relative then we need to append the root before opening
	TCHAR szFullPath[MAX_PATH];
	
	lstrcpy(szFullPath, szFolderPath);
	PrepareSourcePath(szFullPath);

	// always add folder first
	// save file attributes
	zip_fileinfo zfi;
	
	zfi.internal_fa = 0;
	zfi.external_fa = ::GetFileAttributes(szFullPath);
	
	SYSTEMTIME st;
	
	GetLastModified(szFullPath, st);
	
	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;
	
	// if the folder is a fullpath then remove the root path bit
	TCHAR szFolderName[MAX_PATH] = { 0 };
	
	if (bIgnoreFilePath)
	{
		TCHAR szExt[_MAX_EXT];
		_tsplitpath(szFullPath, NULL, NULL, szFolderName, szExt);

		lstrcat(szFolderName, szExt);
	}
	else
	{
		// check the root can be found
		if (0 != _tcsncicmp(szFullPath, m_szRootFolder, lstrlen(m_szRootFolder)))
			return false;
		
		// else
		lstrcpy(szFolderName, szFullPath + lstrlen(m_szRootFolder));
	}
	
	// open the file in the zip making sure we remove any leading '\'
	// provided that the folder name is not empty.
	// note: its ok for it to be empty if this folder coincides with the root folder
	PrepareZipPath(szFolderName);

	if (lstrlen(szFolderName))
	{
		// convert unicode to ansi as required
#ifdef _UNICODE
		char szAnsiPath[MAX_PATH] = { 0 };
		
		::WideCharToMultiByte(CP_ACP, 0, szFolderName, lstrlen(szFolderName), szAnsiPath, MAX_PATH, NULL, NULL);
		const char* pPath = szAnsiPath;
#else
		const char* pPath = szFolderName;
#endif

		int nRet = zipOpenNewFileInZip(m_uzFile, 
										pPath,
										&zfi, 
										NULL, 
										0,
										NULL,
										0, 
										NULL,
										Z_DEFLATED,
										Z_DEFAULT_COMPRESSION);
		
		zipCloseFileInZip(m_uzFile);
	}

	// build searchspec
	TCHAR szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szName[_MAX_FNAME], szExt[_MAX_EXT];
	_tsplitpath(szFullPath, szDrive, szFolder, szName, szExt);

	lstrcat(szFolder, szName);
	lstrcat(szFolder, szExt);

	TCHAR szSearchSpec[MAX_PATH];
	_tmakepath(szSearchSpec, szDrive, szFolder, _T("*"), _T("*"));

	WIN32_FIND_DATA finfo;
	HANDLE hSearch = FindFirstFile(szSearchSpec, &finfo);

	if (hSearch != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if (finfo.cFileName[0] != '.') 
			{
				TCHAR szItem[MAX_PATH];
				_tmakepath(szItem, szDrive, szFolder, finfo.cFileName, NULL);
				
				if (finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					AddFolderToZip(szItem, bIgnoreFilePath);
				}
				else 
					AddFileToZip(szItem, bIgnoreFilePath);
			}
		} 
		while (FindNextFile(hSearch, &finfo));
		
		FindClose(hSearch);
	}

	return TRUE;
}

// extended interface
bool CZipper::OpenZip(LPCTSTR szFilePath, LPCTSTR szRootFolder, bool bAppend)
{
	CloseZip();
	ZeroMemory(&m_info, sizeof(m_info));

	if (!szFilePath || !lstrlen(szFilePath))
		return false;

	// convert szFilePath to fully qualified path 
	TCHAR szFullPath[MAX_PATH];

	if (!GetFullPathName(szFilePath, MAX_PATH, szFullPath, NULL))
		return false;

	// zipOpen will fail if bAppend is TRUE and zip does not exist
	if (bAppend && ::GetFileAttributes(szFullPath) == 0xffffffff)
		bAppend = false;

	// convert unicode to ansi as required
#ifdef _UNICODE
	char szAnsiPath[MAX_PATH] = { 0 };
	
	::WideCharToMultiByte(CP_ACP, 0, szFullPath, lstrlen(szFullPath), szAnsiPath, MAX_PATH, NULL, NULL);
	const char* pPath = szAnsiPath;
#else
	const char* pPath = szFullPath;
#endif
	
	m_uzFile = zipOpen(pPath, bAppend ? APPEND_STATUS_ADDINZIP : APPEND_STATUS_CREATE);

	if (m_uzFile)
	{
		if (!szRootFolder)
		{
			TCHAR szDrive[_MAX_DRIVE], szFolder[MAX_PATH];
			_tsplitpath(szFullPath, szDrive, szFolder, NULL, NULL);

			_tmakepath(m_szRootFolder, szDrive, szFolder, NULL, NULL);
		}
		else if (lstrlen(szRootFolder))
		{
			_tmakepath(m_szRootFolder, NULL, szRootFolder, NULL, NULL);
		}

		// remove any trailing whitespace and '\'
		UnterminatePath(m_szRootFolder);
	}

	return (m_uzFile != NULL);
}

void CZipper::UnterminatePath(LPTSTR szPath)
{
	int nEnd = lstrlen(szPath) - 1;

	while (isspace(szPath[nEnd]))
		nEnd--;

	while (szPath[nEnd] == '\\')
		nEnd--;

	szPath[nEnd + 1] = 0;
}

void CZipper::PrepareZipPath(LPTSTR szPath)
{
	UnterminatePath(szPath);

	// remove leading whitespace and '\'
	TCHAR szTemp[MAX_PATH];
	lstrcpy(szTemp, szPath);

	int nStart = 0;

	while (isspace(szTemp[nStart]))
		nStart++;

	while (szTemp[nStart] == '\\')
		nStart++;

	if (nStart)
		lstrcpy(szPath, szTemp + nStart);
}

void CZipper::PrepareSourcePath(LPTSTR szPath)
{
	bool bFullPath = (_tcschr(szPath, ':') != NULL);

	// if the file is relative then we need to append the root before opening
	if (!bFullPath)
	{
		TCHAR szTemp[MAX_PATH];
		lstrcpy(szTemp, szPath);

		_tmakepath(szPath, NULL, m_szRootFolder, szTemp, NULL);
	}
}

bool CZipper::GetLastModified(LPCTSTR szPath, SYSTEMTIME& sysTime)
{
	ZeroMemory(&sysTime, sizeof(SYSTEMTIME));

	DWORD dwAttr = ::GetFileAttributes(szPath);

	// files only
	if (dwAttr == 0xFFFFFFFF)
		return false;

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((LPTSTR)szPath, &findFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return false;

	FindClose(hFind);

	FILETIME ft = findFileData.ftLastWriteTime;

	FileTimeToLocalFileTime(&findFileData.ftLastWriteTime, &ft);
	FileTimeToSystemTime(&ft, &sysTime);

	return true;
}


