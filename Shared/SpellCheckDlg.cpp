// SpellCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpellCheckDlg.h"
#include "enfiledialog.h"
#include "stringres.h"
#include "filemisc.h"
#include "preferences.h"
#include "richedithelper.h"
#include "enstring.h"
#include "localizer.h"

#include "..\Interfaces\ispellcheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpellCheckDlg dialog

CMap<UINT, UINT, CString, CString&> CSpellCheckDlg::s_mapText;

/////////////////////////////////////////////////////////////////////////////

CSpellCheckDlg::CSpellCheckDlg(const CString& sDictionaryPath, ISpellCheck* pSpellCheck, const CString& sText, CWnd* /*pParent*/) : 
	m_pSpellChecker(NULL), 
	m_reSpellCheck(m_reText),
    m_sSelDictionary(sDictionaryPath), 
	m_stURL(_T("http://wiki.services.openoffice.org/wiki/Dictionaries")),
	m_bMadeChanges(FALSE),
	m_ptTopLeft(-1, -1)
{
	InitDialog(pSpellCheck, sText);

	if (!m_sSelDictionary.IsEmpty())
		FileMisc::MakeRelativePath(m_sSelDictionary, FileMisc::GetAppFolder(), FALSE);
}

void CSpellCheckDlg::InitDialog(ISpellCheck* pSpellCheck, const CString& sText)
{
	AfxEnableControlContainer();
	CRichEditHelper::InitRichEdit();

	AddRCControl(_T("LTEXT"), _T(""), _T("Ac&tive Dictionary:"), 0,0, 7,9,70,8, IDC_SCD_DICTLABEL);
	AddRCControl(_T("COMBOBOX"), _T(""), _T(""), CBS_DROPDOWNLIST | WS_TABSTOP, 0, 71, 8, 182,100, IDC_SCD_DICTIONARIES);
	AddRCControl(_T("PUSHBUTTON"), _T(""), _T("Bro&wse"),WS_TABSTOP, 0,256,7,50,14,IDC_SCD_BROWSE);
	AddRCControl(_T("LTEXT"), _T(""), _T("Download More Dictionaries"), WS_TABSTOP | SS_RIGHT,0, 66,21,182,8,IDC_SCD_URL);
	AddRCControl(_T("LTEXT"), _T(""), _T("C&hecking Text:"), 0,0, 7,30,149,8, IDC_SCD_CHECKINGLABEL);
	AddRCControl(_T("CONTROL"), _T("RICHEDIT"), _T(""),ES_MULTILINE | ES_AUTOVSCROLL | ES_NOHIDESEL | ES_READONLY | WS_VSCROLL | WS_TABSTOP,0, 7,40,242,68,IDC_SCD_TEXT);
	AddRCControl(_T("PUSHBUTTON"), _T(""), _T("R&estart"),WS_TABSTOP, 0,256,40,50,14,IDC_SCD_RESTART);
	AddRCControl(_T("LTEXT"), _T(""), _T("Replace:"),0, 0,7,112,30,8, IDC_SCD_REPLACELABEL);
	AddRCControl(_T("LTEXT"), _T(""), _T("Static"),0, 0,44,112,205,8,IDC_SCD_MISSPELTWORD);
	AddRCControl(_T("LTEXT"), _T(""), _T("&With:"),0, 0,7,124,18,8, IDC_SCD_WITHLABEL);
	AddRCControl(_T("LISTBOX"), _T(""), _T(""), LBS_SORT | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_TABSTOP | LBS_NOTIFY, 0,41,124,208,51, IDC_SCD_SUGGESTIONS);
	AddRCControl(_T("PUSHBUTTON"), _T(""), _T("&Replace"),WS_TABSTOP, 0,256,124,50,14,IDC_SCD_REPLACE);
	AddRCControl(_T("PUSHBUTTON"), _T(""), _T("&Next Word"),WS_TABSTOP, 0,256,144,50,14,IDC_SCD_NEXT);
	AddRCControl(_T("CONTROL"), _T("static"), _T(""),SS_ETCHEDHORZ,0,7,182,299,1, (UINT)IDC_STATIC);

	if (!pSpellCheck)
	{
		AddRCControl(_T("DEFPUSHBUTTON"), _T(""), BTN_OK, WS_TABSTOP, 0, 201,190,50,14,IDOK);
		AddRCControl(_T("PUSHBUTTON"), _T(""), BTN_CANCEL, WS_TABSTOP, 0,256,190,50,14,IDCANCEL);

		SetText(sText);
	}
	else
	{
		AddRCControl(_T("PUSHBUTTON"), _T(""), BTN_CLOSE, WS_TABSTOP, 0,256,190,50,14,IDCANCEL);

		SetSpellCheck(pSpellCheck);
	}
}

