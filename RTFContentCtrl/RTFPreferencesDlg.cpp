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
	CPropertyPage(IDD_PREFERENCES_PAGE), 
	m_bPromptForFileLink(TRUE),
	m_nLinkOption(REP_ASIMAGE),
	m_bConvertWithMSWord(FALSE)
{
	//{{AFX_DATA_INIT(CRTFPreferencesPage)
	//}}AFX_DATA_INIT
}


void CRTFPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRTFPreferencesPage)
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_FILEURL, m_nLinkOption);
	DDX_Check(pDX, IDC_USEMSWORD, m_bConvertWithMSWord);
	DDX_Check(pDX, IDC_PROMPTFORFILELINK, m_bPromptForFileLink);
}

BEGIN_MESSAGE_MAP(CRTFPreferencesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRTFPreferencesPage)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
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
	CPropertyPage::OnInitDialog();

	m_groupLine.AddGroupLine(IDC_LINKTOFILE_GROUP, *this);

	if (!CMSWordHelper::IsWordInstalled(12))
		GetDlgItem(IDC_USEMSWORD)->EnableWindow(FALSE);

	OnPromptForLink();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRTFPreferencesPage::OnEraseBkgnd(CDC* pDC)
{
	CRect rClient;
	GetClientRect(rClient);
	pDC->FillSolidRect(rClient, ::GetSysColor(COLOR_WINDOW));
	
	return TRUE;
}

HBRUSH CRTFPreferencesPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = ::GetSysColorBrush(COLOR_WINDOW);
	}
	
	return hbr;
}

void CRTFPreferencesPage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	// resize controls to fit text
	if (bShow)
		CDialogHelper::ResizeButtonStaticTextFieldsToFit(this);
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
	: CDialog(IDD_PREFERENCES_DIALOG, pParent)
{
	//{{AFX_DATA_INIT(CRTFPreferencesDlg)
	//}}AFX_DATA_INIT

	m_ppHost.AddPage(&m_page);
}


void CRTFPreferencesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRTFPreferencesDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRTFPreferencesDlg, CDialog)
	//{{AFX_MSG_MAP(CRTFPreferencesDlg)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateFileLinkDlg message handlers

BOOL CRTFPreferencesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	VERIFY(m_ppHost.Create(IDC_PPHOST, this));
	
	// shrink the ppHost by 1 pixel to allow the border to show
	CRect rHost;
	m_ppHost.GetWindowRect(rHost);
	ScreenToClient(rHost);
	
	rHost.DeflateRect(1, 1);
	m_ppHost.MoveWindow(rHost, FALSE);
	
	// Replace icon
	m_hIcon = AfxGetApp()->LoadIcon(IDR_RTFCOMMENTS);
	SendMessage(WM_SETICON, ICON_SMALL, (LPARAM)m_hIcon);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRTFPreferencesDlg::OnOK()
{
	CDialog::OnOK();
	
	m_ppHost.OnOK();
}

void CRTFPreferencesDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	::DestroyIcon(m_hIcon);
}

void CRTFPreferencesDlg::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	m_page.SavePreferences(pPrefs, szKey);
}

void CRTFPreferencesDlg::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	m_page.LoadPreferences(pPrefs, szKey);
}

int CRTFPreferencesDlg::DoModal(BOOL bUseMSWord)
{
	m_page.SetConvertWithMSWord(bUseMSWord);

	return CDialog::DoModal();
}

