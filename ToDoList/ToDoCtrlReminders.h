#if !defined(AFX_TODOCTRLREMINDERS_H__E0CF538D_61A2_418B_8702_85A1BFFE05BF__INCLUDED_)
#define AFX_TODOCTRLREMINDERS_H__E0CF538D_61A2_418B_8702_85A1BFFE05BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToDoCtrlReminders.h : header file
//

#include <afxtempl.h>
#include "tdcenum.h"
#include "tdlshowreminderdlg.h"

#include "..\shared\StickiesWnd.h"

/////////////////////////////////////////////////////////////////////////////

class CFilteredToDoCtrl;

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrlReminders window

class CToDoCtrlReminders : protected CTDLShowReminderDlg
{
// Construction
public:
	CToDoCtrlReminders();

	BOOL Initialize(CWnd* pNotify);
	BOOL UseStickies(BOOL bEnable, LPCTSTR szStickiesPath = NULL);

	BOOL DestroyWindow() { return CTDLShowReminderDlg::DestroyWindow(); }
	BOOL IsForegroundWindow() const { return (::GetForegroundWindow() == GetSafeHwnd()); }

	void AddToDoCtrl(const CFilteredToDoCtrl* pTDC);
	void CloseToDoCtrl(const CFilteredToDoCtrl* pTDC);
	void SetReminder(const TDCREMINDER& rem, BOOL bCheckNow = FALSE);
	BOOL ClearReminder(DWORD dwTaskID, const CFilteredToDoCtrl* pTDC);
	BOOL TransferReminder(DWORD dwTaskID, DWORD dwNewTaskID, const CFilteredToDoCtrl* pTDC);
	BOOL GetReminder(int nRem, TDCREMINDER& rem) const;
	int FindReminder(const TDCREMINDER& rem, BOOL bIncludeDisabled = TRUE) const;
	int FindReminder(DWORD dwTaskID, const CFilteredToDoCtrl* pTDC, BOOL bIncludeDisabled = TRUE) const;
	BOOL ToDoCtrlHasReminders(const CFilteredToDoCtrl* pTDC);
	BOOL ToDoCtrlHasReminders(const CString& sFilePath);
	BOOL RemoveDeletedTasks(const CFilteredToDoCtrl* pTDC = NULL);
	BOOL RemoveCompletedTasks(const CFilteredToDoCtrl* pTDC = NULL);
	BOOL GetReminderDate(int nRem, COleDateTime& dtRem) const;

	static CString FormatWhenString(const TDCREMINDER& rem);

// Attributes
protected:
	CWnd* m_pWndNotify;
	CArray<TDCREMINDER, TDCREMINDER&> m_aReminders;
	BOOL m_bUseStickies;
	CString m_sStickiesPath;
	CStickiesWnd m_stickies;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToDoCtrlReminders)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToDoCtrlReminders();

	virtual void DoSnoozeReminder(const TDCREMINDER& rem);
	virtual void DoDismissReminder(const TDCREMINDER& rem, BOOL bGotoTask);

	// Generated message map functions
protected:
	//{{AFX_MSG(CToDoCtrlReminders)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void SaveAndRemoveReminders(const CFilteredToDoCtrl* pTDC);
	void LoadReminders(const CFilteredToDoCtrl* pTDC);
	void StartTimer();
	BOOL ShowReminder(const TDCREMINDER& rem);
	BOOL DeleteReminder(int nRem);
	BOOL DismissReminder(int nRem);
	void NotifyReminder(const TDCREMINDER& rem, UINT nMsg);

	enum 
	{
		TCR_REMOVEDELETED	= 0x01,
		TCR_REMOVEDONE		= 0x02,
	};

	BOOL RemoveTasks(DWORD dwToRemove, const CFilteredToDoCtrl* pTDC = NULL);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOCTRLREMINDERS_H__E0CF538D_61A2_418B_8702_85A1BFFE05BF__INCLUDED_)
