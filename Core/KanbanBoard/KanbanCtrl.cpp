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

const UINT WM_KCM_SELECTTASK = (WM_APP+10); // WPARAM , LPARAM = Task ID

//////////////////////////////////////////////////////////////////////

const UINT IDC_COLUMNCTRL	= 101;
const UINT IDC_HEADER		= 102;

//////////////////////////////////////////////////////////////////////

const int MIN_COL_WIDTH = GraphicsMisc::ScaleByDPIFactor(6);
const int HEADER_HEIGHT = GraphicsMisc::ScaleByDPIFactor(24);

//////////////////////////////////////////////////////////////////////

static CString EMPTY_STR;

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
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, IDC_HEADER, OnHeaderDividerDoubleClick)
	ON_NOTIFY(HDN_ITEMCHANGING, IDC_HEADER, OnHeaderItemChanging)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_COLUMNCTRL, OnBeginDragColumnItem)
	ON_NOTIFY(TVN_SELCHANGED, IDC_COLUMNCTRL, OnColumnItemSelChange)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_COLUMNCTRL, OnColumnEditLabel)
	ON_NOTIFY(NM_SETFOCUS, IDC_COLUMNCTRL, OnColumnSetFocus)
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_KLCN_TOGGLETASKDONE, OnColumnToggleTaskDone)
	ON_MESSAGE(WM_KLCN_TOGGLETASKFLAG, OnColumnToggleTaskFlag)
	ON_MESSAGE(WM_KLCN_GETTASKICON, OnColumnGetTaskIcon)
	ON_MESSAGE(WM_KLCN_EDITTASKICON, OnColumnEditTaskIcon)
	ON_MESSAGE(WM_KCM_SELECTTASK, OnSelectTask)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

BOOL CKanbanCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
}

int CKanbanCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_fonts.Initialise(*this);
	
	ModifyStyleEx(0, WS_EX_CONTROLPARENT, 0);

	if (!m_header.Create(HDS_FULLDRAG | /*HDS_DRAGDROP |*/ WS_CHILD | WS_VISIBLE, 
						 CRect(lpCreateStruct->x, lpCreateStruct->y, lpCreateStruct->cx, 50),
						 this, IDC_HEADER))
	{
		return -1;
	}

	return 0;
}

