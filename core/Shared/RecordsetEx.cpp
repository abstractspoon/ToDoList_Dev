//By: George Polouse

// RecordsetEx.cpp : implementation file
//

#include "stdafx.h"
#include "RecordsetEx.h"

#include "datehelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define VERIFYFIELDINDEX(index, ret) \
if (!VerifyFieldIndex(index)) {	ASSERT(0); return ret; }

/////////////////////////////////////////////////////////////////////////////
// CRecordsetEx

IMPLEMENT_DYNAMIC(CRecordsetEx, CRecordset)

CRecordsetEx::CRecordsetEx(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRecordsetEx)
	//}}AFX_FIELD_INIT
	
	m_nDefaultType = dynaset;
}

CRecordsetEx::~CRecordsetEx()
{
}

BOOL CRecordsetEx::ExecDirect(const CString& sSQL, UINT nOpenType /* = forwardOnly */)
{
	return Open(nOpenType, sSQL, CRecordset::executeDirect);
}

// Copied verbatim from "VC98\MFC\SRC\DBCORE.CPP" because
// we override AllocAndCacheFieldInfo which is not virtual
BOOL CRecordsetEx::Open(UINT nOpenType, LPCTSTR lpszSQL, DWORD dwOptions)
{
	ASSERT(!IsOpen());
	ASSERT_VALID(this);
	ASSERT(lpszSQL == NULL || AfxIsValidString(lpszSQL));
	ASSERT(nOpenType == AFX_DB_USE_DEFAULT_TYPE ||
		nOpenType == dynaset || nOpenType == snapshot ||
		nOpenType == forwardOnly || nOpenType == dynamic);
	ASSERT(!(dwOptions & readOnly && dwOptions & appendOnly));

	// Can only use optimizeBulkAdd with appendOnly recordsets
	ASSERT((dwOptions & optimizeBulkAdd && dwOptions & appendOnly) ||
		!(dwOptions & optimizeBulkAdd));

	// forwardOnly recordsets have limited functionality
	ASSERT(!(nOpenType == forwardOnly && dwOptions & skipDeletedRecords));

	// Cache state info and allocate hstmt
	SetState(nOpenType, lpszSQL, dwOptions);
	if(!AllocHstmt())
		return FALSE;

	// Check if bookmarks upported (CanBookmark depends on open DB)
	ASSERT(dwOptions & useBookmarks ? CanBookmark() : TRUE);

	TRY
	{
		OnSetOptions(m_hstmt);

		// Allocate the field/param status arrays, if necessary
		BOOL bUnbound = FALSE;
		if (m_nFields > 0 || m_nParams > 0)
			AllocStatusArrays();
		else
			bUnbound = TRUE;

		// Build SQL and prep/execute or just execute direct
		m_strSQL = lpszSQL;
		PrepareAndExecute();
		
		// Cache some field info and prepare the rowset
		AllocAndCacheFieldInfo();
		AllocRowset();

		// If late binding, still need to allocate status arrays
		if (bUnbound && (m_nFields > 0 || m_nParams > 0))
			AllocStatusArrays();

		// Give derived classes a call before binding
		PreBindFields();

		if(m_nResultCols)
			MoveNext();

		// If EOF, then result set empty, so set BOF as well
		m_bBOF = m_bEOF;
	}
	CATCH_ALL(e)
	{
		Close();
		THROW_LAST();
	}
	END_CATCH_ALL

	return TRUE;
}

