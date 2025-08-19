// TDLWebUpdater.cpp: implementation of the CTDLWebUpdater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLWebUpdater.h"

#include "..\todolist\tdcswitch.h"

#include "..\shared\filemisc.h"
#include "..\shared\unzipper.h"
#include "..\shared\enstring.h"
#include "..\shared\misc.h"
#include "..\shared\webmisc.h"
#include "..\shared\EnCommandLineInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CTDLWebUpdater::CTDLWebUpdater(const CPoint& ptPos, BOOL bPreRelease)
	: 
	m_sDownloadUri(bPreRelease ? DOWNLOAD_URI_PRERELEASE : DOWNLOAD_URI),
	m_dlgProgress(ptPos)
{
}

CTDLWebUpdater::~CTDLWebUpdater()
{

}

TDL_WEBUPDATE_RESULT CTDLWebUpdater::DoUpdate(const CString& sAppFolder, const CString& sPrevCmdLine, 
											  BOOL bRestartElevated, BOOL bDownloadOnly)
{
	if (WebMisc::IsOnline())
	{
		// reset result
		m_nResUpdate = TDLWUR_SUCCESS;
		
#ifdef _DEBUG
		if (bDownloadOnly)
		{
			m_sDownloadFile = FileMisc::GetTempFilePath();
			m_sAppFolder = FileMisc::GetTempFolder();

			InitialiseTemporaries();
			DoProgressDialog(_T(""), FALSE);
			
			return TDLWUR_SUCCESS;
		}
#else
		UNREFERENCED_PARAMETER(bDownloadOnly);
#endif
		if (SetAppFolder(sAppFolder) && InitialiseTemporaries())
		{
			if (!DoProgressDialog(sPrevCmdLine, bRestartElevated))
				RestoreBackup(m_dlgProgress.GetProgressStatus());
			
			m_dlgProgress.DestroyWindow();
			
			CleanupTemporaries();
		}
	}
	else
	{
		m_nResUpdate = TDLWUR_ERR_CONNECTED;
	}
	
	return LogError(sAppFolder);
}

BOOL CTDLWebUpdater::SetAppFolder(const CString& sAppFolder)
{
	// sanity check
	ASSERT(m_nResUpdate == TDLWUR_SUCCESS);

	if (m_nResUpdate != TDLWUR_SUCCESS)
		return FALSE;
	
	// reset paths
	m_sAppFolder.Empty();
	m_sAppPath.Empty();

	// app folder cannot be empty
	if (sAppFolder.IsEmpty())
	{
		m_nResUpdate = TDLWUR_ERR_APPFOLDER;
		return FALSE;
	}

	// app folder must exist
	if (!FileMisc::FolderExists(sAppFolder))
	{
		m_nResUpdate = TDLWUR_ERR_EXISTAPPFOLDER;
		return FALSE;
	}

	// updater cannot be in the app folder
	CString sUpdaterFolder = FileMisc::GetAppFolder();

	if (FileMisc::IsSamePath(sAppFolder, sUpdaterFolder))
	{
		m_nResUpdate = TDLWUR_ERR_UPDATERFOLDER;
		return FALSE;
	}

	CString sUpdaterPath = FileMisc::GetAppFilePath();

	// updater must be temp folder
#ifndef _DEBUG
	if (!FileMisc::IsTempFilePath(sUpdaterPath))
	{
		m_nResUpdate = TDLWUR_ERR_TEMPUPDATERFOLDER;
		return FALSE;
	}
#endif

	// all good
	FileMisc::LogText(_T("The application folder '%s' was successfully initialised."), sAppFolder);

	m_sAppFolder = sAppFolder;
	m_sAppPath = (FileMisc::TerminatePath(sAppFolder) + _T("ToDoList.exe"));

	return TRUE;
}

