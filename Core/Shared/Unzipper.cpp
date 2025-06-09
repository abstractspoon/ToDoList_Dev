// Unzipper.cpp: implementation of the CUnzipper class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Unzipper.h"

#include "..\3rdParty\Zlib\unzip.h"
#include "..\3rdParty\zlib\iowin32.h"

#pragma comment(lib, "..\\3rdparty\\zlib\\zlibstat.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1400
#	define zsplitpath(path, drive, folder, fname, ext) _tsplitpath_s(path, drive, (drive ? _MAX_DRIVE : 0), folder, (folder ? _MAX_DIR : 0), fname, (fname ? _MAX_FNAME : 0), ext, (ext ? _MAX_EXT : 0))
#	define zmakepath(path, drive, folder, fname, ext) _tmakepath_s(path, _MAX_PATH, drive, folder, fname, ext)
#else
#	define zsplitpath(path, drive, folder, fname, ext) _tsplitpath(path, drive, folder, fname, ext)
#	define zmakepath(path, drive, folder, fname, ext) _tmakepath(path, drive, folder, fname, ext)
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const UINT BUFFERSIZE = 2048;

CUnzipper::CUnzipper(LPCTSTR szFilePath, PFNUNZIPPROGRESS pCallback, DWORD dwUserData) 
	: 
	m_uzFile(0),
	m_pCBProgress(pCallback),
	m_dwCBUserData(dwUserData),
	m_nFileCount(-1),
	m_nCurFile(0)
{
	OpenZip(szFilePath);
}

CUnzipper::~CUnzipper()
{
	CloseZip();
}

bool CUnzipper::CloseZip()
{
	unzCloseCurrentFile(m_uzFile);

	int nRet = unzClose(m_uzFile);
	m_uzFile = NULL;
	m_szOutputFolder[0] = 0;

	return (nRet == UNZ_OK);
}

// simple interface
bool CUnzipper::Unzip(bool bIgnoreFilePath)
{
	if (!m_uzFile)
		return false;

	return UnzipTo(m_szOutputFolder, bIgnoreFilePath);
}

bool CUnzipper::UnzipTo(LPCTSTR szFolder, bool bIgnoreFilePath)
{
	if (!m_uzFile)
		return false;

	if (!szFolder || !CreateFolder(szFolder))
		return false;

	if (GetFileCount() == 0)
		return false;

	if (!GotoFirstFile())
		return false;

	// else
	do
	{
		if (!UnzipFile(szFolder, bIgnoreFilePath))
			return false;
	}
	while (GotoNextFile());
	
	return true;
}

// static version
bool CUnzipper::Unzip(LPCTSTR szFileName, LPCTSTR szFolder, bool bIgnoreFilePath)
{
	CUnzipper unz;

	if (!unz.OpenZip(szFileName))
		return false;

	return unz.UnzipTo(szFolder, bIgnoreFilePath);
}

// extended interface
bool CUnzipper::OpenZip(LPCTSTR szFilePath)
{
	CloseZip();

	if (!szFilePath || !lstrlen(szFilePath))
		return false;

	// convert szFilePath to fully qualified path 
	TCHAR szFullPath[MAX_PATH];
	
	if (!GetFullPathName(szFilePath, MAX_PATH, szFullPath, NULL))
		return false;

	// Path names must be ANSI
	char szAnsiPath[MAX_PATH] = { 0 };
	::WideCharToMultiByte(CP_ACP, 0, szFullPath, lstrlen(szFullPath), szAnsiPath, MAX_PATH, NULL, NULL);

	m_nFileCount = -1;
	m_nCurFile = 0;

	m_uzFile = unzOpen(szAnsiPath);
	
	if (m_uzFile)
	{
		TCHAR szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szFName[_MAX_FNAME];
		
		zsplitpath(szFullPath, szDrive, szFolder, szFName, NULL);
		zmakepath(m_szOutputFolder, szDrive, szFolder, szFName, NULL);
	}

	return (m_uzFile != NULL);
}

