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

CTDCSourceControlHelper::CTDCSourceControlHelper(const CToDoCtrl& tdc) : m_tdc(tdc)
{
}

CTDCSourceControlHelper::~CTDCSourceControlHelper()
{
}

CString CTDCSourceControlHelper::GetTasklistPath() const
{
	ASSERT(!m_tdc.GetFilePath().IsEmpty());

	return m_tdc.GetFilePath();
}

CString CTDCSourceControlHelper::GetTaskSourceControlFolder() const
{
	CString sPath(GetTasklistPath());
	FileMisc::ReplaceExtension(_T(".ssc"));

	return sPath;
}

CString CTDCSourceControlHelper::GetTasklistSourceControlPath() const
{
	CString sPath(GetTasklistPath());
	FileMisc::ReplaceExtension(sPath, _T(".tsc"));

	return sPath;
}

CString CTDCSourceControlHelper::GetTaskSourceControlPath(DWORD dwTaskID) const
{
	if (!dwTaskID)
	{
		ASSERT(0);
		return FALSE;
	}

	CString sFolder(GetTaskSourceControlFolder());

	if (sFolder.IsEmpty())
		return sFolder;

	CString sPath;
	sPath.Format(_T("%s\\%ld.tsc"), sFolder, dwTaskID);
	return sPath;
}

CString CTDCSourceControlHelper::GetSourceControlID(BOOL bAlternate) const
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

BOOL CTDCSourceControlHelper::MatchesOurSourceControlID(const CString& sID) const
{
	if (sID.IsEmpty())
		return FALSE;

	return (SourceControlIDsMatch(sID, GetSourceControlID(FALSE)) ||
			SourceControlIDsMatch(sID, GetSourceControlID(TRUE)));
}

BOOL CTDCSourceControlHelper::SourceControlIDsMatch(const CString& sID1, const CString& sID2)
{
	return (sID1.CompareNoCase(sID2) == 0);
}

BOOL CTDCSourceControlHelper::CheckOutTask(DWORD dwTaskID, CString& sCheckedOutTo) const
{
	const TODOITEM* pTDI = m_tdc.GetTask(dwTaskID);

	if (!pTDI || pTDI->IsReference())
	{
		ASSERT(0);
		return FALSE;
	}

	CString sTaskPath = GetTaskSourceControlPath(dwTaskID); 

	if (!CreateSentinelFile(sTaskPath))
	{ 
		sCheckedOutTo = GetCheckedOutTo(sTaskPath);
		ASSERT(!sCheckedOutTo.IsEmpty());

		return FALSE;
	}

	// Save minimal tasklist
	CTaskFile task;
	HTASKITEM hTask = task.NewTask(pTDI->sTitle, NULL, dwTaskID, 0);

	if (!hTask)
	{
		ASSERT(0);
		return FALSE;
	}

	task.SetTaskAttributes(hTask, pTDI);
	task.SetCheckedOutTo(GetSourceControlID());
	task.SetXmlHeader(m_tdc.m_sXmlHeader);
	task.SetFileFormat(FILEFORMAT_CURRENT);

	if (!task.Save(sTaskPath, SFEF_UTF16))
	{
		VERIFY(DeleteSentinelFile(sTaskPath));
		return FALSE;
	}

	return TRUE;
}

BOOL CTDCSourceControlHelper::CheckInTask(DWORD dwTaskID, TODOITEM& tdi) const
{
	if (!dwTaskID)
	{
		ASSERT(0);
		return FALSE;
	}

	CString sTaskPath = GetTaskSourceControlPath(dwTaskID); 

	if (!FileMisc::FileExists(sTaskPath))
	{
		ASSERT(0);
		return FALSE;
	}

	if (!LoadCheckedOutTask(sTaskPath, dwTaskID, tdi))
	{
		ASSERT(0);
		return FALSE;
	}

	return FileMisc::DeleteFile(sTaskPath);
}

