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

CKanbanCtrl::CKanbanCtrl(CFontCache& fonts) 
	:
	m_fonts(fonts),
	m_bSortAscending(-1), 
	m_dwOptions(0),
	m_bDragging(FALSE),
	m_bReadOnly(FALSE),
	m_nNextColor(0),
	m_pDragFromList(NULL),
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
ON_NOTIFY(NM_SETFOCUS, IDC_LISTCTRL, OnListSetFocus)
ON_WM_SETFOCUS()
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

	ModifyStyleEx(0, WS_EX_CONTROLPARENT, 0);
	m_ilHeight.Create(1, 32, ILC_COLOR32, 1, 0);

	return 0;
}

bool CKanbanCtrl::ProcessMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
		// handle 'escape' during dragging
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_ESCAPE)
			return (CancelOperation() == TRUE);
		break;

	case WM_LBUTTONDOWN:
		{
			CPoint ptScreen(pMsg->lParam);
			::ClientToScreen(pMsg->hwnd, &ptScreen);

			BOOL bHeader = FALSE;
			CKanbanListCtrl* pList = HitTestListCtrl(ptScreen, &bHeader);

			if (bHeader && pList && pList->GetItemCount())
				pList->SetFocus();
		}
		break;
	}
	
	// all else
	return false;
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
	ASSERT((m_pSelectedList == NULL) || (Misc::FindT(m_aListCtrls, m_pSelectedList) != -1));

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
	ClearOtherListSelections(NULL);
	if (aTaskIDs.GetSize() == 0)
	{
		return FALSE;
	}

	// find the list containing the first item and then
	// check that all the other items belong to the same list
	int nItem = -1;
	CKanbanListCtrl* pList = LocateTask(aTaskIDs[0], nItem);

	if (pList && (nItem != -1))
	{
		if (!pList->SelectTasks(aTaskIDs))
			pList = NULL;
	}
	
	ClearOtherListSelections(pList);

	if (pList)
	{
		m_pSelectedList = pList;

		if (IsWindowVisible())
			pList->SetFocus();

		ScrollToSelectedTask();
	}
	
	return (pList != NULL);
}

BOOL CKanbanCtrl::SelectTask(DWORD dwTaskID)
{
	int nItem = -1;
	CKanbanListCtrl* pList = LocateTask(dwTaskID, nItem);

	if (pList && (nItem != -1))
	{
		CDWordArray aItemIDs;
		aItemIDs.Add(dwTaskID);

		m_pSelectedList = pList;
		pList->SelectTasks(aItemIDs);

		if (IsWindowVisible())
			pList->SetFocus();

		ScrollToSelectedTask();
		ClearOtherListSelections(pList);
	}
	
	return (pList != NULL);
}

void CKanbanCtrl::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib)
{
	ASSERT(GetSafeHwnd());

	// always cancel any ongoing operation
	CancelOperation();

	const ITaskList15* pTasks14 = GetITLInterface<ITaskList15>(pTasks, IID_TASKLIST15);
	BOOL bResort = FALSE;
	
	switch (nUpdate)
	{
	case IUI_ALL:
		{
 			CDWordArray aSelIDs;
			GetSelectedTaskIDs(aSelIDs);

 			RebuildData(pTasks14, attrib);
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
			BOOL bChange = UpdateGlobalAttributeValues(pTasks14, attrib);
			bChange |= UpdateData(pTasks14, pTasks14->GetFirstTask(), attrib, TRUE);

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
 			RemoveDeletedTasks(pTasks14);
		}
		break;
		
	case IUI_MOVE:
		ASSERT(0);
		break;
		
	default:
		ASSERT(0);
	}
}

