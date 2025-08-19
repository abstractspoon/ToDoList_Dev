// TaskClipboard.h: interface for the CTaskClipboard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKCLIPBOARD_H__7724479D_9E23_42B2_816F_40FE2B24B9C2__INCLUDED_)
#define AFX_TASKCLIPBOARD_H__7724479D_9E23_42B2_816F_40FE2B24B9C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "taskfile.h"

//////////////////////////////////////////////////////////////////////

class CDWordSet;

//////////////////////////////////////////////////////////////////////

class CTaskClipboard  
{
public:
	static void Reset();
	static BOOL IsEmpty();
	static BOOL TasklistIDMatches(const CString& sRefTasklistID);

	static BOOL SetTasks(const CTaskFile& tasks, const CString& sRefTasklistID, const CDWordArray& aSelTaskIDs, const CString& sValues, TDC_COLUMN nColID = TDCC_NONE);

	static BOOL HasTasks();
	static BOOL HasColumnTasks();
	static BOOL HasColumnTasks(TDC_COLUMN& nColID);
	static BOOL HasAttributeTask();

	static BOOL GetTasks(const CString& sRefTasklistID, CTaskFile& tasks);
	static BOOL GetTasks(const CString& sRefTasklistID, CTaskFile& tasks, CDWordArray& aSelTaskIDs);
	
	static TDC_COLUMN GetColumnTasks(CTaskFile& tasks);
	static TDC_COLUMN GetColumnID();
	static HTASKITEM GetAttributeTask(CTaskFile& task);

protected:
	static CDWordArray s_aSelTaskIDs;

protected:
	static UINT GetTasksClipFmt();
	static UINT GetTasklistIDClipFmt();
	static UINT GetColumnIDClipFmt();

	static CString GetTasklistID();
	static HWND GetMainWnd();
	static void RemoveTaskReferences(CTaskFile& tasks, HTASKITEM hTask, BOOL bAndSiblings, CDWordSet& mapSelTaskIDs);
};

#endif // !defined(AFX_TASKCLIPBOARD_H__7724479D_9E23_42B2_816F_40FE2B24B9C2__INCLUDED_)
