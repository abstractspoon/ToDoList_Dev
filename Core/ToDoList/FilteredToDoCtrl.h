// FilteredToDoCtrl.h: interface for the CFilteredToDoCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILTEREDTODOCTRL_H__356A6EB9_C7EC_4395_8716_623AFF4A269B__INCLUDED_)
#define AFX_FILTEREDTODOCTRL_H__356A6EB9_C7EC_4395_8716_623AFF4A269B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabbedToDoCtrl.h"
#include "TDCFilter.h"

/////////////////////////////////////////////////////////////////////////////

class CFilteredToDoCtrl : public CTabbedToDoCtrl  
{
public:
	CFilteredToDoCtrl(CUIExtensionMgr& mgrUIExt, 
					  CTDLContentMgr& mgrContent, 
					  CShortcutManager& mgrShortcuts,
					  const CONTENTFORMAT& cfDefault,
					  const TDCCOLEDITFILTERVISIBILITY& visDefault);
	
	virtual ~CFilteredToDoCtrl();

	BOOL CreateNewTask(LPCTSTR szText, TDC_INSERTWHERE nWhere = TDC_INSERTATTOPOFSELTASKPARENT, 
						BOOL bEditText = TRUE, DWORD dwDependency = 0); 
	BOOL CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const;

	BOOL ArchiveDoneTasks(TDC_ARCHIVE nFlags, BOOL bRemoveFlagged); // returns true if any tasks were removed
	BOOL ArchiveSelectedTasks(BOOL bRemove); // returns true if any tasks were removed

	virtual BOOL SelectTask(DWORD dwTaskID, BOOL bTaskLink);
	BOOL SelectNextTask(CString sPart, TDC_SELECTNEXTTASK nSelect);
	BOOL SelectTasksInHistory(BOOL bForward);

	FILTER_SHOW GetFilter(CTDCFilter& filter) const { filter = m_filter; }
	void RefreshFilter();
	void ClearFilter();
	void ToggleFilter();
	BOOL HasAnyFilter() const { return m_filter.HasAnyFilter(); }
	BOOL CanToggleFilter() const { return m_filter.CanToggleFilter(); }

	void SetColumnFieldVisibility(const TDCCOLEDITFILTERVISIBILITY& vis);
	void GetColumnFieldVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const;
	const CTDCColumnIDMap& GetVisibleColumns() const;
	const CTDCAttributeMap& GetVisibleEditFields() const;
	const CTDCAttributeMap& GetVisibleFilterFields() const;

	FILTER_SHOW GetFilter(TDCFILTER& filter) const;
	void SetFilter(const TDCFILTER& filter);
	BOOL FilterMatches(const TDCFILTER& filter, LPCTSTR szCustom = NULL, DWORD dwCustomFlags = 0) const;

	BOOL HasAdvancedFilter() const;
	BOOL SetAdvancedFilter(const TDCADVANCEDFILTER& filter);
	CString GetAdvancedFilterName() const;
	DWORD GetAdvancedFilterFlags() const;

	UINT GetTaskCount(UINT* pVisible = 0) const;
	int GetFilteredTasks(CTaskFile& tasks, const TDCGETTASKS& filter = TDCGT_ALL) const;
	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const;

	void Sort(TDC_COLUMN nBy, BOOL bAllowToggle = TRUE);
	void SetModified(BOOL bMod = TRUE) { CTabbedToDoCtrl::SetModified(bMod); }
	void SetDueTaskColors(COLORREF crDue, COLORREF crDueToday);

	int GetArchivableTasks(CTaskFile& tasks, BOOL bSelectedOnly) const;
	void EndTimeTracking(BOOL bAllowConfirm) { CTabbedToDoCtrl::EndTimeTracking(bAllowConfirm); }

protected:
	enum // visible for derived classes
	{ 
		TIMER_NOWFILTER = TIMER_LAST, 
	};
	
	CTDCFilter m_filter;
	TDCCOLEDITFILTERVISIBILITY m_visColEditFilter;
	CDWordArray m_aSelectedTaskIDsForFiltering; // for FT_SELECTED filter
	BOOL m_bIgnoreListRebuild;
	BOOL m_bIgnoreExtensionUpdate;

// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilteredToDoCtrl)
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();

protected:
	// Generated message map functions
	//{{AFX_MSG(CFilteredToDoCtrl)
		//}}AFX_MSG
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEditChangeDueTime();
	afx_msg LRESULT OnMidnight(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	// pseudo-message handler
	void OnTimerNow();

protected:
	BOOL ModsNeedRefilter(const CTDCAttributeMap& attribIDs, const CDWordArray& aModTaskIDs) const;
	BOOL ModNeedsRefilter(TDC_ATTRIBUTE nAttrib, const CDWordArray& aModTaskIDs) const;
	BOOL StyleChangesNeedRefilter(const CTDCStyleMap& styles) const;

	virtual void SetModified(const CTDCAttributeMap& attribIDs, const CDWordArray& aModTaskIDs, BOOL bAllowResort);
	virtual BOOL LoadTasks(const CTaskFile& tasks);
	virtual BOOL RemoveArchivedTask(DWORD dwTaskID);
	virtual BOOL CopySelectedTasks() const;
	virtual void EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify);
	virtual BOOL GetAllTasksForExtensionViewUpdate(const CTDCAttributeMap& mapAttrib, CTaskFile& tasks) const;
	virtual void RebuildList(BOOL bChangeGroup = FALSE, TDC_COLUMN nNewGroupBy = TDCC_NONE, const void* pContext = NULL);
	virtual BOOL WantAddTreeTaskToList(DWORD dwTaskID, const void* pContext) const;
	virtual DWORD RecreateRecurringTaskInTree(const CTaskFile& task, const COleDateTime& dtNext, BOOL bDueDate);
	virtual DWORD MergeNewTaskIntoTree(const CTaskFile& tasks, HTASKITEM hTask, DWORD dwParentTaskID, BOOL bAndSubtasks);
	virtual void OnStylesUpdated(const CTDCStyleMap& styles);
	virtual void LoadAttributeVisibility(const CTaskFile& tasks, const CPreferences& prefs);
	virtual void SaveAttributeVisibility(CTaskFile& tasks) const;
	virtual void SaveAttributeVisibility(CPreferences& prefs) const;
	virtual int GetAllTaskIDs(CDWordArray& aTaskIDs, BOOL bIncParents, BOOL bInCollapsedChildren) const;

	void SaveState() const;
	void LoadState();

	BOOL RefreshFilter(BOOL bExplicit);
	BOOL RefreshTreeFilter();
	void RefreshExtensionFilter(FTC_VIEW nView, BOOL bShowProgress = FALSE);

	void ResetNowFilterTimer();
	BOOL FindNewNowFilterTasks(const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& params, const CHTIMap& htiMap) const;

	HTREEITEM RebuildTree(const void* pContext = NULL);
	BOOL WantAddTaskToTree(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const void* pContext) const; 
	
	// Helper allowing insert position to be specified
	DWORD MergeNewTaskIntoTree(const CTaskFile& tasks, HTASKITEM hTask, DWORD dwParentTaskID, DWORD dwPrevSiblingID, BOOL bAndSubtasks);
};

#endif // !defined(AFX_FILTEREDTODOCTRL_H__356A6EB9_C7EC_4395_8716_623AFF4A269B__INCLUDED_)
