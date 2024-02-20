// TaskClipboard.cpp: implementation of the CTaskClipboard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskClipboard.h"

#include "..\shared\misc.h"
#include "..\shared\clipboard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const LPCTSTR TDL_CLIPFMTTASKS = _T("CF_TODOLIST_TASKS");
const LPCTSTR TDL_CLIPFMTID = _T("CF_TODOLIST_ID");
const LPCTSTR DEF_CLIPID = _T("_emptyID_");

//////////////////////////////////////////////////////////////////////

void CTaskClipboard::Reset()
{
	if (!IsEmpty())
		::EmptyClipboard();
}

BOOL CTaskClipboard::IsEmpty()
{
	return !CClipboard::HasFormat(GetIDClipFmt());
}

BOOL CTaskClipboard::SetTasks(const CTaskFile& tasks, const CString& sID, const CString& sTaskTitles)
{
	ASSERT(tasks.GetTaskCount());
	ASSERT(!sTaskTitles.IsEmpty());
	
	CString sXML; 
	CString sClipID = (sID.IsEmpty() ? DEF_CLIPID : sID);
	
	if (tasks.Export(sXML) && !sXML.IsEmpty())
	{
		sClipID.MakeUpper();

		CClipboard cb(GetMainWnd());

		return (cb.SetText(sXML, GetTaskClipFmt()) && 
				cb.SetText(sClipID, GetIDClipFmt()) &&
				cb.SetText(sTaskTitles));
	}
	
	// else
	return FALSE;
}

BOOL CTaskClipboard::ClipIDMatches(const CString& sID)
{
	return (!sID.IsEmpty() && (sID.CompareNoCase(GetClipID()) == 0));
}

int CTaskClipboard::GetTasks(CTaskFile& tasks, const CString& sID)
{
	if (IsEmpty())
		return 0;

	CString sXML = CClipboard().GetText(GetTaskClipFmt()); 
	
	if (!tasks.LoadContent(sXML))
		return 0;

	CString sClipID = (sID.IsEmpty() ? DEF_CLIPID : sID);

	// remove task references if the clip IDs do not match
	if (sClipID.CompareNoCase(GetClipID()) != 0)
		RemoveTaskReferences(tasks, tasks.GetFirstTask(), TRUE);

	return tasks.GetTaskCount();
}

int CTaskClipboard::GetTaskCount(const CString& sID)
{
	if (IsEmpty())
		return 0;

	CTaskFile unused;
	return GetTasks(unused, sID);
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

CString CTaskClipboard::GetClipID()
{
	return CClipboard().GetText(GetIDClipFmt()); 
}

UINT CTaskClipboard::GetTaskClipFmt()
{
	static UINT nClip = ::RegisterClipboardFormat(TDL_CLIPFMTTASKS);
	return nClip;
}

UINT CTaskClipboard::GetIDClipFmt()
{
	static UINT nClip = ::RegisterClipboardFormat(TDL_CLIPFMTID);
	return nClip;
}

HWND CTaskClipboard::GetMainWnd()
{
	return (*AfxGetMainWnd());
}
