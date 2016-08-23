// OdbcTaskRecordset.cpp : implementation file
//

#include "stdafx.h"
#include "OdbcTaskRecordset.h"
#include "OdbcHelper.h"

#include "..\Shared\EnString.h"
#include "..\Shared\Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// COdbcTaskReader

COdbcTaskReader::COdbcTaskReader(CDatabase* pDatabase) : CRecordsetEx(pDatabase)
{
	ASSERT(pDatabase && pDatabase->IsOpen());

	//{{AFX_FIELD_INIT(COdbcTaskRecordset)
	//}}AFX_FIELD_INIT
	m_nDefaultType = forwardOnly;
}

COdbcTaskReader::COdbcTaskReader()
{
	//{{AFX_FIELD_INIT(COdbcTaskRecordset)
	//}}AFX_FIELD_INIT
	m_nDefaultType = forwardOnly;
}

COdbcTaskReader::~COdbcTaskReader()
{
}


BOOL COdbcTaskReader::VerifyDatabase() const
{
	return (m_pDatabase && m_pDatabase->IsOpen());
}

BOOL COdbcTaskReader::Open(CDatabase* pDatabase, const ODBCDATABASESETUP& dbSetup)
{
	m_pDatabase = pDatabase;

	if (Open(dbSetup))
		return TRUE;

	m_pDatabase = NULL;
	return FALSE;
}
	
BOOL COdbcTaskReader::Open(const ODBCDATABASESETUP& dbSetup)
{
	ASSERT(VerifyDatabase());
	
	if (!VerifyDatabase())
		return FALSE;
	
	if (!dbSetup.Verify())
		return FALSE;

	m_dbSetup = dbSetup;
	
	m_mapAttribMappings.RemoveAll();
	m_mapLinkMappings.RemoveAll();
	
	CString sQuery;
	VERIFY(BuildQuery(sQuery));

	if (CRecordsetEx::Open(forwardOnly, sQuery))
	{
		return (GetAllFieldInfo(m_aFieldInfo) > 0);
	}

	// else 
	return FALSE;
}

BOOL COdbcTaskReader::BuildQuery(CString& sQuery)
{
	if (!m_dbSetup.Verify())
		return FALSE;

	// build a list of attributes to load from 'Tasks' table
	CString sAttribs = COdbcHelper::FormatFieldsForQuery(m_dbSetup.setup);

	// Add WHERE clause for tasklist_ID
	CString sWhereTasklist;

	if (!m_dbSetup.sTasklistID.IsEmpty() && !m_dbSetup.setup.tables.tableTasks.sTasklistKeyField.IsEmpty())
	{
		sWhereTasklist.Format(_T("%s.%s = %s"), 
								m_dbSetup.setup.tables.tableTasks.sTableName,
								m_dbSetup.setup.tables.tableTasks.sTasklistKeyField,
								m_dbSetup.sTasklistID);
	}

	// Add WHERE clause for allocTo_ID
	CString sWhereAllocTo;
	ODBCATTRIBUTESETUP attrib;

	// save tasklist ID off for later link table processing
	m_sTasklistIDFilter = m_dbSetup.sTasklistID;

	// this is only set if we need to advanced filtering
	m_sAllocToFilter.Empty();

	if (!m_dbSetup.sAllocToID.IsEmpty())
	{
		if (COdbcHelper::GetTaskAttributeSetup(m_dbSetup.setup, TDCA_ALLOCTO, attrib))
		{ 
			// this will only work if the mapping is one to one 
			// else we need to filter the results later
			if (attrib.sLinkTable.IsEmpty())
			{
				sWhereAllocTo.Format(_T("%s.%s = %s"), 
										m_dbSetup.setup.tables.tableTasks.sTableName,
										attrib.sAttribKeyField,
										m_dbSetup.sAllocToID);
			}
			else
			{
				m_sAllocToFilter = m_dbSetup.sAllocTo;
			}
		}
	}

	// build complete WHERE clause
	CString sWhere;

	if (!sWhereAllocTo.IsEmpty() && !sWhereTasklist.IsEmpty())
	{
		sWhere.Format(_T("WHERE %s AND %s"), sWhereTasklist, sWhereAllocTo);
	}
	else if (!sWhereTasklist.IsEmpty())
	{
		sWhere.Format(_T("WHERE %s"), sWhereTasklist);
	}
	else if (!sWhereAllocTo.IsEmpty())
	{
		sWhere.Format(_T("WHERE %s"), sWhereAllocTo);
	}

	// build SORT BY clause
	CString sOrderBy;

	if (!m_dbSetup.setup.tables.tableTasks.sParentKeyField.IsEmpty())
		sOrderBy.Format(_T("ORDER BY %s"), m_dbSetup.setup.tables.tableTasks.sParentKeyField);

	sQuery.Format(_T("SELECT %s\nFROM %s\n%s\n%s"), 
					sAttribs, 
					m_dbSetup.setup.tables.tableTasks.sTableName, 
					sWhere, sOrderBy);

	TRACE (_T("COdbcTaskWriter::BuildQuery(%s)\n"), sQuery);

	return TRUE;
}

