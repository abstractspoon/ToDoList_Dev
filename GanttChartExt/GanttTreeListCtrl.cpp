// GanttTreeList.cpp: implementation of the CGanttTreeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "GanttTreeListCtrl.h"
#include "GanttMsg.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\DateHelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\graphicsMisc.h"
#include "..\shared\TreeCtrlHelper.h"
#include "..\shared\autoflag.h"
#include "..\shared\misc.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\themed.h"
#include "..\shared\osversion.h"

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

const int MIN_COL_WIDTH			= 6;
const int MIN_LABEL_EDIT_WIDTH	= 200;
const int DEF_MONTH_WIDTH		= 72;
const int TREE_TITLE_MIN_WIDTH	= 75; 
const int COLUMN_PADDING		= 15;
const int MIN_MONTH_WIDTH		= 10;
const int MINS_IN_HOUR			= 60;
const int MINS_IN_DAY			= (MINS_IN_HOUR * 24);
const int LV_COLPADDING			= 3;
const int TV_TIPPADDING			= 3;
const int HD_COLPADDING			= 6;
const int MAX_HEADER_WIDTH		= 32000; // (SHRT_MAX - tolerance)

const LONG DEPENDPICKPOS_NONE = 0xFFFFFFFF;
const double DAY_WEEK_MULTIPLIER = 1.5;
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

#define GET_GD_RET(id, gd, ret)	\
{								\
	if (id == 0) return ret;	\
	gd = GetGanttDisplay(id);	\
	ASSERT(gd);					\
	if (gd == NULL) return ret;	\
}

#define GET_GD(id, gd)			\
{								\
	if (id == 0) return;		\
	gd = GetGanttDisplay(id);	\
	ASSERT(gd);					\
	if (gd == NULL)	return;		\
}

//////////////////////////////////////////////////////////////////////

#define FROMISABOVE(t) ((t == GCDDT_FROMISABOVELEFT) || (t == GCDDT_FROMISABOVERIGHT))

//////////////////////////////////////////////////////////////////////

class CGanttLockUpdates : public CLockUpdates
{
public:
	CGanttLockUpdates(CGanttTreeListCtrl* pCtrl, BOOL bTree, BOOL bAndSync) 
		: 
	CLockUpdates(bTree ? pCtrl->m_hwndTree : pCtrl->m_hwndList),
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

CGanttTreeListCtrl::CGanttTreeListCtrl() 
	:
	CTreeListSyncer(TLSF_SYNCSELECTION | TLSF_SYNCFOCUS | TLSF_BORDER | TLSF_SYNCDATA),
	m_bSortAscending(-1), 
	m_nSortBy(GTLCC_NONE),
	m_pTCH(NULL),
	m_nMonthWidth(DEF_MONTH_WIDTH),
	m_nMonthDisplay(GTLC_DISPLAY_MONTHS_LONG),
	m_dwOptions(GTLCF_AUTOSCROLLTOTASK),
	m_crAltLine(CLR_NONE),
	m_crGridLine(CLR_NONE),
	m_crDefault(CLR_NONE),
	m_crParent(CLR_NONE),
	m_crToday(CLR_NONE),
	m_crWeekend(CLR_NONE),
	m_nParentColoring(GTLPC_DEFAULTCOLORING),
	m_dtEarliest(COleDateTime::GetCurrentTime()),
	m_dtLatest(COleDateTime::GetCurrentTime()),
	m_bDraggingStart(FALSE), 
	m_bDraggingEnd(FALSE),
	m_bDragging(FALSE),
	m_ptDragStart(0),
	m_ptLastDependPick(0),
	m_pDependEdit(NULL),
	m_bReadOnly(FALSE)
{

}

CGanttTreeListCtrl::~CGanttTreeListCtrl()
{
	Release();
}

BOOL CGanttTreeListCtrl::Initialize(HWND hwndTree, HWND hwndList, UINT nIDTreeHeader)
{
	// misc
	m_nMonthWidth = DEF_MONTH_WIDTH;
	m_fonts.Initialise(hwndTree);

	// initialize tree header
	if (!m_treeHeader.SubclassDlgItem(nIDTreeHeader, CWnd::FromHandle(::GetParent(hwndTree))))
		return FALSE;

	m_treeHeader.ModifyStyle(0, (HDS_FULLDRAG | HDS_HOTTRACK | HDS_BUTTONS | HDS_DRAGDROP));

	// keep our own handles to these to speed lookups
	m_hwndList = hwndList;
	m_hwndTree = hwndTree;

	// subclass the tree and list
	if (!CTreeListSyncer::Sync(hwndTree, hwndList, TLSL_RIGHTDATA_IS_LEFTITEM, m_treeHeader))
		return FALSE;

	// subclass the list header
	VERIFY(m_listHeader.SubclassWindow(ListView_GetHeader(hwndList)));
	
	// prevent column reordering on columns
	m_listHeader.ModifyStyle(HDS_DRAGDROP, 0);

	// prevent translation of the list header
	CLocalizer::EnableTranslation(m_listHeader, FALSE);

	// misc
	CWnd::ModifyStyle(hwndTree, TVS_SHOWSELALWAYS, 0, 0);
	CWnd::ModifyStyle(hwndList, LVS_SHOWSELALWAYS, 0, 0);
	ListView_SetExtendedListViewStyleEx(hwndList, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	BuildTreeColumns();
	BuildListColumns();

	CalculateMinMonthWidths();

	if (m_nMonthWidth != DEF_MONTH_WIDTH)
		RecalcListColumnWidths(DEF_MONTH_WIDTH, m_nMonthWidth);

	return TRUE;
}

void CGanttTreeListCtrl::InitItemHeights()
{
	CTreeListSyncer::InitItemHeights();

	GANTTDEPENDENCY::STUB = (TreeView_GetItemHeight(m_hwndTree) / 2);
}

void CGanttTreeListCtrl::Release() 
{ 
	if (::IsWindow(m_treeHeader))
		m_treeHeader.UnsubclassWindow();

	if (::IsWindow(m_listHeader))
		m_listHeader.UnsubclassWindow();

	Unsync(); 
	m_hwndTree = m_hwndList = NULL;

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

BOOL CGanttTreeListCtrl::GetSelectedTaskDependencies(CStringArray& aDepends) const
{
	DWORD dwTaskID = GetSelectedTaskID();
	
	const GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, FALSE);
	
	aDepends.Copy(pGI->aDepends);
	return TRUE;
}

BOOL CGanttTreeListCtrl::SetSelectedTaskDependencies(const CStringArray& aDepends)
{
	DWORD dwTaskID = GetSelectedTaskID();
	
	GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, FALSE);

	pGI->aDepends.Copy(aDepends);
	RedrawList();
	
	return TRUE;
}

BOOL CGanttTreeListCtrl::GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const
{
	DWORD dwTaskID = GetSelectedTaskID();

	const GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, FALSE);
	
	if (GetStartDueDates(*pGI, dtStart, dtDue))
	{
		// handle durations of whole days
		COleDateTime dtDuration(dtDue - dtStart);

		if (dtDuration > CDateHelper::GetEndOfDay(dtDuration))
		{
			double dWholeDays = (CDateHelper::GetDateOnly(dtDuration).m_dt + 1.0);

			if (!CDateHelper::DateHasTime(dtStart))
				dWholeDays--;

			dtDue.m_dt = (dtStart.m_dt + dWholeDays);
		}

		return TRUE;
	}

	// all else
//	ASSERT(0);
	return FALSE;
}

BOOL CGanttTreeListCtrl::SelectTask(DWORD dwTaskID)
{
	HTREEITEM hti = FindTreeItem(m_hwndTree, dwTaskID);

	if (hti == NULL)
		return FALSE;

	BOOL bWasVisible = IsTreeItemVisible(m_hwndTree, hti);

	SelectTreeItem(m_hwndTree, hti);

	if (!bWasVisible)
		ExpandList();

	return TRUE;
}

void CGanttTreeListCtrl::RecalcParentDates()
{
	GANTTITEM dummy;
	GANTTITEM* pGI = &dummy;

	RecalcParentDates(NULL, pGI);
}

void CGanttTreeListCtrl::RecalcParentDates(HTREEITEM htiParent, GANTTITEM*& pGI)
{
	const CTreeCtrl& tree = TCH()->TreeCtrl();

	// ignore root
	DWORD dwTaskID = 0;

	// get gantt item for this tree item
	if (htiParent)
	{
		dwTaskID = tree.GetItemData(htiParent);
		GET_GI(dwTaskID, pGI);
	}
	
	// bail if this is a reference
	if (pGI->dwRefID)
		return;
	
	// reset dates
	pGI->dtMinStart = pGI->dtStart;
	pGI->dtMaxDue = pGI->dtDue;

	// iterate children 
	HTREEITEM htiChild = tree.GetChildItem(htiParent);
	
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
		htiChild = tree.GetNextItem(htiChild, TVGN_NEXT);
	}
}

int CGanttTreeListCtrl::GetExpandedState(CDWordArray& aExpanded, HTREEITEM hti) const
{
	int nStart = 0;

	if (hti == NULL)
	{
		// guestimate initial size
		aExpanded.SetSize(0, TreeView_GetCount(m_hwndTree) / 4);
	}
	else if (TCH()->IsItemExpanded(hti) <= 0)
	{
		return 0; // nothing added
	}
	else // expanded
	{
		nStart = aExpanded.GetSize();
		aExpanded.Add(GetTaskID(hti));
	}

	// process children
	HTREEITEM htiChild = TreeView_GetChild(m_hwndTree, hti);

	while (htiChild)
	{
		GetExpandedState(aExpanded, htiChild);
		htiChild = TreeView_GetNextItem(m_hwndTree, htiChild, TVGN_NEXT);
	}

	return (aExpanded.GetSize() - nStart);
}

void CGanttTreeListCtrl::SetExpandedState(const CDWordArray& aExpanded)
{
	int nNumExpanded = aExpanded.GetSize();

	if (nNumExpanded)
	{
		CHTIMap mapID;
		TCH()->BuildHTIMap(mapID);
		
		HTREEITEM hti = NULL;
		
		for (int nItem = 0; nItem < nNumExpanded; nItem++)
		{
			if (mapID.Lookup(aExpanded[nItem], hti) && hti)
				TreeView_Expand(m_hwndTree, hti, TVE_EXPAND);
		}

		ExpandList();
	}
}

void CGanttTreeListCtrl::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib)
{
	// we must have been initialized already
	ASSERT(m_hwndList && m_hwndTree);

	// always cancel any ongoing operation
	CancelOperation();

	const ITaskList15* pTasks14 = GetITLInterface<ITaskList15>(pTasks, IID_TASKLIST15);
	BOOL bResort = FALSE;
	
	switch (nUpdate)
	{
	case IUI_ALL:
		{
			CGanttLockUpdates glu(this, TRUE, TRUE);
			
			CDWordArray aExpanded;
			GetExpandedState(aExpanded);
			
			DWORD dwSelID = GetSelectedTaskID();
			
			RebuildTree(pTasks14);

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

			bResort = (m_nSortBy != GTLCC_NONE);
		}
		break;
		
	case IUI_NEW:
	case IUI_EDIT:
		{
			CHoldRedraw hr(m_hwndTree);
			CHoldRedraw hr2(m_hwndList);
			
			// cache current year range to test for changes
			int nNumMonths = GetNumMonths();
			
			// update the task(s)
			if (UpdateTask(pTasks14, pTasks14->GetFirstTask(), nUpdate, attrib, TRUE))
			{
				// recalc parent dates as required
				if (attrib.HasKey(IUI_STARTDATE) || attrib.HasKey(IUI_DUEDATE) || attrib.HasKey(IUI_DONEDATE))
				{
					RecalcDateRange();
					RecalcParentDates();
				}
				
				// fixup list columns as required
				if (GetNumMonths() != nNumMonths)
				{
					ValidateMonthDisplay();
					UpdateListColumns();
				}

				bResort = ((m_nSortBy != GTLCC_NONE) && attrib.HasKey(MapColumnToAttrib(m_nSortBy)));
			}
		}
		break;
		
	case IUI_DELETE:
		{
			CHoldRedraw hr(m_hwndTree);
			CHoldRedraw hr2(m_hwndList);

			CSet<DWORD> mapIDs;
			BuildTaskMap(pTasks14, pTasks14->GetFirstTask(), mapIDs, TRUE);
			
			RemoveDeletedTasks(NULL, pTasks14, mapIDs);

			// cache current year range to test for changes
			int nNumMonths = GetNumMonths();
			
			RecalcDateRange();

			// fixup list columns as required
			if (GetNumMonths() != nNumMonths)
			{
				ValidateMonthDisplay();
				UpdateListColumns();
			}
		}
		break;
		
	case IUI_MOVE:
		ASSERT(0);
		break;
		
	default:
		ASSERT(0);
	}
	
	InitItemHeights();
	RecalcTreeColumns(TRUE);

	if (bResort)
		Sort(m_nSortBy, FALSE);
}

void CGanttTreeListCtrl::PreFixVScrollSyncBug()
{
	// Odd bug: The very last tree item will not scroll into view. 
	// Expanding and collapsing an item is enough to resolve the issue.
	HTREEITEM hti = TCH()->FindFirstParent();
		
	if (hti)
	{
		TCH()->ExpandItem(hti, TRUE);
		TCH()->ExpandItem(hti, FALSE);
	}
}

CString CGanttTreeListCtrl::GetTaskAllocTo(const ITaskList15* pTasks, HTASKITEM hTask)
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
	case IUI_TASKNAME:
	case IUI_DONEDATE:
	case IUI_DUEDATE:
	case IUI_STARTDATE:
	case IUI_ALLOCTO:
	case IUI_COLOR:
	case IUI_TAGS:
	case IUI_DEPENDENCY:
	case IUI_PERCENT:
	case IUI_NONE:
		// 	case IUI_TIMEEST:
		// 	case IUI_TIMESPENT:
		return TRUE;
	}
	
	// all else 
	return (nAttrib == IUI_ALL);
}

BOOL CGanttTreeListCtrl::WantSortUpdate(IUI_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case IUI_TASKNAME:
	case IUI_DUEDATE:
	case IUI_STARTDATE:
	case IUI_ALLOCTO:
	case IUI_PERCENT:
	case IUI_NONE:
		return TRUE;
	}
	
	// all else 
	return FALSE;
}

IUI_ATTRIBUTE CGanttTreeListCtrl::MapColumnToAttrib(GTLC_COLUMN nCol)
{
	switch (nCol)
	{
	case GTLCC_TITLE:		return IUI_TASKNAME;
//	case GTLCC_ENDDATE:		return IUI_DONEDATE;
	case GTLCC_ENDDATE:		return IUI_DUEDATE;
	case GTLCC_STARTDATE:	return IUI_STARTDATE;
	case GTLCC_ALLOCTO:		return IUI_ALLOCTO;
	case GTLCC_PERCENT:		return IUI_PERCENT;
	}
	
	// all else 
	return IUI_NONE;
}

