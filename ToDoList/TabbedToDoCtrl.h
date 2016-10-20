// FilteredToDoCtrl.h: interface for the CTabbedToDoCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABBEDTODOCTRL_H__356A6EB9_C7EC_4395_8716_623AFF4A269B__INCLUDED_)
#define AFX_TABBEDTODOCTRL_H__356A6EB9_C7EC_4395_8716_623AFF4A269B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToDoCtrl.h"
#include "tdltasklistctrl.h"
#include "tdlviewTabcontrol.h"

#include "..\shared\misc.h"
#include "..\shared\subclass.h"
#include "..\shared\UIExtensionMgr.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

struct VIEWDATA
{
    VIEWDATA()
		: 
		bNeedResort(FALSE), 
		pExtension(NULL),
		bNeedFullTaskUpdate(TRUE),
		bCanPrepareNewTask(-1)
	{
	}

	virtual ~VIEWDATA() {}

	TDSORT sort;
	CTaskListDropTarget dropTgt;
	IUIExtension* pExtension;
	CTDCAttributeMap mapWantedAttrib;

	BOOL bNeedResort;
	BOOL bNeedFullTaskUpdate;
	BOOL bCanPrepareNewTask;
};

/////////////////////////////////////////////////////////////////////////////

class CTabbedToDoCtrl : public CToDoCtrl 
{
public:
	CTabbedToDoCtrl(CUIExtensionMgr& mgrUIExt, 
					CContentMgr& mgrContent, 
					const CONTENTFORMAT& cfDefault,
					const TDCCOLEDITFILTERVISIBILITY& visDefault);

	virtual ~CTabbedToDoCtrl();
	
	int GetSelectedTasks(CTaskFile& tasks, const TDCGETTASKS& filter = TDCGT_ALL, DWORD dwFlags = 0) const;

	BOOL CreateNewTask(const CString& sText, TDC_INSERTWHERE nWhere = TDC_INSERTATTOPOFSELTASKPARENT, 
						BOOL bEditText = TRUE, DWORD dwDependency = 0);
	BOOL CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const;

	BOOL SelectTask(DWORD dwTaskID, BOOL bTrue = FALSE);

	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const;
	BOOL SelectTask(CString sPart, TDC_SELECTTASK nSelect); 
	
	void SetView(FTC_VIEW nView);
	void SetNextView();
	FTC_VIEW GetView() const { return m_tabViews.GetActiveView(); }

	void ShowListViewTab(BOOL bVisible = TRUE);
	BOOL IsListViewTabShowing() const;
	void SetVisibleExtensionViews(const CStringArray& aTypeIDs);
	int GetVisibleExtensionViews(CStringArray& aTypeIDs) const;

	BOOL SetTreeFont(HFONT hFont); // caller responsible for deleting

	TDC_HITTEST HitTest(const CPoint& ptScreen) const;
	TDC_COLUMN ColumnHitTest(const CPoint& ptScreen) const;
	BOOL WantTaskContextMenu() const;

	virtual BOOL IsSorting() const;
	virtual BOOL CanSortBy(TDC_COLUMN nBy) const;
	virtual void Resort(BOOL bAllowToggle = FALSE);
	virtual BOOL IsSortingBy(TDC_COLUMN nBy) const;
	virtual void Sort(TDC_COLUMN nBy, BOOL bAllowToggle = TRUE);
	virtual void MultiSort(const TDSORTCOLUMNS& sort);
	virtual TDC_COLUMN GetSortBy() const;
	virtual void GetSortBy(TDSORTCOLUMNS& sort) const;
	virtual BOOL IsMultiSorting() const;
	virtual BOOL CanMultiSort() const;

	BOOL DeleteSelectedTask() { return CToDoCtrl::DeleteSelectedTask(); }
	void SetModified(BOOL bMod = TRUE) { CToDoCtrl::SetModified(bMod); }
	BOOL SetStyle(TDC_STYLE nStyle, BOOL bOn = TRUE) { return CToDoCtrl::SetStyle(nStyle, bOn); }
	void SetMaximizeState(TDC_MAXSTATE nState);

