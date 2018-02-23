// KanbanTreeList.cpp: implementation of the CKanbanTreeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KanbanCtrl.h"
#include "KanbanColors.h"
#include "Kanbanlistctrl.h"
#include "KanbanMsg.h"

#include "..\todolist\tdcenum.h"
#include "..\todolist\tdlschemadef.h"

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

#include "..\Interfaces\iuiextension.h"
#include "..\Interfaces\ipreferences.h"

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

const UINT WM_KCM_SELECTTASK = (WM_APP+10); // WPARAM , LPARAM = Task ID

//////////////////////////////////////////////////////////////////////

const UINT IDC_LISTCTRL = 101;

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
	m_bDragging(FALSE),
	m_bReadOnly(FALSE),
	m_nNextColor(0),
	m_pSelectedList(NULL),
	m_nTrackAttribute(IUI_NONE),
	m_nSortBy(IUI_NONE)
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
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTCTRL, OnBeginDragListItem)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL, OnListItemChange)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LISTCTRL, OnListEditLabel)
	ON_NOTIFY(NM_SETFOCUS, IDC_LISTCTRL, OnListSetFocus)
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_KLCN_CHECKCHANGE, OnListCheckChange)
	ON_MESSAGE(WM_KLCN_GETTASKICON, OnListGetTaskIcon)
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
	m_ilHeight.Create(1, 32, ILC_COLOR32, 1, 0);

	return 0;
}

void CKanbanCtrl::FilterToolTipMessage(MSG* pMsg) 
{
	// List tooltips
	CKanbanListCtrl* pList = HitTestListCtrl(pMsg->pt);

	if (pList)
		pList->FilterToolTipMessage(pMsg);
}

bool CKanbanCtrl::ProcessMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		return (HandleKeyDown(pMsg->wParam, pMsg->lParam) != FALSE);

	case WM_LBUTTONDOWN:
		{
			CPoint ptScreen(pMsg->lParam);
			::ClientToScreen(pMsg->hwnd, &ptScreen);

			BOOL bHeader = FALSE;
			CKanbanListCtrl* pList = HitTestListCtrl(ptScreen, &bHeader);

			if (bHeader && pList/* && pList->GetItemCount()*/)
				pList->SetFocus();
		}
		break;
	}
	
	// all else
	return false;
}

BOOL CKanbanCtrl::HandleKeyDown(WPARAM wp, LPARAM lp)
{
	switch (wp)
	{
	case VK_ESCAPE:
		// handle 'escape' during dragging
		return (CancelOperation() != FALSE);

	case VK_DELETE:
		if (m_pSelectedList && !m_pSelectedList->IsBacklog())
		{
			// For each of the selected tasks remove the attribute value(s) of the
			// selected list (column). Tasks having no values remaining are moved 
			// to the backlog
			CDWordArray aTaskIDs;
			int nTask = GetSelectedTaskIDs(aTaskIDs);

			CStringArray aListValues;
			VERIFY(m_pSelectedList->GetAttributeValues(aListValues));

			while (nTask--)
			{
				DWORD dwTaskID = aTaskIDs[nTask];
				KANBANITEM* pKI = GetKanbanItem(dwTaskID);
				ASSERT(pKI);

				if (pKI)
					pKI->RemoveTrackedAttributeValues(m_sTrackAttribID, aListValues);
			}

			// Notify parent of changes before altering the lists because we can't
			// guarantee that all the modified tasks will be in the same list afterwards
			NotifyParentAttibuteChange(aTaskIDs);

			// Reset selected list before removing items to 
			// to prevent unwanted selection notifications
			CKanbanListCtrl* pList = m_pSelectedList;
			m_pSelectedList = NULL;

			nTask = aTaskIDs.GetSize();

			while (nTask--)
			{
				DWORD dwTaskID = aTaskIDs[nTask];
				KANBANITEM* pKI = GetKanbanItem(dwTaskID);
				ASSERT(pKI);

				if (pKI)
				{
					VERIFY(pList->DeleteTask(dwTaskID));

					if (!pKI->HasTrackedAttributeValues(m_sTrackAttribID))
					{
						CKanbanListCtrl* pBacklog = m_aListCtrls.GetBacklog();

						if (pBacklog)
							pBacklog->AddTask(*pKI, FALSE);
					}
				}
			}

			// try to restore selection
			SelectTasks(aTaskIDs);
			return TRUE;
		}
		break;
	}

	// all else
	return FALSE;
}

int CKanbanCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs) const
{
	const CKanbanListCtrl* pList = GetSelListCtrl();

	if (pList)
		return pList->GetSelectedTasks(aTaskIDs);

	// else
	aTaskIDs.RemoveAll();
	return 0;
}

CKanbanListCtrl* CKanbanCtrl::GetSelListCtrl()
{
	ASSERT((m_pSelectedList == NULL) || Misc::HasT(m_aListCtrls, m_pSelectedList));

	if (!m_pSelectedList && m_aListCtrls.GetSize())
		m_pSelectedList = m_aListCtrls[0];

	return m_pSelectedList;
}

const CKanbanListCtrl* CKanbanCtrl::GetSelListCtrl() const
{
	if (m_pSelectedList)
	{
		return m_pSelectedList;
	}
	else if (m_aListCtrls.GetSize())
	{
		return m_aListCtrls[0];
	}

	// else
	return NULL;
}

BOOL CKanbanCtrl::SelectTasks(const CDWordArray& aTaskIDs)
{
	// Check for 'no change'
	CDWordArray aSelTaskIDs;
	GetSelectedTaskIDs(aSelTaskIDs);

	if (Misc::MatchAll(aSelTaskIDs, aTaskIDs))
		return TRUE;
	
	ClearOtherListSelections(NULL);

	if (aTaskIDs.GetSize() == 0)
	{
		return FALSE;
	}

	// find the list containing the first item and then
	// check that all the other items belong to the same list
	int nItem = -1;
	CKanbanListCtrl* pList = LocateTask(aTaskIDs[0], nItem, TRUE);

	if (pList && (nItem != -1))
	{
		if (!pList->SelectTasks(aTaskIDs))
			pList = NULL;
	}
	
	ClearOtherListSelections(pList);

	if (pList)
	{
		m_pSelectedList = pList;
		FixupFocus();

		ScrollToSelectedTask();
	}
	
	return (pList != NULL);
}

BOOL CKanbanCtrl::SelectTask(DWORD dwTaskID)
{
	CDWordArray aTaskIDs;
	aTaskIDs.Add(dwTaskID);

	return SelectTasks(aTaskIDs);
/*
	// Check for 'no change'
	CDWordArray aSelTaskIDs;
	GetSelectedTaskIDs(aSelTaskIDs);

	if ((aSelTaskIDs.GetSize() == 1) && (aSelTaskIDs[0] == dwTaskID))
		return TRUE;
	
	int nItem = -1;
	CKanbanListCtrl* pList = LocateTask(dwTaskID, nItem, TRUE);

	if (pList && (nItem != -1))
	{
		m_pSelectedList = pList;
		pList->SelectItem(nItem, TRUE);

		FixupFocus();

		ScrollToSelectedTask();
		ClearOtherListSelections(pList);
	}

	return (pList != NULL);
*/
}

BOOL CKanbanCtrl::SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select)
{
	CKanbanListCtrl* pList = NULL;
	int nStartItem = -1;
	BOOL bForwards = TRUE;

	switch (nCmd)
	{
	case IUI_SELECTFIRSTTASK:
		pList = m_aListCtrls.GetFirstNonEmpty();
		nStartItem = 0;
		break;

	case IUI_SELECTNEXTTASK:
		pList = m_pSelectedList;
		nStartItem = (pList->GetLastSelectedItem() + 1);
		break;

	case IUI_SELECTNEXTTASKINCLCURRENT:
		pList = m_pSelectedList;
		nStartItem = pList->GetLastSelectedItem();
		break;

	case IUI_SELECTPREVTASK:
		pList = m_pSelectedList;
		nStartItem = (pList->GetFirstSelectedItem() - 1);
		bForwards = FALSE;
		break;

	case IUI_SELECTLASTTASK:
		pList = m_aListCtrls.GetLastNonEmpty();
		nStartItem = (pList->GetItemCount() - 1);
		bForwards = FALSE;
		break;

	default:
		ASSERT(0);
		break;
	}

	if (pList)
	{
		const CKanbanListCtrl* pStartList = pList;
		int nItem = nStartItem;
		
		if (bForwards)
			nItem = max(0, nItem);
		else
			nItem = min(nItem, (pList->GetItemCount() - 1));

		do
		{
			nItem = pList->FindTask(select, bForwards, nItem);

			if (nItem != -1)
			{
				SelectListCtrl(pList, FALSE);
				return pList->SelectItem(nItem, FALSE);
			}

			// else
			pList = GetNextListCtrl(pList, bForwards, TRUE);
			nItem = (bForwards ? 0 : (pList->GetItemCount() - 1));
		}
		while (pList != pStartList);
	}

	// all else
	return false;
}

