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
#include "..\Shared\Icon.h"
#include "..\Shared\EnListCtrl.h"

/////////////////////////////////////////////////////////////////////////////

class CFilteredToDoCtrl;

/////////////////////////////////////////////////////////////////////////////
// CTDLShowReminderListCtrl

class CTDLShowReminderListCtrl : public CEnListCtrl
{
public:
	CTDLShowReminderListCtrl();

	BOOL AddReminder(const TDCREMINDER& rem);
	BOOL UpdateReminder(const TDCREMINDER& rem);
	BOOL RemoveReminder(const TDCREMINDER& rem);
	int RemoveReminders(const CFilteredToDoCtrl& tdc);

	int GetReminders(const CFilteredToDoCtrl& tdc, CTDCReminderArray& aRem) const;
	int GetReminders(CTDCReminderArray& aRem) const;

	DWORD GetReminderID(const TDCREMINDER& rem) const;
	DWORD GetReminderID(int nItem) const { return GetItemData(nItem); }

	int FindReminder(const TDCREMINDER& rem) const;
	int GetSelectedReminder(TDCREMINDER& rem) const;
	int GetSelectedReminders(CTDCReminderArray& aRem) const;

	void DeleteAllItems();

protected:
	BOOL m_bTasksHaveIcons;
	DWORD m_dwNextReminderID;
	CTDCReminderMap m_mapReminders;

protected:
	virtual int CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn) const;
	virtual COLORREF GetItemTextColor(int nItem, int nSubItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemBackColor(int nItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
	virtual void DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused);

protected:
	void UpdateTasksHaveIcons();
};

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

	BOOL AddListReminder(const TDCREMINDER& rem);
	BOOL UpdateListReminder(const TDCREMINDER& rem);
	BOOL RemoveListReminder(const TDCREMINDER& rem);
	void RemoveAllListReminders();
	void RemoveListReminders(const CFilteredToDoCtrl& tdc);

	int GetListReminders(const CFilteredToDoCtrl& tdc, CTDCReminderArray& aRem) const;

	BOOL GetWantSnoozeUntil() const { return m_bSnoozeUntil; }
	UINT GetSnoozeMinutes() const { return m_nSnoozeMins; }
	double GetSnoozeDays() const;
	COleDateTime GetSnoozeUntil() const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLShowReminderDlg)
	enum { IDD = IDD_SHOWREMINDER_DIALOG };
	//}}AFX_DATA
	CString m_sSoundFile;
	UINT m_nSnoozeMins;
	int m_bSnoozeUntil;
	BOOL m_bChangingReminders;

	CTDLReminderPeriodComboBox m_cbSnoozeFor;
	CDateTimeCtrlEx m_dtcSnoozeDate;
	CTimeComboBox m_cbSnoozeTime;
	COleDateTime m_dtSnoozeUntil;
	CTDLShowReminderListCtrl m_lcReminders;
	CIcon m_icon;

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
	virtual void DoCompleteTask(const TDCREMINDER& /*rem*/) { ASSERT(0); }
	virtual void HideWindow();
	virtual void OnRepositionControls(int dx, int dy);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLShowReminderDlg)
	afx_msg void OnSnooze();
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
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

protected:
	void EnableControls();
	void UpdateControls();
	void UpdateTitleText();
	void UpdateColumnWidths();
	void SnoozeReminders(BOOL bAll);
	void RestoreFocusToList(int nPrevSel);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLSHOWREMINDERDLG_H__6B20922F_A2AA_4C95_B9E6_45F5EBEF18BF__INCLUDED_)
