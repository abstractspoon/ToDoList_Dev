// TDLUpdate.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TDLUpdate.h"
#include "tdlwebupdater.h"
#include "tdlwebupdateprogressdlg.h"

#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\EnCommandLineInfo.h"
#include "..\shared\webmisc.h"
#include "..\shared\localizer.h"
#include "..\shared\misc.h"
#include "..\shared\rtlstylemgr.h"

#include "..\todolist\tdcswitch.h"

#include "..\3rdparty\base64coder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLUpdateApp

BEGIN_MESSAGE_MAP(CTDLUpdateApp, CWinApp)
	//{{AFX_MSG_MAP(CTDLUpdateApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLUpdateApp construction

CTDLUpdateApp::CTDLUpdateApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTDLUpdateApp object

CTDLUpdateApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTDLUpdateApp initialization

BOOL CTDLUpdateApp::InitInstance()
{
	if (!WebMisc::IsOnline())
		return FALSE;

	CEnCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	
	// must have App ID
	if (cmdInfo.GetOption(SWITCH_APPID) == TDLAPPID)
	{
		// RTL support
		if (cmdInfo.HasOption(SWITCH_RTL))
			CRTLStyleMgr::Initialize();

		// position for more than one screen
		CPoint ptPos(_ttol(cmdInfo.GetOption(SWITCH_POSITION)));

#ifdef _DEBUG
		// check if TDL just wants to see the ui
		if (cmdInfo.HasOption(SWITCH_SHOWUI))
		{
			CTDLWebUpdateProgressDlg dialog(ptPos);
			dialog.DoModal();
		}
		else
#endif
		{
			BOOL bPreRelease = cmdInfo.HasOption(SWITCH_PRERELEASE);
			CString sLangFile = cmdInfo.GetOption(SWITCH_LANG);
			
			// if the app folder is not specified then we copy ourselves
			// to the temp directory and run from there
			CString sAppFolder;
			
			if (cmdInfo.GetOption(SWITCH_APPFOLDER, sAppFolder) && !sAppFolder.IsEmpty())
			{
				// initialize translator
				if (!sLangFile.IsEmpty())
				{
					if (cmdInfo.HasOption(SWITCH_ADDTODICT))
					{
						CLocalizer::Initialize(sLangFile, ITTTO_ADD2DICTIONARY);
						CLocalizer::ForceTranslateAllUIElements(IDS_WEBUPDATE_SUCCESS, IDS_WEBUPDATE_SUCCESS + 100);
					}
					else
						CLocalizer::Initialize(sLangFile, ITTTO_TRANSLATEONLY);
				}	

				// previous commandline is encoded
				CString sPrevCmdLine = Base64Coder::Decode(cmdInfo.GetOption(SWITCH_CMDLINE));

				// do the update
				DoUpdate(sAppFolder, sPrevCmdLine, bPreRelease, ptPos);

				CLocalizer::Release();
			}
			else
			{
				CString sAppPath = FileMisc::GetAppFilePath();
				CString sTempAppPath = FileMisc::GetTempFilePath(_T("TDLUpdate"), _T("exe"));
				
				if (FileMisc::CopyFile(sAppPath, sTempAppPath, TRUE, TRUE))
				{
					CEnCommandLineInfo params;
					
					params.SetOption(SWITCH_APPID, TDLAPPID);
					params.SetOption(SWITCH_APPFOLDER, FileMisc::GetAppFolder());
					params.SetOption(SWITCH_CMDLINE, cmdInfo.GetOption(SWITCH_CMDLINE));
					params.SetOption(SWITCH_POSITION, cmdInfo.GetOption(SWITCH_POSITION));
					
					if (CRTLStyleMgr::IsRTL())
						params.SetOption(SWITCH_RTL);
					
					if (!sLangFile.IsEmpty())
					{
						// copy TransText.dll also
						CString sTTPath = FileMisc::GetAppFolder() + _T("\\TransText.dll");
						CString sTempTTPath = FileMisc::GetTempFilePath(_T("TransText"), _T("dll"));

						// note: failure to copy just means no translation
						if (FileMisc::CopyFile(sTTPath, sTempTTPath, TRUE, TRUE))
						{
							params.SetOption(SWITCH_LANG, sLangFile);

							if (cmdInfo.HasOption(SWITCH_ADDTODICT))
								params.SetOption(SWITCH_ADDTODICT);
						}
					}

					if (bPreRelease)
						params.SetOption(SWITCH_PRERELEASE);

					if (FileMisc::Run(NULL, sTempAppPath, params.GetCommandLine()) <= 32)
					{
						// TODO
					}
				}
			}
		}
	}
	
	// always quit
	return FALSE;
}

void CTDLUpdateApp::DoUpdate(const CString& sAppFolder, const CString& sPrevCmdLine, BOOL bPreRelease, const CPoint& ptPos)
{
	// sanity checks
	ASSERT (!sAppFolder.IsEmpty() && FileMisc::FolderExists(sAppFolder));
	
	// always enable logging to APPDATA because we will be in the TEMP folder
	FileMisc::EnableLogging(TRUE, _T("Abstractspoon"), (FMLF_TIMESTAMP | FMLF_NOAPPDIR));
	FileMisc::LogAppModuleState(FBM_SORTBY_FILENAME);

	CTDLWebUpdater wu(ptPos, bPreRelease);
	TDL_WEBUPDATE_RESULT nRes = wu.DoUpdate(sAppFolder, sPrevCmdLine);
	
	// always copy update log to app folder
	CString sLogPath = FileMisc::GetLogFilePath();
	CString sUpdateLog = (FileMisc::TerminatePath(sAppFolder) + _T("Last_Update.log"));
	
	FileMisc::CopyFile(sLogPath, sUpdateLog, TRUE, TRUE);
	
	// error handling
	switch (nRes)
	{
	case TDLWUR_CANCELLED:
		AfxMessageBox(CEnString(IDS_WEBUPDATE_CANCEL), MB_ICONINFORMATION);
		break;
		
	case TDLWUR_SUCCESS:
		// all good
		break;
		
	case TDLWUR_ERR_APPFOLDER:
	case TDLWUR_ERR_CREATEPROGRESSDLG:
	case TDLWUR_ERR_DELETEBACKUPFOLDER:
	case TDLWUR_ERR_DELETEFROMCACHE:
	case TDLWUR_ERR_DELETETEMPZIP:
	case TDLWUR_ERR_DELETEUNZIPFOLDER:
	case TDLWUR_ERR_DOBACKUP:
	case TDLWUR_ERR_DOUPDATE:
	case TDLWUR_ERR_DOWNLOADZIP:
	case TDLWUR_ERR_EXISTAPPFOLDER:
	case TDLWUR_ERR_OPENZIP:
	case TDLWUR_ERR_RESTOREBACKUP:
	case TDLWUR_ERR_RUNRESTORE:
	case TDLWUR_ERR_RUNUPDATE:
	case TDLWUR_ERR_TEMPUPDATERFOLDER:
	case TDLWUR_ERR_UNZIP:
	case TDLWUR_ERR_UPDATERFOLDER:
		// prompt to display update log
		if (IDYES == AfxMessageBox(CEnString(IDS_WEBUPDATE_FAILURE), MB_YESNO | MB_ICONINFORMATION))
		{
			FileMisc::Run(NULL, sUpdateLog);
		}
		break;
		
	default:
		ASSERT(0);
		break;
	}
}
