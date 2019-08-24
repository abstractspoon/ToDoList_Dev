// TDLTransEdit.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TDLTransEdit.h"
#include "TDLTransEditDlg.h"

#include "..\shared\filemisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLTransEditApp

BEGIN_MESSAGE_MAP(CTDLTransEditApp, CWinApp)
	//{{AFX_MSG_MAP(CTDLTransEditApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTransEditApp construction

CTDLTransEditApp::CTDLTransEditApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTDLTransEditApp object

CTDLTransEditApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTDLTransEditApp initialization

BOOL CTDLTransEditApp::InitInstance()
{
	// 'ToDoList.exe' must also exist in the same folder
	CString sTDLPath(FileMisc::GetAppFilePath()), sTDLVer;
	sTDLPath.Replace(FileMisc::GetFileNameFromPath(sTDLPath), _T("ToDoList.exe"));

#ifndef _DEBUG
	if (!FileMisc::FileExists(sTDLPath))
	{
		AfxMessageBox(_T("This application can only be run if ToDoList.exe is present in the same folder."), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	sTDLVer = FileMisc::GetModuleVersion(sTDLPath);
#else
	sTDLVer = _T("999.0.0.0");
#endif
	
	CString sIniPath(FileMisc::GetAppFilePath());
	FileMisc::ReplaceExtension(sIniPath, _T(".ini"));

	m_pszProfileName = _tcsdup(sIniPath);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	
	CTDLTransEditDlg dlg(sTDLVer, cmdInfo.m_strFileName);
	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
