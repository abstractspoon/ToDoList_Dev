// TaskClipboard.cpp: implementation of the CTaskClipboard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskClipboard.h"
#include "ToDoitem.h"

#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
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

CDWordArray CTaskClipboard::s_aSelTaskIDs;

//////////////////////////////////////////////////////////////////////

void CTaskClipboard::Reset()
{
	if (!IsEmpty())
		::EmptyClipboard();

	s_aSelTaskIDs.RemoveAll();
}

BOOL CTaskClipboard::IsEmpty()
{
	return !CClipboard::HasFormat(GetTasklistIDClipFmt());
}

BOOL CTaskClipboard::SetTasks(const CTaskFile& tasks, const CString& sID, const CDWordArray& aSelTaskIDs, const CString& sValues, TDC_COLUMN nColID)
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
			s_aSelTaskIDs.Copy(aSelTaskIDs);

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
	if (sRefTasklistID.IsEmpty())
		return FALSE;
	
	CString sClipID = GetTasklistID();
	BOOL bMatches = (Misc::ToUpper(sRefTasklistID) == sClipID);

	// TEMPORARY LOGGING
	if (sRefTasklistID != DEF_CLIPID)
		FileMisc::LogText(_T("CTaskClipboard::ClipIDMatches(%s, %s) = %d"), sClipID, sRefTasklistID, bMatches);
	
	return bMatches;
}

BOOL CTaskClipboard::HasTasks()
{
	return (CClipboard::HasFormat(GetTasksClipFmt()) &&	!HasColumnTasks());
}

TDC_COLUMN CTaskClipboard::GetColumnID()
{
	if (!IsEmpty())
	{
		CString sColID = CClipboard().GetText(GetColumnIDClipFmt());

		if (!sColID.IsEmpty())
			return (TDC_COLUMN)_ttoi(sColID);
	}

	// All else
	return TDCC_NONE;
}

BOOL CTaskClipboard::HasColumnTasks()
{
	return (GetColumnID() != TDCC_NONE);
}

BOOL CTaskClipboard::HasColumnTasks(TDC_COLUMN& nColID)
{
	nColID = GetColumnID();

	return (nColID != TDCC_NONE);
}

BOOL CTaskClipboard::HasAttributeTask()
{
	return GetAttributeTask(CTaskFile()) != NULL;
}

int CTaskClipboard::GetTasks(const CString& sRefTasklistID, CTaskFile& tasks)
{
	return GetTasks(sRefTasklistID, tasks, CDWordArray());
}

BOOL CTaskClipboard::GetTasks(const CString& sRefTasklistID, CTaskFile& tasks, CDWordArray& aSelTaskIDs)
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

	aSelTaskIDs.Copy(s_aSelTaskIDs);

	// remove task references if the clip IDs do not match
	if (!TasklistIDMatches(sRefTasklistID))
	{
		CDWordSet mapSelTaskIDs;
		mapSelTaskIDs.CopyFrom(aSelTaskIDs);

		RemoveTaskReferences(tasks, tasks.GetFirstTask(), TRUE, mapSelTaskIDs);

		// Sync returned task IDs
		int nID = aSelTaskIDs.GetSize();

		while (nID--)
		{
			if (!mapSelTaskIDs.Has(aSelTaskIDs[nID]))
				aSelTaskIDs.RemoveAt(nID);
		}
	}

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
	if (!GetTasks(DEF_CLIPID, task) || (task.GetTaskCount() != 1))
		return NULL;

	return task.GetFirstTask();
}

void CTaskClipboard::RemoveTaskReferences(CTaskFile& tasks, HTASKITEM hTask, BOOL bAndSiblings, CDWordSet& mapSelTaskIDs)
{
	if (!hTask)
		return;

	// process children first before their parent is potentially removed
	RemoveTaskReferences(tasks, tasks.GetFirstTask(hTask), TRUE, mapSelTaskIDs);

	// Handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = tasks.GetNextTask(hTask);
		
		while (hSibling)
		{
			// Grab next sibling before we potentially delete this one
			HTASKITEM hNextSibling = tasks.GetNextTask(hSibling);

			// FALSE == don't recurse on siblings
			RemoveTaskReferences(tasks, hSibling, FALSE, mapSelTaskIDs);
			
			hSibling = hNextSibling;
		}
	}

	// delete if reference
	if (tasks.GetTaskReferenceID(hTask) > 0)
	{
		mapSelTaskIDs.Remove(tasks.GetTaskID(hTask));
		tasks.DeleteTask(hTask);
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
