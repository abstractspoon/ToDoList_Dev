// GanttTreeList.cpp: implementation of the CGanttTreeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "GanttTreeListCtrl.h"
#include "GanttMsg.h"
#include "GanttStatic.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\DateHelper.h"
#include "..\shared\timeHelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\graphicsMisc.h"
#include "..\shared\TreeCtrlHelper.h"
#include "..\shared\autoflag.h"
#include "..\shared\misc.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\themed.h"
#include "..\shared\osversion.h"
#include "..\shared\enbitmap.h"
#include "..\shared\copywndcontents.h"

#include "..\Interfaces\iuiextension.h"

#include "..\3rdparty\shellicons.h"

#include <float.h> // for DBL_MAX
#include <math.h>  // for fabs()

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#ifndef GET_WHEEL_DELTA_WPARAM
#	define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#endif 

#ifndef CDRF_SKIPPOSTPAINT
#	define CDRF_SKIPPOSTPAINT	(0x00000100)
#endif

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
const int MAX_YEAR = 2100;
#else
const int MAX_YEAR = 2200;
#endif

const int MIN_COL_WIDTH			= GraphicsMisc::ScaleByDPIFactor(6);
const int MIN_LABEL_EDIT_WIDTH	= GraphicsMisc::ScaleByDPIFactor(200);
const int DEF_MONTH_WIDTH		= GraphicsMisc::ScaleByDPIFactor(72);
const int TREE_TITLE_MIN_WIDTH	= GraphicsMisc::ScaleByDPIFactor(75); 
const int COLUMN_PADDING		= GraphicsMisc::ScaleByDPIFactor(15);
const int MIN_MONTH_WIDTH		= GraphicsMisc::ScaleByDPIFactor(2);
const int MINS_IN_HOUR			= 60;
const int MINS_IN_DAY			= (MINS_IN_HOUR * 24);
const int LV_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int TV_TIPPADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int HD_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(6);
const int MAX_HEADER_WIDTH		= 32000; // (SHRT_MAX - tolerance)
const int DRAG_BUFFER			= GraphicsMisc::ScaleByDPIFactor(50);
const int DONE_BOX				= GraphicsMisc::ScaleByDPIFactor(6);
const int IMAGE_SIZE			= GraphicsMisc::ScaleByDPIFactor(16);

const LONG DEPENDPICKPOS_NONE = 0xFFFFFFFF;
const double DAY_WEEK_MULTIPLIER = 1.5;
const double HOUR_DAY_MULTIPLIER = 6;
const double MULTIYEAR_MULTIPLIER = 2.0;
const double DAYS_IN_YEAR = 365.25;
const double DAYS_IN_MONTH = (DAYS_IN_YEAR / 12);

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

#define FROMISABOVE(t) ((t == GCDDT_FROMISABOVELEFT) || (t == GCDDT_FROMISABOVERIGHT))

//////////////////////////////////////////////////////////////////////

class CGanttLockUpdates : public CLockUpdates
{
public:
	CGanttLockUpdates(CGanttTreeListCtrl* pCtrl, BOOL bTree, BOOL bAndSync) 
		: 
	CLockUpdates(bTree ? pCtrl->m_tree.GetSafeHwnd() : pCtrl->m_list.GetSafeHwnd()),
		m_bAndSync(bAndSync), 
		m_pCtrl(pCtrl)
	{
		ASSERT(m_pCtrl);
		
		if (m_bAndSync)
			m_pCtrl->EnableResync(FALSE);
	}
	
	~CGanttLockUpdates()
	{
		ASSERT(m_pCtrl);
		
		if (m_bAndSync)
			m_pCtrl->EnableResync(TRUE, m_hWnd);
	}
	
private:
	BOOL m_bAndSync;
	CGanttTreeListCtrl* m_pCtrl;
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGanttTreeListCtrl::CGanttTreeListCtrl(CGanttTreeCtrl& tree, CListCtrl& list) 
	:
	CTreeListSyncer(TLSF_SYNCSELECTION | TLSF_SYNCFOCUS | TLSF_BORDER | TLSF_SYNCDATA | TLSF_SPLITTER),
	m_tree(tree),
	m_list(list),
	m_pTCH(NULL),
	m_nMonthWidth(DEF_MONTH_WIDTH),
	m_nMonthDisplay(GTLC_DISPLAY_MONTHS_LONG),
	m_dwOptions(GTLCF_AUTOSCROLLTOTASK | GTLCF_SHOWSPLITTERBAR),
	m_crAltLine(CLR_NONE),
	m_crGridLine(CLR_NONE),
	m_crDefault(CLR_NONE),
	m_crParent(CLR_NONE),
	m_crToday(CLR_NONE),
	m_crWeekend(CLR_NONE),
	m_nParentColoring(GTLPC_DEFAULTCOLORING),
	m_nDragging(GTLCD_NONE), 
	m_ptDragStart(0),
	m_ptLastDependPick(0),
	m_pDependEdit(NULL),
	m_dwMaxTaskID(0),
	m_bReadOnly(FALSE),
	m_bMovingTask(FALSE),
	m_nPrevDropHilitedItem(-1),
	m_tshDragDrop(tree),
	m_treeDragDrop(m_tshDragDrop, tree)
{

}

CGanttTreeListCtrl::~CGanttTreeListCtrl()
{
	Release();
}

BOOL CGanttTreeListCtrl::Initialize(UINT nIDTreeHeader)
{
	ASSERT(m_tree.GetSafeHwnd());
	ASSERT(m_list.GetSafeHwnd());

	// misc
	m_nMonthWidth = DEF_MONTH_WIDTH;

	// initialize tree header
	if (!m_treeHeader.SubclassDlgItem(nIDTreeHeader, m_tree.GetParent()))
		return FALSE;

	m_treeHeader.ModifyStyle(0, (HDS_FULLDRAG | HDS_HOTTRACK | HDS_BUTTONS | HDS_DRAGDROP));

	// subclass the tree and list
	if (!CTreeListSyncer::Sync(m_tree, m_list, TLSL_RIGHTDATA_IS_LEFTITEM, m_treeHeader))
		return FALSE;

	// subclass the list header
	VERIFY(m_listHeader.SubclassWindow(ListView_GetHeader(m_list)));
	
	// prevent column reordering on columns
	m_listHeader.ModifyStyle(HDS_DRAGDROP, 0);

	// prevent translation of the list header
	CLocalizer::EnableTranslation(m_listHeader, FALSE);

	// Initialise tree drag and drop
	m_treeDragDrop.Initialize(m_tree.GetParent(), TRUE, FALSE);

	// misc
	m_tree.ModifyStyle(TVS_SHOWSELALWAYS, 0, 0);
	m_list.ModifyStyle(LVS_SHOWSELALWAYS, 0, 0);

	ListView_SetExtendedListViewStyleEx(m_list, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	BuildTreeColumns();
	BuildListColumns();

	CalcMinMonthWidths();

	if (m_nMonthWidth != DEF_MONTH_WIDTH)
		RecalcListColumnWidths(DEF_MONTH_WIDTH, m_nMonthWidth);

	return TRUE;
}

void CGanttTreeListCtrl::InitItemHeights()
{
	CTreeListSyncer::InitItemHeights();

	GANTTDEPENDENCY::STUB = (m_tree.GetItemHeight() / 2);
}

void CGanttTreeListCtrl::Release() 
{ 
	if (::IsWindow(m_treeHeader))
		m_treeHeader.UnsubclassWindow();

	if (::IsWindow(m_listHeader))
		m_listHeader.UnsubclassWindow();

	Unsync(); 

	delete m_pTCH;
	m_pTCH = NULL;
}

HTREEITEM CGanttTreeListCtrl::GetSelectedItem() const
{
	return GetTreeSelItem();
}

DWORD CGanttTreeListCtrl::GetSelectedTaskID() const
{
	HTREEITEM hti = GetTreeSelItem();

	return (hti ? GetTaskID(hti) : 0);
}

BOOL CGanttTreeListCtrl::GetSelectedTaskDependencies(CDWordArray& aDepends) const
{
	DWORD dwTaskID = GetSelectedTaskID();
	const GANTTITEM* pGI = NULL;
	
	GET_GI_RET(dwTaskID, pGI, FALSE);
	
	aDepends.Copy(pGI->aDependIDs);
	return TRUE;
}

BOOL CGanttTreeListCtrl::SetSelectedTaskDependencies(const CDWordArray& aDepends)
{
	DWORD dwTaskID = GetSelectedTaskID();
	GANTTITEM* pGI = NULL;
	
	GET_GI_RET(dwTaskID, pGI, FALSE);

	pGI->aDependIDs.Copy(aDepends);
	RedrawList();
	
	return TRUE;
}

BOOL CGanttTreeListCtrl::GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const
{
	DWORD dwTaskID = GetSelectedTaskID();
	const GANTTITEM* pGI = NULL;

	GET_GI_RET(dwTaskID, pGI, FALSE);
	
	if (GetTaskStartDueDates(*pGI, dtStart, dtDue))
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

BOOL CGanttTreeListCtrl::SelectTask(DWORD dwTaskID)
{
	HTREEITEM hti = FindTreeItem(m_tree, dwTaskID);

	return SelectItem(hti);
}

BOOL CGanttTreeListCtrl::SelectItem(HTREEITEM hti)
{
	if (hti == NULL)
		return FALSE;

	BOOL bWasVisible = IsTreeItemVisible(m_tree, hti);

	SelectTreeItem(hti, FALSE);
	ResyncSelection(m_list, m_tree, FALSE);

	if (!bWasVisible)
		ExpandList();

	return TRUE;
}

BOOL CGanttTreeListCtrl::SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select)
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

BOOL CGanttTreeListCtrl::SelectTask(HTREEITEM hti, const IUISELECTTASK& select, BOOL bForwards)
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

void CGanttTreeListCtrl::RecalcParentDates()
{
	GANTTITEM dummy;
	GANTTITEM* pGI = &dummy;

	RecalcParentDates(NULL, pGI);
}

void CGanttTreeListCtrl::RecalcParentDates(HTREEITEM htiParent, GANTTITEM*& pGI)
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
	
	// reset dates
	pGI->dtMinStart = pGI->dtStart;
	pGI->dtMaxDue = pGI->dtDue;

	// iterate children 
	HTREEITEM htiChild = m_tree.GetChildItem(htiParent);
	
	while (htiChild)
	{
		GANTTITEM* pGIChild;

		// recalc child if it has children itself
		RecalcParentDates(htiChild, pGIChild);
		ASSERT(pGIChild);

		// keep track of earliest start date and latest due date
		if (pGIChild)
			pGI->MinMaxDates(*pGIChild);

		// next child
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}
}

int CGanttTreeListCtrl::GetExpandedState(CDWordArray& aExpanded, HTREEITEM hti) const
{
	int nStart = 0;

	if (hti == NULL)
	{
		// guestimate initial size
		aExpanded.SetSize(0, m_tree.GetCount() / 4);
	}
	else if (TCH().IsItemExpanded(hti) <= 0)
	{
		return 0; // nothing added
	}
	else // expanded
	{
		nStart = aExpanded.GetSize();
		aExpanded.Add(GetTaskID(hti));
	}

	// process children
	HTREEITEM htiChild = m_tree.GetChildItem(hti);

	while (htiChild)
	{
		GetExpandedState(aExpanded, htiChild);
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}

	return (aExpanded.GetSize() - nStart);
}

void CGanttTreeListCtrl::SetExpandedState(const CDWordArray& aExpanded)
{
	int nNumExpanded = aExpanded.GetSize();

	if (nNumExpanded)
	{
		for (int nItem = 0; nItem < nNumExpanded; nItem++)
		{
			HTREEITEM hti = GetTreeItem(aExpanded[nItem]);

			if (hti)
				m_tree.Expand(hti, TVE_EXPAND);
		}

		ExpandList();
	}
}

BOOL CGanttTreeListCtrl::EditWantsResort(IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib) const
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
				return attrib.Has(MapColumnToAttribute(m_sort.single.nBy));

			// else
			for (int nCol = 0; nCol < 3; nCol++)
			{
				if (attrib.Has(MapColumnToAttribute(m_sort.multi.cols[nCol].nBy)))
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

void CGanttTreeListCtrl::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib)
{
	// we must have been initialized already
	ASSERT(m_list.GetSafeHwnd() && m_tree.GetSafeHwnd());

	// always cancel any ongoing operation
	CancelOperation();

	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return;
	}

	switch (nUpdate)
	{
	case IUI_ALL:
		{
			CGanttLockUpdates glu(this, TRUE, TRUE);
			
			CDWordArray aExpanded;
			GetExpandedState(aExpanded);
			
			DWORD dwSelID = GetSelectedTaskID();
			
			RebuildTree(pTasks);

			ValidateMonthDisplay();
			UpdateListColumns();

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
		break;
		
	case IUI_NEW:
	case IUI_EDIT:
		{
			CHoldRedraw hr(m_tree);
			CHoldRedraw hr2(m_list);
			
			// cache current year range to test for changes
			int nNumMonths = GetNumMonths(m_nMonthDisplay);
			
			// update the task(s)
			if (UpdateTask(pTasks, pTasks->GetFirstTask(), nUpdate, attrib, TRUE))
			{
				// recalc parent dates as required
				if (attrib.Has(IUI_STARTDATE) || attrib.Has(IUI_DUEDATE) || attrib.Has(IUI_DONEDATE))
				{
					RecalcDateRange();
					RecalcParentDates();
				}
				
				// Refresh list columns as required
				if (GetNumMonths(m_nMonthDisplay) != nNumMonths)
				{
					ValidateMonthDisplay();
					UpdateListColumns();
				}
			}
		}
		break;
		
	case IUI_DELETE:
		{
			CHoldRedraw hr(m_tree);
			CHoldRedraw hr2(m_list);

			CSet<DWORD> mapIDs;
			BuildTaskMap(pTasks, pTasks->GetFirstTask(), mapIDs, TRUE);
			
			RemoveDeletedTasks(NULL, pTasks, mapIDs);

			// cache current year range to test for changes
			int nNumMonths = GetNumMonths(m_nMonthDisplay);
			
			RefreshTreeItemMap();
			RecalcDateRange();
			RecalcParentDates();

			// fixup list columns as required
			if (GetNumMonths(m_nMonthDisplay) != nNumMonths)
			{
				ValidateMonthDisplay();
				UpdateListColumns();
			}
		}
		break;
		
	default:
		ASSERT(0);
	}
	
	InitItemHeights();
	RecalcTreeColumns(TRUE);

	if (EditWantsResort(nUpdate, attrib))
	{
		ASSERT(m_sort.IsSorting());

		CHoldRedraw hr(m_tree);

		if (m_sort.bMultiSort)
			CTreeListSyncer::Sort(MultiSortProc, (DWORD)this);
		else
			CTreeListSyncer::Sort(SortProc, (DWORD)this);
	}
}

void CGanttTreeListCtrl::PreFixVScrollSyncBug()
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

CString CGanttTreeListCtrl::GetTaskAllocTo(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
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

BOOL CGanttTreeListCtrl::WantEditUpdate(IUI_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case IUI_ALLOCTO:
	case IUI_COLOR:
	case IUI_DEPENDENCY:
	case IUI_DONEDATE:
	case IUI_DUEDATE:
	case IUI_ICON:
	case IUI_ID:
	case IUI_NONE:
	case IUI_PERCENT:
	case IUI_STARTDATE:
	case IUI_SUBTASKDONE:
	case IUI_TAGS:
	case IUI_TASKNAME:
		return TRUE;
	}
	
	// all else 
	return (nAttrib == IUI_ALL);
}

BOOL CGanttTreeListCtrl::WantSortUpdate(IUI_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case IUI_ALLOCTO:
	case IUI_DUEDATE:
	case IUI_ID:
	case IUI_PERCENT:
	case IUI_STARTDATE:
	case IUI_TASKNAME:
	case IUI_TAGS:
	case IUI_DONEDATE:
	case IUI_DEPENDENCY:
		return (MapAttributeToColumn(nAttrib) != GTLCC_NONE);

	case IUI_NONE:
		return TRUE;
	}
	
	// all else 
	return FALSE;
}

IUI_ATTRIBUTE CGanttTreeListCtrl::MapColumnToAttribute(GTLC_COLUMN nCol)
{
	switch (nCol)
	{
	case GTLCC_TITLE:		return IUI_TASKNAME;
	case GTLCC_DUEDATE:		return IUI_DUEDATE;
	case GTLCC_STARTDATE:	return IUI_STARTDATE;
	case GTLCC_ALLOCTO:		return IUI_ALLOCTO;
	case GTLCC_PERCENT:		return IUI_PERCENT;
	case GTLCC_TASKID:		return IUI_ID;
	case GTLCC_DONEDATE:	return IUI_DONEDATE;
	case GTLCC_TAGS:		return IUI_TAGS;
	case GTLCC_DEPENDENCY:	return IUI_DEPENDENCY;
	}
	
	// all else 
	return IUI_NONE;
}

GTLC_COLUMN CGanttTreeListCtrl::MapAttributeToColumn(IUI_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case IUI_TASKNAME:		return GTLCC_TITLE;		
	case IUI_DUEDATE:		return GTLCC_DUEDATE;		
	case IUI_STARTDATE:		return GTLCC_STARTDATE;	
	case IUI_ALLOCTO:		return GTLCC_ALLOCTO;		
	case IUI_PERCENT:		return GTLCC_PERCENT;		
	case IUI_ID:			return GTLCC_TASKID;		
	case IUI_DONEDATE:		return GTLCC_DONEDATE;
	case IUI_TAGS:			return GTLCC_TAGS;
	case IUI_DEPENDENCY:	return GTLCC_DEPENDENCY;
	}
	
	// all else 
	return GTLCC_NONE;
}

