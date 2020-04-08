// ToDoList.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "todolist.h"
#include "ToDoListWnd.h"
#include "tdlwelcomewizard.h"
#include "tdcenum.h"
#include "tdcmsg.h"
#include "tdlprefmigrationdlg.h"
#include "tdllanguagedlg.h"
#include "TDLCmdlineOptionsDlg.h"
#include "tdlolemessagefilter.h"
#include "tdlwebupdatepromptdlg.h"
#include "tdcstartupoptions.h"
#include "tdcanonymizetasklist.h"

#include "..\shared\encommandlineinfo.h"
#include "..\shared\driveinfo.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\regkey.h"
#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\autoflag.h"
#include "..\shared\localizer.h"
#include "..\shared\fileregister.h"
#include "..\shared\osversion.h"
#include "..\shared\rtlstylemgr.h"
#include "..\shared\winhelpbutton.h"
#include "..\shared\messagebox.h"
#include "..\shared\ScopedTimer.h"
#include "..\shared\BrowserDlg.h"

#include "..\3rdparty\xmlnodewrapper.h"
#include "..\3rdparty\ini.h"
#include "..\3rdparty\base64coder.h"
#include "..\3rdparty\LimitSingleInstance.h"
#include "..\3rdparty\gdiplus.h"

#include "..\Interfaces\Preferences.h"

#include "..\TDLUpdate\TDLWebUpdater.h" // for URIs

#include <afxpriv.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef LOAD_IGNORE_CODE_AUTHZ_LEVEL
#	define LOAD_IGNORE_CODE_AUTHZ_LEVEL 0x00000010
#endif

/////////////////////////////////////////////////////////////////////////////

CLimitSingleInstance g_SingleInstanceObj(_T("{3A4EFC98-9BA9-473D-A3CF-6B0FE644470D}")); 

/////////////////////////////////////////////////////////////////////////////

LPCTSTR REGKEY				= _T("AbstractSpoon");
LPCTSTR APPREGKEY			= _T("Software\\AbstractSpoon\\ToDoList");
LPCTSTR UNINSTALLREGKEY		= _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\AbstractSpoon_ToDoList");
LPCTSTR APPDATAINI			= _T("Abstractspoon\\ToDoList\\ToDoList.ini");

LPCTSTR WIKI_URL			= _T("https://www.abstractspoon.com/wiki/doku.php?id="); 
LPCTSTR FORUM_URL			= _T("https://www.abstractspoon.com/phpBB/"); 
LPCTSTR LICENSE_URL			= _T("https://www.abstractspoon.com/wiki/doku.php?id=free-open-source-software"); 
LPCTSTR DONATE_URL			= _T("https://www.paypal.com/cgi-bin/webscr?cmd=_xclick&business=abstractspoon2%40optusnet%2ecom%2eau&item_name=Software"); 

LPCTSTR MSVCR100_DLL		= _T("MSVCR100.dll"); 
LPCTSTR MSVCR100_URL		= _T("https://www.microsoft.com/en-hk/download/details.aspx?id=8328"); 

/////////////////////////////////////////////////////////////////////////////
// CToDoListApp

BEGIN_MESSAGE_MAP(CToDoListApp, CWinApp)
	//{{AFX_MSG_MAP(CToDoListApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP_FORUM, OnHelpForum)
	ON_COMMAND(ID_HELP_LICENSE, OnHelpLicense)
	ON_COMMAND(ID_HELP_COMMANDLINE, OnHelpCommandline)
	ON_COMMAND(ID_HELP_DONATE, OnHelpDonate)
	ON_COMMAND(ID_HELP_UNINSTALL, OnHelpUninstall)
	ON_COMMAND(ID_HELP_RECORDBUGREPORT, OnHelpRecordBugReport)
	ON_COMMAND(ID_HELP_WIKI, OnHelpWiki)

#ifdef _DEBUG
	ON_COMMAND(ID_DEBUGTASKDIALOG_INFO, OnDebugTaskDialogInfo)
	ON_COMMAND(ID_DEBUGSHOWUPDATEDLG, OnDebugShowUpdateDlg)
	ON_COMMAND(ID_DEBUGSHOWSCRIPTDLG, OnDebugShowScriptDlg)
	ON_COMMAND(ID_DEBUGTASKDIALOG_WARNING, OnDebugTaskDialogWarning)
	ON_COMMAND(ID_DEBUGTASKDIALOG_ERROR, OnDebugTaskDialogError)
	ON_COMMAND(ID_DEBUGSHOWLANGDLG, OnDebugShowLanguageDlg)
	ON_COMMAND(ID_DEBUGTESTSTABLEDOWNLOAD, OnDebugTestStableReleaseDownload)
	ON_COMMAND(ID_DEBUGTESTPREDOWNLOAD, OnDebugTestPreReleaseDownload)
	ON_COMMAND(ID_DEBUGSHOWEMBEDDEDURL, OnDebugShowEmbeddedUrl)
#endif

	ON_COMMAND(ID_TOOLS_CHECKFORUPDATES, OnHelpCheckForUpdates)
	ON_COMMAND(ID_TOOLS_IMPORTPREFS, OnImportPrefs)
	ON_COMMAND(ID_TOOLS_EXPORTPREFS, OnExportPrefs)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_IMPORTPREFS, OnUpdateImportPrefs)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EXPORTPREFS, OnUpdateExportPrefs)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToDoListApp construction

