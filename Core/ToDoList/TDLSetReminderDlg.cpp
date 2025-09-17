// TDLReminderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLSetReminderDlg.h"
#include "filteredtodoctrl.h"
#include "todoctrlreminders.h"

#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\graphicsmisc.h"
#include "..\Shared\localizer.h"
#include "..\Shared\DateHelper.h"

#include "..\Interfaces\Preferences.h"

#pragma warning(disable: 4201)
#include <Mmsystem.h> 

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLSetReminderDlg dialog

CTDLSetReminderDlg::CTDLSetReminderDlg(HICON hIcon, BOOL bISODateTimes, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLSetReminderDlg::IDD, _T("Reminders"), pParent), 
	m_cbAbsoluteTime(TCB_HALFHOURS | TCB_HOURSINDAY),
	m_cbLeadIn(TDLRPC_SHOWZERO),
	m_bRelativeFromDueDate(0),
	m_dRelativeLeadInHours(0.25), // 15 mins
	m_bRelative(TRUE),
	m_dtAbsoluteDate(COleDateTime::GetCurrentTime()),
	m_dAbsoluteTime(CDateHelper::GetTimeOnly(m_dtAbsoluteDate))
{
	m_iconDlg.SetIcon(hIcon, FALSE); // not owned
	m_dtcAbsolute.SetISOFormat(bISODateTimes);
	m_cbAbsoluteTime.SetISOFormat(bISODateTimes);
}

void CTDLSetReminderDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLSetReminderDlg)
	DDX_Control(pDX, IDC_ABSOLUTETIME, m_cbAbsoluteTime);
	DDX_Control(pDX, IDC_SOUNDFILE, m_ePlaySound);
	DDX_Control(pDX, IDC_ABSOLUTEDATE, m_dtcAbsolute);
	DDX_CBIndex(pDX, IDC_RELATIVEFROMWHERE, m_bRelativeFromDueDate);
	DDX_Control(pDX, IDC_RELATIVELEADIN, m_cbLeadIn);
	DDX_Text(pDX, IDC_SOUNDFILE, m_sSoundFile);
	DDX_Radio(pDX, IDC_ABSOLUTE, m_bRelative);
	DDX_DateTimeCtrl(pDX, IDC_ABSOLUTEDATE, m_dtAbsoluteDate);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_PLAYSOUND, m_bPlaySound);

	if (pDX->m_bSaveAndValidate)
	{
		m_dRelativeLeadInHours = (m_cbLeadIn.GetSelectedPeriod() / 60.0);
		m_dAbsoluteTime = m_cbAbsoluteTime.GetOleTime();
	}
	else
	{
		m_cbLeadIn.SetSelectedPeriod((UINT)(m_dRelativeLeadInHours * 60));
		m_cbAbsoluteTime.SetOleTime(m_dAbsoluteTime);
	}
}

BEGIN_MESSAGE_MAP(CTDLSetReminderDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLSetReminderDlg)
	ON_CBN_SELCHANGE(IDC_RELATIVELEADIN, OnSelchangeLeadin)
	ON_BN_CLICKED(IDC_RELATIVE, OnChangeRelative)
	ON_BN_CLICKED(IDC_ABSOLUTE, OnChangeRelative)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DISMISS, OnDismissReminder)
	ON_BN_CLICKED(IDC_PLAYSOUND, OnClickPlaySound)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLSetReminderDlg message handlers

int CTDLSetReminderDlg::DoModal(TDCREMINDER& rem, BOOL bNewReminder)
{
	CPreferences prefs;
	LoadPreferences(prefs);

	// If it's not a new reminder overwrite the 
	// loaded state with reminder's actual state
	if (!bNewReminder)
	{
		m_bRelative = rem.bRelative;

		if (m_bRelative)
		{
			m_dRelativeLeadInHours = (rem.dRelativeDaysLeadIn * 24);
			m_bRelativeFromDueDate = (rem.nRelativeFromWhen == TDCR_DUEDATE);

			// init absolute date and time to now
			m_dtAbsoluteDate = GetNextNearestHour();
		}
		else
		{
			m_dtAbsoluteDate = (rem.dtAbsolute.m_dt + rem.dDaysSnooze);
		}

		m_dAbsoluteTime = CDateHelper::GetTimeOnly(m_dtAbsoluteDate);
		m_sModifyDlgTitle.LoadString(IDS_MODIFYTASKREMINDER_TITLE);
		m_bPlaySound = !rem.sSoundFile.IsEmpty();

		if (m_bPlaySound)
			m_sSoundFile = rem.sSoundFile;
	}

	int nRes = CTDLDialog::DoModal();

	if (nRes == IDOK)
	{
		// update reminder
		rem.sSoundFile = (m_bPlaySound ? m_sSoundFile : _T(""));
		rem.bRelative = m_bRelative;
		rem.dDaysSnooze = 0; // always
		rem.nLastSnoozeMins = 0; // always
		rem.bEnabled = TRUE;
		
		if (m_bRelative)
		{
			rem.nRelativeFromWhen = (m_bRelativeFromDueDate ? TDCR_DUEDATE : TDCR_STARTDATE);
			rem.dRelativeDaysLeadIn = (m_dRelativeLeadInHours / 24);
		}
		else
		{
			rem.dtAbsolute = CDateHelper::MakeDate(m_dtAbsoluteDate, m_dAbsoluteTime);
		}

		SavePreferences(prefs);
	}

	return nRes;
}

