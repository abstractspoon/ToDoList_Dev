#if !defined(AFX_CALENDARWND_H__47616F96_0B5B_4F86_97A2_93B9DC796EB4__INCLUDED_)
#define AFX_CALENDARWND_H__47616F96_0B5B_4F86_97A2_93B9DC796EB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalendarWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "CalendarExt.h"
#include "TaskCalendarCtrl.h"
#include "TaskMiniCalendarCtrl.h"
#include "calendarpreferencesdlg.h"

#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"

#include "..\3rdParty\StatLink.h"

#include "..\Interfaces\uitheme.h"
#include "..\Interfaces\IUIExtension.h"

/////////////////////////////////////////////////////////////////////////////
// CCalendarWnd window

class CCalendarWnd : public CDialog, public IUIExtensionWindow
{
	DECLARE_DYNAMIC(CCalendarWnd)

public:
	CCalendarWnd();
	virtual ~CCalendarWnd();

	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);

	// IUIExtensionWindow implementation
	LPCTSTR GetTypeID() const { return CAL_TYPEID; }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetMenuText() const;

	void SetReadOnly(bool bReadOnly);
	HWND GetHwnd() const { return GetSafeHwnd(); }
	
	void SetUITheme(const UITHEME* pTheme);
	void SetTaskFont(HFONT hFont);

	bool GetLabelEditRect(LPRECT pEdit);
	IUI_HITTEST HitTest(POINT ptScreen, IUI_HITTESTREASON nReason) const;
	DWORD HitTestTask(POINT ptScreen, IUI_HITTESTREASON nReason) const;

	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly);
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate);
	bool WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const;
	bool PrepareNewTask(ITaskList* pTask) const;

	bool SelectTask(DWORD dwTaskID, bool bTaskLink);
	bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount);

	bool ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);
	bool DoIdleProcessing() { return false; }

	bool DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData);
	bool CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const;

protected:
	CTaskCalendarCtrl m_BigCalendar;
	CTaskMiniCalendarCtrl m_MiniCalendar;
	CComboBox m_cbNumWeeks;
	CCalendarPreferencesDlg m_dlgPrefs;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CComboBox m_cbSnapModes;
	CStaticLink m_stSelectedTaskDates;

	CBrush m_brBack;
	UITHEME m_theme;
	CIcon m_icon;
	BOOL m_bReadOnly;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK() {}

protected:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNcDestroy();

	afx_msg void OnGototoday();
	afx_msg void OnPreferences();
	afx_msg void OnSelChangeNumWeeks();
	afx_msg void OnSelChangeSnapMode();
	afx_msg void OnHelp();
	afx_msg void OnClickSelectedTaskDates();

	afx_msg void OnBigCalendarNotifyClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBigCalendarNotifyDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMiniCalendarNotifyClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMiniCalendarNotifyDblClk(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT OnBigCalendarNotifyDateChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBigCalendarNotifySelectionChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBigCalendarNotifyDragChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBigCalendarNotifyVisibleWeekChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBigCalendarPrefsHelp(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBigCalendarGetTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBigCalendarEditTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBigCalendarGetTaskFutureDates(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnRecalcDatesLabel(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	void ResizeControls(int cx = 0, int cy = 0);
	void UpdateSelectedTaskDates();
	void InitSnapCombo();
	void UpdateCalendarCtrlPreferences();
	void SyncMiniCalendar(BOOL bScroll);
	void SyncBigCalendar(BOOL bScroll);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_CALENDARFRAMEWND_H__47616F96_0B5B_4F86_97A2_93B9DC796EB4__INCLUDED_)