BOOL CGanttTreeListCtrl::UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
									IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib, 
									BOOL bAndSiblings)
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
			if (!m_data.HasItem(dwParentID))
			{
				ASSERT(0);
				return FALSE;
			}

			htiParent = GetTreeItem(dwParentID);

			if (!htiParent)
			{
				ASSERT(0);
				return FALSE;
			}
		}

		// Before anything else we increment the position of 
		// any tasks having the same position of this new task 
		// or greater within the parent
		int nPos = pTasks->GetTaskPosition(hTask);
		IncrementItemPositions(htiParent, nPos);

		BuildTreeItem(pTasks, hTask, htiParent, FALSE, FALSE);
		RefreshTreeItemMap();

		return TRUE;
	}
	
	GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, FALSE);

	// update taskID to refID 
	if (pGI->dwOrgRefID)
	{
		dwTaskID = pGI->dwOrgRefID;
		pGI->dwOrgRefID = 0;
	}

	// take a snapshot we can check changes against
	GANTTITEM giOrg = *pGI;

	// can't use a switch here because we also need to check for IUI_ALL
	time64_t tDate = 0;
	
	if (attrib.Has(IUI_TASKNAME))
		pGI->sTitle = pTasks->GetTaskTitle(hTask);
	
	if (attrib.Has(IUI_ALLOCTO))
		pGI->sAllocTo = GetTaskAllocTo(pTasks, hTask);
	
	if (attrib.Has(IUI_ICON))
		pGI->bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

	if (attrib.Has(IUI_PERCENT))
		pGI->nPercent = pTasks->GetTaskPercentDone(hTask, TRUE);
		
	if (attrib.Has(IUI_STARTDATE))
	{
		if (pTasks->GetTaskStartDate64(hTask, pGI->bParent, tDate))
		{
			pGI->dtStart = pGI->dtMinStart = GetDate(tDate, FALSE); // start of day
		}
		else
		{
			CDateHelper::ClearDate(pGI->dtStart);
			CDateHelper::ClearDate(pGI->dtMinStart);
		}
	}
	
	if (attrib.Has(IUI_DUEDATE))
	{
		if (pTasks->GetTaskDueDate64(hTask, pGI->bParent, tDate))
		{
			pGI->dtDue = pGI->dtMaxDue = GetDate(tDate, TRUE); // end of day
		}
		else
		{
			CDateHelper::ClearDate(pGI->dtDue);
			CDateHelper::ClearDate(pGI->dtMaxDue);
		}
	}
	
	if (attrib.Has(IUI_DONEDATE))
	{
		if (pTasks->GetTaskDoneDate64(hTask, tDate))
			pGI->dtDone = GetDate(tDate, TRUE);
		else
			CDateHelper::ClearDate(pGI->dtDone);
	}
	
	if (attrib.Has(IUI_SUBTASKDONE))
	{
		LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
		pGI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));
	}

	if (attrib.Has(IUI_TAGS))
	{
		int nTag = pTasks->GetTaskTagCount(hTask);
		pGI->aTags.RemoveAll();
		
		while (nTag--)
			pGI->aTags.Add(pTasks->GetTaskTag(hTask, nTag));
	}
	
	if (attrib.Has(IUI_DEPENDENCY))
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

	// update date range
	m_dateRange.MinMax(*pGI);
	
	// always update lock states
	pGI->bLocked = pTasks->IsTaskLocked(hTask, true);

	// always update colour because it can change for so many reasons
	pGI->color = pTasks->GetTaskTextColor(hTask);

	// likewise 'Good as Done'
	pGI->bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);

	// detect update
	BOOL bChange = !(*pGI == giOrg);
		
	// children
	if (UpdateTask(pTasks, pTasks->GetFirstTask(hTask), nUpdate, attrib, TRUE))
		bChange = TRUE;

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			if (UpdateTask(pTasks, hSibling, nUpdate, attrib, FALSE))
				bChange = TRUE;
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
	
	return bChange;
}

void CGanttTreeListCtrl::BuildTaskMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
									  CSet<DWORD>& mapIDs, BOOL bAndSiblings)
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

void CGanttTreeListCtrl::RemoveDeletedTasks(HTREEITEM hti, const ITASKLISTBASE* pTasks, const CSet<DWORD>& mapIDs)
{
	// traverse the tree looking for items that do not 
	// exist in pTasks and delete them
	if (hti && !mapIDs.Has(GetTaskID(hti)))
	{
		DeleteTreeItem(hti);
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

GANTTITEM* CGanttTreeListCtrl::GetGanttItem(DWORD dwTaskID, BOOL bCopyRefID) const
{
	GANTTITEM* pGI = m_data.GetItem(dwTaskID);
	ASSERT(pGI);
	
	if (pGI)
	{
		// For references we use the 'real' task but with the 
		// original reference reference ID copied over
		DWORD dwRefID = pGI->dwRefID;
		
		if (dwRefID && (dwRefID != dwTaskID) && m_data.Lookup(dwRefID, pGI))
		{
			// copy over the reference id so that the caller can still detect it
			if (bCopyRefID)
			{
				ASSERT((pGI->dwRefID == 0) || (pGI->dwRefID == dwRefID));
				pGI->dwOrgRefID = dwRefID;
			}
		}
		else
		{
			pGI->dwOrgRefID = 0;
		}
	}
	
	return pGI;
}

BOOL CGanttTreeListCtrl::RestoreGanttItem(const GANTTITEM& giPrev)
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

void CGanttTreeListCtrl::RebuildTree(const ITASKLISTBASE* pTasks)
{
	m_tree.DeleteAllItems();
	m_list.DeleteAllItems();

	m_data.RemoveAll();

	m_dwMaxTaskID = 0;

	// cache and reset year range which will get 
	// recalculated as we build the tree
	GANTTDATERANGE prevRange = m_dateRange;
	m_dateRange.Clear();

	BuildTreeItem(pTasks, pTasks->GetFirstTask(), NULL, TRUE);

	// restore previous date range if no data
	if (m_data.GetCount() == 0)
		m_dateRange = prevRange;

	RefreshTreeItemMap();
	RecalcParentDates();
	ExpandList();
	RefreshItemBoldState();
}

void CGanttTreeListCtrl::RefreshTreeItemMap()
{
	TCH().BuildHTIMap(m_mapHTItems);
}

void CGanttTreeListCtrl::RecalcDateRange()
{
	if (m_data.GetCount())
	{
		m_dateRange.Clear();

		POSITION pos = m_data.GetStartPosition();
		GANTTITEM* pGI = NULL;
		DWORD dwTaskID = 0;

		while (pos)
		{
			m_data.GetNextAssoc(pos, dwTaskID, pGI);
			ASSERT(pGI);

			if (pGI)
				m_dateRange.MinMax(*pGI);
		}
	}
}

COleDateTime CGanttTreeListCtrl::GetDate(time64_t tDate, BOOL bEndOfDay)
{
	COleDateTime date = CDateHelper::GetDate(tDate);

	// only implement 'end of day' if the date has no time
	if (CDateHelper::IsDateSet(date) && bEndOfDay && !CDateHelper::DateHasTime(date))
		date = CDateHelper::GetEndOfDay(date);

	return date;
}

void CGanttTreeListCtrl::BuildTreeItem(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
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

	// Except for position
	pGI->nPosition = pTasks->GetTaskPosition(hTask);

	// Only save data for non-references
	if (pGI->dwRefID == 0)
	{
		pGI->sTitle = pTasks->GetTaskTitle(hTask);
		pGI->color = pTasks->GetTaskTextColor(hTask);
		pGI->bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);
		pGI->sAllocTo = GetTaskAllocTo(pTasks, hTask);
		pGI->bParent = pTasks->IsTaskParent(hTask);
		pGI->nPercent = pTasks->GetTaskPercentDone(hTask, TRUE);
		pGI->bLocked = pTasks->IsTaskLocked(hTask, true);
		pGI->bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

		LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
		pGI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));

		time64_t tDate = 0;

		if (pTasks->GetTaskStartDate64(hTask, pGI->bParent, tDate))
			pGI->dtStart = GetDate(tDate, FALSE);

		if (pTasks->GetTaskDueDate64(hTask, pGI->bParent, tDate))
			pGI->dtDue = GetDate(tDate, TRUE);

		if (pTasks->GetTaskDoneDate64(hTask, tDate))
			pGI->dtDone = GetDate(tDate, TRUE);

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
		
		// track earliest and latest dates
		m_dateRange.MinMax(*pGI);
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

	HTREEITEM hti = m_tree.TCH().InsertItem(LPSTR_TEXTCALLBACK, 
											I_IMAGECALLBACK, 
											I_IMAGECALLBACK, 
											dwTaskID, // lParam
											htiParent, 
											htiAfter,
											FALSE,
											FALSE);
	
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

void CGanttTreeListCtrl::IncrementItemPositions(HTREEITEM htiParent, int nFromPos)
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

COleDateTime CGanttTreeListCtrl::GetStartDate(GTLC_MONTH_DISPLAY nDisplay) const
{
	return m_dateRange.GetStart(nDisplay, !HasOption(GTLCF_DECADESAREONEBASED));
}

COleDateTime CGanttTreeListCtrl::GetEndDate(GTLC_MONTH_DISPLAY nDisplay) const
{
	return m_dateRange.GetEnd(nDisplay, !HasOption(GTLCF_DECADESAREONEBASED));
}

int CGanttTreeListCtrl::GetStartYear(GTLC_MONTH_DISPLAY nDisplay) const
{
	return GetStartDate(nDisplay).GetYear();
}

int CGanttTreeListCtrl::GetEndYear(GTLC_MONTH_DISPLAY nDisplay) const
{
	int nYear = GetEndDate(nDisplay).GetYear();

	// for now, do not let end year exceed MAX_YEAR
	return min(nYear, MAX_YEAR);
}

int CGanttTreeListCtrl::GetNumMonths(GTLC_MONTH_DISPLAY nDisplay) const
{
	COleDateTime dtStart(GetStartDate(nDisplay)), dtEnd(GetEndDate(nDisplay));
	
	int nStartMonth = dtStart.GetMonth(), nStartYear = dtStart.GetYear();
	int nEndMonth = dtEnd.GetMonth(), nEndYear = dtEnd.GetYear();

	int nNumMonths = ((((nEndYear * 12) + nEndMonth) - ((nStartYear * 12) + nStartMonth)) + 1);
	ASSERT(nNumMonths > 0);

	return nNumMonths;
}

void CGanttTreeListCtrl::SetOption(DWORD dwOption, BOOL bSet)
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
			case GTLCF_STRIKETHRUDONETASKS:
				m_tree.Fonts().Clear();
				Invalidate(FALSE);
				break;

			case GTLCF_DECADESAREONEBASED:
				if ((m_nMonthDisplay == GTLC_DISPLAY_QUARTERCENTURIES) || 
					(m_nMonthDisplay == GTLC_DISPLAY_DECADES))
				{
					UpdateListColumnsWidthAndText();
				}
				break;

			case GTLCF_DISABLEDEPENDENTDRAGGING:
				return;

			case GTLCF_SHOWSPLITTERBAR:
				CTreeListSyncer::SetSplitBarWidth(bSet ? 10 : 0);
				break;

			case GTLCF_DISPLAYISODATES:
				UpdateListColumnsWidthAndText();
				Invalidate(FALSE);
				break;
			}

			if (IsSyncing())
				RedrawList();
		}
	}
}

CString CGanttTreeListCtrl::FormatListColumnHeaderText(GTLC_MONTH_DISPLAY nDisplay, int nMonth, int nYear) const
{
	if (nMonth == 0)
		return _T("");
	
	//else
	CString sHeader;
	
	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
	case GTLC_DISPLAY_DECADES:
		{
			int nStartYear = GetStartYear(nDisplay);
			int nEndYear = GetEndYear(nDisplay);

			sHeader.Format(_T("%d-%d"), nStartYear, nEndYear);
		}
		break;

	case GTLC_DISPLAY_YEARS:
		sHeader.Format(_T("%d"), nYear);
		break;
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
		sHeader.Format(_T("Q%d %d"), (1 + ((nMonth-1) / 3)), nYear);
		break;
		
	case GTLC_DISPLAY_QUARTERS_MID:
		sHeader.Format(_T("%s-%s %d"), 
			CDateHelper::GetMonthName(nMonth, TRUE),
			CDateHelper::GetMonthName(nMonth+2, TRUE), 
			nYear);
		break;
		
	case GTLC_DISPLAY_QUARTERS_LONG:
		sHeader.Format(_T("%s-%s %d"), 
			CDateHelper::GetMonthName(nMonth, FALSE),
			CDateHelper::GetMonthName(nMonth+2, FALSE), 
			nYear);
		break;
		
	case GTLC_DISPLAY_MONTHS_SHORT:
		sHeader = FormatDate(COleDateTime(nYear, nMonth, 1, 0, 0, 0), (DHFD_NODAY | DHFD_NOCENTURY));
		break;
		
	case GTLC_DISPLAY_MONTHS_MID:
		sHeader.Format(_T("%s %d"), CDateHelper::GetMonthName(nMonth, TRUE), nYear);
		break;
		
	case GTLC_DISPLAY_MONTHS_LONG:
		sHeader.Format(_T("%s %d"), CDateHelper::GetMonthName(nMonth, FALSE), nYear);
		break;

	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		sHeader.Format(_T("%s %d (%s)"), CDateHelper::GetMonthName(nMonth, FALSE), nYear, CEnString(IDS_GANTT_WEEKS));
		break;

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
	case GTLC_DISPLAY_HOURS:
		sHeader.Format(_T("%s %d (%s)"), CDateHelper::GetMonthName(nMonth, FALSE), nYear, CEnString(IDS_GANTT_DAYS));
		break;
		
	default:
		ASSERT(0);
		break;
	}

	return sHeader;
}

double CGanttTreeListCtrl::GetMonthWidth(int nColWidth) const
{
	return GetMonthWidth(m_nMonthDisplay, nColWidth);
}

double CGanttTreeListCtrl::GetMonthWidth(GTLC_MONTH_DISPLAY nDisplay, int nColWidth)
{
	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		return (nColWidth / (25 * 12.0));
		
	case GTLC_DISPLAY_DECADES:
		return (nColWidth / (10 * 12.0));
		
	case GTLC_DISPLAY_YEARS:
		return (nColWidth / 12.0);
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		return (nColWidth / 3.0);
		
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
		return (double)nColWidth;
	}

	ASSERT(0);
	return 0.0;
}

int CGanttTreeListCtrl::GetRequiredListColumnCount() const
{
	return GetRequiredListColumnCount(m_nMonthDisplay);
}

int CGanttTreeListCtrl::GetRequiredListColumnCount(GTLC_MONTH_DISPLAY nDisplay) const
{
	int nNumMonths = GetNumMonths(nDisplay);
	int nNumCols = 0;

	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		nNumCols = (nNumMonths / (25 * 12));
		break;
		
	case GTLC_DISPLAY_DECADES:
		nNumCols = (nNumMonths / (10 * 12));
		break;
		
	case GTLC_DISPLAY_YEARS:
		nNumCols = (nNumMonths / 12);
		break;
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		nNumCols = (nNumMonths / 3);
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
		nNumCols = nNumMonths;
		break;

	default:
		ASSERT(0);
		break;
	}

	return (nNumCols + 1);
}

int CGanttTreeListCtrl::GetColumnWidth() const
{
	return GetColumnWidth(m_nMonthDisplay, m_nMonthWidth);
}

int CGanttTreeListCtrl::GetColumnWidth(GTLC_MONTH_DISPLAY nDisplay) const
{
	return GetColumnWidth(nDisplay, m_nMonthWidth);
}

int CGanttTreeListCtrl::GetNumMonthsPerColumn(GTLC_MONTH_DISPLAY nDisplay)
{
	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		return (25 * 12);
		
	case GTLC_DISPLAY_DECADES:
		return (10 * 12);
		
	case GTLC_DISPLAY_YEARS:
		return 12;
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		return 3;
		
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
		return 1;
	}

	// else
	ASSERT(0);
	return 1;
}

int CGanttTreeListCtrl::GetColumnWidth(GTLC_MONTH_DISPLAY nDisplay, int nMonthWidth)
{
	return (GetNumMonthsPerColumn(nDisplay) * nMonthWidth);
}

BOOL CGanttTreeListCtrl::GetListColumnDate(int nCol, int& nMonth, int& nYear) const
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

void CGanttTreeListCtrl::BuildTreeColumns()
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

BOOL CGanttTreeListCtrl::IsTreeItemLineOdd(HTREEITEM hti) const
{
	int nItem = GetListItem(hti);

	return IsListItemLineOdd(nItem);
}

BOOL CGanttTreeListCtrl::IsListItemLineOdd(int nItem) const
{
	return ((nItem % 2) == 1);
}

void CGanttTreeListCtrl::SetFocus()
{
	if (!HasFocus())
		m_tree.SetFocus();
}

void CGanttTreeListCtrl::Resize()
{
	CRect rect;
	GetBoundingRect(rect);
	
	Resize(rect);
}

void CGanttTreeListCtrl::Resize(const CRect& rect)
{
	if (m_treeHeader.GetItemCount())
	{
		CTreeListSyncer::Resize(rect, GetSplitPos());

		m_tree.SendMessage(WM_GTCN_TITLECOLUMNWIDTHCHANGE, m_treeHeader.GetItemWidth(0), (LPARAM)m_tree.GetSafeHwnd());
	}
}

void CGanttTreeListCtrl::ExpandAll(BOOL bExpand)
{
	ExpandItem(NULL, bExpand, TRUE);

	RecalcTreeColumns(TRUE);
}

void CGanttTreeListCtrl::ExpandItem(HTREEITEM hti, BOOL bExpand, BOOL bAndChildren)
{
	// clear pick line first
	ClearDependencyPickLine();
	
	// avoid unnecessary processing
	if (hti && !CanExpandItem(hti, bExpand))
		return;

	CAutoFlag af(m_bTreeExpanding, TRUE);
	EnableResync(FALSE);

	CHoldRedraw hr(m_list);
	CHoldRedraw hr2(m_tree);

	TCH().ExpandItem(hti, bExpand, bAndChildren);

	if (bExpand)
	{
		if (hti)
		{
			int nNextIndex = (GetListItem(hti) + 1);
			ExpandList(hti, nNextIndex);
		}
		else
			ExpandList(); // all
	}
	else
	{
		CollapseList(hti);
	}
	
	m_tree.EnsureVisible(hti);

	EnableResync(TRUE, m_tree);
	RecalcTreeColumns(TRUE);
}

BOOL CGanttTreeListCtrl::CanExpandItem(HTREEITEM hti, BOOL bExpand) const
{
	int nFullyExpanded = TCH().IsItemExpanded(hti, TRUE);
			
	if (nFullyExpanded == -1)	// item has no children
	{
		return FALSE; // can neither expand nor collapse
	}
	else if (bExpand)
	{
		return !nFullyExpanded;
	}
			
	// else
	return TCH().IsItemExpanded(hti, FALSE);
}

LRESULT CGanttTreeListCtrl::OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD)
{
	HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
	
	switch (pTVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;
								
	case CDDS_ITEMPREPAINT:
		{
			DWORD dwTaskID = pTVCD->nmcd.lItemlParam;
			GANTTITEM* pGI = NULL;

			GET_GI_RET(dwTaskID, pGI, 0L);
				
 			CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
			CRect rItem(pTVCD->nmcd.rc);

			COLORREF crBack = DrawTreeItemBackground(pDC, hti, *pGI, rItem, rItem, FALSE);
				
			// hide text because we will draw it later
			pTVCD->clrTextBk = pTVCD->clrText = crBack;
		
			return (CDRF_NOTIFYPOSTPAINT | CDRF_NEWFONT); // always
		}
		break;
								
	case CDDS_ITEMPOSTPAINT:
		{
			// check row is visible
			CRect rItem;
			GetTreeItemRect(hti, GTLCC_TITLE, rItem);

			CRect rClient;
			m_tree.GetClientRect(rClient);
			
			if ((rItem.bottom > 0) && (rItem.top < rClient.bottom))
			{
				DWORD dwTaskID = pTVCD->nmcd.lItemlParam;
				GANTTITEM* pGI = NULL;

				GET_GI_RET(dwTaskID, pGI, 0L);
				
				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);

				GM_ITEMSTATE nState = GetItemState(hti);
				BOOL bSelected = (nState != GMIS_NONE);

				// draw horz gridline before selection
				DrawItemDivider(pDC, pTVCD->nmcd.rc, DIV_HORZ, bSelected);

				// Draw icon
				if (pGI->bHasIcon || pGI->bParent)
				{
					int iImageIndex = -1;
					HIMAGELIST hilTask = m_tree.GetTaskIcon(pGI->dwTaskID, iImageIndex);

					if (hilTask && (iImageIndex != -1))
					{
						CRect rItem;
						m_tree.GetItemRect(hti, rItem, TRUE);

						CRect rIcon(rItem);
						rIcon.left -= (IMAGE_SIZE + 2);
						rIcon.bottom = (rIcon.top + IMAGE_SIZE);
						GraphicsMisc::CentreRect(rIcon, rItem, FALSE, TRUE);

						ImageList_Draw(hilTask, iImageIndex, *pDC, rIcon.left, rIcon.top, ILD_TRANSPARENT);
					}
				}
				
				// draw background
				COLORREF crBack = DrawTreeItemBackground(pDC, hti, *pGI, rItem, rClient, bSelected);
				
				// draw gantt item attribute columns
				DrawTreeItem(pDC, hti, *pGI, bSelected, crBack);
			}			
	
			return CDRF_SKIPDEFAULT;
		}
		break;
	}

	return CDRF_DODEFAULT;
}

