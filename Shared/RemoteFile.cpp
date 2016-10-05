// RemoteFile.cpp: implementation of the CRemoteFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RemoteFile.h"

#include "serverdlg.h"
#include "RemoteFiledialog.h"
#include "progressdlg.h"
#include "folderdialog.h"
#include "filemisc.h"
#include "misc.h"
#include "enstring.h"

#include <Shlwapi.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const UINT BUFSIZE = 5 * 1024;
LPCTSTR ALLFILES = _T("All Files (*.*)\0*.*\0");

CRemoteFile::CRemoteFile(LPCTSTR szAgent, LPCTSTR szServer, LPCTSTR szUsername, 
						 LPCTSTR szPassword, LPCTSTR szProxy, CWnd* pParent)
	 : m_sAgent(szAgent),
	  m_sProxyAndPort(szProxy),
	  m_sServer(szServer), 
	  m_sUsername(szUsername), 
	  m_sPassword(szPassword),
	  m_pSession(NULL),
	  m_pConnection(NULL),
	  m_pParent(pParent)
{
	Misc::Trim(m_sServer);
	Misc::Trim(m_sUsername);
	Misc::Trim(m_sPassword);

	if (m_pParent == NULL)
		m_pParent = AfxGetMainWnd();
}

CRemoteFile::~CRemoteFile()
{
	CloseConnection();
	CloseSession();
}

RMERR CRemoteFile::SaveErrorMsg(RMERR nErr, LPCTSTR szLocalPath, LPCTSTR szRemotePath)
{
	m_sLastError.Empty();

	switch (nErr)
	{
	case RMERR_SUCCESS:
		break;

	case RMERR_REMOTEPATH:
		m_sLastError = _T("The remote file could not be found or was not correctly specified");
		break;

	case RMERR_LOCALPATH:
		m_sLastError = _T("The local file could not be found or was not correctly specified");
		break;

	case RMERR_OPENINGREMOTEFILETOREAD:
		m_sLastError.Format(_T("The remote file (%s) could not be opened for reading"), szRemotePath);
		break;

	case RMERR_OPENINGREMOTEFILETOWRITE:
		m_sLastError.Format(_T("The remote file (%s) could not be opened for writing"), szRemotePath);
		break;

	case RMERR_OPENINGLOCALFILETOREAD:
		m_sLastError.Format(_T("The local file (%s) could not be opened for reading"), szLocalPath);
		break;

	case RMERR_OPENINGLOCALFILETOWRITE:
		m_sLastError.Format(_T("The local file (%s) could not be opened for writing"), szLocalPath);
		break;

	case RMERR_CONNECTING:
		m_sLastError.Format(_T("The remote server (%s) did not accept the connection"), m_sServer);
		break;

	case RMERR_USERCANCELLED:
		m_sLastError.Format(_T("The user cancelled"));
		break;

	case RMERR_NOTCONNECTED:
		m_sLastError.Format(_T("There is no current connection to the server (%s)"), m_sServer);
		break;

	case RMERR_READINGREMOTEFILE:
		m_sLastError.Format(_T("The remote file (%s) could not be read"), szRemotePath);
		break;

	case RMERR_WRITINGREMOTEFILE:
		m_sLastError.Format(_T("The remote file (%s) could not be written to"), szRemotePath);
		break;

	case RMERR_READINGLOCALFILE:
		m_sLastError.Format(_T("The local file (%s) could not be read"), szLocalPath);
		break;

	case RMERR_WRITINGLOCALFILE:
		m_sLastError.Format(_T("The local file (%s) could not be written to"), szLocalPath);
		break;

	default:
	case RMERR_UNKNOWN:
		if (m_dwInternetErr)
			m_sLastError.Format(_T("An internet error occurred (%d)"), m_dwInternetErr);
		else
			m_sLastError = _T("An unknown error occurred");
	}

	return nErr;
}