BOOL CGanttTreeListCtrl::UpdateTask(const ITaskList15* pTasks, HTASKITEM hTask, 
									IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib, 
									BOOL bAndSiblings)
{
	if (hTask == NULL)
		return FALSE;

	ASSERT((nUpdate == IUI_EDIT) || (nUpdate == IUI_NEW));

	// handle task if not NULL (== root)
	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	time64_t tDate = 0;

	// This can be a new task
	if (!m_data.HasTask(dwTaskID))
	{
		ASSERT(nUpdate == IUI_NEW);

		// Parent must exist or be NULL
		DWORD dwParentID = pTasks->GetTaskParentID(hTask);
		HTREEITEM htiParent = NULL;

		if (dwParentID)
		{
			if (!m_data.HasTask(dwParentID))
			{
				ASSERT(0);
				return FALSE;
			}

			htiParent = TCH()->FindItem(dwParentID);

			if (!htiParent)
			{
				ASSERT(0);
				return FALSE;
			}
		}

		CTreeCtrl* pTree = (CTreeCtrl*)CWnd::FromHandle(m_hwndTree);

		BuildTreeItem(pTasks, hTask, *pTree, htiParent, FALSE);
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
	if (attrib.HasKey(IUI_TASKNAME))
		pGI->sTitle = pTasks->GetTaskTitle(hTask);
	
	if (attrib.HasKey(IUI_ALLOCTO))
		pGI->sAllocTo = GetTaskAllocTo(pTasks, hTask);
	
	if (attrib.HasKey(IUI_PERCENT))
		pGI->nPercent = pTasks->GetTaskPercentDone(hTask, TRUE);
		
	if (attrib.HasKey(IUI_STARTDATE))
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
	
	if (attrib.HasKey(IUI_DUEDATE))
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
	
	if (attrib.HasKey(IUI_DONEDATE))
	{
		if (pTasks->GetTaskDoneDate64(hTask, tDate))
			pGI->dtDone = GetDate(tDate, TRUE);
		else
			CDateHelper::ClearDate(pGI->dtDone);
	}
	
	if (attrib.HasKey(IUI_TAGS))
	{
		int nTag = pTasks->GetTaskTagCount(hTask);
		pGI->aTags.RemoveAll();
		
		while (nTag--)
			pGI->aTags.Add(pTasks->GetTaskTag(hTask, nTag));
	}
	
	if (attrib.HasKey(IUI_DEPENDENCY))
	{
		int nDepend = pTasks->GetTaskDependencyCount(hTask);
		pGI->aDepends.RemoveAll();
		
		while (nDepend--)
			pGI->aDepends.Add(pTasks->GetTaskDependency(hTask, nDepend));
	}

	// update date range
	MinMaxDates(*pGI);
	
	// always update colour because it can change for so many reasons
	pGI->color = pTasks->GetTaskTextColor(hTask);

	// likewise 'Good as Done'
	pGI->bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);

	// clear display props
	m_display.RemoveKey(dwTaskID);
	
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

void CGanttTreeListCtrl::BuildTaskMap(const ITaskList15* pTasks, HTASKITEM hTask, 
									  CSet<DWORD>& mapIDs, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;

	mapIDs.AddKey(pTasks->GetTaskID(hTask));

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

void CGanttTreeListCtrl::RemoveDeletedTasks(HTREEITEM hti, const ITaskList15* pTasks, const CSet<DWORD>& mapIDs)
{
	// traverse the tree looking for items that do not 
	// exist in pTasks and delete them
	if (hti && !mapIDs.HasKey(GetTaskID(hti)))
	{
		DeleteTreeItem(hti);
		return;
	}

	// check its children
	HTREEITEM htiChild = TreeView_GetChild(m_hwndTree, hti);
	
	while (htiChild)
	{
		// get next sibling before we (might) delete this one
		HTREEITEM htiNext = TreeView_GetNextItem(m_hwndTree, htiChild, TVGN_NEXT);
		
		RemoveDeletedTasks(htiChild, pTasks, mapIDs);
		htiChild = htiNext;
	}
}

GANTTITEM* CGanttTreeListCtrl::GetGanttItem(DWORD dwTaskID, BOOL bCopyRefID) const
{
	GANTTITEM* pGI = NULL;

	if (dwTaskID && m_data.Lookup(dwTaskID, pGI))
	{
		ASSERT(pGI);

		// handle reference tasks
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

GANTTDISPLAY* CGanttTreeListCtrl::GetGanttDisplay(DWORD dwTaskID)
{
	GANTTDISPLAY* pGD = NULL;

	if (!m_display.Lookup(dwTaskID, pGD))
	{
		pGD = new GANTTDISPLAY;
		m_display[dwTaskID] = pGD;
	}

	ASSERT(pGD);
	return pGD;
}

void CGanttTreeListCtrl::RebuildTree(const ITaskList15* pTasks)
{
	CTreeCtrl* pTree = (CTreeCtrl*)CWnd::FromHandle(m_hwndTree);

	TreeView_DeleteAllItems(m_hwndTree);
	ListView_DeleteAllItems(m_hwndList);

	m_data.RemoveAll();
	m_display.RemoveAll();

	// cache and reset year range which will get 
	// recalculated as we build the tree
	COleDateTime dtPrevEarliest = m_dtEarliest, dtPrevLatest = m_dtLatest;

	CDateHelper::ClearDate(m_dtEarliest);
	CDateHelper::ClearDate(m_dtLatest);

	BuildTreeItem(pTasks, pTasks->GetFirstTask(), *pTree, NULL, TRUE);

	// restore previous date range if no data
	if (m_data.GetCount() == 0)
	{
		m_dtEarliest = dtPrevEarliest;
		m_dtLatest = dtPrevLatest;
	}

	RecalcParentDates();
	ExpandList();
}

void CGanttTreeListCtrl::RecalcDateRange()
{
	if (m_data.GetCount())
	{
		CDateHelper::ClearDate(m_dtEarliest);
		CDateHelper::ClearDate(m_dtLatest);

		POSITION pos = m_data.GetStartPosition();
		GANTTITEM* pGI = NULL;
		DWORD dwTaskID = 0;

		while (pos)
		{
			m_data.GetNextAssoc(pos, dwTaskID, pGI);
			ASSERT(pGI);

			if (pGI)
				MinMaxDates(*pGI);
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

void CGanttTreeListCtrl::BuildTreeItem(const ITaskList15* pTasks, HTASKITEM hTask, 
									   CTreeCtrl& tree, HTREEITEM htiParent, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;

	// map the data
	GANTTITEM* pGI = new GANTTITEM;
	time64_t tDate = 0;
	
	pGI->dwRefID = pTasks->GetTaskReferenceID(hTask);

	if (pGI->dwRefID == 0) // 'true' task
	{
		pGI->sTitle = pTasks->GetTaskTitle(hTask);
		pGI->color = pTasks->GetTaskTextColor(hTask);
		pGI->bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);
		pGI->sAllocTo = GetTaskAllocTo(pTasks, hTask);
		pGI->bParent = pTasks->IsTaskParent(hTask);
		pGI->nPercent = pTasks->GetTaskPercentDone(hTask, TRUE);
		pGI->nPosition = pTasks->GetTaskPosition(hTask);

		if (pTasks->GetTaskStartDate64(hTask, pGI->bParent, tDate))
			pGI->dtStart = GetDate(tDate, FALSE);

		if (pTasks->GetTaskDueDate64(hTask, pGI->bParent, tDate))
			pGI->dtDue = GetDate(tDate, TRUE);

		if (pTasks->GetTaskDoneDate64(hTask, tDate))
			pGI->dtDone = GetDate(tDate, TRUE);

		int nTag = pTasks->GetTaskTagCount(hTask);

		while (nTag--)
			pGI->aTags.Add(pTasks->GetTaskTag(hTask, nTag));

		int nDepend = pTasks->GetTaskDependencyCount(hTask);
		
		while (nDepend--)
			pGI->aDepends.Add(pTasks->GetTaskDependency(hTask, nDepend));
		
		// track earliest and latest dates
		MinMaxDates(*pGI);
	}
	
	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	ASSERT(!m_data.HasTask(dwTaskID));

	m_data[dwTaskID] = pGI;
	
	// add item to tree
	HTREEITEM hti = tree.InsertItem(LPSTR_TEXTCALLBACK, htiParent);
	tree.SetItemData(hti, dwTaskID);
	
	// add first child which will add all the rest
	BuildTreeItem(pTasks, pTasks->GetFirstTask(hTask), tree, hti, TRUE);
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildTreeItem(pTasks, hSibling, tree, htiParent, FALSE);
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

void CGanttTreeListCtrl::MinMaxDates(const GANTTITEM& gi)
{
	CDateHelper::Min(m_dtEarliest, gi.dtStart);
	CDateHelper::Max(m_dtLatest, gi.dtDue);
	CDateHelper::Max(m_dtLatest, gi.dtDone);
}

void CGanttTreeListCtrl::MinMaxDates(const COleDateTime& date)
{
	CDateHelper::Min(m_dtEarliest, date);
	CDateHelper::Max(m_dtLatest, date);
}

int CGanttTreeListCtrl::GetStartYear() const
{
	if (CDateHelper::IsDateSet(m_dtEarliest))
		return m_dtEarliest.GetYear();

	// else
	return COleDateTime::GetCurrentTime().GetYear();
}

int CGanttTreeListCtrl::GetEndYear() const
{
	if (CDateHelper::IsDateSet(m_dtLatest))
	{
		// for now, do not let end year exceed MAX_YEAR
		int nYear = m_dtLatest.GetYear();

		return min(nYear, MAX_YEAR);
	}

	// else
	return COleDateTime::GetCurrentTime().GetYear();
}

int CGanttTreeListCtrl::GetNumMonths() const
{
	return ((GetEndYear() - GetStartYear() + 1) * 12);
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
				m_fonts.Clear();
				Invalidate(FALSE);
				break;
			}

			if (IsSyncing())
			{
				m_display.RemoveAll();
				RedrawList();
			}
		}
	}
}

void CGanttTreeListCtrl::AddListColumn(int nMonth, int nYear)
{
	ASSERT(nMonth >= 1 && nMonth <= 12);

	LVCOLUMN lvc = { LVCF_FMT | LVCF_TEXT | LVCF_WIDTH, 0 };

	lvc.cx = GetColumnWidth(m_nMonthDisplay);
	lvc.fmt = LVCFMT_CENTER;

	CString sTitle = FormatColumnHeaderText(m_nMonthDisplay, nMonth, nYear);

	lvc.pszText = (LPTSTR)(LPCTSTR)sTitle;
	lvc.cchTextMax = sTitle.GetLength() + 1;

	int nCol = m_listHeader.GetItemCount();
	ListView_InsertColumn(m_hwndList, nCol, &lvc);

	// encode month and year into header item data
	SetListColumnDate(nCol, nMonth, nYear);
}

void CGanttTreeListCtrl::SetListColumnDate(int nCol, int nMonth, int nYear)
{
	ASSERT(nMonth >= 1 && nMonth <= 12);

	// encode month and year into header item data
	m_listHeader.SetItemData(nCol, MAKELONG(nMonth, nYear));
}

CString CGanttTreeListCtrl::FormatColumnHeaderText(GTLC_MONTH_DISPLAY nDisplay, int nMonth, int nYear)
{
	if (nMonth == 0)
		return _T("");
	
	//else
	CString sHeader;
	
	switch (nDisplay)
	{
	case GTLC_DISPLAY_YEARS:
		sHeader.Format(_T("%d"), nYear);
		break;
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
		sHeader.Format(_T("Q%d %d"), (1 + ((nMonth-1) / 3)), nYear);
		break;
		
	case GTLC_DISPLAY_QUARTERS_MID:
		sHeader.Format(_T("%s-%s %d"), CDateHelper::GetMonthName(nMonth, TRUE),
			CDateHelper::GetMonthName(nMonth+2, TRUE), nYear);
		break;
		
	case GTLC_DISPLAY_QUARTERS_LONG:
		sHeader.Format(_T("%s-%s %d"), CDateHelper::GetMonthName(nMonth, FALSE),
			CDateHelper::GetMonthName(nMonth+2, FALSE), nYear);
		break;
		
	case GTLC_DISPLAY_MONTHS_SHORT:
		sHeader.Format(_T("%02d/%d"), nMonth, (nYear%100));
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
		sHeader.Format(_T("%s %d (%s)"), CDateHelper::GetMonthName(nMonth, FALSE), nYear, CEnString(IDS_GANTT_DAYS));
		break;
		
	default:
		ASSERT(0);
		break;
	}

	return sHeader;
}

float CGanttTreeListCtrl::GetMonthWidth(int nColWidth) const
{
	switch (m_nMonthDisplay)
	{
	case GTLC_DISPLAY_YEARS:
		return (nColWidth / 12.0f);
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		return (nColWidth / 3.0f);
		
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		// fall thru

	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		// fall thru

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		return (float)nColWidth;
		
	default:
		ASSERT(0);
		break;
	}

	return 0.0f;
}

int CGanttTreeListCtrl::GetRequiredColumnCount() const
{
	return GetRequiredColumnCount(m_nMonthDisplay);
}

int CGanttTreeListCtrl::GetRequiredColumnCount(GTLC_MONTH_DISPLAY nDisplay) const
{
	switch (nDisplay)
	{
	case GTLC_DISPLAY_YEARS:
		return (GetNumMonths() / 12);
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		return (GetNumMonths() / 3);
		break;
		
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		// fall thru

	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		// fall thru

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		return GetNumMonths();
		
	default:
		ASSERT(0);
		break;
	}

	return 0;
}

int CGanttTreeListCtrl::GetColumnWidth() const
{
	return GetColumnWidth(m_nMonthDisplay, m_nMonthWidth);
}

int CGanttTreeListCtrl::GetColumnWidth(GTLC_MONTH_DISPLAY nDisplay) const
{
	return GetColumnWidth(nDisplay, m_nMonthWidth);
}

int CGanttTreeListCtrl::GetColumnWidth(GTLC_MONTH_DISPLAY nDisplay, int nMonthWidth)
{
	int nColWidth = 0;

	switch (nDisplay)
	{
	case GTLC_DISPLAY_YEARS:
		nColWidth = nMonthWidth * 12;
		break;
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		nColWidth = nMonthWidth * 3;
		break;
		
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		// fall thru

	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		// fall thru

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		nColWidth = nMonthWidth;
		break;
		
	default:
		ASSERT(0);
		break;
	}

	return nColWidth;
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
	m_treeHeader.InsertItem(GTLCC_TITLE, 0, _T("Task"), (HDF_LEFT | HDF_STRING));
	m_treeHeader.EnableItemDragging(GTLCC_TITLE, FALSE);

	m_treeHeader.InsertItem(GTLCC_STARTDATE, 0, _T("Start"), (HDF_RIGHT | HDF_STRING));
	m_treeHeader.InsertItem(GTLCC_ENDDATE,	0, _T("Due"), (HDF_RIGHT | HDF_STRING));
	m_treeHeader.InsertItem(GTLCC_ALLOCTO, 0, _T("Alloc To"), (HDF_LEFT | HDF_STRING));
	m_treeHeader.InsertItem(GTLCC_PERCENT, 0, _T("%"), (HDF_CENTER | HDF_STRING));
}

CTreeCtrlHelper* CGanttTreeListCtrl::TCH()
{
	if (m_pTCH == NULL) // first time init
	{
		CTreeCtrl* pTree = (CTreeCtrl*)CWnd::FromHandle(m_hwndTree);
		m_pTCH = new CTreeCtrlHelper(*pTree);
	}

	ASSERT(m_pTCH);
	return m_pTCH;
}

const CTreeCtrlHelper* CGanttTreeListCtrl::TCH() const
{
	if (m_pTCH == NULL) // first time init
	{
		CTreeCtrl* pTree = (CTreeCtrl*)CWnd::FromHandle(m_hwndTree);
		m_pTCH = new CTreeCtrlHelper(*pTree);
	}

	ASSERT(m_pTCH);
	return m_pTCH;
}

BOOL CGanttTreeListCtrl::IsTreeItemLineOdd(HTREEITEM hti) const
{
	int nItem = CTreeListSyncer::FindListItem(m_hwndList, (DWORD)hti);
	return IsListItemLineOdd(nItem);
}

BOOL CGanttTreeListCtrl::IsListItemLineOdd(int nItem) const
{
	return ((nItem % 2) == 1);
}

void CGanttTreeListCtrl::SetFocus()
{
	if (!HasFocus())
		::SetFocus(m_hwndTree);
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
		int nLastCol = m_treeHeader.GetLastVisibleItem();
		
		CRect rItem;
		m_treeHeader.GetItemRect(nLastCol, rItem);

		int nTreeWidth = (rItem.right + 4);
		CTreeListSyncer::Resize(rect, nTreeWidth);

		::SendMessage(m_hwndTree, WM_GTCN_TITLECOLUMNWIDTHCHANGE, m_treeHeader.GetItemWidth(0), (LPARAM)m_hwndTree);
	}
}

void CGanttTreeListCtrl::ExpandAll(BOOL bExpand)
{
	ExpandItem(NULL, bExpand, TRUE);
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

	CHoldRedraw hr(m_hwndList);
	CHoldRedraw hr2(m_hwndTree);

	TCH()->ExpandItem(hti, bExpand, bAndChildren);

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
	
	TreeView_EnsureVisible(m_hwndTree, TreeView_GetChild(m_hwndTree, NULL));

	EnableResync(TRUE, m_hwndTree);
	Resize();
}

BOOL CGanttTreeListCtrl::CanExpandItem(HTREEITEM hti, BOOL bExpand) const
{
	int nFullyExpanded = TCH()->IsItemExpanded(hti, TRUE);
			
	if (nFullyExpanded == -1)	// item has no children
	{
		return FALSE; // can neither expand nor collapse
	}
	else if (bExpand)
	{
		return !nFullyExpanded;
	}
			
	// else
	return TCH()->IsItemExpanded(hti, FALSE);
}

LRESULT CGanttTreeListCtrl::OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD)
{
	HWND hwndTree = pTVCD->nmcd.hdr.hwndFrom;
	HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
	
	switch (pTVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;
								
	case CDDS_ITEMPREPAINT:
		{
			GANTTITEM* pGI = NULL;
			GET_GI_RET(pTVCD->nmcd.lItemlParam, pGI, 0L);
				
			BOOL bAlternate = (HasAltLineColor() && !IsTreeItemLineOdd(hti));
			COLORREF crBack = GetTreeTextBkColor(*pGI, FALSE, bAlternate);

			CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
			GraphicsMisc::FillItemRect(pDC, &pTVCD->nmcd.rc, crBack, hwndTree);
				
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
			::GetClientRect(hwndTree, rClient);
			
			if ((rItem.bottom > 0) && (rItem.top < rClient.bottom))
			{
				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);

				GM_ITEMSTATE nState = GetItemState(hti);
				BOOL bSelected = (nState != GMIS_NONE);

				GANTTITEM* pGI = NULL;
				GET_GI_RET(pTVCD->nmcd.lItemlParam, pGI, 0L);
				
				// draw horz gridline before selection
				DrawItemDivider(pDC, pTVCD->nmcd.rc, FALSE, FALSE, bSelected);
				
				// draw background
				BOOL bAlternate = (HasAltLineColor() && !IsTreeItemLineOdd(hti));
				COLORREF crBack = GetTreeTextBkColor(*pGI, bSelected, bAlternate);

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
					GraphicsMisc::DrawExplorerItemBkgnd(pDC, hwndTree, GetItemState(hti), rItem, dwFlags);
				}
				
				// draw gantt item attributes
				CIntArray aOrder;
				int nNumCol = m_treeHeader.GetItemOrder(aOrder);

				for (int nCol = 0; nCol < nNumCol; nCol++)
					DrawTreeItem(pDC, hti, nCol, *pGI, bSelected, crBack);
			}			
	
			return CDRF_SKIPDEFAULT;
		}
		break;
	}

	return CDRF_DODEFAULT;
}

