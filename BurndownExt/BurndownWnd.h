#if !defined(AFX_BurndownWND_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BurndownWND_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurndownWnd.h : header file
//

#include <afxtempl.h>

#include "BurndownChart.h"

#include "..\Shared\mapex.h"
#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"
#include "..\shared\timehelper.h"

#include "..\Interfaces\uitheme.h"
#include "..\Interfaces\Itasklist.h"
#include "..\Interfaces\IUIExtension.h"

/////////////////////////////////////////////////////////////////////////////

struct STATSITEM 
{ 
	STATSITEM();
	virtual ~STATSITEM();

	BOOL HasStart() const;
	BOOL IsDone() const;
	
	void MinMax(COleDateTime& dtMin, COleDateTime& dtMax) const;
	
	COleDateTime dtStart, dtDone; 
	double dTimeEstDays, dTimeSpentDays;
	DWORD dwTaskID;

protected:
	static void MinMax(const COleDateTime& date, COleDateTime& dtMin, COleDateTime& dtMax);

};
typedef CMap<DWORD, DWORD, STATSITEM, STATSITEM&> CMapStatsItems;

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
	void Release();

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
	bool WantEditUpdate(IUI_ATTRIBUTE nAttribute) const;
	bool WantSortUpdate(IUI_ATTRIBUTE nAttribute) const;
	bool PrepareNewTask(ITaskList* /*pTask*/) const;

	bool SelectTask(DWORD dwTaskID);
	bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount);

	bool ProcessMessage(MSG* pMsg);

	bool DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra);
	bool CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const;

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

	CMapStatsItems m_data;
	CDWordArray m_aDateOrdered;
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
	DECLARE_MESSAGE_MAP()

protected:
	void UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	void UpdateDataExtents();
	BOOL RemoveDeletedTasks(const ITASKLISTBASE* pTasks);
	void BuildData(const ITASKLISTBASE* pTasks);
	void BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings);
	void SortData();
	BOOL IsDataSorted() const;
	void RebuildGraph(BOOL bSortData, BOOL bUpdateExtents);
	void BuildSprintGraph();
	void BuildBurndownGraph();
	int CalculateIncompleteTaskCount(const COleDateTime& date);
	void BuildBurndown2Graph();
	int CalculateIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nLastDone);
	double CalculateTimeSpentInDays(const COleDateTime& date);
	double GetTaskTimeInDays(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bEstimate);
	double CalcTotalTimeEstimateInDays() const;
	BOOL GetStatsItem(DWORD dwTaskID, STATSITEM& si) const;
	void RebuildXScale();
	int GetDataDuration() const;
	COleDateTime GetGraphStartDate() const;
	COleDateTime GetGraphEndDate() const;
	int CalculateRequiredXScale() const;
	COleDateTime GetTaskStartDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask);
	COleDateTime GetTaskDoneDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask);

	static int CompareStatItems(const void* pV1, const void* pV2);
	static COleDateTime GetTaskDate(time64_t tDate);

protected:
	static TH_UNITS MapUnitsToTHUnits(TDC_UNITS nUnits);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BurndownWND_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
