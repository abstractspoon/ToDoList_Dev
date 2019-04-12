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
	DDX_Check(pDX, IDC_REDUCEIMAGECOLORS, m_bReduceImageColors);
}

BEGIN_MESSAGE_MAP(CRTFPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CRTFPreferencesPage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PROMPTFORFILELINK, OnClickPromptForLink)
	ON_BN_CLICKED(IDC_FILEIMAGE, OnChangeLinkOption)
	ON_BN_CLICKED(IDC_FILELINK, OnChangeLinkOption)
	ON_BN_CLICKED(IDC_FILECOPY, OnChangeLinkOption)
	ON_BN_CLICKED(IDC_FILEURL, OnChangeLinkOption)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRTFPreferencesPage message handlers

void CRTFPreferencesPage::SetFileLinkOption(RE_PASTE nLinkOption, BOOL bPrompt, BOOL bReduceImageColors)
{
	m_bPromptForFileLink = bPrompt;
	m_nLinkOption = nLinkOption;
	m_bReduceImageColors = bReduceImageColors;
}

BOOL CRTFPreferencesPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	m_groupLine.AddGroupLine(IDC_LINKTOFILE_GROUP, *this);

	if (!CMSWordHelper::IsWordInstalled(12))
		GetDlgItem(IDC_USEMSWORD)->EnableWindow(FALSE);

	OnClickPromptForLink();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRTFPreferencesPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// Note: Parent App handles global settings
	pPrefs->WriteProfileInt(szKey, _T("FileLinkOption"), m_nLinkOption);
	pPrefs->WriteProfileInt(szKey, _T("FileLinkOptionIsDefault"), !m_bPromptForFileLink);
	pPrefs->WriteProfileInt(szKey, _T("ReduceImageColors"), m_bReduceImageColors);
}

void CRTFPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	// Note: Parent App handles global settings
	m_nLinkOption = (RE_PASTE)pPrefs->GetProfileInt(szKey, _T("FileLinkOption"), REP_ASIMAGE);
	m_bPromptForFileLink = !pPrefs->GetProfileInt(szKey, _T("FileLinkOptionIsDefault"), TRUE);
	m_bReduceImageColors = pPrefs->GetProfileInt(szKey, _T("ReduceImageColors"), TRUE);
}

void CRTFPreferencesPage::OnClickPromptForLink()
{
	UpdateData();
	
	GetDlgItem(IDC_FILEURL)->EnableWindow(!m_bPromptForFileLink);
	GetDlgItem(IDC_FILECOPY)->EnableWindow(!m_bPromptForFileLink);
	GetDlgItem(IDC_FILELINK)->EnableWindow(!m_bPromptForFileLink);
	GetDlgItem(IDC_FILEIMAGE)->EnableWindow(!m_bPromptForFileLink);
	GetDlgItem(IDC_REDUCEIMAGECOLORS)->EnableWindow(!m_bPromptForFileLink && (m_nLinkOption == REP_ASIMAGE));
}

void CRTFPreferencesPage::OnChangeLinkOption()
{
	UpdateData();
	
	GetDlgItem(IDC_REDUCEIMAGECOLORS)->EnableWindow(!m_bPromptForFileLink && (m_nLinkOption == REP_ASIMAGE));
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

void CRTFPreferencesDlg::SetFileLinkOption(RE_PASTE nLinkOption, BOOL bPrompt, BOOL bReduceImageColors) 
{ 
	m_page.SetFileLinkOption(nLinkOption, bPrompt, bReduceImageColors); 
}
