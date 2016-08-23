// PreferencesUITasklistPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesUITasklistPage.h"

#include "..\shared\colordef.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUITasklistPage property page

CPreferencesUITasklistPage::CPreferencesUITasklistPage() : 
	CPreferencesPageBase(CPreferencesUITasklistPage::IDD)
{
	//{{AFX_DATA_INIT(CPreferencesUITasklistPage)
	//}}AFX_DATA_INIT

}

CPreferencesUITasklistPage::~CPreferencesUITasklistPage()
{
}

void CPreferencesUITasklistPage::DoDataExchange(CDataExchange* pDX)
{ 
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesUITasklistPage)
	DDX_Control(pDX, IDC_PERCENTINCREMENT, m_cbPercentIncrement);
	DDX_Check(pDX, IDC_USEISODATEFORMAT, m_bUseISOForDates);
	DDX_Check(pDX, IDC_SHOWWEEKDAYINDATES, m_bShowWeekdayInDates);
	DDX_Check(pDX, IDC_SHOWPARENTSASFOLDERS, m_bShowParentsAsFolders);
	DDX_Check(pDX, IDC_DISPLAYFIRSTCOMMENTLINE, m_bDisplayFirstCommentLine);
	DDX_Check(pDX, IDC_LIMITINFOTIPCOMMENTS, m_bLimitInfoTipCommentsLength);
	DDX_Check(pDX, IDC_AUTOFOCUSTASKLIST, m_bAutoFocusTasklist);
	DDX_Check(pDX, IDC_RIGHTSIDECOLUMNS, m_bShowColumnsOnRight);
	DDX_Check(pDX, IDC_LIMITCOLWIDTHS, m_bLimitColumnWidths);
	DDX_Text(pDX, IDC_COLWIDTHS, m_nMaxColumnWidth);
	DDX_Check(pDX, IDC_HIDELISTPARENTS, m_bHideListParents);
	DDX_Check(pDX, IDC_HIDEZEROPERCENT, m_bHideZeroPercentDone);
	DDX_Text(pDX, IDC_DATETIMETRAIL, m_sDateTimeTrailText);
	DDX_Check(pDX, IDC_APPENDUSERTODATETIMEPASTE, m_bAppendUserToDateTimePaste);
	DDX_Check(pDX, IDC_APPENDTEXTTODATETIMEPASTE, m_bAppendTextToDateTimePaste);
	DDX_Check(pDX, IDC_SHOWREMINDERSASDATES, m_bShowRemindersAsDateAndTime);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_DISPLAYPATHINHEADER, m_bShowPathInHeader);
	DDX_Check(pDX, IDC_STRIKETHROUGHDONE, m_bStrikethroughDone);
	DDX_Check(pDX, IDC_SHOWINFOTIPS, m_bShowInfoTips);
	DDX_Check(pDX, IDC_SHOWCOMMENTS, m_bShowComments);
	DDX_Text(pDX, IDC_INFOTIPCOMMENTSMAX, m_nMaxInfoTipCommentsLength);
	DDX_Check(pDX, IDC_HIDEUNDEFINEDTIMEST, m_bHideZeroTimeCost);
	DDX_Check(pDX, IDC_HIDESTARTDUEFORDONETASKS, m_bHideStartDueForDoneTasks);
	DDX_Check(pDX, IDC_ROUNDTIMEFRACTIONS, m_bRoundTimeFractions);
	DDX_Check(pDX, IDC_SHOWNONFILEREFSASTEXT, m_bShowNonFilesAsText);
	DDX_Check(pDX, IDC_USEHMSTIMEFORMAT, m_bUseHMSTimeFormat);
	DDX_Check(pDX, IDC_SHOWPERCENTPROGRESSBAR, m_bShowPercentAsProgressbar);
	DDX_Check(pDX, IDC_HIDEPERCENTFORDONETASKS, m_bHidePercentForDoneTasks);
	DDX_Check(pDX, IDC_ALLOWTREEITEMCHECKBOX, m_bAllowCheckboxAgainstTreeItem);
	DDX_Check(pDX, IDC_HIDEPANESPLITBAR, m_bHidePaneSplitBar);

	if (pDX->m_bSaveAndValidate)
		m_nPercentIncrement = CDialogHelper::GetSelectedItemAsValue(m_cbPercentIncrement);
	else
		CDialogHelper::SelectItemByValue(m_cbPercentIncrement, m_nPercentIncrement);
}


