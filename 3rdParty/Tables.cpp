//By: George Polouse

// Tables.cpp: implementation of the CTables class.
//

#include "stdafx.h"
#include "Tables.h"

#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

IMPLEMENT_DYNAMIC(CTables, CRecordsetEx)

CTables::CTables(CDatabase* pDatabase) : CRecordsetEx(pDatabase)
{
}

BOOL CTables::Open(LPCTSTR pszTableQualifier,
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
		AFX_ODBC_CALL(::SQLTables(m_hstmt,
			(UCHAR*)T2A((LPTSTR)pszTableQualifier),SQL_NTS,
			(UCHAR*)T2A((LPTSTR)pszTableOwner),SQL_NTS,
			(UCHAR*)T2A((LPTSTR)pszTableName),SQL_NTS,
			(UCHAR*)T2A((LPTSTR)pszTableType),SQL_NTS));

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

int CTables::GetTableNames(CStringArray& aTableNames)
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
int CTables::GetTableNames(CDatabase& db, CStringArray& aTableNames)
{
	CTables tables(&db);

	if (tables.Open())
		return tables.GetTableNames(aTableNames);

	// else
	ASSERT(0);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTables diagnostics

#ifdef _DEBUG
void CTables::AssertValid() const
{
	CRecordsetEx::AssertValid();
}

void CTables::Dump(CDumpContext& dc) const
{
	CRecordsetEx::Dump(dc);
}
#endif //_DEBUG