BOOL CKanbanCtrl::HasFocus() const
{
	return CDialogHelper::IsChildOrSame(GetSafeHwnd(), ::GetFocus());
}

void CKanbanCtrl::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib)
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
		{
 			CDWordArray aSelIDs;
			GetSelectedTaskIDs(aSelIDs);

 			RebuildData(pTasks, attrib);
 			RebuildListCtrls(TRUE);

			SelectTasks(aSelIDs);

			if (aSelIDs.GetSize())
 				ScrollToSelectedTask();
		}
		break;
		
	case IUI_NEW:
	case IUI_EDIT:
		{
			CDWordArray aSelIDs;
			GetSelectedTaskIDs(aSelIDs);
			
 			// update the task(s)
			BOOL bChange = UpdateGlobalAttributeValues(pTasks, attrib);
			bChange |= UpdateData(pTasks, pTasks->GetFirstTask(), attrib, TRUE);

			if (bChange)
				RebuildListCtrls(TRUE);
			else
				RedrawListCtrls(TRUE);
			
			SelectTasks(aSelIDs);
			
			if (aSelIDs.GetSize())
				ScrollToSelectedTask();
		}
		break;
		
	case IUI_DELETE:
		{
 			RemoveDeletedTasks(pTasks);
		}
		break;
		
	default:
		ASSERT(0);
	}
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
BOOL CKanbanCtrl::WantEditUpdate(IUI_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case IUI_ALLOCBY:
	case IUI_ALLOCTO:
	case IUI_CATEGORY:
	case IUI_COLOR:
	case IUI_COST:
	case IUI_CREATIONDATE:
	case IUI_CREATEDBY:
	case IUI_CUSTOMATTRIB:
	case IUI_DONEDATE:
	case IUI_DUEDATE:
	case IUI_EXTERNALID:
	case IUI_FILEREF:
	case IUI_FLAG:
	case IUI_ICON:
	case IUI_LASTMOD:
	case IUI_PERCENT:
	case IUI_PRIORITY:
	case IUI_RECURRENCE:
	case IUI_RISK:
	case IUI_STARTDATE:
	case IUI_STATUS:
	case IUI_SUBTASKDONE:
	case IUI_TAGS:
	case IUI_TASKNAME:
	case IUI_TIMEEST:
	case IUI_TIMESPENT:
	case IUI_VERSION:
		return TRUE;
	}
	
	// all else 
	return (nAttrib == IUI_ALL);
}

// External interface
BOOL CKanbanCtrl::WantSortUpdate(IUI_ATTRIBUTE nAttrib)
{
	return WantEditUpdate(nAttrib);
}

BOOL CKanbanCtrl::RebuildData(const ITASKLISTBASE* pTasks, const CSet<IUI_ATTRIBUTE>& attrib)
{
	// Rebuild global attribute value lists
	m_mapAttributeValues.RemoveAll();
	m_aCustomAttribDefs.RemoveAll();

	UpdateGlobalAttributeValues(pTasks, attrib);

	// Rebuild data
	m_data.RemoveAll();

	return AddTaskToData(pTasks, pTasks->GetFirstTask(), 0, attrib, TRUE);
}

BOOL CKanbanCtrl::AddTaskToData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, DWORD dwParentID, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings)
{
	if (!hTask)
		return FALSE;

	if (pTasks->IsTaskReference(hTask))
		return FALSE;

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

	pKI->SetColor(pTasks->GetTaskTextColor(hTask));

	LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
	pKI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));
	
	// Path is parent's path + parent's name
	if (dwParentID)
	{
		const KANBANITEM* pKIParent = m_data.GetItem(dwParentID);
		ASSERT(pKIParent);

		if (pKIParent->sPath.IsEmpty())
			pKI->sPath = pKIParent->sTitle;
		else
			pKI->sPath = pKIParent->sPath + '\\' + pKIParent->sTitle;

		pKI->nLevel = (pKIParent->nLevel + 1);
	}
	else
	{
		pKI->nLevel = 0;
	}

	// trackable attributes
	CStringArray aValues;

	if (GetTaskCategories(pTasks, hTask, aValues))
		pKI->SetTrackedAttributeValues(IUI_CATEGORY, aValues);

	if (GetTaskAllocTo(pTasks, hTask, aValues))
		pKI->SetTrackedAttributeValues(IUI_ALLOCTO, aValues);

	if (GetTaskTags(pTasks, hTask, aValues))
		pKI->SetTrackedAttributeValues(IUI_TAGS, aValues);
	
	pKI->SetTrackedAttributeValue(IUI_STATUS, pTasks->GetTaskStatus(hTask));
	pKI->SetTrackedAttributeValue(IUI_ALLOCBY, pTasks->GetTaskAllocatedBy(hTask));
	pKI->SetTrackedAttributeValue(IUI_VERSION, pTasks->GetTaskVersion(hTask));
	pKI->SetTrackedAttributeValue(IUI_PRIORITY, pTasks->GetTaskPriority(hTask, FALSE));
	pKI->SetTrackedAttributeValue(IUI_RISK, pTasks->GetTaskRisk(hTask, FALSE));

	// custom attributes
	int nCust = pTasks->GetCustomAttributeCount();

	while (nCust--)
	{
		CString sCustID(pTasks->GetCustomAttributeID(nCust));
		CString sCustValue(pTasks->GetTaskCustomAttributeData(hTask, sCustID, true));

		CStringArray aCustValues;
		Misc::Split(sCustValue, aCustValues, '\n');

		pKI->SetTrackedAttributeValues(sCustID, aCustValues);

		// Add to global attribute values
		CKanbanValueMap* pValues = m_mapAttributeValues.GetAddMapping(sCustID);
		ASSERT(pValues);
			
		pValues->AddValue(sCustValue);
	}

	// Other display-only attributes
	UpdateItemDisplayAttributes(pKI, pTasks, hTask, attrib);

	// first child
	AddTaskToData(pTasks, pTasks->GetFirstTask(hTask), dwTaskID, attrib, TRUE);

	// Siblings NON-RECURSIVELY
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);

		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			AddTaskToData(pTasks, hSibling, dwParentID, attrib, FALSE);
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}

	return TRUE;
}

