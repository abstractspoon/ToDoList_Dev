// TDCSourceControlHelper.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TDCSourceControl_v2.h"
#include "ToDoCtrl.h"

#include "..\shared\FileMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CTDCSourceControlHelper

CTDCSourceControl::CTDCSourceControl(const CToDoCtrl& tdc) 
	: 
	m_tdc(tdc),
	m_bTasklistSourceControlled(FALSE),
	m_bTasklistCheckedOut(FALSE)
{
}

CTDCSourceControl::~CTDCSourceControl()
{
}

BOOL CTDCSourceControl::Initialise(const CTaskFile& tasks, BOOL bWantCheckout)
{
	m_bTasklistSourceControlled = (bWantCheckout || tasks.IsSourceControlled());

	if (m_bTasklistSourceControlled)
	{
		CString sTasklistPath(tasks.GetFilePath());
		VERIFY(InitialiseSourceControlFolder(sTasklistPath));

		CString sTasklistSSCPath;
		VERIFY(GetSourceControlPath(sTasklistPath, sTasklistSSCPath));

		if (MatchesOurSourceControlID(GetCheckedOutTo(sTasklistSSCPath)))
		{
			// Don't recheckout if already checked out
			bWantCheckout = FALSE;
		}
		else if (!bWantCheckout)
		{
			// convert old ssc to new system if required
			if (MatchesOurSourceControlID(tasks.GetCheckOutTo()))
			{
				bWantCheckout = TRUE;
			}
			else if (m_tdc.HasStyle(TDCS_CHECKOUTONLOAD))
			{
				bWantCheckout = MatchesOurSourceControlID(tasks.GetCheckOutTo());
			}
		}

		if (bWantCheckout)
		{
			CString sUnused;

			if (!CheckOutTasklist(sTasklistPath, tasks.GetXmlHeader(), GetSourceControlID(), sUnused, FALSE))
			{
				ASSERT(0);
				return FALSE;
			}
		}
		else
		{
			CString sCheckedOutTo = GetCheckedOutTo(sTasklistSSCPath);

			m_bTasklistCheckedOut = MatchesOurSourceControlID(sCheckedOutTo);
		}
	}
	else
	{
		m_bTasklistCheckedOut = FALSE;
	}

	return TRUE;
}

BOOL CTDCSourceControl::InitialiseSourceControlFolder(LPCTSTR szTasklistPath)
{
	CString sSSCFolder;
	
	if (!GetSourceControlFolder(szTasklistPath, sSSCFolder))
		return FALSE;
	
	if (!FileMisc::FolderExists(sSSCFolder))
	{
		if (!FileMisc::CreateFolder(sSSCFolder))
			return FALSE;

		VERIFY(FileMisc::ModifyAttributes(sSSCFolder, 0, FILE_ATTRIBUTE_HIDDEN));
	}

	return TRUE;
}

void CTDCSourceControl::PrepareTasksForSave(CTaskFile& tasks) const
{
	if (m_bTasklistSourceControlled)
		tasks.SetCheckedOutTo("");
	else
		tasks.RemoveFromSourceControl();
}

BOOL CTDCSourceControl::GetTasklistPath(CString& sTasklistPath) const
{
	sTasklistPath = m_tdc.GetFilePath();
	
	return !sTasklistPath.IsEmpty();
}

BOOL CTDCSourceControl::GetSourceControlFolder(CString& sSSCFolder) const
{
	return GetSourceControlFolder(m_tdc.GetFilePath(), sSSCFolder);
}

BOOL CTDCSourceControl::GetSourceControlFolder(LPCTSTR szTasklistPath, CString& sSSCFolder)
{
	if (Misc::IsEmpty(szTasklistPath))
		return FALSE;

	FileMisc::ReplaceExtension(sSSCFolder, _T(".ssc"));
	return TRUE;
}