CToDoListApp::CToDoListApp() : CWinApp()
{
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CToDoListApp object

CToDoListApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CToDoListApp initialization

BOOL CToDoListApp::HandleSimpleQueries(const CEnCommandLineInfo& cmdInfo)
{
	// Is the exe version required
	if (cmdInfo.HasOption(SWITCH_VERSION))
	{
		CString sVerFile = cmdInfo.GetOption(SWITCH_VERSION);

		if (sVerFile.IsEmpty())
			sVerFile = _T("ver.txt");

		FileMisc::SaveFile(sVerFile, FileMisc::GetAppVersion(), SFEF_UTF8WITHOUTBOM);
		return TRUE;
	}

	// see if the user wants to uninstall
	if (cmdInfo.HasOption(SWITCH_UNINSTALL))
	{
		// we don't have the uninstaller run directly by the OS
		// to prevent the APPID appearing in plain text
		RunUninstaller();
		return TRUE;
	}

	// see if the user just wants to see the commandline options
	if (cmdInfo.HasOption(SWITCH_HELP1) || 
		cmdInfo.HasOption(SWITCH_HELP2) || 
		cmdInfo.HasOption(SWITCH_HELP3))
	{
		OnHelpCommandline();
		return TRUE;
	}

	// Randomising doesn't need preferences
	if (cmdInfo.HasOption(SWITCH_RANDOMISE))
	{
		CTDCAnonymizeTasklist::Anonymize(cmdInfo.GetOption(SWITCH_RANDOMISE));
		return TRUE;
	}

	return FALSE;
}

BOOL CToDoListApp::HasVS2010Redistributable()
{
	CString sVs2010Runtime;
	VERIFY(FileMisc::GetSpecialFilePath(CSIDL_SYSTEM, MSVCR100_DLL, sVs2010Runtime));

	if (::LoadLibraryEx(sVs2010Runtime, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL) == NULL)
	{
		CToDoListWnd::EnableLogging();

		FileMisc::LogText(_T("LoadLibrary(%s) failed"), sVs2010Runtime);
		FileMisc::LogTextRaw(Misc::FormatGetLastError());
		
		if (AfxMessageBox(CEnString(IDS_MSVCR100_MSG), MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK)
			FileMisc::Run(::GetDesktopWindow(), MSVCR100_URL);

		// Always quit
		return FALSE;
	}

	// All good
	return TRUE;
}

BOOL CToDoListApp::InitInstance()
{
	// .NET plugins require VS2010 redistributable to be installed
	if (!HasVS2010Redistributable())
		return FALSE;

	// Set up icons that might be required during startup
	if (m_iconBrowse.Load(IDI_FILEEDIT_BROWSE) && m_iconGo.Load(IDI_FILEEDIT_GO))
		CFileEdit::SetDefaultButtonImages(m_iconBrowse, m_iconGo);
	
	if (m_iconHelp.Load(IDI_HELPBUTTON))
		CWinHelpButton::SetDefaultIcon(m_iconHelp);

	// Process commandline switches
	CEnCommandLineInfo cmdInfo(_T(".tdl;.xml"));
	ParseCommandLine(cmdInfo);

	if (HandleSimpleQueries(cmdInfo))
		return FALSE; // quit

	// If this is a restart, wait until the previous instance has closed
	if (cmdInfo.HasOption(SWITCH_RESTART))
	{
		DWORD dwProcID = _ttoi(cmdInfo.GetOption(SWITCH_RESTART));
		ASSERT(dwProcID);

		if (!WaitForInstanceToClose(dwProcID))
		{
			// message box the user
			// TODO

			return FALSE; // quit app
		}

		// else strip off the restart switch and continue
		cmdInfo.DeleteOption(SWITCH_RESTART);
		
		// and turn on logging to capture the first run
		CToDoListWnd::EnableLogging();
	}

	AfxOleInit(); // for initializing COM and handling drag and drop via explorer
	AfxEnableControlContainer(); // embedding IE

	// before anything else make sure we've got MSXML3 installed
	if (!CXmlDocumentWrapper::IsVersion3orGreater())
	{
		AfxMessageBox(CEnString(IDS_BADMSXML));
		return FALSE; // quit app
	}

	// init prefs 
	if (!InitPreferences(cmdInfo))
		return FALSE; // quit app

	// commandline options
	CTDCStartupOptions startup(cmdInfo);

	if (ProcessStartupOptions(startup, cmdInfo))
		return FALSE; // quit

	// if no one handled it simply create a new instance
	CToDoListWnd* pTDL = new CToDoListWnd;
	
	if (pTDL && pTDL->Create(startup))
	{
		// If we have just been upgraded we will have admin
		// privileges which will stop single-instance and 
		// Windows drag'n'drop from working because Windows 
		// will prevent non-elevated apps from talking to us.
		if (startup.HasFlag(TLD_UPGRADED))
		{
			GraphicsMisc::ChangeWindowMessageFilter(WM_COPYDATA, TRUE);
			GraphicsMisc::ChangeWindowMessageFilter(WM_DROPFILES, TRUE);
		}

		m_pMainWnd = pTDL;
		return TRUE;
	}

	// else
	return FALSE; // quit app
}

BOOL CToDoListApp::ProcessStartupOptions(CTDCStartupOptions& startup, const CEnCommandLineInfo& cmdInfo) const
{
	// See if another instance can better handle this than us
	TDCFINDWND find;
	int nNumWnds = CToDoListWnd::FindToDoListWnds(find);

	// If another instance is just starting up or closing down
	// ie. has no window handle or is blocking we hang around 
	// for a bit to see what happens
	if (!nNumWnds && g_SingleInstanceObj.IsAnotherInstanceRunning(TRUE)) 
	{
		int nTry = 5;
		
		while (nTry-- && !nNumWnds)
		{
			Sleep(1000);

			if (!g_SingleInstanceObj.IsAnotherInstanceRunning(TRUE))
				break; // we're now the only instance

			nNumWnds = CToDoListWnd::FindToDoListWnds(find);
		}
	}

	// PERMANENT LOGGING ////////////////////////////////////////////
	FileMisc::LogText(_T("CToDoListApp::ProcessStartupOptions found %d other instances"), nNumWnds);
	/////////////////////////////////////////////////////////////////

	if (!nNumWnds)
		return FALSE;

	// If more than one instance is open (ie. more than one possible target), 
	// then a non-empty commandline without a file path or task link is treated as an error
	BOOL bHasFilePath = startup.HasFilePath();
	BOOL bTaskLink = startup.HasFlag(TLD_TASKLINK);

	if ((nNumWnds > 1) && !bHasFilePath && !bTaskLink && !startup.IsEmpty(TRUE))
	{
		AfxMessageBox(CEnString(IDS_MULTIINSTANCENOFILEPATH), (MB_OK | MB_ICONERROR));
		return TRUE; // handled
	}

	// if there IS a tasklist on the commandline and we are NOT importing it, 
	// see if any other instance already has it loaded 
	HWND hwndOtherInst = NULL;

	if (bHasFilePath && !startup.HasFlag(TLD_IMPORTFILE))
	{
		for (int nWnd = 0; nWnd < nNumWnds; nWnd++)
		{
			HWND hWnd = find.aResults[nWnd];

			if (SendStartupOptions(hWnd, startup, TDLCD_HASTASKFILE))
			{
				// PERMANENT LOGGING ////////////////////////////////////////////
				FileMisc::LogText(_T("CToDoListApp::ProcessStartupOptions found instance with '%s' already loaded"), cmdInfo.m_strFileName);
				/////////////////////////////////////////////////////////////////

				SendStartupOptions(hWnd, startup, TDLCD_PROCESSSTARTUP);

				// We don't worry if this fails because:
				// 1. Either this is the only instance that can handle 
				//    the filepath
				// 2. OR we may yet find another instance so we just 
				//    keep going
				hwndOtherInst = hWnd;
			}
		}
	}

	// if no other instance had it open and _we_ are single instance
	// we then see if any of the instances is willing to handle it
	BOOL bMultiInstance = CPreferences().GetProfileInt(_T("Preferences"), _T("MultiInstance"), FALSE);
	BOOL bTasklistOpened = FALSE;

	if ((hwndOtherInst == NULL) && !bMultiInstance)
	{
		// PERMANENT LOGGING ////////////////////////////////////////////
		FileMisc::LogText(_T("CToDoListApp::ProcessStartupOptions found no other instance with '%s' already loaded"), cmdInfo.m_strFileName);
		/////////////////////////////////////////////////////////////////

		for (int nWnd = 0; nWnd < nNumWnds; nWnd++)
		{
			HWND hWnd = find.aResults[nWnd];

			if (SendStartupOptions(hWnd, startup, TDLCD_PROCESSSTARTUP))
			{
				// PERMANENT LOGGING ////////////////////////////////////////////
				FileMisc::LogText(_T("CToDoListApp::ProcessStartupOptions found other instance to handle '%s'"), cmdInfo.m_strFileName);
				/////////////////////////////////////////////////////////////////

				hwndOtherInst = hWnd;
				bTasklistOpened = startup.HasFilePath();
				break;
			}
		}

		// If still nothing just pick the first other instance
		if (hwndOtherInst == NULL)
			hwndOtherInst = find.aResults[0];
	}
	
	// If we found a window, we may need to bring it to the foreground
	if (hwndOtherInst)
	{
		// we only show and bring to the foreground 
		// the other app window if:
		// 0. There are no commandline options
		// 1. This is a task link
		// 2. A tasklist was opened or imported
		// 3. A new task name requires editing
		// 4. A command requiring UI executed
		BOOL bShow = (startup.IsEmpty(TRUE) || bTaskLink ||
					(startup.HasFilePath() && bTasklistOpened) ||
					(startup.HasFlag(TLD_NEWTASK) && !startup.HasNewTaskTitle()));

		if (!bShow && startup.HasCommandID())
		{
			CUIntArray aCmdIDs;
			int nCmd = startup.GetCommandIDs(aCmdIDs);

			while (nCmd-- && !bShow)
				bShow = CommandRequiresUI(aCmdIDs[nCmd]);
		}

		if (bShow)
		{
			::SendMessage(hwndOtherInst, WM_TDL_SHOWWINDOW, 0, 0);
			::SetForegroundWindow(hwndOtherInst);
		}

		return TRUE; // to quit this instance
	}
	ASSERT(bMultiInstance);

	// To prevent the unexpected consequences of opening the
	// previously open tasklists in multiple instances we check:
	//
	// 1. Do we have our own ini file
	if (FileMisc::FileExists(cmdInfo.GetOption(SWITCH_INIFILE)))
	{
		// All good
	}
	// 2. Do we have our own tasklist(s)
	else if (startup.HasFilePath())
	{
		// All good
	}
	// 3. Do we have a unique filepath
	else
	{
		CString sThisModulePath = FileMisc::GetAppFilePath();

		for (int nWnd = 0; nWnd < nNumWnds; nWnd++)
		{
			HWND hWnd = find.aResults[nWnd];
			CString sOtherModulePath = FileMisc::GetWindowModuleFilePath(hWnd);

			if (FileMisc::IsSamePath(sThisModulePath, sOtherModulePath))
			{
				// No, so we start this instance empty
				startup.ModifyFlags(0, TLD_STARTEMPTY); 
				break;
			}
		}
	}

	return FALSE; // no need to quit
}

BOOL CToDoListApp::SendStartupOptions(HWND hWnd, const CTDCStartupOptions& startup, TDL_COPYDATA nMsg)
{
	COPYDATASTRUCT cds = { nMsg, sizeof(startup), (void*)&startup };

	if (::IsWindow(hWnd))
		return (::SendMessage(hWnd, WM_COPYDATA, NULL, (LPARAM)&cds) != 0);

	// else
	return FALSE;
}

BOOL CToDoListApp::ValidateTasklistPath(CString& sPath)
{
	if (FileMisc::HasExtension(sPath, _T("tdl")) ||
		FileMisc::HasExtension(sPath, _T("xml")))
	{
		return ValidateFilePath(sPath);
	}

	// else log it
	FileMisc::LogText(_T("Taskfile '%s' had an invalid extension\n"), sPath);

	return FALSE;
}

BOOL CToDoListApp::GetDefaultIniPath(CString& sIniPath, BOOL bCheckExists)
{
	CString sExePath = FileMisc::GetAppFilePath();
	CString sExeFolder = FileMisc::GetAppFolder();
	
	// Preferred default location is app folder for portability
	CString sExeIniPath = FileMisc::ReplaceExtension(sExePath, _T("ini"));
	
	// However may already be in AppData/Roaming from previous version
	CString sAppDataIniPath;
	VERIFY (FileMisc::GetSpecialFilePath(CSIDL_APPDATA, APPDATAINI, sAppDataIniPath));

	BOOL bHasAppDataIni = FileMisc::FileExists(sAppDataIniPath);
	BOOL bHasExeIni = FileMisc::FileExists(sExeIniPath);
	
	CString sTestIni;

	if (bHasExeIni && bHasAppDataIni)
	{
		// Prefer most recently modified file
		if (FileMisc::GetFileLastModified(sExeIniPath) > 
			FileMisc::GetFileLastModified(sAppDataIniPath))
		{
			sTestIni = sExeIniPath;
		}
		else
		{
			sTestIni = sAppDataIniPath;
		}
		
		FileMisc::LogText(_T("Using newer ini '%s'\n"), sTestIni);
	}
	else if (bHasExeIni)
	{
		sTestIni = sExeIniPath;
	}
	else if (bHasAppDataIni)
	{
		sTestIni = sAppDataIniPath;
	}
	else if (FileMisc::IsFolderWritable(sExeFolder))
	{
		sTestIni = sExeIniPath;
	}
	else
	{
		sTestIni = sAppDataIniPath;
	}

	if (ValidateIniPath(sTestIni, bCheckExists))
	{

		sIniPath = sTestIni;
		return TRUE;
	}
	
	// else
	return FALSE;
}

BOOL CToDoListApp::ValidateIniPath(CString& sFilePath, BOOL bCheckExists)
{
	ASSERT(!::PathIsRelative(sFilePath));

	if (::PathIsRelative(sFilePath))
		return FALSE;

	CString sIniPath(sFilePath);
	FileMisc::ReplaceExtension(sIniPath, _T("ini"));

	// check existence as required
	BOOL bFileExists = FileMisc::FileExists(sIniPath);

	if (bCheckExists && !bFileExists)
		return FALSE;

	// make sure it's writable
	if (bFileExists)
		::SetFileAttributes(sIniPath, FILE_ATTRIBUTE_NORMAL);

	// check containing folder is writable
	CString sIniFolder = FileMisc::GetFolderFromFilePath(sIniPath);

	FileMisc::CreateFolder(sIniFolder);
	::SetFileAttributes(sIniFolder, FILE_ATTRIBUTE_NORMAL);
		
	// Note: If the ini file already exists in a non-writable folder
	// (eg. Program Files) then Windows will allow us to carry on
	// using it even though backing up the ini will fail.
	if (bFileExists || FileMisc::IsFolderWritable(sIniFolder))
	{
		sFilePath = sIniPath;
		return TRUE;
	}

	return FALSE;
}

BOOL CToDoListApp::ValidateFilePath(CString& sPath, const CString& sExt)
{
	if (sPath.IsEmpty())
		return FALSE;

	CString sTemp(sPath);

	if (!sExt.IsEmpty())
		FileMisc::ReplaceExtension(sTemp, sExt);

	// don't change sTemp
	CString sFullPath(sTemp);

	// if relative check app folder first
	if (::PathIsRelative(sTemp))
	{
		sFullPath = FileMisc::GetFullPath(sTemp, FileMisc::GetAppFolder());

		// then try CWD
		if (!FileMisc::FileExists(sFullPath))
			sFullPath = FileMisc::GetFullPath(sTemp);
	}

	// test file existence
	if (FileMisc::FileExists(sFullPath))
	{
		sPath = sFullPath;
		return TRUE;
	}

	// else log it
	if (FileMisc::IsLoggingEnabled())
	{
		if (::PathIsRelative(sTemp))
		{
			FileMisc::LogText(_T("File '%s' not found in '%s' or '%s'\n"),
								sTemp, 
								FileMisc::GetAppFolder(), 
								FileMisc::GetCwd());
		}
		else 
		{
			FileMisc::LogText(_T("File '%s' not found\n"), sFullPath);
		}
	}

	return FALSE;
}

// our own local version
CString CToDoListApp::AfxGetAppName()
{
	return CToDoListWnd::GetTitle(FALSE);
}

void CToDoListApp::ParseCommandLine(CEnCommandLineInfo& cmdInfo)
{
	CWinApp::ParseCommandLine(cmdInfo); // default

	// turn on logging if requested
	if (cmdInfo.HasOption(SWITCH_LOGGING))
	{
		cmdInfo.DeleteOption(SWITCH_LOGGING);

		CToDoListWnd::EnableLogging();
	}

	// validate ini path if present
    if (cmdInfo.HasOption(SWITCH_INIFILE))
	{
		CString sIniPath = cmdInfo.GetOption(SWITCH_INIFILE);

		// always delete existing item
		cmdInfo.DeleteOption(SWITCH_INIFILE);

		// must have ini extension
		if (FileMisc::HasExtension(sIniPath, _T("ini")) &&
			ValidateFilePath(sIniPath))
		{
			// save full path
			cmdInfo.SetOption(SWITCH_INIFILE, sIniPath);
		}
	}

	// Validate randomise path
	if (cmdInfo.HasOption(SWITCH_RANDOMISE))
	{
		CString sFilePath = cmdInfo.GetOption(SWITCH_RANDOMISE);

		// always delete existing item
		cmdInfo.DeleteOption(SWITCH_RANDOMISE);
		
		if (ValidateTasklistPath(sFilePath))
		{
			// save full path
			cmdInfo.SetOption(SWITCH_RANDOMISE, sFilePath);
		}
	}

	// validate import path
	if (cmdInfo.HasOption(SWITCH_IMPORT))
	{
		CString sImportPath = cmdInfo.GetOption(SWITCH_IMPORT);

		// always delete existing item
		cmdInfo.DeleteOption(SWITCH_IMPORT);
		
		if (ValidateFilePath(sImportPath))
		{
			// save full path
			cmdInfo.SetOption(SWITCH_IMPORT, sImportPath);
		}
	}

	// validate main file path
	if (!cmdInfo.m_strFileName.IsEmpty())
	{
		if (!ValidateTasklistPath(cmdInfo.m_strFileName))
			cmdInfo.m_strFileName.Empty();
	}

	// validate multiple filepaths
	if (cmdInfo.HasOption(SWITCH_TASKFILE))
	{
		CString sTaskFiles = cmdInfo.GetOption(SWITCH_TASKFILE);
		CStringArray aTaskFiles; 

		int nFile = Misc::Split(sTaskFiles, aTaskFiles, '|');

		while (nFile--)
		{
			CString& sTaskfile = aTaskFiles[nFile];

			if (!ValidateTasklistPath(sTaskfile))
			{
				aTaskFiles.RemoveAt(nFile);
			}
			// also remove if it's a dupe of m_strFileName
			else if (cmdInfo.m_strFileName == sTaskfile)
			{
				aTaskFiles.RemoveAt(nFile);
			}
		}

		// save results
		cmdInfo.DeleteOption(SWITCH_TASKFILE);

		if (aTaskFiles.GetSize())
			cmdInfo.SetOption(SWITCH_TASKFILE, Misc::FormatArray(aTaskFiles, '|'));
	}
}

BOOL CToDoListApp::PreTranslateMessage(MSG* pMsg) 
{
	// give first chance to main window for handling accelerators
	if (m_pMainWnd && m_pMainWnd->PreTranslateMessage(pMsg))
		return TRUE;

	// -------------------------------------------------------------------
	// Implement CWinApp::PreTranslateMessage(pMsg)	ourselves
	// so as to not call CMainFrame::PreTranslateMessage(pMsg) twice

	// if this is a thread-message, short-circuit this function
	if (pMsg->hwnd == NULL && DispatchThreadMessageEx(pMsg))
		return TRUE;

	// walk from target to main window but excluding main window
	for (HWND hWnd = pMsg->hwnd; 
		hWnd != NULL && hWnd != m_pMainWnd->GetSafeHwnd(); 
		hWnd = ::GetParent(hWnd))
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);

		if (pWnd != NULL)
		{
			// target window is a C++ window
			if (pWnd->PreTranslateMessage(pMsg))
				return TRUE; // trapped by target window (eg: accelerators)
		}
	}
	// -------------------------------------------------------------------

	return FALSE;       // no special processing
	//return CWinApp::PreTranslateMessage(pMsg);
}