void CRecordsetEx::AllocAndCacheFieldInfo()
{
	if (m_rgODBCFieldInfos != NULL)
	{
		delete [] m_rgODBCFieldInfos;
		m_rgODBCFieldInfos = NULL;
	}

	CRecordset::AllocAndCacheFieldInfo();
	
/*
	m_nResultCols = -1;
	ASSERT(GetODBCFieldCount() < 0);
	
	RETCODE nRetCode;
	SWORD nActualLen;	   // Cache the number of result columns.
	AFX_ODBC_CALL(::SQLNumResultCols(m_hstmt, &m_nResultCols));
	if(!Check(nRetCode))
	{
		TRACE(_T("Error: Can't get field info.\n"));
		AfxThrowDBException(nRetCode, m_pDatabase,m_hstmt);
	}
	
	// If there are no fields, quit now
	if(m_nResultCols == 0)
		 return;
	  
	// Allocate buffer and get the ODBC meta data.
	m_rgODBCFieldInfos = new CODBCFieldInfo[m_nResultCols];
	
	LPSTR lpszFieldName;

#ifdef _UNICODE
	  // Need proxy to temporarily store non-UNICODE name.
	  lpszFieldName = new char[MAX_FNAME_LEN + 1];
#endif

      // Get the field info for each field.
   for(WORD n = 1; n <= GetODBCFieldCount(); n++)
   {
#ifndef _UNICODE 	// Reset the buffer to point to next element.
		lpszFieldName = m_rgODBCFieldInfos[n - 1].m_strName.GetBuffer(MAX_FNAME_LEN + 1);
#endif

		AFX_ODBC_CALL(::SQLDescribeCol(m_hstmt, n,
			 (UCHAR*)lpszFieldName, MAX_FNAME_LEN, &nActualLen,
			 &m_rgODBCFieldInfos[n - 1].m_nSQLType,
			 &m_rgODBCFieldInfos[n - 1].m_nPrecision,
			 &m_rgODBCFieldInfos[n - 1].m_nScale,
			 &m_rgODBCFieldInfos[n - 1].m_nNullability));
   
#ifndef _UNICODE
		m_rgODBCFieldInfos[n - 1].m_strName.ReleaseBuffer(nActualLen);
#else		 // Copy the proxy data to correct element.
		m_rgODBCFieldInfos[n - 1].m_strName = lpszFieldName;
#endif
		
		if(!Check(nRetCode))
		{
			TRACE1("Error: ODBC failure getting field #%d info.\n", n);
				AfxThrowDBException(nRetCode, m_pDatabase, m_hstmt);
		}
  }

#ifdef _UNICODE
	delete[] lpszFieldName;
#endif
*/
}

// Copied verbatim from "VC98\MFC\SRC\DBCORE.CPP" because
// we override GetLongCharDataAndCleanup which is static
// and we provide extra handling for binary data
BOOL CRecordsetEx::GetFieldValue(short nIndex, CString& strValue)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	
	VERIFYFIELDINDEX(nIndex, FALSE);
	
	// Convert index to 1-based and check range
	nIndex++;
	if (nIndex < 1 || nIndex > GetODBCFieldCount())
	{
		ThrowDBException(AFX_SQL_ERROR_FIELD_NOT_FOUND);
	}
	
	int nLen = GetTextLen(m_rgODBCFieldInfos[nIndex - 1].m_nSQLType,
		m_rgODBCFieldInfos[nIndex - 1].m_nPrecision);

#ifndef _UNICODE
	CString& strData = strValue;
#else
	CString strProxy;
	CString& strData = strProxy;
#endif

	void* pvData = strData.GetBufferSetLength(nLen);
	
	// Now can actually get the data
	long nActualSize = GetData(m_pDatabase, m_hstmt, nIndex,
		SQL_C_CHAR, pvData, nLen,
		m_rgODBCFieldInfos[nIndex - 1].m_nSQLType);
	
	// Handle NULL data separately
	if (nActualSize == SQL_NULL_DATA)
	{
		// Clear value
		strValue.Empty();
	}
	else
	{
		// May need to cleanup and call SQLGetData again if necessary
		GetLongCharDataAndCleanup(m_pDatabase, m_hstmt, nIndex,
			nActualSize, &pvData, nLen, strData,
			m_rgODBCFieldInfos[nIndex - 1].m_nSQLType);
		
#ifdef _UNICODE
		// Now must convert string to UNICODE
		strValue = (LPCSTR)strData.GetBuffer(0);
#endif // _UNIOCDE
		
		if (!strValue.IsEmpty())
		{
			switch (m_rgODBCFieldInfos[nIndex - 1].m_nSQLType)
			{
			case SQL_BINARY:
			case SQL_LONGVARBINARY:
			case SQL_VARBINARY:
				strValue = _T("0x") + strValue;
				break;
			}
		}
	}

	return TRUE;
}

