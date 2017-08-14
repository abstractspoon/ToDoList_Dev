// TDCSourceControlHelper.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TDCSourceControlHelper.h"
#include "ToDoCtrl.h"

#include "..\shared\FileMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CTDCSourceControlHelper

CTDCSourceControl::CTDCSourceControl(const CToDoCtrl& tdc) : m_tdc(tdc)
{
}

CTDCSourceControl::~CTDCSourceControl()
{
}

BOOL CTDCSourceControl::GetTasklistPath(CString& sTasklistPath) const
{
	sTasklistPath = m_tdc.GetFilePath();
	
	ASSERT(!sTasklistPath.IsEmpty());

	return !sTasklistPath.IsEmpty();
}

BOOL CTDCSourceControl::GetTaskSourceControlFolder(CString& sSCCFolder) const
{
	if (!GetTasklistPath(sSCCFolder))
		return FALSE;

	FileMisc::ReplaceExtension(_T(".ssc"));
	return TRUE;
}

BOOL CTDCSourceControl::GetTasklistSourceControlPath(CString& sTasklistSCCPath) const
{
	if (!GetTasklistPath(sTasklistSCCPath))
		return FALSE;

	FileMisc::ReplaceExtension(sTasklistSCCPath, _T(".tsc"));
	return TRUE;
}

BOOL CTDCSourceControl::GetTaskSourceControlPath(DWORD dwTaskID, CString& sTaskSCCPath) const
{
	if (!dwTaskID)
	{
		ASSERT(0);
		return FALSE;
	}

	CString sSCCFolder;
	
	if (!GetTaskSourceControlFolder(sSCCFolder))
		return FALSE;

	sTaskSCCPath.Format(_T("%s\\%ld.tsc"), sSCCFolder, dwTaskID);
	return TRUE;
}

CString CTDCSourceControl::GetSourceControlID(BOOL bAlternate) const
{
	if (m_tdc.HasStyle(TDCS_INCLUDEUSERINCHECKOUT))
	{
		if (!bAlternate)
			return Misc::FormatComputerNameAndUser();

		// else
		return Misc::GetComputerName();
	}

	// else
	if (!bAlternate)
		return Misc::GetComputerName();

	// else
	return Misc::FormatComputerNameAndUser();
}

BOOL CTDCSourceControl::MatchesOurSourceControlID(const CString& sID) const
{
	if (sID.IsEmpty())
		return FALSE;

	return (SourceControlIDsMatch(sID, GetSourceControlID(FALSE)) ||
			SourceControlIDsMatch(sID, GetSourceControlID(TRUE)));
}

BOOL CTDCSourceControl::SourceControlIDsMatch(const CString& sID1, const CString& sID2)
{
	return (sID1.CompareNoCase(sID2) == 0);
}

BOOL CTDCSourceControl::CheckOutTask(DWORD dwTaskID, const TODOITEM& tdi, CString& sTaskCheckedOutTo)
{
	if (!dwTaskID || tdi.IsReference() || m_mapTasksCheckedOut.Has(dwTaskID))
	{
		ASSERT(0);
		return FALSE;
	}

	CString sTaskSCCPath;
	
	if (!GetTaskSourceControlPath(dwTaskID, sTaskSCCPath))
	{
		ASSERT(0);
		return FALSE;
	}

	if (!CreateSentinelFile(sTaskSCCPath))
	{ 
		sTaskCheckedOutTo = GetCheckedOutTo(sTaskSCCPath);
		ASSERT(!sTaskCheckedOutTo.IsEmpty());

		return FALSE;
	}

	if (!SaveCheckedOutTask(sTaskSCCPath, dwTaskID, tdi))
	{
		ASSERT(0);
		return FALSE;
	}

	m_mapTasksCheckedOut.Add(dwTaskID);

	return TRUE;
}

BOOL CTDCSourceControl::SaveCheckedOutTask(DWORD dwTaskID, const TODOITEM& tdi) const
{
	CString sTaskSCCPath;
	VERIFY(GetTaskSourceControlPath(dwTaskID, sTaskSCCPath));

	return SaveCheckedOutTask(sTaskSCCPath, dwTaskID, tdi);
}