int COdbcTaskReader::GetTasks(COdbcTaskArray& aTasks)
{
	aTasks.RemoveAll();

	ASSERT(IsOpen() && !IsBOF());

	if (IsOpen() && !IsBOF())
	{
		while (!IsEOF())
		{
			ODBCTASK* pTask = new ODBCTASK;
			
			if (PopulateTask(*pTask))
			{
				aTasks.Add(pTask);
			}
			else
			{
				delete pTask;
			}
			
			MoveNext();
		}

		aTasks.Sort();
	}

	return aTasks.GetSize();
}

BOOL COdbcTaskReader::PopulateTask(ODBCTASK& task)
{
	task.sTaskKey = CRecordsetEx::GetFieldValue(0);
	ASSERT(!task.sTaskKey.IsEmpty());

	if (task.sTaskKey.IsEmpty())
		return FALSE;

	task.sParentKey = CRecordsetEx::GetFieldValue(1);

	// NOTE: first field is parent and task 'keys' 
	// so other fields start after that
	short nField = 2;

	for (int nAtt = 0; nAtt < m_dbSetup.setup.aAttrib.GetSize(); nAtt++, nField++)
	{
		const ODBCATTRIBUTESETUP& attrib = m_dbSetup.setup.aAttrib[nAtt];

		switch (attrib.nAttrib)
		{
		case TDCA_ALLOCTO:
			// special case: check for 'alloc-to' filtering
			if (GetFieldValues(nField, attrib, task.aAllocTo, task, OT_ALLOCTO))
			{
				if (!m_sAllocToFilter.IsEmpty())
				{
					if (Misc::Find(task.aAllocTo, m_sAllocToFilter, FALSE, FALSE) == -1)
						return FALSE;
				}
			}
			else if (!m_sAllocToFilter.IsEmpty())
			{
				return FALSE;
			}
			break;

		case TDCA_RECURRENCE: 
			// TODO
			// task.SetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, bRecalcFromDue, nReuse);
			break;

		case TDCA_COLOR:		GetFieldValue(nField, attrib, task.color,			task, OT_COLOR);		break;
		case TDCA_COMMENTS:		GetFieldValue(nField, attrib, task.sComments,		task, OT_COMMENTS);		break;
		case TDCA_COST:			GetFieldValue(nField, attrib, task.dCost,			task, OT_COST);			break;
		case TDCA_CREATEDBY:	GetFieldValue(nField, attrib, task.sCreatedBy,		task, OT_CREATEDBY);	break;
		case TDCA_CREATIONDATE: GetFieldValue(nField, attrib, task.tCreation,		task, OT_CREATIONDATE);	break;
		case TDCA_ALLOCBY:		GetFieldValue(nField, attrib, task.sAllocBy,		task, OT_ALLOCBY);		break;
		case TDCA_DONEDATE:		GetFieldValue(nField, attrib, task.tDone,			task, OT_DONEDATE);		break;
		case TDCA_DUEDATE:		GetFieldValue(nField, attrib, task.tDue,			task, OT_DUEDATE);		break;
		case TDCA_EXTERNALID:	GetFieldValue(nField, attrib, task.sExternalID,		task, OT_EXTERNALID);	break;
		case TDCA_FILEREF:		GetFieldValue(nField, attrib, task.sFileRef,		task, OT_FILEREF);		break;
		case TDCA_FLAG:			GetFieldValue(nField, attrib, task.bFlag,			task, OT_FLAG);			break;
		case TDCA_ICON:			GetFieldValue(nField, attrib, task.sIcon,			task, OT_ICON);			break;
		case TDCA_LASTMOD:		GetFieldValue(nField, attrib, task.tModified,		task, OT_LASTMOD);		break;
		case TDCA_PERCENT:		GetFieldValue(nField, attrib, task.nPercent,		task, OT_PERCENT);		break;
		case TDCA_PRIORITY:		GetFieldValue(nField, attrib, task.nPriority,		task, OT_PRIORITY);		break;
		case TDCA_RISK:			GetFieldValue(nField, attrib, task.nRisk,			task, OT_RISK);			break;
		case TDCA_STARTDATE:	GetFieldValue(nField, attrib, task.tStart,			task, OT_STARTDATE);	break;
		case TDCA_STATUS:		GetFieldValue(nField, attrib, task.sStatus,			task, OT_STATUS);		break;
		case TDCA_TASKNAME:		GetFieldValue(nField, attrib, task.sTitle,			task, OT_TITLE);		break;
		case TDCA_TIMEEST:		GetFieldValue(nField, attrib, task.dTimeEst,		task, OT_TIMEEST);		break;
		case TDCA_TIMESPENT:	GetFieldValue(nField, attrib, task.dTimeSpent,		task, OT_TIMESPENT);	break;
		case TDCA_VERSION:		GetFieldValue(nField, attrib, task.sVersion,		task, OT_VERSION);		break;

		case TDCA_CATEGORY:		GetFieldValues(nField, attrib, task.aCategories,	task, OT_CATEGORY);		break;
		case TDCA_DEPENDENCY:	GetFieldValues(nField, attrib, task.aDepends,		task, OT_DEPENDENCY);	break;
		case TDCA_TAGS:			GetFieldValues(nField, attrib, task.aTags,			task, OT_TAGS);			break;
		}
	}

	return TRUE;
}

