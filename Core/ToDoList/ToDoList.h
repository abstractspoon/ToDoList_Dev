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
#include "..\shared\Icon.h"

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
	~CToDoListApp();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToDoListApp)
protected:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt = 0);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

protected:
	CString m_sLanguageFile;
	CIcon m_iconBrowse, m_iconGo, m_iconHelp;

protected:
// Implementation

	//{{AFX_MSG(CToDoListApp)
	//}}AFX_MSG
	afx_msg void OnHelpForum();
	afx_msg void OnHelpLicense();
	afx_msg void OnHelpCommandline();
	afx_msg void OnHelpDonate();
	afx_msg void OnHelpUninstall();
#ifdef _DEBUG
	afx_msg void OnDebugTaskDialogInfo();
	afx_msg void OnDebugShowUpdateDlg();
	afx_msg void OnDebugShowScriptDlg();
	afx_msg void OnDebugShowLanguageDlg();
	afx_msg void OnDebugTaskDialogWarning();
	afx_msg void OnDebugTaskDialogError();
	afx_msg void OnDebugTestStableReleaseDownload();
	afx_msg void OnDebugTestPreReleaseDownload();
	afx_msg void OnDebugShowEmbeddedUrl();
	afx_msg void OnDebugFormatGetLastError();
#endif
	afx_msg void OnHelpCheckForUpdates();
	afx_msg void OnHelpRecordBugReport();
	afx_msg void OnHelpWiki();
	afx_msg void OnImportPrefs();
	afx_msg void OnUpdateImportPrefs(CCmdUI* pCmdUI);
	afx_msg void OnExportPrefs();
	afx_msg void OnUpdateExportPrefs(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

protected:
	void DoHelp(UINT nHelpID = 0);
	BOOL InitPreferences(CEnCommandLineInfo& cmdInfo);
	BOOL SetPreferences(BOOL bIni, LPCTSTR szPrefs, BOOL bExisting);
	BOOL InitTranslation(BOOL bFirstTime, BOOL bQuiet);
	void UpgradePreferences(CPreferences& prefs, LPCTSTR szPrevVer);
	void ParseCommandLine(CEnCommandLineInfo& cmdInfo);
	void RunUninstaller() const;
	void RunUpdater(BOOL bPreRelease, BOOL bTestDownload = FALSE) const;
	BOOL ProcessStartupOptions(CTDCStartupOptions& startup, const CEnCommandLineInfo& cmdInfo) const;
	BOOL HandleSimpleQueries(const CEnCommandLineInfo& cmdInfo);

	TDL_WEBUPDATE_CHECK CheckForUpdates(BOOL bManual);

	DWORD RunHelperApp(const CString& sAppName, UINT nIDGenErrorMsg, UINT nIDSmartScreenErrorMsg, 
						BOOL bPreRelease = FALSE, BOOL bTestDownload = FALSE) const;

	// our own local version
	CString AfxGetAppName();

	static BOOL CloseAllToDoListWnds();
	static BOOL ValidateFilePath(CString& sFilePath, const CString& sExt = _T(""));
	static BOOL ValidateTasklistPath(CString& sPath);
	static BOOL ValidateIniPath(CString& sIniPath, BOOL bCheckExists);
	static BOOL GetDefaultIniPath(CString& sIniPath, BOOL bCheckExists);
	static void CleanupAppFolder(LPCTSTR szPrevVer);
	static void FixupExampleTasklistsTaskDates(LPCTSTR szPrevVer);

	static BOOL SendStartupOptions(HWND hwnd, const CTDCStartupOptions& startup, TDL_COPYDATA nMsg);
	static BOOL WaitForInstanceToClose(DWORD dwProcessID);
	static BOOL CommandRequiresUI(UINT nCmdID);
	static BOOL HasVSRedistributable();

	static CString GetResourcePath(LPCTSTR szSubFolder = NULL, LPCTSTR szFile = NULL);
	static CString GetHelperAppPath(const CString& sAppName, BOOL bTestDownload = FALSE);
	static DWORD RunHelperApp(LPCTSTR szAppName, const CEnCommandLineInfo& params, UINT nIDGenErrorMsg, UINT nIDSmartScreenErrorMsg, LPCTSTR szVerb);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOLIST_H__CA63D273_DB5E_4DBF_8915_1885E1987A65__INCLUDED_)
