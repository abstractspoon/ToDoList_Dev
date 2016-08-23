#if !defined(AFX_ODBCTASKRECORDSET_H__3AC2AFF2_8263_46BE_B605_9997E5C32336__INCLUDED_)
#define AFX_ODBCTASKRECORDSET_H__3AC2AFF2_8263_46BE_B605_9997E5C32336__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OdbcTaskRecordset.h : header file
//

#include "OdbcStruct.h"
#include "Odbchelper.h"

#include "..\shared\RecordsetEx.h"

#include "..\Interfaces\ITaskList.h"

/////////////////////////////////////////////////////////////////////////////

class CDatabaseEx;

/////////////////////////////////////////////////////////////////////////////
// COdbcTaskRecordset recordset

class COdbcTaskReader : public CRecordsetEx
{
public:
	COdbcTaskReader(CDatabase* pDatabase);
	virtual ~COdbcTaskReader();

	BOOL Open(const ODBCDATABASESETUP& dbSetup);
	BOOL VerifyDatabase() const;

	int GetTasks(COdbcTaskArray& aTasks);
	BOOL PopulateTask(ODBCTASK& task);

protected:
	ODBCDATABASESETUP m_dbSetup;
	CODBCFieldInfoArray m_aFieldInfo;

	// There are three ways to get the value of an attribute:
	//
	// 1. The value in the attribute column of the task table
	//    is the value desired. No special handling is required.
	//
	// 2. The value in the attribute column of the task table
	//    is the key into another table containing the value 
	//    desired.
	//
	//    To handle this we store a map of the 'attribute'
	//    tables and their key/value pairs. We then use the 
	//    key value in the attribute column to lookup the 
	//    desired value in the correct attribute table.
	CMapStringToStringMap m_mapAttribMappings;
	//
	// 3. The value desired is a list of values, in which case
	//    the attribute column of the task table is a key into
	//    a link table that provides a list of keys into the
	//    attribute table.
	//
	//    Because a column in a table ought to only hold a 
	//    single value, we need a third table whose values
	//    are actually a pair of keys that allows the mapping
	//    of multiple attribute values to a single task.
	CMapStringToStringArrayMap m_mapLinkMappings;

	// restrictions
	CString m_sAllocToFilter, m_sTasklistIDFilter;

protected:
	COdbcTaskReader(); // for COdbcTaskWriter
	BOOL Open(CDatabase* pDatabase, const ODBCDATABASESETUP& dbSetup); // for COdbcTaskWriter

	BOOL BuildQuery(CString& sQuery);

	BOOL GetFieldValue(short nField, const ODBCATTRIBUTESETUP& attrib, CString& sVal, ODBCTASK& task, DWORD dwAttrib);
	BOOL GetFieldValue(short nField, const ODBCATTRIBUTESETUP& attrib, double& dVal, ODBCTASK& task, DWORD dwAttrib);
	BOOL GetFieldValue(short nField, const ODBCATTRIBUTESETUP& attrib, long& nVal, ODBCTASK& task, DWORD dwAttrib);
	BOOL GetFieldValue(short nField, const ODBCATTRIBUTESETUP& attrib, time64_t& tVal, ODBCTASK& task, DWORD dwAttrib);
	int GetFieldValues(short nField, const ODBCATTRIBUTESETUP& attrib, CStringArray& aVals, ODBCTASK& task, DWORD dwAttrib);

	CMapStringToString* GetAttributeMapping(const ODBCATTRIBUTESETUP& attrib);
	CMapStringToStringArray* GetLinkMapping(const ODBCATTRIBUTESETUP& attrib);

	BOOL MapValue(CString& sVal, const ODBCATTRIBUTESETUP& attrib);
	int MapValue(const CString& sVal, CStringArray& aVals, const ODBCATTRIBUTESETUP& attrib);
};

class COdbcTaskWriter : protected COdbcTaskReader
{
public:
	COdbcTaskWriter();
	virtual ~COdbcTaskWriter();

	BOOL Open(CDatabaseEx* pDatabase, const ODBCDATABASESETUP& dbSetup, LPCTSTR szTasklistID);
	BOOL VerifyDatabase() const;

	BOOL AddTask(const ODBCTASK& task, CString& sTaskKey);
	BOOL UpdateTask(const ODBCTASK& task);
	BOOL DeleteTask(const ODBCTASK& task);

protected:
	// temp column used to map new tasks to GUIDs so we
	// can reverse lookup the keys of newly added tasks
	mutable CString m_sTempColumnName;

protected:
	CDatabaseEx* m_pDatabaseEx;

protected:
	BOOL BuildUpdateQuery(const ODBCTASK& task, CString& sQuery);
	BOOL BuildAndExecuteUpdateQuery(const ODBCTASK& task);

	BOOL BuildInsertQuery(const ODBCTASK& task, CString& sQuery, CString& sTaskGuid);
	BOOL BuildAndExecuteInsertQuery(const ODBCTASK& task, CString& sTaskGuid);

	// parameterized 
	BOOL BuildUpdateQuery(const ODBCTASK& task, CString& sQuery, CDBVariantArray& aParams);
	BOOL BuildAndExecuteUpdateQuery(const ODBCTASK& task, CDBVariantArray& aParams, BOOL bAllowFallback = FALSE);

	BOOL BuildInsertQuery(const ODBCTASK& task, CString& sQuery, CString& sTaskGuid, CDBVariantArray& aParams);
	BOOL BuildAndExecuteInsertQuery(const ODBCTASK& task, CString& sTaskGuid, CDBVariantArray& aParams, BOOL bAllowFallback = FALSE);

	// misc
	void BuildFieldMappings();
	BOOL InitTempColumn();
	BOOL GetTaskKey(const CString& sTaskGuid, CString& sTaskKey);
	BOOL ExecuteQuery(const CString& sQuery, const CDBVariantArray* pParams = NULL);

	BOOL UpdateAttributeAndLinkTables(const ODBCTASK& task);
	BOOL UpdateAttributeAndLinkTables(const ODBCTASK& task, TDC_ATTRIBUTE nAttrib, const CString& sVal, DWORD dwAttrib);
	BOOL UpdateAttributeAndLinkTables(const ODBCTASK& task, TDC_ATTRIBUTE nAttrib, const CStringArray& aVal, DWORD dwAttrib);
	BOOL UpdateAttributeAndLinkTables(const ODBCTASK& task, TDC_ATTRIBUTE nAttrib, int nVal, DWORD dwAttrib);
	BOOL UpdateAttributeAndLinkTables(const ODBCTASK& task, TDC_ATTRIBUTE nAttrib, double dVal, DWORD dwAttrib);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODBCTASKRECORDSET_H__3AC2AFF2_8263_46BE_B605_9997E5C32336__INCLUDED_)
