// KanbanTreeList.cpp: implementation of the CKanbanTreeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KanbanCtrl.h"
#include "KanbanColumnCtrl.h"
#include "KanbanColors.h"
#include "KanbanMsg.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\DateHelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\graphicsMisc.h"
#include "..\shared\autoflag.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\themed.h"
#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\copywndcontents.h"
#include "..\shared\enbitmap.h"
#include "..\shared\DeferWndMove.h"

#include "..\Interfaces\iuiextension.h"
#include "..\Interfaces\ipreferences.h"
#include "..\Interfaces\TasklistSchemaDef.h"

#include <float.h> // for DBL_MAX
#include <math.h>  // for fabs()

//////////////////////////////////////////////////////////////////////

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

enum // RebuildColumns
{
	KCRC_REBUILDCONTENTS	= 0x01,
	KCRC_RESTORESELECTION	= 0x02,
};

//////////////////////////////////////////////////////////////////////

const UINT IDC_COLUMNCTRL	= 101;
const UINT IDC_HEADER		= 102;

//////////////////////////////////////////////////////////////////////

const int MIN_COL_WIDTH = GraphicsMisc::ScaleByDPIFactor(6);
const int HEADER_HEIGHT = GraphicsMisc::ScaleByDPIFactor(24);

//////////////////////////////////////////////////////////////////////

const CString EMPTY_STR;

const CPoint DRAG_NOT_SET(-10000, -10000);

//////////////////////////////////////////////////////////////////////

#define GET_KI_RET(id, ki, ret)	\
{								\
	if (id == 0) return ret;	\
	ki = GetKanbanItem(id);		\
	ASSERT(ki);					\
	if (ki == NULL) return ret;	\
}

#define GET_KI(id, ki)		\
{							\
	if (id == 0) return;	\
	ki = GetKanbanItem(id);	\
	ASSERT(ki);				\
	if (ki == NULL)	return;	\
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanbanCtrl::CKanbanCtrl() 
	:
	m_bSortAscending(-1), 
	m_dwOptions(0),
	m_bReadOnly(FALSE),
	m_nNextColor(0),
	m_pSelectedColumn(NULL),
	m_nTrackAttribute(TDCA_NONE),
	m_nSortBy(TDCA_NONE),
	m_bSelectTasks(FALSE),
	m_bResizingHeader(FALSE),
	m_bSettingColumnFocus(FALSE),
	m_bSavingToImage(FALSE)
{

}

CKanbanCtrl::~CKanbanCtrl()
{
}

BEGIN_MESSAGE_MAP(CKanbanCtrl, CWnd)
	//{{AFX_MSG_MAP(CKanbanCtrl)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_HEADER, OnHeaderCustomDraw)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_HEADER, OnHeaderClick)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, IDC_HEADER, OnHeaderDividerDoubleClick)
	ON_NOTIFY(HDN_ITEMCHANGING, IDC_HEADER, OnHeaderItemChanging)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_COLUMNCTRL, OnBeginDragColumnItem)
	ON_NOTIFY(TVN_SELCHANGED, IDC_COLUMNCTRL, OnColumnItemSelChange)
	ON_NOTIFY(NM_SETFOCUS, IDC_COLUMNCTRL, OnColumnSetFocus)
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_CAPTURECHANGED()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_KLCN_EDITTASKDONE, OnColumnEditTaskDone)
	ON_MESSAGE(WM_KLCN_EDITTASKFLAG, OnColumnEditTaskFlag)
	ON_MESSAGE(WM_KLCN_EDITTASKPIN, OnColumnEditTaskPin)
	ON_MESSAGE(WM_KLCN_GETTASKICON, OnColumnGetTaskIcon)
	ON_MESSAGE(WM_KLCN_EDITTASKICON, OnColumnEditTaskIcon)
	ON_MESSAGE(WM_KLCN_EDITTASKLABEL, OnColumnEditLabel)
	ON_MESSAGE(WM_KLCN_SHOWFILELINK, OnColumnShowFileLink)
	ON_REGISTERED_MESSAGE(WM_MIDNIGHT, OnMidnight)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

BOOL CKanbanCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
}

int CKanbanCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_bClosing = FALSE;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_fonts.Initialise(*this);
	
	ModifyStyleEx(0, WS_EX_CONTROLPARENT, 0);

	if (!m_header.Create(HDS_FULLDRAG | HDS_BUTTONS | WS_CHILD | WS_VISIBLE, 
						 CRect(lpCreateStruct->x, lpCreateStruct->y, lpCreateStruct->cx, 50),
						 this, IDC_HEADER))
	{
		return -1;
	}

	return 0;
}

void CKanbanCtrl::OnDestroy()
{
	m_bClosing = TRUE;

	CWnd::OnDestroy();
}

void CKanbanCtrl::FilterToolTipMessage(MSG* pMsg) 
{
	m_aColumns.FilterToolTipMessage(pMsg);
}

bool CKanbanCtrl::ProcessMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		return (HandleKeyDown(pMsg->wParam, pMsg->lParam) != FALSE);
	}
	
	// all else
	return false;
}

BOOL CKanbanCtrl::SelectClosestAdjacentItemToSelection(int nAdjacentCol)
{
	// Find the closest task at the currently
	// selected task's scrolled pos
	HTREEITEM hti = m_pSelectedColumn->GetFirstSelectedItem();

	if (!hti)
	{
		ASSERT(0);
		return FALSE;
	}

	if ((nAdjacentCol < 0) || (nAdjacentCol > m_aColumns.GetSize()))
	{
		ASSERT(0);
		return FALSE;
	}

	CKanbanColumnCtrl* pAdjacentCol = m_aColumns[nAdjacentCol];

	if (!pAdjacentCol->GetCount())
		return FALSE;

	// scroll into view first
	m_pSelectedColumn->ScrollToSelection();

	CRect rItem;
	VERIFY(m_pSelectedColumn->GetItemBounds(hti, &rItem));

	HTREEITEM htiClosest = pAdjacentCol->HitTest(rItem.CenterPoint());

	if (!htiClosest)
		htiClosest = pAdjacentCol->TCH().GetLastItem();

	SelectColumn(pAdjacentCol, FALSE);
	ASSERT(m_pSelectedColumn == pAdjacentCol);

	pAdjacentCol->SelectItem(htiClosest, FALSE); // FALSE -> by keyboard
	pAdjacentCol->UpdateWindow();
	
	return TRUE;
}

BOOL CKanbanCtrl::HandleKeyDown(WPARAM wp, LPARAM /*lp*/)
{
	if (!m_pSelectedColumn)
	{
		ASSERT(0);
		return FALSE;
	}

	// We need to process these before they reach the focused column 
	switch (wp)
	{
	case VK_LEFT:
		if (m_pSelectedColumn->GetFirstSelectedItem())
		{
			int nSelCol = GetSelColumnIndex();
			
			for (int nCol = (nSelCol - 1); nCol >= 0; nCol--)
			{
				if (SelectClosestAdjacentItemToSelection(nCol))
					return TRUE;
			}
		}
		break;

	case VK_RIGHT:
		if (m_pSelectedColumn->GetFirstSelectedItem())
		{
			int nSelCol = GetSelColumnIndex();
			int nNumCol = m_aColumns.GetSize();

			for (int nCol = (nSelCol + 1); nCol < nNumCol; nCol++)
			{
				if (SelectClosestAdjacentItemToSelection(nCol))
					return TRUE;
			}
		}
		break;

	case VK_ESCAPE:
		return (CancelOperation() != FALSE);

	case VK_DELETE:
		if (!m_pSelectedColumn->IsBacklog())
		{
			// For each of the selected tasks remove the attribute value(s) of the
			// selected list (column). Tasks having no values remaining are moved 
			// to the backlog
			CStringArray aListValues;
			VERIFY(m_pSelectedColumn->GetAttributeValues(aListValues));

			CDWordArray aTaskIDs;
			int nID = GetSelectedTaskIDs(aTaskIDs);

			while (nID--)
			{
				KANBANITEM* pKI = GetKanbanItem(aTaskIDs[nID]);
				ASSERT(pKI);

				if (pKI)
					pKI->RemoveTrackedAttributeValues(m_sTrackAttribID, aListValues);
			}

			// Notify parent of changes before altering the lists because we can't
			// guarantee that all the modified tasks will be in the same list afterwards
			NotifyParentAttibuteChange(aTaskIDs);

			// Reset selected list before removing items to 
			// to prevent unwanted selection notifications
			CKanbanColumnCtrl* pCol = m_pSelectedColumn;
			m_pSelectedColumn = NULL;

			nID = aTaskIDs.GetSize();

			while (nID--)
			{
				DWORD dwTaskID = aTaskIDs[nID];
				KANBANITEM* pKI = GetKanbanItem(dwTaskID);
				ASSERT(pKI);

				if (pKI)
				{
					VERIFY(pCol->DeleteTask(dwTaskID));

					if (!pKI->HasTrackedAttributeValues(m_sTrackAttribID))
					{
						CKanbanColumnCtrl* pBacklog = m_aColumns.GetBacklog();

						if (pBacklog)
							pBacklog->AddTask(*pKI);
					}
				}
			}

			// No need to rebuild column contents as
			// we've already done a manual adjustment
			RebuildColumns(FALSE, aTaskIDs);

			return TRUE;
		}
		break;
	}

	// all else
	return FALSE;
}

BOOL CKanbanCtrl::SelectTasks(const CDWordArray& aTaskIDs)
{
	CAutoFlag af(m_bSelectTasks, TRUE);

	// Check for 'no change'
	CDWordArray aSelTaskIDs;
	GetSelectedTaskIDs(aSelTaskIDs);

	int nPrevSel = GetSelColumnIndex(), nNewSel = -1;

	if (m_pSelectedColumn && m_pSelectedColumn->HasTasks(aTaskIDs))
		nNewSel = nPrevSel;
	else
		nNewSel = m_aColumns.Find(aTaskIDs);

	if ((nPrevSel != nNewSel) || !aTaskIDs.GetSize() || !Misc::MatchAll(aTaskIDs, aSelTaskIDs))
	{
		m_aColumns.SetSelectedColumn(NULL);

		if ((nNewSel == -1) || !aTaskIDs.GetSize())
		{
			m_pSelectedColumn = NULL;
			return FALSE;
		}

		// else
		SelectColumn(m_aColumns[nNewSel], FALSE);
		VERIFY(m_pSelectedColumn->SelectTasks(aTaskIDs));
	}

	ScrollToSelectedTask();
	return TRUE;
}

int CKanbanCtrl::GetSelectedCount() const
{
	if (m_pSelectedColumn)
		return m_pSelectedColumn->GetSelectedCount();

	return 0;
}

BOOL CKanbanCtrl::SelectAllInSelectedColumn()
{
	if (!GetSelectedCount())
		return FALSE;

	if (!m_pSelectedColumn->SelectAll())
		return FALSE;

	NotifyParentSelectionChange();
	return TRUE;
}

int CKanbanCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs) const
{
	const CKanbanColumnCtrl* pCol = GetSelColumn();

	if (pCol)
		return pCol->GetSelectedTaskIDs(aTaskIDs);

	// else
	aTaskIDs.RemoveAll();
	return 0;
}

CKanbanColumnCtrl* CKanbanCtrl::GetSelColumn()
{
	ASSERT((m_pSelectedColumn == NULL) || Misc::HasT(m_pSelectedColumn, m_aColumns));

	if (!m_pSelectedColumn && m_aColumns.GetSize())
	{
		m_pSelectedColumn = m_aColumns[0];
		m_pSelectedColumn->SetSelected(TRUE);
	}

	return m_pSelectedColumn;
}

const CKanbanColumnCtrl* CKanbanCtrl::GetSelColumn() const
{
	if (m_pSelectedColumn)
	{
		return m_pSelectedColumn;
	}
	else if (m_aColumns.GetSize())
	{
		return m_aColumns[0];
	}

	// else
	return NULL;
}

int CKanbanCtrl::GetSelColumnIndex() const
{
	return m_aColumns.Find(m_pSelectedColumn);
}

BOOL CKanbanCtrl::SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select)
{
	CKanbanColumnCtrl* pCol = NULL;
	HTREEITEM htiStart = NULL;
	BOOL bForwards = TRUE;

	switch (nCmd)
	{
	case IUI_SELECTFIRSTTASK:
		pCol = m_aColumns.GetFirstNonEmpty();

		if (pCol)
			htiStart = pCol->TCH().GetFirstItem();
		break;

	case IUI_SELECTNEXTTASK:
		pCol = m_pSelectedColumn;

		if (pCol)
			htiStart = pCol->GetLastSelectedItem();
		break;

	case IUI_SELECTNEXTTASKINCLCURRENT:
		pCol = m_pSelectedColumn;

		if (pCol)
			htiStart = pCol->GetPrevSiblingItem(pCol->GetFirstSelectedItem());
		break;

	case IUI_SELECTPREVTASK:
		pCol = m_pSelectedColumn;
		bForwards = FALSE;

		if (pCol)
			htiStart = pCol->GetFirstSelectedItem();
		break;

	case IUI_SELECTLASTTASK:
		pCol = m_aColumns.GetLastNonEmpty();
		bForwards = FALSE;

		if (pCol)
			htiStart = pCol->TCH().GetLastItem();
		break;

	default:
		ASSERT(0);
		break;
	}

	if (pCol)
	{
		const CKanbanColumnCtrl* pStartCol = pCol;
		HTREEITEM hti = htiStart;
		
		do
		{
			hti = pCol->FindItem(select, bForwards, hti);

			if (hti)
			{
				SelectColumn(pCol, FALSE);
				return pCol->SelectItem(hti, FALSE);
			}

			// else next/prev column
			pCol = GetNextColumn(pCol, bForwards, TRUE);
			hti = NULL;
		}
		while (pCol != pStartCol);
	}

	// all else
	return false;
}

BOOL CKanbanCtrl::HasFocus() const
{
	return CDialogHelper::IsChildOrSame(GetSafeHwnd(), ::GetFocus());
}

void CKanbanCtrl::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate)
{
	ASSERT(GetSafeHwnd());

	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return;
	}

	// always cancel any ongoing operation
	CancelOperation();

	BOOL bResort = FALSE;
	
	switch (nUpdate)
	{
	case IUI_ALL:
		RebuildData(pTasks);
		break;
		
	case IUI_NEW:
	case IUI_EDIT:
		UpdateData(pTasks);
		break;
		
	case IUI_DELETE:
		RemoveDeletedTasks(pTasks);
		break;
		
	default:
		ASSERT(0);
	}

	RefreshColumnHeaderText();
}