// download
RMERR CRemoteFile::GetFile(CString& sRemotePath, CString& sLocalPath, IPreferences* pPrefs, LPCTSTR szKey, DWORD dwOptions, LPCTSTR szFilter)
{
	m_pPrefs = pPrefs;
	m_sPrefKey = szKey;

	ValidateOptions(dwOptions, TRUE);

	RMERR nRes = RMERR_SUCCESS;

	if (!EstablishConnection(nRes, dwOptions))
		return SaveErrorMsg(nRes);

	// remote path(s)
	CFRArray aRemoteFiles;

	if (nRes == RMERR_SUCCESS)
	{
		Misc::Trim(sRemotePath);

		nRes = GetRemotePaths(aRemoteFiles, dwOptions, szFilter, sRemotePath);
	}

	// local file(s)
	CStringArray aLocalFiles;
	BOOL bTemp = FALSE;

	if (nRes == RMERR_SUCCESS)
	{
		Misc::Trim(sLocalPath);

		// if we are downloading more than one file
		// or the local path has not been specified correctly then get it
		BOOL bLocalPathIsDir = FileMisc::FolderExists(sLocalPath);
		BOOL bMulti = (aRemoteFiles.GetSize() > 1);

		if (bMulti || bLocalPathIsDir || !FileMisc::CreateFolderFromFilePath(sLocalPath))
		{
			CString sInitialDir;

			// try to create/get the initial dir
			if (!bLocalPathIsDir)
			{
				if (!sLocalPath.IsEmpty() && FileMisc::CreateFolderFromFilePath(sLocalPath))
				{
					TCHAR szDrive[_MAX_DRIVE+1] = { 0 }, szPath[MAX_PATH+1] = { 0 };
					_tsplitpath(sLocalPath, szDrive, szPath, NULL, NULL);

					sInitialDir.Format(_T("%s%s"), szDrive, szPath);
				}
			}
			else
				sInitialDir = sLocalPath;

			nRes = GetLocalPaths(aLocalFiles, bTemp, aRemoteFiles, dwOptions, sInitialDir);
		}
		else // single file 
			aLocalFiles.Add(sLocalPath);
	}

	if (nRes == RMERR_SUCCESS && aRemoteFiles.GetSize())
	{
		CProgressDlg progDlg(!(dwOptions & RMO_NOCANCELPROGRESS));
		BOOL bProgress = !(dwOptions & RMO_NOPROGRESS);

		if (bProgress)
		{
			progDlg.Create(_T("Download Progress"), NULL, m_pParent);
			ASSERT (progDlg.GetSafeHwnd());
		}
		
		BOOL bConfirmOverwrite = (dwOptions & RMO_CONFIRMOVERWRITE);
		int nFileCount = aRemoteFiles.GetSize();

		// clear sRemotePath because we are going to re-use it
		// to return the downloaded files
		sRemotePath.Empty();
		
		for (int nFile = 0; nFile < nFileCount; nFile++)
		{
			if (nFileCount > 1 && bProgress)
			{
				CEnString sDlgTitle;
				sDlgTitle.Format(_T("Download Progress (%d of %d)"), nFile + 1, nFileCount);
				progDlg.SetCaption(sDlgTitle);
			}
			
			RMERR nFileRes = DownloadFile(&aRemoteFiles[nFile], aLocalFiles[nFile], bConfirmOverwrite, &progDlg);
			
			// handle single files
			if (nFileCount == 1)
				nRes = nFileRes;

			// concatenate remote names together delimited by ;
			if (!sRemotePath.IsEmpty())
				sRemotePath += ';';

			sRemotePath += aRemoteFiles[nFile].sFilePath;
		}
		
		if (bProgress && m_pParent)
			m_pParent->EnableWindow(TRUE);
	}

	// cleanup
	if (nRes != RMERR_SUCCESS)
	{
		if (bTemp)
		{
			int nFile = aLocalFiles.GetSize();

			while (nFile--)
				::DeleteFile(aLocalFiles[nFile]);
		}
	}
	// join downloaded files delimited by ;
	else 
		sLocalPath = Misc::FormatArray(aLocalFiles, _T(";"));

	CloseConnection();

	return SaveErrorMsg(nRes, sLocalPath, sRemotePath);
}

// upload
RMERR CRemoteFile::SetFile(CString& sLocalPath, CString& sRemotePath, IPreferences* pPrefs, LPCTSTR szKey, DWORD dwOptions)
{
	m_pPrefs = pPrefs;
	m_sPrefKey = szKey;

	ValidateOptions(dwOptions, FALSE);

	RMERR nRes = RMERR_SUCCESS;

	// if local file not specified then get browse for it
	CStringArray aLocalFiles;

	nRes = GetLocalPaths(aLocalFiles, dwOptions, sLocalPath);

	// now we support wildcards we must allow for there being no
	// files matching the upload which is not an error
	if (nRes != RMERR_SUCCESS)
		return SaveErrorMsg(nRes);

	else if (!aLocalFiles.GetSize())
		return RMERR_SUCCESS;

	// now we start the connection
	if (!EstablishConnection(nRes, dwOptions))
		return SaveErrorMsg(nRes);

	// if remote path not specified or is a folder then allow browsing unless bAllowDialog == FALSE
	CFRArray aRemoteFiles;

	if (nRes == RMERR_SUCCESS)
	{
		Misc::Trim(sRemotePath);

		// figure out the local root folder
		CString sLocalRoot;

		if (dwOptions & RMO_PRESERVESTRUCTURE)
		{
			sLocalRoot = FileMisc::GetFolderFromFilePath(sLocalPath);

			// check all files share this root
			// TODO
		}
		
		nRes = GetRemotePaths(aRemoteFiles, aLocalFiles, dwOptions, sRemotePath, sLocalRoot);
	}

	if (nRes == RMERR_SUCCESS && aRemoteFiles.GetSize())
	{
		CProgressDlg progDlg(!(dwOptions & RMO_NOCANCELPROGRESS));
		BOOL bProgress = !(dwOptions & RMO_NOPROGRESS);

		if (bProgress)
		{
			progDlg.Create(_T("Upload Progress"), NULL, m_pParent);
			ASSERT (progDlg.GetSafeHwnd());
		}

		int nFileCount = aRemoteFiles.GetSize();
		
		for (int nFile = 0; nFile < nFileCount; nFile++)
		{
			if (bProgress && nFileCount > 1)
			{
				CEnString sDlgTitle;
				sDlgTitle.Format(_T("Upload Progress (%d of %d)"), nFile + 1, nFileCount);
				progDlg.SetCaption(sDlgTitle);
			}
			
			RMERR nFileRes = UploadFile(aLocalFiles[nFile], &aRemoteFiles[nFile], dwOptions, &progDlg);
			
			// handle single files
			if (nFileCount == 1)
			{
				nRes = nFileRes;
				sRemotePath = aRemoteFiles[0].sFilePath;
				sLocalPath = aLocalFiles[0];
			}
		}
		
		if (bProgress && m_pParent)
			m_pParent->EnableWindow(TRUE);
	}

	CloseConnection();

	return SaveErrorMsg(nRes, sLocalPath, sRemotePath);
}