CString CKanbanCtrl::GetTaskAllocTo(const ITaskList15* pTasks, HTASKITEM hTask)
{
	int nAllocTo = pTasks->GetTaskAllocatedToCount(hTask);
	
	if (nAllocTo == 0)
	{
		return EMPTY_STR;
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
	case IUI_LASTMOD:
	case IUI_PERCENT:
	case IUI_PRIORITY:
	case IUI_RECURRENCE:
	case IUI_RISK:
	case IUI_STARTDATE:
	case IUI_STATUS:
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

BOOL CKanbanCtrl::RebuildData(const ITaskList15* pTasks, const CSet<IUI_ATTRIBUTE>& attrib)
{
	// Rebuild global attribute value lists
	m_mapAttributeValues.RemoveAll();
	m_aCustomAttribIDs.RemoveAll();

	UpdateGlobalAttributeValues(pTasks, attrib);

	// Rebuild data
	m_data.RemoveAll();

	return AddTaskToData(pTasks, pTasks->GetFirstTask(), 0, attrib, TRUE);
}

BOOL CKanbanCtrl::AddTaskToData(const ITaskList15* pTasks, HTASKITEM hTask, DWORD dwParentID, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings)
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

	pKI->SetColor(pTasks->GetTaskTextColor(hTask));

	// Path is parent's path + parent's name
	if (dwParentID)
	{
		const KANBANITEM* pKIParent = m_data.GetItem(dwParentID);
		ASSERT(pKIParent);

		if (pKIParent->sPath.IsEmpty())
			pKI->sPath = pKIParent->sTitle;
		else
			pKI->sPath = pKIParent->sPath + '\\' + pKIParent->sTitle;
	}

	// trackable attributes
	pKI->SetAttributeValue(_T("STATUS"), pTasks->GetTaskStatus(hTask));
	pKI->SetAttributeValue(_T("ALLOCBY"), pTasks->GetTaskAllocatedBy(hTask));
	pKI->SetAttributeValue(_T("ALLOCTO"), pTasks->GetTaskAllocatedTo(hTask, 0));
	pKI->SetAttributeValue(_T("CATEGORY"), pTasks->GetTaskCategory(hTask, 0));
	pKI->SetAttributeValue(_T("VERSION"), pTasks->GetTaskVersion(hTask));
	pKI->SetAttributeValue(_T("TAGS"), pTasks->GetTaskTag(hTask, 0));
	pKI->SetAttributeValue(_T("PRIORITY"), pTasks->GetTaskPriority(hTask, FALSE));
	pKI->SetAttributeValue(_T("RISK"), pTasks->GetTaskRisk(hTask, FALSE));
	// TODO

	// custom attributes
	int nCust = pTasks->GetCustomAttributeCount();

	while (nCust--)
	{
		CString sCustID(pTasks->GetCustomAttributeID(nCust));
		CString sCustValue(pTasks->GetTaskCustomAttributeData(hTask, sCustID));

		pKI->SetAttributeValue(sCustID, sCustValue);

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

BOOL CKanbanCtrl::UpdateData(const ITaskList15* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings)
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

			if (attrib.HasKey(IUI_TASKNAME))
			{
				pKI->sTitle = pTasks->GetTaskTitle(hTask);
			}
			
			if (attrib.HasKey(IUI_DONEDATE))
			{
				BOOL bDone = pTasks->IsTaskDone(hTask);
				BOOL bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);

				if ((pKI->bDone != bDone) || (pKI->bGoodAsDone != bGoodAsDone))
				{
					pKI->bDone = bDone;
					pKI->bGoodAsDone = bGoodAsDone;
				}
			}
			
			// Trackable attributes
			if (attrib.HasKey(IUI_ALLOCBY))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_ALLOCBY, pTasks->GetTaskAllocatedBy(hTask));

			if (attrib.HasKey(IUI_ALLOCTO))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_ALLOCTO, pTasks->GetTaskAllocatedTo(hTask, 0));

			if (attrib.HasKey(IUI_STATUS))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_STATUS, pTasks->GetTaskStatus(hTask));

			if (attrib.HasKey(IUI_CATEGORY))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_CATEGORY, pTasks->GetTaskCategory(hTask, 0));

			if (attrib.HasKey(IUI_VERSION))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_VERSION, pTasks->GetTaskVersion(hTask));

			if (attrib.HasKey(IUI_TAGS))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_TAGS, pTasks->GetTaskTag(hTask, 0));

			if (attrib.HasKey(IUI_PRIORITY))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_PRIORITY, pTasks->GetTaskPriority(hTask, FALSE));

			if (attrib.HasKey(IUI_RISK))
				bChange |= UpdateTrackableTaskAttribute(pKI, IUI_RISK, pTasks->GetTaskRisk(hTask, FALSE));

			if (attrib.HasKey(IUI_CUSTOMATTRIB))
			{
				int nID = m_aCustomAttribIDs.GetSize();

				while (nID--)
				{
					CString sAttribID(m_aCustomAttribIDs[nID]);
					CString sValue(pTasks->GetTaskCustomAttributeData(hTask, sAttribID));

					if (UpdateTrackableTaskAttribute(pKI, sAttribID, sValue))
					{
						// Add to global values
						CKanbanValueMap* pValues = m_mapAttributeValues.GetAddMapping(sAttribID);
						ASSERT(pValues);
						
						pValues->AddValue(sValue);
						bChange = TRUE;
					}
				}
			}

			// other display-only attributes
			UpdateItemDisplayAttributes(pKI, pTasks, hTask, attrib);
			
			// always update colour because it can change for so many reasons
			pKI->SetColor(pTasks->GetTaskTextColor(hTask));
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

