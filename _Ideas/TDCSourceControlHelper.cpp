// TDCSourceControlHelper.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TDCSourceControlHelper.h"

#include "..\shared\Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CTDCSourceControlHelper

CString CTDCSourceControlHelper::GetTaskSourceControlFolder(LPCTSTR szTasklistPath) const
{
	if (Misc::IsEmpty(szTasklistPath))
	{
		ASSERT(0);
		return _T("");
	}

	CString sPath(szTasklistPath);
	FileMisc::ReplaceExtension(_T(".ssc"));

	return sPath;
}

CString CTDCSourceControlHelper::GetTasklistSourceControlPath(LPCTSTR szTasklistPath) const
{
	if (Misc::IsEmpty(szTasklistPath))
	{
		ASSERT(0);
		return _T("");
	}

	CString sPath(szTasklistPath);
	FileMisc::ReplaceExtension(sPath, _T(".tsc"));

	return sPath;
}

CString CTDCSourceControlHelper::GetTaskSourceControlPath(LPCTSTR szTasklistPath, DWORD dwTaskID) const
{
	if (!dwTaskID)
	{
		ASSERT(0);
		return FALSE;
	}

	CString sFolder(GetTaskSourceControlFolder(szTasklistPath));

	if (sFolder.IsEmpty())
		return sFolder;

	CString sPath;
	sPath.Format(_T("%s\\%ld.tsc"), sFolder, dwTaskID);
	return sPath;
}

CString CTDCSourceControlHelper::GetSourceControlID(BOOL bIncludeUser, BOOL bAlternate)
{
	if (bIncludeUser)
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

BOOL CTDCSourceControlHelper::MatchesSourceControlID(const CString& sID)
{
	if (sID.IsEmpty())
		return FALSE;

	return ((GetSourceControlID(FALSE) == sID) ||
			(GetSourceControlID(TRUE) == sID));
}

BOOL CTDCSourceControlHelper::CheckOutTask(LPCTSTR szTasklistPath, DWORD dwTaskID, const TODOITEM& tdi, 
											LPCTSTR szXmlHeader, BOOL bIncludeUserInID)
{
	if (!dwTaskID)
	{
		ASSERT(0);
		return FALSE;
	}

	CString sTaskPath = GetTaskSourceControlPath(szTasklistPath, dwTaskID); 

	HFILE hFile = CreateFile(sTaskPath,				// name of the write
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

	// Save minimal tasklist
	CTaskFile task;
	HTASKITEM hTask = task.NewTask(tdi.sTitle, NULL, dwTaskID, 0);

	if (!hTask)
	{
		ASSERT(0);
		return FALSE;
	}

	task.SetTaskAttributes(hTask, tdi);
	task.SetCheckedOutTo(GetSourceControlID(bIncludeUserInID));
	task.SetXmlHeader(szXmlHeader);
	task.SetFileFormat(FILEFORMAT_CURRENT);

	if (!task.Save(sTaskPath, SFEF_UTF16))
		return FALSE;

	return TRUE;
}

BOOL CTDCSourceControlHelper::CheckInTask(LPCTSTR szTasklistPath, DWORD dwTaskID, TODOITEM& tdi)
{
	if (!dwTaskID)
	{
		ASSERT(0);
		return FALSE;
	}

	CString sTaskPath = GetTaskSourceControlPath(szTasklistPath, dwTaskID); 

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

BOOL CTDCSourceControlHelper::LoadCheckedOutTask(LPCTSTR szTasklistPath, DWORD dwTaskID, TODOITEM& tdi) const
{
	CString sTaskPath = GetTaskSourceControlPath(szTasklistPath, dwTaskID);

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

