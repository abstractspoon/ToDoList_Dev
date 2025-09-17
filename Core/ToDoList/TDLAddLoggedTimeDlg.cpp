// TDLAddLoggedTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLAddLoggedTimeDlg.h"
#include "TDCDialogHelper.h"

#include "..\shared\DateHelper.h"
#include "..\Shared\localizer.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLAddLoggedTimeDlg dialog

CTDLAddLoggedTimeDlg::CTDLAddLoggedTimeDlg(DWORD dwTaskID, BOOL bEnableAddTimeToTimeSpent, 
										   BOOL bISODates, double dHours, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLAddLoggedTimeDlg::IDD, _T("AddLoggedTime"), pParent), 
	m_cbTimeWhen(TCB_HALFHOURS | TCB_HOURSINDAY),
	m_loggedTime(dHours, TDCU_HOURS),
	m_dwTaskID(dwTaskID),
	m_bEnableAddTimeToTimeSpent(bEnableAddTimeToTimeSpent),
	m_bTracked(dHours != 0.0)
{
	//{{AFX_DATA_INIT(CTDLAddLoggedTimeDlg)
	//}}AFX_DATA_INIT
	m_dtWhen = COleDateTime::GetCurrentTime();
	m_dtcWhen.SetISOFormat(bISODates);

	if (m_bTracked)
	{
		// Prevent changes to the time units and ensure positive times
		m_eLoggedTime.EnableUnitsChanges(FALSE);
		m_eLoggedTime.EnableNegativeTimes(FALSE);

		// If the time is less than 1 hour convert to minutes
		if (dHours < 1.0)
			m_loggedTime.SetTHUnits(THU_MINS, TRUE);
	}
	else
	{
		// Restore user's previous units choice
		TH_UNITS nUnits = CPreferences().GetProfileEnum(m_sPrefsKey, _T("AddLoggedTimeUnits"), THU_MINS);
		m_loggedTime.SetTHUnits(nUnits, TRUE);
	
		// Allow negative values only for untracked times
		m_eLoggedTime.EnableNegativeTimes(TRUE);
	}

	if (m_bEnableAddTimeToTimeSpent)
		m_bAddTimeToTimeSpent = CPreferences().GetProfileInt(m_sPrefsKey, _T("AddLoggedTimeToTimeSpent"), TRUE);
}

void CTDLAddLoggedTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLAddLoggedTimeDlg)
	DDX_Control(pDX, IDC_WHENDATE, m_dtcWhen);
	DDX_Control(pDX, IDC_WHENTIME, m_cbTimeWhen);
	DDX_Control(pDX, IDC_LOGGEDTIME, m_eLoggedTime);
	DDX_Check(pDX, IDC_ADDTIMETOTIMESPENT, m_bAddTimeToTimeSpent);
	DDX_Text(pDX, IDC_COMMENT, m_sComment);
	//}}AFX_DATA_MAP

	CTDCDialogHelper::DDX_Text(pDX, m_eLoggedTime, m_loggedTime);

	if (pDX->m_bSaveAndValidate)
	{
		m_dtcWhen.GetTime(m_dtWhen);
		COleDateTime time = m_cbTimeWhen.GetOleTime();

		m_dtWhen = CDateHelper::GetDateOnly(m_dtWhen) + time;
	}
	else
	{
		m_dtcWhen.SetTime(m_dtWhen);
		m_cbTimeWhen.SetOleTime(CDateHelper::GetTimeOnly(m_dtWhen));
	}
}


BEGIN_MESSAGE_MAP(CTDLAddLoggedTimeDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLAddLoggedTimeDlg)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_EN_CHANGE(IDC_LOGGEDTIME, OnChange)
	ON_EN_CHANGE(IDC_COMMENT, OnChange)
	ON_BN_CLICKED(IDC_ADDTIMETOTIMESPENT, OnChange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_WHENDATE, OnNotifyChange)
	ON_CBN_SELCHANGE(IDC_WHENTIME, OnChange)
	ON_CBN_EDITCHANGE(IDC_WHENTIME, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLAddLoggedTimeDlg message handlers

double CTDLAddLoggedTimeDlg::GetLoggedHours() const
{ 
	return m_loggedTime.GetTime(THU_HOURS);
}

COleDateTime CTDLAddLoggedTimeDlg::GetWhen() const
{
	return m_dtWhen;
}

void CTDLAddLoggedTimeDlg::OnOK()
{
	CTDLDialog::OnOK();

	CPreferences prefs;
	prefs.WriteProfileInt(m_sPrefsKey, _T("AddLoggedTimeUnits"), m_loggedTime.GetTHUnits());

	if (m_bEnableAddTimeToTimeSpent)
	{
		prefs.WriteProfileInt(m_sPrefsKey, _T("AddLoggedTimeToTimeSpent"), m_bAddTimeToTimeSpent);

		// Treat as 'cancel' if 'Apply' button is still disabled
		if (!GetDlgItem(IDAPPLY)->IsWindowEnabled())
			EndDialog(IDCANCEL);
	}
}

BOOL CTDLAddLoggedTimeDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	CLocalizer::EnableTranslation(::GetDlgItem(*this, IDC_TASKTITLE), FALSE);
	
	GetDlgItem(IDAPPLY)->EnableWindow(FALSE);
	GetDlgItem(IDC_ADDTIMETOTIMESPENT)->EnableWindow(m_bEnableAddTimeToTimeSpent);

	if (!m_bEnableAddTimeToTimeSpent)
	{
		GetDlgItem(IDAPPLY)->ShowWindow(SW_HIDE);
		
		CRect rApply = CDialogHelper::GetCtrlRect(this, IDAPPLY);
		CRect rCancel = CDialogHelper::GetCtrlRect(this, IDCANCEL);
		int nXOffset = (rApply.left - rCancel.left);
		
		CDialogHelper::OffsetCtrl(this, IDOK, nXOffset, 0);
		CDialogHelper::OffsetCtrl(this, IDCANCEL, nXOffset, 0);
	}
	
	// set focus to time spent if no time specified
	if (m_loggedTime.dAmount == 0.0)
	{
		m_eLoggedTime.SetFocus();
		m_eLoggedTime.SetSel(0, -1);

		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLAddLoggedTimeDlg::OnApply() 
{
	ASSERT(m_bEnableAddTimeToTimeSpent);

	// make sure we are up to date
	UpdateData();

	// notify parent
	CWnd* pParent = (m_pParentWnd ? m_pParentWnd : GetParent());
	ASSERT(pParent);

	if (pParent->SendMessage(WM_TDL_APPLYADDLOGGEDTIME, m_dwTaskID, (LPARAM)this))
	{
		// disable apply button until another change is made
		GetDlgItem(IDAPPLY)->EnableWindow(FALSE);
	}
}

void CTDLAddLoggedTimeDlg::OnChange() 
{
	// re-enable apply button
	if (m_bEnableAddTimeToTimeSpent)
		GetDlgItem(IDAPPLY)->EnableWindow(TRUE);
}

void CTDLAddLoggedTimeDlg::OnNotifyChange(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	OnChange();
	
	*pResult = 0;
}
