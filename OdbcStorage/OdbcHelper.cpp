// OdbcHelper.cpp: implementation of the COdbcHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OdbcHelper.h"
#include "OdbcStruct.h"

#include "..\Shared\databaseex.h"
#include "..\Shared\enstring.h"
#include "..\Shared\datehelper.h"
#include "..\Shared\misc.h"
#include "..\Shared\odbcvaluecombobox.h"
#include "..\Shared\odbcfieldcombobox.h"
#include "..\Shared\odbctablecombobox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static const CString NULL_STRING	= _T("NULL");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString COdbcHelper::FormatName(const ODBCDATABASESETUP& dbSetup, DBEX_FORMAT nFmt)
{
	return CDatabaseEx::FormatName(nFmt, CDatabaseEx::GetDSN(dbSetup.sConnect), dbSetup.sDbName);
}

int COdbcHelper::FindDatabase(const ODBCDATABASESETUP& dbSetup, const COdbcDatabaseSetupArray& aDbSetup)
{
	return FindDatabase(dbSetup.sConnect, aDbSetup);
}

int COdbcHelper::FindDatabase(const CString& sConnect, const COdbcDatabaseSetupArray& aDbSetup)
{
	int nDb = aDbSetup.GetSize();

	while (nDb--)
	{
		const ODBCDATABASESETUP& dbSetup = aDbSetup[nDb];

		if (dbSetup.sConnect.CompareNoCase(sConnect) == 0)
			break;
	}

	return nDb;
}

BOOL COdbcHelper::VerifySetup(const ODBCSETUP& setup, CString& sErrors)
{
	CStringArray aErrors;

	if (!setup.Verify(aErrors))
	{
		sErrors = Misc::FormatArrayAsNumberedList(aErrors);
		return FALSE;
	}

	return TRUE;
}

BOOL COdbcHelper::VerifySetup(const ODBCDATABASESETUP& dbSetup, CString& sErrors)
{
	CStringArray aErrors;

	if (!dbSetup.Verify(aErrors))
	{
		sErrors = Misc::FormatArrayAsNumberedList(aErrors);
		return FALSE;
	}

	return TRUE;
}

void COdbcHelper::MapAttributeValues(const ODBCSETUP& setup, const ODBCTASK& task, CMapStringToStringArray& map)
{
	int nNumAttrib = setup.aAttrib.GetSize();
	
	for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
	{
		const ODBCATTRIBUTESETUP& attrib = setup.aAttrib[nAtt];

		if (!attrib.sAttribTable.IsEmpty())
		{
			CStringArray* pMapping = map.GetAddMapping(attrib.sAttribTable);
			ASSERT(pMapping);

			switch (attrib.nAttrib)
			{
			// simple strings
			case TDCA_TASKNAME:		Misc::AddUniqueItem(task.sTitle,		*pMapping); break;
			case TDCA_STATUS:		Misc::AddUniqueItem(task.sStatus,		*pMapping); break;
			case TDCA_FILEREF:		Misc::AddUniqueItem(task.sFileRef,		*pMapping); break;
			case TDCA_COMMENTS:		Misc::AddUniqueItem(task.sComments,		*pMapping); break;
			case TDCA_CREATEDBY:	Misc::AddUniqueItem(task.sCreatedBy,	*pMapping); break;
			case TDCA_VERSION:		Misc::AddUniqueItem(task.sVersion,		*pMapping); break;
			case TDCA_EXTERNALID:	Misc::AddUniqueItem(task.sExternalID,	*pMapping); break;
			case TDCA_ALLOCBY:		Misc::AddUniqueItem(task.sAllocBy,		*pMapping); break;
			case TDCA_ICON:			Misc::AddUniqueItem(task.sIcon,			*pMapping); break;

			// string arrays
			case TDCA_DEPENDENCY:	Misc::AddUniqueItems(task.aDepends,		*pMapping); break; 
			case TDCA_ALLOCTO:		Misc::AddUniqueItems(task.aAllocTo,		*pMapping); break; 
			case TDCA_CATEGORY:		Misc::AddUniqueItems(task.aCategories,	*pMapping); break; 
			case TDCA_TAGS:			Misc::AddUniqueItems(task.aTags,		*pMapping); break; 
			}
		}
	}
}

