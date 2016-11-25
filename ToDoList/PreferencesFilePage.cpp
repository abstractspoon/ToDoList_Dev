// PreferencesFilePage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesFilePage.h"

#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\importexportmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesFilePage property page

IMPLEMENT_DYNCREATE(CPreferencesFilePage, CPreferencesPageBase)

CPreferencesFilePage::CPreferencesFilePage() : CPreferencesPageBase(CPreferencesFilePage::IDD),
		m_eDueTaskStylesheet(FES_COMBOSTYLEBTN | FES_RELATIVEPATHS, CEnString(IDS_XSLFILEFILTER))
{
//	m_psp.dwFlags &= ~PSP_HASHELP;

	//{{AFX_DATA_INIT(CPreferencesFilePage)
	//}}AFX_DATA_INIT
}

CPreferencesFilePage::~CPreferencesFilePage()
{
}

void CPreferencesFilePage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesFilePage)
	DDX_Control(pDX, IDC_DUETASKSTYLESHEET, m_eDueTaskStylesheet);
	DDX_Check(pDX, IDC_DISPLAYDUETASKSINHTML, m_bDisplayDueTasksInHtml);
	DDX_Check(pDX, IDC_REFRESHFINDTASKS, m_bRefreshFindOnLoad);
	DDX_Check(pDX, IDC_DISPLAYDUETASKTITLESONLY, m_bDueTaskTitlesOnly);
	DDX_Text(pDX, IDC_DUETASKSTYLESHEET, m_sDueTasksStylesheet);
	DDX_Check(pDX, IDC_USESTYLESHEETFORDUEITEMS, m_bUseStyleSheetForDueTasks);
	DDX_Check(pDX, IDC_ONLYSHOWDUETASKFORPERSON, m_bOnlyShowDueTasksForPerson);
	DDX_Text(pDX, IDC_DUETASKPERSON, m_sDueTaskPerson);
	DDX_Check(pDX, IDC_WARNADDDELARCHIVE, m_bWarnAddDeleteArchive);
	DDX_Check(pDX, IDC_DONTREMOVEFLAGGED, m_bDontRemoveFlagged);
	DDX_Check(pDX, IDC_EXPANDTASKS, m_bExpandTasks);
	DDX_Check(pDX, IDC_DISPLAYDUETASKSIMPLECOMMENTS, m_bDisplayDueCommentsInText);
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_NOTIFYDUEBYONLOAD, m_nNotifyDueByOnLoad);
	DDX_CBIndex(pDX, IDC_NOTIFYDUEBYONSWITCH, m_nNotifyDueByOnSwitch);
	DDX_Check(pDX, IDC_NOTIFYDUEONLOAD, m_bNotifyDueOnLoad);
	DDX_Check(pDX, IDC_NOTIFYDUEONSWITCH, m_bNotifyDueOnSwitch);
	DDX_Check(pDX, IDC_NOTIFYREADONLY, m_bNotifyReadOnly);
	DDX_Check(pDX, IDC_AUTOARCHIVE, m_bAutoArchive);
	DDX_Check(pDX, IDC_REMOVEARCHIVEDITEMS, m_bRemoveArchivedTasks);
	DDX_Check(pDX, IDC_REMOVEONLYONABSCOMPLETION, m_bRemoveOnlyOnAbsoluteCompletion);
}


BEGIN_MESSAGE_MAP(CPreferencesFilePage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesFilePage)
	ON_BN_CLICKED(IDC_NOTIFYDUEONLOAD, OnNotifyDueOnLoad)
	ON_BN_CLICKED(IDC_DISPLAYDUETASKSINHTML, OnDisplayduetasksinhtml)
	ON_BN_CLICKED(IDC_USESTYLESHEETFORDUEITEMS, OnUsestylesheetfordueitems)
	ON_BN_CLICKED(IDC_ONLYSHOWDUETASKFORPERSON, OnOnlyshowduetaskforperson)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_NOTIFYDUEONSWITCH, OnNotifyDueOnSwitch)
	ON_BN_CLICKED(IDC_REMOVEARCHIVEDITEMS, OnRemovearchiveditems)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesFilePage message handlers