BOOL CRemoteFile::RemotePathIsFolder(const CString& sPath)
{
	CString sFolder(sPath);
	
	sFolder.Replace('\\', '/');
	sFolder.TrimRight();

	return (sFolder.GetAt(sFolder.GetLength() - 1) == '/');
}

// for upload
RMERR CRemoteFile::GetRemotePaths(CFRArray& aRemoteFiles, const CStringArray& aLocalFiles, 
								  DWORD dwOptions, LPCTSTR szRemoteDir, LPCTSTR szLocalRoot)
{
	aRemoteFiles.RemoveAll();

	// must have one or more files for upload
	int nNumLocal = aLocalFiles.GetSize();

	if (!nNumLocal)
		return RMERR_LOCALPATH;

	// if the remote path is empty then we display an appropriate dialog
	CString sRemotePath(szRemoteDir);
	Misc::Trim(sRemotePath);
	sRemotePath.Replace('\\', '/');

	if (sRemotePath.IsEmpty())
	{
		BOOL bAllowDialog = (dwOptions & RMO_ALLOWDIALOG);

		if (!bAllowDialog)
			return RMERR_REMOTEPATH;

		sRemotePath = m_pPrefs->GetProfileString(_T("RemoteSettings\\LastUploadFolder"), m_sServer);

		// if multiple files are being uploaded then display folder dialog
		if (nNumLocal > 1)
		{
			CRemoteFileDialog dialog(m_pConnection, m_sServer);
			
			if (dialog.DoModal(m_pPrefs, m_sPrefKey, RFD_UPLOAD | RFD_FOLDERSELECT, sRemotePath) == IDOK)
			{
				if (dialog.GetPathCount())
				{
					sRemotePath = dialog.GetFirstPath();
					ASSERT (!sRemotePath.IsEmpty());
				}
				else if (!dialog.IsRootFolder())
				{
					return RMERR_USERCANCELLED;
				}
				
				for (int nFile = 0; nFile < aLocalFiles.GetSize(); nFile++)
				{
					FILERESULT frRemote(MakeRemotePath(aLocalFiles[nFile], sRemotePath, szLocalRoot));
					aRemoteFiles.Add(frRemote);
				}
				
				m_pPrefs->WriteProfileString(_T("RemoteSettings\\LastUploadFolder"), m_sServer, sRemotePath);
			}
			else
				return RMERR_USERCANCELLED;
		}
		else // nNumLocal == 1
		{
			CRemoteFileDialog dialog(m_pConnection, m_sServer, NULL, sRemotePath);

			// base remote name on local name
			TCHAR szFilename[_MAX_FNAME], szExt[_MAX_EXT];
			_tsplitpath(aLocalFiles[0], NULL, NULL, szFilename, szExt);
			lstrcat(szFilename, szExt);
			
			if (dialog.DoModal(m_pPrefs, m_sPrefKey, RFD_UPLOAD, szFilename) == IDOK)
				dialog.GetPaths(aRemoteFiles);
			else
				return RMERR_USERCANCELLED;
		}
	}
	else 
	{
		// if its a folder then create full remote file paths by simply
		// appending the local filenames to the remote folder
		if (RemotePathIsFolder(sRemotePath))
		{
			for (int nFile = 0; nFile < aLocalFiles.GetSize(); nFile++)
			{
				FILERESULT frRemote(MakeRemotePath(aLocalFiles[nFile], sRemotePath, szLocalRoot));
				aRemoteFiles.Add(frRemote);
			}
		}
		// else we assume its a full file path in which case it's only
		// okay for single local files
		else if (nNumLocal == 1)
		{
			FILERESULT frRemote(sRemotePath);
			aRemoteFiles.Add(frRemote);
		}
		else
			return RMERR_REMOTEPATH;
	}
	
	return RMERR_SUCCESS;
}

CString CRemoteFile::MakeRemotePath(const CString& sLocalPath, const CString& sRemoteDir, LPCTSTR szLocalRoot)
{
	TCHAR szRemotePath[MAX_PATH+1] = { 0 };
	int nRootLen = lstrlen(szLocalRoot);

	if (nRootLen) // == 'preserve structure'
	{
		_tmakepath(szRemotePath, NULL, sRemoteDir, ((LPCTSTR)sLocalPath) + nRootLen, NULL);
	}
	else
	{
		TCHAR szFilename[_MAX_FNAME], szExt[_MAX_EXT];

		_tsplitpath(sLocalPath, NULL, NULL, szFilename, szExt);
		_tmakepath(szRemotePath, NULL, sRemoteDir, szFilename, szExt);
	}
	
	return szRemotePath;				
}

