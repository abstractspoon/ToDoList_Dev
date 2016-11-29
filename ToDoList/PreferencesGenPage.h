#if !defined(AFX_PREFERENCESGENPAGE_H__7A8CC153_F3FB_4FBA_8EB9_B8ADF0A59982__INCLUDED_)
#define AFX_PREFERENCESGENPAGE_H__7A8CC153_F3FB_4FBA_8EB9_B8ADF0A59982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesGenPage.h : header file
//

#include "TDLLanguageComboBox.h"

#include "..\shared\hotkeyctrlex.h"
#include "..\shared\preferencesbase.h"
#include "..\shared\fileedit.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_PGP_CLEARMRU			= ::RegisterWindowMessage(_T("WM_PGP_CLEARMRU"));

/////////////////////////////////////////////////////////////////////////////

enum // tray options
{
	STO_NONE = -1,
	STO_ONMINIMIZE,
	STO_ONCLOSE,
	STO_ONMINCLOSE,
};

/////////////////////////////////////////////////////////////////////////////
// CPreferencesGenPage dialog

class CPreferencesGenPage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesGenPage)

// Construction
public:
	CPreferencesGenPage();
	~CPreferencesGenPage();

	BOOL GetAlwaysOnTop() const { return m_bAlwaysOnTop; }
	BOOL GetUseSysTray() const { return m_bUseSysTray; }
	int GetSysTrayOption() const { return !m_bUseSysTray ? STO_NONE : m_nSysTrayOption; }
	BOOL GetConfirmDelete() const { return m_bConfirmDelete; }
	BOOL GetConfirmSaveOnExit() const { return m_bConfirmSaveOnExit; }
	BOOL GetShowOnStartup() const { return m_bShowOnStartup; }
	BOOL GetMultiInstance() const { return m_bMultiInstance; }
	BOOL GetToggleTrayVisibility() const { return m_bToggleTrayVisibility; }
	BOOL GetGlobalHotkey() const { return m_bSpecifyGlobalHotkey ? m_dwGlobalHotkey : 0; }
	BOOL GetAddFilesToMRU() const { return m_bAddFilesToMRU; }
	BOOL GetEnableTDLExtension() const { return m_bEnableTDLExtension; }
	BOOL GetAutoCheckForUpdates() const { return m_bAutoCheckForUpdates; }
	BOOL GetEscapeMinimizes() const { return m_bEscapeMinimizes; }
	BOOL GetEnableTDLProtocol() const { return m_bEnableTDLProtocol; }
	BOOL GetEnableDelayedLoading() const { return m_bEnableDelayedLoading; }
	CString GetLanguageFile() const { return m_cbLanguages.GetSelectedLanguageFile(); }
	BOOL GetSaveStoragePasswords() const { return m_bSaveStoragePasswords; }
	UINT GetAutoMinimizeFrequency() const { return (m_bMinimizeNoChange ? m_nMinimizeNoEditTime : 0); }
	BOOL GetUseStickies(CString& sStickiesPath) const;
	BOOL GetReloadTasklists() const { return m_bReloadTasklists; }
	BOOL GetEnableRTLInput() const { return m_bEnableRTLInput; }
//	BOOL Get() const { return m_b; }

// Dialog Data
	//{{AFX_DATA(CPreferencesGenPage)
	enum { IDD = IDD_PREFGEN_PAGE };
	CComboBox	m_cbNoEditTime;
	CTDLLanguageComboBox	m_cbLanguages;
	CStatic	m_stVistaShield;
	CHotKeyCtrlEx	m_hkGlobal;
	BOOL	m_bAlwaysOnTop;
	BOOL	m_bUseSysTray;
	BOOL	m_bConfirmDelete;
	BOOL	m_bConfirmSaveOnExit;
	BOOL	m_bMultiInstance;
	BOOL	m_bShowOnStartup;
	int		m_nSysTrayOption;
	BOOL	m_bToggleTrayVisibility;
	BOOL	m_bSpecifyGlobalHotkey;
	BOOL	m_bAddFilesToMRU;
	BOOL	m_bEnableTDLExtension;
	BOOL	m_bAutoCheckForUpdates;
	BOOL	m_bEscapeMinimizes;
	BOOL	m_bEnableTDLProtocol;
	BOOL	m_bEnableDelayedLoading;
	BOOL	m_bSaveStoragePasswords;
	BOOL	m_bMinimizeNoChange;
	BOOL	m_bUseStickies;
	CString	m_sStickiesPath;
	BOOL	m_bReloadTasklists;
	BOOL	m_bEnableRTLInput;
	//}}AFX_DATA
	CFileEdit	m_eStickiesPath;
	DWORD	m_dwGlobalHotkey;
	UINT    m_nMinimizeNoEditTime;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesGenPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesGenPage)
	afx_msg void OnUseSystray();
	virtual BOOL OnInitDialog();
	afx_msg void OnSpecifyglobalhotkey();
	afx_msg void OnMultiinstance();
	afx_msg void OnClearMRU();
	afx_msg void OnSelchangeLanguage();
	afx_msg void OnMinimizeonnoedit();
	afx_msg void OnUseStickies();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	
	void EnableDisableLanguageOptions();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESGENPAGE_H__7A8CC153_F3FB_4FBA_8EB9_B8ADF0A59982__INCLUDED_)
