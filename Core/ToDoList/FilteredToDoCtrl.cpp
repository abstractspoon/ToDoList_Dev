// Fi M_BlISlteredToDoCtrl.cpp: implementation of the CFilteredToDoCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilteredToDoCtrl.h"
#include "todoitem.h"
#include "resource.h"
#include "tdcstatic.h"
#include "tdcmsg.h"
#include "TDCSearchParamHelper.h"
#include "taskclipboard.h"

#include "..\shared\holdredraw.h"
#include "..\shared\datehelper.h"
#include "..\shared\enstring.h"
#include "..\shared\deferwndmove.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\osversion.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\savefocus.h"
#include "..\shared\filemisc.h"
#include "..\shared\ScopedTimer.h"

#include "..\Interfaces\Preferences.h"
#include "..\Interfaces\IUIExtension.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////

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
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilteredToDoCtrl::CFilteredToDoCtrl(CUIExtensionMgr& mgrUIExt, 
									 CTDLContentMgr& mgrContent, 
									 CShortcutManager& mgrShortcuts,
									 const CONTENTFORMAT& cfDefault, 
									 const TDCCOLEDITFILTERVISIBILITY& visDefault) 
	:
	CTabbedToDoCtrl(mgrUIExt, mgrContent, mgrShortcuts, cfDefault, visDefault),
	m_visColEditFilter(visDefault),
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
	ON_CBN_EDITCHANGE(IDC_DUETIME, OnEditChangeDueTime)
	ON_REGISTERED_MESSAGE(WM_MIDNIGHT, OnMidnight)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////

BOOL CFilteredToDoCtrl::OnInitDialog()
{
	CTabbedToDoCtrl::OnInitDialog();

	return FALSE;
}

BOOL CFilteredToDoCtrl::SelectTask(DWORD dwTaskID, BOOL bTaskLink)
{	
	if (CTabbedToDoCtrl::SelectTask(dwTaskID, bTaskLink))
		return TRUE;

	// If it's a task link AND the task is filtered out 
	// we toggle the filter and try again
	if (bTaskLink && HasAnyFilter() && HasTask(dwTaskID))
	{
		// Shift the focus away from the comments because toggling
		// the filter may cause the comments to become disabled
		if (m_ctrlComments.HasFocus())
			SetFocusToTasks();
	
		ToggleFilter(); // show all tasks
		
		if (CTabbedToDoCtrl::SelectTask(dwTaskID, bTaskLink))
		{
			GetParent()->PostMessage(WM_TDCN_FILTERCHANGE, (WPARAM)GetSafeHwnd());
			return TRUE;
		}

		// else
		ASSERT(0);
		ToggleFilter(); // restore filter
	}
	
	return FALSE;
}

BOOL CFilteredToDoCtrl::SelectNextTask(CString sPart, TDC_SELECTNEXTTASK nSelect)
{
	return CTabbedToDoCtrl::SelectNextTask(sPart, nSelect); 
}

BOOL CFilteredToDoCtrl::SelectTasksInHistory(BOOL bForward)
{
	// If any of the tasks is filtered out we toggle the filter and try again
	if (HasAnyFilter() && CanSelectTasksInHistory(bForward))
	{
		CDWordArray aTaskIDs;
		VERIFY(m_taskTree.GetSelectedTasksInHistory(bForward, aTaskIDs));
		
		if (!m_taskTree.TreeItemMap().HasItems(aTaskIDs) &&	m_data.HasTasks(aTaskIDs))
		{
			ToggleFilter(); // show all tasks

			ASSERT (m_taskTree.TreeItemMap().HasItems(aTaskIDs));

			return SelectTasks(aTaskIDs);
		}
	}

	return CTabbedToDoCtrl::SelectTasksInHistory(bForward);
}