BOOL CTDLSetReminderDlg::OnInitDialog()
{
	CTDLDialog::OnInitDialog();
	CLocalizer::EnableTranslation(::GetDlgItem(*this, IDC_TASKTITLE), FALSE);

	// init radio button ctrls
	GetDlgItem(IDC_ABSOLUTEDATE)->EnableWindow(!m_bRelative);
	GetDlgItem(IDC_ABSOLUTETIME)->EnableWindow(!m_bRelative);
	GetDlgItem(IDC_RELATIVEFROMWHERE)->EnableWindow(m_bRelative);
	GetDlgItem(IDC_RELATIVELEADIN)->EnableWindow(m_bRelative);
	GetDlgItem(IDC_SOUNDFILE)->EnableWindow(m_bPlaySound);
	
	if (!m_sModifyDlgTitle.IsEmpty())
	{
		SetWindowText(m_sModifyDlgTitle);
	}
	else
	{
		GetDlgItem(IDC_DISMISS)->EnableWindow(FALSE);
		GetDlgItem(IDC_DISMISS)->ShowWindow(SW_HIDE);
	}

	return TRUE;
}

void CTDLSetReminderDlg::OnClickPlaySound()
{
	UpdateData();

	GetDlgItem(IDC_SOUNDFILE)->EnableWindow(m_bPlaySound);
}

void CTDLSetReminderDlg::OnSelchangeLeadin() 
{
	UpdateData();

	m_dRelativeLeadInHours = (m_cbLeadIn.GetSelectedPeriod() / 60.0); // in hours
}

void CTDLSetReminderDlg::OnChangeRelative() 
{
	UpdateData();

	GetDlgItem(IDC_ABSOLUTEDATE)->EnableWindow(!m_bRelative);
	GetDlgItem(IDC_ABSOLUTETIME)->EnableWindow(!m_bRelative);
	GetDlgItem(IDC_RELATIVEFROMWHERE)->EnableWindow(m_bRelative);
	GetDlgItem(IDC_RELATIVELEADIN)->EnableWindow(m_bRelative);
}

COleDateTime CTDLSetReminderDlg::GetNextNearestHour()
{
	COleDateTime dtCur(COleDateTime::GetCurrentTime());
	COleDateTime dtNearest = CDateHelper::GetNearestHour(dtCur, FALSE);

	if (dtNearest < dtCur)
		dtNearest.m_dt += (1.0 / 24.0);

	return dtNearest;
}

void CTDLSetReminderDlg::OnDismissReminder()
{
	EndDialog(IDDISMISS);
}

void CTDLSetReminderDlg::LoadPreferences(const CPreferences& prefs)
{
	m_bRelative = prefs.GetProfileInt(m_sPrefsKey, _T("Relative"), TRUE);
	m_dRelativeLeadInHours = prefs.GetProfileDouble(m_sPrefsKey, _T("LeadIn"), 0.25); // 15 mins
	m_bRelativeFromDueDate = prefs.GetProfileInt(m_sPrefsKey, _T("RelativeFromDue"), TRUE);
	m_bPlaySound = prefs.GetProfileInt(m_sPrefsKey, _T("PlaySound"), -1);
	m_sSoundFile = prefs.GetProfileString(m_sPrefsKey, _T("SoundFile"), m_sSoundFile);

	// Backwards compatibility
	const LPCTSTR NO_SOUND = _T("None");

	if (m_sSoundFile == NO_SOUND)
	{
		m_sSoundFile.Empty();
		m_bPlaySound = FALSE;
	}
	else if (m_bPlaySound == -1) // First time only
	{
		if (m_sSoundFile.IsEmpty())
			m_sSoundFile = CSoundEdit::GetWindowsSound(_T("tada"));

		m_bPlaySound = !m_sSoundFile.IsEmpty();
	}

	// init absolute date and time to now
	m_dtAbsoluteDate = GetNextNearestHour();
	m_dAbsoluteTime = CDateHelper::GetTimeOnly(m_dtAbsoluteDate);
}

void CTDLSetReminderDlg::SavePreferences(CPreferences& prefs) const
{
	prefs.WriteProfileInt(m_sPrefsKey, _T("Relative"), m_bRelative);
	prefs.WriteProfileInt(m_sPrefsKey, _T("RelativeFromDue"), m_bRelativeFromDueDate);
	prefs.WriteProfileDouble(m_sPrefsKey, _T("LeadIn"), m_dRelativeLeadInHours);
	prefs.WriteProfileString(m_sPrefsKey, _T("SoundFile"), m_sSoundFile);
	prefs.WriteProfileInt(m_sPrefsKey, _T("PlaySound"), m_bPlaySound);
}

