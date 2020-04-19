// GanttTreeList.cpp: implementation of the CGanttTreeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "GanttCtrl.h"
#include "GanttMsg.h"
#include "GanttStatic.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\DateHelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\graphicsMisc.h"
#include "..\shared\autoflag.h"
#include "..\shared\misc.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\themed.h"
#include "..\shared\enbitmap.h"
#include "..\shared\copywndcontents.h"
#include "..\shared\WorkingWeek.h"

#include "..\3rdparty\shellicons.h"

#include "..\Interfaces\UITheme.h"

#include <float.h> // for DBL_MAX
#include <math.h>  // for fabs()

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

using namespace GanttStatic;

//////////////////////////////////////////////////////////////////////

#ifndef GET_WHEEL_DELTA_WPARAM
#	define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#endif 

#ifndef CDRF_SKIPPOSTPAINT
#	define CDRF_SKIPPOSTPAINT	(0x00000100)
#endif

//////////////////////////////////////////////////////////////////////

const int DEF_MONTH_WIDTH		= GraphicsMisc::ScaleByDPIFactor(72);
const int COLUMN_PADDING		= GraphicsMisc::ScaleByDPIFactor(15);
const int MIN_MONTH_WIDTH		= GraphicsMisc::ScaleByDPIFactor(2);
const int LV_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int HD_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(6);
const int DRAG_BUFFER			= GraphicsMisc::ScaleByDPIFactor(50);
const int DONE_BOX				= GraphicsMisc::ScaleByDPIFactor(6);
const int PARENT_ARROW_SIZE		= GraphicsMisc::ScaleByDPIFactor(6);

//////////////////////////////////////////////////////////////////////

const LONG DEPENDPICKPOS_NONE		= 0xFFFFFFFF;

const double DAY_WEEK_MULTIPLIER	= 1.5;
const double HOUR_DAY_MULTIPLIER	= 6;
const double MULTIYEAR_MULTIPLIER	= 2.0;
const double DAYS_IN_YEAR			= 365.25;
const double DAYS_IN_MONTH			= (DAYS_IN_YEAR / 12);

const int MINS_IN_HOUR				= 60;
const int MINS_IN_DAY				= (MINS_IN_HOUR * 24);
const int MAX_HEADER_WIDTH			= 32000; // (SHRT_MAX - tolerance)

//////////////////////////////////////////////////////////////////////

#define GET_GI_RET(id, gi, ret)	\
{								\
	if (id == 0) return ret;	\
	gi = GetGanttItem(id);		\
	ASSERT(gi);					\
	if (gi == NULL) return ret;	\
}

#define GET_GI(id, gi)		\
{							\
	if (id == 0) return;	\
	gi = GetGanttItem(id);	\
	ASSERT(gi);				\
	if (gi == NULL)	return;	\
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGanttCtrl::CGanttCtrl() 
	:
	m_nMonthWidth(DEF_MONTH_WIDTH),
	m_nMonthDisplay(GTLC_DISPLAY_MONTHS_LONG),
	m_dwOptions(GTLCF_AUTOSCROLLTOTASK | GTLCF_SHOWSPLITTERBAR),
	m_crBarDefault(CLR_NONE),
	m_crParent(CLR_NONE),
	m_crToday(CLR_NONE),
	m_crWeekend(RGB(224, 224, 224)),
	m_crNonWorkingHours(RGB(224, 224, 224)),
	m_nParentColoring(GTLPC_DEFAULTCOLORING),
	m_nDragging(GTLCD_NONE), 
	m_ptDragStart(0),
	m_ptLastDependPick(0),
	m_pDependEdit(NULL),
	m_dwMaxTaskID(0),
	m_bReadOnly(FALSE)
{

}

CGanttCtrl::~CGanttCtrl()
{
}

BEGIN_MESSAGE_MAP(CGanttCtrl, CTreeListCtrl)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREELISTTREE, OnBeginEditTreeLabel)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_TREELISTTREEHEADER, OnClickTreeHeader)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TREELISTTREE, OnTreeGetDispInfo)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int CGanttCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	BuildTreeColumns();
	BuildListColumns();

	CalcMinMonthWidths();
	EnableTreeImagePlaceholder();

	if (m_nMonthWidth != DEF_MONTH_WIDTH)
		RecalcListColumnWidths(DEF_MONTH_WIDTH, m_nMonthWidth);

	// prevent column reordering on list
	m_listHeader.ModifyStyle(HDS_DRAGDROP, 0);

	// prevent translation of the list header
	CLocalizer::EnableTranslation(m_listHeader, FALSE);

	return 0;
}

void CGanttCtrl::InitItemHeights()
{
	CTreeListCtrl::InitItemHeights();

	GANTTDEPENDENCY::STUB = (m_tree.GetItemHeight() / 2);
}

DWORD CGanttCtrl::GetSelectedTaskID() const
{
	return GetSelectedItemData();
}

BOOL CGanttCtrl::GetSelectedTaskDependencies(CDWordArray& aDepends) const
{
	DWORD dwTaskID = GetSelectedTaskID();
	const GANTTITEM* pGI = NULL;
	
	GET_GI_RET(dwTaskID, pGI, FALSE);
	
	aDepends.Copy(pGI->aDependIDs);
	return TRUE;
}

BOOL CGanttCtrl::SetSelectedTaskDependencies(const CDWordArray& aDepends)
{
	DWORD dwTaskID = GetSelectedTaskID();
	GANTTITEM* pGI = NULL;
	
	GET_GI_RET(dwTaskID, pGI, FALSE);

	pGI->aDependIDs.Copy(aDepends);
	RedrawList();
	
	return TRUE;
}

BOOL CGanttCtrl::GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const
{
	DWORD dwTaskID = GetSelectedTaskID();
	const GANTTITEM* pGI = NULL;

	GET_GI_RET(dwTaskID, pGI, FALSE);
	
	if (GetTaskStartEndDates(*pGI, dtStart, dtDue))
	{
		// handle durations of whole days
		COleDateTime dtDuration(dtDue - dtStart);

		if (CDateHelper::IsDateSet(dtDuration) && (dtDuration > CDateHelper::GetEndOfDay(dtDuration)))
		{
			double dWholeDays = (CDateHelper::GetDateOnly(dtDuration).m_dt + 1.0);

			if (!CDateHelper::DateHasTime(dtStart))
				dWholeDays--;

			dtDue.m_dt = (dtStart.m_dt + dWholeDays);
		}

		return TRUE;
	}

	// all else
	return FALSE;
}

BOOL CGanttCtrl::SelectTask(DWORD dwTaskID)
{
	HTREEITEM hti = GetTreeItem(dwTaskID);

	return SelectItem(hti);
}

BOOL CGanttCtrl::SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select)
{
	HTREEITEM htiStart = NULL;
	BOOL bForwards = TRUE;

	switch (nCmd)
	{
	case IUI_SELECTFIRSTTASK:
		htiStart = m_tree.TCH().GetFirstItem();
		break;

	case IUI_SELECTNEXTTASK:
		htiStart = m_tree.TCH().GetNextItem(GetSelectedItem());
		break;
		
	case IUI_SELECTNEXTTASKINCLCURRENT:
		htiStart = GetSelectedItem();
		break;

	case IUI_SELECTPREVTASK:
		htiStart = m_tree.TCH().GetPrevItem(GetSelectedItem());

		if (htiStart == NULL) // we were on the first task
			htiStart = m_tree.TCH().GetLastItem();
		
		bForwards = FALSE;
		break;

	case IUI_SELECTLASTTASK:
		htiStart = m_tree.TCH().GetLastItem();
		bForwards = FALSE;
		break;

	default:
		return FALSE;
	}

	return SelectTask(htiStart, select, bForwards);
}

BOOL CGanttCtrl::SelectTask(HTREEITEM hti, const IUISELECTTASK& select, BOOL bForwards)
{
	if (!hti)
		return FALSE;

	CString sTitle = m_tree.GetItemText(hti);

	if (Misc::Find(select.szWords, sTitle, select.bCaseSensitive, select.bWholeWord) != -1)
	{
		if (SelectItem(hti))
			return TRUE;

		ASSERT(0);
	}

	if (bForwards)
		return SelectTask(m_tree.TCH().GetNextItem(hti), select, TRUE);

	// else
	return SelectTask(m_tree.TCH().GetPrevItem(hti), select, FALSE);
}

void CGanttCtrl::RecalcParentDates()
{
	GANTTITEM dummy;
	GANTTITEM* pGI = &dummy;

	RecalcParentDates(NULL, pGI);
}

void CGanttCtrl::RecalcParentDates(HTREEITEM htiParent, GANTTITEM*& pGI)
{
	// ignore root
	DWORD dwTaskID = 0;

	// get gantt item for this tree item
	if (htiParent)
	{
		dwTaskID = m_tree.GetItemData(htiParent);
		GET_GI(dwTaskID, pGI);
	}
	
	// bail if this is a reference
	if (pGI->dwRefID)
		return;
	
	// iterate children 
	HTREEITEM htiChild = m_tree.GetChildItem(htiParent);

	if (htiChild)
	{
		// reset dates
		pGI->dtMinMaxRange = pGI->dtRange;

		while (htiChild)
		{
			GANTTITEM* pGIChild;

			// recalc child if it has children itself
			RecalcParentDates(htiChild, pGIChild); // RECURSIVE CALL
			ASSERT(pGIChild);

			// keep track of earliest start date and latest due date
			if (pGIChild)
			{
				pGI->MinMaxDates(*pGIChild,
								HasOption(GTLCF_CALCPARENTDATES),
								HasOption(GTLCF_CALCMISSINGSTARTDATES),
								HasOption(GTLCF_CALCMISSINGDUEDATES));
			}

			// next child
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
	else
	{
		pGI->dtMinMaxRange.Reset();
	}
}

BOOL CGanttCtrl::EditWantsResort(const ITASKLISTBASE* pTasks, IUI_UPDATETYPE nUpdate) const
{
	switch (nUpdate)
	{
	case IUI_ALL:
		// Note: Tasks should arrive 'unsorted' so we only need to
		// resort if an attribute is set
		return m_sort.IsSorting();

	case IUI_NEW:
		// Don't sort new tasks because it's confusing
		return FALSE;

	case IUI_EDIT:
		if (m_sort.IsSorting())
		{
			if (!m_sort.bMultiSort)
				return pTasks->IsAttributeAvailable(MapColumnToAttribute(m_sort.single.nBy));

			// else
			for (int nCol = 0; nCol < 3; nCol++)
			{
				if (pTasks->IsAttributeAvailable(MapColumnToAttribute(m_sort.multi.cols[nCol].nBy)))
					return TRUE;
			}
		}
		break;

	case IUI_DELETE:
		break;

	default:
		ASSERT(0);
	}

	return FALSE;
}

void CGanttCtrl::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate)
{
	// we must have been initialized already
	ASSERT(m_list.GetSafeHwnd() && m_tree.GetSafeHwnd());

	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return;
	}

	// always cancel any ongoing operation
	CancelOperation();

	switch (nUpdate)
	{
	case IUI_ALL:
		EnableResync(FALSE);
		{
			CLockUpdates lu(*this);

			CDWordArray aExpanded;
			GetExpandedState(aExpanded);
			
			DWORD dwSelID = GetSelectedTaskID();
			
			RebuildTree(pTasks);
			RecalcDateRange();

			// Odd bug: The very last tree item will sometimes not scroll into view. 
			// Expanding and collapsing an item is enough to resolve the issue. 
			// First time only though.
			if (aExpanded.GetSize() == 0)
				PreFixVScrollSyncBug();
			
			SetExpandedState(aExpanded);
			SelectTask(dwSelID);

			if (dwSelID)
				ScrollToSelectedTask();
			else
				ScrollToToday();
		}
		EnableResync(TRUE, m_tree);
		UpdateColumnWidths(UTWA_ANY);
		break;
		
	case IUI_NEW:
	case IUI_EDIT:
		{
			CHoldRedraw hr(GetHwnd());
			
			// cache current year range to test for changes
			int nNumMonths = GetNumMonths(m_nMonthDisplay);
			
			// update the task(s)
			if (UpdateTask(pTasks, pTasks->GetFirstTask(), nUpdate, TRUE))
			{
				if (pTasks->IsAttributeAvailable(TDCA_STARTDATE) ||
					pTasks->IsAttributeAvailable(TDCA_DUEDATE) ||
					pTasks->IsAttributeAvailable(TDCA_DONEDATE))
				{
					RecalcDateRange();
				}
			}
		}
		break;
		
	case IUI_DELETE:
		{
			CHoldRedraw hr(GetHwnd());

			CDWordSet mapIDs;
			BuildTaskMap(pTasks, pTasks->GetFirstTask(), mapIDs, TRUE);
			
			RemoveDeletedTasks(NULL, pTasks, mapIDs);
			UpdateParentStatus(pTasks, pTasks->GetFirstTask(), TRUE);
			RecalcDateRange();
		}
		break;
		
	default:
		ASSERT(0);
	}
	InitItemHeights();

	if (EditWantsResort(pTasks, nUpdate))
	{
		ASSERT(m_sort.IsSorting());

		CHoldRedraw hr(m_tree);

		if (m_sort.bMultiSort)
			CTreeListCtrl::Sort(MultiSortProc, (DWORD)this);
		else
			CTreeListCtrl::Sort(SortProc, (DWORD)this);
	}
}

void CGanttCtrl::PreFixVScrollSyncBug()
{
	// Odd bug: The very last tree item will not scroll into view. 
	// Expanding and collapsing an item is enough to resolve the issue.
	HTREEITEM hti = TCH().FindFirstParent();
		
	if (hti)
	{
		TCH().ExpandItem(hti, TRUE);
		TCH().ExpandItem(hti, FALSE);
	}
}

CString CGanttCtrl::GetTaskAllocTo(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	int nAllocTo = pTasks->GetTaskAllocatedToCount(hTask);
	
	if (nAllocTo == 0)
	{
		return _T("");
	}
	else if (nAllocTo == 1)
	{
		return pTasks->GetTaskAllocatedTo(hTask, 0);
	}
	
	// nAllocTo > 1 
	CStringArray aAllocTo;
	
	while (nAllocTo--)
		aAllocTo.InsertAt(0, pTasks->GetTaskAllocatedTo(hTask, nAllocTo));
	
	return Misc::FormatArray(aAllocTo);
}

BOOL CGanttCtrl::WantEditUpdate(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case TDCA_ALLOCTO:
	case TDCA_COLOR:
	case TDCA_DEPENDENCY:
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_ICON:
	case TDCA_ID:
	case TDCA_NONE:
	case TDCA_PERCENT:
	case TDCA_STARTDATE:
	case TDCA_SUBTASKDONE:
	case TDCA_TAGS:
	case TDCA_TASKNAME:
		return TRUE;
	}
	
	// all else 
	return (nAttrib == IUI_ALL);
}

BOOL CGanttCtrl::WantSortUpdate(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case TDCA_ALLOCTO:
	case TDCA_DUEDATE:
	case TDCA_ID:
	case TDCA_PERCENT:
	case TDCA_STARTDATE:
	case TDCA_TASKNAME:
	case TDCA_TAGS:
	case TDCA_DONEDATE:
	case TDCA_DEPENDENCY:
		return (MapAttributeToColumn(nAttrib) != GTLCC_NONE);

	case TDCA_NONE:
		return TRUE;
	}
	
	// all else 
	return FALSE;
}

TDC_ATTRIBUTE CGanttCtrl::MapColumnToAttribute(GTLC_COLUMN nCol)
{
	switch (nCol)
	{
	case GTLCC_TITLE:		return TDCA_TASKNAME;
	case GTLCC_DUEDATE:		return TDCA_DUEDATE;
	case GTLCC_STARTDATE:	return TDCA_STARTDATE;
	case GTLCC_ALLOCTO:		return TDCA_ALLOCTO;
	case GTLCC_PERCENT:		return TDCA_PERCENT;
	case GTLCC_TASKID:		return TDCA_ID;
	case GTLCC_DONEDATE:	return TDCA_DONEDATE;
	case GTLCC_TAGS:		return TDCA_TAGS;
	case GTLCC_DEPENDENCY:	return TDCA_DEPENDENCY;
	}
	
	// all else 
	return TDCA_NONE;
}

GTLC_COLUMN CGanttCtrl::MapAttributeToColumn(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case TDCA_TASKNAME:		return GTLCC_TITLE;		
	case TDCA_DUEDATE:		return GTLCC_DUEDATE;		
	case TDCA_STARTDATE:	return GTLCC_STARTDATE;	
	case TDCA_ALLOCTO:		return GTLCC_ALLOCTO;		
	case TDCA_PERCENT:		return GTLCC_PERCENT;		
	case TDCA_ID:			return GTLCC_TASKID;		
	case TDCA_DONEDATE:		return GTLCC_DONEDATE;
	case TDCA_TAGS:			return GTLCC_TAGS;
	case TDCA_DEPENDENCY:	return GTLCC_DEPENDENCY;
	}
	
	// all else 
	return GTLCC_NONE;
}

BOOL CGanttCtrl::UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return FALSE;

	ASSERT((nUpdate == IUI_EDIT) || (nUpdate == IUI_NEW));

	// handle task if not NULL (== root)
	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	m_dwMaxTaskID = max(m_dwMaxTaskID, dwTaskID);

	// This can be a new task
	if (!m_data.HasItem(dwTaskID))
	{
		ASSERT(nUpdate == IUI_NEW);

		// Parent must exist or be NULL
		DWORD dwParentID = pTasks->GetTaskParentID(hTask);
		HTREEITEM htiParent = NULL;

		if (dwParentID)
		{
			htiParent = GetTreeItem(dwParentID);

			if (!htiParent)
			{
				ASSERT(0);
				return FALSE;
			}

			// Ensure 'parent' status
			GANTTITEM* pGIParent = NULL;
			GET_GI_RET(dwParentID, pGIParent, FALSE);

			pGIParent->bParent = TRUE;
		}

		// Before anything else we increment the position of 
		// any tasks having the same position of this new task 
		// or greater within the parent
		int nPos = pTasks->GetTaskPosition(hTask);
		IncrementItemPositions(htiParent, nPos);

		BuildTreeItem(pTasks, hTask, htiParent, FALSE, FALSE);

		return TRUE;
	}
	
	// Don't resolve references here
	GANTTITEM* pGI = m_data.GetItem(dwTaskID, FALSE);

	if (!pGI)
	{
		ASSERT(0);
		return FALSE;
	}

	// Take a snapshot we can check changes against
	GANTTITEM giOrg = *pGI;

	// Update colour for all tasks
	pGI->color = pTasks->GetTaskTextColor(hTask);

	// Existing tasks should not change reference ID 
	DWORD dwRefID = pTasks->GetTaskReferenceID(hTask);
	ASSERT(pGI->dwRefID == dwRefID);

	pGI->dwOrgRefID = 0;
	
	// Update rest of attributes if not a reference task
	if (pGI->dwRefID == 0)
	{
		// can't use a switch here because we also need to check for IUI_ALL
		time64_t tDate = 0;
	
		if (pTasks->IsAttributeAvailable(TDCA_TASKNAME))
			pGI->sTitle = pTasks->GetTaskTitle(hTask);
	
		if (pTasks->IsAttributeAvailable(TDCA_ALLOCTO))
			pGI->sAllocTo = GetTaskAllocTo(pTasks, hTask);
	
		if (pTasks->IsAttributeAvailable(TDCA_ICON))
			pGI->bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

		if (pTasks->IsAttributeAvailable(TDCA_PERCENT))
			pGI->nPercent = pTasks->GetTaskPercentDone(hTask, TRUE);
		
		if (pTasks->IsAttributeAvailable(TDCA_STARTDATE))
		{ 
			time64_t tDate = 0;
	
			// update min/max too
			if (pTasks->GetTaskStartDate64(hTask, (pGI->bParent != FALSE), tDate))
				pGI->SetStartDate(tDate, TRUE);
			else
				pGI->ClearStartDate(TRUE);
		}
	
		if (pTasks->IsAttributeAvailable(TDCA_DUEDATE))
		{
			time64_t tDate = 0;

			// update min/max too
			if (pTasks->GetTaskDueDate64(hTask, (pGI->bParent != FALSE), tDate))
				pGI->SetDueDate(tDate, TRUE);
			else
				pGI->ClearDueDate(TRUE);
		}
	
		if (pTasks->IsAttributeAvailable(TDCA_DONEDATE))
		{
			time64_t tDate = 0;

			if (pTasks->GetTaskDoneDate64(hTask, tDate))
				pGI->SetDoneDate(tDate);
			else
				pGI->ClearDoneDate();
		}
	
		if (pTasks->IsAttributeAvailable(TDCA_SUBTASKDONE))
		{
			LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
			pGI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));
		}

		if (pTasks->IsAttributeAvailable(TDCA_TAGS))
		{
			int nTag = pTasks->GetTaskTagCount(hTask);
			pGI->aTags.RemoveAll();
		
			while (nTag--)
				pGI->aTags.Add(pTasks->GetTaskTag(hTask, nTag));
		}
	
		if (pTasks->IsAttributeAvailable(TDCA_DEPENDENCY))
		{
			int nDepend = pTasks->GetTaskDependencyCount(hTask);
			pGI->aDependIDs.RemoveAll();
		
			while (nDepend--)
			{
				// Local dependencies only
				DWORD dwTaskID = _ttoi(pTasks->GetTaskDependency(hTask, nDepend));

				if (dwTaskID)
					pGI->aDependIDs.Add(dwTaskID);
			}
		}

		// Always update these
		pGI->bLocked = pTasks->IsTaskLocked(hTask, true);
		pGI->bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);
	}

	// detect update
	BOOL bChange = (*pGI != giOrg);

	// children
	if (UpdateTask(pTasks, pTasks->GetFirstTask(hTask), nUpdate, TRUE))
		bChange = TRUE;

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			if (UpdateTask(pTasks, hSibling, nUpdate, FALSE))
				bChange = TRUE;
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
	
	return bChange;
}

void CGanttCtrl::BuildTaskMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
									  CDWordSet& mapIDs, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;

	mapIDs.Add(pTasks->GetTaskID(hTask));

	// children
	BuildTaskMap(pTasks, pTasks->GetFirstTask(hTask), mapIDs, TRUE);

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildTaskMap(pTasks, hSibling, mapIDs, FALSE);
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

void CGanttCtrl::RemoveDeletedTasks(HTREEITEM hti, const ITASKLISTBASE* pTasks, const CDWordSet& mapIDs)
{
	// traverse the tree looking for items that do not 
	// exist in pTasks and delete them
	if (hti && !mapIDs.Has(GetTaskID(hti)))
	{
		DeleteItem(hti);
		return;
	}

	// check its children
	HTREEITEM htiChild = m_tree.GetChildItem(hti);
	
	while (htiChild)
	{
		// get next sibling before we (might) delete this one
		HTREEITEM htiNext = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		
		RemoveDeletedTasks(htiChild, pTasks, mapIDs);
		htiChild = htiNext;
	}
}