void CToDoListApp::OnHelpWiki() 
{ 
	DoHelp();
}

void CToDoListApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	if (nCmd == HELP_CONTEXT)
		DoHelp(dwData);
}

void CToDoListApp::DoHelp(UINT nHelpID)
{
	CString sHelpID;

	// nHelpID may already be the address of a literal string
	if (HIWORD(nHelpID))
	{
		sHelpID = (LPCTSTR)nHelpID;
	}
	else
	{
#define CASE(id) case id: sHelpID = #id; break;
		
		switch (nHelpID)
		{
			CASE(0) // App itself
			CASE(IDD_ABOUT_DIALOG)               
			CASE(IDD_ADDCUSTOMATTRIB_DIALOG)      
			CASE(IDD_ADDLOGGEDTIME_DIALOG)        
			CASE(IDD_ANALYSELOGGEDTIME_DIALOG)   
			CASE(IDD_CLEANUPINIPREFERENCES_DIALOG)
			CASE(IDD_CMDLINEOPTIONS_DIALOG)       
			CASE(IDD_COLUMNSELECTION_DIALOG)      
			CASE(IDD_CSVIMPORTEXPORT_DIALOG)      
			CASE(IDD_EXPORT_DIALOG)               
			CASE(IDD_FILTER_BAR)                  
			CASE(IDD_FILTER_DIALOG)               
			CASE(IDD_FINDTASKS_DIALOG)            
			CASE(IDD_GOTOTASK_DIALOG)
			CASE(IDD_IMPORT_DIALOG)               
			CASE(IDD_IMPORT_OUTLOOK_DIALOG)
			CASE(IDD_KEYBOARDSHORTCUTDISPLAYDIALOG)
			CASE(IDD_LANGUAGE_DIALOG)             
			CASE(IDD_MULTISORT_DIALOG)            
			CASE(IDD_OFFSETDATES_DIALOG)          
			CASE(IDD_OUTLOOKMSGIMPORT_DIALOG)     
			CASE(IDD_PREFERENCES)                 
			CASE(IDD_PREFEXPORT_PAGE)             
			CASE(IDD_PREFFILE2_PAGE)              
			CASE(IDD_PREFFILE_PAGE)               
			CASE(IDD_PREFGEN_PAGE)                
			CASE(IDD_PREFMULTIUSER_PAGE)          
			CASE(IDD_PREFSHORTCUTS_PAGE)          
			CASE(IDD_PREFTASKCALC_PAGE)           
			CASE(IDD_PREFTASKDEF_PAGE)            
			CASE(IDD_PREFTASKDEF2_PAGE)
			CASE(IDD_PREFTASK_PAGE)               
			CASE(IDD_PREFTOOLBAR_PAGE)
			CASE(IDD_PREFTOOLS_PAGE)              
			CASE(IDD_PREFUITASKLISTCOLORS_PAGE)   
			CASE(IDD_PREFUITASKLIST_PAGE)         
			CASE(IDD_PREFUIVISIBILITY_PAGE)      
			CASE(IDD_PREFUI_PAGE)                 
			CASE(IDD_PRINT_DIALOG)                
			//	CASE(IDD_PRINT_STYLE_PAGE)
			//	CASE(IDD_PRINT_TASKSEL_PAGE)
			CASE(IDD_RECURRING_TASK_DIALOG)       
			//	CASE(IDD_RECURRING_DAILY_PAGE)        
			//	CASE(IDD_RECURRING_MONTHLY_PAGE)      
			//	CASE(IDD_RECURRING_ONCE_PAGE)        
			//	CASE(IDD_RECURRING_WEEKLY_PAGE)       
			//	CASE(IDD_RECURRING_YEARLY_PAGE)       
			CASE(IDD_REGMIGRATION_DIALOG)         
			CASE(IDD_REUSERECURRINGTASK_DIALOG)   
			CASE(IDD_SENDTASKS_DIALOG)            
			CASE(IDD_SETREMINDER_DIALOG)          
			CASE(IDD_SHOWREMINDER_DIALOG)         
			CASE(IDD_TASKICON_DIALOG)             
			CASE(IDD_TASKLISTSAVEAS_DIALOG)          
			//	CASE(IDD_TASKSELECTION_DIALOG)        
			CASE(IDD_TDLIMPORTEXPORT_DIALOG)      
			CASE(IDD_TIMETRACK_DIALOG)             
			CASE(IDD_TRANSFORM_DIALOG)            
			CASE(IDD_WEBUPDATE_PROMPT_PAGE)    
			CASE(IDD_WELCOME_PAGE1)               
			//	CASE(IDD_WELCOME_PAGE2)               
			//	CASE(IDD_WELCOME_PAGE3)               
				
		default:
			ASSERT(0);
			break;
		}
	}

	CString sHelpPage;

	if (!sHelpID.IsEmpty())
	{
		// Load the Help.ini file to find the URL for this topic
		CIni ini(GetResourcePath(_T("Misc"), _T("Help.ini")));
		sHelpPage = ini.GetString(_T("Help IDs"), sHelpID);
	}
	else
	{
		TRACE(_T("CToDoListApp::DoHelp(%d = no help ID)\n"), nHelpID);
	}

#ifdef _DEBUG
	CString sHelpMsg;
	sHelpMsg.Format(_T("DoHelp(%s = %s)   Do you want to go here?"), sHelpID, sHelpPage);

	if (AfxMessageBox(sHelpMsg, MB_YESNO) != IDYES)
		return;
#endif

	FileMisc::Run(*m_pMainWnd, (WIKI_URL + sHelpPage), NULL, SW_SHOWNORMAL);
}