BOOL CKanbanCtrl::UpdateData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings)
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
			bChange = AddTaskToData(pTasks, hTask, pTasks->GetTaskParentID(hTask), attrib, FALSE);
		}
		else
		{
			KANBANITEM* pKI = NULL;
			GET_KI_RET(dwTaskID, pKI, FALSE);

			if (attrib.Has(IUI_TASKNAME))
				pKI->sTitle = pTasks->GetTaskTitle(hTask);
			
			if (attrib.Has(IUI_DONEDATE))
			{
				BOOL bDone = pTasks->IsTaskDone(hTask);
				BOOL bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);

				if ((pKI->bDone != bDone) || (pKI->bGoodAsDone != bGoodAsDone))
				{
					pKI->bDone = bDone;
					pKI->bGoodAsDone = bGoodAsDone;
				}
			}

			if (attrib.Has(IUI_SUBTASKDONE))
			{
				LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
				pKI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));
			}

			if (attrib.Has(IUI_ICON))
				pKI->bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

			if (attrib.Has(IUI_FLAG))
				pKI->bFlag = (pTasks->IsTaskFlagged(hTask, true) ? TRUE : FALSE);
			
			// Trackable attributes
			CStringArray aValues;

			if (attrib.Has(IUI_ALLOCTO))
			{
				GetTaskAllocTo(pTasks, hTask, aValues);
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_ALLOCTO, aValues);
			}

			if (attrib.Has(IUI_CATEGORY))
			{
				GetTaskCategories(pTasks, hTask, aValues);
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_CATEGORY, aValues);
			}

			if (attrib.Has(IUI_TAGS))
			{
				GetTaskTags(pTasks, hTask, aValues);
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_TAGS, aValues);
			}

			if (attrib.Has(IUI_ALLOCBY))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_ALLOCBY, pTasks->GetTaskAllocatedBy(hTask));

			if (attrib.Has(IUI_STATUS))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_STATUS, pTasks->GetTaskStatus(hTask));

			if (attrib.Has(IUI_VERSION))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_VERSION, pTasks->GetTaskVersion(hTask));

			if (attrib.Has(IUI_PRIORITY))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_PRIORITY, pTasks->GetTaskPriority(hTask, true));

			if (attrib.Has(IUI_RISK))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_RISK, pTasks->GetTaskRisk(hTask, true));

			if (attrib.Has(IUI_CUSTOMATTRIB))
			{
				int nID = m_aCustomAttribDefs.GetSize();

				while (nID--)
				{
					KANBANCUSTOMATTRIBDEF& def = m_aCustomAttribDefs[nID];

					CString sValue(pTasks->GetTaskCustomAttributeData(hTask, def.sAttribID, true));
					CStringArray aValues;

					if (!sValue.IsEmpty())
					{
						if (Misc::Split(sValue, aValues, '\n') > 1)
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
			UpdateItemDisplayAttributes(pKI, pTasks, hTask, attrib);
			
			// always update colour because it can change for so many reasons
			pKI->SetColor(pTasks->GetTaskTextColor(hTask));

			// Always update lock state
			pKI->bLocked = pTasks->IsTaskLocked(hTask, true);
		}
	}
		
	// children
	if (UpdateData(pTasks, pTasks->GetFirstTask(hTask), attrib, TRUE))
		bChange = TRUE;

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			if (UpdateData(pTasks, hSibling, attrib, FALSE))
				bChange = TRUE;
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
	
	return bChange;
}