void CKanbanCtrl::FilterToolTipMessage(MSG* pMsg) 
{
	// Column tooltips
	CKanbanColumnCtrl* pCol = m_aColumns.HitTest(pMsg->pt);

	if (pCol)
		pCol->FilterToolTipMessage(pMsg);
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
	HTREEITEM hti = m_pSelectedColumn->GetSelectedItem();

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

BOOL CKanbanCtrl::HandleKeyDown(WPARAM wp, LPARAM lp)
{
	switch (wp)
	{
	case VK_LEFT:
		if (m_pSelectedColumn->GetSelectedItem())
		{
			int nSelCol = m_aColumns.Find(m_pSelectedColumn);

			for (int nCol = (nSelCol - 1); nCol >= 0; nCol--)
			{
				if (SelectClosestAdjacentItemToSelection(nCol))
					return TRUE;
			}
		}
		break;

	case VK_HOME:
		if (m_pSelectedColumn->GetSelectedItem())
		{
			int nSelCol = m_aColumns.Find(m_pSelectedColumn);

			for (int nCol = 0; nCol < nSelCol; nCol++)
			{
				if (SelectClosestAdjacentItemToSelection(nCol))
					return TRUE;
			}
		}
		break;

	case VK_RIGHT:
		if (m_pSelectedColumn->GetSelectedItem())
		{
			int nSelCol = m_aColumns.Find(m_pSelectedColumn);
			int nNumCol = m_aColumns.GetSize();

			for (int nCol = (nSelCol + 1); nCol < nNumCol; nCol++)
			{
				if (SelectClosestAdjacentItemToSelection(nCol))
					return TRUE;
			}
		}
		break;

	case VK_END:
		if (m_pSelectedColumn->GetSelectedItem())
		{
			int nSelCol = m_aColumns.Find(m_pSelectedColumn);
			int nNumCol = m_aColumns.GetSize();

			for (int nCol = (nNumCol - 1); nCol > nSelCol; nCol--)
			{
				if (SelectClosestAdjacentItemToSelection(nCol))
					return TRUE;
			}
		}
		break;

	case VK_ESCAPE:
		// handle 'escape' during dragging
		return (CancelOperation() != FALSE);

	case VK_DELETE:
		if (m_pSelectedColumn && !m_pSelectedColumn->IsBacklog())
		{
			// For each of the selected tasks remove the attribute value(s) of the
			// selected list (column). Tasks having no values remaining are moved 
			// to the backlog
			CStringArray aListValues;
			VERIFY(m_pSelectedColumn->GetAttributeValues(aListValues));

			DWORD dwTaskID = GetSelectedTaskID();
			KANBANITEM* pKI = GetKanbanItem(dwTaskID);
			ASSERT(pKI);

			if (pKI)
				pKI->RemoveTrackedAttributeValues(m_sTrackAttribID, aListValues);

			// Notify parent of changes before altering the lists because we can't
			// guarantee that all the modified tasks will be in the same list afterwards
			NotifyParentAttibuteChange(dwTaskID);

			// Reset selected list before removing items to 
			// to prevent unwanted selection notifications
			CKanbanColumnCtrl* pCol = m_pSelectedColumn;
			m_pSelectedColumn = NULL;

			if (pKI)
			{
				VERIFY(pCol->DeleteTask(dwTaskID));

				if (!pKI->HasTrackedAttributeValues(m_sTrackAttribID))
				{
					CKanbanColumnCtrl* pBacklog = m_aColumns.GetBacklog();

					if (pBacklog)
						pBacklog->AddTask(*pKI, FALSE);
				}

				RebuildColumns(FALSE, FALSE);
			}

			// try to restore selection
			SelectTask(dwTaskID);
			return TRUE;
		}
		break;
	}

	// all else
	return FALSE;
}

BOOL CKanbanCtrl::SelectTask(DWORD dwTaskID)
{
	CAutoFlag af(m_bSelectTasks, TRUE);

	// Check for 'no change'
	DWORD dwSelID = GetSelectedTaskID();

	int nPrevSel = m_aColumns.Find(m_pSelectedColumn), nNewSel = -1;

	if (m_pSelectedColumn && m_pSelectedColumn->FindTask(dwTaskID))
		nNewSel = nPrevSel;
	else
		nNewSel = m_aColumns.Find(dwTaskID);

	if ((nPrevSel != nNewSel) || (dwTaskID != dwSelID))
	{
		m_aColumns.SetSelectedColumn(NULL);

		if ((nNewSel == -1) || (dwTaskID == 0))
			return FALSE;

		// else
		SelectColumn(m_aColumns[nNewSel], FALSE);
		VERIFY(m_pSelectedColumn->SelectTask(dwTaskID));
	}

	ScrollToSelectedTask();
	return TRUE;
}

DWORD CKanbanCtrl::GetSelectedTaskID() const
{
	const CKanbanColumnCtrl* pCol = GetSelColumn();

	if (pCol)
		return pCol->GetSelectedTaskID();

	// else
	return 0;
}

CKanbanColumnCtrl* CKanbanCtrl::GetSelColumn()
{
	ASSERT((m_pSelectedColumn == NULL) || Misc::HasT(m_pSelectedColumn, m_aColumns));

	if (!m_pSelectedColumn && m_aColumns.GetSize())
		m_pSelectedColumn = m_aColumns[0];

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
			htiStart = pCol->GetNextSiblingItem(pCol->GetSelectedItem());;
		break;

	case IUI_SELECTNEXTTASKINCLCURRENT:
		pCol = m_pSelectedColumn;

		if (pCol)
			htiStart = pCol->GetSelectedItem();
		break;

	case IUI_SELECTPREVTASK:
		pCol = m_pSelectedColumn;
		bForwards = FALSE;

		if (pCol)
			htiStart = pCol->GetPrevSiblingItem(pCol->GetSelectedItem());
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
			hti = pCol->FindTask(select, bForwards, hti);

			if (hti)
			{
				SelectColumn(pCol, FALSE);
				return pCol->SelectItem(hti, FALSE);
			}

			// else
			pCol = GetNextColumn(pCol, bForwards, TRUE);
			hti = (bForwards ? pCol->TCH().GetFirstItem() : pCol->TCH().GetLastItem());
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
 		RebuildColumns(TRUE, TRUE);
		break;
		
	case IUI_NEW:
	case IUI_EDIT:
		{
 			// update the task(s)
			BOOL bChange = UpdateGlobalAttributeValues(pTasks);
			bChange |= UpdateData(pTasks, pTasks->GetFirstTask(), TRUE);

			if (bChange)
			{
				RebuildColumns(TRUE, TRUE);
			}
			else if (UpdateNeedsItemHeightRefresh(pTasks))
			{
				m_aColumns.RefreshItemLineHeights();
			}
			else
			{
				m_aColumns.Redraw(FALSE);
			}
		}
		break;
		
	case IUI_DELETE:
		RemoveDeletedTasks(pTasks);
		break;
		
	default:
		ASSERT(0);
	}
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
	case TDCA_FILEREF:
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
	case TDCA_TIMEEST:
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
		return HasOption(KBCF_SORTSUBTASTASKSBELOWPARENTS);
	}

	// all else
	return WantEditUpdate(nAttrib);
}

