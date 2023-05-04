#pragma once
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////

#include "taskfile.h"
#include "TDLTimeTrackerTaskComboBox.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\iconbutton.h"
#include "..\shared\WndPrompt.h"
#include "..\shared\entoolbar.h"
#include "..\shared\toolbarhelper.h"
#include "..\shared\icon.h"
#include "..\shared\enedit.h"
#include "..\shared\TooltipCtrlex.h"

#include "..\Interfaces\uithemefile.h"

/////////////////////////////////////////////////////////////////////////

enum
{
	TTDO_FORMATTIMESASHMS		= 0X01,
	TTDO_SHOWONBEGINTRACKING	= 0X02,
	TTDO_ALLOWPARENTTRACKING	= 0X04,
	TTDO_SHOWPARENTSASFOLDERS	= 0X08,
};

/////////////////////////////////////////////////////////////////////////

class CToDoCtrl;

/////////////////////////////////////////////////////////////////////////

class CTDLTimeTrackerDlg : public CDialog, protected CDialogHelper
{
public:
	CTDLTimeTrackerDlg();
	virtual ~CTDLTimeTrackerDlg();
	
	BOOL Create(CWnd* pNotify, DWORD dwOptions = (TTDO_ALLOWPARENTTRACKING | TTDO_FORMATTIMESASHMS));
	void SetUITheme(const CUIThemeFile& theme);
	void SetOption(DWORD dwOption, BOOL bEnable = TRUE);
	void SetWindowIcons(HICON hIconBig, HICON hIconSmall);
	void SetStartStopShortcut(DWORD dwShortcut) { m_dwStartStopShortcut = dwShortcut; }
	
	BOOL AddTasklist(const CToDoCtrl* pTDC, const CTaskFile& tasks);
	BOOL SetTasks(const CToDoCtrl* pTDC, const CTaskFile& tasks);
	BOOL RemoveTasklist(const CToDoCtrl* pTDC);
	void RemoveAllTasklists();
	void UpdateAllTasks(const CToDoCtrl* pTDC);
	void UpdateSelectedTasks(const CToDoCtrl* pTDC, const CTDCAttributeMap& mapAttrib);
	void UpdateTracking(const CToDoCtrl* pTDC);
	void UpdateTaskTime(const CToDoCtrl* pTDC);
	void UpdateTasklistName(const CToDoCtrl* pTDC);
	BOOL IsSelectedTasklist(const CToDoCtrl* pTDC) const;

	BOOL SelectTaskList(const CToDoCtrl* pTDC);
	const CToDoCtrl* GetSelectedTasklist() const;
	DWORD GetSelectedTaskID() const { return m_cbTasks.GetSelectedTaskID(); }
	BOOL IsAlwaysOnTop() const { return m_bAlwaysOnTop; }
	BOOL IsForegroundWindow() const { return (::GetForegroundWindow() == GetSafeHwnd()); }
		
protected:
	enum { IDD = IDD_TIMETRACK_DIALOG };
	
	BOOL m_bAlwaysOnTop;
	BOOL m_bCollapsed;
	BOOL m_bRecreating;
	BOOL m_bCentreOnShow;

	CComboBox m_cbTasklists;
	CTDLTimeTrackerTaskComboBox m_cbTasks;
	CWnd* m_pWndNotify;
	CIconButton m_btnStart;
	CWndPromptManager m_mgrPrompts;
	CIcon m_iconDlg, m_iconResetElapsed;
	CBrush m_brBack;
	CEnToolBar m_toolbar;
	CEnEdit m_eElapsedTime;
	CImageList m_ilBtns;
	CToolTipCtrlEx m_tipCaption;
	
	CTDCTrackTasklistArray m_aTasklists;
	CUIThemeFile m_theme;
	CString m_sTaskTimes, m_sElapsedTime;
	CString m_sQuickFind;
	CToolbarHelper m_tbHelper;
	CString m_sCaption, m_sOrgCaption;
	DWORD m_dwOptions, m_dwStartStopShortcut;
	CSize m_sizeMin, m_sizeMax;
	int m_nUncollapsedHeight;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual int  DoModal() { ASSERT(0); return IDCANCEL; }
	virtual void OnCancel() { ShowWindow(SW_HIDE); }
	virtual void OnOK() { OnStartStopTracking(); }
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg void OnStartStopTracking();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
#if _MSC_VER >= 1400
	afx_msg LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg UINT OnNcHitTest(CPoint point);
#endif
	afx_msg void OnSelchangeTasklist();
	afx_msg void OnSelchangeTask();
	afx_msg void OnChangeQuickFind();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* pMMI);
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnToggleTopMost();
	afx_msg void OnHelp();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg LRESULT OnEEBtnClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToolHitTest(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	
protected:
	DWORD GetTasklistTrackID(const CToDoCtrl* pTDC) const;
	int GetTasklistCBIndex(const CToDoCtrl* pTDC) const;
	BOOL HasTasklist(const CToDoCtrl* pTDC) const;
	BOOL RemoveTasks(const CToDoCtrl* pTDC, DWORD dwToRemove);
	void DoUpdateAllTasks(const TRACKTASKLIST* pTTL);
	void DoRebuildTaskCombo(TRACKTASKLIST* pTTL);

	BOOL Create();
	BOOL Recreate();
	void UpdatePlayButton(BOOL bCheckVisibility = TRUE);
	BOOL RebuildTasklistCombo();
	COLORREF GetBkgndColor() const;
	BOOL IsTrackingSelectedTasklistAndTask() const;
	CString GetSelectedTaskTitle() const;
	CString GetSelectedTasklistName() const;
	void LoadSettings();
	void SaveSettings() const;
	void SetAlwaysOnTop(BOOL bTopMost);
	void CollapseWindow(BOOL bCollapse);

	void Resize(int cx = 0, int cy = 0);
	int CalcAvailableRows(int nHeight) const;
	void CalcMinMaxSizes();
	BOOL HasOption(DWORD dwOption) const;
	void RefreshCaptionText();
	LRESULT SendNotifyMessage(UINT message, const CToDoCtrl* pTDC, DWORD dwTaskID) const;
	void UpdateTaskTime(const CToDoCtrl* pTDC, BOOL bCheckVisibility);

};
