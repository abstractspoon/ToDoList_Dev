// DatabaseEx.cpp: implementation of the CDatabaseEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DatabaseEx.h"

#include <ATLCONV.H>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static const CString DSN_TAG(_T("DSN="));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDatabaseEx::CDatabaseEx()
{

}

CDatabaseEx::~CDatabaseEx()
{
	Clear();
}

CString CDatabaseEx::GetDSN() const
{
	return GetDSN(GetConnect());
}

CString CDatabaseEx::GetDSN(const CString& sConnect)
{
	CString sDsn, sConnectUpper(sConnect);

	sConnectUpper.MakeUpper();

	int nStartDsn = sConnectUpper.Find(DSN_TAG);
	ASSERT(nStartDsn != -1);

	if (nStartDsn != -1)
	{
		int nEndDsn = sConnectUpper.Find(';', nStartDsn);
		ASSERT(nEndDsn != -1);

		if (nEndDsn != -1)
		{
			nStartDsn += DSN_TAG.GetLength();

			sDsn = sConnect.Mid(nStartDsn, nEndDsn - nStartDsn);
		}
	}

	return sDsn;
}

BOOL CDatabaseEx::Connect(const CString& sConnect)
{
	return CDatabaseEx::Open(NULL, FALSE, FALSE, sConnect);
}

CString CDatabaseEx::FormatName(DBEX_FORMAT nFmt) const
{
	return FormatName(nFmt, GetDSN(), GetDatabaseName());
}

CString CDatabaseEx::FormatName(DBEX_FORMAT nFmt, const CString& sDSN, const CString& sDB)
{
	CString sName;

	switch (nFmt)
	{
	case DBE_FMT_DB_DSN:
		sName.Format(_T("%s [%s]"), sDB, sDSN);
		break;

	case DBE_FMT_DSN_DB:
		sName.Format(_T("%s [%s]"), sDSN, sDB);
		break;

	default:
		ASSERT(0);
		break;
	}

	return sName;
}

void CDatabaseEx::Clear()
{
	m_mapParam.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
//
// PARTS MERGED FROM CDatabaseEx
//
// Author:		lindapeng ( lindapeng@163.net )
//
// Description:	This class is especially for working with parameterized sql and store procedures
//				which do not return record sets.
//
// Using:		(1) Prepare the SQL statement or stored procedure,such as PrepareSQL("insert into table values(?,?)")
//					or PrepareStoreProcedure ("procedure name").
//				(2) Set value for each input parameters, such as SetParam_Long(0,100),SetParam_Double(1,12.34).
//					Keep in mind the field number of stored procedure is starting from 1 not 0, because the 0 is 
//					for return value from store procedure.
//				(3) Execute the statement, such as ExecuteSql("insert into table values(?,?)") or ExecuteSql("{? = call procudure(?,?)}").
//				(4) Get output value from stored procedure, such as retvalue = GetParam_Long(0).
//
// Rights:		It can be used or distributed or modified freely for personal use. If you think it helps you with your
//				work, it will be nice to let me know with a mail.
//////////////////////////////////////////////////////////////////////

// 	Get number of parameters in the SQL statement, 
// 	alloc buffer for parameters, 
// 	get parameter type,length or other information.

BOOL CDatabaseEx::PrepareSQL(const CString& strSQL)
{
	SQLRETURN retcode;
	SQLHSTMT hstmt;

	if(strSQL.GetLength()==0)
		return FALSE;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &hstmt);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		return FALSE;
	}

#if _MSC_VER >= 1400
	SQLTCHAR* szSQL = (SQLTCHAR*)(LPCTSTR)strSQL;
#else
	USES_CONVERSION;
	SQLCHAR* szSQL = (SQLCHAR*)T2CA(strSQL);
#endif

	retcode = SQLPrepare(hstmt, szSQL, SQL_NTS);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		return FALSE;
	}

	SQLSMALLINT nParams;
	retcode = SQLNumParams(hstmt, &nParams);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		return FALSE;
	}

	if (nParams == 0)
	{
		m_mapParam.RemoveAll();
		SQLFreeHandle(SQL_HANDLE_STMT,hstmt);

		return TRUE;
	}

