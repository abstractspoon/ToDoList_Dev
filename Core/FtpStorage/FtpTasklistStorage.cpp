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
#include "..\shared\ServerDlg.h"
#include "..\shared\MessageBox.h"

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

CFtpTasklistStorageApp::CFtpTasklistStorageApp()
{
}

CFtpTasklistStorageApp::~CFtpTasklistStorageApp()
{
}

bool CFtpTasklistStorageApp::RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* /*pDestTaskFile*/, 
										   IPreferences* pPrefs, LPCTSTR szKey, bool bPrompt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sLocalPath(pFInfo->szLocalFileName);

	if (sLocalPath.IsEmpty())
		sLocalPath = FileMisc::GetTempFolder();

	// split the tasklist ID into it constituent parts
	CRemoteFile rf(GetMenuText());
	rf.SetIcon(m_icon);

	CStringArray aIDParts;
	CString sRemotePath;
	
	if (Misc::Split(pFInfo->szTasklistID, aIDParts, _T("::"), TRUE) == 3)
	{
		rf.SetServer(aIDParts[0]);
		sRemotePath = aIDParts[1];
		rf.SetUsername(aIDParts[2]);

		if (Misc::IsEmpty(pFInfo->szPassword))
			rf.SetPassword(m_sCachedPassword);
		else
			rf.SetPassword(pFInfo->szPassword);
	}

	DWORD dwOptions = RMO_CREATEDOWNLOADDIR | RMO_USETEMPFILE | RMO_KEEPFILENAME;
	
	if (bPrompt)
		dwOptions |= RMO_PROMPTFORFILE;

	switch (rf.GetFile(sRemotePath, sLocalPath, pPrefs, szKey, dwOptions, CEnString(IDS_TDLFILEFILTER)))
	{
	case RMERR_SUCCESS:
		{
			// Cache password for the session
			m_sCachedPassword = rf.GetPassword();

			// return information to caller 
			CopyInfo(sLocalPath, sRemotePath, rf, pFInfo);
		}
		return true;

	case RMERR_USERCANCELLED:
		break;

	default:
		CMessageBox::AfxShow(IDS_DOWNLOADERROR_TITLE, rf.GetLastError());
		break;
	}

	return false;
}

bool CFtpTasklistStorageApp::StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* /*pSrcTaskFile*/, 
										IPreferences* pPrefs, LPCTSTR szKey, bool bPrompt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sLocalPath = pFInfo->szLocalFileName;

	// if not yet saved then save to temp filepath
	if (sLocalPath.IsEmpty())
		sLocalPath = FileMisc::GetTempFilePath(_T("rmf"));

	CRemoteFile rf;
	rf.SetIcon(m_icon);

	DWORD dwOptions = RMO_NOCANCELPROGRESS;

	if (bPrompt)
		dwOptions |= RMO_PROMPTFORFILE;

	// split the tasklist ID into it constituent parts
	CStringArray aIDParts;
	CString sRemotePath;
	
	if (Misc::Split(pFInfo->szTasklistID, aIDParts, _T("::"), TRUE) == 3)
	{
		rf.SetServer(aIDParts[0]);
		sRemotePath = aIDParts[1];
		rf.SetUsername(aIDParts[2]);

		if (Misc::IsEmpty(pFInfo->szPassword))
			rf.SetPassword(m_sCachedPassword);
		else
			rf.SetPassword(pFInfo->szPassword);
	}
	else if (!Misc::IsEmpty(pFInfo->szTasklistName))
	{
		sRemotePath = pFInfo->szTasklistName;
	}
	else
	{
		sRemotePath = FileMisc::GetFileNameFromPath(pFInfo->szLocalFileName);
	}

	switch (rf.SetFile(sLocalPath, sRemotePath, pPrefs, szKey, dwOptions, CEnString(IDS_TDLFILEFILTER)))
	{
	case RMERR_SUCCESS:
		{
			// Cache password for the session
			m_sCachedPassword = rf.GetPassword();

			// return information to caller 
			CopyInfo(sLocalPath, sRemotePath, rf, pFInfo);
		}
		return true;

	case RMERR_USERCANCELLED:
		break;

	default:
		CMessageBox::AfxShow(IDS_UPLOADERROR_TITLE, rf.GetLastError());
		break;
	}

	return false;
}

void CFtpTasklistStorageApp::CopyInfo(const CString& sLocalPath, const CString& sRemotePath, const CRemoteFile& rmFrom, ITS_TASKLISTINFO* pToFInfo)
{
	lstrcpyn(pToFInfo->szLocalFileName, sLocalPath, _MAX_PATH);
	lstrcpyn(pToFInfo->szPassword, rmFrom.GetPassword(), ITS_PASSWORD_LEN);
	lstrcpyn(pToFInfo->szTasklistName, FileMisc::GetFileNameFromPath(sRemotePath, FALSE), _MAX_PATH);

	CString sDisplayPath = rmFrom.FormatRemotePath(sRemotePath);
	lstrcpyn(pToFInfo->szDisplayPath, sDisplayPath, _MAX_PATH);

	CString sTasklistID = rmFrom.GetServer() + _T("::") + sRemotePath + _T("::") + rmFrom.GetUsername();
	lstrcpyn(pToFInfo->szTasklistID, sTasklistID, ITS_TASKLISTID_LEN);
}

void CFtpTasklistStorageApp::SetLocalizer(ITransText* pTT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (CLocalizer::Initialize(pTT))
		CServerDlg::SetItemText(SD_TITLE, IDS_SD_TITLE);
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

	CMessageBox::SetAppName(GetMenuText());

	m_icon.Load(IDR_FTPSTORAGE);

	return CWinApp::InitInstance();
}
