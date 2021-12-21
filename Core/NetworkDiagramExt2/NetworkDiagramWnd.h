#if !defined(AFX_NETWORKDIAGRAMWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
#define AFX_NETWORKDIAGRAMWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GanttChartWnd.h : header file
//

#include "NetworkDiagramCtrl.h"

#include "..\shared\Icon.h"
#include "..\shared\EnString.h"

#include "..\Interfaces\uitheme.h"
#include "..\Interfaces\IUIExtension.h"

/////////////////////////////////////////////////////////////////////////////
// CGanttChartWnd window

class CNetworkDiagramWnd : public CDialog, public IUIExtensionWindow
{
// Construction
public:
	CNetworkDiagramWnd(CWnd* pParent = NULL);
	virtual ~CNetworkDiagramWnd();

	// IUIExtensionWindow
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);

	LPCTSTR GetMenuText() const { return _T("Network Diagram"); }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return NETWORKDIAGRAM_TYPEID; }

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
	//{{AFX_DATA(CGanttChartWnd)
	//}}AFX_DATA
	CNetworkDiagramCtrl m_ctrlNetwork;

	CIcon m_icon;
	CBrush m_brBack;
	UITHEME m_theme;
	
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
	afx_msg void OnClickNetworkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnNcDestroy();

	DECLARE_MESSAGE_MAP()

protected:
	void Resize(int cx, int cy);
	void SendParentSelectionUpdate();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKDIAGRAMWND_H__1571B442_7ED5_45D8_A040_C359EAE9FDE1__INCLUDED_)
