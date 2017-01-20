// Fi M_BlISlteredToDoCtrl.cpp: implementation of the CFilteredToDoCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilteredToDoCtrl.h"
#include "todoitem.h"
#include "resource.h"
#include "tdcstatic.h"
#include "tdcmsg.h"
#include "TDCCustomAttributeHelper.h"
#include "TDCSearchParamHelper.h"
#include "taskclipboard.h"

#include "..\shared\holdredraw.h"
#include "..\shared\datehelper.h"
#include "..\shared\enstring.h"
#include "..\shared\preferences.h"
#include "..\shared\deferwndmove.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\osversion.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\savefocus.h"
#include "..\shared\filemisc.h"

#include "..\Interfaces\IUIExtension.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER 0x00010000
#endif

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP     0x00004000
#endif

//////////////////////////////////////////////////////////////////////

const UINT SORTWIDTH = 10;

#ifdef _DEBUG
const UINT ONE_MINUTE = 10000;
#else
const UINT ONE_MINUTE = 60000;
#endif

const UINT TEN_MINUTES = (ONE_MINUTE * 10);

//////////////////////////////////////////////////////////////////////

const UINT WM_TDC_REFRESHFILTER	= (WM_APP + 11);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilteredToDoCtrl::CFilteredToDoCtrl(CUIExtensionMgr& mgrUIExt, CContentMgr& mgrContent, 
									 const CONTENTFORMAT& cfDefault, const TDCCOLEDITFILTERVISIBILITY& visDefault) 
	:
	CTabbedToDoCtrl(mgrUIExt, mgrContent, cfDefault, visDefault),
	m_bIgnoreListRebuild(FALSE),
	m_bIgnoreExtensionUpdate(FALSE)
{
}

CFilteredToDoCtrl::~CFilteredToDoCtrl()
{

}

BEGIN_MESSAGE_MAP(CFilteredToDoCtrl, CTabbedToDoCtrl)
//{{AFX_MSG_MAP(CFilteredToDoCtrl)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_TDCN_VIEWPRECHANGE, OnPreTabViewChange)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TASKTREELIST, OnTreeExpandItem)
	ON_MESSAGE(WM_TDC_REFRESHFILTER, OnRefreshFilter)
	ON_CBN_EDITCHANGE(IDC_DUETIME, OnEditChangeDueTime)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////

BOOL CFilteredToDoCtrl::OnInitDialog()
{
	CTabbedToDoCtrl::OnInitDialog();

	return FALSE;
}

BOOL CFilteredToDoCtrl::LoadTasks(const CTaskFile& file)
{
	// handle reloading of tasklist with a filter present
	if (GetTaskCount() && m_filter.HasAnyFilter())
	{
		SaveSettings();
	}

	BOOL bViewWasSet = IsViewSet();

	if (!CTabbedToDoCtrl::LoadTasks(file))
		return FALSE;

	FTC_VIEW nView = GetView();

	// save visible state
	BOOL bHidden = !IsWindowVisible();

	// reload last view
	if (!bViewWasSet)
	{
		LoadSettings();

		// always refresh the tree filter because all other
		// views depend on it
		if (IsFilterSet(FTCV_TASKTREE))
			RefreshTreeFilter(); // always

		// handle other views
		switch (nView)
		{
		case FTCV_TASKLIST:
			if (IsFilterSet(nView))
			{
				RefreshListFilter();
			}
			else if (!GetPreferencesKey().IsEmpty()) // first time
			{
				GetViewData2(nView)->bNeedRefilter = TRUE;
			}
			break;

		case FTCV_UIEXTENSION1:
		case FTCV_UIEXTENSION2:
		case FTCV_UIEXTENSION3:
		case FTCV_UIEXTENSION4:
		case FTCV_UIEXTENSION5:
		case FTCV_UIEXTENSION6:
		case FTCV_UIEXTENSION7:
		case FTCV_UIEXTENSION8:
		case FTCV_UIEXTENSION9:
		case FTCV_UIEXTENSION10:
		case FTCV_UIEXTENSION11:
		case FTCV_UIEXTENSION12:
		case FTCV_UIEXTENSION13:
		case FTCV_UIEXTENSION14:
		case FTCV_UIEXTENSION15:
		case FTCV_UIEXTENSION16:
			// Note: By way of virtual functions CTabbedToDoCtrl::LoadTasks
			// will already have initialized the active view if it is an
			// extension so we only need to update if the tree actually
			// has a filter
			if (IsFilterSet(FTCV_TASKTREE))
				RefreshExtensionFilter(nView);
			break;
		}
	}
	else if (IsFilterSet(nView))
	{
		RefreshFilter();
	}

	// restore previously visibility
	if (bHidden)
		ShowWindow(SW_HIDE);

	return TRUE;
}

BOOL CFilteredToDoCtrl::DelayLoad(const CString& sFilePath, COleDateTime& dtEarliestDue)
{
	if (CTabbedToDoCtrl::DelayLoad(sFilePath, dtEarliestDue))
	{
		LoadSettings();
		return TRUE;
	}
	
	// else
	return FALSE;
}

void CFilteredToDoCtrl::SaveSettings() const
{
	CPreferences prefs;
	m_filter.SaveFilter(prefs, GetPreferencesKey(_T("Filter")));
}

void CFilteredToDoCtrl::LoadSettings()
{
	if (HasStyle(TDCS_RESTOREFILTERS))
	{
		CPreferences prefs;
		m_filter.LoadFilter(prefs, GetPreferencesKey(_T("Filter")), m_aCustomAttribDefs);
	}
}

void CFilteredToDoCtrl::OnDestroy() 
{
	SaveSettings();

	CTabbedToDoCtrl::OnDestroy();
}

void CFilteredToDoCtrl::OnEditChangeDueTime()
{
	// need some special hackery to prevent a re-filter in the middle
	// of the user manually typing into the time field
	BOOL bNeedsRefilter = ModNeedsRefilter(TDCA_DUEDATE, FTCV_TASKTREE, GetSelectedTaskID());
	
	if (bNeedsRefilter)
		SetStyle(TDCS_REFILTERONMODIFY, FALSE, FALSE);
	
	CTabbedToDoCtrl::OnSelChangeDueTime();
	
	if (bNeedsRefilter)
		SetStyle(TDCS_REFILTERONMODIFY, TRUE, FALSE);
}