COLORREF CGanttTreeListCtrl::DrawTreeItemBackground(CDC* pDC, HTREEITEM hti, const GANTTITEM& gi, const CRect& rItem, const CRect& rClient, BOOL bSelected)
{
	BOOL bAlternate = (HasAltLineColor() && !IsTreeItemLineOdd(hti));
	COLORREF crBack = GetTreeTextBkColor(gi, bSelected, bAlternate);

	if (!bSelected)
	{
		// redraw item background else tooltips cause overwriting
		CRect rBack(rItem);
		rBack.bottom--;
		rBack.right = rClient.right;

		pDC->FillSolidRect(rBack, crBack);
	}
	else
	{
		DWORD dwFlags = (GMIB_THEMECLASSIC | GMIB_EXTENDRIGHT | GMIB_CLIPRIGHT);
		GraphicsMisc::DrawExplorerItemBkgnd(pDC, m_tree, GetItemState(hti), rItem, dwFlags);
	}

	return crBack;
}

GM_ITEMSTATE CGanttTreeListCtrl::GetItemState(int nItem) const
{
	if (!m_bSavingToImage)
	{
		if (IsListItemSelected(m_list, nItem))
		{
			if (HasFocus())
				return GMIS_SELECTED;
			else
				return GMIS_SELECTEDNOTFOCUSED;
		}
		else if (ListItemHasState(m_list, nItem, LVIS_DROPHILITED))
		{
			return GMIS_DROPHILITED;
		}
	}

	// else
	return GMIS_NONE;
}

GM_ITEMSTATE CGanttTreeListCtrl::GetItemState(HTREEITEM hti) const
{
	if (!m_bSavingToImage)
	{
		if (IsTreeItemSelected(m_tree, hti))
		{
			if (HasFocus())
				return GMIS_SELECTED;
			else
				return GMIS_SELECTEDNOTFOCUSED;
		}
		else if (TreeItemHasState(m_tree, hti, TVIS_DROPHILITED))
		{
			return GMIS_DROPHILITED;
		}
	}

	// else
	return GMIS_NONE;
}

LRESULT CGanttTreeListCtrl::OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD)
{
	HWND hwndList = pLVCD->nmcd.hdr.hwndFrom;
	int nItem = (int)pLVCD->nmcd.dwItemSpec;
	
	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
#ifdef _DEBUG
		{
			static int nCount = 1;
			TRACE(_T("\nCGanttTreeListCtrl::OnListCustomDraw(begin_%d)\n"), nCount++);
		}
#endif

		return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
								
	case CDDS_ITEMPREPAINT:
		{
			DWORD dwTaskID = GetTaskID(nItem);
			TRACE(_T("CGanttTreeListCtrl::OnListCustomDraw(ID = %ld)\n"), dwTaskID);

			GANTTITEM* pGI = NULL;
			GET_GI_RET(dwTaskID, pGI, 0L);

			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			
			// draw item bkgnd and gridlines full width of list
			COLORREF crBack = GetRowColor(nItem);
			pLVCD->clrTextBk = pLVCD->clrText = crBack;
			
			CRect rItem;
			VERIFY(GetListItemRect(nItem, rItem));

			CRect rFullWidth(rItem);
			GraphicsMisc::FillItemRect(pDC, rFullWidth, crBack, m_list);
			
			// draw horz gridline before selection
			DrawItemDivider(pDC, rFullWidth, DIV_HORZ, FALSE);

			// draw background
			GM_ITEMSTATE nState = GetItemState(nItem);
			DWORD dwFlags = (GMIB_THEMECLASSIC | GMIB_CLIPLEFT);

			GraphicsMisc::DrawExplorerItemBkgnd(pDC, m_list, nState, rItem, dwFlags);

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

			TRACE(_T("CGanttTreeListCtrl::OnListCustomDraw(end)\n"));

			return CDRF_SKIPDEFAULT;
		}
		break;
	}

	return CDRF_DODEFAULT;
}

COLORREF CGanttTreeListCtrl::GetRowColor(int nItem) const
{
	BOOL bAlternate = (!IsListItemLineOdd(nItem) && (m_crAltLine != CLR_NONE));
	COLORREF crBack = (bAlternate ? m_crAltLine : GetSysColor(COLOR_WINDOW));

	return crBack;
}

LRESULT CGanttTreeListCtrl::OnHeaderCustomDraw(NMCUSTOMDRAW* pNMCD)
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
				GTLC_COLUMN nColID = GetColumnID(nCol);
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);
				
				if (m_sort.IsSingleSortingBy(nColID))
					m_treeHeader.DrawItemSortArrow(pDC, nCol, m_sort.single.bAscending);
			}
			break;
		}
	}
	
	return CDRF_DODEFAULT;
}

void CGanttTreeListCtrl::DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar)
{
	GraphicsMisc::DrawSplitBar(pDC, rSplitter, crSplitBar);
}

void CGanttTreeListCtrl::OnHeaderDividerDblClk(NMHEADER* pHDN)
{
	int nCol = pHDN->iItem;
	ASSERT(nCol != -1);
	
	HWND hwnd = pHDN->hdr.hwndFrom;

	if (hwnd == m_treeHeader)
	{
		CClientDC dc(&m_tree);
		RecalcTreeColumnWidth(GetColumnID(nCol), &dc);

		SetSplitPos(m_treeHeader.CalcTotalItemsWidth());
		
		Resize();
	}
	else if (hwnd == m_listHeader)
	{
		if (nCol > 0) // first column always zero width
			m_listHeader.SetItemWidth(nCol, GetColumnWidth());
	}
}

// Called by parent
void CGanttTreeListCtrl::Sort(GTLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending)
{
	Sort(nBy, bAllowToggle, bAscending, FALSE);
}

void CGanttTreeListCtrl::Sort(GTLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending, BOOL bNotifyParent)
{
	// clear pick line first
	ClearDependencyPickLine();

	m_sort.Sort(nBy, bAllowToggle, bAscending);

	// do the sort
	CHoldRedraw hr(m_tree);
	CTreeListSyncer::Sort(SortProc, (DWORD)this);

	// update sort arrow
	m_treeHeader.Invalidate(FALSE);

	if (bNotifyParent)
		GetCWnd()->PostMessage(WM_GTLC_NOTIFYSORT, 0, m_sort.single.nBy);
}

void CGanttTreeListCtrl::Sort(const GANTTSORTCOLUMNS multi)
{
	// clear pick line first
	ClearDependencyPickLine();

	m_sort.Sort(multi);

	// do the sort
	CHoldRedraw hr(m_tree);
	CTreeListSyncer::Sort(MultiSortProc, (DWORD)this);

	// hide sort arrow
	m_treeHeader.Invalidate(FALSE);
}

LRESULT CGanttTreeListCtrl::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTreeListSyncer::WindowProc(hRealWnd, msg, wp, lp);

	ASSERT(hRealWnd == GetHwnd());
	
	// only interested in notifications from the tree/list pair to their parent
	if (wp == m_scLeft.GetDlgCtrlID() || 
		wp == m_scRight.GetDlgCtrlID() ||
		wp == (UINT)m_treeHeader.GetDlgCtrlID() ||
		wp == (UINT)m_listHeader.GetDlgCtrlID())
	{
		switch (msg)
		{
		case WM_NOTIFY:
			{
				LPNMHDR pNMHDR = (LPNMHDR)lp;
				HWND hwnd = pNMHDR->hwndFrom;
				
				// let base class have its turn first
				LRESULT lr = CTreeListSyncer::WindowProc(hRealWnd, msg, wp, lp);

				// our extra handling
				switch (pNMHDR->code)
				{
				case HDN_ENDDRAG:
					if (lr == 0) // success
						m_tree.InvalidateRect(NULL, TRUE);
					break;

				case HDN_ITEMCLICK:
					if (hwnd == m_treeHeader)
					{
						HD_NOTIFY *pHDN = (HD_NOTIFY *) pNMHDR;

						if (pHDN->iButton == 0) // left button
						{
							GTLC_COLUMN nColID = GetColumnID(pHDN->iItem);
							Sort(nColID, TRUE, -1, TRUE);
						}
					}
					break;

				case HDN_ITEMCHANGING:
					if (hwnd == m_treeHeader)
					{
						NMHEADER* pHDN = (NMHEADER*) pNMHDR;

						if (pHDN->iButton == 0) // left button
						{
							if (pHDN->pitem->mask & HDI_WIDTH)
							{
								// don't allow columns get too small
								GTLC_COLUMN nColID = GetColumnID(pHDN->iItem);

								switch (nColID)
								{
								case GTLCC_TITLE:
									if (pHDN->pitem->cxy < TREE_TITLE_MIN_WIDTH)
										return TRUE; // prevent change
									break;

								case GTLCC_STARTDATE:
								case GTLCC_DUEDATE:
								case GTLCC_DONEDATE:
								case GTLCC_ALLOCTO:
								case GTLCC_PERCENT:
								case GTLCC_TASKID:
									if (m_treeHeader.IsItemVisible(pHDN->iItem))
									{
										if (pHDN->pitem->cxy < MIN_COL_WIDTH)
											pHDN->pitem->cxy = MIN_COL_WIDTH;
									}
									break;
								}
							}
						}
					}
					break;

				case HDN_ITEMCHANGED:
					if (hwnd == m_treeHeader)
					{
						SetSplitPos(m_treeHeader.CalcTotalItemsWidth());
						Resize();
						
						m_tree.UpdateWindow();
						m_list.UpdateWindow();
					}
					break;
					
				case HDN_DIVIDERDBLCLICK:
					if (hwnd == m_treeHeader)
					{
						OnHeaderDividerDblClk((NMHEADER*)pNMHDR);
					}
					break;

				case NM_RCLICK:
					if (hwnd == m_treeHeader)
					{
						// pass on to parent
						::SendMessage(hRealWnd, WM_CONTEXTMENU, (WPARAM)hwnd, (LPARAM)::GetMessagePos());
					}
					break;

				case TVN_GETDISPINFO:
					{
						TV_DISPINFO* pDispInfo = (TV_DISPINFO*)pNMHDR;
						DWORD dwTaskID = pDispInfo->item.lParam;

						const GANTTITEM* pGI = NULL;
						GET_GI_RET(dwTaskID, pGI, 0L);

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
					break;

				case TVN_ITEMEXPANDED:
					RecalcTreeColumns(TRUE);
					break;
				}
				
				return lr;
			}
			break;
		}

		// Drag and drop
		if (msg == WM_DD_DRAGENTER)
		{
			// Make sure the selection helper is synchronised
			// with the tree's current selection
			m_tshDragDrop.ClearHistory();
			m_tshDragDrop.RemoveAll(TRUE, FALSE);
			m_tshDragDrop.AddItem(m_tree.GetSelectedItem(), FALSE);

			return m_treeDragDrop.ProcessMessage(GetCurrentMessage());
		}
		else if (msg == WM_DD_PREDRAGMOVE)
		{
			return m_treeDragDrop.ProcessMessage(GetCurrentMessage());
		}
		else if (msg == WM_DD_DRAGOVER)
		{
			// We currently DON'T support 'linking'
			UINT nCursor = m_treeDragDrop.ProcessMessage(GetCurrentMessage());

			if (nCursor == DD_DROPEFFECT_LINK)
				nCursor = DD_DROPEFFECT_NONE;

			return nCursor;
		}
		else if (msg == WM_DD_DRAGDROP)
		{
			if (m_treeDragDrop.ProcessMessage(GetCurrentMessage()))
			{
				HTREEITEM htiDropTarget = NULL, htiAfterSibling = NULL;

				if (m_treeDragDrop.GetDropTarget(htiDropTarget, htiAfterSibling))
				{
					// Notify parent of move
					HTREEITEM htiSel = GetSelectedItem();
					ASSERT(htiSel);

					IUITASKMOVE move = { 0 };

					move.dwSelectedTaskID = GetTaskID(htiSel);
					move.dwParentID = GetTaskID(htiDropTarget);
					move.dwAfterSiblingID = GetTaskID(htiAfterSibling);
					move.bCopy = (Misc::ModKeysArePressed(MKS_CTRL) != FALSE);

					// If copying a task, app will send us a full update 
					// so we do not need to perform the move ourselves
					if (SendMessage(WM_GTLC_MOVETASK, 0, (LPARAM)&move) && !move.bCopy)
					{
						htiSel = TCH().MoveTree(htiSel, htiDropTarget, htiAfterSibling, TRUE, TRUE);

						RefreshTreeItemMap();
						SelectItem(htiSel);
					}
				}
			}
		}
		else if (msg == WM_DD_DRAGABORT)
		{
			return m_treeDragDrop.ProcessMessage(GetCurrentMessage());
		}
	}
	
	return CTreeListSyncer::WindowProc(hRealWnd, msg, wp, lp);
}

void CGanttTreeListCtrl::OnTreeSelectionChange(NMTREEVIEW* /*pNMTV*/)
{
	if (!m_bMovingTask && HasOption(GTLCF_AUTOSCROLLTOTASK))
		ScrollToSelectedTask();
}

void CGanttTreeListCtrl::ClearDependencyPickLine(CDC* pDC)
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

BOOL CGanttTreeListCtrl::IsDependencyPickLinePosValid() const
{
	if (IsPickingDependencyToTask()) 
	{
		return ((m_ptLastDependPick.x != DEPENDPICKPOS_NONE) || 
				(m_ptLastDependPick.y != DEPENDPICKPOS_NONE));
	}

	// else
	return FALSE;
}

void CGanttTreeListCtrl::ResetDependencyPickLinePos()
{
	m_ptLastDependPick.x = m_ptLastDependPick.y = DEPENDPICKPOS_NONE;
}

BOOL CGanttTreeListCtrl::DrawDependencyPickLine(const CPoint& ptClient)
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
			
			depend.SetTo(m_ptLastDependPick);
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

