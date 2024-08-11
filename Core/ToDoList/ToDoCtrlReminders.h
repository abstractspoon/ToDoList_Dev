#if !defined(AFX_TODOCTRLREMINDERS_H__E0CF538D_61A2_418B_8702_85A1BFFE05BF__INCLUDED_)
#define AFX_TODOCTRLREMINDERS_H__E0CF538D_61A2_418B_8702_85A1BFFE05BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToDoCtrlReminders.h : header file
//

#include "tdcenum.h"
#include "tdlshowreminderdlg.h"

#include "..\shared\StickiesWnd.h"
#include "..\shared\RichEditBaseCtrl.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class CFilteredToDoCtrl;
class CTDCAttributeMap;

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrlReminders window

class CToDoCtrlReminders : public CTDLShowReminderDlg
{
// Construction
public:
	CToDoCtrlReminders();

	BOOL Initialize(CWnd* pNotify);
	BOOL UseStickies(BOOL bEnable, LPCTSTR szStickiesPath, BOOL bShowFullTaskPath, BOOL bAutoStart);
	void EnableReducedFlashing(BOOL bEnable) { m_bReduceFlashing = bEnable; }

	int AddToDoCtrl(const CFilteredToDoCtrl* pTDC);
	void RemoveToDoCtrl(const CFilteredToDoCtrl* pTDC);
	BOOL SetReminder(const TDCREMINDER& rem, BOOL bCheckNow = FALSE);
	BOOL ClearReminder(DWORD dwTaskID, const CFilteredToDoCtrl* pTDC);
	BOOL TransferReminder(DWORD dwTaskID, DWORD dwNewTaskID, const CFilteredToDoCtrl* pTDC);
	BOOL GetReminder(int nRem, TDCREMINDER& rem) const;
	int FindReminder(const TDCREMINDER& rem, BOOL bIncludeDisabled = TRUE) const;
	int FindReminder(DWORD dwTaskID, const CFilteredToDoCtrl* pTDC, BOOL bIncludeDisabled = TRUE) const;
	BOOL ToDoCtrlHasReminders(const CFilteredToDoCtrl* pTDC);
	BOOL ToDoCtrlHasReminders(const CString& sFilePath);
	BOOL UpdateModifiedTasks(const CFilteredToDoCtrl* pTDC, const CDWordArray& aTaskIDs, const CTDCAttributeMap& mapAttrib);
	BOOL GetReminderDate(int nRem, COleDateTime& dtRem) const;
	void CheckReminders();
	int OffsetReminder(DWORD dwTaskID, double dAmount, TDC_UNITS nUnits, const CFilteredToDoCtrl* pTDC, BOOL bAndSubtasks, BOOL bFromToday);
	BOOL GetFirstTaskReminder(const CFilteredToDoCtrl* pTDC, const CDWordArray& aTaskIDs, TDCREMINDER& rem) const;
	BOOL UpdateRecurringTaskReminders(DWORD dwOldTaskID, DWORD dwNewTaskID, const CFilteredToDoCtrl* pTDC);

// Attributes
protected:
	CWnd* m_pWndNotify;
	CArray<TDCREMINDER, TDCREMINDER&> m_aReminders;
	BOOL m_bReduceFlashing;
	BOOL m_bUseStickies, m_bShowFullTaskPathInSticky;
	CString m_sStickiesPath;
	CStickiesWnd m_stickies;
	CRichEditBaseCtrl m_rtfFormatter; // For 'Stickies'

// Implementation
public:
	virtual ~CToDoCtrlReminders();

	virtual void DoSnoozeReminder(const TDCREMINDER& rem);
	virtual void DoModifyReminder(const TDCREMINDER& rem);
	virtual void DoDismissReminder(const TDCREMINDER& rem);
	virtual void DoGotoTask(const TDCREMINDER& rem);
	virtual void DoCompleteTask(const TDCREMINDER& rem);
	virtual void HideWindow();
	virtual BOOL CanModifyReminders() const;

	// Generated message map functions
protected:
	//{{AFX_MSG(CToDoCtrlReminders)
	//}}AFX_MSG
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	void SaveAndRemoveReminders(const CFilteredToDoCtrl* pTDC);
	void NotifyReminder(const TDCREMINDER& rem, UINT nMsg);
	void ActivateNotificationWindow();
	void DoCheckReminders();
	void StartTimer();
	int LoadReminders(const CFilteredToDoCtrl* pTDC);
	int RemoveDeletedTasks(const CFilteredToDoCtrl* pTDC = NULL);
	int RemoveCompletedTasks(const CFilteredToDoCtrl* pTDC = NULL);
	BOOL ShowReminder(const TDCREMINDER& rem);
	BOOL DeleteReminder(int nRem);
	BOOL DismissReminder(int nRem);
	BOOL InitialiseRTFFormatter();
	BOOL BuildStickiesRTFContent(const TDCREMINDER& rem, CString& sContent);
	BOOL NonRecurringReminderHasRecurringParent(const TDCREMINDER& rem, DWORD dwParentID, const CFilteredToDoCtrl* pTDC) const;
	BOOL IsRecurringReminder(const TDCREMINDER& rem, BOOL bIncludeParent = TRUE) const;

	static BOOL OffsetReminder(TDCREMINDER& rem, double dAmount, TDC_UNITS nUnits, BOOL bFromToday);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOCTRLREMINDERS_H__E0CF538D_61A2_418B_8702_85A1BFFE05BF__INCLUDED_)