CSpellCheckDlg::~CSpellCheckDlg()
{
	ReleaseSpellCheckerInterface(m_pSpellChecker);
}

void CSpellCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpellCheckDlg)
	DDX_Control(pDX, IDC_SCD_SUGGESTIONS, m_lbSuggestions);
	DDX_Text(pDX, IDC_SCD_MISSPELTWORD, m_sMisspeltWord);
	DDX_LBString(pDX, IDC_SCD_SUGGESTIONS, m_sSuggestion);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SCD_URL, m_stURL);
	DDX_Control(pDX, IDC_SCD_DICTIONARIES, m_cbDictionaries);

	// we handle m_sText manually for finer control
	DDX_Control(pDX, IDC_SCD_TEXT, m_reText);

	if (pDX->m_bSaveAndValidate)
	{
		int nCurSel = m_cbDictionaries.GetCurSel();

		if (nCurSel != CB_ERR)
			m_cbDictionaries.GetLBText(nCurSel, m_sSelDictionary);
		else
			m_sSelDictionary.Empty();
	}
	else
	{
		// make sure its in the list and move to top
		int nDic = m_cbDictionaries.FindStringExact(-1, m_sSelDictionary);

		if (nDic != CB_ERR)
			m_cbDictionaries.DeleteString(nDic);

		if (!m_sSelDictionary.IsEmpty())
		{
			m_cbDictionaries.InsertString(0, m_sSelDictionary);
			m_cbDictionaries.SetCurSel(0);
		}
	}
}

BEGIN_MESSAGE_MAP(CSpellCheckDlg, CRuntimeDlg)
	//{{AFX_MSG_MAP(CSpellCheckDlg)
	ON_BN_CLICKED(IDC_SCD_REPLACE, OnReplace)
	ON_BN_CLICKED(IDC_SCD_NEXT, OnContinue)
	ON_LBN_SELCHANGE(IDC_SCD_SUGGESTIONS, OnSelchangeSuggestions)
	ON_BN_CLICKED(IDC_SCD_RESTART, OnRestart)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SCD_BROWSE, OnBrowse)
	ON_CBN_SELCHANGE(IDC_SCD_DICTIONARIES, OnChangeDictionary)
	ON_LBN_DBLCLK(IDC_SCD_SUGGESTIONS, OnDblClkSuggestions)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpellCheckDlg message handlers

int CSpellCheckDlg::DoModal(BOOL bEndOnNoErrors) 
{ 
	m_bEndOnNoErrors = bEndOnNoErrors;

	// turn off centering if we saved the last position
	DWORD dwFlags = RTD_DEFSTYLE;

	if (m_ptTopLeft.x != -1 || m_ptTopLeft.y != -1)
		dwFlags &= ~DS_CENTER;

	return CRuntimeDlg::DoModal(GetItemText(SCD_TITLE, _T("Spell Checking")), dwFlags); 
}

BOOL CSpellCheckDlg::InitDictionary(const CString& sDict)
{
	CString sDictPath = FileMisc::GetFullPath(sDict, FileMisc::GetAppFolder());
	
	CString sAffixPath(sDictPath);
	FileMisc::ReplaceExtension(sAffixPath, _T(".aff"));

	ISpellChecker* pTemp = CreateSpellCheckerInterface(m_sEnginePath, sAffixPath, sDictPath);

	if (pTemp) // alls well
	{
		ReleaseSpellCheckerInterface(m_pSpellChecker);

		m_pSpellChecker = pTemp;
		pTemp = NULL;
		m_sSelDictionary = sDict;

		UpdateData(FALSE);
		UpdateButtonStates();

		// clear the current word and reset 
		HighlightWord(FALSE);
	}

	return (NULL != m_pSpellChecker);
}

void CSpellCheckDlg::SetDictionaryDownloadUrl(const CString& sUrl)
{
	m_stURL.m_link = sUrl;
}