BOOL CRecordsetEx::GetFieldValue(short nIndex, double& dVal)
{
	CDBVariant varResult;

	if (!GetFieldValue(nIndex, varResult))
		return FALSE;

	switch (varResult.m_dwType)
	{
	case DBVT_DOUBLE:
		dVal = varResult.m_dblVal;
		break;
		
	case DBVT_SINGLE:
		dVal = varResult.m_fltVal;
		break;
		
	case DBVT_LONG:
		dVal = varResult.m_lVal;
		break;
		
	case DBVT_SHORT:
		dVal = varResult.m_iVal;
		break;
		
	default: // all else
		return FALSE;
	}
	
	return TRUE;
}

BOOL CRecordsetEx::GetFieldValue(short nIndex, long& nVal)
{
	CDBVariant varResult;

	if (!GetFieldValue(nIndex, varResult))
		return FALSE;

	switch (varResult.m_dwType)
	{
	case DBVT_DOUBLE:
		nVal = (long)varResult.m_dblVal;
		break;
		
	case DBVT_SINGLE:
		nVal = (long)varResult.m_fltVal;
		break;
		
	case DBVT_LONG:
		nVal = varResult.m_lVal;
		break;
		
	case DBVT_SHORT:
		nVal = varResult.m_iVal;
		break;
		
	default: // all else
		return FALSE;
	}

	return TRUE;
}

BOOL CRecordsetEx::GetFieldValue(short nIndex, COleDateTime& dtVal)
{
	CDBVariant varResult;

	if (!GetFieldValue(nIndex, varResult))
		return FALSE;

	switch (varResult.m_dwType)
	{
	case DBVT_DATE:
		dtVal.SetDateTime(varResult.m_pdate->year,
			varResult.m_pdate->month,
			varResult.m_pdate->day,
			varResult.m_pdate->hour,
			varResult.m_pdate->minute,
			varResult.m_pdate->second);
		break;
		
	case DBVT_SINGLE:
		dtVal = COleDateTime(varResult.m_fltVal);
		break;
		
	case DBVT_DOUBLE:
		dtVal = COleDateTime(varResult.m_dblVal);
		break;
		
	default: // all else
		return FALSE;
	}

	return (dtVal.GetStatus() == COleDateTime::valid);
}

BOOL CRecordsetEx::GetFieldValue(short nField, time64_t& tVal)
{
	COleDateTime date;

	return (GetFieldValue(nField, date) && 
			CDateHelper::GetTimeT64(date, tVal));
}

short CRecordsetEx::GetFieldIndexByName(const CString& sName, BOOL bCaseSensitive)
{
	if (bCaseSensitive)
		return CRecordset::GetFieldIndexByName(sName);

	// else
	short nIndex;
	for (nIndex = 0; nIndex < GetODBCFieldCount(); nIndex++)
	{
		if (m_rgODBCFieldInfos[nIndex].m_strName.CompareNoCase(sName) == 0)
			break;
	}

	// Check if field name found
	if (nIndex == GetODBCFieldCount())
		ThrowDBException(AFX_SQL_ERROR_FIELD_NOT_FOUND);

	return nIndex;

}

BOOL CRecordsetEx::GetFieldValue(const CString& sName, CDBVariant& varValue, short nFieldType)
{
	// Get the index of the field corresponding to name
	short nIndex = GetFieldIndexByName(sName);

	return GetFieldValue(nIndex, varValue, nFieldType);
}

BOOL CRecordsetEx::GetFieldValue(short nIndex, CDBVariant& varValue, short nFieldType)
{
	VERIFYFIELDINDEX(nIndex, FALSE);

	CRecordset::GetFieldValue(nIndex, varValue, nFieldType);

	return (varValue.m_dwType != DBVT_NULL);
}

CString CRecordsetEx::GetFieldValue(short nIndex)
{
	VERIFYFIELDINDEX(nIndex, _T(""));

	CString sValue;
	GetFieldValue(nIndex, sValue);

	return sValue;
}

BOOL CRecordsetEx::GetFieldValue(const CString& sName, CString& strValue)
{
	ASSERT(!sName.IsEmpty());

	short nIndex = GetFieldIndexByName(sName);
	VERIFYFIELDINDEX(nIndex, FALSE);

	strValue = GetFieldValue(nIndex);
	return TRUE;
}

