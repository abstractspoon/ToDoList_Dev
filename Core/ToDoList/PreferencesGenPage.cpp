// PreferencesGenPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesGenPage.h"

#include "..\shared\OSVersion.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\webmisc.h"
#include "..\shared\fileregister.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef BCM_SETSHIELD
#	define BCM_SETSHIELD 0x0000160C
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR DEFAULT_STICKIES_PATH = _T("C:\\Program Files (x86)\\Stickies\\stickies.exe");

/////////////////////////////////////////////////////////////////////////////
// CPreferencesGenPage property page

IMPLEMENT_DYNCREATE(CPreferencesGenPage, CPreferencesPageBase)

CPreferencesGenPage::CPreferencesGenPage() : 
   CPreferencesPageBase(CPreferencesGenPage::IDD), 
   m_hkGlobal(TRUE), 
   m_cbLanguages(_T("*.csv")),
   m_eStickiesPath(FES_COMBOSTYLEBTN, CEnString(IDS_STICKIESEXEFILTER))
{
	//{{AFX_DATA_INIT(CPreferencesGenPage)
	//}}AFX_DATA_INIT
}

CPreferencesGenPage::~CPreferencesGenPage()
{
}

void CPreferencesGenPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesGenPage)
	DDX_Control(pDX, IDC_NOCHANGETIME, m_cbNoEditTime);
	DDX_Control(pDX, IDC_LANGUAGE, m_cbLanguages);
	DDX_Control(pDX, IDC_GLOBALHOTKEY, m_hkGlobal);
	DDX_Check(pDX, IDC_MULTIINSTANCE, m_bMultiInstance);
	DDX_Check(pDX, IDC_SHOWONSTARTUP, m_bShowOnStartup);
	DDX_CBIndex(pDX, IDC_SYSTRAYOPTION, m_nSysTrayOption);
	DDX_Check(pDX, IDC_TOGGLETRAYVISIBILITY, m_bToggleTrayVisibility);
	DDX_Check(pDX, IDC_SPECIFYGLOBALHOTKEY, m_bSpecifyGlobalHotkey);
	DDX_Check(pDX, IDC_ADDFILESTOMRU, m_bAddFilesToMRU);
	DDX_Check(pDX, IDC_ENABLETDLEXTENSION, m_bEnableTDLExtension);
	DDX_Check(pDX, IDC_CHECKFORUPDATES, m_bAutoCheckForUpdates);
	DDX_Check(pDX, IDC_ESCAPEMINIMIZES, m_bEscapeMinimizes);
	DDX_Check(pDX, IDC_ENABLETDLPROTOCOL, m_bEnableTDLProtocol);
	DDX_Check(pDX, IDC_ENABLEDELAYEDLOADING, m_bEnableDelayedLoading);
	DDX_Check(pDX, IDC_SAVEOTHERPASSWORDS, m_bSaveStoragePasswords);
	DDX_Check(pDX, IDC_MINIMIZEONNOEDIT, m_bMinimizeNoChange);
	DDX_Check(pDX, IDC_USESTICKIES, m_bUseStickies);
	DDX_Text(pDX, IDC_PATHTOSTICKIESEXE, m_sStickiesPath);
	DDX_Check(pDX, IDC_RELOADTASKLISTS, m_bReloadTasklists);
	DDX_Check(pDX, IDC_ENABLERTLINPUT, m_bEnableRTLInput);
	DDX_Control(pDX, IDC_PATHTOSTICKIESEXE, m_eStickiesPath);
	DDX_Text(pDX, IDC_TDLEXTENSIONHANDLER, m_sTDLExtensionHandler);
	DDX_Text(pDX, IDC_TDLPROTOCOLHANDLER, m_sTDLProtocolHandler);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_ALWAYSONTOP, m_bAlwaysOnTop);
	DDX_Check(pDX, IDC_USESYSTRAY, m_bUseSysTray);
	DDX_Check(pDX, IDC_CONFIRMDELETE, m_bConfirmDelete);
	DDX_Check(pDX, IDC_CONFIRMSAVEONEXIT, m_bConfirmSaveOnExit);
	DDX_Check(pDX, IDC_SHOWFULLPATHINSTICKY, m_bShowFullTaskPathInSticky);

	CDialogHelper::DDX_CBValue(pDX, m_cbNoEditTime, m_nMinimizeNoEditTime, 10);

	// custom
	if (pDX->m_bSaveAndValidate)
	{
		WORD wVKeyCode = 0, wModifiers = 0;
		m_hkGlobal.GetHotKey(wVKeyCode, wModifiers);
		m_dwGlobalHotkey = MAKELONG(wVKeyCode, wModifiers);
	}
	else
	{
		if (m_bSpecifyGlobalHotkey)
			m_hkGlobal.SetHotKey(m_dwGlobalHotkey);
	}
}