BOOL CTDLWebUpdater::InitialiseTemporaries()
{
	// sanity check
	ASSERT(!m_sAppFolder.IsEmpty());
	
	if (m_nResUpdate != TDLWUR_SUCCESS)
		return FALSE;

	// reset paths
	m_sBackupFolder.Empty();
	m_sDownloadFile.Empty();
	m_sUnzipFolder.Empty();

	// temp backup folder
	CString sBackupFolder = m_sAppFolder + ".bak";
	
	if (!FileMisc::DeleteFolder(sBackupFolder, FMDF_SUBFOLDERS | FMDF_HIDDENREADONLY))
	{
		m_nResUpdate = TDLWUR_ERR_DELETEBACKUPFOLDER;
		return FALSE;
	}

	// we download the application zip file to temp file
	CString sDownloadFile = FileMisc::GetTempFilePath();
	
	if (!FileMisc::DeleteFile(sDownloadFile, TRUE))
	{
		m_nResUpdate = TDLWUR_ERR_DELETETEMPZIP;
		return FALSE;
	}

	// temp unzip folder
	CString sUnzipFolder = FileMisc::GetTempFilePath();
	FileMisc::RemoveExtension(sUnzipFolder);
	
	if (!FileMisc::DeleteFolder(sUnzipFolder, FMDF_SUBFOLDERS | FMDF_HIDDENREADONLY))
	{
		m_nResUpdate = TDLWUR_ERR_DELETEUNZIPFOLDER;
		return FALSE;
	}

	// all good
	m_sBackupFolder = sBackupFolder;
	m_sDownloadFile = sDownloadFile;
	m_sUnzipFolder = sUnzipFolder;

	m_nResUpdate = TDLWUR_SUCCESS;

	return TRUE;
}