void CKanbanCtrl::UpdateItemDisplayAttributes(KANBANITEM* pKI, const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib)
{
	time64_t tDate = 0;
	
	if (attrib.Has(IUI_TIMEEST))
		pKI->dTimeEst = pTasks->GetTaskTimeEstimate(hTask, pKI->nTimeEstUnits, true);
	
	if (attrib.Has(IUI_TIMESPENT))
		pKI->dTimeSpent = pTasks->GetTaskTimeSpent(hTask, pKI->nTimeSpentUnits, true);
	
	if (attrib.Has(IUI_COST))
		pKI->dCost = pTasks->GetTaskCost(hTask, true);
	
	if (attrib.Has(IUI_CREATEDBY))
		pKI->sCreatedBy = pTasks->GetTaskCreatedBy(hTask);
	
	if (attrib.Has(IUI_CREATIONDATE))
		pKI->dtCreate = pTasks->GetTaskCreationDate(hTask);
	
	if (attrib.Has(IUI_DONEDATE) && pTasks->GetTaskDoneDate64(hTask, tDate))
		pKI->dtDone = CDateHelper::GetDate(tDate);
	
	if (attrib.Has(IUI_DUEDATE) && pTasks->GetTaskDueDate64(hTask, true, tDate))
		pKI->dtDue = CDateHelper::GetDate(tDate);
	
	if (attrib.Has(IUI_STARTDATE) && pTasks->GetTaskStartDate64(hTask, true, tDate))
		pKI->dtStart = CDateHelper::GetDate(tDate);
	
	if (attrib.Has(IUI_LASTMOD) && pTasks->GetTaskLastModified64(hTask, tDate))
		pKI->dtLastMod = CDateHelper::GetDate(tDate);
	
	if (attrib.Has(IUI_PERCENT))
		pKI->nPercent = pTasks->GetTaskPercentDone(hTask, true);
	
	if (attrib.Has(IUI_EXTERNALID))
		pKI->sExternalID = pTasks->GetTaskExternalID(hTask);
	
	if (attrib.Has(IUI_RECURRENCE))
		pKI->sRecurrence = pTasks->GetTaskAttribute(hTask, TDL_TASKRECURRENCE);

	if (attrib.Has(IUI_FILEREF) && pTasks->GetTaskFileLinkCount(hTask))
	{
		pKI->sFileRef = pTasks->GetTaskFileLink(hTask, 0);

		// Get the shortest meaningful bit because of space constraints
		if (FileMisc::IsPath(pKI->sFileRef))
			pKI->sFileRef = FileMisc::GetFileNameFromPath(pKI->sFileRef);
	}
}

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks, const CSet<IUI_ATTRIBUTE>& attrib)
{
	BOOL bChange = FALSE;

	if (attrib.Has(IUI_STATUS))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_STATUS);
	
	if (attrib.Has(IUI_ALLOCTO))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_ALLOCTO);
	
	if (attrib.Has(IUI_CATEGORY))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_CATEGORY);
	
	if (attrib.Has(IUI_ALLOCBY))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_ALLOCBY);
	
	if (attrib.Has(IUI_TAGS))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_TAGS);
	
	if (attrib.Has(IUI_VERSION))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_VERSION);
	
	if (attrib.Has(IUI_PRIORITY))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_PRIORITY);
	
	if (attrib.Has(IUI_RISK))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_RISK);
	
	if (attrib.Has(IUI_CUSTOMATTRIB))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_CUSTOMATTRIB);
	
	return bChange;
}

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks, IUI_ATTRIBUTE nAttribute)
{
	switch (nAttribute)
	{
	case IUI_PRIORITY:
	case IUI_RISK:
		{
			// create once only
			CString sAttribID(KANBANITEM::GetAttributeID(nAttribute));

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
		
	case IUI_STATUS:
	case IUI_ALLOCTO:
	case IUI_ALLOCBY:
	case IUI_CATEGORY:
	case IUI_VERSION:
	case IUI_TAGS:	
		{
			CString sXMLTag(GetXMLTag(nAttribute)); 
			CString sAttribID(KANBANITEM::GetAttributeID(nAttribute));

			return UpdateGlobalAttributeValues(pTasks, sXMLTag, sAttribID);
		}
		
	case IUI_CUSTOMATTRIB:
		{
			int nCustom = pTasks->GetCustomAttributeCount();

			while (nCustom--)
			{
				// Save off each attribute ID
				if (pTasks->IsCustomAttributeEnabled(nCustom))
				{
					CString sAttribID(pTasks->GetCustomAttributeID(nCustom));
					int nDef = m_aCustomAttribDefs.AddDefinition(sAttribID);

					// Add non-empty values to the map
					CKanbanValueMap* pDefValues = m_mapGlobalAttributeValues.GetAddMapping(sAttribID);
					ASSERT(pDefValues);

					pDefValues->RemoveAll();

					CString sDefValues = pTasks->GetCustomAttributeListData(nCustom);
					CStringArray aDefValues;

					if (Misc::Split(sDefValues, aDefValues, '\n'))
					{
						pDefValues->SetValues(aDefValues);

						if (aDefValues.GetSize() > 1)
							m_aCustomAttribDefs.SetMultiValue(nDef);
					}
								
					// Add backlog item
					pDefValues->AddValue(EMPTY_STR);
				}
			}
		}
		break;
	}

	// all else
	return FALSE;
}

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks, LPCTSTR szXMLTag, LPCTSTR szAttribID)
{
	CKanbanValueMap mapNewValues;
	
	int nValue = pTasks->GetAttributeCount(szXMLTag);

	while (nValue--)
	{
		CString sValue(pTasks->GetAttributeItem(szXMLTag, nValue));

		if (!sValue.IsEmpty())
			mapNewValues.AddValue(sValue);
	}

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
		return TRUE;
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

int CKanbanCtrl::GetAttributeValues(IUI_ATTRIBUTE nAttrib, CStringArray& aValues) const
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

int CKanbanCtrl::GetCustomAttributeIDs(CStringArray& aCustAttribIDs) const
{
	return m_aCustomAttribDefs.GetAttributeIDs(aCustAttribIDs);
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

	if (m_nTrackAttribute != IUI_NONE)
		RebuildListCtrls(FALSE);
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

CString CKanbanCtrl::GetXMLTag(IUI_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case IUI_ALLOCTO:	return TDL_TASKALLOCTO;
	case IUI_ALLOCBY:	return TDL_TASKALLOCBY;
	case IUI_STATUS:	return TDL_TASKSTATUS;
	case IUI_CATEGORY:	return TDL_TASKCATEGORY;
	case IUI_VERSION:	return TDL_TASKVERSION;
	case IUI_TAGS:		return TDL_TASKTAG;
		
	case IUI_CUSTOMATTRIB:
		ASSERT(0);
		break;
		
	default:
		ASSERT(0);
		break;
	}
	
	return EMPTY_STR;
}

BOOL CKanbanCtrl::UpdateTrackableTaskAttribute(KANBANITEM* pKI, IUI_ATTRIBUTE nAttrib, int nNewValue)
{
#ifdef _DEBUG
	switch (nAttrib)
	{
	case IUI_PRIORITY:
	case IUI_RISK:
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
	case IUI_PRIORITY:
	case IUI_RISK:
	case IUI_ALLOCBY:
	case IUI_STATUS:
	case IUI_VERSION:
		return FALSE;

	case IUI_ALLOCTO:
	case IUI_CATEGORY:
	case IUI_TAGS:
		return TRUE;

	case IUI_CUSTOMATTRIB:
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

BOOL CKanbanCtrl::UpdateTrackableTaskAttribute(KANBANITEM* pKI, IUI_ATTRIBUTE nAttrib, const CString& sNewValue)
{
#ifdef _DEBUG
	switch (nAttrib)
	{
	case IUI_PRIORITY:
	case IUI_RISK:
	case IUI_ALLOCBY:
	case IUI_STATUS:
	case IUI_VERSION:
		break;

	default:
		ASSERT(0);
		break;
	}
#endif

	CStringArray aNewValues;

	if (!sNewValue.IsEmpty())
		aNewValues.Add(sNewValue);

	return UpdateTrackableTaskAttribute(pKI, KANBANITEM::GetAttributeID(nAttrib), aNewValues);
}

BOOL CKanbanCtrl::UpdateTrackableTaskAttribute(KANBANITEM* pKI, IUI_ATTRIBUTE nAttrib, const CStringArray& aNewValues)
{
#ifdef _DEBUG
	switch (nAttrib)
	{
	case IUI_ALLOCTO:
	case IUI_CATEGORY:
	case IUI_TAGS:
		break;

	default:
		ASSERT(0);
		break;
	}
#endif

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
		
		while (nVal--)
		{
			if (!Misc::Contains(aNewValues, aCurValues[nVal]))
			{
				CKanbanListCtrl* pCurList = GetListCtrl(aCurValues[nVal]);
				ASSERT(pCurList);
				
				if (pCurList)
				{
					VERIFY(pCurList->DeleteTask(pKI->dwTaskID));
					bChange |= (pCurList->GetItemCount() == 0);
				}
				
				// Remove from list to speed up later searching
				aCurValues.RemoveAt(nVal);
			}
		}
		
		// Add any new items not in the current list
		nVal = aNewValues.GetSize();
		
		while (nVal--)
		{
			if (!Misc::Contains(aCurValues, aNewValues[nVal]))
			{
				CKanbanListCtrl* pCurList = GetListCtrl(aNewValues[nVal]);
				
				if (pCurList)
					pCurList->AddTask(*pKI, FALSE);
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
	if (HasOption(KBCF_SHOWEMPTYCOLUMNS) && (m_nTrackAttribute != IUI_CUSTOMATTRIB))
		return TRUE;

	if (HasOption(KBCF_ALWAYSSHOWBACKLOG) && Misc::IsEmpty(szValue))
		return TRUE;

	// else
	const CKanbanItemArray* pKIArr = mapKIArray.GetMapping(szValue);
		
	return (pKIArr && pKIArr->GetSize());
}

BOOL CKanbanCtrl::WantShowColumn(const CKanbanListCtrl* pList) const
{
	if (HasOption(KBCF_SHOWEMPTYCOLUMNS))
		return TRUE;

	if (HasOption(KBCF_ALWAYSSHOWBACKLOG) && pList->IsBacklog())
		return TRUE;

	return (pList->GetItemCount() > 0);
}

void CKanbanCtrl::RedrawListCtrls(BOOL bErase)
{
	m_aListCtrls.Redraw(bErase);
}

BOOL CKanbanCtrl::DeleteListCtrl(int nList)
{
	if ((nList < 0) || (nList >= m_aListCtrls.GetSize()))
	{
		ASSERT(0);
		return FALSE;
	}

	CKanbanListCtrl* pList = m_aListCtrls[nList];
	ASSERT(pList);

	if (pList == m_pSelectedList)
		m_pSelectedList = NULL;

	m_aListCtrls.RemoveAt(nList);

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

int CKanbanCtrl::RemoveOldDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingDynamicColumns())
	{
		ASSERT(0);
		return 0;
	}

	// remove any lists whose values are no longer used
	// or which Optionally have no items 
	const CKanbanValueMap* pGlobals = m_mapAttributeValues.GetMapping(m_sTrackAttribID);
	int nList = m_aListCtrls.GetSize(), nNumRemoved = 0;
	
	while (nList--)
	{
		CKanbanListCtrl* pList = m_aListCtrls[nList];
		ASSERT(pList && !pList->HasMultipleValues());
		
		if ((pGlobals == NULL) || !WantShowColumn(pList))
		{
			DeleteListCtrl(nList);
			nNumRemoved++;
		}
		else
		{
			CString sAttribValueID(pList->GetAttributeValueID());
			
			if (!Misc::HasKey(*pGlobals, sAttribValueID) || 
				!WantShowColumn(sAttribValueID, mapKIArray))
			{
				DeleteListCtrl(nList);
				nNumRemoved++;
			}
		}
	}

	return nNumRemoved;
}

int CKanbanCtrl::AddMissingDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray)
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
			
			CKanbanListCtrl* pList = GetListCtrl(sAttribValueID);
			
			if ((pList == NULL) && WantShowColumn(sAttribValueID, mapKIArray))
			{
				KANBANCOLUMN colDef;
				
				colDef.sAttribID = m_sTrackAttribID;
				colDef.sTitle = sAttribValue;
				colDef.aAttribValues.Add(sAttribValue);
				//colDef.crBackground = KBCOLORS[m_nNextColor++ % NUM_KBCOLORS];
				
				VERIFY (AddNewListCtrl(colDef) != NULL);
				nNumAdded++;
			}
		}

		ASSERT(!HasOption(KBCF_SHOWEMPTYCOLUMNS) || 
				(m_nTrackAttribute == IUI_CUSTOMATTRIB) ||
				(m_aListCtrls.GetSize() == pGlobals->GetCount()));
	}

	return nNumAdded;
}

void CKanbanCtrl::RebuildDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingDynamicColumns())
	{
		ASSERT(0);
		return;
	}
	
	BOOL bNeedResize = RemoveOldDynamicListCtrls(mapKIArray);
	bNeedResize |= AddMissingDynamicListCtrls(mapKIArray);

	// If no columns created, create empty Backlog column
	bNeedResize |= CheckAddBacklogListCtrl();
	
	// (Re)sort
	m_aListCtrls.Sort();

	if (bNeedResize)
		Resize();
}

void CKanbanCtrl::RebuildFixedListCtrls(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingFixedColumns())
	{
		ASSERT(0);
		return;
	}

	if (m_aListCtrls.GetSize() == 0) // first time only
	{
		for (int nDef = 0; nDef < m_aColumnDefs.GetSize(); nDef++)
		{
			const KANBANCOLUMN& colDef = m_aColumnDefs[nDef];
			VERIFY(AddNewListCtrl(colDef) != NULL);
		}

		Resize(); // always
	}
}

BOOL CKanbanCtrl::CheckAddBacklogListCtrl()
{
	if (m_aListCtrls.GetSize() == 0) 
	{
		KANBANCOLUMN colDef;
		
		colDef.sAttribID = m_sTrackAttribID;
		colDef.aAttribValues.Add(_T(""));
		
		VERIFY (AddNewListCtrl(colDef) != NULL);
		return TRUE;
	}

	return FALSE;
}