BOOL CToDoListApp::InitPreferences(CEnCommandLineInfo& cmdInfo)
{
	BOOL bUseIni = FALSE;
	BOOL bSetMultiInstance = FALSE;
	BOOL bRegKeyExists = CRegKey2::KeyExists(HKEY_CURRENT_USER, APPREGKEY);
	
#ifdef _DEBUG
	BOOL bQuiet = cmdInfo.HasOption(SWITCH_QUIET);
#else
	BOOL bQuiet = FALSE;
#endif

    CString sIniPath;

    // try command line override first
    if (cmdInfo.GetOption(SWITCH_INIFILE, sIniPath))
    {
		ASSERT(!sIniPath.IsEmpty());
		ASSERT(!::PathIsRelative(sIniPath));
		ASSERT(FileMisc::PathExists(sIniPath));

		bUseIni = ValidateIniPath(sIniPath, FALSE);

		if (!bUseIni)
		{
			FileMisc::LogText(_T("Specified ini file not found: %s"), sIniPath);
			sIniPath.Empty();
		}
	}
	else if (!cmdInfo.m_strFileName.IsEmpty())
	{
		ASSERT(!::PathIsRelative(cmdInfo.m_strFileName));
		ASSERT(FileMisc::PathExists(cmdInfo.m_strFileName));
		
		// else if there is a tasklist on the commandline 
		// then try for an ini file of the same name
		sIniPath = cmdInfo.m_strFileName;

		if (ValidateIniPath(sIniPath, TRUE))
		{
			bUseIni = TRUE;
			FileMisc::LogText(_T("Ini file matching specified tasklist found: %s"), sIniPath);

			// enable multi-instance because this tasklist
			// has its own ini file
			bSetMultiInstance = TRUE;
		}
	}

	// if all else fails then try for the default ini file
	if (!bUseIni)
	{
		if (GetDefaultIniPath(sIniPath, TRUE))
		{
			bUseIni = TRUE;
		}
		else // handle quiet start
		{
			bUseIni = bQuiet;
		}
	}

	// check existing prefs
	BOOL bFirstTime = (!bUseIni && !bRegKeyExists);

	if (!bFirstTime)
	{
		if (!SetPreferences(bUseIni, sIniPath, TRUE))
		{
			// Notify user and quit app to avoid overwriting the ini file
			AfxMessageBox(CEnString(IDS_INIFILELOCKED), MB_OK);
			return FALSE; 
		}

		// Has the user already chosen a language?
		if (!InitTranslation(FALSE, bQuiet))
		{ 
			// user cancelled -> Quit app
			return FALSE;
		}

		CPreferences prefs;

		// Save language choice 
		FileMisc::MakeRelativePath(m_sLanguageFile, FileMisc::GetAppFolder(), FALSE);
		prefs.WriteProfileString(_T("Preferences"), _T("LanguageFile"), m_sLanguageFile);
		
		if (bSetMultiInstance)
			prefs.WriteProfileInt(_T("Preferences"), _T("MultiInstance"), TRUE);

		// check for web updates
		if (prefs.GetProfileInt(_T("Preferences"), _T("AutoCheckForUpdates"), FALSE))
		{
			TDL_WEBUPDATE_CHECK nCheck = CheckForUpdates(FALSE);

			if ((nCheck == TDLWUC_WANTUPDATE) || (nCheck == TDLWUC_WANTPRERELEASEUPDATE))
			{
				RunUpdater(nCheck == TDLWUC_WANTPRERELEASEUPDATE);
				return FALSE; // quit app
			}
		}
		
		CString sPrevVer = prefs.GetProfileString(_T("AppVer"), _T("Version"));

		CleanupAppFolder(sPrevVer);
		FixupExampleTasklistsTaskDates(sPrevVer);
		UpgradePreferences(prefs, sPrevVer);
	}
	else  // first time so no ini file exists. show wizard
	{
		ASSERT(!bQuiet);

		if (!InitTranslation(bFirstTime, bQuiet))
			return FALSE; // user cancelled -> Quit app

		FileMisc::LogText(_T("Neither ini file nor registry settings found -> Showing setup wizard"));

		CTDLWelcomeWizard wizard(CToDoListWnd::GetVersion());
		
		if (wizard.DoModal() != ID_WIZFINISH)
			return FALSE; // quit app
		
		// use whichever ini location has a writable folder
		bUseIni = (wizard.GetUseIniFile() && GetDefaultIniPath(sIniPath, FALSE));
		SetPreferences(bUseIni, sIniPath, FALSE);
		
		// initialize prefs to defaults
		CPreferencesDlg().InitializePreferences();

		// Save language choice 
		CPreferences prefs;

		FileMisc::MakeRelativePath(m_sLanguageFile, FileMisc::GetAppFolder(), FALSE);
		prefs.WriteProfileString(_T("Preferences"), _T("LanguageFile"), m_sLanguageFile);
		
		// set up some default preferences
		if (wizard.GetShareTasklists()) 
		{
			// set up source control for remote tasklists
			prefs.WriteProfileInt(_T("Preferences"), _T("EnableSourceControl"), TRUE);
			prefs.WriteProfileInt(_T("Preferences"), _T("SourceControlLanOnly"), TRUE);
			prefs.WriteProfileInt(_T("Preferences"), _T("AutoCheckOut"), TRUE);
			prefs.WriteProfileInt(_T("Preferences"), _T("CheckoutOnCheckin"), TRUE);
			prefs.WriteProfileInt(_T("Preferences"), _T("CheckinOnClose"), TRUE);
			prefs.WriteProfileInt(_T("Preferences"), _T("CheckinNoEditTime"), 1);
			prefs.WriteProfileInt(_T("Preferences"), _T("CheckinNoEdit"), TRUE);
			prefs.WriteProfileInt(_T("Preferences"), _T("Use3rdPartySourceControl"), FALSE);
		}
		
		// setup column visibility
		TDCCOLEDITFILTERVISIBILITY vis;
		wizard.GetColumnVisibility(vis);

		vis.Save(prefs, _T("Preferences"));
		prefs.WriteProfileInt(_T("Preferences"), _T("ShowEditMenuAsColumns"), wizard.GetHideAttributes());

		// Save this config
		if (prefs.UsesIni())
			prefs.Save();
		
		// set up initial file
		CString sSample = wizard.GetSampleFilePath();
		
		if (!sSample.IsEmpty())
			cmdInfo.m_strFileName = sSample;

		// setup uninstaller to point to us
		if (!bUseIni)
		{
			CRegKey2 reg;

			if (reg.Open(HKEY_LOCAL_MACHINE, UNINSTALLREGKEY, FALSE) == ERROR_SUCCESS)
			{
				CString sUninstall;
				sUninstall.Format(_T("\"%s\" -%s"), FileMisc::GetAppFilePath(), SWITCH_UNINSTALL);

				reg.Write(_T("DisplayName"), CToDoListWnd::GetTitle(TRUE));			
				reg.Write(_T("NoModify"), 1);
				reg.Write(_T("NoRepair"), 1);
				reg.Write(_T("UninstallString"), sUninstall);
			}
		}
	}

	return TRUE;
}

