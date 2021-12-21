// GanttTreeList.cpp: implementation of the CGanttTreeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "resource.h"
#include "NetworkDiagramCtrl.h"
#include "NetworkDiagramMsg.h"
#include "NetworkDiagramStatic.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\DateHelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\graphicsMisc.h"
#include "..\shared\misc.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\themed.h"
#include "..\shared\enbitmap.h"

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

CNetworkDiagramCtrl::CNetworkDiagramCtrl() 
	:
	m_dwOptions(0),
	m_crBarDefault(CLR_NONE),
	m_crParent(CLR_NONE),
	m_ptDragStart(0),
	m_ptLastDependPick(0),
	m_pDependEdit(NULL),
	m_dwMaxTaskID(0),
	m_bReadOnly(FALSE)
{

}

CNetworkDiagramCtrl::~CNetworkDiagramCtrl()
{
}

BEGIN_MESSAGE_MAP(CNetworkDiagramCtrl, CEnListCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int CNetworkDiagramCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEnListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	BuildListColumns();

	return 0;
}

DWORD CNetworkDiagramCtrl::GetSelectedTaskID() const
{
	return 0;//GetSelectedItemData();
}

BOOL CNetworkDiagramCtrl::GetSelectedTaskDependencies(CDWordArray& aDepends) const
{
	DWORD dwTaskID = GetSelectedTaskID();
	const GANTTITEM* pGI = NULL;
	
	GET_GI_RET(dwTaskID, pGI, FALSE);
	
	aDepends.Copy(pGI->aDependIDs);
	return TRUE;
}

BOOL CNetworkDiagramCtrl::AddSelectedTaskDependency(DWORD dwDependID)
{
	// sanity check
	if (!m_data.HasItem(dwDependID))
		return FALSE;

	DWORD dwTaskID = GetSelectedTaskID();
	GANTTITEM* pGI = NULL;
	
	GET_GI_RET(dwTaskID, pGI, FALSE);

	return Misc::AddUniqueItemT(dwDependID, pGI->aDependIDs);
}

BOOL CNetworkDiagramCtrl::EditSelectedTaskDependency(DWORD dwFromDependID, DWORD dwToDependID)
{
	// sanity check
	if (!m_data.HasItem(dwToDependID))
		return FALSE;

	DWORD dwTaskID = GetSelectedTaskID();
	GANTTITEM* pGI = NULL;

	GET_GI_RET(dwTaskID, pGI, FALSE);

	// Target task must not already be a dependency
	if (Misc::FindT(dwToDependID, pGI->aDependIDs) != -1)
		return FALSE;

	int nFind = Misc::FindT(dwFromDependID, pGI->aDependIDs);

	if (nFind == -1)
		return FALSE;

	pGI->aDependIDs[nFind] = dwToDependID;
	return TRUE;
}

BOOL CNetworkDiagramCtrl::DeleteSelectedTaskDependency(DWORD dwDependID)
{
	DWORD dwTaskID = GetSelectedTaskID();
	GANTTITEM* pGI = NULL;

	GET_GI_RET(dwTaskID, pGI, FALSE);

	return Misc::RemoveItemT(dwDependID, pGI->aDependIDs);
}

BOOL CNetworkDiagramCtrl::GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const
{
	DWORD dwTaskID = GetSelectedTaskID();
	const GANTTITEM* pGI = NULL;

	GET_GI_RET(dwTaskID, pGI, FALSE);
	
/*
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
*/

	// all else
	return FALSE;
}

BOOL CNetworkDiagramCtrl::SelectTask(DWORD dwTaskID)
{
	return FALSE;
}

BOOL CNetworkDiagramCtrl::SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select)
{
	HTREEITEM htiStart = NULL;
	BOOL bForwards = TRUE;

/*
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

	CHoldRedraw hr(m_tree);
*/

	return SelectTask(htiStart, select, bForwards);
}