BOOL CKanbanCtrl::RebuildData(const ITASKLISTBASE* pTasks)
{
	// Rebuild global attribute value lists
	m_mapAttributeValues.RemoveAll();
	m_aCustomAttribDefs.RemoveAll();

	UpdateGlobalAttributeValues(pTasks);

	// Rebuild data
	m_data.RemoveAll();

	return AddTaskToData(pTasks, pTasks->GetFirstTask(), 0, TRUE);
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
		pKI->bFlag = (pTasks->IsTaskFlagged(hTask, false) ? TRUE : FALSE);
		pKI->nPosition = pTasks->GetTaskPosition(hTask);

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
		pKI->SetTrackedAttributeValue(TDCA_PRIORITY, pTasks->GetTaskPriority(hTask, FALSE));
		pKI->SetTrackedAttributeValue(TDCA_RISK, pTasks->GetTaskRisk(hTask, FALSE));

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

BOOL CKanbanCtrl::UpdateData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return FALSE;

	// Not interested in references
	if (pTasks->IsTaskReference(hTask))
		return FALSE; 

	// handle task if not NULL (== root)
	BOOL bChange = FALSE;
	DWORD dwTaskID = pTasks->GetTaskID(hTask);

	if (dwTaskID)
	{
		// Can be a new task
		if (!HasKanbanItem(dwTaskID))
		{
			bChange = AddTaskToData(pTasks, hTask, pTasks->GetTaskParentID(hTask), FALSE);
		}
		else
		{
			KANBANITEM* pKI = NULL;
			GET_KI_RET(dwTaskID, pKI, FALSE);

			if (pTasks->IsAttributeAvailable(TDCA_TASKNAME))
				pKI->sTitle = pTasks->GetTaskTitle(hTask);
			
			if (pTasks->IsAttributeAvailable(TDCA_DONEDATE))
			{
				BOOL bDone = pTasks->IsTaskDone(hTask);
				BOOL bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);

				if ((pKI->bDone != bDone) || (pKI->bGoodAsDone != bGoodAsDone))
				{
					pKI->bDone = bDone;
					pKI->bGoodAsDone = bGoodAsDone;
				}
			}

			if (pTasks->IsAttributeAvailable(TDCA_SUBTASKDONE))
			{
				LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
				pKI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));
			}

			if (pTasks->IsAttributeAvailable(TDCA_ICON))
				pKI->bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

			if (pTasks->IsAttributeAvailable(TDCA_FLAG))
				pKI->bFlag = (pTasks->IsTaskFlagged(hTask, true) ? TRUE : FALSE);
			
			// Trackable attributes
			CStringArray aValues;

			if (pTasks->IsAttributeAvailable(TDCA_ALLOCTO))
			{
				GetTaskAllocTo(pTasks, hTask, aValues);
				bChange |= UpdateTrackableTaskAttribute(pKI, TDCA_ALLOCTO, aValues);
			}

			if (pTasks->IsAttributeAvailable(TDCA_CATEGORY))
			{
				GetTaskCategories(pTasks, hTask, aValues);
				bChange |= UpdateTrackableTaskAttribute(pKI, TDCA_CATEGORY, aValues);
			}

			if (pTasks->IsAttributeAvailable(TDCA_TAGS))
			{
				GetTaskTags(pTasks, hTask, aValues);
				bChange |= UpdateTrackableTaskAttribute(pKI, TDCA_TAGS, aValues);
			}

			if (pTasks->IsAttributeAvailable(TDCA_ALLOCBY))
				bChange |= UpdateTrackableTaskAttribute(pKI, TDCA_ALLOCBY, pTasks->GetTaskAllocatedBy(hTask));

			if (pTasks->IsAttributeAvailable(TDCA_STATUS))
				bChange |= UpdateTrackableTaskAttribute(pKI, TDCA_STATUS, pTasks->GetTaskStatus(hTask));

			if (pTasks->IsAttributeAvailable(TDCA_VERSION))
				bChange |= UpdateTrackableTaskAttribute(pKI, TDCA_VERSION, pTasks->GetTaskVersion(hTask));

			if (pTasks->IsAttributeAvailable(TDCA_PRIORITY))
				bChange |= UpdateTrackableTaskAttribute(pKI, TDCA_PRIORITY, pTasks->GetTaskPriority(hTask, true));

			if (pTasks->IsAttributeAvailable(TDCA_RISK))
				bChange |= UpdateTrackableTaskAttribute(pKI, TDCA_RISK, pTasks->GetTaskRisk(hTask, true));

			if (pTasks->IsAttributeAvailable(TDCA_CUSTOMATTRIB))
			{
				int nID = m_aCustomAttribDefs.GetSize();

				while (nID--)
				{
					KANBANCUSTOMATTRIBDEF& def = m_aCustomAttribDefs[nID];

					CString sValue(pTasks->GetTaskCustomAttributeData(hTask, def.sAttribID, true));
					CStringArray aValues;

					if (!sValue.IsEmpty())
					{
						if (Misc::Split(sValue, aValues) > 1)
							def.bMultiValue = TRUE;
					}

					if (UpdateTrackableTaskAttribute(pKI, def.sAttribID, aValues))
					{
						// Add to global values
						CKanbanValueMap* pValues = m_mapAttributeValues.GetAddMapping(def.sAttribID);
						ASSERT(pValues);
						
						pValues->AddValues(aValues);
						bChange = TRUE;
					}
				}
			}

			// other display-only attributes
			UpdateItemDisplayAttributes(pKI, pTasks, hTask);
			
			// always update colour because it can change for so many reasons
			pKI->SetColor(pTasks->GetTaskTextColor(hTask));

			// Always update lock state
			pKI->bLocked = pTasks->IsTaskLocked(hTask, true);
		}
	}
		
	// children
	if (UpdateData(pTasks, pTasks->GetFirstTask(hTask), TRUE))
		bChange = TRUE;

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			if (UpdateData(pTasks, hSibling, FALSE))
				bChange = TRUE;
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
	
	return bChange;
}