void CKanbanCtrl::UpdateItemDisplayAttributes(KANBANITEM* pKI, const ITaskList15* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib)
{
	time64_t tDate = 0;
	
	if (attrib.HasKey(IUI_TIMEEST))
		pKI->dTimeEst = pTasks->GetTaskTimeEstimate(hTask, pKI->nTimeEstUnits, FALSE);
	
	if (attrib.HasKey(IUI_TIMESPENT))
		pKI->dTimeSpent = pTasks->GetTaskTimeSpent(hTask, pKI->nTimeSpentUnits, FALSE);
	
	if (attrib.HasKey(IUI_COST))
		pKI->dCost = pTasks->GetTaskCost(hTask, FALSE);
	
	if (attrib.HasKey(IUI_CREATEDBY))
		pKI->sCreatedBy = pTasks->GetTaskCreatedBy(hTask);
	
	if (attrib.HasKey(IUI_CREATIONDATE))
		pKI->dtCreate = pTasks->GetTaskCreationDate(hTask);
	
	if (attrib.HasKey(IUI_DONEDATE) && pTasks->GetTaskDoneDate64(hTask, tDate))
		pKI->dtDone = CDateHelper::GetDate(tDate);
	
	if (attrib.HasKey(IUI_DUEDATE) && pTasks->GetTaskDueDate64(hTask, FALSE, tDate))
		pKI->dtDue = CDateHelper::GetDate(tDate);
	
	if (attrib.HasKey(IUI_STARTDATE) && pTasks->GetTaskStartDate64(hTask, FALSE, tDate))
		pKI->dtStart = CDateHelper::GetDate(tDate);
	
	if (attrib.HasKey(IUI_LASTMOD) && pTasks->GetTaskLastModified64(hTask, tDate))
		pKI->dtLastMod = CDateHelper::GetDate(tDate);
	
	if (attrib.HasKey(IUI_PERCENT))
		pKI->nPercent = pTasks->GetTaskPercentDone(hTask, FALSE);
	
	if (attrib.HasKey(IUI_EXTERNALID))
		pKI->sExternalID = pTasks->GetTaskExternalID(hTask);
	
	if (attrib.HasKey(IUI_RECURRENCE))
		pKI->sRecurrence = pTasks->GetTaskAttribute(hTask, TDL_TASKRECURRENCE);

	if (attrib.HasKey(IUI_FILEREF) && pTasks->GetTaskFileLinkCount(hTask))
	{
		pKI->sFileRef = pTasks->GetTaskFileLink(hTask, 0);

		// Get the shortest meaningful bit because of space constraints
		if (FileMisc::IsPath(pKI->sFileRef))
			pKI->sFileRef = FileMisc::GetFileNameFromPath(pKI->sFileRef);
	}
}

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(const ITaskList15* pTasks, const CSet<IUI_ATTRIBUTE>& attrib)
{
	BOOL bChange = FALSE;

	if (attrib.HasKey(IUI_STATUS))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_STATUS);
	
	if (attrib.HasKey(IUI_ALLOCTO))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_ALLOCTO);
	
	if (attrib.HasKey(IUI_CATEGORY))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_CATEGORY);
	
	if (attrib.HasKey(IUI_ALLOCBY))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_ALLOCBY);
	
	if (attrib.HasKey(IUI_TAGS))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_TAGS);
	
	if (attrib.HasKey(IUI_VERSION))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_VERSION);
	
	if (attrib.HasKey(IUI_PRIORITY))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_PRIORITY);
	
	if (attrib.HasKey(IUI_RISK))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_RISK);
	
	if (attrib.HasKey(IUI_CUSTOMATTRIB))
		bChange |= UpdateGlobalAttributeValues(pTasks, IUI_CUSTOMATTRIB);
	
	return bChange;
}

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(const ITaskList15* pTasks, IUI_ATTRIBUTE nAttribute)
{
	switch (nAttribute)
	{
	case IUI_PRIORITY:
	case IUI_RISK:
		{
			// create once only
			CString sAttribID(KANBANITEM::GetAttribID(nAttribute));

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
			CString sAttribID(KANBANITEM::GetAttribID(nAttribute));

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
					Misc::AddUniqueItem(sAttribID, m_aCustomAttribIDs);

					// Add non-empty values to the map
					CKanbanValueMap* pDefValues = m_mapDefAttributeValues.GetAddMapping(sAttribID);
					ASSERT(pDefValues);

					pDefValues->RemoveAll();

					CString sDefValues = pTasks->GetCustomAttributeListData(nCustom);
					CStringArray aDefValues;

					if (Misc::Split(sDefValues, aDefValues, '\n'))
						pDefValues->SetValues(aDefValues);
								
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

BOOL CKanbanCtrl::UpdateGlobalAttributeValues(const ITaskList15* pTasks, LPCTSTR szXMLTag, LPCTSTR szAttribID)
{
	CKanbanValueMap mapNewValues;
	
	int nValue = pTasks->GetAttributeCount(szXMLTag);

	while (nValue--)
	{
		CString sValue(pTasks->GetAttribute(szXMLTag, nValue));

		if (!sValue.IsEmpty())
			mapNewValues.AddValue(sValue);
	}

	// Add in Backlog value
	mapNewValues.AddValue(EMPTY_STR);

	// Merge in default values
	const CKanbanValueMap* pDefValues = m_mapDefAttributeValues.GetMapping(szAttribID);

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

int CKanbanCtrl::GetAttributeValues(IUI_ATTRIBUTE nAttrib, CStringArray& aValues) const
{
	CString sAttribID(KANBANITEM::GetAttribID(nAttrib));

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
	aCustAttribIDs.Copy(m_aCustomAttribIDs);
	return aCustAttribIDs.GetSize();
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
	pos = m_mapDefAttributeValues.GetStartPosition();

	while (pos)
	{
		m_mapDefAttributeValues.GetNextAssoc(pos, sAttribID, pValues);
		ASSERT(pValues);

		CKanbanValueMap* pCopyValues = mapValues.GetAddMapping(sAttribID);
		ASSERT(pCopyValues);

		Misc::Append(*pValues, *pCopyValues);
	}

	return mapValues.GetCount();
}

void CKanbanCtrl::LoadDefaultAttributeValues(const IPreferences* pPrefs)
{
	m_mapDefAttributeValues.RemoveAll();

	LoadDefaultAttributeValues(pPrefs, _T("ALLOCTO"),	_T("AllocToList"));
	LoadDefaultAttributeValues(pPrefs, _T("ALLOCBY"),	_T("AllocByList"));
	LoadDefaultAttributeValues(pPrefs, _T("STATUS"),	_T("StatusList"));
	LoadDefaultAttributeValues(pPrefs, _T("CATEGORY"),	_T("CategoryList"));
	LoadDefaultAttributeValues(pPrefs, _T("VERSION"),	_T("VersionList"));
	LoadDefaultAttributeValues(pPrefs, _T("TAGS"),		_T("TagList"));

	if (m_nTrackAttribute != IUI_NONE)
		RebuildListCtrls(FALSE);
}

void CKanbanCtrl::LoadDefaultAttributeValues(const IPreferences* pPrefs, LPCTSTR szAttribID, LPCTSTR szSubKey)
{
	CKanbanValueMap* pMap = m_mapDefAttributeValues.GetAddMapping(szAttribID);
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
	ASSERT(nAttrib == IUI_PRIORITY || nAttrib == IUI_RISK);

	CString sValue; // empty

	if (nNewValue >= 0)
		sValue = Misc::Format(nNewValue);
	
	// else empty
	return UpdateTrackableTaskAttribute(pKI, nAttrib, sValue);
}

BOOL CKanbanCtrl::UpdateTrackableTaskAttribute(KANBANITEM* pKI, IUI_ATTRIBUTE nAttrib, const CString& sNewValue)
{
	return UpdateTrackableTaskAttribute(pKI, KANBANITEM::GetAttribID(nAttrib), sNewValue);
}

BOOL CKanbanCtrl::UpdateTrackableTaskAttribute(KANBANITEM* pKI, const CString& sAttribID, const CString& sNewValue)
{
	BOOL bChange = FALSE;
	
	if (pKI->GetAttributeValue(sAttribID).CompareNoCase(sNewValue) != 0)
	{
		if (m_sTrackAttribID == sAttribID)
		{
			// remove from it's current list
			int nCurItem = -1;
			CKanbanListCtrl* pCurList = LocateTask(pKI->dwTaskID, nCurItem);
			
			if (pCurList && (nCurItem != -1))
			{
				pCurList->DeleteItem(nCurItem);
				bChange = (pCurList->GetItemCount() == 0);
			}
			
			// add at the head of its new list
			CKanbanListCtrl* pNewList = GetListCtrl(sNewValue);
			
			if (pNewList)
			{
				int nNewItem = pNewList->InsertItem(0, pKI->sTitle, -1);
				pNewList->SetItemData(nNewItem, pKI->dwTaskID);
			}
			else
			{
				bChange = TRUE; // needs new list ctrl
			}
		}
		
		// update status
		pKI->SetAttributeValue(sAttribID, sNewValue);
	}
	
	return bChange;
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
	int nList = m_aListCtrls.GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = m_aListCtrls[nList];
		ASSERT(pList);

		if (pList)
			pList->Invalidate(bErase);
	}
}

BOOL CKanbanCtrl::DeleteListCtrl(int nList)
{
	if (nList < 0 || nList >= m_aListCtrls.GetSize())
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

void CKanbanCtrl::RemoveOldDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingDynamicColumns())
	{
		ASSERT(0);
		return;
	}

	// remove any lists whose values are no longer used
	// or which Optionally have no items 
	const CKanbanValueMap* pGlobals = m_mapAttributeValues.GetMapping(m_sTrackAttribID);
	int nList = m_aListCtrls.GetSize();
	
	while (nList--)
	{
		CKanbanListCtrl* pList = m_aListCtrls[nList];
		ASSERT(pList && !pList->HasMultipleValues());
		
		if (pGlobals == NULL)
		{
			DeleteListCtrl(nList);
		}
		else
		{
			CString sAttribValueID(pList->GetAttributeValueID());
			
			if (!Misc::HasKey(*pGlobals, sAttribValueID) || 
				!WantShowColumn(sAttribValueID, mapKIArray))
			{
				DeleteListCtrl(nList);
			}
		}
	}
}

void CKanbanCtrl::AddMissingDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingDynamicColumns())
	{
		ASSERT(0);
		return;
	}
	
	// Add any new status lists not yet existing
	const CKanbanValueMap* pGlobals = m_mapAttributeValues.GetMapping(m_sTrackAttribID);

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
				
				VERIFY (NewListCtrl(colDef) != NULL);
			}
		}
		ASSERT(!HasOption(KBCF_SHOWEMPTYCOLUMNS) || 
				(m_nTrackAttribute == IUI_CUSTOMATTRIB) ||
				(m_aListCtrls.GetSize() == pGlobals->GetCount()));
	}
}

