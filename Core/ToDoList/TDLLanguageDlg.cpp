// TDLLanguageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLLanguageDlg.h"

#include "..\shared\graphicsmisc.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLLanguageDlg dialog

CTDLLanguageDlg::CTDLLanguageDlg(LPCTSTR szSelLangFile, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_LANGUAGE_DIALOG, _T("Language"), pParent),
	m_cbLanguages(_T("*.csv")),
	m_bFirstTime(Misc::IsEmpty(szSelLangFile))
{
	if (!m_bFirstTime)
		m_cbLanguages.SelectLanguageFile(szSelLangFile);
}

CTDLLanguageDlg::~CTDLLanguageDlg()
{
}

void CTDLLanguageDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LANGUAGES, m_cbLanguages);
}


BEGIN_MESSAGE_MAP(CTDLLanguageDlg, CTDLDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CTDLLanguageDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	SetIcon(IDR_MAINFRAME);

	// Initialise language to user's UI language
	if (m_bFirstTime)
		m_cbLanguages.SelectUserLanguage();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CTDLLanguageDlg::GetSelectedLanguageFile(BOOL bRelative) const
{ 
	return m_cbLanguages.GetSelectedLanguageFile(bRelative); 
}

CString CTDLLanguageDlg::GetDefaultLanguage() 
{ 
	return CTDLLanguageComboBox::GetDefaultLanguage(); 
}
