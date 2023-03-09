#if !defined(AFX_BurndownWND_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BurndownSTRUCT_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurndownWnd.h : header file
//

#include <afxtempl.h>

#include "BurndownExt.h"
#include "BurndownChart.h"
#include "BurndownStruct.h"
#include "BurndownGraphComboBox.h"
#include "BurndownOptionsComboBox.h"
#include "BurndownPreferencesDlg.h"

#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"
#include "..\Shared\RangeSliderCtrl.h"

#include "..\Interfaces\uitheme.h"
#include "..\Interfaces\ITaskList.h"
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
	LPCTSTR GetMenuText() const { return STATS_MENUTEXT; }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return STATS_TYPEID; }

	void SetReadOnly(bool /*bReadOnly*/) {}
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
	bool PrepareNewTask(ITaskList* /*pTask*/) const;

	bool SelectTask(DWORD dwTaskID, bool bTaskLink);
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

	CStatsItemArray m_data;
	COleDateTimeRange m_dtDataRange, m_dtPrevActiveRange;

	CStatic	m_stFrame;
	CBurndownChart m_chart;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CRangeSliderCtrl m_sliderDateRange;
	CBurndownGraphComboBox m_cbGraphs;
	CBurndownOptionsComboBox m_cbOptions;
	CBurndownPreferencesDlg m_dlgPrefs;
	
	DWORD m_dwUpdateGraphOnShow;
	BOOL m_bUpdatingSlider;
	BOOL m_bVisible;
	BURNDOWN_GRAPH m_nGraph;

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
	afx_msg void OnNcDestroy();
	afx_msg void OnOptionChanged();
	afx_msg void OnPreferences();

	afx_msg LRESULT OnRebuildGraph(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnActiveDateRangeChange(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

protected:
	void UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings);
	BOOL RemoveDeletedTasks(const ITASKLISTBASE* pTasks);
	void BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings, BOOL bCheckExist);

	void RebuildGraph(BOOL bSortData, BOOL bUpdateExtents, BOOL bCheckVisibility);
	void UpdateRangeSlider(const COleDateTimeRange& dtActiveRange);
	void UpdateActiveRangeLabel(const COleDateTimeRange& dtActiveRange);
	void UpdateRangeSliderStep();
	BOOL GetSliderDateRange(COleDateTimeRange& dtActiveRange) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BurndownWND_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