BOOL CKanbanCtrl::UpdateNeedsItemHeightRefresh(const ITASKLISTBASE* pTasks) const
{
	if (HasOption(KBCF_HIDEEMPTYATTRIBUTES))
	{
		int nAtt = m_aDisplayAttrib.GetSize();

		while (nAtt--)
		{
			if (pTasks->IsAttributeAvailable(m_aDisplayAttrib[nAtt]))
				return TRUE;
		}
	}

	return FALSE;
}

int CKanbanCtrl::GetTaskAllocTo(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aValues)
{
	aValues.RemoveAll();
	int nItem = pTasks->GetTaskAllocatedToCount(hTask);
	
	if (nItem > 0)
	{
		if (nItem == 1)
		{
			aValues.Add(pTasks->GetTaskAllocatedTo(hTask, 0));
		}
		else
		{
			while (nItem--)
				aValues.InsertAt(0, pTasks->GetTaskAllocatedTo(hTask, nItem));
		}
	}	
	
	return aValues.GetSize();
}

int CKanbanCtrl::GetTaskCategories(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aValues)
{
	aValues.RemoveAll();
	int nItem = pTasks->GetTaskCategoryCount(hTask);

	if (nItem > 0)
	{
		if (nItem == 1)
		{
			aValues.Add(pTasks->GetTaskCategory(hTask, 0));
		}
		else
		{
			while (nItem--)
				aValues.InsertAt(0, pTasks->GetTaskCategory(hTask, nItem));
		}
	}	

	return aValues.GetSize();
}

int CKanbanCtrl::GetTaskTags(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aValues)
{
	aValues.RemoveAll();
	int nItem = pTasks->GetTaskTagCount(hTask);

	if (nItem > 0)
	{
		if (nItem == 1)
		{
			aValues.Add(pTasks->GetTaskTag(hTask, 0));
		}
		else
		{
			while (nItem--)
				aValues.InsertAt(0, pTasks->GetTaskTag(hTask, nItem));
		}
	}	

	return aValues.GetSize();
}

// External interface
BOOL CKanbanCtrl::WantEditUpdate(TDC_ATTRIBUTE nAttrib) const
{
	switch (nAttrib)
	{
	case TDCA_ALLOCBY:
	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_COLOR:
	case TDCA_COST:
	case TDCA_CREATIONDATE:
	case TDCA_CREATEDBY:
	case TDCA_CUSTOMATTRIB:
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_EXTERNALID:
	case TDCA_FILELINK:
	case TDCA_FLAG:
	case TDCA_ICON:
	case TDCA_LASTMODDATE:
	case TDCA_PERCENT:
	case TDCA_PRIORITY:
	case TDCA_RECURRENCE:
	case TDCA_RISK:
	case TDCA_STARTDATE:
	case TDCA_STATUS:
	case TDCA_SUBTASKDONE:
	case TDCA_TAGS:
	case TDCA_TASKNAME:
	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
	case TDCA_VERSION:
		return TRUE;
	}
	
	// all else 
	return (nAttrib == IUI_ALL);
}

// External interface
BOOL CKanbanCtrl::WantSortUpdate(TDC_ATTRIBUTE nAttrib) const
{
	switch (nAttrib)
	{
	case TDCA_NONE:
	case TDCA_TASKNAME:
		return TRUE; // always
	}
	
	return (WantEditUpdate(nAttrib) && (Misc::FindT(nAttrib, m_aDisplayAttrib) != -1));
}

void CKanbanCtrl::RebuildData(const ITASKLISTBASE* pTasks)
{
	// Preserve pinned items
	if (!m_aPrevPinnedTasks.GetSize())
		m_data.GetPinnedItems(m_aPrevPinnedTasks);
	
	// Rebuild global attribute value lists
	m_mapAttributeValues.RemoveAll();
	m_aCustomAttribDefs.RemoveAll();

	UpdateGlobalAttributeValues(pTasks);

	// Rebuild data
	m_data.RemoveAll();

	VERIFY(AddTaskToData(pTasks, pTasks->GetFirstTask(), 0, TRUE));

	m_data.SetPinnedItems(m_aPrevPinnedTasks, FALSE);
	m_aPrevPinnedTasks.RemoveAll();

	// App will take care of restoring selection
	RebuildColumns(KCRC_REBUILDCONTENTS);
}

BOOL CKanbanCtrl::AddTaskToData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, DWORD dwParentID, BOOL bAndSiblings)
{
	if (!hTask)
		return FALSE;

	// Not interested in references
	if (!pTasks->IsTaskReference(hTask))
	{
		DWORD dwTaskID = pTasks->GetTaskID(hTask);

		KANBANITEM* pKI = m_data.NewItem(dwTaskID, pTasks->GetTaskTitle(hTask));
		ASSERT(pKI);
	
		if (!pKI)
			return FALSE;

		pKI->bDone = pTasks->IsTaskDone(hTask);
		pKI->bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);
		pKI->bParent = pTasks->IsTaskParent(hTask);
		pKI->dwParentID = dwParentID;
		pKI->bLocked = pTasks->IsTaskLocked(hTask, true);
		pKI->bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));
		pKI->bFlagged = (pTasks->IsTaskFlagged(hTask, false) ? TRUE : FALSE); // NOT calculated
		pKI->nPosition = pTasks->GetTaskPosition(hTask);
		pKI->sFullPosition = pTasks->GetTaskPositionString(hTask); // for 'Unsorting'

		pKI->SetColor(pTasks->GetTaskTextColor(hTask));

		LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
		pKI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));
	
		if (dwParentID)
		{
			const KANBANITEM* pKIParent = m_data.GetItem(dwParentID);
			ASSERT(pKIParent);

			if (pKIParent)
				pKI->nLevel = (pKIParent->nLevel + 1);
		}
		else
		{
			ASSERT(pKI->nLevel == 0);
		}

		// trackable attributes
		CStringArray aValues;

		if (GetTaskCategories(pTasks, hTask, aValues))
			pKI->SetTrackedAttributeValues(TDCA_CATEGORY, aValues);

		if (GetTaskAllocTo(pTasks, hTask, aValues))
			pKI->SetTrackedAttributeValues(TDCA_ALLOCTO, aValues);

		if (GetTaskTags(pTasks, hTask, aValues))
			pKI->SetTrackedAttributeValues(TDCA_TAGS, aValues);
	
		pKI->SetTrackedAttributeValue(TDCA_STATUS, pTasks->GetTaskStatus(hTask));
		pKI->SetTrackedAttributeValue(TDCA_ALLOCBY, pTasks->GetTaskAllocatedBy(hTask));
		pKI->SetTrackedAttributeValue(TDCA_VERSION, pTasks->GetTaskVersion(hTask));
		pKI->SetTrackedAttributeValue(TDCA_PRIORITY, pTasks->GetTaskPriority(hTask, true)); // calculated
		pKI->SetTrackedAttributeValue(TDCA_RISK, pTasks->GetTaskRisk(hTask, true)); // calculated

		// custom attributes
		int nCust = pTasks->GetCustomAttributeCount();

		while (nCust--)
		{
			CString sCustID(pTasks->GetCustomAttributeID(nCust));
			CString sCustValue(pTasks->GetTaskCustomAttributeData(hTask, sCustID, true));

			CStringArray aCustValues;
			Misc::Split(sCustValue, aCustValues);

			pKI->SetTrackedAttributeValues(sCustID, aCustValues);

			// Add to global attribute values
			CKanbanValueMap* pValues = m_mapAttributeValues.GetAddMapping(sCustID);
			ASSERT(pValues);
			
			pValues->AddValues(aCustValues);
		}

		// Other display-only attributes
		UpdateItemDisplayAttributes(pKI, pTasks, hTask);

		// first child
		AddTaskToData(pTasks, pTasks->GetFirstTask(hTask), dwTaskID, TRUE);
	}

	// Siblings NON-RECURSIVELY
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);

		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			AddTaskToData(pTasks, hSibling, dwParentID, FALSE);
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}

	return TRUE;
}

void CKanbanCtrl::UpdateData(const ITASKLISTBASE* pTasks)
{
	// update the task(s)
	BOOL bRebuild = UpdateGlobalAttributeValues(pTasks);
	bRebuild |= UpdateData(pTasks, pTasks->GetFirstTask(), TRUE);

	if (bRebuild)
	{
		// App will take care of restoring selection
		RebuildColumns(KCRC_REBUILDCONTENTS);
	}
	else
	{
		if (UpdateNeedsItemHeightRefresh(pTasks))
			m_aColumns.RefreshItemLineHeights();
		else
			m_aColumns.Redraw(FALSE);

		// If the update contains the tracked attribute
		// then assume some 'inline' changes were made to
		// one/some of the columns necessitating a resort
		if (pTasks->IsAttributeAvailable(m_nTrackAttribute))
			m_aColumns.Sort(m_nSortBy, m_bSortAscending);
	}
}

BOOL CKanbanCtrl::UpdateData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return FALSE;

	// Not interested in references
	if (pTasks->IsTaskReference(hTask))
		return FALSE; 

	// handle task if not NULL (== root)
	BOOL bRebuild = FALSE;
	DWORD dwTaskID = pTasks->GetTaskID(hTask);

	if (dwTaskID)
	{
		// Can be a new task
		if (!HasKanbanItem(dwTaskID))
		{
			bRebuild = AddTaskToData(pTasks, hTask, pTasks->GetTaskParentID(hTask), FALSE);
		}
		else
		{
			KANBANITEM* pKI = NULL;
			GET_KI_RET(dwTaskID, pKI, FALSE);

			if (pTasks->IsAttributeAvailable(TDCA_TASKNAME))
				pKI->sTitle = pTasks->GetTaskTitle(hTask);
			
			if (pTasks->IsAttributeAvailable(TDCA_DONEDATE))
			{
				// Note: Actual completion date is updated in UpdateItemDisplayAttributes
				BOOL bDone = pTasks->IsTaskDone(hTask);
				BOOL bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);

				if ((pKI->bDone != bDone) || (pKI->bGoodAsDone != bGoodAsDone))
				{
					pKI->bDone = bDone;
					pKI->bGoodAsDone = bGoodAsDone;

					if (HasOption(KBCF_DONEHAVELOWESTPRIORITYRISK) || HasOption(KBCF_DUEHAVEHIGHESTPRIORITYRISK))
						bRebuild = TRUE;
				}
			}

			if (pTasks->IsAttributeAvailable(TDCA_DUEDATE) && HasOption(KBCF_DUEHAVEHIGHESTPRIORITYRISK))
			{
				// Note: Actual due date is updated in UpdateItemDisplayAttributes
				BOOL bDue = pTasks->IsTaskDue(hTask);

				if (pKI->IsDue() != bDue)
					bRebuild = TRUE;
			}

			if (pTasks->IsAttributeAvailable(TDCA_SUBTASKDONE))
			{
				LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
				pKI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));
			}

			if (pTasks->IsAttributeAvailable(TDCA_ICON))
				pKI->bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

			if (pTasks->IsAttributeAvailable(TDCA_FLAG))
				pKI->bFlagged = (pTasks->IsTaskFlagged(hTask, false) ? TRUE : FALSE); // NOT calculated
			
			// Trackable attributes
			CStringArray aValues;

			if (pTasks->IsAttributeAvailable(TDCA_ALLOCTO))
			{
				GetTaskAllocTo(pTasks, hTask, aValues);
				bRebuild |= UpdateTrackableTaskAttribute(pKI, TDCA_ALLOCTO, aValues);
			}

			if (pTasks->IsAttributeAvailable(TDCA_CATEGORY))
			{
				GetTaskCategories(pTasks, hTask, aValues);
				bRebuild |= UpdateTrackableTaskAttribute(pKI, TDCA_CATEGORY, aValues);
			}

			if (pTasks->IsAttributeAvailable(TDCA_TAGS))
			{
				GetTaskTags(pTasks, hTask, aValues);
				bRebuild |= UpdateTrackableTaskAttribute(pKI, TDCA_TAGS, aValues);
			}

			if (pTasks->IsAttributeAvailable(TDCA_ALLOCBY))
				bRebuild |= UpdateTrackableTaskAttribute(pKI, TDCA_ALLOCBY, pTasks->GetTaskAllocatedBy(hTask));

			if (pTasks->IsAttributeAvailable(TDCA_STATUS))
				bRebuild |= UpdateTrackableTaskAttribute(pKI, TDCA_STATUS, pTasks->GetTaskStatus(hTask));

			if (pTasks->IsAttributeAvailable(TDCA_VERSION))
				bRebuild |= UpdateTrackableTaskAttribute(pKI, TDCA_VERSION, pTasks->GetTaskVersion(hTask));

			if (pTasks->IsAttributeAvailable(TDCA_PRIORITY))
				bRebuild |= UpdateTrackableTaskAttribute(pKI, TDCA_PRIORITY, pTasks->GetTaskPriority(hTask, true)); // calculated

			if (pTasks->IsAttributeAvailable(TDCA_RISK))
				bRebuild |= UpdateTrackableTaskAttribute(pKI, TDCA_RISK, pTasks->GetTaskRisk(hTask, true)); // calculated

			if (pTasks->IsAttributeAvailable(TDCA_CUSTOMATTRIB))
			{
				int nID = m_aCustomAttribDefs.GetSize();

				while (nID--)
				{
					const KANBANCUSTOMATTRIBDEF& def = m_aCustomAttribDefs[nID];
					CString sValue(pTasks->GetTaskCustomAttributeData(hTask, def.sAttribID, true));

					CStringArray aCustValues;
					Misc::Split(sValue, aCustValues);

					if (UpdateTrackableTaskAttribute(pKI, def.sAttribID, aCustValues))
					{
						// Add to global values
						CKanbanValueMap* pValues = m_mapAttributeValues.GetAddMapping(def.sAttribID);
						ASSERT(pValues);
						
						pValues->AddValues(aCustValues);
						bRebuild = TRUE;
					}
				}
			}

			// other display-only attributes
			UpdateItemDisplayAttributes(pKI, pTasks, hTask);
			
			// always update colour, lock state and parent-ness
			pKI->SetColor(pTasks->GetTaskTextColor(hTask));

			pKI->bLocked = pTasks->IsTaskLocked(hTask, true); // calculated
			pKI->bParent = pTasks->IsTaskParent(hTask);
		}
	}
		
	// children
	if (UpdateData(pTasks, pTasks->GetFirstTask(hTask), TRUE))
		bRebuild = TRUE;

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			if (UpdateData(pTasks, hSibling, FALSE))
				bRebuild = TRUE;
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
	
	return bRebuild;
}