CString CRecordsetEx::GetFieldValue(const CString& sName)
{
	ASSERT(!sName.IsEmpty());

	// Get the index of the field corresponding to name
	short nIndex = GetFieldIndexByName(sName);
	VERIFYFIELDINDEX(nIndex, _T(""));

	return GetFieldValue(nIndex);
}

CString CRecordsetEx::GetSQLDataType(SWORD nSQLType)
{
	static TCHAR szType[10] = _T("");

	switch (nSQLType)
	{
	case SQL_UNKNOWN_TYPE:	return _T("SQL_UNKNOWN_TYPE");
	case SQL_CHAR:			return _T("SQL_CHAR");        
	case SQL_NUMERIC:		return _T("SQL_NUMERIC");     
	case SQL_DECIMAL:		return _T("SQL_DECIMAL");     
	case SQL_INTEGER:		return _T("SQL_INTEGER");     
	case SQL_SMALLINT:		return _T("SQL_SMALLINT");    
	case SQL_FLOAT:			return _T("SQL_FLOAT");       
	case SQL_REAL:			return _T("SQL_REAL");        
	case SQL_DOUBLE:		return _T("SQL_DOUBLE");      
	case SQL_DATE:			return _T("SQL_DATE");    
	case SQL_VARCHAR:		return _T("SQL_VARCHAR");     
	case SQL_WCHAR:		 	return _T("SQL_WCHAR");     
	case SQL_WVARCHAR:	 	return _T("SQL_WVARCHAR");     
	case SQL_WLONGVARCHAR: 	return _T("SQL_WLONGVARCHAR");     
	case SQL_TIME:			return _T("SQL_TIME");
	case SQL_TIMESTAMP:		return _T("SQL_TIMESTAMP");
	case SQL_LONGVARCHAR:	return _T("SQL_LONGVARCHAR");
	case SQL_BINARY:		return _T("SQL_BINARY");
	case SQL_VARBINARY:		return _T("SQL_VARBINARY");
	case SQL_LONGVARBINARY:	return _T("SQL_LONGVARBINARY");
	case SQL_BIGINT:		return _T("SQL_BIGINT");
	case SQL_TINYINT:		return _T("SQL_TINYINT");
	case SQL_BIT:			return _T("SQL_BIT");
	}

	// all else
	_stprintf(szType, _T("%d"), nSQLType);
	return szType;
}

BOOL CRecordsetEx::VerifyFieldIndex(short nIndex) const
{
	return ((GetODBCFieldCount() > 0) && (nIndex < GetODBCFieldCount()));
}

int CRecordsetEx::GetAllFieldInfo(CODBCFieldInfoArray& aInfo) const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	int nField = GetODBCFieldCount();
	
	aInfo.RemoveAll();
	aInfo.SetSize(nField, 1);

	while (nField--)
		aInfo[nField] = m_rgODBCFieldInfos[nField];

	return aInfo.GetSize();
}

CString CRecordsetEx::GetFieldName(short nIndex) const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	VERIFYFIELDINDEX(nIndex, _T(""));

	CODBCFieldInfo* pInfo = &m_rgODBCFieldInfos[nIndex];
	return pInfo->m_strName;
}

SWORD CRecordsetEx::GetFieldType(short nIndex) const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	VERIFYFIELDINDEX(nIndex, 0);

	CODBCFieldInfo* pInfo = &m_rgODBCFieldInfos[nIndex];
	return pInfo->m_nSQLType;
}

UDWORD CRecordsetEx::GetFieldPrecision(short nIndex) const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	VERIFYFIELDINDEX(nIndex, 0);

	CODBCFieldInfo* pInfo = &m_rgODBCFieldInfos[nIndex];
	return pInfo->m_nPrecision;
}

SWORD CRecordsetEx::GetFieldScale(short nIndex) const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	VERIFYFIELDINDEX(nIndex, 0);

	CODBCFieldInfo* pInfo = &m_rgODBCFieldInfos[nIndex];
	return pInfo->m_nScale;
}

SWORD CRecordsetEx::GetFieldNullability(short nIndex) const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	VERIFYFIELDINDEX(nIndex, 0);

	CODBCFieldInfo* pInfo = &m_rgODBCFieldInfos[nIndex];
	return pInfo->m_nNullability;
}

