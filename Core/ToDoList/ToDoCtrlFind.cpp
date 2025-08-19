// ToDoCtrlData.cpp: implementation of the CToDoCtrlFind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlFind.h"
#include "ToDoCtrlData.h"
#include "ToDoitem.h"

#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\enstring.h"
#include "..\shared\treectrlhelper.h"

#include <float.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlFind::CToDoCtrlFind(const CTreeCtrlHelper& tch, 
							 const CToDoCtrlData& data, 
							 const CTDCCustomAttribDefinitionArray& aCustAttribDefs,
							 const CTDCReminderHelper& reminders,
							 const CContentMgr& mgrContent)
	:
	m_tch(tch), 
	m_data(data),
	m_aCustAttribDefs(aCustAttribDefs),
	m_mgrContent(mgrContent),
	m_matcher(data, reminders, mgrContent),
	m_calculator(data)
{
}

CToDoCtrlFind::~CToDoCtrlFind()
{
}

DWORD CToDoCtrlFind::GetTaskID(HTREEITEM hti) const 
{ 
	if (!hti || (hti == TVI_ROOT) || (hti == TVI_FIRST) || (hti == TVI_LAST))
		return 0;
	
	if (m_tch.TreeCtrl().GetCount() == 0)
		return 0;
	
	return m_tch.TreeCtrl().GetItemData(hti); 
}

const TODOITEM* CToDoCtrlFind::GetTask(HTREEITEM hti, BOOL bTrue) const
{
	DWORD dwTaskID = GetTaskID(hti);

	if (bTrue)
		return m_data.GetTrueTask(dwTaskID);

	// else
	return m_data.GetTask(dwTaskID);
}