void CKanbanCtrl::UpdateItemDisplayAttributes(KANBANITEM* pKI, const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	time64_t tDate = 0;
	
	if (pTasks->IsAttributeAvailable(TDCA_TIMEESTIMATE))
		pKI->dTimeEst = pTasks->GetTaskTimeEstimate(hTask, pKI->nTimeEstUnits, true); // calculated
	
	if (pTasks->IsAttributeAvailable(TDCA_TIMESPENT))
		pKI->dTimeSpent = pTasks->GetTaskTimeSpent(hTask, pKI->nTimeSpentUnits, true); // calculated
	
	if (pTasks->IsAttributeAvailable(TDCA_COST))
	{
		bool bIsRate = false;
		pKI->dCost = pTasks->GetTaskCost(hTask, true, bIsRate); // calculated

		if (bIsRate)
			pKI->dCost = 0.0;
	}
	
	if (pTasks->IsAttributeAvailable(TDCA_CREATEDBY))
		pKI->sCreatedBy = pTasks->GetTaskCreatedBy(hTask);
	
	if (pTasks->IsAttributeAvailable(TDCA_CREATIONDATE))
		pKI->dtCreate = pTasks->GetTaskCreationDate(hTask); // doesn't change
	
	if (pTasks->IsAttributeAvailable(TDCA_DONEDATE))
	{
		if (pTasks->GetTaskDoneDate64(hTask, tDate))
			pKI->dtDone = CDateHelper::GetDate(tDate);
		else
			CDateHelper::ClearDate(pKI->dtDone);
	}
	
	if (pTasks->IsAttributeAvailable(TDCA_DUEDATE))
	{
		if (pTasks->GetTaskDueDate64(hTask, true, tDate)) // calculated
			pKI->dtDue = CDateHelper::GetDate(tDate);
		else
			CDateHelper::ClearDate(pKI->dtDue);
	}
	
	if (pTasks->IsAttributeAvailable(TDCA_STARTDATE))
	{
		if (pTasks->GetTaskStartDate64(hTask, true, tDate)) // calculated
			pKI->dtStart = CDateHelper::GetDate(tDate);
		else
			CDateHelper::ClearDate(pKI->dtStart);
	}
		
	if (pTasks->IsAttributeAvailable(TDCA_LASTMODDATE))
	{
		if (pTasks->GetTaskLastModified64(hTask, tDate))
			pKI->dtLastMod = CDateHelper::GetDate(tDate);
		else
			CDateHelper::ClearDate(pKI->dtLastMod);
	}
	
	if (pTasks->IsAttributeAvailable(TDCA_PERCENT))
		pKI->nPercent = pTasks->GetTaskPercentDone(hTask, true); // calculated
	
	if (pTasks->IsAttributeAvailable(TDCA_EXTERNALID))
		pKI->sExternalID = pTasks->GetTaskExternalID(hTask);
	
	if (pTasks->IsAttributeAvailable(TDCA_RECURRENCE))
		pKI->sRecurrence = ((ITaskList*)pTasks)->GetTaskAttribute(hTask, TDL_TASKRECURRENCE);

	if (pTasks->IsAttributeAvailable(TDCA_FILELINK))
	{
		int nLink = pTasks->GetTaskFileLinkCount(hTask);
		pKI->aFileLinks.SetSize(nLink);

		while (nLink--)
			pKI->aFileLinks[nLink] = pTasks->GetTaskFileLink(hTask, nLink);
	}
}

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks)
{
	BOOL bChange = FALSE;

	bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_STATUS);
	bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_ALLOCTO);
	bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_CATEGORY);
	bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_ALLOCBY);
	bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_TAGS);
	bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_VERSION);
	bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_PRIORITY);
	bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_RISK);
	bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_CUSTOMATTRIB);
	
	return bChange;
}

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks, TDC_ATTRIBUTE nAttribute)
{
	if (!pTasks->IsAttributeAvailable(nAttribute))
		return FALSE;

	switch (nAttribute)
	{
	case TDCA_PRIORITY:
	case TDCA_RISK:
		{
			CString sAttribID(KANBANITEM::GetAttributeID(nAttribute));

			// create once only
			if (!m_mapAttributeValues.HasMapping(sAttribID))
			{
				CKanbanValueMap* pValues = m_mapAttributeValues.GetAddMapping(sAttribID);
				ASSERT(pValues);

				for (int nItem = 0; nItem <= 10; nItem++)
				{
					CString sValue(Misc::Format(nItem));
					pValues->SetAt(sValue, sValue);
				}
				
				// Add backlog item
				pValues->AddValue(EMPTY_STR);
			}
		}
		break;
		
	case TDCA_STATUS:
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_CATEGORY:
	case TDCA_VERSION:
	case TDCA_TAGS:	
		{
			CString sXMLTag(GetXMLTag(nAttribute)); 
			CString sAttribID(KANBANITEM::GetAttributeID(nAttribute));

			CStringArray aNewValues;
			int nValue = pTasks->GetAttributeCount(sXMLTag);

			while (nValue--)
			{
				CString sValue(pTasks->GetAttributeItem(sXMLTag, nValue));

				if (!sValue.IsEmpty())
					aNewValues.Add(sValue);
			}

			return UpdateGlobalAttributeValues(sAttribID, aNewValues);
		}
		break;
		
	case TDCA_CUSTOMATTRIB:
		{
			BOOL bChange = FALSE;
			int nCustom = pTasks->GetCustomAttributeCount();

			while (nCustom--)
			{
				// Save off each attribute ID
				if (pTasks->IsCustomAttributeEnabled(nCustom))
				{
					CString sAttribID(pTasks->GetCustomAttributeID(nCustom));
					CString sAttribName(pTasks->GetCustomAttributeLabel(nCustom));

					// Hack (values from tdcenum.h)
					DWORD dwListType = (pTasks->GetCustomAttributeType(nCustom) & 0xff00);
					BOOL bMultiList = ((dwListType == 0x0300) || (dwListType == 0x0400));
					
					int nDef = m_aCustomAttribDefs.AddDefinition(sAttribID, sAttribName, bMultiList);

					// Add 'default' values to the map
					CKanbanValueMap* pDefValues = m_mapGlobalAttributeValues.GetAddMapping(sAttribID);
					ASSERT(pDefValues);

					pDefValues->RemoveAll();

					CString sListData = pTasks->GetCustomAttributeListData(nCustom);

					// 'Auto' list values follow 'default' list values
					//  separated by a TAB
					CString sDefData(sListData), sAutoData;
					Misc::Split(sDefData, sAutoData, '\t');

					CStringArray aDefValues;
					
					if (Misc::Split(sDefData, aDefValues, '\n'))
						pDefValues->SetValues(aDefValues);

					CStringArray aAutoValues;
					Misc::Split(sAutoData, aAutoValues, '\n');

					bChange |= UpdateGlobalAttributeValues(sAttribID, aAutoValues);
				}
			}

			return bChange;
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	// all else
	return FALSE;
}

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(LPCTSTR szAttribID, const CStringArray& aValues)
{
	CKanbanValueMap mapNewValues;
	mapNewValues.AddValues(aValues);

	// Add in Backlog value
	mapNewValues.AddValue(EMPTY_STR);

	// Merge in default values
	const CKanbanValueMap* pDefValues = m_mapGlobalAttributeValues.GetMapping(szAttribID);

	if (pDefValues)
		Misc::Append(*pDefValues, mapNewValues);

	// Check for changes
	CKanbanValueMap* pValues = m_mapAttributeValues.GetAddMapping(szAttribID);
	ASSERT(pValues);
	
	if (!Misc::MatchAll(mapNewValues, *pValues))
	{
		Misc::Copy(mapNewValues, *pValues);

		return IsTracking(szAttribID);
	}

	// all else
	return FALSE;
}

int CKanbanCtrl::GetTaskTrackedAttributeValues(DWORD dwTaskID, CStringArray& aValues) const
{
	ASSERT(!m_sTrackAttribID.IsEmpty());

	const KANBANITEM* pKI = GetKanbanItem(dwTaskID);
	ASSERT(pKI);

	if (pKI)
		pKI->GetTrackedAttributeValues(m_sTrackAttribID, m_dwOptions, aValues);
	else
		aValues.RemoveAll();
	
	return aValues.GetSize();
}

int CKanbanCtrl::GetAttributeValues(TDC_ATTRIBUTE nAttrib, CStringArray& aValues) const
{
	CString sAttribID(KANBANITEM::GetAttributeID(nAttrib));

	const CKanbanValueMap* pValues = m_mapAttributeValues.GetMapping(sAttribID);
	aValues.SetSize(pValues->GetCount());

	if (pValues)
	{
		POSITION pos = pValues->GetStartPosition();
		int nItem = 0;

		while (pos)
			pValues->GetNextValue(pos, aValues[nItem++]);
	}

	return aValues.GetSize();
}

int CKanbanCtrl::GetAttributeValues(CKanbanAttributeValueMap& mapValues) const
{
	CString sAttribID;
	CKanbanValueMap* pValues = NULL;
	POSITION pos = m_mapAttributeValues.GetStartPosition();

	while (pos)
	{
		m_mapAttributeValues.GetNextAssoc(pos, sAttribID, pValues);
		ASSERT(pValues);

		CKanbanValueMap* pCopyValues = mapValues.GetAddMapping(sAttribID);
		ASSERT(pCopyValues);

		Misc::Copy(*pValues, *pCopyValues);
	}

	// Append default values
	pos = m_mapGlobalAttributeValues.GetStartPosition();

	while (pos)
	{
		m_mapGlobalAttributeValues.GetNextAssoc(pos, sAttribID, pValues);
		ASSERT(pValues);

		CKanbanValueMap* pCopyValues = mapValues.GetAddMapping(sAttribID);
		ASSERT(pCopyValues);

		Misc::Append(*pValues, *pCopyValues);
	}

	return mapValues.GetCount();
}

void CKanbanCtrl::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly)
{
	LoadDefaultAttributeListValues(pPrefs);

	if (!bAppOnly)
	{
		CString sPinned = pPrefs->GetProfileString(szKey, _T("PinnedTasks"));
		Misc::Split(sPinned, m_aPrevPinnedTasks, ',');
	}

	if (m_nTrackAttribute != TDCA_NONE)
	{
		// Both column visibility AND contents may have changed
		RebuildColumns(KCRC_REBUILDCONTENTS | KCRC_RESTORESELECTION);
	}
}

void CKanbanCtrl::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	CDWordArray aPinnedTasks;
	
	if (m_data.GetPinnedItems(aPinnedTasks))
		pPrefs->WriteProfileString(szKey, _T("PinnedTasks"), Misc::FormatArray(aPinnedTasks, ','));
	else
		pPrefs->DeleteProfileEntry(szKey, _T("PinnedTasks"));
}

void CKanbanCtrl::LoadDefaultAttributeListValues(const IPreferences* pPrefs)
{
	m_mapGlobalAttributeValues.RemoveAll();

	LoadDefaultAttributeListValues(pPrefs, _T("ALLOCTO"),	_T("AllocToList"));
	LoadDefaultAttributeListValues(pPrefs, _T("ALLOCBY"),	_T("AllocByList"));
	LoadDefaultAttributeListValues(pPrefs, _T("STATUS"),	_T("StatusList"));
	LoadDefaultAttributeListValues(pPrefs, _T("CATEGORY"),	_T("CategoryList"));
	LoadDefaultAttributeListValues(pPrefs, _T("VERSION"),	_T("VersionList"));
	LoadDefaultAttributeListValues(pPrefs, _T("TAGS"),		_T("TagList"));
}

void CKanbanCtrl::LoadDefaultAttributeListValues(const IPreferences* pPrefs, LPCTSTR szAttribID, LPCTSTR szSubKey)
{
	CKanbanValueMap* pMap = m_mapGlobalAttributeValues.GetAddMapping(szAttribID);
	ASSERT(pMap);

	CString sKey;
	sKey.Format(_T("Preferences\\%s"), szSubKey);

	int nCount = pPrefs->GetProfileInt(sKey, _T("ItemCount"), 0);
	
	// items
	for (int nItem = 0; nItem < nCount; nItem++)
	{
		CString sItemKey;
		sItemKey.Format(_T("Item%d"), nItem);

		CString sValue(pPrefs->GetProfileString(sKey, sItemKey));
		
		if (!sValue.IsEmpty())
			pMap->AddValue(sValue);
	}
}

CString CKanbanCtrl::GetXMLTag(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case TDCA_ALLOCTO:	return TDL_TASKALLOCTO;
	case TDCA_ALLOCBY:	return TDL_TASKALLOCBY;
	case TDCA_STATUS:	return TDL_TASKSTATUS;
	case TDCA_CATEGORY:	return TDL_TASKCATEGORY;
	case TDCA_VERSION:	return TDL_TASKVERSION;
	case TDCA_TAGS:		return TDL_TASKTAG;
		
	case TDCA_CUSTOMATTRIB:
		ASSERT(0);
		break;
		
	default:
		ASSERT(0);
		break;
	}
	
	return EMPTY_STR;
}

