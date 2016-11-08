#pragma once
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////

#include "taskfile.h"
#include "FilteredToDoCtrl.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\mapex.h"
#include "..\shared\uithemefile.h"
#include "..\shared\iconbutton.h"
#include "..\shared\WndPrompt.h"
#include "..\shared\entoolbar.h"
#include "..\shared\toolbarhelper.h"
#include "..\shared\subclass.h"

/////////////////////////////////////////////////////////////////////////

enum
{
	TTDO_FORMATTIMESASHMS		= 0X01,
	TTDO_SHOWONBEGINTRACKING	= 0X02,
	TTDO_ALLOWPARENTTRACKING	= 0X04,
	TTDO_SHOWTASKPATH			= 0X08,
};

/////////////////////////////////////////////////////////////////////////

struct TRACKITEM
{
	TRACKITEM();

	BOOL operator==(const TRACKITEM& ti) const;
	BOOL operator!=(const TRACKITEM& ti) const;
	CString GetTaskTitle(BOOL bWantPath) const;
	
	CString sTaskTitle;
	CString sTaskPath;
	DWORD dwTaskID;
	BOOL bParent;
};
typedef CArray<TRACKITEM, TRACKITEM&> CTrackItemArray;

/////////////////////////////////////////////////////////////////////////

enum 
{
	TTL_REMOVEDELETED	= 0x01,
	TTL_REMOVEDONE		= 0x02,
};

struct TRACKTASKLIST
{
	TRACKTASKLIST();
	virtual ~TRACKTASKLIST();

	int SetTasks(const CTaskFile& tasks);
	BOOL UpdateTasks(const CTaskFile& tasks);
	BOOL RemoveTasks(DWORD dwToRemove);

	BOOL IsTracking(DWORD dwTaskID = 0) const;

	const CFilteredToDoCtrl* pTDC;
	DWORD dwTrackedTaskID;
	BOOL bTrackingPaused;
	CTrackItemArray* pTasks;

protected:
	typedef CMap<DWORD, DWORD, int, int> CMapTaskIndex;
	void BuildTaskMap(CMapTaskIndex& mapTasks) const;
	BOOL UpdateTasks(const CTaskFile& tasks, HTASKITEM hTask, const CString& sParentPath, const CMapTaskIndex& mapTasks);
};

/////////////////////////////////////////////////////////////////////////

class CTDCTrackTasklistArray : protected CArray<TRACKTASKLIST*, TRACKTASKLIST*&>
{
public:
	CTDCTrackTasklistArray();
	virtual ~CTDCTrackTasklistArray();

	int GetNumTasklists() const;
	BOOL UpdateTracking(const CFilteredToDoCtrl* pTDC);

	int AddTasklist(const CFilteredToDoCtrl* pTDC, const CTaskFile& tasks);
	BOOL DeleteTasklist(const CFilteredToDoCtrl* pTDC);
	void DeleteAllTasklists();
	int FindTasklist(const CFilteredToDoCtrl* pTDC) const;

	const TRACKTASKLIST* GetTasklist(const CFilteredToDoCtrl* pTDC) const;
	TRACKTASKLIST* GetTasklist(const CFilteredToDoCtrl* pTDC);
	const TRACKTASKLIST* GetTasklist(int nTasklist) const;

	const CTrackItemArray* GetTasks(const CFilteredToDoCtrl* pTDC) const;
	CTrackItemArray* GetTasks(const CFilteredToDoCtrl* pTDC);

};

/////////////////////////////////////////////////////////////////////////

//#define WHITETHEME

class CTDLTimeTrackerDlg : public CDialog, protected CDialogHelper
{
public:
	CTDLTimeTrackerDlg();
	virtual ~CTDLTimeTrackerDlg();
	
	BOOL Create(DWORD dwOptions = (TTDO_ALLOWPARENTTRACKING | TTDO_FORMATTIMESASHMS));
	void SetUITheme(const CUIThemeFile& theme);
	void SetOption(DWORD dwOption, BOOL bEnable = TRUE);
	void SetWindowIcons(HICON hIconBig, HICON hIconSmall);
	
	BOOL AddTasklist(const CFilteredToDoCtrl* pTDC);
	BOOL AddTasklist(const CFilteredToDoCtrl* pTDC, const CTaskFile& tasks);
	BOOL RemoveTasklist(const CFilteredToDoCtrl* pTDC);
	void RemoveAllTasklists();
	BOOL UpdateTasks(const CFilteredToDoCtrl* pTDC, const CTaskFile& tasks);
	void RemoveDeletedTasks(const CFilteredToDoCtrl* pTDC);
	void RemoveCompletedTasks(const CFilteredToDoCtrl* pTDC);
	BOOL UpdateTracking(const CFilteredToDoCtrl* pTDC);
	void UpdateTaskTime(const CFilteredToDoCtrl* pTDC);
	void UpdateTasklistName(const CFilteredToDoCtrl* pTDC);

	BOOL SelectTaskList(const CFilteredToDoCtrl* pTDC);
	const CFilteredToDoCtrl* GetSelectedTasklist() const;
	DWORD GetSelectedTaskID() const;
	BOOL IsAlwaysOnTop() const { return m_bAlwaysOnTop; }
		
protected:
	enum { IDD = IDD_TIMETRACK_DIALOG };
	
	BOOL m_bAlwaysOnTop;
	BOOL m_bCollapsed;
	BOOL m_bRecreating;

	CComboBox m_cbTasklists;
	CComboBox m_cbTasks;
	CWnd* m_pWndNotify;
	CIconButton m_btnStart;
	CWndPromptManager m_mgrPrompts;
	CSize m_sizeMin, m_sizeMax, m_sizeLast;
	
	CTDCTrackTasklistArray m_aTasklists;
	CUIThemeFile m_theme;
	CImageList m_ilBtns;
	CString m_sTaskTimes;
	CString m_sQuickFind;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CString m_sOrgTitle;
	DWORD m_dwOptions;

#ifdef WHITETHEME
#else
	CBrush m_brBack;
#endif

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual int  DoModal() { ASSERT(0); return IDCANCEL; }
	virtual void OnCancel() { ShowWindow(SW_HIDE); }
	virtual void OnOK() { OnStartStopTracking(); }
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
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
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	
protected:
	DWORD GetTasklistTrackID(const CFilteredToDoCtrl* pTDC) const;
	int GetTasklistCBIndex(const CFilteredToDoCtrl* pTDC) const;
	BOOL HasTasklist(const CFilteredToDoCtrl* pTDC) const;
	void RemoveTasks(const CFilteredToDoCtrl* pTDC, DWORD dwToRemove);
	
	BOOL Create(BOOL bVisible);
	BOOL Recreate();
	void UpdateButtonState();
	void RebuildTaskCombo();
	BOOL RebuildTasklistCombo();
	COLORREF GetBkgndColor() const;
	BOOL IsTrackingSelectedTasklistAndTask() const;
	BOOL IsSelectedTask(DWORD dwTaskID) const;
	BOOL IsSelectedTasklist(const CFilteredToDoCtrl* pTDC) const;
	int QuickFindNextTaskComboItem(int nFrom, BOOL bForward) const;
	BOOL QuickFindNextTaskComboItem(int nFrom, int nTo, int nIncrement, int& nNext) const;
	void LoadSettings();
	void SaveSettings() const;
	void SetAlwaysOnTop(BOOL bTopMost);

	void Resize(int cx = 0, int cy = 0);
	int CalcAvailableRows(int nHeight) const;
	void CalcMinMaxSizes();
	BOOL HasOption(DWORD dwOption) const;

};
