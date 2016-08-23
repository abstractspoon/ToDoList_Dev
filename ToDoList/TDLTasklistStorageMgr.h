// TDLTasklistStorageMgr.h: interface for the CTDLTasklistStorageMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDLTASKLISTSTORAGEMGR_H__3FD3D340_A2C3_415F_BBFF_A407E3A06715__INCLUDED_)
#define AFX_TDLTASKLISTSTORAGEMGR_H__3FD3D340_A2C3_415F_BBFF_A407E3A06715__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "taskfile.h"

#include "..\shared\TaskListStorageMgr.h"

class CTDLTasklistStorageMgr : public CTasklistStorageMgr  
{
public:
	CTDLTasklistStorageMgr();
	virtual ~CTDLTasklistStorageMgr();

	BOOL RetrieveTasklist(TSM_TASKLISTINFO& storageInfo, CTaskFile& tasks, int nByStorage, IPreferences* pPrefs, BOOL bSilent = FALSE);
	BOOL StoreTasklist(TSM_TASKLISTINFO& storageInfo, const CTaskFile& tasks, int nByStorage, IPreferences* pPrefs, BOOL bSilent = FALSE);

protected:
	virtual void Initialize() const;

};

#endif // !defined(AFX_TDLTASKLISTSTORAGEMGR_H__3FD3D340_A2C3_415F_BBFF_A407E3A06715__INCLUDED_)
