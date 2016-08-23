// MyDatabase.cpp: implementation of the CMyDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyDatabase.h"
//#include "catsets.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyDatabase::CMyDatabase()
{
	m_nParams=0;
}

CMyDatabase::~CMyDatabase()
{
	Clear();
}

/*
	Get number of parameters in the SQL statement, 
	alloc buffer for parameters, 
	get parameter type,length or other information.
*/

bool	CMyDatabase::PrepareSQL(CString strSQL)
{
	SQLRETURN retcode;
	SQLHSTMT hstmt;

	if(strSQL.GetLength()==0)
		return false;
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &hstmt);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		return false;
	}
	retcode = SQLPrepare(hstmt,(SQLCHAR *)strSQL.GetBuffer(strSQL.GetLength()),SQL_NTS);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		return false;
	}
	SQLSMALLINT nParams;
	retcode = SQLNumParams(hstmt, &nParams);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		return false;
	}
	class CSQLParam paramInfo;
	m_nParams=nParams;
	if(m_nParams==0)
	{
		SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
		return true;
	}
	SQLSMALLINT DataType, DecimalDigits, Nullable;
	SQLUINTEGER   ParamSize;
	SQLCHAR	SqlState[6],Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER	NativeError;
	SQLSMALLINT	n,MsgLen;
	SQLRETURN retcode2;
	m_mapParam.RemoveAll();
	for(UINT i=0;i<m_nParams;i++)
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
			return false;
		}
		paramInfo.m_nColType = 	SQL_PARAM_INPUT;
		paramInfo.m_nDataType = DataType;
		paramInfo.m_nLength = ParamSize;
		paramInfo.m_strParamName.Format(_T("%d"),i);
		paramInfo.m_nDecimalDigits = DecimalDigits;
		m_mapParam.SetAt(i,paramInfo);
	}
	SQLFreeHandle(SQL_HANDLE_STMT,hstmt);
	return true;
}
/*

bool	CMyDatabase::PrepareStoreProcedure(CString strStoreProcedureName)
{
	CProcedureColumns splParamSet(this);
	BOOL bRet;
	bRet=splParamSet.Open(NULL,NULL,strStoreProcedureName,NULL);
	if(!bRet)
	{
		AfxMessageBox("Failed to open:CMydatabase,PrepareStoreProcedure!");
		return false;
	}
	CString s1;
	int i=0;
	class CSQLParam paramInfo;
	m_mapParam.RemoveAll();
	while(!splParamSet.IsEOF())
	{
		paramInfo.m_strParamName = splParamSet.m_strColumnName;
		paramInfo.m_nColType = splParamSet.m_fColumnType;
		if(paramInfo.m_nColType == SQL_RETURN_VALUE)
			paramInfo.m_nColType = SQL_PARAM_OUTPUT;
		paramInfo.m_nDataType = splParamSet.m_nDataType ;
		paramInfo.m_nLength = splParamSet.m_nLength;
		paramInfo.m_nDecimalDigits = splParamSet.m_nScale;
		m_mapParam.SetAt(i++,paramInfo);
		splParamSet.MoveNext();
	}
	splParamSet.Close();
	m_nParams = UINT( m_mapParam.GetCount());
	return true;
}
*/

/*
	Reset parameters map and set number of parameters to 0.
*/

void CMyDatabase::Clear()
{
	m_mapParam.RemoveAll();
	m_nParams=0;
}

/*
	Execute the SQL statement and clear the parameters map
*/

void CMyDatabase::ExecuteSQL(CString strSql)
{
	if(strSql.GetLength()==0)
		return;
	
	CDatabase::ExecuteSQL(strSql);
}

bool CMyDatabase::SafeExecuteSQL(CString strSql)
{
	if(strSql.GetLength()==0)
		return false;
	try
	{
		ExecuteSQL(strSql);
	}
	catch(CDBException *E)
	{
#ifdef _DEBUG
		AfxMessageBox(strSql);
#endif
		AfxMessageBox(E->m_strError);
		return false;
	}
	return true;
}

