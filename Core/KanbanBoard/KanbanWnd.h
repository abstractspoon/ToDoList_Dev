#if !defined(AFX_KANBANWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
#define AFX_KANBANWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanWnd.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "KanbanCtrl.h"
#include "KanbanPreferencesDlg.h"
#include "KanbanOptionComboBox.h"
#include "KanbanGroupByComboBox.h"

#include "..\Shared\tabbedcombobox.h"
#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"
#include "..\Shared\dialoghelper.h"
#include "..\Shared\checkcombobox.h"
#include "..\Shared\WndPrompt.h"

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

	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);

	// IUIExtensionWindow
	LPCTSTR GetMenuText() const { return _T("Kanban"); } 
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return KANBAN_TYPEID; }

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

	bool DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData);
	bool CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const;

	// Not required
	bool DoIdleProcessing() { return false; }
	bool ShowContextMenu(POINT /*ptScreen*/) { return false; }

protected:
	CKanbanCtrl m_ctrlKanban;
	CKanbanPreferencesDlg m_dlgPrefs;
	CKanbanAttributeComboBox m_cbAttributes;
	CKanbanGroupByComboBox m_cbGroupBy;
	CKanbanOptionComboBox m_cbOptions;

	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;

	BOOL m_bReadOnly;
	CIcon m_icon;
	CBrush m_brBack;
	UITHEME m_theme;
	CString	m_sTrackedCustomAttribID, m_sGroupByCustomAttribID;
	TDC_ATTRIBUTE m_nTrackedAttribID, m_nGroupByAttribID;
	CDWordArray m_aSelTaskIDs;
	CWndPromptManager m_mgrPrompts;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnCancel() {}
	virtual void OnOK() {}

protected:
	afx_msg void OnSelchangeTrackedAttribute();
	afx_msg void OnSelchangeOptions();
	afx_msg void OnSelchangeGroupBy();
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKanbanPreferences();
	afx_msg void OnUpdateKanbanPreferences(CCmdUI* pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnGetFont(WPARAM wp, LPARAM lp);
	afx_msg void OnNcDestroy();

	afx_msg LRESULT OnKanbanNotifySelectionChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyValueChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyEditTaskDone(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyEditTaskFlag(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyGetTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyEditTaskTitle(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyEditTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyEditTaskLock(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifySortChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanNotifyShowFileLink(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnKanbanPrefsHelp(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

protected:
	void Resize(int cx = 0, int cy = 0);
	void UpdateKanbanCtrlPreferences(BOOL bFixedColumnsToggled);
	void SendParentSelectionUpdate();
	void UpdatePriorityColors(const IPreferences* pPrefs);
	void ProcessTrackedAttributeChange();
	void RefreshKanbanCtrlDisplayAttributes();
	void RefreshGrouping();

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_KANBANWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
