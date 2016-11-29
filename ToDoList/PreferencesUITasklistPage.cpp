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

void CPreferencesUITasklistPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	// prefs
	m_bShowInfoTips = pPrefs->GetProfileInt(szKey, _T("ShowInfoTips"), FALSE);
	m_bShowComments = pPrefs->GetProfileInt(szKey, _T("ShowComments"), TRUE);
	m_bDisplayFirstCommentLine = pPrefs->GetProfileInt(szKey, _T("DisplayFirstCommentLine"), FALSE);
	m_bStrikethroughDone = pPrefs->GetProfileInt(szKey, _T("StrikethroughDone"), TRUE);
	m_bShowPathInHeader = pPrefs->GetProfileInt(szKey, _T("ShowPathInHeader"), TRUE);
	m_bUseISOForDates = pPrefs->GetProfileInt(szKey, _T("DisplayDatesInISO"), FALSE);
	m_bShowWeekdayInDates = pPrefs->GetProfileInt(szKey, _T("ShowWeekdayInDates"), FALSE);
	m_bShowParentsAsFolders = pPrefs->GetProfileInt(szKey, _T("ShowParentsAsFolders"), TRUE);
	m_nMaxInfoTipCommentsLength = pPrefs->GetProfileInt(szKey, _T("MaxInfoTipCommentsLength"), 100);
	m_bLimitInfoTipCommentsLength = pPrefs->GetProfileInt(szKey, _T("LimitInfoTipCommentsLength"), TRUE);
	m_bHidePercentForDoneTasks = pPrefs->GetProfileInt(szKey, _T("HidePercentForDoneTasks"), TRUE);
	m_bHideStartDueForDoneTasks = pPrefs->GetProfileInt(szKey, _T("HideStartDueForDoneTasks"), TRUE);
	m_bHideZeroTimeCost = pPrefs->GetProfileInt(szKey, _T("HideZeroTimeEst"), TRUE);
	m_bShowPercentAsProgressbar = pPrefs->GetProfileInt(szKey, _T("ShowPercentAsProgressbar"), FALSE);
	m_bRoundTimeFractions = pPrefs->GetProfileInt(szKey, _T("RoundTimeFractions"), FALSE);
	m_bShowNonFilesAsText = pPrefs->GetProfileInt(szKey, _T("ShowNonFilesAsText"), FALSE);
	m_bUseHMSTimeFormat = pPrefs->GetProfileInt(szKey, _T("UseHMSTimeFormat"), FALSE);
	m_bAutoFocusTasklist = pPrefs->GetProfileInt(szKey, _T("AutoFocusTasklist"), FALSE);
	m_bShowColumnsOnRight = pPrefs->GetProfileInt(szKey, _T("ShowColumnsOnRight"), TRUE);
	m_bLimitColumnWidths = pPrefs->GetProfileInt(szKey, _T("LimitColumnWidths"), FALSE);
	m_nMaxColumnWidth = pPrefs->GetProfileInt(szKey, _T("MaxColumnWidth"), 60);
	m_bHideListParents = pPrefs->GetProfileInt(szKey, _T("AlwaysHideListParents"), FALSE);
	m_nPercentIncrement = pPrefs->GetProfileInt(szKey, _T("PercentIncrement"), 5);
	m_bHideZeroPercentDone = pPrefs->GetProfileInt(szKey, _T("HideZeroPercentDone"), FALSE);
	m_bAppendUserToDateTimePaste = pPrefs->GetProfileInt(szKey, _T("AppendUserToDateTimePaste"), FALSE);
	m_bAppendTextToDateTimePaste = pPrefs->GetProfileInt(szKey, _T("AppendTextToDateTimePaste"), FALSE);
	m_sDateTimeTrailText = pPrefs->GetProfileString(szKey, _T("DateTimeTrail"));
	m_bAllowCheckboxAgainstTreeItem = pPrefs->GetProfileInt(szKey, _T("AllowCheckboxAgainstTreeItem"), TRUE);
	m_bHidePaneSplitBar = pPrefs->GetProfileInt(szKey, _T("HidePaneSplitBar"), FALSE);
	m_bShowRemindersAsDateAndTime = pPrefs->GetProfileInt(szKey, _T("ShowRemindersAsDateAndTime"), FALSE);