BOOL CGanttTreeListCtrl::SetListTaskCursor(DWORD dwTaskID, GTLC_HITTEST nHit) const
{
	if (nHit != GTLCHT_NOWHERE)
	{
		GTLC_DRAG nDrag = MapHitTestToDrag(nHit);
		ASSERT(IsDragging(nDrag));

		if (dwTaskID != 0)
		{
			if (!CanDragTask(dwTaskID, nDrag))
			{
				if (m_data.ItemIsLocked(dwTaskID))
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

LRESULT CGanttTreeListCtrl::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

	if (hRealWnd == m_list)
	{
		switch (msg)
		{
		case WM_TIMER:
			switch (wp)
			{
			case 0x2A:
			case 0x2B:
				// These are timers internal to the list view associated
				// with editing labels and which cause unwanted selection
				// changes. Given that we have disabled label editing for 
				// the attribute columns we can safely kill these timers
				::KillTimer(hRealWnd, wp);
				return TRUE;
			}
			break;
			
		case WM_NOTIFY:
			{
				LPNMHDR pNMHDR = (LPNMHDR)lp;
				HWND hwnd = pNMHDR->hwndFrom;
				
				// let base class have its turn first
				LRESULT lr = CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

				switch (pNMHDR->code)
				{
				case NM_RCLICK:
					if (hwnd == m_listHeader)
					{
						// pass on to parent
						::SendMessage(GetHwnd(), WM_CONTEXTMENU, (WPARAM)hwnd, (LPARAM)::GetMessagePos());
					}
					break;

				case HDN_DIVIDERDBLCLICK:
					if (hwnd == m_listHeader)
					{
						OnHeaderDividerDblClk((NMHEADER*)pNMHDR);
					}
					break;

				case HDN_ITEMCHANGING:
					if (hwnd == m_listHeader)
					{
						NMHEADER* pHDN = (NMHEADER*)pNMHDR;
						
						// don't let user drag column too narrow
						if ((pHDN->iButton == 0) && (pHDN->pitem->mask & HDI_WIDTH))
						{
							if (m_listHeader.IsItemTrackable(pHDN->iItem) && (pHDN->pitem->cxy < MIN_COL_WIDTH))
								pHDN->pitem->cxy = MIN_COL_WIDTH;

							m_list.Invalidate(FALSE);
						}
					}
					break;

				}
				return lr;
			}
			break;
			
		case WM_ERASEBKGND:
			if (COSVersion() == OSV_LINUX)
			{
				CRect rClient;
				m_list.GetClientRect(rClient);
				
				CDC::FromHandle((HDC)wp)->FillSolidRect(rClient, GetSysColor(COLOR_WINDOW));
			}
			return TRUE;

		case WM_SETCURSOR:
			if (!m_bReadOnly && !IsDependencyEditing())
			{
				CPoint ptCursor(GetMessagePos());
				GTLC_HITTEST nHit = GTLCHT_NOWHERE;

				DWORD dwHitID = ListHitTestTask(ptCursor, TRUE, nHit, TRUE);
				ASSERT((nHit == GTLCHT_NOWHERE) || (dwHitID != 0));

				if (SetListTaskCursor(dwHitID, nHit))
					return TRUE;
			}
			break;

		case WM_LBUTTONDBLCLK:
			if (OnListLButtonDblClk(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONDOWN:
			if (OnListLButtonDown(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONUP:
			if (OnListLButtonUp(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_MOUSEWHEEL:
		case WM_MOUSELEAVE:
			ClearDependencyPickLine();
			break;

		case WM_MOUSEMOVE:
			if (OnListMouseMove(wp, lp))
			{
				return FALSE; // eat
			}
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

		case WM_RBUTTONDOWN:
			{
				GTLC_HITTEST nHit = GTLCHT_NOWHERE;
				DWORD dwTaskID = ListHitTestTask(lp, FALSE, nHit, FALSE);

				ASSERT((nHit == GTLCHT_NOWHERE) || (dwTaskID != 0));

				if (nHit != GTLCHT_NOWHERE)
					SelectTask(dwTaskID);
			}
			break;

		case WM_SETFOCUS:
			m_tree.SetFocus();
			break;
		}
	}
	else if (hRealWnd == m_tree)
	{
		switch (msg)
		{
		case WM_RBUTTONDOWN:
			{
				DWORD dwTaskID = TreeHitTestTask(lp, FALSE);

				if (dwTaskID)
					SelectTask(dwTaskID);
			}
			break;

		case WM_LBUTTONDOWN:
			if (OnTreeLButtonDown(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONUP:
			if (OnTreeLButtonUp(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONDBLCLK:
			if (OnTreeLButtonDblClk(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_MOUSEMOVE:
			if (OnTreeMouseMove(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_MOUSELEAVE:
			// Remove any drophilighting from the list
			if (m_nPrevDropHilitedItem != -1)
			{
				m_list.SetItemState(m_nPrevDropHilitedItem, 0, LVIS_DROPHILITED);
				m_nPrevDropHilitedItem = -1;
			}
			break;

		case WM_MOUSEWHEEL:
			// if we have a horizontal scrollbar but NOT a vertical scrollbar
			// then we need to redraw the whole tree to prevent artifacts
			if (HasHScrollBar(hRealWnd) && !HasVScrollBar(hRealWnd))
			{
				CHoldRedraw hr(hRealWnd, NCR_PAINT | NCR_UPDATE);

				return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
			}
			break;

		case WM_HSCROLL:
			{
				CHoldRedraw hr(hRealWnd, NCR_PAINT | NCR_UPDATE);

				return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
			}
			break;

		case WM_SETCURSOR:
			if (!m_bReadOnly)
			{
				CPoint ptCursor(GetMessagePos());
				DWORD dwTaskID = TreeHitTestTask(ptCursor, TRUE);

				if (dwTaskID && m_data.ItemIsLocked(dwTaskID))
					return GraphicsMisc::SetAppCursor(_T("Locked"), _T("Resources\\Cursors"));
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
						dwScrollID = HitTestTask(::GetMessagePos());
					}

					// For reasons I don't understand, the resource context is
					// wrong when handling the mousewheel
					AFX_MANAGE_STATE(AfxGetStaticModuleState());

					// do the zoom
					ZoomIn(zDelta > 0);

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
					GetCWnd()->SendMessage(WM_GTLC_NOTIFYZOOM, nPrevDisplay, m_nMonthDisplay);
				}
				else
				{
					CHoldHScroll hhs(m_tree);
					
					return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
				}
			}
		}
		break;

	case WM_KEYUP:
		{
			LRESULT lr = CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

			NMTVKEYDOWN tvkd = { 0 };

			tvkd.hdr.hwndFrom = hRealWnd;
			tvkd.hdr.idFrom = ::GetDlgCtrlID(hRealWnd);
			tvkd.hdr.code = TVN_KEYUP;
			tvkd.wVKey = LOWORD(wp);
			tvkd.flags = lp;

			SendMessage(WM_NOTIFY, ::GetDlgCtrlID(hRealWnd), (LPARAM)&tvkd);
			return lr;
		}
		
	case WM_VSCROLL:
		{
			CHoldHScroll hhs(m_tree);
			
			return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
		}
		break;
	}
	
	return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
}

void CGanttTreeListCtrl::SetDropHilite(HTREEITEM hti, int nItem)
{
	if (m_nPrevDropHilitedItem != -1)
		m_list.SetItemState(m_nPrevDropHilitedItem, 0, LVIS_DROPHILITED);
	
	m_tree.SelectDropTarget(hti);
	
	if (nItem != -1)
		m_list.SetItemState(nItem, LVIS_DROPHILITED, LVIS_DROPHILITED);
	
	m_nPrevDropHilitedItem = nItem;
}

BOOL CGanttTreeListCtrl::OnTreeMouseMove(UINT /*nFlags*/, CPoint point)
{
	if (!m_bReadOnly)
	{
		if (IsPickingDependencyFromTask() || IsPickingDependencyToTask())
		{
			int nHotItem = -1;
			HTREEITEM htiHot = m_tree.HitTest(point);
			
			if (htiHot)
				nHotItem = GetListItem(GetTaskID(htiHot));
			
			SetDropHilite(htiHot, nHotItem);
			
			// track when the cursor leaves the tree ctrl
			TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, m_tree, 0 };
			TrackMouseEvent(&tme);

			return TRUE; // eat
		}
	}

	// not handled
	return FALSE;
}

BOOL CGanttTreeListCtrl::OnTreeLButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM hti = m_tree.HitTest(point, &nFlags);
	
	// Don't process if expanding an item
	if (nFlags & TVHT_ONITEMBUTTON)
		return FALSE;

	if (!m_bReadOnly)
	{
		if (IsPickingDependencyFromTask())
		{
			DWORD dwFromTaskID = TreeHitTestTask(point, FALSE);
			
			if (dwFromTaskID)
			{
				if (m_data.ItemIsLocked(dwFromTaskID))
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
				else if (m_pDependEdit->SetFromTask(dwFromTaskID))
				{
					SetDropHilite(NULL, -1);
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

	if (!(nFlags & TVHT_ONITEMBUTTON))
	{
		if (hti && (hti != GetTreeSelItem(m_tree)))
		{
			SelectTreeItem(m_tree, hti);
			return TRUE;
		}
	}

	// not handled
	return FALSE;
}

BOOL CGanttTreeListCtrl::OnTreeLButtonUp(UINT nFlags, CPoint point)
{
	HTREEITEM hti = m_tree.HitTest(point, &nFlags);

	if (!m_bReadOnly && (nFlags & TVHT_ONITEMSTATEICON))
	{
		DWORD dwTaskID = GetTaskID(hti);
		const GANTTITEM* pGI = m_data.GetItem(dwTaskID);
		ASSERT(pGI);
		
		if (pGI)
			GetCWnd()->SendMessage(WM_GTLC_COMPLETIONCHANGE, (WPARAM)m_tree.GetSafeHwnd(), !pGI->IsDone(FALSE));
		
		return TRUE; // eat
	}

	// not handled
	return FALSE;
}

BOOL CGanttTreeListCtrl::OnTreeLButtonDblClk(UINT nFlags, CPoint point)
{
	HTREEITEM hti = m_tree.HitTest(point, &nFlags);
				
	if (!(nFlags & (TVHT_ONITEM | TVHT_ONITEMRIGHT)))
		return FALSE;
	
	if (!TCH().TreeCtrl().ItemHasChildren(hti))
	{
		m_tree.EditLabel(hti);
		return TRUE;
	}
	else
	{
		ExpandItem(hti, !TCH().IsItemExpanded(hti), TRUE);
		return TRUE;
	}

	// not handled
	return FALSE;
}

BOOL CGanttTreeListCtrl::OnListMouseMove(UINT /*nFlags*/, CPoint point)
{
	if (!m_bReadOnly)
	{
		if (IsDependencyEditing())
		{
			if (IsPickingDependencyFromTask() || IsPickingDependencyToTask())
			{
				HTREEITEM htiHot = NULL;
				int nHotItem = m_list.HitTest(point);
				
				if (nHotItem != -1)
					htiHot = CTreeListSyncer::GetTreeItem(m_tree, m_list, nHotItem);
				
				if (DrawDependencyPickLine(point))
				{
					nHotItem = -1; // doesn't play well with the pick line
					
					// track when the cursor leaves the list ctrl
					TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, m_list, 0 };
					TrackMouseEvent(&tme);
				}
				
				SetDropHilite(htiHot, nHotItem);
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

BOOL CGanttTreeListCtrl::OnListLButtonDown(UINT /*nFlags*/, CPoint point)
{
	if (!m_bReadOnly)
	{
		if (IsPickingDependencyFromTask())
		{
			CPoint ptScreen(point);
			m_list.ClientToScreen(&ptScreen);

			DWORD dwFromTaskID = HitTestTask(ptScreen);

			if (dwFromTaskID)
			{
				if (m_data.ItemIsLocked(dwFromTaskID))
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
				else if (m_pDependEdit->SetFromTask(dwFromTaskID))
				{
					SetDropHilite(NULL, -1);
					ResetDependencyPickLinePos();
				}
			}

			return TRUE; // eat
		}
		else if (IsPickingFromDependency())
		{
			DWORD dwCurToTaskID = 0;
			DWORD dwFromTaskID = ListDependsHitTest(point, dwCurToTaskID);
			
			if (dwFromTaskID && dwCurToTaskID)
			{
				if (m_data.ItemIsLocked(dwFromTaskID))
				{
					MessageBeep(MB_ICONEXCLAMATION);
				}
				else if (m_pDependEdit->SetFromDependency(dwFromTaskID, dwCurToTaskID))
				{
					// initialise last drag pos
					ResetDependencyPickLinePos();
					
					// redraw to hide the dependency being edited
					RedrawList(FALSE);

					// Then redraw dependency
					DrawDependencyPickLine(point);
				}
			}
			
			return TRUE; // eat
		}
		else if (IsPickingDependencyToTask())
		{
			GTLC_HITTEST nHit = GTLCHT_NOWHERE;
			DWORD dwToTaskID = ListHitTestTask(point, FALSE, nHit, TRUE);
			
			if (dwToTaskID && (nHit != GTLCHT_NOWHERE))
			{
				m_pDependEdit->SetToTask(dwToTaskID);
			}
			
			return TRUE; // eat
		}
		else if (StartDragging(point))
		{
			return TRUE; // eat
		}
	}
	
	// don't let the selection to be set to -1
	{
		CPoint ptScreen(point);
		m_list.ClientToScreen(&ptScreen);
		
		if (HitTestTask(ptScreen) == 0)
		{
			SetFocus();
			return TRUE; // eat
		}
	}

	// not handled
	return FALSE;
}

BOOL CGanttTreeListCtrl::OnListLButtonUp(UINT /*nFlags*/, CPoint point)
{
	if (IsDragging() && EndDragging(point))
	{
		return TRUE; // eat
	}

	// not handled
	return FALSE;
}

BOOL CGanttTreeListCtrl::OnListLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	if (IsDependencyEditing())
		return FALSE;

	int nHit = m_list.HitTest(point);
	
	if (nHit == -1)
		return FALSE;

	HTREEITEM hti = CTreeListSyncer::GetTreeItem(m_tree, m_list, nHit);
	ASSERT(hti == GetTreeSelItem(m_tree));

	if (TCH().TreeCtrl().ItemHasChildren(hti))
	{
		ExpandItem(hti, !TCH().IsItemExpanded(hti));
		return TRUE;
	}

	// not handled
	return FALSE;
}

BOOL CGanttTreeListCtrl::GetLabelEditRect(LPRECT pEdit) const
{
	HTREEITEM htiSel = GetSelectedItem();
	
	// scroll into view first
	m_tree.EnsureVisible(htiSel);
	
	if (m_tree.GetItemRect(htiSel, pEdit, TRUE)) // label only
	{
		// make width of tree column or 200 whichever is larger
		int nWidth = (m_treeHeader.GetItemWidth(0) - pEdit->left);
		nWidth = max(nWidth, MIN_LABEL_EDIT_WIDTH);

		pEdit->right = (pEdit->left + nWidth);

		// convert from tree to 'our' coords
		m_tree.ClientToScreen(pEdit);
		ScreenToClient(pEdit);

		return true;
	}
	
	return false;
}


void CGanttTreeListCtrl::SetAlternateLineColor(COLORREF crAltLine)
{
	SetColor(m_crAltLine, crAltLine);
}

void CGanttTreeListCtrl::SetGridLineColor(COLORREF crGridLine)
{
	SetColor(m_crGridLine, crGridLine);
}

void CGanttTreeListCtrl::SetTodayColor(COLORREF crToday)
{
	SetColor(m_crToday, crToday);
}

void CGanttTreeListCtrl::SetWeekendColor(COLORREF crWeekend)
{
	SetColor(m_crWeekend, crWeekend);
}

void CGanttTreeListCtrl::SetNonWorkingHoursColor(COLORREF crNonWorkingHoursColor)
{
	SetColor(m_crNonWorkingHoursColor, crNonWorkingHoursColor);
}

void CGanttTreeListCtrl::SetDefaultColor(COLORREF crDefault)
{
	SetColor(m_crDefault, crDefault);
}

void CGanttTreeListCtrl::SetSplitBarColor(COLORREF crSplitBar) 
{ 
	CTreeListSyncer::SetSplitBarColor(crSplitBar); 
}

void CGanttTreeListCtrl::SetMilestoneTag(const CString& sTag)
{
	if (sTag != m_sMilestoneTag)
	{
		m_sMilestoneTag = sTag;

		if (IsHooked())
			InvalidateAll();
	}
}

void CGanttTreeListCtrl::SetParentColoring(GTLC_PARENTCOLORING nOption, COLORREF color)
{
	SetColor(m_crParent, color);

	if (IsHooked() && (m_nParentColoring != nOption))
		InvalidateAll();

	m_nParentColoring = nOption;
}

void CGanttTreeListCtrl::SetColor(COLORREF& color, COLORREF crNew)
{
	if (IsHooked() && (crNew != color))
		InvalidateAll();

	color = crNew;
}

CString CGanttTreeListCtrl::FormatDate(const COleDateTime& date, DWORD dwFlags) const
{
	dwFlags &= ~DHFD_ISO;
	dwFlags |= (HasOption(GTLCF_DISPLAYISODATES) ? DHFD_ISO : 0);

	return CDateHelper::FormatDate(date, dwFlags);
}

CString CGanttTreeListCtrl::GetTreeItemColumnText(const GANTTITEM& gi, GTLC_COLUMN nCol) const
{
	CString sItem;

	switch (nCol)
	{
		case GTLCC_TITLE:
			sItem = gi.sTitle;
			break;

		case GTLCC_TASKID:
			sItem.Format(_T("%ld"), gi.dwTaskID);
			break;
			
		case GTLCC_STARTDATE:
		case GTLCC_DUEDATE:
			{
				COleDateTime dtStart, dtDue;
				GetTaskStartDueDates(gi, dtStart, dtDue);

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

void CGanttTreeListCtrl::DrawTreeItem(CDC* pDC, HTREEITEM hti, const GANTTITEM& gi, BOOL bSelected, COLORREF crBack)
{
	int nNumCol = m_treeHeader.GetItemCount();

	for (int nCol = 0; nCol < nNumCol; nCol++)
		DrawTreeItemText(pDC, hti, nCol, gi, bSelected, crBack);
}

void CGanttTreeListCtrl::DrawTreeItemText(CDC* pDC, HTREEITEM hti, int nCol, const GANTTITEM& gi, BOOL bSelected, COLORREF crBack)
{
	CRect rItem;
	GetTreeItemRect(hti, nCol, rItem);

	if (rItem.Width() == 0)
		return;

	DrawItemDivider(pDC, rItem, DIV_VERT_LIGHT, bSelected);

	GTLC_COLUMN nColID = GetColumnID(nCol);
	BOOL bTitleCol = (nColID == GTLCC_TITLE);

	// draw item background colour
	if (!bSelected && (crBack != CLR_NONE))
	{
		CRect rBack(rItem);

		if (bTitleCol)
			GetTreeItemRect(hti, GTLCC_TITLE, rBack, TRUE); // label only
		
		// don't overwrite gridlines
		if (m_crGridLine != CLR_NONE)
			rBack.DeflateRect(0, 0, 1, 1);
		
		pDC->FillSolidRect(rBack, crBack);
	}
	
	if (rItem.Width() <= MIN_COL_WIDTH)
		return;

	// draw text
	CString sItem = GetTreeItemColumnText(gi, nColID);

	if (!sItem.IsEmpty())
	{
		if (bTitleCol)
			rItem.DeflateRect(2, 2, 1, 0);
		else
			rItem.DeflateRect(LV_COLPADDING, 2, LV_COLPADDING, 0);

		// text color and alignment
		BOOL bLighter = FALSE; 
		UINT nFlags = (DT_LEFT | DT_VCENTER | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(m_tree));

		switch (nColID)
		{
		case GTLCC_TITLE:
			nFlags |= DT_END_ELLIPSIS;
			break;

		case  GTLCC_TASKID:
			nFlags |= DT_RIGHT;
			break;
			
		case GTLCC_STARTDATE:
		case GTLCC_DUEDATE:
		case GTLCC_DONEDATE:
			{
				// draw non-selected calculated dates lighter
				if (!bSelected && !gi.IsDone(TRUE))
				{
					bLighter = (gi.bParent && HasOption(GTLCF_CALCPARENTDATES));

					if (!bLighter)
					{
						if (nColID == GTLCC_STARTDATE)
							bLighter = (!gi.HasStart() && HasOption(GTLCF_CALCMISSINGSTARTDATES));
						else
							bLighter = (!gi.HasDue() && HasOption(GTLCF_CALCMISSINGDUEDATES));
					}
				}
				
				// Right-align if the column width can show the entire date
				// else keep left align to ensure day and month remain visible
				if (rItem.Width() >= pDC->GetTextExtent(sItem).cx)
					nFlags |= DT_RIGHT;
			}
			break;
			
		case GTLCC_PERCENT:
			nFlags |= DT_CENTER;
			break;
		}

		COLORREF crText = GetTreeTextColor(gi, bSelected, bLighter);
		COLORREF crOldColor = pDC->SetTextColor(crText);
		HGDIOBJ hFontOld = pDC->SelectObject(GetTreeItemFont(hti, gi, nColID));
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(sItem, rItem, nFlags);
		pDC->SetTextColor(crOldColor);
		pDC->SelectObject(hFontOld);
	}

	// special case: drawing shortcut icon for reference tasks
	if (bTitleCol && gi.dwOrgRefID)
	{
		GetTreeItemRect(hti, nCol, rItem, TRUE);
		CPoint ptIcon(rItem.left, rItem.bottom - 32);

		ShellIcons::DrawIcon(pDC, ShellIcons::SI_SHORTCUT, ptIcon, true);
	}
}

CGanttTreeListCtrl::DIV_TYPE CGanttTreeListCtrl::GetVerticalDivider(int nMonth, int nYear) const
{
	switch (m_nMonthDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		{
			if (nMonth == 12)
			{
				if (nYear == (GetEndYear(m_nMonthDisplay)))
					return DIV_VERT_DARK;

				return DIV_VERT_MID;
			}
			else if ((nMonth % 3) == 0)
			{
				return DIV_VERT_LIGHT;
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
					return DIV_VERT_DARK;

				return DIV_VERT_MID;
			}

			// else
			return DIV_VERT_LIGHT;
		}
		break;

	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		{
			if (nMonth == 12)
				return DIV_VERT_DARK;
			
			if ((nMonth % 3) == 0)
				return DIV_VERT_MID;

			// else
			return DIV_VERT_LIGHT;
		}
		break;

	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		{
			if (nMonth == 12)
				return DIV_VERT_DARK;
	
			if (nMonth == 6)
				return DIV_VERT_MID;
		
			// else
			return DIV_VERT_LIGHT;
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
				return DIV_VERT_DARK;
			
			// else
			return DIV_VERT_MID;
		}
		break;
		
	case GTLC_DISPLAY_HOURS:
		return DIV_VERT_DARK;
	}

	ASSERT(0);
	return DIV_NONE;
}

BOOL CGanttTreeListCtrl::IsVerticalDivider(DIV_TYPE nType)
{
	switch (nType)
	{
	case DIV_VERT_LIGHT:
	case DIV_VERT_MID:
	case DIV_VERT_DARK:
		return TRUE;
	}

	// else
	return FALSE;
}

void CGanttTreeListCtrl::DrawItemDivider(CDC* pDC, const CRect& rItem, DIV_TYPE nType, BOOL bSelected)
{
	if (!HasGridlines() || (nType == DIV_NONE) || (IsVerticalDivider(nType) && (rItem.right < 0)))
		return;

	COLORREF color = m_crGridLine;

	switch (nType)
	{
	case DIV_VERT_LIGHT:
	case DIV_HORZ:
		break;
		
	case DIV_VERT_MID:
		color = GraphicsMisc::Darker(m_crGridLine, 0.25);
		break;
		
	case DIV_VERT_DARK:
		color = GraphicsMisc::Darker(m_crGridLine, 0.5);
		break;
	}

	CRect rDiv(rItem);

	if (nType == DIV_HORZ)
	{
		rDiv.top = (rDiv.bottom - 1);
	}
	else
	{
		rDiv.left = (rDiv.right - 1);
		
		if (bSelected)
			rDiv.DeflateRect(0, 1);
	}

	COLORREF crOld = pDC->GetBkColor();

	pDC->FillSolidRect(rDiv, color);
	pDC->SetBkColor(crOld);
}

CString CGanttTreeListCtrl::GetLongestVisibleAllocTo(HTREEITEM hti) const
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

HFONT CGanttTreeListCtrl::GetTreeItemFont(HTREEITEM hti, const GANTTITEM& gi, GTLC_COLUMN nCol)
{
	BOOL bStrikThru = (HasOption(GTLCF_STRIKETHRUDONETASKS) && gi.IsDone(FALSE));
	BOOL bBold = ((nCol == GTLCC_TITLE) && (m_tree.GetParentItem(hti) == NULL));

	return m_tree.Fonts().GetHFont(bBold, FALSE, FALSE, bStrikThru);
}

void CGanttTreeListCtrl::GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText) const
{
	rItem.SetRectEmpty();

	if (m_tree.GetItemRect(hti, rItem, TRUE)) // text rect only
	{
		GTLC_COLUMN nColID = GetColumnID(nCol);

		switch (nColID)
		{
		case GTLCC_TITLE:
			{
				int nColWidth = m_treeHeader.GetItemWidth(0); // always
	
				if (!bText)
					rItem.right = nColWidth;
				else
					rItem.right = min(rItem.right, nColWidth);
			}
			break;

		case GTLCC_TASKID:
		case GTLCC_STARTDATE:
		case GTLCC_DUEDATE:
		case GTLCC_DONEDATE:
		case GTLCC_ALLOCTO:
		case GTLCC_PERCENT:
			{
				CRect rHdrItem;
				m_treeHeader.GetItemRect(nCol, rHdrItem);
				
				rItem.left = rHdrItem.left;
				rItem.right = rHdrItem.right;
			}
			break;

		case GTLCC_NONE:
		default:
			ASSERT(0);
			break;
		}
	}

	if (m_bSavingToImage)
		rItem.OffsetRect(-1, 0);
}

void CGanttTreeListCtrl::DrawListItemYears(CDC* pDC, const CRect& rItem, 
											int nYear, int nNumYears, const GANTTITEM& gi,
											BOOL bSelected, BOOL bRollup, BOOL& bToday)
{
	double dYearWidth = (rItem.Width() / (double)nNumYears);
	CRect rYear(rItem);

	for (int j = 0; j < nNumYears; j++)
	{
		if (j == (nNumYears - 1))
			rYear.right = rItem.right;
		else
			rYear.right = (rItem.left + (int)(dYearWidth * (j + 1)));

		DrawListItemYear(pDC, rYear, (nYear + j), gi, bSelected, bRollup, bToday);

		// next year
		rYear.left = rYear.right; 
	}
}

void CGanttTreeListCtrl::DrawListItemYear(CDC* pDC, const CRect& rYear, int nYear, 
											const GANTTITEM& gi, BOOL bSelected, 
											BOOL bRollup, BOOL& bToday)
{
	DrawListItemMonths(pDC, rYear, 1, 12, nYear, gi, bSelected, bRollup, bToday);
}

void CGanttTreeListCtrl::DrawListItemMonths(CDC* pDC, const CRect& rItem, 
											int nMonth, int nNumMonths, int nYear, 
											const GANTTITEM& gi, BOOL bSelected, 
											BOOL bRollup, BOOL& bToday)
{
	double dMonthWidth = (rItem.Width() / (double)nNumMonths);
	CRect rMonth(rItem);

	for (int i = 0; i < nNumMonths; i++)
	{
		if ((nMonth + i) == 12)
			rMonth.right = rItem.right;
		else
			rMonth.right = (rItem.left + (int)(dMonthWidth * (i + 1)));

		DrawListItemMonth(pDC, rMonth, (nMonth + i), nYear, gi, bSelected, bRollup, bToday);

		// next item
		rMonth.left = rMonth.right; 
	}
}

void CGanttTreeListCtrl::DrawListItemMonth(CDC* pDC, const CRect& rMonth, 
											int nMonth, int nYear, 
											const GANTTITEM& gi, BOOL bSelected, 
											BOOL bRollup, BOOL& bToday)
{
	if (!bRollup)
	{
		DIV_TYPE nDiv = GetVerticalDivider(nMonth, nYear);
		DrawItemDivider(pDC, rMonth, nDiv, bSelected);

		if (!bToday)
			bToday = DrawToday(pDC, rMonth, nMonth, nYear, bSelected);
	}

	DrawGanttBar(pDC, rMonth, nMonth, nYear, gi);
	DrawGanttDone(pDC, rMonth, nMonth, nYear, gi);
}

void CGanttTreeListCtrl::DrawListItemWeeks(CDC* pDC, const CRect& rMonth, 
											int nMonth, int nYear, 
											const GANTTITEM& gi, BOOL bSelected, 
											BOOL bRollup, BOOL& bToday)
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

		// draw divider
		if (!bRollup)
		{
			// fill weekends if not selected
			if (!bSelected)
				DrawWeekend(pDC, dtDay, rDay);

			if ((dtDay.GetDayOfWeek() == nFirstDOW) && (nDay > 1))
			{
				rDay.right = rDay.left; // draw at start of day
				DrawItemDivider(pDC, rDay, DIV_VERT_LIGHT, bSelected);
			}
		}

		// next day
		dtDay += 1;
	}

	DrawListItemMonth(pDC, rMonth, nMonth, nYear, gi, bSelected, bRollup, bToday);
}

BOOL CGanttTreeListCtrl::DrawWeekend(CDC* pDC, const COleDateTime& dtDay, const CRect& rDay)
{
	COLORREF color = ((m_crWeekend != CLR_NONE) ? m_crWeekend : m_crNonWorkingHoursColor);

	if ((color != CLR_NONE) && CDateHelper::IsWeekend(dtDay))
	{
		// don't overdraw gridlines
		CRect rWeekend(rDay);

		if (HasGridlines())
			rWeekend.bottom--;

		pDC->FillSolidRect(rWeekend, color);
		return TRUE;
	}

	return FALSE;
}

void CGanttTreeListCtrl::DrawListItemDays(CDC* pDC, const CRect& rMonth, 
											int nMonth, int nYear, const GANTTITEM& gi, 
											BOOL bSelected, BOOL bRollup, 
											BOOL& bToday, BOOL bDrawHours)
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
				BOOL bDrawNonWorkingHours = (!bSelected && (m_crNonWorkingHoursColor != CLR_NONE));

				// fill weekends if not selected
				if (!bSelected)
					bDrawNonWorkingHours &= ~DrawWeekend(pDC, dtDay, rDay);

				if (bDrawHours)
				{
					CRect rHour(rDay);
					double dHourWidth = (rMonth.Width() / (nNumDays * 24.0));

					int nStartHour = -1, nEndHour = -1;

					if (bDrawNonWorkingHours)
					{
						CTimeHelper th;

						nStartHour = (int)th.GetStartOfWorkday(FALSE);
						nEndHour = (int)(th.GetEndOfWorkday(FALSE) + 0.5);

						if (m_crGridLine != CLR_NONE)
							rHour.bottom--;
					}
					
					// draw all but the first and last hours dividers
					for (int nHour = 1; nHour <= 24; nHour++)
					{
						rHour.right = (rMonth.left + (int)((dDayWidth * (nDay - 1)) + (dHourWidth * nHour)));

						if (bDrawNonWorkingHours)
						{
							if ((nHour < (nStartHour + 1)) || (nHour > nEndHour))
								pDC->FillSolidRect(rHour, m_crNonWorkingHoursColor);
						}

						if (nHour != 24)
							DrawItemDivider(pDC, rHour, DIV_VERT_LIGHT, bSelected);
						
						rHour.left = rHour.right;
					}
				}

				// draw all but the last day divider
				if (nDay < nNumDays)
					DrawItemDivider(pDC, rDay, (bDrawHours ? DIV_VERT_MID : DIV_VERT_LIGHT), bSelected);
			}

			// next day
			dtDay.m_dt += 1;
			rDay.left = rDay.right;
		}
	}

	DrawListItemMonth(pDC, rMonth, nMonth, nYear, gi, bSelected, bRollup, bToday);
}

void CGanttTreeListCtrl::DrawListItem(CDC* pDC, int nItem, const GANTTITEM& gi, BOOL bSelected)
{
	ASSERT(nItem != -1);
	int nNumCol = GetRequiredListColumnCount();

	// Rollups for collapsed parents
	HTREEITEM htiRollUp = NULL;
	CRect rClip;

	if (HasOption(GTLCF_DISPLAYPARENTROLLUPS) && gi.bParent)
	{
		HTREEITEM htiParent = CTreeListSyncer::GetTreeItem(m_tree, m_list, nItem);
		ASSERT(htiParent);

		if (htiParent && !TCH().IsItemExpanded(htiParent))
		{
			htiRollUp = htiParent;
			pDC->GetClipBox(rClip);
		}
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
		VERIFY(GetListItemRect(nItem, rItem));

		COLORREF crRow = (bSelected ? CLR_NONE : GetRowColor(nItem));

		if (htiRollUp)
			DrawListItemRollupText(pDC, htiRollUp, rItem, rClip, crRow);
		else
			DrawListItemText(pDC, gi, rItem, rClip, crRow);
	}
}

void CGanttTreeListCtrl::DrawListItemRollupText(CDC* pDC, HTREEITEM htiParent, const CRect& rItem, const CRect& rClip, COLORREF crRow)
{
	HTREEITEM htiChild = m_tree.GetChildItem(htiParent);

	while (htiChild)
	{
		if (m_tree.GetChildItem(htiChild))
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

void CGanttTreeListCtrl::DrawListItemText(CDC* pDC, const GANTTITEM& gi, const CRect& rItem, const CRect& rClip, COLORREF crRow)
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

	if ((nTextPos < 0) || (!rClip.IsRectNull() && (nTextPos > rClip.right)))
		return;

	CRect rText(rItem);
	rText.left = (nTextPos + 3);
	rText.top += 2;

	COLORREF crFill, crBorder;
	GetGanttBarColors(gi, crBorder, crFill);

	pDC->SetBkMode((crRow == CLR_NONE) ? TRANSPARENT : OPAQUE);
	pDC->SetTextColor(crBorder);
	pDC->SetBkColor(crRow);
	pDC->DrawText(sTrailing, rText, (DT_LEFT | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(m_list)));
}

void CGanttTreeListCtrl::DrawListItemRollup(CDC* pDC, HTREEITEM htiParent, int nCol, const CRect& rColumn, BOOL bSelected)
{
	HTREEITEM htiChild = m_tree.GetChildItem(htiParent);

	while (htiChild)
	{
		if (m_tree.GetChildItem(htiChild))
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

BOOL CGanttTreeListCtrl::DrawListItemColumn(CDC* pDC, int nItem, int nCol, const GANTTITEM& gi, 
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

BOOL CGanttTreeListCtrl::DrawListItemColumnRect(CDC* pDC, int nCol, const CRect& rColumn, const GANTTITEM& gi, 
												BOOL bSelected, BOOL bRollup)
{
	// get the date range for this column
	int nMonth = 0, nYear = 0;
	
	if (!GetListColumnDate(nCol, nMonth, nYear))
		return FALSE;

	int nSaveDC = pDC->SaveDC();

	double dMonthWidth = GetMonthWidth(rColumn.Width());
	BOOL bToday = FALSE;

	// Use higher resolution where possible
	GTLC_MONTH_DISPLAY nCalcDisplay = GetColumnDisplay((int)dMonthWidth);
	BOOL bUseHigherRes = (CompareDisplays(nCalcDisplay, m_nMonthDisplay) > 0);

	switch (m_nMonthDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		DrawListItemYears(pDC, rColumn, nYear, 25, gi, bSelected, bRollup, bToday);
		break;
		
	case GTLC_DISPLAY_DECADES:
		DrawListItemYears(pDC, rColumn, nYear, 10, gi, bSelected, bRollup, bToday);
		break;
		
	case GTLC_DISPLAY_YEARS:
		DrawListItemYear(pDC, rColumn, nYear, gi, bSelected, bRollup, bToday);
		break;
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		DrawListItemMonths(pDC, rColumn, nMonth, 3, nYear, gi, bSelected, bRollup, bToday);
		break;
		
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		if (bUseHigherRes)
			DrawListItemWeeks(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bToday);
		else
			DrawListItemMonth(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bToday);
		break;
		
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		if (bUseHigherRes)
			DrawListItemDays(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bToday, FALSE);
		else
			DrawListItemWeeks(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bToday);
		break;

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		DrawListItemDays(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bToday, bUseHigherRes);
		break;

	case GTLC_DISPLAY_HOURS:
		DrawListItemDays(pDC, rColumn, nMonth, nYear, gi, bSelected, bRollup, bToday, TRUE);
		break;
	
	default:
		ASSERT(0);
		break;
	}

	pDC->RestoreDC(nSaveDC);

	return TRUE;
}

void CGanttTreeListCtrl::DrawListHeaderItem(CDC* pDC, int nCol)
{
	CRect rItem;
	m_listHeader.GetItemRect(nCol, rItem);

	if (nCol == 0)
		return;

	// get the date range for this column
	int nMonth = 0, nYear = 0;
	
	if (!GetListColumnDate(nCol, nMonth, nYear))
		return;

	int nSaveDC = pDC->SaveDC();
	double dMonthWidth = GetMonthWidth(rItem.Width());
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(pDC, m_listHeader);
	
	CThemed th;
	BOOL bThemed = (th.AreControlsThemed() && th.Open(GetCWnd(), _T("HEADER")));
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
			DrawListHeaderRect(pDC, rRange, m_listHeader.GetItemText(nCol), pThemed);

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
					DrawListHeaderRect(pDC, rYear, Misc::Format(nYear + i), pThemed);
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
			DrawListHeaderRect(pDC, rMonth, m_listHeader.GetItemText(nCol), pThemed);

			// draw week elements
			int nNumDays = CDateHelper::GetDaysInMonth(nMonth, nYear);
			double dDayWidth = (rMonth.Width() / (double)nNumDays);

			// first week starts at 'First DOW of month'
			int nFirstDOW = CDateHelper::GetFirstDayOfWeek();
			int nDay = CDateHelper::CalcDayOfMonth(nFirstDOW, 1, nMonth, nYear);

			// If this is column 1 (column 0 is hidden) then we might need
			// to draw part of the preceding week
			if ((nCol == 1) && (nDay != -1))
			{
				rWeek.right = (rWeek.left + (int)((nDay - 1) * dDayWidth));
				DrawListHeaderRect(pDC, rWeek, _T(""), pThemed);
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
					DrawListHeaderRect(pDC, rWeek, Misc::Format(nWeek), pThemed);

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
			DrawListHeaderRect(pDC, rMonth, m_listHeader.GetItemText(nCol), pThemed);

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

					if (m_nMonthDisplay == GTLC_DISPLAY_HOURS)
					{
						COleDateTime dtDay(nYear, nMonth, nDay, 0, 0, 0);
						sHeader = FormatDate(dtDay, DHFD_DOW);
					}
					else if (m_nMonthDisplay == GTLC_DISPLAY_DAYS_LONG)
					{
						COleDateTime dtDay(nYear, nMonth, nDay, 0, 0, 0);
						sHeader = FormatDate(dtDay, DHFD_NOYEAR);
					}
					else
					{
						sHeader.Format(_T("%d"), nDay);
					}

					DrawListHeaderRect(pDC, rDay, sHeader, pThemed);
				}
			}
		}
		break;
		
	default:
		ASSERT(0);
		break;
	}

	pDC->SelectObject(pOldFont); // not sure if this is necessary but play safe
	pDC->RestoreDC(nSaveDC);
}

void CGanttTreeListCtrl::DrawListHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem, CThemed* pTheme)
{
	if (!pTheme)
	{
		pDC->FillSolidRect(rItem, ::GetSysColor(COLOR_3DFACE));
		pDC->Draw3dRect(rItem, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
	}
	else
	{
		pTheme->DrawBackground(pDC, HP_HEADERITEM, HIS_NORMAL, rItem);
	}
	
	// text
	if (!sItem.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

		const UINT nFlags = (DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_CENTER | GraphicsMisc::GetRTLDrawTextFlags(m_listHeader));
		pDC->DrawText(sItem, (LPRECT)(LPCRECT)rItem, nFlags);
	}
}

BOOL CGanttTreeListCtrl::HasDisplayDates(const GANTTITEM& gi) const
{
	if (HasDoneDate(gi))
		return TRUE;

	// else
	COleDateTime dummy1, dummy2;
	return GetTaskStartDueDates(gi, dummy1, dummy2);
}

BOOL CGanttTreeListCtrl::HasDoneDate(const GANTTITEM& gi) const
{
	if (gi.bParent && HasOption(GTLCF_CALCPARENTDATES))
		return FALSE;

	// else
	return CDateHelper::IsDateSet(gi.dtDone);
}

BOOL CGanttTreeListCtrl::GetTaskStartDueDates(const GANTTITEM& gi, COleDateTime& dtStart, COleDateTime& dtDue) const
{
	BOOL bDoneSet = FALSE;

	if (gi.bParent && HasOption(GTLCF_CALCPARENTDATES))
	{
		dtStart = gi.dtMinStart;
		dtDue = gi.dtMaxDue;
	}
	else
	{
		dtStart = gi.dtStart;
		dtDue = gi.dtDue;

		bDoneSet = CDateHelper::IsDateSet(gi.dtDone);
	}
		
	// do we need to calculate due date?
	if (!CDateHelper::IsDateSet(dtDue) && HasOption(GTLCF_CALCMISSINGDUEDATES))
	{
		// always take completed date if that is set
		if (bDoneSet)
		{
			dtDue = gi.dtDone;
		}
		else // take later of start date and today
		{
			dtDue = CDateHelper::GetDateOnly(dtStart);
			CDateHelper::Max(dtDue, CDateHelper::GetDate(DHD_TODAY));
			
			// and move to end of the day
			dtDue = CDateHelper::GetEndOfDay(dtDue);
		}
		
		ASSERT(CDateHelper::IsDateSet(dtDue));
	}
	
	// do we need to calculate start date?
	if (!CDateHelper::IsDateSet(dtStart) && HasOption(GTLCF_CALCMISSINGSTARTDATES))
	{
		// take earlier of due or completed date
		dtStart = CDateHelper::GetDateOnly(dtDue);
		CDateHelper::Min(dtStart, CDateHelper::GetDateOnly(gi.dtDone));

		// take the earlier of that and 'today'
		CDateHelper::Min(dtStart, CDateHelper::GetDate(DHD_TODAY));
		
		ASSERT(CDateHelper::IsDateSet(dtStart));
	}

	return (CDateHelper::IsDateSet(dtStart) && CDateHelper::IsDateSet(dtDue));
}

COLORREF CGanttTreeListCtrl::GetTreeTextBkColor(const GANTTITEM& gi, BOOL bSelected, BOOL bAlternate) const
{
	COLORREF crTextBk = gi.GetTextBkColor(bSelected, HasOption(GTLCF_TASKTEXTCOLORISBKGND));

	if (crTextBk == CLR_NONE)
	{
		if (bAlternate && HasAltLineColor())
		{
			crTextBk = m_crAltLine;
		}
		else if ((m_crDefault != CLR_NONE) && HasOption(GTLCF_TASKTEXTCOLORISBKGND))
		{
			crTextBk = m_crDefault;
		}
		else
		{
			crTextBk = GetSysColor(COLOR_WINDOW);
		}
	}
	
	return crTextBk;
}

COLORREF CGanttTreeListCtrl::GetTreeTextColor(const GANTTITEM& gi, BOOL bSelected, BOOL bLighter) const
{
	COLORREF crText = gi.GetTextColor(bSelected, HasOption(GTLCF_TASKTEXTCOLORISBKGND));
	ASSERT(crText != CLR_NONE);

	if (bSelected)
	{
		crText = GraphicsMisc::GetExplorerItemTextColor(crText, GMIS_SELECTED, GMIB_THEMECLASSIC);
	}
	else if (bLighter)
	{
		crText = GraphicsMisc::Lighter(crText, 0.5);
	}

	return crText;
}

void CGanttTreeListCtrl::GetGanttBarColors(const GANTTITEM& gi, COLORREF& crBorder, COLORREF& crFill) const
{
	// darker shade of the item crText/crBack
	COLORREF crDefFill = gi.GetFillColor();
	COLORREF crDefBorder = gi.GetBorderColor();

	if (crDefFill == CLR_NONE)
	{
		if ((m_crDefault != CLR_NONE) && 
			(!gi.bParent || m_nParentColoring == GTLPC_DEFAULTCOLORING))
		{
			crDefFill = m_crDefault;
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

BOOL CGanttTreeListCtrl::CalcDateRect(const CRect& rMonth, int nMonth, int nYear, 
							const COleDateTime& dtFrom, const COleDateTime& dtTo, CRect& rDate)
{
	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);

	if (nDaysInMonth == 0)
		return FALSE;
	
	COleDateTime dtMonthStart(nYear, nMonth, 1, 0, 0, 0);
	COleDateTime dtMonthEnd(nYear, nMonth, nDaysInMonth, 23, 59, 59); // end of last day

	return CalcDateRect(rMonth, nDaysInMonth, dtMonthStart, dtMonthEnd, dtFrom, dtTo, rDate);
}

BOOL CGanttTreeListCtrl::CalcDateRect(const CRect& rMonth, int nDaysInMonth, 
							const COleDateTime& dtMonthStart, const COleDateTime& dtMonthEnd, 
							const COleDateTime& dtFrom, const COleDateTime& dtTo, CRect& rDate)
{
	if (dtFrom > dtTo || dtTo < dtMonthStart || dtFrom > dtMonthEnd)
		return FALSE;

	double dDayWidth = (rMonth.Width() / (double)nDaysInMonth);

	if (dtFrom > dtMonthStart)
		rDate.left = (rMonth.left + (int)(((dtFrom.m_dt - dtMonthStart.m_dt) * dDayWidth)));
	else
		rDate.left = rMonth.left;

	if (dtTo < dtMonthEnd)
		rDate.right = (rMonth.left + (int)(((dtTo.m_dt - dtMonthStart.m_dt) * dDayWidth)));
	else
		rDate.right = rMonth.right;

	rDate.top = rMonth.top;
	rDate.bottom = rMonth.bottom;

	return (rDate.right > 0);
}

DWORD CGanttTreeListCtrl::ListDependsHitTest(const CPoint& ptClient, DWORD& dwToTaskID)
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

HTREEITEM CGanttTreeListCtrl::GetTreeItem(DWORD dwTaskID) const
{
	HTREEITEM hti = NULL;
	m_mapHTItems.Lookup(dwTaskID, hti);
	
	return hti;
}

int CGanttTreeListCtrl::GetListItem(DWORD dwTaskID) const
{
	HTREEITEM hti = GetTreeItem(dwTaskID);

	return (hti ? GetListItem(hti) : -1);
}

int CGanttTreeListCtrl::BuildVisibleDependencyList(CGanttDependArray& aDepends) const
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

int CGanttTreeListCtrl::BuildVisibleDependencyList(HTREEITEM htiFrom, CGanttDependArray& aDepends) const
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

BOOL CGanttTreeListCtrl::BuildDependency(DWORD dwFromTaskID, DWORD dwToTaskID, GANTTDEPENDENCY& depend) const
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

BOOL CGanttTreeListCtrl::CalcDependencyEndPos(DWORD dwTaskID, GANTTDEPENDENCY& depend, BOOL bFrom, LPPOINT lpp) const
{
	ASSERT(m_data.HasItem(dwTaskID));

	int nItem = GetListItem(dwTaskID);

	if (nItem == -1) // Collapsed 
	{
		// Use first visible parent as surrogate
		HTREEITEM hti = m_mapHTItems.GetItem(dwTaskID);
		ASSERT(hti);

		while (!TCH().IsItemVisible(hti))
			hti = m_tree.GetParentItem(hti);

		DWORD dwParentID = GetTaskID(hti);

		nItem = GetListItem(dwParentID);
		ASSERT(nItem != -1);
	}

	return CalcDependencyEndPos(dwTaskID, nItem, depend, bFrom, lpp);
}

BOOL CGanttTreeListCtrl::CalcDependencyEndPos(DWORD dwTaskID, int nItem, GANTTDEPENDENCY& depend, BOOL bFrom, LPPOINT lpp) const
{
	if (nItem == -1)
		return FALSE;

	const GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, FALSE);

	CRect rItem, rMilestone;
	VERIFY(GetListItemRect(nItem, rItem));

	int nPos = -1;

	if (CalcMilestoneRect(*pGI, rItem, rMilestone))
		nPos = rMilestone.CenterPoint().x;
	else
		nPos = GetScrollPosFromDate(bFrom ? pGI->dtStart : pGI->dtDue) - m_list.GetScrollPos(SB_HORZ);

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

void CGanttTreeListCtrl::DrawGanttBar(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, const GANTTITEM& gi)
{
	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);

	if (nDaysInMonth == 0)
		return;

	COleDateTime dtMonthStart, dtMonthEnd;

	if (!GetMonthDates(nMonth, nYear, dtMonthStart, dtMonthEnd))
		return;

	if (IsMilestone(gi))
	{
		DrawGanttMilestone(pDC, rMonth, nMonth, nYear, gi);
		return;
	}

	// else normal bar
	COleDateTime dtStart, dtDue;
	
	if (!GetTaskStartDueDates(gi, dtStart, dtDue))
		return;

	// check for visibility
	CRect rBar(rMonth);

	if (!CalcDateRect(rMonth, nDaysInMonth, dtMonthStart, dtMonthEnd, dtStart, dtDue, rBar))
		return;

	COLORREF crBorder = RGB(0, 0, 0), crFill = CLR_NONE;
	GetGanttBarColors(gi, crBorder, crFill);
	
	// adjust bar height
	rBar.DeflateRect(0, GraphicsMisc::ScaleByDPIFactor(2), 0, GraphicsMisc::ScaleByDPIFactor(3));

	if (gi.bParent && HasOption(GTLCF_CALCPARENTDATES))
		rBar.DeflateRect(0, 0, 0, GraphicsMisc::ScaleByDPIFactor(5));
	
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

void CGanttTreeListCtrl::DrawGanttParentEnds(CDC* pDC, const GANTTITEM& gi, const CRect& rBar, 
											 const COleDateTime& dtMonthStart, const COleDateTime& dtMonthEnd)
{
	if (!gi.bParent || !HasOption(GTLCF_CALCPARENTDATES))
		return;

	// The parent ends will be obscured by the limiting subtasks
	// so there is no point drawing them
	if (HasOption(GTLCF_DISPLAYPARENTROLLUPS) && !TCH().IsItemExpanded(GetTreeItem(gi.dwTaskID)))
		return;

	COleDateTime dtStart, dtDue;
	GetTaskStartDueDates(gi, dtStart, dtDue);

	BOOL bDrawStart = (dtStart >= dtMonthStart);
	BOOL bDrawEnd = (dtDue <= dtMonthEnd);

	const int ARROW_SIZE = 6;

	if (bDrawStart || bDrawEnd)
	{
		pDC->SelectObject(GetSysColorBrush(COLOR_WINDOWTEXT));
		pDC->SelectStockObject(NULL_PEN);

		if (bDrawStart)
		{
			POINT pt[3] = 
			{ 
				{ rBar.left, rBar.bottom }, 
				{ rBar.left, rBar.bottom + ARROW_SIZE }, 
				{ rBar.left + ARROW_SIZE, rBar.bottom } 
			};
		
			pDC->Polygon(pt, 3);
		}
	
		if (bDrawEnd)
		{
			POINT pt[3] = 
			{ 
				{ rBar.right, rBar.bottom }, 
				{ rBar.right, rBar.bottom + ARROW_SIZE }, 
				{ rBar.right - ARROW_SIZE, rBar.bottom } 
			};
		
			pDC->Polygon(pt, 3);
		}
	}
}

BOOL CGanttTreeListCtrl::GetMonthDates(int nMonth, int nYear, COleDateTime& dtStart, COleDateTime& dtEnd)
{
	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);
	ASSERT(nDaysInMonth);

	if (nDaysInMonth == 0)
		return FALSE;
	
	dtStart.SetDate(nYear, nMonth, 1);
	dtEnd.m_dt = dtStart.m_dt + nDaysInMonth;
	
	return TRUE;
}

void CGanttTreeListCtrl::DrawGanttDone(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, const GANTTITEM& gi)
{
	if (!HasDoneDate(gi) || IsMilestone(gi))
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

void CGanttTreeListCtrl::DrawGanttMilestone(CDC* pDC, const CRect& rMonth, int /*nMonth*/, int /*nYear*/, const GANTTITEM& gi)
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

BOOL CGanttTreeListCtrl::CalcMilestoneRect(const GANTTITEM& gi, const CRect& rMonth, CRect& rMilestone) const
{
	if (!IsMilestone(gi))
		return FALSE;

	rMilestone = rMonth;

	COleDateTime dtDue = ((gi.bParent && HasOption(GTLCF_CALCPARENTDATES)) ? gi.dtMaxDue : gi.dtDue);
	int nEndPos = GetDrawPosFromDate(dtDue);

	// resize to a square
	rMilestone.DeflateRect(0, 3, 0, 4);

	int nHeight = rMilestone.Height();
	rMilestone.left = (nEndPos - (nHeight / 2));
	rMilestone.right = (nEndPos + (nHeight / 2));

	if (rMilestone.right < rMonth.left || rMilestone.left >= rMonth.right)
		return FALSE;
	
	return TRUE;
}

int CGanttTreeListCtrl::GetBestTextPos(const GANTTITEM& gi, const CRect& rMonth) const
{
	COleDateTime dtDue = ((gi.bParent && HasOption(GTLCF_CALCPARENTDATES)) ? gi.dtMaxDue : gi.dtDue);

	if (!CDateHelper::IsDateSet(dtDue))
		return -1;

	int nPos = GetDrawPosFromDate(dtDue);

	CRect rMilestone;

	if (CalcMilestoneRect(gi, rMonth, rMilestone))
	{
		nPos = max(nPos, rMilestone.right);
	}
	else if (gi.IsDone(FALSE) && (gi.dtDone > gi.dtDue))
	{
		nPos = (GetDrawPosFromDate(gi.dtDone) + (DONE_BOX / 2));
	}

	return nPos;
}

BOOL CGanttTreeListCtrl::DrawToday(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, BOOL bSelected)
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
	COleDateTime dtToday = CDateHelper::GetDateOnly(COleDateTime::GetCurrentTime());
	
	// check for overlap
	if (dtToday < dtMonthStart || dtToday > dtMonthEnd)
		return FALSE;

	CRect rToday;

	if (!CalcDateRect(rMonth, nDaysInMonth, dtMonthStart, dtMonthEnd, dtToday, dtToday.m_dt + 1.0, rToday))
		return FALSE;

	// don't overdraw selection or gridlines
	if (bSelected)
	{
		rToday.DeflateRect(0, 1);
	}
	else if (HasGridlines())
	{
		rToday.bottom--;
	}

	COLORREF crBorder = GetColor(m_crToday, 0.0, bSelected);
	COLORREF crFill = GetColor(m_crToday, 0.5, bSelected);

	GraphicsMisc::DrawRect(pDC, rToday, crFill, crBorder, 0, GMDR_LEFT | GMDR_RIGHT);

	return TRUE;
}

COLORREF CGanttTreeListCtrl::GetColor(COLORREF crBase, double dLighter, BOOL bSelected)
{
	COLORREF crResult(crBase);

	if (dLighter > 0.0)
		crResult = GraphicsMisc::Lighter(crResult, dLighter);

	if (bSelected)
		crResult = GraphicsMisc::Darker(crResult, 0.15);

	return crResult;
}

int CGanttTreeListCtrl::GetListItem(HTREEITEM htiFrom) const
{
	if (!htiFrom)
		return -1;

	return CTreeListSyncer::FindListItem(m_list, (DWORD)htiFrom);
}

void CGanttTreeListCtrl::ExpandList()
{
	int nNextIndex = 0;
	ExpandList(NULL, nNextIndex);
}

void CGanttTreeListCtrl::ExpandList(HTREEITEM htiFrom, int& nNextIndex)
{
	CTreeListSyncer::ExpandList(m_list, m_tree, htiFrom, nNextIndex);
}

void CGanttTreeListCtrl::CollapseList(HTREEITEM htiFrom)
{
	CTreeListSyncer::CollapseList(m_list, m_tree, htiFrom);
}

void CGanttTreeListCtrl::DeleteTreeItem(HTREEITEM htiFrom)
{
	ASSERT(htiFrom);

	DWORD dwTaskID = GetTaskID(htiFrom);

	m_tree.DeleteItem(htiFrom);
	VERIFY(m_data.RemoveKey(dwTaskID));
}

BOOL CGanttTreeListCtrl::ZoomIn(BOOL bIn)
{
	GTLC_MONTH_DISPLAY nNewDisplay = (bIn ? GetNextDisplay(m_nMonthDisplay) : GetPreviousDisplay(m_nMonthDisplay));
	ASSERT(nNewDisplay != GTLC_DISPLAY_NONE);

	return SetMonthDisplay(nNewDisplay);
}

BOOL CGanttTreeListCtrl::SetMonthDisplay(GTLC_MONTH_DISPLAY nNewDisplay)
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

BOOL CGanttTreeListCtrl::CanSetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay) const
{
	return CanSetMonthDisplay(nDisplay, GetMinMonthWidth(nDisplay));
}

BOOL CGanttTreeListCtrl::CanSetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay, int nMonthWidth) const
{
	ASSERT(nMonthWidth > 0);

	if (!IsValidDisplay(nDisplay) || (nMonthWidth < 1))
	{
		ASSERT(0);
		return FALSE;
	}

	int nNumReqColumns = (GetRequiredListColumnCount(nDisplay) + 1);
	int nColWidth = GetColumnWidth(nDisplay, nMonthWidth);
	int nTotalReqdWidth = (nNumReqColumns * nColWidth);
	
	return (nTotalReqdWidth <= MAX_HEADER_WIDTH);
}

BOOL CGanttTreeListCtrl::ValidateMonthDisplay(GTLC_MONTH_DISPLAY& nDisplay) const
{
	if (!IsValidDisplay(nDisplay))
	{
		ASSERT(0);
		return FALSE;
	}

	int nWidth = GetMinMonthWidth(nDisplay);
	
	return ValidateMonthDisplay(nDisplay, nWidth);
}

BOOL CGanttTreeListCtrl::ValidateMonthDisplay(GTLC_MONTH_DISPLAY& nDisplay, int& nMonthWidth) const
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

void CGanttTreeListCtrl::ValidateMonthDisplay()
{
	GTLC_MONTH_DISPLAY nPrevDisplay = m_nMonthDisplay, nDisplay = nPrevDisplay;
	int nMonthWidth = m_nMonthWidth;

	VERIFY(ValidateMonthDisplay(nDisplay, nMonthWidth));

	if (m_nMonthDisplay != nDisplay)
	{
		VERIFY(SetMonthDisplay(nDisplay));
		GetCWnd()->SendMessage(WM_GTLC_NOTIFYZOOM, nPrevDisplay, m_nMonthDisplay);
	}
}

BOOL CGanttTreeListCtrl::BeginDependencyEdit(IGanttDependencyEditor* pDependEdit)
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

void CGanttTreeListCtrl::OnEndDepedencyEdit()
{
	m_pDependEdit = NULL;
	
	ResetDependencyPickLinePos();
	SetFocus();

	m_tree.SelectDropTarget(NULL);
	m_list.SetItemState(m_nPrevDropHilitedItem, 0, LVIS_DROPHILITED);
	m_list.Invalidate(TRUE);

	m_nPrevDropHilitedItem = -1;
}

void CGanttTreeListCtrl::EndDependencyEdit()
{
	if (IsDependencyEditing())
		m_pDependEdit->Cancel();

	OnEndDepedencyEdit();
}

BOOL CGanttTreeListCtrl::IsDependencyEditing() const
{
	return (m_pDependEdit && m_pDependEdit->IsPicking());
}

BOOL CGanttTreeListCtrl::IsPickingDependencyFromTask() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingFromTask());
}

BOOL CGanttTreeListCtrl::IsPickingFromDependency() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingFromDependency());
}

BOOL CGanttTreeListCtrl::IsPickingDependencyToTask() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingToTask());
}

BOOL CGanttTreeListCtrl::IsDependencyEditingCancelled() const
{
	return (!m_pDependEdit || m_pDependEdit->IsPickingCancelled());
}

BOOL CGanttTreeListCtrl::IsDependencyEditingComplete() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingCompleted());
}

BOOL CGanttTreeListCtrl::ZoomTo(GTLC_MONTH_DISPLAY nNewDisplay, int nNewMonthWidth)
{
	// validate new display
	if (!ValidateMonthDisplay(nNewDisplay, nNewMonthWidth))
		return FALSE;

	if ((nNewDisplay == m_nMonthDisplay) && (nNewMonthWidth == m_nMonthWidth))
		return TRUE;

	// cache the scroll-pos at the centre of the view so we can restore it
	CRect rClient;
	GetClientRect(rClient);

	COleDateTime dtPos;
	BOOL bRestorePos = GetDateFromScrollPos((m_list.GetScrollPos(SB_HORZ) + (rClient.Width() / 2)), dtPos);

	// always cancel any ongoing operation
	CancelOperation();
	
	// do the update
	CGanttLockUpdates glu(this, FALSE, FALSE);

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
		int nScrollPos = GetScrollPosFromDate(dtPos);

		// Date was at the centre of the view
		nScrollPos -= (rClient.Width() / 2);
		nScrollPos = max(0, nScrollPos);

		if (nScrollPos)
			ListView_Scroll(m_list, (nScrollPos - m_list.GetScrollPos(SB_HORZ)), 0);
	}

	return TRUE;
}

BOOL CGanttTreeListCtrl::ZoomBy(int nAmount)
{
	int nNewMonthWidth = (m_nMonthWidth + nAmount);

	// will this trigger a min-width change?
	GTLC_MONTH_DISPLAY nNewMonthDisplay = GetColumnDisplay(nNewMonthWidth);

	return ZoomTo(nNewMonthDisplay, nNewMonthWidth);
}

void CGanttTreeListCtrl::RecalcListColumnWidths(int nFromWidth, int nToWidth)
{
	// resize the required number of columns
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

GTLC_COLUMN CGanttTreeListCtrl::GetColumnID(int nCol) const
{
	return (GTLC_COLUMN)m_treeHeader.GetItemData(nCol);
}

void CGanttTreeListCtrl::ResizeColumnsToFit()
{
	// tree columns
	CClientDC dc(&m_tree);
	int nCol = m_treeHeader.GetItemCount(), nTotalColWidth = 0;

	while (nCol--)
		nTotalColWidth += RecalcTreeColumnWidth(GetColumnID(nCol), &dc);

	SetSplitPos(nTotalColWidth);
	
	// list columns (except first dummy column)
	nCol = GetRequiredListColumnCount();
	
	while (--nCol > 0)
		m_listHeader.SetItemWidth(nCol, GetColumnWidth());

	Resize();
}

void CGanttTreeListCtrl::OnNotifySplitterChange(int nSplitPos)
{
	CTreeListSyncer::OnNotifySplitterChange(nSplitPos);

	// Adjust 'Title' column to suit
	int nRestOfColsWidth = m_treeHeader.CalcTotalItemsWidth(0);

	CClientDC dc(&m_tree);
	int nMinColWidth = CalcWidestItemTitle(NULL, &dc, FALSE);

	int nTitleColWidth = max(nMinColWidth, (nSplitPos - nRestOfColsWidth));

	m_treeHeader.SetItemWidth(GTLCC_TITLE, nTitleColWidth);
	m_treeHeader.SetItemTracked(GTLCC_TITLE, TRUE);
	m_treeHeader.UpdateWindow();

	m_tree.SendMessage(WM_GTCN_TITLECOLUMNWIDTHCHANGE, nTitleColWidth, (LPARAM)m_tree.GetSafeHwnd());
}

BOOL CGanttTreeListCtrl::RecalcTreeColumns(BOOL bResize)
{
	// Only need recalc non-fixed column widths
	BOOL bTitle = !m_treeHeader.IsItemTracked(GTLCC_TITLE);
	BOOL bAllocTo = !m_treeHeader.IsItemTracked(GTLCC_ALLOCTO);
	BOOL bTaskID = !m_treeHeader.IsItemTracked(GTLCC_TASKID);

	if (bTitle || bAllocTo || bTaskID)
	{
		CClientDC dc(&m_tree);

		if (bTitle)
			RecalcTreeColumnWidth(GTLCC_TITLE, &dc);
			
		if (bAllocTo)
			RecalcTreeColumnWidth(GTLCC_ALLOCTO, &dc);
		
		if (bTaskID)
			RecalcTreeColumnWidth(GTLCC_TASKID, &dc);
		
		if (bResize)
			Resize();

		return TRUE;
	}

	return FALSE;
}

int CGanttTreeListCtrl::RecalcTreeColumnWidth(int nCol, CDC* pDC)
{
	int nColWidth = CalcTreeColumnWidth(nCol, pDC);

	m_treeHeader.SetItemWidth(nCol, nColWidth);

	return nColWidth;
}

int CGanttTreeListCtrl::CalcTreeColumnWidth(int nCol, CDC* pDC) const
{
	ASSERT(pDC);
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(pDC, m_tree);

	int nColWidth = 0;
	GTLC_COLUMN nColID = GetColumnID(nCol);

	switch (nColID)
	{
	case GTLCC_TITLE:
		nColWidth = CalcWidestItemTitle(NULL, pDC, TRUE);
		nColWidth = max(nColWidth, TREE_TITLE_MIN_WIDTH);
		break;

	case GTLCC_TASKID:
		nColWidth = pDC->GetTextExtent(Misc::Format(m_dwMaxTaskID)).cx;
		break;
		
	case GTLCC_ALLOCTO:
		nColWidth = GraphicsMisc::GetAverageMaxStringWidth(GetLongestVisibleAllocTo(NULL), pDC);
		break;
		
	// rest of attributes are fixed width
	case GTLCC_STARTDATE:
	case GTLCC_DUEDATE: 
	case GTLCC_DONEDATE: 
		{
			COleDateTime date(2015, 12, 31, 23, 59, 59);
			nColWidth = GraphicsMisc::GetAverageMaxStringWidth(FormatDate(date), pDC);
		}
		break;
		
	case GTLCC_PERCENT: 
		nColWidth = GraphicsMisc::GetAverageMaxStringWidth(_T("100%"), pDC);
		break;

	default:
		ASSERT(0);
	}

	if (nColWidth < MIN_COL_WIDTH)
		nColWidth = MIN_COL_WIDTH;
	else
		nColWidth += (2 * LV_COLPADDING);
	
	// take max of this and column title
	int nTitleWidth = (m_treeHeader.GetItemTextWidth(nCol, pDC) + (2 * HD_COLPADDING));
	ASSERT(nTitleWidth);

	pDC->SelectObject(pOldFont);

	return max(nTitleWidth, nColWidth);
}

int CGanttTreeListCtrl::CalcWidestItemTitle(HTREEITEM htiParent, CDC* pDC, BOOL bEnd) const
{
	// we only want parents
	HTREEITEM htiChild = m_tree.GetChildItem(htiParent);
	
	if (htiChild == NULL)
		return 0;
	
	// or we only want expanded items
	if (htiParent && !TCH().IsItemExpanded(htiParent, FALSE))
		return 0;
	
	// Prepare font
	HFONT hFont = NULL, hOldFont = NULL;
	
	if (bEnd)
	{
		hFont = m_tree.Fonts().GetHFont((htiParent == NULL) ? GMFS_BOLD : 0);
		hOldFont = (HFONT)pDC->SelectObject(hFont);
	}
	
	// else try children
	int nWidest = 0;
	
	while (htiChild)
	{
		CRect rChild;
		
		if (m_tree.GetItemRect(htiChild, rChild, TRUE)) // text rect only
		{
			int nWidth = 0;

			if (bEnd)
			{
				DWORD dwTaskID = GetTaskID(htiChild);
				const GANTTITEM* pGI = NULL;
			
				GET_GI_RET(dwTaskID, pGI, 0);
			
				int nTextWidth = pDC->GetTextExtent(pGI->sTitle).cx;
				nWidth = max(nTextWidth, (rChild.left + nTextWidth));
			}
			else
			{
				nWidth = (rChild.left + TREE_TITLE_MIN_WIDTH);
			}
			
			int nWidestChild = CalcWidestItemTitle(htiChild, pDC, bEnd); // RECURSIVE CALL
			nWidest = max(max(nWidest, nWidth), nWidestChild);
		}
		
		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}
	
	pDC->SelectObject(hOldFont);
	
	return nWidest;
}

void CGanttTreeListCtrl::UpdateListColumnsWidthAndText(int nWidth)
{
	// first column is always zero width and not trackable
	m_listHeader.SetItemWidth(0, 0);
	m_listHeader.EnableItemTracking(0, FALSE);

	if (nWidth == -1)
		nWidth = GetColumnWidth();

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

			if (bUsePrevWidth)
				nWidth = m_aPrevColWidths[nCol];

			m_listHeader.SetItem(nCol, nWidth, sTitle, dwData);
			m_listHeader.EnableItemTracking(nCol, TRUE);

			nTotalReqdWidth += nWidth;
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

	TRACE(_T("CGanttTreeListCtrl(Total Column Widths = %d)\n"), nTotalReqdWidth);

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

void CGanttTreeListCtrl::BuildListColumns()
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
	int nNumCols = GetRequiredListColumnCount(m_nMonthDisplay);

	for (int i = 0; i < nNumCols; i++)
	{
		lvc.cx = 0;
		lvc.fmt = LVCFMT_CENTER | HDF_STRING;
		lvc.pszText = _T("");
		lvc.cchTextMax = 50;

		int nCol = m_listHeader.GetItemCount();
		m_list.InsertColumn(nCol, &lvc);
	}

	UpdateListColumnsWidthAndText();
}

void CGanttTreeListCtrl::UpdateListColumns(int nWidth)
{
	// cache the scrolled position
	int nScrollPos = m_list.GetScrollPos(SB_HORZ);

	COleDateTime dtPos;
	BOOL bRestorePos = GetDateFromScrollPos(nScrollPos, dtPos);

	if (nWidth == -1)
		nWidth = GetColumnWidth();

	int nNumCols = m_listHeader.GetItemCount();
	int nReqCols = (GetRequiredListColumnCount(m_nMonthDisplay) + 1);
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
		nScrollPos = GetScrollPosFromDate(dtPos);
		m_list.Scroll(CSize(nScrollPos - m_list.GetScrollPos(SB_HORZ), 0));
	}
	else
	{
		m_list.SetScrollPos(SB_HORZ, 0, TRUE);
	}
}

int CGanttTreeListCtrl::GetMinMonthWidth(GTLC_MONTH_DISPLAY nDisplay) const
{
	int nWidth = 0;
	VERIFY(m_mapMinMonthWidths.Lookup(nDisplay, nWidth) && (nWidth >= MIN_MONTH_WIDTH));

	return max(nWidth, MIN_MONTH_WIDTH);
}

void CGanttTreeListCtrl::CalcMinMonthWidths()
{
	CClientDC dcClient(&m_treeHeader);
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
					
					int nWidth = dcClient.GetTextExtent(sText).cx;
					nMinTextWidth = max(nWidth, nMinTextWidth);
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

				int nWidth = dcClient.GetTextExtent(_T(" 31/12 ")).cx;
				nMinMonthWidth = max((nWidth * 31), nMinMonthWidth);
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

GTLC_MONTH_DISPLAY CGanttTreeListCtrl::GetColumnDisplay(int nMonthWidth)
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

int CALLBACK CGanttTreeListCtrl::MultiSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const CGanttTreeListCtrl* pThis = (CGanttTreeListCtrl*)lParamSort;
	const GANTTSORTCOLUMNS& sort = pThis->m_sort.multi;

	int nCompare = 0;

	for (int nCol = 0; ((nCol < 3) && (nCompare == 0)); nCol++)
	{
		if (sort.cols[nCol].nBy == IUI_NONE)
			break;

		nCompare = pThis->CompareTasks(lParam1, lParam2, sort.cols[nCol]);
	}

	return nCompare;
}

int CALLBACK CGanttTreeListCtrl::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const CGanttTreeListCtrl* pThis = (CGanttTreeListCtrl*)lParamSort;
	
	return pThis->CompareTasks(lParam1, lParam2, pThis->m_sort.single);
}

int CGanttTreeListCtrl::CompareTasks(DWORD dwTaskID1, DWORD dwTaskID2, const GANTTSORTCOLUMN& col) const
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
			nCompare = CDateHelper::Compare(pGI1->dtStart, pGI2->dtStart, TRUE, FALSE);
			break;

		case GTLCC_DUEDATE:
			nCompare = CDateHelper::Compare(pGI1->dtDue, pGI2->dtDue, TRUE, TRUE);
			break;

		case GTLCC_DONEDATE:
			nCompare = CDateHelper::Compare(pGI1->dtDue, pGI2->dtDue, TRUE, TRUE);
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
				if (m_data.IsItemDependentOn(pGI2, dwTaskID1))
				{
					TRACE(_T("Sort(Task %d depends on Task %d. Task %d sorts higher\n"), dwTaskID2, dwTaskID1, dwTaskID1);
					nCompare = -1;
				}
				// else if Task1 is dependent on Task2 then Task2 comes first
				else if (m_data.IsItemDependentOn(pGI1, dwTaskID2))
				{
					TRACE(_T("Sort(Task %d depends on Task %d. Task %d sorts higher\n"), dwTaskID1, dwTaskID2, dwTaskID2);
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

int CGanttTreeListCtrl::Compare(const CString& sText1, const CString& sText2)
{
	BOOL bEmpty1 = sText1.IsEmpty();
	BOOL bEmpty2 = sText2.IsEmpty();
		
	if (bEmpty1 != bEmpty2)
		return (bEmpty1 ? 1 : -1);
	
	return Misc::NaturalCompare(sText1, sText2);
}

void CGanttTreeListCtrl::ScrollToToday()
{
	ScrollTo(COleDateTime::GetCurrentTime());
}

void CGanttTreeListCtrl::ScrollToSelectedTask()
{
	ScrollToTask(GetSelectedTaskID());
}

void CGanttTreeListCtrl::ScrollToTask(DWORD dwTaskID)
{
	GANTTITEM* pGI = NULL;
	GET_GI(dwTaskID, pGI);
	
	COleDateTime dtStart, dtDue;
	
	if (GetTaskStartDueDates(*pGI, dtStart, dtDue))
	{
		ScrollTo(dtStart);
	}
	else if (HasDoneDate(*pGI))
	{
		ScrollTo(pGI->dtDone);
	}
}

void CGanttTreeListCtrl::ScrollTo(const COleDateTime& date)
{
	ClearDependencyPickLine();

	COleDateTime dateOnly = CDateHelper::GetDateOnly(date);

	int nStartPos = GetScrollPosFromDate(dateOnly);
	int nEndPos = GetScrollPosFromDate(dateOnly.m_dt + 1.0);

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
		Invalidate(FALSE);
}

BOOL CGanttTreeListCtrl::GetListColumnRect(int nCol, CRect& rColumn, BOOL bScrolled) const
{
	if (m_list.GetSubItemRect(0, nCol, LVIR_BOUNDS, rColumn))
	{
		if (!bScrolled)
		{
			int nScroll = m_list.GetScrollPos(SB_HORZ);
			rColumn.OffsetRect(nScroll, 0);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CGanttTreeListCtrl::GetDateFromScrollPos(int nScrollPos, GTLC_MONTH_DISPLAY nDisplay, int nMonth, int nYear, const CRect& rColumn, COleDateTime& date)
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

BOOL CGanttTreeListCtrl::GetDateFromScrollPos(int nScrollPos, COleDateTime& date) const
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

int CGanttTreeListCtrl::GetDrawPosFromDate(const COleDateTime& date) const
{
	return (GetScrollPosFromDate(date) - m_list.GetScrollPos(SB_HORZ));
}

int CGanttTreeListCtrl::GetScrollPosFromDate(const COleDateTime& date) const
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
				// Column == 25 years
				nDaysInCol = (int)(DAYS_IN_YEAR * 25);
				dDayInCol = (int)(((nYear - GetStartYear(m_nMonthDisplay)) * DAYS_IN_YEAR) + ((nMonth - 1) * DAYS_IN_MONTH) + nDay);
				break;

			case GTLC_DISPLAY_DECADES:
				// Column == 10 years
				nDaysInCol = (int)(DAYS_IN_YEAR * 10);
				dDayInCol = (int)(((nYear - GetStartYear(m_nMonthDisplay)) * DAYS_IN_YEAR) + ((nMonth - 1) * DAYS_IN_MONTH) + nDay);
				break;

			case GTLC_DISPLAY_YEARS:
				// Column == 12 months
				nDaysInCol = (int)DAYS_IN_YEAR;
				dDayInCol = ((int)((nMonth - 1) * DAYS_IN_MONTH) + nDay);
				break;
				
			case GTLC_DISPLAY_QUARTERS_SHORT:
			case GTLC_DISPLAY_QUARTERS_MID:
			case GTLC_DISPLAY_QUARTERS_LONG:
				// Column == 3 months
				nDaysInCol = (int)(DAYS_IN_MONTH * 3);
				dDayInCol = ((int)(((nMonth - 1) % 3) * DAYS_IN_MONTH) + nDay);
				break;

			default: 
				// Column == Month
				nDaysInCol = CDateHelper::GetDaysInMonth(nMonth, nYear);
				dDayInCol = ((nDay - 1) + CDateHelper::GetTimeOnly(date));
				break;
			}

			ASSERT(nDaysInCol > 0);

			if (nDaysInCol > 0)
			{
				double dOffset = ((rColumn.Width() * dDayInCol) / nDaysInCol);
				return (rColumn.left + (int)dOffset);
			}
		}
	}

	// else
	return 0;
}

int CGanttTreeListCtrl::GetDateInMonths(int nMonth, int nYear) const
{
	return ((nYear * 12) + (nMonth - 1));
}

int CGanttTreeListCtrl::FindColumn(int nMonth, int nYear) const
{	
	int nMonths = GetDateInMonths(nMonth, nYear);
	int nNumReqColumns = GetRequiredListColumnCount();

	for (int i = 1; i <= nNumReqColumns; i++)
	{
		// get date for current column
		VERIFY (GetListColumnDate(i, nMonth, nYear));

		if (nMonths >= GetDateInMonths(nMonth, nYear))
		{
			if (i == nNumReqColumns)
			{
				return i;
			}
			else // get date for next column
			{
				VERIFY (GetListColumnDate(i+1, nMonth, nYear));
				
				if (nMonths < GetDateInMonths(nMonth, nYear))
				{
					return i;
				}
			}
		}
	}

	// not found
	return -1;
}

int CGanttTreeListCtrl::FindColumn(const COleDateTime& date) const
{
	return FindColumn(date.GetMonth(), date.GetYear());
}

int CGanttTreeListCtrl::FindColumn(int nScrollPos) const
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

bool CGanttTreeListCtrl::PrepareNewTask(ITaskList* pTaskList) const
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

DWORD CGanttTreeListCtrl::HitTestTask(const CPoint& ptScreen) const
{
	// try list first
	GTLC_HITTEST nHit = GTLCHT_NOWHERE;
	DWORD dwTaskID = ListHitTestTask(ptScreen, TRUE, nHit, FALSE);

	// then tree
	if (!dwTaskID)
		dwTaskID = TreeHitTestTask(ptScreen, TRUE);

	return dwTaskID;
}

DWORD CGanttTreeListCtrl::TreeHitTestTask(const CPoint& ptScreen, BOOL bScreen) const
{
	HTREEITEM htiHit = TreeHitTestItem(ptScreen, bScreen);
	
	if (htiHit)
		return GetTaskID(htiHit);

	return 0;
}

HTREEITEM CGanttTreeListCtrl::TreeHitTestItem(const CPoint& point, BOOL bScreen) const
{
	CPoint ptTree(point);

	if (bScreen)
		m_tree.ScreenToClient(&ptTree);
	
	return m_tree.HitTest(ptTree);
}

HTREEITEM CGanttTreeListCtrl::GetItem(CPoint ptScreen) const
{
	return TreeHitTestItem(ptScreen, TRUE);
}

CString CGanttTreeListCtrl::GetItemTip(CPoint ptScreen) const
{
	HTREEITEM htiHit = GetItem(ptScreen);

	if (htiHit)
	{
		CRect rItem;

		if (m_tree.GetItemRect(htiHit, rItem, TRUE))
		{
			int nColWidth = m_treeHeader.GetItemWidth(0);

			rItem.left = max(rItem.left, 0);
			rItem.right = nColWidth;

			CPoint ptClient(ptScreen);
			m_tree.ScreenToClient(&ptClient);

			if (rItem.PtInRect(ptClient))
			{
				DWORD dwTaskID = GetTaskID(htiHit);
				GANTTITEM* pGI = NULL;

				GET_GI_RET(dwTaskID, pGI, _T(""));

				int nTextLen = GraphicsMisc::GetTextWidth(pGI->sTitle, m_tree);
				rItem.DeflateRect(TV_TIPPADDING, 0);

				if (nTextLen > rItem.Width())
					return pGI->sTitle;
			}
		}
	}

	// else
	return _T("");
}

BOOL CGanttTreeListCtrl::PointInHeader(const CPoint& ptScreen) const
{
	CRect rHeader;

	// try tree
	m_treeHeader.GetWindowRect(rHeader);

	if (rHeader.PtInRect(ptScreen))
		return TRUE;

	// then list
	m_listHeader.GetWindowRect(rHeader);

	return rHeader.PtInRect(ptScreen);
}

void CGanttTreeListCtrl::GetWindowRect(CRect& rWindow, BOOL bWithHeader) const
{
	CRect rTree, rList;

	m_tree.GetWindowRect(rTree);
	m_list.GetWindowRect(rList);

	if (bWithHeader)
		rWindow = rList; // height will include header
	else
		rWindow = rTree; // height will not include header

	rWindow.left  = min(rTree.left, rList.left);
	rWindow.right = max(rTree.right, rList.right);
}

int CGanttTreeListCtrl::ListHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const
{
	if (m_data.GetCount() == 0)
		return -1;

	// convert to list coords
	LVHITTESTINFO lvht = { 0 };
	lvht.pt = point;

	if (bScreen)
		m_list.ScreenToClient(&(lvht.pt));

	if ((ListView_SubItemHitTest(m_list, &lvht) != -1) &&	(lvht.iSubItem > 0))
	{
		ASSERT(lvht.iItem != -1);

		// convert pos to date
		COleDateTime dtHit;

		if (GetDateFromPoint(lvht.pt, dtHit))
		{
			nCol = lvht.iSubItem;
			return lvht.iItem;
		}
	}

	// all else
	return -1;
}

BOOL CGanttTreeListCtrl::GetListItemRect(int nItem, CRect& rItem) const
{
	if (m_list.GetItemRect(nItem, rItem, LVIR_BOUNDS))
	{
		// Extend to end of client rect
		CRect rClient;
		GetClientRect(rClient);

		rItem.right = max(rItem.right, rClient.right);
		return TRUE;
	}

	return FALSE;
}

DWORD CGanttTreeListCtrl::ListHitTestTask(const CPoint& point, BOOL bScreen, GTLC_HITTEST& nHit, BOOL bDragging) const
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

	COleDateTime dtStart, dtDue;
	GetTaskStartDueDates(*pGI, dtStart, dtDue);

	// Calculate the task rect
	CRect rTask;
	VERIFY(GetListItemRect(nItem, rTask));

	if (!CDateHelper::DateHasTime(dtDue))
		dtDue += 1.0;

	rTask.right = GetScrollPosFromDate(dtDue);
	rTask.left = GetScrollPosFromDate(dtStart);

	rTask.OffsetRect(-m_list.GetScrollPos(SB_HORZ), 0);

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

DWORD CGanttTreeListCtrl::GetTaskID(HTREEITEM htiFrom) const
{
	if ((htiFrom == NULL) || (htiFrom == TVI_FIRST) || (htiFrom == TVI_ROOT))
		return 0;

	return GetTreeItemData(m_tree, htiFrom);
}

DWORD CGanttTreeListCtrl::GetTaskID(int nItem) const
{
	return GetListTaskID(GetListItemData(m_list, nItem));
}

DWORD CGanttTreeListCtrl::GetListTaskID(DWORD dwItemData) const
{
	return GetTaskID((HTREEITEM)dwItemData);
}

BOOL CGanttTreeListCtrl::IsDragging() const
{
	return IsDragging(m_nDragging);
}

BOOL CGanttTreeListCtrl::IsDragging(GTLC_DRAG nDrag)
{
	return ((nDrag != GTLCD_ANY) && (nDrag != GTLCD_NONE));
}

BOOL CGanttTreeListCtrl::IsDraggingEnds(GTLC_DRAG nDrag)
{
	return ((nDrag == GTLCD_START) || (nDrag == GTLCD_END));
}

BOOL CGanttTreeListCtrl::IsValidDragPoint(const CPoint& ptDrag) const
{
	if (!IsDragging())
		return FALSE;

	CRect rLimits;
	GetDragLimits(rLimits);

	return rLimits.PtInRect(ptDrag);
}

void CGanttTreeListCtrl::GetDragLimits(CRect& rLimits) const
{
	m_list.GetClientRect(rLimits);

	// Clip the right hand end to the last column
	CRect rColumn;
	m_listHeader.GetItemRect(m_listHeader.GetItemCount() - 1, rColumn);
	rLimits.right = min(rLimits.right, rColumn.right);

	// Allow a buffer at start and end
	rLimits.InflateRect(DRAG_BUFFER, 0);
}

BOOL CGanttTreeListCtrl::ValidateDragPoint(CPoint& ptDrag) const
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

BOOL CGanttTreeListCtrl::CanDragTask(DWORD dwTaskID, GTLC_DRAG nDrag) const
{
	if (m_data.ItemIsLocked(dwTaskID))
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

BOOL CGanttTreeListCtrl::StartDragging(const CPoint& ptCursor)
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
	
	if (!DragDetect(m_list, ptScreen))
		return FALSE;

	// Ensure the gantt item has valid dates
	GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, FALSE);
	
	COleDateTime dtStart, dtDue;
	GetTaskStartDueDates(*pGI, dtStart, dtDue);
	
	if (!pGI->HasDue())
	{
		if (!CDateHelper::IsDateSet(dtDue))
			return FALSE;

		// else
		pGI->dtDue = pGI->dtMaxDue = dtDue;
	}
	
	if (!pGI->HasStart())
	{
		if (!CDateHelper::IsDateSet(dtStart))
			return FALSE;

		// else
		pGI->dtStart = pGI->dtMinStart = dtStart;
	}
	
	// cache the original task and the start point
	m_giPreDrag = *pGI;
	m_ptDragStart = ptCursor;

	// Start dragging
	m_nDragging = nDrag;
	m_dtDragMin = CalcMinDragDate(m_giPreDrag);

	m_list.SetCapture();
	
	// keep parent informed
	NotifyParentDragChange();

	return TRUE;
}

COleDateTime CGanttTreeListCtrl::CalcMinDragDate(const GANTTITEM& gi) const
{
	COleDateTime dtMin;
	CDateHelper::ClearDate(dtMin);

	int nDepend = gi.aDependIDs.GetSize();

	while (nDepend--)
	{
		DWORD dwDependID = gi.aDependIDs[nDepend];
		ASSERT(dwDependID);

		GANTTITEM* pGI = NULL;
		GET_GI_RET(dwDependID, pGI, dtMin);

		CDateHelper::Max(dtMin, pGI->dtDue);
	}

	return dtMin;
}

BOOL CGanttTreeListCtrl::EndDragging(const CPoint& ptCursor)
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
			int nNumMonths = GetNumMonths(m_nMonthDisplay);
			GANTTDATERANGE prevRange = m_dateRange;

			if (!NotifyParentDateChange(nDrag))
			{
				RestoreGanttItem(m_giPreDrag);
			}
			else
			{
				RecalcDateRange();
				RecalcParentDates();

				// Refresh list columns as required
				if ((GetNumMonths(m_nMonthDisplay) != nNumMonths) ||
					(!prevRange.Contains(*pGI)))
				{
					// For reasons I don't understand, the resource context is
					// wrong when handling the LButtonUp
					AFX_MANAGE_STATE(AfxGetStaticModuleState());

					ValidateMonthDisplay();
					UpdateListColumns();
				}
			}

			NotifyParentDragChange();
		}

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CGanttTreeListCtrl::DragDatesDiffer(const GANTTITEM& gi1, const GANTTITEM& gi2)
{
	return ((gi1.dtStart != gi2.dtStart) || (gi1.dtDue != gi2.dtDue));
}

void CGanttTreeListCtrl::NotifyParentDragChange()
{
	ASSERT(!m_bReadOnly);
	ASSERT(GetSelectedTaskID());

	GetCWnd()->SendMessage(WM_GTLC_DRAGCHANGE, (WPARAM)GetSnapMode(), GetSelectedTaskID());
}

BOOL CGanttTreeListCtrl::NotifyParentDateChange(GTLC_DRAG nDrag)
{
	ASSERT(!m_bReadOnly);
	ASSERT(GetSelectedTaskID());

	if (IsDragging(nDrag))
		return GetCWnd()->SendMessage(WM_GTLC_DATECHANGE, (WPARAM)nDrag, (LPARAM)&m_giPreDrag);

	// else
	return 0L;
}

BOOL CGanttTreeListCtrl::UpdateDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);
	ASSERT(!IsDependencyEditing());
	
	if (IsDragging())
	{
		DWORD dwTaskID = GetSelectedTaskID();
		GANTTITEM* pGI = NULL;

		GET_GI_RET(dwTaskID, pGI, FALSE);

		COleDateTime dtStart, dtDue;
		GetTaskStartDueDates(*pGI, dtStart, dtDue);

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
					pGI->dtStart.m_dt = min(dtDrag.m_dt, (dtDue.m_dt - dMinDuration));

					szCursor = IDC_SIZEWE;
				}
				break;

			case GTLCD_END:
				{
					// prevent the start and end dates from overlapping
					double dMinDuration = CalcMinDragDuration();
					pGI->dtDue.m_dt = max(dtDrag.m_dt, (dtStart.m_dt + dMinDuration));

					// handle day boundary
					if (!CDateHelper::DateHasTime(pGI->dtDue))
					{
						pGI->dtDue.m_dt = (CDateHelper::GetEndOfDay(pGI->dtDue).m_dt - 1.0);
					}

					szCursor = IDC_SIZEWE;
				}
				break;

			case GTLCD_WHOLE:
				{
					// preserve task duration
					COleDateTime dtDuration(dtDue - dtStart);

					// handle whole days
					if (CDateHelper::DateHasTime(dtStart) && 
						CDateHelper::DateHasTime(dtDue) && 
						!CDateHelper::DateHasTime(dtDuration))
					{
						dtDuration = CDateHelper::GetEndOfPreviousDay(dtDuration);
					}

					pGI->dtStart = dtDrag;
					pGI->dtDue = (dtDrag + dtDuration);

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

double CGanttTreeListCtrl::CalcMinDragDuration() const
{
	ASSERT((m_nDragging == GTLCD_START) || (m_nDragging == GTLCD_END));
	
	double dMin;

	if (CalcMinDragDuration(GetSnapMode(), dMin))
		return dMin;

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
		dMin = 1.0;
		break;
	}

	return dMin;
}

BOOL CGanttTreeListCtrl::CalcMinDragDuration(GTLC_SNAPMODE nMode, double& dMin)
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
		break;

	default:
		ASSERT(0);
	}

	return (dMin > 0.0);
}