// for download
RMERR CRemoteFile::GetRemotePaths(CFRArray& aRemoteFiles, DWORD dwOptions, LPCTSTR szFilter, LPCTSTR szRemoteDir)
{
	CString sRemotePath(szRemoteDir);
	Misc::Trim(sRemotePath);
	sRemotePath.Replace('\\', '/');

	// if the remote path is empty or a folder then we must display a dialog
	// to get the actual file of interest
	if (sRemotePath.IsEmpty() || RemotePathIsFolder(sRemotePath))
	{
		BOOL bAllowDialog = (dwOptions & RMO_ALLOWDIALOG);

		if (!bAllowDialog)
			return RMERR_REMOTEPATH;

		// get last folder browsed for this server
		sRemotePath = m_pPrefs->GetProfileString(_T("RemoteSettings\\LastDownloadFolder"), m_sServer);

		// browse for file(s)
		CWaitCursor cursor;
		CRemoteFileDialog dialog(m_pConnection, m_sServer, szFilter, sRemotePath);
		DWORD dwRFDOptions = RFD_DOWNLOAD | RFD_FILEMUSTEXIST;
		
		if (dwOptions & RMO_MULTISELECT)
			dwRFDOptions |= RFD_MULTISELECT;
		
		if (dwOptions & RMO_NONAVIGATE)
			dwRFDOptions &= RFD_NONAVIGATE;
		
		if (dialog.DoModal(m_pPrefs, m_sPrefKey, dwRFDOptions) == IDOK && dialog.GetPathCount())
		{
			dialog.GetPaths(aRemoteFiles);

			m_pPrefs->WriteProfileString(_T("RemoteSettings\\LastDownloadFolder"), m_sServer, sRemotePath);
		}
		else
			return RMERR_USERCANCELLED;
	}
	// else we must assume that the remote path is the full filename
	else
	{
		FILERESULT frRemote(sRemotePath);
		aRemoteFiles.Add(frRemote);
	}
		
	return RMERR_SUCCESS;
}

// for download
RMERR CRemoteFile::GetLocalPaths(CStringArray& aLocalFiles, BOOL& bTemp, const CFRArray& aRemoteFiles, DWORD dwOptions, LPCTSTR szLocalDir)
{
	aLocalFiles.RemoveAll();

	BOOL bAllowDialog = (dwOptions & RMO_ALLOWDIALOG);
	bTemp = (dwOptions & RMO_USETEMPFILE);

	if (bTemp)
	{
		for (int nFile = 0; nFile < aRemoteFiles.GetSize(); nFile++)
		{
			CString sLocalPath = GetTempPath(aRemoteFiles[nFile].sFileName, dwOptions);

			if (!sLocalPath.IsEmpty())
				aLocalFiles.Add(sLocalPath);
			else
			{
				aLocalFiles.RemoveAll();
				return RMERR_LOCALPATH;
			}
		}
	}
	else if (bAllowDialog)
	{
		CString sFolder(szLocalDir);
		BOOL bCreatePath = (dwOptions & RMO_CREATEDOWNLOADDIR);
		
		if (!ValidateLocalFolder(sFolder, bCreatePath))
			sFolder = m_pPrefs->GetProfileString(_T("LocalSettings\\LastDownloadFolder"), m_sServer);
		
		// if multiple remote files are being downloaded then display folder dialog
		if (aRemoteFiles.GetSize() > 1)
		{
			CFolderDialog dialog(_T("Select the Folder to which you want to download your chosen files"), 
									sFolder, m_pParent);

			if (dialog.DoModal() == IDOK)
			{
				CString sFolder = dialog.GetFolderPath();
				TCHAR szLocalPath[MAX_PATH+1] = { 0 };

				for (int nFile = 0; nFile < aRemoteFiles.GetSize(); nFile++)
				{
					_tmakepath(szLocalPath, NULL, sFolder, aRemoteFiles[nFile].sFileName, NULL);
					aLocalFiles.Add(szLocalPath);
				}

				// save folder location for next time
				m_pPrefs->WriteProfileString(_T("LocalSettings\\LastDownloadFolder"), m_sServer, sFolder);
			}
			else
				return RMERR_USERCANCELLED;
		}
		else if (aRemoteFiles.GetSize() == 1)
		{
			CFileDialog dialog(FALSE);

			dialog.m_ofn.lpstrFilter = ALLFILES;
			dialog.m_ofn.Flags &= ~OFN_ALLOWMULTISELECT;

			lstrcpy(dialog.m_ofn.lpstrFile, aRemoteFiles[0].sFileName);
			dialog.m_ofn.nMaxFile = MAX_PATH;

			if (!sFolder.IsEmpty())
				dialog.m_ofn.lpstrInitialDir = sFolder;

			if (dialog.DoModal() == IDOK)
				aLocalFiles.Add(dialog.GetPathName());
			else
				return RMERR_USERCANCELLED;
		}
	}
	else
		return RMERR_LOCALPATH;

	return RMERR_SUCCESS;
}

