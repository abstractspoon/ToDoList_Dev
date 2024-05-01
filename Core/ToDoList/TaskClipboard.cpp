// TaskClipboard.cpp: implementation of the CTaskClipboard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskClipboard.h"
#include "ToDoitem.h"

#include "..\shared\misc.h"
#include "..\shared\clipboard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const LPCTSTR TDLCF_TASKS		= _T("CF_TODOLIST_TASKS");
const LPCTSTR TDLCF_TASKLISTID	= _T("CF_TODOLIST_TASKLISTID");
const LPCTSTR TDLCF_COLUMNID	= _T("CF_TODOLIST_COLUMNID");
const LPCTSTR DEF_CLIPID		= _T("_emptyID_");

//////////////////////////////////////////////////////////////////////

void CTaskClipboard::Reset()
{
	if (!IsEmpty())
		::EmptyClipboard();
}

BOOL CTaskClipboard::IsEmpty()
{
	return !CClipboard::HasFormat(GetTasklistIDClipFmt());
}

BOOL CTaskClipboard::SetTasks(const CTaskFile& tasks, const CString& sID, const CString& sValues, TDC_COLUMN nColID)
{
	// Sanity checks
	if (!tasks.GetTaskCount() || sID.IsEmpty() || (sValues.IsEmpty() && (nColID == TDCC_NONE)))
	{
		ASSERT(0);
		return FALSE;
	}
	
	CString sXML; 
	
	if (tasks.Export(sXML) && !sXML.IsEmpty())
	{
		CClipboard cb(GetMainWnd());

		if (cb.SetText(sXML, GetTasksClipFmt()) && 
			cb.SetText(Misc::ToUpper(sID), GetTasklistIDClipFmt()) &&
			cb.SetText(sValues))
		{
			// Optional column identifier
			if (nColID != TDCC_NONE)
				return cb.SetText(Misc::Format((int)nColID), GetColumnIDClipFmt());

			return TRUE;
		}
	}
	
	// else
	return FALSE;
}

BOOL CTaskClipboard::TasklistIDMatches(const CString& sRefTasklistID)
{
	return (!sRefTasklistID.IsEmpty() && (sRefTasklistID.CompareNoCase(GetTasklistID()) == 0));
}

BOOL CTaskClipboard::HasTasks()
{
	return (CClipboard::HasFormat(GetTasksClipFmt()) &&	!HasColumnTasks());
}

BOOL CTaskClipboard::HasColumnTasks()
{
	TDC_COLUMN nUnused;
	return HasColumnTasks(nUnused);
}

BOOL CTaskClipboard::HasColumnTasks(TDC_COLUMN& nColID)
{
	if (IsEmpty())
		return FALSE;

	CString sColID = CClipboard().GetText(GetColumnIDClipFmt());

	if (sColID.IsEmpty())
		return FALSE;
	
	nColID = (TDC_COLUMN)_ttoi(sColID);
	return TRUE;
}

BOOL CTaskClipboard::HasAttributeTask()
{
	return GetAttributeTask(CTaskFile()) != NULL;
}

BOOL CTaskClipboard::GetTasks(const CString& sRefTasklistID, CTaskFile& tasks)
{
	ASSERT(!sRefTasklistID.IsEmpty());

	if (IsEmpty())
		return FALSE;

	// Exclude 'column tasks'
	if (CClipboard::HasFormat(GetColumnIDClipFmt()))
		return FALSE;

	CString sXML = CClipboard().GetText(GetTasksClipFmt()); 
	
	if (!tasks.LoadContent(sXML) || !tasks.GetTaskCount())
		return FALSE;

	// remove task references if the clip IDs do not match
	if (!TasklistIDMatches(sRefTasklistID))
		RemoveTaskReferences(tasks, tasks.GetFirstTask(), TRUE);

	return TRUE;
}

TDC_COLUMN CTaskClipboard::GetColumnTasks(CTaskFile& tasks)
{
	TDC_COLUMN nColID = TDCC_NONE;

	if (HasColumnTasks(nColID))
	{
		CString sXML = CClipboard().GetText(GetTasksClipFmt());

		if (!tasks.LoadContent(sXML) || !tasks.GetTaskCount())
			nColID = TDCC_NONE;
	}

	return nColID;
}

HTASKITEM CTaskClipboard::GetAttributeTask(CTaskFile& task)
{
	CTaskFile tasks;

	if (!GetTasks(DEF_CLIPID, tasks) || (tasks.GetTaskCount() != 1))
		return NULL;

	return tasks.GetFirstTask();
}

void CTaskClipboard::RemoveTaskReferences(CTaskFile& tasks, HTASKITEM hTask, BOOL bAndSiblings)
{
	if (!hTask)
		return;

	// handle next sibling first in case we want to delete hTask
	// WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = tasks.GetNextTask(hTask);
		
		while (hSibling)
		{
			// Grab next sibling before we potentially delete this one
			HTASKITEM hNextSibling = tasks.GetNextTask(hSibling);

			// FALSE == don't recurse on siblings
			RemoveTaskReferences(tasks, hSibling, FALSE);
			
			hSibling = hNextSibling;
		}
	}

	// delete if reference
	if (tasks.GetTaskReferenceID(hTask))
	{
		tasks.DeleteTask(hTask);
	}
	else // process children
	{
		RemoveTaskReferences(tasks, tasks.GetFirstTask(hTask), TRUE);
	}
}

CString CTaskClipboard::GetTasklistID()
{
	return CClipboard().GetText(GetTasklistIDClipFmt()); 
}

UINT CTaskClipboard::GetTasksClipFmt()
{
	static UINT nClip = ::RegisterClipboardFormat(TDLCF_TASKS);
	return nClip;
}

UINT CTaskClipboard::GetTasklistIDClipFmt()
{
	static UINT nClip = ::RegisterClipboardFormat(TDLCF_TASKLISTID);
	return nClip;
}

UINT CTaskClipboard::GetColumnIDClipFmt()
{
	static UINT nClip = ::RegisterClipboardFormat(TDLCF_COLUMNID);
	return nClip;
}

HWND CTaskClipboard::GetMainWnd()
{
	return (*AfxGetMainWnd());
}