BOOL COdbcTaskReader::GetFieldValue(short nField, const ODBCATTRIBUTESETUP& attrib, CString& sVal, ODBCTASK& task, DWORD dwAttrib)
{
	ASSERT(dwAttrib);

	if (CRecordsetEx::GetFieldValue(nField, sVal))
	{
		// do any necessary mapping
		if (!MapValue(sVal, attrib))
			sVal.Empty();
		else
			task.SetHasAttribute(dwAttrib);
	
		return !sVal.IsEmpty();
	}

	return FALSE;
}

BOOL COdbcTaskReader::GetFieldValue(short nField, const ODBCATTRIBUTESETUP& /*attrib*/, double& dVal, ODBCTASK& task, DWORD dwAttrib)
{
	ASSERT(dwAttrib);

	BOOL bRes = CRecordsetEx::GetFieldValue(nField, dVal);

	if (bRes)
	{
		task.SetHasAttribute(dwAttrib);
	}

	return bRes;
}

BOOL COdbcTaskReader::GetFieldValue(short nField, const ODBCATTRIBUTESETUP& /*attrib*/, long& nVal, ODBCTASK& task, DWORD dwAttrib)
{
	ASSERT(dwAttrib);

	BOOL bRes = CRecordsetEx::GetFieldValue(nField, nVal);

	if (bRes)
	{
		task.SetHasAttribute(dwAttrib);
	}

	return bRes;
}

BOOL COdbcTaskReader::GetFieldValue(short nField, const ODBCATTRIBUTESETUP& /*attrib*/, time64_t& tVal, ODBCTASK& task, DWORD dwAttrib)
{
	ASSERT(dwAttrib);

	BOOL bRes = CRecordsetEx::GetFieldValue(nField, tVal);

	if (bRes)
	{
		task.SetHasAttribute(dwAttrib);
	}

	return bRes;
}

int COdbcTaskReader::GetFieldValues(short nField, const ODBCATTRIBUTESETUP& attrib, CStringArray& aVals, ODBCTASK& task, DWORD dwAttrib)
{
	ASSERT(dwAttrib);

	CString sVal;
	
	if (!CRecordsetEx::GetFieldValue(nField, sVal) || sVal.IsEmpty())
		return 0;

	if (!MapValue(sVal, aVals, attrib))
		return 0;

	task.SetHasAttribute(dwAttrib);

	return aVals.GetSize();
}