void CKanbanCtrl::RebuildDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray)
{
	if (!UsingDynamicColumns())
	{
		ASSERT(0);
		return;
	}
	
	RemoveOldDynamicListCtrls(mapKIArray);
	AddMissingDynamicListCtrls(mapKIArray);

	// If no columns created, create empty Backlog column
	CheckAddBacklogListCtrl();
	
	// (Re)sort
	if (m_aListCtrls.GetSize() > 1)
		qsort(m_aListCtrls.GetData(), m_aListCtrls.GetSize(), sizeof(CKanbanListCtrl**), ListSortProc);
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
			VERIFY(NewListCtrl(colDef) != NULL);
		}
	}
}

void CKanbanCtrl::CheckAddBacklogListCtrl()
{
	if (m_aListCtrls.GetSize() == 0) 
	{
		KANBANCOLUMN colDef;
		
		colDef.sAttribID = m_sTrackAttribID;
		colDef.aAttribValues.Add(_T(""));
		
		VERIFY (NewListCtrl(colDef) != NULL);
	}
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

	if (m_aColumnDefs.GetSize() == 0) // Dynamic columns
		RebuildDynamicListCtrls(mapKIArray);
	else
		RebuildFixedListCtrls(mapKIArray);

	// Rebuild the list data for each list (which can be empty)
	if (bRebuildData)
		RebuildListCtrlData(mapKIArray);

	Resize();
	FixupSelection();
}