BOOL CToDoListApp::SetPreferences(BOOL bIni, LPCTSTR szPrefs, BOOL bExisting)
{
	CString sExisting(bExisting ? _T("existing ") : _T(""));
	
	if (bIni)
	{
		FileMisc::LogText(_T("Using %sini file for preferences: %s"), sExisting, szPrefs);
		
		free((void*)m_pszProfileName);
		m_pszProfileName = _tcsdup(szPrefs);

		free((void*)m_pszRegistryKey);
		m_pszRegistryKey = NULL;

		// Try up the 10 times
		int nTry = 10;

		while (nTry--)
		{
			if (CPreferences::Initialise(szPrefs, TRUE))
				return TRUE;

			FileMisc::LogText(_T("Existing ini file is not readable (%d)"), (10 - nTry));
			Sleep(100);
		}

		return FALSE;
	}

	// else use Registry
	FileMisc::LogText(_T("Using %sregistry settings for preferences"), sExisting);
	SetRegistryKey(REGKEY);

	free((void*)m_pszProfileName);
	m_pszProfileName = NULL;
		
	return CPreferences::Initialise(APPREGKEY, FALSE);
}

BOOL CToDoListApp::InitTranslation(BOOL bFirstTime, BOOL bQuiet)
{
	CLocalizer::Release();

	if (!bFirstTime)
	{
		ASSERT(CPreferences::IsInitialised());

		m_sLanguageFile = CPreferences().GetProfileString(_T("Preferences"), _T("LanguageFile"));

		// language is stored as relative path
		if (!m_sLanguageFile.IsEmpty() && (m_sLanguageFile != CTDLLanguageComboBox::GetDefaultLanguage()))
		{
			FileMisc::MakeFullPath(m_sLanguageFile, FileMisc::GetAppFolder());
		}
	}

	// show language dialog if no language set
	if (m_sLanguageFile.IsEmpty())
	{
		if (bQuiet || !CTDLLanguageComboBox::HasLanguages())
		{
			m_sLanguageFile = CTDLLanguageComboBox::GetDefaultLanguage();
		}
		else
		{
			CTDLLanguageDlg dialog;

			if (dialog.DoModal() == IDCANCEL)
				return FALSE; // quit app

			// else
			CLocalizer::Release();
			m_sLanguageFile = dialog.GetLanguageFile();
		}
	}

	// Initialise language translation.
	BOOL bValidLocalizer = FALSE;

	if (FileMisc::FileExists(m_sLanguageFile))
		bValidLocalizer = CLocalizer::Initialize(m_sLanguageFile, ITTTO_TRANSLATEONLY);

	// If there is no need for translation, we initialise it to NULL
	// so that the dll won't continue to get loaded all the time
	if (!bValidLocalizer)
		CLocalizer::Initialize();
	
	return TRUE;
}

void CToDoListApp::UpgradePreferences(CPreferences& prefs, LPCTSTR szPrevVer)
{
	UNREFERENCED_PARAMETER(prefs);

	// we don't handle the registry because it's too hard (for now)
	if (!CPreferences::UsesIni())
		return;

	if (FileMisc::CompareVersions(szPrevVer, _T("7.2.10")) < 0)
	{
		// Rename 'Tasklists' resource folder to 'Examples'
		LPCTSTR szTasklists = _T(".\\Resources\\Tasklists\\");
		LPCTSTR szExamples = _T(".\\Resources\\Examples\\");

		for (int nFile = 1; nFile <= 16; nFile++)
		{
			CString sKey = Misc::Format(_T("TaskList%d"), nFile);
			CString sFile = prefs.GetProfileString(_T("MRU"), sKey);

			if (sFile.IsEmpty())
				break;

			// Only replace if at the start of the filepath
			if (Misc::Find(szTasklists, sFile, FALSE) == 0)
			{
				Misc::Replace(szTasklists, szExamples, sFile, FALSE);
				prefs.WriteProfileString(_T("MRU"), sKey, sFile);
			}
		}

		int nTDCCount = prefs.GetProfileInt(_T("Settings"), _T("NumLastFiles"), 0);

		for (int nTDC = 0; nTDC < nTDCCount; nTDC++)
		{
			CString sKey = Misc::MakeKey(_T("LastFile%d"), nTDC);
			CString sFile = prefs.GetProfileString(_T("Settings"), sKey);

			// Only replace if at the start of the filepath
			if (Misc::Find(szTasklists, sFile, FALSE) == 0)
			{
				Misc::Replace(szTasklists, szExamples, sFile, FALSE);
				prefs.WriteProfileString(_T("Settings"), sKey, sFile);
			}
		}
	}

	if (FileMisc::CompareVersions(szPrevVer, _T("8.0")) < 0)
	{
		// Each importer and exporter now gets its own preference section
		prefs.DeleteProfileSection(_T("Exporters"));
		prefs.DeleteProfileSection(_T("Importers"));
	}
}

int CToDoListApp::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT /*nIDPrompt*/) 
{
	HWND hwndMain = NULL;

	// make sure app window is visible
	if (m_pMainWnd)
	{
		hwndMain = *m_pMainWnd;

		if (::GetForegroundWindow() != hwndMain)
			m_pMainWnd->SendMessage(WM_TDL_SHOWWINDOW, 0, 0);
	}
	else
	{
		hwndMain = ::GetDesktopWindow();
	}
	
	CString sTitle(AfxGetAppName()), sInstruction, sText(lpszPrompt);
	CStringArray aPrompt;
	
	int nNumInputs = Misc::Split(lpszPrompt, aPrompt, '|');
	
	switch (nNumInputs)
	{
	case 0:
		ASSERT(0);
		break;
		
	case 1:
		// do nothing
		break;
		
	case 2:
		sInstruction = aPrompt[0];
		sText = aPrompt[1];
		break;
		
	case 3:
		sTitle = aPrompt[0];
		sInstruction = aPrompt[1];
		sText = aPrompt[2];
	}
	
	return CMessageBox::Show(hwndMain, sTitle, sInstruction, sText, nType);
}