void CGanttCtrl::UpdateParentStatus(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;

	// this task
	DWORD dwTaskID = pTasks->GetTaskID(hTask);

	GANTTITEM* pGI = NULL;
	GET_GI(dwTaskID, pGI);

	pGI->bParent = pTasks->IsTaskParent(hTask);

	// children
	UpdateParentStatus(pTasks, pTasks->GetFirstTask(hTask), TRUE);

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);

		while (hSibling)
		{
			// FALSE == not siblings
			UpdateParentStatus(pTasks, hSibling, FALSE);
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

GANTTITEM* CGanttCtrl::GetGanttItem(DWORD dwTaskID) const
{
	return m_data.GetItem(dwTaskID, TRUE);
}

BOOL CGanttCtrl::RestoreGanttItem(const GANTTITEM& giPrev)
{
	if (m_data.RestoreItem(giPrev))
	{
		RecalcParentDates();
		RedrawList();
	
		return TRUE;
	}

	// else
	return FALSE;
}

void CGanttCtrl::RebuildTree(const ITASKLISTBASE* pTasks)
{
	m_tree.DeleteAllItems();
	m_list.DeleteAllItems();
	m_data.RemoveAll();

	m_dwMaxTaskID = 0;

	BuildTreeItem(pTasks, pTasks->GetFirstTask(), NULL, TRUE);

	RefreshItemBoldState();
	ExpandList();
}

void CGanttCtrl::RecalcDateRange()
{
	if (m_data.GetCount())
	{
		RecalcParentDates();
	
		GANTTDATERANGE prevRange = m_dtDataRange;

		m_data.CalcDateRange(HasOption(GTLCF_CALCPARENTDATES),
							HasOption(GTLCF_CALCMISSINGSTARTDATES),
							HasOption(GTLCF_CALCMISSINGDUEDATES), 
							m_dtDataRange);

		if (m_dtDataRange != prevRange)
		{
			ValidateMonthDisplay();
			UpdateListColumns();
		}
	}
}

void CGanttCtrl::BuildTreeItem(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
								HTREEITEM htiParent, BOOL bAndSiblings, BOOL bInsertAtEnd)
{
	if (hTask == NULL)
		return;

	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	ASSERT(!m_data.HasItem(dwTaskID));

	m_dwMaxTaskID = max(m_dwMaxTaskID, dwTaskID);

	// map the data
	GANTTITEM* pGI = new GANTTITEM;
	m_data[dwTaskID] = pGI;
	
	pGI->dwTaskID = dwTaskID;
	pGI->dwRefID = pTasks->GetTaskReferenceID(hTask);
	pGI->nPosition = pTasks->GetTaskPosition(hTask);
	pGI->color = pTasks->GetTaskTextColor(hTask);

	// Only save data for non-references
	if (pGI->dwRefID == 0)
	{
		pGI->sTitle = pTasks->GetTaskTitle(hTask);
		pGI->bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);
		pGI->sAllocTo = GetTaskAllocTo(pTasks, hTask);
		pGI->bParent = pTasks->IsTaskParent(hTask);
		pGI->nPercent = pTasks->GetTaskPercentDone(hTask, TRUE);
		pGI->bLocked = pTasks->IsTaskLocked(hTask, true);
		pGI->bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

		LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
		pGI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));

		time64_t tDate = 0;

		if (pTasks->GetTaskStartDate64(hTask, (pGI->bParent != FALSE), tDate))
			pGI->SetStartDate(tDate, FALSE);

		if (pTasks->GetTaskDueDate64(hTask, (pGI->bParent != FALSE), tDate))
			pGI->SetDueDate(tDate, FALSE);

		if (pTasks->GetTaskDoneDate64(hTask, tDate))
			pGI->SetDoneDate(tDate);

		int nTag = pTasks->GetTaskTagCount(hTask);

		while (nTag--)
			pGI->aTags.Add(pTasks->GetTaskTag(hTask, nTag));

		// Local dependencies only
		int nDepend = pTasks->GetTaskDependencyCount(hTask);
		
		while (nDepend--)
		{	
			DWORD dwTaskID = _ttoi(pTasks->GetTaskDependency(hTask, nDepend));

			if (dwTaskID)
				pGI->aDependIDs.Add(dwTaskID);
		}
	}
	
	// add item to tree
	HTREEITEM htiAfter = TVI_LAST; // default

	if (!bInsertAtEnd)
	{
		// Find the sibling task whose position is one less
		HTREEITEM htiSibling = m_tree.GetChildItem(htiParent);

		while (htiSibling)
		{
			DWORD dwSiblingID = m_tree.GetItemData(htiSibling);
			const GANTTITEM* pGISibling = GetGanttItem(dwSiblingID);
			ASSERT(pGISibling);

			if (pGISibling && (pGISibling->nPosition == (pGI->nPosition - 1)))
			{
				htiAfter = htiSibling;
				break;
			}

			htiSibling = m_tree.GetNextItem(htiSibling, TVGN_NEXT);
		}
	}

	HTREEITEM hti = m_tree.InsertItem(LPSTR_TEXTCALLBACK,
									  I_IMAGECALLBACK,
									  I_IMAGECALLBACK,
									  dwTaskID,
									  htiParent,
									  htiAfter);
	
	// add first child which will add all the rest
	BuildTreeItem(pTasks, pTasks->GetFirstTask(hTask), hti, TRUE);
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildTreeItem(pTasks, hSibling, htiParent, FALSE);
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

void CGanttCtrl::IncrementItemPositions(HTREEITEM htiParent, int nFromPos)
{
	HTREEITEM htiChild = m_tree.GetChildItem(htiParent);

	while (htiChild)
	{
		DWORD dwTaskID = GetTaskID(htiChild);
		GANTTITEM* pGI = NULL;

		GET_GI(dwTaskID, pGI);

		if (pGI->nPosition >= nFromPos)
			pGI->nPosition++;
		
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}
}

BOOL CGanttCtrl::GetActiveDateRange(GANTTDATERANGE& dtRange) const
{
	dtRange = ActiveDateRange();

	return dtRange.IsValid();
}

BOOL CGanttCtrl::GetDataDateRange(GANTTDATERANGE& dtRange) const
{
	dtRange = m_dtDataRange;

	return dtRange.IsValid();
}

BOOL CGanttCtrl::GetMaxDateRange(GANTTDATERANGE& dtRange) const
{
	dtRange = m_dtDataRange;

	return GanttStatic::GetMaxDateRange(dtRange, m_nMonthDisplay, HasOption(GTLCF_DECADESAREZEROBASED));
}

const GANTTDATERANGE& CGanttCtrl::ActiveDateRange() const
{
	if (m_dtActiveRange.IsValid())
		return m_dtActiveRange;

	// else
	return m_dtDataRange;
}

void CGanttCtrl::ClearActiveDateRange()
{
	if (m_dtActiveRange.IsValid())
	{
		m_dtActiveRange.Reset();

		ValidateMonthDisplay();
		UpdateListColumns();
	}
}

BOOL CGanttCtrl::SetActiveDateRange(const GANTTDATERANGE& dtRange)
{
	if (!dtRange.IsValid())
		return FALSE;

	if (m_dtActiveRange == dtRange)
		return TRUE;

	CHoldRedraw hr(m_list);

	if (m_dtDataRange == dtRange)
	{
		m_dtActiveRange.Reset();
	}
	else
	{
		m_dtActiveRange.Set(dtRange);
	}

	ValidateMonthDisplay();
	UpdateListColumns();

	return TRUE;
}

COleDateTime CGanttCtrl::GetStartDate(GTLC_MONTH_DISPLAY nDisplay) const
{
	return ActiveDateRange().GetStart(nDisplay, HasOption(GTLCF_DECADESAREZEROBASED));
}

COleDateTime CGanttCtrl::GetEndDate(GTLC_MONTH_DISPLAY nDisplay) const
{
	return ActiveDateRange().GetEnd(nDisplay, HasOption(GTLCF_DECADESAREZEROBASED));
}

int CGanttCtrl::GetStartYear(GTLC_MONTH_DISPLAY nDisplay) const
{
	return ActiveDateRange().GetStartYear(nDisplay, HasOption(GTLCF_DECADESAREZEROBASED));
}

int CGanttCtrl::GetEndYear(GTLC_MONTH_DISPLAY nDisplay) const
{
	return ActiveDateRange().GetEndYear(nDisplay, HasOption(GTLCF_DECADESAREZEROBASED));
}

int CGanttCtrl::GetNumMonths(GTLC_MONTH_DISPLAY nDisplay) const
{
	return ActiveDateRange().GetNumMonths(nDisplay, HasOption(GTLCF_DECADESAREZEROBASED));
}

void CGanttCtrl::SetOption(DWORD dwOption, BOOL bSet)
{
	if (dwOption)
	{
		DWORD dwPrev = m_dwOptions;

		if (bSet)
			m_dwOptions |= dwOption;
		else
			m_dwOptions &= ~dwOption;

		// specific handling
		if (m_dwOptions != dwPrev)
		{
			switch (dwOption)
			{
			case GTLCF_CALCMISSINGDUEDATES:
			case GTLCF_CALCMISSINGSTARTDATES:
				RecalcDateRange();
				break;

			case GTLCF_STRIKETHRUDONETASKS:
				m_tree.Fonts().Clear();
				InvalidateAll(FALSE);
				break;

			case GTLCF_DECADESAREZEROBASED:
				if ((m_nMonthDisplay == GTLC_DISPLAY_QUARTERCENTURIES) || 
					(m_nMonthDisplay == GTLC_DISPLAY_DECADES))
				{
					UpdateListColumnsWidthAndText();
				}
				break;

			case GTLCF_DISABLEDEPENDENTDRAGGING:
				return;

			case GTLCF_SHOWSPLITTERBAR:
				CTreeListCtrl::SetSplitBarWidth(bSet ? 10 : 0);
				break;

			case GTLCF_DISPLAYISODATES:
				UpdateListColumnsWidthAndText();
				InvalidateAll();
				break;
			}

			if (IsSyncing())
				RedrawList();
		}
	}
}

CString CGanttCtrl::FormatListColumnHeaderText(GTLC_MONTH_DISPLAY nDisplay, int nMonth, int nYear) const
{
	if (nMonth == 0)
		return _T("");

	BOOL bZeroBasedDecades = HasOption(GTLCF_DECADESAREZEROBASED);
	BOOL bISODates = HasOption(GTLCF_DISPLAYISODATES);

	CString sDate;

	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		{
			COleDateTime date(nYear, nMonth, 1, 0, 0, 0);

			int nStartYear = CDateHelper::GetStartOfQuarterCentury(date, bZeroBasedDecades).GetYear();
			int nEndYear = (nStartYear + 24);

			sDate.Format(_T("%d-%d"), nStartYear, nEndYear);
		}
		break;

	case GTLC_DISPLAY_DECADES:
		{
			COleDateTime date(nYear, nMonth, 1, 0, 0, 0);

			int nStartYear = CDateHelper::GetStartOfDecade(date, bZeroBasedDecades).GetYear();
			int nEndYear = (nStartYear + 9);

			sDate.Format(_T("%d-%d"), nStartYear, nEndYear);
		}
		break;

	case GTLC_DISPLAY_YEARS:
		sDate.Format(_T("%d"), nYear);
		break;

	case GTLC_DISPLAY_QUARTERS_SHORT:
		sDate.Format(_T("Q%d %d"), (1 + ((nMonth-1) / 3)), nYear);
		break;

	case GTLC_DISPLAY_QUARTERS_MID:
		sDate.Format(_T("%s-%s %d"), 
			CDateHelper::GetMonthName(nMonth, TRUE),
			CDateHelper::GetMonthName(nMonth+2, TRUE), 
			nYear);
		break;

	case GTLC_DISPLAY_QUARTERS_LONG:
		sDate.Format(_T("%s-%s %d"), 
			CDateHelper::GetMonthName(nMonth, FALSE),
			CDateHelper::GetMonthName(nMonth+2, FALSE), 
			nYear);
		break;

	case GTLC_DISPLAY_MONTHS_SHORT:
		sDate = CDateHelper::FormatDate(COleDateTime(nYear, nMonth, 1, 0, 0, 0), 
			(DHFD_NODAY | DHFD_NOCENTURY | (bISODates ? DHFD_ISO : 0)));
		break;

	case GTLC_DISPLAY_MONTHS_MID:
		sDate.Format(_T("%s %d"), CDateHelper::GetMonthName(nMonth, TRUE), nYear);
		break;

	case GTLC_DISPLAY_MONTHS_LONG:
		sDate.Format(_T("%s %d"), CDateHelper::GetMonthName(nMonth, FALSE), nYear);
		break;

	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		sDate.Format(_T("%s %d (%s)"), CDateHelper::GetMonthName(nMonth, FALSE), nYear, CEnString(IDS_GANTT_WEEKS));
		break;

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
	case GTLC_DISPLAY_HOURS:
		sDate.Format(_T("%s %d (%s)"), CDateHelper::GetMonthName(nMonth, FALSE), nYear, CEnString(IDS_GANTT_DAYS));
		break;

	default:
		ASSERT(0);
		break;
	}

	return sDate;
}

double CGanttCtrl::GetMonthWidth(int nColWidth) const
{
	return GetMonthWidth(m_nMonthDisplay, nColWidth);
}

double CGanttCtrl::GetMonthWidth(GTLC_MONTH_DISPLAY nDisplay, int nColWidth)
{
	return (nColWidth / (double)GetNumMonthsPerColumn(nDisplay));
}

int CGanttCtrl::GetRequiredListColumnCount() const
{
	return GetRequiredListColumnCount(ActiveDateRange(), m_nMonthDisplay);
}

int CGanttCtrl::GetRequiredListColumnCount(const GANTTDATERANGE& dtRange, GTLC_MONTH_DISPLAY nDisplay) const
{
	int nNumCols = GetRequiredColumnCount(dtRange, nDisplay, HasOption(GTLCF_DECADESAREZEROBASED));
	
	// Add a buffer column to accommodate trailing text except
	// where an active date range has been set else we'll always
	// end up with one more columns than that specified
	if (!m_dtActiveRange.IsValid())
		nNumCols++;

	return nNumCols;
}

int CGanttCtrl::GetColumnWidth() const
{
	return GetColumnWidth(m_nMonthDisplay, m_nMonthWidth);
}

int CGanttCtrl::GetColumnWidth(GTLC_MONTH_DISPLAY nDisplay, int nMonthWidth)
{
	return (GetNumMonthsPerColumn(nDisplay) * nMonthWidth);
}

BOOL CGanttCtrl::GetListColumnDate(int nCol, int& nMonth, int& nYear) const
{
	ASSERT (nCol > 0);
	nMonth = nYear = 0;
	
	if (nCol > 0)
	{
		DWORD dwData = m_listHeader.GetItemData(nCol);

		nMonth = LOWORD(dwData);
		nYear = HIWORD(dwData);
	}

	return (nMonth >= 1 && nMonth <= 12);
}

BOOL CGanttCtrl::GetListColumnDate(int nCol, COleDateTime& date, BOOL bEndOfMonth) const
{
	int nMonth, nYear;

	if (!GetListColumnDate(nCol, nMonth, nYear))
		return FALSE;

	date.SetDate(nYear, nMonth, 1);

	if (bEndOfMonth)
		date = CDateHelper::GetEndOfMonth(date);

	return TRUE;
}

void CGanttCtrl::BuildTreeColumns()
{
	// delete existing columns
	while (m_treeHeader.DeleteItem(0));

	// add columns
	m_treeHeader.InsertItem(0, 0, _T("Task"), (HDF_LEFT | HDF_STRING), 0, GTLCC_TITLE);
	m_treeHeader.EnableItemDragging(0, FALSE);

	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		m_treeHeader.InsertItem(nCol + 1, 
								0, 
								CEnString(GANTTTREECOLUMNS[nCol].nIDColName), 
								(GANTTTREECOLUMNS[nCol].nColAlign | HDF_STRING),
								0,
								GANTTTREECOLUMNS[nCol].nColID);
	}
}

void CGanttCtrl::ExpandItem(HTREEITEM hti, BOOL bExpand, BOOL bAndChildren)
{
	// clear pick line first
	ClearDependencyPickLine();

	CTreeListCtrl::ExpandItem(hti, bExpand, bAndChildren);
}

COLORREF CGanttCtrl::GetTreeItemBackColor(HTREEITEM hti, DWORD dwItemData, BOOL bSelected) const
{
	GANTTITEM* pGI = NULL;
	GET_GI_RET(dwItemData, pGI, CLR_NONE);

	COLORREF crBack = CLR_NONE;
	BOOL bColorIsBkgnd = HasOption(GTLCF_TASKTEXTCOLORISBKGND);

	if (pGI->dwOrgRefID)
	{
		const GANTTITEM* pGIRef = m_data.GetItem(pGI->dwOrgRefID, FALSE);
		ASSERT(pGIRef);

		if (pGIRef)
			crBack = pGIRef->GetTextBkColor(bSelected, bColorIsBkgnd);
	}
	else
	{
		crBack = pGI->GetTextBkColor(bSelected, bColorIsBkgnd);
	}

	if (crBack == CLR_NONE)
	{
		if (HasColor(m_crBarDefault) && bColorIsBkgnd)
			crBack = m_crBarDefault;
		else
			crBack = CTreeListCtrl::GetTreeItemBackColor(hti, dwItemData, bSelected);
	}

	return crBack;
}

void CGanttCtrl::DrawTreeItemIcon(CDC* pDC, HTREEITEM /*hti*/, DWORD dwItemData, const CRect& rLabel)
{
	GANTTITEM* pGI = NULL;
	GET_GI(dwItemData, pGI);

	if (pGI->bHasIcon || pGI->bParent)
	{
		int iImageIndex = -1;
		HIMAGELIST hilTask = GetTaskIcon(pGI->dwTaskID, iImageIndex);

		if (hilTask && (iImageIndex != -1))
		{
			CRect rIcon;
			GetTreeIconRect(rLabel, rIcon);

			ImageList_Draw(hilTask, iImageIndex, *pDC, rIcon.left, rIcon.top, ILD_TRANSPARENT);
		}
	}
}

void CGanttCtrl::PostDrawTreeItem(CDC* pDC, HTREEITEM /*hti*/, DWORD dwItemData, const CRect& rLabel)
{
	GANTTITEM* pGI = NULL;
	GET_GI(dwItemData, pGI);

	// Draw shortcut icon for reference tasks
	if (pGI->dwOrgRefID)
	{
		CRect rIcon;
		GetTreeIconRect(rLabel, rIcon);

		// Don't want the shortcut icon centred vertically
		rIcon.bottom = rLabel.bottom;

		GraphicsMisc::DrawShortcutOverlay(pDC, rIcon);
	}
}

HIMAGELIST CGanttCtrl::GetTaskIcon(DWORD dwTaskID, int& iImageIndex) const
{
	if (m_tree.GetImageList(TVSIL_NORMAL) == NULL)
		return NULL;

	return (HIMAGELIST)CWnd::GetParent()->SendMessage(WM_GTLC_GETTASKICON, dwTaskID, (LPARAM)&iImageIndex);
}

GM_ITEMSTATE CGanttCtrl::GetItemState(int nItem) const
{
	if (m_bSavingToImage)
		return GMIS_NONE;

	// else
	return CTreeListCtrl::GetItemState(nItem);
}

GM_ITEMSTATE CGanttCtrl::GetItemState(HTREEITEM hti) const
{
	if (m_bSavingToImage)
		return GMIS_NONE;

	// else
	return CTreeListCtrl::GetItemState(hti);
}

LRESULT CGanttCtrl::OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD)
{
	HWND hwndList = pLVCD->nmcd.hdr.hwndFrom;
	int nItem = (int)pLVCD->nmcd.dwItemSpec;
	
	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
								
	case CDDS_ITEMPREPAINT:
		{
			DWORD dwTaskID = GetTaskID(nItem);

			GANTTITEM* pGI = NULL;
			GET_GI_RET(dwTaskID, pGI, 0L);

			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			
			// draw item bkgnd and gridlines full width of list
			COLORREF crBack = GetRowColor(nItem);
			pLVCD->clrTextBk = pLVCD->clrText = crBack;
			
			CRect rItem;
			VERIFY(m_list.GetItemRect(nItem, rItem, LVIR_BOUNDS));

			CRect rFullWidth(rItem);
			GraphicsMisc::FillItemRect(pDC, rFullWidth, crBack, m_list);
			
			// draw horz gridline before selection
			DrawHorzItemDivider(pDC, rFullWidth);

			// Draw selection before text
			GM_ITEMSTATE nState = GetItemState(nItem);
			GraphicsMisc::DrawExplorerItemSelection(pDC, m_list, nState, rItem, (GMIB_THEMECLASSIC | GMIB_CLIPLEFT | GMIB_PREDRAW | GMIB_POSTDRAW));

			// draw row
			DrawListItem(pDC, nItem, *pGI, (nState != GMIS_NONE));
		}
		return CDRF_SKIPDEFAULT;
								
	case CDDS_POSTPAINT:
		{
			// draw dependencies
			CGanttDependArray aDepends;
			
			if (BuildVisibleDependencyList(aDepends))
			{
				CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

				CRect rClient;
				m_list.GetClientRect(rClient);

				// see if we are currently editing a dependency
				DWORD dwFromTaskID = 0, dwToTaskID = 0;

				if (IsDependencyEditing() && 
					m_pDependEdit->IsPickingToTask() &&
					m_pDependEdit->IsEditing())
				{
					dwFromTaskID = m_pDependEdit->GetFromDependency(dwToTaskID);
				}
				
				int nDepend = aDepends.GetSize();

				while (nDepend--)
				{
					GANTTDEPENDENCY& depend = aDepends[nDepend];

					// don't draw the dependency we are actively editing
					if (!IsDependencyEditing() || !depend.Matches(dwFromTaskID, dwToTaskID))
						depend.Draw(pDC, rClient, FALSE);
				}
			}

			return CDRF_SKIPDEFAULT;
		}
		break;
	}

	return CDRF_DODEFAULT;
}

LRESULT CGanttCtrl::OnHeaderCustomDraw(NMCUSTOMDRAW* pNMCD)
{
	if (pNMCD->hdr.hwndFrom == m_listHeader)
	{
		switch (pNMCD->dwDrawStage)
		{
		case CDDS_PREPAINT:
			// only need handle drawing for double row height
			if (m_listHeader.GetRowCount() > 1)
				return CDRF_NOTIFYITEMDRAW;
							
		case CDDS_ITEMPREPAINT:
			// only need handle drawing for double row height
			if (m_listHeader.GetRowCount() > 1)
			{
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);
				int nItem = (int)pNMCD->dwItemSpec;

				DrawListHeaderItem(pDC, nItem);

				return CDRF_SKIPDEFAULT;
			}
			break;
		}
	}
	else if (pNMCD->hdr.hwndFrom == m_treeHeader)
	{
		switch (pNMCD->dwDrawStage)
		{
		case CDDS_PREPAINT:
			return CDRF_NOTIFYITEMDRAW;
			
		case CDDS_ITEMPREPAINT:
			{
				// don't draw columns having min width
				CRect rItem(pNMCD->rc);
				
				if (rItem.Width() <= MIN_COL_WIDTH)
					return CDRF_DODEFAULT;
			}
			return CDRF_NOTIFYPOSTPAINT;
			
		case CDDS_ITEMPOSTPAINT:
			{
				// draw sort direction
				int nCol = (int)pNMCD->dwItemSpec;
				GTLC_COLUMN nColID = GetTreeColumnID(nCol);
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);
				
				if (m_sort.IsSingleSortingBy(nColID))
					m_treeHeader.DrawItemSortArrow(pDC, nCol, m_sort.single.bAscending);
			}
			break;
		}
	}
	
	return CDRF_DODEFAULT;
}

// Called by parent
void CGanttCtrl::Sort(GTLC_COLUMN nBy, BOOL bAscending)
{
	ASSERT(bAscending != -1);

	Sort(nBy, FALSE, bAscending, FALSE);
}

void CGanttCtrl::Sort(GTLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending, BOOL bNotifyParent)
{
	// clear pick line first
	ClearDependencyPickLine();

	m_sort.Sort(nBy, bAllowToggle, bAscending);

	// do the sort
	CHoldRedraw hr(m_tree);
	CTreeListCtrl::Sort(SortProc, (DWORD)this);

	// update sort arrow
	m_treeHeader.Invalidate(FALSE);

	if (bNotifyParent)
		CWnd::GetParent()->PostMessage(WM_GTLC_NOTIFYSORT, m_sort.single.bAscending, m_sort.single.nBy);
}

void CGanttCtrl::Sort(const GANTTSORTCOLUMNS multi)
{
	// clear pick line first
	ClearDependencyPickLine();

	m_sort.Sort(multi);

	// do the sort
	CHoldRedraw hr(m_tree);
	CTreeListCtrl::Sort(MultiSortProc, (DWORD)this);

	// hide sort arrow
	m_treeHeader.Invalidate(FALSE);
}

void CGanttCtrl::OnBeginEditTreeLabel(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = TRUE; // cancel our edit

	if (m_bReadOnly || IsDependencyEditing())
		return;

	CPoint point(GetMessagePos());
	int nCol = -1;
	HTREEITEM hti = TreeHitTestItem(point, TRUE, nCol);

	if (!hti || (nCol == -1))
		return;

	if (m_treeHeader.GetItemData(nCol) != GTLCC_TITLE)
		return;

	// notify app to edit
	CWnd::GetParent()->SendMessage(WM_GTLC_EDITTASKTITLE, 0, GetTaskID(hti));
}

