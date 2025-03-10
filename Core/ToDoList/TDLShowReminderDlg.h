#if !defined(AFX_TDLSHOWREMINDERDLG_H__6B20922F_A2AA_4C95_B9E6_45F5EBEF18BF__INCLUDED_)
#define AFX_TDLSHOWREMINDERDLG_H__6B20922F_A2AA_4C95_B9E6_45F5EBEF18BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLShowReminderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "tdldialog.h"
#include "tdcreminder.h"
#include "TDLReminderPeriodComboBox.h"
#include "TDLShowReminderListCtrl.h"

#include "..\Shared\DateTimeCtrlEx.h"
#include "..\Shared\timecombobox.h"
#include "..\Shared\EnListCtrl.h"

/////////////////////////////////////////////////////////////////////////////

class CFilteredToDoCtrl;

/////////////////////////////////////////////////////////////////////////////
// CTDLShowReminderDlg dialog

class CTDLShowReminderDlg : public CTDLDialog
{
	DECLARE_DYNAMIC(CTDLShowReminderDlg)

// Construction
public:
	CTDLShowReminderDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL Create(CWnd* pParent, BOOL bVisible = TRUE);
	void SetRemindersFont(HFONT hFont);

	void ShowWindow();
	BOOL IsForegroundWindow() const;
	void SetISODateTimeFormat(BOOL bIso);

protected:
// Dialog Data
	//{{AFX_DATA(CTDLShowReminderDlg)
	//}}AFX_DATA
	CString m_sSoundFile;
	UINT m_nSnoozeMins;
	int m_bSnoozeUntil;
	BOOL m_bModifyingList;
	BOOL m_bModifyingReminder;
	BOOL m_bISODateTimes;

	CTDLReminderPeriodComboBox m_cbSnoozeFor;
	CDateTimeCtrlEx m_dtcSnoozeDate;
	CTimeComboBox m_cbSnoozeTime;
	COleDateTime m_dtSnoozeUntil;
	CTDLShowReminderListCtrl m_lcReminders;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLShowReminderDlg)
	//}}AFX_VIRTUAL
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual int OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DoSnoozeReminder(const TDCREMINDER& /*rem*/) { ASSERT(0); }
	virtual void DoDismissReminder(const TDCREMINDER& /*rem*/) { ASSERT(0); }
	virtual void DoGotoTask(const TDCREMINDER& /*rem*/) { ASSERT(0); }
	virtual void DoModifyReminder(const TDCREMINDER& /*rem*/) { ASSERT(0); }
	virtual void DoCompleteTask(const TDCREMINDER& /*rem*/) { ASSERT(0); }
	virtual void HideWindow();
	virtual void OnRepositionControls(int dx, int dy);
	virtual BOOL CanModifyReminders() const;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLShowReminderDlg)
	afx_msg void OnSnooze();
	afx_msg void OnModify();
	afx_msg void OnDismiss();
	afx_msg void OnGotoTask();
	afx_msg void OnCompleteTask();
	afx_msg void OnSnoozeFor();
	afx_msg void OnSnoozeUntil();
	afx_msg void OnItemchangedReminders(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSnoozeAll();
	//}}AFX_MSG
	afx_msg void OnDblClkReminders(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	DECLARE_MESSAGE_MAP()

protected:
	BOOL AddListReminder(const TDCREMINDER& rem);
	BOOL UpdateListReminder(const TDCREMINDER& rem);
	BOOL RemoveListReminder(const TDCREMINDER& rem);
	void RemoveListReminders(const CFilteredToDoCtrl& tdc);

	int GetListReminders(const CFilteredToDoCtrl& tdc, CTDCReminderArray& aRem) const;

	BOOL GetWantSnoozeUntil() const { return m_bSnoozeUntil; }
	UINT GetSnoozeMinutes() const { return m_nSnoozeMins; }
	double GetSnoozeDays() const;
	COleDateTime GetSnoozeUntil() const;

	void EnableDisableControls();
	void EnableDisableModify();
	void UpdateControls();
	void UpdateTitleText();
	void SnoozeReminders(BOOL bAll);
	void RestoreFocusToList(int nPrevSel);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLSHOWREMINDERDLG_H__6B20922F_A2AA_4C95_B9E6_45F5EBEF18BF__INCLUDED_)