void CSpellCheckDlg::SetText(const CString& sText) 
{ 
	m_sText = sText; 
	m_pSpellCheck = &m_reSpellCheck;

	m_sMisspeltWord.Empty();
	m_sSuggestion.Empty();

	if (GetSafeHwnd())
	{
		m_reText.SetBackgroundColor(TRUE, 0);
		m_reText.SetWindowText(m_sText);
		m_lbSuggestions.ResetContent();

		UpdateData(FALSE);
		StartChecking();
	}
}

void CSpellCheckDlg::SetSpellCheck(ISpellCheck* pSpellCheck) 
{ 
	m_sText = pSpellCheck->GetReferenceTextBeingChecked(); 
	m_pSpellCheck = pSpellCheck;

	m_sMisspeltWord.Empty();
	m_sSuggestion.Empty();

	if (GetSafeHwnd())
	{
		m_reText.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
		m_reText.SetWindowText(m_sText);
		m_lbSuggestions.ResetContent();

		UpdateData(FALSE);
		StartChecking();
	}
}

void CSpellCheckDlg::OnChangeDictionary()
{
	UpdateData();

	if (InitDictionary(m_sSelDictionary))
		StartChecking(CH_CURRENT);
}

void CSpellCheckDlg::OnBrowse()
{
	UpdateData();

	CString sFilter = GetItemText(DLG_SCD_DICTFILTER, _T("Dictionaries (*.dic)|*.dic||"));
	CString sTitle = GetItemText(DLG_SCD_BROWSETITLE, _T("Select Dictionary"));

	CFileOpenDialog dialog(sTitle, _T("dic"), m_sSelDictionary, OFN_PATHMUSTEXIST, sFilter);
	
	if (dialog.DoModal() == IDOK)
	{
		CString sDict = FileMisc::GetRelativePath(dialog.GetPathName(), FileMisc::GetAppFolder(), FALSE);

		if (InitDictionary(sDict))
			StartChecking(CH_CURRENT);
	}
}

void CSpellCheckDlg::OnReplace() 
{
	if (!m_pSpellChecker)
		return;

	UpdateData();
	ASSERT (!m_sSuggestion.IsEmpty());

	m_pSpellCheck->ReplaceCurrentWord(m_sSuggestion);
	m_pSpellCheck->ClearSelection();
	m_bMadeChanges = TRUE;

	m_sText = m_pSpellCheck->GetReferenceTextBeingChecked();

	// update richedit with 'fixed' text
	if (m_pSpellCheck != &m_reSpellCheck)
	{
		// Can't get saving/restoring scrollpos to work
		//int nFirstVis = m_reText.GetFirstVisibleLine();

		m_reText.SetWindowText(m_sText);

		//m_reText.SetFirstVisibleLine(nFirstVis);
	}
		
	OnContinue();
}

void CSpellCheckDlg::OnContinue() 
{
	if (m_pSpellChecker)
		StartChecking(CH_NEXT);
}

BOOL CSpellCheckDlg::FindNextWord(CString& sWord, CHECKFROM nFrom)
{
	switch (nFrom)
	{
	case CH_START:
		sWord = m_pSpellCheck->GetFirstWord();
		break;

	case CH_CURRENT:
		sWord = m_pSpellCheck->GetCurrentWord();
		break;

	case CH_NEXT:
		sWord = m_pSpellCheck->GetNextWord();
		break;
	}

	return (!sWord.IsEmpty());
}

BOOL CSpellCheckDlg::FindNextMisspeltWord(CString& sWord, CHECKFROM nFrom)
{
	while (TRUE)
	{
		if (!FindNextWord(sWord, nFrom))
			return FALSE;

		else if (IsWordMisspelt(sWord))
			return TRUE;

		// else continue
		nFrom = CH_NEXT;
	}

	// cant get here
	ASSERT (0);
	return FALSE;
}