BOOL CTDCSourceControl::SaveCheckedOutTask(const CString& sTaskSCCPath, DWORD dwTaskID, const TODOITEM& tdi) const
{
	ASSERT(dwTaskID);
	ASSERT(!tdi.bLocked);
	ASSERT(FileMisc::FileExists(sTaskSCCPath));

	// Save minimal tasklist
	CTaskFile task;
	HTASKITEM hTask = task.NewTask(tdi.sTitle, NULL, dwTaskID, 0);

	if (!hTask)
	{
		ASSERT(0);
		return FALSE;
	}

	task.SetTaskAttributes(hTask, tdi);
	task.SetCheckedOutTo(GetSourceControlID());
	task.SetXmlHeader(m_tdc.m_sXmlHeader);
	task.SetFileFormat(FILEFORMAT_CURRENT);

	if (!task.Save(sTaskSCCPath, SFEF_UTF16))
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CTDCSourceControl::CheckInTask(DWORD dwTaskID, TODOITEM& tdi, CString& sTasklistCheckedOutTo)
{
	if (!dwTaskID || !m_mapTasksCheckedOut.Has(dwTaskID))
	{
		ASSERT(0);
		return FALSE;
	}

	CString sTaskSCCPath = GetTaskSourceControlPath(dwTaskID); 

	if (!FileMisc::FileExists(sTaskSCCPath))
	{
		ASSERT(0);
		return FALSE;
	}

	if (!m_bTasklistCheckedOut)
	{
		// Must be able to checkout tasklist
		CString sTasklistSCCPath = GetTasklistSourceControlPath(); 
		CTaskFile tasks;

		if (!CheckOutTasklist(tasks, sTasklistCheckedOutTo))
		{
			// Not unexpected
			return FALSE;
		}

		HTASKITEM hTask = tasks.FindTask(dwTaskID);

		if (!hTask || !tasks.SetTaskAttributes(hTask, tdi))
		{
			ASSERT(0);
			return FALSE;
		}

		if (!tasks.Save(m_tdc.GetFilePath(), SFEF_UTF16))
		{
			ASSERT(0);
			return FALSE;
		}

		VERIFY(FileMisc::DeleteFile(sTasklistSCCPath));
	}
	
	VERIFY(FileMisc::DeleteFile(sTaskSCCPath));
	m_mapTasksCheckedOut.Remove(dwTaskID);

	return TRUE;
}

BOOL CTDCSourceControl::CheckInAllTasks(CString& sTasklistCheckedOutTo)
{
	// TODO 
	return FALSE;
}

BOOL CTDCSourceControl::LoadCheckedOutTask(DWORD dwTaskID, TODOITEM& tdi)
{
	CString sTaskSCCPath = GetTaskSourceControlPath(dwTaskID);

	return (LoadCheckedOutTask(sTaskSCCPath, tdi) == dwTaskID);
}

DWORD CTDCSourceControl::LoadCheckedOutTask(LPCTSTR szTaskPath, TODOITEM& tdi)
{
	DWORD dwTaskID = GetTaskIDFromFilePath(szTaskPath);

	if (!dwTaskID)
	{
		ASSERT(0);
		return FALSE;
	}

	CTaskFile task;

	if (!task.Load(szTaskPath))
	{
		ASSERT(0);
		return FALSE;
	}

	if (!task.IsCheckedOutTo(GetSourceControlID())) 
	{
		ASSERT(0);
		return FALSE;
	}

	if (!task.GetTaskAttributes(task.GetFirstTask(), tdi))
	{
		ASSERT(0);
		return FALSE;
	}

	tdi.bLocked = FALSE;

	return dwTaskID;
}

BOOL CTDCSourceControl::CheckOutTasklist(CTaskFile& tasks, CString& sTasklistCheckedOutTo)
{
	if (m_bTasklistCheckedOut)
	{
		ASSERT(0);
		return FALSE;
	}

	CString sTasklistSCCPath = GetTasklistSourceControlPath(); 

	if (!CreateSentinelFile(sTasklistSCCPath))
	{ 
		sTasklistCheckedOutTo = GetCheckedOutTo(sTasklistSCCPath);
		ASSERT(!sTasklistCheckedOutTo.IsEmpty());

		return FALSE;
	}

	// Save minimal tasklist
	CTaskFile tasklist;

	tasklist.SetCheckedOutTo(GetSourceControlID());
	tasklist.SetXmlHeader(m_tdc.m_sXmlHeader);
	tasklist.SetFileFormat(FILEFORMAT_CURRENT);

	if (!tasklist.Save(sTasklistSCCPath, SFEF_UTF16))
	{
		VERIFY(DeleteSentinelFile(sTasklistSCCPath));
		return FALSE;
	}

	if (!tasks.Load(m_tdc.GetFilePath()))
	{
		VERIFY(DeleteSentinelFile(sTasklistSCCPath));
		return FALSE;
	}

	return TRUE;
}

BOOL CTDCSourceControl::CheckInTasklist()
{
	// TODO
}

BOOL CTDCSourceControl::GetCheckedOutTo(LPCTSTR szPath, CString& sCheckedOutTo)
{
	if (!FileMisc::FileExists(szPath))
		return FALSE;

	CTaskFile tasklist;

	if (!tasklist.LoadHeader(szPath))
		return FALSE;

	sCheckedOutTo = tasklist.GetCheckOutTo();
	return !sCheckedOutTo.IsEmpty();
}

BOOL CTDCSourceControl::IsTasklistCheckedOut() const
{
	CString sUnused;

	return IsTasklistCheckedOut(sUnused);
}

BOOL CTDCSourceControl::IsTasklistCheckedOut(CString& sTasklistCheckedOutTo) const
{
	CString sTasklistSCCPath = GetTasklistSourceControlPath(); 
	CString sTasklistCheckedOutTo;
	
	return GetCheckedOutTo(sTasklistSCCPath, sTasklistCheckedOutTo);
}

BOOL CTDCSourceControl::IsTasklistCheckedOutToUs() const
{
	if (!m_bTasklistCheckedOut)
		return FALSE;

	CString sTasklistCheckedOutTo;

	if (!IsTasklistCheckedOut(sTasklistCheckedOutTo))
		return FALSE;

	return MatchesOurSourceControlID(sTasklistCheckedOutTo);
}

BOOL CTDCSourceControl::IsTaskCheckedOut(DWORD dwTaskID) const
{
	CString sUnused;

	return IsTaskCheckedOut(dwTaskID, sUnused);
}

BOOL CTDCSourceControl::IsTaskCheckedOut(DWORD dwTaskID, CString& sTaskCheckedOutTo) const
{
	CString sTaskSCCPath = GetTaskSourceControlPath(dwTaskID);

	return FileMisc::FileExists(sTaskSCCPath);
	sTaskCheckedOutTo = GetCheckedOutTo(GetTaskSourceControlPath(dwTaskID));

	return !sTaskCheckedOutTo.IsEmpty();
}

BOOL CTDCSourceControl::IsTaskCheckedOutToUs(DWORD dwTaskID) const
{
	CString sTaskCheckedOutTo;

	if (!IsTaskCheckedOut(dwTaskID, sTaskCheckedOutTo))
		return FALSE;

	return MatchesOurSourceControlID(sTaskCheckedOutTo);
}

BOOL CTDCSourceControl::CreateSentinelFile(LPCTSTR szPath)
{
	HFILE hFile = CreateFile(szPath,				// name of the write
							GENERIC_WRITE,			// open for writing
							0,						// do not share
							NULL,					// default security
							CREATE_NEW,				// create new file only
							FILE_ATTRIBUTE_NORMAL,	// normal file
							NULL);					// no attribute template

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		// ERROR_FILE_EXISTS
		return FALSE;
	}

	::CloseHandle(hFile);
	return TRUE;
}

