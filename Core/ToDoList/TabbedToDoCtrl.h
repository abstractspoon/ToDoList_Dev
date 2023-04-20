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
#include "tdltasklistctrloptionscombobox.h"

#include "..\shared\misc.h"
#include "..\shared\subclass.h"

#include "..\Interfaces\UIExtensionMgr.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

struct VIEWDATA
{
    VIEWDATA();
	virtual ~VIEWDATA();

	BOOL WantAttribute(TDC_ATTRIBUTE nAttribID) const;
	BOOL WantAnyAttribute(const CTDCAttributeMap& other) const;

	TDSORT sort;
	CTaskListDropTarget dropTgt;
	IUIExtension* pExtension;
	CTDCAttributeMap mapWantedAttrib;

	BOOL bNeedResort;
	BOOL bNeedFullTaskUpdate;
	BOOL bNeedFontUpdate;
	BOOL bCanPrepareNewTask;
	BOOL bHasSelectedTask;
};

/////////////////////////////////////////////////////////////////////////////

class CTabbedToDoCtrl : public CToDoCtrl 
{
public:
	CTabbedToDoCtrl(CUIExtensionMgr& mgrUIExt, 
					CTDLContentMgr& mgrContent, 
					CShortcutManager& mgrShortcuts,
					const CONTENTFORMAT& cfDefault,
					const TDCCOLEDITFILTERVISIBILITY& visDefault);

	virtual ~CTabbedToDoCtrl();
	
	BOOL CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const;
	BOOL CreateNewTask(const CString& sText, TDC_INSERTWHERE nWhere = TDC_INSERTATTOPOFSELTASKPARENT, 
						BOOL bEditText = TRUE, DWORD dwDependency = 0);

	virtual BOOL SelectTask(DWORD dwTaskID, BOOL bTaskLink);
	virtual BOOL SelectTasks(const CDWordArray& aTaskIDs);

	int GetSelectedTasks(CTaskFile& tasks, const TDCGETTASKS& filter = TDCGT_ALL) const;
	int GetSelectedTaskCount() const;
	BOOL HasSelection() const { return GetSelectedTaskCount(); }
	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const;
	BOOL SelectNextTask(CString sPart, TDC_SELECTNEXTTASK nSelect); 
	BOOL CanEditSelectedTask(TDC_ATTRIBUTE nAttrib, DWORD dwTaskID = 0) const;
	BOOL SplitSelectedTask(int nNumSubtasks);
	BOOL CanPasteTasks(TDC_PASTE nWhere, BOOL bAsRef) const;

	void SetTaskView(FTC_VIEW nView);
	void SetNextTaskView();
	FTC_VIEW GetTaskView() const { return m_tabViews.GetActiveView(); }
	CString GetTaskViewName() const;
	void ShowListViewTab(BOOL bVisible = TRUE);
	BOOL IsListViewTabShowing() const;
	void SaveAllTaskViewPreferences();
	void SetVisibleTaskViews(const CStringArray& aTypeIDs);
	int GetVisibleTaskViews(CStringArray& aTypeIDs) const;

	static BOOL IsExtensionView(FTC_VIEW nView);
	static void SetDefaultTaskViews(const CStringArray& aTypeIDs);
	
	BOOL SetTreeFont(HFONT hFont); // caller responsible for deleting
	BOOL SaveTaskViewToImage(const CString& sFilePath);
	BOOL CanSaveTaskViewToImage() const;

	TDC_HITTEST HitTest(const CPoint& ptScreen) const;
	TDC_COLUMN HitTestColumn(const CPoint& ptScreen) const;
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

	virtual HTREEITEM GetUpdateControlsItem() const;
	virtual CString FormatSelectedTaskTitles(BOOL bFullPath, TCHAR cSep = 0, int nMaxTasks = -1) const;

	int GetSortableColumns(CTDCColumnIDMap& mapColIDs) const;
	BOOL DeleteSelectedTask() { return CToDoCtrl::DeleteSelectedTask(); }
	void SetModified(BOOL bMod = TRUE) { CToDoCtrl::SetModified(bMod); }
	void SetMaximizeState(TDC_MAXSTATE nState);

	BOOL MoveSelectedTask(TDC_MOVETASK nDirection);
	BOOL CanMoveSelectedTask(TDC_MOVETASK nDirection) const;
	BOOL GotoNextTask(TDC_GOTO nDirection); 
	BOOL CanGotoNextTask(TDC_GOTO nDirection) const;
	BOOL GotoNextTopLevelTask(TDC_GOTO nDirection); 
	BOOL CanGotoNextTopLevelTask(TDC_GOTO nDirection) const;
	BOOL CanDoFindReplace(TDC_ATTRIBUTE nAttrib = TDCA_TASKNAME) const;
	void SelectAll();
	BOOL CanSelectAll() const;