BOOL CSpellCheckDlg::OnInitDialog() 
{
	CRuntimeDlg::OnInitDialog();

	// don't localize certain fields
	CLocalizer::EnableTranslation(m_lbSuggestions, FALSE);
	CLocalizer::EnableTranslation(m_cbDictionaries, FALSE);
	CLocalizer::EnableTranslation(::GetDlgItem(*this, IDC_SCD_MISSPELTWORD), FALSE);

	// init spell checker dll path
	CPreferences prefs;

#ifndef _DEBUG
	m_sEnginePath = prefs.GetProfileString(_T("SpellChecker"), _T("EnginePath"), _T("MySpellCheck.dll"));
#else
	m_sEnginePath = _T("MySpellCheck.dll");
#endif

	if (!IsSpellCheckDll(m_sEnginePath))
	{
		CString sDllPath = FileMisc::GetAppFilePath(), sDrive, sFolder;

		FileMisc::SplitPath(sDllPath, &sDrive, &sFolder);
		FileMisc::MakePath(sDllPath, sDrive, sFolder, _T("*"), _T(".dll"));

		m_sEnginePath = sDllPath;
	}

	m_ncBorder.Initialize(m_reText);
	m_bMadeChanges = FALSE;

	// reload dictionary list
	int nDicCount = prefs.GetProfileInt(_T("SpellChecker"), _T("DictionaryCount"), 0);

	for (int nDic = 0; nDic < nDicCount; nDic++)
	{
		CString sKey;
		sKey.Format(_T("Dictionary%d"), nDic);

		CString sDict = prefs.GetProfileString(_T("SpellChecker"), sKey);
		FileMisc::MakeRelativePath(sDict, FileMisc::GetAppFolder(), FALSE);

		CString sPath = FileMisc::GetFullPath(sDict, FileMisc::GetAppFolder());

		if (FileMisc::FileExists(sPath))
		{
			if (m_cbDictionaries.FindStringExact(-1, sDict) == CB_ERR)
				m_cbDictionaries.AddString(sDict);
		}
	}

	if (m_sSelDictionary.IsEmpty())
	{
		m_sSelDictionary = prefs.GetProfileString(_T("SpellChecker"), _T("ActiveDictionary"));
		FileMisc::MakeRelativePath(m_sSelDictionary, FileMisc::GetAppFolder(), FALSE);
	}

	// check spell check engine is initialized
	BOOL bCancel = FALSE;

	if (!IsSpellCheckDll(m_sEnginePath))
	{
		CString sMsg = GetItemText(DLG_SCD_SETUPMSG, _T("Before you can spell check for the first time you need \nto specify the location of the Spell Checker."));
		bCancel = (IDCANCEL == AfxMessageBox(sMsg, MB_OKCANCEL));

		while (!bCancel && !IsSpellCheckDll(m_sEnginePath))
		{
			// notify user and browse for dll
			CString sFilter = GetItemText(DLG_SCD_ENGINEFILTER, _T("SpellChecker Engines|*.dll||"));
			CString sTitle = GetItemText(DLG_SCD_ENGINETITLE, _T("Locate Spell Check Engine"));

			CFileOpenDialog dialog(sTitle, _T("dll"), m_sEnginePath, OFN_PATHMUSTEXIST, sFilter);

			if (dialog.DoModal(prefs) == IDOK)
				m_sEnginePath = dialog.GetPathName();
			else
				bCancel = TRUE;
		}
	}

	if (bCancel)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// make the rich edit appear disabled if not using text
	if (m_pSpellCheck != &m_reSpellCheck)
		m_reText.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));

	m_reText.SetWindowText(m_sText);
	
	if (IsInitialized() || InitDictionary(m_sSelDictionary))
	{
		if (!StartChecking() && m_bEndOnNoErrors)
		{
			EndDialog(IDNOERRORS);
			return FALSE;
		}
	}

	UpdateButtonStates();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

CString CSpellCheckDlg::GetItemText(UINT nIDItem, const CString& sDefault)
{
	CString sText;
	s_mapText.Lookup(nIDItem, sText);
	
	if (sText.IsEmpty())
		return sDefault;

	return sText;
}

void CSpellCheckDlg::SetItemText(UINT nIDItem, UINT nIDText)
{
	s_mapText[nIDItem] = CEnString(nIDText);
}

BOOL CSpellCheckDlg::StartChecking(CHECKFROM nFrom)
{
	CString sWord; 

	BOOL bMisspelt = FindNextMisspeltWord(sWord, nFrom);

	// if starting from the beginning and no misspelt words are found
	// then e return FALSE to indicate no further checking required
	if (nFrom == CH_START && !bMisspelt)
	{
		return FALSE;
	}
	else if (bMisspelt)
	{
		ProcessMisspeltWord(sWord);
		return TRUE;
	}

	// else reached the end so start again
	ProcessMisspeltWord(_T(""));

	return StartChecking(CH_START);
}