BOOL CTDCSourceControl::DeleteSentinelFile(LPCTSTR szPath)
{
	return FileMisc::DeleteFile(szPath, TRUE);
}

int CTDCSourceControl::RestoreCheckedOutTasks(CToDoCtrlDataItems& tdItems) const
{
	CStringArray aTaskFiles;
	int nFile = FileMisc::FindFiles(GetTaskSourceControlFolder(), aTaskFiles, FALSE, _T("*.tsc"));

	TODOITEM tdi;
	DWORD dwTaskID;
	int nLoaded = 0;

	while (nFile--)
	{
		const CString sTaskSCCPath = aTaskFiles[nFile];

		if (LoadCheckedOutTask(sTaskSCCPath, dwTaskID, tdi))
		{
			// Discard deleted tasks
			if (m_tdc.GetTask(dwTaskID) == NULL)
			{
				DeleteSentinelFile(sTaskSCCPath);
				continue;
			}

			TODOITEM* pTDI = new TODOITEM(tdi);
			pTDI->bLocked = FALSE;

			tdItems.AddTask(dwTaskID, pTDI);
			m_mapTasksCheckedOut.Add(dwTaskID);
		}
	}

	return tdItems.GetCount();
}

BOOL CTDCSourceControl::SaveCheckedOutTasks()
{
	DiscardDeletedCheckedOutTasks();

	POSITION pos = m_mapTasksCheckedOut.GetStartPosition();
	BOOL bSomeFailed = FALSE;

	while (pos)
	{
		DWORD dwTaskID = m_mapTasksCheckedOut.GetNext(pos);
		const TODOITEM* pTDI = m_tdc.GetTask(dwTaskID);
		ASSERT(pTDI);

		if (!SaveCheckedOutTask(dwTaskID, *pTDI))
			bSomeFailed = TRUE;
	}

	return !bSomeFailed;
}

int CTDCSourceControl::DiscardDeletedCheckedOutTasks()
{
	int nNumDiscarded = 0;
	POSITION pos = m_mapTasksCheckedOut.GetStartPosition();

	while (pos)
	{
		DWORD dwTaskID = m_mapTasksCheckedOut.GetNext(pos);
		const TODOITEM* pTDI = m_tdc.GetTask(dwTaskID);

		if (pTDI == NULL)
		{
			CString sTaskSCCPath;
			VERIFY(GetTaskSourceControlPath(dwTaskID, sTaskSCCPath));

			DeleteSentinelFile(sTaskSCCPath);
			m_mapTasksCheckedOut.Remove(dwTaskID);

			nNumDiscarded++;
		}
	}

	return nNumDiscarded;
}

DWORD CTDCSourceControl::GetTaskIDFromFilePath(LPCTSTR szPath)
{
	DWORD dwTaskID = _ttol(FileMisc::GetFileNameFromPath(szPath));
	ASSERT(dwTaskID);

	return dwTaskID;
}