int PASCAL CRecordsetEx::GetTextLen(short nSQLType, UDWORD nPrecision)
{
	// handle new types
	if (nSQLType == SQL_WLONGVARCHAR || nSQLType == SQL_UNICODE_LONGVARCHAR)
	{
		// Use a dummy length of 1 (will just get NULL terminator)
		return 1;
	}

	// else
	return CRecordset::GetTextLen(nSQLType, nPrecision);
}

void PASCAL CRecordsetEx::GetLongCharDataAndCleanup(CDatabase* pdb,
	HSTMT hstmt, short nFieldIndex, long nActualSize, LPVOID* ppvData,
	int nLen, CString& strValue, short nSQLType)
{
	// handle new types
	if (nSQLType == SQL_WLONGVARCHAR || nSQLType == SQL_UNICODE_LONGVARCHAR)
	{
		nSQLType = SQL_LONGVARCHAR;
	}

	CRecordset::GetLongCharDataAndCleanup(pdb, hstmt, nFieldIndex, nActualSize, ppvData, nLen, strValue, nSQLType);

/*
	RETCODE nRetCode;

	// Release the buffer now that data has been fetched
	strValue.ReleaseBuffer(nActualSize < nLen ? nActualSize : nLen);

	// If long data, may need to call SQLGetData again
	if (nLen <= nActualSize && 
		(nSQLType == SQL_LONGVARCHAR ||
		 nSQLType == SQL_LONGVARBINARY || 
		 nSQLType == SQL_WLONGVARCHAR ||
		 nSQLType == SQL_UNICODE_LONGVARCHAR))
	{
		// Reallocate the size (this will copy the data)
		*ppvData = strValue.GetBufferSetLength(nActualSize + 1);

		// Get pointer, skipping over original data, but not the NULL
		int nOldLen = nLen - 1;
		*ppvData = (BYTE*)*ppvData + nOldLen;
		nLen = nActualSize + 1 - nOldLen;

		// Retrieve the column in question
		AFX_ODBC_CALL(::SQLGetData(hstmt, nFieldIndex,
			SQL_C_CHAR, *ppvData, nLen, &nActualSize));

		if (nRetCode == SQL_SUCCESS_WITH_INFO)
		{
#ifdef _DEBUG
			if (afxTraceFlags & traceDatabase)
			{
				TRACE1("Warning: ODBC Success With Info on field %d.\n",
					nFieldIndex - 1);
				CDBException e(nRetCode);
				e.BuildErrorString(pdb, hstmt);
			}
#endif // _DEBUG
		}
		else if (nRetCode != SQL_SUCCESS)
		{
			TRACE1("Error: GetFieldValue operation failed on field %d.\n",
				nFieldIndex - 1);
			AfxThrowDBException(nRetCode, pdb, hstmt);
		}

		// Release the buffer now that data has been fetched
		strValue.ReleaseBuffer(nActualSize + nOldLen);
	}
*/
}

BOOL CRecordsetEx::PrepareQuery(LPCTSTR szTable, LPCTSTR szField1, 
								LPCTSTR szField2, LPCTSTR szWhere,
								CString& sQuery)
{
	ASSERT(szTable && *szTable);
	ASSERT(szField1 && *szField1);
	ASSERT(szField2 && *szField2);

	if (!szTable || !(*szTable) || 
		!szField1 || !(*szField1) || 
		!szField2 || !(*szField2))
	{
		return FALSE;
	}

	// note trailing space in anticipation of a 'where' clause
	sQuery.Format(_T("SELECT %s, %s FROM %s "), szField1, szField2, szTable);
	
	// validate and add 'where' clause
	CString sWhere(szWhere);
	sWhere.MakeUpper();
	sWhere.TrimLeft();
	
	if (!sWhere.IsEmpty())
	{
		if (sWhere.Find(_T("WHERE")) == -1)
			sQuery += _T("WHERE "); // trailing space
		
		sQuery += szWhere;
	}

	return TRUE;
}

