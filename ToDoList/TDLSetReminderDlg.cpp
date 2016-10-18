// TDLReminderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLSetReminderDlg.h"
#include "filteredtodoctrl.h"
#include "todoctrlreminders.h"

#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\preferences.h"
#include "..\shared\graphicsmisc.h"
#include "..\Shared\localizer.h"

#pragma warning(disable: 4201)
#include <Mmsystem.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

// for PlaySound
#pragma comment(lib, "winmm.lib")

/////////////////////////////////////////////////////////////////////////////
// CTDLSetReminderDlg dialog

#define ID_PLAYSOUNDBTN 0xfff0
#define NO_SOUND _T("None")

CTDLSetReminderDlg::CTDLSetReminderDlg(CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLSetReminderDlg::IDD, pParent), 
	m_cbAbsoluteTime(TCB_HALFHOURS),
	m_cbLeadIn(TDLRPC_SHOWZERO)
{
	//{{AFX_DATA_INIT(CTDLSetReminderDlg)
	m_bRelativeFromDueDate = 0;
	m_sSoundFile = _T("");
	m_dRelativeLeadIn = 0.25; // 15 mins
	m_bRelative = TRUE;
	m_dtAbsoluteDate = COleDateTime::GetCurrentTime();
	m_sTaskTitle = _T("");
	//}}AFX_DATA_INIT
	m_dAbsoluteTime = CDateHelper::GetTimeOnly(m_dtAbsoluteDate);

	m_ePlaySound.SetFilter(CEnString(IDS_SOUNDFILEFILTER));
	m_ePlaySound.AddButton(ID_PLAYSOUNDBTN, 0x38, CEnString(IDS_PLAYSOUNDBTNTIP), CALC_BTNWIDTH, _T("Marlett"));
	m_ePlaySound.SetButtonTip(FEBTN_BROWSE, CEnString(IDS_BROWSE));
}


void CTDLSetReminderDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLSetReminderDlg)
	DDX_Control(pDX, IDC_ABSOLUTETIME, m_cbAbsoluteTime);
	DDX_Control(pDX, IDC_PLAYSOUND, m_ePlaySound);
	DDX_CBIndex(pDX, IDC_RELATIVEFROMWHERE, m_bRelativeFromDueDate);
	DDX_Control(pDX, IDC_RELATIVELEADIN, m_cbLeadIn);
	DDX_Text(pDX, IDC_PLAYSOUND, m_sSoundFile);
	DDX_Radio(pDX, IDC_ABSOLUTE, m_bRelative);
	DDX_DateTimeCtrl(pDX, IDC_ABSOLUTEDATE, m_dtAbsoluteDate);
	DDX_Text(pDX, IDC_TASKTITLE, m_sTaskTitle);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TASKTITLE, m_stTaskTitle);

	if (pDX->m_bSaveAndValidate)
	{
		m_dRelativeLeadIn = (m_cbLeadIn.GetSelectedPeriod() / 60.0);
		m_dAbsoluteTime = m_cbAbsoluteTime.GetOleTime();
	}
	else
	{
		m_cbLeadIn.SetSelectedPeriod((UINT)(m_dRelativeLeadIn * 60));
		m_cbAbsoluteTime.SetOleTime(m_dAbsoluteTime);
	}
}


BEGIN_MESSAGE_MAP(CTDLSetReminderDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLSetReminderDlg)
	ON_CBN_SELCHANGE(IDC_RELATIVELEADIN, OnSelchangeLeadin)
	ON_BN_CLICKED(IDC_RELATIVE, OnChangeRelative)
	ON_BN_CLICKED(IDC_ABSOLUTE, OnChangeRelative)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnPlaySound)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLSetReminderDlg message handlers