void CGanttTreeListCtrl::RedrawList(BOOL bErase)
{
	m_list.InvalidateRect(NULL, bErase);
	m_list.UpdateWindow();
}

void CGanttTreeListCtrl::RedrawTree(BOOL bErase)
{
	m_tree.InvalidateRect(NULL, bErase);
	m_tree.UpdateWindow();
}

BOOL CGanttTreeListCtrl::GetValidDragDate(const CPoint& ptCursor, COleDateTime& dtDrag) const
{
	ASSERT(IsDragging());
	CPoint ptDrag(ptCursor);

	if (!ValidateDragPoint(ptDrag))
		return FALSE;

	if (!GetDateFromPoint(ptDrag, dtDrag))
		return FALSE;

	// if dragging the whole task, then we calculate
	// dtDrag as GANTTITEM::dtStart offset by the
	// difference between the current drag pos and the
	// initial drag pos
	if (m_nDragging == GTLCD_WHOLE)
	{
		COleDateTime dtOrg;
		GetDateFromPoint(m_ptDragStart, dtOrg);
		
		// offset from pre-drag position
		double dOffset = dtDrag.m_dt - dtOrg.m_dt;
		dtDrag = m_giPreDrag.dtStart.m_dt + dOffset;
	}
	
	// adjust date depending on modifier keys 
	dtDrag = GetNearestDate(dtDrag);
	
	// else
	return TRUE;
}