void CFilteredToDoCtrl::OnTreeExpandItem(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	if (m_filter.HasFilterFlag(FO_HIDECOLLAPSED))
	{
		if (InListView())
			RefreshListFilter();
		else
			GetViewData2(FTCV_TASKLIST)->bNeedRefilter = TRUE;
	}
}

LRESULT CFilteredToDoCtrl::OnPreTabViewChange(WPARAM nOldView, LPARAM nNewView) 
{
	if (nNewView != FTCV_TASKTREE)
	{
		VIEWDATA2* pData = GetViewData2((FTC_VIEW)nNewView);
		BOOL bFiltered = FALSE;

		// take a note of what task is currently singly selected
		// so that we can prevent unnecessary calls to UpdateControls
		DWORD dwSelTaskID = GetSingleSelectedTaskID();

		switch (nNewView)
		{
		case FTCV_TASKLIST:
			// update filter as required
			if (pData->bNeedRefilter)
			{
				bFiltered = TRUE;
				RefreshListFilter();
			}
			break;

		case FTCV_UIEXTENSION1:
		case FTCV_UIEXTENSION2:
		case FTCV_UIEXTENSION3:
		case FTCV_UIEXTENSION4:
		case FTCV_UIEXTENSION5:
		case FTCV_UIEXTENSION6:
		case FTCV_UIEXTENSION7:
		case FTCV_UIEXTENSION8:
		case FTCV_UIEXTENSION9:
		case FTCV_UIEXTENSION10:
		case FTCV_UIEXTENSION11:
		case FTCV_UIEXTENSION12:
		case FTCV_UIEXTENSION13:
		case FTCV_UIEXTENSION14:
		case FTCV_UIEXTENSION15:
		case FTCV_UIEXTENSION16:
			// update filter as required
			if (pData && pData->bNeedRefilter)
			{
				// initialise progress depending on whether extension
				// window is already created
				UINT nProgressMsg = 0;

				if (GetExtensionWnd((FTC_VIEW)nNewView) == NULL)
					nProgressMsg = IDS_INITIALISINGTABBEDVIEW;

				BeginExtensionProgress(pData, nProgressMsg);
				RefreshExtensionFilter((FTC_VIEW)nNewView);

				bFiltered = TRUE;
			}
			break;
		}

		if (bFiltered)
			pData->bNeedFullTaskUpdate = FALSE;
		
		// update controls only if the selection has changed and 
		// we didn't refilter (RefreshFilter will already have called UpdateControls)
		BOOL bSelChange = HasSingleSelectionChanged(dwSelTaskID);
		
		if (bSelChange && !bFiltered)
			UpdateControls();
	}

	return CTabbedToDoCtrl::OnPreTabViewChange(nOldView, nNewView);
}


BOOL CFilteredToDoCtrl::CopyCurrentSelection() const
{
	// NOTE: we are overriding this function else
	// filtered out subtasks will not get copied

	// NOTE: We DON'T override GetSelectedTasks because
	// most often that only wants visible tasks

	if (!GetSelectedCount())
		return FALSE;
	
	ClearCopiedItem();
	
	TDCGETTASKS filter(TDCGT_ALL, TDCGTF_FILENAME);
	CTaskFile tasks;

	PrepareTaskfileForTasks(tasks, filter);
	
	// get selected tasks ordered, removing duplicate subtasks
	CHTIList selection;
	TSH().CopySelection(selection, TRUE, TRUE);
	
	// copy items
	POSITION pos = selection.GetHeadPosition();
	
	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		DWORD dwTaskID = GetTrueTaskID(hti);

		const TODOSTRUCTURE* pTDS = m_data.LocateTask(dwTaskID);
		const TODOITEM* pTDI = GetTask(dwTaskID);

		if (!pTDS || !pTDI)
			return FALSE;

		// add task
		HTASKITEM hTask = tasks.NewTask(pTDI->sTitle, NULL, dwTaskID);
		ASSERT(hTask);
		
		if (!hTask)
			return FALSE;
		
		SetTaskAttributes(pTDI, pTDS, tasks, hTask, TDCGT_ALL, FALSE);

		// and subtasks
		AddSubTasksToTaskFile(pTDS, tasks, hTask, TRUE);
	}
	
	// extra processing to identify the originally selected tasks
	// in case the user wants to paste as references
	pos = TSH().GetFirstItemPos();
	
	while (pos)
	{
		DWORD dwSelID = TSH().GetNextItemData(pos);
		ASSERT(dwSelID);
		
		HTASKITEM hSelTask = tasks.FindTask(dwSelID);
		ASSERT(hSelTask);
		
		tasks.SetTaskMetaData(hSelTask, _T("selected"), _T("1"));
	}
	
	// and their titles (including child dupes)
	CStringArray aTitles;
	
	VERIFY(TSH().CopySelection(selection, FALSE, TRUE));
	VERIFY(TSH().GetItemTitles(selection, aTitles));
	
	return CTaskClipboard::SetTasks(tasks, GetClipboardID(), Misc::FormatArray(aTitles, '\n'));
}