	BOOL MoveSelectedTask(TDC_MOVETASK nDirection);
	BOOL CanMoveSelectedTask(TDC_MOVETASK nDirection) const;

	BOOL GotoNextTask(TDC_GOTO nDirection); 
	BOOL CanGotoNextTask(TDC_GOTO nDirection) const;
	BOOL GotoNextTopLevelTask(TDC_GOTO nDirection); 
	BOOL CanGotoNextTopLevelTask(TDC_GOTO nDirection) const;

	virtual BOOL CanExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand) const;
	virtual void ExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand = TRUE);

	void SetFocusToTasks();
	BOOL TasksHaveFocus() const;

	void SelectAll();
	void SelectTasksInHistory(BOOL bForward);
	BOOL SelectTasks(const CDWordArray& aTaskIDs);
	BOOL GetSelectionBoundingRect(CRect& rSelection) const;

	void SetUITheme(const CUIThemeFile& theme);
	void ResizeAttributeColumnsToFit();
	BOOL CanResizeAttributeColumnsToFit() const;
	void RedrawReminders();
	void SetMaxInfotipCommentsLength(int nLength);
	void EndTimeTracking(BOOL bAllowConfirm) { CToDoCtrl::EndTimeTracking(bAllowConfirm); }
	void BeginTimeTracking(DWORD dwTaskID) { CToDoCtrl::BeginTimeTracking(dwTaskID); }

	virtual CString GetControlDescription(const CWnd* pCtrl) const;
	virtual void RebuildCustomAttributeUI();
	virtual void NotifyBeginPreferencesUpdate();
	virtual void NotifyEndPreferencesUpdate();

	// override these so we can notify extensions of color changes
	void SetPriorityColors(const CDWordArray& aColors);
	void SetCompletedTaskColor(COLORREF color);
	void SetFlaggedTaskColor(COLORREF color);
	void SetReferenceTaskColor(COLORREF color);
	void SetAttributeColors(TDC_ATTRIBUTE nAttrib, const CTDCColorMap& colors);
	void SetStartedTaskColors(COLORREF crStarted, COLORREF crStartedToday);
	void SetDueTaskColors(COLORREF crDue, COLORREF crDueToday);
    void SetGridlineColor(COLORREF color);
	void SetAlternateLineColor(COLORREF color);

protected:
	CTDLTaskListCtrl m_taskList;
	CTDLViewTabControl m_tabViews;
	CTaskListDropTarget m_dtList;
	CArray<IUIExtensionWindow*, IUIExtensionWindow*> m_aExtViews;
	const CUIExtensionMgr& m_mgrUIExt;

	BOOL m_bTaskColorChange;
	BOOL m_bUpdatingExtensions;
	BOOL m_bRecreatingRecurringTasks;

	IUI_ATTRIBUTE m_nExtModifyingAttrib;
	CDWordArray m_aRecreatedRecurringTasks;

	// almost all of the tree's view data is stored in CToDoCtrl
	// the only extra piece is whether or not the tree needs to
	// be resorted when we switch to it from another view
	mutable BOOL m_bTreeNeedResort;

// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabbedToDoCtrl)
	//}}AFX_VIRTUAL
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	// Generated message map functions
	//{{AFX_MSG(CTabbedToDoCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg void OnTabCtrlRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg LRESULT OnDropObject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPreTabViewChange(WPARAM nOldView, LPARAM nNewView);
	afx_msg LRESULT OnPostTabViewChange(WPARAM nOldView, LPARAM nNewView);
	afx_msg LRESULT OnTDCGetTaskReminder(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnColumnEditClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecreateRecurringTask(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnUIExtSelectTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtModifySelectedTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtEditSelectedTaskTitle(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtSortColumnChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtDoHelp(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL ModNeedsResort(TDC_ATTRIBUTE nModType) const;
	BOOL ModCausesColorChange(TDC_ATTRIBUTE nModType) const;
	BOOL ModCausesColorChange(const CTDCAttributeMap& mapAttrib) const;
	const TDSORT& GetSort() const;

	virtual BOOL DeleteSelectedTask(BOOL bWarnUser, BOOL bResetSel = FALSE);
	virtual void SetModified(BOOL bMod, TDC_ATTRIBUTE nAttrib, DWORD dwModTaskID);
	virtual void ReposTaskTree(CDeferWndMove* pDWM, const CRect& rPos);
	virtual BOOL SetStyle(TDC_STYLE nStyle, BOOL bOn, BOOL bWantUpdate); // one style at a time only 
	virtual void UpdateTasklistVisibility();
	virtual void UpdateVisibleColumns();
	virtual void EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify);
	virtual void BeginTimeTracking(DWORD dwTaskID, BOOL bNotify);
	virtual BOOL SelectTasks(const CDWordArray& aTaskIDs, BOOL bTrue);

	virtual BOOL LoadTasks(const CTaskFile& file);
	virtual void SaveTasksState(CPreferences& prefs, BOOL bRebuildTree = FALSE) const; 
	virtual HTREEITEM LoadTasksState(const CPreferences& prefs, BOOL bRebuildTree = FALSE);

	virtual void Resize(int cx = 0, int cy = 0, BOOL bSplitting = FALSE);
	virtual void OnStylesUpdated();
	virtual void OnTaskIconsChanged();

	void DrawListColumnHeaderText(CDC* pDC, int nCol, const CRect& rCol, UINT nState);
	void RemeasureList();
	void UpdateTreeSelection();
	void UpdateSelectedTaskPath();
	void InvalidateItem(HTREEITEM hti, BOOL bUpdate);
	int FindListTask(const CString& sPart, int nStart = 0, BOOL bNext = TRUE);
	void SetEditTitleTaskID(DWORD dwTaskID);
	void ResortSelectedTaskParents();
	void LoadPrefs();
	void SavePrefs();
	BOOL IsCalculatedAttribute(TDC_ATTRIBUTE nAttrib) const;
	void UpdateListView(TDC_ATTRIBUTE nAttrib, DWORD dwTaskID = 0);

	void ResyncListSelection();
	void ResyncExtensionSelection(FTC_VIEW nView);
	BOOL IsItemSelected(int nItem) const;
	BOOL HasSingleSelectionChanged(DWORD dwSelID) const;
	DWORD GetSingleSelectedTaskID() const;
	int CacheListSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs = TRUE) const;
	void RestoreListSelection(const TDCSELECTIONCACHE& cache);

	int GetListColumnAlignment(int nDTAlign); 

	VIEWDATA* GetActiveViewData() const;
	VIEWDATA* GetViewData(FTC_VIEW nView) const;
	
	BOOL AddView(IUIExtension* pExtension);
	BOOL RemoveView(IUIExtension* pExtension);

	virtual void RebuildList(const void* pContext);
	virtual void AddTreeItemToList(HTREEITEM hti, const void* pContext);
	virtual BOOL GetLabelEditRect(CRect& rScreen); // screen coords

	HTREEITEM GetTreeItem(int nItem) const;
	int GetListItem(HTREEITEM hti) const;
	DWORD GetTaskID(int nItem) const { return ((nItem == -1) ? 0 : m_taskList.List().GetItemData(nItem)); }
	DWORD GetTaskID(HTREEITEM hti) const { return CToDoCtrl::GetTaskID(hti); }
	TODOITEM* CreateNewTask(HTREEITEM htiParent);

	enum TTC_NEXTTASK
	{
		TTCNT_NEXT,
		TTCNT_PREV,
		TTCNT_NEXTTOPLEVEL,
		TTCNT_PREVTOPLEVEL,
	};
	DWORD GetNextTaskID(DWORD dwTaskID, TTC_NEXTTASK nNext, BOOL bExcludeSelected) const;

	BOOL InListView() const;
	BOOL InTreeView() const;
	BOOL InExtensionView() const;
	BOOL IsViewSet() const;

	void UpdateExtensionViews(TDC_ATTRIBUTE nAttrib, DWORD dwTaskID = 0);
	void ExtensionDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd, DWORD dwExtra = 0);
	BOOL ExtensionCanDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd, DWORD dwExtra = 0) const;
	IUIExtensionWindow* GetCreateExtensionWnd(FTC_VIEW nView);
	IUIExtensionWindow* GetExtensionWnd(FTC_VIEW nView) const;
	BOOL GetExtensionWnd(FTC_VIEW nView, IUIExtensionWindow*& pExtWnd, VIEWDATA*& pData) const;
	BOOL HasAnyExtensionViews() const;
	BOOL AnyExtensionViewWantsChange(TDC_ATTRIBUTE nAttrib) const;
	BOOL AnyExtensionViewWantsChange(const CTDCAttributeMap& mapAttrib) const;
	BOOL ExtensionViewWantsChange(int nExt, const CTDCAttributeMap& mapAttrib) const;
	BOOL ExtensionViewWantsChange(int nExt, TDC_ATTRIBUTE nAttrib) const;
	BOOL AllExtensionViewsNeedFullUpdate() const;
	void BeginExtensionProgress(const VIEWDATA* pData, UINT nMsg = 0);
	void EndExtensionProgress();
	void UpdateExtensionView(IUIExtensionWindow* pExtWnd, const CTaskFile& tasks, IUI_UPDATETYPE nType, const CTDCAttributeMap& mapAttrib);
	void RefreshExtensionViewSelection();
	void SetExtensionsNeedUpdate(BOOL bUpdate, FTC_VIEW nIgnore = FTCV_UNSET);
	BOOL ProcessUIExtensionMod(const IUITASKMOD& mod);
	int GetAllExtensionViewsWantedAttributes(CTDCAttributeMap& mapAttrib) const;
	CString GetExtensionPrefsSubKey(const IUIExtensionWindow* pExtWnd);
	void UpdateExtensionViewsSelection(TDC_ATTRIBUTE nAttrib);
	BOOL IsExtensionView(HWND hWnd) const;
	BOOL SelectExtensionTasks(IUIExtensionWindow* pExtWnd, const CDWordArray& aTasks, DWORD dwFocusedTask);

	virtual BOOL GetAllTasksForExtensionViewUpdate(CTaskFile& tasks, const CTDCAttributeMap& mapAttrib) const;
	BOOL GetSelectedTasksForExtensionViewUpdate(CTaskFile& tasks, const CTDCAttributeMap& mapAttrib, DWORD dwFlags) const;
	void GetAttributesAffectedByMod(TDC_ATTRIBUTE nAttrib, CTDCAttributeMap& mapAttrib) const;
	int GetTasks(CTaskFile& tasks, FTC_VIEW nView, const TDCGETTASKS& filter) const;
	int GetSelectedTasks(CTaskFile& tasks, FTC_VIEW nView, const TDCGETTASKS& filter, DWORD dwFlags) const;

	BOOL AddTreeChildrenToTaskFile(HTREEITEM hti, CTaskFile& file, HTASKITEM hTask, const TDCGETTASKS& filter) const;
	BOOL AddTreeItemToTaskFile(HTREEITEM hti, CTaskFile& file, HTASKITEM hParentTask, const TDCGETTASKS& filter) const;
	void AddGlobalsToTaskFile(CTaskFile& tasks, const CTDCAttributeMap& mapAttrib) const;
	int GetGlobals(TDC_ATTRIBUTE nAttrib, TDCAUTOLISTDATA& tld) const;

	virtual VIEWDATA* NewViewData() { return new VIEWDATA(); }

	static BOOL IsExtensionView(FTC_VIEW nView);
	static int GetExtensionViewAttributes(IUIExtensionWindow* pExtWnd, CTDCAttributeMap& mapAttrib);
	static IUI_APPCOMMAND MapGetNextToCommand(TTC_NEXTTASK nNext);
	static TTC_NEXTTASK MapGotoToGetNext(TDC_GOTO nDirection, BOOL bTopLevel);
};

#endif // !defined(AFX_TABBEDTODOCTRL_H__356A6EB9_C7EC_4395_8716_623AFF4A269B__INCLUDED_)