void CKanbanCtrl::UpdateItemDisplayAttributes(KANBANITEM* pKI, const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	time64_t tDate = 0;
	
	if (pTasks->IsAttributeAvailable(TDCA_TIMEEST))
		pKI->dTimeEst = pTasks->GetTaskTimeEstimate(hTask, pKI->nTimeEstUnits, true);
	
	if (pTasks->IsAttributeAvailable(TDCA_TIMESPENT))
		pKI->dTimeSpent = pTasks->GetTaskTimeSpent(hTask, pKI->nTimeSpentUnits, true);
	
	if (pTasks->IsAttributeAvailable(TDCA_COST))
		pKI->sCost = pTasks->GetTaskAttribute(hTask, TDCA_COST);
	
	if (pTasks->IsAttributeAvailable(TDCA_CREATEDBY))
		pKI->sCreatedBy = pTasks->GetTaskCreatedBy(hTask);
	
	if (pTasks->IsAttributeAvailable(TDCA_CREATIONDATE))
		pKI->dtCreate = pTasks->GetTaskCreationDate(hTask);
	
	if (pTasks->IsAttributeAvailable(TDCA_DONEDATE) && pTasks->GetTaskDoneDate64(hTask, tDate))
		pKI->dtDone = CDateHelper::GetDate(tDate);
	
	if (pTasks->IsAttributeAvailable(TDCA_DUEDATE) && pTasks->GetTaskDueDate64(hTask, true, tDate))
		pKI->dtDue = CDateHelper::GetDate(tDate);
	
	if (pTasks->IsAttributeAvailable(TDCA_STARTDATE) && pTasks->GetTaskStartDate64(hTask, true, tDate))
		pKI->dtStart = CDateHelper::GetDate(tDate);
	
	if (pTasks->IsAttributeAvailable(TDCA_LASTMODDATE) && pTasks->GetTaskLastModified64(hTask, tDate))
		pKI->dtLastMod = CDateHelper::GetDate(tDate);
	
	if (pTasks->IsAttributeAvailable(TDCA_PERCENT))
		pKI->nPercent = pTasks->GetTaskPercentDone(hTask, true);
	
	if (pTasks->IsAttributeAvailable(TDCA_EXTERNALID))
		pKI->sExternalID = pTasks->GetTaskExternalID(hTask);
	
	if (pTasks->IsAttributeAvailable(TDCA_RECURRENCE))
		pKI->sRecurrence = ((ITaskList*)pTasks)->GetTaskAttribute(hTask, TDL_TASKRECURRENCE);

	if (pTasks->IsAttributeAvailable(TDCA_FILEREF) && pTasks->GetTaskFileLinkCount(hTask))
	{
		pKI->sFileRef = pTasks->GetTaskFileLink(hTask, 0);

		// Get the shortest meaningful bit because of space constraints
		if (FileMisc::IsPath(pKI->sFileRef))
			pKI->sFileRef = FileMisc::GetFileNameFromPath(pKI->sFileRef);
	}
}

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks)
{
	BOOL bChange = FALSE;

	if (pTasks->IsAttributeAvailable(TDCA_STATUS))
		bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_STATUS);
	
	if (pTasks->IsAttributeAvailable(TDCA_ALLOCTO))
		bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_ALLOCTO);
	
	if (pTasks->IsAttributeAvailable(TDCA_CATEGORY))
		bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_CATEGORY);
	
	if (pTasks->IsAttributeAvailable(TDCA_ALLOCBY))
		bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_ALLOCBY);
	
	if (pTasks->IsAttributeAvailable(TDCA_TAGS))
		bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_TAGS);
	
	if (pTasks->IsAttributeAvailable(TDCA_VERSION))
		bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_VERSION);
	
	if (pTasks->IsAttributeAvailable(TDCA_PRIORITY))
		bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_PRIORITY);
	
	if (pTasks->IsAttributeAvailable(TDCA_RISK))
		bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_RISK);
	
	if (pTasks->IsAttributeAvailable(TDCA_CUSTOMATTRIB))
		bChange |= UpdateGlobalAttributeValues(pTasks, TDCA_CUSTOMATTRIB);
	
	return bChange;
}

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks, TDC_ATTRIBUTE nAttribute)
{
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

					int nDef = m_aCustomAttribDefs.AddDefinition(sAttribID, sAttribName);

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
					{
						pDefValues->SetValues(aDefValues);

						if (aDefValues.GetSize() > 1)
							m_aCustomAttribDefs.SetMultiValue(nDef);
					}

					CStringArray aAutoValues;
					Misc::Split(sAutoData, aAutoValues, '\n');

					bChange |= UpdateGlobalAttributeValues(sAttribID, aAutoValues);
				}
			}

			return bChange;
		}
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
		pKI->GetTrackedAttributeValues(m_sTrackAttribID, aValues);
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