	BOOL CanExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand) const;
	void ExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand = TRUE);
	
	BOOL CanCopyTaskColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly) const;
	BOOL CopyTaskColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly) const;

	void SetFocusToTasks();
	BOOL TasksHaveFocus() const;

	BOOL SelectTasksInHistory(BOOL bForward);
	BOOL GetSelectionBoundingRect(CRect& rSelection) const;

	void SetUITheme(const CUIThemeFile& theme);
	void ResizeAttributeColumnsToFit();
	BOOL CanResizeAttributeColumnsToFit() const;
	void RedrawReminders();
	void EndTimeTracking(BOOL bAllowConfirm) { CToDoCtrl::EndTimeTracking(bAllowConfirm); }
	void BeginTimeTracking(DWORD dwTaskID) { CToDoCtrl::BeginTimeTracking(dwTaskID); }
	void EndSelectedTaskEdit();

	virtual CString GetControlDescription(const CWnd* pCtrl) const;
	virtual void RebuildCustomAttributeUI();
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
	void SetGroupHeaderBackgroundColor(COLORREF color) { m_taskList.SetGroupHeaderBackgroundColor(color); }

protected:
	CTDLTaskListCtrl m_taskList;
	CTDLViewTabControl m_tabViews;
	CTaskListDropTarget m_dtList;
	CComboBox m_cbListGroupBy;
	CTDLTaskListCtrlOptionsComboBox m_cbListOptions;

	const CUIExtensionMgr& m_mgrUIExt;
	CArray<IUIExtensionWindow*, IUIExtensionWindow*> m_aExtViews;

	CTDCAttributeMap m_mapAttribsAffectedByPrefs;

	BOOL m_bUpdatingExtensions;
	BOOL m_bRecreatingRecurringTasks;
	BOOL m_bTreeNeedResort;
	BOOL m_bLoadingTasks;
	
	TDC_COLUMN m_nListViewGroupBy;
	TDC_ATTRIBUTE m_nExtModifyingAttrib;
	CDWordArray m_aRecreatedRecurringTasks;
	DWORD m_dwListOptions;

	static CStringArray s_aDefTaskViews;
	static UINT WM_TDC_RESTORELASTTASKVIEW;	// private message

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
	afx_msg void OnTreeExpandItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnListGroupBySelChanged();
	afx_msg void OnListOptionsCheckChanged();

	afx_msg LRESULT OnDropObject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCanDropObject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPreTabViewChange(WPARAM nOldView, LPARAM nNewView);
	afx_msg LRESULT OnPostTabViewChange(WPARAM nOldView, LPARAM nNewView);
	afx_msg LRESULT OnTDCGetTaskReminder(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnRecreateRecurringTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRestoreLastTaskView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMidnight(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnUIExtSelectTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtModifySelectedTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtMoveSelectedTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtEditSelectedTaskTitle(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtEditSelectedTaskIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtSortChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtDoHelp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtGetTaskIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtGetNextTaskOcurrences(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUIExtShowFileLink(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	// pseudo-handler
	void OnListSelChanged();

protected:
	const TDSORT& GetSort() const;

	virtual void SetModified(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs, BOOL bAllowResort);
	virtual void ReposTaskTree(CDeferWndMove* pDWM, const CRect& rPos);
	virtual DWORD SetStyle(TDC_STYLE nStyle, BOOL bOn);
	virtual void UpdateTasklistVisibility();
	virtual void UpdateVisibleColumns(const CTDCColumnIDMap& mapChanges);
	virtual void EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify);
	virtual BOOL BeginTimeTracking(DWORD dwTaskID, BOOL bNotify);
	virtual DWORD GetNextNonSelectedTaskID() const;
	virtual BOOL SelectNextTask(const CString& sPart, TDC_SELECTNEXTTASK nSelect, TDC_ATTRIBUTE nAttrib, 
							BOOL bCaseSensitive, BOOL bWholeWord, BOOL bFindReplace);
	virtual int CopyTaskColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly, CStringArray& aValues) const;

	virtual BOOL LoadTasks(const CTaskFile& tasks);
	virtual void SaveTasksState(CPreferences& prefs, BOOL bRebuildTree = FALSE) const; 
	virtual HTREEITEM LoadTasksState(const CPreferences& prefs, BOOL bRebuildTree = FALSE);

	virtual void OnStylesUpdated(const CTDCStyleMap& styles);
	virtual void OnTaskIconsChanged();
	virtual DWORD HitTestTask(const CPoint& ptScreen, BOOL bTitleColumnOnly) const;

	virtual void RebuildList(BOOL bChangeGroup = FALSE, TDC_COLUMN nNewGroupBy = TDCC_NONE, const void* pContext = NULL);
	virtual BOOL WantAddTreeTaskToList(DWORD dwTaskID, const void* pContext) const;
	virtual BOOL GetLabelEditRect(CRect& rScreen); // screen coords

	void UpdateSelectedTaskPath();
	void InvalidateItem(HTREEITEM hti, BOOL bUpdate);
	int FindListTask(const CString& sPart, TDC_ATTRIBUTE nAttrib, int nStart, BOOL bNext, BOOL bCaseSensitive, BOOL bWholeWord) const;
	void SetEditTitleTaskID(DWORD dwTaskID);
	void LoadState();
	void SaveState();
	BOOL ModAffectsAggregatedAttributes(TDC_ATTRIBUTE nAttrib) const;
	BOOL ModAffectsAggregatedAttributes(const CTDCAttributeMap& mapAttribIDs) const;
	BOOL WantUpdateInheritedAttibutes(const CTDCAttributeMap& mapAttribIDs) const;
	void UpdateListView(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs, BOOL bAllowResort);
	void UpdateSortStates(const CTDCAttributeMap& mapAttribIDs, BOOL bAllowResort);
	void BuildListGroupByCombo();
	void BuildListOptionsCombo();
	void ResortList(BOOL bAllowToggle = FALSE);
	BOOL HasListOption(DWORD dwOption) const { return ((m_dwListOptions & dwOption) == dwOption); }

	void SyncActiveViewSelectionToTree();
	void SyncListSelectionToTree(BOOL bEnsureSelection);
	void SyncExtensionSelectionToTree(FTC_VIEW nView);
	BOOL HasSingleSelectionChanged(DWORD dwSelID) const;
	DWORD GetSingleSelectedTaskID() const;
	int CacheListSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs = TRUE) const;

	VIEWDATA* GetActiveViewData() const;
	VIEWDATA* GetViewData(FTC_VIEW nView) const;
	
	BOOL AddView(IUIExtension* pExtension);
	BOOL RemoveView(IUIExtension* pExtension);

	HTREEITEM GetTreeItem(int nItem) const;
	int GetListItem(HTREEITEM hti) const;
	DWORD GetTaskID(int nItem) const { return ((nItem == -1) ? 0 : m_taskList.List().GetItemData(nItem)); }
	DWORD GetTaskID(HTREEITEM hti) const { return CToDoCtrl::GetTaskID(hti); }
	TODOITEM* CreateNewTask(HTREEITEM htiParent);

	DWORD GetNextTaskID(DWORD dwTaskID, TTC_NEXTTASK nNext, BOOL bExcludeSelected) const;

	BOOL InListView() const;
	BOOL InTreeView() const;
	BOOL InExtensionView() const;
	BOOL IsViewSet() const;
	BOOL ViewSupportsTaskSelection(FTC_VIEW nView) const;
	BOOL ViewSupportsNewTask(FTC_VIEW nView) const;
	BOOL ViewHasTaskSelection(FTC_VIEW nView) const;
	void SetViewNeedsTaskUpdate(FTC_VIEW nView, BOOL bUpdate = TRUE);

	void UpdateExtensionViews(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs);
	BOOL ExtensionDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd);
	BOOL ExtensionCanDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd) const;
	BOOL ExtensionDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA& data);
	BOOL ExtensionCanDoAppCommand(FTC_VIEW nView, IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA& data) const;
	IUIExtensionWindow* GetCreateExtensionWnd(FTC_VIEW nView);
	IUIExtensionWindow* GetExtensionWnd(FTC_VIEW nView) const;
	BOOL GetExtensionWnd(FTC_VIEW nView, IUIExtensionWindow*& pExtWnd, VIEWDATA*& pData) const;
	BOOL HasAnyExtensionViews() const;
	BOOL AnyExtensionViewWantsChange(TDC_ATTRIBUTE nAttrib) const;
	BOOL AnyExtensionViewWantsChanges(const CTDCAttributeMap& mapAttribIDs) const;
	BOOL GetExtensionViewsWantedChanges(const CTDCAttributeMap& mapAttribIDs, CTDCAttributeMap& mapAttribsWanted) const;
	BOOL ExtensionViewWantsChanges(int nExt, const CTDCAttributeMap& mapAttribIDs) const;
	BOOL GetExtensionViewWantedChanges(int nExt, const CTDCAttributeMap& mapAttribIDs, CTDCAttributeMap& mapAttribsWanted) const;
	BOOL ExtensionViewWantsChange(int nExt, TDC_ATTRIBUTE nAttrib) const;
	BOOL AllExtensionViewsNeedFullUpdate() const;
	void BeginExtensionProgress(const VIEWDATA* pData, UINT nMsg = 0);
	void EndExtensionProgress();
	void UpdateExtensionView(IUIExtensionWindow* pExtWnd, const CTaskFile& tasks, IUI_UPDATETYPE nType);
	void SetExtensionsReadOnly(BOOL bReadOnly);
	void SetExtensionsNeedTaskUpdate(BOOL bUpdate, FTC_VIEW nIgnore = FTCV_UNSET);
	void SetExtensionsNeedFontUpdate(BOOL bUpdate, FTC_VIEW nIgnore = FTCV_UNSET);
	void SetListViewNeedFontUpdate(BOOL bUpdate);
	BOOL ProcessUIExtensionMod(const IUITASKMOD& mod, CDWordArray& aModTaskIDs, CTDCTaskCompletionArray& aTasksForCompletion, CTDCAttributeMap& mapModAttribs);
	int GetAllExtensionViewsWantedAttributes(CTDCAttributeMap& mapAttribIDs) const;
	CString GetExtensionPrefsSubKey(const IUIExtensionWindow* pExtWnd);
	void UpdateExtensionViewsSelection(const CTDCAttributeMap& mapAttribIDs);
	void UpdateExtensionViewsTasks(const CTDCAttributeMap& mapAttribIDs);
	void UpdateExtensionViewsProjectName();
	BOOL IsExtensionView(HWND hWnd) const;
	BOOL ExtensionMoveSelectedTaskStartAndDueDates(const COleDateTime& dtNewStart);
	void RefreshExtensionViewSort(FTC_VIEW nView);
	BOOL ExtensionCanSortBy(FTC_VIEW nView, TDC_ATTRIBUTE nBy) const;
	BOOL GetExtensionInsertLocation(FTC_VIEW nView, TDC_MOVETASK nDirection, DWORD& dwDestParentID, DWORD& dwDestPrevSiblingID) const;
	BOOL AttributeMatchesExtensionMod(TDC_ATTRIBUTE nAttrib) const;
	virtual BOOL GetAllTasksForExtensionViewUpdate(const CTDCAttributeMap& mapAttribIDs, CTaskFile& tasks) const;
	BOOL GetSelectedTasksForExtensionViewUpdate(const CTDCAttributeMap& mapAttribIDs, DWORD dwFlags, CTaskFile& tasks) const;
	
	int GetTasks(CTaskFile& tasks, FTC_VIEW nView, const TDCGETTASKS& filter) const;
	int GetSelectedTasks(CTaskFile& tasks, FTC_VIEW nView, const TDCGETTASKS& filter) const;
	BOOL CanEditSelectedTask(const IUITASKMOD& mod, DWORD& dwTaskID) const;
	BOOL ValidatePreviousSiblingTaskID(DWORD dwTaskID, DWORD& dwPrevSiblingID) const;

	BOOL AddTreeChildrenToTaskFile(HTREEITEM hti, CTaskFile& tasks, HTASKITEM hTask, const TDCGETTASKS& filter) const;
	HTASKITEM AddTreeItemToTaskFile(HTREEITEM hti, CTaskFile& tasks, HTASKITEM hParentTask, HTASKITEM hPrevSiblingTask, const TDCGETTASKS& filter) const;
	void AddGlobalsToTaskFile(CTaskFile& tasks, const CTDCAttributeMap& mapAttribIDs) const;
	void ShowListViewSpecificCtrls(BOOL bShow);

	virtual VIEWDATA* NewViewData() { return new VIEWDATA(); }

	static FTC_VIEW GetExtensionView(int nExt) { return (FTC_VIEW)(nExt + FTCV_FIRSTUIEXTENSION); }
	static int PopulateExtensionViewAttributes(const IUIExtensionWindow* pExtWnd, VIEWDATA* pData);
	static IUI_APPCOMMAND MapGetNextToCommand(TTC_NEXTTASK nNext);
	static TTC_NEXTTASK MapGotoToGetNext(TDC_GOTO nDirection, BOOL bTopLevel);

};

#endif // !defined(AFX_TABBEDTODOCTRL_H__356A6EB9_C7EC_4395_8716_623AFF4A269B__INCLUDED_)
