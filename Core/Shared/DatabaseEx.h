// DatabaseEx.h: interface for the CDatabaseEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASEEX_H__E3B1F9B2_526C_431A_B983_E0F2DB08A8DF__INCLUDED_)
#define AFX_DATABASEEX_H__E3B1F9B2_526C_431A_B983_E0F2DB08A8DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxdb.h>
#include <afxtempl.h>

enum DBEX_FORMAT
{
	DBE_FMT_DSN_DB,
	DBE_FMT_DB_DSN,
};

class CDBVariantArray : public CArray<CDBVariant*, CDBVariant*&>
{
public:
	virtual ~CDBVariantArray()
	{
		int nVar = GetSize();

		while (nVar--)
			delete GetAt(nVar);

		RemoveAll();
	}
};

class CDatabaseEx : public CDatabase  
{
public:
	CDatabaseEx();
	virtual ~CDatabaseEx();

	CString GetDSN() const;
	static CString GetDSN(const CString& sConnect);

	CString FormatName(DBEX_FORMAT nFmt = DBE_FMT_DSN_DB) const;
	static CString FormatName(DBEX_FORMAT nFmt, const CString& sDSN, const CString& sDB);

	BOOL Connect(const CString& sConnect);
	
//////////////////////////////////////////////////////////////////////
//
// PARTS MERGED FROM CMyDatabase
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

	void NewQuery() { Clear(); }

	BOOL PrepareSQL(const CString& strSql);
	BOOL ExecuteSQL(const CString& strSql);
	BOOL PrepareAndExecuteSQL(const CString& strSql, const CDBVariantArray& aVars);
	
	void SetParam(UINT nParamId, bool bParamValue);
	void SetParam(UINT nParamId, char chParamValue);
	void SetParam(UINT nParamId, short iParamValue);
	void SetParam(UINT nParamId, long nParamValue);
	void SetParam(UINT nParamId, float fltParamValue);
	void SetParam(UINT nParamId, double dblParamValue);
	void SetParam(UINT nParamId, const CString& strParamValue);
	void SetParam(UINT nParamId, TIMESTAMP_STRUCT dtParamValue);

	BOOL SetParam(UINT nParamId, const CDBVariant& var);
	BOOL SetParams(const CDBVariantArray& aVars);
	
	bool GetParam_Bool(UINT nParamId);
	char GetParam_Char(UINT nParamId);
	short GetParam_Short(UINT nParamId);
	long GetParam_Long(UINT nParamId);
	float GetParam_Float(UINT nParamId);
	double GetParam_Double(UINT nParamId);
	TIMESTAMP_STRUCT GetParam_DateTime(UINT nParamId);
	
protected:
	void Clear();
	void BindParameters( HSTMT hstmt );
	UINT NumParams() const { return m_mapParam.GetCount(); }
	
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
		
	SQLINTEGER m_nBufferLength;
	CMap<int,int,CSQLParam,CSQLParam> m_mapParam;

// END PARTS MERGED FROM CMyDatabase //////////////////////////////////
};

#endif // !defined(AFX_DATABASEEX_H__E3B1F9B2_526C_431A_B983_E0F2DB08A8DF__INCLUDED_)
