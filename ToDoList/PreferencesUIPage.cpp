// PreferencesUIPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesUIPage.h"
#include "tdcenum.h"

#include "..\shared\misc.h"
#include "..\shared\enstring.h"
#include "..\shared\osversion.h"
#include "..\shared\themed.h"
#include "..\shared\filemisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUIPage property page

IMPLEMENT_DYNCREATE(CPreferencesUIPage, CPreferencesPageBase)

CPreferencesUIPage::CPreferencesUIPage(const CContentMgr* pMgrContent, const CUIExtensionMgr* pMgrUIExt) : 
	CPreferencesPageBase(CPreferencesUIPage::IDD), 
		m_pMgrContent(pMgrContent), 
		m_pMgrUIExt(pMgrUIExt),
		m_cbCommentsFmt(pMgrContent),
		m_nDefaultCommentsFormat(-1),
		m_lbTaskViews(pMgrUIExt)
{
	//{{AFX_DATA_INIT(CPreferencesUIPage)
	//}}AFX_DATA_INIT
}

CPreferencesUIPage::~CPreferencesUIPage()
{
}

void CPreferencesUIPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPreferencesUIPage)
	DDX_Check(pDX, IDC_SHOWCOMMENTSALWAYS, m_bShowCommentsAlways);
	DDX_Check(pDX, IDC_AUTOREPOSCTRLS, m_bAutoReposCtrls);
	DDX_Check(pDX, IDC_SHAREDCOMMENTSHEIGHT, m_bSharedCommentsHeight);
	DDX_Check(pDX, IDC_AUTOHIDETABBAR, m_bAutoHideTabbar);
	DDX_Check(pDX, IDC_STACKTABBARITEMS, m_bStackTabbarItems);
	DDX_Check(pDX, IDC_FOCUSTREEONENTER, m_bFocusTreeOnEnter);
	DDX_Check(pDX, IDC_KEEPTABSORDERED, m_bKeepTabsOrdered);
	DDX_Check(pDX, IDC_SHOWTASKLISTCLOSEBTN, m_bShowTasklistTabCloseButton);
	DDX_Check(pDX, IDC_SHOWEDITMENUASCOLUMNS, m_bShowEditMenuAsColumns);
	DDX_Check(pDX, IDC_MULTISELFILTER, m_bMultiSelFilters);
	DDX_Check(pDX, IDC_RESTORETASKLISTFILTERS, m_bRestoreTasklistFilters);
	DDX_Check(pDX, IDC_AUTOREFILTER, m_bAutoRefilter);
	DDX_Check(pDX, IDC_USEUITHEME, m_bUseUITheme);
	DDX_Check(pDX, IDC_SHOWDEFAULTTASKICONS, m_bShowDefaultTaskIcons);
	DDX_Check(pDX, IDC_SHOWDEFAULTFILTERS, m_bShowDefaultFilters);
	DDX_Check(pDX, IDC_AUTORESORT, m_bAutoResort);
	DDX_Check(pDX, IDC_ENABLECOLUMNHEADERSORTING, m_bEnableColumnHeaderSorting);
	DDX_Check(pDX, IDC_STACKFIELDSANDCOMMENTS, m_bStackEditFieldsAndComments);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_UITHEMEFILE, m_cbThemes);
	DDX_Radio(pDX, IDC_MATCHTITLES, (int&)m_nTitleFilterOption);
	DDX_CBIndex(pDX, IDC_NEWTASKPOSITION, (int&)m_nNewTaskPos);
	DDX_CBIndex(pDX, IDC_NEWSUBTASKPOSITION, (int&)m_nNewSubtaskPos);
	DDX_CBIndex(pDX, IDC_COMMENTPOS, (int&)m_nCommentsPos);
	DDX_CBIndex(pDX, IDC_CTRLSPOS, (int&)m_nCtrlsPos);
	DDX_Control(pDX, IDC_COMMENTSFORMAT, m_cbCommentsFmt);
	DDX_Control(pDX, IDC_TASKVIEWVISIBILITY, m_lbTaskViews);
	DDX_CBIndex(pDX, IDC_COMMENTSFORMAT, m_nDefaultCommentsFormat);
	DDX_Check(pDX, IDC_SORTDONETASKSATBOTTOM, m_bSortDoneTasksAtBottom);
	DDX_Check(pDX, IDC_INCLUDEWEBLINKINCOMMENTSPASTE, m_bIncludeWebLinksInCommentsPaste);
