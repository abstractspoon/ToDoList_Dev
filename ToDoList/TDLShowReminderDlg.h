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

#include "..\Shared\DateTimeCtrlEx.h"
#include "..\Shared\timecombobox.h"

/////////////////////////////////////////////////////////////////////////////

class CFilteredToDoCtrl;

/////////////////////////////////////////////////////////////////////////////
// CTDLShowReminderDlg dialog

class CTDLShowReminderDlg : public CTDLDialog
{
// Construction
public:
	CTDLShowReminderDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL Create(CWnd* pParent, BOOL bVisible = TRUE);

	BOOL AddListReminder(const TDCREMINDER& rem);
	void RemoveListReminder(const TDCREMINDER& rem);
	void RemoveListReminders(const CFilteredToDoCtrl& tdc);
	void RemoveAllListReminders();

	BOOL GetWantSnoozeUntil() const { return m_bSnoozeUntil; }
	UINT GetSnoozeMinutes() const { return m_nSnoozeMins; }
	double GetSnoozeDays() const;
	COleDateTime GetSnoozeUntil() const;
	int GetListReminderCount() const;

// Dialog Data
	//{{AFX_DATA(CTDLShowReminderDlg)
	enum { IDD = IDD_SHOWREMINDER_DIALOG };
	//}}AFX_DATA
	CString m_sSoundFile;
	UINT m_nSnoozeMins;
	int		m_bSnoozeUntil;
	DWORD m_dwNextReminderID;
	BOOL m_bChangingReminders;

	CTDLReminderPeriodComboBox m_cbSnoozeFor;
	CDateTimeCtrlEx m_dtcSnoozeDate;
	CTimeComboBox m_cbSnoozeTime;
	COleDateTime	m_dtSnoozeUntil;
	CListCtrl	m_lcReminders;

	CTDCReminderMap m_mapReminders;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLShowReminderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual int OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	virtual void DoSnoozeReminder(const TDCREMINDER& /*rem*/) {}
	virtual void DoDismissReminder(const TDCREMINDER& /*rem*/, BOOL /*bGotoTask*/) {}
	virtual void DoGotoTask(const TDCREMINDER& /*rem*/) {}

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLShowReminderDlg)
	afx_msg void OnSnooze();
	afx_msg void OnDismiss();
	afx_msg void OnDismissAndGototask();
	afx_msg void OnSnoozeFor();
	afx_msg void OnSnoozeUntil();
	afx_msg void OnItemchangedReminders(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnDblClkReminders(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

protected:
	int FindListReminder(const TDCREMINDER& rem);
	int GetSelectedReminder(TDCREMINDER& rem) const;
	int GetSelectedReminders(CTDCReminderArray& aRem) const;
	void EnableControls();
	void UpdateControls();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLSHOWREMINDERDLG_H__6B20922F_A2AA_4C95_B9E6_45F5EBEF18BF__INCLUDED_)