BOOL COdbcTaskReader::MapValue(CString& sVal, const ODBCATTRIBUTESETUP& attrib)
{
	if (!sVal.IsEmpty() && !attrib.sAttribTable.IsEmpty())
	{
		// get attribute mapping
		CMapStringToString* pAttribMapping = GetAttributeMapping(attrib);
		ASSERT(pAttribMapping);

		if (!pAttribMapping || !pAttribMapping->Lookup(sVal, sVal))
			sVal.Empty();
	}

	return (!sVal.IsEmpty());
}

int COdbcTaskReader::MapValue(const CString& sVal, CStringArray& aVals, const ODBCATTRIBUTESETUP& attrib)
{
	aVals.RemoveAll();

	if (!sVal.IsEmpty() && !attrib.sAttribTable.IsEmpty())
	{
		// to get multiple values there must be a link table
		const CMapStringToStringArray* pLinkMapping = GetLinkMapping(attrib);

		if (pLinkMapping == NULL)
			return 0;

		// get attribute mapping
		const CMapStringToString* pAttribMapping = GetAttributeMapping(attrib);

		ASSERT(pAttribMapping);

		if (pAttribMapping == NULL)
			return 0;

		// retrieve links
		const CStringArray* pLinks = pLinkMapping->GetMapping(sVal);

		if (!pLinks || !pLinks->GetSize())
			return 0;

		// convert to 'content'
		for (int nLink = 0; nLink < pLinks->GetSize(); nLink++)
		{
			CString sAttrib;

			if (pAttribMapping->Lookup(pLinks->GetAt(nLink), sAttrib) && !sAttrib.IsEmpty())
				aVals.Add(sAttrib);
		}
	}

	return aVals.GetSize();
}

CMapStringToString* COdbcTaskReader::GetAttributeMapping(const ODBCATTRIBUTESETUP& attrib)
{
	ASSERT(m_pDatabase);

	if (!attrib.sAttribTable.IsEmpty())
	{
		ASSERT(attrib.Verify(FALSE));

		CMapStringToString* pMapping = m_mapAttribMappings.GetAddMapping(attrib.sAttribTable);
		ASSERT(pMapping);

		CRecordsetEx::BuildFieldMapping(*m_pDatabase, attrib.sAttribTable, 
										attrib.sAttribKeyField, attrib.sAttribContentField,
										*pMapping);

		return pMapping;
	}

	// else 
	return NULL;
}

CMapStringToStringArray* COdbcTaskReader::GetLinkMapping(const ODBCATTRIBUTESETUP& attrib)
{
	ASSERT(m_pDatabase);

	if (!attrib.sLinkTable.IsEmpty())
	{
		ASSERT(attrib.Verify(!m_sTasklistIDFilter.IsEmpty()));

		CMapStringToStringArray* pMapping = m_mapLinkMappings.GetAddMapping(attrib.sLinkTable);
		ASSERT(pMapping);

		// if the tasklist_ID is set then include that as a 'where' clause
		CString sWhere;

		if (!m_sTasklistIDFilter.IsEmpty() && !attrib.sLinkTaskListKeyField.IsEmpty())
			sWhere.Format(_T("WHERE %s = %s"), attrib.sLinkTaskListKeyField, m_sTasklistIDFilter);

		CRecordsetEx::BuildFieldMapping(*m_pDatabase, attrib.sLinkTable, 
										attrib.sLinkTaskKeyField, attrib.sLinkAttribKeyField,
										*pMapping, sWhere);

		return pMapping;
	}

	// else 
	return NULL;

}

/////////////////////////////////////////////////////////////////////////////
// COdbcTaskReader

COdbcTaskWriter::COdbcTaskWriter() : m_pDatabaseEx(NULL)
{
}

COdbcTaskWriter::~COdbcTaskWriter()
{
	// delete temp column used for mapping GUIDs to task keys
	if (VerifyDatabase() && !m_sTempColumnName.IsEmpty())
	{
		Close();

		CString sQuery;
		sQuery.Format(_T("ALTER TABLE %s DROP COLUMN %s"), 
						m_dbSetup.setup.tables.tableTasks.sTableName,
						m_sTempColumnName);


		TRACE (_T("COdbcTaskWriter::~COdbcTaskWriter(%s)\n"), sQuery);

		VERIFY(m_pDatabaseEx->ExecuteSQL(sQuery));
	}
}

BOOL COdbcTaskWriter::VerifyDatabase() const
{
	return (m_pDatabaseEx && m_pDatabaseEx->IsOpen());
}

