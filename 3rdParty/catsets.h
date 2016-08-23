// catsets.h
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef CATSETS_H
#define CATSETS_H

//---------------------------------------------------------------------
// CColumnPrivileges - results from SQLColumnPrivileges()

class CColumnPrivileges : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CColumnPrivileges(CDatabase* pDatabase);
	BOOL            Open(LPCSTR pszTableQualifier = NULL,
						 LPCSTR pszTableOwner = NULL,
						 LPCSTR pszTableName = NULL,
						 LPCSTR pszColumnName = NULL,
						 UINT   nOpenType = forwardOnly);

	CString         m_strTableQualifier;
	CString         m_strTableOwner;
	CString         m_strTableName;
	CString         m_strColumnName;
	CString         m_strGrantor;
	CString         m_strGrantee;
	CString         m_strPrivilege;
	CString         m_strIsGrantable;

	virtual void    DoFieldExchange(CFieldExchange*);
};

//---------------------------------------------------------------------
// CColumns - results from SQLColumns()

class CColumns : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CColumns(CDatabase* pDatabase);
	BOOL            Open(LPCSTR pszTableQualifier = NULL,
						 LPCSTR pszTableOwner = NULL,
						 LPCSTR pszTableName = NULL,
						 LPCSTR pszColumnName = NULL,
						 UINT   nOpenType = forwardOnly);

	CString         m_strTableQualifier;
	CString         m_strTableOwner;
	CString         m_strTableName;
	CString         m_strColumnName;
	int             m_nDataType;
	CString         m_strTypeName;
	long            m_nPrecision;
	long            m_nLength;
	int             m_nScale;
	int             m_nRadix;
	int             m_fNullable;
	CString         m_strRemarks;

	virtual void    DoFieldExchange(CFieldExchange*);
};

//---------------------------------------------------------------------
// CForeignKeys - results from SQLForeignKeys()

class CForeignKeys : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CForeignKeys(CDatabase* pDatabase);
	BOOL            Open(LPCSTR pszPkTableQualifier,
						 LPCSTR pszPkTableOwner,
						 LPCSTR pszPkTableName,
						 LPCSTR pszFkTableQualifier = NULL,
						 LPCSTR pszFkTableOwner = NULL,
						 LPCSTR pszFkTableName = NULL,
						 UINT   nOpenType = forwardOnly);

	CString         m_strPkTableQualifier;
	CString         m_strPkTableOwner;
	CString         m_strPkTableName;
	CString         m_strPkColumnName;
	CString         m_strFkTableQualifier;
	CString         m_strFkTableOwner;
	CString         m_strFkTableName;
	CString         m_strFkColumnName;
	int             m_nKeySeq;
	int             m_fUpdateRule;
	int             m_fDeleteRule;
	CString         m_strFkName;
	CString         m_strPkName;

	virtual void    DoFieldExchange(CFieldExchange*);
};

//---------------------------------------------------------------------
// CGetTypeInfo - results from SQLGetTypeInfo()

class CGetTypeInfo : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CGetTypeInfo(CDatabase* pDatabase);
	BOOL            Open(short fSqlType = SQL_ALL_TYPES,
						 UINT  nOpenType = forwardOnly);

	CString         m_strTypeName;
	int             m_fDataType;
	long            m_nPrecision;
	CString         m_strLiteralPrefix;
	CString         m_strLiteralSuffix;
	CString         m_strCreateParams;
	int             m_fNullable;
	BOOL            m_bCaseSensitive;
	int             m_fSearchable;
	int             m_fUnsignedAttribute;
	BOOL            m_bMoney;
	int             m_fAutoIncrement;
	CString         m_strLocalTypeName;
	int             m_nMinimumScale;
	int             m_nMaximumScale;

	virtual void    DoFieldExchange(CFieldExchange*);
};

//---------------------------------------------------------------------
// CPrimaryKeys - results from SQLPrimaryKeys()

class CPrimaryKeys : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CPrimaryKeys(CDatabase* pDatabase);
	BOOL            Open(LPCSTR pszTableQualifier = NULL,
						 LPCSTR pszTableOwner = NULL,
						 LPCSTR pszTableName = NULL,
						 UINT   nOpenType = forwardOnly);

	CString         m_strTableQualifier;
	CString         m_strTableOwner;
	CString         m_strTableName;
	CString         m_strColumnName;
	int             m_nKeySeq;
	CString         m_strPkName;

	virtual void    DoFieldExchange(CFieldExchange*);
};

//---------------------------------------------------------------------
// CProcedureColumns - results from SQLProcedureColumns()