void CKanbanCtrl::RebuildListCtrlData(const CKanbanItemArrayMap& mapKIArray)
{
	BOOL bShowParents = HasOption(KBCF_SHOWPARENTTASKS);
	BOOL bHideEmpty = !HasOption(KBCF_SHOWEMPTYCOLUMNS);

	int nList = m_aListCtrls.GetSize();
	
	while (nList--)
	{
		CKanbanListCtrl* pList = m_aListCtrls[nList];
		ASSERT(pList);
		
		RebuildListContents(pList, mapKIArray, bShowParents);
		
		// The list can still end up empty if parent tasks are 
		// omitted in Dynamic columns so we recheck and delete if required
		if (!bShowParents && bHideEmpty && UsingDynamicColumns() && !pList->GetItemCount())
		{
			DeleteListCtrl(nList);
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
	if (!m_pSelectedList || (Misc::FindT(m_aListCtrls, m_pSelectedList) == -1))
	{
		m_pSelectedList = NULL;

		// Find the first list with some items
		for (int nList = 0; nList < m_aListCtrls.GetSize(); nList++)
		{
			if (m_aListCtrls[nList]->GetItemCount())
			{
				m_pSelectedList = m_aListCtrls[nList];
				break;
			}
		}

		// No list has items?
		if (!m_pSelectedList)
			m_pSelectedList = m_aListCtrls[0];
	}
	
	if (IsWindowVisible())
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
	if (!bChange && UsingFixedColumns())
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

	m_aColumnDefs.Copy(aColumnDefs);

	// update state
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
		m_sTrackAttribID = KANBANITEM::GetAttribID(nAttrib);
		break;
		
	case IUI_CUSTOMATTRIB:
		m_sTrackAttribID = sCustomAttribID;
		break;
	}
	m_nTrackAttribute = nAttrib;

	// delete all lists and start over
	CHoldRedraw gr(*this, NCR_PAINT | NCR_ERASEBKGND);

	m_pSelectedList = NULL;
	m_aListCtrls.RemoveAll();

	RebuildListCtrls(TRUE);
	Resize();

	return TRUE;
}

CKanbanListCtrl* CKanbanCtrl::NewListCtrl(const KANBANCOLUMN& colDef)
{
	CKanbanListCtrl* pList = new CKanbanListCtrl(m_data, colDef, m_fonts, m_aPriorityColors, m_aDisplayAttrib);
	ASSERT(pList);

	if (pList)
	{
		pList->SetTextColorIsBackground(HasOption(KBCF_TASKTEXTCOLORISBKGND));
		pList->SetStrikeThruDoneTasks(HasOption(KBCF_STRIKETHRUDONETASKS));
		pList->SetColorTasksByPriority(HasOption(KBCF_COLORBARBYPRIORITY));

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

int CKanbanCtrl::ListSortProc(const void* pV1, const void* pV2)
{
	typedef CKanbanListCtrl* LPCKanbanListCtrl;
	
	const CKanbanListCtrl* pKLC1 = *(static_cast<const LPCKanbanListCtrl*>(pV1));
	const CKanbanListCtrl* pKLC2 = *(static_cast<const LPCKanbanListCtrl*>(pV2));

	// backlog always comes first
	if (!pKLC1->HasAnyValues())
		return -1;

	if (!pKLC2->HasAnyValues())
		return 1;
	
	// Sort by ID
	return Misc::NaturalCompare(pKLC1->GetAttributeValue(), pKLC2->GetAttributeValue());
}

BOOL CKanbanCtrl::RebuildListContents(CKanbanListCtrl* pList, const CKanbanItemArrayMap& mapKIArray, BOOL bShowParents)
{
	ASSERT(pList && pList->GetSafeHwnd());

	if (!pList || !pList->GetSafeHwnd())
		return FALSE;

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
					int nItem = pList->AddTask(pKI->sTitle, pKI->dwTaskID, FALSE);
					ASSERT(nItem != -1);
				}
			}
		}
	}
	
	pList->SetRedraw(TRUE);

	return TRUE;
}

void CKanbanCtrl::BuildTaskMap(const ITaskList15* pTasks, HTASKITEM hTask, 
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

void CKanbanCtrl::RemoveDeletedTasks(const ITaskList15* pTasks)
{
	CSet<DWORD> mapIDs;
	BuildTaskMap(pTasks, pTasks->GetFirstTask(NULL), mapIDs, TRUE);

	POSITION pos = m_data.GetStartPosition();
	DWORD dwTaskID = 0;
	KANBANITEM* pKI = NULL;

	while (pos)
	{
		m_data.GetNextAssoc(pos, dwTaskID, pKI);

		if (!mapIDs.HasKey(dwTaskID))
		{
			// Find the list containing this task and delete it
			int nList = m_aListCtrls.GetSize();
			
			while (nList--)
			{
				int nItem = m_aListCtrls[nList]->FindTask(dwTaskID);

				if (nItem != -1)
				{
					m_aListCtrls[nList]->DeleteItem(nItem);
					break;
				}
			}

			m_data.RemoveKey(dwTaskID);
		}
	}
}

KANBANITEM* CKanbanCtrl::GetKanbanItem(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);
	
	KANBANITEM* pKI = NULL;

	if (dwTaskID && m_data.Lookup(dwTaskID, pKI))
	{
		ASSERT(pKI);
 	}
 
 	return pKI;
}

