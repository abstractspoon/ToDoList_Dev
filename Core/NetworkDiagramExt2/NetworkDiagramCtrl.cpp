// NetworkDiagramCtrl.cpp: implementation of the CNetworkDiagramCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
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

using namespace NetworkDiagramStatic;

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

#define GET_NI_RET(id, ni, ret)	\
{								\
	if (id == 0) return ret;	\
	ni = GetNetworkItem(id);	\
	ASSERT(ni);					\
	if (ni == NULL) return ret;	\
}

#define GET_NI(id, ni)			\
{								\
	if (id == 0) return;		\
	ni = GetNetworkItem(id);	\
	ASSERT(ni);					\
	if (ni == NULL)	return;		\
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
	m_dwMaxTaskID(0)
{
	SetMinItemHeight(30);
}

CNetworkDiagramCtrl::~CNetworkDiagramCtrl()
{
}

BEGIN_MESSAGE_MAP(CNetworkDiagramCtrl, CEnListCtrl)
END_MESSAGE_MAP()


void CNetworkDiagramCtrl::PreSubclassWindow()
{
	CEnListCtrl::PreSubclassWindow();
	
	ShowGrid(TRUE, TRUE);

	BuildListColumns();
}

void CNetworkDiagramCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nRow = (int)lpDrawItemStruct->itemID;

	// Draw all group items having a vertical pos equal to the item row
	int nGroup = m_mapGroups.GetCount();

	while (nGroup--)
	{
		const CNetworkGroup* pGroup = m_mapGroups.GetMapping(nGroup);
		ASSERT(pGroup);

		POSITION pos = pGroup->GetStartPosition();

		while (pos)
		{
			DWORD dwTaskID;
			NETWORKGROUPITEM ngi;

			pGroup->GetNextAssoc(pos, dwTaskID, ngi);

			if (ngi.ptLocation.y == nRow)
			{
				CRect rItem;
				GetSubItemRect(nRow, ngi.ptLocation.x + 1, LVIR_BOUNDS, rItem);

				rItem.DeflateRect(0, 2, 10, 2);
				GraphicsMisc::DrawRect(pDC, rItem, CLR_NONE, 0, 2);

				pDC->DrawText(Misc::Format(dwTaskID), rItem, DT_SINGLELINE);
			}
		}
	}
}

DWORD CNetworkDiagramCtrl::GetSelectedTaskID() const
{
	return 0;//GetSelectedItemData();
}

BOOL CNetworkDiagramCtrl::GetSelectedTaskDependencies(CDWordArray& aDepends) const
{
	DWORD dwTaskID = GetSelectedTaskID();
	const NETWORKITEM* pNI = NULL;
	
	GET_NI_RET(dwTaskID, pNI, FALSE);
	
	aDepends.Copy(pNI->aDependIDs);
	return TRUE;
}

BOOL CNetworkDiagramCtrl::AddSelectedTaskDependency(DWORD dwDependID)
{
	// sanity check
	if (!m_data.HasItem(dwDependID))
		return FALSE;

	DWORD dwTaskID = GetSelectedTaskID();
	NETWORKITEM* pNI = NULL;
	
	GET_NI_RET(dwTaskID, pNI, FALSE);

	return Misc::AddUniqueItemT(dwDependID, pNI->aDependIDs);
}

BOOL CNetworkDiagramCtrl::EditSelectedTaskDependency(DWORD dwFromDependID, DWORD dwToDependID)
{
	// sanity check
	if (!m_data.HasItem(dwToDependID))
		return FALSE;

	DWORD dwTaskID = GetSelectedTaskID();
	NETWORKITEM* pNI = NULL;

	GET_NI_RET(dwTaskID, pNI, FALSE);

	// Target task must not already be a dependency
	if (Misc::FindT(dwToDependID, pNI->aDependIDs) != -1)
		return FALSE;

	int nFind = Misc::FindT(dwFromDependID, pNI->aDependIDs);

	if (nFind == -1)
		return FALSE;

	pNI->aDependIDs[nFind] = dwToDependID;
	return TRUE;
}

