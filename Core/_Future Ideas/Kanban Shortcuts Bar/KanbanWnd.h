#if !defined(AFX_KANBANWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
#define AFX_KANBANWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanWnd.h : header file
//

#include "KanbanCtrl.h"
#include "KanbanPreferencesDlg.h"
#include "KanbanOptionComboBox.h"

#include "..\Shared\tabbedcombobox.h"
#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"
#include "..\Shared\dialoghelper.h"
#include "..\Shared\checkcombobox.h"

#include "..\Interfaces\uitheme.h"
#include "..\Interfaces\IUIExtension.h"

/////////////////////////////////////////////////////////////////////////////
// CKanbanWnd window

class CKanbanWnd : public CDialog, public IUIExtensionWindow, protected CDialogHelper
{
// Construction
public:
	CKanbanWnd(CWnd* pParent = NULL);
	virtual ~CKanbanWnd();

	// IUIExtensionWindow
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);

	LPCTSTR GetMenuText() const { return _T("Kanban"); } 
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return KANBAN_TYPEID; }

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

	bool SelectTask(DWORD dwTaskID);
	bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount);

	bool ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);

	bool DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData);
	bool CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const;

protected:
// Dialog Data
	//{{AFX_DATA(CKanbanWnd)
	CTabCtrl	m_tcShortcutBar;
	//}}AFX_DATA
	CKanbanCtrl m_ctrlKanban;
	CKanbanPreferencesDlg m_dlgPrefs;
	CKanbanAttributeComboBox m_cbAttributes;
	CKanbanOptionComboBox m_cbOptions;
	CKanbanCustomAttributeComboBox m_cbCustomAttributes;

	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CToolTipCtrl m_tooltips;

	BOOL m_bReadOnly;
	CIcon m_icon;
	CBrush m_brBack;
	UITHEME m_theme;
	CString	m_sTrackedCustomAttribID;
	TDC_ATTRIBUTE m_nTrackedAttrib;
	DWORD m_dwSelTaskID;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKanbanWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();
	virtual void OnCancel() {}
	virtual void OnOK() {}

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKanbanWnd)
	afx_msg void OnSelchangeTrackedAttribute();
	afx_msg void OnSelchangeOptions();
	//}}AFX_MSG
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKanbanPreferences();
	afx_msg void OnUpdateKanbanPreferences(CCmdUI* pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult );	
	afx_msg LRESULT OnGetFont(WPARAM wp, LPARAM lp);
	afx_msg void OnNcDestroy();

	afx_msg LRESULT OnKanbanNotifySelectionChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyValueChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyEditTaskDone(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyEditTaskFlag(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyFlagChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyGetTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyEditTaskTitle(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyEditTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanPrefsHelp(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

protected:
	void Resize(int cx = 0, int cy = 0);
	void UpdateKanbanCtrlPreferences(BOOL bFixedColumnsToggled);
	void SendParentSelectionUpdate();
	void EnableDisableCtrls();
	void UpdatePriorityColors(const IPreferences* pPrefs);
	void ProcessTrackedAttributeChange();
	void RefreshKanbanCtrlDisplayAttributes();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