BOOL CFilteredToDoCtrl::LoadTasks(const CTaskFile& tasks)
{
	// handle reloading of tasklist with a filter present
	if (!m_bDelayLoaded && m_filter.HasAnyFilter())
	{
		SaveState();
	}

	BOOL bViewWasSet = IsViewSet();

	if (!CTabbedToDoCtrl::LoadTasks(tasks))
		return FALSE;

	FTC_VIEW nView = GetTaskView();

	// save visible state
	BOOL bHidden = !IsWindowVisible();

	// reload last view
	if (!bViewWasSet)
	{
		LoadState();

		// always refresh the tree filter because all other
		// views depend on it
		if (HasAnyFilter())
			RefreshTreeFilter(); // always

		// handle other views
		switch (nView)
		{
		case FTCV_TASKLIST:
			if (HasAnyFilter())
			{
				RebuildList();
			}
			else if (!GetPreferencesKey().IsEmpty()) // first time
			{
				SetViewNeedsTaskUpdate(nView);
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
			if (HasAnyFilter())
				RefreshExtensionFilter(nView);
			break;
		}
	}
	else if (HasAnyFilter())
	{
		RefreshFilter(FALSE);
	}

	// restore previously visibility
	if (bHidden)
		ShowWindow(SW_HIDE);

	return TRUE;
}

void CFilteredToDoCtrl::SaveState() const
{
	CPreferences prefs;
	m_filter.SaveFilter(prefs, GetPreferencesKey(_T("Filter")));
}

void CFilteredToDoCtrl::LoadState()
{
	if (HasStyle(TDCS_RESTOREFILTERS))
	{
		CPreferences prefs;
		m_filter.LoadFilter(prefs, GetPreferencesKey(_T("Filter")), m_aCustomAttribDefs);
	}
}

void CFilteredToDoCtrl::OnDestroy() 
{
	if (!m_bDelayLoaded)
		SaveState();

	CTabbedToDoCtrl::OnDestroy();
}

void CFilteredToDoCtrl::OnEditChangeDueTime()
{
	// need some special hackery to prevent a re-filter in the middle
	// of the user manually typing into the time field
	CDWordArray aSelTaskIDs;
	GetSelectedTaskIDs(aSelTaskIDs, FALSE);

	BOOL bNeedFullTaskUpdate = ModNeedsRefilter(TDCA_DUEDATE, aSelTaskIDs);
	
	if (bNeedFullTaskUpdate)
		m_styles[TDCS_REFILTERONMODIFY] = FALSE;
	
	CTabbedToDoCtrl::OnSelChangeDueTime();
	
	if (bNeedFullTaskUpdate)
		m_styles[TDCS_REFILTERONMODIFY] = TRUE;
}

BOOL CFilteredToDoCtrl::CopySelectedTasks() const
{
	// NOTE: we are overriding this function else
	// filtered out subtasks will not get copied
	if (!HasAnyFilter())
		return CTabbedToDoCtrl::CopySelectedTasks();

	// NOTE: We DON'T override GetSelectedTasks because
	// most often that only wants visible tasks
	if (!GetSelectedTaskCount())
		return FALSE;
	
	ClearCopiedItem();
	
	CTaskFile tasks;
	PrepareTaskfileForTasks(tasks, TDCGETTASKS());
	
	// get selected tasks ordered, removing duplicate subtasks
	CHTIList selection;
	TSH().CopySelection(selection, TRUE, TRUE);
	
	// copy items
	POSITION pos = selection.GetHeadPosition();
	HTASKITEM hPrevSiblingTask = NULL, hTask = NULL;
	
	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		DWORD dwTaskID = GetTaskID(hti);

		if (hPrevSiblingTask == NULL)
			hTask = m_exporter.ExportTask(dwTaskID, tasks, NULL);
		else
			hTask = m_exporter.ExportSiblingTask(dwTaskID, tasks, hPrevSiblingTask);
	}
	
	// extra processing to identify the originally selected tasks
	// in case the user wants to paste as references
	// Note: references can always be pasted 'as references'
	CDWordArray aSelTasks;
	TSH().GetItemData(aSelTasks);

	tasks.SetSelectedTaskIDs(aSelTasks);
	
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
		if (HasAnyFilter())
			RefreshFilter(FALSE);

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CFilteredToDoCtrl::ArchiveSelectedTasks(BOOL bRemove)
{
	if (CTabbedToDoCtrl::ArchiveSelectedTasks(bRemove))
	{
		if (HasAnyFilter())
			RefreshFilter(FALSE);

		return TRUE;
	}

	// else
	return FALSE;
}

int CFilteredToDoCtrl::GetArchivableTasks(CTaskFile& tasks, BOOL bSelectedOnly) const
{
	if (bSelectedOnly || !HasAnyFilter())
		return CTabbedToDoCtrl::GetArchivableTasks(tasks, bSelectedOnly);

	// else process the entire data hierarchy
	return m_exporter.ExportCompletedTasks(tasks);
}

BOOL CFilteredToDoCtrl::RemoveArchivedTask(DWORD dwTaskID)
{
	ASSERT(HasTask(dwTaskID));
	
	// note: if the tasks does not exist in the tree then this is not a bug
	// if a filter is set
	HTREEITEM hti = m_taskTree.GetItem(dwTaskID);
	
	if (!hti && !HasAnyFilter())
	{
		ASSERT(0);
		return FALSE;
	}
	
	if (hti)
		m_taskTree.DeleteItem(hti);

	return m_data.DeleteTask(dwTaskID, TRUE); // TRUE == with undo

}

int CFilteredToDoCtrl::GetFilteredTasks(CTaskFile& tasks, const TDCGETTASKS& filter) const
{
	// synonym for GetTasks which always returns the filtered tasks
	return GetTasks(tasks, GetTaskView(), filter);
}

FILTER_SHOW CFilteredToDoCtrl::GetFilter(TDCFILTER& filter) const
{
	return m_filter.GetFilter(filter);
}

void CFilteredToDoCtrl::SetFilter(const TDCFILTER& filter)
{
	FTC_VIEW nView = GetTaskView();

	if (m_bDelayLoaded)
	{
		m_filter.SetFilter(filter);

		// mark everything needing refilter
		SetViewNeedsTaskUpdate(FTCV_TASKTREE);
		SetViewNeedsTaskUpdate(FTCV_TASKLIST);
		
		SetExtensionsNeedTaskUpdate(TRUE);
	}
	else
	{
		BOOL bNeedFullTaskUpdate = !FilterMatches(filter);

		m_filter.SetFilter(filter);

		if (bNeedFullTaskUpdate)
			RefreshFilter(TRUE);
	}

	ResetNowFilterTimer();
}
	
void CFilteredToDoCtrl::ClearFilter()
{
	if (m_filter.ClearFilter())
		RefreshFilter(TRUE);

	ResetNowFilterTimer();
}

void CFilteredToDoCtrl::ToggleFilter()
{
	// PERMANENT LOGGING //////////////////////////////////////////////
	CScopedLogTimer log(_T("CFilteredToDoCtrl::ToggleFilter(%s)"), (m_filter.HasAnyFilter() ? _T("off") : _T("on")));
	///////////////////////////////////////////////////////////////////

	if (m_filter.ToggleFilter())
		RefreshFilter(TRUE);

	ResetNowFilterTimer();
}

UINT CFilteredToDoCtrl::GetTaskCount(UINT* pVisible) const
{
	if (pVisible)
	{
		if (InListView())
			*pVisible = (m_taskList.GetItemCount() - m_taskList.GetGroupCount());
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
	return m_matcher.FindTasks(params, HasDueTodayColor(), aResults);
}

BOOL CFilteredToDoCtrl::HasAdvancedFilter() const 
{ 
	return m_filter.HasAdvancedFilter(); 
}

CString CFilteredToDoCtrl::GetAdvancedFilterName() const 
{ 
	return m_filter.GetAdvancedFilterName();
}

DWORD CFilteredToDoCtrl::GetAdvancedFilterFlags() const 
{ 
	if (HasAdvancedFilter())
		return m_filter.GetFilterFlags();

	// else
	return 0L;
}

BOOL CFilteredToDoCtrl::SetAdvancedFilter(const TDCADVANCEDFILTER& filter)
{
	if (m_filter.SetAdvancedFilter(filter))
	{
		if (m_bDelayLoaded)
		{
			// mark everything needing refilter
			SetViewNeedsTaskUpdate(FTCV_TASKTREE);
			SetViewNeedsTaskUpdate(FTCV_TASKLIST);

			SetExtensionsNeedTaskUpdate(TRUE);
		}
		else
		{
			RefreshFilter(TRUE);
		}

		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CFilteredToDoCtrl::FilterMatches(const TDCFILTER& filter, LPCTSTR szCustom, DWORD dwCustomFlags) const
{
	return m_filter.FilterMatches(filter, szCustom, dwCustomFlags);
}

// External version
void CFilteredToDoCtrl::RefreshFilter() 
{
	RefreshFilter(TRUE);
}

// Internal version
BOOL CFilteredToDoCtrl::RefreshFilter(BOOL bExplicit) 
{
	CSaveFocus sf;

	// grab the selected items for the filtering
	if (bExplicit && m_filter.HasSelectionFilter())
		m_taskTree.GetSelectedTaskIDs(m_aSelectedTaskIDsForFiltering, FALSE);

	if (!RefreshTreeFilter())
		return FALSE;

	FTC_VIEW nView = GetTaskView();

	switch (nView)
	{
	case FTCV_TASKTREE:
	case FTCV_UNSET:
		SetViewNeedsTaskUpdate(FTCV_TASKLIST);
		SetExtensionsNeedTaskUpdate(TRUE);
		break;

	case FTCV_TASKLIST:
		RebuildList();
		SetExtensionsNeedTaskUpdate(TRUE);
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
		SetViewNeedsTaskUpdate(FTCV_TASKLIST);
		SetExtensionsNeedTaskUpdate(TRUE);
		RefreshExtensionFilter(nView, TRUE);
		SyncExtensionSelectionToTree(nView);
		UpdateSortStates(TDCA_ALL, TRUE);
		break;
	}

	return TRUE;
}

BOOL CFilteredToDoCtrl::RefreshTreeFilter() 
{
	if (m_data.GetTaskCount())
	{
		if (!HandleUnsavedComments())
			return FALSE;

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
	}
	
	// modify the tree prompt depending on whether there is a filter set
	if (HasAnyFilter())
		m_taskTree.SetWindowPrompt(CEnString(IDS_TDC_FILTEREDTASKLISTPROMPT));
	else
		m_taskTree.SetWindowPrompt(CEnString(IDS_TDC_TASKLISTPROMPT));

	return TRUE;
}

void CFilteredToDoCtrl::RebuildList(BOOL bChangeGroup, TDC_COLUMN nNewGroupBy, const void* pContext)
{
	if (m_bIgnoreListRebuild)
		return;

	// else
	ASSERT(pContext == NULL);

	// build a find query that matches the filter
	if (HasAnyFilter())
	{
		SEARCHPARAMS params;
		m_filter.BuildFilterQuery(params, m_aCustomAttribDefs);

		CTabbedToDoCtrl::RebuildList(bChangeGroup, nNewGroupBy, &params);

		m_taskList.SetWindowPrompt(CEnString(IDS_TDC_FILTEREDTASKLISTPROMPT));
	}
	else
	{
		CTabbedToDoCtrl::RebuildList(bChangeGroup, nNewGroupBy, pContext);
	}
}

int CFilteredToDoCtrl::GetAllTaskIDs(CDWordArray& aTaskIDs, BOOL bIncParents, BOOL bIncCollapsedChildren) const
{
	if (!CTabbedToDoCtrl::GetAllTaskIDs(aTaskIDs, bIncParents, bIncCollapsedChildren))
		return 0;

	if (!bIncParents)
	{
		// Note: We can still end up with parents in the list 
		// if all their children were filtered out but the 
		// parent wasn't
		int nID = aTaskIDs.GetSize();

		while (nID--)
		{
			if (m_data.IsTaskParent(aTaskIDs[nID]))
				aTaskIDs.RemoveAt(nID);
		}
	}
	
	return aTaskIDs.GetSize();
}

BOOL CFilteredToDoCtrl::WantAddTreeTaskToList(DWORD dwTaskID, const void* pContext) const
{
	BOOL bWantAdd = CTabbedToDoCtrl::WantAddTreeTaskToList(dwTaskID, pContext);

	if ((pContext == NULL) || !bWantAdd)
		return bWantAdd;

	// Hide non-matching parents
	const TODOSTRUCTURE* pTDS = NULL;
	const TODOITEM* pTDI = NULL;

	if (!m_data.GetTask(dwTaskID, pTDI, pTDS))
		return FALSE;

	if (pTDS->HasSubTasks())
	{
		const SEARCHPARAMS* pFilter = static_cast<const SEARCHPARAMS*>(pContext);
		SEARCHRESULT unused;

		bWantAdd = m_matcher.TaskMatches(pTDI, pTDS, *pFilter, HasDueTodayColor(), unused);
	}

	return bWantAdd;
}

HTREEITEM CFilteredToDoCtrl::RebuildTree(const void* pContext)
{
	ASSERT(pContext == NULL);

	// build a find query that matches the filter
	if (HasAnyFilter())
	{
		SEARCHPARAMS params;
		m_filter.BuildFilterQuery(params, m_aCustomAttribDefs);

		return CTabbedToDoCtrl::RebuildTree(&params);
	}

	// else
	return CTabbedToDoCtrl::RebuildTree(pContext);
}

BOOL CFilteredToDoCtrl::WantAddTaskToTree(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const void* pContext) const
{
	BOOL bWantTask = CTabbedToDoCtrl::WantAddTaskToTree(pTDI, pTDS, pContext);

	DWORD dwTaskID = pTDS->GetTaskID();
#ifdef _DEBUG
	DWORD dwParentID = pTDS->GetParentTaskID();
#endif
	
	if (bWantTask && (pContext != NULL)) // it's a filter
	{
		const SEARCHPARAMS* pFilter = static_cast<const SEARCHPARAMS*>(pContext);
		SEARCHRESULT result;
		
		// special case: selected item
		if (pFilter->HasAttribute(TDCA_SELECTION))
		{
			// check completion
			if (pFilter->bIgnoreDone && m_calculator.IsTaskDone(pTDI, pTDS))
			{
				bWantTask = FALSE;
			}
			else
			{
				bWantTask = Misc::HasT(pTDS->GetTaskID(), m_aSelectedTaskIDsForFiltering);

				// check parents
				if (!bWantTask && pFilter->bWantAllSubtasks)
				{
					TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();

					while (pTDSParent && !pTDSParent->IsRoot() && !bWantTask)
					{
						bWantTask = Misc::HasT(pTDSParent->GetTaskID(), m_aSelectedTaskIDsForFiltering);
						pTDSParent = pTDSParent->GetParentTask();
					}
				}
			}
		}
		else // rest of attributes
		{
			// Special case: References
			if (pTDI->IsReference())
				pTDI = m_data.GetTrueTask(dwTaskID);

			bWantTask = m_matcher.TaskMatches(pTDI, pTDS, *pFilter, HasDueTodayColor(), result);
		}

		if (bWantTask && pTDS->HasSubTasks())
		{
			// To arrive here means we are a parent task with no matching subtasks
			// because otherwise the parent will have been automatically added and
			// will never have needed explicit matching.
			//
			// And because parent tasks are often seen purely as containers
			// (without having explicitly set attributes), certain types of 
			// queries can produce lots of 'orphan' parent tasks which get in
			// the way of the more interesting tasks.
			//
			// So we attempt to filter them out by looking for rules that are
			// targeting specific attribute values and then we exclude the parent
			// if it has no value set for the attribute attached to the rule.
			//
			// NOTE: I'm deliberately not condensing this code because the last
			//       time I did that I could no longer remember the precise 
			//       reasoning behind it when I later came back.
			bWantTask = FALSE;

			int nNumRules = pFilter->aRules.GetSize();
			
			for (int nRule = 0; (nRule < nNumRules) && !bWantTask; nRule++)
			{
				const SEARCHPARAM& rule = pFilter->aRules[nRule];
								
				CString sWhatMatched;
				VERIFY(result.GetWhatMatched(rule.GetAttribute(), m_aCustomAttribDefs, sWhatMatched) && !sWhatMatched.IsEmpty());

				switch (rule.GetOperator())
				{
				case FOP_SET:
					// eg. "Give me all tasks where the category is not empty"
					// User is explicitly interested in tasks WITH attribute values
					// so we always want these tasks
					ASSERT(rule.AttributeIs(TDCA_SELECTION) || !sWhatMatched.IsEmpty());
					bWantTask = TRUE;
					break;

				case FOP_NOT_SET:
					// eg. "Give me all tasks where the category is empty"
					// User is explicitly interested in tasks WITHOUT attribute values
					// so we always want these tasks
					ASSERT(sWhatMatched.IsEmpty());
					bWantTask = TRUE;
					break;

				case FOP_EQUALS:
					// eg. "Give me all tasks where the category equals 'bug'"
					// User most likely interested in tasks WITH attribute values
					// so we only want these tasks if they have an attribute value
					bWantTask = !sWhatMatched.IsEmpty();
					break;

				case FOP_NOT_EQUALS:
					// eg. "Give me all tasks where the category does not equal 'bug'"
					// User most likely interested in tasks WITH attribute values
					// so we only want these tasks if they have an attribute value
					bWantTask = !sWhatMatched.IsEmpty();
					break;

				case FOP_INCLUDES:
					// User most likely interested in tasks WITH attribute values
					// eg. "Give me all tasks where the category includes 'bug'"
					// so we only want these tasks if they have an attribute value
					bWantTask = !sWhatMatched.IsEmpty();
					break;

				case FOP_NOT_INCLUDES:
					// User most likely interested in tasks WITH attribute values
					// eg. "Give me all tasks where the category does not include 'bug'"
					// so we only want these tasks if they have an attribute value
					bWantTask = !sWhatMatched.IsEmpty();
					break;

				default:
					ASSERT(0);
					bWantTask = TRUE;
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
		VIEWDATA* pData = GetViewData(nView);
		ASSERT(pData);

		if (bShowProgress)
			BeginExtensionProgress(pData, IDS_UPDATINGTABBEDVIEW);
		
		// clear all update flag
		pData->bNeedFullTaskUpdate = FALSE;

		// update view with filtered tasks
		CTaskFile tasks;
		GetAllTasksForExtensionViewUpdate(pData->mapWantedAttrib, tasks);

		UpdateExtensionView(pExtWnd, tasks, IUI_ALL); 
		
		if (bShowProgress)
			EndExtensionProgress();
	}
}

void CFilteredToDoCtrl::OnStylesUpdated(const CTDCStyleMap& styles)
{
	// If we're going to refilter anyway we ignore our
	// base class's possible update of the listview in
	// response to TDCS_ALWAYSHIDELISTPARENTS changing
	BOOL bNeedRefilter = StyleChangesNeedRefilter(styles);

	{
		ASSERT(!m_bIgnoreListRebuild);
		CAutoFlag af(m_bIgnoreListRebuild, bNeedRefilter);

		CTabbedToDoCtrl::OnStylesUpdated(styles);
	}
	
	if (bNeedRefilter)
		RefreshFilter(FALSE);
}

void CFilteredToDoCtrl::SetDueTaskColors(COLORREF crDue, COLORREF crDueToday)
{
	// See if we need to refilter
	BOOL bHadDueToday = m_taskTree.HasDueTodayColor();

	CTabbedToDoCtrl::SetDueTaskColors(crDue, crDueToday);

	if (bHadDueToday != m_taskTree.HasDueTodayColor())
	{
		// Because the 'Due Today' colour effectively alters 
		// a task's priority we can treat it as a priority edit
		if (m_filter.ModNeedsRefilter(TDCA_PRIORITY, m_aCustomAttribDefs))
			RefreshFilter(FALSE);
	}
}

BOOL CFilteredToDoCtrl::CreateNewTask(LPCTSTR szText, TDC_INSERTWHERE nWhere, BOOL bEditText, DWORD dwDependency)
{
	if (CTabbedToDoCtrl::CreateNewTask(szText, nWhere, bEditText, dwDependency))
	{
// 		SetViewNeedsTaskUpdate(FTCV_TASKLIST, !InListView());
// 		SetExtensionsNeedTaskUpdate(TRUE, GetTaskView());

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CFilteredToDoCtrl::CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const
{
	return CTabbedToDoCtrl::CanCreateNewTask(nInsertWhere);
}

void CFilteredToDoCtrl::SetModified(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs, BOOL bAllowResort)
{
	BOOL bTreeRefiltered = FALSE, bListRefiltered = FALSE;

	if (ModsNeedRefilter(mapAttribIDs, aModTaskIDs))
	{
		// This will also refresh the list view if it is active
		RefreshFilter(FALSE);

		// Note: This will also have refreshed the active view
		bTreeRefiltered = TRUE;
		bListRefiltered = InListView();
	}

	// This may cause either the list or one of the extensions to be rebuilt
	// we set flags and ignore it
	CAutoFlag af(m_bIgnoreListRebuild, bListRefiltered);
	CAutoFlag af2(m_bIgnoreExtensionUpdate, bTreeRefiltered);

	CTabbedToDoCtrl::SetModified(mapAttribIDs, aModTaskIDs, bAllowResort);

	if ((bListRefiltered || bTreeRefiltered) && mapAttribIDs.Has(TDCA_UNDO) && aModTaskIDs.GetSize())
	{
		// Restore the selection at the time of the undo if possible
		TDCSELECTIONCACHE cache;
		CacheTreeSelection(cache);
		
		if (!SelectTasks(aModTaskIDs))
			RestoreTreeSelection(cache);

		SyncActiveViewSelectionToTree();
	}
}

void CFilteredToDoCtrl::EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify)
{
	BOOL bWasTimeTracking = IsActivelyTimeTracking();
	
	CTabbedToDoCtrl::EndTimeTracking(bAllowConfirm, bNotify);
	
	// do we need to refilter?
	if (bWasTimeTracking && m_filter.HasAdvancedFilter() && m_filter.HasAdvancedFilterAttribute(TDCA_TIMESPENT))
	{
		RefreshFilter(FALSE);
	}
}

BOOL CFilteredToDoCtrl::ModsNeedRefilter(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs) const
{
	if (!m_filter.HasAnyFilter())
		return FALSE;

	POSITION pos = mapAttribIDs.GetStartPosition();

	while (pos)
	{
		if (ModNeedsRefilter(mapAttribIDs.GetNext(pos), aModTaskIDs))
			return TRUE;
	}

	return FALSE;
}

BOOL CFilteredToDoCtrl::ModNeedsRefilter(TDC_ATTRIBUTE nAttrib, const CDWordArray& aModTaskIDs) const
{
	// sanity checks
	if ((nAttrib == TDCA_NONE) || !HasStyle(TDCS_REFILTERONMODIFY))
		return FALSE;

	if (!m_filter.HasAnyFilter())
		return FALSE;

	// we only need to refilter if the modified attribute
	// actually affects the filter
	BOOL bNeedRefilter = m_filter.ModNeedsRefilter(nAttrib, m_aCustomAttribDefs);

	if (!bNeedRefilter)
	{
		// 'Other' attributes
		switch (nAttrib)
		{
		case TDCA_NEWTASK: // handled in CreateNewTask
		case TDCA_DELETE:
		case TDCA_POSITION:
		case TDCA_POSITION_SAMEPARENT:
		case TDCA_POSITION_DIFFERENTPARENT:
		case TDCA_SELECTION: 
			return FALSE;
			
		case TDCA_UNDO:
		case TDCA_PASTE:
		case TDCA_MERGE:
			return TRUE;
		}
	}
	else if (aModTaskIDs.GetSize() == 1)
	{
		DWORD dwModTaskID = aModTaskIDs[0];

		// VERY SPECIAL CASE
		// The task being time tracked has been filtered out
		// in which case we don't need to check if it matches
		if (m_timeTracking.IsTrackingTask(dwModTaskID))
		{
			if (m_taskTree.GetItem(dwModTaskID) == NULL)
			{
				ASSERT(HasTask(dwModTaskID));
				ASSERT(nAttrib == TDCA_TIMESPENT);

				return FALSE;
			}
			// else fall thru
		}

		// Finally, if this was a simple task edit we can just test to 
		// see if the modified task still matches the filter.
		SEARCHPARAMS params;
		SEARCHRESULT result;

		m_filter.BuildFilterQuery(params, m_aCustomAttribDefs);

		BOOL bMatchesFilter = m_matcher.TaskMatches(dwModTaskID, params, FALSE, result);
		BOOL bTreeHasItem = (m_taskTree.GetItem(dwModTaskID) != NULL);

		bNeedRefilter = ((bMatchesFilter && !bTreeHasItem) || (!bMatchesFilter && bTreeHasItem));
		
		// extra handling for 'Find Tasks' filters 
		if (bNeedRefilter && HasAdvancedFilter())
		{
			// don't refilter on Time Spent if time tracking
			bNeedRefilter = !(nAttrib == TDCA_TIMESPENT && IsActivelyTimeTracking());
		}
	}

	return bNeedRefilter;
}

BOOL CFilteredToDoCtrl::StyleChangesNeedRefilter(const CTDCStyleMap& styles) const
{
	// sanity check
	if (!HasAnyFilter())
		return FALSE;

	CTDCAttributeMap mapAttribAffected;
	POSITION pos = styles.GetStartPosition();

	while (pos)
	{
		switch (styles.GetNext(pos))
		{
		case TDCS_NODUEDATEISDUETODAYORSTART:
		case TDCS_USEEARLIESTDUEDATE:
		case TDCS_USELATESTDUEDATE:
			mapAttribAffected.Add(TDCA_DUEDATE);
			break;

		case TDCS_USEEARLIESTSTARTDATE:
		case TDCS_USELATESTSTARTDATE:
			mapAttribAffected.Add(TDCA_STARTDATE);
			break;

		case TDCS_CALCREMAININGTIMEBYDUEDATE:
		case TDCS_CALCREMAININGTIMEBYSPENT:
		case TDCS_CALCREMAININGTIMEBYPERCENT:
			// Not supported
			break;

		case TDCS_DUEHAVEHIGHESTPRIORITY:
		case TDCS_DONEHAVELOWESTPRIORITY:
		case TDCS_USEHIGHESTPRIORITY:
		case TDCS_INCLUDEDONEINPRIORITYCALC:
			mapAttribAffected.Add(TDCA_PRIORITY);
			break;

		case TDCS_DONEHAVELOWESTRISK:
		case TDCS_USEHIGHESTRISK:
		case TDCS_INCLUDEDONEINRISKCALC:
			mapAttribAffected.Add(TDCA_RISK);
			break;

		case TDCS_TREATSUBCOMPLETEDASDONE:
			{
				mapAttribAffected.Add(TDCA_DONEDATE);

				if (styles.HasStyle(TDCS_DONEHAVELOWESTPRIORITY) || 
					styles.HasStyle(TDCS_INCLUDEDONEINPRIORITYCALC))
				{
					mapAttribAffected.Add(TDCA_PRIORITY);
				}

				if (styles.HasStyle(TDCS_DONEHAVELOWESTRISK) ||
					styles.HasStyle(TDCS_INCLUDEDONEINRISKCALC))
				{
					mapAttribAffected.Add(TDCA_PRIORITY);
				}

				if (styles.HasStyle(TDCS_INCLUDEDONEINAVERAGECALC))
				{
					mapAttribAffected.Add(TDCA_PERCENT);
				}
			}
			break;

		case TDCS_USEPERCENTDONEINTIMEEST:
			mapAttribAffected.Add(TDCA_TIMEESTIMATE);
			break;

		case TDCS_INCLUDEDONEINAVERAGECALC:
		case TDCS_WEIGHTPERCENTCALCBYNUMSUB:
		case TDCS_AVERAGEPERCENTSUBCOMPLETION:
		case TDCS_AUTOCALCPERCENTDONE:
			mapAttribAffected.Add(TDCA_PERCENT);
			break;

		case TDCS_HIDESTARTDUEFORDONETASKS:
			//mapAttribAffected.Add(TDCA_);
			break;
		}
	}
	
	return ModsNeedRefilter(mapAttribAffected, CDWordArray());
}

void CFilteredToDoCtrl::Sort(TDC_COLUMN nBy, BOOL bAllowToggle)
{
	CTabbedToDoCtrl::Sort(nBy, bAllowToggle);
}

LRESULT CFilteredToDoCtrl::OnMidnight(WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = CTabbedToDoCtrl::OnMidnight(wParam, lParam);

	// don't re-filter delay-loaded tasklists
	if (!IsDelayLoaded())
	{
		BOOL bRefilter = FALSE;
		TDCFILTER filter;

		if (m_filter.GetFilter(filter) == FS_ADVANCED)
		{
			bRefilter = (m_filter.HasAdvancedFilterAttribute(TDCA_STARTDATE) ||
						 m_filter.HasAdvancedFilterAttribute(TDCA_DUEDATE));
		}
		else
		{
			bRefilter = (((filter.nStartBy != FD_NONE) && (filter.nStartBy != FD_ANY)) ||
						((filter.nDueBy != FD_NONE) && (filter.nDueBy != FD_ANY)));
		}

		if (bRefilter)
			RefreshFilter(FALSE);
	}

	return lr;
}

void CFilteredToDoCtrl::ResetNowFilterTimer()
{
	if (m_filter.HasNowFilter())
	{
		SetTimer(TIMER_NOWFILTER, ONE_MINUTE, NULL);
		return;
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
	ASSERT(m_taskTree.TreeItemMap().GetCount() < m_data.GetTaskCount());
	
	SEARCHPARAMS params;
	m_filter.BuildFilterQuery(params, m_aCustomAttribDefs);
	
	const TODOSTRUCTURE* pTDS = m_data.GetStructure();
	ASSERT(pTDS);
	
	if (FindNewNowFilterTasks(pTDS, params, m_taskTree.TreeItemMap()))
	{
		TDC_ATTRIBUTE nNowAttrib;

		if (m_filter.HasNowFilter(nNowAttrib))
		{
			BOOL bRefilter = FALSE;
		
			switch (nNowAttrib)
			{
			case TDCA_DUEDATE:
				bRefilter = (AfxMessageBox(CEnString(IDS_DUEBYNOW_CONFIRMREFILTER), MB_YESNO | MB_ICONQUESTION) == IDYES);
				break;

			case TDCA_STARTDATE:
				bRefilter = (AfxMessageBox(CEnString(IDS_STARTBYNOW_CONFIRMREFILTER), MB_YESNO | MB_ICONQUESTION) == IDYES);
				break;

			default:
				if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nNowAttrib))
				{
					// TODO
					//bRefilter = (AfxMessageBox(CEnString(IDS_CUSTOMBYNOW_CONFIRMREFILTER), MB_YESNO | MB_ICONQUESTION) == IDYES);
				}
				else
				{
					ASSERT(0);
				}
			}
		
			if (bRefilter)
			{
				RefreshFilter(FALSE);
			}
			else // make the timer 10 minutes so we don't re-prompt them too soon
			{
				SetTimer(TIMER_NOWFILTER, TEN_MINUTES, NULL);
			}
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
			if (m_matcher.TaskMatches(dwTaskID, params, FALSE, result))
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

BOOL CFilteredToDoCtrl::GetAllTasksForExtensionViewUpdate(const CTDCAttributeMap& mapAttrib, CTaskFile& tasks) const
{
	if (m_bIgnoreExtensionUpdate)
	{
		return FALSE;
	}

	// Special case: No filter is set -> All tasks (v much faster)
	if (!HasAnyFilter())
	{
		PrepareTaskfileForTasks(tasks, TDCGT_ALL);

		if (CTabbedToDoCtrl::GetAllTasks(tasks))
		{
			AddGlobalsToTaskFile(tasks, mapAttrib);
			tasks.SetAvailableAttributes(mapAttrib);
			
			return TRUE;
		}

		// else
		return FALSE;
	}

	// else
	return CTabbedToDoCtrl::GetAllTasksForExtensionViewUpdate(mapAttrib, tasks);
}

void CFilteredToDoCtrl::SetColumnFieldVisibility(const TDCCOLEDITFILTERVISIBILITY& vis)
{
	CTabbedToDoCtrl::SetColumnFieldVisibility(vis);

	m_visColEditFilter = vis;
}

void CFilteredToDoCtrl::GetColumnFieldVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const
{
	vis = m_visColEditFilter;
}

const CTDCColumnIDMap& CFilteredToDoCtrl::GetVisibleColumns() const
{
	ASSERT(m_visColEditFilter.GetVisibleColumns().MatchAll(m_visColEdit.GetVisibleColumns()));

	return m_visColEditFilter.GetVisibleColumns();
}

const CTDCAttributeMap& CFilteredToDoCtrl::GetVisibleEditFields() const
{
	ASSERT(m_visColEditFilter.GetVisibleEditFields().MatchAll(m_visColEdit.GetVisibleEditFields()));

	return m_visColEditFilter.GetVisibleEditFields();
}

const CTDCAttributeMap& CFilteredToDoCtrl::GetVisibleFilterFields() const
{
	return m_visColEditFilter.GetVisibleFilterFields();
}

void CFilteredToDoCtrl::SaveAttributeVisibility(CTaskFile& tasks) const
{
	if (HasStyle(TDCS_SAVEUIVISINTASKLIST))
		tasks.SetAttributeVisibility(m_visColEditFilter);
}

void CFilteredToDoCtrl::SaveAttributeVisibility(CPreferences& prefs) const
{
	m_visColEditFilter.Save(prefs, GetPreferencesKey());
}

void CFilteredToDoCtrl::LoadAttributeVisibility(const CTaskFile& tasks, const CPreferences& prefs)
{
	// attrib visibility can be stored inside the file or the preferences
	TDCCOLEDITFILTERVISIBILITY vis;

	if (tasks.GetAttributeVisibility(vis))
	{
		m_styles[TDCS_SAVEUIVISINTASKLIST] = TRUE;
	}
	else if (!vis.Load(prefs, GetPreferencesKey()))
	{
		vis = m_visColEditFilter;
	}

	// Preserve edit field visibility
	vis.ShowColorEditIfAsColumns(m_visColEditFilter.IsEditFieldVisible(TDCA_COLOR));

	SetColumnFieldVisibility(vis);
}

DWORD CFilteredToDoCtrl::MergeNewTaskIntoTree(const CTaskFile& tasks, HTASKITEM hTask, DWORD dwParentTaskID, BOOL bAndSubtasks)
{
	// If the parent has been filtered out we just add 
	// directly to the data model
	if (dwParentTaskID && !m_taskTree.GetItem(dwParentTaskID))
		return MergeNewTaskIntoTree(tasks, hTask, dwParentTaskID, 0, bAndSubtasks);

	// else
	return CTabbedToDoCtrl::MergeNewTaskIntoTree(tasks, hTask, dwParentTaskID, bAndSubtasks);
}

DWORD CFilteredToDoCtrl::MergeNewTaskIntoTree(const CTaskFile& tasks, HTASKITEM hTask, DWORD dwParentTaskID, DWORD dwPrevSiblingID, BOOL bAndSubtasks)
{
	TODOITEM* pTDI = m_data.NewTask(tasks, hTask);

	DWORD dwTaskID = m_dwNextUniqueID++;
	m_data.AddTask(dwTaskID, pTDI, dwParentTaskID, dwPrevSiblingID);

	if (bAndSubtasks)
	{
		HTASKITEM hSubtask = tasks.GetFirstTask(hTask);
		DWORD dwSubtaskID = 0;

		while (hSubtask)
		{
			dwSubtaskID = MergeNewTaskIntoTree(tasks, hSubtask, dwTaskID, dwSubtaskID, TRUE);
			hSubtask = tasks.GetNextTask(hSubtask);
		}
	}

	return dwTaskID;
}

DWORD CFilteredToDoCtrl::RecreateRecurringTaskInTree(const CTaskFile& task, const COleDateTime& dtNext, BOOL bDueDate)
{
	DWORD dwTaskID = task.GetTaskID(task.GetFirstTask());

	// If the just completed task no longer exists in the tree
	// because it has been filtered out as part of its completion
	// then the default implementation will fail, so we need to
	// handle this specific case
	if (HasAnyFilter() && (m_taskTree.GetItem(dwTaskID) == NULL))
	{
		// Merge task into data structure after the existing task
		DWORD dwParentID = m_data.GetTaskParentID(dwTaskID);
		DWORD dwNewTaskID = MergeNewTaskIntoTree(task, task.GetFirstTask(), dwParentID, dwTaskID, TRUE);

		InitialiseNewRecurringTask(dwTaskID, dwNewTaskID, dtNext, bDueDate);
		RefreshFilter(FALSE);

		// Note: there is no guarantee that this new task will not
		// also have been filtered out
		if (m_taskTree.GetItem(dwNewTaskID) == NULL)
			return 0L;

		// else
		return dwNewTaskID;
	}

	// all else
	return CTabbedToDoCtrl::RecreateRecurringTaskInTree(task, dtNext, bDueDate);
}