//	DDX_Check(pDX, IDC_RTLCOMMENTS, m_bRTLComments);

	if (pDX->m_bSaveAndValidate)
		m_sUIThemeFile = m_cbThemes.GetThemePath();
	else
		m_cbThemes.SetThemePath(m_sUIThemeFile);
}

BEGIN_MESSAGE_MAP(CPreferencesUIPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesUIPage)
	ON_BN_CLICKED(IDC_USEUITHEME, OnUseuitheme)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMMENTSFORMAT, OnSelchangeCommentsformat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUIPage message handlers

BOOL CPreferencesUIPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	m_mgrGroupLines.AddGroupLine(IDC_TOOLBARGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_SORTGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_TABBARGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_COMMENTSGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_FILTERGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_TASKVIEWSGROUP, *this);

	m_cbCommentsFmt.SetCurSel(m_nDefaultCommentsFormat);
	GetDlgItem(IDC_COMMENTSFORMAT)->EnableWindow(m_nDefaultCommentsFormat != CB_ERR);

	// theming only available if XP themes are active
	if (CThemed::IsAppThemed())
	{
		m_cbThemes.SetThemePath(m_sUIThemeFile);
		GetDlgItem(IDC_UITHEMEFILE)->EnableWindow(m_bUseUITheme);
	}
	else
	{
		GetDlgItem(IDC_USEUITHEME)->EnableWindow(FALSE);
		GetDlgItem(IDC_UITHEMEFILE)->EnableWindow(FALSE);
	}
	
	m_lbTaskViews.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesUIPage::OnSelchangeCommentsformat() 
{
	m_cbCommentsFmt.GetSelectedFormat(m_cfDefault);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUIPage::LoadPreferences(const IPreferences* pPrefs)
{
	// load settings
	m_bShowEditMenuAsColumns = pPrefs->GetProfileInt(_T("Preferences"), _T("ShowEditMenuAsColumns"), FALSE);
	m_bShowCommentsAlways = pPrefs->GetProfileInt(_T("Preferences"), _T("ShowCommentsAlways"), FALSE);
	m_bAutoReposCtrls = pPrefs->GetProfileInt(_T("Preferences"), _T("AutoReposCtrls"), TRUE);
	m_bSpecifyToolbarImage = pPrefs->GetProfileInt(_T("Preferences"), _T("SpecifyToolbarImage"), FALSE);
	m_bSharedCommentsHeight = pPrefs->GetProfileInt(_T("Preferences"), _T("SharedCommentsHeight"), TRUE);
	m_bAutoHideTabbar = pPrefs->GetProfileInt(_T("Preferences"), _T("AutoHideTabbar"), TRUE);
	m_bStackTabbarItems = pPrefs->GetProfileInt(_T("Preferences"), _T("StackTabbarItems"), FALSE);
	m_bFocusTreeOnEnter = pPrefs->GetProfileInt(_T("Preferences"), _T("FocusTreeOnEnter"), FALSE);
	m_nNewTaskPos = (PUIP_NEWTASKPOS)pPrefs->GetProfileInt(_T("Preferences"), _T("NewTaskPos"), PUIP_BELOW);
	m_nNewSubtaskPos = (PUIP_NEWTASKPOS)pPrefs->GetProfileInt(_T("Preferences"), _T("NewSubtaskPos"), PUIP_BOTTOM);
	m_bKeepTabsOrdered = pPrefs->GetProfileInt(_T("Preferences"), _T("KeepTabsOrdered"), FALSE);
	m_bShowTasklistTabCloseButton = pPrefs->GetProfileInt(_T("Preferences"), _T("ShowTasklistTabCloseButton"), TRUE);
	m_bSortDoneTasksAtBottom = pPrefs->GetProfileInt(_T("Preferences"), _T("SortDoneTasksAtBottom"), TRUE);
	m_bRTLComments = pPrefs->GetProfileInt(_T("Preferences"), _T("RTLComments"), FALSE);
	m_nCommentsPos = (PUIP_LOCATION)pPrefs->GetProfileInt(_T("Preferences"), _T("VertComments"), PUIP_LOCATERIGHT);
	m_nCtrlsPos = (PUIP_LOCATION)pPrefs->GetProfileInt(_T("Preferences"), _T("VertControls"), PUIP_LOCATEBOTTOM);
	m_bMultiSelFilters = pPrefs->GetProfileInt(_T("Preferences"), _T("MultiSelFilters"), TRUE);
	m_bRestoreTasklistFilters = pPrefs->GetProfileInt(_T("Preferences"), _T("RestoreTasklistFilters"), TRUE);
	m_bAutoRefilter = pPrefs->GetProfileInt(_T("Preferences"), _T("AutoRefilter"), TRUE);
	m_bAutoResort = pPrefs->GetProfileInt(_T("Preferences"), _T("AutoResort"), TRUE);
	m_bUseUITheme = CThemed::IsAppThemed() && pPrefs->GetProfileInt(_T("Preferences"), _T("UseUITheme"), TRUE);
	m_sUIThemeFile = pPrefs->GetProfileString(_T("Preferences"), _T("UIThemeFile"), _T(".\\ThemeBlue.xml"));
	m_nTitleFilterOption = (PUIP_MATCHTITLE)pPrefs->GetProfileInt(_T("Preferences"), _T("TitleFilterOption"), PUIP_MATCHONTITLECOMMENTS);
	m_bShowDefaultTaskIcons = pPrefs->GetProfileInt(_T("Preferences"), _T("ShowDefaultTaskIcons"), TRUE);
	m_bShowDefaultFilters = pPrefs->GetProfileInt(_T("Preferences"), _T("ShowDefaultFilters"), TRUE);
	m_bEnableColumnHeaderSorting = pPrefs->GetProfileInt(_T("Preferences"), _T("EnableColumnHeaderSorting"), TRUE);
	m_bStackEditFieldsAndComments = pPrefs->GetProfileInt(_T("Preferences"), _T("StackEditFieldsAndComments"), TRUE);
	m_bIncludeWebLinksInCommentsPaste = pPrefs->GetProfileInt(_T("Preferences"), _T("IncludeWebLinksInCommentsPaste"), TRUE);
//	m_b = pPrefs->GetProfileInt(_T("Preferences"), _T(""), FALSE);

	// comments format
	if (m_cbCommentsFmt.IsInitialized())
	{
		m_cfDefault = pPrefs->GetProfileString(_T("Preferences"), _T("DefaultCommentsFormatID"));
		m_nDefaultCommentsFormat = m_cbCommentsFmt.SetSelectedFormat(m_cfDefault);

		// fallback
		if (m_nDefaultCommentsFormat == CB_ERR)
			m_nDefaultCommentsFormat = pPrefs->GetProfileInt(_T("Preferences"), _T("DefaultCommentsFormat"), -1);

		if ((m_nDefaultCommentsFormat == CB_ERR) || (m_nDefaultCommentsFormat >= m_cbCommentsFmt.GetCount()))
		{
			ASSERT (m_cbCommentsFmt.GetCount());

			m_nDefaultCommentsFormat = 0;
		}

		m_cbCommentsFmt.SetCurSel(m_nDefaultCommentsFormat);
		m_cbCommentsFmt.GetSelectedFormat(m_cfDefault);
	}

	// task view visibility
	CStringArray aViews;
	int nView = pPrefs->GetProfileInt(_T("Preferences\\ViewVisibility"), _T("HiddenCount"), -1);

	if (nView > 0)
	{
		while (nView--)
		{
			CString sKey = Misc::MakeKey(_T("Hidden%d"), nView);
			CString sView = pPrefs->GetProfileString(_T("Preferences\\ViewVisibility"), sKey);
			ASSERT(!sView.IsEmpty());
			
			aViews.Add(sView);
		}

		m_lbTaskViews.SetHiddenViews(aViews);
	}
}

void CPreferencesUIPage::SavePreferences(IPreferences* pPrefs)
{
	// save settings
	pPrefs->WriteProfileInt(_T("Preferences"), _T("ShowEditMenuAsColumns"), m_bShowEditMenuAsColumns);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("ShowCommentsAlways"), m_bShowCommentsAlways);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("AutoReposCtrls"), m_bAutoReposCtrls);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("SpecifyToolbarImage"), m_bSpecifyToolbarImage);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("SharedCommentsHeight"), m_bSharedCommentsHeight);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("AutoHideTabbar"), m_bAutoHideTabbar);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("StackTabbarItems"), m_bStackTabbarItems);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("FocusTreeOnEnter"), m_bFocusTreeOnEnter);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("NewTaskPos"), m_nNewTaskPos);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("NewSubtaskPos"), m_nNewSubtaskPos);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("KeepTabsOrdered"), m_bKeepTabsOrdered);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("ShowTasklistTabCloseButton"), m_bShowTasklistTabCloseButton);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("SortDoneTasksAtBottom"), m_bSortDoneTasksAtBottom);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("RTLComments"), m_bRTLComments);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("VertComments"), m_nCommentsPos);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("VertControls"), m_nCtrlsPos);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("MultiSelFilters"), m_bMultiSelFilters);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("RestoreTasklistFilters"), m_bRestoreTasklistFilters);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("AutoRefilter"), m_bAutoRefilter);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("AutoResort"), m_bAutoResort);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("UseUITheme"), m_bUseUITheme);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("TitleFilterOption"), m_nTitleFilterOption);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("ShowDefaultTaskIcons"), m_bShowDefaultTaskIcons);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("ShowDefaultFilters"), m_bShowDefaultFilters);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("EnableColumnHeaderSorting"), m_bEnableColumnHeaderSorting);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("StackEditFieldsAndComments"), m_bStackEditFieldsAndComments);
	pPrefs->WriteProfileInt(_T("Preferences"), _T("IncludeWebLinksInCommentsPaste"), m_bIncludeWebLinksInCommentsPaste);