BOOL CGanttTreeListCtrl::GetDateFromPoint(const CPoint& ptCursor, COleDateTime& date) const
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

void CGanttTreeListCtrl::SetReadOnly(bool bReadOnly) 
{ 
	m_bReadOnly = bReadOnly;

	m_treeDragDrop.EnableDragDrop(!bReadOnly);
}

// external version
BOOL CGanttTreeListCtrl::CancelOperation()
{
	if (m_treeDragDrop.IsDragging())
	{
		m_treeDragDrop.CancelDrag();
		return TRUE;
	}
	else if (IsDragging())
	{
		CancelDrag(TRUE);
		return TRUE;
	}
	else if (IsDependencyEditing())
	{
		EndDependencyEdit();
		return TRUE;
	}
	
	// else 
	return FALSE;
}

// internal version
void CGanttTreeListCtrl::CancelDrag(BOOL bReleaseCapture)
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

int CGanttTreeListCtrl::GetTreeColumnOrder(CIntArray& aTreeOrder) const
{
	return m_treeHeader.GetItemOrder(aTreeOrder);
}

void CGanttTreeListCtrl::SetTreeColumnVisibility(const CDWordArray& aColumnVis)
{
	int nNumCols = aColumnVis.GetSize();

	for (int nColID = 1; nColID < nNumCols; nColID++)
	{
		int nCol = m_treeHeader.FindItem(nColID);
		m_treeHeader.ShowItem(nCol, aColumnVis[nColID]);
	}

	Resize();
}

