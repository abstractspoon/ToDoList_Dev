#ifndef _MISCFILE_FUNCTIONS_H_
#define _MISCFILE_FUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "..\3rdParty\stdiofileex.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef __int64 time64_t;

///////////////////////////////////////////////////////////////////////////////////////////////////

const UINT SE_ERR_SUCCESS = 33;

///////////////////////////////////////////////////////////////////////////////////////////////////

enum 
{
	FBS_TIMESTAMP	= 0x01,
	FBS_APPVERSION	= 0x02,
	FBS_OVERWRITE	= 0x04,
};

enum FB_MODULE_SORT
{
	FBM_UNSORTED,
	FBM_SORTBY_HMODULE,
	FBM_SORTBY_FILENAME,
};

enum // Delete/Copy/MoveFolder
{
	FMDF_SUBFOLDERS				= 0x01,
	FMDF_PROCESSMSGLOOP			= 0x02,
	FMDF_ALLOWDELETEONREBOOT	= 0x04, // delete/move only
	FMDF_HIDDENREADONLY			= 0x08,
	FMDF_OVERWRITE				= 0x10, // copy/move only
	FMDF_NEWER					= (0x20 | FMDF_OVERWRITE), // copy/move only
};

enum // Logging
{
	FMLF_NORESET	= 0x01,
	FMLF_TIMESTAMP	= 0x02,
	FMLF_TEMPDIR	= 0x04, 
	FMLF_NOAPPDIR	= 0x08, 
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CSIDL_COMMON_APPDATA

enum // GetSpecialFolder
{
	CSIDL_COMMON_APPDATA       = 0x0023,        // All Users\Application Data
	CSIDL_WINDOWS              = 0x0024,        // GetWindowsDirectory()
	CSIDL_SYSTEM               = 0x0025,        // GetSystemDirectory()
	CSIDL_PROGRAM_FILES        = 0x0026,        // C:\Program Files
	CSIDL_MYPICTURES           = 0x0027,        // C:\Program Files\My Pictures
	CSIDL_PROFILE              = 0x0028,        // USERPROFILE
	CSIDL_PROGRAM_FILES_COMMON = 0x002b,        // C:\Program Files\Common
	CSIDL_COMMON_TEMPLATES     = 0x002d,        // All Users\Templates
	CSIDL_COMMON_DOCUMENTS     = 0x002e,        // All Users\Documents
	CSIDL_COMMON_ADMINTOOLS    = 0x002f,        // All Users\Start Menu\Programs\Administrative Tools
	CSIDL_ADMINTOOLS           = 0x0030,        // <user name>\Start Menu\Programs\Administrative Tools
	CSIDL_CONNECTIONS          = 0x0031,        // Network and Dial-up Connections
	CSIDL_COMMON_MUSIC         = 0x0035,        // All Users\My Music
	CSIDL_COMMON_PICTURES      = 0x0036,        // All Users\My Pictures
	CSIDL_COMMON_VIDEO         = 0x0037,        // All Users\My Video
	CSIDL_RESOURCES            = 0x0038,        // Resource Direcotry
	CSIDL_RESOURCES_LOCALIZED  = 0x0039,        // Localized Resource Direcotry
	CSIDL_COMMON_OEM_LINKS     = 0x003a,        // Links to All Users OEM specific apps
	CSIDL_CDBURN_AREA          = 0x003b,        // USERPROFILE\Local Settings\Application Data\Microsoft\CD Burning
	CSIDL_COMPUTERSNEARME      = 0x003d,        // Computers Near Me (computered from Workgroup membership)
};

#endif

#ifndef CSIDL_LOCAL_APPDATA
#define CSIDL_LOCAL_APPDATA      0x001c         // <user name>\Local Settings\Applicaiton Data (non roaming)
#endif // CSIDL_LOCAL_APPDATA

#ifndef CSIDL_PROGRAM_FILESX86
#define CSIDL_PROGRAM_FILESX86   0x002a         // x86 C:\Program Files on RISC
#endif // CSIDL_PROGRAM_FILESX86

///////////////////////////////////////////////////////////////////////////////////////////////////

class CFileBackup
{
public:
	CFileBackup(const CString& sFile = _T(""), 
				DWORD dwFlags = 0, 
				const CString& sFolder = _T(""), 
				const CString& sExt = _T(".bak"));
	~CFileBackup();

	BOOL MakeBackup(const CString& sFile, 
					DWORD dwFlags = 0, 
					const CString& sFolder = _T(""), 
					const CString& sExt = _T(".bak"));
	BOOL RestoreBackup();

