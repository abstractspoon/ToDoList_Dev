// catsets.cpp :
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "catsets.h"
#include <conio.h>
//---------------------------------------------------------------------
// CColumnPrivileges

CColumnPrivileges::CColumnPrivileges(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_strTableQualifier = _T("");
	m_strTableOwner     = _T("");
	m_strTableName      = _T("");
	m_strColumnName     = _T("");
	m_strGrantor        = _T("");
	m_strGrantee        = _T("");
	m_strPrivilege      = _T("");
	m_strIsGrantable    = _T("");
	m_nFields = 8;
}

BOOL CColumnPrivileges::Open(LPCSTR pszTableQualifier,
	LPCSTR pszTableOwner,LPCSTR pszTableName,LPCSTR pszColumnName,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLColumnPrivileges is supported
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLCOLUMNPRIVILEGES,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLColumnPrivileges not supported\n"));
		return FALSE;
	}

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
		AFX_ODBC_CALL(::SQLColumnPrivileges(m_hstmt,
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

void CColumnPrivileges::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("TABLE_QUALIFIER"),m_strTableQualifier);
	RFX_Text(pFX,_T("TABLE_OWNER"),m_strTableOwner);
	RFX_Text(pFX,_T("TABLE_NAME"),m_strTableName);
	RFX_Text(pFX,_T("COLUMN_NAME"),m_strColumnName);
	RFX_Text(pFX,_T("GRANTOR"),m_strGrantor);
	RFX_Text(pFX,_T("GRANTEE"),m_strGrantee);
	RFX_Text(pFX,_T("PRIVILEGE"),m_strPrivilege);
	RFX_Text(pFX,_T("IS_GRANTABLE"),m_strIsGrantable);
}

//---------------------------------------------------------------------
// CColumns

CColumns::CColumns(CDatabase* pDatabase)
	: CRecordset(pDatabase)
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
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLColumns not supported\n"));
		return FALSE;
	}

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

//---------------------------------------------------------------------
// CForeignKeys

CForeignKeys::CForeignKeys(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{

	m_strPkTableQualifier   = _T("");
	m_strPkTableOwner       = _T("");
	m_strPkTableName        = _T("");
	m_strPkColumnName       = _T("");
	m_strFkTableQualifier   = _T("");
	m_strFkTableOwner       = _T("");
	m_strFkTableName        = _T("");
	m_strFkColumnName       = _T("");
	m_nKeySeq               = 0;
	m_fUpdateRule           = 0;
	m_fDeleteRule           = 0;
	m_strFkName             = _T("");
	m_strPkName             = _T("");
	m_nFields = 13;
}

BOOL CForeignKeys::Open(LPCSTR pszPkTableQualifier,
	LPCSTR pszPkTableOwner,LPCSTR pszPkTableName,LPCSTR pszFkTableQualifier,
	LPCSTR pszFkTableOwner,LPCSTR pszFkTableName,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLForeignKeys is supported
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLFOREIGNKEYS,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLForeignKeys not supported\n"));
		return FALSE;
	}

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
		AFX_ODBC_CALL(::SQLForeignKeys(m_hstmt,
			(UCHAR FAR*)pszPkTableQualifier,SQL_NTS,
			(UCHAR FAR*)pszPkTableOwner,SQL_NTS,
			(UCHAR FAR*)pszPkTableName,SQL_NTS,
			(UCHAR FAR*)pszFkTableQualifier,SQL_NTS,
			(UCHAR FAR*)pszFkTableOwner,SQL_NTS,
			(UCHAR FAR*)pszFkTableName,SQL_NTS));
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

void CForeignKeys::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("PKTABLE_QUALIFIER"),m_strPkTableQualifier);
	RFX_Text(pFX,_T("PKTABLE_OWNER"),m_strPkTableOwner);
	RFX_Text(pFX,_T("PKTABLE_NAME"),m_strPkTableName);
	RFX_Text(pFX,_T("PKCOLUMN_NAME"),m_strPkColumnName);
	RFX_Text(pFX,_T("FKTABLE_QUALIFIER"),m_strFkTableQualifier);
	RFX_Text(pFX,_T("FKTABLE_OWNER"),m_strFkTableOwner);
	RFX_Text(pFX,_T("FKTABLE_NAME"),m_strFkTableName);
	RFX_Text(pFX,_T("FKCOLUMN_NAME"),m_strFkColumnName);
	RFX_Int(pFX,_T("KEY_SEQ"),m_nKeySeq);
	RFX_Int(pFX,_T("UPDATE_RULE"),m_fUpdateRule);
	RFX_Int(pFX,_T("DELETE_RULE"),m_fDeleteRule);
	RFX_Text(pFX,_T("FK_NAME"),m_strFkName);
	RFX_Text(pFX,_T("PK_NAME"),m_strPkName);
}

