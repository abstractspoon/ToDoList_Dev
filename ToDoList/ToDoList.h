// ToDoList.h : main header file for the TODOLIST application
//

#if !defined(AFX_TODOLIST_H__CA63D273_DB5E_4DBF_8915_1885E1987A65__INCLUDED_)
#define AFX_TODOLIST_H__CA63D273_DB5E_4DBF_8915_1885E1987A65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "tdcenum.h"

#include "..\shared\Localizer.h"

/////////////////////////////////////////////////////////////////////////////
// CToDoListApp:
// See ToDoList.cpp for the implementation of this class
//

class CEnCommandLineInfo;
class CPreferences;
class CTDCStartupOptions;

struct TDCFINDWND;

class CToDoListApp : public CWinApp
{
public:
	CToDoListApp();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToDoListApp)
protected:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

protected:
	CString m_sLanguageFile;

protected:
// Implementation

	//{{AFX_MSG(CToDoListApp)
	//}}AFX_MSG
	afx_msg void OnHelpGoogleGroup();
	afx_msg void OnHelpLicense();
	afx_msg void OnHelpCommandline();
	afx_msg void OnHelpDonate();
	afx_msg void OnHelpUninstall();
	afx_msg void OnDebugTaskDialogInfo();
	afx_msg void OnDebugShowUpdateDlg();
	afx_msg void OnDebugShowScriptDlg();
	afx_msg void OnDebugShowLanguageDlg();
	afx_msg void OnDebugTaskDialogWarning();
	afx_msg void OnDebugTaskDialogError();
	afx_msg void OnDebugShowReminderDlg();
	afx_msg void OnHelpCheckForUpdates();
	afx_msg void OnHelpRecordBugReport();
	afx_msg void OnHelpWiki();
	afx_msg void OnHelpFacebook();
	afx_msg void OnHelpTwitter();
	afx_msg void OnHelpGooglePlus();
	afx_msg void OnHelpLinkedIn();
	afx_msg void OnImportPrefs();
	afx_msg void OnUpdateImportPrefs(CCmdUI* pCmdUI);
	afx_msg void OnExportPrefs();
	afx_msg void OnUpdateExportPrefs(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

protected:
	void DoHelp(UINT nHelpID = 0);
	BOOL InitPreferences(CEnCommandLineInfo& cmdInfo);
	void SetPreferences(BOOL bIni, LPCTSTR szPrefs, BOOL bExisting);
	BOOL InitTranslation(CEnCommandLineInfo& cmdInfo, BOOL bFirstTime, BOOL bQuiet);
	void UpgradePreferences(CPreferences& prefs);
	void ParseCommandLine(CEnCommandLineInfo& cmdInfo);
	void RunUninstaller();
	void RunUpdater(BOOL bPreRelease);
	BOOL ProcessStartupOptions(CTDCStartupOptions& startup, const CEnCommandLineInfo& cmdInfo);
	
	TDL_WEBUPDATE_CHECK CheckForUpdates(BOOL bManual);

	DWORD RunHelperApp(const CString& sAppName, UINT nIDGenErrorMsg, UINT nIDSmartScreenErrorMsg, BOOL bPreRelease = FALSE);

	// our own local version
	CString AfxGetAppName();

	static BOOL CloseAllToDoListWnds();
	static int FindToDoListWnds(TDCFINDWND& find);
	static BOOL FormatEmailParams(CString& sParams);
	static BOOL ValidateFilePath(CString& sFilePath, const CString& sExt = _T(""));
	static BOOL ValidateTasklistPath(CString& sPath);
	static BOOL ValidateIniPath(CString& sIniPath, BOOL bCheckExists);
	static BOOL GetDefaultIniPath(CString& sIniPath, BOOL bCheckExists);
	static CString GetResourcePath(LPCTSTR szSubFolder = NULL, LPCTSTR szFile = NULL);
	static void CleanupAppFolder();

	static BOOL CALLBACK FindOtherInstance(HWND hwnd, LPARAM lParam);
	static BOOL SendStartupOptions(HWND hwnd, const CTDCStartupOptions& startup, TDL_COPYDATA nMsg);
	static BOOL WaitForInstanceToClose(DWORD dwProcessID);
	static BOOL CommandRequiresUI(UINT nCmdID);

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOLIST_H__CA63D273_DB5E_4DBF_8915_1885E1987A65__INCLUDED_)
