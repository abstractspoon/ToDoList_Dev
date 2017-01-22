// PreferencesTaskPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesTaskCalcPage.h"

#include "..\shared\Misc.h"
#include "..\shared\enstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskCalcPage property page

IMPLEMENT_DYNCREATE(CPreferencesTaskCalcPage, CPreferencesPageBase)

CPreferencesTaskCalcPage::CPreferencesTaskCalcPage() : 
   CPreferencesPageBase(CPreferencesTaskCalcPage::IDD)
{
	//{{AFX_DATA_INIT(CPreferencesTaskCalcPage)
	//}}AFX_DATA_INIT

}

CPreferencesTaskCalcPage::~CPreferencesTaskCalcPage()
{
}

void CPreferencesTaskCalcPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesTaskCalcPage)
	DDX_Control(pDX, IDC_RECENTMODPERIOD, m_eRecentModPeriod);
	DDX_Check(pDX, IDC_TREATSUBCOMPLETEDASDONE, m_bTreatSubCompletedAsDone);
	DDX_Check(pDX, IDC_USEHIGHESTPRIORITY, m_bUseHighestPriority);
	DDX_Check(pDX, IDC_SYNCTIMEESTANDDATES, m_bSyncTimeEstAndDates);
	DDX_Check(pDX, IDC_INCLUDEDONEINPRIORITYCALC, m_bIncludeDoneInPriorityRiskCalc);
	DDX_Check(pDX, IDC_AUTOCALCPERCENTDONE, m_bAutoCalcPercentDone);
	DDX_Check(pDX, IDC_AUTOUPDATEDEPENDEES, m_bAutoAdjustDependents);
	DDX_Check(pDX, IDC_WEIGHTPERCENTCALCBYNUMSUB, m_bWeightPercentCompletionByNumSubtasks);
	DDX_Check(pDX, IDC_DUEHAVEHIGHESTPRIORITY, m_bDueTasksHaveHighestPriority);
	DDX_Check(pDX, IDC_DONEHAVELOWESTPRIORITY, m_bDoneTasksHaveLowestPriority);
	DDX_Check(pDX, IDC_NODUEDATEDUETODAY, m_bNoDueDateDueTodayOrStart);
	DDX_Check(pDX, IDC_SETSTATUSONDONE, m_bSetCompletionStatus);
	DDX_Check(pDX, IDC_SYNCCOMPLETIONTOSTATUS, m_bSyncCompletionToStatus);
	DDX_Text(pDX, IDC_DONESTATUS, m_sCompletionStatus);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_REMAININGTIMEISDUEDATE, (int&)m_nCalcRemainingTime);
	DDX_Radio(pDX, IDC_USEEARLIESTDUEDATE, (int&)m_nCalcDueDate);
	DDX_Radio(pDX, IDC_USEEARLIESTSTARTDATE, (int&)m_nCalcStartDate);
	DDX_Check(pDX, IDC_USEPERCENTDONEINTIMEEST, m_bUsePercentDoneInTimeEst);
	DDX_Check(pDX, IDC_AVERAGEPERCENTSUBCOMPLETION, m_bAveragePercentSubCompletion);
	DDX_Check(pDX, IDC_INCLUDEDONEINAVERAGECALC, m_bIncludeDoneInAverageCalc);

	if (pDX->m_bSaveAndValidate)
	{
		m_dRecentModTime = m_eRecentModPeriod.GetTime();
		m_nRecentModTimeUnits = m_eRecentModPeriod.GetUnits();
	}
	else
	{
		m_eRecentModPeriod.SetTime(m_dRecentModTime, m_nRecentModTimeUnits);
	}
}