void CGanttCtrl::OnClickTreeHeader(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	HD_NOTIFY *pHDN = (HD_NOTIFY *)pNMHDR;

	if (pHDN->iButton == 0) // left button
	{
		GTLC_COLUMN nColID = GetTreeColumnID(pHDN->iItem);
		Sort(nColID, TRUE, -1, TRUE);
	}
}

void CGanttCtrl::OnTreeGetDispInfo(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	TV_DISPINFO* pDispInfo = (TV_DISPINFO*)pNMHDR;
	DWORD dwTaskID = pDispInfo->item.lParam;

	const GANTTITEM* pGI = NULL;
	GET_GI(dwTaskID, pGI);

	if (pDispInfo->item.mask & TVIF_TEXT)
	{
		pDispInfo->item.pszText = (LPTSTR)(LPCTSTR)pGI->sTitle;
	}

	if (pDispInfo->item.mask & (TVIF_SELECTEDIMAGE | TVIF_IMAGE))
	{
		// checkbox image
		pDispInfo->item.mask |= TVIF_STATE;
		pDispInfo->item.stateMask = TVIS_STATEIMAGEMASK;

		if (pGI->IsDone(FALSE))
		{
			pDispInfo->item.state = TCHC_CHECKED;
		}
		else if (pGI->bSomeSubtaskDone)
		{
			pDispInfo->item.state = TCHC_MIXED;
		}
		else
		{
			pDispInfo->item.state = TCHC_UNCHECKED;
		}
	}
}

BOOL CGanttCtrl::OnTreeSelectionChange(NMTREEVIEW* pNMTV)
{
	if (!CTreeListCtrl::OnTreeSelectionChange(pNMTV))
		return FALSE;

	if (!m_bMovingItem && HasOption(GTLCF_AUTOSCROLLTOTASK))
		ScrollToSelectedTask();

	return TRUE;
}

UINT CGanttCtrl::OnDragOverItem(const TLCITEMMOVE& move, UINT nCursor)
{
	// We currently DON'T support 'linking'
	if (nCursor == DD_DROPEFFECT_LINK)
	{
		nCursor = DD_DROPEFFECT_NONE;
	}
	else if (nCursor != DD_DROPEFFECT_NONE)
	{
		// Prevent dropping on locked tasks unless references
		DWORD dwTargetID = GetTaskID(move.htiDestParent);

		if (m_data.ItemIsLocked(dwTargetID, TRUE))
			return DD_DROPEFFECT_NONE;

		// If the target is a reference, the source must be a reference
		DWORD dwSelTaskID = GetTaskID(move.htiSel);

		if (m_data.ItemIsReference(dwTargetID) && !m_data.ItemIsReference(dwSelTaskID))
			return DD_DROPEFFECT_NONE;
	}

	return nCursor;
}

BOOL CGanttCtrl::OnDragDropItem(const TLCITEMMOVE& move)
{
	// Prevent dropping on locked tasks unless references
	DWORD dwTargetID = GetTaskID(move.htiDestParent);

	if (dwTargetID && m_data.ItemIsLocked(dwTargetID, TRUE))
		return FALSE;

	// If the target is a reference, the source must be a reference
	DWORD dwSelTaskID = GetTaskID(move.htiSel);

	if (m_data.ItemIsReference(dwTargetID) && !m_data.ItemIsReference(dwSelTaskID))
		return FALSE;

	// Notify parent of move
	IUITASKMOVE taskMove = { 0 };

	taskMove.dwSelectedTaskID = dwSelTaskID;
	taskMove.dwParentID = GetTaskID(move.htiDestParent);
	taskMove.dwAfterSiblingID = GetTaskID(move.htiDestAfterSibling);
	taskMove.bCopy = (move.bCopy != FALSE);

	// If copying a task, app will send us a full update 
	// so we do not need to perform the move ourselves
	if (CWnd::GetParent()->SendMessage(WM_GTLC_MOVETASK, 0, (LPARAM)&taskMove) && !taskMove.bCopy)
	{
		VERIFY(MoveItem(move));
	}

	return TRUE; // prevent base class handling
}

BOOL CGanttCtrl::OnDragBeginItem(const TLCITEMMOVE& move, BOOL bLeftDrag)
{
	if (!CTreeListCtrl::OnDragBeginItem(move, bLeftDrag))
		return FALSE;

	// No restriction on copying
	if (Misc::ModKeysArePressed(MKS_CTRL))
		return TRUE;

	// Prevent dragging of locked tasks
	DWORD dwTaskID = GetTaskID(move.htiSel);
	ASSERT(dwTaskID);

	if (m_data.ItemIsLocked(dwTaskID, TRUE))
		return FALSE;

	// Prevent dragging of subtasks of locked tasks
	HTREEITEM htiParent = m_tree.GetParentItem(move.htiSel);

	if (!htiParent || (htiParent == TVI_ROOT))
		return TRUE;

	DWORD dwParentID = GetTaskID(htiParent);
	ASSERT(dwTaskID);

	if (m_data.ItemIsLocked(dwParentID, TRUE))
		return FALSE;

	return TRUE;
}

void CGanttCtrl::ClearDependencyPickLine(CDC* pDC)
{
	if (IsPickingDependencyToTask() && IsDependencyPickLinePosValid())
	{
		BOOL bNullDC = (pDC == NULL);

		if (bNullDC)
			pDC = m_list.GetDC();
			
		CRect rClient;
		m_list.GetClientRect(rClient);
		
		// calc 'from' point
		DWORD dwFromTaskID = m_pDependEdit->GetFromTask();
		GANTTDEPENDENCY depend;
		
		if (CalcDependencyEndPos(dwFromTaskID, depend, TRUE))
		{
			depend.SetTo(m_ptLastDependPick);
			depend.Draw(pDC, rClient, TRUE);
		}
			
		// clear last drag pos
		ResetDependencyPickLinePos();

		// cleanup
		if (bNullDC)
			m_list.ReleaseDC(pDC);
	}
}

BOOL CGanttCtrl::IsDependencyPickLinePosValid() const
{
	if (IsPickingDependencyToTask()) 
	{
		return ((m_ptLastDependPick.x != DEPENDPICKPOS_NONE) || 
				(m_ptLastDependPick.y != DEPENDPICKPOS_NONE));
	}

	// else
	return FALSE;
}

void CGanttCtrl::ResetDependencyPickLinePos()
{
	m_ptLastDependPick.x = m_ptLastDependPick.y = DEPENDPICKPOS_NONE;
}

BOOL CGanttCtrl::DrawDependencyPickLine(const CPoint& ptClient)
{
	if (IsPickingDependencyToTask())
	{
		// calc 'from ' point
		DWORD dwFromTaskID = m_pDependEdit->GetFromTask();
		GANTTDEPENDENCY depend;

		if (!CalcDependencyEndPos(dwFromTaskID, depend, TRUE))
			return FALSE;

		// calc new 'To' point to see if anything has actually changed

		GTLC_HITTEST nHit = GTLCHT_NOWHERE;
		DWORD dwToTaskID = ListHitTestTask(ptClient, FALSE, nHit, TRUE);
		CPoint ptTo;
		
		if (dwToTaskID && (nHit != GTLCHT_NOWHERE))
		{
			if (!CalcDependencyEndPos(dwToTaskID, depend, FALSE, &ptTo))
				return FALSE;
		}
		else // use current cursor pos
		{
			ptTo = ::GetMessagePos();
			m_list.ScreenToClient(&ptTo);
			
			depend.SetTo(ptTo);
		}

		if (ptTo != m_ptLastDependPick)
		{
			CClientDC dc(&m_list);
			
			CRect rClient;
			m_list.GetClientRect(rClient);
			
			// clear 'old' line
			if (IsDependencyPickLinePosValid())
			{
				depend.SetTo(m_ptLastDependPick);
				depend.Draw(&dc, rClient, TRUE);
			}		
			
			// draw 'new' line
			depend.SetTo(ptTo);
			depend.Draw(&dc, rClient, TRUE);

			// update pos
			m_ptLastDependPick = ptTo;
		}

		return TRUE;
	}

	// else 
	return FALSE;
}

BOOL CGanttCtrl::SetListTaskCursor(DWORD dwTaskID, GTLC_HITTEST nHit) const
{
	if (nHit != GTLCHT_NOWHERE)
	{
		GTLC_DRAG nDrag = MapHitTestToDrag(nHit);
		ASSERT(IsDragging(nDrag));

		if (dwTaskID != 0)
		{
			if (!CanDragTask(dwTaskID, nDrag))
			{
				if (m_data.ItemIsLocked(dwTaskID, FALSE))
					return GraphicsMisc::SetAppCursor(_T("Locked"), _T("Resources\\Cursors"));

				// else
				return GraphicsMisc::SetAppCursor(_T("NoDrag"), _T("Resources\\Cursors"));
			}
			else
			{
				switch (nDrag)
				{
				case GTLCD_START:
				case GTLCD_END:
					return GraphicsMisc::SetStandardCursor(IDC_SIZEWE);
				}
			}
		}
	}

	return FALSE;
}

BOOL CGanttCtrl::OnHeaderDblClkDivider(NMHEADER* pHDN)
{
	if (pHDN->hdr.hwndFrom == m_listHeader)
	{
		int nCol = pHDN->iItem;
		ASSERT(nCol != -1);

		if (nCol > 0) // first column always zero width
			m_listHeader.SetItemWidth(nCol, GetColumnWidth());

		return TRUE; // no default handling
	}

	return CTreeListCtrl::OnHeaderDblClkDivider(pHDN);
}

LRESULT CGanttCtrl::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTreeListCtrl::ScWindowProc(hRealWnd, msg, wp, lp);

	if (hRealWnd == m_list)
	{
		switch (msg)
		{
		case WM_SETCURSOR:
			if (!m_bReadOnly)
			{
				CPoint ptCursor(GetMessagePos());
	
				m_list.ScreenToClient(&ptCursor);
				int nHit = m_list.HitTest(ptCursor);

				if ((nHit != -1) && m_data.ItemIsLocked(GetTaskID(nHit), FALSE))
					return GraphicsMisc::SetAppCursor(_T("Locked"), _T("Resources\\Cursors"));

				if (!IsDependencyEditing())
				{
					GTLC_HITTEST nHit = GTLCHT_NOWHERE;

					DWORD dwHitID = ListHitTestTask(ptCursor, FALSE, nHit, TRUE);
					ASSERT((nHit == GTLCHT_NOWHERE) || (dwHitID != 0));

					if (SetListTaskCursor(dwHitID, nHit))
						return TRUE;
				}
			}
			break;

		case WM_MOUSEWHEEL:
		case WM_MOUSELEAVE:
			ClearDependencyPickLine();
			break;

		case WM_CAPTURECHANGED:
			// if someone else grabs the capture we cancel any drag
			if (lp && (HWND(lp) != hRealWnd) && IsDragging())
			{
				CancelDrag(FALSE);
				return FALSE; // eat
			}
			break;

		case WM_KEYDOWN:
			if (wp == VK_ESCAPE && IsDragging())
			{
				CancelDrag(TRUE);
				return FALSE; // eat
			}
			break;

		case WM_HSCROLL:
			// If we're dragging the 'thumb' and we're double height 
			// then ensure the top row of our header gets redrawn
			if ((LOWORD(wp) == SB_THUMBTRACK) && (m_listHeader.GetRowCount() == 2))
			{
				LRESULT lr = CTreeListCtrl::ScWindowProc(hRealWnd, msg, wp, lp);

				m_listHeader.RedrawRow(0, FALSE, TRUE);

				return lr;
			}
			break;
		}
	}
	else if (hRealWnd == m_tree)
	{
		switch (msg)
		{
		case WM_SETCURSOR:
			if (!m_bReadOnly)
			{
				CPoint ptCursor(GetMessagePos());
				m_tree.ScreenToClient(&ptCursor);

				UINT nFlags = 0;
				HTREEITEM htiHit = m_tree.HitTest(ptCursor, &nFlags);

				if (htiHit)
				{
					if (m_data.ItemIsLocked(GetTaskID(htiHit), TRUE))
						return GraphicsMisc::SetAppCursor(_T("Locked"), _T("Resources\\Cursors"));

					if (nFlags & TVHT_ONITEMICON)
						return GraphicsMisc::SetHandCursor();
				}
			}
			break;
		}
	}

	// else tree or list
	switch (msg)
	{
	case WM_MOUSEWHEEL:
		{
			int zDelta = GET_WHEEL_DELTA_WPARAM(wp);

			if (zDelta != 0)
			{
				WORD wKeys = LOWORD(wp);
				
				if (wKeys == MK_CONTROL)
				{
					// cache prev value
					GTLC_MONTH_DISPLAY nPrevDisplay = m_nMonthDisplay;

					// work out where we are going to scroll to after the zoom
					DWORD dwScrollID = 0;
					COleDateTime dtScroll;

					// centre on the mouse if over the list
					if (hRealWnd == m_list)
					{
						CPoint pt(::GetMessagePos());
						m_list.ScreenToClient(&pt);

						GetDateFromScrollPos((pt.x + m_list.GetScrollPos(SB_HORZ)), dtScroll);
					}
					else // centre on the task beneath the mouse
					{
						dwScrollID = HitTestTask(::GetMessagePos(), true);
					}

					// For reasons I don't understand, the resource context is
					// wrong when handling the mousewheel
					AFX_MANAGE_STATE(AfxGetStaticModuleState());

					// do the zoom
					if (ZoomIn(zDelta > 0) && (m_nMonthDisplay != nPrevDisplay))
					{
						// scroll to area of interest
						if (dwScrollID)
						{
							ScrollToTask(dwScrollID);
						}
						else if (CDateHelper::IsDateSet(dtScroll))
						{
							ScrollTo(dtScroll);
						}
					
						// notify parent
						CWnd::GetParent()->SendMessage(WM_GTLC_NOTIFYZOOM, nPrevDisplay, m_nMonthDisplay);
					}
				}
			}
		}
		break;
	}
	
	return CTreeListCtrl::ScWindowProc(hRealWnd, msg, wp, lp);
}

BOOL CGanttCtrl::OnTreeMouseMove(UINT /*nFlags*/, CPoint point)
{
	if (!m_bReadOnly)
	{
		if (IsPickingDependencyFromTask() || IsPickingDependencyToTask())
		{
			int nHotItem = -1;
			HTREEITEM htiHot = m_tree.HitTest(point);
			
			if (htiHot)
				nHotItem = GetListItem(GetTaskID(htiHot));
			
			SetDropHighlight(htiHot, nHotItem);
			
			// track when the cursor leaves the tree ctrl
			CDialogHelper::TrackMouseLeave(m_tree);

			return TRUE; // eat
		}
	}

	// not handled
	return FALSE;
}

BOOL CGanttCtrl::OnTreeLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bReadOnly)
	{
		if (IsPickingDependencyFromTask())
		{
			DWORD dwFromTaskID = TreeHitTestTask(point, FALSE);
			
			if (dwFromTaskID)
			{
				if (m_data.ItemIsLocked(dwFromTaskID, FALSE))
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
				else if (m_pDependEdit->SetFromTask(dwFromTaskID))
				{
					SetDropHighlight(NULL, -1);
					ResetDependencyPickLinePos();
				}
			}
			
			return TRUE; // eat
		}
		else if (IsPickingDependencyToTask())
		{
			DWORD dwToTaskID = TreeHitTestTask(point, FALSE);
			
			if (dwToTaskID)
				m_pDependEdit->SetToTask(dwToTaskID);
			
			return TRUE; // eat
		}
		else if (IsPickingFromDependency())
		{
			return TRUE; // eat
		}
	}

	return CTreeListCtrl::OnTreeLButtonDown(nFlags, point);
}

BOOL CGanttCtrl::OnTreeLButtonUp(UINT nFlags, CPoint point)
{
	if (CTreeListCtrl::OnTreeLButtonUp(nFlags, point))
		return TRUE;

	// else
	if (!m_bReadOnly)
	{
		HTREEITEM hti = m_tree.HitTest(point, &nFlags);

		if (nFlags & TVHT_ONITEMICON)
			CWnd::GetParent()->SendMessage(WM_GTLC_EDITTASKICON, (WPARAM)m_tree.GetSafeHwnd());
		
		return TRUE; // eat
	}

	// not handled
	return FALSE;
}

BOOL CGanttCtrl::OnTreeCheckChange(HTREEITEM hti)
{
	if (m_bReadOnly)
		return FALSE;

	DWORD dwTaskID = GetTaskID(hti);
	GANTTITEM* pGI = GetGanttItem(dwTaskID);
	ASSERT(pGI);

	if (pGI)
	{
		BOOL bSetDone = !pGI->IsDone(FALSE);

		if (CWnd::GetParent()->SendMessage(WM_GTLC_COMPLETIONCHANGE, (WPARAM)m_tree.GetSafeHwnd(), bSetDone))
		{
			// If the app hasn't already updated this for us we must do it ourselves
			if (pGI->IsDone(FALSE) != bSetDone)
			{
				if (bSetDone)
					pGI->dtDone = COleDateTime::GetCurrentTime();
				else
					CDateHelper::ClearDate(pGI->dtDone);

				m_tree.Invalidate(FALSE);
			}
		}
	}

	return TRUE; // always
}

BOOL CGanttCtrl::OnListMouseMove(UINT /*nFlags*/, CPoint point)
{
	if (!m_bReadOnly)
	{
		if (IsDependencyEditing())
		{
			if (IsPickingDependencyFromTask() || IsPickingDependencyToTask())
			{
				HTREEITEM htiHot = NULL;
				int nHotItem = m_list.HitTest(point), nPrevHotItem = m_nPrevDropHilitedItem;
				
				if (nHotItem != -1)
					htiHot = GetTreeItem(nHotItem);

				SetDropHighlight(htiHot, nHotItem);

				if (IsPickingDependencyToTask())
				{
					// Drop-highlighting does not play well with the 
					// dependency line drawing so we brute force it
					if ((nPrevHotItem != -1) || (nHotItem != -1))
					{
						InvalidateList(nPrevHotItem, nHotItem);
						RedrawList();
						ResetDependencyPickLinePos();
					}

					if (DrawDependencyPickLine(point))
					{
						// track when the cursor leaves the list ctrl
						TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, m_list, 0 };
						TrackMouseEvent(&tme);
					}
				}
			}
		}
		else if (UpdateDragging(point))
		{
			return TRUE; // eat
		}
	}

	// not handled
	return FALSE;
}

BOOL CGanttCtrl::OnListLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bReadOnly)
	{
		if (IsPickingDependencyFromTask())
		{
			int nHit = m_list.HitTest(point);

			if (nHit != -1)
			{
				DWORD dwFromTaskID = GetTaskID(nHit);

				if (m_data.ItemIsLocked(dwFromTaskID, FALSE))
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
				else if (m_pDependEdit->SetFromTask(dwFromTaskID))
				{
					SetDropHighlight(NULL, -1);
					ResetDependencyPickLinePos();
				}
			}

			return TRUE; // eat
		}
		else if (IsPickingFromDependency()) // to edit
		{
			DWORD dwCurToTaskID = 0;
			DWORD dwFromTaskID = ListDependsHitTest(point, dwCurToTaskID);
			
			if (dwFromTaskID && dwCurToTaskID)
			{
				if (m_data.ItemIsLocked(dwFromTaskID, FALSE))
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
				else if (m_pDependEdit->SetFromDependency(dwFromTaskID, dwCurToTaskID))
				{
					ResetDependencyPickLinePos();
					RedrawList();
					DrawDependencyPickLine(point);
				}
			}
			
			return TRUE; // eat
		}
		else if (IsPickingDependencyToTask())
		{
			int nHit = m_list.HitTest(point);

			if (nHit != -1)
			{
				DWORD dwToTaskID = GetTaskID(nHit);

				if (dwToTaskID != m_pDependEdit->GetFromTask())
					m_pDependEdit->SetToTask(dwToTaskID);
			}

			return TRUE; // eat
		}
		else if (StartDragging(point))
		{
			return TRUE; // eat
		}
	}

	// all else
	return CTreeListCtrl::OnListLButtonDown(nFlags, point);
}

BOOL CGanttCtrl::OnListLButtonUp(UINT /*nFlags*/, CPoint point)
{
	if (IsDragging() && EndDragging(point))
	{
		return TRUE; // eat
	}

	// not handled
	return FALSE;
}

BOOL CGanttCtrl::OnListLButtonDblClk(UINT nFlags, CPoint point)
{
	if (IsDependencyEditing())
		return FALSE;

	return CTreeListCtrl::OnListLButtonDblClk(nFlags, point);
}

void CGanttCtrl::SetUITheme(const UITHEME& theme)
{
	SetSplitBarColor(theme.crAppBackDark);

	SetColor(m_crToday, theme.crToday);
	SetColor(m_crWeekend, theme.crWeekend);
	SetColor(m_crNonWorkingHours, theme.crNonWorkingHours);
}

void CGanttCtrl::SetDefaultBarColor(COLORREF crDefault)
{
	SetColor(m_crBarDefault, crDefault);
}

void CGanttCtrl::SetMilestoneTag(const CString& sTag)
{
	if (sTag != m_sMilestoneTag)
	{
		m_sMilestoneTag = sTag;

		if (IsHooked())
			InvalidateAll();
	}
}

void CGanttCtrl::SetParentColoring(GTLC_PARENTCOLORING nOption, COLORREF color)
{
	SetColor(m_crParent, color);

	if (IsHooked() && (m_nParentColoring != nOption))
		InvalidateAll();

	m_nParentColoring = nOption;
}

CString CGanttCtrl::FormatDate(const COleDateTime& date, DWORD dwFlags) const
{
	dwFlags &= ~DHFD_ISO;
	dwFlags |= (HasOption(GTLCF_DISPLAYISODATES) ? DHFD_ISO : 0);

	return CDateHelper::FormatDate(date, dwFlags);
}

CString CGanttCtrl::GetTreeItemColumnText(const GANTTITEM& gi, GTLC_COLUMN nCol) const
{
	CString sItem;

	switch (nCol)
	{
		case GTLCC_TITLE:
			sItem = gi.sTitle;
			break;

		case GTLCC_TASKID:
			if (gi.dwOrgRefID)
				sItem.Format(_T("%ld"), gi.dwOrgRefID);
			else
				sItem.Format(_T("%ld"), gi.dwTaskID);
			break;
			
		case GTLCC_STARTDATE:
		case GTLCC_DUEDATE:
			{
				COleDateTime dtStart, dtDue;
				GetTaskStartEndDates(gi, dtStart, dtDue);

				sItem = FormatDate((nCol == GTLCC_STARTDATE) ? dtStart : dtDue);
			}
			break;

		case GTLCC_DONEDATE:
			if (CDateHelper::IsDateSet(gi.dtDone))
				sItem = FormatDate(gi.dtDone);
			break;

		case GTLCC_ALLOCTO:
			sItem = gi.sAllocTo;
			break;
			
		case GTLCC_PERCENT:
			sItem.Format(_T("%d%%"), gi.nPercent);
			break;
	}

	return sItem;
}

void CGanttCtrl::DrawTreeSubItemText(CDC* pDC, HTREEITEM hti, DWORD dwItemData, int nCol, const CRect& rSubItem, BOOL bSelected)
{
	GANTTITEM* pGI = NULL;
	GET_GI(dwItemData, pGI);

	GTLC_COLUMN nColID = GetTreeColumnID(nCol);
	CString sItem = GetTreeItemColumnText(*pGI, nColID);

	if (!sItem.IsEmpty())
	{
		CRect rText(rSubItem);

		if (nColID == GTLCC_TITLE)
			rText.DeflateRect(2, 2, 1, 0);
		else
			rText.DeflateRect(LV_COLPADDING, 2, LV_COLPADDING, 0);

		HGDIOBJ hFontOld = pDC->SelectObject(GetTreeItemFont(hti, *pGI, nColID));

		// text color and alignment
		BOOL bLighter = FALSE; 
		UINT nFlags = (DT_LEFT | DT_VCENTER | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(m_tree));

		switch (nColID)
		{
		case GTLCC_TITLE:
			nFlags |= DT_END_ELLIPSIS;
			break;

		case GTLCC_TASKID:
			nFlags |= DT_RIGHT;
			break;
			
		case GTLCC_STARTDATE:
		case GTLCC_DUEDATE:
		case GTLCC_DONEDATE:
			{
				// draw non-selected calculated dates lighter
				if (!bSelected && !pGI->IsDone(TRUE))
				{
					bLighter = (pGI->bParent && HasOption(GTLCF_CALCPARENTDATES));

					if (!bLighter)
					{
						if (nColID == GTLCC_STARTDATE)
							bLighter = (!pGI->HasStartDate() && HasOption(GTLCF_CALCMISSINGSTARTDATES));
						else
							bLighter = (!pGI->HasDueDate() && HasOption(GTLCF_CALCMISSINGDUEDATES));
					}
				}
				
				// Right-align if the column width can show the entire date
				// else keep left align to ensure day and month remain visible
				if (rText.Width() >= pDC->GetTextExtent(sItem).cx)
					nFlags |= DT_RIGHT;
			}
			break;
			
		case GTLCC_PERCENT:
			nFlags |= DT_CENTER;
			break;
		}

		COLORREF crText = GetTreeTextColor(*pGI, bSelected, bLighter);
		COLORREF crOldColor = pDC->SetTextColor(crText);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(sItem, rText, nFlags);
		pDC->SetTextColor(crOldColor);
		pDC->SelectObject(hFontOld);
	}
}