BOOL CTDCSourceControlHelper::LoadCheckedOutTask(DWORD dwTaskID, TODOITEM& tdi) const
{
	CString sTaskPath = GetTaskSourceControlPath(dwTaskID);

	return (LoadCheckedOutTask(sTaskPath, dwTaskID, tdi) == dwTaskID);
}

DWORD CTDCSourceControlHelper::LoadCheckedOutTask(LPCTSTR szTaskPath, TODOITEM& tdi) const
{
	DWORD dwTaskID = _ttol(FileMisc::GetFileNameFromPath(szTaskPath));

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

BOOL CTDCSourceControlHelper::CheckOutTasklist(CString& sCheckedOutTo) const
{
	CString sTasklistPath = GetTasklistSourceControlPath(); 

	if (!CreateSentinelFile(sTasklistPath))
	{ 
		sCheckedOutTo = GetCheckedOutTo(sTasklistPath);
		ASSERT(!sCheckedOutTo.IsEmpty());

		return FALSE;
	}

	// Save minimal tasklist
	CTaskFile tasklist;

	tasklist.SetCheckedOutTo(GetSourceControlID());
	tasklist.SetXmlHeader(m_tdc.m_sXmlHeader);
	tasklist.SetFileFormat(FILEFORMAT_CURRENT);

	if (!tasklist.Save(sTasklistPath, SFEF_UTF16))
	{
		VERIFY(DeleteSentinelFile(sTasklistPath));
		return FALSE;
	}

	return TRUE;
}

BOOL CTDCSourceControlHelper::CheckInTasklist() const
{
	// TODO
}

CString CTDCSourceControlHelper::GetCheckedOutTo(LPCTSTR szPath)
{
	CTaskFile tasklist;

	if (!tasklist.LoadHeader(szPath))
		return FALSE;

	return tasklist.GetCheckOutTo();
}

BOOL CTDCSourceControlHelper::IsTasklistCheckedOut() const
{
	CString sUnused;

	return IsTasklistCheckedOut(sUnused);
}

BOOL CTDCSourceControlHelper::IsTasklistCheckedOut(CString& sCheckedOutTo) const
{
	CString sCheckedOutTo = GetCheckedOutTo(GetTasklistSourceControlPath());

	return !sCheckedOutTo.IsEmpty();
}

BOOL CTDCSourceControlHelper::IsTasklistCheckedOutToUs()
{
	CString sCheckedOutTo;

	if (!IsTasklistCheckedOut(sCheckedOutTo))
		return FALSE;

	return MatchesOurSourceControlID(sCheckedOutTo);
}

BOOL CTDCSourceControlHelper::IsTaskCheckedOut(DWORD dwTaskID)
{
	CString sCheckedOutTo = GetCheckedOutTo(GetTaskSourceControlPath(dwTaskID));

	return !sCheckedOutTo.IsEmpty();
}

BOOL CTDCSourceControlHelper::IsTaskCheckedOut(DWORD dwTaskID, CString& sCheckedOutTo)
{
	CString sUnused;

	return IsTaskCheckedOut(dwTaskID, sUnused);
}

BOOL CTDCSourceControlHelper::IsTaskCheckedOutToUs(DWORD dwTaskID)
{
	CString sCheckedOutTo;

	if (!IsTaskCheckedOut(dwTaskID, sCheckedOutTo))
		return FALSE;

	return MatchesOurSourceControlID(sCheckedOutTo);
}

BOOL CTDCSourceControlHelper::CreateSentinelFile(LPCTSTR szPath)
{
	HFILE hFile = CreateFile(szPath,				// name of the write
							GENERIC_WRITE,			// open for writing
							0,						// do not share
							NULL,					// default security
							CREATE_NEW,				// create new file only
							FILE_ATTRIBUTE_NORMAL,	// normal file
							NULL);					// no attr. template

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		// ERROR_FILE_EXISTS
		return FALSE;
	}

	::CloseHandle(hFile);
	return TRUE;
}

BOOL CTDCSourceControlHelper::DeleteSentinelFile(LPCTSTR szPath)
{
	return FileMisc::DeleteFile(szPath, TRUE);
}
