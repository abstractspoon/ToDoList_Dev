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

class CToDoCtrlReminders : protected CTDLShowReminderDlg
{
// Construction
public:
	CToDoCtrlReminders();

	BOOL Initialize(CWnd* pNotify);
	BOOL UseStickies(BOOL bEnable, LPCTSTR szStickiesPath, BOOL bShowFullTaskPath);

	void ShowWindow() { CTDLShowReminderDlg::ShowWindow(IsIconic() ? SW_RESTORE : SW_SHOW); }
	BOOL DestroyWindow() { return CTDLShowReminderDlg::DestroyWindow(); }
	BOOL IsForegroundWindow() const { return (::GetForegroundWindow() == GetSafeHwnd()); }

	void AddToDoCtrl(const CFilteredToDoCtrl* pTDC);
	void RemoveToDoCtrl(const CFilteredToDoCtrl* pTDC);
	void SetReminder(const TDCREMINDER& rem, BOOL bCheckNow = FALSE);
	BOOL ClearReminder(DWORD dwTaskID, const CFilteredToDoCtrl* pTDC);
	BOOL TransferReminder(DWORD dwTaskID, DWORD dwNewTaskID, const CFilteredToDoCtrl* pTDC);
	BOOL GetReminder(int nRem, TDCREMINDER& rem) const;
	int FindReminder(const TDCREMINDER& rem, BOOL bIncludeDisabled = TRUE) const;
	int FindReminder(DWORD dwTaskID, const CFilteredToDoCtrl* pTDC, BOOL bIncludeDisabled = TRUE) const;
	BOOL ToDoCtrlHasReminders(const CFilteredToDoCtrl* pTDC);
	BOOL ToDoCtrlHasReminders(const CString& sFilePath);
	BOOL UpdateModifiedTasks(const CFilteredToDoCtrl* pTDC, const CDWordArray& aTaskIDs, const CTDCAttributeMap& mapAttrib);

	BOOL GetReminderDate(int nRem, COleDateTime& dtRem) const;
	BOOL IsEmpty() const { return (GetListReminderCount() == 0); }

	void CheckReminders();

// Attributes
protected:
	CWnd* m_pWndNotify;
	CArray<TDCREMINDER, TDCREMINDER&> m_aReminders;
	BOOL m_bUseStickies, m_bShowFullTaskPathInSticky;
	CString m_sStickiesPath;
	CStickiesWnd m_stickies;
	CRichEditBaseCtrl m_rtfFormatter; // For 'Stickies'

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
	virtual void DoGotoTask(const TDCREMINDER& rem);
	virtual void HideWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToDoCtrlReminders)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	void SaveAndRemoveReminders(const CFilteredToDoCtrl* pTDC);
	void LoadReminders(const CFilteredToDoCtrl* pTDC);
	void StartTimer();
	BOOL ShowReminder(const TDCREMINDER& rem);
	BOOL DeleteReminder(int nRem);
	BOOL DismissReminder(int nRem);
	void NotifyReminder(const TDCREMINDER& rem, UINT nMsg);
	void ActivateNotificationWindow();
	void DoCheckReminders();

	enum 
	{
		TCR_REMOVEDELETED	= 0x01,
		TCR_REMOVEDONE		= 0x02,
	};

	BOOL InitialiseRTFFormatter();
	BOOL BuildRTFContent(const TDCREMINDER& rem, CString& sContent);
	int RemoveDeletedTasks(const CFilteredToDoCtrl* pTDC = NULL);
	int RemoveCompletedTasks(const CFilteredToDoCtrl* pTDC = NULL);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOCTRLREMINDERS_H__E0CF538D_61A2_418B_8702_85A1BFFE05BF__INCLUDED_)