CGanttCtrl::VERT_DIV CGanttCtrl::GetListVertDivider(int nMonth, int nYear) const
{
	switch (m_nMonthDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		{
			if (nMonth == 12)
			{
				if (nYear == (GetEndYear(m_nMonthDisplay)))
					return DIV_DARK;

				return DIV_MID;
			}
			else if ((nMonth % 3) == 0)
			{
				return DIV_LIGHT;
			}

			return DIV_NONE;
		}
		break;

	case GTLC_DISPLAY_DECADES:
	case GTLC_DISPLAY_YEARS:
		{
			if (nMonth == 12)
			{
				if (nYear == (GetEndYear(m_nMonthDisplay)))
					return DIV_DARK;

				return DIV_MID;
			}

			// else
			return DIV_LIGHT;
		}
		break;

	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		{
			if (nMonth == 12)
				return DIV_DARK;
			
			if ((nMonth % 3) == 0)
				return DIV_MID;

			// else
			return DIV_LIGHT;
		}
		break;

	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		{
			if (nMonth == 12)
				return DIV_DARK;
	
			if (nMonth == 6)
				return DIV_MID;
		
			// else
			return DIV_LIGHT;
		}
		break;

	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		{
			if (nMonth == 12)
				return DIV_DARK;
			
			// else
			return DIV_MID;
		}
		break;
		
	case GTLC_DISPLAY_HOURS:
		return DIV_DARK;
	}

	ASSERT(0);
	return DIV_NONE;
}

BOOL CGanttCtrl::IsVerticalDivider(VERT_DIV nType)
{
	switch (nType)
	{
	case DIV_LIGHT:
	case DIV_MID:
	case DIV_DARK:
		return TRUE;
	}

	// else
	return FALSE;
}

void CGanttCtrl::DrawListItemVertDivider(CDC* pDC, const CRect& rItem, VERT_DIV nType, BOOL bSelected)
{
	if ((nType == DIV_NONE) || !HasGridlines() || (rItem.right < 0))
		return;

	COLORREF color = m_crGridLine;

	switch (nType)
	{
	case DIV_MID:
		color = GraphicsMisc::Darker(color, 0.25);
		break;

	case DIV_DARK:
		color = GraphicsMisc::Darker(color, 0.5);
		break;
	}

	CTreeListCtrl::DrawVertItemDivider(pDC, rItem, bSelected, color);
}

CString CGanttCtrl::GetLongestVisibleAllocTo(HTREEITEM hti) const
{
	CString sLongest;

	if (hti)
	{
		DWORD dwTaskID = GetTaskID(hti);

		const GANTTITEM* pGI = NULL;
		GET_GI_RET(dwTaskID, pGI, _T(""));

		sLongest = GetTreeItemColumnText(*pGI, GTLCC_ALLOCTO);
	}

	// children
	if (!hti || TCH().IsItemExpanded(hti))
	{
		HTREEITEM htiChild = m_tree.GetChildItem(hti);
		
		while (htiChild)
		{
			CString sLongestChild = GetLongestVisibleAllocTo(htiChild);
			
			if (sLongestChild.GetLength() > sLongest.GetLength())
				sLongest = sLongestChild;
			
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
	
	return sLongest;
}

HFONT CGanttCtrl::GetTreeItemFont(HTREEITEM hti, const GANTTITEM& gi, GTLC_COLUMN nCol)
{
	BOOL bStrikThru = (HasOption(GTLCF_STRIKETHRUDONETASKS) && gi.IsDone(FALSE));
	BOOL bBold = ((nCol == GTLCC_TITLE) && (m_tree.GetParentItem(hti) == NULL));

	return m_tree.Fonts().GetHFont(bBold, FALSE, FALSE, bStrikThru);
}

BOOL CGanttCtrl::GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText) const
{
	if (!CTreeListCtrl::GetTreeItemRect(hti, nCol, rItem, bText))
		return FALSE;

	if (m_bSavingToImage)
		rItem.OffsetRect(-1, 0);

	return TRUE;
}

void CGanttCtrl::DrawListItemYears(CDC* pDC, const CRect& rItem, 
											int nYear, int nNumYears, const GANTTITEM& gi,
											BOOL bSelected, BOOL bRollup, BOOL& bDrawToday)
{
	double dYearWidth = (rItem.Width() / (double)nNumYears);
	CRect rYear(rItem);

	for (int j = 0; j < nNumYears; j++)
	{
		if (j == (nNumYears - 1))
			rYear.right = rItem.right;
		else
			rYear.right = (rItem.left + (int)(dYearWidth * (j + 1)));

		DrawListItemYear(pDC, rYear, (nYear + j), gi, bSelected, bRollup, bDrawToday);

		// next year
		rYear.left = rYear.right; 
	}
}

void CGanttCtrl::DrawListItemYear(CDC* pDC, const CRect& rYear, int nYear, 
											const GANTTITEM& gi, BOOL bSelected, 
											BOOL bRollup, BOOL& bDrawToday)
{
	DrawListItemMonths(pDC, rYear, 1, 12, nYear, gi, bSelected, bRollup, bDrawToday);
}

void CGanttCtrl::DrawListItemMonths(CDC* pDC, const CRect& rItem, 
											int nMonth, int nNumMonths, int nYear, 
											const GANTTITEM& gi, BOOL bSelected, 
											BOOL bRollup, BOOL& bDrawToday)
{
	double dMonthWidth = (rItem.Width() / (double)nNumMonths);
	CRect rMonth(rItem);

	for (int i = 0; i < nNumMonths; i++)
	{
		if ((nMonth + i) == 12)
			rMonth.right = rItem.right;
		else
			rMonth.right = (rItem.left + (int)(dMonthWidth * (i + 1)));

		DrawListItemMonth(pDC, rMonth, (nMonth + i), nYear, gi, bSelected, bRollup, bDrawToday);

		// next item
		rMonth.left = rMonth.right; 
	}
}

void CGanttCtrl::DrawListItemMonth(CDC* pDC, const CRect& rMonth, 
											int nMonth, int nYear, 
											const GANTTITEM& gi, BOOL bSelected, 
											BOOL bRollup, BOOL& bDrawToday)
{
	if (!bRollup)
	{
		VERT_DIV nDiv = GetListVertDivider(nMonth, nYear);
		DrawListItemVertDivider(pDC, rMonth, nDiv, bSelected);

		if (bDrawToday && DrawToday(pDC, rMonth, nMonth, nYear, bSelected))
			bDrawToday = FALSE; // done
	}

	DrawGanttBar(pDC, rMonth, nMonth, nYear, gi);
	DrawGanttDone(pDC, rMonth, nMonth, nYear, gi);
}

void CGanttCtrl::DrawListItemWeeks(CDC* pDC, const CRect& rMonth, 
											int nMonth, int nYear, 
											const GANTTITEM& gi, BOOL bSelected, 
											BOOL bRollup, BOOL& bDrawToday)
{
	// draw vertical week dividers
	int nNumDays = CDateHelper::GetDaysInMonth(nMonth, nYear);
	double dMonthWidth = rMonth.Width();

	int nFirstDOW = CDateHelper::GetFirstDayOfWeek();
	CRect rDay(rMonth);

	COleDateTime dtDay = COleDateTime(nYear, nMonth, 1, 0, 0, 0);

	for (int nDay = 1; nDay <= nNumDays; nDay++)
	{
		rDay.left = rMonth.left + (int)(((nDay - 1) * dMonthWidth) / nNumDays);
		rDay.right = rMonth.left + (int)((nDay * dMonthWidth) / nNumDays);

		// draw divider if visible
		if ((rDay.right > 0) && !bRollup)
		{
			// fill weekends if not selected
			if (!bSelected)
				DrawWeekend(pDC, dtDay, rDay);

			if ((dtDay.GetDayOfWeek() == nFirstDOW) && (nDay > 1))
			{
				rDay.right = rDay.left; // draw at start of day
				DrawListItemVertDivider(pDC, rDay, DIV_LIGHT, bSelected);
			}
		}

		// next day
		dtDay += 1;
	}

	DrawListItemMonth(pDC, rMonth, nMonth, nYear, gi, bSelected, bRollup, bDrawToday);
}

BOOL CGanttCtrl::WantDrawWeekend(const COleDateTime& dtDay) const
{
	COLORREF color = (HasColor(m_crWeekend) ? m_crWeekend : m_crNonWorkingHours);

	return (HasColor(color) && CWeekend().IsWeekend(dtDay));
}

BOOL CGanttCtrl::DrawWeekend(CDC* pDC, const COleDateTime& dtDay, const CRect& rDay)
{
	if (!WantDrawWeekend(dtDay))
		return FALSE;

	// If dtDay is also 'today' and we have a today colour
	// then render using a grey so that it doesn't heavily
	// impact the today colour which will be overlaid on top
	COLORREF color = (HasColor(m_crWeekend) ? m_crWeekend : m_crNonWorkingHours);

	if (HasColor(m_crToday) && CDateHelper::IsToday(dtDay))
		color = RGBX(color).Gray();

	GraphicsMisc::DrawRect(pDC, rDay, color, CLR_NONE, 0, GMDR_NONE, 128);

	return TRUE;
}

void CGanttCtrl::DrawListItemDays(CDC* pDC, const CRect& rMonth, 
											int nMonth, int nYear, const GANTTITEM& gi, 
											BOOL bSelected, BOOL bRollup, 
											BOOL& bDrawToday, BOOL bDrawHours)
{
	// draw vertical day dividers
	if (!bRollup)
	{
		CRect rDay(rMonth);
		COleDateTime dtDay = COleDateTime(nYear, nMonth, 1, 0, 0, 0);

		int nNumDays = CDateHelper::GetDaysInMonth(nMonth, nYear);
		double dDayWidth = (rMonth.Width() / (double)nNumDays);

		for (int nDay = 1; nDay <= nNumDays; nDay++)
		{
			rDay.right = (rMonth.left + (int)(nDay * dDayWidth));

			// only draw visible days
			if (rDay.right > 0)
			{
				BOOL bDrawWeekend = WantDrawWeekend(dtDay);
				BOOL bDrawNonWorkingHours = (!bDrawWeekend && HasColor(m_crNonWorkingHours));

				if (bDrawHours)
				{
					double dHourWidth = (rMonth.Width() / (nNumDays * 24.0));
					
					// draw all but the first and last hours dividers
					CRect rHour(rDay);

					if (HasColor(m_crGridLine))
						rHour.bottom--;

					for (int nHour = 1; nHour < 24; nHour++)
					{
						rHour.right = (rMonth.left + (int)((dDayWidth * (nDay - 1)) + (dHourWidth * nHour)));

						DrawListItemVertDivider(pDC, rHour, DIV_LIGHT, bSelected);
						
						rHour.left = rHour.right;
					}

					if (bDrawNonWorkingHours)
					{
						CWorkingDay wd;
						BOOL bToday = CDateHelper::IsToday(dtDay);

						// Morning
						DrawNonWorkingHours(pDC, rMonth, nDay, bToday, 0.0, wd.GetStartOfDayInHours(), dDayWidth, dHourWidth);

						// Lunch
						DrawNonWorkingHours(pDC, rMonth, nDay, bToday, wd.GetStartOfLunchInHours(), wd.GetEndOfLunchInHours(), dDayWidth, dHourWidth);

						// Afternoon
						DrawNonWorkingHours(pDC, rMonth, nDay, bToday, wd.GetEndOfDayInHours(), 24.0, dDayWidth, dHourWidth);
					}
				}
				
				if (bDrawWeekend)
					DrawWeekend(pDC, dtDay, rDay);

				// draw all but the last day divider
				if (nDay < nNumDays)
					DrawListItemVertDivider(pDC, rDay, (bDrawHours ? DIV_MID : DIV_LIGHT), bSelected);
			}

			// next day
			dtDay.m_dt += 1;
			rDay.left = rDay.right;
		}
	}

	DrawListItemMonth(pDC, rMonth, nMonth, nYear, gi, bSelected, bRollup, bDrawToday);
}

void CGanttCtrl::DrawNonWorkingHours(CDC* pDC, const CRect &rMonth, int nDay, BOOL bToday, 
									 double dFromHour, double dToHour, double dDayWidth, double dHourWidth)
{
	ASSERT(HasColor(m_crNonWorkingHours));

	if (dToHour > dFromHour)
	{
		CRect rNonWorking(rMonth);

		rNonWorking.left = (rMonth.left + (int)((dDayWidth * (nDay - 1)) + (dHourWidth * dFromHour)));
		rNonWorking.right = (rMonth.left + (int)((dDayWidth * (nDay - 1)) + (dHourWidth * dToHour)));

		// If dtDay is also 'today' and we have a today colour
		// then render using a grey so that that it does overly
		// affect the today colour which will be overlaid on top
		COLORREF color = m_crNonWorkingHours;

		if (HasColor(m_crToday) && bToday)
			color = RGBX(color).Gray();

		GraphicsMisc::DrawRect(pDC, rNonWorking, color, CLR_NONE, 0, GMDR_NONE, 128);
	}
}

void CGanttCtrl::DrawListItem(CDC* pDC, int nItem, const GANTTITEM& gi, BOOL bSelected)
{
	ASSERT(nItem != -1);
	int nNumCol = GetRequiredListColumnCount();
	
	// Rollups for collapsed parents
	CRect rClip;
	pDC->GetClipBox(rClip);

	HTREEITEM htiRollUp = NULL;

	if (HasOption(GTLCF_DISPLAYPARENTROLLUPS) && gi.bParent)
	{
		HTREEITEM htiParent = GetTreeItem(nItem);
		ASSERT(htiParent);

		if (htiParent && !TCH().IsItemExpanded(htiParent))
			htiRollUp = htiParent;
	}

	BOOL bContinue = TRUE;

	for (int nCol = 1; ((nCol <= nNumCol) && bContinue); nCol++)
	{
		bContinue = DrawListItemColumn(pDC, nItem, nCol, gi, bSelected, FALSE);

		if (htiRollUp)
		{
			CRect rColumn;
			VERIFY(m_list.GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rColumn));

			if (rColumn.right < rClip.left)
				continue;

			if (rColumn.left > rClip.right)
				break; // we can stop

			DrawListItemRollup(pDC, htiRollUp, nCol, rColumn, bSelected);
		}
	}

	// Trailing text
	if (HasOption(GTLCF_DISPLAYTRAILINGTASKTITLE) || 
		HasOption(GTLCF_DISPLAYTRAILINGALLOCTO))
	{
		CRect rItem;
		VERIFY(m_list.GetItemRect(nItem, rItem, LVIR_BOUNDS));

//		COLORREF crRow = CLR_NONE;//(bSelected ? CLR_NONE : GetRowColor(nItem));

// 		if (htiRollUp)
// 			DrawListItemRollupText(pDC, htiRollUp, rItem, rClip, crRow);
// 		else
			DrawListItemText(pDC, gi, rItem, rClip/*, crRow*/);
	}
}

/*
void CGanttCtrl::DrawListItemRollupText(CDC* pDC, HTREEITEM htiParent, const CRect& rItem, const CRect& rClip, COLORREF crRow)
{
	HTREEITEM htiChild = m_tree.GetChildItem(htiParent);

	while (htiChild)
	{
		if (m_tree.ItemHasChildren(htiChild))
		{
			DrawListItemRollupText(pDC, htiChild, rItem, rClip, crRow); // RECURSIVE CALL
		}
		else
		{
			DWORD dwTaskID = GetTaskID(htiChild);

			GANTTITEM* pGIChild = NULL;
			GET_GI(dwTaskID, pGIChild);

			DrawListItemText(pDC, *pGIChild, rItem, rClip, crRow);
		}

		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}
}
*/

void CGanttCtrl::DrawListItemText(CDC* pDC, const GANTTITEM& gi, const CRect& rItem, const CRect& rClip/*, COLORREF crRow*/)
{
	BOOL bDrawTitle = HasOption(GTLCF_DISPLAYTRAILINGTASKTITLE);
	BOOL bDrawAllocTo = (HasOption(GTLCF_DISPLAYTRAILINGALLOCTO) && !gi.sAllocTo.IsEmpty());

	CString sTrailing;

	if (bDrawTitle && bDrawAllocTo)
	{
		sTrailing.Format(_T("%s (%s)"), gi.sTitle, gi.sAllocTo);
	}
	else if (bDrawTitle)
	{
		sTrailing = gi.sTitle;
	}
	else if (bDrawAllocTo)
	{
		sTrailing = gi.sAllocTo;
	}
	else
	{
		return;
	}

	// get the end pos for this item relative to start of window
	int nTextPos = GetBestTextPos(gi, rItem);

	if (nTextPos == -1)
		return;

	if (!rClip.IsRectNull())
	{
		if (nTextPos > rClip.right)
			return;

		if ((nTextPos + pDC->GetTextExtent(sTrailing).cx + LV_COLPADDING) < rClip.left)
			return;
	}

	CRect rText(rItem);
	rText.left = nTextPos;
	rText.top += 2;

	COLORREF crFill, crBorder;
	GetGanttBarColors(gi, crBorder, crFill);

	HGDIOBJ hFontOld = NULL;

	if (HasOption(GTLCF_STRIKETHRUDONETASKS) && gi.IsDone(FALSE))
		hFontOld = pDC->SelectObject(m_tree.Fonts().GetHFont(FALSE, FALSE, FALSE, TRUE));
	
	pDC->SetBkMode(/*(crRow == CLR_NONE) ?*/ TRANSPARENT /*: OPAQUE*/);
	pDC->SetTextColor(crBorder);
	//pDC->SetBkColor(crRow);
	pDC->DrawText(sTrailing, rText, (DT_LEFT | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(m_list)));

	pDC->SelectObject(hFontOld);
}

void CGanttCtrl::DrawListItemRollup(CDC* pDC, HTREEITEM htiParent, int nCol, const CRect& rColumn, BOOL bSelected)
{
	HTREEITEM htiChild = m_tree.GetChildItem(htiParent);

	while (htiChild)
	{
		if (m_tree.ItemHasChildren(htiChild))
		{
			DrawListItemRollup(pDC, htiChild, nCol, rColumn, bSelected); // RECURSIVE CALL
		}
		else
		{
			DWORD dwTaskID = GetTaskID(htiChild);

			GANTTITEM* pGIChild = NULL;
			GET_GI(dwTaskID, pGIChild);

			DrawListItemColumnRect(pDC, nCol, rColumn, *pGIChild, bSelected, TRUE);
		}

		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}
}

BOOL CGanttCtrl::DrawListItemColumn(CDC* pDC, int nItem, int nCol, const GANTTITEM& gi, 
											BOOL bSelected, BOOL bRollup)
{
	if (nCol == 0)
		return TRUE;

	if (m_listHeader.GetItemWidth(nCol) == 0)
		return TRUE;

	// see if we can avoid drawing this sub-item at all
	CRect rColumn;
	m_list.GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rColumn);

	CRect rClip;
	pDC->GetClipBox(rClip);

	if (rColumn.right < rClip.left)
		return TRUE;
	
	if (rColumn.left > rClip.right)
		return FALSE; // we can stop

	return DrawListItemColumnRect(pDC, nCol, rColumn, gi, bSelected, bRollup);
}

BOOL CGanttCtrl::DrawListItemColumnRect(CDC* pDC, int nCol, const CRect& rColumn, const GANTTITEM& gi, 
												BOOL bSelected, BOOL bRollup)
{
	// get the date range for this column
	int nMonth = 0, nYear = 0;
	
	if (!GetListColumnDate(nCol, nMonth, nYear))
		return FALSE;

	CSaveDC sdc(pDC);

	double dMonthWidth = GetMonthWidth(rColumn.Width());
	BOOL bDrawToday = HasColor(m_crToday);

	// Use higher resolution where possible
	GTLC_MONTH_DISPLAY nCalcDisplay = GetColumnDisplay((int)dMonthWidth);
	BOOL bUseHigherRes = (CompareDisplays(nCalcDisplay, m_nMonthDisplay) > 0);

	switch (m_nMonthDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		DrawListItemYears(pDC, rColumn, nYear, 25, gi, bSelected, bRollup, bDrawToday);
		break;
		
	case GTLC_DISPLAY_DECADES:
		DrawListItemYears(pDC, rColumn, nYear, 10, gi, bSelected, bRollup, bDrawToday);
		break;
		
	case GTLC_DISPLAY_YEARS:
		DrawListItemYear(pDC, rColumn, nYear, gi, bSelected, bRollup, bDrawToday);
		break;
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		DrawListItemMonths(pDC, rColumn, nMonth, 3, nYear, gi, bSelected, bRollup, bDrawToday);
		break;
		
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		if (bUseHigherRes)
			DrawListItemWeeks(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bDrawToday);
		else
			DrawListItemMonth(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bDrawToday);
		break;
		
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		if (bUseHigherRes)
			DrawListItemDays(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bDrawToday, FALSE);
		else
			DrawListItemWeeks(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bDrawToday);
		break;

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		DrawListItemDays(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bDrawToday, bUseHigherRes);
		break;

	case GTLC_DISPLAY_HOURS:
		DrawListItemDays(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bDrawToday, TRUE);
		break;
	
	default:
		ASSERT(0);
		break;
	}

	return TRUE;
}

