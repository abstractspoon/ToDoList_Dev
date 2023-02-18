// PreferencesTaskPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesTaskPage.h"

#include "..\shared\Misc.h"
#include "..\shared\FileMisc.h"
#include "..\shared\DateHelper.h"
#include "..\shared\EnString.h"
#include "..\shared\TimeHelper.h"
#include "..\shared\WorkingWeek.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define NO_SOUND _T("None")

/////////////////////////////////////////////////////////////////////////////

const UINT WM_PTP_ENABLEDISABLE = (WM_APP + 1);

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskPage property page

IMPLEMENT_DYNCREATE(CPreferencesTaskPage, CPreferencesPageBase)

CPreferencesTaskPage::CPreferencesTaskPage() 
	: 
	CPreferencesPageBase(CPreferencesTaskPage::IDD),
	m_cbStartOfWorkday(TCB_HALFHOURS),
	m_cbStartOfLunch(TCB_HALFHOURS | TCB_HOURSINDAY),
	m_cbEndOfLunch(TCB_HALFHOURS | TCB_HOURSINDAY)
{
	//{{AFX_DATA_INIT(CPreferencesTaskPage)
	//}}AFX_DATA_INIT

	m_sTrackReminderSoundFile = FileMisc::GetWindowsFolder() + _T("\\media\\chimes.wav"); // default
}

CPreferencesTaskPage::~CPreferencesTaskPage()
{
}

void CPreferencesTaskPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesTaskPage)
	DDX_Control(pDX, IDC_ENDOFLUNCH, m_cbEndOfLunch);
	DDX_Control(pDX, IDC_STARTOFLUNCH, m_cbStartOfLunch);
	DDX_Control(pDX, IDC_STARTOFDAY, m_cbStartOfWorkday);
	DDX_CBString(pDX, IDC_HOURSINONEDAY, m_sHoursInDay);
	DDX_Check(pDX, IDC_LOGTIME, m_bLogTime);
	DDX_Check(pDX, IDC_LOGTASKSEPARATELY, m_bLogTasksSeparately);
	DDX_Check(pDX, IDC_EXCLUSIVETRACKING, m_bExclusiveTimeTracking);
	DDX_Check(pDX, IDC_ALLOWPARENTRACKING, m_bAllowParentTimeTracking);
	DDX_Check(pDX, IDC_DISPLAYLOGCONFIRM, m_bDisplayLogConfirm);
	DDX_Check(pDX, IDC_NOTIFYTIMETRACKING, m_bTrackReminder);
	DDX_Text(pDX, IDC_NOTIFYTIMETRACKINGFREQUENCY, m_nTrackReminderFrequency);
	DDX_Check(pDX, IDC_SHOWTIMETRACKER, m_bShowTimeTracker);
	DDX_Control(pDX, IDC_WEEKENDS, m_lbWeekends);
	DDX_Check(pDX, IDC_TRACKNONSELECTEDTASKS, m_bTrackNonSelectedTasks);
	DDX_Check(pDX, IDC_TRACKSCREENSAVER, m_bTrackOnScreenSaver);
	DDX_Check(pDX, IDC_TRACKNONACTIVETASKLISTS, m_bTrackNonActiveTasklists);
	DDX_Check(pDX, IDC_TRACKHIBERNATED, m_bTrackHibernated);
	DDX_Check(pDX, IDC_HASLUNCHBREAK, m_bHasLunchBreak);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_PLAYSOUND, m_sTrackReminderSoundFile);
	DDX_Check(pDX, IDC_ENDTRACKINGONREMINDER, m_bEndTrackingOnReminder);
	DDX_Control(pDX, IDC_PLAYSOUND, m_ePlaySound);

	m_cbStartOfWorkday.DDX(pDX, m_dStartOfWorkdayInHours);
	m_cbStartOfLunch.DDX(pDX, m_dStartOfLunchInHours);
	m_cbEndOfLunch.DDX(pDX, m_dEndOfLunchInHours);

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
	ON_BN_CLICKED(IDC_HASLUNCHBREAK, OnHasLunchBreak)
	ON_CBN_EDITUPDATE(IDC_HOURSINONEDAY, OnEditChangeHoursInDay)
	ON_CBN_EDITCHANGE(IDC_HOURSINONEDAY, OnEditChangeHoursInDay)
	ON_CBN_SELENDOK(IDC_HOURSINONEDAY, OnComboChangeHoursInDay)
	ON_CBN_KILLFOCUS(IDC_HOURSINONEDAY, OnKillFocusHoursInDay)
	//}}AFX_MSG_MAP
	ON_CONTROL(CLBN_CHKCHANGE, IDC_WEEKENDS, OnChangeWeekends)
	ON_MESSAGE(WM_PTP_ENABLEDISABLE, OnEnableDisableCtrls)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskPage message handlers