BOOL CPreferencesFilePage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	m_mgrGroupLines.AddGroupLine(IDC_LOADGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_ARCHIVEGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_SWITCHGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_DUEGROUP, *this);

	GetDlgItem(IDC_REMOVEONLYONABSCOMPLETION)->EnableWindow(m_bRemoveArchivedTasks);
	GetDlgItem(IDC_DONTREMOVEFLAGGED)->EnableWindow(m_bRemoveArchivedTasks);
	GetDlgItem(IDC_NOTIFYDUEBYONLOAD)->EnableWindow(m_bNotifyDueOnLoad);
	GetDlgItem(IDC_NOTIFYDUEBYONSWITCH)->EnableWindow(m_bNotifyDueOnSwitch);
	GetDlgItem(IDC_DISPLAYDUETASKSIMPLECOMMENTS)->EnableWindow(m_bDisplayDueTasksInHtml);
	GetDlgItem(IDC_USESTYLESHEETFORDUEITEMS)->EnableWindow(m_bDisplayDueTasksInHtml);
	GetDlgItem(IDC_DUETASKSTYLESHEET)->EnableWindow(m_bDisplayDueTasksInHtml && m_bUseStyleSheetForDueTasks);
	GetDlgItem(IDC_DUETASKPERSON)->EnableWindow(m_bOnlyShowDueTasksForPerson);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesFilePage::OnRemovearchiveditems() 
{
	UpdateData();

	GetDlgItem(IDC_REMOVEONLYONABSCOMPLETION)->EnableWindow(m_bRemoveArchivedTasks);
	GetDlgItem(IDC_DONTREMOVEFLAGGED)->EnableWindow(m_bRemoveArchivedTasks);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFilePage::OnNotifyDueOnLoad() 
{
	UpdateData();
	
	GetDlgItem(IDC_NOTIFYDUEBYONLOAD)->EnableWindow(m_bNotifyDueOnLoad);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFilePage::OnNotifyDueOnSwitch() 
{
	UpdateData();
	
	GetDlgItem(IDC_NOTIFYDUEBYONSWITCH)->EnableWindow(m_bNotifyDueOnSwitch);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFilePage::OnDisplayduetasksinhtml() 
{
	UpdateData();
	
	GetDlgItem(IDC_DISPLAYDUETASKSIMPLECOMMENTS)->EnableWindow(m_bDisplayDueTasksInHtml);
	GetDlgItem(IDC_USESTYLESHEETFORDUEITEMS)->EnableWindow(m_bDisplayDueTasksInHtml);
	GetDlgItem(IDC_DUETASKSTYLESHEET)->EnableWindow(m_bDisplayDueTasksInHtml && m_bUseStyleSheetForDueTasks);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFilePage::OnUsestylesheetfordueitems() 
{
	UpdateData();
	
	GetDlgItem(IDC_DUETASKSTYLESHEET)->EnableWindow(m_bDisplayDueTasksInHtml && m_bUseStyleSheetForDueTasks);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFilePage::OnOnlyshowduetaskforperson() 
{
	UpdateData();
	
	GetDlgItem(IDC_DUETASKPERSON)->EnableWindow(m_bOnlyShowDueTasksForPerson);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFilePage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	m_bNotifyDueOnLoad = pPrefs->GetProfileInt(szKey, _T("NotifyDue"), FALSE);
	m_bNotifyDueOnSwitch = pPrefs->GetProfileInt(szKey, _T("NotifyDueOnSwitch"), FALSE);
	m_bAutoArchive = pPrefs->GetProfileInt(szKey, _T("AutoArchive"), FALSE);
	m_bNotifyReadOnly = pPrefs->GetProfileInt(szKey, _T("NotifyReadOnly"), TRUE);
	m_bRemoveArchivedTasks = pPrefs->GetProfileInt(szKey, _T("RemoveArchivedTasks"), TRUE);
	m_bRemoveOnlyOnAbsoluteCompletion = pPrefs->GetProfileInt(szKey, _T("RemoveOnlyOnAbsoluteCompletion"), TRUE);
	m_nNotifyDueByOnLoad = pPrefs->GetProfileInt(szKey, _T("NotifyDueBy"), PFP_DUETODAY);
	m_nNotifyDueByOnSwitch = pPrefs->GetProfileInt(szKey, _T("NotifyDueByOnSwitch"), PFP_DUETODAY);
	m_bDisplayDueTasksInHtml = pPrefs->GetProfileInt(szKey, _T("DisplayDueTasksInHtml"), TRUE);
	m_bDisplayDueCommentsInText = pPrefs->GetProfileInt(szKey, _T("DisplayDueCommentsInText"), TRUE);
	m_bRefreshFindOnLoad = pPrefs->GetProfileInt(szKey, _T("RefreshFindOnLoad"), FALSE);
	m_bDueTaskTitlesOnly = pPrefs->GetProfileInt(szKey, _T("DueTaskTitlesOnly"), FALSE);
	m_sDueTasksStylesheet = pPrefs->GetProfileString(szKey, _T("DueTasksStylesheet"), FALSE);
	m_sDueTaskPerson = pPrefs->GetProfileString(szKey, _T("DueTaskPerson"));
	m_bWarnAddDeleteArchive = pPrefs->GetProfileInt(szKey, _T("WarnAddDeleteArchive"), TRUE);
	m_bDontRemoveFlagged = pPrefs->GetProfileInt(szKey, _T("DontRemoveFlagged"), FALSE);
	m_bExpandTasks = pPrefs->GetProfileInt(szKey, _T("ExpandTasks"), FALSE);
//	m_b = pPrefs->GetProfileInt(szKey, _T(""), FALSE);

	// init the stylesheet folder to point to the resource folder and make path relative
	CString sFolder = FileMisc::GetAppResourceFolder() + "\\Stylesheets";
	
	m_eDueTaskStylesheet.SetBrowseTitle(CEnString(IDS_SELECTSTYLESHEET_TITLE));	
	m_eDueTaskStylesheet.SetCurrentFolder(sFolder);
	m_sDueTasksStylesheet = FileMisc::GetRelativePath(m_sDueTasksStylesheet, sFolder, FALSE);

	// these are dependent on the values they control for backward compat
	m_bOnlyShowDueTasksForPerson = pPrefs->GetProfileInt(szKey, _T("OnlyShowDueTasksForPerson"), !m_sDueTaskPerson.IsEmpty());
	m_bUseStyleSheetForDueTasks = pPrefs->GetProfileInt(szKey, _T("UseStylesheetForDueTasks"), !m_sDueTasksStylesheet.IsEmpty());
}

void CPreferencesFilePage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileInt(szKey, _T("NotifyDue"), m_bNotifyDueOnLoad);
	pPrefs->WriteProfileInt(szKey, _T("NotifyDueOnSwitch"), m_bNotifyDueOnSwitch);
	pPrefs->WriteProfileInt(szKey, _T("AutoArchive"), m_bAutoArchive);
	pPrefs->WriteProfileInt(szKey, _T("NotifyReadOnly"), m_bNotifyReadOnly);
	pPrefs->WriteProfileInt(szKey, _T("RemoveArchivedTasks"), m_bRemoveArchivedTasks);
	pPrefs->WriteProfileInt(szKey, _T("RemoveOnlyOnAbsoluteCompletion"), m_bRemoveOnlyOnAbsoluteCompletion);
	pPrefs->WriteProfileInt(szKey, _T("NotifyDueBy"), m_nNotifyDueByOnLoad);
	pPrefs->WriteProfileInt(szKey, _T("NotifyDueByOnSwitch"), m_nNotifyDueByOnSwitch);
	pPrefs->WriteProfileInt(szKey, _T("DisplayDueTasksInHtml"), m_bDisplayDueTasksInHtml);
	pPrefs->WriteProfileInt(szKey, _T("DisplayDueCommentsInText"), m_bDisplayDueCommentsInText);
	pPrefs->WriteProfileInt(szKey, _T("RefreshFindOnLoad"), m_bRefreshFindOnLoad);
	pPrefs->WriteProfileInt(szKey, _T("DueTaskTitlesOnly"), m_bDueTaskTitlesOnly);
	pPrefs->WriteProfileInt(szKey, _T("UseStylesheetForDueTasks"), m_bUseStyleSheetForDueTasks);
	pPrefs->WriteProfileString(szKey, _T("DueTasksStylesheet"), m_sDueTasksStylesheet);
	pPrefs->WriteProfileInt(szKey, _T("OnlyShowDueTasksForPerson"), m_bOnlyShowDueTasksForPerson);
	pPrefs->WriteProfileString(szKey, _T("DueTaskPerson"), m_sDueTaskPerson);
	pPrefs->WriteProfileInt(szKey, _T("WarnAddDeleteArchive"), m_bWarnAddDeleteArchive);
	pPrefs->WriteProfileInt(szKey, _T("DontRemoveFlagged"), m_bDontRemoveFlagged);
	pPrefs->WriteProfileInt(szKey, _T("ExpandTasks"), m_bExpandTasks);

//	pPrefs->WriteProfileInt(szKey, _T(""), m_b);
}

CString CPreferencesFilePage::GetDueTaskStylesheet() const 
{ 
	if (m_bUseStyleSheetForDueTasks && !m_sDueTasksStylesheet.IsEmpty())
		return FileMisc::GetFullPath(m_sDueTasksStylesheet, FileMisc::GetAppResourceFolder() + "\\Stylesheets"); 

	// else
	return _T("");
}