#if _MSC_VER >= 1400
	SQLTCHAR	SqlState[6],Msg[SQL_MAX_MESSAGE_LENGTH];
#else
	SQLCHAR		SqlState[6],Msg[SQL_MAX_MESSAGE_LENGTH];
#endif
	SQLSMALLINT DataType, DecimalDigits, Nullable;
	SQLUINTEGER   ParamSize;
	SQLINTEGER	NativeError;
	SQLSMALLINT	n,MsgLen;
	SQLRETURN retcode2;

	CSQLParam paramInfo;

	for(SQLSMALLINT i=0;i<nParams;i++)
	{
		retcode=SQLDescribeParam(hstmt, i+1, &DataType, &ParamSize, &DecimalDigits, &Nullable);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			n = 1;
			CString strMsg;
			while ((retcode2 = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, n, SqlState, &NativeError,Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA) 
			{
				strMsg=Msg;
				AfxMessageBox(strMsg);
				n++;
			}
			SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
			return FALSE;
		}
		paramInfo.m_nColType = 	SQL_PARAM_INPUT;
		paramInfo.m_nDataType = DataType;
		paramInfo.m_nLength = ParamSize;
		paramInfo.m_strParamName.Format(_T("%d"),i);
		paramInfo.m_nDecimalDigits = DecimalDigits;
		m_mapParam.SetAt(i,paramInfo);
	}
	SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	return TRUE;
}

BOOL CDatabaseEx::ExecuteSQL(const CString& strSql)
{
	if (strSql.IsEmpty())
		return FALSE;
	
	try
	{
		CDatabase::ExecuteSQL(strSql);
		return TRUE;
	}
	catch (CDBException* e)
	{
#ifdef _DEBUG
		AfxMessageBox(e->m_strError, MB_ICONEXCLAMATION);
#endif
		// cleanup
		e->Delete();
	}
	catch (...)
	{
	}
	
	return FALSE;
}

BOOL CDatabaseEx::PrepareAndExecuteSQL(const CString& strSql, const CDBVariantArray& aVars)
{
	if (strSql.IsEmpty())
		return FALSE;

	if (!PrepareSQL(strSql))
		return FALSE;

	if (!SetParams(aVars))
		return FALSE;
	
	return ExecuteSQL(strSql);
}

void CDatabaseEx::BindParameters(HSTMT hstmt)
{
	RETCODE retcode;
	m_nBufferLength=0;
	int nDataType,nColType;

	for(UINT i=0; i < NumParams(); i++)
	{
		nDataType = m_mapParam[i].m_nDataType;
		nColType = m_mapParam[i].m_nColType;
		switch( nDataType )
		{
		case SQL_BIT:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_BIT,SQL_BIT, 0, 0, &(m_mapParam[i].m_bVal), 0, &m_nBufferLength);
				break;
		case SQL_TINYINT:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_STINYINT,SQL_TINYINT, 0, 0, &(m_mapParam[i].m_chVal), 0, &m_nBufferLength);
				break;
		case SQL_SMALLINT:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_SSHORT,SQL_SMALLINT, 0, 0, &(m_mapParam[i].m_iVal), 0, &m_nBufferLength);
				break;
		case SQL_INTEGER:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_SLONG,SQL_INTEGER, 0, 0, &(m_mapParam[i].m_lVal), 0, &m_nBufferLength);
				break;
		case SQL_REAL:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_FLOAT,SQL_REAL, 0, m_mapParam[i].m_nDecimalDigits, &(m_mapParam[i].m_fltVal), 0, &m_nBufferLength);
				break;
		case SQL_FLOAT:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_DOUBLE,SQL_FLOAT, 0, m_mapParam[i].m_nDecimalDigits, &(m_mapParam[i].m_dblVal), 0, &m_nBufferLength);
				break;
		case SQL_DOUBLE:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_DOUBLE,SQL_DOUBLE, 0, m_mapParam[i].m_nDecimalDigits, &(m_mapParam[i].m_dblVal), 0, &m_nBufferLength);
				break;
		case SQL_DECIMAL:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_DOUBLE,SQL_DECIMAL, 0, m_mapParam[i].m_nDecimalDigits, &(m_mapParam[i].m_dblVal), 0, &m_nBufferLength);
				break;
		case SQL_NUMERIC:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_DOUBLE,SQL_NUMERIC, 0, m_mapParam[i].m_nDecimalDigits, &(m_mapParam[i].m_dblVal), 0, &m_nBufferLength);
				break;
		case SQL_CHAR:
				m_nBufferLength=SQL_NTS;
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_CHAR,SQL_CHAR, m_mapParam[i].m_nLength, 0, (SQLPOINTER)((LPCTSTR)m_mapParam[i].m_strVal), 0, &m_nBufferLength);
				break;
		case SQL_WCHAR:
				m_nBufferLength=SQL_NTS;
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_CHAR,SQL_WCHAR, m_mapParam[i].m_nLength, 0, (SQLPOINTER)((LPCTSTR)m_mapParam[i].m_strVal), 0, &m_nBufferLength);
				break;
		case SQL_VARCHAR:
				m_nBufferLength=SQL_NTS;
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_CHAR,SQL_VARCHAR, m_mapParam[i].m_nLength, 0, (SQLPOINTER)((LPCTSTR)m_mapParam[i].m_strVal), 0, &m_nBufferLength);
				break;
		case SQL_WVARCHAR:
				m_nBufferLength=SQL_NTS;
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_CHAR,SQL_WVARCHAR, m_mapParam[i].m_nLength, 0, (SQLPOINTER)((LPCTSTR)m_mapParam[i].m_strVal), 0, &m_nBufferLength);
				break;
		case SQL_DATE:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_TIMESTAMP,SQL_DATE, 0, 0, &(m_mapParam[i].m_dtVal), 0, &m_nBufferLength);
				break;
		case SQL_TYPE_DATE:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_TIMESTAMP,SQL_TYPE_DATE, 0, 0, &(m_mapParam[i].m_dtVal), 0, &m_nBufferLength);
				break;
		case SQL_TIMESTAMP:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_TIMESTAMP,SQL_TIMESTAMP, 0, 0, &(m_mapParam[i].m_dtVal), 0, &m_nBufferLength);
				break;
		case SQL_TYPE_TIMESTAMP:
				retcode = SQLBindParameter(hstmt, i+1, nColType, SQL_C_TIMESTAMP,SQL_TYPE_TIMESTAMP, 0, 0, &(m_mapParam[i].m_dtVal), 0, &m_nBufferLength);
				break;
		default:
				ASSERT(FALSE);
				retcode = -1;
				break;
		}
		if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
#if _MSC_VER >= 1400
			SQLTCHAR	SqlState[6],Msg[SQL_MAX_MESSAGE_LENGTH];
#else
			SQLCHAR		SqlState[6],Msg[SQL_MAX_MESSAGE_LENGTH];
#endif
			SQLINTEGER	NativeError;
			SQLSMALLINT	n,MsgLen;
			SQLRETURN retcode2;
			n = 1;
			CString strMsg;
			while ((retcode2 = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, n, SqlState, &NativeError,Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA) 
			{
				strMsg=Msg;
				AfxMessageBox(strMsg);
				n++;
			}
			return;
		}
	}
}

/*
	The following functions set data for each input parameter,
	or get data from each output parameter.
	There is no tranformation between different types,
	so the data type and the parameter type must be equal.
*/

void CDatabaseEx::SetParam(UINT nParamId,bool bParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_BIT)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);

	m_mapParam[nParamId].m_bVal = bParamValue;
}

void CDatabaseEx::SetParam(UINT nParamId,char chParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_TINYINT)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_chVal = chParamValue;
}

void CDatabaseEx::SetParam(UINT nParamId,short iParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_SMALLINT)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_iVal = iParamValue;
}