BEGIN_MESSAGE_MAP(CPreferencesGenPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesGenPage)
	ON_BN_CLICKED(IDC_SPECIFYGLOBALHOTKEY, OnSpecifyglobalhotkey)
	ON_BN_CLICKED(IDC_MULTIINSTANCE, OnMultiinstance)
	ON_BN_CLICKED(IDC_CLEARMRU, OnClearMRU)
	ON_CBN_SELCHANGE(IDC_LANGUAGE, OnSelchangeLanguage)
	ON_BN_CLICKED(IDC_MINIMIZEONNOEDIT, OnMinimizeonnoedit)
	ON_BN_CLICKED(IDC_USESTICKIES, OnUseStickies)
	ON_BN_CLICKED(IDC_EDIT_DISPLAYLANGUAGE, OnEditDisplayLanguage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_USESYSTRAY, OnUseSystray)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesGenPage message handlers

BOOL CPreferencesGenPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesGenPage::OnFirstShow()
{
	CPreferencesPageBase::OnFirstShow();

	m_sTDLExtensionHandler = CFileRegister::GetRegisteredAppPath(_T("tdl"));

	if (m_sTDLExtensionHandler.IsEmpty())
		m_sTDLExtensionHandler = CEnString(IDS_HANDLERNOTINSTALLED);

	m_sTDLProtocolHandler = WebMisc::GetRegisteredProtocolAppPath(_T("tdl"));
	
	if (m_sTDLProtocolHandler.IsEmpty())
		m_sTDLProtocolHandler = CEnString(IDS_HANDLERNOTINSTALLED);

	UpdateData(FALSE);

	GetDlgItem(IDC_SYSTRAYOPTION)->EnableWindow(m_bUseSysTray);
	GetDlgItem(IDC_TOGGLETRAYVISIBILITY)->EnableWindow(m_bUseSysTray);
	GetDlgItem(IDC_MULTIINSTANCE)->EnableWindow(!m_bSpecifyGlobalHotkey);
	GetDlgItem(IDC_SPECIFYGLOBALHOTKEY)->EnableWindow(!m_bMultiInstance);
	GetDlgItem(IDC_GLOBALHOTKEY)->EnableWindow(!m_bMultiInstance && m_bSpecifyGlobalHotkey);
	GetDlgItem(IDC_NOCHANGETIME)->EnableWindow(m_bMinimizeNoChange);
	GetDlgItem(IDC_STICKIESPATHLABEL)->EnableWindow(m_bUseStickies);
	GetDlgItem(IDC_PATHTOSTICKIESEXE)->EnableWindow(m_bUseStickies);
	GetDlgItem(IDC_SHOWFULLPATHINSTICKY)->EnableWindow(m_bUseStickies);

	// Add admin comment as required
	if (COSVersion() >= OSV_VISTA && !FileMisc::IsAdminProcess())
	{
		CString sCtrlText;
		UINT nID[2] = {IDC_ENABLETDLEXTENSION, IDC_ENABLETDLPROTOCOL};

		for (int nItem = 0; nItem < 2; nItem++)
		{
			CWnd* pCtrl = GetDlgItem(nID[nItem]);
			pCtrl->GetWindowText(sCtrlText);

			sCtrlText.Format(_T("%s (%s)"), sCtrlText, CEnString(IDS_REQUIRESADMINTOMODIFY));
			pCtrl->SetWindowText(sCtrlText);

			ResizeButtonStaticTextToFit(this, pCtrl);

			pCtrl->EnableWindow(FALSE);
		}
	}

	EnableDisableLanguageOptions();

	// Show/Enable 'Edit' button only if the initial language is set
	if (m_cbLanguages.IsDefaultLanguageSelected())
	{
		GetDlgItem(IDC_EDIT_DISPLAYLANGUAGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_DISPLAYLANGUAGE)->EnableWindow(FALSE);
	}
}