BEGIN_MESSAGE_MAP(CPreferencesUITasklistPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesUITasklistPage)
	ON_BN_CLICKED(IDC_SHOWCOMMENTS, OnShowcomments)
	ON_BN_CLICKED(IDC_SHOWINFOTIPS, OnShowinfotips)
	ON_BN_CLICKED(IDC_LIMITINFOTIPCOMMENTS, OnLimitinfotipcomments)
	ON_BN_CLICKED(IDC_LIMITCOLWIDTHS, OnLimitcolwidths)
	ON_BN_CLICKED(IDC_SHOWPARENTSASFOLDERS, OnShowparentsasfolders)
	ON_BN_CLICKED(IDC_APPENDTEXTTODATETIMEPASTE, OnAppendTextToDateTimePaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUITasklistPage message handlers

BOOL CPreferencesUITasklistPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	m_mgrGroupLines.AddGroupLine(IDC_DATETIMEGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_OTHERGROUP, *this);

	GetDlgItem(IDC_DISPLAYFIRSTCOMMENTLINE)->EnableWindow(m_bShowComments);
	GetDlgItem(IDC_LIMITINFOTIPCOMMENTS)->EnableWindow(m_bShowInfoTips);
	GetDlgItem(IDC_INFOTIPCOMMENTSMAX)->EnableWindow(m_bShowInfoTips && m_bLimitInfoTipCommentsLength);
	GetDlgItem(IDC_COLWIDTHS)->EnableWindow(m_bLimitColumnWidths);
	GetDlgItem(IDC_DATETIMETRAILTEXT)->EnableWindow(m_bAppendTextToDateTimePaste);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CPreferencesUITasklistPage::GetMaxInfoTipCommentsLength() const
{
	if (m_bShowInfoTips)
		return m_bLimitInfoTipCommentsLength ? max(0, m_nMaxInfoTipCommentsLength) : -1;

	return -1;
}

int CPreferencesUITasklistPage::GetMaxColumnWidth() const
{
	return m_bLimitColumnWidths ? max(0, m_nMaxColumnWidth) : -1;
}

void CPreferencesUITasklistPage::OnShowcomments() 
{
	UpdateData();

	GetDlgItem(IDC_DISPLAYFIRSTCOMMENTLINE)->EnableWindow(m_bShowComments);
}

void CPreferencesUITasklistPage::OnShowinfotips() 
{
	UpdateData();

	GetDlgItem(IDC_LIMITINFOTIPCOMMENTS)->EnableWindow(m_bShowInfoTips);
	GetDlgItem(IDC_INFOTIPCOMMENTSMAX)->EnableWindow(m_bShowInfoTips && m_bLimitInfoTipCommentsLength); 

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistPage::OnLimitinfotipcomments() 
{
	UpdateData();

	GetDlgItem(IDC_INFOTIPCOMMENTSMAX)->EnableWindow(m_bShowInfoTips && m_bLimitInfoTipCommentsLength);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistPage::LoadPreferences(const CPreferences& prefs)
{
	// prefs
	m_bShowInfoTips = prefs.GetProfileInt(_T("Preferences"), _T("ShowInfoTips"), FALSE);
	m_bShowComments = prefs.GetProfileInt(_T("Preferences"), _T("ShowComments"), TRUE);
	m_bDisplayFirstCommentLine = prefs.GetProfileInt(_T("Preferences"), _T("DisplayFirstCommentLine"), FALSE);
	m_bStrikethroughDone = prefs.GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), TRUE);
	m_bShowPathInHeader = prefs.GetProfileInt(_T("Preferences"), _T("ShowPathInHeader"), TRUE);
	m_bUseISOForDates = prefs.GetProfileInt(_T("Preferences"), _T("DisplayDatesInISO"), FALSE);
	m_bShowWeekdayInDates = prefs.GetProfileInt(_T("Preferences"), _T("ShowWeekdayInDates"), FALSE);
	m_bShowParentsAsFolders = prefs.GetProfileInt(_T("Preferences"), _T("ShowParentsAsFolders"), TRUE);
	m_nMaxInfoTipCommentsLength = prefs.GetProfileInt(_T("Preferences"), _T("MaxInfoTipCommentsLength"), 100);
	m_bLimitInfoTipCommentsLength = prefs.GetProfileInt(_T("Preferences"), _T("LimitInfoTipCommentsLength"), TRUE);
	m_bHidePercentForDoneTasks = prefs.GetProfileInt(_T("Preferences"), _T("HidePercentForDoneTasks"), TRUE);
	m_bHideStartDueForDoneTasks = prefs.GetProfileInt(_T("Preferences"), _T("HideStartDueForDoneTasks"), TRUE);
	m_bHideZeroTimeCost = prefs.GetProfileInt(_T("Preferences"), _T("HideZeroTimeEst"), TRUE);
	m_bShowPercentAsProgressbar = prefs.GetProfileInt(_T("Preferences"), _T("ShowPercentAsProgressbar"), FALSE);
	m_bRoundTimeFractions = prefs.GetProfileInt(_T("Preferences"), _T("RoundTimeFractions"), FALSE);
	m_bShowNonFilesAsText = prefs.GetProfileInt(_T("Preferences"), _T("ShowNonFilesAsText"), FALSE);
	m_bUseHMSTimeFormat = prefs.GetProfileInt(_T("Preferences"), _T("UseHMSTimeFormat"), FALSE);
	m_bAutoFocusTasklist = prefs.GetProfileInt(_T("Preferences"), _T("AutoFocusTasklist"), FALSE);
	m_bShowColumnsOnRight = prefs.GetProfileInt(_T("Preferences"), _T("ShowColumnsOnRight"), TRUE);
	m_bLimitColumnWidths = prefs.GetProfileInt(_T("Preferences"), _T("LimitColumnWidths"), FALSE);
	m_nMaxColumnWidth = prefs.GetProfileInt(_T("Preferences"), _T("MaxColumnWidth"), 60);
	m_bHideListParents = prefs.GetProfileInt(_T("Preferences"), _T("AlwaysHideListParents"), FALSE);
	m_nPercentIncrement = prefs.GetProfileInt(_T("Preferences"), _T("PercentIncrement"), 5);
	m_bHideZeroPercentDone = prefs.GetProfileInt(_T("Preferences"), _T("HideZeroPercentDone"), FALSE);
	m_bAppendUserToDateTimePaste = prefs.GetProfileInt(_T("Preferences"), _T("AppendUserToDateTimePaste"), FALSE);
	m_bAppendTextToDateTimePaste = prefs.GetProfileInt(_T("Preferences"), _T("AppendTextToDateTimePaste"), FALSE);
	m_sDateTimeTrailText = prefs.GetProfileString(_T("Preferences"), _T("DateTimeTrail"));
	m_bAllowCheckboxAgainstTreeItem = prefs.GetProfileInt(_T("Preferences"), _T("AllowCheckboxAgainstTreeItem"), TRUE);
	m_bHidePaneSplitBar = prefs.GetProfileInt(_T("Preferences"), _T("HidePaneSplitBar"), FALSE);
	m_bShowRemindersAsDateAndTime = prefs.GetProfileInt(_T("Preferences"), _T("ShowRemindersAsDateAndTime"), FALSE);
//	m_b = prefs.GetProfileInt(_T("Preferences"), _T(""), FALSE);
}

void CPreferencesUITasklistPage::SavePreferences(CPreferences& prefs)
{
	// save settings
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowInfoTips"), m_bShowInfoTips);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowComments"), m_bShowComments);
	prefs.WriteProfileInt(_T("Preferences"), _T("DisplayFirstCommentLine"), m_bDisplayFirstCommentLine);
	prefs.WriteProfileInt(_T("Preferences"), _T("StrikethroughDone"), m_bStrikethroughDone);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowPathInHeader"), m_bShowPathInHeader);
	prefs.WriteProfileInt(_T("Preferences"), _T("DisplayDatesInISO"), m_bUseISOForDates);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowWeekdayInDates"), m_bShowWeekdayInDates);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowParentsAsFolders"), m_bShowParentsAsFolders);
	prefs.WriteProfileInt(_T("Preferences"), _T("MaxInfoTipCommentsLength"), max(m_nMaxInfoTipCommentsLength, 0));
	prefs.WriteProfileInt(_T("Preferences"), _T("LimitInfoTipCommentsLength"), m_bLimitInfoTipCommentsLength);
	prefs.WriteProfileInt(_T("Preferences"), _T("HidePercentForDoneTasks"), m_bHidePercentForDoneTasks);
	prefs.WriteProfileInt(_T("Preferences"), _T("HideStartDueForDoneTasks"), m_bHideStartDueForDoneTasks);
	prefs.WriteProfileInt(_T("Preferences"), _T("HideZeroTimeEst"), m_bHideZeroTimeCost);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowPercentAsProgressbar"), m_bShowPercentAsProgressbar);
	prefs.WriteProfileInt(_T("Preferences"), _T("RoundTimeFractions"), m_bRoundTimeFractions);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowNonFilesAsText"), m_bShowNonFilesAsText);
	prefs.WriteProfileInt(_T("Preferences"), _T("UseHMSTimeFormat"), m_bUseHMSTimeFormat);
	prefs.WriteProfileInt(_T("Preferences"), _T("AutoFocusTasklist"), m_bAutoFocusTasklist);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowColumnsOnRight"), m_bShowColumnsOnRight);
	prefs.WriteProfileInt(_T("Preferences"), _T("LimitColumnWidths"), m_bLimitColumnWidths);
	prefs.WriteProfileInt(_T("Preferences"), _T("MaxColumnWidth"), m_nMaxColumnWidth);
	prefs.WriteProfileInt(_T("Preferences"), _T("AlwaysHideListParents"), m_bHideListParents);
	prefs.WriteProfileInt(_T("Preferences"), _T("PercentIncrement"), m_nPercentIncrement);
	prefs.WriteProfileInt(_T("Preferences"), _T("HideZeroPercentDone"), m_bHideZeroPercentDone);
	prefs.WriteProfileString(_T("Preferences"), _T("DateTimeTrail"), m_sDateTimeTrailText);
	prefs.WriteProfileInt(_T("Preferences"), _T("AppendUserToDateTimePaste"), m_bAppendUserToDateTimePaste);
	prefs.WriteProfileInt(_T("Preferences"), _T("AppendTextToDateTimePaste"), m_bAppendTextToDateTimePaste);
	prefs.WriteProfileInt(_T("Preferences"), _T("AllowCheckboxAgainstTreeItem"), m_bAllowCheckboxAgainstTreeItem);
	prefs.WriteProfileInt(_T("Preferences"), _T("HidePaneSplitBar"), m_bHidePaneSplitBar);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowRemindersAsDateAndTime"), m_bShowRemindersAsDateAndTime);
//	prefs.WriteProfileInt(_T("Preferences"), _T(""), m_b);
}

void CPreferencesUITasklistPage::OnLimitcolwidths() 
{
	UpdateData();

	GetDlgItem(IDC_COLWIDTHS)->EnableWindow(m_bLimitColumnWidths);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistPage::OnShowparentsasfolders() 
{
	UpdateData();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistPage::OnAppendTextToDateTimePaste() 
{
	UpdateData();

	GetDlgItem(IDC_DATETIMETRAILTEXT)->EnableWindow(m_bAppendTextToDateTimePaste);
}
