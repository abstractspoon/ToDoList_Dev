// CreateFileLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rtfcontentctrl.h"
#include "rtfpreferencesdlg.h"

#include "..\shared\EnBitmap.h"
#include "..\shared\EnString.h"
#include "..\shared\FileMisc.h"
#include "..\shared\DialogHelper.h"
#include "..\shared\mswordhelper.h"

#include "..\Interfaces\iPreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRTFPreferencesPage dialog

CRTFPreferencesPage::CRTFPreferencesPage()
	: 
	CPreferencesPageBase(IDD_PREFERENCES_PAGE), 
	m_bPromptForFileLink(TRUE),
	m_nLinkOption(REP_ASIMAGE),
	m_bConvertWithMSWord(FALSE)
{
	//{{AFX_DATA_INIT(CRTFPreferencesPage)
	//}}AFX_DATA_INIT
}


void CRTFPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRTFPreferencesPage)
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_FILEURL, m_nLinkOption);
	DDX_Check(pDX, IDC_USEMSWORD, m_bConvertWithMSWord);
	DDX_Check(pDX, IDC_PROMPTFORFILELINK, m_bPromptForFileLink);
}

BEGIN_MESSAGE_MAP(CRTFPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CRTFPreferencesPage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PROMPTFORFILELINK, OnPromptForLink)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRTFPreferencesPage message handlers

void CRTFPreferencesPage::SetFileLinkOption(RE_PASTE nLinkOption, BOOL bPrompt)
{
	m_bPromptForFileLink = bPrompt;
	m_nLinkOption = nLinkOption;
}

BOOL CRTFPreferencesPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	m_groupLine.AddGroupLine(IDC_LINKTOFILE_GROUP, *this);

	if (!CMSWordHelper::IsWordInstalled(12))
		GetDlgItem(IDC_USEMSWORD)->EnableWindow(FALSE);

	OnPromptForLink();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRTFPreferencesPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// Note: Parent App handles global settings
	pPrefs->WriteProfileInt(szKey, _T("FileLinkOption"), m_nLinkOption);
	pPrefs->WriteProfileInt(szKey, _T("FileLinkOptionIsDefault"), !m_bPromptForFileLink);
}

void CRTFPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	// Note: Parent App handles global settings
	m_nLinkOption = (RE_PASTE)pPrefs->GetProfileInt(szKey, _T("FileLinkOption"), REP_ASIMAGE);
	m_bPromptForFileLink = !pPrefs->GetProfileInt(szKey, _T("FileLinkOptionIsDefault"), TRUE);
}

void CRTFPreferencesPage::OnPromptForLink()
{
	UpdateData();
	
	GetDlgItem(IDC_FILEURL)->EnableWindow(!m_bPromptForFileLink);
	GetDlgItem(IDC_FILECOPY)->EnableWindow(!m_bPromptForFileLink);
	GetDlgItem(IDC_FILELINK)->EnableWindow(!m_bPromptForFileLink);
	GetDlgItem(IDC_FILEIMAGE)->EnableWindow(!m_bPromptForFileLink);
}

/////////////////////////////////////////////////////////////////////////////
// CCreateFileLinkDlg dialog

CRTFPreferencesDlg::CRTFPreferencesDlg(CWnd* pParent /*=NULL*/)
	: CPreferencesDlgBase(IDD_PREFERENCES_DIALOG, IDC_PPHOST, IDR_RTFCOMMENTS, IDI_HELP_BUTTON, pParent)
{
	//{{AFX_DATA_INIT(CRTFPreferencesDlg)
	//}}AFX_DATA_INIT

	m_ppHost.AddPage(&m_page);
}


void CRTFPreferencesDlg::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRTFPreferencesDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRTFPreferencesDlg, CPreferencesDlgBase)
	//{{AFX_MSG_MAP(CRTFPreferencesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateFileLinkDlg message handlers

BOOL CRTFPreferencesDlg::OnInitDialog() 
{
	CPreferencesDlgBase::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int CRTFPreferencesDlg::DoModal(BOOL bUseMSWord)
{
	m_page.SetConvertWithMSWord(bUseMSWord);

	return CPreferencesDlgBase::DoModal();
}

void CRTFPreferencesDlg::DoHelp()
{
	ASSERT(m_pParentWnd);
	
	if (m_pParentWnd)
		m_pParentWnd->SendMessage(WM_RTF_PREFSHELP);
}
