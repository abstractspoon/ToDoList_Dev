#if !defined(AFX_BurndownWND_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BurndownSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurndownWnd.h : header file
//

#include <afxtempl.h>

#include "BurndownChart.h"
#include "BurndownStruct.h"

#include "..\Shared\mapex.h"
#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"

#include "..\Interfaces\uitheme.h"
#include "..\Interfaces\Itasklist.h"
#include "..\Interfaces\IUIExtension.h"

/////////////////////////////////////////////////////////////////////////////
// CBurndownWnd dialog

class CBurndownWnd : public CDialog, public IUIExtensionWindow
{
// Construction
public:
	CBurndownWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBurndownWnd();

	// IUIExtensionWindow
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);

	// IUIExtensionWindow
	LPCTSTR GetMenuText() const { return _T("Burndown"); }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return STATS_TYPEID; }

	void SetReadOnly(bool /*bReadOnly*/) {}
	HWND GetHwnd() const { return GetSafeHwnd(); }
	void SetUITheme(const UITHEME* pTheme);

	bool GetLabelEditRect(LPRECT pEdit);
	IUI_HITTEST HitTest(const POINT& ptScreen) const;

	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly);
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes);
	bool WantTaskUpdate(IUI_ATTRIBUTE nAttribute) const;
	bool PrepareNewTask(ITaskList* /*pTask*/) const;

	bool SelectTask(DWORD dwTaskID);
	bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount);

	bool ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);

	bool DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData);
	bool CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const;

protected:
// Dialog Data
	//{{AFX_DATA(CBurndownWnd)
	enum { IDD = IDD_STATISTICS_DLG };
	//}}AFX_DATA
	CIcon m_icon;
	CBrush m_brBack;
	UITHEME m_theme;

	CStatic	m_stFrame;
	CBurndownChart m_graph;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CComboBox m_cbDisplay;

	int m_nScale;
	int	m_nDisplay;
	DWORD m_dwUpdateGraphOnShow;
	int m_nDaysInWeek;
	double m_dHoursInDay;

	CStatsItemArray m_data;
	COleDateTime m_dtEarliestDate, m_dtLatestDate;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBurndownWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	virtual void OnCancel() {}
	virtual void OnOK() {}

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBurndownWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnSelchangeDisplay();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnRebuildGraph(WPARAM wp, LPARAM lp);
	afx_msg void OnNcDestroy();
	DECLARE_MESSAGE_MAP()

protected:
	void UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	void UpdateDataExtents();
	BOOL RemoveDeletedTasks(const ITASKLISTBASE* pTasks);
	void BuildData(const ITASKLISTBASE* pTasks);
	void BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings, BOOL bCheckExist);

	void RebuildGraph(BOOL bSortData, BOOL bUpdateExtents, BOOL bCheckVisibility);
	void BuildSprintGraph();
	void BuildBurndownGraph();
	double GetTaskTimeInDays(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bEstimate);
	void RebuildXScale();
	int GetDataDuration() const;
	COleDateTime GetGraphStartDate() const;
	COleDateTime GetGraphEndDate() const;
	int CalculateRequiredXScale() const;
	COleDateTime GetTaskStartDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask);
	COleDateTime GetTaskDoneDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask);

	static COleDateTime GetTaskDate(time64_t tDate);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BurndownWND_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
