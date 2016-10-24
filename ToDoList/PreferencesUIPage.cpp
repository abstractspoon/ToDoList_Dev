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
		m_lbTaskViews(pMgrUIExt),
		m_eUITheme(FES_COMBOSTYLEBTN | FES_RELATIVEPATHS, CEnString(IDS_UITHEMEFILEFILTER))
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
	DDX_Control(pDX, IDC_UITHEMEFILE, m_eUITheme);
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
	DDX_Text(pDX, IDC_UITHEMEFILE, m_sUIThemeFile);
	DDX_Check(pDX, IDC_USEUITHEME, m_bUseUITheme);
	DDX_Check(pDX, IDC_SHOWDEFAULTTASKICONS, m_bShowDefaultTaskIcons);
	DDX_Check(pDX, IDC_SHOWDEFAULTFILTERS, m_bShowDefaultFilters);
	DDX_Check(pDX, IDC_AUTORESORT, m_bAutoResort);
	DDX_Check(pDX, IDC_ENABLECOLUMNHEADERSORTING, m_bEnableColumnHeaderSorting);
	DDX_Check(pDX, IDC_STACKFIELDSANDCOMMENTS, m_bStackEditFieldsAndComments);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_MATCHTITLES, (int&)m_nTitleFilterOption);
	DDX_CBIndex(pDX, IDC_NEWTASKPOSITION, (int&)m_nNewTaskPos);
	DDX_CBIndex(pDX, IDC_NEWSUBTASKPOSITION, (int&)m_nNewSubtaskPos);
	DDX_CBIndex(pDX, IDC_COMMENTPOS, (int&)m_nCommentsPos);
	DDX_CBIndex(pDX, IDC_CTRLSPOS, (int&)m_nCtrlsPos);
	DDX_Control(pDX, IDC_COMMENTSFORMAT, m_cbCommentsFmt);
	DDX_Control(pDX, IDC_TASKVIEWVISIBILITY, m_lbTaskViews);
	DDX_CBIndex(pDX, IDC_COMMENTSFORMAT, m_nDefaultCommentsFormat);
	DDX_Check(pDX, IDC_SORTDONETASKSATBOTTOM, m_bSortDoneTasksAtBottom);