CString COdbcHelper::FormatFieldsForQuery(const ODBCSETUP& setup)
{
	CString sAttribs;

	AppendAttributeToList(setup.tables.tableTasks.sTaskKeyField, sAttribs);
	AppendAttributeToList(setup.tables.tableTasks.sParentKeyField, sAttribs);

	int nNumAttrib = setup.aAttrib.GetSize();

	for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
	{
		const ODBCATTRIBUTESETUP& attrib = setup.aAttrib.GetData()[nAtt];

		AppendAttributeToList(attrib.sTaskField, sAttribs);
	}

	return sAttribs;
}

CString COdbcHelper::FormatFieldsAndValuesForUpdate(const ODBCSETUP& setup, const ODBCTASK& task, 
													const CODBCFieldInfoArray& aInfo)
{
	CString sAttribs;
	
	// parent key field (mandatory)
	CString sParentKey = FormatValueAsSQLString(task.sParentKey, aInfo[0], _T("0"));
 	AppendAttributeToList(setup.tables.tableTasks.sParentKeyField, sParentKey, sAttribs);

	// reset of fields
	int nNumAttrib = setup.aAttrib.GetSize();
	ASSERT(nNumAttrib == (aInfo.GetSize() - 2));
	
	for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
	{
		const ODBCATTRIBUTESETUP& attrib = setup.aAttrib[nAtt];
		const CODBCFieldInfo& info = aInfo[nAtt+2];

		CString sValue = GetAttributeValueAsSQLString(task, attrib.nAttrib, info);

		AppendAttributeToList(attrib.sTaskField, sValue, sAttribs);
	}
	
	return sAttribs;
}

CString COdbcHelper::FormatFieldsAndValuesForParameterizedUpdate(const ODBCSETUP& setup, const ODBCTASK& task, 
																const CODBCFieldInfoArray& aInfo, 
																CDBVariantArray& aParams, BOOL bAppend)
{
	if (!bAppend)
		aParams.RemoveAll();

	// parent key field (mandatory)
	CString sAttribs;
	CString sParentKey = FormatValueAsSQLString(task.sParentKey, aInfo[0], _T("0"));

 	AppendAttributeToList(setup.tables.tableTasks.sParentKeyField, sParentKey, sAttribs);

	// reset of fields as parameters
	int nNumAttrib = setup.aAttrib.GetSize();

	ASSERT(nNumAttrib == (aInfo.GetSize() - 2));
	
	for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
	{
		const ODBCATTRIBUTESETUP& attrib = setup.aAttrib[nAtt];
		CDBVariant* pVar = GetAttributeValue(task, attrib.nAttrib);
			
		aParams.Add(pVar);
		AppendAttributeToList(attrib.sTaskField, _T("?"), sAttribs);
	}
	
	return sAttribs;
}

CString COdbcHelper::FormatValuesForParameterizedInsert(const ODBCSETUP& /*setup*/, const ODBCTASK& /*task*/, 
														const CODBCFieldInfoArray& /*aInfo*/, const CString& /*sTasklistID*/, 
														CDBVariantArray& /*aParams*/, BOOL /*bAppend*/)
{
	// TODO

	return _T("");
}

CString COdbcHelper::FormatFieldsForInsert(const ODBCSETUP& setup)
{
	CString sAttribs;
	
	AppendAttributeToList(setup.tables.tableTasks.sParentKeyField, sAttribs);
	AppendAttributeToList(setup.tables.tableTasks.sTasklistKeyField, sAttribs);
	
	int nNumAttrib = setup.aAttrib.GetSize();
	
	for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
	{
		const ODBCATTRIBUTESETUP& attrib = setup.aAttrib.GetData()[nAtt];
		
		AppendAttributeToList(attrib.sTaskField, sAttribs);
	}
	
	return sAttribs;
}