// for upload
RMERR CRemoteFile::GetLocalPaths(CStringArray& aLocalFiles, DWORD dwOptions, LPCTSTR szLocalDir)
{
	// sLocalPath could either be a full path, a folder or a wildcarded path
	CString sLocalPath(szLocalDir);
	Misc::Trim(sLocalPath);

	// check for wildcards
	if (FileMisc::PathHasWildcard(sLocalPath))
	{
		// check first for a valid parent folder
		if (!FileMisc::FolderFromFilePathExists(sLocalPath))
			return RMERR_LOCALPATH;
		
		// files first
		CFileFind ff;

		BOOL bContinue = ff.FindFile(sLocalPath);
		
		while (bContinue)
		{
			bContinue = ff.FindNextFile();
			
			if (!ff.IsDots() && !ff.IsDirectory())
			{
				CString sLocalPath = ff.GetFilePath();

				if (dwOptions & RMO_LOWERCASEPATHS)
					sLocalPath.MakeLower();

				aLocalFiles.Add(sLocalPath);
			}
		}
		ff.Close();

		// then sub folders
		if ((dwOptions & RMO_SUBDIRECTORIES) && sLocalPath.Find(_T("*.")) != -1)
		{
			TCHAR szDrive[_MAX_DRIVE], szFolder[_MAX_DIR], szExt[_MAX_EXT];
			_tsplitpath(sLocalPath, szDrive, szFolder, NULL, szExt);

			sLocalPath.Format(_T("%s%s*.*"), szDrive, szFolder);

			BOOL bContinue = ff.FindFile(sLocalPath);
			
			while (bContinue)
			{
				bContinue = ff.FindNextFile();
				
				if (ff.IsDirectory() && !ff.IsDots())
				{
					CString sLocalSubFolder(ff.GetFilePath());
					sLocalSubFolder += _T("\\*");
					sLocalSubFolder += szExt;

					GetLocalPaths(aLocalFiles, dwOptions, sLocalSubFolder);
				}
			}
			ff.Close();
		}

		return RMERR_SUCCESS;	
	}

	BOOL bFolder = FileMisc::FolderExists(sLocalPath);
	BOOL bFile = !bFolder && (GetFileAttributes(sLocalPath) != 0xffffffff);

	// now check for single file
	if (bFile)
	{
		if (dwOptions & RMO_LOWERCASEPATHS)
			sLocalPath.MakeLower();

		aLocalFiles.Add(sLocalPath);
	}
	// finally if must be folder or invalid (eg bad wildcard) in which
	// case we display a dialog to get the actual filenames
	else
	{
		BOOL bAllowDialog = (dwOptions & RMO_ALLOWDIALOG);

		if (!bAllowDialog)
			return RMERR_LOCALPATH;

		if (!bFolder)  // bad path
			sLocalPath.Empty();

		// get last folder browsed for this server
		if (sLocalPath.IsEmpty())
			sLocalPath = m_pPrefs->GetProfileString(_T("LocalSettings\\LastUploadFolder"), m_sServer);

		// browse for file(s)
		CFileDialog dialog(TRUE);

		const UINT BUFSIZE = 1024 * 5;
		static TCHAR FILEBUF[BUFSIZE] = { 0 };
				
		dialog.m_ofn.lpstrFilter = ALLFILES;
		dialog.m_ofn.lpstrTitle = _T("Select Files to Upload");
		dialog.m_ofn.lpstrFile = FILEBUF;
		dialog.m_ofn.nMaxFile = BUFSIZE;
		dialog.m_ofn.Flags |= OFN_ALLOWMULTISELECT;

		if (!sLocalPath.IsEmpty())
			dialog.m_ofn.lpstrInitialDir = sLocalPath;
		
		if (dialog.DoModal() == IDOK)
		{
			POSITION pos = dialog.GetStartPosition();

			while (pos)
			{
				CString sLocalPath = dialog.GetNextPathName(pos);

				if (dwOptions & RMO_LOWERCASEPATHS)
					sLocalPath.MakeLower();

				aLocalFiles.Add(sLocalPath);
			}

			m_pPrefs->WriteProfileString(_T("LocalSettings\\LastUploadFolder"), m_sServer, sLocalPath);
		}
		else
			return RMERR_USERCANCELLED;

	}

	return RMERR_SUCCESS;
}