	static CString BuildBackupPath(const CString& sFile, 
									DWORD dwFlags = 0, 
									const CString& sFolder = _T(""), 
									const CString& sExt = _T(".bak"));

protected:
	CString m_sFile, m_sBackup;

};

///////////////////////////////////////////////////////////////////////////////////////////////////

class CTempFileBackup : public CFileBackup
{
public:
	CTempFileBackup(const CString& sFile = _T(""), 
					DWORD dwFlags = 0, 
					const CString& sFolder = _T(""), 
					const CString& sExt = _T(".bak"));	
	~CTempFileBackup(); // auto deletes backup file

};

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace FileMisc
{
	CString& TerminatePath(CString& sPath, BOOL bTerminate = TRUE);
	CString TerminatePath(LPCTSTR szPath, BOOL bTerminate = TRUE);
	BOOL IsPathTerminated(const CString& sPath);

	time64_t GetFileLastModified(LPCTSTR szPath);
	BOOL GetFileLastModified(LPCTSTR szPath, FILETIME& fileTime); 
	BOOL SetFileLastModified(LPCTSTR szPath, const FILETIME& fileTime); 
	BOOL GetFileLastModified(LPCTSTR szPath, SYSTEMTIME& sysTime, BOOL bLocalTime = TRUE);
	BOOL TouchFile(LPCTSTR szPath); // resets to current time
	double GetFileSize(LPCTSTR szPath);

	double GetFolderSize(LPCTSTR szFolder, 
						 BOOL bIncludeSubFolders = TRUE, 
						 LPCTSTR szFileMask = NULL, 
						 HANDLE hTerminate = NULL, 
						 BOOL bProcessMsgLoop = FALSE);

	BOOL CreateFolder(LPCTSTR szFolder);
	BOOL CreateFolderFromFilePath(LPCTSTR szFilePath);
	BOOL FolderExists(LPCTSTR szFolder);
	BOOL FileExists(LPCTSTR szFile);
	BOOL IsPath(LPCTSTR szPath);
	BOOL PathExists(LPCTSTR szPath); // file or folder
	BOOL FolderFromFilePathExists(LPCTSTR szFilePath);
	BOOL PathHasWildcard(LPCTSTR szFilePath);
	BOOL IsFileWritable(LPCTSTR szFilePath);
	BOOL IsFileReadable(LPCTSTR szFilePath);
	BOOL IsFolderWritable(LPCTSTR szFolder);
	BOOL CanonicalizePath(CString& sFilePath);
	int GetExistingFiles(const CStringArray& aFilePaths, CStringArray& aExistPaths);
	int GetNonWritableFiles(const CStringArray& aExistPaths, CStringArray& aNonWritablePaths);

	CString GetCwd();
	void SetCwd(const CString& sCwd);

	BOOL Find(LPCTSTR szSearchSpec);
	BOOL FindFirst(LPCTSTR szSearchSpec, CString& sPath);
	int FindFiles(const CString& sFolder, CStringArray& aFiles, BOOL bCheckSubFolders = TRUE, LPCTSTR szPattern = _T("*.*"));
	BOOL FolderContainsFiles(LPCTSTR szFolder, BOOL bCheckSubFolders = TRUE, LPCTSTR szFilter = _T("*.*"));

	CString& ValidateFileName(CString& sFilename, LPCTSTR szReplace = _T(""));
	CString& ValidateFilePath(CString& sFilepath, LPCTSTR szReplace = _T(""));

	BOOL HasExtension(LPCTSTR szFilePath, LPCTSTR szExt);
	CString GetExtension(LPCTSTR szFilePath, BOOL bWithDot = TRUE);
	CString FormatExtension(LPCTSTR szExt, BOOL bWithDot = TRUE);
	CString& ReplaceExtension(CString& sFilePath, LPCTSTR szExt);
	CString& RemoveExtension(CString& sFilePath);
	
	CString GetTempFolder();
	CString GetTempFilePath(LPCTSTR szPrefix = NULL, UINT uUnique = 0);
	CString GetTempFilePath(LPCTSTR szFilename, LPCTSTR szExt);
	BOOL IsTempFilePath(LPCTSTR szFilename);

	BOOL CanReadFile(LPCTSTR szPathname, BOOL bDenyWrite = FALSE);
	BOOL LoadFile(LPCTSTR szPathname, CString& sText, BOOL bDenyWrite = FALSE, UINT nAnsiCodePage = CP_ACP);
	int LoadFile(LPCTSTR szPathname, CStringArray& aLines, int nLineCount = -1, UINT nAnsiCodePage = CP_ACP);

	BOOL SaveFile(LPCTSTR szPathname, const CString& sText, SFE_FORMAT nFormat, UINT nAnsiCodePage = CP_ACP);
	BOOL SaveFile(LPCTSTR szPathname, LPCSTR szText); // save as multibyte

	BOOL ExtractResource(UINT nID, LPCTSTR szType, const CString& sTempFilePath, HINSTANCE hInst = NULL);
	BOOL ExtractResource(LPCTSTR szModulePath, UINT nID, LPCTSTR szType, const CString& sTempFilePath);

	CString GetProcessFilePath(HANDLE hProcess);
	BOOL IsAdminProcess(HANDLE hProcess = NULL);

	CString GetModuleFilePath(HMODULE hMod = NULL);
	CString GetModuleFolder(HMODULE hMod = NULL);
	int GetModuleDriveType(HMODULE hMod = NULL);
	CString GetModuleVersion(HMODULE hMod = NULL, TCHAR cSep = '.');
	CString GetModuleVersion(LPCTSTR szModulePath, TCHAR cSep = '.');
	BOOL GetModuleVersion(HMODULE hMod, CDWordArray& aVersionParts);
	BOOL GetModuleVersion(LPCTSTR szModulePath, CDWordArray& aVersionParts);
	CString GetWindowModuleFilePath(HWND hWnd);
	BOOL IsNativeModule(HMODULE hMod = NULL); // returns TRUE, FALSE, -1
	BOOL IsNativeModule(LPCTSTR szModulePath); // returns TRUE, FALSE, -1

	CString GetAppFilePath();
	CString GetAppFolder(LPCTSTR szSubFolder = NULL);
	CString GetAppResourceFolder(LPCTSTR szResFolder = _T("Resources"));

	// returns the exe and all the dlls, etc, in use by the app
	int GetAppModuleFilePaths(CStringArray& aFileNames, FB_MODULE_SORT nSort = FBM_UNSORTED);
	int GetAppModuleHandles(CDWordArray& aHandles, BOOL bSorted = TRUE);
	
	CString GetAppVersion(TCHAR cSep = '.');
	BOOL GetAppVersion(CDWordArray& aVersionParts);
	int SplitVersionNumber(LPCTSTR szVersion, CDWordArray& aVersionParts);
	int CompareVersions(LPCTSTR szVersion1, LPCTSTR szVersion2, int nNumItems = 0);
	int CompareVersions(CDWordArray& aVersion1, CDWordArray& aVersion2, int nNumItems = 0);
	CString GetPrevAppVersion(DWORD nMaxVer2 = 0, DWORD nMaxVer3 = 0, DWORD nMaxVer4 = 0);
	CString GetPrevAppVersion(LPCTSTR szVersion, DWORD nMaxVer2 = 0, DWORD nMaxVer3 = 0, DWORD nMaxVer4 = 0);
	BOOL GetPrevAppVersion(CDWordArray& aVersionParts, DWORD nMaxVer2 = 0, DWORD nMaxVer3 = 0, DWORD nMaxVer4 = 0);

	CString GetWindowsFolder();
	CString GetWindowsSystemFolder();

	BOOL GetSpecialFolder(int nCSIDL, CString& sFolder, BOOL bAutoCreate = FALSE);
	CString GetSpecialFolder(int nCSIDL);
	BOOL GetSpecialFilePath(int nCSIDL, const CString& sFileName, CString& sFilePath);
	BOOL IsAppInProgramFiles();
	BOOL IsPathInProgramFiles(const CString& sFilePath);
	BOOL IsPathInSpecialFolder(const CString& sFilePath, int nCSIDL);

	BOOL GetVirtualStorePath(const CString& sPath, CString& sVirtualStorePath);

	void SplitPath(LPCTSTR szPath, CString* pDrive, CString* pDir = NULL, CString* pFName = NULL, CString* pExt = NULL);
	CString& MakePath(CString& sPath, LPCTSTR szDrive, LPCTSTR szDir = NULL, LPCTSTR szFName = NULL, LPCTSTR szExt = NULL);

	CString& PathConcat(CString& sDir, LPCTSTR szDirOrFName);
	CString PathConcat(LPCTSTR szDir, LPCTSTR szDirOrFName);
	
	CString GetRelativePath(const CString& sFilePath, const CString& sRelativeToFolder, BOOL bFolder);
	CString& MakeRelativePath(CString& sFilePath, const CString& sRelativeToFolder, BOOL bFolder);
	CString GetFullPath(const CString& sFilePath, const CString& sRelativeToFolder = GetCwd());
	CString& MakeFullPath(CString& sFilePath, const CString& sRelativeToFolder = GetCwd());

	BOOL ResolveShortcut(LPCTSTR szShortcut, CString& sTargetPath);
	BOOL CreateShortCut(LPCTSTR szTargetFile, LPCTSTR szShortcut, 
						LPCTSTR szTargetArgs = NULL, LPCTSTR szDescription = NULL,
						int iShowMode = SW_SHOW, LPCTSTR szCurDir = NULL,
						LPCTSTR szIconFile = NULL, int iIconIndex = -1);
	CString GetLongPathName(LPCTSTR szShortPath);
	BOOL AddToFileName(CString& sFilePath, LPCTSTR szExtra, BOOL bSuffix = TRUE);
	BOOL AddToFileName(CString& sFilePath, int nSuffix);
	
	BOOL IsSamePath(const CString& sPath1, const CString& sPath2);
	int CompareContents(const CString& sPath1, const CString& sPath2);

	CString GetFolderFromFilePath(LPCTSTR szFilePath);
	CString GetFileNameFromPath(LPCTSTR szFilepath, BOOL bIncExtension = TRUE);

	int DeleteFolder(LPCTSTR szFolder,
					DWORD dwFlags = FMDF_SUBFOLDERS,
					HANDLE hTerminate = NULL);
	
	int DeleteFolderContents(LPCTSTR szFolder, 
							DWORD dwFlags,
							LPCTSTR szFileMask = NULL, 
							HANDLE hTerminate = NULL);
	
	
	// will delete the source folder on success
	int  MoveFolder(LPCTSTR szSrcFolder, 
					LPCTSTR szDestFolder, 
					DWORD dwFlags = FMDF_PROCESSMSGLOOP, 
					HANDLE hTerminate = NULL);

	BOOL CopyFolder(LPCTSTR szSrcFolder, 
					LPCTSTR szDestFolder, 
					DWORD dwFlags = FMDF_PROCESSMSGLOOP, 
					HANDLE hTerminate = NULL);

	// will delete the source folder only if file mask was "*.*"
	int  MoveFolder(LPCTSTR szSrcFolder, 
					LPCTSTR szDestFolder, 
					LPCTSTR szFileMask, 
					DWORD dwFlags = FMDF_PROCESSMSGLOOP, 
					HANDLE hTerminate = NULL);

	BOOL CopyFolder(LPCTSTR szSrcFolder, 
					LPCTSTR szDestFolder, 
					LPCTSTR szFileMask, 
					DWORD dwFlags = FMDF_PROCESSMSGLOOP, 
					HANDLE hTerminate = NULL);

	BOOL DeleteFile(LPCTSTR szPath, BOOL bHiddenReadOnly = FALSE);
	BOOL DeleteFileBySize(LPCTSTR szFilePath, double dSize, BOOL bHiddenReadOnly = FALSE);

	BOOL CopyFile(LPCTSTR szSrcPath, LPCTSTR szDestPath, BOOL bOverwrite = FALSE, BOOL bHiddenReadOnly = FALSE, BOOL bNewer = FALSE);
	BOOL MoveFile(LPCTSTR szSrcPath, LPCTSTR szDestPath, BOOL bOverwrite = FALSE, BOOL bHiddenReadOnly = FALSE);

	BOOL ModifyAttributes(LPCTSTR szPath, DWORD dwRemove, DWORD dwAdd, DWORD dwExistAttrib = 0xFFFFFFFF);
	BOOL ModifyFolderAndContentAttributes(LPCTSTR szFolder, DWORD dwRemove, DWORD dwAdd, BOOL bSubFolders = FALSE);

	BOOL IsLoggingEnabled();
	CString GetLogFilePath();
	BOOL EnableLogging(BOOL bEnable, LPCTSTR szAppDataKey = NULL, DWORD dwFlags = FMLF_TIMESTAMP);
	void LogTimeElapsed(DWORD& dwTickStart, LPCTSTR szFormat, ...);
	void LogText(LPCTSTR szText, ...);
	void LogTextRaw(LPCTSTR szText);
	void LogAppModuleState(FB_MODULE_SORT nSort = FBM_UNSORTED);

	DWORD Run(HWND hwnd, LPCTSTR lpFile, LPCTSTR lpParams = NULL, int nShowCmd = SW_SHOWNORMAL, LPCTSTR lpDirectory = NULL, LPCTSTR lpVerb = NULL);
	BOOL SelectFileInExplorer(LPCTSTR szFilePath);
	BOOL AppendLineToFile(LPCTSTR szPathname, const CString& sLine, SFE_FORMAT nFormat);
	BOOL ExpandPathEnvironmentVariables(CString& sFilePath);

	int GetDropFilePaths(HDROP hDrop, CStringArray& aFiles);
	int GetDropFilePaths(COleDataObject* pObject, CStringArray& aFiles);

}

#endif // _MISCFILE_FUNCTIONS_H_