CString COdbcHelper::FormatValuesForInsert(const ODBCSETUP& setup, const ODBCTASK& task, 
										   const CODBCFieldInfoArray& aInfo, const CString& sTasklistID)
{
	CString sAttribs;

	// parent key field (mandatory)
	CString sParentKey = FormatValueAsSQLString(task.sParentKey, aInfo[0], _T("0"));
	AppendAttributeToList(sParentKey, sAttribs);

	// tasklist key field (optional)
	if (!sTasklistID.IsEmpty())
	{
		CString sTasklistKey = FormatValueAsSQLString(sTasklistID, aInfo[1], _T("0"));
		AppendAttributeToList(sTasklistKey, sAttribs);
	}
	
	// NOTE: For fieldinfo array, first field is parent and task 'keys' 
	// so other fields start after that
	int nStartField = 2;
	
	// rest of attributes
	int nNumAttrib = setup.aAttrib.GetSize();
	ASSERT(nNumAttrib == (aInfo.GetSize() - nStartField));
	
	for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
	{
		const ODBCATTRIBUTESETUP& attrib = setup.aAttrib[nAtt];
		const CODBCFieldInfo& info = aInfo[nAtt + nStartField];
		
		CString sValue = GetAttributeValueAsSQLString(task, attrib.nAttrib, info);
		
		AppendAttributeToList(sValue, sAttribs);
	}
	
	return sAttribs;
}

CString COdbcHelper::FormatDateAsSQLString(time64_t tDate, const CODBCFieldInfo& info)
{
	CString sDate;
	COleDateTime date(CDateHelper::GetDate(tDate));

	switch (info.m_nSQLType)
	{
	case SQL_DOUBLE:
	case SQL_FLOAT:
		sDate = Misc::Format(date.m_dt);
		break;

	case SQL_DATE:
	case SQL_TIMESTAMP:
		sDate.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
						date.GetYear(),
						date.GetMonth(),
						date.GetDay(),
						date.GetHour(),
						date.GetMinute(),
						date.GetSecond());
		break;
	}

	ASSERT(!sDate.IsEmpty());
	return sDate;
}

CString COdbcHelper::GetAttributeValueAsSQLString(const ODBCTASK& task, TDC_ATTRIBUTE nAttrib, const CODBCFieldInfo& info)
{
	CString sValue;

#define GET_ATTRIB_STR(attr, str) if (task.HasAttribute(attr)) sValue = str;							break
#define GET_ATTRIB_DATE(attr, dt) if (task.HasAttribute(attr)) sValue = FormatDateAsSQLString(dt, info);break
#define GET_ATTRIB_VAL(attr, val) if (task.HasAttribute(attr)) sValue = Misc::Format(val);				break

	switch (nAttrib)
	{
	// simple strings
	case TDCA_TASKNAME:		GET_ATTRIB_STR(OT_TITLE,			task.sTitle);
	case TDCA_STATUS:		GET_ATTRIB_STR(OT_STATUS,			task.sStatus);
	case TDCA_FILEREF:		GET_ATTRIB_STR(OT_FILEREF,			task.sFileRef);
	case TDCA_COMMENTS:		GET_ATTRIB_STR(OT_COMMENTS,			task.sComments);
	case TDCA_CREATEDBY:	GET_ATTRIB_STR(OT_CREATEDBY,		task.sCreatedBy);
	case TDCA_VERSION:		GET_ATTRIB_STR(OT_VERSION,			task.sVersion);
	case TDCA_EXTERNALID:	GET_ATTRIB_STR(OT_EXTERNALID,		task.sExternalID);
	case TDCA_ALLOCBY:		GET_ATTRIB_STR(OT_ALLOCBY,			task.sAllocBy);
	case TDCA_ICON:			GET_ATTRIB_STR(OT_ICON,				task.sIcon);

	// dates
	case TDCA_DONEDATE:		GET_ATTRIB_DATE(OT_DONEDATE,		task.tDone);
	case TDCA_DUEDATE: 		GET_ATTRIB_DATE(OT_DUEDATE,			task.tDue);
	case TDCA_STARTDATE:	GET_ATTRIB_DATE(OT_STARTDATE,		task.tStart);
	case TDCA_CREATIONDATE:	GET_ATTRIB_DATE(OT_CREATIONDATE,	task.tCreation);
	case TDCA_LASTMOD: 		GET_ATTRIB_DATE(OT_LASTMOD,			task.tModified);

	// ints
	case TDCA_PRIORITY: 	GET_ATTRIB_VAL(OT_PRIORITY,			task.nPriority);
	case TDCA_COLOR:  		GET_ATTRIB_VAL(OT_COLOR,			task.color);
	case TDCA_PERCENT:  	GET_ATTRIB_VAL(OT_PERCENT,			task.nPercent);
	case TDCA_FLAG:  		GET_ATTRIB_VAL(OT_FLAG,				task.bFlag);
	case TDCA_RISK:  		GET_ATTRIB_VAL(OT_RISK,				task.nRisk);

	// arrays
	case TDCA_DEPENDENCY: 
		break;	
		
	case TDCA_ALLOCTO: 
		break;		
		
	case TDCA_CATEGORY: 
		break;		
		
	case TDCA_TAGS: 
		break;			
		
	// floats
	case TDCA_TIMEEST:		GET_ATTRIB_VAL(OT_TIMEEST,			task.dTimeEst);
	case TDCA_TIMESPENT: 	GET_ATTRIB_VAL(OT_TIMESPENT,		task.dTimeSpent);		
	case TDCA_COST:			GET_ATTRIB_VAL(OT_COST,				task.dCost);
		
	case TDCA_RECURRENCE: 
		break;	
		
	case TDCA_POSITION: 
		break;		
	}

	return FormatValueAsSQLString(sValue, info);
}

