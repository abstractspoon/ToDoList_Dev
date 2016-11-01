#if !defined(AFX_GANTTCHARTWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
#define AFX_GANTTCHARTWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GanttChartWnd.h : header file
//

#include "GanttTreeCtrl.h"
#include "GanttTreeListCtrl.h"
#include "GanttPreferencesDlg.h"
#include "GanttCreateDependsDlg.h"

#include "..\Shared\tabbedcombobox.h"
#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"

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
	void Release();

	LPCTSTR GetMenuText() const { return _T("Gantt Chart"); }
	HICON GetIcon() const { return m_hIcon; }
	LPCTSTR GetTypeID() const { return GANTT_TYPEID; }

	void SetReadOnly(bool bReadOnly);
	HWND GetHwnd() const { return GetSafeHwnd(); }
	void SetUITheme(const UITHEME* pTheme);

	bool GetLabelEditRect(LPRECT pEdit);
	IUI_HITTEST HitTest(const POINT& ptScreen) const;

	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly);
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes);
	bool WantEditUpdate(IUI_ATTRIBUTE nAttribute) const;
	bool WantSortUpdate(IUI_ATTRIBUTE nAttribute) const;
	bool PrepareNewTask(ITaskList* pTask) const;

	bool SelectTask(DWORD dwTaskID);
	bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount);

	bool ProcessMessage(MSG* pMsg);

	bool DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra);
	bool CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const;

protected:
// Dialog Data
	//{{AFX_DATA(CGanttChartWnd)
	//}}AFX_DATA
	CComboBox m_cbSnapModes;
	CListCtrl m_list;
	CGanttTreeCtrl m_tree;
	CTabbedComboBox	m_cbDisplayOptions;
	CGanttTreeListCtrl m_ctrlGantt;
	CGanttPreferencesDlg m_dlgPrefs;
	CGanttCreateDependsDlg m_dlgDepends;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;

	HICON m_hIcon;
	CBrush m_brBack;
	UITHEME m_theme;
	CString m_sSelectedTaskDates;
	CMap<GTLC_MONTH_DISPLAY, GTLC_MONTH_DISPLAY, GTLC_SNAPMODE, GTLC_SNAPMODE> m_mapDisplaySnapModes;
	BOOL m_bReadOnly;
	BOOL m_bInSelectTask;

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
	afx_msg void OnSelchangedGanttTree(NMHDR* pNMHDR, LRESULT* pResult);
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
	afx_msg void OnBeginEditTreeLabel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelchangeSnapMode();
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);

	afx_msg LRESULT OnGanttNotifyDateChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttNotifyDragChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttNotifySortChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttNotifyZoomChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttDependencyDlgClose(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGanttPrefsHelp(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	// pseudo handlers
	void OnGanttDepends(GCDD_MODE nMode);
	void OnUpdateGanttDepends(GCDD_MODE nMode, CCmdUI* pCmdUI);

protected:
	void Resize(int cx, int cy);
	void EnableSplitting(BOOL bEnable = TRUE);
	void UpdateGanttCtrlPreferences();
	void SendParentSelectionUpdate();
	void UpdateSelectedTaskDates();
	void BuildSnapCombo();
	void BuildDisplayCombo();
	BOOL SetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay);
	void LoadSnapModePreference(const IPreferences* pPrefs, LPCTSTR szSnapKey, GTLC_MONTH_DISPLAY nDisplay, LPCTSTR szDisplay, GTLC_SNAPMODE nDefaultSnap);
	void SaveSnapModePreference(IPreferences* pPrefs, LPCTSTR szSnapKey, GTLC_MONTH_DISPLAY nDisplay, LPCTSTR szDisplay) const;
	void SaveColumnState(IPreferences* pPrefs, LPCTSTR szKey, const CIntArray& aStates) const;
	int LoadColumnState(const IPreferences* pPrefs, LPCTSTR szKey, CIntArray& aStates) const;

	static DWORD MapColumn(GTLC_COLUMN nColumn);
	static GTLC_COLUMN MapColumn(DWORD dwColumn);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GANTTCHARTWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