void CKanbanCtrl::LoadDefaultAttributeListValues(const IPreferences* pPrefs)
{
	m_mapGlobalAttributeValues.RemoveAll();

	LoadDefaultAttributeListValues(pPrefs, _T("ALLOCTO"),	_T("AllocToList"));
	LoadDefaultAttributeListValues(pPrefs, _T("ALLOCBY"),	_T("AllocByList"));
	LoadDefaultAttributeListValues(pPrefs, _T("STATUS"),	_T("StatusList"));
	LoadDefaultAttributeListValues(pPrefs, _T("CATEGORY"),	_T("CategoryList"));
	LoadDefaultAttributeListValues(pPrefs, _T("VERSION"),	_T("VersionList"));
	LoadDefaultAttributeListValues(pPrefs, _T("TAGS"),		_T("TagList"));

	if (m_nTrackAttribute != TDCA_NONE)
		RebuildColumns(FALSE, FALSE);
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
#ifdef _DEBUG
	switch (nAttrib)
	{
	case TDCA_PRIORITY:
	case TDCA_RISK:
		break;

	default:
		ASSERT(0);
		break;
	}
#endif

	CString sValue; // empty

	if (nNewValue >= 0)
		sValue = Misc::Format(nNewValue);
	
	// else empty
	return UpdateTrackableTaskAttribute(pKI, nAttrib, sValue);
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
	CStringArray aNewValues;

	switch (nAttrib)
	{
	case TDCA_PRIORITY:
	case TDCA_RISK:
		if (!sNewValue.IsEmpty())
			aNewValues.Add(sNewValue);
		break;

	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_VERSION:
		aNewValues.Add(sNewValue);
		break;

	default:
		ASSERT(0);
		break;
	}
	
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
	if (!IsTracking(sAttribID) || (pKI->bParent && !HasOption(KBCF_SHOWPARENTTASKS)))
	{
		pKI->SetTrackedAttributeValues(sAttribID, aNewValues);
		return FALSE; // no effect on list items
	}

	// else
	BOOL bChange = FALSE;
	
	if (!pKI->AttributeValuesMatch(sAttribID, aNewValues))
	{
		CStringArray aCurValues;
		pKI->GetTrackedAttributeValues(sAttribID, aCurValues);
		
		// Remove any list item whose current value is not found in the new values
		int nVal = aCurValues.GetSize();
		
		// Special case: Item needs removing from backlog
		if (nVal == 0)
		{
			aCurValues.Add(_T(""));
			nVal++;
		}

		while (nVal--)
		{
			if (!Misc::Contains(aCurValues[nVal], aNewValues, FALSE, TRUE))
			{
				CKanbanColumnCtrl* pCurCol = m_aColumns.Get(aCurValues[nVal]);
				ASSERT(pCurCol);

				if (pCurCol)
				{
					VERIFY(pCurCol->DeleteTask(pKI->dwTaskID));
					bChange |= (pCurCol->GetCount() == 0);
				}

				// Remove from list to speed up later searching
				aCurValues.RemoveAt(nVal);
			}
		}
		
		// Add any new items not in the current list
		nVal = aNewValues.GetSize();
		
		while (nVal--)
		{
			if (!Misc::Contains(aNewValues[nVal], aCurValues, FALSE, TRUE))
			{
				CKanbanColumnCtrl* pCurCol = m_aColumns.Get(aNewValues[nVal]);
				
				if (pCurCol)
					pCurCol->AddTask(*pKI, FALSE);
				else
					bChange = TRUE; // needs new list ctrl
			}
		}
	
		// update values
		pKI->SetTrackedAttributeValues(sAttribID, aNewValues);
	}
	
	return bChange;
}

BOOL CKanbanCtrl::IsTracking(const CString& sAttribID) const
{
	return (m_sTrackAttribID.CompareNoCase(sAttribID) == 0);
}

BOOL CKanbanCtrl::WantShowColumn(LPCTSTR szValue, const CKanbanItemArrayMap& mapKIArray) const
{
	if (HasOption(KBCF_SHOWEMPTYCOLUMNS))
		return TRUE;

	if (HasOption(KBCF_ALWAYSSHOWBACKLOG) && Misc::IsEmpty(szValue))
		return TRUE;

	// else
	const CKanbanItemArray* pKIArr = mapKIArray.GetMapping(szValue);
		
	return (pKIArr && pKIArr->GetSize());
}

