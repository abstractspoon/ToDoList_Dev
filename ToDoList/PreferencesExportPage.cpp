// PreferencesFilePage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesexportPage.h"

#include "..\shared\Misc.h"
#include "..\shared\enstring.h"
#include "..\shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT ID_RESETCHARSET = 1;
const UINT RESETBTN = 0xc4;

/////////////////////////////////////////////////////////////////////////////
// CPreferencesExportPage property page

IMPLEMENT_DYNCREATE(CPreferencesExportPage, CPreferencesPageBase)

CPreferencesExportPage::CPreferencesExportPage() : 
		CPreferencesPageBase(CPreferencesExportPage::IDD), 
			m_eTextIndent(_T("0123456789")), m_eLineSpaces(_T("0123456789"))
{
//	m_psp.dwFlags &= ~PSP_HASHELP;

	//{{AFX_DATA_INIT(CPreferencesExportPage)
	m_nTextIndent = 2;
	m_nLineSpaces = 8;
	//}}AFX_DATA_INIT

	// add a 'reset' button to the charset field
	m_eCharset.InsertButton(0, ID_RESETCHARSET, RESETBTN, 
								CEnString(IDS_PEP_RESETCHARSET), 
								DEF_BTNWIDTH + 4, _T("Wingdings"));
}

CPreferencesExportPage::~CPreferencesExportPage()
{
}

void CPreferencesExportPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesExportPage)
	DDX_Control(pDX, IDC_NUMLINESPACE, m_eLineSpaces);
	DDX_Control(pDX, IDC_HTMLFONTSIZE, m_cbFontSize);
	DDX_Check(pDX, IDC_EXPORTPARENTTITLECOMMENTSONLY, m_bExportParentTitleCommentsOnly);
	DDX_Check(pDX, IDC_EXPORTSPACEFORNOTES, m_bExportSpaceForNotes);
	DDX_Text(pDX, IDC_CHARSET, m_sHtmlCharSet);
	DDX_Radio(pDX, IDC_TABTEXTINDENT, m_bUseSpaceIndents);
	DDX_Text(pDX, IDC_TABWIDTHS, m_nTextIndent);
	DDX_Text(pDX, IDC_NUMLINESPACE, m_nLineSpaces);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TABWIDTHS, m_eTextIndent);
	DDX_Control(pDX, IDC_HTMLFONTLIST, m_cbFonts);
	DDX_Check(pDX, IDC_PREVIEWEXPORT, m_bPreviewExport);
	DDX_Check(pDX, IDC_EXPORTVISIBLEONLY, m_bExportVisibleOnly);
	DDX_Control(pDX, IDC_CHARSET, m_eCharset);

	m_cbFonts.DDX(pDX, m_sHtmlFont);

	// custom
	if (pDX->m_bSaveAndValidate)
	{
		m_nHtmlFontSize = CDialogHelper::GetSelectedItemAsValue(m_cbFontSize);
	}
	else
	{
		if (!CDialogHelper::SelectItemByValue(m_cbFontSize, m_nHtmlFontSize))
		{
			m_nHtmlFontSize = 3;
			CDialogHelper::SelectItemByValue(m_cbFontSize, m_nHtmlFontSize);
		}
	}
}


BEGIN_MESSAGE_MAP(CPreferencesExportPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesExportPage)
	ON_BN_CLICKED(IDC_TABTEXTINDENT, OnChangeTextIndentType)
	ON_BN_CLICKED(IDC_SPACETEXTINDENT, OnChangeTextIndentType)
	ON_BN_CLICKED(IDC_EXPORTSPACEFORNOTES, OnExportspacefornotes)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEEBtnClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesExportPage message handlers

BOOL CPreferencesExportPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();
	
	m_eTextIndent.EnableWindow(m_bUseSpaceIndents);
	GetDlgItem(IDC_TEXTINDENTTRAIL)->EnableWindow(m_bUseSpaceIndents);
	
	m_eLineSpaces.EnableWindow(m_bExportSpaceForNotes);
	GetDlgItem(IDC_NUMLINESPACETRAIL)->EnableWindow(m_bExportSpaceForNotes);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesExportPage::OnChangeTextIndentType() 
{
	UpdateData();

	m_eTextIndent.EnableWindow(m_bUseSpaceIndents);
	GetDlgItem(IDC_TEXTINDENTTRAIL)->EnableWindow(m_bUseSpaceIndents);

	CPreferencesPageBase::OnControlChange();
}

