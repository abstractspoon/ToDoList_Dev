// TDLTasklistStorageMgr.cpp: implementation of the CTDLTasklistStorageMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDLTasklistStorageMgr.h"

#include "..\shared\Regkey.h"
#include "..\shared\FileMisc.h"
#include "..\shared\EnFileDialog.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\graphicsmisc.h"

#include <Shlobj.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDLTasklistStorageMgr::CTDLTasklistStorageMgr()
{
}

CTDLTasklistStorageMgr::~CTDLTasklistStorageMgr()
{

}

BOOL CTDLTasklistStorageMgr::RetrieveTasklist(TSM_TASKLISTINFO& storageInfo, CTaskFile& tasks, int nByStorage, IPreferences* pPrefs, BOOL bPrompt)
{
	if (CTasklistStorageMgr::RetrieveTasklist(&storageInfo, &tasks, nByStorage, pPrefs, bPrompt))
	{
		CString sTempPath = FileMisc::GetTempFilePath();

		// save tasks to a temp file if no temp file returned
		if (!FileMisc::FileExists(storageInfo.szLocalFileName))
		{
			VERIFY(tasks.Save(sTempPath, SFEF_UTF16));
		}
		else if (!FileMisc::IsTempFilePath(storageInfo.szLocalFileName)) // copy to temp path
		{
			VERIFY(FileMisc::CopyFile(storageInfo.szLocalFileName, sTempPath, TRUE, TRUE));
		}
		else
		{
			sTempPath = storageInfo.szLocalFileName;
		}

		storageInfo.SetLocalFilePath(sTempPath);

		return TRUE;
	}

	return FALSE;
}

BOOL CTDLTasklistStorageMgr::StoreTasklist(TSM_TASKLISTINFO& storageInfo, const CTaskFile& tasks, int nByStorage, IPreferences* pPrefs, BOOL bPrompt)
{
	// snapshot local file in case it gets modified
	CString sLocalFile = storageInfo.szLocalFileName;

	if (!FileMisc::IsTempFilePath(sLocalFile))
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bSuccess = CTasklistStorageMgr::StoreTasklist(&storageInfo, &tasks, nByStorage, pPrefs, bPrompt);

	// cleanup
	FileMisc::DeleteFile(sLocalFile, TRUE);
	storageInfo.ClearLocalFilePath();

	return bSuccess;
}