int CTDLSetReminderDlg::DoModal(TDCREMINDER& rem, BOOL bNewReminder)
{
	CPreferences prefs;

	if (bNewReminder)
	{
		m_bRelative = prefs.GetProfileInt(_T("Reminders"), _T("Relative"), TRUE);
		m_dRelativeLeadIn = prefs.GetProfileDouble(_T("Reminders"), _T("LeadIn"), 0.25); // 15 mins
		m_bRelativeFromDueDate = prefs.GetProfileInt(_T("Reminders"), _T("RelativeFromDue"), TRUE);
		m_sSoundFile = prefs.GetProfileString(_T("Reminders"), _T("SoundFile"), m_sSoundFile);

		if (m_sSoundFile == NO_SOUND)
		{
			m_sSoundFile.Empty();
		}
		else if (m_sSoundFile.IsEmpty()) // backwards compatibility
		{
			m_sSoundFile = FileMisc::GetWindowsFolder() + _T("\\media\\tada.wav");
		}
		
		// init absolute date and time to now
		m_dtAbsoluteDate = GetNextNearestHour();
		m_dAbsoluteTime = CDateHelper::GetTimeOnly(m_dtAbsoluteDate);
	}
	else
	{
		m_bRelative = rem.bRelative;

		if (m_bRelative)
		{
			m_dRelativeLeadIn = (rem.dRelativeDaysLeadIn * 24);
			m_bRelativeFromDueDate = (rem.nRelativeFromWhen == TDCR_DUEDATE);

			// init absolute date and time to now
			m_dtAbsoluteDate = GetNextNearestHour();
		}
		else
		{
			m_dtAbsoluteDate = (rem.dtAbsolute.m_dt + rem.dDaysSnooze);
		}

		m_dAbsoluteTime = CDateHelper::GetTimeOnly(m_dtAbsoluteDate);
		m_sSoundFile = rem.sSoundFile;
		m_sModifyDlgTitle.LoadString(IDS_MODIFYTASKREMINDER_TITLE);
	}

	// verify relative date is feasible
	BOOL bTaskHasDue = CDateHelper::IsDateSet(rem.pTDC->GetTaskDate(rem.dwTaskID, TDCD_DUE));
	BOOL bTaskHasStart = CDateHelper::IsDateSet(rem.pTDC->GetTaskDate(rem.dwTaskID, TDCD_START));

	if (m_bRelativeFromDueDate)
	{
		if (!bTaskHasDue && bTaskHasStart)
			m_bRelativeFromDueDate = FALSE;
	}
	else // relative from start
	{
		if (bTaskHasDue && !bTaskHasStart)
			m_bRelativeFromDueDate = TRUE;
	}

	m_sTaskTitle = rem.GetTaskTitle();
	m_sTaskTitle.Replace(_T("&"), _T("&&"));
		
	int nRes = CTDLDialog::DoModal();

	// save off last specified sound file to be a hint next time
	if (nRes == IDOK)
	{
		// update reminder
		rem.sSoundFile = m_sSoundFile;
		rem.bRelative = m_bRelative;
		rem.dDaysSnooze = 0; // always
		rem.nLastSnoozeMins = 0; // always
		rem.bEnabled = TRUE;
		
		if (m_bRelative)
		{
			rem.nRelativeFromWhen = (m_bRelativeFromDueDate ? TDCR_DUEDATE : TDCR_STARTDATE);
			rem.dRelativeDaysLeadIn = (m_dRelativeLeadIn / 24);
		}
		else
		{
			rem.dtAbsolute = CDateHelper::MakeDate(m_dtAbsoluteDate, m_dAbsoluteTime);
		}
		
		// save for next time a new reminder is created
		CPreferences prefs;
		
		prefs.WriteProfileInt(_T("Reminders"), _T("Relative"), m_bRelative);
		prefs.WriteProfileInt(_T("Reminders"), _T("RelativeFromDue"), m_bRelativeFromDueDate);
		prefs.WriteProfileDouble(_T("Reminders"), _T("LeadIn"), m_dRelativeLeadIn);
		prefs.WriteProfileString(_T("Reminders"), _T("SoundFile"), m_sSoundFile.IsEmpty() ? NO_SOUND : m_sSoundFile);
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
	
	// embolden task title
	m_stTaskTitle.SetFontStyle(TRUE);

	if (!m_sModifyDlgTitle.IsEmpty())
		SetWindowText(m_sModifyDlgTitle);

	return TRUE;
}

void CTDLSetReminderDlg::OnSelchangeLeadin() 
{
	UpdateData();

	m_dRelativeLeadIn = (m_cbLeadIn.GetSelectedPeriod() / 60.0); // in hours
}

LRESULT CTDLSetReminderDlg::OnPlaySound(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDC_PLAYSOUND && lParam == ID_PLAYSOUNDBTN)
	{
		UpdateData();

		if (!m_sSoundFile.IsEmpty())
			PlaySound(m_sSoundFile, NULL, (SND_FILENAME | SND_ASYNC));

		return TRUE;
	}
	
	return 0;
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