void CKanbanCtrl::RebuildListCtrls(BOOL bRebuildData)
{
	if (m_sTrackAttribID.IsEmpty())
	{
		ASSERT(m_nTrackAttribute == IUI_NONE);
		return;
	}

	CHoldRedraw gr(*this, NCR_PAINT | NCR_ERASEBKGND);
	
	CKanbanItemArrayMap mapKIArray;
	m_data.BuildTempItemMaps(m_sTrackAttribID, mapKIArray);

	if (UsingDynamicColumns())
		RebuildDynamicListCtrls(mapKIArray);
	else
		RebuildFixedListCtrls(mapKIArray);

	// Rebuild the list data for each list (which can be empty)
	if (bRebuildData)
	{
		RebuildListCtrlData(mapKIArray);
	}
	else if (UsingDynamicColumns())
	{
		// If not rebuilding the data (which will remove lists
		// which are empty as consequence of not showing parent
		// tasks) we made need to remove such lists.
		RemoveOldDynamicListCtrls(mapKIArray);
	}

	Resize();
	FixupSelection();
}

void CKanbanCtrl::RebuildListCtrlData(const CKanbanItemArrayMap& mapKIArray)
{
	BOOL bShowParents = HasOption(KBCF_SHOWPARENTTASKS);
	int nList = m_aListCtrls.GetSize();
	
	while (nList--)
	{
		CKanbanListCtrl* pList = m_aListCtrls[nList];
		ASSERT(pList);
		
		RebuildListContents(pList, mapKIArray, bShowParents);
		
		// The list can still end up empty if parent tasks are 
		// omitted in Dynamic columns so we recheck and delete if required
		if (UsingDynamicColumns())
		{
			if (!bShowParents && !WantShowColumn(pList))
			{
				DeleteListCtrl(nList);
			}
		}
	}
		
	// Lists can still end up empty if there were 
	// only unwanted parents
	CheckAddBacklogListCtrl();

	// Resort without toggling
	Sort(m_nSortBy, FALSE, m_bSortAscending);
}

void CKanbanCtrl::FixupSelection()
{
	ASSERT(m_aListCtrls.GetSize());

	// Make sure selected list is valid
	if (!m_pSelectedList || !Misc::HasT(m_aListCtrls, m_pSelectedList))
	{
		// Find the first list with some items
		m_pSelectedList = m_aListCtrls.GetFirstNonEmpty();

		// No list has items?
		if (!m_pSelectedList)
			m_pSelectedList = m_aListCtrls[0];
	}

	FixupFocus();
}

void CKanbanCtrl::FixupFocus()
{
	if (IsWindowVisible() && HasFocus())
	{
		m_pSelectedList->SetFocus();
		m_pSelectedList->Invalidate(TRUE);
	}
}

IUI_ATTRIBUTE CKanbanCtrl::GetTrackedAttribute(CString& sCustomAttrib) const
{
	if (m_nTrackAttribute == IUI_CUSTOMATTRIB)
		sCustomAttrib = m_sTrackAttribID;
	else
		sCustomAttrib.Empty();

	return m_nTrackAttribute;
}

BOOL CKanbanCtrl::TrackAttribute(IUI_ATTRIBUTE nAttrib, const CString& sCustomAttribID, 
								 const CKanbanColumnArray& aColumnDefs)
{
	// validate input and check for changes
	BOOL bChange = (nAttrib != m_nTrackAttribute);

	switch (nAttrib)
	{
	case IUI_STATUS:
	case IUI_ALLOCTO:
	case IUI_ALLOCBY:
	case IUI_CATEGORY:
	case IUI_VERSION:
	case IUI_PRIORITY:
	case IUI_RISK:
	case IUI_TAGS:
		break;
		
	case IUI_CUSTOMATTRIB:
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
				ASSERT(nCol == m_aListCtrls.GetSize());

				while (nCol--)
				{
					const KANBANCOLUMN& colDef = aColumnDefs[nCol];
					CKanbanListCtrl* pList = m_aListCtrls[nCol];
					ASSERT(pList);

					if (pList)
					{
						pList->SetBackgroundColor(colDef.crBackground);
						//pList->SetExcessColor(colDef.crExcess);
						//pList->SetMaximumTaskCount(colDef.nMaxTaskCount);
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
	case IUI_STATUS:
	case IUI_ALLOCTO:
	case IUI_ALLOCBY:
	case IUI_CATEGORY:
	case IUI_VERSION:
	case IUI_PRIORITY:
	case IUI_RISK:
	case IUI_TAGS:
		m_sTrackAttribID = KANBANITEM::GetAttributeID(nAttrib);
		break;
		
	case IUI_CUSTOMATTRIB:
		m_sTrackAttribID = sCustomAttribID;
		break;
	}

	// delete all lists and start over
	CHoldRedraw gr(*this, NCR_PAINT | NCR_ERASEBKGND);

	m_pSelectedList = NULL;
	m_aListCtrls.RemoveAll();

	RebuildListCtrls(TRUE);
	Resize();

	return TRUE;
}

CKanbanListCtrl* CKanbanCtrl::AddNewListCtrl(const KANBANCOLUMN& colDef)
{
	CKanbanListCtrl* pList = new CKanbanListCtrl(m_data, colDef, m_fonts, m_aPriorityColors, m_aDisplayAttrib);
	ASSERT(pList);

	if (pList)
	{
		pList->SetTextColorIsBackground(HasOption(KBCF_TASKTEXTCOLORISBKGND));
		pList->SetStrikeThruDoneTasks(HasOption(KBCF_STRIKETHRUDONETASKS));
		pList->SetColorTaskBarByPriority(HasOption(KBCF_COLORBARBYPRIORITY));
		pList->SetShowTaskColorAsBar(HasOption(KBCF_SHOWTASKCOLORASBAR));
		pList->SetShowCompletionCheckboxes(HasOption(KBCF_SHOWCOMPLETIONCHECKBOXES));

		if (pList->Create(IDC_LISTCTRL, this))
		{
			m_aListCtrls.Add(pList);
		}
		else
		{
			delete pList;
			pList = NULL;
		}
	}
	
	return pList;
}

BOOL CKanbanCtrl::RebuildListContents(CKanbanListCtrl* pList, const CKanbanItemArrayMap& mapKIArray, BOOL bShowParents)
{
	ASSERT(pList && pList->GetSafeHwnd());

	if (!pList || !pList->GetSafeHwnd())
		return FALSE;

	CDWordArray aSelTaskIDs;

	pList->GetSelectedTasks(aSelTaskIDs);
	pList->SetRedraw(FALSE);
	pList->DeleteAllItems();

	CStringArray aValueIDs;
	int nNumVals = pList->GetAttributeValueIDs(aValueIDs);

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
					BOOL bSelected = Misc::HasT(aSelTaskIDs, pKI->dwTaskID);

					VERIFY(pList->AddTask(*pKI, bSelected) != -1);
				}
			}
		}
	}
	
	pList->SetRedraw(TRUE);

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

	m_aListCtrls.RemoveDeletedTasks(mapIDs);
	m_data.RemoveDeletedItems(mapIDs);
}

KANBANITEM* CKanbanCtrl::GetKanbanItem(DWORD dwTaskID) const
{
	return m_data.GetItem(dwTaskID);
}

BOOL CKanbanCtrl::HasKanbanItem(DWORD dwTaskID) const
{
	return m_data.HasItem(dwTaskID);
}

CKanbanListCtrl* CKanbanCtrl::LocateTask(DWORD dwTaskID, int& nItem, BOOL bForward) const
{
	// First try selected list
	if (m_pSelectedList)
	{
		nItem = m_pSelectedList->FindTask(dwTaskID);

		if (nItem != -1)
			return m_pSelectedList;
	}

	// try any other list in the specified direction
	const CKanbanListCtrl* pList = GetNextListCtrl(m_pSelectedList, bForward, TRUE);

	if (!pList)
		return NULL;

	const CKanbanListCtrl* pStartList = pList;

	do
	{
		nItem = pList->FindTask(dwTaskID);

		if (nItem != -1)
			return const_cast<CKanbanListCtrl*>(pList);

		// else
		pList = GetNextListCtrl(pList, bForward, TRUE);
	}
	while (pList != pStartList);

	return NULL;
}

