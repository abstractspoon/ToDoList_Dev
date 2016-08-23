// OdbcTasklistStorage.h: interface for the COdbcTasklistStorage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCTASKLISTSTORAGE_H__1A49027C_CC71_4EB8_8540_B319AB9A045E__INCLUDED_)
#define AFX_ODBCTASKLISTSTORAGE_H__1A49027C_CC71_4EB8_8540_B319AB9A045E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OdbcStruct.h"

#include "..\Interfaces\ITaskList.h"
#include "..\Interfaces\ITaskListStorage.h"

class ITransText;
class COdbcTaskWriter;

class COdbcTasklistStorage : public ITasklistStorage  
{
public:
	COdbcTasklistStorage();
	virtual ~COdbcTasklistStorage();

	// interface implementation
    void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);

	// caller must copy result only
	LPCTSTR GetMenuText() const { return _T("ODBC Connection"); }
	HICON GetIcon() const { return m_hIcon; }
	LPCTSTR GetTypeID() const { return ODBCTASK::GetMetaDataKey(NULL); }
	
	bool RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent);
	bool StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bSilent);

protected:
	HICON m_hIcon;

protected:
	int LoadDatabaseSetups(IPreferences* pPrefs, LPCTSTR szKey, COdbcDatabaseSetupArray& aDbSetup) const;
	void SaveDatabaseSetups(IPreferences* pPrefs, LPCTSTR szKey, const COdbcDatabaseSetupArray& aDbSetup) const;
	bool GetDatabaseSetup(const CString& sConnect, const COdbcDatabaseSetupArray& aDbSetup, ODBCDATABASESETUP& dbSetup) const;

	static bool LoadFromDatabase(const ODBCDATABASESETUP& dbSetup, ITaskList12* pTasks, CString& sErrMsg);

	static bool SaveToDatabase(const ITaskList12* pTasks, const ODBCDATABASESETUP& dbSetup, CString& sErrMsg);

	static BOOL AddOrUpdateTask(const ITaskList12* pTasks, HTASKITEM hTask, time64_t tLoad, COdbcTaskWriter& rs, 
								const ODBCDATABASESETUP& dbSetup, COdbcMapIDToKey& mapIDs, BOOL bAndSiblings);

};

#endif // !defined(AFX_ODBCTASKLISTSTORAGE_H__1A49027C_CC71_4EB8_8540_B319AB9A045E__INCLUDED_)
