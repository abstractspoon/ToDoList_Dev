// PreferencesMultiUserPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesMultiUserPage.h"

#include "..\shared\misc.h"
#include "..\shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesMultiUserPage property page

IMPLEMENT_DYNCREATE(CPreferencesMultiUserPage, CPreferencesPageBase)

CPreferencesMultiUserPage::CPreferencesMultiUserPage() : 
   CPreferencesPageBase(CPreferencesMultiUserPage::IDD)
{
	//{{AFX_DATA_INIT(CPreferencesMultiUserPage)
	m_bUse3rdPartySourceControl = FALSE;
	m_bIncludeUserNameInCheckout = FALSE;
	//}}AFX_DATA_INIT
}

CPreferencesMultiUserPage::~CPreferencesMultiUserPage()
{
}

void CPreferencesMultiUserPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesMultiUserPage)
	DDX_Control(pDX, IDC_NOCHANGETIME, m_cbNoEditTime);
	DDX_Check(pDX, IDC_CHECKINONNOEDIT, m_bCheckinNoChange);
	DDX_Check(pDX, IDC_INCLUDEUSERINCHECKOUT, m_bIncludeUserNameInCheckout);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_REMOTEFILECHECK, m_cbRemoteFileCheck);
	DDX_Check(pDX, IDC_PROMPTRELOADONWRITABLE, m_bPromptReloadOnWritable);
	DDX_Check(pDX, IDC_PROMPTRELOADONCHANGE, m_bPromptReloadOnTimestamp);
	DDX_Check(pDX, IDC_AUTOCHECKOUT, m_bAutoCheckOut);
	DDX_Check(pDX, IDC_KEEPTRYINGTOCHECKOUT, m_bKeepTryingToCheckout);
	DDX_CBIndex(pDX, IDC_READONLYRELOADOPTION, m_nReadonlyReloadOption);
	DDX_CBIndex(pDX, IDC_TIMESTAMPRELOADOPTION, m_nTimestampReloadOption);
	DDX_Check(pDX, IDC_CHECKINONCLOSE, m_bCheckinOnClose);

	// custom
	if (pDX->m_bSaveAndValidate)
	{
		m_nRemoteFileCheckFreq = CDialogHelper::GetSelectedItemAsValue(m_cbRemoteFileCheck);
		m_nCheckinNoEditTime = CDialogHelper::GetSelectedItemAsValue(m_cbNoEditTime);
	}
	else
	{
		if (!CDialogHelper::SelectItemByValue(m_cbRemoteFileCheck, m_nRemoteFileCheckFreq))
		{
			m_nRemoteFileCheckFreq = 30;
			CDialogHelper::SelectItemByValue(m_cbRemoteFileCheck, m_nRemoteFileCheckFreq);
		}

		if (!CDialogHelper::SelectItemByValue(m_cbNoEditTime, m_nCheckinNoEditTime))
		{
			m_nCheckinNoEditTime = 10;
			CDialogHelper::SelectItemByValue(m_cbNoEditTime, m_nCheckinNoEditTime);
		}
	}
}