void COdbcHelper::SetVarValue(const CString& sValue, CDBVariant& var)
{
	var.m_dwType = DBVT_STRING; 
	var.m_pstring = new CString(sValue);
}

void COdbcHelper::SetVarValue(const time64_t& tValue, CDBVariant& var)
{
	var.m_dwType = DBVT_DATE; 
	var.m_pdate = new TIMESTAMP_STRUCT;

	COleDateTime date(CDateHelper::GetDate(tValue));

	var.m_pdate->year	= (SQLSMALLINT)date.GetYear();
	var.m_pdate->month	= (SQLSMALLINT)date.GetMonth();
	var.m_pdate->day	= (SQLSMALLINT)date.GetDay();
	var.m_pdate->hour	= (SQLSMALLINT)date.GetHour();
	var.m_pdate->minute = (SQLSMALLINT)date.GetMinute();
	var.m_pdate->second = (SQLSMALLINT)date.GetSecond();
	var.m_pdate->fraction = 0;
}

void COdbcHelper::SetVarValue(const long& lValue, CDBVariant& var)
{
	var.m_dwType = DBVT_LONG; 
	var.m_lVal = lValue;
}

void COdbcHelper::SetVarValue(const double& dValue, CDBVariant& var)
{
	var.m_dwType = DBVT_DOUBLE; 
	var.m_dblVal = dValue;
}