BEGIN_MESSAGE_MAP(CPreferencesTaskCalcPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesTaskCalcPage)
	ON_BN_CLICKED(IDC_USEHIGHESTPRIORITY, OnUsehighestpriority)
	ON_BN_CLICKED(IDC_AUTOCALCPERCENTDONE, OnAutocalcpercentdone)
	ON_BN_CLICKED(IDC_SETSTATUSONDONE, OnSetStatusOnDone)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_AVERAGEPERCENTSUBCOMPLETION, OnAveragepercentChange)
	ON_EN_CHANGE(IDC_DONESTATUS, OnChangeCompletionStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskCalcPage message handlers

BOOL CPreferencesTaskCalcPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	GetDlgItem(IDC_INCLUDEDONEINAVERAGECALC)->EnableWindow(m_bAveragePercentSubCompletion);
	GetDlgItem(IDC_WEIGHTPERCENTCALCBYNUMSUB)->EnableWindow(m_bAveragePercentSubCompletion);
	GetDlgItem(IDC_INCLUDEDONEINPRIORITYCALC)->EnableWindow(m_bUseHighestPriority);

	OnChangeCompletionStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesTaskCalcPage::OnAveragepercentChange() 
{
	UpdateData();

	GetDlgItem(IDC_INCLUDEDONEINAVERAGECALC)->EnableWindow(m_bAveragePercentSubCompletion);
	GetDlgItem(IDC_WEIGHTPERCENTCALCBYNUMSUB)->EnableWindow(m_bAveragePercentSubCompletion);

	// uncheck IDC_AUTOCALCPERCENTDONE if m_bAveragePercentSubCompletion is TRUE
	if (m_bAveragePercentSubCompletion && m_bAutoCalcPercentDone)
	{
		m_bAutoCalcPercentDone = FALSE;
		UpdateData(FALSE);
	}

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesTaskCalcPage::OnUsehighestpriority() 
{
	UpdateData();

	GetDlgItem(IDC_INCLUDEDONEINPRIORITYCALC)->EnableWindow(m_bUseHighestPriority);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesTaskCalcPage::OnAutocalcpercentdone() 
{
	UpdateData();

	// uncheck IDC_AUTOCALCPERCENTDONE if m_bAveragePercentSubCompletion is TRUE
	if (m_bAutoCalcPercentDone && m_bAveragePercentSubCompletion)
	{
		m_bAveragePercentSubCompletion = FALSE;
		UpdateData(FALSE);

		OnAveragepercentChange();
	}

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesTaskCalcPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	// load settings
	m_bTreatSubCompletedAsDone = pPrefs->GetProfileInt(szKey, _T("TreatSubCompletedAsDone"), TRUE);
	m_bAveragePercentSubCompletion = pPrefs->GetProfileInt(szKey, _T("AveragePercentSubCompletion"), TRUE);
	m_bIncludeDoneInAverageCalc = pPrefs->GetProfileInt(szKey, _T("IncludeDoneInAverageCalc"), TRUE);
	m_bUsePercentDoneInTimeEst = pPrefs->GetProfileInt(szKey, _T("UsePercentDoneInTimeEst"), TRUE);
	m_bUseHighestPriority = pPrefs->GetProfileInt(szKey, _T("UseHighestPriority"), FALSE);
	m_bSyncTimeEstAndDates = pPrefs->GetProfileInt(szKey, _T("SyncTimeEstAndDates"), FALSE);
	m_bIncludeDoneInPriorityRiskCalc = pPrefs->GetProfileInt(szKey, _T("IncludeDoneInPriorityCalc"), FALSE);
	m_bAutoCalcPercentDone = pPrefs->GetProfileInt(szKey, _T("AutoCalcPercentDone"), FALSE);
	m_bAutoAdjustDependents = pPrefs->GetProfileInt(szKey, _T("AutoAdjustDependents"), TRUE);
	m_bDueTasksHaveHighestPriority = pPrefs->GetProfileInt(szKey, _T("DueTasksHaveHighestPriority"), FALSE);
	m_bDoneTasksHaveLowestPriority = pPrefs->GetProfileInt(szKey, _T("DoneTasksHaveLowestPriority"), TRUE);
	m_bNoDueDateDueTodayOrStart = pPrefs->GetProfileInt(szKey, _T("NoDueDateIsDueToday"), FALSE);
	m_bWeightPercentCompletionByNumSubtasks = pPrefs->GetProfileInt(szKey, _T("WeightPercentCompletionByNumSubtasks"), TRUE);
	m_nCalcRemainingTime = (PTCP_CALCTIMEREMAINING)pPrefs->GetProfileInt(szKey, _T("CalcRemainingTime"), PTCP_REMAININGTTIMEISDUEDATE);
	m_nCalcDueDate = (PTCP_CALCDUEDATE)pPrefs->GetProfileInt(szKey, _T("CalcDueDate"), PTCP_NOCALCDUEDATE);
	m_nCalcStartDate = (PTCP_CALCSTARTDATE)pPrefs->GetProfileInt(szKey, _T("CalcStartDate"), PTCP_NOCALCSTARTDATE);
	m_bSetCompletionStatus = pPrefs->GetProfileInt(szKey, _T("SetCompletionStatus"), FALSE);
	m_bSyncCompletionToStatus = pPrefs->GetProfileInt(szKey, _T("SyncCompletionToStatus"), TRUE);
	m_sCompletionStatus = pPrefs->GetProfileString(szKey, _T("CompletionStatus"), CEnString(_T("Completed")));

	// backwards compatibility
	if (m_nCalcDueDate == PTCP_NOCALCDUEDATE)
	{
		if (pPrefs->GetProfileInt(szKey, _T("UseEarliestDueDate"), FALSE))
			m_nCalcDueDate = PTCP_EARLIESTDUEDATE;
	}

	// fix up m_bAveragePercentSubCompletion because it's overridden by m_bAutoCalcPercentDone
	if (m_bAutoCalcPercentDone)
		m_bAveragePercentSubCompletion = FALSE;

	m_dRecentModTime = pPrefs->GetProfileDouble(szKey, _T("RecentModTime"), 1.0);
	m_nRecentModTimeUnits = (TH_UNITS)pPrefs->GetProfileInt(szKey, _T("RecentModTimeUnits"), THU_HOURS);

//	m_b = pPrefs->GetProfileInt(szKey, _T(""), FALSE);
}

void CPreferencesTaskCalcPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileInt(szKey, _T("TreatSubCompletedAsDone"), m_bTreatSubCompletedAsDone);
	pPrefs->WriteProfileInt(szKey, _T("AveragePercentSubCompletion"), m_bAveragePercentSubCompletion);
	pPrefs->WriteProfileInt(szKey, _T("IncludeDoneInAverageCalc"), m_bIncludeDoneInAverageCalc);
	pPrefs->WriteProfileInt(szKey, _T("UsePercentDoneInTimeEst"), m_bUsePercentDoneInTimeEst);
	pPrefs->WriteProfileInt(szKey, _T("UseHighestPriority"), m_bUseHighestPriority);
	pPrefs->WriteProfileInt(szKey, _T("SyncTimeEstAndDates"), m_bSyncTimeEstAndDates);
	pPrefs->WriteProfileInt(szKey, _T("IncludeDoneInPriorityCalc"), m_bIncludeDoneInPriorityRiskCalc);
	pPrefs->WriteProfileInt(szKey, _T("WeightPercentCompletionByNumSubtasks"), m_bWeightPercentCompletionByNumSubtasks);
	pPrefs->WriteProfileInt(szKey, _T("AutoCalcPercentDone"), m_bAutoCalcPercentDone);
	pPrefs->WriteProfileInt(szKey, _T("AutoAdjustDependents"), m_bAutoAdjustDependents);
	pPrefs->WriteProfileInt(szKey, _T("DueTasksHaveHighestPriority"), m_bDueTasksHaveHighestPriority);
	pPrefs->WriteProfileInt(szKey, _T("DoneTasksHaveLowestPriority"), m_bDoneTasksHaveLowestPriority);
	pPrefs->WriteProfileInt(szKey, _T("NoDueDateIsDueToday"), m_bNoDueDateDueTodayOrStart);
	pPrefs->WriteProfileInt(szKey, _T("CalcRemainingTime"), m_nCalcRemainingTime);
	pPrefs->WriteProfileInt(szKey, _T("CalcDueDate"), (int)m_nCalcDueDate);
	pPrefs->WriteProfileInt(szKey, _T("CalcStartDate"), (int)m_nCalcStartDate);
	pPrefs->WriteProfileInt(szKey, _T("SetCompletionStatus"), m_bSetCompletionStatus);
	pPrefs->WriteProfileInt(szKey, _T("SyncCompletionToStatus"), m_bSyncCompletionToStatus);

	pPrefs->WriteProfileDouble(szKey, _T("RecentModTime"), m_dRecentModTime);
	pPrefs->WriteProfileInt(szKey, _T("RecentModTimeUnits"), m_nRecentModTimeUnits);

	pPrefs->WriteProfileString(szKey, _T("CompletionStatus"), m_sCompletionStatus);

	// cleanup old entry	
	pPrefs->DeleteProfileEntry(szKey, _T("UseEarliestDueDate"));

//	pPrefs->WriteProfileInt(szKey, _T(""), m_b);
}

COleDateTimeSpan CPreferencesTaskCalcPage::GetRecentlyModifiedPeriod() const
{
	return CTimeHelper(24, 7).GetTime(m_dRecentModTime, m_nRecentModTimeUnits, THU_DAYS);
}

void CPreferencesTaskCalcPage::OnSetStatusOnDone() 
{
	UpdateData();
	GetDlgItem(IDC_DONESTATUS)->EnableWindow(m_bSetCompletionStatus);

	OnChangeCompletionStatus();
}

BOOL CPreferencesTaskCalcPage::GetCompletionStatus(CString& sStatus) const
{
	if (m_bSetCompletionStatus)
		sStatus = m_sCompletionStatus;

	Misc::Trim(sStatus);
	return !sStatus.IsEmpty();
}

void CPreferencesTaskCalcPage::OnChangeCompletionStatus()
{
	UpdateData();

	CString sStatus;
	GetDlgItem(IDC_SYNCCOMPLETIONTOSTATUS)->EnableWindow(GetCompletionStatus(sStatus));
}

BOOL CPreferencesTaskCalcPage::GetSyncCompletionToStatus() const
{
	CString sStatus;
	return (GetCompletionStatus(sStatus) && m_bSyncCompletionToStatus);
}