BOOL CNetworkDiagramCtrl::SelectTask(HTREEITEM hti, const IUISELECTTASK& select, BOOL bForwards)
{
	if (!hti)
		return FALSE;

// 	CString sTitle = m_tree.GetItemText(hti);
// 
// 	if (Misc::Find(select.szWords, sTitle, select.bCaseSensitive, select.bWholeWord) != -1)
// 	{
// 		if (SelectItem(hti))
// 			return TRUE;
// 
// 		ASSERT(0);
// 	}

// 	if (bForwards)
// 		return SelectTask(m_tree.TCH().GetNextItem(hti), select, TRUE);
// 
// 	// else
// 	return SelectTask(m_tree.TCH().GetPrevItem(hti), select, FALSE);
	return FALSE;
}

void CNetworkDiagramCtrl::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate)
{
	// we must have been initialized already
/*
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
	m_data.BuildDependencyChainLengths(m_mapDependencyChainLengths);
*/
}

BOOL CNetworkDiagramCtrl::WantEditUpdate(TDC_ATTRIBUTE nAttrib)
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

BOOL CNetworkDiagramCtrl::WantSortUpdate(TDC_ATTRIBUTE nAttrib)
{
	// all else 
	return FALSE;
}

BOOL CNetworkDiagramCtrl::UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings)
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
/*
		// can't use a switch here because we also need to check for IUI_ALL
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
				DWORD dwDependID = _ttoi(pTasks->GetTaskDependency(hTask, nDepend));

				if (dwDependID)
					pGI->aDependIDs.Add(dwDependID);
			}
		}

		// Always update these
		pGI->bLocked = pTasks->IsTaskLocked(hTask, true);
		pGI->bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);
*/
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

/*
void CNetworkDiagramCtrl::BuildTaskMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
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

void CNetworkDiagramCtrl::RemoveDeletedTasks(HTREEITEM hti, const ITASKLISTBASE* pTasks, const CDWordSet& mapIDs)
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
*/

GANTTITEM* CNetworkDiagramCtrl::GetGanttItem(DWORD dwTaskID) const
{
	return m_data.GetItem(dwTaskID, TRUE);
}

BOOL CNetworkDiagramCtrl::RestoreGanttItem(const GANTTITEM& giPrev)
{
	if (m_data.RestoreItem(giPrev))
	{
// 		RecalcParentDates();
// 		RedrawList();
	
		return TRUE;
	}

	// else
	return FALSE;
}

void CNetworkDiagramCtrl::SetOption(DWORD dwOption, BOOL bSet)
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
/*
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
				CEnListCtrl::SetSplitBarWidth(bSet ? 10 : 0);
				break;

			case GTLCF_DISPLAYISODATES:
				UpdateListColumnsWidthAndText();
				InvalidateAll();
				break;
			}

			if (IsSyncing())
				RedrawList();
*/
		}
	}
}

/*
UINT CNetworkDiagramCtrl::OnDragOverItem(const TLCITEMMOVE& move, UINT nCursor)
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

BOOL CNetworkDiagramCtrl::OnDragDropItem(const TLCITEMMOVE& move)
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

BOOL CNetworkDiagramCtrl::OnDragBeginItem(const TLCITEMMOVE& move, BOOL bLeftDrag)
{
	if (!CEnListCtrl::OnDragBeginItem(move, bLeftDrag))
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
*/

void CNetworkDiagramCtrl::ClearDependencyPickLine(CDC* pDC)
{
	if (IsPickingDependencyToTask() && IsDependencyPickLinePosValid())
	{
		BOOL bNullDC = (pDC == NULL);

/*
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
*/
	}
}

BOOL CNetworkDiagramCtrl::IsDependencyPickLinePosValid() const
{
	if (IsPickingDependencyToTask()) 
	{
		return ((m_ptLastDependPick.x != DEPENDPICKPOS_NONE) || 
				(m_ptLastDependPick.y != DEPENDPICKPOS_NONE));
	}

	// else
	return FALSE;
}

void CNetworkDiagramCtrl::ResetDependencyPickLinePos()
{
	m_ptLastDependPick.x = m_ptLastDependPick.y = DEPENDPICKPOS_NONE;
}