BOOL CPreferencesTaskPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesTaskPage::OnFirstShow()
{
	CPreferencesPageBase::OnFirstShow();

	AddGroupLine(IDC_TRACKGROUP); 
	AddGroupLine(IDC_TIMEGROUP); 
	
	CDialogHelper::RefreshMaxColumnWidth(m_lbWeekends);

	EnableDisableControls();
	UpdateData(FALSE);
}

void CPreferencesTaskPage::EnableDisableControls()
{
	GetDlgItem(IDC_LOGTASKSEPARATELY)->EnableWindow(m_bLogTime);
	GetDlgItem(IDC_NOTIFYTIMETRACKINGFREQUENCY)->EnableWindow(m_bTrackReminder);
	GetDlgItem(IDC_PLAYSOUND)->EnableWindow(m_bTrackReminder);
	GetDlgItem(IDC_ENDTRACKINGONREMINDER)->EnableWindow(m_bTrackReminder);

	BOOL bWholeDay = (m_sHoursInDay.IsEmpty() || (GetHoursInDay() == 24));

	GetDlgItem(IDC_STARTOFDAY)->EnableWindow(!bWholeDay);
	GetDlgItem(IDC_HASLUNCHBREAK)->EnableWindow(!bWholeDay);
	GetDlgItem(IDC_STARTOFLUNCH)->EnableWindow(!bWholeDay && m_bHasLunchBreak);
	GetDlgItem(IDC_ENDOFLUNCH)->EnableWindow(!bWholeDay && m_bHasLunchBreak);
	GetDlgItem(IDC_DAYSINONEWEEK)->EnableWindow(FALSE); // always
}

void CPreferencesTaskPage::OnOK()
{
	// Update working week
	CheckSetWorkingWeek();
}

void CPreferencesTaskPage::CheckSetWorkingWeek()
{
	ValidateWorkingWeek();

	VERIFY(CWorkingWeek::Initialise(m_dwWeekends,
									GetHoursInDay(),
									m_dStartOfWorkdayInHours,
									m_dStartOfLunchInHours,
									m_bHasLunchBreak ? m_dEndOfLunchInHours : m_dStartOfLunchInHours));
}

void CPreferencesTaskPage::ValidateWorkingWeek()
{
	double dHoursInDay = GetHoursInDay();

	m_sHoursInDay = Misc::Format(dHoursInDay, 2);

	if (dHoursInDay == 24)
		m_bHasLunchBreak = FALSE;

	m_dStartOfWorkdayInHours = min(m_dStartOfWorkdayInHours, (24 - dHoursInDay));

	double dEndOfWorkdayInHours = (m_dStartOfWorkdayInHours + dHoursInDay);

	m_dStartOfLunchInHours = max(m_dStartOfWorkdayInHours, m_dStartOfLunchInHours);
	m_dStartOfLunchInHours = min(dEndOfWorkdayInHours, m_dStartOfLunchInHours);

	m_dEndOfLunchInHours = max(m_dEndOfLunchInHours, m_dStartOfLunchInHours);
	m_dEndOfLunchInHours = min(m_dEndOfLunchInHours, dEndOfWorkdayInHours);

	if (GetSafeHwnd())
		UpdateData(FALSE);
}

