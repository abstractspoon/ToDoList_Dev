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

//#include "..\shared\misc.h"

/////////////////////////////////////////////////////////////////////////////

struct VIEWDATA2 : public VIEWDATA
{
	VIEWDATA2() : bNeedRefilter(TRUE) {}
	virtual ~VIEWDATA2() {}

	BOOL bNeedRefilter;
};

/////////////////////////////////////////////////////////////////////////////

class CFilteredToDoCtrl : public CTabbedToDoCtrl  
{
public:
	CFilteredToDoCtrl(CUIExtensionMgr& mgrUIExt, 
						CContentMgr& mgrContent, 
						const CONTENTFORMAT& cfDefault,
						const TDCCOLEDITFILTERVISIBILITY& visDefault);
	
	virtual ~CFilteredToDoCtrl();

	BOOL DelayLoad(const CString& sFilePath, COleDateTime& dtEarliestDue);

	BOOL CreateNewTask(LPCTSTR szText, TDC_INSERTWHERE nWhere = TDC_INSERTATTOPOFSELTASKPARENT, 
						BOOL bEditText = TRUE, DWORD dwDependency = 0); 
	BOOL CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const;

	BOOL ArchiveDoneTasks(TDC_ARCHIVE nFlags, BOOL bRemoveFlagged); // returns true if any tasks were removed
	BOOL ArchiveSelectedTasks(BOOL bRemove); // returns true if any tasks were removed

	FILTER_SHOW GetFilter(CTDCFilter& filter) const { filter = m_filter; }
	void RefreshFilter();
	void ClearFilter();
	void ToggleFilter();
	BOOL HasAnyFilter() const { return m_filter.HasAnyFilter(); }
	BOOL CanToggleFilter() const { return m_filter.CanToggleFilter(); }

	FILTER_SHOW GetFilter(FTDCFILTER& filter) const;
	void SetFilter(const FTDCFILTER& filter);
	BOOL FilterMatches(const FTDCFILTER& filter, LPCTSTR szCustom = NULL, DWORD dwCustomFlags = 0, DWORD dwIgnoreFlags = 0) const;

	BOOL HasCustomFilter() const;
	BOOL SetCustomFilter(const FTDCCUSTOMFILTER& filter);
	CString GetCustomFilterName() const;
	DWORD GetCustomFilterFlags() const;

	UINT GetTaskCount(UINT* pVisible = 0) const;
	int GetFilteredTasks(CTaskFile& tasks, const TDCGETTASKS& filter = TDCGT_ALL) const;
	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const;

	BOOL SplitSelectedTask(int nNumSubtasks);
	BOOL SetStyles(const CTDCStylesMap& styles);
	void Sort(TDC_COLUMN nBy, BOOL bAllowToggle = TRUE);
	void SetModified(BOOL bMod = TRUE) { CTabbedToDoCtrl::SetModified(bMod); }
	BOOL SetStyle(TDC_STYLE nStyle, BOOL bOn = TRUE) { return CTabbedToDoCtrl::SetStyle(nStyle, bOn); }

	int GetArchivableTasks(CTaskFile& tasks, BOOL bSelectedOnly) const;
	void EndTimeTracking(BOOL bAllowConfirm) { CTabbedToDoCtrl::EndTimeTracking(bAllowConfirm); }

protected:
	enum // visible for derived classes
	{ 
		TIMER_NOWFILTER = TIMER_LAST, 
	};
	
	CTDCFilter m_filter;
	CDWordArray m_aFilterSelectedTaskIDs; // for FT_SELECTED filter
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
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnTreeExpandItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnRefreshFilter(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditChangeDueTime();
	afx_msg LRESULT OnPreTabViewChange(WPARAM nOldView, LPARAM nNewView);

	DECLARE_MESSAGE_MAP()

	// pseudo-message handlers
	void OnTimerNow();
	void OnTimerMidnight();

protected:
	BOOL ModNeedsRefilter(TDC_ATTRIBUTE nModType, FTC_VIEW nView, DWORD dwModTaskID) const;

	virtual void SetModified(BOOL bMod, TDC_ATTRIBUTE nAttrib, DWORD dwModTaskID);
	virtual BOOL SetStyle(TDC_STYLE nStyle, BOOL bOn, BOOL bWantUpdate); // one style at a time only 

	virtual BOOL LoadTasks(const CTaskFile& file);
	virtual BOOL RemoveArchivedTask(DWORD dwTaskID);
	virtual BOOL CopyCurrentSelection() const;
	virtual void EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify);
	virtual BOOL GetAllTasksForExtensionViewUpdate(CTaskFile& tasks, const CTDCAttributeMap& mapAttrib) const;

	void GetCompletedTasks(const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTaskParent, BOOL bSelectedOnly = FALSE) const;

	void SaveSettings() const;
	void LoadSettings();

	void RefreshListFilter();
	void RefreshTreeFilter();
	void RefreshExtensionFilter(FTC_VIEW nView, BOOL bShowProgress = FALSE);
	BOOL IsFilterSet(FTC_VIEW nView) const;
	BOOL IsFilterSet(const FTDCFILTER& filter, FTC_VIEW nView) const;
	BOOL FiltersMatch(const FTDCFILTER& filter1, const FTDCFILTER& filter2, FTC_VIEW nView) const;
	BOOL FilterMatches(const FTDCFILTER& filter, FTC_VIEW nView) const;
	void SetExtensionsNeedRefilter(BOOL bRefilter, FTC_VIEW nIgnore = FTCV_UNSET);
	void SetListNeedRefilter(BOOL bRefilter);

	void ResetNowFilterTimer();
	BOOL FindNewNowFilterTasks(const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& params, const CHTIMap& htiMap) const;

	VIEWDATA2* GetActiveViewData2() const;
	VIEWDATA2* GetViewData2(FTC_VIEW nView) const;

	HTREEITEM RebuildTree(const void* pContext = NULL);
	BOOL WantAddTask(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const void* pContext) const; 

	void RebuildList(const void* pContext); 
	void RebuildList(const SEARCHPARAMS& filter);
	virtual void AddTreeItemToList(HTREEITEM hti, const void* pContext);

	virtual VIEWDATA* NewViewData() { return new VIEWDATA2(); }
};

#endif // !defined(AFX_FILTEREDTODOCTRL_H__356A6EB9_C7EC_4395_8716_623AFF4A269B__INCLUDED_)