BOOL CKanbanCtrl::UpdateTrackableTaskAttribute(KANBANITEM* pKI, TDC_ATTRIBUTE nAttrib, int nNewValue)
{
	switch (nAttrib)
	{
	case TDCA_PRIORITY:
	case TDCA_RISK:
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	CString sNewValue;

	if (nNewValue >= 0)
		sNewValue = Misc::Format(nNewValue);

	// special handling for tasks which ought not to be moved
	if (pKI->HasDueOrDonePriorityOrRisk(m_dwOptions))
	{
		CStringArray aNewValues;
		aNewValues.Add(sNewValue);

		pKI->SetTrackedAttributeValues(KANBANITEM::GetAttributeID(nAttrib), aNewValues);
		return FALSE;
	}

	// all else
	return UpdateTrackableTaskAttribute(pKI, nAttrib, sNewValue);
}

BOOL CKanbanCtrl::IsTrackedAttributeMultiValue() const
{
	switch (m_nTrackAttribute)
	{
	case TDCA_PRIORITY:
	case TDCA_RISK:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_VERSION:
		return FALSE;

	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
		return TRUE;

	case TDCA_CUSTOMATTRIB:
		{
			int nDef = m_aCustomAttribDefs.FindDefinition(m_sTrackAttribID);
			
			if (nDef != -1)
				return m_aCustomAttribDefs[nDef].bMultiValue;
		}
		break;
	}

	// all else
	ASSERT(0);
	return FALSE;
}

BOOL CKanbanCtrl::UpdateTrackableTaskAttribute(KANBANITEM* pKI, TDC_ATTRIBUTE nAttrib, const CString& sNewValue)
{
	switch (nAttrib)
	{
	case TDCA_PRIORITY:
	case TDCA_RISK:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_VERSION:
		break;

	default:
		ASSERT(0);
		return FALSE;
	}
	
	CStringArray aNewValues;
	aNewValues.Add(sNewValue);

	return UpdateTrackableTaskAttribute(pKI, KANBANITEM::GetAttributeID(nAttrib), aNewValues);
}

BOOL CKanbanCtrl::UpdateTrackableTaskAttribute(KANBANITEM* pKI, TDC_ATTRIBUTE nAttrib, const CStringArray& aNewValues)
{
	switch (nAttrib)
	{
	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
		if (aNewValues.GetSize() == 0)
		{
			CStringArray aTemp;
			aTemp.Add(_T(""));

			return UpdateTrackableTaskAttribute(pKI, nAttrib, aTemp); // RECURSIVE CALL
		}
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	return UpdateTrackableTaskAttribute(pKI, KANBANITEM::GetAttributeID(nAttrib), aNewValues);
}

BOOL CKanbanCtrl::UpdateTrackableTaskAttribute(KANBANITEM* pKI, const CString& sAttribID, const CStringArray& aNewValues)
{
	// Check if we need to update listctrls or not
	if (!IsTracking(sAttribID) || 
		(pKI->bParent && HasOption(KBCF_HIDEPARENTTASKS)) ||
		(pKI->dwParentID && HasOption(KBCF_HIDESUBTASKS)))
	{
		pKI->SetTrackedAttributeValues(sAttribID, aNewValues);
		return FALSE; // no effect on list items
	}

	// else
	BOOL bRebuild = FALSE;
	
	if (!pKI->AttributeValuesMatch(sAttribID, aNewValues))
	{
		CStringArray aCurValues;
		pKI->GetTrackedAttributeValues(sAttribID, m_dwOptions, aCurValues);
		
		// Remove any list item whose current value is not found in the new values
		int nCurVal = aCurValues.GetSize();
		
		// Special case: Item needs removing from backlog
		if (nCurVal == 0)
		{
			aCurValues.Add(_T(""));
			nCurVal++;
		}

		while (nCurVal--)
		{
			if (!Misc::Contains(aCurValues[nCurVal], aNewValues, FALSE, TRUE))
			{
				CKanbanColumnCtrl* pCurCol = m_aColumns.Get(aCurValues[nCurVal]);
				ASSERT(pCurCol);

				if (pCurCol)
				{
					VERIFY(pCurCol->DeleteTask(pKI->dwTaskID));
					bRebuild |= (pCurCol->IsEmpty() && HasOption(KBCF_HIDEEMPTYCOLUMNS));
				}

				// Remove from list to speed up later searching
				aCurValues.RemoveAt(nCurVal);
			}
		}
		
		// Add any new items not in the current list
		int nNewVal = aNewValues.GetSize();

		// Special case: Item needs adding to backlog
		if (nNewVal == 0)
		{
			ASSERT(m_aColumns.Find(pKI->dwTaskID) == -1);

			CKanbanColumnCtrl* pBacklog = m_aColumns.GetBacklog();

			if (pBacklog)
				pBacklog->AddTask(*pKI);
			else
				bRebuild = TRUE; // Backlog needs reshowing
		}
		else
		{
			while (nNewVal--)
			{
				if (!Misc::Contains(aNewValues[nNewVal], aCurValues, FALSE, TRUE))
				{
					CKanbanColumnCtrl* pCurCol = m_aColumns.Get(aNewValues[nNewVal]);
				
					if (pCurCol)
						pCurCol->AddTask(*pKI);

					bRebuild |= ((pCurCol == NULL) || UsingFixedColumns()); // needs new list ctrl
				}
			}
		}
	
		// update values
		pKI->SetTrackedAttributeValues(sAttribID, aNewValues);
	}
	
	return bRebuild;
}

BOOL CKanbanCtrl::IsTracking(const CString& sAttribID) const
{
	return (m_sTrackAttribID.CompareNoCase(sAttribID) == 0);
}

BOOL CKanbanCtrl::WantShowColumn(LPCTSTR szValue, const CKanbanItemArrayMap& mapKIArray) const
{
	if (!HasOption(KBCF_HIDEEMPTYCOLUMNS))
		return TRUE;

	if (HasOption(KBCF_ALWAYSSHOWBACKLOG) && Misc::IsEmpty(szValue))
		return TRUE;

	// else
	const CKanbanItemArray* pKIArr = mapKIArray.GetMapping(szValue);
		
	return (pKIArr && pKIArr->GetSize());
}

BOOL CKanbanCtrl::WantShowColumn(const CKanbanColumnCtrl* pCol) const
{
	if (!HasOption(KBCF_HIDEEMPTYCOLUMNS))
		return TRUE;

	if (HasOption(KBCF_ALWAYSSHOWBACKLOG) && pCol->IsBacklog())
		return TRUE;

	return (pCol->GetCount() > 0);
}

BOOL CKanbanCtrl::DeleteColumn(int nCol)
{
	if ((nCol < 0) || (nCol >= m_aColumns.GetSize()))
	{
		ASSERT(0);
		return FALSE;
	}

	CKanbanColumnCtrl* pCol = m_aColumns[nCol];
	ASSERT(pCol);

	if (pCol == m_pSelectedColumn)
		m_pSelectedColumn = NULL;

	m_aColumns.RemoveAt(nCol);

	return TRUE;
}

BOOL CKanbanCtrl::HasNonParentTasks(const CKanbanItemArray* pItems)
{
	ASSERT(pItems);

	int nItem = pItems->GetSize();

	while (nItem--)
	{
		if (!pItems->GetAt(nItem)->bParent)
			return TRUE;
	}

	// else all parents
	return FALSE;
}

int CKanbanCtrl::RemoveOldDynamicColumns(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingDynamicColumns())
	{
		ASSERT(0);
		return 0;
	}

	// remove any lists whose values are no longer used
	// or which Optionally have no items 
	const CKanbanValueMap* pGlobals = m_mapAttributeValues.GetMapping(m_sTrackAttribID);
	int nCol = m_aColumns.GetSize(), nNumRemoved = 0;
	
	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = m_aColumns[nCol];
		ASSERT(pCol && !pCol->HasMultipleValues());

		BOOL bDeleteCol = FALSE;
		
		if ((pGlobals == NULL) || !WantShowColumn(pCol))
		{
			bDeleteCol = TRUE;
		}
		else
		{
			CString sAttribValueID(pCol->GetAttributeValueID());
			
			if (!WantShowColumn(sAttribValueID, mapKIArray))
			{
				bDeleteCol = TRUE;
			}
			else if (m_nTrackAttribute == TDCA_CUSTOMATTRIB)
			{
				// Custom neither have globals in the same sense
				// as default attributes nor do all the types of
				// custom attributes have globals at all.
				// So we can't use them to check for the validity
				// of an attribute's values as we do below
			}
			else if (!Misc::HasKey(*pGlobals, sAttribValueID))
			{
				bDeleteCol = TRUE;
			}
		}

		if (bDeleteCol)
		{
			DeleteColumn(nCol);
			nNumRemoved++;
		}
	}

	return nNumRemoved;
}

int CKanbanCtrl::AddMissingDynamicColumns(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingDynamicColumns())
	{
		ASSERT(0);
		return 0;
	}
	
	// Add any new status lists not yet existing
	const CKanbanValueMap* pGlobals = m_mapAttributeValues.GetMapping(m_sTrackAttribID);
	int nNumAdded = 0;

	if (pGlobals)
	{
		POSITION pos = pGlobals->GetStartPosition();
		
		while (pos)
		{
			CString sAttribValueID, sAttribValue;
			pGlobals->GetNextAssoc(pos, sAttribValueID, sAttribValue);
			
			CKanbanColumnCtrl* pCol = m_aColumns.Get(sAttribValueID);
			
			if ((pCol == NULL) && WantShowColumn(sAttribValueID, mapKIArray))
			{
				KANBANCOLUMN colDef;
				
				colDef.sAttribID = m_sTrackAttribID;
				colDef.sTitle = sAttribValue;
				colDef.aAttribValues.Add(sAttribValue);
				//colDef.crBackground = KBCOLORS[m_nNextColor++ % NUM_KBCOLORS];
				
				VERIFY (AddNewColumn(colDef) != NULL);
				nNumAdded++;
			}
		}

		ASSERT(HasOption(KBCF_HIDEEMPTYCOLUMNS) || 
				(m_nTrackAttribute == TDCA_CUSTOMATTRIB) ||
				(m_aColumns.GetSize() == pGlobals->GetCount()));
	}

	return nNumAdded;
}

void CKanbanCtrl::RebuildDynamicColumns(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingDynamicColumns())
	{
		ASSERT(0);
		return;
	}
	
	BOOL bNeedResize = RemoveOldDynamicColumns(mapKIArray);
	bNeedResize |= AddMissingDynamicColumns(mapKIArray);

	// If no columns created, create empty Backlog column
	bNeedResize |= CheckAddBacklogColumn();
	
	// (Re)sort
	m_aColumns.Sort();
}

void CKanbanCtrl::RebuildFixedColumns(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingFixedColumns())
	{
		ASSERT(0);
		return;
	}

	// We only need to build this once because we don't remove empty
	// columns like with dynamic columns, we just hide them
	if (m_aColumns.GetSize() == 0)
	{
		for (int nDef = 0; nDef < m_aColumnDefs.GetSize(); nDef++)
		{
			const KANBANCOLUMN& colDef = m_aColumnDefs[nDef];
			VERIFY(AddNewColumn(colDef) != NULL);
		}
	}
}

BOOL CKanbanCtrl::CheckAddBacklogColumn()
{
	if (m_aColumns.GetSize() == 0) 
	{
		KANBANCOLUMN colDef;
		
		colDef.sAttribID = m_sTrackAttribID;
		colDef.aAttribValues.Add(_T(""));
		
		VERIFY (AddNewColumn(colDef) != NULL);
		return TRUE;
	}

	return FALSE;
}

void CKanbanCtrl::RebuildColumns(DWORD dwFlags)
{
	if (m_sTrackAttribID.IsEmpty())
	{
		ASSERT(m_nTrackAttribute == TDCA_NONE);
		return;
	}

	CDWordArray aTaskIDs;

	if (dwFlags & KCRC_RESTORESELECTION)
		GetSelectedTaskIDs(aTaskIDs);

	RebuildColumns((dwFlags & KCRC_REBUILDCONTENTS), aTaskIDs);
}

void CKanbanCtrl::RebuildColumns(BOOL bRebuildContents, const CDWordArray& aSelTaskIDs)
{
	if (m_sTrackAttribID.IsEmpty())
	{
		ASSERT(m_nTrackAttribute == TDCA_NONE);
		return;
	}

	CHoldRedraw gr(*this, NCR_PAINT | NCR_ERASEBKGND);

	CKanbanItemArrayMap mapKIArray;
	m_data.BuildTempItemMaps(m_sTrackAttribID, m_dwOptions, mapKIArray);

	// Rebuild columns
	if (UsingDynamicColumns())
		RebuildDynamicColumns(mapKIArray);
	else
		RebuildFixedColumns(mapKIArray);

	// Rebuild column contents
	if (bRebuildContents)
	{
		RebuildColumnsContents(mapKIArray);
	}
	else if (UsingDynamicColumns())
	{
		// If not rebuilding the data (which will remove lists
		// which are empty as consequence of not showing parent
		// tasks) we made need to remove such lists.
		RemoveOldDynamicColumns(mapKIArray);
	}

	RebuildColumnHeader();
	Resize();

	// Restore selection
	if (aSelTaskIDs.GetSize())
	{
		if (m_aColumns.Find(aSelTaskIDs) != -1)
		{
			VERIFY(SelectTasks(aSelTaskIDs));
			return;
		}

		// If the selection can't be restored as-is probably
		// because previously visible tasks are now hidden
		// then we give the selected column the chance to do
		// the best it can
		if (m_pSelectedColumn)
		{
			ASSERT(m_aColumns.Find(m_pSelectedColumn) != -1);

			CDWordArray aFoundIDs;
			aFoundIDs.Copy(aSelTaskIDs);

			int nID = aFoundIDs.GetSize();

			while (nID--)
			{
				if (!m_pSelectedColumn->FindItem(aFoundIDs[nID]))
					aFoundIDs.RemoveAt(nID);
			}
		
			if (aFoundIDs.GetSize() > 0)
				m_pSelectedColumn->SelectTasks(aFoundIDs);
			else
				m_pSelectedColumn = NULL;
		}

		FixupSelectedColumn();
 		NotifyParentSelectionChange();
	}
}

void CKanbanCtrl::RebuildColumnHeader()
{
	int nNumVisColumns = GetVisibleColumnCount();

	if (!nNumVisColumns)
		return;

	// Remove excess columns
	while (nNumVisColumns < m_header.GetItemCount())
	{
		m_header.DeleteItem(0);
	}

	// Add new columns
	if (nNumVisColumns > m_header.GetItemCount())
	{
		// Give new columns the average width of old columns
		int nNewColWidth = 1;

		if (m_header.GetItemCount())
			nNewColWidth = m_header.CalcAverageItemWidth();

		while (nNumVisColumns > m_header.GetItemCount())
		{
			m_header.AppendItem(nNewColWidth);
		}
	}

	RefreshColumnHeaderText();
}