BOOL CTDCSourceControl::GetTasklistSourceControlPath(CString& sTasklistSSCPath) const
{
	return GetSourceControlPath(m_tdc.GetFilePath(), sTasklistSSCPath);
}

BOOL CTDCSourceControl::GetSourceControlPath(LPCTSTR szTasklistPath, CString& sTasklistSSCPath)
{
	ASSERT(!Misc::IsEmpty(szTasklistPath));
	ASSERT(FileMisc::FileExists(szTasklistPath));

	CString sSSCFolder;
	
	if (!GetSourceControlFolder(szTasklistPath, sSSCFolder))
		return FALSE;

	CString sTasklistName = FileMisc::GetFileNameFromPath(szTasklistPath, FALSE);
	sTasklistSSCPath.Format(_T("%s\\%s.tsc"), sSSCFolder, sTasklistName);

	return TRUE;
}

BOOL CTDCSourceControl::GetTaskSourceControlPath(DWORD dwTaskID, CString& sTaskSSCPath) const
{
	if (!dwTaskID)
	{
		ASSERT(0);
		return FALSE;
	}

	CString sSSCFolder;
	
	if (!GetSourceControlFolder(sSSCFolder))
	{
		ASSERT(0);
		return FALSE;
	}

	sTaskSSCPath.Format(_T("%s\\%ld.tsc"), sSSCFolder, dwTaskID);
	return TRUE;
}

CString CTDCSourceControl::GetSourceControlID() const
{
	if (m_tdc.HasStyle(TDCS_INCLUDEUSERINCHECKOUT))
		return Misc::FormatComputerNameAndUser();

	// else
	return Misc::GetComputerName();
}

BOOL CTDCSourceControl::MatchesOurSourceControlID(const CString& sID) const
{
	if (sID.IsEmpty())
		return FALSE;

	if (SourceControlIDsMatch(sID, GetSourceControlID()))
		return TRUE;

	// Check if the naming option has been switched since our last check
	CString sReverseID;

	if (m_tdc.HasStyle(TDCS_INCLUDEUSERINCHECKOUT))
		sReverseID = Misc::GetComputerName();
	else 
		sReverseID = Misc::FormatComputerNameAndUser();

	return SourceControlIDsMatch(sID, sReverseID);
}

BOOL CTDCSourceControl::SourceControlIDsMatch(const CString& sID1, const CString& sID2)
{
	return (sID1.CompareNoCase(sID2) == 0);
}

BOOL CTDCSourceControl::HasCheckedOutTask(DWORD dwTaskID) const
{
	COleDateTime dtUnused;
	return m_mapTasksCheckedOut.Lookup(dwTaskID, dtUnused);
}

COleDateTime CTDCSourceControl::GetTaskLastSaveTime(DWORD dwTaskID) const
{
	COleDateTime dtCheckout;
	VERIFY(m_mapTasksCheckedOut.Lookup(dwTaskID, dtCheckout));

	return dtCheckout;
}

BOOL CTDCSourceControl::TaskModifiedSinceLastSave(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = m_tdc.GetTask(dwTaskID);

	if (!pTDI)
		return FALSE;

	COleDateTime dtLastSave;

	if (!m_mapTasksCheckedOut.Lookup(dwTaskID, dtLastSave))
	{
		ASSERT(0);
		return FALSE;
	}

	return (dtLastSave < pTDI->dateLastMod);
}

