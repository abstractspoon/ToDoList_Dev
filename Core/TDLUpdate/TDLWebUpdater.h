// TDLWebUpdater.h: interface for the CTDLWebUpdater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDLWEBUPDATER_H__B88C8F13_1903_43B9_A9CC_BAF526C661E9__INCLUDED_)
#define AFX_TDLWEBUPDATER_H__B88C8F13_1903_43B9_A9CC_BAF526C661E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdlwebupdateprogressdlg.h"

//////////////////////////////////////////////////////////////////////

const LPCTSTR DOWNLOAD_URI				= _T("https://github.com/abstractspoon/ToDoList_Downloads/raw/master/Latest/todolist_exe.zip");
const LPCTSTR DOWNLOAD_URI_PRERELEASE	= _T("https://github.com/abstractspoon/ToDoList_Downloads/raw/master/Latest/todolist_exe_pre.zip");

//////////////////////////////////////////////////////////////////////

enum TDL_WEBUPDATE_RESULT
{
	TDLWUR_CANCELLED = -1,
	TDLWUR_SUCCESS,
		
	TDLWUR_ERR_APPFOLDER,
	TDLWUR_ERR_CONNECTED,
	TDLWUR_ERR_CREATEPROGRESSDLG,
	TDLWUR_ERR_DELETEBACKUPFOLDER,
	TDLWUR_ERR_DELETEFROMCACHE,
	TDLWUR_ERR_DELETETEMPZIP,
	TDLWUR_ERR_DELETEUNZIPFOLDER,
	TDLWUR_ERR_DOBACKUP,
	TDLWUR_ERR_DOUPDATE,
	TDLWUR_ERR_DOWNLOADZIP,
	TDLWUR_ERR_EXISTAPPFOLDER,
	TDLWUR_ERR_OPENZIP,
	TDLWUR_ERR_RESTOREBACKUP,
	TDLWUR_ERR_RUNRESTORE,
	TDLWUR_ERR_RUNUPDATE,
	TDLWUR_ERR_TEMPUPDATERFOLDER,
	TDLWUR_ERR_UNZIP,
	TDLWUR_ERR_UNZIPFOLDER,
	TDLWUR_ERR_UPDATERFOLDER,
	TDLWUR_ERR_UNEXPECTED,
};

//////////////////////////////////////////////////////////////////////

class CTDLWebUpdater : public IBindStatusCallback
{
public:
	CTDLWebUpdater(const CPoint& ptPos, BOOL bPreRelease);
	virtual ~CTDLWebUpdater();

	TDL_WEBUPDATE_RESULT DoUpdate(const CString& sAppFolder, const CString& sPrevCmdLine, 
								  BOOL bRestartElevated, BOOL bDownloadOnly = FALSE);

protected:
	CString m_sAppFolder;
	CString m_sAppPath;
	CString m_sBackupFolder;
	CString m_sUnzipFolder;
	CString m_sDownloadFile;
	
	const CString m_sDownloadUri;

	CTDLWebUpdateProgressDlg m_dlgProgress;
	TDL_WEBUPDATE_RESULT m_nResUpdate;

protected:
	BOOL SetAppFolder(const CString& sAppFolder);
	BOOL InitialiseTemporaries();
	BOOL DoProgressDialog(const CString& sPrevCmdLine, BOOL bRestartElevated);
	void CleanupTemporaries();
	BOOL CheckUpdateCancelled();
	TDL_WEBUPDATE_RESULT LogError(const CString& sAppFolder) const;
	void RestoreBackup(TDL_WEBUPDATE_PROGRESS nCancelled);

	BOOL OnUnzipProgress(int nPercent);

	static bool UnzipCallback(int nPercent, DWORD dwUserData);

protected:
	// IBindStatusCallback progress
	STDMETHOD(OnProgress)(ULONG ulProgress,
						  ULONG ulProgressMax,
						  ULONG ulStatusCode,
						  LPCWSTR szStatusText);

	// Minimum implementation
	STDMETHOD(QueryInterface)(REFIID, void **) { return E_NOINTERFACE; }
	STDMETHOD_(ULONG, AddRef)() { return 1; }
	STDMETHOD_(ULONG, Release)() { return 1; }
	STDMETHOD(OnStartBinding)(DWORD, IBinding *) { return E_NOTIMPL; }
	STDMETHOD(GetPriority)(LONG *) { return E_NOTIMPL; }
	STDMETHOD(OnLowResource)(DWORD) { return E_NOTIMPL; }
	STDMETHOD(OnStopBinding)(HRESULT, LPCWSTR) { return E_NOTIMPL; }
	STDMETHOD(GetBindInfo)(DWORD *, BINDINFO *) { return E_NOTIMPL; }
	STDMETHOD(OnDataAvailable)(DWORD, DWORD, FORMATETC *, STGMEDIUM *) { return E_NOTIMPL; }
	STDMETHOD(OnObjectAvailable)(REFIID, IUnknown *) { return E_NOTIMPL; }

};

#endif // !defined(AFX_TDLWEBUPDATER_H__B88C8F13_1903_43B9_A9CC_BAF526C661E9__INCLUDED_)