void CGanttCtrl::DrawListHeaderItem(CDC* pDC, int nCol)
{
	CRect rItem;
	m_listHeader.GetItemRect(nCol, rItem);

	if (nCol == 0)
		return;

	// get the date range for this column
	int nMonth = 0, nYear = 0;
	
	if (!GetListColumnDate(nCol, nMonth, nYear))
		return;

	CSaveDC sdc(pDC);

	double dMonthWidth = GetMonthWidth(rItem.Width());
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(pDC, m_listHeader);
	
	CThemed th;
	BOOL bThemed = (th.AreControlsThemed() && th.Open(&m_listHeader, _T("HEADER")));
	CThemed* pThemed = (bThemed ? &th :NULL);

	CRect rClip;
	pDC->GetClipBox(rClip);

	switch (m_nMonthDisplay)
	{
	case GTLC_DISPLAY_YEARS:
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		// should never get here
		ASSERT(0);
		break;

	case GTLC_DISPLAY_QUARTERCENTURIES:
	case GTLC_DISPLAY_DECADES:
		{
			CRect rRange(rItem), rYear(rItem);
			rYear.top += (rYear.Height() / 2);

			// draw range header
			rRange.bottom = rYear.top;
			DrawListHeaderRect(pDC, rRange, m_listHeader.GetItemText(nCol), pThemed, TRUE);

			// draw year elements
			int nNumYears = ((m_nMonthDisplay == GTLC_DISPLAY_DECADES) ? 10 : 25);
			double dYearWidth = (rRange.Width() / (double)nNumYears);

			rYear.right = rYear.left;

			for (int i = 0; i < nNumYears; i++)
			{
				rYear.left = rYear.right;
				rYear.right = rRange.left + (int)((i + 1) * dYearWidth);

				// check if we can stop
				if (rYear.left > rClip.right)
					break; 

				// check if we need to draw
				if (rYear.right >= rClip.left)
					DrawListHeaderRect(pDC, rYear, Misc::Format(nYear + i), pThemed, FALSE);
			}
		}
		break;
		
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		{
			CRect rMonth(rItem), rWeek(rItem);
			rWeek.top += (rWeek.Height() / 2);

			// draw month header
			rMonth.bottom = rWeek.top;
			DrawListHeaderRect(pDC, rMonth, m_listHeader.GetItemText(nCol), pThemed, FALSE);

			// draw week elements
			int nNumDays = CDateHelper::GetDaysInMonth(nMonth, nYear);
			double dDayWidth = (rMonth.Width() / (double)nNumDays);

			// first week starts at 'First DOW of month'
			OLE_DAYOFWEEK nFirstDOW = CDateHelper::GetFirstDayOfWeek();
			int nDay = CDateHelper::CalcDayOfMonth(nFirstDOW, 1, nMonth, nYear);

			// If this is column 1 (column 0 is hidden) then we might need
			// to draw part of the preceding week
			if ((nCol == 1) && (nDay != -1))
			{
				rWeek.right = (rWeek.left + (int)((nDay - 1) * dDayWidth));
				DrawListHeaderRect(pDC, rWeek, _T(""), pThemed, FALSE);
			}

			// calc number of first week
			COleDateTime dtWeek(nYear, nMonth, nDay, 0, 0, 0);
			int nWeek = CDateHelper::GetWeekofYear(dtWeek);
			BOOL bDone = FALSE;

			while (!bDone)
			{
				rWeek.left = rMonth.left + (int)((nDay - 1) * dDayWidth);

				// if this week bridges into next month this needs special handling
				if ((nDay + 6) > nNumDays)
				{
					// rest of this month
					rWeek.right = rMonth.right;
					
					// plus some of next month
					nDay += (6 - nNumDays);
					nMonth++;
					
					if (nMonth > 12)
					{
						nMonth = 1;
						nYear++;
					}
					
					// Note: width of next month may be different to this month
					if (m_listHeader.GetItemRect(nCol+1, rMonth))
					{
						nNumDays = CDateHelper::GetDaysInMonth(nMonth, nYear);
						dDayWidth = (rMonth.Width() / (double)nNumDays);

						rWeek.right += (int)(nDay * dDayWidth);
					}

					// if this is week 53, check that its not really week 1 of the next year
					if (nWeek == 53)
					{
						ASSERT(nMonth == 1);

						COleDateTime dtWeek(nYear, nMonth, nDay, 0, 0, 0);
						nWeek = CDateHelper::GetWeekofYear(dtWeek);
					}

					bDone = TRUE;
				}
				else 
				{
					rWeek.right = rMonth.left + (int)((nDay + 6) * dDayWidth);
				}

				// check if we can stop
				if (rWeek.left > rClip.right)
					break; 

				// check if we need to draw
				if (rWeek.right >= rClip.left)
					DrawListHeaderRect(pDC, rWeek, Misc::Format(nWeek), pThemed, FALSE);

				// next week
				nDay += 7;
				nWeek++;

				// are we done?
				bDone = (bDone || nDay > nNumDays);
			}
		}
		break;

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
	case GTLC_DISPLAY_HOURS:
		{
			CRect rMonth(rItem), rDay(rItem);
			rDay.top += (rDay.Height() / 2);

			// draw month header
			rMonth.bottom = rDay.top;
			DrawListHeaderRect(pDC, rMonth, m_listHeader.GetItemText(nCol), pThemed, TRUE);

			// draw day elements
			int nNumDays = CDateHelper::GetDaysInMonth(nMonth, nYear);
			double dDayWidth = (rMonth.Width() / (double)nNumDays);

			rDay.right = rDay.left;
			
			for (int nDay = 1; nDay <= nNumDays; nDay++)
			{
				rDay.left = rDay.right;
				rDay.right = rMonth.left + (int)(nDay * dDayWidth);

				// check if we can stop
				if (rDay.left > rClip.right)
					break; 

				// check if we need to draw
				if (rDay.right >= rClip.left)
				{
					CString sHeader;
					COleDateTime dtDay(nYear, nMonth, nDay, 0, 0, 0);

					if (m_nMonthDisplay == GTLC_DISPLAY_HOURS)
					{
						sHeader = FormatDate(dtDay, DHFD_DOW);
					}
					else if (m_nMonthDisplay == GTLC_DISPLAY_DAYS_LONG)
					{
						OLE_DAYOFWEEK nDOW = CDateHelper::GetDayOfWeek(dtDay);

						sHeader.Format(_T("%s/%d"), CDateHelper::GetDayOfWeekName(nDOW, TRUE), nDay);
					}
					else
					{
						sHeader.Format(_T("%d"), nDay);
					}

					// Check if we need to draw today
					LPCRECT prcToday = NULL;
					
					if (bThemed && HasColor(m_crToday) && CDateHelper::IsToday(dtDay))
						prcToday = rDay;

					DrawListHeaderRect(pDC, rDay, sHeader, pThemed, FALSE, prcToday);
				}
			}
		}
		break;
		
	default:
		ASSERT(0);
		break;
	}

	pDC->SelectObject(pOldFont); // not sure if this is necessary but play safe
}

void CGanttCtrl::DrawListHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem, CThemed* pTheme, BOOL bEnsureLabelVisible, LPCRECT prcToday)
{
	if (!pTheme)
	{
		pDC->FillSolidRect(rItem, ::GetSysColor(COLOR_3DFACE));
		pDC->Draw3dRect(rItem, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
	}
	else
	{
		pTheme->DrawBackground(pDC, HP_HEADERITEM, HIS_NORMAL, rItem);

		if (prcToday)
			GraphicsMisc::DrawRect(pDC, prcToday, m_crToday, CLR_NONE, 0, GMDR_NONE, 64);
	}
	
	// text
	if (!sItem.IsEmpty())
	{
		// Special case: We're a 2-row header and the upper text 
		// wants always to be visible regardless of scroll pos
		int nHorzHAlign = DT_CENTER;
		CRect rDraw(rItem);

		if (bEnsureLabelVisible)
		{
			int nHorzPos = m_list.GetScrollPos(SB_HORZ);

			// Get the draw rect in list coords
			rDraw.OffsetRect(-nHorzPos, 0);

			CRect rHeader;
			m_list.GetClientRect(rHeader);

			rDraw.left = max(rDraw.left, 0);
			rDraw.right = min(rDraw.right, rHeader.right);

			// Calc the default position of the text
			int nTextWidth = pDC->GetTextExtent(sItem).cx;

			if (rDraw.Width() < nTextWidth)
				return;

			rDraw.OffsetRect(nHorzPos, 0);
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

		const UINT nFlags = (DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | nHorzHAlign | GraphicsMisc::GetRTLDrawTextFlags(m_listHeader));
		pDC->DrawText(sItem, (LPRECT)(LPCRECT)rDraw, nFlags);
	}
}

BOOL CGanttCtrl::GetTaskStartEndDates(const GANTTITEM& gi, COleDateTime& dtStart, COleDateTime& dtDue) const
{
	return gi.GetStartEndDates(HasOption(GTLCF_CALCPARENTDATES),
								HasOption(GTLCF_CALCMISSINGSTARTDATES),
								HasOption(GTLCF_CALCMISSINGDUEDATES),
								dtStart,
								dtDue);
}

COLORREF CGanttCtrl::GetTreeTextColor(const GANTTITEM& gi, BOOL bSelected, BOOL bLighter) const
{
	COLORREF crText = CLR_NONE;
	BOOL bColorIsBkgnd = (!bSelected && HasOption(GTLCF_TASKTEXTCOLORISBKGND));
	
	if (gi.dwOrgRefID)
	{
		const GANTTITEM* pGIRef = m_data.GetItem(gi.dwOrgRefID, FALSE);
		crText = pGIRef->GetTextColor(bSelected, bColorIsBkgnd);
	}
	else
	{
		crText = gi.GetTextColor(bSelected, bColorIsBkgnd);
	}
	ASSERT(HasColor(crText));

	if (!m_bSavingToImage)
	{
		if (bSelected)
		{
			crText = GraphicsMisc::GetExplorerItemSelectionTextColor(crText, GMIS_SELECTED, GMIB_THEMECLASSIC);
		}
		else if (bLighter)
		{
			crText = GraphicsMisc::Lighter(crText, 0.5);
		}
	}

	return crText;
}

void CGanttCtrl::GetGanttBarColors(const GANTTITEM& gi, COLORREF& crBorder, COLORREF& crFill) const
{
	// darker shade of the item crText/crBack
	COLORREF crDefFill = gi.GetFillColor();
	COLORREF crDefBorder = gi.GetBorderColor();

	if (crDefFill == CLR_NONE)
	{
		if (HasColor(m_crBarDefault) && (!gi.bParent || m_nParentColoring == GTLPC_DEFAULTCOLORING))
		{
			crDefFill = m_crBarDefault;
			crDefBorder = GraphicsMisc::Darker(crDefFill, 0.4);
		}
		else
		{
			crDefFill = GetSysColor(COLOR_WINDOW);
			crDefBorder = GetSysColor(COLOR_WINDOWFRAME);
		}
	}

	if (gi.bParent)
	{
		switch (m_nParentColoring)
		{
		case GTLPC_NOCOLOR:
			crBorder = crDefBorder;
			crFill = CLR_NONE;
			break;

		case GTLPC_SPECIFIEDCOLOR:
			crBorder = GraphicsMisc::Darker(m_crParent, 0.5);
			crFill = m_crParent;
			break;

		case GTLPC_DEFAULTCOLORING:
		default:
			crBorder = crDefBorder;
			crFill = crDefFill;

			if (HasOption(GTLCF_DISPLAYPARENTROLLUPS) && 
				!TCH().IsItemExpanded(GetTreeItem(gi.dwTaskID)))
			{
				crFill = GraphicsMisc::Lighter(crFill, 0.75);
			}
			break;
		}
	}
	else
	{
		crBorder = crDefBorder;
		crFill = crDefFill;
	}
}

BOOL CGanttCtrl::CalcDateRect(const CRect& rMonth, int nMonth, int nYear, 
							const COleDateTime& dtFrom, const COleDateTime& dtTo, CRect& rDate)
{
	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);

	if (nDaysInMonth == 0)
		return FALSE;
	
	COleDateTime dtMonthStart(nYear, nMonth, 1, 0, 0, 0);
	COleDateTime dtMonthEnd = (dtMonthStart.m_dt + nDaysInMonth);

	return CalcDateRect(rMonth, nDaysInMonth, dtMonthStart, dtMonthEnd, dtFrom, dtTo, rDate);
}

BOOL CGanttCtrl::CalcDateRect(const CRect& rMonth, int nDaysInMonth, 
							const COleDateTime& dtMonthStart, const COleDateTime& dtMonthEnd, 
							const COleDateTime& dtFrom, const COleDateTime& dtTo, CRect& rDate)
{
	if (dtFrom > dtTo || dtTo < dtMonthStart || dtFrom > dtMonthEnd)
		return FALSE;

	double dDayWidth = (rMonth.Width() / (double)nDaysInMonth);
	rDate = rMonth;

	if (dtFrom > dtMonthStart)
		rDate.left += (int)((dtFrom.m_dt - dtMonthStart.m_dt) * dDayWidth);

	if (dtTo < dtMonthEnd)
		rDate.right = (rMonth.left + (int)((dtTo.m_dt - dtMonthStart.m_dt) * dDayWidth));

	return ((rDate.right > 0) && (rDate.Width() > 0));
}

DWORD CGanttCtrl::ListDependsHitTest(const CPoint& ptClient, DWORD& dwToTaskID)
{
	CGanttDependArray aDepends;
	
	int nDepend = BuildVisibleDependencyList(aDepends);

	while (nDepend--)
	{
		const GANTTDEPENDENCY& depend = aDepends[nDepend];

		if (depend.HitTest(ptClient))
		{
			dwToTaskID = depend.GetToID();
			return depend.GetFromID();
		}
	}

	return 0;
}

int CGanttCtrl::BuildVisibleDependencyList(CGanttDependArray& aDepends) const
{
	aDepends.RemoveAll();

	CRect rClient;
	m_list.GetClientRect(rClient);

	// iterate all list items checking for dependencies
	HTREEITEM htiFirstVis = m_tree.GetFirstVisibleItem();
	HTREEITEM htiLastVis = TCH().GetLastVisibleItem();

	HTREEITEM htiFrom = htiFirstVis;

	while (htiFrom)
	{
		BuildVisibleDependencyList(htiFrom, aDepends);

		if (htiFrom == htiLastVis)
			break;

		htiFrom = m_tree.GetNextVisibleItem(htiFrom);
	}
	
	return aDepends.GetSize();
}

int CGanttCtrl::BuildVisibleDependencyList(HTREEITEM htiFrom, CGanttDependArray& aDepends) const
{
	DWORD dwFromTaskID = GetTaskID(htiFrom);

	const GANTTITEM* pGIFrom = GetGanttItem(dwFromTaskID);
	ASSERT(pGIFrom);

	if (pGIFrom)
	{
		if (pGIFrom->aDependIDs.GetSize())
		{
			int nDepend = pGIFrom->aDependIDs.GetSize();

			while (nDepend--)
			{
				DWORD dwToTaskID = pGIFrom->aDependIDs[nDepend];

				if (!m_data.HasItem(dwToTaskID))
					continue;

				GANTTDEPENDENCY depend;

				if (BuildDependency(dwFromTaskID, dwToTaskID, depend))
					aDepends.Add(depend);
			}
		}

		if (pGIFrom->bParent && HasOption(GTLCF_DISPLAYPARENTROLLUPS) && !TCH().IsItemExpanded(htiFrom))
		{
			HTREEITEM htiFromChild = m_tree.GetChildItem(htiFrom);

			while (htiFromChild)
			{
				BuildVisibleDependencyList(htiFromChild, aDepends); // RECURSIVE CALL
				htiFromChild = m_tree.GetNextItem(htiFromChild, TVGN_NEXT);
			}
		}
	}

	return aDepends.GetSize();
}

BOOL CGanttCtrl::BuildDependency(DWORD dwFromTaskID, DWORD dwToTaskID, GANTTDEPENDENCY& depend) const
{
	if (!m_data.HasItem(dwFromTaskID) || !m_data.HasItem(dwToTaskID))
	{
		ASSERT(0);
		return FALSE;
	}

	if (CalcDependencyEndPos(dwFromTaskID, depend, TRUE) && 
		CalcDependencyEndPos(dwToTaskID, depend, FALSE))
	{
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CGanttCtrl::CalcDependencyEndPos(DWORD dwTaskID, GANTTDEPENDENCY& depend, BOOL bFrom, LPPOINT lpp) const
{
	ASSERT(m_data.HasItem(dwTaskID));

	int nItem = GetListItem(dwTaskID);

	if (nItem == -1) // Collapsed 
	{
		// Use first visible parent as surrogate
		HTREEITEM hti = GetTreeItem(dwTaskID);
		ASSERT(hti);

		while (!TCH().IsParentItemExpanded(hti))
			hti = m_tree.GetParentItem(hti);

		DWORD dwParentID = GetTaskID(hti);

		nItem = GetListItem(dwParentID);
		ASSERT(nItem != -1);
	}

	return CalcDependencyEndPos(dwTaskID, nItem, depend, bFrom, lpp);
}

BOOL CGanttCtrl::CalcDependencyEndPos(DWORD dwTaskID, int nItem, GANTTDEPENDENCY& depend, BOOL bFrom, LPPOINT lpp) const
{
	if (nItem == -1)
		return FALSE;

	const GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, FALSE);

	CRect rItem, rMilestone;
	VERIFY(GetListItemRect(nItem, rItem));

	int nPos = -1;

	if (CalcMilestoneRect(*pGI, rItem, rMilestone))
	{
		if (bFrom)
			nPos = rMilestone.CenterPoint().x;
		else
			nPos = rMilestone.right;
	}
	else
	{
		COleDateTime dtStart, dtDue;

		if (!GetTaskStartEndDates(*pGI, dtStart, dtDue))
			return FALSE;

		if (!GetDrawPosFromDate((bFrom ? dtStart : dtDue), nPos))
			return FALSE;

		if (pGI->bParent && HasOption(GTLCF_CALCPARENTDATES))
			rItem.bottom -= PARENT_ARROW_SIZE;
	}

	CPoint pt(nPos, ((rItem.top + rItem.bottom) / 2));

	if (bFrom)
	{
		depend.SetFrom(pt, dwTaskID);
	}
	else
	{
		pt.x--;
		depend.SetTo(pt, dwTaskID);
	}

	if (lpp)
		*lpp = pt;

	return TRUE;
}

void CGanttCtrl::DrawGanttBar(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, const GANTTITEM& gi)
{
	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);

	if (nDaysInMonth == 0)
		return;

	COleDateTime dtMonthStart, dtMonthEnd;

	if (!GetMonthDates(nMonth, nYear, dtMonthStart, dtMonthEnd))
		return;

	if (gi.IsMilestone(m_sMilestoneTag))
	{
		DrawGanttMilestone(pDC, rMonth, nMonth, nYear, gi);
		return;
	}

	// else normal bar
	COleDateTime dtStart, dtDue;
	
	if (!GetTaskStartEndDates(gi, dtStart, dtDue))
		return;

	// Move due date to beginning of next day as necessary
	// to avoid rounding errors
	if (CDateHelper::IsEndOfDay(dtDue, FALSE))
		dtDue = CDateHelper::GetStartOfNextDay(dtDue);

	// check for visibility
	CRect rBar(rMonth);

	if (!CalcDateRect(rMonth, nDaysInMonth, dtMonthStart, dtMonthEnd, dtStart, dtDue, rBar))
		return;

	COLORREF crBorder = RGB(0, 0, 0), crFill = CLR_NONE;
	GetGanttBarColors(gi, crBorder, crFill);
	
	// adjust bar height
	rBar.top += GraphicsMisc::ScaleByDPIFactor(2);
	rBar.bottom -= GraphicsMisc::ScaleByDPIFactor(3);

	if (gi.bParent && HasOption(GTLCF_CALCPARENTDATES))
		rBar.bottom -= PARENT_ARROW_SIZE;
	
	// Determine what borders to draw
	DWORD dwBorders = (GMDR_TOP | GMDR_BOTTOM);

	// draw the ends of the border by deflating in width
	// if the date does not extend beyond the month
	if (dtStart >= dtMonthStart)
		dwBorders |= GMDR_LEFT;

	if (dtDue <= dtMonthEnd)
		dwBorders |= GMDR_RIGHT;

	// if parent and no-fill then clear fill colour
	if (gi.bParent && (m_nParentColoring == GTLPC_NOCOLOR))
	{
		crFill = CLR_NONE;
	}
	else if (!gi.IsDone(TRUE) && HasOption(GTLCF_DISPLAYPROGRESSINBAR))
	{
		CRect rProgress(rBar);

		// Handle the fill colour being white ie. having no lighter shade
		COLORREF crProgress = crFill;
		COLORREF crRest = GraphicsMisc::Lighter(crFill, 0.8);

		if (crRest == crProgress)
			crProgress = GraphicsMisc::Darker(crFill, 0.2);

		if (gi.nPercent <= 0)
		{
			// Whole bar is 'non-progress' color
			crFill = crRest;
		}
		else
		{
			// Calculate the % completion date
			double dTaskPercent = (gi.nPercent / 100.0);

			COleDateTime dtPercentDone;
			dtPercentDone.m_dt = (dtStart.m_dt + ((dtDue.m_dt - dtStart.m_dt) * dTaskPercent));

			if (dtPercentDone <= dtMonthStart)
			{
				// Whole bar is 'non-progress' color
				crFill = crRest;
			}
			else if (dtPercentDone >= dtMonthEnd)
			{
				// Whole bar is crFill
				crFill = crProgress;
			}
			else // falls somewhere in between
			{
				double dMonthDuration = (dtMonthEnd.m_dt - dtMonthStart.m_dt);
				double dTaskDuration = (dtDue.m_dt - dtStart.m_dt);

				double dTaskStartOffset = max(0.0, (dtMonthStart.m_dt - dtStart.m_dt));
				double dTaskEndOffset = max(0.0, dtDue.m_dt - dtMonthEnd.m_dt);

				double dTaskRemaining = (dTaskDuration - dTaskStartOffset - dTaskEndOffset);
				double dProportion = (((dTaskDuration * dTaskPercent) - dTaskStartOffset) / dTaskRemaining);

				// Draw the completed part if it's less than the full width
				rProgress.right = (rBar.left + (int)(rBar.Width() * dProportion));

				if (rProgress.right > (rProgress.left + 1))
				{
					// Just fill, no borders
					GraphicsMisc::DrawRect(pDC, rProgress, crProgress, CLR_NONE, 0, 0);

					// Draw the 'non-progress' part
					rProgress.left = rProgress.right;
					rProgress.right = rBar.right;

					GraphicsMisc::DrawRect(pDC, rProgress, crRest, CLR_NONE, 0, 0);
					crFill = CLR_NONE;
				}
				else // no width -> no progress
				{
					crFill = crRest;
				}
			}
		}
	}

	// draw rect with borders
	GraphicsMisc::DrawRect(pDC, rBar, crFill, crBorder, 0, dwBorders);
	
	// for parent items draw downward facing pointers at the ends
	DrawGanttParentEnds(pDC, gi, rBar, dtMonthStart, dtMonthEnd);
}

void CGanttCtrl::DrawGanttParentEnds(CDC* pDC, const GANTTITEM& gi, const CRect& rBar, 
											 const COleDateTime& dtMonthStart, const COleDateTime& dtMonthEnd)
{
	if (!gi.bParent || !HasOption(GTLCF_CALCPARENTDATES))
		return;

	// The parent ends will be obscured by the limiting subtasks
	// so there is no point drawing them
	if (HasOption(GTLCF_DISPLAYPARENTROLLUPS) && !TCH().IsItemExpanded(GetTreeItem(gi.dwTaskID)))
		return;

	COleDateTime dtStart, dtDue;
	GetTaskStartEndDates(gi, dtStart, dtDue);

	BOOL bDrawStart = (dtStart >= dtMonthStart);
	BOOL bDrawEnd = (dtDue <= dtMonthEnd);

	if (bDrawStart || bDrawEnd)
	{
		pDC->SelectObject(GetSysColorBrush(COLOR_WINDOWTEXT));
		pDC->SelectStockObject(NULL_PEN);

		if (bDrawStart)
		{
			POINT pt[3] = 
			{ 
				{ rBar.left, rBar.bottom }, 
				{ rBar.left, rBar.bottom + PARENT_ARROW_SIZE }, 
				{ rBar.left + PARENT_ARROW_SIZE, rBar.bottom } 
			};
		
			pDC->Polygon(pt, 3);
		}
	
		if (bDrawEnd)
		{
			POINT pt[3] = 
			{ 
				{ rBar.right, rBar.bottom }, 
				{ rBar.right, rBar.bottom + PARENT_ARROW_SIZE }, 
				{ rBar.right - PARENT_ARROW_SIZE, rBar.bottom } 
			};
		
			pDC->Polygon(pt, 3);
		}
	}
}

void CGanttCtrl::DrawGanttDone(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, const GANTTITEM& gi)
{
	if (!gi.HasDoneDate(HasOption(GTLCF_CALCPARENTDATES)) || gi.IsMilestone(m_sMilestoneTag))
		return;

	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);

	if (nDaysInMonth == 0)
		return;

	COleDateTime dtMonthStart, dtMonthEnd;

	if (!GetMonthDates(nMonth, nYear, dtMonthStart, dtMonthEnd))
		return;

	if (gi.dtDone < dtMonthStart || gi.dtDone > dtMonthEnd)
		return;

	CRect rDone(rMonth);

	if (!CalcDateRect(rMonth, nDaysInMonth, dtMonthStart, dtMonthEnd, gi.dtDone, gi.dtDone, rDone))
		return;
	
	// draw done date
	COLORREF crBorder, crFill;
	GetGanttBarColors(gi, crBorder, crFill);

	// resize to a square
	rDone.DeflateRect(0, DONE_BOX, 0, DONE_BOX);
	rDone.right = min(rMonth.right, rDone.left + (DONE_BOX / 2));
	rDone.left = rDone.right - rDone.Height();

	pDC->FillSolidRect(rDone, crBorder);
}

