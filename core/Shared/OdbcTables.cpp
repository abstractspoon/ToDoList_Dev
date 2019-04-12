//By: George Polouse

// Tables.cpp: implementation of the COdbcTables class.
//

#include "stdafx.h"
#include "odbcTables.h"

#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

IMPLEMENT_DYNAMIC(COdbcTables, CRecordsetEx)

COdbcTables::COdbcTables(CDatabase* pDatabase) : CRecordsetEx(pDatabase)
{
}

BOOL COdbcTables::Open(LPCTSTR pszTableQualifier,
	LPCTSTR pszTableOwner, LPCTSTR pszTableName ,LPCTSTR pszTableType,
	UINT nOpenType)
{
	USES_CONVERSION;

	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLTables exists
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLTABLES,&bFunctionExists));

	if (!Check(nRetCode))
		AfxThrowDBException(nRetCode, m_pDatabase, m_hstmt);

	if (!bFunctionExists)
		throw _T("<::SQLTables> not supported.");

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);

	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
#if _MSC_VER >= 1400
		AFX_ODBC_CALL(::SQLTables(m_hstmt,
			(SQLWCHAR*)T2W((LPTSTR)pszTableQualifier),SQL_NTS,
			(SQLWCHAR*)T2W((LPTSTR)pszTableOwner),SQL_NTS,
			(SQLWCHAR*)T2W((LPTSTR)pszTableName),SQL_NTS,
			(SQLWCHAR*)T2W((LPTSTR)pszTableType),SQL_NTS));
#else
		AFX_ODBC_CALL(::SQLTables(m_hstmt,
			(SQLCHAR*)T2A((LPTSTR)pszTableQualifier),SQL_NTS,
			(SQLCHAR*)T2A((LPTSTR)pszTableOwner),SQL_NTS,
			(SQLCHAR*)T2A((LPTSTR)pszTableName),SQL_NTS,
			(SQLCHAR*)T2A((LPTSTR)pszTableType),SQL_NTS));
#endif

		if (!Check(nRetCode))
			ThrowDBException(nRetCode,m_hstmt);

		// Allocate memory and cache info
		AllocAndCacheFieldInfo();
		AllocRowset();

		// Fetch the first row of data
		MoveNext();

		// If EOF, result set is empty, set BOF as well
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

int COdbcTables::GetTableNames(CStringArray& aTableNames)
{
	aTableNames.RemoveAll();

	ASSERT(IsOpen());

 	if (IsOpen() && !IsBOF())
	{
		while (!IsEOF())
		{
			aTableNames.Add(GetTableName());
			MoveNext();			
		}
	}

	return aTableNames.GetSize();
}

// static version
int COdbcTables::GetTableNames(CDatabase& db, CStringArray& aTableNames)
{
	COdbcTables tables(&db);

	if (tables.Open())
		return tables.GetTableNames(aTableNames);

	// else
	ASSERT(0);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// COdbcTables diagnostics

#ifdef _DEBUG
void COdbcTables::AssertValid() const
{
	CRecordsetEx::AssertValid();
}

void COdbcTables::Dump(CDumpContext& dc) const
{
	CRecordsetEx::Dump(dc);
}
#endif //_DEBUG