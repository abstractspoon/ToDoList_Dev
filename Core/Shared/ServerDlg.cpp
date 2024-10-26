// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServerDlg.h"
#include "regkey.h"
#include "enstring.h"
#include "localizer.h"
#include "misc.h"
#include "webmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CMap<UINT, UINT, CString, CString&> CServerDlg::s_mapText;

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

CServerDlg::CServerDlg(LPCTSTR szServer, LPCTSTR szUsername, LPCTSTR szPassword, AL_TYPE nAnonymousLogin, HICON hIcon)
	:
	m_sServer(szServer),
	m_sUsername(szUsername),
	m_sPassword(szPassword),
	m_nAnonLogin(nAnonymousLogin),
	m_pPrefs(NULL),
	m_hIcon(hIcon)
{
	AddRCControl(_T("LTEXT"), _T(""), _T("Server"), 0, 0, 7, 9, 57, 8, IDC_SD_SERVERLABEL);
	AddRCControl(_T("COMBOBOX"), _T(""), _T(""), CBS_DROPDOWN | WS_VSCROLL | WS_TABSTOP | CBS_AUTOHSCROLL, 0, 65, 7, 134, 100, IDC_SD_SERVER);

	AddRCControl(_T("LTEXT"), _T(""), _T("Username"), 0, 0, 7, 26, 57, 8, IDC_SD_USERNAMELABEL);
	AddRCControl(_T("COMBOBOX"), _T(""), _T(""), CBS_DROPDOWN | WS_VSCROLL | WS_TABSTOP, 0, 65, 24, 134, 100, IDC_SD_USERNAME);

	AddRCControl(_T("LTEXT"), _T(""), _T("Password"), 0, 0, 7, 43, 57, 8, IDC_SD_PASSWORDLABEL);
	AddRCControl(_T("EDITTEXT"), _T(""), _T(""), ES_PASSWORD | ES_AUTOHSCROLL | WS_TABSTOP, 0, 65, 41, 134, 13, IDC_SD_PASSWORD);

	BOOL bShowAnonLogin = (m_nAnonLogin != ANONLOGIN_HIDE);
	int YOFFSET = 0;

	if (bShowAnonLogin)
	{
		AddRCControl(_T("CONTROL"), _T("Button"), _T("Anonymous Login"), BS_AUTOCHECKBOX | WS_TABSTOP, 0, 65, 57, 134, 10, IDC_SD_ANONLOGIN);
		YOFFSET = 20;
	}

	AddRCControl(_T("LTEXT"), _T(""), _T("Proxy"), 0, 0, 7, 54 + YOFFSET, 57, 8, IDC_SD_PROXYLABEL);
	AddRCControl(_T("EDITTEXT"), _T(""), _T(""), ES_AUTOHSCROLL | WS_TABSTOP, 0, 65, 52 + YOFFSET, 80, 13, IDC_SD_PROXY);
	AddRCControl(_T("LTEXT"), _T(""), _T("Port"), 0, 0, 149, 54 + YOFFSET, 50, 8, IDC_SD_PROXYPORTLABEL);
	AddRCControl(_T("EDITTEXT"), _T(""), _T(""), ES_NUMBER | ES_AUTOHSCROLL | WS_TABSTOP, 0, 171, 52 + YOFFSET, 28, 13, IDC_SD_PROXYPORT);
	
	// Divider
	AddRCControl(_T("CONTROL"), _T("Static"), _T(""), SS_ETCHEDHORZ, 0, 7, 72 + YOFFSET, 194, 1, (UINT)IDC_STATIC);

	AddRCControl(_T("DEFPUSHBUTTON"), _T(""), _T("OK"), WS_TABSTOP, 0, 91, 80 + YOFFSET, 50, 14, IDOK);
	AddRCControl(_T("PUSHBUTTON"), _T(""), _T("Cancel"), WS_TABSTOP, 0, 150, 80 + YOFFSET, 50, 14, IDCANCEL);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_SD_PASSWORD, m_sPassword);
	DDX_Text(pDX, IDC_SD_PROXY, m_sProxy);
	DDX_Text(pDX, IDC_SD_PROXYPORT, m_nProxyPort);
	DDX_Control(pDX, IDC_SD_SERVER, m_cbServers);
	DDX_Control(pDX, IDC_SD_USERNAME, m_cbUsernames);

	if (m_nAnonLogin >= ANONLOGIN_NO)
		DDX_Check(pDX, IDC_SD_ANONLOGIN, (int&)m_nAnonLogin);

	if (pDX->m_bSaveAndValidate)
	{
		DDX_CBString(pDX, IDC_SD_SERVER, m_sServer);
		DDX_CBString(pDX, IDC_SD_USERNAME, m_sUsername);

		Misc::Trim(m_sServer);
		Misc::Trim(m_sUsername);
		Misc::Trim(m_sPassword);
	}
	else
	{
		// Combos must do exact matches
		SelectItemExact(m_cbServers, m_sServer);
		SelectItemExact(m_cbUsernames, m_sUsername);
	}
}