//	m_b = pPrefs->GetProfileInt(szKey, _T(""), FALSE);
}

void CPreferencesUITasklistPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileInt(szKey, _T("ShowInfoTips"), m_bShowInfoTips);
	pPrefs->WriteProfileInt(szKey, _T("ShowComments"), m_bShowComments);
	pPrefs->WriteProfileInt(szKey, _T("DisplayFirstCommentLine"), m_bDisplayFirstCommentLine);
	pPrefs->WriteProfileInt(szKey, _T("StrikethroughDone"), m_bStrikethroughDone);
	pPrefs->WriteProfileInt(szKey, _T("ShowPathInHeader"), m_bShowPathInHeader);
	pPrefs->WriteProfileInt(szKey, _T("DisplayDatesInISO"), m_bUseISOForDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowWeekdayInDates"), m_bShowWeekdayInDates);
	pPrefs->WriteProfileInt(szKey, _T("ShowParentsAsFolders"), m_bShowParentsAsFolders);
	pPrefs->WriteProfileInt(szKey, _T("MaxInfoTipCommentsLength"), max(m_nMaxInfoTipCommentsLength, 0));
	pPrefs->WriteProfileInt(szKey, _T("LimitInfoTipCommentsLength"), m_bLimitInfoTipCommentsLength);
	pPrefs->WriteProfileInt(szKey, _T("HidePercentForDoneTasks"), m_bHidePercentForDoneTasks);
	pPrefs->WriteProfileInt(szKey, _T("HideStartDueForDoneTasks"), m_bHideStartDueForDoneTasks);
	pPrefs->WriteProfileInt(szKey, _T("HideZeroTimeEst"), m_bHideZeroTimeCost);
	pPrefs->WriteProfileInt(szKey, _T("ShowPercentAsProgressbar"), m_bShowPercentAsProgressbar);
	pPrefs->WriteProfileInt(szKey, _T("RoundTimeFractions"), m_bRoundTimeFractions);
	pPrefs->WriteProfileInt(szKey, _T("ShowNonFilesAsText"), m_bShowNonFilesAsText);
	pPrefs->WriteProfileInt(szKey, _T("UseHMSTimeFormat"), m_bUseHMSTimeFormat);
	pPrefs->WriteProfileInt(szKey, _T("AutoFocusTasklist"), m_bAutoFocusTasklist);
	pPrefs->WriteProfileInt(szKey, _T("ShowColumnsOnRight"), m_bShowColumnsOnRight);
	pPrefs->WriteProfileInt(szKey, _T("LimitColumnWidths"), m_bLimitColumnWidths);
	pPrefs->WriteProfileInt(szKey, _T("MaxColumnWidth"), m_nMaxColumnWidth);
	pPrefs->WriteProfileInt(szKey, _T("AlwaysHideListParents"), m_bHideListParents);
	pPrefs->WriteProfileInt(szKey, _T("PercentIncrement"), m_nPercentIncrement);
	pPrefs->WriteProfileInt(szKey, _T("HideZeroPercentDone"), m_bHideZeroPercentDone);
	pPrefs->WriteProfileString(szKey, _T("DateTimeTrail"), m_sDateTimeTrailText);
	pPrefs->WriteProfileInt(szKey, _T("AppendUserToDateTimePaste"), m_bAppendUserToDateTimePaste);
	pPrefs->WriteProfileInt(szKey, _T("AppendTextToDateTimePaste"), m_bAppendTextToDateTimePaste);
	pPrefs->WriteProfileInt(szKey, _T("AllowCheckboxAgainstTreeItem"), m_bAllowCheckboxAgainstTreeItem);
	pPrefs->WriteProfileInt(szKey, _T("HidePaneSplitBar"), m_bHidePaneSplitBar);
	pPrefs->WriteProfileInt(szKey, _T("ShowRemindersAsDateAndTime"), m_bShowRemindersAsDateAndTime);
//	pPrefs->WriteProfileInt(szKey, _T(""), m_b);
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
