//By: George Polouse

// Columns.cpp: implementation of the CColumns class.
//

#include "stdafx.h"
#include "Columns.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CColumns

IMPLEMENT_DYNAMIC(CColumns, CRecordsetEx)

CColumns::CColumns(CDatabase* pDatabase)
	: CRecordsetEx(pDatabase)
{
	m_strTableQualifier = _T("");
	m_strTableOwner     = _T("");
	m_strTableName      = _T("");
	m_strColumnName     = _T("");
	m_nDataType         = 0;
	m_strTypeName       = _T("");
	m_nPrecision        = 0;
	m_nLength           = 0;
	m_nScale            = 0;
	m_nRadix            = 0;
	m_fNullable         = 0;
	m_strRemarks        = _T("");
	m_nFields = 12;
}

BOOL CColumns::Open(LPCSTR pszTableQualifier,
	LPCSTR pszTableOwner,LPCSTR pszTableName,LPCSTR pszColumnName,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLColumns is supported
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
			(UCHAR FAR*)pszTableQualifier,SQL_NTS,
			(UCHAR FAR*)pszTableOwner,SQL_NTS,
			(UCHAR FAR*)pszTableName,SQL_NTS,
			(UCHAR FAR*)pszColumnName,SQL_NTS));
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

void CColumns::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("TABLE_QUALIFIER"),m_strTableQualifier);
	RFX_Text(pFX,_T("TABLE_OWNER"),m_strTableOwner);
	RFX_Text(pFX,_T("TABLE_NAME"),m_strTableName);
	RFX_Text(pFX,_T("COLUMN_NAME"),m_strColumnName);
	RFX_Int(pFX,_T("DATA_TYPE"),m_nDataType);
	RFX_Text(pFX,_T("TYPE_NAME"),m_strTypeName);
	RFX_Long(pFX,_T("PRECISION"),m_nPrecision);
	RFX_Long(pFX,_T("LENGTH"),m_nLength);
	RFX_Int(pFX,_T("SCALE"),m_nScale);
	RFX_Int(pFX,_T("RADIX"),m_nRadix);
	RFX_Int(pFX,_T("NULLABLE"),m_fNullable);
	RFX_Text(pFX,_T("REMARKS"),m_strRemarks);
}

CString CColumns::GetDefaultConnect()
{ 
	return _T("ODBC;");
}

CString CColumns::GetDefaultSQL()
{ 
	return "!"; // Direct ODBC call
}

/////////////////////////////////////////////////////////////////////////////
// CColumns diagnostics

#ifdef _DEBUG
void CColumns::AssertValid() const
{
	CRecordsetEx::AssertValid();
}

void CColumns::Dump(CDumpContext& dc) const
{
	CRecordsetEx::Dump(dc);
}
#endif //_DEBUG