/*
	This is an overrided function.
	Bind data buffer to each parameter when execute the SQL statement.
*/

void CMyDatabase::BindParameters(HSTMT hstmt)
{
	RETCODE retcode;
	m_nBufferLength=0;
	int nDataType,nColType;

	for(UINT i=0; i < m_nParams; i++)
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
			SQLCHAR	SqlState[6],Msg[SQL_MAX_MESSAGE_LENGTH];
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

void	CMyDatabase::SetParam_Bool(UINT nParamId,bool bParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_BIT)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);

	m_mapParam[nParamId].m_bVal = bParamValue;
}

void	CMyDatabase::SetParam_Char(UINT nParamId,char chParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_TINYINT)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_chVal = chParamValue;
}

void	CMyDatabase::SetParam_Short(UINT nParamId,short iParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_SMALLINT)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_iVal = iParamValue;
}

void	CMyDatabase::SetParam_Long(UINT nParamId,long nParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_INTEGER)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_lVal = nParamValue;
}

void	CMyDatabase::SetParam_Float(UINT nParamId,float fltParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_REAL)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_fltVal = fltParamValue;
}

void	CMyDatabase::SetParam_Double(UINT nParamId,double dblParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
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

void	CMyDatabase::SetParam_DateTime(UINT nParamId,TIMESTAMP_STRUCT dtParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_TIMESTAMP && m_mapParam[nParamId].m_nDataType != SQL_TYPE_TIMESTAMP
		&& m_mapParam[nParamId].m_nDataType != SQL_DATE && m_mapParam[nParamId].m_nDataType != SQL_TYPE_DATE)
	{
		ASSERT(FALSE);
		return;
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	m_mapParam[nParamId].m_dtVal.year = dtParamValue.year;
	m_mapParam[nParamId].m_dtVal.month = dtParamValue.month;
	m_mapParam[nParamId].m_dtVal.day = dtParamValue.day;
	m_mapParam[nParamId].m_dtVal.hour = dtParamValue.hour;
	m_mapParam[nParamId].m_dtVal.minute = dtParamValue.minute;
	m_mapParam[nParamId].m_dtVal.second = dtParamValue.second;
	m_mapParam[nParamId].m_dtVal.fraction = dtParamValue.fraction;
}

void	CMyDatabase::SetParam_String(UINT nParamId,CString strParamValue)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
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

bool	CMyDatabase::GetParam_Bool(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_BIT)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_bVal;
}

char	CMyDatabase::GetParam_Char(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_TINYINT)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_chVal;
}

short	CMyDatabase::GetParam_Short(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_SMALLINT)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_iVal;
}

long	CMyDatabase::GetParam_Long(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_INTEGER)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_lVal;
}

float	CMyDatabase::GetParam_Float(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_REAL)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_fltVal;
}

double	CMyDatabase::GetParam_Double(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_DOUBLE && m_mapParam[nParamId].m_nDataType != SQL_DECIMAL
		&& m_mapParam[nParamId].m_nDataType != SQL_NUMERIC && m_mapParam[nParamId].m_nDataType != SQL_FLOAT)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_dblVal;
}

TIMESTAMP_STRUCT	CMyDatabase::GetParam_DateTime(UINT nParamId)
{
	ASSERT(nParamId>=0);
	ASSERT(nParamId<m_nParams);
	if(m_mapParam[nParamId].m_nDataType != SQL_TIMESTAMP && m_mapParam[nParamId].m_nDataType != SQL_TYPE_TIMESTAMP
		&& m_mapParam[nParamId].m_nDataType != SQL_DATE && m_mapParam[nParamId].m_nDataType != SQL_TYPE_DATE)
	{
		ASSERT(FALSE);
	}
	if(m_mapParam[nParamId].m_nColType != SQL_PARAM_OUTPUT && m_mapParam[nParamId].m_nColType != SQL_PARAM_INPUT_OUTPUT)
		ASSERT(FALSE);
	return m_mapParam[nParamId].m_dtVal;
}
