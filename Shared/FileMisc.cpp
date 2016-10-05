#include "stdafx.h"
#include "filemisc.h"
#include "fileregister.h"
#include "misc.h"
#include "driveinfo.h"
#include "osversion.h"
#include "clipboard.h"

#include "..\3rdParty\stdiofileex.h"
#include "..\3rdParty\fileversioninfo.h"

#include <sys/utime.h>
#include <sys/stat.h>
#include <io.h>
#include <afxole.h>

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

///////////////////////////////////////////////////////////////////////////////////////////////////

struct TOKEN_ELEVATIONEX
{
	DWORD TokenIsElevated;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

static BOOL s_bLogging = FALSE;
static DWORD s_dwLogFlags = 0;
static CString s_sLogAppDataKey;

///////////////////////////////////////////////////////////////////////////////////////////////////

static LPCTSTR ENDL = _T("\r\n");
static LPCTSTR ALLFILESMASK = _T("*.*");
static CString EMPTYSTRING;

///////////////////////////////////////////////////////////////////////////////////////////////////

CTempFileBackup::CTempFileBackup(const CString& sFile, const CString& sFolder, DWORD dwFlags, const CString& sExt)
	: CFileBackup(sFile, sFolder, dwFlags, sExt)
{
}

CTempFileBackup::~CTempFileBackup()
{
	if (FileMisc::FileExists(m_sBackup))
	{
		if (!::DeleteFile(m_sBackup))
			MoveFileEx(m_sBackup, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CFileBackup::CFileBackup(const CString& sFile, const CString& sFolder, DWORD dwFlags, const CString& sExt)
{
	MakeBackup(sFile, sFolder, dwFlags, sExt);
}

CFileBackup::~CFileBackup()
{
}

BOOL CFileBackup::MakeBackup(const CString& sFile, const CString& sFolder, DWORD dwFlags, const CString& sExt)
{
	ASSERT (m_sFile.IsEmpty() && m_sBackup.IsEmpty());

	if (!m_sFile.IsEmpty() || !m_sBackup.IsEmpty())
		return FALSE;

	if (!FileMisc::FileExists(sFile))
		return FALSE;

	m_sFile = sFile;
	m_sBackup = BuildBackupPath(sFile, sFolder, dwFlags, sExt);
	FileMisc::CreateFolderFromFilePath(m_sBackup);

	if (!FileMisc::CopyFile(m_sFile, m_sBackup, TRUE))
	{
		TRACE(Misc::FormatGetLastError() + '\n');

		// try again in temp folder
		m_sBackup = BuildBackupPath(sFile, FileMisc::GetTempFolder(), dwFlags, sExt);
		FileMisc::CreateFolderFromFilePath(m_sBackup);

		if (!FileMisc::CopyFile(m_sFile, m_sBackup, TRUE))
			return FALSE;
	}

	ASSERT (FileMisc::FileExists(m_sBackup));
	return TRUE;
}

BOOL CFileBackup::RestoreBackup()
{
	ASSERT (!m_sFile.IsEmpty() && !m_sBackup.IsEmpty());

	if (m_sFile.IsEmpty() || m_sBackup.IsEmpty())
		return FALSE;

	return FileMisc::CopyFile(m_sBackup, m_sFile, TRUE);
}

CString CFileBackup::BuildBackupPath(const CString& sFile, const CString& sFolder, DWORD dwFlags, const CString& sExt)
{
	CString sBackup(sFile);
	sBackup.TrimRight();

	// handle folder
	if (!sFolder.IsEmpty())
	{
		CString sDrive, sPath, sFName, sExt;
		FileMisc::SplitPath(sFile, &sDrive, &sPath, &sFName, &sExt);

		if (::PathIsRelative(sFolder))
		{
			sPath += sFolder;
			FileMisc::MakePath(sBackup, sDrive, sPath, sFName, sExt);
		}
		else
			FileMisc::MakePath(sBackup, NULL, sFolder, sFName, sExt);
	}

	// add timestamp before existing file extension
	CString sFExt;
	FileMisc::SplitPath(sBackup, NULL, NULL, NULL, &sFExt);

	if (dwFlags & FBS_TIMESTAMP)
	{
		// use ISO date and 24 hour time so that backups can be sorted 
		// by name in date order
		CString sDate = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d_%H-%M-%S"));
		sFExt = ('.' + sDate + sFExt);
	}

	// and app version
	if (dwFlags & FBS_APPVERSION)
	{
		CString sVersion = FileMisc::GetAppVersion('_');
		sFExt = ('.' + sVersion + sFExt);
	}

	// add extension before existing file extension
	if (sExt.IsEmpty())
	{
		// only add a default extension if not copying to another folder or not adding timestamp
		// or versioninfo else we'd overwrite the existing file which wouldn't achieve much
		if (sFolder.IsEmpty() && (dwFlags == 0))
			sFExt = _T(".bak") + sFExt;
	}
	else
	{
		if (sExt.Find('.') == -1)
			sFExt = '.' + sExt + sFExt;
		else
			sFExt = sExt + sFExt;
	}

	FileMisc::ReplaceExtension(sBackup, sFExt);

	return sBackup;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CScopedLogTime::CScopedLogTime(LPCTSTR szScope) 
	: 
	m_sScope(szScope), 
	m_dwTickStart(GetTickCount())
{
}

CScopedLogTime::~CScopedLogTime()
{
	FileMisc::LogTimeElapsed(m_dwTickStart, m_sScope);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CString& FileMisc::TerminatePath(CString& sPath, BOOL bTerminate)
{
	// always unterminate first
	sPath.TrimRight(_T(" \\/"));

	if (bTerminate)
		sPath += '\\';

	return sPath;
}

CString FileMisc::TerminatePath(LPCTSTR szPath, BOOL bTerminate)
{
	CString sPath(szPath);
	return TerminatePath(sPath, bTerminate);
}

BOOL FileMisc::IsPathTerminated(const CString& sPath)
{
	return (sPath.ReverseFind('\\') == (sPath.GetLength() - 1));
}

CString& FileMisc::ReplaceExtension(CString& sFilePath, LPCTSTR szExt)
{
	CString sDrive, sDir, sFile;

	SplitPath(sFilePath, &sDrive, &sDir, &sFile);
	MakePath(sFilePath, sDrive, sDir, sFile, szExt);

	return sFilePath;
}

CString& FileMisc::RemoveExtension(CString& sFilePath)
{
	return ReplaceExtension(sFilePath, NULL);
}

CString& FileMisc::ValidateFilePath(CString& sFilepath, LPCTSTR szReplace)
{
	Misc::Trim(sFilepath);

	sFilepath.Replace(_T("/"), szReplace);
	sFilepath.Replace(_T("*"), szReplace);
	sFilepath.Replace(_T("?"), szReplace);
	sFilepath.Replace(_T("\""), szReplace);
	sFilepath.Replace(_T("<"), szReplace);
	sFilepath.Replace(_T(">"), szReplace);
	sFilepath.Replace(_T("|"), szReplace);

	// make sure if a colon exists it is the 2nd pos
	int nColon = sFilepath.ReverseFind(':');

	while (nColon != -1 && nColon != 1)
	{
		// replace the colon
		sFilepath = sFilepath.Left(nColon) + szReplace + sFilepath.Mid(nColon + 1);
		nColon = sFilepath.ReverseFind(':');
	}

	return sFilepath;
}

CString& FileMisc::ValidateFileName(CString& sFilename, LPCTSTR szReplace)
{
	sFilename.Replace(_T("\\"), szReplace);
	sFilename.Replace(_T(":"), szReplace);
	
	return ValidateFilePath(sFilename, szReplace);
}

CString FileMisc::GetFileNameFromPath(LPCTSTR szFilepath, BOOL bIncExtension)
{
	CString sFName, sExt;
	SplitPath(szFilepath, NULL, NULL, &sFName, &sExt);

	if (bIncExtension)
		sFName += sExt;
	
	return sFName;
}

time_t FileMisc::GetFileLastModified(LPCTSTR szPath)
{
	time_t tTime = 0;
	FILETIME fileTime = { 0 };
	
	if (GetFileLastModified(szPath, fileTime))
	{
		ULARGE_INTEGER ull = { fileTime.dwLowDateTime, fileTime.dwHighDateTime };
		tTime = (time_t)((ull.QuadPart / 10000000UI64) - 11644473600UI64);
		
#ifdef _DEBUG
		struct _stat st;
		_tstat(szPath, &st);
#endif
	}
	
	return tTime;
}

BOOL FileMisc::GetFileLastModified(LPCTSTR szPath, FILETIME& fileTime)
{
	// files only
	if (!FileExists(szPath))
		return FALSE;
	
	CFileFind ff;

	if (ff.FindFile(szPath))
	{
		ff.FindNextFile();
		ff.GetLastWriteTime(&fileTime);

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL FileMisc::SetFileLastModified(LPCTSTR szPath, const FILETIME& fileTime)
{
	HANDLE hFile; 
	
	hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hFile == INVALID_HANDLE_VALUE) 
		return FALSE;

	BOOL bSuccess = ::SetFileTime(hFile, NULL, NULL, &fileTime);
	::CloseHandle(hFile);

	return bSuccess;
}

BOOL FileMisc::GetFileLastModified(LPCTSTR szPath, SYSTEMTIME& sysTime, BOOL bLocalTime)
{
	FILETIME fileTime = { 0 };
	
	if (!GetFileLastModified(szPath, fileTime))
		return FALSE;
	
	if (bLocalTime)
	{
		FILETIME ftLocal = { 0 };
		
		if (!FileTimeToLocalFileTime(&fileTime, &ftLocal))
			return FALSE;
		
		fileTime = ftLocal;
	}
	
	return FileTimeToSystemTime(&fileTime, &sysTime);
}

BOOL FileMisc::TouchFile(LPCTSTR szPath)
{
	::SetFileAttributes(szPath, FILE_ATTRIBUTE_NORMAL);

	return (_tutime(szPath, NULL) == 0);
}

int FileMisc::FindFiles(const CString& sFolder, CStringArray& aFiles, BOOL bCheckSubFolders, LPCTSTR szPattern)
{
	CFileFind ff;
	CString sSearchSpec;

	MakePath(sSearchSpec, NULL, sFolder, szPattern, NULL); 

	BOOL bContinue = ff.FindFile(sSearchSpec);
	
	while (bContinue)
	{
		bContinue = ff.FindNextFile();

		if (!ff.IsDots())
		{
			CString sPath(ff.GetFilePath());
			
			if (ff.IsDirectory())
			{
				if (bCheckSubFolders)
					FindFiles(sPath, aFiles, TRUE, szPattern);
			}
			else
				aFiles.Add(sPath);
		}
	}

	return aFiles.GetSize();
}

int FileMisc::GetDropFilePaths(HDROP hDrop, CStringArray& aFiles)
{
	int nRet = 0;

	aFiles.RemoveAll();

	TCHAR szFileName[_MAX_PATH+1] = { 0 };
	UINT nFileCount = ::DragQueryFile(hDrop, 0xffffffff, NULL, 0);

	if (nFileCount != 0xffffffff)
	{
		for (UINT i=0; i < nFileCount; i++)
		{
			::DragQueryFile(hDrop, i, szFileName, _MAX_PATH);
			::GetLongPathName(szFileName, szFileName, _MAX_PATH);
			
			aFiles.Add(szFileName);
		}

		nRet = aFiles.GetSize();
	}
	else
	{
		// error
		nRet = -1;
	}

	return nRet;
}

int FileMisc::DeleteFolderContents(LPCTSTR szFolder, DWORD dwFlags, LPCTSTR szFileMask, HANDLE hTerminate)
{
	// if the dir does not exists just return
	if (!FolderExists(szFolder))
		return TRUE;

	BOOL bWantSubFolders = Misc::HasFlag(dwFlags, FMDF_SUBFOLDERS);
	BOOL bWantMsgLoop	 = Misc::HasFlag(dwFlags, FMDF_PROCESSMSGLOOP);
	BOOL bHiddenReadOnly = Misc::HasFlag(dwFlags, FMDF_HIDDENREADONLY);
	BOOL bDeleteAll		 = (Misc::IsEmpty(szFileMask) || _tcsstr(szFileMask, ALLFILESMASK));

	// if a file mask has been specified with subfolders 
	// we need to do multiple passes on each folder, 
	// one (or more) for the files and one for the sub folders
	CStringArray aMasks;
	int nNumMasks = Misc::Split(szFileMask, aMasks, ';');

	int nNumPass = (bWantSubFolders ? (nNumMasks + 1) : max(nNumMasks, 1));
		
	int nResult = 1; // success
	BOOL bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);

	for (int nPass = 0; !bStopped && nPass < nNumPass; nPass++)
	{
		CString sSearchSpec(szFolder), sMask(szFileMask);

		if (sMask.IsEmpty() || (nPass == aMasks.GetSize())) // (nPass == last) == 2nd pass (for folders)
			sMask = ALLFILESMASK;
		else
			sMask = aMasks[nPass];

		TerminatePath(sSearchSpec);
		sSearchSpec += sMask;

		WIN32_FIND_DATA finfo;
		HANDLE hSearch = NULL;

		if ((hSearch = FindFirstFile(sSearchSpec, &finfo)) != INVALID_HANDLE_VALUE) 
		{
			do 
			{
				if (bWantMsgLoop)
					Misc::ProcessMsgLoop();

				if (finfo.cFileName[0] != '.') 
				{
					CString sItem = (TerminatePath(szFolder) + finfo.cFileName);

					// handle hidden/readonly files
					if (bHiddenReadOnly)
						ModifyAttributes(sItem, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN, finfo.dwFileAttributes);

					// handle directories
					BOOL bFolder = Misc::HasFlag(finfo.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY);

					if (bFolder && bWantSubFolders)
					{
						// delete folder too?
						if (bDeleteAll)
						{
							nResult = DeleteFolder(sItem, dwFlags, hTerminate);
						}
						else
						{
							nResult = DeleteFolderContents(sItem, dwFlags, sMask, hTerminate);
						}
					}
					else if (!bFolder) // file
					{
						if (FileMisc::DeleteFile(sItem))
						{
							nResult = 1; // succeeded
						}
						else if (dwFlags & FMDF_ALLOWDELETEONREBOOT)
						{
							if (::MoveFileEx(sItem, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))
							{
								LogText(_T("FileMisc::DeleteFolderContents(%s) delayed!"), sItem);
								nResult = -1; // nDelayed
							}
							else
							{
								LogText(_T("FileMisc::DeleteFolderContents(%s) failed!"), sItem);
								nResult = 0; // failed
							}
						}
						else
							nResult = 0; // failed
					}

					// handle failure
					if (nResult == 0)
						break;
				}

				bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);
			} 
			while (!bStopped && FindNextFile(hSearch, &finfo));
			
			FindClose(hSearch);
		}

		// handle failure
		if (nResult == 0)
			break;
	}

	if (bStopped || (nResult == 0))
		return 0;

	return nResult;
}

int FileMisc::DeleteFolder(LPCTSTR szFolder, DWORD dwFlags, HANDLE hTerminate)
{
	// if the dir does not exists just return
	if (!FolderExists(szFolder))
		return 1;

	int nResult = DeleteFolderContents(szFolder, dwFlags, NULL, hTerminate);

	// handle positive result
	if (nResult != 0)
	{
		// handle delayed deletion
		::SetFileAttributes(szFolder, FILE_ATTRIBUTE_NORMAL);
		
		if (nResult == -1) // some files can only be deleted on reboot
		{
			LogText(_T("FileMisc::DeleteFolder(%s) delayed due to files!"), szFolder);

			if (!::MoveFileEx(szFolder, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))
			{
				LogText(_T("FileMisc::DeleteOnReboot(%s) failed!"), szFolder);
				nResult = 0; // failed
			}
		}
		// all contained files were deleted successfully
		// so we can now try to delete the folder itself
		else
		{
			if (!RemoveDirectory(szFolder))
			{
				DWORD dwErr = GetLastError();
				HRESULT hrErr = HRESULT_FROM_WIN32(dwErr);
				LogText(_T("FileMisc::DeleteFolder(%s) failed! (dw = %08x, hr = %08x"), szFolder, dwErr, hrErr);

				if (dwFlags & FMDF_ALLOWDELETEONREBOOT)
				{
					if (::MoveFileEx(szFolder, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))
					{
						LogText(_T("FileMisc::DeleteFolder(%s) delayed!"), szFolder);
						nResult = -1; // nDelayed
					}
					else
					{
						LogText(_T("FileMisc::DeleteOnReboot(%s) failed!"), szFolder);
						nResult = 0; // failed
					}
				}
			}
		}
	}

	return nResult;
}

double FileMisc::GetFolderSize(LPCTSTR szFolder, BOOL bIncludeSubFolders, LPCTSTR szFileMask, HANDLE hTerminate, BOOL bProcessMsgLoop)
{
	// if the dir does not exists just return
	if (!FolderExists(szFolder))
		return 0;
	
	double dSize = 0;

	WIN32_FIND_DATA finfo;
	CString sSearchSpec(szFolder), sFileMask(szFileMask);

	if (sFileMask.IsEmpty())
		sFileMask = ALLFILESMASK;

	TerminatePath(sSearchSpec);
	sSearchSpec += sFileMask;

	BOOL bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);
	HANDLE h = NULL;
		
	if (!bStopped && (h = FindFirstFile(sSearchSpec, &finfo)) != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if (bProcessMsgLoop)
				Misc::ProcessMsgLoop();

			if (finfo.cFileName[0] != '.') 
			{
				BOOL bFolder = Misc::HasFlag(finfo.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY);

				if (bFolder && bIncludeSubFolders)
				{
					CString sSubFolder = (TerminatePath(szFolder) + finfo.cFileName);
					
					dSize += GetFolderSize(sSubFolder, TRUE, sFileMask, hTerminate, bProcessMsgLoop);
				}
				else if (!bFolder)
				{
					dSize += (finfo.nFileSizeHigh * ((double)MAXDWORD + 1)) + finfo.nFileSizeLow;
				}
			}

			bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);
		}
		while (!bStopped && FindNextFile(h, &finfo));
		
		FindClose(h);
	} 

	return bStopped ? -1 : dSize;
}

BOOL FileMisc::IsPath(LPCTSTR szPath)
{
	CString sPath(szPath);
	sPath.TrimLeft();
	sPath.TrimRight();
	
	return (!sPath.IsEmpty() && 
			(sPath.Find(_T(":\\")) == 1) || (sPath.Find(_T("\\\\")) == 0));
}

BOOL FileMisc::PathExists(LPCTSTR szPath)
{
	// special case
	if (Misc::IsEmpty(szPath)) // cwd
		return TRUE;

	return (::GetFileAttributes(szPath) != 0xffffffff);
}

BOOL FileMisc::FolderExists(LPCTSTR szFolder)
{
	// special case
	if (Misc::IsEmpty(szFolder)) // cwd
		return TRUE;

	if (lstrlen(szFolder) > _MAX_PATH)
		return FALSE;

	// else
	DWORD dwAttrib = GetFileAttributes(szFolder);

	return ((dwAttrib != 0xffffffff) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL FileMisc::FileExists(LPCTSTR szFile)
{
	int nLen = lstrlen(szFile);

	if ((nLen <= 0) || (nLen > _MAX_PATH))
		return FALSE;

	// else
	DWORD dwAttrib = GetFileAttributes(szFile);

	if ((dwAttrib != 0xffffffff) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0)
		return TRUE;

#ifdef _DEBUG
	DWORD dwError = GetLastError();
#endif

	return FALSE;
}

BOOL FileMisc::FolderFromFilePathExists(LPCTSTR szFilePath)
{
	return FolderExists(GetFolderFromFilePath(szFilePath));
}

CString FileMisc::GetCwd()
{
	CString sFolder;

	::GetCurrentDirectory(MAX_PATH, sFolder.GetBuffer(MAX_PATH+1));
	sFolder.ReleaseBuffer();

	return sFolder;
}

void FileMisc::SetCwd(const CString& sCwd)
{
	SetCurrentDirectory(sCwd);
}

CString FileMisc::GetFolderFromFilePath(LPCTSTR szFilePath)
{
	CString sFolder;

	// check if its a folder already
	if (FolderExists(szFilePath))
	{
		sFolder = szFilePath;
	}
	else
	{
		// remove file ending
		CString sDrive, sDir;

		SplitPath(szFilePath, &sDrive, &sDir);
		MakePath(sFolder, sDrive, sDir);
	}

	return TerminatePath(sFolder, FALSE);
}

BOOL FileMisc::CreateFolderFromFilePath(LPCTSTR szFilePath)
{
	return CreateFolder(GetFolderFromFilePath(szFilePath));
}

BOOL FileMisc::PathHasWildcard(LPCTSTR szFilePath)
{
	return (_tcschr(szFilePath, '?') || _tcschr(szFilePath, '*'));
}

BOOL FileMisc::IsFolderWritable(LPCTSTR szFolder)
{
	if (!FolderExists(szFolder))
		return FALSE;

	// Detect 'Program Files' explicitly for non-admin users
	if (!IsAdminProcess() && IsPathInSpecialFolder(szFolder, CSIDL_PROGRAM_FILES))
		return FALSE;
		
	// based on generated GUID = {90F97F18-32EC-4B5A-82FF-7F76953EDE37}
	LPCTSTR szUniqueName = _T("90F97F1832EC4B5A82FF7F76953EDE37");

	// try to create a unique file in the folder
	CString sUniquePath;
	MakePath(sUniquePath, NULL, szFolder, szUniqueName);

	if (!CFile().Open(sUniquePath, CFile::modeCreate))
		return FALSE;

	if (FileExists(sUniquePath))
	{
		DeleteFile(sUniquePath, TRUE);
		return TRUE;
	}

	// else 
	return FALSE;
}

BOOL FileMisc::CreateFolder(LPCTSTR szFolder)
{
	if (FolderExists(szFolder))
		return TRUE;

	// start from the highest level folder working to the lowest
	CString sFolder, sRemaining(szFolder);
	TerminatePath(sRemaining, FALSE);

	BOOL bDone = FALSE;
	BOOL bResult = TRUE;

	// pull off the :\ or \\ start
	int nFind = sRemaining.Find(_T(":\\"));

	if (nFind != -1)
	{
		sFolder += sRemaining.Left(nFind + 2);
		sRemaining = sRemaining.Mid(nFind + 2);
	}
	else
	{
		nFind = sRemaining.Find(_T("\\\\"));
		
		if (nFind != -1)
		{
			sFolder += sRemaining.Left(nFind + 2);
			sRemaining = sRemaining.Mid(nFind + 2);
		}
	}

	while (!bDone && bResult)
	{
		nFind = sRemaining.Find('\\', 1);

		if (nFind == -1)
		{
			bDone = TRUE;
			sFolder += sRemaining;
		}
		else
		{
			sFolder += sRemaining.Left(nFind);
			sRemaining = sRemaining.Mid(nFind);
		}

		// if the folder doesn't exist we try to create it
		if (!FolderExists(sFolder) && (::CreateDirectory(sFolder, NULL) == FALSE))
			bResult = FALSE;
	}

	return bResult;
}

BOOL FileMisc::FolderContainsFiles(LPCTSTR szFolder, BOOL bCheckSubFolders, LPCTSTR szFilter)
{
	CFileFind ff;
	CString sSearchSpec;
	
	MakePath(sSearchSpec, NULL, szFolder, szFilter, NULL);
	
	BOOL bContinue = ff.FindFile(sSearchSpec);
	
	while (bContinue)
	{
		bContinue = ff.FindNextFile();
		
		if (!ff.IsDots())
		{
			if (ff.IsDirectory())
			{
				if (bCheckSubFolders)
				{
					if (FolderContainsFiles(ff.GetFilePath(), TRUE, szFilter))
						return TRUE;
				}
			}
			else // is file
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

BOOL FileMisc::MoveFolder(LPCTSTR szSrcFolder, LPCTSTR szDestFolder, 
							LPCTSTR szFileMask, DWORD dwFlags, HANDLE hTerminate)
{
	if (CopyFolder(szSrcFolder, szDestFolder, szFileMask, dwFlags, hTerminate))
	{
		// don't pass on hTerminate to ensure the operation completes
		return DeleteFolderContents(szSrcFolder, dwFlags, szFileMask, NULL);
	}

	return FALSE;
}

BOOL FileMisc::CopyFolder(LPCTSTR szSrcFolder, LPCTSTR szDestFolder, LPCTSTR szFileMask, 
							DWORD dwFlags, HANDLE hTerminate)
{
	if (!CreateFolder(szDestFolder))
		return FALSE;

	if (!FolderExists(szSrcFolder))
		return FALSE;

	BOOL bSubFolders = Misc::HasFlag(dwFlags, FMDF_SUBFOLDERS);
	BOOL bMsgLoop = Misc::HasFlag(dwFlags, FMDF_PROCESSMSGLOOP);

	// if a file mask has been specified with subfolders 
	// we need to do multiple passes on each folder, 
	// one (or more) for the files and one for the sub folders
	CStringArray aMasks;
	int nNumMasks = Misc::Split(szFileMask, aMasks, ';');
	
	int nNumPass = (bSubFolders ? (nNumMasks + 1) : max(nNumMasks, 1));

	BOOL bResult = TRUE;
	BOOL bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);

	for (int nPass = 0; !bStopped && nPass < nNumPass; nPass++)
	{
		CString sSearchSpec(szSrcFolder), sMask(szFileMask);

		if (sMask.IsEmpty() || (nPass == aMasks.GetSize())) // (nPass == last) == 2nd pass (for folders)
			sMask = ALLFILESMASK;
		else
			sMask = aMasks[nPass];

		TerminatePath(sSearchSpec);
		sSearchSpec += sMask;

		WIN32_FIND_DATA finfo;
		HANDLE hSearch = NULL;

		if ((hSearch = FindFirstFile(sSearchSpec, &finfo)) != INVALID_HANDLE_VALUE) 
		{
			do 
			{
				if (bMsgLoop)
					Misc::ProcessMsgLoop();

				if (finfo.cFileName[0] != '.') 
				{
					CString sSource = (TerminatePath(szSrcFolder) + finfo.cFileName);
					CString sDest = (TerminatePath(szDestFolder) + finfo.cFileName);
					
					// process directories
					BOOL bFolder = Misc::HasFlag(finfo.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY);

					if (bSubFolders && bFolder)
					{
						bResult = CopyFolder(sSource, sDest, szFileMask, dwFlags, hTerminate);
					}
					else if (!bFolder) // files 
					{
						bResult = CopyFile(sSource, sDest, 
											Misc::HasFlag(dwFlags, FMDF_HIDDENREADONLY), 
											Misc::HasFlag(dwFlags, FMDF_OVERWRITE), 
											Misc::HasFlag(dwFlags, FMDF_NEWER));
					}
				}

				bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);
			}
			while (!bStopped && bResult && FindNextFile(hSearch, &finfo));
			
			FindClose(hSearch);
		} 
	}

	return (!bStopped && bResult);
}

int FileMisc::MoveFolder(LPCTSTR szSrcFolder, LPCTSTR szDestFolder, DWORD dwFlags, HANDLE hTerminate)
{
	return MoveFolder(szSrcFolder, szDestFolder, NULL, (dwFlags | FMDF_SUBFOLDERS), hTerminate);
}

BOOL FileMisc::CopyFolder(LPCTSTR szSrcFolder, LPCTSTR szDestFolder, DWORD dwFlags, HANDLE hTerminate)
{
	return CopyFolder(szSrcFolder, szDestFolder, NULL, (dwFlags | FMDF_SUBFOLDERS), hTerminate);
}

BOOL FileMisc::DeleteFile(LPCTSTR szPath, BOOL bHiddenReadOnly)
{
	if (bHiddenReadOnly)
		ModifyAttributes(szPath, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN, 0);

	return ::DeleteFile(szPath);
}

BOOL FileMisc::CopyFile(LPCTSTR szSrcPath, LPCTSTR szDestPath, BOOL bOverwrite, BOOL bHiddenReadOnly, BOOL bNewer)
{
	if (bNewer && (GetFileLastModified(szSrcPath) <= GetFileLastModified(szDestPath)))
		return TRUE;

	BOOL bCopied = ::CopyFile(szSrcPath, szDestPath, TRUE);

	if (!bCopied && FileExists(szDestPath))
	{
		// if allowed to overwrite try deleting dest file before re-trying
		if (bOverwrite)
		{
			if (DeleteFile(szDestPath, bHiddenReadOnly))
			{
				bCopied = ::CopyFile(szSrcPath, szDestPath, TRUE);
			}
		}
		else // not an error
		{
			return TRUE;
		}
	}

	if (bCopied)
		LogText(_T("FileMisc::CopyFile(%s -> %s)"), szSrcPath, szDestPath);
	else
		LogText(_T("FileMisc::CopyFile(%s -> %s) FAILED! (%ld)"), szSrcPath, szDestPath, ::GetLastError());

	return bCopied;
}

BOOL FileMisc::MoveFile(LPCTSTR szSrcPath, LPCTSTR szDestPath, BOOL bOverwrite, BOOL bHiddenReadOnly)
{
	BOOL bMoved = ::MoveFile(szSrcPath, szDestPath);

	if (!bMoved && bOverwrite && FileExists(szDestPath))
	{
		// try deleting dest file before re-trying
		if (DeleteFile(szDestPath, bHiddenReadOnly))
		{
			bMoved = ::MoveFile(szSrcPath, szDestPath);
		}
	}
	
	if (bMoved)
		LogText(_T("FileMisc::MoveFile(%s -> %s)"), szSrcPath, szDestPath);
	else
		LogText(_T("FileMisc::MoveFile(%s -> %s) FAILED! (%ld)"), szSrcPath, szDestPath, ::GetLastError());

	return bMoved;
}

BOOL FileMisc::ModifyAttributes(LPCTSTR szPath, DWORD dwRemove, DWORD dwAdd, DWORD dwExistAttrib)
{
	if (dwExistAttrib == 0xFFFFFFFF)
		dwExistAttrib = ::GetFileAttributes(szPath);

	// verify there is anything to change
	BOOL bChange = FALSE;

	if (dwRemove && ((dwExistAttrib & dwRemove) != 0))
	{
		bChange = TRUE;
	}
	else if (dwAdd && ((dwExistAttrib & dwAdd) == 0))
	{
		bChange = TRUE;
	}

	if (!bChange)
		return -1;

	// else
	DWORD dwNewAttrib = dwExistAttrib;

	dwNewAttrib |= dwAdd;
	dwNewAttrib &= ~dwRemove;

	return ::SetFileAttributes(szPath, dwNewAttrib);
}

BOOL FileMisc::ModifyFolderAndContentAttributes(LPCTSTR szFolder, DWORD dwRemove, DWORD dwAdd, BOOL bSubFolders)
{
	if (!FolderExists(szFolder))
		return TRUE; // not an error

	// start with folder
	BOOL bResult = ModifyAttributes(szFolder, dwRemove, dwAdd);

	CString sSearchSpec = (TerminatePath(szFolder) + ALLFILESMASK);
	WIN32_FIND_DATA finfo;
	HANDLE hSearch = NULL;
	
	if ((hSearch = FindFirstFile(sSearchSpec, &finfo)) != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if (finfo.cFileName[0] != '.') 
			{
				CString sPathName = (TerminatePath(szFolder) + finfo.cFileName);
				BOOL bFile = !Misc::HasFlag(finfo.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY);

				if (bFile)
				{
					bResult &= ModifyAttributes(sPathName, dwRemove, dwAdd, finfo.dwFileAttributes);
				}
				else if (bSubFolders)
				{
					// folder and contents
					bResult &= ModifyFolderAndContentAttributes(sPathName, dwRemove, dwAdd, TRUE);
				}

			}
		}
		while (FindNextFile(hSearch, &finfo));
		
		FindClose(hSearch);
	} 
	
	return bResult;
}

double FileMisc::GetFileSize(LPCTSTR szPath)
{
	CFileFind ff;
	
	if (ff.FindFile(szPath))
	{
		ff.FindNextFile();

#if _MSC_VER < 1400
		return (double)ff.GetLength64();
#else
		return (double)ff.GetLength();
#endif
	}

	// else
	return 0;
}

BOOL FileMisc::IsLoggingEnabled()
{
	return s_bLogging;
}

CString FileMisc::GetLogFilePath()
{
	CString sLogFolder, sLogFile;
	CString sAppName(GetFileNameFromPath(GetAppFilePath(), FALSE));

	if (Misc::HasFlag(s_dwLogFlags, FMLF_TEMPDIR))
	{
		sLogFolder = GetTempFolder();
	}
	else
	{
		if (!Misc::HasFlag(s_dwLogFlags, FMLF_NOAPPDIR))
		{
			CString sAppFolder = GetAppFolder();
			
			if (IsFolderWritable(sAppFolder))
				sLogFolder = sAppFolder;
		}

		// else use APPDATA folder
		if (sLogFolder.IsEmpty())
		{
			VERIFY (GetSpecialFilePath(CSIDL_APPDATA, s_sLogAppDataKey, sLogFolder));
			
			// add app name as sub folder if not in key
			if (s_sLogAppDataKey.Find(sAppName) == -1)
				TerminatePath(sLogFolder) += sAppName;
		}
	}

	ASSERT(!sLogFolder.IsEmpty());

	return MakePath(sLogFile, NULL, sLogFolder, sAppName, _T("log"));
}

BOOL FileMisc::EnableLogging(BOOL bEnable, LPCTSTR szAppDataKey, DWORD dwFlags)
{
	if (IsLoggingEnabled())
		return TRUE;

	// these flags are mutually exclusive
	// better to fail because user (me) must be confused!
	ASSERT (!(Misc::HasFlag(dwFlags, FMLF_NOAPPDIR) && Misc::HasFlag(dwFlags, FMLF_TEMPDIR)));

	if (Misc::HasFlag(dwFlags, FMLF_NOAPPDIR) && Misc::HasFlag(dwFlags, FMLF_TEMPDIR))
		return FALSE;

	s_sLogAppDataKey = szAppDataKey;
	s_bLogging = bEnable;
	s_dwLogFlags = dwFlags;

	if (bEnable)
	{
		if (!Misc::HasFlag(s_dwLogFlags, FMLF_NORESET))
			DeleteFile(GetLogFilePath(), TRUE);

		LogText(_T("Logging to: %s"), GetLogFilePath());
		LogTextRaw(_T("-------------------------------------------------"));
		LogText(_T("Windows Version: %s"), COSVersion().FormatOSVersion());
		LogText(_T("App Version: %s"), GetModuleVersion());
		LogTextRaw(_T("-------------------------------------------------"));
	}

	return TRUE;
}

void FileMisc::LogTimeElapsed(DWORD& dwTickStart, LPCTSTR szFormat, ...)
{
	if (!IsLoggingEnabled())
		return;

	CString sMessage, sReason(_T("Action"));

	if (!Misc::IsEmpty(szFormat))
	{
		// from CString::Format
		ASSERT(AfxIsValidString(szFormat));

		va_list argList;
		va_start(argList, szFormat);
		sReason.FormatV(szFormat, argList);
		va_end(argList);
	}

	DWORD dwTickEnd = GetTickCount();
	sMessage.Format(_T("%s took %lu ms"), sReason, dwTickEnd - dwTickStart);

	FileMisc::LogText(sMessage);
	dwTickStart = GetTickCount();
}

void FileMisc::LogTextRaw(LPCTSTR szText)
{
	if (IsLoggingEnabled())
		VERIFY(AppendLineToFile(GetLogFilePath(), szText, SFEF_UTF8));
}

void FileMisc::LogText(LPCTSTR szText, ...)
{
	if (!IsLoggingEnabled())
		return;

	CString sLogLine;

	if (!Misc::IsEmpty(szText))
	{
		// from CString::Format
		ASSERT(AfxIsValidString(szText));

		va_list argList;
		va_start(argList, szText);
		sLogLine.FormatV(szText, argList);
		va_end(argList);
	}

	if (Misc::HasFlag(s_dwLogFlags, FMLF_TIMESTAMP))
	{
		sLogLine += _T(" (");
		sLogLine += COleDateTime::GetCurrentTime().Format();
		sLogLine += _T(")");
	}

	VERIFY(AppendLineToFile(GetLogFilePath(), sLogLine, SFEF_UTF8));
}

BOOL FileMisc::AppendLineToFile(LPCTSTR szPathname, const CString& sLine, SFE_FORMAT nFormat)
{
	if (sLine.IsEmpty())
		return FALSE;

	// make sure parent folder exists
	if (!CreateFolderFromFilePath(szPathname))
		return FALSE;

	nFormat = (FileExists(szPathname) ? SFEF_AUTODETECT : nFormat); 

	CStdioFileEx file;

	if (file.Open(szPathname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite, nFormat))
	{
		file.SeekToEnd();
		file.WriteString(sLine);

		// add trailing new line as necessary
		if (sLine.Find(ENDL) != (sLine.GetLength() - 2))
			file.WriteString(ENDL);

		file.Flush();
		file.Close();

		return TRUE;
	}

	return FALSE;
}

int FileMisc::LoadFile(LPCTSTR szPathname, CStringArray& aLines, int nLineCount, UINT nAnsiCodePage)
{
	int nLinesRead = 0;
	aLines.RemoveAll();

	CStdioFileEx file;

	if (file.Open(szPathname, (CFile::modeRead | CFile::shareDenyWrite), SFEF_AUTODETECT, nAnsiCodePage))
	{
		if (file.GetLength())
		{
			if (nLineCount == -1)
				nLineCount = INT_MAX;

			// read lines
			CString sLine;
			while (file.ReadString(sLine) && nLinesRead < nLineCount)
			{
				aLines.Add(sLine);
				nLinesRead++;
			}
		}
	}

	return nLinesRead;
}

BOOL FileMisc::CanReadFile(LPCTSTR szPathname, BOOL bDenyWrite)
{
	CFile file;
	UINT nFlags = CFile::modeRead | (bDenyWrite ? CFile::shareDenyWrite : CFile::shareDenyNone);

	return (file.Open(szPathname, nFlags) != FALSE);
}

BOOL FileMisc::SaveFile(LPCTSTR szPathname, const CString& sText, SFE_FORMAT nFormat, UINT nAnsiCodePage)
{
	CStdioFileEx file;

	if (file.Open(szPathname, CFile::modeCreate | CFile::modeWrite, nFormat, nAnsiCodePage))
	{
		file.WriteString(sText);
		return TRUE;
	}

	return FALSE;
}

BOOL FileMisc::SaveFile(LPCTSTR szPathname, LPCSTR szText)
{
	CStdioFile file;

	if (file.Open(szPathname, CFile::modeCreate | CFile::modeWrite))
	{
		file.Write(szText, strlen(szText));
		return TRUE;
	}

	return FALSE;
}

BOOL FileMisc::LoadFile(LPCTSTR szPathname, CString& sText, BOOL bDenyWrite, UINT nAnsiCodePage)
{
	CStdioFileEx file;
	UINT nFlags = (CFile::modeRead | (bDenyWrite ? CFile::shareDenyWrite : CFile::shareDenyNone));

	if (file.Open(szPathname, nFlags, SFEF_AUTODETECT, nAnsiCodePage))
	{
		return (file.ReadFile(sText) != FALSE);
	}

	return FALSE;
}

BOOL FileMisc::IsTempFilePath(LPCTSTR szFilename)
{
	CString sFilename(szFilename);
	sFilename.MakeLower();

	CString sTempFolder = GetTempFolder();
	sTempFolder.MakeLower();

	return (sFilename.Find(sTempFolder) == 0);
}

CString FileMisc::GetTempFolder()
{
	TCHAR szTempPath[MAX_PATH+1] = { 0 };
	
	if (::GetTempPath(MAX_PATH, szTempPath))
		return CString(szTempPath);

	// else
	return _T("C:\\Temp");
}

CString FileMisc::GetTempFilePath(LPCTSTR szPrefix, UINT uUnique)
{
	TCHAR szTempFile[MAX_PATH+1] = { 0 }, szTempPath[MAX_PATH+1] = { 0 };
	
	if (::GetTempPath(MAX_PATH, szTempPath))
	{
		if (::GetTempFileName(szTempPath, szPrefix, uUnique, szTempFile))
			return szTempFile;
	}

	return "";
}

CString FileMisc::GetTempFilePath(LPCTSTR szFilename, LPCTSTR szExt)
{
	CString sTempFile;
	TCHAR szTempPath[MAX_PATH+1] = { 0 };
	
	if (::GetTempPath(MAX_PATH, szTempPath))
		MakePath(sTempFile, NULL, szTempPath, szFilename, szExt);

	return sTempFile;
}

BOOL FileMisc::SelectFileInExplorer(LPCTSTR szFilePath)
{
	CString sFile;
	sFile.Format(_T("/n,/select,\"%s\""), szFilePath);
	
	CString sExplorer;
	sExplorer.Format(_T("%s\\Explorer.exe"), GetWindowsFolder());
	
	return (Run(NULL, sExplorer, sFile) > 32);
}

BOOL FileMisc::ExpandPathEnvironmentVariables(CString& sFilePath)
{
	CString sExpanded;
	DWORD dwLen = ExpandEnvironmentStrings(sFilePath, NULL, 0);

	if (ExpandEnvironmentStrings(sFilePath, sExpanded.GetBuffer(dwLen + 1), (dwLen + 1)))
	{
		sExpanded.ReleaseBuffer();
		sFilePath = sExpanded;

		return TRUE;
	}

	return FALSE;
}

DWORD FileMisc::Run(HWND hwnd, LPCTSTR lpFile, LPCTSTR lpParams, int nShowCmd, LPCTSTR lpDirectory, LPCTSTR lpVerb)
{
	CString sFile(lpFile), sParams(lpParams);

	if (lpParams == NULL)
	{
		int nHash = sFile.Find('#');
		
		if (nHash != -1)
		{
			sParams = sFile.Mid(nHash);
			sFile = sFile.Left(nHash);
			
			CString sApp = CFileRegister::GetRegisteredAppPath(sFile);
			
			if (!sApp.IsEmpty())
			{
				sFile = sApp;
				sParams = lpFile;
			}
			else
			{
				sFile = lpFile;
				sParams.Empty();
			}
		}
	}
	
	DWORD dwRes = (DWORD)ShellExecute(hwnd, lpVerb, sFile, sParams, lpDirectory, nShowCmd);
	
	if (dwRes <= 32) // failure
	{
		if (lpVerb)
			LogText(_T("ShellExecute(%s: %s) failed. RetCode = %ld"), lpFile, lpVerb, dwRes);
		else
			LogText(_T("ShellExecute(%s) failed. RetCode = %ld"), lpFile, dwRes);

		// try CreateProcess
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		si.wShowWindow = (WORD)nShowCmd;
		si.dwFlags = STARTF_USESHOWWINDOW;

		ZeroMemory( &pi, sizeof(pi) );

		// Start the child process.
		if (CreateProcess( NULL,			// No module name (use command line).
							(LPTSTR)lpFile,	// Command line.
							NULL,			// Process handle not inheritable.
							NULL,			// Thread handle not inheritable.
							FALSE,			// Set handle inheritance to FALSE.
							0,				// No creation flags.
							NULL,			// Use parent's environment block.
							lpDirectory,	// starting directory.
							&si,			// Pointer to STARTUPINFO structure.
							&pi ))			// Pointer to PROCESS_INFORMATION structure.
		{
			dwRes = 33; // success
		}

		// Close process and thread handles.
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
	return dwRes;
}

BOOL FileMisc::ExtractResource(UINT nID, LPCTSTR szType, const CString& sTempFilePath, HINSTANCE hInst)
{
	// compare time with that of module from which it was loaded
	CString sTempPath;
	CFileStatus fsRes, fsModule;
	CString sModulePath = GetModuleFilePath(hInst);

	if (!CFile::GetStatus(sModulePath, fsModule))
		return FALSE;
	
	// see if the file has been created before
	if (!CFile::GetStatus(sTempFilePath, fsRes) || fsRes.m_mtime < fsModule.m_mtime)
	{
		// Load the resource into memory
		HRSRC hRes = FindResource(hInst, (LPCTSTR)nID, szType);
		
		if (!hRes) 
		{
			TRACE(_T("Couldn't find %s resource %d!\n"), szType, nID);
			return FALSE;
		}
		
		DWORD len = SizeofResource(hInst, hRes);
		
		BYTE* lpRes = (BYTE*)LoadResource(hInst, hRes);
		ASSERT(lpRes);

		CFile file;
		
		if (file.Open(sTempFilePath, CFile::modeCreate | CFile::modeWrite))
		{
			file.Write(lpRes, len);
			file.Close();
			FreeResource((HANDLE)lpRes);
		}
		else
		{
			FreeResource((HANDLE)lpRes);
			return FALSE;
		}
	}
	
	return TRUE;
}

CString FileMisc::GetModuleFilePath(HMODULE hMod)
{
	CString sModulePath;

	BOOL bSuccess = ::GetModuleFileName(hMod, sModulePath.GetBuffer(MAX_PATH+1), MAX_PATH);
   sModulePath.ReleaseBuffer();

   ASSERT(bSuccess);

   if (!bSuccess || !CanonicalizePath(sModulePath))
      return _T("");

	return sModulePath;
}

BOOL FileMisc::CanonicalizePath(CString& sFilePath)
{
   VERIFY(sFilePath.GetLength() <= MAX_PATH);

   if (sFilePath.GetLength() > MAX_PATH)
      return FALSE;

   TCHAR szPath[MAX_PATH+1] = { 0 };
   lstrcpy(szPath, sFilePath);

   BOOL bSuccess = ::PathCanonicalize(sFilePath.GetBuffer(MAX_PATH+1), szPath);
   sFilePath.ReleaseBuffer();

   ASSERT(bSuccess);

   if (!bSuccess)
      sFilePath = szPath; // restore

   return bSuccess;
}

CString FileMisc::GetModuleFolder(HMODULE hMod) 
{ 
	return GetFolderFromFilePath(GetModuleFilePath(hMod)); 
}

int FileMisc::GetModuleDriveType(HMODULE hMod)
{
	return CDriveInfo::GetPathType(GetModuleFilePath(hMod));
}

CString FileMisc::GetModuleVersion(LPCTSTR szModulePath, TCHAR cSep)
{
	CString sModuleVer;
	CFileVersionInfo fvi;

	if (fvi.Create(szModulePath))
	{
		sModuleVer = fvi.GetFileVersion();

		if (!sModuleVer.Replace(',', cSep))
			sModuleVer.Replace('.', cSep);
	}

	return sModuleVer;
}

CString FileMisc::GetModuleVersion(HMODULE hMod, TCHAR cSep)
{
	return GetModuleVersion(GetModuleFilePath(hMod), cSep);
}

BOOL FileMisc::GetModuleVersion(LPCTSTR szModulePath, CDWordArray& aVersionParts)
{
	return (SplitVersionNumber(GetModuleVersion(szModulePath), aVersionParts) > 0);
}

BOOL FileMisc::GetModuleVersion(HMODULE hMod, CDWordArray& aVersionParts)
{
	return GetModuleVersion(GetModuleFilePath(hMod), aVersionParts);
}

CString FileMisc::GetAppFilePath()
{
	return GetModuleFilePath();
}

CString FileMisc::GetAppFolder(LPCTSTR szSubFolder)
{
	CString sFolder = GetModuleFolder();

	if (!Misc::IsEmpty(szSubFolder))
		MakePath(sFolder, NULL, sFolder, szSubFolder, NULL);

	return TerminatePath(sFolder, FALSE);
}

CString FileMisc::GetAppResourceFolder(LPCTSTR szResFolder)
{
	return GetAppFolder(szResFolder);
}

int FileMisc::GetAppModuleFilePaths(CStringArray& aFileNames, FB_MODULE_SORT nSort)
{
	aFileNames.RemoveAll();

	CDWordArray aHandles;

	int nNumMod = GetAppModuleHandles(aHandles, (nSort == FBM_SORTBY_HMODULE));

    for (int i = 0; i < nNumMod; i++)
    {
		aFileNames.Add(GetModuleFilePath((HMODULE)aHandles[i]));
    }
	
	// sort 
	if (nSort == FBM_SORTBY_FILENAME)
		Misc::SortArray(aFileNames);

	return aFileNames.GetSize();
}

int FileMisc::GetAppModuleHandles(CDWordArray& aHandles, BOOL bSorted)
{
	aHandles.RemoveAll();

	HMODULE hMods[1024] = { 0 };
    DWORD cbNeeded = 0;

	static HMODULE hPsapi = LoadLibrary(_T("psapi.dll"));

	if (hPsapi)
	{
		typedef BOOL (WINAPI *FNENUMPROCESSMODULES)(HANDLE,	HMODULE*, DWORD, LPDWORD);

		FNENUMPROCESSMODULES pFN = (FNENUMPROCESSMODULES)GetProcAddress(hPsapi, "EnumProcessModules");

		if (pFN && pFN(::GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded))
		{
			int nNumMod = (int)(cbNeeded / sizeof(HMODULE));

			for (int i = 0; i < nNumMod; i++)
			{
				aHandles.Add((DWORD)hMods[i]);
			}
		
			// sort 
			if (bSorted)
				Misc::SortArray(aHandles);
		}
	}

	return aHandles.GetSize();
}

CString FileMisc::GetProcessFilePath(HANDLE hProcess)
{
	CString sPath;
	static HMODULE hPsapi = LoadLibrary(_T("psapi.dll"));

	if (hPsapi)
	{
		typedef DWORD (WINAPI *FNGETMODULEFILENAMEEX)(HANDLE, HMODULE, LPTSTR, DWORD);

		FNGETMODULEFILENAMEEX pFN = (FNGETMODULEFILENAMEEX)GetProcAddress(hPsapi, "GetModuleFileNameExW");

		if (pFN)
		{
			DWORD dwRes = pFN(hProcess, NULL, sPath.GetBuffer(MAX_PATH+1), MAX_PATH);
			sPath.ReleaseBuffer();

			if (!dwRes)
			{
				dwRes = GetLastError();
				int breakpoint = 0;
			}
		}
	}

	return sPath;
}

BOOL FileMisc::IsAdminProcess(HANDLE hProcess)
{
	if (hProcess == NULL)
		hProcess = GetCurrentProcess();
	
	BOOL bAdmin = -1;
	HANDLE hToken = NULL;
	
	if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) 
	{
		TOKEN_ELEVATIONEX Elevation = { 0 };
		DWORD cbSize = sizeof(TOKEN_ELEVATIONEX);
		
		const TOKEN_INFORMATION_CLASS TokenElevation = (const TOKEN_INFORMATION_CLASS)20;
		
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, cbSize, &cbSize)) 
			bAdmin = Elevation.TokenIsElevated;
	}
	
	if (hToken) 
		CloseHandle(hToken);

	// Fallback
	if (bAdmin == -1)
	{
		bAdmin = FALSE;

		typedef BOOL (WINAPI *PFNISUSERANADMIN)(void);
		
		// load shell32.dll once only
		static HMODULE hShell32 = LoadLibrary(_T("shell32.dll"));
		
		if (hShell32)
		{
			static PFNISUSERANADMIN fnIsUserAdmin = (PFNISUSERANADMIN)GetProcAddress(hShell32, "IsUserAnAdmin");
			
			if (fnIsUserAdmin)
				bAdmin = fnIsUserAdmin();
		}
	}

	return bAdmin;
}

void FileMisc::LogAppModuleState(FB_MODULE_SORT nSort)
{
	if (IsLoggingEnabled())
	{
		CStringArray aModulePaths;

		int nNumMods = GetAppModuleFilePaths(aModulePaths, nSort);
		ASSERT(nNumMods);

		// header
		LogTextRaw(_T("Begin Application Module State-----------------------"));

		for (int nMod = 0; nMod < nNumMods; nMod++)
		{
			HMODULE hMod = GetModuleHandle(aModulePaths[nMod]);

			LogText(_T("Handle = %08X, Path = %s, Ver = %s"), 
					hMod, 
					aModulePaths[nMod], 
					GetModuleVersion(hMod));
		}

		// footer
		LogTextRaw(_T("End Application Module State-------------------------"));
	}
}

CString FileMisc::GetAppVersion(TCHAR cSep)
{
	return GetModuleVersion((HMODULE)NULL, cSep);
}

BOOL FileMisc::GetAppVersion(CDWordArray& aVersionParts)
{
	return GetModuleVersion((HMODULE)NULL, aVersionParts);
}

int FileMisc::SplitVersionNumber(LPCTSTR szVersion, CDWordArray& aVersionParts)
{
	// replace commas with periods to simplify things
	CString sVersion(szVersion);
	sVersion.Replace(',', '.');

	CStringArray aParts;

	if (!Misc::Split(szVersion, aParts, '.', FALSE))
		return 0;

	aVersionParts.RemoveAll();
	int nPart = aParts.GetSize();

	while (nPart--)
		aVersionParts.InsertAt(0, _ttoi(aParts[nPart]));
 
	return aVersionParts.GetSize();
}

CString FileMisc::GetPrevAppVersion(DWORD nMaxVer2, DWORD nMaxVer3, DWORD nMaxVer4)
{
	CDWordArray aParts;
	CString sVersion;
	
	if (GetPrevAppVersion(aParts, nMaxVer2, nMaxVer3, nMaxVer4))
	{
		ASSERT(aParts.GetSize() == 4);
		sVersion.Format(_T("%d.%d.%d.%d"), aParts[0], aParts[1], aParts[2], aParts[3]);
	}

	return sVersion;
}

CString FileMisc::GetPrevAppVersion(LPCTSTR szVersion, DWORD nMaxVer2, DWORD nMaxVer3, DWORD nMaxVer4)
{
	CDWordArray aParts;
	CString sVersion;

	if (SplitVersionNumber(szVersion, aParts) && GetPrevAppVersion(aParts, nMaxVer2, nMaxVer3, nMaxVer4))
	{
		ASSERT(aParts.GetSize() == 4);
		sVersion.Format(_T("%d.%d.%d.%d"), aParts[0], aParts[1], aParts[2], aParts[3]);
	}

	return sVersion;
}

BOOL FileMisc::GetPrevAppVersion(CDWordArray& aVersionParts, DWORD nMaxVer2, DWORD nMaxVer3, DWORD nMaxVer4)
{
	// if empty then initialise
	if (aVersionParts.GetSize() == 0)
		VERIFY(GetAppVersion(aVersionParts));

	// for each part (working from bottom up):
	//   if the part is not zero we just deduct one and return
	//   else set the part to its max value and continue
	int nPart = aVersionParts.GetSize();
	ASSERT(nPart == 4);

	UINT nMaxVers[4] = { 0, nMaxVer2, nMaxVer3, nMaxVer4 };

	while (nPart--)
	{
		if (aVersionParts[nPart] > 0)
		{
			aVersionParts[nPart]--;
			return TRUE;
		}

		aVersionParts[nPart] = nMaxVers[nPart];
	}

	// all else
	return FALSE;
}


int FileMisc::CompareVersions(LPCTSTR szVersion1, LPCTSTR szVersion2, int nNumItems)
{
	// if the first character of either string is not a number
	// then fall back on a standard string comparison
	if (!szVersion1 || !isdigit(szVersion1[0]) || !szVersion2 || !isdigit(szVersion2[0]))
		return lstrcmp(szVersion1, szVersion2);

	CDWordArray aVer1, aVer2;

	SplitVersionNumber(szVersion1, aVer1);
	SplitVersionNumber(szVersion2, aVer2);

	return CompareVersions(aVer1, aVer2, nNumItems);
}

int FileMisc::CompareVersions(CDWordArray& aVersion1, CDWordArray& aVersion2, int nNumItems)
{
	int nNumVer1 = aVersion1.GetSize();
	int nNumVer2 = aVersion2.GetSize();

	if (nNumItems > 0)
	{
		nNumItems = min(nNumVer1, min(nNumVer2, nNumItems));
	}
	else
	{
		nNumItems = min(nNumVer1, nNumVer2);
	}

	// compare starting from the front
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		if (aVersion1[nItem] < aVersion2[nItem])
			return -1;

		else if (aVersion1[nItem] > aVersion2[nItem])
			return 1;

		// else try next item
	}

	// if we got here then compare array lengths
	if (nNumItems <= 0)
	{
		return (nNumVer1 - nNumVer2);
	}

	// else
	return 0;
}

CString FileMisc::GetWindowsFolder()
{
	CString sFolder;

	::GetWindowsDirectory(sFolder.GetBuffer(MAX_PATH+1), MAX_PATH);
	sFolder.ReleaseBuffer();

	return TerminatePath(sFolder, FALSE);
}

CString FileMisc::GetWindowsSystemFolder()
{
	CString sFolder;

	::GetSystemDirectory(sFolder.GetBuffer(MAX_PATH+1), MAX_PATH);
	sFolder.ReleaseBuffer();

	return TerminatePath(sFolder, FALSE);
}

BOOL FileMisc::ExtractResource(LPCTSTR szModulePath, UINT nID, LPCTSTR szType, const CString& sTempFilePath)
{
	HMODULE hModule = LoadLibraryEx(szModulePath, NULL, LOAD_LIBRARY_AS_DATAFILE);

	if (!hModule)
		return FALSE;

	// else
	return ExtractResource(nID, szType, sTempFilePath, hModule);
}

BOOL FileMisc::HasExtension(LPCTSTR szFilePath, LPCTSTR szExt)
{
	if (!szExt || !szFilePath)
		return FALSE;

	if (szExt[0] == '.')
		szExt++;

	CString sExt = GetExtension(szFilePath);

	if (sExt.GetLength() && sExt[0] == '.')
		sExt = sExt.Mid(1);

	return (sExt.CompareNoCase(szExt) == 0);
}

CString FileMisc::GetExtension(LPCTSTR szFilePath)
{
	CString sExt;
	SplitPath(szFilePath, NULL, NULL, NULL, &sExt);
	
	return sExt;
}

void FileMisc::SplitPath(LPCTSTR szPath, CString* pDrive, CString* pDir, CString* pFName, CString* pExt)
{
	TCHAR szDrive[_MAX_DRIVE+1] = { 0 }, 
			szFolder[_MAX_DIR+1] = { 0 }, 
			szFile[_MAX_FNAME+1] = { 0 }, 
			szExt[_MAX_EXT+1] = { 0 };

#if _MSC_VER >= 1400
	_tsplitpath_s(szPath, szDrive,_MAX_DRIVE, szFolder,_MAX_DIR, szFile, _MAX_FNAME, szExt, _MAX_EXT);
#else
	_tsplitpath(szPath, szDrive, szFolder, szFile, szExt);
#endif

	if (pDrive)
		*pDrive = szDrive;

	if (pDir)
		*pDir = szFolder;

	if (pFName)
		*pFName = szFile;

	if (pExt)
		*pExt = szExt;
}

CString& FileMisc::MakePath(CString& sPath, LPCTSTR szDrive, LPCTSTR szDir, LPCTSTR szFName, LPCTSTR szExt)
{
	TCHAR szPath[MAX_PATH+1] = { 0 };

#if _MSC_VER >= 1400
	_tmakepath_s(szPath, MAX_PATH, szDrive, szDir, szFName, szExt);
#else
	_tmakepath(szPath, szDrive, szDir, szFName, szExt);
#endif

	sPath = szPath;

	return sPath;
}

CString& FileMisc::PathConcat(CString& sPath, LPCTSTR szDirOrFName)
{
	CString sExtra(szDirOrFName);
	sExtra.TrimLeft(_T(" \\/"));

	TerminatePath(sPath) += sExtra;

	return sPath;
}

CString FileMisc::PathConcat(LPCTSTR szPath, LPCTSTR szDirOrFName)
{
	CString sPath(szPath);

	return PathConcat(sPath, szDirOrFName);
}

CString FileMisc::GetFullPath(const CString& sFilePath, const CString& sRelativeToFolder)
{
	// Check for URLs and already full paths
	if (sFilePath.Find(':') != -1)
		return sFilePath;

	CString sFullPath;

	if (::PathIsRelative(sFilePath))
	{
		sFullPath = FileMisc::TerminatePath(sRelativeToFolder) + sFilePath;
	}
	else // special case: filename has no drive letter and is not UNC
	{
		if ((sFilePath.Find(_T(":\\")) == -1) && (sFilePath.Find(_T("\\\\")) == -1))
		{
			CString sDrive;

			SplitPath(sRelativeToFolder, &sDrive);
			MakePath(sFullPath, sDrive, NULL, sFilePath);
		}
		else
		{
			sFullPath = sFilePath;
		}
	}

	// Convert all forward slashes to back slashes
	sFullPath.Replace('/', '\\');

	VERIFY(CanonicalizePath(sFullPath));

	return sFullPath;
}

CString& FileMisc::MakeFullPath(CString& sFilePath, const CString& sRelativeToFolder)
{
	sFilePath = GetFullPath(sFilePath, sRelativeToFolder);
	return sFilePath;
}

CString FileMisc::GetRelativePath(const CString& sFilePath, const CString& sRelativeToFolder, BOOL bFolder)
{
	if (::PathIsRelative(sFilePath))
		return sFilePath;

	TCHAR szRelPath[MAX_PATH + 1] = { 0 };

	BOOL bRes = ::PathRelativePathTo(szRelPath, 
									TerminatePath(sRelativeToFolder, FALSE), 
									FILE_ATTRIBUTE_DIRECTORY, 
									sFilePath, 
									(bFolder ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL));

	return bRes ? CString(szRelPath) : sFilePath;
}

CString& FileMisc::MakeRelativePath(CString& sFilePath, const CString& sRelativeToFolder, BOOL bFolder)
{
	sFilePath = GetRelativePath(sFilePath, sRelativeToFolder, bFolder);
	return sFilePath;
}

BOOL FileMisc::IsSamePath(const CString& sPath1, const CString& sPath2)
{
	CString sFullPath1 = GetFullPath(sPath1);
	CString sFullPath2 = GetFullPath(sPath2);

	return (sFullPath1.CompareNoCase(sFullPath2) == 0);
}

int FileMisc::CompareContents(const CString& sPath1, const CString& sPath2)
{
	CFile file1, file2;

	if (!file1.Open(sPath1, CFile::modeRead | CFile::shareDenyWrite))
	{
		return -1;
	}
	else if (!file2.Open(sPath2, CFile::modeRead | CFile::shareDenyWrite))
	{
		return 1;
	}

	DWORD dwLen1 = file1.GetLength();
	DWORD dwLen2 = file2.GetLength();

	if (dwLen1 < dwLen2)
	{
		return -1;
	}
	else if (dwLen2 < dwLen1)
	{
		return 1;
	}

	const UINT BUFLEN = 1024;
	BYTE buf1[BUFLEN], buf2[BUFLEN];

	BOOL bContinue = TRUE;

	while (bContinue)
	{
		DWORD dwRead1 = file1.Read(buf1, BUFLEN);
		DWORD dwRead2 = file2.Read(buf2, BUFLEN);

		// sanity check since files already reported as same length
		ASSERT(dwRead1 == dwRead2);

		if (dwRead1 < dwRead2)
		{
			return -1;
		}
		else if (dwRead2 < dwRead1)
		{
			return 1;
		}
		else if (dwRead1) // something read
		{
			int nCmp = memcmp(buf1, buf2, dwRead1);

			if (nCmp != 0)
				return nCmp;
		}

		bContinue = (dwRead1 == BUFLEN);
	}

	// perfect match
	return 0;
}

BOOL FileMisc::GetSpecialFolder(int nCSIDL, CString& sFolder, BOOL bAutoCreate)
{
	BOOL bRes = SHGetSpecialFolderPath(NULL, sFolder.GetBuffer(MAX_PATH), nCSIDL, bAutoCreate);
	sFolder.ReleaseBuffer();

	// ought not to fail
	ASSERT(bRes);

	return bRes;
}

BOOL FileMisc::GetSpecialFilePath(int nCSIDL, const CString& sFileName, CString& sFilePath)
{
	if (GetSpecialFolder(nCSIDL, sFilePath))
	{
		// append filename if present
		if (!sFileName.IsEmpty())
			TerminatePath(sFilePath) += sFileName;

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL FileMisc::IsPathInSpecialFolder(const CString& sFilePath, int nCSIDL)
{
	CString sSpecialFolder;
	
	if (!GetSpecialFolder(nCSIDL, sSpecialFolder))
		return FALSE;
	
	return (Misc::ToUpper(sFilePath).Find(Misc::ToUpper(sSpecialFolder)) == 0);
}

BOOL FileMisc::IsAppInProgramFiles()
{
	return IsPathInProgramFiles(GetAppFilePath());
}

BOOL FileMisc::IsPathInProgramFiles(const CString& sPath)
{
	return (IsPathInSpecialFolder(sPath, CSIDL_PROGRAM_FILES) ||
			IsPathInSpecialFolder(sPath, CSIDL_PROGRAM_FILESX86));
}

BOOL FileMisc::GetVirtualStorePath(const CString& sPath, CString& sVirtualStorePath)
{
	if (!Misc::Is64BitWindows())
		return FALSE;

	if (!IsPathInSpecialFolder(sPath, CSIDL_PROGRAM_FILESX86))
		return FALSE;

	// else
	CString sVirtualStoreFolder;
	VERIFY (FileMisc::GetSpecialFolder(CSIDL_LOCAL_APPDATA, sVirtualStoreFolder));
	
	// remove drive letter and add to appdata
	CString sFolder, sFile, sExt;
	FileMisc::SplitPath(sPath, NULL, &sFolder, &sFile, &sExt);
	
	sVirtualStorePath.Format(_T("%s\\VirtualStore\\%s\\%s%s"), sVirtualStoreFolder, sFolder, sFile, sExt);
	
	return TRUE;
}

BOOL FileMisc::AddToFileName(CString& sFilePath, LPCTSTR szExtra, BOOL bSuffix)
{
	if (sFilePath.IsEmpty())
		return FALSE;

	CString sDrive, sPath, sFName, sExt;

	FileMisc::SplitPath(sFilePath, &sDrive, &sPath, &sFName, &sExt);

	if (bSuffix)
		FileMisc::MakePath(sFilePath, sDrive, sPath, (sFName + szExtra), sExt);
	else
		FileMisc::MakePath(sFilePath, sDrive, sPath, (szExtra + sFName), sExt);

	return TRUE;
}

BOOL FileMisc::AddToFileName(CString& sFilePath, int nSuffix)
{
	return AddToFileName(sFilePath, Misc::Format(nSuffix), TRUE);
}

CString FileMisc::ResolveShortcut(LPCTSTR szShortcut)
{
	// start by checking its a valid file
	if (!FileExists(szShortcut))
		return "";

    CoInitialize(NULL);

	HRESULT hResult;
	IShellLink*	psl;
	CString sTarget(szShortcut);
	
	hResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
								IID_IShellLink, (LPVOID*)&psl);
	
	if (SUCCEEDED(hResult))
	{
		LPPERSISTFILE ppf;
		
		hResult = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hResult))
		{
			WCHAR wsz[MAX_PATH+1] = { 0 };	// buffer for Unicode string
#ifndef _UNICODE
			MultiByteToWideChar(CP_ACP, 0, szShortcut, -1, wsz, MAX_PATH);
#else
			lstrcpy(wsz, szShortcut);
#endif
			hResult = ppf->Load(wsz, STGM_READ);
			
			if (SUCCEEDED(hResult))
			{
				hResult = psl->Resolve(NULL, SLR_ANY_MATCH | SLR_NO_UI);

				if (SUCCEEDED(hResult))
				{
					TCHAR szPath[MAX_PATH+1] = { 0 };
					WIN32_FIND_DATA wfd;
//fabio_2005
#if _MSC_VER >= 1400
					_tcscpy_s(szPath, szShortcut);
#else
					_tcscpy(szPath, szShortcut);
#endif
					hResult = psl->GetPath(szPath, MAX_PATH, &wfd, SLGP_SHORTPATH);

					if (SUCCEEDED(hResult))
						sTarget = CString(szPath);
				}
			}
		
			ppf->Release();
		}
		
		psl->Release();
	}