BOOL CNetworkDiagramCtrl::DeleteSelectedTaskDependency(DWORD dwDependID)
{
	DWORD dwTaskID = GetSelectedTaskID();
	NETWORKITEM* pNI = NULL;

	GET_NI_RET(dwTaskID, pNI, FALSE);

	return Misc::RemoveItemT(dwDependID, pNI->aDependIDs);
}

BOOL CNetworkDiagramCtrl::GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const
{
	DWORD dwTaskID = GetSelectedTaskID();
	const NETWORKITEM* pNI = NULL;

	GET_NI_RET(dwTaskID, pNI, FALSE);
	
/*
	if (GetTaskStartEndDates(*pNI, dtStart, dtDue))
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

	return FALSE;
}

void CNetworkDiagramCtrl::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate)
{
	// we must have been initialized already
	ASSERT(GetSafeHwnd());

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
		{
			CLockUpdates lu(*this);

			DWORD dwSelID = GetSelectedTaskID();
			
			m_data.RemoveAll();
			BuildData(pTasks, pTasks->GetFirstTask(), TRUE);

			// TODO

			SelectTask(dwSelID);

			if (dwSelID)
				ScrollToSelectedTask();
		}
		break;
		
	case IUI_NEW:
	case IUI_EDIT:
		{
			CHoldRedraw hr(GetSafeHwnd());
			
			// update the task(s)
			if (UpdateData(pTasks, pTasks->GetFirstTask(), nUpdate, TRUE))
			{
				// TODO
			}
		}
		break;
		
	case IUI_DELETE:
		{
			CHoldRedraw hr(GetSafeHwnd());

			if (RemoveDeletedTasks(pTasks))
			{
				// TODO
			}
		}
		break;
		
	default:
		ASSERT(0);
	}

	int nMaxVPos = 0;
	m_data.BuildDependencyGroups(m_mapGroups, nMaxVPos);

	int nListSize = GetItemCount();

	if (nMaxVPos <= nListSize)
	{
		while (--nListSize > nMaxVPos)
			DeleteItem(nListSize);
	}
	else if (nMaxVPos > nListSize)
	{
		for (int nVPos = nListSize; nVPos <= nMaxVPos; nVPos++)
			InsertItem(nVPos, _T(""));
	}

	Invalidate();
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
	return FALSE;
}

void CNetworkDiagramCtrl::BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;

	// Only interested in non-references
	if (pTasks->GetTaskReferenceID(hTask))
		return;

	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	ASSERT(!m_data.HasItem(dwTaskID));

	m_dwMaxTaskID = max(m_dwMaxTaskID, dwTaskID);

	// map the data
	NETWORKITEM* pNI = new NETWORKITEM;
	m_data[dwTaskID] = pNI;

	VERIFY(pNI->Set(pTasks, hTask));

	// add first child which will add all the rest
	BuildData(pTasks, pTasks->GetFirstTask(hTask), TRUE);

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);

		while (hSibling)
		{
			// FALSE == not siblings
			BuildData(pTasks, hSibling, FALSE);

			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

BOOL CNetworkDiagramCtrl::UpdateData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings)
{
	ASSERT((nUpdate == IUI_EDIT) || (nUpdate == IUI_NEW));

	if (hTask == NULL)
		return FALSE;

	// Only interested in non-references
	if (pTasks->GetTaskReferenceID(hTask))
		return FALSE;
	
	// handle task if not NULL (== root)
	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	m_dwMaxTaskID = max(m_dwMaxTaskID, dwTaskID);

	// This can be a new task
	if (!m_data.HasItem(dwTaskID))
	{
		ASSERT(nUpdate == IUI_NEW);
		return TRUE;
	}
	
	NETWORKITEM* pNI = m_data.GetItem(dwTaskID);
	ASSERT(pNI);

	BOOL bChange = pNI->Update(pTasks, hTask);

	// children
	bChange |= UpdateData(pTasks, pTasks->GetFirstTask(hTask), nUpdate, TRUE);

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);

		while (hSibling)
		{
			bChange |= UpdateData(pTasks, hSibling, nUpdate, FALSE); // FALSE == not siblings

			hSibling = pTasks->GetNextTask(hSibling);
		}
	}

	return bChange;
}

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

int CNetworkDiagramCtrl::RemoveDeletedTasks(const ITASKLISTBASE* pTasks)
{
	CDWordSet mapIDs;
	BuildTaskMap(pTasks, pTasks->GetFirstTask(), mapIDs, TRUE);

	POSITION pos = m_data.GetStartPosition();
	int nNumRemoved = 0;

	while (pos)
	{
		DWORD dwTaskID = m_data.GetNextTaskID(pos);

		if (!mapIDs.Has(dwTaskID) && m_data.DeleteItem(dwTaskID))
			nNumRemoved++;
	}

	return nNumRemoved;
}

NETWORKITEM* CNetworkDiagramCtrl::GetNetworkItem(DWORD dwTaskID) const
{
	return m_data.GetItem(dwTaskID);
}

BOOL CNetworkDiagramCtrl::RestoreNetworkItem(const NETWORKITEM& niPrev)
{
	if (m_data.RestoreItem(niPrev))
	{
		// TODO	
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
	if (CWnd::GetParent()->SendMessage(WM_NDC_MOVETASK, 0, (LPARAM)&taskMove) && !taskMove.bCopy)
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
		NETWORKDEPENDENCY depend;
		
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
		NETWORKDEPENDENCY depend;

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
void CNetworkDiagramCtrl::GetItemColors(const NETWORKITEM& ni, COLORREF& crBorder, COLORREF& crFill) const
{
	// darker shade of the item crText/crBack
	COLORREF crDefFill = ni.GetFillColor();
	COLORREF crDefBorder = ni.GetBorderColor();

	if (crDefFill == CLR_NONE)
	{
		if (HasColor(m_crBarDefault) && (!ni.bParent || m_nParentColoring == GTLPC_DEFAULTCOLORING))
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

	if (ni.bParent)
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
				!TCH().IsItemExpanded(GetTreeItem(ni.dwTaskID)))
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

DWORD CNetworkDiagramCtrl::DependsHitTest(const CPoint& ptClient, DWORD& dwToTaskID)
{
	CNetworkDependArray aDepends;
	
	int nDepend = BuildVisibleDependencyList(aDepends);

	while (nDepend--)
	{
		const NETWORKDEPENDENCY& depend = aDepends[nDepend];

		if (depend.HitTest(ptClient))
		{
			dwToTaskID = depend.GetToID();
			return depend.GetFromID();
		}
	}

	return 0;
}

int CNetworkDiagramCtrl::BuildVisibleDependencyList(CNetworkDependArray& aDepends) const
{
	aDepends.RemoveAll();

	// TODO
	
	return aDepends.GetSize();
}

BOOL CNetworkDiagramCtrl::BuildDependency(DWORD dwFromTaskID, DWORD dwToTaskID, NETWORKDEPENDENCY& depend) const
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

BOOL CNetworkDiagramCtrl::CalcDependencyEndPos(DWORD dwTaskID, NETWORKDEPENDENCY& depend, BOOL bFrom, LPPOINT lpp) const
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

BOOL CNetworkDiagramCtrl::CalcDependencyEndPos(DWORD dwTaskID, int nItem, NETWORKDEPENDENCY& depend, BOOL bFrom, LPPOINT lpp) const
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
	const NETWORKITEM* pNI = NULL;
	GET_NI_RET(dwTaskID, pNI, FALSE);

	CRect rItem, rMilestone;
	VERIFY(GetListItemRect(nItem, rItem));


	if (CalcMilestoneRect(*pNI, rItem, rMilestone))
	{
		if (bFrom)
			nPos = rMilestone.CenterPoint().x;
		else
			nPos = rMilestone.right;
	}
	else
	{
		COleDateTime dtStart, dtDue;

		if (!GetTaskStartEndDates(*pNI, dtStart, dtDue))
			return FALSE;

		if (!GetDrawPosFromDate((bFrom ? dtStart : dtDue), nPos))
			return FALSE;

		if (pNI->bParent && HasOption(GTLCF_CALCPARENTDATES))
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

BOOL CNetworkDiagramCtrl::BeginDependencyEdit(INetworkDependencyEditor* pDependEdit)
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
	// once only
	if (GetColumnCount())
		return;

	// add empty column as placeholder so we can
	// easily replace the other columns without
	// losing all our items too
	LVCOLUMN lvc = { LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, 0 };
	InsertColumn(0, &lvc);
	
	// add other columns
	int nNumCols = 50; // TODO

	for (int i = 1; i <= nNumCols; i++)
	{
		lvc.cx = 150;
		lvc.fmt = LVCFMT_CENTER | HDF_STRING;
		lvc.pszText = _T("");
		lvc.cchTextMax = 0;

		InsertColumn(i, &lvc);
	}
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
	NETWORKITEM* pNI = NULL;
	GET_NI(dwTaskID, pNI);

	// TODO
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

BOOL CNetworkDiagramCtrl::CanDragTask(DWORD dwTaskID) const
{
	if (m_data.ItemIsLocked(dwTaskID))
		return FALSE;

	// else
	return TRUE;
}

BOOL CNetworkDiagramCtrl::StartDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);
	ASSERT(!IsDependencyEditing());

	DWORD dwTaskID = 0;//ListHitTestTask(ptCursor, FALSE);

	if (!dwTaskID)
		return FALSE;

	if (!CanDragTask(dwTaskID))
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return FALSE;
	}
	
	if (dwTaskID != GetSelectedTaskID())
		SelectTask(dwTaskID);

	CPoint ptScreen(ptCursor);
	ClientToScreen(&ptScreen);
	
	if (!::DragDetect(*this, ptScreen))
		return FALSE;

	NETWORKITEM* pNI = NULL;
	GET_NI_RET(dwTaskID, pNI, FALSE);
	
	// cache the original task and the start point
	m_niPreDrag = *pNI;
	m_ptDragStart = ptCursor;

	// Start dragging
	// TODO
	SetCapture();
	
	return TRUE;
}

BOOL CNetworkDiagramCtrl::EndDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);
	ASSERT(!IsDependencyEditing());
	ASSERT(IsDragging());

	if (IsDragging())
	{
		// dropping outside the list is a cancel
		if (!IsValidDragPoint(ptCursor))
		{
			CancelDrag(TRUE);
			return FALSE;
		}

		// TODO
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CNetworkDiagramCtrl::UpdateDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);
	ASSERT(!IsDependencyEditing());
	
	if (IsDragging())
	{
		DWORD dwTaskID = GetSelectedTaskID();
		NETWORKITEM* pNI = NULL;

		GET_NI_RET(dwTaskID, pNI, FALSE);

		// TODO

		return TRUE; // always
	}

	// else
	return FALSE; // not dragging
}

void CNetworkDiagramCtrl::SetReadOnly(bool bReadOnly) 
{ 
	CEnListCtrl::SetReadOnly(bReadOnly);
}

// external version
BOOL CNetworkDiagramCtrl::CancelOperation()
{
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
void CNetworkDiagramCtrl::CancelDrag(BOOL bReleaseCapture)
{
	ASSERT(IsDragging());

	if (bReleaseCapture)
		ReleaseCapture();
	
	// cancel drag, restoring original task dependencies
	RestoreNetworkItem(m_niPreDrag);
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
	SendMessage(WM_SETFONT, (WPARAM)hFont);
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
