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
public:
	CRTFPreferencesPage();

	void SetFileLinkOption(RE_PASTE nLinkOption, BOOL bPrompt, BOOL bReduceImageColors);
	RE_PASTE GetLinkOption() const { return (RE_PASTE)m_nLinkOption; }
	BOOL GetPromptForFileLink() const { return m_bPromptForFileLink; }
	BOOL GetReduceImageColors() const { return m_bReduceImageColors; }

	// global settings
	void SetConvertWithMSWord(BOOL bUseMSWord) { m_bConvertWithMSWord = bUseMSWord; }
	BOOL GetConvertWithMSWord() { return m_bConvertWithMSWord; }

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CGroupLineManager m_groupLine;

	int m_nLinkOption;
	BOOL m_bPromptForFileLink;
	BOOL m_bReduceImageColors;

	// global setting
	BOOL m_bConvertWithMSWord;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnClickPromptForLink();
	afx_msg void OnChangeLinkOption();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////

class CRTFPreferencesDlg : public CPreferencesDlgBase
{
public:
	CRTFPreferencesDlg(CWnd* pParent = NULL);

	int DoModal(BOOL bUseMSWord);
	
	void SetFileLinkOption(RE_PASTE nLinkOption, BOOL bPrompt, BOOL bReduceImageColors);
	RE_PASTE GetFileLinkOption() const { return m_page.GetLinkOption(); }
	BOOL GetPromptForFileLink() const { return m_page.GetPromptForFileLink(); }
	BOOL GetReduceImageColors() const { return m_page.GetReduceImageColors(); }
	BOOL GetConvertWithMSWord() { return m_page.GetConvertWithMSWord(); }

protected:
	CRTFPreferencesPage m_page;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void DoHelp();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RTFPREFERENCESDLG_H__F70FEFCC_D6D8_40D7_8FB3_ACE6CD2DEEDB__INCLUDED_)
