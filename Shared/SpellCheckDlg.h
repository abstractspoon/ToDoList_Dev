#if !defined(AFX_SPELLCHECKDLG_H__796FCD48_463A_4291_9261_69EA5122ABA0__INCLUDED_)
#define AFX_SPELLCHECKDLG_H__796FCD48_463A_4291_9261_69EA5122ABA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpellCheckDlg.h : header file
//

#include "runtimedlg.h"
#include "richeditncborder.h"
#include "richeditspellcheck.h"
#include "RichEditBaseCtrl.h"

#include "..\3rdparty\statlink.h"

/////////////////////////////////////////////////////////////////////////////
// CSpellCheckDlg dialog

const int IDNOERRORS = 20; // can be returned from DoModal

class ISpellChecker;
class ISpellCheck;

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

class CSpellCheckDlg : public CRuntimeDlg
{
// Construction
public:
	CSpellCheckDlg(const CString& sDictionaryPath = _T(""), ISpellCheck* pSpellCheck = NULL, const CString& sText = _T(""), CWnd* pParent = NULL);   // standard constructor
	virtual ~CSpellCheckDlg();

	BOOL IsInitialized() { return ((m_pSpellCheck || !m_sText.IsEmpty()) && m_pSpellChecker); }
	int DoModal(BOOL bEndOnNoErrors = FALSE);

	void SetSpellCheck(ISpellCheck* pSpellCheck);
	void SetDictionaryDownloadUrl(const CString& sUrl);

	void SetText(const CString& sText);
	CString GetCorrectedText() { return m_sText; }

	BOOL MadeChanges() const { return m_bMadeChanges; }

	// for overiding the default text for translating
	static void SetItemText(UINT nIDItem, UINT nIDText);

protected:
// Dialog Data
	//{{AFX_DATA(CSpellCheckDlg)
	CRichEditBaseCtrl	m_reText;
	CListBox	m_lbSuggestions;
	CString	m_sText;
	CString	m_sMisspeltWord;
	CString	m_sSuggestion;
	//}}AFX_DATA
	ISpellChecker* m_pSpellChecker;
	ISpellCheck* m_pSpellCheck;
	CString m_sEnginePath;
	CComboBox m_cbDictionaries;
	CString m_sSelDictionary;
	BOOL m_bEndOnNoErrors;
    CStaticLink m_stURL;
	CRichEditNcBorder m_ncBorder;
	CRichEditSpellCheck m_reSpellCheck;
	BOOL m_bMadeChanges;
	mutable CPoint m_ptTopLeft;

	static CMap<UINT, UINT, CString, CString&> s_mapText;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpellCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpellCheckDlg)
	afx_msg void OnReplace();
	afx_msg void OnContinue();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSuggestions();
	afx_msg void OnRestart();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnBrowse();
	afx_msg void OnChangeDictionary();
	afx_msg void OnDblClkSuggestions();
	DECLARE_MESSAGE_MAP()

	enum CHECKFROM { CH_START, CH_CURRENT, CH_NEXT };

	BOOL StartChecking(CHECKFROM nFrom = CH_START);
	BOOL FindNextWord(CString& sWord, CHECKFROM nFrom = CH_NEXT);
	BOOL FindNextMisspeltWord(CString& sWord, CHECKFROM nFrom = CH_NEXT);
	void ProcessMisspeltWord(const CString& sWord);
	BOOL IsWordMisspelt(const CString& sWord);
	void HighlightWord(BOOL bHighlight = TRUE);
	BOOL InitDictionary(const CString& sDict);
	void UpdateButtonStates();

	void InitDialog(ISpellCheck* pSpellCheck, const CString& sText);
	CString GetItemText(UINT nIDItem, const CString& sDefault = _T(""));

	virtual CPoint GetInitialPos() const;
	virtual CString OverrideItemText(UINT nIDItem) { return GetItemText(nIDItem); }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPELLCHECKDLG_H__796FCD48_463A_4291_9261_69EA5122ABA0__INCLUDED_)