BOOL COdbcTaskWriter::Open(CDatabaseEx* pDatabase, const ODBCDATABASESETUP& dbSetup, LPCTSTR szTasklistID)
{
	if (!COdbcTaskReader::Open(pDatabase, dbSetup))
		return FALSE;
	
	COdbcTaskReader::VerifyDatabase();

	// close reader recordset, leaving db open
	Close();

	m_pDatabaseEx = pDatabase;
	m_dbSetup.sTasklistID = szTasklistID;

	BuildFieldMappings();
	
	return TRUE;
}

void COdbcTaskWriter::BuildFieldMappings()
{
	for (int nAtt = 0; nAtt < m_dbSetup.setup.aAttrib.GetSize(); nAtt++)
	{
		const ODBCATTRIBUTESETUP& attrib = m_dbSetup.setup.aAttrib[nAtt];
		
		GetAttributeMapping(attrib);
		GetLinkMapping(attrib);
	}			
}

BOOL COdbcTaskWriter::AddTask(const ODBCTASK& task, CString& sTaskKey)
{
	ASSERT(VerifyDatabase());
	
	if (!VerifyDatabase())
		return FALSE;

	// try parameterized query first
	CString sTaskGuid;
	CDBVariantArray aParams;

	if (BuildAndExecuteInsertQuery(task, sTaskGuid, aParams))
	{
		// extract the key of the just created task
		return GetTaskKey(sTaskGuid, sTaskKey);
	}

	// fallback on 'normal' query
	if (BuildAndExecuteInsertQuery(task, sTaskGuid))
	{
		// extract the key of the just created task
		return GetTaskKey(sTaskGuid, sTaskKey);
	}

	// all else
	return FALSE;
}

BOOL COdbcTaskWriter::GetTaskKey(const CString& sTaskGuid, CString& sTaskKey)
{
	ASSERT(!sTaskGuid.IsEmpty());

	if (sTaskGuid.IsEmpty())
		return FALSE;

	Close();

	// build a query that retrieves the task key for the
	// row having the specified GUID in the temp column
	CString sQuery;
	sQuery.Format(_T("SELECT %s FROM %s WHERE %s = '%s'"),
					m_dbSetup.setup.tables.tableTasks.sTaskKeyField,
					m_dbSetup.setup.tables.tableTasks.sTableName,
					m_sTempColumnName,
					sTaskGuid);
	
	if (CRecordsetEx::Open(forwardOnly, sQuery))
	{
		sTaskKey = CRecordsetEx::GetFieldValue(0);
	}

	// else
	return (!sTaskKey.IsEmpty());
}

BOOL COdbcTaskWriter::UpdateAttributeAndLinkTables(const ODBCTASK& /*task*/)
{
	return FALSE;
}

BOOL COdbcTaskWriter::UpdateAttributeAndLinkTables(const ODBCTASK& /*task*/, TDC_ATTRIBUTE /*nAttrib*/, const CString& /*sVal*/, DWORD /*dwAttrib*/)
{
	return FALSE;
}

BOOL COdbcTaskWriter::UpdateTask(const ODBCTASK& task)
{
	ASSERT(VerifyDatabase());
	
	if (!VerifyDatabase())
		return FALSE;
	
	// try parameterized query first and fallback on 'normal' query
	CDBVariantArray aParams;
	
	if (!BuildAndExecuteUpdateQuery(task, aParams, TRUE)) // TRUE = fallback
		return FALSE;

	// update attribute and link tables with new values
	VERIFY(UpdateAttributeAndLinkTables(task));

	// all else
	return FALSE;
}

BOOL COdbcTaskWriter::BuildAndExecuteUpdateQuery(const ODBCTASK& task)
{
	CString sQuery;
	
	if (!BuildUpdateQuery(task, sQuery))
		return FALSE;
	
	return ExecuteQuery(sQuery);
}