BOOL CToDoCtrlFind::CheckGetTask(HTREEITEM hti, const TODOITEM*& pTDI, BOOL bTrueTask) const
{
	if (hti && !pTDI)
	{
		pTDI = GetTask(hti, bTrueTask);

		if (!pTDI)
		{
			ASSERT(0);
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CToDoCtrlFind::CheckGetTask(HTREEITEM hti, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const
{
	if (hti && (!pTDI || !pTDS))
	{
		DWORD dwTaskID = GetTaskID(hti);

		if (!m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
		{
			ASSERT(0);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CToDoCtrlFind::WantSearchChildren(HTREEITEM hti, BOOL bVisibleOnly) const
{
	if (m_tch.TreeCtrl().GetChildItem(hti) == NULL)
		return FALSE;

	if (hti == NULL)
		return TRUE; // always expanded

	if (!bVisibleOnly)
		return TRUE;

	// else
	return m_tch.IsItemExpanded(hti);
}

int CToDoCtrlFind::GetTaskBreadcrumbs(HTREEITEM hti, CDWordArray& aBreadcrumbs, DWORD dwFlags) const
{
	if (!hti)
	{
		ASSERT(0);
		return 0;
	}
	
	if (!Misc::HasFlag(dwFlags, TCFBC_APPEND))
		aBreadcrumbs.RemoveAll();

	BOOL bUp = Misc::HasFlag(dwFlags, TCFBC_UP);
	BOOL bParentsOnly = Misc::HasFlag(dwFlags, TCFBC_PARENTSONLY);
	BOOL bVisibleOnly = Misc::HasFlag(dwFlags, TCFBC_VISIBLEONLY);
	
	// progressively work our way up the parent chain
	// checking the parent's siblings at each level.
	// Note: we don't search into each task as we traverse it
	if (bUp) // this is just like the task's path, from the task back to the root
	{
		HTREEITEM htiPrev;

		while (hti)
		{
			if (bParentsOnly)
				htiPrev = m_tch.TreeCtrl().GetParentItem(hti);

			else if (bVisibleOnly)
				htiPrev = m_tch.GetPrevVisibleItem(hti, FALSE);
			else
				htiPrev = m_tch.GetPrevItem(hti, FALSE);
			
			if (htiPrev == NULL || htiPrev == TVI_ROOT)
				break;
			
			// insert taskid at start
			aBreadcrumbs.InsertAt(0, GetTaskID(htiPrev));
			
			// keep going
			hti = htiPrev;
		}
	}
	else // this is from the task forward to the last parent's siblings
	{
		HTREEITEM htiNext;

		if (bParentsOnly)
		{
			// we don't include our parent
			htiNext = m_tch.TreeCtrl().GetParentItem(hti);

			while (htiNext)
			{
				htiNext = m_tch.TreeCtrl().GetParentItem(htiNext);

				if (htiNext == NULL)
					break;

				aBreadcrumbs.InsertAt(0, GetTaskID(htiNext));
			}
		}
		else // parents and siblings, but not children
		{
			htiNext = hti;

			do
			{
				if (bVisibleOnly)
					htiNext = m_tch.GetNextVisibleItem(htiNext, FALSE);
				else
					htiNext = m_tch.GetNextItem(htiNext, FALSE);

				if (htiNext)
					aBreadcrumbs.InsertAt(0, GetTaskID(htiNext));
			} 
			while (htiNext);
		}
	}
	
	return aBreadcrumbs.GetSize();
}

int CToDoCtrlFind::FindTasks(const SEARCHPARAMS& params, CResultArray& aResults, BOOL bCheckDueToday) const
{
	if (!m_data.GetTaskCount())
		return 0;

	HTREEITEM hti = m_tch.TreeCtrl().GetChildItem(NULL);
	
	while (hti)
	{
		FindTasks(hti, params, aResults, bCheckDueToday);
		hti = m_tch.TreeCtrl().GetNextItem(hti, TVGN_NEXT);
	}
	
	// else
	return aResults.GetSize();
}

void CToDoCtrlFind::FindTasks(HTREEITEM hti, const SEARCHPARAMS& params, CResultArray& aResults, BOOL bCheckDueToday) const
{
	SEARCHRESULT result;
	DWORD dwID = GetTaskID(hti);

	// if the item is done and we're ignoring completed tasks 
	// (and by corollary their children) then we can stop right-away
	if (params.bIgnoreDone && m_calculator.IsTaskDone(dwID))
		return;

	// also we can ignore parent tasks if required but we still need 
	// to process it's children
	if (m_matcher.TaskMatches(dwID, params, bCheckDueToday, result))
	{
		// check for overdue tasks
		if (!params.bIgnoreOverDue || !m_calculator.IsTaskOverDue(dwID))
			aResults.Add(result);
	}
	
	// process children
	HTREEITEM htiChild = m_tch.TreeCtrl().GetChildItem(hti);
		
	while (htiChild)
	{
		FindTasks(htiChild, params, aResults, bCheckDueToday); // RECURSIVE call
			
		// next
		htiChild = m_tch.TreeCtrl().GetNextItem(htiChild, TVGN_NEXT);
	}
}

HTREEITEM CToDoCtrlFind::FindFirstTask(const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards, BOOL bCheckDueToday) const
{
	if (!m_data.GetTaskCount())
		return 0;

	HTREEITEM htiStart = (bForwards ? m_tch.GetFirstItem() : m_tch.GetLastItem());

	return FindNextTask(htiStart, params, result, bForwards, bCheckDueToday);
}

HTREEITEM CToDoCtrlFind::FindNextTask(HTREEITEM htiStart, const SEARCHPARAMS& params, SEARCHRESULT& result, BOOL bForwards, BOOL bCheckDueToday) const
{
	HTREEITEM hti = htiStart;

	while (hti)
	{
		DWORD dwNextID = GetTaskID(hti);

		if (m_matcher.TaskMatches(dwNextID, params, bCheckDueToday, result))
			return hti;

		// next item
		hti = (bForwards ? m_tch.GetNextItem(hti) : m_tch.GetPrevItem(hti));
	}

	// else
	return NULL; // not found
}