BOOL CFilteredToDoCtrl::ArchiveDoneTasks(TDC_ARCHIVE nFlags, BOOL bRemoveFlagged)
{
	if (CTabbedToDoCtrl::ArchiveDoneTasks(nFlags, bRemoveFlagged))
	{
		if (InListView())
		{
			if (IsFilterSet(FTCV_TASKLIST))
				RefreshListFilter();
		}
		else if (IsFilterSet(FTCV_TASKTREE))
		{
			RefreshTreeFilter();
		}

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CFilteredToDoCtrl::ArchiveSelectedTasks(BOOL bRemove)
{
	if (CTabbedToDoCtrl::ArchiveSelectedTasks(bRemove))
	{
		if (InListView())
		{
			if (IsFilterSet(FTCV_TASKLIST))
				RefreshListFilter();
		}
		else if (IsFilterSet(FTCV_TASKTREE))
		{
			RefreshTreeFilter();
		}

		return TRUE;
	}

	// else
	return FALSE;
}

int CFilteredToDoCtrl::GetArchivableTasks(CTaskFile& tasks, BOOL bSelectedOnly) const
{
	if (bSelectedOnly || !IsFilterSet(FTCV_TASKTREE))
		return CTabbedToDoCtrl::GetArchivableTasks(tasks, bSelectedOnly);

	// else process the entire data hierarchy
	GetCompletedTasks(m_data.GetStructure(), tasks, NULL, FALSE);

	return tasks.GetTaskCount();
}

BOOL CFilteredToDoCtrl::RemoveArchivedTask(DWORD dwTaskID)
{
	ASSERT(m_data.HasTask(dwTaskID));
	
	// note: if the tasks does not exist in the tree then this is not a bug
	// if a filter is set
	HTREEITEM hti = m_taskTree.GetItem(dwTaskID);
	ASSERT(hti || IsFilterSet(FTCV_TASKTREE));
	
	if (!hti && !IsFilterSet(FTCV_TASKTREE))
		return FALSE;
	
	if (hti)
		m_taskTree.Tree().DeleteItem(hti);

	return m_data.DeleteTask(dwTaskID);
}

void CFilteredToDoCtrl::GetCompletedTasks(const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTaskParent, BOOL bSelectedOnly) const
{
	const TODOITEM* pTDI = NULL;

	if (!pTDS->IsRoot())
	{
		DWORD dwTaskID = pTDS->GetTaskID();

		pTDI = m_data.GetTask(dwTaskID);
		ASSERT(pTDI);

		if (!pTDI)
			return;

		// we add the task if it is completed (and optionally selected) or it has children
		if (pTDI->IsDone() || pTDS->HasSubTasks())
		{
			HTASKITEM hTask = tasks.NewTask(_T(""), hTaskParent, dwTaskID);
			ASSERT(hTask);

			// copy attributes
			TDCGETTASKS allTasks;
			SetTaskAttributes(pTDI, pTDS, tasks, hTask, allTasks, FALSE);

			// this task is now the new parent
			hTaskParent = hTask;
		}
	}

	// children
	if (pTDS->HasSubTasks())
	{
		for (int nSubtask = 0; nSubtask < pTDS->GetSubTaskCount(); nSubtask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubtask);
			GetCompletedTasks(pTDSChild, tasks, hTaskParent, bSelectedOnly); // RECURSIVE call
		}

		// if no subtasks were added and the parent is not completed 
		// (and optionally selected) then we remove it
		if (hTaskParent && tasks.GetFirstTask(hTaskParent) == NULL)
		{
			ASSERT(pTDI);

			if (pTDI && !pTDI->IsDone())
				tasks.DeleteTask(hTaskParent);
		}
	}
}

int CFilteredToDoCtrl::GetFilteredTasks(CTaskFile& tasks, const TDCGETTASKS& filter) const
{
	// synonym for GetTasks which always returns the filtered tasks
	return GetTasks(tasks, GetView(), filter);
}

FILTER_SHOW CFilteredToDoCtrl::GetFilter(FTDCFILTER& filter) const
{
	return m_filter.GetFilter(filter);
}

void CFilteredToDoCtrl::SetFilter(const FTDCFILTER& filter)
{
	FTC_VIEW nView = GetView();

	if (m_bDelayLoaded)
	{
		m_filter.SetFilter(filter);

		// mark everything needing refilter
		GetViewData2(FTCV_TASKTREE)->bNeedRefilter = TRUE;
		SetListNeedRefilter(TRUE);
		SetExtensionsNeedRefilter(TRUE);
	}
	else
	{
		BOOL bTreeNeedsFilter = !FilterMatches(filter, FTCV_TASKTREE);
		BOOL bListNeedRefilter = !FilterMatches(filter, FTCV_TASKLIST); 

		m_filter.SetFilter(filter);

		if (bTreeNeedsFilter)
		{
			// this will mark all other views as needing refiltering
			// and refilter them if they are active
			RefreshFilter();
		}
		else if (bListNeedRefilter)
		{
			if (nView == FTCV_TASKLIST)
				RefreshListFilter();
			else
				SetListNeedRefilter(TRUE);
		}
	}

	ResetNowFilterTimer();
}
	
void CFilteredToDoCtrl::ClearFilter()
{
	if (m_filter.ClearFilter())
		RefreshFilter();

	ResetNowFilterTimer();
}

void CFilteredToDoCtrl::ToggleFilter()
{
	if (m_filter.ToggleFilter())
		RefreshFilter();

	ResetNowFilterTimer();
}

BOOL CFilteredToDoCtrl::FiltersMatch(const FTDCFILTER& filter1, const FTDCFILTER& filter2, FTC_VIEW nView) const
{
	if (nView == FTCV_UNSET)
		return FALSE;

	DWORD dwIgnore = 0;

	if (nView == FTCV_TASKTREE)
		dwIgnore = (FO_HIDECOLLAPSED | FO_HIDEPARENTS);

	return FTDCFILTER::FiltersMatch(filter1, filter2, dwIgnore);
}

BOOL CFilteredToDoCtrl::FilterMatches(const FTDCFILTER& filter, FTC_VIEW nView) const
{
	if (nView == FTCV_UNSET)
		return FALSE;

	DWORD dwIgnore = 0;

	if (nView == FTCV_TASKTREE)
		dwIgnore = (FO_HIDECOLLAPSED | FO_HIDEPARENTS);

	return m_filter.FilterMatches(filter, NULL, 0L, dwIgnore);
}

BOOL CFilteredToDoCtrl::IsFilterSet(FTC_VIEW nView) const
{
	if (nView == FTCV_UNSET)
		return FALSE;

	DWORD dwIgnore = 0;

	if (nView == FTCV_TASKTREE)
		dwIgnore = (FO_HIDECOLLAPSED | FO_HIDEPARENTS);

	return m_filter.HasAnyFilter(dwIgnore);
}

UINT CFilteredToDoCtrl::GetTaskCount(UINT* pVisible) const
{
	if (pVisible)
	{
		if (InListView())
			*pVisible = m_taskList.GetItemCount();
		else
			*pVisible = m_taskTree.GetItemCount();
	}

	return CTabbedToDoCtrl::GetTaskCount();
}

int CFilteredToDoCtrl::FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const
{
	if (params.bIgnoreFilteredOut)
		return CTabbedToDoCtrl::FindTasks(params, aResults);
	
	// else all tasks
	return m_data.FindTasks(params, aResults);
}

BOOL CFilteredToDoCtrl::HasCustomFilter() const 
{ 
	return m_filter.HasCustomFilter(); 
}

CString CFilteredToDoCtrl::GetCustomFilterName() const 
{ 
	return m_filter.GetCustomFilterName();
}

DWORD CFilteredToDoCtrl::GetCustomFilterFlags() const 
{ 
	if (HasCustomFilter())
		return m_filter.GetFilterFlags();

	// else
	return 0L;
}

BOOL CFilteredToDoCtrl::SetCustomFilter(const FTDCCUSTOMFILTER& filter)
{
	if (m_filter.SetCustomFilter(filter))
	{
		if (m_bDelayLoaded)
		{
			// mark everything needing refilter
			GetViewData2(FTCV_TASKTREE)->bNeedRefilter = TRUE;
			SetListNeedRefilter(TRUE);
			SetExtensionsNeedRefilter(TRUE);
		}
		else
		{
			RefreshFilter();
		}

		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

LRESULT CFilteredToDoCtrl::OnRefreshFilter(WPARAM wParam, LPARAM lParam)
{
	BOOL bUndo = lParam;
	FTC_VIEW nView = (FTC_VIEW)wParam;

	if (nView == FTCV_TASKTREE)
	{
		// This will also refresh the list view if it is active
		RefreshFilter();
	}
	else if (nView == FTCV_TASKLIST || bUndo)
	{
		// if undoing then we must also refresh the list filter because
		// otherwise ResyncListSelection will fail in the case where
		// we are undoing a delete because the undone item will not yet be in the list.
		RefreshListFilter();
	}
	
	// resync selection?
	if (nView == FTCV_TASKLIST)
		ResyncListSelection();
	
	return 0L;
}

BOOL CFilteredToDoCtrl::FilterMatches(const FTDCFILTER& filter, LPCTSTR szCustom, DWORD dwCustomFlags, DWORD dwIgnoreFlags) const
{
	return m_filter.FilterMatches(filter, szCustom, dwCustomFlags, dwIgnoreFlags);
}

void CFilteredToDoCtrl::RefreshFilter() 
{
	CSaveFocus sf;

	RefreshTreeFilter(); // always

	FTC_VIEW nView = GetView();

	switch (nView)
	{
	case FTCV_TASKTREE:
		// mark all other views as needing refiltering
		SetListNeedRefilter(TRUE);
		SetExtensionsNeedRefilter(TRUE);
		break;

	case FTCV_TASKLIST:
		// mark extensions as needing refiltering
		RefreshListFilter();
		SetExtensionsNeedRefilter(TRUE);
		break;

	case FTCV_UIEXTENSION1:
	case FTCV_UIEXTENSION2:
	case FTCV_UIEXTENSION3:
	case FTCV_UIEXTENSION4:
	case FTCV_UIEXTENSION5:
	case FTCV_UIEXTENSION6:
	case FTCV_UIEXTENSION7:
	case FTCV_UIEXTENSION8:
	case FTCV_UIEXTENSION9:
	case FTCV_UIEXTENSION10:
	case FTCV_UIEXTENSION11:
	case FTCV_UIEXTENSION12:
	case FTCV_UIEXTENSION13:
	case FTCV_UIEXTENSION14:
	case FTCV_UIEXTENSION15:
	case FTCV_UIEXTENSION16:
		SetExtensionsNeedRefilter(TRUE);
		SetListNeedRefilter(TRUE);
		RefreshExtensionFilter(nView, TRUE);
		break;
	}
}

void CFilteredToDoCtrl::SetListNeedRefilter(BOOL bRefilter)
{
	GetViewData2(FTCV_TASKLIST)->bNeedRefilter = bRefilter;
}

void CFilteredToDoCtrl::SetExtensionsNeedRefilter(BOOL bRefilter, FTC_VIEW nIgnore)
{
	for (int nExt = 0; nExt < m_aExtViews.GetSize(); nExt++)
	{
		FTC_VIEW nView = (FTC_VIEW)(FTCV_UIEXTENSION1 + nExt);

		if (nView == nIgnore)
			continue;

		// else
		VIEWDATA2* pData = GetViewData2(nView);

		if (pData)
			pData->bNeedRefilter = bRefilter;
	}
}

void CFilteredToDoCtrl::RefreshTreeFilter() 
{
	if (m_data.GetTaskCount())
	{
		// save and reset current focus to work around a bug
		CSaveFocus sf;
		SetFocusToTasks();
		
		// cache current selection and task breadcrumbs before clearing selection
		TDCSELECTIONCACHE cache;
		CacheTreeSelection(cache);

		// grab the selected items for the filtering
		m_aFilterSelectedTaskIDs.Copy(cache.aSelTaskIDs);
		
		TSH().RemoveAll();
		
		// and expanded state
 		CPreferences prefs;
 		SaveTasksState(prefs, TRUE);
		
		CHoldRedraw hr(GetSafeHwnd());
		CHoldRedraw hr2(m_taskTree);
		CWaitCursor cursor;
		
		// rebuild the tree
		RebuildTree();
		
		// redo last sort
		if (InTreeView() && IsSorting())
		{
			Resort();
			m_bTreeNeedResort = FALSE;
		}
		else
		{
			m_bTreeNeedResort = TRUE;
		}
		
		// restore expanded state
		LoadTasksState(prefs, TRUE); 
		
		// restore selection
		if (!RestoreTreeSelection(cache))
		{
			HTREEITEM hti = m_taskTree.GetChildItem(NULL); // select first item

			if (hti)
				SelectItem(hti);
		}
	}
	
	// modify the tree prompt depending on whether there is a filter set
	UINT nPromptID = (IsFilterSet(FTCV_TASKTREE) ? IDS_TDC_FILTEREDTASKLISTPROMPT : IDS_TDC_TASKLISTPROMPT);
	m_taskTree.SetWindowPrompt(CEnString(nPromptID));
}

HTREEITEM CFilteredToDoCtrl::RebuildTree(const void* pContext)
{
	ASSERT(pContext == NULL);

	m_taskTree.DeleteAll();
	TCH().SelectItem(NULL);

	// build a find query that matches the filter
	if (HasAnyFilter())
	{
		SEARCHPARAMS filter;
		m_filter.BuildFilterQuery(filter, m_aCustomAttribDefs);

		return CTabbedToDoCtrl::RebuildTree(&filter);
	}

	// else
	return CTabbedToDoCtrl::RebuildTree(pContext);
}

BOOL CFilteredToDoCtrl::WantAddTask(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const void* pContext) const
{
	BOOL bWantTask = CTabbedToDoCtrl::WantAddTask(pTDI, pTDS, pContext);
	
	if (bWantTask && pContext != NULL) // it's a filter
	{
		const SEARCHPARAMS* pFilter = static_cast<const SEARCHPARAMS*>(pContext);
		SEARCHRESULT result;
		
		// special case: selected item
		if (pFilter->HasAttribute(TDCA_SELECTION))
		{
			// check completion
			if (pFilter->bIgnoreDone && m_data.IsTaskDone(pTDI, pTDS, TDCCHECKALL))
			{
				bWantTask = FALSE;
			}
			else
			{
				bWantTask = Misc::HasT(m_aFilterSelectedTaskIDs, pTDS->GetTaskID());

				// check parents
				if (!bWantTask && pFilter->bWantAllSubtasks)
				{
					TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();

					while (pTDSParent && !bWantTask)
					{
						bWantTask = Misc::HasT(m_aFilterSelectedTaskIDs, pTDSParent->GetTaskID());
						pTDSParent = pTDSParent->GetParentTask();
					}
				}
			}
		}
		else // rest of attributes
		{
			bWantTask = m_data.TaskMatches(pTDI, pTDS, *pFilter, result);
		}

		if (bWantTask && pTDS->HasSubTasks())
		{
			// NOTE: the only condition under which this method is called for
			// a parent is if none of its subtasks matched the filter.
			//
			// So if we're a parent and match the filter we need to do an extra check
			// to see if what actually matched was the absence of attributes
			//
			// eg. if the parent category is "" and the filter rule is 
			// TDCA_CATEGORY is (FOP_NOT_SET or FOP_NOT_INCLUDES or FOP_NOT_EQUAL) 
			// then we don't treat this as a match.
			//
			// The attributes to check are:
			//  Category
			//  Status
			//  Alloc To
			//  Alloc By
			//  Version
			//  Priority
			//  Risk
			//  Tags
			
			int nNumRules = pFilter->aRules.GetSize();
			
			for (int nRule = 0; nRule < nNumRules && bWantTask; nRule++)
			{
				const SEARCHPARAM& sp = pFilter->aRules[nRule];

				if (!sp.OperatorIs(FOP_NOT_EQUALS) && 
					!sp.OperatorIs(FOP_NOT_INCLUDES) && 
					!sp.OperatorIs(FOP_NOT_SET))
				{
					continue;
				}
				
				// else check for empty parent attributes
				switch (sp.GetAttribute())
				{
				case TDCA_ALLOCTO:
					bWantTask = (pTDI->aAllocTo.GetSize() > 0);
					break;
					
				case TDCA_ALLOCBY:
					bWantTask = !pTDI->sAllocBy.IsEmpty();
					break;
					
				case TDCA_VERSION:
					bWantTask = !pTDI->sVersion.IsEmpty();
					break;
					
				case TDCA_STATUS:
					bWantTask = !pTDI->sStatus.IsEmpty();
					break;
					
				case TDCA_CATEGORY:
					bWantTask = (pTDI->aCategories.GetSize() > 0);
					break;
					
				case TDCA_TAGS:
					bWantTask = (pTDI->aTags.GetSize() > 0);
					break;
					
				case TDCA_PRIORITY:
					bWantTask = (pTDI->nPriority != FM_NOPRIORITY);
					break;
					
				case TDCA_RISK:
					bWantTask = (pTDI->nRisk != FM_NORISK);
					break;
				}
			}
		}
	}
	
	return bWantTask; 
}

void CFilteredToDoCtrl::RefreshExtensionFilter(FTC_VIEW nView, BOOL bShowProgress)
{
	CWaitCursor cursor;

	IUIExtensionWindow* pExtWnd = GetCreateExtensionWnd(nView);
	ASSERT(pExtWnd);

	if (pExtWnd)
	{
		VIEWDATA2* pData = GetViewData2(nView);
		ASSERT(pData);

		if (bShowProgress)
			BeginExtensionProgress(pData, IDS_UPDATINGTABBEDVIEW);
		
		// update view with filtered tasks
		CTaskFile tasks;
		GetAllTasksForExtensionViewUpdate(tasks, pData->mapWantedAttrib);

		UpdateExtensionView(pExtWnd, tasks, IUI_ALL, pData->mapWantedAttrib); 

		// and clear all update flags
		pData->bNeedFullTaskUpdate = FALSE;
		pData->bNeedRefilter = FALSE;

		if (bShowProgress)
			EndExtensionProgress();
	}
}

// base class override
void CFilteredToDoCtrl::RebuildList(const void* pContext)
{
	// This should only be called virtually from base-class
	ASSERT(pContext == NULL);
	UNREFERENCED_PARAMETER(pContext);

	if (!m_bIgnoreListRebuild)
		RefreshListFilter();
}

void CFilteredToDoCtrl::RefreshListFilter() 
{
	GetViewData2(FTCV_TASKLIST)->bNeedRefilter = FALSE;

	// build a find query that matches the filter
	SEARCHPARAMS filter;
	m_filter.BuildFilterQuery(filter, m_aCustomAttribDefs);

	// rebuild the list
	RebuildList(filter);

	// modify the list prompt depending on whether there is a filter set
	UINT nPromptID = (IsFilterSet(FTCV_TASKLIST) ? IDS_TDC_FILTEREDTASKLISTPROMPT : IDS_TDC_TASKLISTPROMPT);
	m_taskList.SetWindowPrompt(CEnString(nPromptID));
}

void CFilteredToDoCtrl::RebuildList(const SEARCHPARAMS& filter)
{
	// since the tree will have already got the items we want 
	// we can optimize the rebuild under certain circumstances
	// which are: 
	// 1. the list is sorted OR
	// 2. the tree is unsorted OR
	// 3. we only want the selected items 
	// otherwise we need the data in it's unsorted state and the 
	// tree doesn't have it
	if (filter.HasAttribute(TDCA_SELECTION) ||
		m_taskList.IsSorting() || 
		!m_taskTree.IsSorting())
	{
		// rebuild the list from the tree
		CTabbedToDoCtrl::RebuildList(&filter);
	}
	else // rebuild from scratch
	{
		// cache current selection
		TDCSELECTIONCACHE cache;
		CacheListSelection(cache);

		// grab the selected items for the filtering
		m_aFilterSelectedTaskIDs.Copy(cache.aSelTaskIDs);

		// remove all existing items
		m_taskList.DeleteAll();

		// do the find
		CResultArray aResults;
		m_data.FindTasks(filter, aResults);

		// add tasks to list
		for (int nRes = 0; nRes < aResults.GetSize(); nRes++)
		{
			const SEARCHRESULT& res = aResults[nRes];

			// some more filtering required
			if (HasStyle(TDCS_ALWAYSHIDELISTPARENTS) || m_filter.HasFilterFlag(FO_HIDEPARENTS))
			{
				const TODOSTRUCTURE* pTDS = m_data.LocateTask(res.dwTaskID);
				ASSERT(pTDS);

				if (pTDS->HasSubTasks())
					continue;
			}
			else if (m_filter.HasFilterFlag(FO_HIDECOLLAPSED))
			{
				HTREEITEM hti = m_taskTree.GetItem(res.dwTaskID);
				ASSERT(hti);			

				if (m_taskTree.ItemHasChildren(hti) && !TCH().IsItemExpanded(hti))
					continue;
			}

			m_taskList.InsertItem(res.dwTaskID);
		}

		// restore selection
		RestoreListSelection(cache);

		Resort();
	}
}

void CFilteredToDoCtrl::AddTreeItemToList(HTREEITEM hti, const void* pContext)
{
	if (pContext == NULL)
	{
		CTabbedToDoCtrl::AddTreeItemToList(hti, NULL);
		return;
	}

	// else it's a filter
	const SEARCHPARAMS* pFilter = static_cast<const SEARCHPARAMS*>(pContext);

	if (hti)
	{
		BOOL bAdd = TRUE;
		DWORD dwTaskID = GetTaskID(hti);

		// check if parent items are to be ignored
		if ((m_filter.HasFilterFlag(FO_HIDEPARENTS) || HasStyle(TDCS_ALWAYSHIDELISTPARENTS)))
		{
			// quick test first
			if (m_taskTree.ItemHasChildren(hti))
			{
				bAdd = FALSE;
			}
			else // item might have children currently filtered out
			{
				const TODOSTRUCTURE* pTDS = m_data.LocateTask(dwTaskID);
				ASSERT(pTDS);

				bAdd = !pTDS->HasSubTasks();
			}
		}
		// else check if it's a parent item that's only present because
		// it has matching subtasks
		else if (m_taskTree.ItemHasChildren(hti) && !pFilter->HasAttribute(TDCA_SELECTION))
		{
			const TODOSTRUCTURE* pTDS = m_data.LocateTask(dwTaskID);
			const TODOITEM* pTDI = GetTask(dwTaskID); 

			if (pTDI)
			{
				SEARCHRESULT result;

				// ie. check that parent actually matches
				bAdd = m_data.TaskMatches(pTDI, pTDS, *pFilter, result);
			}
		}

		if (bAdd)
			m_taskList.InsertItem(dwTaskID);
	}

	// always check the children unless collapsed tasks ignored
	if (!m_filter.HasFilterFlag(FO_HIDECOLLAPSED) || !hti || TCH().IsItemExpanded(hti))
	{
		HTREEITEM htiChild = m_taskTree.GetChildItem(hti);

		while (htiChild)
		{
			// check
			AddTreeItemToList(htiChild, pContext);
			htiChild = m_taskTree.GetNextItem(htiChild);
		}
	}
}

BOOL CFilteredToDoCtrl::SetStyles(const CTDCStylesMap& styles)
{
	if (CTabbedToDoCtrl::SetStyles(styles))
	{
		// do we need to re-filter?
		if (HasAnyFilter() && GetViewData2(FTCV_TASKLIST)->bNeedRefilter)
		{
			RefreshTreeFilter(); // always

			if (InListView())
				RefreshListFilter();
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CFilteredToDoCtrl::SetStyle(TDC_STYLE nStyle, BOOL bOn, BOOL bWantUpdate)
{
	// base class processing
	if (CTabbedToDoCtrl::SetStyle(nStyle, bOn, bWantUpdate))
	{
		// post-precessing
		switch (nStyle)
		{
		case TDCS_DUEHAVEHIGHESTPRIORITY:
		case TDCS_DONEHAVELOWESTPRIORITY:
		case TDCS_ALWAYSHIDELISTPARENTS:
		case TDCS_TREATSUBCOMPLETEDASDONE:
			GetViewData2(FTCV_TASKLIST)->bNeedRefilter = TRUE;
			break;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CFilteredToDoCtrl::SplitSelectedTask(int nNumSubtasks)
{
   if (CTabbedToDoCtrl::SplitSelectedTask(nNumSubtasks))
   {
      if (InListView())
         RefreshListFilter();
 
      return TRUE;
   }
 
   return FALSE;
}

BOOL CFilteredToDoCtrl::CreateNewTask(LPCTSTR szText, TDC_INSERTWHERE nWhere, BOOL bEditText, DWORD dwDependency)
{
	if (CTabbedToDoCtrl::CreateNewTask(szText, nWhere, bEditText, dwDependency))
	{
		SetListNeedRefilter(!InListView());
		SetExtensionsNeedRefilter(TRUE, GetView());

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CFilteredToDoCtrl::CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const
{
	return CTabbedToDoCtrl::CanCreateNewTask(nInsertWhere);
}

void CFilteredToDoCtrl::SetModified(BOOL bMod, TDC_ATTRIBUTE nAttrib, DWORD dwModTaskID)
{
	BOOL bTreeRefiltered = FALSE, bListRefiltered = FALSE;

	if (bMod)
	{
		if (ModNeedsRefilter(nAttrib, FTCV_TASKTREE, dwModTaskID))
		{
			// Refresh the tree filter.
			SendMessage(WM_TDC_REFRESHFILTER, FTCV_TASKTREE, (nAttrib == TDCA_UNDO));

			// Note: This will also have refreshed the list filter if active
			bListRefiltered = (GetView() == FTCV_TASKLIST);
			bTreeRefiltered = TRUE;
		}
		else if (ModNeedsRefilter(nAttrib, FTCV_TASKLIST, dwModTaskID))
		{
			// Refresh the list filter if active, or mark as needing refilter
			if (InListView())
			{
				SendMessage(WM_TDC_REFRESHFILTER, FTCV_TASKLIST, (nAttrib == TDCA_UNDO));
				bListRefiltered = TRUE;
			}
			else
			{
				GetViewData2(FTCV_TASKLIST)->bNeedRefilter = TRUE;
			}
		}
	}

	// This may cause the list to be re-filtered again so if it's already done
	// we set a flag and ignore it
	CAutoFlag af(m_bIgnoreListRebuild, bListRefiltered);
	CAutoFlag af2(m_bIgnoreExtensionUpdate, bTreeRefiltered);

	CTabbedToDoCtrl::SetModified(bMod, nAttrib, dwModTaskID);
}

void CFilteredToDoCtrl::EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify)
{
	BOOL bWasTimeTracking = IsActivelyTimeTracking();
	
	CTabbedToDoCtrl::EndTimeTracking(bAllowConfirm, bNotify);
	
	// do we need to refilter?
	if (bWasTimeTracking && m_filter.HasCustomFilter() && m_filter.HasCustomFilterAttribute(TDCA_TIMESPENT))
	{
		RefreshFilter();
	}
}

BOOL CFilteredToDoCtrl::ModNeedsRefilter(TDC_ATTRIBUTE nModType, FTC_VIEW nView, DWORD dwModTaskID) const 
{
	// sanity checks
	if ((nModType == TDCA_NONE) || !HasStyle(TDCS_REFILTERONMODIFY))
		return FALSE;

	if (!m_filter.HasAnyFilter())
		return FALSE;
	
	// we only need to refilter if the modified attribute
	// actually affects the filter
	BOOL bNeedRefilter = m_filter.ModNeedsRefilter(nModType, m_aCustomAttribDefs);

	// handle attributes common to both filter types
	if (!bNeedRefilter)
	{
		switch (nModType)
		{
		case TDCA_NEWTASK:
			// if we refilter in the middle of adding a task it messes
			// up the tree items so we handle it in CreateNewTask
			break;
			
		case TDCA_DELETE:
			// this is handled in CTabbedToDoCtrl::SetModified
			break;
			
		case TDCA_UNDO:
		case TDCA_PASTE:
			// CTabbedToDoCtrl::SetModified() will force a refilter
			// of the list automatically in response to an undo/paste
			// so we don't need to handle it ourselves
			bNeedRefilter = (nView != FTCV_TASKLIST);
			break;
			
		case TDCA_POSITION: // == move
			bNeedRefilter = (nView == FTCV_TASKLIST && !IsSorting());
			break;

		case TDCA_SELECTION:
			// never need to refilter
			break;
		}
	}

	// finally, if this was a task edit we can just test to 
	// see if the modified task still matches the filter.
	if (bNeedRefilter && dwModTaskID)
	{
		// VERY SPECIAL CASE
		// The task being time tracked has been filtered out
		// in which case we don't need to check if it matches
		if (dwModTaskID == m_dwTimeTrackTaskID)
		{
			if (m_taskTree.GetItem(dwModTaskID) == NULL)
			{
				ASSERT(HasTask(dwModTaskID));
				ASSERT(nModType == TDCA_TIMESPENT);

				return FALSE;
			}
			// else fall thru
		}

		SEARCHPARAMS params;
		SEARCHRESULT result;

		// This will handle custom and 'normal' filters
		m_filter.BuildFilterQuery(params, m_aCustomAttribDefs);
		bNeedRefilter = !m_data.TaskMatches(dwModTaskID, params, result);
		
		// extra handling for 'Find Tasks' filters 
		if (bNeedRefilter && HasCustomFilter())
		{
			// don't refilter on Time Spent if time tracking
			bNeedRefilter = !(nModType == TDCA_TIMESPENT && IsActivelyTimeTracking());
		}
	}

	return bNeedRefilter;
}

void CFilteredToDoCtrl::Sort(TDC_COLUMN nBy, BOOL bAllowToggle)
{
	CTabbedToDoCtrl::Sort(nBy, bAllowToggle);
}

VIEWDATA2* CFilteredToDoCtrl::GetViewData2(FTC_VIEW nView) const
{
	return (VIEWDATA2*)CTabbedToDoCtrl::GetViewData(nView);
}

VIEWDATA2* CFilteredToDoCtrl::GetActiveViewData2() const
{
	return GetViewData2(GetView());
}

void CFilteredToDoCtrl::OnTimerMidnight()
{
	CTabbedToDoCtrl::OnTimerMidnight();

	// don't re-filter delay-loaded tasklists
	if (IsDelayLoaded())
		return;

	BOOL bRefilter = FALSE;
	FTDCFILTER filter;
	
	if (m_filter.GetFilter(filter) == FS_CUSTOM)
	{
		bRefilter = (m_filter.HasCustomFilterAttribute(TDCA_STARTDATE) || 
						m_filter.HasCustomFilterAttribute(TDCA_DUEDATE));
	}
	else
	{
		bRefilter = (((filter.nStartBy != FD_NONE) && (filter.nStartBy != FD_ANY)) ||
					((filter.nDueBy != FD_NONE) && (filter.nDueBy != FD_ANY)));
	}
	
	if (bRefilter)
		RefreshFilter();
}

void CFilteredToDoCtrl::ResetNowFilterTimer()
{
	FTDCFILTER filter;

	if (m_filter.GetFilter(filter) != FS_CUSTOM)
	{
		if ((filter.nDueBy == FD_NOW) || (filter.nStartBy == FD_NOW))
		{
			SetTimer(TIMER_NOWFILTER, ONE_MINUTE, NULL);
			return;
		}
	}

	// all else
	KillTimer(TIMER_NOWFILTER);
}

void CFilteredToDoCtrl::OnTimer(UINT nIDEvent) 
{
	AF_NOREENTRANT;
	
	switch (nIDEvent)
	{
	case TIMER_NOWFILTER:
		OnTimerNow();
		return;
	}

	CTabbedToDoCtrl::OnTimer(nIDEvent);
}

void CFilteredToDoCtrl::OnTimerNow()
{
	// Since this timer gets called every minute we have to
	// find an efficient way of detecting tasks that are
	// currently hidden but need to be shown
	
	// So first thing we do is find reasons not to:
	
	// We are hidden
	if (!IsWindowVisible())
	{
		TRACE(_T("CFilteredToDoCtrl::OnTimerNow eaten (Window not visible)\n"));
		return;
	}
	
	// We're already displaying all tasks
	if (m_taskTree.GetItemCount() == m_data.GetTaskCount())
	{
		TRACE(_T("CFilteredToDoCtrl::OnTimerNow eaten (All tasks showing)\n"));
		return;
	}
	
	// App is minimized or hidden
	if (AfxGetMainWnd()->IsIconic() || !AfxGetMainWnd()->IsWindowVisible())
	{
		TRACE(_T("CFilteredToDoCtrl::OnTimerNow eaten (App not visible)\n"));
		return;
	}
	
	// App is not the foreground window
	if (GetForegroundWindow() != AfxGetMainWnd())
	{
		TRACE(_T("CFilteredToDoCtrl::OnTimerNow eaten (App not active)\n"));
		return;
	}
	
	// iterate the full data looking for items not in the
	// tree and test them for inclusion in the filter
	CHTIMap htiMap;
	VERIFY (TCH().BuildHTIMap(htiMap) < (int)m_data.GetTaskCount());
	
	SEARCHPARAMS params;
	m_filter.BuildFilterQuery(params, m_aCustomAttribDefs);
	
	const TODOSTRUCTURE* pTDS = m_data.GetStructure();
	ASSERT(pTDS);
	
	if (FindNewNowFilterTasks(pTDS, params, htiMap))
	{
		FTDCFILTER filter;
		VERIFY(m_filter.GetFilter(filter) != FS_CUSTOM);
		
		BOOL bNewDue = (filter.nDueBy == FD_NOW);
		BOOL bNewStart = (filter.nStartBy == FD_NOW);
		ASSERT(bNewDue || bNewStart);
		
		BOOL bRefilter = FALSE;
		
		if (bNewDue)
		{
			bRefilter = (AfxMessageBox(CEnString(IDS_DUEBYNOW_CONFIRMREFILTER), MB_YESNO | MB_ICONQUESTION) == IDYES);
		}
		else if (bNewStart)
		{
			bRefilter = (AfxMessageBox(CEnString(IDS_STARTBYNOW_CONFIRMREFILTER), MB_YESNO | MB_ICONQUESTION) == IDYES);
		}
		
		if (bRefilter)
		{
			RefreshFilter();
		}
		else // make the timer 10 minutes so we don't annoy them too soon
		{
			SetTimer(TIMER_NOWFILTER, TEN_MINUTES, NULL);
		}
	}
}

BOOL CFilteredToDoCtrl::FindNewNowFilterTasks(const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& params, const CHTIMap& htiMap) const
{
	ASSERT(pTDS);

	// process task
	if (!pTDS->IsRoot())
	{
		// is the task invisible?
		HTREEITEM htiDummy;
		DWORD dwTaskID = pTDS->GetTaskID();

		if (!htiMap.Lookup(dwTaskID, htiDummy))
		{
			// does the task match the current filter
			SEARCHRESULT result;

			// This will handle custom and 'normal' filters
			if (m_data.TaskMatches(dwTaskID, params, result))
				return TRUE;
		}
	}

	// then children
	for (int nTask = 0; nTask < pTDS->GetSubTaskCount(); nTask++)
	{
		if (FindNewNowFilterTasks(pTDS->GetSubTask(nTask), params, htiMap))
			return TRUE;
	}

	// no new tasks
	return FALSE;
}

BOOL CFilteredToDoCtrl::GetAllTasksForExtensionViewUpdate(CTaskFile& tasks, const CTDCAttributeMap& mapAttrib) const
{
	if (m_bIgnoreExtensionUpdate)
	{
		return FALSE;
	}

	// Special case: No filter is set -> All tasks (v much faster)
	if (!IsFilterSet(FTCV_TASKTREE))
	{
		if (CTabbedToDoCtrl::GetAllTasks(tasks))
		{
			AddGlobalsToTaskFile(tasks, mapAttrib);
			return TRUE;
		}

		// else
		return FALSE;
	}

	// else
	return CTabbedToDoCtrl::GetAllTasksForExtensionViewUpdate(tasks, mapAttrib);
}