CKanbanListCtrl* CKanbanCtrl::GetListCtrl(const CString& sAttribValue) const
{
	return m_aListCtrls.Get(sAttribValue);
}

CKanbanListCtrl* CKanbanCtrl::GetListCtrl(HWND hwnd) const
{
	return m_aListCtrls.Get(hwnd);
}

void CKanbanCtrl::SetDisplayAttributes(const CKanbanAttributeArray& aAttrib)
{
	if (!Misc::MatchAllT(m_aDisplayAttrib, aAttrib, FALSE))
	{
		m_aDisplayAttrib.Copy(aAttrib);
		m_aListCtrls.OnDisplayAttributeChanged();

		// Update list attribute label visibility
		if (m_aDisplayAttrib.GetSize())
			Resize();
	}
}

int CKanbanCtrl::GetDisplayAttributes(CKanbanAttributeArray& aAttrib) const
{
	aAttrib.Copy(m_aDisplayAttrib);
	return aAttrib.GetSize();
}

void CKanbanCtrl::SetOption(DWORD dwOption, BOOL bSet)
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
			case KBCF_SHOWPARENTTASKS:
				RebuildListCtrls(TRUE);
				break;

			case KBCF_SORTSUBTASTASKSBELOWPARENTS:
				if (m_nSortBy != IUI_NONE)
					Sort(m_nSortBy, FALSE, m_bSortAscending);
				break;

			case KBCF_SHOWEMPTYCOLUMNS:
			case KBCF_ALWAYSSHOWBACKLOG:
				if (m_aListCtrls.GetSize())
					RebuildListCtrls(FALSE);
				break;

			case KBCF_TASKTEXTCOLORISBKGND:
				m_aListCtrls.SetTextColorIsBackground(bSet);
				break;

			case KBCF_COLORBARBYPRIORITY:
				m_aListCtrls.SetColorTaskBarByPriority(bSet);
				break;

			case KBCF_STRIKETHRUDONETASKS:
				m_aListCtrls.SetStrikeThruDoneTasks(bSet);
				break;

			case KBCF_SHOWTASKCOLORASBAR:
				m_aListCtrls.SetShowTaskColorAsBar(bSet);
				break;

			case KBCF_SHOWCOMPLETIONCHECKBOXES:
				m_aListCtrls.SetShowCompletionCheckboxes(bSet);
				break;
			}
		}
	}
}

void CKanbanCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	Resize(CRect(0, 0, cx, cy));
}

BOOL CKanbanCtrl::OnEraseBkgnd(CDC* pDC)
{
	if (m_aListCtrls.GetSize())
	{
		// Clip out the list controls
		m_aListCtrls.Exclude(pDC);
		
		// fill the client with gray to create borders
		CRect rClient;
		GetClientRect(rClient);
		
		pDC->FillSolidRect(rClient, GetSysColor(COLOR_3DSHADOW));
	}
	
	return TRUE;
}

void CKanbanCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	CKanbanListCtrl* pList = GetSelListCtrl();

	if (pList)
	{
		pList->SetFocus();
		pList->Invalidate(FALSE);

		ScrollToSelectedTask();
	}
}

int CKanbanCtrl::GetVisibleListCtrlCount() const
{
	if (UsingDynamicColumns() || HasOption(KBCF_SHOWEMPTYCOLUMNS))
		return m_aListCtrls.GetSize();

	// Fixed columns
	BOOL bShowBacklog = HasOption(KBCF_ALWAYSSHOWBACKLOG);

	return m_aListCtrls.GetVisibleCount(bShowBacklog);
}

void CKanbanCtrl::Resize(const CRect& rect)
{
	int nNumVisibleLists = GetVisibleListCtrlCount();

	if (nNumVisibleLists)
	{
		BOOL bHideEmpty = !HasOption(KBCF_SHOWEMPTYCOLUMNS);
		BOOL bShowBacklog = HasOption(KBCF_ALWAYSSHOWBACKLOG);

		// Reduce client by a pixel to create a border
		CRect rAvail(rect);
		rAvail.DeflateRect(1, 1);

		CString sStatus;
		int nListWidth = (rAvail.Width() / nNumVisibleLists);

		// Check whether the lists are wide enough to show attribute labels
		KBC_ATTRIBLABELS nLabelVis = GetListAttributeLabelVisibility(nListWidth);

		// Also update tab order as we go
		int nNumLists = m_aListCtrls.GetSize();
		CWnd* pPrev = NULL;

		for (int nList = 0, nVis = 0; nList < nNumLists; nList++)
		{
			CKanbanListCtrl* pList = m_aListCtrls[nList];
			ASSERT(pList && pList->GetSafeHwnd());
			
			// If we find an empty column, it can only be with 
			// Fixed columns because we only hide columns rather
			// than delete them
			if (UsingFixedColumns())
			{
				if (!WantShowColumn(pList))
				{
					pList->ShowWindow(SW_HIDE);
					pList->EnableWindow(FALSE);
					continue;
				}

				// else
				pList->ShowWindow(SW_SHOW);
				pList->EnableWindow(TRUE);
			}

			CRect rList(rAvail);
			rList.left = (rAvail.left + (nVis * (nListWidth + 1))); // +1 to create a gap

			// Last column takes up rest of width
			if (nVis == (nNumVisibleLists - 1))
				rList.right = rAvail.right;
			else
				rList.right = (rList.left + nListWidth);

			pList->SetWindowPos(pPrev, rList.left, rList.top, rList.Width(), rList.Height(), 0);
			pList->SetAttributeLabelVisibility(nLabelVis);

			pPrev = pList;
			nVis++;
		}
	}
}

float CKanbanCtrl::GetAverageListCharWidth()
{
	return m_aListCtrls.GetAverageCharWidth();
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
				IUI_ATTRIBUTE nAttribID = m_aDisplayAttrib[nAtt];
				CString sLabel = CKanbanListCtrl::FormatAttribute(nAttribID, _T(""), nLabelVis);

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
					IUI_ATTRIBUTE nAttribID = m_aDisplayAttrib[nAtt];

					// Exclude 'File Link' and 'Parent' because these will 
					// almost always push things over the limit
					if ((nAttribID != IUI_FILEREF) && (nAttribID != IUI_PARENT))
					{
						int nValueLen = pKI->GetAttributeDisplayValue(nAttribID).GetLength();

						if ((int)((aLabelLen[nAtt] + nValueLen) * fAveCharWidth) > nAvailWidth)
							return FALSE;
					}
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

KBC_ATTRIBLABELS CKanbanCtrl::GetListAttributeLabelVisibility(int nListWidth)
{
	if (!m_aDisplayAttrib.GetSize() || !m_aListCtrls.GetSize())
		return KBCAL_NONE;

	// Calculate the available width for attributes
	int nAvailWidth = m_aListCtrls[0]->CalcAvailableAttributeWidth(nListWidth);

	// Calculate the fixed attribute label lengths and check if any
	// of them exceed the list width
	float fAveCharWidth = GetAverageListCharWidth();
	KBC_ATTRIBLABELS nLabelVis[2] = { KBCAL_LONG, KBCAL_SHORT };

	for (int nPass = 0; nPass < 2; nPass++)
	{
		if (CanFitAttributeLabels(nAvailWidth, fAveCharWidth, nLabelVis[nPass]))
			return nLabelVis[nPass];
	}

	return KBCAL_NONE;
}

void CKanbanCtrl::Resize()
{
	CRect rClient;
	GetClientRect(rClient);

	Resize(rClient);
}

void CKanbanCtrl::Sort(IUI_ATTRIBUTE nBy, BOOL bAllowToggle, BOOL bAscending)
{
	// if the sort attribute equals the track attribute then
	// tasks are already sorted into separate columns so we  
	// sort by title instead
	if ((nBy != IUI_NONE) && (nBy == m_nTrackAttribute))
		nBy = IUI_TASKNAME;

	IUI_ATTRIBUTE nOldSort = m_nSortBy;
	m_nSortBy = nBy;

	if (nBy != IUI_NONE)
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

		// do the sort
 		CHoldRedraw hr(*this);
		BOOL bSubtasksBelowParent = HasOption(KBCF_SORTSUBTASTASKSBELOWPARENTS);

		m_aListCtrls.SortItems(nBy, m_bSortAscending, bSubtasksBelowParent);
	}
}

