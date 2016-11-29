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

void CPreferencesTaskPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	// load settings
	m_nDaysInWeek = pPrefs->GetProfileInt(szKey, _T("DaysInWeek"), 5);
	m_sHoursInDay = pPrefs->GetProfileString(szKey, _T("HoursInDay"), _T("8.00"));
	m_bLogTime = pPrefs->GetProfileInt(szKey, _T("LogTime"), TRUE);
	m_bLogTasksSeparately = pPrefs->GetProfileInt(szKey, _T("LogTasksSeparately"), FALSE);
	m_bExclusiveTimeTracking = pPrefs->GetProfileInt(szKey, _T("ExclusiveTimeTracking"), TRUE);
	m_bAllowParentTimeTracking = pPrefs->GetProfileInt(szKey, _T("AllowParentTimeTracking"), TRUE);
	m_bTrackNonActiveTasklists = pPrefs->GetProfileInt(szKey, _T("TrackNonActiveTasklists"), TRUE);
	m_bTrackNonSelectedTasks = pPrefs->GetProfileInt(szKey, _T("TrackNonSelectedTasks"), TRUE);
	m_bTrackOnScreenSaver = pPrefs->GetProfileInt(szKey, _T("TrackOnScreenSaver"), TRUE);
	m_bTrackHibernated = pPrefs->GetProfileInt(szKey, _T("AllowTrackingWhenHibernated"), FALSE);
	m_bDisplayLogConfirm = pPrefs->GetProfileInt(szKey, _T("DisplayLogConfirm"), FALSE);
	m_bTrackReminder = pPrefs->GetProfileInt(szKey, _T("TrackReminder"), FALSE);
	m_nTrackReminderFrequency = pPrefs->GetProfileInt(szKey, _T("TrackReminderFrequency"), 5);
	m_bShowTimeTracker = pPrefs->GetProfileInt(szKey, _T("ShowTimeTracker"), TRUE);

	if (m_nTrackReminderFrequency <= 0)
		m_bTrackReminder = FALSE;

	// weekends
	DWORD dwDefWeekend = DHW_SATURDAY | DHW_SUNDAY;

	if (GetDaysInOneWeek() >= 7)
		dwDefWeekend = 0; // some people work 7 days a week

	m_dwWeekends = pPrefs->GetProfileInt(szKey, _T("Weekends"), dwDefWeekend);

//	m_b = pPrefs->GetProfileInt(szKey, _T(""), FALSE);
}

void CPreferencesTaskPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileInt(szKey, _T("TrackNonSelectedTasks"), m_bTrackNonSelectedTasks);
	pPrefs->WriteProfileInt(szKey, _T("TrackNonActiveTasklists"), m_bTrackNonActiveTasklists);
	pPrefs->WriteProfileInt(szKey, _T("TrackOnScreenSaver"), m_bTrackOnScreenSaver);
	pPrefs->WriteProfileInt(szKey, _T("AllowTrackingWhenHibernated"), m_bTrackHibernated);
	pPrefs->WriteProfileInt(szKey, _T("LogTime"), m_bLogTime);
	pPrefs->WriteProfileInt(szKey, _T("LogTasksSeparately"), m_bLogTasksSeparately);
	pPrefs->WriteProfileInt(szKey, _T("ExclusiveTimeTracking"), m_bExclusiveTimeTracking);
	pPrefs->WriteProfileInt(szKey, _T("AllowParentTimeTracking"), m_bAllowParentTimeTracking);
	pPrefs->WriteProfileInt(szKey, _T("DisplayLogConfirm"), m_bDisplayLogConfirm);
	pPrefs->WriteProfileInt(szKey, _T("TrackReminder"), m_bTrackReminder);
	pPrefs->WriteProfileInt(szKey, _T("TrackReminderFrequency"), m_nTrackReminderFrequency);
	pPrefs->WriteProfileInt(szKey, _T("ShowTimeTracker"), m_bShowTimeTracker);

	// validate time periods before writing
	CString sHoursInDay;
	sHoursInDay.Format(_T("%.2f"), GetHoursInOneDay());
	pPrefs->WriteProfileString(szKey, _T("HoursInDay"), sHoursInDay);

	pPrefs->WriteProfileInt(szKey, _T("DaysInWeek"), m_nDaysInWeek); // just for display purposes
	pPrefs->WriteProfileInt(szKey, _T("Weekends"), m_dwWeekends);

//	pPrefs->WriteProfileInt(szKey, _T(""), m_b);
}



void CPreferencesTaskPage::OnNotifyTimeTracking() 
{
	UpdateData();

	GetDlgItem(IDC_NOTIFYTIMETRACKINGFREQUENCY)->EnableWindow(m_bTrackReminder);
}