BOOL CRemoteFile::ValidateLocalFolder(CString& sFolder, BOOL bAllowCreation)
{
	DWORD dwAttrib = GetFileAttributes(sFolder);
	
	if (dwAttrib == 0xffffffff) // no such path
	{
		Misc::Trim(sFolder);

		if (bAllowCreation && sFolder.GetLength())
		{
			// if the folder ends in a backslash then create as is
			sFolder.Replace('/', '\\');

			if (sFolder.GetAt(sFolder.GetLength() - 1) == '\\')
				FileMisc::CreateFolder(sFolder);

			else // split first
			{
				TCHAR szDrive[_MAX_DRIVE+1] = { 0 }, szFolder[MAX_PATH+1] = { 0 };
				_tsplitpath(sFolder, szDrive, szFolder, NULL, NULL);

				CString sTemp;
				sTemp.Format(_T("%s%s"), szDrive, szFolder);

				if (FileMisc::CreateFolder(sTemp))
					sFolder = sTemp;
			}

			return (GetFileAttributes(sFolder) != 0xffffffff);
		}
		else
			return FALSE;
	}
	else
		return (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

CString CRemoteFile::GetTempPath(const CString& sRemotePath, DWORD dwOptions)
{
	CString sTempPath;

  BOOL bKeepExt = (dwOptions & RMO_KEEPEXTENSION);
  BOOL bKeepFName = (dwOptions & RMO_KEEPFILENAME);
	
  if (bKeepFName)
    sTempPath = FileMisc::GetTempFilePath(FileMisc::GetFileNameFromPath(sRemotePath), _T(""));
  else
  {
    sTempPath = FileMisc::GetTempFilePath(_T("rmf"));

    if (bKeepExt)
    {
      CString sExt;
      FileMisc::SplitPath(sRemotePath, NULL, NULL, NULL, &sExt);

      FileMisc::ReplaceExtension(sTempPath, sExt);
    }
    else
      FileMisc::ReplaceExtension(sTempPath, _T("tmp"));
  }

	return sTempPath;
}

BOOL CRemoteFile::EstablishConnection(RMERR& nRes, DWORD dwOptions)
{
	CWaitCursor cursor;
	
	if (!RestartSession())
		return FALSE;

	BOOL bAllowDialog = (dwOptions & RMO_ALLOWDIALOG);
	BOOL bAnonLogin = (dwOptions & RMO_ANONYMOUSLOGIN);

	// try to establish a connection until we succeed or the user cancels
	// or bAllowDialog == FALSE and we fail
	while (TRUE)
	{
		if (!m_sServer.IsEmpty() && (bAnonLogin || (!m_sUsername.IsEmpty() && !m_sPassword.IsEmpty())))
		{
			try
			{
				if (bAnonLogin)
				{
					m_pConnection = m_pSession->GetFtpConnection(m_sServer, NULL, NULL, 
																INTERNET_INVALID_PORT_NUMBER, TRUE);
				}
				else
				{
					m_pConnection = m_pSession->GetFtpConnection(m_sServer, m_sUsername, m_sPassword, 
																INTERNET_INVALID_PORT_NUMBER, TRUE);
				}

				if (m_pConnection)
				{
					nRes = RMERR_SUCCESS;
					return TRUE;
				}
			}
			catch(CInternetException* e)
			{
				nRes = RMERR_CONNECTING;
				m_dwInternetErr = e->m_dwError;
				e->Delete();
			}
			catch (...)
			{
				CloseConnection();
				nRes = RMERR_CONNECTING;
			}

			// must mean failure if we got here
			if (bAllowDialog)
			{
				CEnString sMessage;
				sMessage.Format(_T("A connection to '%s' could not be established.\n\nWould you like to check your details and try again?"), m_sServer);
				
				if (AfxMessageBox(sMessage, MB_YESNO) == IDNO)
				{
					nRes = RMERR_USERCANCELLED;
					return FALSE;
				}

				if (!DoServerDlg(dwOptions, bAnonLogin))
				{
					nRes = RMERR_USERCANCELLED;
					return FALSE;
				}
			}
			else
				return FALSE; // can't try again
		}
		else if (bAllowDialog)
		{
			if (!DoServerDlg(dwOptions, bAnonLogin))
			{
				nRes = RMERR_USERCANCELLED;
				return FALSE;
			}
		}
		else
		{
			nRes = RMERR_SERVERDETAILS;
			return FALSE;
		}
	}

	return FALSE;
}

BOOL CRemoteFile::DoServerDlg(DWORD dwOptions, BOOL& bAnonLogin)
{
	AL_TYPE nAnonLogin = (dwOptions & RMO_NOANONYMOUSLOGIN) ? ANONLOGIN_HIDE : 
						(bAnonLogin ? ANONLOGIN_YES : ANONLOGIN_AUTO);

	CServerDlg dialog(m_sServer, m_sUsername, m_sPassword, nAnonLogin);
				
	if (dialog.DoModal(m_pPrefs, m_sPrefKey) == IDOK)
	{
		m_sServer = dialog.GetServer();
		m_sUsername = dialog.GetUsername();
		m_sPassword = dialog.GetPassword();
		bAnonLogin = dialog.GetAnonymousLogin();

		UINT nProxyPort = 0;
		CString sProxy = dialog.GetProxy(nProxyPort);

		VERIFY(SetProxy(sProxy, nProxyPort));

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CRemoteFile::SetProxy(const CString& sProxy, UINT nPort)
{
	CString sProxyAndPort;

	if (!sProxy.IsEmpty())
		sProxyAndPort.Format(_T("ftp=ftp://%s:%u"), sProxy, nPort);

	// we only need to restart the session if the proxy has changed
	// and a session is active
	if (m_pSession && m_sProxyAndPort != sProxyAndPort)
	{
		m_sProxyAndPort = sProxyAndPort;
		return RestartSession();
	}

	// else
	return TRUE;;
}

BOOL CRemoteFile::RestartSession()
{
	CloseConnection();
	CloseSession();
	
	// restart internet session
	if (m_sProxyAndPort.IsEmpty())
		m_pSession = new CInternetSession(m_sAgent, 1, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_PASSIVE);
	else
		m_pSession = new CInternetSession(m_sAgent, 1, INTERNET_OPEN_TYPE_PROXY, m_sProxyAndPort, NULL, INTERNET_FLAG_PASSIVE);

	return m_pSession && (HINTERNET)(*m_pSession);
}

void CRemoteFile::CloseConnection()
{
	if (m_pConnection)
		m_pConnection->Close();

	delete m_pConnection;
	m_pConnection = NULL;
}

void CRemoteFile::CloseSession()
{
	if (m_pSession)
		m_pSession->Close();

	delete m_pSession;
	m_pSession = NULL;
}

DWORD CRemoteFile::GetRemoteFileSize(LPCTSTR szRemotePath)
{
	try
	{
		CFtpFileFind ff(m_pConnection);
		
		if (ff.FindFile(szRemotePath))
		{
			ff.FindNextFile();
			return ff.GetLength();
		}
	}
	catch(CInternetException* e)
	{
		m_dwInternetErr = e->m_dwError;
		e->Delete();
	}
	catch (...)
	{
	}

	return (DWORD)-1;
}

BOOL CRemoteFile::RemoteFileExists(LPCTSTR szRemotePath)
{
	try
	{
		CFtpFileFind ff(m_pConnection);
		
		if (ff.FindFile(szRemotePath))
			return TRUE;
	}
	catch(CInternetException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	return FALSE;
}

RMERR CRemoteFile::DownloadFile(const FILERESULT* pRemoteFile, LPCTSTR szToLocalPath, DWORD dwOptions, CProgressDlg* pDlg)
{
	if (!m_pConnection)
		return RMERR_NOTCONNECTED;

	// check for existing local file
	BOOL bLocalExists = (::GetFileAttributes(szToLocalPath) != 0xffffffff);

	if (bLocalExists && (dwOptions & RMO_CONFIRMOVERWRITE))
	{
		CEnString sMessage;
		sMessage.Format(_T("The '%s' already exists.\n\nAre you sure you want to overwrite it?"), szToLocalPath);

		HWND hWnd = m_pParent ? m_pParent->GetSafeHwnd() : NULL;

		if (MessageBox(hWnd, sMessage, _T("Confirm Overwrite"), MB_YESNO) == IDNO)
			return RMERR_USERCANCELLED;
	}

	// get remote file size if not specified
	DWORD dwFileSize = pRemoteFile->dwSize;

	if (!dwFileSize)
	{
		dwFileSize = GetRemoteFileSize(pRemoteFile->sFilePath);

		if (-1 == dwFileSize)
			return RMERR_REMOTEPATH; // probably doen't exist
	}

	// initialize progress dialog
	CProgressDlg progDlg;
	BOOL bProgress = !(dwOptions & RMO_NOPROGRESS);

	if (bProgress)
	{
		if (!pDlg && progDlg.Create(_T("Download Progress"), NULL, m_pParent))
		{
			if (m_pParent)
				m_pParent->EnableWindow(FALSE);

			pDlg = &progDlg;
		}

		CEnString sDescription;
		sDescription.Format(_T("Downloading '%s' from '%s'"), pRemoteFile->sFilePath, m_sServer);

		pDlg->SetDescription(sDescription);
		pDlg->Continue();
	}

	// open remote file for input
	CWaitCursor cursor;
	CInternetFile* pFile = NULL;

	try
	{
		pFile = m_pConnection->OpenFile(pRemoteFile->sFilePath);
	}
	catch(CInternetException* e)
	{
		m_dwInternetErr = e->m_dwError;
		e->Delete();
	}
	catch(...)
	{
	}

	if (!pFile)
		return RMERR_OPENINGREMOTEFILETOREAD;

	// open local file for output
	RMERR nRes = RMERR_SUCCESS;
	CFile file;

	// make sure the parent folder is created first
	VERIFY (FileMisc::CreateFolderFromFilePath(szToLocalPath));

	if (file.Open(szToLocalPath, CFile::modeCreate | CFile::modeWrite))
	{
		// write out as we read in
		BYTE ucBuffer[BUFSIZE];
		DWORD dwBytesRead = 0, dwBytesWritten = 0;

		while (pDlg->Continue())
		{
			UINT nCount = 0;

			try
			{
				nCount = pFile->Read((LPVOID)ucBuffer, BUFSIZE);
			}
			catch(CInternetException* e)
			{
				nRes = RMERR_READINGREMOTEFILE;
				m_dwInternetErr = e->m_dwError;
				e->Delete();
			}
			catch (...)
			{
				nRes = RMERR_READINGREMOTEFILE;
				break;
			}

			if (nCount)
			{
				dwBytesRead += nCount;
				
				if (bProgress)
					DoProgress(pDlg, dwBytesRead, dwFileSize, FALSE);

				try
				{
					file.Write(ucBuffer, nCount);

					dwBytesWritten += nCount;
				}
				catch (...)
				{
					nRes = RMERR_WRITINGLOCALFILE;
					break;
				}
			}
			
			if (nCount < BUFSIZE)
				break;
		}

		// if the download failed and the local file did not exist or 
		// we wrote to the file (so it will be corrupt) and we should delete failures
		// then cleanup
		if (nRes != RMERR_SUCCESS && (!bLocalExists || (dwBytesWritten && (dwOptions & RMO_DELETEFAILURES))))
			::DeleteFile(szToLocalPath);
	}
	else
		nRes = RMERR_OPENINGLOCALFILETOWRITE;

	// cleanup
	if (bProgress)
	{
		if (m_pParent && pDlg == &progDlg)
			m_pParent->EnableWindow(TRUE);

		if (!pDlg->Continue())
			nRes = RMERR_USERCANCELLED;

		if (pDlg == &progDlg)
			progDlg.DestroyWindow();
	}

	file.Close();
	pFile->Close();
	delete pFile;


	return nRes;
}

void CRemoteFile::DoProgress(CProgressDlg* pDlg, DWORD dwBytesRead, DWORD dwFileSize, BOOL bUpload)
{
	if (!pDlg || !pDlg->GetSafeHwnd())
		return;

	CEnString sProgress, sTitle;
	int nPercent = 0;

	if (bUpload)
	{
		nPercent = MulDiv(dwBytesRead, 100, dwFileSize);
		sProgress.Format(_T("%0.1f Kb uploaded so far (%0.1f Kb remaining)"), dwBytesRead / 1024.0, (dwFileSize - dwBytesRead) / 1024.0);
		sTitle.Format(_T("Upload Progress (%d%%)"), nPercent);
	}
	else // download
	{
		if (!dwFileSize)
			sProgress.Format(_T("%0.1f Kb downloaded so far"), dwBytesRead);
		else
		{
			nPercent = MulDiv(dwBytesRead, 100, dwFileSize);
			sProgress.Format(_T("%0.1f Kb downloaded so far (%0.1f Kb remaining)"), dwBytesRead / 1024.0, (dwFileSize - dwBytesRead) / 1024.0);
			sTitle.Format(_T("Download Progress (%d%%)"), nPercent);
		}
	}
	
	pDlg->SetProgress(nPercent);
	pDlg->SetProgress(sProgress);
	pDlg->SetWindowText(sTitle);
}

RMERR CRemoteFile::UploadFile(LPCTSTR szFromLocalPath, const FILERESULT* pRemoteFile, DWORD dwOptions, CProgressDlg* pDlg)
{
	if (!m_pConnection)
		return RMERR_NOTCONNECTED;

	// open local file for input
	CFile file;

	if (!file.Open(szFromLocalPath, CFile::modeRead))
		return RMERR_OPENINGLOCALFILETOREAD;

	CString sRemotePath(pRemoteFile->sFilePath);

	// check for existing remote file
	if (dwOptions & RMO_LOWERCASEPATHS)
		sRemotePath.MakeLower();

	BOOL bRemoteExists = RemoteFileExists(sRemotePath);

	if (bRemoteExists && (dwOptions & RMO_CONFIRMOVERWRITE))
	{
		CEnString sMessage;
		sMessage.Format(_T("The remote file '%s' already exists on the server.\n\nAre you sure you want to overwrite it?"), sRemotePath);

		HWND hWnd = m_pParent ? m_pParent->GetSafeHwnd() : NULL;
		int nRet = MessageBox(hWnd, sMessage, _T("Confirm Overwrite"), MB_YESNO);

		if (nRet != IDYES && nRet != IDOK)
			return RMERR_USERCANCELLED;
	}

	// create dialog if required
	CProgressDlg progDlg;
	BOOL bProgress = !(dwOptions & RMO_NOPROGRESS);

	if (bProgress)
	{
		if (!pDlg && progDlg.Create(_T("Upload Progress"), NULL, m_pParent))
		{
			if (m_pParent)
				m_pParent->EnableWindow(FALSE);

			pDlg = &progDlg;
		}

		CEnString sDescription(_T("Uploading '%s' to '%s'"), pRemoteFile->sFileName, m_sServer);
		pDlg->SetDescription(sDescription);
	}

	// open remote file for ouput
	CWaitCursor cursor;
	CInternetFile* pFile = NULL;
	pDlg->Continue();

	try
	{
		pFile = m_pConnection->OpenFile(pRemoteFile->sFilePath, GENERIC_WRITE);
	}
	catch(CInternetException* e)
	{
		m_dwInternetErr = e->m_dwError;
		e->Delete();
	}
	catch(...)
	{
	}

	if (!pFile)
		return RMERR_OPENINGREMOTEFILETOWRITE;

	RMERR nRes = RMERR_SUCCESS;

	// write out as we read in
	BYTE ucBuffer[BUFSIZE];
	DWORD dwBytesRead = 0, dwBytesWritten = 0;
	DWORD dwFileSize = file.GetLength();

	while (pDlg->Continue())
	{
		UINT nCount = 0;
		
		try
		{
			nCount = file.Read((LPVOID)ucBuffer, BUFSIZE);
		}
		catch (...)
		{
			nRes = RMERR_READINGLOCALFILE;
			break;
		}

		if (nCount)
		{
			dwBytesRead += nCount;
			
			if (bProgress)
				DoProgress(pDlg, dwBytesRead, dwFileSize, TRUE);

			try
			{
				pFile->Write(ucBuffer, nCount);
			
				dwBytesWritten += nCount;
			}
			catch (CInternetException* e)
			{
				nRes = RMERR_WRITINGREMOTEFILE;
				m_dwInternetErr = e->m_dwError;
				e->Delete();
				break;
			}
			catch (...)
			{
				nRes = RMERR_WRITINGREMOTEFILE;
				break;
			}
		}
		
		if (nCount < BUFSIZE)
			break;
	}

	// if the upload failed and the remote file did not exist or 
	// (we wrote to the file (so it will be corrupt) and we should delete failures)
	// then cleanup
	if (nRes != RMERR_SUCCESS && (!bRemoteExists || (dwBytesWritten && (dwOptions & RMO_DELETEFAILURES))))
		m_pConnection->Remove(pRemoteFile->sFilePath);

	// cleanup
	if (bProgress)
	{
		if (m_pParent && pDlg == &progDlg)
			m_pParent->EnableWindow(TRUE);

		if (!pDlg->Continue())
			nRes = RMERR_USERCANCELLED;

		if (pDlg == &progDlg)
			progDlg.DestroyWindow();
	}

	file.Close();
	pFile->Close();
	delete pFile;

	return nRes;
}

void CRemoteFile::SplitPath(LPCTSTR szFullRemotePath, CString& sServer, CString& sFile)
{
	DWORD dwDummy = 0;
	INTERNET_PORT nDummy = 0;

	AfxParseURL(szFullRemotePath, dwDummy, sServer, sFile, nDummy);
}

CString CRemoteFile::FormatRemotePath(LPCTSTR szFile) const
{
	ASSERT(!Misc::IsEmpty(szFile));

	CString sRemotePath(m_sServer);

	if (szFile[0] != '/')
		sRemotePath += '/';

	sRemotePath += szFile;
	return sRemotePath;
}

void CRemoteFile::ValidateOptions(DWORD& dwOptions, BOOL bDownload)
{
	// validate options
	if (bDownload)
	{
		if (!(dwOptions & RMO_USETEMPFILE))
    {
			dwOptions &= ~RMO_KEEPEXTENSION;
      dwOptions &= ~RMO_KEEPFILENAME;
    }

    if (dwOptions & RMO_KEEPFILENAME)
      dwOptions &= ~RMO_KEEPEXTENSION;

		if (dwOptions & RMO_NOANONYMOUSLOGIN)
			dwOptions &= ~RMO_ANONYMOUSLOGIN;
	}
	else // upload
		dwOptions &= ~(RMO_USETEMPFILE | RMO_KEEPEXTENSION | RMO_KEEPFILENAME | RMO_CREATEDOWNLOADDIR);
}