BOOL CTDCSourceControl::CheckOutTask(DWORD dwTaskID, CString& sTaskCheckedOutTo)
{
	sTaskCheckedOutTo.Empty();

	if (!dwTaskID || HasCheckedOutTask(dwTaskID))
	{
		ASSERT(0);
		return FALSE;
	}

	const TODOITEM* pTDI = m_tdc.GetTask(dwTaskID);

	if (!pTDI)
		return FALSE;

	CString sTaskSSCPath;
	
	if (!GetTaskSourceControlPath(dwTaskID, sTaskSSCPath))
	{
		ASSERT(0);
		return FALSE;
	}

	if (!CreateSentinelFile(sTaskSSCPath))
	{ 
		sTaskCheckedOutTo = GetCheckedOutTo(sTaskSSCPath);
		ASSERT(!sTaskCheckedOutTo.IsEmpty());

		return FALSE;
	}

	if (!SaveCheckedOutTask(sTaskSSCPath, dwTaskID, *pTDI, TRUE))
	{
		VERIFY(DeleteSentinelFile(sTaskSSCPath));
		return FALSE;
	}

	m_mapTasksCheckedOut[dwTaskID] = COleDateTime::GetCurrentTime();

	return TRUE;
}

BOOL CTDCSourceControl::SaveCheckedOutTask(DWORD dwTaskID, const TODOITEM& tdi) const
{
	ASSERT(HasCheckedOutTask(dwTaskID));

	CString sTaskSSCPath;
	VERIFY(GetTaskSourceControlPath(dwTaskID, sTaskSSCPath));

	if (!TaskModifiedSinceLastSave(dwTaskID))
		return TRUE;

	if (!SaveCheckedOutTask(sTaskSSCPath, dwTaskID, tdi, FALSE))
		return FALSE;

	m_mapTasksCheckedOut[dwTaskID] = COleDateTime::GetCurrentTime();
	return TRUE;
}