BOOL CNetworkDiagramCtrl::DrawDependencyPickLine(const CPoint& ptClient)
{
	if (IsPickingDependencyToTask())
	{
		// calc 'from ' point
/*
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
*/
	}

	// else 
	return FALSE;
}

BOOL CNetworkDiagramCtrl::OnListMouseMove(UINT /*nFlags*/, CPoint point)
{
	if (!m_bReadOnly)
	{
/*
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
*/
	}

	// not handled
	return FALSE;
}

BOOL CNetworkDiagramCtrl::OnListLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bReadOnly)
	{
/*
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
*/
	}

	// all else
//	return CEnListCtrl::OnLButtonDown(nFlags, point);
	return FALSE;
}

BOOL CNetworkDiagramCtrl::OnListLButtonUp(UINT /*nFlags*/, CPoint point)
{
// 	if (IsDragging() && EndDragging(point))
// 	{
// 		return TRUE; // eat
// 	}

	// not handled
	return FALSE;
}

BOOL CNetworkDiagramCtrl::OnListLButtonDblClk(UINT nFlags, CPoint point)
{
// 	if (IsDependencyEditing())
// 		return FALSE;
// 
// 	return CEnListCtrl::OnListLButtonDblClk(nFlags, point);
	return FALSE;
}

void CNetworkDiagramCtrl::SetUITheme(const UITHEME& theme)
{
}

void CNetworkDiagramCtrl::SetDefaultBarColor(COLORREF crDefault)
{
//	SetColor(m_crBarDefault, crDefault);
}

// void CNetworkDiagramCtrl::SetMilestoneTag(const CString& sTag)
// {
// 	if (sTag != m_sMilestoneTag)
// 	{
// 		m_sMilestoneTag = sTag;
// 
// 		if (IsHooked())
// 			InvalidateAll();
// 	}
// }



/*
void CNetworkDiagramCtrl::GetGanttBarColors(const GANTTITEM& gi, COLORREF& crBorder, COLORREF& crFill) const
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
*/

DWORD CNetworkDiagramCtrl::ListDependsHitTest(const CPoint& ptClient, DWORD& dwToTaskID)
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

int CNetworkDiagramCtrl::BuildVisibleDependencyList(CGanttDependArray& aDepends) const
{
	aDepends.RemoveAll();

/*
	CRect rClient;
	GetClientRect(rClient);

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
*/
	
	return aDepends.GetSize();
}

int CNetworkDiagramCtrl::BuildVisibleDependencyList(HTREEITEM htiFrom, CGanttDependArray& aDepends) const
{
/*
	DWORD dwFromTaskID = GetTaskID(htiFrom);

	const GANTTITEM* pGIFrom = GetGanttItem(dwFromTaskID);
	ASSERT(pGIFrom);

	if (pGIFrom)
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
*/

	return aDepends.GetSize();
}

BOOL CNetworkDiagramCtrl::BuildDependency(DWORD dwFromTaskID, DWORD dwToTaskID, GANTTDEPENDENCY& depend) const
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

BOOL CNetworkDiagramCtrl::CalcDependencyEndPos(DWORD dwTaskID, GANTTDEPENDENCY& depend, BOOL bFrom, LPPOINT lpp) const
{
	ASSERT(m_data.HasItem(dwTaskID));

/*
	int nItem = GetListItem(dwTaskID);

	if (nItem == -1) // Collapsed 
	{
		// Use first visible parent as surrogate
		HTREEITEM htiParent = m_tree.GetParentItem(GetTreeItem(dwTaskID));
		ASSERT(htiParent);

		while (htiParent)
		{
			nItem = GetListItem(htiParent);

			if (nItem != -1)
				break;

			htiParent = m_tree.GetParentItem(htiParent);
		}
	}
*/

	return FALSE;//CalcDependencyEndPos(dwTaskID, nItem, depend, bFrom, lpp);
}