//	DDX_Check(pDX, IDC_RTLCOMMENTS, m_bRTLComments);
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
	BOOL bThemeActive = CThemed::IsAppThemed();
	GetDlgItem(IDC_USEUITHEME)->EnableWindow(bThemeActive);
	GetDlgItem(IDC_UITHEMEFILE)->EnableWindow(m_bUseUITheme && bThemeActive);

	m_lbTaskViews.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesUIPage::OnSelchangeCommentsformat() 
{
	m_cbCommentsFmt.GetSelectedFormat(m_cfDefault);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUIPage::LoadPreferences(const CPreferences& prefs)
{
	// load settings
	m_bShowEditMenuAsColumns = prefs.GetProfileInt(_T("Preferences"), _T("ShowEditMenuAsColumns"), FALSE);
	m_bShowCommentsAlways = prefs.GetProfileInt(_T("Preferences"), _T("ShowCommentsAlways"), FALSE);
	m_bAutoReposCtrls = prefs.GetProfileInt(_T("Preferences"), _T("AutoReposCtrls"), TRUE);
	m_bSpecifyToolbarImage = prefs.GetProfileInt(_T("Preferences"), _T("SpecifyToolbarImage"), FALSE);
	m_bSharedCommentsHeight = prefs.GetProfileInt(_T("Preferences"), _T("SharedCommentsHeight"), TRUE);
	m_bAutoHideTabbar = prefs.GetProfileInt(_T("Preferences"), _T("AutoHideTabbar"), TRUE);
	m_bStackTabbarItems = prefs.GetProfileInt(_T("Preferences"), _T("StackTabbarItems"), FALSE);
	m_bFocusTreeOnEnter = prefs.GetProfileInt(_T("Preferences"), _T("FocusTreeOnEnter"), FALSE);
	m_nNewTaskPos = (PUIP_NEWTASKPOS)prefs.GetProfileInt(_T("Preferences"), _T("NewTaskPos"), PUIP_BELOW);
	m_nNewSubtaskPos = (PUIP_NEWTASKPOS)prefs.GetProfileInt(_T("Preferences"), _T("NewSubtaskPos"), PUIP_BOTTOM);
	m_bKeepTabsOrdered = prefs.GetProfileInt(_T("Preferences"), _T("KeepTabsOrdered"), FALSE);
	m_bShowTasklistTabCloseButton = prefs.GetProfileInt(_T("Preferences"), _T("ShowTasklistTabCloseButton"), TRUE);
	m_bSortDoneTasksAtBottom = prefs.GetProfileInt(_T("Preferences"), _T("SortDoneTasksAtBottom"), TRUE);
	m_bRTLComments = prefs.GetProfileInt(_T("Preferences"), _T("RTLComments"), FALSE);
	m_nCommentsPos = (PUIP_LOCATION)prefs.GetProfileInt(_T("Preferences"), _T("VertComments"), PUIP_LOCATERIGHT);
	m_nCtrlsPos = (PUIP_LOCATION)prefs.GetProfileInt(_T("Preferences"), _T("VertControls"), PUIP_LOCATEBOTTOM);
	m_bMultiSelFilters = prefs.GetProfileInt(_T("Preferences"), _T("MultiSelFilters"), TRUE);
	m_bRestoreTasklistFilters = prefs.GetProfileInt(_T("Preferences"), _T("RestoreTasklistFilters"), TRUE);
	m_bAutoRefilter = prefs.GetProfileInt(_T("Preferences"), _T("AutoRefilter"), TRUE);
	m_bAutoResort = prefs.GetProfileInt(_T("Preferences"), _T("AutoResort"), TRUE);
	m_bUseUITheme = CThemed::IsAppThemed() && prefs.GetProfileInt(_T("Preferences"), _T("UseUITheme"), TRUE);
	m_sUIThemeFile = prefs.GetProfileString(_T("Preferences"), _T("UIThemeFile"), _T(".\\ThemeBlue.xml"));
	m_nTitleFilterOption = (PUIP_MATCHTITLE)prefs.GetProfileInt(_T("Preferences"), _T("TitleFilterOption"), PUIP_MATCHONTITLECOMMENTS);
	m_bShowDefaultTaskIcons = prefs.GetProfileInt(_T("Preferences"), _T("ShowDefaultTaskIcons"), TRUE);
	m_bShowDefaultFilters = prefs.GetProfileInt(_T("Preferences"), _T("ShowDefaultFilters"), TRUE);
	m_bEnableColumnHeaderSorting = prefs.GetProfileInt(_T("Preferences"), _T("EnableColumnHeaderSorting"), TRUE);
	m_bStackEditFieldsAndComments = prefs.GetProfileInt(_T("Preferences"), _T("StackEditFieldsAndComments"), TRUE);
//	m_b = prefs.GetProfileInt(_T("Preferences"), _T(""), FALSE);

	// set default theme to blue
	if (CThemed::IsAppThemed())
	{
		CString sResFolder = FileMisc::GetAppResourceFolder() + "\\Themes";

		m_eUITheme.SetCurrentFolder(sResFolder);
		m_sUIThemeFile = FileMisc::GetRelativePath(m_sUIThemeFile, sResFolder, FALSE);
	}
	else
		m_sUIThemeFile.Empty();

	// comments format
	if (m_cbCommentsFmt.IsInitialized())
	{
		m_cfDefault = prefs.GetProfileString(_T("Preferences"), _T("DefaultCommentsFormatID"));
		m_nDefaultCommentsFormat = m_cbCommentsFmt.SetSelectedFormat(m_cfDefault);

		// fallback
		if (m_nDefaultCommentsFormat == CB_ERR)
			m_nDefaultCommentsFormat = prefs.GetProfileInt(_T("Preferences"), _T("DefaultCommentsFormat"), -1);

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
	int nView = prefs.GetProfileInt(_T("Preferences\\ViewVisibility"), _T("HiddenCount"), -1);

	if (nView > 0)
	{
		while (nView--)
		{
			CString sKey = Misc::MakeKey(_T("Hidden%d"), nView);
			CString sView = prefs.GetProfileString(_T("Preferences\\ViewVisibility"), sKey);
			ASSERT(!sView.IsEmpty());
			
			aViews.Add(sView);
		}

		m_lbTaskViews.SetHiddenViews(aViews);
	}
}

void CPreferencesUIPage::SavePreferences(CPreferences& prefs)
{
	// save settings
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowEditMenuAsColumns"), m_bShowEditMenuAsColumns);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowCommentsAlways"), m_bShowCommentsAlways);
	prefs.WriteProfileInt(_T("Preferences"), _T("AutoReposCtrls"), m_bAutoReposCtrls);
	prefs.WriteProfileInt(_T("Preferences"), _T("SpecifyToolbarImage"), m_bSpecifyToolbarImage);
	prefs.WriteProfileInt(_T("Preferences"), _T("SharedCommentsHeight"), m_bSharedCommentsHeight);
	prefs.WriteProfileInt(_T("Preferences"), _T("AutoHideTabbar"), m_bAutoHideTabbar);
	prefs.WriteProfileInt(_T("Preferences"), _T("StackTabbarItems"), m_bStackTabbarItems);
	prefs.WriteProfileInt(_T("Preferences"), _T("FocusTreeOnEnter"), m_bFocusTreeOnEnter);
	prefs.WriteProfileInt(_T("Preferences"), _T("NewTaskPos"), m_nNewTaskPos);
	prefs.WriteProfileInt(_T("Preferences"), _T("NewSubtaskPos"), m_nNewSubtaskPos);
	prefs.WriteProfileInt(_T("Preferences"), _T("KeepTabsOrdered"), m_bKeepTabsOrdered);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowTasklistTabCloseButton"), m_bShowTasklistTabCloseButton);
	prefs.WriteProfileInt(_T("Preferences"), _T("SortDoneTasksAtBottom"), m_bSortDoneTasksAtBottom);
	prefs.WriteProfileInt(_T("Preferences"), _T("RTLComments"), m_bRTLComments);
	prefs.WriteProfileInt(_T("Preferences"), _T("VertComments"), m_nCommentsPos);
	prefs.WriteProfileInt(_T("Preferences"), _T("VertControls"), m_nCtrlsPos);
	prefs.WriteProfileInt(_T("Preferences"), _T("MultiSelFilters"), m_bMultiSelFilters);
	prefs.WriteProfileInt(_T("Preferences"), _T("RestoreTasklistFilters"), m_bRestoreTasklistFilters);
	prefs.WriteProfileInt(_T("Preferences"), _T("AutoRefilter"), m_bAutoRefilter);
	prefs.WriteProfileInt(_T("Preferences"), _T("AutoResort"), m_bAutoResort);
	prefs.WriteProfileString(_T("Preferences"), _T("UIThemeFile"), m_sUIThemeFile);
	prefs.WriteProfileInt(_T("Preferences"), _T("UseUITheme"), m_bUseUITheme);
	prefs.WriteProfileInt(_T("Preferences"), _T("TitleFilterOption"), m_nTitleFilterOption);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowDefaultTaskIcons"), m_bShowDefaultTaskIcons);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowDefaultFilters"), m_bShowDefaultFilters);
	prefs.WriteProfileInt(_T("Preferences"), _T("EnableColumnHeaderSorting"), m_bEnableColumnHeaderSorting);
	prefs.WriteProfileInt(_T("Preferences"), _T("StackEditFieldsAndComments"), m_bStackEditFieldsAndComments);
//	prefs.WriteProfileInt(_T("Preferences"), _T(""), m_b);

	// comments format
	if (m_pMgrContent)
	{
		prefs.WriteProfileInt(_T("Preferences"), _T("DefaultCommentsFormat"), m_nDefaultCommentsFormat);
		prefs.WriteProfileString(_T("Preferences"), _T("DefaultCommentsFormatID"), m_cfDefault);
	}

	// task views
	CStringArray aViews;
	int nView = m_lbTaskViews.GetHiddenViews(aViews);

	prefs.WriteProfileInt(_T("Preferences\\ViewVisibility"), _T("HiddenCount"), nView);

	while (nView--)
	{
		CString sKey = Misc::MakeKey(_T("Hidden%d"), nView);
		prefs.WriteProfileString(_T("Preferences\\ViewVisibility"), sKey, aViews[nView]);
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