BEGIN_MESSAGE_MAP(CServerDlg, CRuntimeDlg)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_CBN_EDITCHANGE(IDC_SD_SERVER, OnChangeServer)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SD_ANONLOGIN, OnAnonLogin)
	ON_EN_CHANGE(IDC_SD_PROXY, OnChangeProxy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

void CServerDlg::OnChangeServer() 
{
	UpdateData();
	GetDlgItem(IDOK)->EnableWindow(!m_sServer.IsEmpty());
}

void CServerDlg::OnChangeProxy()
{
	UpdateData();
	EnableDisableProxy();
}

void CServerDlg::EnableDisableProxy()
{
	GetDlgItem(IDC_SD_PROXYPORTLABEL)->EnableWindow(!m_sProxy.IsEmpty());
	GetDlgItem(IDC_SD_PROXYPORT)->EnableWindow(!m_sProxy.IsEmpty());
}

BOOL CServerDlg::OnInitDialog() 
{
	CRuntimeDlg::OnInitDialog();

	CLocalizer::EnableTranslation(m_cbUsernames, FALSE);
	CLocalizer::EnableTranslation(m_cbServers, FALSE);

	SetIcon(m_hIcon, FALSE);

	// initialize state from prefs
	if (m_sServer.IsEmpty())
		m_sServer = m_pPrefs->GetProfileString(m_sPrefKey, _T("LastServer"));

	if (m_sUsername.IsEmpty())
		m_sUsername = m_pPrefs->GetProfileString(m_sPrefKey, _T("LastUsername"));

	if (m_nAnonLogin == ANONLOGIN_AUTO)
		m_nAnonLogin = m_pPrefs->GetProfileInt(m_sPrefKey, _T("LastAnonLogin"), ANONLOGIN_NO) ? ANONLOGIN_YES : ANONLOGIN_NO;

	m_sProxy = m_pPrefs->GetProfileString(m_sPrefKey, _T("Proxy"));
	m_nProxyPort = m_pPrefs->GetProfileInt(m_sPrefKey, _T("ProxyPort"), 80);
	
	// if the proxy settings are blank, try to get them from the registry
	if (HasProxySettings())
		WebMisc::GetProxySettings(m_sProxy, m_nProxyPort);

	m_wpProxy.Initialize(*GetDlgItem(IDC_SD_PROXY), CEnString(_T("<optional>")), WM_GETTEXTLENGTH);

	UpdateData(FALSE);

	// populate comboboxes from prefs
	PopulateComboHistory(m_cbServers, _T("ServerCount"), _T("Server%d"), m_sServer);
	PopulateComboHistory(m_cbUsernames, _T("UsernameCount"), _T("Username%d"), m_sUsername);

	EnableDisableProxy();
	OnChangeServer();
	OnAnonLogin();

	// If the server and usernames are not empty set the focus to the password field
	if (!m_sServer.IsEmpty() && !m_sUsername.IsEmpty() && !GetAnonymousLogin())
	{
		GetDlgItem(IDC_SD_PASSWORD)->SetFocus();
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerDlg::PopulateComboHistory(CComboBox& combo, LPCTSTR szCountKey, LPCTSTR szItemTemplate, LPCTSTR szCurrentValue) const
{
	int nItem = m_pPrefs->GetProfileInt(m_sPrefKey, szCountKey, 0);

	while (nItem--)
	{
		CString sKey, sItem;

		sKey.Format(szItemTemplate, nItem);
		sItem = m_pPrefs->GetProfileString(m_sPrefKey, sKey);

		if (!sItem.IsEmpty() && combo.FindStringExact(-1, sItem) == CB_ERR)
			combo.InsertString(0, sItem);
	}

	// add current value as appropriate and select
	if (!Misc::IsEmpty(szCurrentValue) && (combo.FindStringExact(-1, szCurrentValue) == CB_ERR))
		combo.InsertString(0, szCurrentValue);

	SelectItemExact(combo, szCurrentValue);
}

int CServerDlg::DoModal(IPreferences* pPrefs, LPCTSTR szKey) 
{ 
	m_pPrefs = pPrefs;
	m_sPrefKey = szKey;

	return CRuntimeDlg::DoModal(GetItemText(SD_TITLE, _T("Remote Server Details"))); 
}

BOOL CServerDlg::HasProxySettings() const
{
	return (!m_sProxy.IsEmpty() && (m_nProxyPort > 0));
}

void CServerDlg::OnOK() 
{
	CRuntimeDlg::OnOK();

	// save server list to registry and last selected item
	int nServer = m_cbServers.GetCount();

	m_pPrefs->WriteProfileInt(m_sPrefKey, _T("ServerCount"), nServer);

	while (nServer--)
	{
		CString sServer, sItem;

		sItem.Format(_T("Server%d"), nServer);
		m_cbServers.GetLBText(nServer, sServer);

		m_pPrefs->WriteProfileString(m_sPrefKey, sItem, sServer);
	}

	m_pPrefs->WriteProfileString(m_sPrefKey, _T("LastServer"), m_sServer);
	m_pPrefs->WriteProfileString(m_sPrefKey, _T("Proxy"), m_sProxy);
	m_pPrefs->WriteProfileInt(m_sPrefKey, _T("ProxyPort"), m_nProxyPort);
	
	// save username list to registry and last selected item
	int nName = m_cbUsernames.GetCount();

	m_pPrefs->WriteProfileInt(m_sPrefKey, _T("UsernameCount"), nName);

	while (nName--)
	{
		CString sName, sItem;

		sItem.Format(_T("Username%d"), nName);
		m_cbUsernames.GetLBText(nName, sName);

		if (!sName.IsEmpty())
			m_pPrefs->WriteProfileString(m_sPrefKey, sItem, sName);
	}

	m_pPrefs->WriteProfileString(m_sPrefKey, _T("LastUsername"), m_sUsername);
	m_pPrefs->WriteProfileInt(m_sPrefKey, _T("LastAnonLogin"), m_nAnonLogin);
}

void CServerDlg::OnAnonLogin()
{
	UpdateData();

	if (m_nAnonLogin != ANONLOGIN_HIDE)
	{
		GetDlgItem(IDC_SD_USERNAME)->EnableWindow(m_nAnonLogin == ANONLOGIN_NO);
		GetDlgItem(IDC_SD_PASSWORD)->EnableWindow(m_nAnonLogin == ANONLOGIN_NO);
	}
}

CString CServerDlg::GetItemText(UINT nIDItem, LPCTSTR szDefault)
{
	CString sText;
	s_mapText.Lookup(nIDItem, sText);
	
	if (sText.IsEmpty() && szDefault)
		return szDefault;

	return sText;
}

void CServerDlg::SetItemText(UINT nIDItem, UINT nIDText)
{
	s_mapText[nIDItem] = CEnString(nIDText);
}