bool CUnzipper::SetOutputFolder(LPCTSTR szFolder)
{
	DWORD dwAttrib = GetFileAttributes(szFolder);

	if (dwAttrib != 0xffffffff && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
		return false;

	lstrcpy(m_szOutputFolder, szFolder);

	return CreateFolder(szFolder);
}

int CUnzipper::GetFileCount()
{
	if (!m_uzFile)
		return 0;

	if (m_nFileCount == -1)
	{
		unz_global_info info;

		if (unzGetGlobalInfo(m_uzFile, &info) == UNZ_OK)
			m_nFileCount = (int)info.number_entry;
	}

	return ((m_nFileCount == -1) ? 0 : m_nFileCount);
}

bool CUnzipper::GetFileInfo(int nFile, UZ_FileInfo& info)
{
	if (!m_uzFile)
		return false;

	if (!GotoFile(nFile))
		return false;

	return GetFileInfo(info);
}

bool CUnzipper::UnzipFile(int nFile, LPCTSTR szFolder, bool bIgnoreFilePath)
{
	if (!m_uzFile)
		return false;

	if (!szFolder)
		szFolder = m_szOutputFolder;

	if (!GotoFile(nFile))
		return false;

	return UnzipFile(szFolder, bIgnoreFilePath);
}

bool CUnzipper::GotoFirstFile(LPCTSTR szExt)
{
	if (!m_uzFile)
		return false;

	m_nCurFile = 0;

	if (!szExt || !lstrlen(szExt))
		return (unzGoToFirstFile(m_uzFile) == UNZ_OK);

	// else
	if (unzGoToFirstFile(m_uzFile) == UNZ_OK)
	{
		UZ_FileInfo info;

		if (!GetFileInfo(info))
			return false;

		// test extension
		TCHAR szFExt[_MAX_EXT];
		zsplitpath(info.szFileName, NULL, NULL, NULL, szFExt);

		if (szFExt[0])
		{
			if (lstrcmpi(szExt, szFExt + 1) == 0)
				return true;
		}

		return GotoNextFile(szExt);
	}

	return false;
}

bool CUnzipper::GotoNextFile(LPCTSTR szExt)
{
	if (!m_uzFile)
		return false;

	if (!szExt || !lstrlen(szExt))
	{
		if (unzGoToNextFile(m_uzFile) != UNZ_OK)
			return false;

		m_nCurFile++;
		return true;
	}

	// else
	UZ_FileInfo info;

	while (unzGoToNextFile(m_uzFile) == UNZ_OK)
	{
		m_nCurFile++;

		if (!GetFileInfo(info))
			return false;

		// test extension
		TCHAR szFExt[_MAX_EXT];
		zsplitpath(info.szFileName, NULL, NULL, NULL, szFExt);

		if (szFExt[0])
		{
			if (lstrcmpi(szExt, szFExt + 1) == 0)
				return true;
		}
	}

	return false;

}

bool CUnzipper::GetFileInfo(UZ_FileInfo& info)
{
	if (!m_uzFile)
		return false;

	unz_file_info uzfi;

	ZeroMemory(&info, sizeof(info));
	ZeroMemory(&uzfi, sizeof(uzfi));

	char szAnsiComment[MAX_COMMENT+1] = { 0 };
	char szAnsiPath[MAX_PATH] = { 0 };

	if (UNZ_OK != unzGetCurrentFileInfo(m_uzFile, &uzfi, szAnsiPath, MAX_PATH, NULL, 0, szAnsiComment, MAX_COMMENT))
		return false;

	// copy across
	info.dwVersion = uzfi.version;	
	info.dwVersionNeeded = uzfi.version_needed;
	info.dwFlags = uzfi.flag;	
	info.dwCompressionMethod = uzfi.compression_method; 
	info.dwDosDate = uzfi.dosDate;  
	info.dwCRC = uzfi.crc;	 
	info.dwCompressedSize = uzfi.compressed_size; 
	info.dwUncompressedSize = uzfi.uncompressed_size;
	info.dwInternalAttrib = uzfi.internal_fa; 
	info.dwExternalAttrib = uzfi.external_fa; 

	::MultiByteToWideChar(CP_ACP, 0, szAnsiPath, strlen(szAnsiPath), info.szFileName, MAX_PATH);
	::MultiByteToWideChar(CP_ACP, 0, szAnsiComment, strlen(szAnsiComment), info.szComment, MAX_COMMENT);

	// replace filename forward slashes with backslashes
	int nLen = lstrlen(info.szFileName);

	while (nLen--)
	{
		if (info.szFileName[nLen] == '/')
			info.szFileName[nLen] = '\\';
	}

	// is it a folder?
	info.bFolder = ((info.dwExternalAttrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);

	return true;
}

bool CUnzipper::UnzipFile(LPCTSTR szFolder, bool bIgnoreFilePath)
{
	if (!m_uzFile)
		return false;

	if (!szFolder)
		szFolder = m_szOutputFolder;

	if (!CreateFolder(szFolder))
		return false;

	UZ_FileInfo info;
	GetFileInfo(info);

	// if the item is a folder then create it and return 'true'
	if (info.bFolder)
	{
		TCHAR szFolderPath[MAX_PATH];
		zmakepath(szFolderPath, NULL, szFolder, info.szFileName, NULL);

		return CreateFolder(szFolderPath);
	}

	// build the output filename
	TCHAR szFilePath[MAX_PATH];

	if (bIgnoreFilePath)
	{
		TCHAR szFName[_MAX_FNAME], szExt[_MAX_EXT];

		zsplitpath(info.szFileName, NULL, NULL, szFName, szExt);
		zmakepath(info.szFileName, NULL, NULL, szFName, szExt);
	}

	zmakepath(szFilePath, NULL, szFolder, info.szFileName, NULL);

	// open the input and output files
	if (!CreateFilePath(szFilePath))
		return false;

	HANDLE hOutputFile = ::CreateFile(szFilePath, 
										GENERIC_WRITE,
										0,
										NULL,
										CREATE_ALWAYS,
										FILE_ATTRIBUTE_NORMAL,
										NULL);

	if (INVALID_HANDLE_VALUE == hOutputFile)
		return false;

	if (unzOpenCurrentFile(m_uzFile) != UNZ_OK)
	{
		CloseHandle(hOutputFile);
		return false;
	}

	// read the file and output
	int nRet = UNZ_OK;
	TCHAR pBuffer[BUFFERSIZE];

	do
	{
		nRet = unzReadCurrentFile(m_uzFile, pBuffer, BUFFERSIZE);

		if (nRet > 0)
		{
			// output
			DWORD dwBytesWritten = 0;

			if (!::WriteFile(hOutputFile, pBuffer, nRet, &dwBytesWritten, NULL) ||
				dwBytesWritten != (DWORD)nRet)
			{
				nRet = UNZ_ERRNO;
				break;
			}
		}
	}
	while (nRet > 0);

	CloseHandle(hOutputFile);
	unzCloseCurrentFile(m_uzFile);

	if (nRet == UNZ_OK)
	{
		SetFileModTime(szFilePath, info.dwDosDate);
		SetFileAttributes(szFilePath, info.dwExternalAttrib);

		if (!CheckUpdateProgress())
			return false;
	}

	return (nRet == UNZ_OK);
}

bool CUnzipper::CheckUpdateProgress()
{
	if (m_pCBProgress)
	{
		int nNumFiles = GetFileCount();

		int nPrevPercent = (((m_nCurFile - 1) * 100) / nNumFiles);
		int nCurPercent = ((m_nCurFile * 100) / nNumFiles);

		if (nCurPercent != nPrevPercent)
			return m_pCBProgress(nCurPercent, m_dwCBUserData);
	}

	return true;
}

bool CUnzipper::GotoFile(int nFile)
{
	if (!m_uzFile)
		return false;

	if (nFile < 0 || nFile >= GetFileCount())
		return false;

	GotoFirstFile();

	while (nFile--)
	{
		if (!GotoNextFile())
			return false;
	}
	ASSERT(m_nCurFile == nFile);

	return true;
}

bool CUnzipper::GotoFile(LPCTSTR szFileName, bool bIgnoreFilePath)
{
	if (!m_uzFile)
		return false;

	if (m_pCBProgress)
	{
		// We disallow this method if a callback has been
		// specified because there's no way to back-calculate
		// the current file index 
		ASSERT(0);
		return false;
	}

	// try the simple approach
	char szAnsiPath[MAX_PATH] = { 0 };
	::WideCharToMultiByte(CP_ACP, 0, szFileName, lstrlen(szFileName), szAnsiPath, MAX_PATH, NULL, NULL);

	if (unzLocateFile(m_uzFile, szAnsiPath, 2) == UNZ_OK)
	{
		return true;
	}
	else if (bIgnoreFilePath)
	{ 
		// brute force way
		if (unzGoToFirstFile(m_uzFile) != UNZ_OK)
			return false;

		UZ_FileInfo info;

		do
		{
			if (!GetFileInfo(info))
				return false;

			// test name
			TCHAR szFName[_MAX_FNAME], szName[_MAX_FNAME], szExt[_MAX_EXT];

			zsplitpath(info.szFileName, NULL, NULL, szName, szExt);
			zmakepath(szFName, NULL, NULL, szName, szExt);

			if (lstrcmpi(szFileName, szFName) == 0)
				return true;
		}
		while (unzGoToNextFile(m_uzFile) == UNZ_OK);
	}

	// else
	return false;
}

bool CUnzipper::CreateFolder(LPCTSTR szFolder)
{
	if (!szFolder)
		return false;

	// remove trailing whitespace and '\'
	TCHAR szPath[MAX_PATH];
	lstrcpy(szPath, szFolder);

	int nEnd = lstrlen(szPath) - 1;

	while (isspace(szPath[nEnd]))
		nEnd--;

	while (szPath[nEnd] == '\\')
		nEnd--;

	szPath[nEnd + 1] = 0;

	if (!lstrlen(szPath))
		return false;

	DWORD dwAttrib = GetFileAttributes(szPath);

	// already exists ?
	if (dwAttrib != 0xffffffff)
		return ((dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);

	// recursively create from the top down
	TCHAR* szParent = _tcsdup(szPath);
	TCHAR* p = _tcsrchr(szParent, '\\');

	if (p) 
	{
		// The parent is a dir, not a drive
		*p = '\0';
			
		// if can't create parent
		if (!CreateFolder(szParent))
		{
			free(szParent);
			return false;
		}
		free(szParent);

		if (!::CreateDirectory(szPath, NULL)) 
		{
			if (GetLastError() != ERROR_ALREADY_EXISTS) 
				return false;
		}
	}
	
	return true;
}

bool CUnzipper::CreateFilePath(LPCTSTR szFilePath)
{
	TCHAR* szPath = _tcsdup(szFilePath);
	TCHAR* p = _tcsrchr(szPath,'\\');

	bool bRes = false;

	if (p)
	{
		*p = '\0';

		bRes = CreateFolder(szPath);
	}

	free(szPath);

	return bRes;
}

bool CUnzipper::SetFileModTime(LPCTSTR szFilePath, DWORD dwDosDate)
{
	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (!hFile)
		return false;
	
	FILETIME ftm, ftLocal, ftCreate, ftLastAcc, ftLastWrite;

	bool bRes = (GetFileTime(hFile, &ftCreate, &ftLastAcc, &ftLastWrite) != FALSE);

	if (bRes)
		bRes = (DosDateTimeToFileTime((WORD)(dwDosDate >> 16), (WORD)dwDosDate, &ftLocal) != FALSE);

	if (bRes)
		bRes = (LocalFileTimeToFileTime(&ftLocal, &ftm) != FALSE);

	if (bRes)
		bRes = (SetFileTime(hFile, &ftm, &ftLastAcc, &ftm) != FALSE);

	CloseHandle(hFile);

	return bRes;
}