//---------------------------------------------------------------------
// CGetTypeInfo

CGetTypeInfo::CGetTypeInfo(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_strTypeName           = _T("");
	m_fDataType             = 0;
	m_nPrecision            = 0;
	m_strLiteralPrefix      = _T("");
	m_strLiteralSuffix      = _T("");
	m_strCreateParams       = _T("");
	m_fNullable             = 0;
	m_bCaseSensitive        = 0;
	m_fSearchable           = 0;
	m_fUnsignedAttribute    = 0;
	m_bMoney                = 0;
	m_fAutoIncrement        = 0;
	m_strLocalTypeName      = _T("");
	m_nMinimumScale         = 0;
	m_nMaximumScale         = 0;
	m_nFields = 15;
}

BOOL CGetTypeInfo::Open(short fSqlType,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLGetTypeInfo exists
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLGETTYPEINFO,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLGetTypeInfo not supported\n"));
		return FALSE;
	}

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
		AFX_ODBC_CALL(::SQLGetTypeInfo(m_hstmt,fSqlType));
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

void CGetTypeInfo::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("TYPE_NAME"),m_strTypeName);
	RFX_Int(pFX,_T("DATA_TYPE"),m_fDataType);
	RFX_Long(pFX,_T("PRECISION"),m_nPrecision);
	RFX_Text(pFX,_T("LITERAL_PREFIX"),m_strLiteralPrefix);
	RFX_Text(pFX,_T("LITERAL_SUFFIX"),m_strLiteralSuffix);
	RFX_Text(pFX,_T("CREATE_PARAMS"),m_strCreateParams);
	RFX_Int(pFX,_T("NULLABLE"),m_fNullable);
	RFX_Bool(pFX,_T("CASE_SENSITIVE"),m_bCaseSensitive);
	RFX_Int(pFX,_T("SEARCHABLE"),m_fSearchable);
	RFX_Int(pFX,_T("UNSIGNED_ATTRIBUTE"),m_fUnsignedAttribute);
	RFX_Bool(pFX,_T("MONEY"),m_bMoney);
	RFX_Int(pFX,_T("AUTO_INCREMENT"),m_fAutoIncrement);
	RFX_Text(pFX,_T("LOCAL_TYPE_NAME"),m_strLocalTypeName);
	RFX_Int(pFX,_T("MINIMUM_SCALE"),m_nMinimumScale);
	RFX_Int(pFX,_T("MAXIMUM_SCALE"),m_nMaximumScale);
}

//---------------------------------------------------------------------
// CPrimaryKeys

CPrimaryKeys::CPrimaryKeys(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_strTableQualifier = _T("");
	m_strTableOwner     = _T("");
	m_strTableName      = _T("");
	m_strColumnName     = _T("");
	m_nKeySeq           = 0;
	m_strPkName         = _T("");
	m_nFields = 6;
}

BOOL CPrimaryKeys::Open(LPCSTR pszTableQualifier,
	LPCSTR pszTableOwner,LPCSTR pszTableName,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLPrimaryKeys exists
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLPRIMARYKEYS,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLPrimaryKeys not supported\n"));
		return FALSE;
	}

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
		AFX_ODBC_CALL(::SQLPrimaryKeys(m_hstmt,
			(UCHAR FAR*)pszTableQualifier,SQL_NTS,
			(UCHAR FAR*)pszTableOwner,SQL_NTS,
			(UCHAR FAR*)pszTableName,SQL_NTS));
		if (!Check(nRetCode))
			ThrowDBException(nRetCode,m_hstmt);

		// Allocate memory and cache info
		AllocAndCacheFieldInfo();
		AllocRowset();

		// Fetch the first row of data
		MoveNext();

		// if EOF, result set is empty, set BOF as well
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

