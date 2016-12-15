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

CPreferencesExportPage::CPreferencesExportPage() 
	: 
	CPreferencesPageBase(CPreferencesExportPage::IDD), 
	m_eTextIndent(_T("0123456789")), 
	m_eLineSpaces(_T("0123456789"))
{
//	m_psp.dwFlags &= ~PSP_HASHELP;

	//{{AFX_DATA_INIT(CPreferencesExportPage)
	m_nTextIndent = 2;
	m_nLineSpaces = 8;
	//}}AFX_DATA_INIT
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
	DDX_Radio(pDX, IDC_TABTEXTINDENT, m_bUseSpaceIndents);
	DDX_Text(pDX, IDC_TABWIDTHS, m_nTextIndent);
	DDX_Text(pDX, IDC_NUMLINESPACE, m_nLineSpaces);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TABWIDTHS, m_eTextIndent);
	DDX_Control(pDX, IDC_HTMLFONTLIST, m_cbFonts);
	DDX_Check(pDX, IDC_PREVIEWEXPORT, m_bPreviewExport);
	DDX_Check(pDX, IDC_EXPORTVISIBLEONLY, m_bExportVisibleOnly);

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

void CPreferencesExportPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	m_sHtmlFont = pPrefs->GetProfileString(szKey, _T("HtmlFont"), _T("Verdana"));
	m_bPreviewExport = pPrefs->GetProfileInt(szKey, _T("PreviewSaveAs"), TRUE);
	m_nHtmlFontSize = pPrefs->GetProfileInt(szKey, _T("HtmlFontSize"), 2);
	m_nTextIndent = pPrefs->GetProfileInt(szKey, _T("TextIndent"), 2);
	m_nLineSpaces = pPrefs->GetProfileInt(szKey, _T("LineSpaces"), 8);
	m_bExportVisibleOnly = pPrefs->GetProfileInt(szKey, _T("ExportVisibleOnly"), FALSE);
	m_bExportParentTitleCommentsOnly = pPrefs->GetProfileInt(szKey, _T("ExportParentTitleCommentsOnly"), FALSE);
	m_bExportSpaceForNotes = pPrefs->GetProfileInt(szKey, _T("ExportSpaceForNotes"), FALSE);
	m_bUseSpaceIndents = pPrefs->GetProfileInt(szKey, _T("UseSpaceIndents"), TRUE);

//	m_b = pPrefs->GetProfileInt(szKey, _T(""), FALSE);
}

void CPreferencesExportPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileString(szKey, _T("HtmlFont"), m_sHtmlFont);
	pPrefs->WriteProfileInt(szKey, _T("HtmlFontSize"), m_nHtmlFontSize);
	pPrefs->WriteProfileInt(szKey, _T("PreviewSaveAs"), m_bPreviewExport);
	pPrefs->WriteProfileInt(szKey, _T("TextIndent"), m_nTextIndent);
	pPrefs->WriteProfileInt(szKey, _T("LineSpaces"), m_nLineSpaces);
	pPrefs->WriteProfileInt(szKey, _T("ExportVisibleOnly"), m_bExportVisibleOnly);
	pPrefs->WriteProfileInt(szKey, _T("ExportParentTitleCommentsOnly"), m_bExportParentTitleCommentsOnly);
	pPrefs->WriteProfileInt(szKey, _T("ExportSpaceForNotes"), m_bExportSpaceForNotes);
	pPrefs->WriteProfileInt(szKey, _T("UseSpaceIndents"), m_bUseSpaceIndents);

//	pPrefs->WriteProfileInt(szKey, _T(""), m_b);
}

void CPreferencesExportPage::OnExportspacefornotes() 
{
	UpdateData();

	m_eLineSpaces.EnableWindow(m_bExportSpaceForNotes);
	GetDlgItem(IDC_NUMLINESPACETRAIL)->EnableWindow(m_bExportSpaceForNotes);

	CPreferencesPageBase::OnControlChange();
}