BOOL CKanbanCtrl::HasKanbanItem(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);

	KANBANITEM* pKI = NULL;

	return (dwTaskID && m_data.Lookup(dwTaskID, pKI));
}

CKanbanListCtrl* CKanbanCtrl::LocateTask(DWORD dwTaskID) const
{
	int nUnused = -1;
	return LocateTask(dwTaskID, nUnused);
}

CKanbanListCtrl* CKanbanCtrl::LocateTask(DWORD dwTaskID, int& nItem) const
{
	int nList = m_aListCtrls.GetSize();
	
	while (nList--)
	{
		CKanbanListCtrl* pList = m_aListCtrls[nList];
		ASSERT(pList);

		nItem = pList->FindTask(dwTaskID);

		if (nItem != -1)
			return pList;
	}

	// else
	nItem = -1;
	return NULL;
}

CKanbanListCtrl* CKanbanCtrl::GetListCtrl(const CString& sAttribValue) const
{
	CString sAttribValueID(Misc::ToUpper(sAttribValue));
	int nList = m_aListCtrls.GetSize();
	
	while (nList--)
	{
		CKanbanListCtrl* pList = m_aListCtrls[nList];
		ASSERT(pList);
		
		if (pList && (pList->GetAttributeValueID() == sAttribValueID))
			return pList;
	}

	// not found
	return NULL;
}

CKanbanListCtrl* CKanbanCtrl::GetListCtrl(HWND hwnd) const
{
	ASSERT(hwnd);

	int nList = m_aListCtrls.GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = m_aListCtrls[nList];
		ASSERT(pList);

		if (pList && (pList->GetSafeHwnd() == hwnd))
			return pList;
	}

	// not found
	return NULL;
}

void CKanbanCtrl::SetDisplayAttributes(const CKanbanAttributeArray& aAttrib)
{
	if (!Misc::MatchAllT(m_aDisplayAttrib, aAttrib))
	{
		m_aDisplayAttrib.Copy(aAttrib);
		
		int nList = m_aListCtrls.GetSize();
		
		while (nList--)
		{
			CKanbanListCtrl* pList = m_aListCtrls[nList];
			ASSERT(pList);
			
			if (pList)
				pList->OnDisplayAttributeChanged();
		}
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

			case KBCF_TASKTEXTCOLORISBKGND:
				{
					int nList = m_aListCtrls.GetSize();

					while (nList--)
					{
						CKanbanListCtrl* pList = m_aListCtrls[nList];
						ASSERT(pList);

						if (pList)
							pList->SetTextColorIsBackground(bSet);
					}
				}
				break;

			case KBCF_COLORBARBYPRIORITY:
				{
					int nList = m_aListCtrls.GetSize();

					while (nList--)
					{
						CKanbanListCtrl* pList = m_aListCtrls[nList];
						ASSERT(pList);

						if (pList)
							pList->SetColorTasksByPriority(bSet);
					}
				}
				break;

			case KBCF_STRIKETHRUDONETASKS:
				{
					int nList = m_aListCtrls.GetSize();

					while (nList--)
					{
						CKanbanListCtrl* pList = m_aListCtrls[nList];
						ASSERT(pList);

						if (pList)
							pList->SetStrikeThruDoneTasks(bSet);
					}
				}
				break;

			case KBCF_SHOWEMPTYCOLUMNS:
			case KBCF_ALWAYSSHOWBACKLOG:
				if (m_aListCtrls.GetSize())
					RebuildListCtrls(FALSE);
				break;

			case KBCF_SHOWTASKCOLORASBAR:
				int nList = m_aListCtrls.GetSize();
				
				while (nList--)
				{
					CKanbanListCtrl* pList = m_aListCtrls[nList];
					ASSERT(pList);
					
					if (pList)
						pList->SetShowTaskColorAsBar(bSet);
				}
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
	int nList = m_aListCtrls.GetSize();
	
	if (nList)
	{
		// Clip out the list controls
		while (nList--)
		{
			CKanbanListCtrl* pList = m_aListCtrls[nList];
			ASSERT(pList);
			
			CDialogHelper::ExcludeChild(pList, pDC);
		}
		
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
// 		TRACE(_T("CKanbanCtrl::SetFocus(OnSetFocus: %s)\n"), pList->GetAttributeValue());
// 
// 		CDWordArray aSelIDs;
// 		pList->GetSelectedTasks(aSelIDs);
// 		int breakpoint = 0;
	}
}

int CKanbanCtrl::GetVisibleListCtrlCount() const
{
	if (UsingDynamicColumns() || HasOption(KBCF_SHOWEMPTYCOLUMNS))
		return m_aListCtrls.GetSize();

	// Fixed columns
	BOOL bShowBacklog = HasOption(KBCF_ALWAYSSHOWBACKLOG);
	int nList = m_aListCtrls.GetSize(), nNumVis = 0;
	
	while (nList--)
	{
		CKanbanListCtrl* pList = m_aListCtrls[nList];
		ASSERT(pList);
			
		if (pList->GetItemCount() || (bShowBacklog && pList->IsBacklog()))
			nNumVis++;
	}

	return nNumVis;
}

void CKanbanCtrl::Resize(const CRect& rect)
{
	int nNumLists = m_aListCtrls.GetSize();
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

		// Also update tab order as we go
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
			pList->SetColumnWidth(0, (rList.Width() - 1 - GetSystemMetrics(SM_CXVSCROLL)));

			pPrev = pList;
			nVis++;
		}
	}
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

		int nList = m_aListCtrls.GetSize();

		while (nList--)
		{
			CKanbanListCtrl* pList = m_aListCtrls[nList];
			ASSERT(pList);

			if (pList)
				pList->Sort(nBy, m_bSortAscending);
		}
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