void CKanbanCtrl::RefreshColumnHeaderText()
{
	int nNumColumns = m_aColumns.GetSize(), nVis = 0;

	for (int nCol = 0; nCol < nNumColumns; nCol++)
	{
		const CKanbanColumnCtrl* pCol = m_aColumns[nCol];

		if (!WantShowColumn(pCol))
			continue;

		CEnString sTitle(pCol->ColumnDefinition().sTitle);

		if (sTitle.IsEmpty())
			sTitle.LoadString(IDS_BACKLOG);

		CString sFormat;
		sFormat.Format(_T("%s (%d)"), sTitle, pCol->GetCount());

		m_header.SetItemText(nVis, sFormat);
		m_header.SetItemData(nVis, (DWORD)pCol);
		m_header.EnableItemTracking(nVis, TRUE);

		nVis++;
	}
	ASSERT(nVis == m_header.GetItemCount());

	// Prevent tracking on the last visible column
	m_header.EnableItemTracking(nVis - 1, FALSE);
}

void CKanbanCtrl::RebuildColumnsContents(const CKanbanItemArrayMap& mapKIArray)
{
	int nCol = m_aColumns.GetSize();
	
	BOOL bHideParents = HasOption(KBCF_HIDEPARENTTASKS);
	BOOL bHideSubtasks = HasOption(KBCF_HIDESUBTASKS);

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = m_aColumns[nCol];
		ASSERT(pCol);
		
		RebuildColumnContents(pCol, mapKIArray, bHideParents, bHideSubtasks);
		
		// The list can still end up empty if parent tasks are 
		// omitted in Dynamic columns so we recheck and delete if required
		if (UsingDynamicColumns())
		{
			if ((bHideParents || bHideSubtasks) && !WantShowColumn(pCol))
			{
				DeleteColumn(nCol);
			}
		}
	}
		
	// Columns can still end up empty if there were 
	// only unwanted parents
	CheckAddBacklogColumn();

	// Always resort because the app will only tell us to resort 
	// if the modification that caused this update matches the
	// current sort, whereas we always need to be maintain some
	// kind of sorted state even if we are technically 'unsorted'
	m_aColumns.Sort(m_nSortBy, m_bSortAscending);
}

void CKanbanCtrl::FixupSelectedColumn()
{
	ASSERT(m_aColumns.GetSize());

	// Make sure selected list is valid
	if (!m_pSelectedColumn || !Misc::HasT(m_pSelectedColumn, m_aColumns))
	{
		// Find the first list with some items
		m_pSelectedColumn = m_aColumns.GetFirstNonEmpty();

		// No list has items?
		if (!m_pSelectedColumn)
			m_pSelectedColumn = m_aColumns[0];

		m_pSelectedColumn->SetSelected(TRUE);
	}

	FixupColumnFocus();
}

void CKanbanCtrl::FixupColumnFocus()
{
	const CWnd* pFocus = GetFocus();

	if (IsWindowVisible() && HasFocus() && m_pSelectedColumn && (pFocus != m_pSelectedColumn))
	{
		{
			CAutoFlag af(m_bSettingColumnFocus, TRUE);

			m_pSelectedColumn->SetFocus();
			m_pSelectedColumn->Invalidate(FALSE);
		}

		if (pFocus)
		{
			CKanbanColumnCtrl* pOtherCol = m_aColumns.Get(*pFocus);

			if (pOtherCol)
				pOtherCol->ClearSelection();
		}
	}
}

TDC_ATTRIBUTE CKanbanCtrl::GetTrackedAttribute(CString& sCustomAttrib) const
{
	if (m_nTrackAttribute == TDCA_CUSTOMATTRIB)
		sCustomAttrib = m_sTrackAttribID;
	else
		sCustomAttrib.Empty();

	return m_nTrackAttribute;
}

BOOL CKanbanCtrl::TrackAttribute(TDC_ATTRIBUTE nAttrib, const CString& sCustomAttribID, 
								 const CKanbanColumnArray& aColumnDefs)
{
	// validate input and check for changes
	BOOL bChange = (nAttrib != m_nTrackAttribute);

	switch (nAttrib)
	{
	case TDCA_STATUS:
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_CATEGORY:
	case TDCA_VERSION:
	case TDCA_PRIORITY:
	case TDCA_RISK:
	case TDCA_TAGS:
		break;
		
	case TDCA_CUSTOMATTRIB:
		if (sCustomAttribID.IsEmpty())
			return FALSE;

		if (!bChange)
			bChange = (sCustomAttribID != m_sTrackAttribID);
		break;

	default:
		return FALSE;
	}

	// Check if only display attributes have changed
	if (!bChange)
	{
		if (UsingFixedColumns())
		{
			if (m_aColumnDefs.MatchesAll(aColumnDefs))
			{
				return TRUE;
			}
			else if (m_aColumnDefs.MatchesAll(aColumnDefs, FALSE))
			{
				int nCol = aColumnDefs.GetSize();
				ASSERT(nCol == m_aColumns.GetSize());

				while (nCol--)
				{
					const KANBANCOLUMN& colDef = aColumnDefs[nCol];
					CKanbanColumnCtrl* pCol = m_aColumns[nCol];
					ASSERT(pCol);

					if (pCol)
					{
						pCol->SetBackgroundColor(colDef.crBackground);
						//pCol->SetExcessColor(colDef.crExcess);
						//pCol->SetMaximumTaskCount(colDef.nMaxTaskCount);

						m_aColumnDefs[nCol] = colDef;
					}
				}
	
				return TRUE;
			}
		}
		else if (!aColumnDefs.GetSize()) // not switching to fixed columns
		{
			return TRUE;
		}
	}

	m_aColumnDefs.Copy(aColumnDefs);

	// update state
	m_nTrackAttribute = nAttrib;

	switch (nAttrib)
	{
	case TDCA_STATUS:
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_CATEGORY:
	case TDCA_VERSION:
	case TDCA_PRIORITY:
	case TDCA_RISK:
	case TDCA_TAGS:
		m_sTrackAttribID = KANBANITEM::GetAttributeID(nAttrib);
		break;
		
	case TDCA_CUSTOMATTRIB:
		m_sTrackAttribID = sCustomAttribID;
		break;
	}

	// delete all lists and start over
	CHoldRedraw gr(*this, NCR_PAINT | NCR_ERASEBKGND);

	m_pSelectedColumn = NULL;
	m_aColumns.RemoveAll();

	// Don't attempt to restore selection
	RebuildColumns(KCRC_REBUILDCONTENTS);
	Resize();

	return TRUE;
}

CKanbanColumnCtrl* CKanbanCtrl::AddNewColumn(const KANBANCOLUMN& colDef)
{
	CKanbanColumnCtrl* pCol = new CKanbanColumnCtrl(m_data, colDef, m_fonts, m_aPriorityColors, m_aDisplayAttrib);
	ASSERT(pCol);

	if (pCol)
	{
		pCol->SetOptions(m_dwOptions);

		if (pCol->Create(IDC_COLUMNCTRL, this))
		{
			m_aColumns.Add(pCol);
		}
		else
		{
			delete pCol;
			pCol = NULL;
		}
	}
	
	return pCol;
}

BOOL CKanbanCtrl::RebuildColumnContents(CKanbanColumnCtrl* pCol, const CKanbanItemArrayMap& mapKIArray, 
										BOOL bHideParents, BOOL bHideSubtasks)
{
	ASSERT(pCol && pCol->GetSafeHwnd());

	if (!pCol || !pCol->GetSafeHwnd())
		return FALSE;

	CDWordArray aSelTaskIDs;
	pCol->GetSelectedTaskIDs(aSelTaskIDs);

	pCol->SetRedraw(FALSE);
	pCol->DeleteAll();

	CStringArray aValueIDs;
	int nNumVals = pCol->GetAttributeValueIDs(aValueIDs);

	for (int nVal = 0; nVal < nNumVals; nVal++)
	{
		const CKanbanItemArray* pKIArr = mapKIArray.GetMapping(aValueIDs[nVal]);
		
		if (pKIArr)
		{
			ASSERT(pKIArr->GetSize());
			
			for (int nKI = 0; nKI < pKIArr->GetSize(); nKI++)
			{
				const KANBANITEM* pKI = pKIArr->GetAt(nKI);
				ASSERT(pKI);
				
				if (pKI->bParent && bHideParents)
					continue;
				
				if (pKI->dwParentID && bHideSubtasks)
					continue;

				VERIFY(pCol->AddTask(*pKI) != NULL);
			}
		}
	}
	
	pCol->RefreshItemLineHeights();
	pCol->SetRedraw(TRUE);

	return TRUE;
}

void CKanbanCtrl::BuildTaskIDMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CDWordSet& mapIDs, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;
	
	mapIDs.Add(pTasks->GetTaskID(hTask));
	
	// children
	BuildTaskIDMap(pTasks, pTasks->GetFirstTask(hTask), mapIDs, TRUE);
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildTaskIDMap(pTasks, hSibling, mapIDs, FALSE);
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

void CKanbanCtrl::RemoveDeletedTasks(const ITASKLISTBASE* pTasks)
{
	CDWordSet mapIDs;
	BuildTaskIDMap(pTasks, pTasks->GetFirstTask(NULL), mapIDs, TRUE);

	BOOL bRebuildColumns = m_aColumns.RemoveDeletedTasks(mapIDs);
	m_data.RemoveDeletedItems(mapIDs);

	if (bRebuildColumns)
	{
		// No need to rebuild column contents as
		// we've already done a manual adjustment
		RebuildColumns(KCRC_RESTORESELECTION);
	}
}

KANBANITEM* CKanbanCtrl::GetKanbanItem(DWORD dwTaskID) const
{
	return m_data.GetItem(dwTaskID);
}

BOOL CKanbanCtrl::HasKanbanItem(DWORD dwTaskID) const
{
	return m_data.HasItem(dwTaskID);
}

CKanbanColumnCtrl* CKanbanCtrl::LocateTask(DWORD dwTaskID, HTREEITEM& hti, BOOL bForward) const
{
	// First try selected list
	if (m_pSelectedColumn)
	{
		hti = m_pSelectedColumn->FindItem(dwTaskID);

		if (hti)
			return m_pSelectedColumn;
	}

	// try any other list in the specified direction
	const CKanbanColumnCtrl* pCol = GetNextColumn(m_pSelectedColumn, bForward, TRUE);

	if (!pCol)
		return NULL;

	const CKanbanColumnCtrl* pStartCol = pCol;

	do
	{
		hti = pCol->FindItem(dwTaskID);

		if (hti)
			return const_cast<CKanbanColumnCtrl*>(pCol);

		// else
		pCol = GetNextColumn(pCol, bForward, TRUE);
	}
	while (pCol != pStartCol);

	return NULL;
}

void CKanbanCtrl::SetDisplayAttributes(const CKanbanAttributeArray& aAttrib)
{
	if (!Misc::MatchAllT(m_aDisplayAttrib, aAttrib, FALSE))
	{
		m_aDisplayAttrib.Copy(aAttrib);
		m_aColumns.OnDisplayAttributeChanged();

		// Update list attribute label visibility
		if (m_aDisplayAttrib.GetSize())
			Resize();
	}
}

void CKanbanCtrl::SetOptions(DWORD dwOptions)
{
	ASSERT(GetSafeHwnd());

	if (dwOptions != m_dwOptions)
	{
		DWORD dwPrevOptions = m_dwOptions;
		m_dwOptions = dwOptions;

		if (Misc::FlagHasChanged(KBCF_HIDEPARENTTASKS | KBCF_HIDESUBTASKS, m_dwOptions, dwPrevOptions))
		{
			// Column visibility AND contents may have changed
			RebuildColumns(KCRC_REBUILDCONTENTS | KCRC_RESTORESELECTION);
		}
		else if (Misc::FlagHasChanged(KBCF_HIDEEMPTYCOLUMNS | KBCF_ALWAYSSHOWBACKLOG, m_dwOptions, dwPrevOptions))
		{
			// Update column visibility only
			RebuildColumns(KCRC_RESTORESELECTION);
		}
		else if (((m_nTrackAttribute == TDCA_PRIORITY) || (m_nTrackAttribute == TDCA_RISK)) && Misc::FlagHasChanged(KBCF_DUEHAVEHIGHESTPRIORITYRISK | KBCF_DONEHAVELOWESTPRIORITYRISK, m_dwOptions, dwPrevOptions))
		{
			// Column visibility AND contents may have changed
			RebuildColumns(KCRC_REBUILDCONTENTS | KCRC_RESTORESELECTION);
		}

		// Column header sorting
		CDialogHelper::SetStyle(&m_header, HDS_BUTTONS, Misc::HasFlag(m_dwOptions, KBCF_COLUMNHEADERSORTING));

		// Column preferences
		dwOptions &= ~(KBCF_HIDEPARENTTASKS | KBCF_HIDESUBTASKS | KBCF_HIDEEMPTYCOLUMNS | KBCF_ALWAYSSHOWBACKLOG | KBCF_COLUMNHEADERSORTING);
		m_aColumns.SetOptions(dwOptions);

		if (Misc::FlagHasChanged(KBCF_SORTSUBTASTASKSBELOWPARENTS, m_dwOptions, dwPrevOptions))
		{
			m_aColumns.Sort(m_nSortBy, m_bSortAscending);
		}
		else if (((m_nSortBy == TDCA_PRIORITY) || (m_nSortBy == TDCA_RISK)) && Misc::FlagHasChanged(KBCF_DUEHAVEHIGHESTPRIORITYRISK | KBCF_DONEHAVELOWESTPRIORITYRISK, m_dwOptions, dwPrevOptions))
		{
			m_aColumns.Sort(m_nSortBy, m_bSortAscending);
		}

		if (HasOption(KBCF_DUEHAVEHIGHESTPRIORITYRISK))
			m_timerMidnight.Enable(*this);
		else
			m_timerMidnight.Disable();
	}
}

void CKanbanCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	Resize(cx, cy);
}

BOOL CKanbanCtrl::OnEraseBkgnd(CDC* pDC)
{
	if (m_aColumns.GetSize())
	{
		CDialogHelper::ExcludeChild(&m_header, pDC);

		// Clip out the list controls
		m_aColumns.Exclude(pDC);
		
		// fill the client with gray to create borders and dividers
		CRect rClient;
		GetClientRect(rClient);
		
		pDC->FillSolidRect(rClient, GetSysColor(COLOR_3DSHADOW));
	}
	
	return TRUE;
}