void CToDoListApp::OnImportPrefs() 
{
	// default location is always app folder
	CString sIniPath(FileMisc::GetAppFilePath());
	FileMisc::ReplaceExtension(sIniPath, _T("ini"));
	
	// scope the prefs object so that no outstanding references
	// are held if we need to re-initialise it
	{
		CPreferences prefs;

		CFileOpenDialog dialog(IDS_IMPORTPREFS_TITLE, 
			_T("ini"), 
			sIniPath, 
			EOFN_DEFAULTOPEN, 
			CEnString(IDS_INIFILEFILTER));
		
		if (dialog.DoModal(prefs) != IDOK)
			return;

		// else
		sIniPath = dialog.GetPathName();
	}

	CRegKey2 reg;
		
	if ((reg.Open(HKEY_CURRENT_USER, APPREGKEY, FALSE) == ERROR_SUCCESS) &&
		 reg.ImportFromIni(sIniPath)) // => import ini to registry
	{
		// use them now?
		// only ask if we're not already using the registry
		BOOL bUsingIni = (m_pszRegistryKey == NULL);
		
		if (bUsingIni)
		{
			if (AfxMessageBox(CEnString(IDS_POSTIMPORTPREFS), MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				VERIFY(SetPreferences(FALSE, APPREGKEY, FALSE));
				
				// reset prefs
				m_pMainWnd->SendMessage(WM_TDL_REFRESHPREFS);
				
				// rename existing prefs file
				CString sNewName = (sIniPath + _T(".bak"));
				VERIFY (FileMisc::MoveFile(sIniPath, sNewName, TRUE, TRUE));
			}
		}
		else // reset prefs
		{
			m_pMainWnd->SendMessage(WM_TDL_REFRESHPREFS);
		}
	}
	else // notify user
	{
		CEnString sMessage(CEnString(IDS_INVALIDPREFFILE), sIniPath);
		AfxMessageBox(sMessage, MB_OK | MB_ICONEXCLAMATION);
	}
}

void CToDoListApp::OnUpdateImportPrefs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CToDoListApp::OnExportPrefs() 
{
	ASSERT (m_pszRegistryKey != NULL);

	CRegKey2 reg;

	if (reg.Open(HKEY_CURRENT_USER, APPREGKEY, TRUE) == ERROR_SUCCESS)
	{
		// default location is always app folder
		CString sAppPath = FileMisc::GetAppFilePath();

		CString sIniPath(sAppPath);
		sIniPath.MakeLower();
		sIniPath.Replace(_T("exe"), _T("ini"));
		
		// scope the prefs object so that no outstanding references
		// are held if we need to re-initialise it
		{
			CPreferences prefs;
			CFileSaveDialog dialog(IDS_IMPORTPREFS_TITLE, 
									_T("ini"), 
									sIniPath, 
									EOFN_DEFAULTSAVE, 
									CEnString(IDS_INIFILEFILTER));
			
			if (dialog.DoModal(prefs) != IDOK)
				return;

			// else
			sIniPath = dialog.GetPathName();
		}

		BOOL bUsingReg = (m_pszRegistryKey != NULL);

		if (bUsingReg && reg.ExportToIni(sIniPath, TRUE))
		{
			// use them now? 
			CString sDefaultIniPath;
			
			// Only if they have the right filename
			if (GetDefaultIniPath(sDefaultIniPath, FALSE) &&
				FileMisc::IsSamePath(sDefaultIniPath, sIniPath))
			{
				if (AfxMessageBox(CEnString(IDS_POSTEXPORTPREFS), MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					if (SetPreferences(TRUE, sIniPath, FALSE))
					{
						// reset prefs
						m_pMainWnd->SendMessage(WM_TDL_REFRESHPREFS);
					}
					else
					{
						// Notify the user
					}
				}
			}
		}
	}
}

void CToDoListApp::OnUpdateExportPrefs(CCmdUI* pCmdUI) 
{
	BOOL bUsingReg = (m_pszRegistryKey != NULL);
	pCmdUI->Enable(bUsingReg);
}

void CToDoListApp::OnHelpForum() 
{
	FileMisc::Run(*m_pMainWnd, FORUM_URL);
}

void CToDoListApp::OnHelpLicense() 
{
	FileMisc::Run(*m_pMainWnd, LICENSE_URL);
}

void CToDoListApp::OnHelpCommandline() 
{
	CTDLCmdlineOptionsDlg dialog;
	dialog.DoModal();
}

void CToDoListApp::OnHelpDonate() 
{
	FileMisc::Run(*m_pMainWnd, DONATE_URL);
}

int CToDoListApp::ExitInstance() 
{
	CLocalizer::Release();
	CPreferences::Release();

	CGdiPlus::Free();

	return CWinApp::ExitInstance();
}

void CToDoListApp::OnHelpUninstall() 
{
	// confirm uninstall first
	if (AfxMessageBox(CEnString(IDS_CONFIRM_UNINSTALL), MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		RunUninstaller();
	}
}

DWORD CToDoListApp::RunHelperApp(const CString& sAppName, UINT nIDGenErrorMsg, UINT nIDSmartScreenErrorMsg, BOOL bPreRelease, BOOL bTestDownload) const
{
	CEnCommandLineInfo params;

	// pass our app id to app 
	params.SetOption(SWITCH_APPID, TDLAPPID);

	// to handle UAC on Vista and above we use the "RunAs" verb
	LPCTSTR szVerb = ((COSVersion() >= OSV_VISTA) ? _T("runas") : NULL);

	// Pass the centroid of the main wnd so that the
	// updater appears in that same location
	if (m_pMainWnd && (GetSystemMetrics(SM_CMONITORS) > 1))
	{
		CRect rWindow;
		m_pMainWnd->GetWindowRect(rWindow);
		CPoint ptPos = rWindow.CenterPoint();

		params.SetOption(SWITCH_POSITION, MAKELPARAM(ptPos.x, ptPos.y));
	}
		
	// the helper app path
	CString sAppFolder = FileMisc::GetAppFolder();
	CString sAppPath = GetHelperAppPath(sAppName, bTestDownload);

#ifdef _DEBUG // ----------------------------------------------------
	if (bTestDownload)
		params.SetOption(SWITCH_TESTDOWNLOAD);
#else // ------------------------------------------------------------
	UNREFERENCED_PARAMETER(bTestDownload);

	// try to close all open instances of TDL
	if (!CloseAllToDoListWnds())
	{
		return 0; // user cancelled
	}
	
	// and the commandline we were started with
	// use base64 encoding to mangle it so that the update
	// doesn't try to interpret the commandline itself
	params.SetOption(SWITCH_CMDLINE, Base64Coder::Encode(m_lpCmdLine));
#endif // -----------------------------------------------------------
	
	// and the current language
	if (CRTLStyleMgr::IsRTL())
		params.SetOption(SWITCH_RTL);

	if (m_sLanguageFile != CTDLLanguageComboBox::GetDefaultLanguage())
	{
		CString sLangFile = FileMisc::GetFullPath(m_sLanguageFile, FileMisc::GetAppFolder());
		params.SetOption(SWITCH_LANG, sLangFile);
		
		if (CLocalizer::GetTranslationOption() == ITTTO_ADD2DICTIONARY)
		{
			//params.SetOption(SWITCH_ADDTODICT);

			// make sure all dictionary changes have been written
			CLocalizer::Release();
		}
	}

	// and whether this is a pre-prelease
	if (bPreRelease)
		params.SetOption(SWITCH_PRERELEASE);

	return RunHelperApp(sAppPath, params, nIDGenErrorMsg, nIDSmartScreenErrorMsg, szVerb);
}

DWORD CToDoListApp::RunHelperApp(LPCTSTR szAppPath, const CEnCommandLineInfo& params, UINT nIDGenErrorMsg, UINT nIDSmartScreenErrorMsg, LPCTSTR szVerb)
{
	DWORD dwRes = FileMisc::Run(NULL, 
								szAppPath, 
								params.GetCommandLine(), 
								SW_SHOWNORMAL,
								NULL, 
								szVerb);
	
	// error handling
	if (dwRes < SE_ERR_SUCCESS)
	{
		switch (dwRes)
		{
		case SE_ERR_ACCESSDENIED:
			// if this is windows 8 or above, assume 
			// this was blocked by SmartScreen
			if (COSVersion() >= OSV_WIN8)
			{
				AfxMessageBox(CEnString(nIDSmartScreenErrorMsg));
				break;
			}
			// else fall thru

		default:
			AfxMessageBox(CEnString(nIDGenErrorMsg));
			break;
		}
	}

	return dwRes;
}

CString CToDoListApp::GetHelperAppPath(const CString& sAppName, BOOL bTestDownload)
{
	CString sAppFolder = FileMisc::GetAppFolder();

#ifdef _DEBUG // -----------------------------------------------------------------------
	if (!bTestDownload)
	{
		// Copy ourselves to a temp location
		CString sTempFolder = (FileMisc::TerminatePath(sAppFolder) + _T("Debug") + sAppName);

		// sanity check
		VERIFY(FileMisc::DeleteFolder(sTempFolder, FMDF_SUBFOLDERS | FMDF_HIDDENREADONLY));

		// create folder and copy
		VERIFY(FileMisc::CreateFolder(sTempFolder));
		VERIFY(FileMisc::CopyFolder(sAppFolder, sTempFolder, _T("*.exe;*.dll;*.ini"), FMDF_HIDDENREADONLY));

		// set the temp folder as the app folder
		sAppFolder = sTempFolder;
	}
#else // ------------------------------------------------------------
	UNREFERENCED_PARAMETER(bTestDownload);
#endif // ------------------------------------------------------------------------------
	
	CString sAppPath;
	FileMisc::MakePath(sAppPath, NULL, sAppFolder, sAppName, _T("exe"));

	return sAppPath;
}

void CToDoListApp::RunUninstaller() const
{
	CString sAppPath = GetHelperAppPath(_T("TDLUninstall"));

	if (FileMisc::FileExists(sAppPath))
	{
		RunHelperApp(_T("TDLUninstall"), IDS_UNINSTALLER_RUNFAILURE, IDS_UNINSTALLER_SMARTSCREENBLOCK);
		return;
	}

	// Fallback
	CString sAppFolder = FileMisc::GetAppFolder();
	FileMisc::DeleteFolder(sAppFolder, FMDF_SUBFOLDERS | FMDF_HIDDENREADONLY | FMDF_ALLOWDELETEONREBOOT);
}

void CToDoListApp::RunUpdater(BOOL bPreRelease, BOOL bTestDownload) const
{
	CString sAppPath = GetHelperAppPath(_T("TDLUpdate"));

	if (FileMisc::FileExists(sAppPath))
	{
		RunHelperApp(_T("TDLUpdate"), IDS_UPDATER_RUNFAILURE, IDS_UPDATER_SMARTSCREENBLOCK, bPreRelease, bTestDownload);
		return;
	}

	// Fallback
	if (AfxMessageBox(CEnString(IDS_UPDATER_FILEMISSING), MB_YESNO) == IDYES)
	{
		FileMisc::Run(NULL, (bPreRelease ? DOWNLOAD_URI_PRERELEASE : DOWNLOAD_URI));
	}
}

BOOL CToDoListApp::CloseAllToDoListWnds()
{
	TDCFINDWND find(NULL, TRUE);
	int nNumWnds = CToDoListWnd::FindToDoListWnds(find);
	
	for (int nWnd = 0; nWnd < nNumWnds; nWnd++)
	{
		HWND hwndMain = find.aResults[nWnd];
		
		// final check for validity
		if (::IsWindow(hwndMain))
		{
			// close the application
			::SendMessage(hwndMain, WM_CLOSE, 0, 1);
		}
	}

	// check for user cancellation by seeing 
	// if any windows are still open
	return (CToDoListWnd::FindToDoListWnds(find) == 0);
}

BOOL CToDoListApp::WaitForInstanceToClose(DWORD dwProcID)
{
	int nTry = 10;

	while (nTry--)
	{
		HANDLE handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);

		if (!handle)
		{
			// no process having this ID so the
			// instance of TDL must have closed
			return TRUE;
		}

		CString sModule = FileMisc::GetProcessFilePath(handle);
		sModule.MakeUpper();

		if (sModule.Find(_T("TODOLIST.EXE")) == -1)
		{
			// the process ID has been reallocated
			// so the instance of TDL must have closed
			return TRUE;
		}

 		::CloseHandle(handle);

		// try again
		Sleep(200);
	}

	TRACE(_T("%d hasn't closed..."), dwProcID);

	return FALSE;
}

void CToDoListApp::OnHelpCheckForUpdates() 
{
	TDL_WEBUPDATE_CHECK nCheck = CheckForUpdates(TRUE);

	switch (nCheck)
	{
	case TDLWUC_WANTUPDATE:
	case TDLWUC_WANTPRERELEASEUPDATE:
		RunUpdater(nCheck == TDLWUC_WANTPRERELEASEUPDATE);
		break;

	case TDLWUC_CANCELLED:
		return;
		
	case TDLWUC_NOTCONNECTED:
		AfxMessageBox(CEnString(IDS_NO_WEBUPDATE_CONNECTION));
		break;

	case TDLWUC_NOUPDATES:
		AfxMessageBox(CEnString(IDS_NO_WEBUPDATE_AVAIL));
		break;

	case TDLWUC_FAILED:
	default:
		// TODO
		break;
	}
}

TDL_WEBUPDATE_CHECK CToDoListApp::CheckForUpdates(BOOL bManual)
{
//  FOR DEBUGGING
// 	return TDLWUC_WANTUPDATE;

	CPreferences prefs;

	// only auto-check once a day
	int nLastUpdate = prefs.GetProfileInt(_T("Updates"), _T("LastUpdate"), 0);
	int nToday = (int)CDateHelper::GetDate(DHD_TODAY);

	if (!bManual && (nLastUpdate >= nToday))
		return TDLWUC_NOUPDATES;

	prefs.WriteProfileInt(_T("Updates"), _T("LastUpdate"), nToday);

	// download the update script to temp file
	return CTDLWebUpdatePromptDlg::CheckForUpdates();
} 

/////////////////////////////////////////////////////////////////////////////
// DEBUG FUNCTIONS

#ifdef _DEBUG

void CToDoListApp::OnDebugTaskDialogInfo() 
{
	LPCTSTR szTestMsg = _T("This the optional caption of the message box|")
						_T("This the title of the Info message box|")
						_T("This is a paragraph of text with embedded carriage \n")
						_T("returns so that it looks ok on XP and below.\n\n")
						_T("This a second paragraph that ought to have a clear\n ")
						_T("line between it and the first para.");

	AfxMessageBox(szTestMsg, MB_ICONINFORMATION);
}

void CToDoListApp::OnDebugTaskDialogWarning() 
{
	LPCTSTR szTestMsg = _T("This the optional caption of the message box|")
						_T("This the title of the Warning message box|")
						_T("This is a paragraph of text with embedded carriage \n")
						_T("returns so that it looks ok on XP and below.\n\n")
						_T("This a second paragraph that ought to have a clear\n ")
						_T("line between it and the first para.");

	AfxMessageBox(szTestMsg, MB_ICONWARNING);
}

void CToDoListApp::OnDebugTaskDialogError() 
{
	LPCTSTR szTestMsg = _T("This the optional caption of the message box|")
						_T("This the title of the Error message box|")
						_T("This is a paragraph of text with embedded carriage \n")
						_T("returns so that it looks ok on XP and below.\n\n")
						_T("This a second paragraph that ought to have a clear\n ")
						_T("line between it and the first para.");
	
	AfxMessageBox(szTestMsg, MB_ICONERROR);
}

void CToDoListApp::OnDebugShowUpdateDlg() 
{
	CString sAppPath = (FileMisc::TerminatePath(FileMisc::GetAppFolder()) + _T("TDLUpdate.exe"));

	// pass our app id to app 
	CEnCommandLineInfo cmdLine;

	cmdLine.SetOption(SWITCH_APPID, TDLAPPID);
	cmdLine.SetOption(SWITCH_SHOWUI);

	// Pass the centroid of the main wnd so that the
	// updater appears in that same location
	if (GetSystemMetrics(SM_CMONITORS) > 1)
	{
		CRect rWindow;
		m_pMainWnd->GetWindowRect(rWindow);
		CPoint ptPos = rWindow.CenterPoint();

		cmdLine.SetOption(SWITCH_POSITION, MAKELPARAM(ptPos.x, ptPos.y));
	}

	DWORD dwRes = FileMisc::Run(NULL, sAppPath, cmdLine.GetCommandLine());
}

void CToDoListApp::OnDebugShowScriptDlg() 
{
	CTDLWebUpdatePromptDlg::ShowDialog();
}

void CToDoListApp::OnDebugShowLanguageDlg()
{
	CTDLLanguageDlg dialog;
	dialog.DoModal();
}


void CToDoListApp::OnDebugTestStableReleaseDownload() 
{
	RunUpdater(FALSE, TRUE);
}

void CToDoListApp::OnDebugTestPreReleaseDownload() 
{
	RunUpdater(TRUE, TRUE);
}

void CToDoListApp::OnDebugShowEmbeddedUrl() 
{
	CFileOpenDialog dialog(_T("Select Web Page"), 
						   NULL,
						   NULL,
						   EOFN_DEFAULTOPEN,
						   _T("Web Pages|*.html"));
	
	if (dialog.DoModal() == IDOK)
	{
		CBrowserDlg().DoModal(NULL, dialog.GetPathName());
	}
}

#endif // DEBUG FUNCTIONS
/////////////////////////////////////////////////////////////////////////////

void CToDoListApp::OnHelpRecordBugReport() 
{
	FileMisc::Run(*m_pMainWnd, _T("psr.exe"));
}

BOOL CToDoListApp::CommandRequiresUI(UINT nCmdID)
{
	CMenu mainMenu;
	mainMenu.LoadMenu(IDR_MAINFRAME);

	HMENU hSubMenu = NULL;
	int nPos = CEnMenu::FindMenuItem(mainMenu, nCmdID, hSubMenu);

	ASSERT((nPos != -1) && hSubMenu);

	// We want the top-level sub-menu
	while ((nPos != -1) && (hSubMenu != mainMenu))
		nPos = CEnMenu::FindMenuItem(mainMenu, hSubMenu, hSubMenu);

	ASSERT(nPos != -1);
	ASSERT(hSubMenu);

	switch (nPos)
	{
	case AM_FILE: 
		switch (nCmdID)
		{
			// These items DO NOT require UI
		case ID_FILE_NEW:
		case ID_OPEN_RELOAD:
		case ID_CLOSE:
		case ID_CLOSEALL:
		case ID_CLOSEALLBUTTHIS:
		case ID_EXIT:
			return FALSE;
		}
		// All the rest DO require UI
		return TRUE;

	case AM_NEWTASK: 
		switch (nCmdID)
		{
			// These items DO require UI
		case ID_NEWTASK_ATTOP:
		case ID_NEWTASK_ATBOTTOM:
		case ID_NEWTASK_ATTOPSELECTED:
		case ID_NEWTASK_ATBOTTOMSELECTED:
		case ID_NEWTASK_BEFORESELECTEDTASK:
		case ID_NEWTASK_AFTERSELECTEDTASK:
		case ID_NEWSUBTASK_ATTOP:
		case ID_NEWSUBTASK_ATBOTTOM:
			return TRUE;
		}
		// All the rest DO NOT require UI
		return FALSE;

	case AM_EDIT: 
		switch (nCmdID)
		{
			// These items DO require UI
		case ID_EDIT_TASKTEXT:
		case ID_EDIT_TASKCOLOR:
		case ID_EDIT_SETTASKICON:
		case ID_ADDTIMETOLOGFILE:
		case ID_EDIT_OFFSETDATES:
		case ID_EDIT_SETFILEREF:
		case ID_EDIT_RECURRENCE:
			return TRUE;
		}
		// All the rest DO NOT require UI
		return FALSE;

	case AM_VIEW: 
		switch (nCmdID)
		{
			// These items DO require UI
		case ID_TASKLIST_SELECTCOLUMNS:
		case ID_TASKLIST_CUSTOMCOLUMNS:
		case ID_VIEW_FILTER:
			return TRUE;
		}
		// All the rest DO NOT require UI
		return FALSE;

	case AM_MOVE: 
		// NO items require UI
		return FALSE;

	case AM_SORT: 
		switch (nCmdID)
		{
			// These items DO require UI
		case ID_SORTBY_MULTI:
			return TRUE;
		}
		// All the rest DO NOT require UI
		return FALSE;

	case AM_TOOLS: 
		switch (nCmdID)
		{
			// These items DO require UI
		case ID_TOOLS_IMPORT:
		case ID_TOOLS_EXPORT:
		case ID_TOOLS_TRANSFORM:
		case ID_TOOLS_ANALYSELOGGEDTIME:
		case ID_PREFERENCES:
		case ID_TOOLS_IMPORTPREFS:
		case ID_TOOLS_EXPORTPREFS:
		case ID_TOOLS_SHOWTASKS_DUETODAY:
		case ID_TOOLS_SHOWTASKS_DUETOMORROW:
		case ID_TOOLS_SHOWTASKS_DUEENDTHISWEEK:
		case ID_TOOLS_SHOWTASKS_DUEENDNEXTWEEK:
		case ID_TOOLS_SHOWTASKS_DUEENDTHISMONTH:
		case ID_TOOLS_SHOWTASKS_DUEENDNEXTMONTH:
			return TRUE;
		}
		// All the rest DO NOT require UI
		return FALSE;

	case AM_SRCCONTROL: 
		// NO items require UI
		return FALSE;

	case AM_WINDOW: 
		// NO items require UI
		return FALSE;

	case AM_HELP: 
		switch (nCmdID)
		{
			// These items DO require UI
		case ID_HELP_UNINSTALL:
		case ID_HELP_COMMANDLINE:
		case ID_HELP_KEYBOARDSHORTCUTS:
		case ID_HELP_RECORDBUGREPORT:
		case ID_TOOLS_CHECKFORUPDATES:
		case ID_ABOUT:
			return TRUE;
		}
		// All the rest DO NOT require UI
		return FALSE;

	case AM_DEBUG:
		return TRUE; // always
	}

	// all else
	ASSERT(0);
	return FALSE;
}

CString CToDoListApp::GetResourcePath(LPCTSTR szSubFolder, LPCTSTR szFile)
{
	CString sResource(_T("Resources"));

	if (!Misc::IsEmpty(szSubFolder))
	{
		sResource += '\\';
		sResource += szSubFolder;
	}

	sResource = FileMisc::GetAppResourceFolder(sResource);

	if (!Misc::IsEmpty(szFile))
	{
		sResource += '\\';
		sResource += szFile;
	}

	return sResource;
}

void CToDoListApp::CleanupAppFolder(LPCTSTR szPrevVer)
{
	CScopedLogTimer log(_T("CleanupAppFolder"));

	CString sAppFolder = FileMisc::GetAppFolder();
	CString sTasklists = FileMisc::GetAppResourceFolder(_T("Resources\\TaskLists"));

	if (FileMisc::CompareVersions(szPrevVer, _T("7.0")) < 0)
	{
		// remove old web updater
		FileMisc::DeleteFile(sAppFolder + _T("\\WebUpdateSvc.exe"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\WebUpdateSvc2.exe"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\WebUpdateSvc.log"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\WebUpdateSvc2.log"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\WebUpdateSvc.LIC"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\install.bmp"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\install.ico"), TRUE);

		// remove old components
		FileMisc::DeleteFile(sAppFolder + _T("\\GoogleDocsStorage.dll"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\ToodleDoStorage.dll"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\ToDoListLOC.dll"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\RTFContentCtrlLOC.dll"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\ChronicleWrap.dll"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\StatisticsExt.dll"), TRUE);
		FileMisc::DeleteFile(sAppFolder + _T("\\OutlookImpExp.dll"), TRUE);

		// gif translation 'flags' replaced with pngs
		CString sTranslations = FileMisc::GetAppResourceFolder(_T("Resources\\Translations"));
		FileMisc::DeleteFolderContents(sTranslations, FMDF_ALLOWDELETEONREBOOT | FMDF_HIDDENREADONLY, _T("*.gif"));

		// Wrongly installed resource files
		FileMisc::DeleteFileBySize((sTasklists + _T("\\Introduction.txt")), 395, TRUE);
		FileMisc::DeleteFileBySize((sTasklists + _T("\\Introduction.csv")), 10602, TRUE);
		FileMisc::DeleteFileBySize((sTasklists + _T("\\Introduction.xml")), 177520, TRUE);
	}

	if (FileMisc::CompareVersions(szPrevVer, _T("7.2.11")) < 0)
	{
		// remove experimental manifest
		FileMisc::DeleteFileBySize(sAppFolder + _T("\\ToDoList.exe.4K.manifest"), 1153, TRUE);

		// Rename 'Tasklists' resource folder
		CString sExamples = FileMisc::GetAppResourceFolder(_T("Resources\\Examples"));
		FileMisc::CreateFolder(sExamples);

		FileMisc::MoveFile(sTasklists + _T("\\Introduction.tdl"), sExamples + _T("\\Introduction.tdl"), TRUE, TRUE);
		FileMisc::MoveFile(sTasklists + _T("\\ToDoListDocumentation.tdl"), sExamples + _T("\\ToDoListDocumentation.tdl"), TRUE, TRUE);

		// Intentionally use raw API call so it will fail if any files remain in the folder
		RemoveDirectory(sTasklists);

		// Rename/move install instructions
		CString sResources = FileMisc::GetAppResourceFolder();
		FileMisc::DeleteFileBySize(sResources + _T("\\Install.txt"), 1795, TRUE);

		CString sReadmes = FileMisc::GetAppResourceFolder(_T("Resources\\ReadMe"));
		FileMisc::DeleteFileBySize(sReadmes + _T("\\Readme.Linux.txt"), 3260, TRUE);

		// Intentionally use raw API call so it will fail if any files remain in the folder
		RemoveDirectory(sReadmes);
	}
}

void CToDoListApp::FixupExampleTasklistsTaskDates(LPCTSTR szPrevVer)
{
	// Update task dates whenever the version changes
	CString sThisVer = FileMisc::GetAppVersion();

	if (FileMisc::CompareVersions(szPrevVer, sThisVer) < 0)
	{
		CScopedLogTimer log(_T("FixupExampleTasklistsTaskDates"));

		CString sExamples = FileMisc::TerminatePath(FileMisc::GetAppResourceFolder(_T("Resources\\Examples")));

		CStringArray aExamples;
		int nNumExamples = FileMisc::FindFiles(sExamples, aExamples, FALSE, _T("*.tdl"));

		for (int nFile = 0; nFile < nNumExamples; nFile++)
		{
			CTaskFile tasks;

			if (tasks.Load(aExamples[nFile]))
			{
				COleDateTime dtNow = COleDateTime::GetCurrentTime(), dtEarliest;

				if (tasks.GetEarliestTaskStartDate(dtEarliest))
				{
					int nOffset = (int)(dtNow.m_dt - dtEarliest.m_dt); // whole days
				
					if (tasks.OffsetDates(nOffset))
						tasks.Save(aExamples[nFile], SFEF_UTF16);
				}
			}
		}
	}
}