void CTDLWebUpdater::CleanupTemporaries()
{
	// download package
	FileMisc::DeleteFile(m_sDownloadFile, TRUE);
	FileMisc::DeleteFolder(m_sUnzipFolder, (FMDF_HIDDENREADONLY | FMDF_SUBFOLDERS));
	
	// 'this' application
	::MoveFileEx(FileMisc::GetAppFilePath(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);

	// backup folder unless backup failed
	if (m_nResUpdate != TDLWUR_ERR_RESTOREBACKUP)
		FileMisc::DeleteFolder(m_sBackupFolder, FMDF_SUBFOLDERS | FMDF_HIDDENREADONLY);
}

BOOL CTDLWebUpdater::CheckUpdateCancelled()
{
	ASSERT(m_dlgProgress.GetSafeHwnd());

	if (m_dlgProgress.IsCancelled())
	{
		m_nResUpdate = TDLWUR_CANCELLED;
		return TRUE;
	}

	// keep going
	return FALSE;
}

TDL_WEBUPDATE_RESULT CTDLWebUpdater::LogError(const CString& sAppFolder) const
{
	switch (m_nResUpdate)
	{
	case TDLWUR_SUCCESS:
		FileMisc::LogText(_T("The update completed successfully."));
		break;
		
	case TDLWUR_CANCELLED:
		FileMisc::LogText(_T("User cancelled the web update."));
		break;
		
	case TDLWUR_ERR_APPFOLDER:
		FileMisc::LogText(_T("The application folder name cannot be empty."));
		break;
		
	case TDLWUR_ERR_CONNECTED:
		FileMisc::LogText(_T("User is not connected to the Internet."));
		break;
		
	case TDLWUR_ERR_EXISTAPPFOLDER:
		FileMisc::LogText(_T("The application folder '%s' does not exist."), sAppFolder);
		break;
		
	case TDLWUR_ERR_UPDATERFOLDER:
		FileMisc::LogText(_T("The application folder '%s' cannot be the same as the updater folder '%s'"), sAppFolder, FileMisc::GetAppFilePath());
		break;
		
	case TDLWUR_ERR_TEMPUPDATERFOLDER:
		FileMisc::LogText(_T("The updater '%s' can only be run in the 'TEMP' folder '%s'."), FileMisc::GetAppFilePath(), FileMisc::GetTempFolder());
		break;
		
	case TDLWUR_ERR_DELETETEMPZIP:
		FileMisc::LogText(_T("The temporary download file '%s' could not be initialised."), m_sDownloadFile);
		break;
		
	case TDLWUR_ERR_DELETEFROMCACHE:
		FileMisc::LogText(_T("The cache entry for '%s' could not be deleted."), m_sDownloadUri);
		break;
		
	case TDLWUR_ERR_DELETEBACKUPFOLDER:
		FileMisc::LogText(_T("The application backup folder '%s' could not be initialised."), m_sBackupFolder);
		break;
		
	case TDLWUR_ERR_DELETEUNZIPFOLDER:
		FileMisc::LogText(_T("The temporary download folder '%s' could not be initialised."), m_sUnzipFolder);
		break;
		
	case TDLWUR_ERR_CREATEPROGRESSDLG:
		FileMisc::LogText(_T("The update progress dialog could not be created."));
		break;
		
	case TDLWUR_ERR_DOWNLOADZIP:
		FileMisc::LogText(_T("The update '%s' could not be downloaded to '%s'."), m_sDownloadUri, m_sDownloadFile);
		break;
		
	case TDLWUR_ERR_OPENZIP:
		FileMisc::LogText(_T("The update file '%s' could not be opened."), m_sDownloadFile);
		break;
		
	case TDLWUR_ERR_UNZIPFOLDER:
		FileMisc::LogText(_T("The temporary unzip folder '%s' could not be created."), m_sUnzipFolder);
		break;
		
	case TDLWUR_ERR_UNZIP:
		FileMisc::LogText(_T("The update file '%s' could not be unzipped to '%s'."), m_sDownloadFile, m_sUnzipFolder);
		break;
		
	case TDLWUR_ERR_DOBACKUP:
		FileMisc::LogText(_T("One or more files in the application folder '%s' could not be backed-up to '%s'."), m_sAppFolder, m_sBackupFolder);
		break;
		
	case TDLWUR_ERR_DOUPDATE:
		FileMisc::LogText(_T("One or more files in the temporary download folder '%s' could not be copied to the application folder '%s'."), m_sUnzipFolder, m_sAppFolder);
		break;
		
	case TDLWUR_ERR_RUNUPDATE:
		FileMisc::LogText(_T("The updated application '%s' could not be started."), m_sAppPath);
		break;
		
	case TDLWUR_ERR_RESTOREBACKUP:
		FileMisc::LogText(_T("The application backup folder '%s' could not be restored to '%s'."), m_sBackupFolder, m_sAppFolder);
		break;
		
	case TDLWUR_ERR_RUNRESTORE:
		FileMisc::LogText(_T("The restored application '%s' could not be started."), m_sAppPath);
		break;

	case TDLWUR_ERR_UNEXPECTED:
	default:
		FileMisc::LogText(_T("The updater '%s' failed unexpectedly."), FileMisc::GetAppFilePath());
		ASSERT(0);
	}

	return m_nResUpdate;
}

BOOL CTDLWebUpdater::DoProgressDialog(const CString& sPrevCmdLine, BOOL bRestartElevated)
{
	// sanity checks
	ASSERT(m_nResUpdate == TDLWUR_SUCCESS);

	if (m_nResUpdate != TDLWUR_SUCCESS)
		return FALSE;

	// sanity checks
	ASSERT(m_dlgProgress.GetSafeHwnd() == NULL);

	if (m_dlgProgress.GetSafeHwnd())
		return FALSE;

	if (!m_dlgProgress.Create(AfxGetMainWnd(), (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION)))
	{
		m_nResUpdate = TDLWUR_ERR_CREATEPROGRESSDLG;
		return FALSE;
	}

	// download update
	m_dlgProgress.SetProgressStatus(TDLWP_DOWNLOAD);
	
	// clear cache first
	if (!WebMisc::DeleteCacheEntry(m_sDownloadUri))
	{
		if (GetLastError() == ERROR_ACCESS_DENIED)
		{
			m_nResUpdate = TDLWUR_ERR_DELETEFROMCACHE;
			return FALSE;
		}
	}

	if (!WebMisc::DownloadFile(m_sDownloadUri, m_sDownloadFile, this))
	{
		if (!CheckUpdateCancelled())
			m_nResUpdate = TDLWUR_ERR_DOWNLOADZIP;

		return FALSE;
	}

	// else
	FileMisc::LogText(_T("The update '%s' was successfully downloaded to '%s'."), m_sDownloadUri, m_sDownloadFile);

	if (CheckUpdateCancelled())
		return FALSE;

	// unzip the package
	m_dlgProgress.SetProgressStatus(TDLWP_UNZIP);

	// scope so unzipper releases any file handles
	try
	{
		CUnzipper unzip(NULL, UnzipCallback, (DWORD)this);
		
		FileMisc::LogText(_T("The unzip component was successfully initialised."));

		if (!unzip.OpenZip(m_sDownloadFile))
		{
			m_nResUpdate = TDLWUR_ERR_OPENZIP;
			return FALSE;
		}

		FileMisc::LogText(_T("The zip '%s' was successfully opened."), m_sDownloadFile);
		
		if (CheckUpdateCancelled())
			return FALSE;
		
		// check we can create the folder
		if (!FileMisc::CreateFolder(m_sUnzipFolder))
		{
			m_nResUpdate = TDLWUR_ERR_UNZIPFOLDER;
			return FALSE;
		}
		
		FileMisc::LogText(_T("The unzip folder '%s' was successfully created."), m_sUnzipFolder);
		
		// check we can do the unzip
		if (!unzip.UnzipTo(m_sUnzipFolder))
		{
			if (!CheckUpdateCancelled())
				m_nResUpdate = TDLWUR_ERR_UNZIP;

			return FALSE;
		}
	}
	catch (...)
	{
		FileMisc::LogText(_T("The unzipping of '%s' threw an exception."), m_sDownloadFile);

		m_nResUpdate = TDLWUR_ERR_UNEXPECTED;
		return FALSE;
	}

	// check we can make contents writable
	if (!FileMisc::ModifyAttributes(m_sUnzipFolder, (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN), 0))
	{
		m_nResUpdate = TDLWUR_ERR_UNZIP;
		return FALSE;
	}
	
	FileMisc::LogText(_T("The update file '%s' was successfully unzipped to '%s'."), m_sDownloadFile, m_sUnzipFolder);
	
	if (CheckUpdateCancelled())
		return FALSE;
	
	// backup existing installation binaries and prefs
	m_dlgProgress.SetProgressStatus(TDLWP_BACKUP);

	DWORD dwBaseFlags = (FMDF_HIDDENREADONLY | FMDF_PROCESSMSGLOOP);
	
	if (!FileMisc::CopyFolder(m_sAppFolder, m_sBackupFolder, _T("*.exe;*.dll;*.ini"), dwBaseFlags))
	{
		m_nResUpdate = TDLWUR_ERR_DOBACKUP;
		return FALSE;
	}
	
	FileMisc::LogText(_T("The application folder '%s' was successfully backed-up to '%s'."), m_sAppFolder, m_sBackupFolder);
	
	if (CheckUpdateCancelled())
		return FALSE;
	
	// NOTE: NO CANCELLING FROM HERE ON
	
	// copy over installation folder
	m_dlgProgress.SetProgressStatus(TDLWP_COPY);

	// binaries (overwrite)
	if (!FileMisc::CopyFolder(m_sUnzipFolder, m_sAppFolder, NULL, (dwBaseFlags | FMDF_OVERWRITE)))
	{
		m_nResUpdate = TDLWUR_ERR_DOUPDATE;
		return FALSE;
	}
	
	// resources (do not overwrite existing)
	CString sSrcResources = (FileMisc::TerminatePath(m_sUnzipFolder) + _T("Resources"));
	CString sDestResources = (FileMisc::TerminatePath(m_sAppFolder) + _T("Resources"));

	if (!FileMisc::CopyFolder(sSrcResources, sDestResources, NULL, (dwBaseFlags | FMDF_SUBFOLDERS | FMDF_NEWER)))
	{
		m_nResUpdate = TDLWUR_ERR_DOUPDATE;
		return FALSE;
	}
	
	FileMisc::LogText(_T("The contents of the temporary download folder '%s' were successfully copied to the application folder '%s'."), m_sUnzipFolder, m_sAppFolder);
	
	// cleanup
	m_dlgProgress.SetProgressStatus(TDLWP_CLEANUP);
	
	// verify we can start the updated application
	CString sParams(sPrevCmdLine);
	sParams += CEnCommandLineInfo::FormatSwitch(SWITCH_UPGRADED);

	if (bRestartElevated)
		sParams += CEnCommandLineInfo::FormatSwitch(SWITCH_RESTARTELEVATED);
	
	if (bRestartElevated || !FileMisc::RunFromExplorer(m_sAppPath, sParams))
	{
		if (FileMisc::Run(NULL, m_sAppPath, sParams) < SE_ERR_SUCCESS)
		{
			m_nResUpdate = TDLWUR_ERR_RUNUPDATE;
			return FALSE;
		}
	}
	
	FileMisc::LogText(_T("The updated application '%s' was successfully restarted."), m_sAppPath);
	
	// success
	m_dlgProgress.SetProgressStatus(TDLWP_COMPLETE);

	ASSERT(m_nResUpdate == TDLWUR_SUCCESS);
	return TRUE;
}

void CTDLWebUpdater::RestoreBackup(TDL_WEBUPDATE_PROGRESS nCancelled)
{
	// Sanity check
	if ((m_nResUpdate == TDLWUR_SUCCESS) || (nCancelled == TDLWP_COMPLETE))
	{
		ASSERT(0);
		return;
	}

	// log the existing error because we may overwrite it
	LogError(m_sAppFolder);

	FileMisc::LogText(_T("Initialising application restore from '%s'."), m_sBackupFolder);

	// restore the backup if we got that far
	BOOL bRestoreBackup = FALSE;

	switch (nCancelled)
	{
		case TDLWP_DOWNLOAD:
			FileMisc::LogText(_T("Cancelled during download - backup not yet created."));
			break;

		case TDLWP_UNZIP:
			FileMisc::LogText(_T("Cancelled during unzipping - backup not yet created."));
			break;

		case TDLWP_BACKUP:
			FileMisc::LogText(_T("Cancelled during backup - backup not yet created."));
			break;

		case TDLWP_COPY:
			FileMisc::LogText(_T("Cancelled during copying - backup needs restoring."));
			bRestoreBackup = TRUE;
			break;

		case TDLWP_CLEANUP:
			FileMisc::LogText(_T("Cancelled during cleanup - backup needs restoring."));
			bRestoreBackup = TRUE;
			break;

		default:
			ASSERT(0);
			return;
	}

	if (bRestoreBackup)
	{
		if (!FileMisc::FolderExists(m_sBackupFolder))
		{
			FileMisc::LogText(_T("Backup folder '%s' does not exist."), m_sBackupFolder);
			m_nResUpdate = TDLWUR_ERR_RESTOREBACKUP;
		}
		else if (!FileMisc::DeleteFolderContents(m_sAppFolder, FMDF_HIDDENREADONLY, _T("*.exe;*.dll")))
		{
			FileMisc::LogText(_T("Unable to delete contents of folder '%s'."), m_sAppFolder);
			m_nResUpdate = TDLWUR_ERR_RESTOREBACKUP;
		}
		else if (!FileMisc::CopyFolder(m_sBackupFolder, m_sAppFolder, (FMDF_SUBFOLDERS | FMDF_HIDDENREADONLY)))
		{
			FileMisc::LogText(_T("Unable to copy contents of backup folder '%s' to '%s'."), m_sBackupFolder, m_sAppFolder);
			m_nResUpdate = TDLWUR_ERR_RESTOREBACKUP;
		}
		else
		{
			FileMisc::LogText(_T("The application '%s' was successfully restored from '%s'."), m_sAppPath, m_sBackupFolder);
		}
	}

	if (FileMisc::Run(NULL, m_sAppPath) < SE_ERR_SUCCESS)
	{
		m_nResUpdate = TDLWUR_ERR_RUNRESTORE;
	}
}

bool CTDLWebUpdater::UnzipCallback(int nPercent, DWORD dwUserData)
{
	if (dwUserData)
	{
		CTDLWebUpdater* pThis = (CTDLWebUpdater*)dwUserData;
		return (pThis->OnUnzipProgress(nPercent) != FALSE);
	}

	ASSERT(0);
	return false;
}

BOOL CTDLWebUpdater::OnUnzipProgress(int nPercent)
{
	if (m_dlgProgress.GetSafeHwnd() && (m_dlgProgress.GetProgressStatus() == TDLWP_UNZIP))
	{
		m_dlgProgress.SetProgressStatus(TDLWP_UNZIP, nPercent);
		return !m_dlgProgress.IsCancelled();
	}

	ASSERT(0);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// IBindStatusCallback

// IUnknown
/*
STDMETHODIMP CTDLWebUpdater::QueryInterface(REFIID riid, void **ppvObject)
{
	TRACE(_T("IUnknown::QueryInterface\n"));

	*ppvObject = NULL;

	// IUnknown
	if (::IsEqualIID(riid, __uuidof(IUnknown)))
	{
		TRACE(_T("IUnknown::QueryInterface(IUnknown)\n"));

		*ppvObject = this;
	}
	// IBindStatusCallback
	else if (::IsEqualIID(riid, __uuidof(IBindStatusCallback)))
	{
		TRACE(_T("IUnknown::QueryInterface(IBindStatusCallback)\n"));

		*ppvObject = static_cast<IBindStatusCallback *>(this);
	}

	if (*ppvObject)
	{
		(*reinterpret_cast<LPUNKNOWN *>(ppvObject))->AddRef();

		return S_OK;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CTDLWebUpdater::AddRef()
{
	TRACE(_T("IUnknown::AddRef\n"));

	return ++m_ulObjRefCount;
}

STDMETHODIMP_(ULONG) CTDLWebUpdater::Release()
{
	TRACE(_T("IUnknown::Release\n"));

	return --m_ulObjRefCount;
}
*/

STDMETHODIMP CTDLWebUpdater::OnProgress(ULONG ulProgress,
										ULONG ulProgressMax,
										ULONG ulStatusCode,
										LPCWSTR /*szStatusText*/)
{
#ifdef _DEBUG
/*
#ifndef BINDSTATUS_ACCEPTRANGES
#	define BINDSTATUS_ACCEPTRANGES (BINDSTATUS_LOADINGMIMEHANDLER + 8)
#endif

	enum
	{
		UF_BINDSTATUS_FIRST = BINDSTATUS_FINDINGRESOURCE,
		UF_BINDSTATUS_LAST = BINDSTATUS_ACCEPTRANGES
	};

	static const LPCTSTR plpszStatus[] =
	{
		_T("BINDSTATUS_FINDINGRESOURCE"),  // 1
		_T("BINDSTATUS_CONNECTING"),
		_T("BINDSTATUS_REDIRECTING"),
		_T("BINDSTATUS_BEGINDOWNLOADDATA"),
		_T("BINDSTATUS_DOWNLOADINGDATA"),
		_T("BINDSTATUS_ENDDOWNLOADDATA"),
		_T("BINDSTATUS_BEGINDOWNLOADCOMPONENTS"),
		_T("BINDSTATUS_INSTALLINGCOMPONENTS"),
		_T("BINDSTATUS_ENDDOWNLOADCOMPONENTS"),
		_T("BINDSTATUS_USINGCACHEDCOPY"),
		_T("BINDSTATUS_SENDINGREQUEST"),
		_T("BINDSTATUS_CLASSIDAVAILABLE"),
		_T("BINDSTATUS_MIMETYPEAVAILABLE"),
		_T("BINDSTATUS_CACHEFILENAMEAVAILABLE"),
		_T("BINDSTATUS_BEGINSYNCOPERATION"),
		_T("BINDSTATUS_ENDSYNCOPERATION"),
		_T("BINDSTATUS_BEGINUPLOADDATA"),
		_T("BINDSTATUS_UPLOADINGDATA"),
		_T("BINDSTATUS_ENDUPLOADINGDATA"),
		_T("BINDSTATUS_PROTOCOLCLASSID"),
		_T("BINDSTATUS_ENCODING"),
		_T("BINDSTATUS_VERFIEDMIMETYPEAVAILABLE"),
		_T("BINDSTATUS_CLASSINSTALLLOCATION"),
		_T("BINDSTATUS_DECODING"),
		_T("BINDSTATUS_LOADINGMIMEHANDLER"),
		_T("BINDSTATUS_CONTENTDISPOSITIONATTACH"),
		_T("BINDSTATUS_FILTERREPORTMIMETYPE"),
		_T("BINDSTATUS_CLSIDCANINSTANTIATE"),
		_T("BINDSTATUS_IUNKNOWNAVAILABLE"),
		_T("BINDSTATUS_DIRECTBIND"),
		_T("BINDSTATUS_RAWMIMETYPE"),
		_T("BINDSTATUS_PROXYDETECTING"),
		_T("BINDSTATUS_ACCEPTRANGES"),
		_T("???")  // unknown
	};

	TRACE(_T("IBindStatusCallback::OnProgress\n"));
	TRACE(_T("ulProgress: %lu, ulProgressMax: %lu\n"), ulProgress, ulProgressMax);
	TRACE(_T("ulStatusCode: %lu "), ulStatusCode);

	if (ulStatusCode < UF_BINDSTATUS_FIRST ||
		ulStatusCode > UF_BINDSTATUS_LAST)
	{
		ulStatusCode = UF_BINDSTATUS_LAST + 1;
	}

	TRACE(_T("(%s), szStatusText: %ls\n"), plpszStatus[ulStatusCode - UF_BINDSTATUS_FIRST], szStatusText);
*/
#endif

	if (m_dlgProgress.GetSafeHwnd()	&& (m_dlgProgress.GetProgressStatus() == TDLWP_DOWNLOAD))
	{
		switch (ulStatusCode)
		{
		case BINDSTATUS_BEGINDOWNLOADDATA:
		case BINDSTATUS_DOWNLOADINGDATA:
		case BINDSTATUS_ENDDOWNLOADDATA:
			{
				m_dlgProgress.SetProgressStatus(TDLWP_DOWNLOAD, (int)MulDiv(ulProgress, 100, ulProgressMax));

				if (m_dlgProgress.IsCancelled())
					return E_ABORT;
			}
			break;
		}
	}

	return S_OK;
}
