#if !defined(AFX_RTFPREFERENCESDLG_H__F70FEFCC_D6D8_40D7_8FB3_ACE6CD2DEEDB__INCLUDED_)
#define AFX_RTFPREFERENCESDLG_H__F70FEFCC_D6D8_40D7_8FB3_ACE6CD2DEEDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateFileLinkDlg.h : header file
//

#include "CreateFileLinkDlg.h"

#include "..\shared\GroupLine.h"
#include "..\shared\preferencesbase.h"

/////////////////////////////////////////////////////////////////////////////

// WPARAM = , LPARAM = 
const UINT WM_RTF_PREFSHELP			= ::RegisterWindowMessage(_T("WM_RTF_PREFSHELP"));

/////////////////////////////////////////////////////////////////////////////
// CRTFPreferencesDlg dialog

class CRTFPreferencesPage : public CPreferencesPageBase
{
// Construction
public:
	CRTFPreferencesPage();

	void SetFileLinkOption(RE_PASTE nLinkOption, BOOL bPrompt);
	RE_PASTE GetLinkOption() const { return (RE_PASTE)m_nLinkOption; }
	BOOL GetPromptForFileLink() const { return m_bPromptForFileLink; }

	// global settings
	void SetConvertWithMSWord(BOOL bUseMSWord) { m_bConvertWithMSWord = bUseMSWord; }
	BOOL GetConvertWithMSWord() { return m_bConvertWithMSWord; }

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
// Dialog Data
	//{{AFX_DATA(CCreateFileLinkDlg)
	//}}AFX_DATA
	CGroupLineManager m_groupLine;

	int m_nLinkOption;
	BOOL m_bPromptForFileLink;

	// global setting
	BOOL m_bConvertWithMSWord;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateFileLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateFileLinkDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnPromptForLink();
	DECLARE_MESSAGE_MAP()
};

class CRTFPreferencesDlg : public CPreferencesDlgBase
{
// Construction
public:
	CRTFPreferencesDlg(CWnd* pParent = NULL);

	int DoModal(BOOL bUseMSWord);
	
	void SetFileLinkOption(RE_PASTE nLinkOption, BOOL bPrompt) { m_page.SetFileLinkOption(nLinkOption, bPrompt); }
	RE_PASTE GetFileLinkOption() const { return m_page.GetLinkOption(); }
	BOOL GetPromptForFileLink() const { return m_page.GetPromptForFileLink(); }
	BOOL GetConvertWithMSWord() { return m_page.GetConvertWithMSWord(); }

protected:
// Dialog Data
	//{{AFX_DATA(CCreateFileLinkDlg)
	//}}AFX_DATA
	CRTFPreferencesPage m_page;

protected:
// Overrides
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void DoHelp();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTFPREFERENCESDLG_H__F70FEFCC_D6D8_40D7_8FB3_ACE6CD2DEEDB__INCLUDED_)
