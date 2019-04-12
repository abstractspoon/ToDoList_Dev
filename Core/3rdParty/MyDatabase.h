// MyDatabase.h: interface for the CMyDatabase class.
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

#if !defined(AFX_MYDATABASE_H__8473C75E_2CA3_4439_8DA1_E1C973E89C8A__INCLUDED_)
#define AFX_MYDATABASE_H__8473C75E_2CA3_4439_8DA1_E1C973E89C8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxdb.h>
#include <afxtempl.h>

class CSQLParam
{
public:
	CString		m_strParamName;
	int			m_nColType;
	int			m_nDataType;
	int			m_nLength;
	int			m_nDecimalDigits;

	bool		m_bVal;
	char		m_chVal;
	short		m_iVal;
	long		m_lVal;
	float		m_fltVal;
	double		m_dblVal;
	CString		m_strVal;
	TIMESTAMP_STRUCT m_dtVal;
};

class CMyDatabase : public CDatabase  
{
public:
	CMyDatabase();
	virtual ~CMyDatabase();

	bool		PrepareSQL(CString strSql);
//	bool		PrepareStoreProcedure(CString strStoreProcedureName);
	void		ExecuteSQL(CString strSql);
	bool		SafeExecuteSQL(CString strSql);

	void		SetParam_Bool(UINT nParamId,bool bParamValue);
	void		SetParam_Char(UINT nParamId,char chParamValue);
	void		SetParam_Short(UINT nParamId,short iParamValue);
	void		SetParam_Long(UINT nParamId,long nParamValue);
	void		SetParam_Float(UINT nParamId,float fltParamValue);
	void		SetParam_Double(UINT nParamId,double dblParamValue);
	void		SetParam_String(UINT nParamId,CString strParamValue);
	void		SetParam_DateTime(UINT nParamId,TIMESTAMP_STRUCT dtParamValue);

	bool		GetParam_Bool(UINT nParamId);
	char		GetParam_Char(UINT nParamId);
	short		GetParam_Short(UINT nParamId);
	long		GetParam_Long(UINT nParamId);
	float		GetParam_Float(UINT nParamId);
	double		GetParam_Double(UINT nParamId);
	TIMESTAMP_STRUCT	GetParam_DateTime(UINT nParamId);

private:
	void		Clear();
	void		BindParameters( HSTMT hstmt );

	UINT		m_nParams;
	SQLINTEGER	m_nBufferLength;
	CMap<int,int,CSQLParam,CSQLParam> m_mapParam;
};

#endif // !defined(AFX_MYDATABASE_H__8473C75E_2CA3_4439_8DA1_E1C973E89C8A__INCLUDED_)