double CPreferencesTaskPage::GetHoursInDay() const
{
	double dHours = Misc::Atof(m_sHoursInDay);

	if (dHours <= 0 || dHours > 24)
		dHours = 8;

	return dHours;
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
	m_bEndTrackingOnReminder = pPrefs->GetProfileInt(szKey, _T("EndTrackingOnReminder"), FALSE);

	m_nDaysInWeek = pPrefs->GetProfileInt(szKey, _T("DaysInWeek"), 5);
	m_sHoursInDay = pPrefs->GetProfileString(szKey, _T("HoursInDay"), _T("8.00"));
	m_dEndOfLunchInHours = pPrefs->GetProfileDouble(szKey, _T("EndOfLunchInHours"), 14.0);
	m_dStartOfLunchInHours = pPrefs->GetProfileDouble(szKey, _T("StartOfLunchInHours"), 13.0);
	m_dStartOfWorkdayInHours = pPrefs->GetProfileDouble(szKey, _T("StartOfWorkdayInHours"), 9.0);
	
	if (m_dEndOfLunchInHours <= m_dStartOfLunchInHours)
		m_bHasLunchBreak = FALSE;
	else
		m_bHasLunchBreak = pPrefs->GetProfileInt(szKey, _T("HasLunchBreak"), TRUE);

	m_sTrackReminderSoundFile = pPrefs->GetProfileString(_T("Reminders"), _T("SoundFile"), m_sTrackReminderSoundFile);

	if (m_sTrackReminderSoundFile == NO_SOUND)
		m_sTrackReminderSoundFile.Empty();

	if (m_nTrackReminderFrequency <= 0)
		m_bTrackReminder = FALSE;

	// weekends
	DWORD dwDefWeekend = DHW_SATURDAY | DHW_SUNDAY;

	if (m_nDaysInWeek >= 7)
		dwDefWeekend = 0; // some people work 7 days a week

	m_dwWeekends = pPrefs->GetProfileInt(szKey, _T("Weekends"), dwDefWeekend);

	CheckSetWorkingWeek();

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
	pPrefs->WriteProfileInt(szKey, _T("EndTrackingOnReminder"), m_bEndTrackingOnReminder);
	pPrefs->WriteProfileInt(szKey, _T("HasLunchBreak"), m_bHasLunchBreak);
	pPrefs->WriteProfileInt(szKey, _T("DaysInWeek"), m_nDaysInWeek); // just for display purposes
	pPrefs->WriteProfileInt(szKey, _T("Weekends"), m_dwWeekends);
	
	pPrefs->WriteProfileDouble(szKey, _T("EndOfLunchInHours"), m_dEndOfLunchInHours);
	pPrefs->WriteProfileDouble(szKey, _T("StartOfLunchInHours"), m_dStartOfLunchInHours);
	pPrefs->WriteProfileDouble(szKey, _T("StartOfWorkdayInHours"), m_dStartOfWorkdayInHours);

	pPrefs->WriteProfileString(szKey, _T("HoursInDay"), Misc::Format(GetHoursInDay(), 2));
	pPrefs->WriteProfileString(_T("Reminders"), _T("SoundFile"), m_sTrackReminderSoundFile.IsEmpty() ? NO_SOUND : m_sTrackReminderSoundFile);

//	pPrefs->WriteProfileInt(szKey, _T(""), m_b);
}

void CPreferencesTaskPage::OnNotifyTimeTracking() 
{
	UpdateData();

	GetDlgItem(IDC_NOTIFYTIMETRACKINGFREQUENCY)->EnableWindow(m_bTrackReminder);
	GetDlgItem(IDC_PLAYSOUND)->EnableWindow(m_bTrackReminder);
	GetDlgItem(IDC_ENDTRACKINGONREMINDER)->EnableWindow(m_bTrackReminder);
}

void CPreferencesTaskPage::OnHasLunchBreak() 
{
	UpdateData();

	GetDlgItem(IDC_STARTOFLUNCH)->EnableWindow(m_bHasLunchBreak);
	GetDlgItem(IDC_ENDOFLUNCH)->EnableWindow(m_bHasLunchBreak);
}

void CPreferencesTaskPage::OnEditChangeHoursInDay() 
{
	UpdateData();
	ValidateWorkingWeek();

	PostMessage(WM_PTP_ENABLEDISABLE);
}

void CPreferencesTaskPage::OnComboChangeHoursInDay()
{
	// We have to get the text via the combo's selected index
	// because the text is not updated until after the notification
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_HOURSINONEDAY);
	ASSERT(pCombo);

	m_sHoursInDay = GetSelectedItem(*pCombo);
	ValidateWorkingWeek();

	PostMessage(WM_PTP_ENABLEDISABLE);
}

void CPreferencesTaskPage::OnKillFocusHoursInDay()
{
	CString sHours = Misc::Format(GetHoursInDay(), 2);
	Misc::TrimTrailingDecimalZeros(sHours);

	if (sHours != m_sHoursInDay)
	{
		m_sHoursInDay = sHours;

		UpdateDataEx(this, IDC_HOURSINONEDAY, m_sHoursInDay, FALSE);
		EnableDisableControls();
	}
}

LRESULT CPreferencesTaskPage::OnEnableDisableCtrls(WPARAM /*wp*/, LPARAM /*lp*/)
{
	UpdateData();
	EnableDisableControls();

	return 0L;
}