void CPreferencesGenPage::OnUseSystray() 
{
	UpdateData();

	GetDlgItem(IDC_SYSTRAYOPTION)->EnableWindow(m_bUseSysTray);
	GetDlgItem(IDC_TOGGLETRAYVISIBILITY)->EnableWindow(m_bUseSysTray);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesGenPage::OnSpecifyglobalhotkey() 
{
	UpdateData();
	
	GetDlgItem(IDC_GLOBALHOTKEY)->EnableWindow(m_bSpecifyGlobalHotkey);
	GetDlgItem(IDC_MULTIINSTANCE)->EnableWindow(!m_bSpecifyGlobalHotkey);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesGenPage::OnMultiinstance() 
{
	UpdateData();
	
	GetDlgItem(IDC_SPECIFYGLOBALHOTKEY)->EnableWindow(!m_bMultiInstance);
	GetDlgItem(IDC_GLOBALHOTKEY)->EnableWindow(!m_bMultiInstance && m_bSpecifyGlobalHotkey);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesGenPage::OnClearMRU() 
{
   GetParent()->SendMessage(WM_PGP_CLEARMRU);
}

void CPreferencesGenPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	// load settings
	m_bAlwaysOnTop = pPrefs->GetProfileInt(szKey, _T("AlwaysOnTop"), FALSE);
	m_bUseSysTray = pPrefs->GetProfileInt(szKey, _T("UseSysTray"), FALSE);
	m_bConfirmDelete = pPrefs->GetProfileInt(szKey, _T("ConfirmDelete"), FALSE);
	m_bConfirmSaveOnExit = pPrefs->GetProfileInt(szKey, _T("ConfirmSaveOnExit"), TRUE);
	m_bMultiInstance = pPrefs->GetProfileInt(szKey, _T("MultiInstance"), FALSE);
	m_bShowOnStartup = pPrefs->GetProfileInt(szKey, _T("ShowOnStartup"), TRUE);
	m_nSysTrayOption = pPrefs->GetProfileInt(szKey, _T("SysTrayOption"), STO_ONCLOSE);
	m_bToggleTrayVisibility = pPrefs->GetProfileInt(szKey, _T("ToggleTrayVisibility"), FALSE);
	m_dwGlobalHotkey = pPrefs->GetProfileInt(szKey, _T("GlobalHotkey"), 0);
	m_bSpecifyGlobalHotkey = (m_dwGlobalHotkey ? 1 : 0);
	m_bAddFilesToMRU = pPrefs->GetProfileInt(szKey, _T("AddFilesToMRU"), TRUE);
	m_bAutoCheckForUpdates = pPrefs->GetProfileInt(szKey, _T("AutoCheckForUpdates"), FALSE);
	m_bEscapeMinimizes = pPrefs->GetProfileInt(szKey, _T("EscapeMinimizes"), FALSE);
	m_bEnableDelayedLoading = pPrefs->GetProfileInt(szKey, _T("EnableDelayedLoading"), TRUE);
	m_bSaveStoragePasswords = pPrefs->GetProfileInt(szKey, _T("SaveStoragePasswords"), FALSE);
	m_nMinimizeNoEditTime = pPrefs->GetProfileInt(szKey, _T("MinimizeNoEditTime"), 5);
	m_bMinimizeNoChange = pPrefs->GetProfileInt(szKey, _T("MinimizeNoEdit"), FALSE);
	m_bReloadTasklists = pPrefs->GetProfileInt(szKey, _T("ReloadTasklists"), TRUE);
	m_bEnableRTLInput = pPrefs->GetProfileInt(szKey, _T("EnableRTLInput"), FALSE);
	m_bEnableTDLExtension = pPrefs->GetProfileInt(szKey, _T("EnableTDLExtension"), TRUE);
	m_bEnableTDLProtocol = pPrefs->GetProfileInt(szKey, _T("EnableTDLProtocol"), FALSE);

	CString sLangFile = pPrefs->GetProfileString(szKey, _T("LanguageFile"), _T(""));
	m_cbLanguages.SelectLanguageFile(sLangFile);

	m_bUseStickies = pPrefs->GetProfileInt(szKey, _T("UseStickies"), FALSE);
	m_bShowFullTaskPathInSticky = pPrefs->GetProfileInt(szKey, _T("ShowFullTaskPathInSticky"), FALSE);
	m_sStickiesPath = pPrefs->GetProfileString(szKey, _T("PathToStickies"));

	if (m_sStickiesPath.IsEmpty() && FileMisc::FileExists(DEFAULT_STICKIES_PATH))
		m_sStickiesPath = DEFAULT_STICKIES_PATH;

//	m_b = pPrefs->GetProfileInt(szKey, _T(""), TRUE);
}

void CPreferencesGenPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileInt(szKey, _T("AlwaysOnTop"), m_bAlwaysOnTop);
	pPrefs->WriteProfileInt(szKey, _T("UseSysTray"), m_bUseSysTray);
	pPrefs->WriteProfileInt(szKey, _T("ConfirmDelete"), m_bConfirmDelete);
	pPrefs->WriteProfileInt(szKey, _T("ConfirmSaveOnExit"), m_bConfirmSaveOnExit);
	pPrefs->WriteProfileInt(szKey, _T("ShowOnStartup"), m_bShowOnStartup);
	pPrefs->WriteProfileInt(szKey, _T("SysTrayOption"), m_nSysTrayOption);
	pPrefs->WriteProfileInt(szKey, _T("ToggleTrayVisibility"), m_bToggleTrayVisibility);
	pPrefs->WriteProfileInt(szKey, _T("MultiInstance"), m_bMultiInstance);
	pPrefs->WriteProfileInt(szKey, _T("GlobalHotkey"), (m_bSpecifyGlobalHotkey ? m_dwGlobalHotkey : 0));
	pPrefs->WriteProfileInt(szKey, _T("AddFilesToMRU"), m_bAddFilesToMRU);
	pPrefs->WriteProfileInt(szKey, _T("AutoCheckForUpdates"), m_bAutoCheckForUpdates);
	pPrefs->WriteProfileInt(szKey, _T("EscapeMinimizes"), m_bEscapeMinimizes);
	pPrefs->WriteProfileInt(szKey, _T("EnableDelayedLoading"), m_bEnableDelayedLoading);
	pPrefs->WriteProfileInt(szKey, _T("SaveStoragePasswords"), m_bSaveStoragePasswords);
	pPrefs->WriteProfileInt(szKey, _T("MinimizeNoEditTime"), m_nMinimizeNoEditTime);
	pPrefs->WriteProfileInt(szKey, _T("MinimizeNoEdit"), m_bMinimizeNoChange);
	pPrefs->WriteProfileInt(szKey, _T("ReloadTasklists"), m_bReloadTasklists);
	pPrefs->WriteProfileInt(szKey, _T("EnableRTLInput"), m_bEnableRTLInput);
	pPrefs->WriteProfileInt(szKey, _T("EnableTDLExtension"), m_bEnableTDLExtension);
	pPrefs->WriteProfileInt(szKey, _T("EnableTDLProtocol"), m_bEnableTDLProtocol);

	pPrefs->WriteProfileInt(szKey, _T("UseStickies"), m_bUseStickies);
	pPrefs->WriteProfileInt(szKey, _T("ShowFullTaskPathInSticky"), m_bShowFullTaskPathInSticky);

	pPrefs->WriteProfileString(szKey, _T("LanguageFile"), m_cbLanguages.GetSelectedLanguageFile(TRUE)); // relative path
	pPrefs->WriteProfileString(szKey, _T("PathToStickies"), m_sStickiesPath);

	//	pPrefs->WriteProfileInt(szKey, _T(""), m_b);
}

void CPreferencesGenPage::OnSelchangeLanguage() 
{
	EnableDisableLanguageOptions();

	// Turn on RTL support for RTL languages only
	BOOL bEnable = FALSE;

	switch (m_cbLanguages.GetSelectedPrimaryLanguageID())
	{
	case LANG_HEBREW:
	case LANG_ARABIC:
	case LANG_URDU:
		bEnable = TRUE;
		break;
	}

	if (m_bEnableRTLInput != bEnable)
	{
		m_bEnableRTLInput = bEnable;
		UpdateData(FALSE);
	}
}

void CPreferencesGenPage::EnableDisableLanguageOptions()
{
	// Enable RTL support for RTL languages only
	BOOL bEnable = FALSE;

	switch (m_cbLanguages.GetSelectedPrimaryLanguageID())
	{
	case LANG_HEBREW:
	case LANG_ARABIC:
	case LANG_URDU:
		bEnable = TRUE;
		break;
	}

	GetDlgItem(IDC_ENABLERTLINPUT)->EnableWindow(bEnable);
}

void CPreferencesGenPage::OnMinimizeonnoedit() 
{
	UpdateData();
	GetDlgItem(IDC_NOCHANGETIME)->EnableWindow(m_bMinimizeNoChange);
}

void CPreferencesGenPage::OnUseStickies() 
{
	UpdateData();

	GetDlgItem(IDC_STICKIESPATHLABEL)->EnableWindow(m_bUseStickies);
	GetDlgItem(IDC_PATHTOSTICKIESEXE)->EnableWindow(m_bUseStickies);
	GetDlgItem(IDC_SHOWFULLPATHINSTICKY)->EnableWindow(m_bUseStickies);
}

BOOL CPreferencesGenPage::GetUseStickies(CString& sStickiesPath, BOOL& bShowFullTaskPath) const
{ 
	if (m_bUseStickies && FileMisc::FileExists(m_sStickiesPath))
	{
		sStickiesPath = m_sStickiesPath;
		bShowFullTaskPath = m_bShowFullTaskPathInSticky;

		return TRUE;
	}
	
	// else
	return FALSE; 
}

void CPreferencesGenPage::OnEditDisplayLanguage() 
{
	GetParent()->SendMessage(WM_PGP_EDITLANGFILE);
}

BOOL CPreferencesGenPage::HasSysTrayOptions(int nOption1, int nOption2) const
{
	int nSysOpt = GetSysTrayOption();

	if (nSysOpt == nOption1)
	{
		return TRUE;
	}
	else if (nOption2 != STO_NONE)
	{
		return (nSysOpt == nOption2);
	}

	// all else
	return FALSE;
}