CString CPreferencesExportPage::GetHtmlCharSet() const
{
	if (m_sHtmlCharSet.IsEmpty())
		return _T("Windows - ") + Misc::GetDefCharset();

	// else
	return m_sHtmlCharSet;
}


void CPreferencesExportPage::LoadPreferences(const CPreferences& prefs)
{
	m_sHtmlFont = prefs.GetProfileString(_T("Preferences"), _T("HtmlFont"), _T("Verdana"));
	m_bPreviewExport = prefs.GetProfileInt(_T("Preferences"), _T("PreviewSaveAs"), TRUE);
	m_nHtmlFontSize = prefs.GetProfileInt(_T("Preferences"), _T("HtmlFontSize"), 2);
	m_nTextIndent = prefs.GetProfileInt(_T("Preferences"), _T("TextIndent"), 2);
	m_nLineSpaces = prefs.GetProfileInt(_T("Preferences"), _T("LineSpaces"), 8);
	m_bExportVisibleOnly = prefs.GetProfileInt(_T("Preferences"), _T("ExportVisibleOnly"), FALSE);
	m_bExportParentTitleCommentsOnly = prefs.GetProfileInt(_T("Preferences"), _T("ExportParentTitleCommentsOnly"), FALSE);
	m_bExportSpaceForNotes = prefs.GetProfileInt(_T("Preferences"), _T("ExportSpaceForNotes"), FALSE);
	m_bUseSpaceIndents = prefs.GetProfileInt(_T("Preferences"), _T("UseSpaceIndents"), TRUE);

	CString sDefCharset = "Windows-" + Misc::GetDefCharset();
	m_sHtmlCharSet = prefs.GetProfileString(_T("Preferences"), _T("HtmlCharSet"), sDefCharset);

//	m_b = prefs.GetProfileInt(_T("Preferences"), _T(""), FALSE);
}

void CPreferencesExportPage::SavePreferences(CPreferences& prefs)
{
	// save settings
	prefs.WriteProfileString(_T("Preferences"), _T("HtmlFont"), m_sHtmlFont);
	prefs.WriteProfileInt(_T("Preferences"), _T("HtmlFontSize"), m_nHtmlFontSize);
	prefs.WriteProfileInt(_T("Preferences"), _T("PreviewSaveAs"), m_bPreviewExport);
	prefs.WriteProfileInt(_T("Preferences"), _T("TextIndent"), m_nTextIndent);
	prefs.WriteProfileInt(_T("Preferences"), _T("LineSpaces"), m_nLineSpaces);
	prefs.WriteProfileInt(_T("Preferences"), _T("ExportVisibleOnly"), m_bExportVisibleOnly);
	prefs.WriteProfileInt(_T("Preferences"), _T("ExportParentTitleCommentsOnly"), m_bExportParentTitleCommentsOnly);
	prefs.WriteProfileInt(_T("Preferences"), _T("ExportSpaceForNotes"), m_bExportSpaceForNotes);
	prefs.WriteProfileString(_T("Preferences"), _T("HtmlCharSet"), m_sHtmlCharSet);
	prefs.WriteProfileInt(_T("Preferences"), _T("UseSpaceIndents"), m_bUseSpaceIndents);
//	prefs.WriteProfileInt(_T("Preferences"), _T(""), m_b);
}

LRESULT CPreferencesExportPage::OnEEBtnClick(WPARAM wp, LPARAM lp)
{
	if (wp == IDC_CHARSET && lp == ID_RESETCHARSET)
	{
		m_sHtmlCharSet = _T("Windows-") + Misc::GetDefCharset();
		UpdateData(FALSE);

		CPreferencesPageBase::OnControlChange();
	}

	return 0L;
}

void CPreferencesExportPage::OnExportspacefornotes() 
{
	UpdateData();

	m_eLineSpaces.EnableWindow(m_bExportSpaceForNotes);
	GetDlgItem(IDC_NUMLINESPACETRAIL)->EnableWindow(m_bExportSpaceForNotes);

	CPreferencesPageBase::OnControlChange();
}