BOOL CGanttTreeListCtrl::SetTreeColumnOrder(const CIntArray& aTreeOrder)
{
	if (!(aTreeOrder.GetSize() && (aTreeOrder[0] == 0)))
	{
		ASSERT(0);
		return FALSE;
	}

	return m_treeHeader.SetItemOrder(aTreeOrder);
}

void CGanttTreeListCtrl::GetColumnWidths(CIntArray& aTreeWidths, CIntArray& aListWidths) const
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

BOOL CGanttTreeListCtrl::SetColumnWidths(const CIntArray& aTreeWidths, const CIntArray& aListWidths)
{
	if (aTreeWidths.GetSize() != (NUM_TREECOLUMNS + 1))
		return FALSE;

	m_treeHeader.SetItemWidths(aTreeWidths);

	// save list column widths for when we've initialised our columns
	// remember to include hidden dummy first column
	if (aListWidths.GetSize() == (GetRequiredListColumnCount() + 1))
		m_listHeader.SetItemWidths(aListWidths);
	else
		m_aPrevColWidths.Copy(aListWidths);

	return TRUE;
}

void CGanttTreeListCtrl::GetTrackedColumns(CIntArray& aTreeTracked, CIntArray& aListTracked) const
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

BOOL CGanttTreeListCtrl::SetTrackedColumns(const CIntArray& aTreeTracked, const CIntArray& aListTracked)
{
	if (aTreeTracked.GetSize() != (NUM_TREECOLUMNS + 1))
		return FALSE;
	
	m_treeHeader.SetTrackedItems(aTreeTracked); 

	// save list column tracking for when we've initialised our columns
	// remember to include hidden dummy first column
	if (aListTracked.GetSize() == (GetRequiredListColumnCount() + 1))
		m_listHeader.SetTrackedItems(aListTracked);
	else
		m_aPrevTrackedCols.Copy(aListTracked);
	
	return TRUE;
}