class CProcedureColumns : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CProcedureColumns(CDatabase* pDatabase);
	BOOL            Open(LPCSTR pszProcQualifier = NULL,
						 LPCSTR pszProcOwner = NULL,
						 LPCSTR pszProcName = NULL,
						 LPCSTR pszColumnName = NULL,
						 UINT   nOpenType = forwardOnly);

	CString         m_strProcedureQualifier;
	CString         m_strProcedureOwner;
	CString         m_strProcedureName;
	CString         m_strColumnName;
	int             m_fColumnType;
	int             m_nDataType;
	CString         m_strTypeName;
	long            m_nPrecision;
	long            m_nLength;
	int             m_nScale;
	int             m_nRadix;
	int             m_fNullable;
	CString         m_strRemarks;

	virtual void    DoFieldExchange(CFieldExchange*);
};

//---------------------------------------------------------------------
// CProcedures - results from SQLProcedures()

class CProcedures : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CProcedures(CDatabase* pDatabase);
	BOOL            Open(LPCSTR pszProcQualifier = NULL,
						 LPCSTR pszProcOwner = NULL,
						 LPCSTR pszProcName = NULL,
						 UINT   nOpenType = forwardOnly);

	CString         m_strProcedureQualifier;
	CString         m_strProcedureOwner;
	CString         m_strProcedureName;
	CString         m_strRemarks;
	int             m_fProcedureType;

	virtual void    DoFieldExchange(CFieldExchange*);
};

//---------------------------------------------------------------------
// CSpecialColumns - results from SQLSpecialColumns()

class CSpecialColumns : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CSpecialColumns(CDatabase* pDatabase);
	BOOL            Open(short fColType = SQL_BEST_ROWID,
						 LPCSTR pszTableQualifier = NULL,
						 LPCSTR pszTableOwner = NULL,
						 LPCSTR pszTableName = NULL,
						 short fScope = SQL_SCOPE_SESSION,
						 short fNullable = SQL_NULLABLE,
						 UINT  nOpenType = forwardOnly);

	int             m_fScope;
	CString         m_strColumnName;
	int             m_nDataType;
	CString         m_strTypeName;
	long            m_nPrecision;
	long            m_nLength;
	int             m_nScale;
	int             m_fPseudoColumn;

	virtual void    DoFieldExchange(CFieldExchange*);
};

//---------------------------------------------------------------------
// CStatistics - results from SQLStatistics()

class CStatistics : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CStatistics(CDatabase* pDatabase);
	BOOL            Open(LPCSTR pszTableQualifier = NULL,
						 LPCSTR pszTableOwner = NULL,
						 LPCSTR pszTableName = NULL,
						 short fUnique = SQL_INDEX_ALL,
						 short fAccuracy = SQL_ENSURE,
						 UINT  nOpenType = forwardOnly);

	CString         m_strTableQualifier;
	CString         m_strTableOwner;
	CString         m_strTableName;
	int             m_fNonUnique;
	CString         m_strIndexQualifier;
	CString         m_strIndexName;
	int             m_fType;
	int             m_nSeqInIndex;
	CString         m_strColumnName;
	CString         m_strCollation;
	long            m_nCardinality;
	long            m_nPages;
	CString         m_strFilterCondition;

	virtual void    DoFieldExchange(CFieldExchange*);
};

//---------------------------------------------------------------------
// CTablePrivileges - results from SQLTablePrivileges()

class CTablePrivileges : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CTablePrivileges(CDatabase* pDatabase);
	BOOL            Open(LPCSTR pszTableQualifier = NULL,
						 LPCSTR pszTableOwner = NULL,
						 LPCSTR pszTableName = NULL,
						 UINT   nOpenType = forwardOnly);

	CString         m_strTableQualifier;
	CString         m_strTableOwner;
	CString         m_strTableName;
	CString         m_strGrantor;
	CString         m_strGrantee;
	CString         m_strPrivilege;
	CString         m_strIsGrantable;

	virtual void    DoFieldExchange(CFieldExchange*);
};

//---------------------------------------------------------------------
// CTables - results from SQLTables

class CTables : public CRecordset
{
private:
	using CRecordset::Open;

protected:
	virtual CString GetDefaultConnect() { return ""; }
	virtual CString GetDefaultSQL() { return ""; }

public:
					CTables(CDatabase* pDatabase);
	BOOL            Open(LPCSTR pszTableQualifier = NULL,
						 LPCSTR pszTableOwner = NULL,
						 LPCSTR pszTableName = NULL,
						 LPCSTR pszTableType = NULL,
						 UINT   nOpenType = forwardOnly);

	CString         m_strTableQualifier;
	CString         m_strTableOwner;
	CString         m_strTableName;
	CString         m_strTableType;
	CString         m_strRemarks;

	virtual void    DoFieldExchange(CFieldExchange*);
};

#endif