void CKanbanCtrl::OnSetFocus(CWnd* pOldWnd)
{
	// Don't handle focus changes during shutdown because these can 
	// cause a hang inside the treectrl whose exact cause I have 
	// yet to determine
	if (m_bClosing)
	{
		CWnd::OnSetFocus(pOldWnd);
	}
	else
	{
		if (m_pSelectedColumn)
			m_pSelectedColumn->SetFocus();
		else
			CWnd::OnSetFocus(pOldWnd);

		ScrollToSelectedTask();
	}
}

int CKanbanCtrl::GetVisibleColumnCount() const
{
	if (UsingDynamicColumns() || !HasOption(KBCF_HIDEEMPTYCOLUMNS))
		return m_aColumns.GetSize();

	// Fixed columns
	int nCol = m_aColumns.GetSize(), nNumVis = 0;

	while (nCol--)
	{
		const CKanbanColumnCtrl* pCol = m_aColumns[nCol];
		ASSERT(pCol);

		if (WantShowColumn(pCol))
			nNumVis++;
	}

	return nNumVis;
}

void CKanbanCtrl::Resize(int cx, int cy)
{
	int nNumVisibleCols = GetVisibleColumnCount();

	if (nNumVisibleCols)
	{
		CDeferWndMove dwm(nNumVisibleCols + 1);
		CRect rAvail(0, 0, cx, cy);

		if (rAvail.IsRectEmpty())
			GetClientRect(rAvail);

		// Create a border
		rAvail.DeflateRect(1, 1);

		ResizeHeader(dwm, rAvail);
		
		CRect rCol(rAvail);
		CWnd* pPrev = NULL;
		int nNumCols = m_aColumns.GetSize();

		for (int nCol = 0, nVis = 0; nCol < nNumCols; nCol++)
		{
			CKanbanColumnCtrl* pCol = m_aColumns[nCol];
			ASSERT(pCol && pCol->GetSafeHwnd());
			
			// If we find an empty column, it can only be with 
			// Fixed columns because we only hide columns rather
			// than delete them
			if (UsingFixedColumns())
			{
				if (!WantShowColumn(pCol))
				{
					pCol->ShowWindow(SW_HIDE);
					pCol->EnableWindow(FALSE);
					continue;
				}

				// else
				pCol->ShowWindow(SW_SHOW);
				pCol->EnableWindow(TRUE);
			}

			rCol.right = (rCol.left + m_header.GetItemWidth(nVis));
			dwm.MoveWindow(pCol, rCol.left, rCol.top, rCol.Width() - 1, rCol.Height(), TRUE);

			// Also update tab order as we go
			pCol->SetWindowPos(pPrev, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW | SWP_NOACTIVATE);

			// Check whether the lists are wide enough to show attribute labels
			KBC_ATTRIBLABELS nLabelVis = GetColumnAttributeLabelVisibility(nCol, rCol.Width());
			pCol->SetAttributeLabelVisibility(nLabelVis);

			pPrev = pCol;
			rCol.left = rCol.right;
			nVis++;
		}
	}
}

void CKanbanCtrl::ResizeHeader(CDeferWndMove& dwm, CRect& rAvail)
{
	if (rAvail.IsRectEmpty())
		return;

	CAutoFlag af(m_bResizingHeader, TRUE);

	ASSERT(m_header.GetSafeHwnd());

	int nNumCols = m_header.GetItemCount();
	ASSERT(nNumCols == GetVisibleColumnCount());

	CRect rNewHeader(rAvail);
	rAvail.top = rNewHeader.bottom = (rNewHeader.top + HEADER_HEIGHT);

	dwm.MoveWindow(&m_header, rNewHeader, TRUE);
		
	// -1 to compensate for the +1 to hide the last divider
	int nCurTotalWidth = (m_header.CalcTotalItemWidth() - 1);
	int nNewTotalWidth = rAvail.Width();

	BOOL bHasTrackedCols = m_header.HasTrackedItems();

	for (int nCol = 0, nColStart = 0; nCol < nNumCols; nCol++)
	{
		// Preserve tracking
		BOOL bTrackedCol = m_header.IsItemTracked(nCol);

		// Default equal width
		int nNewWidth = (nNewTotalWidth / nNumCols);

		if (nCol == (nNumCols - 1)) 
		{
			// last column takes up any slack
			nNewWidth = (rNewHeader.Width() - nColStart) + 1; // +1 hides the divider
		}
		else if (bHasTrackedCols)
		{
			// Preserve column proportions
			nNewWidth = MulDiv(m_header.GetItemWidth(nCol), nNewTotalWidth, nCurTotalWidth);
		}

		m_header.SetItemWidth(nCol, nNewWidth);
		m_header.SetItemTracked(nCol, bTrackedCol);

		nColStart += nNewWidth;
	}
}

float CKanbanCtrl::GetAverageColumnCharWidth()
{
	return m_aColumns.GetAverageCharWidth();
}

BOOL CKanbanCtrl::CanFitAttributeLabels(int nAvailWidth, float fAveCharWidth, KBC_ATTRIBLABELS nLabelVis) const
{
	switch (nLabelVis)
	{
	case KBCAL_NONE:
		return TRUE;

	case KBCAL_LONG:
	case KBCAL_SHORT:
		{
			int nAtt = m_aDisplayAttrib.GetSize();
			CUIntArray aLabelLen;

			aLabelLen.SetSize(nAtt);

			while (nAtt--)
			{
				TDC_ATTRIBUTE nAttribID = m_aDisplayAttrib[nAtt];
				CString sLabel = CKanbanColumnCtrl::FormatAttribute(nAttribID, _T(""), nLabelVis);

				aLabelLen[nAtt] = sLabel.GetLength();

				if ((int)(aLabelLen[nAtt] * fAveCharWidth) > nAvailWidth)
					return FALSE;
			}

			// Look for the first 'Label: Value' item which exceeds the list width
			POSITION pos = m_data.GetStartPosition();

			while (pos)
			{
				KANBANITEM* pKI = NULL;
				DWORD dwTaskID = 0;

				m_data.GetNextAssoc(pos, dwTaskID, pKI);

				nAtt = m_aDisplayAttrib.GetSize();

				while (nAtt--)
				{
					TDC_ATTRIBUTE nAttribID = m_aDisplayAttrib[nAtt];

					// Exclude 'File Link' and 'Parent' because these will 
					// almost always push things over the limit
					// Exclude 'flag' because that is rendered as an icon
					switch (nAttribID)
					{
					case TDCA_FILELINK:
					case TDCA_PARENT:
					case TDCA_FLAG:
						continue;
					}

					// else
					int nValueLen = pKI->GetAttributeDisplayValue(nAttribID).GetLength();

					if ((int)((aLabelLen[nAtt] + nValueLen) * fAveCharWidth) > nAvailWidth)
						return FALSE;
				}
			}

			// else
			return TRUE;
		}
		break;
	}

	// all else
	ASSERT(0);
	return FALSE;
}

KBC_ATTRIBLABELS CKanbanCtrl::GetColumnAttributeLabelVisibility(int nCol, int nColWidth)
{
	if (!m_aDisplayAttrib.GetSize() || !m_aColumns.GetSize())
		return KBCAL_NONE;

	// Calculate the available width for attributes
	int nAvailWidth = m_aColumns[nCol]->CalcAvailableAttributeWidth(nColWidth);

	// Calculate the fixed attribute label lengths and check if any
	// of them exceed the list width
	float fAveCharWidth = GetAverageColumnCharWidth();

	if (CanFitAttributeLabels(nAvailWidth, fAveCharWidth, KBCAL_LONG))
		return KBCAL_LONG;

//	if (CanFitAttributeLabels(nAvailWidth, fAveCharWidth, KBCAL_SHORT))
		return KBCAL_SHORT;

//	return KBCAL_NONE;
}

// Called externally only
void CKanbanCtrl::Sort(TDC_ATTRIBUTE nBy, BOOL bAscending)
{
	if (nBy == m_nTrackAttribute)
	{
		// if the sort attribute equals the track attribute then
		// tasks are already sorted into separate columns so we sort
		// by position instead but without changing the underlying 
		// sort state unless it's currently not set
		if (nBy != TDCA_NONE)
		{
			if (m_nSortBy == TDCA_NONE)
			{
				m_nSortBy = nBy;
				m_bSortAscending = bAscending;
			}

			m_aColumns.Sort(TDCA_POSITION, bAscending);
		}
	}
	else 
	{
		m_nSortBy = nBy;
		m_bSortAscending = bAscending;

		m_aColumns.Sort(m_nSortBy, m_bSortAscending);
	}

	m_header.Invalidate(FALSE);
}

void CKanbanCtrl::SetReadOnly(bool bReadOnly) 
{ 
	m_bReadOnly = bReadOnly; 
}

BOOL CKanbanCtrl::GetLabelEditRect(LPRECT pEdit)
{
	if (!m_pSelectedColumn)
	{
		ASSERT(0);
		return FALSE;
	}
	else if (!m_pSelectedColumn->GetLabelEditRect(pEdit))
	{
		ASSERT(m_pSelectedColumn->GetSelectedCount() > 1);
		return FALSE;
	}

	// else convert from list to 'our' coords
	m_pSelectedColumn->ClientToScreen(pEdit);
	ScreenToClient(pEdit);

	return TRUE;
}

void CKanbanCtrl::SetPriorityColors(const CDWordArray& aColors)
{
	if (!Misc::MatchAll(m_aPriorityColors, aColors))
	{
		m_aPriorityColors.Copy(aColors);

		// Redraw the lists if coloring by priority
		if (GetSafeHwnd() && HasOption(KBCF_COLORBARBYPRIORITY))
			m_aColumns.Redraw(FALSE);
	}
}

void CKanbanCtrl::ScrollToSelectedTask()
{
	CKanbanColumnCtrl* pCol = GetSelColumn();

	if (pCol)
		pCol->ScrollToSelection();
}

bool CKanbanCtrl::PrepareNewTask(ITaskList* pTask) const
{
	ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTask, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return false;
	}

	HTASKITEM hNewTask = pTasks->GetFirstTask();
	ASSERT(hNewTask);

	const CKanbanColumnCtrl* pCol = GetSelColumn();
	CString sValue;

	CRect rColCtrl;
	pCol->GetWindowRect(rColCtrl);

	if (!GetColumnAttributeValue(pCol, rColCtrl.CenterPoint(), sValue))
		return false;

	switch (m_nTrackAttribute)
	{
	case TDCA_STATUS:
		pTasks->SetTaskStatus(hNewTask, sValue);
		break;

	case TDCA_ALLOCTO:
		pTasks->AddTaskAllocatedTo(hNewTask, sValue);
		break;

	case TDCA_ALLOCBY:
		pTasks->SetTaskAllocatedBy(hNewTask, sValue);
		break;

	case TDCA_CATEGORY:
		pTasks->AddTaskCategory(hNewTask, sValue);
		break;

	case TDCA_PRIORITY:
		pTasks->SetTaskPriority(hNewTask, _ttoi(sValue));
		break;

	case TDCA_RISK:
		pTasks->SetTaskRisk(hNewTask, _ttoi(sValue));
		break;

	case TDCA_VERSION:
		pTasks->SetTaskVersion(hNewTask, sValue);
		break;

	case TDCA_TAGS:
		pTasks->AddTaskTag(hNewTask, sValue);
		break;

	case TDCA_CUSTOMATTRIB:
		ASSERT(!m_sTrackAttribID.IsEmpty());
		pTasks->SetTaskCustomAttributeData(hNewTask, m_sTrackAttribID, sValue);
		break;
	}

	return true;
}

DWORD CKanbanCtrl::HitTestTask(const CPoint& ptScreen) const
{
	return m_aColumns.HitTestTask(ptScreen);
}

int CKanbanCtrl::HitTestColumn(const CPoint& ptScreen, BOOL& bHeader) const
{
	CKanbanColumnCtrl* pCol = m_aColumns.HitTest(ptScreen);
	int nCol = m_aColumns.Find(pCol);

	if (nCol != -1)
	{
		bHeader = FALSE;
	}
	else
	{
		CPoint ptHeader(ptScreen);
		m_header.ScreenToClient(&ptHeader);

		nCol = m_header.HitTest(ptHeader);
		bHeader = (nCol != -1);
	}
	
	return nCol;
}

DWORD CKanbanCtrl::GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const
{
	BOOL bForward = ((nCmd == IUI_GETPREVVISIBLETASK) || (nCmd == IUI_GETPREVTOPLEVELTASK));

	HTREEITEM hti = NULL;
	const CKanbanColumnCtrl* pCol = LocateTask(dwTaskID, hti, bForward);
	
	if (!pCol || (UsingFixedColumns() && !pCol->IsWindowVisible()))
	{
		return 0L;
	}
	else if (hti == NULL)
	{
		ASSERT(0);
		return 0L;
	}

	switch (nCmd)
	{
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTVISIBLETASK:
		hti = pCol->GetNextSiblingItem(hti);
		break;

	case IUI_GETPREVTASK:
	case IUI_GETPREVVISIBLETASK:
		hti = pCol->GetPrevSiblingItem(hti);
		break;

	case IUI_GETNEXTTOPLEVELTASK:
		pCol = GetNextColumn(pCol, TRUE, TRUE);
			
		if (pCol)
			hti = pCol->TCH().GetFirstItem();
		break;

	case IUI_GETPREVTOPLEVELTASK:
		pCol = GetNextColumn(pCol, FALSE, TRUE);
			
		if (pCol)
			hti = pCol->TCH().GetFirstItem();
		break;

	default:
		ASSERT(0);
	}

	return (hti ? pCol->GetTaskID(hti) : 0);
}

const CKanbanColumnCtrl* CKanbanCtrl::GetNextColumn(const CKanbanColumnCtrl* pCol, BOOL bNext, BOOL bExcludeEmpty) const
{
	return m_aColumns.GetNext(pCol, bNext, bExcludeEmpty, UsingFixedColumns());
}

CKanbanColumnCtrl* CKanbanCtrl::GetNextColumn(const CKanbanColumnCtrl* pCol, BOOL bNext, BOOL bExcludeEmpty)
{
	return m_aColumns.GetNext(pCol, bNext, bExcludeEmpty, UsingFixedColumns());
}

BOOL CKanbanCtrl::IsDragging() const
{
	return (!m_bReadOnly && (::GetCapture() == *this));
}

