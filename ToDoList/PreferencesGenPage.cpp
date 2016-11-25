// PreferencesGenPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesGenPage.h"

#include "..\shared\OSVersion.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef BCM_SETSHIELD
#	define BCM_SETSHIELD 0x0000160C
#endif
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
	DDX_Control(pDX, IDC_VISTASHIELD, m_stVistaShield);
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
	DDX_Control(pDX, IDC_PATHTOSTICKIESEXE, m_eStickiesPath);
	DDX_Check(pDX, IDC_ENABLERTLINPUT, m_bEnableRTLInput);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_ALWAYSONTOP, m_bAlwaysOnTop);
	DDX_Check(pDX, IDC_USESYSTRAY, m_bUseSysTray);
	DDX_Check(pDX, IDC_CONFIRMDELETE, m_bConfirmDelete);
	DDX_Check(pDX, IDC_CONFIRMSAVEONEXIT, m_bConfirmSaveOnExit);

	// custom
	if (pDX->m_bSaveAndValidate)
	{
		WORD wVKeyCode = 0, wModifiers = 0;
		m_hkGlobal.GetHotKey(wVKeyCode, wModifiers);
		m_dwGlobalHotkey = MAKELONG(wVKeyCode, wModifiers);

		m_nMinimizeNoEditTime = CDialogHelper::GetSelectedItemAsValue(m_cbNoEditTime);
	}
	else
	{
		if (m_bSpecifyGlobalHotkey)
			m_hkGlobal.SetHotKey(m_dwGlobalHotkey);

		if (!CDialogHelper::SelectItemByValue(m_cbNoEditTime, m_nMinimizeNoEditTime))
		{
			m_nMinimizeNoEditTime = 10;
			CDialogHelper::SelectItemByValue(m_cbNoEditTime, m_nMinimizeNoEditTime);
		}
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
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_USESYSTRAY, OnUseSystray)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesGenPage message handlers

BOOL CPreferencesGenPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	GetDlgItem(IDC_SYSTRAYOPTION)->EnableWindow(m_bUseSysTray);
	GetDlgItem(IDC_TOGGLETRAYVISIBILITY)->EnableWindow(m_bUseSysTray);
	GetDlgItem(IDC_MULTIINSTANCE)->EnableWindow(!m_bSpecifyGlobalHotkey);
	GetDlgItem(IDC_SPECIFYGLOBALHOTKEY)->EnableWindow(!m_bMultiInstance);
	GetDlgItem(IDC_GLOBALHOTKEY)->EnableWindow(!m_bMultiInstance && m_bSpecifyGlobalHotkey);
	GetDlgItem(IDC_NOCHANGETIME)->EnableWindow(m_bMinimizeNoChange);
	GetDlgItem(IDC_STICKIESPATHLABEL)->EnableWindow(m_bUseStickies);
	GetDlgItem(IDC_PATHTOSTICKIESEXE)->EnableWindow(m_bUseStickies);

	// OS constraints
	OSVERSION nOS = COSVersion();

	GetDlgItem(IDC_VISTASHIELD)->ShowWindow((nOS >= OSV_VISTA) || (nOS != OSV_LINUX));
	GetDlgItem(IDC_VISTASHIELD2)->ShowWindow((nOS >= OSV_VISTA) || (nOS != OSV_LINUX));
	GetDlgItem(IDC_ENABLETDLEXTENSION)->EnableWindow(nOS != OSV_LINUX);
	GetDlgItem(IDC_ENABLETDLPROTOCOL)->EnableWindow(nOS != OSV_LINUX);

	EnableDisableLanguageOptions();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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