void CGanttCtrl::DrawGanttMilestone(CDC* pDC, const CRect& rMonth, int /*nMonth*/, int /*nYear*/, const GANTTITEM& gi)
{
	CRect rMilestone;

	// sanity check
	if (!CalcMilestoneRect(gi, rMonth, rMilestone))
		return;

	// build a polygon
	CPoint ptMid = rMilestone.CenterPoint();

	// draw milestone as diamond
	POINT pt[5] = 
	{ 
		{ rMilestone.left, ptMid.y }, 
		{ ptMid.x, rMilestone.top }, 
		{ rMilestone.right, ptMid.y }, 
		{ ptMid.x, rMilestone.bottom }, 
		{ rMilestone.left, ptMid.y }
	};

	CBrush brFill;
	CPen penBorder;
	CGdiObject* pOldBrush = NULL;
	CGdiObject* poldPen = NULL;
	
	if (gi.HasColor())
	{
		VERIFY(brFill.CreateSolidBrush(gi.GetFillColor()));
		pOldBrush = pDC->SelectObject(&brFill);

		VERIFY(penBorder.CreatePen(PS_SOLID, 1, gi.GetBorderColor()));
		poldPen = pDC->SelectObject(&penBorder);

	}
	else
	{
		pOldBrush = pDC->SelectStockObject(BLACK_BRUSH);
		poldPen = pDC->SelectStockObject(NULL_PEN);
	}
		
	pDC->Polygon(pt, 5);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(poldPen);
}

BOOL CGanttCtrl::CalcMilestoneRect(const GANTTITEM& gi, const CRect& rMonth, CRect& rMilestone) const
{
	if (!gi.IsMilestone(m_sMilestoneTag))
		return FALSE;

	rMilestone = rMonth;

	COleDateTime dtDue;
	
	if (gi.bParent && HasOption(GTLCF_CALCPARENTDATES))
		dtDue = gi.dtMinMaxRange.GetEnd();
	else
		dtDue = gi.dtRange.GetEnd();

	int nEndPos;
	
	if (!GetDrawPosFromDate(dtDue, nEndPos))
		return FALSE;

	// resize to a square
	rMilestone.DeflateRect(0, 3, 0, 4);

	int nHeight = rMilestone.Height();
	rMilestone.left = (nEndPos - (nHeight / 2));
	rMilestone.right = (nEndPos + (nHeight / 2));

	if (rMilestone.right < rMonth.left || rMilestone.left >= rMonth.right)
		return FALSE;
	
	return TRUE;
}

int CGanttCtrl::GetBestTextPos(const GANTTITEM& gi, const CRect& rMonth) const
{
	COleDateTime dtDue;
	
	if (gi.bParent && HasOption(GTLCF_CALCPARENTDATES))
		dtDue = gi.dtMinMaxRange.GetEnd();
	else
		dtDue = gi.dtRange.GetEnd();

	if (!CDateHelper::IsDateSet(dtDue))
	{
		COleDateTime dtUnused;
		GetTaskStartEndDates(gi, dtUnused, dtDue);

		if (!CDateHelper::IsDateSet(dtDue))
			return -1;
	}

	int nPos;

	if (!GetDrawPosFromDate(dtDue, nPos))
		return -1;

	CRect rMilestone;

	if (CalcMilestoneRect(gi, rMonth, rMilestone))
	{
		nPos = max(nPos, rMilestone.right);
	}
	else if (gi.IsDone(FALSE) && (gi.dtDone > dtDue))
	{
		if (!GetDrawPosFromDate(gi.dtDone, nPos))
			return -1;

		nPos += (DONE_BOX / 2);
	}

	return (nPos + 2);
}

BOOL CGanttCtrl::DrawToday(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, BOOL bSelected)
{
	if (m_crToday == CLR_NONE)
		return TRUE; // so we don't keep trying to draw it

	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);

	if (nDaysInMonth == 0)
		return FALSE;

	COleDateTime dtMonthStart, dtMonthEnd;

	if (!GetMonthDates(nMonth, nYear, dtMonthStart, dtMonthEnd))
		return FALSE;

	// draw 'today'
	COleDateTime dtToday = CDateHelper::GetDate(DHD_TODAY);
	
	// check for overlap
	if (dtToday < dtMonthStart || dtToday > dtMonthEnd)
		return FALSE;

	CRect rToday;

	if (!CalcDateRect(rMonth, nDaysInMonth, dtMonthStart, dtMonthEnd, dtToday, dtToday.m_dt + 1.0, rToday))
		return FALSE;

	// don't overdraw selection
	if (bSelected)
		rToday.DeflateRect(0, 1);

	GraphicsMisc::DrawRect(pDC, rToday, m_crToday, CLR_NONE, 0, GMDR_NONE, 128);

	return TRUE;
}

void CGanttCtrl::DeleteItem(HTREEITEM hti)
{
	DWORD dwTaskID = GetTaskID(hti);
	ASSERT(dwTaskID);

	VERIFY(m_tree.DeleteItem(hti));
	VERIFY(m_data.RemoveKey(dwTaskID));
}

BOOL CGanttCtrl::ZoomIn(BOOL bIn)
{
	GTLC_MONTH_DISPLAY nNewDisplay = (bIn ? GetNextDisplay(m_nMonthDisplay) : GetPreviousDisplay(m_nMonthDisplay));
	ASSERT(nNewDisplay != GTLC_DISPLAY_NONE);

	return SetMonthDisplay(nNewDisplay);
}

BOOL CGanttCtrl::SetMonthDisplay(GTLC_MONTH_DISPLAY nNewDisplay)
{
	if (!IsValidDisplay(nNewDisplay))
	{
		ASSERT(0);
		return FALSE;
	}

	// calculate the min month width for this display
	int nMinMonthWidth = GetMinMonthWidth(nNewDisplay);
	
	// Check that the new mode does not exceed the max allowed
	if (!CanSetMonthDisplay(nNewDisplay, nMinMonthWidth))
		return FALSE;

	// adjust the header height where we need to draw days/weeks
	switch (nNewDisplay)
	{
	case GTLC_DISPLAY_YEARS:
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		m_listHeader.SetRowCount(1);
		m_treeHeader.SetRowCount(1);
		break;

	case GTLC_DISPLAY_QUARTERCENTURIES:
	case GTLC_DISPLAY_DECADES:
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
	case GTLC_DISPLAY_HOURS:
		m_listHeader.SetRowCount(2);
		m_treeHeader.SetRowCount(2);
		break;
		
	default:
		ASSERT(0);
		break;
	}

	return ZoomTo(nNewDisplay, max(MIN_MONTH_WIDTH, nMinMonthWidth));
}

BOOL CGanttCtrl::CanSetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay) const
{
	return CanSetMonthDisplay(nDisplay, GetMinMonthWidth(nDisplay));
}

BOOL CGanttCtrl::CanSetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay, int nMonthWidth) const
{
	ASSERT(nMonthWidth > 0);

	if (!IsValidDisplay(nDisplay) || (nMonthWidth < 1))
	{
		ASSERT(0);
		return FALSE;
	}

	// We're only interested in the actual data we need to display
	// rather than the active range which may be much larger
	GANTTDATERANGE dtActual(m_dtActiveRange);

	if (!dtActual.IntersectWith(m_dtDataRange))
		dtActual = m_dtDataRange;

	int nNumReqColumns = GetRequiredListColumnCount(dtActual, nDisplay);
	int nColWidth = GetColumnWidth(nDisplay, nMonthWidth);
	int nTotalReqdWidth = (nNumReqColumns * nColWidth);
	
	return (nTotalReqdWidth <= MAX_HEADER_WIDTH);
}

BOOL CGanttCtrl::ValidateMonthDisplay(GTLC_MONTH_DISPLAY& nDisplay) const
{
	if (!IsValidDisplay(nDisplay))
	{
		ASSERT(0);
		return FALSE;
	}

	int nWidth = GetMinMonthWidth(nDisplay);
	
	return ValidateMonthDisplay(nDisplay, nWidth);
}

BOOL CGanttCtrl::ValidateMonthDisplay(GTLC_MONTH_DISPLAY& nDisplay, int& nMonthWidth) const
{
	if (!IsValidDisplay(nDisplay))
	{
		ASSERT(0);
		return FALSE;
	}

	if (!CanSetMonthDisplay(nDisplay, nMonthWidth))
	{
		// Look backwards until we find a valid display
		GTLC_MONTH_DISPLAY nPrev = nDisplay;
		nDisplay = GetPreviousDisplay(nDisplay);

		while (nDisplay != nPrev)
		{
			nMonthWidth = GetMinMonthWidth(nDisplay);

			if (CanSetMonthDisplay(nDisplay, nMonthWidth))
				return TRUE;

			nPrev = nDisplay;
			nDisplay = GetPreviousDisplay(nDisplay);
		} 

		// We never get here 
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

void CGanttCtrl::ValidateMonthDisplay()
{
	GTLC_MONTH_DISPLAY nPrevDisplay = m_nMonthDisplay, nDisplay = nPrevDisplay;
	int nMonthWidth = m_nMonthWidth;

	VERIFY(ValidateMonthDisplay(nDisplay, nMonthWidth));

	if (m_nMonthDisplay != nDisplay)
	{
		VERIFY(SetMonthDisplay(nDisplay));
		CWnd::GetParent()->SendMessage(WM_GTLC_NOTIFYZOOM, nPrevDisplay, m_nMonthDisplay);
	}
}

BOOL CGanttCtrl::BeginDependencyEdit(IGanttDependencyEditor* pDependEdit)
{
	ASSERT(!m_bReadOnly);
	ASSERT(m_pDependEdit == NULL);
	ASSERT(pDependEdit->IsPicking());
	
	if (!m_bReadOnly && (m_pDependEdit == NULL) && pDependEdit->IsPicking())
	{
		m_pDependEdit = pDependEdit;
		return TRUE;
	}
	
	// else
	return FALSE;
}

void CGanttCtrl::OnEndDepedencyEdit()
{
	m_pDependEdit = NULL;
	
	ResetDependencyPickLinePos();
	SetFocus();

	m_tree.SelectDropTarget(NULL);
	m_list.SetItemState(m_nPrevDropHilitedItem, 0, LVIS_DROPHILITED);
	m_list.Invalidate(TRUE);

	m_nPrevDropHilitedItem = -1;
}

void CGanttCtrl::EndDependencyEdit()
{
	if (IsDependencyEditing())
		m_pDependEdit->Cancel();

	OnEndDepedencyEdit();
}

BOOL CGanttCtrl::IsDependencyEditing() const
{
	return (m_pDependEdit && m_pDependEdit->IsPicking());
}

BOOL CGanttCtrl::IsPickingDependencyFromTask() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingFromTask());
}

BOOL CGanttCtrl::IsPickingFromDependency() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingFromDependency());
}

BOOL CGanttCtrl::IsPickingDependencyToTask() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingToTask());
}

BOOL CGanttCtrl::IsDependencyEditingCancelled() const
{
	return (!m_pDependEdit || m_pDependEdit->IsPickingCancelled());
}

BOOL CGanttCtrl::IsDependencyEditingComplete() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingCompleted());
}

BOOL CGanttCtrl::ZoomTo(GTLC_MONTH_DISPLAY nNewDisplay, int nNewMonthWidth)
{
	// validate new display
	if (!ValidateMonthDisplay(nNewDisplay, nNewMonthWidth))
		return FALSE;

	if ((nNewDisplay == m_nMonthDisplay) && (nNewMonthWidth == m_nMonthWidth))
		return TRUE;

	// cache the scroll-pos at the centre of the view so we can restore it
	CRect rClient;
	CWnd::GetClientRect(rClient);

	COleDateTime dtPos;
	BOOL bRestorePos = GetDateFromScrollPos((m_list.GetScrollPos(SB_HORZ) + (rClient.Width() / 2)), dtPos);

	// always cancel any ongoing operation
	CancelOperation();
	
	// do the update
	CLockUpdates (*this);

	if (nNewDisplay != m_nMonthDisplay)
	{
		int nNewColWidth = GetColumnWidth(nNewDisplay, nNewMonthWidth);

		m_nMonthWidth = nNewMonthWidth;
		m_nMonthDisplay = nNewDisplay;

		UpdateListColumns(nNewColWidth);
	}
	else
	{
		int nCurColWidth = GetColumnWidth();
		int nNewColWidth = GetColumnWidth(m_nMonthDisplay, nNewMonthWidth);

		RecalcListColumnWidths(nCurColWidth, nNewColWidth);

		m_nMonthWidth = (int)GetMonthWidth(nNewColWidth);
	}

	RefreshSize();

	// restore scroll-pos
	if (bRestorePos)
	{
		int nScrollPos;
		
		if (GetScrollPosFromDate(dtPos, nScrollPos))
		{
			// Date was at the centre of the view
			nScrollPos -= (rClient.Width() / 2);
			nScrollPos = max(0, nScrollPos);

			if (nScrollPos)
				ListView_Scroll(m_list, (nScrollPos - m_list.GetScrollPos(SB_HORZ)), 0);
		}
	}

	return TRUE;
}

BOOL CGanttCtrl::ZoomBy(int nAmount)
{
	int nNewMonthWidth = (m_nMonthWidth + nAmount);

	// will this trigger a min-width change?
	GTLC_MONTH_DISPLAY nNewMonthDisplay = GetColumnDisplay(nNewMonthWidth);

	return ZoomTo(nNewMonthDisplay, nNewMonthWidth);
}

void CGanttCtrl::RecalcListColumnWidths(int nFromWidth, int nToWidth)
{
	// resize the required number of columns
	// remember to ignore hidden dummy first column
	int nNumReqColumns = GetRequiredListColumnCount(), i;

	for (i = 1; i <= nNumReqColumns; i++)
	{
		int nWidth = m_listHeader.GetItemWidth(i);

		if (nFromWidth < nToWidth && nWidth < nToWidth)
		{
			m_listHeader.SetItemWidth(i, nToWidth);
		}
		else if (nFromWidth > nToWidth && nWidth > nToWidth)
		{
			m_listHeader.SetItemWidth(i, nToWidth);
		}
	}

	// and zero out the rest
	int nNumCols = m_listHeader.GetItemCount();

	for (; i < nNumCols; i++)
	{
		m_listHeader.EnableItemTracking(i, FALSE);
		m_listHeader.SetItemWidth(i, 0);
	}
}

GTLC_COLUMN CGanttCtrl::GetTreeColumnID(int nCol) const
{
	return (GTLC_COLUMN)m_treeHeader.GetItemData(nCol);
}

void CGanttCtrl::RecalcListColumnsToFit()
{
	// list columns (except first dummy column)
	int nNumCols = GetRequiredListColumnCount();
	
	for (int nCol = 1; nCol <= nNumCols; nCol++)
		m_listHeader.SetItemWidth(nCol, GetColumnWidth());
}

int CGanttCtrl::CalcTreeColumnTextWidth(int nCol, CDC* pDC) const
{
	ASSERT(pDC);

	switch (GetTreeColumnID(nCol))
	{
	case GTLCC_TITLE:
		break; // handled by base class

	case GTLCC_TASKID:
		return pDC->GetTextExtent(Misc::Format(m_dwMaxTaskID)).cx;
		
	case GTLCC_ALLOCTO:
		return GraphicsMisc::GetAverageMaxStringWidth(GetLongestVisibleAllocTo(NULL), pDC);
		
	case GTLCC_STARTDATE:
	case GTLCC_DUEDATE: 
	case GTLCC_DONEDATE: 
		{
			COleDateTime date(2015, 12, 31, 23, 59, 59);
			return pDC->GetTextExtent(FormatDate(date)).cx;
		}
		break;
		
	case GTLCC_PERCENT: 
		return GraphicsMisc::GetAverageMaxStringWidth(_T("100%"), pDC);
	}

	ASSERT(0);
	return 0;
}

void CGanttCtrl::UpdateListColumnsWidthAndText(int nWidth)
{
	CHoldRedraw hr(m_list);
	CHoldRedraw hr2(m_listHeader);

	// first column is always zero width and not trackable
	m_listHeader.SetItemWidth(0, 0);
	m_listHeader.EnableItemTracking(0, FALSE);

	if (nWidth == -1)
		nWidth = GetColumnWidth();

	// remember to include hidden dummy first column
	int nNumReqColumns = (GetRequiredListColumnCount() + 1);
	BOOL bUsePrevWidth = (m_aPrevColWidths.GetSize() == nNumReqColumns);
	int nTotalReqdWidth = 0;

	COleDateTime dtStart = GetStartDate(m_nMonthDisplay);
	int nYear = dtStart.GetYear(), nMonth = dtStart.GetMonth();
	
	int nCol = 1;

	do
	{
		if (nMonth && nYear)
		{
			CString sTitle = FormatListColumnHeaderText(m_nMonthDisplay, nMonth, nYear);
			DWORD dwData = MAKELONG(nMonth, nYear);

			int nColWidth = nWidth;
			BOOL bTracked = FALSE;
			
			if (bUsePrevWidth)
			{
				bTracked = m_aPrevTrackedCols[nCol];

				if (bTracked)
					nColWidth = m_aPrevColWidths[nCol];
			}

			m_listHeader.SetItem(nCol, nColWidth, sTitle, dwData);
			m_listHeader.SetItemTracked(nCol, bTracked);
			m_listHeader.EnableItemTracking(nCol, TRUE);

			nTotalReqdWidth += nColWidth;
		}

		// Next column
		switch (m_nMonthDisplay)
		{
		case GTLC_DISPLAY_QUARTERCENTURIES:
			nYear += 25;
			break;

		case GTLC_DISPLAY_DECADES:
			nYear += 10;
			break;
			
		case GTLC_DISPLAY_YEARS:
			nYear += 1;
			break;
			
		case GTLC_DISPLAY_QUARTERS_SHORT:
		case GTLC_DISPLAY_QUARTERS_MID:
		case GTLC_DISPLAY_QUARTERS_LONG:
			nMonth += 3;

			if (nMonth > 12)
			{
				nMonth = 1;
				nYear += 1;
			}
			break;
			
		case GTLC_DISPLAY_MONTHS_SHORT:
		case GTLC_DISPLAY_MONTHS_MID:
		case GTLC_DISPLAY_MONTHS_LONG:
		case GTLC_DISPLAY_WEEKS_SHORT:
		case GTLC_DISPLAY_WEEKS_MID:
		case GTLC_DISPLAY_WEEKS_LONG:
		case GTLC_DISPLAY_DAYS_SHORT:
		case GTLC_DISPLAY_DAYS_MID:
		case GTLC_DISPLAY_DAYS_LONG:
		case GTLC_DISPLAY_HOURS:
			nMonth += 1;

			if (nMonth > 12)
			{
				nMonth = 1;
				nYear += 1;
			}
			break;
			
		default:
			ASSERT(0);
			break;
		}
		ASSERT(CDateHelper::IsValidDayInMonth(1, nMonth, nYear));
	}
	while (++nCol < nNumReqColumns);

//	TRACE(_T("CGanttTreeListCtrl2(Total Column Widths = %d)\n"), nTotalReqdWidth);

	// for the rest, clear the text and item data and prevent tracking
	int nNumCols = m_listHeader.GetItemCount();

	for (; nCol < nNumCols; nCol++)
	{
		m_listHeader.EnableItemTracking(nCol, FALSE);
		m_listHeader.SetItem(nCol, 0, _T(""), 0);
	}

	// always clear previous width/tracked arrays
	m_aPrevColWidths.RemoveAll();
	m_aPrevTrackedCols.RemoveAll();
}

void CGanttCtrl::BuildListColumns()
{
	// once only
	if (m_listHeader.GetItemCount())
		return;

	// add empty column as placeholder so we can
	// easily replace the other columns without
	// losing all our items too
	LVCOLUMN lvc = { LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, 0 };
	m_list.InsertColumn(0, &lvc);
	
	// add other columns
	int nNumCols = GetRequiredListColumnCount();

	for (int i = 1; i <= nNumCols; i++)
	{
		lvc.cx = 0;
		lvc.fmt = LVCFMT_CENTER | HDF_STRING;
		lvc.pszText = _T("");
		lvc.cchTextMax = 50;

		m_list.InsertColumn(i, &lvc);
	}

	UpdateListColumnsWidthAndText();
}

void CGanttCtrl::UpdateListColumns(int nWidth)
{
	// cache the scrolled position
	int nScrollPos = m_list.GetScrollPos(SB_HORZ);

	COleDateTime dtPos;
	BOOL bRestorePos = GetDateFromScrollPos(nScrollPos, dtPos);

	if (nWidth == -1)
		nWidth = GetColumnWidth();

	int nNumCols = m_listHeader.GetItemCount(); // includes hidden first column
	int nReqCols = (GetRequiredListColumnCount() + 1);
	int nDiffCols = (nReqCols - nNumCols);

	if (nDiffCols > 0)
	{
		// add other columns
		LVCOLUMN lvc = { LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, 0 };

		for (int i = 0, nCol = nNumCols; i < nDiffCols; i++, nCol++)
		{
			lvc.cx = 0;
			lvc.fmt = LVCFMT_CENTER | HDF_STRING;
			lvc.pszText = _T("");
			lvc.cchTextMax = 50;

			m_list.InsertColumn(nCol, &lvc);
		}
	}
	else if (nDiffCols < 0)
	{
		int i = nNumCols;
		
		while (i-- > nReqCols)
		{
			m_list.DeleteColumn(i);
		}
	}
	ASSERT(m_listHeader.GetItemCount() == nReqCols);

	if (nDiffCols != 0)
		PostResize();

	UpdateListColumnsWidthAndText(nWidth);

	// restore scroll-pos
	if (bRestorePos)
	{
		if (GetDrawPosFromDate(dtPos, nScrollPos))
			m_list.Scroll(CSize(nScrollPos, 0));
	}
	else
	{
		m_list.SetScrollPos(SB_HORZ, 0, TRUE);
	}
}

int CGanttCtrl::GetMinMonthWidth(GTLC_MONTH_DISPLAY nDisplay) const
{
	int nWidth = 0;
	VERIFY(m_mapMinMonthWidths.Lookup(nDisplay, nWidth) && (nWidth >= MIN_MONTH_WIDTH));

	return max(nWidth, MIN_MONTH_WIDTH);
}