void CSpellCheckDlg::UpdateButtonStates()
{
	BOOL bInit = IsInitialized();

	GetDlgItem(IDC_SCD_REPLACE)->EnableWindow(bInit && !m_sMisspeltWord.IsEmpty());
	GetDlgItem(IDC_SCD_NEXT)->EnableWindow(bInit);
	GetDlgItem(IDC_SCD_RESTART)->EnableWindow(bInit);
	GetDlgItem(IDOK)->EnableWindow(bInit);
}

void CSpellCheckDlg::ProcessMisspeltWord(const CString& sWord)
{
	if (sWord.IsEmpty()) // no misspelling
	{
		m_lbSuggestions.ResetContent();
		m_sMisspeltWord.Empty();

		UpdateData(FALSE);
		UpdateButtonStates();

		return;
	}

	// else
	m_sMisspeltWord.Format(_T("'%s'"), sWord);
	UpdateData(FALSE);

	// set new selection
	HighlightWord(TRUE);

	// add suggestions
	m_lbSuggestions.ResetContent();

	if (m_pSpellChecker)
	{
		LPTSTR* pSuggestions = NULL;
		int nNumSuggestions = 0;

		m_pSpellChecker->CheckSpelling(sWord, pSuggestions, nNumSuggestions);

		for (int nSugg = 0; nSugg < nNumSuggestions; nSugg++)
			m_lbSuggestions.AddString(pSuggestions[nSugg]);

		m_pSpellChecker->FreeSuggestions(pSuggestions, nNumSuggestions);
	}

	if (m_lbSuggestions.GetCount())
		m_lbSuggestions.GetText(0, m_sSuggestion);
	else
		m_sSuggestion.Empty();

	m_lbSuggestions.SelectString(-1, m_sSuggestion);
	GetDlgItem(IDC_SCD_REPLACE)->EnableWindow(m_lbSuggestions.GetCount());
}

void CSpellCheckDlg::HighlightWord(BOOL bHighlight)
{
	if (bHighlight)
		m_pSpellCheck->SelectCurrentWord();
	else
		m_pSpellCheck->ClearSelection();
}

BOOL CSpellCheckDlg::IsWordMisspelt(const CString& sWord)
{
	// jump over anything that contains a number
	if (sWord.FindOneOf(_T("0123456789")) != -1)
		return FALSE;

	if (m_pSpellChecker)
		return (m_pSpellChecker->CheckSpelling(sWord) == 0);

	return FALSE;
}

void CSpellCheckDlg::OnSelchangeSuggestions() 
{
	UpdateData();
	
	GetDlgItem(IDC_SCD_REPLACE)->EnableWindow(!m_sSuggestion.IsEmpty());
}

void CSpellCheckDlg::OnRestart() 
{
	if (m_pSpellChecker)
	{
		if (m_pSpellCheck == &m_reSpellCheck)
			m_bMadeChanges = FALSE;

		StartChecking();	
	}
}

void CSpellCheckDlg::OnDestroy() 
{
	CPreferences prefs;

	m_pSpellCheck->ClearSelection();

	// save position
	if (m_ptTopLeft.x == -1 && m_ptTopLeft.y == -1)
	{
		CRect rWindow;
		GetWindowRect(rWindow);

		m_ptTopLeft = rWindow.TopLeft();
	}
	
	CRuntimeDlg::OnDestroy();
	
	prefs.WriteProfileString(_T("SpellChecker"), _T("EnginePath"), m_sEnginePath);
	prefs.WriteProfileString(_T("SpellChecker"), _T("ActiveDictionary"), m_sSelDictionary);

	// save dictionary list
	int nDicCount = m_cbDictionaries.GetCount();
	prefs.WriteProfileInt(_T("SpellChecker"), _T("DictionaryCount"), nDicCount);

	for (int nDic = 0; nDic < nDicCount; nDic++)
	{
		CString sKey;
		sKey.Format(_T("Dictionary%d"), nDic);

		CString sPath;
		m_cbDictionaries.GetLBText(nDic, sPath);

		prefs.WriteProfileString(_T("SpellChecker"), sKey, sPath);
	}
}

void CSpellCheckDlg::OnDblClkSuggestions()
{
	UpdateData();

	if (m_lbSuggestions.GetCount() && !m_sSuggestion.IsEmpty())
		OnReplace();
}

CPoint CSpellCheckDlg::GetInitialPos() const 
{ 
	CPoint ptTemp(m_ptTopLeft);

	m_ptTopLeft.x = m_ptTopLeft.y = -1;  // reset

	return ptTemp;
}