//	pPrefs->WriteProfileInt(_T("Preferences"), _T(""), m_b);

	pPrefs->WriteProfileString(_T("Preferences"), _T("UIThemeFile"), m_sUIThemeFile);

	// comments format
	if (m_pMgrContent)
	{
		pPrefs->WriteProfileInt(_T("Preferences"), _T("DefaultCommentsFormat"), m_nDefaultCommentsFormat);
		pPrefs->WriteProfileString(_T("Preferences"), _T("DefaultCommentsFormatID"), m_cfDefault);
	}

	// task views
	CStringArray aViews;
	int nView = m_lbTaskViews.GetHiddenViews(aViews);

	pPrefs->WriteProfileInt(_T("Preferences\\ViewVisibility"), _T("HiddenCount"), nView);

	while (nView--)
	{
		CString sKey = Misc::MakeKey(_T("Hidden%d"), nView);
		pPrefs->WriteProfileString(_T("Preferences\\ViewVisibility"), sKey, aViews[nView]);
	}
}

void CPreferencesUIPage::OnUseuitheme() 
{
	UpdateData();
	GetDlgItem(IDC_UITHEMEFILE)->EnableWindow(m_bUseUITheme);

	CPreferencesPageBase::OnControlChange();
}

CString CPreferencesUIPage::GetUITheme() const 
{ 
	if (m_bUseUITheme)
		return FileMisc::GetFullPath(m_sUIThemeFile, FileMisc::GetAppResourceFolder() + "\\Themes");
	
	// else
	return _T("");
}

int CPreferencesUIPage::GetDefaultTaskViews(CStringArray& aTypeIDs) const
{
	return m_lbTaskViews.GetVisibleViews(aTypeIDs);
}
