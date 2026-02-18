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
#include "..\shared\OSVersion.h"

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
	m_bPromptForFileLink(FALSE),
	m_nLinkOption(REP_ASFILEURL),
	m_bReduceImageColors(FALSE),
	m_bConvertWithMSWord(FALSE)
{
}

void CRTFPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_FILEURL, m_nLinkOption);
	DDX_Check(pDX, IDC_USEMSWORD, m_bConvertWithMSWord);
	DDX_Check(pDX, IDC_PROMPTFORFILELINK, m_bPromptForFileLink);
	DDX_Check(pDX, IDC_REDUCEIMAGECOLORS, m_bReduceImageColors);
}

BEGIN_MESSAGE_MAP(CRTFPreferencesPage, CPreferencesPageBase)
	ON_BN_CLICKED(IDC_PROMPTFORFILELINK, OnClickPromptForLink)
	ON_BN_CLICKED(IDC_FILEIMAGE, OnChangeLinkOption)
	ON_BN_CLICKED(IDC_FILELINK, OnChangeLinkOption)
	ON_BN_CLICKED(IDC_FILECOPY, OnChangeLinkOption)
	ON_BN_CLICKED(IDC_FILEURL, OnChangeLinkOption)
END_MESSAGE_MAP()

void CRTFPreferencesPage::SetFileLinkOption(RE_PASTE nLinkOption, BOOL bPrompt, BOOL bReduceImageColors)
{
	if (COSVersion() == OSV_LINUX)
	{
		m_bPromptForFileLink = FALSE;
		m_nLinkOption = REP_ASFILEURL;
		m_bReduceImageColors = FALSE;
	}
	else
	{
		m_bPromptForFileLink = bPrompt;
		m_nLinkOption = nLinkOption;
		m_bReduceImageColors = bReduceImageColors;
	}
}

BOOL CRTFPreferencesPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	m_groupLine.AddGroupLine(IDC_LINKTOFILE_GROUP, *this);

	if (!CMSWordHelper::IsWordInstalled(12))
		GetDlgItem(IDC_USEMSWORD)->EnableWindow(FALSE);

	GetDlgItem(IDC_PROMPTFORFILELINK)->EnableWindow(COSVersion() != OSV_LINUX);
	OnClickPromptForLink();
	
	return TRUE;
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
	RE_PASTE nLinkOption = (RE_PASTE)pPrefs->GetProfileInt(szKey, _T("FileLinkOption"), REP_ASIMAGE);
	BOOL bPromptForFileLink = !pPrefs->GetProfileInt(szKey, _T("FileLinkOptionIsDefault"), TRUE);
	BOOL bReduceImageColors = pPrefs->GetProfileInt(szKey, _T("ReduceImageColors"), TRUE);

	SetFileLinkOption(nLinkOption, bPromptForFileLink, bReduceImageColors);
}

void CRTFPreferencesPage::OnClickPromptForLink()
{
	UpdateData();

	BOOL bLinuxOS = (COSVersion() == OSV_LINUX);
	
	GetDlgItem(IDC_FILEURL)->EnableWindow(!m_bPromptForFileLink);
	GetDlgItem(IDC_FILECOPY)->EnableWindow(!bLinuxOS && !m_bPromptForFileLink);
	GetDlgItem(IDC_FILELINK)->EnableWindow(!bLinuxOS && !m_bPromptForFileLink);
	GetDlgItem(IDC_FILEIMAGE)->EnableWindow(!bLinuxOS && !m_bPromptForFileLink);
	GetDlgItem(IDC_REDUCEIMAGECOLORS)->EnableWindow(!bLinuxOS && !m_bPromptForFileLink && (m_nLinkOption == REP_ASIMAGE));
}

void CRTFPreferencesPage::OnChangeLinkOption()
{
	UpdateData();
	
	BOOL bLinuxOS = (COSVersion() == OSV_LINUX);
	
	GetDlgItem(IDC_REDUCEIMAGECOLORS)->EnableWindow(!bLinuxOS && !m_bPromptForFileLink && (m_nLinkOption == REP_ASIMAGE));
}

/////////////////////////////////////////////////////////////////////////////
// CRTFPreferencesDlg dialog

CRTFPreferencesDlg::CRTFPreferencesDlg(CWnd* pParent /*=NULL*/)
	: 
	CPreferencesDlgBase(IDD_PREFERENCES_DIALOG, IDC_PPHOST, IDR_RTFCOMMENTS, IDI_HELP_BUTTON, pParent)
{
	m_ppHost.AddPage(&m_page);
}

void CRTFPreferencesDlg::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesDlgBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRTFPreferencesDlg, CPreferencesDlgBase)
END_MESSAGE_MAP()

BOOL CRTFPreferencesDlg::OnInitDialog() 
{
	CPreferencesDlgBase::OnInitDialog();
	
	return TRUE;
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