BOOL CKanbanCtrl::NotifyParentAttibuteChange(const CDWordArray& aTaskIDs)
{
	ASSERT(!m_bReadOnly);
	ASSERT(aTaskIDs.GetSize());

	return GetParent()->SendMessage(WM_KBC_VALUECHANGE, aTaskIDs.GetSize(), (LPARAM)aTaskIDs.GetData());
}

void CKanbanCtrl::NotifyParentSelectionChange()
{
	ASSERT(!m_bSelectTasks);

	GetParent()->SendMessage(WM_KBC_SELECTIONCHANGE);
}

// external version
BOOL CKanbanCtrl::CancelOperation()
{
	if (IsDragging())
	{
		ReleaseCapture();
		return TRUE;
	}
	
	// else 
	return FALSE;
}

BOOL CKanbanCtrl::SelectColumn(CKanbanColumnCtrl* pCol, BOOL bNotifyParent)
{
	if (pCol)
	{
		if (pCol == m_pSelectedColumn)
		{
			// Make sure header is refreshed
			m_aColumns.SetSelectedColumn(m_pSelectedColumn);
			return TRUE;
		}

		CKanbanColumnCtrl* pPrevSelCol = m_pSelectedColumn;
		m_pSelectedColumn = pCol;

		FixupColumnFocus();

		if (pCol->GetCount() > 0)
		{
			m_aColumns.SetSelectedColumn(m_pSelectedColumn);
			m_pSelectedColumn->ScrollToSelection();

			if (bNotifyParent)
				NotifyParentSelectionChange();
		}
		else
		{
			if (pPrevSelCol)
				pPrevSelCol->SetSelected(FALSE);

			m_pSelectedColumn->SetSelected(TRUE);
		}

		m_header.Invalidate(TRUE);

		return TRUE;
	}

	return FALSE;
}

BOOL CKanbanCtrl::IsSelectedColumn(HWND hWnd) const
{
	return (m_pSelectedColumn && (m_pSelectedColumn->GetSafeHwnd() == hWnd));
}

void CKanbanCtrl::OnColumnItemSelChange(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	// only interested in selection changes caused by the user
	LPNMTREEVIEW pNMTV = (LPNMTREEVIEW)pNMHDR;
	UINT nAction = pNMTV->action;

	if (!m_bSettingColumnFocus && !m_bSelectTasks && !IsDragging() &&
		(nAction & (TVC_BYMOUSE | TVC_BYKEYBOARD)))
	{
		CKanbanColumnCtrl* pCol = m_aColumns.Get(pNMHDR->hwndFrom);

		if (pCol && (pCol != m_pSelectedColumn))
		{
			CAutoFlag af(m_bSelectTasks, TRUE);
			SelectColumn(pCol, FALSE);
		}

		NotifyParentSelectionChange();
	}
#ifdef _DEBUG
	else
	{
		int breakpoint = 0;
	}
#endif
}

LRESULT CKanbanCtrl::OnColumnEditLabel(WPARAM wp, LPARAM lp)
{
	// Sanity checks
	if (!m_pSelectedColumn)
	{
		ASSERT(0);
	}
	else if (m_pSelectedColumn->GetSafeHwnd() != (HWND)wp)
	{
		ASSERT(0);
	}
	else if (m_pSelectedColumn->GetOnlySelectedTask() != (DWORD)lp)
	{
		ASSERT(0);
	}
	else
	{
		GetParent()->SendMessage(WM_KBC_EDITTASKTITLE, lp);
	}

	return 0L;
}

LRESULT CKanbanCtrl::OnColumnShowFileLink(WPARAM wp, LPARAM lp)
{
	ASSERT(lp);
	ASSERT(m_aColumns.Find((HWND)wp) != -1);

	GetParent()->SendMessage(WM_KBC_SHOWFILELINK, wp, lp);

	return 0L;
}

void CKanbanCtrl::OnHeaderDividerDoubleClick(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	NMHEADER* pHDN = (NMHEADER*)pNMHDR;

	// resize just the adjacent columns
	int nCol = ((NMHEADER*)pNMHDR)->iItem, nNumCols = m_header.GetItemCount();

	if (nCol < (nNumCols - 2))
	{
		int nTotalWidth = (m_header.GetItemWidth(nCol) + m_header.GetItemWidth(nCol + 1));

		m_header.SetItemWidth(nCol, (nTotalWidth / 2));
		m_header.SetItemWidth(nCol + 1, (nTotalWidth - (nTotalWidth / 2)));
	}
}

int CKanbanCtrl::MapHeaderItemToColumn(int nItem) const
{
	// When using dynamic columns the header items
	// exactly match the columns
	if (UsingDynamicColumns() || !HasOption(KBCF_HIDEEMPTYCOLUMNS))
		return nItem;

	// When using fixed columns however, hidden columns 
	// are not removed just set to zero width whilst
	// their corresponding header items are removed
	int nCol = 0, nColItem = 0;

	for (; nCol < m_aColumns.GetSize(); nCol++)
	{
		const CKanbanColumnCtrl* pCol = m_aColumns[nCol];

		if (WantShowColumn(pCol))
		{
			if (nColItem == nItem)
				break;
			else 
				nColItem++;
		}
	}

	return nCol;
}

void CKanbanCtrl::OnHeaderItemChanging(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	if (m_bResizingHeader || m_bSavingToImage)
		return;

	NMHEADER* pHDN = (NMHEADER*)pNMHDR;

	if ((pHDN->iButton == 0) && (pHDN->pitem->mask & HDI_WIDTH))
	{
		ASSERT(pHDN->iItem < (m_header.GetItemCount() - 1));

		// prevent 'this' or 'next' columns becoming too small
		int nThisWidth = m_header.GetItemWidth(pHDN->iItem);
		int nNextWidth = m_header.GetItemWidth(pHDN->iItem + 1);

		pHDN->pitem->cxy = max(MIN_COL_WIDTH, pHDN->pitem->cxy);
		pHDN->pitem->cxy = min(pHDN->pitem->cxy, (nThisWidth + nNextWidth - MIN_COL_WIDTH));
		
		// Resize 'next' column
		nNextWidth = (nThisWidth + nNextWidth - pHDN->pitem->cxy);

		CAutoFlag af(m_bResizingHeader, TRUE);
		m_header.SetItemWidth(pHDN->iItem + 1, nNextWidth);
		
		// Resize corresponding listctrls
		int nThisCol = MapHeaderItemToColumn(pHDN->iItem);
		int nNextCol = MapHeaderItemToColumn(pHDN->iItem + 1);

		CKanbanColumnCtrl* pThisCol = m_aColumns[nThisCol];
		CKanbanColumnCtrl* pNextCol = m_aColumns[nNextCol];

		CRect rThisCol = CDialogHelper::GetChildRect(pThisCol);
		rThisCol.right = (rThisCol.left + pHDN->pitem->cxy - 1);

		CRect rNextCol = CDialogHelper::GetChildRect(pNextCol);
		rNextCol.left = (rThisCol.right + 1);

		pThisCol->MoveWindow(rThisCol, TRUE);
		pNextCol->MoveWindow(rNextCol, TRUE);

		// Redraw the vertical divider
		CRect rDivider(rThisCol);
		rDivider.left = rDivider.right;
		rDivider.right++;

		Invalidate(TRUE);
		UpdateWindow();
	}
}

void CKanbanCtrl::OnHeaderClick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;

	// If the sort column is not set then set it to 'title'
	// else flip the sort direction
	if (m_nSortBy == TDCA_NONE)
	{
		m_nSortBy = TDCA_TASKNAME;
		m_bSortAscending = TRUE;
	}
	else
	{
		m_bSortAscending = !m_bSortAscending;
	}

	m_aColumns.Sort(m_nSortBy, m_bSortAscending);
	m_header.Invalidate(FALSE);

	GetParent()->SendMessage(WM_KBC_SORTCHANGE, m_bSortAscending, m_nSortBy);
}

void CKanbanCtrl::OnHeaderCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pNMCD = (NMCUSTOMDRAW*)pNMHDR;
	*pResult = CDRF_DODEFAULT;

	HWND hwndHdr = pNMCD->hdr.hwndFrom;
	ASSERT(hwndHdr == m_header);
	
	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		// Handle RTL text column headers and selected column
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
		
	case CDDS_ITEMPREPAINT:
		if (GraphicsMisc::GetRTLDrawTextFlags(hwndHdr) == DT_RTLREADING)
		{
			*pResult = CDRF_NOTIFYPOSTPAINT;
		}
		else
		{
			if (!m_bSavingToImage && m_pSelectedColumn)
			{
				// Show the text of the selected column in bold
				if (pNMCD->lItemlParam == (LPARAM)m_pSelectedColumn)
					::SelectObject(pNMCD->hdc, m_fonts.GetHFont(GMFS_BOLD));
				else
					::SelectObject(pNMCD->hdc, m_fonts.GetHFont());
			
				*pResult = CDRF_NEWFONT;
			}

			if (m_nSortBy != TDCA_NONE)
				*pResult |= CDRF_NOTIFYPOSTPAINT;
		}
		break;
		
	case CDDS_ITEMPOSTPAINT:
		{
			CDC* pDC = CDC::FromHandle(pNMCD->hdc);

			if (GraphicsMisc::GetRTLDrawTextFlags(hwndHdr) == DT_RTLREADING)
			{
				CRect rItem(pNMCD->rc);
				rItem.DeflateRect(3, 0);

				pDC->SetBkMode(TRANSPARENT);

				// Show the text of the selected column in bold
				HGDIOBJ hPrev = NULL;

				if (!m_bSavingToImage)
				{
					if (pNMCD->lItemlParam == (LPARAM)m_pSelectedColumn)
						hPrev = pDC->SelectObject(m_fonts.GetHFont(GMFS_BOLD));
					else
						hPrev = pDC->SelectObject(m_fonts.GetHFont());
				}
			
				UINT nFlags = (DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(hwndHdr));
				pDC->DrawText(m_header.GetItemText(pNMCD->dwItemSpec), rItem, nFlags);

				if (!m_bSavingToImage)
					pDC->SelectObject(hPrev);
			
				*pResult = CDRF_SKIPDEFAULT;
			}

			if ((m_nSortBy != TDCA_NONE) && (m_nSortBy != m_nTrackAttribute))
				m_header.DrawItemSortArrow(pDC, (int)pNMCD->dwItemSpec, m_bSortAscending);
		}
		break;
	}
}

void CKanbanCtrl::OnBeginDragColumnItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	ReleaseCapture();

	if (!m_bReadOnly && !IsDragging() && (Misc::ModKeysArePressed(0) || Misc::ModKeysArePressed(MKS_CTRL)))
	{
		ASSERT(pNMHDR->idFrom == IDC_COLUMNCTRL);

		if (Misc::IsKeyPressed(VK_LBUTTON))
		{
			NMTREEVIEW* pNMTV = (NMTREEVIEW*)pNMHDR;
			ASSERT(pNMTV->itemNew.hItem);
			ASSERT(pNMTV->itemNew.lParam);

			CKanbanColumnCtrl* pCol = (CKanbanColumnCtrl*)CWnd::FromHandle(pNMHDR->hwndFrom);
			ASSERT(pCol == m_pSelectedColumn);
			ASSERT(pCol->IsTaskSelected(pNMTV->itemNew.lParam));

			if (!SelectionHasNonDraggableTasks(pCol))
			{
				TRACE(_T("CKanbanCtrl::OnBeginDragColItem(start drag)\n"));

				SetCapture();

				// NOTE: We leave the creation of the drag image to the
				// OnMouseMove handler once some distance has been moved
				m_ptDragStart = ::GetMessagePos();
			}
		}
		else
		{
			// Mouse button already released
			TRACE(_T("CKanbanCtrl::OnBeginDragColItem(cancel drag)\n"));
		}
	}
	
	*pResult = 0;
}