GTLC_SNAPMODE CGanttTreeListCtrl::GetSnapMode() const
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

COleDateTime CGanttTreeListCtrl::GetNearestDate(const COleDateTime& dtDrag) const
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

BOOL CGanttTreeListCtrl::IsMilestone(const GANTTITEM& gi) const
{
	if (m_sMilestoneTag.IsEmpty() || (gi.aTags.GetSize() == 0))
		return FALSE;
	
	if (!gi.bParent && !CDateHelper::IsDateSet(gi.dtDue))
		return FALSE;
	
	if (gi.bParent && !CDateHelper::IsDateSet(gi.dtMaxDue))
		return FALSE;
	
	// else
	return Misc::Contains(gi.aTags, m_sMilestoneTag, FALSE, FALSE);
}

DWORD CGanttTreeListCtrl::GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const
{
	HTREEITEM hti = FindTreeItem(m_tree, dwTaskID);
	
	if (!hti)
	{
		ASSERT(0);
		return 0L;
	}

	DWORD dwNextID(dwTaskID);

	switch (nCmd)
	{
	case IUI_GETNEXTTASK:
		{
			HTREEITEM htiNext = TCH().GetNextVisibleItem(hti);
			
			if (htiNext)
				dwNextID = GetTreeItemData(m_tree, htiNext);
		}
		break;
		
	case IUI_GETPREVTASK:
		{
			HTREEITEM htiPrev = TCH().GetPrevVisibleItem(hti);
			
			if (htiPrev)
				dwNextID = GetTreeItemData(m_tree, htiPrev);
		}
		break;
		
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTOPLEVELTASK:
		{
			HTREEITEM htiNext = TCH().GetNextTopLevelItem(hti, (nCmd == IUI_GETNEXTTOPLEVELTASK));
			
			if (htiNext)
				dwNextID = GetTreeItemData(m_tree, htiNext);
		}
		break;
		
	default:
		ASSERT(0);
	}
	
	return dwNextID;
}

BOOL CGanttTreeListCtrl::SaveToImage(CBitmap& bmImage)
{
	if (m_tree.GetCount() == 0)
		return FALSE;

	CLockUpdates lock(m_tree);

	// Resize tree header width to suit title text width
	int nColWidth = m_treeHeader.GetItemWidth(0);
	BOOL bTracked = m_treeHeader.IsItemTracked(0);

	ResizeColumnsToFit();	

	// Calculate the date range in scroll units
	// allow a month's buffer at each end
	COleDateTime dtFrom = CDateHelper::GetStartOfMonth(m_dateRange.GetStart());
	COleDateTime dtTo = CDateHelper::GetEndOfMonth(m_dateRange.GetEnd());

	CDateHelper::IncrementMonth(dtFrom, -1);
	CDateHelper::IncrementMonth(dtTo, 1);

	int nFrom = GetScrollPosFromDate(dtFrom);
	int nTo = GetScrollPosFromDate(dtTo);

	if (nTo == nFrom)
		nTo = -1;
	
	BOOL bRes = CTreeListSyncer::SaveToImage(bmImage, nFrom, nTo);
	
	// Restore title column width
	m_treeHeader.SetItemWidth(0, nColWidth);
	m_treeHeader.SetItemTracked(0, bTracked);

	Resize();
	
	return bRes;
}

void CGanttTreeListCtrl::RefreshItemBoldState(HTREEITEM htiFrom, BOOL bAndChildren)
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

BOOL CGanttTreeListCtrl::SetFont(HFONT hFont, BOOL bRedraw)
{
	if (!hFont || !m_tree.GetSafeHwnd() || !m_list.GetSafeHwnd())
	{
		ASSERT(0);
		return FALSE;
	}
	
	m_tree.SetFont(CFont::FromHandle(hFont), bRedraw);

	CalcMinMonthWidths();
	SetMonthDisplay(m_nMonthDisplay);
	ResizeColumnsToFit();

	return TRUE;
}

void CGanttTreeListCtrl::FilterToolTipMessage(MSG* pMsg)
{
	m_tree.FilterToolTipMessage(pMsg);
	m_treeHeader.FilterToolTipMessage(pMsg);
	m_listHeader.FilterToolTipMessage(pMsg);
}

bool CGanttTreeListCtrl::ProcessMessage(MSG* pMsg) 
{
	return (m_treeDragDrop.ProcessMessage(pMsg) != FALSE);
}

BOOL CGanttTreeListCtrl::CanMoveSelectedItem(const IUITASKMOVE& move) const
{
	return (GetSelectedTaskID() && 
			((move.dwParentID == 0) || m_data.HasItem(move.dwParentID)) &&
			((move.dwAfterSiblingID == 0) || m_data.HasItem(move.dwAfterSiblingID)));
}

BOOL CGanttTreeListCtrl::MoveSelectedItem(const IUITASKMOVE& move)
{
	if (!CanMoveSelectedItem(move))
		return FALSE;

	CAutoFlag af(m_bMovingTask, TRUE);
	
	HTREEITEM htiSel = GetSelectedItem(), htiNew = NULL;
	HTREEITEM htiDestParent = GetTreeItem(move.dwParentID);
	HTREEITEM htiDestAfterSibling = GetTreeItem(move.dwAfterSiblingID);
	
	{
		CHoldRedraw hr2(m_tree, NCR_UPDATE);
		CHoldRedraw hr3(m_list);

		htiNew = TCH().MoveTree(htiSel, htiDestParent, htiDestAfterSibling, TRUE, TRUE);
		ASSERT(htiNew);
	}

	if (htiNew)
	{
		SelectItem(htiNew);
		return TRUE;
	}

	return FALSE;
}
