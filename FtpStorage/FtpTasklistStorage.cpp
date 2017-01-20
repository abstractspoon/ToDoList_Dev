// FtpTasklistStorage.cpp: implementation of the CFtpTasklistStorage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "FtpTasklistStorage.h"

#include "..\shared\RemoteFile.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CKanbanBoardApp

static CFtpTasklistStorageApp theApp;

DLL_DECLSPEC ITasklistStorage* CreateTasklistStorageInterface()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return &theApp;
}

DLL_DECLSPEC int GetInterfaceVersion() 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ITASKLISTSTORAGE_VERSION; 
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFtpTasklistStorageApp::CFtpTasklistStorageApp() : m_hIcon(NULL)
{
}

CFtpTasklistStorageApp::~CFtpTasklistStorageApp()
{

}

bool CFtpTasklistStorageApp::RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* /*pDestTaskFile*/, 
										   IPreferences* pPrefs, LPCTSTR szKey, bool bSilent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sLocalPath(pFInfo->szLocalFileName);

	if (sLocalPath.IsEmpty())
		sLocalPath = FileMisc::GetTempFolder();

	// split the tasklist ID into it constituent parts
	CStringArray aIDParts;
	CString sRemotePath;
	CRemoteFile rf(GetMenuText());
	
	if (Misc::Split(pFInfo->szTasklistID, aIDParts, _T("::"), TRUE) == 3)
	{
		rf.SetServer(aIDParts[0]);
		sRemotePath = aIDParts[1];
		rf.SetUsername(aIDParts[2]);

		// only set the password if the other info was okay
		rf.SetPassword(pFInfo->szPassword);
	}

	DWORD dwOptions = RMO_CREATEDOWNLOADDIR | RMO_USETEMPFILE | RMO_KEEPFILENAME;
	
	if (!bSilent)
		dwOptions |= RMO_ALLOWDIALOG;

	if (rf.GetFile(sRemotePath, sLocalPath, pPrefs, szKey, dwOptions, CEnString(IDS_TDLFILEFILTER)) == RMERR_SUCCESS)
	{
		// return information to caller 
		_tcsncpy(pFInfo->szLocalFileName, sLocalPath, _MAX_PATH);
		_tcsncpy(pFInfo->szDisplayName, rf.FormatRemotePath(sRemotePath), _MAX_PATH);
		_tcsncpy(pFInfo->szPassword, rf.GetPassword(), ITS_PASSWORD_LEN);

		CString sTaskID = rf.GetServer() + _T("::") + sRemotePath + _T("::") + rf.GetUsername();
		_tcsncpy(pFInfo->szTasklistID, sTaskID, ITS_TASKLISTID_LEN);

		return true;
	}

	return false;
}

bool CFtpTasklistStorageApp::StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* /*pSrcTaskFile*/, 
										IPreferences* pPrefs, LPCTSTR szKey, bool bSilent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sLocalPath = pFInfo->szLocalFileName;

	// if not yet saved then save to temp filepath
	if (sLocalPath.IsEmpty())
		sLocalPath = FileMisc::GetTempFilePath(_T("rmf"));

	CRemoteFile rf;
	DWORD dwOptions = RMO_NOCANCELPROGRESS;

	if (bSilent)
		dwOptions |= RMO_NOPROGRESS;
	else
		dwOptions |= RMO_ALLOWDIALOG;

	// split the tasklist ID into it constituent parts
	CStringArray aIDParts;
	CString sRemotePath;
	
	if (Misc::Split(pFInfo->szTasklistID, aIDParts, _T("::"), TRUE) == 3)
	{
		rf.SetServer(aIDParts[0]);
		sRemotePath = aIDParts[1];
		rf.SetUsername(aIDParts[2]);

		// only set the password if the other info was okay
		rf.SetPassword(pFInfo->szPassword);
	}

	if (rf.SetFile(sLocalPath, sRemotePath, pPrefs, szKey, dwOptions) == RMERR_SUCCESS)
	{
		// return information to caller 
		_tcsncpy(pFInfo->szLocalFileName, sLocalPath, _MAX_PATH);
		_tcsncpy(pFInfo->szDisplayName, rf.FormatRemotePath(sRemotePath), _MAX_PATH);
		_tcsncpy(pFInfo->szPassword, rf.GetPassword(), ITS_PASSWORD_LEN);
		
		CString sTaskID = rf.GetServer() + _T("::") + sRemotePath + _T("::") + rf.GetUsername();
		_tcsncpy(pFInfo->szTasklistID, sTaskID, ITS_TASKLISTID_LEN);
		
		return true;
	}
	
	return false;
}

void CFtpTasklistStorageApp::SetLocalizer(ITransText* pTT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CLocalizer::Initialize(pTT);
}

void CFtpTasklistStorageApp::Release()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// Statically allocated -> do nothing
}

BOOL CFtpTasklistStorageApp::InitInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// Set this before anything else
//	CWinHelpButton::SetDefaultIcon(LoadIcon(IDI_HELP_BUTTON));

	m_hIcon = LoadIcon(IDR_FTPSTORAGE);

	if (m_pszAppName)
		free((void*)m_pszAppName);

	m_pszAppName = _tcsdup(GetMenuText());

	return CWinApp::InitInstance();
}
