#if !defined(AFX_PREFERENCESTASKCALCPAGE_H__84CBF881_D8CA_4D00_ADD6_1DCB7DE71C5B__INCLUDED_)
#define AFX_PREFERENCESTASKCALCPAGE_H__84CBF881_D8CA_4D00_ADD6_1DCB7DE71C5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesTaskPage.h : header file
//

#include "..\shared\timeedit.h"
#include "..\shared\groupline.h"
#include "..\shared\preferencesbase.h"

enum PTCP_CALCTIMEREMAINING
{
	PTCP_REMAININGTTIMEISDUEDATE,
	PTCP_REMAININGTTIMEISSPENT,
	PTCP_REMAININGTTIMEISPERCENTAGE,
};

enum PTCP_CALCDUEDATE
{
	PTCP_EARLIESTDUEDATE,
	PTCP_LATESTDUEDATE,
	PTCP_NOCALCDUEDATE,
};

enum PTCP_CALCSTARTDATE
{
	PTCP_EARLIESTSTARTDATE,
	PTCP_LATESTSTARTDATE,
	PTCP_NOCALCSTARTDATE,
};

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskCalcPage dialog

class CPreferencesTaskCalcPage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesTaskCalcPage)

// Construction
public:
	CPreferencesTaskCalcPage();
	~CPreferencesTaskCalcPage();

	BOOL GetAveragePercentSubCompletion() const { return m_bAveragePercentSubCompletion; }
	BOOL GetIncludeDoneInAverageCalc() const { return m_bIncludeDoneInAverageCalc; }
	BOOL GetUsePercentDoneInTimeEst() const { return m_bUsePercentDoneInTimeEst; }
	BOOL GetTreatSubCompletedAsDone() const { return m_bTreatSubCompletedAsDone; }
	BOOL GetUseHighestPriority() const { return m_bUseHighestPriority; }
	BOOL GetUseHighestRisk() const { return m_bUseHighestPriority; } // Note: this uses same flag as priority
	BOOL GetSyncTimeEstimatesAndDates() const { return m_bSyncTimeEstAndDates; }
	BOOL GetIncludeDoneInPriorityRiskCalc() const { return m_bIncludeDoneInPriorityRiskCalc; }
	BOOL GetWeightPercentCompletionByNumSubtasks() const { return m_bWeightPercentCompletionByNumSubtasks; }
	BOOL GetAutoCalcPercentDone() const { return m_bAutoCalcPercentDone; }
	BOOL GetAutoAdjustDependentsDates() const { return m_bAutoAdjustDependents; }
	BOOL GetDueTasksHaveHighestPriority() const { return m_bDueTasksHaveHighestPriority; }
	BOOL GetDoneTasksHaveLowestPriority() const { return m_bDoneTasksHaveLowestPriority; }
	BOOL GetDoneTasksHaveLowestRisk() const { return m_bDoneTasksHaveLowestPriority; } // Note: this uses same flag as priority
	BOOL GetNoDueDateIsDueTodayOrStart() const { return m_bNoDueDateDueTodayOrStart; }
	BOOL GetCompletionStatus(CString& sStatus) const;
	BOOL GetSyncCompletionToStatus() const;

	COleDateTimeSpan GetRecentlyModifiedPeriod() const;

	PTCP_CALCTIMEREMAINING GetTimeRemainingCalculation() const { return m_nCalcRemainingTime; }
	PTCP_CALCDUEDATE GetDueDateCalculation() const { return m_nCalcDueDate; }
	PTCP_CALCSTARTDATE GetStartDateCalculation() const { return m_nCalcStartDate; }


//	BOOL Get() const { return m_b; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesTaskCalcPage)
	enum { IDD = IDD_PREFTASKCALC_PAGE };
	BOOL	m_bTreatSubCompletedAsDone;
	BOOL	m_bUseHighestPriority;
	BOOL	m_bSyncTimeEstAndDates;
	BOOL	m_bIncludeDoneInPriorityRiskCalc;
	BOOL	m_bAutoCalcPercentDone;
	BOOL	m_bAutoAdjustDependents;
	BOOL	m_bWeightPercentCompletionByNumSubtasks;
	BOOL	m_bDueTasksHaveHighestPriority;
	BOOL	m_bDoneTasksHaveLowestPriority;
	BOOL	m_bNoDueDateDueTodayOrStart;
	BOOL	m_bSetCompletionStatus;
	BOOL	m_bSyncCompletionToStatus;
	CString	m_sCompletionStatus;
	//}}AFX_DATA
	CTimeEdit	m_eRecentModPeriod;
	double		m_dRecentModTime;
	TH_UNITS	m_nRecentModTimeUnits;

	PTCP_CALCTIMEREMAINING	m_nCalcRemainingTime;
	PTCP_CALCDUEDATE		m_nCalcDueDate;
	PTCP_CALCSTARTDATE		m_nCalcStartDate;

	BOOL	m_bUsePercentDoneInTimeEst;
	BOOL	m_bAveragePercentSubCompletion;
	BOOL	m_bIncludeDoneInAverageCalc;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesTaskCalcPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesTaskCalcPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnUsehighestpriority();
	afx_msg void OnAutocalcpercentdone();
	afx_msg void OnSetStatusOnDone();
	//}}AFX_MSG
	afx_msg void OnAveragepercentChange();
	afx_msg void OnChangeCompletionStatus();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESTASKCALCPAGE_H__84CBF881_D8CA_4D00_ADD6_1DCB7DE71C5B__INCLUDED_)
