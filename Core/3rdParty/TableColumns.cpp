//By: George Polouse

// Tables.cpp: implementation of the CTableFields class.
//

#include "stdafx.h"
#include "TableColumns.h"

#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

IMPLEMENT_DYNAMIC(CTableFields, CRecordsetEx)

CTableFields::CTableFields(CDatabase* pDatabase) : CRecordsetEx(pDatabase)
{
	m_nFields = 5;
}

BOOL CTableFields::Open(LPCTSTR pszTableName, UINT nOpenType)
{
	USES_CONVERSION;

	RETCODE nRetCode;
	UWORD   bFunctionExists;

	ASSERT(pszTableName && *pszTableName);

	if ((pszTableName == NULL) || (pszTableName[0] == 0))
		return FALSE;

	// Make sure SQLTables exists
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLCOLUMNS,&bFunctionExists));

	if(!Check(nRetCode))
		AfxThrowDBException(nRetCode, m_pDatabase, m_hstmt);

	if(!bFunctionExists)
		throw _T("<::SQLColumns> not supported.");

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);

	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
		AFX_ODBC_CALL(::SQLColumns(m_hstmt,
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

void CTableFields::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Text(pFX,_T("TABLE_QUALIFIER"),m_strTableQualifier);
	RFX_Text(pFX,_T("TABLE_OWNER"),m_strTableOwner);
	RFX_Text(pFX,_T("TABLE_NAME"),m_strTableName);
	RFX_Text(pFX,_T("TABLE_TYPE"),m_strTableType);
	RFX_Text(pFX,_T("REMARKS"),m_strRemarks);
}

CString CTableFields::GetDefaultConnect()
{ 
	return _T("ODBC;");
}

CString CTableFields::GetDefaultSQL()
{ 
	return "!"; // Direct ODBC call
}

/////////////////////////////////////////////////////////////////////////////
// CTableFields diagnostics

#ifdef _DEBUG
void CTableFields::AssertValid() const
{
	CRecordsetEx::AssertValid();
}

void CTableFields::Dump(CDumpContext& dc) const
{
	CRecordsetEx::Dump(dc);
}
#endif //_DEBUG