GM_ITEMSTATE CGanttTreeListCtrl::GetItemState(int nItem) const
{
	if (IsListItemSelected(m_hwndList, nItem))
	{
		if (HasFocus())
			return GMIS_SELECTED;
		else
			return GMIS_SELECTEDNOTFOCUSED;
	}
	else if (ListItemHasState(m_hwndList, nItem, LVIS_DROPHILITED))
	{
		return GMIS_DROPHILITED;
	}

	// else
	return GMIS_NONE;
}

GM_ITEMSTATE CGanttTreeListCtrl::GetItemState(HTREEITEM hti) const
{
	if (IsTreeItemSelected(m_hwndTree, hti))
	{
		if (HasFocus())
			return GMIS_SELECTED;
		else
			return GMIS_SELECTEDNOTFOCUSED;
	}
	else if (TreeItemHasState(m_hwndTree, hti, TVIS_DROPHILITED))
	{
		return GMIS_DROPHILITED;
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
		return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
								
	case CDDS_ITEMPREPAINT:
		{
			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			DWORD dwTaskID = GetTaskID(nItem);
			
			// draw item bkgnd and gridlines full width of list
			BOOL bAlternate = (!IsListItemLineOdd(nItem) && (m_crAltLine != CLR_NONE));
			
			COLORREF crBack = (bAlternate ? m_crAltLine : GetSysColor(COLOR_WINDOW));
			pLVCD->clrTextBk = pLVCD->clrText = crBack;
			
			CRect rItem;
			ListView_GetItemRect(m_hwndList, nItem, rItem, LVIR_BOUNDS);

			CRect rFullWidth(rItem);
			GraphicsMisc::FillItemRect(pDC, rFullWidth, crBack, m_hwndList);
			
			// draw horz gridline before selection
			DrawItemDivider(pDC, rFullWidth, FALSE, FALSE, FALSE);

			// draw background
			GM_ITEMSTATE nState = GetItemState(nItem);
			DWORD dwFlags = (GMIB_THEMECLASSIC | GMIB_CLIPLEFT);

			GraphicsMisc::DrawExplorerItemBkgnd(pDC, m_hwndList, nState, rItem, dwFlags);

			// draw row
			int nNumCol = GetRequiredColumnCount();
			BOOL bSelected = IsListItemSelected(m_hwndList, nItem);

			for (int nCol = 1; nCol <= nNumCol; nCol++)
			{
				if (m_listHeader.GetItemWidth(nCol) > 0)
				{
					if (!DrawListItemColumn(pDC, nItem, nCol, dwTaskID, bSelected))
						break;
				}
			}
			
			PostDrawListItem(pDC, nItem, dwTaskID);
		}
		return CDRF_SKIPDEFAULT;
								
	case CDDS_POSTPAINT:
		// draw dependencies
		{
			CGanttDependArray aDepends;
			
			if (BuildVisibleDependencyList(aDepends))
			{
				CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
				
				CRect rClient;
				::GetClientRect(m_hwndList, rClient);

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
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);
				
				if (nCol == m_nSortBy)
					m_treeHeader.DrawItemSortArrow(pDC, nCol, m_bSortAscending);
			}
			break;
		}
	}
	
	return CDRF_DODEFAULT;
}

void CGanttTreeListCtrl::OnHeaderDividerDblClk(NMHEADER* pHDN)
{
	int nCol = pHDN->iItem;
	ASSERT(nCol != -1);
	
	HWND hwnd = pHDN->hdr.hwndFrom;

	if (hwnd == m_treeHeader)
	{
		CClientDC dc(CWnd::FromHandle(m_hwndTree));
		
		RecalcTreeColumnWidth((GTLC_COLUMN)nCol, &dc);
		Resize();
	}
	else if (hwnd == m_listHeader)
	{
		if (nCol > 0) // first column always zero width
		{
			m_listHeader.SetItemWidth(nCol, GetColumnWidth());
		}
	}
}