BOOL COdbcTaskWriter::BuildUpdateQuery(const ODBCTASK& task, CString& sQuery)
{
	if (!m_dbSetup.Verify())
		return FALSE;
	
	ASSERT(!task.sTaskKey.IsEmpty());
	
	if (task.sTaskKey.IsEmpty())
		return FALSE;
	
	// build a list of attributes to write to the 'Tasks' table
	CString sAttribs = COdbcHelper::FormatFieldsAndValuesForUpdate(m_dbSetup.setup, task, m_aFieldInfo);
	
	// Add WHERE clause for tasklist_ID and task_ID
	CString sWhereTask, sWhereTasklist;
	
	sWhereTask.Format(_T("%s = %s"), 
		m_dbSetup.setup.tables.tableTasks.sTaskKeyField,
		task.sTaskKey);
	
	if (!m_dbSetup.sTasklistID.IsEmpty() && !m_dbSetup.setup.tables.tableTasks.sTasklistKeyField.IsEmpty())
	{
		sWhereTasklist.Format(_T("%s = %s"), 
			m_dbSetup.setup.tables.tableTasks.sTasklistKeyField,
			m_dbSetup.sTasklistID);
	}
	
	// build complete WHERE clause
	CString sWhere;
	
	if (!sWhereTasklist.IsEmpty())
	{
		sWhere.Format(_T("WHERE %s AND %s"), sWhereTasklist, sWhereTask);
	}
	else
	{
		sWhere.Format(_T("WHERE %s"), sWhereTask);
	}
	
	sQuery.Format(_T("UPDATE %s\nSET %s\n%s;"),
		m_dbSetup.setup.tables.tableTasks.sTableName,
		sAttribs, 
		sWhere);
	
	TRACE (_T("COdbcTaskWriter::BuildUpdateQuery(%s)\n"), sQuery);
	
	return TRUE;
}

BOOL COdbcTaskWriter::BuildAndExecuteInsertQuery(const ODBCTASK& task, CString& sTaskGuid)
{
	CString sQuery;
	
	if (!BuildInsertQuery(task, sQuery, sTaskGuid))
		return FALSE;
	
	return ExecuteQuery(sQuery);
}

BOOL COdbcTaskWriter::BuildInsertQuery(const ODBCTASK& task, CString& sQuery, CString& sTaskGuid)
{
	if (!m_dbSetup.Verify())
		return FALSE;
	
	ASSERT(task.sTaskKey.IsEmpty());
	
	if (!task.sTaskKey.IsEmpty())
		return FALSE;
	
	// Build insert statement
	CString sFields = COdbcHelper::FormatFieldsForInsert(m_dbSetup.setup);
	
	// add temp GUID column
	VERIFY(InitTempColumn());
	COdbcHelper::AppendAttributeToList(m_sTempColumnName, sFields);
	
	CString sValues = COdbcHelper::FormatValuesForInsert(m_dbSetup.setup, task, m_aFieldInfo, m_dbSetup.sTasklistID);
	
	// add temp GUID id
	sTaskGuid = Misc::NewGuid();
	COdbcHelper::AppendAttributeToList(COdbcHelper::QuoteValue(sTaskGuid, SQL_VARCHAR), sValues);
	
	// build final query
	sQuery.Format(_T("INSERT INTO %s (%s)\nVALUES (%s);"), 
		m_dbSetup.setup.tables.tableTasks.sTableName,
		sFields,
		sValues);
	
	TRACE (_T("COdbcTaskWriter::BuildInsertQuery(%s)\n"), sQuery);
	
	return TRUE;
}

BOOL COdbcTaskWriter::ExecuteQuery(const CString& sQuery, const CDBVariantArray* pParams)
{
	if (pParams && pParams->GetSize())
		return m_pDatabaseEx->PrepareAndExecuteSQL(sQuery, *pParams);

	// else
	return m_pDatabaseEx->ExecuteSQL(sQuery);
}

BOOL COdbcTaskWriter::BuildAndExecuteUpdateQuery(const ODBCTASK& task, CDBVariantArray& aParams, BOOL bAllowFallback)
{
	CString sQuery;

	if (!BuildUpdateQuery(task, sQuery, aParams))
		return FALSE;
	
	if (ExecuteQuery(sQuery, &aParams))
		return TRUE;

	// try fallback
	if (bAllowFallback)
		return BuildAndExecuteUpdateQuery(task);

	// all else
	return FALSE;
}

