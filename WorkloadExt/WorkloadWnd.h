#if !defined(AFX_WorkloadWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
#define AFX_WorkloadWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkloadWnd.h : header file
//

#include "WorkloadTreeCtrl.h"
#include "WorkloadCtrl.h"
#include "WorkloadPreferencesDlg.h"

#include "..\Shared\tabbedcombobox.h"
#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"
#include "..\shared\Icon.h"

#include "..\Interfaces\uitheme.h"
#include "..\Interfaces\IUIExtension.h"

/////////////////////////////////////////////////////////////////////////////
// CWorkloadWnd window

class CWorkloadWnd : public CDialog, public IUIExtensionWindow
{
// Construction
public:
	CWorkloadWnd(CWnd* pParent = NULL);
	virtual ~CWorkloadWnd();

	// IUIExtensionWindow
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);

	LPCTSTR GetMenuText() const { return _T("Workload Chart"); }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return WORKLOAD_TYPEID; }

	void SetReadOnly(bool bReadOnly);
	HWND GetHwnd() const { return GetSafeHwnd(); }
	void SetUITheme(const UITHEME* pTheme);

	bool GetLabelEditRect(LPRECT pEdit);
	IUI_HITTEST HitTest(const POINT& ptScreen) const;

	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly);
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes);
	bool WantTaskUpdate(IUI_ATTRIBUTE nAttribute) const;
	bool PrepareNewTask(ITaskList* pTask) const;

	bool SelectTask(DWORD dwTaskID);
	bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount);

	bool ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);

	bool DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData);
	bool CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const;

protected:
// Dialog Data
	//{{AFX_DATA(CWorkloadWnd)
	//}}AFX_DATA
	CWorkloadCtrl m_ctrlWorkload;
	CWorkloadPreferencesDlg m_dlgPrefs;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;

	CIcon m_icon;
	CBrush m_brBack;
	UITHEME m_theme;
	CString m_sSelectedTaskDates;
	BOOL m_bReadOnly;
	BOOL m_bInSelectTask;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkloadWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();
	virtual void OnCancel() {}
	virtual void OnOK() {}

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWorkloadWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnWorkloadPreferences();
	afx_msg void OnUpdateWorkloadPreferences(CCmdUI* pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnWorkloadEditAllocations();
	afx_msg void OnUpdateWorkloadEditAllocations(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnNcDestroy();

	afx_msg LRESULT OnWorkloadNotifyAllocationChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnWorkloadNotifyCompletionChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnWorkloadNotifySortChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnWorkloadNotifySelChange(WPARAM wp, LPARAM lp);

	afx_msg LRESULT OnWorkloadEditTaskTitle(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnWorkloadPrefsHelp(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnWorkloadGetTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnWorkloadMoveTask(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

protected:
	void Resize(int cx, int cy);
	void EnableSplitting(BOOL bEnable = TRUE);
	void UpdateWorkloadCtrlPreferences();
	void SendParentSelectionUpdate();
	void UpdateSelectedTaskDates();
	void SaveColumnState(IPreferences* pPrefs, LPCTSTR szKey, const CIntArray& aStates) const;
	int LoadColumnState(const IPreferences* pPrefs, LPCTSTR szKey, CIntArray& aStates) const;

	static DWORD MapColumn(WLC_COLUMN nColumn);
	static WLC_COLUMN MapColumn(DWORD dwColumn);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WorkloadWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