BOOL CTDCSourceControl::SaveCheckedOutTask(const CString& sTaskSSCPath, DWORD dwTaskID, 
											const TODOITEM& tdi, BOOL bPreCheckout) const
{
	ASSERT(dwTaskID);
	ASSERT(!tdi.bLocked || bPreCheckout);
	ASSERT(FileMisc::FileExists(sTaskSSCPath));
	ASSERT(HasCheckedOutTask(dwTaskID) || bPreCheckout);

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

	if (!task.Save(sTaskSSCPath, SFEF_UTF16))
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CTDCSourceControl::CheckInTask(DWORD dwTaskID, CString& sTasklistCheckedOutTo)
{
	if (!dwTaskID || !HasCheckedOutTask(dwTaskID))
	{
		ASSERT(0);
		return FALSE;
	}

	const TODOITEM* pTDI = m_tdc.GetTask(dwTaskID);

	if (!pTDI)
		return FALSE;

	CString sTaskSSCPath;
	
	if (!GetTaskSourceControlPath(dwTaskID, sTaskSSCPath) || 
		!FileMisc::FileExists(sTaskSSCPath))
	{
		ASSERT(0);
		return FALSE;
	}

	if (!m_bTasklistCheckedOut && TaskModifiedSinceLastSave(dwTaskID))
	{
		// Must be able to checkout tasklist
		CString sTasklistSSCPath;
		
		if (!GetTasklistSourceControlPath(sTasklistSSCPath))
		{
			ASSERT(0);
			return FALSE;
		}

		if (!CheckOutTasklist(sTasklistCheckedOutTo))
		{
			// Not unexpected
			return FALSE;
		}

		CString sTasklistPath(m_tdc.GetFilePath());
		CTaskFile tasks;

		if (!tasks.Load(sTasklistPath))
		{
			ASSERT(0);
			return FALSE;
		}

		HTASKITEM hTask = tasks.FindTask(dwTaskID);

		if (!hTask || !tasks.SetTaskAttributes(hTask, *pTDI))
		{
			ASSERT(0);
			return FALSE;
		}

		if (!tasks.Save(sTasklistPath, SFEF_UTF16))
		{
			ASSERT(0);
			return FALSE;
		}

		VERIFY(CheckInTasklist());
	}
	
	VERIFY(DeleteSentinelFile(sTaskSSCPath));
	m_mapTasksCheckedOut.RemoveKey(dwTaskID);

	return TRUE;
}

BOOL CTDCSourceControl::CheckInAllTasks(CString& sTasklistCheckedOutTo)
{
	// TODO 
	return FALSE;
}

BOOL CTDCSourceControl::LoadCheckedOutTask(DWORD dwTaskID, TODOITEM& tdi) const
{
	CString sTaskSSCPath;
	
	if (!GetTaskSourceControlPath(dwTaskID, sTaskSSCPath))
	{
		ASSERT(0);
		return FALSE;
	}

	return (LoadCheckedOutTask(sTaskSSCPath, tdi) == dwTaskID);
}

DWORD CTDCSourceControl::LoadCheckedOutTask(LPCTSTR szTaskPath, TODOITEM& tdi) const
{
	DWORD dwTaskID = GetTaskIDFromFilePath(szTaskPath);

	if (!dwTaskID)
	{
		ASSERT(0);
		return 0;
	}

	CTaskFile task;

	if (!task.Load(szTaskPath))
	{
		ASSERT(0);
		return 0;
	}

	if (!task.IsCheckedOutTo(GetSourceControlID())) 
	{
		ASSERT(0);
		return 0;
	}

	if (!task.GetTaskAttributes(task.GetFirstTask(), tdi))
	{
		ASSERT(0);
		return 0;
	}

	tdi.bLocked = FALSE;

	return dwTaskID;
}

BOOL CTDCSourceControl::CheckOutTasklist()
{
	return CheckOutTasklist(CString());
}

BOOL CTDCSourceControl::CheckOutTasklist(CString& sTasklistCheckedOutTo, BOOL bForce)
{
	if (!m_bTasklistSourceControlled || m_bTasklistCheckedOut)
	{
		ASSERT(0);
		return FALSE;
	}

	if (m_tdc.HasFilePath())
	{
		if (!CheckOutTasklist(m_tdc.GetFilePath(), 
								m_tdc.m_sXmlHeader, 
								GetSourceControlID(), 
								sTasklistCheckedOutTo, 
								bForce))
		{
			ASSERT(0);
			return FALSE;
		}
	}
	
	m_bTasklistCheckedOut = TRUE;
	return TRUE;
}

BOOL CTDCSourceControl::CheckOutTasklist(LPCTSTR szTasklistPath, LPCTSTR szXmlHeader, 
										LPCTSTR szSourceControlID, CString& sTasklistCheckedOutTo, 
										BOOL bForce)
{
	CString sTasklistSSCPath;
	
	if (!GetSourceControlPath(szTasklistPath, sTasklistSSCPath))
	{
		ASSERT(0);
		return FALSE;
	}

	if (bForce)
		DeleteSentinelFile(sTasklistSSCPath);

	if (!CreateSentinelFile(sTasklistSSCPath))
	{ 
		sTasklistCheckedOutTo = GetCheckedOutTo(sTasklistSSCPath);
		ASSERT(!sTasklistCheckedOutTo.IsEmpty());

		return FALSE;
	}

	// Save minimal tasklist
	CTaskFile tasklist;

	tasklist.SetCheckedOutTo(szSourceControlID);
	tasklist.SetXmlHeader(szXmlHeader);

	if (!tasklist.Save(sTasklistSSCPath, SFEF_UTF16))
	{
		VERIFY(DeleteSentinelFile(sTasklistSSCPath));
		return FALSE;
	}

	return TRUE;
}

BOOL CTDCSourceControl::CheckInTasklist()
{
	if (!m_bTasklistSourceControlled || !m_bTasklistCheckedOut)
	{
		ASSERT(0);
		return FALSE;
	}

	if (m_tdc.HasFilePath())
	{
		if (!IsTasklistCheckedOut())
		{
			ASSERT(0);
			return FALSE;
		}

		CString sTasklistSSCPath;

		if (!GetTasklistSourceControlPath(sTasklistSSCPath))
		{
			ASSERT(0);
			return FALSE;
		}

		if (!FileMisc::DeleteFile(sTasklistSSCPath, TRUE))
			return FALSE;
	}

	// else
	m_bTasklistCheckedOut = FALSE;
	return TRUE;
}

CString CTDCSourceControl::GetCheckedOutTo(LPCTSTR szPath)
{
	CString sCheckedOutTo;
	CTaskFile tasklist;

	if (tasklist.LoadHeader(szPath))
		sCheckedOutTo = tasklist.GetCheckOutTo();

	return sCheckedOutTo;
}

BOOL CTDCSourceControl::IsSourceControlled(LPCTSTR szPath)
{
	CTaskFile tasklist;

	if (tasklist.LoadHeader(szPath))
		return tasklist.IsSourceControlled();

	return FALSE;
}

BOOL CTDCSourceControl::IsTasklistCheckedOut() const
{
	return (m_bTasklistSourceControlled && m_bTasklistCheckedOut);
}

BOOL CTDCSourceControl::IsTasklistCheckedOut(CString& sTasklistCheckedOutTo) const
{
	if (IsTasklistCheckedOut())
	{
		sTasklistCheckedOutTo = GetSourceControlID();
		return TRUE;
	}

	CString sTasklistSSCPath;
	
	if (!GetTasklistSourceControlPath(sTasklistSSCPath))
		return FALSE;

	if (!FileMisc::FileExists(sTasklistSSCPath))
		return FALSE;

	sTasklistCheckedOutTo = GetCheckedOutTo(sTasklistSSCPath);
	return !sTasklistCheckedOutTo.IsEmpty();
}

BOOL CTDCSourceControl::IsTaskCheckedOut(DWORD dwTaskID) const
{
	CString sTaskSSCPath;

	if (!GetTaskSourceControlPath(dwTaskID, sTaskSSCPath))
	{
		ASSERT(0);
		return FALSE;
	}

	return FileMisc::FileExists(sTaskSSCPath);
}

BOOL CTDCSourceControl::IsTaskCheckedOut(DWORD dwTaskID, CString& sTaskCheckedOutTo) const
{
	CString sTaskSSCPath;
	
	if (!GetTaskSourceControlPath(dwTaskID, sTaskSSCPath))
	{
		ASSERT(0);
		return FALSE;
	}

	if (!FileMisc::FileExists(sTaskSSCPath))
		return FALSE;

	sTaskCheckedOutTo = GetCheckedOutTo(sTaskSSCPath);
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
	HANDLE hFile = CreateFile(szPath,					// name of the write
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
	ASSERT(m_bTasklistSourceControlled);
	
	CString sSSCFolder, sTasklistSSCPath;

	if (!GetSourceControlFolder(sSSCFolder) || 
		!GetTasklistSourceControlPath(sTasklistSSCPath))
	{
		ASSERT(0);
		return FALSE;
	}

	CStringArray aTaskFiles;
	int nFile = FileMisc::FindFiles(sSSCFolder, aTaskFiles, FALSE, _T("*.tsc"));

	TODOITEM tdi;
	tdItems.DeleteAll();
	m_mapTasksCheckedOut.RemoveAll();

	while (nFile--)
	{
		const CString sTaskSSCPath = aTaskFiles[nFile];

		// Ignore the tasklist sentinel file which will also appear
		if (sTasklistSSCPath.CompareNoCase(sTaskSSCPath) == 0)
			continue;

		DWORD dwTaskID = LoadCheckedOutTask(sTaskSSCPath, tdi);

		if (dwTaskID)
		{
			// Discard deleted tasks
			if (m_tdc.GetTask(dwTaskID) == NULL)
			{
				VERIFY(DeleteSentinelFile(sTaskSSCPath));
				continue;
			}

			TODOITEM* pTDI = new TODOITEM(tdi);
			pTDI->bLocked = FALSE;

			tdItems.AddTask(dwTaskID, pTDI);
			m_mapTasksCheckedOut[dwTaskID] = COleDateTime::GetCurrentTime();
		}
	}

	return tdItems.GetCount();
}

BOOL CTDCSourceControl::HasCheckedOutTasks() const
{
	return (m_bTasklistSourceControlled && (m_mapTasksCheckedOut.GetCount() != 0));
}

BOOL CTDCSourceControl::SaveCheckedOutTasks()
{
	DiscardDeletedCheckedOutTasks();

	POSITION pos = m_mapTasksCheckedOut.GetStartPosition();
	BOOL bSomeFailed = FALSE;

	while (pos)
	{
		COleDateTime dtLastSave;
		DWORD dwTaskID = 0;

		m_mapTasksCheckedOut.GetNextAssoc(pos, dwTaskID, dtLastSave);
		
		const TODOITEM* pTDI = m_tdc.GetTask(dwTaskID);
		ASSERT(pTDI);

		if (pTDI == NULL)
		{
			if (!SaveCheckedOutTask(dwTaskID, *pTDI))
				bSomeFailed = TRUE;
		}
	}

	return !bSomeFailed;
}

int CTDCSourceControl::DiscardDeletedCheckedOutTasks()
{
	int nNumDiscarded = 0;
	POSITION pos = m_mapTasksCheckedOut.GetStartPosition();

	while (pos)
	{
		COleDateTime dtLastSave;
		DWORD dwTaskID = 0;

		m_mapTasksCheckedOut.GetNextAssoc(pos, dwTaskID, dtLastSave);

		if (m_tdc.GetTask(dwTaskID) == NULL)
		{
			CString sTaskSSCPath;
			VERIFY(GetTaskSourceControlPath(dwTaskID, sTaskSSCPath));

			VERIFY(DeleteSentinelFile(sTaskSSCPath));
			m_mapTasksCheckedOut.RemoveKey(dwTaskID);

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

BOOL CTDCSourceControl::IsSourceControlled() const 
{ 
	return m_bTasklistSourceControlled; 
}

BOOL CTDCSourceControl::CanAddTasklistToSourceControl(BOOL bAdd) const
{
	if (m_tdc.m_bArchive)
		return FALSE;

	if ((bAdd && m_bTasklistSourceControlled) || (!bAdd && !m_bTasklistSourceControlled))
		return FALSE;

	if (!bAdd && !m_bTasklistCheckedOut)
		return FALSE;

	return TRUE;
}

BOOL CTDCSourceControl::AddTasklistToSourceControl(BOOL bAdd)
{
	// Sanity checks
	if (m_tdc.m_bArchive)
		return FALSE;

	if ((bAdd && m_bTasklistSourceControlled) || (!bAdd && !m_bTasklistSourceControlled))
		return TRUE;

	if (!bAdd && !m_bTasklistCheckedOut)
		return FALSE;

	BOOL bHasFilePath = m_tdc.HasFilePath();
	CTaskFile file;

	if (bHasFilePath)
		m_tdc.BuildTasksForSave(file);

	if (bAdd)
	{
		if (bHasFilePath)
		{
			file.SetCheckedOutTo("");

			if (!file.Save(m_tdc.GetFilePath(), SFEF_UTF16))
			{
				ASSERT(0);
				return FALSE;
			}
		}

		m_bTasklistSourceControlled = TRUE;

		if (m_tdc.HasStyle(TDCS_CHECKOUTONLOAD))
		{
			if (bHasFilePath)
				VERIFY(CheckOutTasklist());
			else
				m_bTasklistCheckedOut = TRUE;
		}
	}
	else
	{
		if (bHasFilePath)
		{
			if (!CheckInTasklist())
			{
				ASSERT(0);
				return FALSE;
			}
	
			file.RemoveFromSourceControl();

			if (!file.Save(m_tdc.GetFilePath(), SFEF_UTF16))
			{
				ASSERT(0);
				return FALSE;
			}
		}

		m_bTasklistSourceControlled = FALSE;
	}

	return TRUE;
}