void CGanttTreeListCtrl::Sort(GTLC_COLUMN nBy, BOOL bAllowToggle, BOOL bAscending)
{
	// clear pick line first
	ClearDependencyPickLine();
		
	GTLC_COLUMN nOldSort = m_nSortBy;
	m_nSortBy = nBy;

	if (nBy != GTLCC_NONE)
	{
		// if it's the first time or we are changing columns 
		// we always reset the direction
		if ((m_bSortAscending == -1) || (nBy != nOldSort))
		{
			if (bAscending != -1)
			{
				m_bSortAscending = bAscending;
			}
			else
			{
				m_bSortAscending = 1;
			}
		}
		else if (bAllowToggle)
		{
			ASSERT(m_bSortAscending != -1);

			m_bSortAscending = !m_bSortAscending;
		}
	}
	else
	{
		// Always ascending for 'unsorted' to match app
		m_bSortAscending = 1;
	}

	// do the sort
	CHoldRedraw hr(m_hwndTree);
	CTreeListSyncer::Sort(SortProc, (DWORD)this);

	// tell parent
	GetCWnd()->PostMessage(WM_GTLC_NOTIFYSORT, 0, m_nSortBy);

	// update sort arrow
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
						::InvalidateRect(m_hwndTree, NULL, TRUE);
					break;

				case HDN_ITEMCLICK:
					if (hwnd == m_treeHeader)
					{
						HD_NOTIFY *pHDN = (HD_NOTIFY *) pNMHDR;

						if (pHDN->iButton == 0) // left button
						{
							Sort((GTLC_COLUMN)pHDN->iItem, TRUE);
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
								switch (pHDN->iItem)
								{
								case GTLCC_TITLE:
									if (pHDN->pitem->cxy < TREE_TITLE_MIN_WIDTH)
										return TRUE; // prevent change
									break;

								case GTLCC_STARTDATE:
								case GTLCC_ENDDATE:
								case GTLCC_ALLOCTO:
								case GTLCC_PERCENT:
									if (pHDN->pitem->cxy < MIN_COL_WIDTH)
										pHDN->pitem->cxy = MIN_COL_WIDTH;
									break;
								}

								// else
								Resize();
							}
						}
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

				case LVN_GETDISPINFO:
					{
						LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

						if (pDispInfo->item.iSubItem == 0)
						{
							DWORD dwTaskID = GetTaskID((DWORD)pDispInfo->item.lParam);

							const GANTTITEM* pGI = NULL;
							GET_GI_RET(dwTaskID, pGI, 0L);

							static CString sCallback;
							sCallback = pGI->sTitle;
							pDispInfo->item.pszText = (LPTSTR)(LPCTSTR)sCallback;
						}
					}
					break;

				case TVN_SELCHANGED:
					if (HasOption(GTLCF_AUTOSCROLLTOTASK))
						ScrollToSelectedTask();
					break;

				case TVN_GETDISPINFO:
					{
						TV_DISPINFO* pDispInfo = (TV_DISPINFO*)pNMHDR;

						if (pDispInfo->item.mask & TVIF_TEXT)
						{
							DWORD dwTaskID = pDispInfo->item.lParam;

							const GANTTITEM* pGI = NULL;
							GET_GI_RET(dwTaskID, pGI, 0L);

							pDispInfo->item.pszText = (LPTSTR)(LPCTSTR)pGI->sTitle;
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
	}
	
	return CTreeListSyncer::WindowProc(hRealWnd, msg, wp, lp);
}

void CGanttTreeListCtrl::ClearDependencyPickLine(CDC* pDC)
{
	if (IsPickingDependencyToTask() && IsDependencyPickLinePosValid())
	{
		BOOL bNullDC = (pDC == NULL);

		if (bNullDC)
			pDC = CDC::FromHandle(::GetDC(m_hwndList));
			
		CRect rClient;
		::GetClientRect(m_hwndList, rClient);
		
		// calc 'from' point
		DWORD dwFromTaskID = m_pDependEdit->GetFromTask();
		int nFrom = FindListItem(dwFromTaskID);

		GANTTDEPENDENCY depend;
		VERIFY(CalcDependencyEndPos(nFrom, depend, TRUE));

		depend.SetTo(m_ptLastDependPick);
		depend.Draw(pDC, rClient, TRUE);
			
		// clear last drag pos
		ResetDependencyPickLinePos();

		// cleanup
		if (bNullDC)
			::ReleaseDC(m_hwndList, pDC->Detach());
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
		CDC* pDC = CDC::FromHandle(::GetDC(m_hwndList));
		
		// calc 'from ' point
		DWORD dwFromTaskID = m_pDependEdit->GetFromTask();
		int nFrom = FindListItem(dwFromTaskID);
		
		GANTTDEPENDENCY depend;
		VERIFY(CalcDependencyEndPos(nFrom, depend, TRUE));

		// calc new 'To' point to see if anything has actually changed
		GTLC_HITTEST nHit = GTLCHT_NOWHERE;
		DWORD dwToTaskID = ListHitTestTask(ptClient, FALSE, nHit, TRUE);
		CPoint ptTo;
		
		if (dwToTaskID && (nHit != GTLCHT_NOWHERE))
		{
			int nTo = FindListItem(dwToTaskID);
			VERIFY(CalcDependencyEndPos(nTo, depend, FALSE, &ptTo));
		}
		else // use current cursor pos
		{
			ptTo = ::GetMessagePos();
			::ScreenToClient(m_hwndList, &ptTo);
			
			depend.SetTo(m_ptLastDependPick);
		}

		if (ptTo != m_ptLastDependPick)
		{
			CRect rClient;
			::GetClientRect(m_hwndList, rClient);
			
			// clear 'old' line
			if (IsDependencyPickLinePosValid())
			{
				depend.SetTo(m_ptLastDependPick);
				depend.Draw(pDC, rClient, TRUE);
			}		
			
			// draw 'new' line
			depend.SetTo(ptTo);
			depend.Draw(pDC, rClient, TRUE);

			// update pos
			m_ptLastDependPick = ptTo;
		}

		// cleanup
		::ReleaseDC(m_hwndList, pDC->Detach());

		return TRUE;
	}

	// else 
	return FALSE;
}

LRESULT CGanttTreeListCtrl::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

	if (hRealWnd == m_hwndList)
	{
		switch (msg)
		{
		case WM_TIMER:
			// very weird horizontal scroll issue when LVS_EX_FULLROWSELECT
			// is enabled and the mouse is very slowly moved off a just
			// selected task. Research suggests that this relates to the
			// listview's handling of it's internal timer messages, and
			// it scrolls to the start because it thinks it's about to
			// start editing the label, even though LVS_EDITLABELS is not set
			if ((hRealWnd == m_hwndList) && (wp == 0x2B) && (lp == 0) )
			{
				ASSERT(!HasStyle(hRealWnd, LVS_EDITLABELS, FALSE));
				return TRUE; // eat it
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
				::GetClientRect(m_hwndList, rClient);
				
				CDC::FromHandle((HDC)wp)->FillSolidRect(rClient, GetSysColor(COLOR_WINDOW));
			}
			return TRUE;

		case WM_SETCURSOR:
			if (!m_bReadOnly && !IsDependencyEditing())
			{
				CPoint ptCursor(GetMessagePos());
				GTLC_HITTEST nHit = GTLCHT_NOWHERE;

				DWORD dwTaskID = ListHitTestTask(ptCursor, TRUE, nHit, TRUE);
				ASSERT((nHit == GTLCHT_NOWHERE) || (dwTaskID != 0));

				if (nHit != GTLCHT_NOWHERE)
				{
					switch (nHit)
					{
					case GTLCHT_BEGIN:
					case GTLCHT_END:
						SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
						return TRUE;

					case GTLCHT_MIDDLE:
						//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
						break;
					}
				}
			}
			break;

		case WM_LBUTTONDOWN:
			if (!m_bReadOnly)
			{
				if (IsPickingDependencyFromTask())
				{
					CPoint ptScreen(lp);
					::ClientToScreen(hRealWnd, &ptScreen);
					
					DWORD dwFromTaskID = HitTestTask(ptScreen);

					if (dwFromTaskID && m_pDependEdit->SetFromTask(dwFromTaskID))
					{
						// initialise last drag pos
						ResetDependencyPickLinePos();
					}

					return 0; // eat
				}
				else if (IsPickingFromDependency())
				{
					CPoint ptClient(lp);
					DWORD dwCurToTaskID = 0;

					DWORD dwFromTaskID = ListDependsHitTest(ptClient, dwCurToTaskID);
					
					if (dwFromTaskID && dwCurToTaskID)
					{
						if (m_pDependEdit->SetFromDependency(dwFromTaskID, dwCurToTaskID))
						{
							// initialise last drag pos
							ResetDependencyPickLinePos();

							// redraw to hide the dependency being edited
							RedrawList(FALSE);
						}
					}

					return 0; // eat
				}
				else if (IsPickingDependencyToTask())
				{
					return 0; // eat
				}
				else if (StartDragging(lp))
				{
					return 0; // eat
				}
			}

			// don't let the selection to be set to -1
			{
				CPoint ptScreen(lp);
				::ClientToScreen(hRealWnd, &ptScreen);
				
				if (HitTestTask(ptScreen) == 0)
				{
					SetFocus();
					return 0;
				}
			}
			break;

		case WM_LBUTTONUP:
			if (!m_bReadOnly)
			{
				if (IsPickingDependencyFromTask())
				{
					return 0; // eat
				}
				else if (IsPickingFromDependency())
				{
					return 0; // eat
				}
				else if (IsPickingDependencyToTask())
				{
					GTLC_HITTEST nHit = GTLCHT_NOWHERE;
					DWORD dwToTaskID = ListHitTestTask(lp, FALSE, nHit, TRUE);

					if (dwToTaskID && (nHit != GTLCHT_NOWHERE))
					{
						if (m_pDependEdit->SetToTask(dwToTaskID))
						{
							// cancel tracking
// 							TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE | TME_CANCEL, hRealWnd, 0 };
// 							TrackMouseEvent(&tme);
						}
					}

					return 0; // eat
				}
				else if (EndDragging(lp))
				{
					return 0; // eat
				}
			}
			break;

		case WM_MOUSEWHEEL:
		case WM_MOUSELEAVE:
			ClearDependencyPickLine();
			break;

		case WM_MOUSEMOVE:
			if (!m_bReadOnly)
			{
				if (IsDependencyEditing())
				{
					if (DrawDependencyPickLine(lp))
					{
						// track when the cursor leaves the list ctrl
						TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hRealWnd, 0 };
						TrackMouseEvent(&tme);
					}
				}
				else if (UpdateDragging(lp))
				{
					return 0; // eat
				}
			}
			break;

		case WM_CAPTURECHANGED:
			// if someone else grabs the capture we cancel any drag
			if (lp && (HWND(lp) != hRealWnd) && IsDragging())
			{
				CancelDrag(FALSE);
				return 0; // eat
			}
			break;

		case WM_KEYDOWN:
			if (wp == VK_ESCAPE && IsDragging())
			{
				CancelDrag(TRUE);
				return 0; // eat
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
			::SetFocus(m_hwndTree);
			break;
		}
	}
	else if (hRealWnd == m_hwndTree)
	{
		switch (msg)
		{
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
			{
				TVHITTESTINFO tvi = { { LOWORD(lp), HIWORD(lp) }, 0 };
				HTREEITEM hti = TreeView_HitTest(hRealWnd, &tvi);

				if (!(tvi.flags & TVHT_ONITEMBUTTON))
				{
					if (hti && (hti != GetTreeSelItem(hRealWnd)))
						SelectTreeItem(hRealWnd, hti);
				}
			}
			break;

		case WM_HSCROLL:
			{
				LRESULT lr = CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

				::InvalidateRect(m_hwndTree, NULL, FALSE);
				::UpdateWindow(m_hwndTree);

				return lr;
			}
			break;

		case WM_VSCROLL:
			{
				CHoldHScroll hhs(m_hwndTree);
				
				return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
			}
			break;

		case WM_LBUTTONDBLCLK:
			{
				TVHITTESTINFO tvi = { { LOWORD(lp), HIWORD(lp) }, 0 };
				HTREEITEM hti = TreeView_HitTest(hRealWnd, &tvi);
				
				if (tvi.flags & (TVHT_ONITEM | TVHT_ONITEMRIGHT))
				{
					if (TCH()->TreeCtrl().ItemHasChildren(hti))
					{
						TreeView_Expand(hRealWnd, hti, TVE_TOGGLE);
					}
					else
					{
						TreeView_EditLabel(hRealWnd, hti);
					}
					// eat
					return 0L;
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
			WORD wKeys = LOWORD(wp);
			
			if ((wKeys == MK_CONTROL) && (zDelta != 0))
			{
				// cache prev value
				GTLC_MONTH_DISPLAY nPrevDisplay = m_nMonthDisplay;

				// work out where we are going to scroll to after the zoom
				DWORD dwScrollID = 0;
				COleDateTime dtScroll;

				// centre on the mouse if over the list
				if (hRealWnd == m_hwndList)
				{
					CPoint pt(::GetMessagePos());
					::ScreenToClient(m_hwndList, &pt);

					GetDateFromScrollPos((pt.x + ::GetScrollPos(m_hwndList, SB_HORZ)), dtScroll);
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
	}
	
	return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
}

BOOL CGanttTreeListCtrl::GetLabelEditRect(LPRECT pEdit) const
{
	HTREEITEM htiSel = GetSelectedItem();
	
	// scroll into view first
	TreeView_EnsureVisible(m_hwndTree, htiSel);
	
	if (TreeView_GetItemRect(m_hwndTree, htiSel, pEdit, TRUE)) // label only
	{
		// make width of tree column or 200 whichever is larger
		int nWidth = (m_treeHeader.GetItemWidth(GTLCC_TITLE) - pEdit->left);
		nWidth = max(nWidth, MIN_LABEL_EDIT_WIDTH);

		pEdit->right = (pEdit->left + nWidth);

		// convert from tree to 'our' coords
		::ClientToScreen(m_hwndTree, (LPPOINT)pEdit);
		::ClientToScreen(m_hwndTree, ((LPPOINT)pEdit) + 1);

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

void CGanttTreeListCtrl::SetDefaultColor(COLORREF crDefault)
{
	SetColor(m_crDefault, crDefault);
}

void CGanttTreeListCtrl::SetMilestoneTag(const CString& sTag)
{
	if (sTag != m_sMilestoneTag)
	{
		m_sMilestoneTag = sTag;

		if (IsHooked())
			InvalidateAll(FALSE);
	}
}

void CGanttTreeListCtrl::SetParentColoring(GTLC_PARENTCOLORING nOption, COLORREF color)
{
	SetColor(m_crParent, color);

	if (IsHooked() && (m_nParentColoring != nOption))
		InvalidateAll(FALSE);

	m_nParentColoring = nOption;
}

void CGanttTreeListCtrl::SetColor(COLORREF& color, COLORREF crNew)
{
	if (IsHooked() && (crNew != color))
		InvalidateAll(FALSE);

	color = crNew;
}

CString CGanttTreeListCtrl::GetTreeItemColumnText(const GANTTITEM& gi, int nCol)
{
	CString sItem;

	switch (nCol)
	{
		case GTLCC_TITLE:
			sItem = gi.sTitle;
			break;

		case GTLCC_STARTDATE:
			{
				COleDateTime dtStart, dtDummy;
				GetStartDueDates(gi, dtStart, dtDummy);

				sItem = CDateHelper::FormatDate(dtStart);
			}
			break;

		case GTLCC_ENDDATE:
			{
				COleDateTime dtDue, dtDummy;
				GetStartDueDates(gi, dtDummy, dtDue);

				sItem = CDateHelper::FormatDate(dtDue);
			}
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

void CGanttTreeListCtrl::DrawTreeItem(CDC* pDC, HTREEITEM hti, int nCol, const GANTTITEM& gi, BOOL bSelected, COLORREF crBack)
{
	CRect rItem;
	GetTreeItemRect(hti, nCol, rItem);

	if (rItem.Width() == 0)
		return;

	DrawItemDivider(pDC, rItem, FALSE, TRUE, bSelected);

	BOOL bTitleCol = (nCol == GTLCC_TITLE);

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
	CString sItem = GetTreeItemColumnText(gi, nCol);

	if (!sItem.IsEmpty())
	{
		if (bTitleCol)
			rItem.DeflateRect(2, 2, 1, 0);
		else
			rItem.DeflateRect(LV_COLPADDING, 2, LV_COLPADDING, 0);

		// text color and alignment
		BOOL bLighter = FALSE; 
		UINT nFlags = (DT_LEFT | DT_VCENTER | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(m_hwndTree));

		switch (nCol)
		{
		case GTLCC_TITLE:
			nFlags |= DT_END_ELLIPSIS;
			break;
			
		case GTLCC_STARTDATE:
			{
				// draw non-selected calculated dates lighter
				bLighter = (!bSelected && !gi.IsDone(TRUE) && 
							((gi.bParent && HasOption(GTLCF_CALCPARENTDATES)) || 
							(!gi.HasStart() && HasOption(GTLCF_CALCMISSINGSTARTDATES))));
				
				nFlags |= DT_RIGHT;
			}
			break;
			
		case GTLCC_ENDDATE:
			{
				// draw non-selected calculated dates lighter
				bLighter = (!bSelected && !gi.IsDone(TRUE) &&
							((gi.bParent && HasOption(GTLCF_CALCPARENTDATES)) || 
							(!gi.HasDue() && HasOption(GTLCF_CALCMISSINGDUEDATES))));
				
				nFlags |= DT_RIGHT;
			}
			break;
			
		case GTLCC_PERCENT:
			nFlags |= DT_CENTER;
			break;
		}

		COLORREF crText = GetTreeTextColor(gi, bSelected, bLighter);
		COLORREF crOldColor = pDC->SetTextColor(crText);
		HGDIOBJ hFontOld = pDC->SelectObject(GetTreeItemFont(hti, gi, nCol));
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(sItem, rItem, nFlags);
		pDC->SetTextColor(crOldColor);
		pDC->SelectObject(hFontOld);
	}

	// special case: drawing shortcut icon for reference tasks
	if (nCol == 0 && gi.dwOrgRefID)
	{
		GetTreeItemRect(hti, nCol, rItem, TRUE);
		ShellIcons::DrawIcon(pDC, ShellIcons::SI_SHORTCUT, rItem.TopLeft(), false);
	}
}

void CGanttTreeListCtrl::DrawItemDivider(CDC* pDC, const CRect& rItem, BOOL bColumn, BOOL bVert, BOOL bSelected)
{
	if (!HasGridlines())
		return;

	COLORREF color = m_crGridLine;

	if (bColumn && bVert)
		color = GraphicsMisc::Darker(m_crGridLine, 0.5);

	CRect rDiv(rItem);

	if (bVert)
	{
		rDiv.left = (rDiv.right - 1);

		if (bSelected)
			rDiv.DeflateRect(0, 1);
	}
	else
	{
		rDiv.top = (rDiv.bottom - 1);
	}

	COLORREF crOld = pDC->GetBkColor();

	pDC->FillSolidRect(rDiv, color);
	pDC->SetBkColor(crOld);
}

CString CGanttTreeListCtrl::GetLongestVisibleAllocTo(HTREEITEM hti)
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
	if (!hti || TCH()->IsItemExpanded(hti))
	{
		HTREEITEM htiChild = TreeView_GetChild(m_hwndTree, hti);
		
		while (htiChild)
		{
			CString sLongestChild = GetLongestVisibleAllocTo(htiChild);
			
			if (sLongestChild.GetLength() > sLongest.GetLength())
				sLongest = sLongestChild;
			
			htiChild = TreeView_GetNextItem(m_hwndTree, htiChild, TVGN_NEXT);
		}
	}
	
	return sLongest;
}

HFONT CGanttTreeListCtrl::GetTreeItemFont(HTREEITEM hti, const GANTTITEM& gi, int nCol)
{
	BOOL bStrikThru = (HasOption(GTLCF_STRIKETHRUDONETASKS) && gi.IsDone(FALSE));
	BOOL bBold = ((nCol == GTLCC_TITLE) && (TreeView_GetParent(m_hwndTree, hti) == NULL));

	if (bStrikThru || bBold)
		return m_fonts.GetHFont(bBold, FALSE, FALSE, bStrikThru);

	// else
	return NULL;
}

void CGanttTreeListCtrl::GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText) const
{
	rItem.SetRectEmpty();

	if (TreeView_GetItemRect(m_hwndTree, hti, &rItem, TRUE)) // text rect only
	{
		switch (nCol)
		{
		case GTLCC_TITLE:
			{
				int nColWidth = m_treeHeader.GetItemWidth(GTLCC_TITLE);
	
				if (!bText)
					rItem.right = nColWidth;
				else
					rItem.right = min(rItem.right, nColWidth);
			}
			break;

		case GTLCC_STARTDATE:
		case GTLCC_ENDDATE:
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
}

void CGanttTreeListCtrl::PostDrawListItem(CDC* pDC, int nItem, DWORD dwTaskID)
{
	// draw alloc-to name after bar
	if (HasOption(GTLCF_DISPLAYALLOCTOAFTERITEM))
	{
		CRect rItem;
		ListView_GetItemRect(m_hwndList, nItem, rItem, LVIR_BOUNDS);
		
		// get the data for this item
		GANTTITEM* pGI = NULL;
		GET_GI(dwTaskID, pGI);

		if (!pGI->sAllocTo.IsEmpty())
		{
			// get the end pos for this item relative to start of window
			GANTTDISPLAY* pGD = NULL;
			GET_GD(dwTaskID, pGD);

			int nTextPos = pGD->GetBestTextPos();

			if (nTextPos > GCDR_NOTDRAWN)
			{
				int nScrollPos = ::GetScrollPos(m_hwndList, SB_HORZ);

				rItem.left = (nTextPos + 3 - nScrollPos);
				rItem.top += 2;

				COLORREF crFill, crBorder;
				GetGanttBarColors(*pGI, crBorder, crFill);

				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(crBorder);
				pDC->DrawText(pGI->sAllocTo, rItem, (DT_LEFT | GraphicsMisc::GetRTLDrawTextFlags(m_hwndList)));
			}
		}
	}
}

BOOL CGanttTreeListCtrl::DrawListItemColumn(CDC* pDC, int nItem, int nCol, DWORD dwTaskID, BOOL bSelected)
{
	if (nCol == 0)
		return TRUE;

	// see if we can avoid drawing this sub-item at all
	CRect rItem, rClip;
	ListView_GetSubItemRect(m_hwndList, nItem, nCol, LVIR_BOUNDS, &rItem);
	pDC->GetClipBox(rClip);

	if (rItem.right < rClip.left)
		return TRUE;
	
	if (rItem.left > rClip.right)
		return FALSE; // we can stop

	// get the date range for this column
	int nMonth = 0, nYear = 0, i;
	
	if (!GetListColumnDate(nCol, nMonth, nYear))
		return FALSE;

	// get the data and display for this item
	GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, FALSE);

	int nSaveDC = pDC->SaveDC();
	float fMonthWidth = GetMonthWidth(rItem.Width());
	GANTTDISPLAY gdTemp;
	BOOL bToday = FALSE;
	CRect rMonth(rItem);

	switch (m_nMonthDisplay)
	{
	case GTLC_DISPLAY_YEARS:
		for (i = 0; i < 12; i++)
		{
			if (i == 11)
				rMonth.right = rItem.right;
			else
				rMonth.right = (rItem.left + (int)(fMonthWidth * (i + 1)));

			// draw vertical month divider
			DrawItemDivider(pDC, rMonth, (i == 11), TRUE, bSelected);
			
			// if we're passed the end of the item then we can stop drawing
			if (!bToday)
				bToday = DrawToday(pDC, rMonth, nMonth + i, nYear, bSelected);

			DrawGanttBar(pDC, rMonth, nMonth + i, nYear, *pGI, gdTemp);
			DrawGanttDone(pDC, rMonth, nMonth + i, nYear, *pGI, gdTemp);

			// next item
			rMonth.left = rMonth.right; 
		}
		break;
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		for (i = 0; i < 3; i++)
		{
			if (i == 2)
				rMonth.right = rItem.right;
			else
				rMonth.right = (rItem.left + (int)(fMonthWidth * (i + 1)));

			// draw vertical month divider
			DrawItemDivider(pDC, rMonth, (i == 2), TRUE, bSelected);
			
			if (!bToday)
				bToday = DrawToday(pDC, rMonth, nMonth + i, nYear, bSelected);

			DrawGanttBar(pDC, rMonth, nMonth + i, nYear, *pGI, gdTemp);
			DrawGanttDone(pDC, rMonth, nMonth + i, nYear, *pGI, gdTemp);

			// next item
			rMonth.left = rMonth.right;
		}
		break;
		
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		// draw vertical month divider
		DrawItemDivider(pDC, rMonth, (nMonth == 12), TRUE, bSelected);
			
		DrawToday(pDC, rMonth, nMonth, nYear, bSelected);
		DrawGanttBar(pDC, rMonth, nMonth, nYear, *pGI, gdTemp);
		DrawGanttDone(pDC, rMonth, nMonth, nYear, *pGI, gdTemp);
		break;
		
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		{
 			// draw vertical week dividers
			int nNumDays = CDateHelper::GetDaysInMonth(nMonth, nYear);
			double dMonthWidth = rMonth.Width();

			int nFirstDOW = CDateHelper::GetFirstDayOfWeek();
			CRect rDay(rMonth);

			// omit the first one so as not to overwrite the previous month divider
			COleDateTime dtDay = COleDateTime(nYear, nMonth, 1, 0, 0, 0);
			COLORREF crWeekends = GetWeekendColor(bSelected);

			for (int nDay = 1; nDay <= nNumDays; nDay++)
			{
				rDay.left = rMonth.left + (int)((nDay - 1) * dMonthWidth / nNumDays);
				rDay.right = rMonth.left + (int)(nDay * dMonthWidth / nNumDays);

				// fill weekends
				if (crWeekends != CLR_NONE)
				{
					if (CDateHelper::IsWeekend(dtDay))
					{
						CRect rWeekend(rDay);

						// don't overdraw selection or gridlines
						if (bSelected)
						{
							rWeekend.DeflateRect(0, 1);
						}
						else if (HasGridlines())
						{
							rWeekend.bottom--;
						}

						pDC->FillSolidRect(rWeekend, crWeekends);
					}
				}

				// draw divider
				if (dtDay.GetDayOfWeek() == nFirstDOW && nDay > 1)
				{
					rDay.right = rDay.left; // draw at start of day
					DrawItemDivider(pDC, rDay, FALSE, TRUE, bSelected);
				}

				// next day
				dtDay += 1;
			}

			// draw vertical month divider
			DrawItemDivider(pDC, rMonth, TRUE, TRUE, bSelected);

			DrawToday(pDC, rMonth, nMonth, nYear, bSelected);
			DrawGanttBar(pDC, rMonth, nMonth, nYear, *pGI, gdTemp);
			DrawGanttDone(pDC, rMonth, nMonth, nYear, *pGI, gdTemp);
		}
		break;

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		{
			// draw vertical day dividers
			int nNumDays = CDateHelper::GetDaysInMonth(nMonth, nYear);
			double dMonthWidth = rMonth.Width();
			CRect rDay(rMonth);

			// omit the first one so as not to overwrite the previous month divider
			COleDateTime dtDay = COleDateTime(nYear, nMonth, 1, 0, 0, 0);
			COLORREF crWeekends = GetWeekendColor(bSelected);

			for (int nDay = 1; nDay <= nNumDays; nDay++)
			{
				rDay.left = rMonth.left + (int)((nDay - 1) * dMonthWidth / nNumDays);
				rDay.right = rMonth.left + (int)((nDay * dMonthWidth / nNumDays) + 0.5);

				// fill weekends
				if (crWeekends != CLR_NONE)
				{
					if (CDateHelper::IsWeekend(dtDay))
						pDC->FillSolidRect(rDay, crWeekends);
				}

				// draw all but the last divider
				if (nDay < nNumDays)
					DrawItemDivider(pDC, rDay, FALSE, TRUE, bSelected);

				// next day
				dtDay += 1;
			}
				
			// draw vertical month divider
			DrawItemDivider(pDC, rMonth, TRUE, TRUE, bSelected);

			DrawToday(pDC, rMonth, nMonth, nYear, bSelected);
			DrawGanttBar(pDC, rMonth, nMonth, nYear, *pGI, gdTemp);
			DrawGanttDone(pDC, rMonth, nMonth, nYear, *pGI, gdTemp);
		}
		break;
		
	default:
		ASSERT(0);
		break;
	}

	// save off the absolute item end pos
	int nScrollPos = ::GetScrollPos(m_hwndList, SB_HORZ);

	GANTTDISPLAY* pGD = NULL;
	GET_GD_RET(dwTaskID, pGD, FALSE);
	
	if (gdTemp.nStartPos > GCDR_NOTDRAWN)
		pGD->nStartPos = (gdTemp.nStartPos + nScrollPos);
	
	if (gdTemp.nEndPos > GCDR_NOTDRAWN)
		pGD->nEndPos = (gdTemp.nEndPos + nScrollPos);
	
	if (gdTemp.nDonePos > GCDR_NOTDRAWN)
		pGD->nDonePos = (gdTemp.nDonePos + nScrollPos);

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
	float fMonthWidth = GetMonthWidth(rItem.Width());
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(pDC, m_listHeader);
	
	CThemed th;
	BOOL bThemed = (th.AreControlsThemed() && th.Open(GetCWnd(), _T("HEADER")));

	CRect rMonth(rItem), rClip;
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
		
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		{
 			// copy month rect because we are going to modify it
			CRect rWeek(rMonth);
			rWeek.top += (rWeek.Height() / 2);

			// draw month header
			rMonth.bottom = rWeek.top;
			DrawListHeaderRect(pDC, rMonth, m_listHeader.GetItemText(nCol), bThemed ? &th :NULL);

			// draw week elements
			int nNumDays = CDateHelper::GetDaysInMonth(nMonth, nYear);
			double dMonthWidth = rMonth.Width();

			// first week starts at 'First week of first DOW'
			int nFirstDOW = CDateHelper::GetFirstDayOfWeek();
			int nDay = CDateHelper::CalcDayOfMonth(nFirstDOW, 1, nMonth, nYear);

			// calc number of first week
			COleDateTime dtWeek(nYear, nMonth, nDay, 0, 0, 0);
			int nWeek = CDateHelper::GetISOWeekofYear(dtWeek);
			BOOL bDone = FALSE;

			while (!bDone)
			{
				rWeek.left = rMonth.left + (int)((nDay - 1) * dMonthWidth / nNumDays);

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
						dMonthWidth = rMonth.Width();

						rWeek.right += (int)(nDay * dMonthWidth / nNumDays);
					}

					// if this is week 53, check that its not really week 1 of the next year
					if (nWeek == 53)
					{
						ASSERT(nMonth == 1);

						COleDateTime dtWeek(nYear, nMonth, nDay, 0, 0, 0);
						nWeek = CDateHelper::GetISOWeekofYear(dtWeek);
					}

					bDone = TRUE;
				}
				else 
					rWeek.right = rMonth.left + (int)((nDay + 6) * dMonthWidth / nNumDays);

				// check if we can stop
				if (rWeek.left > rClip.right)
					break; 

				// check if we need to draw
				if (rWeek.right >= rClip.left)
					DrawListHeaderRect(pDC, rWeek, Misc::Format(nWeek), bThemed ? &th :NULL);

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
		{
			// copy month rect because we are going to modify it
			CRect rDay(rMonth);
			rDay.top += (rDay.Height() / 2);

			// draw month header
			rMonth.bottom = rDay.top;
			DrawListHeaderRect(pDC, rMonth, m_listHeader.GetItemText(nCol), bThemed ? &th :NULL);

			// draw day elements
			int nNumDays = CDateHelper::GetDaysInMonth(nMonth, nYear);
			double dMonthWidth = rMonth.Width();

			// precalc end of last day to reduce loop calculations
			rDay.right = rDay.left;
			
			for (int nDay = 0; nDay < nNumDays; nDay++)
			{
				rDay.left = rDay.right;
				rDay.right = rMonth.left + (int)((nDay + 1) * dMonthWidth / nNumDays);

				// check if we can stop
				if (rDay.left > rClip.right)
					break; 

				// check if we need to draw
				if (rDay.right >= rClip.left)
					DrawListHeaderRect(pDC, rDay, Misc::Format(nDay+1), bThemed ? &th :NULL);
			}
		}
		break;
		
	default:
		ASSERT(0);
		break;
	}

	pDC->SelectObject(pOldFont); // not sure if this is nec but play safe
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
	return GetStartDueDates(gi, dummy1, dummy2);
}

BOOL CGanttTreeListCtrl::HasDoneDate(const GANTTITEM& gi) const
{
	if (gi.bParent && HasOption(GTLCF_CALCPARENTDATES))
		return FALSE;

	// else
	return CDateHelper::IsDateSet(gi.dtDone);
}

BOOL CGanttTreeListCtrl::GetStartDueDates(const GANTTITEM& gi, COleDateTime& dtStart, COleDateTime& dtDue) const
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

COLORREF CGanttTreeListCtrl::GetWeekendColor(BOOL bSelected) const
{
	return GetColor(m_crWeekend, 0.0, bSelected);
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

HBRUSH CGanttTreeListCtrl::GetGanttBarColors(const GANTTITEM& gi, COLORREF& crBorder, COLORREF& crFill) const
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

	HBRUSH hbrParent = NULL;
	
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
			break;
		}
		
		hbrParent = GetSysColorBrush(COLOR_WINDOWTEXT);
	}
	else
	{
		crBorder = crDefBorder;
		crFill = crDefFill;
	}

	return hbrParent;
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

	rDate = rMonth;

	double dMultiplier = (rMonth.Width() / (double)nDaysInMonth);

	if (dtFrom > dtMonthStart)
		rDate.left += (int)(((dtFrom - dtMonthStart) * dMultiplier));

	if (dtTo < dtMonthEnd)
	{
		if (dtTo == dtFrom)
			rDate.right = rDate.left;
		else
			rDate.right -= (int)(((dtMonthEnd - dtTo) * dMultiplier) + 0.5);
	}

	return TRUE;
}

BOOL CGanttTreeListCtrl::BuildDependency(int nFrom, int nTo, GANTTDEPENDENCY& depend) const
{
	if (CalcDependencyEndPos(nFrom, depend, TRUE) && CalcDependencyEndPos(nTo, depend, FALSE))
	{
// #ifdef _DEBUG
// 		depend.Trace();
// #endif

		return TRUE;
	}

	// else
	return FALSE;
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

int CGanttTreeListCtrl::FindListItem(DWORD dwTaskID) const
{
	HTREEITEM hti = FindTreeItem(m_hwndTree, NULL, dwTaskID);

	return (hti ? GetListItem(hti) : -1);
}

int CGanttTreeListCtrl::FindListItem(DWORD dwTaskID, const CHTIMap& mapItems) const
{
	HTREEITEM hti = NULL;
	
	return ((mapItems.Lookup(dwTaskID, hti) && hti) ? GetListItem(hti) : -1);
}

int CGanttTreeListCtrl::BuildVisibleDependencyList(CGanttDependArray& aDepends) const
{
	aDepends.RemoveAll();

	CRect rClient;
	::GetClientRect(m_hwndList, rClient);

	CHTIMap mapItems;
	TCH()->BuildHTIMap(mapItems, TRUE);
	
	// iterate all list items checking for dependencies
	int nFirstVis = ListView_GetTopIndex(m_hwndList);
	int nLastVis = (nFirstVis + ListView_GetCountPerPage(m_hwndList));
	
	int nNumItems = ListView_GetItemCount(m_hwndList);
	
	for (int nFrom = 0; nFrom < nNumItems; nFrom++)
	{
		DWORD dwFromTaskID = GetTaskID(nFrom);
		
		const GANTTITEM* pGIFrom = GetGanttItem(dwFromTaskID);
		ASSERT(pGIFrom);
		
		if (pGIFrom && pGIFrom->aDepends.GetSize())
		{
			int nDepend = pGIFrom->aDepends.GetSize();
			
			while (nDepend--)
			{
				DWORD dwToTaskID = _ttoi(pGIFrom->aDepends[nDepend]);
				
				if (dwToTaskID == 0)
					continue;

				int nTo = FindListItem(dwToTaskID, mapItems);

				// the 'to' item may not be visible (expanded)
				if (nTo == -1)
					continue;
				
				// and both items may be above or below the visible range
				if ((nFrom < nFirstVis) && (nTo < nFirstVis))
					continue;
				
				if ((nFrom >= nLastVis) && (nTo >= nLastVis))
					continue;
				
				// else
				GANTTDEPENDENCY depend;

				if (BuildDependency(nFrom, nTo, depend))
					aDepends.Add(depend);
			}
		}
	}

	return aDepends.GetSize();
}

BOOL CGanttTreeListCtrl::CalcDependencyEndPos(int nItem, GANTTDEPENDENCY& depend, BOOL bFrom, LPPOINT lpp) const
{
	ASSERT(nItem >= 0);

	if (nItem < 0)
		return FALSE;

	DWORD dwTaskID = GetTaskID(nItem);
	ASSERT(dwTaskID);
	
	GANTTDISPLAY* pGD = NULL;
	
	if (!m_display.Lookup(dwTaskID, pGD))
		return FALSE;

	int nPos = (bFrom ? pGD->nStartPos : pGD->nEndPos);
	int nScrollPos = ::GetScrollPos(m_hwndList, SB_HORZ);

	if (nPos == GCDR_NOTDRAWN)
		return FALSE;
	
	CRect rItem;
	VERIFY(ListView_GetItemRect(m_hwndList, nItem, rItem, LVIR_BOUNDS));
	
	CPoint pt((nPos - nScrollPos), ((rItem.top + rItem.bottom) / 2));

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

void CGanttTreeListCtrl::DrawGanttBar(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, const GANTTITEM& gi, GANTTDISPLAY& gd)
{
	// sanity checks
	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);

	if (nDaysInMonth == 0)
		return;

	COleDateTime dtMonthStart, dtMonthEnd;

	if (!GetMonthDates(nMonth, nYear, dtMonthStart, dtMonthEnd))
		return;

	if (gd.IsEndSet())
		return;

	if (IsMilestone(gi))
	{
		DrawGanttMilestone(pDC, rMonth, nMonth, nYear, gi, gd);
		return;
	}

	// else normal bar
	COleDateTime dtStart, dtDue;
	
	if (!GetStartDueDates(gi, dtStart, dtDue))
	{
		// indicate no further drawing required
		gd.SetHasNoDates();
		return;
	}

	// check for visibility
	CRect rBar(rMonth);

	if (!CalcDateRect(rMonth, nDaysInMonth, dtMonthStart, dtMonthEnd, dtStart, dtDue, rBar))
		return;

	COLORREF crBorder, crFill;
	HBRUSH hbrParent = GetGanttBarColors(gi, crBorder, crFill);
	
	// adjust bar height
	if (gi.bParent && HasOption(GTLCF_CALCPARENTDATES))
		rBar.DeflateRect(0, 4, 0, 6);
	else
		rBar.DeflateRect(0, 2, 0, 3);
	
	// Determine what borders to draw
	DWORD dwBorders = (GMDR_TOP | GMDR_BOTTOM);

	// draw the ends of the border by deflating in width
	// if the date does not extend beyond the month
	if (dtStart >= dtMonthStart)
	{
		gd.nStartPos = rBar.left;
		dwBorders |= GMDR_LEFT;
	}

	if (dtDue <= dtMonthEnd)
	{
		gd.nEndPos = rBar.right;
		dwBorders |= GMDR_RIGHT;
	}

	// if parent and no-fill then clear fill colour
	if (gi.bParent && (m_nParentColoring == GTLPC_NOCOLOR))
	{
		crFill = CLR_NONE;
	}
	else if (!gi.IsDone(TRUE) && HasOption(GTLCF_DISPLAYPROGRESSINBAR))
	{
		CRect rProgress(rBar);

		if (gi.nPercent <= 0)
		{
			// Whole bar is 'non-progress' color
			crFill = GraphicsMisc::Lighter(crFill, 0.8);
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
				crFill = GraphicsMisc::Lighter(crFill, 0.8);
			}
			else if (dtPercentDone >= dtMonthEnd)
			{
				// Whole bar is crFill
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
					GraphicsMisc::DrawRect(pDC, rProgress, crFill, CLR_NONE, 0, 0);

					// Draw the uncompleted part
					rProgress.left = rProgress.right;
					rProgress.right = rBar.right;

					crFill = GraphicsMisc::Lighter(crFill, 0.8);
					GraphicsMisc::DrawRect(pDC, rProgress, crFill, CLR_NONE, 0, 0);

					crFill = CLR_NONE;
				}
				else // no width -> no progress
				{
					crFill = GraphicsMisc::Lighter(crFill, 0.8);
				}
			}
		}
	}

	// draw rect with borders
	GraphicsMisc::DrawRect(pDC, rBar, crFill, crBorder, 0, dwBorders);
	
	// for parent items draw downward facing pointers at the ends
	DrawGanttParentEnds(pDC, gi, rBar, dtMonthStart, dtMonthEnd, hbrParent);
}

void CGanttTreeListCtrl::DrawGanttParentEnds(CDC* pDC, const GANTTITEM& gi, const CRect& rBar, 
											 const COleDateTime& dtMonthStart, const COleDateTime& dtMonthEnd,
											 HBRUSH hbrParent)
{
	if (!gi.bParent || !HasOption(GTLCF_CALCPARENTDATES))
		return;

	ASSERT(hbrParent);
	
	pDC->SelectObject(hbrParent);
	pDC->SelectStockObject(NULL_PEN);

	COleDateTime dtStart, dtDue;
	GetStartDueDates(gi, dtStart, dtDue);
	
	if (dtStart >= dtMonthStart)
	{
		POINT pt[3] = 
		{ 
			{ rBar.left, rBar.bottom }, 
			{ rBar.left, rBar.bottom + 6 }, 
			{ rBar.left + 6, rBar.bottom } 
		};
		
		pDC->Polygon(pt, 3);
	}
	
	if (dtDue <= dtMonthEnd)
	{
		POINT pt[3] = 
		{ 
			{ rBar.right, rBar.bottom }, 
			{ rBar.right, rBar.bottom + 6 }, 
			{ rBar.right - 6, rBar.bottom } 
		};
		
		pDC->Polygon(pt, 3);
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

void CGanttTreeListCtrl::DrawGanttDone(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, const GANTTITEM& gi, GANTTDISPLAY& gd)
{
	if (gd.nDonePos != GCDR_NOTDRAWN)
		return;

	if (!HasDoneDate(gi) || IsMilestone(gi))
	{
		gd.nDonePos = GCDR_NOTDONE;
		return;
	}

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
	rDone.DeflateRect(0, 6, 0, 6);
	rDone.right = min(rMonth.right, rDone.left + rDone.Height());
	rDone.left = rDone.right - rDone.Height();

	pDC->FillSolidRect(rDone, crBorder);

	gd.nDonePos = rDone.right;
}

void CGanttTreeListCtrl::DrawGanttMilestone(CDC* pDC, const CRect& rMonth, int nMonth, int nYear, const GANTTITEM& gi, GANTTDISPLAY& gd)
{
	// sanity check
	ASSERT(IsMilestone(gi));

	int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);

	if (nDaysInMonth == 0)
		return;

	COleDateTime dtMonthStart, dtMonthEnd;

	if (!GetMonthDates(nMonth, nYear, dtMonthStart, dtMonthEnd))
		return;

	COleDateTime dtDue = ((gi.bParent && HasOption(GTLCF_CALCPARENTDATES)) ? gi.dtMaxDue : gi.dtDue);
	ASSERT(CDateHelper::IsDateSet(dtDue));

	if (dtDue < dtMonthStart || dtDue > dtMonthEnd)
		return;

	CRect rMilestone(rMonth);

	if (!CalcDateRect(rMonth, nDaysInMonth, dtMonthStart, dtMonthEnd, dtDue, dtDue, rMilestone))
		return;
	
	// resize to a square
	rMilestone.DeflateRect(0, 3, 0, 4);
	rMilestone.right = min(rMonth.right, rMilestone.left + rMilestone.Height());
	rMilestone.left = rMilestone.right - rMilestone.Height();

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
	
	pDC->SelectStockObject(BLACK_BRUSH);
	pDC->SelectStockObject(NULL_PEN);
	pDC->Polygon(pt, 5);

	gd.nStartPos = rMilestone.left;
	gd.nEndPos = rMilestone.right;
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

	// draw border
	pDC->FillSolidRect(rToday, GetColor(m_crToday, 0.0, bSelected));

	// fill interior if enough space
	if (rToday.Width() > 2)
	{
		rToday.DeflateRect(1, 0);
		pDC->FillSolidRect(rToday, GetColor(m_crToday, 0.5, bSelected));
	}

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

int CGanttTreeListCtrl::GetListItem(HTREEITEM hti) const
{
	if (!hti)
		return -1;

	return CTreeListSyncer::FindListItem(m_hwndList, (DWORD)hti);
}

void CGanttTreeListCtrl::ExpandList()
{
	int nNextIndex = 0;
	ExpandList(NULL, nNextIndex);
}

void CGanttTreeListCtrl::ExpandList(HTREEITEM hti, int& nNextIndex)
{
	CTreeListSyncer::ExpandList(m_hwndList, m_hwndTree, hti, nNextIndex);
}

void CGanttTreeListCtrl::CollapseList(HTREEITEM hti)
{
	CTreeListSyncer::CollapseList(m_hwndList, m_hwndTree, hti);
}

void CGanttTreeListCtrl::DeleteTreeItem(HTREEITEM hti)
{
	ASSERT(hti);

	DWORD dwTaskID = GetTaskID(hti);

	TreeView_DeleteItem(m_hwndTree, hti);

	VERIFY(m_data.RemoveKey(dwTaskID));
}

BOOL CGanttTreeListCtrl::ZoomIn(BOOL bIn)
{
	return SetMonthDisplay((GTLC_MONTH_DISPLAY)(m_nMonthDisplay + (bIn ? 1 : -1)));
}

BOOL CGanttTreeListCtrl::SetMonthDisplay(GTLC_MONTH_DISPLAY nNewDisplay)
{
	if ((nNewDisplay < GTLC_DISPLAY_FIRST) || (nNewDisplay > GTLC_DISPLAY_LAST))
		return FALSE;

	// calculate the min month width for this display
	int nMinMonthWidth = m_aMinMonthWidths[nNewDisplay];
	
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

	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
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
	return CanSetMonthDisplay(nDisplay, m_aMinMonthWidths[nDisplay]);
}

BOOL CGanttTreeListCtrl::CanSetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay, int nMonthWidth) const
{
	ASSERT((nDisplay >= GTLC_DISPLAY_FIRST) && (nDisplay <= GTLC_DISPLAY_LAST));
	ASSERT(nMonthWidth != -1);
	
	int nNumReqColumns = (GetRequiredColumnCount(nDisplay) + 1);
	int nColWidth = GetColumnWidth(nDisplay, nMonthWidth);
	int nTotalReqdWidth = (nNumReqColumns * nColWidth);
	
	return (nTotalReqdWidth <= MAX_HEADER_WIDTH);
}

void CGanttTreeListCtrl::ValidateMonthDisplay(GTLC_MONTH_DISPLAY& nDisplay) const
{
	int nWidth = m_aMinMonthWidths[nDisplay];
	ValidateMonthDisplay(nDisplay, nWidth);
}

void CGanttTreeListCtrl::ValidateMonthDisplay(GTLC_MONTH_DISPLAY& nDisplay, int& nMonthWidth) const
{
	if ((nDisplay < GTLC_DISPLAY_FIRST) || (nDisplay > GTLC_DISPLAY_LAST))
	{
		ASSERT(0);
		return;
	}

	if (!CanSetMonthDisplay(nDisplay, nMonthWidth))
	{
		// for all subsequent displays, use min width
		do 
		{
			nDisplay = (GTLC_MONTH_DISPLAY)(nDisplay - 1);
			nMonthWidth = m_aMinMonthWidths[nDisplay];
		} 
		while (!CanSetMonthDisplay(nDisplay, nMonthWidth));
	}
}

void CGanttTreeListCtrl::ValidateMonthDisplay()
{
	GTLC_MONTH_DISPLAY nPrevDisplay = m_nMonthDisplay, nDisplay = nPrevDisplay;
	int nMonthWidth = m_nMonthWidth;

	ValidateMonthDisplay(nDisplay, nMonthWidth);

	if (m_nMonthDisplay != nDisplay)
	{
		VERIFY(SetMonthDisplay(nDisplay));
		GetCWnd()->SendMessage(WM_GTLC_NOTIFYZOOM, nPrevDisplay, m_nMonthDisplay);
	}
}

BOOL CGanttTreeListCtrl::BeginDependencyEdit(CGanttDependencyEditor* pDependEdit)
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
	InvalidateRect(m_hwndList, NULL, TRUE);
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
	ASSERT (nNewDisplay >= GTLC_DISPLAY_FIRST && nNewDisplay <= GTLC_DISPLAY_LAST);

	// validate new display
	if (nNewDisplay < GTLC_DISPLAY_FIRST || nNewDisplay > GTLC_DISPLAY_LAST)
		return FALSE;

	ValidateMonthDisplay(nNewDisplay, nNewMonthWidth);

	if ((nNewDisplay == m_nMonthDisplay) && (nNewMonthWidth == m_nMonthWidth))
		return TRUE;

	// validate month width
	if (nNewMonthWidth < 10)
		return FALSE;

	// cache the current scroll-pos so we can restore it
	int nScrollPos = ::GetScrollPos(m_hwndList, SB_HORZ);
	COleDateTime dtPos;

	if (nScrollPos)
		VERIFY(GetDateFromScrollPos(nScrollPos, dtPos));

	// clear display cache since it's all about to change
	m_display.RemoveAll();

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
	if (nScrollPos)
	{
		nScrollPos = GetScrollPosFromDate(dtPos);
		ListView_Scroll(m_hwndList, nScrollPos - ::GetScrollPos(m_hwndList, SB_HORZ), 0);
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
	int nNumReqColumns = GetRequiredColumnCount(), i;

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
	for (; i <= GetNumMonths(); i++)
	{
		m_listHeader.EnableItemTracking(i, FALSE);
		m_listHeader.SetItemWidth(i, 0);
	}
}

void CGanttTreeListCtrl::ResizeColumnsToFit()
{
	// tree columns
	CClientDC dc(CWnd::FromHandle(m_hwndTree));
	int nCol = m_treeHeader.GetItemCount();

	while (nCol--)
		RecalcTreeColumnWidth((GTLC_COLUMN)nCol, &dc);
	
	// list columns (except first dummy column)
	nCol = GetRequiredColumnCount();
	
	while (--nCol > 0)
		m_listHeader.SetItemWidth(nCol, GetColumnWidth());

	Resize();
}

BOOL CGanttTreeListCtrl::RecalcTreeColumns(BOOL bResize)
{
	// Only need recalc non-fixed column widths
	BOOL bTitle = !m_treeHeader.IsItemTracked(GTLCC_TITLE);
	BOOL bAllocTo = !m_treeHeader.IsItemTracked(GTLCC_ALLOCTO);

	if (bTitle || bAllocTo)
	{
		CClientDC dc(CWnd::FromHandle(m_hwndTree));

		if (bTitle)
			RecalcTreeColumnWidth(GTLCC_TITLE, &dc);
			
		if (bAllocTo)
			RecalcTreeColumnWidth(GTLCC_ALLOCTO, &dc);
		
		if (bResize)
			Resize();

		return TRUE;
	}

	return FALSE;
}

void CGanttTreeListCtrl::RecalcTreeColumnWidth(int nCol, CDC* pDC)
{
	ASSERT(pDC);
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(pDC, m_hwndTree);

	int nColWidth = 0;

	switch (nCol)
	{
	case GTLCC_TITLE:
		nColWidth = CalcWidestItemTitle(NULL, pDC);
		nColWidth = max(nColWidth, TREE_TITLE_MIN_WIDTH);
		break;
		
	case GTLCC_ALLOCTO:
		nColWidth = GraphicsMisc::GetAverageMaxStringWidth(GetLongestVisibleAllocTo(NULL), pDC);
		break;
		
	// rest of attributes are fixed width
	case GTLCC_STARTDATE:
	case GTLCC_ENDDATE: 
		{
			COleDateTime date(2015, 12, 31, 23, 59, 59);
			nColWidth = GraphicsMisc::GetAverageMaxStringWidth(CDateHelper::FormatDate(date), pDC);
		}
		break;
		
	case GTLCC_PERCENT: 
		nColWidth = GraphicsMisc::GetAverageMaxStringWidth(_T("100%"), pDC);
		break;
	}

	if (nColWidth < MIN_COL_WIDTH)
		nColWidth = MIN_COL_WIDTH;
	else
		nColWidth += (2 * LV_COLPADDING);
	
	// take max of this and column title
	int nTitleWidth = (m_treeHeader.GetItemTextWidth(nCol, pDC) + (2 * HD_COLPADDING));
	ASSERT(nTitleWidth);
	
	m_treeHeader.SetItemWidth(nCol, max(nTitleWidth, nColWidth));

	pDC->SelectObject(pOldFont);
}

int CGanttTreeListCtrl::CalcWidestItemTitle(HTREEITEM htiParent, CDC* pDC) const
{
	// if this task has no children then return itself
	HTREEITEM htiChild = TreeView_GetChild(m_hwndTree, htiParent);
	
	if (htiChild == NULL)
		return 0;
	
	// or we only want expanded items
	if (htiParent && !TCH()->IsItemExpanded(htiParent, FALSE))
		return 0;
	
	// Prepare font
	HFONT hFont = m_fonts.GetHFont((htiParent == NULL) ? GMFS_BOLD : 0);
	HGDIOBJ hOldFont = pDC->SelectObject(hFont);
	
	// else try children
	int nWidest = 0;
	
	while (htiChild)
	{
		CRect rChild;
		
		if (TreeView_GetItemRect(m_hwndTree, htiChild, &rChild, TRUE)) // text rect only
		{
			DWORD dwTaskID = GetTaskID(htiChild);
			const GANTTITEM* pGI = NULL;
			
			GET_GI_RET(dwTaskID, pGI, 0);
			
			int nTextWidth = GraphicsMisc::GetTextWidth(pDC, pGI->sTitle);
			int nWidth = max(nTextWidth, (rChild.left + nTextWidth));
			
			int nWidestChild = CalcWidestItemTitle(htiChild, pDC); // RECURSIVE CALL
			
			nWidest = max(max(nWidest, nWidth), nWidestChild);
		}
		
		htiChild = TreeView_GetNextSibling(m_hwndTree, htiChild);
	}
	
	pDC->SelectObject(hOldFont);
	
	return nWidest;
}

void CGanttTreeListCtrl::UpdateColumnsWidthAndText(int nWidth)
{
	// first column is always zero width and not trackable
	m_listHeader.SetItemWidth(0, 0);
	m_listHeader.EnableItemTracking(0, FALSE);

	if (nWidth == -1)
		nWidth = GetColumnWidth();

	int nNumReqColumns = (GetRequiredColumnCount() + 1), i;
	BOOL bUsePrevWidth = (m_aPrevColWidths.GetSize() == nNumReqColumns);

	int nTotalReqdWidth = 0;
	
	for (i = 1; i < nNumReqColumns; i++)
	{
		int nYear = 0, nMonth = 0;

		switch (m_nMonthDisplay)
		{
		case GTLC_DISPLAY_YEARS:
			nMonth = 1;
			nYear = GetStartYear() + (i - 1);
			break;
			
		case GTLC_DISPLAY_QUARTERS_SHORT:
		case GTLC_DISPLAY_QUARTERS_MID:
		case GTLC_DISPLAY_QUARTERS_LONG:
			// each column represents 3 months
			// the first month of each quarter has the 
			// indices: 1, 4, 7, 10
			nMonth = (((i - 1) % 4) * 3) + 1;
			nYear = (GetStartYear() + ((i - 1) / 4));
			break;
			
		case GTLC_DISPLAY_MONTHS_SHORT:
		case GTLC_DISPLAY_MONTHS_MID:
		case GTLC_DISPLAY_MONTHS_LONG:
			// fall thru

		case GTLC_DISPLAY_WEEKS_SHORT:
		case GTLC_DISPLAY_WEEKS_MID:
		case GTLC_DISPLAY_WEEKS_LONG:
			// fall thru

		case GTLC_DISPLAY_DAYS_SHORT:
		case GTLC_DISPLAY_DAYS_MID:
		case GTLC_DISPLAY_DAYS_LONG:
			nMonth = ((i - 1) % 12) + 1;
			nYear = GetStartYear() + ((i - 1) / 12);
			break;
			
		default:
			ASSERT(0);
			break;
		}

		if (nMonth && nYear)
		{
			CString sTitle = FormatColumnHeaderText(m_nMonthDisplay, nMonth, nYear);
			DWORD dwData = MAKELONG(nMonth, nYear);

			if (bUsePrevWidth)
				nWidth = m_aPrevColWidths[i];

			m_listHeader.SetItem(i, nWidth, sTitle, dwData);
			m_listHeader.EnableItemTracking(i, TRUE);

			nTotalReqdWidth += nWidth;
		}
	}

	TRACE(_T("CGanttTreeListCtrl(Total Column Widths = %d)\n"), nTotalReqdWidth);

	// for the rest, clear the text and item data and prevent tracking
	for (; i <= GetNumMonths(); i++)
	{
		m_listHeader.EnableItemTracking(i, FALSE);
		m_listHeader.SetItem(i, 0, _T(""), 0);
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
	ListView_InsertColumn(m_hwndList, 0, &lvc);
	
	// add other columns
	for (int i = 0; i < GetNumMonths(); i++)
	{
		lvc.cx = 0;
		lvc.fmt = LVCFMT_CENTER | HDF_STRING;
		lvc.pszText = _T("");
		lvc.cchTextMax = 50;

		int nCol = m_listHeader.GetItemCount();
		ListView_InsertColumn(m_hwndList, nCol, &lvc);
	}

	UpdateColumnsWidthAndText();
}

void CGanttTreeListCtrl::UpdateListColumns(int nWidth)
{
	// cache the scrolled position
	int nScrollPos = ::GetScrollPos(m_hwndList, SB_HORZ);

	COleDateTime dtPos;
	BOOL bRestorePos = GetDateFromScrollPos(nScrollPos, dtPos);

	if (nWidth == -1)
		nWidth = GetColumnWidth();

	int nReqMonths = (GetNumMonths() + 1);
	int nDiffMonths = (nReqMonths - m_listHeader.GetItemCount());

	if (nDiffMonths > 0)
	{
		// add other columns
		LVCOLUMN lvc = { LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, 0 };

		for (int i = 0; i < nDiffMonths; i++)
		{
			lvc.cx = 0;
			lvc.fmt = LVCFMT_CENTER | HDF_STRING;
			lvc.pszText = _T("");
			lvc.cchTextMax = 50;

			int nCol = m_listHeader.GetItemCount();
			ListView_InsertColumn(m_hwndList, nCol, &lvc);
		}
	}
	else if (nDiffMonths < 0)
	{
		int i = -nDiffMonths;
		
		while (i--)
		{
			int nCol = (m_listHeader.GetItemCount() - 1);
			ListView_DeleteColumn(m_hwndList, nCol);
		}
		
	}

	if (nDiffMonths != 0)
		PostResize();

	UpdateColumnsWidthAndText(nWidth);

	// restore scroll-pos
	if (bRestorePos)
	{
		nScrollPos = GetScrollPosFromDate(dtPos);
		ListView_Scroll(m_hwndList, nScrollPos - ::GetScrollPos(m_hwndList, SB_HORZ), 0);
	}
	else
	{
		::SetScrollPos(m_hwndList, SB_HORZ, 0, TRUE);
	}

	// clear display cache since it's probably going to change
	m_display.RemoveAll();
}

void CGanttTreeListCtrl::CalculateMinMonthWidths()
{
	m_aMinMonthWidths.SetSize(GTLC_DISPLAY_COUNT);

	CClientDC dcClient(&m_treeHeader);
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(&dcClient, m_hwndList);

	for (int nCol = 0; nCol < GTLC_DISPLAY_COUNT; nCol++)
	{
		GTLC_MONTH_DISPLAY nDisplay = (GTLC_MONTH_DISPLAY)nCol;
		int nMinMonthWidth = 0;

		switch (nDisplay)
		{
			case GTLC_DISPLAY_YEARS:
				{
					CString sText = FormatColumnHeaderText(nDisplay, 1, 2013);

					int nMinTextWidth = GraphicsMisc::GetTextWidth(&dcClient, sText);
					nMinMonthWidth = (nMinTextWidth + COLUMN_PADDING) / 12;
				}
				break;

			case GTLC_DISPLAY_QUARTERS_SHORT:
				{
					CString sText = FormatColumnHeaderText(nDisplay, 1, 2013);

					int nMinTextWidth = GraphicsMisc::GetTextWidth(&dcClient, sText);
					nMinMonthWidth = (nMinTextWidth + COLUMN_PADDING) / 3;
				}
				break;

			case GTLC_DISPLAY_QUARTERS_MID:
			case GTLC_DISPLAY_QUARTERS_LONG:
				{
					int nMinTextWidth = 0;

					for (int nMonth = 1; nMonth <= 12; nMonth += 3)
					{
						CString sText = FormatColumnHeaderText(nDisplay, nMonth, 2013);

						int nWidth = GraphicsMisc::GetTextWidth(&dcClient, sText);
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
						CString sText = FormatColumnHeaderText(nDisplay, nMonth, 2013);

						int nWidth = GraphicsMisc::GetTextWidth(&dcClient, sText);
						nMinTextWidth = max(nWidth, nMinTextWidth);
					}

					nMinMonthWidth = (nMinTextWidth + COLUMN_PADDING);
				}
				break;

			case GTLC_DISPLAY_WEEKS_SHORT:
			case GTLC_DISPLAY_WEEKS_MID:
			case GTLC_DISPLAY_WEEKS_LONG:
				// fall thru

			case GTLC_DISPLAY_DAYS_SHORT:
			case GTLC_DISPLAY_DAYS_MID:
			case GTLC_DISPLAY_DAYS_LONG:
				// just increase the length of the preceding display
				nMinMonthWidth = (int)(m_aMinMonthWidths[nCol-1] * DAY_WEEK_MULTIPLIER);
				break;

			default:
				ASSERT(0);
				break;
		}

		if (nMinMonthWidth > 0)
		{
			nMinMonthWidth++; // for rounding
			m_aMinMonthWidths[nCol] = nMinMonthWidth; 
		}
	}

	dcClient.SelectObject(pOldFont);
}

GTLC_MONTH_DISPLAY CGanttTreeListCtrl::GetColumnDisplay(int nMonthWidth)
{
	int nNumDisplay = GTLC_DISPLAY_COUNT;
	int nMinWidth = 0;

	for (int nDisp = GTLC_DISPLAY_FIRST; nDisp < GTLC_DISPLAY_LAST; nDisp++)
	{
		int nFrom = m_aMinMonthWidths[nDisp];
		int nTo = m_aMinMonthWidths[nDisp+1];

		if (nMonthWidth >= nFrom && nMonthWidth < nTo)
			return (GTLC_MONTH_DISPLAY)nDisp;
	}

	return GTLC_DISPLAY_LAST;
}

int CALLBACK CGanttTreeListCtrl::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const CGanttTreeListCtrl* pThis = (CGanttTreeListCtrl*)lParamSort;

	const GANTTITEM* pGI1 = pThis->GetGanttItem(lParam1);
	const GANTTITEM* pGI2 = pThis->GetGanttItem(lParam2);

	int nCompare = 0;

	if (pGI1 && pGI2)
	{
		switch (pThis->m_nSortBy)
		{
		case GTLCC_TITLE:
			nCompare = Compare(pGI1->sTitle, pGI2->sTitle);
			break;
			
		case GTLCC_STARTDATE:
			nCompare = (int)(pGI1->dtStart - pGI2->dtStart);
			break;
			
		case GTLCC_ENDDATE:
			nCompare = (int)(pGI1->dtDue - pGI2->dtDue);
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
		}
	}

	return (pThis->m_bSortAscending ? nCompare : -nCompare);
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
	
	if (GetStartDueDates(*pGI, dtStart, dtDue))
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
	int nListStart = ::GetScrollPos(m_hwndList, SB_HORZ);

	CRect rList;
	::GetClientRect(m_hwndList, rList);

	if ((nStartPos >= nListStart) && (nEndPos <= (nListStart + rList.Width())))
		return;

	// deduct current scroll pos for relative move
	nStartPos -= nListStart;

	// and arbitrary offset
	nStartPos -= 50;

	ListView_Scroll(m_hwndList, nStartPos, 0);
	Invalidate(FALSE);
}

BOOL CGanttTreeListCtrl::GetListColumnRect(int nCol, CRect& rColumn, BOOL bScrolled) const
{
	if (ListView_GetSubItemRect(m_hwndList, 0, nCol, LVIR_BOUNDS, (LPRECT)rColumn))
	{
		if (!bScrolled)
		{
			int nScroll = ::GetScrollPos(m_hwndList, SB_HORZ);
			rColumn.OffsetRect(nScroll, 0);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CGanttTreeListCtrl::GetDateFromScrollPos(int nScrollPos, COleDateTime& date) const
{
	// find the column containing this scroll pos
	int nCol = FindColumn(nScrollPos);

	if (nCol != -1)
	{
		CRect rColumn;
		VERIFY(GetListColumnRect(nCol, rColumn, FALSE));
		ASSERT(nScrollPos >= rColumn.left && nScrollPos < rColumn.right);

		int nYear, nMonth;
		VERIFY(GetListColumnDate(nCol, nMonth, nYear));

		// further clip 'rColumn' if in 'Year' or 'Quarter' mode
		switch (m_nMonthDisplay)
		{
		case GTLC_DISPLAY_YEARS:
		case GTLC_DISPLAY_QUARTERS_SHORT:
		case GTLC_DISPLAY_QUARTERS_MID:
		case GTLC_DISPLAY_QUARTERS_LONG:
			{
				float fMonthWidth = GetMonthWidth(rColumn.Width());

				// calc month as offset to start of column
				int nPxOffset = (nScrollPos - rColumn.left);
				int nMonthOffset = (int)(nPxOffset / fMonthWidth);

				// clip rect to this month
				rColumn.left += nPxOffset;
				rColumn.right = (rColumn.left + (int)fMonthWidth);

				nMonth += nMonthOffset;
			}
			break;
		}

		int nDaysInMonth = CDateHelper::GetDaysInMonth(nMonth, nYear);
		int nNumMins = MulDiv((nScrollPos - rColumn.left), (60 * 24 * nDaysInMonth), rColumn.Width());

		int nDay = (1 + (nNumMins / MINS_IN_DAY));
		int nHour = ((nNumMins % MINS_IN_DAY) / MINS_IN_HOUR);
		int nMin = (nNumMins % MINS_IN_HOUR);

		ASSERT(nDay >= 1 && nDay <= nDaysInMonth);
		ASSERT(nHour >= 0 && nHour < 24);

		date.SetDateTime(nYear, nMonth, nDay, nHour, nMin, 0);
		return TRUE;
	}

	// else
	return FALSE;
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

			double dDayInCol = ((nDay - 1) + CDateHelper::GetTimeOnly(date));
			int nDaysInCol = 0;

			switch (m_nMonthDisplay)
			{
			case GTLC_DISPLAY_YEARS:
				// Column == 12 months
				nDaysInCol = (int)DAYS_IN_YEAR;
				dDayInCol += (int)((nMonth - 1) * DAYS_IN_MONTH);
				break;
				
			case GTLC_DISPLAY_QUARTERS_SHORT:
			case GTLC_DISPLAY_QUARTERS_MID:
			case GTLC_DISPLAY_QUARTERS_LONG:
				// Column == 3 months
				nDaysInCol = (int)(DAYS_IN_MONTH * 3);
				dDayInCol += (int)(((nMonth -1) % 3) * DAYS_IN_MONTH);
				break;

			default: 
				// Column == Month
				nDaysInCol = CDateHelper::GetDaysInMonth(nMonth, nYear);
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
	int nNumReqColumns = GetRequiredColumnCount();

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

	return -1;
}

int CGanttTreeListCtrl::FindColumn(const COleDateTime& date) const
{
	return FindColumn(date.GetMonth(), date.GetYear());
}

int CGanttTreeListCtrl::FindColumn(int nScrollPos) const
{
	int nNumReqColumns = GetRequiredColumnCount();

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

bool CGanttTreeListCtrl::PrepareNewTask(ITaskList* pTask) const
{
	// Set the start date to today and of duration 1 day
	HTASKITEM hNewTask = pTask->GetFirstTask();
	ASSERT(hNewTask);

	COleDateTime dt = CDateHelper::GetDate(DHD_TODAY);
	time_t tDate = 0;

	if (CDateHelper::GetTimeT(dt, tDate))
	{
		pTask->SetTaskStartDate(hNewTask, tDate);
		pTask->SetTaskDueDate(hNewTask, tDate);
		//pTask->SetTaskTimeEstimate(hNewTask, 1.0, TDCU_WEEKDAYS);
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
		::ScreenToClient(m_hwndTree, &ptTree);
	
	TVHITTESTINFO tvht = { { ptTree.x, ptTree.y }, 0 };

	return TreeView_HitTest(m_hwndTree, &tvht);
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

		if (TreeView_GetItemRect(m_hwndTree, htiHit, &rItem, TRUE))
		{
			int nColWidth = m_treeHeader.GetItemWidth(0);

			rItem.left = max(rItem.left, 0);
			rItem.right = nColWidth;

			CPoint ptClient(ptScreen);
			::ScreenToClient(m_hwndTree, &ptClient);

			if (rItem.PtInRect(ptClient))
			{
				const GANTTITEM* pGI = GetGanttItem(GetTaskID(htiHit));
				ASSERT(pGI);

				int nTextLen = GraphicsMisc::GetTextWidth(pGI->sTitle, *(CWnd::FromHandle(m_hwndTree)));
				rItem.DeflateRect(TV_TIPPADDING, 0);

				if (nTextLen > rItem.Width())
					return pGI->sTitle;
			}
		}
	}

	// else
	return _T("");
}

BOOL CGanttTreeListCtrl::PtInHeader(const CPoint& ptScreen) const
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

	::GetWindowRect(m_hwndTree, rTree);
	::GetWindowRect(m_hwndList, rList);

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
		::ScreenToClient(m_hwndList, &(lvht.pt));

	if ((ListView_SubItemHitTest(m_hwndList, &lvht) != -1) &&	(lvht.iSubItem > 0))
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
	GetStartDueDates(*pGI, dtStart, dtDue);

	// Calculate the task rect
	CRect rTask;
	VERIFY(ListView_GetItemRect(m_hwndList, nItem, rTask, LVIR_BOUNDS));

	if (!CDateHelper::DateHasTime(dtDue))
		dtDue += 1.0;

	rTask.right = GetScrollPosFromDate(dtDue);
	rTask.left = GetScrollPosFromDate(dtStart);

	rTask.OffsetRect(-GetScrollPos(m_hwndList, SB_HORZ), 0);

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
		::ScreenToClient(m_hwndList, &ptClient);
	
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

DWORD CGanttTreeListCtrl::GetTaskID(HTREEITEM hti) const
{
	return GetTreeItemData(m_hwndTree, hti);
}

DWORD CGanttTreeListCtrl::GetTaskID(int nItem) const
{
	return GetListTaskID(GetListItemData(m_hwndList, nItem));
}

DWORD CGanttTreeListCtrl::GetListTaskID(DWORD dwItemData) const
{
	return GetTaskID((HTREEITEM)dwItemData);
}

BOOL CGanttTreeListCtrl::IsDragging() const
{
	return (m_bDragging || m_bDraggingStart || m_bDraggingEnd);
}

BOOL CGanttTreeListCtrl::ValidateDragPoint(CPoint& ptDrag) const
{
	if (!IsDragging())
		return FALSE;

	CRect rClient;
	::GetClientRect(m_hwndList, rClient);

	ptDrag.x = max(ptDrag.x, rClient.left);
	ptDrag.x = min(ptDrag.x, rClient.right);
	ptDrag.y = max(ptDrag.y, rClient.top);
	ptDrag.y = min(ptDrag.y, rClient.bottom);

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

	if (dwTaskID != GetSelectedTaskID())
		SelectTask(dwTaskID);

	CPoint ptScreen(ptCursor);
	::ClientToScreen(m_hwndList, &ptScreen);
	
	if (!DragDetect(m_hwndList, ptScreen))
		return FALSE;

	switch (nHit)
	{
	case GTLCHT_BEGIN:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		m_bDraggingStart = TRUE;
		break;
		
	case GTLCHT_END:
		m_bDraggingEnd = TRUE;
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		break;
		
	case GTLCHT_MIDDLE:
		m_bDragging = TRUE;
		break;
		
	default:
		ASSERT(0);
		return FALSE;
	}
	
	GANTTITEM* pGI = GetGanttItem(dwTaskID, FALSE);
	ASSERT(pGI);

	// cache the original task
	m_giPreDrag = *pGI;
	m_ptDragStart = ptCursor;
	m_dtDragMin = CalcMinDragDate(m_giPreDrag);

	// make sure the gantt item actually has valid dates
	COleDateTime dtStart, dtDue;
	GetStartDueDates(*pGI, dtStart, dtDue);
	
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
	
	::SetCapture(m_hwndList);
	
	// keep parent informed
	NotifyParentDragChange();

	return TRUE;
}

COleDateTime CGanttTreeListCtrl::CalcMinDragDate(const GANTTITEM& gi) const
{
	COleDateTime dtMin;
	CDateHelper::ClearDate(dtMin);

	int nDepend = gi.aDepends.GetSize();

	while (nDepend--)
	{
		DWORD dwDependID = _ttoi(gi.aDepends[nDepend]);

		if (dwDependID == 0)
			continue;

		const GANTTITEM* pGI = GetGanttItem(dwDependID);
		ASSERT(pGI);

		if (pGI)
			CDateHelper::Max(dtMin, pGI->dtDue);
	}

	return dtMin;
}

BOOL CGanttTreeListCtrl::EndDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);
	ASSERT(!IsDependencyEditing());
	
	if (IsDragging())
	{
		DWORD dwTaskID = GetSelectedTaskID();
		GANTTITEM* pGI = GetGanttItem(dwTaskID);
		ASSERT(pGI);

		GTLC_HITTEST nDragWhat = GTLCHT_NOWHERE;

		if (pGI)
		{
			// update taskID to refID because we're really dragging the refID
			if (pGI->dwOrgRefID)
			{
				dwTaskID = pGI->dwOrgRefID;
				pGI->dwOrgRefID = 0;
			}

			// dropping outside the list is a cancel
			CRect rList;
			::GetClientRect(m_hwndList, rList);
			
			if (!rList.PtInRect(ptCursor))
			{
				GANTTITEM* pGI = GetGanttItem(dwTaskID);
				ASSERT(pGI);

				(*pGI) = m_giPreDrag;
			}
			else if (m_bDraggingStart)
			{
				nDragWhat = GTLCHT_BEGIN;
			}
			else if (m_bDraggingEnd)
			{
				nDragWhat = GTLCHT_END;
			}
			else
			{
				ASSERT(m_bDragging);
				nDragWhat = GTLCHT_MIDDLE;
			}
		}
		
		// cleanup
		m_bDraggingStart = m_bDraggingEnd = m_bDragging = FALSE;

		::ReleaseCapture();
		RedrawList();

		// keep parent informed
		if (!NotifyParentDateChange(nDragWhat))
		{
			GANTTITEM* pGI = GetGanttItem(dwTaskID);
			ASSERT(pGI);
			
			(*pGI) = m_giPreDrag;
			RedrawList();
		}
		NotifyParentDragChange();

		return TRUE;
	}

	// else
	return FALSE;
}

void CGanttTreeListCtrl::NotifyParentDragChange()
{
	ASSERT(!m_bReadOnly);
	ASSERT(GetSelectedTaskID());

	GetCWnd()->SendMessage(WM_GTLC_DRAGCHANGE, (WPARAM)GetSnapMode(), GetSelectedTaskID());
}

BOOL CGanttTreeListCtrl::NotifyParentDateChange(GTLC_HITTEST nHit)
{
	ASSERT(!m_bReadOnly);
	ASSERT(GetSelectedTaskID());

	if (nHit != GTLCHT_NOWHERE)
		return GetCWnd()->SendMessage(WM_GTLC_DATECHANGE, (WPARAM)nHit, (LPARAM)&m_giPreDrag);

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
		GetStartDueDates(*pGI, dtStart, dtDue);

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

			if (m_bDragging) 
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
			else // dragging start/end
			{
				double dMinDuration = CalcMinDragDuration();

				if (m_bDraggingStart)
				{
					// prevent the start and end dates from overlapping
					pGI->dtStart.m_dt = min(dtDrag.m_dt, (dtDue.m_dt - dMinDuration));
				}
				else if (m_bDraggingEnd)
				{
					// prevent the start and end dates from overlapping
					pGI->dtDue.m_dt = max(dtDrag.m_dt, (dtStart.m_dt + dMinDuration));

					// handle day boundary
					if (!CDateHelper::DateHasTime(pGI->dtDue))
					{
						pGI->dtDue.m_dt = (CDateHelper::GetEndOfDay(pGI->dtDue).m_dt - 1.0);
					}
				}

				szCursor = IDC_SIZEWE;
			}
			ASSERT(szCursor);

			::SetCursor(bNoDrag ? GraphicsMisc::OleDragDropCursor(GMOC_NO) : AfxGetApp()->LoadStandardCursor(szCursor));

			RecalcParentDates();
			RedrawList();
			RedrawTree();

			// keep parent informed
			NotifyParentDragChange();
		}
		else
		{
			ASSERT(0);
		}

		return TRUE; // always
	}

	// else
	return FALSE; // not dragging
}

double CGanttTreeListCtrl::CalcMinDragDuration() const
{
	ASSERT(m_bDraggingStart || m_bDraggingEnd);
	
	double dMin;

	if (CalcMinDragDuration(GetSnapMode(), dMin))
		return dMin;

	switch (m_nMonthDisplay)
	{
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
	case GTLCSM_NEARESTYEAR:		dMin = 365.0;		break;
	case GTLCSM_NEARESTHALFYEAR:	dMin = 182.0;		break;
	case GTLCSM_NEARESTQUARTER:		dMin = 91.0;		break;
	case GTLCSM_NEARESTMONTH:		dMin = 30.0;		break;
	case GTLCSM_NEARESTWEEK:		dMin = 7.0;			break;
	case GTLCSM_NEARESTDAY:			dMin = 1.0;			break;
	case GTLCSM_NEARESTHALFDAY:		dMin = 0.5;			break;
	case GTLCSM_NEARESTHOUR:		dMin = (1.0 / 24);	break;

	case GTLCSM_FREE:
		break;

	default:
		ASSERT(0);
	}

	return (dMin > 0.0);
}

void CGanttTreeListCtrl::RedrawList(BOOL bErase)
{
	::InvalidateRect(m_hwndList, NULL, bErase);
	::UpdateWindow(m_hwndList);
}

void CGanttTreeListCtrl::RedrawTree(BOOL bErase)
{
	::InvalidateRect(m_hwndTree, NULL, bErase);
	::UpdateWindow(m_hwndTree);
}

BOOL CGanttTreeListCtrl::GetValidDragDate(const CPoint& ptCursor, COleDateTime& dtDrag) const
{
	CPoint ptDrag(ptCursor);

	if (!ValidateDragPoint(ptDrag))
		return FALSE;

	if (!GetDateFromPoint(ptDrag, dtDrag))
		return FALSE;

	// if dragging the whole task, then we calculate
	// dtDrag as GANTTITEM::dtStart offset by the
	// difference between the current drag pos and the
	// initial drag pos
	if (m_bDragging)
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
	// quick hit test
	CRect rList;
	::GetClientRect(m_hwndList, rList);

	if (!rList.PtInRect(ptCursor))
		return FALSE;

	// convert pos to date
	int nScrollPos = (::GetScrollPos(m_hwndList, SB_HORZ) + ptCursor.x);

	return (GetDateFromScrollPos(nScrollPos, date) && CDateHelper::IsDateSet(date));
}

// external version
BOOL CGanttTreeListCtrl::CancelOperation()
{
	if (IsDragging())
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

	// cancel drag, restoring original task dates
	DWORD dwTaskID = GetSelectedTaskID();

	GANTTITEM* pGI = GetGanttItem(dwTaskID);
	ASSERT(pGI);

	(*pGI) = m_giPreDrag;
	m_bDragging = m_bDraggingStart = m_bDraggingEnd = FALSE;
	
	if (bReleaseCapture)
		ReleaseCapture();

	RedrawList();

	// keep parent informed
	NotifyParentDragChange();
}

int CGanttTreeListCtrl::GetColumnOrder(CIntArray& aTreeOrder) const
{
	return m_treeHeader.GetItemOrder(aTreeOrder);
}

BOOL CGanttTreeListCtrl::SetColumnOrder(const CIntArray& aTreeOrder)
{
	ASSERT(aTreeOrder.GetSize() && (aTreeOrder[0] == GTLCC_TITLE));

	if (!(aTreeOrder.GetSize() && (aTreeOrder[0] == GTLCC_TITLE)))
		return FALSE;

	return m_treeHeader.SetItemOrder(aTreeOrder);
}

void CGanttTreeListCtrl::GetColumnWidths(CIntArray& aTreeWidths, CIntArray& aListWidths) const
{
	m_treeHeader.GetItemWidths(aTreeWidths);
	m_listHeader.GetItemWidths(aListWidths);

	// trim the list columns to what's currently visible
	// remember to include hidden dummy first column
	int nNumMonths = (GetRequiredColumnCount() + 1);
	int nItem = aListWidths.GetSize();

	while (nItem-- > nNumMonths)
		aListWidths.RemoveAt(nItem);
}

void CGanttTreeListCtrl::SetColumnWidths(const CIntArray& aTreeWidths, const CIntArray& aListWidths)
{
	m_treeHeader.SetItemWidths(aTreeWidths);

	// save list column widths for when we've initialised our columns
	// remember to include hidden dummy first column
	if (aListWidths.GetSize() == (GetRequiredColumnCount() + 1))
		m_listHeader.SetItemWidths(aListWidths);
	else
		m_aPrevColWidths.Copy(aListWidths);
}

void CGanttTreeListCtrl::GetTrackedColumns(CIntArray& aTreeTracked, CIntArray& aListTracked) const
{
	m_treeHeader.GetTrackedItems(aTreeTracked);
	m_listHeader.GetTrackedItems(aListTracked);

	// trim the list columns to what's currently visible
	// remember to include hidden dummy first column
	int nNumMonths = (GetRequiredColumnCount() + 1);
	int nItem = aListTracked.GetSize();

	while (nItem-- > nNumMonths)
		aListTracked.RemoveAt(nItem);
}

void CGanttTreeListCtrl::SetTrackedColumns(const CIntArray& aTreeTracked, const CIntArray& aListTracked)
{
	m_treeHeader.SetTrackedItems(aTreeTracked); 

	// save list column tracking for when we've initialised our columns
	// remember to include hidden dummy first column
	if (aListTracked.GetSize() == (GetRequiredColumnCount() + 1))
		m_listHeader.SetTrackedItems(aListTracked);
	else
		m_aPrevTrackedCols.Copy(aListTracked);
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

	switch (GetSnapMode())
	{
	case GTLCSM_NEARESTYEAR:
		return CDateHelper::GetNearestYear(dtDrag, m_bDraggingEnd);

	case GTLCSM_NEARESTHALFYEAR:
		return CDateHelper::GetNearestHalfYear(dtDrag, m_bDraggingEnd);

	case GTLCSM_NEARESTQUARTER:
		return CDateHelper::GetNearestQuarter(dtDrag, m_bDraggingEnd);

	case GTLCSM_NEARESTMONTH:
		return CDateHelper::GetNearestMonth(dtDrag, m_bDraggingEnd);

	case GTLCSM_NEARESTWEEK:
		return CDateHelper::GetNearestWeek(dtDrag, m_bDraggingEnd);

	case GTLCSM_NEARESTDAY:
		return CDateHelper::GetNearestDay(dtDrag, m_bDraggingEnd);

	case GTLCSM_NEARESTHALFDAY:
		return CDateHelper::GetNearestHalfDay(dtDrag, m_bDraggingEnd);

	case GTLCSM_NEARESTHOUR:
		return CDateHelper::GetNearestHour(dtDrag, m_bDraggingEnd);

	case GTLCSM_FREE:
		if (m_bDraggingEnd)
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
	return (Misc::Find(gi.aTags, m_sMilestoneTag, FALSE, FALSE) != -1);
}

DWORD CGanttTreeListCtrl::GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const
{
	HTREEITEM hti = FindTreeItem(m_hwndTree, dwTaskID);
	
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
			HTREEITEM htiNext = TCH()->GetNextVisibleItem(hti);
			
			if (htiNext)
				dwNextID = GetTreeItemData(m_hwndTree, htiNext);
		}
		break;
		
	case IUI_GETPREVTASK:
		{
			HTREEITEM htiPrev = TCH()->GetPrevVisibleItem(hti);
			
			if (htiPrev)
				dwNextID = GetTreeItemData(m_hwndTree, htiPrev);
		}
		break;
		
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTOPLEVELTASK:
		{
			HTREEITEM htiNext = TCH()->GetNextTopLevelItem(hti, (nCmd == IUI_GETNEXTTOPLEVELTASK));
			
			if (htiNext)
				dwNextID = GetTreeItemData(m_hwndTree, htiNext);
		}
		break;
		
	default:
		ASSERT(0);
	}
	
	return dwNextID;
}