CDBVariant* COdbcHelper::GetAttributeValue(const ODBCTASK& task, TDC_ATTRIBUTE nAttrib)
{
	CDBVariant* pVar = new CDBVariant;
	
#define GET_ATTRIB_VAR(attr, val) if (task.HasAttribute(attr)) SetVarValue(val, *pVar); break

	switch (nAttrib)
	{
	// simple strings
	case TDCA_TASKNAME:		GET_ATTRIB_VAR(OT_TITLE,		task.sTitle);
	case TDCA_STATUS:		GET_ATTRIB_VAR(OT_STATUS,		task.sStatus);
	case TDCA_FILEREF:		GET_ATTRIB_VAR(OT_FILEREF,		task.sFileRef);
	case TDCA_COMMENTS:		GET_ATTRIB_VAR(OT_COMMENTS,		task.sComments);
	case TDCA_CREATEDBY:	GET_ATTRIB_VAR(OT_CREATEDBY,	task.sCreatedBy);
	case TDCA_VERSION:		GET_ATTRIB_VAR(OT_VERSION,		task.sVersion);
	case TDCA_EXTERNALID:	GET_ATTRIB_VAR(OT_EXTERNALID,	task.sExternalID);
	case TDCA_ALLOCBY:		GET_ATTRIB_VAR(OT_ALLOCBY,		task.sAllocBy);
	case TDCA_ICON:			GET_ATTRIB_VAR(OT_ICON,			task.sIcon);

	// dates
	case TDCA_DONEDATE:		GET_ATTRIB_VAR(OT_DONEDATE,		task.tDone);
	case TDCA_DUEDATE: 		GET_ATTRIB_VAR(OT_DUEDATE,		task.tDue);
	case TDCA_STARTDATE:	GET_ATTRIB_VAR(OT_STARTDATE,	task.tStart);
	case TDCA_CREATIONDATE:	GET_ATTRIB_VAR(OT_CREATIONDATE,	task.tCreation);
	case TDCA_LASTMOD: 		GET_ATTRIB_VAR(OT_LASTMOD,		task.tModified);

	// ints
	case TDCA_PRIORITY: 	GET_ATTRIB_VAR(OT_PRIORITY,		task.nPriority);
	case TDCA_COLOR:  		GET_ATTRIB_VAR(OT_COLOR,		task.color);
	case TDCA_PERCENT:  	GET_ATTRIB_VAR(OT_PERCENT,		task.nPercent);
	case TDCA_FLAG:  		GET_ATTRIB_VAR(OT_FLAG,			task.bFlag);
	case TDCA_RISK:  		GET_ATTRIB_VAR(OT_RISK,			task.nRisk);

	// arrays
	case TDCA_DEPENDENCY: 
		break;	
		
	case TDCA_ALLOCTO: 
		break;		
		
	case TDCA_CATEGORY: 
		break;		
		
	case TDCA_TAGS: 
		break;			
		
	// floats
	case TDCA_TIMEEST:		GET_ATTRIB_VAR(OT_TIMEEST,		task.dTimeEst);
	case TDCA_TIMESPENT: 	GET_ATTRIB_VAR(OT_TIMESPENT,	task.dTimeSpent);		
	case TDCA_COST:			GET_ATTRIB_VAR(OT_COST,			task.dCost);
		
	case TDCA_RECURRENCE: 
		break;	
		
	case TDCA_POSITION: 
		break;		
	}

	return pVar;
}

CString COdbcHelper::FormatValueAsSQLString(const CString& sVal, const CODBCFieldInfo& info, LPCTSTR szDefValue)
{
	CString sValue;
	
	if (sVal.IsEmpty())
	{
		if (info.m_nNullability == SQL_NULLABLE)
			sValue = NULL_STRING;
		else
			sValue = QuoteValue(szDefValue, info);
	}
	else
	{
		sValue = QuoteValue(sVal, info);
	}

	return sValue;
}

CString COdbcHelper::QuoteValue(const CString& sValue, const CODBCFieldInfo& info)
{
	return QuoteValue(sValue, info.m_nSQLType);
}

CString COdbcHelper::QuoteValue(const CString& sValue, int nSQLType)
{
	CString sQuoted(sValue);

	switch (nSQLType)
	{
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_LONGVARCHAR:
	case SQL_WCHAR:		 
	case SQL_WVARCHAR:	 
	case SQL_WLONGVARCHAR:
	case SQL_TIMESTAMP:	
	case SQL_DATE:		
		{
			CString sEscaped(sValue);
			
			// escape any existing quotes
			if (sEscaped.Find('\'') != -1)
				sEscaped.Replace(_T("'"), _T("''"));

			sQuoted.Format(_T("'%s'"), sEscaped);
		}
		break;

	default:
// 	case SQL_NUMERIC:		
// 	case SQL_DECIMAL:		
// 	case SQL_INTEGER:		
// 	case SQL_SMALLINT:		
// 	case SQL_FLOAT:			
// 	case SQL_REAL:			
// 	case SQL_DOUBLE:		
// 	case SQL_TIME:			
// 	case SQL_BINARY:		
// 	case SQL_VARBINARY:		
// 	case SQL_LONGVARBINARY:	
// 	case SQL_BIGINT:		
// 	case SQL_TINYINT:		
// 	case SQL_BIT:			
		ASSERT(sValue.Find(' ') == -1);
		break;
	}

	return sQuoted;
}