void CPreferencesGenPage::LoadPreferences(const IPreferences* pPrefs)
{
	// load settings
	m_bAlwaysOnTop = pPrefs->GetProfileInt(_T("Preferences"), _T("AlwaysOnTop"), FALSE);
	m_bUseSysTray = pPrefs->GetProfileInt(_T("Preferences"), _T("UseSysTray"), FALSE);
	m_bConfirmDelete = pPrefs->GetProfileInt(_T("Preferences"), _T("ConfirmDelete"), FALSE);
	m_bConfirmSaveOnExit = pPrefs->GetProfileInt(_T("Preferences"), _T("ConfirmSaveOnExit"), TRUE);
	m_bMultiInstance = pPrefs->GetProfileInt(_T("Preferences"), _T("MultiInstance"), FALSE);
	m_bShowOnStartup = pPrefs->GetProfileInt(_T("Preferences"), _T("ShowOnStartup"), TRUE);
	m_nSysTrayOption = pPrefs->GetProfileInt(_T("Preferences"), _T("SysTrayOption"), STO_ONCLOSE);
	m_bToggleTrayVisibility = pPrefs->GetProfileInt(_T("Preferences"), _T("ToggleTrayVisibility"), FALSE);
	m_dwGlobalHotkey = pPrefs->GetProfileInt(_T("Preferences"), _T("GlobalHotkey"), 0);
	m_bSpecifyGlobalHotkey = (m_dwGlobalHotkey ? 1 : 0);
	m_bAddFilesToMRU = pPrefs->GetProfileInt(_T("Preferences"), _T("AddFilesToMRU"), TRUE);
	m_bAutoCheckForUpdates = pPrefs->GetProfileInt(_T("Preferences"), _T("AutoCheckForUpdates"), FALSE);
	m_bEscapeMinimizes = pPrefs->GetProfileInt(_T("Preferences"), _T("EscapeMinimizes"), FALSE);
	m_bEnableDelayedLoading = pPrefs->GetProfileInt(_T("Preferences"), _T("EnableDelayedLoading"), TRUE);
	m_bSaveStoragePasswords = pPrefs->GetProfileInt(_T("Preferences"), _T("SaveStoragePasswords"), FALSE);
	m_nMinimizeNoEditTime = pPrefs->GetProfileInt(_T("Preferences"), _T("MinimizeNoEditTime"), 5);
	m_bMinimizeNoChange = pPrefs->GetProfileInt(_T("Preferences"), _T("MinimizeNoEdit"), FALSE);
	m_bReloadTasklists = pPrefs->GetProfileInt(_T("Preferences"), _T("ReloadTasklists"), TRUE);
	m_bEnableRTLInput = pPrefs->GetProfileInt(_T("Preferences"), _T("EnableRTLInput"), FALSE);

	CString sLangFile = pPrefs->GetProfileString(_T("Preferences"), _T("LanguageFile"), _T(""));
	m_cbLanguages.SelectLanguageFile(sLangFile);

	m_bUseStickies = pPrefs->GetProfileInt(_T("Preferences"), _T("UseStickies"), FALSE);
	m_sStickiesPath = pPrefs->GetProfileString(_T("Preferences"), _T("PathToStickies"));

	if (COSVersion() == OSV_LINUX)
	{
		m_bEnableTDLExtension = m_bEnableTDLProtocol = FALSE;
	}
	else
	{
		m_bEnableTDLExtension = pPrefs->GetProfileInt(_T("Preferences"), _T("EnableTDLExtension"), TRUE);
		m_bEnableTDLProtocol = pPrefs->GetProfileInt(_T("Preferences"), _T("EnableTDLProtocol"), FALSE);
	}

//	m_b = pPrefs->GetProfileInt(_T("Preferences"), _T(""), TRUE);
}

void CPreferencesGenPage::SavePreferences(IPreferences* pPrefs)
{
	// save settings
	pPrefs->WriteProfileInt(_T("Preferences"), _T("AlwaysOnTop"), m_bAlwaysOnTop);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("UseSysTray"), m_bUseSysTray);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("ConfirmDelete"), m_bConfirmDelete);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("ConfirmSaveOnExit"), m_bConfirmSaveOnExit);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("ShowOnStartup"), m_bShowOnStartup);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("SysTrayOption"), m_nSysTrayOption);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("ToggleTrayVisibility"), m_bToggleTrayVisibility);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("MultiInstance"), m_bMultiInstance);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("GlobalHotkey"), (m_bSpecifyGlobalHotkey ? m_dwGlobalHotkey : 0));
	pPrefs->WriteProfileInt(_T("Preferences"), _T("AddFilesToMRU"), m_bAddFilesToMRU);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("AutoCheckForUpdates"), m_bAutoCheckForUpdates);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("EscapeMinimizes"), m_bEscapeMinimizes);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("EnableDelayedLoading"), m_bEnableDelayedLoading);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("SaveStoragePasswords"), m_bSaveStoragePasswords);
	pPrefs->WriteProfileString(_T("Preferences"), _T("LanguageFile"), m_cbLanguages.GetSelectedLanguageFile(TRUE)); // relative path
	pPrefs->WriteProfileInt(_T("Preferences"), _T("MinimizeNoEditTime"), m_nMinimizeNoEditTime);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("MinimizeNoEdit"), m_bMinimizeNoChange);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("UseStickies"), m_bUseStickies);
	pPrefs->WriteProfileString(_T("Preferences"), _T("PathToStickies"), m_sStickiesPath);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("ReloadTasklists"), m_bReloadTasklists);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("EnableRTLInput"), m_bEnableRTLInput);

	if (COSVersion() != OSV_LINUX)
	{
		pPrefs->WriteProfileInt(_T("Preferences"), _T("EnableTDLExtension"), m_bEnableTDLExtension);
		pPrefs->WriteProfileInt(_T("Preferences"), _T("EnableTDLProtocol"), m_bEnableTDLProtocol);
	}

	//	pPrefs->WriteProfileInt(_T("Preferences"), _T(""), m_b);
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
}

BOOL CPreferencesGenPage::GetUseStickies(CString& sStickiesPath) const
{ 
	if (m_bUseStickies && FileMisc::FileExists(m_sStickiesPath))
	{
		sStickiesPath = m_sStickiesPath;
		return TRUE;
	}
	
	// else
	return FALSE; 
}
