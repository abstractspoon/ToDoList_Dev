// PreferencesTaskPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesTaskPage.h"

#include "..\shared\Misc.h"
#include "..\shared\DateHelper.h"

//#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskPage property page

IMPLEMENT_DYNCREATE(CPreferencesTaskPage, CPreferencesPageBase)

CPreferencesTaskPage::CPreferencesTaskPage() : 
   CPreferencesPageBase(CPreferencesTaskPage::IDD)
{
	//{{AFX_DATA_INIT(CPreferencesTaskPage)
	//}}AFX_DATA_INIT

}

CPreferencesTaskPage::~CPreferencesTaskPage()
{
}

void CPreferencesTaskPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesTaskPage)
	DDX_CBString(pDX, IDC_HOURSINONEDAY, m_sHoursInDay);
	DDX_Check(pDX, IDC_LOGTIME, m_bLogTime);
	DDX_Check(pDX, IDC_LOGTASKSEPARATELY, m_bLogTasksSeparately);
	DDX_Check(pDX, IDC_EXCLUSIVETRACKING, m_bExclusiveTimeTracking);
	DDX_Check(pDX, IDC_ALLOWPARENTRACKING, m_bAllowParentTimeTracking);
	DDX_Check(pDX, IDC_DISPLAYLOGCONFIRM, m_bDisplayLogConfirm);
	DDX_Check(pDX, IDC_NOTIFYTIMETRACKING, m_bTrackReminder);
	DDX_Text(pDX, IDC_NOTIFYTIMETRACKINGFREQUENCY, m_nTrackReminderFrequency);
	DDX_Control(pDX, IDC_WEEKENDS, m_lbWeekends);
	DDX_Check(pDX, IDC_TRACKNONSELECTEDTASKS, m_bTrackNonSelectedTasks);
	DDX_Check(pDX, IDC_TRACKSCREENSAVER, m_bTrackOnScreenSaver);
	DDX_Check(pDX, IDC_TRACKNONACTIVETASKLISTS, m_bTrackNonActiveTasklists);
	DDX_Check(pDX, IDC_TRACKHIBERNATED, m_bTrackHibernated);
	DDX_Check(pDX, IDC_SHOWTIMETRACKER, m_bShowTimeTracker);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		m_dwWeekends = m_lbWeekends.GetChecked();
		m_nDaysInWeek = (7 - m_lbWeekends.GetNumChecked());
	}
	else
	{
		m_lbWeekends.SetChecked(m_dwWeekends);
		DDX_Text(pDX, IDC_DAYSINONEWEEK, m_nDaysInWeek);
	}
}


BEGIN_MESSAGE_MAP(CPreferencesTaskPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesTaskPage)
	ON_BN_CLICKED(IDC_LOGTIME, OnLogtime)
	ON_BN_CLICKED(IDC_NOTIFYTIMETRACKING, OnNotifyTimeTracking)
	//}}AFX_MSG_MAP
	ON_CONTROL(CLBN_CHKCHANGE, IDC_WEEKENDS, OnChangeWeekends)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskPage message handlers

BOOL CPreferencesTaskPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	m_mgrGroupLines.AddGroupLine(IDC_TRACKGROUP, *this); 
	m_mgrGroupLines.AddGroupLine(IDC_TIMEGROUP, *this); 

	GetDlgItem(IDC_LOGTASKSEPARATELY)->EnableWindow(m_bLogTime);
	GetDlgItem(IDC_NOTIFYTIMETRACKINGFREQUENCY)->EnableWindow(m_bTrackReminder);

	CDialogHelper::RefreshMaxColumnWidth(m_lbWeekends);

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

double CPreferencesTaskPage::GetHoursInOneDay() const
{
	double dHours = Misc::Atof(m_sHoursInDay);

	if (dHours <= 0 || dHours > 24)
		dHours = 8;

	return dHours;
}

double CPreferencesTaskPage::GetDaysInOneWeek() const
{
	return m_nDaysInWeek;
}