CString& COdbcHelper::AppendAttributeToList(const CString& sAttrib, CString& sAttribs)
{
	if (!sAttrib.IsEmpty())
	{
		if (sAttribs.IsEmpty())
		{
			sAttribs = sAttrib;
		}
		else
		{
			sAttribs += _T(", ");
			sAttribs += sAttrib;
		}
	}

	return sAttribs;
}

CString& COdbcHelper::AppendAttributeToList(const CString& sAttrib, const CString& sValue, CString& sAttribs)
{
	CString sAppend;

	if (!sAttrib.IsEmpty())
		sAppend.Format(_T("%s = %s"), sAttrib, sValue);

	return AppendAttributeToList(sAppend, sAttribs);
}

void COdbcHelper::InitCombo(COdbcValueComboBox& cbValues, CDatabaseEx& db, 
							const ODBCTABLESETUP& table, BOOL bAddEmptyItem)
{
	ASSERT(cbValues.GetSafeHwnd());

	if (table.Verify())
	{
		cbValues.Initialize(&db, table.sTableName, table.sKeyField, table.sContentField, bAddEmptyItem);
		cbValues.EnableWindow(TRUE);

		if (!bAddEmptyItem)
			cbValues.SetCurSel(0);
	}
	else
	{
		cbValues.ResetContent();
		cbValues.EnableWindow(FALSE);
	}
}

void COdbcHelper::InitCombo(COdbcFieldComboBox& cbFields, CDatabaseEx& db, 
							const ODBCTABLESETUP& table, BOOL bKeyField, BOOL bAddEmptyItem)
{
	if (bKeyField)
		InitCombo(cbFields, db, table.sTableName, table.sKeyField, bAddEmptyItem);
	else
		InitCombo(cbFields, db, table.sTableName, table.sContentField, bAddEmptyItem);
}

void COdbcHelper::InitCombo(COdbcTableComboBox& cbTables, CDatabaseEx& db, 
							const ODBCTABLESETUP& table, BOOL bAddEmptyItem)
{
	InitCombo(cbTables, db, table.sTableName, bAddEmptyItem);
}


void COdbcHelper::InitCombo(COdbcFieldComboBox& cbFields, CDatabaseEx& db, 
							const CString& sTable, const CString& sField, BOOL bAddEmptyItem)
{
	ASSERT(cbFields.GetSafeHwnd());

	if (!sTable.IsEmpty())
	{
		cbFields.Initialize(&db, sTable, sField, bAddEmptyItem);
		cbFields.EnableWindow(TRUE);

		// if the field is not set, choose the first item
		if (sField.IsEmpty())
			cbFields.SetCurSel(0);
	}
	else
	{
		cbFields.ResetContent();
		cbFields.EnableWindow(FALSE);
	}
}

void COdbcHelper::InitCombo(COdbcTableComboBox& cbTables, CDatabaseEx& db, 
							const CString& sTable, BOOL bAddEmptyItem)
{
	ASSERT(cbTables.GetSafeHwnd());

	cbTables.Initialize(&db, sTable, bAddEmptyItem);
	cbTables.EnableWindow(TRUE);

	// if the table is not set, choose the first item
	if (sTable.IsEmpty())
		cbTables.SetCurSel(0);
}

BOOL COdbcHelper::GetTaskAttributeSetup(const ODBCSETUP& setup, TDC_ATTRIBUTE nAttrib, ODBCATTRIBUTESETUP& attrib)
{
	int nAtt = FindTaskAttributeSetup(setup, nAttrib);

	if (nAtt >= 0)
	{
		attrib = setup.aAttrib[nAtt];
		return TRUE;
	}

	// not found
	return FALSE;
}

int COdbcHelper::FindTaskAttributeSetup(const ODBCSETUP& setup, TDC_ATTRIBUTE nAttrib)
{
	int nAtt = setup.aAttrib.GetSize();

	while (nAtt--)
	{
		const ODBCATTRIBUTESETUP& att = setup.aAttrib[nAtt];

		if (att.nAttrib == nAttrib)
			return nAtt;
	}

	// not found
	return -1;
}