int CRecordsetEx::BuildFieldMapping(CDatabase& db, LPCTSTR szTable, 
									LPCTSTR szField1, LPCTSTR szField2,
									CMapStringToString& mapFields, 
									LPCTSTR szWhere)
{
	mapFields.RemoveAll();

	CRecordsetEx results(&db);
	CString sQuery;
	
	if (!PrepareQuery(szTable, szField1, szField2, szWhere, sQuery))
		return 0;

 	if (results.ExecDirect(sQuery) && !results.IsBOF())
	{
		ASSERT(results.GetODBCFieldCount() == 2);

		while (!results.IsEOF())
		{
			CString sValue1 = results.GetFieldValue(0);
			CString sValue2 = results.GetFieldValue(1);

			mapFields[sValue1] = sValue2;
			
			results.MoveNext();			
		}
	}
	
	return mapFields.GetCount();
}

int CRecordsetEx::BuildFieldMapping(CDatabase& db, LPCTSTR szTable, 
									LPCTSTR szField1, LPCTSTR szField2,
									CMapStringToStringArray& mapFields, 
									LPCTSTR szWhere)
{
	mapFields.RemoveAll();

	CRecordsetEx results(&db);
	CString sQuery;
	
	if (!PrepareQuery(szTable, szField1, szField2, szWhere, sQuery))
		return 0;

 	if (results.ExecDirect(sQuery) && !results.IsBOF())
	{
		ASSERT(results.GetODBCFieldCount() == 2);

		while (!results.IsEOF())
		{
			CString sValue1 = results.GetFieldValue(0);
			CString sValue2 = results.GetFieldValue(1);

			CStringArray* pMapping = mapFields.GetAddMapping(sValue1);
			ASSERT(pMapping);

			pMapping->Add(sValue2);

			results.MoveNext();			
		}
	}
	
	return mapFields.GetCount();

}

int CRecordsetEx::BuildFieldMapping(CDatabase& db, LPCTSTR szTable, 
									LPCTSTR szIDField, LPCTSTR szField2,
									CMapIDToString& mapFields, 
									LPCTSTR szWhere)
{
	mapFields.RemoveAll();

	CRecordsetEx results(&db);
	CString sQuery;
	
	if (!PrepareQuery(szTable, szIDField, szField2, szWhere, sQuery))
		return 0;

 	if (results.ExecDirect(sQuery) && !results.IsBOF())
	{
		ASSERT(results.GetODBCFieldCount() == 2);
		ASSERT(results.GetFieldType(0) == SQL_INTEGER);

		if (results.GetFieldType(0) == SQL_INTEGER)
		{
			while (!results.IsEOF())
			{
				CDBVariant varResult;

				if (!results.GetFieldValue(0, varResult) || (varResult.m_dwType != DBVT_LONG))
				{
					mapFields.RemoveAll();
					break;
				}

				CString sValue2 = results.GetFieldValue(1);
				mapFields[varResult.m_lVal] = sValue2;
				
				results.MoveNext();			
			}
		}
	}
	
	return mapFields.GetCount();
}

int CRecordsetEx::BuildFieldMapping(CDatabase& db, LPCTSTR szTable, 
									LPCTSTR szField1, LPCTSTR szIDField, 
									CMapStringToID& mapFields, 
									LPCTSTR szWhere)
{
	mapFields.RemoveAll();

	CRecordsetEx results(&db);
	CString sQuery;
	
	if (!PrepareQuery(szTable, szField1, szIDField, szWhere, sQuery))
		return 0;

 	if (results.ExecDirect(sQuery) && !results.IsBOF())
	{
		ASSERT(results.GetODBCFieldCount() == 2);
		ASSERT(results.GetFieldType(1) == SQL_INTEGER);

		if (results.GetFieldType(1) == SQL_INTEGER)
		{
			while (!results.IsEOF())
			{
				CDBVariant varResult;

				if (!results.GetFieldValue(1, varResult) || (varResult.m_dwType != DBVT_LONG))
				{
					mapFields.RemoveAll();
					break;
				}

				CString sValue1 = results.GetFieldValue(0);
				mapFields[sValue1] = varResult.m_lVal;
				
				results.MoveNext();			
			}
		}
	}
	
	return mapFields.GetCount();
}

/////////////////////////////////////////////////////////////////////////////
// CRecordsetEx diagnostics

#ifdef _DEBUG
void CRecordsetEx::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRecordsetEx::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG