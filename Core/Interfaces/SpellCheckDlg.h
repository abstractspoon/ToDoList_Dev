#if !defined(AFX_SPELLCHECKDLG_H__796FCD48_463A_4291_9261_69EA5122ABA0__INCLUDED_)
#define AFX_SPELLCHECKDLG_H__796FCD48_463A_4291_9261_69EA5122ABA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpellCheckDlg.h : header file
//

#include "richeditspellcheck.h"

#include "..\Shared\runtimedlg.h"
#include "..\Shared\richeditncborder.h"
#include "..\Shared\RichEditBaseCtrl.h"
#include "..\Shared\FileComboBox.h"

#include "..\3rdparty\statlink.h"

/////////////////////////////////////////////////////////////////////////////

const int IDNOERRORS = 20; // can be returned from DoModal

/////////////////////////////////////////////////////////////////////////////

class ISpellChecker;
class ISpellCheck;
class IPreferences;

/////////////////////////////////////////////////////////////////////////////

enum // control IDs
{
	SCD_TITLE = 0,
	// don't put anything here else it will clash with IDOK, etc

	IDC_SCD_DICTIONARIES = 1001,
	IDC_SCD_DICTLABEL,
	IDC_SCD_BROWSE,
	IDC_SCD_DOWNLOADLABEL,
	IDC_SCD_CHECKINGLABEL,
	IDC_SCD_SUGGESTIONS,
	IDC_SCD_TEXT,
	IDC_SCD_MISSPELTWORD,
	IDC_SCD_REPLACELABEL,
	IDC_SCD_WITHLABEL,
	IDC_SCD_REPLACE,
	IDC_SCD_NEXT,
	IDC_SCD_RESTART,
	IDC_SCD_URL,

	DLG_SCD_BROWSETITLE = 10001,
	DLG_SCD_SETUPMSG,
	DLG_SCD_DICTFILTER,
	DLG_SCD_ENGINEFILTER,
	DLG_SCD_ENGINETITLE,
};

/////////////////////////////////////////////////////////////////////////////
// CSpellCheckDlg dialog

class CSpellCheckDlg : public CRuntimeDlg
{
public:
	CSpellCheckDlg(CWnd* pParent = NULL);
	virtual ~CSpellCheckDlg();

	int DoModal(IPreferences* pPrefs, BOOL bEndOnNoErrors = FALSE);

	void SetSpellCheck(ISpellCheck* pSpellCheck);
	void SetDictionaryDownloadUrl(const CString& sUrl);

	void SetText(const CString& sText);
	CString GetCorrectedText() { return m_sText; }

	BOOL MadeChanges() const { return m_bMadeChanges; }

	// for overiding the default text for translating
	static void SetItemText(UINT nIDItem, UINT nIDText);
	static void SetIcon(HICON hIcon) { s_hIconDlg = hIcon; }

protected:
	CString	m_sText;
	CString	m_sMisspeltWord;
	CString	m_sSuggestion;
	CString m_sEnginePath;
	CString m_sSelDictionary;
	BOOL m_bEndOnNoErrors;
	BOOL m_bMadeChanges;

	CRichEditBaseCtrl	m_reText;
	CListBox	m_lbSuggestions;
	CFileComboBox m_cbDictionaries;
    CStaticLink m_stURL;
	CRichEditNcBorder m_ncBorder;
	CRichEditSpellCheck m_reSpellCheck;

	ISpellChecker* m_pSpellChecker;
	ISpellCheck* m_pSpellCheck;
	IPreferences* m_pPrefs;

	mutable CPoint m_ptTopLeft;

	static CMap<UINT, UINT, CString, CString&> s_mapText;
	static HICON s_hIconDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnReplace();
	afx_msg void OnContinue();
	afx_msg void OnSelchangeSuggestions();
	afx_msg void OnRestart();
	afx_msg void OnDestroy();
	afx_msg void OnChangeDictionary();
	afx_msg void OnDblClkSuggestions();
	DECLARE_MESSAGE_MAP()

	enum CHECKFROM { CH_START, CH_CURRENT, CH_NEXT };

	BOOL IsInitialized() { return ((m_pSpellCheck || !m_sText.IsEmpty()) && m_pSpellChecker); }
	BOOL StartChecking(CHECKFROM nFrom = CH_START);
	BOOL FindNextWord(CString& sWord, CHECKFROM nFrom = CH_NEXT);
	BOOL FindNextMisspeltWord(CString& sWord, CHECKFROM nFrom = CH_NEXT);
	void ProcessMisspeltWord(const CString& sWord);
	BOOL IsWordMisspelt(const CString& sWord);
	void HighlightWord(BOOL bHighlight = TRUE);
	BOOL InitDictionary(const CString& sDict);
	void UpdateButtonStates();
	BOOL IsUsingExternalChecker() const { return (m_pSpellCheck && (m_pSpellCheck != &m_reSpellCheck)); }

	CString GetItemText(UINT nIDItem, const CString& sDefault = _T(""));

	virtual CPoint GetInitialPos() const;
	virtual CString OverrideItemText(UINT nIDItem) { return GetItemText(nIDItem); }
};

#endif // !defined(AFX_SPELLCHECKDLG_H__796FCD48_463A_4291_9261_69EA5122ABA0__INCLUDED_)