void CPrimaryKeys::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("TABLE_QUALIFIER"),m_strTableQualifier);
	RFX_Text(pFX,_T("TABLE_OWNER"),m_strTableOwner);
	RFX_Text(pFX,_T("TABLE_NAME"),m_strTableName);
	RFX_Text(pFX,_T("COLUMN_NAME"),m_strColumnName);
	RFX_Int(pFX,_T("KEY_SEQ"),m_nKeySeq);
	RFX_Text(pFX,_T("PK_NAME"),m_strPkName);
}

//---------------------------------------------------------------------
// CProcedureColumns

CProcedureColumns::CProcedureColumns(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_strProcedureQualifier = _T("");
	m_strProcedureOwner     = _T("");
	m_strProcedureName      = _T("");
	m_strColumnName         = _T("");
	m_fColumnType           = 0;
	m_nDataType             = 0;
	m_strTypeName           = _T("");
	m_nPrecision            = 0;
	m_nLength               = 0;
	m_nScale                = 0;
	m_nRadix                = 0;
	m_fNullable             = 0;
	m_strRemarks            = _T("");
	m_nFields = 13;
}

BOOL CProcedureColumns::Open(LPCSTR pszProcQualifier,
	LPCSTR pszProcOwner,LPCSTR pszProcName,LPCSTR pszColumnName,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLProcedureColumns exists
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLPROCEDURECOLUMNS,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLProcedureColumns not supported\n"));
		return FALSE;
	}

	// Cache stat info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
		AFX_ODBC_CALL(::SQLProcedureColumns(m_hstmt,
			(UCHAR FAR*)pszProcQualifier,SQL_NTS,
			(UCHAR FAR*)pszProcOwner,SQL_NTS,
			(UCHAR FAR*)pszProcName,SQL_NTS,
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

void CProcedureColumns::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("PROCEDURE_QUALIFIER"),m_strProcedureQualifier);
	RFX_Text(pFX,_T("PROCEDURE_OWNER"),m_strProcedureOwner);
	RFX_Text(pFX,_T("PROCEDURE_NAME"),m_strProcedureName);
	RFX_Text(pFX,_T("COLUMN_NAME"),m_strColumnName);
	RFX_Int(pFX,_T("COLUMN_TYPE"),m_fColumnType);
	RFX_Int(pFX,_T("DATA_TYPE"),m_nDataType);
	RFX_Text(pFX,_T("TYPE_NAME"),m_strTypeName);
	RFX_Long(pFX,_T("PRECISION"),m_nPrecision);
	RFX_Long(pFX,_T("LENGTH"),m_nLength);
	RFX_Int(pFX,_T("SCALE"),m_nScale);
	RFX_Int(pFX,_T("RADIX"),m_nRadix);
	RFX_Int(pFX,_T("NULLABLE"),m_fNullable);
	RFX_Text(pFX,_T("REMARKS"),m_strRemarks);
}

//---------------------------------------------------------------------
// CProcedures

CProcedures::CProcedures(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_strProcedureQualifier = _T("");
	m_strProcedureOwner     = _T("");
	m_strProcedureName      = _T("");
	m_strRemarks            = _T("");
	m_fProcedureType        = 0;
	m_nFields = 5;
}

BOOL CProcedures::Open(LPCSTR pszProcQualifier,
	LPCSTR pszProcOwner,LPCSTR pszProcName,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLProcedures is supported
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLPROCEDURES,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLProcedures not supported\n"));
		return FALSE;
	}

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
		AFX_ODBC_CALL(::SQLProcedures(m_hstmt,
			(UCHAR FAR*)pszProcQualifier,SQL_NTS,
			(UCHAR FAR*)pszProcOwner,SQL_NTS,
			(UCHAR FAR*)pszProcName,SQL_NTS));
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

void CProcedures::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("PROCEDURE_QUALIFIER"),m_strProcedureQualifier);
	RFX_Text(pFX,_T("PROCEDURE_OWNER"),m_strProcedureOwner);
	RFX_Text(pFX,_T("PROCEDURE_NAME"),m_strProcedureName);
	RFX_Text(pFX,_T("REMARKS"),m_strRemarks);
	RFX_Int(pFX,_T("PROCEDURE_TYPE"),m_fProcedureType);
}

//---------------------------------------------------------------------
// CSpecialColumns

CSpecialColumns::CSpecialColumns(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_fScope        = 0;
	m_strColumnName = _T("");
	m_nDataType     = 0;
	m_strTypeName   = _T("");
	m_nPrecision    = 0;
	m_nLength       = 0;
	m_nScale        = 0;
	m_fPseudoColumn = 0;
	m_nFields = 8;
}

BOOL CSpecialColumns::Open(short fColType,LPCSTR pszTableQualifier,
	LPCSTR pszTableOwner,LPCSTR pszTableName,short fScope,short fNullable,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLSpecialColumns is supported
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLSPECIALCOLUMNS,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLSpecialColumns not supported\n"));
		return FALSE;
	}

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
		AFX_ODBC_CALL(::SQLSpecialColumns(m_hstmt,
			(UWORD)fColType,
			(UCHAR FAR*)pszTableQualifier,SQL_NTS,
			(UCHAR FAR*)pszTableOwner,SQL_NTS,
			(UCHAR FAR*)pszTableName,SQL_NTS,
			(UWORD)fScope,
			(UWORD)fNullable));
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

void CSpecialColumns::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX,_T("SCOPE"),m_fScope);
	RFX_Text(pFX,_T("COLUMN_NAME"),m_strColumnName);
	RFX_Int(pFX,_T("DATA_TYPE"),m_nDataType);
	RFX_Text(pFX,_T("TYPE_NAME"),m_strTypeName);
	RFX_Long(pFX,_T("PRECISION"),m_nPrecision);
	RFX_Long(pFX,_T("LENGTH"),m_nLength);
	RFX_Int(pFX,_T("SCALE"),m_nScale);
	RFX_Int(pFX,_T("PSEUDO_COLUMN"),m_fPseudoColumn);
}

//---------------------------------------------------------------------
// CStatistics

CStatistics::CStatistics(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_strTableQualifier     = _T("");
	m_strTableOwner         = _T("");
	m_strTableName          = _T("");
	m_fNonUnique            = 0;
	m_strIndexQualifier     = _T("");
	m_strIndexName          = _T("");
	m_fType                 = 0;
	m_nSeqInIndex           = 0;
	m_strColumnName         = _T("");
	m_strCollation          = _T("");
	m_nCardinality          = 0;
	m_nPages                = 0;
	m_strFilterCondition    = _T("");
	m_nFields = 13;
}

BOOL CStatistics::Open(LPCSTR pszTableQualifier,
	LPCSTR pszTableOwner,LPCSTR pszTableName,short fUnique,short fAccuracy,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLStatistics is supported
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLSTATISTICS,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLStatistics not supported\n"));
		return FALSE;
	}

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
		AFX_ODBC_CALL(::SQLStatistics(m_hstmt,
			(UCHAR FAR*)pszTableQualifier,SQL_NTS,
			(UCHAR FAR*)pszTableOwner,SQL_NTS,
			(UCHAR FAR*)pszTableName,SQL_NTS,
			(UWORD)fUnique,
			(UWORD)fAccuracy));
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

void CStatistics::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("TABLE_QUALIFIER"),m_strTableQualifier);
	RFX_Text(pFX,_T("TABLE_OWNER"),m_strTableOwner);
	RFX_Text(pFX,_T("TABLE_NAME"),m_strTableName);
	RFX_Int(pFX,_T("NON_UNIQUE"),m_fNonUnique);
	RFX_Text(pFX,_T("INDEX_QUALIFIER"),m_strIndexQualifier);
	RFX_Text(pFX,_T("INDEX_NAME"),m_strIndexName);
	RFX_Int(pFX,_T("TYPE"),m_fType);
	RFX_Int(pFX,_T("SEQ_IN_INDEX"),m_nSeqInIndex);
	RFX_Text(pFX,_T("COLUMN_NAME"),m_strColumnName);
	RFX_Text(pFX,_T("COLLATION"),m_strCollation);
	RFX_Long(pFX,_T("CARDINALITY"),m_nCardinality);
	RFX_Long(pFX,_T("PAGES"),m_nPages);
	RFX_Text(pFX,_T("FILTER_CONDITION"),m_strFilterCondition);
}

//---------------------------------------------------------------------
// CTablePrivileges

CTablePrivileges::CTablePrivileges(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_strTableQualifier = _T("");
	m_strTableOwner     = _T("");
	m_strTableName      = _T("");
	m_strGrantor        = _T("");
	m_strGrantee        = _T("");
	m_strPrivilege      = _T("");
	m_strIsGrantable    = _T("");
	m_nFields = 7;
}

BOOL CTablePrivileges::Open(LPCSTR pszTableQualifier,
	LPCSTR pszTableOwner,LPCSTR pszTableName,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLTablePrivileges exists
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLTABLEPRIVILEGES,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLTablePrivileges not supported\n"));
		return FALSE;
	}

	// Cache state info and allocate hstmt
	SetState(nOpenType,NULL,readOnly);
	if (!AllocHstmt())
		return FALSE;

	TRY
	{
		OnSetOptions(m_hstmt);
		AllocStatusArrays();

		// Call the ODBC function
		AFX_ODBC_CALL(::SQLTablePrivileges(m_hstmt,
			(UCHAR FAR*)pszTableQualifier,SQL_NTS,
			(UCHAR FAR*)pszTableOwner,SQL_NTS,
			(UCHAR FAR*)pszTableName,SQL_NTS));
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

void CTablePrivileges::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("TABLE_QUALIFIER"),m_strTableQualifier);
	RFX_Text(pFX,_T("TABLE_OWNER"),m_strTableOwner);
	RFX_Text(pFX,_T("TABLE_NAME"),m_strTableName);
	RFX_Text(pFX,_T("GRANTOR"),m_strGrantor);
	RFX_Text(pFX,_T("GRANTEE"),m_strGrantee);
	RFX_Text(pFX,_T("PRIVILEGE"),m_strPrivilege);
	RFX_Text(pFX,_T("IS_GRANTABLE"),m_strIsGrantable);
}

//---------------------------------------------------------------------
// CTables

CTables::CTables(CDatabase* pDatabase)
	: CRecordset(pDatabase)
{
	m_strTableQualifier = _T("");
	m_strTableOwner     = _T("");
	m_strTableName      = _T("");
	m_strTableType      = _T("");
	m_strRemarks        = _T("");
	m_nFields = 5;
}

BOOL CTables::Open(LPCSTR pszTableQualifier,
	LPCSTR pszTableOwner,LPCSTR pszTableName,LPCSTR pszTableType,
	UINT nOpenType)
{
	RETCODE nRetCode;
	UWORD   bFunctionExists;

	// Make sure SQLTables exists
	AFX_SQL_SYNC(::SQLGetFunctions(m_pDatabase->m_hdbc,
		SQL_API_SQLTABLES,&bFunctionExists));
	if (!Check(nRetCode) || !bFunctionExists)
	{
		if (!bFunctionExists)
			TRACE(_T("SQLTables not supported\n"));
		return FALSE;
	}

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
			(UCHAR FAR*)pszTableQualifier,SQL_NTS,
			(UCHAR FAR*)pszTableOwner,SQL_NTS,
			(UCHAR FAR*)pszTableName,SQL_NTS,
			(UCHAR FAR*)pszTableType,SQL_NTS));
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

void CTables::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX,_T("TABLE_QUALIFIER"),m_strTableQualifier);
	RFX_Text(pFX,_T("TABLE_OWNER"),m_strTableOwner);
	RFX_Text(pFX,_T("TABLE_NAME"),m_strTableName);
	RFX_Text(pFX,_T("TABLE_TYPE"),m_strTableType);
	RFX_Text(pFX,_T("REMARKS"),m_strRemarks);
}

