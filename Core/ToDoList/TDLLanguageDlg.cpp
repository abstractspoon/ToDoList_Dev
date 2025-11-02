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
	CTDLDialog(CTDLLanguageDlg::IDD, _T("Language"), pParent), 
	m_cbLanguages(_T("*.csv"))
{
	if (!Misc::IsEmpty(szSelLangFile))
		m_cbLanguages.SelectLanguageFile(szSelLangFile);
}

CTDLLanguageDlg::~CTDLLanguageDlg()
{
}

void CTDLLanguageDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLLanguageDlg)
	DDX_Control(pDX, IDC_LANGUAGES, m_cbLanguages);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLLanguageDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLLanguageDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLLanguageDlg message handlers

BOOL CTDLLanguageDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	SetIcon(IDR_MAINFRAME);

	// Initialise language to user's UI language
	m_cbLanguages.SelectUserLanguage();
	m_cbLanguages.SetFocus();
	
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