BEGIN_MESSAGE_MAP(CPreferencesMultiUserPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesMultiUserPage)
	ON_BN_CLICKED(IDC_CHECKINONNOEDIT, OnCheckinonnoedit)
	ON_BN_CLICKED(IDC_USE3RDPARTYSOURCECTRL, OnUse3rdpartysourcectrl)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PROMPTRELOADONWRITABLE, OnPromptreloadonwritable)
	ON_BN_CLICKED(IDC_PROMPTRELOADONCHANGE, OnPromptreloadontimestamp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesMultiUserPage message handlers

BOOL CPreferencesMultiUserPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	GetDlgItem(IDC_NOCHANGETIME)->EnableWindow(m_bCheckinNoChange);
	GetDlgItem(IDC_READONLYRELOADOPTION)->EnableWindow(m_bPromptReloadOnWritable);
	GetDlgItem(IDC_TIMESTAMPRELOADOPTION)->EnableWindow(m_bPromptReloadOnTimestamp);

	m_mgrGroupLines.AddGroupLine(IDC_CHECKGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_SSCGROUP, *this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CPreferencesMultiUserPage::GetReadonlyReloadOption() const
{ 
	if (!m_bPromptReloadOnWritable)
		return RO_NO;
	else
		return m_nReadonlyReloadOption + 1; 
}

int CPreferencesMultiUserPage::GetTimestampReloadOption() const 
{ 
	if (!m_bPromptReloadOnTimestamp)
		return RO_NO;
	else
		return m_nTimestampReloadOption + 1; 
}

void CPreferencesMultiUserPage::OnPromptreloadonwritable() 
{
	UpdateData();
	GetDlgItem(IDC_READONLYRELOADOPTION)->EnableWindow(m_bPromptReloadOnWritable);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesMultiUserPage::OnPromptreloadontimestamp() 
{
	UpdateData();
	GetDlgItem(IDC_TIMESTAMPRELOADOPTION)->EnableWindow(m_bPromptReloadOnTimestamp);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesMultiUserPage::OnCheckinonnoedit() 
{
	UpdateData();
	GetDlgItem(IDC_NOCHANGETIME)->EnableWindow(/*m_bEnableSourceControl && */m_bCheckinNoChange);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesMultiUserPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	m_bPromptReloadOnWritable = pPrefs->GetProfileInt(szKey, _T("PromptReloadOnWritable"), TRUE);
	m_bAutoCheckOut = pPrefs->GetProfileInt(szKey, _T("AutoCheckOut"), FALSE);
	m_bPromptReloadOnTimestamp = pPrefs->GetProfileInt(szKey, _T("PromptReloadOnTimestamp"), TRUE);
	m_bKeepTryingToCheckout = pPrefs->GetProfileInt(szKey, _T("CheckoutOnCheckin"), TRUE);
	m_nReadonlyReloadOption = pPrefs->GetProfileInt(szKey, _T("ReadonlyReloadOption"), RO_ASK) - 1;
	m_nTimestampReloadOption = pPrefs->GetProfileInt(szKey, _T("TimestampReloadOption"), RO_ASK) - 1;
	m_bCheckinOnClose = pPrefs->GetProfileInt(szKey, _T("CheckinOnClose"), TRUE);
	m_nCheckinNoEditTime = pPrefs->GetProfileInt(szKey, _T("CheckinNoEditTime"), 1);
	m_bCheckinNoChange = pPrefs->GetProfileInt(szKey, _T("CheckinNoEdit"), TRUE);
	m_bIncludeUserNameInCheckout = pPrefs->GetProfileInt(szKey, _T("IncludeUserNameInCheckout"), FALSE);
	m_bUse3rdPartySourceControl = /*!m_bEnableSourceControl && */pPrefs->GetProfileInt(szKey, _T("Use3rdPartySourceControl"), FALSE);

	m_nRemoteFileCheckFreq = pPrefs->GetProfileInt(szKey, _T("RemoteFileCheckFrequency"), 30);

	if (m_nRemoteFileCheckFreq < 10)
		m_nRemoteFileCheckFreq = 30;
}

void CPreferencesMultiUserPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileInt(szKey, _T("PromptReloadOnWritable"), m_bPromptReloadOnWritable);
	pPrefs->WriteProfileInt(szKey, _T("PromptReloadOnTimestamp"), m_bPromptReloadOnTimestamp);
	pPrefs->WriteProfileInt(szKey, _T("AutoCheckOut"), m_bAutoCheckOut);
	pPrefs->WriteProfileInt(szKey, _T("CheckoutOnCheckin"), m_bKeepTryingToCheckout);
	pPrefs->WriteProfileInt(szKey, _T("ReadonlyReloadOption"), m_nReadonlyReloadOption + 1);
	pPrefs->WriteProfileInt(szKey, _T("TimestampReloadOption"), m_nTimestampReloadOption + 1);
	pPrefs->WriteProfileInt(szKey, _T("CheckinOnClose"), m_bCheckinOnClose);
	pPrefs->WriteProfileInt(szKey, _T("RemoteFileCheckFrequency"), m_nRemoteFileCheckFreq);
	pPrefs->WriteProfileInt(szKey, _T("CheckinNoEditTime"), m_nCheckinNoEditTime);
	pPrefs->WriteProfileInt(szKey, _T("CheckinNoEdit"), m_bCheckinNoChange);
	pPrefs->WriteProfileInt(szKey, _T("IncludeUserNameInCheckout"), m_bIncludeUserNameInCheckout);
	pPrefs->WriteProfileInt(szKey, _T("Use3rdPartySourceControl"), m_bUse3rdPartySourceControl);
//	pPrefs->WriteProfileInt(szKey, _T(""), m_b);
}

void CPreferencesMultiUserPage::OnUse3rdpartysourcectrl() 
{
	UpdateData();

	GetDlgItem(IDC_AUTOCHECKOUT)->EnableWindow(!m_bUse3rdPartySourceControl);
	GetDlgItem(IDC_KEEPTRYINGTOCHECKOUT)->EnableWindow(!m_bUse3rdPartySourceControl);
	GetDlgItem(IDC_CHECKINONCLOSE)->EnableWindow(!m_bUse3rdPartySourceControl);
	GetDlgItem(IDC_CHECKINONNOEDIT)->EnableWindow(!m_bUse3rdPartySourceControl);
	GetDlgItem(IDC_INCLUDEUSERINCHECKOUT)->EnableWindow(!m_bUse3rdPartySourceControl);
	GetDlgItem(IDC_NOCHANGETIME)->EnableWindow(!m_bUse3rdPartySourceControl && m_bCheckinNoChange);
	
	CPreferencesPageBase::OnControlChange();
}