BOOL CKanbanCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint ptCursor(GetMessagePos());
	DWORD dwTaskID = HitTestTask(ptCursor);

	if (dwTaskID)
	{
		if (!m_data.IsLocked(dwTaskID) && !CanDragTask(dwTaskID))
			return GraphicsMisc::SetAppCursor(_T("NoDrag"), _T("Resources\\Cursors"));
	}

	// else
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CKanbanCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	TRACE(_T("CKanbanCtrl::OnLButtonUp()\n"));

	if (IsDragging())
	{
		TRACE(_T("CKanbanCtrl::OnLButtonUp(end drag)\n"));

		// get the list under the mouse
		ClientToScreen(&point);

		CKanbanColumnCtrl* pDestCol = m_aColumns.HitTest(point);
		CKanbanColumnCtrl* pSrcCol = m_pSelectedColumn;

		if (CanEndDrag(pSrcCol, pDestCol))
		{
			CString sDestAttribValue;
			
			if (GetColumnAttributeValue(pDestCol, point, sDestAttribValue))
			{
				CDWordArray aTaskIDs;
				int nNumIDs = pSrcCol->GetSelectedTaskIDs(aTaskIDs);

				// Cache current values in case of undo
				CArray<KANBANITEM, KANBANITEM&> aUndo;
				aUndo.SetSize(aTaskIDs.GetSize());

				for (int nID = 0; nID < nNumIDs; nID++)
					aUndo[nID] = *GetKanbanItem(aTaskIDs[nID]);

				int nPrevNumCols = GetVisibleColumnCount();

				BOOL bChange = EndDragItems(pSrcCol, aTaskIDs, pDestCol, sDestAttribValue);

				if (bChange)
				{
					if (NotifyParentAttibuteChange(aTaskIDs))
					{
						HideEmptyColumns(nPrevNumCols);
						RefreshColumnHeaderText();
						Resize();

						// Resort before fixing up selection
						if ((m_nSortBy != TDCA_NONE) || HasOption(KBCF_SORTSUBTASTASKSBELOWPARENTS))
						{	
							pDestCol->Sort(m_nSortBy, m_bSortAscending);
							pSrcCol->Sort(m_nSortBy, m_bSortAscending);
						}

						SelectColumn(pDestCol, FALSE);
					}
					else
					{
						ASSERT(aUndo.GetSize());

						// Application failed the request so we undo the changes
						for (int nID = 0; nID < nNumIDs; nID++)
							*GetKanbanItem(aTaskIDs[nID]) = aUndo[nID];

						// App will take care of restoring selection
						RebuildColumns(KCRC_REBUILDCONTENTS);

						SelectColumn(pSrcCol, FALSE);
					}

					SelectTasks(aTaskIDs);
					NotifyParentSelectionChange();
				}
			}
		}

		// always
		ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CKanbanCtrl::HideEmptyColumns(int nPrevColCount)
{
	if (HasOption(KBCF_HIDEEMPTYCOLUMNS))
	{
		int nCol = m_aColumns.GetSize(), nVisColCount = nCol;

		while (nCol--)
		{
			CKanbanColumnCtrl* pCol = m_aColumns[nCol];

			if (!WantShowColumn(pCol))
			{
				if (pCol == m_pSelectedColumn)
					m_pSelectedColumn = NULL;

				if (UsingDynamicColumns())
				{
					m_aColumns.RemoveAt(nCol);
				}
				else // fixed
				{
					pCol->EnableWindow(FALSE);
					pCol->ShowWindow(SW_HIDE);
				}

				nVisColCount--;
			}
		}

		if (nVisColCount != nPrevColCount)
			RebuildColumnHeader();
	}
}

void CKanbanCtrl::OnCaptureChanged(CWnd* pWnd)
{
	CWnd::OnCaptureChanged(pWnd);

	m_aColumns.SetDropTarget(NULL);

	m_ilDrag.EndDrag();
	m_ilDrag.DeleteImageList();
}

BOOL CKanbanCtrl::CanDragTask(DWORD dwTaskID) const
{
	const KANBANITEM* pKI = NULL;
	GET_KI_RET(dwTaskID, pKI, FALSE);

	if (pKI->bLocked)
		return FALSE;

	if ((m_nTrackAttribute == TDCA_PRIORITY) || (m_nTrackAttribute == TDCA_RISK))
	{
		// Prevent dragging of any calculated values
		if (HasOption(KBCF_DONEHAVELOWESTPRIORITYRISK) && pKI->IsDone(TRUE))
			return FALSE;

		if (HasOption(KBCF_DUEHAVEHIGHESTPRIORITYRISK) && pKI->IsDue())
			return FALSE;

		if (HasOption(KBCF_PARENTSSHOWHIGHESTPRIORITYRISK) && pKI->bParent)
			return FALSE;
	}

	return TRUE;
}

BOOL CKanbanCtrl::SelectionHasNonDraggableTasks(const CKanbanColumnCtrl* pSrcCol) const
{
	CDWordArray aSelTasks;
	int nNumTasks = pSrcCol->GetSelectedTaskIDs(aSelTasks);

	for (int nTask = 0; nTask < nNumTasks; nTask++)
	{
		if (!CanDragTask(aSelTasks[nTask]))
			return TRUE;
	}

	return FALSE;
}

BOOL CKanbanCtrl::CanEndDrag(const CKanbanColumnCtrl* pSrcCol, const CKanbanColumnCtrl* pDestCol) const
{
	// Can only copy MULTI-VALUE attributes
	if (Misc::ModKeysArePressed(MKS_CTRL) && !IsTrackedAttributeMultiValue())
		return FALSE;

	return CKanbanColumnCtrl::CanDrag(pSrcCol, pDestCol);
}

BOOL CKanbanCtrl::EndDragItems(CKanbanColumnCtrl* pSrcCol, const CDWordArray& aTaskIDs, 
								CKanbanColumnCtrl* pDestCol, const CString& sDestAttribValue)
{
	ASSERT(CanEndDrag(pSrcCol, pDestCol));
	ASSERT(pSrcCol->HasTasks(aTaskIDs));

	int nID = aTaskIDs.GetSize();

	while (nID--)
	{
		DWORD dwTaskID = aTaskIDs[nID];
		KANBANITEM* pKI = GetKanbanItem(dwTaskID);

		if (!pKI)
		{
			ASSERT(0);
			return FALSE;
		}

		BOOL bSrcIsBacklog = pSrcCol->IsBacklog();
		BOOL bDestIsBacklog = pDestCol->IsBacklog();
		BOOL bCopy = (!bSrcIsBacklog && 
						Misc::ModKeysArePressed(MKS_CTRL) &&
						IsTrackedAttributeMultiValue());

		// Remove from the source list(s) if moving
		if (bSrcIsBacklog)
		{
			VERIFY(pSrcCol->DeleteTask(dwTaskID));
		}
		else if (!bCopy) // move
		{
			// Remove all values
			pKI->RemoveAllTrackedAttributeValues(m_sTrackAttribID);

			// Remove from all src lists
			m_aColumns.DeleteTaskFromOthers(dwTaskID, pDestCol);
		}
		else if (bDestIsBacklog) // and 'copy'
		{
			// Just remove the source list's value(s)
			CStringArray aSrcValues;
			int nVal = pSrcCol->GetAttributeValues(aSrcValues);

			while (nVal--)
				pKI->RemoveTrackedAttributeValue(m_sTrackAttribID, aSrcValues[nVal]);

			VERIFY(pSrcCol->DeleteTask(dwTaskID));
		}

		// Append to the destination list
		if (bDestIsBacklog)
		{
			if (!pKI->HasTrackedAttributeValues(m_sTrackAttribID))
				pDestCol->AddTask(*pKI);
		}
		else
		{
			pKI->AddTrackedAttributeValue(m_sTrackAttribID, sDestAttribValue);

			if (pDestCol->FindItem(dwTaskID) == NULL)
				pDestCol->AddTask(*pKI);
		}
	}

	return TRUE;
}

BOOL CKanbanCtrl::GetColumnAttributeValue(const CKanbanColumnCtrl* pDestCol, const CPoint& ptScreen, CString& sValue) const
{
	CStringArray aListValues;
	int nNumValues = pDestCol->GetAttributeValues(aListValues);

	switch (nNumValues)
	{
	case 0: // Backlog
		sValue.Empty();
		return TRUE;
		
	case 1:
		sValue = aListValues[0];
		return TRUE;
	}

	// Column has multiple values -> show popup menu
	CMenu menu;
	VERIFY (menu.CreatePopupMenu());

	for (int nVal = 0; nVal < nNumValues; nVal++)
	{
		menu.AppendMenu(MF_STRING, (nVal + 1), aListValues[nVal]);
	}

	UINT nValID = menu.TrackPopupMenu((TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD), 
										ptScreen.x, ptScreen.y, CWnd::FromHandle(*pDestCol));

	if (nValID > 0)
	{
		sValue = aListValues[nValID - 1];
		return TRUE;
	}

	// user cancelled
	return FALSE;
}

void CKanbanCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (IsDragging())
	{
		// get the list and item under the mouse
		ClientToScreen(&point);
		m_ilDrag.DragMove(point);

		const CKanbanColumnCtrl* pDestCol = m_aColumns.HitTest(point);
		BOOL bValidDest = CanEndDrag(m_pSelectedColumn, pDestCol);

		if (bValidDest)
		{
			BOOL bCopy = Misc::ModKeysArePressed(MKS_CTRL);
			GraphicsMisc::SetDragDropCursor(bCopy ? GMOC_COPY : GMOC_MOVE);
		}
		else
		{
			// Only show the 'No' cursor when we've actually moved
			BOOL bSetNoCursor = TRUE;

			if (m_ptDragStart != DRAG_NOT_SET)
			{
				CRect rPos(point.x - 4, point.y - 4, point.x + 4, point.y + 4);

				if (rPos.PtInRect(m_ptDragStart))
				{
					bSetNoCursor = FALSE;
				}
				else
				{
					// We've really started moving
					ASSERT(m_pSelectedColumn);

					m_ptDragStart = DRAG_NOT_SET;

					CSize sizeImage;

					if (m_pSelectedColumn->CreateDragImage(m_ilDrag, sizeImage))
					{
						CPoint ptHotSpot((sizeImage.cx / 2), min(100, (sizeImage.cy / 2)));

						m_ilDrag.BeginDrag(0, ptHotSpot);
						m_ilDrag.DragEnter(NULL, m_ptDragStart);
					}
					else
					{
						ASSERT(0);
					}
				}
			}

			if (bSetNoCursor)
				GraphicsMisc::SetDragDropCursor(GMOC_NO);
		}

		m_aColumns.SetDropTarget(bValidDest ? pDestCol : NULL);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CKanbanCtrl::SaveToImage(CBitmap& bmImage)
{
	CAutoFlag af(m_bSavingToImage, TRUE);
	CEnBitmap bmColumns;

	if (m_aColumns.SaveToImage(bmColumns))
	{
		// Resize header and column widths to suit
		CIntArray aColWidths;
		m_header.GetItemWidths(aColWidths);

		int nTotalColumnWidth = bmColumns.GetSize().cx;
		int nItem = m_header.GetItemCount();

		int nReqColWidth = (nTotalColumnWidth / nItem);

		while (nItem--)
			m_header.SetItemWidth(nItem, nReqColWidth);

		CRect rHeader = CDialogHelper::GetChildRect(&m_header);
		CDialogHelper::ResizeChild(&m_header, (nTotalColumnWidth - rHeader.Width()), 0);

		CEnBitmap bmHeader;

		if (CCopyHeaderCtrlContents(m_header).DoCopy(bmHeader))
		{
			// Restore widths
			m_header.SetItemWidths(aColWidths);

			// Create one bitmap to fit both
			CDC dcImage, dcParts;
			CClientDC dc(this);

			if (dcImage.CreateCompatibleDC(&dc) && dcParts.CreateCompatibleDC(&dc))
			{
				// Create the image big enough to fit the tasks and columns side-by-side
				CSize sizeColumns = bmColumns.GetSize();
				CSize sizeHeader = bmHeader.GetSize();

				CSize sizeImage;

				sizeImage.cx = max(sizeHeader.cx, sizeColumns.cx);
				sizeImage.cy = (sizeHeader.cy + sizeColumns.cy);

				if (bmImage.CreateCompatibleBitmap(&dc, sizeImage.cx, sizeImage.cy))
				{
					CBitmap* pOldImage = dcImage.SelectObject(&bmImage);

					CBitmap* pOldPart = dcParts.SelectObject(&bmHeader);
					dcImage.BitBlt(0, 0, sizeHeader.cx, sizeHeader.cy, &dcParts, 0, 0, SRCCOPY);

					dcParts.SelectObject(&bmColumns);
					dcImage.BitBlt(0, sizeHeader.cy, sizeColumns.cx, sizeColumns.cy, &dcParts, 0, 0, SRCCOPY);

					dcParts.SelectObject(pOldPart);
					dcImage.SelectObject(pOldImage);
				}
			}
		}

		m_header.MoveWindow(rHeader);
	}

	ScrollToSelectedTask();

	return (bmImage.GetSafeHandle() != NULL);
}

BOOL CKanbanCtrl::CanSaveToImage() const
{
	return m_aColumns.CanSaveToImage();
}

LRESULT CKanbanCtrl::OnSetFont(WPARAM wp, LPARAM lp)
{
	m_fonts.Initialise((HFONT)wp, FALSE);
	m_aColumns.SetFont((HFONT)wp);
	m_header.SendMessage(WM_SETFONT, wp, lp);

	return 0L;
}

LRESULT CKanbanCtrl::OnColumnEditTaskDone(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(!m_bReadOnly);

	CDWordArray aTaskIDs;
	int nID = GetSelectedTaskIDs(aTaskIDs);
	ASSERT(nID);

	LRESULT lr = GetParent()->SendMessage(WM_KBC_EDITTASKDONE, 0, lp);

	if (lr)
	{
		ASSERT(m_pSelectedColumn);

		if (m_pSelectedColumn)
			m_pSelectedColumn->Invalidate(FALSE);

		return lr;
	}

	// Don't assert because this can legitimately fail/be cancelled
	return 0L;
}

LRESULT CKanbanCtrl::OnColumnEditTaskIcon(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT(!m_bReadOnly);

	return GetParent()->SendMessage(WM_KBC_EDITTASKICON);
}

LRESULT CKanbanCtrl::OnColumnEditTaskFlag(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(!m_bReadOnly);

	CDWordArray aTaskIDs;
	int nID = GetSelectedTaskIDs(aTaskIDs);
	ASSERT(nID);

	LRESULT lr = GetParent()->SendMessage(WM_KBC_EDITTASKFLAG, 0, lp);

	if (lr)
	{
		// Update item flag states
		while (nID--)
		{
			DWORD dwTaskID = aTaskIDs[nID];
			KANBANITEM* pKI = m_data.GetItem(dwTaskID);

			if (pKI && !pKI->bLocked)
				pKI->bFlagged = lp;
		}

		if (m_pSelectedColumn)
		{
			if (m_nSortBy == TDCA_FLAG)
			{
				m_pSelectedColumn->Sort(TDCA_FLAG, m_bSortAscending);
				ScrollToSelectedTask();
			}

			m_pSelectedColumn->Invalidate(FALSE);
		}

		return lr;
	}

	// else
	ASSERT(0);
	return 0L;
}

LRESULT CKanbanCtrl::OnColumnEditTaskPin(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(!m_bReadOnly);

	CDWordArray aTaskIDs;
	int nID = GetSelectedTaskIDs(aTaskIDs);
	ASSERT(nID);

	while (nID--)
	{
		DWORD dwTaskID = aTaskIDs[nID];
		KANBANITEM* pKI = m_data.GetItem(dwTaskID);

		if (pKI)
			pKI->bPinned = lp;
	}

	// Resort the selected column or all columns for a multi-value attribute
	if (IsTrackedAttributeMultiValue())
	{
		m_aColumns.Sort(m_nSortBy, m_bSortAscending);
	}
	else if (m_pSelectedColumn)
	{
		m_pSelectedColumn->Sort(m_nSortBy, m_bSortAscending);
	}
	else
	{
		ASSERT(0);
	}

	return 0L;
}

LRESULT CKanbanCtrl::OnColumnGetTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_KBC_GETTASKICON, wp, lp);
}

void CKanbanCtrl::OnColumnSetFocus(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;

	// Reverse focus changes outside of our own doing
	if (!m_bSettingColumnFocus)
	{
		FixupColumnFocus();
	}
}

LRESULT CKanbanCtrl::OnMidnight(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (HasOption(KBCF_DUEHAVEHIGHESTPRIORITYRISK))
	{
		// Column visibility AND contents may have changed
		RebuildColumns(KCRC_REBUILDCONTENTS | KCRC_RESTORESELECTION);
	}

	return 0L;
}