BOOL CKanbanCtrl::IsSorted() const
{
	return (m_nSortBy != IUI_NONE);
}

void CKanbanCtrl::SetReadOnly(bool bReadOnly) 
{ 
	m_bReadOnly = bReadOnly; 
}

BOOL CKanbanCtrl::GetLabelEditRect(LPRECT pEdit)
{
	if (!m_pSelectedList || !m_pSelectedList->GetLabelEditRect(pEdit))
	{
		ASSERT(0);
		return FALSE;
	}

	// else convert from list to 'our' coords
	m_pSelectedList->ClientToScreen(pEdit);
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
			RedrawListCtrls(TRUE);
	}
}

void CKanbanCtrl::ScrollToSelectedTask()
{
	CKanbanListCtrl* pList = GetSelListCtrl();

	if (pList)
		pList->ScrollToSelection();
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

	const CKanbanListCtrl* pList = GetSelListCtrl();
	CString sValue;

	CRect rListCtrl;
	pList->GetWindowRect(rListCtrl);

	if (!GetListCtrlAttributeValue(pList, rListCtrl.CenterPoint(), sValue))
		return false;

	switch (m_nTrackAttribute)
	{
	case IUI_STATUS:
		pTasks->SetTaskStatus(hNewTask, sValue);
		break;

	case IUI_ALLOCTO:
		pTasks->AddTaskAllocatedTo(hNewTask, sValue);
		break;

	case IUI_ALLOCBY:
		pTasks->SetTaskAllocatedBy(hNewTask, sValue);
		break;

	case IUI_CATEGORY:
		pTasks->AddTaskCategory(hNewTask, sValue);
		break;

	case IUI_PRIORITY:
		pTasks->SetTaskPriority(hNewTask, _ttoi(sValue));
		break;

	case IUI_RISK:
		pTasks->SetTaskRisk(hNewTask, _ttoi(sValue));
		break;

	case IUI_VERSION:
		pTasks->SetTaskVersion(hNewTask, sValue);
		break;

	case IUI_TAGS:
		pTasks->AddTaskTag(hNewTask, sValue);
		break;

	case IUI_CUSTOMATTRIB:
		ASSERT(!m_sTrackAttribID.IsEmpty());
		pTasks->SetTaskCustomAttributeData(hNewTask, m_sTrackAttribID, sValue);
		break;
	}

	return true;
}

DWORD CKanbanCtrl::HitTestTask(const CPoint& ptScreen) const
{
	return m_aListCtrls.HitTestTask(ptScreen);
}

DWORD CKanbanCtrl::GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const
{
	BOOL bForward = ((nCmd == IUI_GETPREVTASK) || (nCmd == IUI_GETPREVTOPLEVELTASK));

	int nItem = -1;
	const CKanbanListCtrl* pList = LocateTask(dwTaskID, nItem, bForward);
	
	if (!pList || (UsingFixedColumns() && !pList->IsWindowVisible()))
	{
		return 0L;
	}
	else if (nItem == -1)
	{
		ASSERT(0);
		return 0L;
	}

	DWORD dwNextID(dwTaskID);

	switch (nCmd)
	{
	case IUI_GETNEXTTASK:
		if (nItem < (pList->GetItemCount() - 1))
			dwNextID = pList->GetItemData(nItem + 1);
		break;

	case IUI_GETNEXTTOPLEVELTASK:
		{
			const CKanbanListCtrl* pNext = GetNextListCtrl(pList, TRUE, TRUE);
			
			if (pNext)
				dwNextID = pNext->GetItemData(0);
		}
		break;

	case IUI_GETPREVTASK:
		if (nItem > 0)
			dwNextID = pList->GetItemData(nItem - 1);
		break;

	case IUI_GETPREVTOPLEVELTASK:
		{
			const CKanbanListCtrl* pPrev = GetNextListCtrl(pList, FALSE, TRUE);
			
			if (pPrev)
				dwNextID = pPrev->GetItemData(0);
		}
		break;

	default:
		ASSERT(0);
	}

	return dwNextID;
}

const CKanbanListCtrl* CKanbanCtrl::GetNextListCtrl(const CKanbanListCtrl* pList, BOOL bNext, BOOL bExcludeEmpty) const
{
	return m_aListCtrls.GetNext(pList, bNext, bExcludeEmpty, UsingFixedColumns());
}

CKanbanListCtrl* CKanbanCtrl::GetNextListCtrl(const CKanbanListCtrl* pList, BOOL bNext, BOOL bExcludeEmpty)
{
	return m_aListCtrls.GetNext(pList, bNext, bExcludeEmpty, UsingFixedColumns());
}

CKanbanListCtrl* CKanbanCtrl::HitTestListCtrl(const CPoint& ptScreen, BOOL* pbHeader) const
{
	return m_aListCtrls.HitTest(ptScreen, pbHeader);
}

BOOL CKanbanCtrl::IsDragging() const
{
	ASSERT(!m_bReadOnly);

	return (!m_bReadOnly && (::GetCapture() == GetSafeHwnd()));
}

BOOL CKanbanCtrl::NotifyParentAttibuteChange(const CDWordArray& aTaskIDs)
{
	ASSERT(!m_bReadOnly);
	ASSERT(aTaskIDs.GetSize());

	return GetParent()->SendMessage(WM_KBC_VALUECHANGE, (WPARAM)GetSafeHwnd(), (LPARAM)&aTaskIDs);
}

