// EasyFtp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EasyFtp.h"

#include "..\shared\FileMisc.h"
#include "..\shared\GraphicsMisc.h"
#include "..\shared\remotefile.h"
#include "..\shared\encommandlineinfo.h"

#include "..\Interfaces\Preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEasyFtpApp

BEGIN_MESSAGE_MAP(CEasyFtpApp, CWinApp)
	//{{AFX_MSG_MAP(CEasyFtpApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyFtpApp construction

CEasyFtpApp::CEasyFtpApp()
{
	GraphicsMisc::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEasyFtpApp object

CEasyFtpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEasyFtpApp initialization

BOOL CEasyFtpApp::InitInstance()
{
	// Standard initialization
	CPreferences::Initialise(_T("Software\\AbstractSpoon\\EasyFTP"), FALSE);

	// parse command line
	CEnCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	CString sRemotePath, sLocalPath;
	CString sAgent, sServer, sUsername, sPassword;

	BOOL bUpload = cmdInfo.HasOption(_T("up"));
	BOOL bNoLowercase = cmdInfo.HasOption(_T("nl"));
	BOOL bRepeat = cmdInfo.HasOption(_T("rt"));
	BOOL bAnonymousLogin = cmdInfo.HasOption(_T("an"));
	BOOL bNoConfirmOverwrite = cmdInfo.HasOption(_T("nc"));

	cmdInfo.GetOption(_T("rp"), sRemotePath);
	cmdInfo.GetOption(_T("lp"), sLocalPath);
	cmdInfo.GetOption(_T("ag"), sAgent);
	cmdInfo.GetOption(_T("sv"), sServer);
	cmdInfo.GetOption(_T("us"), sUsername);
	cmdInfo.GetOption(_T("pw"), sPassword);

	if (sLocalPath.IsEmpty() && !cmdInfo.m_strFileName.IsEmpty())
	{
		if (FileMisc::ResolveShortcut(cmdInfo.m_strFileName, sLocalPath))
		{
			bUpload = TRUE; // what else could it be?
			bRepeat = FALSE;
		}
	}

	CRemoteFile rf(sAgent, sServer, sUsername, sPassword);

	if (bUpload)
	{
		DWORD dwOptions = RMO_PROMPTFORFILE | RMO_DELETEFAILURES | RMO_MULTISELECT;
		
		if (!bNoLowercase)
			dwOptions |= RMO_LOWERCASEPATHS;

		if (bAnonymousLogin)
			dwOptions |= RMO_ANONYMOUSLOGIN;

		if (!bNoConfirmOverwrite)
			dwOptions |= RMO_CONFIRMOVERWRITE;

		RMERR nErr = RMERR_SUCCESS;
		CPreferences prefs;

		do
		{
			nErr = rf.SetFile(sLocalPath, sRemotePath, prefs, _T(""), dwOptions);

			switch (nErr)
			{
			case RMERR_SUCCESS:
			case RMERR_USERCANCELLED:
				break;

			default:
				{
					CString sMessage;

					if (sLocalPath.IsEmpty())
						sMessage.Format(_T("Sorry, the requested upload to '%s' could not be completed \nfor the following reason:\n\n%s"),
										sServer, rf.GetLastError());
					else
						sMessage.Format(_T("Sorry, the upload of '%s' to '%s' \ncould not be completed for the following reason:\n\n%s"),
										sLocalPath, sServer, rf.GetLastError());

					AfxMessageBox(sMessage, MB_OK | MB_ICONEXCLAMATION);
				}
				break;
			}

			sLocalPath.Empty();
			sRemotePath.Empty();
		}
		while (bRepeat && nErr != RMERR_USERCANCELLED);
	}
	else
	{
		DWORD dwOptions = RMO_PROMPTFORFILE | RMO_CREATEDOWNLOADDIR | RMO_MULTISELECT;
		
		if (bAnonymousLogin)
			dwOptions |= RMO_ANONYMOUSLOGIN;

		if (!bNoConfirmOverwrite)
			dwOptions |= RMO_CONFIRMOVERWRITE;

		RMERR nErr = RMERR_SUCCESS;
		CPreferences prefs;

		do
		{
			nErr = rf.GetFile(sRemotePath, sLocalPath, prefs, _T("EasyFTP"), dwOptions);

			switch (nErr)
			{
			case RMERR_SUCCESS:
#ifdef _DEBUG
				ShellExecute(NULL, NULL, sLocalPath, NULL, NULL, SW_SHOWNORMAL);
#endif
				break;
				
			case RMERR_USERCANCELLED:
				break;
				
			default:
				{
					CString sMessage;
					
					if (sRemotePath.IsEmpty())
						sMessage.Format(_T("Sorry, the requested download from '%s' could not be completed \nfor the following reason:\n\n%s"),
										sServer, rf.GetLastError());
					else
						sMessage.Format(_T("Sorry, the download of '%s' from '%s' \ncould not be completed for the following reason:\n\n%s"),
										sRemotePath, sServer, rf.GetLastError());

					AfxMessageBox(sMessage, MB_OK | MB_ICONEXCLAMATION);
				}
			}

			sLocalPath.Empty();
			sRemotePath.Empty();
		}
		while (bRepeat && nErr != RMERR_USERCANCELLED);
	}

	// done
	return FALSE;
}