BOOL CNetworkDiagramCtrl::CalcDependencyEndPos(DWORD dwTaskID, int nItem, GANTTDEPENDENCY& depend, BOOL bFrom, LPPOINT lpp) const
{
/*
	if (nItem == -1)
		return FALSE;

	int nPos = -1;
	m_mapDependencyChainLengths.Lookup(dwTaskID, nPos);

	int nCol = 1 + (nPos * 2); // alternate columns
	CRect rCol;
	
	m_list.GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rCol);

	if (bFrom)
		nPos = rCol.left;
	else
		nPos = rCol.right;
*/

/*
	const GANTTITEM* pGI = NULL;
	GET_GI_RET(dwTaskID, pGI, FALSE);

	CRect rItem, rMilestone;
	VERIFY(GetListItemRect(nItem, rItem));


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
*/

/*
	CPoint pt(nPos, ((rCol.top + rCol.bottom) / 2));

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
*/

	return TRUE;
}

BOOL CNetworkDiagramCtrl::BeginDependencyEdit(IGanttDependencyEditor* pDependEdit)
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

void CNetworkDiagramCtrl::OnEndDepedencyEdit()
{
	m_pDependEdit = NULL;
	
	ResetDependencyPickLinePos();
	SetFocus();

// 	m_tree.SelectDropTarget(NULL);
// 	m_list.SetItemState(m_nPrevDropHilitedItem, 0, LVIS_DROPHILITED);
// 	m_list.Invalidate(TRUE);

//	m_nPrevDropHilitedItem = -1;
}

void CNetworkDiagramCtrl::EndDependencyEdit()
{
	if (IsDependencyEditing())
		m_pDependEdit->Cancel();

	OnEndDepedencyEdit();
}

BOOL CNetworkDiagramCtrl::IsDependencyEditing() const
{
	return (m_pDependEdit && m_pDependEdit->IsPicking());
}

BOOL CNetworkDiagramCtrl::IsPickingDependencyFromTask() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingFromTask());
}

BOOL CNetworkDiagramCtrl::IsPickingFromDependency() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingFromDependency());
}

BOOL CNetworkDiagramCtrl::IsPickingDependencyToTask() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingToTask());
}

BOOL CNetworkDiagramCtrl::IsDependencyEditingCancelled() const
{
	return (!m_pDependEdit || m_pDependEdit->IsPickingCancelled());
}

BOOL CNetworkDiagramCtrl::IsDependencyEditingComplete() const
{
	return (m_pDependEdit && m_pDependEdit->IsPickingCompleted());
}

void CNetworkDiagramCtrl::BuildListColumns()
{
/*
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
*/
}

void CNetworkDiagramCtrl::UpdateListColumns()
{
/*
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
*/
}

void CNetworkDiagramCtrl::ScrollToSelectedTask()
{
	ScrollToTask(GetSelectedTaskID());
}

void CNetworkDiagramCtrl::ScrollToTask(DWORD dwTaskID)
{
	GANTTITEM* pGI = NULL;
	GET_GI(dwTaskID, pGI);
	
/*
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
*/
}

bool CNetworkDiagramCtrl::PrepareNewTask(ITaskList* pTaskList) const
{
	ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return false;
	}

/*
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
*/

	return true;
}

DWORD CNetworkDiagramCtrl::HitTestTask(const CPoint& ptScreen, bool bTitleColumnOnly) const
{
// 	HTREEITEM htiHit = HitTestItem(ptScreen, bTitleColumnOnly);
// 
// 	if (htiHit)
// 		return GetTaskID(htiHit);

	return 0;
}

DWORD CNetworkDiagramCtrl::GetTaskID(int nItem) const
{
	return 0;//GetItemData(GetTreeItem(nItem));
}

BOOL CNetworkDiagramCtrl::IsDragging() const
{
	return FALSE; //IsDragging(m_nDragging);
}