void CGanttCtrl::CalcMinMonthWidths()
{
	CClientDC dcClient(&m_list);
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(&dcClient, m_list);

	for (int nMode = 0; nMode < NUM_DISPLAYMODES; nMode++)
	{
		GTLC_MONTH_DISPLAY nDisplay = DISPLAYMODES[nMode].nDisplay;
		int nMinMonthWidth = 0;

		switch (nDisplay)
		{
		case GTLC_DISPLAY_QUARTERCENTURIES:
			{
				CString sText = FormatListColumnHeaderText(GTLC_DISPLAY_YEARS, 1, 2013);
				
				int nMinTextWidth = dcClient.GetTextExtent(sText).cx;
				nMinMonthWidth = (nMinTextWidth + COLUMN_PADDING) / 12;
			}
			break;

		case GTLC_DISPLAY_DECADES:
		case GTLC_DISPLAY_YEARS:
			{
				// just increase the width of the preceding display
				GTLC_MONTH_DISPLAY nPrev = DISPLAYMODES[nMode - 1].nDisplay;

				nMinMonthWidth = GetMinMonthWidth(nPrev);
				nMinMonthWidth = (int)(nMinMonthWidth * MULTIYEAR_MULTIPLIER);
			}
			break;
			
		case GTLC_DISPLAY_QUARTERS_SHORT:
			{
				CString sText = FormatListColumnHeaderText(nDisplay, 1, 2013);
				
				int nMinTextWidth = dcClient.GetTextExtent(sText).cx;
				nMinMonthWidth = (nMinTextWidth + COLUMN_PADDING) / 3;
			}
			break;
			
		case GTLC_DISPLAY_QUARTERS_MID:
		case GTLC_DISPLAY_QUARTERS_LONG:
			{
				int nMinTextWidth = 0;
				
				for (int nMonth = 1; nMonth <= 12; nMonth += 3)
				{
					CString sText = FormatListColumnHeaderText(nDisplay, nMonth, 2013);
					
					int nWidth = dcClient.GetTextExtent(sText).cx;
					nMinTextWidth = max(nWidth, nMinTextWidth);
				}
				
				nMinMonthWidth = (nMinTextWidth + COLUMN_PADDING) / 3;
			}
			break;
			
		case GTLC_DISPLAY_MONTHS_SHORT:
		case GTLC_DISPLAY_MONTHS_MID:
		case GTLC_DISPLAY_MONTHS_LONG:
			{
				int nMinTextWidth = 0;
				
				for (int nMonth = 1; nMonth <= 12; nMonth++)
				{
					CString sText = FormatListColumnHeaderText(nDisplay, nMonth, 2013);
					
					int nTextWidth = dcClient.GetTextExtent(sText).cx;
					nMinTextWidth = max(nTextWidth, nMinTextWidth);
				}
				
				nMinMonthWidth = (nMinTextWidth + COLUMN_PADDING);
			}
			break;
			
		case GTLC_DISPLAY_WEEKS_SHORT:
		case GTLC_DISPLAY_WEEKS_MID:
		case GTLC_DISPLAY_WEEKS_LONG:
		case GTLC_DISPLAY_DAYS_SHORT:
		case GTLC_DISPLAY_DAYS_MID:
			{
				// just increase the width of the preceding display
				GTLC_MONTH_DISPLAY nPrev = DISPLAYMODES[nMode - 1].nDisplay;

				nMinMonthWidth = GetMinMonthWidth(nPrev);
				nMinMonthWidth = (int)(nMinMonthWidth * DAY_WEEK_MULTIPLIER);
			}
			break;

		case GTLC_DISPLAY_DAYS_LONG:
			{
				// increase the width of the preceding display
				GTLC_MONTH_DISPLAY nPrev = DISPLAYMODES[nMode - 1].nDisplay;

				nMinMonthWidth = GetMinMonthWidth(nPrev);
				nMinMonthWidth = (int)(nMinMonthWidth * DAY_WEEK_MULTIPLIER);

				int nTextWidth = (dcClient.GetTextExtent(_T("/31")).cx + CDateHelper::GetMaxDayOfWeekNameWidth(&dcClient, TRUE));
				nMinMonthWidth = max((nTextWidth * 31), nMinMonthWidth);
			}
			break;
			
		case GTLC_DISPLAY_HOURS:
			{
				// just increase the width of the preceding display
				GTLC_MONTH_DISPLAY nPrev = DISPLAYMODES[nMode - 1].nDisplay;

				nMinMonthWidth = GetMinMonthWidth(nPrev);
				nMinMonthWidth = (int)(nMinMonthWidth * HOUR_DAY_MULTIPLIER);
			}
			break;
			
		default:
			ASSERT(0);
			break;
		}

		if (nMinMonthWidth > 0)
		{
			nMinMonthWidth++; // for rounding
			m_mapMinMonthWidths[nDisplay] = nMinMonthWidth; 
		}
	}

	dcClient.SelectObject(pOldFont);
}

GTLC_MONTH_DISPLAY CGanttCtrl::GetColumnDisplay(int nMonthWidth)
{
	int nMinWidth = 0;

	for (int nMode = 0; nMode < (NUM_DISPLAYMODES - 1); nMode++)
	{
		GTLC_MONTH_DISPLAY nDisplay = DISPLAYMODES[nMode].nDisplay;
		GTLC_MONTH_DISPLAY nNext = DISPLAYMODES[nMode + 1].nDisplay;

		int nFromWidth = GetMinMonthWidth(nDisplay);
		int nToWidth = GetMinMonthWidth(nNext);

		if ((nMonthWidth >= nFromWidth) && (nMonthWidth < nToWidth))
			return nDisplay;
	}

	return GetLastDisplay();
}

int CALLBACK CGanttCtrl::MultiSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const CGanttCtrl* pThis = (CGanttCtrl*)lParamSort;
	const GANTTSORTCOLUMNS& sort = pThis->m_sort.multi;

	int nCompare = 0;

	for (int nCol = 0; ((nCol < 3) && (nCompare == 0)); nCol++)
	{
		if (sort.cols[nCol].nBy == TDCA_NONE)
			break;

		nCompare = pThis->CompareTasks(lParam1, lParam2, sort.cols[nCol]);
	}

	return nCompare;
}

int CALLBACK CGanttCtrl::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const CGanttCtrl* pThis = (CGanttCtrl*)lParamSort;
	
	return pThis->CompareTasks(lParam1, lParam2, pThis->m_sort.single);
}

int CGanttCtrl::CompareTasks(DWORD dwTaskID1, DWORD dwTaskID2, const GANTTSORTCOLUMN& col) const
{
	int nCompare = 0;

	// Optimise for task ID
	if (col.nBy == GTLCC_TASKID)
	{
		nCompare = (dwTaskID1 - dwTaskID2);
	}
	else
	{
		const GANTTITEM* pGI1 = GetGanttItem(dwTaskID1);
		const GANTTITEM* pGI2 = GetGanttItem(dwTaskID2);

		if (!pGI1 || !pGI2)
		{
			ASSERT(0);
			return 0;
		}

		switch (col.nBy)
		{
		case GTLCC_TITLE:
			nCompare = Compare(pGI1->sTitle, pGI2->sTitle);
			break;

		case GTLCC_STARTDATE:
			{
				COleDateTime dtStart1, dtStart2, dtUnused;

				GetTaskStartEndDates(*pGI1, dtStart1, dtUnused);
				GetTaskStartEndDates(*pGI2, dtStart2, dtUnused);

				nCompare = CDateHelper::Compare(dtStart1, dtStart2, DHC_COMPARETIME);
			}
			break;

		case GTLCC_DUEDATE:
			{
				COleDateTime dtDue1, dtDue2, dtUnused;

				GetTaskStartEndDates(*pGI1, dtUnused, dtDue1);
				GetTaskStartEndDates(*pGI2, dtUnused, dtDue2);


				nCompare = CDateHelper::Compare(dtDue1, dtDue2, (DHC_COMPARETIME | DHC_NOTIMEISENDOFDAY));
			}
			break;

		case GTLCC_DONEDATE:
			nCompare = CDateHelper::Compare(pGI1->dtDone, pGI2->dtDone, (DHC_COMPARETIME | DHC_NOTIMEISENDOFDAY));
			break;

		case GTLCC_ALLOCTO:
			nCompare = Compare(pGI1->sAllocTo, pGI2->sAllocTo);
			break;

		case GTLCC_PERCENT:
			nCompare = (pGI1->nPercent - pGI2->nPercent);
			break;

		case GTLCC_NONE:
			nCompare = (pGI1->nPosition - pGI2->nPosition);
			break;

		case GTLCC_TAGS:
			{
				CString sTags1 = Misc::FormatArray(pGI1->aTags);
				CString sTags2 = Misc::FormatArray(pGI2->aTags);
		
				nCompare = Misc::NaturalCompare(sTags1, sTags2, TRUE);
			}
			break;

		case GTLCC_DEPENDENCY:
			{
				// If Task2 is dependent on Task1 then Task1 comes first
				if (m_data.IsItemDependentOn(*pGI2, dwTaskID1))
				{
					nCompare = -1;
				}
				// else if Task1 is dependent on Task2 then Task2 comes first
				else if (m_data.IsItemDependentOn(*pGI1, dwTaskID2))
				{
					nCompare = 1;
				}
			}
			break;

		default:
			ASSERT(0);
			break;
		}
	}

	return (col.bAscending ? nCompare : -nCompare);
}

void CGanttCtrl::ScrollToToday()
{
	ScrollTo(COleDateTime::GetCurrentTime());
}

void CGanttCtrl::ScrollToSelectedTask()
{
	ScrollToTask(GetSelectedTaskID());
}

void CGanttCtrl::ScrollToTask(DWORD dwTaskID)
{
	GANTTITEM* pGI = NULL;
	GET_GI(dwTaskID, pGI);
	
	// Don't scroll if any part of the task is already visible
	GANTTDATERANGE dtVis;
	VERIFY(GetVisibleDateRange(dtVis));

	COleDateTime dtStart, dtDue;
	
	if (GetTaskStartEndDates(*pGI, dtStart, dtDue))
	{
		if (!dtVis.HasIntersection(COleDateTimeRange(dtStart, dtDue)))
			ScrollTo(pGI->IsMilestone(m_sMilestoneTag) ? dtDue : dtStart);
	}
	else if (pGI->HasDoneDate(HasOption(GTLCF_CALCPARENTDATES)))
	{
		if (!dtVis.Contains(pGI->dtDone))
			ScrollTo(pGI->dtDone);
	}
}

void CGanttCtrl::ScrollTo(const COleDateTime& date)
{
	ClearDependencyPickLine();

	COleDateTime dateOnly = CDateHelper::GetDateOnly(date);

	int nStartPos, nEndPos;
	
	if (GetScrollPosFromDate(dateOnly, nStartPos) &&
		GetScrollPosFromDate(dateOnly.m_dt + 1.0, nEndPos))
	{
		// if it is already visible no need to scroll
		int nListStart = m_list.GetScrollPos(SB_HORZ);

		CRect rList;
		m_list.GetClientRect(rList);

		if ((nStartPos >= nListStart) && (nEndPos <= (nListStart + rList.Width())))
			return;

		// deduct current scroll pos for relative move
		nStartPos -= nListStart;

		// and arbitrary offset
		nStartPos -= 50;

		if (m_list.Scroll(CSize(nStartPos, 0)))
			InvalidateAll(FALSE);
	}
}

BOOL CGanttCtrl::GetVisibleDateRange(GANTTDATERANGE& dtRange) const
{
	dtRange = ActiveDateRange();

	if (m_list.GetStyle() & WS_HSCROLL)
	{
		CRect rList;
		m_list.GetClientRect(rList);

		rList.OffsetRect(m_list.GetScrollPos(SB_HORZ), 0);

		COleDateTime dtStart, dtEnd;

		if (GetDateFromScrollPos(rList.left, dtStart) &&
			GetDateFromScrollPos(rList.right - 1, dtEnd))
		{
			dtRange.Set(COleDateTimeRange(dtStart, dtEnd, FALSE));
		}
	}

	return dtRange.IsValid();
}

BOOL CGanttCtrl::GetDateFromScrollPos(int nScrollPos, GTLC_MONTH_DISPLAY nDisplay, int nMonth, int nYear, const CRect& rColumn, COleDateTime& date)
{
	CRect rMonth(rColumn);

	// Clip 'rColumn' if displaying more than months
	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
	case GTLC_DISPLAY_DECADES:
	case GTLC_DISPLAY_YEARS:
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		{
			double dMonthWidth = GetMonthWidth(nDisplay, rMonth.Width());

			// calc month as offset to start of column
			int nPxOffset = (nScrollPos - rMonth.left);
			int nMonthOffset = (int)(nPxOffset / dMonthWidth);

			// clip rect to this month
			rMonth.left += nPxOffset;
			rMonth.right = (rMonth.left + (int)dMonthWidth);

			nMonth += nMonthOffset;

			// Months here are one-based
			nYear += ((nMonth - 1) / 12);
			nMonth = (((nMonth - 1) % 12) + 1);
		}
		break;
	}

	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);
	int nNumMins = MulDiv((nScrollPos - rMonth.left), (60 * 24 * nDaysInMonth), rMonth.Width());

	int nDay = (1 + (nNumMins / MINS_IN_DAY));
	int nHour = ((nNumMins % MINS_IN_DAY) / MINS_IN_HOUR);
	int nMin = (nNumMins % MINS_IN_HOUR);

	ASSERT(nDay >= 1 && nDay <= nDaysInMonth);
	ASSERT(nHour >= 0 && nHour < 24);

	date.SetDateTime(nYear, nMonth, nDay, nHour, nMin, 0);

	return CDateHelper::IsDateSet(date);
}

BOOL CGanttCtrl::GetDateFromScrollPos(int nScrollPos, COleDateTime& date) const
{
	// find the column containing this scroll pos
	int nCol = FindColumn(nScrollPos);

	if (nCol == -1)
		return FALSE;

	// else
	CRect rColumn;
	VERIFY(GetListColumnRect(nCol, rColumn, FALSE));
	ASSERT(nScrollPos >= rColumn.left && nScrollPos < rColumn.right);

	int nYear, nMonth;
	VERIFY(GetListColumnDate(nCol, nMonth, nYear));

	return GetDateFromScrollPos(nScrollPos, m_nMonthDisplay, nMonth, nYear, rColumn, date);
}

BOOL CGanttCtrl::GetDrawPosFromDate(const COleDateTime& date, int& nPos) const
{
	if (!GetScrollPosFromDate(date, nPos))
		return FALSE;
	
	nPos -= m_list.GetScrollPos(SB_HORZ);
	return TRUE;
}

BOOL CGanttCtrl::GetScrollPosFromDate(const COleDateTime& date, int& nPos) const
{
	// figure out which column contains 'date'
	int nCol = FindColumn(date);

	if (nCol != -1)
	{
		CRect rColumn;

		if (GetListColumnRect(nCol, rColumn, FALSE))
		{
			int nDay = date.GetDay();
			int nMonth = date.GetMonth();
			int nYear = date.GetYear();

			double dDayInCol = 0;
			int nDaysInCol = 0;

			switch (m_nMonthDisplay)
			{
			case GTLC_DISPLAY_QUARTERCENTURIES:
				{
					// Column == 25 years
					nDaysInCol = (int)(DAYS_IN_YEAR * 25);
					int nStartYear = CDateHelper::GetStartOfQuarterCentury(date, HasOption(GTLCF_DECADESAREZEROBASED)).GetYear();
					dDayInCol = (int)(((nYear - nStartYear) * DAYS_IN_YEAR) + ((nMonth - 1) * DAYS_IN_MONTH) + nDay);
				}
				break;

			case GTLC_DISPLAY_DECADES:
				{
					// Column == 10 years
					nDaysInCol = (int)(DAYS_IN_YEAR * 10);
					int nStartYear = CDateHelper::GetStartOfDecade(date, HasOption(GTLCF_DECADESAREZEROBASED)).GetYear();
					dDayInCol = (int)(((nYear - nStartYear) * DAYS_IN_YEAR) + ((nMonth - 1) * DAYS_IN_MONTH) + nDay);
				}
				break;

			case GTLC_DISPLAY_YEARS:
				{
					// Column == 12 months
					nDaysInCol = (int)DAYS_IN_YEAR;
					dDayInCol = ((int)((nMonth - 1) * DAYS_IN_MONTH) + nDay);
				}
				break;
				
			case GTLC_DISPLAY_QUARTERS_SHORT:
			case GTLC_DISPLAY_QUARTERS_MID:
			case GTLC_DISPLAY_QUARTERS_LONG:
				{
					// Column == 3 months
					nDaysInCol = (int)(DAYS_IN_MONTH * 3);
					dDayInCol = ((int)(((nMonth - 1) % 3) * DAYS_IN_MONTH) + nDay);
				}
				break;

			default: 
				{
					// Column == Month
					nDaysInCol = CDateHelper::GetDaysInMonth(nMonth, nYear);
					dDayInCol = ((nDay - 1) + CDateHelper::GetTimeOnly(date));
				}
				break;
			}

			ASSERT(nDaysInCol > 0);

			if (nDaysInCol > 0)
			{
				double dOffset = ((rColumn.Width() * dDayInCol) / nDaysInCol);
				nPos = (rColumn.left + (int)dOffset);

				return TRUE;
			}
		}
	}

	// else
	return FALSE;
}

int CGanttCtrl::FindColumn(int nMonth, int nYear) const
{	
	int nMonths = CDateHelper::GetDateInMonths(nMonth, nYear);
	int nNumReqColumns = GetRequiredListColumnCount();

	for (int i = 1; i <= nNumReqColumns; i++)
	{
		// get date for current column
		VERIFY (GetListColumnDate(i, nMonth, nYear));

		int nColMonths = CDateHelper::GetDateInMonths(nMonth, nYear);

		if (nMonths >= nColMonths)
		{
			if (i == nNumReqColumns) // last column
			{
				int nColEndMonths = (nColMonths + GetNumMonthsPerColumn(m_nMonthDisplay) - 1);

				return ((nMonths <= nColEndMonths) ? i : -1);
			}
			else // get date for next column
			{
				VERIFY (GetListColumnDate(i+1, nMonth, nYear));
				
				if (nMonths < CDateHelper::GetDateInMonths(nMonth, nYear))
				{
					return i;
				}
			}
		}
	}

	// not found
	return -1;
}

int CGanttCtrl::FindColumn(const COleDateTime& date) const
{
	return FindColumn(date.GetMonth(), date.GetYear());
}

int CGanttCtrl::FindColumn(int nScrollPos) const
{
	int nNumReqColumns = GetRequiredListColumnCount();

	for (int i = 1; i <= nNumReqColumns; i++)
	{
		CRect rColumn;
		VERIFY(GetListColumnRect(i, rColumn, FALSE));

		if ((nScrollPos >= rColumn.left) && (nScrollPos < rColumn.right))
			return i;
	}

	// not found
	return -1;
}

bool CGanttCtrl::PrepareNewTask(ITaskList* pTaskList) const
{
	ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return false;
	}

	// Set the start date to today and of duration 1 day
	HTASKITEM hNewTask = pTasks->GetFirstTask();
	ASSERT(hNewTask);

	COleDateTime dt = CDateHelper::GetDate(DHD_TODAY);
	time64_t tDate = 0;

	if (CDateHelper::GetTimeT64(dt, tDate))
	{
		pTasks->SetTaskStartDate64(hNewTask, tDate);
		pTasks->SetTaskDueDate64(hNewTask, tDate);
	}

	return true;
}

DWORD CGanttCtrl::HitTestTask(const CPoint& ptScreen, bool bTitleColumnOnly) const
{
	HTREEITEM htiHit = HitTestItem(ptScreen, bTitleColumnOnly);

	if (htiHit)
		return GetTaskID(htiHit);

	return 0;
}

DWORD CGanttCtrl::TreeHitTestTask(const CPoint& ptScreen, BOOL bScreen) const
{
	HTREEITEM htiHit = TreeHitTestItem(ptScreen, bScreen);
	
	if (htiHit)
		return GetTaskID(htiHit);

	return 0;
}

BOOL CGanttCtrl::GetListItemRect(int nItem, CRect& rItem) const
{
	if (m_list.GetItemRect(nItem, rItem, LVIR_BOUNDS))
	{
		// Extend to end of client rect
		CRect rClient;
		CWnd::GetClientRect(rClient);

		rItem.right = max(rItem.right, rClient.right);
		return TRUE;
	}

	return FALSE;
}

DWORD CGanttCtrl::ListHitTestTask(const CPoint& point, BOOL bScreen, GTLC_HITTEST& nHit, BOOL bDragging) const
{
	nHit = GTLCHT_NOWHERE;

	if (m_data.GetCount() == 0)
		return 0;

	int nCol, nItem = ListHitTestItem(point, bScreen, nCol);

	if (nItem == -1)
		return 0;

	// get task item and see where we hit
	DWORD dwTaskID = GetTaskID(nItem);
	
	GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, 0);
	
	// No dragging on auto-calculated parent tasks
	if (bDragging && HasOption(GTLCF_CALCPARENTDATES) && pGI->bParent)
		return 0;

	COleDateTime dtStart, dtEnd;
	
	if (!GetTaskStartEndDates(*pGI, dtStart, dtEnd))
		return 0;

	// Calculate the task rect
	CRect rTask;
	VERIFY(GetListItemRect(nItem, rTask));

	if (!CDateHelper::DateHasTime(dtEnd))
		dtEnd += 1.0;

	// One or both of the task's dates may be outside the active range
	int nFrom, nTo;

	BOOL bValidFrom = GetDrawPosFromDate(dtStart, nFrom);
	BOOL bValidTo = GetDrawPosFromDate(dtEnd, nTo);

	if (!bValidFrom && !bValidTo)
		return FALSE;

	if (!bValidFrom)
	{
		// Start date precedes active range so dummy something up
		ASSERT(m_dtActiveRange.IsValid() && !m_dtActiveRange.Contains(*pGI));

		nFrom = -1000;
	}
	else if (!bValidTo)
	{
		// End date exceeds active range so dummy something up
		ASSERT(m_dtActiveRange.IsValid() && !m_dtActiveRange.Contains(*pGI));

		CRect rClient;
		CWnd::GetClientRect(rClient);

		nTo = (rClient.Width() + 1000);
	}

	rTask.right = nTo;
	rTask.left = nFrom;

	// Create 'hit' boxes around the two ends
	const int nDragTol = GetSystemMetrics(SM_CXDOUBLECLK);
	CRect rStart(rTask), rEnd(rTask);

	rStart.right = (rStart.left + min((rTask.Width() / 2), nDragTol));
	rStart.left -= nDragTol;
	rEnd.left = (rEnd.right - min((rTask.Width() / 2), nDragTol));
	rEnd.right += nDragTol;

 	// now check for closeness to ends
	CPoint ptClient(point);
	
	if (bScreen)
		m_list.ScreenToClient(&ptClient);
	
	if (rStart.PtInRect(ptClient))
	{
		nHit = GTLCHT_BEGIN;
	}
	else if (rEnd.PtInRect(ptClient))
	{
		nHit = GTLCHT_END;
	}
	else if (rTask.PtInRect(ptClient))
	{
		nHit = GTLCHT_MIDDLE;
	}
	
	return dwTaskID;
}

DWORD CGanttCtrl::GetTaskID(HTREEITEM htiFrom) const
{
	return GetItemData(htiFrom);
}

DWORD CGanttCtrl::GetTaskID(int nItem) const
{
	return GetItemData(GetTreeItem(nItem));
}

BOOL CGanttCtrl::IsDragging() const
{
	return IsDragging(m_nDragging);
}

BOOL CGanttCtrl::IsDragging(GTLC_DRAG nDrag)
{
	return ((nDrag != GTLCD_ANY) && (nDrag != GTLCD_NONE));
}

BOOL CGanttCtrl::IsDraggingEnds(GTLC_DRAG nDrag)
{
	return ((nDrag == GTLCD_START) || (nDrag == GTLCD_END));
}

BOOL CGanttCtrl::IsValidDragPoint(const CPoint& ptDrag) const
{
	if (!IsDragging())
		return FALSE;

	CRect rLimits;
	GetDragLimits(rLimits);

	return rLimits.PtInRect(ptDrag);
}

void CGanttCtrl::GetDragLimits(CRect& rLimits) const
{
	m_list.GetClientRect(rLimits);

	// Clip the right hand end to the last column
	CRect rColumn;
	m_listHeader.GetItemRect(m_listHeader.GetItemCount() - 1, rColumn);
	rLimits.right = min(rLimits.right, rColumn.right);

	// Allow a buffer at start and end
	rLimits.InflateRect(DRAG_BUFFER, 0);
}

BOOL CGanttCtrl::ValidateDragPoint(CPoint& ptDrag) const
{
	if (!IsValidDragPoint(ptDrag))
		return FALSE;

	// Clip to drag limits
	CRect rLimits;
	GetDragLimits(rLimits);

	ptDrag.x = max(ptDrag.x, rLimits.left);
	ptDrag.x = min(ptDrag.x, rLimits.right);
	ptDrag.y = max(ptDrag.y, rLimits.top);
	ptDrag.y = min(ptDrag.y, rLimits.bottom);

	return TRUE;
}

BOOL CGanttCtrl::CanDragTask(DWORD dwTaskID, GTLC_DRAG nDrag) const
{
	if (m_data.ItemIsLocked(dwTaskID, FALSE))
		return FALSE;

	// else
	switch (nDrag)
	{
	case GTLCD_START:
	case GTLCD_WHOLE:
		if (HasOption(GTLCF_DISABLEDEPENDENTDRAGGING) && m_data.ItemHasDependecies(dwTaskID))
			return FALSE;
		break;
	}
	
	// else
	return TRUE;
}

