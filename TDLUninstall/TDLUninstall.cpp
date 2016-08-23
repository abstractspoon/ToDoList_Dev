// TDLUninstall.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TDLUninstall.h"

#include "..\shared\filemisc.h"
#include "..\shared\fileregister.h"
#include "..\shared\regkey.h"
#include "..\shared\enstring.h"
#include "..\shared\EnCommandLineInfo.h"
#include "..\shared\rtlstylemgr.h"

#include "..\todolist\tdcswitch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLUninstallApp

BEGIN_MESSAGE_MAP(CTDLUninstallApp, CWinApp)
	//{{AFX_MSG_MAP(CTDLUninstallApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLUninstallApp construction

CTDLUninstallApp::CTDLUninstallApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTDLUninstallApp object

CTDLUninstallApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTDLUninstallApp initialization

BOOL CTDLUninstallApp::InitInstance()
{
	CEnCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// must have App ID
	if (cmdInfo.GetOption(SWITCH_APPID) == TDLAPPID)
	{
		// RTL support
		if (cmdInfo.HasOption(SWITCH_RTL))
			CRTLStyleMgr::Initialize();
		
		FileMisc::LogText(_T("CTDLUninstallApp::InitInstance(Has TDLAPPID"));

		// if the app folder is not specified then we copy ourselves
		// to the temp directory and run from there
		CString sAppFolder;

		if (cmdInfo.GetOption(SWITCH_APPFOLDER, sAppFolder) && !sAppFolder.IsEmpty())
		{
			// do the unistall
			DoUnistall(sAppFolder);
		}
		else
		{
			CString sAppPath = FileMisc::GetAppFilePath();
			CString sTempPath = FileMisc::GetTempFilePath(_T("TDLUninstall"), _T("exe"));
			
			if (FileMisc::CopyFile(sAppPath, sTempPath, TRUE, TRUE))
			{
				FileMisc::LogText(_T("CTDLUninstallApp::InitInstance(Copied to: %s)"), sTempPath);
				
				CEnCommandLineInfo params;

				params.SetOption(SWITCH_APPID, TDLAPPID);
				params.SetOption(SWITCH_APPFOLDER, FileMisc::GetAppFolder());

				if (CRTLStyleMgr::IsRTL())
					params.SetOption(SWITCH_RTL);

				if (FileMisc::Run(NULL, sTempPath, params.GetCommandLine()) <= 32)
				{
					// TODO
				}
			}
		}
	}
	
	// always quit
	return FALSE;
}

void CTDLUninstallApp::DoUnistall(const CString& sAppFolder)
{
	// sanity checks
	ASSERT (!sAppFolder.IsEmpty() && FileMisc::FolderExists(sAppFolder));
	
	// always enable logging to APPDATA because we will be in the TEMP folder
	FileMisc::EnableLogging(TRUE, NULL, (FMLF_TIMESTAMP | FMLF_TEMPDIR));
	
	// make sure we're in the temp folder
	CString sAppPath = FileMisc::GetAppFilePath();

	if (!FileMisc::IsTempFilePath(sAppPath))
	{
 		AfxMessageBox(CEnString(IDS_UNINSTALL_FAILURE), MB_ICONSTOP);
		return;
	}

	int nRet = DeleteFilesAndFolders(sAppFolder);
	
	switch (nRet)
	{
	case  0: AfxMessageBox(IDS_UNINSTALL_FAILURE); break;
	case  1: AfxMessageBox(IDS_UNINSTALL_SUCCESS); break;
	case -1: AfxMessageBox(IDS_UNINSTALL_DELAY);   break;
	}
	
	// Delete ourselves always
	::MoveFileEx(sAppPath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
}

int CTDLUninstallApp::DeleteFilesAndFolders(const CString& sAppFolder)
{
	// cleanup registry app settings
	CRegKey2::DeleteKey(HKEY_CURRENT_USER, _T("Software\\AbstractSpoon\\ToDoList"));
	CRegKey2::DeleteKey(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\AbstractSpoon_ToDoList"));

	// cleanup '.tdl' file extension
	CFileRegister(_T("tdl"), _T("tdl_Tasklist")).UnRegisterFileType();

	// cleanup 'tdl://' protocol 
	CRegKey2::DeleteKey(HKEY_CLASSES_ROOT, _T("tdl"));
	
	// delete APPDATA folders
	CString sAppData;
	
	if (FileMisc::GetSpecialFilePath(CSIDL_APPDATA, _T("Abstractspoon"), sAppData))
		FileMisc::DeleteFolder(sAppData);
	
	// Delete application folder
	return FileMisc::DeleteFolder(sAppFolder, FMDF_ALLOWDELETEONREBOOT | FMDF_SUBFOLDERS | FMDF_HIDDENREADONLY);
}