BOOL CKanbanCtrl::WantShowColumn(const CKanbanColumnCtrl* pCol) const
{
	if (HasOption(KBCF_SHOWEMPTYCOLUMNS))
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
		
		if ((pGlobals == NULL) || !WantShowColumn(pCol))
		{
			DeleteColumn(nCol);
			nNumRemoved++;
		}
		else
		{
			CString sAttribValueID(pCol->GetAttributeValueID());
			
			if (!Misc::HasKey(*pGlobals, sAttribValueID) || 
				!WantShowColumn(sAttribValueID, mapKIArray))
			{
				DeleteColumn(nCol);
				nNumRemoved++;
			}
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

		ASSERT(!HasOption(KBCF_SHOWEMPTYCOLUMNS) || 
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
	m_aColumns.SortColumns();
}

void CKanbanCtrl::RebuildFixedColumns(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingFixedColumns())
	{
		ASSERT(0);
		return;
	}

	if (m_aColumns.GetSize() == 0) // first time only
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

void CKanbanCtrl::RebuildColumns(BOOL bRebuildData, BOOL bTaskUpdate)
{
	if (m_sTrackAttribID.IsEmpty())
	{
		ASSERT(m_nTrackAttribute == TDCA_NONE);
		return;
	}

	CHoldRedraw gr(*this, NCR_PAINT | NCR_ERASEBKGND);
	DWORD dwSelTaskID = GetSelectedTaskID();
	
	CKanbanItemArrayMap mapKIArray;
	m_data.BuildTempItemMaps(m_sTrackAttribID, mapKIArray);

	if (UsingDynamicColumns())
		RebuildDynamicColumns(mapKIArray);
	else
		RebuildFixedColumns(mapKIArray);

	// Rebuild the list data for each list (which can be empty)
	if (bRebuildData)
	{
		RebuildColumnsData(mapKIArray);
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
		
	// We only need to restore selection if not doing a task update
	// because the app takes care of that
	if (!bTaskUpdate && dwSelTaskID && !SelectTask(dwSelTaskID))
	{
		if (!m_pSelectedColumn || !Misc::HasT(m_pSelectedColumn, m_aColumns))
		{
			// Find the first list with some items
			m_pSelectedColumn = m_aColumns.GetFirstNonEmpty();

			// No list has items?
			if (!m_pSelectedColumn)
				m_pSelectedColumn = m_aColumns[0];
		}
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

	int nNumColumns = m_aColumns.GetSize();

	for (int nCol = 0, nVis = 0; nCol < nNumColumns; nCol++)
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

		// Allow tracking on all but the last column
		m_header.EnableItemTracking(nVis, (nVis < (nNumVisColumns - 1)));

		nVis++;
	}
}

void CKanbanCtrl::RebuildColumnsData(const CKanbanItemArrayMap& mapKIArray)
{
	BOOL bShowParents = HasOption(KBCF_SHOWPARENTTASKS);
	int nCol = m_aColumns.GetSize();
	
	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = m_aColumns[nCol];
		ASSERT(pCol);
		
		RebuildColumnContents(pCol, mapKIArray, bShowParents);
		
		// The list can still end up empty if parent tasks are 
		// omitted in Dynamic columns so we recheck and delete if required
		if (UsingDynamicColumns())
		{
			if (!bShowParents && !WantShowColumn(pCol))
			{
				DeleteColumn(nCol);
			}
		}
	}
		
	// Columns can still end up empty if there were 
	// only unwanted parents
	CheckAddBacklogColumn();

	// Resort
	Sort(m_nSortBy, m_bSortAscending);
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
			m_pSelectedColumn->Invalidate(TRUE);
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

	RebuildColumns(TRUE, TRUE);
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

BOOL CKanbanCtrl::RebuildColumnContents(CKanbanColumnCtrl* pCol, const CKanbanItemArrayMap& mapKIArray, BOOL bShowParents)
{
	ASSERT(pCol && pCol->GetSafeHwnd());

	if (!pCol || !pCol->GetSafeHwnd())
		return FALSE;

	DWORD dwSelID = pCol->GetSelectedTaskID();

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
				
				if (!pKI->bParent || bShowParents)
				{
					BOOL bSelected = (dwSelID == pKI->dwTaskID);

					VERIFY(pCol->AddTask(*pKI, bSelected) != NULL);
				}
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
		RebuildColumns(FALSE, TRUE);
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
		hti = m_pSelectedColumn->FindTask(dwTaskID);

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
		hti = pCol->FindTask(dwTaskID);

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
	if (dwOptions != m_dwOptions)
	{
		DWORD dwPrevOptions = m_dwOptions;
		m_dwOptions = dwOptions;

		if (Misc::FlagHasChanged(KBCF_SHOWPARENTTASKS, m_dwOptions, dwPrevOptions))
		{
			RebuildColumns(TRUE, FALSE);
		}
		else if (Misc::FlagHasChanged(KBCF_SHOWEMPTYCOLUMNS | KBCF_ALWAYSSHOWBACKLOG, m_dwOptions, dwPrevOptions))
		{
			RebuildColumns(FALSE, FALSE);
		}

		m_aColumns.SetOptions(dwOptions & ~(KBCF_SHOWPARENTTASKS | KBCF_SHOWEMPTYCOLUMNS | KBCF_ALWAYSSHOWBACKLOG));

		if ((m_nSortBy != TDCA_NONE) && Misc::FlagHasChanged(KBCF_SORTSUBTASTASKSBELOWPARENTS, m_dwOptions, dwPrevOptions))
		{
			m_aColumns.SortItems(m_nSortBy, m_bSortAscending);
		}
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
	CWnd::OnSetFocus(pOldWnd);

	FixupColumnFocus();
	ScrollToSelectedTask();
}

int CKanbanCtrl::GetVisibleColumnCount() const
{
	if (UsingDynamicColumns() || HasOption(KBCF_SHOWEMPTYCOLUMNS))
		return m_aColumns.GetSize();

	// Fixed columns
	BOOL bAlwaysShowBacklog = HasOption(KBCF_ALWAYSSHOWBACKLOG);
	int nCol = m_aColumns.GetSize(), nNumVis = 0;

	while (nCol--)
	{
		const CKanbanColumnCtrl* pCol = m_aColumns[nCol];
		ASSERT(pCol);

		if (!pCol->GetCount())
		{
			if (!bAlwaysShowBacklog || !pCol->IsBacklog())
				continue;
		}

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

				int nAtt = m_aDisplayAttrib.GetSize();

				while (nAtt--)
				{
					TDC_ATTRIBUTE nAttribID = m_aDisplayAttrib[nAtt];

					// Exclude 'File Link' and 'Parent' because these will 
					// almost always push things over the limit
					// Exclude 'flag' because that is rendered as an icon
					switch (nAttribID)
					{
					case TDCA_FILEREF:
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
	KBC_ATTRIBLABELS nLabelVis[2] = { KBCAL_LONG, KBCAL_SHORT };

	for (int nPass = 0; nPass < 2; nPass++)
	{
		if (CanFitAttributeLabels(nAvailWidth, fAveCharWidth, nLabelVis[nPass]))
			return nLabelVis[nPass];
	}

	return KBCAL_NONE;
}

void CKanbanCtrl::Sort(TDC_ATTRIBUTE nBy, BOOL bAscending)
{
	// if the sort attribute equals the track attribute then
	// tasks are already sorted into separate columns so we  
	// sort by title instead
	if ((nBy != TDCA_NONE) && (nBy == m_nTrackAttribute))
		nBy = TDCA_TASKNAME;
	
	m_nSortBy = nBy;

	if ((m_nSortBy != TDCA_NONE) || HasOption(KBCF_SORTSUBTASTASKSBELOWPARENTS))
	{
		ASSERT((m_nSortBy == TDCA_NONE) || (bAscending != -1));
		m_bSortAscending = bAscending;

		// do the sort
 		CHoldRedraw hr(*this);

		m_aColumns.SortItems(m_nSortBy, m_bSortAscending);
	}
}

void CKanbanCtrl::SetReadOnly(bool bReadOnly) 
{ 
	m_bReadOnly = bReadOnly; 
}

BOOL CKanbanCtrl::GetLabelEditRect(LPRECT pEdit)
{
	if (!m_pSelectedColumn || !m_pSelectedColumn->GetLabelEditRect(pEdit))
	{
		ASSERT(0);
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

BOOL CKanbanCtrl::NotifyParentAttibuteChange(DWORD dwTaskID)
{
	ASSERT(!m_bReadOnly);
	ASSERT(dwTaskID);

	return GetParent()->SendMessage(WM_KBC_VALUECHANGE, (WPARAM)GetSafeHwnd(), dwTaskID);
}

void CKanbanCtrl::NotifyParentSelectionChange()
{
	ASSERT(!m_bSelectTasks);

	GetParent()->SendMessage(WM_KBC_SELECTIONCHANGE, GetSelectedTaskID(), 0);
}

// external version
BOOL CKanbanCtrl::CancelOperation()
{
	if (IsDragging())
	{
		ReleaseCapture();
		m_aColumns.SetDropTarget(NULL);

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

			if (m_pSelectedColumn->GetSelectedItem())
				m_pSelectedColumn->ScrollToSelection();

			if (bNotifyParent)
				NotifyParentSelectionChange();
		}
		else
		{
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

void CKanbanCtrl::OnColumnItemSelChange(NMHDR* pNMHDR, LRESULT* pResult)
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

void CKanbanCtrl::OnColumnEditLabel(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = TRUE; // cancel our edit

	NMTVDISPINFO* pNMTV = (NMTVDISPINFO*)pNMHDR;
	ASSERT(pNMTV->item.lParam);

	GetParent()->SendMessage(WM_KBC_EDITTASKTITLE, pNMTV->item.lParam);
}

void CKanbanCtrl::OnHeaderDividerDoubleClick(NMHDR* pNMHDR, LRESULT* pResult)
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

void CKanbanCtrl::OnHeaderItemChanging(NMHDR* pNMHDR, LRESULT* pResult)
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
		CKanbanColumnCtrl* pThisCol = m_aColumns[pHDN->iItem];
		CKanbanColumnCtrl* pNextCol = m_aColumns[pHDN->iItem + 1];

		CRect rThisCol = CDialogHelper::GetChildRect(pThisCol);
		rThisCol.right = (rThisCol.left + pHDN->pitem->cxy - 1);

		CRect rNextCol = CDialogHelper::GetChildRect(m_aColumns[pHDN->iItem + 1]);
		rNextCol.left = (rThisCol.right + 1);

		pThisCol->MoveWindow(rThisCol, TRUE);
		pNextCol->MoveWindow(rNextCol, TRUE);

		// Redraw the vertical divider
		CRect rDivider(rThisCol);
		rDivider.left = rDivider.right;
		rDivider.right++;

		Invalidate(TRUE);
	}
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
		else if (!m_bSavingToImage && m_pSelectedColumn)
		{
			// Show the text of the selected column in bold
			if (pNMCD->lItemlParam == (LPARAM)m_pSelectedColumn)
				::SelectObject(pNMCD->hdc, m_fonts.GetHFont(GMFS_BOLD));
			else
				::SelectObject(pNMCD->hdc, m_fonts.GetHFont());
			
			*pResult = CDRF_NEWFONT;
		}
		break;
		
	case CDDS_ITEMPOSTPAINT:
		{
			ASSERT(GraphicsMisc::GetRTLDrawTextFlags(hwndHdr) == DT_RTLREADING);

			CRect rItem(pNMCD->rc);
			rItem.DeflateRect(3, 0);

			CDC* pDC = CDC::FromHandle(pNMCD->hdc);
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
		break;
	}
}

void CKanbanCtrl::OnBeginDragColumnItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	ReleaseCapture();

	if (!m_bReadOnly && !IsDragging())
	{
		ASSERT(pNMHDR->idFrom == IDC_COLUMNCTRL);

		if (Misc::IsKeyPressed(VK_LBUTTON))
		{
			NMTREEVIEW* pNMTV = (NMTREEVIEW*)pNMHDR;
			ASSERT(pNMTV->itemNew.hItem);
			ASSERT(pNMTV->itemNew.lParam);
		
			CKanbanColumnCtrl* pCol = (CKanbanColumnCtrl*)CWnd::FromHandle(pNMHDR->hwndFrom);
			ASSERT(pCol == m_pSelectedColumn);

			if (!pCol->SelectionHasLockedTasks())
			{
				SetCapture();
				TRACE(_T("CKanbanCtrl::OnBeginDragColItem(start drag)\n"));
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

	if (m_data.IsLocked(dwTaskID))
		return GraphicsMisc::SetAppCursor(_T("Locked"), _T("Resources\\Cursors"));

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

		if (CanDrag(pSrcCol, pDestCol))
		{
			CString sDestAttribValue;
			
			if (GetColumnAttributeValue(pDestCol, point, sDestAttribValue))
			{
				DWORD dwDragID = pSrcCol->GetSelectedTaskID();
				ASSERT(dwDragID);

				BOOL bChange = EndDragItem(pSrcCol, dwDragID, pDestCol, sDestAttribValue);

				// Remove the source column if it is now empty 
				// and should not be shown
				if (bChange && UsingDynamicColumns() && !WantShowColumn(pSrcCol))
				{
					m_pSelectedColumn = NULL;

					int nCol = Misc::FindT(pSrcCol, m_aColumns);
					ASSERT(nCol != -1);

					m_header.DeleteItem(nCol);
					m_aColumns.RemoveAt(nCol);
				}

				Resize();

				if (bChange)
				{
					RebuildColumnHeader();

					// Resort before fixing up selection
					if ((m_nSortBy != TDCA_NONE) || HasOption(KBCF_SORTSUBTASTASKSBELOWPARENTS))
						pDestCol->Sort(m_nSortBy, m_bSortAscending);

					SelectColumn(pDestCol, FALSE);
					SelectTask(dwDragID); 

					NotifyParentSelectionChange();
					NotifyParentAttibuteChange(dwDragID);
				}
			}
		}

		// always
		m_aColumns.SetDropTarget(NULL);
		ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CKanbanCtrl::CanDrag(const CKanbanColumnCtrl* pSrcCol, const CKanbanColumnCtrl* pDestCol) const
{
	// Can only copy MULTI-VALUE attributes
	if (Misc::ModKeysArePressed(MKS_CTRL) && !IsTrackedAttributeMultiValue())
		return FALSE;

	return CKanbanColumnCtrl::CanDrag(pSrcCol, pDestCol);
}

BOOL CKanbanCtrl::EndDragItem(CKanbanColumnCtrl* pSrcCol, DWORD dwTaskID, 
								CKanbanColumnCtrl* pDestCol, const CString& sDestAttribValue)
{
	ASSERT(CanDrag(pSrcCol, pDestCol));
	ASSERT(pSrcCol->FindTask(dwTaskID) != NULL);

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
			pDestCol->AddTask(*pKI, TRUE);
	}
	else
	{
		pKI->AddTrackedAttributeValue(m_sTrackAttribID, sDestAttribValue);

		if (pDestCol->FindTask(dwTaskID) == NULL)
			pDestCol->AddTask(*pKI, TRUE);
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

		const CKanbanColumnCtrl* pDestCol = m_aColumns.HitTest(point);
		BOOL bValidDest = CanDrag(m_pSelectedColumn, pDestCol);

		if (bValidDest)
		{
			BOOL bCopy = Misc::ModKeysArePressed(MKS_CTRL);
			GraphicsMisc::SetDragDropCursor(bCopy ? GMOC_COPY : GMOC_MOVE);
		}
		else
		{
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

LRESULT CKanbanCtrl::OnColumnToggleTaskDone(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(!m_bReadOnly);
	ASSERT(lp);

	DWORD dwTaskID = lp;
	KANBANITEM* pKI = m_data.GetItem(dwTaskID);

	if (pKI)
	{
		BOOL bSetDone = !pKI->IsDone(FALSE);
		BOOL bMod = GetParent()->SendMessage(WM_KBC_EDITTASKDONE, dwTaskID, bSetDone);

		if (bMod)
		{
			// If the app hasn't already updated this for us we must do it ourselves
			if (pKI->IsDone(FALSE) != bSetDone)
			{
				if (bSetDone)
					pKI->dtDone = COleDateTime::GetCurrentTime();
				else
					CDateHelper::ClearDate(pKI->dtDone);

				pKI->bDone = bSetDone;
				m_pSelectedColumn->Invalidate(FALSE);
			}

			if (m_data.HasItem(dwTaskID))
				PostMessage(WM_KCM_SELECTTASK, 0, dwTaskID);
		}

		return bMod;
	}

	// else
	ASSERT(0);
	return 0L;
}

LRESULT CKanbanCtrl::OnColumnEditTaskIcon(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(!m_bReadOnly);
	ASSERT(lp);

	return GetParent()->SendMessage(WM_KBC_EDITTASKICON, (WPARAM)lp);
}

LRESULT CKanbanCtrl::OnColumnToggleTaskFlag(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(!m_bReadOnly);
	ASSERT(lp);

	DWORD dwTaskID = lp;
	const KANBANITEM* pKI = m_data.GetItem(dwTaskID);

	if (pKI)
	{
		LRESULT lr = GetParent()->SendMessage(WM_KBC_EDITTASKFLAG, dwTaskID, !pKI->bFlag);

		if (lr && m_data.HasItem(dwTaskID))
		{
			KANBANITEM* pKI = m_data.GetItem(dwTaskID);
			ASSERT(pKI);

			pKI->bFlag = !pKI->bFlag;

			if (m_pSelectedColumn)
				m_pSelectedColumn->Invalidate();

			PostMessage(WM_KCM_SELECTTASK, 0, dwTaskID);
		}

		return lr;
	}

	// else
	ASSERT(0);
	return 0L;
}

LRESULT CKanbanCtrl::OnSelectTask(WPARAM /*wp*/, LPARAM lp)
{
	return SelectTask(lp);
}

LRESULT CKanbanCtrl::OnColumnGetTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_KBC_GETTASKICON, wp, lp);
}

void CKanbanCtrl::OnColumnSetFocus(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	// Reverse focus changes outside of our own doing
	if (!m_bSettingColumnFocus)
	{
		FixupColumnFocus();
	}
}