void CPreferencesTaskPage::OnLogtime() 
{
	UpdateData();
	
	GetDlgItem(IDC_LOGTASKSEPARATELY)->EnableWindow(m_bLogTime);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesTaskPage::OnChangeWeekends() 
{
	UpdateData(); // update m_nDaysInWeek
	UpdateData(FALSE); // write it back to edit field

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesTaskPage::LoadPreferences(const CPreferences& prefs)
{
	// load settings
	m_nDaysInWeek = prefs.GetProfileInt(_T("Preferences"), _T("DaysInWeek"), 5);
	m_sHoursInDay = prefs.GetProfileString(_T("Preferences"), _T("HoursInDay"), _T("8.00"));
	m_bLogTime = prefs.GetProfileInt(_T("Preferences"), _T("LogTime"), TRUE);
	m_bLogTasksSeparately = prefs.GetProfileInt(_T("Preferences"), _T("LogTasksSeparately"), FALSE);
	m_bExclusiveTimeTracking = prefs.GetProfileInt(_T("Preferences"), _T("ExclusiveTimeTracking"), TRUE);
	m_bAllowParentTimeTracking = prefs.GetProfileInt(_T("Preferences"), _T("AllowParentTimeTracking"), TRUE);
	m_bTrackNonActiveTasklists = prefs.GetProfileInt(_T("Preferences"), _T("TrackNonActiveTasklists"), TRUE);
	m_bTrackNonSelectedTasks = prefs.GetProfileInt(_T("Preferences"), _T("TrackNonSelectedTasks"), TRUE);
	m_bTrackOnScreenSaver = prefs.GetProfileInt(_T("Preferences"), _T("TrackOnScreenSaver"), TRUE);
	m_bTrackHibernated = prefs.GetProfileInt(_T("Preferences"), _T("AllowTrackingWhenHibernated"), FALSE);
	m_bDisplayLogConfirm = prefs.GetProfileInt(_T("Preferences"), _T("DisplayLogConfirm"), FALSE);
	m_bTrackReminder = prefs.GetProfileInt(_T("Preferences"), _T("TrackReminder"), FALSE);
	m_nTrackReminderFrequency = prefs.GetProfileInt(_T("Preferences"), _T("TrackReminderFrequency"), 5);
	m_bShowTimeTracker = prefs.GetProfileInt(_T("Preferences"), _T("ShowTimeTracker"), TRUE);

	if (m_nTrackReminderFrequency <= 0)
		m_bTrackReminder = FALSE;

	// weekends
	DWORD dwDefWeekend = DHW_SATURDAY | DHW_SUNDAY;

	if (GetDaysInOneWeek() >= 7)
		dwDefWeekend = 0; // some people work 7 days a week

	m_dwWeekends = prefs.GetProfileInt(_T("Preferences"), _T("Weekends"), dwDefWeekend);

//	m_b = prefs.GetProfileInt(_T("Preferences"), _T(""), FALSE);
}

void CPreferencesTaskPage::SavePreferences(CPreferences& prefs)
{
	// save settings
	prefs.WriteProfileInt(_T("Preferences"), _T("TrackNonSelectedTasks"), m_bTrackNonSelectedTasks);
	prefs.WriteProfileInt(_T("Preferences"), _T("TrackNonActiveTasklists"), m_bTrackNonActiveTasklists);
	prefs.WriteProfileInt(_T("Preferences"), _T("TrackOnScreenSaver"), m_bTrackOnScreenSaver);
	prefs.WriteProfileInt(_T("Preferences"), _T("AllowTrackingWhenHibernated"), m_bTrackHibernated);
	prefs.WriteProfileInt(_T("Preferences"), _T("LogTime"), m_bLogTime);
	prefs.WriteProfileInt(_T("Preferences"), _T("LogTasksSeparately"), m_bLogTasksSeparately);
	prefs.WriteProfileInt(_T("Preferences"), _T("ExclusiveTimeTracking"), m_bExclusiveTimeTracking);
	prefs.WriteProfileInt(_T("Preferences"), _T("AllowParentTimeTracking"), m_bAllowParentTimeTracking);
	prefs.WriteProfileInt(_T("Preferences"), _T("DisplayLogConfirm"), m_bDisplayLogConfirm);
	prefs.WriteProfileInt(_T("Preferences"), _T("TrackReminder"), m_bTrackReminder);
	prefs.WriteProfileInt(_T("Preferences"), _T("TrackReminderFrequency"), m_nTrackReminderFrequency);
	prefs.WriteProfileInt(_T("Preferences"), _T("ShowTimeTracker"), m_bShowTimeTracker);

	// validate time periods before writing
	m_sHoursInDay.Format(_T("%.2f"), GetHoursInOneDay());
	prefs.WriteProfileString(_T("Preferences"), _T("HoursInDay"), m_sHoursInDay);

	prefs.WriteProfileInt(_T("Preferences"), _T("DaysInWeek"), m_nDaysInWeek); // just for display purposes
	prefs.WriteProfileInt(_T("Preferences"), _T("Weekends"), m_dwWeekends);

//	prefs.WriteProfileInt(_T("Preferences"), _T(""), m_b);
}



void CPreferencesTaskPage::OnNotifyTimeTracking() 
{
	UpdateData();

	GetDlgItem(IDC_NOTIFYTIMETRACKINGFREQUENCY)->EnableWindow(m_bTrackReminder);
}