void CDatabaseEx::SetParam(UINT nParamId,long nParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_INTEGER)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_lVal = nParamValue;
}

void CDatabaseEx::SetParam(UINT nParamId,float fltParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_REAL)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_fltVal = fltParamValue;
}

void CDatabaseEx::SetParam(UINT nParamId,double dblParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_DOUBLE && m_mapParam[nParamId].m_nDataType != SQL_DECIMAL
		&& m_mapParam[nParamId].m_nDataType != SQL_NUMERIC && m_mapParam[nParamId].m_nDataType != SQL_FLOAT)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_dblVal = dblParamValue;
}

void CDatabaseEx::SetParam(UINT nParamId,TIMESTAMP_STRUCT dtParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_TIMESTAMP && m_mapParam[nParamId].m_nDataType != SQL_TYPE_TIMESTAMP
		&& m_mapParam[nParamId].m_nDataType != SQL_DATE && m_mapParam[nParamId].m_nDataType != SQL_TYPE_DATE)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);

	m_mapParam[nParamId].m_dtVal = dtParamValue;
}

void CDatabaseEx::SetParam(UINT nParamId,const CString& strParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_CHAR && m_mapParam[nParamId].m_nDataType != SQL_VARCHAR
		&& m_mapParam[nParamId].m_nDataType != SQL_WCHAR && m_mapParam[nParamId].m_nDataType != SQL_WVARCHAR)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_strVal = strParamValue;
}

BOOL CDatabaseEx::SetParam(UINT nParamId, const CDBVariant& var)
{
	switch (var.m_dwType)
	{
	case DBVT_BOOL:		SetParam(nParamId, (var.m_boolVal != FALSE));	break;
	case DBVT_UCHAR:	SetParam(nParamId, (char)var.m_chVal);			break;
	case DBVT_SHORT:	SetParam(nParamId, var.m_iVal); 				break; 
	case DBVT_LONG:		SetParam(nParamId, var.m_lVal);					break;   
	case DBVT_SINGLE:	SetParam(nParamId, var.m_fltVal);				break; 
	case DBVT_DOUBLE:	SetParam(nParamId, var.m_dblVal);				break; 
	case DBVT_DATE:		SetParam(nParamId, *(var.m_pdate));				break;   
	case DBVT_STRING:	SetParam(nParamId, *(var.m_pstring));			break; 

	case DBVT_BINARY: // not currently supported
	default:
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CDatabaseEx::SetParams(const CDBVariantArray& aParams)
{
	UINT nParam = aParams.GetSize();

	ASSERT(nParam == NumParams());
	
	if (nParam != NumParams())
		return FALSE;

	// set parameter values
	while (nParam--)
		SetParam(nParam, *(aParams[nParam]));
	
	return TRUE;
}

// ---------------------------------------------------------------------------------------

bool CDatabaseEx::GetParam_Bool(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_BIT)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_bVal;
}

char CDatabaseEx::GetParam_Char(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_TINYINT)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_chVal;
}

short CDatabaseEx::GetParam_Short(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_SMALLINT)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_iVal;
}

long CDatabaseEx::GetParam_Long(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_INTEGER)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_lVal;
}

float CDatabaseEx::GetParam_Float(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_REAL)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_fltVal;
}

double CDatabaseEx::GetParam_Double(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_DOUBLE && m_mapParam[nParamId].m_nDataType != SQL_DECIMAL
		&& m_mapParam[nParamId].m_nDataType != SQL_NUMERIC && m_mapParam[nParamId].m_nDataType != SQL_FLOAT)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_dblVal;
}

TIMESTAMP_STRUCT CDatabaseEx::GetParam_DateTime(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<NumParams());
	if(m_mapParam[nParamId].m_nDataType != SQL_TIMESTAMP && m_mapParam[nParamId].m_nDataType != SQL_TYPE_TIMESTAMP
		&& m_mapParam[nParamId].m_nDataType != SQL_DATE && m_mapParam[nParamId].m_nDataType != SQL_TYPE_DATE)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_dtVal;
}