int CKanbanCtrl::Compare(const CString& sText1, const CString& sText2)
{
	BOOL bEmpty1 = sText1.IsEmpty();
	BOOL bEmpty2 = sText2.IsEmpty();
		
	if (bEmpty1 != bEmpty2)
		return (bEmpty1 ? 1 : -1);
	
	return Misc::NaturalCompare(sText1, sText2);
}

void CKanbanCtrl::ScrollToSelectedTask()
{
	CKanbanListCtrl* pList = GetSelListCtrl();

	if (pList)
		pList->ScrollToSelection();
}

bool CKanbanCtrl::PrepareNewTask(ITaskList* /*pTask*/) const
{
// 	HTASKITEM hNewTask = pTask->GetFirstTask();
// 	ASSERT(hNewTask);

	return true;
}

DWORD CKanbanCtrl::HitTestTask(const CPoint& ptScreen) const
{
	const CKanbanListCtrl* pList = HitTestListCtrl(ptScreen);

	if (pList)
	{
		int nItem = pList->FindTask(ptScreen);

		if (nItem != -1)
			return pList->GetItemData(nItem);
	}

	// else
	return 0;
}

DWORD CKanbanCtrl::GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const
{
	int nItem = -1;
	const CKanbanListCtrl* pList = LocateTask(dwTaskID, nItem);
	
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
	ASSERT(pList);

	int nList = Misc::FindT(m_aListCtrls, pList);
	
	if (nList == -1)
	{
		ASSERT(0);
		return NULL;
	}

	if (bNext)
	{
		for (int nNext = (nList + 1); nNext < m_aListCtrls.GetSize(); nNext++)
		{
			const CKanbanListCtrl* pNext = m_aListCtrls[nNext];

			if (bExcludeEmpty && !pNext->GetItemCount())
				continue;
			
			if (UsingFixedColumns() && !pNext->IsWindowVisible())
				continue;

			// else
			return pNext;
		}
	}
	else // prev
	{
		int nPrev(nList);

		while (nPrev--)
		{
			const CKanbanListCtrl* pPrev = m_aListCtrls[nPrev];
			
			if (bExcludeEmpty && !pPrev->GetItemCount())
				continue;
			
			if (UsingFixedColumns() && !pPrev->IsWindowVisible())
				continue;
			
			// else
			return pPrev;
		}
	}

	return NULL;
}

CKanbanListCtrl* CKanbanCtrl::HitTestListCtrl(const CPoint& ptScreen, BOOL* pbHeader) const
{
	HWND hwnd = ::WindowFromPoint(ptScreen);

	if (hwnd == NULL)
		return NULL;

	CKanbanListCtrl* pList = GetListCtrl(hwnd);

	// If not a list, try for a list's header
	if (!pList && pbHeader && CWinClasses::IsClass(hwnd, WC_HEADER))
	{
		hwnd = ::GetParent(hwnd);
		pList = GetListCtrl(hwnd);

		*pbHeader = (pList != NULL);
	}

	return pList;
}

BOOL CKanbanCtrl::IsDragging() const
{
	ASSERT((m_pDragFromList && (m_aDragItems.GetSize() > 0)) ||
			(!m_pDragFromList && (m_aDragItems.GetSize() == 0)));

	return (m_pDragFromList && (m_aDragItems.GetSize() > 0));
}

BOOL CKanbanCtrl::NotifyParentAttibuteChange(LPCTSTR szValue)
{
	ASSERT(!m_bReadOnly);
//	ASSERT(GetSelectedTaskID());

	return GetParent()->SendMessage(WM_KBC_VALUECHANGE, (WPARAM)GetSafeHwnd(), (LPARAM)szValue);
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

		m_aDragItems.RemoveAll();
		m_pDragFromList = NULL;

		return TRUE;
	}
	
	// else 
	return FALSE;
}


int CKanbanCtrl::GetColumnOrder(CStringArray& aOrder) const
{
	// TODO
	return 0;
}

BOOL CKanbanCtrl::SetColumnOrder(const CStringArray& aTreeOrder)
{
	// TODO
	return FALSE;
}

/*
void CKanbanCtrl::GetColumnWidths(CIntArray& aTreeWidths, CIntArray& aListWidths) const
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

void CKanbanCtrl::SetColumnWidths(const CIntArray& aTreeWidths, const CIntArray& aListWidths)
{
	m_treeHeader.SetItemWidths(aTreeWidths);

	// save list column widths for when we've initialised our columns
	// remember to include hidden dummy first column
	if (aListWidths.GetSize() == (GetRequiredColumnCount() + 1))
		m_listHeader.SetItemWidths(aListWidths);
	else
		m_aPrevColWidths.Copy(aListWidths);
}
*/

void CKanbanCtrl::OnListSetFocus(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	// Ignore focus changes during drag and drop
	if (!IsDragging())
	{
		CKanbanListCtrl* pList = GetListCtrl(pNMHDR->hwndFrom);
		
		SelectListCtrl(pList);
	}
}

