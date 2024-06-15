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

/*
class CBaseCloudStorage : public ITasklistStorage  
{
public:
	CBaseCloudStorage() : m_hIcon(NULL) {}
	virtual ~CBaseCloudStorage() {}
	
	// interface implementation
    virtual void Release() { delete this; }
	void SetLocalizer(ITransText* pTT) { CLocalizer::Initialize(pTT); }

	virtual BOOL IsInstalled(CString& sUserFolder) const = 0;
	virtual bool RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent);
	virtual bool StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent);

protected:
	CIcon m_icon;

protected:
	CString FormatDisplayName(const CString& sFilePath);
	void UpdateTaskListInfo(ITS_TASKLISTINFO* pFInfo, const CString& sFilePath, BOOL bStoring);
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CBaseCloudStorage::RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* / *pDestTaskFile* /, IPreferences* pPrefs, LPCTSTR / *szKey* /, bool bSilent)
{
	CString sUserFolder;
	
	if (IsInstalled(sUserFolder))
	{
		CString sSrcPath = pFInfo->szTasklistID;
		CString sDestPath = pFInfo->szLocalFileName;
		
		if (bSilent && sSrcPath.IsEmpty())
			return false;
		
		if (sSrcPath.IsEmpty())
		{
			CFileOpenDialog	dialog(GetMenuText(), 
									_T("tdl"), 
									NULL, 
									EOFN_DEFAULTOPEN, 
									CEnString(IDS_TDLFILEFILTER));

			dialog.m_ofn.lpstrInitialDir = sUserFolder;
			
			if (dialog.DoModal(pPrefs) != IDOK)
				return false;
			
			// else
			sSrcPath = dialog.GetPathName();
		}
		
		if (sDestPath.IsEmpty())
			sDestPath = sSrcPath;
		
		if (FileMisc::IsSamePath(sSrcPath, sDestPath) || ::CopyFile(sSrcPath, sDestPath, FALSE))
		{
			// return information to caller
			UpdateTaskListInfo(pFInfo, sDestPath, FALSE);
			return true;
		}
	}
	
	return false;
}

bool CBaseCloudStorage::StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* / *pSrcTaskFile* /, IPreferences* pPrefs, LPCTSTR / *szKey* /, bool bSilent)
{
	CString sUserFolder;
	
	if (IsInstalled(sUserFolder))
	{
		CString sSrcPath = pFInfo->szLocalFileName;
		CString sDestPath = pFInfo->szTasklistID;
		
		if (bSilent && sDestPath.IsEmpty())
			return false;
		
		if (sDestPath.IsEmpty())
		{
			CFileSaveDialog	dialog(GetMenuText(), 
									_T("tdl"), 
									NULL,//FileMisc::GetFileNameFromPath(sDestPath.IsEmpty() ? sSrcPath : sDestPath), 
									EOFN_DEFAULTSAVE, 
									_T("Tasklists (*.tdl)|*.tdl"));

			dialog.m_ofn.lpstrInitialDir = sUserFolder;
			
			if (dialog.DoModal(pPrefs) != IDOK)
				return false;
			
			// else 
			sDestPath = dialog.GetPathName();
		}
		
		if (FileMisc::IsSamePath(sSrcPath, sDestPath) || ::CopyFile(sSrcPath, sDestPath, FALSE))
		{
			// return information to caller
			UpdateTaskListInfo(pFInfo, sDestPath, TRUE);
			return true;
		}
	}
	
	return false;
}

CString CBaseCloudStorage::FormatDisplayName(const CString& sFilePath)
{
	CString sDisplay;
	sDisplay.Format(_T("%s: %s"), GetMenuText(), FileMisc::GetFileNameFromPath(sFilePath));
	
	return sDisplay;
}

void CBaseCloudStorage::UpdateTaskListInfo(ITS_TASKLISTINFO* pFInfo,  
											const CString& sFilePath, 
											BOOL bStoring)
{
	ASSERT(FileMisc::FileExists(sFilePath));

	if (bStoring)
		ASSERT(FileMisc::FileExists(pFInfo->szLocalFileName));
	else
		lstrcpyn(pFInfo->szLocalFileName, sFilePath, _MAX_PATH);
	
	lstrcpyn(pFInfo->szDisplayName, FormatDisplayName(sFilePath), _MAX_PATH);
	lstrcpyn(pFInfo->szTasklistID, sFilePath, ITS_TASKLISTID_LEN);
}

//////////////////////////////////////////////////////////////////////
// Private File based storage providers

class CGoogleDriveStorage : public CBaseCloudStorage  
{
public:
	CGoogleDriveStorage();
	virtual ~CGoogleDriveStorage();

	static BOOL IsGoogleDriveInstalled();
	
	// caller must copy result only
	LPCTSTR GetMenuText() const { return _T("Google Drive"); }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return _T("3ADF91FF-CAB9-44FF-BAA6-F397172149AF"); }
	
	bool RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent);
	bool StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent);
	
protected:
	BOOL IsInstalled(CString& sUserFolder) const;
};

class CSkyDriveStorage : public CBaseCloudStorage  
{
public:
	CSkyDriveStorage();
	virtual ~CSkyDriveStorage();
	
	static BOOL IsSkyDriveInstalled();
	
	// caller must copy result only
	LPCTSTR GetMenuText() const { return _T("SkyDrive"); }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return _T("0BE9A394-86EC-412C-A88A-496A737E6110"); }
	
	bool RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent);
	bool StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent);
	
protected:
	BOOL IsInstalled(CString& sUserFolder) const;
};

class CDropBoxStorage : public CBaseCloudStorage  
{
public:
	CDropBoxStorage();
	virtual ~CDropBoxStorage();
	
	static BOOL IsDropBoxInstalled();
	
	// caller must copy result only
	LPCTSTR GetMenuText() const { return _T("DropBox"); }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return _T("9F553544-85AA-4EDD-B0FD-71486246C3E2"); }
	
	bool RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent);
	bool StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent);
	
protected:
	BOOL IsInstalled(CString& sUserFolder) const;
};
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDLTasklistStorageMgr::CTDLTasklistStorageMgr()
{
}

CTDLTasklistStorageMgr::~CTDLTasklistStorageMgr()
{

}

BOOL CTDLTasklistStorageMgr::RetrieveTasklist(TSM_TASKLISTINFO& storageInfo, CTaskFile& tasks, int nByStorage, IPreferences* pPrefs, BOOL bSilent)
{
	if (CTasklistStorageMgr::RetrieveTasklist(&storageInfo, &tasks, nByStorage, pPrefs, bSilent))
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

BOOL CTDLTasklistStorageMgr::StoreTasklist(TSM_TASKLISTINFO& storageInfo, const CTaskFile& tasks, int nByStorage, IPreferences* pPrefs, BOOL bSilent)
{
	// snapshot local file in case it gets modified
	CString sLocalFile = storageInfo.szLocalFileName;

	if (!FileMisc::IsTempFilePath(sLocalFile))
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bSuccess = CTasklistStorageMgr::StoreTasklist(&storageInfo, &tasks, nByStorage, pPrefs, bSilent);

	// cleanup
	FileMisc::DeleteFile(sLocalFile, TRUE);
	storageInfo.ClearLocalFilePath();

	return bSuccess;
}