void CKanbanCtrl::NotifyParentSelectionChange()
{
	GetParent()->SendMessage(WM_KBC_SELECTIONCHANGE, 0, 0);
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

void CKanbanCtrl::OnListSetFocus(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	// Ignore focus changes during drag and drop
	if (!IsDragging() && !IsSelectedListCtrl(pNMHDR->hwndFrom))
	{
		CKanbanListCtrl* pList = GetListCtrl(pNMHDR->hwndFrom);
		ASSERT(pList);
		
		SelectListCtrl(pList);
	}
}

BOOL CKanbanCtrl::SelectListCtrl(CKanbanListCtrl* pList, BOOL bNotifyParent)
{
	if (pList)
	{
		if (pList == m_pSelectedList)
			return TRUE;

		CKanbanListCtrl* pPrevSelList = m_pSelectedList;
		m_pSelectedList = pList;

		FixupFocus();

		if (pList->GetItemCount() > 0)
		{
			ClearOtherListSelections(m_pSelectedList);

			if (m_pSelectedList->GetSelectedCount() == 0)
			{
				// Select the first visible item
				int nFirstVis = m_pSelectedList->GetTopIndex();
				ASSERT(nFirstVis != -1);
				
				m_pSelectedList->SelectItem(nFirstVis, FALSE);
				m_pSelectedList->Invalidate(TRUE);
			}
			else
			{
				// Scroll to the current selection
				POSITION pos = m_pSelectedList->GetFirstSelectedItemPosition();
				int nFirstSel = m_pSelectedList->GetNextSelectedItem(pos);
			
				m_pSelectedList->EnsureVisible(nFirstSel, FALSE);
			}

			if (bNotifyParent)
				NotifyParentSelectionChange();
		}
		else
		{
			pPrevSelList->SetSelected(FALSE);
			m_pSelectedList->SetSelected(TRUE);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CKanbanCtrl::IsSelectedListCtrl(HWND hWnd) const
{
	return (m_pSelectedList && (m_pSelectedList->GetSafeHwnd() == hWnd));
}

void CKanbanCtrl::OnListItemChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	// only interested in selection changes from the selected list 
	// and occurring outside of a drag'n'drop, because the 'actual' 
	// selected task IDs will not change during a drag'n'drop
	if (!IsDragging() && IsSelectedListCtrl(pNMHDR->hwndFrom))
	{
		NMLISTVIEW* pNMLV = (NMLISTVIEW*)pNMHDR;
		
		if (CKanbanListCtrl::IsSelectionChange(pNMLV))
		{
			NotifyParentSelectionChange();
		}
	}
#ifdef _DEBUG
	else
	{
		int breakpoint = 0;
	}
#endif
}

void CKanbanCtrl::OnListEditLabel(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = TRUE; // cancel our edit

	NMLVDISPINFO* pNMLV = (NMLVDISPINFO*)pNMHDR;
	ASSERT(pNMLV->item.lParam);

	GetParent()->SendMessage(WM_KBC_EDITTASKTITLE, pNMLV->item.lParam);
}

void CKanbanCtrl::ClearOtherListSelections(const CKanbanListCtrl* pIgnore)
{
	m_aListCtrls.ClearOtherSelections(pIgnore);
}

void CKanbanCtrl::OnBeginDragListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!m_bReadOnly)
	{
		ASSERT(!IsDragging());
		ASSERT(pNMHDR->idFrom == IDC_LISTCTRL);
		
		NMLISTVIEW* pNMLV = (NMLISTVIEW*)pNMHDR;
		ASSERT(pNMLV->iItem != -1);
		
		CKanbanListCtrl* pList = (CKanbanListCtrl*)CWnd::FromHandle(pNMHDR->hwndFrom);

		if (!pList->SelectionHasLockedTasks())
		{
			CDWordArray aDragTaskIDs;

			if (pList->GetSelectedTasks(aDragTaskIDs))
			{
				// If the 'drag-from' list is not currently selected
				// we select it and then reset the selection to the
				// items we have just copied
				if (pList != m_pSelectedList)
				{
					VERIFY(pList->SelectTasks(aDragTaskIDs));
					SelectListCtrl(pList);
				}

				SetCapture();
			}
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
	if (IsDragging())
	{
		// get the list under the mouse
		ClientToScreen(&point);

		CKanbanListCtrl* pDestList = HitTestListCtrl(point);
		CKanbanListCtrl* pSrcList = m_pSelectedList;

		if (CanDrag(pSrcList, pDestList))
		{
			CString sDestAttribValue;
			
			if (GetListCtrlAttributeValue(pDestList, point, sDestAttribValue))
			{
				CDWordArray aDragTaskIDs, aChangedIDs;

				int nDrag = pSrcList->GetSelectedTasks(aDragTaskIDs);
				ASSERT(nDrag > 0);

				while (nDrag--)
				{
					DWORD dwTaskID = aDragTaskIDs[nDrag];

					if (EndDragItem(pSrcList, dwTaskID, pDestList, sDestAttribValue))
					{
						aChangedIDs.Add(dwTaskID);
					}
				}

				if (!WantShowColumn(pSrcList) && UsingDynamicColumns())
				{
					int nList = Misc::FindT(m_aListCtrls, pSrcList);
					ASSERT(nList != -1);

					m_aListCtrls.RemoveAt(nList);
				}

				Resize();

				if (aChangedIDs.GetSize())
				{
					SelectListCtrl(pDestList, FALSE);
					SelectTasks(aDragTaskIDs); 

					NotifyParentSelectionChange();
					NotifyParentAttibuteChange(aChangedIDs); // only the changed tasks
				}
			}
		}
	}

	// always
	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CKanbanCtrl::CanDrag(const CKanbanListCtrl* pSrcList, const CKanbanListCtrl* pDestList) const
{
	// Can only copy MULTI-VALUE attributes
	if (Misc::ModKeysArePressed(MKS_CTRL) && !IsTrackedAttributeMultiValue())
		return FALSE;

	return CKanbanListCtrl::CanDrag(pSrcList, pDestList);
}

BOOL CKanbanCtrl::EndDragItem(CKanbanListCtrl* pSrcList, DWORD dwTaskID, 
								CKanbanListCtrl* pDestList, const CString& sDestAttribValue)
{
	ASSERT(CanDrag(pSrcList, pDestList));
	ASSERT(pSrcList->FindTask(dwTaskID) != -1);

	KANBANITEM* pKI = GetKanbanItem(dwTaskID);

	if (!pKI)
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bSrcIsBacklog = pSrcList->IsBacklog();
	BOOL bDestIsBacklog = pDestList->IsBacklog();
	BOOL bCopy = (!bSrcIsBacklog && 
					Misc::ModKeysArePressed(MKS_CTRL) &&
					IsTrackedAttributeMultiValue());

	// Remove from the source list(s) if moving
	if (bSrcIsBacklog)
	{
		VERIFY(pSrcList->DeleteTask(dwTaskID));
	}
	else if (!bCopy) // move
	{
		// Remove all values
		pKI->RemoveAllTrackedAttributeValues(m_sTrackAttribID);

		// Remove from all src lists
		m_aListCtrls.DeleteTaskFromOthers(dwTaskID, pDestList);
	}
	else if (bDestIsBacklog) // and 'copy'
	{
		// Just remove the source list's value(s)
		CStringArray aSrcValues;
		int nVal = pSrcList->GetAttributeValues(aSrcValues);

		while (nVal--)
			pKI->RemoveTrackedAttributeValue(m_sTrackAttribID, aSrcValues[nVal]);

		VERIFY(pSrcList->DeleteTask(dwTaskID));
	}

	// Append to the destination list
	if (bDestIsBacklog)
	{
		if (!pKI->HasTrackedAttributeValues(m_sTrackAttribID))
			pDestList->AddTask(*pKI, TRUE);
	}
	else
	{
		pKI->AddTrackedAttributeValue(m_sTrackAttribID, sDestAttribValue);

		if (pDestList->FindTask(dwTaskID) == -1)
			pDestList->AddTask(*pKI, TRUE);
	}

	return TRUE;
}

BOOL CKanbanCtrl::GetListCtrlAttributeValue(const CKanbanListCtrl* pDestList, const CPoint& ptScreen, CString& sValue) const
{
	CStringArray aListValues;
	int nNumValues = pDestList->GetAttributeValues(aListValues);

	switch (nNumValues)
	{
	case 0: // Backlog
		sValue.Empty();
		return TRUE;
		
	case 1:
		sValue = aListValues[0];
		return TRUE;
	}

	// List has multiple values -> show popup menu
	CMenu menu;
	VERIFY (menu.CreatePopupMenu());

	for (int nVal = 0; nVal < nNumValues; nVal++)
	{
		menu.AppendMenu(MF_STRING, (nVal + 1), aListValues[nVal]);
	}

	UINT nValID = menu.TrackPopupMenu((TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD), 
										ptScreen.x, ptScreen.y, CWnd::FromHandle(*pDestList));

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
		ASSERT(!m_bReadOnly);
		
		// get the list under the mouse
		ClientToScreen(&point);
		const CKanbanListCtrl* pDestList = HitTestListCtrl(point);

		BOOL bValidDest = CanDrag(m_pSelectedList, pDestList);
		BOOL bCopy = Misc::ModKeysArePressed(MKS_CTRL);

		GraphicsMisc::SetDragDropCursor(bValidDest ? (bCopy ? GMOC_COPY : GMOC_MOVE) : GMOC_NO);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CKanbanCtrl::SaveToImage(CBitmap& bmImage)
{
	return m_aListCtrls.SaveToImage(bmImage);
}

int CKanbanCtrl::CalcRequiredColumnWidthForImage() const
{
	return m_aListCtrls.CalcRequiredColumnWidthForImage();
}

BOOL CKanbanCtrl::CanSaveToImage() const
{
	return m_aListCtrls.CanSaveToImage();
}

LRESULT CKanbanCtrl::OnSetFont(WPARAM wp, LPARAM lp)
{
	m_fonts.Initialise((HFONT)wp, FALSE);
	m_aListCtrls.OnSetFont((HFONT)wp);

	return 0L;
}

LRESULT CKanbanCtrl::OnListCheckChange(WPARAM /*wp*/, LPARAM lp)
{
	ASSERT(!m_bReadOnly);

	const KANBANITEM* pKI = m_data.GetItem(lp);
	ASSERT(pKI);

	if (pKI)
	{
		LRESULT lr = GetParent()->SendMessage(WM_KBC_COMPLETIONCHANGE, (WPARAM)GetSafeHwnd(), !pKI->IsDone(FALSE));

		if (lr)
			PostMessage(WM_KCM_SELECTTASK, 0, lp);

		return lr;
	}

	// else
	return 0L;
}

LRESULT CKanbanCtrl::OnSelectTask(WPARAM /*wp*/, LPARAM lp)
{
	return SelectTask(lp);
}

LRESULT CKanbanCtrl::OnListGetTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_KBC_GETTASKICON, wp, lp);
}