BOOL CKanbanCtrl::SelectListCtrl(CKanbanListCtrl* pList)
{
	if (pList && (pList != m_pSelectedList) && pList->GetItemCount())
	{
		m_pSelectedList = pList;
		//TRACE(_T("CKanbanCtrl::OnSetFocusToList(%s)\n"), m_pSelectedList->GetAttributeValue());

		ClearOtherListSelections(m_pSelectedList);

		if (m_pSelectedList->GetSelectedCount() == 0)
		{
			// Select the first visible item
			int nFirstVis = m_pSelectedList->GetTopIndex();
			ASSERT(nFirstVis != -1);
		
			m_pSelectedList->SetItemState(nFirstVis, (LVIS_FOCUSED | LVIS_SELECTED), (LVIS_FOCUSED | LVIS_SELECTED));
			m_pSelectedList->Invalidate(TRUE);
		}
		else
		{
			// Scroll to the current selection
			POSITION pos = m_pSelectedList->GetFirstSelectedItemPosition();
			int nFirstSel = m_pSelectedList->GetNextSelectedItem(pos);
		
			m_pSelectedList->EnsureVisible(nFirstSel, FALSE);
		}

		NotifyParentSelectionChange();

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
		
		if ((pNMLV->uChanged & LVIF_STATE) && 
			((pNMLV->uNewState & LVIS_SELECTED) || (pNMLV->uOldState & LVIS_SELECTED)))
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

void CKanbanCtrl::ClearOtherListSelections(const CKanbanListCtrl* pIgnore)
{
	int nList = m_aListCtrls.GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = m_aListCtrls[nList];

		if (pList != pIgnore)
		{
			pList->SetItemState(-1, 0, (LVIS_FOCUSED | LVIS_SELECTED));
		}
#ifdef _DEBUG
		else
		{
 			int breakpoint = 0;
		}
#endif

		pList->SetSelected(pList == pIgnore);
	}
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
		POSITION pos = pList->GetFirstSelectedItemPosition();
		
		while (pos)
			m_aDragItems.Add(pList->GetNextSelectedItem(pos));
		
		if (m_aDragItems.GetSize())
		{
			// This will prevent any selection updates
			m_pDragFromList = pList;

			// If the 'drag-from' list is not currently selected
			// we select it and then reset the selection to the
			// items we have just copied
			if (pList != m_pSelectedList)
			{
				CDWordArray aTaskIDs;
				int nItem = m_aDragItems.GetSize();
			
				while (nItem--)
					aTaskIDs.Add(pList->GetItemData(m_aDragItems[nItem]));

				VERIFY(pList->SelectTasks(aTaskIDs));
				SelectListCtrl(pList);
			}

			SetCapture();
		}
	}
	
	*pResult = 0;
}

void CKanbanCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (IsDragging())
	{
		ASSERT(!m_bReadOnly);

		// get the list under the mouse
		ClientToScreen(&point);
		CKanbanListCtrl* pDestList = HitTestListCtrl(point);

		if (pDestList && (pDestList != m_pDragFromList))
		{
			// move the items
			CString sAttribValue;
			
			if (GetListCtrlAttributeValue(pDestList, point, sAttribValue))
			{
				// Deselect existing target selection
				m_pSelectedList = pDestList;
				pDestList->SetItemState(-1, 0, (LVIS_FOCUSED | LVIS_SELECTED));

				// 1. Copy the source tasks to the destination
				int nDrag = m_aDragItems.GetSize();
				
				while (nDrag--)
				{
					int nDragItem = m_aDragItems[nDrag];
					DWORD dwDragID = m_pDragFromList->GetItemData(nDragItem);
					ASSERT(dwDragID);
					
					KANBANITEM* pKI = GetKanbanItem(dwDragID);
					ASSERT(pKI);
					
					if (pKI)
					{
						pKI->SetAttributeValue(m_sTrackAttribID, sAttribValue);
						
						int nTask = pDestList->AddTask(pKI->sTitle, dwDragID, TRUE);
						pDestList->SetItemState(nTask, (LVIS_FOCUSED | LVIS_SELECTED), (LVIS_FOCUSED | LVIS_SELECTED));
					}
				}

				// 2. Remove the source tasks
				nDrag = m_aDragItems.GetSize();
				
				while (nDrag--)
				{
					int nDragItem = m_aDragItems[nDrag];

					// remove from src list
					m_pDragFromList->DeleteItem(nDragItem);
					m_pDragFromList->RefreshColumnTitle();
				}

				// Handle 'From' list now being empty
				if (!WantShowColumn(m_pDragFromList))
				{
					if (UsingDynamicColumns())
					{
						int nList = Misc::FindT(m_aListCtrls, m_pDragFromList);
						ASSERT(nList != -1);
						
						m_aListCtrls.RemoveAt(nList);
					}
					
					// Fixed columns handled in Resize()
					Resize();
				}
				
				// Very important to make sure that the correct tasks
				// are selected before sending the change notification
				NotifyParentSelectionChange();
				NotifyParentAttibuteChange(sAttribValue);
			}
		}

		ReleaseCapture();
	}

	// reset always
	m_aDragItems.RemoveAll();
	m_pDragFromList = NULL;

	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CKanbanCtrl::GetListCtrlAttributeValue(CKanbanListCtrl* pDestList, const CPoint& ptScreen, CString& sValue)
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
										ptScreen.x, ptScreen.y, pDestList);

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

		BOOL bValidDest = (pDestList && (pDestList != m_pDragFromList));
		SetCursor(GraphicsMisc::OleDragDropCursor(bValidDest ? GMOC_MOVE : GMOC_NO));
	}

	CWnd::OnMouseMove(nFlags, point);
}
