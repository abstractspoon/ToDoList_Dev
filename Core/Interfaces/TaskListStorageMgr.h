// TasklistStorageMgr.h: interface for the CTasklistStorageMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTSTORAGEMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_)
#define AFX_TASKLISTSTORAGEMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\ITasklistStorage.h"

#include <afxtempl.h>

struct TSM_TASKLISTINFO : ITS_TASKLISTINFO
{
	TSM_TASKLISTINFO();
	TSM_TASKLISTINFO(const TSM_TASKLISTINFO& info);

	const TSM_TASKLISTINFO& operator=(const TSM_TASKLISTINFO& info);

	void Reset();
	BOOL HasInfo() const;

	BOOL HasLocalFilePath() const;
	void SetLocalFilePath(LPCTSTR szFilePath);
	void ClearLocalFilePath();

	CString EncodeInfo(BOOL bIncPassword = TRUE) const;
	BOOL DecodeInfo(const CString& sInfo, BOOL bIncPassword = TRUE);

	CString sStorageID;

protected:
	static CString Decode(const CString& sData);
	static CString Encode(const CString& sData);
};

class CTasklistStorageMgr  
{
public:
	CTasklistStorageMgr();
	virtual ~CTasklistStorageMgr();
	
	virtual void Release();
	void UpdateLocalizer();
	
	int GetNumStorage() const;
	CString GetStorageMenuText(int nStorage) const;
	CString GetStorageTypeID(int nStorage) const;
	HICON GetStorageIcon(int nStorage) const;
	
	BOOL RetrieveTasklist(TSM_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, int nByStorage, IPreferences* pPrefs, BOOL bSilent = FALSE);
	BOOL StoreTasklist(TSM_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, int nByStorage, IPreferences* pPrefs, BOOL bSilent = FALSE);

	int FindStorage(LPCTSTR szTypeID) const;
	
protected:
	BOOL m_bInitialized;
	CArray<ITasklistStorage*, ITasklistStorage*> m_aStorage;
	
protected:
	virtual void Initialize() const;

};

#endif // !defined(AFX_TasklistStorageMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_)