	CoUninitialize();
	
	return sTarget;
}


CString FileMisc::GetLongPathName(LPCTSTR szShortPath)
{
// 	CString sLongPath(szShortPath);
// 
// 	// must link dynamically to kernel32 else problem with win95/NT4
// 	static HMODULE hLib = LoadLibrary(_T("kernel32.dll"));
// 
// 	if (hLib)
// 	{
// 		typedef DWORD (WINAPI *FNGETLONGPATHNAME)(LPCTSTR, LPTSTR, DWORD);
// 
// #ifdef _UNICODE
// 		FNGETLONGPATHNAME pFN = (FNGETLONGPATHNAME)GetProcAddress(hLib, "GetLongPathNameW");
// #else
// 		FNGETLONGPATHNAME pFN = (FNGETLONGPATHNAME)GetProcAddress(hLib, "GetLongPathNameA");
// #endif
// 		if (pFN)
// 		{
// 			TCHAR szLongPath[MAX_PATH+1] = { 0 };
// 			pFN(szShortPath, szLongPath, MAX_PATH);
// 
// 			sLongPath = szLongPath;
// 		}
// 	}
// 
// 	return sLongPath;

	TCHAR szLongPath[MAX_PATH+1] = { 0 };
	::GetLongPathName(szShortPath, szLongPath, MAX_PATH);

	return szLongPath;
}

int FileMisc::GetDropFilePaths(COleDataObject* pObject, CStringArray& aFiles)
{
	int nRet = 0;
	aFiles.RemoveAll();

	// first try
	if (pObject->IsDataAvailable(CF_HDROP)) 
	{
		HGLOBAL hGlobal = pObject->GetGlobalData(CF_HDROP);

		if (hGlobal)
		{
			HDROP hDrop = (HDROP)GlobalLock(hGlobal);
			nRet = FileMisc::GetDropFilePaths(hDrop, aFiles);

			GlobalUnlock(hGlobal);
		}
	}

	// second try
	if (nRet == 0 && pObject->IsDataAvailable(CBF_FILENAME))
	{
		HGLOBAL hGlobal = pObject->GetGlobalData(CBF_FILENAME);

		if (hGlobal)
		{
			HDROP hDrop = (HDROP)GlobalLock(hGlobal);
			CString sFilename = GetLongPathName((LPCTSTR)hDrop); // copy
			aFiles.Add(sFilename);

			GlobalUnlock(hGlobal);
		}

		nRet = aFiles.GetSize();
	}

	return nRet;
}