BOOL CNetworkDiagramCtrl::IsValidDragPoint(const CPoint& ptDrag) const
{
	if (!IsDragging())
		return FALSE;

	CRect rLimits;
	GetDragLimits(rLimits);

	return rLimits.PtInRect(ptDrag);
}

void CNetworkDiagramCtrl::GetDragLimits(CRect& rLimits) const
{
/*
	m_list.GetClientRect(rLimits);

	// Clip the right hand end to the last column
	CRect rColumn;
	m_listHeader.GetItemRect(m_listHeader.GetItemCount() - 1, rColumn);
	rLimits.right = min(rLimits.right, rColumn.right);

	// Allow a buffer at start and end
	rLimits.InflateRect(DRAG_BUFFER, 0);
*/
}

BOOL CNetworkDiagramCtrl::ValidateDragPoint(CPoint& ptDrag) const
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

/*
BOOL CNetworkDiagramCtrl::CanDragTask(DWORD dwTaskID, GTLC_DRAG nDrag) const
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

BOOL CNetworkDiagramCtrl::StartDragging(const CPoint& ptCursor)
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

BOOL CNetworkDiagramCtrl::EndDragging(const CPoint& ptCursor)
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
*/

BOOL CNetworkDiagramCtrl::UpdateDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);
	ASSERT(!IsDependencyEditing());
	
	if (IsDragging())
	{
/*
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
*/

		return TRUE; // always
	}

	// else
	return FALSE; // not dragging
}

void CNetworkDiagramCtrl::SetReadOnly(bool bReadOnly) 
{ 
	m_bReadOnly = bReadOnly;

	//CEnListCtrl::EnableDragAndDrop(!bReadOnly);
}

// external version
BOOL CNetworkDiagramCtrl::CancelOperation()
{
/*
	if (CEnListCtrl::CancelOperation())
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
*/
	
	// else 
	return FALSE;
}

// internal version
void CNetworkDiagramCtrl::CancelDrag(BOOL bReleaseCapture)
{
	ASSERT(IsDragging());

	if (bReleaseCapture)
		ReleaseCapture();
	
/*
	// cancel drag, restoring original task dates
	RestoreGanttItem(m_giPreDrag);
	m_nDragging = GTLCD_NONE;

	// keep parent informed
	NotifyParentDragChange();
*/
}

DWORD CNetworkDiagramCtrl::GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const
{
/*
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
*/
	return 0L;
}

BOOL CNetworkDiagramCtrl::SaveToImage(CBitmap& bmImage)
{
/*
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
	
	return CEnListCtrl::SaveToImage(bmImage, nFrom, nTo);
*/
	return FALSE;
}

BOOL CNetworkDiagramCtrl::SetFont(HFONT hFont, BOOL bRedraw)
{
// 	if (!CEnListCtrl::SetFont(hFont, bRedraw))
// 		return FALSE;
// 
// 	CalcMinMonthWidths();
// 	SetMonthDisplay(m_nMonthDisplay);
// 	ResizeListColumnsToFit(TRUE);

	return TRUE;
}

BOOL CNetworkDiagramCtrl::CanMoveSelectedTask(const IUITASKMOVE& move) const
{
	if (m_bReadOnly)
		return FALSE;

// 	TLCITEMMOVE itemMove = { 0 };
// 
// 	itemMove.htiSel = GetTreeItem(move.dwSelectedTaskID);
// 	itemMove.htiDestParent = GetTreeItem(move.dwParentID);
// 	itemMove.htiDestAfterSibling = GetTreeItem(move.dwAfterSiblingID);
// 
	return FALSE;
}

BOOL CNetworkDiagramCtrl::MoveSelectedTask(const IUITASKMOVE& move)
{
	if (m_bReadOnly)
		return FALSE;
/*

	TLCITEMMOVE itemMove = { 0 };

	itemMove.htiSel = GetTreeItem(move.dwSelectedTaskID);
	itemMove.htiDestParent = GetTreeItem(move.dwParentID);
	itemMove.htiDestAfterSibling = GetTreeItem(move.dwAfterSiblingID);
*/

	return FALSE;
}