BOOL COdbcTaskWriter::BuildUpdateQuery(const ODBCTASK& task, CString& sQuery, 
									   CDBVariantArray& aParams)
{
	if (!m_dbSetup.Verify())
		return FALSE;

	ASSERT(!task.sTaskKey.IsEmpty());
	
	if (task.sTaskKey.IsEmpty())
		return FALSE;

	// Build parameterised query
	m_pDatabaseEx->NewQuery();
	aParams.RemoveAll();
	
	// build a list of attributes to write to the 'Tasks' table
	CString sAttribs = COdbcHelper::FormatFieldsAndValuesForParameterizedUpdate(m_dbSetup.setup, task, m_aFieldInfo, aParams);

	// Add WHERE clause for tasklist_ID and task_ID
	CString sWhereTask, sWhereTasklist;

	sWhereTask.Format(_T("%s = %s"), 
						m_dbSetup.setup.tables.tableTasks.sTaskKeyField,
						task.sTaskKey);

	if (!m_dbSetup.sTasklistID.IsEmpty() && !m_dbSetup.setup.tables.tableTasks.sTasklistKeyField.IsEmpty())
	{
		sWhereTasklist.Format(_T("%s = %s"), 
								m_dbSetup.setup.tables.tableTasks.sTasklistKeyField,
								m_dbSetup.sTasklistID);
	}

	// build complete WHERE clause
	CString sWhere;

	if (!sWhereTasklist.IsEmpty())
	{
		sWhere.Format(_T("WHERE %s AND %s"), sWhereTasklist, sWhereTask);
	}
	else
	{
		sWhere.Format(_T("WHERE %s"), sWhereTask);
	}

	sQuery.Format(_T("UPDATE %s\nSET %s\n%s;"),
					m_dbSetup.setup.tables.tableTasks.sTableName,
					sAttribs, 
					sWhere);

	TRACE (_T("COdbcTaskWriter::BuildUpdateQuery(%s)\n"), sQuery);

	return TRUE;
}

BOOL COdbcTaskWriter::BuildAndExecuteInsertQuery(const ODBCTASK& task, CString& sTaskGuid, 
												 CDBVariantArray& aParams, BOOL bAllowFallback)
{
	CString sQuery;
	
	if (!BuildInsertQuery(task, sQuery, sTaskGuid, aParams))
		return FALSE;

	if (ExecuteQuery(sQuery, &aParams))
		return TRUE;

	// try fallback
	if (bAllowFallback)
		return BuildAndExecuteInsertQuery(task, sTaskGuid);

	// else
	return FALSE;
}

BOOL COdbcTaskWriter::BuildInsertQuery(const ODBCTASK& task, CString& sQuery, 
									   CString& sTaskGuid, CDBVariantArray& /*aParams*/)
{
	if (!m_dbSetup.Verify())
		return FALSE;

	ASSERT(task.sTaskKey.IsEmpty());
	
	if (!task.sTaskKey.IsEmpty())
		return FALSE;
	
	// Build insert statement
	CString sFields = COdbcHelper::FormatFieldsForInsert(m_dbSetup.setup);
	
	// add temp GUID column
	VERIFY(InitTempColumn());
	COdbcHelper::AppendAttributeToList(m_sTempColumnName, sFields);

	CString sValues = COdbcHelper::FormatValuesForInsert(m_dbSetup.setup, task, m_aFieldInfo, m_dbSetup.sTasklistID);

	// add temp GUID id
	sTaskGuid = Misc::NewGuid();
	COdbcHelper::AppendAttributeToList(COdbcHelper::QuoteValue(sTaskGuid, SQL_VARCHAR), sValues);

	// build final query
	sQuery.Format(_T("INSERT INTO %s (%s)\nVALUES (%s);"), 
					m_dbSetup.setup.tables.tableTasks.sTableName,
					sFields,
					sValues);
	
	TRACE (_T("COdbcTaskWriter::BuildInsertQuery(%s)\n"), sQuery);

	return TRUE;
}

BOOL COdbcTaskWriter::InitTempColumn()
{
	ASSERT(m_pDatabaseEx && m_pDatabaseEx->IsOpen());
	
	if (m_sTempColumnName.IsEmpty())
	{
		// use a GUID as column name so we do not interfere
		// with anyone else's update 
		CString sColName(_T("COL_") + Misc::NewGuid());
		sColName.Replace(_T("-"), _T(""));

		CString sQuery;
		sQuery.Format(_T("ALTER TABLE %s ADD %s VARCHAR;"), 
						m_dbSetup.setup.tables.tableTasks.sTableName,
						sColName);
			
		TRACE (_T("COdbcTaskWriter::InitTempColumn(%s)\n"), sQuery);

		if (m_pDatabaseEx->ExecuteSQL(sQuery))
			m_sTempColumnName = sColName;
	}

	return !m_sTempColumnName.IsEmpty();
}