BOOL CGanttCtrl::StartDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);
	ASSERT(!IsDependencyEditing());

	GTLC_HITTEST nHit = GTLCHT_NOWHERE;
	
	DWORD dwTaskID = ListHitTestTask(ptCursor, FALSE, nHit, TRUE);
	ASSERT((nHit == GTLCHT_NOWHERE) || (dwTaskID != 0));

	if (nHit == GTLCHT_NOWHERE)
		return FALSE;

	GTLC_DRAG nDrag = MapHitTestToDrag(nHit);
	ASSERT(IsDragging(nDrag));

	if (!CanDragTask(dwTaskID, nDrag))
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return FALSE;
	}
	
	if (dwTaskID != GetSelectedTaskID())
		SelectTask(dwTaskID);

	CPoint ptScreen(ptCursor);
	m_list.ClientToScreen(&ptScreen);
	
	if (!::DragDetect(m_list, ptScreen))
		return FALSE;

	GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, FALSE);
	
	// cache the original task and the start point
	m_giPreDrag = *pGI;
	m_ptDragStart = ptCursor;

	// Ensure the gantt item has valid dates
	COleDateTime dtStart, dtDue;
	GetTaskStartEndDates(*pGI, dtStart, dtDue);
	
	if (!pGI->HasDueDate())
	{
		if (!CDateHelper::IsDateSet(dtDue))
			return FALSE;

		// else
		pGI->SetDueDate(dtDue, TRUE);
	}
	
	if (!pGI->HasStartDate())
	{
		if (!CDateHelper::IsDateSet(dtStart))
			return FALSE;

		// else
		pGI->SetStartDate(dtStart, TRUE);
	}
	
	// Start dragging
	m_nDragging = nDrag;
	m_dtDragMin = m_data.CalcMaxDependencyDate(m_giPreDrag);

	m_list.SetCapture();
	
	// keep parent informed
	NotifyParentDragChange();

	return TRUE;
}

BOOL CGanttCtrl::EndDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);
	ASSERT(!IsDependencyEditing());
	ASSERT(IsDragging());

	if (IsDragging())
	{
		DWORD dwTaskID = GetSelectedTaskID();

		GANTTITEM* pGI = NULL;
		GET_GI_RET(dwTaskID, pGI, FALSE);

		// Restore original refID because that's what we've been really dragging
		if (pGI->dwOrgRefID)
		{
			dwTaskID = pGI->dwOrgRefID;
			pGI->dwOrgRefID = 0;
		}

		// dropping outside the list is a cancel
		if (!IsValidDragPoint(ptCursor))
		{
			CancelDrag(TRUE);
			return FALSE;
		}

		GTLC_DRAG nDrag = m_nDragging;
		
		// cleanup
		m_nDragging = GTLCD_NONE;
		::ReleaseCapture();

		// keep parent informed
		if (DragDatesDiffer(*pGI, m_giPreDrag))
		{
			if (!NotifyParentDateChange(nDrag))
				RestoreGanttItem(m_giPreDrag);
			else
				RecalcDateRange();

			NotifyParentDragChange();
		}

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CGanttCtrl::DragDatesDiffer(const GANTTITEM& gi1, const GANTTITEM& gi2)
{
	return ((gi1.dtRange.GetStart() != gi2.dtRange.GetStart()) || 
			(gi1.dtRange.GetEnd() != gi2.dtRange.GetEnd()));
}

void CGanttCtrl::NotifyParentDragChange()
{
	ASSERT(!m_bReadOnly);
	ASSERT(GetSelectedTaskID());

	CWnd::GetParent()->SendMessage(WM_GTLC_DRAGCHANGE, (WPARAM)GetSnapMode(), GetSelectedTaskID());
}

BOOL CGanttCtrl::NotifyParentDateChange(GTLC_DRAG nDrag)
{
	ASSERT(!m_bReadOnly);
	ASSERT(GetSelectedTaskID());

	if (IsDragging(nDrag))
		return CWnd::GetParent()->SendMessage(WM_GTLC_DATECHANGE, (WPARAM)nDrag, (LPARAM)&m_giPreDrag);

	// else
	return 0L;
}

BOOL CGanttCtrl::UpdateDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);
	ASSERT(!IsDependencyEditing());
	
	if (IsDragging())
	{
		DWORD dwTaskID = GetSelectedTaskID();
		GANTTITEM* pGI = NULL;

		GET_GI_RET(dwTaskID, pGI, FALSE);

		COleDateTime dtStart, dtDue;
		GetTaskStartEndDates(*pGI, dtStart, dtDue);

		// update taskID to refID because we're really dragging the refID
		if (pGI->dwOrgRefID)
		{
			dwTaskID = pGI->dwOrgRefID;
			pGI->dwOrgRefID = 0;
		}
		
		COleDateTime dtDrag;

		if (GetValidDragDate(ptCursor, dtDrag))
		{
			// if the drag date precedes the min date, constrain
			// date appropriately and show the 'no drag' cursor
			BOOL bNoDrag = (CDateHelper::IsDateSet(m_dtDragMin) && (dtDrag < m_dtDragMin));
			LPCTSTR szCursor = NULL;

			CDateHelper::Max(dtDrag, m_dtDragMin);

			switch (m_nDragging)
			{
			case GTLCD_START:
				{
					// prevent the start and end dates from overlapping
					double dMinDuration = CalcMinDragDuration();
					COleDateTime dtNewStart(min(dtDrag.m_dt, (dtDue.m_dt - dMinDuration)));

					pGI->SetStartDate(dtNewStart);
					szCursor = IDC_SIZEWE;
				}
				break;

			case GTLCD_END:
				{
					// prevent the start and end dates from overlapping
					double dMinDuration = CalcMinDragDuration();
					COleDateTime dtNewDue(max(dtDrag.m_dt, (dtStart.m_dt + dMinDuration)));

					pGI->SetDueDate(dtNewDue);
					szCursor = IDC_SIZEWE;
				}
				break;

			case GTLCD_WHOLE:
				{
					// Note: If the end date of the dragged task
					// falls on a day-end GANTTDATERANGE will add
					// a day because that's all it knows how to do.
					// We however don't always want it to so we must
					// detect those times and subtract a day as required
					COleDateTime dtOrgStart, dtOrgDue;
					GetTaskStartEndDates(m_giPreDrag, dtOrgStart, dtOrgDue);
					
					COleDateTime dtDuration(dtOrgDue - dtOrgStart);
					COleDateTime dtEnd = (dtDrag + dtDuration);
					
					if (!CDateHelper::DateHasTime(dtEnd))
						dtEnd.m_dt--;

					pGI->dtRange.SetStart(dtDrag);
					pGI->dtRange.SetEnd(dtEnd);

					szCursor = IDC_SIZEALL;
				}
				break;
			}
			ASSERT(szCursor);

			if (bNoDrag)
				GraphicsMisc::SetDragDropCursor(GMOC_NO);
			else
				GraphicsMisc::SetStandardCursor(szCursor);

			RecalcParentDates();
			RedrawList();
			RedrawTree();

			// keep parent informed
			NotifyParentDragChange();
		}
		else
		{
			// We've dragged outside the client rect
			GraphicsMisc::SetDragDropCursor(GMOC_NO);
		}

		return TRUE; // always
	}

	// else
	return FALSE; // not dragging
}

double CGanttCtrl::CalcMinDragDuration() const
{
	ASSERT((m_nDragging == GTLCD_START) || (m_nDragging == GTLCD_END));
	
	double dMin;

	if (CalcMinDragDuration(GetSnapMode(), dMin))
		return dMin;

	// Handle 'Free' (no snapping)
	switch (m_nMonthDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
	case GTLC_DISPLAY_DECADES:
	case GTLC_DISPLAY_YEARS:
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		VERIFY(CalcMinDragDuration(GTLCSM_NEARESTMONTH, dMin));
		break;
		
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		VERIFY(CalcMinDragDuration(GTLCSM_NEARESTDAY, dMin));
		break;
		
	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		VERIFY(CalcMinDragDuration(GTLCSM_NEARESTHOUR, dMin));
		break;

	case GTLC_DISPLAY_HOURS:
		VERIFY(CalcMinDragDuration(GTLCSM_NEARESTHALFHOUR, dMin));
		break;

	default:
		ASSERT(0);
		VERIFY(CalcMinDragDuration(GTLCSM_NEARESTHOUR, dMin));
		break;
	}

	ASSERT(dMin > 0.0);

	return dMin;
}

BOOL CGanttCtrl::CalcMinDragDuration(GTLC_SNAPMODE nMode, double& dMin)
{
	dMin = -1;

	switch (nMode)
	{
	case GTLCSM_NEARESTQUARTERCENTURY:	dMin = 9125.0;		break;
	case GTLCSM_NEARESTDECADE:			dMin = 3650.0;		break;
	case GTLCSM_NEARESTYEAR:			dMin = 365.0;		break;
	case GTLCSM_NEARESTHALFYEAR:		dMin = 182.0;		break;
	case GTLCSM_NEARESTQUARTER:			dMin = 91.0;		break;
	case GTLCSM_NEARESTMONTH:			dMin = 30.0;		break;
	case GTLCSM_NEARESTWEEK:			dMin = 7.0;			break;
	case GTLCSM_NEARESTDAY:				dMin = 1.0;			break;
	case GTLCSM_NEARESTHALFDAY:			dMin = 0.5;			break;
	case GTLCSM_NEARESTHOUR:			dMin = (1.0 / 24);	break;
	case GTLCSM_NEARESTHALFHOUR:		dMin = (1.0 / 48);	break;

	case GTLCSM_FREE:
		return FALSE;

	default:
		ASSERT(0);
		return FALSE;
	}

	ASSERT(dMin > 0.0);

	// Handle whole days
	if (dMin == (int)dMin)
		dMin = CDateHelper::GetEndOfPreviousDay(dMin).m_dt;

	return TRUE;
}

BOOL CGanttCtrl::GetValidDragDate(const CPoint& ptCursor, COleDateTime& dtDrag) const
{
	ASSERT(IsDragging());
	CPoint ptDrag(ptCursor);

	if (!ValidateDragPoint(ptDrag))
		return FALSE;

	if (!GetDateFromPoint(ptDrag, dtDrag))
		return FALSE;

	// if dragging the whole task, we calculate dtDrag as the 
	// original start date plus the difference between the 
	// current drag pos and the initial drag pos
	if (m_nDragging == GTLCD_WHOLE)
	{
		COleDateTime dtOrg;
		GetDateFromPoint(m_ptDragStart, dtOrg);
		
		COleDateTime dtPreStart, dtNotUsed;
		VERIFY(GetTaskStartEndDates(m_giPreDrag, dtPreStart, dtNotUsed));

		double dOffset = (dtDrag.m_dt - dtOrg.m_dt);
		dtDrag = (dtPreStart.m_dt + dOffset);
	}
	
	// adjust date depending on modifier keys 
	dtDrag = GetNearestDate(dtDrag);
	
	// else
	return TRUE;
}

BOOL CGanttCtrl::GetDateFromPoint(const CPoint& ptCursor, COleDateTime& date) const
{
	// convert pos to date
	int nScrollPos = (m_list.GetScrollPos(SB_HORZ) + ptCursor.x);

	if (GetDateFromScrollPos(nScrollPos, date))
		return TRUE;

	// Fallback for dragging
	if (IsValidDragPoint(ptCursor))
	{
		BOOL bDraggingLeft = (ptCursor.x < 0);
			
		int nRefCol = (bDraggingLeft ? 1 : (m_listHeader.GetItemCount() - 1));
		int nDir = (bDraggingLeft ? -1 : 1);

		CRect rColumn;
		VERIFY(GetListColumnRect(nRefCol, rColumn, FALSE));

		// Calculate the equivalent date for the 'previous/next' column
		rColumn.OffsetRect((nDir * rColumn.Width()), 0);
		ASSERT(nScrollPos >= rColumn.left && nScrollPos < rColumn.right);

		int nYear, nMonth;
		VERIFY(GetListColumnDate(nRefCol, nMonth, nYear));

		int nNumMonthsPerCol = GetNumMonthsPerColumn(m_nMonthDisplay);
		CDateHelper::IncrementMonth(nMonth, nYear, (nDir * nNumMonthsPerCol));

		return GetDateFromScrollPos(nScrollPos, m_nMonthDisplay, nMonth, nYear, rColumn, date);
	}

	// else
	return FALSE;
}

void CGanttCtrl::SetReadOnly(bool bReadOnly) 
{ 
	m_bReadOnly = bReadOnly;

	CTreeListCtrl::EnableDragAndDrop(!bReadOnly);
}

// external version
BOOL CGanttCtrl::CancelOperation()
{
	if (CTreeListCtrl::CancelOperation())
		return TRUE;

	if (IsDragging())
	{
		CancelDrag(TRUE);
		return TRUE;
	}

	if (IsDependencyEditing())
	{
		EndDependencyEdit();
		return TRUE;
	}
	
	// else 
	return FALSE;
}

// internal version
void CGanttCtrl::CancelDrag(BOOL bReleaseCapture)
{
	ASSERT(IsDragging());

	if (bReleaseCapture)
		ReleaseCapture();
	
	// cancel drag, restoring original task dates
	RestoreGanttItem(m_giPreDrag);
	m_nDragging = GTLCD_NONE;

	// keep parent informed
	NotifyParentDragChange();
}

void CGanttCtrl::GetColumnWidths(CIntArray& aTreeWidths, CIntArray& aListWidths) const
{
	m_treeHeader.GetItemWidths(aTreeWidths);
	m_listHeader.GetItemWidths(aListWidths);

	// trim the list columns to what's currently visible
	// remember to include hidden dummy first column
	int nNumMonths = (GetRequiredListColumnCount() + 1);
	int nItem = aListWidths.GetSize();

	while (nItem-- > nNumMonths)
		aListWidths.RemoveAt(nItem);
}

BOOL CGanttCtrl::SetColumnWidths(const CIntArray& aTreeWidths, const CIntArray& aListWidths)
{
	if (!SetTreeColumnWidths(aTreeWidths))
		return FALSE;

	// save list column widths for when we've initialised our columns
	// remember to include hidden dummy first column
	if (aListWidths.GetSize() == (GetRequiredListColumnCount() + 1))
	{
		m_listHeader.SetItemWidths(aListWidths);
	}
	else
	{
		m_aPrevColWidths.Copy(aListWidths);
		SetSplitPos(m_treeHeader.CalcTotalItemWidth());
	}

	return TRUE;
}

void CGanttCtrl::GetTrackedColumns(CIntArray& aTreeTracked, CIntArray& aListTracked) const
{
	m_treeHeader.GetTrackedItems(aTreeTracked);
	m_listHeader.GetTrackedItems(aListTracked);

	// trim the list columns to what's currently visible
	// remember to include hidden dummy first column
	int nNumMonths = (GetRequiredListColumnCount() + 1);
	int nItem = aListTracked.GetSize();

	while (nItem-- > nNumMonths)
		aListTracked.RemoveAt(nItem);
}

BOOL CGanttCtrl::SetTrackedColumns(const CIntArray& aTreeTracked, const CIntArray& aListTracked)
{
	if (!SetTrackedTreeColumns(aTreeTracked))
		return FALSE;

	// save list column tracking for when we've initialised our columns
	// remember to include hidden dummy first column
	if (aListTracked.GetSize() == (GetRequiredListColumnCount() + 1))
		m_listHeader.SetTrackedItems(aListTracked);
	else
		m_aPrevTrackedCols.Copy(aListTracked);
	
	return TRUE;
}

GTLC_SNAPMODE CGanttCtrl::GetSnapMode() const
{
	if (IsDragging())
	{
		BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);
		BOOL bShift = Misc::IsKeyPressed(VK_SHIFT);

		if (bCtrl || bShift)
		{
			switch (m_nMonthDisplay)
			{
			case GTLC_DISPLAY_QUARTERCENTURIES:
			case GTLC_DISPLAY_DECADES:
				if (bCtrl && bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTHALFYEAR;
				}
				else if (bCtrl)
				{
					m_nSnapMode = GTLCSM_NEARESTYEAR;
				}
				else if (bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTDECADE;
				}
				break;

			case GTLC_DISPLAY_YEARS:
				if (bCtrl && bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTHALFYEAR;
				}
				else if (bCtrl)
				{
					m_nSnapMode = GTLCSM_NEARESTMONTH;
				}
				else if (bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTYEAR;
				}
				break;
				
			case GTLC_DISPLAY_QUARTERS_SHORT:
			case GTLC_DISPLAY_QUARTERS_MID:
			case GTLC_DISPLAY_QUARTERS_LONG:
				if (bCtrl)
				{
					m_nSnapMode = GTLCSM_NEARESTMONTH;
				}
				else if (bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTQUARTER;
				}
				break;
				
			case GTLC_DISPLAY_MONTHS_SHORT:
			case GTLC_DISPLAY_MONTHS_MID:
			case GTLC_DISPLAY_MONTHS_LONG:
				if (bCtrl)
				{
					m_nSnapMode = GTLCSM_NEARESTDAY;
				}
				else if (bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTMONTH;
				}
				break;
				
			case GTLC_DISPLAY_WEEKS_SHORT:
			case GTLC_DISPLAY_WEEKS_MID:
			case GTLC_DISPLAY_WEEKS_LONG:
				if (bCtrl)
				{
					m_nSnapMode = GTLCSM_NEARESTDAY;
				}
				else if (bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTWEEK;
				}
				break;
				
			case GTLC_DISPLAY_DAYS_SHORT:
			case GTLC_DISPLAY_DAYS_MID:
			case GTLC_DISPLAY_DAYS_LONG:
				if (bCtrl && bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTHALFDAY;
				}
				else if (bCtrl)
				{
					m_nSnapMode = GTLCSM_NEARESTHOUR;
				}
				else if (bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTDAY;
				}
				break;

			case GTLC_DISPLAY_HOURS:
				if (bCtrl && bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTHOUR;
				}
				else if (bCtrl)
				{
					m_nSnapMode = GTLCSM_NEARESTHALFHOUR;
				}
				else if (bShift)
				{
					m_nSnapMode = GTLCSM_NEARESTHALFDAY;
				}
				// TODO
				break;
				
			default:
				ASSERT(0);
				// fall thru to whatever's currently set
				break;
			}
		}
	}

	// else
	return m_nSnapMode;
}

COleDateTime CGanttCtrl::GetNearestDate(const COleDateTime& dtDrag) const
{
	ASSERT(IsDragging());

	BOOL bDraggingEnd = (m_nDragging == GTLCD_END);

	switch (GetSnapMode())
	{
	case GTLCSM_NEARESTQUARTERCENTURY:
		return CDateHelper::GetNearestQuarterCentury(dtDrag, bDraggingEnd);
		
	case GTLCSM_NEARESTDECADE:
		return CDateHelper::GetNearestDecade(dtDrag, bDraggingEnd);

	case GTLCSM_NEARESTYEAR:
		return CDateHelper::GetNearestYear(dtDrag, bDraggingEnd);
		
	case GTLCSM_NEARESTHALFYEAR:
		return CDateHelper::GetNearestHalfYear(dtDrag, bDraggingEnd);

	case GTLCSM_NEARESTQUARTER:
		return CDateHelper::GetNearestQuarter(dtDrag, bDraggingEnd);

	case GTLCSM_NEARESTMONTH:
		return CDateHelper::GetNearestMonth(dtDrag, bDraggingEnd);

	case GTLCSM_NEARESTWEEK:
		return CDateHelper::GetNearestWeek(dtDrag, bDraggingEnd);

	case GTLCSM_NEARESTDAY:
		return CDateHelper::GetNearestDay(dtDrag, bDraggingEnd);

	case GTLCSM_NEARESTHALFDAY:
		return CDateHelper::GetNearestHalfDay(dtDrag, bDraggingEnd);

	case GTLCSM_NEARESTHOUR:
		return CDateHelper::GetNearestHour(dtDrag, bDraggingEnd);

	case GTLCSM_NEARESTHALFHOUR:
		return CDateHelper::GetNearestHalfHour(dtDrag, bDraggingEnd);

	case GTLCSM_FREE:
		if (bDraggingEnd)
		{
			COleDateTime dtEndOfDragDay(CDateHelper::GetEndOfDay(dtDrag));

			if (dtDrag >= dtEndOfDragDay)
				return dtEndOfDragDay;
		}
		// else
		return dtDrag;
	}

	// all else
	ASSERT(0);
	return dtDrag;
}

DWORD CGanttCtrl::GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const
{
	HTREEITEM hti = GetTreeItem(dwTaskID);
	
	if (!hti)
	{
		ASSERT(0);
		return 0L;
	}

	DWORD dwNextID(dwTaskID);
	HTREEITEM htiNext = NULL;

	switch (nCmd)
	{
	case IUI_GETNEXTTASK:
		htiNext = m_tree.GetNextItem(hti, TVGN_NEXT);
		break;

	case IUI_GETPREVTASK:
		htiNext = m_tree.GetNextItem(hti, TVGN_PREVIOUS);
		break;

	case IUI_GETNEXTVISIBLETASK:
		htiNext = TCH().GetNextVisibleItem(hti);
		break;

	case IUI_GETPREVVISIBLETASK:
		htiNext = TCH().GetPrevVisibleItem(hti);
		break;

	case IUI_GETNEXTTOPLEVELTASK:
		htiNext = TCH().GetNextTopLevelItem(hti, TRUE);
		break;

	case IUI_GETPREVTOPLEVELTASK:
		htiNext = TCH().GetNextTopLevelItem(hti, FALSE);
		break;

	default:
		ASSERT(0);
	}

	if (htiNext)
		dwNextID = GetTreeItemData(m_tree, htiNext);

	return dwNextID;
}

BOOL CGanttCtrl::SaveToImage(CBitmap& bmImage)
{
	if (m_tree.GetCount() == 0)
		return FALSE;

	// Calculate the date range in scroll units
	// allowing a month's buffer at each end if there is space
	COleDateTime dtFrom = CDateHelper::GetStartOfMonth(ActiveDateRange().GetStart());
	COleDateTime dtTo = CDateHelper::GetEndOfMonth(ActiveDateRange().GetEnd());
 
	COleDateTime dtStart, dtEnd;
	
	VERIFY(GetListColumnDate(1, dtStart));
	VERIFY(GetListColumnDate(m_listHeader.GetItemCount() - 1, dtEnd, TRUE));

	if (CDateHelper::CalcMonthsFromTo(dtStart, dtFrom, FALSE) > 0)
		CDateHelper::IncrementMonth(dtFrom, -1);

	if (CDateHelper::CalcMonthsFromTo(dtTo, dtEnd, FALSE) > 0)
		CDateHelper::IncrementMonth(dtTo, 1);

	int nFrom, nTo;
	VERIFY(GetScrollPosFromDate(dtFrom, nFrom));
	VERIFY(GetScrollPosFromDate(dtTo, nTo));

	if (nTo == nFrom)
		nTo = -1;
	
	return CTreeListCtrl::SaveToImage(bmImage, nFrom, nTo);
}

void CGanttCtrl::RefreshItemBoldState(HTREEITEM htiFrom, BOOL bAndChildren)
{
	if (htiFrom && (htiFrom != TVI_ROOT))
	{
		TCH().SetItemBold(htiFrom, (m_tree.GetParentItem(htiFrom) == NULL));
	}
	
	// children
	if (bAndChildren)
	{
		HTREEITEM htiChild = m_tree.GetChildItem(htiFrom);
		
		while (htiChild)
		{
			RefreshItemBoldState(htiChild);
			htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
}

BOOL CGanttCtrl::SetFont(HFONT hFont, BOOL bRedraw)
{
	if (!CTreeListCtrl::SetFont(hFont, bRedraw))
		return FALSE;

	CalcMinMonthWidths();
	SetMonthDisplay(m_nMonthDisplay);
	ResizeListColumnsToFit(TRUE);

	return TRUE;
}

BOOL CGanttCtrl::CanMoveSelectedTask(const IUITASKMOVE& move) const
{
	if (m_bReadOnly)
		return FALSE;

	TLCITEMMOVE itemMove = { 0 };

	itemMove.htiSel = GetTreeItem(move.dwSelectedTaskID);
	itemMove.htiDestParent = GetTreeItem(move.dwParentID);
	itemMove.htiDestAfterSibling = GetTreeItem(move.dwAfterSiblingID);

	return CTreeListCtrl::CanMoveItem(itemMove);
}

BOOL CGanttCtrl::MoveSelectedTask(const IUITASKMOVE& move)
{
	if (m_bReadOnly)
		return FALSE;

	TLCITEMMOVE itemMove = { 0 };

	itemMove.htiSel = GetTreeItem(move.dwSelectedTaskID);
	itemMove.htiDestParent = GetTreeItem(move.dwParentID);
	itemMove.htiDestAfterSibling = GetTreeItem(move.dwAfterSiblingID);

	return CTreeListCtrl::MoveItem(itemMove);
}
