#if !defined(AFX_GANTTCHARTWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
#define AFX_GANTTCHARTWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GanttChartWnd.h : header file
//

#include "GanttCtrl.h"
#include "GanttPreferencesDlg.h"
#include "GanttCreateDependsDlg.h"
#include "GanttDateRangeSlider.h"
#include "GanttDisplayComboBox.h"
#include "GanttSnapComboBox.h"

#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"
#include "..\shared\Icon.h"
#include "..\shared\EnString.h"

#include "..\Interfaces\uitheme.h"
#include "..\Interfaces\IUIExtension.h"

/////////////////////////////////////////////////////////////////////////////
// CGanttChartWnd window

class CGanttChartWnd : public CDialog, public IUIExtensionWindow
{
// Construction
public:
	CGanttChartWnd(CWnd* pParent = NULL);
	virtual ~CGanttChartWnd();

	// IUIExtensionWindow
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);

	LPCTSTR GetMenuText() const { return _T("Gantt Chart"); }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return GANTT_TYPEID; }

	void SetReadOnly(bool bReadOnly);
	HWND GetHwnd() const { return GetSafeHwnd(); }
	
	void SetUITheme(const UITHEME* pTheme);
	void SetTaskFont(HFONT hFont);

	bool GetLabelEditRect(LPRECT pEdit);
	IUI_HITTEST HitTest(POINT ptScreen) const;
	DWORD HitTestTask(POINT ptScreen, bool bTitleColumnOnly) const;

	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly);
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate);
	bool WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const;
	bool PrepareNewTask(ITaskList* pTask) const;

	bool SelectTask(DWORD dwTaskID, bool bTaskLink);
	bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount);

	bool ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);

	bool DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData);
	bool CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const;

protected:
// Dialog Data
	//{{AFX_DATA(CGanttChartWnd)
	//}}AFX_DATA
	CGanttSnapComboBox m_cbSnapModes;
	CGanttCtrl m_ctrlGantt;
	CGanttDisplayComboBox m_cbDisplayOptions;
	CGanttPreferencesDlg m_dlgPrefs;
	CGanttCreateDependsDlg m_dlgDepends;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CGanttDateRangeSlider m_sliderDateRange;

	CIcon m_icon;
	CBrush m_brBack;
	UITHEME m_theme;
	
	BOOL m_bReadOnly;
	BOOL m_bInSelectTask;
	BOOL m_bInSetMonthDisplay;

	CMap<GTLC_MONTH_DISPLAY, GTLC_MONTH_DISPLAY, GTLC_SNAPMODE, GTLC_SNAPMODE> m_mapDisplaySnapModes;
	GANTTDATERANGE m_dtPrevActiveRange;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGanttChartWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();
	virtual void OnCancel() {}
	virtual void OnOK() {}

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGanttChartWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKeyUpGantt(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDisplay();
	afx_msg void OnClickGanttList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnGanttGotoToday();
	afx_msg void OnUpdateGanttGotoToday(CCmdUI* pCmdUI);
	afx_msg void OnGanttNewDepends();
	afx_msg void OnUpdateGanttNewDepends(CCmdUI* pCmdUI);
	afx_msg void OnGanttPreferences();
	afx_msg void OnUpdateGanttPreferences(CCmdUI* pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGanttEditDepends();
	afx_msg void OnUpdateGanttEditDepends(CCmdUI* pCmdUI);
	afx_msg void OnGanttDeleteDepends();
	afx_msg void OnUpdateGanttDeleteDepends(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelchangeSnapMode();
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnNcDestroy();

	afx_msg LRESULT OnGanttEditTaskTitle(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttNotifySelChanged(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttNotifyDateChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttNotifyDragChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttNotifyCompletionChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttNotifySortChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttNotifyZoomChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttDependencyDlgClose(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttPrefsHelp(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttGetTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttEditTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttMoveTask(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSliderDateRangeChange(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	// pseudo handlers
	void OnGanttDepends(GCDD_MODE nMode);
	void OnUpdateGanttDepends(GCDD_MODE nMode, CCmdUI* pCmdUI);

protected:
	void Resize(int cx, int cy);
	void UpdateGanttCtrlPreferences();
	void SendParentSelectionUpdate();
	void UpdateActiveRangeLabel();
	BOOL SetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay);
	void LoadSnapModePreference(const IPreferences* pPrefs, LPCTSTR szSnapKey, GTLC_MONTH_DISPLAY nDisplay, LPCTSTR szDisplay, GTLC_SNAPMODE nDefaultSnap);
	void SaveSnapModePreference(IPreferences* pPrefs, LPCTSTR szSnapKey, GTLC_MONTH_DISPLAY nDisplay, LPCTSTR szDisplay) const;
	void SaveColumnState(IPreferences* pPrefs, LPCTSTR szKey, const CIntArray& aStates) const;
	int LoadColumnState(const IPreferences* pPrefs, LPCTSTR szKey, CIntArray& aStates) const;

	static void InitWorkingWeek(const IPreferences* pPrefs);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GANTTCHARTWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
